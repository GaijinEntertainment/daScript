# THIS IS HOW TO PROFILE, NOTHING ELSE

Three rigs, below, and which one you want depends on the model size:

| rig | models | measures against | when |
|---|---|---|---|
| **1. small tier** — `gen_profile.das -t small` | the catalog under 6 GiB | `profile_llm_<box>.json` + committed `baseline_llm_<box>.tsv` | **the routine check after a code change** |
| **2. the oracle** — `gen_bench_records.das --oracle` | the published board (every stored row, 5-23 GB) | `records/<box>.json` stored means | before a PR, or when a big-model path changed |
| **3. the publishing rig** — `gen_bench_records.das` | the published board, both engines | writes `records/<box>.json` | only to publish new numbers |

Rigs 2 and 3 spawn `benchmarks/lcpp_bench.das` once per cell — that is the only thing that measures
board performance, and no second harness gets written.

**Reach for rig 1 first.** "Run the oracle" on the big board costs tens of minutes per box; the
small tier answers the same question — did this change cost performance — in a fraction of it.

---

## Rig 1 — the small tier (the routine check)

Measures prefill and emission tok/s for every catalog model under 6 GiB that exists on this box,
merges each row into `performance/profile_llm_<box>.json`, and pulls the llama.cpp column from the
committed `baseline_llm_<box>.tsv` (never re-measured here).

```sh
DASLLAMA_BOX=<box> bin/daslang -jit modules/dasLLAMA/performance/gen_profile.das -- -t small
```

- `-t small | large | all` picks the tier; the 6 GiB split is `LARGE_TIER_BYTES` in
  `profile_common.das`. Absent models skip with a warning, so the catalog can list every box's.
- `-o <substring>` narrows to one display name.
- Rows merge update-or-insert, so a small run now and a large run later share one file without
  clobbering each other.
- **`-jit` is required** — this rig runs the engine in-process rather than spawning the bench exe.

---

## Rigs 2 and 3 — first, build the bench exe

Both rigs measure by spawning **the released `lcpp_bench` executable**, never a script. Its tune
winners are baked in at build time, so no cell can tune, re-exec, or drift onto different winners
mid-sweep. Build it once per box, and again whenever `benchmarks/lcpp_bench.das` changes:

```sh
bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks \
  --out modules/dasLLAMA/performance/_rig
```

`daspkg release` tunes any incomplete scope, rebuilds so the exe bakes the winners, and ships the
sidecar beside it — which is also the exe's provenance: the tune gate checks that shipped sidecar,
because `tune_status()` is empty by design in a standalone exe (the winners are compiled in, so the
policy rail never runs). A missing exe — or one older than ANY dasLLAMA source it bakes — is a hard
stop with the build line printed; the rig never rebuilds silently, because that would put minutes of
hidden work inside a measurement run. Measured (old paranoid protocol): 886 s from cold, `--quick` 85 s.

**Do NOT set `DAS_TUNE_MANIFEST` for these rigs.** The exe carries its own winners, and the rig
actively clears an inherited value. Pointing it at a shared box manifest re-opens exactly what the
exe replaced: the manifest is older than a freshly built exe, the staleness rule drops every kernel
to fallback, and the cell measures nothing real. It also pins the ORCHESTRATOR to winners its cells
do not use, so the two disagree about every `.dlim` identity.

**The converter is the opposite case** — `BRINGUP.md` §0 exports the manifest precisely so a
pre-bake lands on the same winners the exe carries. Export it for the bake, leave it out of the
sweep.

**Image lifecycle: both rigs own the model dirs for the whole batch.** The batch starts by
deleting every `.dlim`; a model's images bake when its first cell needs them (rig 3's cv
warm-retry absorbs the cold map, the oracle's prepare pass bakes before the timed cell) and are
deleted after its last cell. Peak disk is one model's lanes, never the catalog's — a standing
two-lane working set once filled a 460 GB box to 97% and killed a bake mid-sweep. Re-profiling
pays a re-bake per model; a fresh box pays nothing extra, which is the case rented hardware
cares about.

## Rig 2 — the oracle (the published board)

Re-measures this box's stored rows and gates each against its recorded mean. llama.cpp never
runs, the store is never written, and the timed cell never mints — a prepare pass bakes and
warms each cell's image first, and `tune_sha` carries the pin-set proof.

```sh
DASLLAMA_BOX=<box> bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --oracle --legs metal
```

- Stops at the first FAIL; `--oracle-keep-going` runs the whole board.
- `-o <substring>` narrows to one model.
- FAIL past 5% below the stored mean, WARN past 3% (`--oracle-fail` / `--oracle-warn`). Gains
  report, and a gain past the fail bar is flagged suspicious — verify it.
- A FAILED cell automatically re-runs solo after a settle (default 180 s — thermal recovery
  takes ~3 minutes) and *that* verdict stands: a board-tail cell reads low on a hot chip.
- Rebuild the bench exe before trusting a delta after any `lcpp_bench.das` change — the rig
  refuses a stale one rather than measuring it.
- Exit is nonzero on any FAIL.

---

## Rig 3 — the publishing rig

Measures both engines and writes `performance/records/<box>.json`.

```sh
DASLLAMA_BOX=<box> bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --legs metal \
  --ref-clean <llama-bench clean-cpu build> --ref-stock <llama-bench stock build>
```

- Reference binaries also come from `LLAMA_BENCH_CLEAN` / `LLAMA_BENCH_STOCK`.
- `--legs` takes a comma list of `cpu` (= neon + amx) / `neon` / `amx` / `metal`; there is no
  `all`. The full Apple board is `--legs cpu,metal`. `-w llm|asr|all`, `-o <substring>` to narrow.
- `-p 512 -n 128 -r 5` are the recorded shapes. Changing them makes the row incomparable to
  every other row in the store.
- `--settle <seconds>` (default 12) idles between passes; a dead child's multi-GB map reclaims
  asynchronously and short cells otherwise bench into that churn.
- `--das-settle <seconds>` (default 180) idles before every das cell — a different mechanism
  than `--settle`. das tuned kernels run near the package power ceiling; on a heat-soaked box
  they under-read with a CLEAN cv (gpt-oss tuned pp512 read −13.6% mid-board; 180 s of idle
  restored it; llama.cpp refs never moved). The cv retry cannot catch a stable-low cell — only
  cool-slot entry can. Refs get no cool slot on purpose: they are insensitive, and skipping it
  saves hours.

Then merge the per-box stores into the file the site renders:

```sh
bin/daslang modules/dasLLAMA/performance/gen_site_records.das
```

---

## The image cell (`--image`)

The image TURN on a vision decoder — what a user actually waits on when they attach a picture.
One process, one tune-key demand, one image-identity stamp, same as the ASR cell:

```sh
modules/dasLLAMA/performance/_rig/dasllama-bench.app/Contents/MacOS/dasllama-bench \
  -m <decoder.gguf> --image-mmproj <mmproj.gguf> --image <picture.jpg> -r 5
```

The released exe (built above) is the protocol — its winners are baked, so the cell cannot
tune or drift mid-run. The `-jit` script form with `--for-debug-purposes` is a debug
instrument only: its rows stamp `debug-jit` and never reach a board or a doc.

- Three keys per row. `img:enc` is the embedder alone in ms (best of `-r`), `img:pp` the SPLICED
  prefill in tok/s (head tokens + soft-token rows + tail tokens — every position the turn holds),
  `img:tg` the decode in tok/s, carrying the reply text.
- The reply rides the same stop protocol `respond` runs; without it a stray channel marker lands in
  the timed text and the row prices a reply the product would never have shown.
- `workload = "image-chat"` on the record, beside `asr` and `audio-chat`. The picture is pinned by
  content hash in `files`, like the weights and the mmproj.
- Three arms, two flags: without `--ngl` the cell runs all-CPU by intent
  (`allow_cpu_prefill()`, the tower driver pinned off and announced, `backend = "cpu"`); with
  `--accel` it is the CPU row with the accelerate float-batch tier armed (`flavor = "accel"`;
  a gemma4v tower then serves the file's planes through it instead of its q8 lane; refused
  with `--ngl`); with `--ngl` it loads the metal-blob flavor and the whole turn — head, span
  (`AttnArgs.uend`), tail, decode — serves on Metal (`backend = "metal"`), with the tripwire
  ARMED so a CPU fallback reds the row instead of sandbagging it. The Vulkan span is still
  declined (`followup_general.md` #23's other half).
- The embedder ALONE is a kernel question, not a board row: price it in the kernel A/B lab, not here.

---

## The ASR cell (`--asr`)

Transcription wall time per corpus clip — what a user waits on when they feed audio. Same
process/tune/identity discipline as the cells above; the reference tools are measured
adjacent by `gen_bench_records`, never in this process.

```sh
modules/dasLLAMA/performance/_rig/dasllama-bench.app/Contents/MacOS/dasllama-bench --asr -m "Whisper large" -r 3          # the q8-CPU serving default
modules/dasLLAMA/performance/_rig/dasllama-bench.app/Contents/MacOS/dasllama-bench --asr -m "Whisper large" --ngl 1 -r 3  # the f32 Metal tower rail
```

Same executable rule as the image cell: the released exe, never the `-jit` script —
`--for-debug-purposes` rows are debug instruments, not measurements.

- One `asr:<clip>.wav` key per corpus bucket: best-of-`-r` transcribe ms, the clip seconds,
  the LAST rep's transcript (under fast-math a token flip is what moves the timing), and
  `encode_ms` (the encoder split, sampled in its own untimed rep off the asr_prof rail).
- Three legs: the plain row pins BOTH GPU drivers off (`set_metal_wdec(false)` +
  `set_metal_tower(false)`, `backend = "cpu"` — a CPU row means CPU); `--accel` adds the
  Accelerate tier (`backend = "accel"`); `--ngl` serves the f32 tower on Metal
  (`set_asr_tower_fp32` — the tower half only, the decoder stays q8) plus the whisper
  decoder driver, all four knobs pinned on (`backend = "metal"`). A whisper gpu row must
  show tower-engage counters, a wdec windows delta, and a non-zero `encode_ms` or the leg
  exits non-zero. A Conformer row under `--ngl` therefore FAILS by design: that family has
  no GPU tower lane to measure.
- `exec_fmt` on the row states the quant mode the encoder actually ran, so a number can
  never silently describe a format nobody serves.

## The tokenizer cell (`--tok`)

Encode/decode throughput of a GGUF's tokenizer, from the same rig — no weights are loaded, so the
tiny `ggml-vocab-*.gguf` fixtures work as `-m`:

```sh
bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- --tok -m <model.gguf> --for-debug-purposes
```

- Five corpora (prose, code, cjk, digits, longword) x a 4x size ladder up to `--tok-max-kb`
  (default 1024). The instrument is the printed per-step **scaling ratio** (linear = 4x time per
  step, quadratic = 16x); the ladder stops itself past a time budget instead of sitting inside a
  quadratic encode. `--tok-corpus` narrows corpora; `--prompts` swaps the prose seed file.
- `tok enc` rows are MB/s of input text; `tok dec` is the full-sequence decode; `tok piece` is the
  streaming per-token path. Every corpus round-trip-gates (`decode(encode(x)) == x`) before timing.
- `--ref <llama-tokenize>` adds a llama.cpp encode estimate on the same bytes and vocab (process
  wall minus a 1-byte baseline — an estimate; both raw walls print beside it).
- Single-threaded by nature, txt output only, no records rail: a decision instrument for tokenizer
  work, not a board row.

---

## The tune generation — session cadence

Every measurement above runs under a sidecar GENERATION, and the rails enforce it:

- **Session start / external-box profiling:** mint as part of building the exe —
  `daspkg release ...`. Release ALWAYS mints; the noise probes stamp the box's cv (only a
  busy box refuses), validation re-races every changed kernel against its fallback and
  demotes a winner that lost its margin, the previous sidecar snapshots to `.bak` with a
  printed DIFF, and every mint archives to `~/.tune-history/<box>/`.
- **Iteration rebuilds within a session:** `daspkg release ... --quick` — the ONLY path that
  inherits, and only a complete fresh sidecar (incomplete or stale still mints). Forgetting
  `--quick` costs one normal re-mint, never correctness.
- **Collect and oracle are different scenarios.** Publishing (rig 3) wants a fresh
  generation; the oracle (rig 2) verifies against the STORED rows' generation — rows carry
  `tune_sha`, a mismatch is `INCOMPARABLE` by default (`--oracle-allow-crossgen` forces,
  legacy rows warn), and each generation's full doc sits beside the store as
  `records/<box>.tune.<sha12>.json`. After a deliberate re-mint, re-record the rows.
- **Rig 1 is a relative instrument** (`-jit`, in-process): same-session comparisons only. The
  board rigs' bench refuses `-jit` script runs outright unless `--for-debug-purposes`, and
  stamps such output `debug-jit` — it can never enter records.
