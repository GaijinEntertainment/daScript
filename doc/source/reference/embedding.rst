.. _theembedding:

*******************************
  Embedding daslang into C++
*******************************

This section explains how to embed the daslang scripting language into a
C++ host application.  It is organized from simple to advanced:

* **Quick Start** — minimal host program, compilation, and evaluation
* **C++ API Reference** — modules, function/type/enum bindings, cast infrastructure
* **C API Reference** — the ``daScript/c_api/c_api.h`` header for C-only hosts
* **Advanced Topics** — AOT compilation, standalone contexts, class adapters

For step-by-step walk-throughs with complete, compilable source code, see the
:ref:`C++ integration tutorials <tutorial_integration_cpp_hello_world>` and
:ref:`C integration tutorials <tutorial_integration_c_hello_world>`.

.. toctree::

   embedding/quickstart.rst
   embedding/cpp_api.rst
   embedding/c_api.rst
   embedding/advanced.rst

