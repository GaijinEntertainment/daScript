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
daslang.exe -jit -module-cache .jitted_scripts/module_cache/<app>.dascache <app>.das \
    -- --jit-split-modules=-1 <app args>
```

One cache file per app, under `.jitted_scripts/` (already gitignored). Caveats that stay:

- **Bench t/s rows run the stock monolith invocation** until an A/B proves split-neutral -
  split loses cross-module inlining across partitions, so its artifact is not the shipped one.
- The first cache-backed run re-keys the JIT cache once (ledger item 1) - expect one cold
  codegen right after a cache write.
- QUIRK 21 (HOW_TO_ADD_A_FORMAT.md) still applies: macro-side emitter edits are invisible
  to every key - `rm -rf .jitted_scripts` after `dasllama_gemm_gen.das` changes (which also
  drops the module caches stored under it - correct, the AST changed).

## Invalidation ledger

1. **Deser-vs-parse AOT-hash divergence.** A `-module-cache` deserialized AST produces
   different per-function AOT hashes than the freshly parsed one, so the first run after a
   cache write re-keys the whole JIT DLL/obj cache (one wasted cold codegen); hashes are
   stable from then on. Done = the same program hashes identically parsed or deserialized,
   witnessed by a cache write followed by a JIT cache HIT.
2. **Split obj keys are a chained prefix fold - one edit re-emits the suffix.** Editing
   `dasllama_repack` re-emitted 72 of 99 partitions (18.3 s, nearly full cold): every module
   AFTER the edited one in program order re-keys, though their IR is unchanged. Done =
   per-module keys derived from the module's own (transitive-interface) hash, so a leaf edit
   re-emits ~itself; measured target: an early-chain kernel edit under ~5 s of codegen.
3. **The AST module cache is all-or-nothing under an edit.** The same repack edit paid the
   full 39.7 s front end and rewrote the whole 148 MB cache - no per-module reuse of the
   unedited prefix. Done = an edit re-parses the edited module + dependents only.
4. **Macro-emitter changes are invisible to every cache key** (QUIRK 21; QUIRK 15's unquirk
   note wants the generator hash folded into the sidecar identity too). Done = an emitter
   edit invalidates exactly the families it generates.
