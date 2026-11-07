options gen2
require dastest/testing_boost

def instantiated(x : auto) : int => 1

def user(x : auto) : int => instantiated(x)

def holds_void_pointer(x : auto) : bool => typeinfo is_void_pointer(x._0)

[test]
def test_a_generic_over_a_tuple_instantiates_for_void_ptr_after_other_pointers(t : T?) {
    var a : int?
    var b : float?
    var c : void?
    t |> equal(user((a,)) + user((b,)) + user((c,)), 3)
}

[test]
def test_a_void_ptr_tuple_gets_its_own_instance_after_one_other_pointer(t : T?) {
    var a : int?
    var c : void?
    t |> equal(holds_void_pointer((a,)), false)
    t |> equal(holds_void_pointer((c,)), true)
}

[test]
def test_a_variant_or_iterator_over_void_ptr_gets_its_own_instance(t : T?) {
    var va : variant<v : int?>
    let vc : variant<v : void?>
    t |> equal(user(va) + user(vc), 2)
    let ia : iterator<int?>
    let ic : iterator<void?>
    t |> equal(user(ia) + user(ic), 2)
}
