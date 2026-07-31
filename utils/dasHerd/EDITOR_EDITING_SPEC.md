# Editor editing spec — commands, semantics, bindings

Status: SPEC OF RECORD, written 2026-07-30 from a three-way research pass
(Microsoft canon: Win32 EDIT/RichEdit/WinForms/WPF documented behavior;
advanced editors: VS Code source + Visual Studio/Sublime/JetBrains/Scintilla;
our infra: imgui_commands + GLFW/ImGui key routing). Companion to
EDITOR_ARC_PLAN.md — that file says WHEN, this file says WHAT each command
does. Boris's framing: "the works" — full fundamental editing, every command
a public verb, every chord editable.

Principles inherited from the plan: **the API is the product** (each command
is a public `text_edit_*` verb on the state, callable headless; the keymap
only names verbs) and single-cursor v1 (multi-cursor is a named non-goal;
selection lives in TextEditState, so growing it to an array later changes
state layout, not verb signatures).

## The three forks, decided

Research found exactly three places where the major editors genuinely
diverge in SEMANTICS. Decisions of record:

1. **Ctrl+Right lands at the START of the next word** (Windows convention:
   documented `WB_MOVEWORDRIGHT` semantics, symmetric with Ctrl+Left; used
   by Visual Studio, Word, Notepad, Chrome). VS Code's land-at-end is a
   deliberate deviation with measured cost (7 presses vs 4 crossing
   `this.is.a.test`). We keep BOTH primitives (`word start` and `word end`)
   so the binding can be flipped later; the default follows Windows.
2. **Tab with a partial single-line selection REPLACES the selection**
   (VS Code's TabOperation); a full-line or multi-line selection
   block-indents. Empty selection inserts an indent unit (tab or spaces to
   the next stop).
3. **Caret movement BREAKS the undo typing run.** RichEdit documents it,
   users expect it; VS Code's don't-break is its most complained-about undo
   behavior. Mechanism: the adjacency rule (new edit not at the last edit's
   end position → new undo unit), which subsumes clicks, arrow moves, and
   programmatic jumps without tracking navigation events.

## Command inventory (the verb list)

Every row is a public verb on `TextEditState` plus a command id registered
in `imgui_commands` with the listed default chord(s). Movement verbs take
`select : bool` (Shift adds selection; NOT doubled command ids). All chords
are rebindable in the standard bindings editor; JSON persistence as today.

### Tier 1 — fundamental (E2)

| Verb / command id | Default chords | Semantics |
|---|---|---|
| `cursor_left` / `cursor_right` | Left / Right (+Shift select) | ±1 grapheme; crosses lines; with a selection and no Shift, COLLAPSES to the selection edge (no move) |
| `cursor_up` / `cursor_down` | Up / Down (+Shift) | ±1 line, preferred-column memory (E1 has this); collapses selection first |
| `cursor_word_left` / `cursor_word_right` | Ctrl+Left / Ctrl+Right (+Shift) | to word START both directions (fork 1); single-separator runs are skipped (the `foo.bar` rule: a lone `.` between words gets no stop; `->`, `::` do); line end is a hard stop; at column 0 / line end the move wraps to the neighboring line |
| `cursor_home` | Home (+Shift) | SMART: to first non-blank; if already there, to column 0 (toggles) |
| `cursor_end` | End (+Shift) | to line end |
| `cursor_document_start` / `_end` | Ctrl+Home / Ctrl+End (+Shift) | buffer start / end |
| `cursor_page_up` / `_down` | PgUp / PgDn (+Shift) | ± viewport rows, preferred column kept |
| `select_all` | Ctrl+A (alias Ctrl+KP5) | whole buffer |
| `cancel_selection` | Escape | collapse selection, keep caret. Escape is CHAINED: find bar open → close find; else selection → collapse; else NOT consumed |
| `cut` / `copy` / `paste` | Ctrl+X / Ctrl+C / Ctrl+V **and** Shift+Del / Ctrl+Ins / Shift+Ins | all six are documented first-class Windows defaults. **Empty-selection rule (both halves):** cut/copy with no selection take the WHOLE line incl. newline and tag the clip as line-mode; pasting a line-mode clip inserts at column 0 of the caret line, pushing it down, caret position preserved |
| `undo` | Ctrl+Z, Alt+Backspace | see undo model below |
| `redo` | Ctrl+Y, Ctrl+Shift+Z | both spellings; new edits clear redo |
| `delete_left` / `delete_right` | Backspace / Delete | selection if any, else one grapheme; joins lines at boundaries (E1 has this) |
| `delete_word_left` / `delete_word_right` | Ctrl+Backspace / Ctrl+Delete | see word rules below |
| `toggle_overtype` | Insert | typed chars replace the char to the right, within the line only (never eats a newline); paste still inserts; selection typed-over is replaced normally; block caret while active; starts in insert mode |
| `type` / `insert_text` | (printable input path) | typed characters NEVER route through the binding table — they come from the io char queue as the `type` verb. Ctrl+Backspace's phantom 0x7F char (and control chars generally, cp < 0x20 / 0x7f) are filtered — E1 already does |
| `newline` | Enter, KeypadEnter, Shift+Enter | paragraph break; auto-indent lands here in E4 |
| `tab` / `outdent` | Tab / Shift+Tab | fork 2 rules; outdent works with empty selection too |

### Tier 2 — expected in a code editor (E4, with the polish pass)

| Verb / command id | Default chords | Semantics |
|---|---|---|
| `delete_lines` | Ctrl+Shift+K | delete every line touched by the selection, incl. newline |
| `move_lines_up` / `_down` | Alt+Up / Alt+Down | move touched block ±1 line; no-op at edges; selection follows |
| `duplicate_lines` | Shift+Alt+Down / Shift+Alt+Up | empty sel: duplicate line, caret to copy; else duplicate selection and select the copy |
| `join_lines` | (unbound; editable) | strip joined line's leading whitespace, single-space seam, no space if either side empty |
| `insert_line_after` / `_before` | Ctrl+Enter / Ctrl+Shift+Enter | open a blank correctly-indented line without splitting |
| `indent_lines` / `outdent_lines` | Ctrl+] / Ctrl+[ | unconditional block indent/outdent. Block rules copied verbatim from VS Code's ShiftCommand: selection ending at column 0 of a later line EXCLUDES that line; empty lines skipped on indent, processed on outdent |
| `goto_line` | Ctrl+G | line(:column) prompt |
| `scroll_line_up` / `_down` | Ctrl+Up / Ctrl+Down | scroll viewport WITHOUT moving the caret (code-editor convention beats WPF's move-by-paragraph) |
| `delete_to_line_start` / `_end` | (unbound; editable) | delete caret→line start / end |

Tier 3 (ship the verbs unbound, or defer): subword/camelCase moves + deletes
(`cursor_word_part_*` — the hump/underscore boundary rules are in the
research log), `jump_to_bracket`, `transpose`, case transforms, sort lines,
`cursor_undo` (selection-history undo), column selection, multi-cursor.

### Mouse (E2)

Click places caret (E1 has); Shift+click extends from anchor; drag selects;
**double-click selects word** (same word classifier) and drag-after extends
by whole words; **triple-click selects line**, drag extends by lines; drag
starting inside a selection = drag-and-drop move of the selected text
(deferred to E4 if fiddly). Auto-scroll at viewport edges while dragging.

## Word boundaries — ONE predicate

Three character classes: **whitespace** (space/tab), **word** (alphanumeric
+ underscore + all non-ASCII), **separator** (the VS Code default set:
`` `~!@#$%^&*()-=+[{]}\|;:'",.<>/? ``). A word is a maximal run of word
chars; boundaries at class transitions; newline is its own hard stop. The
SAME predicate drives word-left, word-right, delete-word-left, and
delete-word-right (Firefox's "Ctrl+Backspace erases the whole URL" bug came
from diverging predicates — never let them drift).

Delete-word semantics (VS Code's whitespace heuristic, exact):

- `delete_word_left`: if ≥2 whitespace chars sit immediately left of the
  caret, delete ONLY that whitespace run. Else delete from the previous
  word's start to the caret (a single intervening space goes with the
  word). At column 0: join with the previous line.
- `delete_word_right`: mirror (first non-whitespace forward; deletes to
  word END; at line end joins the next line up).
- With a non-empty selection, both simply delete the selection.

Grapheme clusters stay the character unit everywhere (E1 already).

## Undo model

Journal of inverse-applicable units. A unit:

```
{ ops (inverse edit list), before_cursor+selection, after_cursor+selection,
  kind, last_edit_end : (line, column), last_edit_time }
```

Kinds: `insert`, `insert_space`, `delete_back`, `delete_forward`, `other`.

**A new edit APPENDS to the open unit unless any of:**

1. **A fence was pushed.** `text_edit_push_undo_stop()` is public from day
   one; every compound command uses it (paste fences before AND after;
   Enter fences before; every line op / replace / replace-all is one unit).
   Replace-all = fence + one multi-range edit + fence — never heuristics.
2. **Kind changed** — with the two VS Code refinements: consecutive spaces
   bucket together, and the FIRST space after a word does not break (so one
   unit ≈ one word: `abc ` then `d` breaks between `abc ` and `d`).
   Backspace interleaved with typing DOES break (delete_back ≠ insert) —
   stricter than RichEdit, matches VS Code; keeps units word-sized.
3. **Adjacency broke** (fork 3): the edit does not start where the last
   edit ended — clicks, arrows, jumps all break the run with zero extra
   tracking.
4. Optional timeout (>500 ms since last edit) — field exists, rule can land
   later.

Undo: close open unit, pop past, apply inverse, **restore before-selection,
scroll caret into view**, push future. Redo mirrors with after-selection.
**Any new edit clears the redo stack** (linear model — no undo tree).
`text_edit_load` (wholesale SetText) CLEARS history; programmatic
`text_edit_replace(range, text)` PARTICIPATES in it. The dirty flag pairs
with a saved-revision marker so undo back to the save point clears dirty.

## Bindings — riding imgui_commands

The registry (`modules/dasImgui/commands/imgui_commands.das`) already gives
us: bare-key bindings (F3 precedent), any ImGuiKey incl. keypad, per-binding
key-repeat, `Shortcut()` exact-modifier matching + focus routing, the
capture-based bindings editor, and JSON persistence to
`%APPDATA%/daScript/<app>-bindings.json`. The editor keymap routes through
it: `text_edit_register_commands(registry)` defines every verb above;
`text_source_edit` polls component scope INSIDE its focus gate and drains
`command_take` into verbs. Extensions needed (from the infra pass):

1. **Key-owner focus fix first** — E1's `ed.focused` is only cleared by a
   click elsewhere (two editors can both claim the keyboard); replace the
   `IsAnyItemActive` heuristic with `SetItemKeyOwner` on the editing keys +
   `IsWindowFocused` gating. Do this before wiring bindings so routing bugs
   and gating bugs stay distinguishable.
2. **Second chord per binding** (schema: `alt_key` + its mod bits) — needed
   for the numpad aliases and the CUA clipboard/undo alternates. The
   bindings editor grows a second capture slot per row.
3. **Collision safety** — `command_poll_scope` currently double-fires two
   commands bound to one chord; first-match-wins per chord + keep the
   editor-side conflict label.
4. `repeat = true` on all navigation/delete bindings.
5. Housekeeping in the same slice: `command_capture_tick` iterates enum
   sentinels (None/NamedKey_END/COUNT) into an OOB `IsKeyPressed` — filter
   them; add `Shortcut`/`SetItemKeyOwner` to ALLOWED_IMGUI; teach
   `imgui_key_chord` named keys ("RightArrow") and add a `key_hold`
   playwright helper so tests can express Ctrl+chords and wall-clock
   repeat.

## Numpad

GLFW reports keypad keys by scancode — `ImGuiKey.Keypad*` arrives regardless
of NumLock, and NumLock state is unreachable through GLFW→ImGui (stripped,
then discarded). Per MS UX guidance ("don't distinguish between key
locations"), keypad nav keys alias their dedicated twins IN ALL MODIFIER
COMBOS via the second-chord slot:

KP8/2/4/6→arrows · KP7/1→Home/End · KP9/3→PgUp/PgDn · KP0→Insert ·
KP.→Delete · Ctrl+KP5→select all · KPEnter→Enter (already). Ctrl/Shift
compose through the same bindings (Ctrl+KP4 = word left, Ctrl+KP1 =
document end, Ctrl+KP. = delete word right).

NumLock-ON typing: a keypad digit press also delivers its char event; the
editor drains chars FIRST and skips keypad nav bindings on any frame whose
char queue was non-empty — NumLock ON types digits, NumLock OFF navigates,
no lock-state API needed. **Known Windows hazard to probe empirically:**
with NumLock ON, Windows injects a synthetic Shift-release around
Shift+keypad presses, so `io.KeyShift` may read false during Shift+KP8 —
verify on our stack; if it bites, Shift+keypad selection gets documented as
NumLock-OFF-only (matching stock Windows controls).

## Escape and keys we do NOT own

Escape follows the chain rule (find → selection → unconsumed — the host owns
transient-UI dismissal beyond that). The component leaves to the host:
Ctrl+PgUp/PgDn (tab switching), Ctrl+L/E/R (find/address-bar muscle memory),
Ctrl+wheel zoom (host gesture today). Alt-held keypad input (Alt+nnn
composition) is OS-level on Windows; the composed char arrives through the
normal char queue and just works — never treat keypad keys as nav while Alt
is held.

## Test hooks

Every verb headless-testable on TextEditState (extend test_text_edit_model);
chords tested through the registry with the extended synth layer (named-key
chords, key_hold repeat, keypad char+key pairs to prove the NumLock
disambiguation). Repeat counts are timing-dependent — assert monotonic
deltas, never exact counts.
