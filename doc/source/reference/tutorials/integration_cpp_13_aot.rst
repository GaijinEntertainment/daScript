.. _tutorial_integration_cpp_aot:

.. index::
   single: Tutorial; C++ Integration; AOT

====================================
 C++ Integration: AOT Compilation
====================================

This tutorial explains daScript's ahead-of-time (AOT) compilation
system, which transpiles daScript functions into C++ source code
for near-native performance.  Topics covered:

* The AOT workflow — generate, compile, link
* ``CodeOfPolicies::aot`` — enabling AOT at runtime
* ``SimFunction::aot`` — checking if a function is AOT-linked
* Self-registration via ``AotListBase``


Prerequisites
=============

* Tutorial 12 completed (:ref:`tutorial_integration_cpp_smart_pointers`).
* Understanding of the compile → simulate → eval workflow.


What AOT does
=============

AOT in daScript does **not** JIT-compile to machine code at runtime.
Instead, it follows a **two-stage build** pattern:

1. **Generate** — the ``daslang`` tool transpiles daScript functions
   into C++ source code
2. **Compile** — the generated ``.cpp`` file is compiled by your C++
   compiler and linked into the host executable
3. **Link** — at runtime, ``simulate()`` replaces interpreted
   simulation nodes with direct calls to the pre-compiled C++
   functions

This gives near-C++ performance while keeping the convenience of
scripting during development.


The AOT workflow
================

Stage 1 — Generate C++ from daScript
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The project's ``CMakeLists.txt`` uses the ``DAS_AOT`` macro,
which runs ``daslang.exe`` with the AOT tool script at build time:

.. code-block:: bash

   daslang.exe utils/aot/main.das -- -aot script.das output.cpp

This produces a ``.cpp`` file containing C++ implementations of all
daScript functions, plus a self-registration block:

.. code-block:: cpp

   // Generated AOT code (simplified)
   namespace das {
       namespace _anon_XXX {
           inline bool test(Context * __context__) {
               // ... translated daScript code ...
           }

           static void registerAotFunctions(AotLibrary & aotLib) {
               aotLib[0x92da443ef141abbb] = +[](Context & ctx) -> SimNode* {
                   return ctx.code->makeNode<SimNode_Aot<...>>();
               };
           }
       }
       AotListBase impl(registerAotFunctions);  // self-registers
   }

The ``AotListBase`` constructor runs at program startup, inserting the
registration function into a global linked list.

Stage 2 — Compile into the host
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``CMakeLists.txt`` for this tutorial uses the ``DAS_AOT`` macro
to automate both generation and compilation:

.. code-block:: cmake

   # 1. Create a custom target and accumulator variable
   SET(INTEGRATION_13_AOT_GENERATED_SRC)
   add_custom_target(integration_cpp_13_dasAotStub)

   # 2. Generate AOT C++ from the .das file using daslang as the tool
   DAS_AOT("tutorials/integration/cpp/13_aot.das"
           INTEGRATION_13_AOT_GENERATED_SRC
           integration_cpp_13_dasAotStub daslang)

   # 3. Build the executable with both hand-written AND generated sources
   add_executable(integration_cpp_13
       13_aot.cpp
       ${INTEGRATION_13_AOT_GENERATED_SRC}
   )
   TARGET_LINK_LIBRARIES(integration_cpp_13 libDaScript Threads::Threads)
   ADD_DEPENDENCIES(integration_cpp_13 libDaScript
                    integration_cpp_13_dasAotStub)

The ``DAS_AOT`` macro:

* Runs ``daslang.exe utils/aot/main.das -- -aot 13_aot.das output.cpp``
  as a custom build command
* Puts the generated file in an ``_aot_generated/`` subdirectory
* Creates a dependency on ``daslang`` so it's built first

The generated ``.cpp`` is compiled alongside ``13_aot.cpp`` —
no manual steps required.

Stage 3 — Enable AOT in the host
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   CodeOfPolicies policies;
   policies.aot = true;              // enable AOT linking
   policies.fail_on_no_aot = true;   // error if any function lacks AOT

   auto program = compileDaScript(scriptPath, fAccess, tout,
                                  libGroup, policies);

   Context ctx(program->getContextStackSize());
   program->simulate(ctx, tout);  // AOT functions linked here

   auto fn = ctx.findFunction("test");
   // Check if AOT-linked:
   if (fn->aot) { /* running native C++ */ }

During ``simulate()``, the engine computes a semantic hash for each
function, looks it up in the global AOT library, and replaces the
simulation node if a match is found.


Checking AOT status
====================

``SimFunction`` (returned by ``ctx.findFunction()``) has an ``aot``
flag:

.. code-block:: cpp

   auto fn = ctx.findFunction("test");
   printf("AOT: %s\n", fn->aot ? "yes" : "no");

Without the generated ``.cpp`` linked in, ``aot`` is ``false`` and
the function runs through the interpreter.  With it linked in,
``aot`` is ``true``.


Key policies
============

+---------------------------+------------------------------------------+
| ``policies.aot``          | Enable AOT linking during simulate       |
+---------------------------+------------------------------------------+
| ``policies.fail_on_no_aot``| Error if a function lacks AOT (default  |
|                           | ``true``)                                |
+---------------------------+------------------------------------------+
| ``policies.aot_module``   | This is a module AOT (not entry point)   |
+---------------------------+------------------------------------------+


Building and running
====================

::

   cmake --build build --config Release --target integration_cpp_13
   bin\Release\integration_cpp_13.exe

Expected output::

   === Interpreter mode ===
     test() is AOT: no
   === AOT Tutorial ===
   fib(0) = 0
   fib(1) = 1
   fib(2) = 1
   fib(3) = 2
   fib(4) = 3
   fib(5) = 5
   fib(6) = 8
   fib(7) = 13
   fib(8) = 21
   fib(9) = 34

   sin(pi/4) = 0.70710677
   cos(pi/4) = 0.70710677
   sqrt(2)   = 1.4142135

   === AOT mode (policies.aot = true) ===
     test() is AOT: yes
   === AOT Tutorial ===
   fib(0) = 0
   fib(1) = 1
   fib(2) = 1
   fib(3) = 2
   fib(4) = 3
   fib(5) = 5
   fib(6) = 8
   fib(7) = 13
   fib(8) = 21
   fib(9) = 34

   sin(pi/4) = 0.70710677
   cos(pi/4) = 0.70710677
   sqrt(2)   = 1.4142135

   === AOT workflow summary ===
   1. daslang.exe -aot script.das script.das.cpp
      -> generates C++ source from daScript functions
   2. Compile the .cpp into your executable
      -> functions self-register via static AotListBase
   3. Set CodeOfPolicies::aot = true before compileDaScript
      -> simulate() links AOT functions automatically
   4. Functions run as native C++ — no interpreter overhead

Without ``policies.aot``, the function runs through the interpreter
(``test() is AOT: no``).  With it set, the pre-compiled C++ code
is linked in (``test() is AOT: yes``) for near-native performance.


.. seealso::

   Full source:
   :download:`13_aot.cpp <../../../../tutorials/integration/cpp/13_aot.cpp>`,
   :download:`13_aot.das <../../../../tutorials/integration/cpp/13_aot.das>`

   Previous tutorial: :ref:`tutorial_integration_cpp_smart_pointers`

   Next tutorial: :ref:`tutorial_integration_cpp_serialization`
