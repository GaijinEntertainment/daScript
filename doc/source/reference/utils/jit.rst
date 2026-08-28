.. _utils_jit:

.. index::
   single: Utils; jit
   single: Utils; JIT
   single: Utils; LLVM

==================================================
 jit --- LLVM Compile Driver
==================================================

The jit tool compiles ``.das`` files through the LLVM backend without
running them.  One run produces, depending on the mode, a cached shared
library (the JIT dll cache), a standalone executable, or a native ``.o``
for static linking into a host binary.

It accepts files and directories, so a whole tree can be compiled in one
go; a directory is walked recursively, skipping ``_``-prefixed entries
and the ``cant_`` / ``failed_`` / ``invalid_`` expected-failure tests.

Requires a daslang built with the LLVM backend.

.. contents::
   :local:
   :depth: 2


Quick start
===========

Prewarm the JIT dll cache for a tree, one worker per hardware thread::

   daslang utils/jit/main.das -- path/to/dir --parallel 0

Build a standalone executable from a script::

   daslang utils/jit/main.das -- my_script.das --exe -o my_script

Emit a native object that registers its functions into the AOT library::

   daslang utils/jit/main.das -- my_script.das --aot-object -o my_script


Modes
=====

Shared library (default)
   The target is compiled JIT-aware and the generated ``.dll`` is written
   under ``.jitted_scripts/<namespace>/``, content-hashed, unless
   ``--output`` names a path.  A later run of the same script loads the
   cached library instead of generating code again.

``--exe``
   Emits a standalone executable.  ``--jit-target`` cross-compiles (for
   example ``wasm32-unknown-emscripten``), and
   ``--jit-register-all-modules`` registers every builtin native module at
   startup, which a standalone compiler-driver binary needs in order to
   recompile arbitrary daslang at runtime.

``--aot-object``
   Emits a native ``.o`` covering the used function set, with a load
   constructor that registers those functions into the AOT library.
   ``--aot-object-prefix`` derives one object per input under
   ``<dir>/_llvm_aot_generated/``, so a single process can emit many
   objects and amortize the dasLLVM load cost.


Parallel compilation
====================

``--parallel N`` spawns *N* worker subprocesses; ``--parallel 0`` (or the
flag with no value) picks the hardware thread count.  Omitting the flag
compiles sequentially in-process.  Workers are full subprocesses because
the compile pipeline is not thread-safe within one process.

``--parallel`` is rejected together with ``--output`` — every file would
collide on the one path.

``--exclude <name>`` skips directories by their own name, at any depth,
and is repeatable.


Tuning flags
============

The JIT tuning flags are accepted alongside the tool's own — among them
``--jit-opt-level``, ``--jit-size-level``, ``--jit-debug``, ``--jit-stack``,
``--jit-dump``, ``--jit-split-modules``, ``--jit-path-to-linker`` and
``--jit-linker-string``.  ``daslang utils/jit/main.das -- -?`` prints both
sets with their descriptions.

.. seealso::

   :ref:`utils_aot` -- the C++ AOT generation driver
