# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id - the int a generated kernel takes as
its format parameter - into a `KqFmt` or any per-format number resolves it through
`kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`): either the panicking overload
`kq_fmt_of_id(id, what)`, or the `bool` overload whose false branch panics.**

**Outside the `kq_desc` row (`dasllama/dasllama_kqformat.das`) and the `ggml_type_bytes` block
table (`dasllama/dasllama_gguf.das`), a per-format number - any number that differs between
`KqFmt` members - under `modules/dasLLAMA/` is read through an accessor of the `kq_desc` row
(`dasllama/dasllama_kqformat.das`), through `kq_qsb` / `kq_ssb` on a format id
(`dasllama/dasllama_gemm_schema.das`), or through `kq_disk_bytes` (`dasllama/dasllama_gguf.das`) -
never written as a literal.** A hand-copied count drifts from the row it restates.

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) keeps `tests/test_kqformat.das` pinning
every row against that table, in the same change** - narrowing that pin is a defect.
