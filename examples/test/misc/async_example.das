require fio
require strings
require daslib/async_boost
require daslib/coroutines


let begin = ref_time_ticks()

def log(msg)
    print("{format("%0.3f", float(get_time_usec(begin)) / 1000000.)}: {msg}")

struct Data
    a : int
    b : string
    arr : array<int>

[async] def get_plus_1(a : int) : int
    log("get_plus_1 1\n")
    await_next_frame() // frame pause
    log("get_plus_1 2\n")
    yield a + 1
    // return false // force exit

[async] def get_data(a : int) : Data
    log("get_data 1\n")
    await_next_frame()
    log("get_data 2\n")
    var data : Data
    data.a = a
    data.b = "hello"
    data.arr <- [{int 1; }]
    yield <- data

[async] def async_range() : int
    yield 1
    yield 3
    yield 5

[async] def bool_range() : bool
    yield true
    yield false
    yield true

[coroutine] def just_coroutine(n) : void
    for i in 0..n
        yield true


[async] def getall() : void
    let a2 = await <| get_plus_1(100)
    debug(a2)

    log("pass frame\n")
    await_next_frame()

    // await <| get_data(1) // 40102: call annotated by AwaitMacro failed to transform, result should be assigned to a variable, use 'let res <- await(get_data(1))'
    let res = await <| get_data(1)
    debug(res)


// coroutine
[async] def getrealall() : void
    log("getrealall 1\n")
    await_next_frame()
    log("getrealall 2\n")
    await_next_frame()
    log("getrealall 3\n")
	await <| getall() // ok, async function with void result

    log("async_range\n")
    for i in async_range()
        print("int {i}\n")
        await_next_frame()

    log("bool_range\n")
    for b in bool_range()
        print("bool {b}\n")
        await_next_frame()

    let n = 5
    log("wait coroutine ({n} frames)\n")
    await <| just_coroutine(n)
    log("coroutine done\n")


[export]
def main()
    log("----\n")
    var all <- getrealall()
    var foo : bool
    while next(all, foo)
        log("step...\n")
        sleep(500u)
    log("----\n")
