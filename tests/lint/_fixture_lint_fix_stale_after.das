//fmt:ignore-file
options gen2
options auto_inline_functions = false

def keep_one() : int {   // nolint:STYLE042 one of the two suppresses
    return 1
}

def drop_all() : int {
    let x = 1
    return x + 1
}


def solo() : int {
    let y = 2
    return y + 2
}
