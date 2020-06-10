
.. _stdlib_ast:

==============================================
Compile time type  and AST information library
==============================================

++++++++++++
Type aliases
++++++++++++

.. das:attribute:: EnumerationPtr = smart_ptr<ast::Enumeration>

to be documented

.. das:attribute:: ExprAscendFlags is a bitfield

- useStackRef
- needTypeInfo

to be documented

.. das:attribute:: ExprAtFlags is a bitfield

- r2v
- r2cr
- write

to be documented

.. das:attribute:: ExprBlockFlags is a bitfield

- isClosure
- hasReturn
- copyOnReturn
- moveOnReturn
- inTheLoop
- finallyBeforeBody
- finallyDisabled
- aotSkipMakeBlock
- aotDoNotSkipAnnotationData
- isCollapseable
- needCollapse

to be documented

.. das:attribute:: ExprCastFlags is a bitfield

- upcastCast
- reinterpretCast

to be documented

.. das:attribute:: ExprFieldDerefFlags is a bitfield

- unsafeDeref
- ignoreCaptureConst

to be documented

.. das:attribute:: ExprFieldFieldFlags is a bitfield

- r2v
- r2cr
- write

to be documented

.. das:attribute:: ExprFlags is a bitfield

- constexpression
- noSideEffects
- noNativeSideEffects

to be documented

.. das:attribute:: ExprGenFlags is a bitfield

- alwaysSafe
- generated

to be documented

.. das:attribute:: ExprMakeBlockFlags is a bitfield

- isLambda
- isLocalFunction

to be documented

.. das:attribute:: ExprMakeLocalFlags is a bitfield

- useStackRef
- useCMRES
- doesNotNeedSp
- doesNotNeedInit
- initAllFields

to be documented

.. das:attribute:: ExprMakeStructFlags is a bitfield

- useInitializer
- isNewHandle

to be documented

.. das:attribute:: ExprPrintFlags is a bitfield

- topLevel
- argLevel
- bottomLevel

to be documented

.. das:attribute:: ExprReturnFlags is a bitfield

- moveSemantics
- returnReference
- returnInBlock
- takeOverRightStack
- returnCallCMRES
- returnCMRES
- fromYield

to be documented

.. das:attribute:: ExprSwizzleFieldFlags is a bitfield

- r2v
- r2cr
- write

to be documented

.. das:attribute:: ExprVarFlags is a bitfield

- local
- argument
- block
- thisBlock
- r2v
- r2cr
- write

to be documented

.. das:attribute:: ExprYieldFlags is a bitfield

- moveSemantics

to be documented

.. das:attribute:: ExpressionPtr = smart_ptr<ast::Expression>

to be documented

.. das:attribute:: FieldDeclarationFlags is a bitfield

- moveSemantics
- parentType
- capturedConstant
- generated

to be documented

.. das:attribute:: FunctionAnnotationPtr = smart_ptr<ast::FunctionAnnotation>

to be documented

.. das:attribute:: FunctionFlags is a bitfield

- builtIn
- policyBased
- callBased
- interopFn
- hasReturn
- copyOnReturn
- moveOnReturn
- exports
- init
- addr
- used
- fastCall
- knownSideEffects
- hasToRunAtCompileTime
- unsafe
- unsafeOperation
- unsafeDeref
- hasMakeBlock
- aotNeedPrologue
- noAot
- aotHybrid
- aotTemplate
- generated
- privateFunction
- _generator
- _lambda
- firstArgReturnType
- isClassMethod

to be documented

.. das:attribute:: FunctionPtr = smart_ptr<ast::Function>

to be documented

.. das:attribute:: FunctionSideEffectFlags is a bitfield

- unsafe
- userScenario
- modifyExternal
- modifyArgument
- accessGlobal
- invoke

to be documented

.. das:attribute:: MakeFieldDeclFlags is a bitfield

- moveSemantics
- cloneSemantics

to be documented

.. das:attribute:: MakeFieldDeclPtr = smart_ptr<ast::MakeFieldDecl>

to be documented

.. das:attribute:: PassMacroPtr = smart_ptr<ast::PassMacro>

to be documented

.. das:attribute:: ProgramPtr = smart_ptr<rtti::Program>

to be documented

.. das:attribute:: ReaderMacroPtr = smart_ptr<ast::ReaderMacro>

to be documented

.. das:attribute:: StructureAnnotationPtr = smart_ptr<ast::StructureAnnotation>

to be documented

.. das:attribute:: StructureFlags is a bitfield

- isClass
- genCtor
- cppLayout
- cppLayoutNotPod
- generated
- persistent
- isLambda

to be documented

.. das:attribute:: StructurePtr = smart_ptr<ast::Structure>

to be documented

.. das:attribute:: TypeDeclFlags is a bitfield

- ref
- constant
- temporary
- implicit
- removeRef
- removeConstant
- removeDim
- removeTemporary
- explicitConst
- aotAlias
- smartPtr

to be documented

.. das:attribute:: TypeDeclPtr = smart_ptr<ast::TypeDecl>

to be documented

.. das:attribute:: VariableAccessFlags is a bitfield

- access_extern
- access_get
- access_ref
- access_init
- access_pass

to be documented

.. das:attribute:: VariableFlags is a bitfield

- init_via_move
- init_via_clone
- used
- aliasCMRES
- marked_used
- global_shared
- do_not_delete
- generated

to be documented

.. das:attribute:: VariablePtr = smart_ptr<ast::Variable>

to be documented

.. das:attribute:: VariantMacroPtr = smart_ptr<ast::VariantMacro>

to be documented

++++++++++++
Enumerations
++++++++++++

.. das:attribute:: SideEffects

to be documented

+-------------------------+--+
+none                     +0 +
+-------------------------+--+
+unsafe                   +1 +
+-------------------------+--+
+userScenario             +2 +
+-------------------------+--+
+modifyExternal           +4 +
+-------------------------+--+
+accessExternal           +4 +
+-------------------------+--+
+modifyArgument           +8 +
+-------------------------+--+
+modifyArgumentAndExternal+12+
+-------------------------+--+
+worstDefault             +12+
+-------------------------+--+
+accessGlobal             +16+
+-------------------------+--+
+invoke                   +32+
+-------------------------+--+
+inferedSideEffects       +56+
+-------------------------+--+


++++++++++++++++++
Handled structures
++++++++++++++++++

.. das:attribute:: Enumeration

it defines as follows ::

  enumModule : rtti::Module?
  at         : rtti::LineInfo
  cppName    : $::das_string
  list       : ast::dasvector`pair`das_string`smart_ptr`Expression
  name       : $::das_string
  external   : bool
  baseType   : rtti::Type

to be documented


.. das:attribute:: ExprAddr

it defines as follows ::

  func       : ast::Function?
  target     : $::das_string
  at         : rtti::LineInfo
  funcType   : smart_ptr<ast::TypeDecl>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprArrayComprehension

it defines as follows ::

  at              : rtti::LineInfo
  printFlags      : bitfield<topLevel;argLevel;bottomLevel>
  generatorSyntax : bool
  subexpr         : smart_ptr<ast::Expression>
  genFlags        : bitfield<alwaysSafe;generated>
  exprFor         : smart_ptr<ast::Expression>
  exprWhere       : smart_ptr<ast::Expression>
  _type           : smart_ptr<ast::TypeDecl>
  flags           : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti          : string const

to be documented


.. das:attribute:: ExprAsVariant

it defines as follows ::

  annotation : smart_ptr<rtti::TypeAnnotation>
  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldIndex : int
  fieldFlags : bitfield<r2v;r2cr;write>
  field      : ast::FieldDeclaration const? const
  derefFlags : bitfield<unsafeDeref;ignoreCaptureConst>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  name       : $::das_string
  atField    : rtti::LineInfo
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprAscend

it defines as follows ::

  ascType     : smart_ptr<ast::TypeDecl>
  at          : rtti::LineInfo
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  stackTop    : uint
  ascendFlags : bitfield<useStackRef;needTypeInfo>
  subexpr     : smart_ptr<ast::Expression>
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti      : string const

to be documented


.. das:attribute:: ExprAssert

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  isVerify               : bool
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprAt

it defines as follows ::

  index      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  atFlags    : bitfield<r2v;r2cr;write>

to be documented


.. das:attribute:: ExprBlock

it defines as follows ::

  stackVarBottom    : uint
  annotationDataSid : uint
  arguments         : ast::dasvector`smart_ptr`Variable
  at                : rtti::LineInfo
  stackCleanVars    : ast::dasvector`pair`uint`uint
  list              : ast::dasvector`smart_ptr`Expression
  returnType        : smart_ptr<ast::TypeDecl>
  printFlags        : bitfield<topLevel;argLevel;bottomLevel>
  annotations       : rtti::AnnotationList
  stackTop          : uint
  maxLabelIndex     : int
  blockFlags        : bitfield<isClosure;hasReturn;copyOnReturn;moveOnReturn;inTheLoop;finallyBeforeBody;finallyDisabled;aotSkipMakeBlock;aotDoNotSkipAnnotationData;isCollapseable;needCollapse>
  finalList         : ast::dasvector`smart_ptr`Expression
  genFlags          : bitfield<alwaysSafe;generated>
  annotationData    : uint64
  stackVarTop       : uint
  flags             : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  _type             : smart_ptr<ast::TypeDecl>
  __rtti            : string const

to be documented


.. das:attribute:: ExprBreak

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprCall

it defines as follows ::

  func                   : ast::Function?
  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  stackTop               : uint
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  doesNotNeedSp          : bool
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprCast

it defines as follows ::

  castFlags  : bitfield<upcastCast;reinterpretCast>
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  castType   : smart_ptr<ast::TypeDecl>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprClone

it defines as follows ::

  right      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  op         : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  left       : smart_ptr<ast::Expression>

to be documented


.. das:attribute:: ExprConst

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstBitfield

it defines as follows ::

  value        : bitfield
  at           : rtti::LineInfo
  bitfieldType : smart_ptr<ast::TypeDecl>
  printFlags   : bitfield<topLevel;argLevel;bottomLevel>
  genFlags     : bitfield<alwaysSafe;generated>
  baseType     : rtti::Type
  _type        : smart_ptr<ast::TypeDecl>
  flags        : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti       : string const

to be documented


.. das:attribute:: ExprConstBool

it defines as follows ::

  value      : bool
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstDouble

it defines as follows ::

  value      : double
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstEnumeration

it defines as follows ::

  value      : $::das_string
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  enumType   : smart_ptr<ast::Enumeration>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprConstFloat

it defines as follows ::

  value      : float
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstFloat2

it defines as follows ::

  value      : float2
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstFloat3

it defines as follows ::

  value      : float3
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstFloat4

it defines as follows ::

  value      : float4
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt

it defines as follows ::

  value      : int
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt16

it defines as follows ::

  value      : int16
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt2

it defines as follows ::

  value      : int2
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt3

it defines as follows ::

  value      : int3
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt4

it defines as follows ::

  value      : int4
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt64

it defines as follows ::

  value      : int64
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstInt8

it defines as follows ::

  value      : int8
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstPtr

it defines as follows ::

  value      : void?
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstRange

it defines as follows ::

  value      : range
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstString

it defines as follows ::

  value      : $::das_string
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt

it defines as follows ::

  value      : uint
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt16

it defines as follows ::

  value      : uint16
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt2

it defines as follows ::

  value      : uint2
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt3

it defines as follows ::

  value      : uint3
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt4

it defines as follows ::

  value      : uint4
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt64

it defines as follows ::

  value      : uint64
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstUInt8

it defines as follows ::

  value      : uint8
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprConstURange

it defines as follows ::

  value      : urange
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprContinue

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprCopy

it defines as follows ::

  takeOverRightStack : bool
  right              : smart_ptr<ast::Expression>
  at                 : rtti::LineInfo
  op                 : $::das_string
  printFlags         : bitfield<topLevel;argLevel;bottomLevel>
  genFlags           : bitfield<alwaysSafe;generated>
  _type              : smart_ptr<ast::TypeDecl>
  flags              : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti             : string const
  left               : smart_ptr<ast::Expression>

to be documented


.. das:attribute:: ExprDebug

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprDelete

it defines as follows ::

  at         : rtti::LineInfo
  native     : bool
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprErase

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprFakeContext

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprFakeLineInfo

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  baseType   : rtti::Type
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprField

it defines as follows ::

  annotation : smart_ptr<rtti::TypeAnnotation>
  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldIndex : int
  fieldFlags : bitfield<r2v;r2cr;write>
  field      : ast::FieldDeclaration const? const
  derefFlags : bitfield<unsafeDeref;ignoreCaptureConst>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  name       : $::das_string
  atField    : rtti::LineInfo
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprFind

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprFor

it defines as follows ::

  at                : rtti::LineInfo
  body              : smart_ptr<ast::Expression>
  iteratorsAt       : ast::dasvector`LineInfo
  printFlags        : bitfield<topLevel;argLevel;bottomLevel>
  iterators         : ast::dasvector`das_string
  iteratorVariables : ast::dasvector`smart_ptr`Variable
  genFlags          : bitfield<alwaysSafe;generated>
  sources           : ast::dasvector`smart_ptr`Expression
  _type             : smart_ptr<ast::TypeDecl>
  flags             : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti            : string const

to be documented


.. das:attribute:: ExprGoto

it defines as follows ::

  at         : rtti::LineInfo
  labelName  : int
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprIfThenElse

it defines as follows ::

  at         : rtti::LineInfo
  if_false   : smart_ptr<ast::Expression>
  isStatic   : bool
  cond       : smart_ptr<ast::Expression>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  if_true    : smart_ptr<ast::Expression>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprInvoke

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  stackTop               : uint
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  doesNotNeedSp          : bool
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprIs

it defines as follows ::

  typeexpr   : smart_ptr<ast::TypeDecl>
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprIsVariant

it defines as follows ::

  annotation : smart_ptr<rtti::TypeAnnotation>
  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldIndex : int
  fieldFlags : bitfield<r2v;r2cr;write>
  field      : ast::FieldDeclaration const? const
  derefFlags : bitfield<unsafeDeref;ignoreCaptureConst>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  name       : $::das_string
  atField    : rtti::LineInfo
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprKeyExists

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprLabel

it defines as follows ::

  comment    : $::das_string
  at         : rtti::LineInfo
  labelName  : int
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprLet

it defines as follows ::

  at         : rtti::LineInfo
  inScope    : bool
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  variables  : ast::dasvector`smart_ptr`Variable
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprLooksLikeCall

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprMakeArray

it defines as follows ::

  makeType    : smart_ptr<ast::TypeDecl>
  values      : ast::dasvector`smart_ptr`Expression
  at          : rtti::LineInfo
  recordType  : smart_ptr<ast::TypeDecl>
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  makeFlags   : bitfield<useStackRef;useCMRES;doesNotNeedSp;doesNotNeedInit;initAllFields>
  stackTop    : uint
  extraOffset : uint
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti      : string const

to be documented


.. das:attribute:: ExprMakeBlock

it defines as follows ::

  mmFlags    : bitfield<isLambda;isLocalFunction>
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  stackTop   : uint
  block      : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprMakeGenerator

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  iterType               : smart_ptr<ast::TypeDecl>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprMakeLocal

it defines as follows ::

  makeType    : smart_ptr<ast::TypeDecl>
  at          : rtti::LineInfo
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  makeFlags   : bitfield<useStackRef;useCMRES;doesNotNeedSp;doesNotNeedInit;initAllFields>
  stackTop    : uint
  extraOffset : uint
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti      : string const

to be documented


.. das:attribute:: ExprMakeStruct

it defines as follows ::

  makeType        : smart_ptr<ast::TypeDecl>
  at              : rtti::LineInfo
  structs         : ast::dasvector`smart_ptr`MakeStruct
  printFlags      : bitfield<topLevel;argLevel;bottomLevel>
  makeFlags       : bitfield<useStackRef;useCMRES;doesNotNeedSp;doesNotNeedInit;initAllFields>
  stackTop        : uint
  extraOffset     : uint
  genFlags        : bitfield<alwaysSafe;generated>
  _type           : smart_ptr<ast::TypeDecl>
  flags           : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti          : string const
  makeStructFlags : bitfield<useInitializer;isNewHandle>

to be documented


.. das:attribute:: ExprMakeTuple

it defines as follows ::

  makeType    : smart_ptr<ast::TypeDecl>
  values      : ast::dasvector`smart_ptr`Expression
  at          : rtti::LineInfo
  recordType  : smart_ptr<ast::TypeDecl>
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  makeFlags   : bitfield<useStackRef;useCMRES;doesNotNeedSp;doesNotNeedInit;initAllFields>
  stackTop    : uint
  extraOffset : uint
  genFlags    : bitfield<alwaysSafe;generated>
  isKeyValue  : bool
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti      : string const

to be documented


.. das:attribute:: ExprMakeVariant

it defines as follows ::

  variants    : ast::dasvector`smart_ptr`MakeFieldDecl
  makeType    : smart_ptr<ast::TypeDecl>
  at          : rtti::LineInfo
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  makeFlags   : bitfield<useStackRef;useCMRES;doesNotNeedSp;doesNotNeedInit;initAllFields>
  stackTop    : uint
  extraOffset : uint
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti      : string const

to be documented


.. das:attribute:: ExprMemZero

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprMove

it defines as follows ::

  right      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  op         : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  left       : smart_ptr<ast::Expression>

to be documented


.. das:attribute:: ExprNamedCall

it defines as follows ::

  arguments              : ast::MakeStruct
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprNew

it defines as follows ::

  func                   : ast::Function?
  typeexpr               : smart_ptr<ast::TypeDecl>
  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  initializer            : bool
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  stackTop               : uint
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprNullCoalescing

it defines as follows ::

  defaultValue : smart_ptr<ast::Expression>
  at           : rtti::LineInfo
  printFlags   : bitfield<topLevel;argLevel;bottomLevel>
  subexpr      : smart_ptr<ast::Expression>
  genFlags     : bitfield<alwaysSafe;generated>
  _type        : smart_ptr<ast::TypeDecl>
  __rtti       : string const
  flags        : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprOp1

it defines as follows ::

  at         : rtti::LineInfo
  op         : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprOp2

it defines as follows ::

  right      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  op         : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  left       : smart_ptr<ast::Expression>

to be documented


.. das:attribute:: ExprOp3

it defines as follows ::

  right      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  op         : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  left       : smart_ptr<ast::Expression>

to be documented


.. das:attribute:: ExprPtr2Ref

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprReader

it defines as follows ::

  macro      : smart_ptr<ast::ReaderMacro>
  sequence   : $::das_string
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprRef2Ptr

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprRef2Value

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprReturn

it defines as follows ::

  at          : rtti::LineInfo
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  stackTop    : uint
  subexpr     : smart_ptr<ast::Expression>
  block       : ast::ExprBlock?
  genFlags    : bitfield<alwaysSafe;generated>
  refStackTop : uint
  _type       : smart_ptr<ast::TypeDecl>
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  returnFlags : bitfield<moveSemantics;returnReference;returnInBlock;takeOverRightStack;returnCallCMRES;returnCMRES;fromYield>
  __rtti      : string const

to be documented


.. das:attribute:: ExprSafeAsVariant

it defines as follows ::

  annotation : smart_ptr<rtti::TypeAnnotation>
  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldIndex : int
  fieldFlags : bitfield<r2v;r2cr;write>
  field      : ast::FieldDeclaration const? const
  skipQQ     : bool
  derefFlags : bitfield<unsafeDeref;ignoreCaptureConst>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  name       : $::das_string
  atField    : rtti::LineInfo
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprSafeAt

it defines as follows ::

  index      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  atFlags    : bitfield<r2v;r2cr;write>

to be documented


.. das:attribute:: ExprSafeField

it defines as follows ::

  annotation : smart_ptr<rtti::TypeAnnotation>
  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldIndex : int
  fieldFlags : bitfield<r2v;r2cr;write>
  field      : ast::FieldDeclaration const? const
  skipQQ     : bool
  derefFlags : bitfield<unsafeDeref;ignoreCaptureConst>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  name       : $::das_string
  atField    : rtti::LineInfo
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprStaticAssert

it defines as follows ::

  arguments              : ast::dasvector`smart_ptr`Expression
  at                     : rtti::LineInfo
  printFlags             : bitfield<topLevel;argLevel;bottomLevel>
  name                   : $::das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprStringBuilder

it defines as follows ::

  at         : rtti::LineInfo
  elements   : ast::dasvector`smart_ptr`Expression
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprSwizzle

it defines as follows ::

  value      : smart_ptr<ast::Expression>
  at         : rtti::LineInfo
  fieldFlags : bitfield<r2v;r2cr;write>
  mask       : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const
  fields     : ast::dasvector`uint8

to be documented


.. das:attribute:: ExprTryCatch

it defines as follows ::

  try_block   : smart_ptr<ast::Expression>
  at          : rtti::LineInfo
  catch_block : smart_ptr<ast::Expression>
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  __rtti      : string const
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprTypeInfo

it defines as follows ::

  typeexpr   : smart_ptr<ast::TypeDecl>
  extratrait : $::das_string
  macro      : ast::TypeInfoMacro?
  subtrait   : $::das_string
  at         : rtti::LineInfo
  trait      : $::das_string
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  subexpr    : smart_ptr<ast::Expression>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti     : string const

to be documented


.. das:attribute:: ExprVar

it defines as follows ::

  at            : rtti::LineInfo
  variable      : smart_ptr<ast::Variable>
  varFlags      : bitfield<local;argument;block;thisBlock;r2v;r2cr;write>
  printFlags    : bitfield<topLevel;argLevel;bottomLevel>
  argumentIndex : int
  name          : $::das_string
  genFlags      : bitfield<alwaysSafe;generated>
  pBlock        : ast::ExprBlock?
  _type         : smart_ptr<ast::TypeDecl>
  flags         : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti        : string const

to be documented


.. das:attribute:: ExprWhile

it defines as follows ::

  at         : rtti::LineInfo
  body       : smart_ptr<ast::Expression>
  cond       : smart_ptr<ast::Expression>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprWith

it defines as follows ::

  at         : rtti::LineInfo
  body       : smart_ptr<ast::Expression>
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _with      : smart_ptr<ast::Expression>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: ExprYield

it defines as follows ::

  at          : rtti::LineInfo
  printFlags  : bitfield<topLevel;argLevel;bottomLevel>
  subexpr     : smart_ptr<ast::Expression>
  genFlags    : bitfield<alwaysSafe;generated>
  _type       : smart_ptr<ast::TypeDecl>
  __rtti      : string const
  flags       : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  returnFlags : bitfield<moveSemantics>

to be documented


.. das:attribute:: Expression

it defines as follows ::

  at         : rtti::LineInfo
  printFlags : bitfield<topLevel;argLevel;bottomLevel>
  genFlags   : bitfield<alwaysSafe;generated>
  _type      : smart_ptr<ast::TypeDecl>
  __rtti     : string const
  flags      : bitfield<constexpression;noSideEffects;noNativeSideEffects>

to be documented


.. das:attribute:: FieldDeclaration

it defines as follows ::

  annotation : rtti::AnnotationArgumentList
  at         : rtti::LineInfo
  name       : $::das_string
  init       : smart_ptr<ast::Expression>
  offset     : int
  _type      : smart_ptr<ast::TypeDecl>
  flags      : bitfield<moveSemantics;parentType;capturedConstant;generated>

to be documented


.. das:attribute:: Function

it defines as follows ::

  arguments       : ast::dasvector`smart_ptr`Variable
  fromGeneric     : ast::Function?
  result          : smart_ptr<ast::TypeDecl>
  aotHash         : uint64
  totalGenLabel   : int
  _module         : rtti::Module?
  index           : int
  at              : rtti::LineInfo
  inferStack      : ast::dasvector`InferHistory
  body            : smart_ptr<ast::Expression>
  atDecl          : rtti::LineInfo
  sideEffectFlags : bitfield<unsafe;userScenario;modifyExternal;modifyArgument;accessGlobal;invoke>
  annotations     : rtti::AnnotationList
  totalStackSize  : uint
  name            : $::das_string
  hash            : uint64
  flags           : bitfield<builtIn;policyBased;callBased;interopFn;hasReturn;copyOnReturn;moveOnReturn;exports;init;addr;used;fastCall;knownSideEffects;hasToRunAtCompileTime;unsafe;unsafeOperation;unsafeDeref;hasMakeBlock;aotNeedPrologue;noAot;aotHybrid;aotTemplate;generated;privateFunction;_generator;_lambda;firstArgReturnType;isClassMethod>

to be documented


.. das:attribute:: FunctionAnnotation

to be documented


.. das:attribute:: InferHistory

it defines as follows ::

  func : ast::Function?
  at   : rtti::LineInfo

to be documented


.. das:attribute:: MakeFieldDecl

it defines as follows ::

  value : smart_ptr<ast::Expression>
  at    : rtti::LineInfo
  name  : $::das_string
  flags : bitfield<moveSemantics;cloneSemantics>

to be documented


.. das:attribute:: ModuleGroup

to be documented


.. das:attribute:: ModuleLibrary

to be documented


.. das:attribute:: PassMacro

it defines as follows ::

  name : $::das_string

to be documented


.. das:attribute:: ReaderMacro

it defines as follows ::

  _module : rtti::Module?
  name    : $::das_string

to be documented


.. das:attribute:: Structure

it defines as follows ::

  _module     : rtti::Module?
  at          : rtti::LineInfo
  parent      : ast::Structure?
  annotations : rtti::AnnotationList
  name        : $::das_string
  fields      : ast::dasvector`FieldDeclaration
  flags       : bitfield<isClass;genCtor;cppLayout;cppLayoutNotPod;generated;persistent;isLambda>

to be documented


.. das:attribute:: StructureAnnotation

to be documented


.. das:attribute:: TypeDecl

it defines as follows ::

  alias      : $::das_string
  annotation : rtti::TypeAnnotation?
  dimExpr    : ast::dasvector`smart_ptr`Expression
  argTypes   : ast::dasvector`smart_ptr`TypeDecl
  dim        : ast::dasvector`int
  _module    : rtti::Module?
  secondType : smart_ptr<ast::TypeDecl>
  at         : rtti::LineInfo
  enumType   : ast::Enumeration?
  argNames   : ast::dasvector`das_string
  baseType   : rtti::Type
  firstType  : smart_ptr<ast::TypeDecl>
  structType : ast::Structure?
  flags      : bitfield<ref;constant;temporary;implicit;removeRef;removeConstant;removeDim;removeTemporary;explicitConst;aotAlias;smartPtr>

it has the following properties ::

  isCtorType : bool
  alignOf    : int
  isVoid     : bool
  canMove    : bool
  isExprType : bool
  sizeOf     : int
  baseSizeOf : int
  canCopy    : bool
  countOf    : int

to be documented


.. das:attribute:: TypeInfoMacro

it defines as follows ::

  _module : rtti::Module?
  name    : $::das_string

to be documented


.. das:attribute:: Variable

it defines as follows ::

  annotation    : rtti::AnnotationArgumentList
  initStackSize : uint
  _module       : rtti::Module?
  index         : int
  at            : rtti::LineInfo
  stackTop      : uint
  name          : $::das_string
  init          : smart_ptr<ast::Expression>
  access_flags  : bitfield<access_extern;access_get;access_ref;access_init;access_pass>
  source        : smart_ptr<ast::Expression>
  _type         : smart_ptr<ast::TypeDecl>
  flags         : bitfield<init_via_move;init_via_clone;used;aliasCMRES;marked_used;global_shared;do_not_delete;generated>

it has the following properties ::

  isAccessUnused : bool

to be documented


.. das:attribute:: VariantMacro

it defines as follows ::

  name : $::das_string

to be documented


.. das:attribute:: VisitorAdapter

to be documented


+++++++
Classes
+++++++

.. das:class:: AstFunctionAnnotation

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstFunctionAnnotation->__finalize (self) 

to be documented


.. das:function:: AstFunctionAnnotation->transform (self;call;errors) 

transform returns ::

 smart_ptr<ast::Expression>

function arguments are

+------+-------------------------------+
+call  +smart_ptr<ast::ExprCall> -const+
+------+-------------------------------+
+errors+$::das_string -const           +
+------+-------------------------------+


to be documented


.. das:function:: AstFunctionAnnotation->apply (self;func;group;args;errors) 

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


.. das:function:: AstFunctionAnnotation->finish (self;func;group;args;progArgs;errors) 

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



.. das:class:: AstPassMacro

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstPassMacro->__finalize (self) 

to be documented


.. das:function:: AstPassMacro->apply (self;prog;mod) 

apply returns ::

 bool

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+
+mod +rtti::Module? const           +
+----+------------------------------+


to be documented



.. das:class:: AstReaderMacro

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstReaderMacro->__finalize (self) 

to be documented


.. das:function:: AstReaderMacro->accept (self;prog;mod;expr;ch;info) 

accept returns ::

 bool

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+
+mod +rtti::Module? const           +
+----+------------------------------+
+expr+ast::ExprReader? const        +
+----+------------------------------+
+ch  +int const                     +
+----+------------------------------+
+info+rtti::LineInfo const          +
+----+------------------------------+


to be documented


.. das:function:: AstReaderMacro->visit (self;prog;mod;expr) 

visit returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+prog+smart_ptr<rtti::Program> const  +
+----+--------------------------------+
+mod +rtti::Module? const             +
+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented



.. das:class:: AstStructureAnnotation

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstStructureAnnotation->__finalize (self) 

to be documented


.. das:function:: AstStructureAnnotation->apply (self;st;group;args;errors) 

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


.. das:function:: AstStructureAnnotation->finish (self;st;group;args;errors) 

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



.. das:class:: AstVariantMacro

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstVariantMacro->__finalize (self) 

to be documented


.. das:function:: AstVariantMacro->visitExprIsVariant (self;prog;mod;expr) 

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


.. das:function:: AstVariantMacro->visitExprAsVariant (self;prog;mod;expr) 

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


.. das:function:: AstVariantMacro->visitExprSafeAsVariant (self;prog;mod;expr) 

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



.. das:class:: AstVisitor

to be documented

it defines as follows ::

  __rtti : void?

.. das:function:: AstVisitor->__finalize (self) 

to be documented


.. das:function:: AstVisitor->preVisitProgram (self;prog) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitProgram (self;porg) 

function arguments are

+----+------------------------------+
+porg+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitProgramBody (self;prog) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitTypeDecl (self;typ) 

function arguments are

+---+------------------------------+
+typ+smart_ptr<ast::TypeDecl> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->visitTypeDecl (self;typ) 

visitTypeDecl returns ::

 smart_ptr<ast::TypeDecl>

function arguments are

+---+------------------------------+
+typ+smart_ptr<ast::TypeDecl> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitAlias (self;typ;name) 

function arguments are

+----+------------------------------+
+typ +smart_ptr<ast::TypeDecl> const+
+----+------------------------------+
+name+$::das_string const           +
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitAlias (self;typ;name) 

visitAlias returns ::

 smart_ptr<ast::TypeDecl>

function arguments are

+----+------------------------------+
+typ +smart_ptr<ast::TypeDecl> const+
+----+------------------------------+
+name+$::das_string const           +
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitEnumeration (self;enu) 

function arguments are

+---+---------------------------------+
+enu+smart_ptr<ast::Enumeration> const+
+---+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitEnumerationValue (self;enu;name;value;last) 

function arguments are

+-----+---------------------------------+
+enu  +smart_ptr<ast::Enumeration> const+
+-----+---------------------------------+
+name +$::das_string const              +
+-----+---------------------------------+
+value+smart_ptr<ast::Expression> const +
+-----+---------------------------------+
+last +bool const                       +
+-----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitEnumerationValue (self;enu;name;value;last) 

visitEnumerationValue returns ::

 smart_ptr<ast::Expression>

function arguments are

+-----+---------------------------------+
+enu  +smart_ptr<ast::Enumeration> const+
+-----+---------------------------------+
+name +$::das_string const              +
+-----+---------------------------------+
+value+smart_ptr<ast::Expression> const +
+-----+---------------------------------+
+last +bool const                       +
+-----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitEnumeration (self;enu) 

visitEnumeration returns ::

 smart_ptr<ast::Enumeration>

function arguments are

+---+---------------------------------+
+enu+smart_ptr<ast::Enumeration> const+
+---+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitStructure (self;str) 

function arguments are

+---+-------------------------------+
+str+smart_ptr<ast::Structure> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitStructureField (self;str;decl;last) 

function arguments are

+----+-------------------------------+
+str +smart_ptr<ast::Structure> const+
+----+-------------------------------+
+decl+ast::FieldDeclaration const    +
+----+-------------------------------+
+last+bool const                     +
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitStructureField (self;str;decl;last) 

function arguments are

+----+-------------------------------+
+str +smart_ptr<ast::Structure> const+
+----+-------------------------------+
+decl+ast::FieldDeclaration const    +
+----+-------------------------------+
+last+bool const                     +
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitStructure (self;str) 

visitStructure returns ::

 smart_ptr<ast::Structure>

function arguments are

+---+-------------------------------+
+str+smart_ptr<ast::Structure> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitFunction (self;fun) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->visitFunction (self;fun) 

visitFunction returns ::

 smart_ptr<ast::Function>

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitFunctionArgument (self;fun;arg;lastArg) 

function arguments are

+-------+------------------------------+
+fun    +smart_ptr<ast::Function> const+
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->visitFunctionArgument (self;fun;arg;lastArg) 

visitFunctionArgument returns ::

 smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+fun    +smart_ptr<ast::Function> const+
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitFunctionArgumentInit (self;fun;arg;value) 

function arguments are

+-----+--------------------------------+
+fun  +smart_ptr<ast::Function> const  +
+-----+--------------------------------+
+arg  +smart_ptr<ast::Variable> const  +
+-----+--------------------------------+
+value+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitFunctionArgumentInit (self;fun;arg;value) 

visitFunctionArgumentInit returns ::

 smart_ptr<ast::Expression>

function arguments are

+-----+--------------------------------+
+fun  +smart_ptr<ast::Function> const  +
+-----+--------------------------------+
+arg  +smart_ptr<ast::Variable> const  +
+-----+--------------------------------+
+value+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitFunctionBody (self;fun) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->visitFunctionBody (self;fun) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExpression (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExpression (self;expr) 

visitExpression returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlock (self;blk) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlock (self;blk) 

visitExprBlock returns ::

 smart_ptr<ast::Expression>

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlockArgument (self;blk;arg;lastArg) 

function arguments are

+-------+-------------------------------+
+blk    +smart_ptr<ast::ExprBlock> const+
+-------+-------------------------------+
+arg    +smart_ptr<ast::Variable> const +
+-------+-------------------------------+
+lastArg+bool const                     +
+-------+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlockArgument (self;blk;arg;lastArg) 

visitExprBlockArgument returns ::

 smart_ptr<ast::Variable>

function arguments are

+-------+-------------------------------+
+blk    +smart_ptr<ast::ExprBlock> const+
+-------+-------------------------------+
+arg    +smart_ptr<ast::Variable> const +
+-------+-------------------------------+
+lastArg+bool const                     +
+-------+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlockArgumentInit (self;blk;arg;expr) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlockArgumentInit (self;blk;arg;expr) 

visitExprBlockArgumentInit returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlockExpression (self;blk;expr) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlockExpression (self;blk;expr) 

visitExprBlockExpression returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlockFinal (self;blk) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlockFinal (self;blk) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBlockFinalExpression (self;blk;expr) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBlockFinalExpression (self;blk;expr) 

visitExprBlockFinalExpression returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLet (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprLet> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLet (self;expr) 

visitExprLet returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprLet> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLetVariable (self;expr;arg;lastArg) 

function arguments are

+-------+------------------------------+
+expr   +smart_ptr<ast::ExprLet> const +
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLetVariable (self;expr;arg;lastArg) 

visitExprLetVariable returns ::

 smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+expr   +smart_ptr<ast::ExprLet> const +
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLetVariableInit (self;blk;arg;expr) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprLet> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLetVariableInit (self;blk;arg;expr) 

visitExprLetVariableInit returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprLet> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitGlobalLet (self;prog) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitGlobalLet (self;prog) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitGlobalLetVariable (self;arg;lastArg) 

function arguments are

+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->visitGlobalLetVariable (self;arg;lastArg) 

visitGlobalLetVariable returns ::

 smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitGlobalLetVariableInit (self;arg;expr) 

function arguments are

+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitGlobalLetVariableInit (self;arg;expr) 

visitGlobalLetVariableInit returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprStringBuilder (self;expr) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprStringBuilder (self;expr) 

visitExprStringBuilder returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprStringBuilderElement (self;expr;elem;last) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+
+elem+smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprStringBuilderElement (self;expr;elem;last) 

visitExprStringBuilderElement returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+
+elem+smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNew (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprNew> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprNew (self;expr) 

visitExprNew returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprNew> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNewArgument (self;expr;arg;last) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprNew> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprNewArgument (self;expr;arg;last) 

visitExprNewArgument returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprNew> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNamedCall (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprNamedCall (self;expr) 

visitExprNamedCall returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNamedCallArgument (self;expr;arg;last) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+
+arg +smart_ptr<ast::MakeFieldDecl> const+
+----+-----------------------------------+
+last+bool const                         +
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprNamedCallArgument (self;expr;arg;last) 

visitExprNamedCallArgument returns ::

 smart_ptr<ast::MakeFieldDecl>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+
+arg +smart_ptr<ast::MakeFieldDecl> const+
+----+-----------------------------------+
+last+bool const                         +
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLooksLikeCall (self;expr) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLooksLikeCall (self;expr) 

visitExprLooksLikeCall returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLooksLikeCallArgument (self;expr;arg;last) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+
+arg +smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLooksLikeCallArgument (self;expr;arg;last) 

visitExprLooksLikeCallArgument returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+
+arg +smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCall (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCall> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprCall (self;expr) 

visitExprCall returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCall> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCallArgument (self;expr;arg;last) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprCall> const  +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprCallArgument (self;expr;arg;last) 

visitExprCallArgument returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprCall> const  +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNullCoalescing (self;expr) 

function arguments are

+----+----------------------------------------+
+expr+smart_ptr<ast::ExprNullCoalescing> const+
+----+----------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprNullCoalescing (self;expr) 

visitExprNullCoalescing returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------------+
+expr+smart_ptr<ast::ExprNullCoalescing> const+
+----+----------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprNullCoalescingDefault (self;expr;defval) 

function arguments are

+------+----------------------------------------+
+expr  +smart_ptr<ast::ExprNullCoalescing> const+
+------+----------------------------------------+
+defval+smart_ptr<ast::Expression> const        +
+------+----------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAt (self;expr) 

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprAt> const+
+----+----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprAt (self;expr) 

visitExprAt returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprAt> const+
+----+----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAtIndex (self;expr;index) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprAt> const    +
+-----+--------------------------------+
+index+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprSafeAt (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprSafeAt> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprSafeAt (self;expr) 

visitExprSafeAt returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprSafeAt> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprSafeAtIndex (self;expr;index) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprAt> const    +
+-----+--------------------------------+
+index+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIs (self;expr) 

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprIs> const+
+----+----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprIs (self;expr) 

visitExprIs returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprIs> const+
+----+----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIsType (self;expr;typeDecl) 

function arguments are

+--------+------------------------------+
+expr    +smart_ptr<ast::ExprAt> const  +
+--------+------------------------------+
+typeDecl+smart_ptr<ast::TypeDecl> const+
+--------+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp2 (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp2> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprOp2 (self;expr) 

visitExprOp2 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp2> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp2Right (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprOp2> const   +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp3 (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp3> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprOp3 (self;expr) 

visitExprOp3 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp3> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp3Left (self;expr;left) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprOp3> const   +
+----+--------------------------------+
+left+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp3Right (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprOp3> const   +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCopy (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCopy> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprCopy (self;expr) 

visitExprCopy returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCopy> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCopyRight (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprCopy> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMove (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprMove> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMove (self;expr) 

visitExprMove returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprMove> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMoveRight (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprMove> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprClone (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprClone> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprClone (self;expr) 

visitExprClone returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprClone> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCloneRight (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprClone> const +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprWith (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprWith> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprWith (self;expr) 

visitExprWith returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprWith> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprWithBody (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprWith> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprWhile (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprWhile> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprWhile (self;expr) 

visitExprWhile returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprWhile> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprWhileBody (self;expr;right) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprWhile> const +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprTryCatch (self;expr) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTryCatch> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprTryCatch (self;expr) 

visitExprTryCatch returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTryCatch> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprTryCatchCatch (self;expr;right) 

function arguments are

+-----+----------------------------------+
+expr +smart_ptr<ast::ExprTryCatch> const+
+-----+----------------------------------+
+right+smart_ptr<ast::Expression> const  +
+-----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIfThenElse (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprIfThenElse> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprIfThenElse (self;expr) 

visitExprIfThenElse returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprIfThenElse> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIfThenElseIfBlock (self;expr;ifBlock) 

function arguments are

+-------+------------------------------------+
+expr   +smart_ptr<ast::ExprIfThenElse> const+
+-------+------------------------------------+
+ifBlock+smart_ptr<ast::Expression> const    +
+-------+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIfThenElseElseBlock (self;expr;elseBlock) 

function arguments are

+---------+------------------------------------+
+expr     +smart_ptr<ast::ExprIfThenElse> const+
+---------+------------------------------------+
+elseBlock+smart_ptr<ast::Expression> const    +
+---------+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprFor (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprFor (self;expr) 

visitExprFor returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprForVariable (self;expr;svar;last) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFor> const +
+----+------------------------------+
+svar+smart_ptr<ast::Variable> const+
+----+------------------------------+
+last+bool const                    +
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprForVariable (self;expr;svar;last) 

visitExprForVariable returns ::

 smart_ptr<ast::Variable>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFor> const +
+----+------------------------------+
+svar+smart_ptr<ast::Variable> const+
+----+------------------------------+
+last+bool const                    +
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprForSource (self;expr;source;last) 

function arguments are

+------+--------------------------------+
+expr  +smart_ptr<ast::ExprFor> const   +
+------+--------------------------------+
+source+smart_ptr<ast::Expression> const+
+------+--------------------------------+
+last  +bool const                      +
+------+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprForSource (self;expr;source;last) 

visitExprForSource returns ::

 smart_ptr<ast::Expression>

function arguments are

+------+--------------------------------+
+expr  +smart_ptr<ast::ExprFor> const   +
+------+--------------------------------+
+source+smart_ptr<ast::Expression> const+
+------+--------------------------------+
+last  +bool const                      +
+------+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprForStack (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprForBody (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeVariant (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprMakeVariant> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeVariant (self;expr) 

visitExprMakeVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprMakeVariant> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeVariantField (self;expr;index;decl;last) 

function arguments are

+-----+-------------------------------------+
+expr +smart_ptr<ast::ExprMakeVariant> const+
+-----+-------------------------------------+
+index+int const                            +
+-----+-------------------------------------+
+decl +smart_ptr<ast::MakeFieldDecl> const  +
+-----+-------------------------------------+
+last +bool const                           +
+-----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeVariantField (self;expr;index;decl;last) 

visitExprMakeVariantField returns ::

 smart_ptr<ast::MakeFieldDecl>

function arguments are

+-----+-------------------------------------+
+expr +smart_ptr<ast::ExprMakeVariant> const+
+-----+-------------------------------------+
+index+int const                            +
+-----+-------------------------------------+
+decl +smart_ptr<ast::MakeFieldDecl> const  +
+-----+-------------------------------------+
+last +bool const                           +
+-----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeStruct (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprMakeStruct> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeStruct (self;expr) 

visitExprMakeStruct returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprMakeStruct> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeStructIndex (self;expr;index;last) 

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeStructIndex (self;expr;index;last) 

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeStructField (self;expr;index;decl;last) 

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+decl +smart_ptr<ast::MakeFieldDecl> const +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeStructField (self;expr;index;decl;last) 

visitExprMakeStructField returns ::

 smart_ptr<ast::MakeFieldDecl>

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+decl +smart_ptr<ast::MakeFieldDecl> const +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeArray (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeArray> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeArray (self;expr) 

visitExprMakeArray returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeArray> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeArrayIndex (self;expr;index;init;last) 

function arguments are

+-----+-----------------------------------+
+expr +smart_ptr<ast::ExprMakeArray> const+
+-----+-----------------------------------+
+index+int const                          +
+-----+-----------------------------------+
+init +smart_ptr<ast::Expression> const   +
+-----+-----------------------------------+
+last +bool const                         +
+-----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeArrayIndex (self;expr;index;init;last) 

visitExprMakeArrayIndex returns ::

 smart_ptr<ast::Expression>

function arguments are

+-----+-----------------------------------+
+expr +smart_ptr<ast::ExprMakeArray> const+
+-----+-----------------------------------+
+index+int const                          +
+-----+-----------------------------------+
+init +smart_ptr<ast::Expression> const   +
+-----+-----------------------------------+
+last +bool const                         +
+-----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeTuple (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeTuple> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeTuple (self;expr) 

visitExprMakeTuple returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeTuple> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeTupleIndex (self;expr;index;init;last) 

function arguments are

+-----+-----------------------------------+
+expr +smart_ptr<ast::ExprMakeTuple> const+
+-----+-----------------------------------+
+index+int const                          +
+-----+-----------------------------------+
+init +smart_ptr<ast::Expression> const   +
+-----+-----------------------------------+
+last +bool const                         +
+-----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeTupleIndex (self;expr;index;init;last) 

visitExprMakeTupleIndex returns ::

 smart_ptr<ast::Expression>

function arguments are

+-----+-----------------------------------+
+expr +smart_ptr<ast::ExprMakeTuple> const+
+-----+-----------------------------------+
+index+int const                          +
+-----+-----------------------------------+
+init +smart_ptr<ast::Expression> const   +
+-----+-----------------------------------+
+last +bool const                         +
+-----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprArrayComprehension (self;expr) 

function arguments are

+----+--------------------------------------------+
+expr+smart_ptr<ast::ExprArrayComprehension> const+
+----+--------------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprArrayComprehension (self;expr) 

visitExprArrayComprehension returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------------+
+expr+smart_ptr<ast::ExprArrayComprehension> const+
+----+--------------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprArrayComprehensionSubexpr (self;expr;subexrp) 

function arguments are

+-------+--------------------------------------------+
+expr   +smart_ptr<ast::ExprArrayComprehension> const+
+-------+--------------------------------------------+
+subexrp+smart_ptr<ast::Expression> const            +
+-------+--------------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprArrayComprehensionWhere (self;expr;filter) 

function arguments are

+------+--------------------------------------------+
+expr  +smart_ptr<ast::ExprArrayComprehension> const+
+------+--------------------------------------------+
+filter+smart_ptr<ast::Expression> const            +
+------+--------------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprTypeInfo (self;expr) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTypeInfo> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprTypeInfo (self;expr) 

visitExprTypeInfo returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTypeInfo> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprPtr2Ref (self;expr) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprPtr2Ref> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprPtr2Ref (self;expr) 

visitExprPtr2Ref returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprPtr2Ref> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprLabel (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprLabel> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprLabel (self;expr) 

visitExprLabel returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprLabel> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprGoto (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprGoto> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprGoto (self;expr) 

visitExprGoto returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprGoto> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprRef2Value (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprRef2Value> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprRef2Value (self;expr) 

visitExprRef2Value returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprRef2Value> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprRef2Ptr (self;expr) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprRef2Ptr> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprRef2Ptr (self;expr) 

visitExprRef2Ptr returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprRef2Ptr> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAddr (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprAddr> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprAddr (self;expr) 

visitExprAddr returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprAddr> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAssert (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAssert> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprAssert (self;expr) 

visitExprAssert returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAssert> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprStaticAssert (self;expr) 

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprStaticAssert> const+
+----+--------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprStaticAssert (self;expr) 

visitExprStaticAssert returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprStaticAssert> const+
+----+--------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprDebug (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprDebug> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprDebug (self;expr) 

visitExprDebug returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprDebug> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprInvoke (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprInvoke> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprInvoke (self;expr) 

visitExprInvoke returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprInvoke> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprErase (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprErase> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprErase (self;expr) 

visitExprErase returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprErase> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprFind (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFind> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprFind (self;expr) 

visitExprFind returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFind> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprKeyExists (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprKeyExists> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprKeyExists (self;expr) 

visitExprKeyExists returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprKeyExists> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAscend (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAscend> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprAscend (self;expr) 

visitExprAscend returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAscend> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprCast (self;expr) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCast> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprCast (self;expr) 

visitExprCast returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCast> const+
+----+------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprDelete (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprDelete> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprDelete (self;expr) 

visitExprDelete returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprDelete> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprVar (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprVar> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprVar (self;expr) 

visitExprVar returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprVar> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprField (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprField> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprField (self;expr) 

visitExprField returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprField> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprSafeField (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprSafeField> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprSafeField (self;expr) 

visitExprSafeField returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprSafeField> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprSwizzle (self;expr) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprSwizzle> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprSwizzle (self;expr) 

visitExprSwizzle returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprSwizzle> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprIsVariant (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprIsVariant (self;expr) 

visitExprIsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprAsVariant (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprAsVariant (self;expr) 

visitExprAsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprSafeAsVariant (self;expr) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprSafeAsVariant (self;expr) 

visitExprSafeAsVariant returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprOp1 (self;expr) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp1> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->visitExprOp1 (self;expr) 

visitExprOp1 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp1> const+
+----+-----------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprReturn (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReturn> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprReturn (self;expr) 

visitExprReturn returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReturn> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprYield (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprYield> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprYield (self;expr) 

visitExprYield returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprYield> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprBreak (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprBreak> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprBreak (self;expr) 

visitExprBreak returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprBreak> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprContinue (self;expr) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprContinue> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprContinue (self;expr) 

visitExprContinue returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprContinue> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeBlock (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeBlock> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeBlock (self;expr) 

visitExprMakeBlock returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeBlock> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMakeGenerator (self;expr) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprMakeGenerator> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMakeGenerator (self;expr) 

visitExprMakeGenerator returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprMakeGenerator> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprMemZero (self;expr) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprMemZero> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprMemZero (self;expr) 

visitExprMemZero returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprMemZero> const+
+----+---------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConst (self;expr) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprConst> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConst (self;expr) 

visitExprConst returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprConst> const+
+----+-------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstPtr (self;expr) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstPtr> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstPtr (self;expr) 

visitExprConstPtr returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstPtr> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstEnumeration (self;expr) 

function arguments are

+----+------------------------------------------+
+expr+smart_ptr<ast::ExprConstEnumeration> const+
+----+------------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstEnumeration (self;expr) 

visitExprConstEnumeration returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------------+
+expr+smart_ptr<ast::ExprConstEnumeration> const+
+----+------------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstBitfield (self;expr) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprConstBitfield> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstBitfield (self;expr) 

visitExprConstBitfield returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprConstBitfield> const+
+----+---------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt8 (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt8> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt8 (self;expr) 

visitExprConstInt8 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt8> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt16 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt16> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt16 (self;expr) 

visitExprConstInt16 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt16> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt64 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt64> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt64 (self;expr) 

visitExprConstInt64 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt64> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt (self;expr) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstInt> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt (self;expr) 

visitExprConstInt returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstInt> const+
+----+----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt2 (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt2> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt2 (self;expr) 

visitExprConstInt2 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt2> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt3 (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt3> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt3 (self;expr) 

visitExprConstInt3 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt3> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstInt4 (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt4> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstInt4 (self;expr) 

visitExprConstInt4 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt4> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt8 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt8> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt8 (self;expr) 

visitExprConstUInt8 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt8> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt16 (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt16> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt16 (self;expr) 

visitExprConstUInt16 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt16> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt64 (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt64> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt64 (self;expr) 

visitExprConstUInt64 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt64> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstUInt> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt (self;expr) 

visitExprConstUInt returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstUInt> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt2 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt2> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt2 (self;expr) 

visitExprConstUInt2 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt2> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt3 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt3> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt3 (self;expr) 

visitExprConstUInt3 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt3> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstUInt4 (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt4> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstUInt4 (self;expr) 

visitExprConstUInt4 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt4> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstRange (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstRange> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstRange (self;expr) 

visitExprConstRange returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstRange> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstURange (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstURange> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstURange (self;expr) 

visitExprConstURange returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstURange> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstBool (self;expr) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstBool> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstBool (self;expr) 

visitExprConstBool returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstBool> const+
+----+-----------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstFloat (self;expr) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstFloat (self;expr) 

visitExprConstFloat returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat> const+
+----+------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstFloat2 (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat2> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstFloat2 (self;expr) 

visitExprConstFloat2 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat2> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstFloat3 (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat3> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstFloat3 (self;expr) 

visitExprConstFloat3 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat3> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstFloat4 (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat4> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstFloat4 (self;expr) 

visitExprConstFloat4 returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat4> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstString (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstString> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstString (self;expr) 

visitExprConstString returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstString> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprConstDouble (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstDouble> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprConstDouble (self;expr) 

visitExprConstDouble returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstDouble> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprFakeContext (self;expr) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprFakeContext> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprFakeContext (self;expr) 

visitExprFakeContext returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprFakeContext> const+
+----+-------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprFakeLineInfo (self;expr) 

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprFakeLineInfo> const+
+----+--------------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprFakeLineInfo (self;expr) 

visitExprFakeLineInfo returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprFakeLineInfo> const+
+----+--------------------------------------+


to be documented


.. das:function:: AstVisitor->preVisitExprReader (self;expr) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented


.. das:function:: AstVisitor->visitExprReader (self;expr) 

visitExprReader returns ::

 smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: ExpressionPtr(expr)

ExpressionPtr returns ::

 smart_ptr<ast::Expression>



function arguments are

+----+-------------------------+
+expr+smart_ptr<auto(TT)> const+
+----+-------------------------+



to be documented


.. das:function:: add_new_function_annotation(name;someClassPtr)

add_new_function_annotation returns ::

 auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_reader_macro(name;someClassPtr)

add_new_reader_macro returns ::

 auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_structure_annotation(name;someClassPtr)

add_new_structure_annotation returns ::

 auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_variant_macro(name;someClassPtr)

add_new_variant_macro returns ::

 auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: describe(expr)

describe returns ::

 auto



function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+



to be documented


.. das:function:: describe(expr)

describe returns ::

 auto



function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::Function> const+
+----+------------------------------+



to be documented


.. das:function:: describe(decl;extra;contracts;modules)

describe returns ::

 auto



function arguments are

+---------+------------------------------+
+decl     +smart_ptr<ast::TypeDecl> const+
+---------+------------------------------+
+extra    +bool const                    +
+---------+------------------------------+
+contracts+bool const                    +
+---------+------------------------------+
+modules  +bool const                    +
+---------+------------------------------+



to be documented


.. das:function:: make_function_annotation(name;someClassPtr)

make_function_annotation returns ::

 smart_ptr<ast::FunctionAnnotation>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_pass_macro(name;someClassPtr)

make_pass_macro returns ::

 smart_ptr<ast::PassMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_reader_macro(name;someClassPtr)

make_reader_macro returns ::

 smart_ptr<ast::ReaderMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_structure_annotation(name;someClassPtr)

make_structure_annotation returns ::

 smart_ptr<ast::StructureAnnotation>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_variant_macro(name;someClassPtr)

make_variant_macro returns ::

 smart_ptr<ast::VariantMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_visitor(someClass)

make_visitor returns ::

 smart_ptr<ast::VisitorAdapter>



function arguments are

+---------+----------+
+someClass+auto const+
+---------+----------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: add_dirty_infer_macro(arg0;arg1;arg2)



function arguments are

+----+----------------------------------------+
+arg0+rtti::Module? const implicit            +
+----+----------------------------------------+
+arg1+smart_ptr<ast::PassMacro> const implicit+
+----+----------------------------------------+
+arg2+ const                                  +
+----+----------------------------------------+



to be documented


.. das:function:: add_function(arg0;arg1;arg2)



function arguments are

+----+---------------------------------------+
+arg0+rtti::Module? const implicit           +
+----+---------------------------------------+
+arg1+smart_ptr<ast::Function> const implicit+
+----+---------------------------------------+
+arg2+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: add_function_annotation(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------+
+arg0+rtti::Module? const implicit                     +
+----+-------------------------------------------------+
+arg1+smart_ptr<ast::FunctionAnnotation> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: add_function_annotation(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------+
+arg0+smart_ptr<ast::Function> const implicit          +
+----+-------------------------------------------------+
+arg1+smart_ptr<ast::FunctionAnnotation> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: add_reader_macro(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------+
+arg0+rtti::Module? const implicit              +
+----+------------------------------------------+
+arg1+smart_ptr<ast::ReaderMacro> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: add_structure_annotation(arg0;arg1;arg2)



function arguments are

+----+--------------------------------------------------+
+arg0+rtti::Module? const implicit                      +
+----+--------------------------------------------------+
+arg1+smart_ptr<ast::StructureAnnotation> const implicit+
+----+--------------------------------------------------+
+arg2+ const                                            +
+----+--------------------------------------------------+



to be documented


.. das:function:: add_structure_annotation(arg0;arg1;arg2)



function arguments are

+----+--------------------------------------------------+
+arg0+smart_ptr<ast::Structure> const implicit          +
+----+--------------------------------------------------+
+arg1+smart_ptr<ast::StructureAnnotation> const implicit+
+----+--------------------------------------------------+
+arg2+ const                                            +
+----+--------------------------------------------------+



to be documented


.. das:function:: add_variant_macro(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------+
+arg0+rtti::Module? const implicit               +
+----+-------------------------------------------+
+arg1+smart_ptr<ast::VariantMacro> const implicit+
+----+-------------------------------------------+
+arg2+ const                                     +
+----+-------------------------------------------+



to be documented


.. das:function:: any_array_foreach(arg0;arg1;arg2;arg3)



function arguments are

+----+----------------------------------+
+arg0+void? const implicit              +
+----+----------------------------------+
+arg1+int const                         +
+----+----------------------------------+
+arg2+block<(void?):void> const implicit+
+----+----------------------------------+
+arg3+ const                            +
+----+----------------------------------+



to be documented


.. das:function:: any_array_size(arg0)

any_array_size returns ::

 int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: any_table_foreach(arg0;arg1;arg2;arg3;arg4)



function arguments are

+----+----------------------------------------+
+arg0+void? const implicit                    +
+----+----------------------------------------+
+arg1+int const                               +
+----+----------------------------------------+
+arg2+int const                               +
+----+----------------------------------------+
+arg3+block<(void?;void?):void> const implicit+
+----+----------------------------------------+
+arg4+ const                                  +
+----+----------------------------------------+



to be documented


.. das:function:: any_table_size(arg0)

any_table_size returns ::

 int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: clone_expression(arg0)

clone_expression returns ::

 smart_ptr<ast::Expression>



function arguments are

+----+-----------------------------------------+
+arg0+smart_ptr<ast::Expression> const implicit+
+----+-----------------------------------------+



to be documented


.. das:function:: clone_type(arg0)

clone_type returns ::

 smart_ptr<ast::TypeDecl>



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+



to be documented


.. das:function:: compiling_module

compiling_module returns ::

 rtti::Module?




to be documented


.. das:function:: compiling_program

compiling_program returns ::

 smart_ptr<rtti::Program>




to be documented


.. das:function:: das_to_string(arg0;arg1)

das_to_string returns ::

 string



function arguments are

+----+----------------+
+arg0+rtti::Type const+
+----+----------------+
+arg1+ const          +
+----+----------------+



to be documented


.. das:function:: describe_expression(arg0;arg1)

describe_expression returns ::

 string



function arguments are

+----+-----------------------------------------+
+arg0+smart_ptr<ast::Expression> const implicit+
+----+-----------------------------------------+
+arg1+ const                                   +
+----+-----------------------------------------+



to be documented


.. das:function:: describe_function(arg0;arg1)

describe_function returns ::

 string



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::Function> const implicit+
+----+---------------------------------------+
+arg1+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: describe_typedecl(arg0;arg1;arg2;arg3;arg4)

describe_typedecl returns ::

 string



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+bool const                             +
+----+---------------------------------------+
+arg2+bool const                             +
+----+---------------------------------------+
+arg3+bool const                             +
+----+---------------------------------------+
+arg4+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: find_bitfield_name(arg0;arg1;arg2)

find_bitfield_name returns ::

 string



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+bitfield const                         +
+----+---------------------------------------+
+arg2+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: for_each_enumeration(arg0;arg1;arg2)



function arguments are

+----+--------------------------------------------------------+
+arg0+rtti::Module? const implicit                            +
+----+--------------------------------------------------------+
+arg1+block<(smart_ptr<ast::Enumeration>):void> const implicit+
+----+--------------------------------------------------------+
+arg2+ const                                                  +
+----+--------------------------------------------------------+



to be documented


.. das:function:: for_each_field(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------------------------------------+
+arg0+rtti::BasicStructureAnnotation const implicit                           +
+----+------------------------------------------------------------------------+
+arg1+block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit+
+----+------------------------------------------------------------------------+
+arg2+ const                                                                  +
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_function(arg0;arg1;arg2;arg3;arg4)



function arguments are

+----+-----------------------------------------------------+
+arg0+rtti::Module? const implicit                         +
+----+-----------------------------------------------------+
+arg1+string const                                         +
+----+-----------------------------------------------------+
+arg2+block<(smart_ptr<ast::Function>):void> const implicit+
+----+-----------------------------------------------------+
+arg3+ const                                               +
+----+-----------------------------------------------------+
+arg4+ const                                               +
+----+-----------------------------------------------------+



to be documented


.. das:function:: for_each_generic(arg0;arg1;arg2)



function arguments are

+----+-----------------------------------------------------+
+arg0+rtti::Module? const implicit                         +
+----+-----------------------------------------------------+
+arg1+block<(smart_ptr<ast::Function>):void> const implicit+
+----+-----------------------------------------------------+
+arg2+ const                                               +
+----+-----------------------------------------------------+



to be documented


.. das:function:: for_each_structure(arg0;arg1;arg2)



function arguments are

+----+------------------------------------------------------+
+arg0+rtti::Module? const implicit                          +
+----+------------------------------------------------------+
+arg1+block<(smart_ptr<ast::Structure>):void> const implicit+
+----+------------------------------------------------------+
+arg2+ const                                                +
+----+------------------------------------------------------+



to be documented


.. das:function:: for_each_typedef(arg0;arg1;arg2)



function arguments are

+----+-------------------------------------------------------------+
+arg0+rtti::Module? const implicit                                 +
+----+-------------------------------------------------------------+
+arg1+block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit+
+----+-------------------------------------------------------------+
+arg2+ const                                                       +
+----+-------------------------------------------------------------+



to be documented


.. das:function:: get_tuple_field_offset(arg0;arg1)

get_tuple_field_offset returns ::

 int



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+int const                              +
+----+---------------------------------------+



to be documented


.. das:function:: get_variant_field_offset(arg0;arg1)

get_variant_field_offset returns ::

 int



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+int const                              +
+----+---------------------------------------+



to be documented


.. das:function:: is_same_type(arg0;arg1;arg2;arg3;arg4)

is_same_type returns ::

 bool



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg2+rtti::RefMatters const                 +
+----+---------------------------------------+
+arg3+rtti::ConstMatters const               +
+----+---------------------------------------+
+arg4+rtti::TemporaryMatters const           +
+----+---------------------------------------+



to be documented


.. das:function:: macro_error(arg0;arg1;arg2)



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit+
+----+---------------------------------------+
+arg1+rtti::LineInfo const implicit          +
+----+---------------------------------------+
+arg2+string const                           +
+----+---------------------------------------+



to be documented


.. das:function:: make_function_annotation(arg0;arg1;arg2;arg3)

make_function_annotation returns ::

 smart_ptr<ast::FunctionAnnotation>



function arguments are

+----+--------------------------------------+
+arg0+string const                          +
+----+--------------------------------------+
+arg1+void? const implicit                  +
+----+--------------------------------------+
+arg2+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg3+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: make_pass_macro(arg0;arg1;arg2;arg3)

make_pass_macro returns ::

 smart_ptr<ast::PassMacro>



function arguments are

+----+--------------------------------------+
+arg0+string const                          +
+----+--------------------------------------+
+arg1+void? const implicit                  +
+----+--------------------------------------+
+arg2+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg3+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: make_reader_macro(arg0;arg1;arg2;arg3)

make_reader_macro returns ::

 smart_ptr<ast::ReaderMacro>



function arguments are

+----+--------------------------------------+
+arg0+string const                          +
+----+--------------------------------------+
+arg1+void? const implicit                  +
+----+--------------------------------------+
+arg2+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg3+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: make_structure_annotation(arg0;arg1;arg2;arg3)

make_structure_annotation returns ::

 smart_ptr<ast::StructureAnnotation>



function arguments are

+----+--------------------------------------+
+arg0+string const                          +
+----+--------------------------------------+
+arg1+void? const implicit                  +
+----+--------------------------------------+
+arg2+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg3+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: make_variant_macro(arg0;arg1;arg2;arg3)

make_variant_macro returns ::

 smart_ptr<ast::VariantMacro>



function arguments are

+----+--------------------------------------+
+arg0+string const                          +
+----+--------------------------------------+
+arg1+void? const implicit                  +
+----+--------------------------------------+
+arg2+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg3+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: make_visitor(arg0;arg1;arg2)

make_visitor returns ::

 smart_ptr<ast::VisitorAdapter>



function arguments are

+----+--------------------------------------+
+arg0+void? const implicit                  +
+----+--------------------------------------+
+arg1+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg2+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: this_module(arg0;arg1)

this_module returns ::

 rtti::Module?



function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: this_program(arg0)

this_program returns ::

 smart_ptr<rtti::Program>



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: visit(arg0;arg1)



function arguments are

+----+---------------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit      +
+----+---------------------------------------------+
+arg1+smart_ptr<ast::VisitorAdapter> const implicit+
+----+---------------------------------------------+



to be documented



