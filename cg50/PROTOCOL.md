# Quantum Breaks AI - CG50 serial protocol v0.2

This protocol connects the native fx-CG50 add-in to the desktop bridge.

## Link settings

- 115200 baud
- 8 data bits
- no parity
- 1 stop bit
- line-delimited ASCII transport
- message text is UTF-8 encoded, then Base64 encoded for transport safety

## Subject-aware request

```text
Q:<request-id>:<subject>:<mode>:<base64(prompt)>\n
```

Example:

```text
Q:42:physics:steps:<base64 of "A 2 kg object accelerates at 3 m/s^2. Find force.">
```

Supported subject identifiers:

```text
auto
math
algebra
geometry
statistics
calculus
biology
chemistry
physics
earth
environmental_science
ela
literature
writing
history
social_studies
geography
government
economics
business
accounting
computer_science
engineering
cte
agriculture
psychology
sociology
art
music
media
language
health
physical_education
```

`auto` lets the desktop bridge detect the subject from the prompt. Unknown or unusual electives can fall back to the general schoolwork profile.

Supported tutor modes:

```text
answer
explain
steps
check
quiz
summary
flashcards
```

## Legacy request

Protocol v0.1 requests remain accepted:

```text
Q:<request-id>:<base64(prompt)>\n
```

The bridge treats these as `subject=auto` and `mode=explain`.

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

The calculator should not need a JSON parser just to talk to the bridge. Short colon-delimited metadata plus Base64 keeps parsing predictable and leaves most intelligence on the PC/server side.
