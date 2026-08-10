.. _statements:


==========
Statements
==========

.. index::
    single: statements

A Daslang program is a sequence of statements. Statements in Daslang are comparable to
those in C-family languages (C/C++, Java, C#, etc.): assignments, function calls,
control flow, and declarations. There are also Daslang-specific statements such as
``with``, ``assume``, ``static_if``, and generators.

Statements can be separated with a newline or a semicolon ``;``. The closing ``}`` of a
block terminates its last statement, so no trailing semicolon is required before it (see
:ref:`automatic semicolons <automatic-semicolons>` in the lexical structure reference).

----------------
Visibility Block
----------------

.. index::
    pair: block; statement

A sequence of statements delimited by curly brackets is called a *visibility block*.
Variables declared inside a visibility block are only visible within that block:

.. code-block:: das

    def foo {
        var x = 1           // x is visible here
        {
            var y = 2       // y is visible here
            x += y
        }
        // y is no longer visible
    }

Bare blocks can be nested, and each nesting creates a new scope:

.. code-block:: das

    def bar {
        var total = 0
        {
            var a = 1
            {
                var b = 2
                total = a + b
            }
            // b is no longer visible
        }
        // a is no longer visible
    }

Sequential bare blocks allow reusing the same variable name with a different type:

.. code-block:: das

    def baz {
        {
            var x = 42          // x is int
            print("{x}\n")
        }
        {
            var x = "hello"     // x is string — no conflict
            print("{x}\n")
        }
    }

Bare blocks can have a ``finally`` clause that runs when the block exits:

.. das-doc: given def acquire_resource : int { return 1 }
.. das-doc: given def compute(t : int) : int { return t * 2 }

.. code-block:: das

    def process {
        var result = 0
        {
            var temp = acquire_resource()
            result = compute(temp)
        } finally {
            print("block cleanup\n")
        }
    }

.. note::

    At the statement level, ``{`` starts a bare block, not a table literal.
    Table literals such as ``{ "one"=>1, "two"=>2 }`` are expressions and appear on
    the right-hand side of declarations or assignments (see :ref:`Tables <tables>`).

-----------------------
Control Flow Statements
-----------------------

.. index::
    single: control flow statements

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
if/elif/else
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: if/else; statement

Conditionally execute a block depending on the result of a boolean expression:

.. das-doc: given var a, b : int

.. code-block:: das

    if ( a > b ) {
        a = b
    } elif ( a < b ) {
        b = a
    } else {
        print("equal\n")
    }

Daslang has a strong boolean type. Only expressions of type ``bool`` can be used as conditions.

**One-liner if syntax:**

A single-expression if statement can be written on one line:

.. code-block:: das

    if ( a > b ) { a = b }

**Postfix if syntax:**

A terminator or call statement (``return``, ``break``, ``continue``, or a function
call) can carry the condition after it:

.. code-block:: das

    def smaller(a, b : int) : int {
        return b if ( a < b )
        return a
    }

**Ternary-style if:**

A full ternary expression can use if/else inline:

.. code-block:: das

    def pick(a : int) : int {
        return 13 if ( a == 42 ) else return 7
    }

.. _static_if:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
static_if / static_elif
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. index::
    pair: static_if; statement
    pair: static_elif; statement

``static_if`` evaluates its condition at compile time. It is used in generic functions
to select code paths based on type properties. Branches that do not match are removed
from the compiled output, and do not need to be valid for the given types:

.. code-block:: das

    def describe(a) {
        static_if ( typeinfo is_pointer(a) ) {
            print("pointer\n")
        } static_elif ( typeinfo is_ref_type(a) ) {
            print("reference type\n")
        } else {
            print("value type\n")
        }
    }

Unlike regular ``if``, ``static_if`` does not require its condition to be of type ``bool`` —
it only requires a compile-time constant expression.

``static_if`` is the primary mechanism for conditional compilation in generic code
(see :ref:`Generic Programming <generic_programming>`).

^^^^^^^^^^^^^^^^^
while
^^^^^^^^^^^^^^^^^

.. index::
    pair: while; statement

Execute a block repeatedly while a boolean condition is true:

.. das-doc: given def done : bool { return true }

.. code-block:: das

    var i = 0
    while ( i < 10 ) {
        print("{i}\n")
        i++
    }

    while ( true ) {
        if ( done() ) {
            break
        }
    }

The condition must be of type ``bool``.

------------
Ranged Loops
------------

.. index::
    single: Loops

^^^^^^^^
for
^^^^^^^^

.. index::
    pair: for; statement

Execute a block once for each element of one or more iterable sources:

.. code-block:: das

    for ( i in range(0, 10) ) {
        print("{i}\n")          // prints 0 through 9
    }

Multiple iterables can be traversed in parallel. Iteration stops when the shortest source is exhausted:

.. code-block:: das

    var a : array<int>
    var b : int[10]
    resize(a, 4)
    for ( l, r in a, b ) {
        print("{l} == {r}\n")   // iterates over 4 elements (length of a)
    }

Table keys and values can be iterated using the ``keys`` and ``values`` functions:

.. code-block:: das

    var tab <- { "one"=>1, "two"=>2 }
    for ( k, v in keys(tab), values(tab) ) {
        print("{k}: {v}\n")
    }

Iterable types include ranges, arrays, fixed arrays, tables (via ``keys``/``values``), strings (via ``each``),
enumerations (via ``each_enum``), and custom iterators
(see :ref:`Iterators <iterators>`).

Any type can be made directly iterable by defining an ``each`` function that accepts the type
and returns an ``iterator``. When such a function exists, ``for (x in y)`` is equivalent to
``for (x in each(y))``:

.. code-block:: das

    struct Foo {
        data : array<int>
    }

    def each ( f : Foo ) : iterator<int&> {
        return unsafe(each(f.data))
    }

    var f = Foo(data <- [1, 2, 3])
    for ( x in f ) {           // calls each(f) automatically
        print("{x}\n")
    }

**Tuple expansion in for loops:**

When iterating over containers of tuples, elements can be unpacked directly:

.. code-block:: das

    var data <- [(1, 2.0, "three"), (4, 5.0, "six")]
    for ( (a, b, c) in data ) {
        print("{a} {b} {c}\n")
    }

-------
break
-------

.. index::
    pair: break; statement

Terminate the enclosing ``for`` or ``while`` loop immediately:

.. das-doc: given var arr : array<int>
.. das-doc: given var target : int

.. code-block:: das

    for ( x in arr ) {
        if ( x == target ) {
            break
        }
    }

``break`` cannot cross block boundaries. Using ``break`` inside a block passed
to a function is a compilation error.

---------
continue
---------

.. index::
    pair: continue; statement

Skip the rest of the current loop iteration and proceed to the next one:

.. code-block:: das

    for ( x in range(0, 10) ) {
        if ( x % 2 == 0 ) {
            continue
        }
        print("{x}\n")      // prints only odd numbers
    }

---------
return
---------

.. index::
    pair: return; statement

Terminate the current function, block, or lambda and optionally return a value:

.. code-block:: das

    def add(a, b : int) : int {
        return a + b
    }

All return statements in a function must return the same type.
If no expression is given, the function is assumed to return ``void``:

.. code-block:: das

    def greet(name : string) {
        print("Hello, {name}!\n")
        return
    }

**Move-on-return** transfers ownership of a value using the ``<-`` operator:

.. code-block:: das

    def make_array : array<int> {
        var result : array<int>
        result |> push(1)
        result |> push(2)
        return <- result
    }

In generator blocks, ``return`` must always return a boolean expression
where ``false`` indicates the end of generation (see :ref:`Generators <generators>`).

------
yield
------

.. index::
    pair: yield; statement

Output a value from a generator and suspend its execution until the next iteration.
``yield`` can only be used inside ``generator`` blocks:

.. code-block:: das

    var gen <- generator<int>{
        yield 0
        yield 1
        return false    // end of generation
    }

Move semantics are also supported:

.. code-block:: das

    var gen_arrays <- generator<array<int>> {
        var some_array <- [1, 2, 3]
        yield <- some_array
        return false
    }

(see :ref:`Generators <generators>`).

------
pass
------

.. index::
    pair: pass; statement

An explicit no-operation statement. ``pass`` does nothing and can be used as a placeholder
in blocks that are intentionally empty:

.. code-block:: das

    def todo_later {
        pass
    }

------------------
finally
------------------

.. index::
    pair: finally; statement

Declare a block of code that executes when the enclosing scope exits, regardless of
how it exits (normal flow, ``break``, ``continue``, or ``return``):

.. code-block:: das

    def test(a : array<int>; target : int) : int {
        for ( x in a ) {
            if ( x == target ) {
                return x
            }
        }
        return -1
    } finally {
        print("search complete\n")
    }

When attached to a ``for`` or ``while`` loop body, ``finally`` runs **at the end of
every iteration** — on normal fall-through, ``continue``, ``break``, and ``return``.
This makes ``var inscope`` inside a loop body safe: each iteration finalizes its own
scoped variables before the next iteration begins.

.. das-doc: given var sizes : array<int>
.. das-doc: given def allocate(n : int) : array<int> { var r : array<int>; r |> resize(n); return <- r }
.. das-doc: given def consume(b : array<int>) { pass }

.. code-block:: das

    for ( x in sizes ) {
        var inscope buf <- allocate(x)
        consume(buf)
    } finally {
        print("iteration done\n")
    }
    // "iteration done" prints once per element in `sizes`

If the iterator is empty (or the initial ``while`` condition is false), the body
never enters and the loop's ``finally`` never runs.

``finally`` can also be attached to a bare visibility block, where it runs once
when the block exits. The block's own locals are still in scope there:

.. das-doc: given def open_file(name : string) : array<int> { var r : array<int>; r |> push(1); return <- r }
.. das-doc: given def close_file(var h : array<int>) { pass }

.. code-block:: das

    {
        var handle <- open_file("data.bin")
        // ... work with handle ...
    } finally {
        close_file(handle)
    }

A ``finally`` block cannot contain ``break``, ``continue``, or ``return`` statements.

The ``defer`` macro from ``daslib/defer`` provides a convenient way to add cleanup code
to the current scope's ``finally`` block:

.. das-doc: given def acquire : array<int> { var r : array<int>; r |> push(1); return <- r }
.. das-doc: given def release(var r : array<int>) { pass }

.. code-block:: das

    require daslib/defer

    def process {
        var resource <- acquire()
        defer() {
            release(resource)
        }
        // ... use resource ...
    }   // release(resource) is called here

Multiple ``defer`` statements execute in reverse order (last-in, first-out).

The ``defer_delete`` macro adds a ``delete`` statement for its argument without requiring a block.

---------------------------
Local Variable Declarations
---------------------------

.. index::
    pair: Local variables declaration; statement

Local variables can be declared at any point inside a function. They exist from the
point of declaration until the end of their enclosing visibility block.

``let`` declares a read-only (constant) variable, and ``var`` declares a mutable variable:

.. code-block:: das

    let pi = 3.14159        // constant, cannot be modified
    var counter = 0         // mutable, can be modified
    counter++

Variables can be initialized with copy (``=``), move (``<-``), or clone (``:=``) semantics:

.. code-block:: das

    var a <- [1, 2, 3]      // move: a now owns the array
    var b : array<int>
    b := a                  // clone: b is a deep copy of a

If a type is specified, the variable is typed explicitly. Otherwise, the type is inferred
from the initializer:

.. code-block:: das

    var x : int = 42        // explicit type
    var y = 42              // inferred as int
    var z : float           // explicit type, initialized to 0.0

**inscope variables:**

When ``inscope`` is specified, a ``delete`` statement is automatically added to the ``finally``
section of the enclosing block:

.. code-block:: das

    var inscope resource <- acquire()
    // ... use resource ...
    // delete resource is called automatically at end of block

``inscope`` is allowed inside loop bodies — the loop's ``finally`` runs per iteration,
so each iteration finalizes its own scoped variables.

**Variable name aliases (aka):**

The ``aka`` keyword creates an alternative name (alias) for a variable. Both names
refer to the same value. This works in ``let``, ``var``, and ``for`` declarations:

.. code-block:: das

    var a aka alpha = 42
    print("{alpha}\n")           // prints 42 — alpha is the same variable as a

    for (x aka element in [1,2,3]) {
        print("{element}\n")    // element is the same as x
    }

**Tuple expansion:**

Variables can be unpacked from tuples:

.. code-block:: das

    var (x, y, z) = (1, 2.0, "three")
    // x is int, y is float, z is string

------
assume
------

.. index::
    pair: assume; statement

The ``assume`` statement creates a named alias for an expression without creating a new variable.
Every use of the alias substitutes the original expression:

.. code-block:: das

    var data : array<array<int>>
    assume inner = data[0]
    inner |> push(42)       // equivalent to data[0] |> push(42)

``assume`` is particularly useful for simplifying repeated access to nested data:

.. das-doc: given struct Resolution { width, height : int }
.. das-doc: given struct Graphics { resolution : Resolution }
.. das-doc: given struct Settings { graphics : Graphics }
.. das-doc: given var settings : Settings

.. code-block:: das

    assume cfg = settings.graphics.resolution
    print("width={cfg.width}, height={cfg.height}\n")

.. note::
    ``assume`` does not create a variable — it creates a textual substitution.
    The expression is re-evaluated at each point of use.

------
with
------

.. index::
    pair: with; statement

The ``with`` statement brings the fields of a structure, class, or handled type into the
current scope, allowing them to be accessed without a prefix:

.. code-block:: das

    struct Player {
        x, y : float
        health : int
    }

    def reset(var p : Player) {
        with ( p ) {
            x = 0.0
            y = 0.0
            health = 100
        }
    }

Without ``with``, the same code would require the ``p.`` prefix on each field access.

Multiple ``with`` blocks can be nested. If field names conflict, the innermost ``with`` takes precedence.

^^^^^^^^^^^^^^^^^
with (module ...)
^^^^^^^^^^^^^^^^^

.. index::
    pair: with module; statement

The module flavor of ``with`` is a compile-time resolution scope: names inside the block
resolve as if the code were written in the named module — including that module's private
functions, globals, structures, enumerations, and type aliases. Given a module that keeps
both of those to itself:

.. das-doc: file game_internals.das

.. code-block:: das

    module game_internals

    var private secret_state = 42

    def private dump_internal_tables {
        print("internal tables\n")
    }

a ``with (module ...)`` block reaches them:

.. code-block:: das

    require game_internals

    def debug_dump() {
        with ( module game_internals ) {
            print("{secret_state}\n")       // private global of game_internals
            dump_internal_tables()          // private function of game_internals
        }
    }

The scope affects name resolution only — it is erased after type inference and has zero
runtime cost. Inside the block:

- Unqualified names resolve in the named module, seeing its privates and its require graph.
- Local variables are lexical and always win over the module's globals.
- ``_::name`` still resolves at the enclosing (real) module — it is the escape hatch back.
- The enclosing module's own symbols are otherwise **not** visible inside the block,
  exactly as if the code were written in the named module.
- Instances of generics reachable through the named module's require graph resolve too,
  even when their origin module is not visible from the program module.

Nested module-``with`` blocks do not combine: the innermost one wins outright.

The inliner manufactures this scope internally: a cross-module ``[inline]`` or auto-inline
splice whose body uses the callee module's private symbols (or generic instances the caller
cannot see) is wrapped in a generated module-``with``, so the spliced body resolves exactly
as at the callee's own compile. Generated scopes are erased with the rest and are exempt
from the ``with_module_is_unsafe`` policy.

Because it bypasses module privacy, a project can gate the statement: the
``with_module_is_unsafe`` :ref:`option/policy <options>` makes user-written module-``with``
require ``unsafe``, and a ``.das_project`` may demand the same per target module by
exporting ``with_module_unsafe(target_module, file : string) : bool``.

------
delete
------

.. index::
    pair: delete; statement

The ``delete`` statement invokes the finalizer for a value, releasing any resources it holds.
After deletion, the value is zeroed:

.. code-block:: das

    var arr <- [1, 2, 3]
    delete arr              // arr is now empty

Deleting pointers is an unsafe operation because other references to the same data
may still exist:

.. code-block:: das

    var p = new Foo()
    unsafe {
        delete p            // p is set to null, memory is freed
    }

(see :ref:`Finalizers <finalizers>`).

--------------------
Function Declaration
--------------------

.. index::
    pair: Function declaration; statement

Functions are declared with the ``def`` keyword:

.. code-block:: das

    def add(a, b : int) : int {
        return a + b
    }

    def greet {
        print("hello\n")
    }

(see :ref:`Functions <functions>` for a complete description of function features).

-----------
try/recover
-----------

.. index::
    pair: try/recover; statement

Enclose a block of code that may trigger a runtime panic, such as null pointer
dereference or out-of-bounds array access:

.. code-block:: das

    try {
        var p : Foo?
        print("{*p}\n")         // would panic: null pointer dereference
    } recover {
        print("recovered from panic\n")
    }

.. warning::
    ``try``/``recover`` is **not** a general error-handling mechanism and should not be
    used for control flow. It is designed for catching runtime panics (similar to Go's
    ``recover``). In production code, these situations should be prevented rather than caught.

------
panic
------

.. index::
    pair: panic; statement

Trigger a runtime panic with an optional message:

.. code-block:: das

    panic("something went very wrong")

The panic message is available in the runtime log. A panic can be caught by
a ``try``/``recover`` block.

--------------
label and goto
--------------

.. index::
    pair: label; statement
    pair: goto; statement

Daslang supports numeric labels and goto for low-level control flow:

.. code-block:: das

    label 0:
        print("start\n")
        goto label 1
    label 1:
        print("end\n")

Labels use integer identifiers. Computed goto is also supported — the target is any
integer expression naming a label:

.. das-doc: given var label_expression : int

.. code-block:: das

    goto label_expression

.. warning::
    Labels and goto are low-level constructs primarily used in generated code (such as
    generators). They are generally not recommended for regular application code.

--------------------
Expression Statement
--------------------

.. index::
    pair: Expression statement; statement

An expression is valid as a statement when it *does* something — a call, a pipe, an
assignment, an increment. Its result is discarded:

.. code-block:: das

    foo()               // function call as statement
    arr |> push(42)     // pipe expression as statement
    counter++           // increment as statement

A bare built-in binary operator is **not** accepted at statement level: since it has no
side effect, its result could only be discarded, so ``a + b`` on its own line is
``error[30151]: top level no side effect operation +``. (A user-defined ``operator +``
is not built-in and is therefore not rejected.)

----------------
Global Variables
----------------

.. index::
    pair: global variables; statement

Global variables are declared at module scope with ``let`` (constant) or ``var`` (mutable):

.. code-block:: das

    let MAX_SIZE = 1024
    var counter = 0

Global variables are initialized once during script initialization (or each time
``init`` is manually called on the context).

``shared`` indicates that the variable's memory is shared between multiple Context instances
and initialized only once:

.. das-doc: given def generate_table : table<string; int> { var t : table<string; int>; return <- t }

.. code-block:: das

    let shared lookup_table <- generate_table()

``private`` indicates the variable is not visible outside its module:

.. code-block:: das

    var private internal_state = 0

(see :ref:`Constants & Enumerations <constants_and_enumerations>` for more details).

--------------
enum
--------------

.. index::
    pair: enum; statement

Declare an enumeration — a set of named integer constants:

.. code-block:: das

    enum Color {
        Red
        Green
        Blue
    }

(see :ref:`Constants & Enumerations <constants_and_enumerations>`).

--------------
typedef
--------------

.. index::
    pair: typedef; statement

Declare a type alias:

.. code-block:: das

    typedef Vec3 = float3
    typedef IntPair = tuple<int; int>

Type aliases can also be declared locally inside functions or structure bodies
(see :ref:`Type Aliases <aliases>`).

.. seealso::

    :ref:`Expressions <expressions>` for expression syntax and operators,
    :ref:`Functions <functions>` for function declarations using ``def``,
    :ref:`Iterators <iterators>` for ``for`` loop iteration patterns,
    :ref:`Generators <generators>` for ``yield`` in generator functions.

