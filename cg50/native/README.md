# Native Quantum Breaks AI client for fx-CG50

This directory contains the native `.g3a` client for Quantum Breaks AI.

## Current status

The native client now has the first complete live-chat path implemented in source:

- color CG50 chat UI
- scrollable subject picker
- scrollable tutor-mode picker
- academic-level picker: AUTO / SCHOOL / COLLEGE / ADVANCED
- college disciplines including linear algebra, differential equations, organic chemistry, algorithms, engineering, research methods, and more
- calculator keyboard prompt editor
- upper/lower-case alpha input plus common math symbols
- Base64 request/response codec
- 115200-baud Casio serial syscall wrappers
- desktop-bridge handshake
- subject-aware request framing
- nonblocking answer polling
- ordered response-chunk validation
- streamed answer assembly
- answer-focus view
- per-subject offline quick-reference cards
- connection diagnostics screen with serial/link state
- local recent-turn history for the last four completed answers
- timeout, cancel, and retry behavior
- local answer-size protection

The bridge and native C code are automatically syntax/protocol checked in GitHub Actions. A physical calculator + compatible serial connection is still required for the first real hardware end-to-end test.

## Controls

### Chat screen

- **F1** - open subject picker
- **F2** - open tutor-mode picker
- **OPTN** - open academic-level picker
- **F3** - open/close answer-focus view
- **F4** - open the selected subject's offline quick reference
- **F5** - open connection diagnostics and probe/link the desktop bridge
- **F6** - retry the current question
- **EXE** - write a new question
- **UP/DOWN** - scroll answer
- **LEFT/RIGHT** - browse recent completed questions/answers
- **EXIT** - cancel a local wait, leave answer focus, or quit

### Prompt editor

- **ALPHA** - switch between letters and number/symbol input
- **SHIFT** - toggle upper/lower case while typing letters
- **DEL** - delete
- **EXE** - send
- **EXIT** - cancel prompt editing

## Build

Install fxSDK and gint, then from this directory run:

```bash
fxsdk build-cg
```

The expected output is:

```text
QBAI.g3a
```

The CMake configuration generates both CG50 menu icons automatically using `tools/make_icon.py`; the generator only uses Python's standard library.

## Architecture

```text
fx-CG50 QBAI.g3a
      |
      | Casio 3-pin serial, 115200 baud
      v
desktop qb_bridge.py
      |
      | OpenAI-compatible HTTP API
      v
Quantum Breaks AI model
```

The calculator does not store an API key. API credentials stay on the Internet-connected computer running the bridge.

## Protocol

The native client uses protocol v0.3 from `../PROTOCOL.md`.

Before a request it can verify the bridge with:

```text
H:QBAI:3
K:QBAI:3
```

A normal request is:

```text
Q:<id>:<subject>:<mode>:<level>:<base64 prompt>
```

Answers arrive as ordered Base64 chunks and are assembled on the calculator.

## Next milestones

1. Physical CG50 end-to-end serial test.
2. Fix any hardware-specific serial timing differences found on-device.
3. Improve mathematical rendering for matrices, vectors, calculus notation, proofs, and non-ASCII language characters.
4. Persistent calculator-side conversation history.
5. Compact offline formula/reference tools.
6. Better keyboard punctuation and symbol entry.
7. Optional settings screen for timeout, answer size, and bridge behavior.
