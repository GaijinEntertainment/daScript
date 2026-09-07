# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping its scan of the prints in
  `trySerializeProgramModule` (`ast_parse.cpp`, `ARCHITECTURE.md` sec.1), or a finding text that
  no longer names what failed. What the gate enforces is read from the gate itself.

- **A diff that moves a module-cache read diagnostic out of `trySerializeProgramModule` - into
  a helper it calls, or another function - extends `REVIEW.das`'s scan to the new home in the
  same change.** The gate scans that one function's body, so a print moved out of it is a print
  the gate no longer checks, and an ungated line there is output every user of the default cache
  sees.

- **A diff that adds a builtin a `.das_module` descriptor can call to change the require
  resolver or the module registry records the call in the descriptor manifest in the same change
  (`ARCHITECTURE.md` sec.2), and a diff that changes what a recorded row means bumps the
  manifest's format version.** A replayed start never runs the descriptor, so an effect the
  recorder does not see is an effect every warm start silently lacks, and a row an older reader
  misreads is a wrong registration with no diagnostic.
