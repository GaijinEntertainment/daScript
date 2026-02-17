
.. _stdlib_profiler_boost:

==============================
Profiler cross-context helpers
==============================

.. das:module:: profiler_boost

The PROFILER_BOOST module extends profiling with high-level macros for
scoped timing (``profile_block``), function-level profiling annotations,
and formatted output of profiling results.

See also :doc:`profiler` for the core profiling infrastructure.

All functions and symbols are in "profiler_boost" module, use require to get access to it. ::

    require daslib/profiler_boost



++++++++++++++++++++++++
Context profiler control
++++++++++++++++++++++++

  *  :ref:`disable_profiler (var ctx: Context) <function-profiler_boost_disable_profiler_Context>`
  *  :ref:`enable_profiler (var ctx: Context) <function-profiler_boost_enable_profiler_Context>`

.. _function-profiler_boost_disable_profiler_Context:

.. das:function:: disable_profiler(ctx: Context)

Disables the profiler for the given context.


:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>`

.. _function-profiler_boost_enable_profiler_Context:

.. das:function:: enable_profiler(ctx: Context)

Enables the profiler for the given context.


:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>`


