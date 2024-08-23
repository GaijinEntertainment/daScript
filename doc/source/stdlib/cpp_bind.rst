
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

  *  :ref:`log_cpp_class_adapter (cpp_file: file; name: string; cinfo: TypeDeclPtr) <function-_at_cpp_bind_c__c_log_cpp_class_adapter_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Cs_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M>` 

.. _function-_at_cpp_bind_c__c_log_cpp_class_adapter_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__Cs_CY_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: log_cpp_class_adapter(cpp_file: file; name: string; cinfo: TypeDeclPtr)

Generates C++ class adapter for the Daslang class.
Intended use::

    log_cpp_class_adapter(cppFileNameDotInc, "daslangClassName", typeinfo(ast_typedecl type<daslangClassName>))

:Arguments: * **cpp_file** :  :ref:`file <alias-file>` 

            * **name** : string

            * **cinfo** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 


