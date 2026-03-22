options gen2
// PERF006: push/emplace in loop without prior reserve()
//
// Problem:
//   Calling push, push_clone, or emplace on an array inside a loop without
//   a preceding reserve() may trigger repeated reallocations as the array
//   grows — each reallocation copies all existing elements.
//
// Bad pattern:
//   var result : array<int>
//   for (i in range(1000)) {
//       result |> push(i)          // may realloc many times
//   }
//
// Good pattern:
//   var result : array<int>
//   result |> reserve(1000)
//   for (i in range(1000)) {
//       result |> push(i)          // no reallocs
//   }

expect 40217:7

require daslib/perf_lint

// --- Bad patterns (warnings expected) ---

def bad_push_for_loop() {
    var result : array<int>
    for (i in range(100)) {
        result |> push(i)                       // PERF006
    }
    delete result
}

def bad_emplace_while() {
    var result : array<int>
    var i = 0
    while (i < 100) {
        result |> emplace(i)                    // PERF006
        i++
    }
    delete result
}

def bad_push_clone() {
    var source <- [1, 2, 3]
    var dest : array<int>
    for (x in source) {
        dest |> push_clone(x)                   // PERF006
    }
    delete source
    delete dest
}

// Bad: field access chain without reserve
struct Container {
    items : array<int>
}

def bad_field_push(var c : Container) {
    for (i in range(100)) {
        c.items |> push(i)                      // PERF006
    }
}

// Bad: different field path — reserve on .a doesn't help .b
struct TwoArrays {
    a : array<int>
    b : array<int>
}

def bad_different_paths(var t : TwoArrays) {
    t.a |> reserve(100)
    for (i in range(100)) {
        t.b |> push(i)                          // PERF006 — .a reserved, not .b
    }
}

// Bad: deep field chain
struct Inner {
    data : array<int>
}
struct Outer {
    inner : Inner
}

def bad_deep_chain(var o : Outer) {
    for (i in range(100)) {
        o.inner.data |> push(i)                 // PERF006
    }
}

// Bad: unconditional push followed by an if (push is NOT inside the if)
def bad_push_before_if() {
    var result : array<int>
    for (i in range(100)) {
        result |> push(i)                       // PERF006 — not inside if
        if (i > 50) {
            pass
        }
    }
    delete result
}

// --- Good patterns (no warnings) ---

def good_with_reserve() {
    var result : array<int>
    result |> reserve(100)
    for (i in range(100)) {
        result |> push(i)                       // no warning — reserved
    }
    delete result
}

def good_field_reserved(var c : Container) {
    c.items |> reserve(100)
    for (i in range(100)) {
        c.items |> push(i)                      // no warning — field path reserved
    }
}

def good_same_path_reserved(var t : TwoArrays) {
    t.b |> reserve(100)
    for (i in range(100)) {
        t.b |> push(i)                          // no warning — same path reserved
    }
}

def good_local_array() {
    for (i in range(10)) {
        var local : array<int>
        local |> push(i)                        // no warning — local to loop
        delete local
    }
}

def good_no_loop() {
    var result : array<int>
    result |> push(42)                          // no warning — not in loop
    delete result
}

def good_conditional_push() {
    var result : array<int>
    for (i in range(100)) {
        if (i > 50) {
            result |> push(i)                   // no warning — conditional push
        }
    }
    delete result
}

def good_conditional_else() {
    var result : array<int>
    for (i in range(100)) {
        if (i > 50) {
            pass
        } else {
            result |> push(i)                   // no warning — else branch
        }
    }
    delete result
}

def good_conditional_nested() {
    var result : array<int>
    for (i in range(100)) {
        if (i > 10) {
            if (i < 90) {
                result |> push(i)               // no warning — nested if
            }
        }
    }
    delete result
}

[export]
def main() { }
