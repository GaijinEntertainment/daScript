# dasLLAMA Weight-Format Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`,
`ARCHITECTURE_GPU_ASYMMETRIES.md`. Planned work: `followup_general.md`, `followup_metal.md`,
`followup_vulkan.md`.

**A diff that adds or changes a function in `dasllama/` that maps a format id - the int a
generated kernel takes as its format parameter (`KqDesc.schema_id`) - or a format slot - the
`int(KqFmt)` the GPU drivers carry - to a `KqFmt`, a per-format value or a per-format predicate
through its own table or branch is a defect - resolve an id through `kq_fmt_of_id`
(`dasllama/dasllama_kqformat.das`: the panicking overload `kq_fmt_of_id(id, what)`, or the `bool`
overload with the caller panicking when it returns false) or an id-taking accessor (`kq_qsb(int)` /
`kq_ssb(int)`, `dasllama/dasllama_gemm_schema.das`), and a slot through `kq_fmt_of_slot` or a
slot-taking accessor (`kq_sb(int)`, `kq_unit(int)`).**

**A diff that adds or changes a function in `dasllama/` that picks among per-format functions or
kernel stamps through its own table or branch over a `KqFmt`, a format id or a format slot is a
defect - generate the arms with `kq_fmt_stamp` or `kq_tile_stamp` (`dasllama/dasllama_kqformat.das`)
instead.**

**A literal that stands for a per-format quantity of a `KqFmt` member, in any unit (words, halves,
superblocks), written outside a Metal or SPIR-V kernel body and outside a definition home -
`dasllama/dasllama_kqformat.das`, the `ggml_type_bytes` table (`dasllama/dasllama_gguf.das`), the
`Q8_*` constants of `dasllama/dasllama_gemm_schema.das`, a `@template_constant` on a Metal or SPIR-V
kernel class, the pins of `tests/test_kqformat.das` - is a defect: read it through a `KqFmt` accessor
(`kq_elems`, `kq_disk_bytes`, ...), through `kq_qsb` / `kq_ssb` on a format id, or through a named
constant a definition home declares, declared there in the same change when none exists.** A
literal `32` reads like a tile width.

**A diff that writes a per-format quantity as a bare literal in a Metal or SPIR-V kernel body is a
defect - read it through a `@template_constant` declared on the shared base of its class family, or
on the class when it has none; a diff that declares such a constant pins its value against the
format's `kq_desc` row in `tests/test_kqformat.das` in the same change.**

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its `disk_bytes` inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing the row's literal pins or the `ggml_type_bytes` walk is a defect.

**A diff that makes a site - a weight plane a backend's (CPU, Metal, Vulkan) serving call reads -
read a weight format through a converted copy on a run that sets no lever is a defect - add that
backend's kernels for the format at the site instead.** A converted copy is any form other than the
format's disk encoding or its served form (a `KqFmt` member with its own descriptor row over the
same GGUF type, `ARCHITECTURE_ENGINE_FORMATS.md#served-form`): a q8 demotion, an f16 or f32
dequant.

A scratch panel - an f16 tile a window's GEMM dequantizes from the site's own bytes - is not a
converted copy while the bytes it holds are the current window's alone and no image carries them;
its buffer may serve the next window.

**A diff that states a backend serves a weight format - in any document, PR description, test
name, or predicate answering that the format fits - while a site that backend's serving call
reaches reads the format through a converted copy or on another backend's kernels is a defect -
state that site as a gap instead, or cite the follow-up row that ledgers it.**

**A diff that adds a lever, off by default, that makes a site read a weight format through a
converted copy for measurement names that arm in `ARCHITECTURE_GPU_ASYMMETRIES.md` in the same
change.** Such a lever is a measurement arm, not a served claim.
