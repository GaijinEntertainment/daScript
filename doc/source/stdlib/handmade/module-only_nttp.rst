The ONLY_NTTP module provides the ``[only_nttp]`` function annotation — a
guard for benchmark kernels. Compilation fails if the annotated function
calls a call-based extern that is not bound through ``addExternInline``
(the NTTP interpreter node flavor), naming the offending call. This keeps a
measured fastpath honest: a silently downgraded binding cannot skew the
benchmark.

All functions and symbols are in "only_nttp" module, use require to get access to it.

.. code-block:: das

    require daslib/only_nttp
