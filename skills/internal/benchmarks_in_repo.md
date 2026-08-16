# Benchmarks in this repo (repo-only)

Read this before adding or moving a file under `benchmarks/`. How to write and run a
benchmark is `skills/writing_benchmarks.md`; this file is the repo's conventions.

## Layout and index

Benchmarks live under `benchmarks/<category>/`, with a subdirectory per subject
(`core/hash/`, `core/table/`, `sort/`, `sql/`, …). Pick an existing category or add one.

`benchmarks/README.md` indexes **every** `.das` file in the tree. Adding, removing, or
renaming a file means editing the index in the same commit: a row with the filename and a
one-line description, under the table for its directory, plus a new section header when the
directory is new. Helper modules (`_`-prefixed, skipped by dastest) are listed too, marked
as not a benchmark.

`benchmarks/REVIEW.md` binds anything that produces record-grade timing — record rigs,
A/B labs, and what may enter `PERF_LEDGER.md`.

## Capturing output

Run one file at a time and redirect to a file, then read the file — a whole directory in
one invocation can run for hours, and terminal capture truncates and interleaves with
scrollback.

```bash
for f in benchmarks/core/hash/test*.das; do
    bin/daslang dastest/dastest.das -- --bench --test "$f" >> _bench_out.txt 2>&1
done
```

Delete the scratch log when done.
