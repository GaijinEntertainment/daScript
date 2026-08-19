# How dasLLAMA is measured

The numbers on [daslang.io/dasllama.html](https://daslang.io/dasllama.html) come from the
profiling rig in `modules/dasLLAMA/performance/`. This page states the method exactly — what is
measured, against what, under which rules — so any number on the site can be traced to a
command, a build, and a file hash. The rig source is in the repository; every claim here is
checkable against it.

## The pairing rule

A das measurement is published only next to a reference measurement of the same model, on the
same machine, in the same category, taken in the same session. If the reference is missing, the
das number does not ship. The site renders rows only for such pairs; there are no unpaired
numbers and no blanks to explain.

References run adjacent to the das cell they pair with — same session, minutes apart, same
thermal state — because interleaving is the only ordering that survives sustained-load drift.
One model process runs at a time, with a settle gap between passes so a dead process's page-cache
churn does not bleed into the next measurement.

## Categories

Three like-for-like comparisons, named by what both sides get:

- **cpu** — portable CPU kernels on both sides. llama.cpp is a *clean-cpu* build (no
  Accelerate/BLAS) at `-ngl 0`; das runs its portable tier. Kernel against kernel.
- **cpu + accel** — both sides get the box's accelerated math path (Accelerate on Apple; MKL or
  AOCL elsewhere). llama.cpp runs *stock* at `-ngl 0 -nopo 1`; `-nopo` stops its op-offload
  from quietly serving "CPU" rows on the GPU — without it, a stock build ships big-batch ops to
  Metal and the row is not a CPU number.
- **gpu** — each engine in its own GPU serving configuration. LLMs: both sides at `-ngl 99`.
  ASR: das runs `--ngl 1` (the f32-tower GPU serving mode + the Metal decoder), the reference
  CLI its own GPU default — whisper-cli keeps its flash-attention default there, while its cpu
  row runs `-nf`. Which backend ran is in the row's receipt.

A box without an accelerated tier simply shows fewer categories.

## What is timed

- **LLM**: llama-bench semantics and naming exactly — `pp512` (prompt processing, tok/s) and
  `tg128` (128-token greedy generation, tok/s), mean ± sample stdev over 5 timed repetitions
  after one untimed full-size warmup. das mirrors this protocol in
  `benchmarks/lcpp_bench.das`; the reference rows are llama-bench's own `-o json` output.
- **Speech-to-text / audio-in**: wall time to transcribe a clip, best of the timed repetitions,
  **engine-internal and load-excluded on every path** — das times `transcribe()` from a warmed
  process; whisper.cpp rows are its own `total time − load time`; the patched multimodal CLI
  reports an instrumented total that starts after load; the Python references time their own
  recognize calls. No process wall-clocks, no startup costs. There is no encode/decode split:
  total time is what a user experiences, and per-stage internals are not comparable across
  engines.
- **Dictation aggregate** (`librispeech25`): whole-set engine time over the 25-clip
  LibriSpeech set (test-clean 1089-134686, ~3.6 min of short utterances) — one number per
  side, never 25 rows. Scoped to the fastest model (Parakeet-TDT v3), paired against both
  references: das sums its per-clip best-of-reps times; parakeet-cli runs the whole set in
  ONE process (its per-file timing blocks accumulate `total time`, so the final
  `total − load` is the set's engine time, best of reps); ONNX Runtime times its recognize
  calls per clip in one amortized process and the per-clip bests sum. Load is excluded on
  every side, as everywhere.

Model load is excluded everywhere. Sweeps run on images the batch bakes itself (the lifecycle
wipe/bake/delete in `PROFILE.md`) so first-load conversion cost cannot leak into a timed cell; a >3% coefficient of variation on a das cell
triggers one warm re-run (page-cache churn from the previous process is the usual cause), and a
cell that stays noisy is kept with its dispersion visible, not smoothed.

## Threads and pinning

Every CPU row runs **min(16, physical performance cores)** threads — the same count on both
engines, pinned. One rule for every box: 8 on M1 Max, 10 on M4 Pro, 4 on M3 Air, 16 on the
big x86 parts. On x86 both sides pin to distinct physical cores (das through the job queue's
hard affinity mask, llama-bench via `--cpu-mask ... --cpu-strict 1`); unpinned, SMT placement
makes prefill bimodal. Apple boxes run the performance cores; there is no pin API, so the job
queue's mode 2 instead classifies das workers via QoS — the dasLLAMA darwin default —
because unclassified workers lose several percent of big-model decode under ambient load.

There is deliberately no per-engine thread detection and no thread matrix: probed-optimal
counts turn a kernel benchmark into a contest of who probes better, and no two boxes stay
comparable. If a reference ever measures faster at a lower count on some box, that goes in the
row's annotation — the policy does not move.

## Tuning

das kernels are auto-tuned per box by the `[tune]` framework before measuring; the tune stamp
(winning kernel variants and their source) is recorded in every das row. The reference runs its
shipped defaults. This is the comparison as intended: das's claim is that a JIT with per-box
tuning beats hand-written universal kernels — tuning is the mechanism, not an unfair
advantage; llama.cpp's own build-time dispatch (AVX tiers, i8mm paths) is likewise active on
its side.

No cross-box das binary is used, here or anywhere: `[tune]` bakes the winning kernel forms at
compile time, so a binary carries whichever box built it. Every das row is the `.das` source
compiled ON the box being measured — `daspkg release` mints that box's standalone bench
executable (its freshly tuned sidecar beside it), records run through that executable, and a
debug-flavored `-jit` script run is excluded from the record store by design (the two vehicles
measure equivalent; the release rail is the one with the identity stamps).

## Identity: what exactly ran, on what

Every row carries, machine-captured at run time — never typed by hand:

- **Engine identity**: das git commit and release; llama.cpp build commit; for ASR references,
  the tool's checkout commit (a dirty tree taints the identity loudly), the patch file's hash
  where a patched build is used, or the resolved Python package versions for the venv-based
  references.
- **Input identity**: every file the run read — weights, projection files, audio clips — with
  its SHA-256, computed once per box and cached in a sidecar that invalidates on any size or
  timestamp change. Two boxes claiming "the same" file are provably the same bytes, or provably
  not.
- **Environment**: the full set of engine environment variables in effect (the whole registry,
  not a curated subset — an unrecorded variable is how two different configs come to look like
  the same row), the exact command lines of both sides, and the auto-probed hardware block
  (chip, cores, RAM and its configuration, GPU, OS, power plan and SMT state where relevant).

The audio corpus is fully reproducible: the clips are whisper.cpp's own samples (or
deterministic concatenations of them, regenerated by script) plus a LibriSpeech test-clean
subset, all pinned by a committed SHA-256 manifest. A box whose fetch produces different bytes
fails the manifest check rather than silently benchmarking a different input.

## The record

Each run is one record, and das and reference runs are siblings in the same store — pairing and
ratios are derived when the site renders, never stored. The store lives at
`performance/records/<box>.json`; `performance/profile_common.das` is its authoritative reader
and writer (`BenchRun`, `read_bench_records` / `write_bench_records`), so every tool that
touches it goes through the same schema. Documents arriving from outside — community
submissions to the ladder service — are gated by `dasllama/dasllama_exchange_schema.das`, an
engine-free validator of the same record shape plus the tune-sidecar form;
`tests/test_exchange_schema.das` keeps it pinned to what the writers emit.

- **`flavor`** names what that engine got, so two rows from one box can never be confused: das
  is `tuned` or `accel` (the +AMX tier over the tuned stack); a reference row carries its build
  flavor (`clean-cpu`, `stock`, `metal`).
- **`tune`** is required on das rows. Per-box tuning makes two identical machines legitimately
  differ, so a das number without its tune stamp is not reproducible and does not ship.
- **`noise`** copies the mint's noise verdict out of the sidecar provenance (`ok` or
  `overridden`): a row measured under a noise-overridden mint says so on the board, not only
  inside the sidecar.
- **`parity`** (`ok`) rides das text-LLM rows: before a model's first cell, the sweep runs the
  model's frozen pinned-greedy parity fixture (`performance/model_specs.das` evidence — the
  same one the test suite gates) and refuses every cell on a mismatch, so a board number can
  never come from an engine that decodes garbage on that box. The pregate prints both decoded
  texts into the sweep log; only the verdict is stored. ASR rows carry their transcript
  (`text`) instead — the eyeball is the row itself.
- **`hardware`** is auto-probed on the box, never typed in — it is the block a reader has to be
  able to trust.
- **`dasllama_version`** rides das rows only: dasLLAMA's own release counter
  (`dasllama/dasllama_version.das`), bumped by any kernel work, decoupled from the daslang
  version in `version`. Sidecar provenance carries the same counter, which is what lets a
  sidecar's validity be judged as (version, box) without opening it.
- **`source`** is `official` or `community`.
- The identity is **`(gguf, box, engine, backend, flavor, workload)`**. A re-measure replaces
  its row in place with a fresh date and commit; git history is the archive. There are no
  superseded copies, ever — which is why an annotation cannot live in the sweep output.

## Anomalies

When a number needs explaining, the explanation is on the row — a hand-written annotation
stored outside the sweep output (re-profiling can never wipe it), rendered with the row and its
receipt. That includes cases where the reference was helped (pinning it away from a bad
scheduler decision), cases where a reference misbehaved (runaway generation on a clip), and
configurations still in active development. Measured rows are annotated, not deleted.

## What is not claimed

- ASR rows split into the same cpu / cpu + accel / gpu categories as the LLMs (the ASR
  accel lever is `flavor: accel` on a cpu row, like the LLM rows), and the macOS reference CLIs get Apple's AMX
  matrix unit through Accelerate in every CPU mode — noted where it applies. llama.cpp has
  no speech-to-text engine; those references are the dedicated ones (whisper.cpp,
  parakeet-cli, ONNX Runtime, NeMo), and the gpu category exists where the reference has a
  GPU mode (whisper.cpp on Metal).
- The audio-in comparison stays a CPU slice: llama.cpp's audio encoders do not route through
  BLAS on CPU, and mtmd GPU builds are not measured here.
- Community-submitted rows are out of scope for now; the record schema carries a source field
  for them, but no submission path is defined yet.

## Reproducing

`modules/dasLLAMA/BRINGUP.md` is the box checklist: build, models, reference
engines (scripted, pinned commits, locked venvs), corpus fetch with manifest verification,
sweep (the board bakes and deletes its own images). The sweep writes a per-box record store after every cell; the site renders
from the merged stores. Every tool narrates its progress on stdout as it runs.
