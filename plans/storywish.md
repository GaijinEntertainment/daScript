# storywish - a second browser example, and the model set that cannot go stale

Branch `bbatkin/storywish`. One PR.

## Why

dasllama.io's storyteller (stories15M writing, KittenTTS reading) is the first dasLLAMA-in-the-browser
example. Two things came out of building a second one:

1. **The model set was minted by hand.** The `.dlim` images the page fetches were baked once,
   uploaded to the rolling `dasllama-web` GitHub release, and the deploy only checked that the
   files exist. IMAGE_VERSION moved 33 -> 35 on 2026-09-08; the deploy kept staging the old images;
   the page's own console said `dasLLAMA image: '/models/stories15M.dlim' declined - version 0x21
   (want 35)` and the visitor saw a black canvas. No gate, no message, no re-mint.
2. **A steerable story model in an arch dasLLAMA runs.** The official TinyStories-Instruct models
   are GPT-Neo; `tinystories-instruct-27M` is our llama-arch training on the same corpus (4K
   SentencePiece vocab, 27M parameters, 15 min on one H100, val loss 1.144), and it follows a
   `Words: a, b, c` request as well as the official 33M does.

## What lands

### The model set pipeline (fixes the storyteller today)

- **Source of record: Hugging Face.** `borisbat/dasllama-stories` (stories15M-Q8_0.gguf converted
  from llama2.c; tinystories-instruct-27M in Q8_0 GGUF, f32 GGUF, llama2.c `.bin`, tokenizer)
  beside the existing `borisbat/dasllama-tts` (kitten-nano.gguf, the two phoneme packs). Per-file
  `.LICENSE` sidecars and a sha256 table in each card. The rolling `dasllama-web` release retires.
- **Each example declares its set**: `examples/dasLLAMA/<example>/models.json` - for every file
  the HF repo, the file name, the sha256, and whether it is a model to mint or a pack to copy.
- **The deploy mints, every time, against the build it just made.** `examples/dasLLAMA/wasm/dlim_config/`
  is a two-line package `daspkg release wasm` cross-compiles; run under node it prints the wasm64
  build's `dlim_config_json`. `dasllama-convert --config <that json>` then bakes each model's
  `.dlim` natively (the portable backend exists on every host, so the fail-closed verify passes).
  The GGUFs are cached by sha256 (`actions/cache`), the mint is seconds. The deploy writes
  `models/manifest.json` beside the page: IMAGE_VERSION, identity, and per-file name + bytes.
- **Three error surfaces, each for a failure the others miss:**
  1. deploy gate - the tree's IMAGE_VERSION against the manifest it just wrote (a mint that did not
     run, a stale cache): mismatch reds the deploy;
  2. the shell - reads `manifest.json` before fetching 70 MB and compares against the version
     the deploy stamped into the page; a stale set shows "minted at 33, this build needs 35";
  3. the program - a declined image is fatal with text (it already logs), and the shell's
     `Module.onAbort` puts that text back on the page instead of a black canvas.

### storywish - the example

`examples/dasLLAMA/storywish/`: `main.das` (desktop and wasm64 from one source, the storyteller's
skeleton - GLFW text on a canvas, the speech thread and clip queue unchanged), `web_shell.html`,
`.das_package`, `models.json`. New: a typed line - the words you wish for - and Enter tells the
story; a key toggles `Features: Dialogue`. Prompt = the corpus's own layout, `Story:` last:

```
Features: Dialogue
Words: dragon, cake, moon
Story: 

```

Context 512 (trained at 512), stop at EOS or at a line that opens with a field name (the corpus
orders its fields at random, so the model sometimes appends a `Summary:`), cap 350 tokens. TTS:
kitten-nano now; the new engine lands as a model-file swap in its own PR. Smoke rail
(`--smoke --words "dragon, cake, moon"`) and its test beside the storyteller's.

### Site and docs

A card and poster on `examples.html`, a news entry, the site README, the pages.yml step 7 loop
over both examples, `modules/dasLLAMA/performance/model_specs.das` row for the new model, the
storyteller's `models.json`.

## Not in this PR

- the new TTS engine (separate PR when it lands; storywish swaps the file)
- a smaller `s` shape (7M, 7 MB) - trainable in 2.5 h on the M5, worth it only if the 31 MB
  download turns out to matter
- retraining longer: loss was still falling at 9500 iterations (1.1443 vs 1.1447 at 9000 - the
  cosine floor, not the model's)

## Found on the way

- **A tree that cross-compiles to wasm must not also carry a desktop build.** The web build's
  module archives land in `<tree>/lib/` under the same names as the native ones; a native build
  there is newer than the sources, so ninja leaves the wasm targets alone and `daspkg build --wasm`
  stages native archives, and the link fails on every module symbol. `daspkg build --wasm` now
  refuses a native archive by its magic and says what to delete.
- **A string captured into the speech thread's lambda is a pointer into the frame thread's heap.**
  The storyteller passed the TTS model path that way and survived on luck: the frame thread reuses
  the block before a slow browser worker reads it when it allocates more per frame (storywish's
  wish line and status are interpolated every frame), and the thread then looked for the phoneme
  packs beside a story sentence. Both examples now send the path and the voice through the
  sentence stream ahead of the sentences; an archived line is copied into the thread's own heap.
- **In the browser build a GLFW callback lambda fires from a JavaScript event, outside any frame
  of the program.** The character callback trapped with `memory access out of bounds` on every
  keystroke even when it only stored the code point; the key callback storing into a global
  survives. storywish types from key codes through the key callback (a printable GLFW key code is
  its upper-case ASCII, repeats come free) into a fixed queue the frame drains; nothing builds a
  string inside a callback.
- **A declined image panicked as "not a prepared image for this box/knobs"** whatever the reason.
  The version mismatch - the one decline a reader can act on - now names both versions.
- `dasfmt --path <file>.das_package` ends with `[daslang atexit] FATAL: g_envTotal=1` on every
  package file, the existing storyteller's included; the tree-wide `--verify` CI run does not.
  Not touched here.

## Ledger

| item | state |
|---|---|
| model trained, exported, scored (`~/Work/tinystories-instruct/RUNBOOK.md`) | done 2026-09-09 |
| live breakage confirmed from the page console | done 2026-09-09 |
| HF `dasllama-stories` repo + card | done 2026-09-09 (16 files, per-file licences) |
| `models.json` per example + `dlim_config` probe + deploy mint + manifest + gate | done: `mint_models.py`, pages.yml step 7 + stage loop + source-model cache |
| shell version check + abort surfacing (both shells) | done, `test_metadata.py` covers both shells |
| storywish app + shell + package + smoke rail + test | done: `test_storywish.das` (3 unit cells + the smoke rail), registered in `run.das` |
| card, poster, news, README, model_specs row | done |
| local end-to-end: native run, wasm build, Playwright on serve.py | done 2026-09-09: both examples write and speak in Chrome from images minted at 35 |
| make_pr checklist | |
