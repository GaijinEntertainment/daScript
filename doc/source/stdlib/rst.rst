
.. _stdlib_rst:

=======================
Documentation generator
=======================

The RST module exposes collection of helper routines to automatically generate Daslang reStructuredText documentation.

All functions and symbols are in "rst" module, use require to get access to it. ::

    require daslib/rst


++++++++++
Structures
++++++++++

.. _struct-rst-DocGroup:

.. das:attribute:: DocGroup

|detail/structure-rst-DocGroup|

.. _struct-rst-DocsHook:

.. das:attribute:: DocsHook

|detail/structure-rst-DocsHook|

++++++++++++++++
Document writers
++++++++++++++++

  *  :ref:`document_enumerations (doc_file: file; mods: array\<Module?\>) : bool <function-_at_rst_c__c_document_enumerations_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C1_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_A>` 
  *  :ref:`document (name: string; mod: Module?; fname: string; groups: array\<DocGroup\>; hook: DocsHook = [[rst::DocsHook ]]) <function-_at_rst_c__c_document_Cs_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_C1_ls_S_ls_rst_c__c_DocGroup_gr__gr_A_S_ls_rst_c__c_DocsHook_gr_>` 
  *  :ref:`documents (name: string; mods: array\<Module?\>; fname: string; groups: array\<DocGroup\>; var hook: DocsHook = [[rst::DocsHook ]]) <function-_at_rst_c__c_documents_Cs_C1_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_A_Cs_C1_ls_S_ls_rst_c__c_DocGroup_gr__gr_A_S_ls_rst_c__c_DocsHook_gr_>` 
  *  :ref:`document_enumeration (doc_file: file; mod: Module?; value: auto) : auto <function-_at_rst_c__c_document_enumeration_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_dot_>` 

.. _function-_at_rst_c__c_document_enumerations_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C1_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_A:

.. das:function:: document_enumerations(doc_file: file; mods: array<Module?>) : bool

|detail/function-rst-document_enumerations-0x976bd37277f1fb55|

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

.. _function-_at_rst_c__c_document_Cs_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_C1_ls_S_ls_rst_c__c_DocGroup_gr__gr_A_S_ls_rst_c__c_DocsHook_gr_:

.. das:function:: document(name: string; mod: Module?; fname: string; groups: array<DocGroup>; hook: DocsHook = [[rst::DocsHook ]])

|detail/function-rst-document-0x1dcb64c1c8c124de|

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-_at_rst_c__c_documents_Cs_C1_ls_1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__gr_A_Cs_C1_ls_S_ls_rst_c__c_DocGroup_gr__gr_A_S_ls_rst_c__c_DocsHook_gr_:

.. das:function:: documents(name: string; mods: array<Module?>; fname: string; groups: array<DocGroup>; hook: DocsHook = [[rst::DocsHook ]])

|detail/function-rst-documents-0x7dce91107c2a3eea|

:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-_at_rst_c__c_document_enumeration_CY_ls_file_gr_1_ls_CH_ls_fio_c__c_FILE_gr__gr__qm__C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__C_dot_:

.. das:function:: document_enumeration(doc_file: file; mod: Module?; value: auto) : auto

|detail/function-rst-document_enumeration-0x750805a4f9ad5c1a|

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **value** : auto

++++++++++++++++++
RST section makers
++++++++++++++++++

  *  :ref:`make_group (name: string; plus: string = "+") : string <function-_at_rst_c__c_make_group_Cs_Cs>` 

.. _function-_at_rst_c__c_make_group_Cs_Cs:

.. das:function:: make_group(name: string; plus: string = "+") : string

|detail/function-rst-make_group-0x5ea77103afb58c31|

:Arguments: * **name** : string

            * **plus** : string

++++++++++++++++++++
Documentaton queries
++++++++++++++++++++

  *  :ref:`function_needs_documenting (func: smart_ptr\<Function\>|Function?) : auto <function-_at_rst_c__c_function_needs_documenting_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|>` 

.. _function-_at_rst_c__c_function_needs_documenting_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|:

.. das:function:: function_needs_documenting(func: smart_ptr<Function>|Function?) : auto

|detail/function-rst-function_needs_documenting-0x24bd940db719ff25|

:Arguments: * **func** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

++++++++++++++++
Group operations
++++++++++++++++

  *  :ref:`hide_group (var group: DocGroup) : DocGroup <function-_at_rst_c__c_hide_group_S_ls_rst_c__c_DocGroup_gr_>` 

.. _function-_at_rst_c__c_hide_group_S_ls_rst_c__c_DocGroup_gr_:

.. das:function:: hide_group(group: DocGroup) : DocGroup

|detail/function-rst-hide_group-0xf759dae975a0584b|

:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_rst_c__c_safe_function_name_Cs:

.. das:function:: safe_function_name(name: string) : string

|detail/function-rst-safe_function_name-0xefb2f32a0bf06a5c|

:Arguments: * **name** : string

.. _function-_at_rst_c__c_function_label_file_C0_ls_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M;C1_ls_H_ls_ast_c__c_Function_gr__gr__qm__gr_|_Ci:

.. das:function:: function_label_file(value: smart_ptr<Function>|Function?; drop_args: int = 0) : auto

|detail/function-rst-function_label_file-0x14df9f225d75bb39|

:Arguments: * **value** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

            * **drop_args** : int

.. _function-_at_rst_c__c_function_label_file_C_dot__C&1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Ci:

.. das:function:: function_label_file(name: auto; value: smart_ptr<TypeDecl> const&; drop_args: int = 0) : auto

|detail/function-rst-function_label_file-0x821ec1ff3a1ddec1|

:Arguments: * **name** : auto

            * **value** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >&

            * **drop_args** : int


