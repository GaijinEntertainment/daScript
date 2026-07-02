# Getting started with Daslang

Build the compiler, run a program, and wire in editor and AI-assistant
tooling. The full version of this guide lives in the
[online documentation](https://daslang.io/doc/) (Getting Started, on the
title page); this file is the short repo-side copy.

## Prerequisites

git, CMake, and a C++ compiler for your platform (MSVC, clang, or gcc).

## Build the compiler

```sh
git clone https://github.com/GaijinEntertainment/daScript
cd daScript
cmake -B build
cmake --build build --config Release -j
```

The compiler lands at `bin/Release/daslang.exe` (Windows / MSVC), or
`bin/daslang` / `build/daslang` (Make and Ninja layouts). Add it to your
`PATH` or alias it — the rest of this guide writes `daslang`.

## Compile a small program

```das
// File: hello_world.das
// Run:  daslang hello_world.das
options gen2

[export]
def main() {
    print("hello, world\n")
}
```

`[export]` makes `main` callable from outside the compiled context —
entry points need it. `options gen2` selects the current syntax
generation; all documentation and new code use it.

From here, the [tutorials](https://daslang.io/doc/) are the fastest tour
of the language (also in-repo under `tutorials/`).

## Set up VSCode

The best supported editor is VSCode with the
[daScript language support](https://marketplace.visualstudio.com/items?itemName=profelis.dascript-plugin)
extension — language server, linting, debugging, and snippets. If
autocompletion seems unresponsive, raise `dascript.server.connectTimeout`
to 10 (or 20 for a Debug-build compiler).

## AI assistants: MCP server

The daslang MCP server (`utils/mcp/`) gives AI coding assistants
compiler-backed tools — compile checks, navigation, parse-aware grep,
test running, live-reload control. Configure in `.mcp.json` at the
project root:

```json
{
  "mcpServers": {
    "daslang": {
      "command": "bin/Release/daslang.exe",
      "args": ["utils/mcp/main.das"],
      "defer_loading": false
    }
  }
}
```

(on Linux/macOS point `command` at your binary), or via CLI:
`claude mcp add daslang -- bin/Release/daslang.exe utils/mcp/main.das`.
Details: `utils/mcp/README.md`.

## AI assistants: LSP server

The daslang LSP server (`utils/lsp/`) adds push diagnostics — after
every edit the compiler and lint report into the assistant's context
automatically — plus definition/references/hover/symbols. It needs only
the daslang binary and Python 3. For Claude Code:

```sh
claude --plugin-dir /abs/path/to/daScript/utils/lsp/plugin
```

Any stdio LSP client can spawn `python3 utils/lsp/lsp_supervisor.py`
directly. Details: `utils/lsp/README.md`.
