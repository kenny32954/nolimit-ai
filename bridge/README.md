# Quantum Breaks AI - Desktop bridge

This folder contains the PC-side bridge for the fx-CG50 integration.

The calculator does not have normal Wi-Fi networking. The live architecture is:

```text
fx-CG50 native add-in
        |
   3-pin serial
        |
PC serial adapter
        |
qb_bridge.py
        |
Internet API
        |
Quantum Breaks AI model
```

## Install

Python 3 is required.

```bash
python -m pip install -r requirements.txt
```

## Configure

Keep API keys out of the repository.

### OpenRouter-style example

```bash
export QB_SERIAL_PORT=/dev/ttyUSB0
export QB_API_KEY=YOUR_KEY
export QB_MODEL=YOUR_MODEL_ID
export QB_API_URL=https://openrouter.ai/api/v1/chat/completions
python qb_bridge.py
```

On Windows PowerShell, use `$env:NAME="value"` instead of `export NAME=value`.

The bridge also works with another OpenAI-compatible chat-completions endpoint by changing `QB_API_URL`, `QB_API_KEY`, and `QB_MODEL`.

## Important

Do not place an API key in the calculator program, `index.html`, or any committed source file.

The current bridge is ready for the serial protocol. The matching live calculator transport will be a native `.g3a` add-in because Casio's built-in MicroPython environment does not expose all hardware APIs needed for this link.
