# daslang MCP Server

A minimal [Model Context Protocol](https://modelcontextprotocol.io/) (MCP) server that exposes daslang compiler diagnostics and program introspection to AI coding assistants like Claude Code.

## Tools

| Tool | Description |
|---|---|
| `compile_check` | Compile a `.das` file and return errors/warnings plus a categorized function listing on success |
| `list_functions` | Compile a `.das` file and list all user functions, class methods, and generic instances (after macro expansion) |

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
      "mcp__daslang__list_functions"
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
