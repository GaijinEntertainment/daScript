# Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

- **A diff that widens the `dasbind` skip in `.das_test` or drops a probe shape from
  `dasbind/test_extern_abi.das` is a defect.** The suite is the only check of which register or
  stack slot an interpreted `[extern]` call puts each argument in - the JIT never takes that
  path - so coverage lost there reports green on every lane.
