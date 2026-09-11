# bench-stand - the nightly benchmark stand

Every night one box checks out master, builds it, runs every `[benchmark]` file under
`benchmarks/` in the interpreter and JIT lanes, and publishes the result at
https://daslang.io/bench/ - a chart per benchmark arm over commits, a per-group index, and
a plain statement of what failed and what moved. The GitHub workflow
`.github/workflows/nightly_bench.yml` is the trigger and the red/green; the box is the
measuring instrument. Review rules: `REVIEW.md`.

## 1. Layout

- `main.das` - the tool: `run` (benchmark the tree into one run record) and `report` (every
  record into `data.json` + `summary.md`, with the exit code the workflow keys on).
- `bench_suite.das` - `suite.json` loading and file discovery: which files, in which group,
  under which limits. Pure; no processes.
- `bench_runner.das` - one file in one lane: spawn `dastest --bench`, enforce the wall-clock
  and memory limits, turn its output into samples and a verdict. The parser is pure and
  tested on captured output shapes.
- `bench_history.das` - records to dataset: series, the group index, regression verdicts,
  the markdown summary. Pure.
- `suite.json` - the configuration (section 4).
- `site/` - the viewer: `index.html`, `app.js`, `style.css`. Static, no build step, no
  dependencies; reads `data.json` and `status.json` beside it.
- `nightly.sh` - the box-side driver (section 3); `bench-stand-deploy.sh` provisions the box;
  `caddy.snippet` is the public route, and the only place a route is written down.
- `REVIEW.md` + `REVIEW.das` - the review checklist and its mechanical half. The gate checks the
  key parity of `suite.json` / `SuiteConfig` / section 4 below, that a `skip` states a reason,
  that a sibling module is required by bare name, that the viewer writes no markup, that every
  `BENCH_STAND_*` knob is documented in both places, that no route literal is copied into
  `bench-stand-deploy.sh`, and that every file here has its placement line.
- `test_bench_*.das` - the three dastest suites; `run_utils_tests` (per PR) runs them.

## 2. Data model

Benchmark identity: `<file id>#<function>/<arm>` per lane, where the file id is the path under
`benchmarks/` without `.das` (`core/hash/test02#builtin_table/insert/600000`, lane `interp`).
The group is the file's directory (`core/hash`, `sort`, `sql`); the viewer sections and the
index follow it, so a new benchmark joins its group by living in the right folder.

`runs/<run_id>.json` - one per night, `RunRecord` in `bench_history.das`. The run id is
`<start UTC>-<sha8>`. It carries the commit, the machine (host, CPU, cores, memory, kernel,
compiler, load at start), the build (status, seconds, log tail), the lane states, and one
`FileResult` per file per lane: status, exit code, seconds, message, the last 60 log lines,
and the samples that completed. A sample is one arm aggregated over the night's repeats:
`ns` = the minimum ns/op (the noise-robust statistic), `ns_median`, `spread` =
(max - min) / min, and per-op allocation medians.

A record's own `status` is `ok`, `build_failed` or `bench_failed`, and `load_runs` re-derives it
from the record's parts on every read, so a hand-edited status cannot mislead the report.

File statuses: `ok`, `skipped` (listed in `suite.json` with a reason), `compile_error`,
`failed` (a `[benchmark]` function failed an assertion or panicked - its arms are dropped, the
other functions' arms kept), `timeout`, `memory` (killed at the RSS cap), `exit_nonzero` (the
process died without a finished dastest report, so the suite was cut short), `spawn_failed`.

A file whose dastest report is finished and whose arms are all there stays `ok` even when the
process then exits non-zero: the measurement stands and only the shutdown went wrong, so the
message records it (`measured, then exited with code 1 at shutdown`). Keying the verdict on the
exit code instead would fail every file on a RelWithDebInfo host, where the C++ allocation
tracker's exit-time report takes every process to 1.

The JIT lane is probed once per night with a trivial program, and the probe believes the marker
that program prints, not the process exit code - for the same reason.

## 2.3 The AOT lane

AOT is not a flag on `daslang`: the benchmark bodies are compiled to C++ at build time and linked
into a binary. That binary is `test_aot`, which the benchmark files join through the
`benchmarks` row of `DAS_AOT_SUITES` in `tests/aot/CMakeLists.txt` (repo root); `--aot-bin` names
it and the night builds `--target test_aot` before measuring. The lane's argv carries **both**
`-use-aot` on the host and `--use-aot` on dastest. The second one is what arms `fail_on_no_aot`:
without it every function silently interprets and the lane reports interpreter numbers under an
AOT label, and with it a function missing its stub is `error[50101]`, so a run that finishes is
proof the stubs were live.

Do not read the tier from dastest's own label: `is_in_aot()` is true only while the AOT emitter
runs, so an AOT run still prints `[INTERP]`. The lane name is the record of what ran.

A benchmark whose require graph reaches a native module needs that module's own AOT half linked
into the same binary. Those suites are excluded from the AOT set in `tests/aot/CMakeLists.txt`
and carry their reason under `lane_excludes` in `suite.json`; widening the first is what lets the
second go away.

`site/data.json` - `Dataset`: run summaries, groups, series (columnar: `runs` the run index into
`Dataset.runs`, `ns` the minimum, `spread` the night's spread), the changes for the latest run
(`regression`, `improvement`, `first_seen`, `missing`), the group index (`runs` and `value`). `site/summary.md` is the
same night in markdown; the workflow posts it as its step summary. `site/status.json` is
written by `nightly.sh` at start and end (`running` / `finished`, run id, exit), so a night
whose build failed before any binary existed is still visible.

### 2.1 Statistics

A change verdict compares tonight's value with the median of the series' previous
`baseline_runs` points (at least 3). It is a regression when the relative move exceeds both
`regression_threshold` and `noise_multiplier` times the baseline's relative median absolute
deviation; an improvement when it falls by the same gate. A series with no earlier point is
`new`; one with a baseline but no point tonight while its file ran ok is `missing` - an arm
was renamed or removed. The group index is the geometric mean over the group's series of
value / reference, times 100, where each series' reference is the median of its first five
points; a run contributes when at least half the group's series measured that night.

### 2.2 Exit codes

`run`: 0 when every file ran ok, 1 otherwise. `report`: 0 when the latest night is ok, 1 when it
failed (build or any file), 2 when it is ok but regressed and `--fail-on-regression` was
given. `nightly.sh run` exits with the run's code, or the report's when the run was clean.

`run --failed "<reason>"` benchmarks nothing: it writes the record of a night that could not run
the suite and exits 1. That is how the driver records a failed build or its own failure without a
second writer of the run-record schema; a build that failed carries no file, since its log tail
already rides the record's `build`.

## 3. The box

`dasweb-1` (the daslang.io origin) runs the stand as the unprivileged `bench` user under
`/srv/bench-stand`: `src/` (the clone each night checks out), `runs/`, `site/` (what Caddy
serves at `/bench/`), `logs/`, `last_good/` (the last passing build's binary, `daslib/` and
this tool - renders the report when tonight's build fails, so a broken master publishes a red
night instead of a stale page), `state/` (run lock, current run id).

The workflow reaches the box over ssh as `bench` with a key whose `authorized_keys` line
forces `nightly.sh gate`: only `start <ref> [fail-on-regression]`, `follow [run_id]`, `status`
and `summary` exist on that connection, at most two arguments each and only path characters in
them, no pty, no forwarding. The gate reads the caller's words from `SSH_ORIGINAL_COMMAND`, which
is also why `start`'s second word is the only way the workflow can ask for a regression to fail
the night - a forced command carries no environment. `start`
resolves the ref, launches the night detached and prints the run id; `follow` streams the log
and exits with the run's code once `status.json` says finished, so a dropped ssh session
reconnects instead of failing the lane. There is deliberately no self-hosted runner: the
repository is public, and one would execute pull-request code on the production web box.

The night builds Release (`-DDAS_SQLITE_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF
-DDAS_LLVM_DISABLED=OFF`, GLFW and HV off), the targets the benchmarks need, and `test_aot` for
the AOT lane - that last one is ~1080 translation units and the bulk of the night's build. Release
matters: RelWithDebInfo arms the C++ allocation tracker, whose exit-time leak report turns
every clean process into exit 1. The JIT lane is probed once per night (`daslang -jit` on a
trivial program); when the probe fails the lane is recorded as unavailable with the reason and
the interpreter lane still runs.

One-time setup, as root on the box, with the public half of a fresh ed25519 key:

```sh
sudo install -m 755 utils/internal/bench-stand/bench-stand-deploy.sh /usr/local/sbin/
sudo bench-stand-deploy.sh provision ./bench_key.pub
sudo bench-stand-deploy.sh caddy
```

Then store the private half as the `DASWEB_BENCH_KEY` environment secret on `github-pages`
and dispatch `nightly_bench.yml` once. `bench-stand-deploy.sh status` shows the layout, the
last `status.json` and whether Caddy carries every route the snippet declares (it derives that
list from `caddy.snippet`, so a route added there is checked without editing the script; a
vhost holding some routes and not others is reported and never repaired automatically). The
deploy script installs the packages the build needs (git, cmake, ninja, g++, ccache, rsync) -
ccache is what keeps the nightly rebuild at minutes rather than the cold 20-40.

Environment knobs `nightly.sh` reads, all optional: `BENCH_STAND_HOME` (layout root),
`BENCH_STAND_REPO_URL` (commit-link base), `BENCH_STAND_SITE_URL` (what the summary links),
`BENCH_STAND_JOBS` (build parallelism; the default counts the box's CPUs through `nproc`, `sysctl hw.ncpu`, or 4), `BENCH_STAND_CMAKE_ARGS` (extra configure arguments),
`BENCH_STAND_LANES` / `BENCH_STAND_REPEAT` / `BENCH_STAND_FAIL_ON_REGRESSION` (forwarded to
the tool), `BENCH_STAND_AOT=skip` (leave the AOT binary
unbuilt, so the aot lane reports itself unavailable and the other two still measure), and two for
local dry runs only: `BENCH_STAND_BUILD=skip` (reuse `src/bin/daslang`) and `BENCH_STAND_FILTER`
(a path substring).

## 4. Configuration - `suite.json`

`root` (the benchmark tree, `benchmarks`), `exclude` (globs over the path under root; helper
modules `_*.das` and `**/tests/**` by default), `lanes` (`interp`, `jit`, `aot`),
`lane_excludes` (per lane, a glob to the reason those files do not run in it - a file the AOT
binary cannot carry still measures in the other two, and the reason shows on the site every
night), `repeat` (dastest `--count`; the
minimum over repeats is the recorded value), `timeout_seconds` and `memory_limit_mb` per file
(the runner kills the child and records `timeout` / `memory`), `regression_threshold`,
`noise_multiplier`, `baseline_runs` (section 2.1), and `files` - per-file overrides of the
limits or a `skip` with its reason. A skipped file is listed on the site and in the summary
every night, so a skip is visible debt, not a deletion; an override naming a file that no
longer exists is an error, so a renamed benchmark cannot silently lose its skip.

## 5. Running locally

```sh
# one night of a slice of the suite, both lanes, into a scratch record
bin/daslang utils/internal/bench-stand/main.das -- run --meta meta.json --out /tmp/stand/runs/n1.json --filter core/math/ --repeat 2
# the report over every record, then open site/ over any static server
bin/daslang utils/internal/bench-stand/main.das -- report --runs /tmp/stand/runs --out-data /tmp/stand/site/data.json --out-summary /tmp/stand/site/summary.md
cp utils/internal/bench-stand/site/* /tmp/stand/site/ && ln -sfn ../runs /tmp/stand/site/runs
bin/daslang dastest/dastest.das -- --test utils/internal/bench-stand
```

`meta.json` is what `nightly.sh` writes: `{"run_id", "started", "commit": {"sha", "date",
"subject", "author"}, "machine": {...}, "build": {"status", "seconds", "log_tail"}}`; any
subset parses. The whole box pipeline runs locally too: point `BENCH_STAND_HOME` at a scratch
layout whose `src/` is a worktree with a built `bin/daslang`, and run
`BENCH_STAND_BUILD=skip BENCH_STAND_FILTER=core/math/ nightly.sh run HEAD`.

## 6. Reading a red night

A night whose build failed says so first: the summary leads with the build's own section and the
last lines of its log, and stops there - nothing was measured, so there is nothing else to read.
Otherwise the step summary (and `summary.md`) leads with the status and the commit, then a failures
table - file, lane, status, message - where the message is the thing to act on: the first
`error[...]` line of a compile error, the failed function names, the kill reason with the knob
that set the limit, or the `FATAL` line of a non-zero exit. The run record has the last 60 log
lines of every failed file. A `build_failed` night's record carries the build log tail. A
regression row names the arm, both values, the change and the noise that the gate was
measured against; the chart behind it is one click from the site's latest-night panel.
