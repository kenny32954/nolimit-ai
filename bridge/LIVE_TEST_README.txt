QUANTUM BREAKS AI - CG50 LIVE TEST PACK

WHAT THIS BUILD DOES
--------------------
The calculator app can:
- type a question on the fx-CG50
- select subject, tutor mode, and academic level
- handshake with the PC bridge
- send the question over the CG50 3-pin serial connection
- receive AI output while the model is still generating
- display streamed response chunks on the calculator
- keep recent local Q&A history

IMPORTANT
---------
The fx-CG50 itself does not have normal Wi-Fi. Live AI currently uses:

  fx-CG50 QBAI.g3a
       |
       | 3-pin serial @ 115200 baud
       v
  USB/serial adapter + computer
       |
       v
  Quantum Breaks AI bridge
       |
       | Internet
       v
  OpenRouter-compatible AI model

The API key stays on the computer and is NOT stored in QBAI.g3a.

WINDOWS
-------
1. Install Python 3 if needed.
2. Open the bridge folder.
3. Double-click INSTALL_WINDOWS.bat once.
4. Connect the CG50 serial adapter to the PC.
5. Double-click START_QBAI_WINDOWS.bat.
6. Pick the serial port.
7. Leave model as openrouter/auto for the easiest first test, or enter another model ID.
8. Enter your API key when prompted. Input is hidden and the key is not saved.
9. Copy QBAI.g3a to the calculator using USB Flash mode.
10. Launch QBAI on the calculator.
11. Press F5, then EXE on the diagnostics screen to retry the handshake.
12. When it says LINKED, return to chat.
13. Press EXE, type a question, and press EXE again to send it.

macOS / Linux
-------------
1. Install Python 3.
2. In the bridge folder:
     python3 -m pip install -r requirements.txt
     python3 qb_launch.py
3. Follow the same calculator steps above.

CALCULATOR CONTROLS
-------------------
F1    subject
F2    tutor mode
OPTN  academic level
F3    answer-focus view
F4    offline quick reference
F5    connection diagnostics
F6    retry current question
EXE   type/send a new question
UP/DOWN scroll answer
LEFT/RIGHT browse recent completed Q&A
EXIT cancel/back/quit

ACADEMIC LEVELS
---------------
AUTO
SCHOOL
COLLEGE
ADVANCED

LIVE STREAMING
--------------
The bridge uses SSE streaming. As model text arrives on the computer it is
encoded into small ordered serial frames and forwarded to the calculator.
The calculator appends those chunks immediately instead of waiting for the
entire answer to finish.

FIRST TEST SUGGESTION
---------------------
Use a short question first, for example:
  "Explain the Pythagorean theorem in two sentences."

Then try:
  SUBJECT: Algebra I
  MODE: Steps
  LEVEL: School
  "Solve 3x + 7 = 25"

Do not use this build to bypass a locked/active test. It is built for study,
homework, review, projects, and tutoring.

IF IT DOES NOT LINK
-------------------
Check:
- the desktop bridge is running
- the selected serial port is the adapter
- the adapter/cable is connected correctly
- both sides are using 115200 baud
- F5 diagnostics says SERIAL INTERFACE OPEN

The first physical test is specifically to find any CG50 hardware timing or
adapter quirks that cannot be reproduced in CI.
