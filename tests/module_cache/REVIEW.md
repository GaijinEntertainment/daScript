# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening what a test in this folder checks a spawned child's output against is a defect;
  an edited assertion weakens when it accepts an output the old one rejected, and re-pinning a
  count or a verdict form to the child's new true output does not.** A child's output is the
  only instrument a human has for what the cache and the scan served.

- **A test in this folder writes only under a directory it created for this process - its own
  files and its children's - and removes that directory.**
