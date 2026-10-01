# NoLimit AI

A responsive single-file AI chat client designed for phones and PCs.

## Current setup

The entire app lives in index.html.

It includes:
- Responsive mobile and desktop layouts
- OpenRouter OAuth / PKCE sign-in
- No hard-coded API key in the public repository
- Multiple local conversations
- Searchable chat history
- Streaming responses
- OpenRouter model browser
- System instructions and temperature controls
- Image attachments for vision-capable models
- Text and code file attachments
- Local browser persistence

## GitHub Pages

To publish it:

1. Open this repository on GitHub.
2. Go to Settings, then Pages.
3. Under Build and deployment, choose Deploy from a branch.
4. Choose the main branch and /(root).
5. Save.

The expected Pages address is:

https://kenny32954.github.io/nolimit-ai/

## Authentication

The public repository intentionally contains no API key.

The Connect button uses OpenRouter OAuth with PKCE. After authorization, OpenRouter returns a user-controlled key to the browser. The key stays in that browser's storage for this site and is not committed to GitHub.

## Security

Do not put a private API key directly into index.html. Any secret included in a public GitHub Pages file can be viewed by visitors.

## Next upgrades

Planned work:
- Rich Markdown and code-block controls
- PDF handling
- Web search and deep-research tools
- Image generation
- Voice input and text-to-speech
- Usage and cost reporting
- Artifact previews
- Better model capability filters
- Import/export and cross-device chat options
