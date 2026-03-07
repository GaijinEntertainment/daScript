# daslang MCP Server

A minimal [Model Context Protocol](https://modelcontextprotocol.io/) (MCP) server that exposes daslang compiler diagnostics and program introspection to AI coding assistants like Claude Code.

## Tools

| Tool | Description |
|---|---|
| `compile_check` | Compile a `.das` file and return errors/warnings plus a categorized function listing on success |
| `list_functions` | Compile a `.das` file and list all user functions, class methods, and generic instances (after macro expansion) |
| `list_types` | Compile a `.das` file and list all structs, classes (with fields), enums (with values), and type aliases |
| `run_test` | Run dastest on a `.das` test file and return pass/fail results |
| `format_file` | Format a `.das` file using `daslib/das_source_formatter` |
| `run_script` | Run a `.das` file or inline code snippet and return stdout/stderr |
| `ast_dump` | Dump AST of an expression or compiled function. `mode=ast` returns S-expression (node types/fields), `mode=source` returns post-macro daslang code. Optional `lineinfo` to include file and line:col spans on each node |
| `program_log` | Produce full post-compilation program text (like `options log`). Shows all types, globals, and functions after macro expansion, template instantiation, and inference. Optional `function` filter |
| `list_modules` | List all available daslang modules (builtin C++ modules and daslib) |
| `find_symbol` | Cross-module symbol search (functions, generics, structs, handled types, enums, globals, typedefs/aliases, fields). Case-insensitive substring by default; `=query` for exact match |
| `list_requires` | Compile a `.das` file and list all `require` dependencies (direct and transitive), with source file paths and builtin annotations |
| `list_module_api` | List all functions, types, enums, and globals exported by a builtin or daslib module (e.g. `math`, `strings`, `fio`, `daslib/json`) |
| `convert_to_gen2` | Convert a `.das` file from gen1 (indentation-based) syntax to gen2 (braces/parentheses) using `das-fmt`. Optional `inplace` flag to modify the file directly |
| `goto_definition` | Given a cursor position (file, line, column), resolve the definition of the symbol under the cursor. Returns location, kind (variable/function/field/builtin/struct/enum/typedef), and source snippet. Optional `no_opt` to preserve pre-optimization AST |
| `type_of` | Given a cursor position (file, line, column), return the resolved type of the expression under the cursor. Shows all expressions at position from innermost to outermost. Optional `no_opt` |
| `find_references` | Find all references to the symbol under the cursor (function calls, variable uses, field accesses, type refs, enum/bitfield values, aliases). Works from both usage and declaration sites. Scope: `file` (default) or `all` (all loaded modules). Optional `no_opt` |

## Prerequisites

- daslang built with **dasHV enabled** (`DAS_HV_DISABLED=OFF` in CMake settings)
- The compiler binary (`daslang.exe` or `daslang`)

To enable dasHV, add `-DDAS_HV_DISABLED=OFF` to your CMake configure step:

```
cmake -B build -DDAS_HV_DISABLED=OFF ...
cmake --build build --config Release
```

## Starting the Server

```bash
bin/Release/daslang.exe utils/mcp/main.das
```

By default it listens on port **9500**. To use a different port:

```bash
bin/Release/daslang.exe utils/mcp/main.das -- 8080
```

You should see:

```
Starting daslang MCP server on port 9500...
Configure Claude Code with: "url": "http://localhost:9500/mcp"
Server running. Press Ctrl+C to stop.
```

## Architecture

- Each tool invocation runs in a **separate thread** (`new_thread`) with its own context/heap — when the thread ends, its memory is freed without GC
- The main loop periodically logs heap stats (every 60s) and auto-collects when the string heap exceeds 1 MB
- Tool handlers are modular: each tool lives in `tools/*.das`, shared utilities in `tools/common.das`

## Configuring Claude Code

Create a `.mcp.json` file in your **project root** (the directory where you run Claude Code):

```json
{
  "mcpServers": {
    "daslang": {
      "type": "http",
      "url": "http://localhost:9500/mcp"
    }
  }
}
```

Adjust the port if you changed it.

Optionally, allow the MCP tools without prompting by adding to `.claude/settings.json`:

```json
{
  "permissions": {
    "allow": [
      "mcp__daslang__compile_check",
      "mcp__daslang__list_functions",
      "mcp__daslang__list_types",
      "mcp__daslang__run_test",
      "mcp__daslang__format_file",
      "mcp__daslang__run_script",
      "mcp__daslang__ast_dump",
      "mcp__daslang__list_modules",
      "mcp__daslang__find_symbol",
      "mcp__daslang__list_requires",
      "mcp__daslang__list_module_api",
      "mcp__daslang__convert_to_gen2",
      "mcp__daslang__goto_definition",
      "mcp__daslang__type_of",
      "mcp__daslang__find_references",
      "mcp__daslang__program_log"
    ]
  }
}
```

After creating/editing these files, restart Claude Code (or start a new session) for it to pick up the MCP server.

## Verifying the Server

You can test the server directly with curl:

```bash
# Health check
curl http://localhost:9500/health

# MCP initialize handshake
curl -X POST http://localhost:9500/mcp \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}'

# List available tools
curl -X POST http://localhost:9500/mcp \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","id":2,"method":"tools/list","params":{}}'

# Compile-check a file
curl -X POST http://localhost:9500/mcp \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"compile_check","arguments":{"file":"tests/hello_world.das"}}}'
```

## How It Works

The server implements a subset of the MCP protocol over HTTP (Streamable HTTP transport):

- **POST `/mcp`** — JSON-RPC endpoint handling `initialize`, `tools/list`, `tools/call`, and `ping`
- **GET `/health`** — simple health check returning `ok`
- Supports both plain JSON and SSE (`text/event-stream`) responses based on the client's `Accept` header
- File paths passed to tools are resolved relative to the server's working directory
