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
| `cpp_grep_usage` | Built-in Grep across `.cpp/.h/.hpp/.cc` files (parse-aware via ast-grep + tree-sitter-cpp) |
| `cpp_find_symbol` | Searching for C++ symbol DECLARATIONS by name + kind (function/class/struct/enum/union/typedef/namespace/macro) |
| `cpp_outline` | Manually scanning C++ files for top-level declarations |
| `cpp_goto_definition` | Approximate "where is this defined" for C++ — best-effort, no scope resolution; returns up to 5 ranked candidates |
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

**C++ source tools** (`cpp_grep_usage`, `cpp_find_symbol`, `cpp_outline`, `cpp_goto_definition`) all use ast-grep with tree-sitter-cpp. Search scope is configured in [`utils/mcp/cpp_search_config.das`](../utils/mcp/cpp_search_config.das) — `CPP_SEARCH_DIRS` (defaults `src/` `include/` `modules/`), `CPP_SEARCH_INCLUDE_GLOBS` locks file extensions to `*.cpp`/`*.h`/`*.hpp`, `CPP_SEARCH_INCLUDE_OVERRIDES` re-includes auto-excluded folders. Folders containing a `.git` file/directory (submodules, daspkg-installed packages, FetchContent destinations) are auto-excluded. `cpp_find_symbol` with `kind=function` covers both definitions and header-only declarations; `kind=typedef` covers both `typedef X Y;` and modern `using X = Y;`. Caveats: best-effort name extraction (complex templates and function-pointer typedefs may report partial names), specializations not separately listed, macro-expanded declarations like `DAS_BIND_FN(foo)` are invisible. `cpp_goto_definition` is approximate — no scope resolution, no overload disambiguation; for substring/usage lookups prefer `cpp_grep_usage`. A clangd-backed precise mode is on the v2 roadmap.

**`with_cpp_source` redirect.** `find_symbol` and `goto_definition` accept an optional `with_cpp_source` boolean. When `true`, results that have a C++ implementation (builtin functions, handled types via `addExtern`/`MAKE_TYPE_FACTORY`) get a resolved C++ source location appended via the lazily-built cpp index. First call costs ~2s (one full scan); subsequent calls cost ~150ms (a git-state staleness signature: `rev-parse HEAD` + filtered `git status` + per-file mtimes + `cpp_search_config.das` mtime). The index rebuilds automatically when relevant `.cpp/.h/.hpp` files change, when HEAD moves, or when the search config is edited. Default off — opt in when the question is "where is X *actually* implemented", not when just enumerating symbols.

**Live tools.** `live_*` interact with a running `daslang-live` instance via its REST API. `live_launch` starts one if not already running (sets working directory to the script's folder). All live tools accept an optional `port` parameter (default 9090). When a compilation error is active, `live_command` and `live_pause` return HTTP 503 with the error — use `live_reload` to fix. Hitting any unknown endpoint returns JSON help with all endpoints + curl examples.

**`shutdown` tool.** Shuts down the MCP server process. Claude Code auto-restarts it, picking up code changes to `.das` tool files. Tool registration changes (adding/removing tools) still require a manual MCP restart.

**Configuration.** Configure `.mcp.json` with `"command"` pointing at the daslang binary (`bin/Release/daslang.exe` on Windows MSVC, `build/daslang` on Linux/macOS, `bin/daslang` for the installed SDK), `"args": ["utils/mcp/main.das"]`. See `utils/mcp/README.md` for details and Claude Code permissions.

**`"defer_loading": false`.** The repo's `.mcp.json` sets this on the `daslang` server entry so tool schemas load at session start instead of being deferred (deferred = caller has to `ToolSearch select:<tool>` before each first call). When the harness honors the flag, this removes the per-call ToolSearch friction. When it doesn't (currently the upstream behavior — see [Issue #26844](https://github.com/anthropics/claude-code/issues/26844)), the flag is harmless and tools fall back to the deferred path; CLAUDE.md's "MCP-first search" rule documents that workflow.

**Tests:** run `dastest/dastest.das -- --test utils/mcp/test_tools.das` through the daslang binary.
