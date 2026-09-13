# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A diff that adds or changes an assertion literal in this folder's tests - a marker or a
  field a test compares against, not the message an assertion reports - makes it match only text
  the spawned child emits, in its output or in a file it wrote.** A child's output is the only
  instrument a human has for what the cache and the module scan served.

- **A diff that adds or changes a helper that decides whether a child's line is the one a test in
  this folder wants makes that decision compare every field of the line except its elapsed
  times.** A field the helper does not compare is a change the test can no longer see.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**

- **Silencing LINT030 for a test in this folder - a `nolint:LINT030` marker, or an entry in the
  repo-root `.lint_config` that turns LINT030 off or excludes this folder's paths - is a defect;
  give the child run a statement of its own and read `out` in the next one.** The order a call's
  arguments are evaluated in is not defined, so a child run nested beside the `out` it writes
  reads that output before the child wrote it on some tiers and after on others.
