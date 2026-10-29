options gen2

module aot_enum_named_like_module

require dastest/testing_boost public

enum aot_enum_named_like_module {
    A
    B
}

struct HoldsEnum {
    f : aot_enum_named_like_module
}

[test]
def test_enum_named_like_module_as_field(t : T?) {
    let h = HoldsEnum(f = aot_enum_named_like_module.B)
    t |> equal(int(h.f), 1)
}
