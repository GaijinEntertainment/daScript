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

---------------------------
Repo-level ``.lint_config``
---------------------------

A ``.lint_config`` file at ``{get_das_root()}/.lint_config`` toggles
individual rules repository-wide. The three lint pass-macros
(``daslib/lint``, ``daslib/perf_lint``, ``daslib/style_lint``), the
standalone runner (``utils/lint/main.das``), and the MCP ``lint`` tool
all consult the same file.

The file is TOML 1.0, parsed by ``daslib/toml``. Toggles live in a
single ``[rules]`` table; each entry sets a rule to ``true`` (on) or
``false`` (off)::

    # Re-enable a default-off rule
    [rules]
    STYLE005 = true

    # Disable a default-on rule
    PERF007  = false

Defaults (applied before the file is read):

- **STYLE005** is **off** by default. Add ``STYLE005 = true`` to a
  repo's ``.lint_config`` to opt back in.
- All other rules are on by default.

The file is optional. When missing, unreadable, or syntactically
malformed the defaults stand — bad TOML is a silent no-op, not a
compile error. Non-boolean entries under ``[rules]`` are ignored.

CLI ``--enable`` on the standalone runner bypasses the defaults (the
explicit whitelist wins), so ``daslang utils/lint/main.das -- --enable
STYLE005 file.das`` always fires STYLE005 regardless of ``.lint_config``.

The ``*_collect()`` APIs (``paranoid_collect``, ``perf_lint_collect``,
``style_lint_collect``) do **not** read the file — callers pass
``disabled_codes`` / ``enabled_codes`` tables explicitly. Tools that
want to honor the repo policy should call
``daslib/lint_config::seed_default_disabled`` and ``load_lint_config``
before invoking the collect overload.

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

LINT010 — dead store
=====================

A local variable is written but the value never reaches a read — either it is
overwritten by a later write with no intervening read, or it goes out of scope
(``return`` / end-of-block) with no subsequent read. Two forms fire:

- ``overwritten without intervening read`` — the next write happens before any
  reader sees this one.
- ``value written but never read before scope exit`` — the scope ends and no
  subsequent code reads this store.

The variable being read elsewhere keeps LINT002 (unused variable) silent —
this rule is for *partial* deadness within an otherwise-used local.

.. code-block:: das

    // Bad — re-init before any read
    def f() : int {
        var x = 1                               // LINT010
        x = 2
        return x
    }

    // Bad — written before return, never read
    def g(z : int) : int {
        var t = z
        print("{t}\n")
        t = compute()                           // LINT010 — scope ends, no read
        return z
    }

    // Bad — lambda captures by-copy at creation, later write never observed
    def h() : int {
        var x = 1
        let f = @() => x + 1
        x = 2                                   // LINT010 — lambda has its own copy
        return invoke(f)
    }

    // Good
    def g_fixed(z : int) : int {
        var t = z
        print("{t}\n")
        return z * 2
    }

The rule's scope is intentionally narrow for the first pass: it only flags
pure stores (LHS of ``=`` / ``<-`` / ``:=``) on function-local variables, only
when the dead store's RHS has ``noSideEffects``, and only in straight-line
basic blocks — control flow (``if`` / ``for`` / ``while`` / ``try``) and
block-argument callbacks (``tab |> get(k) $(v) { … }``) cause the analysis to
bail on the variable to avoid false positives. Bail signals also include
address-of (``addr(x)``), reference bindings (``var r & = x``), mutable-ref
parameter passing (``foo(x)`` where ``foo`` takes ``var T&``), and capture-by-
reference. Suppress structurally-needed dead inits with ``// nolint:LINT010``.

LINT011 — int literal promoted with precision loss
===================================================

When a bare integer literal flows into a ``float`` or ``double`` target via
the implicit promotion described in
:ref:`type_conversions`, the cast can silently lose
precision: ``float`` exactly represents every integer in ``[-2^24, 2^24]``,
but above that range only every other integer (and at higher magnitudes,
only every fourth, eighth, …) is representable. LINT011 flags promotions
where the integer literal does not survive ``int → float → int`` round-trip
at compile time. The check is decided at the promotion site, so the lint
sees a single bit per ``ExprConstFloat`` / ``ExprConstDouble`` and never
has to redo the math.

``double`` covers integers up to ``2^53`` exactly, and the current promotion
sources cap at ``uint32`` (``2^32 - 1``). LINT011 therefore never fires on
``double`` targets today — the rule is wired symmetrically so future broader
sources stay covered.

.. code-block:: das

    // Bad — float can't represent 2^24 + 1 exactly
    let inexact : float = 16777217          // LINT011

    // Good — 2^24 itself IS exactly representable
    let exact : float = 16777216

    // Suppress per call site
    let intentional : float = 16777219      // nolint:LINT011

    // double is fine for the current promotion sources
    let big : double = 1000000000           // no warning

Suppress with ``// nolint:LINT011`` on the offending line when the inexact
value is intentional (a sentinel, a sampled constant, etc.).

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

PERF013 — ``a += 1`` / ``a -= 1`` should be ``a++`` / ``a--``
=============================================================

``a += 1`` lowers to a 2-node read-modify-write in interpreted mode. The
postfix ``++`` / ``--`` collapses to a single ``SimNode_op1`` and reads as
the canonical inc/dec idiom. Applies to the six numeric workhorse scalars
(``int``, ``uint``, ``int64``, ``uint64``, ``float``, ``double``); vectors
(``int2``, ``float3``, …) do **not** support ``++``/``--`` so they are
skipped. ``+= -1`` is also flagged (same effect as ``-= 1``).

.. code-block:: das

    // Bad
    a += 1                                      // PERF013
    a -= 1                                      // PERF013
    a += -1                                     // PERF013

    // Good
    a ++
    a --

PERF014 — closed-interval char-class range check
==================================================

Hand-rolled ranges like ``c >= 'a' && c <= 'z'`` reimplement
``strings::is_alpha``/``is_alnum``/``is_number``/``is_white_space``/etc.
The helper functions read clearer and centralise locale/codepoint
behaviour. Only three closed ranges are flagged:

* ``'0'..'9'`` (48..57) — ``is_number``
* ``'a'..'z'`` (97..122) — ``is_alpha`` lower half
* ``'A'..'Z'`` (65..90) — ``is_alpha`` upper half

The hex extras ``'a'..'f'`` / ``'A'..'F'`` are deliberately **not**
flagged — ``is_hex`` is broader. Open intervals (``c > '0' && c < '9'``)
have different endpoints, so they are also skipped.

.. code-block:: das

    // Bad
    if (c >= 'a' && c <= 'z') { ... }           // PERF014
    if (c >= 48  && c <= 57)  { ... }           // PERF014 (raw int form)

    // Good
    if (is_alpha(c))  { ... }
    if (is_number(c)) { ... }

PERF015 — ternary min / max
============================

``a < b ? a : b`` reimplements ``min(a, b)``. The math builtins are
vec-friendly and the intent is clearer. All eight orientations of
``< / <= / > / >=`` × ``T==L,F==R`` / ``T==R,F==L`` are flagged.

.. code-block:: das

    // Bad
    let smaller = a < b ? a : b                 // PERF015 — min
    let larger  = a > b ? a : b                 // PERF015 — max

    // Good
    let smaller = min(a, b)
    let larger  = max(a, b)

PERF016 — ternary abs
======================

``x < 0 ? -x : x`` reimplements ``abs(x)``. ``abs`` exists for every
signed numeric type. Only the four orientations that match ``abs`` are
flagged; the negabs shape (``x < 0 ? x : -x``) is **not** — it is a
different function.

.. code-block:: das

    // Bad
    let positive = x < 0 ? -x : x               // PERF016
    let positive_alt = x > 0 ? x : -x           // PERF016

    // Good
    let positive = abs(x)

PERF017 — ``length(s) == 0`` should be ``empty(s)``
====================================================

For strings, ``length`` walks the whole string (``strlen``); ``empty``
checks one byte. For arrays/tables both are O(1) but ``empty`` is the
idiomatic form. Six comparison ops are mapped to either ``empty(x)`` or
``!empty(x)``:

* ``length(x) == 0``, ``length(x) <= 0``, ``length(x) < 1`` → ``empty(x)``
* ``length(x) != 0``, ``length(x) > 0``,  ``length(x) >= 1`` → ``!empty(x)``

Vector magnitude (``length(float3_var)`` from the math module) is **not**
flagged — different semantics, no ``empty`` for vectors.

.. code-block:: das

    // Bad
    if (length(s)   == 0) { ... }               // PERF017
    if (length(arr) >  0) { ... }               // PERF017

    // Good
    if (empty(s))   { ... }
    if (!empty(arr)) { ... }

PERF018 — ``for (i in range(length(arr)))`` should iterate directly
=====================================================================

When the loop variable ``i`` is used only as ``arr[i]`` against the same
array, the index is pure overhead — iterate the array directly.

Detection peels at most one ``ExprCast`` between ``range`` and ``length``
(to allow ``range(uint(length(arr)))``) and resolves both the loop's
target and the indexed receiver via the existing ``find_expr_path`` chain
walker. Every use of ``i`` in the body must be the bare index of
``arr[i]`` against the same path; any arithmetic on ``i``
(``arr[i+1]`` / sliding window), use of ``i`` outside an indexing
expression, or indexing a different array disqualifies the loop.

.. code-block:: das

    // Bad — i used only as arr[i]
    for (i in range(length(arr))) {             // PERF018
        process(arr[i])
    }

    // Good — direct iteration
    for (c in arr) {
        process(c)
    }

PERF019 — ``int(T.a) | int(T.b)`` on bitfield/enum — collapse to one cast
==========================================================================

When ``T`` is a type whose values support ``|`` directly — bitfields
always do; enums when an ``operator |(T, T) : T`` overload exists —
``int(T.a) | int(T.b)`` does two casts where one suffices. Write
``int(T.a | T.b)`` instead: the OR happens on the typed value, then a
single cast lowers to ``int``.

The detection walks both operands of an ``|`` ``ExprOp2``, peels one
layer of ``int(...)`` (matched as an ``ExprCall`` whose
``func.name`` / ``fromGeneric.name`` is ``"int"``), and fires when both
inner types are the same bitfield, or the same enum that has an
``operator |`` defined somewhere in the compiling program (probed once
per enum type via ``program_for_each_module`` + ``for_each_function``,
result cached on the visitor).

Note on testing: the "canonical" form with two compile-time constants
(``int(Mode.read) | int(Mode.write)``) is folded by the optimizer to a
single ``ExprConstInt`` under normal compile policies, so dastest can't
observe the rule firing on it. The lint runner sets
``no_optimizations = true`` and ``no_infer_time_folding = true`` on
``CodeOfPolicies``, which preserves the AST and lets the rule fire.
Dastest coverage in ``utils/lint/tests/perf019_int_cast_collapse.das``
uses runtime operands; the constant case is covered by the CI lint gate.

.. code-block:: das

    bitfield Mode { read; write; exec }

    // Bad
    var mask = int(Mode.read) | int(Mode.write)         // PERF019

    def f(m1, m2 : Mode) : int {
        return int(m1) | int(m2)                        // PERF019
    }

    // Good
    var mask = int(Mode.read | Mode.write)

    def f(m1, m2 : Mode) : int {
        return int(m1 | m2)
    }

PERF020 — redundant same-type cast
====================================

``T(x)`` where ``x`` is already of workhorse type ``T`` is a no-op. The
cast produces a real ``ExprCall`` node — the parser/typer does NOT elide
it — so it costs source noise and one call dispatch for zero work.

Fires for the 15 workhorse cast names: ``int``, ``int8``, ``int16``,
``int64``, ``uint``, ``uint8``, ``uint16``, ``uint64``, ``float``,
``double``, ``string``, ``bitfield``, ``bitfield8``, ``bitfield16``,
``bitfield64``. The match is on the call's ``func.name`` /
``fromGeneric.name`` (so generic instantiations of the cast still
trigger) combined with a strict ``arg._type.baseType`` equality check
against the cast's target type. Const / reference / temporary qualifiers
on the argument are ignored — only ``baseType`` matters.

.. code-block:: das

    // Bad — a is already int64
    def widen(a : int64) : int64 {
        return int64(a)                                     // PERF020
    }

    def field_read(s : SomeStruct) : int64 {
        return int64(s.value)                               // PERF020 — s.value is int64
    }

    // Good
    def widen(a : int64) : int64 {
        return a
    }

The rule deliberately does NOT cover:

- User-named bitfield/enum constructors (``MyBitfield(x)``, ``MyEnum(x)``).
  These are parser-synthesized constructors named after the user type, not
  the bare workhorse name in the table above; ``MyBitfield(modeVar)``
  does not match.
- Vector constructors (``int2``, ``float3``, …). They primarily take
  componentwise arguments and are excluded by the single-argument gate.
- ``string(das_string)`` — covered by PERF007 (in comparisons) and
  PERF012 (passed to ``strings`` functions). ``das_string`` has a
  distinct ``baseType``, so it does not collide with this rule.

Cross-type casts (widening, narrowing, signedness change, float ↔ int)
are genuine work and do NOT fire.

PERF021 — hoist common workhorse cast out of ternary
======================================================

``cond ? T(a) : T(b)`` where both branches apply the **same** workhorse
cast ``T`` emits two ``ExprCall`` nodes that do identical work regardless
of which branch is taken. Hoisting the cast outside the ternary collapses
them to one: ``T(cond ? a : b)``.

Uses the same 15-name workhorse cast set as PERF020. The rule fires only
when:

- Both ternary branches are calls to the same workhorse cast name (after
  peeling ``ExprRef2Value``).
- Both calls share the same target ``Type``.
- Both arguments share the same ``baseType`` — so the hoisted
  ``T(cond ? a : b)`` typechecks without an intermediate cast.

If the argument base types differ (e.g. ``cond ? string(intV) :
string(int64V)``), the rule does NOT fire; the rewrite would need a
manual widen on one branch and that is left to the author.

.. code-block:: das

    // Bad
    def to_str(c : bool; a, b : int) : string {
        return c ? string(a) : string(b)                    // PERF021
    }

    def widen(c : bool; a, b : int) : int64 {
        return c ? int64(a) : int64(b)                      // PERF021
    }

    // Good
    def to_str(c : bool; a, b : int) : string {
        return string(c ? a : b)
    }

    def widen(c : bool; a, b : int) : int64 {
        return int64(c ? a : b)
    }

The rewrite is unconditionally safe: the original ternary evaluates
exactly one of ``a`` / ``b``, and so does the hoisted form — argument
evaluation count is unchanged. Only the per-branch cast dispatch is
eliminated.

User-named struct / enum / bitfield constructors (``MyEnum(x)``,
``Foo(v=x)``) and multi-argument vector constructors (``float2(x, y)``)
do not match the workhorse cast set and are intentionally out of scope.

PERF022 — for-loop pushing one element per iteration
=====================================================

A loop body that consists of exactly one ``push(s)`` or ``push_clone(s)``
of the for-loop iteration variable into a destination array is the
element-at-a-time form of an array concatenation. The bulk overloads
``push_from`` / ``push_clone_from`` (in ``daslib/builtin.das``) reserve the
combined capacity up front and skip the per-iteration capacity check.

The rule fires for the iteration-variable shape only — a transform,
if-guard, multi-statement body, or multi-source ``for`` does not match,
because those have no direct bulk equivalent.

Compiler folds ``B |> push(s)``, ``B.push(s)``, and ``push(B, s)`` to the
same call shape, so all three forms are detected by the same rule.

.. code-block:: das

    // Bad
    for (s in src) {                            // PERF022
        dst |> push(s)
    }

    // Good
    dst |> push_from(src)

The bulk forms expect the destination to be ``array<T>`` and the source to
be ``array<T>`` or a fixed-size C-array ``T[N]``. Range, string, iterator,
and generator sources do not have a bulk overload and are left unflagged.

The same recommendation applies to ``push_clone``:

.. code-block:: das

    // Bad
    for (s in src) {                            // PERF022
        dst |> push_clone(s)
    }

    // Good
    dst |> push_clone_from(src)

``emplace`` is not in the rule's scope: a for-loop iteration variable is a
const reference, but ``emplace`` requires a mutable reference, so the
hand-rolled shape ``for (s in src) { dst |> emplace(s) }`` does not
compile. The ``emplace_from`` bulk overload still exists in
``daslib/builtin.das`` for direct calls with a mutable source array.

PERF023 — redundant ``clone_expression`` before qmacro splice
===============================================================

``qmacro``, ``qmacro_block``, ``qmacro_expr``, and ``qmacro_block_to_array``
all go through ``apply_template`` (``daslib/templates_boost.das``), which
calls ``clone_expression`` on every ``$e(...)`` substitution input. Pre-cloning
into a local variable and then splicing the local is wasted work — the same
substitution gets cloned a second time at apply-template time.

.. code-block:: das

    // Bad
    var defaultExpr = clone_expression(terminatorCall.arguments[1])     // PERF023
    preludeStmts |> push <| qmacro_expr() {
        let $i(defaultName) = $e(defaultExpr)
    }

    // Good
    preludeStmts |> push <| qmacro_expr() {
        let $i(defaultName) = $e(terminatorCall.arguments[1])
    }

The rule fires only when **every** use of the candidate variable lives inside
a ``$e(...)`` splice tag — any other use (assignment, passing to a non-splice
consumer, storing into a struct field) means the pre-clone is load-bearing
and the lint stays silent.

**Multi-clone cases.** When the same source feeds N ``$e(...)`` slots in one
qmacro body, the rule still flags every pre-clone. ``apply_template`` clones
each substitution independently, so ``$e(E)`` repeated N times yields N
independent clones — equivalent to one user-side clone repeated N times via
``$e(X)``:

.. code-block:: das

    // Bad — three pre-clones for three splice slots
    var takeA = clone_expression(takeExpr)                              // PERF023
    var takeB = clone_expression(takeExpr)                              // PERF023
    var takeC = clone_expression(takeExpr)                              // PERF023
    body = qmacro_block() {
        let $i(takeNName) = $e(takeA) <= 0 ? 0 : ($e(takeB) < $i(lenName) ? $e(takeC) : $i(lenName))
    }

    // Good — inline takeExpr at each splice; apply_template clones each one
    body = qmacro_block() {
        let $i(takeNName) = $e(takeExpr) <= 0 ? 0 : ($e(takeExpr) < $i(lenName) ? $e(takeExpr) : $i(lenName))
    }

For sources with side effects (rare in AST-building code), bind once via plain
``let baseE = E`` (no clone) and splice the local — that preserves single-eval
semantics while still letting ``apply_template`` produce N clones.

``clone_type`` is out of scope. Types take a different path through
``apply_qrules`` (the ``$<TT>`` tag form emits ``add_type_ptr_ref``, but
``clone_type`` call sites typically feed direct AST construction, not qmacro
splices).

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

STYLE005 — braces around a single-statement early exit
========================================================

A single-statement braced ``if`` whose body is just a ``return`` / ``break``
/ ``continue`` is noise. Use either the braceless form ``if (cond) return X``
or postfix ``return X if (cond)``. Always-on (no opt-in flag).

The discriminator is AST-only: the parser shares ``LineInfo`` between a
synthesized block and its inner terminator for both braceless ``if (c)
return`` and postfix-desugared ``return X if (c)``, so a real
user-written ``{...}`` is detectable as ``blk.at != inner.at``.

.. code-block:: das

    // Bad — braces around a single terminator
    if (x > 0) {                                // STYLE005
        return x
    }
    return -x

    // Good — braceless
    if (x > 0) return x
    return -x

    // Good — postfix
    return x if (x > 0)
    return -x

The auto-fixer at ``utils/fix-lint-errors/`` rewrites STYLE005 hits to the
braceless form. Suppress per-line with ``// nolint:STYLE005``.

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

STYLE016 — adjacent guards leading to identical early-exit
============================================================

Two adjacent ``if`` guards with the same exit (``return`` with the same
payload, or ``break``/``continue``) read as one decision. Combine them
with ``||``. Two AST shapes are detected:

* two adjacent ``if (a) { return X }`` statements in the same block
* the ``if (a) { return X } else if (b) { return X }`` chain

.. code-block:: das

    // Bad
    if (name == "." || name == "..") {          // STYLE016
        return
    }
    if (name |> starts_with("_")) {
        return
    }

    // Good
    if (name == "." || name == ".." || name |> starts_with("_")) {
        return
    }

STYLE017 — ``if (cond) return true; else return false`` should be ``return cond``
==================================================================================

Three lines (or two if-else branches) that just propagate the boolean
condition unchanged. Read better as a single ``return``. Detection covers
both forms:

* ``if (cond) return b1 else return b2`` (b1 ≠ b2)
* ``if (cond) return b1`` immediately followed by ``return b2`` (b1 ≠ b2)

.. code-block:: das

    // Bad
    if (cond) {                                 // STYLE017
        return true
    } else {
        return false
    }

    // Good
    return cond

    // Good (negated)
    return !cond

STYLE018 — redundant boolean comparison
========================================

Comparing a bool to a boolean literal is redundant — the bool already IS
the value. Drop the comparison. Both Yoda forms (``true == flag``) are
detected.

.. code-block:: das

    // Bad
    if (flag == true)  { ... }                  // STYLE018
    if (flag != false) { ... }                  // STYLE018
    if (flag == false) { ... }                  // STYLE018
    if (true == flag)  { ... }                  // STYLE018 (Yoda)

    // Good
    if (flag)  { ... }
    if (!flag) { ... }

STYLE019 — nested ``min(max(...))`` should be ``clamp(...)``
==============================================================

``min(max(x, lo), hi)`` reads as a clamp; the math builtin says so
directly. Both orientations (and the mirror form) are detected — the
inner call must resolve to the math module's ``min`` / ``max``, not a
user overload.

.. code-block:: das

    // Bad
    let bounded = min(max(x, lo), hi)           // STYLE019
    let bounded_alt = max(min(x, hi), lo)       // STYLE019 (mirror)

    // Good
    let bounded = clamp(x, lo, hi)

STYLE020 — scalar ``from_JV`` should be ``v ?? defV``
=======================================================

``daslib/json_boost`` provides ``operator ??`` overloads for every
supported scalar ``JsonValue → primitive`` conversion (int, uint, int8/16/64,
uint8/16/64, float, double, bool, string). The three-arg
``from_JV(v, type<T>, defV)`` form is redundant for scalars — ``v ?? defV``
is one fewer call, reads better, and uses the operator that's already
there. Vector / table / struct / enum / bitfield overloads have no
matching ``??`` and stay silent.

Detection walks ``expr.func.fromGeneric`` to the root of the template-
instantiation chain (two levels deep for json_boost's
``[template(ent)]`` generics) and matches the root's name/module against
``from_JV`` / ``json_boost``. The result-type check uses ``expr._type``,
which is robust under both pre- and post-instantiation argument shapes.

.. code-block:: das

    // Bad
    let n = from_JV(jv, type<int>, 13)          // STYLE020
    let s = from_JV(jv, type<string>, "x")      // STYLE020

    // Good
    let n = jv ?? 13
    let s = jv ?? "x"

STYLE021 — repeated ``table<string; JsonValue?>`` inserts → named-tuple ``JV``
================================================================================

Building a JSON object by declaring an empty
``var args : table<string; JsonValue?>`` followed by ``args |> insert("k",
JV(v))`` calls is verbose. The named-tuple ``JV`` overload
(``daslib/json_boost.das:638``) builds the same object in one line.

Detection requires the variable's static type to be exactly
``table<string; JsonValue?>``, zero initial value, and a contiguous run
of ≥ 2 ``insert`` calls whose key is an ``ExprConstString`` and whose
receiver resolves to the same variable. Computed keys disqualify the
chain.

.. code-block:: das

    // Bad
    var args : table<string; JsonValue?>
    args |> insert("target", JV(target))        // STYLE021
    args |> insert("dx", JV(dx))
    args |> insert("dy", JV(dy))

    // Good
    var args = JV((target = target, dx = dx, dy = dy))

STYLE022 — bitfield ``|=`` / ``&= ~`` single bit → field assignment
=====================================================================

When ``foo`` is a bitfield value, ``foo |= BfT.m`` and ``foo &= ~BfT.m``
are the mask-arithmetic forms of setting and clearing one named bit.
daslang exposes the same operation as a bool field assignment:
``foo.m = true`` and ``foo.m = false``. The field form names the bit
instead of the mask and drops the ``~`` for clears.

The rule fires only when the right-hand side resolves to exactly one
named bit. Under lint policies the RHS appears as ``ExprField``
(``ExprField(value = ExprVar(BfT), name = "m")``); under normal compile
policies the optimizer folds it to ``ExprConstBitfield`` — the rule
covers both shapes and, for the folded case, maps the single-bit mask
back to the symbolic bit name via ``TypeDecl.argNames``. Multi-bit
masks (``Mode.read | Mode.write``) and dynamic RHS are left alone.

The ``&=`` form requires the source pattern ``foo &= ~BfT.m`` — either
an explicit ``ExprOp1("~", ExprField)`` or a single-bit-complement
``ExprConstBitfield``. A bare ``foo &= BfT.m`` (no ``~``) is *not* the
bit-clear idiom; it would mask off every other bit, so it stays silent.

.. code-block:: das

    bitfield Mode { read; write; exec }

    // Bad
    var f : Mode
    f |= Mode.read                          // STYLE022 → f.read = true
    f &= ~Mode.write                        // STYLE022 → f.write = false

    // Good
    var f : Mode
    f.read = true
    f.write = false

STYLE023 — ``int_cast(bf & BfT.m) != 0`` → ``bf.m``
=====================================================================

Testing a single bit via ``uint(bf & BfT.m) != 0u`` (or any of the
``int`` / ``uint`` / ``int64`` / ``uint64`` cast forms compared to
``0``) is the mask-arithmetic counterpart of the boolean field access
daslang exposes on bitfields. ``bf.m`` already evaluates to a ``bool`` —
read it directly. ``== 0`` becomes ``!bf.m``.

Detection matches both operand orders (``cast(...) !=/== 0`` and
``0 !=/== cast(...)``) and accepts all four standard int casts
(``int``, ``uint``, ``int64``, ``uint64``). The inner expression must be
``bitfield & SingleBit`` where ``SingleBit`` resolves to a named bit of
the same bitfield — under lint policies as ``ExprField(BfT, name)``,
under normal compile as ``ExprConstBitfield`` with a single-bit mask.
Multi-bit masks (``Mode.read | Mode.write``) are left alone since the
``!= 0`` semantics differ from any single field read.

.. code-block:: das

    bitfield Mode { read; write; exec }
    struct Io { flags : Mode }

    // Bad
    if (uint(io.flags & Mode.read) != 0u) { ... }       // STYLE023 → io.flags.read
    if (int(io.flags & Mode.write) == 0)  { ... }       // STYLE023 → !io.flags.write

    // Good
    if (io.flags.read)   { ... }
    if (!io.flags.write) { ... }

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
