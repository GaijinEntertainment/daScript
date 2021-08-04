require _enum_total

[enum_total]
enum FooBar
    foo
    bar

[export]
def test()
    static_assert(int(FooBar total)==2)
    return true

