# examples/dasLLAMA - architecture

The contract every architecture doc follows is `ARCHITECTURE_COMMON.md` (repo root); the folder's
checklist is `REVIEW.md` beside this file. The engine these programs drive is documented under
`modules/dasLLAMA/`.

## 1. Charters

- `storyteller/` - a browser example: stories15M writes a children's story a few tokens per
  frame, KittenTTS reads each finished sentence. `main.das` is the whole program, `web_shell.html`
  the page around its canvas, `.das_package` the release, `models.json` its model set. The page
  is Dear ImGui through the `imgui_harness` (sec.3.7): one undecorated window over the viewport,
  the story wrapped to its width, a button that starts a story.
- `storywish/` - a browser example: the typed words become a request in the TinyStoriesInstruct
  corpus's layout, tinystories-instruct-27M writes the story, Pocket TTS reads it in one baked
  voice from a file without the codec encoder (text in, no packs, no cloning). Same four files;
  `wish.das` holds the request side pure (typed line -> words -> prompt, the field-line stop) so a
  test reaches it without a window. The same harness page as the storyteller, with the wish line
  above the story; on a touch device a bank of word chips fills the line, three words at most.
- `parrot/` - a browser example: you press record and talk, Silero VAD ends the take when you go
  quiet, Pocket TTS clones the voice from the take (a file with its codec encoder and its
  roster, which the picker offers beside the clone), and the text in the editor is read aloud in
  it on the say button; recording again replaces the voice. Its panels are Dear ImGui through
  the `imgui_harness` (sec.3.7): the take's waveform and level as it records, the say's chunks
  as they are generated and spoken, the stage times of each, the job queue's knobs. Same four
  files, plus `take.das`, the pure side (the take's numbers, the waveform columns, the chunk
  ledger) a test reaches without a window. Nothing leaves the program: the take is cloned in
  memory and never written.
- `wasm/dlim_config/` - a wasm-only program: prints the running build's DlimConfiguration JSON.
  `wasm/mint_models.py` - the deploy's staging step for a browser example's model set.
  `wasm/run_node.js` - runs the wasm64 engine host under node.
- `library/` - the engine behind a C ABI, for a host that is not daslang: `dasllama_lib.das` is
  the library (flat `[export_c]` entry points over the facade, three surfaces - text completion,
  speech to text and speech synthesis), and `main.c`, `main.cpp` and `main.das` are the three hosts that drive it. Its
  `README.md` is the page a reader starts from.
- `chat.das`, `run.das`, `speak.das`, `speak_server.das`, `transcribe.das`, `dictate.das`,
  `audio_chat.das` - desktop-only programs over the facade; nothing here ships them to a page.

## 2. Definitions

- **A browser example** is a folder with a `web_shell.html`, wherever under `examples/` it sits: `daspkg release
  wasm` builds it to wasm64 for dasllama.io from the same `main.das` the desktop run uses. The
  checklist's rules about the browser build bind browser examples and nothing else.
- **A witness line** is a line a browser example logs under its own name (`storyteller: ...`,
  `storywish: ...`, `parrot: ...`); a smoke test under `modules/dasLLAMA/tests/` matches such
  lines as substrings, so their words and order are an interface.

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
pushes its requests into a stream as archived records (a sentence for the story examples; for
parrot a text to say or a take to clone, the PCM riding in the record) and pops finished clips
from a second stream; a `SeqBox` carries the number of the story (parrot: the say) being told, so
a queued sentence of one the user replaced is skipped instead of synthesized. The thread's own
setup - the TTS model path and the voice - rides the same request stream ahead of the first
request, and so does an engine knob the runtime keeps per context: the frame thread cannot reach
the speech thread's copy of one, so it sends the value and the thread sets it on itself. Parrot's
thread answers a say with its chunk count before the first clip, then each chunk's text before it
synthesizes it and the clip with its stage times after, so the frame
thread can show what is being generated and tell the last clip from a pause. A string captured by the thread's lambda would be a
pointer into the frame thread's heap, which that thread reuses on its own schedule; a browser
worker starts slowly enough to read story text where the path was. An archived message is copied
out of the stream into the reader's heap, so the stream is the one channel that is safe for a
string.

### 3.3 Input is polled

Every browser example reads its input inside its own frame. One on the imgui harness (sec.3.7),
which every example is today, reads ImGui's key and mouse state there - `IsKeyPressed` edge-detects.
One that draws its own text reads the keyboard with `glfwGetKey` each frame, edge-detected per key
code. Neither installs a GLFW callback. In the browser build a callback
lambda fires from a JavaScript event outside any frame of the program, where the example's state
is not live, and the program traps. A printable GLFW key code is its upper-case ASCII, so the key
range doubles as the character range for a typed line, and repeats come from a hold timer. The
mouse is read the same way: a click is `glfwGetMouseButton` edge-detected against a label's own
box in design pixels. On the imgui harness the backend's callbacks are C++ and enter no daslang
code. In the browser the GLFW window is reconciled every frame to the
box the canvas may fill - its parent's box, the stage below the nav (dasGlfw's glue reads the
document viewport only when the parent is the body, or in fullscreen) - so the program's
picture is the stage; a program that keeps its own aspect is letterboxed inside it. Emscripten
maps a click through the canvas element's box with one ratio per axis, which is exact only when
that box is the picture - so a shell sizes the canvas element to the letterboxed box
(`max-width`/`max-height` on the replaced element) instead of stretching it over the stage with
`object-fit`.

### 3.4 The model set is minted for the build that ships it

Each browser example's `models.json` names its source files by Hugging Face repository, file
and sha256, in three lists: `images` (a GGUF the build bakes into a `.dlim`), `packs` (a
front-end pack) and `files` (a GGUF that is its own served form - a Pocket TTS file); a fourth
list, `tree`, names a file the repository itself carries by its repo-relative path and sha256
(the voice-activity weights). `wasm/mint_models.py`
fetches the published ones (cached by sha256), bakes each image against the wasm64 build's own
DlimConfiguration, copies the packs, files and tree files as they are, writes
`models/manifest.json` (the file list, their sizes, the IMAGE_VERSION the images carry - a set
with no image carries the version the deploy expects) and
stamps that version into the page's `/* @image-version */ 0` slot. The shell reads the manifest,
refuses a set minted for another version before fetching it, and shows a program abort's last
engine lines on the page. A fetched file is put in the origin's Cache Storage under its URL and
sha256 once its bytes hash to that sha256 - a wrong body under an HTTP 200, a proxy's error
page, is refused and never cached, since the store would serve it on every later visit - and a
later visit reads it from there, so a set fetches once per browser and a re-minted file (a new
hash) fetches alone; the page's files the manifest no longer names leave the store after the
set is read (the three pages share one store, so each sweeps only the keys under its own
models path); a browser without the store fetches every time. The configuration the mint bakes against comes from the wasm build
itself: `wasm/dlim_config/` is cross-compiled and run under node in the deploy, because an image
is keyed by the build's identity and a set minted for a previous build is declined by the next.

### 3.5 The wasm64 build's configuration probe {#dlim-config-probe}

`wasm/dlim_config/main.das` prints `dlim_config_json(dlim_config_current())` and nothing else.
Its `.das_package` disables the GPU tiers exactly as the browser examples' do, so the
configuration it prints is the one their programs run with.

### 3.6 Parrot's take {#the-take}

The microphone is opened at the speech model's own rate (the speech thread reports it, with
whether the file clones, before the first take), mono, and drained on the frame thread every
frame into the take; the take is resampled to 16 kHz by linear interpolation with one running
position across drains, so the stream the Silero iterator hears has no seam where the drains
met, and the iterator is the only reader of that rate. The take ends on the stop button, two
seconds after the iterator's last speech end, at the model's 60 s clip cap, or when the device
has delivered nothing for six seconds (a refused microphone opens but never delivers); the ring's
tail is drained before the device stops, since stopping frees the ring. The clip is the speech
plus a quarter second at each end, never longer than the cap, and goes to the speech thread as a
clone request, so the clone runs off the frame thread like a synthesis. A take with no speech in
it is dropped, and the status says whether the device gave nothing, silence, or too little. A
take is refused while a say is audible or queued - a clip still playing would be recorded - the
record disc dim and inert until the say ends, and the pure side of all this (`take.das`) is what the model-free cells test. The gain slider scales the
frames as they land in the take, before the detector hears them and before the clone, so it
reaches a quiet microphone; the physical input gain is the browser's or the system's, out of the
program's reach.

### 3.7 Parrot's panels {#parrot-panels}

Parrot draws with Dear ImGui through `imgui_harness`, the same lifecycle the graphics labs run
in the browser: a dockspace over the viewport with five dock windows the user can split and
tab at will, laid out once on the first frame - `parrot` across the top (the promise that the
recording stays in the window as its first line, a status chip that pulses with a spinner
beside it while anything is going on, the status line), the voice on the left (the record disc
with the silence countdown drawn around it, the level, the gain, the take's waveform with its
kept window shaded, the voice picker), the text in the middle (the editor, the play and stop
icon buttons naming the voice, the say's chunks in their state's colour) with the output below
it (the say's waveform growing chunk by chunk with a tick at each chunk's start and the
playhead, the per-chunk table of stage times), and the lab on the right (the model's facts, the
job queue's knobs, the measure button). A window narrower than 760 px, or taller than wide, gets
one column instead: the status strip, the voice over the text, with the lab and the output behind
them as tabs. In that column the voice shares a node with the lab and the text with the output, and
a node's first tab bar makes its own pick, so once the column layout is built the program focuses
`text` then `voice` for two frames: a phone opens on the record disc and the editor. A touch device,
wide or narrow, gets the say buttons grown to a thumb's size. The shell keeps the space
bar's keydown for the editor:
a prevented keydown swallows the keypress the editor reads, and the page cannot scroll anyway;
the editor takes Tab as a character (`AllowTabInput`), since a Tab that moves focus is no use
in a text one types into.
The say is a ledger of chunks (`ChunkRow` in `take.das`): the speech thread answers a say with
its chunk count, then each chunk's text before its synthesis and its clip with the stage times
after, so a chunk reads pending, generating, generated, speaking and spoken in turn, and the
say is read out when every chunk played. The playhead is the mixer's own clock: each clip gets
a fresh status box through `set_status_update`, read every frame for its playback position and
its stopped state, and released when the clip ends - a box shared across clips reads the last
clip's stop as the next one's, which cuts the say short. A waveform is min/max columns of ten
milliseconds each, grown as frames land and redone from the column a drain left open, drawn one
pixel per column or per group of columns. The job queue knobs reach the queue the speech thread
dispatches on, applied after the thread's own setup has run, since that setup restores the
engine's defaults: the jobs slider goes through the engine's own cap (`set_dispatch_worker_limit`,
which also stands the hybrid pool's per-phase parking down - a raw `set_jobque_worker_limit` is
overridden by it at the next phase) and the live queue's limit, the toggles through
`set_jobque_team_mode` and `set_jobque_worker_spin`, and the spin toggle starts from the engine's
own window (`get_jobque_spin_us`, 30 ms on every platform including a browser), turning back on
the value it read - or a desktop's 30 ms where a box profile parked it;
the single-thread checkbox rides to the speech thread as an ask, because the flag it sets
(`set_single_thread`) is a context global and the thread's context is its own, and it greys the
three queue knobs it makes inert;
the measure button says the text three times with playback off and reports each run's speed as
times real time - seconds of audio per second of generation, the inverse of the engine's
real-time factor - the same figure on any box. The lab shows the mixer's underrun count while it
is nonzero (`sound_playback_underrun_frames`, which only the browser build counts): the mixer
thread did not refill the output ring before the device drained it, and the device played SILENCE
for that many frames - the clicking a clip is heard with comes from there, not from the mix. The
output waveform keeps a fixed time scale
(thirty seconds at least) so the playhead moves at one speed while chunks land. The chords are Ctrl (or Command) with Enter to say and with R
to record and stop.

### 3.8 The library's two backends

`library/dasllama_lib.das` is built by both standalone backends from the one source, and the
difference decides what a host can do with it. `daslang -lib` jits it into a native shared
library with a C ABI; `-ctx` emits a C++ header plus one translation unit the host compiles. The
C surface is identical - one describer writes both headers - while the C++ surface exists only on
the `-ctx` side, because a jitted library has no C++ source to put a real class on top of.

Both emissions leave dasVulkan out, and the `-ctx` one leaves dasLLVM out as well. The GPU tier
would pull tens of megabytes of kernel C++ into a translation unit that is already about 33 MB,
and the tune framework is a macro module with no AOT form: a standalone context links no
interpreter, so every function it reaches must have a C++ body, and a module marked no-AOT as a
whole has none.

A library is entered and left once per call, which is what separates this program from the
scripts beside it. `with_job_que()` is a scope no C caller can hold open, so the library opens
the queue persistently (`create_job_que`) and destroys only a queue it made itself, and only once
none of its three surfaces holds a model; and the engine's `stats(session)` counters are filled
by the one-call `generate`, which a caller pulling token by token never runs, so the library times
its own calls instead.

## 4. Exception ledger

None.
