
.. _stdlib_cpp_bind:

======================
C++ bindings generator
======================

.. das:module:: cpp_bind

The CPP_BIND module provides utilities for generating daslang bindings
to C++ code. It helps generate module registration code, type annotations,
and function wrappers for exposing C++ APIs to daslang programs.

All functions and symbols are in "cpp_bind" module, use require to get access to it. ::

    require daslib/cpp_bind



++++++++++++++++++++++
Generation of bindings
++++++++++++++++++++++

  *  :ref:`log_cpp_class_adapter (cpp_file: file; name: string; cinfo: TypeDeclPtr) <function-cpp_bind_log_cpp_class_adapter_file_string_TypeDeclPtr>`

.. _function-cpp_bind_log_cpp_class_adapter_file_string_TypeDeclPtr:

.. das:function:: log_cpp_class_adapter(cpp_file: file; name: string; cinfo: TypeDeclPtr)

Generates C++ class adapter for the Daslang class.
Intended use::

    log_cpp_class_adapter(cppFileNameDotInc, "daslangClassName", typeinfo(ast_typedecl type<daslangClassName>))


:Arguments: * **cpp_file** :  :ref:`file <alias-file>`

            * **name** : string

            * **cinfo** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`


