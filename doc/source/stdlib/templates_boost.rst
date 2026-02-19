
.. _stdlib_templates_boost:

============================
Template application helpers
============================

.. das:module:: templates_boost

The TEMPLATES_BOOST module extends template utilities with high-level macros
for common code generation patterns, including template function generation,
type-parameterized struct creation, and compile-time code expansion.

See also :doc:`templates` for ``decltype`` and ``[template]`` annotations.

All functions and symbols are in "templates_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/templates_boost



++++++++++
Structures
++++++++++

.. _struct-templates_boost-Template:

.. das:attribute:: Template

This structure contains collection of substitution rules for a template.

:Fields: * **kaboomVar** : table<string;tuple<prefix:string;suffix:string>> - variable field access replacement rules

         * **call2name** : table<string;string> - call name replacement rules

         * **field2name** : table<string;string> - field name replacement rules

         * **var2name** : table<string;string> - variable name replacement rules

         * **var2expr** : table<string;smart_ptr< :ref:`Expression <handle-ast-Expression>`>> - variable expression replacement rules

         * **var2exprList** : table<string;array<smart_ptr< :ref:`Expression <handle-ast-Expression>`>>> - variable expression list replacement rules

         * **type2type** : table<string;string> - type name replacement rules

         * **type2etype** : table<string; :ref:`TypeDeclPtr <alias-TypeDeclPtr>`> - type to type declaration replacement rules

         * **struct2etype** : table< :ref:`Structure <handle-ast-Structure>`?; :ref:`TypeDeclPtr <alias-TypeDeclPtr>`> - structure to type declaration replacement rules

         * **blockArgName** : table<string;string> - block argument name replacement rules

         * **annArg** : table<string;lambda<(ann: :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`):void>> - annotation argument replacement rules

         * **blkArg** : table<string;array< :ref:`VariablePtr <alias-VariablePtr>`>> - block argument replacement rules

         * **tag2expr** : table<string;smart_ptr< :ref:`Expression <handle-ast-Expression>`>> - tag expression replacement rules




+++++++++++
Call macros
+++++++++++

.. _call-macro-templates_boost-qmacro_variable:

.. das:attribute:: qmacro_variable

This macro implements expression reification for variables.


.. _call-macro-templates_boost-qmacro_expr:

.. das:attribute:: qmacro_expr

This macro implements first line of the expression block reification 'qmacro_expr'


.. _call-macro-templates_boost-qmacro_function:

.. das:attribute:: qmacro_function

This macro implements expression reification for functions.


.. _call-macro-templates_boost-qmacro_block_to_array:

.. das:attribute:: qmacro_block_to_array

This macro implements expression block to array reification 'qmacro_block_to_array'


.. _call-macro-templates_boost-qmacro_template_class:

.. das:attribute:: qmacro_template_class

Call macro for quoting named template class methods.
This macro implements expression reification for the named expressions (function, variable, etc.)


.. _call-macro-templates_boost-qmacro_method:

.. das:attribute:: qmacro_method

This macro implements expression reification for class methods.


.. _call-macro-templates_boost-qmacro_template_function:

.. das:attribute:: qmacro_template_function

Call macro for quoting named template functions.
This macro implements expression reification for the named expressions (function, variable, etc.)


.. _call-macro-templates_boost-qmacro:

.. das:attribute:: qmacro

This macro implements expression reification 'qmacro'


.. _call-macro-templates_boost-qmacro_block:

.. das:attribute:: qmacro_block

This macro implements expression block reification 'qmacro_block'


.. _call-macro-templates_boost-qmacro_type:

.. das:attribute:: qmacro_type

This macro implements type declaration reification 'qmacro_type'



++++++++++++++
Template rules
++++++++++++++

  *  :ref:`kaboomVarField (var self: Template; name: string; prefix: string; suffix: string) <function-templates_boost_kaboomVarField_Template_string_string_string>`
  *  :ref:`renameCall (var self: Template; name: string; newName: string) <function-templates_boost_renameCall_Template_string_string>`
  *  :ref:`renameCall (var self: Template; name: string; newName: das_string) <function-templates_boost_renameCall_Template_string_das_string>`
  *  :ref:`renameField (var self: Template; name: string; newName: string) <function-templates_boost_renameField_Template_string_string>`
  *  :ref:`renameField (var self: Template; name: string; newName: das_string) <function-templates_boost_renameField_Template_string_das_string>`
  *  :ref:`renameVariable (var self: Template; name: string; newName: string) <function-templates_boost_renameVariable_Template_string_string>`
  *  :ref:`renameVariable (var self: Template; name: string; newName: das_string) <function-templates_boost_renameVariable_Template_string_das_string>`
  *  :ref:`replaceAnnotationArgument (var self: Template; name: string; var cb: lambda\<(var ann:AnnotationDeclaration):void\>) <function-templates_boost_replaceAnnotationArgument_Template_string_lambda_ls_var_ann_c_AnnotationDeclaration_c_void_gr_>`
  *  :ref:`replaceArgumentWithList (var self: Template; name: string; blka: array\<VariablePtr\>) <function-templates_boost_replaceArgumentWithList_Template_string_array_ls_VariablePtr_gr_>`
  *  :ref:`replaceBlockArgument (var self: Template; name: string; newName: string) <function-templates_boost_replaceBlockArgument_Template_string_string>`
  *  :ref:`replaceStructWithTypeDecl (var self: Template; pstruct: Structure?; var expr: TypeDeclPtr) <function-templates_boost_replaceStructWithTypeDecl_Template_Structure_q__TypeDeclPtr>`
  *  :ref:`replaceType (var self: Template; name: string; newName: string) <function-templates_boost_replaceType_Template_string_string>`
  *  :ref:`replaceTypeWithTypeDecl (var self: Template; name: string; var expr: TypeDeclPtr) <function-templates_boost_replaceTypeWithTypeDecl_Template_string_TypeDeclPtr>`
  *  :ref:`replaceVarTag (var self: Template; name: string; var expr: smart_ptr\<Expression\>) <function-templates_boost_replaceVarTag_Template_string_smart_ptr_ls_Expression_gr_>`
  *  :ref:`replaceVariable (var self: Template; name: string; var expr: smart_ptr\<Expression\>) <function-templates_boost_replaceVariable_Template_string_smart_ptr_ls_Expression_gr_>`
  *  :ref:`replaceVariableWithList (var self: Template; name: string; expr: dasvector`smart_ptr`Expression) <function-templates_boost_replaceVariableWithList_Template_string_dasvector_rq_smart_ptr_rq_Expression>`
  *  :ref:`replaceVariableWithList (var self: Template; name: string; expr: array\<ExpressionPtr\>) <function-templates_boost_replaceVariableWithList_Template_string_array_ls_ExpressionPtr_gr_>`

.. _function-templates_boost_kaboomVarField_Template_string_string_string:

.. das:function:: kaboomVarField(self: Template; name: string; prefix: string; suffix: string)

Adds a rule to to the template to replace a variable field access with a prefix and suffix.
I.e. foo.bar into prefix + bar + suffix


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **prefix** : string

            * **suffix** : string


renameCall
^^^^^^^^^^

.. _function-templates_boost_renameCall_Template_string_string:

.. das:function:: renameCall(self: Template; name: string; newName: string)

Adds a rule to the template to rename a call.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **newName** : string

.. _function-templates_boost_renameCall_Template_string_das_string:

.. das:function:: renameCall(self: Template; name: string; newName: das_string)

----


renameField
^^^^^^^^^^^

.. _function-templates_boost_renameField_Template_string_string:

.. das:function:: renameField(self: Template; name: string; newName: string)

Adds a rule to the template to rename any field lookup (., ?., as, is, etc)


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **newName** : string

.. _function-templates_boost_renameField_Template_string_das_string:

.. das:function:: renameField(self: Template; name: string; newName: das_string)

----


renameVariable
^^^^^^^^^^^^^^

.. _function-templates_boost_renameVariable_Template_string_string:

.. das:function:: renameVariable(self: Template; name: string; newName: string)

Adds a rule to the template to rename a variable.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **newName** : string

.. _function-templates_boost_renameVariable_Template_string_das_string:

.. das:function:: renameVariable(self: Template; name: string; newName: das_string)

----

.. _function-templates_boost_replaceAnnotationArgument_Template_string_lambda_ls_var_ann_c_AnnotationDeclaration_c_void_gr_:

.. das:function:: replaceAnnotationArgument(self: Template; name: string; cb: lambda<(var ann:AnnotationDeclaration):void>)

Adds a rule to the template to replace an annotation argument with the result of a callback.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **cb** : lambda<(ann: :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>`):void>

.. _function-templates_boost_replaceArgumentWithList_Template_string_array_ls_VariablePtr_gr_:

.. das:function:: replaceArgumentWithList(self: Template; name: string; blka: array<VariablePtr>)

Adds a rule to the template to replace a block argument with a list of variables.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **blka** : array< :ref:`VariablePtr <alias-VariablePtr>`>

.. _function-templates_boost_replaceBlockArgument_Template_string_string:

.. das:function:: replaceBlockArgument(self: Template; name: string; newName: string)

Adds a rule to the template to rename a block argument.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **newName** : string

.. _function-templates_boost_replaceStructWithTypeDecl_Template_Structure_q__TypeDeclPtr:

.. das:function:: replaceStructWithTypeDecl(self: Template; pstruct: Structure?; expr: TypeDeclPtr)

Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **pstruct** :  :ref:`Structure <handle-ast-Structure>`?

            * **expr** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

.. _function-templates_boost_replaceType_Template_string_string:

.. das:function:: replaceType(self: Template; name: string; newName: string)

Adds a rule to the template to replace a type alias with another type alias, specified by name.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **newName** : string

.. _function-templates_boost_replaceTypeWithTypeDecl_Template_string_TypeDeclPtr:

.. das:function:: replaceTypeWithTypeDecl(self: Template; name: string; expr: TypeDeclPtr)

Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **expr** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

.. _function-templates_boost_replaceVarTag_Template_string_smart_ptr_ls_Expression_gr_:

.. das:function:: replaceVarTag(self: Template; name: string; expr: smart_ptr<Expression>)

Adds a rule to the template to replace a variable tag with an expression.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

.. _function-templates_boost_replaceVariable_Template_string_smart_ptr_ls_Expression_gr_:

.. das:function:: replaceVariable(self: Template; name: string; expr: smart_ptr<Expression>)

Adds a rule to the template to replace a variable with an expression.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>


replaceVariableWithList
^^^^^^^^^^^^^^^^^^^^^^^

.. _function-templates_boost_replaceVariableWithList_Template_string_dasvector_rq_smart_ptr_rq_Expression:

.. das:function:: replaceVariableWithList(self: Template; name: string; expr: dasvector`smart_ptr`Expression)

Adds a rule to the template to replace a variable with an expression list.


:Arguments: * **self** :  :ref:`Template <struct-templates_boost-Template>`

            * **name** : string

            * **expr** : vector<smart_ptr<Expression>>

.. _function-templates_boost_replaceVariableWithList_Template_string_array_ls_ExpressionPtr_gr_:

.. das:function:: replaceVariableWithList(self: Template; name: string; expr: array<ExpressionPtr>)


++++++++++++++++++++
Template application
++++++++++++++++++++

  *  :ref:`apply_template (var rules: Template; at: LineInfo; var typ: smart_ptr\<TypeDecl\>; forceAt: bool = true) : TypeDeclPtr <function-templates_boost_apply_template_Template_LineInfo_smart_ptr_ls_TypeDecl_gr__bool>`
  *  :ref:`apply_template (var rules: Template; at: LineInfo; var expr: smart_ptr\<Expression\>; forceAt: bool = true) : ExpressionPtr <function-templates_boost_apply_template_Template_LineInfo_smart_ptr_ls_Expression_gr__bool>`
  *  :ref:`apply_template (at: LineInfo; var expr: smart_ptr\<Expression\>&; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-templates_boost_apply_template_LineInfo_smart_ptr_ls_Expression_gr__ref__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_template (at: LineInfo; var typ: smart_ptr\<TypeDecl\>&; blk: block\<(var rules:Template):void\>) : TypeDeclPtr <function-templates_boost_apply_template_LineInfo_smart_ptr_ls_TypeDecl_gr__ref__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_template (var expr: smart_ptr\<Expression\>&; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-templates_boost_apply_template_smart_ptr_ls_Expression_gr__ref__block_ls_var_rules_c_Template_c_void_gr_>`


apply_template
^^^^^^^^^^^^^^

.. _function-templates_boost_apply_template_Template_LineInfo_smart_ptr_ls_TypeDecl_gr__bool:

.. das:function:: apply_template(rules: Template; at: LineInfo; typ: smart_ptr<TypeDecl>; forceAt: bool = true) : TypeDeclPtr

Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.


:Arguments: * **rules** :  :ref:`Template <struct-templates_boost-Template>`

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **typ** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>`>

            * **forceAt** : bool

.. _function-templates_boost_apply_template_Template_LineInfo_smart_ptr_ls_Expression_gr__bool:

.. das:function:: apply_template(rules: Template; at: LineInfo; expr: smart_ptr<Expression>; forceAt: bool = true) : ExpressionPtr

.. _function-templates_boost_apply_template_LineInfo_smart_ptr_ls_Expression_gr__ref__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_template(at: LineInfo; expr: smart_ptr<Expression>&; blk: block<(var rules:Template):void>) : ExpressionPtr

.. _function-templates_boost_apply_template_LineInfo_smart_ptr_ls_TypeDecl_gr__ref__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_template(at: LineInfo; typ: smart_ptr<TypeDecl>&; blk: block<(var rules:Template):void>) : TypeDeclPtr

.. _function-templates_boost_apply_template_smart_ptr_ls_Expression_gr__ref__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_template(expr: smart_ptr<Expression>&; blk: block<(var rules:Template):void>) : ExpressionPtr


++++++++++++++++++
Expression helpers
++++++++++++++++++

  *  :ref:`expression_at (var expr: ExpressionPtr; at: LineInfo) : ExpressionPtr <function-templates_boost_expression_at_ExpressionPtr_LineInfo>`
  *  :ref:`remove_deref (varname: string; var expr: smart_ptr\<Expression\>) <function-templates_boost_remove_deref_string_smart_ptr_ls_Expression_gr_>`
  *  :ref:`visit_expression (var expr: ExpressionPtr; var adapter: smart_ptr\<VisitorAdapter\>) <function-templates_boost_visit_expression_ExpressionPtr_smart_ptr_ls_VisitorAdapter_gr_>`

.. _function-templates_boost_expression_at_ExpressionPtr_LineInfo:

.. das:function:: expression_at(expr: ExpressionPtr; at: LineInfo) : ExpressionPtr

Force expression location, then return it.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`

.. _function-templates_boost_remove_deref_string_smart_ptr_ls_Expression_gr_:

.. das:function:: remove_deref(varname: string; expr: smart_ptr<Expression>)

Removes dereferences of the variable `varname` from the expression.
This is typically used when replacing 'workhorse' variable with constant.


:Arguments: * **varname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

.. _function-templates_boost_visit_expression_ExpressionPtr_smart_ptr_ls_VisitorAdapter_gr_:

.. das:function:: visit_expression(expr: ExpressionPtr; adapter: smart_ptr<VisitorAdapter>)

Visits the expression with the given visitor adapter.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`>


+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`make_expression_block (var exprs: dasvector`smart_ptr`Expression) : smart_ptr\<ExprBlock\> <function-templates_boost_make_expression_block_dasvector_rq_smart_ptr_rq_Expression>`
  *  :ref:`make_expression_block (var exprs: array\<ExpressionPtr\>) : smart_ptr\<ExprBlock\> <function-templates_boost_make_expression_block_array_ls_ExpressionPtr_gr_>`


make_expression_block
^^^^^^^^^^^^^^^^^^^^^

.. _function-templates_boost_make_expression_block_dasvector_rq_smart_ptr_rq_Expression:

.. das:function:: make_expression_block(exprs: dasvector`smart_ptr`Expression) : smart_ptr<ExprBlock>

Create ExprBlock and move all expressions from expr to the list of the block.


:Arguments: * **exprs** : vector<smart_ptr<Expression>>

.. _function-templates_boost_make_expression_block_array_ls_ExpressionPtr_gr_:

.. das:function:: make_expression_block(exprs: array<ExpressionPtr>) : smart_ptr<ExprBlock>


+++++++++++++
Block helpers
+++++++++++++

  *  :ref:`move_unquote_block (var expr: ExpressionPtr) : smart_ptr\<ExprBlock\> <function-templates_boost_move_unquote_block_ExpressionPtr>`
  *  :ref:`unquote_block (expr: ExpressionPtr) : smart_ptr\<ExprBlock\> <function-templates_boost_unquote_block_ExpressionPtr>`

.. _function-templates_boost_move_unquote_block_ExpressionPtr:

.. das:function:: move_unquote_block(expr: ExpressionPtr) : smart_ptr<ExprBlock>

Moves the corresponding block subexpression expression from the ExprMakeBlock.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

.. _function-templates_boost_unquote_block_ExpressionPtr:

.. das:function:: unquote_block(expr: ExpressionPtr) : smart_ptr<ExprBlock>

Returns the corresponding block subexpression expression from the ExprMakeBlock.


:Arguments: * **expr** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


+++++++++++++++++++++++
Global variable helpers
+++++++++++++++++++++++

  *  :ref:`add_global_let (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-templates_boost_add_global_let_Module_q__string_LineInfo_ExpressionPtr>`
  *  :ref:`add_global_private_let (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-templates_boost_add_global_private_let_Module_q__string_LineInfo_ExpressionPtr>`
  *  :ref:`add_global_private_var (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-templates_boost_add_global_private_var_Module_q__string_LineInfo_ExpressionPtr>`
  *  :ref:`add_global_var (mod: Module?; vname: string; var typ: TypeDeclPtr; vat: LineInfo; priv: bool; blk: block\<(var v:VariablePtr):void\>) : bool <function-templates_boost_add_global_var_Module_q__string_TypeDeclPtr_LineInfo_bool_block_ls_var_v_c_VariablePtr_c_void_gr_>`
  *  :ref:`add_global_var (mod: Module?; vname: string; vat: LineInfo; var value: ExpressionPtr) : bool <function-templates_boost_add_global_var_Module_q__string_LineInfo_ExpressionPtr>`
  *  :ref:`add_global_var (mod: Module?; vname: string; var typ: TypeDeclPtr; vat: LineInfo; priv: bool) : bool <function-templates_boost_add_global_var_Module_q__string_TypeDeclPtr_LineInfo_bool>`

.. _function-templates_boost_add_global_let_Module_q__string_LineInfo_ExpressionPtr:

.. das:function:: add_global_let(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
Variable type will be constant.


:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

.. _function-templates_boost_add_global_private_let_Module_q__string_LineInfo_ExpressionPtr:

.. das:function:: add_global_private_let(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
It will be private, and type will be constant.


:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`

.. _function-templates_boost_add_global_private_var_Module_q__string_LineInfo_ExpressionPtr:

.. das:function:: add_global_private_var(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

Add global variable to the module, given name and initial value.
It will be private.


:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **vname** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **value** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


add_global_var
^^^^^^^^^^^^^^

.. _function-templates_boost_add_global_var_Module_q__string_TypeDeclPtr_LineInfo_bool_block_ls_var_v_c_VariablePtr_c_void_gr_:

.. das:function:: add_global_var(mod: Module?; vname: string; typ: TypeDeclPtr; vat: LineInfo; priv: bool; blk: block<(var v:VariablePtr):void>) : bool

Add global variable to the module, given name and type.
`priv` specifies if the variable is private to the block.


:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>`?

            * **vname** : string

            * **typ** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>`

            * **priv** : bool

            * **blk** : block<(v: :ref:`VariablePtr <alias-VariablePtr>`):void>

.. _function-templates_boost_add_global_var_Module_q__string_LineInfo_ExpressionPtr:

.. das:function:: add_global_var(mod: Module?; vname: string; vat: LineInfo; value: ExpressionPtr) : bool

.. _function-templates_boost_add_global_var_Module_q__string_TypeDeclPtr_LineInfo_bool:

.. das:function:: add_global_var(mod: Module?; vname: string; typ: TypeDeclPtr; vat: LineInfo; priv: bool) : bool


++++++++++++++
Hygienic names
++++++++++++++

  *  :ref:`make_unique_private_name (prefix: string; vat: LineInfo) : string <function-templates_boost_make_unique_private_name_string_LineInfo>`

.. _function-templates_boost_make_unique_private_name_string_LineInfo:

.. das:function:: make_unique_private_name(prefix: string; vat: LineInfo) : string

Generates unique private name for the variable, given prefix and line info.

.. warning::

   The assumption is that line info is unique for the context of the unique name generation.
   If it is not, additional measures must be taken to ensure uniqueness of prefix.


:Arguments: * **prefix** : string

            * **vat** :  :ref:`LineInfo <handle-rtti-LineInfo>`


++++++++++++++
Quoting macros
++++++++++++++

  *  :ref:`apply_qblock (var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-templates_boost_apply_qblock_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qblock_expr (var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-templates_boost_apply_qblock_expr_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qblock_to_array (var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : array\<ExpressionPtr\> <function-templates_boost_apply_qblock_to_array_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro (var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : ExpressionPtr <function-templates_boost_apply_qmacro_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro_function (fname: string; var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : FunctionPtr <function-templates_boost_apply_qmacro_function_string_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro_method (fname: string; var parent: StructurePtr; var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : FunctionPtr <function-templates_boost_apply_qmacro_method_string_StructurePtr_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro_template_class (instance_name: string; var template_type: smart_ptr\<TypeDecl\>; blk: block\<(var rules:Template):void\>) : TypeDeclPtr <function-templates_boost_apply_qmacro_template_class_string_smart_ptr_ls_TypeDecl_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro_template_function (func: FunctionPtr; blk: block\<(var rules:Template):void\>) : FunctionPtr <function-templates_boost_apply_qmacro_template_function_FunctionPtr_block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qmacro_variable (vname: string; var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : VariablePtr <function-templates_boost_apply_qmacro_variable_string_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`
  *  :ref:`apply_qtype (var expr: smart_ptr\<Expression\>; blk: block\<(var rules:Template):void\>) : TypeDeclPtr <function-templates_boost_apply_qtype_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_>`

.. _function-templates_boost_apply_qblock_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qblock(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reification. This is a block reification.


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qblock_expr_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qblock_expr(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reification. This is a first line of the block as expression reification.


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qblock_to_array_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qblock_to_array(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : array<ExpressionPtr>

Implementation details for the expression reification. This is a block reification.


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : ExpressionPtr

Implementation details for the expression reification.


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_function_string_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro_function(fname: string; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : FunctionPtr

Implementation details for reification. This is a function generation reification.


:Arguments: * **fname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_method_string_StructurePtr_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro_method(fname: string; parent: StructurePtr; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : FunctionPtr

Implementation details for reification. This is a class method function generation reification.


:Arguments: * **fname** : string

            * **parent** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_template_class_string_smart_ptr_ls_TypeDecl_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro_template_class(instance_name: string; template_type: smart_ptr<TypeDecl>; blk: block<(var rules:Template):void>) : TypeDeclPtr

Implementation details for the expression reification. This is a template class instantiation reification.


:Arguments: * **instance_name** : string

            * **template_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_template_function_FunctionPtr_block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro_template_function(func: FunctionPtr; blk: block<(var rules:Template):void>) : FunctionPtr

Applies template rules to a function, cloning it with substituted types.


:Arguments: * **func** :  :ref:`FunctionPtr <alias-FunctionPtr>`

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qmacro_variable_string_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qmacro_variable(vname: string; expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : VariablePtr

Implementation details for reification. This is a variable generation reification.


:Arguments: * **vname** : string

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>

.. _function-templates_boost_apply_qtype_smart_ptr_ls_Expression_gr__block_ls_var_rules_c_Template_c_void_gr_:

.. das:function:: apply_qtype(expr: smart_ptr<Expression>; blk: block<(var rules:Template):void>) : TypeDeclPtr

Implementation details for the expression reification. This is a type declaration reification.


:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>`>

            * **blk** : block<(rules: :ref:`Template <struct-templates_boost-Template>`):void>


++++++++++++++++++++
Type pointer helpers
++++++++++++++++++++

  *  :ref:`add_array_ptr_ref (var a: array\<smart_ptr\<auto(TT)\>\>) : array\<smart_ptr\<TT\>\> <function-templates_boost_add_array_ptr_ref_array_ls_smart_ptr_ls_autoTT_gr__gr_>`
  *  :ref:`add_type_ptr_ref (var st: StructurePtr; flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_StructurePtr_TypeDeclFlags>`
  *  :ref:`add_type_ptr_ref (var st: EnumerationPtr; flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_EnumerationPtr_TypeDeclFlags>`
  *  :ref:`add_type_ptr_ref (a: TypeDeclPtr; flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_TypeDeclPtr_TypeDeclFlags>`
  *  :ref:`add_type_ptr_ref (anything: auto(TT); flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_autoTT_TypeDeclFlags_0x299>`
  *  :ref:`add_type_ptr_ref (var st: Structure?; flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_Structure_q__TypeDeclFlags>`
  *  :ref:`add_type_ptr_ref (var st: Enumeration?; flags: TypeDeclFlags) : TypeDeclPtr <function-templates_boost_add_type_ptr_ref_Enumeration_q__TypeDeclFlags>`

.. _function-templates_boost_add_array_ptr_ref_array_ls_smart_ptr_ls_autoTT_gr__gr_:

.. das:function:: add_array_ptr_ref(a: array<smart_ptr<auto(TT)>>) : array<smart_ptr<TT>>

Implementation details for the reification. This adds any array to the rules.


:Arguments: * **a** : array<smart_ptr<auto(TT)>>


add_type_ptr_ref
^^^^^^^^^^^^^^^^

.. _function-templates_boost_add_type_ptr_ref_StructurePtr_TypeDeclFlags:

.. das:function:: add_type_ptr_ref(st: StructurePtr; flags: TypeDeclFlags) : TypeDeclPtr

Implementation details for the reification. Creates a type declaration from a structure smart pointer.


:Arguments: * **st** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>`

.. _function-templates_boost_add_type_ptr_ref_EnumerationPtr_TypeDeclFlags:

.. das:function:: add_type_ptr_ref(st: EnumerationPtr; flags: TypeDeclFlags) : TypeDeclPtr

.. _function-templates_boost_add_type_ptr_ref_TypeDeclPtr_TypeDeclFlags:

.. das:function:: add_type_ptr_ref(a: TypeDeclPtr; flags: TypeDeclFlags) : TypeDeclPtr

.. _function-templates_boost_add_type_ptr_ref_autoTT_TypeDeclFlags_0x299:

.. das:function:: add_type_ptr_ref(anything: auto(TT); flags: TypeDeclFlags) : TypeDeclPtr

.. _function-templates_boost_add_type_ptr_ref_Structure_q__TypeDeclFlags:

.. das:function:: add_type_ptr_ref(st: Structure?; flags: TypeDeclFlags) : TypeDeclPtr

.. _function-templates_boost_add_type_ptr_ref_Enumeration_q__TypeDeclFlags:

.. das:function:: add_type_ptr_ref(st: Enumeration?; flags: TypeDeclFlags) : TypeDeclPtr


+++++++++++++++++
Structure helpers
+++++++++++++++++

  *  :ref:`add_structure_field (var cls: StructurePtr; name: string; var t: TypeDeclPtr; var init: ExpressionPtr) : int <function-templates_boost_add_structure_field_StructurePtr_string_TypeDeclPtr_ExpressionPtr>`

.. _function-templates_boost_add_structure_field_StructurePtr_string_TypeDeclPtr_ExpressionPtr:

.. das:function:: add_structure_field(cls: StructurePtr; name: string; t: TypeDeclPtr; init: ExpressionPtr) : int

Adds a field to the structure.


:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **name** : string

            * **t** :  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

            * **init** :  :ref:`ExpressionPtr <alias-ExpressionPtr>`


++++++++++++++++
Class generation
++++++++++++++++

  *  :ref:`enum_class_type (st: auto) : auto <function-templates_boost_enum_class_type_auto_0x277>`
  *  :ref:`make_class (name: string; var baseClass: StructurePtr; mod: Module?) : smart_ptr\<Structure\> <function-templates_boost_make_class_string_StructurePtr_Module_q_>`
  *  :ref:`make_class (name: string; mod: Module?) : smart_ptr\<Structure\> <function-templates_boost_make_class_string_Module_q_>`
  *  :ref:`make_class (name: string; var baseClass: Structure?; mod: Module?) : smart_ptr\<Structure\> <function-templates_boost_make_class_string_Structure_q__Module_q_>`
  *  :ref:`make_class_constructor (cls: StructurePtr; ctor: FunctionPtr) : smart_ptr\<Function\> <function-templates_boost_make_class_constructor_StructurePtr_FunctionPtr>`
  *  :ref:`modify_to_class_member (cls: StructurePtr; fun: FunctionPtr; isExplicit: bool; Constant: bool) <function-templates_boost_modify_to_class_member_StructurePtr_FunctionPtr_bool_bool>`

.. _function-templates_boost_enum_class_type_auto_0x277:

.. das:function:: enum_class_type(st: auto) : auto

return underlying type for the enumeration


:Arguments: * **st** : auto


make_class
^^^^^^^^^^

.. _function-templates_boost_make_class_string_StructurePtr_Module_q_:

.. das:function:: make_class(name: string; baseClass: StructurePtr; mod: Module?) : smart_ptr<Structure>

Creates a class structure derived from baseClass. Adds __rtti, __finalize fields.


:Arguments: * **name** : string

            * **baseClass** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **mod** :  :ref:`Module <handle-rtti-Module>`?

.. _function-templates_boost_make_class_string_Module_q_:

.. das:function:: make_class(name: string; mod: Module?) : smart_ptr<Structure>

.. _function-templates_boost_make_class_string_Structure_q__Module_q_:

.. das:function:: make_class(name: string; baseClass: Structure?; mod: Module?) : smart_ptr<Structure>

----

.. _function-templates_boost_make_class_constructor_StructurePtr_FunctionPtr:

.. das:function:: make_class_constructor(cls: StructurePtr; ctor: FunctionPtr) : smart_ptr<Function>

Adds a class constructor from a constructor function.


:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **ctor** :  :ref:`FunctionPtr <alias-FunctionPtr>`

.. _function-templates_boost_modify_to_class_member_StructurePtr_FunctionPtr_bool_bool:

.. das:function:: modify_to_class_member(cls: StructurePtr; fun: FunctionPtr; isExplicit: bool; Constant: bool)

Modifies function to be a member of a particular class.


:Arguments: * **cls** :  :ref:`StructurePtr <alias-StructurePtr>`

            * **fun** :  :ref:`FunctionPtr <alias-FunctionPtr>`

            * **isExplicit** : bool

            * **Constant** : bool


