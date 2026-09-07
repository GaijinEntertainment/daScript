# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening what a test in this folder checks a spawned child's output against is a defect;
  an edited assertion weakens when it accepts an output the old one rejected, and re-pinning a
  count or a verdict form to the child's new true output does not.** The tests here pin that a
  run on the default cache path prints nothing on every read outcome, that `-module-cache
  <path>` prints its verdict and what it says, and what the scan trace says per descriptor - the
  only instruments a human has for what the cache and the scan served.

- **A test in this folder spawns its children against directories it created for this process
  and removes them; a diff that leaves a file of its own in the tree is a defect.** The
  `.das_module.manifest` sidecars a child's module scan writes beside the tree's descriptors are
  the scan's own steady state, gitignored, and stay.
