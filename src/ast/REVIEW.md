# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/builtin/ARCHITECTURE.md` (repo root) - the module cache's home; this folder has no design
document of its own.

- **Every print the module-cache read reaches in `trySerializeProgramModule` (`ast_parse.cpp`)
  is gated on the serializer's `quietCache`; a diff that leaves one ungated is a defect** - the
  default cache is on for every run that executes, so an ungated line - a truncated record, a
  stale module, a changed macro dependency - becomes output every user sees.
