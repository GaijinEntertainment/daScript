
.. _stdlib_ast:

==============================================
Compile time type  and AST information library
==============================================

++++++++++++
Type aliases
++++++++++++

.. _alias_EnumerationPtr:

.. das:attribute:: EnumerationPtr = EnumerationPtr

to be documented

.. _alias_ExprAscendFlags:

.. das:attribute:: ExprAscendFlags is a bitfield

+------------+-+-+
+useStackRef +0+1+
+------------+-+-+
+needTypeInfo+1+2+
+------------+-+-+


to be documented

.. _alias_ExprAtFlags:

.. das:attribute:: ExprAtFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


to be documented

.. _alias_ExprBlockFlags:

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


to be documented

.. _alias_ExprCastFlags:

.. das:attribute:: ExprCastFlags is a bitfield

+---------------+-+-+
+upcastCast     +0+1+
+---------------+-+-+
+reinterpretCast+1+2+
+---------------+-+-+


to be documented

.. _alias_ExprFieldDerefFlags:

.. das:attribute:: ExprFieldDerefFlags is a bitfield

+------------------+-+-+
+unsafeDeref       +0+1+
+------------------+-+-+
+ignoreCaptureConst+1+2+
+------------------+-+-+


to be documented

.. _alias_ExprFieldFieldFlags:

.. das:attribute:: ExprFieldFieldFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


to be documented

.. _alias_ExprFlags:

.. das:attribute:: ExprFlags is a bitfield

+-------------------+-+-+
+constexpression    +0+1+
+-------------------+-+-+
+noSideEffects      +1+2+
+-------------------+-+-+
+noNativeSideEffects+2+4+
+-------------------+-+-+


to be documented

.. _alias_ExprGenFlags:

.. das:attribute:: ExprGenFlags is a bitfield

+----------+-+-+
+alwaysSafe+0+1+
+----------+-+-+
+generated +1+2+
+----------+-+-+


to be documented

.. _alias_ExprMakeBlockFlags:

.. das:attribute:: ExprMakeBlockFlags is a bitfield

+---------------+-+-+
+isLambda       +0+1+
+---------------+-+-+
+isLocalFunction+1+2+
+---------------+-+-+


to be documented

.. _alias_ExprMakeLocalFlags:

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


to be documented

.. _alias_ExprMakeStructFlags:

.. das:attribute:: ExprMakeStructFlags is a bitfield

+--------------+-+-+
+useInitializer+0+1+
+--------------+-+-+
+isNewHandle   +1+2+
+--------------+-+-+


to be documented

.. _alias_ExprPrintFlags:

.. das:attribute:: ExprPrintFlags is a bitfield

+-----------+-+-+
+topLevel   +0+1+
+-----------+-+-+
+argLevel   +1+2+
+-----------+-+-+
+bottomLevel+2+4+
+-----------+-+-+


to be documented

.. _alias_ExprReturnFlags:

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


to be documented

.. _alias_ExprSwizzleFieldFlags:

.. das:attribute:: ExprSwizzleFieldFlags is a bitfield

+-----+-+-+
+r2v  +0+1+
+-----+-+-+
+r2cr +1+2+
+-----+-+-+
+write+2+4+
+-----+-+-+


to be documented

.. _alias_ExprVarFlags:

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


to be documented

.. _alias_ExprYieldFlags:

.. das:attribute:: ExprYieldFlags is a bitfield

+-------------+-+-+
+moveSemantics+0+1+
+-------------+-+-+


to be documented

.. _alias_ExpressionPtr:

.. das:attribute:: ExpressionPtr = ExpressionPtr

to be documented

.. _alias_FieldDeclarationFlags:

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


to be documented

.. _alias_FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = FunctionAnnotationPtr

to be documented

.. _alias_FunctionFlags:

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


to be documented

.. _alias_FunctionPtr:

.. das:attribute:: FunctionPtr = FunctionPtr

to be documented

.. _alias_FunctionSideEffectFlags:

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


to be documented

.. _alias_MakeFieldDeclFlags:

.. das:attribute:: MakeFieldDeclFlags is a bitfield

+--------------+-+-+
+moveSemantics +0+1+
+--------------+-+-+
+cloneSemantics+1+2+
+--------------+-+-+


to be documented

.. _alias_MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = MakeFieldDeclPtr

to be documented

.. _alias_PassMacroPtr:

.. das:attribute:: PassMacroPtr = PassMacroPtr

to be documented

.. _alias_ProgramPtr:

.. das:attribute:: ProgramPtr = ProgramPtr

to be documented

.. _alias_ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = ReaderMacroPtr

to be documented

.. _alias_StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = StructureAnnotationPtr

to be documented

.. _alias_StructureFlags:

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


to be documented

.. _alias_StructurePtr:

.. das:attribute:: StructurePtr = StructurePtr

to be documented

.. _alias_TypeDeclFlags:

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


to be documented

.. _alias_TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = TypeDeclPtr

to be documented

.. _alias_VariableAccessFlags:

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


to be documented

.. _alias_VariableFlags:

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


to be documented

.. _alias_VariablePtr:

.. das:attribute:: VariablePtr = VariablePtr

to be documented

.. _alias_VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = VariantMacroPtr

to be documented

++++++++++++
Enumerations
++++++++++++

.. _enum_ast_SideEffects:

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

.. _handle_ast_Enumeration:

.. das:attribute:: Enumeration

Enumeration fields are

+--------+------------------------------------------------------------------------------------------------------------------------+
+_module + :ref:`rtti::Module <handle_rtti_Module>` ?                                                                             +
+--------+------------------------------------------------------------------------------------------------------------------------+
+at      + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                                                           +
+--------+------------------------------------------------------------------------------------------------------------------------+
+cppName + :ref:`builtin::das_string <handle_builtin_das_string>`                                                                 +
+--------+------------------------------------------------------------------------------------------------------------------------+
+list    + :ref:`ast::dasvector`pair`das_string`smart_ptr`Expression <handle_ast_dasvector`pair`das_string`smart_ptr`Expression>` +
+--------+------------------------------------------------------------------------------------------------------------------------+
+name    + :ref:`builtin::das_string <handle_builtin_das_string>`                                                                 +
+--------+------------------------------------------------------------------------------------------------------------------------+
+external+bool                                                                                                                    +
+--------+------------------------------------------------------------------------------------------------------------------------+
+baseType+ :ref:`rtti::Type <enum_rtti_Type>`                                                                                     +
+--------+------------------------------------------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprAddr:

.. das:attribute:: ExprAddr

ExprAddr fields are

+----------+--------------------------------------------------------+
+func      + :ref:`ast::Function <handle_ast_Function>` ?           +
+----------+--------------------------------------------------------+
+target    + :ref:`builtin::das_string <handle_builtin_das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`           +
+----------+--------------------------------------------------------+
+funcType  +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                     +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+


to be documented


.. _handle_ast_ExprArrayComprehension:

.. das:attribute:: ExprArrayComprehension

ExprArrayComprehension fields are

+---------------+-----------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+---------------+-----------------------------------------------------------+
+printFlags     + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+---------------+-----------------------------------------------------------+
+generatorSyntax+bool                                                       +
+---------------+-----------------------------------------------------------+
+subexpr        +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+---------------+-----------------------------------------------------------+
+genFlags       + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+---------------+-----------------------------------------------------------+
+exprFor        +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+---------------+-----------------------------------------------------------+
+exprWhere      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+---------------+-----------------------------------------------------------+
+_type          +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+---------------+-----------------------------------------------------------+
+flags          + :ref:`ExprFlags <alias_ExprFlags>`                        +
+---------------+-----------------------------------------------------------+
+__rtti         +string const                                               +
+---------------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprAsVariant:

.. das:attribute:: ExprAsVariant

ExprAsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias_ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias_ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprAscend:

.. das:attribute:: ExprAscend

ExprAscend fields are

+-----------+-----------------------------------------------------------+
+ascType    +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+stackTop   +uint                                                       +
+-----------+-----------------------------------------------------------+
+ascendFlags+ :ref:`ExprAscendFlags <alias_ExprAscendFlags>`            +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprAssert:

.. das:attribute:: ExprAssert

ExprAssert fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+isVerify              +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprAt:

.. das:attribute:: ExprAt

ExprAt fields are

+----------+-----------------------------------------------------------+
+index     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+atFlags   + :ref:`ExprAtFlags <alias_ExprAtFlags>`                    +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprBlock:

.. das:attribute:: ExprBlock

ExprBlock fields are

+-----------------+----------------------------------------------------------------------------------------+
+stackVarBottom   +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+annotationDataSid+uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+arguments        + :ref:`ast::dasvector`smart_ptr`Variable <handle_ast_dasvector`smart_ptr`Variable>`     +
+-----------------+----------------------------------------------------------------------------------------+
+at               + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+stackCleanVars   + :ref:`ast::dasvector`pair`uint`uint <handle_ast_dasvector`pair`uint`uint>`             +
+-----------------+----------------------------------------------------------------------------------------+
+list             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+returnType       +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+printFlags       + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+annotations      + :ref:`rtti::AnnotationList <handle_rtti_AnnotationList>`                               +
+-----------------+----------------------------------------------------------------------------------------+
+stackTop         +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+maxLabelIndex    +int                                                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+blockFlags       + :ref:`ExprBlockFlags <alias_ExprBlockFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+finalList        + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+genFlags         + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+-----------------+----------------------------------------------------------------------------------------+
+annotationData   +uint64                                                                                  +
+-----------------+----------------------------------------------------------------------------------------+
+stackVarTop      +uint                                                                                    +
+-----------------+----------------------------------------------------------------------------------------+
+flags            + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+_type            +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+__rtti           +string const                                                                            +
+-----------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprBreak:

.. das:attribute:: ExprBreak

ExprBreak fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprCall:

.. das:attribute:: ExprCall

ExprCall fields are

+----------------------+----------------------------------------------------------------------------------------+
+func                  + :ref:`ast::Function <handle_ast_Function>` ?                                           +
+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+doesNotNeedSp         +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprCast:

.. das:attribute:: ExprCast

ExprCast fields are

+----------+-----------------------------------------------------------+
+castFlags + :ref:`ExprCastFlags <alias_ExprCastFlags>`                +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+castType  +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprClone:

.. das:attribute:: ExprClone

ExprClone fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprConst:

.. das:attribute:: ExprConst

ExprConst fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstBitfield:

.. das:attribute:: ExprConstBitfield

ExprConstBitfield fields are

+------------+-------------------------------------------------------+
+value       +bitfield<>                                             +
+------------+-------------------------------------------------------+
+at          + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+------------+-------------------------------------------------------+
+bitfieldType+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+------------+-------------------------------------------------------+
+printFlags  + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+------------+-------------------------------------------------------+
+genFlags    + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+------------+-------------------------------------------------------+
+baseType    + :ref:`rtti::Type <enum_rtti_Type>`                    +
+------------+-------------------------------------------------------+
+_type       +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+------------+-------------------------------------------------------+
+flags       + :ref:`ExprFlags <alias_ExprFlags>`                    +
+------------+-------------------------------------------------------+
+__rtti      +string const                                           +
+------------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstBool:

.. das:attribute:: ExprConstBool

ExprConstBool fields are

+----------+-------------------------------------------------------+
+value     +bool                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstDouble:

.. das:attribute:: ExprConstDouble

ExprConstDouble fields are

+----------+-------------------------------------------------------+
+value     +double                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstEnumeration:

.. das:attribute:: ExprConstEnumeration

ExprConstEnumeration fields are

+----------+-------------------------------------------------------------+
+value     + :ref:`builtin::das_string <handle_builtin_das_string>`      +
+----------+-------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                +
+----------+-------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                +
+----------+-------------------------------------------------------------+
+enumType  +smart_ptr< :ref:`ast::Enumeration <handle_ast_Enumeration>` >+
+----------+-------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                    +
+----------+-------------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                          +
+----------+-------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >      +
+----------+-------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                          +
+----------+-------------------------------------------------------------+
+__rtti    +string const                                                 +
+----------+-------------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstFloat:

.. das:attribute:: ExprConstFloat

ExprConstFloat fields are

+----------+-------------------------------------------------------+
+value     +float                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstFloat2:

.. das:attribute:: ExprConstFloat2

ExprConstFloat2 fields are

+----------+-------------------------------------------------------+
+value     +float2                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstFloat3:

.. das:attribute:: ExprConstFloat3

ExprConstFloat3 fields are

+----------+-------------------------------------------------------+
+value     +float3                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstFloat4:

.. das:attribute:: ExprConstFloat4

ExprConstFloat4 fields are

+----------+-------------------------------------------------------+
+value     +float4                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt:

.. das:attribute:: ExprConstInt

ExprConstInt fields are

+----------+-------------------------------------------------------+
+value     +int                                                    +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt16:

.. das:attribute:: ExprConstInt16

ExprConstInt16 fields are

+----------+-------------------------------------------------------+
+value     +int16                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt2:

.. das:attribute:: ExprConstInt2

ExprConstInt2 fields are

+----------+-------------------------------------------------------+
+value     +int2                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt3:

.. das:attribute:: ExprConstInt3

ExprConstInt3 fields are

+----------+-------------------------------------------------------+
+value     +int3                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt4:

.. das:attribute:: ExprConstInt4

ExprConstInt4 fields are

+----------+-------------------------------------------------------+
+value     +int4                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt64:

.. das:attribute:: ExprConstInt64

ExprConstInt64 fields are

+----------+-------------------------------------------------------+
+value     +int64                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstInt8:

.. das:attribute:: ExprConstInt8

ExprConstInt8 fields are

+----------+-------------------------------------------------------+
+value     +int8                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstPtr:

.. das:attribute:: ExprConstPtr

ExprConstPtr fields are

+----------+-------------------------------------------------------+
+value     +void?                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstRange:

.. das:attribute:: ExprConstRange

ExprConstRange fields are

+----------+-------------------------------------------------------+
+value     +range                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstString:

.. das:attribute:: ExprConstString

ExprConstString fields are

+----------+--------------------------------------------------------+
+value     + :ref:`builtin::das_string <handle_builtin_das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`           +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                     +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                     +
+----------+--------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt:

.. das:attribute:: ExprConstUInt

ExprConstUInt fields are

+----------+-------------------------------------------------------+
+value     +uint                                                   +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt16:

.. das:attribute:: ExprConstUInt16

ExprConstUInt16 fields are

+----------+-------------------------------------------------------+
+value     +uint16                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt2:

.. das:attribute:: ExprConstUInt2

ExprConstUInt2 fields are

+----------+-------------------------------------------------------+
+value     +uint2                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt3:

.. das:attribute:: ExprConstUInt3

ExprConstUInt3 fields are

+----------+-------------------------------------------------------+
+value     +uint3                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt4:

.. das:attribute:: ExprConstUInt4

ExprConstUInt4 fields are

+----------+-------------------------------------------------------+
+value     +uint4                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt64:

.. das:attribute:: ExprConstUInt64

ExprConstUInt64 fields are

+----------+-------------------------------------------------------+
+value     +uint64                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstUInt8:

.. das:attribute:: ExprConstUInt8

ExprConstUInt8 fields are

+----------+-------------------------------------------------------+
+value     +uint8                                                  +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprConstURange:

.. das:attribute:: ExprConstURange

ExprConstURange fields are

+----------+-------------------------------------------------------+
+value     +urange                                                 +
+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprContinue:

.. das:attribute:: ExprContinue

ExprContinue fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprCopy:

.. das:attribute:: ExprCopy

ExprCopy fields are

+------------------+-----------------------------------------------------------+
+takeOverRightStack+bool                                                       +
+------------------+-----------------------------------------------------------+
+right             +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+------------------+-----------------------------------------------------------+
+at                + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+------------------+-----------------------------------------------------------+
+op                + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+------------------+-----------------------------------------------------------+
+printFlags        + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+------------------+-----------------------------------------------------------+
+genFlags          + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+------------------+-----------------------------------------------------------+
+_type             +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+------------------+-----------------------------------------------------------+
+flags             + :ref:`ExprFlags <alias_ExprFlags>`                        +
+------------------+-----------------------------------------------------------+
+__rtti            +string const                                               +
+------------------+-----------------------------------------------------------+
+left              +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+------------------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprDebug:

.. das:attribute:: ExprDebug

ExprDebug fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprDelete:

.. das:attribute:: ExprDelete

ExprDelete fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+native    +bool                                                       +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprErase:

.. das:attribute:: ExprErase

ExprErase fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprFakeContext:

.. das:attribute:: ExprFakeContext

ExprFakeContext fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprFakeLineInfo:

.. das:attribute:: ExprFakeLineInfo

ExprFakeLineInfo fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                    +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprField:

.. das:attribute:: ExprField

ExprField fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias_ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias_ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprFind:

.. das:attribute:: ExprFind

ExprFind fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprFor:

.. das:attribute:: ExprFor

ExprFor fields are

+-----------------+----------------------------------------------------------------------------------------+
+at               + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+body             +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >                             +
+-----------------+----------------------------------------------------------------------------------------+
+iteratorsAt      + :ref:`ast::dasvector`LineInfo <handle_ast_dasvector`LineInfo>`                         +
+-----------------+----------------------------------------------------------------------------------------+
+printFlags       + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+-----------------+----------------------------------------------------------------------------------------+
+iterators        + :ref:`ast::dasvector`das_string <handle_ast_dasvector`das_string>`                     +
+-----------------+----------------------------------------------------------------------------------------+
+iteratorVariables+ :ref:`ast::dasvector`smart_ptr`Variable <handle_ast_dasvector`smart_ptr`Variable>`     +
+-----------------+----------------------------------------------------------------------------------------+
+genFlags         + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+-----------------+----------------------------------------------------------------------------------------+
+sources          + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+-----------------+----------------------------------------------------------------------------------------+
+_type            +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------------+----------------------------------------------------------------------------------------+
+flags            + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+-----------------+----------------------------------------------------------------------------------------+
+__rtti           +string const                                                                            +
+-----------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprGoto:

.. das:attribute:: ExprGoto

ExprGoto fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+labelName +int                                                        +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprIfThenElse:

.. das:attribute:: ExprIfThenElse

ExprIfThenElse fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+if_false  +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+isStatic  +bool                                                       +
+----------+-----------------------------------------------------------+
+cond      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+if_true   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprInvoke:

.. das:attribute:: ExprInvoke

ExprInvoke fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+doesNotNeedSp         +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprIs:

.. das:attribute:: ExprIs

ExprIs fields are

+----------+-----------------------------------------------------------+
+typeexpr  +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprIsVariant:

.. das:attribute:: ExprIsVariant

ExprIsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias_ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias_ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprKeyExists:

.. das:attribute:: ExprKeyExists

ExprKeyExists fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprLabel:

.. das:attribute:: ExprLabel

ExprLabel fields are

+----------+--------------------------------------------------------+
+comment   + :ref:`builtin::das_string <handle_builtin_das_string>` +
+----------+--------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`           +
+----------+--------------------------------------------------------+
+labelName +int                                                     +
+----------+--------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`           +
+----------+--------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`               +
+----------+--------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+----------+--------------------------------------------------------+
+__rtti    +string const                                            +
+----------+--------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                     +
+----------+--------------------------------------------------------+


to be documented


.. _handle_ast_ExprLet:

.. das:attribute:: ExprLet

ExprLet fields are

+----------+------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                       +
+----------+------------------------------------------------------------------------------------+
+inScope   +bool                                                                                +
+----------+------------------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                       +
+----------+------------------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                           +
+----------+------------------------------------------------------------------------------------+
+variables + :ref:`ast::dasvector`smart_ptr`Variable <handle_ast_dasvector`smart_ptr`Variable>` +
+----------+------------------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                             +
+----------+------------------------------------------------------------------------------------+
+__rtti    +string const                                                                        +
+----------+------------------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                                 +
+----------+------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprLooksLikeCall:

.. das:attribute:: ExprLooksLikeCall

ExprLooksLikeCall fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeArray:

.. das:attribute:: ExprMakeArray

ExprMakeArray fields are

+-----------+----------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+values     + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+-----------+----------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+recordType +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias_ExprMakeLocalFlags>`                                   +
+-----------+----------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+-----------+----------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------+
+__rtti     +string const                                                                            +
+-----------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

ExprMakeBlock fields are

+----------+-----------------------------------------------------------+
+mmFlags   + :ref:`ExprMakeBlockFlags <alias_ExprMakeBlockFlags>`      +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+stackTop  +uint                                                       +
+----------+-----------------------------------------------------------+
+block     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

ExprMakeGenerator fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+iterType              +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeLocal:

.. das:attribute:: ExprMakeLocal

ExprMakeLocal fields are

+-----------+-------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+-----------+-------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+-----------+-------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+-----------+-------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias_ExprMakeLocalFlags>`  +
+-----------+-------------------------------------------------------+
+stackTop   +uint                                                   +
+-----------+-------------------------------------------------------+
+extraOffset+uint                                                   +
+-----------+-------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+-----------+-------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+-----------+-------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                    +
+-----------+-------------------------------------------------------+
+__rtti     +string const                                           +
+-----------+-------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeStruct:

.. das:attribute:: ExprMakeStruct

ExprMakeStruct fields are

+---------------+----------------------------------------------------------------------------------------+
+makeType       +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+---------------+----------------------------------------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+---------------+----------------------------------------------------------------------------------------+
+structs        + :ref:`ast::dasvector`smart_ptr`MakeStruct <handle_ast_dasvector`smart_ptr`MakeStruct>` +
+---------------+----------------------------------------------------------------------------------------+
+printFlags     + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+---------------+----------------------------------------------------------------------------------------+
+makeFlags      + :ref:`ExprMakeLocalFlags <alias_ExprMakeLocalFlags>`                                   +
+---------------+----------------------------------------------------------------------------------------+
+stackTop       +uint                                                                                    +
+---------------+----------------------------------------------------------------------------------------+
+extraOffset    +uint                                                                                    +
+---------------+----------------------------------------------------------------------------------------+
+genFlags       + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+---------------+----------------------------------------------------------------------------------------+
+_type          +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+---------------+----------------------------------------------------------------------------------------+
+flags          + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+---------------+----------------------------------------------------------------------------------------+
+__rtti         +string const                                                                            +
+---------------+----------------------------------------------------------------------------------------+
+makeStructFlags+ :ref:`ExprMakeStructFlags <alias_ExprMakeStructFlags>`                                 +
+---------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeTuple:

.. das:attribute:: ExprMakeTuple

ExprMakeTuple fields are

+-----------+----------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+values     + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+-----------+----------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+recordType +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+-----------+----------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias_ExprMakeLocalFlags>`                                   +
+-----------+----------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+-----------+----------------------------------------------------------------------------------------+
+isKeyValue +bool                                                                                    +
+-----------+----------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+-----------+----------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------+
+__rtti     +string const                                                                            +
+-----------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMakeVariant:

.. das:attribute:: ExprMakeVariant

ExprMakeVariant fields are

+-----------+----------------------------------------------------------------------------------------------+
+variants   + :ref:`ast::dasvector`smart_ptr`MakeFieldDecl <handle_ast_dasvector`smart_ptr`MakeFieldDecl>` +
+-----------+----------------------------------------------------------------------------------------------+
+makeType   +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                       +
+-----------+----------------------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                                 +
+-----------+----------------------------------------------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                                 +
+-----------+----------------------------------------------------------------------------------------------+
+makeFlags  + :ref:`ExprMakeLocalFlags <alias_ExprMakeLocalFlags>`                                         +
+-----------+----------------------------------------------------------------------------------------------+
+stackTop   +uint                                                                                          +
+-----------+----------------------------------------------------------------------------------------------+
+extraOffset+uint                                                                                          +
+-----------+----------------------------------------------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                                     +
+-----------+----------------------------------------------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                       +
+-----------+----------------------------------------------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                                                           +
+-----------+----------------------------------------------------------------------------------------------+
+__rtti     +string const                                                                                  +
+-----------+----------------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMemZero:

.. das:attribute:: ExprMemZero

ExprMemZero fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprMove:

.. das:attribute:: ExprMove

ExprMove fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprNamedCall:

.. das:attribute:: ExprNamedCall

ExprNamedCall fields are

+----------------------+--------------------------------------------------------+
+arguments             + :ref:`ast::MakeStruct <handle_ast_MakeStruct>`         +
+----------------------+--------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`           +
+----------------------+--------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`           +
+----------------------+--------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>` +
+----------------------+--------------------------------------------------------+
+argumentsFailedToInfer+bool                                                    +
+----------------------+--------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`               +
+----------------------+--------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+----------------------+--------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                     +
+----------------------+--------------------------------------------------------+
+__rtti                +string const                                            +
+----------------------+--------------------------------------------------------+


to be documented


.. _handle_ast_ExprNew:

.. das:attribute:: ExprNew

ExprNew fields are

+----------------------+----------------------------------------------------------------------------------------+
+func                  + :ref:`ast::Function <handle_ast_Function>` ?                                           +
+----------------------+----------------------------------------------------------------------------------------+
+typeexpr              +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+initializer           +bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+stackTop              +uint                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprNullCoalescing:

.. das:attribute:: ExprNullCoalescing

ExprNullCoalescing fields are

+------------+-----------------------------------------------------------+
+defaultValue+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+------------+-----------------------------------------------------------+
+at          + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+------------+-----------------------------------------------------------+
+printFlags  + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+------------+-----------------------------------------------------------+
+subexpr     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+------------+-----------------------------------------------------------+
+genFlags    + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+------------+-----------------------------------------------------------+
+_type       +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+------------+-----------------------------------------------------------+
+__rtti      +string const                                               +
+------------+-----------------------------------------------------------+
+flags       + :ref:`ExprFlags <alias_ExprFlags>`                        +
+------------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprOp1:

.. das:attribute:: ExprOp1

ExprOp1 fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprOp2:

.. das:attribute:: ExprOp2

ExprOp2 fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprOp3:

.. das:attribute:: ExprOp3

ExprOp3 fields are

+----------+-----------------------------------------------------------+
+right     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+op        + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+left      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprPtr2Ref:

.. das:attribute:: ExprPtr2Ref

ExprPtr2Ref fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprReader:

.. das:attribute:: ExprReader

ExprReader fields are

+----------+-------------------------------------------------------------+
+macro     +smart_ptr< :ref:`ast::ReaderMacro <handle_ast_ReaderMacro>` >+
+----------+-------------------------------------------------------------+
+sequence  + :ref:`builtin::das_string <handle_builtin_das_string>`      +
+----------+-------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                +
+----------+-------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                +
+----------+-------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                    +
+----------+-------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >      +
+----------+-------------------------------------------------------------+
+__rtti    +string const                                                 +
+----------+-------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                          +
+----------+-------------------------------------------------------------+


to be documented


.. _handle_ast_ExprRef2Ptr:

.. das:attribute:: ExprRef2Ptr

ExprRef2Ptr fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprRef2Value:

.. das:attribute:: ExprRef2Value

ExprRef2Value fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprReturn:

.. das:attribute:: ExprReturn

ExprReturn fields are

+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+stackTop   +uint                                                       +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----------+-----------------------------------------------------------+
+block      + :ref:`ast::ExprBlock <handle_ast_ExprBlock>` ?            +
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+refStackTop+uint                                                       +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+returnFlags+ :ref:`ExprReturnFlags <alias_ExprReturnFlags>`            +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprSafeAsVariant:

.. das:attribute:: ExprSafeAsVariant

ExprSafeAsVariant fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias_ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+skipQQ    +bool                                                                     +
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias_ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprSafeAt:

.. das:attribute:: ExprSafeAt

ExprSafeAt fields are

+----------+-----------------------------------------------------------+
+index     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+atFlags   + :ref:`ExprAtFlags <alias_ExprAtFlags>`                    +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprSafeField:

.. das:attribute:: ExprSafeField

ExprSafeField fields are

+----------+-------------------------------------------------------------------------+
+annotation+smart_ptr< :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` >    +
+----------+-------------------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >              +
+----------+-------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+fieldIndex+int                                                                      +
+----------+-------------------------------------------------------------------------+
+fieldFlags+ :ref:`ExprFieldFieldFlags <alias_ExprFieldFieldFlags>`                  +
+----------+-------------------------------------------------------------------------+
+field     + :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const? const+
+----------+-------------------------------------------------------------------------+
+skipQQ    +bool                                                                     +
+----------+-------------------------------------------------------------------------+
+derefFlags+ :ref:`ExprFieldDerefFlags <alias_ExprFieldDerefFlags>`                  +
+----------+-------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                            +
+----------+-------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                  +
+----------+-------------------------------------------------------------------------+
+atField   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                            +
+----------+-------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                +
+----------+-------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                  +
+----------+-------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                      +
+----------+-------------------------------------------------------------------------+
+__rtti    +string const                                                             +
+----------+-------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprStaticAssert:

.. das:attribute:: ExprStaticAssert

ExprStaticAssert fields are

+----------------------+----------------------------------------------------------------------------------------+
+arguments             + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------------------+----------------------------------------------------------------------------------------+
+at                    + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+printFlags            + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------------------+----------------------------------------------------------------------------------------+
+name                  + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------------------+----------------------------------------------------------------------------------------+
+argumentsFailedToInfer+bool                                                                                    +
+----------------------+----------------------------------------------------------------------------------------+
+genFlags              + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------------------+----------------------------------------------------------------------------------------+
+_type                 +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------------------+----------------------------------------------------------------------------------------+
+flags                 + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------------------+----------------------------------------------------------------------------------------+
+__rtti                +string const                                                                            +
+----------------------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprStringBuilder:

.. das:attribute:: ExprStringBuilder

ExprStringBuilder fields are

+----------+----------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------+----------------------------------------------------------------------------------------+
+elements  + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------+----------------------------------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`                                           +
+----------+----------------------------------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                                               +
+----------+----------------------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+__rtti    +string const                                                                            +
+----------+----------------------------------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                                                     +
+----------+----------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_ExprSwizzle:

.. das:attribute:: ExprSwizzle

ExprSwizzle fields are

+----------+------------------------------------------------------------+
+value     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` > +
+----------+------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`               +
+----------+------------------------------------------------------------+
+fieldFlags+ :ref:`ExprSwizzleFieldFlags <alias_ExprSwizzleFieldFlags>` +
+----------+------------------------------------------------------------+
+mask      + :ref:`builtin::das_string <handle_builtin_das_string>`     +
+----------+------------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`               +
+----------+------------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                   +
+----------+------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >     +
+----------+------------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                         +
+----------+------------------------------------------------------------+
+__rtti    +string const                                                +
+----------+------------------------------------------------------------+
+fields    + :ref:`ast::dasvector`uint8 <handle_ast_dasvector`uint8>`   +
+----------+------------------------------------------------------------+


to be documented


.. _handle_ast_ExprTryCatch:

.. das:attribute:: ExprTryCatch

ExprTryCatch fields are

+-----------+-----------------------------------------------------------+
+try_block  +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+-----------+-----------------------------------------------------------+
+catch_block+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                        +
+-----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprTypeInfo:

.. das:attribute:: ExprTypeInfo

ExprTypeInfo fields are

+----------+-----------------------------------------------------------+
+typeexpr  +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+extratrait+ :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+macro     + :ref:`ast::TypeInfoMacro <handle_ast_TypeInfoMacro>` ?    +
+----------+-----------------------------------------------------------+
+subtrait  + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+trait     + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+subexpr   +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprVar:

.. das:attribute:: ExprVar

ExprVar fields are

+-------------+--------------------------------------------------------+
+at           + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`           +
+-------------+--------------------------------------------------------+
+variable     +smart_ptr< :ref:`ast::Variable <handle_ast_Variable>` > +
+-------------+--------------------------------------------------------+
+varFlags     + :ref:`ExprVarFlags <alias_ExprVarFlags>`               +
+-------------+--------------------------------------------------------+
+printFlags   + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`           +
+-------------+--------------------------------------------------------+
+argumentIndex+int                                                     +
+-------------+--------------------------------------------------------+
+name         + :ref:`builtin::das_string <handle_builtin_das_string>` +
+-------------+--------------------------------------------------------+
+genFlags     + :ref:`ExprGenFlags <alias_ExprGenFlags>`               +
+-------------+--------------------------------------------------------+
+pBlock       + :ref:`ast::ExprBlock <handle_ast_ExprBlock>` ?         +
+-------------+--------------------------------------------------------+
+_type        +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > +
+-------------+--------------------------------------------------------+
+flags        + :ref:`ExprFlags <alias_ExprFlags>`                     +
+-------------+--------------------------------------------------------+
+__rtti       +string const                                            +
+-------------+--------------------------------------------------------+


to be documented


.. _handle_ast_ExprWhile:

.. das:attribute:: ExprWhile

ExprWhile fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+body      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+cond      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprWith:

.. das:attribute:: ExprWith

ExprWith fields are

+----------+-----------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+----------+-----------------------------------------------------------+
+body      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+----------+-----------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+----------+-----------------------------------------------------------+
+_with     +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+----------+-----------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+----------+-----------------------------------------------------------+
+__rtti    +string const                                               +
+----------+-----------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                        +
+----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_ExprYield:

.. das:attribute:: ExprYield

ExprYield fields are

+-----------+-----------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+-----------+-----------------------------------------------------------+
+printFlags + :ref:`ExprPrintFlags <alias_ExprPrintFlags>`              +
+-----------+-----------------------------------------------------------+
+subexpr    +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----------+-----------------------------------------------------------+
+genFlags   + :ref:`ExprGenFlags <alias_ExprGenFlags>`                  +
+-----------+-----------------------------------------------------------+
+_type      +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >    +
+-----------+-----------------------------------------------------------+
+__rtti     +string const                                               +
+-----------+-----------------------------------------------------------+
+flags      + :ref:`ExprFlags <alias_ExprFlags>`                        +
+-----------+-----------------------------------------------------------+
+returnFlags+ :ref:`ExprYieldFlags <alias_ExprYieldFlags>`              +
+-----------+-----------------------------------------------------------+


to be documented


.. _handle_ast_Expression:

.. das:attribute:: Expression

Expression fields are

+----------+-------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`          +
+----------+-------------------------------------------------------+
+printFlags+ :ref:`ExprPrintFlags <alias_ExprPrintFlags>`          +
+----------+-------------------------------------------------------+
+genFlags  + :ref:`ExprGenFlags <alias_ExprGenFlags>`              +
+----------+-------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >+
+----------+-------------------------------------------------------+
+__rtti    +string const                                           +
+----------+-------------------------------------------------------+
+flags     + :ref:`ExprFlags <alias_ExprFlags>`                    +
+----------+-------------------------------------------------------+


to be documented


.. _handle_ast_FieldDeclaration:

.. das:attribute:: FieldDeclaration

FieldDeclaration fields are

+----------+--------------------------------------------------------------------------+
+annotation+ :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>` +
+----------+--------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                             +
+----------+--------------------------------------------------------------------------+
+name      + :ref:`builtin::das_string <handle_builtin_das_string>`                   +
+----------+--------------------------------------------------------------------------+
+init      +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >               +
+----------+--------------------------------------------------------------------------+
+offset    +int                                                                       +
+----------+--------------------------------------------------------------------------+
+_type     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                   +
+----------+--------------------------------------------------------------------------+
+flags     + :ref:`FieldDeclarationFlags <alias_FieldDeclarationFlags>`               +
+----------+--------------------------------------------------------------------------+


to be documented


.. _handle_ast_Function:

.. das:attribute:: Function

Function fields are

+---------------+------------------------------------------------------------------------------------+
+arguments      + :ref:`ast::dasvector`smart_ptr`Variable <handle_ast_dasvector`smart_ptr`Variable>` +
+---------------+------------------------------------------------------------------------------------+
+fromGeneric    + :ref:`ast::Function <handle_ast_Function>` ?                                       +
+---------------+------------------------------------------------------------------------------------+
+result         +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                             +
+---------------+------------------------------------------------------------------------------------+
+aotHash        +uint64                                                                              +
+---------------+------------------------------------------------------------------------------------+
+totalGenLabel  +int                                                                                 +
+---------------+------------------------------------------------------------------------------------+
+_module        + :ref:`rtti::Module <handle_rtti_Module>` ?                                         +
+---------------+------------------------------------------------------------------------------------+
+index          +int                                                                                 +
+---------------+------------------------------------------------------------------------------------+
+at             + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                       +
+---------------+------------------------------------------------------------------------------------+
+inferStack     + :ref:`ast::dasvector`InferHistory <handle_ast_dasvector`InferHistory>`             +
+---------------+------------------------------------------------------------------------------------+
+body           +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >                         +
+---------------+------------------------------------------------------------------------------------+
+atDecl         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                       +
+---------------+------------------------------------------------------------------------------------+
+sideEffectFlags+ :ref:`FunctionSideEffectFlags <alias_FunctionSideEffectFlags>`                     +
+---------------+------------------------------------------------------------------------------------+
+annotations    + :ref:`rtti::AnnotationList <handle_rtti_AnnotationList>`                           +
+---------------+------------------------------------------------------------------------------------+
+totalStackSize +uint                                                                                +
+---------------+------------------------------------------------------------------------------------+
+name           + :ref:`builtin::das_string <handle_builtin_das_string>`                             +
+---------------+------------------------------------------------------------------------------------+
+hash           +uint64                                                                              +
+---------------+------------------------------------------------------------------------------------+
+flags          + :ref:`FunctionFlags <alias_FunctionFlags>`                                         +
+---------------+------------------------------------------------------------------------------------+


to be documented


.. _handle_ast_FunctionAnnotation:

.. das:attribute:: FunctionAnnotation

to be documented


.. _handle_ast_InferHistory:

.. das:attribute:: InferHistory

InferHistory fields are

+----+----------------------------------------------+
+func+ :ref:`ast::Function <handle_ast_Function>` ? +
+----+----------------------------------------------+
+at  + :ref:`rtti::LineInfo <handle_rtti_LineInfo>` +
+----+----------------------------------------------+


to be documented


.. _handle_ast_MakeFieldDecl:

.. das:attribute:: MakeFieldDecl

MakeFieldDecl fields are

+-----+-----------------------------------------------------------+
+value+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >+
+-----+-----------------------------------------------------------+
+at   + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`              +
+-----+-----------------------------------------------------------+
+name + :ref:`builtin::das_string <handle_builtin_das_string>`    +
+-----+-----------------------------------------------------------+
+flags+ :ref:`MakeFieldDeclFlags <alias_MakeFieldDeclFlags>`      +
+-----+-----------------------------------------------------------+


to be documented


.. _handle_ast_ModuleGroup:

.. das:attribute:: ModuleGroup

to be documented


.. _handle_ast_ModuleLibrary:

.. das:attribute:: ModuleLibrary

to be documented


.. _handle_ast_PassMacro:

.. das:attribute:: PassMacro

PassMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle_builtin_das_string>` +
+----+--------------------------------------------------------+


to be documented


.. _handle_ast_ReaderMacro:

.. das:attribute:: ReaderMacro

ReaderMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle_rtti_Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle_builtin_das_string>` +
+-------+--------------------------------------------------------+


to be documented


.. _handle_ast_Structure:

.. das:attribute:: Structure

Structure fields are

+-----------+--------------------------------------------------------------------------------+
+_module    + :ref:`rtti::Module <handle_rtti_Module>` ?                                     +
+-----------+--------------------------------------------------------------------------------+
+at         + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                   +
+-----------+--------------------------------------------------------------------------------+
+parent     + :ref:`ast::Structure <handle_ast_Structure>` ?                                 +
+-----------+--------------------------------------------------------------------------------+
+annotations+ :ref:`rtti::AnnotationList <handle_rtti_AnnotationList>`                       +
+-----------+--------------------------------------------------------------------------------+
+name       + :ref:`builtin::das_string <handle_builtin_das_string>`                         +
+-----------+--------------------------------------------------------------------------------+
+fields     + :ref:`ast::dasvector`FieldDeclaration <handle_ast_dasvector`FieldDeclaration>` +
+-----------+--------------------------------------------------------------------------------+
+flags      + :ref:`StructureFlags <alias_StructureFlags>`                                   +
+-----------+--------------------------------------------------------------------------------+


to be documented


.. _handle_ast_StructureAnnotation:

.. das:attribute:: StructureAnnotation

to be documented


.. _handle_ast_TypeDecl:

.. das:attribute:: TypeDecl

TypeDecl fields are

+----------+----------------------------------------------------------------------------------------+
+alias     + :ref:`builtin::das_string <handle_builtin_das_string>`                                 +
+----------+----------------------------------------------------------------------------------------+
+annotation+ :ref:`rtti::TypeAnnotation <handle_rtti_TypeAnnotation>` ?                             +
+----------+----------------------------------------------------------------------------------------+
+dimExpr   + :ref:`ast::dasvector`smart_ptr`Expression <handle_ast_dasvector`smart_ptr`Expression>` +
+----------+----------------------------------------------------------------------------------------+
+argTypes  + :ref:`ast::dasvector`smart_ptr`TypeDecl <handle_ast_dasvector`smart_ptr`TypeDecl>`     +
+----------+----------------------------------------------------------------------------------------+
+dim       + :ref:`ast::dasvector`int <handle_ast_dasvector`int>`                                   +
+----------+----------------------------------------------------------------------------------------+
+_module   + :ref:`rtti::Module <handle_rtti_Module>` ?                                             +
+----------+----------------------------------------------------------------------------------------+
+secondType+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+at        + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                                           +
+----------+----------------------------------------------------------------------------------------+
+enumType  + :ref:`ast::Enumeration <handle_ast_Enumeration>` ?                                     +
+----------+----------------------------------------------------------------------------------------+
+argNames  + :ref:`ast::dasvector`das_string <handle_ast_dasvector`das_string>`                     +
+----------+----------------------------------------------------------------------------------------+
+baseType  + :ref:`rtti::Type <enum_rtti_Type>`                                                     +
+----------+----------------------------------------------------------------------------------------+
+firstType +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                                 +
+----------+----------------------------------------------------------------------------------------+
+structType+ :ref:`ast::Structure <handle_ast_Structure>` ?                                         +
+----------+----------------------------------------------------------------------------------------+
+flags     + :ref:`TypeDeclFlags <alias_TypeDeclFlags>`                                             +
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


to be documented


.. _handle_ast_TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

TypeInfoMacro fields are

+-------+--------------------------------------------------------+
+_module+ :ref:`rtti::Module <handle_rtti_Module>` ?             +
+-------+--------------------------------------------------------+
+name   + :ref:`builtin::das_string <handle_builtin_das_string>` +
+-------+--------------------------------------------------------+


to be documented


.. _handle_ast_Variable:

.. das:attribute:: Variable

Variable fields are

+-------------+--------------------------------------------------------------------------+
+annotation   + :ref:`rtti::AnnotationArgumentList <handle_rtti_AnnotationArgumentList>` +
+-------------+--------------------------------------------------------------------------+
+initStackSize+uint                                                                      +
+-------------+--------------------------------------------------------------------------+
+_module      + :ref:`rtti::Module <handle_rtti_Module>` ?                               +
+-------------+--------------------------------------------------------------------------+
+index        +int                                                                       +
+-------------+--------------------------------------------------------------------------+
+at           + :ref:`rtti::LineInfo <handle_rtti_LineInfo>`                             +
+-------------+--------------------------------------------------------------------------+
+stackTop     +uint                                                                      +
+-------------+--------------------------------------------------------------------------+
+name         + :ref:`builtin::das_string <handle_builtin_das_string>`                   +
+-------------+--------------------------------------------------------------------------+
+init         +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >               +
+-------------+--------------------------------------------------------------------------+
+access_flags + :ref:`VariableAccessFlags <alias_VariableAccessFlags>`                   +
+-------------+--------------------------------------------------------------------------+
+source       +smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >               +
+-------------+--------------------------------------------------------------------------+
+_type        +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >                   +
+-------------+--------------------------------------------------------------------------+
+flags        + :ref:`VariableFlags <alias_VariableFlags>`                               +
+-------------+--------------------------------------------------------------------------+


Variable properties are

+--------------+----+
+isAccessUnused+bool+
+--------------+----+


to be documented


.. _handle_ast_VariantMacro:

.. das:attribute:: VariantMacro

VariantMacro fields are

+----+--------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle_builtin_das_string>` +
+----+--------------------------------------------------------+


to be documented


.. _handle_ast_VisitorAdapter:

.. das:attribute:: VisitorAdapter

to be documented


+++++++++++++
Handled types
+++++++++++++

.. _handle_ast_MakeStruct:

.. das:attribute:: MakeStruct

to be documented


.. _handle_ast_dasvector`FieldDeclaration:

.. das:attribute:: dasvector`FieldDeclaration

to be documented


.. _handle_ast_dasvector`InferHistory:

.. das:attribute:: dasvector`InferHistory

to be documented


.. _handle_ast_dasvector`LineInfo:

.. das:attribute:: dasvector`LineInfo

to be documented


.. _handle_ast_dasvector`das_string:

.. das:attribute:: dasvector`das_string

to be documented


.. _handle_ast_dasvector`int:

.. das:attribute:: dasvector`int

to be documented


.. _handle_ast_dasvector`pair`das_string`smart_ptr`Expression:

.. das:attribute:: dasvector`pair`das_string`smart_ptr`Expression

to be documented


.. _handle_ast_dasvector`pair`uint`uint:

.. das:attribute:: dasvector`pair`uint`uint

to be documented


.. _handle_ast_dasvector`smart_ptr`Expression:

.. das:attribute:: dasvector`smart_ptr`Expression

to be documented


.. _handle_ast_dasvector`smart_ptr`MakeFieldDecl:

.. das:attribute:: dasvector`smart_ptr`MakeFieldDecl

to be documented


.. _handle_ast_dasvector`smart_ptr`MakeStruct:

.. das:attribute:: dasvector`smart_ptr`MakeStruct

to be documented


.. _handle_ast_dasvector`smart_ptr`TypeDecl:

.. das:attribute:: dasvector`smart_ptr`TypeDecl

to be documented


.. _handle_ast_dasvector`smart_ptr`Variable:

.. das:attribute:: dasvector`smart_ptr`Variable

to be documented


.. _handle_ast_dasvector`uint8:

.. das:attribute:: dasvector`uint8

to be documented


+++++++
Classes
+++++++

.. _struct-ast-AstFunctionAnnotation:

.. das:class:: AstFunctionAnnotation

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstFunctionAnnotation.transform ( self:AstFunctionAnnotation; call:smart_ptr<ast::ExprCall>; errors:das_string )  : ExpressionPtr 

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


.. das:method:: AstFunctionAnnotation.apply ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

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


.. das:method:: AstFunctionAnnotation.finish ( self:AstFunctionAnnotation; func:FunctionPtr; group:ModuleGroup; args:AnnotationArgumentList const; progArgs:AnnotationArgumentList const; errors:das_string )  : bool 

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



.. _struct-ast-AstPassMacro:

.. das:class:: AstPassMacro

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstPassMacro.apply ( self:AstPassMacro; prog:ProgramPtr; mod:rtti::Module? const )  : bool 

apply returns bool

method arguments are

+----+----------------------------------------------------+
+self+ :ref:`ast::AstPassMacro <struct-ast-AstPassMacro>` +
+----+----------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`               +
+----+----------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const   +
+----+----------------------------------------------------+


to be documented



.. _struct-ast-AstReaderMacro:

.. das:class:: AstReaderMacro

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstReaderMacro.accept ( self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:ast::ExprReader? const; ch:int const; info:LineInfo const )  : bool 

accept returns bool

method arguments are

+----+--------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>` +
+----+--------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                   +
+----+--------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const       +
+----+--------------------------------------------------------+
+expr+ :ref:`ast::ExprReader <handle_ast_ExprReader>` ? const +
+----+--------------------------------------------------------+
+ch  +int const                                               +
+----+--------------------------------------------------------+
+info+ :ref:`rtti::LineInfo <handle_rtti_LineInfo>`  const    +
+----+--------------------------------------------------------+


to be documented


.. das:method:: AstReaderMacro.visit ( self:AstReaderMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr 

visit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstReaderMacro <struct-ast-AstReaderMacro>`          +
+----+-----------------------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`                            +
+----+-----------------------------------------------------------------+
+mod + :ref:`rtti::Module <handle_rtti_Module>` ? const                +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle_ast_ExprReader>` > const+
+----+-----------------------------------------------------------------+


to be documented



.. _struct-ast-AstStructureAnnotation:

.. das:class:: AstStructureAnnotation

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstStructureAnnotation.apply ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

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


.. das:method:: AstStructureAnnotation.finish ( self:AstStructureAnnotation; st:StructurePtr; group:ModuleGroup; args:AnnotationArgumentList const; errors:das_string )  : bool 

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



.. _struct-ast-AstVariantMacro:

.. das:class:: AstVariantMacro

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstVariantMacro.visitExprIsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr 

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


.. das:method:: AstVariantMacro.visitExprAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr 

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


.. das:method:: AstVariantMacro.visitExprSafeAsVariant ( self:AstVariantMacro; prog:ProgramPtr; mod:rtti::Module? const; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr 

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



.. _struct-ast-AstVisitor:

.. das:class:: AstVisitor

to be documented

it defines as follows

  __rtti : void?

.. das:method:: AstVisitor.preVisitProgram ( self:AstVisitor; prog:ProgramPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`           +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitProgram ( self:AstVisitor; porg:ProgramPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+porg+ :ref:`ProgramPtr <alias_ProgramPtr>`           +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitProgramBody ( self:AstVisitor; prog:ProgramPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`           +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitTypeDecl ( self:AstVisitor; typ:TypeDeclPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias_TypeDeclPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitTypeDecl ( self:AstVisitor; typ:TypeDeclPtr )  : TypeDeclPtr 

visitTypeDecl returns  :ref:`TypeDeclPtr <alias_TypeDeclPtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias_TypeDeclPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitAlias ( self:AstVisitor; typ:TypeDeclPtr; name:das_string const )  

method arguments are

+----+--------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+----+--------------------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias_TypeDeclPtr>`                       +
+----+--------------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle_builtin_das_string>`  const+
+----+--------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitAlias ( self:AstVisitor; typ:TypeDeclPtr; name:das_string const )  : TypeDeclPtr 

visitAlias returns  :ref:`TypeDeclPtr <alias_TypeDeclPtr>` 

method arguments are

+----+--------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+----+--------------------------------------------------------------+
+typ + :ref:`TypeDeclPtr <alias_TypeDeclPtr>`                       +
+----+--------------------------------------------------------------+
+name+ :ref:`builtin::das_string <handle_builtin_das_string>`  const+
+----+--------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitEnumeration ( self:AstVisitor; enu:EnumerationPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+enu + :ref:`EnumerationPtr <alias_EnumerationPtr>`   +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitEnumerationValue ( self:AstVisitor; enu:EnumerationPtr; name:das_string const; value:ExpressionPtr; last:bool const )  

method arguments are

+-----+--------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+-----+--------------------------------------------------------------+
+enu  + :ref:`EnumerationPtr <alias_EnumerationPtr>`                 +
+-----+--------------------------------------------------------------+
+name + :ref:`builtin::das_string <handle_builtin_das_string>`  const+
+-----+--------------------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                   +
+-----+--------------------------------------------------------------+
+last +bool const                                                    +
+-----+--------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitEnumerationValue ( self:AstVisitor; enu:EnumerationPtr; name:das_string const; value:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitEnumerationValue returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+-----+--------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`               +
+-----+--------------------------------------------------------------+
+enu  + :ref:`EnumerationPtr <alias_EnumerationPtr>`                 +
+-----+--------------------------------------------------------------+
+name + :ref:`builtin::das_string <handle_builtin_das_string>`  const+
+-----+--------------------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                   +
+-----+--------------------------------------------------------------+
+last +bool const                                                    +
+-----+--------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitEnumeration ( self:AstVisitor; enu:EnumerationPtr )  : EnumerationPtr 

visitEnumeration returns  :ref:`EnumerationPtr <alias_EnumerationPtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+enu + :ref:`EnumerationPtr <alias_EnumerationPtr>`   +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitStructure ( self:AstVisitor; str:StructurePtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+str + :ref:`StructurePtr <alias_StructurePtr>`       +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitStructureField ( self:AstVisitor; str:StructurePtr; decl:FieldDeclaration const; last:bool const )  

method arguments are

+----+------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+----+------------------------------------------------------------------+
+str + :ref:`StructurePtr <alias_StructurePtr>`                         +
+----+------------------------------------------------------------------+
+decl+ :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const+
+----+------------------------------------------------------------------+
+last+bool const                                                        +
+----+------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitStructureField ( self:AstVisitor; str:StructurePtr; decl:FieldDeclaration const; last:bool const )  

method arguments are

+----+------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                   +
+----+------------------------------------------------------------------+
+str + :ref:`StructurePtr <alias_StructurePtr>`                         +
+----+------------------------------------------------------------------+
+decl+ :ref:`ast::FieldDeclaration <handle_ast_FieldDeclaration>`  const+
+----+------------------------------------------------------------------+
+last+bool const                                                        +
+----+------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitStructure ( self:AstVisitor; str:StructurePtr )  : StructurePtr 

visitStructure returns  :ref:`StructurePtr <alias_StructurePtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+str + :ref:`StructurePtr <alias_StructurePtr>`       +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunction ( self:AstVisitor; fun:FunctionPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunction ( self:AstVisitor; fun:FunctionPtr )  : FunctionPtr 

visitFunction returns  :ref:`FunctionPtr <alias_FunctionPtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionArgument ( self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; lastArg:bool const )  

method arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+fun    + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionArgument ( self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; lastArg:bool const )  : VariablePtr 

visitFunctionArgument returns  :ref:`VariablePtr <alias_VariablePtr>` 

method arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+fun    + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionArgumentInit ( self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; value:ExpressionPtr )  

method arguments are

+-----+------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-----+------------------------------------------------+
+fun  + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+-----+------------------------------------------------+
+arg  + :ref:`VariablePtr <alias_VariablePtr>`         +
+-----+------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+-----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionArgumentInit ( self:AstVisitor; fun:FunctionPtr; arg:VariablePtr; value:ExpressionPtr )  : ExpressionPtr 

visitFunctionArgumentInit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+-----+------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-----+------------------------------------------------+
+fun  + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+-----+------------------------------------------------+
+arg  + :ref:`VariablePtr <alias_VariablePtr>`         +
+-----+------------------------------------------------+
+value+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+-----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitFunctionBody ( self:AstVisitor; fun:FunctionPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitFunctionBody ( self:AstVisitor; fun:FunctionPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+fun + :ref:`FunctionPtr <alias_FunctionPtr>`         +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExpression ( self:AstVisitor; expr:ExpressionPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExpression ( self:AstVisitor; expr:ExpressionPtr )  : ExpressionPtr 

visitExpression returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlock ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlock ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const )  : ExpressionPtr 

visitExprBlock returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockArgument ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; lastArg:bool const )  

method arguments are

+-------+---------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-------+---------------------------------------------------------------+
+blk    +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+-------+---------------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`                        +
+-------+---------------------------------------------------------------+
+lastArg+bool const                                                     +
+-------+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockArgument ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; lastArg:bool const )  : VariablePtr 

visitExprBlockArgument returns  :ref:`VariablePtr <alias_VariablePtr>` 

method arguments are

+-------+---------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-------+---------------------------------------------------------------+
+blk    +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+-------+---------------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`                        +
+-------+---------------------------------------------------------------+
+lastArg+bool const                                                     +
+-------+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockArgumentInit ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; expr:ExpressionPtr )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`                        +
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockArgumentInit ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr 

visitExprBlockArgumentInit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`                        +
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockExpression ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockExpression ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  : ExpressionPtr 

visitExprBlockExpression returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockFinal ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockFinal ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBlockFinalExpression ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBlockFinalExpression ( self:AstVisitor; blk:smart_ptr<ast::ExprBlock> const; expr:ExpressionPtr )  : ExpressionPtr 

visitExprBlockFinalExpression returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprBlock <handle_ast_ExprBlock>` > const+
+----+---------------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLet ( self:AstVisitor; expr:smart_ptr<ast::ExprLet> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLet ( self:AstVisitor; expr:smart_ptr<ast::ExprLet> const )  : ExpressionPtr 

visitExprLet returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLetVariable ( self:AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:VariablePtr; lastArg:bool const )  

method arguments are

+-------+-----------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-------+-----------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+-------+-----------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`                    +
+-------+-----------------------------------------------------------+
+lastArg+bool const                                                 +
+-------+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLetVariable ( self:AstVisitor; expr:smart_ptr<ast::ExprLet> const; arg:VariablePtr; lastArg:bool const )  : VariablePtr 

visitExprLetVariable returns  :ref:`VariablePtr <alias_VariablePtr>` 

method arguments are

+-------+-----------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-------+-----------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+-------+-----------------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`                    +
+-------+-----------------------------------------------------------+
+lastArg+bool const                                                 +
+-------+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLetVariableInit ( self:AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:VariablePtr; expr:ExpressionPtr )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`                    +
+----+-----------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLetVariableInit ( self:AstVisitor; blk:smart_ptr<ast::ExprLet> const; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr 

visitExprLetVariableInit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+blk +smart_ptr< :ref:`ast::ExprLet <handle_ast_ExprLet>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`                    +
+----+-----------------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLet ( self:AstVisitor; prog:ProgramPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`           +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLet ( self:AstVisitor; prog:ProgramPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+prog+ :ref:`ProgramPtr <alias_ProgramPtr>`           +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLetVariable ( self:AstVisitor; arg:VariablePtr; lastArg:bool const )  

method arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLetVariable ( self:AstVisitor; arg:VariablePtr; lastArg:bool const )  : VariablePtr 

visitGlobalLetVariable returns  :ref:`VariablePtr <alias_VariablePtr>` 

method arguments are

+-------+------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+-------+------------------------------------------------+
+arg    + :ref:`VariablePtr <alias_VariablePtr>`         +
+-------+------------------------------------------------+
+lastArg+bool const                                      +
+-------+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitGlobalLetVariableInit ( self:AstVisitor; arg:VariablePtr; expr:ExpressionPtr )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`         +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitGlobalLetVariableInit ( self:AstVisitor; arg:VariablePtr; expr:ExpressionPtr )  : ExpressionPtr 

visitGlobalLetVariableInit returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>` +
+----+------------------------------------------------+
+arg + :ref:`VariablePtr <alias_VariablePtr>`         +
+----+------------------------------------------------+
+expr+ :ref:`ExpressionPtr <alias_ExpressionPtr>`     +
+----+------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStringBuilder ( self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle_ast_ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStringBuilder ( self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const )  : ExpressionPtr 

visitExprStringBuilder returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle_ast_ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStringBuilderElement ( self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:ExpressionPtr; last:bool const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle_ast_ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+
+elem+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStringBuilderElement ( self:AstVisitor; expr:smart_ptr<ast::ExprStringBuilder> const; elem:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprStringBuilderElement returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStringBuilder <handle_ast_ExprStringBuilder>` > const+
+----+-------------------------------------------------------------------------------+
+elem+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNew ( self:AstVisitor; expr:smart_ptr<ast::ExprNew> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle_ast_ExprNew>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNew ( self:AstVisitor; expr:smart_ptr<ast::ExprNew> const )  : ExpressionPtr 

visitExprNew returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle_ast_ExprNew>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNewArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:ExpressionPtr; last:bool const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle_ast_ExprNew>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNewArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprNew> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprNewArgument returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNew <handle_ast_ExprNew>` > const+
+----+-----------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNamedCall ( self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle_ast_ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNamedCall ( self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const )  : ExpressionPtr 

visitExprNamedCall returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle_ast_ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNamedCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:MakeFieldDeclPtr; last:bool const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle_ast_ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+
+arg + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                      +
+----+-----------------------------------------------------------------------+
+last+bool const                                                             +
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNamedCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprNamedCall> const; arg:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr 

visitExprNamedCallArgument returns  :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNamedCall <handle_ast_ExprNamedCall>` > const+
+----+-----------------------------------------------------------------------+
+arg + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                      +
+----+-----------------------------------------------------------------------+
+last+bool const                                                             +
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLooksLikeCall ( self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle_ast_ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLooksLikeCall ( self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const )  : ExpressionPtr 

visitExprLooksLikeCall returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle_ast_ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLooksLikeCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:ExpressionPtr; last:bool const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle_ast_ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLooksLikeCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprLooksLikeCall> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprLooksLikeCallArgument returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLooksLikeCall <handle_ast_ExprLooksLikeCall>` > const+
+----+-------------------------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                                    +
+----+-------------------------------------------------------------------------------+
+last+bool const                                                                     +
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCall ( self:AstVisitor; expr:smart_ptr<ast::ExprCall> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCall ( self:AstVisitor; expr:smart_ptr<ast::ExprCall> const )  : ExpressionPtr 

visitExprCall returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:ExpressionPtr; last:bool const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` > const+
+----+-------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                  +
+----+-------------------------------------------------------------+
+last+bool const                                                   +
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCallArgument ( self:AstVisitor; expr:smart_ptr<ast::ExprCall> const; arg:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprCallArgument returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCall <handle_ast_ExprCall>` > const+
+----+-------------------------------------------------------------+
+arg + :ref:`ExpressionPtr <alias_ExpressionPtr>`                  +
+----+-------------------------------------------------------------+
+last+bool const                                                   +
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNullCoalescing ( self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const )  

method arguments are

+----+---------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+----+---------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNullCoalescing <handle_ast_ExprNullCoalescing>` > const+
+----+---------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprNullCoalescing ( self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const )  : ExpressionPtr 

visitExprNullCoalescing returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+----+---------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprNullCoalescing <handle_ast_ExprNullCoalescing>` > const+
+----+---------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprNullCoalescingDefault ( self:AstVisitor; expr:smart_ptr<ast::ExprNullCoalescing> const; defval:ExpressionPtr )  

method arguments are

+------+---------------------------------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                  +
+------+---------------------------------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprNullCoalescing <handle_ast_ExprNullCoalescing>` > const+
+------+---------------------------------------------------------------------------------+
+defval+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                                      +
+------+---------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAt ( self:AstVisitor; expr:smart_ptr<ast::ExprAt> const )  

method arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAt <handle_ast_ExprAt>` > const+
+----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAt ( self:AstVisitor; expr:smart_ptr<ast::ExprAt> const )  : ExpressionPtr 

visitExprAt returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAt <handle_ast_ExprAt>` > const+
+----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAtIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:ExpressionPtr )  

method arguments are

+-----+---------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+-----+---------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprAt <handle_ast_ExprAt>` > const+
+-----+---------------------------------------------------------+
+index+ :ref:`ExpressionPtr <alias_ExpressionPtr>`              +
+-----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAt ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAt <handle_ast_ExprSafeAt>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeAt ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeAt> const )  : ExpressionPtr 

visitExprSafeAt returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAt <handle_ast_ExprSafeAt>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAtIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; index:ExpressionPtr )  

method arguments are

+-----+---------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+-----+---------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprAt <handle_ast_ExprAt>` > const+
+-----+---------------------------------------------------------+
+index+ :ref:`ExpressionPtr <alias_ExpressionPtr>`              +
+-----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIs ( self:AstVisitor; expr:smart_ptr<ast::ExprIs> const )  

method arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIs <handle_ast_ExprIs>` > const+
+----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIs ( self:AstVisitor; expr:smart_ptr<ast::ExprIs> const )  : ExpressionPtr 

visitExprIs returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+----+---------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIs <handle_ast_ExprIs>` > const+
+----+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIsType ( self:AstVisitor; expr:smart_ptr<ast::ExprAt> const; typeDecl:TypeDeclPtr )  

method arguments are

+--------+---------------------------------------------------------+
+self    + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`          +
+--------+---------------------------------------------------------+
+expr    +smart_ptr< :ref:`ast::ExprAt <handle_ast_ExprAt>` > const+
+--------+---------------------------------------------------------+
+typeDecl+ :ref:`TypeDeclPtr <alias_TypeDeclPtr>`                  +
+--------+---------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp2 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp2 <handle_ast_ExprOp2>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp2 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const )  : ExpressionPtr 

visitExprOp2 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp2 <handle_ast_ExprOp2>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp2Right ( self:AstVisitor; expr:smart_ptr<ast::ExprOp2> const; right:ExpressionPtr )  

method arguments are

+-----+-----------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-----+-----------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprOp2 <handle_ast_ExprOp2>` > const+
+-----+-----------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+-----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle_ast_ExprOp3>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp3 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const )  : ExpressionPtr 

visitExprOp3 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle_ast_ExprOp3>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3Left ( self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const; left:ExpressionPtr )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp3 <handle_ast_ExprOp3>` > const+
+----+-----------------------------------------------------------+
+left+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp3Right ( self:AstVisitor; expr:smart_ptr<ast::ExprOp3> const; right:ExpressionPtr )  

method arguments are

+-----+-----------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+-----+-----------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprOp3 <handle_ast_ExprOp3>` > const+
+-----+-----------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+-----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCopy ( self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCopy <handle_ast_ExprCopy>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCopy ( self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const )  : ExpressionPtr 

visitExprCopy returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCopy <handle_ast_ExprCopy>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCopyRight ( self:AstVisitor; expr:smart_ptr<ast::ExprCopy> const; right:ExpressionPtr )  

method arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprCopy <handle_ast_ExprCopy>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMove ( self:AstVisitor; expr:smart_ptr<ast::ExprMove> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMove <handle_ast_ExprMove>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMove ( self:AstVisitor; expr:smart_ptr<ast::ExprMove> const )  : ExpressionPtr 

visitExprMove returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMove <handle_ast_ExprMove>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMoveRight ( self:AstVisitor; expr:smart_ptr<ast::ExprMove> const; right:ExpressionPtr )  

method arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMove <handle_ast_ExprMove>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprClone ( self:AstVisitor; expr:smart_ptr<ast::ExprClone> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprClone <handle_ast_ExprClone>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprClone ( self:AstVisitor; expr:smart_ptr<ast::ExprClone> const )  : ExpressionPtr 

visitExprClone returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprClone <handle_ast_ExprClone>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCloneRight ( self:AstVisitor; expr:smart_ptr<ast::ExprClone> const; right:ExpressionPtr )  

method arguments are

+-----+---------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-----+---------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprClone <handle_ast_ExprClone>` > const+
+-----+---------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+-----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWith ( self:AstVisitor; expr:smart_ptr<ast::ExprWith> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWith <handle_ast_ExprWith>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprWith ( self:AstVisitor; expr:smart_ptr<ast::ExprWith> const )  : ExpressionPtr 

visitExprWith returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWith <handle_ast_ExprWith>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWithBody ( self:AstVisitor; expr:smart_ptr<ast::ExprWith> const; right:ExpressionPtr )  

method arguments are

+-----+-------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+-----+-------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprWith <handle_ast_ExprWith>` > const+
+-----+-------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                  +
+-----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWhile ( self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWhile <handle_ast_ExprWhile>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprWhile ( self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const )  : ExpressionPtr 

visitExprWhile returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprWhile <handle_ast_ExprWhile>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprWhileBody ( self:AstVisitor; expr:smart_ptr<ast::ExprWhile> const; right:ExpressionPtr )  

method arguments are

+-----+---------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+-----+---------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprWhile <handle_ast_ExprWhile>` > const+
+-----+---------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                    +
+-----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTryCatch ( self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const )  

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTryCatch <handle_ast_ExprTryCatch>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprTryCatch ( self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const )  : ExpressionPtr 

visitExprTryCatch returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTryCatch <handle_ast_ExprTryCatch>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTryCatchCatch ( self:AstVisitor; expr:smart_ptr<ast::ExprTryCatch> const; right:ExpressionPtr )  

method arguments are

+-----+---------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+-----+---------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprTryCatch <handle_ast_ExprTryCatch>` > const+
+-----+---------------------------------------------------------------------+
+right+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                          +
+-----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElse ( self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIfThenElse <handle_ast_ExprIfThenElse>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIfThenElse ( self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const )  : ExpressionPtr 

visitExprIfThenElse returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIfThenElse <handle_ast_ExprIfThenElse>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElseIfBlock ( self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; ifBlock:ExpressionPtr )  

method arguments are

+-------+-------------------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-------+-------------------------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprIfThenElse <handle_ast_ExprIfThenElse>` > const+
+-------+-------------------------------------------------------------------------+
+ifBlock+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                              +
+-------+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIfThenElseElseBlock ( self:AstVisitor; expr:smart_ptr<ast::ExprIfThenElse> const; elseBlock:ExpressionPtr )  

method arguments are

+---------+-------------------------------------------------------------------------+
+self     + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+---------+-------------------------------------------------------------------------+
+expr     +smart_ptr< :ref:`ast::ExprIfThenElse <handle_ast_ExprIfThenElse>` > const+
+---------+-------------------------------------------------------------------------+
+elseBlock+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                              +
+---------+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFor ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFor ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const )  : ExpressionPtr 

visitExprFor returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForVariable ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:VariablePtr; last:bool const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+
+svar+ :ref:`VariablePtr <alias_VariablePtr>`                    +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprForVariable ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; svar:VariablePtr; last:bool const )  : VariablePtr 

visitExprForVariable returns  :ref:`VariablePtr <alias_VariablePtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+
+svar+ :ref:`VariablePtr <alias_VariablePtr>`                    +
+----+-----------------------------------------------------------+
+last+bool const                                                 +
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForSource ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:ExpressionPtr; last:bool const )  

method arguments are

+------+-----------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+------+-----------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+------+-----------------------------------------------------------+
+source+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+------+-----------------------------------------------------------+
+last  +bool const                                                 +
+------+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprForSource ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const; source:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprForSource returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+------+-----------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+------+-----------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+------+-----------------------------------------------------------+
+source+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                +
+------+-----------------------------------------------------------+
+last  +bool const                                                 +
+------+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForStack ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprForBody ( self:AstVisitor; expr:smart_ptr<ast::ExprFor> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFor <handle_ast_ExprFor>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeVariant <handle_ast_ExprMakeVariant>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const )  : ExpressionPtr 

visitExprMakeVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeVariant <handle_ast_ExprMakeVariant>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeVariantField ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  

method arguments are

+-----+---------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+-----+---------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeVariant <handle_ast_ExprMakeVariant>` > const+
+-----+---------------------------------------------------------------------------+
+index+int const                                                                  +
+-----+---------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                          +
+-----+---------------------------------------------------------------------------+
+last +bool const                                                                 +
+-----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeVariantField ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeVariant> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr 

visitExprMakeVariantField returns  :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>` 

method arguments are

+-----+---------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+-----+---------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeVariant <handle_ast_ExprMakeVariant>` > const+
+-----+---------------------------------------------------------------------------+
+index+int const                                                                  +
+-----+---------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                          +
+-----+---------------------------------------------------------------------------+
+last +bool const                                                                 +
+-----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeStruct ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeStruct ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const )  : ExpressionPtr 

visitExprMakeStruct returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeStructIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const )  

method arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeStructIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; last:bool const )  

method arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeStructField ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  

method arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                        +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeStructField ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeStruct> const; index:int const; decl:MakeFieldDeclPtr; last:bool const )  : MakeFieldDeclPtr 

visitExprMakeStructField returns  :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>` 

method arguments are

+-----+-------------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+-----+-------------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeStruct <handle_ast_ExprMakeStruct>` > const+
+-----+-------------------------------------------------------------------------+
+index+int const                                                                +
+-----+-------------------------------------------------------------------------+
+decl + :ref:`MakeFieldDeclPtr <alias_MakeFieldDeclPtr>`                        +
+-----+-------------------------------------------------------------------------+
+last +bool const                                                               +
+-----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeArray ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeArray <handle_ast_ExprMakeArray>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeArray ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const )  : ExpressionPtr 

visitExprMakeArray returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeArray <handle_ast_ExprMakeArray>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeArrayIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:ExpressionPtr; last:bool const )  

method arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeArray <handle_ast_ExprMakeArray>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias_ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeArrayIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeArray> const; index:int const; init:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprMakeArrayIndex returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeArray <handle_ast_ExprMakeArray>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias_ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeTuple ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeTuple <handle_ast_ExprMakeTuple>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeTuple ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const )  : ExpressionPtr 

visitExprMakeTuple returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeTuple <handle_ast_ExprMakeTuple>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeTupleIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:ExpressionPtr; last:bool const )  

method arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeTuple <handle_ast_ExprMakeTuple>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias_ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeTupleIndex ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeTuple> const; index:int const; init:ExpressionPtr; last:bool const )  : ExpressionPtr 

visitExprMakeTupleIndex returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+-----+-----------------------------------------------------------------------+
+self + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+-----+-----------------------------------------------------------------------+
+expr +smart_ptr< :ref:`ast::ExprMakeTuple <handle_ast_ExprMakeTuple>` > const+
+-----+-----------------------------------------------------------------------+
+index+int const                                                              +
+-----+-----------------------------------------------------------------------+
+init + :ref:`ExpressionPtr <alias_ExpressionPtr>`                            +
+-----+-----------------------------------------------------------------------+
+last +bool const                                                             +
+-----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprArrayComprehension ( self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const )  

method arguments are

+----+-----------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+----+-----------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprArrayComprehension <handle_ast_ExprArrayComprehension>` > const+
+----+-----------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprArrayComprehension ( self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const )  : ExpressionPtr 

visitExprArrayComprehension returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+----+-----------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprArrayComprehension <handle_ast_ExprArrayComprehension>` > const+
+----+-----------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprArrayComprehensionSubexpr ( self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; subexrp:ExpressionPtr )  

method arguments are

+-------+-----------------------------------------------------------------------------------------+
+self   + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+-------+-----------------------------------------------------------------------------------------+
+expr   +smart_ptr< :ref:`ast::ExprArrayComprehension <handle_ast_ExprArrayComprehension>` > const+
+-------+-----------------------------------------------------------------------------------------+
+subexrp+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                                              +
+-------+-----------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprArrayComprehensionWhere ( self:AstVisitor; expr:smart_ptr<ast::ExprArrayComprehension> const; filter:ExpressionPtr )  

method arguments are

+------+-----------------------------------------------------------------------------------------+
+self  + :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                          +
+------+-----------------------------------------------------------------------------------------+
+expr  +smart_ptr< :ref:`ast::ExprArrayComprehension <handle_ast_ExprArrayComprehension>` > const+
+------+-----------------------------------------------------------------------------------------+
+filter+ :ref:`ExpressionPtr <alias_ExpressionPtr>`                                              +
+------+-----------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprTypeInfo ( self:AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const )  

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTypeInfo <handle_ast_ExprTypeInfo>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprTypeInfo ( self:AstVisitor; expr:smart_ptr<ast::ExprTypeInfo> const )  : ExpressionPtr 

visitExprTypeInfo returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprTypeInfo <handle_ast_ExprTypeInfo>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprPtr2Ref ( self:AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const )  

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprPtr2Ref <handle_ast_ExprPtr2Ref>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprPtr2Ref ( self:AstVisitor; expr:smart_ptr<ast::ExprPtr2Ref> const )  : ExpressionPtr 

visitExprPtr2Ref returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprPtr2Ref <handle_ast_ExprPtr2Ref>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprLabel ( self:AstVisitor; expr:smart_ptr<ast::ExprLabel> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLabel <handle_ast_ExprLabel>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprLabel ( self:AstVisitor; expr:smart_ptr<ast::ExprLabel> const )  : ExpressionPtr 

visitExprLabel returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprLabel <handle_ast_ExprLabel>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprGoto ( self:AstVisitor; expr:smart_ptr<ast::ExprGoto> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprGoto <handle_ast_ExprGoto>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprGoto ( self:AstVisitor; expr:smart_ptr<ast::ExprGoto> const )  : ExpressionPtr 

visitExprGoto returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprGoto <handle_ast_ExprGoto>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprRef2Value ( self:AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Value <handle_ast_ExprRef2Value>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprRef2Value ( self:AstVisitor; expr:smart_ptr<ast::ExprRef2Value> const )  : ExpressionPtr 

visitExprRef2Value returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Value <handle_ast_ExprRef2Value>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprRef2Ptr ( self:AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const )  

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Ptr <handle_ast_ExprRef2Ptr>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprRef2Ptr ( self:AstVisitor; expr:smart_ptr<ast::ExprRef2Ptr> const )  : ExpressionPtr 

visitExprRef2Ptr returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprRef2Ptr <handle_ast_ExprRef2Ptr>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAddr ( self:AstVisitor; expr:smart_ptr<ast::ExprAddr> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAddr <handle_ast_ExprAddr>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAddr ( self:AstVisitor; expr:smart_ptr<ast::ExprAddr> const )  : ExpressionPtr 

visitExprAddr returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAddr <handle_ast_ExprAddr>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAssert ( self:AstVisitor; expr:smart_ptr<ast::ExprAssert> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAssert <handle_ast_ExprAssert>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAssert ( self:AstVisitor; expr:smart_ptr<ast::ExprAssert> const )  : ExpressionPtr 

visitExprAssert returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAssert <handle_ast_ExprAssert>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprStaticAssert ( self:AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const )  

method arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStaticAssert <handle_ast_ExprStaticAssert>` > const+
+----+-----------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprStaticAssert ( self:AstVisitor; expr:smart_ptr<ast::ExprStaticAssert> const )  : ExpressionPtr 

visitExprStaticAssert returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprStaticAssert <handle_ast_ExprStaticAssert>` > const+
+----+-----------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprDebug ( self:AstVisitor; expr:smart_ptr<ast::ExprDebug> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDebug <handle_ast_ExprDebug>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprDebug ( self:AstVisitor; expr:smart_ptr<ast::ExprDebug> const )  : ExpressionPtr 

visitExprDebug returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDebug <handle_ast_ExprDebug>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprInvoke ( self:AstVisitor; expr:smart_ptr<ast::ExprInvoke> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprInvoke <handle_ast_ExprInvoke>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprInvoke ( self:AstVisitor; expr:smart_ptr<ast::ExprInvoke> const )  : ExpressionPtr 

visitExprInvoke returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprInvoke <handle_ast_ExprInvoke>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprErase ( self:AstVisitor; expr:smart_ptr<ast::ExprErase> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprErase <handle_ast_ExprErase>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprErase ( self:AstVisitor; expr:smart_ptr<ast::ExprErase> const )  : ExpressionPtr 

visitExprErase returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprErase <handle_ast_ExprErase>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFind ( self:AstVisitor; expr:smart_ptr<ast::ExprFind> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFind <handle_ast_ExprFind>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFind ( self:AstVisitor; expr:smart_ptr<ast::ExprFind> const )  : ExpressionPtr 

visitExprFind returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFind <handle_ast_ExprFind>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprKeyExists ( self:AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprKeyExists <handle_ast_ExprKeyExists>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprKeyExists ( self:AstVisitor; expr:smart_ptr<ast::ExprKeyExists> const )  : ExpressionPtr 

visitExprKeyExists returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprKeyExists <handle_ast_ExprKeyExists>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAscend ( self:AstVisitor; expr:smart_ptr<ast::ExprAscend> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAscend <handle_ast_ExprAscend>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAscend ( self:AstVisitor; expr:smart_ptr<ast::ExprAscend> const )  : ExpressionPtr 

visitExprAscend returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAscend <handle_ast_ExprAscend>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprCast ( self:AstVisitor; expr:smart_ptr<ast::ExprCast> const )  

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCast <handle_ast_ExprCast>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprCast ( self:AstVisitor; expr:smart_ptr<ast::ExprCast> const )  : ExpressionPtr 

visitExprCast returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`              +
+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprCast <handle_ast_ExprCast>` > const+
+----+-------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprDelete ( self:AstVisitor; expr:smart_ptr<ast::ExprDelete> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDelete <handle_ast_ExprDelete>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprDelete ( self:AstVisitor; expr:smart_ptr<ast::ExprDelete> const )  : ExpressionPtr 

visitExprDelete returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprDelete <handle_ast_ExprDelete>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprVar ( self:AstVisitor; expr:smart_ptr<ast::ExprVar> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprVar <handle_ast_ExprVar>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprVar ( self:AstVisitor; expr:smart_ptr<ast::ExprVar> const )  : ExpressionPtr 

visitExprVar returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprVar <handle_ast_ExprVar>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprField ( self:AstVisitor; expr:smart_ptr<ast::ExprField> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprField <handle_ast_ExprField>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprField ( self:AstVisitor; expr:smart_ptr<ast::ExprField> const )  : ExpressionPtr 

visitExprField returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprField <handle_ast_ExprField>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeField ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeField> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeField <handle_ast_ExprSafeField>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeField ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeField> const )  : ExpressionPtr 

visitExprSafeField returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeField <handle_ast_ExprSafeField>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSwizzle ( self:AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const )  

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSwizzle <handle_ast_ExprSwizzle>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSwizzle ( self:AstVisitor; expr:smart_ptr<ast::ExprSwizzle> const )  : ExpressionPtr 

visitExprSwizzle returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSwizzle <handle_ast_ExprSwizzle>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprIsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle_ast_ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprIsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprIsVariant> const )  : ExpressionPtr 

visitExprIsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprIsVariant <handle_ast_ExprIsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprAsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle_ast_ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprAsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprAsVariant> const )  : ExpressionPtr 

visitExprAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprAsVariant <handle_ast_ExprAsVariant>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprSafeAsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle_ast_ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprSafeAsVariant ( self:AstVisitor; expr:smart_ptr<ast::ExprSafeAsVariant> const )  : ExpressionPtr 

visitExprSafeAsVariant returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprSafeAsVariant <handle_ast_ExprSafeAsVariant>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprOp1 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp1> const )  

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp1 <handle_ast_ExprOp1>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprOp1 ( self:AstVisitor; expr:smart_ptr<ast::ExprOp1> const )  : ExpressionPtr 

visitExprOp1 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`            +
+----+-----------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprOp1 <handle_ast_ExprOp1>` > const+
+----+-----------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprReturn ( self:AstVisitor; expr:smart_ptr<ast::ExprReturn> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReturn <handle_ast_ExprReturn>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprReturn ( self:AstVisitor; expr:smart_ptr<ast::ExprReturn> const )  : ExpressionPtr 

visitExprReturn returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReturn <handle_ast_ExprReturn>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprYield ( self:AstVisitor; expr:smart_ptr<ast::ExprYield> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprYield <handle_ast_ExprYield>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprYield ( self:AstVisitor; expr:smart_ptr<ast::ExprYield> const )  : ExpressionPtr 

visitExprYield returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprYield <handle_ast_ExprYield>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprBreak ( self:AstVisitor; expr:smart_ptr<ast::ExprBreak> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprBreak <handle_ast_ExprBreak>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprBreak ( self:AstVisitor; expr:smart_ptr<ast::ExprBreak> const )  : ExpressionPtr 

visitExprBreak returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprBreak <handle_ast_ExprBreak>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprContinue ( self:AstVisitor; expr:smart_ptr<ast::ExprContinue> const )  

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprContinue <handle_ast_ExprContinue>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprContinue ( self:AstVisitor; expr:smart_ptr<ast::ExprContinue> const )  : ExpressionPtr 

visitExprContinue returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprContinue <handle_ast_ExprContinue>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeBlock ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeBlock <handle_ast_ExprMakeBlock>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeBlock ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeBlock> const )  : ExpressionPtr 

visitExprMakeBlock returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeBlock <handle_ast_ExprMakeBlock>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMakeGenerator ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeGenerator <handle_ast_ExprMakeGenerator>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMakeGenerator ( self:AstVisitor; expr:smart_ptr<ast::ExprMakeGenerator> const )  : ExpressionPtr 

visitExprMakeGenerator returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMakeGenerator <handle_ast_ExprMakeGenerator>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprMemZero ( self:AstVisitor; expr:smart_ptr<ast::ExprMemZero> const )  

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMemZero <handle_ast_ExprMemZero>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprMemZero ( self:AstVisitor; expr:smart_ptr<ast::ExprMemZero> const )  : ExpressionPtr 

visitExprMemZero returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                    +
+----+-------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprMemZero <handle_ast_ExprMemZero>` > const+
+----+-------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConst ( self:AstVisitor; expr:smart_ptr<ast::ExprConst> const )  

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConst <handle_ast_ExprConst>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConst ( self:AstVisitor; expr:smart_ptr<ast::ExprConst> const )  : ExpressionPtr 

visitExprConst returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                +
+----+---------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConst <handle_ast_ExprConst>` > const+
+----+---------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstPtr ( self:AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const )  

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstPtr <handle_ast_ExprConstPtr>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstPtr ( self:AstVisitor; expr:smart_ptr<ast::ExprConstPtr> const )  : ExpressionPtr 

visitExprConstPtr returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstPtr <handle_ast_ExprConstPtr>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstEnumeration ( self:AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const )  

method arguments are

+----+-------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+----+-------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstEnumeration <handle_ast_ExprConstEnumeration>` > const+
+----+-------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstEnumeration ( self:AstVisitor; expr:smart_ptr<ast::ExprConstEnumeration> const )  : ExpressionPtr 

visitExprConstEnumeration returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                      +
+----+-------------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstEnumeration <handle_ast_ExprConstEnumeration>` > const+
+----+-------------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstBitfield ( self:AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const )  

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBitfield <handle_ast_ExprConstBitfield>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstBitfield ( self:AstVisitor; expr:smart_ptr<ast::ExprConstBitfield> const )  : ExpressionPtr 

visitExprConstBitfield returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                                +
+----+-------------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBitfield <handle_ast_ExprConstBitfield>` > const+
+----+-------------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt8 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt8 <handle_ast_ExprConstInt8>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt8 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt8> const )  : ExpressionPtr 

visitExprConstInt8 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt8 <handle_ast_ExprConstInt8>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt16 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt16 <handle_ast_ExprConstInt16>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt16 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt16> const )  : ExpressionPtr 

visitExprConstInt16 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt16 <handle_ast_ExprConstInt16>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt64 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt64 <handle_ast_ExprConstInt64>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt64 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt64> const )  : ExpressionPtr 

visitExprConstInt64 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt64 <handle_ast_ExprConstInt64>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt> const )  

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt <handle_ast_ExprConstInt>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt> const )  : ExpressionPtr 

visitExprConstInt returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                      +
+----+---------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt <handle_ast_ExprConstInt>` > const+
+----+---------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt2 <handle_ast_ExprConstInt2>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt2> const )  : ExpressionPtr 

visitExprConstInt2 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt2 <handle_ast_ExprConstInt2>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt3 <handle_ast_ExprConstInt3>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt3> const )  : ExpressionPtr 

visitExprConstInt3 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt3 <handle_ast_ExprConstInt3>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstInt4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt4 <handle_ast_ExprConstInt4>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstInt4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstInt4> const )  : ExpressionPtr 

visitExprConstInt4 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstInt4 <handle_ast_ExprConstInt4>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt8 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt8 <handle_ast_ExprConstUInt8>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt8 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt8> const )  : ExpressionPtr 

visitExprConstUInt8 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt8 <handle_ast_ExprConstUInt8>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt16 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt16 <handle_ast_ExprConstUInt16>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt16 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt16> const )  : ExpressionPtr 

visitExprConstUInt16 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt16 <handle_ast_ExprConstUInt16>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt64 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt64 <handle_ast_ExprConstUInt64>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt64 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt64> const )  : ExpressionPtr 

visitExprConstUInt64 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt64 <handle_ast_ExprConstUInt64>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt <handle_ast_ExprConstUInt>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt> const )  : ExpressionPtr 

visitExprConstUInt returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt <handle_ast_ExprConstUInt>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt2 <handle_ast_ExprConstUInt2>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt2> const )  : ExpressionPtr 

visitExprConstUInt2 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt2 <handle_ast_ExprConstUInt2>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt3 <handle_ast_ExprConstUInt3>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt3> const )  : ExpressionPtr 

visitExprConstUInt3 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt3 <handle_ast_ExprConstUInt3>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstUInt4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt4 <handle_ast_ExprConstUInt4>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstUInt4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstUInt4> const )  : ExpressionPtr 

visitExprConstUInt4 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstUInt4 <handle_ast_ExprConstUInt4>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstRange ( self:AstVisitor; expr:smart_ptr<ast::ExprConstRange> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstRange <handle_ast_ExprConstRange>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstRange ( self:AstVisitor; expr:smart_ptr<ast::ExprConstRange> const )  : ExpressionPtr 

visitExprConstRange returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstRange <handle_ast_ExprConstRange>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstURange ( self:AstVisitor; expr:smart_ptr<ast::ExprConstURange> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstURange <handle_ast_ExprConstURange>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstURange ( self:AstVisitor; expr:smart_ptr<ast::ExprConstURange> const )  : ExpressionPtr 

visitExprConstURange returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstURange <handle_ast_ExprConstURange>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstBool ( self:AstVisitor; expr:smart_ptr<ast::ExprConstBool> const )  

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBool <handle_ast_ExprConstBool>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstBool ( self:AstVisitor; expr:smart_ptr<ast::ExprConstBool> const )  : ExpressionPtr 

visitExprConstBool returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                        +
+----+-----------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstBool <handle_ast_ExprConstBool>` > const+
+----+-----------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const )  

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat <handle_ast_ExprConstFloat>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat> const )  : ExpressionPtr 

visitExprConstFloat returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                          +
+----+-------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat <handle_ast_ExprConstFloat>` > const+
+----+-------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat2 <handle_ast_ExprConstFloat2>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat2 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat2> const )  : ExpressionPtr 

visitExprConstFloat2 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat2 <handle_ast_ExprConstFloat2>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat3 <handle_ast_ExprConstFloat3>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat3 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat3> const )  : ExpressionPtr 

visitExprConstFloat3 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat3 <handle_ast_ExprConstFloat3>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstFloat4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat4 <handle_ast_ExprConstFloat4>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstFloat4 ( self:AstVisitor; expr:smart_ptr<ast::ExprConstFloat4> const )  : ExpressionPtr 

visitExprConstFloat4 returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstFloat4 <handle_ast_ExprConstFloat4>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstString ( self:AstVisitor; expr:smart_ptr<ast::ExprConstString> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstString <handle_ast_ExprConstString>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstString ( self:AstVisitor; expr:smart_ptr<ast::ExprConstString> const )  : ExpressionPtr 

visitExprConstString returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstString <handle_ast_ExprConstString>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprConstDouble ( self:AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstDouble <handle_ast_ExprConstDouble>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprConstDouble ( self:AstVisitor; expr:smart_ptr<ast::ExprConstDouble> const )  : ExpressionPtr 

visitExprConstDouble returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprConstDouble <handle_ast_ExprConstDouble>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFakeContext ( self:AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const )  

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeContext <handle_ast_ExprFakeContext>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFakeContext ( self:AstVisitor; expr:smart_ptr<ast::ExprFakeContext> const )  : ExpressionPtr 

visitExprFakeContext returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+---------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                            +
+----+---------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeContext <handle_ast_ExprFakeContext>` > const+
+----+---------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprFakeLineInfo ( self:AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const )  

method arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeLineInfo <handle_ast_ExprFakeLineInfo>` > const+
+----+-----------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprFakeLineInfo ( self:AstVisitor; expr:smart_ptr<ast::ExprFakeLineInfo> const )  : ExpressionPtr 

visitExprFakeLineInfo returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                              +
+----+-----------------------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprFakeLineInfo <handle_ast_ExprFakeLineInfo>` > const+
+----+-----------------------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.preVisitExprReader ( self:AstVisitor; expr:smart_ptr<ast::ExprReader> const )  

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle_ast_ExprReader>` > const+
+----+-----------------------------------------------------------------+


to be documented


.. das:method:: AstVisitor.visitExprReader ( self:AstVisitor; expr:smart_ptr<ast::ExprReader> const )  : ExpressionPtr 

visitExprReader returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 

method arguments are

+----+-----------------------------------------------------------------+
+self+ :ref:`ast::AstVisitor <struct-ast-AstVisitor>`                  +
+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::ExprReader <handle_ast_ExprReader>` > const+
+----+-----------------------------------------------------------------+


to be documented



++++++++
Generics
++++++++

.. das:function:: ExpressionPtr ( expr:smart_ptr<auto(TT)> const )  : ExpressionPtr

ExpressionPtr returns  :ref:`ExpressionPtr <alias_ExpressionPtr>` 



function arguments are

+----+-------------------------+
+expr+smart_ptr<auto(TT)> const+
+----+-------------------------+



to be documented


.. das:function:: add_new_function_annotation ( name:string const; someClassPtr:auto const )  : auto

add_new_function_annotation returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_reader_macro ( name:string const; someClassPtr:auto const )  : auto

add_new_reader_macro returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_structure_annotation ( name:string const; someClassPtr:auto const )  : auto

add_new_structure_annotation returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: add_new_variant_macro ( name:string const; someClassPtr:auto const )  : auto

add_new_variant_macro returns auto



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: describe ( expr:smart_ptr<ast::Expression> const )  : auto

describe returns auto



function arguments are

+----+-----------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` > const+
+----+-----------------------------------------------------------------+



to be documented


.. das:function:: describe ( expr:smart_ptr<ast::Function> const )  : auto

describe returns auto



function arguments are

+----+-------------------------------------------------------------+
+expr+smart_ptr< :ref:`ast::Function <handle_ast_Function>` > const+
+----+-------------------------------------------------------------+



to be documented


.. das:function:: describe ( decl:smart_ptr<ast::TypeDecl> const; extra:bool const; contracts:bool const; modules:bool const )  : auto

describe returns auto



function arguments are

+---------+-------------------------------------------------------------+
+decl     +smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const+
+---------+-------------------------------------------------------------+
+extra    +bool const                                                   +
+---------+-------------------------------------------------------------+
+contracts+bool const                                                   +
+---------+-------------------------------------------------------------+
+modules  +bool const                                                   +
+---------+-------------------------------------------------------------+



to be documented


.. das:function:: make_function_annotation ( name:string const; someClassPtr:auto const )  : FunctionAnnotationPtr

make_function_annotation returns  :ref:`FunctionAnnotationPtr <alias_FunctionAnnotationPtr>` 



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_pass_macro ( name:string const; someClassPtr:auto const )  : PassMacroPtr

make_pass_macro returns  :ref:`PassMacroPtr <alias_PassMacroPtr>` 



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_reader_macro ( name:string const; someClassPtr:auto const )  : ReaderMacroPtr

make_reader_macro returns  :ref:`ReaderMacroPtr <alias_ReaderMacroPtr>` 



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_structure_annotation ( name:string const; someClassPtr:auto const )  : StructureAnnotationPtr

make_structure_annotation returns  :ref:`StructureAnnotationPtr <alias_StructureAnnotationPtr>` 



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_variant_macro ( name:string const; someClassPtr:auto const )  : VariantMacroPtr

make_variant_macro returns  :ref:`VariantMacroPtr <alias_VariantMacroPtr>` 



function arguments are

+------------+------------+
+name        +string const+
+------------+------------+
+someClassPtr+auto const  +
+------------+------------+



to be documented


.. das:function:: make_visitor ( someClass:auto const )  : smart_ptr<ast::VisitorAdapter>

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle_ast_VisitorAdapter>` >



function arguments are

+---------+----------+
+someClass+auto const+
+---------+----------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: add_dirty_infer_macro ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::PassMacro> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit              +
+----+------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::PassMacro <handle_ast_PassMacro>` > const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: add_function ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::Function> const implicit ) 



function arguments are

+----+----------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit            +
+----+----------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::Function <handle_ast_Function>` > const implicit+
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: add_function_annotation ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                                +
+----+------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::FunctionAnnotation <handle_ast_FunctionAnnotation>` > const implicit+
+----+------------------------------------------------------------------------------------------+



to be documented


.. das:function:: add_function_annotation ( arg0:smart_ptr<ast::Function> const implicit; arg1:smart_ptr<ast::FunctionAnnotation> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Function <handle_ast_Function>` > const implicit                    +
+----+------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::FunctionAnnotation <handle_ast_FunctionAnnotation>` > const implicit+
+----+------------------------------------------------------------------------------------------+



to be documented


.. das:function:: add_reader_macro ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::ReaderMacro> const implicit ) 



function arguments are

+----+----------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                  +
+----+----------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::ReaderMacro <handle_ast_ReaderMacro>` > const implicit+
+----+----------------------------------------------------------------------------+



to be documented


.. das:function:: add_structure_annotation ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit ) 



function arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                                  +
+----+--------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::StructureAnnotation <handle_ast_StructureAnnotation>` > const implicit+
+----+--------------------------------------------------------------------------------------------+



to be documented


.. das:function:: add_structure_annotation ( arg0:smart_ptr<ast::Structure> const implicit; arg1:smart_ptr<ast::StructureAnnotation> const implicit ) 



function arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Structure <handle_ast_Structure>` > const implicit                    +
+----+--------------------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::StructureAnnotation <handle_ast_StructureAnnotation>` > const implicit+
+----+--------------------------------------------------------------------------------------------+



to be documented


.. das:function:: add_variant_macro ( arg0:rtti::Module? const implicit; arg1:smart_ptr<ast::VariantMacro> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                    +
+----+------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::VariantMacro <handle_ast_VariantMacro>` > const implicit+
+----+------------------------------------------------------------------------------+



to be documented


.. das:function:: any_array_foreach ( arg0:void? const implicit; arg1:int const; arg2:block<(void?):void> const implicit ) 



function arguments are

+----+----------------------------------+
+arg0+void? const implicit              +
+----+----------------------------------+
+arg1+int const                         +
+----+----------------------------------+
+arg2+block<(void?):void> const implicit+
+----+----------------------------------+



to be documented


.. das:function:: any_array_size ( arg0:void? const implicit )  : int

any_array_size returns int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: any_table_foreach ( arg0:void? const implicit; arg1:int const; arg2:int const; arg3:block<(void?;void?):void> const implicit ) 



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



to be documented


.. das:function:: any_table_size ( arg0:void? const implicit )  : int

any_table_size returns int



function arguments are

+----+--------------------+
+arg0+void? const implicit+
+----+--------------------+



to be documented


.. das:function:: clone_expression ( arg0:smart_ptr<ast::Expression> const implicit )  : smart_ptr<ast::Expression>

clone_expression returns smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` >



function arguments are

+----+--------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` > const implicit+
+----+--------------------------------------------------------------------------+



to be documented


.. das:function:: clone_type ( arg0:smart_ptr<ast::TypeDecl> const implicit )  : smart_ptr<ast::TypeDecl>

clone_type returns smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: compiling_module

compiling_module returns  :ref:`rtti::Module <handle_rtti_Module>` ?




to be documented


.. das:function:: compiling_program

compiling_program returns smart_ptr< :ref:`rtti::Program <handle_rtti_Program>` >




to be documented


.. das:function:: das_to_string ( arg0:Type const )  : string

das_to_string returns string



function arguments are

+----+------------------------------------------+
+arg0+ :ref:`rtti::Type <enum_rtti_Type>`  const+
+----+------------------------------------------+



to be documented


.. das:function:: describe_expression ( arg0:smart_ptr<ast::Expression> const implicit )  : string

describe_expression returns string



function arguments are

+----+--------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Expression <handle_ast_Expression>` > const implicit+
+----+--------------------------------------------------------------------------+



to be documented


.. das:function:: describe_function ( arg0:smart_ptr<ast::Function> const implicit )  : string

describe_function returns string



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::Function <handle_ast_Function>` > const implicit+
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: describe_typedecl ( arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bool const; arg2:bool const; arg3:bool const )  : string

describe_typedecl returns string



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+bool const                                                            +
+----+----------------------------------------------------------------------+
+arg2+bool const                                                            +
+----+----------------------------------------------------------------------+
+arg3+bool const                                                            +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: find_bitfield_name ( arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:bitfield const )  : string

find_bitfield_name returns string



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+bitfield<> const                                                      +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: for_each_enumeration ( arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Enumeration>):void> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                                +
+----+------------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Enumeration <handle_ast_Enumeration>` >):void> const implicit+
+----+------------------------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_field ( arg0:BasicStructureAnnotation const implicit; arg1:block<(string;string;smart_ptr<ast::TypeDecl>;uint):void> const implicit ) 



function arguments are

+----+-------------------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::BasicStructureAnnotation <handle_rtti_BasicStructureAnnotation>`  const implicit          +
+----+-------------------------------------------------------------------------------------------------------+
+arg1+block<(string;string;smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >;uint):void> const implicit+
+----+-------------------------------------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_function ( arg0:rtti::Module? const implicit; arg1:string const; arg2:block<(smart_ptr<ast::Function>):void> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                          +
+----+------------------------------------------------------------------------------------+
+arg1+string const                                                                        +
+----+------------------------------------------------------------------------------------+
+arg2+block<(smart_ptr< :ref:`ast::Function <handle_ast_Function>` >):void> const implicit+
+----+------------------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_generic ( arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Function>):void> const implicit ) 



function arguments are

+----+------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                          +
+----+------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Function <handle_ast_Function>` >):void> const implicit+
+----+------------------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_structure ( arg0:rtti::Module? const implicit; arg1:block<(smart_ptr<ast::Structure>):void> const implicit ) 



function arguments are

+----+--------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                            +
+----+--------------------------------------------------------------------------------------+
+arg1+block<(smart_ptr< :ref:`ast::Structure <handle_ast_Structure>` >):void> const implicit+
+----+--------------------------------------------------------------------------------------+



to be documented


.. das:function:: for_each_typedef ( arg0:rtti::Module? const implicit; arg1:block<(string#;smart_ptr<ast::TypeDecl>):void> const implicit ) 



function arguments are

+----+--------------------------------------------------------------------------------------------+
+arg0+ :ref:`rtti::Module <handle_rtti_Module>` ? const implicit                                  +
+----+--------------------------------------------------------------------------------------------+
+arg1+block<(string#;smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` >):void> const implicit+
+----+--------------------------------------------------------------------------------------------+



to be documented


.. das:function:: get_tuple_field_offset ( arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const )  : int

get_tuple_field_offset returns int



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+int const                                                             +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: get_variant_field_offset ( arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:int const )  : int

get_variant_field_offset returns int



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+int const                                                             +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: is_same_type ( arg0:smart_ptr<ast::TypeDecl> const implicit; arg1:smart_ptr<ast::TypeDecl> const implicit; arg2:RefMatters const; arg3:ConstMatters const; arg4:TemporaryMatters const )  : bool

is_same_type returns bool



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::TypeDecl <handle_ast_TypeDecl>` > const implicit+
+----+----------------------------------------------------------------------+
+arg2+ :ref:`rtti::RefMatters <enum_rtti_RefMatters>`  const                +
+----+----------------------------------------------------------------------+
+arg3+ :ref:`rtti::ConstMatters <enum_rtti_ConstMatters>`  const            +
+----+----------------------------------------------------------------------+
+arg4+ :ref:`rtti::TemporaryMatters <enum_rtti_TemporaryMatters>`  const    +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: macro_error ( arg0:smart_ptr<rtti::Program> const implicit; arg1:LineInfo const implicit; arg2:string const ) 



function arguments are

+----+----------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`rtti::Program <handle_rtti_Program>` > const implicit+
+----+----------------------------------------------------------------------+
+arg1+ :ref:`rtti::LineInfo <handle_rtti_LineInfo>`  const implicit         +
+----+----------------------------------------------------------------------+
+arg2+string const                                                          +
+----+----------------------------------------------------------------------+



to be documented


.. das:function:: make_function_annotation ( arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::FunctionAnnotation>

make_function_annotation returns smart_ptr< :ref:`ast::FunctionAnnotation <handle_ast_FunctionAnnotation>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: make_pass_macro ( arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::PassMacro>

make_pass_macro returns smart_ptr< :ref:`ast::PassMacro <handle_ast_PassMacro>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: make_reader_macro ( arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::ReaderMacro>

make_reader_macro returns smart_ptr< :ref:`ast::ReaderMacro <handle_ast_ReaderMacro>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: make_structure_annotation ( arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::StructureAnnotation>

make_structure_annotation returns smart_ptr< :ref:`ast::StructureAnnotation <handle_ast_StructureAnnotation>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: make_variant_macro ( arg0:string const; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit )  : smart_ptr<ast::VariantMacro>

make_variant_macro returns smart_ptr< :ref:`ast::VariantMacro <handle_ast_VariantMacro>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+string const                                                            +
+----+------------------------------------------------------------------------+
+arg1+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: make_visitor ( arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit )  : smart_ptr<ast::VisitorAdapter>

make_visitor returns smart_ptr< :ref:`ast::VisitorAdapter <handle_ast_VisitorAdapter>` >



function arguments are

+----+------------------------------------------------------------------------+
+arg0+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg1+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: this_module (  )  : rtti::Module?

this_module returns  :ref:`rtti::Module <handle_rtti_Module>` ?



function arguments are

+



to be documented


.. das:function:: this_program (  )  : smart_ptr<rtti::Program>

this_program returns smart_ptr< :ref:`rtti::Program <handle_rtti_Program>` >



function arguments are

+



to be documented


.. das:function:: visit ( arg0:smart_ptr<rtti::Program> const implicit; arg1:smart_ptr<ast::VisitorAdapter> const implicit ) 



function arguments are

+----+----------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`rtti::Program <handle_rtti_Program>` > const implicit            +
+----+----------------------------------------------------------------------------------+
+arg1+smart_ptr< :ref:`ast::VisitorAdapter <handle_ast_VisitorAdapter>` > const implicit+
+----+----------------------------------------------------------------------------------+



to be documented



