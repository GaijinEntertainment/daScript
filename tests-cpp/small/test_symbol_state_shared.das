options gen2
options indenting = 4

module test_symbol_state_shared shared

[never_inline, sideeffects]
def used_by_a : int {
    return 1
}

[never_inline, sideeffects]
def used_by_b : int {
    return 2
}
