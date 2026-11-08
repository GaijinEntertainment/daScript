options gen2

require daslib/assert_once
require dastest/testing_boost public

[test(description = "assert_once fails on the first violation only, so five passes through a false condition recover exactly once")]
def test_assert_once(t : T?) {
    var ii = 0
    var count = 0
    label 0:
    try {
        assert_once(ii == 0)
    } recover {
        count++
    }
    ii++
    if (ii != 5) {
        goto 0
    }
    t |> equal(count, 1)
}

// assert over an expression whose only effect is modifyArgument on a freshly-built temporary.
// Modifying a temporary the expression itself constructs is unobservable, so the side-effect
// analysis treats such a call as side-effect-free and assert must accept it.
//
// Before the side-effect fix this file failed to COMPILE with error[30151] "assert expressions
// can't have side-effects": the move-through to_array_move / to_table_move over an array/table
// literal, and a `var` parameter that mutates a make-local argument, all carried modifyArgument.
// assert is used deliberately here — these statements ARE the thing under test (that assert
// accepts them), not test assertions; the dastest API checks come after.

struct A {
    x : int
}

def modify_temp(var a : A) : bool {
    a.x = 5            // mutates the argument -> the function carries modifyArgument
    return a.x == 5
}

[test(description = "assert accepts an expression whose only side effect mutates a temporary the expression itself built")]
def test_assert_over_temporary(t : T?) {
    assert(modify_temp(A(x = 1)))               // make-local temp, directly modified
    assert(length([10, 20, 30]) == 3)           // array literal (to_array_move), operator-wrapped
    assert(length({1 => 10, 2 => 20}) == 2)     // table literal (to_table_move)
    assert(([1, 2, 3] |> length) == 3)          // array literal, pipe form
    t |> success(true, "asserts over temporary-modifying expressions compiled")
}
