# THIS IS HOW TO PROFILE, NOTHING ELSE

Two rigs, below. Both spawn `benchmarks/lcpp_bench.das` once per cell — that is the only thing
that measures dasLLAMA performance, and no second harness gets written.

---

## Before either: the box manifest must be newer than the binary

A manifest older than `bin/daslang` fails every oracle cell, and an untuned run re-execs into a
full retune instead of measuring. Re-minting takes a couple of minutes:

```sh
DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/<box>.tune.json \
  bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das
```

Then check the fresh winners against the stored rows' `tune` stamps before trusting any delta —
a manifest that picked different winners moves the numbers on its own.

---

## 1. The oracle — after every code change

Re-measures this box's stored rows and gates each against its recorded mean. llama.cpp never
runs, the store is never written, artifacts are frozen (a missing `.dlim` panics rather than
minting).

```sh
bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --oracle --legs metal
```

- Stops at the first FAIL; `--oracle-keep-going` runs the whole board.
- `-o <substring>` narrows to one model.
- FAIL past 5% below the stored mean, WARN past 3% (`--oracle-fail` / `--oracle-warn`). Gains
  report, and a gain past the fail bar is flagged suspicious — verify it.
- A FAILED cell automatically re-runs solo after a settle and *that* verdict stands: a
  board-tail cell reads low on a hot chip.
- Exit is nonzero on any FAIL.

---

## 2. The profiling rig — to publish new numbers

Measures both engines and writes `performance/records/<box>.json`.

```sh
bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --legs metal \
  --ref-clean <llama-bench clean-cpu build> --ref-stock <llama-bench stock build>
```

- Reference binaries also come from `LLAMA_BENCH_CLEAN` / `LLAMA_BENCH_STOCK`.
- `--legs cpu|metal|all`, `-w llm|asr|all`, `-o <substring>` to narrow.
- `-p 512 -n 128 -r 5` are the recorded shapes. Changing them makes the row incomparable to
  every other row in the store.
- `--settle <seconds>` (default 12) idles between passes; a dead child's multi-GB map reclaims
  asynchronously and short cells otherwise bench into that churn.

Then merge the per-box stores into the file the site renders:

```sh
bin/daslang modules/dasLLAMA/performance/gen_site_records.das
```
