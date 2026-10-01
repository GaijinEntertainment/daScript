options gen2
options disable_auto_inline

require dastest/testing_boost public

// Each operand appends its digit to order, so the value shows which side ran first.
// Auto-inline is off so the inliner does not hoist the calls into ordered locals.
// The digits go through a reference argument: AOT code then hands the operands to a
// policy call, and gcc evaluates its arguments right to left if nothing sequences them.
def take(var order : int&; v : int) : int {
    order = order * 10 + v
    return v
}

def take_bool(var order : int&; v : int; result : bool) : bool {
    order = order * 10 + v
    return result
}

[test]
def test_plain_op_evaluates_left_first(t : T?) {
    var order = 0
    t |> equal(take(order, 1) + take(order, 2), 3)
    t |> equal(order, 12, "left operand of + evaluated first")
    order = 0
    t |> equal(take(order, 1) < take(order, 2), true)
    t |> equal(order, 12, "left operand of < evaluated first")
}

[test]
def test_policy_op_evaluates_left_first(t : T?) {
    var order = 0
    t |> equal(take(order, 3) % take(order, 4), 3)
    t |> equal(order, 34, "left operand of % evaluated first")
    order = 0
    var acc = 0
    for (_i in range(3)) {
        acc += take(order, 1) % (take(order, 2) + 1)
    }
    t |> equal(acc, 3)
    t |> equal(order, 121212, "left operand of % evaluated first in a loop")
}

[test]
def test_bool_xor_evaluates_left_first(t : T?) {
    var order = 0
    t |> equal(take_bool(order, 1, true) ^^ take_bool(order, 2, false), true)
    t |> equal(order, 12, "left operand of ^^ evaluated first")
}

[test]
def test_short_circuit_skips_right(t : T?) {
    var order = 0
    t |> equal(take_bool(order, 1, false) && take_bool(order, 2, true), false)
    t |> equal(order, 1, "&& does not evaluate the right operand")
    order = 0
    t |> equal(take_bool(order, 1, true) || take_bool(order, 2, false), true)
    t |> equal(order, 1, "|| does not evaluate the right operand")
}

[test]
def test_post_decrement_operand_in_loop(t : T?) {
    var roll = 2
    var limit = 5
    var hits = 0
    while (limit > 0) {
        limit--
        if (roll-- <= 0) {
            hits++
        }
    }
    t |> equal(hits, 3, "the operand is the value before the decrement")
    t |> equal(roll, -3)
}
