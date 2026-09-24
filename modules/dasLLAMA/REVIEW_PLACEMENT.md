# dasLLAMA Placement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md` and the `ARCHITECTURE_*.md` companions its sec.1 routing block names.
Planned work: `followup_general.md`, `followup_vulkan.md` for Vulkan, `followup_metal.md` for Metal.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.**

**A per-file inventory restated in this checklist is a defect of the checklist.** The
`ARCHITECTURE_*.md` companions' sec.1 charters own the per-file list; a rule naming what KIND of
code lands in which file is the checklist's own.

**A function, a class (a kernel class among them), a module global (`let` or `var`, private or
not), a named constant or a `require` under `dasllama/` lands in the file whose charter line - in
an `ARCHITECTURE_*.md` companion's sec.1, or a role row of `ARCHITECTURE_GPU.md` sec.1.5 - names
its concern, unless that charter row carries a `must not hold` cell - the concerns that file
never holds - naming the concern; a diff may instead change that charter line in the same
change. Another file's charter naming the same kind of code does not license the landing.** `ARCHITECTURE.md`'s sec.1 routing
block names the companion that holds each file's charter line. A driver arm is host code that
ensures, binds, or encodes a dispatch; a backend capability is a function a driver registers in
a hook or capability registry.

**A charter line's parenthetical examples illustrate its concern and never narrow it: a diff
adding a family, format or arm the parenthetical does not name lands it in the file whose
concern is named, and adds it to that parenthetical in the same change.**

**A file outside `dasllama/` carries no charter line and answers to its own folder's
checklist.**

**The grid rule - the expression that computes a dispatch's workgroup counts - of a class whose
family ships more than one arm lands in `dasllama/dasllama_vulkan_classes.das`.** An arm is one
of the coopmat forms a family ships (cm2, KHR).

**A Vulkan host-side ensure/set/encode chain (an if/else over stamps) that picks a stamp from
its push-constant and shape arguments alone lands in `dasllama/dasllama_vulkan_classes.das`.** A
stamp is one class stamped from a kernel class template.

**A Vulkan predicate over shape values alone lands in `dasllama/dasllama_vulkan_classes.das`,
whichever file calls it.**

**A Vulkan host-side ensure/set/encode chain whose pick reads the driver's state - `g_rd` or an
`RLayer` field - lands in `dasllama/dasllama_vulkan_decode.das` when the decode step calls it,
and in `dasllama/dasllama_vulkan_prefill.das` when the prefill pass calls it.**

**A host-side ensure/set/encode chain on any backend that only switches on a stamp it is handed
lands in the file of the function that decides that stamp.**

**A HOST-side tensor format conversion lands in `dasllama/dasllama_convert.das`; a kernel-side
decode helper lands in its backend's kernel file (`dasllama/dasllama_metal_kernels.das`,
`dasllama/dasllama_vulkan_classes.das`).**

**A disk-order -> compute-order transform lands by its consumer: a transform into the layout
the CPU kernels read row by row in `dasllama/dasllama_repack.das`, a transform into the layout a
GPU plane or gather reads in `dasllama/dasllama_layout.das`.**

**A CPU KV-cache store, read, score dot, or V-accumulate OVER CACHE BYTES - a codec primitive
that knows the K/V element format - lands in `dasllama/dasllama_kv_codec.das`.** A dot over an
already-decoded f32 row is not a codec primitive.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its
tokenizer's own file (`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A GPU kernel body under `dasllama/` - the arithmetic loop a dispatch class (a class a
`[metal_dispatch]` or `[vk_dispatch]` declares) picks one variant of - lands in its backend's
kernel file, `dasllama/dasllama_metal_kernels.das` or `dasllama/dasllama_vulkan_classes.das`.** A
class stamped from a template declared elsewhere is not a kernel body: it compiles its own
pipeline where it is stamped.

**A CPU kernel body under `dasllama/` - the arithmetic loop a `[tune]` family picks one variant
of, or a function a `register_kernel_backend` call names - lands in a tier file,
`dasllama/dasllama_math_<tier>.das`, never in `dasllama/dasllama_math.das`; a body that a
`register_kernel_backend` call in another tier file names, or that two tier files' calls name,
lands in `dasllama/dasllama_math_default.das`.**

**A branch only one model family takes lands in that model family's file.**

**A special case only one backend driver needs lands in that driver's file.**

**A branch that is one model family's branch of a kernel two or more model families share lands
in the kernel's file, and that file's charter line names the condition that selects the
branch.**

**Logic or a named constant that two files in one folder both use lands in a file both already
require - a new file of its own when they require none in common - never as a second copy.** Two
spellings drift apart on the first edit to one. A restatement the language or the test contract
forces - an enum-and-int pair of one predicate, a test's CPU oracle of the arithmetic - is not a
copy.

**A piece that two folders both need, neither containing the other, lands in the folder that owns
the concern, and the other folder requires it - never a copy in each.**

**A family gaining support for a media kind adds that kind's span markers to that family's chat
template, never to another family's; a diff claiming that support while the family's chat template
or vocab lacks the markers is a defect.** Span markers are the template text that opens and closes
the media rows.

**No signature in `dasllama/dasllama_tower.das` takes a type that
`dasllama/dasllama_audio.das`, `dasllama/dasllama_vision.das`, or a family file declares - the
shared shape lands in `dasllama/dasllama_asr_types.das`.**

**`dasllama/dasllama_tower.das` requires none of `dasllama/dasllama_audio.das`,
`dasllama/dasllama_vision.das`, or a family file - a diff adding such a require is a defect.**

**A `dasllama/dasllama_tower.das` helper with one calling family lands in that family's file.**

**Tool wire text (the text of a model's tool/function call, built or parsed) is produced only
in `dasllama/dasllama_tools.das`.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_audio_io.das` requires `audio`
(the miniaudio decode module) - decode through that file.**

**No engine file (`dasllama/`) other than `dasllama/dasllama_vision_io.das` requires
`stbimage` - decode through that file.**

**Engine, HTTP, or response-writing logic never lands in `dasllama/dasllama_scheduler.das`** -
the forward loops and the model state stay in the other `dasllama/` files; HTTP, and the code
that turns a step's output into the wire text a client reads, in `utils/dasllama-server` (repo
root).

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_common.das` when `dasllama_common.das`'s own code needs the registration to have
run and the registered module does not require the engine back, and in
`dasllama/dasllama_transformer.das` otherwise.** The require umbrella breaks the cycle a module
requiring the engine back would close.

**A registration only a program root (test, harness, benchmark, tool) needs gets no side-effect
require in an engine file - the program root requires the registration module directly.**

**A function in `dasllama/dasllama_common.das` that performs work through a hook another module
registers panics on the unset hook, with a message naming the module to require.** A function
that returns quietly hides which registration a program root forgot.

**A function in `dasllama/dasllama_common.das` that reports whether a hook another module
registers is installed returns false when the hook is unset - never a panic.**

**A `dasllama/` module whose `[init]` registers a hook the engine dispatches through gets its
side-effect require in the same change that adds it** - a registration no engine file reaches
never fires for a consumer of the `dasllama/dasllama.das` facade.

**Platform-specific code - a device call, a `require` of a backend module, or a read of a
backend's own state other than its `g_env_<backend>` knobs - in an engine file (`dasllama/`) lands only
in that platform's backend file.**
