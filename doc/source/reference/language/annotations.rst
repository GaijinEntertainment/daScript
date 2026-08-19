.. _annotations:

===========
Annotations
===========

.. index::
    single: Annotations

Annotations are metadata decorators attached to functions, structures, classes, enumerations,
and variables. They control compiler behavior — export, initialization, safety, optimization,
and macro registration.

An annotation is written in square brackets before the declaration it applies to:

.. code-block:: das

    [export]
    def main {
        print("hello\n")
    }

Multiple annotations can be combined with commas:

.. das-doc: alt
.. code-block:: das

    [export, no_aot]
    def main {
        print("hello\n")
    }

Some annotations accept arguments:

.. das-doc: alt
.. code-block:: das

    [init(tag="db")]
    def init_database {
        pass
    }

    [unused_argument(x, y)]
    def foo(x, y : int) {
        pass
    }

---------------------
Function Annotations
---------------------

^^^^^^^^^^^^^^^^^^^^^
Lifecycle
^^^^^^^^^^^^^^^^^^^^^

``[export]``
    Marks a function as callable from the host application. The host invokes exported functions
    by name through the context API:

    .. das-doc: alt
    .. code-block:: das

        [export]
        def main {
            print("hello\n")
        }

``[init]``
    Marks a function to run automatically during context initialization. The function must
    take no arguments and return ``void``:

    .. code-block:: das

        [init]
        def setup {
            pass
        }

    Ordering can be controlled with attributes:

    - ``tag`` — defines a named initialization pass
    - ``before`` — runs before the named pass
    - ``after`` — runs after the named pass

    All ordering attributes imply late initialization:

    .. code-block:: das

        [init(tag="db")]
        def init_database {
            pass
        }

        [init(after="db")]
        def init_cache {
            pass
        }

``[finalize]``
    Marks a function to run automatically during context shutdown. Same constraints as
    ``[init]`` — no arguments, no return value:

    .. code-block:: das

        [finalize]
        def cleanup {
            pass
        }

    Supports a ``late`` attribute for ordering.

``[run]``
    Evaluates calls to the function at compile time and folds the result into the program.
    The function must be free of side effects and must return a foldable value — a call the
    compiler cannot fold is ``error[50500] function did not run at compilation time``:

    .. code-block:: das

        [run]
        def table_size(n : int) : int {
            return n * n + 1
        }

        let size = table_size(4)        // folded to 17 during compilation

    Disabled by the ``disable_run`` option (see :ref:`Options <options>`).

(see :ref:`Program Structure <program_structure>` for the full initialization lifecycle).

^^^^^^^^^^^^^^^^^^^^^
Visibility
^^^^^^^^^^^^^^^^^^^^^

Visibility is controlled with the ``private`` prefix keyword, not an annotation. Place
``private`` after ``def`` to make a function private to the current module:

.. code-block:: das

    def private helper {
        pass
    }

There is no ``[private]`` annotation form.

^^^^^^^^^^^^^^^^^^^^^
Safety
^^^^^^^^^^^^^^^^^^^^^

``[unsafe_deref]``
    Marks a function as allowing unsafe dereferences inside its body:

    .. code-block:: das

        [unsafe_deref]
        def read_ptr(p : int?) {
            return *p
        }

``[unsafe_operation]``
    Marks a function as an unsafe operation. Calling it requires an ``unsafe`` block:

    .. code-block:: das

        [unsafe_operation]
        def dangerous_thing {
            pass
        }

        unsafe {
            dangerous_thing()
        }

``[unsafe_outside_of_for]``
    Marks a function as unsafe when called outside of a ``for`` loop body.

^^^^^^^^^^^^^^^^^^^^^
Lint Control
^^^^^^^^^^^^^^^^^^^^^

``[unused_argument]``
    Suppresses "unused argument" warnings for specific arguments:

    .. code-block:: das

        [unused_argument(x)]
        def handler(x : int) {
            pass
        }

    Multiple arguments can be listed: ``[unused_argument(x, y)]``.

``[nodiscard]``
    Errors if the return value of the function is discarded:

    .. das-doc: expect error[30166]
    .. code-block:: das

        [nodiscard]
        def compute : int {
            return 42
        }

        compute()       // error[30166]: call to compute result is discarded

``[deprecated]``
    Marks a function as deprecated. Produces a compile-time warning when called:

    .. code-block:: das

        [deprecated(message="use new_func instead")]
        def old_func {
            pass
        }

``[no_lint]``
    Disables all lint checks for this function.

``[sideeffects]``
    Declares that the function has side effects, even if the compiler cannot detect any.
    Prevents the optimizer from removing calls to this function.

^^^^^^^^^^^^^^^^^^^^^^
Generics and Contracts
^^^^^^^^^^^^^^^^^^^^^^

``[generic]``
    Marks a function as generic (a template that is instantiated for each unique set of
    argument types):

    .. code-block:: das

        [generic]
        def add(a, b : auto) {
            return a + b
        }

    (see :ref:`Generic Programming <generic_programming>`).

``[expect_ref]``
    Specialization contract: requires named arguments to be references:

    .. code-block:: das

        [expect_ref(arr)]
        def process(var arr : auto) {
            pass
        }

``[expect_dim]``
    Specialization contract: requires named arguments to be fixed-size arrays.

``[expect_any_vector]``
    Specialization contract: requires named arguments to be vector template types.

``[local_only]``
    Verifies that specific arguments are passed as local constructors. The argument value
    indicates the expected state — ``true`` means the argument must be a literal constructor,
    ``false`` means it must not be:

    .. code-block:: das

        [local_only(data=true)]
        def process(data : Foo) {
            pass
        }

Additional contract annotations are available in ``daslib/contracts``
(see `Contract Annotations (daslib)`_ below).

^^^^^^^^^^^^^^^^^^^^^
Optimization and AOT
^^^^^^^^^^^^^^^^^^^^^

``[no_aot]``
    Disables AOT (ahead-of-time) compilation for this function.

``[no_jit]``
    Disables JIT compilation for this function.

``[jit]``
    Requests JIT compilation for this function.

``[inline]``
    Splices the function body into every direct call site during compilation, in every
    execution tier (interpreter, JIT, AOT). This is a contract, not a hint: a function
    shape the inliner can't splice (by-reference or temporary ``#`` result, generators,
    lambdas, class methods, block/lambda literals or ``assume`` expressions in the body,
    recursion through other ``[inline]`` functions) is a compile-time error, as is taking
    the function's address with ``@@`` or calling it from a global or field initializer:

    .. code-block:: das

        require math

        [inline]
        def clamp01(x : float) : float {
            return saturate(x)
        }

    Leaf arguments (constants and variables) substitute textually; pure single-use
    arguments substitute in place; everything else binds a temporary at the call site,
    preserving call-order evaluation of side effects. Bodies may return from several
    places: every return becomes a store into the call site's result temporary — a
    return after which nothing executes rewrites in place at zero cost, and a return
    under a loop rides a generated boolean flag that breaks out of the spliced loops
    (``finally`` sections and iterator cleanup run exactly as they did for the real
    return). The one refused mix is an early return inside a ``finally``-carrying
    block with statements after it — the rewrite would separate the finally from
    declarations it references. Cross-module splices resolve exactly: a body that uses
    its home module's private symbols (or generic instances the caller cannot see)
    splices under a generated ``with (module ...)`` resolution scope, with argument
    temporaries bound outside it so caller expressions keep resolving at the caller.
    What cannot cross is user-spelled ``_::`` dispatch — it binds the program module
    at the call site, so a cross-module ``[inline]`` body carrying it is a compile-time
    error. Operator overloads (``+``, ``==``,
    ``+=``, unary ``-``, ...) take ``[inline]`` too - their operator sites splice exactly
    like calls; punctuation functions dispatched through other node kinds (``[]``,
    ``??``, properties) are refused. ``options disable_inline`` (or the
    host-side ``CodeOfPolicies::disable_inline``) turns splicing off - calls stay regular
    calls - while the declaration-level contract checks (body shape, recursion, ``@@``)
    still run; call-site splice checks do not apply, since nothing splices.

    Separately from the ``[inline]`` contract, optimized builds also inline best-effort
    (silent declines, never errors): calls passing a block literal argument splice
    automatically, and so do plain calls and operator sites of small SAME-MODULE
    non-generic callees - loop-free bodies within the ``auto_inline_cost`` node budget
    (default 32), or private functions referenced exactly once, whose body moves rather
    than duplicates. The heuristic tier never crosses modules: a transplanted body is
    not context-free by language design (``_::`` dispatch and generic-operator
    resolution consult the calling module), so cross-module inlining stays the
    author's explicit ``[inline]`` contract. The plain-call tier is on by default;
    ``options auto_inline_functions = false`` (or the host-side
    ``CodeOfPolicies::auto_inline_functions``) turns just that tier off, and
    ``options disable_auto_inline`` turns all best-effort splicing off.

``[never_inline]``
    Keeps the function out of best-effort (automatic) inlining - block-literal call-site
    splicing and the heuristic ``auto_inline_functions`` tier both skip it. Combining it
    with ``[inline]`` is a compile-time error: the two contracts are contradictory.
    On a block literal (``$ [never_inline] (args) { ... }``) it keeps that block from
    invoke-block inlining. Module-wide, ``options never_inline`` takes the whole module
    out of the game: the pass skips its bodies (instances of its generics included,
    wherever they land), and its functions refuse as callees everywhere - an
    ``[inline]`` call to one degrades to a plain call. As with ``disable_inline``,
    the declaration-level contract checks (body shape, recursion, ``@@``) still run.

``[hybrid]``
    Marks a function as an AOT hybrid — it can call interpreted code from AOT context.

``[alias_cmres]``
    Allows aliasing of the copy-on-move return value.

``[never_alias_cmres]``
    Prevents aliasing of the copy-on-move return value.

``[pinvoke]``
    Marks a function for platform invoke (external function call).

``[type_function]``
    Registers the function as usable in type expressions.

^^^^^^^^^^^^^^^^^^^^^
Macros
^^^^^^^^^^^^^^^^^^^^^

``[_macro]``
    Marks a function as macro initialization code (runs during macro compilation pass).

``[macro_function]``
    Marks a function as existing only in the macro context — excluded from the regular
    program.

``[macro]``
    Defined in ``daslib/ast_boost``. Like ``[_macro]`` but wraps the function body in a
    module-ready check. Requires ``require daslib/ast_boost``, and — like every macro
    registration — a file that declares a ``module``:

    .. das-doc: alt
    .. code-block:: das

        module my_macros
        require daslib/ast_boost

        [macro]
        def setup_macros {
            print("registering macros\n")
        }

``[tag_function]``
    Defined in ``daslib/ast_boost``. Tags a function with string tags for retrieval via
    ``for_each_tag_function``:

    .. code-block:: das

        require daslib/ast_boost

        [tag_function(my_tag)]
        def tagged_func {
            pass
        }

^^^^^^^^^^^^^^^^^^^^^
Markers and Hints
^^^^^^^^^^^^^^^^^^^^^

``[hint]``
    A dummy annotation that carries key-value arguments for optimization hints. Does not
    change behavior by itself.

``[marker]``
    A generic function marker annotation. Does not change behavior.

-------------------------------
Structure and Class Annotations
-------------------------------

``[cpp_layout]``
    Uses C++ memory layout (matching C++ struct alignment rules):

    .. code-block:: das

        [cpp_layout]
        struct CppInterop {
            x : int
            y : float
        }

    Pass ``pod=false`` to allow non-POD layouts: ``[cpp_layout(pod=false)]``.

``[safe_when_uninitialized]``
    Marks the struct as safe even when fields are uninitialized (zero-filled memory is
    a valid state):

    .. code-block:: das

        [safe_when_uninitialized]
        struct Vec2 {
            x : float
            y : float
        }

``[persistent]``
    Makes a structure persistent (survives context reset). All fields must be POD unless
    ``mixed_heap=true`` is specified:

    .. code-block:: das

        [persistent]
        struct Config {
            value : int
        }

``[no_default_initializer]``
    Suppresses generation of the default constructor for this structure.

``[macro_interface]``
    Marks a structure as a macro interface (for the macro system).

``[comment]``
    A dummy annotation for attaching comment metadata to a structure.

``[tag_structure]``
    Defined in ``daslib/ast_boost``. Tags a structure with string tags for later retrieval.

------------------------------------------
Macro Registration Annotations (daslib)
------------------------------------------

These annotations are defined in ``daslib/ast_boost`` and applied to ``class`` declarations that
inherit from the appropriate AST base class. They auto-register the class as a macro during
module compilation.

All accept an optional ``name`` argument. If omitted, the class name is used.

.. list-table::
   :header-rows: 1
   :widths: 30 35 35

   * - Annotation
     - Base class
     - Purpose
   * - ``[function_macro]``
     - ``AstFunctionAnnotation``
     - Custom function decorator
   * - ``[block_macro]``
     - ``AstBlockAnnotation``
     - Custom block decorator
   * - ``[structure_macro]``
     - ``AstStructureAnnotation``
     - Custom struct/class decorator
   * - ``[enumeration_macro]``
     - ``AstEnumerationAnnotation``
     - Custom enum decorator
   * - ``[contract]``
     - ``AstFunctionAnnotation``
     - Specialization constraint
   * - ``[reader_macro]``
     - ``AstReaderMacro``
     - Custom literal/expression reader
   * - ``[comment_reader]``
     - ``AstCommentReader``
     - Custom comment reader
   * - ``[call_macro]``
     - ``AstCallMacro``
     - Intercepts function-like calls
   * - ``[typeinfo_macro]``
     - ``AstTypeInfoMacro``
     - Custom ``typeinfo(...)`` handler
   * - ``[variant_macro]``
     - ``AstVariantMacro``
     - Custom variant type processing
   * - ``[for_loop_macro]``
     - ``AstForLoopMacro``
     - Custom for-loop behavior
   * - ``[capture_macro]``
     - ``AstCaptureMacro``
     - Custom closure capture handling
   * - ``[type_macro]``
     - ``AstTypeMacro``
     - Custom type expression processing
   * - ``[simulate_macro]``
     - ``AstSimulateMacro``
     - Custom simulation node generation
   * - ``[infer_macro]``
     - ``AstPassMacro``
     - Runs during type inference
   * - ``[dirty_infer_macro]``
     - ``AstPassMacro``
     - Runs during dirty inference passes
   * - ``[optimization_macro]``
     - ``AstPassMacro``
     - Runs during optimization
   * - ``[pre_infer_macro]``
     - ``AstPassMacro``
     - Runs before every (re-)inference pass; ``canVisitPass`` gates it per pass
   * - ``[post_infer_macro]``
     - ``AstPassMacro``
     - Runs once inference is done, before lint, folding, and codegen
   * - ``[lint_macro]``
     - ``AstPassMacro``
     - Runs during linting
   * - ``[global_lint_macro]``
     - ``AstPassMacro``
     - Runs after all modules are compiled
   * - ``[post_compile_macro]``
     - ``AstPassMacro``
     - Runs once the module is compiled and its gc root collected
   * - ``[tag_function_macro(tag="...")]``
     - ``AstFunctionAnnotation``
     - Attaches the macro to every function tagged with ``[tag_function]``

Every pass macro — ``[infer_macro]``, ``[dirty_infer_macro]``, ``[optimization_macro]``,
``[pre_infer_macro]``, ``[post_infer_macro]``, ``[lint_macro]``, ``[global_lint_macro]``,
``[post_compile_macro]`` —
derives from the single ``AstPassMacro`` base class and overrides
``apply(prog : ProgramPtr; mod : Module?) : bool``. There is no per-pass base class.
A ``[pre_infer_macro]`` may also override ``canVisitPass(prog, mod, index) : bool``
and return ``false`` to skip a pass (``index`` is the pass number within the current
inference run, 0 after every restart); the other kinds never consult it.

``[tag_function_macro]`` additionally requires a ``tag`` argument; unlike the other
annotations here, that one is not optional.

Example — macro registration runs during module compilation, so the file declares a ``module``:

.. das-doc: alt
.. code-block:: das

    module my_macros
    require daslib/ast_boost

    [function_macro(name="my_decorator")]
    class MyDecorator : AstFunctionAnnotation {
        def override apply(var func : FunctionPtr; var group : ModuleGroup;
                           args : AnnotationArgumentList; var errors : das_string) : bool {
            print("decorating {func.name}\n")
            return true
        }
    }

(see :ref:`Macros <macros>` for details on writing macros).

------------------------------------------
Contract Annotations (daslib)
------------------------------------------

These annotations are defined in ``daslib/contracts`` and used as specialization constraints
on generic function arguments. Each accepts one or more argument names to constrain.

Requires ``require daslib/contracts``.

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Annotation
     - Constraint
   * - ``[expect_any_array(arg)]``
     - Argument must be a dynamic array
   * - ``[expect_any_enum(arg)]``
     - Argument must be an enum
   * - ``[expect_any_bitfield(arg)]``
     - Argument must be a bitfield
   * - ``[expect_any_vector_type(arg)]``
     - Argument must be a vector template type
   * - ``[expect_any_struct(arg)]``
     - Argument must be a struct
   * - ``[expect_any_numeric(arg)]``
     - Argument must be a numeric type
   * - ``[expect_any_workhorse(arg)]``
     - Argument must be a "workhorse" type (int, float, etc.)
   * - ``[expect_any_workhorse_raw(arg)]``
     - Argument must be a raw workhorse type — one with a ``T(x)`` constructor, or ``bool``.
       Narrower than ``expect_any_workhorse``: pointers and enums are rejected
   * - ``[expect_any_tuple(arg)]``
     - Argument must be a tuple
   * - ``[expect_any_variant(arg)]``
     - Argument must be a variant
   * - ``[expect_any_function(arg)]``
     - Argument must be a function type
   * - ``[expect_any_lambda(arg)]``
     - Argument must be a lambda
   * - ``[contracts::expect_ref(arg)]``
     - Argument must be a reference. The name collides with the built-in ``[expect_ref]``
       above, so in a file that requires ``daslib/contracts`` this one is written
       module-qualified — the bare name is ``error[30600] too many options for annotation``
   * - ``[expect_pointer(arg)]``
     - Argument must be a pointer
   * - ``[expect_class(arg)]``
     - Argument must be a class pointer
   * - ``[expect_value_handle(arg)]``
     - Argument must be a value handle type

Example:

.. das-doc: alt
.. code-block:: das

    require daslib/contracts

    [expect_any_array(arr)]
    def first_element(arr : auto) {
        return arr[0]
    }

------------------------------------------
Annotation Syntax Details
------------------------------------------

Annotations can be combined with logical operators for contract composition:

.. code-block:: das

    [expect_ref(a) && expect_dim(b)]
    def process(var a : auto; b : auto) {
        pass
    }

Negation is also supported:

.. code-block:: das

    [!expect_ref(a)]
    def no_ref(a : auto) {
        pass
    }

Annotations on struct/class fields use the ``@`` metadata syntax and appear before the field
declaration:

.. code-block:: das

    class Foo {
        @big
        @min = 13
        @max = 42
        value : int
    }

These ``@`` decorators attach metadata to the field. Macros read them from the structure's
field declarations at compile time. At runtime they are reachable through ``rtti`` — the
field's ``VarInfo`` carries ``annotation_argument_count`` and ``get_annotation_argument``,
under ``options rtti``. A bare ``@big`` is a ``bool`` argument set to ``true``; ``@min = 13``
carries the value.

