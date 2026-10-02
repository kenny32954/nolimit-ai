# Quantum Breaks AI - CG50 serial protocol v0.1

This protocol connects the future native fx-CG50 add-in to the desktop bridge.

## Link settings

- 115200 baud
- 8 data bits
- no parity
- 1 stop bit
- line-delimited ASCII transport
- message text is UTF-8 encoded, then Base64 encoded for transport safety

The fx-CG50 exposes a 3-pin serial port. Community-tested CG50 add-ins can access Casio's serial syscalls; a 115200 baud mode is represented by baud selector 9 in the serial configuration. The native add-in will use this path rather than relying on the restricted built-in MicroPython environment.

## Request

```text
Q:<request-id>:<base64(prompt)>\n
```

Example logical payload before Base64:

```text
Explain why the sky is blue.
```

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

## Design goals

The protocol is intentionally tiny so the calculator side can parse it without a JSON library. Base64 avoids delimiter collisions and lets responses contain newlines and punctuation safely.

The desktop bridge is provider-agnostic at the transport layer. The current bridge implementation accepts an OpenAI-compatible chat-completions endpoint through environment variables.
