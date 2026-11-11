//! Setting a component from a constant array must not compile through
//! any of the writers: the array moves into the component, and a
//! constant can't be emptied. set_direct forwards to set_direct_at,
//! so the two share one failing instance.
options gen2
expect 31400:3

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_set_rejects_constant_container(t : T?) {
    var cmp : ComponentMap
    let values <- [1, 2, 3]
    cmp |> set("values", values)
    set(cmp.other, values)
}

def direct_rejects_constant_container(var arch : Archetype) {
    let values <- [1, 2, 3]
    arch |> set_direct("values", 0, values)
    arch |> set_direct_at(0, 0, values)
}
