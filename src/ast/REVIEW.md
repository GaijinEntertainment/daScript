# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping its scan of the prints in
  `trySerializeProgramModule` (`ast_parse.cpp`, `ARCHITECTURE.md` sec.1), or a finding text that
  no longer names what failed. What the gate enforces is read from the gate itself.
