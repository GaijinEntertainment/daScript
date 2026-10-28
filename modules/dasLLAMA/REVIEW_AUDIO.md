# dasLLAMA audio and ASR Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_MEDIA.md`, `ARCHITECTURE_MEDIA_ASR.md`. Planned work: `followup_general.md`,
`followup_vulkan.md`.

An ASR family file is a `dasllama/` file holding one speech-recognition family's CPU model. An
ASR model file is an ASR family file or `dasllama/dasllama_rnnt.das`, the transducer decode those
families share. An audio-rail file is `dasllama/dasllama_tower.das`, `dasllama/dasllama_audio.das`,
`dasllama/dasllama_audio_io.das`, `dasllama/dasllama_audio_embedder.das`,
`dasllama/dasllama_asr_types.das`, `dasllama/dasllama_vad.das`, or an ASR model file. A
licensed namer is a file that drives a family past the facade's verbs: the GPU family hooks
(`dasllama/dasllama_metal_tower.das`, `dasllama/dasllama_metal_asr_dec.das`,
`dasllama/dasllama_vulkan_tower.das`, `dasllama/dasllama_vulkan_asr_dec.das`),
`benchmarks/asr/asr_bench.das`, `harness/pk_parity_probe.das`, `harness/cpu_asr_race.das` and
`utils/dasllama-convert/main.das` (repo root).

**In `dasllama/dasllama_asr.das` and `dasllama/dasllama_audio_embedder.das` - the union
carriers that route each facade function to one family - a family arm that does anything but
one forwarding call is a defect; the work goes in the family file.** The loader's format or
family check that picks the arm is the dispatch, not an arm.

**A diff that adds a family to a union carrier changes the carrier, for that family, only at its
`require` line, union field, finalize line, kind value, loader dispatch check, and one-line arms.**

**Code that names a public enum, class, variant or bitfield an ASR family file declares, or a
public function of that file whose name does not contain the family's name (the file name after
`dasllama_`), is a defect outside that family file, the union carriers `dasllama/dasllama_asr.das`
and `dasllama/dasllama_audio_embedder.das`, `tests/`, and the licensed namers - route the call through
`dasllama/dasllama_asr.das`'s public functions instead.**

**A prompt, a decode loop, a caps value, or a language rule (code that picks or validates a
session language) in a union carrier is a defect - it goes in the family file.**

**An ASR family file's new arm for a media kind that reaches the layer stack by any path but
`forward_prefill_embd` is a defect, a second prefill body for it included** - write a sibling
`eval_embd_span*` entry that feeds the same body instead.

**A GEMM against model weights in an ASR model file, other than the transducer decode step's
per-token gate and joint products in `dasllama/dasllama_rnnt.das`, that does not go through a
`*_mm*` wrapper or `mm_blob_b` is a defect, hand-written dot-product loops included.** Those
products run once per token over half-precision copies of the weights, inside one multi-thread
dispatch, which no `*_mm*` wrapper serves (`ARCHITECTURE_MEDIA_ASR.md#rnnt-decode-step`).

**A matrix product of two activations - tensors the encoder or decoder computes, never a weight or
a constant table built from the audio config alone (the window, the DFT twiddles, the mel
filterbank) - in an ASR model file that does not go through `gemm_f32_jo` is a defect,
hand-written loops included.**

**A buffer reused across calls in an audio-rail file that is not `@scratch` - on its
declaration, or on the parameter of every callee that grows it - is a defect.**

**A debug or profiling code path in an audio-rail file whose work is not inside a `[cold_path]`
function is a defect.** Such a path is a function or branch that runs only while a debug or
profiling knob (an environment variable, a setter or a flag) is on, and whose work the result
does not need.

**Never implement a transform inside an ASR model file - take the mel frontend's transform
table from `dasllama/dasllama_audio.das` (`build_dft_twiddles`, or `build_fft_plan` +
`fft_pow2_run`) instead.**

**Never accept a `create_session` or `transcribe` option that the model's `caps()` does not
declare - panic at the call site instead.** The session language maps to `languages`, `translate`
to `translate`, and `context` to `prompt`; `no_timestamps` only drops output and maps to no field.
