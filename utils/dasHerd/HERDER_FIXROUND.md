# dasHerd fix round — play-session triage (2026-07-25)

Source: Boris's first real play session (31 notes) + parity audit #1.
Rule of the round: obvious fixes land without discussion; everything
else gets a decision here first. Standing practice adopted the same day:
screenshot vs live-inspection parity — anything visible but not
inspectable, or editable but not commandable, is a bug (the app is as
much for the agent as for the human).

## Raw notes (numbering shared with the session)

1. Sessions & Activity: no "New session" icon in the panel.
2. Worktree right-click: no context menu; expected "New session" with
   that worktree preselected (when not already claimed).
3. New Session launcher must be a MODAL DIALOG, not dockable; while
   any dialog is up the rest of the UI is disabled and dimmed (darker
   background), standard modal behavior. (Reverses the dockable call.)
4. Agent session launch: (a) terminal colors wrong; (b) env leak —
   child Claude Code inherits CLAUDE_CODE_CHILD_SESSION → transcript
   saving off → Relaunch/`--continue` broken.
5. Launch button doesn't close the launcher.
6. Changelist per-row action icons: no tooltip on hover.
7. Session appears twice (herd card + raw PTY row) — reads as duplicate.
8. Terminal has no scrollback access. (Parity audit: `scrollback_rows`
   is 0 — no retained scrollback data either; data + UI fix.)
9. Git surfaces don't aim at the attached session's worktree
   ("select a worktree" despite known origin).
10. Mixed path separators in herd records (`D:/...\.codex/...`) —
    string identity matches (aim, conflicts, joins) silently fail.
    Investigated live: agent work was in the right worktree all along;
    surfaces were aimed elsewhere + PR tab counts commits only.
11. Launcher: chose an EXISTING worktree, a new one was created —
    "Create a dedicated worktree" default-on makes "Start from" mean
    "base for new", not "run here". Semantics must be explicit.
12. Ctrl+wheel zoom is global; should zoom the view under the cursor.
13. File Inspector missing search; audit every view for search.
14. Inspector Diff/View mode must stick across file selections.
15. "Previous/next change" are text buttons — icons; audit the rest.
16. Project view → real file browser: distinct colors
    (folder/file/hidden), explicit sort modes with cycle, dirty
    propagation up folders, table with toggleable size/date columns,
    name filter with include-subfolders checkbox (default on).
17. Launched agent unaware of dasHerd/dasherder skills.
18. Repositories & Worktrees readability: colored path/branch
    components, actively-used markers, worktree ↔ session navigation.
19. Sessions panel: Attention/Bundles belong under each session as
    subtree children; zero-count sections don't render.
20. Typed-token color language (SHA, PID, path, branch) — one scheme,
    applied everywhere.
21. PR view shows its base (merge-base commit) as a disabled row even
    with zero outgoing commits; hint at uncommitted Changelist work.
22. Active perspective indication too subtle; History needs a visible
    branch/base delimiter.
23. Settings: separate tabs from the start; command pattern with
    editable keyboard shortcuts first.
24. Settings stays a dock tab; zoom is PER WINDOW and persisted;
    settings write-through on every change.
25. Multiple terminals (or instant switching with preserved state) —
    scenario 1 is several agents at once.
26. "Agent needs me": surface blocked-at-prompt / waiting-for-input on
    cards, not just explicit Attention.
27. Destructive actions (Terminate, Close session) need confirmation.
28. Error history surface (latest-only chrome loses dismissed errors).
29. Cards show output age, not just registry age.
30. Parity audit #1: (a) selectable rows carry no label text in
    snapshots — dasImgui fix; (b) terminal colors not inspectable
    (need cell attributes); (c) scrollback_rows exposed = good, value 0
    = bug; (d) no attach-session / terminal-input semantic rails;
    zoom % not in state.
31. The agent (Claude) talks to sessions through the terminal: type,
    submit, read. Acceptance test after next restart — PowerShell
    first, then a live agent session.

## Priority (settled 2026-07-25: bugs over everything)

Defects first — anything broken, silently wrong, or lying to the user
or the agent lands before any feature, polish, or redesign work.

P0 — broken core flows:
- 4b env leak (kills `claude --continue` for every launched agent)
- 10 path-separator identity mismatches (conflicts/aim/joins dead)
- 11 launcher ignores "run in existing worktree" intent
- 34 launcher race: Launch clicked before the repository list arrives
  is a silent no-op (button silently disabled; no feedback) — same
  family as 11: the launcher lies about what it will do
- 8 scrollback — INVESTIGATED, decomposed (2026-07-25): the emulator
  retains history fine (probe: 200 lines → scrollback_rows 136) and
  the wheel already scrolls it (verified scroll_offset 0→15→0). Two
  real items remain: (a) DISCOVERABILITY — no scrollbar, so scrolling
  looks impossible (P2, add the scrollbar); (b) agent TUIs like Claude
  Code repaint in place, so terminal-level history is legitimately
  empty there — transcript scrolling belongs to the TUI itself; a
  watcher-ring history view is the eventual answer if we want one.
  Rail nuance found on the way: herder_terminal_state.screen_text is
  the live bottom screen regardless of scroll_offset (fine — the agent
  wants the live screen), and the attached session can change between
  input calls — herder_terminal_input should grow an optional
  expected-session guard.
- DONE 17 agent awareness (2026-07-25, four slices): per-profile
  intro_lines (opt-in first-prompt injection); marker-guarded
  CLAUDE.local.md drop + per-worktree git exclude at agent launch;
  dasherder row in the CLAUDE.md skill table; dasherd MCP shim
  (utils/dasHerd/mcp_main.das — whoami/inbox/outbox/bundle/repository
  tools over the watcher HTTP API, wired into .mcp.json by the
  supervisor's emit-config; CLI stays as human/fallback). Env scrub +
  transcript persistence landed earlier with 4b. Codex rides MCP —
  no AGENTS.md shadow (tracked-file checkout collision).
- 4a terminal colors (blocked on 30b tooling)

P1 — tooling bugs by the parity rule (agent-blindness is a defect):
- 30a selectable rows carry no label text in snapshots
- 30b terminal cell attributes not inspectable
- 33 minimized window starves UI-level agent driving
- 35 no human/agent UI input arbitration: while the agent drives
  synthetically, human clicks still partially react (real and synth
  input MERGE unless set_user_control(false) explicitly detaches the
  GLFW callbacks — nothing in dasHerd manages that today, and OS
  window chrome is never suppressible from ImGui anyway). Fix shape:
  an explicit UI input lease mirroring the terminal controller lease —
  agent driving detaches real input + shows a visible "agent driving"
  banner with a human takeover gesture; semantic rails (Wave 0)
  sidestep the race entirely and stay the preferred path.

P2 — small defects:
- 5 launch doesn't close launcher; 6 changelist row tooltips dead;
  14 inspector mode resets; 7 double-listed session
- RESOLVED 9 by the note-10 fix: attach_session always re-aimed at the
  session origin (selection + review/history/refs) — it looked dead
  because the origin carried the mixed-slash spelling and matched no
  observed worktree. Canonical spellings restored the behavior; the
  selection-is-sacred rule still holds for task auto-attaches (they
  pass follow_origin=false since the ghost fix).
- 36 Launch has no in-flight debounce: duplicate clicks (incl.
  synthetic clicks queued while the window was minimized, then
  replayed on restore) each create a session — observed three
  identical env-probe sessions from one intended launch. Debounce on
  a pending-create flag cleared by the herd revision bump.

Features, foundation (command pattern/settings), and surface redesigns
queue strictly behind these — the wave structure below survives as the
build order within what remains.

## Wave 0 — rails first (enables everything else, incl. testing 31)

- 30a: selectable/selectable_label snapshot payload gains its label
  text (dasImgui boost; benefits every app).
- DONE 30d: `herder_attach_session` (by PTY or herd id),
  `herder_terminal_input` (text + submit, lease-checked),
  `herder_terminal_state` (screen text, cursor, scroll, zoom).
- 30b: terminal cell-attribute/palette inspection (prerequisite for
  diagnosing 4a).
- 31 acceptance test: DONE for PowerShell (echo round-trip through the
  three commands, no UI choreography); agent-session leg pending.

Findings while landing Wave 0:
- 32. Sessions-list loop crash (FIXED): bare `same_line()` inside the
  per-session row loop — single-global widget renders once per frame —
  crashed update() the first time TWO interactive sessions were
  visible at once. Latent since the state-glyph slice; the multi-agent
  scenario would have hit it immediately.
- 33. Minimized window starves the agent: the harness frame gate stops
  ImGui frames, so imgui_click/screenshot silently do nothing while
  WS-level rails keep working. Either keep rendering agent-facing
  frames while minimized, or route everything the agent needs through
  frame-independent semantic commands (the direction 30d already
  takes).

## Wave 1 — obvious fixes, no discussion

- 4b env sanitization: strip CLAUDE_CODE_* child markers; set
  CLAUDE_CODE_FORCE_SESSION_PERSISTENCE=1 for agent profiles.
- 10 path canonicalization: one spelling (forward slashes) at every
  boundary (launcher suggestions, registry, announce), via fio helpers.
- 5 launch closes launcher; 1 new-session icon button in Sessions;
  2 worktree right-click → New Session preselected.
- 6 changelist row tooltip bug; 14 inspector mode stickiness;
  15 prev/next → icon buttons.
- 21 PR base row + "N uncommitted changes in Changelist" status hint.

## Wave 2 — foundation (discuss shape, then build)

- DONE 23/24 settings core (2026-07-25): dasHerd.ui.json, write-through
  on every change, tabbed Settings window (General: base zoom slider,
  session-list toggles), migrates the old layout-choice file; verified
  save+load live.
- DONE 12 per-window zoom (2026-07-25): window_zoom_scope wraps each
  work window — Ctrl+wheel adjusts only the hovered window, persists in
  window_zoom, composes with the base zoom via with_font. Terminal
  keeps its explicit buttons on the base zoom pending a renderer pass.
- REMAINING: command pattern (named commands as the single dispatch) +
  editable shortcuts + Settings > Shortcuts tab.

## Bug-queue status (2026-07-25, end of first burn-down)

Every in-repo bug from the triage is fixed, live-verified, and
committed. Still open, all in the dasImgui repo (separate checkout,
separate PR flow; rebuilding its DLLs tears down the running rig):
30a selectable snapshot labels, 30b terminal cell-attribute
inspection, 4a terminal colors (blocked on 30b), the terminal
scrollbar. Note 31's agent-session leg waits for the next live agent
session.

## Wave 3 — surface reworks (each needs a short design pass)

- 3+11+5 launcher: modal dialog, explicit run-here vs create-new,
  claimed-worktree awareness (ties to conflict advisory).
- 7+19+26+28+29 sessions panel: session subtree (terminal link,
  attention, bundles, errors as children), no zero-count sections,
  needs-me + output-age signals (watcher activity telemetry).
- 8+4a terminal: watcher-side scrollback retention → client viewport,
  wheel scroll, then the color/palette fix (after 30b makes it
  provable). Search lands here too (13).
- 16 project view table; 18 repositories readability; 20 token color
  language (one shared palette module feeds 16/18/20/22).
- 9 auto-aim policy: empty selection may aim at attach origin;
  explicit selection stays sacred.
- 27 destructive confirmations.

## Wave 4 — structural

- 25 multiple terminals.
- 17 agent awareness of dasherder (CONTEXT_PATH surfacing into the
  agent's instruction set) — unlocks real Attention/Bundle testing.

## Icon-set expansion (dasImgui, consult Claude Design)

Existing set covers refresh/stop/add/folder/search/gear/prev/next.
Likely new needs: file glyph (vs folder), dirty badge/dot, sort-mode
cycle, filter, confirm/danger, collapse/expand-all, send/submit.
Run the proposed additions past Claude Design (daslang.io Forge design
system) before drawing them; regen the icon catalog after.

## Sequencing (settled 2026-07-25)

No intermediate PR — no users yet. The whole fix round (Waves 0-4)
lands on this branch, then one PR for historical record, then the big
review happens on that PR.
