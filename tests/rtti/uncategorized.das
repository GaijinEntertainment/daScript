options gen2

require daslib/rtti
require dastest/testing_boost public

var g_touch = 0

[init]
def init_alpha {
    g_touch ++
}

[init]
def init_beta {
    g_touch ++
}

[init(late = true)]
def init_gamma_late {
    g_touch ++
}

[test(description = "for_each_init_function yields plain init functions in declaration order and the late init function after them")]
def test_init_function_order(t : T?) {
    var order : array<uint64>
    this_context() |> for_each_init_function($(mnh : uint64) {
        order |> push(mnh)
    })
    let alpha_pos = find_index(order, get_function_mangled_name_hash(@@init_alpha))
    let beta_pos = find_index(order, get_function_mangled_name_hash(@@init_beta))
    let gamma_pos = find_index(order, get_function_mangled_name_hash(@@init_gamma_late))
    t |> success(alpha_pos >= 0 && beta_pos >= 0 && gamma_pos >= 0, "all three [init] functions are yielded")
    t |> success(alpha_pos < beta_pos, "plain [init] functions keep declaration order")
    t |> success(gamma_pos > beta_pos, "[init(late = true)] runs after the plain [init] functions")
}

[test(description = "extract_lint_code returns the text before the first colon, empty when there is no colon")]
def test_extract_lint_code(t : T?) {
    t |> run("strips prefix before first colon") @@(t : T?) {
        t |> equal(extract_lint_code("LINT001: foo bar"), "LINT001")
        t |> equal(extract_lint_code("PERF018: nested loop"), "PERF018")
        t |> equal(extract_lint_code("STYLE005: early-exit braces"), "STYLE005")
    }
    t |> run("empty string when no colon present") @@(t : T?) {
        t |> equal(extract_lint_code("no colon here"), "")
        t |> equal(extract_lint_code(""), "")
    }
    t |> run("first colon wins") @@(t : T?) {
        t |> equal(extract_lint_code("LINT003: foo: bar: baz"), "LINT003")
    }
}
