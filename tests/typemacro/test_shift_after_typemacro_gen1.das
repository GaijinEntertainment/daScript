options gen2 = false
require dastest/testing_boost public
require _typemacro_mod

// gen1: '>>' after a closed $name<types> list is a shift
typedef Arr = $tm_make<float>(4, true, "arr")

[test]
def test_shift_after_type_list_gen1(t : T?)
    let x = 0x100u
    t |> equal(x >> 4u, 0x10u)
    t |> equal(x >>> 4u, 0x10u)
