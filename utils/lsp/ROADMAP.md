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

`definition`, `hover`, `documentSymbol`, `references`, `workspaceSymbol` — remaps of
the existing MCP subtool logic (`goto_definition`, `describe_type`, `outline`,
`find_references`, `find_symbol`) to LSP request/response shapes.

### Wave 3 — hardening

- `set_file_source` overlay for unsaved-buffer clients (CC edits disk; this is
  generality for other LSP clients, not a CC need).
- Protocol tests in `tests/lsp/` driving the supervisor over a pipe
  (precedent: `tests/mcp/test_popen_argv_pipe.das`): initialize → didOpen broken
  file → expect publishDiagnostics.
- `skills/` entry + README; `setup.das`-style bootstrap if wave 0 shows the manifest
  can't be committed portably as-is.

## Non-goals

- Completion (CC doesn't consume it), formatting-over-LSP (CC has the MCP/CLI
  formatter), replacing the profelis VSCode plugin, a resident das server.
