require UnitTest

options log = true
options print_var_access = true

[export]
def efoo ( var foo : TestObjectFoo? )
    set_foo_data(foo, 1)

[export]
def test
    return true

