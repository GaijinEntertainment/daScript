# dasLLAMA audio and ASR rules

**Routed from `REVIEW.md`: a diff touching `dasllama/dasllama_asr.das`, `dasllama/dasllama_asr_types.das`,
`dasllama/dasllama_audio.das`, `dasllama/dasllama_audio_io.das`, `dasllama/dasllama_vad.das`, or an ASR family file —
one `dasllama/dasllama_<family>.das` holding a single speech model family — applies this list with
the master's.** `REVIEW_COMMON.md` (repo root) binds this file too. Architecture doc:
`ARCHITECTURE.md`.

**A verb arm in `dasllama/dasllama_asr.das` is one forwarding call.** A new family touches the facade
only at the union field, the finalize line, the `AsrKind` value, and the one-line arms; a
prompt, a decode loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written
dot-product loop beside them is a defect.

**Every function that runs per encode — in `dasllama/dasllama_audio.das` or in a family file —
takes `@scratch` on its reused buffers and `[cold_path]` on its debug and profiling legs**; a
nolint where either fits is a defect.

**A mel frontend builds on the FFT-plan machinery in `dasllama/dasllama_audio.das`.** A hand-rolled DFT
in a family file is a defect.

**A `create_session` / `transcribe` option that the model's `caps()` does not declare panics at
the call site.** Accepting it and silently ignoring it is a defect.
