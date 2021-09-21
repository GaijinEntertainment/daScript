options persistent_heap
options persistent_string_heap
options gc

struct Foo
    a : int

struct Bar
    p : Foo?

[export]
def main
    var f = new [[Foo]]
    var a = new [[Bar p=f]]
    unsafe
        delete f
    unsafe
        heap_collect(true)

