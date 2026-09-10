# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Editing an assertion literal, or the helper that produces the text a test in this folder
  compares a spawned child's output or the files it wrote against, so it accepts an output the
  old text rejected is a defect; re-pinning a count or the fixed words of the scan-trace
  `[module] descriptor ...` line or the cache's verdict line to the child's new true output is
  not.** A child's output is the only instrument a human has for what the cache and the scan
  served.

- **A helper that trims a child's line before the compare drops only its elapsed times - every
  other field on the line, a count included, stays in the compared text.** A dropped field is
  a change the test can no longer see.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**

- **A test in this folder runs a child as a statement of its own - `run_child_reported`,
  `run_driver_reported`, or the run's result bound to a local before the compare - never as an
  argument beside the `out` it writes, and carries `options _lint = "LINT030"`.** The order a
  call's arguments are evaluated in is not defined, so a nested run reads its output empty on a
  Windows AOT host and full everywhere else.
