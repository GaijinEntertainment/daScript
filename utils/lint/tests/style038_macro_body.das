options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _function_length = 10           // keep fixture functions small
// STYLE038 counts the lines the author wrote. A function annotation that swaps the
// body for a block built in its own file moves the body's location into that file;
// the rule does not measure across files.

expect 31209:1

require daslib/style_lint
require _style038_wrap_macro

// 3 lines here; the body the macro built ends past line 10 of _style038_wrap_macro.das
[wraps]
def wrapped_short() : int {
    return 42
}

// 12 physical lines -> over the limit of 10, macro or not
def plain_long(x : int) : int {                         // STYLE038
    var r = x
    r += 1
    r += 2
    r += 3
    r += 4
    r += 5
    r += 6
    r += 7
    r += 8
    return r
}

[export]
def main {
    print("{wrapped_short()} {plain_long(1)}\n")
}
