
.. _stdlib_templates_boost:

============================
Template application helpers
============================

The templates boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_templates>`.

All functions and symbols are in "templates_boost" module, use require to get access to it. ::

    require daslib/templates_boost


++++++++++
Structures
++++++++++

.. _struct-templates_boost-Template:

.. das:attribute:: Template

This structure contains collection of subsitution rules for a template.

:Fields: * **kaboomVar** : table<string;tuple<prefix:string;suffix:string>>

         * **call2name** : table<string;string>

         * **field2name** : table<string;string>

         * **var2name** : table<string;string>

         * **var2expr** : table<string;smart_ptr< :ref:`Expression <handle-ast-Expression>` >>

         * **var2exprList** : table<string;array<smart_ptr< :ref:`Expression <handle-ast-Expression>` >>>

         * **type2type** : table<string;string>

         * **type2etype** : table<string; :ref:`TypeDeclPtr <alias-TypeDeclPtr>` >

         * **blockArgName** : table<string;string>

         * **annArg** : table<string;lambda<(ann: :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>>

         * **blkArg** : table<string;array< :ref:`VariablePtr <alias-VariablePtr>` >>

         * **tag2expr** : table<string;smart_ptr< :ref:`Expression <handle-ast-Expression>` >>

This structure contains collection of subsitution rules for a template.

+++++++++++
Call macros
+++++++++++

.. _call-macro-templates_boost-qmacro_expr:

.. das:attribute:: qmacro_expr

|detail/function_annotation-templates_boost-qmacro_expr|

.. _call-macro-templates_boost-qmacro_variable:

.. das:attribute:: qmacro_variable

|detail/function_annotation-templates_boost-qmacro_variable|

.. _call-macro-templates_boost-qmacro_block_to_array:

.. das:attribute:: qmacro_block_to_array

|detail/function_annotation-templates_boost-qmacro_block_to_array|

.. _call-macro-templates_boost-qmacro_function:

.. das:attribute:: qmacro_function

|detail/function_annotation-templates_boost-qmacro_function|

.. _call-macro-templates_boost-qmacro:

.. das:attribute:: qmacro

|detail/function_annotation-templates_boost-qmacro|

.. _call-macro-templates_boost-qmacro_method:

.. das:attribute:: qmacro_method

This macro implements expression reification for class methods.

.. _call-macro-templates_boost-qmacro_block:

.. das:attribute:: qmacro_block

|detail/function_annotation-templates_boost-qmacro_block|

.. _call-macro-templates_boost-qmacro_type:

.. das:attribute:: qmacro_type

|detail/function_annotation-templates_boost-qmacro_type|

++++++++++++++
Template rules
++++++++++++++

  *  :ref:`kaboomVarField (var self: Template; name: string; prefix: string; suffix: string) <function-_at_templates_boost_c__c_kaboomVarField_S_ls_templates_boost_c__c_Template_gr__Cs_Cs_Cs>` 
  *  :ref:`replaceVariable (var self: Template; name: string; var expr: smart_ptr\<Expression\>) <function-_at_templates_boost_c__c_replaceVariable_S_ls_templates_boost_c__c_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`renameVariable (var self: Template; name: string; newName: string) <function-_at_templates_boost_c__c_renameVariable_S_ls_templates_boost_c__c_Template_gr__Cs_Cs>` 
  *  :ref:`renameVariable (var self: Template; name: string; newName: das_string) <function-_at_templates_boost_c__c_renameVariable_S_ls_templates_boost_c__c_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`replaceType (var self: Template; name: string; newName: string) <function-_at_templates_boost_c__c_replaceType_S_ls_templates_boost_c__c_Template_gr__Cs_Cs>` 
  *  :ref:`replaceAnnotationArgument (var self: Template; name: string; var cb: lambda\<(var ann:AnnotationDeclaration):void\>) <function-_at_templates_boost_c__c_replaceAnnotationArgument_S_ls_templates_boost_c__c_Template_gr__Cs_N_ls_ann_gr_0_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr_1_ls_v_gr__at_>` 
  *  :ref:`replaceBlockArgument (var self: Template; name: string; newName: string) <function-_at_templates_boost_c__c_replaceBlockArgument_S_ls_templates_boost_c__c_Template_gr__Cs_Cs>` 

.. _function-_at_templates_boost_c__c_kaboomVarField_S_ls_templates_boost_c__c_Template_gr__Cs_Cs_Cs:

.. das:function:: kaboomVarField(self: Template; name: string; prefix: string; suffix: string)

Adds a rule to to the template to replace a variable field access with a prefix and suffix.
I.e. foo.bar into prefix + bar + suffix

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **prefix** : string

            * **suffix** : string

.. _function-_at_templates_boost_c__c_replaceVariable_S_ls_templates_boost_c__c_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: replaceVariable(self: Template; name: string; expr: smart_ptr<Expression>)

Adds a rule to the template to replace a variable with an expression.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-_at_templates_boost_c__c_renameVariable_S_ls_templates_boost_c__c_Template_gr__Cs_Cs:

.. das:function:: renameVariable(self: Template; name: string; newName: string)

Adds a rule to the template to rename a variable.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** : string

.. _function-_at_templates_boost_c__c_renameVariable_S_ls_templates_boost_c__c_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameVariable(self: Template; name: string; newName: das_string)

Adds a rule to the template to rename a variable.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-_at_templates_boost_c__c_replaceType_S_ls_templates_boost_c__c_Template_gr__Cs_Cs:

.. das:function:: replaceType(self: Template; name: string; newName: string)

Adds a rule to the template to replace a type alias with another type alias, specified by name.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** : string

.. _function-_at_templates_boost_c__c_replaceAnnotationArgument_S_ls_templates_boost_c__c_Template_gr__Cs_N_ls_ann_gr_0_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr_1_ls_v_gr__at_:

.. das:function:: replaceAnnotationArgument(self: Template; name: string; cb: lambda<(var ann:AnnotationDeclaration):void>)

Adds a rule to the template to replace an annotation argument with the result of a callback.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **cb** : lambda<(ann: :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>

.. _function-_at_templates_boost_c__c_replaceBlockArgument_S_ls_templates_boost_c__c_Template_gr__Cs_Cs:

.. das:function:: replaceBlockArgument(self: Template; name: string; newName: string)

Adds a rule to the template to rename a block argument.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** : string

++++++++++++++++++++
Template application
++++++++++++++++++++

  *  :ref:`apply_template (var rules: Template; at: LineInfo; var expr: smart_ptr\<Expression\>; forceAt: bool = true) : ExpressionPtr <function-_at_templates_boost_c__c_apply_template_S_ls_templates_boost_c__c_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_Cb>` 
  *  :ref:`apply_template (at: LineInfo; var expr: smart_ptr\<Expression\>&; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__&1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`apply_template (var expr: smart_ptr\<Expression\>&; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-_at_templates_boost_c__c_apply_template_&1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at_templates_boost_c__c_apply_template_S_ls_templates_boost_c__c_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_Cb:

.. das:function:: apply_template(rules: Template; at: LineInfo; expr: smart_ptr<Expression>; forceAt: bool = true) : ExpressionPtr

Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.

:Arguments: * **rules** :  :ref:`Template <struct-templates_boost-Template>` 

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **forceAt** : bool

.. _function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__&1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(at: LineInfo; expr: smart_ptr<Expression>&; blk: block<(var rules:Template):void>) : ExpressionPtr

|detail/function-templates_boost-apply_template-0x6ac76ec254f15fa0|

:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >&

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_template_&1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(expr: smart_ptr<Expression>&; blk: block<(var rules:Template):void>) : ExpressionPtr

|detail/function-templates_boost-apply_template-0x6d4d0b9515bf6e9b|

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >&

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

++++++++++++++++++
Expression helpers
++++++++++++++++++

  *  :ref:`remove_deref (varname: string; var expr: smart_ptr\<Expression\>) <function-_at_templates_boost_c__c_remove_deref_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 

.. _function-_at_templates_boost_c__c_remove_deref_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: remove_deref(varname: string; expr: smart_ptr<Expression>)

Removes dereferences of the variable `varname` from the expression.
This is typically used when replacing 'workhorse' variable with constant.

:Arguments: * **varname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

+++++++++++++
Block helpers
+++++++++++++

  *  :ref:`unquote_block (expr: ExpressionPtr) : smart_ptr\<ExprBlock\> <function-_at_templates_boost_c__c_unquote_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`move_unquote_block (var expr: ExpressionPtr) : smart_ptr\<ExprBlock\> <function-_at_templates_boost_c__c_move_unquote_block_&Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 

.. _function-_at_templates_boost_c__c_unquote_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: unquote_block(expr: ExpressionPtr) : smart_ptr<ExprBlock>

Returns the corresponding block subexpression expression form the ExprMakeBlock.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_move_unquote_block_&Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: move_unquote_block(expr: ExpressionPtr) : smart_ptr<ExprBlock>

Moves the corresponding block subexpression expression form the ExprMakeBlock.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+++++++++++++++++++++++
Global variable helpers
+++++++++++++++++++++++

  *  :ref:`add_global_var (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`add_global_var (mod: Module?; vname: string; var typ: TypeDeclPtr; vat: LineInfo; priv: bool; blk: block\<(var v:smart_ptr\<Variable\>):void\>) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cb_CN_ls_v_gr_0_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`add_global_var (mod: Module?; vname: string; var typ: TypeDeclPtr; vat: LineInfo; priv: bool) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cb>` 
  *  :ref:`add_global_let (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-_at_templates_boost_c__c_add_global_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`add_global_private_var (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-_at_templates_boost_c__c_add_global_private_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 
  *  :ref:`add_global_private_let (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-_at_templates_boost_c__c_add_global_private_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M>` 

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: add_global_var(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Adds global variable to the module, given name and initial value.
Global variables type is would be inferred from the initial value.
`priv` specifies if the variable is private to the block.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cb_CN_ls_v_gr_0_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_gr_1_ls_v_gr__builtin_:

.. das:function:: add_global_var(mod: Module?; vname: string; typ: TypeDeclPtr; vat: LineInfo; priv: bool; blk: block<(var v:smart_ptr<Variable>):void>) : bool

|detail/function-templates_boost-add_global_var-0xe5ca7576a1221942|

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **priv** : bool

            * **blk** : block<(v: :ref:`VariablePtr <alias-VariablePtr>` ):void>

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr__Cb:

.. das:function:: add_global_var(mod: Module?; vname: string; typ: TypeDeclPtr; vat: LineInfo; priv: bool) : bool

|detail/function-templates_boost-add_global_var-0x536e00496abe066|

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **priv** : bool

.. _function-_at_templates_boost_c__c_add_global_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: add_global_let(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
Variable type will be constant.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_add_global_private_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: add_global_private_var(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
It will be private.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_add_global_private_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm__Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: add_global_private_let(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
It will be private, and type will be constant.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+++++++++++++
Hygenic names
+++++++++++++

  *  :ref:`make_unique_private_name (prefix: string; vat: LineInfo) : string <function-_at_templates_boost_c__c_make_unique_private_name_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 

.. _function-_at_templates_boost_c__c_make_unique_private_name_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: make_unique_private_name(prefix: string; vat: LineInfo) : string

Generates unique private name for the variable, given prefix and line info.

.. warning::
The assumption is that line info is unique for the context of the unique name generation.
If it is not, additional measures must be taken to ensure uniqueness of prefix.

:Arguments: * **prefix** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_templates_boost_c__c_replaceVarTag_S_ls_templates_boost_c__c_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: replaceVarTag(self: Template; name: string; expr: smart_ptr<Expression>)

Adds a rule to the template to replace a variable tag with an expression.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-_at_templates_boost_c__c_replaceArgumentWithList_S_ls_templates_boost_c__c_Template_gr__Cs_C1_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_gr_A:

.. das:function:: replaceArgumentWithList(self: Template; name: string; blka: array<smart_ptr<Variable>>)

Adds a rule to the template to replace a block argument with a list of variables.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **blka** : array< :ref:`VariablePtr <alias-VariablePtr>` >

.. _function-_at_templates_boost_c__c_replaceVariableWithList_S_ls_templates_boost_c__c_Template_gr__Cs_C1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_gr_A:

.. das:function:: replaceVariableWithList(self: Template; name: string; expr: array<smart_ptr<Expression>>)

Adds a rule to the template to replace a variable with an expression list.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **expr** : array< :ref:`ExpressionPtr <alias-ExpressionPtr>` >

.. _function-_at_templates_boost_c__c_replaceVariableWithList_S_ls_templates_boost_c__c_Template_gr__Cs_CH_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr_:

.. das:function:: replaceVariableWithList(self: Template; name: string; expr: dasvector`smart_ptr`Expression)

Adds a rule to the template to replace a variable with an expression list.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **expr** : vector<smart_ptr<Expression>>

.. _function-_at_templates_boost_c__c_renameField_S_ls_templates_boost_c__c_Template_gr__Cs_Cs:

.. das:function:: renameField(self: Template; name: string; newName: string)

Adds a rule to the template to rename any field lookup (., ?., as, is, etc)

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** : string

.. _function-_at_templates_boost_c__c_renameField_S_ls_templates_boost_c__c_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameField(self: Template; name: string; newName: das_string)

Adds a rule to the template to rename any field lookup (., ?., as, is, etc)

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-_at_templates_boost_c__c_replaceTypeWithTypeDecl_S_ls_templates_boost_c__c_Template_gr__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M:

.. das:function:: replaceTypeWithTypeDecl(self: Template; name: string; expr: TypeDeclPtr)

Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **expr** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

.. _function-_at_templates_boost_c__c_renameCall_S_ls_templates_boost_c__c_Template_gr__Cs_Cs:

.. das:function:: renameCall(self: Template; name: string; newName: string)

Adds a rule to the template to rename a call.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** : string

.. _function-_at_templates_boost_c__c_renameCall_S_ls_templates_boost_c__c_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameCall(self: Template; name: string; newName: das_string)

Adds a rule to the template to rename a call.

:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>` 

            * **name** : string

            * **newName** :  :ref:`das_string <handle-builtin-das_string>` 

.. _function-_at_templates_boost_c__c_make_expression_block_1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_gr_A:

.. das:function:: make_expression_block(exprs: array<smart_ptr<Expression>>) : smart_ptr<ExprBlock>

Create ExprBlock and move all expressions from expr to the list of the block.

:Arguments: * **exprs** : array< :ref:`ExpressionPtr <alias-ExpressionPtr>` >

.. _function-_at_templates_boost_c__c_make_expression_block_H_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr_:

.. das:function:: make_expression_block(exprs: dasvector`smart_ptr`Expression) : smart_ptr<ExprBlock>

Create ExprBlock and move all expressions from expr to the list of the block.

:Arguments: * **exprs** : vector<smart_ptr<Expression>>

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(a: TypeDeclPtr; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **a** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(st: StructurePtr; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(st: Structure?; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **st** :  :ref:`Structure <handle-ast-Structure>` ?

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_Y_ls_EnumerationPtr_gr_1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm_M_CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(st: EnumerationPtr; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **st** :  :ref:`EnumerationPtr <alias-EnumerationPtr>` 

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_1_ls_H_ls_ast_c__c_Enumeration_gr__gr__qm__CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(st: Enumeration?; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **st** :  :ref:`Enumeration <handle-ast-Enumeration>` ?

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

.. _function-_at_templates_boost_c__c_apply_qmacro_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reificaiton. This is a generat expression reification.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qblock_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qblock(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reificaiton. This is a block reification.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qblock_to_array_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qblock_to_array(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : array<smart_ptr<Expression>>

Implementation details for the expression reificaiton. This is a block reification.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qblock_expr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qblock_expr(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reificaiton. This is a frist line of the block as expression reification.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qtype_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qtype(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : TypeDeclPtr

Implementation details for the expression reificaiton. This is a type declaration reification.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_expression_at_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: expression_at(expr: ExpressionPtr; at: LineInfo) : ExpressionPtr

Force expression location, than return it.

:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_templates_boost_c__c_emplace_new_1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_gr_A_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: emplace_new(arr: array<smart_ptr<Expression>>; expr: ExpressionPtr)

Unifies emplace and emplace_new for the array<ExpressionPtr>

:Arguments: * **arr** : array< :ref:`ExpressionPtr <alias-ExpressionPtr>` >

            * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_emplace_new_1_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M_gr_A_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr__qm_M:

.. das:function:: emplace_new(arr: array<smart_ptr<Variable>>; expr: VariablePtr)

Unifies emplace and emplace_new for the array<VariablePtr>

:Arguments: * **arr** : array< :ref:`VariablePtr <alias-VariablePtr>` >

            * **expr** :  :ref:`VariablePtr <alias-VariablePtr>` 

.. _function-_at_templates_boost_c__c_apply_qmacro_function_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro_function(fname: string; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : FunctionPtr

Implementation details for reification. This is a function generation reification.

:Arguments: * **fname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qmacro_method_Cs_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro_method(fname: string; parent: StructurePtr; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : FunctionPtr

Implementation details for reification. This is a class method function generation reification.

:Arguments: * **fname** : string

            * **parent** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_apply_qmacro_variable_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M_CN_ls_rules_gr_0_ls_S_ls_templates_boost_c__c_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro_variable(vname: string; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : VariablePtr

Implementation details for reification. This is a variable generation reification.

:Arguments: * **vname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>` ):void>

.. _function-_at_templates_boost_c__c_add_structure_field_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr__qm_M_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr__qm_M:

.. das:function:: add_structure_field(cls: StructurePtr; name: string; t: TypeDeclPtr; init: ExpressionPtr) : int

Adds a field to the structure.

:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **name** : string

            * **t** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

            * **init** :  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

.. _function-_at_templates_boost_c__c_make_class_Cs_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_:

.. das:function:: make_class(name: string; mod: Module?) : smart_ptr<Structure>

Creates a class structure. Adds __rtti, __finalize fields.

:Arguments: * **name** : string

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

.. _function-_at_templates_boost_c__c_make_class_Cs_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_:

.. das:function:: make_class(name: string; baseClass: StructurePtr; mod: Module?) : smart_ptr<Structure>

|detail/function-templates_boost-make_class-0x78215a2572a890c0|

:Arguments: * **name** : string

            * **baseClass** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

.. _function-_at_templates_boost_c__c_make_class_Cs_C1_ls_H_ls_ast_c__c_Structure_gr__gr__qm__C1_ls_H_ls_rtti_c__c_Module_gr__gr__qm_:

.. das:function:: make_class(name: string; baseClass: Structure?; mod: Module?) : smart_ptr<Structure>

|detail/function-templates_boost-make_class-0x7282121947310544|

:Arguments: * **name** : string

            * **baseClass** :  :ref:`Structure <handle-ast-Structure>` ?

            * **mod** :  :ref:`Module <handle-rtti-Module>` ?

.. _function-_at_templates_boost_c__c_make_class_constructor_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M:

.. das:function:: make_class_constructor(cls: StructurePtr; ctor: FunctionPtr) : smart_ptr<Function>

Adds a class constructor from a constructor function.

:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **ctor** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

.. _function-_at_templates_boost_c__c_modify_to_class_member_CY_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr__qm_M_CY_ls_FunctionPtr_gr_1_ls_H_ls_ast_c__c_Function_gr__gr__qm_M_Cb_Cb:

.. das:function:: modify_to_class_member(cls: StructurePtr; fun: FunctionPtr; isExplicit: bool; Constant: bool)

Modifies function to be a member of a particular class.

:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>` 

            * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>` 

            * **isExplicit** : bool

            * **Constant** : bool

.. _function-_at_templates_boost_c__c_add_array_ptr_ref_1_ls_1_ls_Y_ls_TT_gr__dot__gr__qm_M_gr_A:

.. das:function:: add_array_ptr_ref(a: array<smart_ptr<auto(TT)>>) : array<smart_ptr<TT>>

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **a** : array<smart_ptr<auto(TT)>>

.. _function-_at_templates_boost_c__c_enum_class_type_C_dot_:

.. das:function:: enum_class_type(st: auto) : auto

|detail/function-templates_boost-enum_class_type-0x65e9ff847d38d510|

:Arguments: * **st** : auto

.. _function-_at_templates_boost_c__c_add_type_ptr_ref_CY_ls_TT_gr__dot__CY_ls_TypeDeclFlags_gr_N_ls_ref;constant;temporary;_implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr;smartPtrNative;isExplicit;isNativeDim;isTag;explicitRef;isPrivateAlias;autoToAlias_gr_t:

.. das:function:: add_type_ptr_ref(anything: auto(TT); flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. This adds any array to the rules.

:Arguments: * **anything** : auto(TT)

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 


