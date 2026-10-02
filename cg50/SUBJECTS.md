# School subject coverage

Quantum Breaks AI's CG50 bridge uses subject-aware tutoring profiles plus a general fallback, so an uncommon elective still works even if it does not have its own named profile yet.

## Mathematics

- General math
- Algebra
- Geometry
- Statistics
- Calculus

## Science and STEM

- Biology
- Chemistry
- Physics
- Earth/space science
- Environmental science
- Computer science/programming
- Engineering

## English and communication

- English/ELA
- Literature
- Writing
- World languages

## Social sciences

- History
- Social studies
- Geography
- Government/civics
- Economics
- Psychology
- Sociology

## Business and career subjects

- Business/marketing
- Accounting
- CTE/career tech
- Agriculture

## Arts and media

- Visual art
- Music
- Media/audio-video/film

## Health and activity

- Health/nutrition
- Physical education

If a class is not listed, `auto` can route to `general`, so the model can still help with the assignment.

## Tutor modes

- `answer` - direct answer plus compact explanation
- `explain` - teach the concept
- `steps` - show an ordered method
- `check` - review student work and locate errors
- `quiz` - one-question-at-a-time tutoring
- `summary` - compact study notes
- `flashcards` - quick question/answer review

## Assessment behavior

Quantum Breaks AI is being built as a study and schoolwork assistant. If a prompt explicitly says it is a currently active locked or graded test/exam, the bridge instructs the model to switch from answer delivery to concept/method help instead of offering a bypass workflow.
