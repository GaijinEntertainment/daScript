options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _enable_default_off_rules = true   // fixture mode: the repo .lint_config keeps PERF031 off until the tree sweep
// PERF031: slice/chop on a loop-invariant string inside a loop
//
// Problem:
//   `slice(s, i, j)` and `chop(s, i, n)` call strlen on the WHOLE source string
//   on every call (das strings cache no length), and the result is a fresh heap
//   string. Slicing the same string per iteration is O(length * iterations).
//
// Bad pattern:
//   while (i < n) {
//       let cp = slice(text, i, j)   // strlen(text) per iteration -> O(n^2)
//   }
//
// Good pattern:
//   peek_data(text) $(d) {           // one strlen up front
//       ... d[i] ...                 // O(1) reads
//   }

expect 31208:4

require daslib/perf_lint
require strings

def bad_slice_for(text : string) : int {
    var total = 0
    for (i in range(10)) {
        total += length(slice(text, i, i + 1))      // PERF031
    }
    return total
}

def bad_slice_while(text : string) : int {
    var total = 0
    var i = 0
    while (i < 8) {
        total += length(slice(text, i, i + 2))      // PERF031 (the shape that hid in the SPM tokenizer)
        i++
    }
    return total
}

def bad_chop_for(text : string) : int {
    var total = 0
    for (i in range(10)) {
        total += length(chop(text, i, 2))           // PERF031: chop shares the per-call strlen
    }
    return total
}

def bad_slice_in_block(text : string) : int {
    var total = 0
    for (_i in range(4)) {
        peek_data(text) $(d : array<uint8> const#) {
            total += length(slice(text, 0, length(d) % 4 + 1))   // PERF031: block args run inline in the loop
        }
    }
    return total
}

def good_slice_of_loop_var(parts : array<string>) : int {
    var total = 0
    for (p in parts) {
        total += length(slice(p, 0, 1))             // bounded by the element, not the invariant source
    }
    return total
}

def good_hoisted(text : string) : int {
    let head = slice(text, 0, 4)
    var total = 0
    for (_i in range(10)) {
        total += length(head)
    }
    return total
}

def good_no_loop(text : string) : int {
    return length(slice(text, 1, 3))
}
