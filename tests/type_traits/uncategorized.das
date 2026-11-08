options gen2

require dastest/testing_boost public
require daslib/bitfield_trait
require daslib/type_traits

bitfield Test {
    one
    two
    three
}

[test(description = "each_bit_name lists the names of a bitfield type in bit order")]
def test_bitfield_traits(t : T?) {
    for (name, bit in each_bit_name(type<Test>), count()) {
        if (bit == 0) {
            t |> equal("one", name)
        } elif (bit == 1) {
            t |> equal("two", name)
        } elif (bit == 2) {
            t |> equal("three", name)
        } else {
            t |> failure("unexpected bitfield value")
        }
    }
}

struct Foo {
    a : int
    b : string
}

struct Bar : Foo {
    c : float
}

[test(description = "fields_count reports the number of fields of a struct type")]
def test_traits(t : T?) {
    t |> equal(2, typeinfo fields_count(type<Foo>))
    t |> equal(3, typeinfo fields_count(type<Bar>))
}
