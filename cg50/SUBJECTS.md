# Academic subject coverage

Quantum Breaks AI's CG50 bridge supports secondary-school through undergraduate/advanced academic work. The subject router has a general fallback, so uncommon electives can still be handled even when they do not have a named profile.

## Academic levels

- `auto` - infer the appropriate level from the prompt/course wording
- `school` - secondary-school depth and scaffolding
- `college` - undergraduate terminology, derivations, theorem conditions, and normal prerequisites
- `advanced` - upper-undergraduate / early-graduate rigor with more formal definitions, proofs, abstraction, and edge cases

On the native CG50 client, press **OPTN** to change academic level.

## Mathematics

- General mathematics
- Algebra
- Geometry
- Statistics
- Calculus
- Linear algebra
- Discrete mathematics
- Differential equations
- Number theory
- Real analysis
- Abstract algebra

## Science

- Biology
- Genetics
- Microbiology
- Anatomy/physiology
- Chemistry
- Organic chemistry
- Biochemistry
- Physics
- Thermodynamics
- Circuit analysis
- Earth/space science
- Environmental science

## Computer science and engineering

- Computer science/programming
- Data structures
- Algorithms
- Databases
- Computer architecture
- Engineering
- Statics/dynamics
- Materials science
- CTE/career tech
- Agriculture

## English, humanities, and social sciences

- English/ELA
- Literature
- Academic writing
- History
- Social studies
- Geography
- Government/civics
- Political science
- Economics
- Finance
- Psychology
- Research methods
- Sociology
- Philosophy/logic
- World languages

## Business, arts, and other subjects

- Business/marketing
- Accounting
- Visual art
- Music
- Media/audio-video/film
- Health/nutrition
- Physical education

## Tutor modes

- `answer` - direct answer with compact justification
- `explain` - teach the concept
- `steps` - ordered method
- `check` - review student work and locate errors
- `quiz` - one-question-at-a-time tutoring
- `summary` - compact study notes
- `flashcards` - quick review cards
- `derive` - derive results from definitions/governing equations
- `proof` - rigorous proof with hypotheses and proof strategy
- `research` - academic research/methodology help without fabricated citations or data

## Assessment behavior

Quantum Breaks AI is being built as a study and schoolwork assistant. If a prompt explicitly says it is a currently active locked or graded test/exam, the bridge switches from answer delivery to concept/method help rather than offering a bypass workflow.
