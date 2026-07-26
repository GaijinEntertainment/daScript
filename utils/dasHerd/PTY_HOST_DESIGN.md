# Detached PTY host — "a terminal that depends on nothing"

Status: DRAFT for review (2026-07-25). The next block per Boris: drop
everything and make terminal sessions fully independent. `--continue`
resume demotes to disaster recovery, never the mechanism.

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

Spawned **detached**: `DETACHED_PROCESS` + breakaway from any inherited job
(`CREATE_BREAKAWAY_FROM_JOB`; if the spawning environment forbids breakaway,
spawn through a one-shot intermediary). No inherited console, no inherited
job, no parent-death ties. The watcher becomes a *client* of the host.

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

## Operational hard lesson folded in

Hosts must NOT run from `bin/Release` — today's builds died on LNK1104 file
locks from live processes, and a rebuild must never collide with a running
session. Each host launches from a **copied runtime snapshot** (version-
stamped dir of daslang.exe + DLLs + the host script); upgrades lay down a
new snapshot and never touch a running one.

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
