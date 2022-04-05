
.. _stdlib_templates_boost:

============================
Template application helpers
============================

.. include:: detail/templates_boost.rst

|module-templates_boost|

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



+++++++++++++
Uncategorized
+++++++++++++

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


|function-templates_boost-kaboomVarField|

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


|function-templates_boost-replaceVariable|

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


|function-templates_boost-renameVariable|

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


|function-templates_boost-replaceType|

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


|function-templates_boost-replaceAnnotationArgument|

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


|function-templates_boost-replaceBlockArgument|

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


|function-templates_boost-apply_template|

.. _function-_at_templates_boost_c__c_unquote_block_CY_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: unquote_block(expr: ExpressionPtr)

unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-templates_boost-unquote_block|

.. _function-_at_templates_boost_c__c_move_unquote_block_&Y_ls_ExpressionPtr_gr_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M:

.. das:function:: move_unquote_block(expr: ExpressionPtr)

move_unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-templates_boost-move_unquote_block|

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


|function-templates_boost-add_global_var|

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


|function-templates_boost-add_global_var|

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


|function-templates_boost-add_global_var|

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


|function-templates_boost-add_global_let|

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


|function-templates_boost-add_global_private_var|

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


|function-templates_boost-add_global_private_let|

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


|function-templates_boost-make_unique_private_name|

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


|function-templates_boost-apply_template|

.. _function-_at_templates_boost_c__c_apply_template_1_ls_H_ls_ast_c__c_Expression_gr__gr_?M_CN_ls_rules_gr_0_ls_S_ls_Template_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: apply_template(expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const)

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+


|function-templates_boost-apply_template|


