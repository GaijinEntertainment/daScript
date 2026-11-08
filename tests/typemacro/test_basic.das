// Stage 0 coverage for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Direct tests for the typeMacro payload surface that Stage 1b migrated to
// TypeDecl.typeMacroExpr: all four grammar forms, const-argument extraction
// (int/bool/string), type arguments, and typedecl(expr).
// Deep indirect coverage lives in tests/option + tests/hash_map + tests/delegate
// (typemacro_boost-based); this file covers the raw forms those don't touch.
options gen2
require dastest/testing_boost public
require _typemacro_mod

[test(description = "a type macro is usable in the name(args), $name(args), name<types>(args) and $name<types>(args) forms")]
def test_grammar_forms(t : T?) {
    t |> run("name(args) form") @(t : T?) {
        var a : tm_make(type<float>, 4, true, "arr")
        t |> equal(typeinfo typename(a), "float[4]")
        a[3] = 1.5
        t |> equal(a[3], 1.5)
    }
    t |> run("$name(args) form") @(t : T?) {
        var _a : $tm_make(type<float>, 4, true, "arr")
        t |> equal(typeinfo typename(_a), "float[4]")
    }
    t |> run("name<types>(args) form") @(t : T?) {
        var _a : tm_make<float>(4, true, "arr")
        t |> equal(typeinfo typename(_a), "float[4]")
    }
    t |> run("$name<types>(args) form") @(t : T?) {
        var _a : $tm_make<float>(4, true, "arr")
        t |> equal(typeinfo typename(_a), "float[4]")
    }
}

[test(description = "a type macro takes bool and int constant arguments that change the shape and size of the result and composes with structs")]
def test_const_arguments(t : T?) {
    t |> run("bool argument switches result shape") @(t : T?) {
        var _plain : tm_make(type<int>, 1, false, "plain")
        t |> equal(typeinfo typename(_plain), "int")
        var _wrapped : tm_make(type<int>, 7, true, "arr")
        t |> equal(typeinfo typename(_wrapped), "int[7]")
    }
    t |> run("int argument carries the size") @(t : T?) {
        var _a : tm_make(type<int8>, 13, true, "arr")
        t |> equal(typeinfo dim(_a), 13)
        t |> equal(typeinfo sizeof(_a), 13)
    }
    t |> run("macro composes with structs") @(t : T?) {
        var a : tm_make(type<tuple<x : int; y : int>>, 2, true, "arr")
        a[1].x = 5
        t |> equal(a[1].x, 5)
        t |> equal(a[0].x, 0)
    }
}

[test(description = "typedecl(expr) resolves to the type of the expression")]
def test_typedecl_payload(t : T?) {
    t |> run("typedecl(expr) resolves to expression type") @(t : T?) {
        var _x = 5
        var y : typedecl(_x) = 9   // nolint:LINT003 - let would make typename read 'int const'
        t |> equal(typeinfo typename(y), "int")
        t |> equal(y, 9)
        var _f = 1.5
        var _g : typedecl(_f)
        t |> equal(typeinfo typename(_g), "float")
    }
}

[test(description = "shift and rotate operators still parse after a name<types> type macro form")]
def test_shift_after_type_list(t : T?) {
    // '>>' after a closed type list is a shift, not two '>'
    t |> run("shift and rotate after name<types> forms") @(t : T?) {
        var _a : tm_make<float>(4, true, "arr")
        var _b : $tm_make<int>(2, true, "arr")
        let x = 0x100u
        t |> equal(x >> 4u, 0x10u)
        t |> equal(x >>> 4u, 0x10u)
        var y = x
        y >>= 8u
        t |> equal(y, 1u)
    }
}
