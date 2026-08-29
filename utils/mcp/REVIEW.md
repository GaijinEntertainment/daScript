# mcp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `ROADMAP.md`.

**Never add `mcp` to `DAS_UTILS` or `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) - run it
interpreted through `.mcp.json` instead.**
Development runs the server through the python keep-alive supervisor, so an exe form would
never be used in development before it ships.

**A diff that adds a top-level `.das` under `utils/mcp/` that `main.das` reaches also adds it to
the `install(FILES ...)` block in `CMakeLists.txt` (repo root), in the same change.** `tools/`
and `subtools/` are globbed; a top-level file left out of the list dies in the shipped SDK on
`error[20605] missing prerequisite` while the in-tree server keeps working.
