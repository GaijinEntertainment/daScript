
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

Group of documentation items.

:Fields: * **name** : string - Name of the group.

         * **func** : array<tuple<fn: :ref:`Function <handle-ast-Function>` ?;mod: :ref:`Module <handle-rtti-Module>` ?>> - Functions in the group.

         * **hidden** : bool - Whether the group is hidden.

         * **_module** :  :ref:`Module <handle-rtti-Module>` ? - Module, to which this group belongs.


.. _struct-rst-DocsHook:

.. das:attribute:: DocsHook

Hook for RST documentation generation.

:Fields: * **annotationFilter** : lambda<(ann: :ref:`Annotation <handle-rtti-Annotation>` ):bool> - Filter for the supported annotations.

         * **afterEnums** : lambda<(f: :ref:`FILE <handle-fio-FILE>` ?;was_enums:bool):void> - Additional generation hook after the enumerations.


++++++++++++++++
Document writers
++++++++++++++++

  *  :ref:`document_enumerations (doc_file: file; mods: array\<Module?\>) : bool <function-rst_document_enumerations_file_array_ls_Module_q__gr_>` 
  *  :ref:`document (name: string; var mod: Module?; fname: string; var groups: array\<DocGroup\>; hook: DocsHook = DocsHook()) <function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook>` 
  *  :ref:`documents (name: string; mods: array\<Module?\>; fname: string; var groups: array\<DocGroup\>; var hook: DocsHook = DocsHook()) <function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook>` 
  *  :ref:`document_enumeration (doc_file: file; mod: Module?; value: auto) : auto <function-rst_document_enumeration_file_Module_q__auto>` 

.. _function-rst_document_enumerations_file_array_ls_Module_q__gr_:

.. das:function:: document_enumerations(doc_file: file; mods: array<Module?>) : bool

Documentation for enumerations in the given modules.

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

.. _function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: document(name: string; mod: Module?; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

Documents the specified module into a RST file.

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: documents(name: string; mods: array<Module?>; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

Documents the specified modules into a RST file.

:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>` >

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>` 

.. _function-rst_document_enumeration_file_Module_q__auto:

.. das:function:: document_enumeration(doc_file: file; mod: Module?; value: auto) : auto

Documentation for enumeration types.

:Arguments: * **doc_file** :  :ref:`file <alias-file>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **value** : auto

++++++++++++
Descriptions
++++++++++++

  *  :ref:`describe_short (expr: Expression?|smart_ptr\<Expression\>) : auto <function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_>` 

.. _function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_:

.. das:function:: describe_short(expr: Expression?|smart_ptr<Expression>) : auto

Describes the expression in short form.

:Arguments: * **expr** : option< :ref:`Expression <handle-ast-Expression>` ?|smart_ptr< :ref:`Expression <handle-ast-Expression>` >&>

++++++++++++++++++
RST section makers
++++++++++++++++++

  *  :ref:`make_group (name: string; plus: string = "+") : string <function-rst_make_group_string_string>` 

.. _function-rst_make_group_string_string:

.. das:function:: make_group(name: string; plus: string = "+") : string

Creates a decorative group header for RST documentation.

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

Adds functions matching a regex to a documentation group.

:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_group_by_regex_string_Module_q__Regex:

.. das:function:: group_by_regex(name: string; mod: Module?; reg: Regex) : DocGroup

Groups module items by regex.

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_group_by_regex_string_array_ls_Module_q__gr__Regex:

.. das:function:: group_by_regex(name: string; mods: array<Module?>; reg: Regex) : DocGroup

Groups items in the module by matching their names against the provided regular expression.

:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>` ?>

            * **reg** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-rst_hide_group_DocGroup:

.. das:function:: hide_group(group: DocGroup) : DocGroup

Makes the specified documentation group hidden.

:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-rst_safe_function_name_string:

.. das:function:: safe_function_name(name: string) : string

Creates a safe function name by replacing special characters.

:Arguments: * **name** : string

.. _function-rst_function_label_file_smart_ptr_ls_Function_gr_Function_q__int:

.. das:function:: function_label_file(value: smart_ptr<Function>|Function?; drop_args: int = 0) : auto

Creates a unique label for a function suitable for use in file names.

:Arguments: * **value** : option< :ref:`FunctionPtr <alias-FunctionPtr>` | :ref:`Function <handle-ast-Function>` ?>

            * **drop_args** : int

.. _function-rst_function_label_file_auto_smart_ptr_ls_TypeDecl_gr__int:

.. das:function:: function_label_file(name: auto; value: smart_ptr<TypeDecl>; drop_args: int = 0) : auto

Creates a unique label for a function based on its name and argument types.

:Arguments: * **name** : auto

            * **value** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >&

            * **drop_args** : int


