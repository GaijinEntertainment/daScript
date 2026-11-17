options gen2
options rtti

// _standalone_gc_free_fixture.das with options rtti - nothing here reads RTTI, the option keeps it

var g_counter = 0

[export]
def count_up : int {
    g_counter++
    return g_counter
}
