
.. _stdlib_ast:

========================
AST manipulation library
========================

The AST module implements compilation time reflection for the Daslang syntax tree.

All functions and symbols are in "ast" module, use require to get access to it. ::

    require ast

++++++++++++
Type aliases
++++++++++++

.. _alias-TypeDeclFlags:

.. das:attribute:: bitfield TypeDeclFlags

properties of the `TypeDecl` object.

:Fields: * **ref** (0x1) - The type is a reference type.

         * **constant** (0x2) - The type is a constant type.

         * **temporary** (0x4) - The type is a temporary type.

         * **_implicit** (0x8) - The type is an implicit type.

         * **removeRef** (0x10) - Remove the reference flag.

         * **removeConstant** (0x20) - Remove the constant flag.

         * **removeDim** (0x40) - Remove the dimension flag.

         * **removeTemporary** (0x80) - Remove the temporary flag.

         * **explicitConst** (0x100) - The type is an explicit constant type.

         * **aotAlias** (0x200) - The type is an AOT alias.

         * **smartPtr** (0x400) - The type is a smart pointer type.

         * **smartPtrNative** (0x800) - The type is a native smart pointer type (smart_ptr_raw).

         * **isExplicit** (0x1000) - The type is explicit.

         * **isNativeDim** (0x2000) - The type is a native dimension.

         * **isTag** (0x4000) - The type is a reification tag.

         * **explicitRef** (0x8000) - The type is an explicit reference.

         * **isPrivateAlias** (0x10000) - The type is a private alias.

         * **autoToAlias** (0x20000) - The type is an auto-to-alias.


.. _alias-FieldDeclarationFlags:

.. das:attribute:: bitfield FieldDeclarationFlags

properties of the `FieldDeclaration` object.

:Fields: * **moveSemantics** (0x1) - The field is initialized using move semantics.

         * **parentType** (0x2) - Which parent type this field belongs to.

         * **capturedConstant** (0x4) - This field is a captured constant (via lambda or generator).

         * **generated** (0x8) - This field is compiler-generated.

         * **capturedRef** (0x10) - This field is a captured reference (via lambda or generator).

         * **doNotDelete** (0x20) - Has @do_not_delete attribute.

         * **privateField** (0x40) - This field is private.

         * **_sealed** (0x80) - The field is sealed. It cannot be overridden in derived types.

         * **implemented** (0x100) - Already implemented.

         * **classMethod** (0x200) - This field is a class method.


.. _alias-StructureFlags:

.. das:attribute:: bitfield StructureFlags

properties of the `Structure` object.

:Fields: * **isClass** (0x1) - The structure is a class.

         * **genCtor** (0x2) - Generate constructor.

         * **cppLayout** (0x4) - C++ data layout.

         * **cppLayoutNotPod** (0x8) - C++ layout not POD type, i.e. has alignment to accommodate for inheritance.

         * **generated** (0x10) - This structure is compiler-generated.

         * **persistent** (0x20) - This structure is using persistent heap (C++ heap).

         * **isLambda** (0x40) - This structure is a lambda.

         * **privateStructure** (0x80) - This structure is private.

         * **macroInterface** (0x100) - This structure is a macro interface.

         * **_sealed** (0x200) - This structure is sealed. It cannot be inherited.

         * **skipLockCheck** (0x400) - Skip lock check.

         * **circular** (0x800) - This structure has circulare references (and is invalid).

         * **_generator** (0x1000) - This structure is a generator.

         * **hasStaticMembers** (0x2000) - This structure has static members.

         * **hasStaticFunctions** (0x4000) - This structure has static functions.

         * **hasInitFields** (0x8000) - This structure has initialized fields.

         * **safeWhenUninitialized** (0x10000) - This structure is safe when uninitialized.

         * **isTemplate** (0x20000) - This structure is a template.

         * **hasDefaultInitializer** (0x40000) - This structure has a default initializer.


.. _alias-ExprGenFlags:

.. das:attribute:: bitfield ExprGenFlags

generation (genFlags) properties of the `Expression` object.

:Fields: * **alwaysSafe** (0x1) - Expression is always safe.

         * **generated** (0x2) - Expression is compiler-generated.

         * **userSaidItsSafe** (0x4) - Expression is marked as safe explicitly.


.. _alias-ExprLetFlags:

.. das:attribute:: bitfield ExprLetFlags

properties of the `ExprLet` object.

:Fields: * **inScope** (0x1) - It's 'let inscope' expression.

         * **hasEarlyOut** (0x2) - It's 'let hasEarlyOut' expression.

         * **itTupleExpansion** (0x4) - It's 'let itTupleExpansion' expression.


.. _alias-ExprFlags:

.. das:attribute:: bitfield ExprFlags

properties of the `Expression` object.

:Fields: * **constexpression** (0x1) - Expression is a constant expression.

         * **noSideEffects** (0x2) - Expression has no side effects.

         * **noNativeSideEffects** (0x4) - Expression has no native side effects, i.e. expression itself has no sideeffects.

         * **isForLoopSource** (0x8) - Expression is a for loop source.

         * **isCallArgument** (0x10) - Expression is a call argument.


.. _alias-ExprPrintFlags:

.. das:attribute:: bitfield ExprPrintFlags

printing properties of the `Expression` object.

:Fields: * **topLevel** (0x1) - Its a top level expression.

         * **argLevel** (0x2) - Its an argument level expression.

         * **bottomLevel** (0x4) - Its a bottom level expression - no sub-expressions or nesting.


.. _alias-FunctionFlags:

.. das:attribute:: bitfield FunctionFlags

properties of the `Function` object.

:Fields: * **builtIn** (0x1) - Function is built-in.

         * **policyBased** (0x2) - Function is policy-based.

         * **callBased** (0x4) - Function is call-based.

         * **interopFn** (0x8) - Function is interop function.

         * **hasReturn** (0x10) - Function has a return value.

         * **copyOnReturn** (0x20) - Function copies return value.

         * **moveOnReturn** (0x40) - Function moves return value.

         * **exports** (0x80) - Its an exported function.

         * **init** (0x100) - Its an init function.

         * **addr** (0x200) - Function has address requested.

         * **used** (0x400) - Function is used.

         * **fastCall** (0x800) - Function is fast call.

         * **knownSideEffects** (0x1000) - Function has known side effects (user defined).

         * **hasToRunAtCompileTime** (0x2000) - Function has to run at compile time.

         * **unsafeOperation** (0x4000) - Function is unsafe operation.

         * **unsafeDeref** (0x8000) - All dereferences in the function will be simulated without safety checks.

         * **hasMakeBlock** (0x10000) - Function has 'make block' operation.

         * **aotNeedPrologue** (0x20000) - Function needs AOT prologue.

         * **noAot** (0x40000) - Function is not AOT.

         * **aotHybrid** (0x80000) - Function is AOT hybrid, i.e. can be called from both AOT and interpreted code. Call in never hardcoded.

         * **aotTemplate** (0x100000) - Function is AOT template, i.e. instantiated from template at C++ compile time.

         * **generated** (0x200000) - Function is compiler-generated.

         * **privateFunction** (0x400000) - Function is private.

         * **_generator** (0x800000) - Function is a generator.

         * **_lambda** (0x1000000) - Function is a lambda.

         * **firstArgReturnType** (0x2000000) - First argument type is return type.

         * **noPointerCast** (0x4000000) - Function has no pointer cast.

         * **isClassMethod** (0x8000000) - Function is a class method.

         * **isTypeConstructor** (0x10000000) - Function is a type constructor.

         * **shutdown** (0x20000000) - Function is a shutdown function.

         * **anyTemplate** (0x40000000) - Function is any template.

         * **macroInit** (0x80000000) - Function is macro init.


.. _alias-MoreFunctionFlags:

.. das:attribute:: bitfield MoreFunctionFlags

additional properties of the `Function` object.

:Fields: * **macroFunction** (0x1) - Function is a macro function.

         * **needStringCast** (0x2) - Converts das string arguments to C++ char *. Empty string, which is null in das, is converted to "".

         * **aotHashDeppendsOnArguments** (0x4) - Function hash depends on arguments.

         * **lateInit** (0x8) - Function is late initialized.

         * **requestJit** (0x10) - Function is requested to be JIT compiled.

         * **unsafeOutsideOfFor** (0x20) - Function is unsafe outside of for loop sources.

         * **skipLockCheck** (0x40) - Skip lock check for this function.

         * **safeImplicit** (0x80) - Function is safe for implicit calls. Otherwise temp values are to be specialized for in the generic.

         * **deprecated** (0x100) - Function is deprecated.

         * **aliasCMRES** (0x200) - Function aliases CMRES (Copy or Move return result).

         * **neverAliasCMRES** (0x400) - Function never aliases CMRES.

         * **addressTaken** (0x800) - Function address is taken.

         * **propertyFunction** (0x1000) - Function is a property function.

         * **pinvoke** (0x2000) - Function is a P/Invoke function, i.e. cross-context call.

         * **jitOnly** (0x4000) - Function is JIT only.

         * **isStaticClassMethod** (0x8000) - Function is a static class method.

         * **requestNoJit** (0x10000) - Function is requested to not be JIT compiled.

         * **jitContextAndLineInfo** (0x20000) - Function requires JIT context and line info.

         * **nodiscard** (0x40000) - Discarding the return value of the function is unsafe.

         * **captureString** (0x80000) - Function captures string arguments.

         * **callCaptureString** (0x100000) - Function calls capture string arguments.

         * **hasStringBuilder** (0x200000) - Function has a string builder.

         * **recursive** (0x400000) - Function is recursive.

         * **isTemplate** (0x800000) - Function is a template function.


.. _alias-FunctionSideEffectFlags:

.. das:attribute:: bitfield FunctionSideEffectFlags

side-effect properties of the `Function` object.

:Fields: * **_unsafe** (0x1) - Function is unsafe.

         * **userScenario** (0x2) - User specified [sideeffects] annotation to indicate side effects.

         * **modifyExternal** (0x4) - Function may modify external state.

         * **modifyArgument** (0x8) - Function may modify argument values.

         * **accessGlobal** (0x10) - Function may access global state (variables and such).

         * **invoke** (0x20) - Function is using 'invoke', so we don't know any additional side effects.


.. _alias-VariableFlags:

.. das:attribute:: bitfield VariableFlags

properties of the `Variable` object.

:Fields: * **init_via_move** (0x1) - Variable is initialized via move <-

         * **init_via_clone** (0x2) - Variable is initialized via clone :=

         * **used** (0x4) - Variable is used

         * **aliasCMRES** (0x8) - Variable is an alias for CMRES return value

         * **marked_used** (0x10) - Variable is marked as used (to suppress unused warnings)

         * **global_shared** (0x20) - Variable is a global shared variable

         * **do_not_delete** (0x40) - @do_not_delete annotation on the variable

         * **generated** (0x80) - Variable is generated by the compiler

         * **capture_as_ref** (0x100) - Variable is captured by reference in a closure

         * **can_shadow** (0x200) - Variable can shadow another variable in an inner scope

         * **private_variable** (0x400) - Variable is private to the class/struct

         * **tag** (0x800) - Variable is a reification tag

         * **global** (0x1000) - Variable is a global variable

         * **inScope** (0x2000) - Variable is 'let inscope', i.e. there is a coresponding 'delete' in the 'finally' section of the block

         * **no_capture** (0x4000) - This variable will not be captured in lambda (think 'self').

         * **early_out** (0x8000) - There is an early out from the scope where this variable is defined (via return and otherwise)

         * **used_in_finally** (0x10000) - Variable is used in the finally block

         * **static_class_member** (0x20000) - Variable is a static class member

         * **bitfield_constant** (0x40000) - Variable is a bitfield constant


.. _alias-VariableAccessFlags:

.. das:attribute:: bitfield VariableAccessFlags

access properties of the `Variable` object.

:Fields: * **access_extern** (0x1) - Variable is Function or block argument.

         * **access_get** (0x2) - Variable is accessed via get (read of some kind).

         * **access_ref** (0x4) - Variable is accessed via ref (written to).

         * **access_init** (0x8) - Variable is initialized.

         * **access_pass** (0x10) - Variable is passed to a function, or invoke.

         * **access_fold** (0x20) - Variable was folded aways (optimized out).


.. _alias-ExprBlockFlags:

.. das:attribute:: bitfield ExprBlockFlags

properties of the `ExprBlock` object.

:Fields: * **isClosure** (0x1) - Block is a closure, and not a regular expression list.

         * **hasReturn** (0x2) - Block has a return statement.

         * **copyOnReturn** (0x4) - When invoked, the block result is copied on return.

         * **moveOnReturn** (0x8) - When invoked, the block result is moved on return.

         * **inTheLoop** (0x10) - Block is inside a loop.

         * **finallyBeforeBody** (0x20) - Finally is to be visited before the body.

         * **finallyDisabled** (0x40) - Finally is disabled.

         * **aotSkipMakeBlock** (0x80) - AOT is allowed to skip make block generation, and pass [&]() directly.

         * **aotDoNotSkipAnnotationData** (0x100) - AOT should not skip annotation data even if make block is skipped.

         * **isCollapseable** (0x200) - Block is eligible for collapse optimization.

         * **needCollapse** (0x400) - Block needs to be collapsed.

         * **hasMakeBlock** (0x800) - Block has make block operation.

         * **hasEarlyOut** (0x1000) - Block has early out (break/continue/return).

         * **forLoop** (0x2000) - Block is a for loop body.

         * **hasExitByLabel** (0x4000) - Block has exit by label (goto outside).

         * **isLambdaBlock** (0x8000) - Block is a lambda block.


.. _alias-ExprAtFlags:

.. das:attribute:: bitfield ExprAtFlags

properties of the `ExprAt` object.

:Fields: * **r2v** (0x1) - Reference to value conversion is applied.

         * **r2cr** (0x2) - Read to const reference is propagated.

         * **write** (0x4) - The result is written to.

         * **no_promotion** (0x8) - Promotion to operator is disabled, even if operator [] is overloaded.


.. _alias-ExprMakeLocalFlags:

.. das:attribute:: bitfield ExprMakeLocalFlags

properties of the `ExprMakeLocal` object (`ExprMakeArray`, `ExprMakeStruct`, 'ExprMakeTuple', 'ExprMakeVariant').

:Fields: * **useStackRef** (0x1) - Use stack reference, i.e. there is an address on the stack - where the reference is written to.

         * **useCMRES** (0x2) - Result is returned via CMRES pointer. Usually this is 'return <- [[ExprMakeLocal]]'

         * **doesNotNeedSp** (0x4) - Does not need stack pointer, usually due to being part of bigger initialization.

         * **doesNotNeedInit** (0x8) - Does not need field initialization, usually due to being fully initialized via constructor.

         * **initAllFields** (0x10) - Initialize all fields.

         * **alwaysAlias** (0x20) - Always alias the result, so temp value is always allocated on the stack.


.. _alias-ExprAscendFlags:

.. das:attribute:: bitfield ExprAscendFlags

properties of the `ExprAscend` object.

:Fields: * **useStackRef** (0x1) - Use stack reference, i.e. there is an address on the stack - where the reference is written to.

         * **needTypeInfo** (0x2) - Simulated node needs type information at runtime.

         * **isMakeLambda** (0x4) - Is a lambda expression.


.. _alias-ExprCastFlags:

.. das:attribute:: bitfield ExprCastFlags

properties of the `ExprCast` object.

:Fields: * **upcastCast** (0x1) - Upcast cast, i.e. casting from based class to derived class.

         * **reinterpretCast** (0x2) - Reinterpret cast, i.e. casting between unrelated types (like pointer to integer)


.. _alias-ExprVarFlags:

.. das:attribute:: bitfield ExprVarFlags

properties of the `ExprVar` object.

:Fields: * **local** (0x1) - Local variable.

         * **argument** (0x2) - Function argument.

         * **_block** (0x4) - Block argument

         * **thisBlock** (0x8) - Argument of the most-nested block.

         * **r2v** (0x10) - Reference to value conversion is applied.

         * **r2cr** (0x20) - Read to const reference is propagated.

         * **write** (0x40) - Written to.

         * **under_clone** (0x80) - This is a foo := bar expression, and the variable is being cloned to.


.. _alias-ExprMakeStructFlags:

.. das:attribute:: bitfield ExprMakeStructFlags

properties of the `ExprMakeStruct` object.

:Fields: * **useInitializer** (0x1) - Use initializer, i.e. 'Foo(...)', and not 'Foo(uninitialized ...)'.

         * **isNewHandle** (0x2) - Its 'new Foo(...)'.

         * **usedInitializer** (0x4) - 'useInitializer' was used optimized out.

         * **nativeClassInitializer** (0x8) - Generated class initializer.

         * **isNewClass** (0x10) - Its 'new ClassName(...)'.

         * **forceClass** (0x20) - Its declared via 'class'syntax, so using it for regular types will fail.

         * **forceStruct** (0x40) - Its declared via 'struct' syntax, so using it for regular types will fail.

         * **forceVariant** (0x80) - Its declared via 'variant' syntax, so using it for regular types will fail.

         * **forceTuple** (0x100) - Its declared via 'tuple' syntax, so using it for regular types will fail.

         * **alwaysUseInitializer** (0x200) - Always use initializer, even for default construction.

         * **ignoreVisCheck** (0x400) - Ignores visibility check between modules.

         * **canShadowBlock** (0x800) - 'where' section argument can shadow other variables. This is for nested comprehensions and such.


.. _alias-MakeFieldDeclFlags:

.. das:attribute:: bitfield MakeFieldDeclFlags

Properties of the `MakeFieldDecl` object.

:Fields: * **moveSemantics** (0x1) - Initialized with move semantics, <-

         * **cloneSemantics** (0x2) - Initialized with clone semantics, :=


.. _alias-ExprFieldDerefFlags:

.. das:attribute:: bitfield ExprFieldDerefFlags

dereferencing properties of the `ExprField` object.

:Fields: * **unsafeDeref** (0x1) - Dereference without safety checking.

         * **ignoreCaptureConst** (0x2) - Ignore capture const, i.e. was captured as constant - but used as mutable.


.. _alias-ExprFieldFieldFlags:

.. das:attribute:: bitfield ExprFieldFieldFlags

field properties of the `ExprField` object.

:Fields: * **r2v** (0x1) - Reference to value conversion is applied.

         * **r2cr** (0x2) - Read to const reference is propagated.

         * **write** (0x4) - This is part of a write operation, and a field or part of it is being assigned to.

         * **no_promotion** (0x8) - No promotion to property, even if available.

         * **under_clone** (0x10) - Under clone, i.e. 'Foo.bar := ...'


.. _alias-ExprSwizzleFieldFlags:

.. das:attribute:: bitfield ExprSwizzleFieldFlags

properties of the `ExprSwizzle` object.

:Fields: * **r2v** (0x1) - Reference to value conversion is applied.

         * **r2cr** (0x2) - Read to const reference is propagated.

         * **write** (0x4) - This is part of a write operation, and a field or part of it is being assigned to.


.. _alias-ExprYieldFlags:

.. das:attribute:: bitfield ExprYieldFlags

properties of the `ExprYield` object.

:Fields: * **moveSemantics** (0x1) - Its 'yield <- ...'.

         * **skipLockCheck** (0x2) - Skip lock checks.


.. _alias-ExprReturnFlags:

.. das:attribute:: bitfield ExprReturnFlags

properties of the `ExprReturn` object.

:Fields: * **moveSemantics** (0x1) - Its 'return <- ...'.

         * **returnReference** (0x2) - Return a reference. Function result is a reference.

         * **returnInBlock** (0x4) - Return in block, not in function.

         * **takeOverRightStack** (0x8) - Take over right stack, i.e its 'return [MakeLocal]' and temp stack value is allocated by return expression.

         * **returnCallCMRES** (0x10) - Return call CMRES, i.e. 'return call(...)'.

         * **returnCMRES** (0x20) - Return CMRES, i.e. 'return [MakeLocal]' or 'return [CmresVariable]'

         * **fromYield** (0x40) - From yield.

         * **fromComprehension** (0x80) - From comprehension.

         * **skipLockCheck** (0x100) - Skip lock checks.


.. _alias-ExprMakeBlockFlags:

.. das:attribute:: bitfield ExprMakeBlockFlags

properties of the `ExprMakeBlock` object.

:Fields: * **isLambda** (0x1) - Is lambda, i.e. @(...) { ... }

         * **isLocalFunction** (0x2) - Is a local function, i.e. @@(...) { ... }


.. _alias-CopyFlags:

.. das:attribute:: bitfield CopyFlags

properties of the `ExprCopy` object.

:Fields: * **allowCopyTemp** (0x1) - This copy is allowed to copy a temporary value.

         * **takeOverRightStack** (0x2) - Its 'foo = [MakeLocal]' and temp stack value is allocated by copy expression.

         * **promoteToClone** (0x4) - Promote to clone, i.e. this is 'foo := bar' and not 'foo = bar'


.. _alias-MoveFlags:

.. das:attribute:: bitfield MoveFlags

Properties of the `ExprMove` object.

:Fields: * **skipLockCheck** (0x1) - Skip lock checks.

         * **takeOverRightStack** (0x2) - Its 'foo <- [MakeLocal]' and temp stack value is allocated by move expression.


.. _alias-IfFlags:

.. das:attribute:: bitfield IfFlags

properties of the `ExprIf` object.

:Fields: * **isStatic** (0x1) - This is a 'static_if' or 'static_elif' expression.

         * **doNotFold** (0x2) - Do not fold this 'if' expression during compilation.


.. _alias-ExpressionPtr:

.. das:attribute:: ExpressionPtr = smart_ptr<Expression>

 Smart pointer to `Expression` object.

.. _alias-ProgramPtr:

.. das:attribute:: ProgramPtr = smart_ptr<Program>

 Smart pointer to `Program` object.

.. _alias-TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = smart_ptr<TypeDecl>

 Smart pointer to `TypeDecl` object.

.. _alias-VectorTypeDeclPtr:

.. das:attribute:: VectorTypeDeclPtr = dasvector`smart_ptr`TypeDecl

 Smart pointer to das::vector<ExpressionPtr>.

.. _alias-EnumerationPtr:

.. das:attribute:: EnumerationPtr = smart_ptr<Enumeration>

 Smart pointer to `Enumeration` object.

.. _alias-StructurePtr:

.. das:attribute:: StructurePtr = smart_ptr<Structure>

 Smart pointer to `Structure` object.

.. _alias-FunctionPtr:

.. das:attribute:: FunctionPtr = smart_ptr<Function>

 Smart pointer to `Function` object.

.. _alias-VariablePtr:

.. das:attribute:: VariablePtr = smart_ptr<Variable>

 Smart pointer to `Variable` object.

.. _alias-MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = smart_ptr<MakeFieldDecl>

 Smart pointer to `MakeFieldDecl` object.

.. _alias-ExprMakeBlockPtr:

.. das:attribute:: ExprMakeBlockPtr = smart_ptr<ExprMakeBlock>

 Smart pointer to 'ExprMakeBlock'.

.. _alias-FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = smart_ptr<FunctionAnnotation>

 Smart pointer to `FunctionAnnotation` object.

.. _alias-StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = smart_ptr<StructureAnnotation>

 Smart pointer to `StructureAnnotation` object.

.. _alias-EnumerationAnnotationPtr:

.. das:attribute:: EnumerationAnnotationPtr = smart_ptr<EnumerationAnnotation>

 Smart pointer to `EnumerationAnnotation` object.

.. _alias-PassMacroPtr:

.. das:attribute:: PassMacroPtr = smart_ptr<PassMacro>

 Smart pointer to `PassMacro` object.

.. _alias-VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = smart_ptr<VariantMacro>

 Smart pointer to `VariantMacro` object.

.. _alias-ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = smart_ptr<ReaderMacro>

 Smart pointer to `ReaderMacro` object.

.. _alias-CommentReaderPtr:

.. das:attribute:: CommentReaderPtr = smart_ptr<CommentReader>

 Smart pointer to `CommentReader` object.

.. _alias-CallMacroPtr:

.. das:attribute:: CallMacroPtr = smart_ptr<CallMacro>

 Smart pointer to `CallMacro` object.

.. _alias-TypeInfoMacroPtr:

.. das:attribute:: TypeInfoMacroPtr = smart_ptr<TypeInfoMacro>

 Smart pointer to `TypeInfoMacro` object.

.. _alias-ForLoopMacroPtr:

.. das:attribute:: ForLoopMacroPtr = smart_ptr<ForLoopMacro>

 Smart pointer to 'ForLoopMacro'.

.. _alias-CaptureMacroPtr:

.. das:attribute:: CaptureMacroPtr = smart_ptr<CaptureMacro>

 Smart pointer to 'CaptureMacro'.

.. _alias-TypeMacroPtr:

.. das:attribute:: TypeMacroPtr = smart_ptr<TypeMacro>

 Smart pointer to `TypeMacro` object.

.. _alias-SimulateMacroPtr:

.. das:attribute:: SimulateMacroPtr = smart_ptr<SimulateMacro>

 Smart pointer to `SimulateMacro` object.

++++++++++++
Enumerations
++++++++++++

.. _enum-ast-CaptureMode:

.. das:attribute:: CaptureMode

 Enumeration with lambda variables capture modes.

:Values: * **capture_any** = 0

         * **capture_by_copy** = 1

         * **capture_by_reference** = 2

         * **capture_by_clone** = 3

         * **capture_by_move** = 4


.. _enum-ast-SideEffects:

.. das:attribute:: SideEffects

 Enumeration with all possible side effects of expression or function.

:Values: * **none** = 0

         * **unsafe** = 1

         * **userScenario** = 2

         * **modifyExternal** = 4

         * **accessExternal** = 4

         * **modifyArgument** = 8

         * **modifyArgumentAndExternal** = 12

         * **worstDefault** = 12

         * **accessGlobal** = 16

         * **invoke** = 32

         * **inferredSideEffects** = 56


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-ast-ModuleLibrary:

.. das:attribute:: ModuleLibrary

 Object which holds list of `Module` and provides access to them.


.. _handle-ast-Expression:

.. das:attribute:: Expression

 Any expression (base class).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 


.. _handle-ast-TypeDecl:

.. das:attribute:: TypeDecl

.. _function-ast__dot__rq_canAot_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canAot() : bool

Returns whether the given type can be ahead-of-time compiled.

.. _function-ast__dot__rq_isExprType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isExprType() : bool

Returns if somewhere in the type hierarchy the given type is an expression type.

.. _function-ast__dot__rq_isSimpleType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isSimpleType() : bool

Returns if the given type is a simple type, and does not need resolve at infer time. Nor is it `void`.

.. _function-ast__dot__rq_isArray_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isArray() : bool

Returns if the given type is an array type.

.. _function-ast__dot__rq_isGoodIteratorType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodIteratorType() : bool

Returns if the given type is an iterator type.

.. _function-ast__dot__rq_isGoodArrayType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodArrayType() : bool

Returns if the given type is a dynamic array type.

.. _function-ast__dot__rq_isGoodTableType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodTableType() : bool

Returns if the given type is a table type.

.. _function-ast__dot__rq_isGoodBlockType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodBlockType() : bool

Returns if the given type is a block type.

.. _function-ast__dot__rq_isGoodFunctionType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodFunctionType() : bool

Returns if the given type is a function type.

.. _function-ast__dot__rq_isGoodLambdaType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodLambdaType() : bool

Returns if the given type is a lambda type.

.. _function-ast__dot__rq_isGoodTupleType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodTupleType() : bool

Returns if the given type is a tuple type.

.. _function-ast__dot__rq_isGoodVariantType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isGoodVariantType() : bool

Returns if the given type is a variant type.

.. _function-ast__dot__rq_isVoid_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isVoid() : bool

Returns if the given type is a void type.

.. _function-ast__dot__rq_isAnyType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAnyType() : bool

Returns whether the given type is the Any type (C++ side vec4f, passed via standard interop).

.. _function-ast__dot__rq_isRef_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isRef() : bool

Returns if the given type is a reference value.

.. _function-ast__dot__rq_isRefType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isRefType() : bool

Returns if the given type is a reference type.

.. _function-ast__dot__rq_canWrite_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canWrite() : bool

Returns whether the given type can be written to.

.. _function-ast__dot__rq_isAotAlias_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAotAlias() : bool

Returns if somewhere in the type definition it is an AOT alias type.

.. _function-ast__dot__rq_isShareable_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isShareable() : bool

Returns if the given type is a shareable type.

.. _function-ast__dot__rq_isIndex_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isIndex() : bool

Returns if the given type is an index type.

.. _function-ast__dot__rq_isBool_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isBool() : bool

Returns whether the given type is a boolean type.

.. _function-ast__dot__rq_isInteger_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isInteger() : bool

Returns if the given type is an integer type.

.. _function-ast__dot__rq_isSignedInteger_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isSignedInteger() : bool

Returns if the given type is a signed integer type.

.. _function-ast__dot__rq_isUnsignedInteger_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isUnsignedInteger() : bool

Returns if the given type is an unsigned integer type.

.. _function-ast__dot__rq_isSignedIntegerOrIntVec_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isSignedIntegerOrIntVec() : bool

Returns if the given type is a signed integer or signed integer vector type.

.. _function-ast__dot__rq_isUnsignedIntegerOrIntVec_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isUnsignedIntegerOrIntVec() : bool

Returns if the given type is an unsigned integer or unsigned integer vector type.

.. _function-ast__dot__rq_isFloatOrDouble_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isFloatOrDouble() : bool

Returns if the given type is a float or double type.

.. _function-ast__dot__rq_isNumeric_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isNumeric() : bool

Returns if the given type is a numeric type.

.. _function-ast__dot__rq_isNumericComparable_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isNumericComparable() : bool

Returns if the given type is a numeric comparable type.

.. _function-ast__dot__rq_isPointer_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isPointer() : bool

Returns if the given type is a pointer type.

.. _function-ast__dot__rq_isSmartPointer_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isSmartPointer() : bool

Returns if the given type is a smart pointer type.

.. _function-ast__dot__rq_isVoidPointer_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isVoidPointer() : bool

Returns if the given type is a void pointer type.

.. _function-ast__dot__rq_isIterator_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isIterator() : bool

Returns if the given type is an iterator type.

.. _function-ast__dot__rq_isEnum_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isEnum() : bool

Returns if the given type is an enum type.

.. _function-ast__dot__rq_isEnumT_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isEnumT() : bool

Returns if the given type's base type is an enum type.

.. _function-ast__dot__rq_isHandle_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isHandle() : bool

Returns if the given type is a handle type, i.e. a C++ type exposed to DaScript via TypeAnnotation.

.. _function-ast__dot__rq_isStructure_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isStructure() : bool

Returns if the given type is a structure type.

.. _function-ast__dot__rq_isClass_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isClass() : bool

Returns if the given type is a class type.

.. _function-ast__dot__rq_isFunction_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isFunction() : bool

Returns if the given type is a function type.

.. _function-ast__dot__rq_isTuple_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isTuple() : bool

Returns if the given type is a tuple type.

.. _function-ast__dot__rq_isVariant_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isVariant() : bool

Returns if the given type is a variant type.

.. _function-ast__dot__rq_sizeOf_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.sizeOf() : int

Returns the size of the given type in bytes.

.. _function-ast__dot__rq_countOf_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.countOf() : int

Returns the number of elements in the given type if it is a fixed array; otherwise, returns 1.

.. _function-ast__dot__rq_alignOf_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.alignOf() : int

Returns the alignment requirement of the given type in bytes.




.. _function-ast__dot__rq_baseSizeOf_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.baseSizeOf() : int

Returns the size of the given type in bytes, not accounting for the fixed array dimensions.

.. _function-ast__dot__rq_stride_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.stride() : int

Returns if the given type is a fixed array, return the size of its stride (indexed element) in bytes.

.. _function-ast__dot__rq_tupleSize_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.tupleSize() : int

Returns the size of the given tuple type in bytes.

.. _function-ast__dot__rq_tupleAlign_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.tupleAlign() : int

Returns the alignment of the given tuple type in bytes.

.. _function-ast__dot__rq_variantSize_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.variantSize() : int

Returns the size of the given variant type in bytes.

.. _function-ast__dot__rq_variantAlign_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.variantAlign() : int

Returns the alignment of the given variant type in bytes.

.. _function-ast__dot__rq_canCopy_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canCopy() : bool

Returns whether the given type can be copied.

.. _function-ast__dot__rq_canMove_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canMove() : bool

Returns whether the given type can be moved.

.. _function-ast__dot__rq_canClone_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canClone() : bool

Returns whether the given type can be cloned.

.. _function-ast__dot__rq_canCloneFromConst_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canCloneFromConst() : bool

Returns whether the given type can be cloned from a const instance.

.. _function-ast__dot__rq_canNew_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canNew() : bool

Returns whether the given type can be instantiated via 'new' on the heap.

.. _function-ast__dot__rq_canDeletePtr_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canDeletePtr() : bool

Returns whether the given type can have its pointer deleted.

.. _function-ast__dot__rq_canDelete_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canDelete() : bool

Returns whether the given type can be deleted.

.. _function-ast__dot__rq_needDelete_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.needDelete() : bool

Returns if the given type requires deletion.

.. _function-ast__dot__rq_isPod_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isPod() : bool

Returns if the given type is a pod (plain old data) type.

.. _function-ast__dot__rq_isRawPod_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isRawPod() : bool

Returns if the given type is a raw pod (plain old data without any pointers or strings) type.

.. _function-ast__dot__rq_isNoHeapType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isNoHeapType() : bool

Returns if the given type is a no-heap type, i.e. can be used without heap allocation.

.. _function-ast__dot__rq_isWorkhorseType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isWorkhorseType() : bool

Returns if the given type is a workhorse type, which is a built-in non-ref type.

.. _function-ast__dot__rq_isPolicyType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isPolicyType() : bool

Returns if the given type is a policy type, i.e. there are SimNodes available for it.

.. _function-ast__dot__rq_isVecPolicyType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isVecPolicyType() : bool

Returns if the given type is a vector policy type, which is any policy type which is not a string.


.. _function-ast__dot__rq_isReturnType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isReturnType() : bool

Returns if the given type is a return type (basically anything but block).


.. _function-ast__dot__rq_isCtorType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isCtorType() : bool

Returns if the given basic type is a constructor type, i.e. can be constructed via type name, for example int(3.4)

.. _function-ast__dot__rq_isRange_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isRange() : bool

Returns if the given type is a range type.

.. _function-ast__dot__rq_isString_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isString() : bool

Returns if the given type is a string type.

.. _function-ast__dot__rq_isConst_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isConst() : bool

Returns if the given type is a const type.

.. _function-ast__dot__rq_isFoldable_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isFoldable() : bool

Returns if the given type is a foldable type, like integer or float and unlike pointer or array.

.. _function-ast__dot__rq_isAlias_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAlias() : bool

Returns of somehwere in the type definition it is an alias type.

.. _function-ast__dot__rq_isAutoArrayResolved_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAutoArrayResolved() : bool

Returns if the fixed array dimensions are fully resolved (no 'auto' or expression dimensions).


.. _function-ast__dot__rq_isAuto_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAuto() : bool

Returns if somewhere in the type definition there is an auto type.

.. _function-ast__dot__rq_isAutoOrAlias_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isAutoOrAlias() : bool

Returns if somewhere in the type definition there is an auto or alias type.

.. _function-ast__dot__rq_isVectorType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isVectorType() : bool

Returns if the given type is a vector type, like int2 float3, or range64.


.. _function-ast__dot__rq_isBitfield_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isBitfield() : bool

Returns whether the given type is a bitfield.

.. _function-ast__dot__rq_isLocal_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.isLocal() : bool

Returns if the given type is a local type, i.e. can be allocated on stack.

.. _function-ast__dot__rq_hasClasses_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.hasClasses() : bool

Returns whether somewhere in the type definition there are classes.

.. _function-ast__dot__rq_hasNonTrivialCtor_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.hasNonTrivialCtor() : bool

Returns whether somewhere in the type definition there are non-trivial constructors.

.. _function-ast__dot__rq_hasNonTrivialDtor_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.hasNonTrivialDtor() : bool

Returns if somehwere in the type definition there are non-trivial destructors.



.. _function-ast__dot__rq_hasNonTrivialCopy_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.hasNonTrivialCopy() : bool

Returns if somehwere in the type definition there are non-trivial copy operations.


.. _function-ast__dot__rq_canBePlacedInContainer_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canBePlacedInContainer() : bool

Returns whether the given type can be placed in a container.

.. _function-ast__dot__rq_vectorBaseType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.vectorBaseType() : Type

Returns the base type of a vector type, e.g. for vector4 it returns float.

.. _function-ast__dot__rq_vectorDim_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.vectorDim() : int


Returns the dimension of a vector type, e.g. for vector4 it returns 4.

.. _function-ast__dot__rq_canInitWithZero_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.canInitWithZero() : bool

Returns whether the given type can be initialized with zero (i.e. memset its memory to zero).

.. _function-ast__dot__rq_rangeBaseType_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.rangeBaseType() : Type

Returns the base type of a range type, e.g. for range64 it returns int64.

.. _function-ast__dot__rq_unsafeInit_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.unsafeInit() : bool

Returns if the given type requires initialization, and its unsafe to skip it.

.. _function-ast__dot__rq_get_mnh_TypeDecl_implicit:

.. das:function:: TypeDecl implicit.get_mnh() : uint64

Returns mangled name hash for the given type.

:Properties: * **canAot** : bool

             * **isExprType** : bool

             * **isSimpleType** : bool

             * **isArray** : bool

             * **isGoodIteratorType** : bool

             * **isGoodArrayType** : bool

             * **isGoodTableType** : bool

             * **isGoodBlockType** : bool

             * **isGoodFunctionType** : bool

             * **isGoodLambdaType** : bool

             * **isGoodTupleType** : bool

             * **isGoodVariantType** : bool

             * **isVoid** : bool

             * **isAnyType** : bool

             * **isRef** : bool

             * **isRefType** : bool

             * **canWrite** : bool

             * **isAotAlias** : bool

             * **isShareable** : bool

             * **isIndex** : bool

             * **isBool** : bool

             * **isInteger** : bool

             * **isSignedInteger** : bool

             * **isUnsignedInteger** : bool

             * **isSignedIntegerOrIntVec** : bool

             * **isUnsignedIntegerOrIntVec** : bool

             * **isFloatOrDouble** : bool

             * **isNumeric** : bool

             * **isNumericComparable** : bool

             * **isPointer** : bool

             * **isSmartPointer** : bool

             * **isVoidPointer** : bool

             * **isIterator** : bool

             * **isEnum** : bool

             * **isEnumT** : bool

             * **isHandle** : bool

             * **isStructure** : bool

             * **isClass** : bool

             * **isFunction** : bool

             * **isTuple** : bool

             * **isVariant** : bool

             * **sizeOf** : int

             * **countOf** : int

             * **alignOf** : int

             * **baseSizeOf** : int

             * **stride** : int

             * **tupleSize** : int

             * **tupleAlign** : int

             * **variantSize** : int

             * **variantAlign** : int

             * **canCopy** : bool

             * **canMove** : bool

             * **canClone** : bool

             * **canCloneFromConst** : bool

             * **canNew** : bool

             * **canDeletePtr** : bool

             * **canDelete** : bool

             * **needDelete** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isNoHeapType** : bool

             * **isWorkhorseType** : bool

             * **isPolicyType** : bool

             * **isVecPolicyType** : bool

             * **isReturnType** : bool

             * **isCtorType** : bool

             * **isRange** : bool

             * **isString** : bool

             * **isConst** : bool

             * **isFoldable** : bool

             * **isAlias** : bool

             * **isAutoArrayResolved** : bool

             * **isAuto** : bool

             * **isAutoOrAlias** : bool

             * **isVectorType** : bool

             * **isBitfield** : bool

             * **isLocal** : bool

             * **hasClasses** : bool

             * **hasNonTrivialCtor** : bool

             * **hasNonTrivialDtor** : bool

             * **hasNonTrivialCopy** : bool

             * **canBePlacedInContainer** : bool

             * **vectorBaseType** :  :ref:`Type <enum-rtti-Type>` 

             * **vectorDim** : int

             * **canInitWithZero** : bool

             * **rangeBaseType** :  :ref:`Type <enum-rtti-Type>` 

             * **unsafeInit** : bool

             * **get_mnh** : uint64

 Any type declaration.

:Fields: * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **structType** :  :ref:`Structure <handle-ast-Structure>` ?

         * **enumType** :  :ref:`Enumeration <handle-ast-Enumeration>` ?

         * **annotation** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ?

         * **firstType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **secondType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **argTypes** : vector<smart_ptr<TypeDecl>>

         * **argNames** : vector<das_string>

         * **dim** : vector<int>

         * **dimExpr** : vector<smart_ptr<Expression>>

         * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` 

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?


.. _handle-ast-Structure:

.. das:attribute:: Structure

.. _function-ast__dot__rq_sizeOf_Structure_implicit:

.. das:function:: Structure implicit.sizeOf() : int

Returns the size of the given type in bytes.

:Properties: * **sizeOf** : int

 Structure declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **fields** : vector<FieldDeclaration>

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **parent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **flags** :  :ref:`StructureFlags <alias-StructureFlags>` 


.. _handle-ast-FieldDeclaration:

.. das:attribute:: FieldDeclaration

 Structure field declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **init** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **offset** : int

         * **flags** :  :ref:`FieldDeclarationFlags <alias-FieldDeclarationFlags>` 


.. _handle-ast-EnumEntry:

.. das:attribute:: EnumEntry

 Entry in the enumeration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-Enumeration:

.. das:attribute:: Enumeration

 Enumeration declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **list** : vector<EnumEntry>

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **external** : bool

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **isPrivate** : bool


.. _handle-ast-Function:

.. das:attribute:: Function

.. _function-ast__dot__rq_origin_Function_implicit:

.. das:function:: Function implicit.origin() : Function?

Returns the origin function, i.e. which generic it came from (if any).

.. _function-ast__dot__rq_getMangledNameHash_Function_implicit:

.. das:function:: Function implicit.getMangledNameHash() : uint64

Returns mangled name hash for the given function.

.. _function-ast__dot__rq_isGeneric_Function_implicit:

.. das:function:: Function implicit.isGeneric() : bool

Returns if the given function is a generic function.

:Properties: * **origin** :  :ref:`Function <handle-ast-Function>` ?

             * **getMangledNameHash** : uint64

             * **isGeneric** : bool

 Function declaration.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Variable>>

         * **result** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : int

         * **totalStackSize** : uint

         * **totalGenLabel** : int

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **classParent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` 

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` 

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` 

         * **inferStack** : vector<InferHistory>

         * **fromGeneric** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **hash** : uint64

         * **aotHash** : uint64


.. _handle-ast-BuiltInFunction:

.. das:attribute:: BuiltInFunction

 Bindings for the 'BuiltInFunction', which is used for the builtin (bound) functions in Daslang.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Variable>>

         * **result** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : int

         * **totalStackSize** : uint

         * **totalGenLabel** : int

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **classParent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` 

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` 

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` 

         * **inferStack** : vector<InferHistory>

         * **fromGeneric** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **hash** : uint64

         * **aotHash** : uint64

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExternalFnBase:

.. das:attribute:: ExternalFnBase

 Base class for external function bindings.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Variable>>

         * **result** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : int

         * **totalStackSize** : uint

         * **totalGenLabel** : int

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **classParent** :  :ref:`Structure <handle-ast-Structure>` ?

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` 

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` 

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` 

         * **inferStack** : vector<InferHistory>

         * **fromGeneric** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **hash** : uint64

         * **aotHash** : uint64

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-InferHistory:

.. das:attribute:: InferHistory

 Generic function infer history.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?


.. _handle-ast-Variable:

.. das:attribute:: Variable

.. _function-ast__dot__rq_isAccessUnused_Variable_implicit:

.. das:function:: Variable implicit.isAccessUnused() : bool

Returns whether the given variable is never accessed in the code.


.. _function-ast__dot__rq_getMangledNameHash_Variable_implicit:

.. das:function:: Variable implicit.getMangledNameHash() : uint64

Returns mangled name hash for the given function.

:Properties: * **isAccessUnused** : bool

             * **getMangledNameHash** : uint64

 Variable declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_aka** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **init** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **source** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **index** : int

         * **stackTop** : uint

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?

         * **initStackSize** : uint

         * **flags** :  :ref:`VariableFlags <alias-VariableFlags>` 

         * **access_flags** :  :ref:`VariableAccessFlags <alias-VariableAccessFlags>` 

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` 


.. _handle-ast-AstContext:

.. das:attribute:: AstContext

 Lexical context for the particular expression.

:Fields: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` >

         * **_loop** : vector<smart_ptr<Expression>>

         * **blocks** : vector<smart_ptr<Expression>>

         * **scopes** : vector<smart_ptr<Expression>>

         * **_with** : vector<smart_ptr<Expression>>


.. _handle-ast-ExprBlock:

.. das:attribute:: ExprBlock

 Any block expression, including regular blocks and all types of closures.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **list** : vector<smart_ptr<Expression>>

         * **finalList** : vector<smart_ptr<Expression>>

         * **returnType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **arguments** : vector<smart_ptr<Variable>>

         * **stackTop** : uint

         * **stackVarTop** : uint

         * **stackVarBottom** : uint

         * **stackCleanVars** : vector<pair`uint`uint>

         * **maxLabelIndex** : int

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` 

         * **annotationData** : uint64

         * **annotationDataSid** : uint64

         * **blockFlags** :  :ref:`ExprBlockFlags <alias-ExprBlockFlags>` 

         * **inFunction** :  :ref:`Function <handle-ast-Function>` ?


.. _handle-ast-ExprLet:

.. das:attribute:: ExprLet

 Local variable declaration (`let v = expr;`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **variables** : vector<smart_ptr<Variable>>

         * **atInit** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **letFlags** :  :ref:`ExprLetFlags <alias-ExprLetFlags>` 


.. _handle-ast-ExprStringBuilder:

.. das:attribute:: ExprStringBuilder

 String builder expression ("blah{blah1}blah2").

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **elements** : vector<smart_ptr<Expression>>

         * **stringBuilderFlags** :  :ref:`StringBuilderFlags <alias-StringBuilderFlags>` 


.. _handle-ast-MakeFieldDecl:

.. das:attribute:: MakeFieldDecl

 Part of `ExprMakeStruct`, declares single field (`a = expr` or `a <- expr` etc)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **tag** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **flags** :  :ref:`MakeFieldDeclFlags <alias-MakeFieldDeclFlags>` 


.. _handle-ast-ExprNamedCall:

.. das:attribute:: ExprNamedCall

 Named call (`call([argname1=expr1, argname2=expr2])`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **nonNamedArguments** : vector<smart_ptr<Expression>>

         * **arguments** :  :ref:`MakeStruct <handle-ast-MakeStruct>` 

         * **argumentsFailedToInfer** : bool


.. _handle-ast-ExprLooksLikeCall:

.. das:attribute:: ExprLooksLikeCall

 Anything which looks like call (`call(expr1,expr2)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprCallFunc:

.. das:attribute:: ExprCallFunc

 Actual function call (`func(expr1,...)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint


.. _handle-ast-ExprNew:

.. das:attribute:: ExprNew

 New expression (`new Foo`, `new Bar(expr1..)`, but **NOT** `new [[Foo ...]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **initializer** : bool


.. _handle-ast-ExprCall:

.. das:attribute:: ExprCall

 Anything which looks like call (`call(expr1,expr2)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **doesNotNeedSp** : bool

         * **cmresAlias** : bool

         * **notDiscarded** : bool


.. _handle-ast-ExprPtr2Ref:

.. das:attribute:: ExprPtr2Ref

 Pointer dereference (`*expr` or `deref(expr)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **unsafeDeref** : bool

         * **assumeNoAlias** : bool


.. _handle-ast-ExprNullCoalescing:

.. das:attribute:: ExprNullCoalescing

 Null coalescing (`expr1 ?? expr2`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **unsafeDeref** : bool

         * **assumeNoAlias** : bool

         * **defaultValue** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprAt:

.. das:attribute:: ExprAt

 Index lookup (`expr[expr1]`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` 


.. _handle-ast-ExprSafeAt:

.. das:attribute:: ExprSafeAt

 Safe index lookup (`expr?[expr1]`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **index** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` 


.. _handle-ast-ExprIs:

.. das:attribute:: ExprIs

 Is expression for variants and such (`expr is Foo`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >


.. _handle-ast-ExprOp:

.. das:attribute:: ExprOp

 Compilation time only base class for any operator.


.. _handle-ast-ExprOp2:

.. das:attribute:: ExprOp2

 Two operand operator (`expr1 + expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprOp3:

.. das:attribute:: ExprOp3

 Three operand operator (`cond ? expr1 : expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprCopy:

.. das:attribute:: ExprCopy

 Copy operator (`expr1 = expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **copy_flags** :  :ref:`CopyFlags <alias-CopyFlags>` 


.. _handle-ast-ExprMove:

.. das:attribute:: ExprMove

 Move operator (`expr1 <- expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **move_flags** :  :ref:`MoveFlags <alias-MoveFlags>` 


.. _handle-ast-ExprClone:

.. das:attribute:: ExprClone

 Clone operator (`expr1 := expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **left** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **right** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprWith:

.. das:attribute:: ExprWith

 With section (`with expr {your; block; here}`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **_with** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprAssume:

.. das:attribute:: ExprAssume

 Assume expression (`assume name = expr`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprWhile:

.. das:attribute:: ExprWhile

 While loop (`while expr {your; block; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **cond** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprTryCatch:

.. das:attribute:: ExprTryCatch

 Try-recover expression (`try {your; block; here;} recover {your; recover; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **try_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **catch_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprIfThenElse:

.. das:attribute:: ExprIfThenElse

 If-then-else expression (`if expr1 {your; block; here;} else {your; block; here;}`) including `static_if`'s.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **cond** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_true** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_false** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **if_flags** :  :ref:`IfFlags <alias-IfFlags>` 


.. _handle-ast-ExprFor:

.. das:attribute:: ExprFor

 For loop (`for expr1 in expr2 {your; block; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **iterators** : vector<das_string>

         * **iteratorsAka** : vector<das_string>

         * **iteratorsAt** : vector<LineInfo>

         * **iteratorsTags** : vector<smart_ptr<Expression>>

         * **iteratorsTupleExpansion** : vector<uint8>

         * **iteratorVariables** : vector<smart_ptr<Variable>>

         * **sources** : vector<smart_ptr<Expression>>

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **visibility** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **allowIteratorOptimization** : bool

         * **canShadow** : bool


.. _handle-ast-ExprMakeLocal:

.. das:attribute:: ExprMakeLocal

 Any make expression (`ExprMakeBlock`, `ExprMakeTuple`, `ExprMakeVariant`, `ExprMakeStruct`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 


.. _handle-ast-ExprMakeStruct:

.. das:attribute:: ExprMakeStruct

 Make structure expression (`[[YourStruct v1=expr1elem1, v2=expr2elem1, ...; v1=expr1elem2, ...  ]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **structs** : vector<smart_ptr<MakeStruct>>

         * **_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **constructor** :  :ref:`Function <handle-ast-Function>` ?

         * **makeStructFlags** :  :ref:`ExprMakeStructFlags <alias-ExprMakeStructFlags>` 


.. _handle-ast-ExprMakeVariant:

.. das:attribute:: ExprMakeVariant

 Make variant expression (`[YourVariant variantName=expr1]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **variants** : vector<smart_ptr<MakeFieldDecl>>


.. _handle-ast-ExprMakeArray:

.. das:attribute:: ExprMakeArray

 Make array expression (`[[auto 1;2;3]]` or `[{auto "foo";"bar"}]` for static and dynamic arrays accordingly).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **recordType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **values** : vector<smart_ptr<Expression>>

         * **gen2** : bool


.. _handle-ast-ExprMakeTuple:

.. das:attribute:: ExprMakeTuple

 Make tuple expression (`[[auto f1,f2,f3]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **makeType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **extraOffset** : uint

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` 

         * **recordType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **values** : vector<smart_ptr<Expression>>

         * **gen2** : bool

         * **isKeyValue** : bool


.. _handle-ast-ExprArrayComprehension:

.. das:attribute:: ExprArrayComprehension

 Array comprehension (`[{for x in 0..3; x}]`, `[[for y in range(100); x*2; where x!=13]]` for arrays or generators accordingly).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **exprFor** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **exprWhere** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **generatorSyntax** : bool

         * **tableSyntax** : bool


.. _handle-ast-TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

 Compilation time only structure which holds live information about typeinfo expression for the specific macro.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?


.. _handle-ast-ExprTypeInfo:

.. das:attribute:: ExprTypeInfo

 typeinfo() expression (`typeinfo(dim a)`, `typeinfo(is_ref_type type<int&>)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **trait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **subtrait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **extratrait** :  :ref:`das_string <handle-builtin-das_string>` 

         * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ?


.. _handle-ast-ExprTypeDecl:

.. das:attribute:: ExprTypeDecl

 typedecl() expression (`typedecl(1+2)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **typeexpr** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >


.. _handle-ast-ExprLabel:

.. das:attribute:: ExprLabel

 Label (`label 13:`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **labelName** : int

         * **comment** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprGoto:

.. das:attribute:: ExprGoto

 Goto expression (`goto label 13`, `goto x`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **labelName** : int

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprRef2Value:

.. das:attribute:: ExprRef2Value

 Compilation time only structure which holds reference to value conversion for the value types, i.e. goes from int& to int and such.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprRef2Ptr:

.. das:attribute:: ExprRef2Ptr

 Addr expresion (`addr(expr)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprAddr:

.. das:attribute:: ExprAddr

 Function address (`@@foobarfunc` or `@@foobarfunc<(int;int):bool>`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **target** :  :ref:`das_string <handle-builtin-das_string>` 

         * **funcType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **func** :  :ref:`Function <handle-ast-Function>` ?


.. _handle-ast-ExprAssert:

.. das:attribute:: ExprAssert

 Assert expression (`assert(x<13)` or `assert(x<13, "x is too big")`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **isVerify** : bool


.. _handle-ast-ExprQuote:

.. das:attribute:: ExprQuote

 Compilation time expression which holds its subexpressions but does not infer them (`quote() <| x+5`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprStaticAssert:

.. das:attribute:: ExprStaticAssert

 Static assert expression (`static_assert(x<13)` or `static_assert(x<13, "x is too big")`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprDebug:

.. das:attribute:: ExprDebug

 Debug expression (`debug(x)` or `debug(x,"x=")`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprInvoke:

.. das:attribute:: ExprInvoke

.. _function-ast__dot__rq_isCopyOrMove_ExprInvoke_implicit:

.. das:function:: ExprInvoke implicit.isCopyOrMove() : bool

Returns if the given invoke expression requires copy or move of a ref type.

:Properties: * **isCopyOrMove** : bool

 Invoke expression (`invoke(fn)` or `invoke(lamb, arg1, arg2, ...)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **stackTop** : uint

         * **doesNotNeedSp** : bool

         * **isInvokeMethod** : bool

         * **cmresAlias** : bool


.. _handle-ast-ExprErase:

.. das:attribute:: ExprErase

 Erase expression (`erase(tab,key)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprSetInsert:

.. das:attribute:: ExprSetInsert

 Set insert expression, i.e. tab |> insert(key).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprFind:

.. das:attribute:: ExprFind

 Find expression (`find(tab,key) <| { your; block; here; }`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprKeyExists:

.. das:attribute:: ExprKeyExists

 Key exists expression (`key_exists(tab,key)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprAscend:

.. das:attribute:: ExprAscend

 New expression for ExprMakeLocal (`new [[Foo fld=val,...]]` or `new [[Foo() fld=...]]`, but **NOT** `new Foo()`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **ascType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **stackTop** : uint

         * **ascendFlags** :  :ref:`ExprAscendFlags <alias-ExprAscendFlags>` 


.. _handle-ast-ExprCast:

.. das:attribute:: ExprCast

 Any cast expression (`cast<int> a`, `upcast<Foo> b` or `reinterpret<Bar?> c`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **castType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **castFlags** :  :ref:`ExprCastFlags <alias-ExprCastFlags>` 


.. _handle-ast-ExprDelete:

.. das:attribute:: ExprDelete

 Delete expression (`delete blah`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **sizeexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **native** : bool


.. _handle-ast-ExprVar:

.. das:attribute:: ExprVar

 Variable access (`foo`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >

         * **pBlock** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

         * **argumentIndex** : int

         * **varFlags** :  :ref:`ExprVarFlags <alias-ExprVarFlags>` 


.. _handle-ast-ExprTag:

.. das:attribute:: ExprTag

 Compilation time only tag expression, used for reification. For example $c(....).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprSwizzle:

.. das:attribute:: ExprSwizzle

 Vector swizzle operatrion (`vec.xxy` or `vec.y`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **mask** :  :ref:`das_string <handle-builtin-das_string>` 

         * **fields** : vector<uint8>

         * **fieldFlags** :  :ref:`ExprSwizzleFieldFlags <alias-ExprSwizzleFieldFlags>` 


.. _handle-ast-ExprField:

.. das:attribute:: ExprField

.. _function-ast__dot__rq_field_ExprField_implicit:

.. das:function:: ExprField implicit.field() : FieldDeclaration?

Returns pointer to a field of a structure, or null if the field does not exist or its not a structure.

:Properties: * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ?

 Field lookup (`foo.bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 


.. _handle-ast-ExprSafeField:

.. das:attribute:: ExprSafeField

 Safe field lookup (`foo?.bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

         * **skipQQ** : bool


.. _handle-ast-ExprIsVariant:

.. das:attribute:: ExprIsVariant

 Is expression (`foo is bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 


.. _handle-ast-ExprAsVariant:

.. das:attribute:: ExprAsVariant

 As expression (`foo as bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 


.. _handle-ast-ExprSafeAsVariant:

.. das:attribute:: ExprSafeAsVariant

 Safe as expression (`foo? as bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **value** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **fieldIndex** : int

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` >

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` 

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` 

         * **skipQQ** : bool


.. _handle-ast-ExprOp1:

.. das:attribute:: ExprOp1

 Single operator expression (`+a` or `-a` or `!a` or `~a`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **func** :  :ref:`Function <handle-ast-Function>` ?

         * **stackTop** : uint

         * **op** :  :ref:`das_string <handle-builtin-das_string>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-ExprReturn:

.. das:attribute:: ExprReturn

 Return expression (`return` or `return foo`, or `return <- foo`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **returnFlags** :  :ref:`ExprReturnFlags <alias-ExprReturnFlags>` 

         * **stackTop** : uint

         * **refStackTop** : uint

         * **returnFunc** :  :ref:`Function <handle-ast-Function>` ?

         * **_block** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?


.. _handle-ast-ExprYield:

.. das:attribute:: ExprYield

 Yield expression (`yield foo` or `yeild <- bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **subexpr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **returnFlags** :  :ref:`ExprYieldFlags <alias-ExprYieldFlags>` 


.. _handle-ast-ExprBreak:

.. das:attribute:: ExprBreak

 Break expression (`break`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 


.. _handle-ast-ExprContinue:

.. das:attribute:: ExprContinue

 Continue expression (`continue`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 


.. _handle-ast-ExprConst:

.. das:attribute:: ExprConst

 Compilation time constant expression base class

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 


.. _handle-ast-ExprFakeContext:

.. das:attribute:: ExprFakeContext

 Compilation time only fake context expression. Will simulate as current evaluation `Context`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 


.. _handle-ast-ExprFakeLineInfo:

.. das:attribute:: ExprFakeLineInfo

.. _function-ast__dot__rq_getValue_ExprFakeLineInfo_implicit:

.. das:function:: ExprFakeLineInfo implicit.getValue() : void?

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : void?

 Compilation time only fake lineinfo expression. Will simulate as current file and line `LineInfo`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : void?


.. _handle-ast-ExprConstPtr:

.. das:attribute:: ExprConstPtr

.. _function-ast__dot__rq_getValue_ExprConstPtr_implicit:

.. das:function:: ExprConstPtr implicit.getValue() : void?

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : void?

 Null (`null`). Technically can be any other pointer, but it is used for nullptr.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : void?


.. _handle-ast-ExprConstInt8:

.. das:attribute:: ExprConstInt8

.. _function-ast__dot__rq_getValue_ExprConstInt8_implicit:

.. das:function:: ExprConstInt8 implicit.getValue() : int8

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int8

 Holds int8 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int8


.. _handle-ast-ExprConstInt16:

.. das:attribute:: ExprConstInt16

.. _function-ast__dot__rq_getValue_ExprConstInt16_implicit:

.. das:function:: ExprConstInt16 implicit.getValue() : int16

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int16

 Holds int16 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int16


.. _handle-ast-ExprConstInt64:

.. das:attribute:: ExprConstInt64

.. _function-ast__dot__rq_getValue_ExprConstInt64_implicit:

.. das:function:: ExprConstInt64 implicit.getValue() : int64

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int64

 Holds int64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int64


.. _handle-ast-ExprConstInt:

.. das:attribute:: ExprConstInt

.. _function-ast__dot__rq_getValue_ExprConstInt_implicit:

.. das:function:: ExprConstInt implicit.getValue() : int

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int

 Holds int constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int


.. _handle-ast-ExprConstInt2:

.. das:attribute:: ExprConstInt2

.. _function-ast__dot__rq_getValue_ExprConstInt2_implicit:

.. das:function:: ExprConstInt2 implicit.getValue() : int2

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int2

 Holds int2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int2


.. _handle-ast-ExprConstInt3:

.. das:attribute:: ExprConstInt3

.. _function-ast__dot__rq_getValue_ExprConstInt3_implicit:

.. das:function:: ExprConstInt3 implicit.getValue() : int3

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int3

 Holds int3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int3


.. _handle-ast-ExprConstInt4:

.. das:attribute:: ExprConstInt4

.. _function-ast__dot__rq_getValue_ExprConstInt4_implicit:

.. das:function:: ExprConstInt4 implicit.getValue() : int4

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : int4

 Holds int4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : int4


.. _handle-ast-ExprConstUInt8:

.. das:attribute:: ExprConstUInt8

.. _function-ast__dot__rq_getValue_ExprConstUInt8_implicit:

.. das:function:: ExprConstUInt8 implicit.getValue() : uint8

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint8

 Holds uint8 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint8


.. _handle-ast-ExprConstUInt16:

.. das:attribute:: ExprConstUInt16

.. _function-ast__dot__rq_getValue_ExprConstUInt16_implicit:

.. das:function:: ExprConstUInt16 implicit.getValue() : uint16

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint16

 Holds uint16 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint16


.. _handle-ast-ExprConstUInt64:

.. das:attribute:: ExprConstUInt64

.. _function-ast__dot__rq_getValue_ExprConstUInt64_implicit:

.. das:function:: ExprConstUInt64 implicit.getValue() : uint64

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint64

 Holds uint64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint64


.. _handle-ast-ExprConstUInt:

.. das:attribute:: ExprConstUInt

.. _function-ast__dot__rq_getValue_ExprConstUInt_implicit:

.. das:function:: ExprConstUInt implicit.getValue() : uint

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint

 Holds uint constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint


.. _handle-ast-ExprConstUInt2:

.. das:attribute:: ExprConstUInt2

.. _function-ast__dot__rq_getValue_ExprConstUInt2_implicit:

.. das:function:: ExprConstUInt2 implicit.getValue() : uint2

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint2

 Holds uint2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint2


.. _handle-ast-ExprConstUInt3:

.. das:attribute:: ExprConstUInt3

.. _function-ast__dot__rq_getValue_ExprConstUInt3_implicit:

.. das:function:: ExprConstUInt3 implicit.getValue() : uint3

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint3

 Holds uint3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint3


.. _handle-ast-ExprConstUInt4:

.. das:attribute:: ExprConstUInt4

.. _function-ast__dot__rq_getValue_ExprConstUInt4_implicit:

.. das:function:: ExprConstUInt4 implicit.getValue() : uint4

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint4

 Holds uint4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : uint4


.. _handle-ast-ExprConstRange:

.. das:attribute:: ExprConstRange

.. _function-ast__dot__rq_getValue_ExprConstRange_implicit:

.. das:function:: ExprConstRange implicit.getValue() : range

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : range

 Holds range constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : range


.. _handle-ast-ExprConstURange:

.. das:attribute:: ExprConstURange

.. _function-ast__dot__rq_getValue_ExprConstURange_implicit:

.. das:function:: ExprConstURange implicit.getValue() : urange

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : urange

 Holds urange constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : urange


.. _handle-ast-ExprConstRange64:

.. das:attribute:: ExprConstRange64

.. _function-ast__dot__rq_getValue_ExprConstRange64_implicit:

.. das:function:: ExprConstRange64 implicit.getValue() : range64

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : range64

 Holds range64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : range64


.. _handle-ast-ExprConstURange64:

.. das:attribute:: ExprConstURange64

.. _function-ast__dot__rq_getValue_ExprConstURange64_implicit:

.. das:function:: ExprConstURange64 implicit.getValue() : urange64

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : urange64

 Holds urange64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : urange64


.. _handle-ast-ExprConstFloat:

.. das:attribute:: ExprConstFloat

.. _function-ast__dot__rq_getValue_ExprConstFloat_implicit:

.. das:function:: ExprConstFloat implicit.getValue() : float

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : float

 Holds float constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float


.. _handle-ast-ExprConstFloat2:

.. das:attribute:: ExprConstFloat2

.. _function-ast__dot__rq_getValue_ExprConstFloat2_implicit:

.. das:function:: ExprConstFloat2 implicit.getValue() : float2

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : float2

 Holds float2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float2


.. _handle-ast-ExprConstFloat3:

.. das:attribute:: ExprConstFloat3

.. _function-ast__dot__rq_getValue_ExprConstFloat3_implicit:

.. das:function:: ExprConstFloat3 implicit.getValue() : float3

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : float3

 Holds float3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float3


.. _handle-ast-ExprConstFloat4:

.. das:attribute:: ExprConstFloat4

.. _function-ast__dot__rq_getValue_ExprConstFloat4_implicit:

.. das:function:: ExprConstFloat4 implicit.getValue() : float4

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : float4

 Holds float4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : float4


.. _handle-ast-ExprConstDouble:

.. das:attribute:: ExprConstDouble

.. _function-ast__dot__rq_getValue_ExprConstDouble_implicit:

.. das:function:: ExprConstDouble implicit.getValue() : double

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : double

 Holds double constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : double


.. _handle-ast-ExprConstBool:

.. das:attribute:: ExprConstBool

.. _function-ast__dot__rq_getValue_ExprConstBool_implicit:

.. das:function:: ExprConstBool implicit.getValue() : bool

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : bool

 Holds bool constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : bool


.. _handle-ast-CaptureEntry:

.. das:attribute:: CaptureEntry

 Single entry in lambda capture.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **mode** :  :ref:`CaptureMode <enum-ast-CaptureMode>` 


.. _handle-ast-ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

 Any closure. Holds block as well as capture information in `CaptureEntry`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **_capture** : vector<CaptureEntry>

         * **_block** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

         * **stackTop** : uint

         * **mmFlags** :  :ref:`ExprMakeBlockFlags <alias-ExprMakeBlockFlags>` 

         * **aotFunctorName** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

 Generator closure (`generator<int>` or `generator<Foo&>`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **iterType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **_capture** : vector<CaptureEntry>


.. _handle-ast-ExprMemZero:

.. das:attribute:: ExprMemZero

 Memzero (`memzero(expr)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 


.. _handle-ast-ExprConstEnumeration:

.. das:attribute:: ExprConstEnumeration

 Holds enumeration constant, both type and entry (`Foo bar`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **enumType** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` >

         * **value** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprConstBitfield:

.. das:attribute:: ExprConstBitfield

.. _function-ast__dot__rq_getValue_ExprConstBitfield_implicit:

.. das:function:: ExprConstBitfield implicit.getValue() : uint64

Returns the pointer value to the LineInfo * associated with the expression, as a void pointer.



:Properties: * **getValue** : uint64

 Holds bitfield constant (`Foo bar`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** : bitfield<>

         * **bitfieldType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >


.. _handle-ast-ExprConstString:

.. das:attribute:: ExprConstString

 Holds string constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **baseType** :  :ref:`Type <enum-rtti-Type>` 

         * **value** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprUnsafe:

.. das:attribute:: ExprUnsafe

 Unsafe expression (`unsafe(addr(x))`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **body** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >


.. _handle-ast-VisitorAdapter:

.. das:attribute:: VisitorAdapter

 Adapter for the `AstVisitor` interface.


.. _handle-ast-FunctionAnnotation:

.. das:attribute:: FunctionAnnotation

 Adapter for the `AstFunctionAnnotation`.


.. _handle-ast-StructureAnnotation:

.. das:attribute:: StructureAnnotation

 Adapter for the `AstStructureAnnotation`.


.. _handle-ast-EnumerationAnnotation:

.. das:attribute:: EnumerationAnnotation

 Adapater for the `AstEnumearationAnnotation`.


.. _handle-ast-PassMacro:

.. das:attribute:: PassMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Adapter for the `AstPassMacro`.


.. _handle-ast-ReaderMacro:

.. das:attribute:: ReaderMacro

 Adapter for the `AstReaderMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?


.. _handle-ast-CommentReader:

.. das:attribute:: CommentReader

 Adapter for the `AstCommentReader`.


.. _handle-ast-CallMacro:

.. das:attribute:: CallMacro

 Adapter for the `AstCallMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **_module** :  :ref:`Module <handle-rtti-Module>` ?


.. _handle-ast-VariantMacro:

.. das:attribute:: VariantMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Adapter for the `AstVariantMacro`.


.. _handle-ast-ForLoopMacro:

.. das:attribute:: ForLoopMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Adapter for the 'AstForLoopMacro'.


.. _handle-ast-CaptureMacro:

.. das:attribute:: CaptureMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Adapter for the `AstCaptureMacro`.


.. _handle-ast-TypeMacro:

.. das:attribute:: TypeMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Compilation time only structure which holds live information about type macro.


.. _handle-ast-SimulateMacro:

.. das:attribute:: SimulateMacro

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Adapter for the `AstSimulateMacro`.


.. _handle-ast-ExprReader:

.. das:attribute:: ExprReader

 Compilation time only expression which holds temporary information for the `AstReaderMacro`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **macro** : smart_ptr< :ref:`ReaderMacro <handle-ast-ReaderMacro>` >

         * **sequence** :  :ref:`das_string <handle-builtin-das_string>` 


.. _handle-ast-ExprCallMacro:

.. das:attribute:: ExprCallMacro

 Compilation time only expression which holds temporary infromation for the `AstCallMacro`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **_type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

         * **__rtti** : string

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` 

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` 

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` 

         * **name** :  :ref:`das_string <handle-builtin-das_string>` 

         * **arguments** : vector<smart_ptr<Expression>>

         * **argumentsFailedToInfer** : bool

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

         * **macro** :  :ref:`CallMacro <handle-ast-CallMacro>` ?


+++++++++++
Call macros
+++++++++++

.. _call-macro-ast-quote:

.. das:attribute:: quote

 Returns ast expression tree of the input, without evaluating or infering it.

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-ast-ast_typedecl:

.. das:attribute:: ast_typedecl

 Returns TypeDeclPtr of the type specified via type<> or subexpression type, for example typeinfo(ast_typedecl type<int?>)

.. _call-macro-ast-ast_function:

.. das:attribute:: ast_function

 Returns FunctionPtr to the function specified by subexrepssion, for example typeinfo(ast_function @@foo)

+++++++++++++
Handled types
+++++++++++++

.. _handle-ast-MakeStruct:

.. das:attribute:: MakeStruct

 Part of `ExprMakeStruct`, happens to be vector of `MakeFieldDecl`.

+++++++
Classes
+++++++

.. _struct-ast-AstFunctionAnnotation:

.. das:attribute:: AstFunctionAnnotation

 Annotation macro which is attached to the `Function`.


.. _struct-ast-AstBlockAnnotation:

.. das:attribute:: AstBlockAnnotation

 Annotation macro which is attached to the `ExprBlock`.


.. _struct-ast-AstStructureAnnotation:

.. das:attribute:: AstStructureAnnotation

 Annotation macro which is attached to the `Structure`.


.. _struct-ast-AstPassMacro:

.. das:attribute:: AstPassMacro

 This macro is used to implement custom `infer` passes.


.. _struct-ast-AstVariantMacro:

.. das:attribute:: AstVariantMacro

 This macro is used to implement custom `is`, `as` and `?as` expressions.


.. _struct-ast-AstForLoopMacro:

.. das:attribute:: AstForLoopMacro

 This macro is used to implement custom for-loop handlers. It is similar to visitExprFor callback of the AstVisitor.


.. _struct-ast-AstCaptureMacro:

.. das:attribute:: AstCaptureMacro

 This macro is used to implement custom lambda capturing functionality.


.. _struct-ast-AstTypeMacro:

.. das:attribute:: AstTypeMacro

 Macro which is part of the type declaration, for example $type_macro_name<type_macro_type_args...>(type_macro_args).


.. _struct-ast-AstSimulateMacro:

.. das:attribute:: AstSimulateMacro

 Macro which is attached to the context simulation.


.. _struct-ast-AstReaderMacro:

.. das:attribute:: AstReaderMacro

 This macro is used to implement custom parsing functionality, i.e. anything starting with %NameOfTheMacro~ and ending when the macro says it ends.


.. _struct-ast-AstCommentReader:

.. das:attribute:: AstCommentReader

 This macro is used to implement custom comment parsing function (such as doxygen-style documentation etc).


.. _struct-ast-AstCallMacro:

.. das:attribute:: AstCallMacro

 This macro is used to implement custom call-like expressions ( like `foo(bar,bar2,...)` ).


.. _struct-ast-AstTypeInfoMacro:

.. das:attribute:: AstTypeInfoMacro

 This macro is used to implement type info traits, i.e. `typeinfo(YourTraitHere ...)` expressions.


.. _struct-ast-AstEnumerationAnnotation:

.. das:attribute:: AstEnumerationAnnotation

 Annotation macro which is attached to `Enumeration`.


.. _struct-ast-AstVisitor:

.. das:attribute:: AstVisitor

 This class implements `Visitor` interface for the ast tree.


+++++++++++++++
Call generation
+++++++++++++++

  *  :ref:`make_call (at: LineInfo implicit; name: string implicit) : smart_ptr\<Expression\> <function-ast_make_call_LineInfo_implicit_string_implicit>` 

.. _function-ast_make_call_LineInfo_implicit_string_implicit:

.. das:function:: make_call(at: LineInfo implicit; name: string implicit) : smart_ptr<Expression>

 Creates appropriate call expression for the given call function name in the `Program`.

:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **name** : string implicit

+++++++++++++++
Visitor pattern
+++++++++++++++

  *  :ref:`visit (program: smart_ptr\<Program\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit_modules (program: smart_ptr\<Program\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_modules_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit_module (program: smart_ptr\<Program\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit; module: Module? implicit) <function-ast_visit_module_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit_Module_q__implicit>` 
  *  :ref:`visit (function: smart_ptr\<Function\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit_enumeration (program: smart_ptr\<Program\> implicit; enumeration: smart_ptr\<Enumeration\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_enumeration_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Enumeration_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit_structure (program: smart_ptr\<Program\> implicit; structure: smart_ptr\<Structure\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_structure_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit (expression: smart_ptr\<Expression\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) : smart_ptr\<Expression\> <function-ast_visit_smart_ptr_ls_Expression_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit (expression: smart_ptr\<TypeDecl\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) : smart_ptr\<TypeDecl\> <function-ast_visit_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 
  *  :ref:`visit_finally (expression: smart_ptr\<ExprBlock\> implicit; adapter: smart_ptr\<VisitorAdapter\> implicit) <function-ast_visit_finally_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit>` 

.. _function-ast_visit_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit(program: smart_ptr<Program> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Invokes visitor for the given object.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_modules_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit_modules(program: smart_ptr<Program> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Invokes visitor for the given list of modules inside the `Program`.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_module_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit_Module_q__implicit:

.. das:function:: visit_module(program: smart_ptr<Program> implicit; adapter: smart_ptr<VisitorAdapter> implicit; module: Module? implicit)

 Invokes visitor for the given module.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

            * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-ast_visit_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit(function: smart_ptr<Function> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Invokes visitor for the given object.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_enumeration_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Enumeration_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit_enumeration(program: smart_ptr<Program> implicit; enumeration: smart_ptr<Enumeration> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Invokes visitor for the given enumeration.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **enumeration** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_structure_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit_structure(program: smart_ptr<Program> implicit; structure: smart_ptr<Structure> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Invokes visitor for the given structure.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_smart_ptr_ls_Expression_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit(expression: smart_ptr<Expression> implicit; adapter: smart_ptr<VisitorAdapter> implicit) : smart_ptr<Expression>

 Invokes visitor for the given object.

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit(expression: smart_ptr<TypeDecl> implicit; adapter: smart_ptr<VisitorAdapter> implicit) : smart_ptr<TypeDecl>

 Invokes visitor for the given object.

:Arguments: * **expression** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

.. _function-ast_visit_finally_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_VisitorAdapter_gr__implicit:

.. das:function:: visit_finally(expression: smart_ptr<ExprBlock> implicit; adapter: smart_ptr<VisitorAdapter> implicit)

 Calls visit on the `finally` section of the block.

:Arguments: * **expression** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **adapter** : smart_ptr< :ref:`VisitorAdapter <handle-ast-VisitorAdapter>` > implicit

+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`force_generated (expression: smart_ptr\<Expression\> const& implicit; value: bool) <function-ast_force_generated_smart_ptr_ls_Expression_gr__const_implicit_bool>` 
  *  :ref:`force_generated (function: smart_ptr\<Function\> const& implicit; value: bool) <function-ast_force_generated_smart_ptr_ls_Function_gr__const_implicit_bool>` 
  *  :ref:`get_expression_annotation (expr: Expression? implicit) : Annotation? <function-ast_get_expression_annotation_Expression_q__implicit>` 
  *  :ref:`make_type_info_structure (ctx: Context implicit; type: smart_ptr\<TypeDecl\> implicit) : TypeInfo? <function-ast_make_type_info_structure_Context_implicit_smart_ptr_ls_TypeDecl_gr__implicit>` 

.. _function-ast_force_generated_smart_ptr_ls_Expression_gr__const_implicit_bool:

.. das:function:: force_generated(expression: smart_ptr<Expression> const& implicit; value: bool)

 Forces `generated` flag on subexrepssion.

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **value** : bool

.. _function-ast_force_generated_smart_ptr_ls_Function_gr__const_implicit_bool:

.. das:function:: force_generated(function: smart_ptr<Function> const& implicit; value: bool)

 Forces `generated` flag on subexrepssion.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

            * **value** : bool

.. _function-ast_get_expression_annotation_Expression_q__implicit:

.. das:function:: get_expression_annotation(expr: Expression? implicit) : Annotation?

 Get 'Annotation' for the 'ast::Expression' and its inherited types.

:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>` ? implicit

.. _function-ast_make_type_info_structure_Context_implicit_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: make_type_info_structure(ctx: Context implicit; type: smart_ptr<TypeDecl> implicit) : TypeInfo?

 Returns new `TypeInfo` corresponding to the specific type.

:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

++++++++++++++++++
Adapter generation
++++++++++++++++++

  *  :ref:`make_visitor (class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<VisitorAdapter\> <function-ast_make_visitor_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_function_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<FunctionAnnotation\> <function-ast_make_function_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_block_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<FunctionAnnotation\> <function-ast_make_block_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_structure_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<StructureAnnotation\> <function-ast_make_structure_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_enumeration_annotation (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<EnumerationAnnotation\> <function-ast_make_enumeration_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_pass_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<PassMacro\> <function-ast_make_pass_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_reader_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<ReaderMacro\> <function-ast_make_reader_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_comment_reader (class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CommentReader\> <function-ast_make_comment_reader_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_call_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CallMacro\> <function-ast_make_call_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_typeinfo_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<TypeInfoMacro\> <function-ast_make_typeinfo_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_variant_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<VariantMacro\> <function-ast_make_variant_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_for_loop_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<ForLoopMacro\> <function-ast_make_for_loop_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_capture_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<CaptureMacro\> <function-ast_make_capture_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_type_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<TypeMacro\> <function-ast_make_type_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_simulate_macro (name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr\<SimulateMacro\> <function-ast_make_simulate_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`make_clone_structure (structure: Structure? implicit) : smart_ptr\<Function\> <function-ast_make_clone_structure_Structure_q__implicit>` 
  *  :ref:`make_type_info (helper: smart_ptr\<DebugInfoHelper\> implicit; info: TypeInfo? implicit; type: smart_ptr\<TypeDecl\> const& implicit) : TypeInfo? <function-ast_make_type_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_TypeInfo_q__implicit_smart_ptr_ls_TypeDecl_gr__const_implicit>` 
  *  :ref:`make_variable_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; var: Variable? implicit) : VarInfo? <function-ast_make_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Variable_q__implicit>` 
  *  :ref:`make_struct_variable_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; st: Structure const? implicit; var: FieldDeclaration const? implicit) : VarInfo? <function-ast_make_struct_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Structure_const_q__implicit_FieldDeclaration_const_q__implicit>` 
  *  :ref:`make_struct_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; st: Structure const? implicit) : StructInfo? <function-ast_make_struct_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Structure_const_q__implicit>` 
  *  :ref:`make_function_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; fn: Function const? implicit) : FuncInfo? <function-ast_make_function_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Function_const_q__implicit>` 
  *  :ref:`make_enum_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; en: Enumeration const? implicit) : EnumInfo? <function-ast_make_enum_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Enumeration_const_q__implicit>` 
  *  :ref:`make_invokable_type_debug_info (helper: smart_ptr\<DebugInfoHelper\> implicit; blk: smart_ptr\<TypeDecl\> implicit; at: LineInfo implicit) : FuncInfo? <function-ast_make_invokable_type_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_LineInfo_implicit>` 
  *  :ref:`make_block_type (blk: ExprBlock? implicit) : smart_ptr\<TypeDecl\> <function-ast_make_block_type_ExprBlock_q__implicit>` 
  *  :ref:`make_function_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr <function-ast_make_function_annotation_string_auto>` 
  *  :ref:`make_block_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr <function-ast_make_block_annotation_string_auto>` 
  *  :ref:`make_structure_annotation (name: string; var someClassPtr: auto) : StructureAnnotationPtr <function-ast_make_structure_annotation_string_auto>` 
  *  :ref:`make_enumeration_annotation (name: string; var someClassPtr: auto) : EnumerationAnnotationPtr <function-ast_make_enumeration_annotation_string_auto>` 
  *  :ref:`make_visitor (someClass: auto) : smart_ptr\<VisitorAdapter\> <function-ast_make_visitor_auto>` 
  *  :ref:`make_reader_macro (name: string; var someClassPtr: auto) : ReaderMacroPtr <function-ast_make_reader_macro_string_auto>` 
  *  :ref:`make_comment_reader (name: string; var someClassPtr: auto) : CommentReaderPtr <function-ast_make_comment_reader_string_auto>` 
  *  :ref:`make_call_macro (name: string; var someClassPtr: auto) : CallMacroPtr <function-ast_make_call_macro_string_auto>` 
  *  :ref:`make_typeinfo_macro (name: string; var someClassPtr: auto) : TypeInfoMacroPtr <function-ast_make_typeinfo_macro_string_auto>` 
  *  :ref:`make_pass_macro (name: string; var someClassPtr: auto) : PassMacroPtr <function-ast_make_pass_macro_string_auto>` 
  *  :ref:`make_variant_macro (name: string; var someClassPtr: auto) : VariantMacroPtr <function-ast_make_variant_macro_string_auto>` 
  *  :ref:`make_for_loop_macro (name: string; var someClassPtr: auto) : ForLoopMacroPtr <function-ast_make_for_loop_macro_string_auto>` 
  *  :ref:`make_capture_macro (name: string; var someClassPtr: auto) : CaptureMacroPtr <function-ast_make_capture_macro_string_auto>` 
  *  :ref:`make_type_macro (name: string; var someClassPtr: auto) : TypeMacroPtr <function-ast_make_type_macro_string_auto>` 
  *  :ref:`make_simulate_macro (name: string; var someClassPtr: auto) : SimulateMacroPtr <function-ast_make_simulate_macro_string_auto>` 

.. _function-ast_make_visitor_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_visitor(class: void? implicit; info: StructInfo const? implicit) : smart_ptr<VisitorAdapter>

 Creates adapter for the `AstVisitor` interface.

:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_function_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_function_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<FunctionAnnotation>

 Creates adapter for the `AstFunctionAnnotation`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_block_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_block_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<FunctionAnnotation>

 Creates adapter for the `AstBlockAnnotation`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_structure_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_structure_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<StructureAnnotation>

 Creates adapter for the `AstStructureAnnotation`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_enumeration_annotation_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_enumeration_annotation(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<EnumerationAnnotation>

 Creates adapter for the `AstEnumearationAnnotation`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_pass_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_pass_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<PassMacro>

 Creates adapter for the `AstPassMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_reader_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_reader_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<ReaderMacro>

 Creates adapter for the `AstReaderMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_comment_reader_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_comment_reader(class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CommentReader>

 Creates adapter for the `AstCommentReader`.

:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_call_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_call_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CallMacro>

 Creates adapter for the `AstCallMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_typeinfo_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_typeinfo_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<TypeInfoMacro>

 Creates adapter for the `AstTypeInfo` macro.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_variant_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_variant_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<VariantMacro>

 Creates adapter for the `AstVariantMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_for_loop_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_for_loop_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<ForLoopMacro>

 Creates adapter for the `AstForLoopMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_capture_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_capture_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<CaptureMacro>

 Creates adapter for the `AstCaptureMacro`.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_type_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_type_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<TypeMacro>

 Creates adapter for the 'AstTypeMacro' interface.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_simulate_macro_string_implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_simulate_macro(name: string implicit; class: void? implicit; info: StructInfo const? implicit) : smart_ptr<SimulateMacro>

 Creates adapter for the 'AstSimulateMacro' interface.

:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_make_clone_structure_Structure_q__implicit:

.. das:function:: make_clone_structure(structure: Structure? implicit) : smart_ptr<Function>

 Generates `clone` function for the given structure.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

.. _function-ast_make_type_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_TypeInfo_q__implicit_smart_ptr_ls_TypeDecl_gr__const_implicit:

.. das:function:: make_type_info(helper: smart_ptr<DebugInfoHelper> implicit; info: TypeInfo? implicit; type: smart_ptr<TypeDecl> const& implicit) : TypeInfo?

 Generates TypeInfo? for the specified type, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >& implicit

.. _function-ast_make_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Variable_q__implicit:

.. das:function:: make_variable_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; var: Variable? implicit) : VarInfo?

 Generates VariableInfo? for the specified variable, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **var** :  :ref:`Variable <handle-ast-Variable>` ? implicit

.. _function-ast_make_struct_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Structure_const_q__implicit_FieldDeclaration_const_q__implicit:

.. das:function:: make_struct_variable_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; st: Structure const? implicit; var: FieldDeclaration const? implicit) : VarInfo?

 Generates VariableInfo? for the specified structure field, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **var** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>` ? implicit

.. _function-ast_make_struct_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Structure_const_q__implicit:

.. das:function:: make_struct_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; st: Structure const? implicit) : StructInfo?

 Generates StructInfo? for the specified structure, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>` ? implicit

.. _function-ast_make_function_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Function_const_q__implicit:

.. das:function:: make_function_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; fn: Function const? implicit) : FuncInfo?

 Generates FunctInfo? for the specified function, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **fn** :  :ref:`Function <handle-ast-Function>` ? implicit

.. _function-ast_make_enum_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_Enumeration_const_q__implicit:

.. das:function:: make_enum_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; en: Enumeration const? implicit) : EnumInfo?

 Generates EnumInfo? for the specified enumeration, given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **en** :  :ref:`Enumeration <handle-ast-Enumeration>` ? implicit

.. _function-ast_make_invokable_type_debug_info_smart_ptr_ls_DebugInfoHelper_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_LineInfo_implicit:

.. das:function:: make_invokable_type_debug_info(helper: smart_ptr<DebugInfoHelper> implicit; blk: smart_ptr<TypeDecl> implicit; at: LineInfo implicit) : FuncInfo?

 Generates FuncInfo? for the specified invokable type (lambda, block), given DebugInfoHelper.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

.. _function-ast_make_block_type_ExprBlock_q__implicit:

.. das:function:: make_block_type(blk: ExprBlock? implicit) : smart_ptr<TypeDecl>

 Generates TypeDeclPtr for the specified block or lambda type.

:Arguments: * **blk** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ? implicit

.. _function-ast_make_function_annotation_string_auto:

.. das:function:: make_function_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

 Creates adapter for the `AstFunctionAnnotation`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_block_annotation_string_auto:

.. das:function:: make_block_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

 Creates adapter for the `AstBlockAnnotation`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_structure_annotation_string_auto:

.. das:function:: make_structure_annotation(name: string; someClassPtr: auto) : StructureAnnotationPtr

 Creates adapter for the `AstStructureAnnotation`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_enumeration_annotation_string_auto:

.. das:function:: make_enumeration_annotation(name: string; someClassPtr: auto) : EnumerationAnnotationPtr

 Creates adapter for the `AstEnumearationAnnotation`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_visitor_auto:

.. das:function:: make_visitor(someClass: auto) : smart_ptr<VisitorAdapter>

 Creates adapter for the `AstVisitor` interface.

:Arguments: * **someClass** : auto

.. _function-ast_make_reader_macro_string_auto:

.. das:function:: make_reader_macro(name: string; someClassPtr: auto) : ReaderMacroPtr

 Creates adapter for the `AstReaderMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_comment_reader_string_auto:

.. das:function:: make_comment_reader(name: string; someClassPtr: auto) : CommentReaderPtr

 Creates adapter for the `AstCommentReader`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_call_macro_string_auto:

.. das:function:: make_call_macro(name: string; someClassPtr: auto) : CallMacroPtr

 Creates adapter for the `AstCallMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_typeinfo_macro_string_auto:

.. das:function:: make_typeinfo_macro(name: string; someClassPtr: auto) : TypeInfoMacroPtr

 Creates adapter for the `AstTypeInfo` macro.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_pass_macro_string_auto:

.. das:function:: make_pass_macro(name: string; someClassPtr: auto) : PassMacroPtr

 Creates adapter for the `AstPassMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_variant_macro_string_auto:

.. das:function:: make_variant_macro(name: string; someClassPtr: auto) : VariantMacroPtr

 Creates adapter for the `AstVariantMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_for_loop_macro_string_auto:

.. das:function:: make_for_loop_macro(name: string; someClassPtr: auto) : ForLoopMacroPtr

 Creates adapter for the `AstForLoopMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_capture_macro_string_auto:

.. das:function:: make_capture_macro(name: string; someClassPtr: auto) : CaptureMacroPtr

 Creates adapter for the `AstCaptureMacro`.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_type_macro_string_auto:

.. das:function:: make_type_macro(name: string; someClassPtr: auto) : TypeMacroPtr

 Creates adapter for the 'AstTypeMacro' interface.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_simulate_macro_string_auto:

.. das:function:: make_simulate_macro(name: string; someClassPtr: auto) : SimulateMacroPtr

 Creates adapter for the 'AstSimulateMacro' interface.

:Arguments: * **name** : string

            * **someClassPtr** : auto

+++++++++++++++++++
Adapter application
+++++++++++++++++++

  *  :ref:`add_function_annotation (module: Module? implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-ast_add_function_annotation_Module_q__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit>` 
  *  :ref:`add_function_annotation (function: smart_ptr\<Function\> implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-ast_add_function_annotation_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit>` 
  *  :ref:`add_function_annotation (function: smart_ptr\<Function\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-ast_add_function_annotation_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit>` 
  *  :ref:`add_block_annotation (block: smart_ptr\<ExprBlock\> implicit; annotation: smart_ptr\<FunctionAnnotation\>& implicit) <function-ast_add_block_annotation_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit>` 
  *  :ref:`add_block_annotation (block: smart_ptr\<ExprBlock\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-ast_add_block_annotation_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit>` 
  *  :ref:`add_structure_annotation (module: Module? implicit; annotation: smart_ptr\<StructureAnnotation\>& implicit) <function-ast_add_structure_annotation_Module_q__implicit_smart_ptr_ls_StructureAnnotation_gr__implicit>` 
  *  :ref:`add_structure_annotation (structure: smart_ptr\<Structure\> implicit; annotation: smart_ptr\<StructureAnnotation\>& implicit) <function-ast_add_structure_annotation_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_StructureAnnotation_gr__implicit>` 
  *  :ref:`add_structure_annotation (structure: smart_ptr\<Structure\> implicit; annotation: smart_ptr\<AnnotationDeclaration\>& implicit) <function-ast_add_structure_annotation_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit>` 
  *  :ref:`add_enumeration_annotation (module: Module? implicit; annotation: smart_ptr\<EnumerationAnnotation\>& implicit) <function-ast_add_enumeration_annotation_Module_q__implicit_smart_ptr_ls_EnumerationAnnotation_gr__implicit>` 
  *  :ref:`add_infer_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-ast_add_infer_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit>` 
  *  :ref:`add_dirty_infer_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-ast_add_dirty_infer_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit>` 
  *  :ref:`add_lint_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-ast_add_lint_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit>` 
  *  :ref:`add_global_lint_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-ast_add_global_lint_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit>` 
  *  :ref:`add_optimization_macro (module: Module? implicit; annotation: smart_ptr\<PassMacro\>& implicit) <function-ast_add_optimization_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit>` 
  *  :ref:`add_reader_macro (module: Module? implicit; annotation: smart_ptr\<ReaderMacro\>& implicit) <function-ast_add_reader_macro_Module_q__implicit_smart_ptr_ls_ReaderMacro_gr__implicit>` 
  *  :ref:`add_comment_reader (module: Module? implicit; reader: smart_ptr\<CommentReader\>& implicit) <function-ast_add_comment_reader_Module_q__implicit_smart_ptr_ls_CommentReader_gr__implicit>` 
  *  :ref:`add_call_macro (module: Module? implicit; annotation: smart_ptr\<CallMacro\>& implicit) <function-ast_add_call_macro_Module_q__implicit_smart_ptr_ls_CallMacro_gr__implicit>` 
  *  :ref:`add_typeinfo_macro (module: Module? implicit; annotation: smart_ptr\<TypeInfoMacro\>& implicit) <function-ast_add_typeinfo_macro_Module_q__implicit_smart_ptr_ls_TypeInfoMacro_gr__implicit>` 
  *  :ref:`add_variant_macro (module: Module? implicit; annotation: smart_ptr\<VariantMacro\>& implicit) <function-ast_add_variant_macro_Module_q__implicit_smart_ptr_ls_VariantMacro_gr__implicit>` 
  *  :ref:`add_for_loop_macro (module: Module? implicit; annotation: smart_ptr\<ForLoopMacro\>& implicit) <function-ast_add_for_loop_macro_Module_q__implicit_smart_ptr_ls_ForLoopMacro_gr__implicit>` 
  *  :ref:`add_capture_macro (module: Module? implicit; annotation: smart_ptr\<CaptureMacro\>& implicit) <function-ast_add_capture_macro_Module_q__implicit_smart_ptr_ls_CaptureMacro_gr__implicit>` 
  *  :ref:`add_type_macro (module: Module? implicit; annotation: smart_ptr\<TypeMacro\>& implicit) <function-ast_add_type_macro_Module_q__implicit_smart_ptr_ls_TypeMacro_gr__implicit>` 
  *  :ref:`add_simulate_macro (module: Module? implicit; annotation: smart_ptr\<SimulateMacro\>& implicit) <function-ast_add_simulate_macro_Module_q__implicit_smart_ptr_ls_SimulateMacro_gr__implicit>` 
  *  :ref:`add_module_option (module: Module? implicit; option: string implicit; type: Type) <function-ast_add_module_option_Module_q__implicit_string_implicit_Type>` 
  *  :ref:`add_new_block_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_block_annotation_string_auto>` 
  *  :ref:`add_new_function_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_function_annotation_string_auto>` 
  *  :ref:`add_new_contract_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_contract_annotation_string_auto>` 
  *  :ref:`add_new_structure_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_structure_annotation_string_auto>` 
  *  :ref:`add_new_enumeration_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_enumeration_annotation_string_auto>` 
  *  :ref:`add_new_variant_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_variant_macro_string_auto>` 
  *  :ref:`add_new_for_loop_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_for_loop_macro_string_auto>` 
  *  :ref:`add_new_capture_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_capture_macro_string_auto>` 
  *  :ref:`add_new_type_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_type_macro_string_auto>` 
  *  :ref:`add_new_simulate_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_simulate_macro_string_auto>` 
  *  :ref:`add_new_reader_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_reader_macro_string_auto>` 
  *  :ref:`add_new_comment_reader (name: string; var someClassPtr: auto) : auto <function-ast_add_new_comment_reader_string_auto>` 
  *  :ref:`add_new_call_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_call_macro_string_auto>` 
  *  :ref:`add_new_typeinfo_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_typeinfo_macro_string_auto>` 
  *  :ref:`add_new_infer_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_infer_macro_string_auto>` 
  *  :ref:`add_new_dirty_infer_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_dirty_infer_macro_string_auto>` 
  *  :ref:`add_new_lint_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_lint_macro_string_auto>` 
  *  :ref:`add_new_global_lint_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_global_lint_macro_string_auto>` 
  *  :ref:`add_new_optimization_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_optimization_macro_string_auto>` 

.. _function-ast_add_function_annotation_Module_q__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit:

.. das:function:: add_function_annotation(module: Module? implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

 Adds function annotation to the given object. Calls `apply` if applicable.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-ast_add_function_annotation_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit:

.. das:function:: add_function_annotation(function: smart_ptr<Function> implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

 Adds function annotation to the given object. Calls `apply` if applicable.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-ast_add_function_annotation_smart_ptr_ls_Function_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit:

.. das:function:: add_function_annotation(function: smart_ptr<Function> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

 Adds function annotation to the given object. Calls `apply` if applicable.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-ast_add_block_annotation_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_FunctionAnnotation_gr__implicit:

.. das:function:: add_block_annotation(block: smart_ptr<ExprBlock> implicit; annotation: smart_ptr<FunctionAnnotation>& implicit)

 Adds annotation declaration to the block.

:Arguments: * **block** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` >& implicit

.. _function-ast_add_block_annotation_smart_ptr_ls_ExprBlock_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit:

.. das:function:: add_block_annotation(block: smart_ptr<ExprBlock> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

 Adds annotation declaration to the block.

:Arguments: * **block** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-ast_add_structure_annotation_Module_q__implicit_smart_ptr_ls_StructureAnnotation_gr__implicit:

.. das:function:: add_structure_annotation(module: Module? implicit; annotation: smart_ptr<StructureAnnotation>& implicit)

 Adds structure annotation to the given object. Calls `apply` if applicable.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` >& implicit

.. _function-ast_add_structure_annotation_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_StructureAnnotation_gr__implicit:

.. das:function:: add_structure_annotation(structure: smart_ptr<Structure> implicit; annotation: smart_ptr<StructureAnnotation>& implicit)

 Adds structure annotation to the given object. Calls `apply` if applicable.

:Arguments: * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **annotation** : smart_ptr< :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` >& implicit

.. _function-ast_add_structure_annotation_smart_ptr_ls_Structure_gr__implicit_smart_ptr_ls_AnnotationDeclaration_gr__implicit:

.. das:function:: add_structure_annotation(structure: smart_ptr<Structure> implicit; annotation: smart_ptr<AnnotationDeclaration>& implicit)

 Adds structure annotation to the given object. Calls `apply` if applicable.

:Arguments: * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **annotation** : smart_ptr< :ref:`AnnotationDeclaration <handle-rtti-AnnotationDeclaration>` >& implicit

.. _function-ast_add_enumeration_annotation_Module_q__implicit_smart_ptr_ls_EnumerationAnnotation_gr__implicit:

.. das:function:: add_enumeration_annotation(module: Module? implicit; annotation: smart_ptr<EnumerationAnnotation>& implicit)

 Adds enumeration annotation to the given object. Calls `apply` if applicable.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`EnumerationAnnotation <handle-ast-EnumerationAnnotation>` >& implicit

.. _function-ast_add_infer_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit:

.. das:function:: add_infer_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

 Adds `AstPassMacro` adapter to the `infer`` pass.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-ast_add_dirty_infer_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit:

.. das:function:: add_dirty_infer_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

 Adds `AstPassMacro` adapter to the `dirty infer` pass.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-ast_add_lint_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit:

.. das:function:: add_lint_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

 Adds `AstPassMacro` adapter to the `lint` pass.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-ast_add_global_lint_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit:

.. das:function:: add_global_lint_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

 Adds `AstPassMacro` adapter to the `global lint` pass.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-ast_add_optimization_macro_Module_q__implicit_smart_ptr_ls_PassMacro_gr__implicit:

.. das:function:: add_optimization_macro(module: Module? implicit; annotation: smart_ptr<PassMacro>& implicit)

 Adds `AstPassMacro` adapter to the `optimization` pass.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`PassMacro <handle-ast-PassMacro>` >& implicit

.. _function-ast_add_reader_macro_Module_q__implicit_smart_ptr_ls_ReaderMacro_gr__implicit:

.. das:function:: add_reader_macro(module: Module? implicit; annotation: smart_ptr<ReaderMacro>& implicit)

 Adds `AstReaderMacro` adapter to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`ReaderMacro <handle-ast-ReaderMacro>` >& implicit

.. _function-ast_add_comment_reader_Module_q__implicit_smart_ptr_ls_CommentReader_gr__implicit:

.. das:function:: add_comment_reader(module: Module? implicit; reader: smart_ptr<CommentReader>& implicit)

 Adds `AstCommentReader` adapter to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **reader** : smart_ptr< :ref:`CommentReader <handle-ast-CommentReader>` >& implicit

.. _function-ast_add_call_macro_Module_q__implicit_smart_ptr_ls_CallMacro_gr__implicit:

.. das:function:: add_call_macro(module: Module? implicit; annotation: smart_ptr<CallMacro>& implicit)

 Adds `AstCallMacro` adapter to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`CallMacro <handle-ast-CallMacro>` >& implicit

.. _function-ast_add_typeinfo_macro_Module_q__implicit_smart_ptr_ls_TypeInfoMacro_gr__implicit:

.. das:function:: add_typeinfo_macro(module: Module? implicit; annotation: smart_ptr<TypeInfoMacro>& implicit)

 Adds `AstTypeInfo` adapter to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` >& implicit

.. _function-ast_add_variant_macro_Module_q__implicit_smart_ptr_ls_VariantMacro_gr__implicit:

.. das:function:: add_variant_macro(module: Module? implicit; annotation: smart_ptr<VariantMacro>& implicit)

 Adds `AstVariantMacro` to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`VariantMacro <handle-ast-VariantMacro>` >& implicit

.. _function-ast_add_for_loop_macro_Module_q__implicit_smart_ptr_ls_ForLoopMacro_gr__implicit:

.. das:function:: add_for_loop_macro(module: Module? implicit; annotation: smart_ptr<ForLoopMacro>& implicit)

 Adds `AstForLoopMacro` to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`ForLoopMacro <handle-ast-ForLoopMacro>` >& implicit

.. _function-ast_add_capture_macro_Module_q__implicit_smart_ptr_ls_CaptureMacro_gr__implicit:

.. das:function:: add_capture_macro(module: Module? implicit; annotation: smart_ptr<CaptureMacro>& implicit)

 Adds `AstCaptureMacro` to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`CaptureMacro <handle-ast-CaptureMacro>` >& implicit

.. _function-ast_add_type_macro_Module_q__implicit_smart_ptr_ls_TypeMacro_gr__implicit:

.. das:function:: add_type_macro(module: Module? implicit; annotation: smart_ptr<TypeMacro>& implicit)

 Adds 'AstTypeMacro' to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`TypeMacro <handle-ast-TypeMacro>` >& implicit

.. _function-ast_add_simulate_macro_Module_q__implicit_smart_ptr_ls_SimulateMacro_gr__implicit:

.. das:function:: add_simulate_macro(module: Module? implicit; annotation: smart_ptr<SimulateMacro>& implicit)

 Adds `AstSimulateMacro` to the specific module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **annotation** : smart_ptr< :ref:`SimulateMacro <handle-ast-SimulateMacro>` >& implicit

.. _function-ast_add_module_option_Module_q__implicit_string_implicit_Type:

.. das:function:: add_module_option(module: Module? implicit; option: string implicit; type: Type)

 Add module-specific option, which is accessible via "options" keyword.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **option** : string implicit

            * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_add_new_block_annotation_string_auto:

.. das:function:: add_new_block_annotation(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstBlockAnnotation` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_function_annotation_string_auto:

.. das:function:: add_new_function_annotation(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstFunctionAnnotation` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_contract_annotation_string_auto:

.. das:function:: add_new_contract_annotation(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstContractAnnotation` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_structure_annotation_string_auto:

.. das:function:: add_new_structure_annotation(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstStructureAnnotation` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_enumeration_annotation_string_auto:

.. das:function:: add_new_enumeration_annotation(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstEnumerationAnnotation` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_variant_macro_string_auto:

.. das:function:: add_new_variant_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstVariantMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_for_loop_macro_string_auto:

.. das:function:: add_new_for_loop_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstForLoopMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_capture_macro_string_auto:

.. das:function:: add_new_capture_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstCaptureMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_type_macro_string_auto:

.. das:function:: add_new_type_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the 'AstTypeMacro' and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_simulate_macro_string_auto:

.. das:function:: add_new_simulate_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstSimulateMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_reader_macro_string_auto:

.. das:function:: add_new_reader_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstReaderMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_comment_reader_string_auto:

.. das:function:: add_new_comment_reader(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstCommentReader` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_call_macro_string_auto:

.. das:function:: add_new_call_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstCallMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_typeinfo_macro_string_auto:

.. das:function:: add_new_typeinfo_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstTypeInfoMacro` and adds it to the current module.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_infer_macro_string_auto:

.. das:function:: add_new_infer_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstPassMacro` and adds it to the current module `infer` pass.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_dirty_infer_macro_string_auto:

.. das:function:: add_new_dirty_infer_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstPassMacro` and adds it to the current module `dirty infer` pass.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_lint_macro_string_auto:

.. das:function:: add_new_lint_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstPassMacro` and adds it to the current module `lint` pass.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_global_lint_macro_string_auto:

.. das:function:: add_new_global_lint_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstPassMacro` and adds it to the current module `global lint` pass.

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_optimization_macro_string_auto:

.. das:function:: add_new_optimization_macro(name: string; someClassPtr: auto) : auto

 Makes adapter to the `AstPassMacro` and adds it to the current module `optimization` pass.

:Arguments: * **name** : string

            * **someClassPtr** : auto

+++++++++++++++++++++++++
Adding objects to objects
+++++++++++++++++++++++++

  *  :ref:`add_enumeration_entry (enum: smart_ptr\<Enumeration\> implicit; name: string implicit) : int <function-ast_add_enumeration_entry_smart_ptr_ls_Enumeration_gr__implicit_string_implicit>` 
  *  :ref:`add_function (module: Module? implicit; function: smart_ptr\<Function\>& implicit) : bool <function-ast_add_function_Module_q__implicit_smart_ptr_ls_Function_gr__implicit>` 
  *  :ref:`add_generic (module: Module? implicit; function: smart_ptr\<Function\>& implicit) : bool <function-ast_add_generic_Module_q__implicit_smart_ptr_ls_Function_gr__implicit>` 
  *  :ref:`add_variable (module: Module? implicit; variable: smart_ptr\<Variable\>& implicit) : bool <function-ast_add_variable_Module_q__implicit_smart_ptr_ls_Variable_gr__implicit>` 
  *  :ref:`add_keyword (module: Module? implicit; keyword: string implicit; needOxfordComma: bool) : bool <function-ast_add_keyword_Module_q__implicit_string_implicit_bool>` 
  *  :ref:`add_type_function (module: Module? implicit; keyword: string implicit) : bool <function-ast_add_type_function_Module_q__implicit_string_implicit>` 
  *  :ref:`add_structure (module: Module? implicit; structure: smart_ptr\<Structure\>& implicit) : bool <function-ast_add_structure_Module_q__implicit_smart_ptr_ls_Structure_gr__implicit>` 
  *  :ref:`add_alias (module: Module? implicit; structure: smart_ptr\<TypeDecl\>& implicit) : bool <function-ast_add_alias_Module_q__implicit_smart_ptr_ls_TypeDecl_gr__implicit>` 
  *  :ref:`add_module_require (module: Module? implicit; publicModule: Module? implicit; pub: bool) : bool <function-ast_add_module_require_Module_q__implicit_Module_q__implicit_bool>` 
  *  :ref:`add_structure_alias (structure: Structure? implicit; aliasName: string implicit; alias: smart_ptr\<TypeDecl\> const& implicit) : bool <function-ast_add_structure_alias_Structure_q__implicit_string_implicit_smart_ptr_ls_TypeDecl_gr__const_implicit>` 

.. _function-ast_add_enumeration_entry_smart_ptr_ls_Enumeration_gr__implicit_string_implicit:

.. das:function:: add_enumeration_entry(enum: smart_ptr<Enumeration> implicit; name: string implicit) : int

 Adds entry to enumeration annotation.

:Arguments: * **enum** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` > implicit

            * **name** : string implicit

.. _function-ast_add_function_Module_q__implicit_smart_ptr_ls_Function_gr__implicit:

.. das:function:: add_function(module: Module? implicit; function: smart_ptr<Function>& implicit) : bool

 Adds function to a `Module`. Will return false on duplicates.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

.. _function-ast_add_generic_Module_q__implicit_smart_ptr_ls_Function_gr__implicit:

.. das:function:: add_generic(module: Module? implicit; function: smart_ptr<Function>& implicit) : bool

 Adds generic function to a `Module`. Will return false on duplicates.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

.. _function-ast_add_variable_Module_q__implicit_smart_ptr_ls_Variable_gr__implicit:

.. das:function:: add_variable(module: Module? implicit; variable: smart_ptr<Variable>& implicit) : bool

 Adds variable to a `Module`. Will return false on duplicates.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >& implicit

.. _function-ast_add_keyword_Module_q__implicit_string_implicit_bool:

.. das:function:: add_keyword(module: Module? implicit; keyword: string implicit; needOxfordComma: bool) : bool

 Adds new `keyword`. It can appear in the `keyword <type> expr` or `keyword expr block` syntax. See daslib/match as implementation example.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **keyword** : string implicit

            * **needOxfordComma** : bool

.. _function-ast_add_type_function_Module_q__implicit_string_implicit:

.. das:function:: add_type_function(module: Module? implicit; keyword: string implicit) : bool

 Adds type function keyword, i.e allow function call to have several type arguments first via the following syntax some_call<type_args>(regular_args).

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **keyword** : string implicit

.. _function-ast_add_structure_Module_q__implicit_smart_ptr_ls_Structure_gr__implicit:

.. das:function:: add_structure(module: Module? implicit; structure: smart_ptr<Structure>& implicit) : bool

 Adds structure to a `Module`. Will return false on duplicates.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >& implicit

.. _function-ast_add_alias_Module_q__implicit_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: add_alias(module: Module? implicit; structure: smart_ptr<TypeDecl>& implicit) : bool

 Adds type alias to the specified module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >& implicit

.. _function-ast_add_module_require_Module_q__implicit_Module_q__implicit_bool:

.. das:function:: add_module_require(module: Module? implicit; publicModule: Module? implicit; pub: bool) : bool

 Add module dependencies similar to "require" keyword.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **publicModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **pub** : bool

.. _function-ast_add_structure_alias_Structure_q__implicit_string_implicit_smart_ptr_ls_TypeDecl_gr__const_implicit:

.. das:function:: add_structure_alias(structure: Structure? implicit; aliasName: string implicit; alias: smart_ptr<TypeDecl> const& implicit) : bool

Adds an alias to a structure type in the AST. Its the 'typedef' in the structure body.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **aliasName** : string implicit

            * **alias** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >& implicit

+++++++++++++++++++++++++
Program and module access
+++++++++++++++++++++++++

  *  :ref:`this_program () : smart_ptr\<Program\> <function-ast_this_program>` 
  *  :ref:`this_module () : Module? <function-ast_this_module>` 
  *  :ref:`compiling_program () : smart_ptr\<Program\> <function-ast_compiling_program>` 
  *  :ref:`compiling_module () : Module? <function-ast_compiling_module>` 

.. _function-ast_this_program:

.. das:function:: this_program() : smart_ptr<Program>

 Program attached to the current context (or null if RTTI is disabled).

.. _function-ast_this_module:

.. das:function:: this_module() : Module?

 Main module attached to the current context (will through if RTTI is disabled).

.. _function-ast_compiling_program:

.. das:function:: compiling_program() : smart_ptr<Program>

 Currently compiling program.

.. _function-ast_compiling_module:

.. das:function:: compiling_module() : Module?

 Currently compiling module.

+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`describe_typedecl (type: smart_ptr\<TypeDecl\> implicit; extra: bool; contracts: bool; module: bool) : string <function-ast_describe_typedecl_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool>` 
  *  :ref:`describe_typedecl_cpp (type: smart_ptr\<TypeDecl\> implicit; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool; choose_smart_ptr: bool) : string <function-ast_describe_typedecl_cpp_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool_bool_bool>` 
  *  :ref:`describe_expression (expression: smart_ptr\<Expression\> implicit) : string <function-ast_describe_expression_smart_ptr_ls_Expression_gr__implicit>` 
  *  :ref:`describe_function (function: smart_ptr\<Function\> implicit) : string <function-ast_describe_function_smart_ptr_ls_Function_gr__implicit>` 
  *  :ref:`das_to_string (type: Type) : string <function-ast_das_to_string_Type>` 
  *  :ref:`describe (decl: smart_ptr\<TypeDecl\>; extra: bool = true; contracts: bool = true; modules: bool = true) : auto <function-ast_describe_smart_ptr_ls_TypeDecl_gr__bool_bool_bool>` 
  *  :ref:`describe_cpp (decl: smart_ptr\<TypeDecl\>; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true; chooseSmartPtr: bool = true) : auto <function-ast_describe_cpp_smart_ptr_ls_TypeDecl_gr__bool_bool_bool_bool_bool>` 
  *  :ref:`describe (expr: smart_ptr\<Expression\>) : auto <function-ast_describe_smart_ptr_ls_Expression_gr_>` 
  *  :ref:`describe (expr: smart_ptr\<Function\>) : auto <function-ast_describe_smart_ptr_ls_Function_gr_>` 

.. _function-ast_describe_typedecl_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool:

.. das:function:: describe_typedecl(type: smart_ptr<TypeDecl> implicit; extra: bool; contracts: bool; module: bool) : string

 Returns description of the `TypeDecl` which should match corresponding Daslang type declaration.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **extra** : bool

            * **contracts** : bool

            * **module** : bool

.. _function-ast_describe_typedecl_cpp_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool_bool_bool:

.. das:function:: describe_typedecl_cpp(type: smart_ptr<TypeDecl> implicit; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool; choose_smart_ptr: bool) : string

 Returns description of the `TypeDecl` which should match corresponding C++ type declaration.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **substitueRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

            * **choose_smart_ptr** : bool

.. _function-ast_describe_expression_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: describe_expression(expression: smart_ptr<Expression> implicit) : string

 Returns description of the `Expression` which should match corresponding Daslang code.

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_describe_function_smart_ptr_ls_Function_gr__implicit:

.. das:function:: describe_function(function: smart_ptr<Function> implicit) : string

 Returns description of the `Function` which should match corresponding Daslang function declaration.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-ast_das_to_string_Type:

.. das:function:: das_to_string(type: Type) : string

 Returns description (name) of the corresponding `Type`.

:Arguments: * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-ast_describe_smart_ptr_ls_TypeDecl_gr__bool_bool_bool:

.. das:function:: describe(decl: smart_ptr<TypeDecl>; extra: bool = true; contracts: bool = true; modules: bool = true) : auto

 Describes object and produces corresponding Daslang code as string.

:Arguments: * **decl** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

            * **extra** : bool

            * **contracts** : bool

            * **modules** : bool

.. _function-ast_describe_cpp_smart_ptr_ls_TypeDecl_gr__bool_bool_bool_bool_bool:

.. das:function:: describe_cpp(decl: smart_ptr<TypeDecl>; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true; chooseSmartPtr: bool = true) : auto

 Describes `TypeDecl` and produces corresponding C++ code as a string.

:Arguments: * **decl** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >

            * **substitureRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

            * **chooseSmartPtr** : bool

.. _function-ast_describe_smart_ptr_ls_Expression_gr_:

.. das:function:: describe(expr: smart_ptr<Expression>) : auto

 Describes object and produces corresponding Daslang code as string.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >

.. _function-ast_describe_smart_ptr_ls_Function_gr_:

.. das:function:: describe(expr: smart_ptr<Function>) : auto

 Describes object and produces corresponding Daslang code as string.

:Arguments: * **expr** : smart_ptr< :ref:`Function <handle-ast-Function>` >

+++++++++
Searching
+++++++++

  *  :ref:`find_call_macro (module: Module? implicit; name: string implicit) : CallMacro? <function-ast_find_call_macro_Module_q__implicit_string_implicit>` 
  *  :ref:`find_module_via_rtti (program: smart_ptr\<Program\> implicit; name: string implicit) : Module? <function-ast_find_module_via_rtti_smart_ptr_ls_Program_gr__implicit_string_implicit>` 
  *  :ref:`find_module_function_via_rtti (module: Module? implicit; function: function\<():void\>) : smart_ptr\<Function\> <function-ast_find_module_function_via_rtti_Module_q__implicit_function_ls__c_void_gr_>` 
  *  :ref:`find_variable (module: Module? implicit; variable: string implicit) : smart_ptr\<Variable\> <function-ast_find_variable_Module_q__implicit_string_implicit>` 
  *  :ref:`find_matching_variable (program: Program? implicit; function: Function? implicit; name: string implicit; seePrivate: bool; block: block\<(array\<smart_ptr\<Variable\>\>#):void\>) <function-ast_find_matching_variable_Program_q__implicit_Function_q__implicit_string_implicit_bool_block_ls_array_ls_smart_ptr_ls_Variable_gr__gr__hh__c_void_gr_>` 
  *  :ref:`find_bitfield_name (bit: smart_ptr\<TypeDecl\> implicit; value: bitfield) : string <function-ast_find_bitfield_name_smart_ptr_ls_TypeDecl_gr__implicit_bitfield>` 
  *  :ref:`find_enum_name (enum: Enumeration? implicit; value: int64) : string <function-ast_find_enum_name_Enumeration_q__implicit_int64>` 
  *  :ref:`find_enum_value (enum: smart_ptr\<Enumeration\> implicit; value: string implicit) : int64 <function-ast_find_enum_value_smart_ptr_ls_Enumeration_gr__implicit_string_implicit>` 
  *  :ref:`find_enum_value (enum: Enumeration? implicit; value: string implicit) : int64 <function-ast_find_enum_value_Enumeration_q__implicit_string_implicit>` 
  *  :ref:`find_structure_field (structPtr: Structure? implicit; field: string implicit) : FieldDeclaration? <function-ast_find_structure_field_Structure_q__implicit_string_implicit>` 
  *  :ref:`find_unique_structure (program: smart_ptr\<Program\> implicit; name: string implicit) : Structure? <function-ast_find_unique_structure_smart_ptr_ls_Program_gr__implicit_string_implicit>` 
  *  :ref:`find_struct_field_parent (structure: smart_ptr\<Structure\> implicit; name: string implicit) : Structure const? <function-ast_find_struct_field_parent_smart_ptr_ls_Structure_gr__implicit_string_implicit>` 
  *  :ref:`find_module (prog: smart_ptr\<Program\>; name: string) : Module? <function-ast_find_module_smart_ptr_ls_Program_gr__string>` 
  *  :ref:`find_module (name: string) : Module? <function-ast_find_module_string>` 
  *  :ref:`find_compiling_module (name: string) : Module? <function-ast_find_compiling_module_string>` 

.. _function-ast_find_call_macro_Module_q__implicit_string_implicit:

.. das:function:: find_call_macro(module: Module? implicit; name: string implicit) : CallMacro?

 Find CallMacro by name in the Module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

.. _function-ast_find_module_via_rtti_smart_ptr_ls_Program_gr__implicit_string_implicit:

.. das:function:: find_module_via_rtti(program: smart_ptr<Program> implicit; name: string implicit) : Module?

 Find module by name in the `Program`.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **name** : string implicit

.. _function-ast_find_module_function_via_rtti_Module_q__implicit_function_ls__c_void_gr_:

.. das:function:: find_module_function_via_rtti(module: Module? implicit; function: function<():void>) : smart_ptr<Function>

 Find function by name in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **function** : function<void>

.. _function-ast_find_variable_Module_q__implicit_string_implicit:

.. das:function:: find_variable(module: Module? implicit; variable: string implicit) : smart_ptr<Variable>

 Finds variable in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **variable** : string implicit

.. _function-ast_find_matching_variable_Program_q__implicit_Function_q__implicit_string_implicit_bool_block_ls_array_ls_smart_ptr_ls_Variable_gr__gr__hh__c_void_gr_:

.. das:function:: find_matching_variable(program: Program? implicit; function: Function? implicit; name: string implicit; seePrivate: bool; block: block<(array<smart_ptr<Variable>>#):void>)

 Finds global or shared variable in the given function, according to visibility and privacy rules.

:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **function** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **name** : string implicit

            * **seePrivate** : bool

            * **block** : block<(array<smart_ptr< :ref:`Variable <handle-ast-Variable>` >>#):void> implicit

.. _function-ast_find_bitfield_name_smart_ptr_ls_TypeDecl_gr__implicit_bitfield:

.. das:function:: find_bitfield_name(bit: smart_ptr<TypeDecl> implicit; value: bitfield) : string

 Finds name of the corresponding bitfield value in the specified type.

:Arguments: * **bit** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **value** : bitfield<>

.. _function-ast_find_enum_name_Enumeration_q__implicit_int64:

.. das:function:: find_enum_name(enum: Enumeration? implicit; value: int64) : string

 Finds name of the corresponding enumeration value in the specified type.

:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>` ? implicit

            * **value** : int64

.. _function-ast_find_enum_value_smart_ptr_ls_Enumeration_gr__implicit_string_implicit:

.. das:function:: find_enum_value(enum: smart_ptr<Enumeration> implicit; value: string implicit) : int64

 Finds name of the corresponding enumeration value in the specified type.

:Arguments: * **enum** : smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` > implicit

            * **value** : string implicit

.. _function-ast_find_enum_value_Enumeration_q__implicit_string_implicit:

.. das:function:: find_enum_value(enum: Enumeration? implicit; value: string implicit) : int64

 Finds name of the corresponding enumeration value in the specified type.

:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>` ? implicit

            * **value** : string implicit

.. _function-ast_find_structure_field_Structure_q__implicit_string_implicit:

.. das:function:: find_structure_field(structPtr: Structure? implicit; field: string implicit) : FieldDeclaration?

 Returns `FieldDeclaration` for the specific field of the structure type, or `null` if not found.

:Arguments: * **structPtr** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **field** : string implicit

.. _function-ast_find_unique_structure_smart_ptr_ls_Program_gr__implicit_string_implicit:

.. das:function:: find_unique_structure(program: smart_ptr<Program> implicit; name: string implicit) : Structure?

 Find structure in the program with the specified name. If its unique - return it, otherwise null.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **name** : string implicit

.. _function-ast_find_struct_field_parent_smart_ptr_ls_Structure_gr__implicit_string_implicit:

.. das:function:: find_struct_field_parent(structure: smart_ptr<Structure> implicit; name: string implicit) : Structure const?

 Finds parent structure of the specified field declaration.

:Arguments: * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **name** : string implicit

.. _function-ast_find_module_smart_ptr_ls_Program_gr__string:

.. das:function:: find_module(prog: smart_ptr<Program>; name: string) : Module?

 Finds `Module` in the `Program`.

:Arguments: * **prog** : smart_ptr< :ref:`Program <handle-rtti-Program>` >

            * **name** : string

.. _function-ast_find_module_string:

.. das:function:: find_module(name: string) : Module?

 Finds `Module` in the `Program`.

:Arguments: * **name** : string

.. _function-ast_find_compiling_module_string:

.. das:function:: find_compiling_module(name: string) : Module?

 Finds `Module` in the currently compiling `Program`.

:Arguments: * **name** : string

+++++++++
Iterating
+++++++++

  *  :ref:`for_each_module (program: Program? implicit; block: block\<(Module?):void\>) <function-ast_for_each_module_Program_q__implicit_block_ls_Module_q__c_void_gr_>` 
  *  :ref:`for_each_module_no_order (program: Program? implicit; block: block\<(Module?):void\>) <function-ast_for_each_module_no_order_Program_q__implicit_block_ls_Module_q__c_void_gr_>` 
  *  :ref:`for_each_function (module: Module? implicit; name: string implicit; block: block\<(smart_ptr\<Function\>):void\>) <function-ast_for_each_function_Module_q__implicit_string_implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_>` 
  *  :ref:`for_each_generic (module: Module? implicit; name: string implicit; block: block\<(smart_ptr\<Function\>):void\>) <function-ast_for_each_generic_Module_q__implicit_string_implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_>` 
  *  :ref:`any_table_foreach (table: void? implicit; keyStride: int; valueStride: int; block: block\<(void?;void?):void\>) <function-ast_any_table_foreach_void_q__implicit_int_int_block_ls_void_q_;void_q__c_void_gr_>` 
  *  :ref:`any_array_foreach (array: void? implicit; stride: int; block: block\<(void?):void\>) <function-ast_any_array_foreach_void_q__implicit_int_block_ls_void_q__c_void_gr_>` 
  *  :ref:`for_each_typedef (module: Module? implicit; block: block\<(string#;smart_ptr\<TypeDecl\>):void\>) <function-ast_for_each_typedef_Module_q__implicit_block_ls_string_hh_;smart_ptr_ls_TypeDecl_gr__c_void_gr_>` 
  *  :ref:`for_each_enumeration (module: Module? implicit; block: block\<(smart_ptr\<Enumeration\>):void\>) <function-ast_for_each_enumeration_Module_q__implicit_block_ls_smart_ptr_ls_Enumeration_gr__c_void_gr_>` 
  *  :ref:`for_each_structure (module: Module? implicit; block: block\<(smart_ptr\<Structure\>):void\>) <function-ast_for_each_structure_Module_q__implicit_block_ls_smart_ptr_ls_Structure_gr__c_void_gr_>` 
  *  :ref:`for_each_generic (module: Module? implicit; block: block\<(smart_ptr\<Function\>):void\>) <function-ast_for_each_generic_Module_q__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_>` 
  *  :ref:`for_each_global (module: Module? implicit; block: block\<(smart_ptr\<Variable\>):void\>) <function-ast_for_each_global_Module_q__implicit_block_ls_smart_ptr_ls_Variable_gr__c_void_gr_>` 
  *  :ref:`for_each_annotation_ordered (module: Module? implicit; block: block\<(uint64;uint64):void\>) <function-ast_for_each_annotation_ordered_Module_q__implicit_block_ls_uint64;uint64_c_void_gr_>` 
  *  :ref:`for_each_call_macro (module: Module? implicit; block: block\<(string#):void\>) <function-ast_for_each_call_macro_Module_q__implicit_block_ls_string_hh__c_void_gr_>` 
  *  :ref:`for_each_reader_macro (module: Module? implicit; block: block\<(string#):void\>) <function-ast_for_each_reader_macro_Module_q__implicit_block_ls_string_hh__c_void_gr_>` 
  *  :ref:`for_each_variant_macro (module: Module? implicit; block: block\<(smart_ptr\<VariantMacro\>):void\>) <function-ast_for_each_variant_macro_Module_q__implicit_block_ls_smart_ptr_ls_VariantMacro_gr__c_void_gr_>` 
  *  :ref:`for_each_for_loop_macro (module: Module? implicit; block: block\<(smart_ptr\<ForLoopMacro\>):void\>) <function-ast_for_each_for_loop_macro_Module_q__implicit_block_ls_smart_ptr_ls_ForLoopMacro_gr__c_void_gr_>` 
  *  :ref:`for_each_typeinfo_macro (module: Module? implicit; block: block\<(smart_ptr\<TypeInfoMacro\>):void\>) <function-ast_for_each_typeinfo_macro_Module_q__implicit_block_ls_smart_ptr_ls_TypeInfoMacro_gr__c_void_gr_>` 
  *  :ref:`for_each_field (annotation: BasicStructureAnnotation implicit; block: block\<(string;string;smart_ptr\<TypeDecl\>;uint):void\>) <function-ast_for_each_field_BasicStructureAnnotation_implicit_block_ls_string;string;smart_ptr_ls_TypeDecl_gr_;uint_c_void_gr_>` 
  *  :ref:`for_each_module_function (module: Module? implicit; blk: block\<(smart_ptr\<Function\>):void\>) <function-ast_for_each_module_function_Module_q__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_>` 
  *  :ref:`for_each_structure_alias (structure: Structure? implicit; block: block\<(smart_ptr\<TypeDecl\>):void\>) <function-ast_for_each_structure_alias_Structure_q__implicit_block_ls_smart_ptr_ls_TypeDecl_gr__c_void_gr_>` 

.. _function-ast_for_each_module_Program_q__implicit_block_ls_Module_q__c_void_gr_:

.. das:function:: for_each_module(program: Program? implicit; block: block<(Module?):void>)

 Iterates through each module in the program.

:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

.. _function-ast_for_each_module_no_order_Program_q__implicit_block_ls_Module_q__c_void_gr_:

.. das:function:: for_each_module_no_order(program: Program? implicit; block: block<(Module?):void>)

 Iterates through each module in the program in no particular order (in order they appear in libgroup).

:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

.. _function-ast_for_each_function_Module_q__implicit_string_implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_:

.. das:function:: for_each_function(module: Module? implicit; name: string implicit; block: block<(smart_ptr<Function>):void>)

 Iterates through each function in the given `Module`. If the `name` is empty matches all functions.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-ast_for_each_generic_Module_q__implicit_string_implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_:

.. das:function:: for_each_generic(module: Module? implicit; name: string implicit; block: block<(smart_ptr<Function>):void>)

 Iterates through each generic function in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-ast_any_table_foreach_void_q__implicit_int_int_block_ls_void_q_;void_q__c_void_gr_:

.. das:function:: any_table_foreach(table: void? implicit; keyStride: int; valueStride: int; block: block<(void?;void?):void>)

 Iterates through any table<> type in a typeless fasion (via void?)

:Arguments: * **table** : void? implicit

            * **keyStride** : int

            * **valueStride** : int

            * **block** : block<(void?;void?):void> implicit

.. _function-ast_any_array_foreach_void_q__implicit_int_block_ls_void_q__c_void_gr_:

.. das:function:: any_array_foreach(array: void? implicit; stride: int; block: block<(void?):void>)

 Iterates through any array<> type in a typeless fasion (via void?)

:Arguments: * **array** : void? implicit

            * **stride** : int

            * **block** : block<(void?):void> implicit

.. _function-ast_for_each_typedef_Module_q__implicit_block_ls_string_hh_;smart_ptr_ls_TypeDecl_gr__c_void_gr_:

.. das:function:: for_each_typedef(module: Module? implicit; block: block<(string#;smart_ptr<TypeDecl>):void>)

 Iterates through every typedef in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#;smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >):void> implicit

.. _function-ast_for_each_enumeration_Module_q__implicit_block_ls_smart_ptr_ls_Enumeration_gr__c_void_gr_:

.. das:function:: for_each_enumeration(module: Module? implicit; block: block<(smart_ptr<Enumeration>):void>)

 Iterates through every enumeration in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Enumeration <handle-ast-Enumeration>` >):void> implicit

.. _function-ast_for_each_structure_Module_q__implicit_block_ls_smart_ptr_ls_Structure_gr__c_void_gr_:

.. das:function:: for_each_structure(module: Module? implicit; block: block<(smart_ptr<Structure>):void>)

 Iterates through every structure in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Structure <handle-ast-Structure>` >):void> implicit

.. _function-ast_for_each_generic_Module_q__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_:

.. das:function:: for_each_generic(module: Module? implicit; block: block<(smart_ptr<Function>):void>)

 Iterates through each generic function in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-ast_for_each_global_Module_q__implicit_block_ls_smart_ptr_ls_Variable_gr__c_void_gr_:

.. das:function:: for_each_global(module: Module? implicit; block: block<(smart_ptr<Variable>):void>)

 Iterates through every global variable in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`Variable <handle-ast-Variable>` >):void> implicit

.. _function-ast_for_each_annotation_ordered_Module_q__implicit_block_ls_uint64;uint64_c_void_gr_:

.. das:function:: for_each_annotation_ordered(module: Module? implicit; block: block<(uint64;uint64):void>)

 Iterates through each annotation in the given module in the order they were added.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(uint64;uint64):void> implicit

.. _function-ast_for_each_call_macro_Module_q__implicit_block_ls_string_hh__c_void_gr_:

.. das:function:: for_each_call_macro(module: Module? implicit; block: block<(string#):void>)

 Iterates through every CallMacro adapter in the `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#):void> implicit

.. _function-ast_for_each_reader_macro_Module_q__implicit_block_ls_string_hh__c_void_gr_:

.. das:function:: for_each_reader_macro(module: Module? implicit; block: block<(string#):void>)

 Iterates through each reader macro in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(string#):void> implicit

.. _function-ast_for_each_variant_macro_Module_q__implicit_block_ls_smart_ptr_ls_VariantMacro_gr__c_void_gr_:

.. das:function:: for_each_variant_macro(module: Module? implicit; block: block<(smart_ptr<VariantMacro>):void>)

 Iterates through each variant macro in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`VariantMacro <handle-ast-VariantMacro>` >):void> implicit

.. _function-ast_for_each_for_loop_macro_Module_q__implicit_block_ls_smart_ptr_ls_ForLoopMacro_gr__c_void_gr_:

.. das:function:: for_each_for_loop_macro(module: Module? implicit; block: block<(smart_ptr<ForLoopMacro>):void>)

 Iterates through each for loop macro in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`ForLoopMacro <handle-ast-ForLoopMacro>` >):void> implicit

.. _function-ast_for_each_typeinfo_macro_Module_q__implicit_block_ls_smart_ptr_ls_TypeInfoMacro_gr__c_void_gr_:

.. das:function:: for_each_typeinfo_macro(module: Module? implicit; block: block<(smart_ptr<TypeInfoMacro>):void>)

 Iterates through each typeinfo macro in the given `Module`.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<(smart_ptr< :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` >):void> implicit

.. _function-ast_for_each_field_BasicStructureAnnotation_implicit_block_ls_string;string;smart_ptr_ls_TypeDecl_gr_;uint_c_void_gr_:

.. das:function:: for_each_field(annotation: BasicStructureAnnotation implicit; block: block<(string;string;smart_ptr<TypeDecl>;uint):void>)

 Iterates through every field in the `BuiltinStructure` handled type.

:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<(string;string;smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >;uint):void> implicit

.. _function-ast_for_each_module_function_Module_q__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_:

.. das:function:: for_each_module_function(module: Module? implicit; blk: block<(smart_ptr<Function>):void>)

 Iterates through each function in the given module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **blk** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

.. _function-ast_for_each_structure_alias_Structure_q__implicit_block_ls_smart_ptr_ls_TypeDecl_gr__c_void_gr_:

.. das:function:: for_each_structure_alias(structure: Structure? implicit; block: block<(smart_ptr<TypeDecl>):void>)

Iterates over all structure aliases defined in the given structure type and invokes the provided block for each alias.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **block** : block<(smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` >):void> implicit

+++++++
Cloning
+++++++

  *  :ref:`clone_structure (structure: Structure const? implicit) : smart_ptr\<Structure\> <function-ast_clone_structure_Structure_const_q__implicit>` 
  *  :ref:`clone_expression (expression: smart_ptr\<Expression\> implicit) : smart_ptr\<Expression\> <function-ast_clone_expression_smart_ptr_ls_Expression_gr__implicit>` 
  *  :ref:`clone_function (function: smart_ptr\<Function\> implicit) : smart_ptr\<Function\> <function-ast_clone_function_smart_ptr_ls_Function_gr__implicit>` 
  *  :ref:`clone_variable (variable: smart_ptr\<Variable\> implicit) : smart_ptr\<Variable\> <function-ast_clone_variable_smart_ptr_ls_Variable_gr__implicit>` 
  *  :ref:`clone_type (type: smart_ptr\<TypeDecl\> implicit) : smart_ptr\<TypeDecl\> <function-ast_clone_type_smart_ptr_ls_TypeDecl_gr__implicit>` 
  *  :ref:`clone_file_info (name: string implicit; tab_size: int) : FileInfo? <function-ast_clone_file_info_string_implicit_int>` 
  *  :ref:`clone_function (fn: Function?) : FunctionPtr <function-ast_clone_function_Function_q_>` 

.. _function-ast_clone_structure_Structure_const_q__implicit:

.. das:function:: clone_structure(structure: Structure const? implicit) : smart_ptr<Structure>

 Returns clone of the `Structure`.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

.. _function-ast_clone_expression_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: clone_expression(expression: smart_ptr<Expression> implicit) : smart_ptr<Expression>

 Clones `Expression` with subexpressions, including corresponding type.

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_clone_function_smart_ptr_ls_Function_gr__implicit:

.. das:function:: clone_function(function: smart_ptr<Function> implicit) : smart_ptr<Function>

 Clones `Function` and everything in it.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-ast_clone_variable_smart_ptr_ls_Variable_gr__implicit:

.. das:function:: clone_variable(variable: smart_ptr<Variable> implicit) : smart_ptr<Variable>

 Clones `Variable` and everything in it.

:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` > implicit

.. _function-ast_clone_type_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: clone_type(type: smart_ptr<TypeDecl> implicit) : smart_ptr<TypeDecl>

 Clones `TypeDecl` with subtypes.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-ast_clone_file_info_string_implicit_int:

.. das:function:: clone_file_info(name: string implicit; tab_size: int) : FileInfo?

 Clones FileInfo structure.

:Arguments: * **name** : string implicit

            * **tab_size** : int

.. _function-ast_clone_function_Function_q_:

.. das:function:: clone_function(fn: Function?) : FunctionPtr

 Clones `Function` and everything in it.

:Arguments: * **fn** :  :ref:`Function <handle-ast-Function>` ?

++++++++++++
Mangled name
++++++++++++

  *  :ref:`parse_mangled_name (txt: string implicit; lib: ModuleGroup implicit; thisModule: Module? implicit) : smart_ptr\<TypeDecl\> <function-ast_parse_mangled_name_string_implicit_ModuleGroup_implicit_Module_q__implicit>` 
  *  :ref:`get_mangled_name (function: smart_ptr\<Function\> implicit) : string <function-ast_get_mangled_name_smart_ptr_ls_Function_gr__implicit>` 
  *  :ref:`get_mangled_name (type: smart_ptr\<TypeDecl\> implicit) : string <function-ast_get_mangled_name_smart_ptr_ls_TypeDecl_gr__implicit>` 
  *  :ref:`get_mangled_name (variable: smart_ptr\<Variable\> implicit) : string <function-ast_get_mangled_name_smart_ptr_ls_Variable_gr__implicit>` 
  *  :ref:`get_mangled_name (variable: smart_ptr\<ExprBlock\> implicit) : string <function-ast_get_mangled_name_smart_ptr_ls_ExprBlock_gr__implicit>` 
  *  :ref:`get_mangled_name (fn: Function?) : auto <function-ast_get_mangled_name_Function_q_>` 
  *  :ref:`get_mangled_name (decl: TypeDecl?) : auto <function-ast_get_mangled_name_TypeDecl_q_>` 
  *  :ref:`get_mangled_name (decl: Variable?) : auto <function-ast_get_mangled_name_Variable_q_>` 
  *  :ref:`get_mangled_name (decl: ExprBlock?) : auto <function-ast_get_mangled_name_ExprBlock_q_>` 

.. _function-ast_parse_mangled_name_string_implicit_ModuleGroup_implicit_Module_q__implicit:

.. das:function:: parse_mangled_name(txt: string implicit; lib: ModuleGroup implicit; thisModule: Module? implicit) : smart_ptr<TypeDecl>

 Parses mangled name and creates corresponding `TypeDecl`.

:Arguments: * **txt** : string implicit

            * **lib** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>`  implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-ast_get_mangled_name_smart_ptr_ls_Function_gr__implicit:

.. das:function:: get_mangled_name(function: smart_ptr<Function> implicit) : string

 Returns mangled name of the object.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

.. _function-ast_get_mangled_name_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: get_mangled_name(type: smart_ptr<TypeDecl> implicit) : string

 Returns mangled name of the object.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-ast_get_mangled_name_smart_ptr_ls_Variable_gr__implicit:

.. das:function:: get_mangled_name(variable: smart_ptr<Variable> implicit) : string

 Returns mangled name of the object.

:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` > implicit

.. _function-ast_get_mangled_name_smart_ptr_ls_ExprBlock_gr__implicit:

.. das:function:: get_mangled_name(variable: smart_ptr<ExprBlock> implicit) : string

 Returns mangled name of the object.

:Arguments: * **variable** : smart_ptr< :ref:`ExprBlock <handle-ast-ExprBlock>` > implicit

.. _function-ast_get_mangled_name_Function_q_:

.. das:function:: get_mangled_name(fn: Function?) : auto

 Returns mangled name of the object.

:Arguments: * **fn** :  :ref:`Function <handle-ast-Function>` ?

.. _function-ast_get_mangled_name_TypeDecl_q_:

.. das:function:: get_mangled_name(decl: TypeDecl?) : auto

 Returns mangled name of the object.

:Arguments: * **decl** :  :ref:`TypeDecl <handle-ast-TypeDecl>` ?

.. _function-ast_get_mangled_name_Variable_q_:

.. das:function:: get_mangled_name(decl: Variable?) : auto

 Returns mangled name of the object.

:Arguments: * **decl** :  :ref:`Variable <handle-ast-Variable>` ?

.. _function-ast_get_mangled_name_ExprBlock_q_:

.. das:function:: get_mangled_name(decl: ExprBlock?) : auto

 Returns mangled name of the object.

:Arguments: * **decl** :  :ref:`ExprBlock <handle-ast-ExprBlock>` ?

+++++++++++++++
Size and offset
+++++++++++++++

  *  :ref:`get_variant_field_offset (variant: smart_ptr\<TypeDecl\> implicit; index: int) : int <function-ast_get_variant_field_offset_smart_ptr_ls_TypeDecl_gr__implicit_int>` 
  *  :ref:`get_tuple_field_offset (typle: smart_ptr\<TypeDecl\> implicit; index: int) : int <function-ast_get_tuple_field_offset_smart_ptr_ls_TypeDecl_gr__implicit_int>` 
  *  :ref:`any_array_size (array: void? implicit) : int <function-ast_any_array_size_void_q__implicit>` 
  *  :ref:`any_table_size (table: void? implicit) : int <function-ast_any_table_size_void_q__implicit>` 
  *  :ref:`get_handled_type_field_offset (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit) : uint <function-ast_get_handled_type_field_offset_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit>` 

.. _function-ast_get_variant_field_offset_smart_ptr_ls_TypeDecl_gr__implicit_int:

.. das:function:: get_variant_field_offset(variant: smart_ptr<TypeDecl> implicit; index: int) : int

 Returns offset of the variant field in bytes.

:Arguments: * **variant** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **index** : int

.. _function-ast_get_tuple_field_offset_smart_ptr_ls_TypeDecl_gr__implicit_int:

.. das:function:: get_tuple_field_offset(typle: smart_ptr<TypeDecl> implicit; index: int) : int

 Returns offset of the tuple field in bytes.

:Arguments: * **typle** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **index** : int

.. _function-ast_any_array_size_void_q__implicit:

.. das:function:: any_array_size(array: void? implicit) : int

 Returns array size from pointer to array<> object.

:Arguments: * **array** : void? implicit

.. _function-ast_any_table_size_void_q__implicit:

.. das:function:: any_table_size(table: void? implicit) : int

 Returns table size from pointer to the table<> object.

:Arguments: * **table** : void? implicit

.. _function-ast_get_handled_type_field_offset_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit:

.. das:function:: get_handled_type_field_offset(type: smart_ptr<TypeAnnotation> implicit; field: string implicit) : uint

 Returns offset of the field in the ManagedStructure handled type.

:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

+++++++++++
Evaluations
+++++++++++

  *  :ref:`eval_single_expression (expr: smart_ptr\<Expression\> const& implicit; ok: bool& implicit) : float4 <function-ast_eval_single_expression_smart_ptr_ls_Expression_gr__const_implicit_bool_implicit>` 

.. _function-ast_eval_single_expression_smart_ptr_ls_Expression_gr__const_implicit_bool_implicit:

.. das:function:: eval_single_expression(expr: smart_ptr<Expression> const& implicit; ok: bool& implicit) : float4

.. warning:: 
  This is unsafe operation.

 Simulates and evaluates single expression on the separate context.

:Arguments: * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **ok** : bool& implicit

+++++++++++++++
Error reporting
+++++++++++++++

  *  :ref:`macro_error (porogram: smart_ptr\<Program\> implicit; at: LineInfo implicit; message: string implicit) <function-ast_macro_error_smart_ptr_ls_Program_gr__implicit_LineInfo_implicit_string_implicit>` 

.. _function-ast_macro_error_smart_ptr_ls_Program_gr__implicit_LineInfo_implicit_string_implicit:

.. das:function:: macro_error(porogram: smart_ptr<Program> implicit; at: LineInfo implicit; message: string implicit)

 Reports error to the currently compiling program to whatever current pass is.

:Arguments: * **porogram** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **message** : string implicit

++++++++++++++++++++
Location and context
++++++++++++++++++++

  *  :ref:`force_at (expression: smart_ptr\<Expression\> const& implicit; at: LineInfo implicit) <function-ast_force_at_smart_ptr_ls_Expression_gr__const_implicit_LineInfo_implicit>` 
  *  :ref:`force_at (function: smart_ptr\<Function\> const& implicit; at: LineInfo implicit) <function-ast_force_at_smart_ptr_ls_Function_gr__const_implicit_LineInfo_implicit>` 
  *  :ref:`collect_dependencies (function: smart_ptr\<Function\> implicit; block: block\<(array\<Function?\>;array\<Variable?\>):void\>) <function-ast_collect_dependencies_smart_ptr_ls_Function_gr__implicit_block_ls_array_ls_Function_q__gr_;array_ls_Variable_q__gr__c_void_gr_>` 
  *  :ref:`get_ast_context (program: smart_ptr\<Program\> implicit; expression: smart_ptr\<Expression\> implicit; block: block\<(bool;AstContext):void\>) <function-ast_get_ast_context_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Expression_gr__implicit_block_ls_bool;AstContext_c_void_gr_>` 

.. _function-ast_force_at_smart_ptr_ls_Expression_gr__const_implicit_LineInfo_implicit:

.. das:function:: force_at(expression: smart_ptr<Expression> const& implicit; at: LineInfo implicit)

 Replaces line info in the expression, its subexpressions, and its types.

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

.. _function-ast_force_at_smart_ptr_ls_Function_gr__const_implicit_LineInfo_implicit:

.. das:function:: force_at(function: smart_ptr<Function> const& implicit; at: LineInfo implicit)

 Replaces line info in the expression, its subexpressions, and its types.

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` >& implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

.. _function-ast_collect_dependencies_smart_ptr_ls_Function_gr__implicit_block_ls_array_ls_Function_q__gr_;array_ls_Variable_q__gr__c_void_gr_:

.. das:function:: collect_dependencies(function: smart_ptr<Function> implicit; block: block<(array<Function?>;array<Variable?>):void>)

 Collects dependencies of the given function (other functions it calls, global variables it accesses).

:Arguments: * **function** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(array< :ref:`Function <handle-ast-Function>` ?>;array< :ref:`Variable <handle-ast-Variable>` ?>):void> implicit

.. _function-ast_get_ast_context_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_Expression_gr__implicit_block_ls_bool;AstContext_c_void_gr_:

.. das:function:: get_ast_context(program: smart_ptr<Program> implicit; expression: smart_ptr<Expression> implicit; block: block<(bool;AstContext):void>)

 Returns `AstContext` for the given expression. It includes current function (if applicable), loops, blocks, scopes, and with sections.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

            * **block** : block<(bool; :ref:`AstContext <handle-ast-AstContext>` ):void> implicit

+++++++++++
Use queries
+++++++++++

  *  :ref:`get_use_global_variables (func: smart_ptr\<Function\> implicit; block: block\<(smart_ptr\<Variable\>):void\>) <function-ast_get_use_global_variables_smart_ptr_ls_Function_gr__implicit_block_ls_smart_ptr_ls_Variable_gr__c_void_gr_>` 
  *  :ref:`get_use_functions (func: smart_ptr\<Function\> implicit; block: block\<(smart_ptr\<Function\>):void\>) <function-ast_get_use_functions_smart_ptr_ls_Function_gr__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_>` 

.. _function-ast_get_use_global_variables_smart_ptr_ls_Function_gr__implicit_block_ls_smart_ptr_ls_Variable_gr__c_void_gr_:

.. das:function:: get_use_global_variables(func: smart_ptr<Function> implicit; block: block<(smart_ptr<Variable>):void>)

 Provides invoked block with the list of all global variables, used by a function.

:Arguments: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(smart_ptr< :ref:`Variable <handle-ast-Variable>` >):void> implicit

.. _function-ast_get_use_functions_smart_ptr_ls_Function_gr__implicit_block_ls_smart_ptr_ls_Function_gr__c_void_gr_:

.. das:function:: get_use_functions(func: smart_ptr<Function> implicit; block: block<(smart_ptr<Function>):void>)

 Provides invoked block with the list of all functions, used by a function.

:Arguments: * **func** : smart_ptr< :ref:`Function <handle-ast-Function>` > implicit

            * **block** : block<(smart_ptr< :ref:`Function <handle-ast-Function>` >):void> implicit

+++
Log
+++

  *  :ref:`to_compilation_log (text: string implicit) <function-ast_to_compilation_log_string_implicit>` 

.. _function-ast_to_compilation_log_string_implicit:

.. das:function:: to_compilation_log(text: string implicit)

 Writes to compilation log from macro during compilation.

:Arguments: * **text** : string implicit

+++++++
Removal
+++++++

  *  :ref:`remove_structure (module: Module? implicit; structure: smart_ptr\<Structure\>& implicit) : bool <function-ast_remove_structure_Module_q__implicit_smart_ptr_ls_Structure_gr__implicit>` 

.. _function-ast_remove_structure_Module_q__implicit_smart_ptr_ls_Structure_gr__implicit:

.. das:function:: remove_structure(module: Module? implicit; structure: smart_ptr<Structure>& implicit) : bool

 Removes structure declaration from the specified module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **structure** : smart_ptr< :ref:`Structure <handle-ast-Structure>` >& implicit

++++++++++
Properties
++++++++++

  *  :ref:`get_current_search_module (program: Program? implicit; function: Function? implicit; moduleName: string implicit) : Module? <function-ast_get_current_search_module_Program_q__implicit_Function_q__implicit_string_implicit>` 
  *  :ref:`can_access_global_variable (variable: smart_ptr\<Variable\> const& implicit; module: Module? implicit; thisModule: Module? implicit) : bool <function-ast_can_access_global_variable_smart_ptr_ls_Variable_gr__const_implicit_Module_q__implicit_Module_q__implicit>` 
  *  :ref:`is_temp_type (type: smart_ptr\<TypeDecl\> implicit; refMatters: bool) : bool <function-ast_is_temp_type_smart_ptr_ls_TypeDecl_gr__implicit_bool>` 
  *  :ref:`is_same_type (leftType: smart_ptr\<TypeDecl\> implicit; rightType: smart_ptr\<TypeDecl\> implicit; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool <function-ast_is_same_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_RefMatters_ConstMatters_TemporaryMatters>` 
  *  :ref:`get_underlying_value_type (type: smart_ptr\<TypeDecl\> implicit) : smart_ptr\<TypeDecl\> <function-ast_get_underlying_value_type_smart_ptr_ls_TypeDecl_gr__implicit>` 
  *  :ref:`get_handled_type_field_type (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit) : TypeInfo? <function-ast_get_handled_type_field_type_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit>` 
  *  :ref:`get_handled_type_field_type_declaration (type: smart_ptr\<TypeAnnotation\> implicit; field: string implicit; isConst: bool) : smart_ptr\<TypeDecl\> <function-ast_get_handled_type_field_type_declaration_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit_bool>` 
  *  :ref:`get_handled_type_index_type_declaration (type: TypeAnnotation? implicit; src: Expression? implicit; idx: Expression? implicit) : smart_ptr\<TypeDecl\> <function-ast_get_handled_type_index_type_declaration_TypeAnnotation_q__implicit_Expression_q__implicit_Expression_q__implicit>` 
  *  :ref:`get_vector_ptr_at_index (vec: void? implicit; type: TypeDecl? implicit; idx: int) : void? <function-ast_get_vector_ptr_at_index_void_q__implicit_TypeDecl_q__implicit_int>` 
  *  :ref:`get_vector_length (vec: void? implicit; type: smart_ptr\<TypeDecl\> implicit) : int <function-ast_get_vector_length_void_q__implicit_smart_ptr_ls_TypeDecl_gr__implicit>` 
  *  :ref:`has_field (type: smart_ptr\<TypeDecl\> implicit; fieldName: string implicit; constant: bool) : bool <function-ast_has_field_smart_ptr_ls_TypeDecl_gr__implicit_string_implicit_bool>` 
  *  :ref:`get_field_type (type: smart_ptr\<TypeDecl\> implicit; fieldName: string implicit; constant: bool) : smart_ptr\<TypeDecl\> <function-ast_get_field_type_smart_ptr_ls_TypeDecl_gr__implicit_string_implicit_bool>` 
  *  :ref:`is_visible_directly (from_module: Module? implicit; which_module: Module? implicit) : bool <function-ast_is_visible_directly_Module_q__implicit_Module_q__implicit>` 
  *  :ref:`is_expr_like_call (expression: smart_ptr\<Expression\> const& implicit) : bool <function-ast_is_expr_like_call_smart_ptr_ls_Expression_gr__const_implicit>` 
  *  :ref:`is_expr_const (expression: smart_ptr\<Expression\> const& implicit) : bool <function-ast_is_expr_const_smart_ptr_ls_Expression_gr__const_implicit>` 
  *  :ref:`get_function_aot_hash (fun: Function const? implicit) : uint64 <function-ast_get_function_aot_hash_Function_const_q__implicit>` 
  *  :ref:`get_function_hash_by_id (fun: Function? implicit; id: int; pctx: void? implicit) : uint64 <function-ast_get_function_hash_by_id_Function_q__implicit_int_void_q__implicit>` 
  *  :ref:`get_aot_arg_suffix (func: Function? implicit; call: ExprCallFunc? implicit; argIndex: int) : string <function-ast_get_aot_arg_suffix_Function_q__implicit_ExprCallFunc_q__implicit_int>` 
  *  :ref:`get_aot_arg_prefix (func: Function? implicit; call: ExprCallFunc? implicit; argIndex: int) : string <function-ast_get_aot_arg_prefix_Function_q__implicit_ExprCallFunc_q__implicit_int>` 
  *  :ref:`get_func_aot_prefix (ann: FunctionAnnotation? implicit; stg: StringBuilderWriter? implicit; call: ExprCallFunc? implicit) <function-ast_get_func_aot_prefix_FunctionAnnotation_q__implicit_StringBuilderWriter_q__implicit_ExprCallFunc_q__implicit>` 
  *  :ref:`get_struct_aot_prefix (ann: StructureAnnotation? implicit; structure: Structure? implicit; args: AnnotationArgumentList implicit; stg: StringBuilderWriter? implicit) <function-ast_get_struct_aot_prefix_StructureAnnotation_q__implicit_Structure_q__implicit_AnnotationArgumentList_implicit_StringBuilderWriter_q__implicit>` 
  *  :ref:`get_aot_name (func: Function? implicit; call: ExprCallFunc? implicit) : string <function-ast_get_aot_name_Function_q__implicit_ExprCallFunc_q__implicit>` 
  *  :ref:`is_same_type (argType: smart_ptr\<TypeDecl\> implicit; passType: smart_ptr\<TypeDecl\> implicit; refMatters: bool; constMatters: bool; temporaryMatters: bool; allowSubstitute: bool) : bool <function-ast_is_same_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool_bool>` 
  *  :ref:`get_structure_alias (structure: Structure? implicit; aliasName: string implicit) : smart_ptr\<TypeDecl\> <function-ast_get_structure_alias_Structure_q__implicit_string_implicit>` 

.. _function-ast_get_current_search_module_Program_q__implicit_Function_q__implicit_string_implicit:

.. das:function:: get_current_search_module(program: Program? implicit; function: Function? implicit; moduleName: string implicit) : Module?

 Returns the module which is currently being searched for the function, given module name. Resolves "", "_", "*", and "__" correctly.

:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>` ? implicit

            * **function** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **moduleName** : string implicit

.. _function-ast_can_access_global_variable_smart_ptr_ls_Variable_gr__const_implicit_Module_q__implicit_Module_q__implicit:

.. das:function:: can_access_global_variable(variable: smart_ptr<Variable> const& implicit; module: Module? implicit; thisModule: Module? implicit) : bool

 Returns true if global variable is accessible from the specified module.

:Arguments: * **variable** : smart_ptr< :ref:`Variable <handle-ast-Variable>` >& implicit

            * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-ast_is_temp_type_smart_ptr_ls_TypeDecl_gr__implicit_bool:

.. das:function:: is_temp_type(type: smart_ptr<TypeDecl> implicit; refMatters: bool) : bool

 Returns true if type can be temporary.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **refMatters** : bool

.. _function-ast_is_same_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_RefMatters_ConstMatters_TemporaryMatters:

.. das:function:: is_same_type(leftType: smart_ptr<TypeDecl> implicit; rightType: smart_ptr<TypeDecl> implicit; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool

 Compares two types given comparison parameters and returns true if they match.

:Arguments: * **leftType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **rightType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **constMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **tempMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

.. _function-ast_get_underlying_value_type_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: get_underlying_value_type(type: smart_ptr<TypeDecl> implicit) : smart_ptr<TypeDecl>

 Returns Daslang type which is aliased with ManagedValue handled type.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-ast_get_handled_type_field_type_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit:

.. das:function:: get_handled_type_field_type(type: smart_ptr<TypeAnnotation> implicit; field: string implicit) : TypeInfo?

 Returns type of the field in the ManagedStructure handled type.

:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

.. _function-ast_get_handled_type_field_type_declaration_smart_ptr_ls_TypeAnnotation_gr__implicit_string_implicit_bool:

.. das:function:: get_handled_type_field_type_declaration(type: smart_ptr<TypeAnnotation> implicit; field: string implicit; isConst: bool) : smart_ptr<TypeDecl>

 Returns type declaration of the field in the ManagedStructure handled type.

:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` > implicit

            * **field** : string implicit

            * **isConst** : bool

.. _function-ast_get_handled_type_index_type_declaration_TypeAnnotation_q__implicit_Expression_q__implicit_Expression_q__implicit:

.. das:function:: get_handled_type_index_type_declaration(type: TypeAnnotation? implicit; src: Expression? implicit; idx: Expression? implicit) : smart_ptr<TypeDecl>

 Returns type declaration of the index type in the handled type.

:Arguments: * **type** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ? implicit

            * **src** :  :ref:`Expression <handle-ast-Expression>` ? implicit

            * **idx** :  :ref:`Expression <handle-ast-Expression>` ? implicit

.. _function-ast_get_vector_ptr_at_index_void_q__implicit_TypeDecl_q__implicit_int:

.. das:function:: get_vector_ptr_at_index(vec: void? implicit; type: TypeDecl? implicit; idx: int) : void?

 Returns pointer to the vector element at the specified index, given pointer to the vector object and TypeDeclPtr.

:Arguments: * **vec** : void? implicit

            * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>` ? implicit

            * **idx** : int

.. _function-ast_get_vector_length_void_q__implicit_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: get_vector_length(vec: void? implicit; type: smart_ptr<TypeDecl> implicit) : int

 Returns length of the vector, given pointer to the vector object and TypeDeclPtr.

:Arguments: * **vec** : void? implicit

            * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

.. _function-ast_has_field_smart_ptr_ls_TypeDecl_gr__implicit_string_implicit_bool:

.. das:function:: has_field(type: smart_ptr<TypeDecl> implicit; fieldName: string implicit; constant: bool) : bool

 Returns if structure, variant, tuple, or handled type or pointer to either of those has specific field.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-ast_get_field_type_smart_ptr_ls_TypeDecl_gr__implicit_string_implicit_bool:

.. das:function:: get_field_type(type: smart_ptr<TypeDecl> implicit; fieldName: string implicit; constant: bool) : smart_ptr<TypeDecl>

 Returns type of the field if structure, variant, tuple, or handled type or pointer to either of those has it. It's null otherwise.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-ast_is_visible_directly_Module_q__implicit_Module_q__implicit:

.. das:function:: is_visible_directly(from_module: Module? implicit; which_module: Module? implicit) : bool

 Returns true if module is visible directly from the other module.

:Arguments: * **from_module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **which_module** :  :ref:`Module <handle-rtti-Module>` ? implicit

.. _function-ast_is_expr_like_call_smart_ptr_ls_Expression_gr__const_implicit:

.. das:function:: is_expr_like_call(expression: smart_ptr<Expression> const& implicit) : bool

 Returns true if expression is or inherited from `ExprLooksLikeCall`

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

.. _function-ast_is_expr_const_smart_ptr_ls_Expression_gr__const_implicit:

.. das:function:: is_expr_const(expression: smart_ptr<Expression> const& implicit) : bool

 Returns true if expression is or inherited from `ExprConst`

:Arguments: * **expression** : smart_ptr< :ref:`Expression <handle-ast-Expression>` >& implicit

.. _function-ast_get_function_aot_hash_Function_const_q__implicit:

.. das:function:: get_function_aot_hash(fun: Function const? implicit) : uint64

 Returns hash of the function for the AOT matching.

:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ? implicit

.. _function-ast_get_function_hash_by_id_Function_q__implicit_int_void_q__implicit:

.. das:function:: get_function_hash_by_id(fun: Function? implicit; id: int; pctx: void? implicit) : uint64

 Returns hash of the function by its unique id.

:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **id** : int

            * **pctx** : void? implicit

.. _function-ast_get_aot_arg_suffix_Function_q__implicit_ExprCallFunc_q__implicit_int:

.. das:function:: get_aot_arg_suffix(func: Function? implicit; call: ExprCallFunc? implicit; argIndex: int) : string

 Returns AOT argument suffix for the specified function.

:Arguments: * **func** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>` ? implicit

            * **argIndex** : int

.. _function-ast_get_aot_arg_prefix_Function_q__implicit_ExprCallFunc_q__implicit_int:

.. das:function:: get_aot_arg_prefix(func: Function? implicit; call: ExprCallFunc? implicit; argIndex: int) : string

 Returns AOT argument prefix for the specified function.

:Arguments: * **func** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>` ? implicit

            * **argIndex** : int

.. _function-ast_get_func_aot_prefix_FunctionAnnotation_q__implicit_StringBuilderWriter_q__implicit_ExprCallFunc_q__implicit:

.. das:function:: get_func_aot_prefix(ann: FunctionAnnotation? implicit; stg: StringBuilderWriter? implicit; call: ExprCallFunc? implicit)

 Returns AOT function prefix for the specified function.

:Arguments: * **ann** :  :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>` ? implicit

            * **stg** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>` ? implicit

.. _function-ast_get_struct_aot_prefix_StructureAnnotation_q__implicit_Structure_q__implicit_AnnotationArgumentList_implicit_StringBuilderWriter_q__implicit:

.. das:function:: get_struct_aot_prefix(ann: StructureAnnotation? implicit; structure: Structure? implicit; args: AnnotationArgumentList implicit; stg: StringBuilderWriter? implicit)

 Returns AOT structure prefix for the specified structure.

:Arguments: * **ann** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` ? implicit

            * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  implicit

            * **stg** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

.. _function-ast_get_aot_name_Function_q__implicit_ExprCallFunc_q__implicit:

.. das:function:: get_aot_name(func: Function? implicit; call: ExprCallFunc? implicit) : string

 Returns AOT name for the specified function.

:Arguments: * **func** :  :ref:`Function <handle-ast-Function>` ? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>` ? implicit

.. _function-ast_is_same_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool_bool_bool:

.. das:function:: is_same_type(argType: smart_ptr<TypeDecl> implicit; passType: smart_ptr<TypeDecl> implicit; refMatters: bool; constMatters: bool; temporaryMatters: bool; allowSubstitute: bool) : bool

 Compares two types given comparison parameters and returns true if they match.

:Arguments: * **argType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **passType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **refMatters** : bool

            * **constMatters** : bool

            * **temporaryMatters** : bool

            * **allowSubstitute** : bool

.. _function-ast_get_structure_alias_Structure_q__implicit_string_implicit:

.. das:function:: get_structure_alias(structure: Structure? implicit; aliasName: string implicit) : smart_ptr<TypeDecl>

Finds and returns the structure alias type given alias name.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>` ? implicit

            * **aliasName** : string implicit

+++++
Infer
+++++

  *  :ref:`infer_generic_type (type: smart_ptr\<TypeDecl\> implicit; passType: smart_ptr\<TypeDecl\> implicit; topLevel: bool; isPassType: bool) : smart_ptr\<TypeDecl\> <function-ast_infer_generic_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool>` 
  *  :ref:`update_alias_map (program: smart_ptr\<Program\> implicit; argType: smart_ptr\<TypeDecl\> implicit; passType: smart_ptr\<TypeDecl\> implicit) <function-ast_update_alias_map_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit>` 

.. _function-ast_infer_generic_type_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_bool_bool:

.. das:function:: infer_generic_type(type: smart_ptr<TypeDecl> implicit; passType: smart_ptr<TypeDecl> implicit; topLevel: bool; isPassType: bool) : smart_ptr<TypeDecl>

 Infers generic type for the specified type and pass type.

:Arguments: * **type** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **passType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **topLevel** : bool

            * **isPassType** : bool

.. _function-ast_update_alias_map_smart_ptr_ls_Program_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit_smart_ptr_ls_TypeDecl_gr__implicit:

.. das:function:: update_alias_map(program: smart_ptr<Program> implicit; argType: smart_ptr<TypeDecl> implicit; passType: smart_ptr<TypeDecl> implicit)

 Updates alias map for the specified infer.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **argType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

            * **passType** : smart_ptr< :ref:`TypeDecl <handle-ast-TypeDecl>` > implicit

+++++++++++++
Uncategorized
+++++++++++++

.. _function-ast_module_find_annotation_Module_const_q__implicit_string_implicit:

.. das:function:: module_find_annotation(module: Module const? implicit; name: string implicit) : smart_ptr<Annotation>

 Finds annotation of the specified type in the given module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

.. _function-ast_module_find_type_annotation_Module_const_q__implicit_string_implicit:

.. das:function:: module_find_type_annotation(module: Module const? implicit; name: string implicit) : TypeAnnotation?

 Finds type annotation of the specified type in the given module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

.. _function-ast_not_inferred_Function_q__implicit:

.. das:function:: not_inferred(function: Function? implicit)

 Specifies that function has been modified via macro and should be inferred again.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>` ? implicit

.. _function-ast_module_find_structure_Module_const_q__implicit_string_implicit:

.. das:function:: module_find_structure(program: Module const? implicit; name: string implicit) : Structure?

 Finds structure by name in the specified module.

:Arguments: * **program** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **name** : string implicit

.. _function-ast_debug_helper_iter_structs_smart_ptr_ls_DebugInfoHelper_gr__implicit_block_ls_string;StructInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_structs(helper: smart_ptr<DebugInfoHelper> implicit; blk: block<(string;StructInfo?):void>)

 Iterates through all structures in the DebugInfoHelper, calling the provided block with each structure.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : block<(string; :ref:`StructInfo <handle-rtti-StructInfo>` ?):void> implicit

.. _function-ast_debug_helper_iter_types_smart_ptr_ls_DebugInfoHelper_gr__implicit_block_ls_string;TypeInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_types(helper: smart_ptr<DebugInfoHelper> implicit; blk: block<(string;TypeInfo?):void>)

 Iterates through all types in the DebugInfoHelper, calling the provided block with each type.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : block<(string; :ref:`TypeInfo <handle-rtti-TypeInfo>` ?):void> implicit

.. _function-ast_debug_helper_iter_vars_smart_ptr_ls_DebugInfoHelper_gr__implicit_block_ls_string;VarInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_vars(helper: smart_ptr<DebugInfoHelper> implicit; blk: block<(string;VarInfo?):void>)

 Iterates through all variables in the DebugInfoHelper, calling the provided block with each variable.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : block<(string; :ref:`VarInfo <handle-rtti-VarInfo>` ?):void> implicit

.. _function-ast_debug_helper_iter_funcs_smart_ptr_ls_DebugInfoHelper_gr__implicit_block_ls_string;FuncInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_funcs(helper: smart_ptr<DebugInfoHelper> implicit; blk: block<(string;FuncInfo?):void>)

 Iterates through all functions in the DebugInfoHelper, calling the provided block with each function.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : block<(string; :ref:`FuncInfo <handle-rtti-FuncInfo>` ?):void> implicit

.. _function-ast_debug_helper_iter_enums_smart_ptr_ls_DebugInfoHelper_gr__implicit_block_ls_string;EnumInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_enums(helper: smart_ptr<DebugInfoHelper> implicit; blk: block<(string;EnumInfo?):void>)

 Iterates through all enumerations in the DebugInfoHelper, calling the provided block with each enumeration.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` > implicit

            * **blk** : block<(string; :ref:`EnumInfo <handle-rtti-EnumInfo>` ?):void> implicit

.. _function-ast_debug_helper_find_type_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_implicit_TypeInfo_q__implicit:

.. das:function:: debug_helper_find_type_cppname(helper: smart_ptr<DebugInfoHelper> const& implicit; type_info: TypeInfo? implicit) : string

 Finds type in the DebugInfoHelper and returns it C++ name.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` >& implicit

            * **type_info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-ast_debug_helper_find_struct_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_implicit_StructInfo_q__implicit:

.. das:function:: debug_helper_find_struct_cppname(helper: smart_ptr<DebugInfoHelper> const& implicit; struct_info: StructInfo? implicit) : string

 Finds structure in the DebugInfoHelper and returns it C++ name.

:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>` >& implicit

            * **struct_info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-ast_macro_aot_infix_TypeInfoMacro_q__implicit_StringBuilderWriter_q__implicit_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: macro_aot_infix(macro: TypeInfoMacro? implicit; ss: StringBuilderWriter? implicit; expr: smart_ptr<Expression> implicit) : bool

 Returns true if macro requires AOT infix for the specified handled type.

:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_getInitSemanticHashWithDep_smart_ptr_ls_Program_gr__implicit_uint64:

.. das:function:: getInitSemanticHashWithDep(program: smart_ptr<Program> implicit; init: uint64) : uint64

 Returns initialization semantic hash including dependencies for the entire program.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **init** : uint64

.. _function-ast_aot_require_Module_q__implicit_StringBuilderWriter_q__implicit:

.. das:function:: aot_require(mod: Module? implicit; ss: StringBuilderWriter? implicit) : bool

 Write data to the 'require' section of the AOT module.

:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

.. _function-ast_aot_type_ann_get_field_ptr_TypeAnnotation_q__implicit_StringBuilderWriter_q__implicit_string_implicit:

.. das:function:: aot_type_ann_get_field_ptr(ann: TypeAnnotation? implicit; ss: StringBuilderWriter? implicit; name: string implicit)

 Returns string with access symbol for the field (like -> for the pointer types, or . for the value types).

:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **name** : string implicit

.. _function-ast_aot_need_type_info_TypeInfoMacro_const_q__implicit_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: aot_need_type_info(macro: TypeInfoMacro const? implicit; expr: smart_ptr<Expression> implicit) : bool

 Returns true if TypeInfo? is needed for the specified type in 'typeinfo' expression.

:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ? implicit

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_write_aot_body_StructureAnnotation_q__implicit_smart_ptr_ls_Structure_gr__implicit_AnnotationArgumentList_implicit_StringBuilderWriter_q__implicit:

.. das:function:: write_aot_body(structure: StructureAnnotation? implicit; st: smart_ptr<Structure> implicit; args: AnnotationArgumentList implicit; writer: StringBuilderWriter? implicit)

 Writes AOT body for the specified StructureAnnotation.

:Arguments: * **structure** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` ? implicit

            * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  implicit

            * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

.. _function-ast_write_aot_suffix_StructureAnnotation_q__implicit_smart_ptr_ls_Structure_gr__implicit_AnnotationArgumentList_implicit_StringBuilderWriter_q__implicit:

.. das:function:: write_aot_suffix(structure: StructureAnnotation? implicit; st: smart_ptr<Structure> implicit; args: AnnotationArgumentList implicit; writer: StringBuilderWriter? implicit)

 Writes AOT suffix for the specified StructureAnnotation.

:Arguments: * **structure** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>` ? implicit

            * **st** : smart_ptr< :ref:`Structure <handle-ast-Structure>` > implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  implicit

            * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

.. _function-ast_write_aot_macro_suffix_TypeInfoMacro_q__implicit_StringBuilderWriter_q__implicit_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: write_aot_macro_suffix(macro: TypeInfoMacro? implicit; ss: StringBuilderWriter? implicit; expr: smart_ptr<Expression> implicit)

 Writes AOT macro suffix for the specified TypeInfoMacro.

:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_write_aot_macro_prefix_TypeInfoMacro_q__implicit_StringBuilderWriter_q__implicit_smart_ptr_ls_Expression_gr__implicit:

.. das:function:: write_aot_macro_prefix(macro: TypeInfoMacro? implicit; ss: StringBuilderWriter? implicit; expr: smart_ptr<Expression> implicit)

 Writes AOT macro prefix for the specified TypeInfoMacro.

:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **expr** : smart_ptr< :ref:`Expression <handle-ast-Expression>` > implicit

.. _function-ast_aot_previsit_get_field_ptr_TypeAnnotation_q__implicit_StringBuilderWriter_q__implicit_string_implicit:

.. das:function:: aot_previsit_get_field_ptr(ann: TypeAnnotation? implicit; ss: StringBuilderWriter? implicit; name: string implicit)

 Previsit for getting field pointer in AOT generation.

:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **name** : string implicit

.. _function-ast_aot_previsit_get_field_TypeAnnotation_q__implicit_StringBuilderWriter_q__implicit_string_implicit:

.. das:function:: aot_previsit_get_field(ann: TypeAnnotation? implicit; ss: StringBuilderWriter? implicit; name: string implicit)

 Previsit for getting field in AOT generation.

:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **name** : string implicit

.. _function-ast_aot_visit_get_field_TypeAnnotation_q__implicit_StringBuilderWriter_q__implicit_string_implicit:

.. das:function:: aot_visit_get_field(ann: TypeAnnotation? implicit; ss: StringBuilderWriter? implicit; name: string implicit)

 Visit for getting field in AOT generation.

:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

            * **name** : string implicit

.. _function-ast_string_builder_str_StringBuilderWriter_q__implicit:

.. das:function:: string_builder_str(ss: StringBuilderWriter? implicit) : string

 Returns string from the StringBuilder expression, given pointer to the StringBuilder object.

:Arguments: * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit

.. _function-ast_string_builder_clear_StringBuilderWriter_q__implicit:

.. das:function:: string_builder_clear(ss: StringBuilderWriter? implicit)

 Clears the StringBuilder object, given pointer to the StringBuilder object.

:Arguments: * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ? implicit


