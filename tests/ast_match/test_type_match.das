options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── ExprTypeDecl matching ────────────────────────────────────────────────

[test(description = "a type<int> pattern matches a type<int> expression")]
def test_type_int(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        let r = qmatch(expr, type<int>)
        t |> success(r.matched, "type<int> should match")
    }
}

[test(description = "a type<float> pattern matches a type<float> expression")]
def test_type_float(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<float>)
        let r = qmatch(expr, type<float>)
        t |> success(r.matched, "type<float> should match")
    }
}

[test(description = "a type<float> pattern does not match type<int>")]
def test_type_wrong(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        let r = qmatch(expr, type<float>)
        t |> success(!r.matched, "type<int> should not match type<float>")
        t |> equal(int(r.error), int(QMatchError.type_mismatch))
    }
}

[test(description = "a type<int const?> pattern matches an identical const pointer type")]
def test_type_const_pointer(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int const?>)
        let r = qmatch(expr, type<int const?>)
        t |> success(r.matched, "type<int const?> should match")
    }
}

[test(description = "a const pointer type pattern does not match a non-const pointer type")]
def test_type_const_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int?>)
        let r = qmatch(expr, type<int const?>)
        t |> success(!r.matched, "type<int?> should not match type<int const?>")
    }
}

[test(description = "a pointer type pattern does not match a non-pointer type")]
def test_type_pointer_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        let r = qmatch(expr, type<int?>)
        t |> success(!r.matched, "type<int> should not match type<int?>")
    }
}

[test(description = "a type<array<int>> pattern matches an identical array type")]
def test_type_array(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<array<int>>)
        let r = qmatch(expr, type<array<int>>)
        t |> success(r.matched, "type<array<int>> should match")
    }
}

[test(description = "an array type pattern does not match an array with a different element type")]
def test_type_array_wrong_elem(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<array<int>>)
        let r = qmatch(expr, type<array<float>>)
        t |> success(!r.matched, "type<array<int>> should not match type<array<float>>")
    }
}

[test(description = "a table type pattern matches an identical table type")]
def test_type_table(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<string; int>>)
        t |> success(r.matched, "type<table<string;int>> should match")
    }
}

[test(description = "a table type pattern does not match a table with a different value type")]
def test_type_table_wrong_value(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<string; float>>)
        t |> success(!r.matched, "type<table<string;int>> should not match type<table<string;float>>")
    }
}

// ── $t capture ───────────────────────────────────────────────────────────

[test(description = "a $t capture binds the type of a type<> expression")]
def test_capture_t(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        var captured : TypeDeclPtr
        let r = qmatch(expr, type<$t(captured)>)
        t |> success(r.matched, "should capture type with $t")
        if (r.matched) {
            t |> equal(int(captured.baseType), int(Type.tInt))
        }
    }
}

[test(description = "a $t capture binds a const pointer type with its inner type")]
def test_capture_t_pointer(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<float const?>)
        var captured : TypeDeclPtr
        let r = qmatch(expr, type<$t(captured)>)
        t |> success(r.matched, "should capture pointer type with $t")
        if (r.matched) {
            let td = captured
            t |> equal(int(td.baseType), int(Type.tPointer))
            t |> success(td.firstType != null, "should have firstType")
            if (td.firstType != null) {
                t |> equal(int(td.firstType.baseType), int(Type.tFloat))
                t |> success(td.firstType.flags.constant, "inner type should be const")
            }
        }
    }
}

// ── Type matching in call arguments ──────────────────────────────────────

[test(description = "a type<> pattern matches a type argument inside a call")]
def test_type_in_call(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(type<int>))
        let r = qmatch(expr, foo(type<int>))
        t |> success(r.matched, "foo(type<int>) should match")
    }
}

[test(description = "a call pattern rejects a type argument of a different type")]
def test_type_in_call_wrong(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(type<int>))
        let r = qmatch(expr, foo(type<float>))
        t |> success(!r.matched, "foo(type<int>) should not match foo(type<float>)")
    }
}

[test(description = "a $t capture binds the type argument of a call")]
def test_capture_t_in_call(t : T?) {
    ast_gc_guard() {
        // Capture type from a call argument
        var expr = qmacro(foo(type<int>))
        var captured : TypeDeclPtr
        let r = qmatch(expr, foo(type<$t(captured)>))
        t |> success(r.matched, "should capture type from call arg")
        if (r.matched) {
            t |> equal(int(captured.baseType), int(Type.tInt))
        }
    }
}

// ── Type matching in blocks ──────────────────────────────────────────────

[test(description = "qmatch_block matches a type<> argument in a block statement")]
def test_type_in_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo(type<int>); })
        let r = qmatch_block(expr) $ {
            foo(type<int>)
        }
        t |> success(r.matched, "type<int> in block should match")
    }
}

[test(description = "qmatch_block rejects a type<> argument of a different type")]
def test_type_in_block_wrong(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo(type<int>); })
        let r = qmatch_block(expr) $ {
            foo(type<float>)
        }
        t |> success(!r.matched, "type<int> in block should not match type<float>")
    }
}

// ── auto (wildcard type) ─────────────────────────────────────────────

[test(description = "type<auto> matches type<int>")]
def test_auto_matches_int(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        let r = qmatch(expr, type<auto>)
        t |> success(r.matched, "type<auto> should match type<int>")
    }
}

[test(description = "type<auto> matches type<float>")]
def test_auto_matches_float(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<float>)
        let r = qmatch(expr, type<auto>)
        t |> success(r.matched, "type<auto> should match type<float>")
    }
}

[test(description = "type<auto> matches type<string>")]
def test_auto_matches_string(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<string>)
        let r = qmatch(expr, type<auto>)
        t |> success(r.matched, "type<auto> should match type<string>")
    }
}

[test(description = "type<auto> matches a pointer type")]
def test_auto_matches_pointer(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int?>)
        let r = qmatch(expr, type<auto>)
        t |> success(r.matched, "type<auto> should match type<int?>")
    }
}

[test(description = "type<array<auto>> matches an int array type")]
def test_array_auto_matches_array_int(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<array<int>>)
        let r = qmatch(expr, type<array<auto>>)
        t |> success(r.matched, "type<array<auto>> should match type<array<int>>")
    }
}

[test(description = "type<array<auto>> matches a float array type")]
def test_array_auto_matches_array_float(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<array<float>>)
        let r = qmatch(expr, type<array<auto>>)
        t |> success(r.matched, "type<array<auto>> should match type<array<float>>")
    }
}

[test(description = "type<array<auto>> does not match a non-array type")]
def test_array_auto_rejects_non_array(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<int>)
        let r = qmatch(expr, type<array<auto>>)
        t |> success(!r.matched, "type<array<auto>> should not match type<int>")
    }
}

[test(description = "a table type pattern with an auto value matches any value type")]
def test_table_auto_value(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<string; auto>>)
        t |> success(r.matched, "type<table<string;auto>> should match type<table<string;int>>")
    }
}

[test(description = "a table type pattern with an auto key matches any key type")]
def test_table_auto_key(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<auto; int>>)
        t |> success(r.matched, "type<table<auto;int>> should match type<table<string;int>>")
    }
}

[test(description = "a table type pattern with auto key and value matches any table type")]
def test_table_auto_both(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<auto; auto>>)
        t |> success(r.matched, "type<table<auto;auto>> should match type<table<string;int>>")
    }
}

[test(description = "a table type pattern with a wrong fixed key rejects despite an auto value")]
def test_table_auto_wrong_key(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(type<table<string; int>>)
        let r = qmatch(expr, type<table<int; auto>>)
        t |> success(!r.matched, "type<table<int;auto>> should not match type<table<string;int>>")
    }
}

[test(description = "an auto-parameterized array type matches a type argument inside a call")]
def test_auto_in_call(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(type<array<int>>))
        let r = qmatch(expr, foo(type<array<auto>>))
        t |> success(r.matched, "foo(type<array<auto>>) should match foo(type<array<int>>)")
    }
}

[test(description = "an auto-parameterized array type matches a type argument in a block statement")]
def test_auto_in_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo(type<array<int>>); })
        let r = qmatch_block(expr) $ {
            foo(type<array<auto>>)
        }
        t |> success(r.matched, "type<array<auto>> in block should match")
    }
}
