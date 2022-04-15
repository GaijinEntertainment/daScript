
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
+var2name    +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+var2expr    +table<string;smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >>                                +
+------------+---------------------------------------------------------------------------------------------------------+
+type2type   +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+blockArgName+table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+annArg      +table<string;lambda<(ann: :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>>+
+------------+---------------------------------------------------------------------------------------------------------+


This structure contains collection of subsitution rules for a template.

++++++++++++++
Template rules
++++++++++++++

  *  :ref:`kaboomVarField (self:templates_boost::Template -const;name:string const;prefix:string const;suffix:string const) : void <function-_at_templates_boost_c__c_kaboomVarField_S_ls_Template_gr__Cs_Cs_Cs>` 
  *  :ref:`replaceVariable (self:templates_boost::Template -const;name:string const;expr:smart_ptr\<ast::Expression\> -const) : void <function-_at_templates_boost_c__c_replaceVariable_S_ls_Template_gr__Cs_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M>` 
  *  :ref:`renameVariable (self:templates_boost::Template -const;name:string const;newName:string const) : void <function-_at_templates_boost_c__c_renameVariable_S_ls_Template_gr__Cs_Cs>` 
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


Adds a rule to the template to replace a type alias with another type alias.

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

  *  :ref:`apply_template (rules:templates_boost::Template -const;at:rtti::LineInfo const;expr:smart_ptr\<ast::Expression\> -const;forceAt:bool const) : void <function-_at_templates_boost_c__c_apply_template_S_ls_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_Cb>` 
  *  :ref:`apply_template (at:rtti::LineInfo const;expr:smart_ptr\<ast::Expression\> -const;blk:block\<(rules:templates_boost::Template -const):void\> const) : void <function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`apply_template (expr:smart_ptr\<ast::Expression\> -const;blk:block\<(rules:templates_boost::Template -const):void\> const) : void <function-_at_templates_boost_c__c_apply_template_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at_templates_boost_c__c_apply_template_S_ls_Template_gr__CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_Cb:

.. das:function:: apply_template(rules: Template; at: LineInfo const; expr: smart_ptr<ast::Expression>; forceAt: bool const)

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

.. _function-_at_templates_boost_c__c_apply_template_CH_ls_rtti_c__c_LineInfo_gr__1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(at: LineInfo const; expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const                                          +
+--------+----------------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


Applies the template to the given expression. If `forceAt` is set, the resulting expression will have the same line info as 'at'.

.. _function-_at_templates_boost_c__c_apply_template_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
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


