.. |class-ast_boost-MacroMacro| replace:: This macro implements [macro] function annotation.
    This adds macro initialization function, which will only be called during macro compilation.

.. |method-ast_boost-MacroMacro.apply| replace:: Implements [macro] function annotation.
    Internally it adds macro initialiation flag, as well as wraps function block in `if is_compiling_macros()` condition.

.. |class-ast_boost-TagFunctionAnnotation| replace:: This annotation is used for tagging specific funcstion.

.. |method-ast_boost-TagFunctionAnnotation.apply| replace:: Implements [tag_function] annotaiton.
    Internally this just verifies if tag has a name, i.e. bool argument without value (set to true).

.. |class-ast_boost-TagStructureAnnotation| replace:: This annotation is used for tagging specific structure.
    This annotation is used to tag structure with a name, which can be used to identify structure in the code.

.. |method-ast_boost-TagStructureAnnotation.apply| replace:: Implements [tag_structure] annotation.
    Internally this just verifies if tag has a name, i.e. bool argument without value (set to true).

.. |class-ast_boost-SetupAnyAnnotation| replace:: This is base class for any annotation or macro setup.

.. |method-ast_boost-SetupAnyAnnotation.apply| replace:: Implements macro registration setup.
    Internally this creates __setup_macros function, which is only called during this module macro compilation.
    For the particular macro it adds call to the annotation registration function call (which is overrideable member `annotation_function_call`).

.. |method-ast_boost-SetupAnyAnnotation.setup_call| replace:: Implements macro registration name setup.
    Internally this adds name parameter to the annotation registration function call (which is overridable member `name`).

.. |class-ast_boost-SetupForLoopMacro| replace:: [for_loop_macro] implementation.

.. |class-ast_boost-SetupCaptureMacro| replace:: [capture_macro] implementation.

.. |class-ast_boost-SetupFunctionAnnotatoin| replace:: [function_macro] implementation.

.. |class-ast_boost-SetupBlockAnnotatoin| replace:: [block_macro] implementation.

.. |class-ast_boost-SetupStructureAnnotatoin| replace:: [structure_macro] implementation.

.. |class-ast_boost-SetupEnumerationAnnotation| replace:: [enumration_macro] implementation.

.. |class-ast_boost-SetupContractAnnotatoin| replace:: [contract] implementation.

.. |class-ast_boost-SetupReaderMacro| replace:: [reader_macro] implementation.

.. |class-ast_boost-SetupCommentReader| replace:: [comment_reader] implementation.

.. |class-ast_boost-SetupVariantMacro| replace:: [variant_macro] implementation.

.. |class-ast_boost-SetupCallMacro| replace:: [call_macro] implementation.

.. |class-ast_boost-SetupTypeInfoMacro| replace:: [typeinfo_macro] implementation.

.. |class-ast_boost-SetupInferMacro| replace:: [infer_macro] implementation.

.. |class-ast_boost-SetupDirtyInferMacro| replace:: [dirty_infer_macro] implementation.

.. |class-ast_boost-SetupLintMacro| replace:: [lint_macro] implementation.

.. |class-ast_boost-SetupGlobalLintMacro| replace:: [global_lint_macro] implementation.

.. |class-ast_boost-SetupOptimizationMacro| replace:: [optimization_macro] implementation.

.. |class-ast_boost-TagFunctionMacro| replace:: [tag_function_macro] implementation.
    Applies annotation to all tagged functions.

.. |method-ast_boost-TagFunctionMacro.apply| replace:: Makes sure tag is defined and is a string.

.. |method-ast_boost-TagFunctionMacro.setup_call| replace:: Attaches tag as well as name to the setup call.

.. |class-ast_boost-BetterRttiVisitor| replace:: Implements `expr is type` and `expr as type` checks, using RTTI.

.. |method-ast_boost-BetterRttiVisitor.visitExprIsVariant| replace:: Implements `is type`.

.. |method-ast_boost-BetterRttiVisitor.visitExprAsVariant| replace:: Implements `as type`.

.. |method-ast_boost-BetterRttiVisitor.visitExprSafeAsVariant| replace:: Implements `?as type`.

.. |function-ast_boost-isVectorType| replace:: Returns true if type is vector type, i.e. int2, float3, and such, including range and urange.

.. |function-ast_boost-describe| replace:: Returns textuan description of the object.

.. |function-ast_boost-describe_function_short| replace:: Gives short (name, arguments with types, result type) description of the function.

.. |function-ast_boost-isExpression| replace:: Returns true if given object is derived from ast::Expression.

.. |function-ast_boost-is_same_or_inherited| replace:: Returns true if child is the same class as parent, or is inherited from the parent.

.. |function-ast_boost-is_class_method| replace:: Returns true if field is a class method.

.. |function-ast_boost-emplace_new| replace:: Emplaces newly created object into the container without memory leak (i.e. correct ptr_ref_count).

.. |function-ast_boost-override_method| replace:: Override class method `name` with new function.

.. |function-ast_boost-find_arg| replace:: Find argument in annotation argument list.

.. |function-ast_boost-find_unique_function| replace:: Returns unique function of that specific name, or null if there is none or more than one.

.. |function-ast_boost-find_unique_generic| replace:: Returns unique generic function of that specific name, or null if there is none or more than one.

.. |function-ast_boost-setup_call_list| replace:: Create new function which will contain collection of calls. Returns body block to where the call is to be appended.

.. |function-ast_boost-setup_macro| replace:: Setup macro initialization function, which will only be called during compilation of this module. Returns body block to where the macro initialization is to be appended.

.. |function-ast_boost-panic_expr_as| replace:: Function call which panics with "invalid 'as' expression or null pointer dereference" message.

.. |function-ast_boost-find_annotation| replace:: Finds annotation in the module.

.. |function-ast_boost-append_annotation| replace:: Appends function annotation to the function given its name and arguments.

.. |function-ast_boost-add_annotation_argument| replace:: Adds annotation argument to the argument list.

.. |function-ast_boost-get_for_source_index| replace:: Find index of the for loop source variable.

.. |function-ast_boost-make_static_assert_false| replace:: Creates `static_assert(false,text)` expression.

.. |function-ast_boost-describe_bitfield| replace:: Returns textual description of the bitfield.

.. |function-ast_boost-setup_tag_annotation| replace:: Creates annotation and applies it to all tagged functions given tag.

.. |function-ast_boost-convert_to_expression| replace:: Converts value to expression, which generates this value.

.. |function_annotation-ast_boost-macro| replace:: MacroMacro function annotation.

.. |function_annotation-ast_boost-tag_function| replace:: TagFunctionAnnotation function annotation.

.. |variant_macro-ast_boost-better_rtti_in_expr| replace:: This macro is used to implement `is type`, `as type` and `?as type` runtime checks for the `Expression` class and its subclasses.

.. |function-ast_boost-clone| replace:: Clones between array types.

.. |function-ast_boost-visit_expression| replace:: Visits expression, returns new expression with correct reference count (or old expression with unmodified reference count).



