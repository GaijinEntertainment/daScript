# dasLLAMA audio and ASR rules

**Routed from `REVIEW.md`: a diff touching `dasllama_asr.das`, `dasllama_audio.das`, or a
family file (`dasllama_whisper.das`, `dasllama_qwen3a.das`, `dasllama_parakeet.das`,
`dasllama_canary.das`, `dasllama_gemma4a.das`) applies this list with the master's; a GPU ASR
file applies `REVIEW_GPU.md` too.** `REVIEW_COMMON.md` (repo root) binds this
file too.

**A verb arm in `dasllama_asr.das` is one forwarding call.** A new family touches the facade
only at the union field, the finalize line, the `AsrKind` value, and the one-line arms; a
prompt, a decode loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written
dot-product loop beside them is a defect.

**Every mel frontend and every encoder entry point in `dasllama_audio.das` and the family
files lints at zero.** Reused buffers take `@scratch`, debug and profiling legs
`[cold_path]`; a nolint where either fits is a defect (region coverage is the master's rule).

**A mel frontend builds on the FFT-plan machinery in `dasllama_audio.das`.** A hand-rolled DFT
in a family file is a defect.

**A `create_session` / `transcribe` option that the model's `caps()` does not declare panics at
the call site.** Accepting it and silently ignoring it is a defect.
