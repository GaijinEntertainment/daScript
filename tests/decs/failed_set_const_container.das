//! Setting a component from a constant array must not compile: the
//! array moves into the component, and a constant can't be emptied.
options gen2
expect 31400

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_set_rejects_constant_container(t : T?) {
    var cmp : ComponentMap
    let values <- [1, 2, 3]
    cmp |> set("values", values)
}
