.. _utils_aot:

.. index::
   single: Utils; aot
   single: Utils; AOT

=====================================
 aot --- AOT Generation Driver
=====================================

The aot tool compiles ``.das`` files ahead of time into C++ for linking
into a host application.  It is what the C++ integration tutorial
scaffolds (``tutorials/integration/*/CMakeLists.standalone.cmake``) and
this repo's own build invoke; it accepts multiple inputs per run, so
build systems can batch.

Quick start
===========

Generate AOT C++ for one script::

   daslang utils/aot/main.das -- -aot my_script.das output.cpp

Generate a standalone context (a directory with ``.h`` + ``.cpp``)::

   daslang utils/aot/main.das -- -ctx my_script.das output_dir/

The AOT workflow — registering generated sources, hash mismatches, and
the CMake integration — is covered by the C++ integration tutorials.

.. seealso::

   :ref:`tutorial_integration_cpp_aot` -- the end-to-end AOT integration tutorial
