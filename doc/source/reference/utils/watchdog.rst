.. _utils_watchdog:

.. index::
   single: Utils; watchdog
   single: Utils; Supervisor

======================================
 watchdog --- Program Supervisor
======================================

One Python supervisor for any daslang program that needs to stay up.  It
restarts the child with bounded backoff, captures crashes into bundles,
reports startup progress, and optionally exposes a per-program control
page.  In-tree it supervises ``utils/dasllama-server`` (JIT) and the
telegram dictation example (a baked exe).

Quick start
===========

In a deployed bundle, beside the program::

   python watchdog.py

From the source tree, point it at the program's directory::

   python utils/watchdog/watchdog.py --cwd utils/dasllama-server

What to supervise resolves in order, first match winning: command-line
flags (``--program``, ``--script``, ``--name``, …); ``watchdog.json``
beside ``watchdog.py`` (each key sets the default for the same-named
flag; unknown keys are a hard error); layout discovery (``main.das``
beside ``bin/Release/daslang`` → ``daslang -jit main.das``; exactly one
``*.exe`` → that program — anything ambiguous is an error, never a
guess).

.. seealso::

   :ref:`utils_daspkg` -- ``release_include_from`` ships the watchdog inside a package release
