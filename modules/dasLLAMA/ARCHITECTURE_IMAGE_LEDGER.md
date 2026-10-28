# dasLLAMA Architecture - the prepared-image rail, the exception ledger

Companion to `ARCHITECTURE_IMAGE.md`; this document carries the rail's ledgered cases - a lane that mints no image, and the
suite cells that mint one by design - each section cited by its anchor.

### The assistant drafter is a ledgered non-image lane {#drafter-non-image-lane}

**The gemma-4 assistant drafter (`dasllama_mtp_gemma.das`) reads its Q8_0 sidecar straight into
its own blob and never mints a `.dlim`.** The sidecar is 440 MiB, its Q8_0 blocks are already the
34-byte form the Metal encoder uploads unrepacked, and the copy takes a fraction of a second - an
image would shorten nothing. The lane is optional (no sidecar, no drafter) and invisible to
`dasllama-convert`'s list and GC by design.

### Suite cells that mint a model image by design {#suite-image-mints}

The loader rule of `tests/REVIEW.md` keeps a test whose subject is not the `.dlim` image rail off the rail. The cells below
fire it and are ruled acceptable, each with its reason.

- **The TTS facade smoke cells** - `facade_smoke` in `tests/_tts_parity.das`, called from `tests/test_tts_kitten.das` and
  `tests/test_tts_kokoro.das`. They load through the facade's `load_tts_model`, which bakes the StyleTTS2 `.dlim` beside
  the model when `DASLLAMA_IMAGE` is unset, so each mints an image per carrier. The reason: the facade's own load path is
  the cells' subject, and they run under the suite child's identity - on the tuned arm the box's own, with no policy
  override - so the image they mint is the one the image suite's `kitten` arm and the serving rig read.
- **The Metal MTP rails** - the fixtures of `tests/test_metal_mtp_parity.das` that load their served twin through
  `load_model_cached`, and every served arm of `tests/test_mtp_gemma_drafter.das` on the Metal home, each loading its
  target through `gdraft_target` (the Q4_K_M target and, for `mtp-gdraft-iq3`, the UD-IQ3_XXS one) - mint the target's
  metal-blob `.dlim`. The reason: the metal-blob flavor is what the Metal MTP rail and the drafter's attach serve. They pin
  no lane, and `--no-tune` runs them with `DASLLAMA_IMAGE=0`, so the image they mint is the box's own.
- **The translation facade cells** - `test_nmt_llm_translator`, `test_nmt_chat_translator` and
  `test_nmt_chat_thinking_off` in `tests/test_nmt.das` load through `load_translator`: a GGUF through `load_model_cached`, a
  Bergamot student through `load_nmt_model`, each baking its `.dlim` beside the model (`bergamot-q8`, and `bergamot-f32` for
  the `exact = true` load). `test_translate_facade_contracts` loads a synthetic student the same way, its image landing in
  the test's temp directory beside the student. The reason: the facade's load is the cells' subject - the kind it picks
  off the file and the lane `exact` asks - and it has no in-memory form; they pin no lane, and `--no-tune` runs them with
  `DASLLAMA_IMAGE=0`, so the image they mint is the box's own.
