// options persistent_heap

require daslib/rtti
require fio

[sideeffects]
def fibR(n:int)
    if (n < 2)
        return n
    return fibR(n - 1) + fibR(n - 2)

[sideeffects]
def wait_1_msec
    let t0 = ref_time_ticks()
    while get_time_usec(t0) < 1000
        pass

struct Foo
    i : int

[sideeffects]
def use_heap
    for i in range(10)
        var f = new [[Foo i=i]]
        print("f = {f}\n")

[export]
def main()
    this_context().name := "boohoo"
    use_heap()

