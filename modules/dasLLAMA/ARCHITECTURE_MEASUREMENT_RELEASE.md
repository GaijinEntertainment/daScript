# dasLLAMA Architecture - the board's vocabulary and its release boundary

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries the terms the measurement checklist (`REVIEW_MEASUREMENT.md`) reads a figure, a run and the
board with (`#measurement-vocabulary`), and where a between-release kernel change's board rows land
(`#board-rows-release-boundary`).

### The measurement checklist's vocabulary {#measurement-vocabulary}

`REVIEW_MEASUREMENT.md` defines each term in place, and its rules apply them as written there; each bullet here
states the same boundary.

- **Figure** - any number a run produced: a rate, a wall, a count, a byte size, or a ratio or
  percentage of two such numbers, produced by the runs its sides came from. An error or agreement
  reading (a cosine, a max-abs error) is a figure when a test's tolerance, or the bound an
  instrument's bounded-difference compare checks against, is set from it, or when a change cites it
  as evidence that a tolerance set for another cell, model or tier holds for the one it adds or
  alters. A test suite's pass, fail and skip tally, the count of kernels a compare of two dumps of
  emitted kernel code finds identical or different, and a `provenance` value read in its
  `performance/defaults/<class>.tune-defaults.json` profile or tune sidecar are not figures; that
  value copied into other text is one.
- **Served turn** - one request a serving path answers end to end: a chat or completion turn, or a
  facade `transcribe` / `synthesize` / `translate` call, whether or not an app runs it as one stage
  of a larger turn.
- **Served-turn leg** - a chat or completion turn's prefill or token decode, a batched decode row,
  or a whole served turn. The wall of one stage - one model component's forward pass or decode
  inside a facade call, or one decoder block - is not a leg.
- **Instrument** - a file whose printed or returned result is a wall or rate of a run it drove,
  read by its own clock or by timestamps a GPU driver returns, wherever the file sits. A file
  reading a spawned child process's clock, and a report a serving path prints only under a
  profiling switch, are not instruments.
- **Harness** - the `harness/` script that drove a run, where one did.
- **Route** - the end-to-end code path a served turn takes. The default route is the one a run
  given no command-line arguments and no environment overrides takes.
- **Flags of a serving run** - the tier (`-jit` or not), the `DAS_TUNE_POLICY` value in force, the
  CPU kernel backend the registry served (the one `DASLLAMA_PIN_BACKEND` pinned, or, with it unset,
  the one the auto-selection picked), the GPU backend where one served with its device, on Vulkan
  the coopmat mode (`DASLLAMA_COOPMAT`), and on CUDA the prefill GEMM form (`DASLLAMA_CUDA_MMQ`) and
  whether a launch overlaps the tail of the one before it (`DASLLAMA_CUDA_PDL`). A run that serves a
  model through the engine has every flag of a serving run, whatever drives it.
- **Flags of an instrument run** - the tier, the `DAS_TUNE_POLICY` value, the device,
  `DASLLAMA_COOPMAT` on Vulkan or `DASLLAMA_CUDA_MMQ` and `DASLLAMA_CUDA_PDL` on CUDA, and the
  instrument's arguments.
- **Flags of a test run** - for a dastest test whose reading sets or checks a tolerance: the tier,
  the `DAS_TUNE_POLICY` value, the device, the CPU kernel backend the registry served, the GPU
  backend where one served, and every environment variable it sets and every engine setter it calls
  to choose the code path it checks. An environment-variable flag a serving, instrument or test run
  left unset still belongs to its flags, with the value unset.
- **Environment override** - an env variable given to the run that changes what it compiles,
  tunes, or serves, other than the run's flags.
- **Board cell** - a run whose reading lands as a row of `performance/records/<box>.json` or as a
  figure in `PERF_LEDGER.md`.
- **Spawned cell** - a run `performance/gen_bench_records.das` spawns whose reading lands as a row
  of `performance/records/<box>.json`. A **manual cell** is the `benchmarks/lcpp_bench.das` command
  a `PROFILE.md` section documents, run by hand, its reading written as such a row.
- **Cell's input** - includes every environment setting the cell's spawn or its `PROFILE.md` command
  sets.
- **The `-jit` script** - `benchmarks/lcpp_bench.das` run as a script under `-jit`, not as the
  released exe. A **`-jit` reading** is a wall or rate that an instrument - the `-jit` script
  included - printed while running under `-jit`, or a ratio of two such prints, whoever computed it.
- **Rig leg** - a child cell `performance/gen_bench_records.das` spawns whose record row has
  `engine` `das`, named by that row's `(backend, flavor)` pair whatever its `workload`.
- **Third-party row** - a figure from a run of a third-party program serving the same model on the
  same input size.
- **Cell, script or exe of this repository** - a spawned or manual cell, a script in this
  repository's tree at the diff's tip or at a named commit, or an exe built from such a tree. A
  script outside the tree is not one, whatever it calls.
- **A box can mint a figure** - from a spawned cell when a rig leg it neither refuses nor skips
  drives the path the owed figure measures and `performance/gen_bench_records.das --catalog
  official` carries the model; from a manual cell when it runs the documented command's OS and
  backend and holds its model; a figure owed for new code when a rig leg or a documented command
  reaches that code's input. A figure owed for a route that no rig leg drives and no `PROFILE.md`
  section documents a command for is one no box can mint.
- **Board box** - a box with a `performance/records/<box>.json`.
- **Release re-mint** - a diff that runs `performance/gen_bench_records.das` with a release's exe on
  one or more board boxes and writes their rows into `performance/records/<box>.json`.
- **`held` entry** - a `performance/records/annotations.json` entry carrying a board row's `gguf`,
  `box`, `engine`, `backend`, `flavor` and `workload`, whose `held` field names the follow-up ledger
  row that keeps the row off the board.

### Board rows land at a release boundary {#board-rows-release-boundary}

The published board is a release's. Each das row of `performance/records/<box>.json` carries the
`dasllama_version` of the exe that minted it, and only a release's exe ships: a row minted between
releases would carry the current release's version while timing kernels no shipped binary has, so
the site's per-model display would show a speed no user can get. The release re-mint
(`performance/gen_bench_records.das` over every board box, `BRINGUP.md`) writes the das rows.
Between releases, a kernel change's measured wall-clock times live in a dated `PERF_LEDGER.md`
entry, and that entry is the regression baseline until the re-mint writes the row.
