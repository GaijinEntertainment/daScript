options gen2
expect 30220 : 4    // returning void value into a non-void result, via move, and inside a generator block
expect 30343 : 1    // structured auto result (auto?) never collapses to void
expect 30407 : 1

require dastest/testing_boost public

var g_hits = 0

def bump_by(n : int) {
    g_hits += n
}

def take_int_block(a : block<(x : int) : int>) : int {
    return a(1)
}

def fail_int_result(n : int) : int {
    return bump_by(n)
}

def fail_move_void(n : int) : void {
    return <- bump_by(n)
}

def fail_int_block(n : int) : int {
    return take_int_block() $(x) : int => bump_by(x)
}

def fail_structured_auto(n : int) : auto? {
    return bump_by(n)
}

def fail_generator : iterator<int> {
    return generator<int>() <| $ {
        yield 1
        return bump_by(1)
    }
}
