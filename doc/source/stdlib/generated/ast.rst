
.. _stdlib_ast:

========================
AST manipulation library
========================

.. das:module:: ast

The AST module provides access to the abstract syntax tree representation of daslang programs.
It defines node types for all language constructs (expressions, statements, types, functions,
structures, enumerations, etc.), visitors for tree traversal, and utilities for AST
construction and manipulation. This module is the foundation for writing macros, code
generators, and source-level program transformations.

All functions and symbols are in "ast_core" module, use require to get access to it.

.. code-block:: das

    require daslib/ast



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
The structure is a class.

:Fields: * **isClass** (0x1) - Generate constructor.

         * **genCtor** (0x2) - C++ data layout.

         * **cppLayout** (0x4) - C++ layout not POD type, i.e. has alignment to accommodate for inheritance.

         * **cppLayoutNotPod** (0x8) - This structure is compiler-generated.

         * **generated** (0x10) - This structure is using persistent heap (C++ heap).

         * **persistent** (0x20) - This structure is a lambda.

         * **isLambda** (0x40) - This structure is private.

         * **privateStructure** (0x80) - This structure is a macro interface.

         * **macroInterface** (0x100) - This structure is sealed. It cannot be inherited.

         * **_sealed** (0x200) - Skip lock check.

         * **circular** (0x400) - This structure has circular references (and is invalid).

         * **_generator** (0x800) - This structure is a generator.

         * **hasStaticMembers** (0x1000) - This structure has static members.

         * **hasStaticFunctions** (0x2000) - This structure has static functions.

         * **hasInitFields** (0x4000) - This structure has initialized fields.

         * **safeWhenUninitialized** (0x8000) - This structure is safe when uninitialized.

         * **isTemplate** (0x10000) - This structure is a template.

         * **hasDefaultInitializer** (0x20000) - This structure has a default initializer.

         * **noGenCtor** (0x40000) - This structure does not generate a default constructor.



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

         * **needStringCast** (0x2) - Converts das string arguments to C++ ``char *``. Empty string, which is null in das, is converted to "".

         * **aotHashDeppendsOnArguments** (0x4) - Function hash depends on arguments.

         * **lateInit** (0x8) - Function is late initialized.

         * **requestJit** (0x10) - Function is requested to be JIT compiled.

         * **unsafeOutsideOfFor** (0x20) - Function is unsafe outside of for loop sources.

         * **safeImplicit** (0x40) - Skip lock check for this function.

         * **deprecated** (0x80) - Function is safe for implicit calls. Otherwise temp values are to be specialized for in the generic.

         * **aliasCMRES** (0x100) - Function is deprecated.

         * **neverAliasCMRES** (0x200) - Function aliases CMRES (Copy or Move return result).

         * **addressTaken** (0x400) - Function never aliases CMRES.

         * **propertyFunction** (0x800) - Function address is taken.

         * **pinvoke** (0x1000) - Function is a property function.

         * **jitOnly** (0x2000) - Function is a P/Invoke function, i.e. cross-context call.

         * **isStaticClassMethod** (0x4000) - Function is JIT only.

         * **requestNoJit** (0x8000) - Function is a static class method.

         * **jitContextAndLineInfo** (0x10000) - Function is requested to not be JIT compiled.

         * **nodiscard** (0x20000) - Function requires JIT context and line info.

         * **captureString** (0x40000) - Discarding the return value of the function is unsafe.

         * **callCaptureString** (0x80000) - Function captures string arguments.

         * **hasStringBuilder** (0x100000) - Function calls capture string arguments.

         * **recursive** (0x200000) - Function has a string builder.

         * **isTemplate** (0x400000) - Function is recursive.

         * **unsafeWhenNotCloneArray** (0x800000) - Function is a template function.

         * **stub** (0x1000000) - Function is unsafe, when its not used to clone arrays.

         * **lateShutdown** (0x2000000) - This flag is a stub.

         * **hasTryRecover** (0x4000000) - Function will shutdown after all other shutdonws are done.

         * **hasUnsafe** (0x8000000) - Function has try\recover blocks.

         * **isConstClassMethod** (0x10000000) - Function has unsafe operations made by user.

         * **isCustomProperty** (0x20000000) - Function is a const class method.



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

         * **pod_delete** (0x80000) - This variable can be deleted as POD

         * **pod_delete_gen** (0x100000) - POD delete has been generated for this variable

         * **single_return_via_move** (0x200000) - This variable is returned via move in a function with only one return path



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

         * **isGeneratorBlock** (0x10000) - Block is a generator block.



.. _alias-ExprAtFlags:

.. das:attribute:: bitfield ExprAtFlags

properties of the `ExprAt` object.

:Fields: * **r2v** (0x1) - Reference to value conversion is applied.

         * **r2cr** (0x2) - Read to const reference is propagated.

         * **write** (0x4) - The result is written to.

         * **no_promotion** (0x8) - Promotion to operator is disabled, even if operator [] is overloaded.

         * **under_clone** (0x10) - The expression is under a clone operation.

         * **under_deref** (0x20) - The expression is under a dereference (ExprRef2Value), safe for table lookup collision detection.



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
Its 'yield <- ...'.

:Fields: * **moveSemantics** (0x1) - Skip lock checks.



.. _alias-ExprReturnFlags:

.. das:attribute:: bitfield ExprReturnFlags

properties of the `ExprReturn` object.
Its 'return <- ...'.

:Fields: * **moveSemantics** (0x1) - Return a reference. Function result is a reference.

         * **returnReference** (0x2) - Return in block, not in function.

         * **returnInBlock** (0x4) - Take over right stack, i.e its 'return [MakeLocal]' and temp stack value is allocated by return expression.

         * **takeOverRightStack** (0x8) - Return call CMRES, i.e. 'return call(...)'.

         * **returnCallCMRES** (0x10) - Return CMRES, i.e. 'return [MakeLocal]' or 'return [CmresVariable]'

         * **returnCMRES** (0x20) - From yield.

         * **fromYield** (0x40) - From comprehension.

         * **fromComprehension** (0x80) - Skip lock checks.



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

         * **allowConstantLValue** (0x4) - Promote to clone, i.e. this is 'foo := bar' and not 'foo = bar'



.. _alias-MoveFlags:

.. das:attribute:: bitfield MoveFlags

Properties of the `ExprMove` object.
Skip lock checks.

:Fields: * **takeOverRightStack** (0x1) - Its 'foo <- [MakeLocal]' and temp stack value is allocated by move expression.

         * **allowConstantLValue** (0x2) - Move is allowed for constant lvalue, for example x <- 5

         * **podDelete** (0x4) - Move is a POD delete.



.. _alias-IfFlags:

.. das:attribute:: bitfield IfFlags

properties of the `ExprIf` object.

:Fields: * **isStatic** (0x1) - This is a 'static_if' or 'static_elif' expression.

         * **doNotFold** (0x2) - Do not fold this 'if' expression during compilation.



.. _alias-StringBuilderFlags:

.. das:attribute:: bitfield StringBuilderFlags

properties of the `ExprStringBuilder` object.

:Fields: * **isTempString** (0x1) - String builder produces a temporary string.



.. _alias-ExpressionPtr:

.. das:attribute:: ExpressionPtr = Expression?

typedef ExpressionPtr = ast_core::Expression? aka ExpressionPtr

.. _alias-ProgramPtr:

.. das:attribute:: ProgramPtr = smart_ptr<Program>

typedef ProgramPtr = smart_ptr<rtti_core::Program> aka ProgramPtr

.. _alias-TypeDeclPtr:

.. das:attribute:: TypeDeclPtr = TypeDecl?

typedef TypeDeclPtr = ast_core::TypeDecl? aka TypeDeclPtr

.. _alias-VectorTypeDeclPtr:

.. das:attribute:: VectorTypeDeclPtr = dasvector`ptr`TypeDecl

typedef VectorTypeDeclPtr = $::dasvector`ptr`TypeDecl aka VectorTypeDeclPtr

.. _alias-EnumerationPtr:

.. das:attribute:: EnumerationPtr = Enumeration?

typedef EnumerationPtr = ast_core::Enumeration? aka EnumerationPtr

.. _alias-StructurePtr:

.. das:attribute:: StructurePtr = Structure?

typedef StructurePtr = ast_core::Structure? aka StructurePtr

.. _alias-FunctionPtr:

.. das:attribute:: FunctionPtr = Function?

typedef FunctionPtr = ast_core::Function? aka FunctionPtr

.. _alias-VariablePtr:

.. das:attribute:: VariablePtr = Variable?

typedef VariablePtr = ast_core::Variable? aka VariablePtr

.. _alias-MakeFieldDeclPtr:

.. das:attribute:: MakeFieldDeclPtr = MakeFieldDecl?

typedef MakeFieldDeclPtr = ast_core::MakeFieldDecl? aka MakeFieldDeclPtr

.. _alias-ExprMakeBlockPtr:

.. das:attribute:: ExprMakeBlockPtr = ExprMakeBlock?

typedef ExprMakeBlockPtr = ast_core::ExprMakeBlock? aka ExprMakeBlockPtr

.. _alias-FunctionAnnotationPtr:

.. das:attribute:: FunctionAnnotationPtr = smart_ptr<FunctionAnnotation>

typedef FunctionAnnotationPtr = smart_ptr<ast_core::FunctionAnnotation> aka FunctionAnnotationPtr

.. _alias-StructureAnnotationPtr:

.. das:attribute:: StructureAnnotationPtr = smart_ptr<StructureAnnotation>

typedef StructureAnnotationPtr = smart_ptr<ast_core::StructureAnnotation> aka StructureAnnotationPtr

.. _alias-EnumerationAnnotationPtr:

.. das:attribute:: EnumerationAnnotationPtr = smart_ptr<EnumerationAnnotation>

typedef EnumerationAnnotationPtr = smart_ptr<ast_core::EnumerationAnnotation> aka EnumerationAnnotationPtr

.. _alias-PassMacroPtr:

.. das:attribute:: PassMacroPtr = PassMacro?

typedef PassMacroPtr = ast_core::PassMacro? aka PassMacroPtr

.. _alias-VariantMacroPtr:

.. das:attribute:: VariantMacroPtr = VariantMacro?

typedef VariantMacroPtr = ast_core::VariantMacro? aka VariantMacroPtr

.. _alias-ReaderMacroPtr:

.. das:attribute:: ReaderMacroPtr = ReaderMacro?

typedef ReaderMacroPtr = ast_core::ReaderMacro? aka ReaderMacroPtr

.. _alias-CommentReaderPtr:

.. das:attribute:: CommentReaderPtr = CommentReader?

typedef CommentReaderPtr = ast_core::CommentReader? aka CommentReaderPtr

.. _alias-CallMacroPtr:

.. das:attribute:: CallMacroPtr = CallMacro?

typedef CallMacroPtr = ast_core::CallMacro? aka CallMacroPtr

.. _alias-TypeInfoMacroPtr:

.. das:attribute:: TypeInfoMacroPtr = TypeInfoMacro?

typedef TypeInfoMacroPtr = ast_core::TypeInfoMacro? aka TypeInfoMacroPtr

.. _alias-ForLoopMacroPtr:

.. das:attribute:: ForLoopMacroPtr = ForLoopMacro?

typedef ForLoopMacroPtr = ast_core::ForLoopMacro? aka ForLoopMacroPtr

.. _alias-CaptureMacroPtr:

.. das:attribute:: CaptureMacroPtr = CaptureMacro?

typedef CaptureMacroPtr = ast_core::CaptureMacro? aka CaptureMacroPtr

.. _alias-TypeMacroPtr:

.. das:attribute:: TypeMacroPtr = TypeMacro?

typedef TypeMacroPtr = ast_core::TypeMacro? aka TypeMacroPtr

.. _alias-SimulateMacroPtr:

.. das:attribute:: SimulateMacroPtr = SimulateMacro?

typedef SimulateMacroPtr = ast_core::SimulateMacro? aka SimulateMacroPtr


++++++++++++
Enumerations
++++++++++++

.. _enum-ast-CaptureMode:

.. das:attribute:: CaptureMode

Enumeration with lambda variables capture modes.

:Values: * **capture_any** = 0 - Unspecified capture mode (will try copy, then reference - and ask for unsafe).

         * **capture_by_copy** = 1 - Value is copied.

         * **capture_by_reference** = 2 - Reference to the original value is captured (this one is unsafe)

         * **capture_by_clone** = 3 - Value is cloned.

         * **capture_by_move** = 4 - Value is moved.



.. _enum-ast-SideEffects:

.. das:attribute:: SideEffects

Enumeration with all possible side effects of expression or function.

:Values: * **none** = 0 - No side effects.

         * **unsafe** = 1 - Function is unsafe.

         * **userScenario** = 2 - [sideeffects] annotation to indicate side effects.

         * **modifyExternal** = 4 - Function may modify external state.

         * **accessExternal** = 4 - Access to external state.

         * **modifyArgument** = 8 - Function may modify argument values.

         * **modifyArgumentAndExternal** = 12 - Function may modify argument values and external state.

         * **worstDefault** = 12 - Function has all sideeffects, except for a user scenario. This is to bind functions, whith unknown sideeffects.

         * **accessGlobal** = 16 - Function may access global state (variables and such).

         * **invoke** = 32 - Function is using 'invoke', so we don't know any additional side effects.

         * **inferredSideEffects** = 56 - Mask for all sideefects, which can be inferred from the code.




++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-ast-ModuleLibrary:

.. das:attribute:: ModuleLibrary

 Object which holds list of `Module` and provides access to them.



.. _handle-ast-Expression:

.. das:attribute:: Expression

Any expression (base class).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags



.. _handle-ast-TypeDecl:

.. das:attribute:: TypeDecl

.. _function-ast__dot__rq_canAot_TypeDecl:

.. das:function:: TypeDecl.canAot() : bool

Returns whether the given type can be ahead-of-time compiled.


.. _function-ast__dot__rq_isExprType_TypeDecl:

.. das:function:: TypeDecl.isExprType() : bool

Returns whether the type hierarchy contains an expression type.


.. _function-ast__dot__rq_isSimpleType_TypeDecl:

.. das:function:: TypeDecl.isSimpleType() : bool

Returns whether the given type is a simple non-void type that does not require resolution at inference time.


.. _function-ast__dot__rq_isArray_TypeDecl:

.. das:function:: TypeDecl.isArray() : bool

Returns whether the given type is an array type.


.. _function-ast__dot__rq_isGoodIteratorType_TypeDecl:

.. das:function:: TypeDecl.isGoodIteratorType() : bool

Returns whether the given type is an iterator type.


.. _function-ast__dot__rq_isGoodArrayType_TypeDecl:

.. das:function:: TypeDecl.isGoodArrayType() : bool

Returns whether the given type is a dynamic array type.


.. _function-ast__dot__rq_isGoodTableType_TypeDecl:

.. das:function:: TypeDecl.isGoodTableType() : bool

Returns whether the given type is a table type.


.. _function-ast__dot__rq_isGoodBlockType_TypeDecl:

.. das:function:: TypeDecl.isGoodBlockType() : bool

Returns whether the given type is a block type.


.. _function-ast__dot__rq_isGoodFunctionType_TypeDecl:

.. das:function:: TypeDecl.isGoodFunctionType() : bool

Returns whether the given type is a function type.


.. _function-ast__dot__rq_isGoodLambdaType_TypeDecl:

.. das:function:: TypeDecl.isGoodLambdaType() : bool

Returns whether the given type is a lambda type.


.. _function-ast__dot__rq_isGoodTupleType_TypeDecl:

.. das:function:: TypeDecl.isGoodTupleType() : bool

Returns whether the given type is a tuple type.


.. _function-ast__dot__rq_isGoodVariantType_TypeDecl:

.. das:function:: TypeDecl.isGoodVariantType() : bool

Returns whether the given type is a variant type.


.. _function-ast__dot__rq_isVoid_TypeDecl:

.. das:function:: TypeDecl.isVoid() : bool

Returns whether the given type is the void type.


.. _function-ast__dot__rq_isAnyType_TypeDecl:

.. das:function:: TypeDecl.isAnyType() : bool

Returns whether the given type is the any type, passed as vec4f via standard C++ interop.


.. _function-ast__dot__rq_isRef_TypeDecl:

.. das:function:: TypeDecl.isRef() : bool

Returns whether the given type is a reference value.


.. _function-ast__dot__rq_isRefType_TypeDecl:

.. das:function:: TypeDecl.isRefType() : bool

Returns whether the given type is a reference type.


.. _function-ast__dot__rq_canWrite_TypeDecl:

.. das:function:: TypeDecl.canWrite() : bool

Returns whether the given type can be written to.


.. _function-ast__dot__rq_isAotAlias_TypeDecl:

.. das:function:: TypeDecl.isAotAlias() : bool

Returns whether the type definition contains an AOT alias type.


.. _function-ast__dot__rq_isShareable_TypeDecl:

.. das:function:: TypeDecl.isShareable() : bool

Returns whether the given type is shareable across contexts.


.. _function-ast__dot__rq_isIndex_TypeDecl:

.. das:function:: TypeDecl.isIndex() : bool

Returns whether the given type is an index type.


.. _function-ast__dot__rq_isBool_TypeDecl:

.. das:function:: TypeDecl.isBool() : bool

Returns whether the given type is a boolean type.


.. _function-ast__dot__rq_isInteger_TypeDecl:

.. das:function:: TypeDecl.isInteger() : bool

Returns whether the given type is an integer type.


.. _function-ast__dot__rq_isSignedInteger_TypeDecl:

.. das:function:: TypeDecl.isSignedInteger() : bool

Returns whether the given type is a signed integer type.


.. _function-ast__dot__rq_isUnsignedInteger_TypeDecl:

.. das:function:: TypeDecl.isUnsignedInteger() : bool

Returns whether the given type is an unsigned integer type.


.. _function-ast__dot__rq_isSignedIntegerOrIntVec_TypeDecl:

.. das:function:: TypeDecl.isSignedIntegerOrIntVec() : bool

Returns whether the given type is a signed integer or signed integer vector type.


.. _function-ast__dot__rq_isUnsignedIntegerOrIntVec_TypeDecl:

.. das:function:: TypeDecl.isUnsignedIntegerOrIntVec() : bool

Returns whether the given type is an unsigned integer or unsigned integer vector type.


.. _function-ast__dot__rq_isFloatOrDouble_TypeDecl:

.. das:function:: TypeDecl.isFloatOrDouble() : bool

Returns whether the given type is a float or double type.


.. _function-ast__dot__rq_isNumeric_TypeDecl:

.. das:function:: TypeDecl.isNumeric() : bool

Returns whether the given type is a numeric type.


.. _function-ast__dot__rq_isNumericComparable_TypeDecl:

.. das:function:: TypeDecl.isNumericComparable() : bool

Returns whether the given type supports numeric comparison.


.. _function-ast__dot__rq_isPointer_TypeDecl:

.. das:function:: TypeDecl.isPointer() : bool

Returns whether the given type is a pointer type.


.. _function-ast__dot__rq_isSmartPointer_TypeDecl:

.. das:function:: TypeDecl.isSmartPointer() : bool

Returns whether the given type is a smart pointer type.


.. _function-ast__dot__rq_isVoidPointer_TypeDecl:

.. das:function:: TypeDecl.isVoidPointer() : bool

Returns whether the given type is a void pointer type.


.. _function-ast__dot__rq_isIterator_TypeDecl:

.. das:function:: TypeDecl.isIterator() : bool

Returns whether the given type is an iterator type.


.. _function-ast__dot__rq_isEnum_TypeDecl:

.. das:function:: TypeDecl.isEnum() : bool

Returns whether the given type is an enumeration type.


.. _function-ast__dot__rq_isEnumT_TypeDecl:

.. das:function:: TypeDecl.isEnumT() : bool

Returns whether the base type of the given type is an enumeration type.


.. _function-ast__dot__rq_isHandle_TypeDecl:

.. das:function:: TypeDecl.isHandle() : bool

Returns whether the given type is a handle type, representing a C++ type exposed to daslang via TypeAnnotation.


.. _function-ast__dot__rq_isStructure_TypeDecl:

.. das:function:: TypeDecl.isStructure() : bool

Returns whether the given type is a structure type.


.. _function-ast__dot__rq_isClass_TypeDecl:

.. das:function:: TypeDecl.isClass() : bool

Returns whether the given type is a class type.


.. _function-ast__dot__rq_isFunction_TypeDecl:

.. das:function:: TypeDecl.isFunction() : bool

Returns whether the given type is a function type.


.. _function-ast__dot__rq_isTuple_TypeDecl:

.. das:function:: TypeDecl.isTuple() : bool

Returns whether the given type is a tuple type.


.. _function-ast__dot__rq_isVariant_TypeDecl:

.. das:function:: TypeDecl.isVariant() : bool

Returns whether the given type is a variant type.


.. _function-ast__dot__rq_sizeOf_TypeDecl:

.. das:function:: TypeDecl.sizeOf() : int

Returns the size of the given type in bytes.


.. _function-ast__dot__rq_countOf_TypeDecl:

.. das:function:: TypeDecl.countOf() : int

Returns the number of elements if the given type is a fixed array, otherwise returns 1.


.. _function-ast__dot__rq_alignOf_TypeDecl:

.. das:function:: TypeDecl.alignOf() : int

Returns the memory alignment requirement of the type in bytes.


.. _function-ast__dot__rq_baseSizeOf_TypeDecl:

.. das:function:: TypeDecl.baseSizeOf() : int

Returns the size of the given type in bytes, excluding fixed array dimensions.


.. _function-ast__dot__rq_stride_TypeDecl:

.. das:function:: TypeDecl.stride() : int

Returns the stride size in bytes of an element in a fixed array type.


.. _function-ast__dot__rq_tupleSize_TypeDecl:

.. das:function:: TypeDecl.tupleSize() : int

Returns the size of the given tuple type in bytes.


.. _function-ast__dot__rq_tupleAlign_TypeDecl:

.. das:function:: TypeDecl.tupleAlign() : int

Returns the alignment of the given tuple type in bytes.


.. _function-ast__dot__rq_variantSize_TypeDecl:

.. das:function:: TypeDecl.variantSize() : int

Returns the size of the given variant type in bytes.


.. _function-ast__dot__rq_variantAlign_TypeDecl:

.. das:function:: TypeDecl.variantAlign() : int

Returns the alignment of the given variant type in bytes.


.. _function-ast__dot__rq_canCopy_TypeDecl:

.. das:function:: TypeDecl.canCopy() : bool

Returns whether the given type can be copied.


.. _function-ast__dot__rq_canMove_TypeDecl:

.. das:function:: TypeDecl.canMove() : bool

Returns whether the given type can be moved.


.. _function-ast__dot__rq_canClone_TypeDecl:

.. das:function:: TypeDecl.canClone() : bool

Returns whether the given type can be cloned.


.. _function-ast__dot__rq_canCloneFromConst_TypeDecl:

.. das:function:: TypeDecl.canCloneFromConst() : bool

Returns whether the given type can be cloned from a const instance.


.. _function-ast__dot__rq_canNew_TypeDecl:

.. das:function:: TypeDecl.canNew() : bool

Returns whether the given type can be heap-allocated via the new operator.


.. _function-ast__dot__rq_canDeletePtr_TypeDecl:

.. das:function:: TypeDecl.canDeletePtr() : bool

Returns whether the pointer to the given type can be deleted.


.. _function-ast__dot__rq_canDelete_TypeDecl:

.. das:function:: TypeDecl.canDelete() : bool

Returns whether the given type can be deleted.


.. _function-ast__dot__rq_needDelete_TypeDecl:

.. das:function:: TypeDecl.needDelete() : bool

Returns whether the given type requires explicit deletion.


.. _function-ast__dot__rq_isPod_TypeDecl:

.. das:function:: TypeDecl.isPod() : bool

Returns whether the given type is a plain old data (POD) type.


.. _function-ast__dot__rq_isRawPod_TypeDecl:

.. das:function:: TypeDecl.isRawPod() : bool

Returns whether the given type is a raw POD type containing no pointers or strings.


.. _function-ast__dot__rq_isNoHeapType_TypeDecl:

.. das:function:: TypeDecl.isNoHeapType() : bool

Returns whether the given type can be used without heap allocation.


.. _function-ast__dot__rq_isWorkhorseType_TypeDecl:

.. das:function:: TypeDecl.isWorkhorseType() : bool

Returns whether the given type is a workhorse type, which is a built-in non-reference type.


.. _function-ast__dot__rq_isPolicyType_TypeDecl:

.. das:function:: TypeDecl.isPolicyType() : bool

Returns whether the given type is a policy type with SimNode implementations available for it.


.. _function-ast__dot__rq_isVecPolicyType_TypeDecl:

.. das:function:: TypeDecl.isVecPolicyType() : bool

Returns whether the given type is a vector policy type, which is any policy type other than string.


.. _function-ast__dot__rq_isReturnType_TypeDecl:

.. das:function:: TypeDecl.isReturnType() : bool

Returns whether the given type can be used as a return type, which includes anything except block.


.. _function-ast__dot__rq_isCtorType_TypeDecl:

.. das:function:: TypeDecl.isCtorType() : bool

Returns whether the given basic type is a constructor type that can be constructed via its type name, such as int(3.4).


.. _function-ast__dot__rq_isRange_TypeDecl:

.. das:function:: TypeDecl.isRange() : bool

Returns whether the given type is a range type.


.. _function-ast__dot__rq_isString_TypeDecl:

.. das:function:: TypeDecl.isString() : bool

Returns whether the given type is a string type.


.. _function-ast__dot__rq_isConst_TypeDecl:

.. das:function:: TypeDecl.isConst() : bool

Returns whether the given type is const-qualified.


.. _function-ast__dot__rq_isFoldable_TypeDecl:

.. das:function:: TypeDecl.isFoldable() : bool

Returns whether the given type is foldable, such as integer or float, as opposed to pointer or array.


.. _function-ast__dot__rq_isAlias_TypeDecl:

.. das:function:: TypeDecl.isAlias() : bool

Returns whether the type definition contains an alias type.


.. _function-ast__dot__rq_isAutoArrayResolved_TypeDecl:

.. das:function:: TypeDecl.isAutoArrayResolved() : bool

Returns whether all fixed array dimensions are fully resolved with no auto or expression dimensions remaining.


.. _function-ast__dot__rq_isAuto_TypeDecl:

.. das:function:: TypeDecl.isAuto() : bool

Returns whether the type definition contains an auto type.


.. _function-ast__dot__rq_isAutoOrAlias_TypeDecl:

.. das:function:: TypeDecl.isAutoOrAlias() : bool

Returns whether the type definition contains an auto or alias type.


.. _function-ast__dot__rq_isVectorType_TypeDecl:

.. das:function:: TypeDecl.isVectorType() : bool

Returns whether the given type is a vector type such as int2, float3, or range64.


.. _function-ast__dot__rq_isBitfield_TypeDecl:

.. das:function:: TypeDecl.isBitfield() : bool

Returns whether the given type is a bitfield type.


.. _function-ast__dot__rq_isLocal_TypeDecl:

.. das:function:: TypeDecl.isLocal() : bool

Returns whether the given type is a local type that can be allocated on the stack.


.. _function-ast__dot__rq_hasClasses_TypeDecl:

.. das:function:: TypeDecl.hasClasses() : bool

Returns whether the type definition contains any class types.


.. _function-ast__dot__rq_hasNonTrivialCtor_TypeDecl:

.. das:function:: TypeDecl.hasNonTrivialCtor() : bool

Returns whether the type definition contains any non-trivial constructors.


.. _function-ast__dot__rq_hasNonTrivialDtor_TypeDecl:

.. das:function:: TypeDecl.hasNonTrivialDtor() : bool

Returns whether the type definition contains any non-trivial destructors.


.. _function-ast__dot__rq_hasNonTrivialCopy_TypeDecl:

.. das:function:: TypeDecl.hasNonTrivialCopy() : bool

Returns whether the type definition contains any non-trivial copy operations.


.. _function-ast__dot__rq_canBePlacedInContainer_TypeDecl:

.. das:function:: TypeDecl.canBePlacedInContainer() : bool

Returns whether the given type can be placed in a container.


.. _function-ast__dot__rq_vectorBaseType_TypeDecl:

.. das:function:: TypeDecl.vectorBaseType() : Type

Returns the scalar base type of a vector type, for example float for float4.


.. _function-ast__dot__rq_vectorDim_TypeDecl:

.. das:function:: TypeDecl.vectorDim() : int

Returns the number of components in a vector type, for example 4 for float4.


.. _function-ast__dot__rq_canInitWithZero_TypeDecl:

.. das:function:: TypeDecl.canInitWithZero() : bool

Returns whether the given type can be initialized by zeroing its memory.


.. _function-ast__dot__rq_rangeBaseType_TypeDecl:

.. das:function:: TypeDecl.rangeBaseType() : Type

Returns the base type of a range type, for example int64 for range64.


.. _function-ast__dot__rq_unsafeInit_TypeDecl:

.. das:function:: TypeDecl.unsafeInit() : bool

Returns whether the given type requires initialization and skipping it would be unsafe.


.. _function-ast__dot__rq_get_mnh_TypeDecl:

.. das:function:: TypeDecl.get_mnh() : uint64

Returns the mangled name hash of the given type.


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

:Fields: * **baseType** :  :ref:`Type <enum-rtti-Type>` - Basic declaration type

         * **structType** :  :ref:`Structure <handle-ast-Structure>`? - Structure type if baseType is Type::tStructure

         * **enumType** :  :ref:`Enumeration <handle-ast-Enumeration>`? - Enumeration type if baseType is Type::tEnumeration

         * **annotation** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? - Handled type if baseType is Type::tHandle

         * **firstType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - First type for compound types (like array<firstType> or table<firstType, secondType>)

         * **secondType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Second type for compound types (like table<firstType, secondType>)

         * **argTypes** : vector<TypeDecl*> - Argument types for function types, tuples, variants, etc

         * **argNames** : vector<das_string> - Argument names for function types

         * **dim** : vector<int> - Dimensions for fixed array types

         * **dimExpr** : vector<Expression*> - Dimension expressions for fixed array types, when dimension is specified by expression

         * **flags** :  :ref:`TypeDeclFlags <alias-TypeDeclFlags>` - Type declaration flags

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` - Alias name for typedefs, i.e. 'int aka MyInt' or 'MyInt'

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the type declaration in the source code

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this type belongs to



.. _handle-ast-Structure:

.. das:attribute:: Structure

.. _function-ast__dot__rq_sizeOf_Structure:

.. das:function:: Structure.sizeOf() : int

Returns the size of the given type in bytes.


:Properties: * **sizeOf** : int

Structure declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the structure

         * **fields** : vector<FieldDeclaration> - Field declarations of the structure

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the structure declaration in the source code

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this structure belongs to

         * **parent** :  :ref:`Structure <handle-ast-Structure>`? - Parent structure, if any

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - List of annotations attached to this structure

         * **flags** :  :ref:`StructureFlags <alias-StructureFlags>` - Structure flags



.. _handle-ast-FieldDeclaration:

.. das:attribute:: FieldDeclaration

Structure field declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the field

         * **init** :  :ref:`Expression <handle-ast-Expression>`? - Expression for field initializer, if any

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` - Annotations attached to this field

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field declaration in the source code

         * **offset** : int - Offset of the field in the structure

         * **flags** :  :ref:`FieldDeclarationFlags <alias-FieldDeclarationFlags>` - Field flags



.. _handle-ast-EnumEntry:

.. das:attribute:: EnumEntry

Entry in the enumeration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the enumeration entry

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` - C++ name of the enumeration entry

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enumeration entry in the source code

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Value of the enumeration entry (typicall 'ExprConst' derivative)



.. _handle-ast-Enumeration:

.. das:attribute:: Enumeration

Enumeration declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the enumeration

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` - C++ name of the enumeration

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enumeration declaration in the source code

         * **list** : vector<EnumEntry> - List of entries in the enumeration

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this enumeration belongs to

         * **external** : bool - Whether this enumeration is external (defined on the C++ side)

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Enumeration underlying type (int8, int16, int, or int64)

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - Annotations attached to this enumeration

         * **isPrivate** : bool - Is this enumeration private (not visible from outside the module)



.. _handle-ast-Function:

.. das:attribute:: Function

.. _function-ast__dot__rq_origin_Function:

.. das:function:: Function.origin() : Function?

Returns the origin function, indicating which generic function this was instantiated from, if any.


.. _function-ast__dot__rq_getMangledNameHash_Function:

.. das:function:: Function.getMangledNameHash() : uint64

Returns the mangled name hash of the given function.


.. _function-ast__dot__rq_isGeneric_Function:

.. das:function:: Function.isGeneric() : bool

Returns whether the given function is a generic function.


:Properties: * **origin** :  :ref:`Function <handle-ast-Function>`?

             * **getMangledNameHash** : uint64

             * **isGeneric** : bool

Function declaration.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - Annotations attached to this function

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the function

         * **arguments** : vector<Variable*> - Arguments of the function

         * **result** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Result type of the function

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - Body expression of the function (usually 'ExprBlock' but can be optimized out on later stages)

         * **index** : int - Index of the function in the 'Context'

         * **totalStackSize** : uint - Stack size required for this function

         * **totalGenLabel** : int - Number of generated labels in the jump table (for the generator)

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function in the source code

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function declaration in the source code

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this function belongs to

         * **classParent** :  :ref:`Structure <handle-ast-Structure>`? - Parent structure if this is a method

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` - Function flags

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` - More function flags

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` - Function side effect flags

         * **inferStack** : vector<InferHistory> - Inference history

         * **fromGeneric** :  :ref:`Function <handle-ast-Function>`? - If this function was instantiated from a generic function, pointer to the generic function

         * **hash** : uint64 - Hash of the function signature

         * **aotHash** : uint64 - Hash of the function signature for AOT purposes



.. _handle-ast-BuiltInFunction:

.. das:attribute:: BuiltInFunction

Bindings for the 'BuiltInFunction', which is used for the builtin (bound) functions in Daslang.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - Annotations attached to this function

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the function

         * **arguments** : vector<Variable*> - Arguments of the function

         * **result** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Result type of the function

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - Body expression of the function (null just about every time for the builtins)

         * **index** : int - Index of the function in the 'Context'

         * **totalStackSize** : uint - Stack size required for this function

         * **totalGenLabel** : int - Number of generated labels in the jump table (for the generator)

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function in the source code

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function declaration in the source code

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this function belongs to

         * **classParent** :  :ref:`Structure <handle-ast-Structure>`? - Parent structure if this is a method

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` - Function flags

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` - More function flags

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` - Function side effect flags

         * **inferStack** : vector<InferHistory> - Inference history

         * **fromGeneric** :  :ref:`Function <handle-ast-Function>`? - If this function was instantiated from a generic function, pointer to the generic function

         * **hash** : uint64 - Hash of the function signature

         * **aotHash** : uint64 - Hash of the function signature for AOT purposes

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` - C++ function name.



.. _handle-ast-ExternalFnBase:

.. das:attribute:: ExternalFnBase

Base class for external function bindings.
Bindings for the 'BuiltInFunction', which is used for the builtin (bound) functions in Daslang.

:Fields: * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - Annotations attached to this function

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the function

         * **arguments** : vector<Variable*> - Arguments of the function

         * **result** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Result type of the function

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - Body expression of the function (null just about every time for the external functions)

         * **index** : int - Index of the function in the 'Context'

         * **totalStackSize** : uint - Stack size required for this function

         * **totalGenLabel** : int - Number of generated labels in the jump table (for the generator)

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function in the source code

         * **atDecl** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function declaration in the source code

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this function belongs to

         * **classParent** :  :ref:`Structure <handle-ast-Structure>`? - Parent structure if this is a method

         * **flags** :  :ref:`FunctionFlags <alias-FunctionFlags>` - Function flags

         * **moreFlags** :  :ref:`MoreFunctionFlags <alias-MoreFunctionFlags>` - More function flags

         * **sideEffectFlags** :  :ref:`FunctionSideEffectFlags <alias-FunctionSideEffectFlags>` - Function side effect flags

         * **inferStack** : vector<InferHistory> - Inference history

         * **fromGeneric** :  :ref:`Function <handle-ast-Function>`? - If this function was instantiated from a generic function, pointer to the generic function

         * **hash** : uint64 - Hash of the function signature

         * **aotHash** : uint64 - Hash of the function signature for AOT purposes

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>` - C++ function name.



.. _handle-ast-InferHistory:

.. das:attribute:: InferHistory

Generic function infer history.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the function in the source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being inferred



.. _handle-ast-Variable:

.. das:attribute:: Variable

.. _function-ast__dot__rq_isAccessUnused_Variable:

.. das:function:: Variable.isAccessUnused() : bool

Returns whether the given variable is never accessed in the code.


.. _function-ast__dot__rq_getMangledNameHash_Variable:

.. das:function:: Variable.getMangledNameHash() : uint64

Returns the mangled name hash of the given function.


:Properties: * **isAccessUnused** : bool

             * **getMangledNameHash** : uint64

Variable declaration.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the variable

         * **_aka** :  :ref:`das_string <handle-builtin-das_string>` - Alternative name of the variable

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the variable

         * **init** :  :ref:`Expression <handle-ast-Expression>`? - Initializer expression for the variable, if any

         * **source** :  :ref:`Expression <handle-ast-Expression>`? - If its an iterator variable for the for loop, source expression being iterated over

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the variable declaration in the source code

         * **index** : int - Index of the variable in the global variable list (for global variables)

         * **stackTop** : uint - Stack top offset for local variables

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module this variable belongs to

         * **initStackSize** : uint - Stack size required to evaluate the initializer expression (for global variables)

         * **flags** :  :ref:`VariableFlags <alias-VariableFlags>` - Variable flags

         * **access_flags** :  :ref:`VariableAccessFlags <alias-VariableAccessFlags>` - Variable access flags

         * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` - Annotations attached to this variable



.. _handle-ast-AstContext:

.. das:attribute:: AstContext

Lexical context for the particular expression.

:Fields: * **func** :  :ref:`Function <handle-ast-Function>`? - Function this expression belongs to

         * **_loop** : vector<Expression*> - Loop stack

         * **blocks** : vector<Expression*> - Stack of active blocks

         * **scopes** : vector<Expression*> - Stack of active scopes

         * **_with** : vector<Expression*> - Stack of active 'with' expressions



.. _handle-ast-ExprBlock:

.. das:attribute:: ExprBlock

Any block expression, including regular blocks and all types of closures.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **list** : vector<Expression*> - List of expressions in the main body of the block

         * **finalList** : vector<Expression*> - List of expressions in the 'finally' section of the block

         * **returnType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Declared return type of the block, if any (for closures)

         * **arguments** : vector<Variable*> - List of arguments for the block (for closures)

         * **stackTop** : uint - Stack top offset for the block declaration

         * **stackVarTop** : uint - Where variables of the block start on the stack

         * **stackVarBottom** : uint - Where variables of the block end on the stack

         * **stackCleanVars** : vector<pair`uint`uint> - Variables which are to be zeroed, if there is 'finally' section of the block. If there is 'inscope' variable after the return, it should be zeroed before entering the block.

         * **maxLabelIndex** : int - Maximum label index used in this block (for goto statements)

         * **annotations** :  :ref:`AnnotationList <handle-rtti-AnnotationList>` - AnnotationList - Annotations attached to this block

         * **annotationData** : uint64 - Opaque data associated with block

         * **annotationDataSid** : uint64 - Opaque data source unique-ish id associated with block

         * **blockFlags** :  :ref:`ExprBlockFlags <alias-ExprBlockFlags>` - Block expression flags

         * **inFunction** :  :ref:`Function <handle-ast-Function>`? - Which function this block belongs to



.. _handle-ast-ExprLet:

.. das:attribute:: ExprLet

Local variable declaration (`let v = expr;`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **variables** : vector<Variable*> - List of variables being declared in this `let` expression

         * **atInit** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the initializer expression in source code

         * **letFlags** :  :ref:`ExprLetFlags <alias-ExprLetFlags>` - Properties of the `ExprLet` object.



.. _handle-ast-ExprStringBuilder:

.. das:attribute:: ExprStringBuilder

String builder expression ("blah{blah1}blah2").

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **elements** : vector<Expression*> - List of expressions that make up the string builder (literals and expressions)

         * **stringBuilderFlags** :  :ref:`StringBuilderFlags <alias-StringBuilderFlags>` - Flags specific to string builder expressions



.. _handle-ast-MakeFieldDecl:

.. das:attribute:: MakeFieldDecl

Part of `ExprMakeStruct`, declares single field (`a = expr` or `a <- expr` etc)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being assigned

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Initializer expression for the field

         * **tag** :  :ref:`Expression <handle-ast-Expression>`? - Tag associated with the field, if any

         * **flags** :  :ref:`MakeFieldDeclFlags <alias-MakeFieldDeclFlags>` - Flags specific to this field declaration



.. _handle-ast-ExprNamedCall:

.. das:attribute:: ExprNamedCall

Named call (`call([argname1=expr1, argname2=expr2])`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the named call

         * **nonNamedArguments** : vector<Expression*> - Non-named arguments passed to the call

         * **arguments** :  :ref:`MakeStruct <handle-ast-MakeStruct>`? - Named arguments passed to the call

         * **argumentsFailedToInfer** : bool - Whether any arguments failed to infer their types



.. _handle-ast-ExprLooksLikeCall:

.. das:attribute:: ExprLooksLikeCall

Anything which looks like call (`call(expr1,expr2)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call

         * **arguments** : vector<Expression*> - List of arguments passed to the call

         * **argumentsFailedToInfer** : bool - Whether any arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code



.. _handle-ast-ExprCallFunc:

.. das:attribute:: ExprCallFunc

Actual function call (`func(expr1,...)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the called function

         * **arguments** : vector<Expression*> - Arguments passed to the function

         * **argumentsFailedToInfer** : bool - Whether any arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Pointer to the function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed



.. _handle-ast-ExprNew:

.. das:attribute:: ExprNew

New expression (`new Foo`, `new Bar(expr1..)`, but **NOT** `new [[Foo ...]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the new expression

         * **arguments** : vector<Expression*> - List of arguments passed to the constructor

         * **argumentsFailedToInfer** : bool - Whether any arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Pointer to the constructor function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **typeexpr** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type expression for the type being constructed

         * **initializer** : bool - Whether there is an initializer for the new expression, or it's just default construction



.. _handle-ast-ExprCall:

.. das:attribute:: ExprCall

Anything which looks like call (`call(expr1,expr2)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call

         * **arguments** : vector<Expression*> - List of arguments passed to the function

         * **argumentsFailedToInfer** : bool - Whether any arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Pointer to the function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **doesNotNeedSp** : bool - If the call does not need stack pointer

         * **cmresAlias** : bool - If the call uses CMRES (Copy or Move result) aliasing, i.e would need temporary

         * **notDiscarded** : bool - If the call result is not discarded



.. _handle-ast-ExprPtr2Ref:

.. das:attribute:: ExprPtr2Ref

Pointer dereference (`*expr` or `deref(expr)`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Expression being dereferenced

         * **unsafeDeref** : bool - If true, skip runtime null-pointer check

         * **assumeNoAlias** : bool - If true, assume no aliasing occurs



.. _handle-ast-ExprNullCoalescing:

.. das:attribute:: ExprNullCoalescing

Null coalescing (`expr1 ?? default_value`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Expression being coalesced

         * **unsafeDeref** : bool - If true, skip runtime null-pointer check

         * **assumeNoAlias** : bool - Assume no aliasing occurs

         * **defaultValue** :  :ref:`Expression <handle-ast-Expression>`? - Default value expression



.. _handle-ast-ExprAt:

.. das:attribute:: ExprAt

Index lookup (`expr[expr1]`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression being indexed

         * **index** :  :ref:`Expression <handle-ast-Expression>`? - Index expression

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` - Flags specific to `ExprAt` expressions



.. _handle-ast-ExprSafeAt:

.. das:attribute:: ExprSafeAt

Safe index lookup (`expr?[expr1]`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression being indexed

         * **index** :  :ref:`Expression <handle-ast-Expression>`? - Index expression

         * **atFlags** :  :ref:`ExprAtFlags <alias-ExprAtFlags>` - Flags specific to `ExprAt` expressions



.. _handle-ast-ExprIs:

.. das:attribute:: ExprIs

Is expression for variants and such (`expr is Foo`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression being checked

         * **typeexpr** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being checked against



.. _handle-ast-ExprOp:

.. das:attribute:: ExprOp

 Compilation time only base class for any operator.



.. _handle-ast-ExprOp2:

.. das:attribute:: ExprOp2

Two operand operator (`expr1 + expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call (unused)

         * **arguments** : vector<Expression*> - Arguments (unused)

         * **argumentsFailedToInfer** : bool - If arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **left** :  :ref:`Expression <handle-ast-Expression>`? - Left operand expression

         * **right** :  :ref:`Expression <handle-ast-Expression>`? - Right operand expression



.. _handle-ast-ExprOp3:

.. das:attribute:: ExprOp3

Three operand operator (`cond ? expr1 : expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call (unused)

         * **arguments** : vector<Expression*> - Arguments (unused)

         * **argumentsFailedToInfer** : bool - If arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Condition expression

         * **left** :  :ref:`Expression <handle-ast-Expression>`? - Left operand expression

         * **right** :  :ref:`Expression <handle-ast-Expression>`? - Right operand expression



.. _handle-ast-ExprCopy:

.. das:attribute:: ExprCopy

Copy operator (`expr1 = expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call (unused)

         * **arguments** : vector<Expression*> - Arguments (unused)

         * **argumentsFailedToInfer** : bool - If arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **left** :  :ref:`Expression <handle-ast-Expression>`? - Left operand expression

         * **right** :  :ref:`Expression <handle-ast-Expression>`? - Right operand expression

         * **copy_flags** :  :ref:`CopyFlags <alias-CopyFlags>` - Flags specific to copy operation



.. _handle-ast-ExprMove:

.. das:attribute:: ExprMove

Move operator (`expr1 <- expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call (unused)

         * **arguments** : vector<Expression*> - Arguments (unused)

         * **argumentsFailedToInfer** : bool - If arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **left** :  :ref:`Expression <handle-ast-Expression>`? - Left operand expression

         * **right** :  :ref:`Expression <handle-ast-Expression>`? - Right operand expression

         * **move_flags** :  :ref:`MoveFlags <alias-MoveFlags>` - Flags specific to move operation



.. _handle-ast-ExprClone:

.. das:attribute:: ExprClone

Clone operator (`expr1 := expr2`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the call (unused)

         * **arguments** : vector<Expression*> - Arguments (unused)

         * **argumentsFailedToInfer** : bool - If arguments failed to infer their types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being called, if resolved

         * **stackTop** : uint - Stack top at the point of call, if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **left** :  :ref:`Expression <handle-ast-Expression>`? - Left operand expression

         * **right** :  :ref:`Expression <handle-ast-Expression>`? - Right operand expression



.. _handle-ast-ExprWith:

.. das:attribute:: ExprWith

With section (`with expr {your; block; here}`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **_with** :  :ref:`Expression <handle-ast-Expression>`? - The expression to be used as the context for the with block

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - The body of the with block



.. _handle-ast-ExprAssume:

.. das:attribute:: ExprAssume

Assume expression (`assume name = expr`) or (`typedef name = type`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **alias** :  :ref:`das_string <handle-builtin-das_string>` - The alias name for the assume expression

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The expression being aliased, if specified

         * **assumeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - The type being assumed, if specified



.. _handle-ast-ExprWhile:

.. das:attribute:: ExprWhile

While loop (`while expr {your; block; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **cond** :  :ref:`Expression <handle-ast-Expression>`? - The condition expression

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - The body of the while loop



.. _handle-ast-ExprTryCatch:

.. das:attribute:: ExprTryCatch

Try-recover expression (`try {your; block; here;} recover {your; recover; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **try_block** :  :ref:`Expression <handle-ast-Expression>`? - The try block

         * **catch_block** :  :ref:`Expression <handle-ast-Expression>`? - The recover block



.. _handle-ast-ExprIfThenElse:

.. das:attribute:: ExprIfThenElse

If-then-else expression (`if expr1 {your; block; here;} else {your; block; here;}`) including `static_if`'s.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **cond** :  :ref:`Expression <handle-ast-Expression>`? - The condition expression

         * **if_true** :  :ref:`Expression <handle-ast-Expression>`? - The 'then' block expression

         * **if_false** :  :ref:`Expression <handle-ast-Expression>`? - The 'else' block expression

         * **if_flags** :  :ref:`IfFlags <alias-IfFlags>` - Flags specific to if-then-else expressions



.. _handle-ast-ExprFor:

.. das:attribute:: ExprFor

For loop (`for expr1 in expr2 {your; block; here;}`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **iterators** : vector<das_string> - Names of the iterator variables

         * **iteratorsAka** : vector<das_string> - Aliases for the iterator variables

         * **iteratorsAt** : vector<LineInfo> - Line information for each iterator

         * **iteratorsTags** : vector<Expression*> - Tags associated with each iterator

         * **iteratorsTupleExpansion** : vector<uint8> - Tuple expansion flags for iterators

         * **iteratorVariables** : vector<Variable*> - Variables associated with each iterator

         * **sources** : vector<Expression*> - Source expressions to iterate over

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - The body of the for loop

         * **visibility** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Line information for visibility of the iterators

         * **allowIteratorOptimization** : bool - Whether iterator optimization is allowed

         * **canShadow** : bool - Whether shadowing is allowed, i.e. if the iterator names can shadow outer scope variables



.. _handle-ast-ExprMakeLocal:

.. das:attribute:: ExprMakeLocal

Any make expression (`ExprMakeBlock`, `ExprMakeTuple`, `ExprMakeVariant`, `ExprMakeStruct`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **makeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Stack top offset for the data, if applicable

         * **extraOffset** : uint - Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` - Flags specific to make-local expressions



.. _handle-ast-ExprMakeStruct:

.. das:attribute:: ExprMakeStruct

Make structure expression (`[[YourStruct v1=expr1elem1, v2=expr2elem1, ...; v1=expr1elem2, ...  ]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` -  Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **makeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Stack top offset for the data, if applicable

         * **extraOffset** : uint - Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` - Flags specific to make-local expressions

         * **structs** : vector<MakeStruct*> - Array of structures being made

         * **_block** :  :ref:`Expression <handle-ast-Expression>`? - Optional block expression to run after the struct is made

         * **constructor** :  :ref:`Function <handle-ast-Function>`? - Constructor function to call, if any

         * **makeStructFlags** :  :ref:`ExprMakeStructFlags <alias-ExprMakeStructFlags>` - Flags specific to make-struct expressions



.. _handle-ast-ExprMakeVariant:

.. das:attribute:: ExprMakeVariant

Make variant expression (`[YourVariant variantName=expr1]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **makeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Stack top offset for the data, if applicable

         * **extraOffset** : uint - Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` - Flags specific to make-local expressions

         * **variants** : vector<MakeFieldDecl*> - Array of variants being made



.. _handle-ast-ExprMakeArray:

.. das:attribute:: ExprMakeArray

Make array expression (`[[auto 1;2;3]]` or `[{auto "foo";"bar"}]` for static and dynamic arrays accordingly).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **makeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Stack top offset for the data, if applicable

         * **extraOffset** : uint - Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` - Flags specific to make-local expressions

         * **recordType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the array elements

         * **values** : vector<Expression*> - Array of expressions for the elements

         * **gen2** : bool - If gen2 syntax is used (i.e. `[...]` instead of `[[...]]`)



.. _handle-ast-ExprMakeTuple:

.. das:attribute:: ExprMakeTuple

Make tuple expression (`[[auto f1,f2,f3]]`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **makeType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Stack top offset for the data, if applicable

         * **extraOffset** : uint - Extra offset for the data, if applicable. If part of the larger initialization, extra offset is that

         * **makeFlags** :  :ref:`ExprMakeLocalFlags <alias-ExprMakeLocalFlags>` - Flags specific to make-local expressions

         * **recordType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the array elements

         * **values** : vector<Expression*> - Array of expressions for the elements

         * **gen2** : bool - If gen2 syntax is used (i.e. `[...]` instead of `[[...]]`)

         * **isKeyValue** : bool - If key-value syntax is used (i.e. `[key=>val; key2=>val2]`)



.. _handle-ast-ExprArrayComprehension:

.. das:attribute:: ExprArrayComprehension

Array comprehension (`[for (x in 0..3); x]`, `[iterator for (y in range(100)); x*2; where (x!=13)]]` for arrays or generators accordingly).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **exprFor** :  :ref:`Expression <handle-ast-Expression>`? - The 'for' expression

         * **exprWhere** :  :ref:`Expression <handle-ast-Expression>`? - The 'where' expression

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The subexpression

         * **generatorSyntax** : bool - If generator syntax is used (i.e. `[iterator for ...]` instead of `[for]`)

         * **tableSyntax** : bool - If table syntax is used (i.e. `{for ...}` instead of `[for]`)



.. _handle-ast-TypeInfoMacro:

.. das:attribute:: TypeInfoMacro

Compilation time only structure which holds live information about typeinfo expression for the specific macro.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - The name of the macro

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - The module where the macro is defined



.. _handle-ast-ExprTypeInfo:

.. das:attribute:: ExprTypeInfo

typeinfo() expression (`typeinfo dim(a)`, `typeinfois_ref_type<int&>()`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **trait** :  :ref:`das_string <handle-builtin-das_string>` - The trait name

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The expression being queried for type information

         * **typeexpr** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - The type expression being queried for type information

         * **subtrait** :  :ref:`das_string <handle-builtin-das_string>` - The sub-trait name

         * **extratrait** :  :ref:`das_string <handle-builtin-das_string>` - The extra trait name

         * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? - The macro associated with the typeinfo expression



.. _handle-ast-ExprTypeDecl:

.. das:attribute:: ExprTypeDecl

typedecl() expression (`typedecl(1+2)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **typeexpr** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - The type expression being queried for type information



.. _handle-ast-ExprLabel:

.. das:attribute:: ExprLabel

Label (`label 13:`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **labelName** : int - The label name

         * **comment** :  :ref:`das_string <handle-builtin-das_string>` - The label comment



.. _handle-ast-ExprGoto:

.. das:attribute:: ExprGoto

Goto expression (`goto label 13`, `goto x`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **labelName** : int - Label to go to, if specified

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Expression evaluating to label to go to, if specified



.. _handle-ast-ExprRef2Value:

.. das:attribute:: ExprRef2Value

Compilation time only structure which holds reference to value conversion for the value types, i.e. goes from int& to int and such.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The sub-expression being converted from reference to value



.. _handle-ast-ExprRef2Ptr:

.. das:attribute:: ExprRef2Ptr

Addr expresion (`addr(expr)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The sub-expression being converted from pointer to reference



.. _handle-ast-ExprAddr:

.. das:attribute:: ExprAddr

Function address (`@@foobarfunc` or `@@foobarfunc<(int;int):bool>`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **target** :  :ref:`das_string <handle-builtin-das_string>` - Name of the function being referenced

         * **funcType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the function being referenced

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function being referenced (if resolved)



.. _handle-ast-ExprAssert:

.. das:attribute:: ExprAssert

Assert expression (`assert(x<13)`, or `assert(x<13, "x is too big")`, or `verify(foo()!=0)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the asserted expression

         * **arguments** : vector<Expression*> - Arguments of the assert expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the assert is used

         * **isVerify** : bool - Whether the assert is a verify expression (verify expressions have to have sideeffects, assert expressions cant)



.. _handle-ast-ExprQuote:

.. das:attribute:: ExprQuote

Compilation time expression which holds its subexpressions but does not infer them (`quote() <| x+5`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the query expression

         * **arguments** : vector<Expression*> - Arguments of the query expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the query is used



.. _handle-ast-ExprStaticAssert:

.. das:attribute:: ExprStaticAssert

Static assert expression (`static_assert(x<13)` or `static_assert(x<13, "x is too big")`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the static_assert expression

         * **arguments** : vector<Expression*> - Arguments of the static_assert expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the static_assert is used



.. _handle-ast-ExprDebug:

.. das:attribute:: ExprDebug

Debug expression (`debug(x)` or `debug(x,"x=")`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the debug expression

         * **arguments** : vector<Expression*> - Arguments of the debug expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the debug is used



.. _handle-ast-ExprInvoke:

.. das:attribute:: ExprInvoke

.. _function-ast__dot__rq_isCopyOrMove_ExprInvoke:

.. das:function:: ExprInvoke.isCopyOrMove() : bool

Returns whether the given invoke expression requires a copy or move of a reference type.


:Properties: * **isCopyOrMove** : bool

Invoke expression (`invoke(fn)` or `invoke(lamb, arg1, arg2, ...)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the invoke expression

         * **arguments** : vector<Expression*> - Arguments of the invoke expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the invoke is used

         * **stackTop** : uint - Stack top for invoke, if applicable

         * **doesNotNeedSp** : bool - Does not need stack pointer

         * **isInvokeMethod** : bool - Is invoke of class method

         * **cmresAlias** : bool - If true, then CMRES aliasing is allowed for this invoke (and stack will be allocated)



.. _handle-ast-ExprErase:

.. das:attribute:: ExprErase

Erase expression (`erase(tab,key)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the erase expression

         * **arguments** : vector<Expression*> - Arguments of the erase expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the erase is used



.. _handle-ast-ExprSetInsert:

.. das:attribute:: ExprSetInsert

Set insert expression, i.e. ``tab |> insert(key)``.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the set-insert expression

         * **arguments** : vector<Expression*> - Arguments of the set-insert expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the set-insert is used



.. _handle-ast-ExprFind:

.. das:attribute:: ExprFind

Find expression (`find(tab,key) <| { your; block; here; }`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the find expression

         * **arguments** : vector<Expression*> - Arguments of the find expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the find is used



.. _handle-ast-ExprKeyExists:

.. das:attribute:: ExprKeyExists

Key exists expression (`key_exists(tab,key)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the key-exists expression

         * **arguments** : vector<Expression*> - Arguments of the key-exists expression

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the key-exists is used



.. _handle-ast-ExprAscend:

.. das:attribute:: ExprAscend

New expression for ExprMakeLocal (`new [[Foo fld=val,...]]` or `new [[Foo() fld=...]]`, but **NOT** `new Foo()`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression being ascended (newed)

         * **ascType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type being made

         * **stackTop** : uint - Location on the stack where the temp object is created, if necessary

         * **ascendFlags** :  :ref:`ExprAscendFlags <alias-ExprAscendFlags>` - Flags specific to `ExprAscend` expressions



.. _handle-ast-ExprCast:

.. das:attribute:: ExprCast

Any cast expression (`cast<int> a`, `upcast<Foo> b` or `reinterpret<Bar?> c`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression being cast

         * **castType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type to which the expression is being cast

         * **castFlags** :  :ref:`ExprCastFlags <alias-ExprCastFlags>` - Flags specific to `ExprCast` expressions



.. _handle-ast-ExprDelete:

.. das:attribute:: ExprDelete

Delete expression (`delete blah`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The expression being deleted

         * **sizeexpr** :  :ref:`Expression <handle-ast-Expression>`? - The size expression for deleting classes. This one determines how big instance is to be deleted.

         * **native** : bool - True if the delete is native, and not to be expanded at compilation time.



.. _handle-ast-ExprVar:

.. das:attribute:: ExprVar

Variable access (`foo`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - The name of the variable

         * **variable** :  :ref:`Variable <handle-ast-Variable>`? - The variable being accessed

         * **pBlock** :  :ref:`ExprBlock <handle-ast-ExprBlock>`? - The block in which the variable is accessed (if any)

         * **argumentIndex** : int - The argument index of the variable (if variable is an argument of a function or a block)

         * **varFlags** :  :ref:`ExprVarFlags <alias-ExprVarFlags>` - The flags of the variable



.. _handle-ast-ExprTag:

.. das:attribute:: ExprTag

Compilation time only tag expression, used for reification. For example $c(....).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The subexpression of the tag

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Value of the tag

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the tag



.. _handle-ast-ExprSwizzle:

.. das:attribute:: ExprSwizzle

Vector swizzle operation (`vec.xxy` or `vec.y`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Value being swizzled

         * **mask** :  :ref:`das_string <handle-builtin-das_string>` - Swizzle mask

         * **fields** : vector<uint8> - Swizzle fields

         * **fieldFlags** :  :ref:`ExprSwizzleFieldFlags <alias-ExprSwizzleFieldFlags>` - Flags specific to `ExprSwizzle` expressions



.. _handle-ast-ExprField:

.. das:attribute:: ExprField

.. _function-ast__dot__rq_field_ExprField:

.. das:function:: ExprField.field() : FieldDeclaration?

Returns a pointer to the named field of a structure, or null if the field does not exist or the type is not a structure.


:Properties: * **field** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>`?

Field lookup (`foo.bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression whose field is being accessed

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being accessed

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field access in source code

         * **fieldIndex** : int - Index of the field in the type's field list

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> - Type annotation for the field

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` - Flags for dereferencing operations

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` - Flags specific to field access expressions



.. _handle-ast-ExprSafeField:

.. das:attribute:: ExprSafeField

Safe field lookup (`foo?.bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression whose field is being accessed

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being accessed

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field access in source code

         * **fieldIndex** : int - Index of the field in the type's field list

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> - Type annotation for the field

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` - Flags for dereferencing operations

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` - Flags specific to field access expressions

         * **skipQQ** : bool - If true the subexpression is already a pointer and no additional dereference is needed



.. _handle-ast-ExprIsVariant:

.. das:attribute:: ExprIsVariant

Is expression (`foo is bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression whose field is being accessed

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being accessed

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field access in source code

         * **fieldIndex** : int - Index of the field in the type's field list

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> - Type annotation for the field

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` - Flags for dereferencing operations

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` - Flags specific to field access expressions



.. _handle-ast-ExprAsVariant:

.. das:attribute:: ExprAsVariant

As expression (`foo as bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression whose field is being accessed

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being accessed

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field access in source code

         * **fieldIndex** : int - Index of the field in the type's field list

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> - Type annotation for the field

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` - Flags for dereferencing operations

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` - Flags specific to field access expressions



.. _handle-ast-ExprSafeAsVariant:

.. das:attribute:: ExprSafeAsVariant

Safe as expression (`foo? as bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **value** :  :ref:`Expression <handle-ast-Expression>`? - Subexpression whose field is being accessed

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the field being accessed

         * **atField** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the field access in source code

         * **fieldIndex** : int - Index of the field in the type's field list

         * **annotation** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> - Type annotation for the field

         * **derefFlags** :  :ref:`ExprFieldDerefFlags <alias-ExprFieldDerefFlags>` - Flags for dereferencing operations

         * **fieldFlags** :  :ref:`ExprFieldFieldFlags <alias-ExprFieldFieldFlags>` - Flags specific to field access expressions

         * **skipQQ** : bool - If true the subexpression is already a pointer and no additional dereference is needed



.. _handle-ast-ExprOp1:

.. das:attribute:: ExprOp1

Single operator expression (`+a` or `-a` or `!a` or `~a`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator (unused)

         * **arguments** : vector<Expression*> - Arguments of the operator (unused)

         * **argumentsFailedToInfer** : bool - Whether arguments failed to infer

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **func** :  :ref:`Function <handle-ast-Function>`? - Function associated with the expression

         * **stackTop** : uint - Stack top position if temporary variable allocation is needed

         * **op** :  :ref:`das_string <handle-builtin-das_string>` - Name of the operator

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - That one argument of the operator



.. _handle-ast-ExprReturn:

.. das:attribute:: ExprReturn

Return expression (`return` or `return foo`, or `return <- foo`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The expression being returned (if any)

         * **returnFlags** :  :ref:`ExprReturnFlags <alias-ExprReturnFlags>` - Return flags

         * **stackTop** : uint - Stack top position if temporary variable allocation is needed

         * **refStackTop** : uint - Reference stack top position if temporary variable allocation is needed

         * **returnFunc** :  :ref:`Function <handle-ast-Function>`? - Function associated with the return expression

         * **_block** :  :ref:`ExprBlock <handle-ast-ExprBlock>`? - Block associated with the return expression



.. _handle-ast-ExprYield:

.. das:attribute:: ExprYield

Yield expression (`yield foo` or `yield <- bar`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **subexpr** :  :ref:`Expression <handle-ast-Expression>`? - The expression being yielded (never empty)

         * **returnFlags** :  :ref:`ExprYieldFlags <alias-ExprYieldFlags>` - Yield flags



.. _handle-ast-ExprBreak:

.. das:attribute:: ExprBreak

Break expression (`break`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags



.. _handle-ast-ExprContinue:

.. das:attribute:: ExprContinue

Continue expression (`continue`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags



.. _handle-ast-ExprConst:

.. das:attribute:: ExprConst

Compilation time constant expression base class

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression



.. _handle-ast-ExprFakeContext:

.. das:attribute:: ExprFakeContext

Compilation time only fake context expression. Will simulate as current evaluation `Context`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::fakeContext)



.. _handle-ast-ExprFakeLineInfo:

.. das:attribute:: ExprFakeLineInfo

.. _function-ast__dot__rq_getValue_ExprFakeLineInfo:

.. das:function:: ExprFakeLineInfo.getValue() : void?

Returns the constant value stored in this expression node.


:Properties: * **getValue** : void?

Compilation time only fake lineinfo expression. Will simulate as current file and line `LineInfo`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::fakeLineInfo)

         * **value** : void? - Pointer to the LineInfo, as void?



.. _handle-ast-ExprConstPtr:

.. das:attribute:: ExprConstPtr

.. _function-ast__dot__rq_getValue_ExprConstPtr:

.. das:function:: ExprConstPtr.getValue() : void?

Returns the constant value stored in this expression node.


:Properties: * **getValue** : void?

Null (`null`). Technically can be any other pointer, but it is used for nullptr.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression

         * **value** : void? - Pointer value. Typically this is 'null' constant, so the value is zero.



.. _handle-ast-ExprConstInt8:

.. das:attribute:: ExprConstInt8

.. _function-ast__dot__rq_getValue_ExprConstInt8:

.. das:function:: ExprConstInt8.getValue() : int8

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int8

Holds int8 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt8)

         * **value** : int8 - Value of the constant expression



.. _handle-ast-ExprConstInt16:

.. das:attribute:: ExprConstInt16

.. _function-ast__dot__rq_getValue_ExprConstInt16:

.. das:function:: ExprConstInt16.getValue() : int16

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int16

Holds int16 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt16)

         * **value** : int16 - Value of the constant expression



.. _handle-ast-ExprConstInt64:

.. das:attribute:: ExprConstInt64

.. _function-ast__dot__rq_getValue_ExprConstInt64:

.. das:function:: ExprConstInt64.getValue() : int64

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int64

Holds int64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt64)

         * **value** : int64 - Value of the constant expression



.. _handle-ast-ExprConstInt:

.. das:attribute:: ExprConstInt

.. _function-ast__dot__rq_getValue_ExprConstInt:

.. das:function:: ExprConstInt.getValue() : int

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int

Holds int constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt)

         * **value** : int - Value of the constant expression



.. _handle-ast-ExprConstInt2:

.. das:attribute:: ExprConstInt2

.. _function-ast__dot__rq_getValue_ExprConstInt2:

.. das:function:: ExprConstInt2.getValue() : int2

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int2

Holds int2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt2)

         * **value** : int2 - Value of the constant expression



.. _handle-ast-ExprConstInt3:

.. das:attribute:: ExprConstInt3

.. _function-ast__dot__rq_getValue_ExprConstInt3:

.. das:function:: ExprConstInt3.getValue() : int3

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int3

Holds int3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt3)

         * **value** : int3 - Value of the constant expression



.. _handle-ast-ExprConstInt4:

.. das:attribute:: ExprConstInt4

.. _function-ast__dot__rq_getValue_ExprConstInt4:

.. das:function:: ExprConstInt4.getValue() : int4

Returns the constant value stored in this expression node.


:Properties: * **getValue** : int4

Holds int4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tInt4)

         * **value** : int4 - Value of the constant expression



.. _handle-ast-ExprConstUInt8:

.. das:attribute:: ExprConstUInt8

.. _function-ast__dot__rq_getValue_ExprConstUInt8:

.. das:function:: ExprConstUInt8.getValue() : uint8

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint8

Holds uint8 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt8)

         * **value** : uint8 - Value of the constant expression



.. _handle-ast-ExprConstUInt16:

.. das:attribute:: ExprConstUInt16

.. _function-ast__dot__rq_getValue_ExprConstUInt16:

.. das:function:: ExprConstUInt16.getValue() : uint16

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint16

Holds uint16 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt16)

         * **value** : uint16 - Value of the constant expression



.. _handle-ast-ExprConstUInt64:

.. das:attribute:: ExprConstUInt64

.. _function-ast__dot__rq_getValue_ExprConstUInt64:

.. das:function:: ExprConstUInt64.getValue() : uint64

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint64

Holds uint64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt64)

         * **value** : uint64 - Value of the constant expression



.. _handle-ast-ExprConstUInt:

.. das:attribute:: ExprConstUInt

.. _function-ast__dot__rq_getValue_ExprConstUInt:

.. das:function:: ExprConstUInt.getValue() : uint

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint

Holds uint constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt)

         * **value** : uint - Value of the constant expression



.. _handle-ast-ExprConstUInt2:

.. das:attribute:: ExprConstUInt2

.. _function-ast__dot__rq_getValue_ExprConstUInt2:

.. das:function:: ExprConstUInt2.getValue() : uint2

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint2

Holds uint2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt2)

         * **value** : uint2 - Value of the constant expression



.. _handle-ast-ExprConstUInt3:

.. das:attribute:: ExprConstUInt3

.. _function-ast__dot__rq_getValue_ExprConstUInt3:

.. das:function:: ExprConstUInt3.getValue() : uint3

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint3

Holds uint3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt3)

         * **value** : uint3 - Value of the constant expression



.. _handle-ast-ExprConstUInt4:

.. das:attribute:: ExprConstUInt4

.. _function-ast__dot__rq_getValue_ExprConstUInt4:

.. das:function:: ExprConstUInt4.getValue() : uint4

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint4

Holds uint4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tUInt4)

         * **value** : uint4 - Value of the constant expression



.. _handle-ast-ExprConstRange:

.. das:attribute:: ExprConstRange

.. _function-ast__dot__rq_getValue_ExprConstRange:

.. das:function:: ExprConstRange.getValue() : range

Returns the constant value stored in this expression node.


:Properties: * **getValue** : range

Holds range constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tRange)

         * **value** : range - Value of the constant expression



.. _handle-ast-ExprConstURange:

.. das:attribute:: ExprConstURange

.. _function-ast__dot__rq_getValue_ExprConstURange:

.. das:function:: ExprConstURange.getValue() : urange

Returns the constant value stored in this expression node.


:Properties: * **getValue** : urange

Holds urange constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tURange)

         * **value** : urange - Value of the constant expression



.. _handle-ast-ExprConstRange64:

.. das:attribute:: ExprConstRange64

.. _function-ast__dot__rq_getValue_ExprConstRange64:

.. das:function:: ExprConstRange64.getValue() : range64

Returns the constant value stored in this expression node.


:Properties: * **getValue** : range64

Holds range64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tRange64)

         * **value** : range64 - Value of the constant expression



.. _handle-ast-ExprConstURange64:

.. das:attribute:: ExprConstURange64

.. _function-ast__dot__rq_getValue_ExprConstURange64:

.. das:function:: ExprConstURange64.getValue() : urange64

Returns the constant value stored in this expression node.


:Properties: * **getValue** : urange64

Holds urange64 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tURange64)

         * **value** : urange64 - Value of the constant expression



.. _handle-ast-ExprConstFloat:

.. das:attribute:: ExprConstFloat

.. _function-ast__dot__rq_getValue_ExprConstFloat:

.. das:function:: ExprConstFloat.getValue() : float

Returns the constant value stored in this expression node.


:Properties: * **getValue** : float

Holds float constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tFloat)

         * **value** : float - Value of the constant expression



.. _handle-ast-ExprConstFloat2:

.. das:attribute:: ExprConstFloat2

.. _function-ast__dot__rq_getValue_ExprConstFloat2:

.. das:function:: ExprConstFloat2.getValue() : float2

Returns the constant value stored in this expression node.


:Properties: * **getValue** : float2

Holds float2 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tFloat2)

         * **value** : float2 - Value of the constant expression



.. _handle-ast-ExprConstFloat3:

.. das:attribute:: ExprConstFloat3

.. _function-ast__dot__rq_getValue_ExprConstFloat3:

.. das:function:: ExprConstFloat3.getValue() : float3

Returns the constant value stored in this expression node.


:Properties: * **getValue** : float3

Holds float3 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tFloat3)

         * **value** : float3 - Value of the constant expression



.. _handle-ast-ExprConstFloat4:

.. das:attribute:: ExprConstFloat4

.. _function-ast__dot__rq_getValue_ExprConstFloat4:

.. das:function:: ExprConstFloat4.getValue() : float4

Returns the constant value stored in this expression node.


:Properties: * **getValue** : float4

Holds float4 constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tFloat4)

         * **value** : float4 - Value of the constant expression



.. _handle-ast-ExprConstDouble:

.. das:attribute:: ExprConstDouble

.. _function-ast__dot__rq_getValue_ExprConstDouble:

.. das:function:: ExprConstDouble.getValue() : double

Returns the constant value stored in this expression node.


:Properties: * **getValue** : double

Holds double constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tDouble)

         * **value** : double - Value of the constant expression



.. _handle-ast-ExprConstBool:

.. das:attribute:: ExprConstBool

.. _function-ast__dot__rq_getValue_ExprConstBool:

.. das:function:: ExprConstBool.getValue() : bool

Returns the constant value stored in this expression node.


:Properties: * **getValue** : bool

Holds bool constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tBool)

         * **value** : bool - Value of the constant expression



.. _handle-ast-CaptureEntry:

.. das:attribute:: CaptureEntry

Single entry in lambda capture.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the captured variable

         * **mode** :  :ref:`CaptureMode <enum-ast-CaptureMode>` - How the variable is captured (by value, by reference, etc.)



.. _handle-ast-ExprMakeBlock:

.. das:attribute:: ExprMakeBlock

Any closure. Holds block as well as capture information in `CaptureEntry`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **_capture** : vector<CaptureEntry> - List of captured variables

         * **_block** :  :ref:`Expression <handle-ast-Expression>`? - The block expression

         * **stackTop** : uint - Stack top for the block

         * **mmFlags** :  :ref:`ExprMakeBlockFlags <alias-ExprMakeBlockFlags>` - Expression generation flags

         * **aotFunctorName** :  :ref:`das_string <handle-builtin-das_string>` - Name of the AOT functor (if applicable)



.. _handle-ast-ExprMakeGenerator:

.. das:attribute:: ExprMakeGenerator

Generator closure (`generator<int>` or `generator<Foo&>`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the AOT functor (if applicable)

         * **arguments** : vector<Expression*> - Arguments passed to the generator

         * **argumentsFailedToInfer** : bool - Whether arguments failed to infer

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure

         * **iterType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Iterator type, i.e. type of values produced by the generated iterator

         * **_capture** : vector<CaptureEntry> - List of captured variables



.. _handle-ast-ExprMemZero:

.. das:attribute:: ExprMemZero

Memzero (`memzero(expr)`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the memzero call

         * **arguments** : vector<Expression*> - Arguments of the memzero call

         * **argumentsFailedToInfer** : bool - Whether the arguments failed to infer types

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the enclosure where the memzero is used



.. _handle-ast-ExprConstEnumeration:

.. das:attribute:: ExprConstEnumeration

Holds enumeration constant, both type and entry (`Foo bar`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tEnumeration, Type::tEnumeration8, Type::tEnumeration16, or Type::tEnumeration64)

         * **enumType** :  :ref:`Enumeration <handle-ast-Enumeration>`? - Enumeration type declaration

         * **value** :  :ref:`das_string <handle-builtin-das_string>` - Value of the constant expression



.. _handle-ast-ExprConstBitfield:

.. das:attribute:: ExprConstBitfield

.. _function-ast__dot__rq_getValue_ExprConstBitfield:

.. das:function:: ExprConstBitfield.getValue() : uint64

Returns the constant value stored in this expression node.


:Properties: * **getValue** : uint64

Holds bitfield constant (`Foo bar`).

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tBitfield, Type::tBitfield8, Type::tBitfield16, or Type::tBitfield64)

         * **value** : bitfield<> - Value of the constant expression

         * **bitfieldType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type declaration of the bitfield



.. _handle-ast-ExprConstString:

.. das:attribute:: ExprConstString

Holds string constant.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **baseType** :  :ref:`Type <enum-rtti-Type>` - Base type of the constant expression (Type::tString)

         * **value** :  :ref:`das_string <handle-builtin-das_string>` - Value of the constant expression



.. _handle-ast-ExprUnsafe:

.. das:attribute:: ExprUnsafe

Unsafe expression (`unsafe(addr(x))`)

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **body** :  :ref:`Expression <handle-ast-Expression>`? - Body expression that is marked as unsafe



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

Adapter for the `AstEnumerationAnnotation`.



.. _handle-ast-PassMacro:

.. das:attribute:: PassMacro

Adapter for the `AstPassMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro



.. _handle-ast-ReaderMacro:

.. das:attribute:: ReaderMacro

Adapter for the `AstReaderMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module where the macro is defined



.. _handle-ast-CommentReader:

.. das:attribute:: CommentReader

 Adapter for the `AstCommentReader`.



.. _handle-ast-CallMacro:

.. das:attribute:: CallMacro

Adapter for the `AstCallMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro

         * **_module** :  :ref:`Module <handle-rtti-Module>`? - Module where the macro is defined



.. _handle-ast-VariantMacro:

.. das:attribute:: VariantMacro

Adapter for the `AstVariantMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro



.. _handle-ast-ForLoopMacro:

.. das:attribute:: ForLoopMacro

Adapter for the 'AstForLoopMacro'.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro



.. _handle-ast-CaptureMacro:

.. das:attribute:: CaptureMacro

Adapter for the `AstCaptureMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro



.. _handle-ast-TypeMacro:

.. das:attribute:: TypeMacro

Compilation time only structure which holds live information about type macro.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro



.. _handle-ast-SimulateMacro:

.. das:attribute:: SimulateMacro

Adapter for the `AstSimulateMacro`.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>` - Name of the macro.



.. _handle-ast-ExprReader:

.. das:attribute:: ExprReader

Compilation time only expression which holds temporary information for the `AstReaderMacro`.

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Location of the expression in source code

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Type of the expression

         * **__rtti** : string - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Expression generation flags

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression print flags

         * **macro** :  :ref:`ReaderMacro <handle-ast-ReaderMacro>`? - Macro which is attached to the context parser.

         * **sequence** :  :ref:`das_string <handle-builtin-das_string>` - Sequence of characters being read.



.. _handle-ast-ExprCallMacro:

.. das:attribute:: ExprCallMacro

:Fields: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` - Compilation time only expression which holds temporary information for the `AstCallMacro`.

         * **_type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? - Location of the expression in source code

         * **__rtti** : string - Type of the expression

         * **genFlags** :  :ref:`ExprGenFlags <alias-ExprGenFlags>` - Runtime type information of the class of the expression (i.e "ExprConstant", "ExprCall", etc)

         * **flags** :  :ref:`ExprFlags <alias-ExprFlags>` - Expression generation flags

         * **printFlags** :  :ref:`ExprPrintFlags <alias-ExprPrintFlags>` - Expression flags

         * **name** :  :ref:`das_string <handle-builtin-das_string>` - Expression print flags

         * **arguments** : vector<Expression*> - Name of the macro being called

         * **argumentsFailedToInfer** : bool - List of argument expressions

         * **atEnclosure** :  :ref:`LineInfo <handle-rtti-LineInfo>` - If the arguments failed to infer their types

         * **inFunction** :  :ref:`Function <handle-ast-Function>`? - Location of the expression in source code

         * **macro** :  :ref:`CallMacro <handle-ast-CallMacro>`? - Call macro, if resolved




+++++++++++
Call macros
+++++++++++

.. _call-macro-ast-quote:

.. das:attribute:: quote

Returns the AST expression tree of the provided code without evaluating or type-inferring it. Used in macro programming to capture source code as a manipulable AST.


+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-ast-ast_typedecl:

.. das:attribute:: ast_typedecl

Returns a `TypeDeclPtr` for the type specified via `type<>` or subexpression type, for example `typeinfo ast_typedecl(type<int?>)`. Useful in macros that need compile-time access to type declarations.

.. _call-macro-ast-ast_function:

.. das:attribute:: ast_function

Returns a `FunctionPtr` to the function specified by the subexpression, for example ``typeinfo ast_function(@@foo)``. Useful in macros that need compile-time access to function declarations.


+++++++++++++
Handled types
+++++++++++++

.. _handle-ast-MakeStruct:

.. das:attribute:: MakeStruct

Annotation representing a vector of `MakeFieldDecl` used to initialize fields in `ExprMakeStruct` expressions.


+++++++
Classes
+++++++

.. _struct-ast-AstFunctionAnnotation:

.. das:attribute:: AstFunctionAnnotation

class AstFunctionAnnotation

.. _struct-ast-AstBlockAnnotation:

.. das:attribute:: AstBlockAnnotation

class AstBlockAnnotation

.. _struct-ast-AstStructureAnnotation:

.. das:attribute:: AstStructureAnnotation

class AstStructureAnnotation

.. _struct-ast-AstPassMacro:

.. das:attribute:: AstPassMacro

class AstPassMacro

.. _struct-ast-AstVariantMacro:

.. das:attribute:: AstVariantMacro

class AstVariantMacro

.. _struct-ast-AstForLoopMacro:

.. das:attribute:: AstForLoopMacro

class AstForLoopMacro

.. _struct-ast-AstCaptureMacro:

.. das:attribute:: AstCaptureMacro

class AstCaptureMacro

.. _struct-ast-AstTypeMacro:

.. das:attribute:: AstTypeMacro

class AstTypeMacro

.. _struct-ast-AstSimulateMacro:

.. das:attribute:: AstSimulateMacro

class AstSimulateMacro

.. _struct-ast-AstReaderMacro:

.. das:attribute:: AstReaderMacro

class AstReaderMacro

.. _struct-ast-AstCommentReader:

.. das:attribute:: AstCommentReader

class AstCommentReader

.. _struct-ast-AstCallMacro:

.. das:attribute:: AstCallMacro

class AstCallMacro

.. _struct-ast-AstTypeInfoMacro:

.. das:attribute:: AstTypeInfoMacro

class AstTypeInfoMacro

.. _struct-ast-AstEnumerationAnnotation:

.. das:attribute:: AstEnumerationAnnotation

class AstEnumerationAnnotation

.. _struct-ast-AstVisitor:

.. das:attribute:: AstVisitor

class AstVisitor


+++++++++++++++
Call generation
+++++++++++++++

  *  :ref:`make_call (at: LineInfo; name: string) : Expression? <function-ast_make_call_LineInfo_string>`

.. _function-ast_make_call_LineInfo_string:

.. das:function:: make_call(at: LineInfo; name: string) : Expression?

Creates the appropriate call expression for a given function name in the program.


:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

            * **name** : string implicit


+++++++++++++++
Visitor pattern
+++++++++++++++

  *  :ref:`visit (program: smart_ptr\<Program\>; adapter: VisitorAdapter?; sortStructures: bool) <function-ast_visit_smart_ptr_ls_Program_gr__VisitorAdapter_q__bool>`
  *  :ref:`visit (program: smart_ptr\<Program\>; adapter: VisitorAdapter?) <function-ast_visit_smart_ptr_ls_Program_gr__VisitorAdapter_q_>`
  *  :ref:`visit (expression: Expression?; adapter: VisitorAdapter?) : Expression? <function-ast_visit_Expression_q__VisitorAdapter_q_>`
  *  :ref:`visit (function: Function?; adapter: VisitorAdapter?) <function-ast_visit_Function_q__VisitorAdapter_q_>`
  *  :ref:`visit (expression: TypeDecl?; adapter: VisitorAdapter?) : TypeDecl? <function-ast_visit_TypeDecl_q__VisitorAdapter_q_>`
  *  :ref:`visit_enumeration (program: smart_ptr\<Program\>; enumeration: Enumeration?; adapter: VisitorAdapter?) <function-ast_visit_enumeration_smart_ptr_ls_Program_gr__Enumeration_q__VisitorAdapter_q_>`
  *  :ref:`visit_finally (expression: ExprBlock?; adapter: VisitorAdapter?) <function-ast_visit_finally_ExprBlock_q__VisitorAdapter_q_>`
  *  :ref:`visit_module (program: smart_ptr\<Program\>; adapter: VisitorAdapter?; module: Module?) <function-ast_visit_module_smart_ptr_ls_Program_gr__VisitorAdapter_q__Module_q_>`
  *  :ref:`visit_modules (program: smart_ptr\<Program\>; adapter: VisitorAdapter?) <function-ast_visit_modules_smart_ptr_ls_Program_gr__VisitorAdapter_q_>`
  *  :ref:`visit_structure (program: smart_ptr\<Program\>; structure: Structure?; adapter: VisitorAdapter?) <function-ast_visit_structure_smart_ptr_ls_Program_gr__Structure_q__VisitorAdapter_q_>`
  *  :ref:`visit_with_generics (program: smart_ptr\<Program\>; adapter: VisitorAdapter?) <function-ast_visit_with_generics_smart_ptr_ls_Program_gr__VisitorAdapter_q_>`


visit
^^^^^

.. _function-ast_visit_smart_ptr_ls_Program_gr__VisitorAdapter_q__bool:

.. das:function:: visit(program: smart_ptr<Program>; adapter: VisitorAdapter?; sortStructures: bool)

Visit the program with the given visitor adapter. When sortStructures is true, struct declarations are visited in topological (dependency) order, ensuring that structs used by value in fields are visited before the structs that contain them.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

            * **sortStructures** : bool

.. _function-ast_visit_smart_ptr_ls_Program_gr__VisitorAdapter_q_:

.. das:function:: visit(program: smart_ptr<Program>; adapter: VisitorAdapter?)

.. _function-ast_visit_Expression_q__VisitorAdapter_q_:

.. das:function:: visit(expression: Expression?; adapter: VisitorAdapter?) : Expression?

.. _function-ast_visit_Function_q__VisitorAdapter_q_:

.. das:function:: visit(function: Function?; adapter: VisitorAdapter?)

.. _function-ast_visit_TypeDecl_q__VisitorAdapter_q_:

.. das:function:: visit(expression: TypeDecl?; adapter: VisitorAdapter?) : TypeDecl?

----

.. _function-ast_visit_enumeration_smart_ptr_ls_Program_gr__Enumeration_q__VisitorAdapter_q_:

.. das:function:: visit_enumeration(program: smart_ptr<Program>; enumeration: Enumeration?; adapter: VisitorAdapter?)

Applies the given visitor adapter to the specified enumeration within the context of the program, triggering the appropriate visitor callbacks.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **enumeration** :  :ref:`Enumeration <handle-ast-Enumeration>`? implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

.. _function-ast_visit_finally_ExprBlock_q__VisitorAdapter_q_:

.. das:function:: visit_finally(expression: ExprBlock?; adapter: VisitorAdapter?)

Invokes the visitor on the finally section of a block.


:Arguments: * **expression** :  :ref:`ExprBlock <handle-ast-ExprBlock>`? implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

.. _function-ast_visit_module_smart_ptr_ls_Program_gr__VisitorAdapter_q__Module_q_:

.. das:function:: visit_module(program: smart_ptr<Program>; adapter: VisitorAdapter?; module: Module?)

Invokes an AST visitor on the given module.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

            * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

.. _function-ast_visit_modules_smart_ptr_ls_Program_gr__VisitorAdapter_q_:

.. das:function:: visit_modules(program: smart_ptr<Program>; adapter: VisitorAdapter?)

Invokes an AST visitor on all modules in the specified program.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

.. _function-ast_visit_structure_smart_ptr_ls_Program_gr__Structure_q__VisitorAdapter_q_:

.. das:function:: visit_structure(program: smart_ptr<Program>; structure: Structure?; adapter: VisitorAdapter?)

Applies the given visitor adapter to the specified structure within the context of the program, triggering the appropriate visitor callbacks.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit

.. _function-ast_visit_with_generics_smart_ptr_ls_Program_gr__VisitorAdapter_q_:

.. das:function:: visit_with_generics(program: smart_ptr<Program>; adapter: VisitorAdapter?)

Visits the program AST including generic function instantiations. Unlike visit, which only walks non-generic functions, this also visits each instantiated generic, making it suitable for lint passes that need to check generated code.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **adapter** :  :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`? implicit


+++++++++++++++++++++
Expression generation
+++++++++++++++++++++

  *  :ref:`force_generated (function: Function?; value: bool) <function-ast_force_generated_Function_q__bool>`
  *  :ref:`force_generated (expression: Expression?; value: bool) <function-ast_force_generated_Expression_q__bool>`
  *  :ref:`get_expression_annotation (expr: Expression?) : Annotation? <function-ast_get_expression_annotation_Expression_q_>`
  *  :ref:`make_type_info_structure (ctx: Context; type: TypeDecl?) : TypeInfo? <function-ast_make_type_info_structure_Context_TypeDecl_q_>`


force_generated
^^^^^^^^^^^^^^^

.. _function-ast_force_generated_Function_q__bool:

.. das:function:: force_generated(function: Function?; value: bool)

Sets or clears the generated flag on the specified function and all expressions within its body. Generated functions are treated as compiler-produced rather than user-written.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **value** : bool

.. _function-ast_force_generated_Expression_q__bool:

.. das:function:: force_generated(expression: Expression?; value: bool)

----

.. _function-ast_get_expression_annotation_Expression_q_:

.. das:function:: get_expression_annotation(expr: Expression?) : Annotation?

Returns the Annotation associated with an Expression or its inherited types.


:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_make_type_info_structure_Context_TypeDecl_q_:

.. das:function:: make_type_info_structure(ctx: Context; type: TypeDecl?) : TypeInfo?

Returns a new TypeInfo corresponding to the specified type.


:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>` implicit

            * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit


++++++++++++++++++
Adapter generation
++++++++++++++++++

  *  :ref:`make_block_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr <function-ast_make_block_annotation_string_auto_0x29a>`
  *  :ref:`make_block_annotation (name: string; class: void?; info: StructInfo const?) : smart_ptr\<FunctionAnnotation\> <function-ast_make_block_annotation_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_block_type (blk: ExprBlock?) : TypeDecl? <function-ast_make_block_type_ExprBlock_q_>`
  *  :ref:`make_call_macro (name: string; class: void?; info: StructInfo const?) : CallMacro? <function-ast_make_call_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_call_macro (name: string; var someClassPtr: auto) : CallMacroPtr <function-ast_make_call_macro_string_auto_0x2ec>`
  *  :ref:`make_capture_macro (name: string; class: void?; info: StructInfo const?) : CaptureMacro? <function-ast_make_capture_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_capture_macro (name: string; var someClassPtr: auto) : CaptureMacroPtr <function-ast_make_capture_macro_string_auto_0x332>`
  *  :ref:`make_clone_structure (structure: Structure?) : Function? <function-ast_make_clone_structure_Structure_q_>`
  *  :ref:`make_comment_reader (class: void?; info: StructInfo const?) : CommentReader? <function-ast_make_comment_reader_void_q__StructInfo_const_q_>`
  *  :ref:`make_comment_reader (name: string; var someClassPtr: auto) : CommentReaderPtr <function-ast_make_comment_reader_string_auto_0x2de>`
  *  :ref:`make_enum_debug_info (helper: smart_ptr\<DebugInfoHelper\>; en: Enumeration const?) : EnumInfo? <function-ast_make_enum_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Enumeration_const_q_>`
  *  :ref:`make_enumeration_annotation (name: string; var someClassPtr: auto) : EnumerationAnnotationPtr <function-ast_make_enumeration_annotation_string_auto_0x2b6>`
  *  :ref:`make_enumeration_annotation (name: string; class: void?; info: StructInfo const?) : smart_ptr\<EnumerationAnnotation\> <function-ast_make_enumeration_annotation_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_for_loop_macro (name: string; class: void?; info: StructInfo const?) : ForLoopMacro? <function-ast_make_for_loop_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_for_loop_macro (name: string; var someClassPtr: auto) : ForLoopMacroPtr <function-ast_make_for_loop_macro_string_auto_0x324>`
  *  :ref:`make_function_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr <function-ast_make_function_annotation_string_auto_0x28c>`
  *  :ref:`make_function_annotation (name: string; class: void?; info: StructInfo const?) : smart_ptr\<FunctionAnnotation\> <function-ast_make_function_annotation_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_function_debug_info (helper: smart_ptr\<DebugInfoHelper\>; fn: Function const?) : FuncInfo? <function-ast_make_function_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Function_const_q_>`
  *  :ref:`make_invokable_type_debug_info (helper: smart_ptr\<DebugInfoHelper\>; blk: TypeDecl?; at: LineInfo) : FuncInfo? <function-ast_make_invokable_type_debug_info_smart_ptr_ls_DebugInfoHelper_gr__TypeDecl_q__LineInfo>`
  *  :ref:`make_pass_macro (name: string; class: void?; info: StructInfo const?) : PassMacro? <function-ast_make_pass_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_pass_macro (name: string; var someClassPtr: auto) : PassMacroPtr <function-ast_make_pass_macro_string_auto_0x308>`
  *  :ref:`make_reader_macro (name: string; var someClassPtr: auto) : ReaderMacroPtr <function-ast_make_reader_macro_string_auto_0x2d0>`
  *  :ref:`make_reader_macro (name: string; class: void?; info: StructInfo const?) : ReaderMacro? <function-ast_make_reader_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_simulate_macro (name: string; var someClassPtr: auto) : SimulateMacroPtr <function-ast_make_simulate_macro_string_auto_0x34b>`
  *  :ref:`make_simulate_macro (name: string; class: void?; info: StructInfo const?) : SimulateMacro? <function-ast_make_simulate_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_struct_debug_info (helper: smart_ptr\<DebugInfoHelper\>; st: Structure const?) : StructInfo? <function-ast_make_struct_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Structure_const_q_>`
  *  :ref:`make_struct_variable_debug_info (helper: smart_ptr\<DebugInfoHelper\>; st: Structure const?; var: FieldDeclaration const?) : VarInfo? <function-ast_make_struct_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Structure_const_q__FieldDeclaration_const_q_>`
  *  :ref:`make_structure_annotation (name: string; var someClassPtr: auto) : StructureAnnotationPtr <function-ast_make_structure_annotation_string_auto_0x2a8>`
  *  :ref:`make_structure_annotation (name: string; class: void?; info: StructInfo const?) : smart_ptr\<StructureAnnotation\> <function-ast_make_structure_annotation_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_type_info (helper: smart_ptr\<DebugInfoHelper\>; info: TypeInfo?; type: TypeDecl? const&) : TypeInfo? <function-ast_make_type_info_smart_ptr_ls_DebugInfoHelper_gr__TypeInfo_q__TypeDecl_q__const_ref_>`
  *  :ref:`make_type_macro (name: string; var someClassPtr: auto) : TypeMacroPtr <function-ast_make_type_macro_string_auto_0x33e>`
  *  :ref:`make_type_macro (name: string; class: void?; info: StructInfo const?) : TypeMacro? <function-ast_make_type_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_typeinfo_macro (name: string; class: void?; info: StructInfo const?) : TypeInfoMacro? <function-ast_make_typeinfo_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_typeinfo_macro (name: string; var someClassPtr: auto) : TypeInfoMacroPtr <function-ast_make_typeinfo_macro_string_auto_0x2fa>`
  *  :ref:`make_variable_debug_info (helper: smart_ptr\<DebugInfoHelper\>; var: Variable?) : VarInfo? <function-ast_make_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Variable_q_>`
  *  :ref:`make_variant_macro (name: string; var someClassPtr: auto) : VariantMacroPtr <function-ast_make_variant_macro_string_auto_0x316>`
  *  :ref:`make_variant_macro (name: string; class: void?; info: StructInfo const?) : VariantMacro? <function-ast_make_variant_macro_string_void_q__StructInfo_const_q_>`
  *  :ref:`make_visitor (someClass: auto; blk: block\<(var adapter:VisitorAdapter?):void\>) : auto <function-ast_make_visitor_auto_block_ls_var_adapter_c_VisitorAdapter_q__c_void_gr__0x2c1>`
  *  :ref:`make_visitor (class: void?; info: StructInfo const?; blk: block\<(VisitorAdapter?):void\>) <function-ast_make_visitor_void_q__StructInfo_const_q__block_ls_VisitorAdapter_q__c_void_gr_>`


make_block_annotation
^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_block_annotation_string_auto_0x29a:

.. das:function:: make_block_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

def make_block_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_block_annotation_string_void_q__StructInfo_const_q_:

.. das:function:: make_block_annotation(name: string; class: void?; info: StructInfo const?) : smart_ptr<FunctionAnnotation>

----

.. _function-ast_make_block_type_ExprBlock_q_:

.. das:function:: make_block_type(blk: ExprBlock?) : TypeDecl?

Generates a TypeDeclPtr for a specified block or lambda type.


:Arguments: * **blk** :  :ref:`ExprBlock <handle-ast-ExprBlock>`? implicit


make_call_macro
^^^^^^^^^^^^^^^

.. _function-ast_make_call_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_call_macro(name: string; class: void?; info: StructInfo const?) : CallMacro?

Creates an adapter for the AstCallMacro interface.


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_call_macro_string_auto_0x2ec:

.. das:function:: make_call_macro(name: string; someClassPtr: auto) : CallMacroPtr

----


make_capture_macro
^^^^^^^^^^^^^^^^^^

.. _function-ast_make_capture_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_capture_macro(name: string; class: void?; info: StructInfo const?) : CaptureMacro?

Creates an adapter for the AstCaptureMacro interface.


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_capture_macro_string_auto_0x332:

.. das:function:: make_capture_macro(name: string; someClassPtr: auto) : CaptureMacroPtr

----

.. _function-ast_make_clone_structure_Structure_q_:

.. das:function:: make_clone_structure(structure: Structure?) : Function?

Generates a clone function for the given structure.


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit


make_comment_reader
^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_comment_reader_void_q__StructInfo_const_q_:

.. das:function:: make_comment_reader(class: void?; info: StructInfo const?) : CommentReader?

Creates an adapter for the AstCommentReader interface.


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_comment_reader_string_auto_0x2de:

.. das:function:: make_comment_reader(name: string; someClassPtr: auto) : CommentReaderPtr

----

.. _function-ast_make_enum_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Enumeration_const_q_:

.. das:function:: make_enum_debug_info(helper: smart_ptr<DebugInfoHelper>; en: Enumeration const?) : EnumInfo?

Generates an EnumInfo for the specified enumeration using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **en** :  :ref:`Enumeration <handle-ast-Enumeration>`? implicit


make_enumeration_annotation
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_enumeration_annotation_string_auto_0x2b6:

.. das:function:: make_enumeration_annotation(name: string; someClassPtr: auto) : EnumerationAnnotationPtr

def make_enumeration_annotation (name: string; var someClassPtr: auto) : EnumerationAnnotationPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_enumeration_annotation_string_void_q__StructInfo_const_q_:

.. das:function:: make_enumeration_annotation(name: string; class: void?; info: StructInfo const?) : smart_ptr<EnumerationAnnotation>

----


make_for_loop_macro
^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_for_loop_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_for_loop_macro(name: string; class: void?; info: StructInfo const?) : ForLoopMacro?

Creates an adapter for the AstForLoopMacro interface.


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_for_loop_macro_string_auto_0x324:

.. das:function:: make_for_loop_macro(name: string; someClassPtr: auto) : ForLoopMacroPtr

----


make_function_annotation
^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_function_annotation_string_auto_0x28c:

.. das:function:: make_function_annotation(name: string; someClassPtr: auto) : FunctionAnnotationPtr

def make_function_annotation (name: string; var someClassPtr: auto) : FunctionAnnotationPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_function_annotation_string_void_q__StructInfo_const_q_:

.. das:function:: make_function_annotation(name: string; class: void?; info: StructInfo const?) : smart_ptr<FunctionAnnotation>

----

.. _function-ast_make_function_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Function_const_q_:

.. das:function:: make_function_debug_info(helper: smart_ptr<DebugInfoHelper>; fn: Function const?) : FuncInfo?

Generates a FuncInfo for the specified function using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **fn** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_make_invokable_type_debug_info_smart_ptr_ls_DebugInfoHelper_gr__TypeDecl_q__LineInfo:

.. das:function:: make_invokable_type_debug_info(helper: smart_ptr<DebugInfoHelper>; blk: TypeDecl?; at: LineInfo) : FuncInfo?

Generates a FuncInfo for an invokable type such as a lambda or block using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit


make_pass_macro
^^^^^^^^^^^^^^^

.. _function-ast_make_pass_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_pass_macro(name: string; class: void?; info: StructInfo const?) : PassMacro?

Creates an adapter for the AstPassMacro interface.


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_pass_macro_string_auto_0x308:

.. das:function:: make_pass_macro(name: string; someClassPtr: auto) : PassMacroPtr

----


make_reader_macro
^^^^^^^^^^^^^^^^^

.. _function-ast_make_reader_macro_string_auto_0x2d0:

.. das:function:: make_reader_macro(name: string; someClassPtr: auto) : ReaderMacroPtr

def make_reader_macro (name: string; var someClassPtr: auto) : ReaderMacroPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_reader_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_reader_macro(name: string; class: void?; info: StructInfo const?) : ReaderMacro?

----


make_simulate_macro
^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_simulate_macro_string_auto_0x34b:

.. das:function:: make_simulate_macro(name: string; someClassPtr: auto) : SimulateMacroPtr

def make_simulate_macro (name: string; var someClassPtr: auto) : SimulateMacroPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_simulate_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_simulate_macro(name: string; class: void?; info: StructInfo const?) : SimulateMacro?

----

.. _function-ast_make_struct_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Structure_const_q_:

.. das:function:: make_struct_debug_info(helper: smart_ptr<DebugInfoHelper>; st: Structure const?) : StructInfo?

Generates a StructInfo for the specified structure using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>`? implicit

.. _function-ast_make_struct_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Structure_const_q__FieldDeclaration_const_q_:

.. das:function:: make_struct_variable_debug_info(helper: smart_ptr<DebugInfoHelper>; st: Structure const?; var: FieldDeclaration const?) : VarInfo?

Generates a VariableInfo for a structure field using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **var** :  :ref:`FieldDeclaration <handle-ast-FieldDeclaration>`? implicit


make_structure_annotation
^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_structure_annotation_string_auto_0x2a8:

.. das:function:: make_structure_annotation(name: string; someClassPtr: auto) : StructureAnnotationPtr

def make_structure_annotation (name: string; var someClassPtr: auto) : StructureAnnotationPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_structure_annotation_string_void_q__StructInfo_const_q_:

.. das:function:: make_structure_annotation(name: string; class: void?; info: StructInfo const?) : smart_ptr<StructureAnnotation>

----

.. _function-ast_make_type_info_smart_ptr_ls_DebugInfoHelper_gr__TypeInfo_q__TypeDecl_q__const_ref_:

.. das:function:: make_type_info(helper: smart_ptr<DebugInfoHelper>; info: TypeInfo?; type: TypeDecl? const&) : TypeInfo?

Generates a TypeInfo for the specified type using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>`? implicit

            * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`?\ & implicit


make_type_macro
^^^^^^^^^^^^^^^

.. _function-ast_make_type_macro_string_auto_0x33e:

.. das:function:: make_type_macro(name: string; someClassPtr: auto) : TypeMacroPtr

def make_type_macro (name: string; var someClassPtr: auto) : TypeMacroPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_type_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_type_macro(name: string; class: void?; info: StructInfo const?) : TypeMacro?

----


make_typeinfo_macro
^^^^^^^^^^^^^^^^^^^

.. _function-ast_make_typeinfo_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_typeinfo_macro(name: string; class: void?; info: StructInfo const?) : TypeInfoMacro?

Creates an adapter for the AstTypeInfoMacro interface.


:Arguments: * **name** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_make_typeinfo_macro_string_auto_0x2fa:

.. das:function:: make_typeinfo_macro(name: string; someClassPtr: auto) : TypeInfoMacroPtr

----

.. _function-ast_make_variable_debug_info_smart_ptr_ls_DebugInfoHelper_gr__Variable_q_:

.. das:function:: make_variable_debug_info(helper: smart_ptr<DebugInfoHelper>; var: Variable?) : VarInfo?

Generates a VariableInfo for the specified variable using the given DebugInfoHelper.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **var** :  :ref:`Variable <handle-ast-Variable>`? implicit


make_variant_macro
^^^^^^^^^^^^^^^^^^

.. _function-ast_make_variant_macro_string_auto_0x316:

.. das:function:: make_variant_macro(name: string; someClassPtr: auto) : VariantMacroPtr

def make_variant_macro (name: string; var someClassPtr: auto) : VariantMacroPtr

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_make_variant_macro_string_void_q__StructInfo_const_q_:

.. das:function:: make_variant_macro(name: string; class: void?; info: StructInfo const?) : VariantMacro?

----


make_visitor
^^^^^^^^^^^^

.. _function-ast_make_visitor_auto_block_ls_var_adapter_c_VisitorAdapter_q__c_void_gr__0x2c1:

.. das:function:: make_visitor(someClass: auto; blk: block<(var adapter:VisitorAdapter?):void>) : auto

def make_visitor (someClass: auto; blk: block<(var adapter:VisitorAdapter?):void>) : auto

:Arguments: * **someClass** : auto

            * **blk** : block<(adapter: :ref:`VisitorAdapter <handle-ast-VisitorAdapter>`?):void>

.. _function-ast_make_visitor_void_q__StructInfo_const_q__block_ls_VisitorAdapter_q__c_void_gr_:

.. das:function:: make_visitor(class: void?; info: StructInfo const?; blk: block<(VisitorAdapter?):void>)


+++++++++++++++++++
Adapter application
+++++++++++++++++++

  *  :ref:`add_block_annotation (block: ExprBlock?; annotation: smart_ptr\<FunctionAnnotation\>&) <function-ast_add_block_annotation_ExprBlock_q__smart_ptr_ls_FunctionAnnotation_gr__ref_>`
  *  :ref:`add_block_annotation (block: ExprBlock?; annotation: smart_ptr\<AnnotationDeclaration\>&) <function-ast_add_block_annotation_ExprBlock_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_>`
  *  :ref:`add_call_macro (module: Module?; annotation: CallMacro?) <function-ast_add_call_macro_Module_q__CallMacro_q_>`
  *  :ref:`add_capture_macro (module: Module?; annotation: CaptureMacro?) <function-ast_add_capture_macro_Module_q__CaptureMacro_q_>`
  *  :ref:`add_comment_reader (module: Module?; reader: CommentReader?) <function-ast_add_comment_reader_Module_q__CommentReader_q_>`
  *  :ref:`add_dirty_infer_macro (module: Module?; annotation: PassMacro?) <function-ast_add_dirty_infer_macro_Module_q__PassMacro_q_>`
  *  :ref:`add_enumeration_annotation (module: Module?; annotation: smart_ptr\<EnumerationAnnotation\>&) <function-ast_add_enumeration_annotation_Module_q__smart_ptr_ls_EnumerationAnnotation_gr__ref_>`
  *  :ref:`add_for_loop_macro (module: Module?; annotation: ForLoopMacro?) <function-ast_add_for_loop_macro_Module_q__ForLoopMacro_q_>`
  *  :ref:`add_function_annotation (function: Function?; annotation: smart_ptr\<FunctionAnnotation\>&) <function-ast_add_function_annotation_Function_q__smart_ptr_ls_FunctionAnnotation_gr__ref_>`
  *  :ref:`add_function_annotation (module: Module?; annotation: smart_ptr\<FunctionAnnotation\>&) <function-ast_add_function_annotation_Module_q__smart_ptr_ls_FunctionAnnotation_gr__ref_>`
  *  :ref:`add_function_annotation (function: Function?; annotation: smart_ptr\<AnnotationDeclaration\>&) <function-ast_add_function_annotation_Function_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_>`
  *  :ref:`add_global_lint_macro (module: Module?; annotation: PassMacro?) <function-ast_add_global_lint_macro_Module_q__PassMacro_q_>`
  *  :ref:`add_infer_macro (module: Module?; annotation: PassMacro?) <function-ast_add_infer_macro_Module_q__PassMacro_q_>`
  *  :ref:`add_lint_macro (module: Module?; annotation: PassMacro?) <function-ast_add_lint_macro_Module_q__PassMacro_q_>`
  *  :ref:`add_module_option (module: Module?; option: string; type: Type) <function-ast_add_module_option_Module_q__string_Type>`
  *  :ref:`add_new_block_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_block_annotation_string_auto_0x357>`
  *  :ref:`add_new_call_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_call_macro_string_auto_0x39e>`
  *  :ref:`add_new_capture_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_capture_macro_string_auto_0x381>`
  *  :ref:`add_new_comment_reader (name: string; var someClassPtr: auto) : auto <function-ast_add_new_comment_reader_string_auto_0x398>`
  *  :ref:`add_new_contract_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_contract_annotation_string_auto_0x363>`
  *  :ref:`add_new_dirty_infer_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_dirty_infer_macro_string_auto_0x3b0>`
  *  :ref:`add_new_enumeration_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_enumeration_annotation_string_auto_0x36f>`
  *  :ref:`add_new_for_loop_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_for_loop_macro_string_auto_0x37b>`
  *  :ref:`add_new_function_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_function_annotation_string_auto_0x35d>`
  *  :ref:`add_new_global_lint_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_global_lint_macro_string_auto_0x3bc>`
  *  :ref:`add_new_infer_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_infer_macro_string_auto_0x3aa>`
  *  :ref:`add_new_lint_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_lint_macro_string_auto_0x3b6>`
  *  :ref:`add_new_optimization_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_optimization_macro_string_auto_0x3c2>`
  *  :ref:`add_new_reader_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_reader_macro_string_auto_0x393>`
  *  :ref:`add_new_simulate_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_simulate_macro_string_auto_0x38d>`
  *  :ref:`add_new_structure_annotation (name: string; var someClassPtr: auto) : auto <function-ast_add_new_structure_annotation_string_auto_0x369>`
  *  :ref:`add_new_type_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_type_macro_string_auto_0x387>`
  *  :ref:`add_new_typeinfo_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_typeinfo_macro_string_auto_0x3a4>`
  *  :ref:`add_new_variant_macro (name: string; var someClassPtr: auto) : auto <function-ast_add_new_variant_macro_string_auto_0x375>`
  *  :ref:`add_optimization_macro (module: Module?; annotation: PassMacro?) <function-ast_add_optimization_macro_Module_q__PassMacro_q_>`
  *  :ref:`add_reader_macro (module: Module?; annotation: ReaderMacro?) <function-ast_add_reader_macro_Module_q__ReaderMacro_q_>`
  *  :ref:`add_simulate_macro (module: Module?; annotation: SimulateMacro?) <function-ast_add_simulate_macro_Module_q__SimulateMacro_q_>`
  *  :ref:`add_structure_annotation (structure: Structure?; annotation: smart_ptr\<StructureAnnotation\>&) <function-ast_add_structure_annotation_Structure_q__smart_ptr_ls_StructureAnnotation_gr__ref_>`
  *  :ref:`add_structure_annotation (module: Module?; annotation: smart_ptr\<StructureAnnotation\>&) <function-ast_add_structure_annotation_Module_q__smart_ptr_ls_StructureAnnotation_gr__ref_>`
  *  :ref:`add_structure_annotation (structure: Structure?; annotation: smart_ptr\<AnnotationDeclaration\>&) <function-ast_add_structure_annotation_Structure_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_>`
  *  :ref:`add_type_macro (module: Module?; annotation: TypeMacro?) <function-ast_add_type_macro_Module_q__TypeMacro_q_>`
  *  :ref:`add_typeinfo_macro (module: Module?; annotation: TypeInfoMacro?) <function-ast_add_typeinfo_macro_Module_q__TypeInfoMacro_q_>`
  *  :ref:`add_variant_macro (module: Module?; annotation: VariantMacro?) <function-ast_add_variant_macro_Module_q__VariantMacro_q_>`


add_block_annotation
^^^^^^^^^^^^^^^^^^^^

.. _function-ast_add_block_annotation_ExprBlock_q__smart_ptr_ls_FunctionAnnotation_gr__ref_:

.. das:function:: add_block_annotation(block: ExprBlock?; annotation: smart_ptr<FunctionAnnotation>&)

Adds an annotation declaration to a block.


:Arguments: * **block** :  :ref:`ExprBlock <handle-ast-ExprBlock>`? implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>`>\ & implicit

.. _function-ast_add_block_annotation_ExprBlock_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_:

.. das:function:: add_block_annotation(block: ExprBlock?; annotation: smart_ptr<AnnotationDeclaration>&)

----

.. _function-ast_add_call_macro_Module_q__CallMacro_q_:

.. das:function:: add_call_macro(module: Module?; annotation: CallMacro?)

Adds an AstCallMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`CallMacro <handle-ast-CallMacro>`? implicit

.. _function-ast_add_capture_macro_Module_q__CaptureMacro_q_:

.. das:function:: add_capture_macro(module: Module?; annotation: CaptureMacro?)

Adds an AstCaptureMacro to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`CaptureMacro <handle-ast-CaptureMacro>`? implicit

.. _function-ast_add_comment_reader_Module_q__CommentReader_q_:

.. das:function:: add_comment_reader(module: Module?; reader: CommentReader?)

Adds an AstCommentReader adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **reader** :  :ref:`CommentReader <handle-ast-CommentReader>`? implicit

.. _function-ast_add_dirty_infer_macro_Module_q__PassMacro_q_:

.. das:function:: add_dirty_infer_macro(module: Module?; annotation: PassMacro?)

Adds an AstPassMacro adapter to the dirty inference pass.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`PassMacro <handle-ast-PassMacro>`? implicit

.. _function-ast_add_enumeration_annotation_Module_q__smart_ptr_ls_EnumerationAnnotation_gr__ref_:

.. das:function:: add_enumeration_annotation(module: Module?; annotation: smart_ptr<EnumerationAnnotation>&)

Adds an annotation to an enumeration and calls apply if applicable.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** : smart_ptr< :ref:`EnumerationAnnotation <handle-ast-EnumerationAnnotation>`>\ & implicit

.. _function-ast_add_for_loop_macro_Module_q__ForLoopMacro_q_:

.. das:function:: add_for_loop_macro(module: Module?; annotation: ForLoopMacro?)

Adds an AstForLoopMacro to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`ForLoopMacro <handle-ast-ForLoopMacro>`? implicit


add_function_annotation
^^^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_add_function_annotation_Function_q__smart_ptr_ls_FunctionAnnotation_gr__ref_:

.. das:function:: add_function_annotation(function: Function?; annotation: smart_ptr<FunctionAnnotation>&)

Attaches a function annotation to the specified function. The annotation is moved from the provided smart pointer.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **annotation** : smart_ptr< :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>`>\ & implicit

.. _function-ast_add_function_annotation_Module_q__smart_ptr_ls_FunctionAnnotation_gr__ref_:

.. das:function:: add_function_annotation(module: Module?; annotation: smart_ptr<FunctionAnnotation>&)

.. _function-ast_add_function_annotation_Function_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_:

.. das:function:: add_function_annotation(function: Function?; annotation: smart_ptr<AnnotationDeclaration>&)

----

.. _function-ast_add_global_lint_macro_Module_q__PassMacro_q_:

.. das:function:: add_global_lint_macro(module: Module?; annotation: PassMacro?)

Adds an AstPassMacro adapter to the global lint pass.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`PassMacro <handle-ast-PassMacro>`? implicit

.. _function-ast_add_infer_macro_Module_q__PassMacro_q_:

.. das:function:: add_infer_macro(module: Module?; annotation: PassMacro?)

Adds an AstPassMacro adapter to the type inference pass.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`PassMacro <handle-ast-PassMacro>`? implicit

.. _function-ast_add_lint_macro_Module_q__PassMacro_q_:

.. das:function:: add_lint_macro(module: Module?; annotation: PassMacro?)

Adds an AstPassMacro adapter to the lint pass of the current module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`PassMacro <handle-ast-PassMacro>`? implicit

.. _function-ast_add_module_option_Module_q__string_Type:

.. das:function:: add_module_option(module: Module?; option: string; type: Type)

Adds a module-specific option accessible via the `options` keyword.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **option** : string implicit

            * **type** :  :ref:`Type <enum-rtti-Type>`

.. _function-ast_add_new_block_annotation_string_auto_0x357:

.. das:function:: add_new_block_annotation(name: string; someClassPtr: auto) : auto

def add_new_block_annotation (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_call_macro_string_auto_0x39e:

.. das:function:: add_new_call_macro(name: string; someClassPtr: auto) : auto

def add_new_call_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_capture_macro_string_auto_0x381:

.. das:function:: add_new_capture_macro(name: string; someClassPtr: auto) : auto

def add_new_capture_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_comment_reader_string_auto_0x398:

.. das:function:: add_new_comment_reader(name: string; someClassPtr: auto) : auto

def add_new_comment_reader (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_contract_annotation_string_auto_0x363:

.. das:function:: add_new_contract_annotation(name: string; someClassPtr: auto) : auto

def add_new_contract_annotation (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_dirty_infer_macro_string_auto_0x3b0:

.. das:function:: add_new_dirty_infer_macro(name: string; someClassPtr: auto) : auto

def add_new_dirty_infer_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_enumeration_annotation_string_auto_0x36f:

.. das:function:: add_new_enumeration_annotation(name: string; someClassPtr: auto) : auto

def add_new_enumeration_annotation (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_for_loop_macro_string_auto_0x37b:

.. das:function:: add_new_for_loop_macro(name: string; someClassPtr: auto) : auto

def add_new_for_loop_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_function_annotation_string_auto_0x35d:

.. das:function:: add_new_function_annotation(name: string; someClassPtr: auto) : auto

def add_new_function_annotation (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_global_lint_macro_string_auto_0x3bc:

.. das:function:: add_new_global_lint_macro(name: string; someClassPtr: auto) : auto

def add_new_global_lint_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_infer_macro_string_auto_0x3aa:

.. das:function:: add_new_infer_macro(name: string; someClassPtr: auto) : auto

def add_new_infer_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_lint_macro_string_auto_0x3b6:

.. das:function:: add_new_lint_macro(name: string; someClassPtr: auto) : auto

def add_new_lint_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_optimization_macro_string_auto_0x3c2:

.. das:function:: add_new_optimization_macro(name: string; someClassPtr: auto) : auto

def add_new_optimization_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_reader_macro_string_auto_0x393:

.. das:function:: add_new_reader_macro(name: string; someClassPtr: auto) : auto

def add_new_reader_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_simulate_macro_string_auto_0x38d:

.. das:function:: add_new_simulate_macro(name: string; someClassPtr: auto) : auto

def add_new_simulate_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_structure_annotation_string_auto_0x369:

.. das:function:: add_new_structure_annotation(name: string; someClassPtr: auto) : auto

def add_new_structure_annotation (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_type_macro_string_auto_0x387:

.. das:function:: add_new_type_macro(name: string; someClassPtr: auto) : auto

def add_new_type_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_typeinfo_macro_string_auto_0x3a4:

.. das:function:: add_new_typeinfo_macro(name: string; someClassPtr: auto) : auto

def add_new_typeinfo_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_new_variant_macro_string_auto_0x375:

.. das:function:: add_new_variant_macro(name: string; someClassPtr: auto) : auto

def add_new_variant_macro (name: string; var someClassPtr: auto) : auto

:Arguments: * **name** : string

            * **someClassPtr** : auto

.. _function-ast_add_optimization_macro_Module_q__PassMacro_q_:

.. das:function:: add_optimization_macro(module: Module?; annotation: PassMacro?)

Adds an AstPassMacro adapter to the optimization pass of a specific module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`PassMacro <handle-ast-PassMacro>`? implicit

.. _function-ast_add_reader_macro_Module_q__ReaderMacro_q_:

.. das:function:: add_reader_macro(module: Module?; annotation: ReaderMacro?)

Adds an AstReaderMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`ReaderMacro <handle-ast-ReaderMacro>`? implicit

.. _function-ast_add_simulate_macro_Module_q__SimulateMacro_q_:

.. das:function:: add_simulate_macro(module: Module?; annotation: SimulateMacro?)

Adds an AstSimulateMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`SimulateMacro <handle-ast-SimulateMacro>`? implicit


add_structure_annotation
^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-ast_add_structure_annotation_Structure_q__smart_ptr_ls_StructureAnnotation_gr__ref_:

.. das:function:: add_structure_annotation(structure: Structure?; annotation: smart_ptr<StructureAnnotation>&)

Attaches a structure annotation to the specified structure. The annotation is moved from the provided smart pointer.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **annotation** : smart_ptr< :ref:`StructureAnnotation <handle-ast-StructureAnnotation>`>\ & implicit

.. _function-ast_add_structure_annotation_Module_q__smart_ptr_ls_StructureAnnotation_gr__ref_:

.. das:function:: add_structure_annotation(module: Module?; annotation: smart_ptr<StructureAnnotation>&)

.. _function-ast_add_structure_annotation_Structure_q__smart_ptr_ls_AnnotationDeclaration_gr__ref_:

.. das:function:: add_structure_annotation(structure: Structure?; annotation: smart_ptr<AnnotationDeclaration>&)

----

.. _function-ast_add_type_macro_Module_q__TypeMacro_q_:

.. das:function:: add_type_macro(module: Module?; annotation: TypeMacro?)

Adds an AstTypeMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`TypeMacro <handle-ast-TypeMacro>`? implicit

.. _function-ast_add_typeinfo_macro_Module_q__TypeInfoMacro_q_:

.. das:function:: add_typeinfo_macro(module: Module?; annotation: TypeInfoMacro?)

Adds an AstTypeInfoMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? implicit

.. _function-ast_add_variant_macro_Module_q__VariantMacro_q_:

.. das:function:: add_variant_macro(module: Module?; annotation: VariantMacro?)

Adds an AstVariantMacro adapter to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **annotation** :  :ref:`VariantMacro <handle-ast-VariantMacro>`? implicit


+++++++++++++++++++++++++
Adding objects to objects
+++++++++++++++++++++++++

  *  :ref:`add_alias (module: Module?; structure: TypeDecl?&) : bool <function-ast_add_alias_Module_q__TypeDecl_q__ref_>`
  *  :ref:`add_enumeration_entry (enum: Enumeration?; name: string) : int <function-ast_add_enumeration_entry_Enumeration_q__string>`
  *  :ref:`add_function (module: Module?; function: Function?&) : bool <function-ast_add_function_Module_q__Function_q__ref_>`
  *  :ref:`add_generic (module: Module?; function: Function?&) : bool <function-ast_add_generic_Module_q__Function_q__ref_>`
  *  :ref:`add_keyword (module: Module?; keyword: string; needOxfordComma: bool) : bool <function-ast_add_keyword_Module_q__string_bool>`
  *  :ref:`add_module_require (module: Module?; publicModule: Module?; pub: bool) : bool <function-ast_add_module_require_Module_q__Module_q__bool>`
  *  :ref:`add_ptr_ref (expression: Expression?) : Expression? <function-ast_add_ptr_ref_Expression_q_>`
  *  :ref:`add_structure (module: Module?; structure: Structure?&) : bool <function-ast_add_structure_Module_q__Structure_q__ref_>`
  *  :ref:`add_structure_alias (structure: Structure?; aliasName: string; alias: TypeDecl? const&) : bool <function-ast_add_structure_alias_Structure_q__string_TypeDecl_q__const_ref_>`
  *  :ref:`add_type_function (module: Module?; keyword: string) : bool <function-ast_add_type_function_Module_q__string>`
  *  :ref:`add_variable (module: Module?; variable: Variable?&) : bool <function-ast_add_variable_Module_q__Variable_q__ref_>`

.. _function-ast_add_alias_Module_q__TypeDecl_q__ref_:

.. das:function:: add_alias(module: Module?; structure: TypeDecl?&) : bool

Adds a type alias to the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **structure** :  :ref:`TypeDecl <handle-ast-TypeDecl>`?\ & implicit

.. _function-ast_add_enumeration_entry_Enumeration_q__string:

.. das:function:: add_enumeration_entry(enum: Enumeration?; name: string) : int

Adds a new entry with the given name to the specified enumeration. Returns the integer value assigned to the new entry.

:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>`? implicit

            * **name** : string implicit

.. _function-ast_add_function_Module_q__Function_q__ref_:

.. das:function:: add_function(module: Module?; function: Function?&) : bool

Adds a function to the specified module. Returns true if the function was added successfully, or false if a conflicting function already exists.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **function** :  :ref:`Function <handle-ast-Function>`?\ & implicit

.. _function-ast_add_generic_Module_q__Function_q__ref_:

.. das:function:: add_generic(module: Module?; function: Function?&) : bool

Adds a generic (template) function to the specified module. Returns true if the generic was added successfully, or false if a conflicting generic already exists.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **function** :  :ref:`Function <handle-ast-Function>`?\ & implicit

.. _function-ast_add_keyword_Module_q__string_bool:

.. das:function:: add_keyword(module: Module?; keyword: string; needOxfordComma: bool) : bool

Registers a new keyword in the specified module, making it available to the parser.



:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **keyword** : string implicit

            * **needOxfordComma** : bool

.. _function-ast_add_module_require_Module_q__Module_q__bool:

.. das:function:: add_module_require(module: Module?; publicModule: Module?; pub: bool) : bool

Adds module dependencies, similar to the `require` keyword.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **publicModule** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **pub** : bool

.. _function-ast_add_ptr_ref_Expression_q_:

.. das:function:: add_ptr_ref(expression: Expression?) : Expression?

Wraps an expression in a pointer-to-reference (r2v) conversion node if needed.


:Arguments: * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_add_structure_Module_q__Structure_q__ref_:

.. das:function:: add_structure(module: Module?; structure: Structure?&) : bool

Adds a structure definition to the specified module. Returns true if the structure was added successfully, or false if a structure with the same name already exists.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **structure** :  :ref:`Structure <handle-ast-Structure>`?\ & implicit

.. _function-ast_add_structure_alias_Structure_q__string_TypeDecl_q__const_ref_:

.. das:function:: add_structure_alias(structure: Structure?; aliasName: string; alias: TypeDecl? const&) : bool

Adds a typedef alias to a structure type in the AST, equivalent to a typedef in the structure body.


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **aliasName** : string implicit

            * **alias** :  :ref:`TypeDecl <handle-ast-TypeDecl>`?\ & implicit

.. _function-ast_add_type_function_Module_q__string:

.. das:function:: add_type_function(module: Module?; keyword: string) : bool

Adds a type function keyword, allowing function calls to accept type arguments before regular arguments via the `some_call<type_args>(regular_args)` syntax.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **keyword** : string implicit

.. _function-ast_add_variable_Module_q__Variable_q__ref_:

.. das:function:: add_variable(module: Module?; variable: Variable?&) : bool

Adds a global variable to the specified module. Returns true if the variable was added successfully, or false if a variable with the same name already exists.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **variable** :  :ref:`Variable <handle-ast-Variable>`?\ & implicit


+++++++++++++++++++++++++
Program and module access
+++++++++++++++++++++++++

  *  :ref:`compiling_module () : Module? <function-ast_compiling_module>`
  *  :ref:`compiling_program () : smart_ptr\<Program\> <function-ast_compiling_program>`
  *  :ref:`this_module () : Module? <function-ast_this_module>`
  *  :ref:`this_program () : smart_ptr\<Program\> <function-ast_this_program>`

.. _function-ast_compiling_module:

.. das:function:: compiling_module() : Module?

Returns the module currently being compiled.


.. _function-ast_compiling_program:

.. das:function:: compiling_program() : smart_ptr<Program>

Returns the program currently being compiled.


.. _function-ast_this_module:

.. das:function:: this_module() : Module?

Returns the main module attached to the current context, throwing an error if RTTI is disabled.


.. _function-ast_this_program:

.. das:function:: this_program() : smart_ptr<Program>

Returns the program attached to the current context, or null if RTTI is disabled.



+++++++++++++++++++++++++++++++++++
Textual descriptions of the objects
+++++++++++++++++++++++++++++++++++

  *  :ref:`das_to_string (type: Type) : string <function-ast_das_to_string_Type>`
  *  :ref:`describe (expr: FunctionPtr) : auto <function-ast_describe_FunctionPtr>`
  *  :ref:`describe (prog: smart_ptr\<Program\>) : auto <function-ast_describe_smart_ptr_ls_Program_gr_>`
  *  :ref:`describe (expr: Expression?) : auto <function-ast_describe_Expression_q_>`
  *  :ref:`describe (decl: TypeDecl?; extra: bool = true; contracts: bool = true; modules: bool = true) : auto <function-ast_describe_TypeDecl_q__bool_bool_bool>`
  *  :ref:`describe_cpp (decl: TypeDecl?; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true; chooseSmartPtr: bool = true) : auto <function-ast_describe_cpp_TypeDecl_q__bool_bool_bool_bool_bool>`
  *  :ref:`describe_expression (expression: Expression?) : string <function-ast_describe_expression_Expression_q_>`
  *  :ref:`describe_function (function: Function?) : string <function-ast_describe_function_Function_q_>`
  *  :ref:`describe_program (program: smart_ptr\<Program\>) : string <function-ast_describe_program_smart_ptr_ls_Program_gr_>`
  *  :ref:`describe_typedecl (type: TypeDecl?; extra: bool; contracts: bool; module: bool) : string <function-ast_describe_typedecl_TypeDecl_q__bool_bool_bool>`
  *  :ref:`describe_typedecl_cpp (type: TypeDecl?; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool; choose_smart_ptr: bool) : string <function-ast_describe_typedecl_cpp_TypeDecl_q__bool_bool_bool_bool_bool>`

.. _function-ast_das_to_string_Type:

.. das:function:: das_to_string(type: Type) : string

Returns the name of the corresponding daslang base type as a string.


:Arguments: * **type** :  :ref:`Type <enum-rtti-Type>`


describe
^^^^^^^^

.. _function-ast_describe_FunctionPtr:

.. das:function:: describe(expr: FunctionPtr) : auto

def describe (expr: FunctionPtr) : auto

:Arguments: * **expr** :  :ref:`FunctionPtr <alias-FunctionPtr>`

.. _function-ast_describe_smart_ptr_ls_Program_gr_:

.. das:function:: describe(prog: smart_ptr<Program>) : auto

.. _function-ast_describe_Expression_q_:

.. das:function:: describe(expr: Expression?) : auto

.. _function-ast_describe_TypeDecl_q__bool_bool_bool:

.. das:function:: describe(decl: TypeDecl?; extra: bool = true; contracts: bool = true; modules: bool = true) : auto

----

.. _function-ast_describe_cpp_TypeDecl_q__bool_bool_bool_bool_bool:

.. das:function:: describe_cpp(decl: TypeDecl?; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true; chooseSmartPtr: bool = true) : auto

def describe_cpp (decl: TypeDecl?; substitureRef: bool = false; skipRef: bool = false; skipConst: bool = false; redundantConst: bool = true; chooseSmartPtr: bool = true) : auto

:Arguments: * **decl** :  :ref:`TypeDecl <handle-ast-TypeDecl>`?

            * **substitureRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

            * **chooseSmartPtr** : bool

.. _function-ast_describe_expression_Expression_q_:

.. das:function:: describe_expression(expression: Expression?) : string

Returns a string description of the Expression matching the corresponding daslang source code.


:Arguments: * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_describe_function_Function_q_:

.. das:function:: describe_function(function: Function?) : string

Returns a human-readable string description of the specified function, including its name, arguments, and return type.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_describe_program_smart_ptr_ls_Program_gr_:

.. das:function:: describe_program(program: smart_ptr<Program>) : string

Returns a textual representation of the entire compiled program, equivalent to the output produced by ``options log``. Includes all types, globals, and functions after macro expansion, template instantiation, and type inference.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

.. _function-ast_describe_typedecl_TypeDecl_q__bool_bool_bool:

.. das:function:: describe_typedecl(type: TypeDecl?; extra: bool; contracts: bool; module: bool) : string

Returns a string description of the TypeDecl matching the corresponding daslang type declaration.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **extra** : bool

            * **contracts** : bool

            * **module** : bool

.. _function-ast_describe_typedecl_cpp_TypeDecl_q__bool_bool_bool_bool_bool:

.. das:function:: describe_typedecl_cpp(type: TypeDecl?; substitueRef: bool; skipRef: bool; skipConst: bool; redundantConst: bool; choose_smart_ptr: bool) : string

Returns a string description of the TypeDecl matching the corresponding C++ type declaration.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **substitueRef** : bool

            * **skipRef** : bool

            * **skipConst** : bool

            * **redundantConst** : bool

            * **choose_smart_ptr** : bool


+++++++++
Searching
+++++++++

  *  :ref:`find_bitfield_name (bit: TypeDecl?; value: bitfield) : string <function-ast_find_bitfield_name_TypeDecl_q__bitfield>`
  *  :ref:`find_call_macro (module: Module?; name: string) : CallMacro? <function-ast_find_call_macro_Module_q__string>`
  *  :ref:`find_compiling_function_by_mangled_name_hash (moduleName: string; mangledNameHash: uint64) : Function? <function-ast_find_compiling_function_by_mangled_name_hash_string_uint64>`
  *  :ref:`find_compiling_module (name: string) : Module? <function-ast_find_compiling_module_string>`
  *  :ref:`find_enum_name (enum: Enumeration?; value: int64) : string <function-ast_find_enum_name_Enumeration_q__int64>`
  *  :ref:`find_enum_value (enum: Enumeration?; value: string) : int64 <function-ast_find_enum_value_Enumeration_q__string>`
  *  :ref:`find_matching_variable (program: Program?; function: Function?; name: string; seePrivate: bool; block: block\<(array\<Variable?\>#):void\>) <function-ast_find_matching_variable_Program_q__Function_q__string_bool_block_ls_array_ls_Variable_q__gr__hh__c_void_gr_>`
  *  :ref:`find_module (prog: smart_ptr\<Program\>; name: string) : Module? <function-ast_find_module_smart_ptr_ls_Program_gr__string>`
  *  :ref:`find_module (name: string) : Module? <function-ast_find_module_string>`
  *  :ref:`find_module_function_via_rtti (module: Module?; function: function\<():void\>) : Function? <function-ast_find_module_function_via_rtti_Module_q__function_ls__c_void_gr_>`
  *  :ref:`find_module_via_rtti (program: smart_ptr\<Program\>; name: string) : Module? <function-ast_find_module_via_rtti_smart_ptr_ls_Program_gr__string>`
  *  :ref:`find_struct_field_parent (structure: Structure?; name: string) : Structure const? <function-ast_find_struct_field_parent_Structure_q__string>`
  *  :ref:`find_structure_field (structPtr: Structure?; field: string) : FieldDeclaration? <function-ast_find_structure_field_Structure_q__string>`
  *  :ref:`find_unique_structure (program: smart_ptr\<Program\>; name: string) : Structure? <function-ast_find_unique_structure_smart_ptr_ls_Program_gr__string>`
  *  :ref:`find_variable (module: Module?; variable: string) : Variable? <function-ast_find_variable_Module_q__string>`

.. _function-ast_find_bitfield_name_TypeDecl_q__bitfield:

.. das:function:: find_bitfield_name(bit: TypeDecl?; value: bitfield) : string

Finds the name of a bitfield value in the specified type.


:Arguments: * **bit** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **value** : bitfield<>

.. _function-ast_find_call_macro_Module_q__string:

.. das:function:: find_call_macro(module: Module?; name: string) : CallMacro?

Finds a CallMacro by name in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

.. _function-ast_find_compiling_function_by_mangled_name_hash_string_uint64:

.. das:function:: find_compiling_function_by_mangled_name_hash(moduleName: string; mangledNameHash: uint64) : Function?

Returns a Function from the currently compiling program given its mangled name hash.


:Arguments: * **moduleName** : string implicit

            * **mangledNameHash** : uint64

.. _function-ast_find_compiling_module_string:

.. das:function:: find_compiling_module(name: string) : Module?

def find_compiling_module (name: string) : Module?

:Arguments: * **name** : string

.. _function-ast_find_enum_name_Enumeration_q__int64:

.. das:function:: find_enum_name(enum: Enumeration?; value: int64) : string

Finds the name corresponding to an enumeration value in the specified type.


:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>`? implicit

            * **value** : int64

.. _function-ast_find_enum_value_Enumeration_q__string:

.. das:function:: find_enum_value(enum: Enumeration?; value: string) : int64

Finds the integer value corresponding to an enumeration name in the specified type.


:Arguments: * **enum** :  :ref:`Enumeration <handle-ast-Enumeration>`? implicit

            * **value** : string implicit

.. _function-ast_find_matching_variable_Program_q__Function_q__string_bool_block_ls_array_ls_Variable_q__gr__hh__c_void_gr_:

.. das:function:: find_matching_variable(program: Program?; function: Function?; name: string; seePrivate: bool; block: block<(array<Variable?>#):void>)

Searches the program for global variables matching the given name that are visible from the specified function's scope. The matching variables are passed to the provided block as a temporary array.

:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>`? implicit

            * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **name** : string implicit

            * **seePrivate** : bool

            * **block** : block<(array< :ref:`Variable <handle-ast-Variable>`?>\ #):void> implicit


find_module
^^^^^^^^^^^

.. _function-ast_find_module_smart_ptr_ls_Program_gr__string:

.. das:function:: find_module(prog: smart_ptr<Program>; name: string) : Module?

def find_module (prog: smart_ptr<Program>; name: string) : Module?

:Arguments: * **prog** : smart_ptr< :ref:`Program <handle-rtti-Program>`>

            * **name** : string

.. _function-ast_find_module_string:

.. das:function:: find_module(name: string) : Module?

----

.. _function-ast_find_module_function_via_rtti_Module_q__function_ls__c_void_gr_:

.. das:function:: find_module_function_via_rtti(module: Module?; function: function<():void>) : Function?

Finds a function by name in the specified module using RTTI.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **function** : function<void>

.. _function-ast_find_module_via_rtti_smart_ptr_ls_Program_gr__string:

.. das:function:: find_module_via_rtti(program: smart_ptr<Program>; name: string) : Module?

Finds a module by name in the specified program using RTTI.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **name** : string implicit

.. _function-ast_find_struct_field_parent_Structure_q__string:

.. das:function:: find_struct_field_parent(structure: Structure?; name: string) : Structure const?

Finds and returns the parent structure in the inheritance chain that originally declares the field with the given name. Returns null if the field is not found.

:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **name** : string implicit

.. _function-ast_find_structure_field_Structure_q__string:

.. das:function:: find_structure_field(structPtr: Structure?; field: string) : FieldDeclaration?

Returns the FieldDeclaration for a specific field of a structure type, or null if not found.


:Arguments: * **structPtr** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **field** : string implicit

.. _function-ast_find_unique_structure_smart_ptr_ls_Program_gr__string:

.. das:function:: find_unique_structure(program: smart_ptr<Program>; name: string) : Structure?

Finds a uniquely named structure in the program, returning it if unique or null if ambiguous.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **name** : string implicit

.. _function-ast_find_variable_Module_q__string:

.. das:function:: find_variable(module: Module?; variable: string) : Variable?

Finds a variable by name in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **variable** : string implicit


+++++++++
Iterating
+++++++++

  *  :ref:`any_array_foreach (array: void?; stride: int; block: block\<(void?):void\>) <function-ast_any_array_foreach_void_q__int_block_ls_void_q__c_void_gr_>`
  *  :ref:`any_table_foreach (table: void?; keyStride: int; valueStride: int; block: block\<(void?;void?):void\>) <function-ast_any_table_foreach_void_q__int_int_block_ls_void_q_;void_q__c_void_gr_>`
  *  :ref:`for_each_annotation_ordered (module: Module?; block: block\<(uint64;uint64):void\>) <function-ast_for_each_annotation_ordered_Module_q__block_ls_uint64;uint64_c_void_gr_>`
  *  :ref:`for_each_call_macro (module: Module?; block: block\<(string#):void\>) <function-ast_for_each_call_macro_Module_q__block_ls_string_hh__c_void_gr_>`
  *  :ref:`for_each_enumeration (module: Module?; block: block\<(Enumeration?):void\>) <function-ast_for_each_enumeration_Module_q__block_ls_Enumeration_q__c_void_gr_>`
  *  :ref:`for_each_enumeration (mod: Module?; blk: block\<(var en:EnumerationPtr):void\>) : auto <function-ast_for_each_enumeration_Module_q__block_ls_var_en_c_EnumerationPtr_c_void_gr_>`
  *  :ref:`for_each_field (annotation: BasicStructureAnnotation; block: block\<(string;string;TypeDecl?;uint):void\>) <function-ast_for_each_field_BasicStructureAnnotation_block_ls_string;string;TypeDecl_q_;uint_c_void_gr_>`
  *  :ref:`for_each_for_loop_macro (module: Module?; block: block\<(ForLoopMacro?):void\>) <function-ast_for_each_for_loop_macro_Module_q__block_ls_ForLoopMacro_q__c_void_gr_>`
  *  :ref:`for_each_function (module: Module?; name: string; block: block\<(Function?):void\>) <function-ast_for_each_function_Module_q__string_block_ls_Function_q__c_void_gr_>`
  *  :ref:`for_each_function (mod: Module?; name: string; blk: block\<(var func:FunctionPtr):void\>) : auto <function-ast_for_each_function_Module_q__string_block_ls_var_func_c_FunctionPtr_c_void_gr_>`
  *  :ref:`for_each_generic (module: Module?; name: string; block: block\<(Function?):void\>) <function-ast_for_each_generic_Module_q__string_block_ls_Function_q__c_void_gr_>`
  *  :ref:`for_each_generic (module: Module?; block: block\<(Function?):void\>) <function-ast_for_each_generic_Module_q__block_ls_Function_q__c_void_gr_>`
  *  :ref:`for_each_global (module: Module?; block: block\<(Variable?):void\>) <function-ast_for_each_global_Module_q__block_ls_Variable_q__c_void_gr_>`
  *  :ref:`for_each_global (mod: Module?; blk: block\<(var value:VariablePtr):void\>) : auto <function-ast_for_each_global_Module_q__block_ls_var_value_c_VariablePtr_c_void_gr_>`
  *  :ref:`for_each_module (program: Program?; block: block\<(Module?):void\>) <function-ast_for_each_module_Program_q__block_ls_Module_q__c_void_gr_>`
  *  :ref:`for_each_module_function (module: Module?; blk: block\<(Function?):void\>) <function-ast_for_each_module_function_Module_q__block_ls_Function_q__c_void_gr_>`
  *  :ref:`for_each_module_no_order (program: Program?; block: block\<(Module?):void\>) <function-ast_for_each_module_no_order_Program_q__block_ls_Module_q__c_void_gr_>`
  *  :ref:`for_each_reader_macro (module: Module?; block: block\<(string#):void\>) <function-ast_for_each_reader_macro_Module_q__block_ls_string_hh__c_void_gr_>`
  *  :ref:`for_each_structure (module: Module?; block: block\<(Structure?):void\>) <function-ast_for_each_structure_Module_q__block_ls_Structure_q__c_void_gr_>`
  *  :ref:`for_each_structure (mod: Module?; blk: block\<(var st:StructurePtr):void\>) : auto <function-ast_for_each_structure_Module_q__block_ls_var_st_c_StructurePtr_c_void_gr_>`
  *  :ref:`for_each_structure_alias (structure: Structure?; block: block\<(TypeDecl?):void\>) <function-ast_for_each_structure_alias_Structure_q__block_ls_TypeDecl_q__c_void_gr_>`
  *  :ref:`for_each_typedef (module: Module?; block: block\<(string#;TypeDecl?):void\>) <function-ast_for_each_typedef_Module_q__block_ls_string_hh_;TypeDecl_q__c_void_gr_>`
  *  :ref:`for_each_typedef (mod: Module?; blk: block\<(name:string#;var value:TypeDeclPtr):void\>) : auto <function-ast_for_each_typedef_Module_q__block_ls_name_c_string_hh_;var_value_c_TypeDeclPtr_c_void_gr_>`
  *  :ref:`for_each_typeinfo_macro (module: Module?; block: block\<(TypeInfoMacro?):void\>) <function-ast_for_each_typeinfo_macro_Module_q__block_ls_TypeInfoMacro_q__c_void_gr_>`
  *  :ref:`for_each_typemacro (module: Module?; block: block\<(TypeMacro?):void\>) <function-ast_for_each_typemacro_Module_q__block_ls_TypeMacro_q__c_void_gr_>`
  *  :ref:`for_each_variant_macro (module: Module?; block: block\<(VariantMacro?):void\>) <function-ast_for_each_variant_macro_Module_q__block_ls_VariantMacro_q__c_void_gr_>`

.. _function-ast_any_array_foreach_void_q__int_block_ls_void_q__c_void_gr_:

.. das:function:: any_array_foreach(array: void?; stride: int; block: block<(void?):void>)

Iterates through any `array<>` type in a typeless fashion using `void?` pointers.


:Arguments: * **array** : void? implicit

            * **stride** : int

            * **block** : block<(void?):void> implicit

.. _function-ast_any_table_foreach_void_q__int_int_block_ls_void_q_;void_q__c_void_gr_:

.. das:function:: any_table_foreach(table: void?; keyStride: int; valueStride: int; block: block<(void?;void?):void>)

Iterates through any `table<>` type in a typeless fashion using `void?` pointers.


:Arguments: * **table** : void? implicit

            * **keyStride** : int

            * **valueStride** : int

            * **block** : block<(void?;void?):void> implicit

.. _function-ast_for_each_annotation_ordered_Module_q__block_ls_uint64;uint64_c_void_gr_:

.. das:function:: for_each_annotation_ordered(module: Module?; block: block<(uint64;uint64):void>)

Iterates through each annotation in the given module in the order they were added.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<(uint64;uint64):void> implicit

.. _function-ast_for_each_call_macro_Module_q__block_ls_string_hh__c_void_gr_:

.. das:function:: for_each_call_macro(module: Module?; block: block<(string#):void>)

Iterates through every CallMacro adapter in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<(string\ #):void> implicit


for_each_enumeration
^^^^^^^^^^^^^^^^^^^^

.. _function-ast_for_each_enumeration_Module_q__block_ls_Enumeration_q__c_void_gr_:

.. das:function:: for_each_enumeration(module: Module?; block: block<(Enumeration?):void>)

Iterates over all enumerations defined in the given module, calling the provided block for each one.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`Enumeration <handle-ast-Enumeration>`?):void> implicit

.. _function-ast_for_each_enumeration_Module_q__block_ls_var_en_c_EnumerationPtr_c_void_gr_:

.. das:function:: for_each_enumeration(mod: Module?; blk: block<(var en:EnumerationPtr):void>) : auto

----

.. _function-ast_for_each_field_BasicStructureAnnotation_block_ls_string;string;TypeDecl_q_;uint_c_void_gr_:

.. das:function:: for_each_field(annotation: BasicStructureAnnotation; block: block<(string;string;TypeDecl?;uint):void>)

Iterates through every field in a BuiltinStructure handled type.


:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>` implicit

            * **block** : block<(string;string; :ref:`TypeDecl <handle-ast-TypeDecl>`?;uint):void> implicit

.. _function-ast_for_each_for_loop_macro_Module_q__block_ls_ForLoopMacro_q__c_void_gr_:

.. das:function:: for_each_for_loop_macro(module: Module?; block: block<(ForLoopMacro?):void>)

Iterates through every for-loop macro in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`ForLoopMacro <handle-ast-ForLoopMacro>`?):void> implicit


for_each_function
^^^^^^^^^^^^^^^^^

.. _function-ast_for_each_function_Module_q__string_block_ls_Function_q__c_void_gr_:

.. das:function:: for_each_function(module: Module?; name: string; block: block<(Function?):void>)

Iterates over all functions with the specified name in the given module, calling the provided block for each matching function.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

            * **block** : block<( :ref:`Function <handle-ast-Function>`?):void> implicit

.. _function-ast_for_each_function_Module_q__string_block_ls_var_func_c_FunctionPtr_c_void_gr_:

.. das:function:: for_each_function(mod: Module?; name: string; blk: block<(var func:FunctionPtr):void>) : auto

----


for_each_generic
^^^^^^^^^^^^^^^^

.. _function-ast_for_each_generic_Module_q__string_block_ls_Function_q__c_void_gr_:

.. das:function:: for_each_generic(module: Module?; name: string; block: block<(Function?):void>)

Iterates over all generic functions with the specified name in the given module, calling the provided block for each matching generic.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

            * **block** : block<( :ref:`Function <handle-ast-Function>`?):void> implicit

.. _function-ast_for_each_generic_Module_q__block_ls_Function_q__c_void_gr_:

.. das:function:: for_each_generic(module: Module?; block: block<(Function?):void>)

----


for_each_global
^^^^^^^^^^^^^^^

.. _function-ast_for_each_global_Module_q__block_ls_Variable_q__c_void_gr_:

.. das:function:: for_each_global(module: Module?; block: block<(Variable?):void>)

Iterates over all global variables defined in the given module, calling the provided block for each one.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`Variable <handle-ast-Variable>`?):void> implicit

.. _function-ast_for_each_global_Module_q__block_ls_var_value_c_VariablePtr_c_void_gr_:

.. das:function:: for_each_global(mod: Module?; blk: block<(var value:VariablePtr):void>) : auto

----

.. _function-ast_for_each_module_Program_q__block_ls_Module_q__c_void_gr_:

.. das:function:: for_each_module(program: Program?; block: block<(Module?):void>)

Iterates through each module in the program in dependency order.


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>`? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>`?):void> implicit

.. _function-ast_for_each_module_function_Module_q__block_ls_Function_q__c_void_gr_:

.. das:function:: for_each_module_function(module: Module?; blk: block<(Function?):void>)

Iterates over all functions defined in the given module, calling the provided block for each function regardless of name.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **blk** : block<( :ref:`Function <handle-ast-Function>`?):void> implicit

.. _function-ast_for_each_module_no_order_Program_q__block_ls_Module_q__c_void_gr_:

.. das:function:: for_each_module_no_order(program: Program?; block: block<(Module?):void>)

Iterates through each module in the program in no particular order, as they appear in the library group.


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>`? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>`?):void> implicit

.. _function-ast_for_each_reader_macro_Module_q__block_ls_string_hh__c_void_gr_:

.. das:function:: for_each_reader_macro(module: Module?; block: block<(string#):void>)

Iterates through each reader macro in the given module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<(string\ #):void> implicit


for_each_structure
^^^^^^^^^^^^^^^^^^

.. _function-ast_for_each_structure_Module_q__block_ls_Structure_q__c_void_gr_:

.. das:function:: for_each_structure(module: Module?; block: block<(Structure?):void>)

Iterates over all structures defined in the given module, calling the provided block for each one.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`Structure <handle-ast-Structure>`?):void> implicit

.. _function-ast_for_each_structure_Module_q__block_ls_var_st_c_StructurePtr_c_void_gr_:

.. das:function:: for_each_structure(mod: Module?; blk: block<(var st:StructurePtr):void>) : auto

----

.. _function-ast_for_each_structure_alias_Structure_q__block_ls_TypeDecl_q__c_void_gr_:

.. das:function:: for_each_structure_alias(structure: Structure?; block: block<(TypeDecl?):void>)

Iterates over all structure aliases defined in the given structure type, invoking the provided block for each alias.


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **block** : block<( :ref:`TypeDecl <handle-ast-TypeDecl>`?):void> implicit


for_each_typedef
^^^^^^^^^^^^^^^^

.. _function-ast_for_each_typedef_Module_q__block_ls_string_hh_;TypeDecl_q__c_void_gr_:

.. das:function:: for_each_typedef(module: Module?; block: block<(string#;TypeDecl?):void>)

Iterates through every typedef in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<(string\ #; :ref:`TypeDecl <handle-ast-TypeDecl>`?):void> implicit

.. _function-ast_for_each_typedef_Module_q__block_ls_name_c_string_hh_;var_value_c_TypeDeclPtr_c_void_gr_:

.. das:function:: for_each_typedef(mod: Module?; blk: block<(name:string#;var value:TypeDeclPtr):void>) : auto

----

.. _function-ast_for_each_typeinfo_macro_Module_q__block_ls_TypeInfoMacro_q__c_void_gr_:

.. das:function:: for_each_typeinfo_macro(module: Module?; block: block<(TypeInfoMacro?):void>)

Iterates through each typeinfo macro in the given module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`?):void> implicit

.. _function-ast_for_each_typemacro_Module_q__block_ls_TypeMacro_q__c_void_gr_:

.. das:function:: for_each_typemacro(module: Module?; block: block<(TypeMacro?):void>)

Iterates over all type macros registered in the given module, invoking the provided block for each one.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`TypeMacro <handle-ast-TypeMacro>`?):void> implicit

.. _function-ast_for_each_variant_macro_Module_q__block_ls_VariantMacro_q__c_void_gr_:

.. das:function:: for_each_variant_macro(module: Module?; block: block<(VariantMacro?):void>)

Iterates through each variant macro in the given module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **block** : block<( :ref:`VariantMacro <handle-ast-VariantMacro>`?):void> implicit


+++++++
Cloning
+++++++

  *  :ref:`clone_expression (expression: Expression?) : Expression? <function-ast_clone_expression_Expression_q_>`
  *  :ref:`clone_file_info (name: string; tab_size: int) : FileInfo? <function-ast_clone_file_info_string_int>`
  *  :ref:`clone_function (function: Function?) : Function? <function-ast_clone_function_Function_q_>`
  *  :ref:`clone_structure (structure: Structure const?) : Structure? <function-ast_clone_structure_Structure_const_q_>`
  *  :ref:`clone_type (type: TypeDecl?) : TypeDecl? <function-ast_clone_type_TypeDecl_q_>`
  *  :ref:`clone_variable (variable: Variable?) : Variable? <function-ast_clone_variable_Variable_q_>`

.. _function-ast_clone_expression_Expression_q_:

.. das:function:: clone_expression(expression: Expression?) : Expression?

Clones an Expression along with all its subexpressions and corresponding type information.


:Arguments: * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_clone_file_info_string_int:

.. das:function:: clone_file_info(name: string; tab_size: int) : FileInfo?

Clones a FileInfo structure.


:Arguments: * **name** : string implicit

            * **tab_size** : int

.. _function-ast_clone_function_Function_q_:

.. das:function:: clone_function(function: Function?) : Function?

Creates and returns a deep copy of the specified function, including its body, arguments, and annotations.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_clone_structure_Structure_const_q_:

.. das:function:: clone_structure(structure: Structure const?) : Structure?

Returns a deep clone of the specified Structure.


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

.. _function-ast_clone_type_TypeDecl_q_:

.. das:function:: clone_type(type: TypeDecl?) : TypeDecl?

Clones a TypeDecl along with all its subtypes.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

.. _function-ast_clone_variable_Variable_q_:

.. das:function:: clone_variable(variable: Variable?) : Variable?

Creates and returns a deep copy of the specified variable, including its type declaration and initializer expression.

:Arguments: * **variable** :  :ref:`Variable <handle-ast-Variable>`? implicit


++++++++++++
Mangled name
++++++++++++

  *  :ref:`get_mangled_name (function: Function?) : string <function-ast_get_mangled_name_Function_q_>`
  *  :ref:`get_mangled_name (type: TypeDecl?) : string <function-ast_get_mangled_name_TypeDecl_q_>`
  *  :ref:`get_mangled_name (variable: Variable?) : string <function-ast_get_mangled_name_Variable_q_>`
  *  :ref:`get_mangled_name (variable: ExprBlock?) : string <function-ast_get_mangled_name_ExprBlock_q_>`
  *  :ref:`get_mangled_name (decl: ExprBlock?) : auto <function-ast_get_mangled_name_ExprBlock_q__0x5e>`
  *  :ref:`parse_mangled_name (txt: string; lib: ModuleGroup; thisModule: Module?) : TypeDecl? <function-ast_parse_mangled_name_string_ModuleGroup_Module_q_>`


get_mangled_name
^^^^^^^^^^^^^^^^

.. _function-ast_get_mangled_name_Function_q_:

.. das:function:: get_mangled_name(function: Function?) : string

Returns the mangled name of the given function, which uniquely identifies it by encoding the function name along with its argument and return types.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_get_mangled_name_TypeDecl_q_:

.. das:function:: get_mangled_name(type: TypeDecl?) : string

.. _function-ast_get_mangled_name_Variable_q_:

.. das:function:: get_mangled_name(variable: Variable?) : string

.. _function-ast_get_mangled_name_ExprBlock_q_:

.. das:function:: get_mangled_name(variable: ExprBlock?) : string

.. _function-ast_get_mangled_name_ExprBlock_q__0x5e:

.. das:function:: get_mangled_name(decl: ExprBlock?) : auto

----

.. _function-ast_parse_mangled_name_string_ModuleGroup_Module_q_:

.. das:function:: parse_mangled_name(txt: string; lib: ModuleGroup; thisModule: Module?) : TypeDecl?

Parses a mangled name string and creates the corresponding TypeDecl.


:Arguments: * **txt** : string implicit

            * **lib** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>`? implicit


+++++++++++++++
Size and offset
+++++++++++++++

  *  :ref:`any_array_size (array: void?) : int <function-ast_any_array_size_void_q_>`
  *  :ref:`any_table_size (table: void?) : int <function-ast_any_table_size_void_q_>`
  *  :ref:`get_handled_type_field_offset (type: smart_ptr\<TypeAnnotation\>; field: string) : uint <function-ast_get_handled_type_field_offset_smart_ptr_ls_TypeAnnotation_gr__string>`
  *  :ref:`get_tuple_field_offset (typle: TypeDecl?; index: int) : int <function-ast_get_tuple_field_offset_TypeDecl_q__int>`
  *  :ref:`get_variant_field_offset (variant: TypeDecl?; index: int) : int <function-ast_get_variant_field_offset_TypeDecl_q__int>`

.. _function-ast_any_array_size_void_q_:

.. das:function:: any_array_size(array: void?) : int

Returns the size of an array from a pointer to an `array<>` object.


:Arguments: * **array** : void? implicit

.. _function-ast_any_table_size_void_q_:

.. das:function:: any_table_size(table: void?) : int

Returns the size of a table from a pointer to a `table<>` object.


:Arguments: * **table** : void? implicit

.. _function-ast_get_handled_type_field_offset_smart_ptr_ls_TypeAnnotation_gr__string:

.. das:function:: get_handled_type_field_offset(type: smart_ptr<TypeAnnotation>; field: string) : uint

Returns the byte offset of a field in a ManagedStructure handled type.


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> implicit

            * **field** : string implicit

.. _function-ast_get_tuple_field_offset_TypeDecl_q__int:

.. das:function:: get_tuple_field_offset(typle: TypeDecl?; index: int) : int

Returns the byte offset of a tuple field.


:Arguments: * **typle** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **index** : int

.. _function-ast_get_variant_field_offset_TypeDecl_q__int:

.. das:function:: get_variant_field_offset(variant: TypeDecl?; index: int) : int

Returns the byte offset of a variant field.


:Arguments: * **variant** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **index** : int


+++++++++++
Evaluations
+++++++++++

  *  :ref:`eval_single_expression (expr: Expression?; ok: bool&) : float4 <function-ast_eval_single_expression_Expression_q__bool_ref_>`

.. _function-ast_eval_single_expression_Expression_q__bool_ref_:

.. das:function:: eval_single_expression(expr: Expression?; ok: bool&) : float4

.. warning::
  This is unsafe operation.

Simulates and evaluates a single expression on a separate context.


:Arguments: * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

            * **ok** : bool\ & implicit


+++++++++++++++
Error reporting
+++++++++++++++

  *  :ref:`macro_error (porogram: smart_ptr\<Program\>; at: LineInfo; message: string) <function-ast_macro_error_smart_ptr_ls_Program_gr__LineInfo_string>`
  *  :ref:`macro_performance_warning (porogram: smart_ptr\<Program\>; at: LineInfo; message: string) <function-ast_macro_performance_warning_smart_ptr_ls_Program_gr__LineInfo_string>`
  *  :ref:`macro_style_warning (porogram: smart_ptr\<Program\>; at: LineInfo; message: string) <function-ast_macro_style_warning_smart_ptr_ls_Program_gr__LineInfo_string>`

.. _function-ast_macro_error_smart_ptr_ls_Program_gr__LineInfo_string:

.. das:function:: macro_error(porogram: smart_ptr<Program>; at: LineInfo; message: string)

Reports an error to the currently compiling program during the active compilation pass.


:Arguments: * **porogram** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

            * **message** : string implicit

.. _function-ast_macro_performance_warning_smart_ptr_ls_Program_gr__LineInfo_string:

.. das:function:: macro_performance_warning(porogram: smart_ptr<Program>; at: LineInfo; message: string)

Reports a performance warning (error code 40217) at the given source location during compilation.


:Arguments: * **porogram** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

            * **message** : string implicit

.. _function-ast_macro_style_warning_smart_ptr_ls_Program_gr__LineInfo_string:

.. das:function:: macro_style_warning(porogram: smart_ptr<Program>; at: LineInfo; message: string)

Reports a style warning (error code 40218) at the given source location during compilation.


:Arguments: * **porogram** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

            * **message** : string implicit


++++++++++++++++++++
Location and context
++++++++++++++++++++

  *  :ref:`collect_dependencies (function: Function?; block: block\<(array\<Function?\>;array\<Variable?\>):void\>) <function-ast_collect_dependencies_Function_q__block_ls_array_ls_Function_q__gr_;array_ls_Variable_q__gr__c_void_gr_>`
  *  :ref:`force_at (function: Function?; at: LineInfo) <function-ast_force_at_Function_q__LineInfo>`
  *  :ref:`force_at (expression: Expression?; at: LineInfo) <function-ast_force_at_Expression_q__LineInfo>`
  *  :ref:`get_ast_context (program: smart_ptr\<Program\>; expression: Expression?; block: block\<(bool;AstContext):void\>) <function-ast_get_ast_context_smart_ptr_ls_Program_gr__Expression_q__block_ls_bool;AstContext_c_void_gr_>`

.. _function-ast_collect_dependencies_Function_q__block_ls_array_ls_Function_q__gr_;array_ls_Variable_q__gr__c_void_gr_:

.. das:function:: collect_dependencies(function: Function?; block: block<(array<Function?>;array<Variable?>):void>)

Collects all functions and global variables that the specified function depends on, and passes them to the provided block as two arrays.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **block** : block<(array< :ref:`Function <handle-ast-Function>`?>;array< :ref:`Variable <handle-ast-Variable>`?>):void> implicit


force_at
^^^^^^^^

.. _function-ast_force_at_Function_q__LineInfo:

.. das:function:: force_at(function: Function?; at: LineInfo)

Overrides the line information of the specified function and all expressions within its body to the given location. Useful for macro-generated functions that should point to a specific source location.

:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

.. _function-ast_force_at_Expression_q__LineInfo:

.. das:function:: force_at(expression: Expression?; at: LineInfo)

----

.. _function-ast_get_ast_context_smart_ptr_ls_Program_gr__Expression_q__block_ls_bool;AstContext_c_void_gr_:

.. das:function:: get_ast_context(program: smart_ptr<Program>; expression: Expression?; block: block<(bool;AstContext):void>)

Returns the AstContext for a given expression, including the current function, loops, blocks, scopes, and with sections.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit

            * **block** : block<(bool; :ref:`AstContext <handle-ast-AstContext>`):void> implicit


+++++++++++
Use queries
+++++++++++

  *  :ref:`get_use_functions (func: Function?; block: block\<(Function?):void\>) <function-ast_get_use_functions_Function_q__block_ls_Function_q__c_void_gr_>`
  *  :ref:`get_use_global_variables (func: Function?; block: block\<(Variable?):void\>) <function-ast_get_use_global_variables_Function_q__block_ls_Variable_q__c_void_gr_>`

.. _function-ast_get_use_functions_Function_q__block_ls_Function_q__c_void_gr_:

.. das:function:: get_use_functions(func: Function?; block: block<(Function?):void>)

Iterates over all functions that the given function calls or references, invoking the provided block for each one.

:Arguments: * **func** :  :ref:`Function <handle-ast-Function>`? implicit

            * **block** : block<( :ref:`Function <handle-ast-Function>`?):void> implicit

.. _function-ast_get_use_global_variables_Function_q__block_ls_Variable_q__c_void_gr_:

.. das:function:: get_use_global_variables(func: Function?; block: block<(Variable?):void>)

Iterates over all global variables that the given function accesses, invoking the provided block for each one.

:Arguments: * **func** :  :ref:`Function <handle-ast-Function>`? implicit

            * **block** : block<( :ref:`Variable <handle-ast-Variable>`?):void> implicit


+++
Log
+++

  *  :ref:`to_compilation_log (text: string) <function-ast_to_compilation_log_string>`

.. _function-ast_to_compilation_log_string:

.. das:function:: to_compilation_log(text: string)

Writes a message to the compilation log from a macro during compilation.


:Arguments: * **text** : string implicit


+++++++
Removal
+++++++

  *  :ref:`remove_structure (module: Module?; structure: Structure?&) : bool <function-ast_remove_structure_Module_q__Structure_q__ref_>`

.. _function-ast_remove_structure_Module_q__Structure_q__ref_:

.. das:function:: remove_structure(module: Module?; structure: Structure?&) : bool

Removes the specified structure from the given module. Returns true if the structure was found and removed, false otherwise.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **structure** :  :ref:`Structure <handle-ast-Structure>`?\ & implicit


++++++++++
Properties
++++++++++

  *  :ref:`can_access_global_variable (variable: Variable? const&; module: Module?; thisModule: Module?) : bool <function-ast_can_access_global_variable_Variable_q__const_ref__Module_q__Module_q_>`
  *  :ref:`get_aot_arg_prefix (func: Function?; call: ExprCallFunc?; argIndex: int) : string <function-ast_get_aot_arg_prefix_Function_q__ExprCallFunc_q__int>`
  *  :ref:`get_aot_arg_suffix (func: Function?; call: ExprCallFunc?; argIndex: int) : string <function-ast_get_aot_arg_suffix_Function_q__ExprCallFunc_q__int>`
  *  :ref:`get_aot_hash_comment (fun: Function const?) : string <function-ast_get_aot_hash_comment_Function_const_q_>`
  *  :ref:`get_aot_name (func: Function?; call: ExprCallFunc?) : string <function-ast_get_aot_name_Function_q__ExprCallFunc_q_>`
  *  :ref:`get_current_search_module (program: Program?; function: Function?; moduleName: string) : Module? <function-ast_get_current_search_module_Program_q__Function_q__string>`
  *  :ref:`get_field_type (type: TypeDecl?; fieldName: string; constant: bool) : TypeDecl? <function-ast_get_field_type_TypeDecl_q__string_bool>`
  *  :ref:`get_file_source_line (info: FileInfo?; line: uint; blk: block\<(string#):void\>) <function-ast_get_file_source_line_FileInfo_q__uint_block_ls_string_hh__c_void_gr_>`
  *  :ref:`get_func_aot_prefix (ann: FunctionAnnotation?; stg: StringBuilderWriter?; call: ExprCallFunc?) <function-ast_get_func_aot_prefix_FunctionAnnotation_q__StringBuilderWriter_q__ExprCallFunc_q_>`
  *  :ref:`get_function_aot_hash (fun: Function const?) : uint64 <function-ast_get_function_aot_hash_Function_const_q_>`
  *  :ref:`get_function_hash_by_id (fun: Function?; id: int; pctx: void?) : uint64 <function-ast_get_function_hash_by_id_Function_q__int_void_q_>`
  *  :ref:`get_handled_type_field_type (type: smart_ptr\<TypeAnnotation\>; field: string) : TypeInfo? <function-ast_get_handled_type_field_type_smart_ptr_ls_TypeAnnotation_gr__string>`
  *  :ref:`get_handled_type_field_type_declaration (type: smart_ptr\<TypeAnnotation\>; field: string; isConst: bool) : TypeDecl? <function-ast_get_handled_type_field_type_declaration_smart_ptr_ls_TypeAnnotation_gr__string_bool>`
  *  :ref:`get_handled_type_index_type_declaration (type: TypeAnnotation?; src: Expression?; idx: Expression?) : TypeDecl? <function-ast_get_handled_type_index_type_declaration_TypeAnnotation_q__Expression_q__Expression_q_>`
  *  :ref:`get_struct_aot_prefix (ann: StructureAnnotation?; structure: Structure?; args: AnnotationArgumentList; stg: StringBuilderWriter?) <function-ast_get_struct_aot_prefix_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_>`
  *  :ref:`get_structure_alias (structure: Structure?; aliasName: string) : TypeDecl? <function-ast_get_structure_alias_Structure_q__string>`
  *  :ref:`get_underlying_value_type (type: TypeDecl?) : TypeDecl? <function-ast_get_underlying_value_type_TypeDecl_q_>`
  *  :ref:`get_vector_length (vec: void?; type: TypeDecl?) : int <function-ast_get_vector_length_void_q__TypeDecl_q_>`
  *  :ref:`get_vector_ptr_at_index (vec: void?; type: TypeDecl?; idx: int) : void? <function-ast_get_vector_ptr_at_index_void_q__TypeDecl_q__int>`
  *  :ref:`has_field (type: TypeDecl?; fieldName: string; constant: bool) : bool <function-ast_has_field_TypeDecl_q__string_bool>`
  *  :ref:`is_expr_const (expression: Expression?) : bool <function-ast_is_expr_const_Expression_q_>`
  *  :ref:`is_expr_like_call (expression: Expression?) : bool <function-ast_is_expr_like_call_Expression_q_>`
  *  :ref:`is_same_type (argType: TypeDecl?; passType: TypeDecl?; refMatters: bool; constMatters: bool; temporaryMatters: bool; allowSubstitute: bool) : bool <function-ast_is_same_type_TypeDecl_q__TypeDecl_q__bool_bool_bool_bool>`
  *  :ref:`is_same_type (leftType: TypeDecl?; rightType: TypeDecl?; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool <function-ast_is_same_type_TypeDecl_q__TypeDecl_q__RefMatters_ConstMatters_TemporaryMatters>`
  *  :ref:`is_temp_type (type: TypeDecl?; refMatters: bool) : bool <function-ast_is_temp_type_TypeDecl_q__bool>`
  *  :ref:`is_visible_directly (from_module: Module?; which_module: Module?) : bool <function-ast_is_visible_directly_Module_q__Module_q_>`

.. _function-ast_can_access_global_variable_Variable_q__const_ref__Module_q__Module_q_:

.. das:function:: can_access_global_variable(variable: Variable? const&; module: Module?; thisModule: Module?) : bool

Checks whether the given global variable is accessible from the specified module context, taking into account visibility rules and the module where the access originates.

:Arguments: * **variable** :  :ref:`Variable <handle-ast-Variable>`?\ & implicit

            * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **thisModule** :  :ref:`Module <handle-rtti-Module>`? implicit

.. _function-ast_get_aot_arg_prefix_Function_q__ExprCallFunc_q__int:

.. das:function:: get_aot_arg_prefix(func: Function?; call: ExprCallFunc?; argIndex: int) : string

Returns the AOT argument prefix string for the specified function.


:Arguments: * **func** :  :ref:`Function <handle-ast-Function>`? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>`? implicit

            * **argIndex** : int

.. _function-ast_get_aot_arg_suffix_Function_q__ExprCallFunc_q__int:

.. das:function:: get_aot_arg_suffix(func: Function?; call: ExprCallFunc?; argIndex: int) : string

Returns the AOT argument suffix string for the specified function.


:Arguments: * **func** :  :ref:`Function <handle-ast-Function>`? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>`? implicit

            * **argIndex** : int

.. _function-ast_get_aot_hash_comment_Function_const_q_:

.. das:function:: get_aot_hash_comment(fun: Function const?) : string

Returns a diagnostic string containing the function's own semantic hash and all non-builtin dependency hashes with their mangled names. Used for comparing AOT-generated hash comments with runtime-computed values to diagnose AOT link failures (error 50101).


:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_get_aot_name_Function_q__ExprCallFunc_q_:

.. das:function:: get_aot_name(func: Function?; call: ExprCallFunc?) : string

Returns the AOT-generated name for the specified function.


:Arguments: * **func** :  :ref:`Function <handle-ast-Function>`? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>`? implicit

.. _function-ast_get_current_search_module_Program_q__Function_q__string:

.. das:function:: get_current_search_module(program: Program?; function: Function?; moduleName: string) : Module?

Returns the module currently being searched for a function by name, correctly resolving special names like `""`, `"_"`, `"*"`, and `"__"`.


:Arguments: * **program** :  :ref:`Program <handle-rtti-Program>`? implicit

            * **function** :  :ref:`Function <handle-ast-Function>`? implicit

            * **moduleName** : string implicit

.. _function-ast_get_field_type_TypeDecl_q__string_bool:

.. das:function:: get_field_type(type: TypeDecl?; fieldName: string; constant: bool) : TypeDecl?

Returns the type of a field if the target is a structure, variant, tuple, handled type, or pointer to any of those, or null otherwise.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-ast_get_file_source_line_FileInfo_q__uint_block_ls_string_hh__c_void_gr_:

.. das:function:: get_file_source_line(info: FileInfo?; line: uint; blk: block<(string#):void>)

Reads a single source line from a FileInfo and invokes the block with the line text as a temporary string. Zero-alloc — the string is valid only within the block.


:Arguments: * **info** :  :ref:`FileInfo <handle-rtti-FileInfo>`? implicit

            * **line** : uint

            * **blk** : block<(string\ #):void> implicit

.. _function-ast_get_func_aot_prefix_FunctionAnnotation_q__StringBuilderWriter_q__ExprCallFunc_q_:

.. das:function:: get_func_aot_prefix(ann: FunctionAnnotation?; stg: StringBuilderWriter?; call: ExprCallFunc?)

Returns the AOT function prefix string for the specified function.


:Arguments: * **ann** :  :ref:`FunctionAnnotation <handle-ast-FunctionAnnotation>`? implicit

            * **stg** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **call** :  :ref:`ExprCallFunc <handle-ast-ExprCallFunc>`? implicit

.. _function-ast_get_function_aot_hash_Function_const_q_:

.. das:function:: get_function_aot_hash(fun: Function const?) : uint64

Returns the hash of a function used for AOT matching.


:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>`? implicit

.. _function-ast_get_function_hash_by_id_Function_q__int_void_q_:

.. das:function:: get_function_hash_by_id(fun: Function?; id: int; pctx: void?) : uint64

Returns the hash of a function given its unique identifier.


:Arguments: * **fun** :  :ref:`Function <handle-ast-Function>`? implicit

            * **id** : int

            * **pctx** : void? implicit

.. _function-ast_get_handled_type_field_type_smart_ptr_ls_TypeAnnotation_gr__string:

.. das:function:: get_handled_type_field_type(type: smart_ptr<TypeAnnotation>; field: string) : TypeInfo?

Returns the type of a field in a ManagedStructure handled type.


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> implicit

            * **field** : string implicit

.. _function-ast_get_handled_type_field_type_declaration_smart_ptr_ls_TypeAnnotation_gr__string_bool:

.. das:function:: get_handled_type_field_type_declaration(type: smart_ptr<TypeAnnotation>; field: string; isConst: bool) : TypeDecl?

Returns the type declaration of a field in a ManagedStructure handled type.


:Arguments: * **type** : smart_ptr< :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`> implicit

            * **field** : string implicit

            * **isConst** : bool

.. _function-ast_get_handled_type_index_type_declaration_TypeAnnotation_q__Expression_q__Expression_q_:

.. das:function:: get_handled_type_index_type_declaration(type: TypeAnnotation?; src: Expression?; idx: Expression?) : TypeDecl?

Returns the type declaration of the index operator for a handled type.


:Arguments: * **type** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? implicit

            * **src** :  :ref:`Expression <handle-ast-Expression>`? implicit

            * **idx** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_get_struct_aot_prefix_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_:

.. das:function:: get_struct_aot_prefix(ann: StructureAnnotation?; structure: Structure?; args: AnnotationArgumentList; stg: StringBuilderWriter?)

Returns the AOT prefix string for the specified structure.


:Arguments: * **ann** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>`? implicit

            * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` implicit

            * **stg** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

.. _function-ast_get_structure_alias_Structure_q__string:

.. das:function:: get_structure_alias(structure: Structure?; aliasName: string) : TypeDecl?

Finds and returns a structure alias type by its alias name.


:Arguments: * **structure** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **aliasName** : string implicit

.. _function-ast_get_underlying_value_type_TypeDecl_q_:

.. das:function:: get_underlying_value_type(type: TypeDecl?) : TypeDecl?

Returns the daslang type aliased by a ManagedValue handled type.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

.. _function-ast_get_vector_length_void_q__TypeDecl_q_:

.. das:function:: get_vector_length(vec: void?; type: TypeDecl?) : int

Returns the length of a vector given a pointer to the vector object and its type declaration.


:Arguments: * **vec** : void? implicit

            * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

.. _function-ast_get_vector_ptr_at_index_void_q__TypeDecl_q__int:

.. das:function:: get_vector_ptr_at_index(vec: void?; type: TypeDecl?; idx: int) : void?

Returns a pointer to the vector element at the specified index given a pointer to the vector object and its TypeDeclPtr.


:Arguments: * **vec** : void? implicit

            * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **idx** : int

.. _function-ast_has_field_TypeDecl_q__string_bool:

.. das:function:: has_field(type: TypeDecl?; fieldName: string; constant: bool) : bool

Returns true if a structure, variant, tuple, handled type, or pointer to any of those has the specified field.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **fieldName** : string implicit

            * **constant** : bool

.. _function-ast_is_expr_const_Expression_q_:

.. das:function:: is_expr_const(expression: Expression?) : bool

Returns true if the expression is or inherits from ExprConst.


:Arguments: * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_is_expr_like_call_Expression_q_:

.. das:function:: is_expr_like_call(expression: Expression?) : bool

Returns true if the expression is or inherits from ExprLooksLikeCall.


:Arguments: * **expression** :  :ref:`Expression <handle-ast-Expression>`? implicit


is_same_type
^^^^^^^^^^^^

.. _function-ast_is_same_type_TypeDecl_q__TypeDecl_q__bool_bool_bool_bool:

.. das:function:: is_same_type(argType: TypeDecl?; passType: TypeDecl?; refMatters: bool; constMatters: bool; temporaryMatters: bool; allowSubstitute: bool) : bool

Compares two types using the given comparison parameters and returns true if they match.


:Arguments: * **argType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **passType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **refMatters** : bool

            * **constMatters** : bool

            * **temporaryMatters** : bool

            * **allowSubstitute** : bool

.. _function-ast_is_same_type_TypeDecl_q__TypeDecl_q__RefMatters_ConstMatters_TemporaryMatters:

.. das:function:: is_same_type(leftType: TypeDecl?; rightType: TypeDecl?; refMatters: RefMatters; constMatters: ConstMatters; tempMatters: TemporaryMatters) : bool

----

.. _function-ast_is_temp_type_TypeDecl_q__bool:

.. das:function:: is_temp_type(type: TypeDecl?; refMatters: bool) : bool

Returns true if the specified type can be temporary.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **refMatters** : bool

.. _function-ast_is_visible_directly_Module_q__Module_q_:

.. das:function:: is_visible_directly(from_module: Module?; which_module: Module?) : bool

Returns true if one module is directly visible from another module.


:Arguments: * **from_module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **which_module** :  :ref:`Module <handle-rtti-Module>`? implicit


+++++
Infer
+++++

  *  :ref:`infer_generic_type (type: TypeDecl?; passType: TypeDecl?; topLevel: bool; isPassType: bool) : TypeDecl? <function-ast_infer_generic_type_TypeDecl_q__TypeDecl_q__bool_bool>`
  *  :ref:`update_alias_map (program: smart_ptr\<Program\>; argType: TypeDecl?; passType: TypeDecl?) <function-ast_update_alias_map_smart_ptr_ls_Program_gr__TypeDecl_q__TypeDecl_q_>`

.. _function-ast_infer_generic_type_TypeDecl_q__TypeDecl_q__bool_bool:

.. das:function:: infer_generic_type(type: TypeDecl?; passType: TypeDecl?; topLevel: bool; isPassType: bool) : TypeDecl?

Infers a concrete type from a generic type declaration and a pass type.


:Arguments: * **type** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **passType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **topLevel** : bool

            * **isPassType** : bool

.. _function-ast_update_alias_map_smart_ptr_ls_Program_gr__TypeDecl_q__TypeDecl_q_:

.. das:function:: update_alias_map(program: smart_ptr<Program>; argType: TypeDecl?; passType: TypeDecl?)

Updates the alias map for the specified type during inference.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **argType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit

            * **passType** :  :ref:`TypeDecl <handle-ast-TypeDecl>`? implicit


++++++++++++++
Module queries
++++++++++++++

  *  :ref:`module_find_annotation (module: Module const?; name: string) : smart_ptr\<Annotation\> <function-ast_module_find_annotation_Module_const_q__string>`
  *  :ref:`module_find_structure (program: Module const?; name: string) : Structure? <function-ast_module_find_structure_Module_const_q__string>`
  *  :ref:`module_find_type_annotation (module: Module const?; name: string) : TypeAnnotation? <function-ast_module_find_type_annotation_Module_const_q__string>`
  *  :ref:`not_inferred (function: Function?) <function-ast_not_inferred_Function_q_>`

.. _function-ast_module_find_annotation_Module_const_q__string:

.. das:function:: module_find_annotation(module: Module const?; name: string) : smart_ptr<Annotation>

Finds an annotation by name in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

.. _function-ast_module_find_structure_Module_const_q__string:

.. das:function:: module_find_structure(program: Module const?; name: string) : Structure?

Finds a structure by name in the specified module.


:Arguments: * **program** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

.. _function-ast_module_find_type_annotation_Module_const_q__string:

.. das:function:: module_find_type_annotation(module: Module const?; name: string) : TypeAnnotation?

Finds a type annotation by name in the specified module.


:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **name** : string implicit

.. _function-ast_not_inferred_Function_q_:

.. das:function:: not_inferred(function: Function?)

Marks a function as modified by a macro so that it will be inferred again.


:Arguments: * **function** :  :ref:`Function <handle-ast-Function>`? implicit


++++++++++++++++++
Debug info helpers
++++++++++++++++++

  *  :ref:`debug_helper_find_struct_cppname (helper: smart_ptr\<DebugInfoHelper\> const&; struct_info: StructInfo?) : string <function-ast_debug_helper_find_struct_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_ref__StructInfo_q_>`
  *  :ref:`debug_helper_find_type_cppname (helper: smart_ptr\<DebugInfoHelper\> const&; type_info: TypeInfo?) : string <function-ast_debug_helper_find_type_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_ref__TypeInfo_q_>`
  *  :ref:`debug_helper_iter_enums (helper: smart_ptr\<DebugInfoHelper\>; blk: block\<(string;EnumInfo?):void\>) <function-ast_debug_helper_iter_enums_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;EnumInfo_q__c_void_gr_>`
  *  :ref:`debug_helper_iter_funcs (helper: smart_ptr\<DebugInfoHelper\>; blk: block\<(string;FuncInfo?):void\>) <function-ast_debug_helper_iter_funcs_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;FuncInfo_q__c_void_gr_>`
  *  :ref:`debug_helper_iter_structs (helper: smart_ptr\<DebugInfoHelper\>; blk: block\<(string;StructInfo?):void\>) <function-ast_debug_helper_iter_structs_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;StructInfo_q__c_void_gr_>`
  *  :ref:`debug_helper_iter_types (helper: smart_ptr\<DebugInfoHelper\>; blk: block\<(string;TypeInfo?):void\>) <function-ast_debug_helper_iter_types_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;TypeInfo_q__c_void_gr_>`
  *  :ref:`debug_helper_iter_vars (helper: smart_ptr\<DebugInfoHelper\>; blk: block\<(string;VarInfo?):void\>) <function-ast_debug_helper_iter_vars_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;VarInfo_q__c_void_gr_>`

.. _function-ast_debug_helper_find_struct_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_ref__StructInfo_q_:

.. das:function:: debug_helper_find_struct_cppname(helper: smart_ptr<DebugInfoHelper> const&; struct_info: StructInfo?) : string

Finds a structure in the DebugInfoHelper and returns its C++ name.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`>\ & implicit

            * **struct_info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-ast_debug_helper_find_type_cppname_smart_ptr_ls_DebugInfoHelper_gr__const_ref__TypeInfo_q_:

.. das:function:: debug_helper_find_type_cppname(helper: smart_ptr<DebugInfoHelper> const&; type_info: TypeInfo?) : string

Finds a type in the DebugInfoHelper and returns its C++ name.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`>\ & implicit

            * **type_info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>`? implicit

.. _function-ast_debug_helper_iter_enums_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;EnumInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_enums(helper: smart_ptr<DebugInfoHelper>; blk: block<(string;EnumInfo?):void>)

Iterates through all enumerations in the DebugInfoHelper, invoking the provided block for each one.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** : block<(string; :ref:`EnumInfo <handle-rtti-EnumInfo>`?):void> implicit

.. _function-ast_debug_helper_iter_funcs_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;FuncInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_funcs(helper: smart_ptr<DebugInfoHelper>; blk: block<(string;FuncInfo?):void>)

Iterates through all functions in the DebugInfoHelper, invoking the provided block for each one.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** : block<(string; :ref:`FuncInfo <handle-rtti-FuncInfo>`?):void> implicit

.. _function-ast_debug_helper_iter_structs_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;StructInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_structs(helper: smart_ptr<DebugInfoHelper>; blk: block<(string;StructInfo?):void>)

Iterates through all structures in the DebugInfoHelper, invoking the provided block for each one.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** : block<(string; :ref:`StructInfo <handle-rtti-StructInfo>`?):void> implicit

.. _function-ast_debug_helper_iter_types_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;TypeInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_types(helper: smart_ptr<DebugInfoHelper>; blk: block<(string;TypeInfo?):void>)

Iterates through all types in the DebugInfoHelper, invoking the provided block for each one.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** : block<(string; :ref:`TypeInfo <handle-rtti-TypeInfo>`?):void> implicit

.. _function-ast_debug_helper_iter_vars_smart_ptr_ls_DebugInfoHelper_gr__block_ls_string;VarInfo_q__c_void_gr_:

.. das:function:: debug_helper_iter_vars(helper: smart_ptr<DebugInfoHelper>; blk: block<(string;VarInfo?):void>)

Iterates through all variables in the DebugInfoHelper, invoking the provided block for each one.


:Arguments: * **helper** : smart_ptr< :ref:`DebugInfoHelper <handle-rtti-DebugInfoHelper>`> implicit

            * **blk** : block<(string; :ref:`VarInfo <handle-rtti-VarInfo>`?):void> implicit


+++++++++++
AOT support
+++++++++++

  *  :ref:`aot_need_type_info (macro: TypeInfoMacro const?; expr: Expression?) : bool <function-ast_aot_need_type_info_TypeInfoMacro_const_q__Expression_q_>`
  *  :ref:`aot_previsit_get_field (ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string) <function-ast_aot_previsit_get_field_TypeAnnotation_q__StringBuilderWriter_q__string>`
  *  :ref:`aot_previsit_get_field_ptr (ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string) <function-ast_aot_previsit_get_field_ptr_TypeAnnotation_q__StringBuilderWriter_q__string>`
  *  :ref:`aot_require (mod: Module?; ss: StringBuilderWriter?) : bool <function-ast_aot_require_Module_q__StringBuilderWriter_q_>`
  *  :ref:`aot_type_ann_get_field_ptr (ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string) <function-ast_aot_type_ann_get_field_ptr_TypeAnnotation_q__StringBuilderWriter_q__string>`
  *  :ref:`aot_visit_get_field (ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string) <function-ast_aot_visit_get_field_TypeAnnotation_q__StringBuilderWriter_q__string>`
  *  :ref:`getInitSemanticHashWithDep (program: smart_ptr\<Program\>; init: uint64) : uint64 <function-ast_getInitSemanticHashWithDep_smart_ptr_ls_Program_gr__uint64>`
  *  :ref:`macro_aot_infix (macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?) : bool <function-ast_macro_aot_infix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_>`
  *  :ref:`write_aot_body (structure: StructureAnnotation?; st: Structure?; args: AnnotationArgumentList; writer: StringBuilderWriter?) <function-ast_write_aot_body_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_>`
  *  :ref:`write_aot_macro_prefix (macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?) <function-ast_write_aot_macro_prefix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_>`
  *  :ref:`write_aot_macro_suffix (macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?) <function-ast_write_aot_macro_suffix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_>`
  *  :ref:`write_aot_suffix (structure: StructureAnnotation?; st: Structure?; args: AnnotationArgumentList; writer: StringBuilderWriter?) <function-ast_write_aot_suffix_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_>`

.. _function-ast_aot_need_type_info_TypeInfoMacro_const_q__Expression_q_:

.. das:function:: aot_need_type_info(macro: TypeInfoMacro const?; expr: Expression?) : bool

Returns true if a `TypeInfo?` is needed for the specified type in a typeinfo expression.


:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? implicit

            * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_aot_previsit_get_field_TypeAnnotation_q__StringBuilderWriter_q__string:

.. das:function:: aot_previsit_get_field(ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string)

Performs the pre-visit step for field access during AOT code generation.


:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **name** : string implicit

.. _function-ast_aot_previsit_get_field_ptr_TypeAnnotation_q__StringBuilderWriter_q__string:

.. das:function:: aot_previsit_get_field_ptr(ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string)

Performs the pre-visit step for field pointer access during AOT code generation.


:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **name** : string implicit

.. _function-ast_aot_require_Module_q__StringBuilderWriter_q_:

.. das:function:: aot_require(mod: Module?; ss: StringBuilderWriter?) : bool

Writes data to the require section of an AOT module.


:Arguments: * **mod** :  :ref:`Module <handle-rtti-Module>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

.. _function-ast_aot_type_ann_get_field_ptr_TypeAnnotation_q__StringBuilderWriter_q__string:

.. das:function:: aot_type_ann_get_field_ptr(ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string)

Returns the access symbol string for a field, such as `->` for pointer types or `.` for value types.


:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **name** : string implicit

.. _function-ast_aot_visit_get_field_TypeAnnotation_q__StringBuilderWriter_q__string:

.. das:function:: aot_visit_get_field(ann: TypeAnnotation?; ss: StringBuilderWriter?; name: string)

Performs the visit step for field access during AOT code generation.


:Arguments: * **ann** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **name** : string implicit

.. _function-ast_getInitSemanticHashWithDep_smart_ptr_ls_Program_gr__uint64:

.. das:function:: getInitSemanticHashWithDep(program: smart_ptr<Program>; init: uint64) : uint64

Returns the initialization semantic hash including dependencies for the entire program.


:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>`> implicit

            * **init** : uint64

.. _function-ast_macro_aot_infix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_:

.. das:function:: macro_aot_infix(macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?) : bool

Returns true if the macro requires an AOT infix operator for the specified handled type.


:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_write_aot_body_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_:

.. das:function:: write_aot_body(structure: StructureAnnotation?; st: Structure?; args: AnnotationArgumentList; writer: StringBuilderWriter?)

Writes the AOT (ahead-of-time compilation) body section for the given structure annotation, outputting generated C++ code to the string builder writer.

:Arguments: * **structure** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>`? implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` implicit

            * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

.. _function-ast_write_aot_macro_prefix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_:

.. das:function:: write_aot_macro_prefix(macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?)

Writes the AOT macro prefix code for the specified TypeInfoMacro.


:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_write_aot_macro_suffix_TypeInfoMacro_q__StringBuilderWriter_q__Expression_q_:

.. das:function:: write_aot_macro_suffix(macro: TypeInfoMacro?; ss: StringBuilderWriter?; expr: Expression?)

Writes the AOT macro suffix code for the specified TypeInfoMacro.


:Arguments: * **macro** :  :ref:`TypeInfoMacro <handle-ast-TypeInfoMacro>`? implicit

            * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

            * **expr** :  :ref:`Expression <handle-ast-Expression>`? implicit

.. _function-ast_write_aot_suffix_StructureAnnotation_q__Structure_q__AnnotationArgumentList_StringBuilderWriter_q_:

.. das:function:: write_aot_suffix(structure: StructureAnnotation?; st: Structure?; args: AnnotationArgumentList; writer: StringBuilderWriter?)

Writes the AOT (ahead-of-time compilation) suffix section for the given structure annotation, outputting generated C++ code to the string builder writer.

:Arguments: * **structure** :  :ref:`StructureAnnotation <handle-ast-StructureAnnotation>`? implicit

            * **st** :  :ref:`Structure <handle-ast-Structure>`? implicit

            * **args** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>` implicit

            * **writer** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit


+++++++++++++++++++++
String builder writer
+++++++++++++++++++++

  *  :ref:`string_builder_clear (ss: StringBuilderWriter?) <function-ast_string_builder_clear_StringBuilderWriter_q_>`
  *  :ref:`string_builder_str (ss: StringBuilderWriter?) : string <function-ast_string_builder_str_StringBuilderWriter_q_>`

.. _function-ast_string_builder_clear_StringBuilderWriter_q_:

.. das:function:: string_builder_clear(ss: StringBuilderWriter?)

Clears a StringBuilder object given a pointer to it.


:Arguments: * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit

.. _function-ast_string_builder_str_StringBuilderWriter_q_:

.. das:function:: string_builder_str(ss: StringBuilderWriter?) : string

Returns the accumulated string from a StringBuilder object given a pointer to it.


:Arguments: * **ss** :  :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`? implicit


++
GC
++

  *  :ref:`ast_gc_guard (block: block\<():void\>) <function-ast_ast_gc_guard_block_ls__c_void_gr_>`

.. _function-ast_ast_gc_guard_block_ls__c_void_gr_:

.. das:function:: ast_gc_guard(block: block<():void>)

Executes a block and then collects all unreachable GC nodes (TypeDecl instances) created during execution. Use this to prevent GC leak warnings in tools that create temporary AST types.


:Arguments: * **block** : block<void> implicit


