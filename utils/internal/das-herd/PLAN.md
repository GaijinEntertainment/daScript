# dasHerd

Status: working design

`dasHerd` is a local control plane for coding-agent workspaces. It launches and
reattaches to agent sessions, gives every session an owned worktree, presents
the complete state of that worktree, and unifies local Git and GitHub review
workflows.

The name is intentional: this is an agent herder, not a general-purpose IDE.
The initial implementation may force a narrow workflow when that makes state
unambiguous and reliable.

The proposed ordering and shared interaction contract for agent-assisted local
review and the later GitHub PR surface are recorded in
[`AGENT_REVIEW_WORKFLOWS.md`](AGENT_REVIEW_WORKFLOWS.md).

## Product contract

- One writable agent session owns one worktree.
- dasHerd creates the worktree, branch, session, and agent process together.
- The main checkout can be adopted as an explicit special workspace.
- Relative paths emitted by an agent resolve against its session worktree.
- Closing or restarting the dasHerd UI does not stop attached agents.
- Git, process, and GitHub state are observed independently; terminal text is
  never the sole source of truth.
- Source positions use workspace identity, worktree-relative path, revision,
  and UTF-8 byte or line range. Screen coordinates are never persisted.
- Direct editing starts with the PR body in Markdown. Code views are initially
  read-only, selectable, and addressable.
- Accessibility is a default, not a preference hidden in settings: terminal
  text starts at 150%, exposes a visible bottom 50-300% zoom control plus
  Ctrl+wheel in 5% steps, provides Reset in the View menu, and the desktop
  shell opens maximized.
- Terminal history is retained and searchable. Search covers the complete
  retained primary-screen scrollback, not only currently visible rows.
- Copy, paste, select-all, and clear-selection are semantic terminal actions
  reachable from the bottom Edit menu as well as keyboard bindings.
- Any hovered token that a host resolver accepts is actionable even when it
  is not pre-highlighted. OSC 8 and URI syntax are built-in resolvers;
  workspace filenames and custom schemes are host-provided resolvers.
- A protocol-requested blinking cursor actually blinks while the terminal is
  focused; its style, visibility, and blink phase are observable to UI tests.
- The default terminal font stack covers common scripts, symbols, CJK, and
  emoji through platform fallbacks while terminal cell widths remain fixed.
  Bidirectional layout and complex-script shaping are tracked separately from
  glyph availability.

## First vertical slice: terminal

The terminal is the first subsystem because it proves the hardest lifetime
contract and becomes the spine of every later workspace feature.

The existing dasImgui text stack already provides much of the presentation
foundation: grapheme grouping, 0/1/2-cell widths, selection, clipboard, font
roles, URI recognition, wrapping, source ranges, and deterministic interaction
tests. The substantial missing pieces are terminal emulation, pseudoterminal
ownership, durable session hosting, and terminal-specific input modes.

The split is strict:

1. `modules/dasTerminal` is renderless. It owns terminal protocol and state.
2. `modules/dasImgui/terminal` renders and interacts with terminal snapshots.
3. `utils/internal/das-herd` owns workspace policy, session discovery, and application UI.

This follows the shared text architecture recorded in
[`modules/dasImgui/plans/text-components-architecture.md`](../../modules/dasImgui/plans/text-components-architecture.md).

## Architecture

```text
dasHerd UI
   |
   | local IPC: attach, input lease, resize, snapshots, updates
   v
per-session host process
   |
   | ConPTY on Windows; PTY on POSIX
   v
agent process: Codex / Claude Code / Pi / OpenCode / shell

per-session host
   +-- dasTerminal emulator and scrollback
   +-- child/PTY lifetime and exit status
   +-- worktree/session metadata
   +-- versioned attach endpoint
   +-- bounded transcript and diagnostics
```

The UI never owns the agent process or pseudoterminal. Detaching closes only
the IPC connection. A session host stays alive until the agent exits or the
user explicitly terminates the session.

One host process per session provides fault isolation. dasHerd discovers hosts
through persisted session records and verifies them by connecting to their IPC
endpoints. A later supervisor may monitor and restart hosts, but no design may
claim that a live PTY can be recovered after its owning host has died.

Only one attached client holds the input lease. Additional clients may observe
the same session read-only. Lease transfer is explicit so two windows cannot
interleave keystrokes.

## `dasTerminal` responsibilities

The renderless module owns:

- a terminal screen grid, primary and alternate screens, cursor, saved cursor,
  tab stops, margins, modes, and bounded scrollback;
- incremental VT/ANSI parsing across arbitrarily split byte chunks;
- UTF-8 decoding, combining graphemes, wide cells, and continuation cells;
- SGR attributes and indexed/true-color values without UI colors or fonts;
- OSC metadata, including OSC 7 current-directory hints and OSC 8 hyperlinks;
- resize/reflow policy and terminal buffer revisions;
- semantic selection/copy queries over terminal cells;
- normalized input operations for keys, UTF-8 text, paste, focus, and mouse;
- a platform-neutral pseudoterminal/session interface;
- ConPTY and POSIX PTY implementations behind that interface.

It does not know about ImGui, pixels, textures, worktrees, GitHub, or agent
brands.

PTY transport and VT emulation remain separate interfaces so the engine can be
replaced without disturbing the renderer or dasHerd. T0 starts with a small
in-tree state machine behind the canonical snapshot contract. This is not a
commitment to implement every terminal escape ourselves: the conformance
corpus is the decision instrument. If the required compatibility surface grows
faster than the engine, evaluate `libghostty-vt`, `libtsm`, and `libvterm`
behind the same tests.

The initial dependency decision favors the in-tree slice because it gives us:

- one exact snapshot and future IPC schema rather than adapting a library's
  renderer-shaped callbacks;
- C++11, MSVC, and daScript bindings with no new build or license surface;
- explicit unknown-sequence diagnostics instead of silently accepting an
  unmeasured compatibility claim;
- a measurable point at which adopting a proven engine becomes cheaper.

The first research-derived test sources are:

- [Termless](https://termless.dev/) for structured cross-backend semantic test
  shape;
- [terminfo.dev](https://terminfo.dev/) for a machine-readable capability and
  probe catalog (author original vectors or preserve its CC BY attribution);
- [xterm.js escape fixtures](https://github.com/xtermjs/xterm.js/tree/master/test/fixtures/escape_sequence_files)
  for replay streams, with the limitation that its text goldens omit styles,
  cursor, and modes;
- [Charmbracelet `x/vttest`](https://pkg.go.dev/github.com/charmbracelet/x/vttest)
  for the closest precedent to the canonical rich snapshot schema;
- `esctest2`, classic `vttest`, Ghostty fuzz seeds, Microsoft Terminal adapter
  tests, and Neovim/libvterm tests as external oracles and robustness inputs,
  not copied test code.

## dasImgui terminal responsibilities

The dasImgui counterpart owns:

- measuring and drawing a terminal snapshot with the shared monospace/font
  stack;
- cursor, selection, hover, search highlights, and visible scrollback;
- keyboard, text, paste, focus, mouse-mode, wheel, and resize input mapping;
- color-theme mapping from terminal attributes to ImGui colors;
- OSC 8 link hit testing;
- fallback URI recognition through the existing `text_uris` service;
- context actions such as copy, open link, copy link, and create source anchor;
- stable screen geometry for automation and deterministic headless tests.

Terminal cells remain the authority for layout. Font fallback and color emoji
are drawing concerns inside allocated cells and may not move later columns.

## Links and source anchors

Links have three levels:

1. OSC 8 hyperlinks are authoritative when emitted by the child process.
2. URI-shaped text is recognized by the shared `text_uris` fallback.
3. dasHerd recognizes workspace file references such as `foo.das:42` or
   `/examples/foo/bar.das` and resolves them against the session worktree.

The terminal renderer reports a clicked payload and terminal location; dasHerd
decides whether to open a browser, a source view, a Markdown view, or another
workspace action.

Agent skills should eventually ask supported agents to emit canonical
worktree-relative file references or structured hyperlinks. The tool may also
inject a selected source anchor into the terminal input as a normal agent
prompt. Structured hints improve the experience but never replace observable
Git/worktree state.

## Session record

Each persisted session record contains at least:

- stable session ID and display name;
- agent profile and launch command;
- repository and worktree roots;
- branch and intended base branch;
- host PID, IPC endpoint, creation time, and last observed state;
- terminal dimensions and scrollback policy;
- optional PR repository/number;
- paths to bounded transcript and diagnostic logs.

Runtime state belongs in a user-data location, not in a deployment directory
or the repository. Diagnostic logs use a dedicated `logs/` directory.

## Demonstrable milestones

### T0: emulator contract and fixtures (in progress)

- Define renderless cell, line, cursor, style, snapshot, update, and revision
  contracts.
- Select or reject candidate VT engines with a small written rationale.
- Feed chunked deterministic fixtures for text, color, cursor movement,
  alternate screen, clear operations, OSC 7, OSC 8, Unicode, and resizing.

The canonical test surface is:

```text
create(columns, rows)
feed(bytes)
resize(columns, rows)
encode_key(event)
encode_paste(text)
send_mouse(event)
snapshot()
drain_replies()
take_events()
```

The first implementation already covers create/feed/resize/snapshot/replies/
events and tests one-shot, byte-at-a-time, and seeded random chunk equivalence.
The semantic table pins the selected Core cursor, erase, and SGR controls,
including wide-cell invariants when erasing or overwriting a continuation cell.
Normalized key and paste encoding is mode-sensitive and covered in both C++ and
daScript. Mouse encoding joins the contract with the renderer/ConPTY slice.

### T1: local interactive terminal (in progress)

- Launch PowerShell through ConPTY on Windows. (implemented and covered by a
  semantic-handshake process test; `pwsh` is preferred with Windows PowerShell
  fallback)
- Render it through the dasImgui terminal view. (implemented as
  `imgui/imgui_terminal` with a runnable headless/windowed PowerShell example)
- Support typing, paste, resize, selection, clipboard, colors, cursor, and
  scrollback. (first interactive slice implemented; deterministic interaction
  now covers measured resize, click-to-focus, synthesized typing, and semantic
  screen output; paste/scrollback/selection/clipboard coverage and
  terminal-specific selection polish remain)
- Run a full-screen agent TUI rather than validating only line-oriented shells.
- Add retained-history search with next/previous result navigation and visible
  match counts.
- Add the resolver-driven hover/click contract for OSC 8, fallback URIs, and
  worktree filenames.
- Add record-once/replay-twice semantic comparison against `@xterm/headless`;
  use synchronized side-by-side rendering only as the human inspection layer.

### T2: attach and detach

- Move PTY and emulator ownership into a per-session host executable.
- Attach the dasHerd UI over local IPC.
- Close the UI while the agent continues working, reopen it, and recover the
  exact current screen plus scrollback.
- Enforce and visibly transfer the single input lease.

### T3: links and source handoff

- Activate OSC 8 and fallback URI links.
- Resolve worktree-relative file references.
- Open a full source or Markdown view at the referenced range.
- Select a source range and send its canonical anchor to the active agent.

### T4: first dasHerd workspace shell

- List sessions with agent, worktree, branch, dirty state, process state, and
  last activity.
- Create a worktree and launch an agent from a forced profile.
- Switch between terminal and file inspection without losing context.
- Show changed files, full files, working diff, and aggregate base diff.

### T5: local PR workflow

- Associate or create a PR for the workspace.
- Edit a locally autosaved Markdown PR-body draft with explicit publish.
- Detect remote-body conflicts before publishing.
- Show CI, review threads, and Copilot activity.
- Support reply, resolve, and deliberate batch resolve operations.

## Testing rules

- VT parsing tests split every interesting sequence at multiple byte
  boundaries; correctness cannot depend on read chunking.
- Emulator tests are renderless and use exact snapshots/revisions.
- PTY process tests use deterministic child probes and explicit handshakes.
- UI tests wait for terminal buffer, layout, hover, selection, and input
  revisions. Timeouts are deadlock guards, never test semantics.
- Attach/detach tests prove the child PID survives UI exit.
- Tests cover non-ASCII text, combining graphemes, CJK, emoji, CR/LF variants,
  bracketed paste, alternate screen, and resize during output.
- Logs and abandoned probe artifacts are cleaned when work completes.

## Performance and stress gate

Terminal performance is a product contract. A terminal that is semantically
correct but drops interaction below real-time under ordinary agent output is not
complete. JIT is an eventual deployment tool, not permission to keep work
proportional to retained history or to rebuild unchanged state every frame.

Measure interpreter and JIT lanes separately and report cold start separately
from warmed steady state. Every performance report must attribute at least PTY
poll/feed, emulator mutation, C++ snapshot/copy, daScript projection, layout and
input, draw-list emission, and total frame time. The live inspection surface
must expose those timings together with bytes read and cells visited, copied,
and drawn so screenshots and subjective feel are never the only evidence.

The stress matrix covers:

- 80x24, 160x50, and 240x80 grids at 0, 1,000, and 10,000 retained rows;
- idle cursor blink, continuous ASCII, dense SGR/color changes, alternate-screen
  redraw, combining/CJK/emoji text, resize during output, selection, and search;
- 1,000-line interactive bursts plus sustained and bursty multi-megabyte output;
- bottom-follow and scrolled-back viewports, including input while output is
  still arriving.

Initial acceptance budgets on the reference workstation are:

- retained history changes warmed idle/render cost by no more than 10%; only the
  visible viewport and bounded inspection request may be copied or traversed;
- the full UI remains at or above 60 FPS (16.7 ms p95 frame time) with 10,000
  retained rows, with no output-burst frame longer than 50 ms;
- focused input reaches the PTY and its echo becomes visible within 50 ms p95
  while output is active;
- benchmarks pin emulator feed throughput, viewport snapshot cost, allocation
  counts, and interpreter-versus-JIT deltas before optimization claims land.

The first measured 78x26 interpreter/live baseline on 2026-07-19 demonstrated
the current failure clearly: warmed idle with zero history held 120 FPS / 8.3
ms, 978 retained rows fell to 18.4 FPS / about 54 ms, and 4,980 rows fell to
about 3.1 FPS / 332 ms. The immediate cause is repeated deep copies of both
buffers plus full-history daScript projection and traversal on every frame.

The viewport-bounded adapter pass on the same day removed history from the
frame path. Viewport projection remained between 0.94 and 1.08 ms in the
interpreter and 0.36 and 0.37 ms under JIT from zero through 9,975 retained
rows, with a flat 147,456 bytes and 2,028 string allocations per 78x26
projection. The live 78x26 UI then held about 120 FPS at 978, 4,980, and 9,982
retained rows, including a viewport scrolled back 300 rows and new output while
scrolled. The explicit compatibility snapshot remains history-proportional and
cost 407.7 ms interpreted / 164.5 ms JIT at 9,975 rows, so it is reserved for
explicit whole-history operations and must not return to rendering.

A persistent renderer cache now keys that bounded projection by terminal
revision and scroll offset, so unchanged blink/idle frames reuse cells and
strings without polluting the concise inspectable state. A single 10,000-line
live burst stayed around 86.5 FPS while output was still arriving, returned to
about 120 FPS after settlement, and held about 109 FPS while scrolled back 300
rows at 9,933 retained rows.

The compact native paint pass on 2026-07-19 removed ordinary cells from the
projection entirely, emitted geometry-safe ASCII as color-grouped text runs,
and reduced selection to one rectangle per intersected row. The 78x26 viewport
benchmark fell to 0.058-0.063 ms and 1,536 bytes / 2 allocations at 1,000-
10,000 retained rows in both interpreter and JIT lanes. On the maximized
318x69 live grid, a 10,000-line stream with a color change on every glyph held
about 119-121 FPS while output was active: compact cell projection was about
0.07 ms, native run projection about 0.8 ms, and text submission about 0.6 ms.
The same lane measured about 70 ms / 13 FPS before batching and about 18 ms /
48 FPS after run batching but before compact cell projection. Selection across
nine visible rows and the Unicode/background fallback lane both remained near
120 FPS, with zero ordinary paint cells and only the exceptional cells copied.

## Structured UI inspection gate

Terminal and rich-text correctness must be inspectable through live commands;
`view_image` and screenshots are never the verdict. Screenshots remain useful
only for the human before/after story.

The terminal inspection surface must expose, for the visible viewport and a
bounded requested history range:

- each drawn cell's row, column, grapheme, terminal width, foreground,
  background, attributes, selection/search state, and screen rectangle;
- the actual cursor rectangle, style, visibility, blink request, and current
  drawn blink phase;
- viewport/scrollback mapping, clip rectangle, hover cell, and active resolved
  OSC 8/URI/file payload;
- the requested font role, actual fallback source used for each glyph run,
  glyph availability, and an explicit missing-glyph marker;
- stable content/layout/paint/hover revisions so tests can wait for effects
  rather than frames or sleeps.

`imgui_snapshot` should carry a concise terminal summary. A dedicated bounded
live command may return the complete cell/run detail so large histories do not
make every ordinary snapshot enormous.

Rich-text and source views need the same generic contract over visible runs:
display text, source byte range, semantic/style IDs, resolved color, requested
and actual font role/source, glyph availability, screen rectangle, clipping,
selection, hover, link/control payloads, and revisions. The Markdown viewer's
current example-specific `markdown_probe_state` and `markdown_probe_fragment`
commands cover useful geometry and interaction state, but not final glyph/font
fallback or paint state; the shared source view has no equivalent generic live
inspection surface yet.

No terminal, Markdown, source-view, or future editor rendering fix is complete
until the wrong value is reproducible and the corrected value is readable from
this structured inspection layer. The oracle harness validates terminal
semantics; live inspection independently validates how those semantics were
painted and made interactive.

## Explicit non-goals for the first slice

- SSH sessions; the protocol must permit a remote host in v2, but v1 is local.
- A general code editor or language server client.
- Per-agent authorship inside a shared worktree.
- Recovering a live terminal after the session-host process itself dies.
- Inferring authoritative agent state by scraping prose from terminal output.
- Implementing every obscure VT escape before Codex/Claude/shell workflows are
  demonstrably correct.

## Immediate next step

Continue T1 from the working local terminal:

1. complete the remaining resize/alternate-screen stress lanes and pin p95 PTY
   feed, input-to-echo, and whole-frame timing; compact viewport projection and
   batched glyph emission now clear the 120 Hz dense-SGR lane;
2. finish structured terminal-cell and rich-text-run inspection through live
   commands, including actual font fallback and missing-glyph reporting;
3. finish the bottom Edit/View menus, 5% zoom, cursor blink, and Unicode font
   fallback with structured snapshot coverage;
4. extend deterministic headless interaction tests to paste, scrollback,
   selection, clipboard, retained-history search, and resolver hover/click;
5. build the record-once/replay-twice `@xterm/headless` oracle harness;
6. run a full-screen agent TUI through the embedded-terminal example and pin
   every semantic discrepancy as a renderless regression test;
7. move PTY/emulator ownership into the first detachable session host.

## Decision log

- 2026-07-18: Name the tool `dasHerd`.
- 2026-07-18: Force one writable agent session per worktree.
- 2026-07-18: Begin with the terminal vertical slice.
- 2026-07-18: Keep `dasTerminal` renderless and put rendering in dasImgui.
- 2026-07-18: The session host, not the UI, owns the child process and PTY.
- 2026-07-18: Start direct editing with a local Markdown PR-body draft.
- 2026-07-18: Let semantic conformance tests, replay streams, live PTY tests,
  and chunk-invariance/fuzz tests shape the terminal implementation.
- 2026-07-18: Start T0 with an in-tree C++11 state machine behind a replaceable
  canonical snapshot API; reconsider a library when measured coverage demands
  it.
- 2026-07-18: Keep the first live daScript adapter thin: one terminal handle
  pumps a separately implemented PTY into the emulator and automatically routes
  terminal replies, while exposing process status and explicit termination.
- 2026-07-18: Land the first dasImgui terminal view with snapshot-owned cell
  geometry, semantic input encoding, scrollback, and clipboard selection before
  introducing the detachable host boundary.
- 2026-07-18: Gate terminal UI automation on observable geometry, focus,
  input/content revisions, and semantic screen text; never use frame counts or
  sleeps as interaction-success criteria.
- 2026-07-18: Default dasHerd terminal typography to 150% with visible zoom and
  Ctrl+wheel, and maximize the desktop terminal shell on launch.
- 2026-07-18: Put terminal Edit/View controls at the bottom, use 5% zoom steps,
  expose Reset, and treat history/search, resolver-driven links, cursor blink,
  and broad Unicode/emoji fallback as product requirements rather than polish.
- 2026-07-18: Validate TUI rendering by replaying one recorded PTY stream into
  dasTerminal and an independent semantic oracle; the renderer's own plain-text
  projection is telemetry, never an oracle.
- 2026-07-19: Treat terminal and rich-text live-command inspection as a product
  gate. Structured state must expose the final drawn cells/runs, font fallback,
  missing glyphs, paint, clipping, hover, and cursor; `view_image` is not proof.
- 2026-07-19: Treat stress and performance as a terminal correctness gate.
  Retained history must not affect per-frame work; measure cold and warm,
  interpreter and JIT, emulator and renderer phases independently before tuning.
- 2026-07-19: Keep the full terminal snapshot as an explicit compatibility API,
  but render only a bounded active viewport. The first pass restored about 120
  FPS at 10,000 retained rows and pinned flat interpreter/JIT projection costs.
- 2026-07-24: A session is a durable entity, not a process run. The watcher
  persists a herd registry (name, host, agent profile, workspace, lifecycle
  `running/exited/closed`); the PTY is a linked attribute. Exited sessions stay
  listed and relaunch via the profile's resume command in the same primary
  worktree; a watcher restart flips running records to `exited/watcher_restart`;
  `closed` is an explicit user act.
- 2026-07-24: Agent profiles are user-editable config (`dasHerd.profiles.json`
  beside `dasHerd.json`), shipped with `shell`, `claude`, and `codex` defaults:
  launch argv, resume argv, and an `agent` flag that gates DASHERD env +
  context/skill injection. A herd-managed agent session may be task-less.
- 2026-07-24: Workspace is a primary worktree plus satellite repositories.
  Growth is announce-and-verify (agent declares via the CLI, watcher validates
  and attaches) or explicit user add; no passive checkout sniffing.
- 2026-07-24: Worktree/checkout contention is advisory only: flag every
  non-closed session attending the same canonical worktree path, never enforce.
- 2026-07-24: Agent-to-human notifications stay free text over the mailbox; the
  UI surfaces unread counts, no typed status vocabulary.
- 2026-07-24: Workflow (regular work / assist-PR / review-PR) is a creation
  template that parameterizes setup only; after birth all sessions are the same
  kind of thing. New worktrees default to repo-local `.codex/worktrees/<name>`
  with the path editable in the launcher.
