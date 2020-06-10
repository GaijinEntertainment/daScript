
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

+++++++
Classes
+++++++

.. das:class:: BetterRttiVisitor : AstVariantMacro

to be documented

.. das:method:: BetterRttiVisitor.visitExprIsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const) 

visitExprIsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: BetterRttiVisitor.visitExprAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const) 

visitExprAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: BetterRttiVisitor.visitExprSafeAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const) 

visitExprSafeAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+prog+smart_ptr<rtti::Program> const         +
+----+---------------------------------------+
+mod +rtti::Module? const                    +
+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented



.. das:class:: MacroMacro : AstFunctionAnnotation

to be documented

.. das:method:: MacroMacro.transform (self:ast::AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string) 

transform returns smart_ptr<ast::Expression>

function arguments are

+------+------------------------+
+call  +smart_ptr<ast::ExprCall>+
+------+------------------------+
+errors+das_string              +
+------+------------------------+


to be documented


.. das:method:: MacroMacro.apply (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+func  +smart_ptr<ast::Function>          +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: MacroMacro.finish (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; progArgs:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+--------+----------------------------------+
+func    +smart_ptr<ast::Function>          +
+--------+----------------------------------+
+group   +ast::ModuleGroup                  +
+--------+----------------------------------+
+args    +rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+progArgs+rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+errors  +das_string                        +
+--------+----------------------------------+


to be documented



.. das:class:: SetupAnyAnnotation : AstStructureAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:method:: SetupAnyAnnotation.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupAnyAnnotation.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupAnyAnnotation.setup_call (self:ast_boost::SetupAnyAnnotation; st:smart_ptr<ast::Structure>; cll:smart_ptr<ast::ExprCall>) 

function arguments are

+---+-------------------------+
+st +smart_ptr<ast::Structure>+
+---+-------------------------+
+cll+smart_ptr<ast::ExprCall> +
+---+-------------------------+


to be documented



.. das:class:: SetupFunctionAnnotatoin : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:method:: SetupFunctionAnnotatoin.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupFunctionAnnotatoin.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupFunctionAnnotatoin.setup_call (self:ast_boost::SetupAnyAnnotation; st:smart_ptr<ast::Structure>; cll:smart_ptr<ast::ExprCall>) 

function arguments are

+---+-------------------------+
+st +smart_ptr<ast::Structure>+
+---+-------------------------+
+cll+smart_ptr<ast::ExprCall> +
+---+-------------------------+


to be documented



.. das:class:: SetupReaderMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:method:: SetupReaderMacro.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupReaderMacro.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupReaderMacro.setup_call (self:ast_boost::SetupAnyAnnotation; st:smart_ptr<ast::Structure>; cll:smart_ptr<ast::ExprCall>) 

function arguments are

+---+-------------------------+
+st +smart_ptr<ast::Structure>+
+---+-------------------------+
+cll+smart_ptr<ast::ExprCall> +
+---+-------------------------+


to be documented



.. das:class:: SetupVariantMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:method:: SetupVariantMacro.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupVariantMacro.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: SetupVariantMacro.setup_call (self:ast_boost::SetupAnyAnnotation; st:smart_ptr<ast::Structure>; cll:smart_ptr<ast::ExprCall>) 

function arguments are

+---+-------------------------+
+st +smart_ptr<ast::Structure>+
+---+-------------------------+
+cll+smart_ptr<ast::ExprCall> +
+---+-------------------------+


to be documented



.. das:class:: TagFunctionAnnotation : AstFunctionAnnotation

to be documented

.. das:method:: TagFunctionAnnotation.transform (self:ast::AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string) 

transform returns smart_ptr<ast::Expression>

function arguments are

+------+------------------------+
+call  +smart_ptr<ast::ExprCall>+
+------+------------------------+
+errors+das_string              +
+------+------------------------+


to be documented


.. das:method:: TagFunctionAnnotation.apply (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+func  +smart_ptr<ast::Function>          +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: TagFunctionAnnotation.finish (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; progArgs:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+--------+----------------------------------+
+func    +smart_ptr<ast::Function>          +
+--------+----------------------------------+
+group   +ast::ModuleGroup                  +
+--------+----------------------------------+
+args    +rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+progArgs+rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+errors  +das_string                        +
+--------+----------------------------------+


to be documented



.. das:class:: TagFunctionMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string
  tag                      : string

.. das:method:: TagFunctionMacro.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: TagFunctionMacro.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: TagFunctionMacro.setup_call (self:ast_boost::SetupAnyAnnotation; st:smart_ptr<ast::Structure>; cll:smart_ptr<ast::ExprCall>) 

function arguments are

+---+-------------------------+
+st +smart_ptr<ast::Structure>+
+---+-------------------------+
+cll+smart_ptr<ast::ExprCall> +
+---+-------------------------+


to be documented



.. das:class:: TagStructureAnnotation : AstStructureAnnotation

to be documented

.. das:method:: TagStructureAnnotation.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

apply returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented


.. das:method:: TagStructureAnnotation.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

finish returns bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure>         +
+------+----------------------------------+
+group +ast::ModuleGroup                  +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+das_string                        +
+------+----------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: convert_to_expression(value:auto&; at:rtti::LineInfo const)

convert_to_expression returns auto



function arguments are

+-----+--------------------+
+value+auto&               +
+-----+--------------------+
+at   +rtti::LineInfo const+
+-----+--------------------+



to be documented


.. das:function:: describe_bitfield(bf:auto const; merger:string const)

describe_bitfield returns auto



function arguments are

+------+------------+
+bf    +auto const  +
+------+------------+
+merger+string const+
+------+------------+



to be documented


.. das:function:: setup_tag_annotation(name:string const; tag:string const; classPtr:auto const)

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

.. das:function:: describe(list:rtti::AnnotationArgumentList const)

describe returns string const



function arguments are

+----+----------------------------------+
+list+rtti::AnnotationArgumentList const+
+----+----------------------------------+



to be documented


.. das:function:: describe(ann:rtti::AnnotationDeclaration const)

describe returns string



function arguments are

+---+---------------------------------+
+ann+rtti::AnnotationDeclaration const+
+---+---------------------------------+



to be documented


.. das:function:: describe(list:rtti::AnnotationList const)

describe returns string const



function arguments are

+----+--------------------------+
+list+rtti::AnnotationList const+
+----+--------------------------+



to be documented


.. das:function:: find_arg(argn:string const; args:rtti::AnnotationArgumentList const)

find_arg returns variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>



function arguments are

+----+----------------------------------+
+argn+string const                      +
+----+----------------------------------+
+args+rtti::AnnotationArgumentList const+
+----+----------------------------------+



to be documented


.. das:function:: isExpression(t:smart_ptr<ast::TypeDecl> const; top:bool const)

isExpression returns bool



function arguments are

+---+------------------------------+
+t  +smart_ptr<ast::TypeDecl> const+
+---+------------------------------+
+top+bool const                    +
+---+------------------------------+



to be documented


.. das:function:: is_class_method(cinfo:smart_ptr<ast::Structure> const; finfo:smart_ptr<ast::TypeDecl> const)

is_class_method returns bool const



function arguments are

+-----+-------------------------------+
+cinfo+smart_ptr<ast::Structure> const+
+-----+-------------------------------+
+finfo+smart_ptr<ast::TypeDecl> const +
+-----+-------------------------------+



to be documented


.. das:function:: is_same_or_inherited(parent:ast::Structure? const; child:ast::Structure? const)

is_same_or_inherited returns bool const



function arguments are

+------+---------------------+
+parent+ast::Structure? const+
+------+---------------------+
+child +ast::Structure? const+
+------+---------------------+



to be documented


.. das:function:: setup_macro(name:string const; at:rtti::LineInfo const)

setup_macro returns ast::ExprBlock?



function arguments are

+----+--------------------+
+name+string const        +
+----+--------------------+
+at  +rtti::LineInfo const+
+----+--------------------+



to be documented



