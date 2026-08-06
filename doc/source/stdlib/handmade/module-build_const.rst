The BUILD_CONST module provides compile-time constants from the build
environment. Each ``build_*`` accessor is a call macro that reads the daslang
process's command line or environment variables while the requiring module
compiles, and splices the answer into the code as a literal — feed a global
``let`` and gate code with ``static_if`` to erase the disabled branch entirely.
Interpreted and JIT runs resolve the constants per launch; an ``-exe`` build
bakes them at build time.

All functions and symbols are in "build_const" module, use require to get access to it.

.. code-block:: das

    require daslib/build_const
