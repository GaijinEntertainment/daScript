# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/builtin/ARCHITECTURE.md` (repo root).

- **Weakening the default-cache silence check in `test_default_cache_path.das` (beside this
  file) is a defect** - it pins that a run on the default cache path prints nothing, cold, warm
  or over a damaged cache file, and that an explicit `-module-cache` still prints its verdict.
  The default cache is on for every run that executes, so a line it prints is output every user
  sees.
