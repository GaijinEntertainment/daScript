# dasLLAMA vision and media rules

**Routed from `REVIEW.md`: a diff touching a media splice or its eval shape
(`eval_embd_span_` / `forward_prefill_embd`'s span bounds / `encode_image_`),
`dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`, a media-carrying
scheduler path, `dasllama/dasllama_vision_embedder.das`, `dasllama/dasllama_tower.das` (with
`REVIEW_AUDIO.md` — the shared encoder-tower home serves both), or a vision family file — one
`dasllama/dasllama_<family>.das` holding a single vision projector family — applies this list
with the master's.** `REVIEW_COMMON.md` (repo root) binds this file too. Architecture doc:
`ARCHITECTURE.md`.

**A GEMM in a vision family file goes through a shared batch-GEMM entry point — `mm_blob_b`,
`mm_bf16_b`, `mm_plane_b` (`dasllama/dasllama_tower.das`) or `matmul_q8q8_batch`
(`dasllama/dasllama_math.das`).** A hand-written dot-product loop beside them is a defect.

**A per-encode reused buffer in `dasllama/dasllama_vision_embedder.das` or a vision family
file is `@scratch` — on its declaration, or on the callee parameter it grows through.** A
nolint where the annotation fits is a defect.

**A debug or profiling leg in `dasllama/dasllama_vision_embedder.das` or a vision family file
is `[cold_path]`.** A nolint where it fits is a defect.

**A vision family file takes every clamp bound from `read_clamp`, never from a literal.**
`read_clamp` returns the four scalars stored beside a weight tensor in the projector file
(`<base>.input_min` / `.input_max` / `.output_min` / `.output_max`), or an inactive ±FLT_MAX
clamp where the file carries none.

**A vision family whose forward applies no clamp at all says so in its file header.**

**A tower piece two tower families both need lives in `dasllama/dasllama_tower.das`** (the
encoder-tower home — its inventory is `ARCHITECTURE.md` §1.7); a family file that re-implements
one is a defect.

**A media splice's rows reach `forward_prefill_embd` in ONE call** — splitting them across
calls, or letting a driver chunk them by row, is a defect: the span bounds are call-relative,
so a boundary inside the span changes the mask. A driver DECLINING the whole call to the CPU
loop, or splitting command buffers per layer, is not a split, and whether the one call also
carries the surrounding head and tail tokens is free.

**A media splice is expressed as two token spans plus a row block, everywhere it appears** — so
BPE merges never cross the media. The engine, the scheduler, and the server all carry the same
shape; any other representation at the seam is a defect.

**A family gaining an arm for a media kind rides the existing splice prefill path** — a
parallel prefill path for it is a defect.
