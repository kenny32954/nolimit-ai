# Quantum Breaks AI on the Casio fx-CG50

This directory contains the calculator side of Quantum Breaks AI.

## Two calculator clients

### `qb50.py`

A conservative built-in MicroPython prototype that can be copied directly onto the fx-CG50. It provides the school-assistant shell, subject controls, tutor-mode controls, local history, and a quick calculator mode.

### `native/`

The real live-AI client. It is an fxSDK/gint `.g3a` add-in with a color UI, keyboard editor, subject/mode pickers, serial transport, bridge handshake, request framing, streamed response assembly, scrolling, retry, and answer-focus mode.

## Install the Python prototype

1. Connect the fx-CG50 to a computer over USB.
2. Choose **USB Flash** on the calculator.
3. Copy `qb50.py` to calculator storage.
4. Eject the calculator cleanly.
5. Open **Python** on the fx-CG50 and run `qb50.py`.

The Python prototype is useful offline but does not pretend to provide cloud AI responses.

## Live AI architecture

```text
QBAI.g3a on fx-CG50
        |
        | 3-pin serial
        v
PC running bridge/qb_bridge.py
        |
        | Internet
        v
AI model/API
```

The native client keeps API keys off the calculator and out of committed source.

See:

- `native/README.md` for the native build/client details.
- `PROTOCOL.md` for calculator-to-bridge framing.
- `SUBJECTS.md` for school subject coverage.
- `../bridge/README.md` for the desktop bridge.
