
.. _stdlib_ast_boost:

=========================
Boost package for the AST
=========================

+++++++
Classes
+++++++

.. das:class:: BetterRttiVisitor : AstVariantMacro

to be documented

.. das:function:: BetterRttiVisitor->__finalize (self) 

to be documented


.. das:function:: BetterRttiVisitor->visitExprIsVariant (self;prog;mod;expr) 

visitExprIsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: BetterRttiVisitor->visitExprAsVariant (self;prog;mod;expr) 

visitExprAsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+prog+smart_ptr<rtti::Program> const     +
+----+-----------------------------------+
+mod +rtti::Module? const                +
+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: BetterRttiVisitor->visitExprSafeAsVariant (self;prog;mod;expr) 

visitExprSafeAsVariant returns ::

 smart_ptr<ast::Expression>

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

.. das:function:: MacroMacro->__finalize (self) 

to be documented


.. das:function:: MacroMacro->transform (self;call;errors) 

transform returns ::

 smart_ptr<ast::Expression>

function arguments are

+------+-------------------------------+
+call  +smart_ptr<ast::ExprCall> -const+
+------+-------------------------------+
+errors+$::das_string -const           +
+------+-------------------------------+


to be documented


.. das:function:: MacroMacro->apply (self;func;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+func  +smart_ptr<ast::Function> -const   +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: MacroMacro->finish (self;func;group;args;progArgs;errors) 

finish returns ::

 bool

function arguments are

+--------+----------------------------------+
+func    +smart_ptr<ast::Function> -const   +
+--------+----------------------------------+
+group   +ast::ModuleGroup -const           +
+--------+----------------------------------+
+args    +rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+progArgs+rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+errors  +$::das_string -const              +
+--------+----------------------------------+


to be documented



.. das:class:: SetupAnyAnnotation : AstStructureAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:function:: SetupAnyAnnotation->__finalize (self) 

to be documented


.. das:function:: SetupAnyAnnotation->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupAnyAnnotation->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupAnyAnnotation->setup_call (self;st;cll) 

function arguments are

+---+--------------------------------+
+st +smart_ptr<ast::Structure> -const+
+---+--------------------------------+
+cll+smart_ptr<ast::ExprCall> -const +
+---+--------------------------------+


to be documented



.. das:class:: SetupFunctionAnnotatoin : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:function:: SetupFunctionAnnotatoin->__finalize (self) 

to be documented


.. das:function:: SetupFunctionAnnotatoin->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupFunctionAnnotatoin->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupFunctionAnnotatoin->setup_call (self;st;cll) 

function arguments are

+---+--------------------------------+
+st +smart_ptr<ast::Structure> -const+
+---+--------------------------------+
+cll+smart_ptr<ast::ExprCall> -const +
+---+--------------------------------+


to be documented



.. das:class:: SetupReaderMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:function:: SetupReaderMacro->__finalize (self) 

to be documented


.. das:function:: SetupReaderMacro->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupReaderMacro->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupReaderMacro->setup_call (self;st;cll) 

function arguments are

+---+--------------------------------+
+st +smart_ptr<ast::Structure> -const+
+---+--------------------------------+
+cll+smart_ptr<ast::ExprCall> -const +
+---+--------------------------------+


to be documented



.. das:class:: SetupVariantMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string

.. das:function:: SetupVariantMacro->__finalize (self) 

to be documented


.. das:function:: SetupVariantMacro->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupVariantMacro->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: SetupVariantMacro->setup_call (self;st;cll) 

function arguments are

+---+--------------------------------+
+st +smart_ptr<ast::Structure> -const+
+---+--------------------------------+
+cll+smart_ptr<ast::ExprCall> -const +
+---+--------------------------------+


to be documented



.. das:class:: TagFunctionAnnotation : AstFunctionAnnotation

to be documented

.. das:function:: TagFunctionAnnotation->__finalize (self) 

to be documented


.. das:function:: TagFunctionAnnotation->transform (self;call;errors) 

transform returns ::

 smart_ptr<ast::Expression>

function arguments are

+------+-------------------------------+
+call  +smart_ptr<ast::ExprCall> -const+
+------+-------------------------------+
+errors+$::das_string -const           +
+------+-------------------------------+


to be documented


.. das:function:: TagFunctionAnnotation->apply (self;func;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+func  +smart_ptr<ast::Function> -const   +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: TagFunctionAnnotation->finish (self;func;group;args;progArgs;errors) 

finish returns ::

 bool

function arguments are

+--------+----------------------------------+
+func    +smart_ptr<ast::Function> -const   +
+--------+----------------------------------+
+group   +ast::ModuleGroup -const           +
+--------+----------------------------------+
+args    +rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+progArgs+rtti::AnnotationArgumentList const+
+--------+----------------------------------+
+errors  +$::das_string -const              +
+--------+----------------------------------+


to be documented



.. das:class:: TagFunctionMacro : SetupAnyAnnotation

to be documented

it defines as follows ::

  annotation_function_call : string
  name                     : string
  tag                      : string

.. das:function:: TagFunctionMacro->__finalize (self) 

to be documented


.. das:function:: TagFunctionMacro->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: TagFunctionMacro->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: TagFunctionMacro->setup_call (self;st;cll) 

function arguments are

+---+--------------------------------+
+st +smart_ptr<ast::Structure> -const+
+---+--------------------------------+
+cll+smart_ptr<ast::ExprCall> -const +
+---+--------------------------------+


to be documented



.. das:class:: TagStructureAnnotation : AstStructureAnnotation

to be documented

.. das:function:: TagStructureAnnotation->__finalize (self) 

to be documented


.. das:function:: TagStructureAnnotation->apply (self;st;group;args;errors) 

apply returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented


.. das:function:: TagStructureAnnotation->finish (self;st;group;args;errors) 

finish returns ::

 bool

function arguments are

+------+----------------------------------+
+st    +smart_ptr<ast::Structure> -const  +
+------+----------------------------------+
+group +ast::ModuleGroup -const           +
+------+----------------------------------+
+args  +rtti::AnnotationArgumentList const+
+------+----------------------------------+
+errors+$::das_string -const              +
+------+----------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: convert_to_expression(value;at)

convert_to_expression returns ::

 auto



function arguments are

+-----+--------------------+
+value+auto& -const        +
+-----+--------------------+
+at   +rtti::LineInfo const+
+-----+--------------------+



to be documented


.. das:function:: describe_bitfield(bf;merger)

describe_bitfield returns ::

 auto



function arguments are

+------+------------+
+bf    +auto const  +
+------+------------+
+merger+string const+
+------+------------+



to be documented


.. das:function:: setup_tag_annotation(name;tag;classPtr)

setup_tag_annotation returns ::

 auto



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

.. das:function:: describe(list)

describe returns ::

 string const



function arguments are

+----+----------------------------------+
+list+rtti::AnnotationArgumentList const+
+----+----------------------------------+



to be documented


.. das:function:: describe(ann)

describe returns ::

 string



function arguments are

+---+---------------------------------+
+ann+rtti::AnnotationDeclaration const+
+---+---------------------------------+



to be documented


.. das:function:: describe(list)

describe returns ::

 string const



function arguments are

+----+--------------------------+
+list+rtti::AnnotationList const+
+----+--------------------------+



to be documented


.. das:function:: find_arg(argn;args)

find_arg returns ::

 variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any>



function arguments are

+----+----------------------------------+
+argn+string const                      +
+----+----------------------------------+
+args+rtti::AnnotationArgumentList const+
+----+----------------------------------+



to be documented


.. das:function:: isExpression(t;top)

isExpression returns ::

 bool



function arguments are

+---+------------------------------+
+t  +smart_ptr<ast::TypeDecl> const+
+---+------------------------------+
+top+bool const                    +
+---+------------------------------+



to be documented


.. das:function:: is_class_method(cinfo;finfo)

is_class_method returns ::

 bool const



function arguments are

+-----+-------------------------------+
+cinfo+smart_ptr<ast::Structure> const+
+-----+-------------------------------+
+finfo+smart_ptr<ast::TypeDecl> const +
+-----+-------------------------------+



to be documented


.. das:function:: is_same_or_inherited(parent;child)

is_same_or_inherited returns ::

 bool const



function arguments are

+------+---------------------+
+parent+ast::Structure? const+
+------+---------------------+
+child +ast::Structure? const+
+------+---------------------+



to be documented


.. das:function:: setup_macro(name;at)

setup_macro returns ::

 ast::ExprBlock?



function arguments are

+----+--------------------+
+name+string const        +
+----+--------------------+
+at  +rtti::LineInfo const+
+----+--------------------+



to be documented



