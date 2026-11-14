//! an `[inline]` call under a node the inliner cannot host - here
//! the target of a computed `goto` - is a compile error naming the
//! node, never a call left in place
options gen2
expect 30100

var g_label_offset = 1

[inline]
def private label_of(k : int) : int => k + g_label_offset

[export]
def jump_to_computed_label(k : int) : int {
    var res = 0
    goto label_of(k)
    label 1:
    res = 10
    label 2:
    res += 1
    return res
}
