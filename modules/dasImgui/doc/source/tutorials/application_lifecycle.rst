.. _tutorial_application_lifecycle:

#####################
Application lifecycle
#####################

A long-running dasImgui program needs two compatible owners:

* ``daslang-live`` calls the exported ``init`` / ``update`` / ``shutdown``
  functions and collects between update calls.
* Standalone ``daslang`` calls ``main()``, which owns the same lifecycle and
  must also provide a safe garbage-collection boundary.

The canonical application shape below supports windowed, live-reload, and
headless execution from one source file. It follows the same memory rule as
``utils/dasllama-server/main.das``: application state that survives a frame is
global, and collection happens only after one update has finished and before
the next frame creates collectable locals.

Source: ``examples/tutorial/application_lifecycle.das``.

.. literalinclude:: ../../../examples/tutorial/application_lifecycle.das
   :language: das
   :linenos:

Lifecycle ownership
===================

``init()``
   Creates long-lived resources. It is called once by standalone ``main`` and
   by the live host on load/reload. Harness setup is reload-safe.

``update()``
   Processes exactly one frame. A ``false`` result from
   ``harness_begin_frame()`` skips rendering while the host is paused,
   reloading, closing, or a headless frame cap has been reached.

``shutdown()``
   Releases resources in reverse ownership order. The harness distinguishes a
   live reload from final process exit and preserves the live ImGui context when
   appropriate.

``main()``
   Is used only by standalone ``daslang``. It calls ``init`` once, repeatedly
   calls ``update``, and always finishes with ``shutdown``.

The two heap options
====================

Long-running applications that allocate transient strings or containers use:

.. code-block:: das

   options persistent_heap
   options gc

``persistent_heap`` makes the context heap collectable. ``gc`` enables
collection in the context. Both are needed before calling ``heap_collect`` and
both are entry-program policies, so the tutorial declares them explicitly.

They do **not** make ordinary JIT stack locals traceable. A collection while a
live string, array, table, JSON value, or other collectable object exists only
in a JIT local can reclaim that object and crash. This is why the lifecycle
shape, rather than merely the two options, is the safety mechanism.

The safe collection point
=========================

GC is the safety net, not the per-frame ownership mechanism. Long-running
daslang applications should release uniquely-owned transient arrays, strings,
tables, JSON trees, and replaced caches with ``delete`` as soon as their
ownership ends. Stable derived data should be cached and rebuilt only when its
inputs change. With that discipline, the heap reuses freed storage and a real
collection is needed only occasionally.

Standalone programs do not receive the live host's between-update collection
pass. Their ``main`` loop must call ``harness_maybe_collect_gc()`` immediately
after ``update()`` returns. The helper delegates to the existing ``glfw_live``
fragmentation heuristic: calling the check every loop does **not** mean
collecting every loop. It only collects when the heap has enough unused space
to make compaction worthwhile. It is a no-op under ``daslang-live``, because
the host already owns collection there.

This placement matters. Do not call ``heap_collect`` from the middle of widget
rendering or while a request/frame-local collectable value must remain alive.
Keep durable state in module globals, keep standalone ``main`` free of
collectable locals that span the collection call, and collect only after
``update`` returns completely.

For a non-harness service, use the explicit server form:

.. code-block:: das

   [export]
   def main() {
       init()
       while (!exit_requested()) {
           update()
           maybe_collect_gc() // update returned; request locals are dead
       }
       shutdown()
   }

For non-harness services the equivalent call is their own
``maybe_collect_gc``. ``utils/dasllama-server/main.das`` is the production reference. Its
``maybe_collect_gc`` skips host-owned live mode, rate-limits collections, uses
heap-fragmentation ratios, and can honor a forced diagnostic collection.
Simple GLFW applications can use ``live/glfw_live``'s existing
``maybe_collect_gc`` helper at the same post-update boundary.

Delete-first example
====================

.. code-block:: das

   def update() {
       var response <- build_transient_response()
       send(response)
       delete response // deterministic ownership end; no GC required here
   }

For a cached UI layout, keep the cache global. When document text, available
width, font roles, style, or zoom changes, ``delete`` the old cached layout and
replace it once. An idle frame should render the retained layout without
creating a new graph for GC to discover later.

Run modes
=========

.. code-block:: bash

   # Standalone window.
   daslang.exe modules/dasImgui/examples/tutorial/application_lifecycle.das

   # Live-reload host owns init/update/shutdown.
   daslang-live modules/dasImgui/examples/tutorial/application_lifecycle.das

   # CPU-only smoke run with a deterministic exit.
   daslang.exe modules/dasImgui/examples/tutorial/application_lifecycle.das -- --headless --headless-frames=600

.. seealso::

   * :ref:`tutorial_live_reload` for reload state and lifecycle hooks.
   * :ref:`tutorial_harness_headless_mode` for the harness backend split.
   * ``skills/application.md`` for the implementation checklist used when
     creating or reviewing an application.
