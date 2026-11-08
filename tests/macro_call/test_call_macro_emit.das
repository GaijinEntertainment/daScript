options gen2

require dastest/testing_boost public
require _call_macro_emit_gate_helper

// Pre-declared global. test_reuse_existing_global verifies that
// emit_or_reuse_int("_gate_b") detects this via find_variable and
// hands back the SAME global (read = 999), rather than emitting
// a duplicate that shadows it.
var _gate_b : int = 999

[test(description = "emit_or_reuse_int emits a new zero-initialized int global for a name that is not declared")]
def test_emit_when_absent(t : T?) {
    //! Fresh name (_gate_a not declared anywhere): macro emits a new
    //! int global with the default zero-init, ExprVar reads back 0.
    t |> equal(emit_or_reuse_int("_gate_a"), 0,
               "fresh name should be emitted as a new int global with default 0")
}

[test(description = "emit_or_reuse_int reuses an already declared global instead of overwriting or duplicating it")]
def test_reuse_existing_global(t : T?) {
    //! Pre-declared name (_gate_b = 999 above): macro detects the
    //! existing global via find_variable and re-uses it. If gating
    //! regresses and add_variable is called anyway, the typer sees
    //! a duplicate and either errors or shadows — either way this
    //! test fails loud.
    t |> equal(emit_or_reuse_int("_gate_b"), 999,
               "existing global must be reused, not overwritten or duplicated")
}

// Pre-declared global to anchor the right-hand side of the
// "fresh + existing" mix in test_mixed_fresh_and_existing.
var _cx_existing : int = 7

[test(description = "two call-macro emits of fresh globals in one expression both resolve")]
def test_two_fresh_in_one_expression(t : T?) {
    //! Two emit_or_reuse calls in ONE expression both emit fresh globals.
    //! Each macro returns ExprVar(name) for a global it just add_variable()'d.
    //! If the typer's reportAstChanged() invariant ever regresses, the
    //! second ExprVar fails to resolve in the same infer pass and this
    //! errors at compile time.
    t |> equal(emit_or_reuse_int("_cx_two_fresh_a") + emit_or_reuse_int("_cx_two_fresh_b"), 0,
               "two fresh globals emitted in same expression must both resolve, summing to 0+0")
}

[test(description = "a fresh emit and a reuse of a declared global in one expression both resolve")]
def test_mixed_fresh_and_existing(t : T?) {
    //! Mix: left side is a fresh emit (_cx_mixed = 0), right side is the
    //! pre-declared _cx_existing = 7. Confirms find_variable still sees
    //! pre-existing module globals after a sibling emit in the same
    //! expression node.
    t |> equal(emit_or_reuse_int("_cx_mixed") + emit_or_reuse_int("_cx_existing"), 7,
               "fresh+existing in same expression must resolve to 0+7")
}

[test(description = "emitting the same fresh name twice in one expression reuses the first global instead of duplicating it")]
def test_repeated_same_name_in_one_expression(t : T?) {
    //! Same fresh name twice in one expression: first call emits, second
    //! call must find_variable() the just-emitted global and reuse it
    //! (NOT add_variable() a duplicate). Duplicate would error at typer.
    t |> equal(emit_or_reuse_int("_cx_repeat") + emit_or_reuse_int("_cx_repeat"), 0,
               "repeated name in one expression must reuse, not duplicate")
}
