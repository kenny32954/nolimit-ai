# Quantum Breaks AI - CG50 serial protocol v0.3

This protocol connects the native fx-CG50 add-in to the desktop bridge.

## Link settings

- 115200 baud
- 8 data bits
- no parity
- 1 stop bit
- line-delimited ASCII transport
- message text is UTF-8 encoded, then Base64 encoded for transport safety

## Handshake

The v0.3 calculator probes the desktop bridge with:

```text
H:QBAI:3
```

The bridge responds:

```text
K:QBAI:3
```

The bridge also accepts the older v0.2 handshake for compatibility.

## College-capable request

```text
Q:<request-id>:<subject>:<mode>:<level>:<base64(prompt)>\n
```

Example:

```text
Q:42:linear_algebra:derive:college:<base64 of "Find the eigenvalues and explain the eigenspaces.">
```

## Academic levels

```text
auto
school
college
advanced
```

- `auto`: infer suitable depth from the prompt/course wording
- `school`: secondary-school depth
- `college`: undergraduate rigor, terminology, derivations, and prerequisite knowledge
- `advanced`: upper-undergraduate / early-graduate rigor with more formal proofs, abstraction, assumptions, and edge cases

## Subject identifiers

```text
auto
math
algebra
geometry
statistics
calculus
linear_algebra
discrete_math
differential_equations
number_theory
real_analysis
abstract_algebra
biology
genetics
microbiology
anatomy_physiology
chemistry
organic_chemistry
biochemistry
physics
thermodynamics
circuits
earth
environmental_science
ela
literature
writing
history
social_studies
geography
government
political_science
economics
finance
business
accounting
computer_science
data_structures
algorithms
databases
computer_architecture
engineering
statics_dynamics
materials_science
cte
agriculture
psychology
research_methods
sociology
philosophy_logic
art
music
media
language
health
physical_education
```

`auto` lets the desktop bridge detect the subject from the prompt.

## Tutor modes

```text
answer
explain
steps
check
quiz
summary
flashcards
derive
proof
research
```

## Compatibility

Protocol v0.2 requests are still accepted:

```text
Q:<request-id>:<subject>:<mode>:<base64(prompt)>\n
```

The bridge treats them as `level=auto`.

Protocol v0.1 requests are also still accepted:

```text
Q:<request-id>:<base64(prompt)>\n
```

The bridge treats them as `subject=auto`, `mode=explain`, and `level=auto`.

## Response chunks

```text
A:<request-id>:<sequence>:<done>:<base64(chunk)>\n
```

- `sequence` starts at 0.
- `done` is `1` for the final chunk and `0` otherwise.
- The calculator concatenates decoded chunks in sequence order.

## Error

```text
E:<request-id>:<base64(error message)>\n
```

## Why the protocol stays small

The calculator should not need a JSON parser just to talk to the bridge. Short colon-delimited metadata plus Base64 keeps parsing predictable while the desktop bridge handles the heavier academic routing.
