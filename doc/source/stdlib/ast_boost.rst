
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

+++++++
Classes
+++++++

.. _struct-ast_boost-BetterRttiVisitor:

.. das:class:: BetterRttiVisitor : AstVariantMacro

to be documented

.. das:method:: BetterRttiVisitor.visitExprIsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr 

visitExprIsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle_ast_ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: BetterRttiVisitor.visitExprAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr 

visitExprAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle_ast_ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: BetterRttiVisitor.visitExprSafeAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr 

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+----+-------------------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                                          +
+----+-------------------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                              +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle_ast_ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-MacroMacro:

.. das:class:: MacroMacro : AstFunctionAnnotation

to be documented

.. das:method:: MacroMacro.transform ( self:AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string )  : ExpressionPtr 

transform returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+------+----------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+------+----------------------------------------------------------------------+
+call  +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >               +
+------+----------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`               +
+------+----------------------------------------------------------------------+


to be documented


.. das:method:: MacroMacro.apply ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+------+--------------------------------------------------------------------------------+
+func  + :ref:`FunctionPtr <alias_FunctionPtr>`                                         +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: MacroMacro.finish ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; progArgs:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+--------+--------------------------------------------------------------------------------+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias_FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+--------+--------------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-SetupAnyAnnotation:

.. das:class:: SetupAnyAnnotation : AstStructureAnnotation

to be documented

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:method:: SetupAnyAnnotation.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupAnyAnnotation.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupAnyAnnotation.setup_call ( self:SetupAnyAnnotation; st:StructurePtr; cll:smart_ptr<ast::ExprCall> )  

method arguments are

+----+----------------------------------------------------------------------------+
+self+ :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+----+----------------------------------------------------------------------------+
+st  + :ref:`StructurePtr <alias_StructurePtr>`                                   +
+----+----------------------------------------------------------------------------+
+cll +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >                     +
+----+----------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-SetupFunctionAnnotatoin:

.. das:class:: SetupFunctionAnnotatoin : SetupAnyAnnotation

to be documented

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:method:: SetupFunctionAnnotatoin.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupFunctionAnnotatoin.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupFunctionAnnotatoin.setup_call ( self:SetupAnyAnnotation; st:StructurePtr; cll:smart_ptr<ast::ExprCall> )  

method arguments are

+----+----------------------------------------------------------------------------+
+self+ :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+----+----------------------------------------------------------------------------+
+st  + :ref:`StructurePtr <alias_StructurePtr>`                                   +
+----+----------------------------------------------------------------------------+
+cll +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >                     +
+----+----------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-SetupReaderMacro:

.. das:class:: SetupReaderMacro : SetupAnyAnnotation

to be documented

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:method:: SetupReaderMacro.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupReaderMacro.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupReaderMacro.setup_call ( self:SetupAnyAnnotation; st:StructurePtr; cll:smart_ptr<ast::ExprCall> )  

method arguments are

+----+----------------------------------------------------------------------------+
+self+ :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+----+----------------------------------------------------------------------------+
+st  + :ref:`StructurePtr <alias_StructurePtr>`                                   +
+----+----------------------------------------------------------------------------+
+cll +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >                     +
+----+----------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-SetupVariantMacro:

.. das:class:: SetupVariantMacro : SetupAnyAnnotation

to be documented

it defines as follows

  annotation_function_call : string
  name                     : string

.. das:method:: SetupVariantMacro.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupVariantMacro.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: SetupVariantMacro.setup_call ( self:SetupAnyAnnotation; st:StructurePtr; cll:smart_ptr<ast::ExprCall> )  

method arguments are

+----+----------------------------------------------------------------------------+
+self+ :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+----+----------------------------------------------------------------------------+
+st  + :ref:`StructurePtr <alias_StructurePtr>`                                   +
+----+----------------------------------------------------------------------------+
+cll +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >                     +
+----+----------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-TagFunctionAnnotation:

.. das:class:: TagFunctionAnnotation : AstFunctionAnnotation

to be documented

.. das:method:: TagFunctionAnnotation.transform ( self:AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string )  : ExpressionPtr 

transform returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+------+----------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+------+----------------------------------------------------------------------+
+call  +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >               +
+------+----------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`               +
+------+----------------------------------------------------------------------+


to be documented


.. das:method:: TagFunctionAnnotation.apply ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+------+--------------------------------------------------------------------------------+
+func  + :ref:`FunctionPtr <alias_FunctionPtr>`                                         +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: TagFunctionAnnotation.finish ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; progArgs:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+--------+--------------------------------------------------------------------------------+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias_FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+--------+--------------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-TagFunctionMacro:

.. das:class:: TagFunctionMacro : SetupAnyAnnotation

to be documented

it defines as follows

  annotation_function_call : string
  name                     : string
  tag                      : string

.. das:method:: TagFunctionMacro.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: TagFunctionMacro.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: TagFunctionMacro.setup_call ( self:SetupAnyAnnotation; st:StructurePtr; cll:smart_ptr<ast::ExprCall> )  

method arguments are

+----+----------------------------------------------------------------------------+
+self+ :ref:`ast_boost::SetupAnyAnnotation <struct-ast_boost-SetupAnyAnnotation>` +
+----+----------------------------------------------------------------------------+
+st  + :ref:`StructurePtr <alias_StructurePtr>`                                   +
+----+----------------------------------------------------------------------------+
+cll +smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` >                     +
+----+----------------------------------------------------------------------------+


to be documented



.. _struct-ast_boost-TagStructureAnnotation:

.. das:class:: TagStructureAnnotation : AstStructureAnnotation

to be documented

.. das:method:: TagStructureAnnotation.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

apply returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented


.. das:method:: TagStructureAnnotation.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

finish returns bool

method arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias_StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle_ast_ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+------+--------------------------------------------------------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: convert_to_expression ( value:auto&; at:LineInfo const )  : auto

convert_to_expression returns auto



function arguments are

+-----+----------------------------------------------------+
+value+auto&                                               +
+-----+----------------------------------------------------+
+at   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`  const+
+-----+----------------------------------------------------+



to be documented


.. das:function:: describe_bitfield ( bf:auto const; merger:string const )  : auto

describe_bitfield returns auto



function arguments are

+------+------------+
+bf    +auto const  +
+------+------------+
+merger+string const+
+------+------------+



to be documented


.. das:function:: setup_tag_annotation ( name:string const; tag:string const; classPtr:auto const )  : auto

setup_tag_annotation returns auto



function arguments are

+--------+------------+
+name    +string const+
+--------+------------+
+tag     +string const+
+--------+------------+
+classPtr+auto const  +
+--------+------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: describe ( list:AnnotationArgumentList const )  : string const

describe returns string const



function arguments are

+----+--------------------------------------------------------------------------------+
+list+ :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+----+--------------------------------------------------------------------------------+



to be documented


.. das:function:: describe ( ann:AnnotationDeclaration const )  : string

describe returns string



function arguments are

+---+------------------------------------------------------------------------------+
+ann+ :ref:`rtti::AnnotationDeclaration <handle_rtti_AnnotationDeclaration>`  const+
+---+------------------------------------------------------------------------------+



to be documented


.. das:function:: describe ( list:AnnotationList const )  : string const

describe returns string const



function arguments are

+----+----------------------------------------------------------------+
+list+ :ref:`rtti::AnnotationList <handle_rtti_AnnotationList>`  const+
+----+----------------------------------------------------------------+



to be documented


.. das:function:: find_arg ( argn:string const; args:AnnotationArgumentList const )  : RttiValue

find_arg returns  :ref:`RttiValue <alias_RttiValue>` 



function arguments are

+----+--------------------------------------------------------------------------------+
+argn+string const                                                                    +
+----+--------------------------------------------------------------------------------+
+args+ :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>`  const+
+----+--------------------------------------------------------------------------------+



to be documented


.. das:function:: isExpression ( t:TypeDeclPtr; top:bool const )  : bool

isExpression returns bool



function arguments are

+---+----------------------------------------+
+t  + :ref:`TypeDeclPtr <alias_TypeDeclPtr>` +
+---+----------------------------------------+
+top+bool const                              +
+---+----------------------------------------+



to be documented


.. das:function:: is_class_method ( cinfo:StructurePtr; finfo:TypeDeclPtr )  : bool const

is_class_method returns bool const



function arguments are

+-----+------------------------------------------+
+cinfo+ :ref:`StructurePtr <alias_StructurePtr>` +
+-----+------------------------------------------+
+finfo+ :ref:`TypeDeclPtr <alias_TypeDeclPtr>`   +
+-----+------------------------------------------+



to be documented


.. das:function:: is_same_or_inherited ( parent:ast::Structure? const; child:ast::Structure? const )  : bool const

is_same_or_inherited returns bool const



function arguments are

+------+-----------------------------------------------------+
+parent+ :ref:`ast::Structure <handle_ast_Structure>` ? const+
+------+-----------------------------------------------------+
+child + :ref:`ast::Structure <handle_ast_Structure>` ? const+
+------+-----------------------------------------------------+



to be documented


.. das:function:: setup_macro ( name:string const; at:LineInfo const )  : ast::ExprBlock?

setup_macro returns  :ref:`ast::ExprBlock <handle_ast_ExprBlock>` ?



function arguments are

+----+----------------------------------------------------+
+name+string const                                        +
+----+----------------------------------------------------+
+at  + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`  const+
+----+----------------------------------------------------+



to be documented



