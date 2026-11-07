options gen2
require dastest/testing_boost

[export]
def skip_init(n : int) : int {
    if (n > 1) {
        goto label 1
    }
    var g = n * 2
    label 1:
    g = n
    return g
}

[export]
def skip_always(n : int) : int {
    goto label 1
    var g = n * 2
    label 1:
    g = n + 1
    return g
}

[never_inline]
def call_it(blk : block<(x : int) : int>; v : int) : int => invoke(blk, v)

[export]
def skip_block_let(n : int) : int {
    if (n > 1) {
        goto label 1
    }
    let b = $(x : int) : int => x + n
    label 1:
    return n > 1 ? n : call_it(b, 1)
}

[test]
def test_a_goto_past_a_var_declaration_runs_on_every_tier(t : T?) {
    t |> equal(skip_init(3), 3)
    t |> equal(skip_init(1), 1)
    t |> equal(skip_always(3), 4, "the declaration survives the unreachable region")
    t |> equal(skip_block_let(1), 2, "a block-typed let after a conditional goto")
    t |> equal(skip_block_let(3), 3)
}
