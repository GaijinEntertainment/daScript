options gen2
require dastest/testing_boost public

def panic_under_finally(var ran : bool&) {
    panic("PANIC_SKIPS_FINALLY_PROBE")
} finally {
    ran = true
}

[test]
def test_panic_skips_finally(t : T?) {
    t |> run("a panic skips the function's finally, as in every tier") @(t : T?) {
        var ran = false
        var caught = false
        try {
            panic_under_finally(ran)
        } recover {
            caught = true
        }
        t |> equal(caught, true)
        t |> equal(ran, false)
    }
}
