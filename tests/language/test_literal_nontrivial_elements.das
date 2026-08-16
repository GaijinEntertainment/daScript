// Bracket/ctor literal lowering must construct non-trivial elements identically in
// every tier: moved-in iterator elements, fixed-array elements, and a comprehension
// moved straight into a constructor argument.
options gen2
require dastest/testing_boost public
require daslib/coroutines

var g_a_steps = 0
var g_b_steps = 0

[coroutine]
def step_co_a() {   // nolint:STYLE038 length is the [coroutine] expansion, not the source body
    g_a_steps ++
    co_continue()
    g_a_steps ++
}

[coroutine]
def step_co_b() {   // nolint:STYLE038 length is the [coroutine] expansion, not the source body
    g_b_steps ++
    co_continue()
    g_b_steps ++
    co_continue()
    g_b_steps ++
}

def make_iota() : int[4] {
    var r : int[4]
    for (i in range(4)) {
        r[i] = i + 1
    }
    return r
}

struct IntsPair {
    a : array<int>
    b : array<int>
}

[test]
def test_literal_iterator_elements(t : T?) {
    t |> run("moved-in iterator elements arrive live") @(t : T?) {
        g_a_steps = 0
        g_b_steps = 0
        var tasks <- [ <- step_co_a(), <- step_co_b()]
        cr_run_all(tasks)
        t |> equal(g_a_steps, 2)
        t |> equal(g_b_steps, 3)
    }
}

[test]
def test_literal_fixed_array_elements(t : T?) {
    t |> run("fixed-array elements carry their contents") @(t : T?) {
        var arr <- [make_iota(), make_iota()]
        var total = 0
        for (fa in arr) {
            for (x in fa) {
                total += x
            }
        }
        t |> equal(total, 20)
        delete arr
    }
}

[test]
def test_ctor_comprehension_move_init(t : T?) {
    t |> run("a comprehension moved into a constructor argument keeps its elements") @(t : T?) {
        var src <- [1, 2, 3]
        var other <- [7, 8]
        var pair = IntsPair(a <- [for (x in src); x * 10], b <- other)
        t |> equal(length(pair.a), 3)
        t |> equal(pair.a[0], 10)
        t |> equal(pair.a[2], 30)
        t |> equal(length(pair.b), 2)
        t |> equal(pair.b[1], 8)
        delete pair
        delete src
    }
}
