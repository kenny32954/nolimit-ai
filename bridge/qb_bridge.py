#!/usr/bin/env python3
"""Quantum Breaks AI live serial bridge for the Casio fx-CG50.

Protocol v0.3:
    Handshake:
        CG50 -> PC: H:QBAI:3
        PC -> CG50: K:QBAI:3

    Request:
        Q:<id>:<subject>:<mode>:<level>:<base64 UTF-8 prompt>

    Streaming response:
        A:<id>:<seq>:0:<base64 UTF-8 chunk>
        ...
        A:<id>:<seq>:1:<base64 final chunk or empty payload>

    Error:
        E:<id>:<base64 UTF-8 error>

The default API endpoint is OpenRouter's OpenAI-compatible
/chat/completions endpoint with SSE streaming enabled.
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
HTTP_TIMEOUT = int(os.getenv("QB_HTTP_TIMEOUT", "120"))

# Separate histories by subject + academic level so different courses/levels
# do not accidentally contaminate one another.
histories = {}


def b64e(text):
    return base64.b64encode(text.encode("utf-8")).decode("ascii")


def b64d(text):
    return base64.b64decode(text.encode("ascii"), validate=True).decode("utf-8")


def send_line(ser, line):
    ser.write((line + "\n").encode("ascii"))
    ser.flush()


def parse_request(line):
    line = line.strip()
    if not line:
        return None

    # v0.1 compatibility: Q:<id>:<payload>
    if line.startswith("Q:") and line.count(":") == 2:
        parts = line.split(":", 2)
        return {
            "id": parts[1],
            "subject": "auto",
            "mode": "explain",
            "level": "auto",
            "prompt": b64d(parts[2]).strip(),
        }

    # v0.2 compatibility: Q:<id>:<subject>:<mode>:<payload>
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


def utf8_chunks(text, max_bytes):
    """Yield UTF-8 byte chunks without splitting a multibyte character."""
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


def delta_text(payload):
    """Extract visible text from an OpenAI-compatible SSE delta."""
    choices = payload.get("choices") or []
    if not choices:
        return ""

    delta = choices[0].get("delta") or {}
    content = delta.get("content", "")

    if isinstance(content, str):
        return content

    if isinstance(content, list):
        parts = []
        for item in content:
            if not isinstance(item, dict):
                continue
            if item.get("type") in ("text", "output_text"):
                value = item.get("text", "")
                if isinstance(value, str):
                    parts.append(value)
                elif isinstance(value, dict):
                    parts.append(str(value.get("value", "")))
        return "".join(parts)

    return ""


def build_messages(prompt, requested_subject, requested_mode, requested_level):
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

    return subject, mode, level, history_key, history, messages


def stream_model(prompt, requested_subject="auto", requested_mode="explain",
                 requested_level="auto"):
    """Yield visible model text as SSE events arrive.

    Returns metadata through StopIteration.value:
        (subject, mode, level, history_key, history, full_answer)
    """
    subject, mode, level, history_key, history, messages = build_messages(
        prompt,
        requested_subject,
        requested_mode,
        requested_level,
    )

    body = json.dumps({
        "model": MODEL,
        "messages": messages,
        "stream": True,
    }).encode("utf-8")

    request = urllib.request.Request(
        API_URL,
        data=body,
        method="POST",
        headers={
            "Authorization": "Bearer " + API_KEY,
            "Content-Type": "application/json",
            "Accept": "text/event-stream",
            "User-Agent": "QuantumBreaksAI-CG50-Bridge/0.5",
            "X-Title": "Quantum Breaks AI CG50",
        },
    )

    answer_parts = []

    try:
        with urllib.request.urlopen(request, timeout=HTTP_TIMEOUT) as response:
            for raw in response:
                line = raw.decode("utf-8", "replace").strip()

                if not line or line.startswith(":"):
                    continue
                if not line.startswith("data:"):
                    continue

                data = line[5:].strip()
                if data == "[DONE]":
                    break

                try:
                    payload = json.loads(data)
                except json.JSONDecodeError:
                    continue

                # Some OpenAI-compatible providers can emit an error object
                # inside a 200 streaming response.
                if payload.get("error"):
                    error = payload["error"]
                    if isinstance(error, dict):
                        message = error.get("message") or str(error)
                    else:
                        message = str(error)
                    raise RuntimeError("API stream error: " + message)

                piece = delta_text(payload)
                if piece:
                    answer_parts.append(piece)
                    yield piece

    except urllib.error.HTTPError as exc:
        detail = exc.read().decode("utf-8", "replace")
        raise RuntimeError("API HTTP %s: %s" % (exc.code, detail[:400]))
    except urllib.error.URLError as exc:
        raise RuntimeError("API connection error: %s" % exc.reason)

    answer = "".join(answer_parts).strip()
    if not answer:
        raise RuntimeError("API returned no text.")

    history.append({"role": "user", "content": prompt})
    history.append({"role": "assistant", "content": answer})
    histories[history_key] = history[-MAX_HISTORY_MESSAGES:]

    return subject, mode, level, history_key, history, answer


def send_streaming_answer(ser, req_id, iterator):
    """Forward model output to the calculator while it is being generated."""
    seq = 0
    pending = ""
    full = []

    while True:
        try:
            piece = next(iterator)
        except StopIteration as stop:
            metadata = stop.value
            break

        if not piece:
            continue

        full.append(piece)
        pending += piece

        # Flush complete CHUNK_BYTES-sized UTF-8-safe pieces while keeping
        # the small tail to combine with the next SSE event.
        encoded_pending = pending.encode("utf-8")
        if len(encoded_pending) < CHUNK_BYTES:
            continue

        chunks = list(utf8_chunks(pending, CHUNK_BYTES))
        if len(chunks) == 1:
            continue

        # Keep the final partial chunk buffered.
        for raw_chunk in chunks[:-1]:
            send_line(
                ser,
                "A:%s:%d:0:%s" % (
                    req_id,
                    seq,
                    base64.b64encode(raw_chunk).decode("ascii"),
                ),
            )
            seq += 1

        pending = chunks[-1].decode("utf-8")

    # Flush remaining visible text. If there is none, send an empty final
    # frame so the calculator can still transition out of THINKING.
    if pending:
        raw_chunks = list(utf8_chunks(pending, CHUNK_BYTES))
        for index, raw_chunk in enumerate(raw_chunks):
            done = 1 if index == len(raw_chunks) - 1 else 0
            send_line(
                ser,
                "A:%s:%d:%d:%s" % (
                    req_id,
                    seq,
                    done,
                    base64.b64encode(raw_chunk).decode("ascii"),
                ),
            )
            seq += 1
    else:
        send_line(ser, "A:%s:%d:1:" % (req_id, seq))

    return metadata


def send_answer(ser, req_id, text):
    """Non-streaming helper retained for protocol tests/fallbacks."""
    chunks = list(utf8_chunks(text or "(empty response)", CHUNK_BYTES))
    for seq, chunk in enumerate(chunks):
        done = 1 if seq == len(chunks) - 1 else 0
        encoded = base64.b64encode(chunk).decode("ascii")
        send_line(ser, "A:%s:%d:%d:%s" % (req_id, seq, done, encoded))


def handle_line(ser, line):
    line = line.strip()

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

        print("[%s] %s / %s / %s" % (
            req_id,
            req["subject"],
            req["mode"],
            req["level"],
        ))
        print(" > %s" % prompt.replace("\n", " ")[:220])

        iterator = stream_model(
            prompt,
            req["subject"],
            req["mode"],
            req["level"],
        )
        subject, mode, level, _, _, answer = send_streaming_answer(
            ser, req_id, iterator
        )

        print(" -> %s / %s / %s" % (
            subject_label(subject),
            mode,
            level,
        ))
        print("    %s" % answer.replace("\n", " ")[:220])

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

    print("Quantum Breaks AI CG50 bridge v0.5 LIVE")
    print("Serial: %s @ %d" % (port, BAUD))
    print("API: %s" % API_URL)
    print("Model: %s" % MODEL)
    print("Streaming: SSE -> live CG50 chunks")
    print("Handshake: QBAI protocol v3")
    print("Academic levels: auto, school, college, advanced")
    print("Ready. Open QBAI on the calculator and press F5.")

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
