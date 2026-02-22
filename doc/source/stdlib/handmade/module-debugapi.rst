The DEBUGAPI module provides the debug agent infrastructure — creating, installing,
and communicating with persistent debug agents that live in their own forked contexts.
It supports cross-context function invocation, agent method calls, log interception,
data and stack walking, instrumentation, and breakpoint management.

All functions and symbols are in "debugapi" module, use require to get access to it.

.. code-block:: das

    require debugapi
