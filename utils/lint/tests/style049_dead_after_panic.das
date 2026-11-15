//! STYLE049 (armed by _enable_default_off_rules) is reported for: a
//! statement after a call that never returns, such as panic.
options gen2
options _enable_default_off_rules = true
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them

expect 31209:3

require daslib/style_lint

// --- Bad patterns (must fire - 3 total) ---

def bad_return_after_panic(x : int) : int {
    return 1 if (x > 0)
    panic("bad_return_after_panic: {x}")
    return 0                                // STYLE049
}

def bad_statement_after_panic(var x : int) {
    if (x < 0) {
        panic("negative")
        x = 0                               // STYLE049
    }
}

def bad_reports_first_only(var x : int) {
    panic("always")
    x ++                                    // STYLE049 - only the first dead statement
    x ++
}

// --- Good patterns (must not fire) ---

def good_panic_ends_block(x : int) : int {
    return 1 if (x > 0)
    panic("good_panic_ends_block: {x}")
}

def good_panic_in_branch(var x : int) {
    if (x < 0) {
        panic("negative")
    }
    x ++
}

[export]
def main {
    var x = 1
    bad_statement_after_panic(x)
    good_panic_in_branch(x)
    print("{bad_return_after_panic(1)} {good_panic_ends_block(1)}\n")
}
