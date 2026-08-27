# lsp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**Never add `lsp` to `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` in `utils/CMakeLists.txt` - clients
start the server through `lsp_supervisor.py` instead.** An exe form would ship without anyone
having used it.
