# daslang LSP server (utils/lsp/)

Read this before working on `utils/lsp/` — the LSP server that gives Claude
Code (and any stdio LSP client) push diagnostics + navigation for `.das`.
Consumer docs: `utils/lsp/README.md`. Plan, wave history, and all probe
findings: `utils/lsp/ROADMAP.md`.

## Session adoption (this repo)

`.claude/skills/daslang-lsp/.claude-plugin/plugin.json` (checked in; the
`.gitignore` has a `.claude/skills/` carve-out) loads the plugin on workspace
trust with no CLI flag — skills-dir plugins load only when the session STARTS
at the repo root and don't walk up. `--plugin-dir utils/lsp/plugin` still
works for development and wins over the checked-in copy (name-keyed dedup).

## Architecture (LOCKED — do not revisit without a new decision)

- **`lsp_supervisor.py`** = the LSP endpoint. ALL session state lives here:
  Content-Length framing, initialize, `{uri → text}` document shadow, 0.1 s
  debounce, kill-stale-validate generations, worker threads for nav requests,
  das-path → `file://` URI mapping. Zero language knowledge.
- **`subtools/validate.das` / `subtools/nav.das`** (+ shared
  `subtools/lsp_common.das` module) = stateless spawn-per-request batch tools:
  argv in, LSP-shaped JSON on stdout, exit. das owns byte↔UTF-16 position
  conversion in both directions.
- **Call-hierarchy round-trip**: each item's `data` carries the anchor uri +
  decl file/line + FULL function name; incoming/outgoing re-identify the
  target after a fresh compile from those keys (the client echoes `data`
  verbatim). Class-method Function names are class-prefixed
  (`Animal`speak`) — items display the bare name, `data.name` keeps the full
  one. Generated members (`Foo'__finalize`, apostrophe names) sit ON the
  `class` source line — both cursor paths skip synthesized functions.
- **NO resident daslang, ever** (macro-state leak, binary/DLL locks vs builds,
  crash isolation). Same rationale as the MCP subtool pattern.
- **`--overlay <path>`**: the supervisor writes the document shadow to a temp
  file per request; subtools `set_file_source(access, file, text)` and
  position-map against that text, so unsaved buffers compile correctly.
  The overlay is the buffer of the op's *file argument* (nav: the navigated
  file; workspaceSymbol: the context file).

## Coordinate conventions (probed + lexer-verified)

- daslang `LineInfo`: **1-based lines, 0-based BYTE columns, end-exclusive**
  (`das_yycolumn = 0` at line start, `last_column = column + yyleng` —
  ds2_lexer.lpp). `Function.at` points at the NAME token; `Structure.at` is
  one column left of the name (cosmetic).
- LSP: 0-based lines, 0-based UTF-16 code units, end-exclusive. Conversions
  live in `lsp_common` (`make_position`, `utf16_units_in`,
  `utf16_to_byte_offset`).

## Claude Code wire facts (CC v2.1.198, probed live)

- Diagnostics attach to the **next tool result** after a publish — any tool.
  Injection is delta-based, once per publish set. Severity render: 1 `✘`,
  2 `⚠`, 3 `ℹ`; `code` renders `[CODE]`, `source` `(source)`.
- Doc sync is lazy: first Edit → `didOpen` with post-edit text; later Edits →
  full-text `didChange` + `didSave`. CC saves to disk before notifying.
- CC does **NOT restart a dead LSP server mid-session** — the supervisor must
  never die (`handle()` is exception-guarded; subtool crashes are isolated in
  the child).
- The model passes 1-based positions; CC converts to 0-based LSP.

## Dev rig (headless, no interactive session)

```
echo "<prompt>" | env -u ANTHROPIC_API_KEY DASLANG_LSP_LOG=<log> \
  claude -p --plugin-dir <abs>/utils/lsp/plugin --allowedTools "Edit,Read,LSP"
```
from a scratch project dir. The log records every message both ways.

## Tests

`tests/lsp/test_lsp_protocol.das` — full session over `popen_argv_pipe`
(initialize → didOpen broken-buffer/clean-disk → publishDiagnostics with the
exact 30341 range → didChange clean → empty publish → definition → shutdown).
Byte-exact frame bodies via `fread(f, array<uint8>)` (C fread loops until the
requested length); headers via `fgets`. Probes `python3` then `python`,
verifying output starts with "Python" (dodges the Windows Store alias), and
skips with a `to_log` notice when neither exists. AOT-registered in
`tests/aot/CMakeLists.txt` (glob excludes `/_` fixtures).

## Gotchas

- Compiler paths must be **absolute** — subtools spawn with per-request cwd
  (`find_compiler` absolutizes; keep it that way).
- The lint-profile compile (`lint_check`, `export_all`, `no_optimizations`,
  `no_infer_time_folding`) serves BOTH errors and lint — errors surface at
  infer, so fidelity is unchanged; lint needs the unoptimized AST.
- dastest expect-files: validate emits one Information note and suppresses
  diagnostics (intentional errors are not noise to report).
- The plugin manifest spawns `python3`; Windows may only have `python` —
  README documents the local edit. Don't hardcode a fallback chain in the
  manifest (no such mechanism).
