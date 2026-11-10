options gen2

require module_identity_mount/test_module_identity_a
require test_module_identity_b

[export]
def probe() : int {
    return identity_a() + identity_b()
}
