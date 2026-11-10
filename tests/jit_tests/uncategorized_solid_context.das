options gen2
options solid_context

require dastest/testing_boost

include _global_variables.das

[test(description = "under solid_context a global var and a shared let global keep their values across calls, and the functions reading them JIT")]
def test_global_variables_solid(t : T?) {
    check_global_variables(t)
}

var g_self : uint8?

def self_address : int {
    unsafe {
        g_self = addr<uint8?>(g_self)
        deref(g_self) = uint8(uint(deref(g_self)) + 1u)
        return intptr(g_self) == intptr(addr<uint8?>(g_self)) ? 3 : 0
    }
}

[test(description = "a global pointer assigned its own address keeps the store under solid_context: incrementing the byte it points at moves the pointer itself")]
def test_global_pointer_holds_its_own_address(t : T?) {
    t |> equal(0, self_address())
}
