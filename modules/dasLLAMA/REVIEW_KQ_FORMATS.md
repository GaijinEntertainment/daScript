# dasLLAMA Weight-Format Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_CPU_KERNELS.md`,
`ARCHITECTURE_GPU_ASYMMETRIES.md`, `ARCHITECTURE_MEASUREMENT.md`. Planned work:
`followup_general.md`, `followup_metal.md`, `followup_vulkan.md`.

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

**Narrowing `check_kq_form_matrix` (`REVIEW.das`) - the gate that holds every `KqFmt` member of a
Metal GEMM stamp family (`stamp = "<family>:<fmt>:<form>"`) to the same forms (the `<form>` field of
its stamps), to its own `kq_mulmm_<fmt>` or `moe_mulmm_<fmt>` race (the per-machine timing contest
that picks between the format's tensor twin and its base kernel) and, for a format in the `kq_mm`
family, to a dev-W dequant pass (the pass that dequantizes its weights into an f16 panel on the
device: `enc_kq_dq_<fmt>` or `enc_dequant_<fmt>h`) - is a defect.** A form one format has and
another lacks dispatches the two through different kernel sequences on one machine, so a timing
compared across the two measures the sequences, not the formats' kernels.

**A diff that changes when a Metal MoE format takes its tensor-op twin over its base kernel of
the same form - in `metal_moe_kq_twin_pick` or a per-format condition beside it, by the window's
mean rows per expert, the K width, or this machine's race winner - makes the identical edit for
every `KqFmt` member of that format's `moe_mm` / `moe_mm_split` stamp family, at every K width
the member's twin can run, in that diff.** When two formats'
picks differ, a timing compared across them measures one format's base kernel against the other's
tensor twin, not the two formats' kernels.

**A literal that stands for a per-format quantity of a `KqFmt` member, in any unit (words,
halves, superblocks) - a number derived from several formats' sizes included - written anywhere
but a GPU kernel body, a kernel builder's `requires =` alignment string, a test's expected-message
text, or a definition home - `dasllama/dasllama_kqformat.das`, the `ggml_type_bytes` table
(`dasllama/dasllama_gguf.das`), the `Q8_*` constants of `dasllama/dasllama_gemm_schema.das`, a
`@template_constant` on a GPU kernel class, the pins of `tests/test_kqformat.das` - is a defect:
read it through a `KqFmt` accessor (`kq_elems`, `kq_disk_bytes`, ...), through `kq_qsb` /
`kq_ssb` on a format id, or through a named constant a definition home declares, declared there
in the same change when none exists.** A literal `32` reads like a tile width.

**A diff that writes a literal integer into a kernel builder's `requires =` alignment string pins
it against the format's block length in `tests/test_kqformat.das` in the same change** - the lens
accepts no name there.

**A diff that writes a per-format quantity of a `KqFmt` member as a bare literal in a GPU kernel
body is a defect - read it through a `@template_constant` declared on the shared base of its
class family, or on the class when it has none.**

**A diff that declares, on a GPU kernel class, a `@template_constant` holding a per-format
quantity of a `KqFmt` member pins its value against the format's `kq_desc` row in
`tests/test_kqformat.das` in the same change.**

**A diff that adds or changes a `kq_desc` row (`dasllama/dasllama_kqformat.das`) or a
`ggml_type_bytes` entry (`dasllama/dasllama_gguf.das`) lands the row's pins in
`tests/test_kqformat.das` in the same change - its literal strides, id, stream code and enum
value, and its `disk_bytes` inside the walk that checks every row against `ggml_type_bytes`.**
Narrowing the row's literal pins or the `ggml_type_bytes` walk is a defect.

A site is a weight plane a backend's serving call reads, on the CPU or on any GPU backend. A
lever is an env knob or setter that changes which form a site reads. A converted copy is any form
of the weights - other than a re-encoding of their own quantized values that is value-exact
against the disk form and has no wider element type
(`ARCHITECTURE_ENGINE_FORMATS.md#served-form`) - whose bytes outlive the one kernel call or window
that made them, or reach the `.dlim` image.

**A diff that makes a site read a weight format through a converted copy, or hands a GPU site to
the CPU's kernels, on a run with every lever at its default is a defect - add that backend's
kernels for the format at the site instead.**

**A diff that adds a site, adds a backend's serving call to a site, or changes which `KqFmt`
members a site serves, serves at it, on each backend whose serving call reaches the site, in the
same change, every `KqFmt` member whose block length divides the site's row length (256 for a
superblock member, 32 for a per-32 member), or makes the load decline a member it does not serve
there with a reason naming the site and the member.** A site that serves only the formats its
author's files carried converts the rest without saying so, and the first file in another format
fails or converts at load with no message naming the site.

**A diff that makes the load accept a `.scale` tensor - a per-plane dequant factor stored beside a
weight tensor - at a site makes each backend whose serving call reaches the site apply that factor
in the same change, or makes the load decline the plane on that backend with a reason naming the
site and the member.** A backend that reads the plane and skips the factor produces wrong output,
and no message names the site.

**A diff that adds or changes a site that names, with a literal `KqFmt` member, the format of a
weight plane whose disk type some `KqFmt` member describes is a defect: tag the plane from its
disk type with `kq_tag` / `kq_fmt_of` (`dasllama/dasllama_load.das`) and read that tag at the
site instead.** A literal at the site is a conversion no report names: every disk type the file
carries becomes the literal's form, and each backend that copies the literal serves that form in
place of the file's.

**Weakening `REVIEW.das`'s `check_load_big_format` is a defect.** That check fails every `load_big`
call in `dasllama/dasllama_load.das` that passes no format: such a call converts the plane to one
form, whatever its disk type, and no report names the conversion.

**A diff that states a backend serves a weight format - in any document, PR description, test
name, or a predicate that returns true for "this backend runs this format" - while, on a run with
every lever at its default, a site that backend's serving call reaches reads the format through a
converted copy or hands a GPU site to the CPU's kernels is a defect - state that site as a gap
instead, or cite the follow-up row that ledgers it.**

**A diff that states, in any document, PR description, test name or predicate, that a GPU backend
serves a `KqFmt` member at a site lands or cites a subtest that loads a file whose plane at that
site is in that format on disk, and asserts that the loader's format tag for that plane equals its
disk type and that a kernel counter (`metal_kernel_coverage_of(name)` on Metal,
`vk_kernel_coverage_of(name)` on Vulkan, `cud_kernel_coverage_of(name)` on CUDA) that no other plane
of that file moves rose during the run.** The file is a downloaded `performance/model_specs.das`
row's file, or, when no downloaded row carries the format, a row whose `file` ends `-local.gguf` and
whose `recipe` requantizes into it. A subtest that only asserts the backend ran still passes when
the site reads a converted copy or runs on the CPU pre-step.

**A diff that adds a lever, off by default, that makes a site read a weight format through a
converted copy names the lever in the same change - in `ARCHITECTURE_GPU_ASYMMETRIES.md` when one
GPU backend carries it and another lacks it, else in
`ARCHITECTURE_MEASUREMENT.md#sanctioned-instrumentation-rails`.**
