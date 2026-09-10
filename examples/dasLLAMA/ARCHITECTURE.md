# examples/dasLLAMA - architecture

The contract every architecture doc follows is `ARCHITECTURE_COMMON.md` (repo root); the folder's
checklist is `REVIEW.md` beside this file. The engine these programs drive is documented under
`modules/dasLLAMA/`.

## 1. Charters

- `storyteller/` - a browser example: stories15M writes a children's story a few tokens per
  frame, KittenTTS reads each finished sentence. `main.das` is the whole program, `web_shell.html`
  the page around its canvas, `.das_package` the release, `models.json` its model set.
- `storywish/` - a browser example: the typed words become a request in the TinyStoriesInstruct
  corpus's layout, tinystories-instruct-27M writes the story, Pocket TTS reads it in one baked
  voice from a file without the codec encoder (text in, no packs, no cloning). Same four files;
  `wish.das` holds the request side pure (typed line -> words -> prompt, the field-line stop) so a
  test reaches it without a window.
- `wasm/dlim_config/` - a wasm-only program: prints the running build's DlimConfiguration JSON.
  `wasm/mint_models.py` - the deploy's staging step for a browser example's model set.
  `wasm/run_node.js` - runs the wasm64 engine host under node.
- `chat.das`, `run.das`, `speak.das`, `speak_server.das`, `transcribe.das`, `dictate.das`,
  `audio_chat.das` - desktop-only programs over the facade; nothing here ships them to a page.

## 2. Definitions

- **A browser example** is a subfolder of this folder with a `web_shell.html`: `daspkg release
  wasm` builds it to wasm64 for dasllama.io from the same `main.das` the desktop run uses. The
  checklist's rules about the browser build bind browser examples and nothing else.
- **A witness line** is a line a browser example logs under its own name (`storyteller: ...`,
  `storywish: ...`); a smoke test under `modules/dasLLAMA/tests/` matches such lines as
  substrings, so their words and order are an interface.

## 3. Mechanisms

### 3.1 One source, two builds

A browser example's `main.das` runs its frame loop through `eval_main_loop`, a blocking loop on
the desktop and `requestAnimationFrame` in the browser. The `.das_package` disables the GPU tiers
for the wasm build, so the host needs no Metal or Vulkan, and names the shell that fetches the
models and starts the program on a click (audio needs the gesture). The desktop run and the page
therefore exercise the same code, which is why a browser-only failure is a language-runtime fact
worth a rule rather than an app bug. A page the browser restores from its back-forward cache
(Back to another page, then Forward) comes back with the program's workers and the audio output
frozen mid-frame and out of step, and the first sound is whatever the output ring held; the
shell reloads such a page (`pageshow` with `persisted`), so it starts from the gate again.

### 3.2 The speech thread and its stream {#speech-thread-stream}

Speech runs on its own thread so the frame loop never blocks on synthesis. The frame thread
pushes sentences into a stream as archived `Line` records and pops finished clips from a second
stream; a `SeqBox` carries the number of the story being told, so a queued sentence of a story
the user replaced is skipped instead of synthesized. The thread's own setup - the TTS model path
and the voice - rides the same sentence stream ahead of the first sentence. A string captured by
the thread's lambda would be a pointer into the frame thread's heap, which that thread reuses on
its own schedule; a browser worker starts slowly enough to read story text where the path was.
An archived message is copied out of the stream into the reader's heap, so the stream is the one
channel that is safe for a string.

### 3.3 Input is polled {#polled-keys}

A browser example reads the keyboard with `glfwGetKey` each frame, edge-detected per key code,
never through a GLFW callback. In the browser build a callback lambda fires from a JavaScript
event outside any frame of the program, where the example's state is not live, and the program
traps. A printable GLFW key code is its upper-case ASCII, so the key range doubles as the
character range for a typed line, and repeats come from a hold timer.

### 3.4 The model set is minted for the build that ships it

Each browser example's `models.json` names its source files by Hugging Face repository, file
and sha256, in three lists: `images` (a GGUF the build bakes into a `.dlim`), `packs` (a
front-end pack) and `files` (a GGUF that is its own served form - a Pocket TTS file). `wasm/mint_models.py`
fetches them (cached by sha256), bakes each image against the wasm64 build's own
DlimConfiguration, copies the packs and files as they are, writes
`models/manifest.json` (the file list, their sizes, the IMAGE_VERSION the images carry) and
stamps that version into the page's `/* @image-version */ 0` slot. The shell reads the manifest,
refuses a set minted for another version before fetching it, and shows a program abort's last
engine lines on the page. The configuration the mint bakes against comes from the wasm build
itself: `wasm/dlim_config/` is cross-compiled and run under node in the deploy, because an image
is keyed by the build's identity and a set minted for a previous build is declined by the next.

### 3.5 The wasm64 build's configuration probe {#dlim-config-probe}

`wasm/dlim_config/main.das` prints `dlim_config_json(dlim_config_current())` and nothing else.
Its `.das_package` disables the GPU tiers exactly as the browser examples' do, so the
configuration it prints is the one their programs run with.

## 4. Exception ledger

None.
