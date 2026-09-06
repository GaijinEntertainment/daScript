.. _utils_watchdog:

.. index::
   single: Utils; watchdog
   single: Utils; Supervisor

======================================
 watchdog --- Program Supervisor
======================================

One supervisor for any daslang program that needs to stay up.  It
restarts the child with bounded backoff, captures crashes into bundles,
reports startup progress, and polls health.  In-tree it supervises
``utils/dasllama-server`` (JIT) and the telegram dictation example (a
baked exe).

It ships as a static executable, ``bin/watchdog`` (``bin/Release/watchdog.exe``
in an MSVC tree): a standalone context on the full runtime that compiles
nothing at run time, loads no shared module, and holds no lock on any file a
deploy replaces.  The same code runs under the interpreter for development.

Quick start
===========

In a deployed bundle, beside the program::

   ./watchdog

From the source tree, point it at the program's directory::

   bin/watchdog --cwd utils/dasllama-server

or, under the interpreter::

   daslang utils/watchdog/main.das -- --cwd utils/dasllama-server

What to supervise resolves in order, first match winning: command-line
flags (``--program``, ``--script``, ``--name``, …); ``watchdog.json``
beside the executable (each key is a flag name with underscores and sets
the default for that flag; unknown keys are a hard error); layout
discovery (``main.das`` beside ``bin/Release/daslang`` → ``daslang -jit
main.das``; exactly one ``*.exe`` → that program — anything ambiguous is
an error, never a guess).  Everything after ``--`` goes to the child.

.. seealso::

   :ref:`utils_daspkg` -- ``release_include_tool("watchdog")`` ships the executable inside a package release
