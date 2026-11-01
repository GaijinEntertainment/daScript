# How to add a speech-recognition family to dasLLAMA

This document walks the code in the order a speech-recognition (ASR) family lands - an encoder
plus a decoder head, with or without an LLM decoder - and cites the architecture section that
owns each step. The worked example is GigaAM v3 (a Conformer with rotary attention, a CTC or an
RNN-T head, one GGUF); parakeet (FastConformer + TDT, a ggml bin), canary (FastConformer + an LLM
decoder, a converted pair) and gemma4a (a Conformer tower beside an LLM, GGUF + mmproj) are the
precedents a step names where a family of their shape takes another path.

**A family is served when its transcript matches a reference token for token on the CPU and on
every GPU backend whose chain admits its shape, the rest declining by name.** The reference is the
oracle's dump (step 1) on the CPU and the CPU chain on each GPU backend.

## 0. Decide the family's shape before touching code

Answer these from the upstream model code and the carrier's metadata; the answers are the first
lines of the PR body.

1. **Carrier** - the file or file pair the weights ship in. A ggml bin (parakeet, whisper: a
   magic and a vocab size the sniff reads), a GGUF that is the whole model (GigaAM:
   `general.architecture` names it), a GGUF decoder plus an mmproj (gemma4a, qwen3a), or a
   checkpoint that needs a converter first (canary: `harness/convert_canary_to_ggml.py`, its
   recipe a catalog row). Every carrier serves through a prepared image
   (`ARCHITECTURE_IMAGE.md#there-is-one-way-to-load-a`). A lane is the weight precision a served
   model runs at: the exact lane serves the file's own precision, the q8 lane converts the block
   GEMMs to Q8_0. A carrier whose top precision is f16 makes the family's exact lane f16, not f32:
   its image tag says so (`gigaam-f16`) and its oracle rounds through f16 (step 1).
2. **Encoder class and the shared chain it runs on.** A whisper-class tower (`dasllama_audio.das`'s
   encode and block loop), a FastConformer (parakeet and canary share one block body over
   `CanaryLayerOffs`), a Conformer (gemma4a's chunked attention, GigaAM's macaron block), or an
   AuT-style tower. A block that matches an existing body joins it through an offsets record, as
   parakeet's does canary's; a new block form is the family's own loop over the tower home's
   pieces (`ARCHITECTURE_MEDIA.md#tower-audio-charters`) - a piece two families need moves up into
   `dasllama_tower.das`, never sideways.
3. **Attention form and what the GPU chains admit.** Transformer-XL rel-pos (`rel_pos_table`,
   FastConformer), rotary (`rope_neox_tab_rows` over tables from the one fill `build_rope_tabs`,
   `ARCHITECTURE_ENGINE_FORMATS.md#rope-one-fill`; GigaAM's NEOX half-split at base 5000), or
   chunked relative position (gemma4a). Read where the model's forward applies each op, not where
   the paper puts it: GigaAM rotates the normed rows ahead of the q and k projections
   (`ARCHITECTURE_MEDIA_ASR.md#gigaam-rotary-input`), and only the per-layer dumps tell that from
   rotating q and k. The CPU's `attention_bidir` takes any head width; each GPU chain admits
   head widths by its shape predicate (`fc_shape_ok` in `dasllama_metal_tower.das`,
   `vt_fc_blocks` in `dasllama_vulkan_tower.das`). List each backend's predicate and, for the
   family's head width, its answer: the chain serves it, a padded route
   (`ARCHITECTURE_GPU_TOWER.md#tower-attn-routes`) serves it, or the seat declines by shape.
4. **Front end.** Sample rate, mel count, `n_fft`, window and hop, centring, the log's floor and
   base, any per-feature normalization, dither or preemphasis, then the subsampling convs. A
   power-of-two FFT takes `build_fft_plan` + `fft_pow2_run`; any other size takes the twiddle DFT
   `build_dft_twiddles` (whisper's 400, GigaAM's 320), both from `dasllama_audio.das`
   (`ARCHITECTURE_MEDIA.md#tower-audio-charters`); the filterbank comes from the file when the
   file carries one.
5. **Decoder head.** CTC (a linear, a greedy argmax, repeats collapsed, blank dropped), RNN-T or TDT
   on the shared transducer home `dasllama_rnnt.das`, or an LLM session
   (`ARCHITECTURE_MEDIA.md#asr-decoder-session`). A transducer is `rnnt_decode` over an `RnntNet`
   descriptor of the family's weight offsets and widths, its flags naming the model's form
   (`zero_start`: the first step reads a zero input; `torch_gates`: gate rows i, f, g, o;
   `two_biases`: torch's `b_ih` and `b_hh`), an empty `durations` a plain RNN-T, and an `RnntEnc`
   naming the joint's encoder half; the family's block projects the encoder rows for the CPU loop
   (`rnnt_greedy`, off half-precision halves laid once a session,
   `ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step`), while a GPU build decodes the whole loop on the
   device, projecting there wherever the half sits in the f32 plane
   (`ARCHITECTURE_MEDIA_ASR.md#rnnt-device-decode`).
6. **Tokenizer.** SPM pieces through the shared `asr_spm_detokenize`, characters as they are, or
   the decoder LLM's BPE (`ARCHITECTURE_MEDIA_ASR.md#asr-tokens-long-form`); a detokenizer the
   family writes for itself duplicates the shared one.
7. **Language set and caps.** The languages, whether the model translates, timestamp granularity
   (the encoder frame length), streaming and prompting - the `AsrCaps` the family returns
   (`ARCHITECTURE_MEDIA_ASR.md#asr-tokens-long-form`). `create_session(m)` defaults to `""`, which
   the facade maps off the caps alone - the one listed code, `auto` for none, else `en` - so the
   family's check accepts whichever of those its caps imply.
8. **Clip window and long-form policy.** The longest clip the encoder takes whole, and what happens
   past it: whisper's 30 s windows, parakeet's local attention past `parakeet_max_frames()`, or
   silero-VAD spans (`dasllama_vad.das`) of at most the window, one segment a span with `t0` /
   `t1` from its bounds (GigaAM, past 25 s; `ARCHITECTURE_MEDIA_ASR.md#asr-tokens-long-form`). An
   upstream long-form path the oracle does not reproduce makes the long clip's check word
   agreement against the reference text, not a token compare.

## 1. The oracle - `benchmarks/asr/`

Build the oracle from the upstream package in a pinned venv the rig builds
(`ARCHITECTURE_MEDIA_ASR.md#asr-oracle`):

- Pin `benchmarks/asr/requirements-<family>.txt` (CPython 3.11), add the family's `ReferenceVenv`
  row (name, interpreter, import probe, env name) in `performance/setup_asr_rig.das` so `--venvs`
  creates `benchmarks/asr/.venv-<family>`, the interpreter's resolver `<family>_py()` over `ref_py`
  in `performance/profile_common.das`, and its override knob `<FAMILY>_PY` on `ProfilingEnv`
  (`dasllama_env.das`; regenerate `ENVIRONMENT.md`, `ARCHITECTURE_RUNTIME.md#env-knobs`).
- Write `benchmarks/asr/<family>_oracle.py` (precedents `gigaam_oracle.py`, `canary_qwen_bench.py`):
  CPU, fp32 activations, one `.npy` per stage under `asr_oracle/<family>/<clip>_<variant>/` in the
  model root (`tests/_model_tier.das`'s `asr_oracle_dir`) - mel, front, every layer, the encoder
  output, the head's logits - plus `tokens.json` (ids, frames, text), one dump set per carrier and
  head through `--variant`. The dumps are made locally; a box that runs the external-reference
  cells makes them again with the script, over the corpus clips.
- Round the weights the carrier stores at reduced precision through that precision before the
  forward, and only those (GigaAM's converter keeps biases, norms and the pre-encode convs f32).
- Run every script that reads downloaded data with `python -I`, from a path outside the data's
  folder.
- Read a `.npy` by its header's `fortran_order`; a reader that ignores the flag compares a
  transposed stage.

## 2. The family file - `dasllama/dasllama_<family>.das`

One file owns the family's weights, its encode, its decode loop and its quirks
(`ARCHITECTURE_MEDIA.md#tower-audio-charters`). It requires `dasllama_asr_types.das` and the tower
home, never another family file. In landing order:

- **The served struct and its staging pair.** `<Family>Model` holds planes that borrow the mapped
  image and the scalars; `<Family>Staging` owns the plane bytes the reader produced. Expose
  `stage_<family>_model`, `mint_<family>_model` (the in-memory rail suites use) and
  `load_<family>_model(path, q8)`, which maps a `.dlim` named directly (a wrong identity panics),
  else maps or builds the cached image, else builds it in memory - a served model is always
  image-backed (`ARCHITECTURE_IMAGE.md#there-is-one-way-to-load-a`).
- **The image tags.** Register both lanes at `[init]`: `register_image_family_tag("<family>-q8")`
  and the exact lane's tag with `config_free = true`, since its planes are the file's own
  (`ARCHITECTURE_IMAGE.md#image-lane-name`).
- **The meta serializer.** Hand-list every meta field and open with
  `verify(count_meta_fields(t) == <FAMILY>_META_FIELDS)`
  (`ARCHITECTURE_IMAGE.md#image-meta-tripwire`); derived tables (an FFT plan, twiddles, rope
  tables) rebuild in `image_post_load`. A new serializer moves the layout stamp: re-stamp
  `IMAGE_LAYOUT_STAMP_HASH` in `REVIEW.das` with the value the finding prints, leaving
  `IMAGE_VERSION` where it is when no existing layout moved
  (`ARCHITECTURE_IMAGE.md#image-layout-stamp`).
- **`finalize`.** Delete the owned arrays and release the image backing. Delete on an `array<T?>`
  frees the pointees - a list of pointers into borrowed planes is cleared first.
- **GEMMs.** Every weight GEMM goes through `tower_mm` / `tower_mm_rq` (any model carrying `q8`,
  `qblob`/`qscales` and `blob`), another `*_mm*` wrapper, or `mm_blob_b`, every
  activation-by-activation product through `gemm_f32_jo`. Served widths pad up to 64 with zero
  rows and columns (`ARCHITECTURE_MEDIA.md#tower-padded-widths`). The q8 lane transcodes the block
  GEMMs alone to Q8_0 at stage; a carrier that holds them quantized serves them as its own planes
  on the kq lane (GigaAM's `gemm_seats`, `tower_mm`'s seat arm); a head the carrier holds in f16 serves its own halfwords on both
  lanes (`mm_f16_b`), as do f16 depthwise taps (`fc_dw_bias_rows_f16` on the CPU, the TAP_F16
  stamps of `GkDwConvRows` on the devices - permuted at mint as halfwords, never widened into the
  image), and the front and the transducer stay f32 (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`).
- **The mel.** `<family>_log_mel(samples, m, st)` writes the state's mel rows from the transform
  step 0 named; the window and filterbank come from the file or from config.
- **The encode.** `<family>_encode(m, st)` returns the frame count: front, block loop, output
  norm. Every buffer reused across encodes is `@scratch`.
- **The decode.** CTC greedy in the family file; RNN-T/TDT through `dasllama_rnnt.das` with the
  family's weight offsets in its descriptor; an LLM decoder as a `Model` session
  (`ARCHITECTURE_MEDIA.md#asr-decoder-session`).
- **Transcribe, caps, language.** `<family>_transcribe(m, st, samples, blk, detected_lang)` runs
  mel, encode and decode, applies the long-form policy and emits `TranscribeSegment`s in
  centiseconds; `<family>_asr_caps`, `<family>_check_lang` and `<family>_exec_fmt` answer for the
  facade. An option `caps()` does not declare panics at the call site.
- **The GPU hooks.** One `var private` function slot plus a `register_<family>_gpu*` entry per
  seat the GPU can serve - the blocks, a front, or the whole encode - each answering decline in its
  return (`false`, `-1`), and the encode asking its hook first
  (`ARCHITECTURE_MEDIA.md#tower-gpu-hook`). `<family>_gpu_serves()` reads the driver's answer. A
  CTC head is a seat on every home - Metal's `metal_gigaam_ctc` and Vulkan's `vulkan_gigaam_ctc`
  over the shared pick `GkCtcPick` - reading the encode's held rows by generation and declining
  by name.
- **The lane pin.** `set_<family>_q8` / `reset_<family>_q8` / `<family>_serves_q8` over one
  `GemmLane` through `lane_serves_q8`, and a LOG_INFO line naming the lane and its reason at load
  (`ARCHITECTURE_MEDIA.md#tower-weight-lane`).
- **The profiling buckets.** `asr_prof_add` under `<prefix>.mel`, `.front`, `.blocks`,
  `.decode_total`, and `<FAMILY>_ENCODE_BUCKET = "<prefix>.encode_total"`
  (`ARCHITECTURE_MEASUREMENT.md#sanctioned-instrumentation-rails`); a debug or profiling path is
  `[cold_path]`.

Gate the file stage by stage against the oracle before step 3: a probe under `harness/` or a
cell of the family's test file holding mel, front, each layer and the encoder output to the dumps
(fp32 rounding, about 1e-4 relative, on the exact lane; the tower's q8 tolerance on the q8 lane).

## 3. The facade seams - `dasllama/dasllama_asr.das`

This file routes each facade function to one family, and a family appears in it only at its
`require` line, its union fields (`AsrModel`, and the session's state field in `AsrSession`), its
finalize line, its `AsrKind` value, its sniff check, and one-line forwarding arms in
`asr_exec_fmt`, `asr_encode_bucket`, `caps`, `create_session` and `transcribe`
(`ARCHITECTURE_MEDIA.md#tower-audio-charters`). The
sniff that picks the arm is the dispatch, in one order: the `.dlim` tag, the ggml magic and vocab
size, the GGUF arch, the mmproj pair (`ARCHITECTURE_MEDIA_ASR.md#asr-carrier-sniff`).

- A GGUF that is the whole model sniffs on `gguf_file_arch(path)` ahead of the one-argument
  overload's `pass its mmproj too` panic, which fires on every GGUF the sniff does not name; the
  family also answers `asr_needs_mmproj`, the question `examples/dasLLAMA/transcribe.das` asks.
- A GGUF + mmproj family sniffs in the two-argument overload on the mmproj (or a `.dlim`'s baked
  tag).
- A ggml bin sniffs on its magic and vocab size; a family whose vocab range overlaps another's
  takes a header field the other lacks.
- The load passes the lane: `!(g_asr_force_fp32 || g_asr_tower_fp32) && <family>_serves_q8()`.

## 4. The bake tool - `utils/dasllama-convert/`

The tool bakes a family's `.dlim`; `ARCHITECTURE_ENGINE.md#instrumentation-and-support` says why
it sets `options _dasllama_internal`. Three edits:

- `convert_sniff.das`: an `AsrCarrierKind` member and its arm in `sniff_asr_kind`, in the
  facade's order (`ARCHITECTURE_MEDIA_ASR.md#asr-carrier-sniff`).
- `main.das`: the load arm, its image tag taken from the family file's exported tag function
  (`<family>-q8` or the exact lane's tag), never a literal.
- `test_convert.das`: the family's carrier in the sniff cell.

## 5. The module gates - `REVIEW.das`, `.das_module`

- A file outside the family's own that names its types joins `ASR_SEAM_EXEMPT` in `REVIEW.das` (a
  licensed namer) or `ASR_KIND_FILES` (an `AsrKind` served by another kind's family file), and its
  line in `ARCHITECTURE_MEDIA.md#tower-audio-charters` in the same change; `check_family_seams`
  fails any namer not listed. A type two families share moves up into `dasllama_asr_types.das`
  (where `CanaryLayerOffs` sits), so no family file names another's.
- Add `dasllama_<family>` (and a new shared home such as `dasllama_rnnt`) to the module list in
  `.das_module`; `gate_descriptor_census` fails a module file the descriptor does not list.
- Run `./bin/daslang -jit modules/dasLLAMA/REVIEW.das` from the repo root; it exits 0 clean.

## 6. Tests - `tests/`

The test folder's documents track its files (`ARCHITECTURE_ENGINE.md#test-folder-gates`).

- **The family's test file** `tests/test_<family>.das` (stocked: it reads `models_dir()`; it
  declares `options persistent_heap`): the stage external-reference cells against the oracle's
  dumps, and the transcript token for token against `tokens.json` on each lane and head, and the
  long clip's word agreement against the corpus text. Each cell skips with a logged reason
  without its carrier or its dumps, and each compare carries a control that must miss (a poisoned
  stage, another clip's dump). Hold a running difference through `nan_wide_diff`, never
  `max(m, abs(a - b))`, which
  `check_running_max_nan` fails. Every cell that reads the oracle's dumps joins the pinned set in
  `tests/REVIEW_PINNED_GATES.md` in the same change, as named cells or as one file entry naming
  what it pins.
- **The runner** `tests/run.das`: the file in the stocked list and in `area_tests("audio")`, the
  family module's `MODULE_AREAS` row (`audio`), and the family's image arm in
  `area_image_arms("audio")` when `test_model_image.das` carries one.
- **The kind tables:** `test_asr_verbs.das` (`<family>_exec_fmt` per lane, the detokenizer over a
  toy vocab), `test_model_image.das` (the exact lane's tag in the list whose identity keys one
  image under every CPU kernel backend `kernel_backend_names()` lists that is not batch-only, and
  the family's image arm: a cold build, a warm map, element-exact planes),
  `test_kernel_coverage.das` (a `cov_tower` row once a GPU chain exists).
- **The document:** an entry for the new file in `tests/CLAUDE.md` naming its cells, and every
  `arm_on` token the file filters on inside a backtick span there - `check_arm_tokens_documented`
  and `check_doc_names_resolve` fail otherwise.
- **A shared-home change** (a lifted transducer, a tower piece moved up) keeps the existing
  family's output bit for bit: run that family's gate (`harness/pk_gate.sh` for parakeet) and its
  cells before and after.

Run `./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed` with `DAS_TUNE_MANIFEST` set
the way `tests/CLAUDE.md` says.

## 7. Provenance: the catalog, the corpus, the licences - `performance/`, `benchmarks/asr/corpus/`

- **The provenance pin** in `performance/model_specs.das`: exact Hugging Face repo, revision and
  sha256 per carrier, or the on-box conversion recipe
  (`ARCHITECTURE_ENGINE.md#instrumentation-and-support`). `performance/fetch_models.das` verifies
  it; a converted carrier adds its `--convert` arm there.
- **The board row** in `asr_catalog()` (`performance/profile_common.das`): display name, das model,
  the provenance pin, the other heads' carriers as `companions`, the clip `buckets`, `lang` (read
  by the board runner `benchmarks/lcpp_bench.das`) and the reference tools (`AsrRef`,
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references`). Prefer a variant of an existing tool to
  a new reference engine: GigaAM's reference is `onnx-asr`'s export as `tool = "onnx"` variants,
  its keys in `benchmarks/asr/onnx_bench.py` and in `setup_asr_rig.das`'s export fetch list.
- **The fixture clips:** 16 kHz mono wavs in the family's languages, each with a licence that
  admits redistribution. The corpus is gitignored: `setup_asr_rig.das --corpus` makes the clips (a
  fetch script per source dataset, `benchmarks/asr/fetch_fleurs_ru.py` through the family's venv)
  with their reference texts, and `benchmarks/asr/corpus.sha256` is the only checked-in pin. A
  test reads every corpus file through `tests/_model_tier.das`'s `asr_clip`, the path the rig fills.
- **The ASR bench** `benchmarks/asr/asr_bench.das` names the family where it pins a serving knob.
- **The licences:** `LICENSE.<FAMILY>` (the model's and, where code derives from one, the
  reference implementation's), its section in `THIRD_PARTY_NOTICES.md`, and its
  `install(FILES ...)` line in `modules/dasLLAMA/CMakeLists.txt`; a fixture clip's licence
  (`LICENSE.CC-BY-4.0`) is cited the same way. Licence text lives only there and in a model card.

## 8. The documents

- **The charter line:** the family file joins `ARCHITECTURE_MEDIA.md#tower-audio-charters`'s
  family list in the same change (`ARCHITECTURE.md#file-charters`), and a mechanism the family
  owns gets its own anchored section there, cited by the functions it describes.
- **The user surface:** `README.md`'s model list, the speech-to-text tutorial
  (`tutorials/dasLLAMA/07_speech_to_text.das` and its RST page), the server's catalog and docs
  (`utils/dasllama-server/`, `doc/source/reference/utils/dasllama_server.rst`), and the CLI and
  server's language fallback for a family without English; a new environment
  knob is a `dasllama_env.das` field and `ENVIRONMENT.md` regenerates from it
  (`harness/gen_env_doc.das`, `ARCHITECTURE_RUNTIME.md#env-knobs`).

## 9. Metal - `dasllama/dasllama_metal_tower.das`

A tower driver fills the family's seats from its `[init]` (`dasllama_metal_tower_register`,
`ARCHITECTURE_GPU_TOWER.md#tower-reach`).

- **The chain:** one command buffer per encode, running the family's CPU loop's operations in its
  dependency order and at the same operand shapes - fused or split across dispatches where that
  pays (`ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`). A
  block that matches an existing chain joins it through its offsets record (parakeet on canary's);
  a new block form gets its own body over the shared row templates
  (`dasllama_gpu_kernels_common.das`).
- **Admission:** a shape predicate beside `fc_shape_ok` answering decline, never an outage, for
  every shape, knob or lane the chain does not serve - the head width step 0, question 3 names
  first; a padded head uses `hs_pad` (`ARCHITECTURE_GPU_TOWER.md#tower-attn-routes`).
- **The seats:** `register_<family>_gpu(@@metal_<family>_blocks, @@metal_tower_serves)` and the
  front or whole-encode seat. Whether a seat served reads off its engage counters - the dispatch
  counters a GPU seat moves when it serves (`metal_tower_stats`) - never off the model running.
- **Tests:** a kernel cell per new class in `tests/test_metal_prefill_kernels.das` with its
  poisoned input, the class in the coverage census, and the family's `mtower` cell in
  `test_model_image.das` - CPU vs GPU transcript, encoder rows' rel-l2, counter deltas, and a
  control clip that must read other words.
- **The board:** `metal_served = true` on the catalog row once tower and decoder both serve
  (`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs`).

## 10. Vulkan - `dasllama/dasllama_vulkan_tower.das`

The Vulkan tower fills the same seats on a build without das_metal, from
`dasllama_vulkan_tower_register` (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-encode-chains`).

- **The chain:** the CPU loop's graph on the tower's row classes and attention routes
  (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes`); a decline is `vt_decline(r,
  VulkanTowerDecline.<why>)`, counted.
- **The seats:** `register_<family>_gpu(@@vulkan_<family>_blocks, @@vulkan_tower_serves_exact)`
  and the front or whole-encode seat; an LLM decoder runs on the box's decode policy, a
  hand-written decoder on the GPU takes an ASR-decoder driver
  (`ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder`).
- **Tests:** a cell per new class in `tests/test_vulkan_tower_kernels.das` through `CellPlanes` and
  `check_rows`, each with its poisoned element; the family's three-way twin in `test_audio.das`
  (`test_<family>_vulkan_twin`: the exact tower on the CPU, the q8 tower on the CPU and on the
  device, `tower_twin_bar`, the zeroed-blocks input poison that must exceed the bar); the class in
  the census (`VK_CENSUS_NEVER_DISPATCHED` where no stocked carrier dispatches it).

## 11. End to end

- **The external-reference rail:** the family's test file green on the exact and q8 lanes
  against the oracle, every head; on a GPU box, the twin cells green with their engage counters
  moving.
- **The area:** `tests/run.das -- --area audio`, then `--suite model-free`, then `--changed` on a
  box with the carriers (`tests/CLAUDE.md`).
- **The board row:** `performance/gen_bench_records.das -w asr` on the box writes the CPU pair,
  the das row against the catalog's reference tools
  (`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references`), under the audio outlier rule
  (`ARCHITECTURE_MEASUREMENT.md#audio-outlier-rule`); a GPU row pairs only where `metal_served`
  says so (`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs`).
- **The served cells:** every backend the PR claims serves the family holds a transcript cell
  with its engage counters; every other backend declines by name at the seat.
- **The dedupe:** every helper the family wrote checked against the shared homes before the PR
  (`skills/dupe_audit.md`, repo root) - a piece another family already carries moves up to the
  ASR floor `dasllama/dasllama_asr_types.das` or the tower home `dasllama/dasllama_tower.das`
  (`ARCHITECTURE_MEDIA.md#tower-audio-charters`) and both call it.
- **The PR:** `skills/internal/make_pr.md` (repo root), with step 0's answers in the body.
