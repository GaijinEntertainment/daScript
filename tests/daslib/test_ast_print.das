options gen2
options rtti
options no_aot
options no_coverage

require dastest/testing_boost public
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/ast_print
require strings
require daslib/strings_boost

var g_seed = 1

[never_inline]
def apply_ro(v : int; blk : block<(x : int) : int>) : int {
    return invoke(blk, v)
}

[never_inline]
def apply_rw(v : int; blk : block<(var y : int) : int>) : int {
    return invoke(blk, v)
}

[never_inline]
def fixture_shape() : int {
    let a = g_seed + 1
    var b = a * 2
    b += apply_ro(b) $(x : int) : int {
        return x + 1
    }
    b += apply_rw(b) $(var y : int) : int {
        y ++
        return y
    }
    return b
}

def printed_fixture(t : T?) : string {
    var txt = ""
    compiling_module() |> for_each_function("fixture_shape") $(var func) {
        if (empty(txt)) {
            txt = build_string() $(var w) {
                printFunc(func, unsafe(addr(w)))
            }
        }
    }
    t |> success(!empty(txt), "fixture_shape not found in the compiling module")
    return txt
}

[test]
def test_ast_print_let_var(t : T?) {
    g_seed = 1
    t |> equal(fixture_shape(), 19)
    let golden = join(fixed_array<string>(
        "[never_inline]",
        "def fixture_shape : int const \{",
        "    let a : int const = (g_seed + 1)",
        "    var b : int = (a * 2)",
        "    (b += apply_ro(b,$(x : int const):int \{",
        "        return (x + 1)",
        "    \}))",
        "    (b += apply_rw(b,$(var y : int -const):int \{",
        "        ++(y)",
        "        return y",
        "    \}))",
        "    return b",
        "\}",
        "",
        ""), "\n")
    t |> run("locals and closure arguments print their own mutability") @(t : T?) {
        t |> equal(printed_fixture(t), golden)
    }
}
