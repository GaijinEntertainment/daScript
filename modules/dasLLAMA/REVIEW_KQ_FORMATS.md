# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id into plane strides resolves the id
through `kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`, either overload) and panics on an id
no format carries - never a default arm that walks another format's stride.**

**A per-format byte count under `modules/dasLLAMA/` is read off the `kq_desc` row through its
accessors (`kq_qsb`, `kq_ssb`, `kq_elems`, `kq_disk_bytes` - `dasllama/dasllama_kqformat.das`),
never written out by hand; the numbers are written in two places only - the row's own
definition and the test that pins the row against `ggml_type_bytes`.** A hand-copied count
drifts from the table it restates.
