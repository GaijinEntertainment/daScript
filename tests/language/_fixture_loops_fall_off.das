options gen2

//! every function here can fall off its end: the return-path check
//! reports each one, folded or not

struct Holder {
    v : int
}

var g_bound = 3

def break_in_else(x : int) : int {
    while (true) {
        if (x > 0) {
            return 1
        } else {
            break
        }
    }
}

def break_in_with(x : int) : int {
    var h = Holder(v = x)
    while (true) {
        with (h) {
            if (v > 0) {
                break
            }
        }
        return 1
    }
}

def break_in_unsafe(x : int) : int {
    while (true) {
        unsafe {
            if (x > 0) {
                break
            }
        }
        return 1
    }
}

def break_in_try(x : int) : int {
    while (true) {
        try {
            if (x > 0) {
                break
            }
        } recover {
            return 2
        }
        return 1
    }
}

def break_in_recover(x : int) : int {
    while (true) {
        try {
            if (x > 0) {
                return 2
            }
        } recover {
            break
        }
        return 1
    }
}

def while_false() : int {
    while (false) {
        return 1
    }
}

def global_var_bound() : int {
    for (i in range(g_bound)) {
        return i
    }
}

def empty_range_two_args() : int {
    for (i in range(3, 3)) {
        return i
    }
}

def empty_urange() : int {
    for (i in urange(0u)) {
        return int(i)
    }
}

def empty_range64() : int {
    for (i in range64(0l)) {
        return int(i)
    }
}

def empty_urange64() : int {
    for (i in urange64(4ul, 4ul)) {
        return int(i)
    }
}

def one_source_not_constant(a : array<int>) : int {
    for (i, x in range(3), a) {
        return i + x
    }
}

def body_returns_on_one_path(x : int) : int {
    for (i in range(3)) {
        if (i == x) {
            return i
        }
    }
}

[export]
def main {
    var a : array<int>
    print("{break_in_else(0)} {break_in_with(1)} {break_in_unsafe(1)} {break_in_try(1)}\n")
    print("{break_in_recover(0)} {while_false()} {global_var_bound()}\n")
    print("{empty_range_two_args()} {empty_urange()} {empty_range64()} {empty_urange64()}\n")
    print("{one_source_not_constant(a)} {body_returns_on_one_path(9)}\n")
}
