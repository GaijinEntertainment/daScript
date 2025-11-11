
.. _stdlib_cpp_bind:

======================
C++ bindings generator
======================

The cpp_bind module implements generation of C++ bindings for the Daslang interfaces.

All functions and symbols are in "cpp_bind" module, use require to get access to it. ::

    require daslib/cpp_bind

For example, from tutorial04.das ::

    require fio
    require ast
    require daslib/cpp_bind
    [init]
    def generate_cpp_bindings
        let root = get_das_root() + "/examples/tutorial/"
        fopen(root + "tutorial04_gen.inc","wb") <| $ ( cpp_file )
            log_cpp_class_adapter(cpp_file, "TutorialBaseClass", typeinfo(ast_typedecl type<TutorialBaseClass>))


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


