# Security

## API keys

Quantum Breaks AI is a browser application. Anything hard-coded into index.html is public to anyone who can load the page.

Do not commit real API keys, passwords, tokens, or other secrets.

The app supports:
- OpenRouter OAuth with PKCE
- a manually entered OpenRouter key stored in that browser only

The validation workflow rejects strings that look like a real OpenRouter API key in index.html.

## Browser isolation

The main app uses a Content Security Policy that restricts network connections to OpenRouter.

Generated HTML previews run inside a sandboxed iframe without same-origin access to the Quantum Breaks AI page.

## Reporting a security problem

If you find a bug that could expose credentials or browser data, do not publish the secret or exploit details in a public issue. Remove or rotate any exposed credential first, then fix the affected code before publishing a reproducer.

## Static-app limitation

A static GitHub Pages app cannot make a browser-stored API key impossible to inspect by the person using that browser. The goal is to keep the key out of the public repository and prevent accidental exposure to unrelated sites.

If this project later gains a backend, server-side secret storage should replace browser storage for any shared deployment.
