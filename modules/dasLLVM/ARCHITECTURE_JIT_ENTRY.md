# dasLLVM Architecture - the JIT entry and the cache hit

Companion of `ARCHITECTURE.md` (sec.1 routes here). Contract: `ARCHITECTURE_COMMON.md` (repo
root). This document carries how a program reaches the JIT and what a DLL cache hit costs.

## 1. Three modules where there was one

`daslib/just_in_time.das` - the module the host injects under `-jit` - requires
`llvm_jit_link.das` and nothing of the emitter. The driver is split by what a cache hit needs:

- `llvm_jit_plan.das` holds everything `run_jit` decides before it touches LLVM: the resolved
  options (`JitPlan`, `make_jit_plan`), the candidate set, the DLL key (`jit_dll_basename`,
  `jit_env_salt`, `LLVM_JIT_CODEGEN_VERSION`), the DLL probe (`probe_dll`, `get_dll_missing`),
  the extern-slot resolution (`DllExternResolver`, `resolve_dll_externs`) and the sim-node
  install (`install_dll_functions`). Its one use of the LLVM bindings is the three host
  queries the key folds - default triple, host CPU name, host CPU features.
- `llvm_jit_link.das` is the entry: the `jit_llvm` simulate macro, `run_jit_linked`, and the
  `[finalize(late), no_jit]` finalizer that frees the jit state when the last sharing context
  goes.
- `llvm_macro.das` is the emitter's macro context, reached only by name: `run_jit_codegen`
  wraps `run_jit` (`llvm_jit_run.das`), `free_jit_engine` disposes an in-memory engine. Nothing
  requires it from a program.

`run_jit_linked` computes the plan, and when the plan is a content-addressed or pinned DLL with
candidates it opens the DLL and probes it. A hit binds in place: `mark_jit_selection` sets
`requestJit` on the functions the DLL holds and clears it elsewhere, `resolve_dll_externs` fills
the DLL's extern slots, `install_dll_functions` points the sim nodes at the DLL's code, and
`set_jit_state` records the handle. Everything else - a miss, an exe, a wasm, an AOT object,
compile-only, an in-memory run, a static host - goes to the emitter:
`macro_context_of("llvm/daslib/llvm_macro")` (`daslib/cross_context`) brings the module in
through `require_module_now` on the first call and answers the context it already has after,
and `call_in_context` runs `run_jit_codegen` there with the program and context pointers. The
emitter recomputes the same plan - its inputs are the program and the command line - and takes
the pipeline `ARCHITECTURE.md` sec.1 describes; on a warm run the emitter's ~40 modules are never
read from the module cache and its macro contexts never simulated, which is the whole of the
saving (a warm `-jit` hello world: total 0.20 s to 0.08 s, compile 0.17 s to 0.065 s).

## 2. The key folds the candidate set

`jit_dll_basename` folds the AOT hashes of the CANDIDATES - every function the context holds
that the policy selects, `[no_jit]` aside - not of the functions the emitter finally emits.
The emitter's content gate (`DisableJitVisitor`) drops candidates it cannot lower, but the
gate is a function of the candidates, the emitter version and the target, all of which the
key folds, so one key still names one DLL. What the hit path gains is that it never runs the
gate: `probe_dll` binds a candidate the DLL holds under a matching hash, leaves one the DLL
lacks to the interpreter (the gate dropped it at codegen), and calls a candidate held under
another hash, or a `[no_jit]` function the DLL holds, a miss. `LLVM_JIT_CODEGEN_VERSION` 0x78
is the key's change of shape.

The candidate set reads the CONTEXT, not the `used` flag alone: `get_function_by_mangled_name_hash`
answers whether the simulated program holds the function. A module compiled into the process
after this program simulated - the emitter a late require brings in - runs its own symbol-use
passes over the shared modules it requires, and `requireModuleNow` restores every shared
function's and global's `used` flag afterwards (`src/ast/ARCHITECTURE.md` sec.3); the context
check is the second line, for the window inside one plan.

## 3. The extern resolver needs no intrinsic table

`DllExternResolver` walks each bound function and fills the DLL's extern slots: the interop
address of every builtin called, the interpreter address of every callee the DLL does not hold
(`requestJit` false), the handled-type and table helpers. The emitter's resolver used to consult
`has_intrinsic` to skip calls it had lowered inline; the DLL already knows - a lowered call has no
slot - so the resolver asks `set_glob_address`, which returns false for a slot the DLL lacks, and
only a slot the DLL holds and nothing can fill is an error. The emitter and the hit path share the
one resolver, and the visitor's borrowed fields (`dll`, `jit_context`, `uid`) carry
`@do_not_delete` because the resolver is deleted after the walk and its objects live on.

## 4. What stays out of the program's jitted set

The finalizer is program code, so everything it reaches joins the program's used set and is a JIT
candidate in every jitted program. `free_jit_context` and `free_jit_engine_in_emitter` are
`[no_jit]` and call externs only (`get_module`, `find_macro_context`, `invoke_in_context`); a das
helper on that path - `macro_context_of`, say - would put `daslib/cross_context`'s functions,
blocks passed to C++ with a `LineInfoArg`, into every program's DLL, and the emitter cannot lower
those. `llvm_exe.das`'s standalone-exe sweep skips `llvm_jit_link` the way it skips `llvm_macro`:
an exe has no jit state to free, and the strict sweep refuses a `[no_jit]` function otherwise.
A batch of programs in one process (dastest) exercises both: a program that fails inside the
emitter must drain the codegen accumulators (`reset_codegen_accumulators`) on its way out, or the
next program's fileinfo ctor asserts on names that belonged to the last one.
