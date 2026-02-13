
.. _stdlib_heartbeat:

====================================
Heartbeat callback injection
====================================

The HEARTBEAT module provides heartbeat callback injection.
It automatically inserts a ``heartbeat()`` call at the beginning of every
function body and at the start of every ``for`` / ``while`` loop body.
This allows long-running scripts to periodically yield control for
cooperative multitasking or watchdog checks.

All functions and symbols are in "heartbeat" module, use require to get access to it. ::

    require daslib/heartbeat

.. note::

    This module uses ``[infer_macro]`` to instrument code at compile time.
    The ``heartbeat()`` function must be defined in the calling context.
