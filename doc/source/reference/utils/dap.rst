.. _utils_dap:

.. index::
   single: Utils; DAP MCP Bridge
   single: Utils; Debug Adapter Protocol
   single: Utils; Debugging with MCP

================================
 DAP MCP Bridge --- AI Debugging
================================

``utils/dap/mcp_bridge.py`` exposes the daslang TCP
`Debug Adapter Protocol <https://microsoft.github.io/debug-adapter-protocol/>`_
server as a stateful Model Context Protocol server.  An AI coding agent can
launch or attach to a program, set breakpoints, inspect paused state, evaluate
expressions, step, and terminate the session through MCP tool calls.

The bridge requires Python 3.10 or newer and a daslang executable.  It contains no language
semantics: requests and responses are translated between MCP JSON-RPC and DAP,
while the native daslang debugger remains responsible for execution and state
inspection.

.. contents::
   :local:
   :depth: 2


Configuration
=============

Configure one bridge process per agent session.  Pin both the target workspace
and compiler so source paths and dynamic modules resolve in the intended tree:

.. code-block:: toml

   [mcp_servers.daslang-dap]
   command = "python3"
   args = [
       "/abs/path/to/sdk/utils/dap/mcp_bridge.py",
       "--repo-root",
       "/abs/path/to/project",
       "--executable",
       "/abs/path/to/sdk/bin/daslang",
   ]
   cwd = "/abs/path/to/project"
   enabled = true
   required = true

The configured executable is the default for ``debug_launch``; a launch call
can override it.  Paths passed to tools may be absolute or relative to
``--repo-root``.


Launch workflow
===============

The startup order is significant:

#. Call ``debug_launch`` with the ``.das`` entry point.  The bridge starts
   daslang with ``--das-wait-debugger``, chooses an available loopback port when
   no port is supplied, connects, initializes DAP, and sends ``launch``.
#. Install source breakpoints with ``debug_set_breakpoints``.
#. Call ``debug_threads``.  This satisfies the native debugger's startup gate.
#. Call ``debug_configuration_done`` to finish the DAP configuration phase.
#. Wait for a ``stopped`` event with ``debug_wait_event``.
#. Inspect the selected thread with ``debug_stack_trace``, ``debug_scopes``,
   ``debug_variables``, and ``debug_evaluate``.
#. Resume with ``debug_continue`` or one of the stepping tools.
#. Finish with ``debug_terminate`` or ``debug_disconnect``.

Instrumentation is the default launch mode.  Set
``stepping_debugger=true`` to opt into native statement stepping.  Source
breakpoints sent before ``configurationDone`` are retained and instrumented in
contexts that already exist as well as contexts created later.


Attach workflow
===============

For a runtime that already owns a DAP listener, call ``debug_connect``,
``debug_initialize``, and ``debug_attach``.  Complete the same
``debug_threads`` and ``debug_configuration_done`` startup sequence before
waiting for stops.


Tools
=====

Session lifecycle
-----------------

``debug_connect``
   Connect to an existing DAP TCP endpoint.

``debug_initialize``
   Initialize DAP and return the debugger capabilities.

``debug_launch``
   Start a daslang process owned by the bridge and initialize its DAP session.

``debug_attach``
   Attach to a runtime started outside the bridge.

``debug_configuration_done``
   Complete startup after threads and breakpoints have been configured.

``debug_terminate``
   Request debuggee termination through DAP.

``debug_disconnect``
   Close the session.  Cleanup is idempotent: a repeated call succeeds with
   ``already_disconnected=true``.

Breakpoints and execution
-------------------------

``debug_set_breakpoints``
   Replace all source breakpoints for one file.  An empty line list clears
   them.

``debug_data_breakpoint_info`` and ``debug_set_data_breakpoints``
   Resolve a visible variable to a hardware data-breakpoint identifier and
   replace the active data breakpoints.

``debug_continue``, ``debug_pause``, ``debug_step_in``, ``debug_step_over``, and ``debug_step_out``
   Control execution of the selected DAP thread.

Inspection and events
---------------------

``debug_threads`` and ``debug_stack_trace``
   Enumerate debuggee contexts and the call stack of a selected context.

``debug_scopes`` and ``debug_variables``
   Enumerate frame scopes and expand their values.

``debug_evaluate``
   Evaluate an expression in a paused stack frame.

``debug_wait_event``
   Wait for the next DAP event, optionally filtering by event name.


Lifecycle diagnostics
=====================

The bridge owns only processes started by ``debug_launch``.  A ``terminated``
event returned by ``debug_wait_event`` and an idempotent disconnect response
include a session snapshot with the endpoint, owned process identifier and
return code, close reason, last DAP termination body, and a bounded
stdout/stderr tail.  This preserves the cause when the DAP socket closes before
cleanup.


Application-specific scopes
===========================

Debug-agent modules can add application state to a paused frame from
``DapiDebugAgent.onCollect`` by calling ``report_context_state``.  Each
reported category appears as another ``debug_scopes`` result and expands via
``debug_variables`` without bridge-specific adapters.

Inspect every returned scope rather than assuming only ``Locals``,
``Arguments``, and ``Globals`` exist.  For example,
``opengl/opengl_state`` supplies OpenGL scopes and ``daslib/decs_state``
supplies DECS archetype and request scopes.  Their corresponding boost modules
require these state modules automatically.


Tests
=====

Run the end-to-end suite in both debugger modes::

   PYTHONDONTWRITEBYTECODE=1 python3 utils/dap/test_mcp_bridge.py
   DAS_TEST_STEPPING=1 PYTHONDONTWRITEBYTECODE=1 python3 utils/dap/test_mcp_bridge.py

The suite invokes all 21 MCP tools against real debuggee processes.  It covers
launch, attach, automatic port selection, breakpoint mutation while stopped,
stepping, termination, process failure diagnostics, and repeated cleanup.  The
runtime probes also cover cancellation before source-context readiness and a
repeated debugger-worker lifecycle in one process.  The Linux
``extended_checks`` job executes both commands.


.. seealso::

   ``utils/dap/README.md`` -- compact setup and workflow reference

   :ref:`utils_mcp` -- compiler, source-navigation, and live-runtime MCP tools

   :ref:`utils_lsp` -- push diagnostics and source navigation
