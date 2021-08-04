
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
+var2expr    +table<string;smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >>                                +
+------------+---------------------------------------------------------------------------------------------------------+
+type2type   +table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+blockArgName+table<string;string>                                                                                     +
+------------+---------------------------------------------------------------------------------------------------------+
+annArg      +table<string;lambda<(ann: :ref:`rtti::AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` ):void>>+
+------------+---------------------------------------------------------------------------------------------------------+


|structure-templates_boost-Template|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_templates_boost_c__c_add_global_let__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: add_global_let(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

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

.. _function-_at_templates_boost_c__c_add_global_private_let__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: add_global_private_let(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

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

.. _function-_at_templates_boost_c__c_add_global_private_var__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: add_global_private_var(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

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

.. _function-_at_templates_boost_c__c_add_global_var__hh_const_hh__hh_ptr_hh__hh_handle_hh_Module__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: add_global_var(mod: rtti::Module? const; vname: string const; vat: LineInfo const; value: ExpressionPtr)

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

.. _function-_at_templates_boost_c__c_apply_template__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression__hh_const_hh__hh_block_hh_Template_hh__rq_rules_hh__c_void__hh_const_hh_bool:

.. das:function:: apply_template(at: LineInfo const; expr: smart_ptr<ast::Expression>; blk: block<(rules:templates_boost::Template -const):void> const; forceAt: bool const)

+--------+----------------------------------------------------------------------------------------------+
+argument+argument type                                                                                 +
+========+==============================================================================================+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const                                          +
+--------+----------------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                                   +
+--------+----------------------------------------------------------------------------------------------+
+blk     +block<(rules: :ref:`templates_boost::Template <struct-templates_boost-Template>` ):void> const+
+--------+----------------------------------------------------------------------------------------------+
+forceAt +bool const                                                                                    +
+--------+----------------------------------------------------------------------------------------------+


|function-templates_boost-apply_template|

.. _function-_at_templates_boost_c__c_apply_template_Template__hh_const_hh__hh_handle_hh_LineInfo__hh_smart_ptr_hh__hh_handle_hh_Expression__hh_const_hh_bool:

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

.. _function-_at_templates_boost_c__c_make_unique_private_name__hh_const_hh_string__hh_const_hh__hh_handle_hh_LineInfo:

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

.. _function-_at_templates_boost_c__c_move_unquote_block__hh_ref_hh__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: move_unquote_block(expr: ExpressionPtr)

move_unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-templates_boost-move_unquote_block|

.. _function-_at_templates_boost_c__c_renameVariable_Template__hh_const_hh_string__hh_const_hh_string:

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

.. _function-_at_templates_boost_c__c_replaceAnnotationArgument_Template__hh_const_hh_string__hh_lambda_hh__hh_handle_hh_AnnotationDeclaration_hh__rq_ann_hh__c_void:

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

.. _function-_at_templates_boost_c__c_replaceBlockArgument_Template__hh_const_hh_string__hh_const_hh_string:

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

.. _function-_at_templates_boost_c__c_replaceType_Template__hh_const_hh_string__hh_const_hh_string:

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

.. _function-_at_templates_boost_c__c_replaceVariable_Template__hh_const_hh_string__hh_smart_ptr_hh__hh_handle_hh_Expression:

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

.. _function-_at_templates_boost_c__c_unquote_block__hh_const_hh__hh_smart_ptr_hh__hh_handle_hh_Expression:

.. das:function:: unquote_block(expr: ExpressionPtr)

unquote_block returns smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` >

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>` +
+--------+--------------------------------------------+


|function-templates_boost-unquote_block|


