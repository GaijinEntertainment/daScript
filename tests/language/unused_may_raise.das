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
        var _unused = arr[10] - arr[10]
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
}
