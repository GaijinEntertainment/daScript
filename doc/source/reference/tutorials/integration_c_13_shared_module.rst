.. _tutorial_integration_c_shared_module:

.. index::
   single: Tutorial; C Integration; Shared Module
   single: Tutorial; C Integration; Global Module Registry
   single: Tutorial; C Integration; module shared

=============================================
 C Integration: Shared Modules
=============================================

This tutorial demonstrates the ``module X shared`` mechanism and how it
solves a common integration problem: making a daslang module available
across independent compilations without requiring every ``das_file_access``
to carry the module's source file.


The problem
===========

When you introduce a module file via ``das_fileaccess_introduce_file``,
it is only visible to compilations that use **that** ``das_file_access``.
A second file access (or a script in a different directory) cannot find
the file and fails with ``missing prerequisite; file not found``:

.. code-block:: c

   // FileAccess #1 — both files present, works fine
   das_file_access * fa1 = das_fileaccess_make_default();
   das_fileaccess_introduce_file(fa1, "my_helpers.das", MODULE_SRC, 0);
   das_fileaccess_introduce_file(fa1, "user_script.das", SCRIPT_SRC, 0);
   das_program * p1 = das_program_compile("user_script.das", fa1, tout, libgrp);
   // --> 0 errors

   // FileAccess #2 — module file missing, fails
   das_file_access * fa2 = das_fileaccess_make_default();
   das_fileaccess_introduce_file(fa2, "user_script.das", SCRIPT_SRC, 0);
   das_program * p2 = das_program_compile("user_script.das", fa2, tout, libgrp);
   // --> error[30901]: missing prerequisite 'my_helpers'; file not found


The solution — ``module X shared``
===================================

Add the ``shared`` keyword to the module declaration:

.. code-block:: das

   options gen2
   module my_helpers shared

   [export]
   def double_it(x : int) : int {
       return x * 2
   }

When the module is compiled as a **dependency** (via ``require``), the
compiler promotes it to the **global module registry**.  Subsequent
compilations find it there -- no file needed.

The promotion is a compile-time mechanism.  A minimal "loader" script
triggers it:

.. code-block:: das

   options gen2
   require my_helpers


How it works internally
-----------------------

1. The parser sees ``module my_helpers shared`` and sets
   ``program.promoteToBuiltin = true``.

2. During dependency compilation, the compiler calls
   ``Module::promoteToBuiltin()`` which inserts the module into the
   global linked list (``daScriptEnvironment::modules``).

3. Ownership is transferred away from the program -- the promoted
   module survives even after the program is released.

4. ``Module::require()`` finds the promoted module on subsequent
   compilations without any file lookup.

5. ``das_shutdown()`` calls ``Module::ClearSharedModules()`` to free
   all promoted modules.


The C host code
===============

All daslang source is embedded as C string constants.  Four scripts are
used:

========================  ===================================================
Script                    Purpose
========================  ===================================================
``MODULE_NOT_SHARED``     Helper module **without** ``shared`` keyword
``MODULE_SHARED``         Same module **with** ``module my_helpers shared``
``LOADER_SCRIPT``         Minimal ``require my_helpers`` -- triggers promotion
``USER_SCRIPT``           Uses the module: calls ``double_it`` and ``greet``
========================  ===================================================


Part 1 — demonstrating the problem
-----------------------------------

.. code-block:: c

   // 1a: Same FileAccess — both files present, compiles fine
   das_fileaccess_introduce_file(fa, "my_helpers.das", MODULE_NOT_SHARED, 0);
   das_fileaccess_introduce_file(fa, "user_script.das", USER_SCRIPT, 0);
   das_program * program = das_program_compile("user_script.das", fa, tout, libgrp);
   // 0 errors

   // 1b: Fresh FileAccess — module file not introduced
   das_fileaccess_introduce_file(fa2, "user_script.das", USER_SCRIPT, 0);
   das_program * program2 = das_program_compile("user_script.das", fa2, tout, libgrp);
   // error[30901]: missing prerequisite 'my_helpers'; file not found


Part 2 — the shared module solution
------------------------------------

.. code-block:: c

   // 2a: Compile the loader — promotes my_helpers to the global registry
   das_fileaccess_introduce_file(fa, "my_helpers.das", MODULE_SHARED, 0);
   das_fileaccess_introduce_file(fa, "loader.das", LOADER_SCRIPT, 0);
   das_program * loader = das_program_compile("loader.das", fa, tout, libgrp);
   // Release everything — the promoted module survives
   das_program_release(loader);
   das_fileaccess_release(fa);

   // 2b: Fresh FileAccess — only the user script, no module file
   das_file_access * fa2 = das_fileaccess_make_default();
   das_fileaccess_introduce_file(fa2, "user_script.das", USER_SCRIPT, 0);
   das_program * program = das_program_compile("user_script.das", fa2, tout, libgrp);
   // 0 errors — module found in the global registry!

After successful compilation, the host simulates and calls ``test()``
to verify the shared module's functions work.


Build & run
===========

Build::

   cmake --build build --config Release --target integration_c_13

Run::

   bin/Release/integration_c_13

Expected output::

   === Part 1: The Problem -- non-shared module across FileAccess boundaries ===

   Part 1a: same FileAccess -- compilation succeeded (0 errors)

   Part 1b: different FileAccess, module file missing -- compilation failed (1 error):
     error 0: error[30901]: missing prerequisite 'my_helpers'; file not found ...

   === Part 2: The Solution -- module my_helpers shared ===

   Part 2a: loader compiled -- 'my_helpers' promoted to global module registry

   Part 2b: different FileAccess, no module file -- compilation succeeded!
   double_it(21) = 42
   Hello from my_helpers, tutorial 13!
     test() returned: 42


.. seealso::

   Full source:
   :download:`13_shared_module.c <../../../../tutorials/integration/c/13_shared_module.c>`

   Previous tutorial: :ref:`tutorial_integration_c_ecs`

   Tutorial :ref:`tutorial_integration_c_sandbox` — ``das_fileaccess_introduce_file`` and filesystem locking

   C API reference: :ref:`embedding_c_api`

   daScriptC.h API header: ``include/daScript/daScriptC.h``
