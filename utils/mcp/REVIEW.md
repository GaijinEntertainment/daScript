# mcp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`.

**mcp never enters `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) — adding it
to either is a defect.** Development runs it through the python keep-alive supervisor, so an
exe form would never be dogfooded before shipping.
