# dasHerd rich client UX contract

Status: working design; each section carries its own discussion status.
Measured baseline and trigger forensics: `herder_perf_state` and
`herder_ui_events` live commands, added 2026-07-24.

## Principles

- The user's selection is sacred. Nothing re-targets the selected
  repository, worktree, file, tab, or scroll position except an explicit
  user action on that surface. Automatic events may mark and count, never
  navigate. (This is the Attention-panel rule from
  `AGENT_REVIEW_WORKFLOWS.md` generalized to every async arrival.)
- Content is stale-while-revalidate everywhere: last-good content stays
  visible and interactive during any refresh; replacement is atomic per
  logical unit (one worktree's files, one review result, one prepared
  document).
- Every async surface states what it is doing in a reserved place that
  does not move content when it appears. Busy is a glyph plus a short
  phrase in the owning window's status line; app-wide state lives in the
  chrome, not inside work surfaces.
- Icons carry state; words carry identity. A row's noun stays text; its
  state (running, dirty, failed, staged) becomes a glyph with a hover
  tip. No icon without a tooltip; no state-word where a glyph will do.
- Terminal text is for terminals. Watcher plumbing (git observation
  tasks) is activity telemetry, not a peer of the user's sessions.

## Notification model

Discussion status: slice 1 implemented

Three channels, never mixed:

1. **App chrome (menu-bar right cluster)** — connection state, active
   background work count, pending Attention count, and the latest error
   with a dismiss control. Fixed position, appears/disappears without
   moving any window content. Clicking the Attention bell opens the
   Sessions window; clicking the error's close glyph dismisses it.
2. **Per-window status line** — each Git window keeps its reserved
   bottom line: severity glyph + text. Loading and errors replace the
   summary text only, never insert rows above the document.
3. **Attention panel** — agent handoff only (focus sets, bundles), as
   specified in `AGENT_REVIEW_WORKFLOWS.md`. Arrival never navigates.

Errors no longer render inline inside the Terminal window; the copy /
send-to-agent context actions move with them to the chrome cluster.

## Iconography

Discussion status: glyph language settled for slice 1; grows per surface

From the `imgui_icons` set (amber, tooltip mandatory):

| State/action | Glyph |
|---|---|
| Watcher connected / disconnected | `link` / `unlink` |
| Background work active | `clock` (+ count) |
| Attention pending | `bell` (+ count) |
| Error | `warning`; dismiss via `close` |
| Session running / ok-exit / failed-exit / terminating / starting | `play` / `check` / `warning` / `stop` / `clock` |
| Refresh action | `refresh` |
| Terminate action | `stop` |
| Copy SHA (existing) | `duplicate` |
| Stage / unstage (existing) | `check` / `minus` |
| Focus visible / hidden (existing) | `visible` / `hidden` |

Planned next: worktree dirty markers (dot + count as text is fine),
branch glyph (`merge`) on ref labels, `folder` on repository headers,
`gear` for Settings, `search` where search lands.

## Busy and progress language

Discussion status: slice 1 implemented; richer progress later

- The observer's five streams (refresh, review, history, commit files,
  refs) each surface as busy only in the window that shows their result;
  the chrome cluster shows the aggregate count.
- Delay is dominated by git process time; the job of the UI is to make
  waiting legible, not to hide it. No spinners over content; no modal
  waits.
- Later: per-stream elapsed time in the status line once a stream runs
  longer than ~1s (the data is already in `herder_perf_state`).

## Sessions vs watcher tasks

Discussion status: slice 1 implemented

The Sessions window lists interactive sessions only (agents, shells).
Watcher plumbing kinds (`git-*`, `worktree-create`) are hidden behind a
View toggle ("Show watcher task sessions", default off) — they remain
fully inspectable, and Git failures still attach their task terminal
automatically. Session rows carry a state glyph. Automatic attaches
never target plumbing kinds and never re-aim the Git surfaces
(committed as 6a48bc7ee).

## Default layout

Discussion status: PROPOSED — do not implement before discussion

Current default tabs Git Activity, Git Changelist, File Inspector, and
Terminal into one right-hand dock, which makes the inspector — the
actual work surface — a tab fight. Proposal for wide displays:

```text
+----------------+---------------------------+----------------------+
| Repositories   |                           |  Git Changelist      |
| & Worktrees    |   File Inspector          |  (PR/History/Tree    |
+----------------+   (Diff | View)           |   above the files)   |
| Sessions       |                           +----------------------+
| & Activity     |                           |  Terminal            |
+----------------+---------------------------+----------------------+
```

- File Inspector is the permanent center; it never shares a tab bar.
- Changelist and Activity merge into one right column (perspectives on
  top, file list below) — they are one workflow, not two windows.
- Terminal docks under the right column, tall enough for ~15 rows.
- Settings stays closed by default; opens floating.

Open questions: whether Activity perspectives + Changelist merge into
one window or stay separately dockable; terminal default height; where
the launcher lands (see below).

## Session launcher

Discussion status: PROPOSED

Replace the modal "Start Codex debug session" popup with a dockable
launcher pane (per `WORKTREES_AND_TASK_TERMINALS.md` section 3: shell
or agent, new/resume, worktree, argument preview). The modal blocks
inspection of the very worktree state the launch depends on.

## Context menus and hints

Discussion status: PROPOSED

- Every right-click surface gets exactly one context menu; entries are
  verbs with objects ("Copy error", "Look at that") — no bare nouns.
- Hover tips on rows appear only for truncated content or glyph-only
  controls; no tooltip that repeats the visible text.
- The Look-at-that flow stays as specified in
  `AGENT_REVIEW_WORKFLOWS.md`.

## Slices

1. Notification chrome + status glyphs + sessions/tasks split +
   session-state glyphs. (implemented with this doc)
2. Layout redo per the settled section above; icon adoption on rows and
   action buttons (refresh/terminate/add).
3. Launcher pane; context-menu sweep; per-stream elapsed telemetry in
   status lines.

## Decision log

- 2026-07-24: Selection is never re-targeted by automatic events; the
  attach ghost (task auto-attach following session origin) is the
  canonical violation and is fixed.
- 2026-07-24: Watcher task sessions are activity, not sessions; hidden
  by default behind a View toggle.
- 2026-07-24: One app-chrome notification cluster in the menu bar;
  per-window status lines keep surface-local state; the Attention panel
  remains agent-handoff-only.
