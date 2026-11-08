options gen2
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ``match_call_in_linq(expr, name) : ExprCall?`` is a thin wrapper on
// ``match_call_in_module(expr, name, "linq")``.
//
// SCOPE NOTE: see test_match_call_in_module.das — positive matches require
// typed AST and are covered transitively by PR 1b/1c. These tests pin the
// structural / null-guard behavior callable without a typed AST.

[test(description = "match_call_in_linq returns null for a null expression")]
def test_null_does_not_match(t : T?) {
    var expr : Expression?
    t |> success(match_call_in_linq(expr, "_select") == null,
                 "null input must not match")
}

[test(description = "match_call_in_linq returns null for an expression that is not a call")]
def test_non_call_does_not_match(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        t |> success(match_call_in_linq(expr, "_select") == null,
                     "non-call must not match")
    }
}

[test(description = "match_call_in_linq returns null for a call whose resolved function is null")]
def test_call_with_null_func_does_not_match(t : T?) {
    ast_gc_guard() {
        var call : Expression? = new ExprCall(at = LineInfo(), name := "_select")
        t |> success(match_call_in_linq(call, "_select") == null,
                     "ExprCall with null .func must not match")
    }
}

// ``match_call_in_module(expr, name, modName) : ExprCall?`` returns the call
// when ``expr`` is a typed ``ExprCall`` whose ``func.name`` / ``func._module.name``
// (or the ``fromGeneric`` fallback) match ``(name, modName)``.
//
// SCOPE NOTE: The positive-match path requires a fully type-inferred ``ExprCall``,
// which is reachable from ``[call_macro]`` hooks running on already-typed parent
// trees (the production usage pattern in linq_fold and sqlite_linq). It is NOT
// reliably reachable from a unit test that constructs a macro arg directly,
// because daslang resolves macro args before the macro fires but a freshly-
// constructed ``length(_arr)`` arg may not have ``.func`` populated at that
// point. The helper is byte-identical to the production version it replaces;
// the positive path is exercised transitively by PR 1b (sqlite_linq migration)
// and PR 1c (linq_fold migration).
//
// These tests pin the structural / null-guard behavior — the parts callable
// without a typed AST.

// ── Null and non-call inputs ─────────────────────────────────────────────

[test(description = "match_call_in_module returns null for a null expression")]
def test_null_does_not_match_module(t : T?) {
    var expr : Expression?
    t |> success(match_call_in_module(expr, "length", "builtin") == null,
                 "null input must not match")
}

[test(description = "match_call_in_module returns null for an expression that is not a call")]
def test_non_call_does_not_match_module(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        t |> success(match_call_in_module(expr, "length", "builtin") == null,
                     "ExprConstInt must not match")
    }
}

[test(description = "match_call_in_module returns null for a call whose resolved function is null")]
def test_call_with_null_func_does_not_match_module(t : T?) {
    ast_gc_guard() {
        // Build an ExprCall by name only (no .func resolution). This is the
        // typer's pre-inference shape — match_call_in_module must return null.
        var call : Expression? = new ExprCall(at = LineInfo(), name := "length")
        t |> success(match_call_in_module(call, "length", "builtin") == null,
                     "ExprCall with null .func must not match")
    }
}
