require daslib/safe_addr

struct Foo
    a : int

struct Bar
    pfoo : Foo?#

[export]
def main
    var t : Foo
    var pt = safe_addr(t)
    var abar : array<Bar>
    abar |> push([[Bar pfoo=pt]])

