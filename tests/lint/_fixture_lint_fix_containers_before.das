//fmt:ignore-file
options gen2
options auto_inline_functions = false

require daslib/linq

struct Bag {
    nums : array<int>
}

def squares(src : array<int>) : array<int> {
    var out : array<int>
    for (x in src) {
        out |> push(x * x)
    }
    return <- out
}

def evens(src : array<int>) : array<int> {
    var out : array<int>
    for (x in src) {
        if (x % 2 == 0) {
            out |> push(x)
        }
    }
    return <- out
}

def index_of(src : array<string>) : table<string; int> {
    var out : table<string; int>
    for (s, i in src, count()) {
        out |> insert(s, i)
    }
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
    var t : table<string; int>
    t |> insert("one", 1)
    t["two"] = 2
    return <- t
}

def names() : table<string> {
    var s : table<string>
    s |> insert("a")
    s |> insert("b")
    return <- s
}

def copy_of(src : array<int>) : array<int> {
    var w : array<int>
    w |> push_from(src)
    return <- w
}

def both(a, b : array<int>) : array<int> {
    var c : array<int>
    c |> push_clone_from(a)
    c |> push_clone_from(b)
    return <- c
}

def append(var dst : array<int>; a, b : array<int>) {
    dst |> push_from(a)
    dst |> push_from(b)
}

def append_field(var bag : Bag; a, b : array<int>) {
    bag.nums |> push_clone_from(a)
    bag.nums |> push_clone_from(b)
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

def shared_decls(src : array<int>) : int {
    var keys, vals : table<string; int>
    keys |> insert("a", 1)
    keys |> insert("b", 2)
    var w, z : array<int>
    w |> push_from(src)
    var sq, cubes : array<int>
    for (x in src) {
        sq |> push(x * x)
    }
    return length(keys) + length(vals) + length(w) + length(z) + length(sq) + length(cubes)
}
