# dasLLVM Architecture - codegen identity and the DLL cache

Companion of `ARCHITECTURE.md` (sec.2 routes here). Contract: `ARCHITECTURE_COMMON.md` (repo
root). Section 2 keeps its number from the parent.

## 2. Codegen identity - the DLL cache

Jit DLLs are content-addressed: `jit_dll_basename` (`llvm_jit_plan.das`) folds the candidate
set's per-function AOT hashes (`ARCHITECTURE_JIT_ENTRY.md` sec.2), `LLVM_JIT_CODEGEN_VERSION`, the
opt/size levels, prologue and debug-info flags, and the target triple - same inputs, same
filename, cache hit. AST-level changes therefore self-invalidate through the function hashes;
**emitter-level changes do not** - a change that alters generated machine code for identical
inputs (IR generation, target-machine setup, `[llvm_code]` generators, the jit ABI) is invisible
to the key and silently serves stale code from cache unless `LLVM_JIT_CODEGEN_VERSION` is bumped. Stamped `[llvm_code]` *arguments* are not
emitter-level: they fold into both cache keys per function (the hint folds), so a change that
merely re-selects which perm gets stamped - the `[tune]` machinery - self-invalidates with no
bump. "The jit call ABI" is the contract between the
generated code and the engine: the generated function signatures and name scheme
(`create_uid_nodes` / `get_dll_fn_name`), the prologue shape (`jit_emit_prologue`), the
`LlvmJitFlags`/`LlvmJitMode` inputs to the emitter, and the extern-resolution surface the install
phase binds (`ResolveExternVisitor`, `generate_llvm_code`, `instrument_jit`). A pinned `jit_output_path`
bypasses the content-addressed name entirely; its probe compares function hashes only, which is why
the summary line asserts the opt-level tag only when the tier is actually known.

### 2.1 The split obj cache - positional invalidation

Under `--jit-split-modules`, each per-module partition object is content-addressed too
(`--jit-obj-cache`, on by default under split): its key is the running fold of every module
hash up to and including its own - symbol names, per-function AOT hashes, and the JIT-only
hint folds - combined with `jit_env_salt`, the ONE helper both the DLL key and the partition
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
