options gen2
options indenting = 4
options rtti
options no_coverage
options auto_inline_functions = false
options disable_algebraic_folding = true // the built-in identity arms stand down
options _fold_rules = false // ... and so do the declarative ones

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require strings
require daslib/strings_boost
require math
require daslib/fold_rules_std
require dastest/testing_boost public

var g_i = 3

[export, never_inline]
def t_untouched(x : int) : int {
    return x * 1
}

[export, never_inline]
def t_idiom_untouched(x : float) : float {
    return pow(x, 2.0)
}

def body_of(nm : string) : string {
    var body = ""
    compiling_module() |> for_each_function(nm) $(func) {
        if (empty(body)) {
            body = "{describe(func.body)}"
        }
    }
    let clean <- [for (ln in split(body, "\n")); ln; where find(ln, "coverage(", 0) < 0]
    return join(clean, " ")
}

[test]
def test_the_option_turns_every_rule_off(t : T?) {
    ast_gc_guard() {
        let b = body_of("t_untouched")
        t |> success(find(b, "*", 0) >= 0, "x * 1 must survive with _fold_rules = false: {b}")
        let p = body_of("t_idiom_untouched")
        t |> success(find(p, "pow", 0) >= 0, "pow must survive with _fold_rules = false: {p}")
    }
}

[test]
def test_unfolded_functions_still_compute(t : T?) {
    t |> equal(t_untouched(g_i), 3)
    t |> equal(t_idiom_untouched(3.0), 9.0)
}
