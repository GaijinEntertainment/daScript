# Language support in the inspector — plan of record

Status: PLANNED 2026-07-29, not started. Written to be implementable from
cold context: every file, tool and flag it names was verified against the
tree on the day it was written.

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

## Step 4 — editing, exactly two doors

- **PR body**: a plain multiline editor (the commit box pattern, bigger
  capacity) bound to the coming GitHub-PR surface. No LSP, no source view —
  it is markdown text. Ships with the PR arc, listed here because the
  editor widget is shared groundwork.
- **Conflict resolution**: its own slice AFTER viewing-with-LSP proves out.
  Three panes (ours / base / theirs) + an editable result buffer,
  per-conflict accept-ours / accept-theirs / edit-by-hand, save writes the
  file and stages it (the changelist already knows conflicted rows and
  refuses to discard them). The editable result buffer is the first real
  editor in the app; keep it to conflicts — the app is a reviewer, not an
  IDE.

## Non-goals (v1)

- No resident LSP process, no lsp_supervisor.py dependency, no LSP framing.
- No .cpp semantics (clangd is a different animal; .cpp keeps tree-sitter
  colors only).
- No completion, no rename, no formatting-on-edit. Review first.

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

1. Step 1 skeleton + the cold-hover measurement, commit.
2. Step 2 both-sides overlay hover, commit.
3. Step 3 definition + navigation stack, commit.
4. References/call-hierarchy as a follow-up decision after Boris reviews
   the feel; Step 4 rides the GitHub-PR arc.
