options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public

[never_inline]
def private call_with_block_argument(value : int;
                                     blk : block<(arg : int) : int2>) : int2 {
    return invoke(blk, value)
}
[never_inline]
def private throw_with_block_argument(blk : block<(arg : int) : void>) {
    invoke(blk, 91)
}
def private block_arguments_after_recovery(value : int) : int2 {
    return (call_with_block_argument(value) $(outer : int) : int2 {
        var recovered = 0
        try {
            throw_with_block_argument($(inner : int) {
                panic("nested {inner}")
            })
        } recover {
            recovered = outer
        }
        return int2(recovered, outer)
    })
}

[test]
def test_try_recover_restores_block_arguments(t : T?) {
    t |> equal(block_arguments_after_recovery(17), int2(17))
}

[test]
def test_try_recover(t : T?) {
    t |> run("deref null recovers") @(t : T?) {
        var a = 1
        let b : int?
        try {
            a = deref(b)
        } recover {
            a = 13
        }
        t |> equal(a, 13)
    }
    t |> run("panic recovers") @(t : T?) {
        var a = 1
        try {
            panic("now panic")
        } recover {
            a = 14
        }
        t |> equal(a, 14)
    }
}
