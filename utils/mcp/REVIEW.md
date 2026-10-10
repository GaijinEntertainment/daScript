# mcp Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `ROADMAP.md`.

**A diff that adds a file directly in this folder that `main.das` or `cpp_main.das` reaches
through `require`, that a `.cmd` launcher runs, or that a shipped document tells the user to
run adds it to an `install(FILES ...)` block with `DESTINATION utils/mcp` in `CMakeLists.txt`
(repo root), in the same change.** A file left out is missing from the shipped SDK.

**Weakening any case in `test_tools.das` that pins which comments the formatter keeps is a
defect.**

**Never normalize a path that `resolve_path` or `server_root` (`tools/common.das`) returns - keep
the generic, forward-slash spelling.** `normalize` / `lexically_normal` rewrites every separator to
the platform's preferred one, so on Windows the served root's forward slashes become backslashes and
every string comparison against that root stops matching.
