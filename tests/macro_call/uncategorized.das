options gen2

require dastest/testing_boost public
require _mint_macro
require _dyn_register_decl
require _find_call_macro_helper
require _widget_dispatch_helper

// [mints] fails this file's compile unless every annotation its apply registers is found in the compiling module
[mints]
def minted_answer : int => 41

[test(description = "a call macro and annotations registered from a function annotation apply are found in the compiling module and the call macro folds its call")]
def test_a_call_macro_registered_from_a_function_annotation(t : T?) {
    t |> equal(minted_answer() + minted_answer_one(), 42)
}

// Regression: a [function_macro] that dynamically registers a call_macro
// from inside its apply() must dispatch to the new instance at every
// future call site of the registered name. dasImgui's [widget] relies on
// this; if it regresses, every dasImgui boost call silently falls through
// to the underlying function without prelude/postlude/state wrapping.

[test(description = "a call macro registered dynamically from a function macro apply folds every call of its name to the constant, ignoring the argument")]
def test_dyn_registered_call_macro_dispatches(t : T?) {
    t |> equal(answer(0), 42,
               "[dyn_register_42] should fold answer(N) -> 42 via the dynamically-registered call_macro")
    t |> equal(answer(123), 42,
               "argument value should be ignored (macro replaces the whole call)")
}

// Regression: find_call_macro(mod, "absent_name") used to null-deref in the
// C++ binding (Module::findCall returns nullptr on miss). It now returns null,
// making it a safe soft probe.

[test(description = "find_call_macro for an unregistered name returns null instead of crashing")]
def test_find_call_macro_missing_returns_null(t : T?) {
    t |> success(probe_find_call_macro_missing(),
                 "find_call_macro for an unregistered name should return null, not crash")
}

[widget]
def my_button(var state : StubState; text : string) : int {
    return state.value
}

[test(description = "a widget call macro intercepts the call, emits the missing module global and rewrites the call to the full signature")]
def test_widget_call_macro_fires(t : T?) {
    //! `my_button(BTN_A, "click me")` should be intercepted by the [widget]
    //! call_macro: it auto-emits BTN_A as a module global of type StubState,
    //! then rewrites the call to `my_button(BTN_A, "BTN_A", "click me")`,
    //! matching the macro-expanded function signature
    //! `def my_button(state, widget_ident, text)`.
    //!
    //! If the call_macro never fires, the typer sees a 2-arg call against
    //! a 3-arg function and errors with "no matching functions or generics".
    //!
    //! If the auto-emit never happens, BTN_A is unresolved and the typer
    //! errors with "can't locate variable 'BTN_A'".
    let v = my_button(BTN_A, (text = "click me"))
    t |> equal(v, 0, "auto-emitted BTN_A's StubState defaults to value=0")
}

[test(description = "a global auto-emitted by a call macro is visible to other expressions in the module and keeps mutations between calls")]
def test_widget_global_visible_outside_call(t : T?) {
    //! After `my_button(BTN_B, ...)` auto-emits BTN_B, a separate
    //! ExprVar(BTN_B) elsewhere in the module must resolve to that global.
    //! This is the cross-expression resolution path (gap #3 in the
    //! dasImgui plan) — guaranteed by reportAstChanged() in
    //! ast_infer_type.cpp:5220.
    let v = my_button(BTN_B, (text = "x"))
    t |> equal(v, 0, "fresh BTN_B reads as value=0")
    BTN_B.value = 42
    let v2 = my_button(BTN_B, (text = "x"))
    t |> equal(v2, 42, "second call sees the mutated module global")
}
