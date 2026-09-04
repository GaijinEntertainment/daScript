# daScript DAP MCP bridge

`mcp_bridge.py` exposes the repository's TCP Debug Adapter Protocol server as a
stateful MCP server for Codex. One MCP process owns one DAP connection and, for
`debug_launch`, the launched `daslang` process.

The bridge requires Python 3.10 or newer.

For an external project, start the bridge with that project's workspace root
and pinned compiler. The configured executable becomes the default for every
`debug_launch`; an individual call can still override it:

```toml
[mcp_servers.daslang-dap]
command = "python3"
args = [
    "/abs/path/to/daScript/utils/dap/mcp_bridge.py",
    "--repo-root",
    "/abs/path/to/project",
    "--executable",
    "/abs/path/to/pinned-sdk/bin/daslang",
]
cwd = "/abs/path/to/project"
enabled = true
required = true
```

## Local launch workflow

1. Call `debug_launch` with a `.das` file. The bridge starts `daslang` with
   `--das-wait-debugger`, connects, initializes DAP, and sends `launch`. When
   `port` is omitted, the bridge chooses an available local port; pass an
   explicit port only when another process needs to know it in advance.
2. Call `debug_set_breakpoints` as needed.
3. Call `debug_threads`. The daScript startup gate requires this request.
4. Call `debug_configuration_done`.
5. Wait for `stopped` with `debug_wait_event`, then use `debug_stack_trace`,
   `debug_scopes`, `debug_variables`, and `debug_evaluate`.
6. Resume with `debug_continue`, `debug_step_in`, `debug_step_over`, or
   `debug_step_out`.
7. Finish with `debug_terminate` or `debug_disconnect`.

`debug_disconnect` is safe to repeat. If the DAP peer has already gone away,
it returns success with `already_disconnected=true` and a `session` snapshot
containing the last endpoint, owned-process return code, termination reason,
recent DAP events, and captured stdout/stderr tail. A `terminated` result from
`debug_wait_event` carries the same snapshot.

Local launch uses instrumentation mode by default. Source breakpoints are sent
to DAP immediately; the native debug agent keeps unverified breakpoints and
instruments contexts already present when `configurationDone` arrives, then
instruments later contexts when they are created, before their code runs.
Statement stepping remains available with `stepping_debugger=true`; it stays
opt-in so callers choose between statement stepping and instrumentation.

## Custom debugger state

daScript debug-agent modules can add application-specific state to a paused
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

## Test

```sh
PYTHONDONTWRITEBYTECODE=1 python3 utils/dap/test_mcp_bridge.py
```

To exercise the native stepping mode and its locked-table regression:

```sh
DAS_TEST_STEPPING=1 PYTHONDONTWRITEBYTECODE=1 python3 utils/dap/test_mcp_bridge.py
```

The end-to-end test invokes all 21 MCP tools against real daScript debuggee
processes, including automatic port allocation, launch, attach, stepping,
termination, repeated cleanup, disconnect while stopped at a breakpoint, and
debug-agent callbacks without a preconfigured context mutex. Runtime probes
also cover shutdown and source-context destruction while a debugger worker is
still waiting, repeated worker lifecycle in one process, and immediate
rejection of a duplicate singleton worker. It is also run by the Linux
`extended_checks` job.
