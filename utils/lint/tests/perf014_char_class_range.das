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
//   if (c < '0' || c > '9')   { ... }       // negated / out-of-range form
//
// Good pattern:
//   if (is_alpha(c))   { ... }              // covers a-z, A-Z (and your locale)
//   if (is_number(c))  { ... }
//   if (!is_number(c)) { ... }              // negated form
//
// Hex extras ('a'..'f' / 'A'..'F') deliberately do NOT trigger — is_hex is
// broader and would change behavior for the digit half.

expect 31208:9

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

// Negated (out-of-range) form: c < lo || c > hi is the De Morgan complement
// of the closed in-range check, so it maps to the negated helper.

def bad_neg_digits(c : int) : bool {
    return c < '0' || c > '9'                  // PERF014 (negated → !is_number)
}

def bad_neg_lower_alpha(c : int) : bool {
    return c < 'a' || c > 'z'                  // PERF014 (negated → !is_alpha lower)
}

def bad_neg_yoda(c : int) : bool {
    return '0' > c || '9' < c                  // PERF014 (negated, const-on-left)
}

def bad_neg_cast(c : uint8) : bool {
    return int(c) < '0' || int(c) > '9'        // PERF014 (negated, with int() cast)
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
    return c > '0' && c < '9'                  // open interval (&&-intersection) — NOT flagged
}

def good_neg_always_true(c : int) : bool {
    return c < '9' || c > '0'                  // degenerate always-true (lo>=hi) — NOT flagged
}

def good_neg_hex(c : int) : bool {
    return c < 'a' || c > 'f'                  // hex complement — NOT flagged (is_hex broader)
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

def good_neg_impure(c : int) : bool {
    return impure_helper(c) < 'a' || impure_helper(c) > 'z'     // NOT flagged (impure, negated)
}
