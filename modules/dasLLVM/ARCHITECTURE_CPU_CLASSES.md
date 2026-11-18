# dasLLVM architecture - CPU classes and tune under a class

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`); the CPU feature truth and
the tier gates a class is read through are `ARCHITECTURE_TARGET_FEATURES.md`. Sections 10 and 11
keep their numbers. Each section is cited by the code that embodies it.

## 10. CPU classes and the baseline {#cpu-classes}

A CPU class is a feature set, never a box model: the set every box of the class carries, spelled
in LLVM's target-feature names. The classes are the rows of `JIT_CPU_CLASSES`
(`daslib/llvm_cpu_class.das`): `x86-base`, `x86-avx`, `x86-avx2`, `x86-vnni256`, `x86-vnni512`,
`x86-amx`, `arm-neon`, `arm-i8mm`, `arm-sve`, and `wasm-simd128`, the one class a cross triple
implies (the cross-class section below). `x86-avx` is the AVX parts with no AVX2 - Jaguar, Ivy
Bridge, Piledriver: hardware half converts, integer SIMD at 128 bits. The rows above it do not
name `avx`, so a `requires=` that admits it and none of them names the absence, `!avx2`. Each row carries the LLVM cpu its target machine schedules for and its
feature set, and the cpu is the arch's bare baseline (`x86-64`, `generic`) on every row: an LLVM
cpu level such as `x86-64-v3` implies features the row does not name (bmi2, lzcnt, movbe; `v4`
adds avx512cd/dq), the backend would emit them, and the membership test below would admit a box
without them. The set alone is what the backend enables and what cpuid is asked for. `x86-amx`
carries `amx-bf16` and `avx512bf16` beside `amx-tile` and `amx-int8` because every AMX part,
from Sapphire Rapids on, has all four, so the extra features do not narrow the class.
The set is the fingerprint the class's shipped profile records (`skills/tune.md`, the shipped
defaults profiles), so a profile and the class it is keyed by state the same silicon. A box is
in a class when it carries every feature of the set; `tune_native_target().klass` names the highest class
this box is in, and `jit_cpu_class_ladder` is the one adoption ladder both the compile-time
profile walk and the runtime pick descend. M1 is `arm-neon` (it has no i8mm); M2 and later are
`arm-i8mm`; Neoverse V1/V2 (Graviton3+) are `arm-sve`. `arm-sve` gates no seat of its own - it
splits off because the same seats crown different winners there: SMMLA issues at several per
cycle on Neoverse and on one NEON pipe on Apple, so one profile cannot serve both, and Apple
reports no SVE (its SME is the streaming mode only). A vnni512 part without vbmi - the pre-Ice-Lake Xeons - is not in `x86-vnni512` and
runs as `x86-avx2`, because the set is the contract and a subset of it is not.

`DAS_JIT_BASELINE=<class>` builds for a class instead of the running box. It is one truth on
every rail - the DLL, the standalone exe, the AOT object: the class's set reads as forced
(`forced_plus_features` puts it before the force env's names), so the tier
gates, the machine's feature string and the `requires=` eligibility of every `[tune_perm]` follow
the class; `make_jit_target` drops to the generic rail whatever the caller asked;
`create_default_target_machine` names the class's cpu; `tune_process_target().klass` answers the class, so the
profile ladder starts at it; and `jit_env_salt` folds the class, so the artifact never serves a
host run. A kernel-carrying exe stops targeting the box (`exe_host_cpu` is false under a
baseline). The force env still appends after the class's set, as after detection. A baseline
never crosses architectures - `jit_baseline_error` refuses a class of another arch, and a name no
row carries, by name, before any IR is built. `jit_baseline_class` is pure - the variable comes
in as an argument - and the class is read once, where a build starts: the JIT plan
(`plan.baseline`, then `JitTarget.baseline` for everything the target drives) and each tune macro's
entry, and passed down from there. The tune side passes content, not the name. A compile stamps kernels from one `TuneView` - its
`TuneTarget` (the class and the `requires=` set it passes, the class's row or else this box's
cpuid), the sidecar's picks and the profiles read for that target down the target's own arch's
ladder - built once per macro entry by `tune_view` and passed down, so no helper below an entry
reads a file, a flag or cpuid. A running program has no view: its kernels are stamped already, and
what it writes - a sidecar, an adopted profile - is this box's, so it reads `tune_native_target()`.
Beside `--jit-target` the class is the cross build's: the triple names the arch, the class its
features. A cross target's default machine is the target's own - its triple, the class's cpu (none without
a class), its feature string (`create_default_target_machine`) - so the optimizer runs on the target's cost model and
no host machine is ever handed a set that belongs to another machine. A wasm compile is the exception: its
passes run under the host's generic machine (`ARCHITECTURE_TARGET_FEATURES.md#wasm-feature-string`), which carries no feature string at all.

At the runtime of a built artifact the variable is inert for dispatch: the class an exe runs on
is read from cpuid, never from an environment a build left behind.

## 11. The fat mode - one clone per class, picked from cpuid {#fat-mode}

`DAS_TUNE_MODE=fat` is the third `[tune]` mode (the other two: `skills/tune.md`). It is a
standalone-artifact mode under a baseline: `stamp_fat` (`daslib/llvm_tune.das`) refuses a build that
is neither `-exe` nor `-lib` and one with no `DAS_JIT_BASELINE`. The command line sets one compile
policy for both (`jit_exe_mode`), which is what the macros read (`tune_exe_gate`): the option that
tells the JIT to emit a library is set after the compile, too late for a macro. The ship set is every class of the baseline's
ladder that some `[tune_scope(defaults=)]` ships a profile for, at or above the baseline, plus
the baseline itself - most capable first, the baseline last; adding a profile file grows the
exe. For each `[tune]` function and each class of the ship set the apply adds a generated clone
`<name>__fat_<class>` (the class with `_` for `-`), stamped with the class profile's winner when
the grid carries it and the class's set passes its `requires=`, else the first `fallback=` entry
the class's set passes, else the reference body. Companions clone per class alongside, taking
their own profile entry when the class's set can run it and the main's pick otherwise. The
stamp carries one extra `[llvm_code]` argument, `tune_class`, which folds into the cache keys
with the rest. A reference clone carries no stamp, so its status row reads the class profile
instead: an explicit `reference` entry is the profile's decision and the row's source is
`profile`, and a companion's row with no entry of its own reads its main's entry - the fat
release gate and the exe's own start-up gate both count only `profile` rows.

The function's own body becomes the dispatch, generated as daslang: one arm per non-baseline
class, `if ((tune_fat_mask() & bit) != 0) return <clone>(args)`, most capable first, and the
unguarded baseline call last. The JIT lowers that to a load, a compare per arm and direct
calls - no function pointer, no `invoke`. A bit is the class's position on its arch's ladder
(`tune_fat_class_bit`); the mask starts at zero, so a kernel called before the `[init]` filled
it runs the baseline clone, which is always correct.

The mask is an LLVM-side word, not a das global: `tune_fat_mask()` and its setter are
`[llvm_code]` stubs whose generators (`daslib/llvm_tune_fat_word.das`) emit a load and a store
of one module global, `das.tune.fat_mask` (`tune_fat_built()` rides a second one). A das global
lives per context, and an engine's job contexts are pooled forks cloned with the init script
skipped (`set_jobque_fork_pool(keep, skipInit)`), so their copy would stay zero and every
kernel dispatched from a worker would run the baseline clone against planes the main context
laid out for its own class. Process memory has one copy, so a fork reads what the main
context's `[init]` stored, and the chain's guard is a plain load the backend hoists. The stubs'
das bodies (the das globals) serve the interpreter and AOT, where there is no fat exe. The
spelling `[llvm_code]` inside `llvm_tune` comes from `daslib/llvm_code_shell.das`, an
annotation-only module required non-publicly: `daslib/tune` carries the user-facing annotation
and requires `llvm_tune`, so `llvm_tune` cannot see it, and a program must never see both. The
shell is its own module rather than a corner of the generator module because `llvm_tune`'s
closure reaches every program with a `[tune_policy]`, JIT or not: pulling the LLVM bindings in
there applies their `[extern]` stubs to the `dasbind` builtin in a different set and order per
process, and the module cache reads that as a builtin hash drift and reparses.

The emitter (`try_llvm_code_function`, `daslib/llvm_jit.das`) reads `tune_class` off the
annotation and gives the clone's impl three things: the `target-cpu` and `target-features`
attributes of the class row, so the backend lowers this one function for the class inside a
module that targets the baseline; the generator's `JitTarget` (`gc.jit.target`) as the class's
copy (`class_target`, `daslib/llvm_jit_common.das`), so a generator that branches on the
`x64_*` gates emits the class's form; and `noinline`. The last one is
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

The status `[init]` hangs on the default-policy pass, which finds a program root by its `main`. A
library has none, so a standalone root with no `main` gets the `[init]` from `library_status_init`,
once, with one call ahead of the rows: `tune_library_init`, which sets a third LLVM-side word,
`das.tune.library_built`. The pick then runs when the host creates the library's context, and
`tune_library_built()` tells code that would write a per-user file for an exe that its host owns
the process.

## A cross triple's class {#cross-class}

A cross compile builds for a class when one serves its triple: `jit_cross_class`
(`daslib/llvm_cpu_class.das`) answers `wasm-simd128` for a wasm64 triple, the baseline's class
for any other triple built under `DAS_JIT_BASELINE`, and nothing for a triple with neither. The
wasm class's set is the wasm runtime archive's own - `simd128`, the feature string of
`ARCHITECTURE_TARGET_FEATURES.md#wasm-feature-string` - so every wasm64 artifact is in the class and no box is asked. A baseline under
a triple is how a target of fixed hardware gets its class: the build names the class the
platform is, and nothing reads a cpuid. `tune_process_target` takes the named class when one is
set and the cross triple's otherwise, the cross class with its set alone, and marks the target a
cross one under any triple a class serves. The `TuneTarget`'s set, the `requires=` test and the
profile ladder read it, so the build box's cpuid decides nothing about
a cross artifact; inside the artifact `cpu_supports("simd128")` answers true, because the
runtime archive is compiled with the feature. Its tune identity asks no box either:
`tune_box_identity` names the target's platform - the triple itself where no platform name maps
it - and arch, and puts the class in the cpu field,
so every artifact of one triple and class carries the same identity.

Under a cross class the tune framework stamps as it does for a standalone exe: each `[tune]`
function takes its class profile's winner, else the first `fallback=` entry the class's set
passes, else its reference body. The sidecar beside the app does not stamp
(`stamp_manifest_path`, `daslib/llvm_tune.das`): the build box measured it, for its own silicon.
`DAS_TUNE_FORCE` still stamps - it is how a sidecar minted on the target reaches a build. A
cross triple no class serves stamps reference bodies (`tune_aot_gate`), and
`--jit-compile-only` keeps its grid for an emission dump.

A cross release ships no `llvm` module (`write_release_deps`, `daslib/llvm_exe.das`): the
artifact cannot host the compiler, so a program's `require llvm` is a build-time witness there
and names no archive to link. A tuner built for the target requires it for its registries and
links without it.

## A build for a class keeps the rows the class can run {#cross-grid}

`DAS_TUNE_MODE=tune` and `=test` stamp a family's whole grid on a build for the running box:
the box that runs the grid is asked at run time, a grid compiled on one box races on another,
and a force variable widens the set. A build for a class - a cross triple's, or the one
`DAS_JIT_BASELINE` names - stamps only the rows whose `requires=` the class passes
(`stamped_rows`): the class is the build's answer, every row above it would emit its reference
body, and a mint under the class would race those bodies family after family. Nothing widens a
cross artifact's set; under a native baseline the force variables still append, as they do to
the class's set on every rail. A family with no such row keeps its reference body and a one-row
`<name>_variants()` registry, which a tuner reads as nothing to race: it writes no entry for it -
the entry the section below leaves off the demand.

## A family with no seat the class can run demands no entry {#seatless-families}

A scope's demand is the list of kernels a sidecar or a profile owes an entry for
(`module_kernel_fnames`, `daslib/llvm_tune.das`). A `[tune]` family is on it only when the class
the compile builds for has a seat in the family: a `[tune_perm]` row with no `requires=`, or one
whose `requires=` the class passes. A family whose every row is gated on features the class
lacks stamps its reference body there whatever a file says, so an entry for it would decide
nothing; the demand leaves it out, and with it the completeness check, the startup guard and
the release's status walk, which all read that one list. This is what lets a library carry
kernels for one class alone - the wasm rows kernels gate every row on `simd128` - without every
other class's profile owing them an entry. A fat build asks the same of its ship set
(`grid_has_seat_in`): a family that no class of the set has a seat in gets no clone and runs its
reference body.

## A mint with no file system keeps its sidecar in memory {#tune-memory-sidecar}

A target whose file IO is its host's own - a console - cannot hand the tuners a sidecar path.
`set_tune_manifest_in_memory` (`daslib/llvm_tune.das`) points the sidecar at a text the process
holds: `tune_manifest_path` answers a name no file carries, and the one reader and the one
writer of a sidecar's bytes (`sidecar_read`, `sidecar_write`) serve that name from memory, so
every get, set, merge and export runs as on a file and nothing is opened. The checks that
judge a file's sidecar by its date and its box start from the file's `stat`, which the name
fails, so the text is never stale: the process that reads it wrote it. `tune_manifest_text` hands the whole
sidecar back and `tune_profile_text` the class profile cut from it, for the host to carry out.
The setter ranks ahead of `DAS_TUNE_FORCE` and `DAS_TUNE_MANIFEST` for the rest of the process.

## A forced build stamps from one named file {#tune-forced-file}

`DAS_TUNE_FORCE=<file>` builds against one tune file whatever box minted it - a box sidecar or a
class profile, which carry the same `kernels` map. The file takes the sidecar's place:
`tune_manifest_path` answers it ahead of `DAS_TUNE_MANIFEST` and of the default beside the app.
The two checks that judge a sidecar by where and when it was written - another box's identity, a
date before the binary's - do not apply to it (`tune_sidecar_stale`, `tune_sidecar_verdict`),
and the compile prints one line that names the knob and the file. No saver writes the file:
`save_sidecar_doc` refuses it, so a tuner run under the force cannot write over a profile.
Everything else is the sidecar rail unchanged: a kernel the file has no entry for falls to its
class profile and then to its `fallback=`, a scope the file leaves incomplete prints the warning
naming the kernels it misses, and the policy rail is off, as under any named sidecar.

Those checks exist because a measurement belongs to the box and the binary that made it. A forced
build is for hardware the build machine is not - a console, the lowest class of a fleet - where
the file is a decision and there is no box to measure. The file sets no baseline:
`DAS_JIT_BASELINE` still names the class the code is lowered for, and a forced build sets both.
