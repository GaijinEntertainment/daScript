# src/builtin

## 1. Extern bind flavors

Every C++ function a module registers becomes an interpreter call node. Two node
flavors exist, and the choice is per bind:

- **Shared per-signature node** (`addExtern`, `addExternEx`, `addExternTempRef`,
  `addExternProperty`, `addExternPropertyForType`): the node templates on the function
  SIGNATURE and carries the function pointer as a member, so every bind of one signature
  shares a single instantiation - this is what keeps module binaries small. The typed-eval
  matrix lives once per return carrier in `SimNode_ExtFuncCallRet`
  (`include/daScript/simulate/interop.h`), and a consumed extern costs one virtual call.
- **Per-function NTTP node** (`addExternInline`, `addExternInlineEx`): the function
  pointer is a template constant, so the callee can inline into the call node. Costs one
  node instantiation per function; buys direct-call/inlined dispatch. Functions bound this
  way carry `Function::moreFlags2.nttp`, which `daslib/only_nttp.das` and tooling key on.

The policy: **the modules `REVIEW.md` names bind NTTP; every other module binds
per-signature.** Those modules are on interpreter hot paths, their bind count is bounded,
and misjudgment is one-sided - a needlessly-NTTP cold function costs ~1KB of binary, a
missed hot one costs a few percent forever. The list lives in the rule and in the gate that
enforces it (`review_nttp.das`); repeating it here is how it goes stale. Exempt by
construction, because the NTTP entry points cannot express them: cmres binds (a
struct/array/tuple-shaped result the callee writes
into the caller's result slot - `SimNode_ExtFuncCallAndCopyOrMove`), ref-returning binds
(`SimNode_ExtFuncCallRef`), and interop binds (`addInterop`, raw argument array).

**Boundary case**: the generic container and equality helpers declared in
`include/daScript/ast/ast_handle.h` (repo root) - `das_vector_*`, `das_equ*`,
`das_nequ*`, `das_handle_equ*`, `das_handle_nequ*` - instantiate into whichever module
registers the handled type, from one shared call site. They bind NTTP everywhere: they are
the hottest container plumbing in every module, and a single call site cannot pick a
flavor by destination module. The gate (`review_nttp.das`) skips them by `cppName` prefix.

The gate scans the module registry of its own program, so only modules its
`require` list pulls in are covered - the require list is the coverage list.

## 2. The default module-cache path

`ModuleFileCache::defaultPath` (`module_builtin_ast_serialize.cpp`) returns
`.jitted_scripts/module_cache/<stem>-<hash>.dascache` - relative, so the cache follows the
current directory, and it sits beside the JIT DLL cache. `<stem>` is the script's file name
without its extension. `<hash>` is the first 8 hex digits of a 64-bit hash over the normalized
script path, the running executable's mtime and size (the host resolves its own path through the
OS, so a launch by bare name through `PATH` keys the same file as a launch by path), the host's
own command line - every argument up to the first `--`, plus a `--jit-target` after it (the one
script argument that changes what the host compiles: the target's folds); the other arguments after
`--` belong to the script - and every `NAME=VALUE` environment pair whose name starts with `DAS`,
sorted.

The binary, the command line and the environment are in the key because each changes the
compile without changing a source file: macros read the tune and JIT environment at compile
time and stamp what they read into the AST the cache stores, and a host flag such as
`-v1syntax`, `-jit` or `-project` selects a different grammar, policy set or module graph. A
rebuild, a changed flag set or a changed `DAS*` variable is a different compile, so it gets its
own file instead of a stale hit, and two same-named scripts in different directories never
share one. `-no-module-cache` disables the cache outright, over an explicit `-module-cache
<path>` on the same command line as well as over the default, so a spawner can append it as an
override; beside `-ser` / `-deser` - the explicit round-trip halves, whose verdict is the point
of the run - the host rejects the command line instead of silently disabling them.

A script's own `compile_file` (`rtti_builtin_compile_file`, `module_builtin_ast.cpp`) installs
the same default cache when its `CodeOfPolicies.module_cache` is set, with
`ModuleFileCache::embeddedHostOptions` standing in for the host's command line: this process's
arguments up to `--`, a `--jit-target` after it, and a hash of the policies streamed as a record
stamps them, so two tools compiling one file under different policies keep separate records
instead of rewriting one. `ScriptModuleCache` is the guard around that compile: it empties the
environment's serializer slots before installing - the enclosing compile's stream (a
`compile_file` from an `[init]` runs under the host's armed cache) never feeds the nested one, a
cold nested read included - finishes the cache and puts the slots back on every exit, an
exception's included, and the finish happens before the block runs, so a require the block
issues is not this cache's. The writeback's temporary file carries the process id and a per-process
sequence number, since two subtools compiling one file at once key the same record, as do two
threads of one process.

Every variant is its own record and an engine root's record is 200 MB, so the default directory
is capped: after a writeback `ModuleFileCache::finish` lists the directory's `.dascache` files
and removes the oldest by mtime until it fits `DAS_MODULE_CACHE_LIMIT` megabytes (4096 unless
set; `0` disables eviction), never the record just written. `install` touches the record it
reads, so a record in use is the newest and a stale variant the oldest. Only the default
directory is pruned - an explicit `-module-cache <path>` is the user's - and the limit variable
is the one `DAS*` name the record key skips, since it decides nothing about a compile.

## 3. The interpreter's `[extern]` call

An `[extern]` function (`module_builtin_dasbind.cpp`) is called from the interpreter through
a wrapper `vec4f (*)(void * fn, vec4f * args)`: the node evaluates every argument into a
`vec4f` lane and the wrapper calls `fn` through a C prototype. The wrappers are generated -
`generate_x86_64_calls.das` writes `win_x86_64_wrapper.inc`, `systemV_64_wrapper.inc` and
`systemV_64_extra_wrapper.inc` - one per argument count, result class and float mask of the
first register-class arguments, indexed from 0: four positions on Windows, six on SystemV,
with every integer-class argument spelled `int64_t`, every float-class one `double`, and the
result `int64_t` or `vec4f`. Float class is a `float` or `double` by value; a ref, whatever it
points to, is a pointer and so integer class, on the table and the layout alike. A SystemV call with more than six arguments and a float at index
6 or later needs a prototype from the extra table, keyed by argument count, result class (1 for
a float or double result, as the main table indexes it) and the full mask; its entries are the
hand-listed `systemV_extra` rows in the generator - a new shape is added there and the
generator re-run - and a shape the list lacks is a compile error naming the mask.

The C prototype passes a stack argument as an 8-byte slot. That fits Windows x64, where the
mask covers four positions and everything past the fourth argument is an 8-byte stack slot,
float included. It fits SystemV x86-64 too: its float registers past index 5 are exactly the
extra list's cases. It does not fit arm64: eight float registers, so a float at index 6 or 7
belongs in d6 or d7, and on Apple the stack is packed at natural size and alignment - a 32-bit
`int` in 4 bytes, a `bool` in 1 - where Linux and Android keep 8-byte slots. So on arm64
(Apple, Linux, Android; a Windows arm64 build keeps the table, since `DAS_BIND_ARM64_LAYOUT`
turns on for `__APPLE__` and `__linux__` only while `DAS_BIND_EXTERNAL` is on for every 64-bit
Windows) a call with more than six arguments leaves the table: at bind time the
function's das types produce an `Arm64Layout` - which argument rides in x0-x7, which in d0-d7,
and at which byte offset of a stack image the rest sit, packed on Apple and slotted elsewhere -
and the node calls `das_arm64_call`, an assembly trampoline that copies the image below its
own frame, loads the sixteen argument registers, calls, and stores x0 and d0 for the node to
return. A call of six or fewer arguments has no stack argument and no float past the mask
under any of these ABIs, so it keeps the table there too. A packed width comes from the das
type: a ref, pointer or string is 8 bytes, a scalar its `getBaseSizeOf`, and anything else (an
aggregate by value, which the binder does not support) rides as 8 bytes, as the table would
pass it. A layout lives for the process - `computeArm64Layout` keeps every layout in a static
vector it never frees - because a re-applied `[extern]` replaces the module's function object
while an earlier context's call node, which carries the layout pointer, may still run.

The JIT uses neither: `modules/dasLLVM/daslib/llvm_jit.das` (repo root) emits an extern call
with the function's own LLVM types, so a compiled call is right on every platform. The wrapper
table and the layout are the interpreter's alone, which is why an ABI defect there shows only
in interpreted code - a macro context, a `[no_jit]` function, a plain run. `tests/dasbind`
probes both paths against `tests/dasbind/probe`, a library whose functions sum their arguments
under distinct multipliers, so one argument in the wrong register or slot changes the total.
`tests/dasbind/CMakeLists.txt` builds the probe on a 64-bit host that builds and dlopens a
shared library at test time - not wasm, Android or iOS, none of which walks `tests/` - and
`tests/.das_test` skips the suite only on a 32-bit host, where the probe is not built, and under
dastest's `--ser`/`--deser` sweep. A deserialized program never applies `[extern]`, while a
deserialized call already names its `__dasbind__` function in the `dasbind` module - and only
`apply` or a retarget manufactures that function. A 64-bit desktop tree without the library
fails the suite instead of skipping it.

The `__dasbind__` proxy - the `DasBindFunction` a call to an `[extern]` is retargeted to - is
registered by the `[extern]` annotation's `apply` callback when the registrar - the module
carrying the `[extern]` declarations - compiles, and on demand by `transformCall` when the
`dasbind` module lacks it: the module cache serves a registrar in a process that already
compiled it (a live reload - the same process, so the `dasbind` module's hash shows no drift and
nothing reparses the registrar), or a fresh process reads a dependent's record before any
compile applied the registrar. `transformCall` takes the bind name from the call target's own
`[extern]` declaration every time (`bindNameOf`) - the one input every process has - rather than
from a table keyed on the function object, which every deserialization mints anew and whose
freed addresses a later compile reuses. The proxy's signature types are clones with no source
location (`proxyType`): the declaration's types and file record belong to the registrar's
compile, which a served registrar's program takes with it, while the proxy lives for the
process. A failed bind on the on-demand path is the call's transform error, the diagnostic
`apply` would have given.

## 4. A message that crosses the panic jump

`Context::throw_error_at` formats into a stack buffer and jumps; without C++ exceptions the
jump is a `longjmp`, which unwinds nothing, so a heap-owning local alive at the call - a
`string` a builtin built its message in - leaks the allocation on every panic a `try`/`recover`
catches, and the ASan lane reports it. A builtin that composes its message in a `string` before
the throw releases it first: the text moves to a stack buffer of `throw_error_at`'s own size,
the string's storage is swapped away, and the throw runs with nothing owning heap on the frame.
The pinvoke family - `pinvoke_named`, `pinvoke_impl2_core` and `pinvoke_impl3` in
`module_builtin_debugger.cpp` - does it through `throw_pinvoke_error`; the `[extern]` binder's
refusal (`crash_and_burn` in `module_builtin_dasbind.cpp`, thrown at the first call) does it
inline. The same discipline is the `FMT_THROW` stash in `include/daScript/das_config.h`, where
the temporary dies when the stash statement ends.

## 5. A spawned child's stdout pipe

`spawn_process` (`module_builtin_fio.cpp`) hands the child one pipe for stdout and stderr and
never blocks on it: `process_drain` takes what the pipe holds and returns, and the caller decides
when to come back - the watchdog every 250 ms. The pipe's capacity is therefore the child's
write budget between two drains, and a child that fills it blocks until the next one; it is also
the most a single drain hands the caller, which is what the caller's heap sees between two
collects. A POSIX pipe carries 64 KB by default, and Windows sizes an anonymous pipe at 4 KB
when asked for the default - a chatty child under the watchdog's tick moves at most 16 KB a
second through one of those, the pipe's 4 KB four drains a second - so the Windows pipe is
created at the POSIX capacity, and every platform drains the same bursts.
