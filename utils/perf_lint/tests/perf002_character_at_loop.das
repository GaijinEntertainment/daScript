options gen2
expect 40217:5
// PERF002: character_at in loop with loop variable as index
//
// Problem:
//   `character_at(s, i)` is O(n) per call because it calls strlen internally
//   to bounds-check the index. In a loop iterating over string indices, this
//   becomes O(n^2) total.
//
// Bad pattern:
//   for (i in range(length(s))) {
//       let ch = character_at(s, i)   // O(n) per call, O(n^2) total
//   }
//
// Good pattern:
//   peek_data(s) <| $(arr) {
//       for (i in range(length(arr))) {
//           let ch = int(arr[i])       // O(1) per access
//       }
//   }

require daslib/perf_lint
require strings

def bad_character_at_for() {
    let s = "hello world"
    for (i in range(length(s))) {
        let ch = character_at(s, i)     // PERF002
    }
}

def bad_character_at_nested() {
    let s = "hello"
    for (i in range(length(s))) {
        for (j in range(length(s))) {
            let ch = character_at(s, i) // PERF002 (i is a loop var)
        }
    }
}

def bad_character_at_expr() {
    let s = "hello world"
    for (i in range(length(s))) {
        let ch = character_at(s, i + 1) // PERF002 (loop var nested in expression)
    }
}

def bad_character_at_mul() : int {
    let s = "hello world"
    var total = 0
    for (i in range(length(s))) {
        total += character_at(s, i * 2) // PERF002 (loop var in expression)
    }
    return total
}

// --- Good patterns (no warnings from PERF002) ---

def good_peek_data() {
    let s = "hello world"
    peek_data(s) <| $(arr) {
        for (i in range(length(arr))) {
            let ch = int(arr[i])        // O(1)
        }
    }
}

def good_character_at_non_loop_var() : int {
    let s = "hello world"
    var idx = 3
    for (i in range(length(s))) {
        idx = character_at(s, idx)      // idx is not a loop variable — no PERF002
    }
    return idx
}
