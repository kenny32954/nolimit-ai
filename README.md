# NoLimit AI

NoLimit AI is a responsive, single-file AI client built to feel good on both phones and PCs.

The actual app is still one file:

- index.html

Everything else in this repository is project support such as documentation and automated validation.

## Current feature set

### Chat
- Streaming OpenRouter chat completions
- Searchable local conversation history
- Responsive mobile and desktop layouts
- Stop generation
- Regenerate replies
- Copy replies
- Read replies aloud with browser speech synthesis
- Export the current conversation as Markdown
- System instructions
- Temperature control
- Reasoning effort controls from none through extra high
- Optional OpenRouter response caching
- Token and cost display when returned by the provider

### Models
- Live OpenRouter model catalog
- Model search
- Filters for free, vision, and reasoning-capable models
- Auto Router support
- Model switching without changing the rest of the app

### Web and research
- OpenRouter server-side web search
- OpenRouter server-side web fetch
- Reliable server-side datetime context
- One-tap Research mode that enables search and page reading
- Optional image-generation server tool inside normal chat

### Agent orchestration
- Subagent mode so a capable model can delegate focused work
- Fusion mode for multi-model deliberation on harder tasks
- Fusion is deliberately opt-in because it can use more model calls, take longer, and cost more

### Files and multimodal input
- Image attachments for vision-capable models
- PDF attachments through OpenRouter PDF parsing
- Text, code, JSON, CSV, Markdown, HTML, CSS, and other text-file attachments
- Audio-file transcription before sending a message

### Voice
- Record from the microphone in supported HTTPS browsers
- Transcribe recordings through OpenRouter speech-to-text
- Browser text-to-speech for replies
- Optional automatic read-aloud

### Code and artifacts
- Rich Markdown rendering
- Copyable code blocks
- Sandboxed preview button for HTML artifacts
- Paste and drag/drop file support

### Image generation
- Dedicated image-generation panel
- Live image-model catalog
- Aspect-ratio selection
- PNG, JPEG, and WebP output options
- Add generated images back into the current conversation

## Authentication

The public repository contains no private API key.

You have two connection options inside Settings:

1. Connect OpenRouter
   - Uses OpenRouter OAuth with PKCE.
   - The returned user-controlled key is stored only in that browser for this site.

2. Use my API key
   - Lets you paste an OpenRouter key directly into the app.
   - It is saved in that browser's local storage and is not written into the repository.

Do not commit a private API key into index.html.

## GitHub Pages

To publish the app with GitHub Pages:

1. Open this repository on GitHub.
2. Go to Settings -> Pages.
3. Under Build and deployment, choose Deploy from a branch.
4. Select main and /(root).
5. Save.

Expected site address:

https://kenny32954.github.io/nolimit-ai/

Running under HTTPS is important because browser features such as microphone access and reliable cross-origin API requests work properly there.

## Validation

Every push to main runs:

- HTML marker checks
- Inline JavaScript extraction
- node --check syntax validation

Workflow:

.github/workflows/validate.yml

## Storage

Chats, settings, and the locally connected key are stored in browser localStorage. That means:

- they stay on the device/browser you used;
- clearing site data clears them;
- different devices do not automatically share chat history.

Cross-device sync would require a real backend or a cloud storage integration and is intentionally not faked by this static-only version.

## Architecture

Browser
  -> OpenRouter API
  -> selected model/provider

The browser talks directly to OpenRouter over HTTPS. OpenRouter handles model routing and supported server-side tools.

## Important limitations

No third-party API client can literally duplicate every private feature inside ChatGPT, Claude, or Grok. This project instead exposes the capabilities OpenRouter and the browser actually provide.

Usage is also subject to:
- the selected model's context window;
- OpenRouter balance and rate limits;
- provider availability;
- model-specific multimodal/tool support;
- browser storage and memory limits.

There is no artificial message counter implemented by NoLimit AI itself.

## Current release

v1.0 foundation:
- phone + PC UI
- secure browser authentication choices
- multimodal chat
- research tools
- reasoning controls
- image generation
- agent subcalls and Fusion
- sandboxed HTML artifact previews
- speech-to-text
- text-to-speech
- model browser
- local conversation management
- automated syntax validation

