# dasLLVM architecture - target feature truth

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`). Sections 4 and 6 moved
here with their numbers; section 10 continues the numbering. Each section is cited by the code
that embodies it.

## 4. Host CPU feature truth on aarch64 {#aarch64-feature-truth}

An aarch64 host target reads its CPU features from two sources, because neither answers alone.
`LLVMGetHostCPUFeatures` returns an EMPTY string on macOS - there the CPU name is meant to imply
the features - and a part this LLVM cannot name maps to the generic CPU, where SDOT and SMMLA
have no instruction to select and codegen aborts. `cpu_supports` reads the operating system
instead (sysctl / `AT_HWCAP` / `IsProcessorFeaturePresent`), so it answers for silicon LLVM has
never heard of. The tier gates (`init_jit_target_flags` - `g_target_arm64_dotprod`, `_i8mm`,
`_fullfp16`) and the target machine's feature string (`create_default_target_machine`) therefore
take the union of the two: an LLVM host-string hit OR a `cpu_supports` hit (fullfp16 additionally
reads darwin-arm64 as always-on - every Apple Silicon part has it). One asymmetry: the host rail's
machine string carries `+dotprod` unconditionally (every part the JIT has run on has it), while the
DotProd GATE probes like its siblings - on an ARMv8.0 host the gate declines and the `sdot4` family
compiles its fallback, whatever the string says. A cross-compile triple takes neither - only the force env - and so
does a generic-CPU standalone exe (one carrying no `[llvm_code]` kernel): its machine is the
ARMv8.0 baseline, which cannot select SDOT or SMMLA, so the DotProd and i8mm gates
(`g_target_arm64_dotprod`, `g_target_arm64_i8mm`) stay off there and every `aarch64_neon` call
that needs either compiles its daslang fallback body. The gates and the machine string are one
truth on both rails: a force-env feature raises the gate AND is appended to the generic machine.

The two ways a feature reaches the target machine's string license different things. A
detection-derived append - `+dotprod` always, `+i8mm` when `cpu_supports` confirms it - is
EXECUTION-safe: the silicon running this process really has the instruction. A
`DAS_JIT_ARM64_FORCE_FEATURES` append is EMISSION-only: it may name silicon this box does not
have, so the artifact is for another machine and executing it here traps.

## 6. The x64 kernel-matrix tier gates {#x64-tier-gates}

An x64 host target publishes eight boolean gates (`g_target_x64_*`), one per instruction tier the
kernel matrix and the tune grids select on: `avx2`, `f16c`, `vnni256` (256-bit VPDPBUSD by either
VEX AVX-VNNI or EVEX AVX512-VNNI+VL), `avx512bw` (zmm byte ops - BW, not merely F), `avx512vnni`
(zmm VPDPBUSD; implies bw, the sign trick around it is BW), `avx512vbmi` (VPERMI2B / VPERMB /
VPMULTISHIFTQB - the grid formats' symbol lattice), `vnniint8` (VEX VPDPBSSD, native s8 x s8),
and `amx` (both amx-tile and amx-int8; the per-process XTILEDATA grant is a separate runtime step
the family's own witness performs). `init_jit_target_flags` decides each from cpuid truth OR'd with
the `DAS_JIT_X64_FORCE_FEATURES` emission-only override; a cross triple or a generic target
(`host_features = false`) drops to forced-only truth - cpuid is not consulted, the force env is
the only tier source there.

The cpuid truth is `das_cpu_supports` (`src/builtin/module_builtin_runtime.cpp`), a hand-kept table
keyed by the LLVM target-feature spelling - so the force env and `llc -mattr` take the same names.
A tier feature usually lands as three parts: its cpuid line there, its name in
`TUNE_KNOWN_FEATURES` (`daslib/llvm_tune.das`, the profile fingerprint the `requires=` gates are
checked against), and - when the emitters branch on it - a `g_target_x64_*` gate. The cpuid line
is the load-bearing one: a name missing from the table answers false on every box, so the perm
that requires it declines everywhere and no error names the cause.

## 10. CPU classes and the baseline {#cpu-classes}

A CPU class is a feature set, never a box model: the set every box of the class carries, spelled
in LLVM's target-feature names. The classes are the rows of `JIT_CPU_CLASSES`
(`daslib/llvm_cpu_class.das`): `x86-base`, `x86-avx2`, `x86-vnni256`, `x86-vnni512`, `x86-amx`,
`arm-neon`, `arm-i8mm`. Each row carries the LLVM cpu its target machine schedules for
(`x86-64-v3` for avx2, `x86-64-v4` for the AVX-512 classes, `generic` on arm) and its feature set.
The set is the fingerprint the class's shipped profile records (`skills/tune.md`, the shipped
defaults profiles), so a profile and the class it is keyed by state the same silicon. A box is
in a class when it carries every feature of the set; `tune_cpu_class()` names the highest class
this box is in, and `jit_cpu_class_ladder` is the one adoption ladder both the compile-time
profile walk and the runtime pick descend. M1 is `arm-neon` (it has no i8mm); M2 and later are
`arm-i8mm`. A vnni512 part without vbmi - the pre-Ice-Lake Xeons - is not in `x86-vnni512` and
runs as `x86-avx2`, because the set is the contract and a subset of it is not.

`DAS_JIT_BASELINE=<class>` builds for a class instead of the running box. It is one truth on
every rail - the DLL, the standalone exe, the AOT object: the class's set reads as forced
(`x64_forced_feature` / `arm64_forced_feature` answer it before the force env), so the tier
gates, the machine's feature string and the `requires=` eligibility of every `[tune_perm]` follow
the class; `init_jit_target_flags` drops to the generic rail whatever the caller asked;
`create_default_target_machine` names the class's cpu; `tune_cpu_class()` answers the class, so the
profile ladder starts at it; and `jit_env_salt` folds the class, so the artifact never serves a
host run. A kernel-carrying exe stops targeting the box (`exe_host_cpu` is false under a
baseline). The force env still appends after the class's set, as after detection. A baseline
never crosses architectures - `jit_baseline_error` refuses a class of another arch, and a name no
row carries, by name, before any IR is built. The variable is environment-only by construction:
the AST module cache keys on `DAS*`, and a flag read at JIT time would serve stamps minted for
another class.

At the runtime of a built artifact the variable is inert for dispatch: the class an exe runs on
is read from cpuid, never from an environment a build left behind.

## 11. The fat mode - one clone per class, picked from cpuid {#fat-mode}

`DAS_TUNE_MODE=fat` is the third `[tune]` mode (the other two: `skills/tune.md`). It is a
standalone-exe mode under a baseline: `stamp_fat` (`daslib/llvm_tune.das`) refuses a build that is
not `-exe` and one with no `DAS_JIT_BASELINE`. The ship set is every class of the baseline's
ladder that some `[tune_scope(defaults=)]` ships a profile for, at or above the baseline, plus
the baseline itself - most capable first, the baseline last; adding a profile file grows the
exe. For each `[tune]` function and each class of the ship set the apply adds a generated clone
`<name>__fat_<class>` (the class with `_` for `-`), stamped with the class profile's winner when
the grid carries it and the class's set passes its `requires=`, else the first `fallback=` entry
the class's set passes, else the reference body. Companions clone per class alongside, taking
their own profile entry when the class's set can run it and the main's pick otherwise. The
stamp carries one extra `[llvm_code]` argument, `tune_class`, which folds into the cache keys
with the rest.

The function's own body becomes the dispatch, generated as daslang: one arm per non-baseline
class, `if ((tune_fat_mask() & bit) != 0) return <clone>(args)`, most capable first, and the
unguarded baseline call last. The JIT lowers that to a load, a compare per arm and direct
calls - no function pointer, no `invoke`. A bit is the class's position on its arch's ladder
(`tune_fat_class_bit`); the mask is one runtime global that starts at zero, so a kernel called
before the `[init]` filled it runs the baseline clone, which is always correct.

The emitter (`try_llvm_code_function`, `daslib/llvm_jit.das`) reads `tune_class` off the
annotation and gives the clone's impl three things: the `target-cpu` and `target-features`
attributes of the class row, so the backend lowers this one function for the class inside a
module that targets the baseline; the tier gates swapped to the class's set for the span of the
generator call (`with_class_target_gates`, `daslib/llvm_jit_common.das`), so a generator that
branches on `g_target_x64_*` emits the class's form; and `noinline`. The last one is
load-bearing: x86's inliner refuses a callee whose features exceed the caller's, but AArch64's
admits one whose body carries no target intrinsic, and the inlined copy would then lower for
the baseline and the clone would be dead code.

The exe's status `[init]` (`emit_status_init`) first calls `tune_fat_init(baseline)`: it refuses
a box that lacks any feature of the baseline class by name, then sets the mask to every class
of the ladder the box is in - membership is cpuid only (`tune_fat_box_in_class`), never the
build's baseline or a force env - or to the one class `DAS_TUNE_FAT_CLASS` pins, refusing a pin
the box is not in. `tune_status()` lists one row per clone with its class (`klass`), and
`log_tune_status` marks the classes this box runs. A profile-covered class needs no sidecar and
no tuner: the exe carries no grid beyond the reference rows, and a re-tune is a new profile and
a rebuild.
