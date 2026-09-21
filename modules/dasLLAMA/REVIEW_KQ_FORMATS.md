# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id - the int a generated kernel takes as
its format parameter - into a `KqFmt`, a plane stride, an interleave, or any other per-format
number resolves it through `kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`): either the panicking
overload `kq_fmt_of_id(id, what)`, or the `bool` overload whose false branch panics - or reads the
answer through a `kq_desc` accessor that takes the id (`kq_sb`, `kq_block32`, the id forms of
`kq_qsb` / `kq_ssb`), which resolve it once for every caller. A function that resolves the id
itself and answers with another format's number for an unknown id is a defect.**

**A per-format number - any number that differs between `KqFmt` members - under
`modules/dasLLAMA/` outside the `kq_desc` row and the named stride and stream-tag constants it
reads (`dasllama/dasllama_kqformat.das`), the `ggml_type_bytes` block table
(`dasllama/dasllama_gguf.das`) and the pins of `tests/test_kqformat.das`, is read off the
`kq_desc` row through its accessors - `kq_qsb` /
`kq_ssb` / `kq_elems` on a `KqFmt` (`dasllama/dasllama_kqformat.das`), `kq_qsb` / `kq_ssb` on a
format id (`dasllama/dasllama_gemm_schema.das`), `kq_disk_bytes` (`dasllama/dasllama_gguf.das`) -
never written as a literal.** A hand-copied count drifts from the table it restates.

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its disk bytes inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing a pin is a defect.
