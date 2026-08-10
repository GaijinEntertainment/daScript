# dasLLVM Architecture

The design document `CODEREVIEW.md` cites. Numbered sections are the stable reference targets;
usage and installation live in `README.md`, the debugger rail and its roadmap in `DEBUGGING.md`.

## 1. The jit backend pipeline

`run_jit` (`llvm_jit_run.das`) drives one linear pipeline per program: **hash** (content-address
the DLL — first per-function AOT hash plus the loop-hint/llvm-code folds), **init** (engine +
target machine), **declare** (LLVM function declarations for the jit set), **probe** (open the
cached DLL, compare per-function hashes), **irgen** (the das IR emitter over every function),
**optimize** (the LLVM pass pipeline at the requested level, plus the opt-in IR dump and the
post-opt verify), **emit+link** (artifact production — for the DLL path `write_dll` in
`llvm_jit_common.das`, which itself splits into **emit-obj**, machine-code emission, and
**link**, the lld-link spawn), **install** (resolve externs, instrument sim nodes), and
**finalize** (engine teardown / state install). On a cache hit irgen, optimize, and emit+link
are skipped and read as zero.

### 1.1 The timing contract

Every phase above reports its wall time in the `LLVM JIT time:` breakdown printed under
`options log_compile_time` / `policies.log_compile_time` — the same option the front end uses
for its `compiler took` line. The contract exists because an untimed phase is invisible exactly
when someone is hunting where compile time went: a phase outside the breakdown makes the
printed numbers sum short of the always-on total, and the gap has no name. Nested totals are
allowed — a parent entry (emit+link) may cover steps a callee reports separately (emit-obj,
link) — provided both levels print. When a phase is split into finer steps, each step reports
its own number; an aggregate label silently absorbing new sub-steps breaks the contract the
same way an untimed phase does.

### 1.2 The codegen tier

`--jit-opt-level` (CLI, over `policies.jit_opt_level`, default 3) drives both the optimize
pipeline and the DLL path's codegen-side target machine. `write_exe` and AOT-object emission
(`emit_object_only`) deliberately stay at codegen level 3: shipped artifacts are not
content-addressed, so a tier change there has no cache guard to catch it. At level 0 the
injected tune-policy default becomes `fallback` (`jit_cli_opt_level()` in `llvm_tune.das`):
tune winners are raced under O3 codegen, so an O0 run cannot represent them and must not block
on the tuner to mint them.

## 2. Codegen identity — the DLL cache

Jit DLLs are content-addressed: `jit_dll_basename` folds the per-function AOT hashes,
`LLVM_JIT_CODEGEN_VERSION`, the opt/size levels, prologue and debug-info flags, and the target
triple — same inputs, same filename, cache hit. AST-level changes therefore self-invalidate
through the function hashes; **emitter-level changes do not** — a change that alters generated
machine code for identical inputs (IR generation, target-machine setup, `[llvm_code]`
generators, the jit ABI) is invisible to the key and silently serves stale code from cache
unless `LLVM_JIT_CODEGEN_VERSION` is bumped. A pinned `jit_output_path` bypasses the
content-addressed name entirely; its probe compares function hashes only, which is why the
summary line asserts the opt-level tag only when the tier is actually known.
