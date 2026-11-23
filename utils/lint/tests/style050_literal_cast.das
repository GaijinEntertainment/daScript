//! STYLE050 is reported for: a numeric cast of an integer literal
//! that has a literal spelling.
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options infer_time_folding = false   // dastest otherwise folds the cast before the lint pass

expect 31209:9

require daslib/style_lint

// --- Bad patterns (must fire - 9 total) ---

def bad_float(x : float) : float => x * float(2)        // STYLE050 - 2.0

def bad_negative_float(x : float) : float => x + float(-2)  // STYLE050 - -2.0

def bad_double(x : double) : double => x * double(7)    // STYLE050 - 7.0lf

def bad_uint(x : uint) : uint => x & uint(3)            // STYLE050 - 3u

def bad_int64(x : int64) : int64 => x + int64(5)        // STYLE050 - 5l

def bad_uint64(x : uint64) : uint64 => x + uint64(6)    // STYLE050 - 6ul

def bad_float_bound() : float => float(16777216)        // STYLE050 - 16777216.0

def bad_negative_double(x : double) : double => x + double(-7)  // STYLE050 - -7.0lf

def bad_negative_int64(x : int64) : int64 => x + int64(-5)      // STYLE050 - -5l

// --- Good patterns (must not fire) ---

def good_literals(x : float) : float => x * 2.0 + float(3u) + -2.0

def good_double_literal(x : double) : double => x * 7.0lf

def good_uint_literal(x : uint) : uint => x & 3u

def good_int64_literal(x : int64) : int64 => x + 5l

def good_uint64_literal(x : uint64) : uint64 => x + 6ul

def good_runtime_cast(n : int) : float => float(n)

def good_not_exact_in_float() : float => float(16777217) + float(-16777217)

def good_wraps_below_zero() : uint => uint(-1)

def good_digit_separators() : float => float(1_000)

def good_negative_zero() : float => float(-0)

def good_char_source() : float => float('a')

def good_no_literal_form() : int16 => int16(3)

def good_type_variable(x : auto(TT)) : TT => x * TT(2)

[export]
def main {
    print("{bad_float_bound()} {bad_negative_double(1.0lf)} {bad_negative_int64(1l)}\n")
    print("{bad_float(1.0)} {bad_negative_float(1.0)} {bad_double(1.0lf)} {bad_uint(1u)}\n")
    print("{bad_int64(1l)} {bad_uint64(1ul)}\n")
    print("{good_double_literal(1.0lf)} {good_uint_literal(1u)} {good_int64_literal(1l)}\n")
    print("{good_uint64_literal(1ul)}\n")
    print("{good_literals(1.0)} {good_runtime_cast(1)} {good_not_exact_in_float()}\n")
    print("{good_wraps_below_zero()} {good_digit_separators()} {good_negative_zero()}\n")
    print("{good_char_source()} {good_no_literal_form()} {good_type_variable(1.5)}\n")
}
