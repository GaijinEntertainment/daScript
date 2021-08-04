
.. _stdlib_rst:

=======================
Documentation generator
=======================

.. include:: detail/rst.rst

The RST module exposes collection of helper routines to automatically generate daScript reStructuredText documentation.

All functions and symbols are in "rst" module, use require to get access to it. ::

    require daslib/rst


+++++++++
Constants
+++++++++

.. _global-rst-log_documentation:

.. das:attribute:: log_documentation = true

|variable-rst-log_documentation|

.. _global-rst-show_hidden_groups:

.. das:attribute:: show_hidden_groups = true

|variable-rst-show_hidden_groups|

.. _struct-rst-DocGroup:

.. das:attribute:: DocGroup



DocGroup fields are

+------+----------------------------------------------------+
+name  +string                                              +
+------+----------------------------------------------------+
+func  +array< :ref:`ast::Function <handle-ast-Function>` ?>+
+------+----------------------------------------------------+
+hidden+bool                                                +
+------+----------------------------------------------------+


|structure-rst-DocGroup|

++++++++++++++++
Document writers
++++++++++++++++

  *  :ref:`document (name:string const;mod:rtti::Module? const;fname:string const;substname:string const;groups:array\<rst::DocGroup\> const) : void <function-_at_rst_c__c_document__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_array_hh_DocGroup>` 
  *  :ref:`document_annotations (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_bitfield (doc_file:fio::FILE const? const;value:smart_ptr\<ast::TypeDecl\> const) : void <function-_at_rst_c__c_document_bitfield__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`document_call_macro (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:string const implicit) : void <function-_at_rst_c__c_document_call_macro__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`document_call_macros (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_call_macros__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_classes (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_classes__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_enumerations (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_enumerations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_function (doc_file:fio::FILE const? const;mod:rtti::Module? const;func:smart_ptr\<ast::Function\> const) : void <function-_at_rst_c__c_document_function__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function>` 
  *  :ref:`document_function_annotations (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_function_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_function_arguments (doc_file:fio::FILE const? const;argNames:array\<string\> const;argTypes:array\<ast::TypeDecl?\> const) : void <function-_at_rst_c__c_document_function_arguments__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_array_hh_string__hh_const_hh__hh_array_hh__hh_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`document_function_declaration (doc_file:fio::FILE const? const;domain:string const;func_name:string const;argNames:array\<string\> const;argTypes:array\<ast::TypeDecl?\> const;resType:smart_ptr\<ast::TypeDecl\> const) : void <function-_at_rst_c__c_document_function_declaration__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_array_hh_string__hh_const_hh__hh_array_hh__hh_ptr_hh__hh_handle_hh_TypeDecl__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`document_functions (doc_file:fio::FILE const? const;mod:rtti::Module? const;groups:array\<rst::DocGroup\> const) : void <function-_at_rst_c__c_document_functions__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_array_hh_DocGroup>` 
  *  :ref:`document_global_constant (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:smart_ptr\<ast::Variable\> const) : void <function-_at_rst_c__c_document_global_constant__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Variable>` 
  *  :ref:`document_global_constants (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_global_constants__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_structure (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:smart_ptr\<ast::Structure\> const) : void <function-_at_rst_c__c_document_structure__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Structure>` 
  *  :ref:`document_structure_annotations (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_structure_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_structures (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_structures__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_topic (doc_file:fio::FILE const? const;topic:string const implicit) : void <function-_at_rst_c__c_document_topic__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`document_typedefs (doc_file:fio::FILE const? const;mod:rtti::Module? const) : void <function-_at_rst_c__c_document_typedefs__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`document_variant (doc_file:fio::FILE const? const;value:smart_ptr\<ast::TypeDecl\> const) : void <function-_at_rst_c__c_document_variant__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`document_warning (doc_file:fio::FILE const? const;issue:string const) : void <function-_at_rst_c__c_document_warning__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_string>` 
  *  :ref:`document_annotation (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:auto const) : auto <function-_at_rst_c__c_document_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto>` 
  *  :ref:`document_class_method (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:auto const;fld:auto const) : auto <function-_at_rst_c__c_document_class_method__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto__hh_const_hh__hh_auto>` 
  *  :ref:`document_enumeration (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:auto const) : auto <function-_at_rst_c__c_document_enumeration__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto>` 
  *  :ref:`document_function_annotation (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:auto const) : auto <function-_at_rst_c__c_document_function_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto>` 
  *  :ref:`document_structure_annotation (doc_file:fio::FILE const? const;mod:rtti::Module? const;value:auto const) : auto <function-_at_rst_c__c_document_structure_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto>` 
  *  :ref:`document_typedef (doc_file:fio::FILE const? const;mod:rtti::Module? const;name:string const implicit;value:auto const) : auto <function-_at_rst_c__c_document_typedef__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_auto>` 

.. _function-_at_rst_c__c_document__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_array_hh_DocGroup:

.. das:function:: document(name: string const; mod: rtti::Module? const; fname: string const; substname: string const; groups: array<rst::DocGroup> const)

+---------+---------------------------------------------------------+
+argument +argument type                                            +
+=========+=========================================================+
+name     +string const                                             +
+---------+---------------------------------------------------------+
+mod      + :ref:`rtti::Module <handle-rtti-Module>` ? const        +
+---------+---------------------------------------------------------+
+fname    +string const                                             +
+---------+---------------------------------------------------------+
+substname+string const                                             +
+---------+---------------------------------------------------------+
+groups   +array< :ref:`rst::DocGroup <struct-rst-DocGroup>` > const+
+---------+---------------------------------------------------------+


|function-rst-document|

.. _function-_at_rst_c__c_document_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_annotations(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_annotations|

.. _function-_at_rst_c__c_document_bitfield__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: document_bitfield(doc_file: file; value: TypeDeclPtr)

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+doc_file+ :ref:`file <alias-file>`               +
+--------+----------------------------------------+
+value   + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-rst-document_bitfield|

.. _function-_at_rst_c__c_document_call_macro__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string:

.. das:function:: document_call_macro(doc_file: file; mod: rtti::Module? const; value: string const implicit)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +string const implicit                            +
+--------+-------------------------------------------------+


|function-rst-document_call_macro|

.. _function-_at_rst_c__c_document_call_macros__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_call_macros(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_call_macros|

.. _function-_at_rst_c__c_document_classes__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_classes(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_classes|

.. _function-_at_rst_c__c_document_enumerations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_enumerations(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_enumerations|

.. _function-_at_rst_c__c_document_function__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function:

.. das:function:: document_function(doc_file: file; mod: rtti::Module? const; func: FunctionPtr)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`          +
+--------+-------------------------------------------------+


|function-rst-document_function|

.. _function-_at_rst_c__c_document_function_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_function_annotations(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_function_annotations|

.. _function-_at_rst_c__c_document_function_arguments__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_array_hh_string__hh_const_hh__hh_array_hh__hh_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: document_function_arguments(doc_file: file; argNames: array<string> const; argTypes: array<ast::TypeDecl?> const)

+--------+----------------------------------------------------------+
+argument+argument type                                             +
+========+==========================================================+
+doc_file+ :ref:`file <alias-file>`                                 +
+--------+----------------------------------------------------------+
+argNames+array<string> const                                       +
+--------+----------------------------------------------------------+
+argTypes+array< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` ?> const+
+--------+----------------------------------------------------------+


|function-rst-document_function_arguments|

.. _function-_at_rst_c__c_document_function_declaration__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_array_hh_string__hh_const_hh__hh_array_hh__hh_ptr_hh__hh_handle_hh_TypeDecl__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: document_function_declaration(doc_file: file; domain: string const; func_name: string const; argNames: array<string> const; argTypes: array<ast::TypeDecl?> const; resType: TypeDeclPtr)

+---------+----------------------------------------------------------+
+argument +argument type                                             +
+=========+==========================================================+
+doc_file + :ref:`file <alias-file>`                                 +
+---------+----------------------------------------------------------+
+domain   +string const                                              +
+---------+----------------------------------------------------------+
+func_name+string const                                              +
+---------+----------------------------------------------------------+
+argNames +array<string> const                                       +
+---------+----------------------------------------------------------+
+argTypes +array< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` ?> const+
+---------+----------------------------------------------------------+
+resType  + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                   +
+---------+----------------------------------------------------------+


|function-rst-document_function_declaration|

.. _function-_at_rst_c__c_document_functions__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_array_hh_DocGroup:

.. das:function:: document_functions(doc_file: file; mod: rtti::Module? const; groups: array<rst::DocGroup> const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+doc_file+ :ref:`file <alias-file>`                                +
+--------+---------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const        +
+--------+---------------------------------------------------------+
+groups  +array< :ref:`rst::DocGroup <struct-rst-DocGroup>` > const+
+--------+---------------------------------------------------------+


|function-rst-document_functions|

.. _function-_at_rst_c__c_document_global_constant__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Variable:

.. das:function:: document_global_constant(doc_file: file; mod: rtti::Module? const; value: VariablePtr)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   + :ref:`VariablePtr <alias-VariablePtr>`          +
+--------+-------------------------------------------------+


|function-rst-document_global_constant|

.. _function-_at_rst_c__c_document_global_constants__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_global_constants(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_global_constants|

.. _function-_at_rst_c__c_document_structure__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Structure:

.. das:function:: document_structure(doc_file: file; mod: rtti::Module? const; value: StructurePtr)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   + :ref:`StructurePtr <alias-StructurePtr>`        +
+--------+-------------------------------------------------+


|function-rst-document_structure|

.. _function-_at_rst_c__c_document_structure_annotations__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_structure_annotations(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_structure_annotations|

.. _function-_at_rst_c__c_document_structures__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_structures(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_structures|

.. _function-_at_rst_c__c_document_topic__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_implicit_hh_string:

.. das:function:: document_topic(doc_file: file; topic: string const implicit)

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+doc_file+ :ref:`file <alias-file>` +
+--------+--------------------------+
+topic   +string const implicit     +
+--------+--------------------------+


|function-rst-document_topic|

.. _function-_at_rst_c__c_document_typedefs__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: document_typedefs(doc_file: file; mod: rtti::Module? const)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-document_typedefs|

.. _function-_at_rst_c__c_document_variant__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: document_variant(doc_file: file; value: TypeDeclPtr)

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+doc_file+ :ref:`file <alias-file>`               +
+--------+----------------------------------------+
+value   + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-rst-document_variant|

.. _function-_at_rst_c__c_document_warning__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh_string:

.. das:function:: document_warning(doc_file: file; issue: string const)

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+doc_file+ :ref:`file <alias-file>` +
+--------+--------------------------+
+issue   +string const              +
+--------+--------------------------+


|function-rst-document_warning|

.. _function-_at_rst_c__c_document_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto:

.. das:function:: document_annotation(doc_file: file; mod: rtti::Module? const; value: auto const)

document_annotation returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_annotation|

.. _function-_at_rst_c__c_document_class_method__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto__hh_const_hh__hh_auto:

.. das:function:: document_class_method(doc_file: file; mod: rtti::Module? const; value: auto const; fld: auto const)

document_class_method returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+
+fld     +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_class_method|

.. _function-_at_rst_c__c_document_enumeration__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto:

.. das:function:: document_enumeration(doc_file: file; mod: rtti::Module? const; value: auto const)

document_enumeration returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_enumeration|

.. _function-_at_rst_c__c_document_function_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto:

.. das:function:: document_function_annotation(doc_file: file; mod: rtti::Module? const; value: auto const)

document_function_annotation returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_function_annotation|

.. _function-_at_rst_c__c_document_structure_annotation__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_auto:

.. das:function:: document_structure_annotation(doc_file: file; mod: rtti::Module? const; value: auto const)

document_structure_annotation returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_structure_annotation|

.. _function-_at_rst_c__c_document_typedef__hh_const_hh__hh_ptr_hh__hh_const_hh__hh_handle_hh_FILE__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string__hh_const_hh__hh_auto:

.. das:function:: document_typedef(doc_file: file; mod: rtti::Module? const; name: string const implicit; value: auto const)

document_typedef returns auto

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+doc_file+ :ref:`file <alias-file>`                        +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const implicit                            +
+--------+-------------------------------------------------+
+value   +auto const                                       +
+--------+-------------------------------------------------+


|function-rst-document_typedef|

++++++++++++
Descriptions
++++++++++++

  *  :ref:`describe_type (td:ast::TypeDecl? const) : string <function-_at_rst_c__c_describe_type__hh_const_hh__hh_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`describe_type (td:smart_ptr\<ast::TypeDecl\> const) : string <function-_at_rst_c__c_describe_type__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`describe_type_short (td:ast::TypeDecl? const) : string <function-_at_rst_c__c_describe_type_short__hh_const_hh__hh_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`describe_type_short (td:smart_ptr\<ast::TypeDecl\> const) : string <function-_at_rst_c__c_describe_type_short__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 

.. _function-_at_rst_c__c_describe_type__hh_const_hh__hh_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: describe_type(td: ast::TypeDecl? const)

describe_type returns string

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+td      + :ref:`ast::TypeDecl <handle-ast-TypeDecl>` ? const+
+--------+---------------------------------------------------+


|function-rst-describe_type|

.. _function-_at_rst_c__c_describe_type__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: describe_type(td: TypeDeclPtr)

describe_type returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+td      + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-rst-describe_type|

.. _function-_at_rst_c__c_describe_type_short__hh_const_hh__hh_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: describe_type_short(td: ast::TypeDecl? const)

describe_type_short returns string

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+td      + :ref:`ast::TypeDecl <handle-ast-TypeDecl>` ? const+
+--------+---------------------------------------------------+


|function-rst-describe_type_short|

.. _function-_at_rst_c__c_describe_type_short__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: describe_type_short(td: TypeDeclPtr)

describe_type_short returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+td      + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-rst-describe_type_short|

++++++++++++
Label makers
++++++++++++

  *  :ref:`alias_label_name (td:smart_ptr\<ast::TypeDecl\> const) : string <function-_at_rst_c__c_alias_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl>` 
  *  :ref:`enum_label_name (value:ast::Enumeration? const) : string <function-_at_rst_c__c_enum_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Enumeration>` 
  *  :ref:`enum_label_name (value:smart_ptr\<ast::Enumeration\> const) : string <function-_at_rst_c__c_enum_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Enumeration>` 
  *  :ref:`function_label_name (value:smart_ptr\<ast::Function\> const) : string <function-_at_rst_c__c_function_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function>` 
  *  :ref:`global_label_name (value:ast::Variable? const) : string <function-_at_rst_c__c_global_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Variable>` 
  *  :ref:`global_label_name (value:smart_ptr\<ast::Variable\> const) : string <function-_at_rst_c__c_global_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Variable>` 
  *  :ref:`struct_label_name (value:ast::Structure? const) : string <function-_at_rst_c__c_struct_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Structure>` 
  *  :ref:`struct_label_name (value:smart_ptr\<ast::Structure\> const) : string <function-_at_rst_c__c_struct_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Structure>` 
  *  :ref:`call_macro_label_name (mod:auto const;value:auto const) : auto <function-_at_rst_c__c_call_macro_label_name__hh_const_hh__hh_auto__hh_const_hh__hh_auto>` 
  *  :ref:`handle_label_name (value:auto const) : auto <function-_at_rst_c__c_handle_label_name__hh_const_hh__hh_auto>` 

.. _function-_at_rst_c__c_alias_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl:

.. das:function:: alias_label_name(td: TypeDeclPtr)

alias_label_name returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+td      + :ref:`TypeDeclPtr <alias-TypeDeclPtr>` +
+--------+----------------------------------------+


|function-rst-alias_label_name|

.. _function-_at_rst_c__c_enum_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Enumeration:

.. das:function:: enum_label_name(value: ast::Enumeration? const)

enum_label_name returns string

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+value   + :ref:`ast::Enumeration <handle-ast-Enumeration>` ? const+
+--------+---------------------------------------------------------+


|function-rst-enum_label_name|

.. _function-_at_rst_c__c_enum_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Enumeration:

.. das:function:: enum_label_name(value: EnumerationPtr)

enum_label_name returns string

+--------+----------------------------------------------+
+argument+argument type                                 +
+========+==============================================+
+value   + :ref:`EnumerationPtr <alias-EnumerationPtr>` +
+--------+----------------------------------------------+


|function-rst-enum_label_name|

.. _function-_at_rst_c__c_function_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function:

.. das:function:: function_label_name(value: FunctionPtr)

function_label_name returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+value   + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-rst-function_label_name|

.. _function-_at_rst_c__c_global_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Variable:

.. das:function:: global_label_name(value: ast::Variable? const)

global_label_name returns string

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+value   + :ref:`ast::Variable <handle-ast-Variable>` ? const+
+--------+---------------------------------------------------+


|function-rst-global_label_name|

.. _function-_at_rst_c__c_global_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Variable:

.. das:function:: global_label_name(value: VariablePtr)

global_label_name returns string

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+value   + :ref:`VariablePtr <alias-VariablePtr>` +
+--------+----------------------------------------+


|function-rst-global_label_name|

.. _function-_at_rst_c__c_struct_label_name__hh_const_hh__hh_ptr_hh__hh_handle_hh_Structure:

.. das:function:: struct_label_name(value: ast::Structure? const)

struct_label_name returns string

+--------+-----------------------------------------------------+
+argument+argument type                                        +
+========+=====================================================+
+value   + :ref:`ast::Structure <handle-ast-Structure>` ? const+
+--------+-----------------------------------------------------+


|function-rst-struct_label_name|

.. _function-_at_rst_c__c_struct_label_name__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Structure:

.. das:function:: struct_label_name(value: StructurePtr)

struct_label_name returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+value   + :ref:`StructurePtr <alias-StructurePtr>` +
+--------+------------------------------------------+


|function-rst-struct_label_name|

.. _function-_at_rst_c__c_call_macro_label_name__hh_const_hh__hh_auto__hh_const_hh__hh_auto:

.. das:function:: call_macro_label_name(mod: auto const; value: auto const)

call_macro_label_name returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+mod     +auto const   +
+--------+-------------+
+value   +auto const   +
+--------+-------------+


|function-rst-call_macro_label_name|

.. _function-_at_rst_c__c_handle_label_name__hh_const_hh__hh_auto:

.. das:function:: handle_label_name(value: auto const)

handle_label_name returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+value   +auto const   +
+--------+-------------+


|function-rst-handle_label_name|

++++++++++++++++++
RST section makers
++++++++++++++++++

  *  :ref:`make_domain (attr:string const) : string <function-_at_rst_c__c_make_domain__hh_const_hh_string>` 
  *  :ref:`make_group (name:string const;plus:string const) : string <function-_at_rst_c__c_make_group__hh_const_hh_string__hh_const_hh_string>` 
  *  :ref:`make_header (name:string const;lab:string const) : string <function-_at_rst_c__c_make_header__hh_const_hh_string__hh_const_hh_string>` 
  *  :ref:`make_label (name:string const) : string <function-_at_rst_c__c_make_label__hh_const_hh_string>` 
  *  :ref:`make_ref (name:string const;text:string const) : string <function-_at_rst_c__c_make_ref__hh_const_hh_string__hh_const_hh_string>` 
  *  :ref:`make_table (tab:array\<array\<string\>\> const;withHeader:bool const) : string <function-_at_rst_c__c_make_table__hh_const_hh__hh_array_hh__hh_array_hh_string__hh_const_hh_bool>` 

.. _function-_at_rst_c__c_make_domain__hh_const_hh_string:

.. das:function:: make_domain(attr: string const)

make_domain returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+attr    +string const +
+--------+-------------+


|function-rst-make_domain|

.. _function-_at_rst_c__c_make_group__hh_const_hh_string__hh_const_hh_string:

.. das:function:: make_group(name: string const; plus: string const)

make_group returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+
+plus    +string const +
+--------+-------------+


|function-rst-make_group|

.. _function-_at_rst_c__c_make_header__hh_const_hh_string__hh_const_hh_string:

.. das:function:: make_header(name: string const; lab: string const)

make_header returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+
+lab     +string const +
+--------+-------------+


|function-rst-make_header|

.. _function-_at_rst_c__c_make_label__hh_const_hh_string:

.. das:function:: make_label(name: string const)

make_label returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+


|function-rst-make_label|

.. _function-_at_rst_c__c_make_ref__hh_const_hh_string__hh_const_hh_string:

.. das:function:: make_ref(name: string const; text: string const)

make_ref returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+
+text    +string const +
+--------+-------------+


|function-rst-make_ref|

.. _function-_at_rst_c__c_make_table__hh_const_hh__hh_array_hh__hh_array_hh_string__hh_const_hh_bool:

.. das:function:: make_table(tab: array<array<string>> const; withHeader: bool const)

make_table returns string

+----------+--------------------------+
+argument  +argument type             +
+==========+==========================+
+tab       +array<array<string>> const+
+----------+--------------------------+
+withHeader+bool const                +
+----------+--------------------------+


|function-rst-make_table|

+++++++++++++++++++++
Documentation queries
+++++++++++++++++++++

  *  :ref:`function_needs_documenting (func:smart_ptr\<ast::Function\> const) : bool const <function-_at_rst_c__c_function_needs_documenting__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function>` 
  *  :ref:`argument_needs_documenting (tt:auto const) : auto <function-_at_rst_c__c_argument_needs_documenting__hh_const_hh__hh_auto>` 

.. _function-_at_rst_c__c_function_needs_documenting__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Function:

.. das:function:: function_needs_documenting(func: FunctionPtr)

function_needs_documenting returns bool const

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>` +
+--------+----------------------------------------+


|function-rst-function_needs_documenting|

.. _function-_at_rst_c__c_argument_needs_documenting__hh_const_hh__hh_auto:

.. das:function:: argument_needs_documenting(tt: auto const)

argument_needs_documenting returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+tt      +auto const   +
+--------+-------------+


|function-rst-argument_needs_documenting|

++++++++++++++++
Group operations
++++++++++++++++

  *  :ref:`append_to_group_by_regex (group:rst::DocGroup -const;mod:rtti::Module? const;reg:regex::Regex -const) : void <function-_at_rst_c__c_append_to_group_by_regex_DocGroup__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module_Regex>` 
  *  :ref:`debug_group (group:rst::DocGroup -const) : rst::DocGroup <function-_at_rst_c__c_debug_group_DocGroup>` 
  *  :ref:`group_by_regex (name:string const;mod:rtti::Module? const;reg:regex::Regex -const) : rst::DocGroup <function-_at_rst_c__c_group_by_regex__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module_Regex>` 
  *  :ref:`hide_group (group:rst::DocGroup -const) : rst::DocGroup <function-_at_rst_c__c_hide_group_DocGroup>` 

.. _function-_at_rst_c__c_append_to_group_by_regex_DocGroup__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module_Regex:

.. das:function:: append_to_group_by_regex(group: DocGroup; mod: rtti::Module? const; reg: Regex)

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+group   + :ref:`rst::DocGroup <struct-rst-DocGroup>`      +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+reg     + :ref:`regex::Regex <struct-regex-Regex>`        +
+--------+-------------------------------------------------+


|function-rst-append_to_group_by_regex|

.. _function-_at_rst_c__c_debug_group_DocGroup:

.. das:function:: debug_group(group: DocGroup)

debug_group returns  :ref:`rst::DocGroup <struct-rst-DocGroup>` 

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+group   + :ref:`rst::DocGroup <struct-rst-DocGroup>` +
+--------+--------------------------------------------+


|function-rst-debug_group|

.. _function-_at_rst_c__c_group_by_regex__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module_Regex:

.. das:function:: group_by_regex(name: string const; mod: rtti::Module? const; reg: Regex)

group_by_regex returns  :ref:`rst::DocGroup <struct-rst-DocGroup>` 

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+name    +string const                                     +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+reg     + :ref:`regex::Regex <struct-regex-Regex>`        +
+--------+-------------------------------------------------+


|function-rst-group_by_regex|

.. _function-_at_rst_c__c_hide_group_DocGroup:

.. das:function:: hide_group(group: DocGroup)

hide_group returns  :ref:`rst::DocGroup <struct-rst-DocGroup>` 

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+group   + :ref:`rst::DocGroup <struct-rst-DocGroup>` +
+--------+--------------------------------------------+


|function-rst-hide_group|

++++++++++++++
Naming helpers
++++++++++++++

  *  :ref:`function_name (name:string const) : string <function-_at_rst_c__c_function_name__hh_const_hh_string>` 
  *  :ref:`topic (mod:rtti::Module? const) : string <function-_at_rst_c__c_topic__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module>` 
  *  :ref:`topic (category:string const;mod:rtti::Module? const;name:string const implicit) : string <function-_at_rst_c__c_topic__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string>` 
  *  :ref:`module_name (mod:auto const) : auto <function-_at_rst_c__c_module_name__hh_const_hh__hh_auto>` 

.. _function-_at_rst_c__c_function_name__hh_const_hh_string:

.. das:function:: function_name(name: string const)

function_name returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+


|function-rst-function_name|

.. _function-_at_rst_c__c_topic__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module:

.. das:function:: topic(mod: rtti::Module? const)

topic returns string

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+


|function-rst-topic|

.. _function-_at_rst_c__c_topic__hh_const_hh_string__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh__hh_implicit_hh_string:

.. das:function:: topic(category: string const; mod: rtti::Module? const; name: string const implicit)

topic returns string

+--------+-------------------------------------------------+
+argument+argument type                                    +
+========+=================================================+
+category+string const                                     +
+--------+-------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const+
+--------+-------------------------------------------------+
+name    +string const implicit                            +
+--------+-------------------------------------------------+


|function-rst-topic|

.. _function-_at_rst_c__c_module_name__hh_const_hh__hh_auto:

.. das:function:: module_name(mod: auto const)

module_name returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+mod     +auto const   +
+--------+-------------+


|function-rst-module_name|


