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
  dependencies; reads `data.json` and `status.json` beside it.
- `run_stand.sh` - one pass on a ref, the thing cron calls (section 3); `caddy.snippet` is the public route.

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
`site/status.json` is written by `run_stand.sh` at start and end (`running` / `finished`, run id,
exit), so a night whose build failed is still visible.

### 2.1 Reading the numbers

The stand publishes the series and leaves the judgement to a person. Nothing decides what counts
as a regression: `benchctl compare --old-commit <a> --new-commit <b>` answers that on demand, with
a Welch test over the samples, for whichever two commits are actually in question.

### 2.2 Exit codes

`run`: 0 when every file ran ok, 1 otherwise. `report`: 0 when the latest night is ok, 1 when it
failed. `run --failed "<reason>"` benchmarks nothing: it writes the record of a night that could
not run the suite (the driver's way to record a failed build) and exits 1.

## 3. The box

`dasweb-1` (the daslang.io origin) runs `run_stand.sh master` from the `bench` user's cron under
`/srv/bench-stand`: `src/` (the clone), `runs/`, `site/` (what Caddy serves at `/bench/`),
`logs/`. The night builds Release (RelWithDebInfo arms the C++ allocation tracker, whose exit-time
report costs the run time) with the module set the benchmarks require, then `run`, then `report`;
`bin/` survives the checkout's clean, so a night whose build fails still renders a red night with
the previous binary. There is no CI runner and no ssh path into the box: the repository is public.

One-time setup, as root:

```sh
useradd -r -m -d /srv/bench-stand -s /bin/bash bench && apt-get install -y git cmake ninja-build g++ ccache
su - bench -c 'git clone https://github.com/GaijinEntertainment/daScript src && mkdir -p runs site logs'
echo '0 5 * * * bench /srv/bench-stand/src/utils/internal/bench-stand/run_stand.sh master >> /srv/bench-stand/logs/cron.log 2>&1' > /etc/cron.d/bench-stand
```

then paste `caddy.snippet` into the `daslang.io` block of the Caddyfile and `systemctl reload caddy`.

Arguments after the ref go to `main.das run` - `run_stand.sh master --filter core/math/ --repeat 1`
is a slice of a night. `BENCH_STAND_HOME` moves the layout, `BENCH_STAND_BUILD=skip` reuses the
last build; both are for local dry runs.

## 4. Configuration - `suite.json`

`root`, `exclude` (globs over the path under root; helper modules `_*.das` and `**/tests/**` by
default), `lanes` (`interp`, `jit`), `lane_excludes` (per lane, glob -> the reason those files do
not run in it), `repeat` (dastest `--count`), `timeout_seconds` per file, and `files` - per-file
`timeout_seconds` or a `skip` with its reason. A skipped file is listed on the site every night, so a skip is visible debt; an override
naming a file that no longer exists is an error.

## 5. Running locally

```sh
bin/daslang utils/internal/bench-stand/main.das -- run --meta meta.json --out /tmp/stand/runs/n1.json --filter core/math/ --repeat 2
bin/daslang utils/internal/bench-stand/main.das -- report --runs /tmp/stand/runs --out-data /tmp/stand/site/data.json
cp utils/internal/bench-stand/site/* /tmp/stand/site/ && ln -sfn ../runs /tmp/stand/site/runs
bin/daslang dastest/dastest.das -- --test utils/internal/bench-stand
```

`meta.json` is what `run_stand.sh` writes: `{"run_id", "started", "commit": {"sha", "date",
"subject", "author"}, "machine": {...}, "build": {"status", "seconds", "log_tail"}}`; any
subset parses. The whole pipeline runs locally too: `BENCH_STAND_HOME=<scratch>
BENCH_STAND_BUILD=skip run_stand.sh <branch> --filter core/math/` with `<scratch>/src` a clone
holding a built `bin/daslang`.
