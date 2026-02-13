
.. _stdlib__builtin:

================
Built-in runtime
================

The BUILTIN module contains core runtime functions available in all daScript programs
without explicit ``require``. It includes:

- Heap and memory management (``heap_bytes_allocated``, ``heap_report``, ``memory_report``)
- Debug output (``print``, ``debug``, ``stackwalk``)
- Panic and error handling (``panic``, ``terminate``, ``assert``)
- Pointer and memory operations (``intptr``, ``malloc``, ``free``)
- Profiling (``profile``)
- Type conversion (``string``)

All functions and symbols are in "builtin" module, use require to get access to it. ::

    require builtin

Example: ::

    [export]
        def main() {
            print("hello, world!\n")
            assert(1 + 1 == 2)
            let s = string(42)
            print("string(42) = {s}\n")
            let name = "daScript"
            print("welcome to {name}\n")
            var arr : array<int>
            arr |> push(10)
            arr |> push(20)
            print("length = {length(arr)}\n")
            print("arr[0] = {arr[0]}\n")
        }
        // output:
        // hello, world!
        // string(42) = 42
        // welcome to daScript
        // length = 2
        // arr[0] = 10

++++++++++++
Type aliases
++++++++++++

.. _alias-print_flags:

.. das:attribute:: bitfield print_flags

This bitfield specifies how exactly values are to be printed

:Fields: * **escapeString** (0x1) - if string is to be escaped

         * **namesAndDimensions** (0x2) - names of the fields and dimensions of the arrays

         * **typeQualifiers** (0x4) - type qualifiers for the specific types like double and uint64

         * **refAddresses** (0x8) - addresses in hexadecimal of each reference value

         * **singleLine** (0x10) - human readable vs single line

         * **fixedPoint** (0x20) - always output fixed point precision for floating point values


+++++++++
Constants
+++++++++

.. _global-builtin-DAS_MAX_FUNCTION_ARGUMENTS:

.. das:attribute:: DAS_MAX_FUNCTION_ARGUMENTS = 32

Maximum number of arguments a function can accept, used to pre-allocate stack space for function call arguments.

.. _global-builtin-INT_MIN:

.. das:attribute:: INT_MIN = -2147483648

Minimum representable value of a signed 32-bit integer (`int`), equal to -2147483648.

.. _global-builtin-INT_MAX:

.. das:attribute:: INT_MAX = 2147483647

Maximum representable value of a signed 32-bit integer (`int`), equal to 2147483647.

.. _global-builtin-UINT_MAX:

.. das:attribute:: UINT_MAX = 0xffffffff

Maximum representable value of an unsigned 32-bit integer (`uint`), equal to 4294967295.

.. _global-builtin-LONG_MIN:

.. das:attribute:: LONG_MIN = -9223372036854775808

Minimum representable value of a signed 64-bit integer (`int64`).

.. _global-builtin-LONG_MAX:

.. das:attribute:: LONG_MAX = 9223372036854775807

Maximum representable value of a signed 64-bit integer (`int64`).

.. _global-builtin-ULONG_MAX:

.. das:attribute:: ULONG_MAX = 0xffffffffffffffff

Maximum representable value of an unsigned 64-bit integer (`uint64`).

.. _global-builtin-FLT_MIN:

.. das:attribute:: FLT_MIN = 1.1754944e-38f

Smallest positive non-zero normalized value of the `float` type; for the most negative value use `-FLT_MAX`.

.. _global-builtin-FLT_MAX:

.. das:attribute:: FLT_MAX = 3.4028235e+38f

Maximum finite representable value of the `float` (32-bit floating-point) type.

.. _global-builtin-DBL_MIN:

.. das:attribute:: DBL_MIN = 2.2250738585072014e-308lf

Smallest positive non-zero normalized value of the `double` type; for the most negative value use `-DBL_MAX`.

.. _global-builtin-DBL_MAX:

.. das:attribute:: DBL_MAX = 1.7976931348623157e+308lf

Maximum finite representable value of the `double` (64-bit floating-point) type.

.. _global-builtin-LOG_CRITICAL:

.. das:attribute:: LOG_CRITICAL = 50000

Log level constant for critical errors such as panics, fatal failures, and shutdown notifications.

.. _global-builtin-LOG_ERROR:

.. das:attribute:: LOG_ERROR = 40000

Log level constant for recoverable error conditions that do not require immediate shutdown.

.. _global-builtin-LOG_WARNING:

.. das:attribute:: LOG_WARNING = 30000

Log level constant for warnings about potential problems, API misuse, or non-fatal error conditions.

.. _global-builtin-LOG_INFO:

.. das:attribute:: LOG_INFO = 20000

Log level constant for general informational messages about normal program operation.

.. _global-builtin-LOG_DEBUG:

.. das:attribute:: LOG_DEBUG = 10000

Log level constant for debug-level diagnostic messages useful during development.

.. _global-builtin-LOG_TRACE:

.. das:attribute:: LOG_TRACE = 0

Log level constant for the most verbose tracing and diagnostic output, typically used for detailed debugging.

.. _global-builtin-VEC_SEP:

.. das:attribute:: VEC_SEP = ","

Read-only string constant used as the separator between vector components when printing; defaults to `","`.

.. _global-builtin-print_flags_debugger:

.. das:attribute:: print_flags_debugger = bitfield(0xf)

Predefined set of print_flags configured to match the output formatting used by the `debug` function.

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-builtin-HashBuilder:

.. das:attribute:: HashBuilder

 Helper structure to facilitate calculating hash values.


++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-builtin-marker:

.. das:attribute:: marker

Attaches arbitrary key-value annotation arguments to a function, typically used by macros to tag functions with metadata.

.. _handle-builtin-generic:

.. das:attribute:: generic

Forces a function to be treated as generic regardless of its argument types, causing it to be instanced in each calling module.

.. _handle-builtin-_macro:

.. das:attribute:: _macro

Marks a function to be executed during the macro compilation pass, similar to `[init]` but running at macro time.

.. _handle-builtin-macro_function:

.. das:attribute:: macro_function

Marks a function as part of the macro subsystem, excluding it from the final compiled context unless it is explicitly referenced.

.. _handle-builtin-hint:

.. das:attribute:: hint

Provides optimization hints to the compiler for the annotated function via annotation arguments.

.. _handle-builtin-jit:

.. das:attribute:: jit

Explicitly forces the annotated function to be compiled using the JIT compiler, overriding default compilation decisions.

.. _handle-builtin-no_jit:

.. das:attribute:: no_jit

Prevents JIT compilation for the annotated function, forcing it to run in interpreted mode.

.. _handle-builtin-nodiscard:

.. das:attribute:: nodiscard

Enforces that the return value of the function must be used by the caller; discarding the result produces a compilation error.

.. _handle-builtin-deprecated:

.. das:attribute:: deprecated

Marks a function as deprecated, causing a compilation warning when referenced and excluding it from the final compiled context.

.. _handle-builtin-alias_cmres:

.. das:attribute:: alias_cmres

Declares that the function always aliases cmres (copy-or-move result), disabling cmres return optimizations for it.

.. _handle-builtin-never_alias_cmres:

.. das:attribute:: never_alias_cmres

Declares that the function never aliases cmres (copy-or-move result), disabling aliasing safety checks for the return value.

.. _handle-builtin-export:

.. das:attribute:: export

Forces a function to be exported and retained in the final compiled context, even if it is not directly called.

.. _handle-builtin-pinvoke:

.. das:attribute:: pinvoke

Marks a function as a platform invoke (pinvoke) entry, routing its calls through the pinvoke interop machinery.

.. _handle-builtin-no_lint:

.. das:attribute:: no_lint

Skips all lint-pass checks for the annotated function, suppressing any lint warnings or errors it would produce.

.. _handle-builtin-sideeffects:

.. das:attribute:: sideeffects

Declares that the function has side effects, preventing the compiler from optimizing away or reordering its calls.

.. _handle-builtin-run:

.. das:attribute:: run

Forces the function to be evaluated at compile time, ensuring its body executes during compilation rather than at runtime.

.. _handle-builtin-unsafe_operation:

.. das:attribute:: unsafe_operation

Marks a function as an unsafe operation, requiring callers to wrap the call in an `unsafe` block.

.. _handle-builtin-unsafe_outside_of_for:

.. das:attribute:: unsafe_outside_of_for

Marks a function as unsafe to call outside of a source-level `for` loop, enforcing iterator-context usage.

.. _handle-builtin-unsafe_when_not_clone_array:

.. das:attribute:: unsafe_when_not_clone_array

Marks a function as unsafe to call outside of an array `clone` operation, restricting its usage context.

.. _handle-builtin-no_aot:

.. das:attribute:: no_aot

Prevents ahead-of-time (AOT) C++ code generation for the annotated function, keeping it interpreted only.

.. _handle-builtin-init:

.. das:attribute:: init

Registers a function to be called automatically during context initialization, before any user code runs.

.. _handle-builtin-finalize:

.. das:attribute:: finalize

Registers a function to be called automatically when the context is shut down, for cleanup and resource release.

.. _handle-builtin-hybrid:

.. das:attribute:: hybrid

Marks a function as a hybrid call target so that AOT generates indirect calls to it, allowing the function to be patched without recompiling dependent AOT code.

.. _handle-builtin-unsafe_deref:

.. das:attribute:: unsafe_deref

Optimization annotation that removes null-pointer checks, bounds checks on array and string indexing, and similar safety validations.

.. _handle-builtin-skip_lock_check:

.. das:attribute:: skip_lock_check

Optimization annotation that disables runtime lock-check validation for the annotated function.

.. _handle-builtin-unused_argument:

.. das:attribute:: unused_argument

Suppresses unused-argument warnings or errors for specific function parameters, providing a workaround when strict code policies are enabled.

.. _handle-builtin-local_only:

.. das:attribute:: local_only

Restricts a function to accept only local `make` expressions such as structure initializers and tuple constructors.

.. _handle-builtin-expect_any_vector:

.. das:attribute:: expect_any_vector

Contract annotation restricting a function argument to accept only `das::vector` template types.

.. _handle-builtin-expect_dim:

.. das:attribute:: expect_dim

Contract annotation requiring a function argument to be a fixed-size (statically dimensioned) array.

.. _handle-builtin-expect_ref:

.. das:attribute:: expect_ref

Contract annotation requiring a function argument to be passed by reference.

.. _handle-builtin-type_function:

.. das:attribute:: type_function

Marks a function as a type function, meaning it operates on types at compile time and does not generate runtime code.

.. _handle-builtin-builtin_array_sort:

.. das:attribute:: builtin_array_sort

Internal function annotation that provides the sorting implementation used by the built-in `sort` function.

+++++++++++
Call macros
+++++++++++

.. _call-macro-builtin-make_function_unsafe:

.. das:attribute:: make_function_unsafe

Propagates the `unsafe` requirement to the calling function, making any function that calls it also require an `unsafe` block.

.. _call-macro-builtin-concept_assert:

.. das:attribute:: concept_assert

Compile-time assertion that reports the error at the call site of the asserted function rather than at the assert line itself.

.. _call-macro-builtin-__builtin_table_set_insert:

.. das:attribute:: __builtin_table_set_insert

Internal function annotation that implements the low-level key insertion for set-style tables (tables with keys only).

.. _call-macro-builtin-__builtin_table_key_exists:

.. das:attribute:: __builtin_table_key_exists

Internal function annotation that implements the low-level key presence check for the `key_exists` operation.

.. _call-macro-builtin-static_assert:

.. das:attribute:: static_assert

Compile-time assertion that produces a compilation error with an optional message when the condition is false.

.. _call-macro-builtin-verify:

.. das:attribute:: verify

Assertion that preserves the evaluated expression even when asserts are disabled, ensuring side effects are never optimized out.

.. _call-macro-builtin-debug:

.. das:attribute:: debug

Prints the human-readable representation of a value to the log and returns that same value, allowing inline debugging in expressions.

.. _call-macro-builtin-assert:

.. das:attribute:: assert

Runtime assertion that panics with an optional error message when the first argument evaluates to false; can be disabled globally.

.. _call-macro-builtin-memzero:

.. das:attribute:: memzero

Fills a region of memory with zeros, used internally for default-initializing values.

.. _call-macro-builtin-__builtin_table_find:

.. das:attribute:: __builtin_table_find

Internal function annotation that implements the low-level table lookup for the `find` operation.

.. _call-macro-builtin-invoke:

.. das:attribute:: invoke

Invokes a block, function pointer, or lambda, dispatching the call through the appropriate calling convention.

.. _call-macro-builtin-__builtin_table_erase:

.. das:attribute:: __builtin_table_erase

Internal function annotation that implements the low-level table entry removal for the `erase` operation.

+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-builtin-_esc:

.. das:attribute:: _esc

Reader macro that returns the raw string content without processing escape sequences, e.g. `%_esc~\n\r~%_esc` yields the literal four characters `\`, `n`, `\`, `r`.

+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-builtin-rtti_classinfo:

.. das:attribute:: rtti_classinfo

Typeinfo macro that generates RTTI `TypeInfo` metadata required for class initialization and reflection.

+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-das_string:

.. das:attribute:: das_string

Handled type wrapping `das::string` (typically `std::string`), providing heap-allocated mutable string storage.

.. _handle-builtin-clock:

.. das:attribute:: clock

Handled type wrapping `das::Time`, which encapsulates the C `time_t` value for calendar time representation.

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-builtin-comment:

.. das:attribute:: comment

No-op structure annotation that holds annotation arguments as metadata without affecting code generation.

.. _handle-builtin-no_default_initializer:

.. das:attribute:: no_default_initializer

Prevents the compiler from generating a default initializer for the annotated structure.

.. _handle-builtin-macro_interface:

.. das:attribute:: macro_interface

Marks a class hierarchy as a macro interface, preventing it and its descendants from being exported by default.

.. _handle-builtin-skip_field_lock_check:

.. das:attribute:: skip_field_lock_check

Optimization annotation that disables runtime lock checks when accessing fields of the annotated structure.

.. _handle-builtin-cpp_layout:

.. das:attribute:: cpp_layout

Forces the structure to use C++ memory layout rules (alignment and padding) instead of native daScript layout.

.. _handle-builtin-safe_when_uninitialized:

.. das:attribute:: safe_when_uninitialized

Declares that the structure is safe to access before explicit initialization, suppressing uninitialized-use errors.

.. _handle-builtin-persistent:

.. das:attribute:: persistent

Allocates the structure on the C++ heap (via `new`) instead of the daScript context heap, allowing it to outlive the context.

++++++++++
Containers
++++++++++

  *  :ref:`clear (array: array\<anything\>) <function-builtin_clear_array_ls_anything_gr_>` 
  *  :ref:`length (array: array\<anything\>) : int <function-builtin_length_array_ls_anything_gr_>` 
  *  :ref:`capacity (array: array\<anything\>) : int <function-builtin_capacity_array_ls_anything_gr_>` 
  *  :ref:`empty (iterator: iterator implicit) : bool <function-builtin_empty_iterator_implicit>` 
  *  :ref:`length (table: table\<anything, anything\>) : int <function-builtin_length_table_ls_anything,_anything_gr_>` 
  *  :ref:`capacity (table: table\<anything, anything\>) : int <function-builtin_capacity_table_ls_anything,_anything_gr_>` 
  *  :ref:`empty (str: string implicit) : bool <function-builtin_empty_string_implicit>` 
  *  :ref:`empty (str: das_string implicit) : bool <function-builtin_empty_das_string_implicit>` 
  *  :ref:`resize (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_array_ls_autonumT_gr__int>` 
  *  :ref:`resize_and_init (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_and_init_array_ls_autonumT_gr__int>` 
  *  :ref:`resize_and_init (var Arr: array\<auto(numT)\>; newSize: int; initValue: numT) : auto <function-builtin_resize_and_init_array_ls_autonumT_gr__int_numT>` 
  *  :ref:`resize_no_init (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_resize_no_init_array_ls_autonumT_gr__int>` 
  *  :ref:`reserve (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-builtin_reserve_array_ls_autonumT_gr__int>` 
  *  :ref:`pop (var Arr: array\<auto(numT)\>) : auto <function-builtin_pop_array_ls_autonumT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT ==const; at: int) : auto <function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const_int>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var value: numT ==const; at: int) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const_int>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT ==const) : auto <function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var value: numT ==const) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: array\<numT\>) : auto <function-builtin_push_array_ls_autonumT_gr__array_ls_numT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; var varr: array\<numT\>) : auto <function-builtin_push_array_ls_autonumT_gr___array_ls_numT_gr_>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: numT[]) : auto <function-builtin_push_array_ls_autonumT_gr__numT>` 
  *  :ref:`push (var Arr: array\<auto(numT)[]\>; varr: numT const[] ==const) : auto <function-builtin_push_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push (var Arr: array\<auto(numT)[]\>; var varr: numT[] ==const) : auto <function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&; at: int) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT_int>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT[]) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)[]\>; var value: numT[]) : auto <function-builtin_emplace_array_ls_autonumT_gr__numT>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT ==const|numT const# ==const; at: int) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const_int>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var value: numT ==const|numT# ==const; at: int) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const_int>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT ==const|numT const# ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var value: numT ==const|numT# ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; varr: numT const[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; var varr: numT[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr___numT__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)[]\>; varr: numT const[] ==const) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)[]\>; var varr: numT[]) : auto <function-builtin_push_clone_array_ls_autonumT_gr__numT>` 
  *  :ref:`push_clone (var A: auto(CT); b: auto(TT)|auto(TT)#) : auto <function-builtin_push_clone_autoCT_autoTTautoTT_hh_>` 
  *  :ref:`back (var a: array\<auto(TT)\>) : TT& <function-builtin_back__array_ls_autoTT_gr_>` 
  *  :ref:`back (var a: array\<auto(TT)\>#) : TT&# <function-builtin_back__array_ls_autoTT_gr__hh_>` 
  *  :ref:`back (a: array\<auto(TT)\>) : TT <function-builtin_back_array_ls_autoTT_gr_>` 
  *  :ref:`back (a: array\<auto(TT)\>#) : TT const&# <function-builtin_back_array_ls_autoTT_gr__hh_>` 
  *  :ref:`back (var arr: auto(TT) ==const) : auto& <function-builtin_back__autoTT__eq__eq_const>` 
  *  :ref:`back (arr: auto(TT) ==const) : auto <function-builtin_back_autoTT__eq__eq_const>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int) : auto <function-builtin_erase_array_ls_autonumT_gr__int>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int; count: int) : auto <function-builtin_erase_array_ls_autonumT_gr__int_int>` 
  *  :ref:`erase_if (var arr: array\<auto(TT)\>; blk: block\<(key:TT const):bool\>|block\<(var key:TT&):bool\>) : auto <function-builtin_erase_if_array_ls_autoTT_gr__block_ls_key_c_TT_const_c_bool_gr_block_ls_var_key_c_TT_c_bool_gr_>` 
  *  :ref:`remove_value (var arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : bool <function-builtin_remove_value_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT>` 
  *  :ref:`length (a: auto|auto#) : int <function-builtin_length_autoauto_hh_>` 
  *  :ref:`empty (a: array\<auto\>|array\<auto\>#) : bool <function-builtin_empty_array_ls_auto_gr_array_ls_auto_gr__hh_>` 
  *  :ref:`empty (a: table\<auto;auto\>|table\<auto;auto\>#) : bool <function-builtin_empty_table_ls_auto;auto_gr_table_ls_auto;auto_gr__hh_>` 
  *  :ref:`reserve (var Tab: table\<auto(keyT), auto\>; newSize: int) : auto <function-builtin_reserve_table_ls_autokeyT,_auto_gr__int>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)\>#; at: keyT|keyT#; blk: block\<(p:valT const&#):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const_hh__c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(p:valT):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)\>#; at: keyT|keyT#; blk: block\<(var p:valT&#):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT_hh__c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(var p:valT&):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)[]\>#; at: keyT|keyT#; blk: block\<(p:valT const[-2]&#):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const-2_hh__c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; blk: block\<(p:valT const[-2]&):void\>) : auto <function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_const-2_c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)[]\>#; at: keyT|keyT#; blk: block\<(var p:valT[-2]&#):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT-2_hh__c_void_gr_>` 
  *  :ref:`get (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; blk: block\<(var p:valT[-2]&):void\>) : auto <function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT-2_c_void_gr_>` 
  *  :ref:`get (Tab: table\<auto(keyT), void\>; at: keyT|keyT#; blk: block\<(var p:void?):void\>) : auto <function-builtin_get_table_ls_autokeyT,_void_gr__keyTkeyT_hh__block_ls_var_p_c_void_q__c_void_gr_>` 
  *  :ref:`get_value (Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT <function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), smart_ptr\<auto(valT)\>\>; at: keyT|keyT#) : smart_ptr\<valT\> <function-builtin_get_value_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT <function-builtin_get_value__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_value (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#) : valT[-2] <function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`erase (var Tab: table\<auto(keyT), auto(valT)\>; at: string#) : bool <function-builtin_erase_table_ls_autokeyT,_autovalT_gr__string_hh_>` 
  *  :ref:`erase (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : bool <function-builtin_erase_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), void\>; at: keyT|keyT#) : auto <function-builtin_insert_table_ls_autokeyT,_void_gr__keyTkeyT_hh_>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT ==const|valT# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[] ==const|valT[]# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert_clone (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto <function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT ==const|valT# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[] ==const|valT[]# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const>` 
  *  :ref:`insert (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto <function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; var val: valT&) : auto <function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), smart_ptr\<auto(valT)\>\>; at: keyT|keyT#; var val: smart_ptr\<valT\>&) : auto <function-builtin_emplace_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh__smart_ptr_ls_valT_gr_>` 
  *  :ref:`emplace (var Tab: table\<auto(keyT), auto(valT)[]\>; at: keyT|keyT#; var val: valT[]&) : auto <function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT>` 
  *  :ref:`emplace_new (var tab: table\<auto(kT), smart_ptr\<auto(vT)\>\>; key: kT; var value: smart_ptr\<vT\>) : auto <function-builtin_emplace_new_table_ls_autokT,_smart_ptr_ls_autovT_gr__gr__kT_smart_ptr_ls_vT_gr_>` 
  *  :ref:`emplace (var Tab: table\<auto, auto\>; key: auto; value: auto) : auto <function-builtin_emplace_table_ls_auto,_auto_gr__auto_auto>` 
  *  :ref:`emplace (var a: array\<auto\>; value: auto) : auto <function-builtin_emplace_array_ls_auto_gr__auto>` 
  *  :ref:`emplace_new (var Arr: array\<smart_ptr\<auto(numT)\>\>; var value: smart_ptr\<numT\>) : auto <function-builtin_emplace_new_array_ls_smart_ptr_ls_autonumT_gr__gr__smart_ptr_ls_numT_gr_>` 
  *  :ref:`insert_default (var tab: table\<auto(TT), auto(QQ)\>; key: TT|TT#; var value: QQ ==const|QQ# ==const) <function-builtin_insert_default_table_ls_autoTT,_autoQQ_gr__TTTT_hh__QQ__eq__eq_constQQ_hh___eq__eq_const>` 
  *  :ref:`insert_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#; value: valT ==const|valT const# ==const) <function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const>` 
  *  :ref:`insert_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#) <function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`emplace_default (var tab: table\<auto(keyT), auto(valT)\>; key: keyT|keyT#) <function-builtin_emplace_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 
  *  :ref:`get_with_default (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(var p:valT&):void\>) <function-builtin_get_with_default__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_>` 
  *  :ref:`modify (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#; blk: block\<(p:valT):valT\>) <function-builtin_modify__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_valT_gr_>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: string#) : bool <function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__string_hh_>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: keyT|keyT#) : bool <function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__keyTkeyT_hh_>` 
  *  :ref:`copy_to_local (a: auto(TT)) : TT <function-builtin_copy_to_local_autoTT>` 
  *  :ref:`move_to_local (var a: auto(TT)&) : TT <function-builtin_move_to_local_autoTT>` 
  *  :ref:`keys (a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\> const# ==const) : iterator\<keyT\> <function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`keys (var a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\># ==const) : iterator\<keyT\> <function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);void\> ==const|table\<auto(keyT);void\> const# ==const) : auto <function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);void\> ==const|table\<auto(keyT);void\># ==const) : auto <function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\> const# ==const) : iterator\<valT const&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\># ==const) : iterator\<valT&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`values (a: table\<auto(keyT);auto(valT)[]\> ==const|table\<auto(keyT);auto(valT)[]\> const# ==const) : iterator\<valT const[-2]&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const>` 
  *  :ref:`values (var a: table\<auto(keyT);auto(valT)[]\> ==const|table\<auto(keyT);auto(valT)[]\># ==const) : iterator\<valT[-2]&\> <function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const>` 
  *  :ref:`lock (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; blk: block\<(t:table\<keyT, valT\>#):void\>) : auto <function-builtin_lock_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__block_ls_t_c_table_ls_keyT,_valT_gr__hh__c_void_gr_>` 
  *  :ref:`lock_forever (var Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#) : table\<keyT, valT\># <function-builtin_lock_forever_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh_>` 
  *  :ref:`next (var it: iterator\<auto(TT)\>; var value: TT&) : bool <function-builtin_next_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`each (rng: range) : iterator\<int\> <function-builtin_each_range>` 
  *  :ref:`each (rng: urange) : iterator\<uint\> <function-builtin_each_urange>` 
  *  :ref:`each (rng: range64) : iterator\<int64\> <function-builtin_each_range64>` 
  *  :ref:`each (rng: urange64) : iterator\<uint64\> <function-builtin_each_urange64>` 
  *  :ref:`each (str: string) : iterator\<int\> <function-builtin_each_string>` 
  *  :ref:`each (a: auto(TT)[]) : iterator\<TT&\> <function-builtin_each_autoTT>` 
  *  :ref:`each (a: array\<auto(TT)\>) : iterator\<TT&\> <function-builtin_each_array_ls_autoTT_gr_>` 
  *  :ref:`each (a: array\<auto(TT)\>#) : iterator\<TT&#\> <function-builtin_each_array_ls_autoTT_gr__hh_>` 
  *  :ref:`each (lam: lambda\<(var arg:auto(argT)):bool\>) : iterator\<argT\> <function-builtin_each_lambda_ls_var_arg_c_autoargT_c_bool_gr_>` 
  *  :ref:`each_ref (lam: lambda\<(var arg:auto(argT)?):bool\>) : iterator\<argT&\> <function-builtin_each_ref_lambda_ls_var_arg_c_autoargT_q__c_bool_gr_>` 
  *  :ref:`each_enum (tt: auto(TT)) : iterator\<TT\> <function-builtin_each_enum_autoTT>` 
  *  :ref:`nothing (var it: iterator\<auto(TT)\>) : iterator\<TT\> <function-builtin_nothing_iterator_ls_autoTT_gr_>` 
  *  :ref:`to_array (var it: iterator\<auto(TT)\>) : array\<TT\> <function-builtin_to_array_iterator_ls_autoTT_gr_>` 
  *  :ref:`to_array (a: auto(TT)[]) : array\<TT\> <function-builtin_to_array_autoTT>` 
  *  :ref:`to_array_move (var a: auto(TT)[]) : array\<TT\> <function-builtin_to_array_move_autoTT>` 
  *  :ref:`to_array_move (a: auto(TT) ==const) : array\<TT\> <function-builtin_to_array_move_autoTT__eq__eq_const>` 
  *  :ref:`to_array_move (var a: auto(TT) ==const) : array\<TT\> <function-builtin_to_array_move__autoTT__eq__eq_const>` 
  *  :ref:`to_table (a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT, valT\> <function-builtin_to_table_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table (a: auto(keyT)[]) : table\<keyT, void\> <function-builtin_to_table_autokeyT>` 
  *  :ref:`to_table_move (a: auto(keyT)[]) : table\<keyT, void\> <function-builtin_to_table_move_autokeyT>` 
  *  :ref:`to_table_move (a: array\<auto(keyT)\>) : table\<keyT, void\> <function-builtin_to_table_move_array_ls_autokeyT_gr_>` 
  *  :ref:`to_table_move (a: auto(keyT)) : table\<keyT, void\> <function-builtin_to_table_move_autokeyT>` 
  *  :ref:`to_table_move (var a: tuple\<auto(keyT);auto(valT)\>) : table\<keyT, valT\> <function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table_move (var a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT, valT\> <function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_>` 
  *  :ref:`to_table_move (var a: array\<tuple\<auto(keyT);auto(valT)\>\>) : table\<keyT, valT\> <function-builtin_to_table_move_array_ls_tuple_ls_autokeyT;autovalT_gr__gr_>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#) : auto <function-builtin_sort_autoTTautoTT_hh_>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#) : auto <function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh_>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#; cmp: block\<(x:TT;y:TT):bool\>) : auto <function-builtin_sort_autoTTautoTT_hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#; cmp: block\<(x:TT;y:TT):bool\>) : auto <function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_>` 
  *  :ref:`lock (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var x:array\<TT\>#):auto\>) : auto <function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_x_c_array_ls_TT_gr__hh__c_auto_gr_>` 
  *  :ref:`lock (a: array\<auto(TT)\> ==const|array\<auto(TT)\> const# ==const; blk: block\<(x:array\<TT\>#):auto\>) : auto <function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_x_c_array_ls_TT_gr__hh__c_auto_gr_>` 
  *  :ref:`find_index (arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : auto <function-builtin_find_index_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT>` 
  *  :ref:`find_index (arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto <function-builtin_find_index_autoTTautoTT_hh__TT>` 
  *  :ref:`find_index (var arr: iterator\<auto(TT)\>; key: TT) : auto <function-builtin_find_index_iterator_ls_autoTT_gr__TT>` 
  *  :ref:`find_index_if (arr: array\<auto(TT)\>|array\<auto(TT)\>#; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`find_index_if (arr: auto(TT)[]|auto(TT)[]#; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_autoTTautoTT_hh__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`find_index_if (var arr: iterator\<auto(TT)\>; blk: block\<(key:TT):bool\>) : auto <function-builtin_find_index_if_iterator_ls_autoTT_gr__block_ls_key_c_TT_c_bool_gr_>` 
  *  :ref:`has_value (a: auto; key: auto) : auto <function-builtin_has_value_auto_auto>` 
  *  :ref:`has_value (var a: iterator\<auto\>; key: auto) : auto <function-builtin_has_value_iterator_ls_auto_gr__auto>` 
  *  :ref:`subarray (a: auto(TT)[]; r: range) : auto <function-builtin_subarray_autoTT_range>` 
  *  :ref:`subarray (a: auto(TT)[]; r: urange) : auto <function-builtin_subarray_autoTT_urange>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: range) : auto <function-builtin_subarray_array_ls_autoTT_gr__range>` 
  *  :ref:`subarray (var a: array\<auto(TT)\>; r: range) : auto <function-builtin_subarray__array_ls_autoTT_gr__range>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: urange) : auto <function-builtin_subarray_array_ls_autoTT_gr__urange>` 
  *  :ref:`move_to_ref (var a: auto&; var b: auto) : auto <function-builtin_move_to_ref_auto_auto>` 
  *  :ref:`clear (var t: table\<auto(KT), auto(VT)\>) : auto <function-builtin_clear_table_ls_autoKT,_autoVT_gr_>` 

.. _function-builtin_clear_array_ls_anything_gr_:

.. das:function:: clear(array: array<anything>)

Removes all elements from the dynamic `array`, leaving it empty with a size of 0.

:Arguments: * **array** : array implicit

.. _function-builtin_length_array_ls_anything_gr_:

.. das:function:: length(array: array<anything>) : int

Returns the number of elements currently stored in the given dynamic `array`.

:Arguments: * **array** : array implicit

.. _function-builtin_capacity_array_ls_anything_gr_:

.. das:function:: capacity(array: array<anything>) : int

Returns the current capacity of the `array` — the number of elements it can hold before triggering a reallocation.

:Arguments: * **array** : array implicit

.. _function-builtin_empty_iterator_implicit:

.. das:function:: empty(iterator: iterator implicit) : bool

Returns `true` if the iterator is empty, meaning it would not produce any more values or has not been initialized.

:Arguments: * **iterator** : iterator implicit

.. _function-builtin_length_table_ls_anything,_anything_gr_:

.. das:function:: length(table: table<anything, anything>) : int

Returns the number of key-value pairs currently stored in the given `table`.

:Arguments: * **table** : table implicit

.. _function-builtin_capacity_table_ls_anything,_anything_gr_:

.. das:function:: capacity(table: table<anything, anything>) : int

Returns the current capacity of the `table` — the number of key-value pairs it can hold before triggering a reallocation.

:Arguments: * **table** : table implicit

.. _function-builtin_empty_string_implicit:

.. das:function:: empty(str: string implicit) : bool

Checks whether the string `str` is empty (null or zero-length) and returns `true` if so.

:Arguments: * **str** : string implicit

.. _function-builtin_empty_das_string_implicit:

.. das:function:: empty(str: das_string implicit) : bool

Checks whether the `das_string` is empty (has zero length) and returns `true` if so.

:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-builtin_resize_array_ls_autonumT_gr__int:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int) : auto

Resizes dynamic array `Arr` to `newSize` elements; new elements beyond the previous length are zero-initialized, and excess elements are removed.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_resize_and_init_array_ls_autonumT_gr__int:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int) : auto

Resizes dynamic array `Arr` to `newSize` elements, initializing any newly added elements with their default constructor.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_resize_and_init_array_ls_autonumT_gr__int_numT:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int; initValue: numT) : auto

Resizes dynamic array `Arr` to `newSize` elements, initializing any newly added elements with the provided `initValue`.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

            * **initValue** : numT

.. _function-builtin_resize_no_init_array_ls_autonumT_gr__int:

.. das:function:: resize_no_init(Arr: array<auto(numT)>; newSize: int) : auto

Resizes dynamic array `Arr` to `newSize` elements without initializing newly added entries, leaving their memory contents undefined.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_reserve_array_ls_autonumT_gr__int:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int) : auto

Pre-allocates memory in `Arr` to hold at least `newSize` elements without reallocation, improving performance of subsequent push operations.

:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-builtin_pop_array_ls_autonumT_gr_:

.. das:function:: pop(Arr: array<auto(numT)>) : auto

Removes and discards the last element of `Arr`, reducing its length by one.

:Arguments: * **Arr** : array<auto(numT)>

.. _function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const_int:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const; at: int) : auto

Inserts `value` into dynamic array `Arr` at the specified index `at`, shifting subsequent elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

            * **at** : int

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const_int:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const; at: int) : auto

Inserts a mutable `value` into dynamic array `Arr` at the specified index `at`, shifting subsequent elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

            * **at** : int

.. _function-builtin_push_array_ls_autonumT_gr__numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const) : auto

Appends a single `value` to the end of dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)>; value: numT ==const) : auto

Appends a mutable `value` to the end of dynamic array `Arr`, moving it if possible.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT!

.. _function-builtin_push_array_ls_autonumT_gr__array_ls_numT_gr_:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT>) : auto

Appends all elements of dynamic array `varr` to the end of dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : array<numT>!

.. _function-builtin_push_array_ls_autonumT_gr___array_ls_numT_gr_:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT>) : auto

Appends all elements of mutable dynamic array `varr` to the end of dynamic array `Arr`, moving elements when possible.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : array<numT>!

.. _function-builtin_push_array_ls_autonumT_gr__numT:

.. das:function:: push(Arr: array<auto(numT)>; varr: numT[]) : auto

Appends all elements of the static array `varr` to the end of dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]

.. _function-builtin_push_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT const[] ==const) : auto

Appends a constant fixed-size array element `varr` to the end of `Arr`, an array of fixed-size arrays.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_push_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT[] ==const) : auto

Appends a mutable fixed-size array element `varr` to the end of `Arr`, an array of fixed-size arrays.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_emplace_array_ls_autonumT_gr__numT_int:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&; at: int) : auto

Moves `value` into the dynamic array `Arr` at index `at` using move semantics, shifting subsequent elements forward.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

            * **at** : int

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&) : auto

Moves `value` into the end of the dynamic array `Arr` using move semantics, avoiding unnecessary copies.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT[]) : auto

Moves a fixed-size array `value` into the end of the dynamic array `Arr` of fixed-size arrays using move semantics.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT[-1]

.. _function-builtin_emplace_array_ls_autonumT_gr__numT:

.. das:function:: emplace(Arr: array<auto(numT)[]>; value: numT[]) : auto

Moves a fixed-size array `value` into the end of the dynamic array `Arr` using move semantics, avoiding unnecessary copies.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **value** : numT[-1]

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const_int:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT const# ==const; at: int) : auto

Clones a constant `value` and inserts the copy into dynamic array `Arr` at the specified index `at`, shifting subsequent elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

            * **at** : int

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const_int:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const; at: int) : auto

Clones `value` and inserts the copy into dynamic array `Arr` at the specified index `at`, shifting subsequent elements.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

            * **at** : int

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_const_hh___eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT const# ==const) : auto

Clones a constant `value` and appends the deep copy to the end of dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT__eq__eq_constnumT_hh___eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT ==const|numT# ==const) : auto

Clones a mutable `value` and appends the deep copy to the end of dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT!|numT#!>

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT const[] ==const) : auto

Clones all elements of a constant static array `varr` and appends them to dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr___numT__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT[] ==const) : auto

Clones all elements of a mutable static array `varr` and appends them to dynamic array `Arr`.

:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT_const__eq__eq_const:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT const[] ==const) : auto

Clones a constant fixed-size array `varr` and appends the copy to `Arr`, an array of fixed-size arrays.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]!

.. _function-builtin_push_clone_array_ls_autonumT_gr__numT:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT[]) : auto

Clones a mutable fixed-size array `varr` and appends the copy to `Arr`, an array of fixed-size arrays.

:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]

.. _function-builtin_push_clone_autoCT_autoTTautoTT_hh_:

.. das:function:: push_clone(A: auto(CT); b: auto(TT)|auto(TT)#) : auto

Clones and appends element `b` to the container `A`, using deep copy semantics rather than move or shallow copy.

:Arguments: * **A** : auto(CT)

            * **b** : option<auto(TT)|auto(TT)#>

.. _function-builtin_back__array_ls_autoTT_gr_:

.. das:function:: back(a: array<auto(TT)>) : TT&

Accesses and returns a mutable reference to the last element of the dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>!

.. _function-builtin_back__array_ls_autoTT_gr__hh_:

.. das:function:: back(a: array<auto(TT)>#) : TT&#

Accesses and returns a temporary mutable reference to the last element of the temporary dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>#!

.. _function-builtin_back_array_ls_autoTT_gr_:

.. das:function:: back(a: array<auto(TT)>) : TT

Accesses and returns by value the last element of the dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>!

.. _function-builtin_back_array_ls_autoTT_gr__hh_:

.. das:function:: back(a: array<auto(TT)>#) : TT const&#

Accesses and returns a const temporary reference to the last element of the temporary dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>#!

.. _function-builtin_back__autoTT__eq__eq_const:

.. das:function:: back(arr: auto(TT) ==const) : auto&

Accesses and returns a mutable reference to the last element of the fixed-size array or static array `arr`.

:Arguments: * **arr** : auto(TT)!

.. _function-builtin_back_autoTT__eq__eq_const:

.. das:function:: back(arr: auto(TT) ==const) : auto

Accesses and returns the last element of the fixed-size array or static array `arr` by value.

:Arguments: * **arr** : auto(TT)!

.. _function-builtin_erase_array_ls_autonumT_gr__int:

.. das:function:: erase(Arr: array<auto(numT)>; at: int) : auto

Erases the element at index `at` from the array `Arr`, shifting subsequent elements to fill the gap.

:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

.. _function-builtin_erase_array_ls_autonumT_gr__int_int:

.. das:function:: erase(Arr: array<auto(numT)>; at: int; count: int) : auto

Erases `count` elements from the array `Arr` starting at index `at`, shifting subsequent elements to fill the gap.

:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

            * **count** : int

.. _function-builtin_erase_if_array_ls_autoTT_gr__block_ls_key_c_TT_const_c_bool_gr_block_ls_var_key_c_TT_c_bool_gr_:

.. das:function:: erase_if(arr: array<auto(TT)>; blk: block<(key:TT const):bool>|block<(var key:TT&):bool>) : auto

Iterates over the array `arr` and removes all elements for which the block `blk` returns `true`.

:Arguments: * **arr** : array<auto(TT)>

            * **blk** : option<block<(key:TT):bool>|block<(key:TT&):bool>>

.. _function-builtin_remove_value_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT:

.. das:function:: remove_value(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : bool

Searches `arr` for the first element equal to `key` and removes it, returning true if an element was found and removed or false otherwise.

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-builtin_length_autoauto_hh_:

.. das:function:: length(a: auto|auto#) : int

Returns the number of elements currently stored in a table or dynamic array `a`.

:Arguments: * **a** : option<auto|auto#>

.. _function-builtin_empty_array_ls_auto_gr_array_ls_auto_gr__hh_:

.. das:function:: empty(a: array<auto>|array<auto>#) : bool

Checks whether the array `a` contains no elements and returns `true` if it is empty.

:Arguments: * **a** : option<array<auto>|array<auto>#>

.. _function-builtin_empty_table_ls_auto;auto_gr_table_ls_auto;auto_gr__hh_:

.. das:function:: empty(a: table<auto;auto>|table<auto;auto>#) : bool

Checks whether the table `a` contains no entries and returns `true` if it is empty.

:Arguments: * **a** : option<table<auto;auto>|table<auto;auto>#>

.. _function-builtin_reserve_table_ls_autokeyT,_auto_gr__int:

.. das:function:: reserve(Tab: table<auto(keyT), auto>; newSize: int) : auto

Pre-allocates memory in `Tab` to hold at least `newSize` entries without rehashing, improving performance of subsequent insertions.

:Arguments: * **Tab** : table<auto(keyT);auto>

            * **newSize** : int

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>#; at: keyT|keyT#; blk: block<(p:valT const&#):void>) : auto

Looks up `at` in the temporary table `Tab` and, if found, invokes `blk` with a const handle reference to the value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&#):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(p:valT):void>) : auto

Looks up `at` in the table `Tab` and, if found, invokes `blk` with a reference to the value; returns `true` if the key existed, `false` otherwise.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>#; at: keyT|keyT#; blk: block<(var p:valT&#):void>) : auto

Looks up `at` in the temporary table `Tab` and, if found, invokes `blk` with a mutable handle reference to the value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&#):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(var p:valT&):void>) : auto

Looks up `at` in the table `Tab` and, if found, invokes `blk` with a mutable reference to the value; returns `true` if the key existed, `false` otherwise.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_p_c_valT_const-2_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>#; at: keyT|keyT#; blk: block<(p:valT const[-2]&#):void>) : auto

Looks up `at` in the temporary table `Tab` (whose values are fixed-size arrays) and, if found, invokes `blk` with a const handle reference to the array value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&#):void>

.. _function-builtin_get_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_const-2_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; blk: block<(p:valT const[-2]&):void>) : auto

Looks up `at` in the table `Tab` (whose values are fixed-size arrays) and, if found, invokes `blk` with a const reference to the array value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__hh__keyTkeyT_hh__block_ls_var_p_c_valT-2_hh__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>#; at: keyT|keyT#; blk: block<(var p:valT[-2]&#):void>) : auto

Looks up `at` in the temporary table `Tab` (whose values are fixed-size arrays) and, if found, invokes `blk` with a mutable reference to the array value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>#!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&#):void>

.. _function-builtin_get__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT-2_c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; blk: block<(var p:valT[-2]&):void>) : auto

Looks up `at` in the table `Tab` (whose values are fixed-size arrays) and, if found, invokes `blk` with a mutable reference to the array value; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT[-2]&):void>

.. _function-builtin_get_table_ls_autokeyT,_void_gr__keyTkeyT_hh__block_ls_var_p_c_void_q__c_void_gr_:

.. das:function:: get(Tab: table<auto(keyT), void>; at: keyT|keyT#; blk: block<(var p:void?):void>) : auto

Looks up `at` in the set-style table `Tab` (with `void` values) and, if found, invokes `blk` with a void pointer; returns `true` if the key existed.

:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT

Retrieves the value associated with key `at` from the const table `Tab`, returning a copy of the value.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), smart_ptr<auto(valT)>>; at: keyT|keyT#) : smart_ptr<valT>

Retrieves and moves the `smart_ptr` value associated with key `at` from the mutable table `Tab`, removing it from the table.

:Arguments: * **Tab** : table<auto(keyT);smart_ptr<auto(valT)>>

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT

Retrieves the value associated with key `at` from the mutable table `Tab`, returning a copy of the value.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

.. _function-builtin_get_value_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: get_value(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#) : valT[-2]

Retrieves the fixed-size array value associated with key `at` from the mutable table `Tab`.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

.. _function-builtin_erase_table_ls_autokeyT,_autovalT_gr__string_hh_:

.. das:function:: erase(Tab: table<auto(keyT), auto(valT)>; at: string#) : bool

Removes the entry with the string key `at` from the table `Tab`, returning `true` if the key was found and erased.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : string#

.. _function-builtin_erase_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: erase(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : bool

Removes the entry with key `at` from the table `Tab`, returning `true` if the key was found and erased.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

.. _function-builtin_insert_table_ls_autokeyT,_void_gr__keyTkeyT_hh_:

.. das:function:: insert(Tab: table<auto(keyT), void>; at: keyT|keyT#) : auto

Inserts the key `at` into the set-style table `Tab` (a table with `void` values), effectively adding to a set.

:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT# ==const) : auto

Inserts or updates an entry in the table `Tab` at key `at` by cloning the mutable value `val` into the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto

Inserts or updates an entry in the table `Tab` at key `at` by cloning the const value `val` into the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[] ==const|valT[]# ==const) : auto

Inserts or updates an entry in the table `Tab` at key `at` by cloning the mutable fixed-size array `val` into the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_clone_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_clone(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto

Inserts or updates an entry in the table `Tab` at key `at` by cloning the const fixed-size array `val` into the table.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT# ==const) : auto

Inserts or updates an entry in the table `Tab` by moving the mutable value `val` at the specified key `at`.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT ==const|valT const# ==const) : auto

Inserts or updates an entry in the table `Tab` by copying the const value `val` at the specified key `at`.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT!|valT#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[] ==const|valT[]# ==const) : auto

Inserts or updates an entry in the table `Tab` by moving a mutable fixed-size array `val` at the specified key `at`.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_insert_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT_const__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT const[] ==const|valT const[]# ==const) : auto

Inserts or updates an entry in the table `Tab` by copying a const fixed-size array `val` at the specified key `at`.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : option<valT[-1]!|valT[-1]#!>

.. _function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT:

.. das:function:: emplace(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; val: valT&) : auto

Moves `val` into the table `Tab` at key `at` using move semantics, avoiding unnecessary copies.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

            * **val** : valT&

.. _function-builtin_emplace_table_ls_autokeyT,_smart_ptr_ls_autovalT_gr__gr__keyTkeyT_hh__smart_ptr_ls_valT_gr_:

.. das:function:: emplace(Tab: table<auto(keyT), smart_ptr<auto(valT)>>; at: keyT|keyT#; val: smart_ptr<valT>&) : auto

Moves the smart pointer `val` into the table `Tab` at key `at`, transferring ownership of the pointed-to object into the table entry.

:Arguments: * **Tab** : table<auto(keyT);smart_ptr<auto(valT)>>

            * **at** : option<keyT|keyT#>

            * **val** : smart_ptr<valT>&

.. _function-builtin_emplace_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT:

.. das:function:: emplace(Tab: table<auto(keyT), auto(valT)[]>; at: keyT|keyT#; val: valT[]&) : auto

Moves the fixed-size array `val` into the table `Tab` at key `at` using move semantics.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)[-1]>

            * **at** : option<keyT|keyT#>

            * **val** : valT[-1]&

.. _function-builtin_emplace_new_table_ls_autokT,_smart_ptr_ls_autovT_gr__gr__kT_smart_ptr_ls_vT_gr_:

.. das:function:: emplace_new(tab: table<auto(kT), smart_ptr<auto(vT)>>; key: kT; value: smart_ptr<vT>) : auto

Moves a smart pointer `value` into the table `tab` at the given `key`, constructing the entry in-place and returning a reference to it.

:Arguments: * **tab** : table<auto(kT);smart_ptr<auto(vT)>>

            * **key** : kT

            * **value** : smart_ptr<vT>

.. _function-builtin_emplace_table_ls_auto,_auto_gr__auto_auto:

.. das:function:: emplace(Tab: table<auto, auto>; key: auto; value: auto) : auto

Moves `value` into the table `Tab` at the specified `key` using move semantics, avoiding unnecessary copies.

:Arguments: * **Tab** : table<auto;auto>

            * **key** : auto

            * **value** : auto

.. _function-builtin_emplace_array_ls_auto_gr__auto:

.. das:function:: emplace(a: array<auto>; value: auto) : auto

Appends `value` to the end of the dynamic array `a` using move semantics when possible.

:Arguments: * **a** : array<auto>

            * **value** : auto

.. _function-builtin_emplace_new_array_ls_smart_ptr_ls_autonumT_gr__gr__smart_ptr_ls_numT_gr_:

.. das:function:: emplace_new(Arr: array<smart_ptr<auto(numT)>>; value: smart_ptr<numT>) : auto

Moves a smart pointer `value` into the end of the array `Arr`, constructing the entry in-place and returning a reference to it.

:Arguments: * **Arr** : array<smart_ptr<auto(numT)>>

            * **value** : smart_ptr<numT>

.. _function-builtin_insert_default_table_ls_autoTT,_autoQQ_gr__TTTT_hh__QQ__eq__eq_constQQ_hh___eq__eq_const:

.. das:function:: insert_default(tab: table<auto(TT), auto(QQ)>; key: TT|TT#; value: QQ ==const|QQ# ==const)

Inserts key `key` with the given mutable `value` into table `tab` only if the key does not already exist; existing entries are left unchanged.

:Arguments: * **tab** : table<auto(TT);auto(QQ)>

            * **key** : option<TT|TT#>

            * **value** : option<QQ!|QQ#!>

.. _function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__valT__eq__eq_constvalT_const_hh___eq__eq_const:

.. das:function:: insert_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#; value: valT ==const|valT const# ==const)

Inserts key `key` with the given const `value` into table `tab` only if the key does not already exist; existing entries are left unchanged.

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

            * **value** : option<valT!|valT#!>

.. _function-builtin_insert_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: insert_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#)

Inserts key `key` with a default-initialized value into table `tab` only if the key does not already exist; existing entries are left unchanged.

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

.. _function-builtin_emplace_default_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: emplace_default(tab: table<auto(keyT), auto(valT)>; key: keyT|keyT#)

Constructs a new default-initialized element in the table `tab` at the given `key`, only if that key does not already exist.

:Arguments: * **tab** : table<auto(keyT);auto(valT)>

            * **key** : option<keyT|keyT#>

.. _function-builtin_get_with_default__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_var_p_c_valT_c_void_gr_:

.. das:function:: get_with_default(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(var p:valT&):void>)

Looks up key `at` in the table `Tab`, inserting a default-initialized entry if the key is absent, then invokes `blk` with a mutable reference to the value.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):void>

.. _function-builtin_modify__table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh__block_ls_p_c_valT_c_valT_gr_:

.. das:function:: modify(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#; blk: block<(p:valT):valT>)

Looks up `at` in `Tab` and, if found, invokes `blk` with the current value, replacing it with the value returned by the block.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:valT&):valT>

.. _function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__string_hh_:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: string#) : bool

Checks whether the string key `at` exists in the table `Tab` and returns `true` if found.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : string#

.. _function-builtin_key_exists_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__keyTkeyT_hh_:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: keyT|keyT#) : bool

Checks whether the key `at` exists in the table `Tab` and returns `true` if found.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : option<keyT|keyT#>

.. _function-builtin_copy_to_local_autoTT:

.. das:function:: copy_to_local(a: auto(TT)) : TT

Copies the value `a` and returns it as a new local value on the stack, which can be used to work around aliasing issues where a reference might be invalidated.

:Arguments: * **a** : auto(TT)

.. _function-builtin_move_to_local_autoTT:

.. das:function:: move_to_local(a: auto(TT)&) : TT

Moves the value referenced by `a` onto the stack as a local copy and returns it, clearing the original; useful for resolving aliasing issues.

:Arguments: * **a** : auto(TT)&

.. _function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)> const# ==const) : iterator<keyT>

Creates an iterator over all keys of the const table `a`, allowing enumeration of the table's key set.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_keys_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: keys(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const) : iterator<keyT>

Creates an iterator over all keys of the mutable table `a`, allowing enumeration of the table's key set.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void> const# ==const) : auto

Returns an iterator over all entries in a constant `table<keyT, void>` used as a set.

:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-builtin_values_table_ls_autokeyT;void_gr___eq__eq_consttable_ls_autokeyT;void_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void># ==const) : auto

Returns an iterator over all entries in a mutable `table<keyT, void>` used as a set.

:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)> const# ==const) : iterator<valT const&>

Returns a read-only iterator over all values in the given constant `table<keyT, valT>`, yielding each value by const reference.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const) : iterator<valT&>

Returns a mutable iterator over all values in the given mutable `table<keyT, valT>`, yielding each value by reference.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__const_hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> ==const|table<auto(keyT);auto(valT)[]> const# ==const) : iterator<valT const[-2]&>

Returns a read-only iterator over all fixed-size array values in a constant `table<keyT, valT[]>`, yielding each array by const reference.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)[-1]>!|table<auto(keyT);auto(valT)[-1]>#!>

.. _function-builtin_values_table_ls_autokeyT;autovalT_gr___eq__eq_consttable_ls_autokeyT;autovalT_gr__hh___eq__eq_const:

.. das:function:: values(a: table<auto(keyT);auto(valT)[]> ==const|table<auto(keyT);auto(valT)[]># ==const) : iterator<valT[-2]&>

Returns a mutable iterator over all fixed-size array values in a mutable `table<keyT, valT[]>`, yielding each array by reference.

:Arguments: * **a** : option<table<auto(keyT);auto(valT)[-1]>!|table<auto(keyT);auto(valT)[-1]>#!>

.. _function-builtin_lock_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh__block_ls_t_c_table_ls_keyT,_valT_gr__hh__c_void_gr_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; blk: block<(t:table<keyT, valT>#):void>) : auto

Locks a table for the duration of `blk`, preventing insertions or deletions while providing read-only access through a temporary reference.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **blk** : block<(t:table<keyT;valT>#):void>

.. _function-builtin_lock_forever_table_ls_autokeyT;autovalT_gr_table_ls_autokeyT;autovalT_gr__hh_:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#) : table<keyT, valT>#

Permanently locks a table, preventing any future insertions, deletions, or structural modifications, and returns a temporary reference to it.

:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

.. _function-builtin_next_iterator_ls_autoTT_gr__TT:

.. das:function:: next(it: iterator<auto(TT)>; value: TT&) : bool

Advances the iterator `it` and stores the next element in `value`, returning true if an element was retrieved or false if the iterator is exhausted or null.

:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT&

.. _function-builtin_each_range:

.. das:function:: each(rng: range) : iterator<int>

Creates an iterator that yields each `int` value in the range `rng`.

:Arguments: * **rng** : range

.. _function-builtin_each_urange:

.. das:function:: each(rng: urange) : iterator<uint>

Creates an iterator that yields each `uint` value in the unsigned range `rng`.

:Arguments: * **rng** : urange

.. _function-builtin_each_range64:

.. das:function:: each(rng: range64) : iterator<int64>

Creates an iterator that yields each `int64` value in the 64-bit range `rng`.

:Arguments: * **rng** : range64

.. _function-builtin_each_urange64:

.. das:function:: each(rng: urange64) : iterator<uint64>

Creates an iterator that yields each `uint64` value in the 64-bit unsigned range `rng`.

:Arguments: * **rng** : urange64

.. _function-builtin_each_string:

.. das:function:: each(str: string) : iterator<int>

Creates an iterator that yields the Unicode codepoint (as `int`) of each character in the string `str`.

:Arguments: * **str** : string

.. _function-builtin_each_autoTT:

.. das:function:: each(a: auto(TT)[]) : iterator<TT&>

Creates an iterator that yields mutable references to each element of the fixed-size (static) array `a`.

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_each_array_ls_autoTT_gr_:

.. das:function:: each(a: array<auto(TT)>) : iterator<TT&>

Creates an iterator that yields mutable references to each element of the dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>

.. _function-builtin_each_array_ls_autoTT_gr__hh_:

.. das:function:: each(a: array<auto(TT)>#) : iterator<TT&#>

Creates an iterator that yields temporary references to each element of the temporary dynamic array `a`.

:Arguments: * **a** : array<auto(TT)>#

.. _function-builtin_each_lambda_ls_var_arg_c_autoargT_c_bool_gr_:

.. das:function:: each(lam: lambda<(var arg:auto(argT)):bool>) : iterator<argT>

Wraps a lambda `lam` — which receives a mutable argument and returns a bool indicating whether to continue — into an iterator that yields values until the lambda returns false.

:Arguments: * **lam** : lambda<(arg:auto(argT)):bool>

.. _function-builtin_each_ref_lambda_ls_var_arg_c_autoargT_q__c_bool_gr_:

.. das:function:: each_ref(lam: lambda<(var arg:auto(argT)?):bool>) : iterator<argT&>

Wraps a lambda `lam` — which receives a mutable pointer argument and returns a bool indicating whether to continue — into an iterator that yields references to each value rather than copies.

:Arguments: * **lam** : lambda<(arg:auto(argT)?):bool>

.. _function-builtin_each_enum_autoTT:

.. das:function:: each_enum(tt: auto(TT)) : iterator<TT>

.. warning:: 
  This function is deprecated.

Creates an iterator that yields every value of the enumeration type inferred from `tt`, allowing iteration over all members of an enum.

:Arguments: * **tt** : auto(TT)

.. _function-builtin_nothing_iterator_ls_autoTT_gr_:

.. das:function:: nothing(it: iterator<auto(TT)>) : iterator<TT>

Produces an empty iterator of the same element type as `it` that yields no elements.

:Arguments: * **it** : iterator<auto(TT)>

.. _function-builtin_to_array_iterator_ls_autoTT_gr_:

.. das:function:: to_array(it: iterator<auto(TT)>) : array<TT>

Consumes iterator `it` and collects all yielded elements into a new dynamic array by cloning each one.

:Arguments: * **it** : iterator<auto(TT)>

.. _function-builtin_to_array_autoTT:

.. das:function:: to_array(a: auto(TT)[]) : array<TT>

Converts a fixed-size array `a` into a new dynamic array by cloning each element.

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_to_array_move_autoTT:

.. das:function:: to_array_move(a: auto(TT)[]) : array<TT>

Converts a mutable fixed-size array `a` into a new dynamic array, moving each element instead of cloning.

:Arguments: * **a** : auto(TT)[-1]

.. _function-builtin_to_array_move_autoTT__eq__eq_const:

.. das:function:: to_array_move(a: auto(TT) ==const) : array<TT>

Converts container `a` into a new dynamic array, copying elements into the result.

:Arguments: * **a** : auto(TT)!

.. _function-builtin_to_array_move__autoTT__eq__eq_const:

.. das:function:: to_array_move(a: auto(TT) ==const) : array<TT>

Converts a mutable container `a` into a new dynamic array, moving elements when possible instead of cloning.

:Arguments: * **a** : auto(TT)!

.. _function-builtin_to_table_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT, valT>

Converts a fixed-size array of key-value tuples `a` into a `table<keyT, valT>` by cloning each key and value.

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-builtin_to_table_autokeyT:

.. das:function:: to_table(a: auto(keyT)[]) : table<keyT, void>

Converts a fixed-size array of keys `a` into a `table<keyT, void>` set by cloning each key element.

:Arguments: * **a** : auto(keyT)[-1]

.. _function-builtin_to_table_move_autokeyT:

.. das:function:: to_table_move(a: auto(keyT)[]) : table<keyT, void>

Converts a fixed-size array of keys `a` into a `table<keyT, void>` set, copying or moving each key.

:Arguments: * **a** : auto(keyT)[-1]

.. _function-builtin_to_table_move_array_ls_autokeyT_gr_:

.. das:function:: to_table_move(a: array<auto(keyT)>) : table<keyT, void>

Converts a dynamic array of keys `a` into a `table<keyT, void>` set, copying or moving each key.

:Arguments: * **a** : array<auto(keyT)>

.. _function-builtin_to_table_move_autokeyT:

.. das:function:: to_table_move(a: auto(keyT)) : table<keyT, void>

Converts a generic key container `a` into a `table<keyT, void>` set, copying or moving each key.

:Arguments: * **a** : auto(keyT)

.. _function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>) : table<keyT, valT>

Converts a single mutable key-value tuple `a` into a `table<keyT, valT>` containing one entry, moving the key and value.

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>

.. _function-builtin_to_table_move_tuple_ls_autokeyT;autovalT_gr_:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT, valT>

Converts a mutable fixed-size array of key-value tuples `a` into a `table<keyT, valT>`, moving elements when possible.

:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-builtin_to_table_move_array_ls_tuple_ls_autokeyT;autovalT_gr__gr_:

.. das:function:: to_table_move(a: array<tuple<auto(keyT);auto(valT)>>) : table<keyT, valT>

Converts a mutable dynamic array of key-value tuples `a` into a `table<keyT, valT>`, moving elements when possible.

:Arguments: * **a** : array<tuple<auto(keyT);auto(valT)>>

.. _function-builtin_sort_autoTTautoTT_hh_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#) : auto

Sorts a fixed-size array in place in ascending order using the default comparison for its element type.

:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

.. _function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#) : auto

Sorts a dynamic array in place in ascending order using the default comparison for its element type.

:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

.. _function-builtin_sort_autoTTautoTT_hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#; cmp: block<(x:TT;y:TT):bool>) : auto

Sorts a fixed-size array in place using the custom comparison block `cmp`, where `cmp` returns true when `x` should precede `y`.

:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-builtin_sort_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_x_c_TT;y_c_TT_c_bool_gr_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#; cmp: block<(x:TT;y:TT):bool>) : auto

Sorts a dynamic array in place using the custom comparison block `cmp`, where `cmp` returns true when `x` should precede `y`.

:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_x_c_array_ls_TT_gr__hh__c_auto_gr_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var x:array<TT>#):auto>) : auto

Locks a mutable array for the duration of `blk`, preventing resizing, push, or pop operations while allowing element access through a temporary reference.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-builtin_lock_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_x_c_array_ls_TT_gr__hh__c_auto_gr_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)> const# ==const; blk: block<(x:array<TT>#):auto>) : auto

Locks a constant array for the duration of `blk`, preventing structural modifications while providing read-only access through a temporary reference.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-builtin_find_index_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__TT:

.. das:function:: find_index(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : auto

Searches the dynamic array `arr` for the first occurrence of `key` and returns its index, or -1 if not found.

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-builtin_find_index_autoTTautoTT_hh__TT:

.. das:function:: find_index(arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto

Searches the fixed-size array `arr` for the first occurrence of `key` and returns its index, or -1 if not found.

:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **key** : TT

.. _function-builtin_find_index_iterator_ls_autoTT_gr__TT:

.. das:function:: find_index(arr: iterator<auto(TT)>; key: TT) : auto

Consumes elements from the iterator `arr` and returns the index of the first element equal to `key`, or -1 if not found.

:Arguments: * **arr** : iterator<auto(TT)>

            * **key** : TT

.. _function-builtin_find_index_if_array_ls_autoTT_gr_array_ls_autoTT_gr__hh__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: array<auto(TT)>|array<auto(TT)>#; blk: block<(key:TT):bool>) : auto

Searches the dynamic array `arr` and returns the index of the first element for which the block `blk` returns `true`, or -1 if no match is found.

:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_find_index_if_autoTTautoTT_hh__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: auto(TT)[]|auto(TT)[]#; blk: block<(key:TT):bool>) : auto

Searches the fixed-size array `arr` and returns the index of the first element for which the block `blk` returns `true`, or -1 if no match is found.

:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_find_index_if_iterator_ls_autoTT_gr__block_ls_key_c_TT_c_bool_gr_:

.. das:function:: find_index_if(arr: iterator<auto(TT)>; blk: block<(key:TT):bool>) : auto

Consumes elements from the iterator `arr` and returns the index of the first element for which the block `blk` returns `true`, or -1 if no match is found.

:Arguments: * **arr** : iterator<auto(TT)>

            * **blk** : block<(key:TT):bool>

.. _function-builtin_has_value_auto_auto:

.. das:function:: has_value(a: auto; key: auto) : auto

Checks whether the iterable container `a` (array, fixed-size array, etc.) contains the element `key`, returning `true` if found.

:Arguments: * **a** : auto

            * **key** : auto

.. _function-builtin_has_value_iterator_ls_auto_gr__auto:

.. das:function:: has_value(a: iterator<auto>; key: auto) : auto

Consumes elements from the iterator `a` and returns `true` if any element equals `key`.

:Arguments: * **a** : iterator<auto>

            * **key** : auto

.. _function-builtin_subarray_autoTT_range:

.. das:function:: subarray(a: auto(TT)[]; r: range) : auto

Creates and returns a new dynamic array containing a copy of elements from fixed-size array `a` within the signed range `r`.

:Arguments: * **a** : auto(TT)[-1]

            * **r** : range

.. _function-builtin_subarray_autoTT_urange:

.. das:function:: subarray(a: auto(TT)[]; r: urange) : auto

Creates and returns a new dynamic array containing a copy of elements from fixed-size array `a` within the unsigned range `r`.

:Arguments: * **a** : auto(TT)[-1]

            * **r** : urange

.. _function-builtin_subarray_array_ls_autoTT_gr__range:

.. das:function:: subarray(a: array<auto(TT)>; r: range) : auto

Creates and returns a new dynamic array containing a copy of elements from `a` within the signed range `r`.

:Arguments: * **a** : array<auto(TT)>!

            * **r** : range

.. _function-builtin_subarray__array_ls_autoTT_gr__range:

.. das:function:: subarray(a: array<auto(TT)>; r: range) : auto

Creates and returns a new dynamic array containing a copy of elements from mutable array `a` within the signed range `r`.

:Arguments: * **a** : array<auto(TT)>!

            * **r** : range

.. _function-builtin_subarray_array_ls_autoTT_gr__urange:

.. das:function:: subarray(a: array<auto(TT)>; r: urange) : auto

Creates and returns a new dynamic array containing a copy of elements from `a` within the unsigned range `r`.

:Arguments: * **a** : array<auto(TT)>

            * **r** : urange

.. _function-builtin_move_to_ref_auto_auto:

.. das:function:: move_to_ref(a: auto&; b: auto) : auto

Moves `b` into the reference `a`; if `b` is a value type rather than a reference, it is copied instead of moved.

:Arguments: * **a** : auto&

            * **b** : auto

.. _function-builtin_clear_table_ls_autoKT,_autoVT_gr_:

.. das:function:: clear(t: table<auto(KT), auto(VT)>) : auto

Removes all key-value pairs from the table `t`, leaving it empty with a size of 0.

:Arguments: * **t** : table<auto(KT);auto(VT)>

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`peek (src: das_string implicit; block: block\<(string#):void\>) <function-builtin_peek_das_string_implicit_block_ls_string_hh__c_void_gr_>` 

.. _function-builtin_peek_das_string_implicit_block_ls_string_hh__c_void_gr_:

.. das:function:: peek(src: das_string implicit; block: block<(string#):void>)

Provides zero-copy read access to the contents of a `das_string` by invoking `block` with a temporary string reference, avoiding allocation.

:Arguments: * **src** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **block** : block<(string#):void> implicit

++++++++++++++
Heap reporting
++++++++++++++

  *  :ref:`heap_allocation_stats () : urange64 <function-builtin_heap_allocation_stats>` 
  *  :ref:`heap_allocation_count () : uint64 <function-builtin_heap_allocation_count>` 
  *  :ref:`string_heap_allocation_stats () : urange64 <function-builtin_string_heap_allocation_stats>` 
  *  :ref:`string_heap_allocation_count () : uint64 <function-builtin_string_heap_allocation_count>` 
  *  :ref:`heap_bytes_allocated () : uint64 <function-builtin_heap_bytes_allocated>` 
  *  :ref:`heap_depth () : int <function-builtin_heap_depth>` 
  *  :ref:`string_heap_bytes_allocated () : uint64 <function-builtin_string_heap_bytes_allocated>` 
  *  :ref:`string_heap_depth () : int <function-builtin_string_heap_depth>` 
  *  :ref:`heap_collect (string_heap: bool = true; validate: bool = false) <function-builtin_heap_collect_bool_bool>` 
  *  :ref:`string_heap_report () <function-builtin_string_heap_report>` 
  *  :ref:`heap_report () <function-builtin_heap_report>` 
  *  :ref:`memory_report (errorsOnly: bool) <function-builtin_memory_report_bool>` 

.. _function-builtin_heap_allocation_stats:

.. das:function:: heap_allocation_stats() : urange64

Returns heap allocation statistics as a `urange64`, where the `x` component is total bytes allocated and the `y` component is total bytes freed.

.. _function-builtin_heap_allocation_count:

.. das:function:: heap_allocation_count() : uint64

Returns the total number of heap allocations performed by the current context since it was created.

.. _function-builtin_string_heap_allocation_stats:

.. das:function:: string_heap_allocation_stats() : urange64

Returns string heap allocation statistics as a `urange64` where `x` is total bytes allocated and `y` is total bytes deleted.

.. _function-builtin_string_heap_allocation_count:

.. das:function:: string_heap_allocation_count() : uint64

Returns the total number of individual string allocations performed on the current context's string heap.

.. _function-builtin_heap_bytes_allocated:

.. das:function:: heap_bytes_allocated() : uint64

Returns the number of bytes currently in use on the heap (allocated minus freed), not counting reserved but unused memory.

.. _function-builtin_heap_depth:

.. das:function:: heap_depth() : int

Returns the number of generations (depth of the allocation chain) in the context's regular heap.

.. _function-builtin_string_heap_bytes_allocated:

.. das:function:: string_heap_bytes_allocated() : uint64

Returns the total number of bytes currently allocated in the current context's string heap.

.. _function-builtin_string_heap_depth:

.. das:function:: string_heap_depth() : int

Returns the number of generational layers (depth) in the current context's string heap.

.. _function-builtin_heap_collect_bool_bool:

.. das:function:: heap_collect(string_heap: bool = true; validate: bool = false)

.. warning:: 
  This is unsafe operation.

Triggers garbage collection on the context heap; when `string_heap` is `true` the string heap is also collected, and when `validate` is `true` additional validation checks are performed.

:Arguments: * **string_heap** : bool

            * **validate** : bool

.. _function-builtin_string_heap_report:

.. das:function:: string_heap_report()

Prints a detailed report of string heap usage, including allocation counts and byte statistics, to the log output.

.. _function-builtin_heap_report:

.. das:function:: heap_report()

Prints a diagnostic report of current heap usage and allocation statistics to the output log.

.. _function-builtin_memory_report_bool:

.. das:function:: memory_report(errorsOnly: bool)

Prints a report of memory allocations for the current context; when `errorsOnly` is true, only GC-related errors are included.

:Arguments: * **errorsOnly** : bool

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_save_ptr (name: string implicit; data: void? implicit) <function-builtin_gc0_save_ptr_string_implicit_void_q__implicit>` 
  *  :ref:`gc0_save_smart_ptr (name: string implicit; data: smart_ptr\<void\> implicit) <function-builtin_gc0_save_smart_ptr_string_implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`gc0_restore_ptr (name: string implicit) : void? <function-builtin_gc0_restore_ptr_string_implicit>` 
  *  :ref:`gc0_restore_smart_ptr (name: string implicit) : smart_ptr\<void\> <function-builtin_gc0_restore_smart_ptr_string_implicit>` 
  *  :ref:`gc0_reset () <function-builtin_gc0_reset>` 

.. _function-builtin_gc0_save_ptr_string_implicit_void_q__implicit:

.. das:function:: gc0_save_ptr(name: string implicit; data: void? implicit)

Stores a raw pointer `data` into gc0 storage under the specified `name`, allowing it to be retrieved later with `gc0_restore_ptr`.

:Arguments: * **name** : string implicit

            * **data** : void? implicit

.. _function-builtin_gc0_save_smart_ptr_string_implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: gc0_save_smart_ptr(name: string implicit; data: smart_ptr<void> implicit)

Stores a `smart_ptr<void>` `data` into gc0 storage under the specified `name`, allowing it to be retrieved later with `gc0_restore_smart_ptr`.

:Arguments: * **name** : string implicit

            * **data** : smart_ptr<void> implicit

.. _function-builtin_gc0_restore_ptr_string_implicit:

.. das:function:: gc0_restore_ptr(name: string implicit) : void?

Retrieves a raw pointer previously saved in gc0 storage under the specified `name`, returning `null` if not found.

:Arguments: * **name** : string implicit

.. _function-builtin_gc0_restore_smart_ptr_string_implicit:

.. das:function:: gc0_restore_smart_ptr(name: string implicit) : smart_ptr<void>

Retrieves a `smart_ptr<void>` previously saved in gc0 storage under the specified `name`.

:Arguments: * **name** : string implicit

.. _function-builtin_gc0_reset:

.. das:function:: gc0_reset()

Clears the entire gc0 storage, invalidating all previously saved pointers and smart pointers stored within it.

++++++++++++++++++++++++
Smart ptr infrastructure
++++++++++++++++++++++++

  *  :ref:`move_new (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-builtin_move_new_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-builtin_move_smart_ptr_ls_void_gr__implicit_void_q__implicit>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\>& implicit) <function-builtin_move_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_void_q__implicit>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_use_count (ptr: smart_ptr\<void\> implicit) : uint <function-builtin_smart_ptr_use_count_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`smart_ptr_is_valid (dest: smart_ptr\<void\> implicit) : bool <function-builtin_smart_ptr_is_valid_smart_ptr_ls_void_gr__implicit>` 
  *  :ref:`get_ptr (src: smart_ptr\<auto(TT)\> ==const) : TT? <function-builtin_get_ptr_smart_ptr_ls_autoTT_gr___eq__eq_const>` 
  *  :ref:`get_ptr (var src: smart_ptr\<auto(TT)\> ==const) : TT? <function-builtin_get_ptr__smart_ptr_ls_autoTT_gr___eq__eq_const>` 
  *  :ref:`get_const_ptr (src: smart_ptr\<auto(TT)\>) : TT? <function-builtin_get_const_ptr_smart_ptr_ls_autoTT_gr_>` 
  *  :ref:`add_ptr_ref (src: smart_ptr\<auto(TT)\>) : smart_ptr\<TT\> <function-builtin_add_ptr_ref_smart_ptr_ls_autoTT_gr_>` 

.. _function-builtin_move_new_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: move_new(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

Moves a newly constructed smart pointer value `src` into `dest`, used to initialize a `smart_ptr` from a `new` expression.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-builtin_move_smart_ptr_ls_void_gr__implicit_void_q__implicit:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: void? implicit)

Moves a raw pointer `src` into the smart pointer `dest`, nullifying the previous contents of `dest` and transferring ownership of `src`.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-builtin_move_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: smart_ptr<void>& implicit)

Moves smart pointer `src` into smart pointer `dest`, nullifying `src` and transferring ownership so that `dest` holds the managed object.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void>& implicit

.. _function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_void_q__implicit:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: void? implicit)

Clones a raw pointer `src` into smart pointer `dest`, incrementing the internal reference count to share ownership.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-builtin_smart_ptr_clone_smart_ptr_ls_void_gr__implicit_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

Clones smart pointer `src` into smart pointer `dest`, incrementing the internal reference count so both share ownership of the managed object.

:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-builtin_smart_ptr_use_count_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_use_count(ptr: smart_ptr<void> implicit) : uint

Returns the current reference count of the object managed by `ptr`, indicating how many smart pointers share ownership.

:Arguments: * **ptr** : smart_ptr<void> implicit

.. _function-builtin_smart_ptr_is_valid_smart_ptr_ls_void_gr__implicit:

.. das:function:: smart_ptr_is_valid(dest: smart_ptr<void> implicit) : bool

Checks whether the smart pointer `dest` holds a non-null reference to valid data, returning true if it does.

:Arguments: * **dest** : smart_ptr<void> implicit

.. _function-builtin_get_ptr_smart_ptr_ls_autoTT_gr___eq__eq_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const) : TT?

Extracts a raw pointer of type `TT?` from the given `smart_ptr<TT>`, without affecting reference counting.

:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-builtin_get_ptr__smart_ptr_ls_autoTT_gr___eq__eq_const:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const) : TT?

Extracts a mutable raw pointer of type `TT?` from the given mutable `smart_ptr<TT>`, without affecting reference counting.

:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-builtin_get_const_ptr_smart_ptr_ls_autoTT_gr_:

.. das:function:: get_const_ptr(src: smart_ptr<auto(TT)>) : TT?

Extracts a constant raw pointer of type `TT?` from the given `smart_ptr<TT>`, without affecting reference counting.

:Arguments: * **src** : smart_ptr<auto(TT)>

.. _function-builtin_add_ptr_ref_smart_ptr_ls_autoTT_gr_:

.. das:function:: add_ptr_ref(src: smart_ptr<auto(TT)>) : smart_ptr<TT>

Increments the reference count of the smart pointer `src` and returns a new smart_ptr that shares ownership of the same object.

:Arguments: * **src** : smart_ptr<auto(TT)>

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling () : bool <function-builtin_is_compiling>` 
  *  :ref:`is_compiling_macros () : bool <function-builtin_is_compiling_macros>` 
  *  :ref:`is_compiling_macros_in_module (name: string implicit) : bool <function-builtin_is_compiling_macros_in_module_string_implicit>` 
  *  :ref:`is_reporting_compilation_errors () : bool <function-builtin_is_reporting_compilation_errors>` 
  *  :ref:`is_in_completion () : bool <function-builtin_is_in_completion>` 
  *  :ref:`is_folding () : bool <function-builtin_is_folding>` 

.. _function-builtin_is_compiling:

.. das:function:: is_compiling() : bool

Returns `true` if the current context is in the process of being compiled, allowing compile-time logic to distinguish from runtime execution.

.. _function-builtin_is_compiling_macros:

.. das:function:: is_compiling_macros() : bool

Returns `true` if the current context is being compiled and the compiler is currently executing the macro pass.

.. _function-builtin_is_compiling_macros_in_module_string_implicit:

.. das:function:: is_compiling_macros_in_module(name: string implicit) : bool

Returns `true` if the current context is being compiled during the macro pass and the compiler is processing the module specified by `name`.

:Arguments: * **name** : string implicit

.. _function-builtin_is_reporting_compilation_errors:

.. das:function:: is_reporting_compilation_errors() : bool

Returns `true` if the context failed to compile and the inference pass is currently reporting compilation errors.

.. _function-builtin_is_in_completion:

.. das:function:: is_in_completion() : bool

Returns `true` if the compiler is running in completion mode, generating lexical information for a text editor's code-completion system.

.. _function-builtin_is_folding:

.. das:function:: is_folding() : bool

Returns `true` if the compiler is currently performing its constant folding optimization pass.

++++++++
Profiler
++++++++

  *  :ref:`reset_profiler () <function-builtin_reset_profiler>` 
  *  :ref:`dump_profile_info () <function-builtin_dump_profile_info>` 
  *  :ref:`collect_profile_info () : string <function-builtin_collect_profile_info>` 
  *  :ref:`profile (count: int; category: string implicit; block: block\<():void\>) : float <function-builtin_profile_int_string_implicit_block_ls__c_void_gr_>` 

.. _function-builtin_reset_profiler:

.. das:function:: reset_profiler()

Resets all counters and accumulated data in the built-in profiler to zero.

.. _function-builtin_dump_profile_info:

.. das:function:: dump_profile_info()

Prints the execution counts and timing data for all lines collected by the built-in line profiler to the standard output.

.. _function-builtin_collect_profile_info:

.. das:function:: collect_profile_info() : string

Collects profiling information gathered by the built-in line profiler and returns it as a formatted string containing execution counts and timing data.

.. _function-builtin_profile_int_string_implicit_block_ls__c_void_gr_:

.. das:function:: profile(count: int; category: string implicit; block: block<():void>) : float

Executes `block` a total of `count` times under the given `category` label, prints the timing, and returns the minimum elapsed time in seconds across all iterations.

:Arguments: * **count** : int

            * **category** : string implicit

            * **block** : block<void> implicit

+++++++++++++++++++++
System infrastructure
+++++++++++++++++++++

  *  :ref:`get_das_root () : string <function-builtin_get_das_root>` 
  *  :ref:`panic (text: string implicit) <function-builtin_panic_string_implicit>` 
  *  :ref:`print (text: string implicit) <function-builtin_print_string_implicit>` 
  *  :ref:`error (text: string implicit) <function-builtin_error_string_implicit>` 
  *  :ref:`sprint (value: any; flags: print_flags) : string <function-builtin_sprint_any_print_flags>` 
  *  :ref:`sprint_json (value: any; humanReadable: bool) : string <function-builtin_sprint_json_any_bool>` 
  *  :ref:`terminate () <function-builtin_terminate>` 
  *  :ref:`breakpoint () <function-builtin_breakpoint>` 
  *  :ref:`stackwalk (args: bool = true; vars: bool = true) <function-builtin_stackwalk_bool_bool>` 
  *  :ref:`is_intern_strings () : bool <function-builtin_is_intern_strings>` 
  *  :ref:`is_in_aot () : bool <function-builtin_is_in_aot>` 
  *  :ref:`to_log (level: int; text: string implicit) <function-builtin_to_log_int_string_implicit>` 
  *  :ref:`to_compiler_log (text: string implicit) <function-builtin_to_compiler_log_string_implicit>` 
  *  :ref:`eval_main_loop (block: block\<():void\>) <function-builtin_eval_main_loop_block_ls__c_void_gr_>` 
  *  :ref:`aot_enabled () : bool <function-builtin_aot_enabled>` 

.. _function-builtin_get_das_root:

.. das:function:: get_das_root() : string

Returns the file-system path to the daScript root directory, where `daslib` and other standard libraries are located.

.. _function-builtin_panic_string_implicit:

.. das:function:: panic(text: string implicit)

will cause panic. The program will be determinated if there is no recover. Panic is not a error handling mechanism and can not be used as such. It is indeed panic, fatal error. It is not supposed that program can completely correctly recover from panic, recover construction is provided so program can try to correcly shut-down or report fatal error. If there is no recover withing script, it will be called in calling eval (in C++ callee code).

:Arguments: * **text** : string implicit

.. _function-builtin_print_string_implicit:

.. das:function:: print(text: string implicit)

Outputs `text` to the current context's log, typically printing to standard output.

:Arguments: * **text** : string implicit

.. _function-builtin_error_string_implicit:

.. das:function:: error(text: string implicit)

Outputs the string `text` to the context's error stream, similar to `print` but directed to the error output channel.

:Arguments: * **text** : string implicit

.. _function-builtin_sprint_any_print_flags:

.. das:function:: sprint(value: any; flags: print_flags) : string

Converts `value` to its string representation using the specified `flags` to control formatting, and returns the result as a string.

:Arguments: * **value** : any

            * **flags** :  :ref:`print_flags <alias-print_flags>` 

.. _function-builtin_sprint_json_any_bool:

.. das:function:: sprint_json(value: any; humanReadable: bool) : string

Serializes `value` directly to a JSON string, bypassing intermediate representation for speed; set `humanReadable` to true for indented output.

:Arguments: * **value** : any

            * **humanReadable** : bool

.. _function-builtin_terminate:

.. das:function:: terminate()

Immediately terminates execution of the current daScript context.

.. _function-builtin_breakpoint:

.. das:function:: breakpoint()

Triggers a debugger breakpoint by calling `os_debugbreakpoint`, which is a link-time dependency expected to be provided by the host application or debugger tool.

.. _function-builtin_stackwalk_bool_bool:

.. das:function:: stackwalk(args: bool = true; vars: bool = true)

Prints the current call stack to the log; set `args` to include function arguments and `vars` to include local variable values in the output.

:Arguments: * **args** : bool

            * **vars** : bool

.. _function-builtin_is_intern_strings:

.. das:function:: is_intern_strings() : bool

Returns `true` if string interning is enabled in the current context, meaning identical strings share the same memory.

.. _function-builtin_is_in_aot:

.. das:function:: is_in_aot() : bool

Returns `true` if the compiler is currently generating ahead-of-time (AOT) compiled code.

.. _function-builtin_to_log_int_string_implicit:

.. das:function:: to_log(level: int; text: string implicit)

Outputs `text` to the logging infrastructure at the specified `level` (e.g. LOG_INFO, LOG_ERROR), rather than to standard output.

:Arguments: * **level** : int

            * **text** : string implicit

.. _function-builtin_to_compiler_log_string_implicit:

.. das:function:: to_compiler_log(text: string implicit)

Outputs `text` to the compiler's log stream, typically used from within macro code during compilation.

:Arguments: * **text** : string implicit

.. _function-builtin_eval_main_loop_block_ls__c_void_gr_:

.. das:function:: eval_main_loop(block: block<():void>)

Executes the application main loop by repeatedly invoking `block` until it returns `false`; on Emscripten targets, uses the platform-specific main loop mechanism instead.

:Arguments: * **block** : block<void> implicit

.. _function-builtin_aot_enabled:

.. das:function:: aot_enabled() : bool

Checks whether ahead-of-time (AOT) compilation is enabled for the current program and returns true if it is.

+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`variant_index (arg0: variant\<\> implicit) : int <function-builtin_variant_index_variant_ls__gr__implicit>` 
  *  :ref:`set_variant_index (variant: variant\<\> implicit; index: int) <function-builtin_set_variant_index_variant_ls__gr__implicit_int>` 
  *  :ref:`hash (data: any) : uint64 <function-builtin_hash_any>` 
  *  :ref:`hash (data: string implicit) : uint64 <function-builtin_hash_string_implicit>` 
  *  :ref:`hash (value: int8) : uint64 <function-builtin_hash_int8>` 
  *  :ref:`hash (value: uint8) : uint64 <function-builtin_hash_uint8>` 
  *  :ref:`hash (value: int16) : uint64 <function-builtin_hash_int16>` 
  *  :ref:`hash (value: uint16) : uint64 <function-builtin_hash_uint16>` 
  *  :ref:`hash (value: int) : uint64 <function-builtin_hash_int>` 
  *  :ref:`hash (value: uint) : uint64 <function-builtin_hash_uint>` 
  *  :ref:`hash (value: int64) : uint64 <function-builtin_hash_int64>` 
  *  :ref:`hash (value: uint64) : uint64 <function-builtin_hash_uint64>` 
  *  :ref:`hash (value: void? implicit) : uint64 <function-builtin_hash_void_q__implicit>` 
  *  :ref:`hash (value: float) : uint64 <function-builtin_hash_float>` 
  *  :ref:`hash (value: double) : uint64 <function-builtin_hash_double>` 
  *  :ref:`hash (value: das_string implicit) : uint64 <function-builtin_hash_das_string_implicit>` 
  *  :ref:`memcpy (left: void? implicit; right: void? implicit; size: int) <function-builtin_memcpy_void_q__implicit_void_q__implicit_int>` 
  *  :ref:`memcmp (left: void? implicit; right: void? implicit; size: int) : int <function-builtin_memcmp_void_q__implicit_void_q__implicit_int>` 
  *  :ref:`intptr (p: void?) : uint64 <function-builtin_intptr_void_q_>` 
  *  :ref:`intptr (p: smart_ptr\<auto\>) : uint64 <function-builtin_intptr_smart_ptr_ls_auto_gr_>` 
  *  :ref:`lock_data (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var p:TT?#;s:int):auto\>) : auto <function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_p_c_TT_q__hh_;s_c_int_c_auto_gr_>` 
  *  :ref:`lock_data (a: array\<auto(TT)\> ==const|array\<auto(TT)\> const# ==const; blk: block\<(p:TT const?#;s:int):auto\>) : auto <function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_p_c_TT_const_q__hh_;s_c_int_c_auto_gr_>` 
  *  :ref:`map_to_array (data: void?; len: int; blk: block\<(var arg:array\<auto(TT)\>#):auto\>) : auto <function-builtin_map_to_array_void_q__int_block_ls_var_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_>` 
  *  :ref:`map_to_ro_array (data: void?; len: int; blk: block\<(arg:array\<auto(TT)\>#):auto\>) : auto <function-builtin_map_to_ro_array_void_q__int_block_ls_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_>` 

.. _function-builtin_variant_index_variant_ls__gr__implicit:

.. das:function:: variant_index(arg0: variant<> implicit) : int

Returns the zero-based index indicating which alternative the variant currently holds.

:Arguments: * **arg0** : variant<> implicit

.. _function-builtin_set_variant_index_variant_ls__gr__implicit_int:

.. das:function:: set_variant_index(variant: variant<> implicit; index: int)

.. warning:: 
  This is unsafe operation.

Overwrites the internal type discriminator of `variant` to `index`, changing which alternative the variant is considered to hold.

:Arguments: * **variant** : variant<> implicit

            * **index** : int

.. _function-builtin_hash_any:

.. das:function:: hash(data: any) : uint64

Computes a 64-bit FNV-1a hash of the given value of any type and returns it as `uint64`.

:Arguments: * **data** : any

.. _function-builtin_hash_string_implicit:

.. das:function:: hash(data: string implicit) : uint64

Computes a 64-bit FNV-1a hash of the given string and returns it as `uint64`.

:Arguments: * **data** : string implicit

.. _function-builtin_hash_int8:

.. das:function:: hash(value: int8) : uint64

Computes a 64-bit FNV-1a hash of the given `int8` value and returns it as `uint64`.

:Arguments: * **value** : int8

.. _function-builtin_hash_uint8:

.. das:function:: hash(value: uint8) : uint64

Computes a 64-bit FNV-1a hash of the given `uint8` value and returns it as `uint64`.

:Arguments: * **value** : uint8

.. _function-builtin_hash_int16:

.. das:function:: hash(value: int16) : uint64

Computes a 64-bit FNV-1a hash of the given `int16` value and returns it as `uint64`.

:Arguments: * **value** : int16

.. _function-builtin_hash_uint16:

.. das:function:: hash(value: uint16) : uint64

Computes a 64-bit FNV-1a hash of the given `uint16` value and returns it as `uint64`.

:Arguments: * **value** : uint16

.. _function-builtin_hash_int:

.. das:function:: hash(value: int) : uint64

Computes a 64-bit FNV-1a hash of the given `int` value and returns it as `uint64`.

:Arguments: * **value** : int

.. _function-builtin_hash_uint:

.. das:function:: hash(value: uint) : uint64

Computes a 64-bit FNV-1a hash of the given `uint` value and returns it as `uint64`.

:Arguments: * **value** : uint

.. _function-builtin_hash_int64:

.. das:function:: hash(value: int64) : uint64

Computes a 64-bit FNV-1a hash of the given `int64` value and returns it as `uint64`.

:Arguments: * **value** : int64

.. _function-builtin_hash_uint64:

.. das:function:: hash(value: uint64) : uint64

Computes a 64-bit FNV-1a hash of the given `uint64` value and returns it as `uint64`.

:Arguments: * **value** : uint64

.. _function-builtin_hash_void_q__implicit:

.. das:function:: hash(value: void? implicit) : uint64

Computes a 64-bit FNV-1a hash of the given raw pointer `value` and returns it as `uint64`.

:Arguments: * **value** : void? implicit

.. _function-builtin_hash_float:

.. das:function:: hash(value: float) : uint64

Computes a 64-bit FNV-1a hash of the given `float` value and returns it as `uint64`.

:Arguments: * **value** : float

.. _function-builtin_hash_double:

.. das:function:: hash(value: double) : uint64

Computes a 64-bit FNV-1a hash of the given `double` value and returns it as `uint64`.

:Arguments: * **value** : double

.. _function-builtin_hash_das_string_implicit:

.. das:function:: hash(value: das_string implicit) : uint64

Computes a 64-bit FNV-1a hash of the given `das_string` value and returns it as `uint64`.

:Arguments: * **value** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-builtin_memcpy_void_q__implicit_void_q__implicit_int:

.. das:function:: memcpy(left: void? implicit; right: void? implicit; size: int)

.. warning:: 
  This is unsafe operation.

Copies `size` bytes of memory from the address pointed to by `right` into the address pointed to by `left`.

:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-builtin_memcmp_void_q__implicit_void_q__implicit_int:

.. das:function:: memcmp(left: void? implicit; right: void? implicit; size: int) : int

.. warning:: 
  This is unsafe operation.

Compares `size` bytes of memory at `left` and `right`, returning -1 if `left` is less, 1 if `left` is greater, or 0 if both regions are identical.

:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-builtin_intptr_void_q_:

.. das:function:: intptr(p: void?) : uint64

Converts a raw pointer `p` to its `uint64` integer representation, useful for pointer arithmetic or serialization.

:Arguments: * **p** : void?

.. _function-builtin_intptr_smart_ptr_ls_auto_gr_:

.. das:function:: intptr(p: smart_ptr<auto>) : uint64

Converts a `smart_ptr` `p` to its `uint64` integer representation, useful for pointer arithmetic or serialization.

:Arguments: * **p** : smart_ptr<auto>

.. _function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__hh___eq__eq_const_block_ls_var_p_c_TT_q__hh_;s_c_int_c_auto_gr_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var p:TT?#;s:int):auto>) : auto

Locks a mutable array and invokes `blk` with a raw pointer `p` to the array's contiguous data and its size `s`, allowing direct memory-level access to elements.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-builtin_lock_data_array_ls_autoTT_gr___eq__eq_constarray_ls_autoTT_gr__const_hh___eq__eq_const_block_ls_p_c_TT_const_q__hh_;s_c_int_c_auto_gr_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)> const# ==const; blk: block<(p:TT const?#;s:int):auto>) : auto

Locks a constant array and invokes `blk` with a read-only pointer `p` to the array's contiguous data and its size `s`, allowing direct memory-level read access.

:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-builtin_map_to_array_void_q__int_block_ls_var_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_:

.. das:function:: map_to_array(data: void?; len: int; blk: block<(var arg:array<auto(TT)>#):auto>) : auto

.. warning:: 
  This is unsafe operation.

Constructs a temporary mutable array of type `TT` over raw memory at `data` with `len` elements, and passes it to `blk` without copying the underlying data.

:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

.. _function-builtin_map_to_ro_array_void_q__int_block_ls_arg_c_array_ls_autoTT_gr__hh__c_auto_gr_:

.. das:function:: map_to_ro_array(data: void?; len: int; blk: block<(arg:array<auto(TT)>#):auto>) : auto

.. warning:: 
  This is unsafe operation.

Constructs a temporary read-only array of type `TT` over raw memory at `data` with `len` elements, and passes it to `blk` without copying the underlying data.

:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

+++++++++++++++++
Binary serializer
+++++++++++++++++

  *  :ref:`binary_save (obj: auto; subexpr: block\<(data:array\<uint8\>#):void\>) : auto <function-builtin_binary_save_auto_block_ls_data_c_array_ls_uint8_gr__hh__c_void_gr_>` 
  *  :ref:`binary_load (var obj: auto; data: array\<uint8\>) : auto <function-builtin_binary_load_auto_array_ls_uint8_gr_>` 

.. _function-builtin_binary_save_auto_block_ls_data_c_array_ls_uint8_gr__hh__c_void_gr_:

.. das:function:: binary_save(obj: auto; subexpr: block<(data:array<uint8>#):void>) : auto

Serializes `obj` into a binary representation and passes the resulting uint8 byte array to the block `subexpr`. Obsolete — use `daslib/archive` instead.

:Arguments: * **obj** : auto

            * **subexpr** : block<(data:array<uint8>#):void>

.. _function-builtin_binary_load_auto_array_ls_uint8_gr_:

.. das:function:: binary_load(obj: auto; data: array<uint8>) : auto

Deserializes `obj` from the binary representation stored in `data` (an array of uint8 bytes). Obsolete — use `daslib/archive` instead.

:Arguments: * **obj** : auto

            * **data** : array<uint8> implicit

+++++++++++++++++++++
Path and command line
+++++++++++++++++++++

  *  :ref:`get_command_line_arguments () : array\<string\> <function-builtin_get_command_line_arguments>` 

.. _function-builtin_get_command_line_arguments:

.. das:function:: get_command_line_arguments() : array<string>

Returns an array of strings containing the command-line arguments passed to the program.

+++++++++++++
Time and date
+++++++++++++

  *  :ref:`get_clock () : clock <function-builtin_get_clock>` 
  *  :ref:`mktime (year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock <function-builtin_mktime_int_int_int_int_int_int>` 
  *  :ref:`ref_time_ticks () : int64 <function-builtin_ref_time_ticks>` 
  *  :ref:`get_time_usec (ref: int64) : int <function-builtin_get_time_usec_int64>` 
  *  :ref:`get_time_nsec (ref: int64) : int64 <function-builtin_get_time_nsec_int64>` 

.. _function-builtin_get_clock:

.. das:function:: get_clock() : clock

Returns the current calendar time as a `clock` value representing the number of seconds since 00:00 UTC, January 1, 1970 (the Unix epoch).

.. _function-builtin_mktime_int_int_int_int_int_int:

.. das:function:: mktime(year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock

Converts the calendar date and time specified by `year`, `month`, `mday`, `hour`, `min`, and `sec` into a `clock` value representing time since epoch.

:Arguments: * **year** : int

            * **month** : int

            * **mday** : int

            * **hour** : int

            * **min** : int

            * **sec** : int

.. _function-builtin_ref_time_ticks:

.. das:function:: ref_time_ticks() : int64

Captures the current high-resolution time in ticks, suitable for measuring elapsed intervals with `get_time_usec`.

.. _function-builtin_get_time_usec_int64:

.. das:function:: get_time_usec(ref: int64) : int

Computes the elapsed time in microseconds since the reference point `ref`, which is typically obtained from `ref_time_ticks`.

:Arguments: * **ref** : int64

.. _function-builtin_get_time_nsec_int64:

.. das:function:: get_time_nsec(ref: int64) : int64

Computes the elapsed time in nanoseconds since the reference point `ref`, which is typically obtained from `ref_time_ticks`.

:Arguments: * **ref** : int64

++++++++++++++++
Platform queries
++++++++++++++++

  *  :ref:`get_context_share_counter () : uint64 <function-builtin_get_context_share_counter>` 
  *  :ref:`das_is_dll_build () : bool <function-builtin_das_is_dll_build>` 
  *  :ref:`get_platform_name () : string <function-builtin_get_platform_name>` 
  *  :ref:`get_architecture_name () : string <function-builtin_get_architecture_name>` 

.. _function-builtin_get_context_share_counter:

.. das:function:: get_context_share_counter() : uint64

Returns the use-count of the shared context, which is incremented each time a thread accesses it; useful for tracking concurrent context usage.

.. _function-builtin_das_is_dll_build:

.. das:function:: das_is_dll_build() : bool

Checks whether the current build is configured as a DLL (dynamic library) build, which determines if daslib symbols are available for the JIT compiler.

.. _function-builtin_get_platform_name:

.. das:function:: get_platform_name() : string

Returns the name of the operating system the program is running on, such as `"windows"`, `"linux"`, `"darwin"`, `"emscripten"`, or `"unknown"`.

.. _function-builtin_get_architecture_name:

.. das:function:: get_architecture_name() : string

Returns the name of the CPU architecture the program is running on, such as `"x86_64"`, `"x86"`, `"arm64"`, `"arm"`, `"wasm32"`, or `"unknown"`.

+++++++++++++++++
String formatting
+++++++++++++++++

  *  :ref:`fmt (format: string implicit; value: int8) : string <function-builtin_fmt_string_implicit_int8>` 
  *  :ref:`fmt (format: string implicit; value: uint8) : string <function-builtin_fmt_string_implicit_uint8>` 
  *  :ref:`fmt (format: string implicit; value: int16) : string <function-builtin_fmt_string_implicit_int16>` 
  *  :ref:`fmt (format: string implicit; value: uint16) : string <function-builtin_fmt_string_implicit_uint16>` 
  *  :ref:`fmt (format: string implicit; value: int) : string <function-builtin_fmt_string_implicit_int>` 
  *  :ref:`fmt (format: string implicit; value: uint) : string <function-builtin_fmt_string_implicit_uint>` 
  *  :ref:`fmt (format: string implicit; value: int64) : string <function-builtin_fmt_string_implicit_int64>` 
  *  :ref:`fmt (format: string implicit; value: uint64) : string <function-builtin_fmt_string_implicit_uint64>` 
  *  :ref:`fmt (format: string implicit; value: float) : string <function-builtin_fmt_string_implicit_float>` 
  *  :ref:`fmt (format: string implicit; value: double) : string <function-builtin_fmt_string_implicit_double>` 

.. _function-builtin_fmt_string_implicit_int8:

.. das:function:: fmt(format: string implicit; value: int8) : string

Formats an `int8` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : int8

.. _function-builtin_fmt_string_implicit_uint8:

.. das:function:: fmt(format: string implicit; value: uint8) : string

Formats a `uint8` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : uint8

.. _function-builtin_fmt_string_implicit_int16:

.. das:function:: fmt(format: string implicit; value: int16) : string

Formats an `int16` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : int16

.. _function-builtin_fmt_string_implicit_uint16:

.. das:function:: fmt(format: string implicit; value: uint16) : string

Formats a `uint16` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : uint16

.. _function-builtin_fmt_string_implicit_int:

.. das:function:: fmt(format: string implicit; value: int) : string

Formats an `int` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : int

.. _function-builtin_fmt_string_implicit_uint:

.. das:function:: fmt(format: string implicit; value: uint) : string

Formats a `uint` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : uint

.. _function-builtin_fmt_string_implicit_int64:

.. das:function:: fmt(format: string implicit; value: int64) : string

Formats an `int64` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : int64

.. _function-builtin_fmt_string_implicit_uint64:

.. das:function:: fmt(format: string implicit; value: uint64) : string

Formats a `uint64` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : uint64

.. _function-builtin_fmt_string_implicit_float:

.. das:function:: fmt(format: string implicit; value: float) : string

Formats a `float` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : float

.. _function-builtin_fmt_string_implicit_double:

.. das:function:: fmt(format: string implicit; value: double) : string

Formats a `double` value as a string using the given `format` specifier (following libfmt / C++20 `std::format` syntax).

:Arguments: * **format** : string implicit

            * **value** : double

++++++++++++++++++++
Argument consumption
++++++++++++++++++++

  *  :ref:`consume_argument (var a: auto(TT)&) : TT& <function-builtin_consume_argument_autoTT>` 

.. _function-builtin_consume_argument_autoTT:

.. das:function:: consume_argument(a: auto(TT)&) : TT&

Marks argument `a` as consumed, signaling to the compiler that it will not be used after this call, which enables move optimizations and avoids unnecessary clones. Equivalent to the `<-arg` syntax.

:Arguments: * **a** : auto(TT)&

+++++++++++++
Lock checking
+++++++++++++

  *  :ref:`lock_count (array: array\<anything\>) : int <function-builtin_lock_count_array_ls_anything_gr_>` 
  *  :ref:`set_verify_array_locks (array: array\<anything\>; check: bool) : bool <function-builtin_set_verify_array_locks_array_ls_anything_gr__bool>` 
  *  :ref:`set_verify_table_locks (table: table\<anything, anything\>; check: bool) : bool <function-builtin_set_verify_table_locks_table_ls_anything,_anything_gr__bool>` 

.. _function-builtin_lock_count_array_ls_anything_gr_:

.. das:function:: lock_count(array: array<anything>) : int

Returns the current internal lock count for the given `array`, indicating how many active locks prevent it from being resized.

:Arguments: * **array** : array implicit

.. _function-builtin_set_verify_array_locks_array_ls_anything_gr__bool:

.. das:function:: set_verify_array_locks(array: array<anything>; check: bool) : bool

.. warning:: 
  This is unsafe operation.

Enables or disables runtime lock verification for the given `array`; when `check` is false, lock safety checks are skipped as a performance optimization.

:Arguments: * **array** : array implicit

            * **check** : bool

.. _function-builtin_set_verify_table_locks_table_ls_anything,_anything_gr__bool:

.. das:function:: set_verify_table_locks(table: table<anything, anything>; check: bool) : bool

.. warning:: 
  This is unsafe operation.

Enables or disables runtime lock verification for the given `table`; when `check` is false, lock safety checks are skipped as a performance optimization.

:Arguments: * **table** : table implicit

            * **check** : bool

+++++++++++++++++++++++
Lock checking internals
+++++++++++++++++++++++

  *  :ref:`_move_with_lockcheck (var a: auto(valA)&; var b: auto(valB)&) : auto <function-builtin__move_with_lockcheck_autovalA_autovalB>` 
  *  :ref:`_return_with_lockcheck (var a: auto(valT)& ==const) : auto& <function-builtin__return_with_lockcheck__autovalT__eq__eq_const>` 
  *  :ref:`_return_with_lockcheck (a: auto(valT) const& ==const) : auto& <function-builtin__return_with_lockcheck_autovalT_const__eq__eq_const>` 
  *  :ref:`_at_with_lockcheck (var Tab: table\<auto(keyT), auto(valT)\>; at: keyT|keyT#) : valT& <function-builtin__at_with_lockcheck_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_>` 

.. _function-builtin__move_with_lockcheck_autovalA_autovalB:

.. das:function:: _move_with_lockcheck(a: auto(valA)&; b: auto(valB)&) : auto

Moves the contents of `b` into `a`, verifying that neither `a` nor `b` (nor any of their lockable sub-elements) is currently locked.

:Arguments: * **a** : auto(valA)&

            * **b** : auto(valB)&

.. _function-builtin__return_with_lockcheck__autovalT__eq__eq_const:

.. das:function:: _return_with_lockcheck(a: auto(valT)& ==const) : auto&

Passes through and returns a mutable reference to `a`, verifying that `a` and all of its lockable sub-elements are not currently locked.

:Arguments: * **a** : auto(valT)&!

.. _function-builtin__return_with_lockcheck_autovalT_const__eq__eq_const:

.. das:function:: _return_with_lockcheck(a: auto(valT) const& ==const) : auto&

Passes through and returns a const reference to `a`, verifying that `a` and all of its lockable sub-elements are not currently locked.

:Arguments: * **a** : auto(valT)&!

.. _function-builtin__at_with_lockcheck_table_ls_autokeyT,_autovalT_gr__keyTkeyT_hh_:

.. das:function:: _at_with_lockcheck(Tab: table<auto(keyT), auto(valT)>; at: keyT|keyT#) : valT&

Looks up and returns a reference to the element at key `at` in the table `Tab`, while verifying that `Tab` and its lockable sub-elements are not locked.

:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

++++++++++++++
Bit operations
++++++++++++++

  *  :ref:`clz (bits: uint) : uint <function-builtin_clz_uint>` 
  *  :ref:`clz (bits: uint64) : uint64 <function-builtin_clz_uint64>` 
  *  :ref:`ctz (bits: uint) : uint <function-builtin_ctz_uint>` 
  *  :ref:`ctz (bits: uint64) : uint64 <function-builtin_ctz_uint64>` 
  *  :ref:`popcnt (bits: uint) : uint <function-builtin_popcnt_uint>` 
  *  :ref:`popcnt (bits: uint64) : uint64 <function-builtin_popcnt_uint64>` 
  *  :ref:`mul128 (a: uint64; b: uint64) : urange64 <function-builtin_mul128_uint64_uint64>` 
  *  :ref:`__bit_set (value: bitfield& implicit; mask: bitfield; on: bool) <function-builtin___bit_set_bitfield_implicit_bitfield_bool>` 
  *  :ref:`__bit_set (value: bitfield8:uint8\<\>& implicit; mask: bitfield8:uint8\<\>; on: bool) <function-builtin___bit_set_bitfield8_c_uint8_ls__gr__implicit_bitfield8_c_uint8_ls__gr__bool>` 
  *  :ref:`__bit_set (value: bitfield16:uint16\<\>& implicit; mask: bitfield16:uint16\<\>; on: bool) <function-builtin___bit_set_bitfield16_c_uint16_ls__gr__implicit_bitfield16_c_uint16_ls__gr__bool>` 
  *  :ref:`__bit_set (value: bitfield64:uint64\<\>& implicit; mask: bitfield64:uint64\<\>; on: bool) <function-builtin___bit_set_bitfield64_c_uint64_ls__gr__implicit_bitfield64_c_uint64_ls__gr__bool>` 

.. _function-builtin_clz_uint:

.. das:function:: clz(bits: uint) : uint

Counts the number of leading zero bits in the 32-bit unsigned integer `bits`, returning 32 if the value is zero.

:Arguments: * **bits** : uint

.. _function-builtin_clz_uint64:

.. das:function:: clz(bits: uint64) : uint64

Counts the number of leading zero bits in the 64-bit unsigned integer `bits`, returning 64 if the value is zero.

:Arguments: * **bits** : uint64

.. _function-builtin_ctz_uint:

.. das:function:: ctz(bits: uint) : uint

Counts the number of trailing zero bits in the 32-bit unsigned integer `bits`, returning 32 if the value is zero.

:Arguments: * **bits** : uint

.. _function-builtin_ctz_uint64:

.. das:function:: ctz(bits: uint64) : uint64

Counts the number of trailing zero bits in the 64-bit unsigned integer `bits`, returning 64 if the value is zero.

:Arguments: * **bits** : uint64

.. _function-builtin_popcnt_uint:

.. das:function:: popcnt(bits: uint) : uint

Counts and returns the number of set (1) bits in the 32-bit unsigned integer `bits`.

:Arguments: * **bits** : uint

.. _function-builtin_popcnt_uint64:

.. das:function:: popcnt(bits: uint64) : uint64

Counts and returns the number of set (1) bits in the 64-bit unsigned integer `bits`.

:Arguments: * **bits** : uint64

.. _function-builtin_mul128_uint64_uint64:

.. das:function:: mul128(a: uint64; b: uint64) : urange64

Multiplies two 64-bit unsigned integers `a` and `b`, returning the full 128-bit result as a `urange64` containing the low and high 64-bit halves.

:Arguments: * **a** : uint64

            * **b** : uint64

.. _function-builtin___bit_set_bitfield_implicit_bitfield_bool:

.. das:function:: __bit_set(value: bitfield& implicit; mask: bitfield; on: bool)

Sets or clears the bits specified by `mask` in the bitfield `value`, turning them on if `on` is true or off if `on` is false.

:Arguments: * **value** : bitfield<>& implicit

            * **mask** : bitfield<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield8_c_uint8_ls__gr__implicit_bitfield8_c_uint8_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield8:uint8<>& implicit; mask: bitfield8:uint8<>; on: bool)

Sets or clears the bits specified by `mask` in the 8-bit bitfield `value`, turning them on if `on` is true or off if `on` is false.

:Arguments: * **value** : bitfield : uint8<>& implicit

            * **mask** : bitfield : uint8<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield16_c_uint16_ls__gr__implicit_bitfield16_c_uint16_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield16:uint16<>& implicit; mask: bitfield16:uint16<>; on: bool)

Sets or clears the bits specified by `mask` in the 16-bit bitfield `value`, turning them on if `on` is true or off if `on` is false.

:Arguments: * **value** : bitfield : uint16<>& implicit

            * **mask** : bitfield : uint16<>

            * **on** : bool

.. _function-builtin___bit_set_bitfield64_c_uint64_ls__gr__implicit_bitfield64_c_uint64_ls__gr__bool:

.. das:function:: __bit_set(value: bitfield64:uint64<>& implicit; mask: bitfield64:uint64<>; on: bool)

Sets or clears the bits specified by `mask` in the 64-bit bitfield `value`, turning them on if `on` is true or off if `on` is false.

:Arguments: * **value** : bitfield : uint64<>& implicit

            * **mask** : bitfield : uint64<>

            * **on** : bool

+++++++++
Intervals
+++++++++

  *  :ref:`interval (arg0: int; arg1: int) : range <function-builtin_interval_int_int>` 
  *  :ref:`interval (arg0: uint; arg1: uint) : urange <function-builtin_interval_uint_uint>` 
  *  :ref:`interval (arg0: int64; arg1: int64) : range64 <function-builtin_interval_int64_int64>` 
  *  :ref:`interval (arg0: uint64; arg1: uint64) : urange64 <function-builtin_interval_uint64_uint64>` 

.. _function-builtin_interval_int_int:

.. das:function:: interval(arg0: int; arg1: int) : range

Constructs a `range` value from the two `int` endpoints `arg0` (inclusive) and `arg1` (exclusive).

:Arguments: * **arg0** : int

            * **arg1** : int

.. _function-builtin_interval_uint_uint:

.. das:function:: interval(arg0: uint; arg1: uint) : urange

Constructs a `urange` value from the two `uint` endpoints `arg0` (inclusive) and `arg1` (exclusive).

:Arguments: * **arg0** : uint

            * **arg1** : uint

.. _function-builtin_interval_int64_int64:

.. das:function:: interval(arg0: int64; arg1: int64) : range64

Constructs a `range64` value from the two `int64` endpoints `arg0` (inclusive) and `arg1` (exclusive).

:Arguments: * **arg0** : int64

            * **arg1** : int64

.. _function-builtin_interval_uint64_uint64:

.. das:function:: interval(arg0: uint64; arg1: uint64) : urange64

Constructs a `urange64` value from the two `uint64` endpoints `arg0` (inclusive) and `arg1` (exclusive).

:Arguments: * **arg0** : uint64

            * **arg1** : uint64

++++
RTTI
++++

  *  :ref:`class_rtti_size (ptr: void? implicit) : int <function-builtin_class_rtti_size_void_q__implicit>` 

.. _function-builtin_class_rtti_size_void_q__implicit:

.. das:function:: class_rtti_size(ptr: void? implicit) : int

Examines the RTTI (runtime type information) associated with the class at `ptr` and returns the size in bytes of its TypeInfo structure.

:Arguments: * **ptr** : void? implicit

+++++++++++++++++
Lock verification
+++++++++++++++++

  *  :ref:`set_verify_context_locks (check: bool) : bool <function-builtin_set_verify_context_locks_bool>` 

.. _function-builtin_set_verify_context_locks_bool:

.. das:function:: set_verify_context_locks(check: bool) : bool

.. warning:: 
  This is unsafe operation.

Enables or disables runtime lock verification for all arrays and tables in the current context; returns the previous verification state.

:Arguments: * **check** : bool

+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`using (arg0: block\<(das_string):void\>) <function-builtin_using_block_ls_das_string_c_void_gr_>` 

.. _function-builtin_using_block_ls_das_string_c_void_gr_:

.. das:function:: using(arg0: block<(das_string):void>)

Creates a temporary `das_string` and passes it to the block, automatically managing its lifetime for the duration of the call.

:Arguments: * **arg0** : block<( :ref:`das_string <handle-builtin-das_string>` ):void> implicit

++++++++++
Algorithms
++++++++++

  *  :ref:`count (start: int = 0; step: int = 1) : iterator\<int\> <function-builtin_count_int_int>` 
  *  :ref:`ucount (start: uint = 0x0; step: uint = 0x1) : iterator\<uint\> <function-builtin_ucount_uint_uint>` 
  *  :ref:`iter_range (foo: auto) : auto <function-builtin_iter_range_auto>` 
  *  :ref:`swap (var a: auto(TT)&; var b: auto(TT)&) : auto <function-builtin_swap_autoTT_autoTT>` 

.. _function-builtin_count_int_int:

.. das:function:: count(start: int = 0; step: int = 1) : iterator<int>

Creates an infinite iterator that yields integer values starting from `start` and incrementing by `step` on each iteration, intended for use as a counter alongside other sequences in a `for` loop.

:Arguments: * **start** : int

            * **step** : int

.. _function-builtin_ucount_uint_uint:

.. das:function:: ucount(start: uint = 0x0; step: uint = 0x1) : iterator<uint>

Creates an infinite iterator over unsigned integers beginning at `start` and incrementing by `step` on each iteration.

:Arguments: * **start** : uint

            * **step** : uint

.. _function-builtin_iter_range_auto:

.. das:function:: iter_range(foo: auto) : auto

Creates a `range` from `0` to the length of the given iterable `foo`, useful for index-based iteration over containers.

:Arguments: * **foo** : auto

.. _function-builtin_swap_autoTT_autoTT:

.. das:function:: swap(a: auto(TT)&; b: auto(TT)&) : auto

Exchanges the values of `a` and `b` in place, leaving each variable holding the other's former value.

:Arguments: * **a** : auto(TT)&

            * **b** : auto(TT)&

++++++
Memset
++++++

  *  :ref:`memset8 (left: void? implicit; value: uint8; count: int) <function-builtin_memset8_void_q__implicit_uint8_int>` 
  *  :ref:`memset16 (left: void? implicit; value: uint16; count: int) <function-builtin_memset16_void_q__implicit_uint16_int>` 
  *  :ref:`memset32 (left: void? implicit; value: uint; count: int) <function-builtin_memset32_void_q__implicit_uint_int>` 
  *  :ref:`memset64 (left: void? implicit; value: uint64; count: int) <function-builtin_memset64_void_q__implicit_uint64_int>` 
  *  :ref:`memset128 (left: void? implicit; value: uint4; count: int) <function-builtin_memset128_void_q__implicit_uint4_int>` 

.. _function-builtin_memset8_void_q__implicit_uint8_int:

.. das:function:: memset8(left: void? implicit; value: uint8; count: int)

.. warning:: 
  This is unsafe operation.

Fills memory at `left` with `count` copies of the 8-bit `value`, equivalent to the C `memset` function.

:Arguments: * **left** : void? implicit

            * **value** : uint8

            * **count** : int

.. _function-builtin_memset16_void_q__implicit_uint16_int:

.. das:function:: memset16(left: void? implicit; value: uint16; count: int)

.. warning:: 
  This is unsafe operation.

Fills memory at `left` with `count` copies of the 16-bit `value`.

:Arguments: * **left** : void? implicit

            * **value** : uint16

            * **count** : int

.. _function-builtin_memset32_void_q__implicit_uint_int:

.. das:function:: memset32(left: void? implicit; value: uint; count: int)

.. warning:: 
  This is unsafe operation.

Fills memory at `left` with `count` copies of the 32-bit `value`.

:Arguments: * **left** : void? implicit

            * **value** : uint

            * **count** : int

.. _function-builtin_memset64_void_q__implicit_uint64_int:

.. das:function:: memset64(left: void? implicit; value: uint64; count: int)

.. warning:: 
  This is unsafe operation.

Fills memory at `left` with `count` copies of the 64-bit `value`.

:Arguments: * **left** : void? implicit

            * **value** : uint64

            * **count** : int

.. _function-builtin_memset128_void_q__implicit_uint4_int:

.. das:function:: memset128(left: void? implicit; value: uint4; count: int)

.. warning:: 
  This is unsafe operation.

Fills memory at `left` with `count` copies of the 128-bit `uint4` vector `value`.

:Arguments: * **left** : void? implicit

            * **value** : uint4

            * **count** : int

++++++
Malloc
++++++

  *  :ref:`malloc (size: uint64) : void? <function-builtin_malloc_uint64>` 
  *  :ref:`free (ptr: void? implicit) <function-builtin_free_void_q__implicit>` 
  *  :ref:`malloc_usable_size (ptr: void? implicit) : uint64 <function-builtin_malloc_usable_size_void_q__implicit>` 

.. _function-builtin_malloc_uint64:

.. das:function:: malloc(size: uint64) : void?

.. warning:: 
  This is unsafe operation.

Allocates a block of uninitialized memory of the specified `size` in bytes, C-style, and returns a raw pointer to it; must be freed with `free`.

:Arguments: * **size** : uint64

.. _function-builtin_free_void_q__implicit:

.. das:function:: free(ptr: void? implicit)

.. warning:: 
  This is unsafe operation.

Frees memory previously allocated with `malloc`, following C-style manual memory management semantics.

:Arguments: * **ptr** : void? implicit

.. _function-builtin_malloc_usable_size_void_q__implicit:

.. das:function:: malloc_usable_size(ptr: void? implicit) : uint64

.. warning:: 
  This is unsafe operation.

Returns the usable size in bytes of the memory block pointed to by `ptr`, as reported by the underlying allocator.

:Arguments: * **ptr** : void? implicit

+++++++++++++++++++
Compilation and AOT
+++++++++++++++++++

  *  :ref:`set_aot () <function-builtin_set_aot>` 
  *  :ref:`reset_aot () <function-builtin_reset_aot>` 
  *  :ref:`compiling_file_name () : string <function-builtin_compiling_file_name>` 
  *  :ref:`compiling_module_name () : string <function-builtin_compiling_module_name>` 

.. _function-builtin_set_aot:

.. das:function:: set_aot()

Notifies the compiler that ahead-of-time code generation is now in progress.

.. _function-builtin_reset_aot:

.. das:function:: reset_aot()

Notifies the compiler that ahead-of-time code generation has finished, restoring normal compilation mode.

.. _function-builtin_compiling_file_name:

.. das:function:: compiling_file_name() : string

Returns the file name of the source file currently being compiled, useful for compile-time metaprogramming and diagnostics.

.. _function-builtin_compiling_module_name:

.. das:function:: compiling_module_name() : string

Returns the name of the module currently being compiled, useful for compile-time metaprogramming and diagnostics.


