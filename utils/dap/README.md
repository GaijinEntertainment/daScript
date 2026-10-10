# daslang DAP MCP bridge

`utils/dap/main.das` exposes the repository's TCP Debug Adapter Protocol server as a
stateful MCP server. One bridge process owns one DAP connection and, for
`debug_launch`, the `daslang` process it started. It runs on the daslang binary, under
the watchdog's `-tool dap` front like the daslang MCP server - `daslang-watchdog -tool dap`
from an installed SDK, rooted at the client's directory; `utils/mcp/setup.das` writes the
`daslang-dap` entry into a tree's `.mcp.json`.

For an external project, start the bridge with that project's workspace root and
pinned compiler. The configured executable becomes the default for every
`debug_launch`; an individual call can still override it:

```toml
[mcp_servers.daslang-dap]
command = "/abs/path/to/daScript/bin/daslang"
args = [
    "/abs/path/to/daScript/utils/dap/main.das",
    "--",
    "--repo-root",
    "/abs/path/to/project",
    "--executable",
    "/abs/path/to/pinned-sdk/bin/daslang",
]
cwd = "/abs/path/to/project"
enabled = true
required = true
```

`--timeout <seconds>` sets the DAP request timeout (default 90).

Behind the watchdog front the `initialize` answer is the front's own - server name
`daslang`, no `instructions` - since the front answers before any child exists. The
bridge's workflow guidance is in the tool descriptions, which the front does forward;
the `instructions` text reaches only a client that runs `main.das` directly.

## Local launch workflow

1. Call `debug_launch` with a `.das` file. The bridge starts `daslang` with
   `--das-wait-debugger`, connects, initializes DAP, and sends `launch`. When
   `port` is omitted, the bridge chooses an available local port; pass an
   explicit port only when another process needs to know it in advance.
2. Call `debug_set_breakpoints` as needed.
3. Call `debug_threads`. The daslang startup gate requires this request.
4. Call `debug_configuration_done`.
5. Wait for `stopped` with `debug_wait_event`, then use `debug_stack_trace`,
   `debug_scopes`, `debug_variables`, and `debug_evaluate`. A `debug_evaluate`
   result carries the value and its type; an evaluation the server could not
   perform is a failed call whose text is the evaluator's diagnostic. The bridge
   keeps the first response to every request and fails the next call when a
   server answers one request twice.
6. Resume with `debug_continue`, `debug_step_in`, `debug_step_over`, or
   `debug_step_out`.
7. Finish with `debug_terminate` or `debug_disconnect`.

`debug_disconnect` is safe to repeat. If the DAP peer has already gone away,
it returns success with `already_disconnected=true` and a `session` snapshot
containing the last endpoint, owned-process return code, termination reason,
recent DAP events, and the captured output tail. A `terminated` result from
`debug_wait_event` carries the same snapshot.

The bridge has no thread: the socket and the debuggee's output are drained
between MCP requests and while a tool waits, so an event or an exit is never
missed, only read a little later.

Local launch uses instrumentation mode by default. Source breakpoints are sent
to DAP immediately; the native debug agent keeps unverified breakpoints and
instruments contexts already present when `configurationDone` arrives, then
instruments later contexts when they are created, before their code runs.
Statement stepping remains available with `stepping_debugger=true`; it stays
opt-in so callers choose between statement stepping and instrumentation.

The program compiles optimized by default, the same code a plain run executes,
so a call the optimizer evaluates at compile time never runs and a breakpoint
inside it never hits. `optimize=false` launches with `-no-optimization` (the
program-side spelling is `options optimize = false`): every source statement
and call survives and the debugger stops where the source says. The compiler's
own folding contexts are never reported as threads.

## Custom debugger state

daslang debug-agent modules can add application-specific state to a paused
stack frame. Their `DapiDebugAgent.onCollect` implementation calls
`report_context_state`; each reported category then appears as an extra result
from `debug_scopes`, and `debug_variables` expands the values normally. The
bridge does not need per-module adapters.

Always inspect every scope returned by `debug_scopes`, not only `Locals`,
`Arguments`, and `Globals`. For example:

- `require opengl/opengl_state` adds scopes such as `OPENGL`, `OPENGL program`,
  and `OPENGL texture` when the paused context is an OpenGL context.
- `require daslib/decs_state` adds `DECS archetype` and `DECS requests`.

`opengl/opengl_boost` and `daslib/decs_boost` already require their respective
state modules, so code using either boost module gets these scopes
automatically.

## Attach workflow

For a runtime already started with the debug server, use `debug_connect`,
`debug_initialize`, and `debug_attach`, followed by `debug_threads` and
`debug_configuration_done`.

## Layout

- `dap_bridge.das` - the library: the frame parser, `DapClient` (the connection, its
  responses and event queue), `DapBridge` (the tools and the owned debuggee).
- `main.das` - the entry: the tool schemas and the stdio loop over `utils/mcp/mcp_core.das`.
- `test_dap_bridge.das` - the tests; `_fixture*.das` are the debuggees they run.

## Test

```sh
bin/daslang dastest/dastest.das -- --test utils/dap/test_dap_bridge.das
```

The end-to-end cases drive all 21 MCP tools against real daslang debuggee
processes, once with the instrumentation debugger and once with native stepping,
including automatic port allocation, launch, attach, stepping, termination,
repeated cleanup, disconnect while stopped at a breakpoint, and debug-agent
callbacks without a preconfigured context mutex. The fixture cases cover shutdown
and source-context destruction while a debugger worker is still waiting, repeated
worker lifecycle in one process, and immediate rejection of a duplicate singleton
worker. The frame parser's limits and the resume-command mapping have their own
cases. The Linux `extended_checks` job runs the file.
