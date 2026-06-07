# flatten-fuzz

A metamorphic differential fuzzer for [`daslib/flatten`](../../daslib/flatten.das).

It generates random daslang functions, compiles them **in-process** (the
integrated compiler — no subprocess), and checks the flattened twin against the
pristine original over many integer inputs:

```
f(x) == f_flat(x)   for all generated f, for all x
```

`[flatten] def f` leaves `f` untouched (real control flow) and emits the
branchless `f_flat` twin, so the un-flattened `f` is a **free, independent ground
truth** that never trusts flatten. The fuzzer is, in effect, an auto-generated,
randomized, at-scale [`tests/flatten/test_flatten_basic.das`](../../tests/flatten/test_flatten_basic.das).

## Run

```
daslang utils/flatten-fuzz/main.das -- [flags]
```

| flag | meaning |
|---|---|
| `-s, --seed N` | base RNG seed (default 1) |
| `-u, --units N` | function pairs per batch (default 20) |
| `-n, --inputs N` | random inputs per unit (default 40) |
| `-d, --depth N` | max nesting / expression depth (default 3) |
| `-b, --batches N` | run seeds `seed .. seed+N-1` (default 1) |
| `-f, --strict-fold` | enable the `strict_fold` structural oracle + const-density bias |
| `-k, --keep` | keep the generated temp source (don't delete) |
| `-L, --log-infer` | emit `options log_infer_passes` in generated source (dump the compiler's per-infer-pass AST) |
| `-?, --show-help` | help |

`--log-infer` makes the fuzzer a probe for the **compiler** too, not just flatten:
the per-pass AST dump exposes infer non-convergence / oscillation on
flatten-generated code. The in-process compiler routes that log into the
compile-result string (not stdout), so the tool surfaces it explicitly.

Exit code is non-zero if any batch fails. On a mismatch or compile failure the
offending unit is bisected out and dumped as a standalone reproducer
(`flatten_fuzz_fail_seed<S>_unit<N>.das`).

## Two oracles

- **Value-differential** (default) — `f(x) == f_flat(x)`. Catches semantic
  divergence in the mask machinery (early-return / break / continue / nested
  loops / inlining).
- **Structural** (`--strict-fold`) — compiles the twins with
  `[flatten(strict_fold=true)]` and biases the grammar toward constants, so a
  *missed* constant fold becomes a compile error. This is the "did the fold
  fire" half that value equivalence can't see.

## Design

- **Integer domain only** — the differential is then *exact* (no float epsilon).
- **Trap-free** — no `/`, `%`, or array indexing; shift counts masked to `& 31`.
  Predication executes *both* arms, so a trapping op in an untaken branch would
  panic in the twin but not the original — an inherent property of predication,
  not a flatten bug (and the shader backends flatten targets are trap-free
  anyway).
- **Mask-biased grammar** — if/elif/else, early return, `range`/const-array
  loops, break/continue, nested loops, helper inlining, compound assignment.
  That combinatorial surface is where flatten bugs live.
- **Pure-functional** — params + locals + return value; no globals (yet).

## Found

- The inlined-callee-local shadowing bug (`inline_call` not uniquifying callee
  locals across multiple inline sites) — fixed in the same change that added
  this tool.
- At `--depth 4+`, deeply-nested constant `for` loops unroll without any
  expansion cap, so the flattened twin grows exponentially → very slow compiles
  and a heap-layout-dependent SIGSEGV during the macro. Diagnosis: `lower_for`
  has no unroll-size guard; the flatten opt passes are bounded but super-linear
  in body size. The compiler's own `max_infer_passes` (default 50) bounds infer,
  so this is blowup, not an infinite loop. Default depth is 3 (clean at scale).
