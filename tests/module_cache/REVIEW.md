# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A diff that adds or changes an assertion literal in this folder's tests makes it match only
  text the spawned child emits, in its output or in a file it wrote.** A child's output is the
  only instrument a human has for what the cache and the module scan served.

- **A diff that adds or changes a helper that produces the text a test in this folder compares
  against a spawned child's output keeps every field of the child's line but its elapsed
  times.** A dropped field is a change the test can no longer see.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**

- **Silencing LINT030 for a test in this folder - a `nolint:LINT030` marker or a `.lint_config`
  exclusion - is a defect.** The order a call's arguments are evaluated in is not defined, so a
  child run nested beside the `out` it writes reads that output before the child wrote it on
  some tiers and after on others; the rule is what keeps every run a statement of its own.
