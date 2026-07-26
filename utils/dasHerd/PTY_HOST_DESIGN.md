# Detached PTY host — "a terminal that depends on nothing"

Status: CORE LANDED (2026-07-25 night). Remaining: the watcher rework
below ("What changes where"). `--continue` resume demotes to disaster
recovery, never the mechanism.

## Resume state for the next session (written before a /clear)

- DONE, committed, tests green (watcher suite 68/68; test_ptyhost 1.9s):
  spawn_detached + env blocks in dasTerminal; utils/dasHerd/ptyhost/main.das
  (protocol v1); daspkg release packaging; popen BREAKAWAY_OK fix in
  src/builtin/module_builtin_fio.cpp; lifecycle test
  utils/dasHerd/watcher/tests/test_ptyhost.das.
- NEXT: the watcher rework only — launch-via-host, adoption on startup
  (sessions resurrect as RUNNING), pumps proxied over the host WS. Client
  unchanged in v1. Read this doc + utils/dasHerd/HERDER_FIXROUND.md notes
  40-51 first.
- Rig state: watcher and rich client are STOPPED (Boris's go). daslang.exe,
  libDaScriptDyn_runtime.dll, dasModuleTerminal rebuilt with the fixes;
  release bundle at logs/dasHerd/releases/dasherd-ptyhost (rebuild with
  `daslang utils/daspkg/main.das -- release --out logs/dasHerd/releases
  --root utils/dasHerd/ptyhost`, 7s).
- Standing rules: file Boris's bug reports BEFORE fixing; never restart a
  watcher with live agent sessions without his go; preflight budget for
  this branch: at most one full run (none used); branch
  codex/herder-view-without-diff, ~20 unpushed commits, PR later on his
  word. Parked after this block: external sessions arc, notes 42/47
  terminal rendering bugs, WebSocketServer handle leak at client exit.

## Problem, precisely

`ConPtyProcess::launch` (modules/dasTerminal/src/pty.cpp:163) creates the
pipes, the pseudoconsole (HPCON), AND a `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`
job object **inside the watcher process**, then assigns the child to that
job. Watcher exit therefore kills every hosted session twice over: the job
handle closes (kill-on-close reaps the child tree) and the pseudoconsole
tears down (console-close for anything that survived). The session's
*record*, *journal*, and *conversation* survive; the *process* cannot.

## Shape: one tiny host process per session (tmux-server model)

`daslang.exe utils/dasHerd/ptyhost/main.das -- --session <id> --listen <ep>
--journal <dir>` — a minimal, boring, rarely-changing program that does
exactly three things:

1. **Owns the terminal.** Creates the ConPTY, the kill-on-close job, and
   the child (same dasTerminal externs the watcher uses today). The host is
   the one honest owner; host death still reaps the child tree — that
   guarantee moves, it does not disappear.
2. **Journals its own output.** Raw bytes append to a per-session journal
   on disk (ring + high-water) — replay has no gaps even while nobody is
   attached. The journal the watcher keeps today moves into the host.
3. **Serves a versioned IPC surface.** hello{version, session, token} /
   attach{from_byte} → replay + live tail / input{bytes} / resize{cols,rows}
   / stat / terminate{reason}. Nothing else. Protocol v1 is forever — a new
   watcher must always speak old hosts.

Spawned **detached**: `CREATE_NO_WINDOW` (NOT `DETACHED_PROCESS` — console
apps such as PowerShell break with no console at all; a hidden conhost of
its own keeps std handles working AND survives the caller) plus
`CREATE_BREAKAWAY_FROM_JOB` and a new process group. No inherited console,
no inherited job, no parent-death ties. The watcher becomes a *client* of
the host.

Two job-object facts learned live: the spawn falls back to no-breakaway
with a reported warning when the caller's job forbids it — treat that
warning as fatal for host spawns, the host would die with the job. And
daslang's own `popen_argv` job needed `JOB_OBJECT_LIMIT_BREAKAWAY_OK`
added (src/builtin/module_builtin_fio.cpp) so a popen descendant that
explicitly requests breakaway can leave the kill-on-close subtree — that
is what lets the lifecycle test (dastest → popen launcher → detached
host) prove spawner-death survival.

## Discovery and adoption

- On start the host atomically writes `logs/dasHerd/ptyhosts/<session>.json`
  — endpoint, token, host pid, child pid, started stamp.
- On child exit it rewrites the stamp with exit_code + drained, lingers a
  configurable grace for late replay attaches, then exits.
- Watcher startup: scan stamps, connect + stat each, adopt live hosts (the
  registry session resurrects as **running**, not `watcher_restart`), fold
  dead hosts' exit stamps into the registry, then archive their stamps.

## What changes where

- **watcher_core launch path** — spawns a host and connects, instead of
  calling ConPtyProcess directly. The per-session read pump becomes an IPC
  tail. Resize/input/terminate proxy through the host connection.
- **Raw history / replay** — served from the host journal (watcher proxies;
  checkpoint/restore byte offsets keep working, same numbers).
- **herd_tick observe-exit** — from host stat + exit stamps instead of
  process handles.
- **Rich client** — unchanged in v1 (still talks to the watcher). Direct
  client→host attach is a natural v2, and is also the bridge to external
  sessions (an external session is just a host the watcher never spawned).

## Deployment: `daspkg release` (Boris's call, 2026-07-25)

The host ships as a **released bundle**, not a script on the dev runtime:
`daspkg release` produces `<bundle>/dasherd-ptyhost.exe` via `daslang -exe`
(AOT standalone) plus only the `.shared_module` dylibs it transitively
requires (dasTerminal, dasHV), copied into the bundle and resolved
exe-relative. Consequences, all load-bearing:

- **No dependency on `bin/Release`** — a running host locks only its own
  bundle's files; rebuilding the tree can never LNK1104 against a live
  session (today's failure mode, eliminated by construction).
- **Versioned by directory** — one bundle per host protocol version; the
  watcher spawns from the bundle matching the version it speaks; running
  hosts keep their bundle until their session ends; upgrades lay down a new
  bundle beside the old.
- **AOT-clean requirement** — the host script and its requires must pass
  the `-exe` gate (no `options no_aot` anywhere in its graph); one more
  reason the host stays tiny and boring.
- `.das_package` under `utils/dasHerd/ptyhost/` declares `release_main`,
  `release_name("dasherd-ptyhost")`, and the forced shared modules.

## Failure matrix

| dies | effect |
|---|---|
| watcher | nothing; hosts keep running; next watcher adopts |
| rich client | nothing |
| host | that one session dies (host is minimal on purpose); watcher sees the drop + stamp |
| machine | sessions die; registry resurrect via `--continue` — the crutch's only remaining job |

## Decision points for Boris

1. **Transport**: dashv localhost TCP + WS framing (in-tree today, same
   token model as the watcher) vs named pipes (more Windows-native, new
   plumbing). Draft assumes dashv.
2. **Granularity**: per-session host (isolation, trivial lifecycle;
   recommended) vs one host for all sessions (fewer processes, but one
   upgrade unit owning every terminal).
3. **Linger** after child exit (draft: 24h or until adopted+drained).
4. Whether v1 already lets the rich client attach straight to a host.
