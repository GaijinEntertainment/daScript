# dasLLAMA Placement Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_ENGINE_SERVING.md`,
`ARCHITECTURE_ENGINE_FORMATS.md`, `ARCHITECTURE_GPU.md`, `ARCHITECTURE_GPU_SHARED_KERNELS.md`,
`ARCHITECTURE_MEDIA.md`, `ARCHITECTURE_TTS.md`, `ARCHITECTURE_POCKET.md`. Planned work:
`followup_general.md`, `followup_vulkan.md` for Vulkan, `followup_metal.md` for Metal.

A charter line is a file's line in the companion section that `ARCHITECTURE.md#file-charters` routes
that file to, or its role row or file bullet in `ARCHITECTURE_GPU.md#gpu-backends`.

**A per-file inventory restated in this checklist is a defect of the checklist.** The charter
lines own the per-file list; a rule naming what KIND of code lands in which file is the
checklist's own.

**A top-level declaration under `dasllama/` - a def, a class (a kernel class among them), a
struct, an enum, a typedef, a module global (`let` or `var`, private or not), a named constant or
a `require` - lands in the file whose charter line names its concern, or the diff adds the concern
to the charter line of the file it lands in. Another file's charter naming the same kind of code
does not license the landing.**

**A landing in a file whose charter row carries a `must not hold` cell - the concerns that file
never holds - naming the landed concern is a defect - land it where its concern is chartered, or
change that charter row in the same change.**

**A diff that lands, in one backend's file, a concern a role row of
`ARCHITECTURE_GPU.md#gpu-backends` scopes to another backend ("on Vulkan ...", "Metal's ...",
"on a build without das_metal" - the Vulkan file) is a defect - land it in the backend file the
row names; a concern the row names without a backend lands in any file of the row.**

**A diff adding a new item of the sort a charter line's parenthetical lists (a model family, a
weight format, a dispatch arm - and a function of the sort a parenthetical of function names lists)
lands it in that charter line's file, whether or not the parenthetical names it, and adds it to the
parenthetical in the same change.**

**Never add a charter line for a file outside `dasllama/` - state what it holds in its own
folder's architecture doc or `CLAUDE.md` instead.**

**The expression that computes a dispatch's workgroup counts from one coopmat form's tile shape,
for a class whose family - every class stamped from one class template, directly or through a
template derived from it - ships more than one coopmat form (cm2, KHR), lands in
`dasllama/dasllama_vulkan_classes.das`.** A count computed the same way for every form is not keyed
on a form and stays in the function that computes it for all of them.

**A Vulkan host-side ensure/set/encode chain (an if/else over stamps) whose pick reads nothing but
its arguments and the device's cooperative-matrix mode (`g_gpu.coopmat_mode`), and no driver
state, lands in `dasllama/dasllama_vulkan_classes.das`; a stamp selector it is handed - a value
naming the stamp, such as a variant ordinal - is one of those arguments.** A stamp is one class
stamped from a kernel class template.

**A function that reads shape values alone for its result - a bool or a count over a model's
dims, row counts and formats, nothing read off a device - that a `dasllama/dasllama_*vulkan*.das`
file calls and no `dasllama/dasllama_*cuda*.das` or `dasllama/dasllama_metal*.das` file calls lands
in `dasllama/dasllama_vulkan_classes.das`.**

**A function that reads shape values alone for its result, called from a
`dasllama/dasllama_*vulkan*.das` file and a `dasllama/dasllama_metal*.das` file and from no
`dasllama/dasllama_*cuda*.das` file, lands in a file that every calling file requires, directly or
through a public require, after the change.**

**A function that reads shape values alone for its result, called from both a
`dasllama/dasllama_*vulkan*.das` file and a `dasllama/dasllama_*cuda*.das` file, lands in
`dasllama/dasllama_gpu_math.das`.**

**A Vulkan descriptor set's writer - a host-side ensure/set/encode chain whose pick reads driver
state (`g_rd`, or an `RLayer` field read from a global or an argument), or a function that builds
the set at model load - lands in the Vulkan driver file of the pass whose dispatches read that
set, per its role row in `ARCHITECTURE_GPU.md#gpu-backends`, and in
`dasllama/dasllama_vulkan_common.das` when dispatches of more than one pass read it.**

**A Metal host-side ensure/set/encode chain whose pick reads nothing but a stamp selector it is
handed lands in the file of the function that decides that stamp, and a diff that moves that
function moves the chain with it.**

**A HOST-side tensor format conversion lands in `dasllama/dasllama_convert.das`.**

**A kernel-side helper - a free `def` outside any class that a kernel body calls - that one
home's kernels splice lands in that home's kernel file: `dasllama/dasllama_metal_kernels.das`,
`dasllama/dasllama_vulkan_classes.das` or `dasllama/dasllama_cuda_classes.das`.**

**A kernel-side helper that the Vulkan and the CUDA kernels splice and the Metal kernels do not
lands once in `dasllama/dasllama_gpu_kernels_discrete.das` - never as a copy per home.**

**A kernel-side helper that reads no codebook table and that the Metal kernels and a discrete
home's kernels splice lands once - never as a copy per home - in
`dasllama/dasllama_gpu_kernels_common.das` when it carries `[spirv_inline]`, and in
`dasllama/dasllama_gpu_math.das` when it does not.**

**A method of a class template more than one home stamps lands with its template.**

**A disk-order -> compute-order transform lands by its consumer: a transform into the layout the CPU
kernels read row by row in `dasllama/dasllama_repack.das`, a transform into the layout a GPU plane
or gather reads in `dasllama/dasllama_layout.das`; a panel a batch call builds into scratch and
consumes in the same call is no such transform and lands beside the walk that builds it.**

**A CPU KV-cache store, read, score dot, or V-accumulate OVER CACHE BYTES - a codec primitive
that knows the K/V element format - lands in `dasllama/dasllama_kv_codec.das`.** A dot over an
already-decoded f32 row is not a codec primitive.

**A pre-tokenizer split lands in `dasllama/dasllama_pretok.das`; a merge algorithm in its
tokenizer's own file (`dasllama/dasllama_spm.das` / `dasllama/dasllama_bpe.das`).**

**A GPU kernel body under `dasllama/` - the arithmetic loop a dispatch class (a class a
`[metal_dispatch]`, `[vk_dispatch]` or `[cuda_dispatch]` declares) picks one variant of - that one
home stamps lands in that home's kernel file: `dasllama/dasllama_metal_kernels.das`,
`dasllama/dasllama_vulkan_classes.das` or `dasllama/dasllama_cuda_classes.das` - a class stamped
from a template declared in another file is not one.**

**A class stamped from a template declared in another file lands in its home's kernel file, or in a
file whose role row in `ARCHITECTURE_GPU.md#gpu-backends` names that template's stamps; a diff that
stamps one in another file of the role table adds that template's stamps to the file's row, and a
file in no row of the role table never holds one.**

**A `class template` two homes stamp never sits in one home's kernel file: it lands in
`dasllama/dasllama_gpu_kernels_discrete.das` when the Vulkan and the CUDA homes stamp it and Metal
does not, and in `dasllama/dasllama_gpu_kernels_common.das` when the Metal and the Vulkan homes
stamp it, whether or not the CUDA home does
(`ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-shared-kernels`).** Weakening `REVIEW.das`'s
`check_shared_template_homes` is a defect.

**A CPU kernel body under `dasllama/` - a function whose loop turns a weight format's packed bytes
(block-quantized bytes with their scales, not an f16 or bf16 element plane) into a dot product or a
dequantized f32 row, a `[tune]` variant, a function a `register_kernel_backend` call or a
`KernelBackend` field default names, or a scalar body those call - lands in a tier file,
`dasllama/dasllama_math_<tier>.das`, never in `dasllama/dasllama_math.das`; a body that a
`register_kernel_backend` call in another tier file names, or that two tier files' calls name, lands
in `dasllama/dasllama_math_default.das`.**

**A branch - one arm of an `if` or a `match` - that only one model family takes, in a file whose
charter line names no concern of that family, lands in that family's own file
(`dasllama/dasllama_arch_<name>.das` for a text model, `dasllama/dasllama_<family>.das` for a media
or speech model).**

**Engine code - a `dasllama/` file in no row of the role table of
`ARCHITECTURE_GPU.md#gpu-backends` - that calls a tier seat (a hook
`dasllama/dasllama_gpu_tier.das` installs for a backend driver) lands in a file that already
calls that seat; a seat's first engine-side call lands by its concern's charter line.**

**A branch that is one model family's branch of a kernel two or more model families share lands
in the kernel's file, and that file's charter line names the condition that selects the
branch.**

**Code two tower families' files call directly that names no family type - compute, stage/read, or
load orchestration - lands in the file whose charter line names that shared concern, and in
`dasllama/dasllama_tower.das` when no charter line does.**

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
`dasllama/dasllama_transformer.das` otherwise.** The require umbrella breaks the cycle a module
requiring the engine back would close.

**A diff that requires a backend's entry in `dasllama/dasllama_transformer.das`, for a backend
whose module a build can lack, writes the umbrella's conditional require on the backend module
(`require ?vulkan ...` / `require ?das_metal ...`) - never a plain require.**

**A diff that requires a backend's entry in `dasllama/dasllama_transformer.das`, for a backend
whose module registers in every build that carries it (`dasCUDA`, under its CMake switch), writes
the umbrella's conditional require on the `cuda` witness module (`require ?cuda`) - never a device
probe.**

**A registration only a program root (test, harness, benchmark, tool) needs gets no side-effect
require in an engine file - the program root requires the registration module directly.**

**A function-typed global that a job (a forked context) invokes, or that a serialized exe must
re-establish and no other file's `[init]` arms, lands in a `dasllama/` file beside the `[init]` that
establishes it; a global declared beside its public setter, filled from the registering file's
`[init]`, complies.** The `[init]` is the only code that runs where the global arrives unset.

**A `dasllama/` module whose `[init]` registers a hook the engine dispatches through gets its
side-effect require in the same change that adds it** - a registration no engine file reaches
never fires for a consumer of the `dasllama/dasllama.das` facade.

**Platform-specific code - a device call, a `require` of a backend module (the platform's binding:
`vulkan`, `das_metal`, a `cuda/` module of `dasCUDA`), or a read of a backend's own state other
than its `g_env_<backend>` knobs - in an engine file (`dasllama/`) lands only in one of that
backend's role files or its entry, per the role table of `ARCHITECTURE_GPU.md#gpu-backends`.**
