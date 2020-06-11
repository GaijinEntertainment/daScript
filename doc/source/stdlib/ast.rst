
.. _stdlib_ast:

==============================================
Compile time type  and AST information library
==============================================

+++++++++++
TypeAliases
+++++++++++

.. _alias-EnumerationPtr:

.. das:attribute:: EnumerationPtr = EnumerationPtr

typedef|ast|EnumerationPtr to be documented

.. _alias-ExprAscendFlags:

.. das:attribute:: ExprAscendFlags is a bitfield

+------------+-+-+
+useStackRef +0+1+
+------------+-+-+
+needTypeInfo+1+2+
+------------+-+-+


typedef|ast|ExprAscendFlags to be documented

.. _alias-ExprAtFlags:

.. das:attribute:: ExprAtFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


typedef|ast|ExprAtFlags to be documented

.. _alias-ExprBlockFlags:

.. das:attribute:: ExprBlockFlags is a bitfield

+--------------------------+--+----+
+isClosure                 +0 +1   +
+--------------------------+--+----+
+hasReturn                 +1 +2   +
+--------------------------+--+----+
+copyOnReturn              +2 +4   +
+--------------------------+--+----+
+moveOnReturn              +3 +8   +
+--------------------------+--+----+
+inTheLoop                 +4 +16  +
+--------------------------+--+----+
+finallyBeforeBody         +5 +32  +
+--------------------------+--+----+
+finallyDisabled           +6 +64  +
+--------------------------+--+----+
+aotSkipMakeBlock          +7 +128 +
+--------------------------+--+----+
+aotDoNotSkipAnnotationData+8 +256 +
+--------------------------+--+----+
+isCollapseable            +9 +512 +
+--------------------------+--+----+
+needCollapse              +10+1024+
+--------------------------+--+----+


typedef|ast|ExprBlockFlags to be documented

.. _alias-ExprCastFlags:

.. das:attribute:: ExprCastFlags is a bitfield

+---------------+-+-+
+upcastCast     +0+1+
+---------------+-+-+
+reinterpretCast+1+2+
+---------------+-+-+


typedef|ast|ExprCastFlags to be documented

.. _alias-ExprFieldDerefFlags:

.. das:attribute:: ExprFieldDerefFlags is a bitfield

+------------------+-+-+
+unsafeDeref       +0+1+
+------------------+-+-+
+ignoreCaptureConst+1+2+
+------------------+-+-+


typedef|ast|ExprFieldDerefFlags to be documented

.. _alias-ExprFieldFieldFlags:

.. das:attribute:: ExprFieldFieldFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


typedef|ast|ExprFieldFieldFlags to be documented

.. _alias-ExprFlags:

.. das:attribute:: ExprFlags is a bitfield

+-------------------+-+-+
+constexpression    +0+1+
+-------------------+-+-+
+noSideEffects      +1+2+
+-------------------+-+-+
+noNativeSideEffects+2+4+
+-------------------+-+-+


typedef|ast|ExprFlags to be documented

.. _alias-ExprGenFlags:

.. das:attribute:: ExprGenFlags is a bitfield

+----------+-+-+
+alwaysSafe+0+1+
+----------+-+-+
+generated +1+2+
+----------+-+-+


typedef|ast|ExprGenFlags to be documented

.. _alias-ExprMakeBlockFlags:

.. das:attribute:: ExprMakeBlockFlags is a bitfield

+---------------+-+-+
+isLambda       +0+1+
+---------------+-+-+
+isLocalFunction+1+2+
+---------------+-+-+


typedef|ast|ExprMakeBlockFlags to be documented

.. _alias-ExprMakeLocalFlags:

.. das:attribute:: ExprMakeLocalFlags is a bitfield

+---------------+-+--+
+useStackRef    +0+1 +
+---------------+-+--+
+useCMRES       +1+2 +
+---------------+-+--+
+doesNotNeedSp  +2+4 +
+---------------+-+--+
+doesNotNeedInit+3+8 +
+---------------+-+--+
+initAllFields  +4+16+
+---------------+-+--+


typedef|ast|ExprMakeLocalFlags to be documented

.. _alias-ExprMakeStructFlags:

.. das:attribute:: ExprMakeStructFlags is a bitfield

+--------------+-+-+
+useInitializer+0+1+
+--------------+-+-+
+isNewHandle   +1+2+
+--------------+-+-+


typedef|ast|ExprMakeStructFlags to be documented

.. _alias-ExprPrintFlags:

.. das:attribute:: ExprPrintFlags is a bitfield

+-----------+-+-+
+topLevel   +0+1+
+-----------+-+-+
+argLevel   +1+2+
+-----------+-+-+
+bottomLevel+2+4+
+-----------+-+-+


typedef|ast|ExprPrintFlags to be documented

.. _alias-ExprReturnFlags:

.. das:attribute:: ExprReturnFlags is a bitfield

+------------------+-+--+
+moveSemantics     +0+1 +
+------------------+-+--+
+returnReference   +1+2 +
+------------------+-+--+
+returnInBlock     +2+4 +
+------------------+-+--+
+takeOverRightStack+3+8 +
+------------------+-+--+
+returnCallCMRES   +4+16+
+------------------+-+--+
+returnCMRES       +5+32+
+------------------+-+--+
+fromYield         +6+64+
+------------------+-+--+


typedef|ast|ExprReturnFlags to be documented

.. _alias-ExprSwizzleFieldFlags:

.. das:attribute:: ExprSwizzleFieldFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


typedef|ast|ExprSwizzleFieldFlags to be documented

.. _alias-ExprVarFlags:

.. das:attribute:: ExprVarFlags is a bitfield

+---------+-+--+
+local    +0+1 +
+---------+-+--+
+argument +1+2 +
+---------+-+--+
+block    +2+4 +
+---------+-+--+
+thisBlock+3+8 +
+---------+-+--+
+r2v      +4+16+
+---------+-+--+
+r2cr     +5+32+
+---------+-+--+
+write    +6+64+
+---------+-+--+


typedef|ast|ExprVarFlags to be documented

.. _alias-ExprYieldFlags:

.. das:attribute:: ExprYieldFlags is a bitfield

+-------------+-+-+
+moveSemantics+0+1+
+-------------+-+-+


typedef|ast|ExprYieldFlags to be documented

.. _alias-ExpressionPtr:

.. das:attribute:: ExpressionPtr = ExpressionPtr

typedef|ast|ExpressionPtr to be documented

.. _alias-FieldDeclarationFlags:

.. das:attribute:: FieldDeclarationFlags is a bitfield

+----------------+-+-+
+moveSemantics   +0+1+
+----------------+-+-+
+parentType      +1+2+
+----------------+-+-+
+capturedConstant+2+4+
+----------------+-+-+
+generated       +3+8+
+----------------+-+-+


typedef|ast|FieldDeclarationFlags to be documented

.. _alias-FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = FunctionAnnotationPtr

typedef|ast|FunctionAnnotationPtr to be documented

.. _alias-FunctionFlags:

.. das:attribute:: FunctionFlags is a bitfield

+---------------------+--+---------+
+builtIn              +0 +1        +
+---------------------+--+---------+
+policyBased          +1 +2        +
+---------------------+--+---------+
+callBased            +2 +4        +
+---------------------+--+---------+
+interopFn            +3 +8        +
+---------------------+--+---------+
+hasReturn            +4 +16       +
+---------------------+--+---------+
+copyOnReturn         +5 +32       +
+---------------------+--+---------+
+moveOnReturn         +6 +64       +
+---------------------+--+---------+
+exports              +7 +128      +
+---------------------+--+---------+
+init                 +8 +256      +
+---------------------+--+---------+
+addr                 +9 +512      +
+---------------------+--+---------+
+used                 +10+1024     +
+---------------------+--+---------+
+fastCall             +11+2048     +
+---------------------+--+---------+
+knownSideEffects     +12+4096     +
+---------------------+--+---------+
+hasToRunAtCompileTime+13+8192     +
+---------------------+--+---------+
+unsafe               +14+16384    +
+---------------------+--+---------+
+unsafeOperation      +15+32768    +
+---------------------+--+---------+
+unsafeDeref          +16+65536    +
+---------------------+--+---------+
+hasMakeBlock         +17+131072   +
+---------------------+--+---------+
+aotNeedPrologue      +18+262144   +
+---------------------+--+---------+
+noAot                +19+524288   +
+---------------------+--+---------+
+aotHybrid            +20+1048576  +
+---------------------+--+---------+
+aotTemplate          +21+2097152  +
+---------------------+--+---------+
+generated            +22+4194304  +
+---------------------+--+---------+
+privateFunction      +23+8388608  +
+---------------------+--+---------+
+_generator           +24+16777216 +
+---------------------+--+---------+
+_lambda              +25+33554432 +
+---------------------+--+---------+
+firstArgReturnType   +26+67108864 +
+---------------------+--+---------+
+isClassMethod        +27+134217728+
+---------------------+--+---------+


typedef|ast|FunctionFlags to be documented

.. _alias-FunctionPtr:

.. das:attribute:: FunctionPtr = FunctionPtr

typedef|ast|FunctionPtr to be documented

.. _alias-FunctionSideEffectFlags:

.. das:attribute:: FunctionSideEffectFlags is a bitfield

+--------------+-+--+
+unsafe        +0+1 +
+--------------+-+--+
+userScenario  +1+2 +
+--------------+-+--+
+modifyExternal+2+4 +
+--------------+-+--+
+modifyArgument+3+8 +
+--------------+-+--+
+accessGlobal  +4+16+
+--------------+-+--+
+invoke        +5+32+
+--------------+-+--+


typedef|ast|FunctionSideEffectFlags to be documented

.. _alias-MakeFieldDeclFlags:

.. das:attribute:: MakeFieldDeclFlags is a bitfield

+--------------+-+-+
+moveSemantics +0+1+
+--------------+-+-+
+cloneSemantics+1+2+
+--------------+-+-+


typedef|ast|MakeFieldDeclFlags to be documented

.. _alias-MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = MakeFieldDeclPtr

typedef|ast|MakeFieldDeclPtr to be documented

.. _alias-PassMacroPtr:

.. das:attribute:: PassMacroPtr = PassMacroPtr

typedef|ast|PassMacroPtr to be documented

.. _alias-ProgramPtr:

.. das:attribute:: ProgramPtr = ProgramPtr

typedef|ast|ProgramPtr to be documented

.. _alias-ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = ReaderMacroPtr

typedef|ast|ReaderMacroPtr to be documented

.. _alias-StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = StructureAnnotationPtr

typedef|ast|StructureAnnotationPtr to be documented

.. _alias-StructureFlags:

.. das:attribute:: StructureFlags is a bitfield

+---------------+-+--+
+isClass        +0+1 +
+---------------+-+--+
+genCtor        +1+2 +
+---------------+-+--+
+cppLayout      +2+4 +
+---------------+-+--+
+cppLayoutNotPod+3+8 +
+---------------+-+--+
+generated      +4+16+
+---------------+-+--+
+persistent     +5+32+
+---------------+-+--+
+isLambda       +6+64+
+---------------+-+--+


typedef|ast|StructureFlags to be documented

.. _alias-StructurePtr:

.. das:attribute:: StructurePtr = StructurePtr

typedef|ast|StructurePtr to be documented

.. _alias-TypeDeclFlags:

.. das:attribute:: TypeDeclFlags is a bitfield

+---------------+--+----+
+ref            +0 +1   +
+---------------+--+----+
+constant       +1 +2   +
+---------------+--+----+
+temporary      +2 +4   +
+---------------+--+----+
+_implicit      +3 +8   +
+---------------+--+----+
+removeRef      +4 +16  +
+---------------+--+----+
+removeConstant +5 +32  +
+---------------+--+----+
+removeDim      +6 +64  +
+---------------+--+----+
+removeTemporary+7 +128 +
+---------------+--+----+
+explicitConst  +8 +256 +
+---------------+--+----+
+aotAlias       +9 +512 +
+---------------+--+----+
+smartPtr       +10+1024+
+---------------+--+----+


typedef|ast|TypeDeclFlags to be documented

.. _alias-TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = TypeDeclPtr

typedef|ast|TypeDeclPtr to be documented

.. _alias-VariableAccessFlags:

.. das:attribute:: VariableAccessFlags is a bitfield

+-------------+-+--+
+access_extern+0+1 +
+-------------+-+--+
+access_get   +1+2 +
+-------------+-+--+
+access_ref   +2+4 +
+-------------+-+--+
+access_init  +3+8 +
+-------------+-+--+
+access_pass  +4+16+
+-------------+-+--+


typedef|ast|VariableAccessFlags to be documented

.. _alias-VariableFlags:

.. das:attribute:: VariableFlags is a bitfield

+--------------+-+---+
+init_via_move +0+1  +
+--------------+-+---+
+init_via_clone+1+2  +
+--------------+-+---+
+used          +2+4  +
+--------------+-+---+
+aliasCMRES    +3+8  +
+--------------+-+---+
+marked_used   +4+16 +
+--------------+-+---+
+global_shared +5+32 +
+--------------+-+---+
+do_not_delete +6+64 +
+--------------+-+---+
+generated     +7+128+
+--------------+-+---+


typedef|ast|VariableFlags to be documented

.. _alias-VariablePtr:

.. das:attribute:: VariablePtr = VariablePtr

typedef|ast|VariablePtr to be documented

.. _alias-VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = VariantMacroPtr

typedef|ast|VariantMacroPtr to be documented

++++++++++++
Enumerations
++++++++++++

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
+inferedSideEffects       +56+
+-------------------------+--+


enumeration|ast|SideEffects to be documented

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-ast-Enumeration:

.. das:attribute:: Enumeration

Enumeration fields are

+--------+------------------------------------------------------------------------------------------------------------------------+
+_module + :ref:`rtti::Module <handle-rtti-Module>` ?                                                                             +
+--------+------------------------------------------------------------------------------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                                                           +
+--------+------------------------------------------------------------------------------------------------------------------------+
+cppName + :ref:`builtin::das_string <handle-builtin-das_string>`                                                                 +
+--------+------------------------------------------------------------------------------------------------------------------------+
+list    + :ref:`ast::dasvector`pair`das_string`smart_ptr`Expression <handle-ast-dasvector`pair`das_string`smart_ptr`Expression>` +
+--------+------------------------------------------------------------------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle-builtin-das_string>`                                                                 +
+--------+------------------------------------------------------------------------------------------------------------------------+
+external+bool                                                                                                                    +
+--------+------------------------------------------------------------------------------------------------------------------------+
+baseType+ :ref:`rtti::Type <enum-rtti-Type>`                                                                                     +
+--------+------------------------------------------------------------------------------------------------------------------------+


structure_annotation|ast|Enumeration to be documented

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


structure_annotation|ast|ExprAddr to be documented

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


structure_annotation|ast|ExprArrayComprehension to be documented

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


structure_annotation|ast|ExprAsVariant to be documented

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


structure_annotation|ast|ExprAscend to be documented

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


structure_annotation|ast|ExprAssert to be documented

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


structure_annotation|ast|ExprAt to be documented

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


structure_annotation|ast|ExprBlock to be documented

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


structure_annotation|ast|ExprBreak to be documented

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


structure_annotation|ast|ExprCall to be documented

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


structure_annotation|ast|ExprCast to be documented

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


structure_annotation|ast|ExprClone to be documented

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


structure_annotation|ast|ExprConst to be documented

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


structure_annotation|ast|ExprConstBitfield to be documented

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


structure_annotation|ast|ExprConstBool to be documented

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


structure_annotation|ast|ExprConstDouble to be documented

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


structure_annotation|ast|ExprConstEnumeration to be documented

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


structure_annotation|ast|ExprConstFloat to be documented

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


structure_annotation|ast|ExprConstFloat2 to be documented

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


structure_annotation|ast|ExprConstFloat3 to be documented

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


structure_annotation|ast|ExprConstFloat4 to be documented

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


structure_annotation|ast|ExprConstInt to be documented

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


structure_annotation|ast|ExprConstInt16 to be documented

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


structure_annotation|ast|ExprConstInt2 to be documented

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


structure_annotation|ast|ExprConstInt3 to be documented

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


structure_annotation|ast|ExprConstInt4 to be documented

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


structure_annotation|ast|ExprConstInt64 to be documented

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


structure_annotation|ast|ExprConstInt8 to be documented

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


structure_annotation|ast|ExprConstPtr to be documented

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


structure_annotation|ast|ExprConstRange to be documented

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


structure_annotation|ast|ExprConstString to be documented

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


structure_annotation|ast|ExprConstUInt to be documented

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


structure_annotation|ast|ExprConstUInt16 to be documented

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


structure_annotation|ast|ExprConstUInt2 to be documented

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


structure_annotation|ast|ExprConstUInt3 to be documented

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


structure_annotation|ast|ExprConstUInt4 to be documented

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


structure_annotation|ast|ExprConstUInt64 to be documented

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


structure_annotation|ast|ExprConstUInt8 to be documented

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


structure_annotation|ast|ExprConstURange to be documented

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


structure_annotation|ast|ExprContinue to be documented

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


structure_annotation|ast|ExprCopy to be documented

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


structure_annotation|ast|ExprDebug to be documented

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


structure_annotation|ast|ExprDelete to be documented

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


structure_annotation|ast|ExprErase to be documented

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


structure_annotation|ast|ExprFakeContext to be documented

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


structure_annotation|ast|ExprFakeLineInfo to be documented

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


structure_annotation|ast|ExprField to be documented

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


structure_annotation|ast|ExprFind to be documented

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


structure_annotation|ast|ExprFor to be documented

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


structure_annotation|ast|ExprGoto to be documented

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


structure_annotation|ast|ExprIfThenElse to be documented

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


structure_annotation|ast|ExprInvoke to be documented

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


structure_annotation|ast|ExprIs to be documented

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


structure_annotation|ast|ExprIsVariant to be documented

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


structure_annotation|ast|ExprKeyExists to be documented

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


structure_annotation|ast|ExprLabel to be documented

.. _handle-ast-ExprLet:

.. das:attribute:: ExprLet

ExprLet fields are

+----------+------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`                                       +
+----------+------------------------------------------------------------------------------------+
+inScope   +bool                                                                                +
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


structure_annotation|ast|ExprLet to be documented

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


structure_annotation|ast|ExprLooksLikeCall to be documented

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


structure_annotation|ast|ExprMakeArray to be documented

.. _handle-ast-ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

ExprMakeBlock fields are

+----------+-----------------------------------------------------------+
+mmFlags   + :ref:`ExprMakeBlockFlags <alias-ExprMakeBlockFlags>`      +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle-rtti-LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias-ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+stackTop  +uint                                                       +
+----------+-----------------------------------------------------------+
+block     +smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias-ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias-ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


structure_annotation|ast|ExprMakeBlock to be documented

.. _handle-ast-ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

ExprMakeGenerator fields are

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
+iterType              +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias-ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


structure_annotation|ast|ExprMakeGenerator to be documented

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


structure_annotation|ast|ExprMakeLocal to be documented

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


structure_annotation|ast|ExprMakeStruct to be documented

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


structure_annotation|ast|ExprMakeTuple to be documented

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


structure_annotation|ast|ExprMakeVariant to be documented

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


structure_annotation|ast|ExprMemZero to be documented

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


structure_annotation|ast|ExprMove to be documented

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


structure_annotation|ast|ExprNamedCall to be documented

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


structure_annotation|ast|ExprNew to be documented

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


structure_annotation|ast|ExprNullCoalescing to be documented

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


structure_annotation|ast|ExprOp1 to be documented

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


structure_annotation|ast|ExprOp2 to be documented

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


structure_annotation|ast|ExprOp3 to be documented

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


structure_annotation|ast|ExprPtr2Ref to be documented

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


structure_annotation|ast|ExprReader to be documented

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


structure_annotation|ast|ExprRef2Ptr to be documented

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


structure_annotation|ast|ExprRef2Value to be documented

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


structure_annotation|ast|ExprReturn to be documented

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


structure_annotation|ast|ExprSafeAsVariant to be documented

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


structure_annotation|ast|ExprSafeAt to be documented

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


structure_annotation|ast|ExprSafeField to be documented

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


structure_annotation|ast|ExprStaticAssert to be documented

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


structure_annotation|ast|ExprStringBuilder to be documented

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


structure_annotation|ast|ExprSwizzle to be documented

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


structure_annotation|ast|ExprTryCatch to be documented

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


structure_annotation|ast|ExprTypeInfo to be documented

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


structure_annotation|ast|ExprVar to be documented

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


structure_annotation|ast|ExprWhile to be documented

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


structure_annotation|ast|ExprWith to be documented

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


structure_annotation|ast|ExprYield to be documented

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


structure_annotation|ast|Expression to be documented

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


structure_annotation|ast|FieldDeclaration to be documented

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


structure_annotation|ast|Function to be documented

.. _handle-ast-FunctionAnnotation:

.. das:attribute:: FunctionAnnotation

structure_annotation|ast|FunctionAnnotation to be documented

.. _handle-ast-InferHistory:

.. das:attribute:: InferHistory

InferHistory fields are

+----+----------------------------------------------+
+func+ :ref:`ast::Function <handle-ast-Function>` ? +
+----+----------------------------------------------+
+at  + :ref:`rtti::LineInfo <handle-rtti-LineInfo>` +
+----+----------------------------------------------+


structure_annotation|ast|InferHistory to be documented

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


structure_annotation|ast|MakeFieldDecl to be documented

.. _handle-ast-ModuleGroup:

.. das:attribute:: ModuleGroup

structure_annotation|ast|ModuleGroup to be documented

.. _handle-ast-ModuleLibrary:

.. das:attribute:: ModuleLibrary

structure_annotation|ast|ModuleLibrary to be documented

.. _handle-ast-PassMacro:

.. das:attribute:: PassMacro

PassMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+


structure_annotation|ast|PassMacro to be documented

.. _handle-ast-ReaderMacro:

.. das:attribute:: ReaderMacro

ReaderMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


structure_annotation|ast|ReaderMacro to be documented

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


structure_annotation|ast|Structure to be documented

.. _handle-ast-StructureAnnotation:

.. das:attribute:: StructureAnnotation

structure_annotation|ast|StructureAnnotation to be documented

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

+----------+----+
+isCtorType+bool+
+----------+----+
+alignOf   +int +
+----------+----+
+isVoid    +bool+
+----------+----+
+isAuto    +bool+
+----------+----+
+canMove   +bool+
+----------+----+
+isExprType+bool+
+----------+----+
+sizeOf    +int +
+----------+----+
+isAlias   +bool+
+----------+----+
+baseSizeOf+int +
+----------+----+
+canCopy   +bool+
+----------+----+
+countOf   +int +
+----------+----+


structure_annotation|ast|TypeDecl to be documented

.. _handle-ast-TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

TypeInfoMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle-rtti-Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle-builtin-das_string>` +
+-------+--------------------------------------------------------+


structure_annotation|ast|TypeInfoMacro to be documented

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


structure_annotation|ast|Variable to be documented

.. _handle-ast-VariantMacro:

.. das:attribute:: VariantMacro

VariantMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>` +
+----+--------------------------------------------------------+


structure_annotation|ast|VariantMacro to be documented

.. _handle-ast-VisitorAdapter:

.. das:attribute:: VisitorAdapter

structure_annotation|ast|VisitorAdapter to be documented

+++++++++++++
Handled types
+++++++++++++

.. _handle-ast-MakeStruct:

.. das:attribute:: MakeStruct

any_annotation|ast|MakeStruct to be documented

.. _handle-ast-dasvector`FieldDeclaration:

.. das:attribute:: dasvector`FieldDeclaration

any_annotation|ast|dasvector`FieldDeclaration to be documented

.. _handle-ast-dasvector`InferHistory:

.. das:attribute:: dasvector`InferHistory

any_annotation|ast|dasvector`InferHistory to be documented

.. _handle-ast-dasvector`LineInfo:

.. das:attribute:: dasvector`LineInfo

any_annotation|ast|dasvector`LineInfo to be documented

.. _handle-ast-dasvector`das_string:

.. das:attribute:: dasvector`das_string

any_annotation|ast|dasvector`das_string to be documented

.. _handle-ast-dasvector`int:

.. das:attribute:: dasvector`int

any_annotation|ast|dasvector`int to be documented

.. _handle-ast-dasvector`pair`das_string`smart_ptr`Expression:

.. das:attribute:: dasvector`pair`das_string`smart_ptr`Expression

any_annotation|ast|dasvector`pair`das_string`smart_ptr`Expression to be documented

.. _handle-ast-dasvector`pair`uint`uint:

.. das:attribute:: dasvector`pair`uint`uint

any_annotation|ast|dasvector`pair`uint`uint to be documented

.. _handle-ast-dasvector`smart_ptr`Expression:

.. das:attribute:: dasvector`smart_ptr`Expression

any_annotation|ast|dasvector`smart_ptr`Expression to be documented

.. _handle-ast-dasvector`smart_ptr`MakeFieldDecl:

.. das:attribute:: dasvector`smart_ptr`MakeFieldDecl

any_annotation|ast|dasvector`smart_ptr`MakeFieldDecl to be documented

.. _handle-ast-dasvector`smart_ptr`MakeStruct:

.. das:attribute:: dasvector`smart_ptr`MakeStruct

any_annotation|ast|dasvector`smart_ptr`MakeStruct to be documented

.. _handle-ast-dasvector`smart_ptr`TypeDecl:

.. das:attribute:: dasvector`smart_ptr`TypeDecl

any_annotation|ast|dasvector`smart_ptr`TypeDecl to be documented

.. _handle-ast-dasvector`smart_ptr`Variable:

.. das:attribute:: dasvector`smart_ptr`Variable

any_annotation|ast|dasvector`smart_ptr`Variable to be documented

.. _handle-ast-dasvector`uint8:

.. das:attribute:: dasvector`uint8

any_annotation|ast|dasvector`uint8 to be documented

+++++++
Classes
+++++++

.. _struct-ast-AstFunctionAnnotation:

.. das:class:: AstFunctionAnnotation

class|ast|AstFunctionAnnotation to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstFunctionAnnotation.transform (self:AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string )  : ExpressionPtr

transform returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+------+----------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>` +
+------+----------------------------------------------------------------------+
+call  +smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` >               +
+------+----------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle-builtin-das_string>`               +
+------+----------------------------------------------------------------------+


method|ast|AstFunctionAnnotation.transform to be documented

.. das:method:: AstFunctionAnnotation.apply (self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool

apply returns bool

arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstFunctionAnnotation <struct-ast-AstFunctionAnnotation>`           +
+------+--------------------------------------------------------------------------------+
+func  + :ref:`FunctionPtr <alias-FunctionPtr>`                                         +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+------+--------------------------------------------------------------------------------+


method|ast|AstFunctionAnnotation.apply to be documented

.. das:method:: AstFunctionAnnotation.finish (self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; progArgs:AnnotationArgumentList const; errors:das_string )  : bool

finish returns bool

arguments are

+--------+--------------------------------------------------------------------------------+
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


method|ast|AstFunctionAnnotation.finish to be documented

.. _struct-ast-AstPassMacro:

.. das:class:: AstPassMacro

class|ast|AstPassMacro to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstPassMacro.apply (self:AstPassMacro; prog:ProgramPtr; mod:rtti::Module? const )  : bool

apply returns bool

arguments are

+----+----------------------------------------------------+
+self+ :ref:`ast::AstPassMacro <struct-ast-AstPassMacro>` +
+----+----------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`               +
+----+----------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const   +
+----+----------------------------------------------------+


method|ast|AstPassMacro.apply to be documented

.. _struct-ast-AstReaderMacro:

.. das:class:: AstReaderMacro

class|ast|AstReaderMacro to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstReaderMacro.accept (self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:LineInfo const )  : bool

accept returns bool

arguments are

+----+--------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+----+--------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                   +
+----+--------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const       +
+----+--------------------------------------------------------+
+expr+ :ref:`ast::ExprReader <handle-ast-ExprReader>` ? const +
+----+--------------------------------------------------------+
+ch  +int const                                               +
+----+--------------------------------------------------------+
+info+ :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const    +
+----+--------------------------------------------------------+


method|ast|AstReaderMacro.accept to be documented

.. das:method:: AstReaderMacro.visit (self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr

visit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+----+-----------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                            +
+----+-----------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstReaderMacro.visit to be documented

.. _struct-ast-AstStructureAnnotation:

.. das:class:: AstStructureAnnotation

class|ast|AstStructureAnnotation to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstStructureAnnotation.apply (self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool

apply returns bool

arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+------+--------------------------------------------------------------------------------+


method|ast|AstStructureAnnotation.apply to be documented

.. das:method:: AstStructureAnnotation.finish (self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool

finish returns bool

arguments are

+------+--------------------------------------------------------------------------------+
+self  + :ref:`ast::AstStructureAnnotation <struct-ast-AstStructureAnnotation>`         +
+------+--------------------------------------------------------------------------------+
+st    + :ref:`StructurePtr <alias-StructurePtr>`                                       +
+------+--------------------------------------------------------------------------------+
+group + :ref:`ast::ModuleGroup <handle-ast-ModuleGroup>`                               +
+------+--------------------------------------------------------------------------------+
+args  + :ref:`rtti::AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  const+
+------+--------------------------------------------------------------------------------+
+errors+ :ref:`builtin::das_string <handle-builtin-das_string>`                         +
+------+--------------------------------------------------------------------------------+


method|ast|AstStructureAnnotation.finish to be documented

.. _struct-ast-AstVariantMacro:

.. das:class:: AstVariantMacro

class|ast|AstVariantMacro to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstVariantMacro.visitExprIsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVariantMacro.visitExprIsVariant to be documented

.. das:method:: AstVariantMacro.visitExprAsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`              +
+----+-----------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                  +
+----+-----------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                      +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVariantMacro.visitExprAsVariant to be documented

.. das:method:: AstVariantMacro.visitExprSafeAsVariant (self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVariantMacro <struct-ast-AstVariantMacro>`                      +
+----+-------------------------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`                                          +
+----+-------------------------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle-rtti-Module>` ? const                              +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVariantMacro.visitExprSafeAsVariant to be documented

.. _struct-ast-AstVisitor:

.. das:class:: AstVisitor

class|ast|AstVisitor to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstVisitor.preVisitProgram (self:AstVisitor; prog:ProgramPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`           +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitProgram to be documented

.. das:method:: AstVisitor.visitProgram (self:AstVisitor; porg:ProgramPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+porg+ :ref:`ProgramPtr <alias-ProgramPtr>`           +
+----+------------------------------------------------+


method|ast|AstVisitor.visitProgram to be documented

.. das:method:: AstVisitor.preVisitProgramBody (self:AstVisitor; prog:ProgramPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`           +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitProgramBody to be documented

.. das:method:: AstVisitor.preVisitTypeDecl (self:AstVisitor; typ:TypeDeclPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitTypeDecl to be documented

.. das:method:: AstVisitor.visitTypeDecl (self:AstVisitor; typ:TypeDeclPtr )  : TypeDeclPtr

visitTypeDecl returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.visitTypeDecl to be documented

.. das:method:: AstVisitor.preVisitAlias (self:AstVisitor; typ:TypeDeclPtr; name:das_string const ) 

arguments are

+----+--------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+----+--------------------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+----+--------------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+----+--------------------------------------------------------------+


method|ast|AstVisitor.preVisitAlias to be documented

.. das:method:: AstVisitor.visitAlias (self:AstVisitor; typ:TypeDeclPtr; name:das_string const )  : TypeDeclPtr

visitAlias returns  :ref:`TypeDeclPtr <alias-TypeDeclPtr>` 

arguments are

+----+--------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+----+--------------------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                       +
+----+--------------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+----+--------------------------------------------------------------+


method|ast|AstVisitor.visitAlias to be documented

.. das:method:: AstVisitor.preVisitEnumeration (self:AstVisitor; enu:EnumerationPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+enu + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitEnumeration to be documented

.. das:method:: AstVisitor.preVisitEnumerationValue (self:AstVisitor; enu:EnumerationPtr; name:das_string const; value:ExpressionPtr; last:bool const ) 

arguments are

+-----+--------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+-----+--------------------------------------------------------------+
+enu  + :ref:`EnumerationPtr <alias-EnumerationPtr>`                 +
+-----+--------------------------------------------------------------+
+name + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+-----+--------------------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                   +
+-----+--------------------------------------------------------------+
+last +bool const                                                    +
+-----+--------------------------------------------------------------+


method|ast|AstVisitor.preVisitEnumerationValue to be documented

.. das:method:: AstVisitor.visitEnumerationValue (self:AstVisitor; enu:EnumerationPtr; name:das_string const; value:ExpressionPtr; last:bool const )  : ExpressionPtr

visitEnumerationValue returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+-----+--------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+-----+--------------------------------------------------------------+
+enu  + :ref:`EnumerationPtr <alias-EnumerationPtr>`                 +
+-----+--------------------------------------------------------------+
+name + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+-----+--------------------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                   +
+-----+--------------------------------------------------------------+
+last +bool const                                                    +
+-----+--------------------------------------------------------------+


method|ast|AstVisitor.visitEnumerationValue to be documented

.. das:method:: AstVisitor.visitEnumeration (self:AstVisitor; enu:EnumerationPtr )  : EnumerationPtr

visitEnumeration returns  :ref:`EnumerationPtr <alias-EnumerationPtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+enu + :ref:`EnumerationPtr <alias-EnumerationPtr>`   +
+----+------------------------------------------------+


method|ast|AstVisitor.visitEnumeration to be documented

.. das:method:: AstVisitor.preVisitStructure (self:AstVisitor; str:StructurePtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+str + :ref:`StructurePtr <alias-StructurePtr>`       +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitStructure to be documented

.. das:method:: AstVisitor.preVisitStructureField (self:AstVisitor; str:StructurePtr; decl:FieldDeclaration const; last:bool const ) 

arguments are

+----+------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+----+------------------------------------------------------------------+
+str + :ref:`StructurePtr <alias-StructurePtr>`                         +
+----+------------------------------------------------------------------+
+decl+ :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const+
+----+------------------------------------------------------------------+
+last+bool const                                                        +
+----+------------------------------------------------------------------+


method|ast|AstVisitor.preVisitStructureField to be documented

.. das:method:: AstVisitor.visitStructureField (self:AstVisitor; str:StructurePtr; decl:FieldDeclaration const; last:bool const ) 

arguments are

+----+------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+----+------------------------------------------------------------------+
+str + :ref:`StructurePtr <alias-StructurePtr>`                         +
+----+------------------------------------------------------------------+
+decl+ :ref:`ast::FieldDeclaration <handle-ast-FieldDeclaration>`  const+
+----+------------------------------------------------------------------+
+last+bool const                                                        +
+----+------------------------------------------------------------------+


method|ast|AstVisitor.visitStructureField to be documented

.. das:method:: AstVisitor.visitStructure (self:AstVisitor; str:StructurePtr )  : StructurePtr

visitStructure returns  :ref:`StructurePtr <alias-StructurePtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+str + :ref:`StructurePtr <alias-StructurePtr>`       +
+----+------------------------------------------------+


method|ast|AstVisitor.visitStructure to be documented

.. das:method:: AstVisitor.preVisitFunction (self:AstVisitor; fun:FunctionPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitFunction to be documented

.. das:method:: AstVisitor.visitFunction (self:AstVisitor; fun:FunctionPtr )  : FunctionPtr

visitFunction returns  :ref:`FunctionPtr <alias-FunctionPtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.visitFunction to be documented

.. das:method:: AstVisitor.preVisitFunctionArgument (self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; lastArg:bool const ) 

arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+fun    + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


method|ast|AstVisitor.preVisitFunctionArgument to be documented

.. das:method:: AstVisitor.visitFunctionArgument (self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; lastArg:bool const )  : VariablePtr

visitFunctionArgument returns  :ref:`VariablePtr <alias-VariablePtr>` 

arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+fun    + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


method|ast|AstVisitor.visitFunctionArgument to be documented

.. das:method:: AstVisitor.preVisitFunctionArgumentInit (self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; value:ExpressionPtr ) 

arguments are

+-----+------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-----+------------------------------------------------+
+fun  + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+-----+------------------------------------------------+
+arg  + :ref:`VariablePtr <alias-VariablePtr>`         +
+-----+------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+-----+------------------------------------------------+


method|ast|AstVisitor.preVisitFunctionArgumentInit to be documented

.. das:method:: AstVisitor.visitFunctionArgumentInit (self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; value:ExpressionPtr )  : ExpressionPtr

visitFunctionArgumentInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+-----+------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-----+------------------------------------------------+
+fun  + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+-----+------------------------------------------------+
+arg  + :ref:`VariablePtr <alias-VariablePtr>`         +
+-----+------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+-----+------------------------------------------------+


method|ast|AstVisitor.visitFunctionArgumentInit to be documented

.. das:method:: AstVisitor.preVisitFunctionBody (self:AstVisitor; fun:FunctionPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitFunctionBody to be documented

.. das:method:: AstVisitor.visitFunctionBody (self:AstVisitor; fun:FunctionPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias-FunctionPtr>`         +
+----+------------------------------------------------+


method|ast|AstVisitor.visitFunctionBody to be documented

.. das:method:: AstVisitor.preVisitExpression (self:AstVisitor; expr:ExpressionPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitExpression to be documented

.. das:method:: AstVisitor.visitExpression (self:AstVisitor; expr:ExpressionPtr )  : ExpressionPtr

visitExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+----+------------------------------------------------+


method|ast|AstVisitor.visitExpression to be documented

.. das:method:: AstVisitor.preVisitExprBlock (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlock to be documented

.. das:method:: AstVisitor.visitExprBlock (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const )  : ExpressionPtr

visitExprBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlock to be documented

.. das:method:: AstVisitor.preVisitExprBlockArgument (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; lastArg:bool const ) 

arguments are

+-------+---------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-------+---------------------------------------------------------------+
+blk    +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+-------+---------------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`                        +
+-------+---------------------------------------------------------------+
+lastArg+bool const                                                     +
+-------+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlockArgument to be documented

.. das:method:: AstVisitor.visitExprBlockArgument (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; lastArg:bool const )  : VariablePtr

visitExprBlockArgument returns  :ref:`VariablePtr <alias-VariablePtr>` 

arguments are

+-------+---------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-------+---------------------------------------------------------------+
+blk    +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+-------+---------------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`                        +
+-------+---------------------------------------------------------------+
+lastArg+bool const                                                     +
+-------+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlockArgument to be documented

.. das:method:: AstVisitor.preVisitExprBlockArgumentInit (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; expr:ExpressionPtr ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`                        +
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlockArgumentInit to be documented

.. das:method:: AstVisitor.visitExprBlockArgumentInit (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr

visitExprBlockArgumentInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`                        +
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlockArgumentInit to be documented

.. das:method:: AstVisitor.preVisitExprBlockExpression (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlockExpression to be documented

.. das:method:: AstVisitor.visitExprBlockExpression (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  : ExpressionPtr

visitExprBlockExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlockExpression to be documented

.. das:method:: AstVisitor.preVisitExprBlockFinal (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlockFinal to be documented

.. das:method:: AstVisitor.visitExprBlockFinal (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlockFinal to be documented

.. das:method:: AstVisitor.preVisitExprBlockFinalExpression (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBlockFinalExpression to be documented

.. das:method:: AstVisitor.visitExprBlockFinalExpression (self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  : ExpressionPtr

visitExprBlockFinalExpression returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle-ast-ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBlockFinalExpression to be documented

.. das:method:: AstVisitor.preVisitExprLet (self:AstVisitor; expr:smart_ptr<ast::ExprLet> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLet to be documented

.. das:method:: AstVisitor.visitExprLet (self:AstVisitor; expr:smart_ptr<ast::ExprLet> const )  : ExpressionPtr

visitExprLet returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprLet to be documented

.. das:method:: AstVisitor.preVisitExprLetVariable (self:AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:VariablePtr; lastArg:bool const ) 

arguments are

+-------+-----------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-------+-----------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+-------+-----------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+-------+-----------------------------------------------------------+
+lastArg+bool const                                                 +
+-------+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLetVariable to be documented

.. das:method:: AstVisitor.visitExprLetVariable (self:AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:VariablePtr; lastArg:bool const )  : VariablePtr

visitExprLetVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

arguments are

+-------+-----------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-------+-----------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+-------+-----------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`                    +
+-------+-----------------------------------------------------------+
+lastArg+bool const                                                 +
+-------+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprLetVariable to be documented

.. das:method:: AstVisitor.preVisitExprLetVariableInit (self:AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:VariablePtr; expr:ExpressionPtr ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`                    +
+----+-----------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLetVariableInit to be documented

.. das:method:: AstVisitor.visitExprLetVariableInit (self:AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr

visitExprLetVariableInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprLet <handle-ast-ExprLet>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`                    +
+----+-----------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprLetVariableInit to be documented

.. das:method:: AstVisitor.preVisitGlobalLet (self:AstVisitor; prog:ProgramPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`           +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitGlobalLet to be documented

.. das:method:: AstVisitor.visitGlobalLet (self:AstVisitor; prog:ProgramPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias-ProgramPtr>`           +
+----+------------------------------------------------+


method|ast|AstVisitor.visitGlobalLet to be documented

.. das:method:: AstVisitor.preVisitGlobalLetVariable (self:AstVisitor; arg:VariablePtr; lastArg:bool const ) 

arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


method|ast|AstVisitor.preVisitGlobalLetVariable to be documented

.. das:method:: AstVisitor.visitGlobalLetVariable (self:AstVisitor; arg:VariablePtr; lastArg:bool const )  : VariablePtr

visitGlobalLetVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias-VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


method|ast|AstVisitor.visitGlobalLetVariable to be documented

.. das:method:: AstVisitor.preVisitGlobalLetVariableInit (self:AstVisitor; arg:VariablePtr; expr:ExpressionPtr ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`         +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+----+------------------------------------------------+


method|ast|AstVisitor.preVisitGlobalLetVariableInit to be documented

.. das:method:: AstVisitor.visitGlobalLetVariableInit (self:AstVisitor; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr

visitGlobalLetVariableInit returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+arg + :ref:`VariablePtr <alias-VariablePtr>`         +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias-ExpressionPtr>`     +
+----+------------------------------------------------+


method|ast|AstVisitor.visitGlobalLetVariableInit to be documented

.. das:method:: AstVisitor.preVisitExprStringBuilder (self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprStringBuilder to be documented

.. das:method:: AstVisitor.visitExprStringBuilder (self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const )  : ExpressionPtr

visitExprStringBuilder returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprStringBuilder to be documented

.. das:method:: AstVisitor.preVisitExprStringBuilderElement (self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:ExpressionPtr; last:bool const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+
+elem+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprStringBuilderElement to be documented

.. das:method:: AstVisitor.visitExprStringBuilderElement (self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprStringBuilderElement returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle-ast-ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+
+elem+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprStringBuilderElement to be documented

.. das:method:: AstVisitor.preVisitExprNew (self:AstVisitor; expr:smart_ptr<ast::ExprNew> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNew to be documented

.. das:method:: AstVisitor.visitExprNew (self:AstVisitor; expr:smart_ptr<ast::ExprNew> const )  : ExpressionPtr

visitExprNew returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprNew to be documented

.. das:method:: AstVisitor.preVisitExprNewArgument (self:AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:ExpressionPtr; last:bool const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNewArgument to be documented

.. das:method:: AstVisitor.visitExprNewArgument (self:AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprNewArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle-ast-ExprNew>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprNewArgument to be documented

.. das:method:: AstVisitor.preVisitExprNamedCall (self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNamedCall to be documented

.. das:method:: AstVisitor.visitExprNamedCall (self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const )  : ExpressionPtr

visitExprNamedCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprNamedCall to be documented

.. das:method:: AstVisitor.preVisitExprNamedCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:MakeFieldDeclPtr; last:bool const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+
+arg + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                      +
+----+-----------------------------------------------------------------------+
+last+bool const                                                             +
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNamedCallArgument to be documented

.. das:method:: AstVisitor.visitExprNamedCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr

visitExprNamedCallArgument returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle-ast-ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+
+arg + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                      +
+----+-----------------------------------------------------------------------+
+last+bool const                                                             +
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprNamedCallArgument to be documented

.. das:method:: AstVisitor.preVisitExprLooksLikeCall (self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLooksLikeCall to be documented

.. das:method:: AstVisitor.visitExprLooksLikeCall (self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const )  : ExpressionPtr

visitExprLooksLikeCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprLooksLikeCall to be documented

.. das:method:: AstVisitor.preVisitExprLooksLikeCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:ExpressionPtr; last:bool const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLooksLikeCallArgument to be documented

.. das:method:: AstVisitor.visitExprLooksLikeCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprLooksLikeCallArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle-ast-ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprLooksLikeCallArgument to be documented

.. das:method:: AstVisitor.preVisitExprCall (self:AstVisitor; expr:smart_ptr<ast::ExprCall> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCall to be documented

.. das:method:: AstVisitor.visitExprCall (self:AstVisitor; expr:smart_ptr<ast::ExprCall> const )  : ExpressionPtr

visitExprCall returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprCall to be documented

.. das:method:: AstVisitor.preVisitExprCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:ExpressionPtr; last:bool const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+----+-------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+----+-------------------------------------------------------------+
+last+bool const                                                   +
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCallArgument to be documented

.. das:method:: AstVisitor.visitExprCallArgument (self:AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprCallArgument returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle-ast-ExprCall>` > const+
+----+-------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+----+-------------------------------------------------------------+
+last+bool const                                                   +
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprCallArgument to be documented

.. das:method:: AstVisitor.preVisitExprNullCoalescing (self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const ) 

arguments are

+----+---------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+----+---------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+----+---------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNullCoalescing to be documented

.. das:method:: AstVisitor.visitExprNullCoalescing (self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const )  : ExpressionPtr

visitExprNullCoalescing returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+----+---------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+----+---------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprNullCoalescing to be documented

.. das:method:: AstVisitor.preVisitExprNullCoalescingDefault (self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const; defval:ExpressionPtr ) 

arguments are

+------+---------------------------------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+------+---------------------------------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprNullCoalescing <handle-ast-ExprNullCoalescing>` > const+
+------+---------------------------------------------------------------------------------+
+defval+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                                      +
+------+---------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprNullCoalescingDefault to be documented

.. das:method:: AstVisitor.preVisitExprAt (self:AstVisitor; expr:smart_ptr<ast::ExprAt> const ) 

arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+----+---------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAt to be documented

.. das:method:: AstVisitor.visitExprAt (self:AstVisitor; expr:smart_ptr<ast::ExprAt> const )  : ExpressionPtr

visitExprAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+----+---------------------------------------------------------+


method|ast|AstVisitor.visitExprAt to be documented

.. das:method:: AstVisitor.preVisitExprAtIndex (self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:ExpressionPtr ) 

arguments are

+-----+---------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+-----+---------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+-----+---------------------------------------------------------+
+index+ :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+-----+---------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAtIndex to be documented

.. das:method:: AstVisitor.preVisitExprSafeAt (self:AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprSafeAt to be documented

.. das:method:: AstVisitor.visitExprSafeAt (self:AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const )  : ExpressionPtr

visitExprSafeAt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAt <handle-ast-ExprSafeAt>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprSafeAt to be documented

.. das:method:: AstVisitor.preVisitExprSafeAtIndex (self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:ExpressionPtr ) 

arguments are

+-----+---------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+-----+---------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+-----+---------------------------------------------------------+
+index+ :ref:`ExpressionPtr <alias-ExpressionPtr>`              +
+-----+---------------------------------------------------------+


method|ast|AstVisitor.preVisitExprSafeAtIndex to be documented

.. das:method:: AstVisitor.preVisitExprIs (self:AstVisitor; expr:smart_ptr<ast::ExprIs> const ) 

arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+----+---------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIs to be documented

.. das:method:: AstVisitor.visitExprIs (self:AstVisitor; expr:smart_ptr<ast::ExprIs> const )  : ExpressionPtr

visitExprIs returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIs <handle-ast-ExprIs>` > const+
+----+---------------------------------------------------------+


method|ast|AstVisitor.visitExprIs to be documented

.. das:method:: AstVisitor.preVisitExprIsType (self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; typeDecl:TypeDeclPtr ) 

arguments are

+--------+---------------------------------------------------------+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle-ast-ExprAt>` > const+
+--------+---------------------------------------------------------+
+typeDecl+ :ref:`TypeDeclPtr <alias-TypeDeclPtr>`                  +
+--------+---------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIsType to be documented

.. das:method:: AstVisitor.preVisitExprOp2 (self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp2 to be documented

.. das:method:: AstVisitor.visitExprOp2 (self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const )  : ExpressionPtr

visitExprOp2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprOp2 to be documented

.. das:method:: AstVisitor.preVisitExprOp2Right (self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const; right:ExpressionPtr ) 

arguments are

+-----+-----------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-----+-----------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprOp2 <handle-ast-ExprOp2>` > const+
+-----+-----------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+-----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp2Right to be documented

.. das:method:: AstVisitor.preVisitExprOp3 (self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp3 to be documented

.. das:method:: AstVisitor.visitExprOp3 (self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const )  : ExpressionPtr

visitExprOp3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprOp3 to be documented

.. das:method:: AstVisitor.preVisitExprOp3Left (self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const; left:ExpressionPtr ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+----+-----------------------------------------------------------+
+left+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp3Left to be documented

.. das:method:: AstVisitor.preVisitExprOp3Right (self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const; right:ExpressionPtr ) 

arguments are

+-----+-----------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-----+-----------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprOp3 <handle-ast-ExprOp3>` > const+
+-----+-----------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+-----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp3Right to be documented

.. das:method:: AstVisitor.preVisitExprCopy (self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCopy to be documented

.. das:method:: AstVisitor.visitExprCopy (self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const )  : ExpressionPtr

visitExprCopy returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprCopy to be documented

.. das:method:: AstVisitor.preVisitExprCopyRight (self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const; right:ExpressionPtr ) 

arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprCopy <handle-ast-ExprCopy>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCopyRight to be documented

.. das:method:: AstVisitor.preVisitExprMove (self:AstVisitor; expr:smart_ptr<ast::ExprMove> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMove to be documented

.. das:method:: AstVisitor.visitExprMove (self:AstVisitor; expr:smart_ptr<ast::ExprMove> const )  : ExpressionPtr

visitExprMove returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprMove to be documented

.. das:method:: AstVisitor.preVisitExprMoveRight (self:AstVisitor; expr:smart_ptr<ast::ExprMove> const; right:ExpressionPtr ) 

arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMove <handle-ast-ExprMove>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMoveRight to be documented

.. das:method:: AstVisitor.preVisitExprClone (self:AstVisitor; expr:smart_ptr<ast::ExprClone> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprClone to be documented

.. das:method:: AstVisitor.visitExprClone (self:AstVisitor; expr:smart_ptr<ast::ExprClone> const )  : ExpressionPtr

visitExprClone returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprClone to be documented

.. das:method:: AstVisitor.preVisitExprCloneRight (self:AstVisitor; expr:smart_ptr<ast::ExprClone> const; right:ExpressionPtr ) 

arguments are

+-----+---------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-----+---------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprClone <handle-ast-ExprClone>` > const+
+-----+---------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+-----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCloneRight to be documented

.. das:method:: AstVisitor.preVisitExprWith (self:AstVisitor; expr:smart_ptr<ast::ExprWith> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprWith to be documented

.. das:method:: AstVisitor.visitExprWith (self:AstVisitor; expr:smart_ptr<ast::ExprWith> const )  : ExpressionPtr

visitExprWith returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprWith to be documented

.. das:method:: AstVisitor.preVisitExprWithBody (self:AstVisitor; expr:smart_ptr<ast::ExprWith> const; right:ExpressionPtr ) 

arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprWith <handle-ast-ExprWith>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprWithBody to be documented

.. das:method:: AstVisitor.preVisitExprWhile (self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprWhile to be documented

.. das:method:: AstVisitor.visitExprWhile (self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const )  : ExpressionPtr

visitExprWhile returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprWhile to be documented

.. das:method:: AstVisitor.preVisitExprWhileBody (self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const; right:ExpressionPtr ) 

arguments are

+-----+---------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-----+---------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprWhile <handle-ast-ExprWhile>` > const+
+-----+---------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                    +
+-----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprWhileBody to be documented

.. das:method:: AstVisitor.preVisitExprTryCatch (self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const ) 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprTryCatch to be documented

.. das:method:: AstVisitor.visitExprTryCatch (self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const )  : ExpressionPtr

visitExprTryCatch returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.visitExprTryCatch to be documented

.. das:method:: AstVisitor.preVisitExprTryCatchCatch (self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const; right:ExpressionPtr ) 

arguments are

+-----+---------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+-----+---------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprTryCatch <handle-ast-ExprTryCatch>` > const+
+-----+---------------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                          +
+-----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprTryCatchCatch to be documented

.. das:method:: AstVisitor.preVisitExprIfThenElse (self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIfThenElse to be documented

.. das:method:: AstVisitor.visitExprIfThenElse (self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const )  : ExpressionPtr

visitExprIfThenElse returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprIfThenElse to be documented

.. das:method:: AstVisitor.preVisitExprIfThenElseIfBlock (self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; ifBlock:ExpressionPtr ) 

arguments are

+-------+-------------------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-------+-------------------------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+-------+-------------------------------------------------------------------------+
+ifBlock+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+-------+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIfThenElseIfBlock to be documented

.. das:method:: AstVisitor.preVisitExprIfThenElseElseBlock (self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; elseBlock:ExpressionPtr ) 

arguments are

+---------+-------------------------------------------------------------------------+
+self     + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+---------+-------------------------------------------------------------------------+
+expr     +smart_ptr< :ref:`ast::ExprIfThenElse <handle-ast-ExprIfThenElse>` > const+
+---------+-------------------------------------------------------------------------+
+elseBlock+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                              +
+---------+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIfThenElseElseBlock to be documented

.. das:method:: AstVisitor.preVisitExprFor (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprFor to be documented

.. das:method:: AstVisitor.visitExprFor (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const )  : ExpressionPtr

visitExprFor returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprFor to be documented

.. das:method:: AstVisitor.preVisitExprForVariable (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:VariablePtr; last:bool const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+
+svar+ :ref:`VariablePtr <alias-VariablePtr>`                    +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprForVariable to be documented

.. das:method:: AstVisitor.visitExprForVariable (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:VariablePtr; last:bool const )  : VariablePtr

visitExprForVariable returns  :ref:`VariablePtr <alias-VariablePtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+
+svar+ :ref:`VariablePtr <alias-VariablePtr>`                    +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprForVariable to be documented

.. das:method:: AstVisitor.preVisitExprForSource (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:ExpressionPtr; last:bool const ) 

arguments are

+------+-----------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+------+-----------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+------+-----------------------------------------------------------+
+source+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+------+-----------------------------------------------------------+
+last  +bool const                                                 +
+------+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprForSource to be documented

.. das:method:: AstVisitor.visitExprForSource (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprForSource returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+------+-----------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+------+-----------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+------+-----------------------------------------------------------+
+source+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                +
+------+-----------------------------------------------------------+
+last  +bool const                                                 +
+------+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprForSource to be documented

.. das:method:: AstVisitor.preVisitExprForStack (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprForStack to be documented

.. das:method:: AstVisitor.preVisitExprForBody (self:AstVisitor; expr:smart_ptr<ast::ExprFor> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle-ast-ExprFor>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprForBody to be documented

.. das:method:: AstVisitor.preVisitExprMakeVariant (self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeVariant to be documented

.. das:method:: AstVisitor.visitExprMakeVariant (self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const )  : ExpressionPtr

visitExprMakeVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeVariant to be documented

.. das:method:: AstVisitor.preVisitExprMakeVariantField (self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:MakeFieldDeclPtr; last:bool const ) 

arguments are

+-----+---------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+-----+---------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+-----+---------------------------------------------------------------------------+
+index+int const                                                                  +
+-----+---------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                          +
+-----+---------------------------------------------------------------------------+
+last +bool const                                                                 +
+-----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeVariantField to be documented

.. das:method:: AstVisitor.visitExprMakeVariantField (self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr

visitExprMakeVariantField returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

arguments are

+-----+---------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+-----+---------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeVariant <handle-ast-ExprMakeVariant>` > const+
+-----+---------------------------------------------------------------------------+
+index+int const                                                                  +
+-----+---------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                          +
+-----+---------------------------------------------------------------------------+
+last +bool const                                                                 +
+-----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeVariantField to be documented

.. das:method:: AstVisitor.preVisitExprMakeStruct (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeStruct to be documented

.. das:method:: AstVisitor.visitExprMakeStruct (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const )  : ExpressionPtr

visitExprMakeStruct returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeStruct to be documented

.. das:method:: AstVisitor.preVisitExprMakeStructIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const ) 

arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeStructIndex to be documented

.. das:method:: AstVisitor.visitExprMakeStructIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const ) 

arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeStructIndex to be documented

.. das:method:: AstVisitor.preVisitExprMakeStructField (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:MakeFieldDeclPtr; last:bool const ) 

arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                        +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeStructField to be documented

.. das:method:: AstVisitor.visitExprMakeStructField (self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr

visitExprMakeStructField returns  :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>` 

arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle-ast-ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias-MakeFieldDeclPtr>`                        +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeStructField to be documented

.. das:method:: AstVisitor.preVisitExprMakeArray (self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeArray to be documented

.. das:method:: AstVisitor.visitExprMakeArray (self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const )  : ExpressionPtr

visitExprMakeArray returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeArray to be documented

.. das:method:: AstVisitor.preVisitExprMakeArrayIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:ExpressionPtr; last:bool const ) 

arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeArrayIndex to be documented

.. das:method:: AstVisitor.visitExprMakeArrayIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprMakeArrayIndex returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeArray <handle-ast-ExprMakeArray>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeArrayIndex to be documented

.. das:method:: AstVisitor.preVisitExprMakeTuple (self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeTuple to be documented

.. das:method:: AstVisitor.visitExprMakeTuple (self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const )  : ExpressionPtr

visitExprMakeTuple returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeTuple to be documented

.. das:method:: AstVisitor.preVisitExprMakeTupleIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:ExpressionPtr; last:bool const ) 

arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeTupleIndex to be documented

.. das:method:: AstVisitor.visitExprMakeTupleIndex (self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:ExpressionPtr; last:bool const )  : ExpressionPtr

visitExprMakeTupleIndex returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeTuple <handle-ast-ExprMakeTuple>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias-ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeTupleIndex to be documented

.. das:method:: AstVisitor.preVisitExprArrayComprehension (self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const ) 

arguments are

+----+-----------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+----+-----------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+----+-----------------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprArrayComprehension to be documented

.. das:method:: AstVisitor.visitExprArrayComprehension (self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const )  : ExpressionPtr

visitExprArrayComprehension returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+----+-----------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+----+-----------------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprArrayComprehension to be documented

.. das:method:: AstVisitor.preVisitExprArrayComprehensionSubexpr (self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; subexrp:ExpressionPtr ) 

arguments are

+-------+-----------------------------------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+-------+-----------------------------------------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+-------+-----------------------------------------------------------------------------------------+
+subexrp+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+-------+-----------------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprArrayComprehensionSubexpr to be documented

.. das:method:: AstVisitor.preVisitExprArrayComprehensionWhere (self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; filter:ExpressionPtr ) 

arguments are

+------+-----------------------------------------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+------+-----------------------------------------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprArrayComprehension <handle-ast-ExprArrayComprehension>` > const+
+------+-----------------------------------------------------------------------------------------+
+filter+ :ref:`ExpressionPtr <alias-ExpressionPtr>`                                              +
+------+-----------------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprArrayComprehensionWhere to be documented

.. das:method:: AstVisitor.preVisitExprTypeInfo (self:AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const ) 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprTypeInfo to be documented

.. das:method:: AstVisitor.visitExprTypeInfo (self:AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const )  : ExpressionPtr

visitExprTypeInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTypeInfo <handle-ast-ExprTypeInfo>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.visitExprTypeInfo to be documented

.. das:method:: AstVisitor.preVisitExprPtr2Ref (self:AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const ) 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprPtr2Ref to be documented

.. das:method:: AstVisitor.visitExprPtr2Ref (self:AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const )  : ExpressionPtr

visitExprPtr2Ref returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprPtr2Ref <handle-ast-ExprPtr2Ref>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.visitExprPtr2Ref to be documented

.. das:method:: AstVisitor.preVisitExprLabel (self:AstVisitor; expr:smart_ptr<ast::ExprLabel> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprLabel to be documented

.. das:method:: AstVisitor.visitExprLabel (self:AstVisitor; expr:smart_ptr<ast::ExprLabel> const )  : ExpressionPtr

visitExprLabel returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLabel <handle-ast-ExprLabel>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprLabel to be documented

.. das:method:: AstVisitor.preVisitExprGoto (self:AstVisitor; expr:smart_ptr<ast::ExprGoto> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprGoto to be documented

.. das:method:: AstVisitor.visitExprGoto (self:AstVisitor; expr:smart_ptr<ast::ExprGoto> const )  : ExpressionPtr

visitExprGoto returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprGoto <handle-ast-ExprGoto>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprGoto to be documented

.. das:method:: AstVisitor.preVisitExprRef2Value (self:AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprRef2Value to be documented

.. das:method:: AstVisitor.visitExprRef2Value (self:AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const )  : ExpressionPtr

visitExprRef2Value returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Value <handle-ast-ExprRef2Value>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprRef2Value to be documented

.. das:method:: AstVisitor.preVisitExprRef2Ptr (self:AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const ) 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprRef2Ptr to be documented

.. das:method:: AstVisitor.visitExprRef2Ptr (self:AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const )  : ExpressionPtr

visitExprRef2Ptr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Ptr <handle-ast-ExprRef2Ptr>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.visitExprRef2Ptr to be documented

.. das:method:: AstVisitor.preVisitExprAddr (self:AstVisitor; expr:smart_ptr<ast::ExprAddr> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAddr to be documented

.. das:method:: AstVisitor.visitExprAddr (self:AstVisitor; expr:smart_ptr<ast::ExprAddr> const )  : ExpressionPtr

visitExprAddr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAddr <handle-ast-ExprAddr>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprAddr to be documented

.. das:method:: AstVisitor.preVisitExprAssert (self:AstVisitor; expr:smart_ptr<ast::ExprAssert> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAssert to be documented

.. das:method:: AstVisitor.visitExprAssert (self:AstVisitor; expr:smart_ptr<ast::ExprAssert> const )  : ExpressionPtr

visitExprAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAssert <handle-ast-ExprAssert>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprAssert to be documented

.. das:method:: AstVisitor.preVisitExprStaticAssert (self:AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const ) 

arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+----+-----------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprStaticAssert to be documented

.. das:method:: AstVisitor.visitExprStaticAssert (self:AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const )  : ExpressionPtr

visitExprStaticAssert returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStaticAssert <handle-ast-ExprStaticAssert>` > const+
+----+-----------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprStaticAssert to be documented

.. das:method:: AstVisitor.preVisitExprDebug (self:AstVisitor; expr:smart_ptr<ast::ExprDebug> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprDebug to be documented

.. das:method:: AstVisitor.visitExprDebug (self:AstVisitor; expr:smart_ptr<ast::ExprDebug> const )  : ExpressionPtr

visitExprDebug returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDebug <handle-ast-ExprDebug>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprDebug to be documented

.. das:method:: AstVisitor.preVisitExprInvoke (self:AstVisitor; expr:smart_ptr<ast::ExprInvoke> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprInvoke to be documented

.. das:method:: AstVisitor.visitExprInvoke (self:AstVisitor; expr:smart_ptr<ast::ExprInvoke> const )  : ExpressionPtr

visitExprInvoke returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprInvoke <handle-ast-ExprInvoke>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprInvoke to be documented

.. das:method:: AstVisitor.preVisitExprErase (self:AstVisitor; expr:smart_ptr<ast::ExprErase> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprErase to be documented

.. das:method:: AstVisitor.visitExprErase (self:AstVisitor; expr:smart_ptr<ast::ExprErase> const )  : ExpressionPtr

visitExprErase returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprErase <handle-ast-ExprErase>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprErase to be documented

.. das:method:: AstVisitor.preVisitExprFind (self:AstVisitor; expr:smart_ptr<ast::ExprFind> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprFind to be documented

.. das:method:: AstVisitor.visitExprFind (self:AstVisitor; expr:smart_ptr<ast::ExprFind> const )  : ExpressionPtr

visitExprFind returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFind <handle-ast-ExprFind>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprFind to be documented

.. das:method:: AstVisitor.preVisitExprKeyExists (self:AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprKeyExists to be documented

.. das:method:: AstVisitor.visitExprKeyExists (self:AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const )  : ExpressionPtr

visitExprKeyExists returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprKeyExists <handle-ast-ExprKeyExists>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprKeyExists to be documented

.. das:method:: AstVisitor.preVisitExprAscend (self:AstVisitor; expr:smart_ptr<ast::ExprAscend> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAscend to be documented

.. das:method:: AstVisitor.visitExprAscend (self:AstVisitor; expr:smart_ptr<ast::ExprAscend> const )  : ExpressionPtr

visitExprAscend returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAscend <handle-ast-ExprAscend>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprAscend to be documented

.. das:method:: AstVisitor.preVisitExprCast (self:AstVisitor; expr:smart_ptr<ast::ExprCast> const ) 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprCast to be documented

.. das:method:: AstVisitor.visitExprCast (self:AstVisitor; expr:smart_ptr<ast::ExprCast> const )  : ExpressionPtr

visitExprCast returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCast <handle-ast-ExprCast>` > const+
+----+-------------------------------------------------------------+


method|ast|AstVisitor.visitExprCast to be documented

.. das:method:: AstVisitor.preVisitExprDelete (self:AstVisitor; expr:smart_ptr<ast::ExprDelete> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprDelete to be documented

.. das:method:: AstVisitor.visitExprDelete (self:AstVisitor; expr:smart_ptr<ast::ExprDelete> const )  : ExpressionPtr

visitExprDelete returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDelete <handle-ast-ExprDelete>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprDelete to be documented

.. das:method:: AstVisitor.preVisitExprVar (self:AstVisitor; expr:smart_ptr<ast::ExprVar> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprVar to be documented

.. das:method:: AstVisitor.visitExprVar (self:AstVisitor; expr:smart_ptr<ast::ExprVar> const )  : ExpressionPtr

visitExprVar returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprVar <handle-ast-ExprVar>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprVar to be documented

.. das:method:: AstVisitor.preVisitExprField (self:AstVisitor; expr:smart_ptr<ast::ExprField> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprField to be documented

.. das:method:: AstVisitor.visitExprField (self:AstVisitor; expr:smart_ptr<ast::ExprField> const )  : ExpressionPtr

visitExprField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprField <handle-ast-ExprField>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprField to be documented

.. das:method:: AstVisitor.preVisitExprSafeField (self:AstVisitor; expr:smart_ptr<ast::ExprSafeField> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprSafeField to be documented

.. das:method:: AstVisitor.visitExprSafeField (self:AstVisitor; expr:smart_ptr<ast::ExprSafeField> const )  : ExpressionPtr

visitExprSafeField returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeField <handle-ast-ExprSafeField>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprSafeField to be documented

.. das:method:: AstVisitor.preVisitExprSwizzle (self:AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const ) 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprSwizzle to be documented

.. das:method:: AstVisitor.visitExprSwizzle (self:AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const )  : ExpressionPtr

visitExprSwizzle returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSwizzle <handle-ast-ExprSwizzle>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.visitExprSwizzle to be documented

.. das:method:: AstVisitor.preVisitExprIsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprIsVariant to be documented

.. das:method:: AstVisitor.visitExprIsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr

visitExprIsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle-ast-ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprIsVariant to be documented

.. das:method:: AstVisitor.preVisitExprAsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprAsVariant to be documented

.. das:method:: AstVisitor.visitExprAsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr

visitExprAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle-ast-ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprAsVariant to be documented

.. das:method:: AstVisitor.preVisitExprSafeAsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprSafeAsVariant to be documented

.. das:method:: AstVisitor.visitExprSafeAsVariant (self:AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle-ast-ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprSafeAsVariant to be documented

.. das:method:: AstVisitor.preVisitExprOp1 (self:AstVisitor; expr:smart_ptr<ast::ExprOp1> const ) 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.preVisitExprOp1 to be documented

.. das:method:: AstVisitor.visitExprOp1 (self:AstVisitor; expr:smart_ptr<ast::ExprOp1> const )  : ExpressionPtr

visitExprOp1 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp1 <handle-ast-ExprOp1>` > const+
+----+-----------------------------------------------------------+


method|ast|AstVisitor.visitExprOp1 to be documented

.. das:method:: AstVisitor.preVisitExprReturn (self:AstVisitor; expr:smart_ptr<ast::ExprReturn> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprReturn to be documented

.. das:method:: AstVisitor.visitExprReturn (self:AstVisitor; expr:smart_ptr<ast::ExprReturn> const )  : ExpressionPtr

visitExprReturn returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReturn <handle-ast-ExprReturn>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprReturn to be documented

.. das:method:: AstVisitor.preVisitExprYield (self:AstVisitor; expr:smart_ptr<ast::ExprYield> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprYield to be documented

.. das:method:: AstVisitor.visitExprYield (self:AstVisitor; expr:smart_ptr<ast::ExprYield> const )  : ExpressionPtr

visitExprYield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprYield <handle-ast-ExprYield>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprYield to be documented

.. das:method:: AstVisitor.preVisitExprBreak (self:AstVisitor; expr:smart_ptr<ast::ExprBreak> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprBreak to be documented

.. das:method:: AstVisitor.visitExprBreak (self:AstVisitor; expr:smart_ptr<ast::ExprBreak> const )  : ExpressionPtr

visitExprBreak returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprBreak <handle-ast-ExprBreak>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprBreak to be documented

.. das:method:: AstVisitor.preVisitExprContinue (self:AstVisitor; expr:smart_ptr<ast::ExprContinue> const ) 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprContinue to be documented

.. das:method:: AstVisitor.visitExprContinue (self:AstVisitor; expr:smart_ptr<ast::ExprContinue> const )  : ExpressionPtr

visitExprContinue returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprContinue <handle-ast-ExprContinue>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.visitExprContinue to be documented

.. das:method:: AstVisitor.preVisitExprMakeBlock (self:AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeBlock to be documented

.. das:method:: AstVisitor.visitExprMakeBlock (self:AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const )  : ExpressionPtr

visitExprMakeBlock returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeBlock <handle-ast-ExprMakeBlock>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeBlock to be documented

.. das:method:: AstVisitor.preVisitExprMakeGenerator (self:AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMakeGenerator to be documented

.. das:method:: AstVisitor.visitExprMakeGenerator (self:AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const )  : ExpressionPtr

visitExprMakeGenerator returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeGenerator <handle-ast-ExprMakeGenerator>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMakeGenerator to be documented

.. das:method:: AstVisitor.preVisitExprMemZero (self:AstVisitor; expr:smart_ptr<ast::ExprMemZero> const ) 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprMemZero to be documented

.. das:method:: AstVisitor.visitExprMemZero (self:AstVisitor; expr:smart_ptr<ast::ExprMemZero> const )  : ExpressionPtr

visitExprMemZero returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMemZero <handle-ast-ExprMemZero>` > const+
+----+-------------------------------------------------------------------+


method|ast|AstVisitor.visitExprMemZero to be documented

.. das:method:: AstVisitor.preVisitExprConst (self:AstVisitor; expr:smart_ptr<ast::ExprConst> const ) 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConst to be documented

.. das:method:: AstVisitor.visitExprConst (self:AstVisitor; expr:smart_ptr<ast::ExprConst> const )  : ExpressionPtr

visitExprConst returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConst <handle-ast-ExprConst>` > const+
+----+---------------------------------------------------------------+


method|ast|AstVisitor.visitExprConst to be documented

.. das:method:: AstVisitor.preVisitExprConstPtr (self:AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const ) 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstPtr to be documented

.. das:method:: AstVisitor.visitExprConstPtr (self:AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const )  : ExpressionPtr

visitExprConstPtr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstPtr <handle-ast-ExprConstPtr>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstPtr to be documented

.. das:method:: AstVisitor.preVisitExprConstEnumeration (self:AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const ) 

arguments are

+----+-------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+----+-------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+----+-------------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstEnumeration to be documented

.. das:method:: AstVisitor.visitExprConstEnumeration (self:AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const )  : ExpressionPtr

visitExprConstEnumeration returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+----+-------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstEnumeration <handle-ast-ExprConstEnumeration>` > const+
+----+-------------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstEnumeration to be documented

.. das:method:: AstVisitor.preVisitExprConstBitfield (self:AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const ) 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstBitfield to be documented

.. das:method:: AstVisitor.visitExprConstBitfield (self:AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const )  : ExpressionPtr

visitExprConstBitfield returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBitfield <handle-ast-ExprConstBitfield>` > const+
+----+-------------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstBitfield to be documented

.. das:method:: AstVisitor.preVisitExprConstInt8 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt8 to be documented

.. das:method:: AstVisitor.visitExprConstInt8 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const )  : ExpressionPtr

visitExprConstInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt8 <handle-ast-ExprConstInt8>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt8 to be documented

.. das:method:: AstVisitor.preVisitExprConstInt16 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt16 to be documented

.. das:method:: AstVisitor.visitExprConstInt16 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const )  : ExpressionPtr

visitExprConstInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt16 <handle-ast-ExprConstInt16>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt16 to be documented

.. das:method:: AstVisitor.preVisitExprConstInt64 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt64 to be documented

.. das:method:: AstVisitor.visitExprConstInt64 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const )  : ExpressionPtr

visitExprConstInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt64 <handle-ast-ExprConstInt64>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt64 to be documented

.. das:method:: AstVisitor.preVisitExprConstInt (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt> const ) 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt to be documented

.. das:method:: AstVisitor.visitExprConstInt (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt> const )  : ExpressionPtr

visitExprConstInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt <handle-ast-ExprConstInt>` > const+
+----+---------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt to be documented

.. das:method:: AstVisitor.preVisitExprConstInt2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt2 to be documented

.. das:method:: AstVisitor.visitExprConstInt2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const )  : ExpressionPtr

visitExprConstInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt2 <handle-ast-ExprConstInt2>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt2 to be documented

.. das:method:: AstVisitor.preVisitExprConstInt3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt3 to be documented

.. das:method:: AstVisitor.visitExprConstInt3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const )  : ExpressionPtr

visitExprConstInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt3 <handle-ast-ExprConstInt3>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt3 to be documented

.. das:method:: AstVisitor.preVisitExprConstInt4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstInt4 to be documented

.. das:method:: AstVisitor.visitExprConstInt4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const )  : ExpressionPtr

visitExprConstInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt4 <handle-ast-ExprConstInt4>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstInt4 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt8 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt8 to be documented

.. das:method:: AstVisitor.visitExprConstUInt8 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const )  : ExpressionPtr

visitExprConstUInt8 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt8 <handle-ast-ExprConstUInt8>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt8 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt16 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt16 to be documented

.. das:method:: AstVisitor.visitExprConstUInt16 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const )  : ExpressionPtr

visitExprConstUInt16 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt16 <handle-ast-ExprConstUInt16>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt16 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt64 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt64 to be documented

.. das:method:: AstVisitor.visitExprConstUInt64 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const )  : ExpressionPtr

visitExprConstUInt64 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt64 <handle-ast-ExprConstUInt64>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt64 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt to be documented

.. das:method:: AstVisitor.visitExprConstUInt (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const )  : ExpressionPtr

visitExprConstUInt returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt <handle-ast-ExprConstUInt>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt2 to be documented

.. das:method:: AstVisitor.visitExprConstUInt2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const )  : ExpressionPtr

visitExprConstUInt2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt2 <handle-ast-ExprConstUInt2>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt2 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt3 to be documented

.. das:method:: AstVisitor.visitExprConstUInt3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const )  : ExpressionPtr

visitExprConstUInt3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt3 <handle-ast-ExprConstUInt3>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt3 to be documented

.. das:method:: AstVisitor.preVisitExprConstUInt4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstUInt4 to be documented

.. das:method:: AstVisitor.visitExprConstUInt4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const )  : ExpressionPtr

visitExprConstUInt4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt4 <handle-ast-ExprConstUInt4>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstUInt4 to be documented

.. das:method:: AstVisitor.preVisitExprConstRange (self:AstVisitor; expr:smart_ptr<ast::ExprConstRange> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstRange to be documented

.. das:method:: AstVisitor.visitExprConstRange (self:AstVisitor; expr:smart_ptr<ast::ExprConstRange> const )  : ExpressionPtr

visitExprConstRange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstRange <handle-ast-ExprConstRange>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstRange to be documented

.. das:method:: AstVisitor.preVisitExprConstURange (self:AstVisitor; expr:smart_ptr<ast::ExprConstURange> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstURange to be documented

.. das:method:: AstVisitor.visitExprConstURange (self:AstVisitor; expr:smart_ptr<ast::ExprConstURange> const )  : ExpressionPtr

visitExprConstURange returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstURange <handle-ast-ExprConstURange>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstURange to be documented

.. das:method:: AstVisitor.preVisitExprConstBool (self:AstVisitor; expr:smart_ptr<ast::ExprConstBool> const ) 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstBool to be documented

.. das:method:: AstVisitor.visitExprConstBool (self:AstVisitor; expr:smart_ptr<ast::ExprConstBool> const )  : ExpressionPtr

visitExprConstBool returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBool <handle-ast-ExprConstBool>` > const+
+----+-----------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstBool to be documented

.. das:method:: AstVisitor.preVisitExprConstFloat (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const ) 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstFloat to be documented

.. das:method:: AstVisitor.visitExprConstFloat (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const )  : ExpressionPtr

visitExprConstFloat returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat <handle-ast-ExprConstFloat>` > const+
+----+-------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstFloat to be documented

.. das:method:: AstVisitor.preVisitExprConstFloat2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstFloat2 to be documented

.. das:method:: AstVisitor.visitExprConstFloat2 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const )  : ExpressionPtr

visitExprConstFloat2 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat2 <handle-ast-ExprConstFloat2>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstFloat2 to be documented

.. das:method:: AstVisitor.preVisitExprConstFloat3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstFloat3 to be documented

.. das:method:: AstVisitor.visitExprConstFloat3 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const )  : ExpressionPtr

visitExprConstFloat3 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat3 <handle-ast-ExprConstFloat3>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstFloat3 to be documented

.. das:method:: AstVisitor.preVisitExprConstFloat4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstFloat4 to be documented

.. das:method:: AstVisitor.visitExprConstFloat4 (self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const )  : ExpressionPtr

visitExprConstFloat4 returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat4 <handle-ast-ExprConstFloat4>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstFloat4 to be documented

.. das:method:: AstVisitor.preVisitExprConstString (self:AstVisitor; expr:smart_ptr<ast::ExprConstString> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstString to be documented

.. das:method:: AstVisitor.visitExprConstString (self:AstVisitor; expr:smart_ptr<ast::ExprConstString> const )  : ExpressionPtr

visitExprConstString returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstString <handle-ast-ExprConstString>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstString to be documented

.. das:method:: AstVisitor.preVisitExprConstDouble (self:AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprConstDouble to be documented

.. das:method:: AstVisitor.visitExprConstDouble (self:AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const )  : ExpressionPtr

visitExprConstDouble returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstDouble <handle-ast-ExprConstDouble>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprConstDouble to be documented

.. das:method:: AstVisitor.preVisitExprFakeContext (self:AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const ) 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprFakeContext to be documented

.. das:method:: AstVisitor.visitExprFakeContext (self:AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const )  : ExpressionPtr

visitExprFakeContext returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeContext <handle-ast-ExprFakeContext>` > const+
+----+---------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprFakeContext to be documented

.. das:method:: AstVisitor.preVisitExprFakeLineInfo (self:AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const ) 

arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+----+-----------------------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprFakeLineInfo to be documented

.. das:method:: AstVisitor.visitExprFakeLineInfo (self:AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const )  : ExpressionPtr

visitExprFakeLineInfo returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeLineInfo <handle-ast-ExprFakeLineInfo>` > const+
+----+-----------------------------------------------------------------------------+


method|ast|AstVisitor.visitExprFakeLineInfo to be documented

.. das:method:: AstVisitor.preVisitExprReader (self:AstVisitor; expr:smart_ptr<ast::ExprReader> const ) 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.preVisitExprReader to be documented

.. das:method:: AstVisitor.visitExprReader (self:AstVisitor; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr

visitExprReader returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle-ast-ExprReader>` > const+
+----+-----------------------------------------------------------------+


method|ast|AstVisitor.visitExprReader to be documented

.. das:function:: ExpressionPtr (expr:smart_ptr<auto(TT)> const )  : ExpressionPtr

ExpressionPtr returns  :ref:`ExpressionPtr <alias-ExpressionPtr>` 

arguments are

+----+-------------------------+
+expr+smart_ptr<auto(TT)> const+
+----+-------------------------+


function|ast|ExpressionPtr to be documented

.. das:function:: add_new_function_annotation (name:string const; someClassPtr:auto const )  : auto

add_new_function_annotation returns auto

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|add_new_function_annotation to be documented

.. das:function:: add_new_reader_macro (name:string const; someClassPtr:auto const )  : auto

add_new_reader_macro returns auto

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|add_new_reader_macro to be documented

.. das:function:: add_new_structure_annotation (name:string const; someClassPtr:auto const )  : auto

add_new_structure_annotation returns auto

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|add_new_structure_annotation to be documented

.. das:function:: add_new_variant_macro (name:string const; someClassPtr:auto const )  : auto

add_new_variant_macro returns auto

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|add_new_variant_macro to be documented

.. das:function:: describe (expr:smart_ptr<ast::Expression> const )  : auto

describe returns auto

arguments are

+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const+
+----+-----------------------------------------------------------------+


function|ast|describe to be documented

.. das:function:: describe (expr:smart_ptr<ast::Function> const )  : auto

describe returns auto

arguments are

+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const+
+----+-------------------------------------------------------------+


function|ast|describe to be documented

.. das:function:: describe (decl:smart_ptr<ast::TypeDecl> const; extra:bool const; contracts:bool const; modules:bool const )  : auto

describe returns auto

arguments are

+---------+-------------------------------------------------------------+
+decl     +smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const+
+---------+-------------------------------------------------------------+
+extra    +bool const                                                   +
+---------+-------------------------------------------------------------+
+contracts+bool const                                                   +
+---------+-------------------------------------------------------------+
+modules  +bool const                                                   +
+---------+-------------------------------------------------------------+


function|ast|describe to be documented

.. das:function:: make_function_annotation (name:string const; someClassPtr:auto const )  : FunctionAnnotationPtr

make_function_annotation returns  :ref:`FunctionAnnotationPtr <alias-FunctionAnnotationPtr>` 

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|make_function_annotation to be documented

.. das:function:: make_pass_macro (name:string const; someClassPtr:auto const )  : PassMacroPtr

make_pass_macro returns  :ref:`PassMacroPtr <alias-PassMacroPtr>` 

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|make_pass_macro to be documented

.. das:function:: make_reader_macro (name:string const; someClassPtr:auto const )  : ReaderMacroPtr

make_reader_macro returns  :ref:`ReaderMacroPtr <alias-ReaderMacroPtr>` 

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|make_reader_macro to be documented

.. das:function:: make_structure_annotation (name:string const; someClassPtr:auto const )  : StructureAnnotationPtr

make_structure_annotation returns  :ref:`StructureAnnotationPtr <alias-StructureAnnotationPtr>` 

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|make_structure_annotation to be documented

.. das:function:: make_variant_macro (name:string const; someClassPtr:auto const )  : VariantMacroPtr

make_variant_macro returns  :ref:`VariantMacroPtr <alias-VariantMacroPtr>` 

arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+


function|ast|make_variant_macro to be documented

.. das:function:: make_visitor (someClass:auto const )  : smart_ptr<ast::VisitorAdapter>

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` >

arguments are

+---------+----------+
+someClass+auto const+
+---------+----------+


function|ast|make_visitor to be documented

.. das:function:: add_dirty_infer_macro (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::PassMacro> const implicit ) 

arguments are

+----+------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit              +
+----+------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::PassMacro <handle-ast-PassMacro>` > const implicit+
+----+------------------------------------------------------------------------+


function|ast|add_dirty_infer_macro to be documented

.. das:function:: add_function (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::Function> const implicit ) 

arguments are

+----+----------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit            +
+----+----------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+----+----------------------------------------------------------------------+


function|ast|add_function to be documented

.. das:function:: add_function_annotation (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                +
+----+------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` > const implicit+
+----+------------------------------------------------------------------------------------------+


function|ast|add_function_annotation to be documented

.. das:function:: add_function_annotation (arg0:smart_ptr<ast::Function> const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit                    +
+----+------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` > const implicit+
+----+------------------------------------------------------------------------------------------+


function|ast|add_function_annotation to be documented

.. das:function:: add_reader_macro (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::ReaderMacro> const implicit ) 

arguments are

+----+----------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                  +
+----+----------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::ReaderMacro <handle-ast-ReaderMacro>` > const implicit+
+----+----------------------------------------------------------------------------+


function|ast|add_reader_macro to be documented

.. das:function:: add_structure_annotation (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit ) 

arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                  +
+----+--------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` > const implicit+
+----+--------------------------------------------------------------------------------------------+


function|ast|add_structure_annotation to be documented

.. das:function:: add_structure_annotation (arg0:smart_ptr<ast::Structure> const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit ) 

arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` > const implicit                    +
+----+--------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` > const implicit+
+----+--------------------------------------------------------------------------------------------+


function|ast|add_structure_annotation to be documented

.. das:function:: add_variant_macro (arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::VariantMacro> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                    +
+----+------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::VariantMacro <handle-ast-VariantMacro>` > const implicit+
+----+------------------------------------------------------------------------------+


function|ast|add_variant_macro to be documented

.. das:function:: any_array_foreach (arg0:void? const implicit; arg1:int const; arg2:block<(void?):void> const implicit ) 

arguments are

+----+----------------------------------+
+arg0+void? const implicit              +
+----+----------------------------------+
+arg1+int const                         +
+----+----------------------------------+
+arg2+block<(void?):void> const implicit+
+----+----------------------------------+


function|ast|any_array_foreach to be documented

.. das:function:: any_array_size (arg0:void? const implicit )  : int

any_array_size returns int

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+


function|ast|any_array_size to be documented

.. das:function:: any_table_foreach (arg0:void? const implicit; arg1:int const; arg2:int const; arg3:block<(void?;void?):void> const implicit ) 

arguments are

+----+----------------------------------------+
+arg0+void? const implicit                    +
+----+----------------------------------------+
+arg1+int const                               +
+----+----------------------------------------+
+arg2+int const                               +
+----+----------------------------------------+
+arg3+block<(void?;void?):void> const implicit+
+----+----------------------------------------+


function|ast|any_table_foreach to be documented

.. das:function:: any_table_size (arg0:void? const implicit )  : int

any_table_size returns int

arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+


function|ast|any_table_size to be documented

.. das:function:: clone_expression (arg0:smart_ptr<ast::Expression> const implicit )  : smart_ptr<ast::Expression>

clone_expression returns smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` >

arguments are

+----+--------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const implicit+
+----+--------------------------------------------------------------------------+


function|ast|clone_expression to be documented

.. das:function:: clone_type (arg0:smart_ptr<ast::TypeDecl> const implicit )  : smart_ptr<ast::TypeDecl>

clone_type returns smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+


function|ast|clone_type to be documented

.. das:function:: compiling_module ( )  : rtti::Module?

compiling_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

arguments are

+


function|ast|compiling_module to be documented

.. das:function:: compiling_program ( )  : smart_ptr<rtti::Program>

compiling_program returns smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >

arguments are

+


function|ast|compiling_program to be documented

.. das:function:: das_to_string (arg0:Type const )  : string

das_to_string returns string

arguments are

+----+------------------------------------------+
+arg0+ :ref:`rtti::Type <enum-rtti-Type>`  const+
+----+------------------------------------------+


function|ast|das_to_string to be documented

.. das:function:: describe_expression (arg0:smart_ptr<ast::Expression> const implicit )  : string

describe_expression returns string

arguments are

+----+--------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Expression <handle-ast-Expression>` > const implicit+
+----+--------------------------------------------------------------------------+


function|ast|describe_expression to be documented

.. das:function:: describe_function (arg0:smart_ptr<ast::Function> const implicit )  : string

describe_function returns string

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Function <handle-ast-Function>` > const implicit+
+----+----------------------------------------------------------------------+


function|ast|describe_function to be documented

.. das:function:: describe_typedecl (arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bool const; arg2:bool const; arg3:bool const )  : string

describe_typedecl returns string

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+bool const                                                            +
+----+----------------------------------------------------------------------+
+arg2+bool const                                                            +
+----+----------------------------------------------------------------------+
+arg3+bool const                                                            +
+----+----------------------------------------------------------------------+


function|ast|describe_typedecl to be documented

.. das:function:: find_bitfield_name (arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bitfield const )  : string

find_bitfield_name returns string

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+bitfield<> const                                                      +
+----+----------------------------------------------------------------------+


function|ast|find_bitfield_name to be documented

.. das:function:: for_each_enumeration (arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Enumeration>):void> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                +
+----+------------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Enumeration <handle-ast-Enumeration>` >):void> const implicit+
+----+------------------------------------------------------------------------------------------+


function|ast|for_each_enumeration to be documented

.. das:function:: for_each_field (arg0:BasicStructureAnnotation const implicit; arg1:block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit ) 

arguments are

+----+-------------------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  const implicit          +
+----+-------------------------------------------------------------------------------------------------------+
+arg1+block<(string;string;smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >;uint):void> const implicit+
+----+-------------------------------------------------------------------------------------------------------+


function|ast|for_each_field to be documented

.. das:function:: for_each_function (arg0:rtti::Module? const implicit; arg1:string const; arg2:block<(smart_ptr<ast::Function>):void> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+----+------------------------------------------------------------------------------------+
+arg1+string const                                                                        +
+----+------------------------------------------------------------------------------------+
+arg2+block<(smart_ptr< :ref:`ast::Function <handle-ast-Function>` >):void> const implicit+
+----+------------------------------------------------------------------------------------+


function|ast|for_each_function to be documented

.. das:function:: for_each_generic (arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Function>):void> const implicit ) 

arguments are

+----+------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                          +
+----+------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Function <handle-ast-Function>` >):void> const implicit+
+----+------------------------------------------------------------------------------------+


function|ast|for_each_generic to be documented

.. das:function:: for_each_structure (arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Structure>):void> const implicit ) 

arguments are

+----+--------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                            +
+----+--------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Structure <handle-ast-Structure>` >):void> const implicit+
+----+--------------------------------------------------------------------------------------+


function|ast|for_each_structure to be documented

.. das:function:: for_each_typedef (arg0:rtti::Module? const implicit; arg1:block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit ) 

arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle-rtti-Module>` ? const implicit                                  +
+----+--------------------------------------------------------------------------------------------+
+arg1+block<(string#;smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` >):void> const implicit+
+----+--------------------------------------------------------------------------------------------+


function|ast|for_each_typedef to be documented

.. das:function:: get_tuple_field_offset (arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const )  : int

get_tuple_field_offset returns int

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+int const                                                             +
+----+----------------------------------------------------------------------+


function|ast|get_tuple_field_offset to be documented

.. das:function:: get_variant_field_offset (arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const )  : int

get_variant_field_offset returns int

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+int const                                                             +
+----+----------------------------------------------------------------------+


function|ast|get_variant_field_offset to be documented

.. das:function:: is_same_type (arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:smart_ptr<ast::TypeDecl> const implicit; arg2:RefMatters const; arg3:ConstMatters const; arg4:TemporaryMatters const )  : bool

is_same_type returns bool

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::TypeDecl <handle-ast-TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg2+ :ref:`rtti::RefMatters <enum-rtti-RefMatters>`  const                +
+----+----------------------------------------------------------------------+
+arg3+ :ref:`rtti::ConstMatters <enum-rtti-ConstMatters>`  const            +
+----+----------------------------------------------------------------------+
+arg4+ :ref:`rtti::TemporaryMatters <enum-rtti-TemporaryMatters>`  const    +
+----+----------------------------------------------------------------------+


function|ast|is_same_type to be documented

.. das:function:: macro_error (arg0:smart_ptr<rtti::Program> const implicit; arg1:LineInfo const implicit; arg2:string const ) 

arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+ :ref:`rtti::LineInfo <handle-rtti-LineInfo>`  const implicit         +
+----+----------------------------------------------------------------------+
+arg2+string const                                                          +
+----+----------------------------------------------------------------------+


function|ast|macro_error to be documented

.. das:function:: make_function_annotation (arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::FunctionAnnotation>

make_function_annotation returns smart_ptr< :ref:`ast::FunctionAnnotation <handle-ast-FunctionAnnotation>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_function_annotation to be documented

.. das:function:: make_pass_macro (arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::PassMacro>

make_pass_macro returns smart_ptr< :ref:`ast::PassMacro <handle-ast-PassMacro>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_pass_macro to be documented

.. das:function:: make_reader_macro (arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::ReaderMacro>

make_reader_macro returns smart_ptr< :ref:`ast::ReaderMacro <handle-ast-ReaderMacro>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_reader_macro to be documented

.. das:function:: make_structure_annotation (arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::StructureAnnotation>

make_structure_annotation returns smart_ptr< :ref:`ast::StructureAnnotation <handle-ast-StructureAnnotation>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_structure_annotation to be documented

.. das:function:: make_variant_macro (arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::VariantMacro>

make_variant_macro returns smart_ptr< :ref:`ast::VariantMacro <handle-ast-VariantMacro>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_variant_macro to be documented

.. das:function:: make_visitor (arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit )  : smart_ptr<ast::VisitorAdapter>

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` >

arguments are

+----+------------------------------------------------------------------------+
+arg0+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg1+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|ast|make_visitor to be documented

.. das:function:: this_module ( )  : rtti::Module?

this_module returns  :ref:`rtti::Module <handle-rtti-Module>` ?

arguments are

+


function|ast|this_module to be documented

.. das:function:: this_program ( )  : smart_ptr<rtti::Program>

this_program returns smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` >

arguments are

+


function|ast|this_program to be documented

.. das:function:: visit (arg0:smart_ptr<rtti::Program> const implicit; arg1:smart_ptr<ast::VisitorAdapter> const implicit ) 

arguments are

+----+----------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`rtti::Program <handle-rtti-Program>` > const implicit            +
+----+----------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::VisitorAdapter <handle-ast-VisitorAdapter>` > const implicit+
+----+----------------------------------------------------------------------------------+


function|ast|visit to be documented


