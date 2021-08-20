

options debugger = true
require daslib/debug

struct Foo
    a: int

def finalize(var a:Foo&)
    print("destroy Foo[{a.a}]\n")

[export]
def main()
    var a = [[Foo a=0]]
    delete a
    breakpoint()

    var arr <- [{ Foo[] [[Foo a=1]]; [[Foo a=2]] }]
    print("arr len = {length(arr)}\n")

    delete arr
    var arr2 <- [{ Foo[] [[Foo a=3]] }]
    arr <- arr2
    delete arr
