# Quantum Breaks AI

Quantum Breaks AI is a responsive, single-file AI client for phones and PCs.

The actual application is still one file:

- index.html

Everything else in this repository supports the app with documentation and automated validation.

## Current feature set

### Quantum Breaks AI 2.5 additions
- Conversation branch-lineage navigator for parent/child chat forks
- Per-conversation Tasks board with optional AI task extraction
- Per-chat model locks and chat-specific instructions layered over project/global defaults
- Automatic context-fit protection that restores the draft instead of sending a doomed over-context request
- Configurable network-stall watchdog for requests that stop returning data
- Daily locally tracked spend warnings in addition to per-request cost warnings
- Model sorting and compatible-model selection learned from your own local 👍/👎 ratings
- Keyboard-shortcut help and a clickable conversation title for fast management
- Dynamic browser-tab titles and optional background completion notifications
- Tasks preserved through snapshots, branches, backups, and imports

### Quantum Breaks AI 2.4 additions
- User-approved, privacy-filtered local memory suggestions that explicitly exclude sensitive categories
- Review-driven answer improvement with local AI-response version history and restoration
- Multi-model answer synthesis after model comparisons
- Optional automatic incremental context summaries before long-chat trimming
- Per-chat and per-project model, reasoning, temperature, instructions, and context controls
- Searchable recent prompt history with keyboard recall and workspace-backup support
- Folder attachments with safe binary filtering and large-text truncation
- Persistent unsent image/PDF/text draft attachments across reloads
- Project export/import with associated chats and locally stored media
- Local response ratings plus model helpfulness analytics
- Searchable Settings navigation
- Local response editing with a visible edited marker and restore-original support
- Request cost estimates and optional cost-warning thresholds
- Stronger SSE parsing, multimodal response normalization, compatibility retries, and cross-chat generation safety
- One-click source copying and source counts on cited responses


### UI and organization
- Glassy responsive desktop and mobile interface
- Dark, Midnight, Ember, and OLED themes
- Coral, blue, violet, green, or fully custom accent colors
- Adjustable chat width, message size, and compact/cozy density
- Focus mode
- Grouped chat history with pinned, recent, archived, and trashed conversations
- Fullscreen image lightbox with zoom
- Command palette with keyboard navigation
- Global workspace search
- Selection toolbar for quoting, explaining, copying, prompt saving, and scratchpad capture

### App experience
- Share a chat with the Web Share API or clipboard fallback
- Pinned chats
- Recent-model filtering
- Approximate context-size meter in the composer footer
- Installable PWA support
- Offline app-shell caching
- Slash commands such as /help, /models, /research, /image, and /video
- Keyboard shortcuts including Ctrl/Cmd+K for models
- Online/offline connection status

### Recovery and persistence
- Autosaved composer drafts per conversation
- Recoverable Trash with one-click Undo
- Local IndexedDB persistence for image and PDF attachments
- Automatic media cleanup when chats are permanently deleted
- Plain workspace backups
- Password-encrypted workspace backups using PBKDF2-SHA256 + AES-256-GCM

### Power-user workflow
- Autosaved per-chat drafts
- Recoverable Trash with Undo
- Chat history grouped by Pinned, Today, Yesterday, Previous 7 days, and Older
- Smart Fast, Balanced, Deep, and Research modes
- Per-chat scratchpad with optional AI-context inclusion
- Pin individual messages so important context survives history trimming
- Workspace-wide search across chats, messages, projects, prompts, and AI profiles
- Saved AI profiles for instant model/tool/system-prompt switching
- Reusable prompt variables such as `{{topic}}`
- AI-generated conversation titles
- Manual long-chat context compression
- Clean Print / Save PDF conversation view
- Password-encrypted full workspace backups using PBKDF2 + AES-GCM
- Text-selection toolbar for copy, quote, explain, prompt saving, and scratchpad capture

### Reliability and workflow
- One-level undo for destructive edits and regenerations
- Conversation snapshots with restore points
- Cached model catalog with stale fallback when the catalog endpoint is unavailable
- Cross-tab workspace synchronization
- Image-model compatibility checks before sending attachments
- Browser storage quota warnings
- Recoverable deletes, archives, and portable chat backups with media
- OpenRouter response-cache hit/miss visibility in message metadata

### Chat
- Per-chat token/cost stats when usage is available
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

### Advanced context
- Smart Balanced, Fast, Deep, and Research modes
- Per-chat scratchpad with optional model-context injection
- Message-level context pins that survive history trimming
- Manual AI context compression for long chats
- Approximate context meter with selected-model context limits and input-cost hints
- AI-generated conversation titles

### Conversation organization
- Searchable conversation tags
- Conversation outline with click-to-jump navigation
- Local analytics for chats, messages, tokens, tracked cost, activity, attachments, and model usage
- Single-response Markdown downloads with citation preservation
- Citations retained in share, print/PDF, and Markdown exports

### Projects and memory
- Project-level model, reasoning, and temperature defaults
- Portable project export/import with chats and local media
- Local persistent memory that is appended to system context
- Local projects with project-specific instructions and reference knowledge
- Assign chats to projects
- Temporary chats that disappear after reload
- Search across chat titles, project names, and message content
- Full workspace export/import for chats, projects, memory, and settings

### Structured responses and review
- Review-driven answer improvement with local version history
- Multi-model comparison synthesis
- User-approved privacy-filtered memory suggestions that exclude sensitive information
- Normal text, JSON object, and strict JSON Schema response formats
- Provider routing that can require requested parameters
- Structured-output model filtering and capability badges
- Example confidence schema generator
- Optional second-pass AI answer review with confidence, verdict, and issue list
- On-demand review of any assistant response
- AI prompt improver before sending

### Models and routing
- Recent-model list kept locally
- Optional history-message cap to control context size
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

### Comparison and usage
- Current-key usage dashboard using OpenRouter's key endpoint
- Daily, weekly, monthly, total, limit, and remaining spend when available
- Side-by-side comparison across up to three models
- Per-comparison token and cost display

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

### Local data and media
- IndexedDB persistence for attached images and PDFs
- Attachment hydration after reload
- Storage manager with quota information and orphan cleanup
- Optional media-inclusive workspace backups for cross-device restore
- Password-encrypted workspace backups using PBKDF2-SHA256 and AES-256-GCM

### Multimodal files
- Folder attachments with relative paths and unsupported-binary filtering
- Unsent image/PDF/text draft attachments can survive reloads
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
- Download generated code blocks with a matching file extension
- Run JavaScript code blocks in a disposable Web Worker with a 5-second timeout
- Rich Markdown rendering
- Copyable code blocks
- Sandboxed HTML artifact preview
- Open artifact in a separate opaque page
- Copy generated artifact HTML

### Image generation
- Image-to-image reference input
- Multi-image output when supported
- Quality and background controls
- Controls automatically adapt to the selected model's live capabilities
- Dedicated image generation panel
- Live image-model catalog
- Aspect-ratio selection
- PNG, JPEG, and WebP output choices
- Add generated images into the current conversation

### Video generation
- First-frame image input for image-to-video
- Reference image input for reference-to-video
- Video controls automatically adapt to the selected model's supported durations, resolutions, aspect ratios, and audio capability
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

HTML artifact previews run in a sandboxed iframe without same-origin access to the main Quantum Breaks AI page.

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

Chats, settings, projects, prompts, profiles, and a locally connected key are kept in browser storage.

Image and PDF attachments are now persisted locally in IndexedDB, so supported media can survive normal page reloads and be rehydrated when the conversation needs it. Permanently deleting a chat also cleans up its stored media.

Plain JSON workspace backups remain focused on workspace data and are not a substitute for a full binary-media archive. Password-encrypted workspace backups use PBKDF2-SHA256 plus AES-256-GCM and never include the OpenRouter API key.

Chat backups can be exported as JSON and imported on another device.

## Architecture

Browser
  -> OpenRouter API
  -> selected model / provider / server tools

There is no Quantum Breaks AI message counter.

Actual usage is still subject to:
- OpenRouter balance and rate limits
- the selected model context window
- provider availability
- feature support for the selected model
- browser storage and memory limits

## Release

Current app generation: v4.0.0

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
