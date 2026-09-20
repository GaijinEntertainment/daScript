# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id - the int a generated kernel takes as
its format parameter - into a `KqFmt`, a plane stride, an interleave, or any other per-format
number resolves it through `kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`): either the panicking
overload `kq_fmt_of_id(id, what)`, or the `bool` overload whose false branch panics. A fallback
branch that returns another format's number for an unknown id is a defect.**

**A per-format byte or stride-unit count under `modules/dasLLAMA/` is read off the `kq_desc` row
through its accessors - `kq_qsb` / `kq_ssb` / `kq_elems` on a `KqFmt`
(`dasllama/dasllama_kqformat.das`), `kq_qsb` / `kq_ssb` on a format id
(`dasllama/dasllama_gemm_schema.das`), `kq_disk_bytes` (`dasllama/dasllama_gguf.das`) - never
written as a literal. The two homes a literal per-format count lives in are the `kq_desc` row
(`dasllama/dasllama_kqformat.das`) and the `ggml_type_bytes` block table
(`dasllama/dasllama_gguf.das`), and `tests/test_kqformat.das` pins every row against that
table.** A hand-copied count drifts from the table it restates.
