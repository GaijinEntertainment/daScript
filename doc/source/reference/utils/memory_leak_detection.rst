.. _utils_memory_leak_detection:

.. index::
   single: Utils; Memory leak detection
   single: Utils; Debugging
   single: Utils; Heap tracking

===================================================
 Memory Leak Detection --- Diagnostics Cheat Sheet
===================================================

daslang ships six distinct leak-detection mechanisms, each narrow in scope.
This page is the umbrella: which one to pick, how to invoke it, and how to
read the output. Each mechanism has its own in-depth page or skill file
linked at the end of the section.

.. contents::
   :local:
   :depth: 2


At a glance
===========

.. list-table::
   :header-rows: 1
   :widths: 3 30 25 30

   * - #
     - Mechanism
     - Scope
     - Invoke
   * - 1
     - :ref:`daslang leak profiler <utils_mlk_profiler>`
     - daslang heap, per-allocation with captured call stack
     - ``--das-profiler --das-profiler-leaks``
   * - 2
     - :ref:`C++ heap report <utils_mlk_heap_report>`
     - Context heap + string heap, all blocks alive at exit
     - ``-track-allocations -heap-report``
   * - 3
     - :ref:`gc_node leak detection <utils_mlk_gc_node>`
     - AST nodes outliving compile or execution
     - automatic (every run)
   * - 4
     - :ref:`Smart-pointer tracking <utils_mlk_smart_ptr>`
     - one specific ``ptr_ref_count`` id
     - ``--track-smart-ptr <hexId>``
   * - 5
     - :ref:`JobStatus / Channel / LockBox <utils_mlk_jobque>`
     - threading primitives with manual refcount
     - ``--track-job-status <id>``
   * - 6
     - :ref:`HandleRegistry <utils_mlk_handle_registry>` (dasHV)
     - value-sized handles (WebSocket client/server/channel)
     - *infrastructure exists, not auto-reporting today*


Picking the right tool
======================

If you see multiple reports at exit, fix in this order: #3 (gc_node) first
(any survivor indicates an ownership bug that can cascade), then #5
(threading primitives), then #1 or #2 (heap), then #4 (smart_ptr) and #6
(dasHV handles) if relevant.

- daslang run exits 0 but prints ``GC COMPILE LEAK`` / ``GC APP LEAK`` ---
  jump to :ref:`utils_mlk_gc_node`.
- You need the call stack for every leaked block in a daslang program ---
  use :ref:`utils_mlk_profiler`.
- You want a quick per-block survey (sizes, ids, daslang source locations) ---
  use :ref:`utils_mlk_heap_report`.
- You already know an object id from a prior dump and want a debug break on
  every refcount bump --- use :ref:`utils_mlk_smart_ptr`.
- You use ``daslib/jobque`` / ``daslib/jobque_boost`` / channels / streams
  and the exit banner lists ``JobStatus`` / ``Channel`` / ``LockBox``
  survivors --- jump to :ref:`utils_mlk_jobque`.
- Long-running dasHV server, suspected WebSocket-client handle leak ---
  see :ref:`utils_mlk_handle_registry`.


.. _utils_mlk_profiler:

1. daslang leak profiler (``--das-profiler-leaks``)
====================================================

Records every live daslang-side heap allocation with the full daslang call
stack, and dumps them on context destroy sorted by size.

.. code-block:: bash

   daslang --das-profiler --das-profiler-leaks path/to/script.das

Sample output::

   === Memory leaks in context '<unnamed>' (3 allocations, 0x2b0 bytes) ===
   [leak] size=0x200 bytes
     at builtin`push`4379756157886752001 daslib/builtin.das:117
     at make_big_leak examples/leak_smoke.das:14
     at main examples/leak_smoke.das:37

Sizes are in hex. Stack frames are in leaf-first order, matching ``panic()``.
The ``file:line`` portion is standalone so the VSCode terminal auto-links
to the source line. Multi-context programs produce one block per live
context at shutdown.

Full guide: :doc:`profiler`.


.. _utils_mlk_heap_report:

2. C++ heap report (``-track-allocations -heap-report``)
=========================================================

Enables per-block origin tracking at runtime and prints each Context's heap
and string-heap state at exit.

.. code-block:: bash

   daslang -track-allocations -heap-report path/to/script.das

.. note::
   These two flags use a single leading dash, not two --- the CLI is
   historically inconsistent about leading-dash count across flags.
   ``--track-smart-ptr``, ``--track-job-status``, and
   ``--das-profiler-leaks`` all use two.

Sample output with the default linear heap (one line per chunk)::

   --- heap report ---
   2b0202057	944 of 65536
   --- string heap report ---
   2b0191a05	52 of 65536

Columns: chunk address, bytes allocated, chunk size. Linear heap reset is
bulk --- you don't see individual blocks here.

With ``options persistent_heap = true`` (or
``-Dpersistent_heap=true`` as a policy), the persistent allocator reports
individual blocks (``big stuff``) and slab occupancy (``decks``)::

   --- heap report ---
   big stuff:
   	size	pointer		id
   	512	0x2069eeae630	1	array
   	48	0x2069ece4c20	2	new [[ ]]	D:/script.das:22:12
   	128	0x2069f1f4fa0	3	array
   bytes per location:
   48	D:/script.das:22:12

Rows: size in bytes, pointer, per-heap sequential ``id``, an optional
comment stamped by the runtime helper (``array``, ``new [[ ]]``, ``table``,
etc.), and the LineInfo of the call site if available. The
"bytes per location" section aggregates totals by ``file:line``.

**Build-time gate.** Tracking is compiled into every build by default via
``DAS_TRACK_ALLOCATIONS=1`` in ``include/daScript/misc/platform.h``. Shipping
builds can set ``-DDAS_TRACK_ALLOCATIONS=0`` to dead-code-eliminate the
infrastructure. The runtime flag (``-track-allocations``) is what turns
tracking *on* in a normal build --- without it the heap still reports
"decks" occupancy but "big stuff" blocks don't carry ``id``/``comment``/
``file:line``.

**Compare with #1.** #1 shows every live block with a full call stack;
#2 shows every live block with a single LineInfo but also shows slab
occupancy. Use #1 when you need "how did this get allocated", #2 when you
want "what's alive and roughly from where".


.. _utils_mlk_gc_node:

3. gc_node leak detection (automatic)
======================================

``gc_node`` is the ownership mechanism for AST types: ``TypeDecl``,
``Expression``, ``Function``, ``Structure``, ``Enumeration``, ``Variable``,
``MakeFieldDecl``, ``MakeStruct``, and their subclasses. Every allocation
links into a per-thread GC root list; leaks are any survivors at compile or
app exit.

No flag needed --- ``daslang.exe`` and ``daslang-live.exe`` automatically
check ``gc_root::gc_get_thread_root().gc_count`` at two points and dump any
survivors:

* **``GC COMPILE LEAK: N gc_node(s) after compile``** --- the compilation
  pass finished but some AST nodes from that pass are still referenced.
* **``GC APP LEAK: N gc_node(s) after execution``** --- ``main`` returned
  but runtime code allocated AST nodes (usually via ``clone_type`` / ``new
  TypeDecl`` / ``qmacro`` in daslang) that nothing cleaned up.

Sample output::

   GC APP LEAK: 3 gc_node(s) after execution
   gc_root 0x7ff...: count=3
     node 0x7ff...: id=1234 type=TypeDecl magic=0xDA5C0001
     node 0x7ff...: id=1235 type=Expression magic=0xDA5C0002

**Narrow it down with the env var.** Pick an id from the report and rerun
under a debugger with ``DAS_GC_BREAK_ON_ID`` set:

.. code-block:: bash

   DAS_GC_BREAK_ON_ID=1234 daslang path/to/script.das

The ``gc_node`` constructor calls ``os_debug_break()`` when it allocates a
node whose id matches --- you get the full C++ + daslang stack at the
creation site.

**Common fix.** daslang tools/utilities that build AST nodes at runtime need
``ast_gc_guard() { ... }`` around the scope. See the ``gc_migration`` skill
for the full ownership story.


.. _utils_mlk_smart_ptr:

4. Smart-pointer tracking (``--track-smart-ptr <hexId>``)
==========================================================

``ptr_ref_count`` is the base class for daslang's refcounted smart pointers
(``Context``, ``Program``, ``FileAccess``, compiler analyses, etc.). Each
instance has a unique ``ref_count_id`` and links into a global list
(``ref_count_head``).

At exit ``daslang.exe`` calls ``ptr_ref_count::DumpTrackPtr()`` which
lists every survivor::

   0x7ffee1301000 (rc=2, id=5a) Context main_ctx
   0x7ffee1301100 (rc=1, id=5b) Program
   total 2 tracked pointers

Pick an id from there and rerun with ``--track-smart-ptr <hexId>``:

.. code-block:: bash

   daslang --track-smart-ptr 0x5a path/to/script.das

``addRef``, ``delRef``, and the destructor on that specific id call
``os_debug_break()``. Attach a debugger (or ``-das-wait-debugger``) to
collect stack traces for each refcount bump. This makes it easy to find the
site that's holding on too long.

A second static (``ref_count_track_destructor``) breaks only on the
destructor call. It is not exposed as a CLI flag; set it from the debugger
if you need to distinguish "who destructed it" from "who bumped it".


.. _utils_mlk_jobque:

5. JobStatus / Channel / LockBox tracker
=========================================

Threading primitives (``JobStatus`` and subclasses ``Channel`` and
``LockBox``, plus the ``Feature`` value-type) have their own manual
refcount system. ``DumpJobQueLeaks()`` runs automatically at exit and
lists survivors with subtype and ``created at`` source location.

.. code-block:: bash

   daslang --track-job-status <id> path/to/script.das

The ``--track-job-status <id>`` flag traces every addRef / releaseRef on one
specific object with the source location of each call. The workflow is
essentially identical to #4 but specialized for these threading types ---
they have their own exit dump format and their own narrower trace output.

Full workflow (refcount accounting, shutdown order, lockbox fill/grab/join
lifecycle, capture-macro hidden refs): see
``skills/jobque_debugging.md`` in the source tree.


.. _utils_mlk_handle_registry:

6. HandleRegistry (dasHV handle objects)
=========================================

dasHV exposes C++ objects such as ``hv::WebSocketClient``,
``hv::WebSocketServer``, and ``hv::WebSocketChannel`` to daslang as
value-sized ``Handle<T>`` integers backed by
``HandleRegistry<T>::instance()`` --- a generation-tagged slot table owning
a ``std::shared_ptr<T>`` per handle. The registry is defined in
``include/daScript/misc/handle_registry.h``.

**Current status.** The registry publishes ``live_count()``,
``for_each_live(cb)``, and a module-wide
``handleRegistry_dumpAll()`` / ``handleRegistry_registerDump(fn)`` pair,
but **no call site invokes them at shutdown today**. The infrastructure is
in place; the auto-report is not.

**Manual query from C++.**

.. code-block:: cpp

   auto & reg = HandleRegistry<hv::WebSocketClient>::instance();
   if ( auto n = reg.live_count() ) {
       reg.for_each_live([](Handle<hv::WebSocketClient> h, auto & p){
           // log h.value, p.get()
       });
   }

**Wiring an automatic report.** Register per-type dump hooks in dasHV's
module init:

.. code-block:: cpp

   handleRegistry_registerDump([]{
       auto & reg = HandleRegistry<hv::WebSocketClient>::instance();
       if ( auto n = reg.live_count() ) { /* log */ }
   });

Then call ``handleRegistry_dumpAll()`` from ``shutdownDebugAgent()`` or the
program-teardown path. This is a small, tractable C++ change if the need
arises for a production service.


See also
========

* :doc:`profiler` --- the performance profiler + the daslang leak profiler
  in detail.
* :doc:`../../stdlib/generated/dashv` --- dasHV module reference.
* ``skills/memory_leak_detection.md`` --- the compact version of this
  guide intended as a Claude reference.
* ``skills/jobque_debugging.md`` --- full workflow for mechanism #5.
* ``skills/gc_migration.md`` --- background for mechanism #3 and the
  ``ast_gc_guard`` helper.
