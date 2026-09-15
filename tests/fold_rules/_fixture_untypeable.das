options gen2
options indenting = 4

require _rules_untypeable

var g_seed = 1

[export]
def sub_two(x, y : int) : int {
    return x - y - g_seed
}
