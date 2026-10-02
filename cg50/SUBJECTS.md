# School subject coverage

Quantum Breaks AI's CG50 bridge now has subject-aware tutoring profiles.

## Core subjects

- General math
- Algebra
- Geometry
- Statistics
- Calculus
- Biology
- Chemistry
- Physics
- Earth/space science
- English/ELA
- Literature
- Writing
- History
- Government/civics
- Economics
- Business/marketing
- Computer science/programming
- World languages
- Health/nutrition

The architecture is intentionally extensible: adding another subject only requires adding a subject profile and optional detection keywords in `bridge/subject_router.py`.

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
