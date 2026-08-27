# dasLLAMA Vision Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEDIA.md`.

**This checklist governs `dasllama/dasllama_vision.das`, `dasllama/dasllama_vision_io.das`,
`dasllama/dasllama_vision_embedder.das`, `dasllama/dasllama_tower.das` (the shared
encoder-tower home), a vision family file - one `dasllama/dasllama_<family>.das` holding a
single vision projector family - and any path that runs inside the program under review, not
a spawned child process, and splices or schedules a stream carrying decoded media - pixels
or audio samples; routed files apply this list with `REVIEW.md`'s.**

**A GEMM in a vision family file that does not go through a shared batch-GEMM entry point is a
defect, a hand-written dot-product loop included** - the entry points are `mm_blob_b`,
`mm_bf16_b`, `mm_plane_b` (`dasllama/dasllama_tower.das`) and `matmul_q8q8_batch`
(`dasllama/dasllama_math.das`).

**A per-encode buffer in `dasllama/dasllama_vision_embedder.das` or a vision family file whose
size follows the input - patch count, pixel count, clip frames - is a defect without
`@exact_size`, and one reused across encodes rather than freshly allocated is a defect without
`@scratch` - a nolint is not a substitute for either.** The annotation goes on the declaration,
or on the callee parameter the buffer grows through; a buffer grown only through such a
callee - one whose parameter carries `@scratch` and that reserves before it resizes - carries
none of its own.

**A debug or profiling leg in `dasllama/dasllama_vision_embedder.das` or a vision family file
that is not `[cold_path]` is a defect** - a nolint is not a substitute.

**A vision family file takes every clamp bound from `read_clamp`, never from a literal.**
`read_clamp` returns the four scalars stored beside a weight tensor in the projector file
(`<base>.input_min` / `.input_max` / `.output_min` / `.output_max`), or an inactive +/-FLT_MAX
clamp where the file carries none.

**A vision family file whose forward applies no clamp at all, and whose header does not say so,
is a defect.**

**A family file that holds a second copy of code two tower families both need - compute,
stage/read, or load-orchestration code that names no family type - is a defect; that code
lives in `dasllama/dasllama_tower.das`, the encoder-tower home.**

**A diff that puts a family's exported runtime setter, or the module global it writes,
anywhere but that family's file is a defect.**

**Splitting a media splice's rows across more than one `forward_prefill_embd` call, or letting
a driver chunk them by row, is a defect** - the span bounds are counted from the start of the
call, so a boundary inside the span changes the mask. A driver declining the whole call to the
CPU loop, or splitting command buffers per layer, is not a split, and the one call may also
carry the surrounding head and tail tokens.

**A media splice written at the seam as anything other than two token spans plus a row block is
a defect, everywhere the splice appears** - the two-span form is what keeps BPE merges from
crossing the media.

**A diff that changes the media row-block shape also re-checks every file outside this module
that carries that shape, in the same change** - `utils/dasllama-server/openai_server.das`.

**A family's new arm for a media kind that reaches the layer stack by any path but
`forward_prefill_embd` is a defect, a second prefill body for it included** - write a sibling
`eval_embd_span*` entry that feeds the same body instead.
