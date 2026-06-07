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
- Unbounded loop unrolling: deeply-nested constant `for` loops unroll
  multiplicatively (`range(20)^4` = 160k body copies) with no expansion cap, so
  the flattened twin blows the heap to multiple GB. **Fixed** — `lower_for` now
  caps the product of nested iteration counts (`FLATTEN_MAX_UNROLL`, default 4096;
  `[flatten(max_unroll=N)]` overrides), bailing with a clean error before cloning.
- A deterministic compiler crash (the SIGSEGV/SIGBUS whose faulting address was
  always ASCII bytes of a flatten-generated name like `__flat_loop` / `v46_0`).
  Root cause was **not** in flatten: an incomplete 64-bit migration of the core
  shoe allocator (`MemoryModel::Deck`) — `total * size` (entry count × element
  size) overflowed `uint32_t` once a string-heap size-class grew past 4 GB, so
  `das_aligned_alloc16(0)` returned a degenerate pointer the next string
  overwrote. **Fixed** by widening the allocator's byte/count fields to 64-bit
  (regression: `tests-cpp/big/memory_model_4gb`). flatten amplified it via a
  per-`ExprVar` `"{name}"` string built inside the O(n²) opt-pass tree walks;
  that churn is now allocation-free (`das_string` compares). Default depth is 3.
