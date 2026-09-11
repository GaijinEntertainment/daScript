# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **A diff that changes an assertion literal, or the helper producing the text a test in this
  folder compares a spawned child's output or the files it wrote against, so the compare accepts
  text the child does not truly emit is a defect.** A child's output is the only instrument a
  human has for what the cache and the module scan served.

- **A helper that trims a child's line before the compare drops only its elapsed times - every
  other field on the line, a count included, stays in the compared text.** A dropped field is
  a change the test can no longer see.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**

- **Silencing LINT030 for a test in this folder - a `nolint:LINT030` marker or a `.lint_config`
  exclusion - is a defect.** The order a call's arguments are evaluated in is not defined, so a
  child run nested beside the `out` it writes reads that output before the child wrote it on
  some tiers and after on others; the rule is what keeps every run a statement of its own.
