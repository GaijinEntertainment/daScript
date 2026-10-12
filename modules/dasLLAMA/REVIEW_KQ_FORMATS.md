# dasLLAMA Weight-Format Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`. Planned work:
`followup_general.md`.

A definition home of a per-format quantity is `dasllama/dasllama_kqformat.das`, the
`ggml_type_bytes` table (`dasllama/dasllama_gguf.das`), the `Q8_*` constants of
`dasllama/dasllama_gemm_schema.das`, a `@template_constant` on a Metal or SPIR-V kernel class, or
the pins of `tests/test_kqformat.das`.
A site is any weight plane a serving call of a backend (CPU, Metal, Vulkan) reads. A served form
of a weight format is a `KqFmt` member with its own descriptor row over the same GGUF type
(`ARCHITECTURE_ENGINE_FORMATS.md#served-form`). A converted copy is any form of a weight format
other than its disk encoding or its served form: a q8 demotion, an f16 or f32 dequant, or a GPU
site handed to the CPU kernels.

**A diff that adds or changes a function in `dasllama/` that maps a kernel/IR format id - the int
a generated kernel takes as its format parameter (`KqDesc.schema_id`) - or a format slot - the
`int(KqFmt)` the GPU drivers carry - to a `KqFmt`, a per-format value or predicate, or a choice
among per-format functions through its own table or branch is a defect - resolve an id through
`kq_fmt_of_id` (`dasllama/dasllama_kqformat.das`: the panicking overload `kq_fmt_of_id(id, what)`,
or the `bool` overload with the caller panicking when it returns false) or an id-taking accessor (`kq_qsb(int)` /
`kq_ssb(int)`, `dasllama/dasllama_gemm_schema.das`), and a slot through `kq_fmt_of_slot` or a
slot-taking accessor (`kq_sb(int)`, `kq_unit(int)`).**

**A literal that stands for a per-format quantity of a `KqFmt` member - the value a definition
home defines for it, in any unit (words, halves, superblocks) - written outside a Metal or SPIR-V
kernel body and outside a definition home is a defect: read it through a `KqFmt` accessor
(`kq_elems`, `kq_disk_bytes`, ...), through `kq_qsb` / `kq_ssb` on a format id, or through a named
constant a definition home declares, declared there in the same change when none exists.** A
hand-copied value drifts from the definition it restates, and a literal `32` cannot be told apart
from a tile width that happens to match.

**A diff that writes a per-format quantity as a bare literal in a Metal or SPIR-V kernel body is a
defect - read it through a `@template_constant` declared on the shared base of its class family, or
on the class when it has none; a diff that declares such a constant pins its value against the
format's `kq_desc` row in `tests/test_kqformat.das` in the same change.**

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its `disk_bytes` inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing the row's literal pins or the `ggml_type_bytes` walk is a defect.

**A diff that states a weight format is served on a backend - in any document, PR description,
test name, or predicate answering that the format fits - while a site a serving call of that
backend reaches reads the format through a converted copy is a defect.**

**A diff that makes a site read a weight format through a converted copy is a defect - add that
backend's kernels for the format at the site instead.**
