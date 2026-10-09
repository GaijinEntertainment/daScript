options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// The defer-pipe scan (STYLE001/STYLE002) reads the lines the author wrote. A function
// annotation that swaps the body for a block built in its own file moves the body's
// location into that file; the scan still reads this function's own lines, and never
// this file's lines at the macro body's line numbers - the decoy strings below sit at them.

expect 31209:1

require daslib/defer
require daslib/style_lint
require _style038_wrap_macro

[wraps]
def wrapped_defer_pipe() : int {
    var x = 1
    defer <| $() {                                      // STYLE002
        x = 0
    }
    return x
}

let style001_decoy_padding_a = 1
let style001_decoy_padding_b = 2
let style001_decoy_padding_c = 3
let style001_decoy_padding_d = 4
let style001_decoy_padding_e = 5
let style001_decoy_padding_f = 6
let style001_decoy_padding_g = 7
let style001_decoy_padding_h = 8
let style001_decoy_padding_i = 9
let style001_decoy_padding_j = 10
let style001_decoy_padding_k = 11
let style001_decoy_padding_l = 12
let style001_decoy_padding_m = 13
let style001_decoy_a = " defer <| $() pass "
let style001_decoy_b = " defer <| $() pass "
let style001_decoy_c = " defer <| $() pass "
let style001_decoy_d = " defer <| $() pass "

[export]
def main {
    print("{wrapped_defer_pipe()}{style001_decoy_a}{style001_decoy_b}{style001_decoy_c}{style001_decoy_d}\n")
    print("{style001_decoy_padding_a + style001_decoy_padding_b + style001_decoy_padding_c + style001_decoy_padding_d}\n")
    print("{style001_decoy_padding_e + style001_decoy_padding_f + style001_decoy_padding_g + style001_decoy_padding_h}\n")
    print("{style001_decoy_padding_i + style001_decoy_padding_j + style001_decoy_padding_k + style001_decoy_padding_l + style001_decoy_padding_m}\n")
}
