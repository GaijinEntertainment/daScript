# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A function in `dasllama/` that turns a kernel/IR format id - the int a generated kernel takes as
its format parameter - into a `KqFmt` or any per-format number resolves it through
`kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`) - either the panicking overload
`kq_fmt_of_id(id, what)`, the `bool` overload whose false branch panics, or a per-format accessor
that takes the id (`kq_qsb` / `kq_ssb`) - never with a mapping of its own.**

**A number that differs between `KqFmt` members, written as a literal anywhere under
`modules/dasLLAMA/` outside `dasllama/dasllama_kqformat.das` (the `kq_desc` rows and the named
stride and stream-tag constants they read), the `ggml_type_bytes` table
(`dasllama/dasllama_gguf.das`), `Q8_BLOCK_ELEMS` / `Q8_QPB` / `Q8_SPB`
(`dasllama/dasllama_gemm_schema.das`) and the pins of `tests/test_kqformat.das`, is a defect: read
it through a `kq_desc` accessor (`kq_elems`, `kq_disk_bytes`, ...), through `kq_qsb` / `kq_ssb` on
a format id, or through a named constant one of those files declares.** A hand-copied count drifts
from the row it restates.

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its `disk_bytes` inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing either pin is a defect.
