# Module cache tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening a default-path silence check in `test_default_cache_path.das` is a defect** - the
  file pins that a run on the default cache path prints nothing on every read outcome. The
  default cache is on unasked for an ordinary run, so a line it prints is output every user sees.

- **Weakening an explicit-cache verdict check in `test_default_cache_path.das` is a defect** -
  the file pins that `-module-cache <path>` prints its verdict, and what that verdict says. The
  verdict is the only instrument a human has for what the cache served.
