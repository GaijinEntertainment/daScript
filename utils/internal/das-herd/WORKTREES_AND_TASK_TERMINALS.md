# Worktrees and task terminals

Status: discussion draft

This is a focused design document for the first dasHerd workspace application.
It is intentionally separate from `PLAN.md`: the worktree, Git, task-terminal,
and automation contracts have enough nuance that they must be discussed one
section at a time.

No section below authorizes implementation merely by appearing here. Discuss
and settle each section before implementing the stage that depends on it.

## Established direction

- A detachable command host is the foundation beneath dasHerd. Callers do not
  own launched OS processes directly: JIT invocations, Git commands, setup
  scripts, shells, tests, and agents all execute as hosted command sessions.
- Every hosted command session has process state, a terminal, and a durable
  log whether or not its terminal is currently visible. The UI and automation
  attach to the session; detaching or closing the UI does not stop it.
- Git observation is asynchronous from the first implementation. No Git
  command runs on the UI/render thread.
- The application lists the main checkout and linked worktrees, and inspects
  the selected tree.
- The repository base and the main checkout are distinct. For example, a
  repository may use `origin/master` as its base while its main checkout is on
  a feature branch. The UI must show both values rather than label the main
  checkout "master".
- A worktree can have a PowerShell terminal rooted in that tree and later an
  agent session.
- Worktree creation supports a setup recipe. The built-in MCP bootstrap
  (`utils/mcp/setup.das`) is the default recipe; `None` and a user-specified
  recipe are alternatives.
- Removal is preceded by an explainable "is it safe to remove?" evaluation.
- Terminals are a general control and debugging surface. Git and setup work
  can have visible, colored, scrollable terminals whose output is also logged.
- Every terminal is addressable through live commands. A human, Codex, another
  agent, or later automation uses the same read/write terminal operations.
- Execution is not assumed to be local forever. A repository, worktree, Git
  process, setup task, shell, or agent may live behind SSH. The UI must always
  provide a route to the terminal that owns or can repair the operation.

## Section 0: detachable command host

Discussion status: foundational direction agreed; contract details open

The host is not an agent-specific service added late in the product. It is the
single execution building block used whenever dasHerd calls something. A JIT
compiler run, a Git query, a setup recipe, a test, an interactive PowerShell,
and a long-lived coding agent differ in profile and lifetime, not in whether
they receive a hosted session.

The caller supplies a target, working directory, structured command and
arguments, environment additions, terminal geometry, and retention policy.
The host owns:

- process creation and lifetime;
- the PTY and renderless terminal emulator;
- input serialization;
- terminal revisions, scrollback, title, CWD hints, and process status;
- an append-only session log and bounded retained terminal state;
- attach/detach and recovery after the UI disconnects;
- explicit termination and final exit status.

The same session remains inspectable while running and after exit. A terminal
window is one client of that state, not its owner. Live commands, the human UI,
and future agent routing attach through the same session contract.

Structured callers such as the Git observer still receive an explicit command
result and raw/captured output suitable for parsing. They do not reconstruct a
result by scraping rendered cells. Their hosted terminal and log remain
available for diagnostics even when the normal UI shows only structured state.

The host must survive dasHerd UI exit from its first useful version. The later
SSH design decides where the host process physically runs, but not whether the
session has this contract.

Questions to settle before implementation:

- Host topology: one OS host process per command session, a supervisor with
  child sessions, or a small supervisor plus isolated per-session hosts.
- The local attach protocol, discovery records, versioning, and authentication.
- Log format, rotation, crash consistency, terminal replay, and input-payload
  privacy.
- Whether very short structured commands allocate a PTY or use a capture mode
  that still exposes the same terminal/log/session API.
- Session retention after successful exit, failed exit, and explicit cleanup.
- What state can be reconstructed after the command host itself crashes; a PTY
  cannot be recovered if its owning process is gone.

### Chunk 0: measure terminal execution overhead

Discussion status: agreed

Before choosing a capture fast path or optimizing host topology, measure local
Git through the terminal stack that already exists. SSH Git is a separate Git
installation and execution target; it gets the same probe after SSH exists.

Compare the same forced-color Git argv through:

1. direct process capture;
2. direct process capture plus a raw durable log;
3. ConPTY, terminal emulation, and a raw durable log.

The probe records launch-to-first-output, launch-to-exit, final-output drain,
terminal poll/feed time, log-write time, byte count, and exit code. It separates
the cold first run from warmed median and p95 samples. The command set contains
a process-launch baseline, a representative colored repository status, and an
intentional failure to prove nonzero exit propagation.

One windowed run displays the real terminal with color and scrolling while
retaining its raw log and final exit code. Repeated timing runs are headless.
This first comparison measures in-process ConPTY/emulator/log overhead; the
same probe is rerun through detach/IPC after the minimal host exists.

No capture-mode or short-command optimization is selected until these numbers
exist.

The first measurement found raw logging indistinguishable from noise and
in-process terminal overhead of roughly 5-16 ms at process exit. dasHerd keeps
one terminal execution path; it does not add a direct-capture fast path. The
conservative post-exit drain must not block result delivery or scheduling.

### Chunk 1: measure an isolated session process

Discussion status: agreed; first measurement complete

The first process-isolation probe launches a fresh interpreted daScript worker
for every session. The worker owns ConPTY, terminal emulation, and logging and
acknowledges readiness over a pipe. This deliberately simple prototype measures
the complete extra process and runtime boundary before it becomes architecture.

The fresh worker added roughly 334-345 ms at median and peaked near 71 MiB
working set in one status sample. Almost all latency occurred before the worker
ready acknowledgement; terminal execution inside the worker continued to match
the in-process lane. Therefore an interpreted worker compiled afresh per Git
query is rejected. Per-session isolation itself remains open pending the same
probe with a small AOT/native host; a persistent per-worktree host is the next
fallback, not an unmeasured optimization.

### Chunk 2: smallest persistent watcher

Discussion status: first local slice implemented

One persistent JIT watcher owns multiple PTYs and terminal emulators. Browser,
UI, live-command, and future agent surfaces are clients of this process; they
do not own its child processes. The first client is a minimal browser control
panel. The local protocol is authenticated HTTP plus WebSocket on
`127.0.0.1` only.

The first API provides health and session listing, launch, UTF-8 input, resize,
termination, attach/detach, snapshots, and controller heartbeats. Many clients
may observe a session, but only one client holds its controller lease. Browser
close, explicit detach, and heartbeat expiry release that lease without
terminating the child. Polling is bounded per session on each watcher tick so
one noisy PTY cannot create an unbounded drain loop.

Each session writes the following under
`logs/dasHerd/watcher/sessions/<session-id>/`:

- `session.json`: immutable launch request;
- `output.raw`: exact PTY output bytes, flushed as they arrive;
- `events.jsonl`: lifecycle, timeout, exit, drain, and controller events.

Input payload contents are deliberately absent from the event log. An optional
external wall deadline terminates hung children and retains `wall_timeout` as
the final reason. Process exit is reported immediately; raw-output drain uses
the existing conservative 250 ms quiet interval without delaying the exit
observation.

Focused validation covers normal output and exit, wall timeout, exclusive
controller transfer, browser/WebSocket disconnect while the child continues,
heartbeat lease expiry, loopback token rejection, and watcher crash. A forced
watcher crash loses its live PTYs as expected, kills their children when the
ConPTY owner disappears, and leaves already-flushed raw logs readable. PTY
recovery after watcher death is explicitly not part of this slice.

Still deferred are remote authentication/pairing, TLS/LAN binding, SSH target
transport, styled browser terminal rendering, replay after watcher restart,
log rotation and retention policy, native dasHerd UI integration, and the
live-command terminal schema.

## Section 1: repository, base, and worktree identity

Discussion status: initial identity and configuration decisions settled

The first model needs to distinguish:

- an execution target, initially the local machine and later an SSH target;
- a repository identity on that target;
- the repository's configured or detected base ref;
- the main checkout, which is a special worktree but not necessarily the base
  branch;
- linked worktrees discovered from Git;
- worktrees created and managed by dasHerd;
- a stable session identity independent of a branch name.

A path alone is not a global identity once SSH exists. Candidate identities
therefore include at least `target ID + repository Git-common-dir` for a
repository and `repository ID + canonical worktree path` for an initial
worktree identity. Session records get their own generated IDs.

Repositories are registered explicitly by checkout path; dasHerd does not
recursively scan arbitrary directory trees. One global user configuration owns
per-repository records. A repository record may contain its display name,
target ID, checkout path, default/base ref, managed-worktree parent and naming
rule, setup trust and recipe selection, protected branches/removal policy, and
preferred Git remote. Targets themselves remain global. Repository-local
configuration is deferred; a future shared file must never execute setup merely
because a repository was opened.

The main checkout is the first, visually distinct worktree row. Linked
worktrees remain ordinary children of the registered repository. Interactive
sessions retain the worktree association chosen at launch even if their live
shell working directory later changes; current-directory observation is
separate from ownership.

Questions to settle:

- Are externally created worktrees fully manageable or initially read-only?
- Where are default managed worktrees placed, and how editable is that path?
- Whether a worktree needs an explicit base override in addition to the
  repository default.

## Section 2: asynchronous Git observation

Discussion status: first local slice implemented; SSH transport deferred

Background observation needs structured, non-interactive results. It submits
commands through the detachable host, uses Git's porcelain/plumbing output
without color, and publishes versioned snapshots to the UI. Its hosted
terminal and log remain addressable for diagnostics. The intended authority is
an unbounded machine-output sidecar; the bounded first slice below uses the
terminal model only with explicit overflow detection.

The initial observer covers:

- repository and Git-common-dir discovery;
- default-base detection, preferring `origin/HEAD` when available;
- main and linked worktree inventory;
- branch or detached HEAD and commit identity;
- staged, unstaged, untracked, renamed, and conflicted state;
- ahead/behind or containment relative to the selected base;
- locked and prunable worktree state;
- errors, last-success time, and whether a result is refreshing or stale.

The local Git is currently 2.17.1, so the implementation cannot require modern
options such as `git worktree list --porcelain -z` or
`git branch --show-current`. Managed paths can reject CR/LF, while spaces and
Unicode remain supported.

Worker rules:

- invoke Git through hosted sessions with argument arrays, never interpolated
  shell commands;
- send immutable/versioned results back to the UI thread;
- discard stale generations and coalesce redundant refresh requests;
- allow independent reads while serializing repository mutations;
- refresh after every mutation;
- do not fetch the network implicitly.

The first implementation registers explicit repositories in the global user
configuration, resolves the first Git 2.17 porcelain worktree as Main, and
runs inventory/status queries as watcher-owned structured-argv task sessions.
It publishes staged, unstaged, untracked, conflicted, ahead/behind, locked, and
prunable state plus filenames. Selecting a worktree requests an immediate local
refresh; no fetch is implied. ConPTY's raw bytes contain console-generated VT
cursor operations, so this slice parses the terminal model's reconstructed
240x512 capture screen and fails visibly if output reaches scrollback. Git log
and ref records use an explicit visible field delimiter because rendered tabs
become spaces. Observation sessions compact immediately after parsing. An
unbounded machine-output sidecar is a follow-up, not a reason to parse replay
bytes or truncated state.
Completed repository-refresh observation terminals compact to 160x50, while
file Diff/View task terminals compact to 80x25. A replacement refresh keeps only
the newest matching emulator in memory while preserving every older raw, event,
and launch log on disk; interactive sessions are unaffected.

Questions to settle:

- Refresh triggers and cadence for selected and unselected worktrees.
- Whether a manual refresh means local observation only, with a separate
  explicit Fetch action.
- How much per-command timing and argv detail is retained for the Git debug
  window.

## Section 3: execution targets and command sessions

Discussion status: partially settled

Git cannot be designed as "run a Windows process in this local directory."
Commands run against an execution target. The first target implementation is
local; an SSH target later provides the same operations:

- execute a structured non-interactive command and return a result;
- start or attach to a PTY terminal;
- identify a working directory on that target;
- observe process state and exit status;
- stream updates and retain diagnostics;
- terminate only through an explicit authorized action.

Target-specific path syntax stays behind this boundary. UI and persisted
records carry a target ID together with paths rather than treating local
absolute paths as universal.

The data model includes `target_id` from the first version, but the UI does not
offer inert SSH checkboxes or disabled remote controls. It shows the active
Local target in repository context. A target selector appears only when more
than one functional target exists; SSH will be a structured target containing
host, authentication, remote paths, shell, and capabilities rather than a
boolean session option.

The initial Commands menu has one immediate action: PowerShell in the selected
worktree, using `powershell.exe -NoLogo -NoProfile`. A successful launch opens,
selects, controls, and focuses that session's terminal. A later dockable Session
Launcher will customize shell or agent, new/resume mode, worktree, target,
arguments, environment, and command preview. `Codex` and `Codex resume` are
modes of that launcher, not separate proliferating profiles.

Questions to settle:

- How much of the target protocol is defined before implementing local-only
  execution?
- Whether the future SSH host runs a small dasHerd helper or begins with direct
  SSH command and PTY channels.
- Which environment, credential, and agent-command settings belong to a target
  profile.

## Section 4: Git and setup task terminals

Discussion status: initial dockspace and session behavior settled

User-initiated Git mutations, setup recipes, and repair work are activities
with terminals. Their terminal provides:

- live colored output through a PTY;
- normal terminal scrolling, selection, search, links, and zoom;
- retained logs and structured process status;
- input when credentials, confirmation, or repair is required;
- an obvious "go to terminal" action from the worktree and activity UI;
- a recovery shell in the same target and working directory when a task fails.

Short background observations remain structured jobs rather than flooding a
terminal. The Git debug UI can show their invocation, timing, exit status, and
bounded output, and can offer a terminal rerun or recovery shell. After any
interactive/mutating task, the structured observer refreshes Git state; the
colored terminal output is not parsed as truth.

Logs live in the dasHerd user-data `logs/` area with rotation. A completed task
terminal remains inspectable after its process exits. Long setup tasks should
survive closing the UI, which may pull the detachable session host earlier in
the implementation order than agent launching.

Questions to settle:

- Which successful task terminals close automatically, remain as history, or
  leave an interactive shell open?
- Whether a failed command automatically drops to a shell or offers an
  explicit Open recovery shell action.
- Log retention, rotation, and whether input payloads are logged. Input can
  contain secrets, so logging output and logging typed input are separate
  decisions.
- Whether every mutation must be visibly terminal-backed or whether very short
  non-interactive mutations may use the job path while retaining a one-click
  diagnostic terminal.

## Section 5: terminal identity and live-command control

Discussion status: open

Every terminal has a stable terminal/session ID independent of whether its
window is currently visible. Live commands must be able to:

- list terminals with target, worktree, task/session, process, and activity
  state;
- open or focus a terminal in the UI;
- inspect its bounded visible viewport and requested scrollback;
- read semantic text or changes since a known terminal revision;
- write UTF-8 text atomically;
- send normalized keys and paste operations;
- observe cursor, title, current-directory hint, exit state, and revisions.

The same operations serve the user, Codex controlling dasHerd, another agent,
and future automation. The PTY receives the same bytes regardless of source;
there is no weaker "automation terminal." The control plane may still record
origin metadata outside the byte stream for audit, loop prevention, and
permissions.

Input operations are serialized as complete events so a paste or prompt cannot
interleave byte-by-byte with human typing. This must be reconciled with the
single-input-lease rule for detachable sessions: live commands from the
lease-owning client are straightforward, while cross-client and cross-agent
writes need an explicit transfer, queue, or policy.

Questions to settle:

- The exact live-command names and bounded read/inspection schemas.
- Text, key, and paste operations versus an expert raw-byte operation.
- Whether writes require the input lease, can be queued for its owner, or can
  use separately granted automation capability.
- What provenance is retained without making terminal behavior differ by
  writer.

## Section 6: basic application layout and navigation

Discussion status: partially settled

The rich client is a real ImGui dockspace. Repositories/worktrees,
sessions/activity, Git status, settings, session launchers, diffs, and every
open terminal are independent dockable windows. Opening an item creates or
focuses its window; users may dock, float, tab, or arrange multiple instances
side by side. Closing a terminal window detaches its client and never
terminates the watcher-owned session.

The default layout is:

- a left repository/worktree rail with the main checkout visually distinct;
- Git status above an always-visible terminal on the right;
- a Git/tasks activity window containing running and retained jobs;
- status and failure affordances that always navigate to the relevant
  terminal or recovery terminal.

The top menu bar owns File, Commands, Edit, View, and Settings. View contains
zoom, the exited-session filter, and Reset Layout. Docking state and window
visibility persist in global user configuration; Reset Layout restores the
default arrangement. Exited sessions are retained but hidden by default. A
newly launched session becomes the active controlled terminal immediately.

Each worktree row eventually shows branch/HEAD, dirty/conflict state,
refresh/error state, setup state, and live terminal/agent activity. Switching
worktrees does not restart their terminals.

The Git/tasks window is a terminal when a task has a terminal, not a custom
plain-text log approximation. Structured Git observations may appear as debug
records beside the terminal activities.

### Worktree review perspectives

Git inspection is scoped to the selected worktree and the agent activity that
produced it. It is not a repository-wide staging dashboard. Three switchable
perspectives share one file list and the existing Diff/View inspector:

- **PR** answers what this worktree will send for review. Its included result
  is the aggregate change from the worktree's recorded merge base through the
  index: outgoing commits plus staged changes. Unstaged and untracked files
  remain visible first as subdued rows because they may be work an agent forgot
  to stage; color carries that state without a redundant Not Included label. A
  path with staged and later unstaged edits appears in
  both layers; the included row inspects the index result and the subdued row
  inspects the remaining working delta.
- **History** shows the selected worktree's current branch, newest-first commit
  descriptions, and the files belonging to the selected commit. A compact
  outgoing-commit list also appears above PR Total; selecting a commit narrows
  the file list, while PR Total restores the aggregate payload.
- **Tree** shows the repository commit graph and other local and remote branch
  labels for context. Remote labels are visible by default but visually
  secondary. Selecting a commit reuses the same file list and inspector.

The review base belongs to the worktree. Worktrees created by dasHerd retain
the base selected at creation. Externally discovered worktrees may infer an
initial base from their upstream and then `origin/master` or `origin/main`, but
the chosen value is visible, correctable, and never silently changed.

Initial staging controls operate on whole files. Stage and Unstage execute
immediately as watcher-hosted Git tasks, then asynchronously refresh the model;
hunk staging may be added without changing the file model. Greyed-out files
remain clickable. If any Git observation or mutation fails, its hosted Git session
automatically opens and receives focus in the Terminal window so a human or an
agent can inspect and repair the real command environment.

History and graph loading are bounded and incremental. The initial target is
200 commits with additional pages requested near the end, rather than an
unbounded UI-thread query.

The first vertical slice implements PR, History, and Tree as tabs over a shared
top file surface. Rows show exact added/deleted counts, style path components
and separators separately, keep local-only changes subdued and first, and
reveal stage/unstage controls only on hover. History and Tree share the same
selected-commit file result and revision-pinned Diff/View inspector. Tree reads
up to 200 commits across all refs and renders virtualized, stable-color parent
lanes, merge connectors, commit nodes, and local/remote ref labels; paging
beyond 200 commits remains the next graph-specific layer. Hovering a commit
reveals a copy control for its full SHA, establishing the context-icon pattern
that a future GitHub PR-number label will reuse. Git 2.17 cannot capture a blob
with `git show --output`,
so full staged/commit View runs a small daScript capture helper as a hosted task.
It uses argv-based Git execution and binary pipes, preserving image bytes
without shell redirection or quoting.

### Git file inspection workspace

The useful center of Git inspection is a selected file with two instantly
switchable projections, not a longer status table:

- **Diff** is a synchronized side-by-side comparison. Both sides remain plain
  source text and use extension-driven syntax highlighting for daScript, C,
  C++, Markdown, and other installed syntax providers.
- **View** shows the resulting file without diff furniture. Source files use
  the shared read-only source view with a thin changed-line gutter. Markdown
  uses the shared rich Markdown viewer; it is not flattened into code text.

File selection updates the inspector immediately. Loading is asynchronous and
must never block the UI thread; a new selection supersedes stale work without
requiring an Apply/Open button. The inspector is an independent dockable
window so status, file view/diff, and terminal can be arranged together.

Comparison scope is explicit and shares the same inspector:

- **Working** compares the index with unstaged and untracked content.
- **Staged** compares `HEAD` with the index.
- **PR** compares the selected worktree's merge base with its index, aggregating
  outgoing commits and staged changes rather than aliasing staged-only diff.
- **Commit** compares one observed commit with its first parent (including root
  and merge commits) and reads View from that exact revision.

The first file list is flat, grouped by conflicts, staged, modified, and
untracked, and shows the full repository-relative path. A tree projection may
be added as a switch over the same model; it must not become a separate data
path. Files which have both index and working-tree changes legitimately appear
in both comparison groups.

The document/source model owns exact UTF-8 bytes, syntax spans, aligned diff
rows, and changed-line marks. Rendering owns layout, synchronized scrolling,
selection, the narrow View gutter, and Diff line tint. This keeps live-command
inspection semantic and avoids screenshot-only validation.

The implemented working-tree path deliberately separates compact Diff from
complete View. Git emits three context lines into the hosted terminal, while
the resulting working file is read as exact bytes after the task succeeds.
Large unwrapped Views virtualize visible lines. Every compact, expanded, or
full Diff request now reconstructs the complete pre-change file from the
complete result plus its unified patch, runs syntax preparation on both
complete files, and projects those stable syntax roles onto the visible aligned
rows. Context switching therefore cannot reinterpret a truncated declaration,
comment, or scope. The current measured stress case is a
628 KiB/12,116-line file: Diff is ready in about 0.6 seconds, synchronized
scrolling is exact in both directions, and the prepared View renders at roughly
50 FPS at 4K/150% zoom. Syntax preparation remains off the UI thread and newest
request wins; the projection pass is linear in source spans and visible rows.

### Diff vertical slice

Discussion status: scope agreed; implementation in progress

Diff is the first inspector projection taken to near-production depth before
the rest of Git inspection expands. It establishes the reusable cost and test
baseline for later View, PR, history, and other asynchronous projections.
The slice includes failure, replacement, large-input, and binary behavior; it
is not complete merely when the happy-path text comparison renders.

The first complete Diff surface has:

- one vertical scroll position and one visible vertical scrollbar for both
  panes, including wheel, keyboard, live-command, and scrollbar-drag input;
- a draggable center splitter with a useful minimum width on both sides;
- absolute old/new line-number gutters, with absent lines rendered as gaps;
- retained hunk identity and previous/next-change navigation;
- compact, expanded-context, and full-file modes over the same aligned-row
  model;
- automatic syntax selection plus an explicit override for Plain, daScript,
  C, C++, Markdown, and JSON; compound suffixes such as `.md.local` resolve to
  the underlying Markdown language;
- Git-native binary detection and an image preview only when the bundled
  stb_image decoder accepts the bytes.

The bundled stb_image 2.30 build currently enables its stock PNG, JPEG, BMP,
TGA, PSD-composite, GIF-first-frame, HDR, PIC, and binary PNM decoders. This is
a capability boundary, not a promise to grow an extension list. WebP,
animation, and video are deferred; future video work belongs with dasVideo.
The watcher transports exact bytes, because a rich client must not assume it
can open a target-local path once execution targets include SSH. CPU decoding
may run on a worker; ImGui and graphics-resource creation remain on the UI
thread.

Each inspector projection exposes request state separately from retained
content. Request state is idle, queued, or running; content is absent, ready,
or failed. Starting a refresh does not erase the last successful content, and
a failed refresh may report its error while leaving that content inspectable.
Every request carries a monotonically increasing generation. The UI installs
only results matching the current generation, so selecting another file
supersedes work without waiting for or joining the old request.

One persistent rich-client worker initially services preparation jobs through
bounded/coalescing queues. It performs CPU-only parsing, syntax preparation,
and image decoding and catches failures into result events. The UI thread
drains those events once per frame. Watcher-hosted Git remains a separate
asynchronous stage, and any graphics upload is an explicit final UI-thread
stage. Shutdown closes the worker channel and releases captured channel
references; it must neither leak a thread nor stall client exit.

The dedicated fixture worktree covers at least staged and unstaged text,
untracked files, rename, deletion, conflicts where practical, Markdown and a
compound Markdown suffix, JSON, daScript, C/C++, a decodable image, an
unsupported binary, no-final-newline input, Unicode paths/content, separated
hunks, and a large file. Test coverage is counted in six groups: pure diff and
language models, deterministic worker-state transitions, widget/render
behavior, real watcher/Git integration, semantic live-command inspection, and
performance/regression probes. The resulting count and the defects these tests
catch become the estimating baseline for later projections.

Questions still to settle:

- How completed task terminals are grouped and pruned from the activity UI.
- Whether closing the last visible window for a running session should leave a
  persistent reopen affordance beyond the Sessions window.
- Exact PR/base discovery and multi-PR selection policy.
- Whether flat or tree file projection becomes the eventual default after both
  can be tested with large real changesets.

## Section 7: worktree creation and setup recipes

Discussion status: open

The initial creation flow collects a name, new branch, base ref, target path,
setup recipe, and post-create action. The default setup recipe is the MCP
worktree bootstrap; `None` and a user-entered recipe remain available.

Recipes are structured command profiles with an executable, argument array,
working directory, environment additions, and placeholders such as repository
root, worktree root, target, and bootstrap daslang. Repository-provided recipes
must not execute merely because a repository was opened.

Creation progresses through visible, retryable states:

1. validate branch, base, target, and path;
2. create the worktree;
3. publish refreshed structured state;
4. run the selected setup recipe in a task terminal;
5. preserve and clearly mark the tree if setup fails;
6. open a shell or launch the selected agent after success.

Once Git has created a real worktree, setup failure does not silently delete
it. The user can repair it in the task/recovery terminal, retry setup, keep the
tree, or proceed to the separately guarded removal flow.

Questions to settle:

- New branches only in v1, or attaching an existing branch too.
- Default worktree path and branch-name derivation.
- Recipe configuration location and first-use trust UI.
- Whether a shell may open while setup is still running.

## Section 8: removal safety and recovery

Discussion status: open

"Is it safe to remove?" produces `Safe`, `Needs review`, or `Blocked` with
specific evidence. The mutation worker revalidates immediately before removal
to avoid acting on a stale UI snapshot.

Initial evidence includes:

- main checkout and protected-target rules;
- locked/prunable state;
- dasHerd-owned terminals, agents, setup jobs, and mutations;
- staged, unstaged, untracked, and conflicted changes;
- branch containment relative to the selected base;
- whether the branch is checked out elsewhere;
- whether repository, target, and path still match the inspected identity.

Every blocking or review condition offers a route to the relevant terminal so
the user or an agent can inspect, commit, move, clean, or otherwise repair the
tree and ask again.

Initial removal uses `git worktree remove` without force. Worktree removal and
branch deletion are separate operations; branch deletion uses the safe Git
operation rather than forced deletion. A force-removal design, if added, is a
later discussion.

Questions to settle:

- Exact distinction between `Needs review` and `Blocked`.
- Whether unpushed and not-contained-in-base are separate warnings.
- What evidence is sufficient for remote targets whose connection is down.
- Whether removal of worktrees not created by dasHerd is permitted in v1.

## Section 9: agents, cross-agent communication, and self-prompting

Discussion status: open, future-facing

Agent integration should be built on terminal/session capabilities rather than
brand-specific screen scraping. A hosted PowerShell rooted in the worktree can
launch Codex, Claude Code, Pi, OpenCode, or a custom command. Closing the UI
must not stop the hosted terminal or agent.

Cross-agent communication is a routed terminal write to a stable session ID.
Self-prompting is the same operation with source and destination equal. Both
use the ordinary terminal read/write contract; neither requires a second
hidden agent-control protocol.

Terminal equivalence does not mean uncontrolled recursion. The control plane
can retain source session, destination session, event ID, parent event, and
timestamp outside the PTY stream. Later policy can use that provenance for
authorization, rate limits, hop/loop protection, and an inspectable audit
trail without changing what bytes the terminal receives.

Questions to settle later:

- Per-session permission to accept user, live-command, cross-agent, and
  self-prompt writes.
- Loop and runaway protection that remains visible and overridable.
- Delivery acknowledgement: accepted into the terminal input queue versus
  semantically acted upon by the agent.
- How an agent discovers addressable sessions without exposing unrelated
  repositories or targets.

## Tentative discussion and implementation order

This order is a discussion aid, not an approved implementation schedule:

1. Settle Section 0 and the core of Section 5: host, log, terminal, attach, and
   live-command contracts.
2. Implement and stress the minimal local detachable host before building
   worktree or agent policy on it.
3. Settle Sections 1-3: identity, asynchronous observation, execution target.
4. Implement the tested hosted Git observer and basic worktree/inspection app.
5. Settle Sections 4 and 6: task-terminal behavior and UI navigation.
6. Add worktree PowerShell terminals, Git activity/debug terminals, and the
   live-command terminal surface.
7. Settle and implement Section 7 creation/setup.
8. Settle and implement Section 8 safe removal/recovery.
9. Settle Section 9 before one-click durable agent launching and any
   cross-agent or self-prompt automation.
