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

[export]
def main()
    print("this context is {intptr(unsafe(addr(this_context())))} {this_context().name} // {this_context().category}\n")
    let t0 = ref_time_ticks()
    let f = fibR(13)
    wait_1_msec()
    let dt = get_time_usec(t0)
    print("hello, world {dt}usec, {double(dt)/1000000.0lf}sec {f}\n")


