# dasLLAMA text-to-speech Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEDIA.md`.

**Routed from `REVIEW.md`: a diff touching `dasllama/dasllama_tts.das`,
`dasllama/dasllama_tts_types.das`, `dasllama/dasllama_tts_blocks.das`,
`dasllama/dasllama_styletts2.das`, a TTS family file - one `dasllama/dasllama_<family>.das`
holding a single speech-synthesis family - a text front-end file - one stage of the pass
that turns text into phonemes (`dasllama/dasllama_textnorm.das`,
`dasllama/dasllama_postag.das`, `dasllama/dasllama_g2p.das`) - or a call that pins the TTS
weight lane (`set_tts_q8`), wherever the diff puts it, applies this list together with
`REVIEW.md`.**

**A synthesis entry point - `styletts2_synthesize`, and any function `dasllama/dasllama_tts.das`
calls to produce audio - carries `[hot_path]`.**

**A buffer reused across syntheses in `dasllama/dasllama_tts_blocks.das`,
`dasllama/dasllama_styletts2.das`, or a TTS family file that is not `@scratch` - on its
declaration, or on the callee parameter it grows through - is a defect.** A `nolint` where
the annotation fits is a defect.

**A debug or profiling code path in a file this checklist routes that is not `[cold_path]`
is a defect.** A `nolint` where `[cold_path]` fits is a defect.

**A GEMM in `dasllama/dasllama_styletts2.das` or a TTS family file that does not go through
a kernel `dasllama/dasllama_tts_blocks.das` exports is a defect, hand-written dot-product
loops included.**

**A block in `dasllama/dasllama_tts_blocks.das` that gains a rows form (token-major [T][C])
ships its channel-major form and a `tests/test_tts_blocks.das` cell holding the two at the
dot-envelope bar - each element within a tolerance times the sum of `|w|*|x|` feeding it -
in the same change.** The channel-major form is what the parity rail and any GPU driver are
checked against.

**A rows kernel whose result depends on how its row blocks split across the parallel workers
is a defect - a reduction accumulates per fixed-size row block and combines in a fixed
order.** The facade's streaming cell compares two syntheses sample for sample, and the
worker count differs between calls.

**A `read_*` call in `dasllama/dasllama_styletts2.das` that leaves a conv or linear on the
channel-major default while the forward assembly runs it through a rows kernel is a defect -
pass the consumer (`rows`, `rows_only`) so `conv1d_prepare` / `linear_prepare` drop the
layout nobody reads.**

**A caller that pins the TTS weight lane (`set_tts_q8`) around a load resets it
(`reset_tts_q8`) before returning, on every path out, panics included - pin through
`defer()`.** A pin that outlives its load silently changes the lane of the next model loaded
in the process.

**A diff that reorders the float operations of `sine_source` or `source_resize`
(`dasllama/dasllama_tts_blocks.das`), or changes the rounding of any step in the phase they
build, is a defect.** One float32 ulp of the accumulated phase is a hundredth of a radian, so
only the reference's own operation order reproduces the reference.

**A tensor operator - a conv, a norm, an activation, a resampler, an LSTM, an RNG, or an STFT
step - implemented in a TTS family file is a defect; it goes in
`dasllama/dasllama_tts_blocks.das`.**

**A family type, a family-keyed branch, or a family metadata key in
`dasllama/dasllama_tts_blocks.das` or `dasllama/dasllama_styletts2.das` is a defect - the
family's quirk goes in its family file.**

**A change to a kernel, a lexicon, a normalizer rule, or a phoneme rule ships the WER and
UTMOS of `harness/tts_rig.py`, before and after, on every model and voice the change
reaches, in the PR body.**

**A text normalization or grapheme-to-phoneme error `harness/tts_rig.py`'s transcripts
expose lands as a failing-first case in `tests/test_tts_textnorm.das` or
`tests/test_tts_g2p.das` in the same change as its fix.**

**Never accept a voice or language a model's `caps()` does not declare, or a cloning request
when `caps()` says it cannot clone - panic at the call site instead.**
