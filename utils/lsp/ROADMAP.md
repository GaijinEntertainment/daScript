# daslang LSP server — plan

An LSP server for `.das`, registered with Claude Code via its LSP-plugin mechanism.
What it buys over the MCP tools: **push diagnostics** — the compiler (and lint)
report after *every* edit with no explicit tool call — plus native go-to-definition /
references / hover / symbols, at zero setup cost for consumers (a daslang binary +
python; no sgconfig, no tree-sitter, no MCP server).

## Architecture (locked)

Two processes, hard split:

- **`utils/lsp/lsp_supervisor.py`** — the LSP endpoint Claude Code spawns. Owns ALL
  session state: Content-Length framing, `initialize` handshake, `{uri → latest text}`
  document shadow, debounce, and request dispatch. Zero language knowledge.
- **`utils/lsp/subtools/*.das`** — stateless batch tools. argv in → **LSP-shaped JSON**
  out (das owns the 1-based-byte → 0-based-UTF-16 position conversion; it has the file
  text) → exit. One fresh `daslang` process per request; nothing stays resident.

Why spawn-per-request instead of a resident das server (same rationale as the MCP
subtool pattern, `utils/mcp/tools/common.das`):

- **Macro-state leak**: in-process `compile_file` leaks `[function_macro]` /
  `[call_macro]` registrations into a long-lived process; the second compile of an
  edited file sees stale macros. Fresh process = fresh state, by construction.
- **Binary/DLL locks**: no resident daslang means `bin/daslang` and the
  `.shared_module` DLLs are never held between requests — builds never block, no
  kill-before-rebuild guard, no respawn/replay machinery (cf. `utils/mcp/mcp_supervisor.py`,
  which exists precisely because the MCP das child *is* resident).
- **Crash isolation**: a compiler crash on a broken buffer costs one request, not the session.
- **Cost**: every request pays a compile (~0.2–1 s) — the same profile as the MCP tools,
  which has been acceptable. Diagnostics are debounced; navigation is on-demand. If it
  ever hurts, a compiled-AST cache slots in behind the subtool boundary without changing
  the architecture.

The supervisor kills an in-flight validate when a newer edit for the same URI arrives
(python-trivial; the das side never needs cancellation).

## Registration

Claude Code has **no plugin-less LSP path** (no root `.lsp.json`, no settings key —
unlike `.mcp.json`). The vehicle is one checked-in manifest:

```
.claude/skills/daslang-lsp/.claude-plugin/plugin.json
```

```json
{
  "name": "daslang-lsp",
  "description": "daslang language server (compiler diagnostics + navigation)",
  "lspServers": {
    "daslang": {
      "command": "python3",
      "args": ["utils/lsp/lsp_supervisor.py"],
      "extensionToLanguage": { ".das": "daslang" }
    }
  }
}
```

Loads on workspace trust; `--plugin-dir` for development. The supervisor locates the
daslang binary like `mcp_supervisor.py::_default_launcher` (bin/Release → bin → build).

Claude Code consumes: publish-diagnostics (auto-injected after edits), definition,
references, hover, documentSymbol, workspaceSymbol, implementation, call hierarchy.
It does NOT consume completion — that whole surface is out of scope.

## Waves

### Wave 0 — consumer-end probe (position 0)

Throwaway supervisor: framing + `initialize` + a hardcoded diagnostic on every
didOpen/didChange. Wire via the manifest, drive a live Claude Code session, answer
empirically:

1. Handshake: which capabilities must we advertise for CC to engage at all?
2. Do diagnostics inject into Claude's context after an Edit? In what form?
3. What does CC send when a file changes on disk — didChange (full/incremental?),
   didSave, watched-files? (Decides doc-sync mode; CC edits disk, so the shadow map
   may be redundant for CC specifically — keep it anyway for other clients.)
4. Which navigation requests does CC actually issue, with what params?
5. `command` resolution: `python3` on PATH ok on all three OSes? Windows spelling?
6. What CC's `maxRestarts` does on supervisor death (cold reconnect vs replay).

Deliverable: probe notes in this file; wave 1 scope locked by answers, not docs.

#### Probe results (2026-07-01, CC v2.1.198, probe-plugin/ + headless `claude -p --plugin-dir`)

1. **Handshake**: `initialize` carries `rootUri`+`workspaceFolders` (project dir),
   `clientInfo{Claude Code, 2.1.198}`, `initializationOptions={}` (empty unless the
   manifest sets them). Advertising `textDocumentSync{change:1}` + provider flags is
   accepted; the LSP tool exposes ops: goToDefinition, findReferences, hover,
   documentSymbol, workspaceSymbol, goToImplementation, prepareCallHierarchy,
   incomingCalls, outgoingCalls. Marker payloads came back to the model verbatim.
2. **Diagnostics inject**: yes — after an Edit the model sees
   `hello.das: ✘ [Line 1:1] <message> (<source>)` (1-based display). First-edit
   injection confirmed end-to-end.
3. **Doc sync**: lazy — first Edit of a file sends `didOpen` with the **post-edit**
   text; each subsequent Edit sends `didChange` (FULL text, `version` increments —
   honors `change:1`) followed by `didSave`. No open at session start, no
   watched-files. So: validate on didOpen + debounced didChange; didSave can be a
   no-op (didChange already fired).
4. **Nav requests**: arrive as plain LSP `textDocument/hover`, `documentSymbol`, etc.
   when the model invokes the LSP tool.
5. **Paths/env**: `${CLAUDE_PLUGIN_ROOT}` expands in `args`; `python3` resolves on
   macOS (Windows spelling TBD on the first Windows pass); manifest `env` merges in;
   the server also gets `CLAUDE_PROJECT_DIR`, `CLAUDE_PLUGIN_ROOT`, `CLAUDE_PLUGIN_DATA`;
   cwd = project dir. Headless `-p` sessions load the plugin — no interactive session
   needed for development.
6. **maxRestarts semantics**: answered in the wave-1 kill probe — CC v2.1.198 does
   NOT restart a dead LSP server mid-session (killed the supervisor, then edited a
   compile error in: one `startup` in the log, no respawn, no diagnostics ever again,
   no signal to the model). Supervisor death is silent until the session restarts —
   so the supervisor must never die: `handle()` is exception-guarded, and subtool
   crashes are already isolated in the child process.

Wave-1 injection facts (beyond wave 0): diagnostics attach to whichever tool result
comes next — any tool (a lint publish landed on a `Bash` result). Injection is
delta-based: the block is titled "new diagnostic issues", each publish injects once,
and repeating an identical publish set does not re-inject on later tool calls.
Severity rendering: 1 → `✘`, 2 → `⚠`, 3 → `ℹ` (Information DOES render); the
`code` field renders as a `[CODE]` suffix, `source` as `(source)`.

### Wave 1 — diagnostics MVP

**Status: COMPLETE, proven live** — `lsp_supervisor.py` + `subtools/validate.das`
+ `plugin/.claude-plugin/plugin.json`. A headless CC session with the plugin gets real
compiler diagnostics after an Edit (verified verbatim: position, error code 30341, full
message). Injection semantics: diagnostics attach to the **next tool result** after the
edit — a publish that lands after the model's final message has nowhere to inject, so
latency matters. Debounce is 0.1 s (CC sends exactly one didChange per Edit call — no
keystroke bursts exist in this client).

- Supervisor: lifecycle, doc sync per wave-0 findings, 0.1 s debounce,
  kill-stale-validate, config via `initializationOptions` (project `.das_project`,
  `project_root`, `load_module` list, exe override) mapped onto subtool argv exactly
  like `build_subtool_argv`.
- `subtools/validate.das`: compile via `make_file_access` + `compile_file` (reuse
  `utils/mcp` compile machinery), emit LSP `Diagnostic[]`.
- Lint-as-diagnostics: **shipped**. One lint-profile compile (same policies as
  `utils/lint`: `lint_check`, `export_all`, `no_optimizations`,
  `no_infer_time_folding`) serves both errors and lint — compile errors surface at
  infer, before optimization, so error fidelity is unchanged; lint rules need the
  unoptimized AST anyway. On a clean compile the three passes run via the structured
  `*_collect_issues` overloads (added to `daslib/lint` / `perf_lint` / `style_lint`;
  shared `LintIssue` record in `daslib/lint_config`, tested in
  `tests/lint/test_collect_issues.das`) — no parsing positions back out of display
  strings, and `last_line`/`last_column` give real LSP ranges. Severity map:
  LINT/PERF → Warning, STYLE → Information (probe-verified `⚠`/`ℹ` render). Repo
  policy honored (`seed_default_disabled` + `.lint_config` + per-file
  `_comment_hygiene` option); `// nolint` works as everywhere else. dastest
  expect-files get one Information note instead of their intentional errors.
  Cost: ~0.3 s spawn+compile for a small file (the lint modules' own compile,
  up from ~70 ms core-only) — still well inside the next-tool-result window.

### Wave 2 — navigation

**Status: COMPLETE, proven live** — `definition`, `hover`, `documentSymbol`,
`references`, `workspaceSymbol`, all five driven end-to-end through a headless
CC session (exact positions verified; CC renders each op natively — definition
as `Defined in file:line:col`, documentSymbol as a kind-annotated tree,
references grouped by file).

Shape:

- `subtools/nav.das` — ONE stateless subtool, op as first argv token; ports the
  MCP subtool logic (`goto_definition` resolve, `find_references`
  identify-target + RefVisitor, `type_of`-style hover) onto one shared
  nav-profile compile (`export_all` + `no_optimizations` +
  `no_infer_time_folding` — faithful positions). documentSymbol is a
  compile-based walk of thisModule (functions + generics deduped by name:line,
  struct fields / class methods / enum members as children) — NOT the ast-grep
  outline; the LSP stays tree-sitter-free. workspaceSymbol searches all
  compiled program modules, source-located symbols only.
- `subtools/lsp_common.das` — shared module: LSP structs, byte↔UTF-16
  conversion both directions, argv split; validate.das refactored onto it.
- Supervisor: advertises the five providers, runs each request's subtool on a
  daemon worker thread (read loop keeps consuming didChange), maps das `path`
  fields to `file://` URIs via pathlib (das owns positions, python owns URIs).
  workspace/symbol is anchored to the most-recent didOpen/didChange document
  (tracked as `active_uri`) — no open doc → `[]`.

🔑 **Coordinate convention (probed + lexer-verified)**: daslang `LineInfo`
columns are **0-based byte offsets** (`das_yycolumn = 0` at line start,
`last_column = column + yyleng`, end-exclusive — ds2_lexer.lpp); lines are
1-based. Wave 1 assumed 1-based columns, so every published character was one
LEFT of the token; fixed in `lsp_common::make_position`/`utf16_units_in`, and
the fix flows to diagnostics too (live-verified: LINT003 ⚠ rendered at the
exact `var p` column). Note `Function.at` points at the NAME token (not `def`);
`Structure.at` sits one column left of the name (cosmetic only).

Wave-2 wire facts (CC v2.1.198): CC sends `initialized` (notification — ignore);
the LSP tool sends `didOpen` for files it navigates (geometry.das got one before
its documentSymbol), so nav also triggers validate publishes; an Edit's first
touch is still `didOpen` with post-edit text; the model passes 1-based positions
and CC converts to 0-based LSP before they reach the server.

### Wave 3 — hardening

**Status: COMPLETE.**

- `set_file_source` overlay: the supervisor writes the `{uri → text}` shadow
  to a temp file and passes `--overlay <path>` to BOTH subtools (validate and
  nav — the overlay is the buffer of the op's file argument); the das side
  calls `set_file_source(access, file, text)` and position-maps against the
  overlay text. Unsaved-buffer clients now compile the buffer, not the disk;
  a didOpen of a file that doesn't exist on disk works too. For CC (which
  saves before notifying) the overlay is identical to disk — the mechanism
  still runs every request, so it stays tested.
- Protocol tests: `tests/lsp/test_lsp_protocol.das` drives
  `python3 lsp_supervisor.py` over `popen_argv_pipe` — initialize handshake →
  didOpen with BROKEN buffer text while the committed fixture stays clean
  (publishDiagnostics must carry the buffer's 30341 at the exact range —
  overlay proven through the wire) → didChange back to clean (empty publish)
  → definition at the call site (exact def location) → shutdown/exit rc 0.
  Frame bodies read byte-exact via `fread(f, array<uint8>)`; headers via
  `fgets`. Probes `python3` then `python` (output must start with "Python" —
  dodges the Windows Store alias) and skips with a log notice when neither
  exists. AOT-registered in `tests/aot/CMakeLists.txt`.
  The test immediately caught a real bug: `find_compiler` accepted a relative
  binary path that broke under the subtools' per-request cwd — compiler
  discovery now absolutizes.
- Docs: `utils/lsp/README.md` (registration, config, Windows `python3`
  spelling note), `skills/daslang_lsp.md`, CLAUDE.md skill-table row. No
  bootstrap script needed — the committed manifest is portable as-is
  (`${CLAUDE_PLUGIN_ROOT}`-relative args; only the `python3` spelling can
  need a local edit on Windows).

### Wave 4 — dogfooding: session adoption + call hierarchy / implementation

**Status: COMPLETE, proven live** — checked-in plugin manifest (headless
session with NO `--plugin-dir` gets diagnostics), call hierarchy + implementation
driven end-to-end through CC's LSP tool (goToImplementation rendered the
derived override; incoming/outgoing calls consistent in both directions,
exact positions). Protocol test extended with the call-hierarchy loop
(prepare → incomingCalls with `data` echoed verbatim → outgoingCalls from
the returned caller item) and implementation.

Implementation findings: class-method Function names are class-prefixed
(`Animal`speak`) — items display bare names, `data.name`/`symbol` round-trips
the full one; generated members (apostrophe names, `Foo'__finalize`) sit ON
the `class` line and finalizer-chain calls do too, so both cursor paths skip
synthesized functions; `to_void` takes `auto(TT) const?` so both pointer
flavors share one instance (mangler ICE 50609); `->` virtual invokes and
lambda invokes are not statically resolvable — call hierarchy covers direct
calls only.

Original plan:

Use the LSP to build the LSP: adopt it in this repo's Claude Code
sessions first, then develop the new ops with live diagnostics on.

1. **Session adoption** — make repo sessions load the plugin without
   `--plugin-dir`. Wave-0 finding: CC has no plugin-less LSP path; the
   checked-in vehicle is a `.claude/`-side plugin manifest (probe
   suggested `.claude/skills/daslang-lsp/.claude-plugin/plugin.json`,
   loads on workspace trust). Verify empirically: commit the manifest,
   restart a session, confirm diagnostics fire on an Edit with no CLI
   flag. Watch for: python3-on-PATH, compiler discovery (already
   repo-layout-aware), duplicate registration when `--plugin-dir` is
   also passed.
2. **Call hierarchy** — `textDocument/prepareCallHierarchy` →
   `callHierarchy/incomingCalls` / `outgoingCalls` (CC consumes all
   three per the wave-0 probe). das side in nav.das: prepare =
   identify-function-at-cursor → CallHierarchyItem; incoming = walk all
   program functions for calls to the target (RefVisitor precedent);
   outgoing = walk the target body's ExprCall/ExprCallFunc/ExprInvoke.
   The item must round-trip client→server between prepare and
   incoming/outgoing — carry file/line/name in `item.data`.
3. **goToImplementation** — `textDocument/implementation`: for a class
   method, its overrides in derived classes; for an abstract method,
   the implementations; consider generic → instances. Scope the exact
   semantics in-session.
4. Extend `tests/lsp/test_lsp_protocol.das` + live-verify in a headless
   CC session, as before.

PR for the whole branch AFTER wave 4 (single preflight + CI round).

#### Session adoption — PROVEN (2026-07-02, CC v2.1.198)

- Vehicle: `.claude/skills/daslang-lsp/.claude-plugin/plugin.json`, committed —
  the documented "skills-directory plugin" path (loads as `daslang-lsp@skills-dir`
  on workspace trust). Headless probe from the repo root with NO `--plugin-dir`
  got the exact 30341 diagnostic end-to-end; supervisor log shows one startup
  spawned via the checked-in manifest.
- `args` hop back to the tree relatively:
  `${CLAUDE_PLUGIN_ROOT}/../../../utils/lsp/lsp_supervisor.py` — portable to
  worktrees/clones as-is.
- Loads only when the session STARTS at the repo root — skills-dir plugins do
  not walk up from subdirectories (documented limitation of this vehicle).
- Duplicate registration probed: `--plugin-dir utils/lsp/plugin` alongside the
  checked-in manifest yields ONE supervisor, spawned from the `--plugin-dir`
  copy (name-keyed dedup, local dir wins) — dev workflow unaffected.
- Worktree bootstrap: the manifest travels with git, so `utils/mcp/setup.das`
  needs no LSP wiring — its build already produces the binary `find_compiler`
  discovers (`build/daslang` et al.); setup's done-message now says so.

## Non-goals

- Completion (CC doesn't consume it), formatting-over-LSP (CC has the MCP/CLI
  formatter), replacing the profelis VSCode plugin, a resident das server.
