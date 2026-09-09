# dastest Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**A diff under this folder applies the shipped-exe and arm/CI-row rules of `utils/REVIEW.md`
(repo root) too, reading `dastest/` for `utils/`** - dastest ships as a prebuilt exe, the
`DAS_UTILS_SHIPPED_EXES` entry in `utils/CMakeLists.txt` (repo root).

**A diff that adds, renames, or removes a public function in `review_gate.das` also updates
the `review_gate.das` section of `README.md`, in the same change.**

**A new test for dastest itself goes under `tests/` (this folder) with no registration - the
extended lane runs that whole directory on every PR.**
