
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

+++++++
Classes
+++++++

.. _struct-templates_boost-TemplateVisitor:

.. das:attribute:: TemplateVisitor : AstVisitor

|class-templates_boost-TemplateVisitor|

it defines as follows

  rules :  :ref:`templates_boost::Template <struct-templates_boost-Template>` ?

.. das:function:: TemplateVisitor.preVisitProgram(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitProgram|

.. das:function:: TemplateVisitor.visitProgram(self: AstVisitor; porg: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+porg    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitProgram|

.. das:function:: TemplateVisitor.preVisitProgramBody(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitProgramBody|

.. das:function:: TemplateVisitor.preVisitTypeDecl(self: AstVisitor; typ: TypeDeclPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitTypeDecl|

.. das:function:: TemplateVisitor.visitTypeDecl(self: AstVisitor; typ: TypeDeclPtr)

visitTypeDecl returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitTypeDecl|

.. das:function:: TemplateVisitor.preVisitAlias(self: AstVisitor; typ: TypeDeclPtr; name: das_string const)

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+--------+--------------------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+--------+--------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitAlias|

.. das:function:: TemplateVisitor.visitAlias(self: AstVisitor; typ: TypeDeclPtr; name: das_string const)

visitAlias returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+--------+--------------------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+--------+--------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitAlias|

.. das:function:: TemplateVisitor.preVisitEnumeration(self: AstVisitor; enu: EnumerationPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitEnumeration|

.. das:function:: TemplateVisitor.preVisitEnumerationValue(self: AstVisitor; enu: EnumerationPtr; name: das_string const; value: ExpressionPtr; last: bool const)

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+--------+--------------------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`                 +
+--------+--------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                   +
+--------+--------------------------------------------------------------+
+last    +bool const                                                    +
+--------+--------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitEnumerationValue|

.. das:function:: TemplateVisitor.visitEnumerationValue(self: AstVisitor; enu: EnumerationPtr; name: das_string const; value: ExpressionPtr; last: bool const)

visitEnumerationValue returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+--------+--------------------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`                 +
+--------+--------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                   +
+--------+--------------------------------------------------------------+
+last    +bool const                                                    +
+--------+--------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitEnumerationValue|

.. das:function:: TemplateVisitor.visitEnumeration(self: AstVisitor; enu: EnumerationPtr)

visitEnumeration returns  :ref:`EnumerationPtr <alias-EnumerationPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitEnumeration|

.. das:function:: TemplateVisitor.preVisitStructure(self: AstVisitor; str: StructurePtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`       +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitStructure|

.. das:function:: TemplateVisitor.preVisitStructureField(self: AstVisitor; str: StructurePtr; decl: FieldDeclaration const; last: bool const)

+--------+------------------------------------------------------------------+
+argument+argument type                                                     +
+========+==================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+--------+------------------------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`                         +
+--------+------------------------------------------------------------------+
+decl    + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const+
+--------+------------------------------------------------------------------+
+last    +bool const                                                        +
+--------+------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitStructureField|

.. das:function:: TemplateVisitor.visitStructureField(self: AstVisitor; str: StructurePtr; decl: FieldDeclaration const; last: bool const)

+--------+------------------------------------------------------------------+
+argument+argument type                                                     +
+========+==================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+--------+------------------------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`                         +
+--------+------------------------------------------------------------------+
+decl    + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const+
+--------+------------------------------------------------------------------+
+last    +bool const                                                        +
+--------+------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitStructureField|

.. das:function:: TemplateVisitor.visitStructure(self: AstVisitor; str: StructurePtr)

visitStructure returns  :ref:`StructurePtr <alias-StructurePtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`       +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitStructure|

.. das:function:: TemplateVisitor.preVisitFunction(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitFunction|

.. das:function:: TemplateVisitor.visitFunction(self: AstVisitor; fun: FunctionPtr)

visitFunction returns  :ref:`FunctionPtr <alias-FunctionPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitFunction|

.. das:function:: TemplateVisitor.preVisitFunctionArgument(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; lastArg: bool const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+lastArg +bool const                                      +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitFunctionArgument|

.. das:function:: TemplateVisitor.visitFunctionArgument(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; lastArg: bool const)

visitFunctionArgument returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+lastArg +bool const                                      +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitFunctionArgument|

.. das:function:: TemplateVisitor.preVisitFunctionArgumentInit(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; value: ExpressionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitFunctionArgumentInit|

.. das:function:: TemplateVisitor.visitFunctionArgumentInit(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; value: ExpressionPtr)

visitFunctionArgumentInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+value   + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitFunctionArgumentInit|

.. das:function:: TemplateVisitor.preVisitFunctionBody(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitFunctionBody|

.. das:function:: TemplateVisitor.visitFunctionBody(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitFunctionBody|

.. das:function:: TemplateVisitor.preVisitExpression(self: AstVisitor; expr: ExpressionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExpression|

.. das:function:: TemplateVisitor.visitExpression(self: AstVisitor; expr: ExpressionPtr)

visitExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExpression|

.. das:function:: TemplateVisitor.preVisitExprBlock(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlock|

.. das:function:: TemplateVisitor.visitExprBlock(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

visitExprBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlock|

.. das:function:: TemplateVisitor.preVisitExprBlockArgument(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; lastArg: bool const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                        +
+--------+---------------------------------------------------------------+
+lastArg +bool const                                                     +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlockArgument|

.. das:function:: TemplateVisitor.visitExprBlockArgument(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; lastArg: bool const)

visitExprBlockArgument returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                        +
+--------+---------------------------------------------------------------+
+lastArg +bool const                                                     +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlockArgument|

.. das:function:: TemplateVisitor.preVisitExprBlockArgumentInit(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; expr: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                        +
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlockArgumentInit|

.. das:function:: TemplateVisitor.visitExprBlockArgumentInit(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; expr: ExpressionPtr)

visitExprBlockArgumentInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                        +
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlockArgumentInit|

.. das:function:: TemplateVisitor.preVisitExprBlockExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlockExpression|

.. das:function:: TemplateVisitor.visitExprBlockExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

visitExprBlockExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlockExpression|

.. das:function:: TemplateVisitor.preVisitExprBlockFinal(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlockFinal|

.. das:function:: TemplateVisitor.visitExprBlockFinal(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlockFinal|

.. das:function:: TemplateVisitor.preVisitExprBlockFinalExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBlockFinalExpression|

.. das:function:: TemplateVisitor.visitExprBlockFinalExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

visitExprBlockFinalExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBlockFinalExpression|

.. das:function:: TemplateVisitor.preVisitExprLet(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLet|

.. das:function:: TemplateVisitor.visitExprLet(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const)

visitExprLet returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLet|

.. das:function:: TemplateVisitor.preVisitExprLetVariable(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const; arg: VariablePtr; lastArg: bool const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+lastArg +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLetVariable|

.. das:function:: TemplateVisitor.visitExprLetVariable(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const; arg: VariablePtr; lastArg: bool const)

visitExprLetVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+lastArg +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLetVariable|

.. das:function:: TemplateVisitor.preVisitExprLetVariableInit(self: AstVisitor; blk: smart_ptr<ast::ExprLet> const; arg: VariablePtr; expr: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLetVariableInit|

.. das:function:: TemplateVisitor.visitExprLetVariableInit(self: AstVisitor; blk: smart_ptr<ast::ExprLet> const; arg: VariablePtr; expr: ExpressionPtr)

visitExprLetVariableInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLetVariableInit|

.. das:function:: TemplateVisitor.preVisitGlobalLet(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitGlobalLet|

.. das:function:: TemplateVisitor.visitGlobalLet(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitGlobalLet|

.. das:function:: TemplateVisitor.preVisitGlobalLetVariable(self: AstVisitor; arg: VariablePtr; lastArg: bool const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+lastArg +bool const                                      +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitGlobalLetVariable|

.. das:function:: TemplateVisitor.visitGlobalLetVariable(self: AstVisitor; arg: VariablePtr; lastArg: bool const)

visitGlobalLetVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+lastArg +bool const                                      +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitGlobalLetVariable|

.. das:function:: TemplateVisitor.preVisitGlobalLetVariableInit(self: AstVisitor; arg: VariablePtr; expr: ExpressionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitGlobalLetVariableInit|

.. das:function:: TemplateVisitor.visitGlobalLetVariableInit(self: AstVisitor; arg: VariablePtr; expr: ExpressionPtr)

visitGlobalLetVariableInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitGlobalLetVariableInit|

.. das:function:: TemplateVisitor.preVisitExprStringBuilder(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprStringBuilder|

.. das:function:: TemplateVisitor.visitExprStringBuilder(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const)

visitExprStringBuilder returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprStringBuilder|

.. das:function:: TemplateVisitor.preVisitExprStringBuilderElement(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const; elem: ExpressionPtr; last: bool const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+
+elem    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+--------+-------------------------------------------------------------------------------+
+last    +bool const                                                                     +
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprStringBuilderElement|

.. das:function:: TemplateVisitor.visitExprStringBuilderElement(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const; elem: ExpressionPtr; last: bool const)

visitExprStringBuilderElement returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+
+elem    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+--------+-------------------------------------------------------------------------------+
+last    +bool const                                                                     +
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprStringBuilderElement|

.. das:function:: TemplateVisitor.preVisitExprNew(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNew|

.. das:function:: TemplateVisitor.visitExprNew(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const)

visitExprNew returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprNew|

.. das:function:: TemplateVisitor.preVisitExprNewArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const; arg: ExpressionPtr; last: bool const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNewArgument|

.. das:function:: TemplateVisitor.visitExprNewArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const; arg: ExpressionPtr; last: bool const)

visitExprNewArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprNewArgument|

.. das:function:: TemplateVisitor.preVisitExprNamedCall(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNamedCall|

.. das:function:: TemplateVisitor.visitExprNamedCall(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const)

visitExprNamedCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprNamedCall|

.. das:function:: TemplateVisitor.preVisitExprNamedCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const; arg: MakeFieldDeclPtr; last: bool const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+
+arg     + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                      +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNamedCallArgument|

.. das:function:: TemplateVisitor.visitExprNamedCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const; arg: MakeFieldDeclPtr; last: bool const)

visitExprNamedCallArgument returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+
+arg     + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                      +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprNamedCallArgument|

.. das:function:: TemplateVisitor.preVisitExprLooksLikeCall(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLooksLikeCall|

.. das:function:: TemplateVisitor.visitExprLooksLikeCall(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const)

visitExprLooksLikeCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLooksLikeCall|

.. das:function:: TemplateVisitor.preVisitExprLooksLikeCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const; arg: ExpressionPtr; last: bool const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+--------+-------------------------------------------------------------------------------+
+last    +bool const                                                                     +
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLooksLikeCallArgument|

.. das:function:: TemplateVisitor.visitExprLooksLikeCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const; arg: ExpressionPtr; last: bool const)

visitExprLooksLikeCallArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+--------+-------------------------------------------------------------------------------+
+last    +bool const                                                                     +
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLooksLikeCallArgument|

.. das:function:: TemplateVisitor.preVisitExprCall(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCall|

.. das:function:: TemplateVisitor.visitExprCall(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const)

visitExprCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprCall|

.. das:function:: TemplateVisitor.preVisitExprCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const; arg: ExpressionPtr; last: bool const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+
+last    +bool const                                                   +
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCallArgument|

.. das:function:: TemplateVisitor.visitExprCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const; arg: ExpressionPtr; last: bool const)

visitExprCallArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+
+arg     + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+
+last    +bool const                                                   +
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprCallArgument|

.. das:function:: TemplateVisitor.preVisitExprNullCoalescing(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNullCoalescing|

.. das:function:: TemplateVisitor.visitExprNullCoalescing(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const)

visitExprNullCoalescing returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprNullCoalescing|

.. das:function:: TemplateVisitor.preVisitExprNullCoalescingDefault(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const; defval: ExpressionPtr)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+
+defval  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                      +
+--------+---------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprNullCoalescingDefault|

.. das:function:: TemplateVisitor.preVisitExprAt(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAt|

.. das:function:: TemplateVisitor.visitExprAt(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const)

visitExprAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprAt|

.. das:function:: TemplateVisitor.preVisitExprAtIndex(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; index: ExpressionPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+index   + :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAtIndex|

.. das:function:: TemplateVisitor.preVisitExprSafeAt(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAt> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprSafeAt|

.. das:function:: TemplateVisitor.visitExprSafeAt(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAt> const)

visitExprSafeAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprSafeAt|

.. das:function:: TemplateVisitor.preVisitExprSafeAtIndex(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; index: ExpressionPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+index   + :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprSafeAtIndex|

.. das:function:: TemplateVisitor.preVisitExprIs(self: AstVisitor; expr: smart_ptr<ast::ExprIs> const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIs|

.. das:function:: TemplateVisitor.visitExprIs(self: AstVisitor; expr: smart_ptr<ast::ExprIs> const)

visitExprIs returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprIs|

.. das:function:: TemplateVisitor.preVisitExprIsType(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; typeDecl: TypeDeclPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+typeDecl+ :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                  +
+--------+---------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIsType|

.. das:function:: TemplateVisitor.preVisitExprOp2(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp2|

.. das:function:: TemplateVisitor.visitExprOp2(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const)

visitExprOp2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprOp2|

.. das:function:: TemplateVisitor.preVisitExprOp2Right(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const; right: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp2Right|

.. das:function:: TemplateVisitor.preVisitExprOp3(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp3|

.. das:function:: TemplateVisitor.visitExprOp3(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const)

visitExprOp3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprOp3|

.. das:function:: TemplateVisitor.preVisitExprOp3Left(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const; left: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+
+left    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp3Left|

.. das:function:: TemplateVisitor.preVisitExprOp3Right(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const; right: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp3Right|

.. das:function:: TemplateVisitor.preVisitExprCopy(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCopy|

.. das:function:: TemplateVisitor.visitExprCopy(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const)

visitExprCopy returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprCopy|

.. das:function:: TemplateVisitor.preVisitExprCopyRight(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCopyRight|

.. das:function:: TemplateVisitor.preVisitExprMove(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMove|

.. das:function:: TemplateVisitor.visitExprMove(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const)

visitExprMove returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMove|

.. das:function:: TemplateVisitor.preVisitExprMoveRight(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMoveRight|

.. das:function:: TemplateVisitor.preVisitExprClone(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprClone|

.. das:function:: TemplateVisitor.visitExprClone(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const)

visitExprClone returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprClone|

.. das:function:: TemplateVisitor.preVisitExprCloneRight(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCloneRight|

.. das:function:: TemplateVisitor.preVisitExprWith(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprWith|

.. das:function:: TemplateVisitor.visitExprWith(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const)

visitExprWith returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprWith|

.. das:function:: TemplateVisitor.preVisitExprWithBody(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprWithBody|

.. das:function:: TemplateVisitor.preVisitExprWhile(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprWhile|

.. das:function:: TemplateVisitor.visitExprWhile(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const)

visitExprWhile returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprWhile|

.. das:function:: TemplateVisitor.preVisitExprWhileBody(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprWhileBody|

.. das:function:: TemplateVisitor.preVisitExprTryCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprTryCatch|

.. das:function:: TemplateVisitor.visitExprTryCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const)

visitExprTryCatch returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprTryCatch|

.. das:function:: TemplateVisitor.preVisitExprTryCatchCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                          +
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprTryCatchCatch|

.. das:function:: TemplateVisitor.preVisitExprIfThenElse(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIfThenElse|

.. das:function:: TemplateVisitor.visitExprIfThenElse(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const)

visitExprIfThenElse returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprIfThenElse|

.. das:function:: TemplateVisitor.preVisitExprIfThenElseIfBlock(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const; ifBlock: ExpressionPtr)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+
+ifBlock + :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIfThenElseIfBlock|

.. das:function:: TemplateVisitor.preVisitExprIfThenElseElseBlock(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const; elseBlock: ExpressionPtr)

+---------+-------------------------------------------------------------------------+
+argument +argument type                                                            +
+=========+=========================================================================+
+self     + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+---------+-------------------------------------------------------------------------+
+expr     +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+---------+-------------------------------------------------------------------------+
+elseBlock+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+---------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIfThenElseElseBlock|

.. das:function:: TemplateVisitor.preVisitExprFor(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprFor|

.. das:function:: TemplateVisitor.visitExprFor(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

visitExprFor returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprFor|

.. das:function:: TemplateVisitor.preVisitExprForVariable(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; svar: VariablePtr; last: bool const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+svar    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprForVariable|

.. das:function:: TemplateVisitor.visitExprForVariable(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; svar: VariablePtr; last: bool const)

visitExprForVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+svar    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprForVariable|

.. das:function:: TemplateVisitor.preVisitExprForSource(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; source: ExpressionPtr; last: bool const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+source  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprForSource|

.. das:function:: TemplateVisitor.visitExprForSource(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; source: ExpressionPtr; last: bool const)

visitExprForSource returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+
+source  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+
+last    +bool const                                                 +
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprForSource|

.. das:function:: TemplateVisitor.preVisitExprForStack(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprForStack|

.. das:function:: TemplateVisitor.preVisitExprForBody(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprForBody|

.. das:function:: TemplateVisitor.preVisitExprMakeVariant(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeVariant|

.. das:function:: TemplateVisitor.visitExprMakeVariant(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const)

visitExprMakeVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeVariant|

.. das:function:: TemplateVisitor.preVisitExprMakeVariantField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+
+index   +int const                                                                  +
+--------+---------------------------------------------------------------------------+
+decl    + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                          +
+--------+---------------------------------------------------------------------------+
+last    +bool const                                                                 +
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeVariantField|

.. das:function:: TemplateVisitor.visitExprMakeVariantField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

visitExprMakeVariantField returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+
+index   +int const                                                                  +
+--------+---------------------------------------------------------------------------+
+decl    + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                          +
+--------+---------------------------------------------------------------------------+
+last    +bool const                                                                 +
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeVariantField|

.. das:function:: TemplateVisitor.preVisitExprMakeStruct(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeStruct|

.. das:function:: TemplateVisitor.visitExprMakeStruct(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const)

visitExprMakeStruct returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeStruct|

.. das:function:: TemplateVisitor.preVisitExprMakeStructIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; last: bool const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+
+index   +int const                                                                +
+--------+-------------------------------------------------------------------------+
+last    +bool const                                                               +
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeStructIndex|

.. das:function:: TemplateVisitor.visitExprMakeStructIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; last: bool const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+
+index   +int const                                                                +
+--------+-------------------------------------------------------------------------+
+last    +bool const                                                               +
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeStructIndex|

.. das:function:: TemplateVisitor.preVisitExprMakeStructField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+
+index   +int const                                                                +
+--------+-------------------------------------------------------------------------+
+decl    + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                        +
+--------+-------------------------------------------------------------------------+
+last    +bool const                                                               +
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeStructField|

.. das:function:: TemplateVisitor.visitExprMakeStructField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

visitExprMakeStructField returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+
+index   +int const                                                                +
+--------+-------------------------------------------------------------------------+
+decl    + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                        +
+--------+-------------------------------------------------------------------------+
+last    +bool const                                                               +
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeStructField|

.. das:function:: TemplateVisitor.preVisitExprMakeArray(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeArray|

.. das:function:: TemplateVisitor.visitExprMakeArray(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const)

visitExprMakeArray returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeArray|

.. das:function:: TemplateVisitor.preVisitExprMakeArrayIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const; index: int const; init: ExpressionPtr; last: bool const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+
+index   +int const                                                              +
+--------+-----------------------------------------------------------------------+
+init    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeArrayIndex|

.. das:function:: TemplateVisitor.visitExprMakeArrayIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const; index: int const; init: ExpressionPtr; last: bool const)

visitExprMakeArrayIndex returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+
+index   +int const                                                              +
+--------+-----------------------------------------------------------------------+
+init    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeArrayIndex|

.. das:function:: TemplateVisitor.preVisitExprMakeTuple(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeTuple|

.. das:function:: TemplateVisitor.visitExprMakeTuple(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const)

visitExprMakeTuple returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeTuple|

.. das:function:: TemplateVisitor.preVisitExprMakeTupleIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const; index: int const; init: ExpressionPtr; last: bool const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+
+index   +int const                                                              +
+--------+-----------------------------------------------------------------------+
+init    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeTupleIndex|

.. das:function:: TemplateVisitor.visitExprMakeTupleIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const; index: int const; init: ExpressionPtr; last: bool const)

visitExprMakeTupleIndex returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+
+index   +int const                                                              +
+--------+-----------------------------------------------------------------------+
+init    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+--------+-----------------------------------------------------------------------+
+last    +bool const                                                             +
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeTupleIndex|

.. das:function:: TemplateVisitor.preVisitExprArrayComprehension(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprArrayComprehension|

.. das:function:: TemplateVisitor.visitExprArrayComprehension(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const)

visitExprArrayComprehension returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprArrayComprehension|

.. das:function:: TemplateVisitor.preVisitExprArrayComprehensionSubexpr(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const; subexrp: ExpressionPtr)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+
+subexrp + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+--------+-----------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprArrayComprehensionSubexpr|

.. das:function:: TemplateVisitor.preVisitExprArrayComprehensionWhere(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const; filter: ExpressionPtr)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+
+filter  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+--------+-----------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprArrayComprehensionWhere|

.. das:function:: TemplateVisitor.preVisitExprTypeInfo(self: AstVisitor; expr: smart_ptr<ast::ExprTypeInfo> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprTypeInfo|

.. das:function:: TemplateVisitor.visitExprTypeInfo(self: AstVisitor; expr: smart_ptr<ast::ExprTypeInfo> const)

visitExprTypeInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprTypeInfo|

.. das:function:: TemplateVisitor.preVisitExprPtr2Ref(self: AstVisitor; expr: smart_ptr<ast::ExprPtr2Ref> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprPtr2Ref|

.. das:function:: TemplateVisitor.visitExprPtr2Ref(self: AstVisitor; expr: smart_ptr<ast::ExprPtr2Ref> const)

visitExprPtr2Ref returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprPtr2Ref|

.. das:function:: TemplateVisitor.preVisitExprLabel(self: AstVisitor; expr: smart_ptr<ast::ExprLabel> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprLabel|

.. das:function:: TemplateVisitor.visitExprLabel(self: AstVisitor; expr: smart_ptr<ast::ExprLabel> const)

visitExprLabel returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprLabel|

.. das:function:: TemplateVisitor.preVisitExprGoto(self: AstVisitor; expr: smart_ptr<ast::ExprGoto> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprGoto|

.. das:function:: TemplateVisitor.visitExprGoto(self: AstVisitor; expr: smart_ptr<ast::ExprGoto> const)

visitExprGoto returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprGoto|

.. das:function:: TemplateVisitor.preVisitExprRef2Value(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Value> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprRef2Value|

.. das:function:: TemplateVisitor.visitExprRef2Value(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Value> const)

visitExprRef2Value returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprRef2Value|

.. das:function:: TemplateVisitor.preVisitExprRef2Ptr(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Ptr> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprRef2Ptr|

.. das:function:: TemplateVisitor.visitExprRef2Ptr(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Ptr> const)

visitExprRef2Ptr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprRef2Ptr|

.. das:function:: TemplateVisitor.preVisitExprAddr(self: AstVisitor; expr: smart_ptr<ast::ExprAddr> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAddr|

.. das:function:: TemplateVisitor.visitExprAddr(self: AstVisitor; expr: smart_ptr<ast::ExprAddr> const)

visitExprAddr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprAddr|

.. das:function:: TemplateVisitor.preVisitExprAssert(self: AstVisitor; expr: smart_ptr<ast::ExprAssert> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAssert|

.. das:function:: TemplateVisitor.visitExprAssert(self: AstVisitor; expr: smart_ptr<ast::ExprAssert> const)

visitExprAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprAssert|

.. das:function:: TemplateVisitor.preVisitExprStaticAssert(self: AstVisitor; expr: smart_ptr<ast::ExprStaticAssert> const)

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+--------+-----------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprStaticAssert|

.. das:function:: TemplateVisitor.visitExprStaticAssert(self: AstVisitor; expr: smart_ptr<ast::ExprStaticAssert> const)

visitExprStaticAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+--------+-----------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprStaticAssert|

.. das:function:: TemplateVisitor.preVisitExprQuote(self: AstVisitor; expr: smart_ptr<ast::ExprQuote> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprQuote <handle-ast-ExprQuote>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprQuote|

.. das:function:: TemplateVisitor.visitExprQuote(self: AstVisitor; expr: smart_ptr<ast::ExprQuote> const)

visitExprQuote returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprQuote <handle-ast-ExprQuote>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprQuote|

.. das:function:: TemplateVisitor.preVisitExprDebug(self: AstVisitor; expr: smart_ptr<ast::ExprDebug> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprDebug|

.. das:function:: TemplateVisitor.visitExprDebug(self: AstVisitor; expr: smart_ptr<ast::ExprDebug> const)

visitExprDebug returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprDebug|

.. das:function:: TemplateVisitor.preVisitExprInvoke(self: AstVisitor; expr: smart_ptr<ast::ExprInvoke> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprInvoke|

.. das:function:: TemplateVisitor.visitExprInvoke(self: AstVisitor; expr: smart_ptr<ast::ExprInvoke> const)

visitExprInvoke returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprInvoke|

.. das:function:: TemplateVisitor.preVisitExprErase(self: AstVisitor; expr: smart_ptr<ast::ExprErase> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprErase|

.. das:function:: TemplateVisitor.visitExprErase(self: AstVisitor; expr: smart_ptr<ast::ExprErase> const)

visitExprErase returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprErase|

.. das:function:: TemplateVisitor.preVisitExprFind(self: AstVisitor; expr: smart_ptr<ast::ExprFind> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprFind|

.. das:function:: TemplateVisitor.visitExprFind(self: AstVisitor; expr: smart_ptr<ast::ExprFind> const)

visitExprFind returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprFind|

.. das:function:: TemplateVisitor.preVisitExprKeyExists(self: AstVisitor; expr: smart_ptr<ast::ExprKeyExists> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprKeyExists|

.. das:function:: TemplateVisitor.visitExprKeyExists(self: AstVisitor; expr: smart_ptr<ast::ExprKeyExists> const)

visitExprKeyExists returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprKeyExists|

.. das:function:: TemplateVisitor.preVisitExprAscend(self: AstVisitor; expr: smart_ptr<ast::ExprAscend> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAscend|

.. das:function:: TemplateVisitor.visitExprAscend(self: AstVisitor; expr: smart_ptr<ast::ExprAscend> const)

visitExprAscend returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprAscend|

.. das:function:: TemplateVisitor.preVisitExprCast(self: AstVisitor; expr: smart_ptr<ast::ExprCast> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCast|

.. das:function:: TemplateVisitor.visitExprCast(self: AstVisitor; expr: smart_ptr<ast::ExprCast> const)

visitExprCast returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+--------+-------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprCast|

.. das:function:: TemplateVisitor.preVisitExprDelete(self: AstVisitor; expr: smart_ptr<ast::ExprDelete> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprDelete|

.. das:function:: TemplateVisitor.visitExprDelete(self: AstVisitor; expr: smart_ptr<ast::ExprDelete> const)

visitExprDelete returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprDelete|

.. das:function:: TemplateVisitor.preVisitExprVar(self: AstVisitor; expr: smart_ptr<ast::ExprVar> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprVar|

.. das:function:: TemplateVisitor.visitExprVar(self: AstVisitor; expr: smart_ptr<ast::ExprVar> const)

visitExprVar returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprVar|

.. das:function:: TemplateVisitor.preVisitExprField(self: AstVisitor; expr: smart_ptr<ast::ExprField> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprField|

.. das:function:: TemplateVisitor.visitExprField(self: AstVisitor; expr: smart_ptr<ast::ExprField> const)

visitExprField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprField|

.. das:function:: TemplateVisitor.preVisitExprSafeField(self: AstVisitor; expr: smart_ptr<ast::ExprSafeField> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprSafeField|

.. das:function:: TemplateVisitor.visitExprSafeField(self: AstVisitor; expr: smart_ptr<ast::ExprSafeField> const)

visitExprSafeField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprSafeField|

.. das:function:: TemplateVisitor.preVisitExprSwizzle(self: AstVisitor; expr: smart_ptr<ast::ExprSwizzle> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprSwizzle|

.. das:function:: TemplateVisitor.visitExprSwizzle(self: AstVisitor; expr: smart_ptr<ast::ExprSwizzle> const)

visitExprSwizzle returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprSwizzle|

.. das:function:: TemplateVisitor.preVisitExprIsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprIsVariant> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprIsVariant|

.. das:function:: TemplateVisitor.visitExprIsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprIsVariant> const)

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprIsVariant|

.. das:function:: TemplateVisitor.preVisitExprAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprAsVariant> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprAsVariant|

.. das:function:: TemplateVisitor.visitExprAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprAsVariant> const)

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprAsVariant|

.. das:function:: TemplateVisitor.preVisitExprSafeAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAsVariant> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprSafeAsVariant|

.. das:function:: TemplateVisitor.visitExprSafeAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAsVariant> const)

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprSafeAsVariant|

.. das:function:: TemplateVisitor.preVisitExprOp1(self: AstVisitor; expr: smart_ptr<ast::ExprOp1> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprOp1|

.. das:function:: TemplateVisitor.visitExprOp1(self: AstVisitor; expr: smart_ptr<ast::ExprOp1> const)

visitExprOp1 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+--------+-----------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprOp1|

.. das:function:: TemplateVisitor.preVisitExprReturn(self: AstVisitor; expr: smart_ptr<ast::ExprReturn> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprReturn|

.. das:function:: TemplateVisitor.visitExprReturn(self: AstVisitor; expr: smart_ptr<ast::ExprReturn> const)

visitExprReturn returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprReturn|

.. das:function:: TemplateVisitor.preVisitExprYield(self: AstVisitor; expr: smart_ptr<ast::ExprYield> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprYield|

.. das:function:: TemplateVisitor.visitExprYield(self: AstVisitor; expr: smart_ptr<ast::ExprYield> const)

visitExprYield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprYield|

.. das:function:: TemplateVisitor.preVisitExprBreak(self: AstVisitor; expr: smart_ptr<ast::ExprBreak> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprBreak|

.. das:function:: TemplateVisitor.visitExprBreak(self: AstVisitor; expr: smart_ptr<ast::ExprBreak> const)

visitExprBreak returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprBreak|

.. das:function:: TemplateVisitor.preVisitExprContinue(self: AstVisitor; expr: smart_ptr<ast::ExprContinue> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprContinue|

.. das:function:: TemplateVisitor.visitExprContinue(self: AstVisitor; expr: smart_ptr<ast::ExprContinue> const)

visitExprContinue returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprContinue|

.. das:function:: TemplateVisitor.preVisitExprMakeBlock(self: AstVisitor; expr: smart_ptr<ast::ExprMakeBlock> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeBlock|

.. das:function:: TemplateVisitor.visitExprMakeBlock(self: AstVisitor; expr: smart_ptr<ast::ExprMakeBlock> const)

visitExprMakeBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeBlock|

.. das:function:: TemplateVisitor.preVisitExprMakeGenerator(self: AstVisitor; expr: smart_ptr<ast::ExprMakeGenerator> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMakeGenerator|

.. das:function:: TemplateVisitor.visitExprMakeGenerator(self: AstVisitor; expr: smart_ptr<ast::ExprMakeGenerator> const)

visitExprMakeGenerator returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMakeGenerator|

.. das:function:: TemplateVisitor.preVisitExprMemZero(self: AstVisitor; expr: smart_ptr<ast::ExprMemZero> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprMemZero|

.. das:function:: TemplateVisitor.visitExprMemZero(self: AstVisitor; expr: smart_ptr<ast::ExprMemZero> const)

visitExprMemZero returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+--------+-------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprMemZero|

.. das:function:: TemplateVisitor.preVisitExprConst(self: AstVisitor; expr: smart_ptr<ast::ExprConst> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConst|

.. das:function:: TemplateVisitor.visitExprConst(self: AstVisitor; expr: smart_ptr<ast::ExprConst> const)

visitExprConst returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+--------+---------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConst|

.. das:function:: TemplateVisitor.preVisitExprConstPtr(self: AstVisitor; expr: smart_ptr<ast::ExprConstPtr> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstPtr|

.. das:function:: TemplateVisitor.visitExprConstPtr(self: AstVisitor; expr: smart_ptr<ast::ExprConstPtr> const)

visitExprConstPtr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstPtr|

.. das:function:: TemplateVisitor.preVisitExprConstEnumeration(self: AstVisitor; expr: smart_ptr<ast::ExprConstEnumeration> const)

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+--------+-------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+--------+-------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstEnumeration|

.. das:function:: TemplateVisitor.visitExprConstEnumeration(self: AstVisitor; expr: smart_ptr<ast::ExprConstEnumeration> const)

visitExprConstEnumeration returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+--------+-------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+--------+-------------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstEnumeration|

.. das:function:: TemplateVisitor.preVisitExprConstBitfield(self: AstVisitor; expr: smart_ptr<ast::ExprConstBitfield> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstBitfield|

.. das:function:: TemplateVisitor.visitExprConstBitfield(self: AstVisitor; expr: smart_ptr<ast::ExprConstBitfield> const)

visitExprConstBitfield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+--------+-------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstBitfield|

.. das:function:: TemplateVisitor.preVisitExprConstInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt8> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt8|

.. das:function:: TemplateVisitor.visitExprConstInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt8> const)

visitExprConstInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt8|

.. das:function:: TemplateVisitor.preVisitExprConstInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt16> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt16|

.. das:function:: TemplateVisitor.visitExprConstInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt16> const)

visitExprConstInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt16|

.. das:function:: TemplateVisitor.preVisitExprConstInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt64> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt64|

.. das:function:: TemplateVisitor.visitExprConstInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt64> const)

visitExprConstInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt64|

.. das:function:: TemplateVisitor.preVisitExprConstInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt|

.. das:function:: TemplateVisitor.visitExprConstInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt> const)

visitExprConstInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+--------+---------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt|

.. das:function:: TemplateVisitor.preVisitExprConstInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt2> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt2|

.. das:function:: TemplateVisitor.visitExprConstInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt2> const)

visitExprConstInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt2|

.. das:function:: TemplateVisitor.preVisitExprConstInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt3> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt3|

.. das:function:: TemplateVisitor.visitExprConstInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt3> const)

visitExprConstInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt3|

.. das:function:: TemplateVisitor.preVisitExprConstInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt4> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstInt4|

.. das:function:: TemplateVisitor.visitExprConstInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt4> const)

visitExprConstInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstInt4|

.. das:function:: TemplateVisitor.preVisitExprConstUInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt8> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt8|

.. das:function:: TemplateVisitor.visitExprConstUInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt8> const)

visitExprConstUInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt8|

.. das:function:: TemplateVisitor.preVisitExprConstUInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt16> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt16|

.. das:function:: TemplateVisitor.visitExprConstUInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt16> const)

visitExprConstUInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt16|

.. das:function:: TemplateVisitor.preVisitExprConstUInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt64> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt64|

.. das:function:: TemplateVisitor.visitExprConstUInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt64> const)

visitExprConstUInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt64|

.. das:function:: TemplateVisitor.preVisitExprConstUInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt|

.. das:function:: TemplateVisitor.visitExprConstUInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt> const)

visitExprConstUInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt|

.. das:function:: TemplateVisitor.preVisitExprConstUInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt2> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt2|

.. das:function:: TemplateVisitor.visitExprConstUInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt2> const)

visitExprConstUInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt2|

.. das:function:: TemplateVisitor.preVisitExprConstUInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt3> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt3|

.. das:function:: TemplateVisitor.visitExprConstUInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt3> const)

visitExprConstUInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt3|

.. das:function:: TemplateVisitor.preVisitExprConstUInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt4> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstUInt4|

.. das:function:: TemplateVisitor.visitExprConstUInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt4> const)

visitExprConstUInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstUInt4|

.. das:function:: TemplateVisitor.preVisitExprConstRange(self: AstVisitor; expr: smart_ptr<ast::ExprConstRange> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstRange|

.. das:function:: TemplateVisitor.visitExprConstRange(self: AstVisitor; expr: smart_ptr<ast::ExprConstRange> const)

visitExprConstRange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstRange|

.. das:function:: TemplateVisitor.preVisitExprConstURange(self: AstVisitor; expr: smart_ptr<ast::ExprConstURange> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstURange|

.. das:function:: TemplateVisitor.visitExprConstURange(self: AstVisitor; expr: smart_ptr<ast::ExprConstURange> const)

visitExprConstURange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstURange|

.. das:function:: TemplateVisitor.preVisitExprConstBool(self: AstVisitor; expr: smart_ptr<ast::ExprConstBool> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstBool|

.. das:function:: TemplateVisitor.visitExprConstBool(self: AstVisitor; expr: smart_ptr<ast::ExprConstBool> const)

visitExprConstBool returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstBool|

.. das:function:: TemplateVisitor.preVisitExprConstFloat(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstFloat|

.. das:function:: TemplateVisitor.visitExprConstFloat(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat> const)

visitExprConstFloat returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+--------+-------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstFloat|

.. das:function:: TemplateVisitor.preVisitExprConstFloat2(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat2> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstFloat2|

.. das:function:: TemplateVisitor.visitExprConstFloat2(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat2> const)

visitExprConstFloat2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstFloat2|

.. das:function:: TemplateVisitor.preVisitExprConstFloat3(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat3> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstFloat3|

.. das:function:: TemplateVisitor.visitExprConstFloat3(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat3> const)

visitExprConstFloat3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstFloat3|

.. das:function:: TemplateVisitor.preVisitExprConstFloat4(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat4> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstFloat4|

.. das:function:: TemplateVisitor.visitExprConstFloat4(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat4> const)

visitExprConstFloat4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstFloat4|

.. das:function:: TemplateVisitor.preVisitExprConstString(self: AstVisitor; expr: smart_ptr<ast::ExprConstString> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstString|

.. das:function:: TemplateVisitor.visitExprConstString(self: AstVisitor; expr: smart_ptr<ast::ExprConstString> const)

visitExprConstString returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstString|

.. das:function:: TemplateVisitor.preVisitExprConstDouble(self: AstVisitor; expr: smart_ptr<ast::ExprConstDouble> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprConstDouble|

.. das:function:: TemplateVisitor.visitExprConstDouble(self: AstVisitor; expr: smart_ptr<ast::ExprConstDouble> const)

visitExprConstDouble returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprConstDouble|

.. das:function:: TemplateVisitor.preVisitExprFakeContext(self: AstVisitor; expr: smart_ptr<ast::ExprFakeContext> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprFakeContext|

.. das:function:: TemplateVisitor.visitExprFakeContext(self: AstVisitor; expr: smart_ptr<ast::ExprFakeContext> const)

visitExprFakeContext returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+--------+---------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprFakeContext|

.. das:function:: TemplateVisitor.preVisitExprFakeLineInfo(self: AstVisitor; expr: smart_ptr<ast::ExprFakeLineInfo> const)

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+--------+-----------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprFakeLineInfo|

.. das:function:: TemplateVisitor.visitExprFakeLineInfo(self: AstVisitor; expr: smart_ptr<ast::ExprFakeLineInfo> const)

visitExprFakeLineInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+--------+-----------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprFakeLineInfo|

.. das:function:: TemplateVisitor.preVisitExprReader(self: AstVisitor; expr: smart_ptr<ast::ExprReader> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprReader|

.. das:function:: TemplateVisitor.visitExprReader(self: AstVisitor; expr: smart_ptr<ast::ExprReader> const)

visitExprReader returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprReader|

.. das:function:: TemplateVisitor.preVisitExprUnsafe(self: AstVisitor; expr: smart_ptr<ast::ExprUnsafe> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprUnsafe <handle-ast-ExprUnsafe>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprUnsafe|

.. das:function:: TemplateVisitor.visitExprUnsafe(self: AstVisitor; expr: smart_ptr<ast::ExprUnsafe> const)

visitExprUnsafe returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprUnsafe <handle-ast-ExprUnsafe>` > const+
+--------+-----------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprUnsafe|

.. das:function:: TemplateVisitor.preVisitExprCallMacro(self: AstVisitor; expr: smart_ptr<ast::ExprCallMacro> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.preVisitExprCallMacro|

.. das:function:: TemplateVisitor.visitExprCallMacro(self: AstVisitor; expr: smart_ptr<ast::ExprCallMacro> const)

visitExprCallMacro returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.visitExprCallMacro|

.. das:function:: TemplateVisitor.replaceAlias(self: TemplateVisitor; typ: TypeDeclPtr)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+self    + :ref:`templates_boost::TemplateVisitor <struct-templates_boost-TemplateVisitor>` +
+--------+----------------------------------------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                                           +
+--------+----------------------------------------------------------------------------------+


|method-templates_boost-TemplateVisitor.replaceAlias|

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


