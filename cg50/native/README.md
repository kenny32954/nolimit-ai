# Native Quantum Breaks AI client for fx-CG50

This is the native `.g3a` path for Quantum Breaks AI.

The first milestone is intentionally UI-first. It gives the calculator a real color application shell with subject and tutor-mode controls while keeping the serial layer isolated behind `transport.c`.

## Current native controls

- **F1** - cycle school subject
- **F2** - cycle tutor mode
- **F5** - probe the bridge transport
- **UP/DOWN** - reserved for transcript scrolling
- **EXIT** - quit

The screen already exposes the same subject/mode model as the desktop bridge.

## Build

Current fxSDK projects use CMake, and CG50 builds use `fxsdk build-cg`.

From this directory:

```bash
fxsdk build-cg
```

The CMake configuration runs `tools/make_icon.py` automatically. That script uses only the Python standard library and generates `src/icon.png`, so no Pillow dependency is needed.

The expected output is:

```text
QBAI.g3a
```

## Install

Connect the fx-CG50 over USB, select USB Flash, copy `QBAI.g3a` onto the calculator storage according to your add-in setup, eject cleanly, and launch QBAI from the calculator menu.

## Next native milestones

1. 3-pin serial initialization and bridge handshake.
2. Protocol v0.2 request/response framing.
3. Base64 encode/decode on the calculator.
4. Text-entry editor using the CG50 keyboard.
5. Chunked live response rendering.
6. Scrollback/history.
7. Subject/mode selector overlay instead of cycling.
8. Retry, cancel, and connection recovery.
9. Compact math/science rendering improvements.

The current `transport.c` is a deliberate stub so the UI can be built and tested independently of serial hardware.
