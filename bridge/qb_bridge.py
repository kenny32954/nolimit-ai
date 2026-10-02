#!/usr/bin/env python3
"""Quantum Breaks AI serial bridge for the Casio fx-CG50.

Protocol v0.3:
    Handshake:
        CG50 -> PC: H:QBAI:3
        PC -> CG50: K:QBAI:3

    Legacy request (v0.1):
        Q:<id>:<base64 UTF-8 prompt>

    Subject-aware request (v0.2):
        Q:<id>:<subject>:<mode>:<base64 UTF-8 prompt>

    College-capable request (v0.3):
        Q:<id>:<subject>:<mode>:<level>:<base64 UTF-8 prompt>

    PC response:
        A:<id>:<seq>:<done 0|1>:<base64 UTF-8 chunk>

    PC error:
        E:<id>:<base64 UTF-8 error>

The API endpoint is any OpenAI-compatible /chat/completions endpoint.
"""

import base64
import json
import os
import sys
import time
import urllib.error
import urllib.request

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    print("Missing dependency: pyserial")
    print("Install with: python -m pip install pyserial")
    raise

from subject_router import (
    build_system_prompt,
    normalize_level,
    normalize_mode,
    resolve_subject,
    subject_label,
)

BAUD = int(os.getenv("QB_SERIAL_BAUD", "115200"))
PORT = os.getenv("QB_SERIAL_PORT", "")
API_URL = os.getenv("QB_API_URL", "https://openrouter.ai/api/v1/chat/completions")
API_KEY = os.getenv("QB_API_KEY", "")
MODEL = os.getenv("QB_MODEL", "")
SYSTEM_PROMPT = os.getenv(
    "QB_SYSTEM_PROMPT",
    "You are Quantum Breaks AI, an academic tutor running through a Casio fx-CG50. "
    "You can handle secondary-school through advanced undergraduate work. "
    "Be accurate, rigorous, concise, and clear."
)
MAX_HISTORY_MESSAGES = int(os.getenv("QB_HISTORY_MESSAGES", "12"))
CHUNK_BYTES = max(32, int(os.getenv("QB_CHUNK_BYTES", "144")))

# Separate histories by subject + academic level so a proof-heavy linear algebra
# session does not contaminate a simpler school-level session.
histories = {}


def b64e(text):
    return base64.b64encode(text.encode("utf-8")).decode("ascii")


def b64d(text):
    return base64.b64decode(text.encode("ascii"), validate=True).decode("utf-8")


def send_line(ser, line):
    ser.write((line + "\n").encode("ascii"))
    ser.flush()


def extract_text(payload):
    choices = payload.get("choices") or []
    if not choices:
        return ""

    message = choices[0].get("message") or {}
    content = message.get("content", "")

    if isinstance(content, str):
        return content

    if isinstance(content, list):
        parts = []
        for item in content:
            if isinstance(item, dict) and item.get("type") in ("text", "output_text"):
                parts.append(str(item.get("text", "")))
        return "".join(parts)

    return str(content)


def parse_request(line):
    line = line.strip()
    if not line:
        return None

    # v0.1: Q:<id>:<payload>
    if line.startswith("Q:") and line.count(":") == 2:
        parts = line.split(":", 2)
        return {
            "id": parts[1],
            "subject": "auto",
            "mode": "explain",
            "level": "auto",
            "prompt": b64d(parts[2]).strip(),
        }

    # v0.2: Q:<id>:<subject>:<mode>:<payload>
    if line.startswith("Q:") and line.count(":") == 4:
        parts = line.split(":", 4)
        return {
            "id": parts[1],
            "subject": parts[2],
            "mode": parts[3],
            "level": "auto",
            "prompt": b64d(parts[4]).strip(),
        }

    # v0.3: Q:<id>:<subject>:<mode>:<level>:<payload>
    if line.startswith("Q:") and line.count(":") == 5:
        parts = line.split(":", 5)
        return {
            "id": parts[1],
            "subject": parts[2],
            "mode": parts[3],
            "level": parts[4],
            "prompt": b64d(parts[5]).strip(),
        }

    if line.startswith("Q:"):
        raise RuntimeError("Malformed request frame.")

    return None


def call_model(
    prompt,
    requested_subject="auto",
    requested_mode="explain",
    requested_level="auto",
):
    subject = resolve_subject(requested_subject, prompt)
    mode = normalize_mode(requested_mode)
    level = normalize_level(requested_level)
    history_key = (subject, level)
    history = histories.get(history_key, [])

    messages = [{
        "role": "system",
        "content": build_system_prompt(SYSTEM_PROMPT, subject, mode, level),
    }]
    messages.extend(history[-MAX_HISTORY_MESSAGES:])
    messages.append({"role": "user", "content": prompt})

    body = json.dumps({
        "model": MODEL,
        "messages": messages,
    }).encode("utf-8")

    request = urllib.request.Request(
        API_URL,
        data=body,
        method="POST",
        headers={
            "Authorization": "Bearer " + API_KEY,
            "Content-Type": "application/json",
            "User-Agent": "QuantumBreaksAI-CG50-Bridge/0.4",
        },
    )

    try:
        with urllib.request.urlopen(request, timeout=90) as response:
            payload = json.loads(response.read().decode("utf-8"))
    except urllib.error.HTTPError as exc:
        detail = exc.read().decode("utf-8", "replace")
        raise RuntimeError("API HTTP %s: %s" % (exc.code, detail[:400]))
    except urllib.error.URLError as exc:
        raise RuntimeError("API connection error: %s" % exc.reason)

    answer = extract_text(payload).strip()
    if not answer:
        raise RuntimeError("API returned no text.")

    history.append({"role": "user", "content": prompt})
    history.append({"role": "assistant", "content": answer})
    histories[history_key] = history[-MAX_HISTORY_MESSAGES:]

    return subject, mode, level, answer


def utf8_chunks(text, max_bytes):
    """Yield UTF-8 chunks without splitting a multibyte character."""
    current = bytearray()

    for char in text:
        encoded = char.encode("utf-8")
        if current and len(current) + len(encoded) > max_bytes:
            yield bytes(current)
            current.clear()

        if len(encoded) > max_bytes:
            if current:
                yield bytes(current)
                current.clear()
            yield encoded
        else:
            current.extend(encoded)

    if current:
        yield bytes(current)


def send_answer(ser, req_id, text):
    chunks = list(utf8_chunks(text or "(empty response)", CHUNK_BYTES))

    for seq, chunk in enumerate(chunks):
        done = 1 if seq == len(chunks) - 1 else 0
        encoded = base64.b64encode(chunk).decode("ascii")
        send_line(ser, "A:%s:%d:%d:%s" % (req_id, seq, done, encoded))


def handle_line(ser, line):
    line = line.strip()

    # Keep v2 handshake compatibility while preferring v3.
    if line == "H:QBAI:3":
        send_line(ser, "K:QBAI:3")
        return
    if line == "H:QBAI:2":
        send_line(ser, "K:QBAI:2")
        return

    try:
        req = parse_request(line)
        if req is None:
            return

        req_id = req["id"]
        prompt = req["prompt"]
        if not prompt:
            raise RuntimeError("Empty prompt.")

        subject, mode, level, answer = call_model(
            prompt,
            req["subject"],
            req["mode"],
            req["level"],
        )

        print("[%s] %s / %s / %s: %s" % (
            req_id,
            subject_label(subject),
            mode,
            level,
            prompt,
        ))
        print(" -> %s" % answer.replace("\n", " ")[:200])
        send_answer(ser, req_id, answer)

    except Exception as exc:
        message = str(exc)
        print(" !! " + message, file=sys.stderr)
        req_id = "0"

        try:
            if line.startswith("Q:"):
                req_id = line.split(":", 2)[1]
        except Exception:
            pass

        send_line(ser, "E:%s:%s" % (req_id, b64e(message)))


def resolve_serial_port(configured_port):
    if configured_port:
        return configured_port

    ports = list(list_ports.comports())

    if len(ports) == 1:
        chosen = ports[0].device
        print("Auto-selected serial port: %s" % chosen)
        return chosen

    if not ports:
        print("No serial ports found.")
        print("Connect the CG50 serial adapter or set QB_SERIAL_PORT manually.")
        return ""

    print("Multiple serial ports found. Set QB_SERIAL_PORT to one of:")
    for port in ports:
        description = getattr(port, "description", "") or ""
        print("  %s  %s" % (port.device, description))

    return ""


def main():
    port = resolve_serial_port(PORT)
    if not port:
        return 2

    if not API_KEY:
        print("Set QB_API_KEY in your environment. Do not hard-code it.")
        return 2

    if not MODEL:
        print("Set QB_MODEL to the model ID you want Quantum Breaks AI to use.")
        return 2

    print("Quantum Breaks AI CG50 bridge v0.4")
    print("Serial: %s @ %d" % (port, BAUD))
    print("API: %s" % API_URL)
    print("Model: %s" % MODEL)
    print("Handshake: QBAI protocol v3")
    print("Academic levels: auto, school, college, advanced")
    print("Modes: answer, explain, steps, check, quiz, summary, flashcards, derive, proof, research")

    with serial.Serial(port, BAUD, timeout=0.25) as ser:
        time.sleep(0.5)
        ser.reset_input_buffer()

        while True:
            raw = ser.readline()
            if raw:
                handle_line(ser, raw.decode("ascii", "replace"))


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except KeyboardInterrupt:
        print("\nBridge stopped.")
