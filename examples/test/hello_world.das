options log

options persistent_heap
options persistent_string_heap

class Foo
    a : int

class Bar : Foo
    b : int

[export]
def main
    print("sizeof(Foo)=={typeinfo(sizeof type<Foo>)}\n")
    print("sizeof(Bar)=={typeinfo(sizeof type<Bar>)}\n")
    var p : Foo? = new Bar()
    print("p = {p}\n")
    memory_report()
    unsafe
        delete p
    memory_report()
    p = new Bar()
    print("p = {p}\n")
    memory_report()
    p = null
    unsafe
        heap_collect(true)
    memory_report()
