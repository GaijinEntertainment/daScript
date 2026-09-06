# dasLLAMA text-to-speech Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_TTS.md`. Planned work: `followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
`REVIEW.md`.**

**The synthesis entry point `styletts2_synthesize` carries `[hot_path]`, and a model stage it
drives - a rows kernel in `dasllama/dasllama_tts_blocks.das`, the decoder and generator
assembly in `dasllama/dasllama_styletts2.das` - sizes every buffer through a `@scratch`
carrier so the annotation holds through it.** The text front end (the normalizer, the tagger,
the grapheme-to-phoneme pass, the token-id builders) builds strings and tables by construction
and sits outside the annotation.

**A buffer reused across syntheses in a file this checklist routes that is not `@scratch` -
on its declaration, or on the callee parameter it grows through - is a defect.** A `nolint`
where the annotation fits is a defect.

**A function that exists for debugging or profiling, in a file this checklist routes, that is
not `[cold_path]` is a defect.** A `nolint` where `[cold_path]` fits is a defect.

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

**A new rows kernel ships its `tests/test_tts_blocks.das` bit-equality cell on both axes that
move the split - the batch lane cap and the jobque worker limit - in the same change.**

**A `read_*` call in `dasllama/dasllama_styletts2.das` that leaves a conv or linear on the
channel-major default while the forward assembly runs it through a rows kernel is a defect -
pass the consumer (`rows`, `rows_only`) so `conv1d_prepare` / `linear_prepare` drop the
layout nobody reads.**

**A caller that pins the TTS weight lane (`set_tts_q8` / `set_styletts2_q8`) around a load
resets it (`reset_tts_q8` / `reset_styletts2_q8`) before returning, on every path out, panics
included - pin through `defer()` - and pins in the context that loads: a `new_thread` context
starts every module global at its declared default, so a worker that wants a lane pins where
it loads, never through the context that spawned it.** A pin that outlives its load silently
changes the lane of the next model loaded in the process; a pin set in another context never
arrives.

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
UTMOS of `harness/tts_rig.py`, before and after, on every model the change reaches at the
rig's voice, in the PR body - unless a corpus-identity cell in `tests/test_tts_g2p.das` proves
the change moves no phoneme of the 200-sentence corpus, which pins the audio bit for bit.**

**A text normalization or grapheme-to-phoneme error `harness/tts_rig.py`'s transcripts
expose lands as a failing-first case in `tests/test_tts_textnorm.das` or
`tests/test_tts_g2p.das` in the same change as its fix.**

**Never accept a voice or language a model's `caps()` does not declare, or a cloning request
when `caps()` says it cannot clone - panic at the call site instead.**
