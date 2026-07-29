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

## Shape: ONE component, one arc, two skins

One arc, not two. The editing core — buffer, cursor, selection, undo,
input, search/replace — is identical for markdown and code; only the
presentation differs, and both presentations already exist as viewers.
The **rich text editor edits markdown SOURCE with a live preview pane**
(the existing markdown viewer) beside it — the shape every good md editor
uses. True WYSIWYG editing inside the laid-out rich text is a different,
much larger beast: named non-goal, revisit only if the split-view editor
proves insufficient in use.

- Component home: `modules/dasImgui/text/imgui_text_source_edit.das`
  (dasImgui repo), the editing sibling of `imgui_text_source_view`.
- Example 1 — **markdown editor**: editor pane + live preview via the
  existing `markdown_view`, debounced re-parse on edit. Lives in dasImgui
  `examples/` (self-contained, no daslang-repo dependency).
- Example 2 — **das code editor**: editor + syntax + LSP hover /
  definition / diagnostics. Lives in daslang `/examples` — it needs
  `utils/lsp/subtools` and a compiler, which only exist there. (If Boris
  prefers both examples in one place, this is the one decision to
  revisit; the component itself does not care.)

## The component, in detail

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

## Order of work (basic editing before LSP, as suspected)

- **E1** Buffer + cursor + virtualised render + typed input. A file
  opens, you type, it shows. No selection yet. dasImgui example
  `features/`-style smoke proves keystrokes → buffer via imgui_snapshot.
- **E2** Selection + clipboard + undo/redo + the keymap. The component is
  now an honest plain-text editor.
- **E3** Search/replace/replace-all (the standing rule lands here, not
  later).
- **E4** Syntax-while-editing (fallback immediate + debounced full pass).
- **E5** The two examples assembled: markdown split-view editor
  (dasImgui), das code editor (daslang /examples). Both carry save,
  dirty-state, and the find widget.
- **E6** LSP in the code editor: hover + definition reuse the viewing
  arc's spawn-per-request transport verbatim (`nav.das`, overlay = the
  unsaved buffer — the SAME `--overlay` flag, no new machinery);
  diagnostics via `validate.das` on the debounce, squiggles from the
  span layer. Completion stays in LANGUAGE_SUPPORT_PLAN step 6 with the
  profelis floor.
- **E7** dasHerd binds it: PR body (markdown instance, no preview pane
  needed at first) and conflict resolution (ours / base / theirs +
  editable result, per-conflict accept-ours / accept-theirs / edit,
  save writes and stages — the changelist already refuses to discard
  conflicts).

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
