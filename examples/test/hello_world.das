require UnitTest

options log = true
options print_var_access = true

def efoo ( var foo : TestObjectFoo )
    unsafe
        var pfoo = addr(foo)
        set_foo_data(pfoo, 1)

[export]
def test
    var foo: TestObjectFoo
    efoo(foo)
    return true

