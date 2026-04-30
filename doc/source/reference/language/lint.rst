.. _lint:

===========
Lint Tools
===========

.. index::
    single: lint
    single: perf_lint
    single: style_lint
    single: Performance Lint
    single: Style Lint

daslang provides three complementary lint passes that detect issues at compile time:

- **Paranoid lint** (``daslib/lint``) — unreachable code, unused variables, variables that can be ``let``, underscore naming, redundant reinterpret casts
- **Performance lint** (``daslib/perf_lint``) — performance anti-patterns (error code ``40217``)
- **Style lint** (``daslib/style_lint``) — non-idiomatic patterns (error code ``40218``)

Each pass can be used independently or together.

-----------
Quick start
-----------

Add the corresponding ``require`` to any file. The lint runs automatically at
compile time and reports warnings inline::

    options gen2
    require daslib/lint          // paranoid
    require daslib/perf_lint     // performance
    require daslib/style_lint    // style

-------------------
Standalone utility
-------------------

A unified utility runs all three passes on files and directories::

    bin/Release/daslang.exe utils/lint/main.das -- <files-or-dirs...> [options]

Options:

- ``--quiet`` — suppress PASS lines and progress messages
- ``--postfix-conditionals`` — enable STYLE005 check
- ``--comment-hygiene`` — enable STYLE014/STYLE015 comment-length checks
- ``--paranoid-only`` — only run paranoid lint
- ``--perf-only`` — only run performance lint
- ``--style-only`` — only run style lint

Examples::

    # Lint a single file (all 3 passes)
    bin/Release/daslang.exe utils/lint/main.das -- daslib/json.das

    # Lint a directory recursively
    bin/Release/daslang.exe utils/lint/main.das -- daslib/

    # Performance lint only, quiet mode
    bin/Release/daslang.exe utils/lint/main.das -- daslib/ --perf-only --quiet

Output format per file:

- ``FAIL`` — file failed to compile
- ``WARN`` — file has lint issues (count and details follow)
- ``PASS`` — file is clean (suppressed with ``--quiet``)

Exit codes: ``0`` = clean, ``1`` = compile errors, ``2`` = warnings only.

------------------------------
Suppressing specific warnings
------------------------------

Add a ``// nolint:CODE`` comment on the same line as the flagged expression::

    let ch = character_at(s, idx) // nolint:PERF003 — single indexed access
    build_string() <| $(var w) {  // nolint:STYLE001 — intentional pipe

The suppression is exact: ``// nolint:PERF003`` only suppresses PERF003, not other
rules. An optional explanation after the code is recommended but not required.

------------------
Important notes
------------------

**Lint runs after optimization.** The lint pass runs on the post-optimization AST.
Patterns in dead code may not trigger warnings. In real code where results are
used, the patterns are preserved and detected correctly.

**ExprRef2Value wrapping.** The compiler wraps many value-type reads in
``ExprRef2Value`` nodes. The lint visitors unwrap these transparently — this is
an implementation detail, not something users need to worry about.

**Closures are excluded.** Code inside closures (blocks, lambdas) is not checked
for loop-related performance patterns, since the closure may be called outside
the loop context.

.. _paranoid_lint:

--------------
Paranoid rules
--------------

.. index::
    single: lint

LINT001 — unreachable code
===========================

Code after a ``return`` or ``panic()`` in the same block is unreachable and
will never execute.

.. code-block:: das

    def foo() {
        return 1
        print("never reached\n")            // LINT001
    }

LINT002 — unused variable
==========================

A declared variable is never read. Prefix the name with an underscore
(``_x``) to suppress the warning, or remove the variable entirely.

.. code-block:: das

    def foo() {
        var x = 5                           // LINT002 — x is never used
        return 1
    }

LINT003 — variable can be ``let``
==================================

A ``var`` variable is never mutated. Declare it with ``let`` instead.

.. code-block:: das

    // Bad
    var x = 5                               // LINT003
    return x

    // Good
    let x = 5
    return x

LINT004 — underscore-prefixed variable is used
================================================

A variable named ``_x`` is conventionally unused. If it is actually accessed,
rename it without the underscore prefix.

.. code-block:: das

    def foo(_x : int) : int {
        return _x                           // LINT004
    }

LINT005 — redundant ``reinterpret`` cast
=========================================

``reinterpret<T>(x)`` where ``T`` is the same type as ``x`` is a no-op.
Remove the cast.

The rule skips casts that strip ``const`` or ``temporary`` modifiers (those
serve a purpose) and casts between ``void?`` and typed pointers. It also
skips generic instantiations and compiler-generated functions.

.. code-block:: das

    // Bad — x is already int?
    var y = unsafe(reinterpret<int?>(x))    // LINT005

    // Good
    var y = x

    // Good — strips const (not flagged)
    var y = unsafe(reinterpret<int?>(const_ptr))

LINT006 — division by zero (constant zero divisor)
====================================================

``x / 0`` and ``x % 0`` produce a runtime panic (integer) or ``inf`` / ``nan``
(float). When the right-hand side is a literal zero, this is almost always a
typo. Also covers the compound forms ``/=`` and ``%=``. Recognizes literal zero
across ``int``, ``uint``, ``int64``, ``uint64``, ``float``, and ``double``.

.. code-block:: das

    // Bad
    let y = x / 0                           // LINT006
    x %= 0                                  // LINT006

    // Good
    let y = x / divisor

LINT007 — identical left and right operands
=============================================

Both sides of a binary operator are the same expression. The result is trivial
(``x == x`` is always true, ``x - x`` is always 0, ``x && x`` is just ``x``)
and the code is almost always a copy-paste typo. Triggers on: ``==``, ``!=``,
``<``, ``>``, ``<=``, ``>=``, ``-``, ``/``, ``%``, ``&&``, ``||``, ``&``,
``|``, ``^``, ``-=``, ``/=``, ``%=``.

.. code-block:: das

    // Bad — author meant `size == capacity` or similar
    if (size == size) { ... }               // LINT007

    // Good
    if (size == capacity) { ... }

Operators like ``+`` and ``*`` are deliberately excluded: ``x + x`` is the
common way to double a value and ``x * x`` is squaring, both of which are
intentional.

**NaN idiom.** ``x != x`` on ``float`` or ``double`` is the canonical IEEE 754
NaN check — daslang has no dedicated ``is_nan`` helper for scalar floats.
Suppress LINT007 on the one line that needs it:

.. code-block:: das

    def is_nan(x : float) : bool {
        return x != x // nolint:LINT007 — canonical IEEE 754 NaN check
    }

**Note on constant folding.** The paranoid lint runs after optimization.
``let x = 1; x == x`` is folded to ``true`` before lint sees it and will not
fire. LINT007 catches cases where operands are runtime values (function
parameters, field reads, function calls).

Structural equality uses the expression pretty-printer: ``describe(left) ==
describe(right)``. This catches both ``x`` vs ``x`` and ``a.b.c`` vs ``a.b.c``,
at the cost of serializing both subtrees. Pointer-identity is not used as a
fast path because daslang AST nodes are not shared — each ``Expression`` has
exactly one parent, so sibling operands are always distinct nodes.

LINT008 — both ternary branches equivalent
============================================

``cond ? x : x`` ignores ``cond`` and always produces ``x``. Copy-paste bug.

.. code-block:: das

    // Bad
    let y = cond ? value : value            // LINT008

    // Good
    let y = cond ? then_value : else_value

LINT009 — ``then`` branch equivalent to ``else`` branch
=========================================================

``if (c) { A } else { A }`` runs ``A`` regardless of ``c``. Usually the author
copy-pasted one branch and forgot to edit the other. Caught even when ``A``
has side effects — the structural pattern is suspicious regardless of purity.

.. code-block:: das

    // Bad
    if (flag) {
        x = 1                               // LINT009
    } else {
        x = 1
    }

    // Good
    if (flag) {
        x = 1
    } else {
        x = 2
    }

.. _perf_lint:

-----------------
Performance rules
-----------------

.. index::
    single: perf_lint

PERF001 — string ``+=`` in loop
================================

String concatenation with ``+=`` inside a loop creates O(n\ :sup:`2`) allocations.
Each iteration allocates a new string of increasing length, copying all previous content.

.. code-block:: das

    // Bad — O(n^2)
    var result = ""
    for (i in range(100)) {
        result += "x"                   // PERF001
    }

    // Good — O(n)
    let result = build_string() $(var writer) {
        for (i in range(100)) {
            write(writer, "x")
        }
    }

PERF002 — ``character_at`` in loop with loop variable
======================================================

``character_at(s, i)`` is O(n) per call because it internally calls ``strlen``
to validate the index. In a loop iterating over string indices with the loop
variable as the index, this becomes O(n\ :sup:`2`) total.

.. code-block:: das

    // Bad — O(n^2)
    for (i in range(length(s))) {
        let ch = character_at(s, i)     // PERF002
    }

    // Good — O(n) total, O(1) per access
    peek_data(s) $(arr) {
        for (i in range(length(arr))) {
            let ch = int(arr[i])
        }
    }

PERF003 — ``character_at`` anywhere
====================================

Informational warning for any use of ``character_at``. Each call does a bounds
check by scanning to the index. For accessing the first character, use
``first_character`` which is O(1). For bulk access in hot paths, consider
``peek_data`` for reads or ``modify_data`` for mutations.

.. code-block:: das

    let ch = character_at(s, 0)         // PERF003 — use first_character(s) instead
    let ch2 = first_character(s)        // O(1), returns 0 for empty string

PERF004 — string interpolation reassignment in loop
=====================================================

``str = "{str}{more}"`` inside a loop has the same O(n\ :sup:`2`) behavior as
``str += "..."``. Each iteration allocates a new string containing all previous
content.

.. code-block:: das

    // Bad — O(n^2)
    var result = ""
    for (i in range(100)) {
        result = "{result}x"            // PERF004
    }

    // Good — O(n)
    let result = build_string() $(var writer) {
        for (i in range(100)) {
            write(writer, "x")
        }
    }

PERF005 — ``length(string)`` in while condition
=================================================

``while (i < length(s))`` recomputes ``strlen(s)`` on every iteration. If ``s``
is not modified in the loop body, this is wasted work. Note that ``for`` loops
do **not** have this problem because ``for`` computes its source expression once.

.. code-block:: das

    // Bad — strlen every iteration
    var i = 0
    while (i < length(s)) {             // PERF005
        i ++
    }

    // Good — cached length
    let slen = length(s)
    var i = 0
    while (i < slen) {
        i ++
    }

PERF006 — ``push``/``emplace`` in loop without ``reserve()``
=============================================================

Calling ``push``, ``push_clone``, or ``emplace`` on an array inside a loop without
a preceding ``reserve()`` may trigger repeated reallocations as the array grows.
The rule traces through field access chains (``self.items``, ``data.buffer``, etc.)
to find the root variable, and distinguishes different field paths — ``reserve(t.a, N)``
does not suppress a warning for ``t.b |> push(x)``.

Conditional pushes (inside ``if``/``else``) and loops with ``break``/``continue``
are not flagged — the number of items is unpredictable, so ``reserve`` would be
guesswork.

.. code-block:: das

    // Bad — may realloc each iteration
    var result : array<int>
    for (i in range(1000)) {
        result |> push(i)                       // PERF006
    }

    // Good — pre-allocate
    var result : array<int>
    result |> reserve(1000)
    for (i in range(1000)) {
        result |> push(i)
    }

PERF007 — unnecessary ``string(das_string)`` in comparison
============================================================

``das_string`` supports direct comparison with string literals and other
``das_string`` values via ``==`` and ``!=``. Wrapping in ``string()`` allocates
a new string unnecessarily.

.. code-block:: das

    // Bad — unnecessary allocation
    if (string(name) == "foo") { ... }      // PERF007

    // Good — direct comparison
    if (name == "foo") { ... }

PERF008 — unnecessary ``get_ptr()`` for ``is``/``as``
======================================================

``ExpressionPtr`` and ``TypeDeclPtr`` support ``is`` and ``as``
type checks directly. Calling ``get_ptr()`` first is unnecessary.

.. code-block:: das

    // Bad — get_ptr is redundant
    if (get_ptr(expr) is ExprVar) { ... }   // PERF008

    // Good — direct type check
    if (expr is ExprVar) { ... }

PERF009 — redundant move-init variable immediately returned
=============================================================

``var x <- expr(); return <- x`` introduces an unnecessary intermediate variable.
The value is moved in and then immediately moved out. Simplify to
``return <- expr()``.

.. code-block:: das

    // Bad — redundant variable
    var inscope result <- make_thing()
    return <- result                        // PERF009

    // Good — direct return
    return <- make_thing()

PERF010 — unnecessary ``get_ptr()`` for null comparison
=========================================================

``smart_ptr`` supports ``==`` and ``!=`` against ``null`` directly.
Calling ``get_ptr()`` first is unnecessary overhead.

.. code-block:: das

    // Bad — get_ptr is redundant
    if (get_ptr(expr) == null) { ... }      // PERF010

    // Good — direct comparison
    if (expr == null) { ... }

PERF011 — unnecessary ``get_ptr()`` for field access
======================================================

``smart_ptr`` auto-dereferences for field access. Calling ``get_ptr()``
first to access a field is unnecessary.

.. code-block:: das

    // Bad — get_ptr is redundant
    let name = get_ptr(expr).__rtti         // PERF011

    // Good — direct field access
    let name = expr.__rtti

PERF012 — ``string(das_string)`` passed to strings function
=============================================================

Wrapping a ``das_string`` in ``string()`` before passing to a function from
the ``strings`` module allocates a temporary string unnecessarily. Use
``peek(das_string)`` instead, which provides a zero-allocation read-only
string reference.

.. code-block:: das

    // Bad — allocates a temporary string
    let pos = find(string(name), "foo")         // PERF012

    // Good — zero allocation
    var pos = -1
    peek(name) $(s) {
        pos = find(s, "foo")
    }

.. _style_lint:

-----------
Style rules
-----------

.. index::
    single: style_lint

STYLE001 — unnecessary ``<|`` pipe before block argument
==========================================================

The ``<|`` pipe syntax is gen1 style and unnecessary in gen2. Use direct
trailing block syntax instead.

.. code-block:: das

    // Bad — gen1 pipe style
    build_string() <| $(var w) {                // STYLE001
        write(w, "hello")
    }

    // Good — gen2 trailing block
    build_string() $(var w) {
        write(w, "hello")
    }

STYLE002 — ``<|`` pipe before parameterless block
===================================================

When the block takes no parameters, both the ``<|`` pipe and ``$()`` are
unnecessary. Use a direct trailing block.

.. code-block:: das

    // Bad — pipe and $() both unnecessary
    takes_block() <| $() {                      // STYLE002
        print("done\n")
    }

    // Good — direct block
    takes_block() {
        print("done\n")
    }

STYLE003 — redundant ``$()`` on parameterless block
=====================================================

When a block takes no parameters, the ``$()`` prefix is unnecessary even
without a pipe. Use a bare trailing block.

.. code-block:: das

    // Bad — redundant $()
    takes_block() $() {                         // STYLE003
        print("done\n")
    }

    // Good — bare block
    takes_block() {
        print("done\n")
    }

STYLE005 — single-statement ``if`` can use postfix form (configurable)
========================================================================

.. note::

    This rule is **off by default**. Enable with ``--postfix-conditionals``
    in the standalone utility.

A single-statement ``if`` without an ``else`` branch that contains only
``return``, ``break``, or ``continue`` can be written more concisely as a
postfix conditional.

.. code-block:: das

    // Suggestion — can use postfix form
    if (x > 0) {
        return x                                // STYLE005
    }
    return -x

    // Postfix form
    return x if (x > 0)
    return -x

STYLE006 — ``string(__rtti)`` comparison should use ``is``
============================================================

Comparing ``string(expr.__rtti) == "ExprFoo"`` is verbose and fragile.
Use the ``is`` operator instead, which is type-safe and cleaner.

.. code-block:: das

    // Bad — manual RTTI string comparison
    if (string(expr.__rtti) == "ExprReturn") { ... }    // STYLE006

    // Good — is operator
    if (expr is ExprReturn) { ... }

STYLE010 — ``if (true)`` should be a bare block
=================================================

``if (true)`` is always taken and adds unnecessary noise. Use a bare
block (lexical scope) instead.

.. code-block:: das

    // Bad — always true
    if (true) {                                 // STYLE010
        print("always\n")
    }

    // Good — bare block
    {
        print("always\n")
    }

STYLE011 — variable declaration followed by immediate assignment
=================================================================

A ``var`` declaration with no initializer immediately followed by an
assignment to that variable should be combined into a single declaration
with initialization.

The rule excludes ``var inscope`` (needs separate declaration for cleanup
semantics), compiler-generated variables, and generic instantiations.

.. code-block:: das

    // Bad — split declaration and init
    var x : int
    x = 5                                       // STYLE011

    // Good — combined
    var x = 5

    // Bad — clone on next line
    var s : string
    s := src                                    // STYLE011

    // Good — combined
    var s := src

STYLE012 — ``array<T>`` initialized by a run of ``push``/``emplace``
====================================================================

Declaring an empty ``array<T>`` variable immediately followed by two or
more contiguous ``push`` or ``emplace`` calls that all target the same
variable is a common "I forgot how to initialize an array" pattern. Use
an array literal instead — it is shorter, faster (one allocation with
the right capacity instead of repeated grows), and makes the initial
contents obvious at a glance.

A single ``push`` right after the declaration is **not** flagged,
because the verbose form is often the most readable option for a single
element. ``push_clone`` is deliberately excluded — there is no clean
array-literal equivalent.

The rule excludes ``var inscope``, compiler-generated variables, and
generic instantiations (same exclusions as STYLE011).

.. code-block:: das

    // Bad — two pushes after empty array declaration
    var a : array<int>
    a |> push(1)                                // STYLE012
    a |> push(2)

    // Good — inferred element type
    var a <- [1, 2]

    // Good — typed constructor, useful for polymorphic upcasts or
    // interface pointers where array literal type inference picks the
    // first element's type
    var shapes <- array<Shape?>(new Circle(3.0),
                                new Rectangle(2.0, 5.0),
                                new Circle(1.0))

    // Good — conditional / loop pushes are not flagged
    var b : array<int>
    for (i in range(10)) {
        b |> push(i)
    }

STYLE014 — comment block exceeds 3 lines at module/public scope
================================================================

.. note::

   This check is **opt-in**. Enable it by adding ``options _comment_hygiene = true``
   at the top of your file, or pass ``--comment-hygiene`` to the standalone utility.

A contiguous run of more than three ``//`` or ``//!`` comment lines at
module scope or above a public symbol is flagged as
multi-paragraph prose. The convention is "no architectural prose at the
head of a section" — long-form design notes belong in design docs
(``.md``), not in source.

The block before the file's first AST decl (the module-leading
docstring, e.g. ``daslib/regex_boost.das`` lines 9–18) is always
allowed. Suppress an individual block on its first line:

.. code-block:: das

    // Bad — 5 contiguous //! lines on a public function
    //! First sentence.                          // STYLE014
    //! Second.
    //! Third.
    //! Fourth.
    //! Fifth.
    def foo() { ... }

    //!@nolint
    //! First sentence — kept verbose intentionally.   // suppressed
    //! Second.
    //! Third.
    //! Fourth.
    def bar() { ... }

``daslib/rst_comment`` recognises the ``//!@nolint`` first line and
strips **only that marker line** from the emitted doc, so the rest of
the ``//!`` block still appears in
``doc/source/stdlib/generated/detail/*.rst``. The marker's only job is
to suppress the lint — the rest of the block stays visible. For a
``//`` block (no doc-comment), put ``// nolint:STYLE014`` on the first
line; those blocks never reach the doc generator.

STYLE015 — comment block exceeds 1 line inside a ``def private``
================================================================

.. note::

   This check is **opt-in**. Enable it by adding ``options _comment_hygiene = true``
   at the top of your file, or pass ``--comment-hygiene`` to the standalone utility.

Private symbols don't surface in any doc generator, so multi-line
comment prose inside a ``def private`` body is dead weight. Trim to one
line, or suppress with ``// nolint:STYLE015`` on the first line of the
block.

.. code-block:: das

    def private bad() {
        // First line — explanation                // STYLE015
        // Second line — fires (>1 line in private)
        ...
    }

    def private good() {
        // single WHY line — silent
        ...
    }

-----
Tests
-----

Lint tests are in ``utils/lint/tests/``::

    bin/Release/daslang.exe dastest/dastest.das -- --test utils/lint/tests

.. seealso::

    ``daslib/lint.das`` (paranoid lint source),
    ``daslib/perf_lint.das`` (performance lint source),
    ``daslib/style_lint.das`` (style lint source),
    ``utils/lint/main.das`` (unified standalone utility)
