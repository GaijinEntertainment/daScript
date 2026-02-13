
.. _stdlib_profiler:

======================
Instrumenting profiler
======================

The PROFILER module provides CPU profiling infrastructure for measuring
function execution times. It includes instrumentation-based profiling with
hierarchical call tracking and timing statistics.

All functions and symbols are in "profiler" module, use require to get access to it. ::

    require daslib/profiler

++++++++++++++++
Profiler control
++++++++++++++++

  *  :ref:`set_enable_profiler (var ctxId: uint64; enabled: bool) <function-profiler_set_enable_profiler_uint64_bool>` 

.. _function-profiler_set_enable_profiler_uint64_bool:

.. das:function:: set_enable_profiler(ctxId: uint64; enabled: bool)

Enables or disables the profiler for the given context ID.

:Arguments: * **ctxId** : uint64

            * **enabled** : bool


