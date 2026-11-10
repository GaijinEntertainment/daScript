options gen2

//! no function here can fall off its end: the return-path check
//! accepts each one, folded or not

let LOOP_COUNT = 3

def over_global_constant() : int {
    for (i in range(LOOP_COUNT)) {
        return i + 1
    }
}

def over_range_two_args() : int {
    for (i in range(2, 5)) {
        return i
    }
}

def over_urange() : int {
    for (i in urange(3u)) {
        return int(i) + 3
    }
}

def over_range64() : int {
    for (i in range64(4l, 9l)) {
        return int(i)
    }
}

def over_urange64() : int {
    for (i in urange64(5ul)) {
        return int(i) + 5
    }
}

def dead_break_after_return(x : int) : int {
    while (true) {
        return x
        break
    }
}

[export]
def main {
    print("{over_global_constant()} {over_range_two_args()} {over_urange()}\n")
    print("{over_range64()} {over_urange64()} {dead_break_after_return(6)}\n")
}
