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

**A diff that changes when a format's Metal MoE twin pick - `metal_moe_kq_twin_pick`, or a
per-format condition beside it, the per-dispatch choice between the tensor-op kernel of the same
form and the base kernel - takes the twin (the window's mean rows per expert, the K width, or
whether the race chose the twin as the winner on this machine) makes the same change for every
`KqFmt` member of the format's `moe_mm` / `moe_mm_split` stamp family, in the same change; a member
whose twin cannot run a given K width is outside the trigger at that width.** When two formats'
picks differ, a timing compared across them measures one format's base kernel against the other's
tensor twin, not the two formats' kernels.

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

A site is a weight plane a backend's (CPU, Metal, Vulkan) serving call reads. A lever is an env
knob or setter that changes which form a site reads. A format's served form is a re-layout of its
own quantized values, value-exact against the disk form, with no wider element type (a `KqFmt`
member over the same GGUF type, `ARCHITECTURE_ENGINE_FORMATS.md#served-form`). A converted copy is
any other form of the weights whose bytes outlive the one kernel call or window that made them,
or reach the `.dlim` image.

**A diff that makes a site read a weight format through a converted copy, or hands a GPU site to
the CPU's kernels, on a run with every lever at its default is a defect - add that backend's
kernels for the format at the site instead.**

**A diff that adds a site, or adds a backend's serving call to a site, serves at it every `KqFmt`
member that can occur at the site's row length (a superblock member needs the length on the 256
lattice, a per-32 member on the 32 lattice) on that backend in the same change, or makes the load
decline a format it does not serve there with a reason naming the site and the format.** A site
that serves the formats its author's files carried and transcodes the rest serves a format matrix
nobody chose, and the next file finds the gap at load time with no line saying so.

**A diff that adds or changes a site reading a weight plane whose format is a literal `KqFmt`
member, or a plane the loader converts to one form whatever its disk type (a `load_big` call with
no format), is a defect - the loader tags the plane from its disk type through `kq_tag` /
`kq_fmt_of`, and the site reads the tag.** A literal at the site is a conversion no report names:
every disk type the file carries becomes the literal's form, and every home that copies the
literal agrees on the same wrong answer.

**A diff that states a backend serves a weight format - in any document, PR description, test
name, or a predicate that returns true for "this backend runs this format" - while, on a run with
every lever at its default, a site that backend's serving call reaches reads the format through a
converted copy or hands a GPU site to the CPU's kernels is a defect - state that site as a gap
instead, or cite the follow-up row that ledgers it.**

**A diff that states a backend serves a weight format at a site lands, or cites, a cell that loads
a file whose plane at that site carries that format - a requantized local file
(`performance/model_specs.das`, the `-local` recipe rows) where no catalogued file does - and
asserts both the plane's load tag against the disk type and that the backend's kernel counter for
the site rose.** A cell that asserts engagement alone passes with the site on a converted copy or
on the CPU pre-step.

**A diff that adds a lever, off by default, that makes a site read a weight format through a
converted copy names the lever in the same change - in `ARCHITECTURE_GPU_ASYMMETRIES.md` where
only one GPU backend carries it, else in
`ARCHITECTURE_MEASUREMENT.md#sanctioned-instrumentation-rails`.**
