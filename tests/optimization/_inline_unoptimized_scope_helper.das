options gen2
options optimize = false

module _inline_unoptimized_scope_helper public

require _inline_unsafe_helper

[inline]
def scoped_unoptimized_value() : int {
    var value : int
    with (module _inline_unsafe_helper) {
        value = auto_inline_visibility_private_value
    }
    return value
}

var public g_unoptimized_seed = 1

def public unoptimized_loop_sum() : int {
    var s = g_unoptimized_seed
    for (i in range(4)) {
        s += i
    }
    return s
}
