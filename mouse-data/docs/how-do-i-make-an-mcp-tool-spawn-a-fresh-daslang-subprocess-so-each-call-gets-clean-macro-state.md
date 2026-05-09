---
slug: how-do-i-make-an-mcp-tool-spawn-a-fresh-daslang-subprocess-so-each-call-gets-clean-macro-state
title: How do I make an MCP tool spawn a fresh daslang subprocess so each call gets a clean macro state, and what is the cold-start cost?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

When an MCP tool runs `compile_file()` on user code that registers `[function_macro]` / `[call_macro]` annotations, the C++-side Annotation pointers persist for the rest of the daslang process's lifetime. In a long-lived MCP server, this means **subsequent edits to the macro source are invisible** until the server restarts. Symptoms: identical input that worked an hour ago now hits a stale macro version; restarting the MCP fixes it.

The fix is to make each MCP tool that compiles user code shell out to a fresh `daslang.exe` subprocess. Macro state is process-local, so every call starts clean.

## Pattern

`utils/mcp/tools/<tool>.das` becomes a thin wrapper:

```das
options gen2
require common public

def do_compile_check(file : string; project : string = ""; json : bool = false) : string {
    return run_mcp_subtool("compile_check", [file, project, json ? "true" : "false"])
}
```

The real logic lives at `utils/mcp/subtools/<tool>.das`:

```das
options gen2
require ../tools/common.das public
// ... real work ...

[export]
def main {
    let raw <- get_command_line_arguments()
    let args <- subtool_user_args(raw)
    if (length(args) < 3) {
        print(make_tool_result("compile_check subtool: expected 3 args, got {length(args)}", true))
        return
    }
    let file = string(args[0])
    let project = string(args[1])
    let json = string(args[2]) == "true"
    print(run_compile_check(file, project, json))
}
```

`run_mcp_subtool` (in `utils/mcp/tools/common.das`) handles the popen, the `--` separator, timeout, and exit-code translation:

```das
def run_mcp_subtool(subtool_name : string; args : array<string>; timeout_sec : float = 120.0) : string {
    let exe = get_daslang_exe()
    let subtool_path = path_join(get_das_root(), "utils/mcp/subtools/{subtool_name}.das")
    var argv <- [exe, subtool_path, "--"]      // `--` is critical
    argv |> reserve(length(argv) + length(args))
    for (a in args) { argv |> push(a) }
    var output : string
    let exit_code = run_and_capture(argv, output, timeout_sec)
    if (exit_code == popen_timed_out) {
        return make_tool_result("MCP subtool '{subtool_name}' timed out after {timeout_sec}s:\n{output}", true)
    }
    if (exit_code != 0) {
        return make_tool_result("MCP subtool '{subtool_name}' failed (exit {exit_code}):\n{output}", true)
    }
    return output
}
```

## The `--` separator gotcha

Without `--`, daslang treats positional argv past the script path as additional `.das` files to load AND auto-runs each. With `--`, daslang stops parsing its own options and exposes the rest via `get_command_line_arguments()`. The subtool then uses `subtool_user_args(raw)` (also in `tools/common.das`) to skip past the interpreter+script-path prefix and the `--` to get just the user args.

## Cold-start cost

Every subprocess pays the daslang.exe boot + subtool-script compile + require-chain compile cost. Measured on Windows (Release):

| Subtool | Fast-fail wall time |
|---|---|
| `daslang.exe --version` | ~35 ms |
| `compile_check.das` / `find_symbol.das` / `list_module_api.das` | ~0.7 s |
| `aot.das` | ~1.9 s (extra ~1.2 s for `daslib/aot_cpp`) |

So per-MCP-call overhead is 0.7–1.9 s before the actual work. For a 270-test MCP test suite, totalling ~89 s. Acceptable for offline runs. The dasImgui PR #2620 has the full benchmark + the deferred-speedup backlog.

## When to apply

- Any MCP tool that calls `compile_file()` on user code (i.e., the user's macros may register).
- Any tool that walks RTTI of compiled programs (the same `compile_file` underlies it).

When NOT to apply:
- Tools that only walk the C++ AST index (`cpp_*` family) — no daslang macro state involved.
- `live_*` tools (talk to live HTTP host).
- Tools that already shell out to external processes (run_script, run_test, format_file, ast-grep wrappers).

Verified 2026-05-09 in PR #2620.

## Questions
- How do I make an MCP tool spawn a fresh daslang subprocess so each call gets a clean macro state, and what is the cold-start cost?
