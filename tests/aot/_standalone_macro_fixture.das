options gen2

require _standalone_macro_module

// 5050 is summed at compile time; the runtime only ever sees the number.
[export]
def macro_baked_value : int {
    return bake_sum(100)
}
