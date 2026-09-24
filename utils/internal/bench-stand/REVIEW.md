# bench-stand Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. A viewer file - a page, script, or stylesheet the stand serves at `/bench/` - answers
to the viewer checklist beside it, `site/REVIEW.md` in this folder, wherever the diff puts it.
**A diff that changes what a run
record contains or how it is parsed, wherever it lands, answers to `utils/benchctl/REVIEW.md`
too** - the records are written and read there, and the viewer parses them.

**Placement - one file, one line: a diff keeps each file inside its line, and a new file adds its
line here, with its tests, in the same change.**

- `site/` - the viewer. Zero dependencies, zero build step.
- `stand.cmake` - one pass over the current tree: run, report, publish. The `run_bench_stand` target runs it.
- `caddy.snippet` - the public route.
