options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module helpers

require daslib/fio

def abort(message: string)
    print("{message}\n")
    stackwalk(false, false)
    unsafe { exit(1); }


def iota
    return <- generator<int>() <|
        var i: int = 0
        while true
            i += 1
            return false if i == 0 else yield i
