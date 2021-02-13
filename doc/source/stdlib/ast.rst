
.. _stdlib_ast:

========================
AST manipulation library
========================

.. include:: detail/ast.rst

The AST module implements compilation time reflection for the daScript syntax tree.

All functions and symbols are in "ast" module, use require to get access to it. ::

    require ast


++++++++++++
Type aliases
++++++++++++

.. _alias-CallMacroPtr:

.. das:attribute:: CallMacroPtr = smart_ptr<ast::CallMacro>

|typedef-ast-CallMacroPtr|

.. _alias-EnumerationPtr:

.. das:attribute:: EnumerationPtr = smart_ptr<ast::Enumeration>

|typedef-ast-EnumerationPtr|

.. _alias-ExprAscendFlags:

.. das:attribute:: ExprAscendFlags is a bitfield

+------------+---+-----+
+field       +bit+value+
+============+===+=====+
+useStackRef +0  +1    +
+------------+---+-----+
+needTypeInfo+1  +2    +
+------------+---+-----+


|typedef-ast-ExprAscendFlags|

.. _alias-ExprAtFlags:

.. das:attribute:: ExprAtFlags is a bitfield

+-----+---+-----+
+field+bit+value+
+=====+===+=====+
+r2v  +0  +1    +
+-----+---+-----+
+r2cr +1  +2    +
+-----+---+-----+
+write+2  +4    +
+-----+---+-----+


|typedef-ast-ExprAtFlags|

.. _alias-ExprBlockFlags:

.. das:attribute:: ExprBlockFlags is a bitfield

+--------------------------+---+-----+
+field                     +bit+value+
+==========================+===+=====+
+isClosure                 +0  +1    +
+--------------------------+---+-----+
+hasReturn                 +1  +2    +
+--------------------------+---+-----+
+copyOnReturn              +2  +4    +
+--------------------------+---+-----+
+moveOnReturn              +3  +8    +
+--------------------------+---+-----+
+inTheLoop                 +4  +16   +
+--------------------------+---+-----+
+finallyBeforeBody         +5  +32   +
+--------------------------+---+-----+
+finallyDisabled           +6  +64   +
+--------------------------+---+-----+
+aotSkipMakeBlock          +7  +128  +
+--------------------------+---+-----+
+aotDoNotSkipAnnotationData+8  +256  +
+--------------------------+---+-----+
+isCollapseable            +9  +512  +
+--------------------------+---+-----+
+needCollapse              +10 +1024 +
+--------------------------+---+-----+


|typedef-ast-ExprBlockFlags|

.. _alias-ExprCastFlags:

.. das:attribute:: ExprCastFlags is a bitfield

+---------------+---+-----+
+field          +bit+value+
+===============+===+=====+
+upcastCast     +0  +1    +
+---------------+---+-----+
+reinterpretCast+1  +2    +
+---------------+---+-----+


|typedef-ast-ExprCastFlags|

.. _alias-ExprFieldDerefFlags:

.. das:attribute:: ExprFieldDerefFlags is a bitfield

+------------------+---+-----+
+field             +bit+value+
+==================+===+=====+
+unsafeDeref       +0  +1    +
+------------------+---+-----+
+ignoreCaptureConst+1  +2    +
+------------------+---+-----+


|typedef-ast-ExprFieldDerefFlags|

.. _alias-ExprFieldFieldFlags:

.. das:attribute:: ExprFieldFieldFlags is a bitfield

+-----+---+-----+
+field+bit+value+
+=====+===+=====+
+r2v  +0  +1    +
+-----+---+-----+
+r2cr +1  +2    +
+-----+---+-----+
+write+2  +4    +
+-----+---+-----+


|typedef-ast-ExprFieldFieldFlags|

.. _alias-ExprFlags:

.. das:attribute:: ExprFlags is a bitfield

+-------------------+---+-----+
+field              +bit+value+
+===================+===+=====+
+constexpression    +0  +1    +
+-------------------+---+-----+
+noSideEffects      +1  +2    +
+-------------------+---+-----+
+noNativeSideEffects+2  +4    +
+-------------------+---+-----+


|typedef-ast-ExprFlags|

.. _alias-ExprGenFlags:

.. das:attribute:: ExprGenFlags is a bitfield

+----------+---+-----+
+field     +bit+value+
+==========+===+=====+
+alwaysSafe+0  +1    +
+----------+---+-----+
+generated +1  +2    +
+----------+---+-----+


|typedef-ast-ExprGenFlags|

.. _alias-ExprMakeBlockFlags:

.. das:attribute:: ExprMakeBlockFlags is a bitfield

+---------------+---+-----+
+field          +bit+value+
+===============+===+=====+
+isLambda       +0  +1    +
+---------------+---+-----+
+isLocalFunction+1  +2    +
+---------------+---+-----+


|typedef-ast-ExprMakeBlockFlags|

.. _alias-ExprMakeLocalFlags:

.. das:attribute:: ExprMakeLocalFlags is a bitfield

+---------------+---+-----+
+field          +bit+value+
+===============+===+=====+
+useStackRef    +0  +1    +
+---------------+---+-----+
+useCMRES       +1  +2    +
+---------------+---+-----+
+doesNotNeedSp  +2  +4    +
+---------------+---+-----+
+doesNotNeedInit+3  +8    +
+---------------+---+-----+
+initAllFields  +4  +16   +
+---------------+---+-----+


|typedef-ast-ExprMakeLocalFlags|

.. _alias-ExprMakeStructFlags:

.. das:attribute:: ExprMakeStructFlags is a bitfield

+--------------+---+-----+
+field         +bit+value+
+==============+===+=====+
+useInitializer+0  +1    +
+--------------+---+-----+
+isNewHandle   +1  +2    +
+--------------+---+-----+


|typedef-ast-ExprMakeStructFlags|

.. _alias-ExprPrintFlags:

.. das:attribute:: ExprPrintFlags is a bitfield

+-----------+---+-----+
+field      +bit+value+
+===========+===+=====+
+topLevel   +0  +1    +
+-----------+---+-----+
+argLevel   +1  +2    +
+-----------+---+-----+
+bottomLevel+2  +4    +
+-----------+---+-----+


|typedef-ast-ExprPrintFlags|

.. _alias-ExprReturnFlags:

.. das:attribute:: ExprReturnFlags is a bitfield

+------------------+---+-----+
+field             +bit+value+
+==================+===+=====+
+moveSemantics     +0  +1    +
+------------------+---+-----+
+returnReference   +1  +2    +
+------------------+---+-----+
+returnInBlock     +2  +4    +
+------------------+---+-----+
+takeOverRightStack+3  +8    +
+------------------+---+-----+
+returnCallCMRES   +4  +16   +
+------------------+---+-----+
+returnCMRES       +5  +32   +
+------------------+---+-----+
+fromYield         +6  +64   +
+------------------+---+-----+


|typedef-ast-ExprReturnFlags|

.. _alias-ExprSwizzleFieldFlags:

.. das:attribute:: ExprSwizzleFieldFlags is a bitfield

+-----+---+-----+
+field+bit+value+
+=====+===+=====+
+r2v  +0  +1    +
+-----+---+-----+
+r2cr +1  +2    +
+-----+---+-----+
+write+2  +4    +
+-----+---+-----+


|typedef-ast-ExprSwizzleFieldFlags|

.. _alias-ExprVarFlags:

.. das:attribute:: ExprVarFlags is a bitfield

+---------+---+-----+
+field    +bit+value+
+=========+===+=====+
+local    +0  +1    +
+---------+---+-----+
+argument +1  +2    +
+---------+---+-----+
+block    +2  +4    +
+---------+---+-----+
+thisBlock+3  +8    +
+---------+---+-----+
+r2v      +4  +16   +
+---------+---+-----+
+r2cr     +5  +32   +
+---------+---+-----+
+write    +6  +64   +
+---------+---+-----+


|typedef-ast-ExprVarFlags|

.. _alias-ExprYieldFlags:

.. das:attribute:: ExprYieldFlags is a bitfield

+-------------+---+-----+
+field        +bit+value+
+=============+===+=====+
+moveSemantics+0  +1    +
+-------------+---+-----+


|typedef-ast-ExprYieldFlags|

.. _alias-ExpressionPtr:

.. das:attribute:: ExpressionPtr = smart_ptr<ast::Expression>

|typedef-ast-ExpressionPtr|

.. _alias-FieldDeclarationFlags:

.. das:attribute:: FieldDeclarationFlags is a bitfield

+----------------+---+-----+
+field           +bit+value+
+================+===+=====+
+moveSemantics   +0  +1    +
+----------------+---+-----+
+parentType      +1  +2    +
+----------------+---+-----+
+capturedConstant+2  +4    +
+----------------+---+-----+
+generated       +3  +8    +
+----------------+---+-----+
+capturedRef     +4  +16   +
+----------------+---+-----+
+doNotDelete     +5  +32   +
+----------------+---+-----+


|typedef-ast-FieldDeclarationFlags|

.. _alias-FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = smart_ptr<ast::FunctionAnnotation>

|typedef-ast-FunctionAnnotationPtr|

.. _alias-FunctionFlags:

.. das:attribute:: FunctionFlags is a bitfield

+---------------------+---+---------+
+field                +bit+value    +
+=====================+===+=========+
+builtIn              +0  +1        +
+---------------------+---+---------+
+policyBased          +1  +2        +
+---------------------+---+---------+
+callBased            +2  +4        +
+---------------------+---+---------+
+interopFn            +3  +8        +
+---------------------+---+---------+
+hasReturn            +4  +16       +
+---------------------+---+---------+
+copyOnReturn         +5  +32       +
+---------------------+---+---------+
+moveOnReturn         +6  +64       +
+---------------------+---+---------+
+exports              +7  +128      +
+---------------------+---+---------+
+init                 +8  +256      +
+---------------------+---+---------+
+addr                 +9  +512      +
+---------------------+---+---------+
+used                 +10 +1024     +
+---------------------+---+---------+
+fastCall             +11 +2048     +
+---------------------+---+---------+
+knownSideEffects     +12 +4096     +
+---------------------+---+---------+
+hasToRunAtCompileTime+13 +8192     +
+---------------------+---+---------+
+unsafeOperation      +14 +16384    +
+---------------------+---+---------+
+unsafeDeref          +15 +32768    +
+---------------------+---+---------+
+hasMakeBlock         +16 +65536    +
+---------------------+---+---------+
+aotNeedPrologue      +17 +131072   +
+---------------------+---+---------+
+noAot                +18 +262144   +
+---------------------+---+---------+
+aotHybrid            +19 +524288   +
+---------------------+---+---------+
+aotTemplate          +20 +1048576  +
+---------------------+---+---------+
+generated            +21 +2097152  +
+---------------------+---+---------+
+privateFunction      +22 +4194304  +
+---------------------+---+---------+
+_generator           +23 +8388608  +
+---------------------+---+---------+
+_lambda              +24 +16777216 +
+---------------------+---+---------+
+firstArgReturnType   +25 +33554432 +
+---------------------+---+---------+
+isClassMethod        +26 +67108864 +
+---------------------+---+---------+
+safe                 +27 +134217728+
+---------------------+---+---------+


|typedef-ast-FunctionFlags|

.. _alias-FunctionPtr:

.. das:attribute:: FunctionPtr = smart_ptr<ast::Function>

|typedef-ast-FunctionPtr|

.. _alias-FunctionSideEffectFlags:

.. das:attribute:: FunctionSideEffectFlags is a bitfield

+--------------+---+-----+
+field         +bit+value+
+==============+===+=====+
+unsafe        +0  +1    +
+--------------+---+-----+
+userScenario  +1  +2    +
+--------------+---+-----+
+modifyExternal+2  +4    +
+--------------+---+-----+
+modifyArgument+3  +8    +
+--------------+---+-----+
+accessGlobal  +4  +16   +
+--------------+---+-----+
+invoke        +5  +32   +
+--------------+---+-----+


|typedef-ast-FunctionSideEffectFlags|

.. _alias-MakeFieldDeclFlags:

.. das:attribute:: MakeFieldDeclFlags is a bitfield

+--------------+---+-----+
+field         +bit+value+
+==============+===+=====+
+moveSemantics +0  +1    +
+--------------+---+-----+
+cloneSemantics+1  +2    +
+--------------+---+-----+


|typedef-ast-MakeFieldDeclFlags|

.. _alias-MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = smart_ptr<ast::MakeFieldDecl>

|typedef-ast-MakeFieldDeclPtr|

.. _alias-PassMacroPtr:

.. das:attribute:: PassMacroPtr = smart_ptr<ast::PassMacro>

|typedef-ast-PassMacroPtr|

.. _alias-ProgramPtr:

.. das:attribute:: ProgramPtr = smart_ptr<rtti::Program>

|typedef-ast-ProgramPtr|

.. _alias-ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = smart_ptr<ast::ReaderMacro>

|typedef-ast-ReaderMacroPtr|

.. _alias-StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = smart_ptr<ast::StructureAnnotation>

|typedef-ast-StructureAnnotationPtr|

.. _alias-StructureFlags:

.. das:attribute:: StructureFlags is a bitfield

+---------------+---+-----+
+field          +bit+value+
+===============+===+=====+
+isClass        +0  +1    +
+---------------+---+-----+
+genCtor        +1  +2    +
+---------------+---+-----+
+cppLayout      +2  +4    +
+---------------+---+-----+
+cppLayoutNotPod+3  +8    +
+---------------+---+-----+
+generated      +4  +16   +
+---------------+---+-----+
+persistent     +5  +32   +
+---------------+---+-----+
+isLambda       +6  +64   +
+---------------+---+-----+


|typedef-ast-StructureFlags|

.. _alias-StructurePtr:

.. das:attribute:: StructurePtr = smart_ptr<ast::Structure>

|typedef-ast-StructurePtr|

.. _alias-TypeDeclFlags:

.. das:attribute:: TypeDeclFlags is a bitfield

+---------------+---+-----+
+field          +bit+value+
+===============+===+=====+
+ref            +0  +1    +
+---------------+---+-----+
+constant       +1  +2    +
+---------------+---+-----+
+temporary      +2  +4    +
+---------------+---+-----+
+_implicit      +3  +8    +
+---------------+---+-----+
+removeRef      +4  +16   +
+---------------+---+-----+
+removeConstant +5  +32   +
+---------------+---+-----+
+removeDim      +6  +64   +
+---------------+---+-----+
+removeTemporary+7  +128  +
+---------------+---+-----+
+explicitConst  +8  +256  +
+---------------+---+-----+
+aotAlias       +9  +512  +
+---------------+---+-----+
+smartPtr       +10 +1024 +
+---------------+---+-----+
+smartPtrNative +11 +2048 +
+---------------+---+-----+
+isExplicit     +12 +4096 +
+---------------+---+-----+


|typedef-ast-TypeDeclFlags|

.. _alias-TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = smart_ptr<ast::TypeDecl>

|typedef-ast-TypeDeclPtr|

.. _alias-VariableAccessFlags:

.. das:attribute:: VariableAccessFlags is a bitfield

+-------------+---+-----+
+field        +bit+value+
+=============+===+=====+
+access_extern+0  +1    +
+-------------+---+-----+
+access_get   +1  +2    +
+-------------+---+-----+
+access_ref   +2  +4    +
+-------------+---+-----+
+access_init  +3  +8    +
+-------------+---+-----+
+access_pass  +4  +16   +
+-------------+---+-----+


|typedef-ast-VariableAccessFlags|

.. _alias-VariableFlags:

.. das:attribute:: VariableFlags is a bitfield

+--------------+---+-----+
+field         +bit+value+
+==============+===+=====+
+init_via_move +0  +1    +
+--------------+---+-----+
+init_via_clone+1  +2    +
+--------------+---+-----+
+used          +2  +4    +
+--------------+---+-----+
+aliasCMRES    +3  +8    +
+--------------+---+-----+
+marked_used   +4  +16   +
+--------------+---+-----+
+global_shared +5  +32   +
+--------------+---+-----+
+do_not_delete +6  +64   +
+--------------+---+-----+
+generated     +7  +128  +
+--------------+---+-----+
+capture_as_ref+8  +256  +
+--------------+---+-----+
+can_shadow    +9  +512  +
+--------------+---+-----+


|typedef-ast-VariableFlags|

.. _alias-VariablePtr:

.. das:attribute:: VariablePtr = smart_ptr<ast::Variable>

|typedef-ast-VariablePtr|

.. _alias-VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = smart_ptr<ast::VariantMacro>

|typedef-ast-VariantMacroPtr|

++++++++++++
Enumerations
++++++++++++

.. _enum-ast-CaptureMode:

.. das:attribute:: CaptureMode

+--------------------+-+
+capture_any         +0+
+--------------------+-+
+capture_by_copy     +1+
+--------------------+-+
+capture_by_reference+2+
+--------------------+-+
+capture_by_clone    +3+
+--------------------+-+
+capture_by_move     +4+
+--------------------+-+


|enumeration-ast-CaptureMode|

.. _enum-ast-SideEffects:

.. das:attribute:: SideEffects

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
+inferredSideEffects      +56+
+-------------------------+--+


|enumeration-ast-SideEffects|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-ast-CallMacro:

.. das:attribute:: CallMacro

CallMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


|structure_annotation-ast-CallMacro|

.. _handle-ast-CaptureEntry:

.. das:attribute:: CaptureEntry

CaptureEntry fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+
+mode+ :ref:`ast::CaptureMode <enum-ast-CaptureMode>`         +
+----+--------------------------------------------------------+


|structure_annotation-ast-CaptureEntry|

.. _handle-ast-EnumEntry:

.. das:attribute:: EnumEntry

EnumEntry fields are

+-----+-----------------------------------------------------------+
+value+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----+-----------------------------------------------------------+
+at   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----+-----------------------------------------------------------+
+name + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+-----+-----------------------------------------------------------+


|structure_annotation-ast-EnumEntry|

.. _handle-ast-Enumeration:

.. das:attribute:: Enumeration

Enumeration fields are

+--------+------------------------------------------------------------------+
+_module + :ref:`rtti::Module <handle-rtti-Module>` ?                       +
+--------+------------------------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                     +
+--------+------------------------------------------------------------------+
+cppName + :ref:`builtin::das_string <handle-builtin-das_string>`           +
+--------+------------------------------------------------------------------+
+list    + :ref:`ast::dasvector`EnumEntry <handle-ast-dasvector`EnumEntry>` +
+--------+------------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`           +
+--------+------------------------------------------------------------------+
+external+bool                                                              +
+--------+------------------------------------------------------------------+
+baseType+ :ref:`rtti::Type <enum-rtti-Type>`                               +
+--------+------------------------------------------------------------------+


|structure_annotation-ast-Enumeration|

.. _handle-ast-ExprAddr:

.. das:attribute:: ExprAddr

ExprAddr fields are

+----------+--------------------------------------------------------+
+func      + :ref:`ast::Function <handle-ast-Function>` ?           +
+----------+--------------------------------------------------------+
+target    + :ref:`builtin::das_string <handle-builtin-das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`           +
+----------+--------------------------------------------------------+
+funcType  +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                     +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+


|structure_annotation-ast-ExprAddr|

.. _handle-ast-ExprArrayComprehension:

.. das:attribute:: ExprArrayComprehension

ExprArrayComprehension fields are

+---------------+-----------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+---------------+-----------------------------------------------------------+
+printFlags     + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+---------------+-----------------------------------------------------------+
+generatorSyntax+bool                                                       +
+---------------+-----------------------------------------------------------+
+subexpr        +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+---------------+-----------------------------------------------------------+
+genFlags       + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+---------------+-----------------------------------------------------------+
+exprFor        +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+---------------+-----------------------------------------------------------+
+exprWhere      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+---------------+-----------------------------------------------------------+
+_type          +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+---------------+-----------------------------------------------------------+
+flags          + :ref:`ExprFlags <alias-ExprFlags>`                        +
+---------------+-----------------------------------------------------------+
+__rtti         +string const                                               +
+---------------+-----------------------------------------------------------+


|structure_annotation-ast-ExprArrayComprehension|

.. _handle-ast-ExprAsVariant:

.. das:attribute:: ExprAsVariant

ExprAsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


|structure_annotation-ast-ExprAsVariant|

.. _handle-ast-ExprAscend:

.. das:attribute:: ExprAscend

ExprAscend fields are

+-----------+-----------------------------------------------------------+
+ascType    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+stackTop   +uint                                                       +
+-----------+-----------------------------------------------------------+
+ascendFlags+ :ref:`ExprAscendFlags <alias-ExprAscendFlags>`            +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprAscend|

.. _handle-ast-ExprAssert:

.. das:attribute:: ExprAssert

ExprAssert fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+isVerify              +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprAssert|

.. _handle-ast-ExprAt:

.. das:attribute:: ExprAt

ExprAt fields are

+----------+-----------------------------------------------------------+
+index     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+atFlags   + :ref:`ExprAtFlags <alias-ExprAtFlags>`                    +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprAt|

.. _handle-ast-ExprBlock:

.. das:attribute:: ExprBlock

ExprBlock fields are

+-----------------+----------------------------------------------------------------------------------------+
+stackVarBottom   +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+annotationDataSid+uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+arguments        + :ref:`ast::dasvector`smart_ptr`Variable <handle-ast-dasvector`smart_ptr`Variable>`     +
+-----------------+----------------------------------------------------------------------------------------+
+at               + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+stackCleanVars   + :ref:`ast::dasvector`pair`uint`uint <handle-ast-dasvector`pair`uint`uint>`             +
+-----------------+----------------------------------------------------------------------------------------+
+list             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+returnType       +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+printFlags       + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+annotations      + :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`                               +
+-----------------+----------------------------------------------------------------------------------------+
+stackTop         +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+maxLabelIndex    +int                                                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+blockFlags       + :ref:`ExprBlockFlags <alias-ExprBlockFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+finalList        + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+genFlags         + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+-----------------+----------------------------------------------------------------------------------------+
+annotationData   +uint64                                                                                  +
+-----------------+----------------------------------------------------------------------------------------+
+stackVarTop      +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+flags            + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+_type            +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+__rtti           +string const                                                                            +
+-----------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprBlock|

.. _handle-ast-ExprBreak:

.. das:attribute:: ExprBreak

ExprBreak fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprBreak|

.. _handle-ast-ExprCall:

.. das:attribute:: ExprCall

ExprCall fields are

+----------------------+----------------------------------------------------------------------------------------+
+func                  + :ref:`ast::Function <handle-ast-Function>` ?                                           +
+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+doesNotNeedSp         +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprCall|

.. _handle-ast-ExprCallFunc:

.. das:attribute:: ExprCallFunc

ExprCallFunc fields are

+----------------------+----------------------------------------------------------------------------------------+
+func                  + :ref:`ast::Function <handle-ast-Function>` ?                                           +
+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprCallFunc|

.. _handle-ast-ExprCallMacro:

.. das:attribute:: ExprCallMacro

ExprCallMacro fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+macro                 + :ref:`ast::CallMacro <handle-ast-CallMacro>` ?                                         +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprCallMacro|

.. _handle-ast-ExprCast:

.. das:attribute:: ExprCast

ExprCast fields are

+----------+-----------------------------------------------------------+
+castFlags + :ref:`ExprCastFlags <alias-ExprCastFlags>`                +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+castType  +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprCast|

.. _handle-ast-ExprClone:

.. das:attribute:: ExprClone

ExprClone fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprClone|

.. _handle-ast-ExprConst:

.. das:attribute:: ExprConst

ExprConst fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConst|

.. _handle-ast-ExprConstBitfield:

.. das:attribute:: ExprConstBitfield

ExprConstBitfield fields are

+------------+-------------------------------------------------------+
+value       +bitfield<>                                             +
+------------+-------------------------------------------------------+
+at          + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+------------+-------------------------------------------------------+
+bitfieldType+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+------------+-------------------------------------------------------+
+printFlags  + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+------------+-------------------------------------------------------+
+genFlags    + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+------------+-------------------------------------------------------+
+baseType    + :ref:`rtti::Type <enum-rtti-Type>`                    +
+------------+-------------------------------------------------------+
+_type       +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+------------+-------------------------------------------------------+
+flags       + :ref:`ExprFlags <alias-ExprFlags>`                    +
+------------+-------------------------------------------------------+
+__rtti      +string const                                           +
+------------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstBitfield|

.. _handle-ast-ExprConstBool:

.. das:attribute:: ExprConstBool

ExprConstBool fields are

+----------+-------------------------------------------------------+
+value     +bool                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstBool|

.. _handle-ast-ExprConstDouble:

.. das:attribute:: ExprConstDouble

ExprConstDouble fields are

+----------+-------------------------------------------------------+
+value     +double                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstDouble|

.. _handle-ast-ExprConstEnumeration:

.. das:attribute:: ExprConstEnumeration

ExprConstEnumeration fields are

+----------+-------------------------------------------------------------+
+value     + :ref:`builtin::das_string <handle-builtin-das_string>`      +
+----------+-------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                +
+----------+-------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                +
+----------+-------------------------------------------------------------+
+enumType  +smart_ptr< :ref:`ast::Enumeration <handle-ast-Enumeration>` >+
+----------+-------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                    +
+----------+-------------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                          +
+----------+-------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >      +
+----------+-------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                          +
+----------+-------------------------------------------------------------+
+__rtti    +string const                                                 +
+----------+-------------------------------------------------------------+


|structure_annotation-ast-ExprConstEnumeration|

.. _handle-ast-ExprConstFloat:

.. das:attribute:: ExprConstFloat

ExprConstFloat fields are

+----------+-------------------------------------------------------+
+value     +float                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstFloat|

.. _handle-ast-ExprConstFloat2:

.. das:attribute:: ExprConstFloat2

ExprConstFloat2 fields are

+----------+-------------------------------------------------------+
+value     +float2                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstFloat2|

.. _handle-ast-ExprConstFloat3:

.. das:attribute:: ExprConstFloat3

ExprConstFloat3 fields are

+----------+-------------------------------------------------------+
+value     +float3                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstFloat3|

.. _handle-ast-ExprConstFloat4:

.. das:attribute:: ExprConstFloat4

ExprConstFloat4 fields are

+----------+-------------------------------------------------------+
+value     +float4                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstFloat4|

.. _handle-ast-ExprConstInt:

.. das:attribute:: ExprConstInt

ExprConstInt fields are

+----------+-------------------------------------------------------+
+value     +int                                                    +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt|

.. _handle-ast-ExprConstInt16:

.. das:attribute:: ExprConstInt16

ExprConstInt16 fields are

+----------+-------------------------------------------------------+
+value     +int16                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt16|

.. _handle-ast-ExprConstInt2:

.. das:attribute:: ExprConstInt2

ExprConstInt2 fields are

+----------+-------------------------------------------------------+
+value     +int2                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt2|

.. _handle-ast-ExprConstInt3:

.. das:attribute:: ExprConstInt3

ExprConstInt3 fields are

+----------+-------------------------------------------------------+
+value     +int3                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt3|

.. _handle-ast-ExprConstInt4:

.. das:attribute:: ExprConstInt4

ExprConstInt4 fields are

+----------+-------------------------------------------------------+
+value     +int4                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt4|

.. _handle-ast-ExprConstInt64:

.. das:attribute:: ExprConstInt64

ExprConstInt64 fields are

+----------+-------------------------------------------------------+
+value     +int64                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt64|

.. _handle-ast-ExprConstInt8:

.. das:attribute:: ExprConstInt8

ExprConstInt8 fields are

+----------+-------------------------------------------------------+
+value     +int8                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstInt8|

.. _handle-ast-ExprConstPtr:

.. das:attribute:: ExprConstPtr

ExprConstPtr fields are

+----------+-------------------------------------------------------+
+value     +void?                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstPtr|

.. _handle-ast-ExprConstRange:

.. das:attribute:: ExprConstRange

ExprConstRange fields are

+----------+-------------------------------------------------------+
+value     +range                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstRange|

.. _handle-ast-ExprConstString:

.. das:attribute:: ExprConstString

ExprConstString fields are

+----------+--------------------------------------------------------+
+value     + :ref:`builtin::das_string <handle-builtin-das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`           +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                     +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                     +
+----------+--------------------------------------------------------+


|structure_annotation-ast-ExprConstString|

.. _handle-ast-ExprConstUInt:

.. das:attribute:: ExprConstUInt

ExprConstUInt fields are

+----------+-------------------------------------------------------+
+value     +uint                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt|

.. _handle-ast-ExprConstUInt16:

.. das:attribute:: ExprConstUInt16

ExprConstUInt16 fields are

+----------+-------------------------------------------------------+
+value     +uint16                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt16|

.. _handle-ast-ExprConstUInt2:

.. das:attribute:: ExprConstUInt2

ExprConstUInt2 fields are

+----------+-------------------------------------------------------+
+value     +uint2                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt2|

.. _handle-ast-ExprConstUInt3:

.. das:attribute:: ExprConstUInt3

ExprConstUInt3 fields are

+----------+-------------------------------------------------------+
+value     +uint3                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt3|

.. _handle-ast-ExprConstUInt4:

.. das:attribute:: ExprConstUInt4

ExprConstUInt4 fields are

+----------+-------------------------------------------------------+
+value     +uint4                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt4|

.. _handle-ast-ExprConstUInt64:

.. das:attribute:: ExprConstUInt64

ExprConstUInt64 fields are

+----------+-------------------------------------------------------+
+value     +uint64                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt64|

.. _handle-ast-ExprConstUInt8:

.. das:attribute:: ExprConstUInt8

ExprConstUInt8 fields are

+----------+-------------------------------------------------------+
+value     +uint8                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstUInt8|

.. _handle-ast-ExprConstURange:

.. das:attribute:: ExprConstURange

ExprConstURange fields are

+----------+-------------------------------------------------------+
+value     +urange                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprConstURange|

.. _handle-ast-ExprContinue:

.. das:attribute:: ExprContinue

ExprContinue fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprContinue|

.. _handle-ast-ExprCopy:

.. das:attribute:: ExprCopy

ExprCopy fields are

+------------------+-----------------------------------------------------------+
+takeOverRightStack+bool                                                       +
+------------------+-----------------------------------------------------------+
+right             +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+------------------+-----------------------------------------------------------+
+at                + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+------------------+-----------------------------------------------------------+
+op                + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+------------------+-----------------------------------------------------------+
+printFlags        + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+------------------+-----------------------------------------------------------+
+genFlags          + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+------------------+-----------------------------------------------------------+
+_type             +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+------------------+-----------------------------------------------------------+
+flags             + :ref:`ExprFlags <alias-ExprFlags>`                        +
+------------------+-----------------------------------------------------------+
+__rtti            +string const                                               +
+------------------+-----------------------------------------------------------+
+left              +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+------------------+-----------------------------------------------------------+


|structure_annotation-ast-ExprCopy|

.. _handle-ast-ExprDebug:

.. das:attribute:: ExprDebug

ExprDebug fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprDebug|

.. _handle-ast-ExprDelete:

.. das:attribute:: ExprDelete

ExprDelete fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+native    +bool                                                       +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprDelete|

.. _handle-ast-ExprErase:

.. das:attribute:: ExprErase

ExprErase fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprErase|

.. _handle-ast-ExprFakeContext:

.. das:attribute:: ExprFakeContext

ExprFakeContext fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprFakeContext|

.. _handle-ast-ExprFakeLineInfo:

.. das:attribute:: ExprFakeLineInfo

ExprFakeLineInfo fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-ExprFakeLineInfo|

.. _handle-ast-ExprField:

.. das:attribute:: ExprField

ExprField fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


|structure_annotation-ast-ExprField|

.. _handle-ast-ExprFind:

.. das:attribute:: ExprFind

ExprFind fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprFind|

.. _handle-ast-ExprFor:

.. das:attribute:: ExprFor

ExprFor fields are

+-----------------+----------------------------------------------------------------------------------------+
+at               + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+body             +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                             +
+-----------------+----------------------------------------------------------------------------------------+
+iteratorsAt      + :ref:`ast::dasvector`LineInfo <handle-ast-dasvector`LineInfo>`                         +
+-----------------+----------------------------------------------------------------------------------------+
+printFlags       + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+iterators        + :ref:`ast::dasvector`das_string <handle-ast-dasvector`das_string>`                     +
+-----------------+----------------------------------------------------------------------------------------+
+iteratorVariables+ :ref:`ast::dasvector`smart_ptr`Variable <handle-ast-dasvector`smart_ptr`Variable>`     +
+-----------------+----------------------------------------------------------------------------------------+
+genFlags         + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+-----------------+----------------------------------------------------------------------------------------+
+sources          + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+_type            +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+flags            + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+__rtti           +string const                                                                            +
+-----------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprFor|

.. _handle-ast-ExprGoto:

.. das:attribute:: ExprGoto

ExprGoto fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+labelName +int                                                        +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprGoto|

.. _handle-ast-ExprIfThenElse:

.. das:attribute:: ExprIfThenElse

ExprIfThenElse fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+if_false  +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+isStatic  +bool                                                       +
+----------+-----------------------------------------------------------+
+cond      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+if_true   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprIfThenElse|

.. _handle-ast-ExprInvoke:

.. das:attribute:: ExprInvoke

ExprInvoke fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+doesNotNeedSp         +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprInvoke|

.. _handle-ast-ExprIs:

.. das:attribute:: ExprIs

ExprIs fields are

+----------+-----------------------------------------------------------+
+typeexpr  +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprIs|

.. _handle-ast-ExprIsVariant:

.. das:attribute:: ExprIsVariant

ExprIsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


|structure_annotation-ast-ExprIsVariant|

.. _handle-ast-ExprKeyExists:

.. das:attribute:: ExprKeyExists

ExprKeyExists fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprKeyExists|

.. _handle-ast-ExprLabel:

.. das:attribute:: ExprLabel

ExprLabel fields are

+----------+--------------------------------------------------------+
+comment   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`           +
+----------+--------------------------------------------------------+
+labelName +int                                                     +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                     +
+----------+--------------------------------------------------------+


|structure_annotation-ast-ExprLabel|

.. _handle-ast-ExprLet:

.. das:attribute:: ExprLet

ExprLet fields are

+----------+------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                       +
+----------+------------------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                       +
+----------+------------------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                           +
+----------+------------------------------------------------------------------------------------+
+variables + :ref:`ast::dasvector`smart_ptr`Variable <handle-ast-dasvector`smart_ptr`Variable>` +
+----------+------------------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                             +
+----------+------------------------------------------------------------------------------------+
+__rtti    +string const                                                                        +
+----------+------------------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                                 +
+----------+------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprLet|

.. _handle-ast-ExprLooksLikeCall:

.. das:attribute:: ExprLooksLikeCall

ExprLooksLikeCall fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprLooksLikeCall|

.. _handle-ast-ExprMakeArray:

.. das:attribute:: ExprMakeArray

ExprMakeArray fields are

+-----------+----------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+values     + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+-----------+----------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+recordType +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>`                                   +
+-----------+----------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+-----------+----------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------+
+__rtti     +string const                                                                            +
+-----------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeArray|

.. _handle-ast-ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

ExprMakeBlock fields are

+----------+------------------------------------------------------------------------+
+mmFlags   + :ref:`ExprMakeBlockFlags <alias-ExprMakeBlockFlags>`                   +
+----------+------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                           +
+----------+------------------------------------------------------------------------+
+capture   + :ref:`ast::dasvector`CaptureEntry <handle-ast-dasvector`CaptureEntry>` +
+----------+------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                           +
+----------+------------------------------------------------------------------------+
+stackTop  +uint                                                                    +
+----------+------------------------------------------------------------------------+
+block     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >             +
+----------+------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                               +
+----------+------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                 +
+----------+------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                     +
+----------+------------------------------------------------------------------------+
+__rtti    +string const                                                            +
+----------+------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeBlock|

.. _handle-ast-ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

ExprMakeGenerator fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+capture               + :ref:`ast::dasvector`CaptureEntry <handle-ast-dasvector`CaptureEntry>`                 +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+iterType              +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeGenerator|

.. _handle-ast-ExprMakeLocal:

.. das:attribute:: ExprMakeLocal

ExprMakeLocal fields are

+-----------+-------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+-----------+-------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+-----------+-------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+-----------+-------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>`  +
+-----------+-------------------------------------------------------+
+stackTop   +uint                                                   +
+-----------+-------------------------------------------------------+
+extraOffset+uint                                                   +
+-----------+-------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+-----------+-------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+-----------+-------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                    +
+-----------+-------------------------------------------------------+
+__rtti     +string const                                           +
+-----------+-------------------------------------------------------+


|structure_annotation-ast-ExprMakeLocal|

.. _handle-ast-ExprMakeStruct:

.. das:attribute:: ExprMakeStruct

ExprMakeStruct fields are

+---------------+----------------------------------------------------------------------------------------+
+makeType       +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+---------------+----------------------------------------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+---------------+----------------------------------------------------------------------------------------+
+structs        + :ref:`ast::dasvector`smart_ptr`MakeStruct <handle-ast-dasvector`smart_ptr`MakeStruct>` +
+---------------+----------------------------------------------------------------------------------------+
+printFlags     + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+---------------+----------------------------------------------------------------------------------------+
+makeFlags      + :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>`                                   +
+---------------+----------------------------------------------------------------------------------------+
+stackTop       +uint                                                                                    +
+---------------+----------------------------------------------------------------------------------------+
+extraOffset    +uint                                                                                    +
+---------------+----------------------------------------------------------------------------------------+
+genFlags       + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+---------------+----------------------------------------------------------------------------------------+
+_type          +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+---------------+----------------------------------------------------------------------------------------+
+flags          + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+---------------+----------------------------------------------------------------------------------------+
+__rtti         +string const                                                                            +
+---------------+----------------------------------------------------------------------------------------+
+makeStructFlags+ :ref:`ExprMakeStructFlags <alias-ExprMakeStructFlags>`                                 +
+---------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeStruct|

.. _handle-ast-ExprMakeTuple:

.. das:attribute:: ExprMakeTuple

ExprMakeTuple fields are

+-----------+----------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+values     + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+-----------+----------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+recordType +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>`                                   +
+-----------+----------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+-----------+----------------------------------------------------------------------------------------+
+isKeyValue +bool                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------+
+__rtti     +string const                                                                            +
+-----------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeTuple|

.. _handle-ast-ExprMakeVariant:

.. das:attribute:: ExprMakeVariant

ExprMakeVariant fields are

+-----------+----------------------------------------------------------------------------------------------+
+variants   + :ref:`ast::dasvector`smart_ptr`MakeFieldDecl <handle-ast-dasvector`smart_ptr`MakeFieldDecl>` +
+-----------+----------------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                       +
+-----------+----------------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                                 +
+-----------+----------------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                                 +
+-----------+----------------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>`                                         +
+-----------+----------------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                          +
+-----------+----------------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                          +
+-----------+----------------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                       +
+-----------+----------------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                                                           +
+-----------+----------------------------------------------------------------------------------------------+
+__rtti     +string const                                                                                  +
+-----------+----------------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMakeVariant|

.. _handle-ast-ExprMemZero:

.. das:attribute:: ExprMemZero

ExprMemZero fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprMemZero|

.. _handle-ast-ExprMove:

.. das:attribute:: ExprMove

ExprMove fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprMove|

.. _handle-ast-ExprNamedCall:

.. das:attribute:: ExprNamedCall

ExprNamedCall fields are

+----------------------+--------------------------------------------------------+
+arguments             + :ref:`ast::MakeStruct <handle-ast-MakeStruct>`         +
+----------------------+--------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`           +
+----------------------+--------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`           +
+----------------------+--------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>` +
+----------------------+--------------------------------------------------------+
+argumentsFailedToInfer+bool                                                    +
+----------------------+--------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`               +
+----------------------+--------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+----------------------+--------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                     +
+----------------------+--------------------------------------------------------+
+__rtti                +string const                                            +
+----------------------+--------------------------------------------------------+


|structure_annotation-ast-ExprNamedCall|

.. _handle-ast-ExprNew:

.. das:attribute:: ExprNew

ExprNew fields are

+----------------------+----------------------------------------------------------------------------------------+
+func                  + :ref:`ast::Function <handle-ast-Function>` ?                                           +
+----------------------+----------------------------------------------------------------------------------------+
+typeexpr              +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+initializer           +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprNew|

.. _handle-ast-ExprNullCoalescing:

.. das:attribute:: ExprNullCoalescing

ExprNullCoalescing fields are

+------------+-----------------------------------------------------------+
+defaultValue+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+------------+-----------------------------------------------------------+
+at          + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+------------+-----------------------------------------------------------+
+printFlags  + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+------------+-----------------------------------------------------------+
+subexpr     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+------------+-----------------------------------------------------------+
+genFlags    + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+------------+-----------------------------------------------------------+
+_type       +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+------------+-----------------------------------------------------------+
+__rtti      +string const                                               +
+------------+-----------------------------------------------------------+
+flags       + :ref:`ExprFlags <alias-ExprFlags>`                        +
+------------+-----------------------------------------------------------+


|structure_annotation-ast-ExprNullCoalescing|

.. _handle-ast-ExprOp:

.. das:attribute:: ExprOp

|structure_annotation-ast-ExprOp|

.. _handle-ast-ExprOp1:

.. das:attribute:: ExprOp1

ExprOp1 fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprOp1|

.. _handle-ast-ExprOp2:

.. das:attribute:: ExprOp2

ExprOp2 fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprOp2|

.. _handle-ast-ExprOp3:

.. das:attribute:: ExprOp3

ExprOp3 fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprOp3|

.. _handle-ast-ExprPtr2Ref:

.. das:attribute:: ExprPtr2Ref

ExprPtr2Ref fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprPtr2Ref|

.. _handle-ast-ExprReader:

.. das:attribute:: ExprReader

ExprReader fields are

+----------+-------------------------------------------------------------+
+macro     +smart_ptr< :ref:`ast::ReaderMacro <handle-ast-ReaderMacro>` >+
+----------+-------------------------------------------------------------+
+sequence  + :ref:`builtin::das_string <handle-builtin-das_string>`      +
+----------+-------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                +
+----------+-------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                +
+----------+-------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                    +
+----------+-------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >      +
+----------+-------------------------------------------------------------+
+__rtti    +string const                                                 +
+----------+-------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                          +
+----------+-------------------------------------------------------------+


|structure_annotation-ast-ExprReader|

.. _handle-ast-ExprRef2Ptr:

.. das:attribute:: ExprRef2Ptr

ExprRef2Ptr fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprRef2Ptr|

.. _handle-ast-ExprRef2Value:

.. das:attribute:: ExprRef2Value

ExprRef2Value fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprRef2Value|

.. _handle-ast-ExprReturn:

.. das:attribute:: ExprReturn

ExprReturn fields are

+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+stackTop   +uint                                                       +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----------+-----------------------------------------------------------+
+block      + :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?            +
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+refStackTop+uint                                                       +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+returnFlags+ :ref:`ExprReturnFlags <alias-ExprReturnFlags>`            +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprReturn|

.. _handle-ast-ExprSafeAsVariant:

.. das:attribute:: ExprSafeAsVariant

ExprSafeAsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+skipQQ    +bool                                                                     +
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


|structure_annotation-ast-ExprSafeAsVariant|

.. _handle-ast-ExprSafeAt:

.. das:attribute:: ExprSafeAt

ExprSafeAt fields are

+----------+-----------------------------------------------------------+
+index     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+atFlags   + :ref:`ExprAtFlags <alias-ExprAtFlags>`                    +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprSafeAt|

.. _handle-ast-ExprSafeField:

.. das:attribute:: ExprSafeField

ExprSafeField fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+skipQQ    +bool                                                                     +
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


|structure_annotation-ast-ExprSafeField|

.. _handle-ast-ExprStaticAssert:

.. das:attribute:: ExprStaticAssert

ExprStaticAssert fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprStaticAssert|

.. _handle-ast-ExprStringBuilder:

.. das:attribute:: ExprStringBuilder

ExprStringBuilder fields are

+----------+----------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------+----------------------------------------------------------------------------------------+
+elements  + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------+----------------------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`                                           +
+----------+----------------------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                                               +
+----------+----------------------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+__rtti    +string const                                                                            +
+----------+----------------------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------+----------------------------------------------------------------------------------------+


|structure_annotation-ast-ExprStringBuilder|

.. _handle-ast-ExprSwizzle:

.. das:attribute:: ExprSwizzle

ExprSwizzle fields are

+----------+------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > +
+----------+------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`               +
+----------+------------------------------------------------------------+
+fieldFlags+ :ref:`ExprSwizzleFieldFlags <alias-ExprSwizzleFieldFlags>` +
+----------+------------------------------------------------------------+
+mask      + :ref:`builtin::das_string <handle-builtin-das_string>`     +
+----------+------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`               +
+----------+------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                   +
+----------+------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >     +
+----------+------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                         +
+----------+------------------------------------------------------------+
+__rtti    +string const                                                +
+----------+------------------------------------------------------------+
+fields    + :ref:`ast::dasvector`uint8 <handle-ast-dasvector`uint8>`   +
+----------+------------------------------------------------------------+


|structure_annotation-ast-ExprSwizzle|

.. _handle-ast-ExprTryCatch:

.. das:attribute:: ExprTryCatch

ExprTryCatch fields are

+-----------+-----------------------------------------------------------+
+try_block  +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----------+-----------------------------------------------------------+
+catch_block+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                        +
+-----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprTryCatch|

.. _handle-ast-ExprTypeInfo:

.. das:attribute:: ExprTypeInfo

ExprTypeInfo fields are

+----------+-----------------------------------------------------------+
+typeexpr  +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+extratrait+ :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+macro     + :ref:`ast::TypeInfoMacro <handle-ast-TypeInfoMacro>` ?    +
+----------+-----------------------------------------------------------+
+subtrait  + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+trait     + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprTypeInfo|

.. _handle-ast-ExprUnsafe:

.. das:attribute:: ExprUnsafe

ExprUnsafe fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+body      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprUnsafe|

.. _handle-ast-ExprVar:

.. das:attribute:: ExprVar

ExprVar fields are

+-------------+--------------------------------------------------------+
+at           + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`           +
+-------------+--------------------------------------------------------+
+variable     +smart_ptr< :ref:`ast::Variable <handle-ast-Variable>` > +
+-------------+--------------------------------------------------------+
+varFlags     + :ref:`ExprVarFlags <alias-ExprVarFlags>`               +
+-------------+--------------------------------------------------------+
+printFlags   + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`           +
+-------------+--------------------------------------------------------+
+argumentIndex+int                                                     +
+-------------+--------------------------------------------------------+
+name         + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------------+--------------------------------------------------------+
+genFlags     + :ref:`ExprGenFlags <alias-ExprGenFlags>`               +
+-------------+--------------------------------------------------------+
+pBlock       + :ref:`ast::ExprBlock <handle-ast-ExprBlock>` ?         +
+-------------+--------------------------------------------------------+
+_type        +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > +
+-------------+--------------------------------------------------------+
+flags        + :ref:`ExprFlags <alias-ExprFlags>`                     +
+-------------+--------------------------------------------------------+
+__rtti       +string const                                            +
+-------------+--------------------------------------------------------+


|structure_annotation-ast-ExprVar|

.. _handle-ast-ExprWhile:

.. das:attribute:: ExprWhile

ExprWhile fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+body      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+cond      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprWhile|

.. _handle-ast-ExprWith:

.. das:attribute:: ExprWith

ExprWith fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+body      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_with     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprWith|

.. _handle-ast-ExprYield:

.. das:attribute:: ExprYield

ExprYield fields are

+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias-ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+returnFlags+ :ref:`ExprYieldFlags <alias-ExprYieldFlags>`              +
+-----------+-----------------------------------------------------------+


|structure_annotation-ast-ExprYield|

.. _handle-ast-Expression:

.. das:attribute:: Expression

Expression fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                    +
+----------+-------------------------------------------------------+


|structure_annotation-ast-Expression|

.. _handle-ast-FieldDeclaration:

.. das:attribute:: FieldDeclaration

FieldDeclaration fields are

+----------+--------------------------------------------------------------------------+
+annotation+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+----------+--------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                             +
+----------+--------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle-builtin-das_string>`                   +
+----------+--------------------------------------------------------------------------+
+init      +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >               +
+----------+--------------------------------------------------------------------------+
+offset    +int                                                                       +
+----------+--------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                   +
+----------+--------------------------------------------------------------------------+
+flags     + :ref:`FieldDeclarationFlags <alias-FieldDeclarationFlags>`               +
+----------+--------------------------------------------------------------------------+


|structure_annotation-ast-FieldDeclaration|

.. _handle-ast-Function:

.. das:attribute:: Function

Function fields are

+---------------+------------------------------------------------------------------------------------+
+arguments      + :ref:`ast::dasvector`smart_ptr`Variable <handle-ast-dasvector`smart_ptr`Variable>` +
+---------------+------------------------------------------------------------------------------------+
+fromGeneric    + :ref:`ast::Function <handle-ast-Function>` ?                                       +
+---------------+------------------------------------------------------------------------------------+
+result         +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                             +
+---------------+------------------------------------------------------------------------------------+
+aotHash        +uint64                                                                              +
+---------------+------------------------------------------------------------------------------------+
+totalGenLabel  +int                                                                                 +
+---------------+------------------------------------------------------------------------------------+
+_module        + :ref:`rtti::Module <handle-rtti-Module>` ?                                         +
+---------------+------------------------------------------------------------------------------------+
+index          +int                                                                                 +
+---------------+------------------------------------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                       +
+---------------+------------------------------------------------------------------------------------+
+inferStack     + :ref:`ast::dasvector`InferHistory <handle-ast-dasvector`InferHistory>`             +
+---------------+------------------------------------------------------------------------------------+
+body           +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >                         +
+---------------+------------------------------------------------------------------------------------+
+atDecl         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                       +
+---------------+------------------------------------------------------------------------------------+
+sideEffectFlags+ :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>`                     +
+---------------+------------------------------------------------------------------------------------+
+annotations    + :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`                           +
+---------------+------------------------------------------------------------------------------------+
+totalStackSize +uint                                                                                +
+---------------+------------------------------------------------------------------------------------+
+name           + :ref:`builtin::das_string <handle-builtin-das_string>`                             +
+---------------+------------------------------------------------------------------------------------+
+hash           +uint64                                                                              +
+---------------+------------------------------------------------------------------------------------+
+flags          + :ref:`FunctionFlags <alias-FunctionFlags>`                                         +
+---------------+------------------------------------------------------------------------------------+


|structure_annotation-ast-Function|

.. _handle-ast-FunctionAnnotation:

.. das:attribute:: FunctionAnnotation

|structure_annotation-ast-FunctionAnnotation|

.. _handle-ast-InferHistory:

.. das:attribute:: InferHistory

InferHistory fields are

+----+----------------------------------------------+
+func+ :ref:`ast::Function <handle-ast-Function>` ? +
+----+----------------------------------------------+
+at  + :ref:`rtti::LineInfo <handle-rtti-LineInfo>` +
+----+----------------------------------------------+


|structure_annotation-ast-InferHistory|

.. _handle-ast-MakeFieldDecl:

.. das:attribute:: MakeFieldDecl

MakeFieldDecl fields are

+-----+-----------------------------------------------------------+
+value+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+-----+-----------------------------------------------------------+
+at   + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+-----+-----------------------------------------------------------+
+name + :ref:`builtin::das_string <handle-builtin-das_string>`    +
+-----+-----------------------------------------------------------+
+flags+ :ref:`MakeFieldDeclFlags <alias-MakeFieldDeclFlags>`      +
+-----+-----------------------------------------------------------+


|structure_annotation-ast-MakeFieldDecl|

.. _handle-ast-ModuleGroup:

.. das:attribute:: ModuleGroup

|structure_annotation-ast-ModuleGroup|

.. _handle-ast-ModuleLibrary:

.. das:attribute:: ModuleLibrary

|structure_annotation-ast-ModuleLibrary|

.. _handle-ast-PassMacro:

.. das:attribute:: PassMacro

PassMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+


|structure_annotation-ast-PassMacro|

.. _handle-ast-ReaderMacro:

.. das:attribute:: ReaderMacro

ReaderMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


|structure_annotation-ast-ReaderMacro|

.. _handle-ast-Structure:

.. das:attribute:: Structure

Structure fields are

+-----------+--------------------------------------------------------------------------------+
+_module    + :ref:`rtti::Module <handle-rtti-Module>` ?                                     +
+-----------+--------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                   +
+-----------+--------------------------------------------------------------------------------+
+parent     + :ref:`ast::Structure <handle-ast-Structure>` ?                                 +
+-----------+--------------------------------------------------------------------------------+
+annotations+ :ref:`rtti::AnnotationList <handle-rtti-AnnotationList>`                       +
+-----------+--------------------------------------------------------------------------------+
+name       + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+-----------+--------------------------------------------------------------------------------+
+fields     + :ref:`ast::dasvector`FieldDeclaration <handle-ast-dasvector`FieldDeclaration>` +
+-----------+--------------------------------------------------------------------------------+
+flags      + :ref:`StructureFlags <alias-StructureFlags>`                                   +
+-----------+--------------------------------------------------------------------------------+


|structure_annotation-ast-Structure|

.. _handle-ast-StructureAnnotation:

.. das:attribute:: StructureAnnotation

|structure_annotation-ast-StructureAnnotation|

.. _handle-ast-TypeDecl:

.. das:attribute:: TypeDecl

TypeDecl fields are

+----------+----------------------------------------------------------------------------------------+
+alias     + :ref:`builtin::das_string <handle-builtin-das_string>`                                 +
+----------+----------------------------------------------------------------------------------------+
+annotation+ :ref:`rtti::TypeAnnotation <handle-rtti-TypeAnnotation>` ?                             +
+----------+----------------------------------------------------------------------------------------+
+dimExpr   + :ref:`ast::dasvector`smart_ptr`Expression <handle-ast-dasvector`smart_ptr`Expression>` +
+----------+----------------------------------------------------------------------------------------+
+argTypes  + :ref:`ast::dasvector`smart_ptr`TypeDecl <handle-ast-dasvector`smart_ptr`TypeDecl>`     +
+----------+----------------------------------------------------------------------------------------+
+dim       + :ref:`ast::dasvector`int <handle-ast-dasvector`int>`                                   +
+----------+----------------------------------------------------------------------------------------+
+_module   + :ref:`rtti::Module <handle-rtti-Module>` ?                                             +
+----------+----------------------------------------------------------------------------------------+
+secondType+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                           +
+----------+----------------------------------------------------------------------------------------+
+enumType  + :ref:`ast::Enumeration <handle-ast-Enumeration>` ?                                     +
+----------+----------------------------------------------------------------------------------------+
+argNames  + :ref:`ast::dasvector`das_string <handle-ast-dasvector`das_string>`                     +
+----------+----------------------------------------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum-rtti-Type>`                                                     +
+----------+----------------------------------------------------------------------------------------+
+firstType +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+structType+ :ref:`ast::Structure <handle-ast-Structure>` ?                                         +
+----------+----------------------------------------------------------------------------------------+
+flags     + :ref:`TypeDeclFlags <alias-TypeDeclFlags>`                                             +
+----------+----------------------------------------------------------------------------------------+


TypeDecl properties are

+---------------+------------------------------------+
+isCtorType     +bool                                +
+---------------+------------------------------------+
+alignOf        +int                                 +
+---------------+------------------------------------+
+vectorBaseType + :ref:`rtti::Type <enum-rtti-Type>` +
+---------------+------------------------------------+
+vectorDim      +int                                 +
+---------------+------------------------------------+
+isVoid         +bool                                +
+---------------+------------------------------------+
+isAuto         +bool                                +
+---------------+------------------------------------+
+canMove        +bool                                +
+---------------+------------------------------------+
+isExprType     +bool                                +
+---------------+------------------------------------+
+sizeOf         +int                                 +
+---------------+------------------------------------+
+isAlias        +bool                                +
+---------------+------------------------------------+
+baseSizeOf     +int                                 +
+---------------+------------------------------------+
+isString       +bool                                +
+---------------+------------------------------------+
+canCopy        +bool                                +
+---------------+------------------------------------+
+countOf        +int                                 +
+---------------+------------------------------------+
+isWorkhorseType+bool                                +
+---------------+------------------------------------+


|structure_annotation-ast-TypeDecl|

.. _handle-ast-TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

TypeInfoMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


|structure_annotation-ast-TypeInfoMacro|

.. _handle-ast-Variable:

.. das:attribute:: Variable

Variable fields are

+-------------+--------------------------------------------------------------------------+
+annotation   + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` +
+-------------+--------------------------------------------------------------------------+
+initStackSize+uint                                                                      +
+-------------+--------------------------------------------------------------------------+
+_module      + :ref:`rtti::Module <handle-rtti-Module>` ?                               +
+-------------+--------------------------------------------------------------------------+
+index        +int                                                                       +
+-------------+--------------------------------------------------------------------------+
+at           + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                             +
+-------------+--------------------------------------------------------------------------+
+stackTop     +uint                                                                      +
+-------------+--------------------------------------------------------------------------+
+name         + :ref:`builtin::das_string <handle-builtin-das_string>`                   +
+-------------+--------------------------------------------------------------------------+
+init         +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >               +
+-------------+--------------------------------------------------------------------------+
+access_flags + :ref:`VariableAccessFlags <alias-VariableAccessFlags>`                   +
+-------------+--------------------------------------------------------------------------+
+source       +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >               +
+-------------+--------------------------------------------------------------------------+
+_type        +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                   +
+-------------+--------------------------------------------------------------------------+
+flags        + :ref:`VariableFlags <alias-VariableFlags>`                               +
+-------------+--------------------------------------------------------------------------+


Variable properties are

+--------------+----+
+isAccessUnused+bool+
+--------------+----+


|structure_annotation-ast-Variable|

.. _handle-ast-VariantMacro:

.. das:attribute:: VariantMacro

VariantMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+


|structure_annotation-ast-VariantMacro|

.. _handle-ast-VisitorAdapter:

.. das:attribute:: VisitorAdapter

|structure_annotation-ast-VisitorAdapter|

+++++++++++++
Handled types
+++++++++++++

.. _handle-ast-MakeStruct:

.. das:attribute:: MakeStruct

|any_annotation-ast-MakeStruct|

.. _handle-ast-dasvector`CaptureEntry:

.. das:attribute:: dasvector`CaptureEntry

|any_annotation-ast-dasvector`CaptureEntry|

.. _handle-ast-dasvector`EnumEntry:

.. das:attribute:: dasvector`EnumEntry

|any_annotation-ast-dasvector`EnumEntry|

.. _handle-ast-dasvector`FieldDeclaration:

.. das:attribute:: dasvector`FieldDeclaration

|any_annotation-ast-dasvector`FieldDeclaration|

.. _handle-ast-dasvector`InferHistory:

.. das:attribute:: dasvector`InferHistory

|any_annotation-ast-dasvector`InferHistory|

.. _handle-ast-dasvector`LineInfo:

.. das:attribute:: dasvector`LineInfo

|any_annotation-ast-dasvector`LineInfo|

.. _handle-ast-dasvector`das_string:

.. das:attribute:: dasvector`das_string

|any_annotation-ast-dasvector`das_string|

.. _handle-ast-dasvector`int:

.. das:attribute:: dasvector`int

|any_annotation-ast-dasvector`int|

.. _handle-ast-dasvector`pair`uint`uint:

.. das:attribute:: dasvector`pair`uint`uint

|any_annotation-ast-dasvector`pair`uint`uint|

.. _handle-ast-dasvector`smart_ptr`Expression:

.. das:attribute:: dasvector`smart_ptr`Expression

|any_annotation-ast-dasvector`smart_ptr`Expression|

.. _handle-ast-dasvector`smart_ptr`MakeFieldDecl:

.. das:attribute:: dasvector`smart_ptr`MakeFieldDecl

|any_annotation-ast-dasvector`smart_ptr`MakeFieldDecl|

.. _handle-ast-dasvector`smart_ptr`MakeStruct:

.. das:attribute:: dasvector`smart_ptr`MakeStruct

|any_annotation-ast-dasvector`smart_ptr`MakeStruct|

.. _handle-ast-dasvector`smart_ptr`TypeDecl:

.. das:attribute:: dasvector`smart_ptr`TypeDecl

|any_annotation-ast-dasvector`smart_ptr`TypeDecl|

.. _handle-ast-dasvector`smart_ptr`Variable:

.. das:attribute:: dasvector`smart_ptr`Variable

|any_annotation-ast-dasvector`smart_ptr`Variable|

.. _handle-ast-dasvector`uint8:

.. das:attribute:: dasvector`uint8

|any_annotation-ast-dasvector`uint8|

+++++++
Classes
+++++++

.. _struct-ast-AstCallMacro:

.. das:attribute:: AstCallMacro

|class-ast-AstCallMacro|

it defines as follows

  __rtti : void?

.. das:function:: AstCallMacro.visit(self: AstCallMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprCallMacro> const)

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


|method-ast-AstCallMacro.visit|

.. _struct-ast-AstFunctionAnnotation:

.. das:attribute:: AstFunctionAnnotation

|class-ast-AstFunctionAnnotation|

it defines as follows

  __rtti : void?

.. das:function:: AstFunctionAnnotation.transform(self: AstFunctionAnnotation; call: smart_ptr<ast::ExprCall>; errors: das_string)

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+--------+----------------------------------------------------------------------+
+call    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >               +
+--------+----------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`               +
+--------+----------------------------------------------------------------------+


|method-ast-AstFunctionAnnotation.transform|

.. das:function:: AstFunctionAnnotation.apply(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast-AstFunctionAnnotation.apply|

.. das:function:: AstFunctionAnnotation.finish(self: AstFunctionAnnotation; func: FunctionPtr; group: ModuleGroup; args: AnnotationArgumentList const; progArgs: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+--------+--------------------------------------------------------------------------------+
+func    + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+progArgs+ :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast-AstFunctionAnnotation.finish|

.. _struct-ast-AstPassMacro:

.. das:attribute:: AstPassMacro

|class-ast-AstPassMacro|

it defines as follows

  __rtti : void?

.. das:function:: AstPassMacro.apply(self: AstPassMacro; prog: ProgramPtr; mod: rtti::Module? const)

apply returns bool

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+self    + :ref:`ast::AstPassMacro <struct-ast-AstPassMacro>` +
+--------+----------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`               +
+--------+----------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+--------+----------------------------------------------------+


|method-ast-AstPassMacro.apply|

.. _struct-ast-AstReaderMacro:

.. das:attribute:: AstReaderMacro

|class-ast-AstReaderMacro|

it defines as follows

  __rtti : void?

.. das:function:: AstReaderMacro.accept(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: ast::ExprReader? const; ch: int const; info: LineInfo const)

accept returns bool

+--------+--------------------------------------------------------+
+argument+argument type                                           +
+========+========================================================+
+self    + :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+--------+--------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                   +
+--------+--------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const       +
+--------+--------------------------------------------------------+
+expr    + :ref:`ast::ExprReader <handle-ast-ExprReader>` ? const +
+--------+--------------------------------------------------------+
+ch      +int const                                               +
+--------+--------------------------------------------------------+
+info    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const    +
+--------+--------------------------------------------------------+


|method-ast-AstReaderMacro.accept|

.. das:function:: AstReaderMacro.visit(self: AstReaderMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprReader> const)

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+--------+-----------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                            +
+--------+-----------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstReaderMacro.visit|

.. _struct-ast-AstStructureAnnotation:

.. das:attribute:: AstStructureAnnotation

|class-ast-AstStructureAnnotation|

it defines as follows

  __rtti : void?

.. das:function:: AstStructureAnnotation.apply(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

apply returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast-AstStructureAnnotation.apply|

.. das:function:: AstStructureAnnotation.finish(self: AstStructureAnnotation; st: StructurePtr; group: ModuleGroup; args: AnnotationArgumentList const; errors: das_string)

finish returns bool

+--------+--------------------------------------------------------------------------------+
+argument+argument type                                                                   +
+========+================================================================================+
+self    + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+--------+--------------------------------------------------------------------------------+
+st      + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+--------+--------------------------------------------------------------------------------+
+group   + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+--------+--------------------------------------------------------------------------------+
+args    + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+--------+--------------------------------------------------------------------------------+
+errors  + :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+--------+--------------------------------------------------------------------------------+


|method-ast-AstStructureAnnotation.finish|

.. _struct-ast-AstVariantMacro:

.. das:attribute:: AstVariantMacro

|class-ast-AstVariantMacro|

it defines as follows

  __rtti : void?

.. das:function:: AstVariantMacro.visitExprIsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprIsVariant> const)

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVariantMacro.visitExprIsVariant|

.. das:function:: AstVariantMacro.visitExprAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprAsVariant> const)

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+--------+-----------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+--------+-----------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVariantMacro.visitExprAsVariant|

.. das:function:: AstVariantMacro.visitExprSafeAsVariant(self: AstVariantMacro; prog: ProgramPtr; mod: rtti::Module? const; expr: smart_ptr<ast::ExprSafeAsVariant> const)

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+--------+-------------------------------------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`                                          +
+--------+-------------------------------------------------------------------------------+
+mod     + :ref:`rtti::Module <handle-rtti-Module>` ? const                              +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVariantMacro.visitExprSafeAsVariant|

.. _struct-ast-AstVisitor:

.. das:attribute:: AstVisitor

|class-ast-AstVisitor|

it defines as follows

  __rtti : void?

.. das:function:: AstVisitor.preVisitProgram(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitProgram|

.. das:function:: AstVisitor.visitProgram(self: AstVisitor; porg: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+porg    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitProgram|

.. das:function:: AstVisitor.preVisitProgramBody(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitProgramBody|

.. das:function:: AstVisitor.preVisitTypeDecl(self: AstVisitor; typ: TypeDeclPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitTypeDecl|

.. das:function:: AstVisitor.visitTypeDecl(self: AstVisitor; typ: TypeDeclPtr)

visitTypeDecl returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>`

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitTypeDecl|

.. das:function:: AstVisitor.preVisitAlias(self: AstVisitor; typ: TypeDeclPtr; name: das_string const)

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+--------+--------------------------------------------------------------+
+typ     + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+--------+--------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+


|method-ast-AstVisitor.preVisitAlias|

.. das:function:: AstVisitor.visitAlias(self: AstVisitor; typ: TypeDeclPtr; name: das_string const)

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


|method-ast-AstVisitor.visitAlias|

.. das:function:: AstVisitor.preVisitEnumeration(self: AstVisitor; enu: EnumerationPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitEnumeration|

.. das:function:: AstVisitor.preVisitEnumerationValue(self: AstVisitor; enu: EnumerationPtr; name: das_string const; value: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitEnumerationValue|

.. das:function:: AstVisitor.visitEnumerationValue(self: AstVisitor; enu: EnumerationPtr; name: das_string const; value: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitEnumerationValue|

.. das:function:: AstVisitor.visitEnumeration(self: AstVisitor; enu: EnumerationPtr)

visitEnumeration returns  :ref:`EnumerationPtr <alias-EnumerationPtr>`

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+enu     + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitEnumeration|

.. das:function:: AstVisitor.preVisitStructure(self: AstVisitor; str: StructurePtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`       +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitStructure|

.. das:function:: AstVisitor.preVisitStructureField(self: AstVisitor; str: StructurePtr; decl: FieldDeclaration const; last: bool const)

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


|method-ast-AstVisitor.preVisitStructureField|

.. das:function:: AstVisitor.visitStructureField(self: AstVisitor; str: StructurePtr; decl: FieldDeclaration const; last: bool const)

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


|method-ast-AstVisitor.visitStructureField|

.. das:function:: AstVisitor.visitStructure(self: AstVisitor; str: StructurePtr)

visitStructure returns  :ref:`StructurePtr <alias-StructurePtr>`

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+str     + :ref:`StructurePtr <alias-StructurePtr>`       +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitStructure|

.. das:function:: AstVisitor.preVisitFunction(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitFunction|

.. das:function:: AstVisitor.visitFunction(self: AstVisitor; fun: FunctionPtr)

visitFunction returns  :ref:`FunctionPtr <alias-FunctionPtr>`

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitFunction|

.. das:function:: AstVisitor.preVisitFunctionArgument(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.preVisitFunctionArgument|

.. das:function:: AstVisitor.visitFunctionArgument(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.visitFunctionArgument|

.. das:function:: AstVisitor.preVisitFunctionArgumentInit(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; value: ExpressionPtr)

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


|method-ast-AstVisitor.preVisitFunctionArgumentInit|

.. das:function:: AstVisitor.visitFunctionArgumentInit(self: AstVisitor; fun: FunctionPtr; arg: VariablePtr; value: ExpressionPtr)

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


|method-ast-AstVisitor.visitFunctionArgumentInit|

.. das:function:: AstVisitor.preVisitFunctionBody(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitFunctionBody|

.. das:function:: AstVisitor.visitFunctionBody(self: AstVisitor; fun: FunctionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+fun     + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitFunctionBody|

.. das:function:: AstVisitor.preVisitExpression(self: AstVisitor; expr: ExpressionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitExpression|

.. das:function:: AstVisitor.visitExpression(self: AstVisitor; expr: ExpressionPtr)

visitExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitExpression|

.. das:function:: AstVisitor.preVisitExprBlock(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprBlock|

.. das:function:: AstVisitor.visitExprBlock(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

visitExprBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprBlock|

.. das:function:: AstVisitor.preVisitExprBlockArgument(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.preVisitExprBlockArgument|

.. das:function:: AstVisitor.visitExprBlockArgument(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.visitExprBlockArgument|

.. das:function:: AstVisitor.preVisitExprBlockArgumentInit(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; expr: ExpressionPtr)

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


|method-ast-AstVisitor.preVisitExprBlockArgumentInit|

.. das:function:: AstVisitor.visitExprBlockArgumentInit(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; arg: VariablePtr; expr: ExpressionPtr)

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


|method-ast-AstVisitor.visitExprBlockArgumentInit|

.. das:function:: AstVisitor.preVisitExprBlockExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprBlockExpression|

.. das:function:: AstVisitor.visitExprBlockExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

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


|method-ast-AstVisitor.visitExprBlockExpression|

.. das:function:: AstVisitor.preVisitExprBlockFinal(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprBlockFinal|

.. das:function:: AstVisitor.visitExprBlockFinal(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprBlockFinal|

.. das:function:: AstVisitor.preVisitExprBlockFinalExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+blk     +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+--------+---------------------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprBlockFinalExpression|

.. das:function:: AstVisitor.visitExprBlockFinalExpression(self: AstVisitor; blk: smart_ptr<ast::ExprBlock> const; expr: ExpressionPtr)

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


|method-ast-AstVisitor.visitExprBlockFinalExpression|

.. das:function:: AstVisitor.preVisitExprLet(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprLet|

.. das:function:: AstVisitor.visitExprLet(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const)

visitExprLet returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprLet|

.. das:function:: AstVisitor.preVisitExprLetVariable(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.preVisitExprLetVariable|

.. das:function:: AstVisitor.visitExprLetVariable(self: AstVisitor; expr: smart_ptr<ast::ExprLet> const; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.visitExprLetVariable|

.. das:function:: AstVisitor.preVisitExprLetVariableInit(self: AstVisitor; blk: smart_ptr<ast::ExprLet> const; arg: VariablePtr; expr: ExpressionPtr)

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


|method-ast-AstVisitor.preVisitExprLetVariableInit|

.. das:function:: AstVisitor.visitExprLetVariableInit(self: AstVisitor; blk: smart_ptr<ast::ExprLet> const; arg: VariablePtr; expr: ExpressionPtr)

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


|method-ast-AstVisitor.visitExprLetVariableInit|

.. das:function:: AstVisitor.preVisitGlobalLet(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitGlobalLet|

.. das:function:: AstVisitor.visitGlobalLet(self: AstVisitor; prog: ProgramPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+prog    + :ref:`ProgramPtr <alias-ProgramPtr>`           +
+--------+------------------------------------------------+


|method-ast-AstVisitor.visitGlobalLet|

.. das:function:: AstVisitor.preVisitGlobalLetVariable(self: AstVisitor; arg: VariablePtr; lastArg: bool const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+lastArg +bool const                                      +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitGlobalLetVariable|

.. das:function:: AstVisitor.visitGlobalLetVariable(self: AstVisitor; arg: VariablePtr; lastArg: bool const)

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


|method-ast-AstVisitor.visitGlobalLetVariable|

.. das:function:: AstVisitor.preVisitGlobalLetVariableInit(self: AstVisitor; arg: VariablePtr; expr: ExpressionPtr)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+--------+------------------------------------------------+
+arg     + :ref:`VariablePtr <alias-VariablePtr>`         +
+--------+------------------------------------------------+
+expr    + :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+--------+------------------------------------------------+


|method-ast-AstVisitor.preVisitGlobalLetVariableInit|

.. das:function:: AstVisitor.visitGlobalLetVariableInit(self: AstVisitor; arg: VariablePtr; expr: ExpressionPtr)

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


|method-ast-AstVisitor.visitGlobalLetVariableInit|

.. das:function:: AstVisitor.preVisitExprStringBuilder(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprStringBuilder|

.. das:function:: AstVisitor.visitExprStringBuilder(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const)

visitExprStringBuilder returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprStringBuilder|

.. das:function:: AstVisitor.preVisitExprStringBuilderElement(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const; elem: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprStringBuilderElement|

.. das:function:: AstVisitor.visitExprStringBuilderElement(self: AstVisitor; expr: smart_ptr<ast::ExprStringBuilder> const; elem: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprStringBuilderElement|

.. das:function:: AstVisitor.preVisitExprNew(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprNew|

.. das:function:: AstVisitor.visitExprNew(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const)

visitExprNew returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprNew|

.. das:function:: AstVisitor.preVisitExprNewArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprNewArgument|

.. das:function:: AstVisitor.visitExprNewArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNew> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprNewArgument|

.. das:function:: AstVisitor.preVisitExprNamedCall(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprNamedCall|

.. das:function:: AstVisitor.visitExprNamedCall(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const)

visitExprNamedCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprNamedCall|

.. das:function:: AstVisitor.preVisitExprNamedCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const; arg: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprNamedCallArgument|

.. das:function:: AstVisitor.visitExprNamedCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprNamedCall> const; arg: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprNamedCallArgument|

.. das:function:: AstVisitor.preVisitExprLooksLikeCall(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprLooksLikeCall|

.. das:function:: AstVisitor.visitExprLooksLikeCall(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const)

visitExprLooksLikeCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprLooksLikeCall|

.. das:function:: AstVisitor.preVisitExprLooksLikeCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprLooksLikeCallArgument|

.. das:function:: AstVisitor.visitExprLooksLikeCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprLooksLikeCall> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprLooksLikeCallArgument|

.. das:function:: AstVisitor.preVisitExprCall(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCall|

.. das:function:: AstVisitor.visitExprCall(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const)

visitExprCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprCall|

.. das:function:: AstVisitor.preVisitExprCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprCallArgument|

.. das:function:: AstVisitor.visitExprCallArgument(self: AstVisitor; expr: smart_ptr<ast::ExprCall> const; arg: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprCallArgument|

.. das:function:: AstVisitor.preVisitExprNullCoalescing(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprNullCoalescing|

.. das:function:: AstVisitor.visitExprNullCoalescing(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const)

visitExprNullCoalescing returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprNullCoalescing|

.. das:function:: AstVisitor.preVisitExprNullCoalescingDefault(self: AstVisitor; expr: smart_ptr<ast::ExprNullCoalescing> const; defval: ExpressionPtr)

+--------+---------------------------------------------------------------------------------+
+argument+argument type                                                                    +
+========+=================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+--------+---------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+--------+---------------------------------------------------------------------------------+
+defval  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                      +
+--------+---------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprNullCoalescingDefault|

.. das:function:: AstVisitor.preVisitExprAt(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAt|

.. das:function:: AstVisitor.visitExprAt(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const)

visitExprAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.visitExprAt|

.. das:function:: AstVisitor.preVisitExprAtIndex(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; index: ExpressionPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+index   + :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAtIndex|

.. das:function:: AstVisitor.preVisitExprSafeAt(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAt> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprSafeAt|

.. das:function:: AstVisitor.visitExprSafeAt(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAt> const)

visitExprSafeAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprSafeAt|

.. das:function:: AstVisitor.preVisitExprSafeAtIndex(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; index: ExpressionPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+index   + :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprSafeAtIndex|

.. das:function:: AstVisitor.preVisitExprIs(self: AstVisitor; expr: smart_ptr<ast::ExprIs> const)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIs|

.. das:function:: AstVisitor.visitExprIs(self: AstVisitor; expr: smart_ptr<ast::ExprIs> const)

visitExprIs returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.visitExprIs|

.. das:function:: AstVisitor.preVisitExprIsType(self: AstVisitor; expr: smart_ptr<ast::ExprAt> const; typeDecl: TypeDeclPtr)

+--------+---------------------------------------------------------+
+argument+argument type                                            +
+========+=========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+typeDecl+ :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                  +
+--------+---------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIsType|

.. das:function:: AstVisitor.preVisitExprOp2(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp2|

.. das:function:: AstVisitor.visitExprOp2(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const)

visitExprOp2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprOp2|

.. das:function:: AstVisitor.preVisitExprOp2Right(self: AstVisitor; expr: smart_ptr<ast::ExprOp2> const; right: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+--------+-----------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp2Right|

.. das:function:: AstVisitor.preVisitExprOp3(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp3|

.. das:function:: AstVisitor.visitExprOp3(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const)

visitExprOp3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprOp3|

.. das:function:: AstVisitor.preVisitExprOp3Left(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const; left: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+
+left    + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp3Left|

.. das:function:: AstVisitor.preVisitExprOp3Right(self: AstVisitor; expr: smart_ptr<ast::ExprOp3> const; right: ExpressionPtr)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+--------+-----------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp3Right|

.. das:function:: AstVisitor.preVisitExprCopy(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCopy|

.. das:function:: AstVisitor.visitExprCopy(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const)

visitExprCopy returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprCopy|

.. das:function:: AstVisitor.preVisitExprCopyRight(self: AstVisitor; expr: smart_ptr<ast::ExprCopy> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCopyRight|

.. das:function:: AstVisitor.preVisitExprMove(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMove|

.. das:function:: AstVisitor.visitExprMove(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const)

visitExprMove returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMove|

.. das:function:: AstVisitor.preVisitExprMoveRight(self: AstVisitor; expr: smart_ptr<ast::ExprMove> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMoveRight|

.. das:function:: AstVisitor.preVisitExprClone(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprClone|

.. das:function:: AstVisitor.visitExprClone(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const)

visitExprClone returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprClone|

.. das:function:: AstVisitor.preVisitExprCloneRight(self: AstVisitor; expr: smart_ptr<ast::ExprClone> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+--------+---------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCloneRight|

.. das:function:: AstVisitor.preVisitExprWith(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprWith|

.. das:function:: AstVisitor.visitExprWith(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const)

visitExprWith returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprWith|

.. das:function:: AstVisitor.preVisitExprWithBody(self: AstVisitor; expr: smart_ptr<ast::ExprWith> const; right: ExpressionPtr)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+--------+-------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprWithBody|

.. das:function:: AstVisitor.preVisitExprWhile(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprWhile|

.. das:function:: AstVisitor.visitExprWhile(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const)

visitExprWhile returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprWhile|

.. das:function:: AstVisitor.preVisitExprWhileBody(self: AstVisitor; expr: smart_ptr<ast::ExprWhile> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+--------+---------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprWhileBody|

.. das:function:: AstVisitor.preVisitExprTryCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprTryCatch|

.. das:function:: AstVisitor.visitExprTryCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const)

visitExprTryCatch returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprTryCatch|

.. das:function:: AstVisitor.preVisitExprTryCatchCatch(self: AstVisitor; expr: smart_ptr<ast::ExprTryCatch> const; right: ExpressionPtr)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+--------+---------------------------------------------------------------------+
+right   + :ref:`ExpressionPtr <alias-ExpressionPtr>`                          +
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprTryCatchCatch|

.. das:function:: AstVisitor.preVisitExprIfThenElse(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIfThenElse|

.. das:function:: AstVisitor.visitExprIfThenElse(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const)

visitExprIfThenElse returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprIfThenElse|

.. das:function:: AstVisitor.preVisitExprIfThenElseIfBlock(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const; ifBlock: ExpressionPtr)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+--------+-------------------------------------------------------------------------+
+ifBlock + :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIfThenElseIfBlock|

.. das:function:: AstVisitor.preVisitExprIfThenElseElseBlock(self: AstVisitor; expr: smart_ptr<ast::ExprIfThenElse> const; elseBlock: ExpressionPtr)

+---------+-------------------------------------------------------------------------+
+argument +argument type                                                            +
+=========+=========================================================================+
+self     + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+---------+-------------------------------------------------------------------------+
+expr     +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+---------+-------------------------------------------------------------------------+
+elseBlock+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+---------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIfThenElseElseBlock|

.. das:function:: AstVisitor.preVisitExprFor(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprFor|

.. das:function:: AstVisitor.visitExprFor(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

visitExprFor returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprFor|

.. das:function:: AstVisitor.preVisitExprForVariable(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; svar: VariablePtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprForVariable|

.. das:function:: AstVisitor.visitExprForVariable(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; svar: VariablePtr; last: bool const)

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


|method-ast-AstVisitor.visitExprForVariable|

.. das:function:: AstVisitor.preVisitExprForSource(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; source: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprForSource|

.. das:function:: AstVisitor.visitExprForSource(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const; source: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprForSource|

.. das:function:: AstVisitor.preVisitExprForStack(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprForStack|

.. das:function:: AstVisitor.preVisitExprForBody(self: AstVisitor; expr: smart_ptr<ast::ExprFor> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprForBody|

.. das:function:: AstVisitor.preVisitExprMakeVariant(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeVariant|

.. das:function:: AstVisitor.visitExprMakeVariant(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const)

visitExprMakeVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeVariant|

.. das:function:: AstVisitor.preVisitExprMakeVariantField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprMakeVariantField|

.. das:function:: AstVisitor.visitExprMakeVariantField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeVariant> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprMakeVariantField|

.. das:function:: AstVisitor.preVisitExprMakeStruct(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeStruct|

.. das:function:: AstVisitor.visitExprMakeStruct(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const)

visitExprMakeStruct returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeStruct|

.. das:function:: AstVisitor.preVisitExprMakeStructIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; last: bool const)

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


|method-ast-AstVisitor.preVisitExprMakeStructIndex|

.. das:function:: AstVisitor.visitExprMakeStructIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; last: bool const)

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


|method-ast-AstVisitor.visitExprMakeStructIndex|

.. das:function:: AstVisitor.preVisitExprMakeStructField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprMakeStructField|

.. das:function:: AstVisitor.visitExprMakeStructField(self: AstVisitor; expr: smart_ptr<ast::ExprMakeStruct> const; index: int const; decl: MakeFieldDeclPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprMakeStructField|

.. das:function:: AstVisitor.preVisitExprMakeArray(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeArray|

.. das:function:: AstVisitor.visitExprMakeArray(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const)

visitExprMakeArray returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeArray|

.. das:function:: AstVisitor.preVisitExprMakeArrayIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const; index: int const; init: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprMakeArrayIndex|

.. das:function:: AstVisitor.visitExprMakeArrayIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeArray> const; index: int const; init: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprMakeArrayIndex|

.. das:function:: AstVisitor.preVisitExprMakeTuple(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeTuple|

.. das:function:: AstVisitor.visitExprMakeTuple(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const)

visitExprMakeTuple returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeTuple|

.. das:function:: AstVisitor.preVisitExprMakeTupleIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const; index: int const; init: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.preVisitExprMakeTupleIndex|

.. das:function:: AstVisitor.visitExprMakeTupleIndex(self: AstVisitor; expr: smart_ptr<ast::ExprMakeTuple> const; index: int const; init: ExpressionPtr; last: bool const)

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


|method-ast-AstVisitor.visitExprMakeTupleIndex|

.. das:function:: AstVisitor.preVisitExprArrayComprehension(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprArrayComprehension|

.. das:function:: AstVisitor.visitExprArrayComprehension(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const)

visitExprArrayComprehension returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprArrayComprehension|

.. das:function:: AstVisitor.preVisitExprArrayComprehensionSubexpr(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const; subexrp: ExpressionPtr)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+
+subexrp + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+--------+-----------------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprArrayComprehensionSubexpr|

.. das:function:: AstVisitor.preVisitExprArrayComprehensionWhere(self: AstVisitor; expr: smart_ptr<ast::ExprArrayComprehension> const; filter: ExpressionPtr)

+--------+-----------------------------------------------------------------------------------------+
+argument+argument type                                                                            +
+========+=========================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+--------+-----------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+--------+-----------------------------------------------------------------------------------------+
+filter  + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+--------+-----------------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprArrayComprehensionWhere|

.. das:function:: AstVisitor.preVisitExprTypeInfo(self: AstVisitor; expr: smart_ptr<ast::ExprTypeInfo> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprTypeInfo|

.. das:function:: AstVisitor.visitExprTypeInfo(self: AstVisitor; expr: smart_ptr<ast::ExprTypeInfo> const)

visitExprTypeInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprTypeInfo|

.. das:function:: AstVisitor.preVisitExprPtr2Ref(self: AstVisitor; expr: smart_ptr<ast::ExprPtr2Ref> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprPtr2Ref|

.. das:function:: AstVisitor.visitExprPtr2Ref(self: AstVisitor; expr: smart_ptr<ast::ExprPtr2Ref> const)

visitExprPtr2Ref returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprPtr2Ref|

.. das:function:: AstVisitor.preVisitExprLabel(self: AstVisitor; expr: smart_ptr<ast::ExprLabel> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprLabel|

.. das:function:: AstVisitor.visitExprLabel(self: AstVisitor; expr: smart_ptr<ast::ExprLabel> const)

visitExprLabel returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprLabel|

.. das:function:: AstVisitor.preVisitExprGoto(self: AstVisitor; expr: smart_ptr<ast::ExprGoto> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprGoto|

.. das:function:: AstVisitor.visitExprGoto(self: AstVisitor; expr: smart_ptr<ast::ExprGoto> const)

visitExprGoto returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprGoto|

.. das:function:: AstVisitor.preVisitExprRef2Value(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Value> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprRef2Value|

.. das:function:: AstVisitor.visitExprRef2Value(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Value> const)

visitExprRef2Value returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprRef2Value|

.. das:function:: AstVisitor.preVisitExprRef2Ptr(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Ptr> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprRef2Ptr|

.. das:function:: AstVisitor.visitExprRef2Ptr(self: AstVisitor; expr: smart_ptr<ast::ExprRef2Ptr> const)

visitExprRef2Ptr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprRef2Ptr|

.. das:function:: AstVisitor.preVisitExprAddr(self: AstVisitor; expr: smart_ptr<ast::ExprAddr> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAddr|

.. das:function:: AstVisitor.visitExprAddr(self: AstVisitor; expr: smart_ptr<ast::ExprAddr> const)

visitExprAddr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprAddr|

.. das:function:: AstVisitor.preVisitExprAssert(self: AstVisitor; expr: smart_ptr<ast::ExprAssert> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAssert|

.. das:function:: AstVisitor.visitExprAssert(self: AstVisitor; expr: smart_ptr<ast::ExprAssert> const)

visitExprAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprAssert|

.. das:function:: AstVisitor.preVisitExprStaticAssert(self: AstVisitor; expr: smart_ptr<ast::ExprStaticAssert> const)

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+--------+-----------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprStaticAssert|

.. das:function:: AstVisitor.visitExprStaticAssert(self: AstVisitor; expr: smart_ptr<ast::ExprStaticAssert> const)

visitExprStaticAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+--------+-----------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprStaticAssert|

.. das:function:: AstVisitor.preVisitExprDebug(self: AstVisitor; expr: smart_ptr<ast::ExprDebug> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprDebug|

.. das:function:: AstVisitor.visitExprDebug(self: AstVisitor; expr: smart_ptr<ast::ExprDebug> const)

visitExprDebug returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprDebug|

.. das:function:: AstVisitor.preVisitExprInvoke(self: AstVisitor; expr: smart_ptr<ast::ExprInvoke> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprInvoke|

.. das:function:: AstVisitor.visitExprInvoke(self: AstVisitor; expr: smart_ptr<ast::ExprInvoke> const)

visitExprInvoke returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprInvoke|

.. das:function:: AstVisitor.preVisitExprErase(self: AstVisitor; expr: smart_ptr<ast::ExprErase> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprErase|

.. das:function:: AstVisitor.visitExprErase(self: AstVisitor; expr: smart_ptr<ast::ExprErase> const)

visitExprErase returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprErase|

.. das:function:: AstVisitor.preVisitExprFind(self: AstVisitor; expr: smart_ptr<ast::ExprFind> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprFind|

.. das:function:: AstVisitor.visitExprFind(self: AstVisitor; expr: smart_ptr<ast::ExprFind> const)

visitExprFind returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprFind|

.. das:function:: AstVisitor.preVisitExprKeyExists(self: AstVisitor; expr: smart_ptr<ast::ExprKeyExists> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprKeyExists|

.. das:function:: AstVisitor.visitExprKeyExists(self: AstVisitor; expr: smart_ptr<ast::ExprKeyExists> const)

visitExprKeyExists returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprKeyExists|

.. das:function:: AstVisitor.preVisitExprAscend(self: AstVisitor; expr: smart_ptr<ast::ExprAscend> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAscend|

.. das:function:: AstVisitor.visitExprAscend(self: AstVisitor; expr: smart_ptr<ast::ExprAscend> const)

visitExprAscend returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprAscend|

.. das:function:: AstVisitor.preVisitExprCast(self: AstVisitor; expr: smart_ptr<ast::ExprCast> const)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCast|

.. das:function:: AstVisitor.visitExprCast(self: AstVisitor; expr: smart_ptr<ast::ExprCast> const)

visitExprCast returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+--------+-------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+--------+-------------------------------------------------------------+


|method-ast-AstVisitor.visitExprCast|

.. das:function:: AstVisitor.preVisitExprDelete(self: AstVisitor; expr: smart_ptr<ast::ExprDelete> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprDelete|

.. das:function:: AstVisitor.visitExprDelete(self: AstVisitor; expr: smart_ptr<ast::ExprDelete> const)

visitExprDelete returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprDelete|

.. das:function:: AstVisitor.preVisitExprVar(self: AstVisitor; expr: smart_ptr<ast::ExprVar> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprVar|

.. das:function:: AstVisitor.visitExprVar(self: AstVisitor; expr: smart_ptr<ast::ExprVar> const)

visitExprVar returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprVar|

.. das:function:: AstVisitor.preVisitExprField(self: AstVisitor; expr: smart_ptr<ast::ExprField> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprField|

.. das:function:: AstVisitor.visitExprField(self: AstVisitor; expr: smart_ptr<ast::ExprField> const)

visitExprField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprField|

.. das:function:: AstVisitor.preVisitExprSafeField(self: AstVisitor; expr: smart_ptr<ast::ExprSafeField> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprSafeField|

.. das:function:: AstVisitor.visitExprSafeField(self: AstVisitor; expr: smart_ptr<ast::ExprSafeField> const)

visitExprSafeField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprSafeField|

.. das:function:: AstVisitor.preVisitExprSwizzle(self: AstVisitor; expr: smart_ptr<ast::ExprSwizzle> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprSwizzle|

.. das:function:: AstVisitor.visitExprSwizzle(self: AstVisitor; expr: smart_ptr<ast::ExprSwizzle> const)

visitExprSwizzle returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprSwizzle|

.. das:function:: AstVisitor.preVisitExprIsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprIsVariant> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprIsVariant|

.. das:function:: AstVisitor.visitExprIsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprIsVariant> const)

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprIsVariant|

.. das:function:: AstVisitor.preVisitExprAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprAsVariant> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprAsVariant|

.. das:function:: AstVisitor.visitExprAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprAsVariant> const)

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprAsVariant|

.. das:function:: AstVisitor.preVisitExprSafeAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAsVariant> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprSafeAsVariant|

.. das:function:: AstVisitor.visitExprSafeAsVariant(self: AstVisitor; expr: smart_ptr<ast::ExprSafeAsVariant> const)

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprSafeAsVariant|

.. das:function:: AstVisitor.preVisitExprOp1(self: AstVisitor; expr: smart_ptr<ast::ExprOp1> const)

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprOp1|

.. das:function:: AstVisitor.visitExprOp1(self: AstVisitor; expr: smart_ptr<ast::ExprOp1> const)

visitExprOp1 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------+
+argument+argument type                                              +
+========+===========================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+--------+-----------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+--------+-----------------------------------------------------------+


|method-ast-AstVisitor.visitExprOp1|

.. das:function:: AstVisitor.preVisitExprReturn(self: AstVisitor; expr: smart_ptr<ast::ExprReturn> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprReturn|

.. das:function:: AstVisitor.visitExprReturn(self: AstVisitor; expr: smart_ptr<ast::ExprReturn> const)

visitExprReturn returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprReturn|

.. das:function:: AstVisitor.preVisitExprYield(self: AstVisitor; expr: smart_ptr<ast::ExprYield> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprYield|

.. das:function:: AstVisitor.visitExprYield(self: AstVisitor; expr: smart_ptr<ast::ExprYield> const)

visitExprYield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprYield|

.. das:function:: AstVisitor.preVisitExprBreak(self: AstVisitor; expr: smart_ptr<ast::ExprBreak> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprBreak|

.. das:function:: AstVisitor.visitExprBreak(self: AstVisitor; expr: smart_ptr<ast::ExprBreak> const)

visitExprBreak returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprBreak|

.. das:function:: AstVisitor.preVisitExprContinue(self: AstVisitor; expr: smart_ptr<ast::ExprContinue> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprContinue|

.. das:function:: AstVisitor.visitExprContinue(self: AstVisitor; expr: smart_ptr<ast::ExprContinue> const)

visitExprContinue returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprContinue|

.. das:function:: AstVisitor.preVisitExprMakeBlock(self: AstVisitor; expr: smart_ptr<ast::ExprMakeBlock> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeBlock|

.. das:function:: AstVisitor.visitExprMakeBlock(self: AstVisitor; expr: smart_ptr<ast::ExprMakeBlock> const)

visitExprMakeBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeBlock|

.. das:function:: AstVisitor.preVisitExprMakeGenerator(self: AstVisitor; expr: smart_ptr<ast::ExprMakeGenerator> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMakeGenerator|

.. das:function:: AstVisitor.visitExprMakeGenerator(self: AstVisitor; expr: smart_ptr<ast::ExprMakeGenerator> const)

visitExprMakeGenerator returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMakeGenerator|

.. das:function:: AstVisitor.preVisitExprMemZero(self: AstVisitor; expr: smart_ptr<ast::ExprMemZero> const)

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprMemZero|

.. das:function:: AstVisitor.visitExprMemZero(self: AstVisitor; expr: smart_ptr<ast::ExprMemZero> const)

visitExprMemZero returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------+
+argument+argument type                                                      +
+========+===================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+--------+-------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+--------+-------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprMemZero|

.. das:function:: AstVisitor.preVisitExprConst(self: AstVisitor; expr: smart_ptr<ast::ExprConst> const)

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConst|

.. das:function:: AstVisitor.visitExprConst(self: AstVisitor; expr: smart_ptr<ast::ExprConst> const)

visitExprConst returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------+
+argument+argument type                                                  +
+========+===============================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+--------+---------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+--------+---------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConst|

.. das:function:: AstVisitor.preVisitExprConstPtr(self: AstVisitor; expr: smart_ptr<ast::ExprConstPtr> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstPtr|

.. das:function:: AstVisitor.visitExprConstPtr(self: AstVisitor; expr: smart_ptr<ast::ExprConstPtr> const)

visitExprConstPtr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstPtr|

.. das:function:: AstVisitor.preVisitExprConstEnumeration(self: AstVisitor; expr: smart_ptr<ast::ExprConstEnumeration> const)

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+--------+-------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+--------+-------------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstEnumeration|

.. das:function:: AstVisitor.visitExprConstEnumeration(self: AstVisitor; expr: smart_ptr<ast::ExprConstEnumeration> const)

visitExprConstEnumeration returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------------+
+argument+argument type                                                                        +
+========+=====================================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+--------+-------------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+--------+-------------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstEnumeration|

.. das:function:: AstVisitor.preVisitExprConstBitfield(self: AstVisitor; expr: smart_ptr<ast::ExprConstBitfield> const)

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstBitfield|

.. das:function:: AstVisitor.visitExprConstBitfield(self: AstVisitor; expr: smart_ptr<ast::ExprConstBitfield> const)

visitExprConstBitfield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------------+
+argument+argument type                                                                  +
+========+===============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+--------+-------------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+--------+-------------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstBitfield|

.. das:function:: AstVisitor.preVisitExprConstInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt8> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt8|

.. das:function:: AstVisitor.visitExprConstInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt8> const)

visitExprConstInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt8|

.. das:function:: AstVisitor.preVisitExprConstInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt16> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt16|

.. das:function:: AstVisitor.visitExprConstInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt16> const)

visitExprConstInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt16|

.. das:function:: AstVisitor.preVisitExprConstInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt64> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt64|

.. das:function:: AstVisitor.visitExprConstInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt64> const)

visitExprConstInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt64|

.. das:function:: AstVisitor.preVisitExprConstInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt> const)

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt|

.. das:function:: AstVisitor.visitExprConstInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt> const)

visitExprConstInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------+
+argument+argument type                                                        +
+========+=====================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+--------+---------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+--------+---------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt|

.. das:function:: AstVisitor.preVisitExprConstInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt2> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt2|

.. das:function:: AstVisitor.visitExprConstInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt2> const)

visitExprConstInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt2|

.. das:function:: AstVisitor.preVisitExprConstInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt3> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt3|

.. das:function:: AstVisitor.visitExprConstInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt3> const)

visitExprConstInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt3|

.. das:function:: AstVisitor.preVisitExprConstInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt4> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstInt4|

.. das:function:: AstVisitor.visitExprConstInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstInt4> const)

visitExprConstInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstInt4|

.. das:function:: AstVisitor.preVisitExprConstUInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt8> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt8|

.. das:function:: AstVisitor.visitExprConstUInt8(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt8> const)

visitExprConstUInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt8|

.. das:function:: AstVisitor.preVisitExprConstUInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt16> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt16|

.. das:function:: AstVisitor.visitExprConstUInt16(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt16> const)

visitExprConstUInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt16|

.. das:function:: AstVisitor.preVisitExprConstUInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt64> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt64|

.. das:function:: AstVisitor.visitExprConstUInt64(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt64> const)

visitExprConstUInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt64|

.. das:function:: AstVisitor.preVisitExprConstUInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt|

.. das:function:: AstVisitor.visitExprConstUInt(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt> const)

visitExprConstUInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt|

.. das:function:: AstVisitor.preVisitExprConstUInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt2> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt2|

.. das:function:: AstVisitor.visitExprConstUInt2(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt2> const)

visitExprConstUInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt2|

.. das:function:: AstVisitor.preVisitExprConstUInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt3> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt3|

.. das:function:: AstVisitor.visitExprConstUInt3(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt3> const)

visitExprConstUInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt3|

.. das:function:: AstVisitor.preVisitExprConstUInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt4> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstUInt4|

.. das:function:: AstVisitor.visitExprConstUInt4(self: AstVisitor; expr: smart_ptr<ast::ExprConstUInt4> const)

visitExprConstUInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstUInt4|

.. das:function:: AstVisitor.preVisitExprConstRange(self: AstVisitor; expr: smart_ptr<ast::ExprConstRange> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstRange|

.. das:function:: AstVisitor.visitExprConstRange(self: AstVisitor; expr: smart_ptr<ast::ExprConstRange> const)

visitExprConstRange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstRange|

.. das:function:: AstVisitor.preVisitExprConstURange(self: AstVisitor; expr: smart_ptr<ast::ExprConstURange> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstURange|

.. das:function:: AstVisitor.visitExprConstURange(self: AstVisitor; expr: smart_ptr<ast::ExprConstURange> const)

visitExprConstURange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstURange|

.. das:function:: AstVisitor.preVisitExprConstBool(self: AstVisitor; expr: smart_ptr<ast::ExprConstBool> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstBool|

.. das:function:: AstVisitor.visitExprConstBool(self: AstVisitor; expr: smart_ptr<ast::ExprConstBool> const)

visitExprConstBool returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstBool|

.. das:function:: AstVisitor.preVisitExprConstFloat(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat> const)

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstFloat|

.. das:function:: AstVisitor.visitExprConstFloat(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat> const)

visitExprConstFloat returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------------------------------------------------------+
+argument+argument type                                                            +
+========+=========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+--------+-------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+--------+-------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstFloat|

.. das:function:: AstVisitor.preVisitExprConstFloat2(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat2> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstFloat2|

.. das:function:: AstVisitor.visitExprConstFloat2(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat2> const)

visitExprConstFloat2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstFloat2|

.. das:function:: AstVisitor.preVisitExprConstFloat3(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat3> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstFloat3|

.. das:function:: AstVisitor.visitExprConstFloat3(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat3> const)

visitExprConstFloat3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstFloat3|

.. das:function:: AstVisitor.preVisitExprConstFloat4(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat4> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstFloat4|

.. das:function:: AstVisitor.visitExprConstFloat4(self: AstVisitor; expr: smart_ptr<ast::ExprConstFloat4> const)

visitExprConstFloat4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstFloat4|

.. das:function:: AstVisitor.preVisitExprConstString(self: AstVisitor; expr: smart_ptr<ast::ExprConstString> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstString|

.. das:function:: AstVisitor.visitExprConstString(self: AstVisitor; expr: smart_ptr<ast::ExprConstString> const)

visitExprConstString returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstString|

.. das:function:: AstVisitor.preVisitExprConstDouble(self: AstVisitor; expr: smart_ptr<ast::ExprConstDouble> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprConstDouble|

.. das:function:: AstVisitor.visitExprConstDouble(self: AstVisitor; expr: smart_ptr<ast::ExprConstDouble> const)

visitExprConstDouble returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprConstDouble|

.. das:function:: AstVisitor.preVisitExprFakeContext(self: AstVisitor; expr: smart_ptr<ast::ExprFakeContext> const)

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprFakeContext|

.. das:function:: AstVisitor.visitExprFakeContext(self: AstVisitor; expr: smart_ptr<ast::ExprFakeContext> const)

visitExprFakeContext returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+---------------------------------------------------------------------------+
+argument+argument type                                                              +
+========+===========================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+--------+---------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+--------+---------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprFakeContext|

.. das:function:: AstVisitor.preVisitExprFakeLineInfo(self: AstVisitor; expr: smart_ptr<ast::ExprFakeLineInfo> const)

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+--------+-----------------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprFakeLineInfo|

.. das:function:: AstVisitor.visitExprFakeLineInfo(self: AstVisitor; expr: smart_ptr<ast::ExprFakeLineInfo> const)

visitExprFakeLineInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------------+
+argument+argument type                                                                +
+========+=============================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+--------+-----------------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+--------+-----------------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprFakeLineInfo|

.. das:function:: AstVisitor.preVisitExprReader(self: AstVisitor; expr: smart_ptr<ast::ExprReader> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprReader|

.. das:function:: AstVisitor.visitExprReader(self: AstVisitor; expr: smart_ptr<ast::ExprReader> const)

visitExprReader returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprReader|

.. das:function:: AstVisitor.preVisitExprUnsafe(self: AstVisitor; expr: smart_ptr<ast::ExprUnsafe> const)

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprUnsafe <handle-ast-ExprUnsafe>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprUnsafe|

.. das:function:: AstVisitor.visitExprUnsafe(self: AstVisitor; expr: smart_ptr<ast::ExprUnsafe> const)

visitExprUnsafe returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+--------+-----------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprUnsafe <handle-ast-ExprUnsafe>` > const+
+--------+-----------------------------------------------------------------+


|method-ast-AstVisitor.visitExprUnsafe|

.. das:function:: AstVisitor.preVisitExprCallMacro(self: AstVisitor; expr: smart_ptr<ast::ExprCallMacro> const)

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.preVisitExprCallMacro|

.. das:function:: AstVisitor.visitExprCallMacro(self: AstVisitor; expr: smart_ptr<ast::ExprCallMacro> const)

visitExprCallMacro returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-----------------------------------------------------------------------+
+argument+argument type                                                          +
+========+=======================================================================+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+--------+-----------------------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprCallMacro <handle-ast-ExprCallMacro>` > const+
+--------+-----------------------------------------------------------------------+


|method-ast-AstVisitor.visitExprCallMacro|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_ast_c__c_FunctionPtr__hh_ptr_hh__hh_handle_hh_Function_hh_const:

.. das:function:: FunctionPtr(fun: ast::Function? const)

FunctionPtr returns  :ref:`FunctionPtr <alias-FunctionPtr>`

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+fun     + :ref:`ast::Function <handle-ast-Function>` ? const+
+--------+---------------------------------------------------+


|function-ast-FunctionPtr|

.. _function-_at_ast_c__c_add_call_macro__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_CallMacro_hh_const_hh_implicit__hh_const:

.. das:function:: add_call_macro(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::CallMacro> const implicit)

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit              +
+--------+------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::CallMacro <handle-ast-CallMacro>` > const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-add_call_macro|

.. _function-_at_ast_c__c_add_dirty_infer_macro__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_PassMacro_hh_const_hh_implicit__hh_const:

.. das:function:: add_dirty_infer_macro(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::PassMacro> const implicit)

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit              +
+--------+------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::PassMacro <handle-ast-PassMacro>` > const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-add_dirty_infer_macro|

.. _function-_at_ast_c__c_add_function__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit__hh_const:

.. das:function:: add_function(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::Function> const implicit)

add_function returns bool

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit            +
+--------+----------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-ast-add_function|

.. _function-_at_ast_c__c_add_function_annotation__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_FunctionAnnotation_hh_const_hh_implicit__hh_const:

.. das:function:: add_function_annotation(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::FunctionAnnotation> const implicit)

+--------+------------------------------------------------------------------------------------------+
+argument+argument type                                                                             +
+========+==========================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                +
+--------+------------------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` > const implicit+
+--------+------------------------------------------------------------------------------------------+


|function-ast-add_function_annotation|

.. _function-_at_ast_c__c_add_function_annotation__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_FunctionAnnotation_hh_const_hh_implicit__hh_const:

.. das:function:: add_function_annotation(arg0: smart_ptr<ast::Function> const implicit; arg1: smart_ptr<ast::FunctionAnnotation> const implicit)

+--------+------------------------------------------------------------------------------------------+
+argument+argument type                                                                             +
+========+==========================================================================================+
+arg0    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit                    +
+--------+------------------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` > const implicit+
+--------+------------------------------------------------------------------------------------------+


|function-ast-add_function_annotation|

.. _function-_at_ast_c__c_add_reader_macro__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_ReaderMacro_hh_const_hh_implicit__hh_const:

.. das:function:: add_reader_macro(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::ReaderMacro> const implicit)

+--------+----------------------------------------------------------------------------+
+argument+argument type                                                               +
+========+============================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                  +
+--------+----------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::ReaderMacro <handle-ast-ReaderMacro>` > const implicit+
+--------+----------------------------------------------------------------------------+


|function-ast-add_reader_macro|

.. _function-_at_ast_c__c_add_structure_annotation__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_StructureAnnotation_hh_const_hh_implicit__hh_const:

.. das:function:: add_structure_annotation(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::StructureAnnotation> const implicit)

+--------+--------------------------------------------------------------------------------------------+
+argument+argument type                                                                               +
+========+============================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                  +
+--------+--------------------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` > const implicit+
+--------+--------------------------------------------------------------------------------------------+


|function-ast-add_structure_annotation|

.. _function-_at_ast_c__c_add_structure_annotation__hh_smart_ptr_hh__hh_handle_hh_Structure_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_StructureAnnotation_hh_const_hh_implicit__hh_const:

.. das:function:: add_structure_annotation(arg0: smart_ptr<ast::Structure> const implicit; arg1: smart_ptr<ast::StructureAnnotation> const implicit)

+--------+--------------------------------------------------------------------------------------------+
+argument+argument type                                                                               +
+========+============================================================================================+
+arg0    +smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` > const implicit                    +
+--------+--------------------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` > const implicit+
+--------+--------------------------------------------------------------------------------------------+


|function-ast-add_structure_annotation|

.. _function-_at_ast_c__c_add_variant_macro__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_VariantMacro_hh_const_hh_implicit__hh_const:

.. das:function:: add_variant_macro(arg0: rtti::Module? const implicit; arg1: smart_ptr<ast::VariantMacro> const implicit)

+--------+------------------------------------------------------------------------------+
+argument+argument type                                                                 +
+========+==============================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                    +
+--------+------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::VariantMacro <handle-ast-VariantMacro>` > const implicit+
+--------+------------------------------------------------------------------------------+


|function-ast-add_variant_macro|

.. _function-_at_ast_c__c_any_array_foreach__hh_ptr_hh_const_hh_implicit_int_hh_const__hh_block_hh__hh_ptr_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: any_array_foreach(arg0: void? const implicit; arg1: int const; arg2: block<(void?):void> const implicit)

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+arg0    +void? const implicit              +
+--------+----------------------------------+
+arg1    +int const                         +
+--------+----------------------------------+
+arg2    +block<(void?):void> const implicit+
+--------+----------------------------------+


|function-ast-any_array_foreach|

.. _function-_at_ast_c__c_any_array_size__hh_ptr_hh_const_hh_implicit:

.. das:function:: any_array_size(arg0: void? const implicit)

any_array_size returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+arg0    +void? const implicit+
+--------+--------------------+


|function-ast-any_array_size|

.. _function-_at_ast_c__c_any_table_foreach__hh_ptr_hh_const_hh_implicit_int_hh_const_int_hh_const__hh_block_hh__hh_ptr_hh__hh_ptr_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: any_table_foreach(arg0: void? const implicit; arg1: int const; arg2: int const; arg3: block<(void?;void?):void> const implicit)

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+arg0    +void? const implicit                    +
+--------+----------------------------------------+
+arg1    +int const                               +
+--------+----------------------------------------+
+arg2    +int const                               +
+--------+----------------------------------------+
+arg3    +block<(void?;void?):void> const implicit+
+--------+----------------------------------------+


|function-ast-any_table_foreach|

.. _function-_at_ast_c__c_any_table_size__hh_ptr_hh_const_hh_implicit:

.. das:function:: any_table_size(arg0: void? const implicit)

any_table_size returns int

+--------+--------------------+
+argument+argument type       +
+========+====================+
+arg0    +void? const implicit+
+--------+--------------------+


|function-ast-any_table_size|

.. _function-_at_ast_c__c_clone_expression__hh_smart_ptr_hh__hh_handle_hh_Expression_hh_const_hh_implicit:

.. das:function:: clone_expression(arg0: smart_ptr<ast::Expression> const implicit)

clone_expression returns smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >

+--------+--------------------------------------------------------------------------+
+argument+argument type                                                             +
+========+==========================================================================+
+arg0    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const implicit+
+--------+--------------------------------------------------------------------------+


|function-ast-clone_expression|

.. _function-_at_ast_c__c_clone_function__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit:

.. das:function:: clone_function(arg0: smart_ptr<ast::Function> const implicit)

clone_function returns smart_ptr< :ref:`ast::Function <handle-ast-Function>` >

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-ast-clone_function|

.. _function-_at_ast_c__c_clone_type__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit:

.. das:function:: clone_type(arg0: smart_ptr<ast::TypeDecl> const implicit)

clone_type returns smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-ast-clone_type|

.. _function-_at_ast_c__c_compiling_module__hh_const:

.. das:function:: compiling_module()

compiling_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

|function-ast-compiling_module|

.. _function-_at_ast_c__c_compiling_program__hh_const:

.. das:function:: compiling_program()

compiling_program returns smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >

|function-ast-compiling_program|

.. _function-_at_ast_c__c_das_to_string__hh_enum_hh_rtti_c__c_Type_hh_int_hh_const__hh_const:

.. das:function:: das_to_string(arg0: Type const)

das_to_string returns string

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+arg0    + :ref:`rtti::Type <enum-rtti-Type>`  const+
+--------+------------------------------------------+


|function-ast-das_to_string|

.. _function-_at_ast_c__c_describe_expression__hh_smart_ptr_hh__hh_handle_hh_Expression_hh_const_hh_implicit__hh_const:

.. das:function:: describe_expression(arg0: smart_ptr<ast::Expression> const implicit)

describe_expression returns string

+--------+--------------------------------------------------------------------------+
+argument+argument type                                                             +
+========+==========================================================================+
+arg0    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const implicit+
+--------+--------------------------------------------------------------------------+


|function-ast-describe_expression|

.. _function-_at_ast_c__c_describe_function__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit__hh_const:

.. das:function:: describe_function(arg0: smart_ptr<ast::Function> const implicit)

describe_function returns string

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-ast-describe_function|

.. _function-_at_ast_c__c_describe_typedecl__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit_bool_hh_const_bool_hh_const_bool_hh_const__hh_const:

.. das:function:: describe_typedecl(arg0: smart_ptr<ast::TypeDecl> const implicit; arg1: bool const; arg2: bool const; arg3: bool const)

describe_typedecl returns string

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    +bool const                                                            +
+--------+----------------------------------------------------------------------+
+arg2    +bool const                                                            +
+--------+----------------------------------------------------------------------+
+arg3    +bool const                                                            +
+--------+----------------------------------------------------------------------+


|function-ast-describe_typedecl|

.. _function-_at_ast_c__c_find_bitfield_name__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit__hh_bitfield_hh_const__hh_const:

.. das:function:: find_bitfield_name(arg0: smart_ptr<ast::TypeDecl> const implicit; arg1: bitfield const)

find_bitfield_name returns string

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    +bitfield<> const                                                      +
+--------+----------------------------------------------------------------------+


|function-ast-find_bitfield_name|

.. _function-_at_ast_c__c_find_module_string_hh_const:

.. das:function:: find_module(name: string const)

find_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

+--------+-------------+
+argument+argument type+
+========+=============+
+name    +string const +
+--------+-------------+


|function-ast-find_module|

.. _function-_at_ast_c__c_for_each_enumeration__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Enumeration_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_enumeration(arg0: rtti::Module? const implicit; arg1: block<(smart_ptr<ast::Enumeration>):void> const implicit)

+--------+------------------------------------------------------------------------------------------+
+argument+argument type                                                                             +
+========+==========================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                +
+--------+------------------------------------------------------------------------------------------+
+arg1    +block<(smart_ptr< :ref:`ast::Enumeration <handle-ast-Enumeration>` >):void> const implicit+
+--------+------------------------------------------------------------------------------------------+


|function-ast-for_each_enumeration|

.. _function-_at_ast_c__c_for_each_field__hh_handle_hh_BasicStructureAnnotation_hh_const_hh_implicit__hh_block_hh_string_hh_string_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_uint_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_field(arg0: BasicStructureAnnotation const implicit; arg1: block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit)

+--------+-------------------------------------------------------------------------------------------------------+
+argument+argument type                                                                                          +
+========+=======================================================================================================+
+arg0    + :ref:`rtti::BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  const implicit          +
+--------+-------------------------------------------------------------------------------------------------------+
+arg1    +block<(string;string;smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >;uint):void> const implicit+
+--------+-------------------------------------------------------------------------------------------------------+


|function-ast-for_each_field|

.. _function-_at_ast_c__c_for_each_function__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Function_hh__c_void_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: for_each_function(arg0: rtti::Module? const implicit; arg1: string const implicit; arg2: block<(smart_ptr<ast::Function>):void> const implicit)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+--------+------------------------------------------------------------------------------------+
+arg1    +string const implicit                                                               +
+--------+------------------------------------------------------------------------------------+
+arg2    +block<(smart_ptr< :ref:`ast::Function <handle-ast-Function>` >):void> const implicit+
+--------+------------------------------------------------------------------------------------+


|function-ast-for_each_function|

.. _function-_at_ast_c__c_for_each_generic__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Function_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_generic(arg0: rtti::Module? const implicit; arg1: block<(smart_ptr<ast::Function>):void> const implicit)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+--------+------------------------------------------------------------------------------------+
+arg1    +block<(smart_ptr< :ref:`ast::Function <handle-ast-Function>` >):void> const implicit+
+--------+------------------------------------------------------------------------------------+


|function-ast-for_each_generic|

.. _function-_at_ast_c__c_for_each_generic__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit_string_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Function_hh__c_void_hh_const_hh_implicit__hh_const__hh_const:

.. das:function:: for_each_generic(arg0: rtti::Module? const implicit; arg1: string const implicit; arg2: block<(smart_ptr<ast::Function>):void> const implicit)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+--------+------------------------------------------------------------------------------------+
+arg1    +string const implicit                                                               +
+--------+------------------------------------------------------------------------------------+
+arg2    +block<(smart_ptr< :ref:`ast::Function <handle-ast-Function>` >):void> const implicit+
+--------+------------------------------------------------------------------------------------+


|function-ast-for_each_generic|

.. _function-_at_ast_c__c_for_each_global__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Variable_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_global(arg0: rtti::Module? const implicit; arg1: block<(smart_ptr<ast::Variable>):void> const implicit)

+--------+------------------------------------------------------------------------------------+
+argument+argument type                                                                       +
+========+====================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+--------+------------------------------------------------------------------------------------+
+arg1    +block<(smart_ptr< :ref:`ast::Variable <handle-ast-Variable>` >):void> const implicit+
+--------+------------------------------------------------------------------------------------+


|function-ast-for_each_global|

.. _function-_at_ast_c__c_for_each_structure__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh__hh_smart_ptr_hh__hh_handle_hh_Structure_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_structure(arg0: rtti::Module? const implicit; arg1: block<(smart_ptr<ast::Structure>):void> const implicit)

+--------+--------------------------------------------------------------------------------------+
+argument+argument type                                                                         +
+========+======================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                            +
+--------+--------------------------------------------------------------------------------------+
+arg1    +block<(smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` >):void> const implicit+
+--------+--------------------------------------------------------------------------------------+


|function-ast-for_each_structure|

.. _function-_at_ast_c__c_for_each_typedef__hh_ptr_hh__hh_handle_hh_Module_hh_const_hh_implicit__hh_block_hh_string_hh_temporary_hh__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh__c_void_hh_const_hh_implicit__hh_const:

.. das:function:: for_each_typedef(arg0: rtti::Module? const implicit; arg1: block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit)

+--------+--------------------------------------------------------------------------------------------+
+argument+argument type                                                                               +
+========+============================================================================================+
+arg0    + :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                  +
+--------+--------------------------------------------------------------------------------------------+
+arg1    +block<(string#;smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >):void> const implicit+
+--------+--------------------------------------------------------------------------------------------+


|function-ast-for_each_typedef|

.. _function-_at_ast_c__c_get_mangled_name__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit__hh_const:

.. das:function:: get_mangled_name(arg0: smart_ptr<ast::Function> const implicit)

get_mangled_name returns string

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+--------+----------------------------------------------------------------------+


|function-ast-get_mangled_name|

.. _function-_at_ast_c__c_get_tuple_field_offset__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit_int_hh_const:

.. das:function:: get_tuple_field_offset(arg0: smart_ptr<ast::TypeDecl> const implicit; arg1: int const)

get_tuple_field_offset returns int

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    +int const                                                             +
+--------+----------------------------------------------------------------------+


|function-ast-get_tuple_field_offset|

.. _function-_at_ast_c__c_get_variant_field_offset__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit_int_hh_const:

.. das:function:: get_variant_field_offset(arg0: smart_ptr<ast::TypeDecl> const implicit; arg1: int const)

get_variant_field_offset returns int

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    +int const                                                             +
+--------+----------------------------------------------------------------------+


|function-ast-get_variant_field_offset|

.. _function-_at_ast_c__c_is_same_type__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_hh_implicit__hh_enum_hh_rtti_c__c_RefMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_ConstMatters_hh_int_hh_const__hh_enum_hh_rtti_c__c_TemporaryMatters_hh_int_hh_const:

.. das:function:: is_same_type(arg0: smart_ptr<ast::TypeDecl> const implicit; arg1: smart_ptr<ast::TypeDecl> const implicit; arg2: RefMatters const; arg3: ConstMatters const; arg4: TemporaryMatters const)

is_same_type returns bool

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg2    + :ref:`rtti::RefMatters <enum-rtti-RefMatters>`  const                +
+--------+----------------------------------------------------------------------+
+arg3    + :ref:`rtti::ConstMatters <enum-rtti-ConstMatters>`  const            +
+--------+----------------------------------------------------------------------+
+arg4    + :ref:`rtti::TemporaryMatters <enum-rtti-TemporaryMatters>`  const    +
+--------+----------------------------------------------------------------------+


|function-ast-is_same_type|

.. _function-_at_ast_c__c_macro_error__hh_smart_ptr_hh__hh_handle_hh_Program_hh_const_hh_implicit__hh_handle_hh_LineInfo_hh_const_hh_implicit_string_hh_const_hh_implicit:

.. das:function:: macro_error(arg0: smart_ptr<rtti::Program> const implicit; arg1: LineInfo const implicit; arg2: string const implicit)

+--------+----------------------------------------------------------------------+
+argument+argument type                                                         +
+========+======================================================================+
+arg0    +smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit+
+--------+----------------------------------------------------------------------+
+arg1    + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const implicit         +
+--------+----------------------------------------------------------------------+
+arg2    +string const implicit                                                 +
+--------+----------------------------------------------------------------------+


|function-ast-macro_error|

.. _function-_at_ast_c__c_make_call_macro_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_call_macro(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_call_macro returns smart_ptr< :ref:`ast::CallMacro <handle-ast-CallMacro>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_call_macro|

.. _function-_at_ast_c__c_make_function_annotation_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_function_annotation(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_function_annotation returns smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_function_annotation|

.. _function-_at_ast_c__c_make_pass_macro_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_pass_macro(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_pass_macro returns smart_ptr< :ref:`ast::PassMacro <handle-ast-PassMacro>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_pass_macro|

.. _function-_at_ast_c__c_make_reader_macro_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_reader_macro(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_reader_macro returns smart_ptr< :ref:`ast::ReaderMacro <handle-ast-ReaderMacro>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_reader_macro|

.. _function-_at_ast_c__c_make_structure_annotation_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_structure_annotation(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_structure_annotation returns smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_structure_annotation|

.. _function-_at_ast_c__c_make_variant_macro_string_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_variant_macro(arg0: string const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

make_variant_macro returns smart_ptr< :ref:`ast::VariantMacro <handle-ast-VariantMacro>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +string const implicit                                                   +
+--------+------------------------------------------------------------------------+
+arg1    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_variant_macro|

.. _function-_at_ast_c__c_make_visitor__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_visitor(arg0: void? const implicit; arg1: rtti::StructInfo const? const implicit)

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` >

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg1    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-ast-make_visitor|

.. _function-_at_ast_c__c_this_module__hh_const__hh_const:

.. das:function:: this_module()

this_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

|function-ast-this_module|

.. _function-_at_ast_c__c_this_program__hh_const:

.. das:function:: this_program()

this_program returns smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >

|function-ast-this_program|

.. _function-_at_ast_c__c_visit__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_VisitorAdapter_hh_const_hh_implicit:

.. das:function:: visit(arg0: smart_ptr<ast::Function> const implicit; arg1: smart_ptr<ast::VisitorAdapter> const implicit)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+arg0    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit            +
+--------+----------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` > const implicit+
+--------+----------------------------------------------------------------------------------+


|function-ast-visit|

.. _function-_at_ast_c__c_visit__hh_smart_ptr_hh__hh_handle_hh_Program_hh_const_hh_implicit__hh_smart_ptr_hh__hh_handle_hh_VisitorAdapter_hh_const_hh_implicit:

.. das:function:: visit(arg0: smart_ptr<rtti::Program> const implicit; arg1: smart_ptr<ast::VisitorAdapter> const implicit)

+--------+----------------------------------------------------------------------------------+
+argument+argument type                                                                     +
+========+==================================================================================+
+arg0    +smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit            +
+--------+----------------------------------------------------------------------------------+
+arg1    +smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` > const implicit+
+--------+----------------------------------------------------------------------------------+


|function-ast-visit|

.. _function-_at_ast_c__c_ExpressionPtr__hh_smart_ptr_hh__hh_auto_hh_const:

.. das:function:: ExpressionPtr(expr: smart_ptr<auto(TT)> const)

ExpressionPtr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>`

+--------+-------------------------+
+argument+argument type            +
+========+=========================+
+expr    +smart_ptr<auto(TT)> const+
+--------+-------------------------+


|function-ast-ExpressionPtr|

.. _function-_at_ast_c__c_add_new_call_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: add_new_call_macro(name: string const; someClassPtr: auto const)

add_new_call_macro returns auto

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-add_new_call_macro|

.. _function-_at_ast_c__c_add_new_function_annotation_string_hh_const__hh_auto_hh_const:

.. das:function:: add_new_function_annotation(name: string const; someClassPtr: auto const)

add_new_function_annotation returns auto

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-add_new_function_annotation|

.. _function-_at_ast_c__c_add_new_reader_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: add_new_reader_macro(name: string const; someClassPtr: auto const)

add_new_reader_macro returns auto

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-add_new_reader_macro|

.. _function-_at_ast_c__c_add_new_structure_annotation_string_hh_const__hh_auto_hh_const:

.. das:function:: add_new_structure_annotation(name: string const; someClassPtr: auto const)

add_new_structure_annotation returns auto

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-add_new_structure_annotation|

.. _function-_at_ast_c__c_add_new_variant_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: add_new_variant_macro(name: string const; someClassPtr: auto const)

add_new_variant_macro returns auto

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-add_new_variant_macro|

.. _function-_at_ast_c__c_describe__hh_smart_ptr_hh__hh_handle_hh_Expression_hh_const:

.. das:function:: describe(expr: smart_ptr<ast::Expression> const)

describe returns auto

+--------+-----------------------------------------------------------------+
+argument+argument type                                                    +
+========+=================================================================+
+expr    +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const+
+--------+-----------------------------------------------------------------+


|function-ast-describe|

.. _function-_at_ast_c__c_describe__hh_smart_ptr_hh__hh_handle_hh_Function_hh_const:

.. das:function:: describe(expr: smart_ptr<ast::Function> const)

describe returns auto

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+expr    +smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const+
+--------+-------------------------------------------------------------+


|function-ast-describe|

.. _function-_at_ast_c__c_describe__hh_smart_ptr_hh__hh_handle_hh_TypeDecl_hh_const_bool_hh_const_bool_hh_const_bool_hh_const:

.. das:function:: describe(decl: smart_ptr<ast::TypeDecl> const; extra: bool const; contracts: bool const; modules: bool const)

describe returns auto

+---------+-------------------------------------------------------------+
+argument +argument type                                                +
+=========+=============================================================+
+decl     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const+
+---------+-------------------------------------------------------------+
+extra    +bool const                                                   +
+---------+-------------------------------------------------------------+
+contracts+bool const                                                   +
+---------+-------------------------------------------------------------+
+modules  +bool const                                                   +
+---------+-------------------------------------------------------------+


|function-ast-describe|

.. _function-_at_ast_c__c_make_call_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: make_call_macro(name: string const; someClassPtr: auto const)

make_call_macro returns  :ref:`CallMacroPtr <alias-CallMacroPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_call_macro|

.. _function-_at_ast_c__c_make_function_annotation_string_hh_const__hh_auto_hh_const:

.. das:function:: make_function_annotation(name: string const; someClassPtr: auto const)

make_function_annotation returns  :ref:`FunctionAnnotationPtr <alias-FunctionAnnotationPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_function_annotation|

.. _function-_at_ast_c__c_make_pass_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: make_pass_macro(name: string const; someClassPtr: auto const)

make_pass_macro returns  :ref:`PassMacroPtr <alias-PassMacroPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_pass_macro|

.. _function-_at_ast_c__c_make_reader_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: make_reader_macro(name: string const; someClassPtr: auto const)

make_reader_macro returns  :ref:`ReaderMacroPtr <alias-ReaderMacroPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_reader_macro|

.. _function-_at_ast_c__c_make_structure_annotation_string_hh_const__hh_auto_hh_const:

.. das:function:: make_structure_annotation(name: string const; someClassPtr: auto const)

make_structure_annotation returns  :ref:`StructureAnnotationPtr <alias-StructureAnnotationPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_structure_annotation|

.. _function-_at_ast_c__c_make_variant_macro_string_hh_const__hh_auto_hh_const:

.. das:function:: make_variant_macro(name: string const; someClassPtr: auto const)

make_variant_macro returns  :ref:`VariantMacroPtr <alias-VariantMacroPtr>`

+------------+-------------+
+argument    +argument type+
+============+=============+
+name        +string const +
+------------+-------------+
+someClassPtr+auto const   +
+------------+-------------+


|function-ast-make_variant_macro|

.. _function-_at_ast_c__c_make_visitor__hh_auto_hh_const:

.. das:function:: make_visitor(someClass: auto const)

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` >

+---------+-------------+
+argument +argument type+
+=========+=============+
+someClass+auto const   +
+---------+-------------+


|function-ast-make_visitor|


