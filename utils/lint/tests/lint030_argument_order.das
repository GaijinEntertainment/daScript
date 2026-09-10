options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _lint = "LINT030"   // the rule ships default-off; the fixture arms it
// LINT030: a call whose argument list reads a variable that a nested call in another
// argument writes by reference. The order arguments are evaluated in is not defined and
// differs between tiers, so the read sees the value before or after the write by the tier's
// choice.

expect 50503:7

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

def take_ref(s : string&; rc : int) : int {
    return rc + length(s)
}

def bump_copy(var x : int) : int {
    x++
    return x
}

def call_block(blk : block<(q : int) : int>) : int {
    return invoke(blk, 1)
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

// the writing call sits inside an expression, not at the argument's root
def bad_writer_in_expression(var x : int) : int {
    return pair(1 + bump(x), x)   // LINT030
}

// a block handed to a nested call runs while that argument is evaluated - its reads are the
// argument's, unlike a block the outer call itself takes (below), which runs after every argument
def bad_block_invoked_by_sibling(var x : int) : int {
    return pair(bump(x), call_block($(q : int) : int { return x + q }))   // LINT030
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

// the outer call takes the written variable by reference - the callee sees its final state
// whatever the order
def good_outer_takes_by_ref() : int {
    var out : string
    return take_ref(out, fill(out))
}

// the nested call writes its own copy - a by-value parameter reaches nothing of the caller's
def good_by_value_parameter(var x : int) : int {
    return pair(bump_copy(x), x)
}

// a lambda argument runs later, not while the arguments are evaluated
def good_lambda_reads_later(var x : int) : int {
    let read = @(y : int) : int { return y }
    return pair(bump(x), invoke(read, 0))
}

// a marked site is the author's call
def good_marked(var x : int) : int {
    return pair(bump(x), x)   // nolint:LINT030
}

// a call inside a block argument's body runs later too - its reads stay inside the block
def good_block_call_reads_later(var x : int) : int {
    var seen : int
    pair(bump(x), 0) |> with_value() $(v : int) {
        seen = pair(x, v)
    }
    return seen
}

def with_value(v : int; blk : block<(v : int) : void>) : int {
    invoke(blk, v)
    return v
}
