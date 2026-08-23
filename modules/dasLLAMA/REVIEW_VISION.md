# dasLLAMA Vision Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`.

**Routed from `REVIEW.md`: a diff touching `dasllama/dasllama_vision.das`,
`dasllama/dasllama_vision_io.das`, `dasllama/dasllama_vision_embedder.das`,
`dasllama/dasllama_tower.das` (the shared encoder-tower home), a vision family file - one
`dasllama/dasllama_<family>.das` holding a single vision projector family - or an in-process
path that splices decoded media into a prompt or schedules such a stream, applies this list
with the master's.**

**A GEMM in a vision family file goes through a shared batch-GEMM entry point - `mm_blob_b`,
`mm_bf16_b`, `mm_plane_b` (`dasllama/dasllama_tower.das`) or `matmul_q8q8_batch`
(`dasllama/dasllama_math.das`).** A hand-written dot-product loop beside them is a defect.

**A per-encode buffer in `dasllama/dasllama_vision_embedder.das` or a vision family file
carries `@exact_size` when its size follows the input - patch count, pixel count, clip
frames - and `@scratch` when it is reused across encodes rather than freshly allocated; a
buffer that is both carries both.** The annotation goes on the declaration, or on the callee
parameter it grows through. A nolint where an annotation fits is a defect.

**A debug or profiling leg in `dasllama/dasllama_vision_embedder.das` or a vision family file
is `[cold_path]`.** A nolint where it fits is a defect.

**A vision family file takes every clamp bound from `read_clamp`, never from a literal.**
`read_clamp` returns the four scalars stored beside a weight tensor in the projector file
(`<base>.input_min` / `.input_max` / `.output_min` / `.output_max`), or an inactive +/-FLT_MAX
clamp where the file carries none.

**A vision family whose forward applies no clamp at all says so in its file header.**

**Code two tower families both need - compute, stage/read, or load-orchestration code that
names no family type - lives in `dasllama/dasllama_tower.das` (the encoder-tower home); a
second copy in a family file is a defect.** Per-family serving state - a family's exported
runtime setter and the module global it writes - stays in the family file.

**A media splice's rows reach `forward_prefill_embd` in ONE call** - splitting them across
calls, or letting a driver chunk them by row, is a defect: the span bounds are call-relative,
so a boundary inside the span changes the mask. A driver DECLINING the whole call to the CPU
loop, or splitting command buffers per layer, is not a split, and whether the one call also
carries the surrounding head and tail tokens is free.

**A media splice is expressed as two token spans plus a row block, everywhere it appears** - so
BPE merges never cross the media. The engine, the scheduler, and the server all carry the same
shape; any other representation at the seam is a defect.

**A family gaining an arm for a media kind reaches the layer stack only through
`forward_prefill_embd`** - a second prefill BODY for it is a defect; a sibling
`eval_embd_span*` entry that feeds the same body is the sanctioned shape.
