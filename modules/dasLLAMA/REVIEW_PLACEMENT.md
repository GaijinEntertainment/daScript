# dasLLAMA Placement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_ENGINE_FORMATS.md`,
`ARCHITECTURE_GPU.md`, `ARCHITECTURE_MEDIA.md`, `ARCHITECTURE_TTS.md`, `ARCHITECTURE_POCKET.md`.
Planned work: `followup_general.md`, `followup_vulkan.md` for Vulkan, `followup_metal.md` for Metal.

A charter line is the one line saying what a file under `dasllama/` holds: in
`ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_MEDIA.md`,
`ARCHITECTURE_TTS.md` or `ARCHITECTURE_POCKET.md` (`ARCHITECTURE.md#file-charters` names which),
or a role row or a file bullet of `ARCHITECTURE_GPU.md#gpu-backends`.

**A per-file inventory restated in this checklist is a defect of the checklist.** The charter
lines own the per-file list; a rule naming what KIND of code lands in which file is the
checklist's own.

**A function, a class (a kernel class among them), a module global (`let` or `var`, private or
not), a named constant or a `require` under `dasllama/` lands in the file whose charter line
names its concern, or the diff adds the concern to the charter line of the file it lands in.
Another file's charter naming the same kind of code does not license the landing.**

**A landing in a file whose charter row carries a `must not hold` cell - the concerns that file
never holds - naming the landed concern is a defect - land it where its concern is chartered, or
change that charter row in the same change.**

**A concern that a two-file role row of `ARCHITECTURE_GPU.md#gpu-backends` scopes to one backend
("on Vulkan ...", "Metal's ...", "on a build without das_metal" - the Vulkan file) licenses only
that backend's file; a concern the row names without a backend licenses both.**

**A diff adding a new item of the sort a charter line's parenthetical lists (a model family, a
weight format, a dispatch arm) lands it in that charter line's file, whether or not the
parenthetical names it, and adds it to the parenthetical in the same change.**

**Never add a charter line for a file outside `dasllama/` - that file answers to its own
folder's checklist.**

**The grid rule - the expression that computes a dispatch's workgroup counts - of a class whose
family ships more than one coopmat form (cm2, KHR) lands in
`dasllama/dasllama_vulkan_classes.das`.**

**A Vulkan host-side ensure/set/encode chain (an if/else over stamps) that picks a stamp from
its push-constant and shape arguments and the device's cooperative-matrix mode
(`g_gpu.coopmat_mode`) alone lands in `dasllama/dasllama_vulkan_classes.das`.** A stamp is one
class stamped from a kernel class template.

**A Vulkan predicate over shape values alone lands in `dasllama/dasllama_vulkan_classes.das`,
whichever file calls it.**

**A Vulkan host-side ensure/set/encode chain whose pick reads driver state - `g_rd` or an
`RLayer` field - lands in the Vulkan driver file whose pass calls it -
`dasllama/dasllama_vulkan_decode.das` for the decode step, `dasllama/dasllama_vulkan_prefill.das`
for the prefill pass, the tower, ASR-decoder or TTS driver file for theirs - and in
`dasllama/dasllama_vulkan_common.das` when both the decode step and the prefill pass call it.**

**A host-side ensure/set/encode chain on any backend that only switches on a stamp it is handed
lands in the file of the function that decides that stamp, and a diff that moves that function
moves the chain with it.**

**A HOST-side tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A kernel-side helper that one backend's kernels splice lands in that backend's kernel file
(`dasllama/dasllama_metal_kernels.das`, `dasllama/dasllama_vulkan_classes.das`); one that both
backends' kernels splice and that reads no codebook table lands in
`dasllama/dasllama_gpu_math.das`, never as a copy per home.**

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

**A branch - one arm of an `if` or a `match` - only one model family takes lands in that family's
own file (`dasllama/dasllama_arch_<name>.das` for a text model, `dasllama/dasllama_<family>.das`
for a media or speech model).**

**A special case only one backend driver needs lands in that driver's file.**

**A branch that is one model family's branch of a kernel two or more model families share lands
in the kernel's file, and that file's charter line names the condition that selects the
branch.**

**Code two tower families both need that names no family type - compute, stage/read, or load
orchestration - lands in `dasllama/dasllama_tower.das`.**

**No signature in `dasllama/dasllama_tower.das` takes a type that
`dasllama/dasllama_audio.das`, `dasllama/dasllama_vision.das`, or a family file declares - the
shared shape lands in `dasllama/dasllama_asr_types.das`.**

**A `dasllama/dasllama_tower.das` helper with one calling family lands in that family's file.**

**Tool wire text (the text of a model's tool/function call, built or parsed) is produced only
in `dasllama/dasllama_tools.das`.**

**Engine, HTTP, or response-writing logic never lands in `dasllama/dasllama_scheduler.das`** -
the forward loops and the model state stay in the other `dasllama/` files; HTTP, and the code
that turns a step's output into the wire text a client reads, in `utils/dasllama-server` (repo
root).

**An `[init]`-only side-effect require in an engine file (`dasllama/`) lives in
`dasllama/dasllama_common.das` when `dasllama_common.das`'s own code needs the registration to have
run and the registered module does not require the engine back, and in
`dasllama/dasllama_transformer.das` otherwise - a require of a backend's entry as that umbrella's
conditional require on the backend module, `require ?vulkan ...` or `require ?das_metal ...`.**
The require umbrella breaks the cycle a module requiring the engine back would close.

**A registration only a program root (test, harness, benchmark, tool) needs gets no side-effect
require in an engine file - the program root requires the registration module directly.**

**A function-typed global that a job (a forked context) invokes, or that a serialized exe must
re-establish and no other file's `[init]` arms, lands in a `dasllama/` file beside the `[init]`
that establishes it.** The `[init]` is the only code that runs where the global arrives unset.

**A `dasllama/` module whose `[init]` registers a hook the engine dispatches through gets its
side-effect require in the same change that adds it** - a registration no engine file reaches
never fires for a consumer of the `dasllama/dasllama.das` facade.

**Platform-specific code - a device call, a `require` of a backend module (the platform's C++
binding: `vulkan`, `das_metal`), or a read of a backend's own state other than its
`g_env_<backend>` knobs - in an engine file (`dasllama/`) lands only in one of that backend's
role files or its entry, per the role table of `ARCHITECTURE_GPU.md#gpu-backends`.**
