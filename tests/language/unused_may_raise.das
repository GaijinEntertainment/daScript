options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require strings

[sideeffects, no_jit]
def unused_index() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10]
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_div(num : int; den : int) : bool {
    var ex = false
    try {
        var _unused = num / den
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_first_character(s : string) : bool {
    var ex = false
    try {
        var _unused = first_character(s)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def dead_store() : bool {
    let arr <- [1, 2]
    var y = 0
    var ex = false
    try {
        y = arr[10]
        y = 5
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def same_expression() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10] - arr[10]  // nolint:LINT007 -- deliberate: the fold under test
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def folded_operand() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10] * 0
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def dropped_condition() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        if (arr[10] > 0) {
        }
    } recover {
        ex = true
    }
    return ex
}

def at_ten(a : array<int>) : int => a[10]

[sideeffects, no_jit]
def unused_user_call() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = at_ten(arr)
    } recover {
        ex = true
    }
    return ex
}

struct Boxed {
    v : int
}

def operator +(a, b : Boxed) : Boxed {
    let arr <- [a.v, b.v]
    return Boxed(v = arr[10])
}

[sideeffects, no_jit]
def unused_user_operator() : bool {
    var ex = false
    try {
        var _unused = Boxed(v = 1) + Boxed(v = 2)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_mod(num : int; den : int) : bool {
    var ex = false
    try {
        var _unused = num % den
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_string_to_int(s : string) : bool {
    var ex = false
    try {
        var _unused = int(s)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_invoked_block() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        let blk = $() : int {
            return arr[10]
        }
        var _unused = invoke(blk)
    } recover {
        ex = true
    }
    return ex
}

[never_inline]
def asserted(x : int) : int {
    assert(x > 0, "not positive")
    return x
}

[never_inline]
def verified(x : int) : int {
    verify(x > 0, "not positive")
    return x
}

[sideeffects, no_jit]
def unused_asserted_call() : bool {
    var ex = false
    try {
        var _unused = asserted(-1)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_verified_call() : bool {
    var ex = false
    try {
        var _unused = verified(-1)
    } recover {
        ex = true
    }
    return ex
}

[test]
def test_unused_may_raise(t : T?) {
    t |> run("unused out of range index still raises") @(t : T?) {
        t |> equal(unused_index(), true)
    }
    t |> run("unused division by zero still raises") @(t : T?) {
        t |> equal(unused_div(7, 0), true)
    }
    t |> run("unused first_character of an empty string still raises") @(t : T?) {
        t |> equal(unused_first_character(""), true)
    }
    t |> run("a dead store whose value raises still raises") @(t : T?) {
        t |> equal(dead_store(), true)
    }
    t |> run("E op E folding keeps the raise") @(t : T?) {
        t |> equal(same_expression(), true)
    }
    t |> run("an operand folded away by x * 0 still raises") @(t : T?) {
        t |> equal(folded_operand(), true)
    }
    t |> run("the condition of an empty if still raises") @(t : T?) {
        t |> equal(dropped_condition(), true)
    }
    t |> run("an unused call to a das function whose body raises still raises") @(t : T?) {
        t |> equal(unused_user_call(), true)
    }
    t |> run("an unused user operator whose body raises still raises") @(t : T?) {
        t |> equal(unused_user_operator(), true)
    }
    t |> run("unused modulo by zero still raises") @(t : T?) {
        t |> equal(unused_mod(7, 0), true)
    }
    t |> run("an unused failing string-to-int conversion still raises") @(t : T?) {
        t |> equal(unused_string_to_int("abc"), true)
    }
    t |> run("an unused invoke of a block whose body raises still raises") @(t : T?) {
        t |> equal(unused_invoked_block(), true)
    }
    t |> run("an unused call to a das function that asserts still raises") @(t : T?) {
        t |> equal(unused_asserted_call(), true)
    }
    t |> run("an unused call to a das function that verifies still raises") @(t : T?) {
        t |> equal(unused_verified_call(), true)
    }
}
