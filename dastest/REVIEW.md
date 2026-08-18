# dastest Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

dastest ships as a prebuilt exe — the `DAS_UTILS_SHIPPED_EXES` entry (`utils/CMakeLists.txt`,
repo root) records this decision; the rules for changing it live in `utils/REVIEW.md`.

**A helper added to, renamed in, or removed from `review_gate.das` updates its section in
`README.md` in the same change.**
