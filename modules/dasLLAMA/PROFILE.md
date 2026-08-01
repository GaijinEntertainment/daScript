# THIS IS HOW TO PROFILE, NOTHING ELSE

Two rigs, below. Both spawn `benchmarks/lcpp_bench.das` once per cell — that is the only thing
that measures dasLLAMA performance, and no second harness gets written.

---

## Before either: build the bench exe

Both rigs measure by spawning **the released `lcpp_bench` executable**, never a script. Its tune
winners are baked in at build time, so no cell can tune, re-exec, or drift onto different winners
mid-sweep. Build it once per box, and again whenever `benchmarks/lcpp_bench.das` changes:

```sh
bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks \
  --out modules/dasLLAMA/performance/_rig
```

`daspkg release` tunes any incomplete scope, rebuilds so the exe bakes the winners, and ships the
sidecar beside it. A missing exe — or one older than the bench source — is a hard stop with the
build line printed; the rig never rebuilds silently, because that would put minutes of hidden work
inside a measurement run.

**Do NOT set `DAS_TUNE_MANIFEST`.** The exe carries its own winners, and the rig actively clears an
inherited value. Pointing it at a shared box manifest re-opens exactly what the exe replaced: the
manifest is older than a freshly built exe, the staleness rule drops every kernel to fallback, and
the cell measures nothing real.

## 1. The oracle — after every code change

Re-measures this box's stored rows and gates each against its recorded mean. llama.cpp never
runs, the store is never written, artifacts are frozen (a missing `.dlim` panics rather than
minting).

```sh
DASLLAMA_BOX=<box> bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --oracle --legs metal
```

- Stops at the first FAIL; `--oracle-keep-going` runs the whole board.
- `-o <substring>` narrows to one model.
- FAIL past 5% below the stored mean, WARN past 3% (`--oracle-fail` / `--oracle-warn`). Gains
  report, and a gain past the fail bar is flagged suspicious — verify it.
- A FAILED cell automatically re-runs solo after a settle and *that* verdict stands: a
  board-tail cell reads low on a hot chip.
- Rebuild the bench exe before trusting a delta after any `lcpp_bench.das` change — the rig
  refuses a stale one rather than measuring it.
- Exit is nonzero on any FAIL.

---

## 2. The profiling rig — to publish new numbers

Measures both engines and writes `performance/records/<box>.json`.

```sh
DASLLAMA_BOX=<box> bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --legs metal \
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
