#!/usr/bin/env python3
"""Interactive launcher for the Quantum Breaks AI CG50 live bridge.

Secrets are never written to disk. The launcher may remember non-secret
settings (serial port, model, API URL) in qb_bridge_config.json.
"""

import getpass
import json
import os
from pathlib import Path
import subprocess
import sys

try:
    from serial.tools import list_ports
except ImportError:
    print("pyserial is required.")
    print("Run: python -m pip install -r requirements.txt")
    raise

HERE = Path(__file__).resolve().parent
CONFIG_PATH = HERE / "qb_bridge_config.json"
BRIDGE_PATH = HERE / "qb_bridge.py"

DEFAULTS = {
    "api_url": "https://openrouter.ai/api/v1/chat/completions",
    "model": "openrouter/auto",
    "serial_port": "",
}


def load_config():
    config = dict(DEFAULTS)
    try:
        if CONFIG_PATH.exists():
            saved = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
            if isinstance(saved, dict):
                for key in DEFAULTS:
                    value = saved.get(key)
                    if isinstance(value, str):
                        config[key] = value
    except Exception:
        pass
    return config


def save_config(config):
    safe = {
        "api_url": config["api_url"],
        "model": config["model"],
        "serial_port": config["serial_port"],
    }
    CONFIG_PATH.write_text(
        json.dumps(safe, indent=2) + "\n",
        encoding="utf-8",
    )


def choose_port(previous):
    ports = list(list_ports.comports())

    print("\nSerial ports:")
    if not ports:
        print("  No serial ports detected.")
        manual = input(
            "Enter a port manually (example COM5 or /dev/ttyUSB0)"
            + ((" [" + previous + "]") if previous else "")
            + ": "
        ).strip()
        return manual or previous

    for index, port in enumerate(ports, 1):
        description = getattr(port, "description", "") or ""
        marker = " *" if port.device == previous else ""
        print("  %d) %s  %s%s" % (
            index, port.device, description, marker
        ))

    default = ""
    for index, port in enumerate(ports, 1):
        if port.device == previous:
            default = str(index)
            break
    if not default and len(ports) == 1:
        default = "1"

    raw = input(
        "Choose port"
        + ((" [" + default + "]") if default else "")
        + " or type a port name: "
    ).strip()

    if not raw:
        raw = default

    if raw.isdigit():
        index = int(raw)
        if 1 <= index <= len(ports):
            return ports[index - 1].device

    return raw or previous


def prompt_value(label, current):
    raw = input("%s [%s]: " % (label, current)).strip()
    return raw or current


def main():
    print("=" * 54)
    print(" Quantum Breaks AI - CG50 LIVE BRIDGE")
    print("=" * 54)
    print("API keys are used only for this process and are not saved.")

    config = load_config()
    config["serial_port"] = choose_port(config["serial_port"])
    config["model"] = prompt_value("Model", config["model"])
    config["api_url"] = prompt_value("API URL", config["api_url"])

    if not config["serial_port"]:
        print("\nNo serial port selected.")
        return 2

    api_key = getpass.getpass(
        "\nAPI key (input hidden; not saved): "
    ).strip()
    if not api_key:
        print("No API key entered.")
        return 2

    save_config(config)

    env = os.environ.copy()
    env["QB_SERIAL_PORT"] = config["serial_port"]
    env["QB_MODEL"] = config["model"]
    env["QB_API_URL"] = config["api_url"]
    env["QB_API_KEY"] = api_key

    print("\nStarting live bridge...")
    print("Calculator: open QBAI -> F5 -> EXE to handshake.")
    print("Then press EXE on the chat screen to type a question.")
    print("Press Ctrl+C here to stop the bridge.\n")

    return subprocess.call(
        [sys.executable, str(BRIDGE_PATH)],
        cwd=str(HERE),
        env=env,
    )


if __name__ == "__main__":
    raise SystemExit(main())
