# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening the text a test in this folder compares a spawned child's output against - an
  assertion literal, or the helper that produces the compared text - is a defect; an edit
  weakens when it accepts an output the old one rejected, and re-pinning a count or the fixed
  words of a scan or cache line to the child's new true output does not. A timing the child
  newly prints leaves the compared text; every other field stays compared.** A child's output is
  the only instrument a human has for what the cache and the scan served.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**
