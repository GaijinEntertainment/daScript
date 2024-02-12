options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module helpers shared

require daslib/fio

var public g_failed = false

def unreachable()
    stackwalk(false, false)
    error("dasPEG: unreachable")

def abort(message: string)
    g_failed = true
    error(message)

def iota
    return <- generator<int>() <|
        var i: int = 0
        while true
            i += 1
            return false if i == 0 else yield i
