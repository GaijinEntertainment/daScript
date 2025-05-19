options gen2
require UnitTest

def test_fn(a, b) {
}

[sideeffects]
def takeFoo(var foo : TestObjectFoo; fooRO : TestObjectFoo) {
    let interactMask = SomeEnum_16.SomeEnum_16_two & (~(SomeEnum_16.SomeEnum_16_zero | SomeEnum_16.SomeEnum_16_one))
    test_fn(interactMask, foo.isReadOnly);
}

[export]
def test {
    var foo : TestObjectFoo
    let fooRO : TestObjectFoo
    verify(!foo.isReadOnly)
    assert(fooRO.isReadOnly)
    takeFoo(foo, fooRO)
    return true
}

