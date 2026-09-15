.. _fold_rules:

==========================================================================
Declarative fold rules — ``daslib/fold_rules``
==========================================================================

``daslib/fold_rules`` is a peephole optimizer whose rules are written as
daslang *source patterns* rather than as compiler code. A rule module
subclasses ``FoldRules``, annotates the class ``[fold_rules]``, and writes one
``qmatch`` ladder arm per rewrite; the arm body returns the replacement
expression, and ``null`` means "leave this node alone".

.. code-block:: das

    options gen2

    module game_folds shared

    require daslib/fold_rules public

    [fold_rules]
    class GameFolds : FoldRules {
        def override fold(var expr : ExpressionPtr) : ExpressionPtr {
            qmatch(expr) {
                if (pow($e(x), 2.0)) {
                    if (is_pure(x)) return qmacro($e(x) * $e(x))
                }
                if (lerp($e(a), $e(b), 0.0)) {
                    if (is_pure(b)) return a
                }
            }
            return null
        }
    }

Requiring that module from a program arms the rules for it. Nothing else is
needed: the annotation registers the class as an ``[optimization_macro]``.

Where the engine runs
---------------------

The engine is an optimization pass macro, so it sees each function **after**
the built-in optimizer passes of the same round — constants propagated, calls
inlined, references folded, common subexpressions eliminated. That is the point:
a rule like ``pow(x, 2.0) -> x * x`` only matches once ``2.0`` has actually
reached the call site.

There is no re-inference at that phase, so the engine types every replacement
itself. The grammar it can type is:

* constants,
* unary, binary and ternary operators,
* calls to pure builtins (``sqrt``, ``abs``, ``min`` ...),
* any subtree captured from the matched expression, which is already typed.

A replacement outside that grammar is a compile error naming the rule's input,
its output and the reason — never a partially typed node spliced into the tree.
A replacement whose type differs from the node it replaces is refused the same
way.

Each rewrite is retried against the whole rule set, so a chain of rewrites
settles before the walk moves on, and a function the rules changed is marked so
the built-in passes revisit it in the next round.

Guards live in the pattern
--------------------------

Every condition a rule depends on is a **predicate pattern**, not a test in the body: a call whose
name starts with ``_`` names a boolean function the node must satisfy. A rule whose guard does not
hold simply does not match, and the next rule is tried.

.. code-block:: das

    if (_droppable(_value_safe($e(x))) * 0)      { return zero_like(expr) }
    if (_droppable(_not_float($e(x))) == _same(x))  { return qmacro(true) }
    if (_signed($e(x)) * -1)                     { return qmacro(-$e(x)) }

A literal needs no wrapper: a bare number in a pattern matches that value in any numeric type, so
``* 1`` covers ``int``, ``float``, ``double`` and a splat whose every lane is one.

The predicates the engine ships:

``_droppable(P)``
    safe to evaluate ZERO times - nothing observable happens in it, so a rule may drop it. A call
    qualifies when the compiler proved the callee side-effect free.

``_repeatable(P)``
    safe to evaluate MORE than once: droppable, and under a node budget, because duplicating a
    large pure expression is a pessimization. ``options _fold_repeat_budget = N`` sets the budget.

``_const(P)``, ``_not_const(P)``
    is or is not a constant.

``_same(name)``
    the same droppable expression as one the pattern already captured.

``_int(P)``, ``_signed(P)``, ``_float(P)``, ``_not_float(P)``, ``_bool_typed(P)``
    base-type families.

``_value_safe(P)``, ``_zero_sign_safe(P)``
    the float laxity gates, as predicates: a rewrite that changes an infinity or NaN result, and one
    whose only effect is the sign of a zero.

A predicate is an ordinary das function, so a rule module adds its own by writing one whose name
starts with ``_``. No registration.

Writing rules that terminate
----------------------------

The rule set must be **monotonic**: a rule should eliminate work or move toward
a canonical form, never undo another rule. A pair that rewrites ``a + b`` to
``b + a`` and back never settles; the engine reports that as
``fold rules do not converge`` after sixteen rewrites of one node, rather than
looping. A canonicalization — the standard catalog moves constants to the right
of commutative operators — must therefore be one-directional: it fires only when
the left operand is the constant one.

Float and integer laxity
------------------------

Integer identities are exact. Float ones are not, and the helpers say which
gate a rule needs:

``_zero_sign_safe`` is true for ``float`` and, only under ``fast_math``, for ``double``;
``_value_safe`` is true for integers and, under ``fast_math``, for floats. Self-comparison folds
only behind ``_not_float``, because a NaN is not equal to itself.

For the body of a rule, ``zero_like(e)`` and ``typed_const(k, like)`` build a constant carrying the
type and location of the expression it replaces.

The standard catalog
--------------------

``daslib/fold_rules_std`` carries the algebraic identities the compiler's own
constant folder knows — ``x * 1``, ``x + 0``, ``x - x``, ``x ^ x``, ``!!x``,
``c ? a : a`` and the rest — plus idioms it does not: ``pow(x, 2)`` into a
multiply, ``pow(x, 0.5)`` into a square root, ``abs(abs(x))``, ``abs(-x)``,
``min(a, a)``, and self-comparisons on integers.

Requiring it arms those rules for the requiring module. In a normal build the
built-in arms run first each round and the declarative ones pick up what is
left, which is every idiom row. Setting
``options disable_algebraic_folding = true`` stands the built-in identity arms
down — constant *evaluation* is unaffected — and hands the identities to the
rules; that is how the catalog is tested against the compiler for parity.

Diagnostics
-----------

``options _fold_rules_log = true``
    log every function a rule changed, with the rewrite count per round.

``options _fold_rules = false``
    turn every rule off for this module.

``daslang --ast-verify``
    check the rewritten tree structurally, at the node where a break happened.
