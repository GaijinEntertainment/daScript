options gen2
options indenting = 4

require _rules_cycle

var g_seed = 1

[export]
def add_two(x, y : int) : int {
    return x + y + g_seed
}
