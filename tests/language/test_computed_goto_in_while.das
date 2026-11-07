options gen2
require dastest/testing_boost

def run(c : int) : int {
    var r = 0
    while (true) {
        goto c + 1000
        label 1000:
        if (c == 0) {
            r = 10
            break
        }
        label 1001:
        if (c == 1) {
            r = 11
            break
        }
        r = -1
        break
    }
    return r
}

[sideeffects]
def jump_to(target : int) : int {
    var r = 0
    try {
        goto target
        label 1:
        r = 1
    } recover {
        r = -1
    }
    return r
}

[test]
def test_a_computed_goto_into_a_while_body_lands_on_its_label(t : T?) {
    let k = 1
    t |> equal(11, run(k), "the target held in a variable")
    for (j in range(2)) {
        t |> equal(10 + j, run(j), "the target from a loop variable {j}")
    }
}

[sideeffects]
def jump_in_block(target : int) : int {
    return invoke($(n : int) : int {
        var q = 0
        try {
            goto n
            label 3:
            q = 3
        } recover {
            q = -1
        }
        return q
    }, target)
}

[sideeffects]
def jump_past_a_block_label(target : int) : int {
    var r = 0
    try {
        goto target
        label 1:
        r = 1
    } recover {
        r = -1
    }
    return r + invoke($() : int {
        label 3:
        return 3
    })
}

[test]
def test_a_computed_goto_resolves_against_the_labels_of_its_own_block(t : T?) {
    let three = 3
    t |> equal(3, jump_in_block(three), "a block literal's computed goto lands on its own label")
    let one = 1
    t |> equal(-1, jump_in_block(one),
               "a block literal's computed goto does not see the function's labels")
    t |> equal(4, jump_past_a_block_label(one))
    t |> equal(2, jump_past_a_block_label(three),
               "a label only a block literal declares is not the function's")
}

[sideeffects]
def nine : int => 9

var g_init_jump = invoke($() : int {
    var r = 0  // nolint:LINT003 - written inside the try arms of a block literal
    try {
        goto nine()
        label 1:
        r = 1
    } recover {
        r = -2
    }
    return r
})

[test]
def test_a_computed_goto_to_a_label_nothing_declares_throws(t : T?) {
    let one = 1
    t |> equal(1, jump_to(one))
    let missing = 77
    t |> equal(-1, jump_to(missing), "a label no block declares is a run-time error")
    let negative = -5
    t |> equal(-1, jump_to(negative))
    let unused = 0
    t |> equal(-1, jump_to(unused), "a number below the highest label that no label uses")
    t |> equal(-2, g_init_jump, "a global initializer's computed goto is checked too")
}
