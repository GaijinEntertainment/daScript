# dasLLAMA audio and ASR rules

**Routed from `REVIEW.md`: a diff touching `dasllama/dasllama_asr.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_tower.das` (with `REVIEW_VISION.md` - the
shared encoder-tower home serves both), `dasllama/dasllama_audio.das`,
`dasllama/dasllama_audio_io.das`, `dasllama/dasllama_audio_embedder.das`,
`dasllama/dasllama_vad.das`, or an ASR family file - one `dasllama/dasllama_<family>.das`
holding a single speech model family - applies this list with `REVIEW.md`'s.**
`REVIEW_COMMON.md` (repo root) binds this file too. Architecture doc: `ARCHITECTURE_MEDIA.md`.

**In `dasllama/dasllama_asr.das` and `dasllama/dasllama_audio_embedder.das` - the union
carriers that route each facade function to one family - a family arm that does anything but
one forwarding call is a defect; the work goes in the family file.** The loader's format or
family check that picks the arm is the dispatch, not an arm. A diff that adds a family changes
the carrier only at the union field, the finalize line, the kind value, and the one-line arms;
a prompt, a decode loop, a caps value, or a language rule in the carrier is a defect.

**A GEMM in an ASR family file that does not go through a `*_mm` wrapper or `mm_blob_b` is a
defect, hand-written dot-product loops included.**

**A buffer reused across encodes in `dasllama/dasllama_tower.das`,
`dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_embedder.das`, or an ASR family file
that is not `@scratch` - on its declaration, or on the callee parameter it grows through - is a
defect.** A `nolint` where the annotation fits is a defect.

**A debug or profiling code path in `dasllama/dasllama_tower.das`,
`dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_embedder.das`, or an ASR family file
that is not `[cold_path]` is a defect.** A `nolint` where `[cold_path]` fits is a defect.

**Never implement a transform inside an ASR family file - take the mel frontend's transform
table from `dasllama/dasllama_audio.das` (`build_dft_twiddles`, or `build_fft_plan` +
`fft_pow2_run`) instead.**

**Never accept a `create_session` or `transcribe` option that the model's `caps()` does not
declare - panic at the call site instead.**

**A caller that pins a family tower's lane (`set_*_q8`) around a load resets it
(`reset_*_q8`) before returning, on every path out.** A pin that outlives its load silently
changes the lane of the next tower loaded in the process.
