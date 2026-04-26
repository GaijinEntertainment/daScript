options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

// Companion macro module for test_07_sql_composability.das.
//
// Defines two user-level wrapper call_macros that expand to `_where(it, ...)`.
// Their purpose is to prove the chunk-2 composability story: under Mode 2
// (`canVisitArgument=true`, the daslang default), inner call_macros expand
// before the outer's `visit` fires. So when `_sql(... |> when_price_lt(...))`
// runs, the macro pass first turns `when_price_lt` into `_where`, then turns
// `_where` into `where_(...,$(_:T) => ...)`, then `_sql` sees the same
// `where_` shape its analyzer recognizes — no special-casing required.

module test_07_macros shared private

require daslib/ast
require daslib/ast_boost
require daslib/macro_boost
require daslib/templates_boost

require daslib/linq_boost public
require sqlite/sqlite_linq public

// ──────────────────────────────────────────────────────────────────────────
// Baked-field form: when_price_lt(it, value) → _where(it, _.Price < value)
//
// Field name is hardcoded in the macro. This is the simpler composability
// case — only one free `_.Field` reference embedded inside the qmacro.
// ──────────────────────────────────────────────────────────────────────────

[call_macro(name="when_price_lt")]
class WhenPriceLtMacro : AstCallMacro {
    //! `when_price_lt(it, value)` expands to `_where(it, _.Price < value)`.
    //! `_where` is itself a call_macro, so the macro pass cascades.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 2, prog, call.at, "expecting when_price_lt(iterator, value)")
        var it = call.arguments[0]
        var val = call.arguments[1]
        var res = qmacro(_where($e(it), _.Price < $e(val)))
        res.force_at(call.at)
        return res
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Generic form: when_lt(it, _.Field, value) → _where(it, _.Field < value)
//
// Field reference is passed in by the caller as the second argument. Tests
// that a free-`_` field-ref expression survives transit through an outer
// call_macro's argument list before the inner `_where` lambda binds it.
// ──────────────────────────────────────────────────────────────────────────

[call_macro(name="when_lt")]
class WhenLtMacro : AstCallMacro {
    //! `when_lt(it, fieldExpr, value)` expands to `_where(it, fieldExpr < value)`.
    //! `fieldExpr` typically has shape `_.Field`; the unbound `_` resolves once
    //! `_where` wraps the predicate in a `$(_:T) => ...` lambda.
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 3, prog, call.at, "expecting when_lt(iterator, field, value)")
        var it = call.arguments[0]
        var fld = call.arguments[1]
        var val = call.arguments[2]
        var res = qmacro(_where($e(it), $e(fld) < $e(val)))
        res.force_at(call.at)
        return res
    }
}
