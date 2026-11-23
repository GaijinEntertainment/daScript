//! STYLE051 is reported for: a uint8 cast of an integer literal,
//! which has the literal spelling Nu8.
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options infer_time_folding = false   // dastest otherwise folds the cast before the lint pass

expect 31209:3

require daslib/style_lint

// --- Bad patterns (must fire - 3 total) ---

def bad_uint8() : uint8 => uint8(200)                   // STYLE051 - 200u8

def bad_pair(x : uint8) : bool => x == uint8(0) || x == uint8(255)  // STYLE051 twice

// --- Good patterns (must not fire) ---

def good_literal() : uint8 => 200u8

def good_does_not_fit() : uint8 => uint8(300)

def good_wraps_below_zero() : uint8 => uint8(-1)

def good_runtime_cast(n : int) : uint8 => uint8(n)

def good_digit_separators() : uint8 => uint8(1_0)

def good_char_source() : uint8 => uint8('a')

[export]
def main {
    print("{bad_uint8()} {bad_pair(1u8)} {good_literal()} {good_does_not_fit()}\n")
    print("{good_wraps_below_zero()} {good_runtime_cast(1)} {good_digit_separators()}\n")
    print("{good_char_source()}\n")
}
