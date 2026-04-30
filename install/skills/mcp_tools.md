# MCP Tools Reference

The daslang MCP server (`utils/mcp/main.das`) exposes compiler diagnostics, program introspection, and live-reload control to AI coding assistants via the [Model Context Protocol](https://modelcontextprotocol.io/). Stdio transport — no extra build dependencies.

**When MCP tools are available, prefer them** over manual compilation and grep-based exploration. **For searching `.das` files, prefer MCP tools over built-in Grep/Glob** — `grep_usage` is parse-aware (tree-sitter), `find_references` resolves cross-module symbols, `find_symbol` searches all loaded modules. **For interacting with `daslang-live`, always use MCP live tools** — not curl.

## Tool table

| Tool | Use instead of... |
|---|---|
| `compile_check` | Running `daslang` manually and parsing errors |
| `list_functions` | Grepping for `def ` in `.das` files |
| `list_types` | Grepping for `struct`/`class`/`enum` definitions |
| `find_symbol` | Searching across modules for function/type names |
| `list_module_api` | Reading daslib source to find available functions |
| `list_modules` | Guessing module names or scanning `daslib/` directory |
| `ast_dump` | Manually inspecting AST or post-macro output (supports `lineinfo` for source locations) |
| `program_log` | Running with `options log` to see full post-compilation program text |
| `run_script` | Running scripts via shell and capturing output |
| `run_test` | Running dastest via shell and parsing results |
| `format_file` | Running the formatter script manually; supports comma-separated list or glob, returns JSON array of results |
| `convert_to_gen2` | Running `das-fmt` manually to convert gen1→gen2 syntax |
| `goto_definition` | Manually tracing symbol definitions across files |
| `type_of` | Manually inspecting expression types |
| `list_requires` | Grepping for `require` statements and guessing transitive deps |
| `find_references` | Manually searching for all usages of a symbol across files |
| `eval_expression` | Evaluating expressions by writing throwaway scripts |
| `describe_type` | Reading source to understand type fields, methods, and values |
| `grep_usage` | Built-in Grep across `.das` files (parse-aware via ast-grep + tree-sitter) |
| `outline` | Manually scanning files for function/struct/enum declarations |
| `aot` | Manually running AOT generation and extracting function C++ |
| `lint` | Running lint/perf_lint/style_lint manually |
| `export_corpus` | `detect-dupe --export-functions` from a shell |
| `detect_duplicates` | `detect-dupe --against` to ask "did I just write this?" — wraps B2 mode end-to-end |
| `judge_duplicates` | Manually invoking `find-dupe` over a `detect-dupe` JSON report. Returns Claude-judged verdicts (real / partial / false_positive). Requires daspkg-installed `anthropic/anthropic` and `ANTHROPIC_API_KEY` |
| `find_dupe` | One-shot duplicate-finder + judge. Use when starting fresh on a directory or PR; `detect_duplicates` + `judge_duplicates` separately when you already have a curated corpus |
| `live_launch` | Manually starting `daslang-live` from shell |
| `live_status` | `curl http://localhost:9090/status` |
| `live_error` | `curl http://localhost:9090/error` |
| `live_reload` | `curl -X POST http://localhost:9090/reload` |
| `live_pause` | `curl -X POST http://localhost:9090/pause` or `/unpause` |
| `live_command` | `curl -X POST http://localhost:9090/command -d '{"name":"..."}` |
| `live_shutdown` | `curl -X POST http://localhost:9090/shutdown` |
| `shutdown` | Manually restarting the MCP server process |

## Notes

**Cursor-based tools** (`goto_definition`, `type_of`, `find_references`) accept a `no_opt` parameter that disables compiler optimizations to preserve the full AST — useful when globals, enum values, or bitfield constants get constant-folded away.

**Live tools.** `live_*` interact with a running `daslang-live` instance via its REST API. `live_launch` starts one if not already running (sets working directory to the script's folder). All live tools accept an optional `port` parameter (default 9090). When a compilation error is active, `live_command` and `live_pause` return HTTP 503 with the error — use `live_reload` to fix. Hitting any unknown endpoint returns JSON help with all endpoints + curl examples.

**Configuration.** Configure `.mcp.json` with `"command": "bin/daslang", "args": ["utils/mcp/main.das"]`. See `utils/mcp/README.md` for details and Claude Code permissions.
