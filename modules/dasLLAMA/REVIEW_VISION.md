# dasLLAMA Vision Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEDIA.md`. Planned work: `followup_general.md`, `followup_vulkan.md`.

A vision family file is one `dasllama/dasllama_<family>.das` holding a single vision projector
family.

**A GEMM in a vision family file that does not go through a shared batch-GEMM entry point is a
defect, a hand-written dot-product loop included** - the entry points are `mm_blob_b`,
`mm_bf16_b`, `mm_plane_b` (`dasllama/dasllama_tower.das`) and `matmul_q8q8_batch`
(`dasllama/dasllama_math.das`).

**A per-encode buffer in `dasllama/dasllama_vision_embedder.das` or a vision family file whose
size follows the input - patch count, pixel count, clip frames - is a defect without
`@exact_size` on its declaration.**

**A buffer in `dasllama/dasllama_vision_embedder.das` or a vision family file that outlives one
encode and is reused by the next (a state field, a module global) is a defect unless `@scratch`
sits on its declaration or on the parameter of every callee that grows it.**

**A debug or profiling leg in `dasllama/dasllama_vision_embedder.das` or a vision family file
that is not `[cold_path]` is a defect.**

**A vision family file takes every clamp bound from `read_clamp`, never from a literal.**
`read_clamp` returns the four scalars stored beside a weight tensor in the projector file
(`<base>.input_min` / `.input_max` / `.output_min` / `.output_max`), or an inactive +/-FLT_MAX
clamp where the file carries none.

**A vision family file whose forward applies no clamp at all, and whose opening `//!` doc block
does not say so, is a defect.**

**A diff that puts a vision family's `set_*` runtime knob, or the module global it writes,
anywhere but that family's file is a defect.**

**A family gaining support for a media kind adds that kind's span markers to that family's chat
template, never to another family's; a diff claiming that support while the family's chat template
or vocab lacks the markers is a defect.** Span markers are the template text that opens and closes
the media rows.

**Splitting a media splice's rows across more than one `forward_prefill_embd` call, or letting
a driver chunk them by row, is a defect** - the span bounds are counted from the start of the
call, so a boundary inside the span changes the mask. A driver declining the whole call to the
CPU loop, or splitting command buffers per layer, is not a split, and the one call may also
carry the surrounding head and tail tokens.

**A diff that adds a session kind or an admission mode to an in-process scheduler either serves
a stream carrying decoded media on it or refuses that stream at admission - a mode that admits
such a stream onto a path that panics later, or drops its rows, is a defect.**

**A media splice written, where the media rows join the prompt's tokens, as anything other than
two token spans plus a row block is a defect, everywhere the splice appears** - the two-span form
is what keeps BPE merges from crossing the media.

**A diff that changes the media row-block shape also updates, in the same change,
`utils/dasllama-server/openai_server.das` (repo root), and a diff that adds another file building
or parsing the row block names it here.**

**A family's new arm for a media kind that reaches the layer stack by any path but
`forward_prefill_embd` is a defect, a second prefill body for it included** - write a sibling
`eval_embd_span*` entry that feeds the same body instead.
