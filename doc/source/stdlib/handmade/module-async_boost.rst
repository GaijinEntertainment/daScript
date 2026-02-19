The ASYNC_BOOST module implements an async/await pattern for daslang using
channels and coroutines. It provides ``async`` for launching concurrent tasks
and ``await`` for waiting on their results, built on top of the job queue
infrastructure.

All functions and symbols are in "async_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/async_boost
