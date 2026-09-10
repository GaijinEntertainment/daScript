# dasLLAMA text-to-speech Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_TTS.md`, `ARCHITECTURE_POCKET.md`. Planned work: `followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
`REVIEW.md`.**

**A family's synthesis entry point (`styletts2_synthesize`, `pocket_synthesize`) carries
`[hot_path]`.**

**A text front-end stage (`dasllama/dasllama_textnorm.das`, `dasllama/dasllama_postag.das`,
`dasllama/dasllama_g2p.das`) called below a family's synthesis entry point is a defect -
phonemize before the entry point.**

**A buffer reused across syntheses, or filled at load for syntheses to reuse, in a file this
checklist routes that is not `@scratch` - on its declaration, or on the callee parameter it
grows through - is a defect.** The annotation is what lets `[hot_path]` hold through every stage
the entry point drives.

**A function that exists for debugging or profiling, in a file this checklist routes, that is
not `[cold_path]` is a defect.**

**A GEMM in `dasllama/dasllama_styletts2.das` or a TTS family file that does not go through
a kernel `dasllama/dasllama_tts_blocks.das` exports is a defect, hand-written dot-product
loops included.**

**A block in `dasllama/dasllama_tts_blocks.das` that gains a rows form (token-major [T][C])
ships its channel-major form and a `tests/test_tts_blocks.das` cell holding the two at the
dot-envelope bar - each element within a tolerance times the sum of `|w|*|x|` feeding it -
in the same change.** The channel-major form is what the parity rail and any GPU driver are
checked against.

**A rows kernel whose result depends on how its row blocks split across the parallel workers
is a defect.** How a rows kernel stays split-invariant is the "Two layouts, one oracle" section
of `ARCHITECTURE_TTS.md`.

**A new arithmetic path in `dasllama/dasllama_tts_blocks.das` - a kernel, a weight lane of one,
a layout - ships a `tests/test_tts_blocks.das` numeric cell in the same change, against the leaf
it applies per row or a double-precision form of its arithmetic; a path whose rows split across
workers, wherever the split happens - its own `maybe_parallel_for` / `lanes_for_work`, or a
backend kernel it hands a row block to - also ships the bit-equality cell on both axes that move
the split, the batch lane cap and the jobque worker limit.**

**A `read_*` call in `dasllama/dasllama_styletts2.das` that leaves a conv or linear on the
channel-major default while the forward assembly runs it through a rows kernel is a defect -
pass the consumer (`rows`, `rows_only`) so `conv1d_prepare` / `linear_prepare` drop the
layout nobody reads.**

**A caller that pins a TTS weight lane (`set_tts_q8` / `set_styletts2_q8` / `set_pocket_q8`)
around a load resets it (`reset_tts_q8` / `reset_styletts2_q8` / `reset_pocket_q8`) before
returning, on every path out, panics included - pin through `defer()` - and pins in the context
that loads: a `new_thread` context starts every module global at its declared default, so a
worker that wants a lane pins where it loads, never through the context that spawned it.** A pin
that outlives its load silently changes the lane of the next model loaded in the process; a pin
set in another context never arrives.

**A diff that reorders the float operations of `sine_source` or `source_resize`
(`dasllama/dasllama_tts_blocks.das`), or changes the rounding of any step in the phase they
build, is a defect.** One float32 ulp of the accumulated phase is a hundredth of a radian, so
only the reference's own operation order reproduces the reference.

**A tensor operator - a conv, a norm, an activation, a resampler, an LSTM, an RNG, or an STFT
step - implemented in a TTS family file is a defect; it goes in
`dasllama/dasllama_tts_blocks.das`.**

**Family BEHAVIOR in `dasllama/dasllama_tts_blocks.das` or `dasllama/dasllama_styletts2.das` -
a family-keyed branch, a tensor quirk, a symbol or token rule - is a defect; the quirk goes in
its family file.** Family DATA is not: the shared carrier holds each family's data record and
the reader that fills it from the gguf or the image (`ARCHITECTURE_TTS.md` sec.2.32).

**A diff that adds a field to a family's data record on the shared carrier (`KittenFamily`,
`KokoroFamily`) also serializes that field in `serialize_image_meta` and grows the field-count
`verify` beside it (`dasllama/dasllama_styletts2.das`), in the same change** - an unwritten
field reads back zero from a mapped image.

**A change to a kernel, or a change to a lexicon, a normalizer rule or a phoneme rule that
moves a phoneme of the rig corpus (the corpus-identity cell in `tests/test_tts_g2p.das`
decides; an unmoved corpus pins the audio bit for bit), ships the WER and UTMOS of
`harness/tts_rig.py`, before and after, on every model the change reaches, on every weight
lane that model can take - the unpinned default and each pin - at the rig's voice, in the PR
body.** A lane's per-frame figures against the f32 oracle say nothing about the speech; only
the rig does.

**A text normalization or grapheme-to-phoneme error `harness/tts_rig.py`'s transcripts
expose lands as a failing-first case in `tests/test_tts_textnorm.das` or
`tests/test_tts_g2p.das` in the same change as its fix.**

**Never accept a voice or language a model's `caps()` does not declare, a cloning request when
`caps().cloning` is false, or a speed other than 1.0 when `caps().speed` is false - panic at
the call site instead.**

**A Pocket codec conv (`dasllama/dasllama_pocket.das`) carries its causal context as geometry -
`pad_l = k - stride` on a forward conv, `pad_r = k - stride` on a transposed one, the replicate
pad `pocket_encode_latents` applies before `mimi.downsample` - and a diff that gives one of
them conv state, a symmetric pad, or a trim of the output by hand is a defect.** The codec runs
a chunk in one shot (`ARCHITECTURE_POCKET.md`, "The codec runs a chunk in one shot");
`harness/pocket_oracle.py` checks the one-shot decode against the package's frame-by-frame output.

**A change to which quant format a published file stores a Pocket tensor in, or to its layout
(`q8_linear` / `q8_conv` / `kq_tensor` / `head_q8_linear` in `harness/convert_pocket.py`,
`read_linear` / `read_conv_q8` and the K-quant branch in `dasllama/dasllama_pocket.das`), ships
both sides in the same diff, and weakening `test_pocket_q8_file` or `test_pocket_kq_file` in
`tests/test_tts_pocket.das` is a defect** - the reader's eligibility rule and the converter's are
the same rule written twice.
