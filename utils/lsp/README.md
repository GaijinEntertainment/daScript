# daslang LSP server

An LSP server for `.das` files. What it buys over the MCP tools: **push
diagnostics** — the compiler and lint report after *every* edit with no
explicit tool call — plus native go-to-definition / references / hover /
document & workspace symbols. Zero setup beyond a daslang binary and python;
no sgconfig, no tree-sitter, no MCP server.

## Requirements

- A daslang binary (`bin/daslang`, `bin/Release/daslang[.exe]`, `build/daslang`,
  or anywhere — see the lookup order below).
- Python 3 on `PATH`. The plugin manifest spawns `python3`; on Windows, python.org
  installs typically ship only `python.exe` — either create a `python3` alias or
  edit the manifest's `command` locally. (The protocol tests probe both spellings.)

## Registering with Claude Code

Claude Code loads LSP servers from plugins. The manifest lives at
`utils/lsp/plugin/.claude-plugin/plugin.json`; wire it in with:

```
claude --plugin-dir /abs/path/to/daScript/utils/lsp/plugin
```

Diagnostics then attach automatically to the next tool result after any edit
of a `.das` file, and the LSP tool exposes definition / references / hover /
documentSymbol / workspaceSymbol.

Any other stdio LSP client works too — point it at
`python3 utils/lsp/lsp_supervisor.py`.

## Configuration

`initializationOptions` (all optional):

| Key | Meaning |
|---|---|
| `compiler` | Path to the daslang binary |
| `project` | `.das_project` file passed to subtools |
| `project_root` | Passed as `-project_root` (dynamic native modules) |
| `load_module` | List; each entry passed as `-load_module` |

Compiler lookup order: `initializationOptions.compiler` →
`$DASLANG_LSP_COMPILER` → repo layouts (`bin/Release/daslang[.exe]`,
`bin/daslang`, `build/daslang`, `build/bin/daslang`) under the workspace and
the plugin's own repo → `daslang` on `PATH`. Relative paths are absolutized
at discovery (subtools spawn with per-request cwd).

Debug log: set `DASLANG_LSP_LOG=<path>` (default: `daslang_lsp.log` in the
system temp dir).

## Architecture (short version)

Two processes, hard split — full rationale and wave history in
[ROADMAP.md](ROADMAP.md):

- **`lsp_supervisor.py`** — the endpoint the client spawns. Owns all session
  state: framing, handshake, `{uri → text}` document shadow, debounce,
  dispatch. Zero language knowledge.
- **`subtools/*.das`** — stateless batch tools (`validate.das`, `nav.das`).
  One fresh `daslang` process per request; argv in, LSP-shaped JSON out, exit.
  The document shadow rides along as a `--overlay` temp file, so compiles see
  the client's buffer even when unsaved.

No resident daslang, by design: no macro-state leaks across compiles, no
binary/DLL locks while builds run, per-request crash isolation.

## Diagnostics

One lint-profile compile serves both: compile errors (severity Error) and, on
a clean compile, the repo's paranoid/perf/style lint passes (LINT/PERF →
Warning, STYLE → Information), honoring `.lint_config`, default-off rules and
`// nolint`. dastest expect-files get a single note instead of their
intentional errors.

## Tests

`tests/lsp/test_lsp_protocol.das` drives the supervisor over a stdio pipe:
initialize → didOpen with broken buffer text against a clean disk file
(proves the overlay) → didChange back to clean → definition → shutdown/exit.

```
bin/daslang dastest/dastest.das -- --test tests/lsp
```

Skips with a log notice when python is not on `PATH`.
