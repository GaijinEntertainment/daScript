# lsp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**lsp never ships as a prebuilt exe — adding it to `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES`
(`utils/CMakeLists.txt`) is a defect.** Development runs it through the python keep-alive
supervisor, so an exe form would never be dogfooded before shipping.
