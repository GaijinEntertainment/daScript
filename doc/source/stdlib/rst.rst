
.. _stdlib_rst:

=======================
Documentation generator
=======================

.. das:module:: rst

The RST module implements the documentation generation pipeline for daslang.
It uses RTTI to introspect modules, types, and functions, then produces
reStructuredText output suitable for Sphinx documentation builds.

All functions and symbols are in "rst" module, use require to get access to it.

.. code-block:: das

    require daslib/rst



++++++++++
Structures
++++++++++

.. _struct-rst-DocGroup:

.. das:attribute:: DocGroup

Group of documentation items.

:Fields: * **name** : string - Name of the group.

         * **func** : array<tuple<fn: :ref:`Function <handle-ast-Function>`?;mod: :ref:`Module <handle-rtti-Module>`?>> - Functions in the group.

         * **hidden** : bool - Whether the group is hidden.

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module, to which this group belongs.



.. _struct-rst-DocsHook:

.. das:attribute:: DocsHook

Hook for RST documentation generation.

:Fields: * **annotationFilter** : lambda<(ann: :ref:`Annotation <handle-rtti-Annotation>`):bool> - Filter for the supported annotations.

         * **afterEnums** : lambda<(f: :ref:`FILE <handle-fio-FILE>`?;was_enums:bool):void> - Additional generation hook after the enumerations.




++++++++++++++++
Document writers
++++++++++++++++

  *  :ref:`document (name: string; var mod: Module?; fname: string; var groups: array\<DocGroup\>; hook: DocsHook = DocsHook()) <function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook>`
  *  :ref:`document_enumeration (doc_file: file; mod: Module?; value: auto) : auto <function-rst_document_enumeration_file_Module_q__auto_0x42a>`
  *  :ref:`document_enumerations (doc_file: file; mods: array\<Module?\>) : bool <function-rst_document_enumerations_file_array_ls_Module_q__gr_>`
  *  :ref:`documents (name: string; mods: array\<Module?\>; fname: string; var groups: array\<DocGroup\>; var hook: DocsHook = DocsHook()) <function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook>`

.. _function-rst_document_string_Module_q__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: document(name: string; mod: Module?; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

Generates RST documentation for a single module and writes it to a file.


:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>`>

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>`

.. _function-rst_document_enumeration_file_Module_q__auto_0x42a:

.. das:function:: document_enumeration(doc_file: file; mod: Module?; value: auto) : auto

Generates RST documentation for a single enumeration type.


:Arguments: * **doc_file** :  :ref:`file <alias-file>`

            * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **value** : auto

.. _function-rst_document_enumerations_file_array_ls_Module_q__gr_:

.. das:function:: document_enumerations(doc_file: file; mods: array<Module?>) : bool

Generates RST documentation for all enumerations in the given modules.


:Arguments: * **doc_file** :  :ref:`file <alias-file>`

            * **mods** : array< :ref:`Module <handle-rtti-Module>`?>

.. _function-rst_documents_string_array_ls_Module_q__gr__string_array_ls_DocGroup_gr__DocsHook:

.. das:function:: documents(name: string; mods: array<Module?>; fname: string; groups: array<DocGroup>; hook: DocsHook = DocsHook())

Generates RST documentation for multiple modules and writes them to files.


:Arguments: * **name** : string

            * **mods** : array< :ref:`Module <handle-rtti-Module>`?>

            * **fname** : string

            * **groups** : array< :ref:`DocGroup <struct-rst-DocGroup>`>

            * **hook** :  :ref:`DocsHook <struct-rst-DocsHook>`


++++++++++++
Descriptions
++++++++++++

  *  :ref:`describe_short (expr: Expression?|smart_ptr\<Expression\>) : auto <function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_>`

.. _function-rst_describe_short_Expression_q_smart_ptr_ls_Expression_gr_:

.. das:function:: describe_short(expr: Expression?|smart_ptr<Expression>) : auto

Returns a concise one-line description of an expression or type.


:Arguments: * **expr** : option< :ref:`Expression <handle-ast-Expression>`?\ | smart_ptr< :ref:`Expression <handle-ast-Expression>`>\ &>


++++++++++++
Label makers
++++++++++++

  *  :ref:`function_label_file (name: auto; value: smart_ptr\<TypeDecl\>; drop_args: int = 0) : auto <function-rst_function_label_file_auto_smart_ptr_ls_TypeDecl_gr__int_0xa0>`
  *  :ref:`function_label_file (value: smart_ptr\<Function\>|Function?; drop_args: int = 0) : auto <function-rst_function_label_file_smart_ptr_ls_Function_gr_Function_q__int>`


function_label_file
^^^^^^^^^^^^^^^^^^^

.. _function-rst_function_label_file_auto_smart_ptr_ls_TypeDecl_gr__int_0xa0:

.. das:function:: function_label_file(name: auto; value: smart_ptr<TypeDecl>; drop_args: int = 0) : auto

Creates a unique, file-name-safe label string for a function.


:Arguments: * **name** : auto

            * **value** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>`>\ &

            * **drop_args** : int

.. _function-rst_function_label_file_smart_ptr_ls_Function_gr_Function_q__int:

.. das:function:: function_label_file(value: smart_ptr<Function>|Function?; drop_args: int = 0) : auto


++++++++++++++++++
RST section makers
++++++++++++++++++

  *  :ref:`make_group (name: string; plus: string = "+") : string <function-rst_make_group_string_string>`

.. _function-rst_make_group_string_string:

.. das:function:: make_group(name: string; plus: string = "+") : string

Creates a named documentation group with a decorative RST section header.


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

Appends functions whose names match a regex to an existing documentation group.


:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>`

            * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **reg** :  :ref:`Regex <struct-regex-Regex>`


group_by_regex
^^^^^^^^^^^^^^

.. _function-rst_group_by_regex_string_Module_q__Regex:

.. das:function:: group_by_regex(name: string; mod: Module?; reg: Regex) : DocGroup

Groups module items whose names match the provided regular expression under a documentation section.


:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **reg** :  :ref:`Regex <struct-regex-Regex>`

.. _function-rst_group_by_regex_string_array_ls_Module_q__gr__Regex:

.. das:function:: group_by_regex(name: string; mods: array<Module?>; reg: Regex) : DocGroup

----

.. _function-rst_hide_group_DocGroup:

.. das:function:: hide_group(group: DocGroup) : DocGroup

Marks the specified documentation group as hidden so it is excluded from output.


:Arguments: * **group** :  :ref:`DocGroup <struct-rst-DocGroup>`


++++++++++++++
Naming helpers
++++++++++++++

  *  :ref:`safe_function_name (name: string) : string <function-rst_safe_function_name_string>`

.. _function-rst_safe_function_name_string:

.. das:function:: safe_function_name(name: string) : string

Escapes special characters in a function name to produce a safe identifier for RST output.


:Arguments: * **name** : string


