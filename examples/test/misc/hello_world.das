require daslib/coroutines

require fio
require strings
options log_infer_passes

var time : int64 = ref_time_ticks()

struct Data
    a : int
    b : string
    arr : array<int>

def get_time()
    return format("%.2f", double(get_time_usec(time)) / 1000.lf)

[coroutine] def get_plus_1(a : int) : int
    print("{get_time()} get_plus_1 1\n")
    async_continue() // frame pause
    print("{get_time()} get_plus_1 2\n")
    yield a + 1
    return false // force exit

[coroutine] def get_data(a : int) : Data
    print("{get_time()} get_data 1\n")
    async_continue()
    print("{get_time()} get_data 2\n")
    var data : Data
    data.a = a
    data.b = "hello"
    data.arr <- [{int 1; }]
    yield <- data

[coroutine] def getall()
    // async_continue()
    // async_continue()
    var a2 : int = async <| get_plus_1(1)
    debug(a2)
    // var a : int
    // a |> async <| get_plus_1(1)
    // debug(a)

    // async_continue()

    // var b : Data
    // b |> async <| get_data(1)
    // debug(b)


[export]
def main()
    print("----\n")
    var all <- getall()
    var foo : bool
    while next(all, foo)
        print("{get_time()} step...\n")
        sleep(1000u)
    print("----\n")

    // var a <- get_plus_1(1)
    // var resa : int = cr_get(a)
    // debug(resa)
    // print("\n")

    // var b <- get_data(1)
    // var resb <- cr_get(b)
    // debug(resb)
    // print("\n")
