options gen2
// PERF014: closed-interval char-class range checks
//
// Problem:
//   Hand-written ranges like `c >= 'a' && c <= 'z'` reimplement the strings
//   module's is_alpha/is_alnum/is_number/is_white_space/etc. helpers. Using
//   the helper makes intent obvious and lets future codepoint widening
//   propagate automatically.
//
// Bad pattern:
//   if (c >= 'a' && c <= 'z') { ... }
//   if (c >= 48  && c <= 57)  { ... }       // '0'..'9' as raw int
//
// Good pattern:
//   if (is_alpha(c))   { ... }              // covers a-z, A-Z (and your locale)
//   if (is_number(c))  { ... }
//
// Hex extras ('a'..'f' / 'A'..'F') deliberately do NOT trigger — is_hex is
// broader and would change behavior for the digit half.

expect 31208:5

require daslib/perf_lint
require strings

// --- Bad patterns (PERF014) ---

def bad_lower_alpha(c : int) : bool {
    return c >= 'a' && c <= 'z'                // PERF014
}

def bad_upper_alpha(c : int) : bool {
    return c >= 'A' && c <= 'Z'                // PERF014
}

def bad_digits_char(c : int) : bool {
    return c >= '0' && c <= '9'                // PERF014
}

def bad_digits_int(c : int) : bool {
    return c >= 48 && c <= 57                  // PERF014 (raw int form)
}

def bad_yoda_form(c : int) : bool {
    return 'a' <= c && c <= 'z'                // PERF014 (mixed const-on-left)
}

// --- Good patterns (no warnings) ---

def good_is_alpha(c : int) : bool {
    return is_alpha(c)
}

def good_hex_lower(c : int) : bool {
    return c >= 'a' && c <= 'f'                // hex range — NOT flagged
}

def good_hex_upper(c : int) : bool {
    return c >= 'A' && c <= 'F'                // hex range — NOT flagged
}

def good_open_range(c : int) : bool {
    return c > '0' && c < '9'                  // open interval — NOT flagged
}

def good_partial_range(c : int) : bool {
    return c >= 'a' && c <= 'b'                // not a known class — NOT flagged
}

def good_different_vars(a, b : int) : bool {
    return a >= 'a' && b <= 'z'                // two different vars — NOT flagged
}

// Side-effectful subexpressions: replacing `f() >= 'a' && f() <= 'z'` with
// `is_alpha(f())` would call f() once instead of twice — silently changing
// semantics. The rule must NOT fire when operands aren't pure.

[unused_argument(c)]
def impure_helper(c : int) : int {
    print("called\n")                           // observable side effect
    return c
}

def good_impure_call(c : int) : bool {
    return impure_helper(c) >= 'a' && impure_helper(c) <= 'z'   // NOT flagged (impure)
}
