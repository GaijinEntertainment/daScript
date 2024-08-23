
.. _stdlib__builtin:

================
Built-in runtime
================

Builtin module is automatically required by any other das file. It includes basic language infrastructure,
support for containers, heap, miscellaneous iterators, profiler, and interaction with host application.

++++++++++++
Type aliases
++++++++++++

.. _alias-print_flags:

.. das:attribute:: print_flags is a bitfield

This bitfield specifies how exactly values are to be printed.

:Fields: * **escapeString** (0) - the string is printed with escape characters.

         * **namesAndDimensions** (1) - the names of the types and dimensions of the structures are printed.

         * **typeQualifiers** (2) - type qualifiers (like f, lf, u8 etc) are printed.

         * **refAddresses** (3) - the addresses of the references are printed.

         * **singleLine** (4) - the output is printed in a single line.

         * **fixedPoint** (5) - the floating-point numbers are printed in fixed-point format.


+++++++++
Constants
+++++++++

.. _global-builtin-DAS_MAX_FUNCTION_ARGUMENTS:

.. das:attribute:: DAS_MAX_FUNCTION_ARGUMENTS = 32

Maximum number of arguments for the function. This is used to pre-allocate stack space for the function arguments.
.. _global-builtin-INT_MIN:

.. das:attribute:: INT_MIN = -2147483648

Minimum 32-bit signed integer value (negative).


.. _global-builtin-INT_MAX:

.. das:attribute:: INT_MAX = 2147483647

Maximum 32-bit signed integer value (positive).


.. _global-builtin-UINT_MAX:

.. das:attribute:: UINT_MAX = 0xffffffff

Maximum 32-bit unsigned integer value.
.. _global-builtin-LONG_MIN:

.. das:attribute:: LONG_MIN = -9223372036854775808

Minimum 64-bit signed integer value (negative).

.. _global-builtin-LONG_MAX:

.. das:attribute:: LONG_MAX = 9223372036854775807

Maximum 64-bit signed integer value (positive).


.. _global-builtin-ULONG_MAX:

.. das:attribute:: ULONG_MAX = 0xffffffffffffffff

Maximum 64-bit unsigned integer value.


.. _global-builtin-FLT_MIN:

.. das:attribute:: FLT_MIN = 1.1754944e-38f

Minimum normalized positive single-precision floating-point number.


.. _global-builtin-FLT_MAX:

.. das:attribute:: FLT_MAX = 3.4028235e+38f

Maximum normalized single-precision floating-point number.


.. _global-builtin-DBL_MIN:

.. das:attribute:: DBL_MIN = 2.2250738585072014e-308lf

Minimum normalized positive double-precision floating-point number.


.. _global-builtin-DBL_MAX:

.. das:attribute:: DBL_MAX = 1.7976931348623157e+308lf

Maximum normalized double-precision floating-point number.
.. _global-builtin-LOG_CRITICAL:

.. das:attribute:: LOG_CRITICAL = 50000

Indicates maximum log level. Critial errors, panic, shutdown.


.. _global-builtin-LOG_ERROR:

.. das:attribute:: LOG_ERROR = 40000

Indicates log level recoverable errors.


.. _global-builtin-LOG_WARNING:

.. das:attribute:: LOG_WARNING = 30000

Indicates log level for API misuse, non-fatal errors.


.. _global-builtin-LOG_INFO:

.. das:attribute:: LOG_INFO = 20000

Indicates log level for miscellaneous informative messages.


.. _global-builtin-LOG_DEBUG:

.. das:attribute:: LOG_DEBUG = 10000

Indicates log level for debug messages.


.. _global-builtin-LOG_TRACE:

.. das:attribute:: LOG_TRACE = 0

 Indicates log level for the most noisy debug and tracing messages.


.. _global-builtin-VEC_SEP:

.. das:attribute:: VEC_SEP = ","

Read-only string constant which is used to separate elements of vectors. By default its ",".

.. _global-builtin-print_flags_debugger:

.. das:attribute:: print_flags_debugger = bitfield(0xf)

Printing flags similar to those used by the 'debug' function.
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

Marker annotation is used to attach arbitrary marker values to a function (in form of annotation arguments). Its typically used for implementation of macros.


.. _handle-builtin-generic:

.. das:attribute:: generic

Indicates that the function is generic, regardless of its argument types. Generic functions will be instanced in the calling module::

    [generic]
    def random_seed ( seed:int )    // this is now a generic function
        return int4(seed,seed+1,seed+2,seed+3)


.. _handle-builtin-_macro:

.. das:attribute:: _macro

Indicates that the function will be called during the macro pass, similar to `[init]`.
Here is an example from the parse_macro implementation::

    [_macro]
    def private setup   // this is called every time during initialization of any macro module
        if is_compiling_macros_in_module("parse_macro")
            compiling_module() |> add_keyword("parse", false)


.. _handle-builtin-macro_function:

.. das:attribute:: macro_function

Indicates that the function is part of the macro implementation, and will not be present in the final compiled context, unless explicitly called.


.. _handle-builtin-hint:

.. das:attribute:: hint

Hints the compiler to use specific optimization::

    [hint(noalias=objects,vec3_ldu)]    // object never aliases, we use vec3_ldu to load vectors, not 3-x float reads
    def testSimI(var objects:array<NObject>)
        for obj in objects
            obj.position += obj.velocity


.. _handle-builtin-jit:

.. das:attribute:: jit

Explicitly marks (forces) function to be compiled with JIT compiler.


.. _handle-builtin-no_jit:

.. das:attribute:: no_jit

Disables JIT compilation for the function.


.. _handle-builtin-nodiscard:

.. das:attribute:: nodiscard

Marks function as nodiscard. Result of the function should be used.
Here is an example from buildin.das::

    [nodiscard] // can't call each(foo) without using the result, need to assign or pass to other function
    def each ( lam : lambda<(var arg:auto(argT)):bool> ) : iterator<argT -&>
        concept_assert(typeinfo(is_ref type<argT>),"lamda-to-iterator argument has to be reference; expecting value& or reference type")
        ...



.. _handle-builtin-deprecated:

.. das:attribute:: deprecated

Deprecated annotation is used to mark a function as deprecated.
It will generate a warning during compilation, and will not be callable from the final compiled context.
Here is an example from ast_boost::

    [deprecated]
    def find_arg(argn:string; args:AnnotationArgumentList) : RttiValue



.. _handle-builtin-alias_cmres:

.. das:attribute:: alias_cmres

Indicates that function always aliases cmres (copy or move result), and cmres optimizations are disabled.




.. _handle-builtin-never_alias_cmres:

.. das:attribute:: never_alias_cmres

Indicates that function never aliases cmres (copy or move result), and cmres checks will not be performed.


.. _handle-builtin-export:

.. das:attribute:: export

Indicates that function is to be exported to the final compiled context::

    [export]
    def main
        ...



.. _handle-builtin-pinvoke:

.. das:attribute:: pinvoke

Indicates that the function is a pinvoke function, and will be called via pinvoke machinery::

    [pinvoke, export]   // we will invoke this via pinvoke(other_context,"setup_command_processor",ch,cch)
    def setup_command_processor(ch, cch : Channel?)
        ...


.. _handle-builtin-no_lint:

.. das:attribute:: no_lint

Indicates that the lint pass should be skipped for the specific function.


.. _handle-builtin-sideeffects:

.. das:attribute:: sideeffects

Indicates that the function should be treated as if it has side-effects. For example it will not be optimized out::

    [sideeffects]
    def foo
        var a = 1
        a ++

    [export]
    def main
        foo()   // will actually call foo(), even though it does nothing and will be optimized out otherwise


.. _handle-builtin-run:

.. das:attribute:: run

Ensures that the function is always evaluated at compilation time.


.. _handle-builtin-unsafe_operation:

.. das:attribute:: unsafe_operation

Indicates that function is unsafe, and will require `unsafe` keyword to be called::

    [unsafe_operation]
    def public temp_array ( var arr : auto implicit ==const )
        ...

    [export]
    def main
        ...
        temp_array(arr) // will not compile, need to use `unsafe(temp_array(arr))` instead


.. _handle-builtin-unsafe_outside_of_for:

.. das:attribute:: unsafe_outside_of_for

Marks function as unsafe to be called outside of the sources `for` loop::


    [unsafe_outside_of_for,nodiscard]
    def keys(a:table<auto(keyT);auto(valT)>|#) : iterator<keyT & const>
        ...

    [export]
    def main
        ...
        keys(arr) // will not compile, need to use `var keys <- unsafe(keys(arr))` instead

        for k in keys(arr)  // this is fine
            ...


.. _handle-builtin-no_aot:

.. das:attribute:: no_aot

Indicates that the AOT will not be generated for this specific function.


.. _handle-builtin-init:

.. das:attribute:: init

Indicates that the function would be called at the context initialization time::

    [init]
    def init
        // do some initialization
        // this function will be called at the context initialization time

    [export]
    def main
        // main function


.. _handle-builtin-finalize:

.. das:attribute:: finalize

Indicates that the function would be called at the context shutdown time::

    [finalize]
    def finalize
        // do some finalization
        // this function will be called at the context shutdown time



.. _handle-builtin-hybrid:

.. das:attribute:: hybrid

Indicates that the function is likely candidate for later patching, and the AOT will generate hybrid calls to it - instead of direct calls.
That way modifying the function will not affect AOT of other functions.


.. _handle-builtin-unsafe_deref:

.. das:attribute:: unsafe_deref

Optimization, which indicates that pointer dereference, array and string indexing, and few other operations would not check for null or bounds.

.. _handle-builtin-skip_lock_check:

.. das:attribute:: skip_lock_check

Optimization, which indicates that lock checks are not needed in this function.


.. _handle-builtin-unused_argument:

.. das:attribute:: unused_argument

Marks function arguments, which are unused.
That way when code policies make unused arguments an error, a workaround can be provided::

    [unused_argument(ctx)]
    def private start_debug_agent(var ctx: Context)
        pass    // notice, how `ctx` is not used in the function body



.. _handle-builtin-local_only:

.. das:attribute:: local_only

Indicates that function can only accept local `make` expressions, like [[make tuple]] and [[make structure]]::

    [local_only (arg)]
    def take_local(arg)
        print("arg={arg}\n")

    struct Foo
        bar : int

    [export]
    def test
        take_local(1)             // 40102: call annotated by local_only failed, expecting [[...]]
        take_local(Foo(bar=1))   // ok, structure are local
        take_local((1,2.0,"3"))  // ok, tuple are local
        return true

.. _handle-builtin-expect_any_vector:

.. das:attribute:: expect_any_vector

Indicates that function can only accept das::vector templates.
Here is an example from builtin.das::

    [expect_any_vector(arr)]
    def back ( var arr : auto(TT) ==const ) : auto &
        let n = _::length(arr)
        if n == 0
            panic("vector is empty")
        unsafe
            return arr[n-1]


.. _handle-builtin-expect_dim:

.. das:attribute:: expect_dim

A contract to mark function argument to be a static array::

    [expect_dim(arg)]
    def test(arg)
        print("arg={arg}\n")

    [export]
    def main
        test(1)                     // compilation error
        test(fixed_array(1,2,3))    // int[3] is ok


.. _handle-builtin-type_function:

.. das:attribute:: type_function

Specifies that the function is a type function, and can use type-function syntax::

    [type_function]
    def take ( a : auto(TT); a1,a2 : int )
        return typeinfo(sizeof type<TT>) + a1 + a2

    [export]
    def main
        let size = take<int>(1,2)   // note syntax which expands into take(type<int>,1,2)

.. _handle-builtin-builtin_array_sort:

.. das:attribute:: builtin_array_sort

Indicates sort function for builtin 'sort' machinery. Used internally.


+++++++++++
Call macros
+++++++++++

.. _call-macro-builtin-make_function_unsafe:

.. das:attribute:: make_function_unsafe

Makes function unsafe during compilation. This is usually accompanied with the 'static_if'::

    def resize(var Arr:array<auto(numT)>;newSize:int)
	    static_if typeinfo(is_unsafe_when_uninitialized type<numT>)
		    make_function_unsafe()  // make the function unsafe, if the type is unsafe when uninitialized
        ...




.. _call-macro-builtin-concept_assert:

.. das:attribute:: concept_assert

Similar to regular `assert` function, but always happens at compilation time.
It would also display the error message from where the asserted function was called from, not the assert line itself::

    def join(it; separator:string implicit)
        static_if !typeinfo(is_iterable it)
            concept_assert(false,"can't join non-iterable type")
            return ""
        ...


.. _call-macro-builtin-__builtin_table_set_insert:

.. das:attribute:: __builtin_table_set_insert

Part of internal implementation for `insert` of the sets (tables with keys only).


.. _call-macro-builtin-__builtin_table_key_exists:

.. das:attribute:: __builtin_table_key_exists

Part of internal implementation for `key_exists`.


.. _call-macro-builtin-static_assert:

.. das:attribute:: static_assert

Similar to regular `assert` function, but always happens at compilation time::

    def take_small(a)
        static_assert(typeinfo(sizeof a)<=16,"a is too big")



.. _call-macro-builtin-verify:

.. das:attribute:: verify

Assert for the expression with side effects. Expression will not be optimized out if asserts are disabled.


.. _call-macro-builtin-debug:

.. das:attribute:: debug

Prints value and returns that same value::

    debug(a)            // prints detailed information about a
    if debug(foo) == 0  // prints detailed information about foo
        ...


.. _call-macro-builtin-assert:

.. das:attribute:: assert

Throws panic if first operand is false. Can be disabled.
Optional second operand is error message::

    assert(a==5)
    assert(a==5,"a should be 5")


.. _call-macro-builtin-memzero:

.. das:attribute:: memzero

Initializes section of memory with '0'::

    var x = 1.0f
    memzero(x)
    assert(x==0.0f)


.. _call-macro-builtin-__builtin_table_find:

.. das:attribute:: __builtin_table_find

Part of internal implementation for `find`.


.. _call-macro-builtin-invoke:

.. das:attribute:: invoke

Invokes block, function, or lambda. Takes arguments, returns result::

    blk |> invoke()
    fn |> invoke(1, 2, 3)
    res = lambda |> invoke(1, "2")


.. _call-macro-builtin-__builtin_table_erase:

.. das:attribute:: __builtin_table_erase

Part of internal implementation for `erase`.


+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-builtin-_esc:

.. das:attribute:: _esc

Returns raw string input, without regards for escape sequences. For example::

     %_esc\n\r%_esc // will return 4 character string '\\','n','\\','r'


+++++++++++++++
Typeinfo macros
+++++++++++++++

.. _call-macro-builtin-rtti_classinfo:

.. das:attribute:: rtti_classinfo

Generates rtti's TypeInfo given type or value::

    let tinfo = typeinfo(rtti_typeinfo value)
    let tinfo = typeinfo(rtti_typeinto type<Foo>)



+++++++++++++
Handled types
+++++++++++++

.. _handle-builtin-das_string:

.. das:attribute:: das_string

das::string which is typically std::string or equivalent (eastl::string and such).


.. _handle-builtin-clock:

.. das:attribute:: clock

das::Time which is a wrapper around `time_t`.



++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-builtin-comment:

.. das:attribute:: comment

Structure annotation, which does nothing but holds the values of the annotation.
For example::

    [comment (sValue="hello,world",iValue=5,fValue=2.0,bValue=true)]
    struct Dummy



.. _handle-builtin-macro_interface:

.. das:attribute:: macro_interface

Specifies that class and its inherited children are used as a macro interfaces, and would not be exported by default::

    [macro_interface]
    class AstBlockAnnotation    // this class is not exported to regular modules, and only stays in the macro modules
        def abstract apply ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
        def abstract finish ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool


.. _handle-builtin-skip_field_lock_check:

.. das:attribute:: skip_field_lock_check

Optimization, which indicates that the structure does not need lock checks::

    [skip_field_lock_check]
    struct PerfContext
        events : array<PerfEvent> // when pushing to this array, lock-check won't be performed
        enabled : bool

.. _handle-builtin-cpp_layout:

.. das:attribute:: cpp_layout

Specifies that structure uses C++ memory layout rules, as oppose to native Daslang memory layout rules::

    [cpp_layout (pod=false)]        // pod=false means that the structure is not plain old data (and different padding rules apply)
    struct CppS1
        vtable : void?              // we are simulating C++ class
        b : int64 = 2l
        c : int = 3

    [cpp_layout (pod=false)]
    struct CppS2 : CppS1            // d will be aligned on the class bounds
        d : int = 4


.. _handle-builtin-safe_when_uninitialized:

.. das:attribute:: safe_when_uninitialized

Marks structure as safe to be used when uninitialized::

    [safe_when_uninitialized]
    struct Foo      // it is safe to use variables of type Foo, without initializing them (they will be initialized with 0s)
        data_bool:bool=true
        data_int:int = 1
        data_float:float = 3.14
        data_bar:Bar <- Bar()
        data_uint_3 : uint[3] = [[ uint 1u; 2u; 3u ]]


.. _handle-builtin-persistent:

.. das:attribute:: persistent

Specifies, that a class or a structure is allocated on the C++ heap, as oppose to Daslang heap::

    [persistent]
    class Foo   // this class will say in memory even after the heap is destroyed or reset
        count : int = 0
        def Foo ( ic : int )
            count = ic


++++++++++
Containers
++++++++++

  *  :ref:`clear (array: array implicit) <function-_at__builtin__c__c_clear_IA_C_c_C_l>` 
  *  :ref:`length (array: array implicit) : int <function-_at__builtin__c__c_length_CIA>` 
  *  :ref:`capacity (array: array implicit) : int <function-_at__builtin__c__c_capacity_CIA>` 
  *  :ref:`empty (iterator: iterator implicit) : bool <function-_at__builtin__c__c_empty_CIG>` 
  *  :ref:`length (table: table implicit) : int <function-_at__builtin__c__c_length_CIT>` 
  *  :ref:`capacity (table: table implicit) : int <function-_at__builtin__c__c_capacity_CIT>` 
  *  :ref:`empty (str: string implicit) : bool <function-_at__builtin__c__c_empty_CIs>` 
  *  :ref:`empty (str: das_string implicit) : bool <function-_at__builtin__c__c_empty_CIH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`resize (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-_at__builtin__c__c_resize_1_ls_Y_ls_numT_gr__dot__gr_A_Ci>` 
  *  :ref:`resize_no_init (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-_at__builtin__c__c_resize_no_init_1_ls_Y_ls_numT_gr__dot__gr_A_Ci>` 
  *  :ref:`reserve (var Arr: array\<auto(numT)\>; newSize: int) : auto <function-_at__builtin__c__c_reserve_1_ls_Y_ls_numT_gr__dot__gr_A_Ci>` 
  *  :ref:`pop (var Arr: array\<auto(numT)\>) : auto <function-_at__builtin__c__c_pop_1_ls_Y_ls_numT_gr__dot__gr_A>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT; at: int) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_CY_ls_numT_gr_L_Ci>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; value: numT) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_CY_ls_numT_gr_L>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: array\<numT\>) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_C1_ls_Y_ls_numT_gr_L_gr_A>` 
  *  :ref:`push (var Arr: array\<auto(numT)\>; varr: numT[]) : auto <function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push (var Arr: array\<auto(numT)[]\>; varr: numT[]) : auto <function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&; at: int) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A_&Y_ls_numT_gr_L_Ci>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT&) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A_&Y_ls_numT_gr_L>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)\>; var value: numT[]) : auto <function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`emplace (var Arr: array\<auto(numT)[]\>; var value: numT[]) : auto <function-_at__builtin__c__c_emplace_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A__lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT|numT#; at: int) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C0_ls_CY_ls_numT_gr_L;C_hh_Y_ls_numT_gr_L_gr_|_Ci>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; value: numT|numT#) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C0_ls_CY_ls_numT_gr_L;C_hh_Y_ls_numT_gr_L_gr_|>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)\>; varr: numT[]) : auto <function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (var Arr: array\<auto(numT)[]\>; varr: numT[]) : auto <function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L>` 
  *  :ref:`push_clone (var A: auto(CT); b: auto(TT)|auto(TT)#) : auto <function-_at__builtin__c__c_push_clone_Y_ls_CT_gr__dot__C0_ls_CY_ls_TT_gr__dot_;C_hh_Y_ls_TT_gr__dot__gr_|>` 
  *  :ref:`back (var a: array\<auto(TT)\> ==const) : TT& <function-_at__builtin__c__c_back__eq_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`back (var a: array\<auto(TT)\># ==const) : TT&# <function-_at__builtin__c__c_back__hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`back (a: array\<auto(TT)\> const ==const) : TT const& <function-_at__builtin__c__c_back_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`back (a: array\<auto(TT)\> const# ==const) : TT const&# <function-_at__builtin__c__c_back_C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`back (var arr: auto(TT) ==const) : auto& <function-_at__builtin__c__c_back__eq_Y_ls_TT_gr__dot_>` 
  *  :ref:`back (arr: auto(TT) const ==const) : auto const& <function-_at__builtin__c__c_back_C_eq_Y_ls_TT_gr__dot_>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int) : auto <function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr__dot__gr_A_Ci>` 
  *  :ref:`erase (var Arr: array\<auto(numT)\>; at: int; count: int) : auto <function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr__dot__gr_A_Ci_Ci>` 
  *  :ref:`remove_value (var arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : bool <function-_at__builtin__c__c_remove_value_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`length (a: auto|auto#) : int <function-_at__builtin__c__c_length_C0_ls_C_dot_;C_hh__dot__gr_|>` 
  *  :ref:`empty (a: array\<auto\>|array\<auto\>#) : bool <function-_at__builtin__c__c_empty_C0_ls_C1_ls__dot__gr_A;C_hh_1_ls__dot__gr_A_gr_|>` 
  *  :ref:`empty (a: table\<auto;auto\>|table\<auto;auto\>#) : bool <function-_at__builtin__c__c_empty_C0_ls_C1_ls__dot__gr_2_ls__dot__gr_T;C_hh_1_ls__dot__gr_2_ls__dot__gr_T_gr_|>` 
  *  :ref:`find (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: keyT; blk: block\<(p:valT? const#):void\>) : auto <function-_at__builtin__c__c_find_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C_hh_1_ls_Y_ls_valT_gr_L_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find (Tab: table\<auto(keyT);void\>; at: keyT|keyT#; blk: block\<(p:void? const):void\>) : auto <function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab: table\<auto(keyT);auto(valT)\> const# ==const; at: keyT; blk: block\<(p:valT const&#):void\>) : auto <function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab: table\<auto(keyT);auto(valT)\> const ==const; at: keyT; blk: block\<(p:valT const&):void\>) : auto <function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (var Tab: table\<auto(keyT);auto(valT)\># ==const; at: keyT; blk: block\<(var p:valT&#):void\>) : auto <function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (var Tab: table\<auto(keyT);auto(valT)\> ==const; at: keyT; blk: block\<(var p:valT&):void\>) : auto <function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`get (Tab: table\<auto(keyT);void\>; at: keyT|keyT#; blk: block\<(var p:void?):void\>) : auto <function-_at__builtin__c__c_get_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_if_exists (Tab: table\<auto(keyT);auto(valT)\>; at: keyT; blk: block\<(p:valT const&):void\>) : auto <function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_if_exists (Tab: table\<auto(keyT);auto(valT)\>#; at: keyT; blk: block\<(p:valT const&#):void\>) : auto <function-_at__builtin__c__c_find_if_exists_C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_if_exists (Tab: table\<auto(keyT);void\>; at: keyT; blk: block\<(p:void? const):void\>) : auto <function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit (var Tab: table\<auto(keyT);auto(valT)\>; at: keyT; blk: block\<(var p:valT?#):void\>) : auto <function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls__hh_1_ls_Y_ls_valT_gr_L_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit (var Tab: table\<auto(keyT);void\>; at: keyT|keyT#; blk: block\<(var p:void?):void\>) : auto <function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit (var Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: keyT) : valT? <function-_at__builtin__c__c_find_for_edit_0_ls_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CY_ls_keyT_gr_L>` 
  *  :ref:`find_for_edit (var Tab: table\<auto(keyT);void\>; at: keyT|keyT#) : void? <function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`find_for_edit_if_exists (var Tab: table\<auto(keyT);auto(valT)\>#; at: keyT; blk: block\<(var p:valT&#):void\>) : auto <function-_at__builtin__c__c_find_for_edit_if_exists__hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit_if_exists (var Tab: table\<auto(keyT);auto(valT)\>; at: keyT; blk: block\<(var p:valT&):void\>) : auto <function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`find_for_edit_if_exists (var Tab: table\<auto(keyT);void\>; at: keyT|keyT#; blk: block\<(var p:void?):void\>) : auto <function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`erase (var Tab: table\<auto(keyT);auto(valT)\>; at: string#) : bool <function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C_hh_s>` 
  *  :ref:`erase (var Tab: table\<auto(keyT);auto(valT)\>; at: keyT|keyT#) : bool <function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`insert (var Tab: table\<auto(keyT);void\>; at: keyT|keyT#) : auto <function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: string#) : bool <function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_C_hh_s>` 
  *  :ref:`key_exists (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; at: keyT|keyT#) : bool <function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 
  *  :ref:`copy_to_local (a: auto(TT)) : TT <function-_at__builtin__c__c_copy_to_local_CY_ls_TT_gr__dot_>` 
  *  :ref:`move_to_local (var a: auto(TT)&) : TT <function-_at__builtin__c__c_move_to_local_&Y_ls_TT_gr__dot_>` 
  *  :ref:`keys (a: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#) : iterator\<keyT const&\> <function-_at__builtin__c__c_keys_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|>` 
  *  :ref:`values (a: table\<auto(keyT);void\> const ==const|table\<auto(keyT);void\> const# ==const) : auto <function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_gr_|>` 
  *  :ref:`values (var a: table\<auto(keyT);void\> ==const|table\<auto(keyT);void\># ==const) : auto <function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T;_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_gr_|>` 
  *  :ref:`values (a: table\<auto(keyT);auto(valT)\> const ==const|table\<auto(keyT);auto(valT)\> const# ==const) : iterator\<valT const&\> <function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|>` 
  *  :ref:`values (var a: table\<auto(keyT);auto(valT)\> ==const|table\<auto(keyT);auto(valT)\># ==const) : iterator\<valT&\> <function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|>` 
  *  :ref:`lock (Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#; blk: block\<(t:table\<keyT;valT\> const#):void\>) : auto <function-_at__builtin__c__c_lock_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`lock_forever (var Tab: table\<auto(keyT);auto(valT)\>|table\<auto(keyT);auto(valT)\>#) : table\<keyT;valT\># <function-_at__builtin__c__c_lock_forever_0_ls_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|>` 
  *  :ref:`next (it: iterator\<auto(TT)\>; var value: TT&) : bool <function-_at__builtin__c__c_next_C1_ls_Y_ls_TT_gr__dot__gr_G_&Y_ls_TT_gr_L>` 
  *  :ref:`each (rng: range) : iterator\<int\> <function-_at__builtin__c__c_each_Cr>` 
  *  :ref:`each (str: string) : iterator\<int\> <function-_at__builtin__c__c_each_Cs>` 
  *  :ref:`each (a: auto(TT)[]) : iterator\<TT&\> <function-_at__builtin__c__c_each_C_lb_-1_rb_Y_ls_TT_gr__dot_>` 
  *  :ref:`each (a: array\<auto(TT)\>) : iterator\<TT&\> <function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`each (a: array\<auto(TT)\>#) : iterator\<TT&#\> <function-_at__builtin__c__c_each_C_hh_1_ls_Y_ls_TT_gr__dot__gr_A>` 
  *  :ref:`each (lam: lambda\<(var arg:auto(argT)):bool\>) : iterator\<argT\> <function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr__dot__gr_1_ls_b_gr__at_>` 
  *  :ref:`each_ref (lam: lambda\<(var arg:auto(argT)?):bool\>) : iterator\<argT&\> <function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr__dot__gr__qm__gr_1_ls_b_gr__at_>` 
  *  :ref:`each_enum (tt: auto(TT)) : iterator\<TT\> <function-_at__builtin__c__c_each_enum_CY_ls_TT_gr__dot_>` 
  *  :ref:`nothing (var it: iterator\<auto(TT)\>) : iterator\<TT\> <function-_at__builtin__c__c_nothing_1_ls_Y_ls_TT_gr__dot__gr_G>` 
  *  :ref:`to_array (it: iterator\<auto(TT)\>) : array\<TT\> <function-_at__builtin__c__c_to_array_C1_ls_Y_ls_TT_gr__dot__gr_G>` 
  *  :ref:`to_array (a: auto(TT)[]) : array\<TT\> <function-_at__builtin__c__c_to_array_C_lb_-1_rb_Y_ls_TT_gr__dot_>` 
  *  :ref:`to_array_move (var a: auto(TT)[]) : array\<TT\> <function-_at__builtin__c__c_to_array_move__lb_-1_rb_Y_ls_TT_gr__dot_>` 
  *  :ref:`to_array_move (var a: auto(TT)) : array\<TT\> <function-_at__builtin__c__c_to_array_move_Y_ls_TT_gr__dot_>` 
  *  :ref:`to_table (a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT;valT\> <function-_at__builtin__c__c_to_table_C_lb_-1_rb_0_ls_Y_ls_keyT_gr__dot_;Y_ls_valT_gr__dot__gr_U>` 
  *  :ref:`to_table (a: auto(keyT)[]) : table\<keyT;void\> <function-_at__builtin__c__c_to_table_C_lb_-1_rb_Y_ls_keyT_gr__dot_>` 
  *  :ref:`to_table_move (var a: auto(keyT)[]) : table\<keyT;void\> <function-_at__builtin__c__c_to_table_move__lb_-1_rb_Y_ls_keyT_gr__dot_>` 
  *  :ref:`to_table_move (var a: tuple\<auto(keyT);auto(valT)\>[]) : table\<keyT;valT\> <function-_at__builtin__c__c_to_table_move__lb_-1_rb_0_ls_Y_ls_keyT_gr__dot_;Y_ls_valT_gr__dot__gr_U>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#) : auto <function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr__dot_;_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#) : auto <function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|>` 
  *  :ref:`sort (var a: auto(TT)[]|auto(TT)[]#; cmp: block\<(x:TT const;y:TT const):bool\>) : auto <function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr__dot_;_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`sort (var a: array\<auto(TT)\>|array\<auto(TT)\>#; cmp: block\<(x:TT const;y:TT const):bool\>) : auto <function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`lock (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var x:array\<TT\>#):auto\>) : auto <function-_at__builtin__c__c_lock_0_ls__eq_1_ls_Y_ls_TT_gr__dot__gr_A;_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`lock (a: array\<auto(TT)\> const ==const|array\<auto(TT)\> const# ==const; blk: block\<(x:array\<TT\> const#):auto\>) : auto <function-_at__builtin__c__c_lock_C0_ls_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A;C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`find_index (arr: array\<auto(TT)\>|array\<auto(TT)\>#; key: TT) : auto <function-_at__builtin__c__c_find_index_C0_ls_C1_ls_Y_ls_TT_gr__dot__gr_A;C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto <function-_at__builtin__c__c_find_index_C0_ls_C_lb_-1_rb_Y_ls_TT_gr__dot_;C_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CY_ls_TT_gr_L>` 
  *  :ref:`find_index (arr: iterator\<auto(TT)\>; key: TT) : auto <function-_at__builtin__c__c_find_index_C1_ls_Y_ls_TT_gr__dot__gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`find_index_if (arr: array\<auto(TT)\>|array\<auto(TT)\>#; blk: block\<(key:TT const):bool\>) : auto <function-_at__builtin__c__c_find_index_if_C0_ls_C1_ls_Y_ls_TT_gr__dot__gr_A;C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr: auto(TT)[]|auto(TT)[]#; blk: block\<(key:TT const):bool\>) : auto <function-_at__builtin__c__c_find_index_if_C0_ls_C_lb_-1_rb_Y_ls_TT_gr__dot_;C_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`find_index_if (arr: iterator\<auto(TT)\>; blk: block\<(key:TT const):bool\>) : auto <function-_at__builtin__c__c_find_index_if_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`has_value (a: auto; key: auto) : auto <function-_at__builtin__c__c_has_value_C_dot__C_dot_>` 
  *  :ref:`subarray (a: auto(TT)[]; r: range) : auto <function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr__dot__Cr>` 
  *  :ref:`subarray (a: auto(TT)[]; r: urange) : auto <function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr__dot__Cz>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: range) : auto <function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr__dot__gr_A_Cr>` 
  *  :ref:`subarray (a: array\<auto(TT)\>; r: urange) : auto <function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr__dot__gr_A_Cz>` 
  *  :ref:`move_to_ref (var a: auto&; var b: auto) : auto <function-_at__builtin__c__c_move_to_ref_&_dot___dot_>` 
  *  :ref:`clear (var t: table\<auto(KT);auto(VT)\>) : auto <function-_at__builtin__c__c_clear_1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T>` 

.. _function-_at__builtin__c__c_clear_IA_C_c_C_l:

.. das:function:: clear(array: array implicit)

// stub


:Arguments: * **array** : array implicit

.. _function-_at__builtin__c__c_length_CIA:

.. das:function:: length(array: array implicit) : int

// stub


:Arguments: * **array** : array implicit

.. _function-_at__builtin__c__c_capacity_CIA:

.. das:function:: capacity(array: array implicit) : int

// stub


:Arguments: * **array** : array implicit

.. _function-_at__builtin__c__c_empty_CIG:

.. das:function:: empty(iterator: iterator implicit) : bool

// stub


:Arguments: * **iterator** : iterator implicit

.. _function-_at__builtin__c__c_length_CIT:

.. das:function:: length(table: table implicit) : int

// stub


:Arguments: * **table** : table implicit

.. _function-_at__builtin__c__c_capacity_CIT:

.. das:function:: capacity(table: table implicit) : int

// stub


:Arguments: * **table** : table implicit

.. _function-_at__builtin__c__c_empty_CIs:

.. das:function:: empty(str: string implicit) : bool

// stub


:Arguments: * **str** : string implicit

.. _function-_at__builtin__c__c_empty_CIH_ls__builtin__c__c_das_string_gr_:

.. das:function:: empty(str: das_string implicit) : bool

// stub


:Arguments: * **str** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-_at__builtin__c__c_resize_1_ls_Y_ls_numT_gr__dot__gr_A_Ci:

.. das:function:: resize(Arr: array<auto(numT)>; newSize: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-_at__builtin__c__c_resize_no_init_1_ls_Y_ls_numT_gr__dot__gr_A_Ci:

.. das:function:: resize_no_init(Arr: array<auto(numT)>; newSize: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-_at__builtin__c__c_reserve_1_ls_Y_ls_numT_gr__dot__gr_A_Ci:

.. das:function:: reserve(Arr: array<auto(numT)>; newSize: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-_at__builtin__c__c_pop_1_ls_Y_ls_numT_gr__dot__gr_A:

.. das:function:: pop(Arr: array<auto(numT)>) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_CY_ls_numT_gr_L_Ci:

.. das:function:: push(Arr: array<auto(numT)>; value: numT; at: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT

            * **at** : int

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_CY_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; value: numT) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_C1_ls_Y_ls_numT_gr_L_gr_A:

.. das:function:: push(Arr: array<auto(numT)>; varr: array<numT>) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : array<numT>

.. _function-_at__builtin__c__c_push_1_ls_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)>; varr: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]

.. _function-_at__builtin__c__c_push_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push(Arr: array<auto(numT)[]>; varr: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A_&Y_ls_numT_gr_L_Ci:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&; at: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

            * **at** : int

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A_&Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT&) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT&

.. _function-_at__builtin__c__c_emplace_1_ls_Y_ls_numT_gr__dot__gr_A__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)>; value: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : numT[-1]

.. _function-_at__builtin__c__c_emplace_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A__lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: emplace(Arr: array<auto(numT)[]>; value: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **value** : numT[-1]

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C0_ls_CY_ls_numT_gr_L;C_hh_Y_ls_numT_gr_L_gr_|_Ci:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT|numT#; at: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT|numT#>

            * **at** : int

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C0_ls_CY_ls_numT_gr_L;C_hh_Y_ls_numT_gr_L_gr_|:

.. das:function:: push_clone(Arr: array<auto(numT)>; value: numT|numT#) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **value** : option<numT|numT#>

.. _function-_at__builtin__c__c_push_clone_1_ls_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)>; varr: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **varr** : numT[-1]

.. _function-_at__builtin__c__c_push_clone_1_ls__lb_-1_rb_Y_ls_numT_gr__dot__gr_A_C_lb_-1_rb_Y_ls_numT_gr_L:

.. das:function:: push_clone(Arr: array<auto(numT)[]>; varr: numT[]) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)[-1]>

            * **varr** : numT[-1]

.. _function-_at__builtin__c__c_push_clone_Y_ls_CT_gr__dot__C0_ls_CY_ls_TT_gr__dot_;C_hh_Y_ls_TT_gr__dot__gr_|:

.. das:function:: push_clone(A: auto(CT); b: auto(TT)|auto(TT)#) : auto

// stub


:Arguments: * **A** : auto(CT)

            * **b** : option<auto(TT)|auto(TT)#>

.. _function-_at__builtin__c__c_back__eq_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: back(a: array<auto(TT)> ==const) : TT&

// stub


:Arguments: * **a** : array<auto(TT)>!

.. _function-_at__builtin__c__c_back__hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: back(a: array<auto(TT)># ==const) : TT&#

// stub


:Arguments: * **a** : array<auto(TT)>#!

.. _function-_at__builtin__c__c_back_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: back(a: array<auto(TT)> const ==const) : TT const&

// stub


:Arguments: * **a** : array<auto(TT)>!

.. _function-_at__builtin__c__c_back_C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: back(a: array<auto(TT)> const# ==const) : TT const&#

// stub


:Arguments: * **a** : array<auto(TT)>#!

.. _function-_at__builtin__c__c_back__eq_Y_ls_TT_gr__dot_:

.. das:function:: back(arr: auto(TT) ==const) : auto&

// stub


:Arguments: * **arr** : auto(TT)!

.. _function-_at__builtin__c__c_back_C_eq_Y_ls_TT_gr__dot_:

.. das:function:: back(arr: auto(TT) const ==const) : auto const&

// stub


:Arguments: * **arr** : auto(TT)!

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr__dot__gr_A_Ci:

.. das:function:: erase(Arr: array<auto(numT)>; at: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_numT_gr__dot__gr_A_Ci_Ci:

.. das:function:: erase(Arr: array<auto(numT)>; at: int; count: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **at** : int

            * **count** : int

.. _function-_at__builtin__c__c_remove_value_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CY_ls_TT_gr_L:

.. das:function:: remove_value(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : bool

// stub


:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-_at__builtin__c__c_length_C0_ls_C_dot_;C_hh__dot__gr_|:

.. das:function:: length(a: auto|auto#) : int

// stub


:Arguments: * **a** : option<auto|auto#>

.. _function-_at__builtin__c__c_empty_C0_ls_C1_ls__dot__gr_A;C_hh_1_ls__dot__gr_A_gr_|:

.. das:function:: empty(a: array<auto>|array<auto>#) : bool

// stub


:Arguments: * **a** : option<array<auto>|array<auto>#>

.. _function-_at__builtin__c__c_empty_C0_ls_C1_ls__dot__gr_2_ls__dot__gr_T;C_hh_1_ls__dot__gr_2_ls__dot__gr_T_gr_|:

.. das:function:: empty(a: table<auto;auto>|table<auto;auto>#) : bool

// stub


:Arguments: * **a** : option<table<auto;auto>|table<auto;auto>#>

.. _function-_at__builtin__c__c_find_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C_hh_1_ls_Y_ls_valT_gr_L_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: keyT; blk: block<(p:valT? const#):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : keyT

            * **blk** : block<(p:valT?#):void>

.. _function-_at__builtin__c__c_find_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_C1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find(Tab: table<auto(keyT);void>; at: keyT|keyT#; blk: block<(p:void? const):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-_at__builtin__c__c_get_C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> const# ==const; at: keyT; blk: block<(p:valT const&#):void>) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : keyT

            * **blk** : block<(p:valT&#):void>

.. _function-_at__builtin__c__c_get_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> const ==const; at: keyT; blk: block<(p:valT const&):void>) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : keyT

            * **blk** : block<(p:valT&):void>

.. _function-_at__builtin__c__c_get__hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)># ==const; at: keyT; blk: block<(var p:valT&#):void>) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#!

            * **at** : keyT

            * **blk** : block<(p:valT&#):void>

.. _function-_at__builtin__c__c_get__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);auto(valT)> ==const; at: keyT; blk: block<(var p:valT&):void>) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>!

            * **at** : keyT

            * **blk** : block<(p:valT&):void>

.. _function-_at__builtin__c__c_get_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: get(Tab: table<auto(keyT);void>; at: keyT|keyT#; blk: block<(var p:void?):void>) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: find_if_exists(Tab: table<auto(keyT);auto(valT)>; at: keyT; blk: block<(p:valT const&):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : keyT

            * **blk** : block<(p:valT&):void>

.. _function-_at__builtin__c__c_find_if_exists_C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: find_if_exists(Tab: table<auto(keyT);auto(valT)>#; at: keyT; blk: block<(p:valT const&#):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#

            * **at** : keyT

            * **blk** : block<(p:valT&#):void>

.. _function-_at__builtin__c__c_find_if_exists_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_C1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find_if_exists(Tab: table<auto(keyT);void>; at: keyT; blk: block<(p:void? const):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : keyT

            * **blk** : block<(p:void?):void>

.. _function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls__hh_1_ls_Y_ls_valT_gr_L_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>; at: keyT; blk: block<(var p:valT?#):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : keyT

            * **blk** : block<(p:valT?#):void>

.. _function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit(Tab: table<auto(keyT);void>; at: keyT|keyT#; blk: block<(var p:void?):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-_at__builtin__c__c_find_for_edit_0_ls_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CY_ls_keyT_gr_L:

.. das:function:: find_for_edit(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: keyT) : valT?

.. warning:: 
  This is unsafe operation.

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : keyT

.. _function-_at__builtin__c__c_find_for_edit_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: find_for_edit(Tab: table<auto(keyT);void>; at: keyT|keyT#) : void?

.. warning:: 
  This is unsafe operation.

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

.. _function-_at__builtin__c__c_find_for_edit_if_exists__hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&_hh_Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit_if_exists(Tab: table<auto(keyT);auto(valT)>#; at: keyT; blk: block<(var p:valT&#):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>#

            * **at** : keyT

            * **blk** : block<(p:valT&#):void>

.. _function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_CY_ls_keyT_gr_L_CN_ls_p_gr_0_ls_&Y_ls_valT_gr_L_gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit_if_exists(Tab: table<auto(keyT);auto(valT)>; at: keyT; blk: block<(var p:valT&):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : keyT

            * **blk** : block<(p:valT&):void>

.. _function-_at__builtin__c__c_find_for_edit_if_exists_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|_CN_ls_p_gr_0_ls_1_ls_v_gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: find_for_edit_if_exists(Tab: table<auto(keyT);void>; at: keyT|keyT#; blk: block<(var p:void?):void>) : auto

.. warning:: 
  This function is deprecated.

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

            * **blk** : block<(p:void?):void>

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C_hh_s:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: string#) : bool

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : string#

.. _function-_at__builtin__c__c_erase_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: erase(Tab: table<auto(keyT);auto(valT)>; at: keyT|keyT#) : bool

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

.. _function-_at__builtin__c__c_insert_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: insert(Tab: table<auto(keyT);void>; at: keyT|keyT#) : auto

// stub


:Arguments: * **Tab** : table<auto(keyT);void>

            * **at** : option<keyT|keyT#>

.. _function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_C_hh_s:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: string#) : bool

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : string#

.. _function-_at__builtin__c__c_key_exists_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: key_exists(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; at: keyT|keyT#) : bool

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **at** : option<keyT|keyT#>

.. _function-_at__builtin__c__c_copy_to_local_CY_ls_TT_gr__dot_:

.. das:function:: copy_to_local(a: auto(TT)) : TT

// stub


:Arguments: * **a** : auto(TT)

.. _function-_at__builtin__c__c_move_to_local_&Y_ls_TT_gr__dot_:

.. das:function:: move_to_local(a: auto(TT)&) : TT

// stub


:Arguments: * **a** : auto(TT)&

.. _function-_at__builtin__c__c_keys_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|:

.. das:function:: keys(a: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#) : iterator<keyT const&>

// stub


:Arguments: * **a** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

.. _function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);void> const ==const|table<auto(keyT);void> const# ==const) : auto

// stub


:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T;_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_v_gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);void> ==const|table<auto(keyT);void># ==const) : auto

// stub


:Arguments: * **a** : option<table<auto(keyT);void>!|table<auto(keyT);void>#!>

.. _function-_at__builtin__c__c_values_C0_ls_C_eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)> const ==const|table<auto(keyT);auto(valT)> const# ==const) : iterator<valT const&>

// stub


:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-_at__builtin__c__c_values_0_ls__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh__eq_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|:

.. das:function:: values(a: table<auto(keyT);auto(valT)> ==const|table<auto(keyT);auto(valT)># ==const) : iterator<valT&>

// stub


:Arguments: * **a** : option<table<auto(keyT);auto(valT)>!|table<auto(keyT);auto(valT)>#!>

.. _function-_at__builtin__c__c_lock_C0_ls_C1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;C_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|_CN_ls_t_gr_0_ls_C_hh_1_ls_Y_ls_keyT_gr_L_gr_2_ls_Y_ls_valT_gr_L_gr_T_gr_1_ls_v_gr__builtin_:

.. das:function:: lock(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#; blk: block<(t:table<keyT;valT> const#):void>) : auto

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

            * **blk** : block<(t:table<keyT;valT>#):void>

.. _function-_at__builtin__c__c_lock_forever_0_ls_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T;_hh_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_gr_|:

.. das:function:: lock_forever(Tab: table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#) : table<keyT;valT>#

// stub


:Arguments: * **Tab** : option<table<auto(keyT);auto(valT)>|table<auto(keyT);auto(valT)>#>

.. _function-_at__builtin__c__c_next_C1_ls_Y_ls_TT_gr__dot__gr_G_&Y_ls_TT_gr_L:

.. das:function:: next(it: iterator<auto(TT)>; value: TT&) : bool

// stub


:Arguments: * **it** : iterator<auto(TT)>

            * **value** : TT&

.. _function-_at__builtin__c__c_each_Cr:

.. das:function:: each(rng: range) : iterator<int>

// stub


:Arguments: * **rng** : range

.. _function-_at__builtin__c__c_each_Cs:

.. das:function:: each(str: string) : iterator<int>

// stub


:Arguments: * **str** : string

.. _function-_at__builtin__c__c_each_C_lb_-1_rb_Y_ls_TT_gr__dot_:

.. das:function:: each(a: auto(TT)[]) : iterator<TT&>

// stub


:Arguments: * **a** : auto(TT)[-1]

.. _function-_at__builtin__c__c_each_C1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: each(a: array<auto(TT)>) : iterator<TT&>

// stub


:Arguments: * **a** : array<auto(TT)>

.. _function-_at__builtin__c__c_each_C_hh_1_ls_Y_ls_TT_gr__dot__gr_A:

.. das:function:: each(a: array<auto(TT)>#) : iterator<TT&#>

// stub


:Arguments: * **a** : array<auto(TT)>#

.. _function-_at__builtin__c__c_each_CN_ls_arg_gr_0_ls_Y_ls_argT_gr__dot__gr_1_ls_b_gr__at_:

.. das:function:: each(lam: lambda<(var arg:auto(argT)):bool>) : iterator<argT>

// stub


:Arguments: * **lam** : lambda<(arg:auto(argT)):bool>

.. _function-_at__builtin__c__c_each_ref_CN_ls_arg_gr_0_ls_1_ls_Y_ls_argT_gr__dot__gr__qm__gr_1_ls_b_gr__at_:

.. das:function:: each_ref(lam: lambda<(var arg:auto(argT)?):bool>) : iterator<argT&>

// stub


:Arguments: * **lam** : lambda<(arg:auto(argT)?):bool>

.. _function-_at__builtin__c__c_each_enum_CY_ls_TT_gr__dot_:

.. das:function:: each_enum(tt: auto(TT)) : iterator<TT>

// stub


:Arguments: * **tt** : auto(TT)

.. _function-_at__builtin__c__c_nothing_1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: nothing(it: iterator<auto(TT)>) : iterator<TT>

// stub


:Arguments: * **it** : iterator<auto(TT)>

.. _function-_at__builtin__c__c_to_array_C1_ls_Y_ls_TT_gr__dot__gr_G:

.. das:function:: to_array(it: iterator<auto(TT)>) : array<TT>

// stub


:Arguments: * **it** : iterator<auto(TT)>

.. _function-_at__builtin__c__c_to_array_C_lb_-1_rb_Y_ls_TT_gr__dot_:

.. das:function:: to_array(a: auto(TT)[]) : array<TT>

// stub


:Arguments: * **a** : auto(TT)[-1]

.. _function-_at__builtin__c__c_to_array_move__lb_-1_rb_Y_ls_TT_gr__dot_:

.. das:function:: to_array_move(a: auto(TT)[]) : array<TT>

// stub


:Arguments: * **a** : auto(TT)[-1]

.. _function-_at__builtin__c__c_to_array_move_Y_ls_TT_gr__dot_:

.. das:function:: to_array_move(a: auto(TT)) : array<TT>

// stub


:Arguments: * **a** : auto(TT)

.. _function-_at__builtin__c__c_to_table_C_lb_-1_rb_0_ls_Y_ls_keyT_gr__dot_;Y_ls_valT_gr__dot__gr_U:

.. das:function:: to_table(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT;valT>

// stub


:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-_at__builtin__c__c_to_table_C_lb_-1_rb_Y_ls_keyT_gr__dot_:

.. das:function:: to_table(a: auto(keyT)[]) : table<keyT;void>

// stub


:Arguments: * **a** : auto(keyT)[-1]

.. _function-_at__builtin__c__c_to_table_move__lb_-1_rb_Y_ls_keyT_gr__dot_:

.. das:function:: to_table_move(a: auto(keyT)[]) : table<keyT;void>

// stub


:Arguments: * **a** : auto(keyT)[-1]

.. _function-_at__builtin__c__c_to_table_move__lb_-1_rb_0_ls_Y_ls_keyT_gr__dot_;Y_ls_valT_gr__dot__gr_U:

.. das:function:: to_table_move(a: tuple<auto(keyT);auto(valT)>[]) : table<keyT;valT>

// stub


:Arguments: * **a** : tuple<auto(keyT);auto(valT)>[-1]

.. _function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr__dot_;_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#) : auto

// stub


:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

.. _function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

.. _function-_at__builtin__c__c_sort_0_ls__lb_-1_rb_Y_ls_TT_gr__dot_;_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: auto(TT)[]|auto(TT)[]#; cmp: block<(x:TT const;y:TT const):bool>) : auto

// stub


:Arguments: * **a** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-_at__builtin__c__c_sort_0_ls_1_ls_Y_ls_TT_gr__dot__gr_A;_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x;y_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: sort(a: array<auto(TT)>|array<auto(TT)>#; cmp: block<(x:TT const;y:TT const):bool>) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>|array<auto(TT)>#>

            * **cmp** : block<(x:TT;y:TT):bool>

.. _function-_at__builtin__c__c_lock_0_ls__eq_1_ls_Y_ls_TT_gr__dot__gr_A;_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var x:array<TT>#):auto>) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-_at__builtin__c__c_lock_C0_ls_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A;C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_x_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr_L_gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: lock(a: array<auto(TT)> const ==const|array<auto(TT)> const# ==const; blk: block<(x:array<TT> const#):auto>) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(x:array<TT>#):auto>

.. _function-_at__builtin__c__c_find_index_C0_ls_C1_ls_Y_ls_TT_gr__dot__gr_A;C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: array<auto(TT)>|array<auto(TT)>#; key: TT) : auto

// stub


:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **key** : TT

.. _function-_at__builtin__c__c_find_index_C0_ls_C_lb_-1_rb_Y_ls_TT_gr__dot_;C_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: auto(TT)[]|auto(TT)[]#; key: TT) : auto

// stub


:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **key** : TT

.. _function-_at__builtin__c__c_find_index_C1_ls_Y_ls_TT_gr__dot__gr_G_CY_ls_TT_gr_L:

.. das:function:: find_index(arr: iterator<auto(TT)>; key: TT) : auto

// stub


:Arguments: * **arr** : iterator<auto(TT)>

            * **key** : TT

.. _function-_at__builtin__c__c_find_index_if_C0_ls_C1_ls_Y_ls_TT_gr__dot__gr_A;C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: array<auto(TT)>|array<auto(TT)>#; blk: block<(key:TT const):bool>) : auto

// stub


:Arguments: * **arr** : option<array<auto(TT)>|array<auto(TT)>#>

            * **blk** : block<(key:TT):bool>

.. _function-_at__builtin__c__c_find_index_if_C0_ls_C_lb_-1_rb_Y_ls_TT_gr__dot_;C_hh__lb_-1_rb_Y_ls_TT_gr__dot__gr_|_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: auto(TT)[]|auto(TT)[]#; blk: block<(key:TT const):bool>) : auto

// stub


:Arguments: * **arr** : option<auto(TT)[-1]|auto(TT)[-1]#>

            * **blk** : block<(key:TT):bool>

.. _function-_at__builtin__c__c_find_index_if_C1_ls_Y_ls_TT_gr__dot__gr_G_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: find_index_if(arr: iterator<auto(TT)>; blk: block<(key:TT const):bool>) : auto

// stub


:Arguments: * **arr** : iterator<auto(TT)>

            * **blk** : block<(key:TT):bool>

.. _function-_at__builtin__c__c_has_value_C_dot__C_dot_:

.. das:function:: has_value(a: auto; key: auto) : auto

// stub


:Arguments: * **a** : auto

            * **key** : auto

.. _function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr__dot__Cr:

.. das:function:: subarray(a: auto(TT)[]; r: range) : auto

// stub


:Arguments: * **a** : auto(TT)[-1]

            * **r** : range

.. _function-_at__builtin__c__c_subarray_C_lb_-1_rb_Y_ls_TT_gr__dot__Cz:

.. das:function:: subarray(a: auto(TT)[]; r: urange) : auto

// stub


:Arguments: * **a** : auto(TT)[-1]

            * **r** : urange

.. _function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr__dot__gr_A_Cr:

.. das:function:: subarray(a: array<auto(TT)>; r: range) : auto

// stub


:Arguments: * **a** : array<auto(TT)>

            * **r** : range

.. _function-_at__builtin__c__c_subarray_C1_ls_Y_ls_TT_gr__dot__gr_A_Cz:

.. das:function:: subarray(a: array<auto(TT)>; r: urange) : auto

// stub


:Arguments: * **a** : array<auto(TT)>

            * **r** : urange

.. _function-_at__builtin__c__c_move_to_ref_&_dot___dot_:

.. das:function:: move_to_ref(a: auto&; b: auto) : auto

// stub


:Arguments: * **a** : auto&

            * **b** : auto

.. _function-_at__builtin__c__c_clear_1_ls_Y_ls_KT_gr__dot__gr_2_ls_Y_ls_VT_gr__dot__gr_T:

.. das:function:: clear(t: table<auto(KT);auto(VT)>) : auto

// stub


:Arguments: * **t** : table<auto(KT);auto(VT)>

++++++++++++++++++++++++
das::string manipulation
++++++++++++++++++++++++

  *  :ref:`peek (src: das_string implicit; block: block\<(arg0:string const#):void\> implicit) <function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_peek_CIH_ls__builtin__c__c_das_string_gr__CI0_ls_C_hh_s_gr_1_ls_v_gr__builtin__C_c_C_l:

.. das:function:: peek(src: das_string implicit; block: block<(arg0:string const#):void> implicit)

// stub


:Arguments: * **src** :  :ref:`das_string <handle-builtin-das_string>`  implicit

            * **block** : block<(string#):void> implicit

++++++++++++++
Heap reporting
++++++++++++++

  *  :ref:`heap_allocation_stats () : urange64 <function-_at__builtin__c__c_heap_allocation_stats_C_c>` 
  *  :ref:`heap_allocation_count () : uint64 <function-_at__builtin__c__c_heap_allocation_count_C_c>` 
  *  :ref:`string_heap_allocation_stats () : urange64 <function-_at__builtin__c__c_string_heap_allocation_stats_C_c>` 
  *  :ref:`string_heap_allocation_count () : uint64 <function-_at__builtin__c__c_string_heap_allocation_count_C_c>` 
  *  :ref:`heap_bytes_allocated () : uint64 <function-_at__builtin__c__c_heap_bytes_allocated_C_c>` 
  *  :ref:`heap_depth () : int <function-_at__builtin__c__c_heap_depth_C_c>` 
  *  :ref:`string_heap_bytes_allocated () : uint64 <function-_at__builtin__c__c_string_heap_bytes_allocated_C_c>` 
  *  :ref:`string_heap_depth () : int <function-_at__builtin__c__c_string_heap_depth_C_c>` 
  *  :ref:`heap_collect (string_heap: bool = true; validate: bool = false) <function-_at__builtin__c__c_heap_collect_Cb_Cb_C_c_C_l>` 
  *  :ref:`string_heap_report () <function-_at__builtin__c__c_string_heap_report_C_c_C_l>` 
  *  :ref:`heap_report () <function-_at__builtin__c__c_heap_report_C_c_C_l>` 
  *  :ref:`memory_report (errorsOnly: bool) <function-_at__builtin__c__c_memory_report_Cb_C_c_C_l>` 

.. _function-_at__builtin__c__c_heap_allocation_stats_C_c:

.. das:function:: heap_allocation_stats() : urange64

// stub


.. _function-_at__builtin__c__c_heap_allocation_count_C_c:

.. das:function:: heap_allocation_count() : uint64

// stub


.. _function-_at__builtin__c__c_string_heap_allocation_stats_C_c:

.. das:function:: string_heap_allocation_stats() : urange64

// stub


.. _function-_at__builtin__c__c_string_heap_allocation_count_C_c:

.. das:function:: string_heap_allocation_count() : uint64

// stub


.. _function-_at__builtin__c__c_heap_bytes_allocated_C_c:

.. das:function:: heap_bytes_allocated() : uint64

// stub


.. _function-_at__builtin__c__c_heap_depth_C_c:

.. das:function:: heap_depth() : int

// stub


.. _function-_at__builtin__c__c_string_heap_bytes_allocated_C_c:

.. das:function:: string_heap_bytes_allocated() : uint64

// stub


.. _function-_at__builtin__c__c_string_heap_depth_C_c:

.. das:function:: string_heap_depth() : int

// stub


.. _function-_at__builtin__c__c_heap_collect_Cb_Cb_C_c_C_l:

.. das:function:: heap_collect(string_heap: bool = true; validate: bool = false)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **string_heap** : bool

            * **validate** : bool

.. _function-_at__builtin__c__c_string_heap_report_C_c_C_l:

.. das:function:: string_heap_report()

// stub


.. _function-_at__builtin__c__c_heap_report_C_c_C_l:

.. das:function:: heap_report()

// stub


.. _function-_at__builtin__c__c_memory_report_Cb_C_c_C_l:

.. das:function:: memory_report(errorsOnly: bool)

// stub


:Arguments: * **errorsOnly** : bool

++++++++++++++++++
GC0 infrastructure
++++++++++++++++++

  *  :ref:`gc0_save_ptr (name: string implicit; data: void? implicit) <function-_at__builtin__c__c_gc0_save_ptr_CIs_CI_qm__C_c_C_l>` 
  *  :ref:`gc0_save_smart_ptr (name: string implicit; data: smart_ptr\<void\> implicit) <function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`gc0_restore_ptr (name: string implicit) : void? <function-_at__builtin__c__c_gc0_restore_ptr_CIs_C_c>` 
  *  :ref:`gc0_restore_smart_ptr (name: string implicit) : smart_ptr\<void\> <function-_at__builtin__c__c_gc0_restore_smart_ptr_CIs_C_c>` 
  *  :ref:`gc0_reset () <function-_at__builtin__c__c_gc0_reset>` 

.. _function-_at__builtin__c__c_gc0_save_ptr_CIs_CI_qm__C_c_C_l:

.. das:function:: gc0_save_ptr(name: string implicit; data: void? implicit)

// stub


:Arguments: * **name** : string implicit

            * **data** : void? implicit

.. _function-_at__builtin__c__c_gc0_save_smart_ptr_CIs_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: gc0_save_smart_ptr(name: string implicit; data: smart_ptr<void> implicit)

// stub


:Arguments: * **name** : string implicit

            * **data** : smart_ptr<void> implicit

.. _function-_at__builtin__c__c_gc0_restore_ptr_CIs_C_c:

.. das:function:: gc0_restore_ptr(name: string implicit) : void?

// stub


:Arguments: * **name** : string implicit

.. _function-_at__builtin__c__c_gc0_restore_smart_ptr_CIs_C_c:

.. das:function:: gc0_restore_smart_ptr(name: string implicit) : smart_ptr<void>

// stub


:Arguments: * **name** : string implicit

.. _function-_at__builtin__c__c_gc0_reset:

.. das:function:: gc0_reset()

// stub


++++++++++++++++++++++++
Smart ptr infrastructure
++++++++++++++++++++++++

  *  :ref:`move_new (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-_at__builtin__c__c_move_new_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l>` 
  *  :ref:`move (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\>& implicit) <function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_&I1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: void? implicit) <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l>` 
  *  :ref:`smart_ptr_clone (dest: smart_ptr\<void\>& implicit; src: smart_ptr\<void\> implicit) <function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_use_count (ptr: smart_ptr\<void\> implicit) : uint <function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr__qm_M_C_c_C_l>` 
  *  :ref:`smart_ptr_is_valid (dest: smart_ptr\<void\> implicit) : bool <function-_at__builtin__c__c_smart_ptr_is_valid_CI1_ls_v_gr__qm_M>` 
  *  :ref:`get_ptr (src: smart_ptr\<auto(TT)\> const ==const) : TT? <function-_at__builtin__c__c_get_ptr_C_eq_1_ls_Y_ls_TT_gr__dot__gr__qm_M>` 
  *  :ref:`get_ptr (var src: smart_ptr\<auto(TT)\> ==const) : TT? <function-_at__builtin__c__c_get_ptr__eq_1_ls_Y_ls_TT_gr__dot__gr__qm_M>` 
  *  :ref:`get_const_ptr (src: smart_ptr\<auto(TT)\>) : TT? <function-_at__builtin__c__c_get_const_ptr_C1_ls_Y_ls_TT_gr__dot__gr__qm_M>` 
  *  :ref:`add_ptr_ref (src: smart_ptr\<auto(TT)\>) : smart_ptr\<TT\> <function-_at__builtin__c__c_add_ptr_ref_C1_ls_Y_ls_TT_gr__dot__gr__qm_M>` 

.. _function-_at__builtin__c__c_move_new_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: move_new(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

// stub


:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: void? implicit)

// stub


:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-_at__builtin__c__c_move_&I1_ls_v_gr__qm_M_&I1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: move(dest: smart_ptr<void>& implicit; src: smart_ptr<void>& implicit)

// stub


:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void>& implicit

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI_qm__C_c_C_l:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: void? implicit)

// stub


:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : void? implicit

.. _function-_at__builtin__c__c_smart_ptr_clone_&I1_ls_v_gr__qm_M_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: smart_ptr_clone(dest: smart_ptr<void>& implicit; src: smart_ptr<void> implicit)

// stub


:Arguments: * **dest** : smart_ptr<void>& implicit

            * **src** : smart_ptr<void> implicit

.. _function-_at__builtin__c__c_smart_ptr_use_count_CI1_ls_v_gr__qm_M_C_c_C_l:

.. das:function:: smart_ptr_use_count(ptr: smart_ptr<void> implicit) : uint

// stub


:Arguments: * **ptr** : smart_ptr<void> implicit

.. _function-_at__builtin__c__c_smart_ptr_is_valid_CI1_ls_v_gr__qm_M:

.. das:function:: smart_ptr_is_valid(dest: smart_ptr<void> implicit) : bool

// stub


:Arguments: * **dest** : smart_ptr<void> implicit

.. _function-_at__builtin__c__c_get_ptr_C_eq_1_ls_Y_ls_TT_gr__dot__gr__qm_M:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> const ==const) : TT?

// stub


:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-_at__builtin__c__c_get_ptr__eq_1_ls_Y_ls_TT_gr__dot__gr__qm_M:

.. das:function:: get_ptr(src: smart_ptr<auto(TT)> ==const) : TT?

// stub


:Arguments: * **src** : smart_ptr<auto(TT)>!

.. _function-_at__builtin__c__c_get_const_ptr_C1_ls_Y_ls_TT_gr__dot__gr__qm_M:

.. das:function:: get_const_ptr(src: smart_ptr<auto(TT)>) : TT?

// stub


:Arguments: * **src** : smart_ptr<auto(TT)>

.. _function-_at__builtin__c__c_add_ptr_ref_C1_ls_Y_ls_TT_gr__dot__gr__qm_M:

.. das:function:: add_ptr_ref(src: smart_ptr<auto(TT)>) : smart_ptr<TT>

// stub


:Arguments: * **src** : smart_ptr<auto(TT)>

++++++++++++++++++++
Macro infrastructure
++++++++++++++++++++

  *  :ref:`is_compiling () : bool <function-_at__builtin__c__c_is_compiling>` 
  *  :ref:`is_compiling_macros () : bool <function-_at__builtin__c__c_is_compiling_macros>` 
  *  :ref:`is_compiling_macros_in_module (name: string implicit) : bool <function-_at__builtin__c__c_is_compiling_macros_in_module_CIs>` 
  *  :ref:`is_reporting_compilation_errors () : bool <function-_at__builtin__c__c_is_reporting_compilation_errors>` 
  *  :ref:`is_in_completion () : bool <function-_at__builtin__c__c_is_in_completion>` 
  *  :ref:`is_folding () : bool <function-_at__builtin__c__c_is_folding>` 

.. _function-_at__builtin__c__c_is_compiling:

.. das:function:: is_compiling() : bool

// stub


.. _function-_at__builtin__c__c_is_compiling_macros:

.. das:function:: is_compiling_macros() : bool

// stub


.. _function-_at__builtin__c__c_is_compiling_macros_in_module_CIs:

.. das:function:: is_compiling_macros_in_module(name: string implicit) : bool

// stub


:Arguments: * **name** : string implicit

.. _function-_at__builtin__c__c_is_reporting_compilation_errors:

.. das:function:: is_reporting_compilation_errors() : bool

// stub


.. _function-_at__builtin__c__c_is_in_completion:

.. das:function:: is_in_completion() : bool

// stub


.. _function-_at__builtin__c__c_is_folding:

.. das:function:: is_folding() : bool

// stub


++++++++
Profiler
++++++++

  *  :ref:`reset_profiler () <function-_at__builtin__c__c_reset_profiler_C_c>` 
  *  :ref:`dump_profile_info () <function-_at__builtin__c__c_dump_profile_info_C_c>` 
  *  :ref:`collect_profile_info () : string <function-_at__builtin__c__c_collect_profile_info_C_c_C_l>` 
  *  :ref:`profile (count: int; category: string implicit; block: block\<\> implicit) : float <function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l>` 

.. _function-_at__builtin__c__c_reset_profiler_C_c:

.. das:function:: reset_profiler()

// stub


.. _function-_at__builtin__c__c_dump_profile_info_C_c:

.. das:function:: dump_profile_info()

// stub


.. _function-_at__builtin__c__c_collect_profile_info_C_c_C_l:

.. das:function:: collect_profile_info() : string

// stub


.. _function-_at__builtin__c__c_profile_Ci_CIs_CI_builtin__C_c_C_l:

.. das:function:: profile(count: int; category: string implicit; block: block<> implicit) : float

// stub


:Arguments: * **count** : int

            * **category** : string implicit

            * **block** : block<void> implicit

++++++++++++++++++++
System infastructure
++++++++++++++++++++

  *  :ref:`get_das_root () : string <function-_at__builtin__c__c_get_das_root_C_c_C_l>` 
  *  :ref:`panic (text: string implicit) <function-_at__builtin__c__c_panic_CIs_C_c_C_l>` 
  *  :ref:`print (text: string implicit) <function-_at__builtin__c__c_print_CIs_C_c_C_l>` 
  *  :ref:`error (text: string implicit) <function-_at__builtin__c__c_error_CIs_C_c_C_l>` 
  *  :ref:`sprint (value: any; flags: print_flags) : string <function-_at__builtin__c__c_sprint__st__CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;singleLine;fixedPoint_gr_t>` 
  *  :ref:`sprint_json (value: any; humanReadable: bool) : string <function-_at__builtin__c__c_sprint_json__st__Cb>` 
  *  :ref:`terminate () <function-_at__builtin__c__c_terminate_C_c_C_l>` 
  *  :ref:`breakpoint () <function-_at__builtin__c__c_breakpoint>` 
  *  :ref:`stackwalk (args: bool = true; vars: bool = true) <function-_at__builtin__c__c_stackwalk_Cb_Cb_C_c_C_l>` 
  *  :ref:`is_intern_strings () : bool <function-_at__builtin__c__c_is_intern_strings_C_c>` 
  *  :ref:`is_in_aot () : bool <function-_at__builtin__c__c_is_in_aot>` 
  *  :ref:`to_log (level: int; text: string implicit) <function-_at__builtin__c__c_to_log_Ci_CIs_C_c_C_l>` 
  *  :ref:`to_compiler_log (text: string implicit) <function-_at__builtin__c__c_to_compiler_log_CIs_C_c_C_l>` 
  *  :ref:`eval_main_loop (block: block\<bool\> implicit) <function-_at__builtin__c__c_eval_main_loop_CI1_ls_b_gr__builtin__C_c_C_l>` 
  *  :ref:`aot_enabled () : bool <function-_at__builtin__c__c_aot_enabled_C_c_C_l>` 

.. _function-_at__builtin__c__c_get_das_root_C_c_C_l:

.. das:function:: get_das_root() : string

// stub


.. _function-_at__builtin__c__c_panic_CIs_C_c_C_l:

.. das:function:: panic(text: string implicit)

// stub


:Arguments: * **text** : string implicit

.. _function-_at__builtin__c__c_print_CIs_C_c_C_l:

.. das:function:: print(text: string implicit)

// stub


:Arguments: * **text** : string implicit

.. _function-_at__builtin__c__c_error_CIs_C_c_C_l:

.. das:function:: error(text: string implicit)

// stub


:Arguments: * **text** : string implicit

.. _function-_at__builtin__c__c_sprint__st__CY_ls_print_flags_gr_N_ls_escapeString;namesAndDimensions;typeQualifiers;refAddresses;singleLine;fixedPoint_gr_t:

.. das:function:: sprint(value: any; flags: print_flags) : string

// stub


:Arguments: * **value** : any

            * **flags** :  :ref:`print_flags <alias-print_flags>` 

.. _function-_at__builtin__c__c_sprint_json__st__Cb:

.. das:function:: sprint_json(value: any; humanReadable: bool) : string

// stub


:Arguments: * **value** : any

            * **humanReadable** : bool

.. _function-_at__builtin__c__c_terminate_C_c_C_l:

.. das:function:: terminate()

// stub


.. _function-_at__builtin__c__c_breakpoint:

.. das:function:: breakpoint()

// stub


.. _function-_at__builtin__c__c_stackwalk_Cb_Cb_C_c_C_l:

.. das:function:: stackwalk(args: bool = true; vars: bool = true)

// stub


:Arguments: * **args** : bool

            * **vars** : bool

.. _function-_at__builtin__c__c_is_intern_strings_C_c:

.. das:function:: is_intern_strings() : bool

// stub


.. _function-_at__builtin__c__c_is_in_aot:

.. das:function:: is_in_aot() : bool

// stub


.. _function-_at__builtin__c__c_to_log_Ci_CIs_C_c_C_l:

.. das:function:: to_log(level: int; text: string implicit)

// stub


:Arguments: * **level** : int

            * **text** : string implicit

.. _function-_at__builtin__c__c_to_compiler_log_CIs_C_c_C_l:

.. das:function:: to_compiler_log(text: string implicit)

// stub


:Arguments: * **text** : string implicit

.. _function-_at__builtin__c__c_eval_main_loop_CI1_ls_b_gr__builtin__C_c_C_l:

.. das:function:: eval_main_loop(block: block<bool> implicit)

// stub


:Arguments: * **block** : block<void> implicit

.. _function-_at__builtin__c__c_aot_enabled_C_c_C_l:

.. das:function:: aot_enabled() : bool

// stub


+++++++++++++++++++
Memory manipulation
+++++++++++++++++++

  *  :ref:`variant_index (arg0: variant\<\> implicit) : int <function-_at__builtin__c__c_variant_index_CIV>` 
  *  :ref:`set_variant_index (variant: variant\<\> implicit; index: int) <function-_at__builtin__c__c_set_variant_index_IV_Ci>` 
  *  :ref:`hash (data: any) : uint64 <function-_at__builtin__c__c_hash__st_>` 
  *  :ref:`hash (data: string implicit) : uint64 <function-_at__builtin__c__c_hash_CIs>` 
  *  :ref:`hash (value: int8) : uint64 <function-_at__builtin__c__c_hash_Ci8>` 
  *  :ref:`hash (value: uint8) : uint64 <function-_at__builtin__c__c_hash_Cu8>` 
  *  :ref:`hash (value: int16) : uint64 <function-_at__builtin__c__c_hash_Ci16>` 
  *  :ref:`hash (value: uint16) : uint64 <function-_at__builtin__c__c_hash_Cu16>` 
  *  :ref:`hash (value: int) : uint64 <function-_at__builtin__c__c_hash_Ci>` 
  *  :ref:`hash (value: uint) : uint64 <function-_at__builtin__c__c_hash_Cu>` 
  *  :ref:`hash (value: int64) : uint64 <function-_at__builtin__c__c_hash_Ci64>` 
  *  :ref:`hash (value: uint64) : uint64 <function-_at__builtin__c__c_hash_Cu64>` 
  *  :ref:`hash (value: void? implicit) : uint64 <function-_at__builtin__c__c_hash_CI_qm_>` 
  *  :ref:`hash (value: float) : uint64 <function-_at__builtin__c__c_hash_Cf>` 
  *  :ref:`hash (value: double) : uint64 <function-_at__builtin__c__c_hash_Cd>` 
  *  :ref:`hash (value: das_string implicit) : uint64 <function-_at__builtin__c__c_hash_CIH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`memcpy (left: void? implicit; right: void? implicit; size: int) <function-_at__builtin__c__c_memcpy_CI_qm__CI_qm__Ci>` 
  *  :ref:`memcmp (left: void? implicit; right: void? implicit; size: int) : int <function-_at__builtin__c__c_memcmp_CI_qm__CI_qm__Ci>` 
  *  :ref:`intptr (p: void?) : uint64 <function-_at__builtin__c__c_intptr_C1_ls_v_gr__qm_>` 
  *  :ref:`intptr (p: smart_ptr\<auto\>) : uint64 <function-_at__builtin__c__c_intptr_C1_ls__dot__gr__qm_M>` 
  *  :ref:`lock_data (var a: array\<auto(TT)\> ==const|array\<auto(TT)\># ==const; blk: block\<(var p:TT?#;s:int const):auto\>) : auto <function-_at__builtin__c__c_lock_data_0_ls__eq_1_ls_Y_ls_TT_gr__dot__gr_A;_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_p;s_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`lock_data (a: array\<auto(TT)\> const ==const|array\<auto(TT)\> const# ==const; blk: block\<(p:TT const? const#;s:int const):auto\>) : auto <function-_at__builtin__c__c_lock_data_C0_ls_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A;C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_p;s_gr_0_ls_C_hh_1_ls_CY_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`map_to_array (data: void?; len: int; blk: block\<(var arg:array\<auto(TT)\>#):auto\>) : auto <function-_at__builtin__c__c_map_to_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_1_ls__dot__gr__builtin_>` 
  *  :ref:`map_to_ro_array (data: void?; len: int; blk: block\<(arg:array\<auto(TT)\> const#):auto\>) : auto <function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_1_ls__dot__gr__builtin_>` 

.. _function-_at__builtin__c__c_variant_index_CIV:

.. das:function:: variant_index(arg0: variant<> implicit) : int

// stub


:Arguments: * **arg0** : variant<> implicit

.. _function-_at__builtin__c__c_set_variant_index_IV_Ci:

.. das:function:: set_variant_index(variant: variant<> implicit; index: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **variant** : variant<> implicit

            * **index** : int

.. _function-_at__builtin__c__c_hash__st_:

.. das:function:: hash(data: any) : uint64

// stub


:Arguments: * **data** : any

.. _function-_at__builtin__c__c_hash_CIs:

.. das:function:: hash(data: string implicit) : uint64

// stub


:Arguments: * **data** : string implicit

.. _function-_at__builtin__c__c_hash_Ci8:

.. das:function:: hash(value: int8) : uint64

// stub


:Arguments: * **value** : int8

.. _function-_at__builtin__c__c_hash_Cu8:

.. das:function:: hash(value: uint8) : uint64

// stub


:Arguments: * **value** : uint8

.. _function-_at__builtin__c__c_hash_Ci16:

.. das:function:: hash(value: int16) : uint64

// stub


:Arguments: * **value** : int16

.. _function-_at__builtin__c__c_hash_Cu16:

.. das:function:: hash(value: uint16) : uint64

// stub


:Arguments: * **value** : uint16

.. _function-_at__builtin__c__c_hash_Ci:

.. das:function:: hash(value: int) : uint64

// stub


:Arguments: * **value** : int

.. _function-_at__builtin__c__c_hash_Cu:

.. das:function:: hash(value: uint) : uint64

// stub


:Arguments: * **value** : uint

.. _function-_at__builtin__c__c_hash_Ci64:

.. das:function:: hash(value: int64) : uint64

// stub


:Arguments: * **value** : int64

.. _function-_at__builtin__c__c_hash_Cu64:

.. das:function:: hash(value: uint64) : uint64

// stub


:Arguments: * **value** : uint64

.. _function-_at__builtin__c__c_hash_CI_qm_:

.. das:function:: hash(value: void? implicit) : uint64

// stub


:Arguments: * **value** : void? implicit

.. _function-_at__builtin__c__c_hash_Cf:

.. das:function:: hash(value: float) : uint64

// stub


:Arguments: * **value** : float

.. _function-_at__builtin__c__c_hash_Cd:

.. das:function:: hash(value: double) : uint64

// stub


:Arguments: * **value** : double

.. _function-_at__builtin__c__c_hash_CIH_ls__builtin__c__c_das_string_gr_:

.. das:function:: hash(value: das_string implicit) : uint64

// stub


:Arguments: * **value** :  :ref:`das_string <handle-builtin-das_string>`  implicit

.. _function-_at__builtin__c__c_memcpy_CI_qm__CI_qm__Ci:

.. das:function:: memcpy(left: void? implicit; right: void? implicit; size: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-_at__builtin__c__c_memcmp_CI_qm__CI_qm__Ci:

.. das:function:: memcmp(left: void? implicit; right: void? implicit; size: int) : int

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **right** : void? implicit

            * **size** : int

.. _function-_at__builtin__c__c_intptr_C1_ls_v_gr__qm_:

.. das:function:: intptr(p: void?) : uint64

// stub


:Arguments: * **p** : void?

.. _function-_at__builtin__c__c_intptr_C1_ls__dot__gr__qm_M:

.. das:function:: intptr(p: smart_ptr<auto>) : uint64

// stub


:Arguments: * **p** : smart_ptr<auto>

.. _function-_at__builtin__c__c_lock_data_0_ls__eq_1_ls_Y_ls_TT_gr__dot__gr_A;_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_p;s_gr_0_ls__hh_1_ls_Y_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls__dot__gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> ==const|array<auto(TT)># ==const; blk: block<(var p:TT?#;s:int const):auto>) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-_at__builtin__c__c_lock_data_C0_ls_C_eq_1_ls_Y_ls_TT_gr__dot__gr_A;C_hh__eq_1_ls_Y_ls_TT_gr__dot__gr_A_gr_|_CN_ls_p;s_gr_0_ls_C_hh_1_ls_CY_ls_TT_gr_L_gr__qm_;Ci_gr_1_ls__dot__gr__builtin_:

.. das:function:: lock_data(a: array<auto(TT)> const ==const|array<auto(TT)> const# ==const; blk: block<(p:TT const? const#;s:int const):auto>) : auto

// stub


:Arguments: * **a** : option<array<auto(TT)>!|array<auto(TT)>#!>

            * **blk** : block<(p:TT?#;s:int):auto>

.. _function-_at__builtin__c__c_map_to_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls__hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: map_to_array(data: void?; len: int; blk: block<(var arg:array<auto(TT)>#):auto>) : auto

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

.. _function-_at__builtin__c__c_map_to_ro_array_C1_ls_v_gr__qm__Ci_CN_ls_arg_gr_0_ls_C_hh_1_ls_Y_ls_TT_gr__dot__gr_A_gr_1_ls__dot__gr__builtin_:

.. das:function:: map_to_ro_array(data: void?; len: int; blk: block<(arg:array<auto(TT)> const#):auto>) : auto

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **data** : void?

            * **len** : int

            * **blk** : block<(arg:array<auto(TT)>#):auto>

+++++++++++++++++
Binary serializer
+++++++++++++++++

  *  :ref:`binary_save (obj: auto; subexpr: block\<(data:array\<uint8\> const):void\>) : auto <function-_at__builtin__c__c_binary_save_C_dot__CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`binary_load (var obj: auto; data: array\<uint8\>) : auto <function-_at__builtin__c__c_binary_load__dot__C1_ls_u8_gr_A>` 

.. _function-_at__builtin__c__c_binary_save_C_dot__CN_ls_data_gr_0_ls_C1_ls_u8_gr_A_gr_1_ls_v_gr__builtin_:

.. das:function:: binary_save(obj: auto; subexpr: block<(data:array<uint8> const):void>) : auto

// stub


:Arguments: * **obj** : auto

            * **subexpr** : block<(data:array<uint8>):void>

.. _function-_at__builtin__c__c_binary_load__dot__C1_ls_u8_gr_A:

.. das:function:: binary_load(obj: auto; data: array<uint8>) : auto

// stub


:Arguments: * **obj** : auto

            * **data** : array<uint8>

+++++++++++++++++++++
Path and command line
+++++++++++++++++++++

  *  :ref:`get_command_line_arguments () : array\<string\> <function-_at__builtin__c__c_get_command_line_arguments>` 

.. _function-_at__builtin__c__c_get_command_line_arguments:

.. das:function:: get_command_line_arguments() : array<string>

// stub


+++++++++++++
Time and date
+++++++++++++

  *  :ref:`get_clock () : clock <function-_at__builtin__c__c_get_clock>` 
  *  :ref:`mktime (year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock <function-_at__builtin__c__c_mktime_Ci_Ci_Ci_Ci_Ci_Ci>` 
  *  :ref:`ref_time_ticks () : int64 <function-_at__builtin__c__c_ref_time_ticks>` 
  *  :ref:`get_time_usec (ref: int64) : int <function-_at__builtin__c__c_get_time_usec_Ci64>` 
  *  :ref:`get_time_nsec (ref: int64) : int64 <function-_at__builtin__c__c_get_time_nsec_Ci64>` 

.. _function-_at__builtin__c__c_get_clock:

.. das:function:: get_clock() : clock

// stub


.. _function-_at__builtin__c__c_mktime_Ci_Ci_Ci_Ci_Ci_Ci:

.. das:function:: mktime(year: int; month: int; mday: int; hour: int; min: int; sec: int) : clock

// stub


:Arguments: * **year** : int

            * **month** : int

            * **mday** : int

            * **hour** : int

            * **min** : int

            * **sec** : int

.. _function-_at__builtin__c__c_ref_time_ticks:

.. das:function:: ref_time_ticks() : int64

// stub


.. _function-_at__builtin__c__c_get_time_usec_Ci64:

.. das:function:: get_time_usec(ref: int64) : int

// stub


:Arguments: * **ref** : int64

.. _function-_at__builtin__c__c_get_time_nsec_Ci64:

.. das:function:: get_time_nsec(ref: int64) : int64

// stub


:Arguments: * **ref** : int64

+++++++++++++
Lock checking
+++++++++++++

  *  :ref:`lock_count (array: array implicit) : int <function-_at__builtin__c__c_lock_count_CIA>` 
  *  :ref:`set_verify_array_locks (array: array implicit; check: bool) : bool <function-_at__builtin__c__c_set_verify_array_locks_IA_Cb>` 
  *  :ref:`set_verify_table_locks (table: table implicit; check: bool) : bool <function-_at__builtin__c__c_set_verify_table_locks_IT_Cb>` 

.. _function-_at__builtin__c__c_lock_count_CIA:

.. das:function:: lock_count(array: array implicit) : int

// stub


:Arguments: * **array** : array implicit

.. _function-_at__builtin__c__c_set_verify_array_locks_IA_Cb:

.. das:function:: set_verify_array_locks(array: array implicit; check: bool) : bool

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **array** : array implicit

            * **check** : bool

.. _function-_at__builtin__c__c_set_verify_table_locks_IT_Cb:

.. das:function:: set_verify_table_locks(table: table implicit; check: bool) : bool

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **table** : table implicit

            * **check** : bool

+++++++++++++++++++++++
Lock checking internals
+++++++++++++++++++++++

  *  :ref:`_move_with_lockcheck (var a: auto(valA)&; var b: auto(valB)&) : auto <function-_at__builtin__c__c__move_with_lockcheck_&Y_ls_valA_gr__dot__&Y_ls_valB_gr__dot_>` 
  *  :ref:`_return_with_lockcheck (var a: auto(valT)& ==const) : auto& <function-_at__builtin__c__c__return_with_lockcheck_&_eq_Y_ls_valT_gr__dot_>` 
  *  :ref:`_return_with_lockcheck (a: auto(valT) const& ==const) : auto& <function-_at__builtin__c__c__return_with_lockcheck_C&_eq_Y_ls_valT_gr__dot_>` 
  *  :ref:`_at_with_lockcheck (var Tab: table\<auto(keyT);auto(valT)\>; at: keyT|keyT#) : valT& <function-_at__builtin__c__c__at_with_lockcheck_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|>` 

.. _function-_at__builtin__c__c__move_with_lockcheck_&Y_ls_valA_gr__dot__&Y_ls_valB_gr__dot_:

.. das:function:: _move_with_lockcheck(a: auto(valA)&; b: auto(valB)&) : auto

// stub


:Arguments: * **a** : auto(valA)&

            * **b** : auto(valB)&

.. _function-_at__builtin__c__c__return_with_lockcheck_&_eq_Y_ls_valT_gr__dot_:

.. das:function:: _return_with_lockcheck(a: auto(valT)& ==const) : auto&

// stub


:Arguments: * **a** : auto(valT)&!

.. _function-_at__builtin__c__c__return_with_lockcheck_C&_eq_Y_ls_valT_gr__dot_:

.. das:function:: _return_with_lockcheck(a: auto(valT) const& ==const) : auto&

// stub


:Arguments: * **a** : auto(valT)&!

.. _function-_at__builtin__c__c__at_with_lockcheck_1_ls_Y_ls_keyT_gr__dot__gr_2_ls_Y_ls_valT_gr__dot__gr_T_C0_ls_CY_ls_keyT_gr_L;C_hh_Y_ls_keyT_gr_L_gr_|:

.. das:function:: _at_with_lockcheck(Tab: table<auto(keyT);auto(valT)>; at: keyT|keyT#) : valT&

// stub


:Arguments: * **Tab** : table<auto(keyT);auto(valT)>

            * **at** : option<keyT|keyT#>

++++++++++++++
Bit operations
++++++++++++++

  *  :ref:`clz (bits: uint) : uint <function-_at__builtin__c__c_clz_Cu>` 
  *  :ref:`clz (bits: uint64) : uint64 <function-_at__builtin__c__c_clz_Cu64>` 
  *  :ref:`ctz (bits: uint) : uint <function-_at__builtin__c__c_ctz_Cu>` 
  *  :ref:`ctz (bits: uint64) : uint64 <function-_at__builtin__c__c_ctz_Cu64>` 
  *  :ref:`popcnt (bits: uint) : uint <function-_at__builtin__c__c_popcnt_Cu>` 
  *  :ref:`popcnt (bits: uint64) : uint64 <function-_at__builtin__c__c_popcnt_Cu64>` 
  *  :ref:`mul128 (a: uint64; b: uint64) : urange64 <function-_at__builtin__c__c_mul128_Cu64_Cu64>` 

.. _function-_at__builtin__c__c_clz_Cu:

.. das:function:: clz(bits: uint) : uint

// stub


:Arguments: * **bits** : uint

.. _function-_at__builtin__c__c_clz_Cu64:

.. das:function:: clz(bits: uint64) : uint64

// stub


:Arguments: * **bits** : uint64

.. _function-_at__builtin__c__c_ctz_Cu:

.. das:function:: ctz(bits: uint) : uint

// stub


:Arguments: * **bits** : uint

.. _function-_at__builtin__c__c_ctz_Cu64:

.. das:function:: ctz(bits: uint64) : uint64

// stub


:Arguments: * **bits** : uint64

.. _function-_at__builtin__c__c_popcnt_Cu:

.. das:function:: popcnt(bits: uint) : uint

// stub


:Arguments: * **bits** : uint

.. _function-_at__builtin__c__c_popcnt_Cu64:

.. das:function:: popcnt(bits: uint64) : uint64

// stub


:Arguments: * **bits** : uint64

.. _function-_at__builtin__c__c_mul128_Cu64_Cu64:

.. das:function:: mul128(a: uint64; b: uint64) : urange64

// stub


:Arguments: * **a** : uint64

            * **b** : uint64

+++++++++
Intervals
+++++++++

  *  :ref:`interval (arg0: int; arg1: int) : range <function-_at__builtin__c__c_interval_Ci_Ci>` 
  *  :ref:`interval (arg0: uint; arg1: uint) : urange <function-_at__builtin__c__c_interval_Cu_Cu>` 
  *  :ref:`interval (arg0: int64; arg1: int64) : range64 <function-_at__builtin__c__c_interval_Ci64_Ci64>` 
  *  :ref:`interval (arg0: uint64; arg1: uint64) : urange64 <function-_at__builtin__c__c_interval_Cu64_Cu64>` 

.. _function-_at__builtin__c__c_interval_Ci_Ci:

.. das:function:: interval(arg0: int; arg1: int) : range

// stub


:Arguments: * **arg0** : int

            * **arg1** : int

.. _function-_at__builtin__c__c_interval_Cu_Cu:

.. das:function:: interval(arg0: uint; arg1: uint) : urange

// stub


:Arguments: * **arg0** : uint

            * **arg1** : uint

.. _function-_at__builtin__c__c_interval_Ci64_Ci64:

.. das:function:: interval(arg0: int64; arg1: int64) : range64

// stub


:Arguments: * **arg0** : int64

            * **arg1** : int64

.. _function-_at__builtin__c__c_interval_Cu64_Cu64:

.. das:function:: interval(arg0: uint64; arg1: uint64) : urange64

// stub


:Arguments: * **arg0** : uint64

            * **arg1** : uint64

++++
RTTI
++++

  *  :ref:`class_rtti_size (ptr: void? implicit) : int <function-_at__builtin__c__c_class_rtti_size_CI_qm_>` 

.. _function-_at__builtin__c__c_class_rtti_size_CI_qm_:

.. das:function:: class_rtti_size(ptr: void? implicit) : int

// stub


:Arguments: * **ptr** : void? implicit

+++++++++++++++++
Lock verification
+++++++++++++++++

  *  :ref:`set_verify_context_locks (check: bool) : bool <function-_at__builtin__c__c_set_verify_context_locks_Cb_C_c>` 

.. _function-_at__builtin__c__c_set_verify_context_locks_Cb_C_c:

.. das:function:: set_verify_context_locks(check: bool) : bool

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **check** : bool

+++++++++++++++++++++++++++++++
Initialization and finalization
+++++++++++++++++++++++++++++++

  *  :ref:`using (arg0: block\<(var arg0:das_string):void\> implicit) <function-_at__builtin__c__c_using_CI0_ls_XH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin_>` 

.. _function-_at__builtin__c__c_using_CI0_ls_XH_ls__builtin__c__c_das_string_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: using(arg0: block<(var arg0:das_string):void> implicit)

// stub


:Arguments: * **arg0** : block<( :ref:`das_string <handle-builtin-das_string>` ):void> implicit

++++++++++
Algorithms
++++++++++

  *  :ref:`count (start: int = 0; step: int = 1) : iterator\<int\> <function-_at__builtin__c__c_count_Ci_Ci_C_c_C_l>` 
  *  :ref:`ucount (start: uint = 0x0; step: uint = 0x1) : iterator\<uint\> <function-_at__builtin__c__c_ucount_Cu_Cu_C_c_C_l>` 
  *  :ref:`iter_range (foo: auto) : auto <function-_at__builtin__c__c_iter_range_C_dot_>` 
  *  :ref:`swap (var a: auto(TT)&; var b: auto(TT)&) : auto <function-_at__builtin__c__c_swap_&Y_ls_TT_gr__dot__&Y_ls_TT_gr__dot_>` 

.. _function-_at__builtin__c__c_count_Ci_Ci_C_c_C_l:

.. das:function:: count(start: int = 0; step: int = 1) : iterator<int>

// stub


:Arguments: * **start** : int

            * **step** : int

.. _function-_at__builtin__c__c_ucount_Cu_Cu_C_c_C_l:

.. das:function:: ucount(start: uint = 0x0; step: uint = 0x1) : iterator<uint>

// stub


:Arguments: * **start** : uint

            * **step** : uint

.. _function-_at__builtin__c__c_iter_range_C_dot_:

.. das:function:: iter_range(foo: auto) : auto

// stub


:Arguments: * **foo** : auto

.. _function-_at__builtin__c__c_swap_&Y_ls_TT_gr__dot__&Y_ls_TT_gr__dot_:

.. das:function:: swap(a: auto(TT)&; b: auto(TT)&) : auto

// stub


:Arguments: * **a** : auto(TT)&

            * **b** : auto(TT)&

++++++
Memset
++++++

  *  :ref:`memset8 (left: void? implicit; value: uint8; count: int) <function-_at__builtin__c__c_memset8_CI_qm__Cu8_Ci>` 
  *  :ref:`memset16 (left: void? implicit; value: uint16; count: int) <function-_at__builtin__c__c_memset16_CI_qm__Cu16_Ci>` 
  *  :ref:`memset32 (left: void? implicit; value: uint; count: int) <function-_at__builtin__c__c_memset32_CI_qm__Cu_Ci>` 
  *  :ref:`memset64 (left: void? implicit; value: uint64; count: int) <function-_at__builtin__c__c_memset64_CI_qm__Cu64_Ci>` 
  *  :ref:`memset128 (left: void? implicit; value: uint4; count: int) <function-_at__builtin__c__c_memset128_CI_qm__Cu4_Ci>` 

.. _function-_at__builtin__c__c_memset8_CI_qm__Cu8_Ci:

.. das:function:: memset8(left: void? implicit; value: uint8; count: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **value** : uint8

            * **count** : int

.. _function-_at__builtin__c__c_memset16_CI_qm__Cu16_Ci:

.. das:function:: memset16(left: void? implicit; value: uint16; count: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **value** : uint16

            * **count** : int

.. _function-_at__builtin__c__c_memset32_CI_qm__Cu_Ci:

.. das:function:: memset32(left: void? implicit; value: uint; count: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **value** : uint

            * **count** : int

.. _function-_at__builtin__c__c_memset64_CI_qm__Cu64_Ci:

.. das:function:: memset64(left: void? implicit; value: uint64; count: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **value** : uint64

            * **count** : int

.. _function-_at__builtin__c__c_memset128_CI_qm__Cu4_Ci:

.. das:function:: memset128(left: void? implicit; value: uint4; count: int)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **left** : void? implicit

            * **value** : uint4

            * **count** : int

++++++
Malloc
++++++

  *  :ref:`malloc (size: uint64) : void? <function-_at__builtin__c__c_malloc_Cu64>` 
  *  :ref:`free (ptr: void? implicit) <function-_at__builtin__c__c_free_CI_qm_>` 
  *  :ref:`malloc_usable_size (ptr: void? implicit) : uint64 <function-_at__builtin__c__c_malloc_usable_size_CI_qm_>` 

.. _function-_at__builtin__c__c_malloc_Cu64:

.. das:function:: malloc(size: uint64) : void?

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **size** : uint64

.. _function-_at__builtin__c__c_free_CI_qm_:

.. das:function:: free(ptr: void? implicit)

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **ptr** : void? implicit

.. _function-_at__builtin__c__c_malloc_usable_size_CI_qm_:

.. das:function:: malloc_usable_size(ptr: void? implicit) : uint64

.. warning:: 
  This is unsafe operation.

// stub


:Arguments: * **ptr** : void? implicit

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr__dot__gr_A_Ci:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

.. _function-_at__builtin__c__c_resize_and_init_1_ls_Y_ls_numT_gr__dot__gr_A_Ci_CY_ls_numT_gr_L:

.. das:function:: resize_and_init(Arr: array<auto(numT)>; newSize: int; initValue: numT) : auto

// stub


:Arguments: * **Arr** : array<auto(numT)>

            * **newSize** : int

            * **initValue** : numT

.. _function-_at__builtin__c__c_erase_if_1_ls_Y_ls_TT_gr__dot__gr_A_C0_ls_CN_ls_key_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_;CN_ls_key_gr_0_ls_&Y_ls_TT_gr_L_gr_1_ls_b_gr__builtin__gr_|:

.. das:function:: erase_if(arr: array<auto(TT)>; blk: block<(key:TT const):bool>|block<(var key:TT&):bool>) : auto

// stub


:Arguments: * **arr** : array<auto(TT)>

            * **blk** : option<block<(key:TT):bool>|block<(key:TT&):bool>>

.. _function-_at__builtin__c__c_float2_C_dot__C_dot_:

.. das:function:: float2(a: auto; b: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

.. _function-_at__builtin__c__c_float3_C_dot__C_dot__C_dot_:

.. das:function:: float3(a: auto; b: auto; c: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

.. _function-_at__builtin__c__c_float4_C_dot__C_dot__C_dot__C_dot_:

.. das:function:: float4(a: auto; b: auto; c: auto; d: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

            * **d** : auto

.. _function-_at__builtin__c__c_int2_C_dot__C_dot_:

.. das:function:: int2(a: auto; b: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

.. _function-_at__builtin__c__c_int3_C_dot__C_dot__C_dot_:

.. das:function:: int3(a: auto; b: auto; c: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

.. _function-_at__builtin__c__c_int4_C_dot__C_dot__C_dot__C_dot_:

.. das:function:: int4(a: auto; b: auto; c: auto; d: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

            * **d** : auto

.. _function-_at__builtin__c__c_uint2_C_dot__C_dot_:

.. das:function:: uint2(a: auto; b: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

.. _function-_at__builtin__c__c_uint3_C_dot__C_dot__C_dot_:

.. das:function:: uint3(a: auto; b: auto; c: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

.. _function-_at__builtin__c__c_uint4_C_dot__C_dot__C_dot__C_dot_:

.. das:function:: uint4(a: auto; b: auto; c: auto; d: auto) : auto

// stub


:Arguments: * **a** : auto

            * **b** : auto

            * **c** : auto

            * **d** : auto


