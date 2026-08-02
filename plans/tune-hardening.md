# Tune hardening — the sidecar stops being a random-number generator

The incident: winners raced on a non-silent box are noise, the sidecar is a FILE, so one noisy
mint persisted for days and masqueraded as a code regression (exe and `-jit` each read whatever
sidecar sits beside them; `daspkg release` inherited silently). Exposure scales with tuned-kernel
coverage: barely visible on Apple boxes (hand-rolled sdot decode), catastrophic on x64 (the
generated GEMM tier is all tuned). Diagnosing zen2 required manually killing every app — the
noise was unobservable until it ruined a measurement. This plan makes the tuner the detector.

**The mental model, corrected (Boris, 2026-08-02):**
- `.exe` builds MINT, always. Quick mode is the ONLY path that accepts an existing sidecar, it is
  explicit and by hand, and forgetting it costs a 2-minute re-mint, never correctness.
- Full-blown "collect results on the box" and "oracle on the box" are DIFFERENT scenarios, and
  BRINGUP.md / PROFILE.md are their two documents.
- Session cadence: paranoid mint at session start (and always for external-box profiling) →
  `--quick` by hand for iteration rebuilds → forgiving normal default when forgotten.

## Deliverables

### 1. The tuner (`modules/dasLLAMA/harness/tune_kernels.das`, `modules/dasLLVM/daslib/llvm_tune.das`)

- **1a. Noise-floor gate.** Before the grid: race the fixed baseline perm (`vec8_u2` dot, same
  N/REPS as a real round) ~12 rounds, compute cv. Above threshold → refuse to tune, exit nonzero,
  print what was measured. Thresholds start at normal ≤2% / paranoid ≤1%, confirmed empirically
  on the quiet M1 and zen2 during rollout. Escape env stamps the sidecar `noise: overridden`.
- **1b. Re-probes.** Every few kernels and at the end; a noisy end-probe = the sidecar is NOT
  written. The probe cv prints even when passing — drift becomes visible across history.
- **1c. Statistics.** Rank finalists by MEDIAN of finalist rounds (best-of still printed).
  Per-kernel min-vs-median divergence is a per-kernel noise flag. A winner must beat the shipped
  fallback by more than the measured noise floor or the FALLBACK keeps the seat — deterministic
  beats lottery. (ES/confidence-interval refinements welcome under the same shape.)
- **1d. Validation phase.** After winners are picked, re-race a known heavy subset twice: winners
  must reproduce and times must land in bound, else the MINT FAILS as a whole. Self-validation,
  not self-certification.
- **1e. Paranoid mode.** ~3× rounds (≈6 min, not 2), stricter thresholds, validation mandatory.
- **1f. Missing entries are fatal.** An incomplete sidecar fails `-jit` runs AND exe builds. No
  silent fallback winners — partial data is no data; the only fix is a re-mint.

### 2. Sidecar provenance + history

- **2a.** The sidecar doc gains provenance: date, commit sha (repo; `unknown` in SDK), mode,
  rounds, measured noise cv, per-winner median ms, validation verdict. Data is cheap — save it.
- **2b.** Every mint archives to `~/.tune-history/<box>/` (timestamped, failed ones kept and
  marked FAILED). Never deleted, searchable. The history is the box's longitudinal health record.
- **2c.** Re-mint renames the old sidecar `.bak` and prints the DIFF (winners + times). Uniform
  time shift with same winners = box state; scattered winner flips = one of the mints was noisy.

### 3. `daspkg release` inversion (`utils/daspkg/commands.das`)

- **3a.** Release ALWAYS mints (normal by default, `--paranoid` for bringup/session start). An
  existing sidecar is accepted ONLY under explicit `--quick`.
- **3b.** Release prints the provenance of whatever it ships.

### 4. `lcpp_bench` guards (`modules/dasLLAMA/benchmarks/lcpp_bench.das`)

- **4a.** Refuses to run under `-jit` unless `--for-debug-purposes`; debug output is stamped
  debug and never accepted into records. Measurement exists only from the released exe.
- **4b.** On PC the bench SETS affinity (forced) and jobque configuration itself and fails if it
  cannot; external env vars are overrides, not prerequisites. 16 jobs max, with the pool-size vs
  threads-used distinction explicit. Never guess whether the settings are right.

### 5. Records + oracle provenance (`performance/gen_bench_records.das`)

- **5a.** `records/<box>.json` embeds the full sidecar doc + its shasum + engine sha; rows
  reference it.
- **5b.** The oracle DECLINES a comparison across sidecar generations (sha mismatch):
  `INCOMPARABLE (sidecar generation changed)`, nonzero exit; `--oracle-allow-crossgen` forces.
  Legacy rows without provenance warn. The week's trap becomes a one-line decline.

### 6. The two documents

- **6a. BRINGUP.md rewritten** around the sequence for a box with existing data: (1) delete all
  `.dlim`, (2) paranoid mint as part of building the exe (`.bak` + logged diff), (3) oracle set —
  LLM and audio — plus one big model (fastest known-good) on CPU, human+AI review, anything out
  of the ordinary = stop and discuss. New box adds (4): the same big model vs llama.cpp — the
  RATIO is the known quantity when absolute numbers have no history.
- **6b. PROFILE.md** gets the session cadence (paranoid start → `--quick` iteration → forgiving
  default) and the collect-vs-oracle scenario split.

## Sequencing

1c+1a first (the math and the gate — everything else reports what these measure) → 1d+1e →
2 (provenance/history/diff) → 3 (release inversion) → 4 (bench guards) → 5 (records/oracle) →
6 (docs last, describing what exists). `skills/llvm_tune.md` and the env-registry entries update
with the pieces that touch them; llvm_tune is SDK-shipped, so its skill edits mind the shipping
gate.

## Non-goals

No perm-grid redesign, no `[tune]` macro surface changes, no per-kernel adaptive budgets
(ledger if wanted later).
