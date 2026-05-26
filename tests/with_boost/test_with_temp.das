options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/with_boost

struct A {
    f1 : int
    f2 : int
}

[test]
def test_temp_array_literal(t : T?) {
    //! Inline array literal expression as the container — temporary,
    //! private to the with_ expression. The helper's generic
    //! ``array<auto(TT)>`` accepts the temp; lock+unlock around it
    //! are harmless extra counter ops.
    var observed = 0
    with_([A(f1=11, f2=22), A(f1=33, f2=44)][0]) $(a) {
        observed = a.f1
    }
    t |> equal(observed, 11, "temp array element bound correctly")
}

[test]
def test_temp_in_chain(t : T?) {
    //! A chain expression where the container itself is a temp inside
    //! a longer chain still works.
    var observed = 0
    with_([A(f1=100, f2=200)][0]) $(a) {
        observed = a.f1 + a.f2
    }
    t |> equal(observed, 300, "temp element bound + read across the body")
}
