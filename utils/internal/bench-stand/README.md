# bench-stand - the nightly benchmark stand

Every night one box checks out master, builds it, runs every `[benchmark]` file under
`benchmarks/` in the interpreter and JIT lanes, and publishes the result at
https://daslang.io/bench/ - a chart per benchmark arm over commits and a plain statement of what
failed and what moved. Review rules: `REVIEW.md`.

## 1. Layout

- The tool is `utils/benchctl` - `run` benchmarks the tree into one run record, `report` turns
  every record into `data.json` with an exit code, and its database verbs (`insert`, `query`,
  `compare`) answer ad-hoc questions about the same numbers. Every statistic it computes lives in
  `benchctl/benchstat.das`, which depends on no storage.
- `site/` - the viewer: `index.html`, `app.js`, `style.css`. Static, no build step, no
  dependencies; `app.js` reads `data.json`, `status.json` and `runs/` beside itself and builds
  the viewer's markup into a host page's `#stand`. The same two files back daslang.io's
  `site/nightly.html` (repo root), whose stylesheet tokens they adopt through `var(--token,
  fallback)`. The folder's `REVIEW.md` is not copied to the box.
- `stand.cmake` - one pass over the current tree, run by the `run_bench_stand` target (section 5); `caddy.snippet` is the public route.

## 2. Data model

Benchmark identity: `<file id>#<function>/<arm>` per lane, where the file id is the path under
`benchmarks/` without `.das` (`core/hash/test02#builtin_table/insert/600000`, lane `interp`).
The group is the file's directory; a new benchmark joins its group by living in the right folder.

`runs/<run_id>.json` - one per night, `RunRecord` in `bench_history.das`, never rewritten. The run
id is `<start UTC>-<sha8>`. It carries the commit, the machine, the build (status, seconds, log
tail), the lane states, and one `FileResult` per file per lane: status, exit code, seconds,
message, the last 40 log lines, and the samples. A sample is one arm over the night's repeats:
`ns` = the minimum ns/op (the recorded value), `ns_median`, `spread` = (max - min) / min, and
per-op allocation medians. A record's `status` (`ok`, `build_failed`, `bench_failed`) is
re-derived from its parts on every read.

File statuses: `ok`, `skipped` (listed in `suite.json` with a reason), `compile_error`, `failed`
(a `[benchmark]` function failed or panicked - its arms are dropped, the other functions' kept),
`timeout` (killed at `timeout_seconds`), `exit_nonzero` (the process died without a finished
dastest report), `spawn_failed`. A file whose report finished with its arms in hand stays `ok` on
a non-zero exit - only the shutdown went wrong - and the message says so; the JIT lane's probe
likewise believes the marker its program prints, not the exit code.

`site/data.json` - `Dataset`: run summaries (with failures, skips, lane states), groups, series
(columnar: `runs` indexes `Dataset.runs`, `ns`, `spread`).

### 2.1 Reading the numbers

The stand publishes the series and leaves the judgement to a person. Nothing decides what counts
as a regression: `benchctl compare --old-commit <a> --new-commit <b>` answers that on demand, with
a Welch test over the samples, for whichever two commits are actually in question.

### 2.2 Exit codes

`run`: 0 when every file ran ok, 1 otherwise. `report`: 0 when the latest night is ok, 1 when it
failed. `run --failed "<reason>"` benchmarks nothing: it writes the record of a night that could
not run the suite (the driver's way to record a failed build) and exits 1.

## 3. The box

`zen4` runs the night; `dasweb-1` (the daslang.io origin) only serves it. On zen4, cron runs
`~/bench-stand/nightly.sh` at 03:00 box time over `~/bench-stand/{src,runs,site,logs}`: the script
resets `src` to `origin/master`, builds `daslang`, the module libraries the benchmarks require
(SQLITE, PUGIXML, Audio, Minfft, Terminal, UnitTest, LLVM) and `test_aot_bench` (Release:
RelWithDebInfo arms the C++ allocation tracker, whose exit-time report costs the run time), writes
`meta.json`, then runs `benchctl run` - or `run --failed` after a red build, so the night is recorded
either way - then `report`, copies the viewer files from this folder's `site/`, writes `status.json`,
and publishes. Its arguments go to `benchctl run`. Logs: `~/bench-stand/logs/<date>.log` and
`build-<run_id>.log`. The script is box-local, not in the tree; `run_bench_stand` (section 5) is the
same night as one cmake target for a box that builds in-tree.

Publishing is two `rsync`s over ssh to `boris@dasweb-1`, `runs/` first and then `site/` with its
`site/runs -> ../runs` symlink (excluding the symlink 404s every record link), with the deploy key
`~/.ssh/bench_stand_deploy`, which dasweb-1's `authorized_keys` restricts to
`rrsync /srv/bench-stand`. dasweb-1 holds `/srv/bench-stand/{site,runs}` and serves them through
`caddy.snippet`, pasted into the `daslang.io` block of its Caddyfile. dasweb-1 builds nothing and has
no CI runner; the repository is public.

zen4 needs `llvm-22-dev` (apt.llvm.org carries it for bookworm; without it there is no jit lane),
`cmake`, `ninja-build`, `clang` and the module libraries' dev packages.

`BENCH_STAND_ARGS` goes to `benchctl run` - `-DBENCH_STAND_ARGS="--filter core/math/ --repeat 1"` is
a slice of a night. `BENCH_STAND_OUT` is where `runs/` and `site/` go (default `build/bench-stand`).

## 4. Configuration - `utils/benchctl/suite.json`

`root`, `exclude` (globs over the path under root; helper modules `_*.das` and `**/tests/**` by
default), `lanes` (`interp`, `jit`), `lane_excludes` (per lane, glob -> the reason those files do
not run in it), `repeat` (dastest `--count`), `timeout_seconds` per file, and `files` - per-file
`timeout_seconds` or a `skip` with its reason. A skipped file is listed on the site every night, so a skip is visible debt; an override
naming a file that no longer exists is an error.

## 5. Running locally

```sh
cmake -B build -DBENCH_STAND_ARGS="--filter core/math/ --repeat 1"
cmake --build build --target run_bench_stand
```

It benchmarks the checkout as it is into `build/bench-stand/` (`runs/`, `site/`, and the
`meta.json` it writes: `{"run_id", "started", "commit": {"sha", "date", "subject", "author"},
"machine": {...}, "build": {"status"}}`; any subset parses). Serve `build/bench-stand/site/` to
view it. The tool's tests: `bin/daslang dastest/dastest.das -- --test utils/benchctl/tests`.
