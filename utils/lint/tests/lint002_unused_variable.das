options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// LINT002: a local variable that is declared and never read
//
// Problem: a local nothing reads is either a leftover from an edit or
// a value the author meant to use; either way the compiler keeps it
// silently.
//
// Bad pattern:
//   let count = length(items)      // nothing reads count
//
// Good pattern:
// read it, delete it, or name it with a leading underscore when the
// binding is deliberate.

expect 50503:2

require daslib/lint

// bad: declared, never read (1 warning)
def bad_unread_let() {
    let count = 1
}

// bad: a block variable nothing reads (1 warning)
def bad_unread_in_block(items : array<int>) {
    for (item in items) {
        let doubled = item * 2
    }
}

// good: read
def good_read() : int {
    let count = 1
    return count + 1
}

// good: a deliberate binding carries the underscore
def good_underscore(items : array<int>) : int {
    var seen = 0
    for (_item in items) {
        seen++
    }
    return seen
}

[export]
def main {
    bad_unread_let()
    let items <- [1, 2, 3]
    bad_unread_in_block(items)
    print("{good_read()} {good_underscore(items)}\n")
}
