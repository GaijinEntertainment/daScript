
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
  cppName    : das_string
  list       : ast::dasvector`pair`das_string`smart_ptr`Expression
  name       : das_string
  external   : bool
  baseType   : rtti::Type

to be documented


.. das:attribute:: ExprAddr

it defines as follows ::

  func       : ast::Function?
  target     : das_string
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
  name       : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  op         : das_string
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

  value      : das_string
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

  value      : das_string
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
  op                 : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  name       : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  name       : das_string
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
  name                   : das_string
  argumentsFailedToInfer : bool
  genFlags               : bitfield<alwaysSafe;generated>
  _type                  : smart_ptr<ast::TypeDecl>
  flags                  : bitfield<constexpression;noSideEffects;noNativeSideEffects>
  __rtti                 : string const

to be documented


.. das:attribute:: ExprLabel

it defines as follows ::

  comment    : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  op         : das_string
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
  name                   : das_string
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
  name                   : das_string
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
  op         : das_string
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
  op         : das_string
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
  op         : das_string
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
  sequence   : das_string
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
  name       : das_string
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
  name       : das_string
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
  name                   : das_string
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
  mask       : das_string
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
  extratrait : das_string
  macro      : ast::TypeInfoMacro?
  subtrait   : das_string
  at         : rtti::LineInfo
  trait      : das_string
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
  name          : das_string
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
  name       : das_string
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
  name            : das_string
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
  name  : das_string
  flags : bitfield<moveSemantics;cloneSemantics>

to be documented


.. das:attribute:: ModuleGroup

to be documented


.. das:attribute:: ModuleLibrary

to be documented


.. das:attribute:: PassMacro

it defines as follows ::

  name : das_string

to be documented


.. das:attribute:: ReaderMacro

it defines as follows ::

  _module : rtti::Module?
  name    : das_string

to be documented


.. das:attribute:: Structure

it defines as follows ::

  _module     : rtti::Module?
  at          : rtti::LineInfo
  parent      : ast::Structure?
  annotations : rtti::AnnotationList
  name        : das_string
  fields      : ast::dasvector`FieldDeclaration
  flags       : bitfield<isClass;genCtor;cppLayout;cppLayoutNotPod;generated;persistent;isLambda>

to be documented


.. das:attribute:: StructureAnnotation

to be documented


.. das:attribute:: TypeDecl

it defines as follows ::

  alias      : das_string
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
  name    : das_string

to be documented


.. das:attribute:: Variable

it defines as follows ::

  annotation    : rtti::AnnotationArgumentList
  initStackSize : uint
  _module       : rtti::Module?
  index         : int
  at            : rtti::LineInfo
  stackTop      : uint
  name          : das_string
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

  name : das_string

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

.. das:method:: AstFunctionAnnotation.transform (self:ast::AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string) 

transform returns smart_ptr<ast::Expression>

function arguments are

+------+------------------------+
+call  +smart_ptr<ast::ExprCall>+
+------+------------------------+
+errors+das_string              +
+------+------------------------+


to be documented


.. das:method:: AstFunctionAnnotation.apply (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

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


.. das:method:: AstFunctionAnnotation.finish (self:ast::AstFunctionAnnotation; func:smart_ptr<ast::Function>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; progArgs:rtti::AnnotationArgumentList const; errors:das_string) 

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



.. das:class:: AstPassMacro

to be documented

it defines as follows ::

  __rtti : void?

.. das:method:: AstPassMacro.apply (self:ast::AstPassMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const) 

apply returns bool

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

.. das:method:: AstReaderMacro.accept (self:ast::AstReaderMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:rtti::LineInfo const) 

accept returns bool

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


.. das:method:: AstReaderMacro.visit (self:ast::AstReaderMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const) 

visit returns smart_ptr<ast::Expression>

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

.. das:method:: AstStructureAnnotation.apply (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

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


.. das:method:: AstStructureAnnotation.finish (self:ast::AstStructureAnnotation; st:smart_ptr<ast::Structure>; group:ast::ModuleGroup; args:rtti::AnnotationArgumentList const; errors:das_string) 

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



.. das:class:: AstVariantMacro

to be documented

it defines as follows ::

  __rtti : void?

.. das:method:: AstVariantMacro.visitExprIsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const) 

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


.. das:method:: AstVariantMacro.visitExprAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const) 

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


.. das:method:: AstVariantMacro.visitExprSafeAsVariant (self:ast::AstVariantMacro; prog:smart_ptr<rtti::Program> const; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const) 

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



.. das:class:: AstVisitor

to be documented

it defines as follows ::

  __rtti : void?

.. das:method:: AstVisitor.preVisitProgram (self:ast::AstVisitor; prog:smart_ptr<rtti::Program> const) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitProgram (self:ast::AstVisitor; porg:smart_ptr<rtti::Program> const) 

function arguments are

+----+------------------------------+
+porg+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitProgramBody (self:ast::AstVisitor; prog:smart_ptr<rtti::Program> const) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitTypeDecl (self:ast::AstVisitor; typ:smart_ptr<ast::TypeDecl> const) 

function arguments are

+---+------------------------------+
+typ+smart_ptr<ast::TypeDecl> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.visitTypeDecl (self:ast::AstVisitor; typ:smart_ptr<ast::TypeDecl> const) 

visitTypeDecl returns smart_ptr<ast::TypeDecl>

function arguments are

+---+------------------------------+
+typ+smart_ptr<ast::TypeDecl> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitAlias (self:ast::AstVisitor; typ:smart_ptr<ast::TypeDecl> const; name:das_string const) 

function arguments are

+----+------------------------------+
+typ +smart_ptr<ast::TypeDecl> const+
+----+------------------------------+
+name+das_string const              +
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitAlias (self:ast::AstVisitor; typ:smart_ptr<ast::TypeDecl> const; name:das_string const) 

visitAlias returns smart_ptr<ast::TypeDecl>

function arguments are

+----+------------------------------+
+typ +smart_ptr<ast::TypeDecl> const+
+----+------------------------------+
+name+das_string const              +
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitEnumeration (self:ast::AstVisitor; enu:smart_ptr<ast::Enumeration> const) 

function arguments are

+---+---------------------------------+
+enu+smart_ptr<ast::Enumeration> const+
+---+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitEnumerationValue (self:ast::AstVisitor; enu:smart_ptr<ast::Enumeration> const; name:das_string const; value:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+-----+---------------------------------+
+enu  +smart_ptr<ast::Enumeration> const+
+-----+---------------------------------+
+name +das_string const                 +
+-----+---------------------------------+
+value+smart_ptr<ast::Expression> const +
+-----+---------------------------------+
+last +bool const                       +
+-----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitEnumerationValue (self:ast::AstVisitor; enu:smart_ptr<ast::Enumeration> const; name:das_string const; value:smart_ptr<ast::Expression> const; last:bool const) 

visitEnumerationValue returns smart_ptr<ast::Expression>

function arguments are

+-----+---------------------------------+
+enu  +smart_ptr<ast::Enumeration> const+
+-----+---------------------------------+
+name +das_string const                 +
+-----+---------------------------------+
+value+smart_ptr<ast::Expression> const +
+-----+---------------------------------+
+last +bool const                       +
+-----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitEnumeration (self:ast::AstVisitor; enu:smart_ptr<ast::Enumeration> const) 

visitEnumeration returns smart_ptr<ast::Enumeration>

function arguments are

+---+---------------------------------+
+enu+smart_ptr<ast::Enumeration> const+
+---+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitStructure (self:ast::AstVisitor; str:smart_ptr<ast::Structure> const) 

function arguments are

+---+-------------------------------+
+str+smart_ptr<ast::Structure> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitStructureField (self:ast::AstVisitor; str:smart_ptr<ast::Structure> const; decl:ast::FieldDeclaration const; last:bool const) 

function arguments are

+----+-------------------------------+
+str +smart_ptr<ast::Structure> const+
+----+-------------------------------+
+decl+ast::FieldDeclaration const    +
+----+-------------------------------+
+last+bool const                     +
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitStructureField (self:ast::AstVisitor; str:smart_ptr<ast::Structure> const; decl:ast::FieldDeclaration const; last:bool const) 

function arguments are

+----+-------------------------------+
+str +smart_ptr<ast::Structure> const+
+----+-------------------------------+
+decl+ast::FieldDeclaration const    +
+----+-------------------------------+
+last+bool const                     +
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitStructure (self:ast::AstVisitor; str:smart_ptr<ast::Structure> const) 

visitStructure returns smart_ptr<ast::Structure>

function arguments are

+---+-------------------------------+
+str+smart_ptr<ast::Structure> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunction (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunction (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const) 

visitFunction returns smart_ptr<ast::Function>

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionArgument (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

function arguments are

+-------+------------------------------+
+fun    +smart_ptr<ast::Function> const+
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionArgument (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

visitFunctionArgument returns smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+fun    +smart_ptr<ast::Function> const+
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionArgumentInit (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const; arg:smart_ptr<ast::Variable> const; value:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+fun  +smart_ptr<ast::Function> const  +
+-----+--------------------------------+
+arg  +smart_ptr<ast::Variable> const  +
+-----+--------------------------------+
+value+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionArgumentInit (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const; arg:smart_ptr<ast::Variable> const; value:smart_ptr<ast::Expression> const) 

visitFunctionArgumentInit returns smart_ptr<ast::Expression>

function arguments are

+-----+--------------------------------+
+fun  +smart_ptr<ast::Function> const  +
+-----+--------------------------------+
+arg  +smart_ptr<ast::Variable> const  +
+-----+--------------------------------+
+value+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionBody (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionBody (self:ast::AstVisitor; fun:smart_ptr<ast::Function> const) 

function arguments are

+---+------------------------------+
+fun+smart_ptr<ast::Function> const+
+---+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExpression (self:ast::AstVisitor; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExpression (self:ast::AstVisitor; expr:smart_ptr<ast::Expression> const) 

visitExpression returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlock (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlock (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const) 

visitExprBlock returns smart_ptr<ast::Expression>

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockArgument (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

function arguments are

+-------+-------------------------------+
+blk    +smart_ptr<ast::ExprBlock> const+
+-------+-------------------------------+
+arg    +smart_ptr<ast::Variable> const +
+-------+-------------------------------+
+lastArg+bool const                     +
+-------+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockArgument (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

visitExprBlockArgument returns smart_ptr<ast::Variable>

function arguments are

+-------+-------------------------------+
+blk    +smart_ptr<ast::ExprBlock> const+
+-------+-------------------------------+
+arg    +smart_ptr<ast::Variable> const +
+-------+-------------------------------+
+lastArg+bool const                     +
+-------+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockArgumentInit (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockArgumentInit (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

visitExprBlockArgumentInit returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockExpression (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockExpression (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:smart_ptr<ast::Expression> const) 

visitExprBlockExpression returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockFinal (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockFinal (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const) 

function arguments are

+---+-------------------------------+
+blk+smart_ptr<ast::ExprBlock> const+
+---+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockFinalExpression (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockFinalExpression (self:ast::AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:smart_ptr<ast::Expression> const) 

visitExprBlockFinalExpression returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprBlock> const +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLet (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLet> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprLet> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLet (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLet> const) 

visitExprLet returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprLet> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLetVariable (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

function arguments are

+-------+------------------------------+
+expr   +smart_ptr<ast::ExprLet> const +
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLetVariable (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

visitExprLetVariable returns smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+expr   +smart_ptr<ast::ExprLet> const +
+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLetVariableInit (self:ast::AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprLet> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLetVariableInit (self:ast::AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

visitExprLetVariableInit returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+blk +smart_ptr<ast::ExprLet> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLet (self:ast::AstVisitor; prog:smart_ptr<rtti::Program> const) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLet (self:ast::AstVisitor; prog:smart_ptr<rtti::Program> const) 

function arguments are

+----+------------------------------+
+prog+smart_ptr<rtti::Program> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLetVariable (self:ast::AstVisitor; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

function arguments are

+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLetVariable (self:ast::AstVisitor; arg:smart_ptr<ast::Variable> const; lastArg:bool const) 

visitGlobalLetVariable returns smart_ptr<ast::Variable>

function arguments are

+-------+------------------------------+
+arg    +smart_ptr<ast::Variable> const+
+-------+------------------------------+
+lastArg+bool const                    +
+-------+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLetVariableInit (self:ast::AstVisitor; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLetVariableInit (self:ast::AstVisitor; arg:smart_ptr<ast::Variable> const; expr:smart_ptr<ast::Expression> const) 

visitGlobalLetVariableInit returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+arg +smart_ptr<ast::Variable> const  +
+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStringBuilder (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStringBuilder (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const) 

visitExprStringBuilder returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStringBuilderElement (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+
+elem+smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStringBuilderElement (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:smart_ptr<ast::Expression> const; last:bool const) 

visitExprStringBuilderElement returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprStringBuilder> const+
+----+---------------------------------------+
+elem+smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNew (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNew> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprNew> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNew (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNew> const) 

visitExprNew returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprNew> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNewArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprNew> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNewArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

visitExprNewArgument returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprNew> const   +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNamedCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNamedCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const) 

visitExprNamedCall returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNamedCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+
+arg +smart_ptr<ast::MakeFieldDecl> const+
+----+-----------------------------------+
+last+bool const                         +
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNamedCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

visitExprNamedCallArgument returns smart_ptr<ast::MakeFieldDecl>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprNamedCall> const+
+----+-----------------------------------+
+arg +smart_ptr<ast::MakeFieldDecl> const+
+----+-----------------------------------+
+last+bool const                         +
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLooksLikeCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLooksLikeCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const) 

visitExprLooksLikeCall returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLooksLikeCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+
+arg +smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLooksLikeCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

visitExprLooksLikeCallArgument returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprLooksLikeCall> const+
+----+---------------------------------------+
+arg +smart_ptr<ast::Expression> const       +
+----+---------------------------------------+
+last+bool const                             +
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCall> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCall> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCall (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCall> const) 

visitExprCall returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCall> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprCall> const  +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCallArgument (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:smart_ptr<ast::Expression> const; last:bool const) 

visitExprCallArgument returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprCall> const  +
+----+--------------------------------+
+arg +smart_ptr<ast::Expression> const+
+----+--------------------------------+
+last+bool const                      +
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNullCoalescing (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const) 

function arguments are

+----+----------------------------------------+
+expr+smart_ptr<ast::ExprNullCoalescing> const+
+----+----------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNullCoalescing (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const) 

visitExprNullCoalescing returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------------+
+expr+smart_ptr<ast::ExprNullCoalescing> const+
+----+----------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNullCoalescingDefault (self:ast::AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const; defval:smart_ptr<ast::Expression> const) 

function arguments are

+------+----------------------------------------+
+expr  +smart_ptr<ast::ExprNullCoalescing> const+
+------+----------------------------------------+
+defval+smart_ptr<ast::Expression> const        +
+------+----------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAt> const) 

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprAt> const+
+----+----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAt> const) 

visitExprAt returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprAt> const+
+----+----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAtIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprAt> const    +
+-----+--------------------------------+
+index+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprSafeAt> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeAt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const) 

visitExprSafeAt returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprSafeAt> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAtIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprAt> const    +
+-----+--------------------------------+
+index+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIs (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIs> const) 

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprIs> const+
+----+----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIs (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIs> const) 

visitExprIs returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------+
+expr+smart_ptr<ast::ExprIs> const+
+----+----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIsType (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAt> const; typeDecl:smart_ptr<ast::TypeDecl> const) 

function arguments are

+--------+------------------------------+
+expr    +smart_ptr<ast::ExprAt> const  +
+--------+------------------------------+
+typeDecl+smart_ptr<ast::TypeDecl> const+
+--------+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp2> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp2> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp2> const) 

visitExprOp2 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp2> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp2Right (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp2> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprOp2> const   +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp3> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp3> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp3> const) 

visitExprOp3 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp3> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3Left (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp3> const; left:smart_ptr<ast::Expression> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprOp3> const   +
+----+--------------------------------+
+left+smart_ptr<ast::Expression> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3Right (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp3> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprOp3> const   +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCopy (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCopy> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCopy> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCopy (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCopy> const) 

visitExprCopy returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCopy> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCopyRight (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCopy> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprCopy> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMove (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMove> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprMove> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMove (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMove> const) 

visitExprMove returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprMove> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMoveRight (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMove> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprMove> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprClone (self:ast::AstVisitor; expr:smart_ptr<ast::ExprClone> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprClone> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprClone (self:ast::AstVisitor; expr:smart_ptr<ast::ExprClone> const) 

visitExprClone returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprClone> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCloneRight (self:ast::AstVisitor; expr:smart_ptr<ast::ExprClone> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprClone> const +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWith (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWith> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprWith> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprWith (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWith> const) 

visitExprWith returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprWith> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWithBody (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWith> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprWith> const  +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWhile (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWhile> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprWhile> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprWhile (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWhile> const) 

visitExprWhile returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprWhile> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWhileBody (self:ast::AstVisitor; expr:smart_ptr<ast::ExprWhile> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+--------------------------------+
+expr +smart_ptr<ast::ExprWhile> const +
+-----+--------------------------------+
+right+smart_ptr<ast::Expression> const+
+-----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTryCatch (self:ast::AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTryCatch> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprTryCatch (self:ast::AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const) 

visitExprTryCatch returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTryCatch> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTryCatchCatch (self:ast::AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const; right:smart_ptr<ast::Expression> const) 

function arguments are

+-----+----------------------------------+
+expr +smart_ptr<ast::ExprTryCatch> const+
+-----+----------------------------------+
+right+smart_ptr<ast::Expression> const  +
+-----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElse (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprIfThenElse> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIfThenElse (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const) 

visitExprIfThenElse returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprIfThenElse> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElseIfBlock (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; ifBlock:smart_ptr<ast::Expression> const) 

function arguments are

+-------+------------------------------------+
+expr   +smart_ptr<ast::ExprIfThenElse> const+
+-------+------------------------------------+
+ifBlock+smart_ptr<ast::Expression> const    +
+-------+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElseElseBlock (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; elseBlock:smart_ptr<ast::Expression> const) 

function arguments are

+---------+------------------------------------+
+expr     +smart_ptr<ast::ExprIfThenElse> const+
+---------+------------------------------------+
+elseBlock+smart_ptr<ast::Expression> const    +
+---------+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFor (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFor (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const) 

visitExprFor returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForVariable (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:smart_ptr<ast::Variable> const; last:bool const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFor> const +
+----+------------------------------+
+svar+smart_ptr<ast::Variable> const+
+----+------------------------------+
+last+bool const                    +
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprForVariable (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:smart_ptr<ast::Variable> const; last:bool const) 

visitExprForVariable returns smart_ptr<ast::Variable>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFor> const +
+----+------------------------------+
+svar+smart_ptr<ast::Variable> const+
+----+------------------------------+
+last+bool const                    +
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForSource (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:smart_ptr<ast::Expression> const; last:bool const) 

function arguments are

+------+--------------------------------+
+expr  +smart_ptr<ast::ExprFor> const   +
+------+--------------------------------+
+source+smart_ptr<ast::Expression> const+
+------+--------------------------------+
+last  +bool const                      +
+------+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprForSource (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:smart_ptr<ast::Expression> const; last:bool const) 

visitExprForSource returns smart_ptr<ast::Expression>

function arguments are

+------+--------------------------------+
+expr  +smart_ptr<ast::ExprFor> const   +
+------+--------------------------------+
+source+smart_ptr<ast::Expression> const+
+------+--------------------------------+
+last  +bool const                      +
+------+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForStack (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForBody (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFor> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprFor> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprMakeVariant> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const) 

visitExprMakeVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprMakeVariant> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeVariantField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

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


.. das:method:: AstVisitor.visitExprMakeVariantField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

visitExprMakeVariantField returns smart_ptr<ast::MakeFieldDecl>

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


.. das:method:: AstVisitor.preVisitExprMakeStruct (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprMakeStruct> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeStruct (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const) 

visitExprMakeStruct returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprMakeStruct> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeStructIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const) 

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeStructIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const) 

function arguments are

+-----+------------------------------------+
+expr +smart_ptr<ast::ExprMakeStruct> const+
+-----+------------------------------------+
+index+int const                           +
+-----+------------------------------------+
+last +bool const                          +
+-----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeStructField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

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


.. das:method:: AstVisitor.visitExprMakeStructField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:smart_ptr<ast::MakeFieldDecl> const; last:bool const) 

visitExprMakeStructField returns smart_ptr<ast::MakeFieldDecl>

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


.. das:method:: AstVisitor.preVisitExprMakeArray (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeArray> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeArray (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const) 

visitExprMakeArray returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeArray> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeArrayIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:smart_ptr<ast::Expression> const; last:bool const) 

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


.. das:method:: AstVisitor.visitExprMakeArrayIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:smart_ptr<ast::Expression> const; last:bool const) 

visitExprMakeArrayIndex returns smart_ptr<ast::Expression>

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


.. das:method:: AstVisitor.preVisitExprMakeTuple (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeTuple> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeTuple (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const) 

visitExprMakeTuple returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeTuple> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeTupleIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:smart_ptr<ast::Expression> const; last:bool const) 

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


.. das:method:: AstVisitor.visitExprMakeTupleIndex (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:smart_ptr<ast::Expression> const; last:bool const) 

visitExprMakeTupleIndex returns smart_ptr<ast::Expression>

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


.. das:method:: AstVisitor.preVisitExprArrayComprehension (self:ast::AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const) 

function arguments are

+----+--------------------------------------------+
+expr+smart_ptr<ast::ExprArrayComprehension> const+
+----+--------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprArrayComprehension (self:ast::AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const) 

visitExprArrayComprehension returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------------+
+expr+smart_ptr<ast::ExprArrayComprehension> const+
+----+--------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprArrayComprehensionSubexpr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; subexrp:smart_ptr<ast::Expression> const) 

function arguments are

+-------+--------------------------------------------+
+expr   +smart_ptr<ast::ExprArrayComprehension> const+
+-------+--------------------------------------------+
+subexrp+smart_ptr<ast::Expression> const            +
+-------+--------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprArrayComprehensionWhere (self:ast::AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; filter:smart_ptr<ast::Expression> const) 

function arguments are

+------+--------------------------------------------+
+expr  +smart_ptr<ast::ExprArrayComprehension> const+
+------+--------------------------------------------+
+filter+smart_ptr<ast::Expression> const            +
+------+--------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTypeInfo (self:ast::AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTypeInfo> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprTypeInfo (self:ast::AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const) 

visitExprTypeInfo returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprTypeInfo> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprPtr2Ref (self:ast::AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprPtr2Ref> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprPtr2Ref (self:ast::AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const) 

visitExprPtr2Ref returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprPtr2Ref> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLabel (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLabel> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprLabel> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLabel (self:ast::AstVisitor; expr:smart_ptr<ast::ExprLabel> const) 

visitExprLabel returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprLabel> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprGoto (self:ast::AstVisitor; expr:smart_ptr<ast::ExprGoto> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprGoto> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprGoto (self:ast::AstVisitor; expr:smart_ptr<ast::ExprGoto> const) 

visitExprGoto returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprGoto> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprRef2Value (self:ast::AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprRef2Value> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprRef2Value (self:ast::AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const) 

visitExprRef2Value returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprRef2Value> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprRef2Ptr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprRef2Ptr> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprRef2Ptr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const) 

visitExprRef2Ptr returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprRef2Ptr> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAddr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAddr> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprAddr> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAddr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAddr> const) 

visitExprAddr returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprAddr> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAssert (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAssert> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAssert> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAssert (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAssert> const) 

visitExprAssert returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAssert> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStaticAssert (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const) 

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprStaticAssert> const+
+----+--------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStaticAssert (self:ast::AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const) 

visitExprStaticAssert returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprStaticAssert> const+
+----+--------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprDebug (self:ast::AstVisitor; expr:smart_ptr<ast::ExprDebug> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprDebug> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprDebug (self:ast::AstVisitor; expr:smart_ptr<ast::ExprDebug> const) 

visitExprDebug returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprDebug> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprInvoke (self:ast::AstVisitor; expr:smart_ptr<ast::ExprInvoke> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprInvoke> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprInvoke (self:ast::AstVisitor; expr:smart_ptr<ast::ExprInvoke> const) 

visitExprInvoke returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprInvoke> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprErase (self:ast::AstVisitor; expr:smart_ptr<ast::ExprErase> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprErase> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprErase (self:ast::AstVisitor; expr:smart_ptr<ast::ExprErase> const) 

visitExprErase returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprErase> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFind (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFind> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFind> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFind (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFind> const) 

visitExprFind returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprFind> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprKeyExists (self:ast::AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprKeyExists> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprKeyExists (self:ast::AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const) 

visitExprKeyExists returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprKeyExists> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAscend (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAscend> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAscend> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAscend (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAscend> const) 

visitExprAscend returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprAscend> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCast (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCast> const) 

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCast> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCast (self:ast::AstVisitor; expr:smart_ptr<ast::ExprCast> const) 

visitExprCast returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::ExprCast> const+
+----+------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprDelete (self:ast::AstVisitor; expr:smart_ptr<ast::ExprDelete> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprDelete> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprDelete (self:ast::AstVisitor; expr:smart_ptr<ast::ExprDelete> const) 

visitExprDelete returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprDelete> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprVar (self:ast::AstVisitor; expr:smart_ptr<ast::ExprVar> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprVar> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprVar (self:ast::AstVisitor; expr:smart_ptr<ast::ExprVar> const) 

visitExprVar returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprVar> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprField> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprField> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprField> const) 

visitExprField returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprField> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeField> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprSafeField> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeField (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeField> const) 

visitExprSafeField returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprSafeField> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSwizzle (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprSwizzle> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSwizzle (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const) 

visitExprSwizzle returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprSwizzle> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const) 

visitExprIsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprIsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const) 

visitExprAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprAsVariant> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeAsVariant (self:ast::AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const) 

visitExprSafeAsVariant returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprSafeAsVariant> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp1 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp1> const) 

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp1> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp1 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprOp1> const) 

visitExprOp1 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------+
+expr+smart_ptr<ast::ExprOp1> const+
+----+-----------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprReturn (self:ast::AstVisitor; expr:smart_ptr<ast::ExprReturn> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReturn> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprReturn (self:ast::AstVisitor; expr:smart_ptr<ast::ExprReturn> const) 

visitExprReturn returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReturn> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprYield (self:ast::AstVisitor; expr:smart_ptr<ast::ExprYield> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprYield> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprYield (self:ast::AstVisitor; expr:smart_ptr<ast::ExprYield> const) 

visitExprYield returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprYield> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBreak (self:ast::AstVisitor; expr:smart_ptr<ast::ExprBreak> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprBreak> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBreak (self:ast::AstVisitor; expr:smart_ptr<ast::ExprBreak> const) 

visitExprBreak returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprBreak> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprContinue (self:ast::AstVisitor; expr:smart_ptr<ast::ExprContinue> const) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprContinue> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprContinue (self:ast::AstVisitor; expr:smart_ptr<ast::ExprContinue> const) 

visitExprContinue returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprContinue> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeBlock (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeBlock> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeBlock (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const) 

visitExprMakeBlock returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprMakeBlock> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeGenerator (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprMakeGenerator> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeGenerator (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const) 

visitExprMakeGenerator returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprMakeGenerator> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMemZero (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMemZero> const) 

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprMemZero> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMemZero (self:ast::AstVisitor; expr:smart_ptr<ast::ExprMemZero> const) 

visitExprMemZero returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------+
+expr+smart_ptr<ast::ExprMemZero> const+
+----+---------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConst (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConst> const) 

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprConst> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConst (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConst> const) 

visitExprConst returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------+
+expr+smart_ptr<ast::ExprConst> const+
+----+-------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstPtr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstPtr> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstPtr (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const) 

visitExprConstPtr returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstPtr> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstEnumeration (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const) 

function arguments are

+----+------------------------------------------+
+expr+smart_ptr<ast::ExprConstEnumeration> const+
+----+------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstEnumeration (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const) 

visitExprConstEnumeration returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------------+
+expr+smart_ptr<ast::ExprConstEnumeration> const+
+----+------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstBitfield (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const) 

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprConstBitfield> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstBitfield (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const) 

visitExprConstBitfield returns smart_ptr<ast::Expression>

function arguments are

+----+---------------------------------------+
+expr+smart_ptr<ast::ExprConstBitfield> const+
+----+---------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt8 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt8> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt8 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const) 

visitExprConstInt8 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt8> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt16 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt16> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt16 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const) 

visitExprConstInt16 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt16> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt64 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt64> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt64 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const) 

visitExprConstInt64 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstInt64> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt> const) 

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstInt> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt> const) 

visitExprConstInt returns smart_ptr<ast::Expression>

function arguments are

+----+----------------------------------+
+expr+smart_ptr<ast::ExprConstInt> const+
+----+----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt2> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const) 

visitExprConstInt2 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt2> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt3> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const) 

visitExprConstInt3 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt3> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt4> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const) 

visitExprConstInt4 returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstInt4> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt8 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt8> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt8 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const) 

visitExprConstUInt8 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt8> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt16 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt16> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt16 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const) 

visitExprConstUInt16 returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt16> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt64 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt64> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt64 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const) 

visitExprConstUInt64 returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt64> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstUInt> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const) 

visitExprConstUInt returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstUInt> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt2> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const) 

visitExprConstUInt2 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt2> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt3> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const) 

visitExprConstUInt3 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt3> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt4> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const) 

visitExprConstUInt4 returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstUInt4> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstRange (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstRange> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstRange> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstRange (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstRange> const) 

visitExprConstRange returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstRange> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstURange (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstURange> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstURange> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstURange (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstURange> const) 

visitExprConstURange returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstURange> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstBool (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstBool> const) 

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstBool> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstBool (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstBool> const) 

visitExprConstBool returns smart_ptr<ast::Expression>

function arguments are

+----+-----------------------------------+
+expr+smart_ptr<ast::ExprConstBool> const+
+----+-----------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const) 

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const) 

visitExprConstFloat returns smart_ptr<ast::Expression>

function arguments are

+----+------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat> const+
+----+------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat2> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat2 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const) 

visitExprConstFloat2 returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat2> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat3> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat3 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const) 

visitExprConstFloat3 returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat3> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat4> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat4 (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const) 

visitExprConstFloat4 returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstFloat4> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstString (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstString> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstString> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstString (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstString> const) 

visitExprConstString returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstString> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstDouble (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstDouble> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstDouble (self:ast::AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const) 

visitExprConstDouble returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprConstDouble> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFakeContext (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const) 

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprFakeContext> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFakeContext (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const) 

visitExprFakeContext returns smart_ptr<ast::Expression>

function arguments are

+----+-------------------------------------+
+expr+smart_ptr<ast::ExprFakeContext> const+
+----+-------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFakeLineInfo (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const) 

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprFakeLineInfo> const+
+----+--------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFakeLineInfo (self:ast::AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const) 

visitExprFakeLineInfo returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------------+
+expr+smart_ptr<ast::ExprFakeLineInfo> const+
+----+--------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprReader (self:ast::AstVisitor; expr:smart_ptr<ast::ExprReader> const) 

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprReader (self:ast::AstVisitor; expr:smart_ptr<ast::ExprReader> const) 

visitExprReader returns smart_ptr<ast::Expression>

function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::ExprReader> const+
+----+--------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: ExpressionPtr(expr:smart_ptr<auto(TT)> const)

ExpressionPtr returns smart_ptr<ast::Expression>



function arguments are

+----+-------------------------+
+expr+smart_ptr<auto(TT)> const+
+----+-------------------------+



to be documented


.. das:function:: add_new_function_annotation(name:string const; someClassPtr:auto const)

add_new_function_annotation returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_reader_macro(name:string const; someClassPtr:auto const)

add_new_reader_macro returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_structure_annotation(name:string const; someClassPtr:auto const)

add_new_structure_annotation returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_variant_macro(name:string const; someClassPtr:auto const)

add_new_variant_macro returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: describe(expr:smart_ptr<ast::Expression> const)

describe returns auto



function arguments are

+----+--------------------------------+
+expr+smart_ptr<ast::Expression> const+
+----+--------------------------------+



to be documented


.. das:function:: describe(expr:smart_ptr<ast::Function> const)

describe returns auto



function arguments are

+----+------------------------------+
+expr+smart_ptr<ast::Function> const+
+----+------------------------------+



to be documented


.. das:function:: describe(decl:smart_ptr<ast::TypeDecl> const; extra:bool const; contracts:bool const; modules:bool const)

describe returns auto



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


.. das:function:: make_function_annotation(name:string const; someClassPtr:auto const)

make_function_annotation returns smart_ptr<ast::FunctionAnnotation>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_pass_macro(name:string const; someClassPtr:auto const)

make_pass_macro returns smart_ptr<ast::PassMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_reader_macro(name:string const; someClassPtr:auto const)

make_reader_macro returns smart_ptr<ast::ReaderMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_structure_annotation(name:string const; someClassPtr:auto const)

make_structure_annotation returns smart_ptr<ast::StructureAnnotation>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_variant_macro(name:string const; someClassPtr:auto const)

make_variant_macro returns smart_ptr<ast::VariantMacro>



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_visitor(someClass:auto const)

make_visitor returns smart_ptr<ast::VisitorAdapter>



function arguments are

+---------+----------+
+someClass+auto const+
+---------+----------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: add_dirty_infer_macro(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::PassMacro> const implicit)



function arguments are

+----+----------------------------------------+
+arg0+rtti::Module? const implicit            +
+----+----------------------------------------+
+arg1+smart_ptr<ast::PassMacro> const implicit+
+----+----------------------------------------+
+arg2+ const                                  +
+----+----------------------------------------+



to be documented


.. das:function:: add_function(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::Function> const implicit)



function arguments are

+----+---------------------------------------+
+arg0+rtti::Module? const implicit           +
+----+---------------------------------------+
+arg1+smart_ptr<ast::Function> const implicit+
+----+---------------------------------------+
+arg2+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: add_function_annotation(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit)



function arguments are

+----+-------------------------------------------------+
+arg0+rtti::Module? const implicit                     +
+----+-------------------------------------------------+
+arg1+smart_ptr<ast::FunctionAnnotation> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: add_function_annotation(arg0:smart_ptr<ast::Function> const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit)



function arguments are

+----+-------------------------------------------------+
+arg0+smart_ptr<ast::Function> const implicit          +
+----+-------------------------------------------------+
+arg1+smart_ptr<ast::FunctionAnnotation> const implicit+
+----+-------------------------------------------------+
+arg2+ const                                           +
+----+-------------------------------------------------+



to be documented


.. das:function:: add_reader_macro(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::ReaderMacro> const implicit)



function arguments are

+----+------------------------------------------+
+arg0+rtti::Module? const implicit              +
+----+------------------------------------------+
+arg1+smart_ptr<ast::ReaderMacro> const implicit+
+----+------------------------------------------+
+arg2+ const                                    +
+----+------------------------------------------+



to be documented


.. das:function:: add_structure_annotation(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit)



function arguments are

+----+--------------------------------------------------+
+arg0+rtti::Module? const implicit                      +
+----+--------------------------------------------------+
+arg1+smart_ptr<ast::StructureAnnotation> const implicit+
+----+--------------------------------------------------+
+arg2+ const                                            +
+----+--------------------------------------------------+



to be documented


.. das:function:: add_structure_annotation(arg0:smart_ptr<ast::Structure> const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit)



function arguments are

+----+--------------------------------------------------+
+arg0+smart_ptr<ast::Structure> const implicit          +
+----+--------------------------------------------------+
+arg1+smart_ptr<ast::StructureAnnotation> const implicit+
+----+--------------------------------------------------+
+arg2+ const                                            +
+----+--------------------------------------------------+



to be documented


.. das:function:: add_variant_macro(arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::VariantMacro> const implicit)



function arguments are

+----+-------------------------------------------+
+arg0+rtti::Module? const implicit               +
+----+-------------------------------------------+
+arg1+smart_ptr<ast::VariantMacro> const implicit+
+----+-------------------------------------------+
+arg2+ const                                     +
+----+-------------------------------------------+



to be documented


.. das:function:: any_array_foreach(arg0:void? const implicit; arg1:int const; arg2:block<(void?):void> const implicit)



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


.. das:function:: any_array_size(arg0:void? const implicit)

any_array_size returns int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: any_table_foreach(arg0:void? const implicit; arg1:int const; arg2:int const; arg3:block<(void?;void?):void> const implicit)



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


.. das:function:: any_table_size(arg0:void? const implicit)

any_table_size returns int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: clone_expression(arg0:smart_ptr<ast::Expression> const implicit)

clone_expression returns smart_ptr<ast::Expression>



function arguments are

+----+-----------------------------------------+
+arg0+smart_ptr<ast::Expression> const implicit+
+----+-----------------------------------------+



to be documented


.. das:function:: clone_type(arg0:smart_ptr<ast::TypeDecl> const implicit)

clone_type returns smart_ptr<ast::TypeDecl>



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+



to be documented


.. das:function:: compiling_module

compiling_module returns rtti::Module?




to be documented


.. das:function:: compiling_program

compiling_program returns smart_ptr<rtti::Program>




to be documented


.. das:function:: das_to_string(arg0:rtti::Type const)

das_to_string returns string



function arguments are

+----+----------------+
+arg0+rtti::Type const+
+----+----------------+
+arg1+ const          +
+----+----------------+



to be documented


.. das:function:: describe_expression(arg0:smart_ptr<ast::Expression> const implicit)

describe_expression returns string



function arguments are

+----+-----------------------------------------+
+arg0+smart_ptr<ast::Expression> const implicit+
+----+-----------------------------------------+
+arg1+ const                                   +
+----+-----------------------------------------+



to be documented


.. das:function:: describe_function(arg0:smart_ptr<ast::Function> const implicit)

describe_function returns string



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::Function> const implicit+
+----+---------------------------------------+
+arg1+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: describe_typedecl(arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bool const; arg2:bool const; arg3:bool const)

describe_typedecl returns string



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


.. das:function:: find_bitfield_name(arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bitfield const)

find_bitfield_name returns string



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+bitfield const                         +
+----+---------------------------------------+
+arg2+ const                                 +
+----+---------------------------------------+



to be documented


.. das:function:: for_each_enumeration(arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Enumeration>):void> const implicit)



function arguments are

+----+--------------------------------------------------------+
+arg0+rtti::Module? const implicit                            +
+----+--------------------------------------------------------+
+arg1+block<(smart_ptr<ast::Enumeration>):void> const implicit+
+----+--------------------------------------------------------+
+arg2+ const                                                  +
+----+--------------------------------------------------------+



to be documented


.. das:function:: for_each_field(arg0:rtti::BasicStructureAnnotation const implicit; arg1:block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit)



function arguments are

+----+------------------------------------------------------------------------+
+arg0+rtti::BasicStructureAnnotation const implicit                           +
+----+------------------------------------------------------------------------+
+arg1+block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit+
+----+------------------------------------------------------------------------+
+arg2+ const                                                                  +
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_function(arg0:rtti::Module? const implicit; arg1:string const; arg2:block<(smart_ptr<ast::Function>):void> const implicit)



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


.. das:function:: for_each_generic(arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Function>):void> const implicit)



function arguments are

+----+-----------------------------------------------------+
+arg0+rtti::Module? const implicit                         +
+----+-----------------------------------------------------+
+arg1+block<(smart_ptr<ast::Function>):void> const implicit+
+----+-----------------------------------------------------+
+arg2+ const                                               +
+----+-----------------------------------------------------+



to be documented


.. das:function:: for_each_structure(arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Structure>):void> const implicit)



function arguments are

+----+------------------------------------------------------+
+arg0+rtti::Module? const implicit                          +
+----+------------------------------------------------------+
+arg1+block<(smart_ptr<ast::Structure>):void> const implicit+
+----+------------------------------------------------------+
+arg2+ const                                                +
+----+------------------------------------------------------+



to be documented


.. das:function:: for_each_typedef(arg0:rtti::Module? const implicit; arg1:block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit)



function arguments are

+----+-------------------------------------------------------------+
+arg0+rtti::Module? const implicit                                 +
+----+-------------------------------------------------------------+
+arg1+block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit+
+----+-------------------------------------------------------------+
+arg2+ const                                                       +
+----+-------------------------------------------------------------+



to be documented


.. das:function:: get_tuple_field_offset(arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const)

get_tuple_field_offset returns int



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+int const                              +
+----+---------------------------------------+



to be documented


.. das:function:: get_variant_field_offset(arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const)

get_variant_field_offset returns int



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<ast::TypeDecl> const implicit+
+----+---------------------------------------+
+arg1+int const                              +
+----+---------------------------------------+



to be documented


.. das:function:: is_same_type(arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:smart_ptr<ast::TypeDecl> const implicit; arg2:rtti::RefMatters const; arg3:rtti::ConstMatters const; arg4:rtti::TemporaryMatters const)

is_same_type returns bool



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


.. das:function:: macro_error(arg0:smart_ptr<rtti::Program> const implicit; arg1:rtti::LineInfo const implicit; arg2:string const)



function arguments are

+----+---------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit+
+----+---------------------------------------+
+arg1+rtti::LineInfo const implicit          +
+----+---------------------------------------+
+arg2+string const                           +
+----+---------------------------------------+



to be documented


.. das:function:: make_function_annotation(arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)

make_function_annotation returns smart_ptr<ast::FunctionAnnotation>



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


.. das:function:: make_pass_macro(arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)

make_pass_macro returns smart_ptr<ast::PassMacro>



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


.. das:function:: make_reader_macro(arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)

make_reader_macro returns smart_ptr<ast::ReaderMacro>



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


.. das:function:: make_structure_annotation(arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)

make_structure_annotation returns smart_ptr<ast::StructureAnnotation>



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


.. das:function:: make_variant_macro(arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)

make_variant_macro returns smart_ptr<ast::VariantMacro>



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


.. das:function:: make_visitor(arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit)

make_visitor returns smart_ptr<ast::VisitorAdapter>



function arguments are

+----+--------------------------------------+
+arg0+void? const implicit                  +
+----+--------------------------------------+
+arg1+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg2+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: this_module()

this_module returns rtti::Module?



function arguments are

+----+------+
+arg0+ const+
+----+------+
+arg1+ const+
+----+------+



to be documented


.. das:function:: this_program()

this_program returns smart_ptr<rtti::Program>



function arguments are

+----+------+
+arg0+ const+
+----+------+



to be documented


.. das:function:: visit(arg0:smart_ptr<rtti::Program> const implicit; arg1:smart_ptr<ast::VisitorAdapter> const implicit)



function arguments are

+----+---------------------------------------------+
+arg0+smart_ptr<rtti::Program> const implicit      +
+----+---------------------------------------------+
+arg1+smart_ptr<ast::VisitorAdapter> const implicit+
+----+---------------------------------------------+



to be documented



