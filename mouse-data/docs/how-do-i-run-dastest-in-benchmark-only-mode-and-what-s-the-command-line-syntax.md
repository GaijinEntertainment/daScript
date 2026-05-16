---
slug: how-do-i-run-dastest-in-benchmark-only-mode-and-what-s-the-command-line-syntax
title: How do I run dastest in benchmark-only mode and what's the command-line syntax?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Benchmarks are functions annotated with `[benchmark]` from `dastest/testing_boost.das`. Run them via the dastest harness with `--bench`:

```bash
# All benchmarks in a directory (skip the regular tests)
./bin/daslang dastest/dastest.das -- --bench --test benchmarks/sql --test-names none

# Just one file
./bin/daslang dastest/dastest.das -- --bench --test benchmarks/sql/count_aggregate.das --test-names none

# Filter by [benchmark] function-name prefix (substring match on the function name)
./bin/daslang dastest/dastest.das -- --bench --bench-names sum_ --test benchmarks/sql --test-names none

# Collect N samples for variance / averaging
./bin/daslang dastest/dastest.das -- --bench --test benchmarks/sql/count_aggregate.das --test-names none --count 5
```

Key flags:
- `--bench` — enable benchmark execution
- `--test <path>` — folder or single file (NOT positional)
- `--test-names none` — skip regular `[test]` discovery (benchmarks only)
- `--bench-names <prefix>` — filter benchmarks by function-name prefix
- `--bench-format <native|go|json>` — output format
- `--count <n>` — repeat all benchmarks N times

Benchmarks only run after all module **tests** have passed; that's why `--test-names none` is the canonical "skip tests, run benchmarks" combo.

Output is `<sub_name> N ns/op <bytes>/op <allocs>/op <SB>/op <strings>/op`. If the benchmark `b |> run(name, chunk_size, op)` form passes a chunk_size (typically the dataset size), the displayed ns/op is **divided by that chunk_size** — i.e. per-element time, not per-op-call time. Sub-nanosecond results (`0 ns/op`) usually mean early-exit hit the answer in O(1) regardless of dataset size.

Reference: `dastest/README.md` and `dastest/dastest_clargs.das`.

## Questions
- How do I run dastest in benchmark-only mode and what's the command-line syntax?
- What's the dastest --bench command line?
- How do I filter dastest benchmarks by name?

## Questions
- How do I run dastest in benchmark-only mode and what's the command-line syntax?
