# JIT compile-time program - the fast dev loop and its invalidation ledger

The iquant arc's session cost is dominated by recompilation, not by racing or by the model:
one zen2 session logged ~20 of 32 hours in front-end + LLVM codegen + full-walk mints. The
mints are fixed (`--tune-only`, skills/tune.md); this file carries the compile-time half:
the measured levers, the adopted dev-loop, and the invalidation defects that cap it.

## Measurements (2026-08-31, zen2, lcpp_bench: 99 das-modules, 7215 fns, 16 threads)

| lever | cold | warm | after one mid-chain kernel edit |
|---|---|---|---|
| default (monolith JIT, no AST cache) | 196 s | 48 s | 196 s |
| `--jit-split-modules=-1` (after `--`) | 65 s (codegen 19.2 s vs 155.8 s - 8.1x) | 48 s (codegen 1.1 s) | 65 s (72/99 partitions re-emit) |
| `-module-cache <path>` (front end 39.7 s -> 3.1 s) | +write | 5.2 s combined | no help - full re-parse + re-write |
| front end alone (parse + infer + interp macros) | 39.7 s | - | 39.7 s |

The combined warm loop (AST cache + split obj cache) is 5.2 s vs 48 s; the edit loop is
59 s vs 196 s. Most tuner/parity/bench invocations recompile UNEDITED programs (mints,
re-execs, A/B reruns), so the warm 9x is where the session hours went.

## Adopted dev loop (the kernel workflow)

```
daslang.exe -jit <app>.das -- <app args>
```

The module cache and the split-module JIT are both on by default (one cache file per app under
`.jitted_scripts/module_cache/`, already gitignored; `-no-module-cache` opts out, `-module-cache
<path>` names it; `--jit-split-modules=0` after the `--` selects the one-unit monolith). Caveats
that stay:

- **Bench t/s rows run the stock monolith invocation** until an A/B proves split-neutral -
  split loses cross-module inlining across partitions, so its artifact is not the shipped one.
- The first cache-backed run re-keys the JIT cache once (ledger item 1) - expect one cold
  codegen right after a cache write.
- QUIRK 21 (HOW_TO_ADD_A_FORMAT.md) still applies: macro-side emitter edits are invisible
  to every key - `rm -rf .jitted_scripts` after `dasllama_gemm_gen.das` changes (which also
  drops the module caches stored under it - correct, the AST changed).

## Invalidation ledger

1. **Deser-vs-parse AOT-hash divergence - DONE.** A deserialized AST hashes identically to the
   parsed one: `tests/jit_tests/llvm_ast_roundtrip.das` round 2 links 3/3 cached partitions
   from a `-deser` compile, and a cache write followed by a JIT DLL cache HIT was observed at
   lcpp_bench scale (9364 functions, M5, 2026-09-04).
2. **Split obj keys are a chained prefix fold - one edit re-emits the suffix.** Editing
   `dasllama_repack` re-emitted 72 of 99 partitions (18.3 s, nearly full cold): every module
   AFTER the edited one in program order re-keys, though their IR is unchanged. Done =
   per-module keys derived from the module's own (transitive-interface) hash, so a leaf edit
   re-emits ~itself; measured target: an early-chain kernel edit under ~5 s of codegen.
3. **An edit re-parses everything after the edited module - RULED OUT as a defect.** Measured
   (lcpp_bench, M5): a one-line edit to module 67 of 165 served the 66-module prefix (0.99 s
   cold -> 0.00 s) and re-parsed the 99-module suffix (11.65 -> 12.05 s); the run reads as
   cold because dasLLAMA's heaviest modules (`dasllama_vulkan_classes` 4.1 s,
   `dasllama_metal_kernels` 1.7 s, `dasllama_metal_prefill` 1.1 s) sit late in the chain. A
   dependency-aware cutoff is impossible with macros - a macro can rewrite any later module,
   and no `require` graph bounds that, so the positional prefix is the only sound rule. The
   verdict line now says how much was served and where the cutoff fell; the mitigation stays
   require order (a hot-edit module as late in the chain as its dependencies allow).
4. **Macro-emitter changes are invisible to every cache key** (QUIRK 21; QUIRK 15's unquirk
   note wants the generator hash folded into the sidecar identity too). Done = an emitter
   edit invalidates exactly the families it generates.

## Front-end record read - the macro-context simulate share (ledgered 2026-09-08)

A warm `-jit` hello world spends 70 of its 88 ms in the compile, and the compile is the
module-cache read: 61 records deserialized (~10 ms of raw bytes) plus the macro-context
simulate of every macro module the program requires - typemacro_boost 4.6 ms, clargs 4.1,
templates_boost 3.5, quote 2.8, ast_boost 2.5, llvm_jit_link 4.7, some twenty modules in all
(`-log-compile-time`, the `cache read took ... -- macro simulate` lines). The emitter's own
context no longer loads on a DLL hit (`llvm_jit_link`), so what remains is daslib's. Separate
from the JIT and from the group/late-require work; not started. Also in the read:
`bindings/llvm_func` re-parses on every run (11 ms) - its `require dasbind` drifts the
builtin hash, so the record never serves ("reparsing in place").

Levers, unmeasured: a macro context that simulates lazily on the first macro call instead of
at record read; a cheaper simulate for a context whose program did not change (the record could
carry the simulated context's tables); a `[_macro]`-free module skipping the context entirely.

## Follow-ups from the require-group review (ledgered 2026-09-08)

- **A changed record resumes instead of cutting.** `trySerializeProgramModule` cuts the stream
  at a changed file ("file changed"), so every record after it - the emitter's ~40 modules a
  JIT miss appended to the script's stream - parses from source on the run after an edit to
  the script. The record header carries the payload size, so a changed file could be reparsed
  in place and the rest served, as the corrupt-record resume already does; the reason it cuts
  today is that a later module may depend on the changed one through a macro. A resume gated
  on "no later record lists this file among its macro dependencies" keeps the emitter warm.
- **`collectRequireNames` scans the source twice more per module** - the reader recomputes the
  collector's answer for every record whose hash matched, the writer for every fresh parse -
  and the reader's copy evaluates the guards (a deferred module load inside a cache read). The
  walk already ran the collector over the same file; the answer could ride the `ModuleInfo`.
- **`require_module_now` records ride the script's stream**, so a script that ever JIT-missed
  carries the emitter's records (~34 MB) in its own cache under the default-directory LRU. One
  emitter stream shared by every script would need a stream the environment can bind beside
  the host's, which is the persistent-serializer shape a host installs, not the CLI's.
- **The hit path binds `llvm/bindings/llvm_func`** (2320 `[extern]` lines, ~10 ms of the warm
  0.08 s) for three host queries - triple, CPU name, CPU features - in `jit_env_salt`. Three
  C++ binds in the `jit` module would drop the binding from the hit path.
- **The extern resolver looks every operator and intrinsic node up in the DLL** - the lowered
  ones answer "no such slot" after a mangled-name build and a `dlsym` miss - where the emitter's
  visitor skipped them by `has_intrinsic` / `isExprOp2_Func` before any of that.
- **The in-memory engine's finalizer runs through the emitter's macro context**
  (`free_jit_engine_in_emitter`), so a host whose context outlives module shutdown reaches a
  context that is gone; a C++ `free_jit_engine` in the `jit` module would not.
- **`requestJit` is still a bit on the shared `Function`** (`mark_jit_selection`), the shape
  the per-program tables removed for `used` and `index`; two programs JIT-ing in one process
  re-stamp each other's answer between plan and resolve.
- **A late require nested in a parse or a record read is never cached.** `requireModuleNow`
  hides the stream (`LateRequireEnvScope::hideStream`) when the bound program is compiling or
  the reader is inside a record, so a `[call_macro]`'s or a macro module's `[init]`'s late
  require parses from source on every run. The JIT's simulate-time require, the one that
  matters today, is between records and joins the stream; the nested shapes would need a
  reader that can suspend a record and a writer that can insert after the enclosing module's.
- **Nothing checks that a descriptor's `grp` rows agree with the module's C++ rows.** A
  static host gets the rows `registerModuleGroupMember` adds from the module constructor, a
  shared_module build the rows the descriptor's `register_module_group` recorded; the two are
  spelled by hand in two files (`dasLLVM.cpp` / `.das_module`, and the sibling pairs) and drift
  is a member one build has and the other lacks. A gate that loads each descriptor and diffs
  its `grp` rows against the constructor's registry would catch it.
