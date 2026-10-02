# Quantum Breaks AI academic coverage

The CG50 client is designed to cover the standard U.S. high-school curriculum, common electives/CTE pathways, and college-level extensions. Districts use different course names, so the bridge also keeps a `general` fallback for school-specific classes that do not exactly match a named profile.

## High-school mathematics

- Pre-Algebra
- Algebra I
- Geometry
- Algebra II
- Trigonometry
- Precalculus
- Statistics
- Calculus / AP-style calculus support
- Consumer Math
- Financial Math

College extensions include Linear Algebra, Discrete Math, Differential Equations, Number Theory, Real Analysis, and Abstract Algebra.

## High-school science

- Physical Science
- Biology
- Chemistry
- Physics
- Earth/Space Science
- Astronomy
- Environmental Science
- Anatomy/Physiology
- Genetics
- Microbiology
- Forensic Science
- Marine Science

College extensions include Organic Chemistry, Biochemistry, Thermodynamics, and Circuit Analysis.

## English and communication

- English/ELA
- English Composition
- American Literature
- British Literature
- World Literature
- General Literature
- Creative Writing
- Journalism
- Speech/Debate
- Media Literacy

## Social studies and humanities

- World History
- U.S. History
- European History
- General History
- Social Studies
- Geography
- Government
- Civics
- Economics
- Psychology
- Sociology
- Anthropology
- Philosophy/Logic
- Research Methods
- Political Science

## Business and finance

- Business
- Marketing
- Entrepreneurship
- Accounting
- Personal Finance
- Financial Math
- Finance
- Business Law

## Computer science, engineering, and technology

- Computer Science
- Web Development
- Cybersecurity
- Information Technology
- Data Structures
- Algorithms
- Databases
- Computer Architecture
- Engineering
- Robotics
- Electronics
- CAD/Drafting
- Statics/Dynamics
- Materials Science

Cybersecurity support is classroom/defensive: it does not provide unauthorized-access workflows.

## Agriculture, trades, and CTE

- CTE/Career Tech
- Career Readiness
- Agriculture
- Animal Science
- Plant Science/Horticulture
- Construction Trades
- Automotive Technology
- Culinary Arts
- Family & Consumer Science
- Child Development

Hazardous hands-on trade procedures are kept safety-first and deferred to qualified instructors and official lab/shop procedures.

## Fine arts and media

- Visual Art
- Drawing/Painting
- Graphic Design
- Photography
- Ceramics/Sculpture
- Art History
- Music
- Music Theory
- Band/Orchestra
- Choir
- Theater/Drama
- Dance
- Media/Audio-Video
- Film Studies

## World languages

- General World Language
- Spanish
- French
- German
- Latin
- American Sign Language

The general language profile can also handle other languages not explicitly named.

## Health, PE, and life skills

- Health
- Nutrition
- Physical Education
- Sports Medicine
- Exercise Science
- Driver Education
- JROTC/Leadership
- Yearbook
- Study Skills / Academic Support

## Academic levels

- `auto` - infer suitable depth
- `school` - high-school depth and scaffolding
- `college` - undergraduate terminology, derivations, and prerequisite knowledge
- `advanced` - upper-undergraduate / early-graduate rigor

On the native CG50 client, press **OPTN** to select the academic level.

## Tutor modes

- `answer`
- `explain`
- `steps`
- `check`
- `quiz`
- `summary`
- `flashcards`
- `derive`
- `proof`
- `research`

## Assessment behavior

Quantum Breaks AI is a study, homework, review, and project assistant. If a prompt explicitly says it is a currently active locked or graded test/exam, the bridge switches to concept/method help instead of providing a bypass workflow.
