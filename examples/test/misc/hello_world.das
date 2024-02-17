require math

require daslib/spoof public

struct Foo
    a : int

[jit]
def mk_arr
    var a <- [[auto
        1 => [[Foo a=2]];
        3 => [[Foo a=4]]
    ]]
    debug(a,"a=")
    return <- to_array_move(a)

[export]
def main
    var arr <- mk_arr()
    debug(arr)
