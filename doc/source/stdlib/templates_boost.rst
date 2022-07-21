
.. _stdlib_templates_boost:

============================
Template application helpers
============================

.. include:: detail/templates_boost.rst

The templates boost module implements collection of helper macros and functions to accompany :ref:`AST <stdlib_templates>`.

All functions and symbols are in "templates_boost" module, use require to get access to it. ::

    require daslib/templates_boost


.. _struct-templates_boost-Template:

.. das:attribute:: Template



Template fields are

+------------+---------------------------------------------------------------------------------------------------------+
+kaboomVar   +table<string;tuple<prefix:string;suffix:string>>                                                         +
+------------+---------------------------------------------------------------------------------------------------------+
+call2name   +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+field2name  +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+var2name    +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+var2expr    +table<string;smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >>                                +
+------------+---------------------------------------------------------------------------------------------------------+
+var2exprList+table<string;array<smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >>>                         +
+------------+---------------------------------------------------------------------------------------------------------+
+type2type   +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+type2etype  +table<string; :ref:`TypeDeclPtr <alias-TypeDeclPtr>` >                                                   +
+------------+---------------------------------------------------------------------------------------------------------+
+blockArgName+table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+annArg      +table<string;lambda<(ann: :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>>+
+------------+---------------------------------------------------------------------------------------------------------+
+blkArg      +table<string;array< :ref:`VariablePtr <alias-VariablePtr>` >>                                            +
+------------+---------------------------------------------------------------------------------------------------------+


This structure contains collection of subsitution rules for a template.

+++++++++++
Call macros
+++++++++++

.. _call-macro-templates_boost-qmacro_expr:

.. das:attribute:: qmacro_expr

|function_annotation-templates_boost-qmacro_expr|

.. _call-macro-templates_boost-qmacro_variable:

.. das:attribute:: qmacro_variable

|function_annotation-templates_boost-qmacro_variable|

.. _call-macro-templates_boost-qmacro_function:

.. das:attribute:: qmacro_function

|function_annotation-templates_boost-qmacro_function|

.. _call-macro-templates_boost-qmacro:

.. das:attribute:: qmacro

This macro will implement expression substitution macro

.. _call-macro-templates_boost-qmacro_block:

.. das:attribute:: qmacro_block

|function_annotation-templates_boost-qmacro_block|

.. _call-macro-templates_boost-qmacro_type:

.. das:attribute:: qmacro_type

|function_annotation-templates_boost-qmacro_type|

+++++++
Classes
+++++++

.. _struct-templates_boost-AstQCallMacro:

.. das:attribute:: AstQCallMacro : AstCallMacro

This macro will implement expression reification

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. das:function:: AstQCallMacro.visit(self: AstCallMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprCallMacro> const)

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstCallMacro <struct-ast-AstCallMacro>`                    +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-AstQCallMacro.visit|

.. das:function:: AstQCallMacro.canVisitArguments(self: AstCallMacro; expr: smart_ptr<ast::ExprCallMacro> const)

canVisitArguments returns bool

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstCallMacro <struct-ast-AstCallMacro>`                    +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-AstQCallMacro.canVisitArguments|

.. das:function:: AstQCallMacro.wrap_call(self: AstQCallMacro; expr: ExpressionPtr)

wrap_call returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+self    + :ref:`templates_boost::AstQCallMacro <struct-templates_boost-AstQCallMacro>` +
+--------+------------------------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                   +
+--------+------------------------------------------------------------------------------+


|method-templates_boost-AstQCallMacro.wrap_call|

.. _struct-templates_boost-QMacro:

.. das:attribute:: QMacro : AstQCallMacro

|class-templates_boost-QMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. _struct-templates_boost-QBlockMacro:

.. das:attribute:: QBlockMacro : AstQCallMacro

|class-templates_boost-QBlockMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. _struct-templates_boost-QBlockExprMacro:

.. das:attribute:: QBlockExprMacro : AstQCallMacro

|class-templates_boost-QBlockExprMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. _struct-templates_boost-QTypeMacro:

.. das:attribute:: QTypeMacro : AstQCallMacro

|class-templates_boost-QTypeMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. _struct-templates_boost-AstQNamedMacro:

.. das:attribute:: AstQNamedMacro : AstCallMacro

This macro will implement expression reification for the named expressions (function, variable, etc.)

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. das:function:: AstQNamedMacro.visit(self: AstCallMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprCallMacro> const)

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstCallMacro <struct-ast-AstCallMacro>`                    +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-AstQNamedMacro.visit|

.. _struct-templates_boost-AstQFunctionMacro:

.. das:attribute:: AstQFunctionMacro : AstQNamedMacro

|class-templates_boost-AstQFunctionMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

.. _struct-templates_boost-AstQVariableMacro:

.. das:attribute:: AstQVariableMacro : AstQNamedMacro

|class-templates_boost-AstQVariableMacro|

it defines as follows

  | apply_call : string const
  | macro_call : string const

++++++++++++++
Template rules
++++++++++++++

  *  :ref:`kaboomVarField (self:templates_boost::Template -const;name:string const;prefix:string const;suffix:string const) : void <function-_at_templates_boost_c__c_kaboomVarField_S_ls_Template_gr__Cs_Cs_Cs>` 
  *  :ref:`replaceVariable (self:templates_boost::Template -const;name:string const;expr:smart_ptr\<ast::Expression\> -const) : void <function-_at_templates_boost_c__c_replaceVariable_S_ls_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`renameVariable (self:templates_boost::Template -const;name:string const;newName:string const) : void <function-_at_templates_boost_c__c_renameVariable_S_ls_Template_gr__Cs_Cs>` 
  *  :ref:`renameVariable (self:templates_boost::Template -const;name:string const;newName:$::das_string const) : void <function-_at_templates_boost_c__c_renameVariable_S_ls_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`replaceType (self:templates_boost::Template -const;name:string const;newName:string const) : void <function-_at_templates_boost_c__c_replaceType_S_ls_Template_gr__Cs_Cs>` 
  *  :ref:`replaceAnnotationArgument (self:templates_boost::Template -const;name:string const;cb:lambda\<(ann:rtti::AnnotationDeclaration -const):void\> -const) : void <function-_at_templates_boost_c__c_replaceAnnotationArgument_S_ls_Template_gr__Cs_N_ls_ann_gr_0_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr_1_ls_v_gr__at_>` 
  *  :ref:`replaceBlockArgument (self:templates_boost::Template -const;name:string const;newName:string const) : void <function-_at_templates_boost_c__c_replaceBlockArgument_S_ls_Template_gr__Cs_Cs>` 

.. _function-_at_templates_boost_c__c_kaboomVarField_S_ls_Template_gr__Cs_Cs_Cs:

.. das:function:: kaboomVarField(self: Template; name: string const; prefix: string const; suffix: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+prefix  +string const                                                        +
+--------+--------------------------------------------------------------------+
+suffix  +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to to the template to replace a variable field access with a prefix and suffix.
I.e. foo.bar into prefix + bar + suffix

.. _function-_at_templates_boost_c__c_replaceVariable_S_ls_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: replaceVariable(self: Template; name: string const; expr: smart_ptr<ast::Expression>)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >         +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to replace a variable with an expression.

.. _function-_at_templates_boost_c__c_renameVariable_S_ls_Template_gr__Cs_Cs:

.. das:function:: renameVariable(self: Template; name: string const; newName: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename a variable.

.. _function-_at_templates_boost_c__c_renameVariable_S_ls_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameVariable(self: Template; name: string const; newName: das_string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName + :ref:`builtin::das_string <handle-builtin-das_string>`  const      +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename a variable.

.. _function-_at_templates_boost_c__c_replaceType_S_ls_Template_gr__Cs_Cs:

.. das:function:: replaceType(self: Template; name: string const; newName: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to replace a type alias with another type alias, specified by name.

.. _function-_at_templates_boost_c__c_replaceAnnotationArgument_S_ls_Template_gr__Cs_N_ls_ann_gr_0_ls_H_ls_rtti_c__c_AnnotationDeclaration_gr__gr_1_ls_v_gr__at_:

.. das:function:: replaceAnnotationArgument(self: Template; name: string const; cb: lambda<(ann:rtti::AnnotationDeclaration -const):void>)

+--------+-------------------------------------------------------------------------------------------+
+argument+argument type                                                                              +
+========+===========================================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>`                        +
+--------+-------------------------------------------------------------------------------------------+
+name    +string const                                                                               +
+--------+-------------------------------------------------------------------------------------------+
+cb      +lambda<(ann: :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>+
+--------+-------------------------------------------------------------------------------------------+


Adds a rule to the template to replace an annotation argument with the result of a callback.

.. _function-_at_templates_boost_c__c_replaceBlockArgument_S_ls_Template_gr__Cs_Cs:

.. das:function:: replaceBlockArgument(self: Template; name: string const; newName: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename a block argument.

++++++++++++++++++++
Template application
++++++++++++++++++++

  *  :ref:`apply_template (rules:templates_boost::Template -const;at:rtti::LineInfo const;expr:smart_ptr\<ast::Expression\> -const;forceAt:bool const) : smart_ptr\<ast::Expression\> <function-_at_templates_boost_c__c_apply_template_S_ls_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_Cb>` 
  *  :ref:`apply_template (at:rtti::LineInfo const;expr:smart_ptr\<ast::Expression\>& -const;blk:block\<(rules:templates_boost::Template -const):void\> const) : smart_ptr\<ast::Expression\> <function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__&1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`apply_template (expr:smart_ptr\<ast::Expression\>& -const;blk:block\<(rules:templates_boost::Template -const):void\> const) : smart_ptr\<ast::Expression\> <function-_at_templates_boost_c__c_apply_template_&1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at_templates_boost_c__c_apply_template_S_ls_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_Cb:

.. das:function:: apply_template(rules: Template; at: LineInfo const; expr: smart_ptr<ast::Expression>; forceAt: bool const)

apply_template returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+rules   + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const                +
+--------+--------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >         +
+--------+--------------------------------------------------------------------+
+forceAt +bool const                                                          +
+--------+--------------------------------------------------------------------+


Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.

.. _function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__&1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(at: LineInfo const; expr: smart_ptr<ast::Expression>&; blk: block<(rules:templates_boost::Template -const):void> const)

apply_template returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const                                          +
+--------+----------------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >&                                  +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.

.. _function-_at_templates_boost_c__c_apply_template_&1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(expr: smart_ptr<ast::Expression>&; blk: block<(rules:templates_boost::Template -const):void> const)

apply_template returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >&                                  +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.

++++++++++++++++++
Expression helpers
++++++++++++++++++

  *  :ref:`remove_deref (varname:string const;expr:smart_ptr\<ast::Expression\> -const) : void <function-_at_templates_boost_c__c_remove_deref_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 

.. _function-_at_templates_boost_c__c_remove_deref_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: remove_deref(varname: string const; expr: smart_ptr<ast::Expression>)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+varname +string const                                               +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+--------+-----------------------------------------------------------+


Removes dereferences of the variable `varname` from the expression.
This is typically used when replacing 'workhorse' variable with constant.

+++++++++++++
Block helpers
+++++++++++++

  *  :ref:`unquote_block (expr:smart_ptr\<ast::Expression\> const) : smart_ptr\<ast::ExprBlock\> <function-_at_templates_boost_c__c_unquote_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`move_unquote_block (expr:smart_ptr\<ast::Expression\>& -const) : smart_ptr\<ast::ExprBlock\> <function-_at_templates_boost_c__c_move_unquote_block_&Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 

.. _function-_at_templates_boost_c__c_unquote_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: unquote_block(expr: ExpressionPtr)

unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Returns the corresponding block subexpression expression form the ExprMakeBlock.

.. _function-_at_templates_boost_c__c_move_unquote_block_&Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: move_unquote_block(expr: ExpressionPtr)

move_unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


Moves the corresponding block subexpression expression form the ExprMakeBlock.

+++++++++++++++++++++++
Global variable helpers
+++++++++++++++++++++++

  *  :ref:`add_global_var (mod:rtti::Module? const;vname:string const;vat:rtti::LineInfo const;value:smart_ptr\<ast::Expression\> -const) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`add_global_var (mod:rtti::Module? const;vname:string const;typ:smart_ptr\<ast::TypeDecl\> -const;vat:rtti::LineInfo const;priv:bool const;blk:block\<(v:smart_ptr\<ast::Variable\> -const):void\> const) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cb_CN_ls_v_gr_0_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`add_global_var (mod:rtti::Module? const;vname:string const;typ:smart_ptr\<ast::TypeDecl\> -const;vat:rtti::LineInfo const;priv:bool const) : bool <function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cb>` 
  *  :ref:`add_global_let (mod:rtti::Module? const;vname:string const;vat:rtti::LineInfo const;value:smart_ptr\<ast::Expression\> -const) : bool <function-_at_templates_boost_c__c_add_global_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`add_global_private_var (mod:rtti::Module? const;vname:string const;vat:rtti::LineInfo const;value:smart_ptr\<ast::Expression\> -const) : bool <function-_at_templates_boost_c__c_add_global_private_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`add_global_private_let (mod:rtti::Module? const;vname:string const;vat:rtti::LineInfo const;value:smart_ptr\<ast::Expression\> -const) : bool <function-_at_templates_boost_c__c_add_global_private_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: add_global_var(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

add_global_var returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+
+vname   +string const                                        +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`         +
+--------+----------------------------------------------------+


Adds global variable to the module, given name and initial value.
Global variables type is would be inferred from the initial value.
`priv` specifies if the variable is private to the block.

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cb_CN_ls_v_gr_0_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M_gr_1_ls_v_gr__builtin_:

.. das:function:: add_global_var(mod: rtti::Module? const; vname: string const; typ: TypeDeclPtr; vat: LineInfo const; priv: bool const; blk: block<(v:smart_ptr<ast::Variable> -const):void> const)

add_global_var returns bool

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const             +
+--------+--------------------------------------------------------------+
+vname   +string const                                                  +
+--------+--------------------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+--------+--------------------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const          +
+--------+--------------------------------------------------------------+
+priv    +bool const                                                    +
+--------+--------------------------------------------------------------+
+blk     +block<(v: :ref:`VariablePtr <alias-VariablePtr>` ):void> const+
+--------+--------------------------------------------------------------+


Adds global variable to the module, given name and initial value.
Global variables type is would be inferred from the initial value.
`priv` specifies if the variable is private to the block.

.. _function-_at_templates_boost_c__c_add_global_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr__Cb:

.. das:function:: add_global_var(mod: rtti::Module? const; vname: string const; typ: TypeDeclPtr; vat: LineInfo const; priv: bool const)

add_global_var returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+
+vname   +string const                                        +
+--------+----------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`             +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+priv    +bool const                                          +
+--------+----------------------------------------------------+


Adds global variable to the module, given name and initial value.
Global variables type is would be inferred from the initial value.
`priv` specifies if the variable is private to the block.

.. _function-_at_templates_boost_c__c_add_global_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: add_global_let(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

add_global_let returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+
+vname   +string const                                        +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`         +
+--------+----------------------------------------------------+


Add global variable to the module, given name and initial value.
Variable type will be constant.

.. _function-_at_templates_boost_c__c_add_global_private_var_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: add_global_private_var(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

add_global_private_var returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+
+vname   +string const                                        +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`         +
+--------+----------------------------------------------------+


Add global variable to the module, given name and initial value.
It will be private.

.. _function-_at_templates_boost_c__c_add_global_private_let_C1_ls_H_ls_rtti_c__c_Module_gr__gr_?_Cs_CH_ls_rtti_c__c_LineInfo_gr__Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: add_global_private_let(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

add_global_private_let returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+
+vname   +string const                                        +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`         +
+--------+----------------------------------------------------+


Add global variable to the module, given name and initial value.
It will be private, and type will be constant.

+++++++++++++
Hygenic names
+++++++++++++

  *  :ref:`make_unique_private_name (prefix:string const;vat:rtti::LineInfo const) : string <function-_at_templates_boost_c__c_make_unique_private_name_Cs_CH_ls_rtti_c__c_LineInfo_gr_>` 

.. _function-_at_templates_boost_c__c_make_unique_private_name_Cs_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: make_unique_private_name(prefix: string const; vat: LineInfo const)

make_unique_private_name returns string

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+prefix  +string const                                        +
+--------+----------------------------------------------------+
+vat     + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


Generates unique private name for the variable, given prefix and line info.

.. warning::
The assumption is that line info is unique for the context of the unique name generation.
If it is not, additional measures must be taken to ensure uniqueness of prefix.

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_templates_boost_c__c_replaceArgumentWithList_S_ls_Template_gr__Cs_C1_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M_gr_A:

.. das:function:: replaceArgumentWithList(self: Template; name: string const; blka: array<smart_ptr<ast::Variable>> const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+blka    +array< :ref:`VariablePtr <alias-VariablePtr>` > const               +
+--------+--------------------------------------------------------------------+


|function-templates_boost-replaceArgumentWithList|

.. _function-_at_templates_boost_c__c_replaceVariableWithList_S_ls_Template_gr__Cs_C1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_gr_A:

.. das:function:: replaceVariableWithList(self: Template; name: string const; expr: array<smart_ptr<ast::Expression>> const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+expr    +array< :ref:`ExpressionPtr <alias-ExpressionPtr>` > const           +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to replace a variable with an expression list.

.. _function-_at_templates_boost_c__c_replaceVariableWithList_S_ls_Template_gr__Cs_CH_ls__builtin__c__c_dasvector_rq_smart_ptr_rq_Expression_gr_:

.. das:function:: replaceVariableWithList(self: Template; name: string const; expr: dasvector`smart_ptr`Expression const)

+--------+------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                         +
+========+======================================================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>`                                   +
+--------+------------------------------------------------------------------------------------------------------+
+name    +string const                                                                                          +
+--------+------------------------------------------------------------------------------------------------------+
+expr    + :ref:`builtin::dasvector`smart_ptr`Expression <handle-builtin-dasvector`smart_ptr`Expression>`  const+
+--------+------------------------------------------------------------------------------------------------------+


Adds a rule to the template to replace a variable with an expression list.

.. _function-_at_templates_boost_c__c_renameField_S_ls_Template_gr__Cs_Cs:

.. das:function:: renameField(self: Template; name: string const; newName: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename any field lookup (., ?., as, is, etc)

.. _function-_at_templates_boost_c__c_renameField_S_ls_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameField(self: Template; name: string const; newName: das_string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName + :ref:`builtin::das_string <handle-builtin-das_string>`  const      +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename any field lookup (., ?., as, is, etc)

.. _function-_at_templates_boost_c__c_replaceTypeWithTypeDecl_S_ls_Template_gr__Cs_Y_ls_StructurePtr_gr_1_ls_H_ls_ast_c__c_Structure_gr__gr_?M:

.. das:function:: replaceTypeWithTypeDecl(self: Template; name: string const; st: StructurePtr)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                           +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.

.. _function-_at_templates_boost_c__c_replaceTypeWithTypeDecl_S_ls_Template_gr__Cs_Y_ls_TypeDeclPtr_gr_1_ls_H_ls_ast_c__c_TypeDecl_gr__gr_?M:

.. das:function:: replaceTypeWithTypeDecl(self: Template; name: string const; expr: TypeDeclPtr)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+expr    + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                             +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to replace a type alias with another type alias, specified by type declaration.

.. _function-_at_templates_boost_c__c_renameCall_S_ls_Template_gr__Cs_Cs:

.. das:function:: renameCall(self: Template; name: string const; newName: string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName +string const                                                        +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename a call.

.. _function-_at_templates_boost_c__c_renameCall_S_ls_Template_gr__Cs_CH_ls__builtin__c__c_das_string_gr_:

.. das:function:: renameCall(self: Template; name: string const; newName: das_string const)

+--------+--------------------------------------------------------------------+
+argument+argument type                                                       +
+========+====================================================================+
+self    + :ref:`templates_boost::Template <struct-templates_boost-Template>` +
+--------+--------------------------------------------------------------------+
+name    +string const                                                        +
+--------+--------------------------------------------------------------------+
+newName + :ref:`builtin::das_string <handle-builtin-das_string>`  const      +
+--------+--------------------------------------------------------------------+


Adds a rule to the template to rename a call.

.. _function-_at_templates_boost_c__c_make_expression_block_1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_gr_A:

.. das:function:: make_expression_block(exprs: array<smart_ptr<ast::Expression>>)

make_expression_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+exprs   +array< :ref:`ExpressionPtr <alias-ExpressionPtr>` >+
+--------+---------------------------------------------------+


Create ExprBlock and move all expressions from expr to the list of the block.

.. _function-_at_templates_boost_c__c_apply_qmacro_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qmacro returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Implementation details for the expression reificaiton.

.. _function-_at_templates_boost_c__c_apply_qblock_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qblock(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qblock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Implementation details for the expression reificaiton.

.. _function-_at_templates_boost_c__c_apply_qblock_expr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qblock_expr(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qblock_expr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Implementation details for the expression reificaiton.

.. _function-_at_templates_boost_c__c_apply_qtype_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qtype(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qtype returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Implementation details for the expression reificaiton.

.. _function-_at_templates_boost_c__c_expression_at_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: expression_at(expr: ExpressionPtr; at: LineInfo const)

expression_at returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`         +
+--------+----------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const+
+--------+----------------------------------------------------+


Force expression location, than return it.

.. _function-_at_templates_boost_c__c_emplace_new_1_ls_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_gr_A_Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: emplace_new(arr: array<smart_ptr<ast::Expression>>; expr: ExpressionPtr)

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+arr     +array< :ref:`ExpressionPtr <alias-ExpressionPtr>` >+
+--------+---------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`        +
+--------+---------------------------------------------------+


|function-templates_boost-emplace_new|

.. _function-_at_templates_boost_c__c_emplace_new_1_ls_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M_gr_A_Y_ls_VariablePtr_gr_1_ls_H_ls_ast_c__c_Variable_gr__gr_?M:

.. das:function:: emplace_new(arr: array<smart_ptr<ast::Variable>>; expr: VariablePtr)

+--------+-----------------------------------------------+
+argument+argument type                                  +
+========+===============================================+
+arr     +array< :ref:`VariablePtr <alias-VariablePtr>` >+
+--------+-----------------------------------------------+
+expr    + :ref:`VariablePtr <alias-VariablePtr>`        +
+--------+-----------------------------------------------+


|function-templates_boost-emplace_new|

.. _function-_at_templates_boost_c__c_apply_qmacro_function_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro_function(fname: string const; expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qmacro_function returns  :ref:`FunctionPtr <alias-FunctionPtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+fname   +string const                                                                                  +
+--------+----------------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


|function-templates_boost-apply_qmacro_function|

.. _function-_at_templates_boost_c__c_apply_qmacro_variable_Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_qmacro_variable(vname: string const; expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

apply_qmacro_variable returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+vname   +string const                                                                                  +
+--------+----------------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


|function-templates_boost-apply_qmacro_variable|

.. _function-_at_templates_boost_c__c_add_array_ptr_ref_1_ls_1_ls_Y_ls_TT_gr_._gr_?M_gr_A:

.. das:function:: add_array_ptr_ref(a: array<smart_ptr<auto(TT)>>)

add_array_ptr_ref returns array<smart_ptr<TT>>

+--------+--------------------------+
+argument+argument type             +
+========+==========================+
+a       +array<smart_ptr<auto(TT)>>+
+--------+--------------------------+


|function-templates_boost-add_array_ptr_ref|


