# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening the default-cache output checks in `test_default_cache_path.das` is a defect** -
  it pins that a run on the default cache path prints nothing, cold, warm or over a truncated
  cache file, and that an explicit `-module-cache` still prints its verdict. The default cache
  is on unasked for an ordinary run, so a line it prints is output every user sees.
