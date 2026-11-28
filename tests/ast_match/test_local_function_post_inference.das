options gen2
options indenting = 4
options rtti
options no_coverage
options disable_forward    // each target pins a `let` that is read once
options _cyclomatic_complexity = 0  // qmatch patterns expand into matcher code

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

[export]
def target_with_local_fn() {
    let f = @@(x : int) { return x + 1; }
    feint("{f}")
}

[export]
def target_with_local_fn_wrong() {
    let f = @@(x : int) { return x - 1; }
    feint("{f}")
}

[test(description = "a local function pattern matches the inferred form, an address-of expression, in a compiled function")]
def test_local_fn_post_inference(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_with_local_fn)
        t |> success(func != null, "should find target_with_local_fn")
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $() {
            let f = @@(x : int) { return x + 1; }
            feint("{f}")
        }
        t |> success(r.matched, "should match post-inference local function via ExprAddr")
    }
}

[test(description = "a local function pattern with a different body fails against the inferred form")]
def test_local_fn_post_inference_wrong_body(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_with_local_fn)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $() {
            let f = @@(x : int) { return x - 1; }
            feint("{f}")
        }
        t |> success(!r.matched, "wrong body in local function should fail")
    }
}

[test(description = "a $e capture binds the body statement of an inferred local function")]
def test_local_fn_capture_body(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_with_local_fn)
        if (func == null) {
            return
        }
        var body_expr : ExpressionPtr
        let r = qmatch_function(func) $() {
            let f = @@(x : int) { $e(body_expr); }
            feint("{f}")
        }
        t |> success(r.matched, "should match with $e capture on local fn body")
        if (r.matched) {
            t |> equal(body_expr.__rtti, "ExprReturn")
        }
    }
}

[export]
def target_with_local_fn_two_args() {
    let f = @@(a : int; b : float) { return float(a) + b; }
    feint("{f}")
}

[test(description = "a local function pattern with two arguments matches the inferred form")]
def test_local_fn_match_two_args(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(),
            @@target_with_local_fn_two_args)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $() {
            let f = @@(a : int; b : float) { return float(a) + b; }
            feint("{f}")
        }
        t |> success(r.matched, "should match local fn with two args")
    }
}

[test(description = "an $i capture binds the name of the let variable holding an inferred local function")]
def test_local_fn_capture_var_name(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_with_local_fn)
        if (func == null) {
            return
        }
        var vname : string
        let r = qmatch_function(func) $() {
            let $i(vname) = @@(x : int) { return x + 1; }
            feint("{f}")
        }
        t |> success(r.matched, "should match with $i capture on let variable name")
        if (r.matched) {
            t |> equal(vname, "f")
        }
    }
}
