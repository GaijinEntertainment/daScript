# bench-stand Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**Never add a run-record field without saying in `README.md` section 2 what reads it - the
viewer, or a person opening the record.** A field nobody named is one nobody notices going wrong.

**A diff that changes a run-record field keeps the new reader parsing a record written by the old
code, a missing field keeping its declared default.** Records already on the box are never
rewritten.

**Never let a child's own output overwrite a `timeout` status in `run_bench_file` - a killed child
that printed a passing report is still killed.** Loosen `timeout_seconds` in `suite.json` instead.

**A series a diff adds or recolors in `site/app.js` takes its color from its lane, never from its
position in the series list, and a chart drawing more than one lane shows a legend.**

**Never read a benchmark's identity from anywhere but its path under `benchmarks/`** - the group
is the directory, the id is the path without `.das`.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds its
line here, with its tests, in the same change.**

- `site/` - the viewer. Zero dependencies, zero build step.
- `run_stand.sh` - one pass on a ref: checkout, build, run, report, publish. Cron calls it.
- `caddy.snippet` - the public route, and the only place a route is written down.

The tool this box runs is `utils/benchctl` - its modules, verbs and tests answer to
`utils/benchctl/REVIEW.md`.
