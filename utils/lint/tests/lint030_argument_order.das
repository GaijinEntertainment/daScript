options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _lint = "LINT030"   // the rule ships default-off; the fixture arms it
// LINT030: a call whose argument list reads a variable that a nested call in another
// argument writes by reference. The order arguments are evaluated in is not defined - the
// interpreter and the JIT go left to right, the AOT C++ goes whichever way the compiler
// does - so the read sees the value before or after the write by the tier's choice.

expect 50503:5

require daslib/lint

def bump(var x : int&) : int {
    x++
    return x
}

def fill(var s : string&) : int {
    s = "filled"
    return 0
}

def grow(var xs : array<int>) : int {
    xs |> push(1)
    return length(xs)
}

def pair(a, b : int) : int {
    return a * 10 + b
}

def report(rc : int; text : string) : bool {
    return rc == 0 && !empty(text)
}

// === Positives - a sibling argument reads what the nested call writes ===

def bad_read_after(var x : int) : int {
    return pair(bump(x), x)   // LINT030
}

def bad_read_before(var x : int) : int {
    return pair(x, bump(x))   // LINT030
}

def bad_out_parameter() : bool {
    var out : string
    return report(fill(out), out)   // LINT030
}

def bad_read_in_expression(var x : int) : int {
    return pair(bump(x), x + 1)   // LINT030
}

def bad_array(var xs : array<int>) : int {
    return pair(grow(xs), length(xs))   // LINT030
}

// === Negatives ===

// the write is a statement of its own, the read comes after it
def good_sequenced(var x : int) : int {
    let bumped = bump(x)
    return pair(bumped, x)
}

// every argument only reads
def good_read_only(x : int) : int {
    return pair(pair(x, x), x)
}

// the nested call writes one variable, the sibling reads another
def good_other_variable(var x : int; y : int) : int {
    return pair(bump(x), y)
}

// a lambda argument runs later, not while the arguments are evaluated
def good_lambda_reads_later(var x : int) : int {
    let read = @(y : int) : int { return y }
    return pair(bump(x), invoke(read, 0))
}
