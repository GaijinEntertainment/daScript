# dasHerd watcher

The watcher is dasHerd's local detachable terminal host. One persistent JIT
process owns multiple PTYs, terminal emulators, and durable session logs while
native/live-command surfaces attach as clients. The browser panel is an
interactive debug and recovery fallback; the ImGui client remains the primary UI.

Run it from the repository root:

```powershell
bin\Release\daslang.exe -jit utils\dasHerd\watcher\main.das
```

Run the docked native client with the token printed by the watcher:

```powershell
bin\Release\daslang-live.exe utils\dasHerd\watcher\rich_client.das -- --token=<token>
```

The client opens maximized at 150% zoom. It is a real ImGui dockspace with
repository/worktree, Git status, session/activity, terminal, and settings
windows. Windows can be resized, rearranged, tabbed, floated, closed, and
reopened from the View menu. Commands > PowerShell launches in the selected
worktree, attaches with control, and raises the terminal. `--port=9191` is
available on both processes when the default port is occupied.

Commands > Debug with Codex opens the Session Launcher. It can create a named
branch and dedicated worktree in a watcher-owned Git terminal, then starts
Codex there after Git succeeds. The watcher injects the session route and
writes a durable debug context without persisting the routing token. Sessions
show their immutable origin and agent-declared multi-repository Review Bundles;
clicking a participant switches the ordinary Git surfaces to that exact
worktree.

The watcher prints a token-bearing control-panel URL. Optional arguments after
`--` are `--port=9191`, `--token=<token>`, `--log-root=<directory>`, and
`--config=<path>`. The default global configuration is
`%APPDATA%\daScript\dasHerd.json` on Windows. Repositories are registered by
explicit checkout path; the Git observer resolves the first porcelain entry as
the visually distinct main checkout and lists linked worktrees without scanning
unrelated directories.

This first slice binds only to `127.0.0.1`. Its startup token prevents
accidental access from an unrelated local page; it is not the future remote
authentication design. Do not expose this port through a proxy or tunnel.

HTTP endpoints under `/api/v1/` provide health, session and repository listing,
session launch/input/resize/termination, repository registration/refresh, and a
deferred `POST /gc` diagnostic. `/ws` provides attach/detach, controller leases,
heartbeats, session and repository updates, launch/termination, input, resize,
terminal snapshots, and bounded raw PTY replay. A launch accepts either the
existing interactive `command_line` or a structured `argv`; repository Git
observation always uses the latter.
Raw output is delivered as binary WebSocket frames, so the browser can render
the same color and control sequences and send keyboard input back to the PTY.
Every request must include the startup token as the `token` query parameter.

Git observation runs exclusively inside watcher-owned task sessions and never
blocks the UI thread. The current local-first slice uses Git 2.17-compatible
`worktree list --porcelain` and `status --porcelain=1 --branch`, retaining the
branch/HEAD, staged, unstaged, untracked, conflicted, ahead/behind, locked, and
prunable state. ConPTY rewrites console output into VT cursor operations, so the
structured parser consumes the terminal model's reconstructed 240x100 screen;
if output enters scrollback it reports a visible error and points at the task
session rather than silently parsing truncated data. An unbounded structured
capture sidecar remains a follow-up.

File inspection uses the same hosted-task primitive and a 15-second external
timeout. Working-tree diffs request three context lines, then read the complete
working file separately for View; this keeps a 12,000-line file out of the PTY
diff stream without changing Git semantics. The rich client aligns compact
hunks for Diff, synchronizes both vertical scroll positions, and lazily prepares
the complete View. Unwrapped source files at least 128 KiB use a clipped
large-file renderer, so frame cost follows visible lines rather than file size.
daScript fragments receive a lexical recovery pass only where Tree-sitter left
unstyled ranges, preserving keyword/type/function colors across disconnected
hunks and parser recovery nodes.

Completed repository-refresh observation terminals are compacted to 160x50;
the shorter-lived file Diff/View task terminals compact to 80x25. When a later
refresh replaces the same repository/worktree observation, only the newest
emulator is kept in memory; every older `session.json`, `output.raw`, and
`events.jsonl` remains on disk for postmortem inspection. Interactive sessions
are not pruned by this policy.

The script exposes `init`, `update`, and `shutdown` for lifecycle hosts. Its
standalone loop performs optional GC only after `update` returns, when request
temporaries are dead; all state that must survive collection is globally rooted.
Automatic collection is fragmentation-driven and rate-limited. `POST /gc`
coalesces a request for a validated collection at that same safe boundary.

The browser terminal vendors xterm.js 5.5.0 with the matching fit 0.10.0,
Unicode 11 0.8.0, and web-links 0.11.0 addons under `vendor/xterm/`. It works
offline; the corresponding MIT license files are kept beside the assets.

Session logs are stored under
`logs/dasHerd/watcher/sessions/<session-id>/`. Each `events.jsonl` record has a
contiguous per-session sequence, wall and elapsed time, lifecycle event/detail,
and a snapshot of state, kind, purpose, repository, worktree, PID, and current
controller. Attach attempts, rejected controller claims, detach reasons,
disconnect/timeout releases, agent-worktree sequencing, bundle replacement,
and Attention projection are therefore reconstructible without correlating UI
screenshots. Closing a client releases its controller lease but does not stop
its child. Killing the watcher necessarily loses its live PTYs; already-flushed
logs remain readable.

Run the focused suite from the repository root with:

```powershell
bin\Release\daslang.exe dastest\dastest.das -- --test utils\dasHerd\watcher\tests
```

The Windows protocol integration test starts a real watcher and fake Codex,
creates two temporary Git repositories plus a dedicated worktree, drives HTTP,
WebSocket attach/detach/contention/disconnect, and syncs a two-repository bundle
through the real CLI. Its fixtures live under `tmp/dasHerd/` and diagnostics
under `logs/dasHerd/integration/`; successful runs remove both, while failed
runs print and preserve their exact directories for postmortem inspection.

The native terminal registers its full `ImGuiTerminalState` with the live-command
surface. `imgui_snapshot` can therefore read terminal text, selection, geometry,
cursor/blink state, performance counters, and input revisions without a screenshot;
`imgui_click` plus `imgui_key_type` drives the same input path as a human user.

The Git inspector is likewise semantic: `herder_file_inspector_open` selects a
repository/worktree/file/comparison, `herder_file_inspector_mode` switches Diff
or View, `herder_file_inspector_state` exposes loading, byte/row/change counts,
syntax and renderer telemetry, and `herder_file_inspector_text` returns exact
before/after/View text. Visual inspection is therefore optional for automation.
