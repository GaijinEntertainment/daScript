// options log=true, log_stack=true, log_nodes=true
// options log_infer_passes=true
// options print_var_access=true

options log=true

require unit_tests.foo_bar

[export]
def test
    print("{getValue()}")
    return true

