# Detached PTY host — "a terminal that depends on nothing"

Status: LANDED (2026-07-26) — host core AND the watcher rework. Durable
herd sessions launch via detached hosts, watcher restart adopts them back
as RUNNING (proven by test_watcher_adoption.das). `--continue` resume is
disaster recovery only (machine death).

## Rig / branch state

- Watcher and rich client are STOPPED (Boris's go). Release bundle at
  logs/dasHerd/releases/dasherd-ptyhost is current (rebuild:
  `daslang utils/daspkg/main.das -- release --out logs/dasHerd/releases
  --root utils/dasHerd/ptyhost`, 7s).
- Standing rules: file Boris's bug reports BEFORE fixing; never restart a
  watcher with live agent sessions without his go; preflight budget for
  this branch: at most one full run (none used); branch
  codex/herder-view-without-diff, unpushed commits, PR on his word.
  Parked after this block: external sessions arc, notes 42/47 terminal
  rendering bugs, WebSocketServer handle leak at client exit.

## Landed decisions (2026-07-26, the watcher rework)

- **Host-backed iff `herd_session_id` is set** (and a ptyhost root is
  configured): durable sessions get hosts; plumbing launches (git captures,
  task terminals) stay in-process ConPTY — survival there is pointless
  overhead.
- **Token rides the discovery stamp** — adoption after restart must
  authenticate, and the stamp is the discovery record. Same-user local
  file; same trust model as the watcher token on its own stdout.
- **Host pushes a stat on exit/drain transitions** (v1-compatible; the
  watcher still polls at 500ms as backstop).
- **The host journal IS the session's raw output** — host-backed sessions
  write no watcher-side output.raw; replay offsets equal journal offsets.
- **ConPTY never signals pipe closure while the host holds the HPCON**
  (observed live: exited=true, drained=false for minutes). Hence: the
  watcher folds exit via the direct path's quiet interval, and the host's
  linger gate keys on exit alone.
- **Host release + archive**: once the exit folds and the journal is fully
  replicated, the watcher sends `shutdown`; when the host obliges (link
  closes), stamp+journal move to `ptyhosts/archive/`. Stamps of hosts that
  died with no watcher fold at the next adoption scan.
- **Watcher-side adoption restores mailbox/bundles/events** from the
  session-dir jsonl records (newest-per-id wins; id counters bumped past
  restored maxima).
- Deferred hardening (unchanged from before): agent env still rides the
  powershell wrapper argv, so DASHERD_TOKEN is visible in the host's
  command line exactly as it was in the ConPTY child's; the host's own
  env also leaks DASHERD_HOST_TOKEN to the child. Move to the host's
  DASHERD_CHILD_* env forwarding later, way later.

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

## What changed where (landed 2026-07-26)

- **watcher_core launch path** — for herd sessions, spawns a host
  (`spawn_detached`, port scan 9700+ with bind-failure retry via the
  stamp) and connects as a WS client; the per-session read pump is an IPC
  tail feeding a transport-less local `Terminal` screen model
  (`terminal_feed_bytes`; parser auto-replies forwarded back as input).
  Resize/input/terminate proxy through the host connection.
- **Raw history / replay** — the ring is fed from the host stream;
  checkpoint/restore byte offsets equal journal offsets, same numbers.
- **observe-exit** — from host stat (pushed on transitions + 500ms poll);
  drain folds on the quiet interval since ConPTY never closes the pipe
  under a live HPCON.
- **Adoption on startup** — `watcher_adopt_hosts()` before `herd_init()`:
  live hosts re-attach (registry records stay **running**); dead hosts
  fold their exit stamp into an exited, replayable session and archive.
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

## Remote boxes: watcher-per-box (decided 2026-07-26)

The ssh story is topology, not transport, and the deciding weight is GIT: a
local watcher reaching out to remote hosts would drag every git surface
(changelist, history, captures, inspection, worktree ops, path identity)
through ssh. Instead we copy tmux: **one watcher per box**, its sessions,
hosts, git layer, registry, and mailbox all box-local and unchanged; the
rich client federates.

- **The client owns a watcher list** — each entry: name, how to reach it
  (local port, or an ssh target yielding a forwarded loopback port), token
  source. A remote watcher behind a forwarded port is indistinguishable
  from the local one: same protocol, same reattach.
- **ssh has exactly two jobs**, both boring: the tunnel (no watcher port is
  ever exposed; tokens stay loopback secrets on both ends) and the control
  channel (deploy bundle, start/stop/upgrade the watcher, fetch its token).
  Remote watcher restarts are safe because their sessions sit in detached
  hosts — the terminal arc was the prerequisite.
- **ssh drop = client reconnect with backoff**, nothing more. Sessions,
  hosts, git, mailbox keep running remotely (tmux model: the server is
  remote, the client is disposable).
- **Agent MCP stays box-local**: DASHERD_URL points at that box's watcher
  over loopback. No reverse tunnels.
- This topology keeps watcher<->host loopback-only forever, which is what
  makes the pump's "link closed = host died" assumption permanently valid.
- Deferred consciously: cross-box sessions (satellites/bundles spanning
  boxes) — the federation seam lives in the client, later.

## Hardening block (LANDED 2026-07-26)

- **Host log file** `<session>.log` beside the journal (the host runs
  CREATE_NO_WINDOW — stdout evaporates; this file is the only witness).
  Logs start config, lock acquisition, child launch/exit, every
  connect/auth/attach with offsets, every op except the 500ms stat poll,
  drain/linger transitions, and the exit summary. Archived with the
  journal.
- **Lock-file liveness** `<session>.lock`, held open for the host's
  lifetime. Windows CRT handles carry no FILE_SHARE_DELETE, so `remove()`
  fails while the host lives and succeeds the instant it dies — crash
  included; removing a dead host's lock IS the cleanup. Adoption probes
  the lock before connecting (dead-fold dropped ~2s → ~20ms); the connect
  timeout stays as the alive-but-wedged fallback. Clean exit deletes the
  lock; the archive sweep removes crash leftovers.
- **Test coverage landed**: bad token / wrong protocol / pre-auth ops
  rejected (host-side), lock held-while-alive + released-on-exit, log
  content, corrupt-jsonl restore lines skipped, host death under a live
  watcher fails that one session only (`host_disconnected`), bind
  collision retries onto the next port. Suite 73/73.
- **Not covered, documented**: port-exhaustion (impractical to occupy 200
  ports) and degraded job breakaway (popen jobs now permit breakaway, so
  denial cannot be simulated in-tree); both fail loudly at launch by
  construction.
