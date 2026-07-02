# daslang LSP server

An LSP server for `.das` files. What it buys over the MCP tools: **push
diagnostics** — the compiler and lint report after *every* edit with no
explicit tool call — plus native go-to-definition / references / hover /
document & workspace symbols / call hierarchy / go-to-implementation. Zero
setup beyond a daslang binary and python; no sgconfig, no tree-sitter, no
MCP server.

## Requirements

- A daslang binary (`bin/daslang`, `bin/Release/daslang[.exe]`, `build/daslang`,
  or anywhere — see the lookup order below).
- Python 3 on `PATH`. The plugin manifest spawns `python3`; on Windows, python.org
  installs typically ship only `python.exe` — either create a `python3` alias or
  edit the manifest's `command` locally. (The protocol tests probe both spellings.)

## Registering with Claude Code

**In this repo: automatic.** A checked-in skills-directory plugin manifest
(`.claude/skills/daslang-lsp/.claude-plugin/plugin.json`) loads on workspace
trust whenever a session starts at the repo root — no CLI flag. (Skills-dir
plugins don't walk up: a session started in a subdirectory won't load it.
Plugin discovery happens only at CLI launch — `/clear`, `/resume`, and even
`/reload-plugins` don't load a new LSP server; exit and relaunch `claude`.)

For development, or from another checkout, the standalone plugin still works
and takes precedence over the checked-in one (same plugin name, local dir
wins):

```
claude --plugin-dir /abs/path/to/daScript/utils/lsp/plugin
```

Other repos adopt the same way: copy the manifest into their
`.claude/skills/daslang-lsp/.claude-plugin/plugin.json`, point `args` at this
repo's `lsp_supervisor.py`, and set `initializationOptions` (below) for module
resolution.

Diagnostics then attach automatically to the next tool result after any edit
of a `.das` file, and the LSP tool exposes definition / references / hover /
documentSymbol / workspaceSymbol / implementation / call hierarchy
(prepare + incoming + outgoing).

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
`$DASLANG_LSP_COMPILER` → repo and installed-SDK layouts
(`bin/Release/daslang[.exe]`, `bin/daslang[.exe]`, `build/daslang`,
`build/bin/daslang`) under the workspace and the supervisor's own tree →
`daslang` on `PATH`. All option paths (`compiler`, `project`, `project_root`,
`load_module`) are absolutized against the workspace root at initialize —
subtools spawn with a per-request cwd, so relative paths would otherwise
re-resolve per file.

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

## Navigation notes

- **Call hierarchy** covers direct calls (`foo()`, `obj.method()`). Virtual
  invocations through `obj->method()` and lambda / function-pointer invokes
  are not statically resolvable — they don't appear. Generic instances
  collapse onto their generic; class-method items display the bare method
  name (`speak`, not ``Animal`speak``).
- **Go to implementation**: on a class method — its overrides in derived
  classes; on a class or struct — the types deriving from it.
- Search scope for callers/overrides is the program of the file at the
  cursor (its `require` closure) — callers in unrelated files that require
  it from elsewhere are not in that program.

## Diagnostics

One lint-profile compile serves both: compile errors (severity Error) and, on
a clean compile, the repo's paranoid/perf/style lint passes (LINT/PERF →
Warning, STYLE → Information), honoring `.lint_config`, default-off rules and
`// nolint`. dastest expect-files get a single note instead of their
intentional errors.

## Tests

`tests/lsp/test_lsp_protocol.das` drives the supervisor over a stdio pipe:
initialize → didOpen with broken buffer text against a clean disk file
(proves the overlay) → didChange back to clean → definition → the
call-hierarchy loop (prepare → incomingCalls with the item's `data` echoed
verbatim → outgoingCalls from the returned caller item) → implementation
(base method → derived override) → shutdown/exit.

```
bin/daslang dastest/dastest.das -- --test tests/lsp
```

Skips with a log notice when python is not on `PATH`.
