# dasLLVM Architecture - codegen identity and the DLL cache

Companion of `ARCHITECTURE.md` (sec.2 routes here). Contract: `ARCHITECTURE_COMMON.md` (repo
root). Section 2 keeps its number from the parent.

## 2. Codegen identity - the DLL cache

Jit DLLs are content-addressed: `jit_dll_basename` (`llvm_jit_plan.das`) folds the candidate
set's per-function AOT hashes (`ARCHITECTURE_JIT_ENTRY.md` sec.2), the JIT-only inputs
`fold_jit_only_inputs` collects (`ARCHITECTURE_JIT_ENTRY.md#hint-folds`), `LLVM_JIT_CODEGEN_VERSION`,
and `jit_env_salt`'s build and host inputs - the opt/size levels, prologue and debug-info flags,
the target triple and the rest - same inputs, same filename, cache hit. AST-level changes therefore self-invalidate through the function hashes;
**emitter-level changes do not** - a change that alters generated machine code for identical
inputs (IR generation, target-machine setup, `[llvm_code]` generators, the jit ABI) is invisible
to the key and silently serves stale code from cache unless `LLVM_JIT_CODEGEN_VERSION` is bumped. Stamped `[llvm_code]` *arguments* are not
emitter-level: they fold into both cache keys per function (the hint folds), so a change that
merely re-selects which perm gets stamped - the `[tune]` machinery - self-invalidates with no
bump. "The jit call ABI" is the contract between the
generated code and the engine: the generated function signatures and name scheme
(`create_uid_nodes` / `get_dll_fn_name`), the prologue shape (`jit_emit_prologue`), the
`LlvmJitFlags`/`LlvmJitMode` inputs to the emitter, and the extern-resolution surface the install
phase binds (`DllExternResolver`, `generate_llvm_code`, `instrument_jit`). A pinned `jit_output_path`
bypasses the content-addressed name entirely; its probe compares function hashes only, which is why
the summary line asserts the opt-level tag only when the tier is actually known.

### 2.1 The split obj cache - positional invalidation {#split-obj-cache}

Under `--jit-split-modules`, each per-module partition object is content-addressed too
(`--jit-obj-cache`, on by default under split): its key is the running fold of every module
hash up to and including its own - symbol names, per-function AOT hashes, and the JIT-only
inputs `fold_jit_only_inputs` collects - combined with `jit_env_salt`, the ONE helper both the DLL key and the partition
keys fold their config/environment inputs through (a component folded into one key but not
the other would let a config change link stale objects). The chained prefix makes
invalidation **positional**: module order is topological, so a change in module j re-keys
every partition from j on, while everything before j links its cached `.o` - the probe is
bare file existence, before any per-partition LLVM state is created, so a hit skips
declaration, irgen, and the optimize/emit pool outright. Three consequences for consumers:
**require order is the cache layout** - a module you edit often belongs as late in the
require chain as its dependencies allow (registration-only requires, like the dasLLAMA GPU
tiers, belong at the END of an umbrella, not in a root module everything depends on); a
rename-without-body change still re-keys, because the module hash folds symbol names, not
just function hashes; and **the cache holds exactly one generation** - the GC keep-set is
the current link set, so reverting an edit is an eviction, not a hit: the run after a revert
re-emits from the reverted module on, same as the edit did.

### The runtime's type layouts are a key input {#handled-layouts-key}

Generated code bakes the size of a handled (C++-bound) type and the offset of each of its fields
as constants, and no AST hash sees either: a C++ member added to a bound class moves every field
behind it while each function's source stays the same. `jit_env_salt` therefore folds
`handled_layout_hash` - one term per handled type annotation (module, name, size) and, where the
annotation binds a structure, one per field (module, type, field, offset), over every module the
running binary registers, summed so the order of the walk is no input. A rebuilt runtime whose
layouts moved keys a new DLL and new partition objects, and the old ones are collected as stale;
a rebuild that moved none hits the cache. The walk costs tens of microseconds, so every compile
takes it. What the salt does not see is a bound type's behavior behind an unchanged layout -
that is `LLVM_JIT_CODEGEN_VERSION`'s job.

### Extern slot names are a key input {#extern-slot-names-key}

Generated code reaches a builtin, an interpreted callee, a `@@` function address, a constructor,
a handled type's `at`/`each`/`new`/`delete`/`clone` and the table helpers through address slots
the DLL exports by name, and a cache hit refills them by name (`DllExternResolver`); a name the
DLL does not export is skipped, because a slot lowered inline is absent by design. The names
carry what no AOT hash sees: the callee's mangled name, and the mangled type of a handled
operand, `const` and `&` included - a `var` local turned `let` keeps every function's hash while
renaming its `at` slot. A DLL keyed the same but exporting the old name would keep that slot's
codegen-time address, from a process long gone. So `fold_extern_slot_names` runs the resolver
itself in a fold-only walk, which takes each slot's name and stops before any address is
computed - `UidNodes.hints_only` returns the raw hint, without the per-node id - and
`fold_jit_only_inputs` brings the fold into both keys. The walk folds a call's slot whether or not
the callee is jitted: which callees are is decided after the key, and a link that falls back to
the emitter plans again with that selection set, so a key that read it would differ between the
two plans of one program. The fold is therefore a superset of the slots any load binds, and the
same for every plan. Each hint names its subject by mangled name, mangled-name hash or table key
`Type`; an interpolated node's text changes every run, which would re-key every run.
