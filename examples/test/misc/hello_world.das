require daslib/profiler

require daslib/rtti

[sideeffects]
def fibR(n:int)
    if (n < 2)
        return n
    return fibR(n - 1) + fibR(n - 2)

[export]
def main()
    print("this context is {intptr(unsafe(addr(this_context())))}\n")
    let t0 = ref_time_ticks()
    let f = fibR(13)
    let dt = get_time_usec(t0)
    print("hello, world {dt}usec\n")

