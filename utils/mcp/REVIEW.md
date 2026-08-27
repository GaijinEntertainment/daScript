# mcp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`.

**Never add `mcp` to `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) - run it
interpreted through `.mcp.json` instead.**
Development runs the server through the python keep-alive supervisor, so an exe form would
never be used in development before it ships.
