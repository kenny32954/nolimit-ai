# NoLimit AI

NoLimit AI is a responsive, single-file AI client for phones and PCs.

The actual application is still one file:

- index.html

Everything else in this repository supports the app with documentation and automated validation.

## Current feature set

### Chat
- Streaming OpenRouter chat completions
- Responsive mobile and desktop layouts
- Searchable local conversation history
- Stop generation
- Regenerate responses
- Edit a previous user message and resend from that point
- Branch a conversation from any assistant response
- Rename, duplicate, and delete conversations
- Copy responses
- Browser read-aloud
- AI-generated speech
- Export the current conversation as Markdown
- JSON chat backup and restore
- Automatic compatibility retry when optional model features cause a 400/422 response
- Detailed network errors instead of a generic "Failed to fetch"
- Token and cost display when OpenRouter returns usage data

### Models and routing
- Live OpenRouter model catalog
- Model search
- Filters for free, vision, and reasoning-capable models
- Auto Router support
- Primary plus fallback model routing
- Provider routing modes:
  - balanced
  - lowest price
  - highest throughput
  - lowest latency
- Optional provider data-collection denial preference
- Reasoning effort controls from none through extra high
- Optional response caching

### Web and research
- OpenRouter server-side web search
- OpenRouter server-side web fetch
- Reliable server-side datetime context
- One-tap Research mode
- Citations rendered as source links when the API returns annotations

### Agent orchestration
- Subagent mode for delegated focused work
- Advisor mode so the active model can consult a stronger model
- Configurable advisor model
- Fusion mode for multi-model deliberation
- Fusion remains opt-in because it can use more model calls, take longer, and cost more

### Multimodal files
- Image attachments for vision-capable models
- PDF attachments through OpenRouter PDF parsing
- Text, code, JSON, CSV, Markdown, HTML, CSS, and other text-file attachments
- Audio-file transcription before sending
- Paste image/file attachments from the clipboard
- Drag and drop attachments on desktop

### Voice
- Microphone recording in supported HTTPS browsers
- OpenRouter speech-to-text transcription
- Browser text-to-speech
- OpenRouter text-to-speech
- Speech-model catalog loading
- Download generated speech as MP3
- Optional automatic browser read-aloud

### Code and artifacts
- Rich Markdown rendering
- Copyable code blocks
- Sandboxed HTML artifact preview
- Open artifact in a separate opaque page
- Copy generated artifact HTML

### Image generation
- Dedicated image generation panel
- Live image-model catalog
- Aspect-ratio selection
- PNG, JPEG, and WebP output choices
- Add generated images into the current conversation

### Video generation
- Live video-model catalog
- Asynchronous video job submission
- Status polling
- Duration, resolution, aspect ratio, and audio controls
- In-app video playback
- Download completed videos

## Authentication

The public repository contains no private API key.

There are two connection choices:

1. Connect OpenRouter
   - Uses OpenRouter OAuth with PKCE.
   - The returned user-controlled key is stored only in that browser for this site.

2. Use my API key
   - Lets you paste an OpenRouter key into Settings.
   - It is stored in that browser's local storage.
   - It is not written into the repository.

Never hard-code a private API key into index.html.

## Browser security

The app includes a Content Security Policy restricting network requests to OpenRouter and preventing plugin/object embedding.

HTML artifact previews run in a sandboxed iframe without same-origin access to the main NoLimit AI page.

Generated artifact pages opened in a new tab use an opaque data URL with noopener.

## GitHub Pages

To publish:

1. Open this repository on GitHub.
2. Go to Settings -> Pages.
3. Under Build and deployment, choose Deploy from a branch.
4. Select main and /(root).
5. Save.

Expected address:

https://kenny32954.github.io/nolimit-ai/

HTTPS is important for microphone permissions and reliable browser API requests.

## Validation

Every push to main runs automated checks for:

- required UI and feature markers
- inline JavaScript syntax
- accidental committed OpenRouter API keys

Workflow:

.github/workflows/validate.yml

## Storage

Chats, settings, and a locally connected key are kept in browser localStorage.

Large base64 images and PDFs are intentionally not persisted into chat history because browser storage quotas are small. Their visible filenames and textual conversation remain, but reloading the page may require reattaching large binary files.

Chat backups can be exported as JSON and imported on another device.

## Architecture

Browser
  -> OpenRouter API
  -> selected model / provider / server tools

There is no NoLimit AI message counter.

Actual usage is still subject to:
- OpenRouter balance and rate limits
- the selected model context window
- provider availability
- feature support for the selected model
- browser storage and memory limits

## Release

Current app generation: v1.2

Major capabilities now include:
- phone + PC UI
- OpenRouter OAuth and local key connection
- multimodal chat
- web research
- reasoning
- subagents
- advisor
- Fusion
- images
- video
- speech-to-text
- text-to-speech
- PDFs
- routing/fallback controls
- HTML artifact previews
- chat branching/editing
- backups
- automated validation and secret scanning
