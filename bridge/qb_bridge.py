#!/usr/bin/env python3
"""Quantum Breaks AI serial bridge for the Casio fx-CG50.

Protocol (ASCII lines):
    Calculator -> PC: Q:<id>:<base64 UTF-8 prompt>
    PC -> Calculator: A:<id>:<seq>:<done 0|1>:<base64 UTF-8 chunk>
    PC -> Calculator: E:<id>:<base64 UTF-8 error>

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
except ImportError:
    print("Missing dependency: pyserial")
    print("Install with: python -m pip install pyserial")
    raise

BAUD = int(os.getenv("QB_SERIAL_BAUD", "115200"))
PORT = os.getenv("QB_SERIAL_PORT", "")
API_URL = os.getenv("QB_API_URL", "https://openrouter.ai/api/v1/chat/completions")
API_KEY = os.getenv("QB_API_KEY", "")
MODEL = os.getenv("QB_MODEL", "")
SYSTEM_PROMPT = os.getenv(
    "QB_SYSTEM_PROMPT",
    "You are Quantum Breaks AI running through a Casio fx-CG50. "
    "Be concise because the calculator screen is small."
)
MAX_HISTORY_MESSAGES = int(os.getenv("QB_HISTORY_MESSAGES", "12"))
CHUNK_BYTES = int(os.getenv("QB_CHUNK_BYTES", "144"))

history = []


def b64e(text):
    return base64.b64encode(text.encode("utf-8")).decode("ascii")


def b64d(text):
    return base64.b64decode(text.encode("ascii")).decode("utf-8")


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


def call_model(prompt):
    global history

    messages = [{"role": "system", "content": SYSTEM_PROMPT}]
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
            "User-Agent": "QuantumBreaksAI-CG50-Bridge/0.1",
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
    history = history[-MAX_HISTORY_MESSAGES:]
    return answer


def send_answer(ser, req_id, text):
    raw = text.encode("utf-8")
    if not raw:
        raw = b"(empty response)"

    seq = 0
    offset = 0
    while offset < len(raw):
        chunk = raw[offset:offset + CHUNK_BYTES]
        offset += len(chunk)
        done = 1 if offset >= len(raw) else 0
        encoded = base64.b64encode(chunk).decode("ascii")
        send_line(ser, "A:%s:%d:%d:%s" % (req_id, seq, done, encoded))
        seq += 1


def handle_line(ser, line):
    line = line.strip()
    if not line:
        return

    parts = line.split(":", 2)
    if len(parts) != 3 or parts[0] != "Q":
        return

    req_id = parts[1]

    try:
        prompt = b64d(parts[2]).strip()
        if not prompt:
            raise RuntimeError("Empty prompt.")
        print("[%s] %s" % (req_id, prompt))
        answer = call_model(prompt)
        print(" -> %s" % answer.replace("\n", " ")[:200])
        send_answer(ser, req_id, answer)
    except Exception as exc:
        message = str(exc)
        print(" !! " + message, file=sys.stderr)
        send_line(ser, "E:%s:%s" % (req_id, b64e(message)))


def main():
    if not PORT:
        print("Set QB_SERIAL_PORT first, for example COM5 or /dev/ttyUSB0.")
        return 2
    if not API_KEY:
        print("Set QB_API_KEY in your environment. Do not hard-code it.")
        return 2
    if not MODEL:
        print("Set QB_MODEL to the model ID you want Quantum Breaks AI to use.")
        return 2

    print("Quantum Breaks AI CG50 bridge")
    print("Serial: %s @ %d" % (PORT, BAUD))
    print("API: %s" % API_URL)
    print("Model: %s" % MODEL)

    with serial.Serial(PORT, BAUD, timeout=0.25) as ser:
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
