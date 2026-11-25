# dasLLAMA Architecture - the prepared-image rail, the exception ledger

Companion to `ARCHITECTURE_IMAGE.md`; this document carries the rail's ledgered cases - a lane that mints no image, a
load that serves with none, and the suite cells that mint one by design - each section cited by its anchor.

### The assistant drafter is a ledgered non-image lane {#drafter-non-image-lane}

**The gemma-4 assistant drafter (`dasllama_mtp_gemma.das`) reads its Q8_0 sidecar straight into
its own blob and never mints a `.dlim`.** The sidecar is 440 MiB, its Q8_0 blocks are already the
34-byte form the Metal encoder uploads unrepacked, and the copy takes a fraction of a second - an
image would shorten nothing. The lane is optional (no sidecar, no drafter) and invisible to
`dasllama-convert`'s list and GC by design.

### A gguf streamed onto a discrete home is a ledgered non-image load {#live-load-non-image}

**A q8 gguf loaded on an armed discrete home with no lane to write - the image rail off, the image save off, a save
that lost a write - is served with no image: the mint's walk places its planes on the device live** (`load_gguf_live`;
`ARCHITECTURE_ENGINE_MEMORY.md#gguf-live-load`, `#failed-save-restarts`). An image built in memory to be served and
thrown away would hold every plane on the host for a model whose device already holds them. The load is no second mint
path: the walk and its gather hook are the lane mint's, run with no sink.

### Suite cells that mint a model image by design {#suite-image-mints}

The loader rule of `tests/REVIEW.md` keeps a test whose subject is not the `.dlim` image rail off the rail. The cells below
fire it and are ruled acceptable, each with its reason.

- **The TTS facade cells** - the `image` form of a voice file's block (`styletts2_form`, `pocket_form` in
  `tests/_tts_parity.das`), loaded once for the cells of `tests/test_tts_kitten.das`, `tests/test_tts_kokoro.das`
  and `tests/test_tts_pocket.das` that read the facade's own load, `test_kokoro_ru_facade` among them,
  kitten nano's `image_cpu` form - that load under `with_cpu_flavor`, for the cells that hold the CPU chain behind
  the facade's model (`test_kitten_decode_hook`, and the streaming, scratch-release and chunk-cap cells of
  `tests/_tts_facade_cells.das`) - and the two cells of `test_kitten_stored_file` that load a stored file through
  `load_styletts2` (its lane choice under a pin is their subject; they wipe the images they mint beside their temp
  files). They
  load through the facade's `load_tts_model`, which bakes the family's `.dlim` beside the model when `DASLLAMA_IMAGE` is unset, so each mints an
  image per carrier. The reason: the facade's own load path is the cells' subject - for the Russian cell the packs it
  picks by the voices' declared languages - and they run under the suite child's identity - on the tuned arm the box's
  own, with no policy override - so the image they mint is the one the image suite's `kitten` arm and the serving rig read.
  The `image_cpu` form's reason: its cells run on every box, a box with no GPU device among them, where the block
  loads no minted CPU form of the lane an unpinned load serves (the q8 form is a skipped cell without its `@dev`
  form); under `with_cpu_flavor` the load bakes the unpinned lane's CPU-flavor image, the one a box with no GPU home
  serves.
- **The Metal MTP rails** - the fixtures of `tests/test_metal_mtp_parity.das` that load their served twin through
  `load_model_cached`, and the Metal form of each 26B trunk's block in `tests/test_mtp_gemma_drafter.das`
  (`metal_form(image = true)`: the Q4_K_M target for the served arms and the regular DFlash drafter's Metal arm, the
  UD-IQ3_XXS one for `mtp-gdraft-iq3`), loaded once a block - mint the target's
  metal-blob `.dlim`. The reason: the metal-blob flavor is what the Metal MTP rail and the drafter's attach serve. They pin
  no lane, and `--no-tune` runs them with `DASLLAMA_IMAGE=0`, so the image they mint is the box's own.
- **The translation facade cells** - `test_nmt_llm_translator`, `test_nmt_chat_translator` and
  `test_nmt_chat_thinking_off` in `tests/test_nmt.das`, and the `image` and `image_f32` forms of the ru-en base-memory
  student's block in `test_nmt_ru_en_pairs`, load through `load_translator`: a GGUF through `load_model_cached`, a
  Bergamot student through `load_nmt_model`, each baking its `.dlim` beside the model (`bergamot-q8`, and `bergamot-f32` for
  the `exact = true` load). `test_translate_facade_contracts` loads a synthetic student the same way, its image landing in
  the test's temp directory beside the student. The reason: the facade's load is the cells' subject - the kind it picks
  off the file and the lane `exact` asks - and it has no in-memory form; they pin no lane, and `--no-tune` runs them with
  `DASLLAMA_IMAGE=0`, so the image they mint is the box's own.
- **The gemma4a facade cells** - `test_gemma4a_e4b_audio_facade` (the E4B pair) and `test_gemma4a_audio_oracle` (E2B),
  cells of `tests/_audio_asr_cells.das` on the `image_asrdev` and `image_asr` forms of the mmprojs' blocks in
  `tests/test_audio.das` - load through `load_asr_model(dec, mmproj)`, which bakes the decoder's `.dlim` (`load_model_cached`)
  and the encoder's lane image (`load_gemma4a_encoder`) beside the files. The reason: the facade's pairing is the cells'
  subject - the projector-type sniff routing the mmproj to gemma4a, the decoder width the embedder must project to, the lane
  the facade picks - and it has no in-memory form; they pin no lane, and `--no-tune` runs them with `DASLLAMA_IMAGE=0`, so
  the images they mint are the box's own.
- **The load-once block's cells** - `test_gpu_resident_gemma3` (`tests/test_gpu_resident_gemma2_3.das`) and
  `test_gpu_resident_gemma4_e2b` (`tests/test_gpu_resident_gemma4_e2b.das`), and any cell whose `with_cpu_and_gpu_model` call
  (`tests/_cpu_gpu_model.das`) lists three or more device forms that keep no host planes, where the image rail is on: the
  first such form streams the file, and each later one loads through `load_model_cached` (`load_resident_lane`), which
  mints the file's device lane beside it once and maps it after. The reason: a streamed load rebuilds the device's planes
  from the file's blocks for every form, the same work each time, and a mapped lane skips it. The block removes each lane
  it minted when its last device form ends and no image that stood beside the file before it; where a load renamed one
  of those to its lane's name, the block removes nothing. The forms pin no lane, so the image is the box's own.
