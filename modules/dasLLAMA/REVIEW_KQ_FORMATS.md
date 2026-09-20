# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md` (the format file charters) and `ARCHITECTURE_CPU_KERNELS.md`
(the plane layouts). Planned work: `followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id into plane strides resolves the id
through `kq_fmt_of_id(id, "<site>")` (`dasllama/dasllama_kqformat.das`) and panics on an id no
format carries - never a default arm that walks another format's stride.**

**A per-format byte count under `modules/dasLLAMA/` is read off the descriptor row - `kq_qsb`,
`kq_ssb`, `kq_elems` (`dasllama/dasllama_kqformat.das`) - never written out by hand.** A
hand-copied count drifts from the table it restates.
