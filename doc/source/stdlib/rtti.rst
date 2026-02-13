
.. _stdlib_rtti:

================================
Runtime type information library
================================

The RTTI module exposes runtime type information and program introspection facilities.
It allows querying module structure, type declarations, function signatures, annotations,
and other compile-time metadata at runtime. Used primarily by macro libraries and
code generation tools.

All functions and symbols are in "rtti" module, use require to get access to it. ::

    require rtti

++++++++++++
Type aliases
++++++++++++

.. _alias-ProgramFlags:

.. das:attribute:: bitfield ProgramFlags

Flags which represent state of the `Program` object, both during and after compilation.

:Fields: * **failToCompile** (0x1) - indicates that the program failed to compile.

         * **_unsafe** (0x2) - indicates that the program contains unsafe code.

         * **isCompiling** (0x4) - indicates that the program is currently compiling.

         * **isSimulating** (0x8) - indicates that the program is currently simulating.

         * **isCompilingMacros** (0x10) - indicates that the program is currently compiling macros.

         * **needMacroModule** (0x20) - indicates that the program needs a macro module.

         * **promoteToBuiltin** (0x40) - indicates that the program is being promoted to a builtin module.

         * **isDependency** (0x80) - indicates that the program is a dependency module.

         * **macroException** (0x100) - indicates that a macro exception has occurred.


.. _alias-context_category_flags:

.. das:attribute:: bitfield context_category_flags

Flags which specify type of the `Context`.

:Fields: * **dead** (0x1) - indicates that the context is dead.

         * **debug_context** (0x2) - indicates that the context is a debug context.

         * **thread_clone** (0x4) - indicates that the context is a thread clone.

         * **job_clone** (0x8) - indicates that the context is a job clone.

         * **opengl** (0x10) - indicates that the context is an opengl context.

         * **debugger_tick** (0x20) - indicates that the context is a debugger tick.

         * **debugger_attached** (0x40) - indicates that the context has a debugger attached.

         * **macro_context** (0x80) - indicates that the context is a macro context (i.e. compiled macro module)

         * **folding_context** (0x100) - indicates that the context is a folding context (used during compilation or optimization for the purposes of folding constants)

         * **audio** (0x200) - indicates that the context is an audio context.


.. _alias-TypeInfoFlags:

.. das:attribute:: bitfield TypeInfoFlags

Flags which specify properties of the `TypeInfo` object (any rtti type).

:Fields: * **ref** (0x1) - indicates that the type is a reference value.

         * **refType** (0x2) - indicates that the type is a reference type.

         * **canCopy** (0x4) - indicates that the type can be copied.

         * **isPod** (0x8) - indicates that the type is a plain old data type.

         * **isRawPod** (0x10) - indicates that the type is a raw plain old data type (without pointers or strings).

         * **isConst** (0x20) - indicates that the type is a const type.

         * **isTemp** (0x40) - indicates that the type is a temporary type.

         * **isImplicit** (0x80) - indicates that the type is an implicit type.

         * **refValue** (0x100) - indicates that the type is a reference value.

         * **hasInitValue** (0x200) - indicates that the type has an initial value.

         * **isSmartPtr** (0x400) - indicates that the type is a smart pointer.

         * **isSmartPtrNative** (0x800) - indicates that the type is a smart pointer native (smart_ptr_raw)

         * **isHandled** (0x1000) - indicates that the type is a handled type (annotation)

         * **heapGC** (0x2000) - indicates that the type needs marking by the garbage collector.

         * **stringHeapGC** (0x4000) - indicates that the type needs marking of strings by the garbage collector.

         * **lockCheck** (0x8000) - indicates that the type needs lock checking.

         * **isPrivate** (0x10000) - indicates that the type is private.


.. _alias-StructInfoFlags:

.. das:attribute:: bitfield StructInfoFlags

Flags which represent properties of the `StructInfo` object (rtti object which represents structure type).

:Fields: * **_class** (0x1) - This structure is a class.

         * **_lambda** (0x2) - This structure is a lambda.

         * **heapGC** (0x4) - This structure needs marking by the garbage collector.

         * **stringHeapGC** (0x8) - This structure needs marking of strings by the garbage collector.

         * **lockCheck** (0x10) - This structure needs lock checking.


.. _alias-ModuleFlags:

.. das:attribute:: bitfield ModuleFlags

:Fields: * **builtIn** (0x1) - Flags which represent the module's state.

         * **promoted** (0x2) - This module is built-in.

         * **isPublic** (0x4) - This module is promoted to a builtin module.

         * **isModule** (0x8) - This module is public.

         * **isSolidContext** (0x10) - This module is a module.

         * **fromExtraDependency** (0x20) - This module is a solid context (can't be called from other contexts via pinvoke, global variables are cemented at locations)

         * **doNotAllowUnsafe** (0x40) - This module does not allow unsafe code.

         * **wasParsedNameless** (0x80) - This module was parsed nameless.

         * **visibleEverywhere** (0x100) - This module is visible everywhere.

         * **skipLockCheck** (0x200) - This module skips lock checking.

         * **allowPodInscope** (0x400) - This module allows pod inscope.


.. _alias-AnnotationDeclarationFlags:

.. das:attribute:: bitfield AnnotationDeclarationFlags

Flags which represent properties of the `AnnotationDeclaration` object.

:Fields: * **inherited** (0x1) - Indicates that the annotation is inherited.


.. _alias-RttiValue:

.. das:attribute:: variant RttiValue

Variant type which represents value of any annotation arguments and variable annotations.

:Variants: * **tBool** : bool - boolean value

           * **tInt** : int - integer value

           * **tUInt** : uint - unsigned integer value

           * **tInt64** : int64 - 64-bit integer value

           * **tUInt64** : uint64 - 64-bit unsigned integer value

           * **tFloat** : float - floating point value

           * **tDouble** : double - double precision floating point value

           * **tString** : string - string value

           * **nothing** : any - no value


.. _alias-FileAccessPtr:

.. das:attribute:: FileAccessPtr = smart_ptr<FileAccess>

Type alias for ``smart_ptr<FileAccess>`` — a reference-counted pointer to a ``FileAccess`` object, used as the standard way to pass file access to the compiler.
+++++++++
Constants
+++++++++

.. _global-rtti-FUNCINFO_INIT:

.. das:attribute:: FUNCINFO_INIT = 0x1

Bit flag constant on ``FuncInfo.flags`` indicating that the function runs during ``Context`` initialization (``[init]`` attribute).
.. _global-rtti-FUNCINFO_BUILTIN:

.. das:attribute:: FUNCINFO_BUILTIN = 0x2

Bit flag constant on ``FuncInfo.flags`` indicating that the function is a built-in (C++-bound) function rather than a daScript-defined one.
.. _global-rtti-FUNCINFO_PRIVATE:

.. das:attribute:: FUNCINFO_PRIVATE = 0x4

Bit flag constant on ``FuncInfo.flags`` indicating that the function has ``[private]`` visibility and cannot be called from other modules.
.. _global-rtti-FUNCINFO_SHUTDOWN:

.. das:attribute:: FUNCINFO_SHUTDOWN = 0x8

Bit flag constant on ``FuncInfo.flags`` indicating that the function runs during ``Context`` shutdown (``[finalize]`` attribute).
.. _global-rtti-FUNCINFO_LATE_INIT:

.. das:attribute:: FUNCINFO_LATE_INIT = 0x20

Bit flag constant on ``FuncInfo.flags`` indicating the function uses late initialization with a custom init order (``[init(order)]`` attribute).
++++++++++++
Enumerations
++++++++++++

.. _enum-rtti-CompilationError:

.. das:attribute:: CompilationError

Enumeration which represents error type for each of the errors which compiler returns and various stages.

:Values: * **unspecified** = 0 - Unspecified error.

         * **mismatching_parentheses** = 10001 - Mismatching parentheses.

         * **mismatching_curly_bracers** = 10002 - Mismatching curly braces.

         * **string_constant_exceeds_file** = 10003 - String constant exceeds file.

         * **string_constant_exceeds_line** = 10004 - String constant exceeds line.

         * **unexpected_close_comment** = 10005 - Unexpected close comment.

         * **integer_constant_out_of_range** = 10006 - Integer constant out of range.

         * **comment_contains_eof** = 10007 - Comment contains EOF (end of file).

         * **invalid_escape_sequence** = 10008 - Invalid escape sequence.

         * **invalid_line_directive** = 10009 - Invalid line directive.

         * **syntax_error** = 20000 - Syntax error, usually invalid grammar.

         * **malformed_ast** = 20001 - Malformed AST.

         * **invalid_type** = 30101 - Invalid type.

         * **invalid_return_type** = 30102 - Invalid return type.

         * **invalid_argument_type** = 30103 - Invalid argument type.

         * **invalid_structure_field_type** = 30104 - Invalid structure field type.

         * **invalid_array_type** = 30105 - Invalid array type.

         * **invalid_table_type** = 30106 - Invalid table type.

         * **invalid_argument_count** = 30107 - Invalid argument count.

         * **invalid_variable_type** = 30108 - Invalid variable type.

         * **invalid_new_type** = 30109 - Invalid new type.

         * **invalid_index_type** = 30110 - Invalid index type.

         * **invalid_annotation** = 30111 - Invalid annotation.

         * **invalid_swizzle_mask** = 30112 - Invalid swizzle mask.

         * **invalid_initialization_type** = 30113 - Invalid initialization type.

         * **invalid_with_type** = 30114 - Invalid with type.

         * **invalid_override** = 30115 - Invalid override.

         * **invalid_name** = 30116 - Invalid name.

         * **invalid_array_dimension** = 30117 - Invalid array dimension.

         * **invalid_iteration_source** = 30118 - Invalid iteration source.

         * **invalid_loop** = 30119 - Invalid loop.

         * **invalid_label** = 30120 - Invalid label.

         * **invalid_enumeration** = 30121 - Invalid enumeration.

         * **invalid_option** = 30122 - Invalid or unsupported option.

         * **invalid_member_function** = 30123 - Invalid member function.

         * **function_already_declared** = 30201 - Function already declared.

         * **argument_already_declared** = 30202 - Argument already declared.

         * **local_variable_already_declared** = 30203 - Local variable already declared.

         * **global_variable_already_declared** = 30204 - Global variable already declared.

         * **structure_field_already_declared** = 30205 - Structure field already declared.

         * **structure_already_declared** = 30206 - Structure already declared.

         * **structure_already_has_initializer** = 30207 - Structure already has initializer.

         * **enumeration_already_declared** = 30208 - Enumeration already declared.

         * **enumeration_value_already_declared** = 30209 - Enumeration value already declared.

         * **type_alias_already_declared** = 30210 - Type alias already declared.

         * **field_already_initialized** = 30211 - Field already initialized.

         * **type_not_found** = 30301 - Type not found.

         * **structure_not_found** = 30302 - Structure not found.

         * **operator_not_found** = 30303 - Operator not found.

         * **function_not_found** = 30304 - Function not found.

         * **variable_not_found** = 30305 - Variable not found.

         * **handle_not_found** = 30306 - Handle not found.

         * **annotation_not_found** = 30307 - Annotation not found.

         * **enumeration_not_found** = 30308 - Enumeration not found.

         * **enumeration_value_not_found** = 30309 - Enumeration value not found.

         * **type_alias_not_found** = 30310 - Type alias not found.

         * **bitfield_not_found** = 30311 - Bitfield not found.

         * **cant_initialize** = 30401 - Can't initialize.

         * **cant_dereference** = 30501 - Can't dereference (not a pointer or dereferencable type).

         * **cant_index** = 30502 - Can't index (not an array, table, or indexable type).

         * **cant_get_field** = 30503 - Can't get field (not a structure or table type).

         * **cant_write_to_const** = 30504 - Can't write to const.

         * **cant_move_to_const** = 30505 - Can't move to const.

         * **cant_write_to_non_reference** = 30506 - Can't write to non-reference.

         * **cant_copy** = 30507 - Can't copy.

         * **cant_move** = 30508 - Can't move.

         * **cant_pass_temporary** = 30509 - Can't pass temporary value to non-temporary parameter.

         * **condition_must_be_bool** = 30601 - Condition must be boolean.

         * **condition_must_be_static** = 30602 - Condition must be static (for 'static_if' and 'static_elif')

         * **cant_pipe** = 30701 - Can't pipe (invalid left-hand side or right-hand side).

         * **invalid_block** = 30801 - Invalid block.

         * **return_or_break_in_finally** = 30802 - Return or break in finally section is not allowed.

         * **module_not_found** = 30901 - Module not found.

         * **module_already_has_a_name** = 30902 - Module already has a name.

         * **cant_new_handle** = 31001 - Can't new handled type.

         * **bad_delete** = 31002 - Bad delete.

         * **cant_infer_generic** = 31100 - Can't infer generic.

         * **cant_infer_missing_initializer** = 31101 - Can't infer missing initializer.

         * **cant_infer_mismatching_restrictions** = 31102 - Can't infer mismatching restrictions.

         * **invalid_cast** = 31200 - Invalid cast.

         * **incompatible_cast** = 31201 - Incompatible cast.

         * **unsafe** = 31300 - Unsafe operation.

         * **index_out_of_range** = 31400 - Index out of range.

         * **expecting_return_value** = 32101 - Expecting return value.

         * **not_expecting_return_value** = 32102 - Not expecting return value (void function or block).

         * **invalid_return_semantics** = 32103 - Invalid return semantics.

         * **invalid_yield** = 32104 - Invalid yield.

         * **typeinfo_reference** = 39901 - 'typeinfo' error, the type is a reference.

         * **typeinfo_auto** = 39902 - 'typeinfo' error, the type is auto.

         * **typeinfo_undefined** = 39903 - 'typeinfo' error, the type is undefined.

         * **typeinfo_dim** = 39904 - 'typeinfo' error, the type is not a static array.

         * **typeinfo_macro_error** = 39905 - Macro returned error.

         * **static_assert_failed** = 40100 - Static assert failed.

         * **run_failed** = 40101 - Run failed (attempt of folding constant function without side-effects failed)

         * **annotation_failed** = 40102 - Annotation throw panic during compile time.

         * **concept_failed** = 40103 - Concept throw panic during compile time.

         * **not_all_paths_return_value** = 40200 - Not all paths return value.

         * **assert_with_side_effects** = 40201 - Assert with side effects.

         * **only_fast_aot_no_cpp_name** = 40202 - Only fast AOT no C++ name.

         * **aot_side_effects** = 40203 - AOT side effects.

         * **no_global_heap** = 40204 - No global heap is specified, but program requests it.

         * **no_global_variables** = 40205 - No global variables are allowed in this context.

         * **unused_function_argument** = 40206 - Unused function argument.

         * **unsafe_function** = 40207 - Unsafe function.

         * **too_many_infer_passes** = 41000 - Too many infer passes.

         * **missing_node** = 50100 - Missing simulation node.


.. _enum-rtti-ConstMatters:

.. das:attribute:: ConstMatters

Yes or no flag which indicates if constant flag of the type matters (during comparison).

:Values: * **no** = 0 - const does not matter, when comparing types.

         * **yes** = 1 - const matters, when comparing types.


.. _enum-rtti-RefMatters:

.. das:attribute:: RefMatters

Yes or no flag which indicates if reference flag of the type matters (during comparison).

:Values: * **no** = 0 - Ref does not matter, when comparing types.

         * **yes** = 1 - Ref matters, when comparing types.


.. _enum-rtti-TemporaryMatters:

.. das:attribute:: TemporaryMatters

Yes or no flag which indicates if temporary flag of the type matters (during comparison).

:Values: * **no** = 0 - Temporary does not matter, when comparing types.

         * **yes** = 1 - Temporary matters, when comparing types.


.. _enum-rtti-Type:

.. das:attribute:: Type

One of the fundamental (base) types of any type object.

:Values: * **none** = 0 - No type specified (not void, not anything).

         * **autoinfer** = 1 - Auto-inferred type (auto)

         * **alias** = 2 - Type alias.

         * **option** = 3 - Optional type (foo|bar|...)

         * **typeDecl** = 4 - Type declaration typedecl(expr...)

         * **typeMacro** = 5 - Type macro. $type_macro_name(args...)

         * **fakeContext** = 6 - Fake context type (used for internal purposes to pass context as argument to C++ functions)

         * **fakeLineInfo** = 7 - Fake line info type (used for internal purposes to pass line info as argument to C++ functions)

         * **anyArgument** = 8 - Any argument type (only available for C++ interop functions, TypeInfo is provided)

         * **tVoid** = 9 - Void type.

         * **tBool** = 10 - Boolean type.

         * **tInt8** = 11 - 8-bit integer type.

         * **tUInt8** = 12 - 8-bit unsigned integer type.

         * **tInt16** = 13 - 16-bit integer type.

         * **tUInt16** = 14 - 16-bit unsigned integer type.

         * **tInt64** = 15 - 64-bit integer type.

         * **tUInt64** = 16 - 64-bit unsigned integer type.

         * **tInt** = 17 - 32-bit integer type.

         * **tInt2** = 18 - vector of 2 32-bit integers.

         * **tInt3** = 19 - vector of 3 32-bit integers.

         * **tInt4** = 20 - vector of 4 32-bit integers.

         * **tUInt** = 21 - 32-bit unsigned integer type.

         * **tUInt2** = 22 - vector of 2 32-bit unsigned integers.

         * **tUInt3** = 23 - vector of 3 32-bit unsigned integers.

         * **tUInt4** = 24 - vector of 4 32-bit unsigned integers.

         * **tFloat** = 25 - 32-bit floating point type.

         * **tFloat2** = 26 - vector of 2 32-bit floating point numbers.

         * **tFloat3** = 27 - vector of 3 32-bit floating point numbers.

         * **tFloat4** = 28 - vector of 4 32-bit floating point numbers.

         * **tDouble** = 29 - 64-bit floating point type.

         * **tRange** = 30 - 32-bit range type, similar to vector of two 32-bit integers.

         * **tURange** = 31 - 32-bit unsigned range type, similar to vector of two 32-bit unsigned integers.

         * **tRange64** = 32 - 64-bit range type, similar to vector of two 64-bit integers.

         * **tURange64** = 33 - 64-bit unsigned range type, similar to vector of two 64-bit unsigned integers.

         * **tString** = 34 - String type.

         * **tStructure** = 35 - Structure type.

         * **tHandle** = 36 - Handle type (C++ type specified via annotation).

         * **tEnumeration** = 37 - 32-bit enumeration type.

         * **tEnumeration8** = 38 - 8-bit enumeration type.

         * **tEnumeration16** = 39 - 16-bit enumeration type.

         * **tEnumeration64** = 40 - 64-bit enumeration type.

         * **tBitfield** = 41 - 32-bit bitfield type.

         * **tBitfield8** = 42 - 8-bit bitfield type.

         * **tBitfield16** = 43 - 16-bit bitfield type.

         * **tBitfield64** = 44 - 64-bit bitfield type.

         * **tPointer** = 45 - Pointer type.

         * **tFunction** = 46 - Function type.

         * **tLambda** = 47 - Lambda type.

         * **tIterator** = 48 - Iterator type.

         * **tArray** = 49 - Array type.

         * **tTable** = 50 - Table type.

         * **tBlock** = 51 - Block type.

         * **tTuple** = 52 - Tuple type.

         * **tVariant** = 53 - Variant type.


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-rtti-CodeOfPolicies:

.. das:attribute:: CodeOfPolicies

:Fields: * **aot** : bool - Object which holds compilation and simulation settings and restrictions.

         * **aot_lib** : bool - whether ahead-of-time compilation is enabled

         * **standalone_context** : bool - library??

         * **aot_module** : bool - whether paranoid validation is enabled (extra checks, no optimizations)

         * **aot_macros** : bool - whether cross-platform AOT is enabled (if not, we generate code for the current platform)

         * **paranoid_validation** : bool - whether standalone context AOT compilation is enabled

         * **cross_platform** : bool - specifies to AOT if we are compiling a module, or a final program

         * **aot_result** :  :ref:`das_string <handle-builtin-das_string>`  - enables AOT of macro code (like 'qmacro_block' etc)

         * **completion** : bool - file name for AOT output (if not set, we generate a temporary file)

         * **export_all** : bool - if we are in code completion mode

         * **serialize_main_module** : bool - export all functions and global variables

         * **keep_alive** : bool - if not, we recompile main module each time

         * **very_safe_context** : bool - keep context alive after main function

         * **always_report_candidates_threshold** : int - whether to use very safe context (delete of data is delayed, to avoid table[foo]=table[bar] lifetime bugs)

         * **max_infer_passes** : int - threshold for reporting candidates for function calls. if less than this number, we always report them

         * **stack** : uint - maximum number of inference passes

         * **intern_strings** : bool - stack size

         * **persistent_heap** : bool - whether to intern strings

         * **multiple_contexts** : bool - whether to use persistent heap (or linear heap)

         * **heap_size_hint** : uint - whether multiple contexts are allowed (pinvokes between contexts)

         * **string_heap_size_hint** : uint - heap size hint

         * **solid_context** : bool - string heap size hint

         * **macro_context_persistent_heap** : bool - whether to use solid context (global variables are cemented at locations, can't be called from other contexts via pinvoke)

         * **macro_context_collect** : bool - whether macro context uses persistent heap

         * **max_static_variables_size** : uint64 - whether macro context does garbage collection

         * **max_heap_allocated** : uint64 - maximum size of static variables

         * **max_string_heap_allocated** : uint64 - maximum heap allocated

         * **rtti** : bool - maximum string heap allocated

         * **unsafe_table_lookup** : bool - whether to enable RTTI

         * **relaxed_pointer_const** : bool - whether to allow unsafe table lookups (via [] operator)

         * **version_2_syntax** : bool - whether to relax pointer constness rules

         * **gen2_make_syntax** : bool - allows use of version 2 syntax

         * **relaxed_assign** : bool - whether to use gen2 make syntax

         * **no_unsafe** : bool - allows relaxing of the assignment rules

         * **local_ref_is_unsafe** : bool - disables all unsafe operations

         * **no_global_variables** : bool - local references are considered unsafe

         * **no_global_variables_at_all** : bool - disallows global variables in this context (except for generated)

         * **no_global_heap** : bool - disallows global variables at all in this context

         * **only_fast_aot** : bool - disallows global heap in this context

         * **aot_order_side_effects** : bool - only fast AOT, no C++ name generation

         * **no_unused_function_arguments** : bool - whether to consider side effects during AOT ordering

         * **no_unused_block_arguments** : bool - errors on unused function arguments

         * **allow_block_variable_shadowing** : bool - errors on unused block arguments

         * **allow_local_variable_shadowing** : bool - allows block variable shadowing

         * **allow_shared_lambda** : bool - allows local variable shadowing

         * **ignore_shared_modules** : bool - allows shared lambdas

         * **default_module_public** : bool - ignore shared modules during compilation

         * **no_deprecated** : bool - default module mode is public

         * **no_aliasing** : bool - disallows use of deprecated features

         * **strict_smart_pointers** : bool - disallows aliasing (if aliasing is allowed, temporary lifetimes are extended)

         * **no_init** : bool - enables strict smart pointer checks

         * **strict_unsafe_delete** : bool - disallows use of 'init' in structures

         * **no_members_functions_in_struct** : bool - enables strict unsafe delete checks

         * **no_local_class_members** : bool - disallows members functions in structures

         * **report_invisible_functions** : bool - disallows local class members

         * **report_private_functions** : bool - report invisible functions

         * **strict_properties** : bool - report private functions

         * **no_optimizations** : bool - enables strict property checks

         * **fail_on_no_aot** : bool - disables all optimizations

         * **fail_on_lack_of_aot_export** : bool - fails compilation if AOT is not available

         * **log_compile_time** : bool - fails compilation if AOT export is not available

         * **log_total_compile_time** : bool - log compile time

         * **no_fast_call** : bool - log total compile time

         * **scoped_stack_allocator** : bool - disables fast call optimization

         * **force_inscope_pod** : bool - enables debugger support

         * **log_inscope_pod** : bool - enables debug inference flag

         * **debugger** : bool - sets debug module (module which will be loaded when IDE connects)

         * **debug_infer_flag** : bool - enables profiler support

         * **debug_module** :  :ref:`das_string <handle-builtin-das_string>`  - sets profile module (module which will be loaded when profiler connects)

         * **profiler** : bool - enables JIT support

         * **profile_module** :  :ref:`das_string <handle-builtin-das_string>`  - sets JIT module (module which will be loaded when JIT is enabled)

         * **threadlock_context** : bool - enables threadlock context

         * **jit_enabled** : bool - JIT enabled - if enabled, JIT will be used to compile code at runtime. if not enabled, code will be interpreted.

         * **jit_module** :  :ref:`das_string <handle-builtin-das_string>`  - JIT module - module, loaded when -jit is specified.

         * **jit_jit_all_functions** : bool - JIT all functions - if enabled, JIT will compile all functions in the module, otherwise it will compile only functions which are called at runtime.

         * **jit_debug_info** : bool - JIT debug info - if enabled, JIT will generate debug info for JIT compiled code, which can be used by debuggers and profilers.

         * **jit_use_dll_mode** : bool - JIT opt level - optimization level for JIT compiled code (0-3)

         * **jit_output_folder** :  :ref:`das_string <handle-builtin-das_string>`  - JIT size level - size optimization level for JIT compiled code (0-3)

         * **jit_opt_level** : int - JIT dll mode - if enabled, JIT will generate DLL's into JIT output folder, and load them from there. if not enabled, JIT will generate code in memory and execute it directly.

         * **jit_size_level** : int - JIT output folder (where JIT compiled code will be stored)

         * **jit_path_to_shared_lib** :  :ref:`das_string <handle-builtin-das_string>`  - path to shared library, which is used in JIT

         * **jit_path_to_linker** :  :ref:`das_string <handle-builtin-das_string>`  - path to linker, which is used in JIT


.. _handle-rtti-FileInfo:

.. das:attribute:: FileInfo

Information about a single file stored in the `FileAccess` object.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  - File name.

         * **tabSize** : int - Tab size for this file.


.. _handle-rtti-LineInfo:

.. das:attribute:: LineInfo

Information about a section of the file stored in the `FileAccess` object.

:Fields: * **fileInfo** :  :ref:`FileInfo <handle-rtti-FileInfo>` ? - File information object.

         * **column** : uint - Column number (1-based).

         * **line** : uint - Line number (1-based).

         * **last_column** : uint - Last column number (1-based).

         * **last_line** : uint - Last line number (1-based).


.. _handle-rtti-Context:

.. das:attribute:: Context

.. _function-rtti__dot__rq_getInitSemanticHash_Context_implicit:

.. das:function:: Context implicit.getInitSemanticHash() : uint64

Property-like accessor that returns the ``uint64`` semantic hash of the initialization code for the given ``Context``, useful for detecting code changes.

.. _function-rtti__dot__rq_totalFunctions_Context_implicit:

.. das:function:: Context implicit.totalFunctions() : int

Property-like accessor that returns the total number of registered ``SimFunction`` entries in the given ``Context``.

.. _function-rtti__dot__rq_totalVariables_Context_implicit:

.. das:function:: Context implicit.totalVariables() : int

Property-like accessor that returns the total number of global variables registered in the given ``Context``.

.. _function-rtti__dot__rq_getCodeAllocatorId_Context_implicit:

.. das:function:: Context implicit.getCodeAllocatorId() : uint64

Property-like accessor that returns a non-persistent unique integer ID of the code (node) allocator associated with the given ``Context``.

:Properties: * **getInitSemanticHash** : uint64

             * **totalFunctions** : int

             * **totalVariables** : int

             * **getCodeAllocatorId** : uint64

Object which holds single Daslang Context. Context is the result of the simulation of the Daslang program.

:Fields: * **breakOnException** : bool - Calls breakpoint when exception is thrown.

         * **alwaysErrorOnException** : bool - Always error on exception.

         * **alwaysStackWalkOnException** : bool - Always stack walk on exception.

         * **name** :  :ref:`das_string <handle-builtin-das_string>`  - Context name.

         * **category** :  :ref:`context_category_flags <alias-context_category_flags>`  - Context category flags.

         * **exceptionAt** :  :ref:`LineInfo <handle-rtti-LineInfo>`  - Exception at line info.

         * **exception** : string - Exception message.

         * **last_exception** : string - Last exception message.

         * **contextMutex** :  :ref:`recursive_mutex <handle-rtti-recursive_mutex>` ? - Context mutex.


.. _handle-rtti-Error:

.. das:attribute:: Error

Object which holds information about compilation error or exception.

:Fields: * **what** :  :ref:`das_string <handle-builtin-das_string>`  - What error is about.

         * **extra** :  :ref:`das_string <handle-builtin-das_string>`  - Extra information about error.

         * **fixme** :  :ref:`das_string <handle-builtin-das_string>`  - Fixme information about error.

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  - Line info where error occurred.

         * **cerr** :  :ref:`CompilationError <enum-rtti-CompilationError>`  - Compilation error.


.. _handle-rtti-FileAccess:

.. das:attribute:: FileAccess

 Object which holds collection of files as well as means to access them (Project).


.. _handle-rtti-Module:

.. das:attribute:: Module

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  -  Collection of types, aliases, functions, classes, macros etc under a single namespace.

         * **fileName** :  :ref:`das_string <handle-builtin-das_string>`  - Module name.

         * **moduleFlags** :  :ref:`ModuleFlags <alias-ModuleFlags>`  - Module flags.


.. _handle-rtti-ModuleGroup:

.. das:attribute:: ModuleGroup

 Collection of modules.


.. _handle-rtti-AnnotationArgument:

.. das:attribute:: AnnotationArgument

Single argument of the annotation, typically part of the `AnnotationArgumentList`.

:Fields: * **basicType** :  :ref:`Type <enum-rtti-Type>`  - type of the argument value

         * **name** :  :ref:`das_string <handle-builtin-das_string>`  - argument name

         * **sValue** :  :ref:`das_string <handle-builtin-das_string>`  - string value

         * **iValue** : int - integer value

         * **fValue** : float - float value

         * **bValue** : bool - boolean value

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  - line info where the argument is defined


.. _handle-rtti-Program:

.. das:attribute:: Program

.. _function-rtti__dot__rq_getThisModule_Program_implicit:

.. das:function:: Program implicit.getThisModule() : Module?

Property-like accessor that returns the ``Module`` pointer for the module currently being inferred in the given ``Program``.

.. _function-rtti__dot__rq_getDebugger_Program_implicit:

.. das:function:: Program implicit.getDebugger() : bool

Property-like accessor that returns ``true`` if the debugger is attached and enabled for the given ``Program``.

:Properties: * **getThisModule** :  :ref:`Module <handle-rtti-Module>` ?

             * **getDebugger** : bool

Object representing full information about Daslang program during and after compilation (but not the simulated result of the program).

:Fields: * **thisNamespace** :  :ref:`das_string <handle-builtin-das_string>`  - namespace of the program

         * **thisModuleName** :  :ref:`das_string <handle-builtin-das_string>`  - module name

         * **totalFunctions** : int - total number of functions in the program

         * **totalVariables** : int - total number of variables in the program

         * **errors** : vector<Error> - compilation errors and exceptions

         * **globalStringHeapSize** : uint - size of the global string heap

         * **initSemanticHashWithDep** : uint64 - initial semantic hash with dependencies

         * **flags** :  :ref:`ProgramFlags <alias-ProgramFlags>`  - program flags

         * **_options** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  - program options

         * **policies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  - code of policies


.. _handle-rtti-Annotation:

.. das:attribute:: Annotation

.. _function-rtti__dot__rq_isTypeAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isTypeAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is a ``TypeAnnotation`` (defines a handled type).

.. _function-rtti__dot__rq_isBasicStructureAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isBasicStructureAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is a ``BasicStructureAnnotation``, which exposes C++ struct fields to daScript.

.. _function-rtti__dot__rq_isStructureAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isStructureAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is a structure annotation (applied to struct declarations).

.. _function-rtti__dot__rq_isStructureTypeAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isStructureTypeAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is a ``StructureTypeAnnotation``, which binds a C++ class as a daScript handled struct.

.. _function-rtti__dot__rq_isFunctionAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isFunctionAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is a ``FunctionAnnotation`` (applied to functions).

.. _function-rtti__dot__rq_isEnumerationAnnotation_Annotation_implicit:

.. das:function:: Annotation implicit.isEnumerationAnnotation() : bool

Property-like accessor that returns ``true`` if the given ``Annotation`` is an ``EnumerationAnnotation``.

:Properties: * **isTypeAnnotation** : bool

             * **isBasicStructureAnnotation** : bool

             * **isStructureAnnotation** : bool

             * **isStructureTypeAnnotation** : bool

             * **isFunctionAnnotation** : bool

             * **isEnumerationAnnotation** : bool

Handled type or macro.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  - name of the annotation

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>`  - name of the associated C++ type or macro

         * **_module** :  :ref:`Module <handle-rtti-Module>` ? - module where the annotation is defined


.. _handle-rtti-AnnotationDeclaration:

.. das:attribute:: AnnotationDeclaration

Annotation declaration, its location, and arguments.

:Fields: * **annotation** : smart_ptr< :ref:`Annotation <handle-rtti-Annotation>` > - pointer to the handled annotation object.

         * **arguments** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  - list of annotation arguments.

         * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>`  - line information where the annotation is defined.

         * **flags** :  :ref:`AnnotationDeclarationFlags <alias-AnnotationDeclarationFlags>`  - flags associated with the annotation.


.. _handle-rtti-TypeAnnotation:

.. das:attribute:: TypeAnnotation

.. _function-rtti__dot__rq_is_any_vector_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.is_any_vector() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` wraps any C++ vector-like container (e.g., ``std::vector``).

.. _function-rtti__dot__rq_canMove_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canMove() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports move semantics.

.. _function-rtti__dot__rq_canCopy_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canCopy() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports copy semantics.

.. _function-rtti__dot__rq_canClone_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canClone() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports the clone operation.

.. _function-rtti__dot__rq_isPod_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isPod() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is a POD (plain old data) type — no constructor, destructor, or special semantics.

.. _function-rtti__dot__rq_isRawPod_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isRawPod() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is a raw POD type — a basic value type excluding pointers and strings.

.. _function-rtti__dot__rq_isRefType_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isRefType() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is always passed by reference, or is itself a reference type.

.. _function-rtti__dot__rq_hasNonTrivialCtor_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.hasNonTrivialCtor() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` has a non-trivial constructor (requires explicit initialization).

.. _function-rtti__dot__rq_hasNonTrivialDtor_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.hasNonTrivialDtor() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` has a non-trivial destructor (requires explicit finalization).

.. _function-rtti__dot__rq_hasNonTrivialCopy_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.hasNonTrivialCopy() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` has non-trivial copy semantics (i.e., a custom copy constructor).

.. _function-rtti__dot__rq_canBePlacedInContainer_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canBePlacedInContainer() : bool

Property-like accessor that returns ``true`` if values of the given ``TypeAnnotation`` can be stored inside arrays, tables, or other containers.

.. _function-rtti__dot__rq_isLocal_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isLocal() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` can be used as a local variable type within a function.

.. _function-rtti__dot__rq_canNew_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canNew() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports heap allocation via ``new``.

.. _function-rtti__dot__rq_canDelete_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canDelete() : bool

Property-like accessor that returns ``true`` if values of the given ``TypeAnnotation`` can be explicitly deleted.

.. _function-rtti__dot__rq_needDelete_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.needDelete() : bool

Property-like accessor that returns ``true`` if values of the given ``TypeAnnotation`` require explicit ``delete`` to free resources.

.. _function-rtti__dot__rq_canDeletePtr_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.canDeletePtr() : bool

Property-like accessor that returns ``true`` if a pointer to the given ``TypeAnnotation`` type can be explicitly deleted.

.. _function-rtti__dot__rq_isIterable_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isIterable() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports iteration via ``for``.

.. _function-rtti__dot__rq_isShareable_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isShareable() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` can be shared across multiple ``Context`` objects.

.. _function-rtti__dot__rq_isSmart_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.isSmart() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` represents a ``smart_ptr`` managed type.

.. _function-rtti__dot__rq_avoidNullPtr_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.avoidNullPtr() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` requires pointers to its type to be non-null (i.e., must be initialized on creation).

.. _function-rtti__dot__rq_sizeOf_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.sizeOf() : uint64

Property-like accessor that returns the size in bytes of the type described by the given ``TypeAnnotation``.

.. _function-rtti__dot__rq_alignOf_TypeAnnotation_implicit:

.. das:function:: TypeAnnotation implicit.alignOf() : uint64

Property-like accessor that returns the memory alignment requirement (in bytes) of the type described by the given ``TypeAnnotation``.

:Properties: * **is_any_vector** : bool

             * **canMove** : bool

             * **canCopy** : bool

             * **canClone** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isRefType** : bool

             * **hasNonTrivialCtor** : bool

             * **hasNonTrivialDtor** : bool

             * **hasNonTrivialCopy** : bool

             * **canBePlacedInContainer** : bool

             * **isLocal** : bool

             * **canNew** : bool

             * **canDelete** : bool

             * **needDelete** : bool

             * **canDeletePtr** : bool

             * **isIterable** : bool

             * **isShareable** : bool

             * **isSmart** : bool

             * **avoidNullPtr** : bool

             * **sizeOf** : uint64

             * **alignOf** : uint64

Handled type.

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  - name of the type annotation

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>`  - name of the associated C++ type

         * **_module** :  :ref:`Module <handle-rtti-Module>` ? - module where the annotation is defined


.. _handle-rtti-BasicStructureAnnotation:

.. das:attribute:: BasicStructureAnnotation

.. _function-rtti__dot__rq_fieldCount_BasicStructureAnnotation_implicit:

.. das:function:: BasicStructureAnnotation implicit.fieldCount() : int

Property-like accessor that returns the number of fields declared in the given ``BasicStructureAnnotation``.

:Properties: * **fieldCount** : int

Handled type which represents structure-like object.
name of the structure
module where the structure is defined
list of fields in the structure
hash of the structure
hash of the structure initializer
flags associated with the structure
number of fields in the structure

:Fields: * **name** :  :ref:`das_string <handle-builtin-das_string>`  - size of the structure in bytes

         * **cppName** :  :ref:`das_string <handle-builtin-das_string>`  - index of the first GC field, i.e. field which requires garbage collection marking


.. _handle-rtti-EnumValueInfo:

.. das:attribute:: EnumValueInfo

Single element of enumeration, its name and value.

:Fields: * **name** : string - name of the enumeration value

         * **value** : int64 - value of the enumeration value


.. _handle-rtti-EnumInfo:

.. das:attribute:: EnumInfo

Type object which represents enumeration.

:Fields: * **name** : string - name of the enumeration

         * **module_name** : string - module where the enumeration is defined

         * **fields** :  :ref:`EnumValueInfo <handle-rtti-EnumValueInfo>` ?? - fields in the enumeration

         * **count** : uint - number of fields in the enumeration

         * **hash** : uint64 - hash of the enumeration


.. _handle-rtti-StructInfo:

.. das:attribute:: StructInfo

Type object which represents structure or class.

:Fields: * **name** : string - name of the structure

         * **module_name** : string - module where the structure is defined

         * **fields** :  :ref:`VarInfo <handle-rtti-VarInfo>` ?? - fields in the structure

         * **hash** : uint64 - hash of the structure

         * **init_mnh** : uint64 - hash of the structure initializer

         * **flags** :  :ref:`StructInfoFlags <alias-StructInfoFlags>`  - flags associated with the structure

         * **count** : uint - number of fields in the structure

         * **size** : uint - size of the structure in bytes

         * **firstGcField** : uint - index of the first GC field in the structure, i.e. field which requires garbage collection marking


.. _handle-rtti-TypeInfo:

.. das:attribute:: TypeInfo

.. _function-rtti__dot__rq_enumType_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.enumType() : EnumInfo?

Property-like accessor that returns the ``EnumInfo`` pointer describing the underlying enumeration for the given enum ``TypeAnnotation``.

.. _function-rtti__dot__rq_isRef_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isRef() : bool

Property-like accessor that returns ``true`` if the given ``TypeInfo`` describes a reference (``&``) type.

.. _function-rtti__dot__rq_isRefType_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isRefType() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is always passed by reference, or is itself a reference type.

.. _function-rtti__dot__rq_isRefValue_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isRefValue() : bool

Property-like accessor that returns ``true`` if the given ``TypeInfo`` describes a ref-value type (boxed value accessed by reference).

.. _function-rtti__dot__rq_canCopy_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.canCopy() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` supports copy semantics.

.. _function-rtti__dot__rq_isPod_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isPod() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is a POD (plain old data) type — no constructor, destructor, or special semantics.

.. _function-rtti__dot__rq_isRawPod_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isRawPod() : bool

Property-like accessor that returns ``true`` if the given ``TypeAnnotation`` is a raw POD type — a basic value type excluding pointers and strings.

.. _function-rtti__dot__rq_isConst_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isConst() : bool

Property-like accessor that returns ``true`` if the given ``TypeInfo`` describes a ``const``-qualified type.

.. _function-rtti__dot__rq_isTemp_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isTemp() : bool

Property-like accessor that returns ``true`` if the given ``TypeInfo`` describes a temporary (``#``) type that cannot be captured or stored.

.. _function-rtti__dot__rq_isImplicit_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.isImplicit() : bool

Property-like accessor that returns ``true`` if the given ``TypeInfo`` describes an implicit (compiler-inferred) type.

.. _function-rtti__dot__rq_annotation_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.annotation() : TypeAnnotation?

Property-like accessor that returns the ``Annotation`` pointer associated with the given ``TypeInfo``.

.. _function-rtti__dot__rq_annotation_or_name_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.annotation_or_name() : TypeAnnotation?

Property-like accessor that returns the annotation name if one exists, otherwise returns the raw type name from the given ``TypeInfo``.

.. _function-rtti__dot__rq_structType_TypeInfo_implicit:

.. das:function:: TypeInfo implicit.structType() : StructInfo?

Property-like accessor that returns the ``StructInfo`` pointer for the struct described by the given ``TypeInfo``, or null if not a struct type.

:Properties: * **enumType** :  :ref:`EnumInfo <handle-rtti-EnumInfo>` ?

             * **isRef** : bool

             * **isRefType** : bool

             * **isRefValue** : bool

             * **canCopy** : bool

             * **isPod** : bool

             * **isRawPod** : bool

             * **isConst** : bool

             * **isTemp** : bool

             * **isImplicit** : bool

             * **annotation** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ?

             * **annotation_or_name** :  :ref:`TypeAnnotation <handle-rtti-TypeAnnotation>` ?

             * **structType** :  :ref:`StructInfo <handle-rtti-StructInfo>` ?

Object which represents any Daslang type.

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - first type parameter

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - second type parameter

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?? - argument types list

         * **argNames** : string? - argument names list

         * **dim** : uint? - dimensions list

         * **hash** : uint64 - hash of the type

         * **_type** :  :ref:`Type <enum-rtti-Type>`  - kind of the type

         * **basicType** :  :ref:`Type <enum-rtti-Type>`  - basic type category

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>`  - flags associated with the type

         * **size** : uint - size of the type in bytes

         * **argCount** : uint - number of arguments in the type

         * **dimSize** : uint - number of dimensions in the type


.. _handle-rtti-VarInfo:

.. das:attribute:: VarInfo

Object which represents variable declaration.

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - first type parameter

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - second type parameter

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?? - argument types list

         * **argNames** : string? - argument names list

         * **hash** : uint64 - hash of the type

         * **basicType** :  :ref:`Type <enum-rtti-Type>`  - basic type category

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>`  - flags associated with the type

         * **size** : uint - size of the type in bytes

         * **argCount** : uint - number of arguments in the type

         * **dimSize** : uint - number of dimensions in the type

         * **value** : any - value of the variable

         * **sValue** : string - string value of the variable

         * **name** : string - name of the variable

         * **annotation_arguments** :  :ref:`AnnotationArguments <handle-rtti-AnnotationArguments>` ? - annotation arguments

         * **offset** : uint - offset of the variable in the structure

         * **nextGcField** : uint - next garbage collection field in the structure, which requires garbage collection marking


.. _handle-rtti-LocalVariableInfo:

.. das:attribute:: LocalVariableInfo

Object which represents local variable declaration.

:Fields: * **firstType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - first type parameter

         * **secondType** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - second type parameter

         * **argTypes** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ?? - argument types list

         * **argNames** : string? - argument names list

         * **hash** : uint64 - hash of the type

         * **basicType** :  :ref:`Type <enum-rtti-Type>`  - basic type category

         * **flags** :  :ref:`TypeInfoFlags <alias-TypeInfoFlags>`  - flags associated with the type

         * **size** : uint - size of the type in bytes

         * **argCount** : uint - number of arguments in the type

         * **dimSize** : uint - number of dimensions in the type

         * **visibility** :  :ref:`LineInfo <handle-rtti-LineInfo>`  - visibility information

         * **name** : string - name of the variable

         * **stackTop** : uint - stack top offset

         * **localFlags** :  :ref:`LocalVariableInfoFlags <alias-LocalVariableInfoFlags>`  - local variable flags


.. _handle-rtti-FuncInfo:

.. das:attribute:: FuncInfo

Object which represents function declaration.

:Fields: * **name** : string - function name

         * **cppName** : string - C++ name (for the builtin functions)

         * **fields** :  :ref:`VarInfo <handle-rtti-VarInfo>` ?? - function parameters

         * **result** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? - function result type

         * **locals** :  :ref:`LocalVariableInfo <handle-rtti-LocalVariableInfo>` ?? - local variables (with visibility info)

         * **globals** :  :ref:`VarInfo <handle-rtti-VarInfo>` ?? - accessed global variables

         * **hash** : uint64 - hash of the function

         * **flags** : uint - flags associated with the function

         * **count** : uint - number of arguments in the function

         * **stackSize** : uint - stack size in bytes

         * **localCount** : uint - number of local variables

         * **globalCount** : uint - number of accessed global variables


.. _handle-rtti-SimFunction:

.. das:attribute:: SimFunction

.. _function-rtti__dot__rq_lineInfo_SimFunction_implicit:

.. das:function:: SimFunction implicit.lineInfo() : LineInfo const?

Property-like accessor that returns the ``LineInfo`` (source location) associated with the given function's ``FuncInfo``.

:Properties: * **lineInfo** :  :ref:`LineInfo <handle-rtti-LineInfo>` ?

Object which represents simulated function in the `Context`.

:Fields: * **name** : string - original function name

         * **mangledName** : string - mangled function name (with arguments and types encoded)

         * **debugInfo** :  :ref:`FuncInfo <handle-rtti-FuncInfo>` ? - pointer to the function debug info, if available

         * **mangledNameHash** : uint64 - hash of the mangled function name

         * **stackSize** : uint - stack size in bytes

         * **flags** :  :ref:`SimFunctionFlags <alias-SimFunctionFlags>`  - flags associated with the function


.. _handle-rtti-DebugInfoHelper:

.. das:attribute:: DebugInfoHelper

:Fields: * **rtti** : bool -  Helper object which holds debug information about the simulated program.


+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-rtti-rtti_typeinfo:

.. das:attribute:: rtti_typeinfo

Typeinfo macro that provides compile-time access to RTTI type information structures.
Typeinfo macro rtti_typeinfo

+++++++++++++
Handled types
+++++++++++++

.. _handle-rtti-recursive_mutex:

.. das:attribute:: recursive_mutex

Handled type wrapping a system ``std::recursive_mutex``, used with ``lock_mutex`` for thread-safe access to shared data across contexts.
.. _handle-rtti-AnnotationArguments:

.. das:attribute:: AnnotationArguments

Handled type representing a collection of annotation arguments, typically the raw argument list parsed from an annotation declaration.
.. _handle-rtti-AnnotationArgumentList:

.. das:attribute:: AnnotationArgumentList

Handled type representing an ordered list of annotation arguments and properties, providing indexed and named access to argument entries.
.. _handle-rtti-AnnotationList:

.. das:attribute:: AnnotationList

Handled type representing all annotations attached to a single object (function, structure, or variable), iterable via ``each``.
+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`CodeOfPolicies () : CodeOfPolicies <function-rtti_CodeOfPolicies>` 
  *  :ref:`using (arg0: block\<(CodeOfPolicies):void\>) <function-rtti_using_block_ls_CodeOfPolicies_c_void_gr_>` 
  *  :ref:`LineInfo () : LineInfo <function-rtti_LineInfo>` 
  *  :ref:`LineInfo (arg0: FileInfo? implicit; arg1: int; arg2: int; arg3: int; arg4: int) : LineInfo <function-rtti_LineInfo_FileInfo_q__implicit_int_int_int_int>` 
  *  :ref:`using (arg0: block\<(recursive_mutex):void\>) <function-rtti_using_block_ls_recursive_mutex_c_void_gr_>` 
  *  :ref:`using (arg0: block\<(ModuleGroup):void\>) <function-rtti_using_block_ls_ModuleGroup_c_void_gr_>` 
  *  :ref:`RttiValue_nothing () : auto <function-rtti_RttiValue_nothing>` 

.. _function-rtti_CodeOfPolicies:

.. das:function:: CodeOfPolicies() : CodeOfPolicies

Constructs a default-initialized ``CodeOfPolicies`` structure, which controls compiler behavior and optimization settings.

.. _function-rtti_using_block_ls_CodeOfPolicies_c_void_gr_:

.. das:function:: using(arg0: block<(CodeOfPolicies):void>)

Creates a temporary RTTI helper object (e.g., ``Program``, ``DebugInfoHelper``) scoped to the given block, automatically finalized on block exit.

:Arguments: * **arg0** : block<( :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>` ):void> implicit

.. _function-rtti_LineInfo:

.. das:function:: LineInfo() : LineInfo

Constructs a default-initialized ``LineInfo`` structure representing source file location (file, line, column).

.. _function-rtti_LineInfo_FileInfo_q__implicit_int_int_int_int:

.. das:function:: LineInfo(arg0: FileInfo? implicit; arg1: int; arg2: int; arg3: int; arg4: int) : LineInfo

Constructs a default-initialized ``LineInfo`` structure representing source file location (file, line, column).

:Arguments: * **arg0** :  :ref:`FileInfo <handle-rtti-FileInfo>` ? implicit

            * **arg1** : int

            * **arg2** : int

            * **arg3** : int

            * **arg4** : int

.. _function-rtti_using_block_ls_recursive_mutex_c_void_gr_:

.. das:function:: using(arg0: block<(recursive_mutex):void>)

Creates a temporary RTTI helper object (e.g., ``Program``, ``DebugInfoHelper``) scoped to the given block, automatically finalized on block exit.

:Arguments: * **arg0** : block<( :ref:`recursive_mutex <handle-rtti-recursive_mutex>` ):void> implicit

.. _function-rtti_using_block_ls_ModuleGroup_c_void_gr_:

.. das:function:: using(arg0: block<(ModuleGroup):void>)

Creates a temporary RTTI helper object (e.g., ``Program``, ``DebugInfoHelper``) scoped to the given block, automatically finalized on block exit.

:Arguments: * **arg0** : block<( :ref:`ModuleGroup <handle-rtti-ModuleGroup>` ):void> implicit

.. _function-rtti_RttiValue_nothing:

.. das:function:: RttiValue_nothing() : auto

Constructs an ``RttiValue`` variant set to the ``nothing`` alternative, representing an absent or void value.

+++++++++++
Type access
+++++++++++

  *  :ref:`get_dim (typeinfo: TypeInfo implicit; index: int) : int <function-rtti_get_dim_TypeInfo_implicit_int>` 
  *  :ref:`get_dim (typeinfo: VarInfo implicit; index: int) : int <function-rtti_get_dim_VarInfo_implicit_int>` 
  *  :ref:`builtin_is_same_type (a: TypeInfo const? implicit; b: TypeInfo const? implicit; refMatters: RefMatters; cosntMatters: ConstMatters; tempMatters: TemporaryMatters; topLevel: bool) : bool <function-rtti_builtin_is_same_type_TypeInfo_const_q__implicit_TypeInfo_const_q__implicit_RefMatters_ConstMatters_TemporaryMatters_bool>` 
  *  :ref:`get_type_size (type: TypeInfo? implicit) : int <function-rtti_get_type_size_TypeInfo_q__implicit>` 
  *  :ref:`get_type_align (type: TypeInfo? implicit) : int <function-rtti_get_type_align_TypeInfo_q__implicit>` 
  *  :ref:`is_compatible_cast (from: StructInfo const? implicit; to: StructInfo const? implicit) : bool <function-rtti_is_compatible_cast_StructInfo_const_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`get_das_type_name (type: Type) : string <function-rtti_get_das_type_name_Type>` 
  *  :ref:`is_same_type (a: TypeInfo; b: TypeInfo; refMatters: RefMatters = RefMatters.yes; constMatters: ConstMatters = ConstMatters.yes; temporaryMatters: TemporaryMatters = TemporaryMatters.yes; topLevel: bool = true) : auto <function-rtti_is_same_type_TypeInfo_TypeInfo_RefMatters_ConstMatters_TemporaryMatters_bool>` 
  *  :ref:`is_compatible_cast (a: StructInfo; b: StructInfo) : auto <function-rtti_is_compatible_cast_StructInfo_StructInfo>` 
  *  :ref:`each_dim (info: TypeInfo) : auto <function-rtti_each_dim_TypeInfo>` 
  *  :ref:`each_dim (info: VarInfo) : auto <function-rtti_each_dim_VarInfo>` 
  *  :ref:`arg_types (info: TypeInfo) : auto <function-rtti_arg_types_TypeInfo>` 
  *  :ref:`arg_types (info: VarInfo) : auto <function-rtti_arg_types_VarInfo>` 
  *  :ref:`arg_names (info: TypeInfo) : auto <function-rtti_arg_names_TypeInfo>` 
  *  :ref:`arg_names (info: VarInfo) : auto <function-rtti_arg_names_VarInfo>` 

.. _function-rtti_get_dim_TypeInfo_implicit_int:

.. das:function:: get_dim(typeinfo: TypeInfo implicit; index: int) : int

Returns the dimension size (``int``) at the specified index for a fixed-size array type described by ``TypeInfo``.

:Arguments: * **typeinfo** :  :ref:`TypeInfo <handle-rtti-TypeInfo>`  implicit

            * **index** : int

.. _function-rtti_get_dim_VarInfo_implicit_int:

.. das:function:: get_dim(typeinfo: VarInfo implicit; index: int) : int

Returns the dimension size (``int``) at the specified index for a fixed-size array type described by ``TypeInfo``.

:Arguments: * **typeinfo** :  :ref:`VarInfo <handle-rtti-VarInfo>`  implicit

            * **index** : int

.. _function-rtti_builtin_is_same_type_TypeInfo_const_q__implicit_TypeInfo_const_q__implicit_RefMatters_ConstMatters_TemporaryMatters_bool:

.. das:function:: builtin_is_same_type(a: TypeInfo const? implicit; b: TypeInfo const? implicit; refMatters: RefMatters; cosntMatters: ConstMatters; tempMatters: TemporaryMatters; topLevel: bool) : bool

Returns ``true`` if two ``TypeInfo`` pointers describe the same type, with flags controlling whether ref, const, temp, and other qualifiers are included in the comparison.

:Arguments: * **a** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **b** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **cosntMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **tempMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

            * **topLevel** : bool

.. _function-rtti_get_type_size_TypeInfo_q__implicit:

.. das:function:: get_type_size(type: TypeInfo? implicit) : int

Returns the size (``int``, in bytes) of the type described by the given ``TypeInfo``.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-rtti_get_type_align_TypeInfo_q__implicit:

.. das:function:: get_type_align(type: TypeInfo? implicit) : int

Returns the memory alignment (``int``, in bytes) of the type described by the given ``TypeInfo``.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-rtti_is_compatible_cast_StructInfo_const_q__implicit_StructInfo_const_q__implicit:

.. das:function:: is_compatible_cast(from: StructInfo const? implicit; to: StructInfo const? implicit) : bool

Returns ``true`` if an object of type ``from`` (``StructInfo``) can be safely cast to type ``to`` (``StructInfo``), following the class hierarchy.

:Arguments: * **from** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

            * **to** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-rtti_get_das_type_name_Type:

.. das:function:: get_das_type_name(type: Type) : string

Returns the canonical ``string`` name of the given ``Type`` enumeration value (e.g., ``tInt`` → ``"int"``).

:Arguments: * **type** :  :ref:`Type <enum-rtti-Type>` 

.. _function-rtti_is_same_type_TypeInfo_TypeInfo_RefMatters_ConstMatters_TemporaryMatters_bool:

.. das:function:: is_same_type(a: TypeInfo; b: TypeInfo; refMatters: RefMatters = RefMatters.yes; constMatters: ConstMatters = ConstMatters.yes; temporaryMatters: TemporaryMatters = TemporaryMatters.yes; topLevel: bool = true) : auto

Returns ``true`` if two ``TypeInfo`` objects describe the same type, with flags controlling comparison of qualifiers (ref, const, temp, etc.).

:Arguments: * **a** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

            * **b** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

            * **refMatters** :  :ref:`RefMatters <enum-rtti-RefMatters>` 

            * **constMatters** :  :ref:`ConstMatters <enum-rtti-ConstMatters>` 

            * **temporaryMatters** :  :ref:`TemporaryMatters <enum-rtti-TemporaryMatters>` 

            * **topLevel** : bool

.. _function-rtti_is_compatible_cast_StructInfo_StructInfo:

.. das:function:: is_compatible_cast(a: StructInfo; b: StructInfo) : auto

Returns ``true`` if an object of type ``from`` (``StructInfo``) can be safely cast to type ``to`` (``StructInfo``), following the class hierarchy.

:Arguments: * **a** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

            * **b** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

.. _function-rtti_each_dim_TypeInfo:

.. das:function:: each_dim(info: TypeInfo) : auto

Iterates through the dimension sizes of a fixed-size array ``TypeInfo``, yielding each ``int`` dimension value (e.g., ``int[3][4]`` yields 3 then 4).

:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-rtti_each_dim_VarInfo:

.. das:function:: each_dim(info: VarInfo) : auto

Iterates through the dimension sizes of a fixed-size array ``TypeInfo``, yielding each ``int`` dimension value (e.g., ``int[3][4]`` yields 3 then 4).

:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

.. _function-rtti_arg_types_TypeInfo:

.. das:function:: arg_types(info: TypeInfo) : auto

Iterates through the argument types of an RTTI type, yielding each element as a ``TypeInfo`` pointer — used for inspecting function or call-site parameter types.

:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-rtti_arg_types_VarInfo:

.. das:function:: arg_types(info: VarInfo) : auto

Iterates through the argument types of an RTTI type, yielding each element as a ``TypeInfo`` pointer — used for inspecting function or call-site parameter types.

:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

.. _function-rtti_arg_names_TypeInfo:

.. das:function:: arg_names(info: TypeInfo) : auto

Iterates through the argument names of an RTTI type, yielding each name as a ``string`` — used for inspecting function or call-site parameter names.

:Arguments: * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` 

.. _function-rtti_arg_names_VarInfo:

.. das:function:: arg_names(info: VarInfo) : auto

Iterates through the argument names of an RTTI type, yielding each name as a ``string`` — used for inspecting function or call-site parameter names.

:Arguments: * **info** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

+++++++++++++++++++
Rtti context access
+++++++++++++++++++

  *  :ref:`get_total_functions (context: Context implicit) : int <function-rtti_get_total_functions_Context_implicit>` 
  *  :ref:`get_total_variables (context: Context implicit) : int <function-rtti_get_total_variables_Context_implicit>` 
  *  :ref:`get_function_info (context: any; index: int) : FuncInfo <function-rtti_get_function_info_any_int>` 
  *  :ref:`get_variable_info (context: any; index: int) : VarInfo <function-rtti_get_variable_info_any_int>` 
  *  :ref:`get_variable_value (varInfo: VarInfo implicit) : RttiValue <function-rtti_get_variable_value_VarInfo_implicit>` 
  *  :ref:`get_function_info (context: Context implicit; function: function\<():void\>) : FuncInfo const? <function-rtti_get_function_info_Context_implicit_function_ls__c_void_gr_>` 
  *  :ref:`get_function_by_mnh (context: Context implicit; MNH: uint64) : function\<():void\> <function-rtti_get_function_by_mnh_Context_implicit_uint64>` 
  *  :ref:`get_line_info () : LineInfo <function-rtti_get_line_info>` 
  *  :ref:`get_line_info (depth: int) : LineInfo <function-rtti_get_line_info_int>` 
  *  :ref:`this_context () : Context& <function-rtti_this_context>` 
  *  :ref:`context_for_each_function (blk: block\<(info:FuncInfo):void\>) : auto <function-rtti_context_for_each_function_block_ls_info_c_FuncInfo_c_void_gr_>` 
  *  :ref:`context_for_each_variable (blk: block\<(info:VarInfo):void\>) : auto <function-rtti_context_for_each_variable_block_ls_info_c_VarInfo_c_void_gr_>` 
  *  :ref:`class_info (cl: auto) : StructInfo const? <function-rtti_class_info_auto>` 
  *  :ref:`type_info (cl: auto) : TypeInfo const? <function-rtti_type_info_auto>` 
  *  :ref:`type_info (vinfo: LocalVariableInfo) : TypeInfo const? <function-rtti_type_info_LocalVariableInfo>` 
  *  :ref:`type_info (vinfo: VarInfo) : TypeInfo const? <function-rtti_type_info_VarInfo>` 

.. _function-rtti_get_total_functions_Context_implicit:

.. das:function:: get_total_functions(context: Context implicit) : int

Returns the total number of registered functions (``int``) in the given ``Context``.

:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-rtti_get_total_variables_Context_implicit:

.. das:function:: get_total_variables(context: Context implicit) : int

Returns the total number of global variables (``int``) in the given ``Context``.

:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-rtti_get_function_info_any_int:

.. das:function:: get_function_info(context: any; index: int) : FuncInfo

Returns the ``FuncInfo`` pointer for a function at the given index in the ``Context``, providing access to its name, arguments, and return type.

:Arguments: * **context** : any

            * **index** : int

.. _function-rtti_get_variable_info_any_int:

.. das:function:: get_variable_info(context: any; index: int) : VarInfo

Returns the ``VarInfo`` pointer for a global variable at the given index in the ``Context``, providing access to its name, type, and offset.

:Arguments: * **context** : any

            * **index** : int

.. _function-rtti_get_variable_value_VarInfo_implicit:

.. das:function:: get_variable_value(varInfo: VarInfo implicit) : RttiValue

Returns an ``RttiValue`` variant representing the current value of a global variable, looked up by ``VarInfo`` in the given ``Context``.

:Arguments: * **varInfo** :  :ref:`VarInfo <handle-rtti-VarInfo>`  implicit

.. _function-rtti_get_function_info_Context_implicit_function_ls__c_void_gr_:

.. das:function:: get_function_info(context: Context implicit; function: function<():void>) : FuncInfo const?

Returns the ``FuncInfo`` pointer for a function at the given index in the ``Context``, providing access to its name, arguments, and return type.

:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **function** : function<void>

.. _function-rtti_get_function_by_mnh_Context_implicit_uint64:

.. das:function:: get_function_by_mnh(context: Context implicit; MNH: uint64) : function<():void>

Returns a ``SimFunction`` pointer looked up by mangled name hash — an alternative form of ``get_function_address``.

:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **MNH** : uint64

.. _function-rtti_get_line_info:

.. das:function:: get_line_info() : LineInfo

Returns a ``LineInfo`` structure representing the source location (file, line, column) of the call site where ``get_line_info`` is invoked.

.. _function-rtti_get_line_info_int:

.. das:function:: get_line_info(depth: int) : LineInfo

Returns a ``LineInfo`` structure representing the source location (file, line, column) of the call site where ``get_line_info`` is invoked.

:Arguments: * **depth** : int

.. _function-rtti_this_context:

.. das:function:: this_context() : Context&

Returns a pointer to the current ``Context`` in which the calling code is executing.

.. _function-rtti_context_for_each_function_block_ls_info_c_FuncInfo_c_void_gr_:

.. das:function:: context_for_each_function(blk: block<(info:FuncInfo):void>) : auto

Iterates through all functions in the given ``Context``, yielding a ``FuncInfo`` pointer for each registered function.

:Arguments: * **blk** : block<(info: :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void>

.. _function-rtti_context_for_each_variable_block_ls_info_c_VarInfo_c_void_gr_:

.. das:function:: context_for_each_variable(blk: block<(info:VarInfo):void>) : auto

Iterates through all global variables in the given ``Context``, yielding a ``VarInfo`` pointer for each registered variable.

:Arguments: * **blk** : block<(info: :ref:`VarInfo <handle-rtti-VarInfo>` ):void>

.. _function-rtti_class_info_auto:

.. das:function:: class_info(cl: auto) : StructInfo const?

Returns a ``StructInfo`` pointer for the given class instance, enabling runtime introspection of its fields and annotations via RTTI.

:Arguments: * **cl** : auto

.. _function-rtti_type_info_auto:

.. das:function:: type_info(cl: auto) : TypeInfo const?

Returns the ``TypeInfo`` object for the specified local variable or expression, resolved at compile time via the ``[typeinfo(...)]`` macro.

:Arguments: * **cl** : auto

.. _function-rtti_type_info_LocalVariableInfo:

.. das:function:: type_info(vinfo: LocalVariableInfo) : TypeInfo const?

Returns the ``TypeInfo`` object for the specified local variable or expression, resolved at compile time via the ``[typeinfo(...)]`` macro.

:Arguments: * **vinfo** :  :ref:`LocalVariableInfo <handle-rtti-LocalVariableInfo>` 

.. _function-rtti_type_info_VarInfo:

.. das:function:: type_info(vinfo: VarInfo) : TypeInfo const?

Returns the ``TypeInfo`` object for the specified local variable or expression, resolved at compile time via the ``[typeinfo(...)]`` macro.

:Arguments: * **vinfo** :  :ref:`VarInfo <handle-rtti-VarInfo>` 

++++++++++++++
Program access
++++++++++++++

  *  :ref:`get_this_module (program: smart_ptr\<Program\> implicit) : Module? <function-rtti_get_this_module_smart_ptr_ls_Program_gr__implicit>` 
  *  :ref:`get_module (name: string implicit) : Module? <function-rtti_get_module_string_implicit>` 
  *  :ref:`program_for_each_module (program: smart_ptr\<Program\> implicit; block: block\<(Module?):void\>) <function-rtti_program_for_each_module_smart_ptr_ls_Program_gr__implicit_block_ls_Module_q__c_void_gr_>` 
  *  :ref:`program_for_each_registered_module (block: block\<(Module?):void\>) <function-rtti_program_for_each_registered_module_block_ls_Module_q__c_void_gr_>` 

.. _function-rtti_get_this_module_smart_ptr_ls_Program_gr__implicit:

.. das:function:: get_this_module(program: smart_ptr<Program> implicit) : Module?

Returns the ``Module`` pointer for the module currently being compiled or inferred, retrieved from the ``Program``.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

.. _function-rtti_get_module_string_implicit:

.. das:function:: get_module(name: string implicit) : Module?

Returns a ``Module`` pointer looked up by module name ``string``, or null if no such module is registered.

:Arguments: * **name** : string implicit

.. _function-rtti_program_for_each_module_smart_ptr_ls_Program_gr__implicit_block_ls_Module_q__c_void_gr_:

.. das:function:: program_for_each_module(program: smart_ptr<Program> implicit; block: block<(Module?):void>)

Iterates through all modules referenced by the given ``Program`` (including transitive dependencies), yielding a ``Module`` pointer for each.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

.. _function-rtti_program_for_each_registered_module_block_ls_Module_q__c_void_gr_:

.. das:function:: program_for_each_registered_module(block: block<(Module?):void>)

Iterates through all modules registered in the daScript runtime (globally, not per-program), yielding a ``Module`` pointer for each.

:Arguments: * **block** : block<( :ref:`Module <handle-rtti-Module>` ?):void> implicit

+++++++++++++
Module access
+++++++++++++

  *  :ref:`module_for_each_dependency (module: Module? implicit; block: block\<(Module?;bool):void\>) <function-rtti_module_for_each_dependency_Module_q__implicit_block_ls_Module_q_;bool_c_void_gr_>` 
  *  :ref:`module_for_each_structure (module: Module? implicit; block: block\<(StructInfo):void\>) <function-rtti_module_for_each_structure_Module_q__implicit_block_ls_StructInfo_c_void_gr_>` 
  *  :ref:`module_for_each_enumeration (module: Module? implicit; block: block\<(EnumInfo):void\>) <function-rtti_module_for_each_enumeration_Module_q__implicit_block_ls_EnumInfo_c_void_gr_>` 
  *  :ref:`module_for_each_function (module: Module? implicit; block: block\<(FuncInfo):void\>) <function-rtti_module_for_each_function_Module_q__implicit_block_ls_FuncInfo_c_void_gr_>` 
  *  :ref:`module_for_each_generic (module: Module? implicit; block: block\<(FuncInfo):void\>) <function-rtti_module_for_each_generic_Module_q__implicit_block_ls_FuncInfo_c_void_gr_>` 
  *  :ref:`module_for_each_global (module: Module? implicit; block: block\<(VarInfo):void\>) <function-rtti_module_for_each_global_Module_q__implicit_block_ls_VarInfo_c_void_gr_>` 
  *  :ref:`module_for_each_annotation (module: Module? implicit; block: block\<(Annotation):void\>) <function-rtti_module_for_each_annotation_Module_q__implicit_block_ls_Annotation_c_void_gr_>` 

.. _function-rtti_module_for_each_dependency_Module_q__implicit_block_ls_Module_q_;bool_c_void_gr_:

.. das:function:: module_for_each_dependency(module: Module? implicit; block: block<(Module?;bool):void>)

Iterates through each module dependency of the given ``Module``, yielding the dependent ``Module`` pointer for each required module.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?;bool):void> implicit

.. _function-rtti_module_for_each_structure_Module_q__implicit_block_ls_StructInfo_c_void_gr_:

.. das:function:: module_for_each_structure(module: Module? implicit; block: block<(StructInfo):void>)

Iterates through each structure declaration in the given ``Module``, yielding a ``StructInfo`` pointer for each struct.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`StructInfo <handle-rtti-StructInfo>` ):void> implicit

.. _function-rtti_module_for_each_enumeration_Module_q__implicit_block_ls_EnumInfo_c_void_gr_:

.. das:function:: module_for_each_enumeration(module: Module? implicit; block: block<(EnumInfo):void>)

Iterates through each enumeration declared in the given ``Module``, yielding an ``EnumInfo`` pointer for each enum.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`EnumInfo <handle-rtti-EnumInfo>` ):void> implicit

.. _function-rtti_module_for_each_function_Module_q__implicit_block_ls_FuncInfo_c_void_gr_:

.. das:function:: module_for_each_function(module: Module? implicit; block: block<(FuncInfo):void>)

Iterates through each function declared in the given ``Module``, yielding a ``FuncInfo`` pointer for each function.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void> implicit

.. _function-rtti_module_for_each_generic_Module_q__implicit_block_ls_FuncInfo_c_void_gr_:

.. das:function:: module_for_each_generic(module: Module? implicit; block: block<(FuncInfo):void>)

Iterates through each generic (template) function declared in the given ``Module``, yielding a ``FuncInfo`` pointer for each generic.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`FuncInfo <handle-rtti-FuncInfo>` ):void> implicit

.. _function-rtti_module_for_each_global_Module_q__implicit_block_ls_VarInfo_c_void_gr_:

.. das:function:: module_for_each_global(module: Module? implicit; block: block<(VarInfo):void>)

Iterates through each global variable declared in the given ``Module``, yielding a ``VarInfo`` pointer for each variable.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`VarInfo <handle-rtti-VarInfo>` ):void> implicit

.. _function-rtti_module_for_each_annotation_Module_q__implicit_block_ls_Annotation_c_void_gr_:

.. das:function:: module_for_each_annotation(module: Module? implicit; block: block<(Annotation):void>)

Iterates through each annotation (handled type) in the given ``Module``, yielding an ``Annotation`` pointer for each registered annotation.

:Arguments: * **module** :  :ref:`Module <handle-rtti-Module>` ? implicit

            * **block** : block<( :ref:`Annotation <handle-rtti-Annotation>` ):void> implicit

+++++++++++++++++
Annotation access
+++++++++++++++++

  *  :ref:`get_annotation_argument_value (info: AnnotationArgument implicit) : RttiValue <function-rtti_get_annotation_argument_value_AnnotationArgument_implicit>` 
  *  :ref:`add_annotation_argument (annotation: AnnotationArgumentList implicit; name: string implicit) : int <function-rtti_add_annotation_argument_AnnotationArgumentList_implicit_string_implicit>` 

.. _function-rtti_get_annotation_argument_value_AnnotationArgument_implicit:

.. das:function:: get_annotation_argument_value(info: AnnotationArgument implicit) : RttiValue

Returns an ``RttiValue`` variant representing the value of a specific named argument from an ``AnnotationArgumentList``.

:Arguments: * **info** :  :ref:`AnnotationArgument <handle-rtti-AnnotationArgument>`  implicit

.. _function-rtti_add_annotation_argument_AnnotationArgumentList_implicit_string_implicit:

.. das:function:: add_annotation_argument(annotation: AnnotationArgumentList implicit; name: string implicit) : int

Appends an annotation argument (name-value pair) to the given ``AnnotationArgumentList``, used when constructing annotations programmatically.

:Arguments: * **annotation** :  :ref:`AnnotationArgumentList <handle-rtti-AnnotationArgumentList>`  implicit

            * **name** : string implicit

++++++++++++++++++++++++++
Compilation and simulation
++++++++++++++++++++++++++

  *  :ref:`compile (module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; block: block\<(bool;smart_ptr\<Program\>;das_string):void\>) <function-rtti_compile_string_implicit_string_implicit_CodeOfPolicies_implicit_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_>` 
  *  :ref:`compile (module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; exportAll: bool; block: block\<(bool;smart_ptr\<Program\>;das_string):void\>) <function-rtti_compile_string_implicit_string_implicit_CodeOfPolicies_implicit_bool_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_>` 
  *  :ref:`compile_file (module_name: string implicit; fileAccess: smart_ptr\<FileAccess\> implicit; moduleGroup: ModuleGroup? implicit; codeOfPolicies: CodeOfPolicies implicit; block: block\<(bool;smart_ptr\<Program\>;das_string):void\>) <function-rtti_compile_file_string_implicit_smart_ptr_ls_FileAccess_gr__implicit_ModuleGroup_q__implicit_CodeOfPolicies_implicit_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_>` 
  *  :ref:`for_each_expected_error (program: smart_ptr\<Program\> implicit; block: block\<(CompilationError;int):void\>) <function-rtti_for_each_expected_error_smart_ptr_ls_Program_gr__implicit_block_ls_CompilationError;int_c_void_gr_>` 
  *  :ref:`for_each_require_declaration (program: smart_ptr\<Program\> implicit; block: block\<(Module?;string#;string#;bool;LineInfo):void\>) <function-rtti_for_each_require_declaration_smart_ptr_ls_Program_gr__implicit_block_ls_Module_q_;string_hh_;string_hh_;bool;LineInfo_c_void_gr_>` 
  *  :ref:`simulate (program: smart_ptr\<Program\> const& implicit; block: block\<(bool;smart_ptr\<Context\>;das_string):void\>) <function-rtti_simulate_smart_ptr_ls_Program_gr__const_implicit_block_ls_bool;smart_ptr_ls_Context_gr_;das_string_c_void_gr_>` 

.. _function-rtti_compile_string_implicit_string_implicit_CodeOfPolicies_implicit_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_:

.. das:function:: compile(module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; block: block<(bool;smart_ptr<Program>;das_string):void>)

Compiles a daScript program from a source code string using the provided ``FileAccess`` and ``ModuleGroup``, returning a ``ProgramPtr`` (null on failure).

:Arguments: * **module_name** : string implicit

            * **codeText** : string implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-rtti_compile_string_implicit_string_implicit_CodeOfPolicies_implicit_bool_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_:

.. das:function:: compile(module_name: string implicit; codeText: string implicit; codeOfPolicies: CodeOfPolicies implicit; exportAll: bool; block: block<(bool;smart_ptr<Program>;das_string):void>)

Compiles a daScript program from a source code string using the provided ``FileAccess`` and ``ModuleGroup``, returning a ``ProgramPtr`` (null on failure).

:Arguments: * **module_name** : string implicit

            * **codeText** : string implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **exportAll** : bool

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-rtti_compile_file_string_implicit_smart_ptr_ls_FileAccess_gr__implicit_ModuleGroup_q__implicit_CodeOfPolicies_implicit_block_ls_bool;smart_ptr_ls_Program_gr_;das_string_c_void_gr_:

.. das:function:: compile_file(module_name: string implicit; fileAccess: smart_ptr<FileAccess> implicit; moduleGroup: ModuleGroup? implicit; codeOfPolicies: CodeOfPolicies implicit; block: block<(bool;smart_ptr<Program>;das_string):void>)

Compiles a daScript program from a file registered in the given ``FileAccess`` object, returning a ``ProgramPtr`` (null on failure).

:Arguments: * **module_name** : string implicit

            * **fileAccess** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **moduleGroup** :  :ref:`ModuleGroup <handle-rtti-ModuleGroup>` ? implicit

            * **codeOfPolicies** :  :ref:`CodeOfPolicies <handle-rtti-CodeOfPolicies>`  implicit

            * **block** : block<(bool;smart_ptr< :ref:`Program <handle-rtti-Program>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

.. _function-rtti_for_each_expected_error_smart_ptr_ls_Program_gr__implicit_block_ls_CompilationError;int_c_void_gr_:

.. das:function:: for_each_expected_error(program: smart_ptr<Program> implicit; block: block<(CompilationError;int):void>)

Iterates through each expected compilation error declared in the ``Program`` (via ``expect``), yielding the error code for each.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`CompilationError <enum-rtti-CompilationError>` ;int):void> implicit

.. _function-rtti_for_each_require_declaration_smart_ptr_ls_Program_gr__implicit_block_ls_Module_q_;string_hh_;string_hh_;bool;LineInfo_c_void_gr_:

.. das:function:: for_each_require_declaration(program: smart_ptr<Program> implicit; block: block<(Module?;string#;string#;bool;LineInfo):void>)

Iterates through each ``require`` declaration of the compiled ``Program``, yielding the module name, public/private flag, and source ``LineInfo``.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` > implicit

            * **block** : block<( :ref:`Module <handle-rtti-Module>` ?;string#;string#;bool; :ref:`LineInfo <handle-rtti-LineInfo>` &):void> implicit

.. _function-rtti_simulate_smart_ptr_ls_Program_gr__const_implicit_block_ls_bool;smart_ptr_ls_Context_gr_;das_string_c_void_gr_:

.. das:function:: simulate(program: smart_ptr<Program> const& implicit; block: block<(bool;smart_ptr<Context>;das_string):void>)

Simulates (links and initializes) a compiled ``Program``, returning a ``Context`` pointer ready for function execution, or null on failure.

:Arguments: * **program** : smart_ptr< :ref:`Program <handle-rtti-Program>` >& implicit

            * **block** : block<(bool;smart_ptr< :ref:`Context <handle-rtti-Context>` >; :ref:`das_string <handle-builtin-das_string>` ):void> implicit

+++++++++++
File access
+++++++++++

  *  :ref:`make_file_access (project: string implicit) : smart_ptr\<FileAccess\> <function-rtti_make_file_access_string_implicit>` 
  *  :ref:`set_file_source (access: smart_ptr\<FileAccess\> implicit; fileName: string implicit; text: string implicit) : bool <function-rtti_set_file_source_smart_ptr_ls_FileAccess_gr__implicit_string_implicit_string_implicit>` 
  *  :ref:`add_file_access_root (access: smart_ptr\<FileAccess\> implicit; mod: string implicit; path: string implicit) : bool <function-rtti_add_file_access_root_smart_ptr_ls_FileAccess_gr__implicit_string_implicit_string_implicit>` 

.. _function-rtti_make_file_access_string_implicit:

.. das:function:: make_file_access(project: string implicit) : smart_ptr<FileAccess>

Creates and returns a new ``FileAccessPtr`` (``smart_ptr<FileAccess>``) initialized as a default file-system-backed project.

:Arguments: * **project** : string implicit

.. _function-rtti_set_file_source_smart_ptr_ls_FileAccess_gr__implicit_string_implicit_string_implicit:

.. das:function:: set_file_source(access: smart_ptr<FileAccess> implicit; fileName: string implicit; text: string implicit) : bool

Registers a source code ``string`` for the given file name inside the ``FileAccess`` object, allowing in-memory compilation without disk files.

:Arguments: * **access** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **fileName** : string implicit

            * **text** : string implicit

.. _function-rtti_add_file_access_root_smart_ptr_ls_FileAccess_gr__implicit_string_implicit_string_implicit:

.. das:function:: add_file_access_root(access: smart_ptr<FileAccess> implicit; mod: string implicit; path: string implicit) : bool

Adds an extra root directory (search path) to the given ``FileAccess`` object, expanding where ``require`` resolves files from.

:Arguments: * **access** : smart_ptr< :ref:`FileAccess <handle-rtti-FileAccess>` > implicit

            * **mod** : string implicit

            * **path** : string implicit

++++++++++++++++
Structure access
++++++++++++++++

  *  :ref:`rtti_builtin_structure_for_each_annotation (struct: StructInfo implicit; block: block\<():void\>) <function-rtti_rtti_builtin_structure_for_each_annotation_StructInfo_implicit_block_ls__c_void_gr_>` 
  *  :ref:`basic_struct_for_each_field (annotation: BasicStructureAnnotation implicit; block: block\<(string;string;TypeInfo;uint):void\>) <function-rtti_basic_struct_for_each_field_BasicStructureAnnotation_implicit_block_ls_string;string;TypeInfo;uint_c_void_gr_>` 
  *  :ref:`basic_struct_for_each_parent (annotation: BasicStructureAnnotation implicit; block: block\<(Annotation?):void\>) <function-rtti_basic_struct_for_each_parent_BasicStructureAnnotation_implicit_block_ls_Annotation_q__c_void_gr_>` 
  *  :ref:`structure_for_each_annotation (st: StructInfo; subexpr: block\<(ann:Annotation;args:AnnotationArguments):void\>) : auto <function-rtti_structure_for_each_annotation_StructInfo_block_ls_ann_c_Annotation;args_c_AnnotationArguments_c_void_gr_>` 

.. _function-rtti_rtti_builtin_structure_for_each_annotation_StructInfo_implicit_block_ls__c_void_gr_:

.. das:function:: rtti_builtin_structure_for_each_annotation(struct: StructInfo implicit; block: block<():void>)

Iterates through each annotation attached to a ``StructInfo``, yielding the annotation name and its ``AnnotationArgumentList`` for each.

:Arguments: * **struct** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit

            * **block** : block<void> implicit

.. _function-rtti_basic_struct_for_each_field_BasicStructureAnnotation_implicit_block_ls_string;string;TypeInfo;uint_c_void_gr_:

.. das:function:: basic_struct_for_each_field(annotation: BasicStructureAnnotation implicit; block: block<(string;string;TypeInfo;uint):void>)

Iterates through each field of a ``BasicStructureAnnotation``, yielding the field name, C++ name, ``TypeInfo``, and byte offset for each field.

:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<(string;string; :ref:`TypeInfo <handle-rtti-TypeInfo>` ;uint):void> implicit

.. _function-rtti_basic_struct_for_each_parent_BasicStructureAnnotation_implicit_block_ls_Annotation_q__c_void_gr_:

.. das:function:: basic_struct_for_each_parent(annotation: BasicStructureAnnotation implicit; block: block<(Annotation?):void>)

Iterates through each parent (base class) of a ``BasicStructureAnnotation``, yielding the parent ``TypeInfo`` for each ancestor.

:Arguments: * **annotation** :  :ref:`BasicStructureAnnotation <handle-rtti-BasicStructureAnnotation>`  implicit

            * **block** : block<( :ref:`Annotation <handle-rtti-Annotation>` ?):void> implicit

.. _function-rtti_structure_for_each_annotation_StructInfo_block_ls_ann_c_Annotation;args_c_AnnotationArguments_c_void_gr_:

.. das:function:: structure_for_each_annotation(st: StructInfo; subexpr: block<(ann:Annotation;args:AnnotationArguments):void>) : auto

Iterates through each annotation attached to a ``StructInfo``, yielding the annotation name and ``AnnotationArgumentList`` — an alias of ``rtti_builtin_structure_for_each_annotation``.

:Arguments: * **st** :  :ref:`StructInfo <handle-rtti-StructInfo>` 

            * **subexpr** : block<(ann: :ref:`Annotation <handle-rtti-Annotation>` ;args: :ref:`AnnotationArguments <handle-rtti-AnnotationArguments>` ):void>

+++++++++++++++++++++++++
Data walking and printing
+++++++++++++++++++++++++

  *  :ref:`sprint_data (data: void? implicit; type: TypeInfo const? implicit; flags: bitfield) : string <function-rtti_sprint_data_void_q__implicit_TypeInfo_const_q__implicit_bitfield>` 
  *  :ref:`sprint_data (data: float4; type: TypeInfo const? implicit; flags: bitfield) : string <function-rtti_sprint_data_float4_TypeInfo_const_q__implicit_bitfield>` 
  *  :ref:`describe (type: TypeInfo const? implicit) : string <function-rtti_describe_TypeInfo_const_q__implicit>` 
  *  :ref:`describe (lineinfo: LineInfo implicit; fully: bool = false) : string <function-rtti_describe_LineInfo_implicit_bool>` 
  *  :ref:`get_mangled_name (type: TypeInfo const? implicit) : string <function-rtti_get_mangled_name_TypeInfo_const_q__implicit>` 

.. _function-rtti_sprint_data_void_q__implicit_TypeInfo_const_q__implicit_bitfield:

.. das:function:: sprint_data(data: void? implicit; type: TypeInfo const? implicit; flags: bitfield) : string

Returns a ``string`` representation of a value given its data pointer and ``TypeInfo``, similar to ``debug`` or ``print`` but capturing output as a string.

:Arguments: * **data** : void? implicit

            * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **flags** : bitfield<>

.. _function-rtti_sprint_data_float4_TypeInfo_const_q__implicit_bitfield:

.. das:function:: sprint_data(data: float4; type: TypeInfo const? implicit; flags: bitfield) : string

Returns a ``string`` representation of a value given its data pointer and ``TypeInfo``, similar to ``debug`` or ``print`` but capturing output as a string.

:Arguments: * **data** : float4

            * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **flags** : bitfield<>

.. _function-rtti_describe_TypeInfo_const_q__implicit:

.. das:function:: describe(type: TypeInfo const? implicit) : string

Returns a human-readable ``string`` description of an RTTI object (``TypeInfo``, ``VarInfo``, ``FuncInfo``, etc.), useful for logging and debug output.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

.. _function-rtti_describe_LineInfo_implicit_bool:

.. das:function:: describe(lineinfo: LineInfo implicit; fully: bool = false) : string

Returns a human-readable ``string`` description of an RTTI object (``TypeInfo``, ``VarInfo``, ``FuncInfo``, etc.), useful for logging and debug output.

:Arguments: * **lineinfo** :  :ref:`LineInfo <handle-rtti-LineInfo>`  implicit

            * **fully** : bool

.. _function-rtti_get_mangled_name_TypeInfo_const_q__implicit:

.. das:function:: get_mangled_name(type: TypeInfo const? implicit) : string

Returns the full mangled name ``string`` for the given ``FuncInfo``, encoding its module, name, and argument types.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

++++++++++++++++++++++++++++++
Function and mangled name hash
++++++++++++++++++++++++++++++

  *  :ref:`get_function_by_mangled_name_hash (src: uint64) : function\<():void\> <function-rtti_get_function_by_mangled_name_hash_uint64>` 
  *  :ref:`get_function_by_mangled_name_hash (src: uint64; context: Context implicit) : function\<():void\> <function-rtti_get_function_by_mangled_name_hash_uint64_Context_implicit>` 
  *  :ref:`get_function_mangled_name_hash (src: function\<():void\>) : uint64 <function-rtti_get_function_mangled_name_hash_function_ls__c_void_gr_>` 
  *  :ref:`get_function_address (MNH: uint64; at: Context implicit) : uint64 <function-rtti_get_function_address_uint64_Context_implicit>` 

.. _function-rtti_get_function_by_mangled_name_hash_uint64:

.. das:function:: get_function_by_mangled_name_hash(src: uint64) : function<():void>

Returns a ``function<>`` lambda value looked up by its mangled name hash in the given ``Context``.

:Arguments: * **src** : uint64

.. _function-rtti_get_function_by_mangled_name_hash_uint64_Context_implicit:

.. das:function:: get_function_by_mangled_name_hash(src: uint64; context: Context implicit) : function<():void>

Returns a ``function<>`` lambda value looked up by its mangled name hash in the given ``Context``.

:Arguments: * **src** : uint64

            * **context** :  :ref:`Context <handle-rtti-Context>`  implicit

.. _function-rtti_get_function_mangled_name_hash_function_ls__c_void_gr_:

.. das:function:: get_function_mangled_name_hash(src: function<():void>) : uint64

Returns the ``uint64`` mangled name hash for the given ``function<>`` value, which uniquely identifies the function in its ``Context``.

:Arguments: * **src** : function<void>

.. _function-rtti_get_function_address_uint64_Context_implicit:

.. das:function:: get_function_address(MNH: uint64; at: Context implicit) : uint64

Returns a ``SimFunction`` pointer looked up by mangled name hash in the given ``Context``, or null if not found.

:Arguments: * **MNH** : uint64

            * **at** :  :ref:`Context <handle-rtti-Context>`  implicit

+++++++++++++++++++++++++
Context and mutex locking
+++++++++++++++++++++++++

  *  :ref:`lock_this_context (block: block\<():void\>) <function-rtti_lock_this_context_block_ls__c_void_gr_>` 
  *  :ref:`lock_context (lock_context: Context implicit; block: block\<():void\>) <function-rtti_lock_context_Context_implicit_block_ls__c_void_gr_>` 
  *  :ref:`lock_mutex (mutex: recursive_mutex implicit; block: block\<():void\>) <function-rtti_lock_mutex_recursive_mutex_implicit_block_ls__c_void_gr_>` 

.. _function-rtti_lock_this_context_block_ls__c_void_gr_:

.. das:function:: lock_this_context(block: block<():void>)

Acquires a recursive lock on the current ``Context`` and executes a block, ensuring thread-safe access within the scope.

:Arguments: * **block** : block<void> implicit

.. _function-rtti_lock_context_Context_implicit_block_ls__c_void_gr_:

.. das:function:: lock_context(lock_context: Context implicit; block: block<():void>)

Acquires a recursive lock on the given ``Context`` and executes a block, ensuring thread-safe access to context data within the scope.

:Arguments: * **lock_context** :  :ref:`Context <handle-rtti-Context>`  implicit

            * **block** : block<void> implicit

.. _function-rtti_lock_mutex_recursive_mutex_implicit_block_ls__c_void_gr_:

.. das:function:: lock_mutex(mutex: recursive_mutex implicit; block: block<():void>)

Acquires a recursive lock on the given ``recursive_mutex`` and executes a block, releasing the lock when the block exits.

:Arguments: * **mutex** :  :ref:`recursive_mutex <handle-rtti-recursive_mutex>`  implicit

            * **block** : block<void> implicit

+++++++++++++++++++
Runtime data access
+++++++++++++++++++

  *  :ref:`get_table_key_index (table: void? implicit; key: any; baseType: Type; valueTypeSize: int) : int <function-rtti_get_table_key_index_void_q__implicit_any_Type_int>` 

.. _function-rtti_get_table_key_index_void_q__implicit_any_Type_int:

.. das:function:: get_table_key_index(table: void? implicit; key: any; baseType: Type; valueTypeSize: int) : int

Returns the internal slot index (``int``) for the given key within a ``table`` value, or ``-1`` if the key is not present.

:Arguments: * **table** : void? implicit

            * **key** : any

            * **baseType** :  :ref:`Type <enum-rtti-Type>` 

            * **valueTypeSize** : int

++++++++++++++++++++++++
Tuple and variant access
++++++++++++++++++++++++

  *  :ref:`get_tuple_field_offset (type: TypeInfo? implicit; index: int) : int <function-rtti_get_tuple_field_offset_TypeInfo_q__implicit_int>` 
  *  :ref:`get_variant_field_offset (type: TypeInfo? implicit; index: int) : int <function-rtti_get_variant_field_offset_TypeInfo_q__implicit_int>` 

.. _function-rtti_get_tuple_field_offset_TypeInfo_q__implicit_int:

.. das:function:: get_tuple_field_offset(type: TypeInfo? implicit; index: int) : int

Returns the byte offset (``int``) of a field at the given index within a tuple type described by ``TypeInfo``.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **index** : int

.. _function-rtti_get_variant_field_offset_TypeInfo_q__implicit_int:

.. das:function:: get_variant_field_offset(type: TypeInfo? implicit; index: int) : int

Returns the byte offset (``int``) of a field at the given index within a variant type described by ``TypeInfo``.

:Arguments: * **type** :  :ref:`TypeInfo <handle-rtti-TypeInfo>` ? implicit

            * **index** : int

+++++++++
Iteration
+++++++++

  *  :ref:`each (info: FuncInfo implicit ==const) : iterator\<VarInfo&\> <function-rtti_each_FuncInfo_implicit__eq__eq_const>` 
  *  :ref:`each (info: FuncInfo const implicit ==const) : iterator\<VarInfo const&\> <function-rtti_each_FuncInfo_const_implicit__eq__eq_const>` 
  *  :ref:`each (info: StructInfo implicit ==const) : iterator\<VarInfo&\> <function-rtti_each_StructInfo_implicit__eq__eq_const>` 
  *  :ref:`each (info: StructInfo const implicit ==const) : iterator\<VarInfo const&\> <function-rtti_each_StructInfo_const_implicit__eq__eq_const>` 
  *  :ref:`each (info: EnumInfo implicit ==const) : iterator\<EnumValueInfo&\> <function-rtti_each_EnumInfo_implicit__eq__eq_const>` 
  *  :ref:`each (info: EnumInfo const implicit ==const) : iterator\<EnumValueInfo const&\> <function-rtti_each_EnumInfo_const_implicit__eq__eq_const>` 

.. _function-rtti_each_FuncInfo_implicit__eq__eq_const:

.. das:function:: each(info: FuncInfo implicit ==const) : iterator<VarInfo&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`  implicit!

.. _function-rtti_each_FuncInfo_const_implicit__eq__eq_const:

.. das:function:: each(info: FuncInfo const implicit ==const) : iterator<VarInfo const&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`  implicit!

.. _function-rtti_each_StructInfo_implicit__eq__eq_const:

.. das:function:: each(info: StructInfo implicit ==const) : iterator<VarInfo&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit!

.. _function-rtti_each_StructInfo_const_implicit__eq__eq_const:

.. das:function:: each(info: StructInfo const implicit ==const) : iterator<VarInfo const&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`  implicit!

.. _function-rtti_each_EnumInfo_implicit__eq__eq_const:

.. das:function:: each(info: EnumInfo implicit ==const) : iterator<EnumValueInfo&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`EnumInfo <handle-rtti-EnumInfo>`  implicit!

.. _function-rtti_each_EnumInfo_const_implicit__eq__eq_const:

.. das:function:: each(info: EnumInfo const implicit ==const) : iterator<EnumValueInfo const&>

Iterates through each element of an RTTI container (e.g., ``AnnotationArguments``, ``AnnotationArgumentList``, ``AnnotationList``), yielding individual entries.

:Arguments: * **info** :  :ref:`EnumInfo <handle-rtti-EnumInfo>`  implicit!


