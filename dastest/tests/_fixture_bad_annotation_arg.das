options gen2

require dastest/testing_boost public

//! a fixture for test_annotation_args.das: a timeout spelled as a
//! string does not compile

[test(timeout = "soon")]
def test_bad_timeout(t : T?) {
    t |> run("never runs") <| @(t : T?) {
        t |> equal(1, 1)
    }
}
