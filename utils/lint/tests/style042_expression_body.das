options gen2

expect 31209:3

require daslib/style_lint


def bad_one_line { return 13 }

def bad_move(var a : array<int>) : array<int> {
    return <- a
}

class Counter {
    n : int
    def bad_method : int {
        return n + 1
    }
}


def good_two_statements(a : int) : int {
    let b = a * 2
    return b
}

def good_void_return(var a : array<int>) {
    a |> push(1)
    return
}

def good_wrapped(a, b : int) : int {
    return (a
        + b)
}

def good_table_body() : table<string; int> {
    return <- { "a" => 1 }
}

def good_range_body(a, b : int) : range {
    return a..b
}

def good_long_header_comment(first_argument, second_argument : int; third_argument : float) : float {  // a header comment keeps the block
    return float(first_argument * second_argument) + third_argument * 2.0
}

def good_finally(var a : array<int>) : int {
    return length(a)
} finally {
    a |> clear()
}
