//! Reading a component that can't be copied out of a component map
//! must not compile: update_entity tells a replaced value from a kept
//! one by its bytes, which holds only while a block can't take such a
//! value out, change it, and set it back.
options gen2
expect 30222:1

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_get_rejects_an_owned_component(t : T?) {
    var cmp : ComponentMap
    var held : array<int>
    get(cmp, "values", held)
}
