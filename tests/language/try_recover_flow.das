options gen2
require dastest/testing_boost public

var g_rounds = 5

[export, no_jit]
def break_in_for : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            break if (i == 2)
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def continue_in_while : int {
    var seen = 0
    var i = 0
    while (i < g_rounds) {
        i ++
        try {
            continue if (i % 2 == 0)
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def break_in_recover : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            panic("stop") if (i == 2)
            seen ++
        } recover {
            break
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def nested_try : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            try {
                continue if (i == 1)
                break if (i == 3)
            } recover {
            }
            seen ++
        } recover {
        }
        seen += 10
    }
    return seen
}

[export, no_jit]
def inner_loop_break : int {
    var seen = 0
    for (i in range(0, g_rounds)) {
        try {
            for (j in range(0, 4)) {
                break if (j == 1)
                seen ++
            }
            break if (i == 2)
        } recover {
        }
        seen += 10
    }
    return seen
}

[export]
def inner_only_break : int {
    var seen = 0
    try {
        for (j in range(0, 4)) {
            break if (j == 2)
            seen ++
        }
        seen += 100
    } recover {
    }
    return seen
}

[export]
def try_without_exit : int {
    var seen = 0
    for (_i in range(0, g_rounds)) {
        try {
            seen ++
        } recover {
        }
    }
    return seen
}

[test]
def test_try_recover_control_flow(t : T?) {
    t |> equal(break_in_for(), 22, "break leaves the for loop")
    t |> equal(continue_in_while(), 33, "continue skips the rest of the while iteration")
    t |> equal(break_in_recover(), 22, "break in the recover block leaves the loop")
    t |> equal(nested_try(), 22, "nested try chains the exit outwards")
    t |> equal(inner_loop_break(), 23, "break in a loop inside the try stays inner")
    t |> equal(inner_only_break(), 102, "a loop break with no loop around the try needs no exit")
    t |> equal(try_without_exit(), 5, "try with no exit is unaffected")
}
