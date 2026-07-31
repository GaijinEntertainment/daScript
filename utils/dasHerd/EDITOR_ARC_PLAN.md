# The editor arc — plan of record

Status: PLANNED 2026-07-29, not started. Detailed plan for the editor
component named in LANGUAGE_SUPPORT_PLAN.md Step 4. Written to be
implementable from cold context; every file and fact named here was
verified against the tree on the day it was written.

Where this comes from. Boris: an integrated code editor — with LSP, for
das — is a feature users have asked for; dasHerd (conflict resolution, PR
body) is the first customer, not the definition. Basic editing comes
before LSP integration. Editors get search / replace / replace-all from
day one (the standing rule). And the constraint: **no .cpp changes
expected — if any arrive, Boris is told before they land.**

## The .cpp answer, up front (verified 2026-07-29)

Everything the editor needs already crosses the binding:

- **Typed input**: `modules/dasImgui/terminal/imgui_terminal.das:514`
  iterates `io.InputQueueCharacters` in pure das — the terminal widget is
  an existing, battle-tested full-keyboard consumer (codepoints, Ctrl
  chords, key repeat via `IsKeyPressed(key, true)`). The editor reuses
  that exact pattern, and the `codepoint_utf8` helper moves to a shared
  module instead of being copied a third time.
- **Clipboard**: the `clipboard` module (get/set with verification) is
  already used by dasHerd and the source view's copy path.
- **Drawing**: the virtualised text pass in `text_source_view` /
  `imgui_terminal` proves per-line drawlist rendering at 120 FPS.
- **Syntax**: `syntax_highlight` (native tree-sitter) + the das span
  pipeline, fast after the 2026-07-29 fixes (716 KB ≈ 360 ms native).

**One named residual risk**: tree-sitter *incremental* re-parse (per
keystroke on very large files) would need new C++ binding surface. V1
deliberately avoids it — debounced full re-parse plus an immediate
per-line lexical fallback — so the risk stays theoretical. If profiling
ever demands incremental parsing, that is the moment Boris hears about a
.cpp change, before it happens.

**.cpp change record (2026-07-30, fold slice):** the fold feature needed
node-level access the native surface lacked — ONE new extern in
dasTreeSitter (`_tree_sitter_structure`: parse + cursor walk, callback per
named multi-line node with type/bytes/rows, mirroring `_tree_sitter_highlight`'s
shape) + its `syntax_structure` das wrapper. Foldable node types stay
das-side data (`g_fold_types` per language in `imgui_text_tree_sitter`) —
the "config" Boris asked about is a table in the module, not a file.

**Fold slice status (shipped 2026-07-30):** source view folds das/C/C++/
markdown regions (gutter chevrons, hidden rows, line numbers keep true
values); markdown view collapses code blocks (hover chevron + copy-all
icon, one-line stand-in bar); find auto-expands through collapsed regions
in both views. Known v1 bounds: no folding in the VIRTUAL source path
(files >= 128 KB) and none in wrap mode; collapse state resets when the
document revision changes; code-block interiors still have no selection
highlight (the E2/E4 gap above).

**E1 status (shipped 2026-07-30):** `imgui_text_source_edit.das` carries the
line-buffer core (load/source round-trip incl. CRLF flavor, grapheme-aware
cursor + insert/backspace/delete, vertical moves with preferred column) and
the widget (InvisibleButton hit target + own virtualised drawlist rows,
terminal-pattern typed input, scroll-follow, blink caret, current-line tint,
line-number gutter). Preview: `examples/features/text_edit.das`; tests:
`test_text_edit_model` (pure logic) + `test_text_edit` (keystrokes → buffer
via snapshot — the registered state serializes the whole `lines` array).
`codepoint_utf8` moved to `imgui_text_flow` (terminal now shares it); the
view's tab-expanded draw/measure helpers went public
(`text_source_draw_range` / `text_source_range_width`). Bug found by the
smoke and fixed: drawing against the pre-input line count crashed on
line-join edits — the draw section recounts after input runs.

**E2 status (SHIPPED 2026-07-30, commits bcf40261f..dc9edcc8e):** the
component is an honest plain-text editor with every binding editable.
Landed per the spec: window-focus keyboard arbitration; selection model
(Shift-nav select twins, collapse rules, mouse drag/double-word/
triple-line select, per-row rects with newline nub); one-predicate word
boundaries (Ctrl+Left word starts w/ lone-separator skip, Ctrl+Right
word ENDS, whitespace-heuristic word deletes); the undo journal
(commit_edit primitive, word-sized coalescing, adjacency break on caret
jumps, Enter/paste fences, public push_undo_stop, selection restore,
redo cleared on edit); clipboard verbs with line-mode copy/cut/paste +
the six CUA/modern chords; numpad twins in ALL modifier combos with
char-queue NumLock disambiguation; insert/overtype with block caret;
and the keymap routed through imgui_commands (second-chord slot,
collision-safe polling, capture-sentinel OOB fix, command_take_prefix,
JSON-persisted bindings editor in the demo). Synth layer grew
key_chord_tap / key_hold + named-key imgui_key_chord. Tests: edit model
22, smoke 2 (registry-routed chords incl. keypad + overtype), commands
3, viewer 8, terminal 1 — all green. Known E2 residuals: drag after
double-click extends by characters (not words); Shift+keypad selection
under NumLock-ON Windows fake-Shift not yet probed on hardware;
multi-editor invocation routing by context field deferred; the base
(registry-less) overload keeps the built-in keymap by design.

**E3 status (SHIPPED 2026-07-30, commits ce7cb09d8 + 93a921cbd):**
replace rides the find bar. Find module: replace row (toggle chevron,
input, Replace/All buttons; Enter = replace one, Ctrl+Alt+Enter =
replace all; Esc closes), request counters the host applies, per-match
template expansion in regex mode ($0/$&, $1-$9, ${n}/${name}, $$ —
mirrors daslib/regex's private expand_replacement). Editor: byte↔
(line,column) mapping over the LF-joined `text_edit_text` domain,
`text_edit_replace_range` / `text_edit_replace_all` (ascending spans
applied back-to-front as ONE undo unit), match highlights painted under
the text (`match_color`), and `text_edit_find_sync` — the one-call pure
glue (rebuild, apply requests, select current match, refresh
highlights; replace-one resumes PAST its insertion so self-matching
replacements can't pin). find.* commands in the registry (Ctrl+F seeds
from single-line selection, Ctrl+H opens replace focused, F3/Shift+F3,
Escape closes BEFORE edit.cancel — registration order is the Escape
chain). Bug found by the smoke and fixed: component-scope Shortcut
routing follows ImGui item focus, which the bar's inputs kept after
close — the editor now reclaims item focus (FocusItem) whenever its
model owns the keyboard, which is also VS Code's Escape-returns-focus
behavior. Tests: edit model 32, find model 9, smoke 3 — all green.
Known E3 residuals: F3 doesn't step while typing in a bar input (Enter
covers it); Ctrl+F/Ctrl+H only fire while the editor is focused
(component scope — application-scope twins are a host decision); no
case/whole-word toggles yet (find bar is case-insensitive literal or
regex, as shipped in the viewer arc).

## Shape: ONE component, one arc, two skins

One arc, not two. The editing core — buffer, cursor, selection, undo,
input, search/replace — is identical for markdown and code; only the
presentation differs, and both presentations already exist as viewers.
The **rich text editor edits markdown SOURCE with a live preview pane**
(the existing markdown viewer) beside it — the shape every good md editor
uses. True WYSIWYG editing inside the laid-out rich text is a different,
much larger beast: named non-goal, revisit only if the split-view editor
proves insufficient in use.

- Component home: `modules/dasImgui/text/imgui_text_source_edit.das`,
  the editing sibling of `imgui_text_source_view`.
- **The example home is `examples/text` (amended 2026-07-30, Boris).**
  The existing viewer — already a docked multi-document app with both
  presentations (rendered Markdown + syntax-highlighted source), command
  registry, live rails, and a dastest suite — is the parallel preview app
  for every feature slice: search, LSP, editing. It accretes the arc
  instead of new `examples/editor/*` apps being built beside it; whether a
  separate minimal code-editor example is still worth shipping is decided
  at E5, not before. (The dasImgui merge landed 2026-07-30 — one repo,
  the old staging caveat is gone.)

## The component, in detail

> **EDITOR_EDITING_SPEC.md** (2026-07-30) is the spec of record for command
> semantics, default chords, word-boundary rules, the undo model, numpad
> aliasing, and the imgui_commands integration — research-backed (MS canon +
> VS Code/VS/Sublime/JetBrains + our infra map). Where the sketches below
> differ, the spec wins.

### The API is the product (Boris, 2026-07-30)

Every editing capability is a PUBLIC, programmatic verb on the state —
`undo`, `redo`, `delete`, `select`, `type`/`insert`, `navigate_to`, word
moves, line ops, search/replace — usable by anyone building their own
editor on the component, with no ImGui frame required. The keymap is just
one CLIENT of that verb surface (the live rails and tests are two more);
a chord never carries logic of its own, it only names a verb. E1 set the
shape (`text_edit_insert`/`text_edit_move_*`/... are public pure ops
driven headless by tests); every E2+ capability lands verb-first, then
gets its binding.

### Buffer

- `array<string>` of lines, owned by the editor state. Edits are
  line-local operations; no per-keystroke re-join of the whole document.
  (A gap buffer is deliberate over-engineering at our sizes — the perf
  work proved the costs are in scans and allocations, not in line edits.)
- All positions are (line, byte_column), UTF-8-aware movement using the
  same byte-walk helpers the LSP subtools use (`lsp_common` owns
  byte↔UTF-16; the editor needs only byte↔codepoint).
- Load: split source into lines once. Save: join + write via fio, with
  the dirty flag and the host told (dasHerd stages conflict saves; the
  examples just write).

### Cursor, selection, input

- One cursor + one anchor (selection = cursor..anchor). Multi-cursor is a
  named non-goal for v1.
- Input loop is the terminal's, retargeted: focused child window,
  `io.InputQueueCharacters` for text, `IsKeyPressed(key, true)` for
  moves/edits with repeat, `io.KeyCtrl/KeyShift/KeyAlt` chords. Standard
  keymap: arrows/home/end/pgup/pgdn (+Shift extend), Ctrl+arrows word
  moves, Ctrl+A/C/X/V, Tab/Shift+Tab indent, Ctrl+Z/Y.
- Mouse: click places cursor, drag selects, double-click word,
  triple-click line — same hit-testing the source view's selection
  already does.

### Undo

- Operation journal: `{kind = insert | remove, line, column, text}` with
  inverse application; typing runs coalesce (same line, adjacent column,
  no boundary event); cursor jumps, paste, and edit-type changes start a
  new group. Redo stack cleared on new edits. Undo depth unbounded —
  operations are small and the journal is the future basis for
  edit-tracking features.

### Search / replace (day one, per the standing rule)

- The find widget from the viewing arc (VSCode feature set: regex / case /
  whole-word, count, prev/next, Ctrl+F/Esc) plus the editor-only half:
  replace one / replace all, both running through the undo journal as a
  single group so one Ctrl+Z reverts a replace-all.
- `daslib/regex` is the engine (the `skills/regex.md` surface).

### Syntax while editing

- Immediate: the edited line (and its neighbours when a multiline token
  is suspected — open string/comment) restyled by the per-line lexical
  fallback, so typing never waits.
- Debounced (~300 ms idle): full re-parse via `syntax_highlight`, spans
  mapped back per-line. After the perf fixes this is ~ms for normal files
  and well under a second for 700 KB monsters, off the UI thread via the
  same jobque worker pattern the inspector prepare uses.

### Rendering

- Virtualised: visible lines only, drawlist text runs per style cell —
  the exact discipline that took the terminal from 3 to 120 FPS. Gutter
  with line numbers; current-line tint; selection rects; a blinking
  cursor drawn from the style's cursor color.

**Known view gap (found 2026-07-30, find slice):** code-block nodes in the
markdown view have NO inline selection paint and no caret — selection
positions map (runs exist) but nothing draws, and `selection_screen_valid`
stays false. Find scroll-anchors through the focus node's flow line
(`markdown_view_selection_focus_screen_y`) as the fallback; the visible
highlight inside code blocks lands with the selection work in E2/E4.

## Order of work (basic editing before LSP, as suspected)

- **E1** Buffer + cursor + virtualised render + typed input. A file
  opens, you type, it shows. No selection yet. dasImgui example
  `features/`-style smoke proves keystrokes → buffer via imgui_snapshot.
- **E2** Selection + clipboard + undo/redo + the keymap — per
  EDITOR_EDITING_SPEC.md Tier 1. Work order within the slice (each
  sub-step commits + tests headless): (1) key-owner focus fix
  (`SetItemKeyOwner`, kills the two-editors-both-focused bug); (2)
  selection model + Shift-extended nav + mouse word/line click-select;
  (3) word boundaries (one predicate) + word moves/deletes; (4) undo
  journal (fences, kind-coalescing, adjacency break, `push_undo_stop`
  public from day one); (5) clipboard incl. the six chords + empty-
  selection line mode; (6) the keymap through imgui_commands (registry
  extensions: second chord per binding, collision safety, capture-tick
  sentinel fix, ALLOWED_IMGUI additions) + numpad aliases + smart Home +
  overtype; (7) synth-layer extensions (named-key chords, key_hold) so
  the chord tests read cleanly. The component is then an honest
  plain-text editor with every binding editable.
- **E3** Search/replace/replace-all (the standing rule lands here, not
  later).
- **E4** Syntax-while-editing (fallback immediate + debounced full pass)
  plus editing polish — scope set by Boris's brain dump (2026-07-30):
  1. **Language profile** (the mechanism the rest hangs off): per-language
     editor settings — indent rules, bracket pairs, keyword list, fold
     kinds. NOT keybindings (those stay global). Hardcoded first-class
     das/C++/md profiles are fine; data-driven-ness is an open question,
     not a requirement.
  2. **Bracket-aware auto-indent on Enter**: new line lands indented;
     counts `{ }` — deeper after `{`, and typing `}` dedents the line.
     Per-language on/off + which symbols (md: off; some languages may
     indent with no symbol at all).
  3. **Bracket-pair highlight**: cursor on `[ ( {` or a closer highlights
     the counterpart; pairs come from the language profile.
  4. **Basic completion, from the get go**: candidates = words in this
     document + the language's keyword list (+ words across the
     language's open documents); ghost-text as-you-type (type `hel`,
     greyed completion shows), Tab accepts. LSP tier waits for E6; LLM
     tier is a recorded follow-up (below), not pressing.
  5. **Go-to-line dialog** (very important) + permanent Ln/Col readout in
     the editor chrome.
  6. Leftover Tier-2: join lines, double-click-drag word-granular extend,
     drag-and-drop selection move.
  7. **Inline color embed** (tail of E4): `0x12AF2A` literals (with and
     without alpha) get a swatch + real ImGui color dialog writing back —
     the proof case that the editor embeds live widgets over source
     ranges.
  Settled in discussion (2026-07-30, no pushback): language profile is a
  plain das struct with hardcoded das/cpp/md constructors (not
  data-driven); syntax-while-editing lands BEFORE auto-indent so the
  lexical pass tells the indenter which braces are real (`// {` and
  `"{"` must not count); ghost-text completion defaults ON with a
  per-language kill switch. All revisable if it feels wrong in use.
  Settled 2026-07-31 (Boris: "now, for sure"): the E6 provider interface
  is built IN E4 as the completion carrier from day one —
  document-words+keywords is provider #1; LSP and LLM slot in later
  without touching the editor, and the interface grows hover/definition
  arms in E6. The language profile ABSORBS the per-language data already
  scattered in the tree — what's collapsible (`g_fold_types` in
  imgui_text_tree_sitter), extension→language mapping, bracket pairs,
  keyword lists — one home, the modules consume the profile instead of
  private tables. Tab default: a visible ghost suggestion accepts on
  Tab; otherwise Tab keeps its indent behavior (the VS Code rule).
  Settled 2026-07-31 (second round): the provider interface ships with
  TWO REAL BACKENDS in E4, minimal but real, to keep it honest — .das
  calls the compiler infrastructure borderline directly (in-process
  worker, das data back, no protocol tax; utils/lsp internals minus the
  protocol), .cpp does one honest textDocument/completion round-trip to
  an external clangd. Each guards a different lie: the external one
  forces spawn/lifecycle/latency/cancellation into the interface, the
  local one forbids baking protocol assumptions in. Async-first shape
  either way: request → token, poll/cancel, results land in the frame
  loop (jobque worker + clone-back for the local provider). Depth and
  quality stay E6; E4 proves the interface against both extremes.
- **E4-follow-up (recorded, not pressing): LLM completion tier.** Own
  section when it comes: API port setting, async request, model picked
  for fill speed, model-per-language setting; basic-basic completion.
- **E4-follow-up (Boris, 2026-07-31, "feels later"): snippets +
  format-on-type.** The observed gap: an accepted completion lands
  unformatted next to auto-closed braces (no space after `{`). Two
  standard mechanisms, both recorded: (1) TextMate-style snippet
  completions (tab stops/placeholders — the format VS Code and LSP
  adopted verbatim; slots into the provider interface as items carrying
  templates instead of words); (2) format-on-type — reformat the
  construct just closed (on `}` / Enter). (2) is uniquely cheap here:
  we OWN das_source_formatter in-process, no LSP hop — "least
  keystrokes to formatted code" (Boris's energy-compaction framing;
  the KLM/GOMS literature is the study lineage) likely lands as
  format-on-type first, snippets second.
- **Post-LSP tail (Boris): block/column selection** — Alt+mouse drag +
  multi-line insert. Deliberately late: the undo journal already carries
  multi-op units and selection lives inside TextEditState, so nothing
  architectural forces it early.
- **E5** `examples/text` becomes an honest editor: the component wired
  into both its presentations (markdown source with live preview beside
  it, plain/code source), carrying save/load, dirty-state (undo back to
  the save point clears dirty), and the find widget. Decide HERE whether
  a separate minimal code-editor example still earns its keep or the
  viewer covers it.
- **E6** LSP in the code editor: hover + definition reuse the viewing
  arc's spawn-per-request transport verbatim (`nav.das`, overlay = the
  unsaved buffer — the SAME `--overlay` flag, no new machinery);
  diagnostics via `validate.das` on the debounce, squiggles from the
  span layer. Completion stays in LANGUAGE_SUPPORT_PLAN step 6 with the
  profelis floor.
  **Completion plan of record (Boris, 2026-07-31, mid-E4): the daslang
  VS Code plugin does completion amazingly well — EXTRACT that code,
  generalize it into daslib somewhere, and use it in the LSP for
  completion.** Until then the E4 heuristics (tiers, statement-start
  keyword boost) are FROZEN — good enough, no more position jazz; that
  intelligence belongs to the LSP tier when it lands.
  **Provider abstraction (Boris, 2026-07-30, dropped mid-E3): the editor
  talks to an LSP-LIKE interface, not to LSP.** One abstracted
  language-intelligence surface (hover / definition / diagnostics /
  completion verbs); per-language providers plug in behind it — for .das
  some requests answer from tree-sitter, others from the compiler; C++
  may route to a real LSP server, and even .das could. Completion goes
  through the SAME route: the E4 document-words/keywords tier, the LSP
  tier, and the LLM follow-up are providers behind one interface, not
  special cases.
- **E7** dasHerd binds it: PR body (markdown instance, no preview pane
  needed at first), conflict resolution (ours / base / theirs +
  editable result, per-conflict accept-ours / accept-theirs / edit,
  save writes and stages — the changelist already refuses to discard
  conflicts), and the **git change gutter** (Boris): green/red/modified
  line marks against HEAD beside the editable buffer, updating as you
  type — the viewer's line-mark gutter + the inspector's diff pipeline
  already produce exactly these marks.

Each step ends the usual way: lint clean, suite green, live drive of the
example through the rails, commit.

## Tests

- Pure-logic unit tests (no ImGui): buffer ops at boundaries (empty file,
  line joins/splits, end-of-file), UTF-8 cursor movement over multibyte
  fixtures, undo coalescing and group boundaries, search/replace with
  regex + replace-all-as-one-undo. Same style as
  test_changelist_selection.das.
- Integration: the examples run headless; synthetic keystrokes through
  the terminal-proven input path, buffer state read back via a
  `editor_state` live rail (the parity rule: every editor capability
  commandable + inspectable — cursor, selection, dirty, undo depth,
  search state).
- The conflict-resolve binding gets a scratch-repo test in the
  changelist suite: construct a real conflict, resolve via the editor's
  save-and-stage path, assert porcelain.

## Non-goals (v1)

- WYSIWYG in-layout markdown editing (split view instead, see above).
- Multi-cursor, code folding, minimap.
- Incremental tree-sitter parse (the one named .cpp risk — avoided).
- Editing inside the DIFF panes (the conflict view edits the RESULT
  buffer only).
