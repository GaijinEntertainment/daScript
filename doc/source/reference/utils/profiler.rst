.. _utils_profiler:

.. index::
   single: Utils; profiler
   single: Utils; Profiling
   single: Utils; Memory leak tracking

================================
 Profiler --- Runtime Profiling
================================

The daslang profiler is an **instrumenting** profiler implemented as a debug
agent in :doc:`../../stdlib/generated/profiler`. It has two modes:

* **Performance profiling** --- per-function wall-clock timing with optional
  per-function heap accounting. Emits a Chrome-compatible trace JSON for
  visualization in ``chrome://tracing`` or Perfetto.

* **Memory-leak tracking** --- records every live heap allocation with its
  captured daslang call stack, and on context destroy dumps the leaked
  allocations sorted by size (largest first).

Both modes install a ``DapiDebugAgent`` that hooks into the daslang
runtime; there is no separate profiler binary. The agent is started
automatically when you run a script with one of the ``--das-profiler*``
command-line flags, or explicitly via ``require daslib/profiler`` in the
script itself.

.. contents::
   :local:
   :depth: 2


Quick start --- performance profiler
=====================================

Run any daslang script with ``--das-profiler`` and optionally a log-file path::

   daslang --das-profiler --das-profiler-log-file /tmp/trace.json path/to/script.das

Open the resulting ``/tmp/trace.json`` in ``chrome://tracing`` or
`Perfetto UI <https://ui.perfetto.dev/>`_ to explore the call tree. A tree
summary is also written to the log (``LOG_INFO``) on context destroy, for
example::

   main 1 71900ns
     builtin`push`4379756157886752001 ... 100 13000ns
     builtin`finalize`4179837999686245486 ... 1 700ns

Each row is ``<function-name> <call-count> <inclusive-time>``, indented to
reflect the call tree.


Quick start --- memory-leak tracker
====================================

Run with ``--das-profiler-leaks`` (mutually exclusive with the performance
profiler)::

   daslang --das-profiler --das-profiler-leaks path/to/script.das

On context destroy the tracker prints every live allocation with its captured
call stack, sorted by size::

   === Memory leaks in context '<unnamed>' (3 allocations, 0x2b0 bytes) ===
   [leak] size=0x200 bytes
     at builtin`push`4379756157886752001 daslib/builtin.das:117
     at make_big_leak examples/leak_smoke.das:14
     at main examples/leak_smoke.das:37
   [leak] size=0x80 bytes
     at builtin`reserve`12130697888660093679 daslib/builtin.das:84
     at make_widget_leak examples/leak_smoke.das:24
     at main examples/leak_smoke.das:37
   [leak] size=0x30 bytes
     at make_widget_leak examples/leak_smoke.das:24
     at main examples/leak_smoke.das:37

Stack frames are in leaf-first order (``panic()`` convention). The
``file:line`` portion is standalone so the VSCode terminal turns it into a
clickable link that jumps to the source line.

If the program has multiple live contexts at shutdown (e.g. an audio thread
spawned by strudel), each one produces its own ``=== Memory leaks in
context '<name>' ===`` block.


Enabling the profiler
=====================

There are two supported ways to install the profiler agent:

Auto-install via ``--das-profiler``
-----------------------------------

Passing any ``--das-profiler*`` CLI flag to ``daslang.exe`` implicitly
injects ``daslib/profiler.das`` into your program and runs the agent's
``[_macro] installing`` hook, which forks a debug-agent context and installs
the appropriate agent. You do not need to ``require`` it in the script.

Manual ``require``
------------------

Alternatively, add ``require daslib/profiler`` to your script. The
auto-installing macro runs at compile time for the ``profiler`` module and
installs the agent the same way. The CLI flags are still consulted at
runtime to choose the mode. Use ``require daslib/profiler_boost`` as well
if you want to call ``set_enable_profiler`` from script code
to gate collection around a region of interest (``disable_profiler()`` /
``enable_profiler()``).


Command-line flags
==================

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Flag
     - Meaning
   * - ``--das-profiler``
     - Required prefix flag that auto-requires :doc:`../../stdlib/generated/profiler`.
   * - ``--das-profiler-log-file <path>``
     - Write the Chrome trace JSON (performance mode) or the leak report
       (leaks mode) to ``<path>``. If omitted, the performance tree summary
       and leak report go to ``to_log(LOG_INFO, ...)``.
   * - ``--das-profiler-manual``
     - Performance mode: start with collection **disabled**, so nothing is
       recorded until you call ``enable_profiler`` from script
       code (via ``daslib/profiler_boost``). Useful for profiling one hot
       region of a longer run.
   * - ``--das-profiler-memory``
     - Performance mode: also record per-function heap and string-heap
       accounting. The tree-summary report then shows ``heap=N string_heap=M``
       instead of timings, plus two "Top 10 offenders" tables. Implies
       ``--das-profiler-global`` unless overridden.
   * - ``--das-profiler-time-unit <ns|us|ms|s>``
     - Performance mode: time unit for the tree summary. Default ``ns``. The
       Chrome trace JSON always uses microseconds (unchanged).
   * - ``--das-profiler-thread-local``
     - Performance mode: install one agent per thread (default when not
       tracking memory). Instrumentation events are dispatched only to the
       current thread's debug agent.
   * - ``--das-profiler-global``
     - Performance mode: install a single named agent observing all threads.
       Default when ``--das-profiler-memory`` is set.
   * - ``--das-profiler-leaks``
     - Install the memory-leak tracker instead of the performance profiler.
       Always a singleton named ``"memleaks"``; observes allocations from
       every live context.


Performance mode details
========================

The performance profiler wraps every daslang function body in an
instrumentation node (:cpp:class:`SimNodeDebug_InstrumentFunction` or its
thread-local variant). Each call fires ``onInstrumentFunction(entering, ...)``
on the agent, which records a timestamp (and optionally a heap snapshot)
into a per-context event buffer. On context destroy the events are folded
into a call tree and dumped.

Chrome trace JSON
-----------------

When ``--das-profiler-log-file`` is given, each event becomes a
Chrome-tracing *begin* (``"ph":"B"``) or *end* (``"ph":"E"``) entry in the
JSON array. Thread IDs are synthesized from context pointer addresses so
each daslang context shows as a separate track. Open the file in
``chrome://tracing`` (Chromium-based browsers) or Perfetto.

Gating collection around a region
---------------------------------

For long-running programs you usually want to profile only a specific
phase. Combine ``--das-profiler-manual`` with ``daslib/profiler_boost``:

.. code-block:: das

   require daslib/profiler_boost

   [export]
   def main() {
       warm_up()
       enable_profiler(this_context())
       hot_region()
       disable_profiler(this_context())
       cool_down()
   }

With ``--das-profiler-manual`` the profiler starts in the disabled state,
so only ``hot_region`` is recorded. Without ``--das-profiler-manual`` the
profiler starts enabled and the ``disable/enable`` pair toggles collection
off then back on (handy for *excluding* a region).

Per-function heap accounting
----------------------------

With ``--das-profiler-memory`` the tree summary shows each function's
inclusive and *own* (self - children) heap and string-heap allocation
totals. Two top-10 tables follow, ranking functions by own heap and own
string-heap byte totals. This mode implies ``--das-profiler-global``
because the accounting is aggregated across contexts.


Memory-leak mode details
========================

The leak tracker subscribes to four runtime callbacks:

* ``onAllocate(ctx, ptr, size, at)`` --- record a new allocation, keyed by
  ``intptr(ptr)``; store the current per-context shadow call stack.
* ``onReallocate(ctx, old, oldSize, new, newSize, at)`` --- erase ``old``,
  insert ``new`` with ``newSize`` (the realloc site becomes the new home
  of the block).
* ``onFree(ctx, ptr, at)`` --- erase the record.
* ``onInstrumentFunction(ctx, fn, entering, _)`` --- maintain a shadow
  ``array<SimFunction?>`` stack per instrumented context. No event buffer,
  no timing --- push on entry, pop on exit. Used at allocation time to
  snapshot the stack with a single array clone.

On context destroy the tracker emits the report shown in the quick start.
The report is routed to the log file if ``--das-profiler-log-file`` is set,
otherwise to ``to_log(LOG_INFO, ...)``.

Multi-context programs
----------------------

The leak agent is installed as a named singleton, so a single report is
produced covering every live context (main thread, spawned threads, any
sub-contexts such as the strudel audio mixer or job-queue workers). Each
context's allocations go into their own bookkeeping table keyed by the
context's address, and each ``onDestroyContext`` emits that context's
block of the final report.

What is **not** tracked
-----------------------

* Allocations made before the agent finishes installing. The
  ``[_macro] installing`` hook runs during compile time, which is early
  enough for main-thread user code and any threads the script spawns
  afterwards, but not for the compiler's own macro/folding contexts ---
  those are out of scope by design.
* String-heap allocations (``onAllocateString`` / ``onFreeString``). The
  leak agent only hooks the raw heap. String-heap leaks still show up in
  the C++-side heap tracker if enabled.
* Allocations in contexts where ``instrumentAllocations`` has been
  manually disabled via ``instrument_context_allocations``
  after the agent enabled it.


Writing your own profiler agent
================================

The leak and performance agents both inherit from
``ProfilerBaseAgent``, which in turn inherits from
``DapiDebugAgent`` in :doc:`../../stdlib/generated/debugapi`.
You can write your own agent by subclassing either.

Subclassing the profiler base
-----------------------------

Use this when you want to piggy-back on the CLI-option parsing and the
per-code-allocator instrumentation dedup logic:

.. code-block:: das

   require daslib/profiler

   class MyAgent : ProfilerBaseAgent {
       def override onInstall(agent : DebugAgent?) : void {
           use_thread_local = false      // or true, depending on your needs
       }
       def override onCreateContext(var ctx : Context) : void {
           if (!isProfileable(ctx)) {
               return
           }
           ensure_instrumented(ctx)      // installs onInstrumentFunction hooks
           instrument_context_allocations(ctx, true)  // enables alloc hooks
           // ... your per-context state setup
       }
       // ... your onAllocate / onInstrumentFunction / onDestroyContext
   }

Subclassing ``DapiDebugAgent`` directly
------------------------------------------------

Use this when you don't need any of the profiler's scaffolding and just
want raw access to the debug-agent hooks. See
:doc:`../../stdlib/generated/debugapi` for the complete list of
overridable methods. The ``examples/debugapi/`` directory in the source
tree contains worked examples, including ``allocation_tracking.das``
which shows the minimal allocation-hook setup.

The key API calls your agent will use:

* ``install_new_debug_agent(agent, "category")`` --- install as a named
  singleton. Allocation hooks and function-instrumentation hooks reach
  named agents only when the instrumentation uses the non-thread-local
  variant (``instrument_all_functions(ctx)`` and the fact that
  ``Context::onAllocate`` dispatches via ``for_each_debug_agent``).
* ``install_new_thread_local_debug_agent(agent)`` --- install in the
  current thread's thread-local slot (one agent per thread). Receives
  events from every thread-local instrumentation variant on the same
  thread.
* ``instrument_all_functions(ctx)`` vs
  ``instrument_all_functions_thread_local(ctx)`` --- pick the variant
  matching how your agent is installed, otherwise the
  ``onInstrumentFunction`` callbacks never reach you. The profiler's
  ``ProfilerBaseAgent::ensure_instrumented`` picks correctly from the
  ``use_thread_local`` field.
* ``instrument_context_allocations(ctx, true)`` --- opt-in to
  ``onAllocate`` / ``onReallocate`` / ``onFree`` callbacks for that
  context. Without this, no allocation hooks fire regardless of how the
  agent is installed.


Performance impact
==================

Every instrumented function call pays the cost of two dispatches through
the debug-agent adapter (entry + exit). Every heap allocation in an
instrumented context pays one more dispatch. For the performance mode this
is usually 5-15% overhead depending on call density; for leak mode it is
higher because the shadow stack is cloned into an
``AllocationRecord`` on each ``onAllocate``. Neither mode is suitable for
shipping builds --- they are debug/diagnostic tools.

AOT-compiled functions that bypass instrumentation entirely (e.g. native
C++ shims without daslang stubs) are invisible to the profiler. Pure
daslang functions always show up when their context is instrumented.


See also
========

* :doc:`../../stdlib/generated/profiler` --- generated API reference for
  the ``profiler`` module (classes, structs, helpers).
* :doc:`../../stdlib/generated/profiler_boost` --- cross-context enable
  / disable helpers used from user code.
* :doc:`../../stdlib/generated/debugapi` --- C++ ``DapiDebugAgent`` binding
  that the profiler subclasses.
* ``examples/debugapi/allocation_tracking.das`` --- minimal example of a
  custom allocation-tracking debug agent.
