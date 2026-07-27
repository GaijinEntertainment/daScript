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
- DONE command pattern + editable shortcuts (2026-07-25): built on
  imgui/imgui_commands (the existing dasImgui registry — ImGui-routed
  Shortcut() chords, macOS primary/secondary, invocation queue,
  bindings file with overlay-on-defaults load) instead of a bespoke
  table. rich_commands.das is a thin adapter: id -> action table,
  herd_commands_poll/drain in the frame loop, write-through save on
  any binding edit (dasHerd-bindings.json). 21 commands registered
  (Session/View/Zoom/Layout/Terminal); menus route through
  run_herd_command and display live chords; Settings > Shortcuts tab
  lists all commands with click-to-capture, clear, conflict warnings,
  and Restore defaults; rails: herder_commands_state / herder_command
  / herder_set_shortcut. Chord polling is gated off while the terminal
  is focused or a text field owns typing (chords are PTY bytes there).
  Live-verified end to end: synthetic Ctrl+Shift+2 fired layout.review
  through real ImGui routing and persisted (bare "2" control test
  correctly did nothing); click-to-capture armed on the binding button
  and captured Ctrl+Shift+R with modifiers; rail set/clear
  round-tripped the bindings file; menu shortcut hints render from the
  registry. imgui_commands changes (dasImgui): capture_binding renamed
  public command_capture_tick for custom binding UIs, and the capture
  loop now skips the side-modifier keys (LeftCtrl..RightSuper) — it
  used to bind "Ctrl+LeftCtrl" the instant a real keyboard pressed the
  modifier half of a chord. Synth-test gotcha for the record:
  imgui_key_chord mods is an ARRAY (["Ctrl","Shift"]); a "Ctrl+Shift"
  string silently parses as no mods.

## Bug-queue status (2026-07-25, end of first burn-down)

Every in-repo bug from the triage is fixed, live-verified, and
committed.

DONE 30b (2026-07-25): herder_terminal_state gained an `ansi` arg
returning the emulator's checkpoint ANSI with full SGR — terminal
attributes are now inspectable end to end.

DONE 4a (2026-07-25, root-caused via 30b): the colors were never a
rendering bug. Probes proved the dasTerminal emulator decodes and the
imgui_terminal renderer draws 16/256/truecolor + dim + resets
correctly. The TUIs were monochrome because Claude Code sets
NO_COLOR=1 in its tool shells — the watcher inherited it and passed it
to every PTY child, so chalk disabled color at the source. The env
sanitizer now also scrubs NO_COLOR, CLAUDE_CODE_SESSION_ID, and
CLAUDE_PID; verified with a live Claude launch rendering the brand
truecolor logo.

DONE in the dasImgui checkout, branch codex/embedded-terminal (all
das-side — no DLL rebuild was ever needed):
- 30a (144f0ed): ClickState/ToggleState carry the visible label
  (## suffix stripped) in snapshot payloads; icon_button reports its
  glyph. Verified: worktree rows now name themselves.
- Scrollbar (f85b643): draggable right-edge scrollback track on the
  terminal widget, inset clear of the dock splitter's hover-stealing
  grab zone. Verified: drag jumped scroll_offset 0→218 of 236.

DONE note 31 in full + the conversation rails (dasHerd 5f9a38160):
herder_session_say / herder_session_reply / herder_session_key
(enter/escape/ctrl-c/tab/shift-tab/arrows), with the Enter deferred
~200ms past the text so Ink TUIs submit instead of treating it as
paste. Proven in a live conversation with a herd-launched Claude
(greeting, 2+2 -> "4", paced ack). onOpen now resets herd_revision so
a restarted watcher's first broadcast always applies.

38. (Boris, post-batch; diagnosis corrected by Boris) The @live
    user-control lock leaked real input: with set_user_control(false)
    active (agent owns synth input), clicks and typing were dead as
    intended but REAL mouse wheel still scrolled the terminal. Root
    cause class: three independent owners of the GLFW callback slots
    (raw glfw, ImGui_ImplGlfw Install/Restore, the DasGlfw chain with
    its retained prev_* pointers) interleave; any glfw_post_* re-arms
    a slot with the chain dispatcher whose captured prev still routes
    into ImGui, so the backend detach alone cannot sever real input.
    DONE (three layers):
    - dasGlfw C++: real/synth entry split — GLFW slots now get
      DasGlfw_Real* trampolines gated by glfw_set_real_input_muted;
      DasGlfw_Post* (synth) call the dispatchers directly and bypass
      the mute. Depth guard in dispatchers breaks historical
      prev-pointer cycles. imgui_live's toggle sets the mute alongside
      the backend detach.
    - Visibility (Boris: "indicate in window header"): generic
      live_apply_control_badge in glfw_live appends "[AGENT CONTROL]"
      to the OS window title while locked (any glfw app); dasHerd
      menu status cluster shows an amber SYNTH LOCK badge; terminal
      footer states VIEW ONLY when the PTY controller lease is held
      elsewhere (the original, lease-level half of this note).
    - Inspectability: herder_input_state live command (user-control
      lock, synth cursor/keys, io cursor, real wheel/click counters)
      — the lock was previously invisible to both screenshot and
      snapshot, a parity-audit violation.
    The lease-arbitration UX ("input is leased to X — click to
    claim") still folds into the note-35 design.

Still open, small, dasImgui-internal (agent-rail QoL, no dasHerd flow
blocked): combo imgui_force_set stops applying after a live reload
(popup-click workaround exists); ClickState.click_count counts clicks
on enabled=false widgets (the macro gates the return value after the
widget already counted). Also learned: set_user_control(false)
detaches the GLFW layer imgui_click itself injects through — use the
imgui_mouse_* bypass commands while input is detached.

39. (Boris, end of round) Opening a portal (nested-repo) file from the
    Project view in sticky Diff mode shows a SILENT EMPTY inspector —
    reads as stuck. Forensics (live, while stuck): the app was fine
    (frames advancing); diff_row_count/old/new byte counts all 0 with
    prepare_outcome "ready", status "" — the parent repo's git cannot
    diff a nested-repo file, and nothing says so. The VIEW side had
    prepared fine (13.7KB). Fix shape for the review round: portal
    files force View mode (or the diff pane states "file belongs to a
    nested repository — no diff against the parent") — pairs with the
    registered-repo synthesis follow-up under note 37.

## Wave 3 — surface reworks (each needs a short design pass)

- DONE 3+11+5 launcher (11/5 landed earlier; 3 on 2026-07-25): the
  launcher is now a true MODAL dialog — popup_modal centers it, dims
  the whole UI, and blocks input to everything behind (note 3's exact
  ask); Escape or the X dismisses; a successful Launch closes it via
  close_current_popup. LAUNCHER_WIN is gone; all openers (menus, the
  session.new command, worktree context menu, herder_open_launcher
  rail) route through open_launcher_modal, and herder_herd_state's
  launcher_open now reads the modal. Claimed-worktree awareness stays
  covered by the conflict icon/advisory from the earlier rounds.
  Live-verified: rail-opened modal renders centered over a dimmed,
  input-blocked UI.
- MOSTLY DONE 7+19+28+29 sessions panel (2026-07-25): each herd card
  now carries its own Attention (N) / Review bundles (N) children
  (mailbox/bundle session_id joined to the card's pty_session_id via
  shared row renderers with frame-global widget indices); the global
  sections render only UNOWNED items and only when non-empty — the
  perpetual "(0)" sections are gone. Cards show "output <age>" from
  client-witnessed output_bytes deltas beside the registry age (29).
  A client-side error ring (note 28, cap 20) keeps every error
  transition the chrome dismissed, shown as an Errors (N) section
  with clear, hidden while empty. Verified live: zero-count sections
  vanished, panel clean; per-card children reuse the verified
  fallback renderers and light up with live agent data. REMAINING 26:
  "agent needs me" (blocked-at-prompt) needs watcher-side terminal
  heuristics — pairs with the note-25 multiple-terminals work.
- 8+4a terminal: watcher-side scrollback retention → client viewport,
  wheel scroll, then the color/palette fix (after 30b makes it
  provable). Search lands here too (13).
  - 13 FIRST SLICE DONE (2026-07-25): File Inspector View mode has
    find — case-insensitive, incremental (rebuilds per keystroke),
    prev/next with wraparound, "N of M" counter, amber "no matches";
    jumps reuse the focus-scroll rail (row * line_height). Verified
    live: "th" in rich_state.das → "1 of 154" scrolled exactly to
    "require math"; next → "2 of 154" deep-jump. Diff-mode search
    deferred: the aligned panes need a text-line -> display-row map
    before jumps can land right. Terminal + history search still open
    with the scrollback work.
- DONE 16 project view (2026-07-25): cached tree model; folder/file/
  hidden color tiers; dirty * propagating up folders from repository
  status; four sort modes cycling on a header button (stat-backed
  Largest/Newest aggregate up folders); persisted size/modified
  columns; name filter + include-subfolders flat results. Verified
  live: 8439-file model, 21 dirty nodes on exactly the changed chains,
  filter and sort driven through the rails.
- DONE 18 repositories readability (2026-07-25, also closes the
  never-landed Wave 1 notes 1 and 2): worktree rows draw MAIN in the
  title tint, branches slash-segmented in the path palette, dirty
  counts amber; a play/clock marker shows the claiming (running/
  parked) herd session with a right-click "Open session '<name>'"
  that raises Sessions and attaches only when the PTY is still alive
  (a registry entry can outlive its PTY across a watcher restart);
  right-click also offers "New Session here..." (note 2) which
  selects the worktree and routes through the session.new command;
  the Sessions header gained the "add" new-session icon (note 1).
  Live-verified: colored rows, claim markers on both occupied
  worktrees, both context items, dead-PTY navigation no longer
  surfaces "unknown session".
- DONE 22 perspective indication (2026-07-25): every Git Activity tab
  opens with a loud banner — perspective name in the warning accent,
  context in the branch tint (PR shows "branch -> base", History
  "branch - outgoing first, then base", Tree the branch, Project the
  worktree path). The History table inserts a branch/base delimiter
  row where the outgoing (review-set) commits end and base history
  begins, list_clipper-safe via a +1 visual-row mapping. Verified
  live on the HISTORY banner; the delimiter sits below the fold on a
  177-ahead branch (deterministic row math against the review set).
- 20 token color language (one shared palette module feeds 18/20/22).
  - 20 FOUNDATION DONE (2026-07-25): rich_tokens.das names every token
    color once (sha/branch/pid/path dir-sep-name/warning/error/
    added/deleted/title/muted/state) and rich_state re-exports it to
    every surface; all previously hard-coded literals in git_ui,
    files_ui, client chrome, terminal footer, and the shortcuts tab
    now pull from it (identical values — zero visual delta), and the
    terminal session line renders state + pid as colored tokens as
    the exemplar for previously-plain tokens. Remaining under 18/22:
    branch/path/sha adoption on herd cards and the repositories panel,
    perspective indication.
- 37 nested repositories in the Project view (found by Boris opening
  modules/dasVulkan): git lists a nested repo as one "dir/" entry.
  FIXED same day: the entry builds a folder chain tagged "(repo)"
  instead of a phantom empty-named file row that opened the directory
  as a file. PORTAL DONE (2026-07-25): expanding a "(repo)" folder
  lazily lists the FILESYSTEM (skipping .git), children recurse as
  portals, hidden tier applies, only the portal root carries the tag,
  and children sort with the active mode. Verified live: dasVulkan in
  MAIN expands to .github/_build/cmake/daslib/doc/examples/generator —
  contents no git listing can see. LIMITATION: a GITIGNORED nested
  repo (modules/dasImgui inside worktrees, .gitignore:108) produces
  no status entry at all, so no portal root exists — discovery for
  those needs registered-repository synthesis (planned with the
  portal-into-own-Project-view follow-up). Opening a portal file in
  the inspector still goes through the parent repo's git plumbing and
  may error — acceptable until nested repos register as repositories.
- DONE 9 auto-aim policy (2026-07-25): g_selection_explicit tracks
  whether the selection came from a deliberate pick (worktree row
  click, focus-target navigation, herder_git_select_worktree) vs a
  seed/auto-aim; attach_session may re-aim at the session origin
  whenever the selection is not explicit, even for task auto-attaches
  with follow_origin=false. Flag exposed in herder_client_state;
  verified false on the seeded default, true after an explicit select.
- DONE 27 destructive confirmations (2026-07-25): Terminate-session
  and Close-session route through one confirmation modal; the target
  is captured at open and re-checked at confirm so a selection change
  under an open dialog can't redirect the action. Live-verified:
  right-click Close on scroll-probe -> modal with target-specific
  text -> Confirm -> closed_by_user in the registry. Agent rails
  (herder_close_session) stay direct by design — confirmation is a
  human-mistake guard, not an agent gate.

## Wave 4 — structural

- 25 multiple terminals.
- 17 agent awareness of dasherder (CONTEXT_PATH surfacing into the
  agent's instruction set) — unlocks real Attention/Bundle testing.

## Icon-set expansion (dasImgui, consult Claude Design)

Existing set covers refresh/stop/add/folder/search/gear/prev/next,
plus play/clock/check/warning/bolt/chevron-right/bell/link/unlink/
close/edit in use across dasHerd.
Run the proposed additions past Claude Design (daslang.io Forge design
system) before drawing them; regen the icon catalog after.

PROPOSAL (ready for the consult, 2026-07-25 — every entry is a place
the UI currently substitutes text or a wrong-shaped glyph):
- file: Project rows borrow "edit" for files today; a document glyph
  pairs with the existing folder.
- repo-portal: the "(repo)" text tag on nested-repository folders
  wants a small badge glyph (folder + branch node?).
- dirty-dot: the amber "*N" works but a small dot/badge form would
  read cleaner in narrow rows (worktrees, Project).
- sort-cycle: the Project header sort is a text button ("Sort:
  Folders first"); one glyph with a mode sub-mark.
- filter: the Project filter row and future search fields.
- confirm / danger: the destructive-confirm modal buttons are plain
  text; a danger accent glyph for Terminate/Close affordances.
- send: "Send to session agent" context items and future outbox.
- keyboard: the Shortcuts tab and capture state.
- terminal: session cards and the worktree->session jump currently
  borrow play/clock; a terminal glyph distinguishes "has a terminal"
  from lifecycle state.
- agent-lock: the SYNTH LOCK / [AGENT CONTROL] states borrow
  "warning"; a dedicated takeover glyph reads less like an error.
- expand-all / collapse-all: Project tree QoL.

## Sequencing (settled 2026-07-25)

No intermediate PR — no users yet. The whole fix round (Waves 0-4)
lands on this branch, then one PR for historical record, then the big
review happens on that PR.

## Review round (2026-07-25, post-merge of PR #3567)

Full-tree review: personal pass on watcher core/server/net + 3 agent
passes (git/inspector/files UI, sessions/launcher/terminal, C++/glue).
9 verified defects fixed in commits 872fe420d + c6e285d74 (token
dual-spelling auth, empty-cwd guard, worktree-row highlight, portal
re-list, say-queue garbling, output-age table growth, glfw chain
dispatch UAF, daslang-live is_reload, CMake stale-module sweep
deleting standalone-module artifacts).

## Block: external sessions (2026-07-25 evening)

Goal (Boris): "external sessions" — a session whose terminal lives OUTSIDE
the herder (like the dev session building dasHerd itself), claimed from the
outside, attached to worktree(s), coordination-only (mailbox/bundles/claims;
no terminal view, no lease, no input), and able to OUTLIVE the herder across
watcher restarts. Follow-up: "Repositories and worktrees" migrates to the
right side as "Git repositories and worktrees"; the left side is sessions,
sessions-first.

Issue intake (live rig, numbering continues):
- 40: sessions list: alive on top, dead in a different color, delete button
  for session + associated worktrees — IMPLEMENTED, pending deploy
- 41: worktree delete from the git panel — in-use = hard block; uncommitted
  changes = block + offer to launch a resolver session briefed with the
  blocking state — IMPLEMENTED, pending deploy
- 42: terminal in a NEW session does not scroll — FIXED (2026-07-26 goal
  round). Root cause proven from the session journal: Claude Code runs on
  the ALTERNATE screen (?1049h — no terminal scrollback by design) and
  enables mouse reporting (?1000/1002/1003/1006h); in a real terminal the
  wheel goes TO Claude, which scrolls its own transcript. Our widget only
  did local scrollback scrolling (empty on alt screen). Fix: dasTerminal
  grew terminal_mouse_reporting_active + terminal_encode_mouse (SGR/X10);
  imgui_terminal forwards wheel + clicks to the child when reporting is
  active (Shift reserves local scroll/selection; Ctrl+wheel stays zoom).
  Live-proven: wheel over the claude session scrolled ITS transcript
  ("Jump to bottom (ctrl+End)" indicator on screen). Shell sessions were
  never broken (wheel 0->9 over 56 history rows on the same build).
- 43: herd card click selects then unselects (cc-color-probe3 clicked while
  "Towards 0.6.4 release" attached; selection bounces back) — FIXED
  (2026-07-26). The dead-PTY attach guard from the fix round stopped the
  selection yank but left the click with ZERO feedback (the highlight was
  bound to pty==selected_id, which never changes for a dead PTY). Cards now
  highlight through a herd-level pick (g_selected_herd_id); PTY selections
  made outside the cards retarget it once per change. Proven both ways:
  dead-card pick sticks, live-card pick transfers and attaches.
- 44: after creating a new session the terminal opens but has no keyboard
  focus — a mouse click is needed before typing. FIXED (2026-07-26),
  two layers: (a) SetKeyboardFocusHere's tabbing request falls through
  the terminal's InvisibleButton onto the next tab stop (snapshot showed
  the -5% zoom button holding focus) — focus now goes through the
  terminal's own model (g_terminal_view.focused); (b) the attach-time
  checkpoint restore rebuilt ImGuiTerminalState AFTER the handshake ran
  — reset_terminal now carries focus over. Proven: real keystrokes
  land in a fresh session with no click; snapshot focused=true
- 45: the icons left of each session card (profile / state / conflict) have
  no tooltips — unexplained glyphs. VERIFIED FIXED on deploy
  (2026-07-26): tooltips landed with the fix round; hover probe shows
  HERD_ICON_TOOLTIP hover=true value="Agent session (profile 'claude')"
- 46: Sessions & Activity shows opaque rows — "REGULAR s2563..." entries and
  an error list ("unknown session", "4m ago") that is not clickable, not
  scrollable, not hideable, purpose unclear. Needs names over raw ids,
  click-through, dismiss/collapse, and dropping entries whose session no
  longer resolves. LANDED IN BUILD (2026-07-26 audit): names-over-ids in
  the context panel and cards, attention rows show subjects and
  click-navigate, errors are a collapsed/scrollable/clearable ring,
  zero-count sections gone; the "unknown session" error source (dead-PTY
  attach) fixed under 43. Boris re-test decides any residue
- 47: visual artifacts in a NEW session's terminal — stray letters in a
  one-character column outside the text flow (Boris saw red "S"s stacked
  vertically bottom-right; captured 'e'/'w'/'s'/'n' on the pane's left
  edge). Looks like a stale/displaced grid column surviving the
  attach-time resize, un-clipped; fades as output overwrites. FIXED
  (2026-07-26): not the emulator — the "column" was Sessions-panel card
  text clipped at the WINDOW edge (one character into the padding, flush
  against the terminal border; bbox z=739 vs window edge 690 in the
  snapshot). Card detail/context/summary rows now elide to the content
  region (elide_to_avail). Screenshot-verified before/after
- 51: BLOCK PIVOT (Boris, 2026-07-25): --continue resume is a crutch, not
  the answer. Next block, ahead of external sessions and everything else:
  redesign PTY hosting until a terminal session "does not depend on
  anything, and yet can be communicated to" — detached ConPTY host that
  owns the console + child on its own, survives watcher/client/upgrade,
  reachable over a versioned IPC channel. Current UI/feature work is
  parked (committed on this branch) until that lands.
  PROGRESS (same evening): dasTerminal grew spawn_detached (CREATE_NO_WINDOW
  + job breakaway; DETACHED_PROCESS breaks console apps) and environment
  blocks on both spawn paths (retires the token-on-command-line item);
  utils/dasHerd/ptyhost/main.das v1 landed and smoke-proved the whole
  claim live: host spawned detached, launcher died, host kept journaling;
  a fresh client authenticated, replayed from byte 0, sent input, got the
  child's echo with a forwarded env var. Remaining: dastest lifecycle
  test, watcher launch-via-host + adoption, daspkg release packaging.
  LANDED (2026-07-26): all of it — lifecycle test, release packaging, and
  the watcher rework (launch-via-host for herd sessions, pumps proxied
  over the host WS, adoption on startup with sessions resurrecting as
  RUNNING, herd registry fold of dead hosts' exit stamps). Suite 71/71
  incl. test_watcher_adoption.das proving restart survival end-to-end.
  Decisions + the ConPTY drained-never-fires finding: PTY_HOST_DESIGN.md.
- 50: HARD RULE + arc — a watcher restart must never kill hosted sessions
  ("its not ok to kill my terminal session"). Today PTYs are ConPTY
  children of the watcher and die with it; needs a per-session broker
  process that owns the ConPTY and outlives the watcher (tmux-server
  model), with restart = re-discover + re-attach. Operationally until
  then: the watcher only restarts when no agent session is running or
  Boris explicitly says go. RESOLVED BY ARCHITECTURE (2026-07-26): herd
  sessions run in detached hosts; watcher restart adopts them back as
  running (see 51 / PTY_HOST_DESIGN.md). The operational rule stays until
  the rework is deployed to the live rig and proven there
- 49: default layout — Git Changelist docks bottom-right as its own pane
  under the Git Activity + File Inspector tab stack (per Boris's live
  arrangement, captured in boris_changelist_dock.png); update
  setup_layout_preset so dock reset / fresh install lands there.
  VERIFIED (2026-07-26): the preset already lands this shape; dock reset
  proven live (right column: Repositories / Git Activity+Inspector /
  Changelist bottom at y=1043)
- 48: VERIFIED FIXED on deploy (2026-07-26): a session pick aims the git
  surfaces (attach_herd_card), the Project tab kicks the files request on
  entry (8490 files on direct perspective switch), and the empty state
  distinguishes no-selection / failed / empty. Original: Project tab says
  "Select a worktree to browse its files" while a
  session is attached and selected. Rule: selecting or creating a session
  selects its primary worktree (a session pick IS a deliberate worktree
  pick; set the explicit flag), so Project/Changelist immediately point
  at the session's tree. Evidence: the header above that empty-state
  ALREADY shows "PROJECT D:/Work/..." (the path), and visiting History or
  Tree populates Project — so (1) the files request is not kicked on
  Project tab entry, and (2) the empty-state message lies (state is
  not-requested, not no-selection)

Deferred — nice-to-have, never over real work (Boris, 2026-07-25):
- token in child command line: fix is env-block support in dasTerminal
  spawn (pass DASHERD_* via CreateProcess lpEnvironment instead of a
  powershell -Command prefix). Harden later, way later.
- lease heartbeat starvation under multi-second frame stalls (client
  pumps ~1s, server timeout 5s) — observation, no repro.
- diff BEFORE/AFTER one-frame scroll desync when the AFTER pane drives
  the wheel — cosmetic, inherent child draw order.
- ImGui Install/RestoreCallbacks vs chain prev caches can strand a das
  glfw_chain_add_* listener after mute/unmute — latent, zero in-tree
  callers; touching the interleave risks regressing note 38.
- mcp_supervisor.py cannot answer ping while a tool call blocks
  (single-threaded stdin loop); mcp_main.das query values not
  URL-encoded (watcher-generated ids/tokens are URL-safe).

## Block: dogfood round 2 (2026-07-27, rig-start intake)

Found while bringing the rig up on freshly built binaries (master with the
whole arc merged), BEFORE Boris's play session — filed, not fixed.

- 52: the terminal pane keeps rendering a DEAD session's restored content
  while its own header says "No session selected". Repro: restart the
  watcher under an attached client — the client reconnects, the session is
  gone (attached=false, session_id=""), but g_terminal still holds the
  checkpoint-restored screen, so the pane shows a ghost until the client
  restarts. Structured proof: herder_terminal_state reported attached=false
  with 36 non-empty rows, footer "Terminal restored at byte 89489". Fix
  shape: clear the terminal model on detach / on a reconnect that does not
  re-establish the session (reset_terminal exists; the detach path never
  calls it).
- 53: note 47 had a SECOND cause, still open. A checkpoint restored into a
  terminal of a DIFFERENT width strands each wrapped line's first character
  in its own column: observed 'C' at column 119 with "hecking for updates"
  at column 0 on every row, in a 176-column terminal restored from a
  ~120-column checkpoint. That is exactly Boris's original note-47 sighting
  ("stray letters in a one-character column"); the panel-eliding fix covered
  only the Sessions-panel half. Fix shape: the checkpoint ANSI carries its
  own geometry — resize to it before feeding, or reflow after.
- 55: (Boris, live) File Inspector View mode on a .md file — Ctrl+wheel
  "zooms the wrong thing: status bar, not rich text". Mechanism: the
  per-window zoom (window_zoom_scope("inspector"), rich_state.das:899)
  only pushes an ImGui font, which sizes the window's CHROME — labels,
  search box, status line. The content renderers size themselves from an
  explicit `zoom` argument that reads the GLOBAL base zoom instead:
  markdown at rich_inspector_ui.das:415 and its gutter width at :421,
  source/diff views via source_view_style at :84 — all
  `float(g_zoom_percent) / 100.0f`. So Ctrl+wheel over the inspector
  resizes everything EXCEPT the content the window exists to show.
  Not markdown-specific: code in View mode and both Diff panes inherit
  the same miss. FIXED (2026-07-27): the three sites now read
  inspector_zoom() = window_zoom_percent("inspector"), because the base
  zoom already reaches the content through FontScaleMain — passing it
  explicitly applied it TWICE. Measured before: markdown glyphs 1.71x the
  chrome glyphs at base 150%. Measured after: content tracks the window
  zoom (90% -> 19 text lines in an 790px band; 180% -> 8 lines, inked
  rows 202 -> 483) and matches chrome size at the same setting.
  SIBLING, still open: rich_git_ui.das:864 lane_spacing sizes the commit
  graph from the base zoom inside window_zoom_scope("git-activity") —
  same miss, drawn geometry rather than text.
- 56: per-window zoom was mouse-only — Ctrl+wheel adjusted it, but no
  command could set or read it and no state dump exposed it, so neither a
  test nor an agent could verify note 55 either way (parity rule:
  editable-but-not-commandable is a bug). FIXED (2026-07-27): new
  herder_window_zoom rail reads any window's zoom and optionally sets it,
  reporting the base zoom alongside.
- 54: closing a herd session ORPHANS its PTY into the raw session list when
  that PTY ended in a non-exited terminal state. herd_owns_pty skips closed
  records (rich_sessions_ui.das:566) so the PTY reads as "unowned", and the
  raw list hides only the "exited" state, not "failed" — leaving a bare
  warning-glyph row with no context. Seen after closing two probe sessions
  whose hosts had been killed (state=failed, reason=host_lost).
