# dasLLAMA text-to-speech Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_TTS.md`, `ARCHITECTURE_TTS_MEMORY.md`, `ARCHITECTURE_POCKET.md`. Planned
work: `followup_general.md`.

**A family's synthesis entry point (`styletts2_synthesize`, `pocket_synthesize`) carries
`[hot_path]`.**

**A text front-end stage (`dasllama/dasllama_textnorm.das`, `dasllama/dasllama_postag.das`,
`dasllama/dasllama_g2p.das`) called below a family's synthesis entry point is a defect -
phonemize before the entry point.**

**An array or table - a module global, or a field of the activation scratch struct one synthesis
reuses for every stage (`St2Scratch`, `PocketScratch`; never the model struct) - that a synthesis
sizes or fills, or that model load fills for syntheses to reuse, in a TTS source file under
`dasllama/`, that is not `@scratch` on its declaration or on the callee parameter it grows
through, is a defect.** The annotation is what lets `[hot_path]` hold through every stage the
entry point drives. A TTS source file is `dasllama_tts.das`, `dasllama_tts_types.das`,
`dasllama_tts_blocks.das`, `dasllama_styletts2.das`, a TTS family file, or a text front-end file.

**A function that exists only for tests, debugging or profiling - a stats, trace or dump reader
no synthesis entry point calls - in a TTS source file under `dasllama/`, that is not
`[cold_path]`, is a defect.**

**A GEMM in `dasllama/dasllama_styletts2.das` or a TTS family file that does not go through
a kernel `dasllama/dasllama_tts_blocks.das` exports is a defect, hand-written dot-product
loops included.**

**A block in `dasllama/dasllama_tts_blocks.das` that gains a rows form (token-major [T][C])
ships its channel-major form and a cell in the `tests/` file that holds that kernel family's
cells, holding the two at the dot-envelope bar - each element within a tolerance times the sum
of `|w|*|x|` feeding it - in the same change.** The channel-major form is what the parity rail and
any GPU driver are checked against.

**A rows kernel whose result depends on how its row blocks split across the parallel workers
is a defect.** How a rows kernel stays split-invariant is `ARCHITECTURE_TTS.md#tts-two-layouts`.

**A new arithmetic path in `dasllama/dasllama_tts_blocks.das` - a kernel, a weight lane of one,
a layout, or a window form of one (a form that computes one window of the whole-row result at a
time) - ships a numeric cell in the same change, in the `tests/` file that holds that kernel
family's cells (`test_tts_blocks.das`; `test_tower_asr_kernels.das` for the shared resamplers),
against the leaf it applies per row - the single-row reference kernel the path calls for each
row - or a double-precision form of its arithmetic; a window form also ships a cell holding it
bit for bit equal to the whole-row form.**

**A new arithmetic path in `dasllama/dasllama_tts_blocks.das` whose rows split across workers -
in its own `maybe_parallel_for` / `lanes_for_work`, or in a backend kernel it hands a row block
to - also ships a bit-equality cell on both axes that move the split, the batch lane cap and the
jobque worker limit.**

**A `read_*` call in `dasllama/dasllama_styletts2.das` that leaves a conv or linear on the
channel-major default while the forward assembly runs it through a rows kernel is a defect -
pass the consumer (`rows`, `rows_only`) so `conv1d_prepare` / `linear_prepare` drop the
layout nobody reads.**

**A caller that pins a TTS weight lane (`set_tts_q8` / `set_styletts2_q8` / `set_pocket_q8`)
for a load pins in the context that loads, never through the context that spawned it.** A
`new_thread` context starts every module global at its declared default, so a pin set in another
context never arrives.

**A diff that reorders the float operations, or changes the rounding of any step, of the phase
the CPU harmonic source builds in `dasllama/dasllama_tts_blocks.das` - the cycles, the resamples,
the cumulative sum - is a defect.** One float32 ulp of the accumulated phase is a hundredth of a
radian, so only the reference's own operation order reproduces the reference.

**A diff that moves or rewrites any step of the phase the CPU harmonic source builds in
`dasllama/dasllama_tts_blocks.das`, without changing its arithmetic, ships in the PR body the PCM
hash of one synthesis per family before and after, and the two match.**

**A tensor operator - a conv, a norm, an activation, a resampler, an LSTM, an RNG, or an STFT
step - implemented in a TTS family file is a defect; it goes in
`dasllama/dasllama_tts_blocks.das`.**

**Family BEHAVIOR in `dasllama/dasllama_tts_blocks.das` or `dasllama/dasllama_styletts2.das` -
a family-keyed branch, a tensor quirk, a symbol or token rule - is a defect; the quirk goes in
its family file.** Family DATA is not: the shared carrier holds each family's data record and
the reader that fills it from the gguf or the image (`ARCHITECTURE_TTS.md#tts-image-rail`).

**A diff that adds a field to a family's data record on the shared carrier (`KittenFamily`,
`KokoroFamily`) also serializes that field in `serialize_image_meta` and grows the field-count
`verify` beside it (`dasllama/dasllama_styletts2.das`), in the same change** - an unwritten
field reads back zero from a mapped image.

**A diff that moves what a served synthesis computes - a run with no flags and no environment
overrides, the text front end included (a moved phoneme of the rig corpus, which
`test_corpus_phonemes` in `tests/test_tts_g2p.das` decides) - ships the WER and UTMOS of
`harness/tts_rig.py`, before and after, on every model the change reaches, on every weight lane
that model can take - the unpinned default and each pin - at the rig's voice, in the PR body.** A
lane's per-frame figures against the f32 oracle say nothing about the speech; only the rig does.

**A text normalization or grapheme-to-phoneme error `harness/tts_rig.py`'s transcripts
expose lands as a failing-first case in `tests/test_tts_textnorm.das` or
`tests/test_tts_g2p.das` in the same change as its fix.**

**Never accept a voice or language a model's `caps()` does not declare, a cloning request when
`caps().cloning` is false, or a speed other than 1.0 when `caps().speed` is false - panic at
the call site instead.**

**A diff that makes a windowed stage a family file assembles from kernels - one that runs its
input a window at a time over a carry, the state one window hands to the next - produce a result
on the f32 lane that differs from the same stage run over the whole input in one pass by more
than float rounding (a few ulp per element) is a defect** (`ARCHITECTURE_TTS_MEMORY.md#tts-source-stream`,
`ARCHITECTURE_POCKET.md#pocket-codec-stream`).

**A diff that adds a windowed stage a family file assembles from kernels ships, in the same
change, the cell that runs that stage windowed and over the whole input in one pass and holds the
two together within float rounding, in the `tests/` file that holds that stage's cells.**

**A Pocket codec conv (`dasllama/dasllama_pocket.das`) carries its causal context as the
stream's carry - the rows its taps reach before a window, zero or edge-replicated ahead of the
first (`ARCHITECTURE_POCKET.md#pocket-codec-stream`) - and a diff that pads one symmetrically or trims the
final output by hand is a defect.**

**A change to which quant format a published file stores a Pocket tensor in, or to its layout
(`q8_linear` / `kq_tensor` / `head_q8_linear` / `dense_codec_conv` in
`harness/convert_pocket.py`, `read_linear` / `read_conv_q8` and the K-quant branch in
`dasllama/dasllama_pocket.das`), ships both sides in the same diff** - which tensors a format
covers is written once in the converter and again in the reader.

**A diff that changes what `harness/convert_pocket.py` writes into a published file re-mints
that file and lands its new bytes and sha256 in `harness/tts_model_card.md` and
`performance/model_specs.das`, and its new sha256 in every `examples/dasLLAMA/*/models.json`
(repo root) pinning it, in the same change.** The fetches refuse a file whose sha256 moved, and
the card is what a reader downloads against.
