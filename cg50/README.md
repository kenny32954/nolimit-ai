# Quantum Breaks AI on the Casio fx-CG50

This directory starts the calculator side of Quantum Breaks AI.

## What works now

`qb50.py` is a conservative MicroPython prototype that can be copied directly onto an fx-CG50. It provides:

- Quantum Breaks AI pocket-client shell
- prompt/history UI prototype
- compact 21-character text wrapping
- local quick calculation command
- command menu

It intentionally does **not** fake AI answers while offline.

## Install the Python prototype

1. Connect the fx-CG50 to a computer over USB.
2. Choose **USB Flash** on the calculator.
3. Copy `qb50.py` to the calculator storage.
4. Eject the calculator cleanly.
5. Open **Python** on the fx-CG50 and run `qb50.py`.

## Live AI plan

The built-in Python environment is Casio's adapted MicroPython 1.9.4 and does not provide the normal desktop networking stack. For live Quantum Breaks AI, the project uses a separate native add-in plus the PC bridge in `../bridge/`.

The native add-in will handle:

- calculator keyboard input
- scrollable chat rendering
- serial request/response framing
- response chunk assembly
- connection indicator
- history
- cancel/retry controls

See `PROTOCOL.md` for the transport protocol.

## Branch status

This is the first CG50 integration milestone. The existing web app is intentionally unchanged on this branch.
