# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping its scan of the prints in
  `trySerializeProgramModule` (`ast_parse.cpp`, `ARCHITECTURE.md` sec.1), or a finding text that
  no longer names what failed. What the gate enforces is read from the gate itself.

- **A diff that moves a module-cache read diagnostic out of `trySerializeProgramModule` - into
  a helper it calls, or another function - extends `REVIEW.das`'s scan to the new home in the
  same change.** The gate scans that one function's body; a print that leaves it leaves the
  gate, and an ungated line there is output every user of the default cache sees.
