# Language support in the inspector — plan of record

Status: PLANNED 2026-07-29, not started. Written to be implementable from
cold context: every file, tool and flag it names was verified against the
tree on the day it was written.

**Amendment 2026-07-30 (Boris): `examples/text` is the parallel preview
app.** The existing text/Markdown viewer (brought up to date the same day:
new-lint clean, suite green) previews EVERY feature this arc and the editor
arc add — search, LSP hover/definition, editing — in parallel with the
dasHerd integration. Each slice lands in the reusable module component,
shows up in `examples/text`, and then dasHerd binds it; the example is the
demo and the test bench, not an afterthought. dasImgui is in-tree now, so
this is all one repo.

Where this comes from. Boris, reviewing the changelist arc live: "when i
review changes - in .das file - i'd like at least a viewing experience -
similar to what i get in vscode with our plugin. i mouse over - i get hint.
i have 'go to definition', and other lsp features. this would significantly
help me with review. this should be way better than vscode - because in
'diff' mode i should still get the same lsp benefits." And on editing:
justified in exactly two places — the PR-body .md for the coming GitHub-PR
assistant, and manual conflict resolution, because "its more typing to
explain llm what to keep."

## What already exists (verified 2026-07-29)

- `utils/lsp/subtools/nav.das` is a STATELESS spawn-per-request tool:
  `daslang utils/lsp/subtools/nav.das -- hover <file> <line0> <char16>
  [project]` — also `definition`, `references`, call hierarchy. Argv in,
  LSP-shaped JSON on stdout, exit. This is the same shape as every git
  capture the watcher already runs.
- `--overlay <path>` makes the subtool compile the op's file argument from a
  TEMP FILE instead of disk (`set_file_source`), with positions mapped
  against that text. Unsaved buffers already work; a HISTORICAL buffer is
  the same trick — write the git blob to the overlay. This single flag is
  what makes diff-mode LSP possible, including the OLD side no editor gives
  you.
- The "no resident daslang" architecture is LOCKED (`skills/daslang_lsp.md`)
  for macro-state, lock, and crash-isolation reasons. dasHerd inherits that
  decision by spawning subtools per request; it does NOT need
  `lsp_supervisor.py` (that layer exists to speak LSP framing to editors —
  dasHerd is not an editor, it wants argv→JSON).
- Coordinates: daslang LineInfo is 1-based lines / 0-based BYTE columns;
  LSP-side is 0-based lines / UTF-16 code units. `subtools/lsp_common.das`
  owns both conversions — dasHerd must NOT reimplement them.
- The inspector already knows, per diff row, the old/new line numbers
  (`prepared_*.line_numbers`) and renders through `text_source_view`, which
  reports hover position. The prepare worker (`inspection_worker.das`) runs
  off-thread with generation guards — LSP requests ride the same pattern.
- The watcher task-session rail gives dasHerd process-spawn + capture with
  timeouts today; for LOCAL subtool calls the client can also spawn
  directly (popen_argv) since the daslang binary is local by definition.

## Which tree answers (added 2026-07-29, Boris's review)

The subtool runs IN a worktree, and dasHerd shows MANY worktrees. An answer
computed by the wrong tree's binary is the cross-tree staleness bug the MCP
tools already warn about (#3392): wrong module sources AND wrong compiled-in
C++ bindings. So resolution is per-request, from the worktree being viewed:

- Preferred: `<worktree>/bin/Release/daslang(.exe)` running
  `<worktree>/utils/lsp/subtools/nav.das` — that tree's binary, that tree's
  subtool, that tree's daslib. Both paths verified per worktree ONCE and
  cached until its HEAD moves.
- Fallback (worktree has no built binary — fresh `git worktree add`): the
  MAIN checkout's binary and subtool, cwd still the viewed worktree. The
  answer is then best-effort: the hover tooltip carries a one-line
  "answered by MAIN's compiler" marker, because for a worktree that changed
  the compiler or daslib the answer may be stale — say it, never imply
  tree-local truth.
- No binary anywhere → no hover, and the status line says why once per
  worktree, not per mouse move.
- The state rail reports, per answer: which binary path answered and
  whether it was the fallback.

## Search is table stakes (standing rule, 2026-07-29)

Declared during the first full in-app plan review: **any text view of any
kind — rich or plain, diff or not — gets IDE-quality search.** Copy the
VSCode find widget: input, regex / case / whole-word toggles, match count,
prev/next (F3 / Shift+F3), Ctrl+F opens, Esc closes, full dialog.

**Status (2026-07-30, herder bring-up shipped):** the shared
`imgui_text_find` component (proven in examples/text) replaced
`draw_inspector_search` and now serves ALL THREE inspector surfaces —
source View (with fold auto-expand on navigate), markdown View (code-block
auto-expand), and Diff mode (matches over the AFTER pane's aligned text,
jump rides the shared hunk-scroll). Ctrl+F opens, Esc closes, one query
survives a tab switch. Rails: `herder_inspector_find` / `_step` /
`_close`, plus find/fold/md-collapse fields on
`herder_file_inspector_state`. Diff-mode find searches the **whole
file** (Boris's live-review catch: searching only the compact aligned
text answered "not found" for text plainly in the file — the VSCode
diff-search trap), and it **stays in diff**: a jump to a match outside
the visible rows makes the diff DISPLAY that region — a context window
(±3 lines) injected as synthesized unchanged rows with true line
numbers on both sides (`git_patch_inject_context`), riding the normal
prepare pipeline so syntax colors and hunk navigation stay correct.
Windows accumulate per inspected file, so revealed regions stay
revealed. Diff find searches **both sides**: the whole new file plus
the BEFORE pane's removed rows (deleted text exists nowhere else;
context rows are skipped as duplicates), merged in visual diff order —
`text_find_collect` is the component's pure per-domain collector, the
merge and the `find_match_old_side` flags are the herder's. Old-side
jumps select in the BEFORE pane; removed rows are always displayed, so
they never need a window. Remaining gaps against the rule: **case /
whole-word toggles** (component has regex only — add in the component,
both apps inherit) and F3/Shift+F3 bindings.

**Editors get replace from day one** — search / replace / replace-all
within the file is part of the editor component's core surface, not a
follow-up. Follow-up (its own slice): search per TREE — project-wide
search across the selected worktree.

## Follow-up (recorded 2026-07-30, Boris): side-by-side RICH diff

Markdown files in Diff mode show source panes today; the follow-up is a
side-by-side **rendered** diff — two markdown views, change-aligned like
the source panes, gutter marks on changed blocks. Builds on the aligned
row model plus `markdown_view_with_change_gutter`; needs a block-level
alignment map (source rows → layout nodes both sides). Not scheduled —
after the editor arc unless review pain promotes it.

## Step 1 — hover in the View tab (the walking skeleton)

Smallest end-to-end slice, all plumbing proven before any diff complexity:

- Mouse dwells over the View text (>350 ms on one token) → resolve the
  byte offset under the cursor from the text_source_view geometry → convert
  to line0/char16 via the lsp_common conventions → spawn
  `nav.das -- hover <abs-file> <line0> <char16>` with cwd = the worktree.
- Result renders as a tooltip near the cursor: signature line, then doc
  comment when present. Compile errors in the file mean no hover — show
  nothing, never a stale answer.
- Async and generation-guarded exactly like the prepare worker: a hover
  that lands after the mouse moved or the file changed is dropped.
- Cache per (file, revision): hovering the same token twice must not spawn
  twice. A spawn-per-request hover costs a daslang compile of the module —
  measure it on `dasllama_common.das` FIRST and print the number in the
  status line like PREP; if a cold hover is >1 s on typical review files,
  add a per-file warm cache keyed on content hash before shipping.

## Step 2 — hover in diff mode, both sides

The "better than VSCode" step, and the reason `--overlay` matters:

- NEW side: overlay = the working file text the inspector already holds
  (`view_source`); positions come from the row's new_line number.
- OLD side: overlay = the old text the inspector already reconstructs
  (`inspector_reconstruct_old_source`); positions from the row's old_line.
  Hovering deleted code answers with the types it HAD — that is the review
  superpower, and it needs nothing new from the LSP side.
- One rule: the overlay file must carry the REAL file's path shape (same
  directory) or requires will not resolve; use the subtool's existing
  overlay contract (it maps the op's file argument), not a temp-dir copy.

## Step 3 — go to definition

- Ctrl+click (VSCode muscle memory) on either side or in View.
- Definition inside the same worktree → open it in the inspector (working
  comparison) at the target line, pushing a back entry; the inspector needs
  a tiny navigation stack (Back = the browser convention).
- Definition in daslib/ or another module → open read-only View at the
  line. It is outside the changelist, and that is fine — review reads
  library code constantly.
- References and call hierarchy exist in nav.das but are NOT in this arc's
  first three steps; they arrive free once the transport is proven.

## Step 4 — the editor is a PRODUCT, dasHerd is its first customer

> Detailed plan of record: **EDITOR_ARC_PLAN.md** (2026-07-29) — one
> component, two skins, E1–E7 order (basic editing before LSP), the
> .cpp-free proof, and the one named .cpp risk (incremental parse,
> avoided in v1).

(Reframed 2026-07-29, Boris's review.) An integrated code editor — with
LSP, for das — is a feature USERS have asked for: they want a code editor
inside their own tools. dasHerd's conflict resolution is the first use
case, not the definition, and heavy follow-up focus is expected. So it is
NOT hacked together inside dasHerd:

- The editing capability is built as a reusable dasImgui component, the
  editing sibling of `text_source_view` / the markdown view — same
  document model, same syntax pipeline, plus a cursor/selection/undo
  layer and an edit-ops interface a host binds (dasHerd binds LSP hover /
  definition / diagnostics; a game tool binds whatever it has).
- **Two /examples ship WITH the component and are part of the
  deliverable, not demos after the fact:**
  1. **Rich text editor** — the .md viewer we have gains editing (or a
     separate .md editor app if the viewer's layout model fights
     editability; that choice is the first design decision of the arc).
     Short text does not make it less full-featured: cursor, selection,
     undo, styling round-trip — the PR-body editor IS this example
     embedded in dasHerd.
  2. **Code editor app** — a das code editor with LSP hover, definition
     and (step 5) completion. The example is the proof the component
     works OUTSIDE dasHerd; it is what users were asking for.
- **dasHerd's two consumers** then bind the component: the PR body
  (markdown editor instance) and conflict resolution — three panes
  (ours / base / theirs) + an editable result buffer, per-conflict
  accept-ours / accept-theirs / edit-by-hand, save writes the file and
  stages it (the changelist already knows conflicted rows and refuses to
  discard them).

Bigger arc than the viewing steps, same corner of the detour: viewing
(steps 1–3) proves the LSP transport the editor will reuse.

## The quality bar (added 2026-07-29, Boris's review)

https://github.com/profelis/daScript-plugin — the existing daScript VSCode
LSP — is the reference implementation for HOVER: its hovers are rich and
very specific (resolved signatures, types, docs from the compiled program),
and our `nav.das -- hover` must reach at least that level before Step 1
ships. Port what it computes, not its architecture — it keeps a resident
server; we keep spawn-per-request.

**Completion is on the roadmap with the same bar.** Same nature of problem,
similar code to port from that plugin, and NOT a small issue — it is its own
step after Step 3, not a v1 stretch goal. The profelis plugin's completion
quality is the FLOOR; LLM-assisted completion comes later (it will), on top
of, not instead of, compiler-truth completion.

## Non-goals (v1 = steps 1–3)

- No resident LSP process, no lsp_supervisor.py dependency, no LSP framing.
- No .cpp semantics (clangd is a different animal; .cpp keeps tree-sitter
  colors only).
- No rename, no formatting-on-edit. Completion is NOT a non-goal — it is
  step 5, after the viewing experience proves out (see the quality bar).

## Tests

- Unit: cursor-byte → (line0, char16) mapping against multibyte fixtures
  (the `unicode-Привет` fixture worktree exists); overlay content selection
  per diff side; the navigation stack.
- Integration: spawn nav.das against a scratch .das fixture and assert a
  known hover string and a known definition target — proves the argv
  contract without any UI.
- Live (the parity rule): `herder_inspector_hover {side, line, character}`
  returning exactly what the tooltip would say, and
  `herder_inspector_goto {side, line, character}` performing the
  navigation. Screenshots show it renders; the rails show it is right.

## Order of work

1. Step 1 skeleton + the cold-hover measurement, commit. Hover CONTENT
   brought up to the profelis-plugin bar before calling the step done.
2. Step 2 both-sides overlay hover, commit.
3. Step 3 definition + navigation stack, commit.
4. References/call-hierarchy as a follow-up decision after Boris reviews
   the feel.
5. The editor arc (Step 4): the reusable dasImgui editing component with
   its TWO /examples (rich text editor, das code editor with LSP), then
   dasHerd binds it for the PR body and conflict resolution. Sized as its
   own arc — a requested product feature, not a dasHerd hack.
6. Completion, profelis floor, ported into the subtool shape — lands in
   both the code-editor example and dasHerd. LLM-assisted completion
   follows later, on top.
