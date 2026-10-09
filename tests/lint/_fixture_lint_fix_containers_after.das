//fmt:ignore-file
options gen2
options auto_inline_functions = false

require daslib/linq

struct Bag {
    nums : array<int>
}

def squares(src : array<int>) : array<int> {
    var out <- [for (x in src); x * x]
    return <- out
}

def evens(src : array<int>) : array<int> {
    var out <- [for (x in src); x; where x % 2 == 0]
    return <- out
}

def index_of(src : array<string>) : table<string; int> {
    var out <- {for (s, i in src, count()); s => i}
    return <- out
}

def commented_loop(src : array<int>) : array<int> {
    var out : array<int>
    for (x in src) {
        out |> push(x + 1)  // a comment keeps the loop
    }
    return <- out
}

def lookup() : table<string; int> {
    var t <- { "one" => 1, "two" => 2 }
    return <- t
}

def names() : table<string> {
    var s <- { "a", "b" }
    return <- s
}

def copy_of(src : array<int>) : array<int> {
    var w := src
    return <- w
}

def both(a, b : array<int>) : array<int> {
    var c <- concat(a, b)
    return <- c
}

def append(var dst : array<int>; a, b : array<int>) {
    dst |> push_from(a, b)
}

def append_field(var bag : Bag; a, b : array<int>) {
    bag.nums |> push_clone_from(a, b)
}

def reads_itself(src : array<int>) : array<int> {
    var out : array<int>
    for (x in src) {
        out |> push(length(out) + x)
    }
    return <- out
}

def commented_run() : table<string; int> {
    var t : table<string; int>
    t |> insert("one", 1)  // first
    t |> insert("two", 2)
    return <- t
}

def uses_linq(a, b : array<int>) : array<int> => concat(a, b)
