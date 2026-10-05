# dasLLAMA audio and ASR Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEDIA.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

An ASR family file is a `dasllama/` file holding one speech-recognition family's CPU model. An
audio-rail file is `dasllama/dasllama_tower.das`, `dasllama/dasllama_audio.das`,
`dasllama/dasllama_audio_io.das`, `dasllama/dasllama_audio_embedder.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_vad.das`, or an ASR family file.

**In `dasllama/dasllama_asr.das` and `dasllama/dasllama_audio_embedder.das` - the union
carriers that route each facade function to one family - a family arm that does anything but
one forwarding call is a defect; the work goes in the family file.** The loader's format or
family check that picks the arm is the dispatch, not an arm.

**A diff that adds a family to a union carrier changes the carrier only at the family's union
field, finalize line, kind value, and one-line arms.**

**A prompt, a decode loop, a caps value, or a language rule in a union carrier is a defect - it
goes in the family file.**

**A GEMM against model weights in an ASR family file that does not go through a `*_mm*` wrapper
or `mm_blob_b` is a defect, hand-written dot-product loops included.**

**A matrix product of two activations - tensors the encoder or decoder computes, never a weight
or the mel frontend's transform table (`build_dft_twiddles`) - in an ASR family file that does
not go through `gemm_f32_jo` is a defect, hand-written loops included.**

**A buffer reused across encodes in an audio-rail file that is not `@scratch` - on its
declaration, or on the parameter of every callee that grows it - is a defect.**

**A debug or profiling code path in an audio-rail file that is not `[cold_path]` is a defect.**

**Never implement a transform inside an ASR family file - take the mel frontend's transform
table from `dasllama/dasllama_audio.das` (`build_dft_twiddles`, or `build_fft_plan` +
`fft_pow2_run`) instead.**

**Never accept a `create_session` or `transcribe` option that the model's `caps()` does not
declare - panic at the call site instead.**
