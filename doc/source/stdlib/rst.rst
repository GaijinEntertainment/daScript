
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

  *  :ref:`document_enumerations (doc_file: file; mods: array\<Module?\>) : bool <function-rst_document_enumerations_file_array_ls_Module_q__gr_>` 
  *  :ref:`document (name: string; var mod: Module?; fname: string; var groups: array\<DocGroup\>; hook: DocsHook = DocsHook()) <function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook>` 
  *  :ref:`documents (name: string; mods: array\<Module?\>; fname: string; var groups: array\<DocGroup\>; var hook: DocsHook = DocsHook()) <function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook>` 
  *  :ref:`document_enumeration (doc_file: file; mod: Module?; value: auto) : auto <function-rst_document_enumeration_file_Module_q__auto>` 

.. _function-rst_document_enumerations_file_array_ls_Module_q__gr_:

.. das:function:: document_enumerations(doc_file: file; mods: array<Module?>) : bool

|detail/function-rst-document_enumerations-0x976bd37277f1fb55|

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

.. _function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: document(name: string; mod: Module?; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

|detail/function-rst-document-0x79ba3e8e1f0b229d|

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: documents(name: string; mods: array<Module?>; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

|detail/function-rst-documents-0x86ca6f0890c1f49d|

:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-rst_document_enumeration_file_Module_q__auto:

.. das:function:: document_enumeration(doc_file: file; mod: Module?; value: auto) : auto

|detail/function-rst-document_enumeration-0x750805a4f9ad5c1a|

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **value** : auto

++++++++++++
Descriptions
++++++++++++

  *  :ref:`describe_short (expr: Expression?|smart_ptr\<Expression\>) : auto <function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_>` 

.. _function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_:

.. das:function:: describe_short(expr: Expression?|smart_ptr<Expression>) : auto

|detail/function-rst-describe_short-0xdc1e2684fd95608e|

:Arguments: * **expr** : option< :ref:`Expression <handle-ast-Expression>` ?|smart_ptr< :ref:`Expression <handle-ast-Expression>` >&>

++++++++++++++++++
RST section makers
++++++++++++++++++

  *  :ref:`make_group (name: string; plus: string = "+") : string <function-rst_make_group_string_string>` 

.. _function-rst_make_group_string_string:

.. das:function:: make_group(name: string; plus: string = "+") : string

|detail/function-rst-make_group-0x5ea77103afb58c31|

:Arguments: * **name** : string

            * **plus** : string

++++++++++++++++
Group operations
++++++++++++++++

  *  :ref:`append_to_group_by_regex (var group: DocGroup; var mod: Module?; var reg: Regex) : DocGroup& <function-rst_append_to_group_by_regex_DocGroup_Module_q__Regex>` 
  *  :ref:`group_by_regex (name: string; var mod: Module?; var reg: Regex) : DocGroup <function-rst_group_by_regex_string_Module_q__Regex>` 
  *  :ref:`group_by_regex (name: string; var mods: array\<Module?\>; var reg: Regex) : DocGroup <function-rst_group_by_regex_string_array_ls_Module_q__gr__Regex>` 
  *  :ref:`hide_group (var group: DocGroup) : DocGroup <function-rst_hide_group_DocGroup>` 

.. _function-rst_append_to_group_by_regex_DocGroup_Module_q__Regex:

.. das:function:: append_to_group_by_regex(group: DocGroup; mod: Module?; reg: Regex) : DocGroup&

|detail/function-rst-append_to_group_by_regex-0xef2819470b739bd8|

:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_group_by_regex_string_Module_q__Regex:

.. das:function:: group_by_regex(name: string; mod: Module?; reg: Regex) : DocGroup

|detail/function-rst-group_by_regex-0xe99d9c2c0e10bd06|

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_group_by_regex_string_array_ls_Module_q__gr__Regex:

.. das:function:: group_by_regex(name: string; mods: array<Module?>; reg: Regex) : DocGroup

|detail/function-rst-group_by_regex-0x7b1aedda3b773906|

:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_hide_group_DocGroup:

.. das:function:: hide_group(group: DocGroup) : DocGroup

|detail/function-rst-hide_group-0xc1116dada303b6e7|

:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-rst_safe_function_name_string:

.. das:function:: safe_function_name(name: string) : string

|detail/function-rst-safe_function_name-0xefb2f32a0bf06a5c|

:Arguments: * **name** : string

.. _function-rst_mkdir_rec_string:

.. das:function:: mkdir_rec(path: string) : bool

|detail/function-rst-mkdir_rec-0x5bdf80adff742967|

:Arguments: * **path** : string

.. _function-rst_function_label_file_smart_ptr_ls_Function_gr_Function_q__int:

.. das:function:: function_label_file(value: smart_ptr<Function>|Function?; drop_args: int = 0) : auto

|detail/function-rst-function_label_file-0x14df9f225d75bb39|

:Arguments: * **value** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

            * **drop_args** : int

.. _function-rst_function_label_file_auto_smart_ptr_ls_TypeDecl_gr__int:

.. das:function:: function_label_file(name: auto; value: smart_ptr<TypeDecl>; drop_args: int = 0) : auto

|detail/function-rst-function_label_file-0xc77a64e03bd14298|

:Arguments: * **name** : auto

            * **value** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >&

            * **drop_args** : int


