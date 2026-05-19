---
slug: daslang-live-no-port-flag-single-instance-lock-e2e-spawn-tests-infeasible
title: Why is reliable in-suite e2e testing of daslang-live spawn infeasible?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

> **RESOLVED 2026-05-18.** daslang-live now accepts `--live-port <N>` and the single-instance lock is keyed on the resolved port. The MCP `do_live_launch` also forwards its `port` arg to the spawned binary, so polling and bind now match. Historical body preserved below.

---

**daslang-live has no `-port` CLI flag** — the HTTP server port is hardcoded to 9090 in the binary. The MCP `port` argument (in `do_live_launch`, `do_live_status`, etc.) only changes which port the MCP *polls*, not what daslang-live *binds*. So a test using an isolated high port like 19090:

1. Calls `do_live_launch(file, ..., port="19090")`. The launcher spawns daslang-live which binds 9090.
2. `do_live_launch`'s post-spawn poll loop hits `live_is_running("19090")` → returns false (nothing on 19090).
3. After 10s the loop returns `"daslang-live launched but did not respond on port {p} within 10 seconds"` — the test sees an error, but daslang-live is alive on 9090.
4. Test's cleanup `do_live_shutdown("19090")` no-ops. The orphan process needs manual `taskkill`.

Plus **`acquire_single_instance()` lock** (`utils/daslang-live/main.cpp:709`) — only ONE daslang-live can run per host. A stale instance from a previous failed test blocks the next spawn entirely. The lock is process-wide, not port-scoped.

Combined: parallel test isolation requires either (a) a `-port <N>` flag on daslang-live or (b) a `--dry-run-launch` mode in `do_live_launch` that returns the would-be argv without spawning. Without either, the practical path is:

- **Pin args-builder logic at unit level** — extract `build_live_script_args(file, project, project_root, is_windows)` and test the string output directly. No spawn, no port. See `utils/mcp/test_tools.das` Tier 4a tests.
- **Pin the dispatcher side at MCP level** — Tier 1 tests against the daspkg-style fixture exercise `do_compile_check / do_lint / do_list_requires / ...` end-to-end through the subprocess argv path, proving `-project_root` flows correctly. Doesn't touch daslang-live but covers the same code.
- **Defer real spawn tests** until an upstream daslang-live change adds port-iso or dry-run mode.

Surfaced 2026-05-18 during MCP `test_live_tools.das` implementation. Tier 4b deferred to follow-up; documented in PR plan + `skills/external_module_debugging.md`.

## Questions
- Why is reliable in-suite e2e testing of daslang-live spawn infeasible?
- Why does daslang-live ignore the `port` argument I passed to `do_live_launch`?
- Can I run two daslang-live instances in parallel for test isolation?

## Questions
- Why is reliable in-suite e2e testing of daslang-live spawn infeasible?
