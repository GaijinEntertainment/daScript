# bench-stand Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A diff here that changes what the nightly workflow invokes - a `main.das` verb or
flag, an exit code, or the `nightly.sh gate` vocabulary - applies `.github/workflows/REVIEW.md`
(repo root) too.

**Never put a `[test]` file outside this directory - a test of this tool lives beside the file it
tests.**

**Never add a test that touches the filesystem outside a `temp_directory`-rooted path, or that
leaves behind what it creates.**

**A failure message a diff adds or changes states the underlying reason, and the summary line
carrying it names the file and the lane it ran in (`interp` or `jit`).**

**Never add a run-record field without saying in `README.md` section 2 what reads it** - the
summary, the viewer, or a person opening the record. A record is an archive as well as the
viewer's input, so a field only a human reads is fine; a field nobody named is one nobody
notices going wrong.

**A diff that changes a run-record field keeps the new reader parsing a record written by the old
code, a missing field keeping its declared default.** Records already on the box are never
rewritten.

**Never let a child's own output overwrite a `timeout` or `memory` status in `run_bench_file` -
a killed child that printed a passing report is still killed. Loosen a value in `suite.json`
instead.**

**Weakening the limit arms of `test_run_bench_file_limits` (`test_bench_runner.das`) is a defect:
dropping an arm, or relaxing what it asserts about the kill.**

**A series a diff adds or recolors in `site/app.js` takes its color from its lane, never from its
position in the series list** - a color that moves when a series is added makes two nights
uncomparable.

**A chart a diff adds to `site/app.js` shows a legend whenever it draws more than one lane.**

**A chart a diff adds to `site/app.js` also ships its table view - the same points as a table of
numbers, behind the card's `table` toggle.**

**Never read a benchmark's identity from anywhere but its path under `benchmarks/`** - the group
is the directory, the id is the path without `.das`.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a check
walks, or rewriting a finding text so it no longer names what failed.**

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds its
line here, with its tests, in the same change.**

- `main.das` - verbs, argv, exit-code mapping. No parsing of dastest output, no statistics.
- `bench_suite.das` - `suite.json` and file discovery. No processes.
- `bench_runner.das` - one file in one lane: spawn, limits, output to samples and verdict.
- `bench_history.das` - records to dataset, verdicts, summary markdown. No processes, no argv.
- `suite.json` - configuration.
- `site/` - the viewer. Zero dependencies, zero build step.
- `nightly.sh` - the box-side driver.
- `bench-stand-deploy.sh` - root-side provisioning.
- `caddy.snippet` - the public route, and the only place a route is written down.
- `REVIEW.das` - the mechanical half of this checklist.
- `test_bench_suite.das`, `test_bench_runner.das`, `test_bench_history.das` - the module suites.
- `test_bench_cli.das` - the two programs, spawned: `main.das`'s verbs and `nightly.sh`'s ssh gate.
