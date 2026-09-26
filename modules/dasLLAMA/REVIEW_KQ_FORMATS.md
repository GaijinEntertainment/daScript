# dasLLAMA Weight-Format Descriptor Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

A definition home of the per-format quantities is `dasllama/dasllama_kqformat.das` (the
`kq_desc` rows and the named stride and stream-tag constants they read), the `ggml_type_bytes` table
(`dasllama/dasllama_gguf.das`), `Q8_BLOCK_ELEMS` / `Q8_QPB` / `Q8_SPB`
(`dasllama/dasllama_gemm_schema.das`), or the pins of `tests/test_kqformat.das`.

**A diff that adds or changes a function in `dasllama/` that turns a kernel/IR format id - the
int a generated kernel takes as its format parameter - into a `KqFmt` or any per-format number
with a mapping of its own is a defect - resolve the id through `kq_fmt_of_id`
(`dasllama/dasllama_kqformat.das`): the panicking overload `kq_fmt_of_id(id, what)`, the `bool`
overload whose false branch panics, or a per-format accessor that takes the id (`kq_qsb` /
`kq_ssb`).**

**A literal that stands for a per-format quantity of a `KqFmt` member - the value a `kq_desc`
column, that member's `ggml_type_bytes` entry or a named per-format constant defines for it -
written anywhere under `modules/dasLLAMA/` outside a definition home is a defect: read it through a
`kq_desc` accessor (`kq_elems`, `kq_disk_bytes`, ...), through `kq_qsb` / `kq_ssb` on a format id,
or through a named constant a definition home declares - in a kernel body, which calls no
accessor, a `uint` one `dasllama/dasllama_kqformat.das` declares.** A hand-copied value drifts
from the definition it restates.

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its `disk_bytes` inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing the row's literal pins or the `ggml_type_bytes` walk is a defect.
