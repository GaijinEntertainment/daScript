// Treap benchmark — ported from dasProfile/tests/tree.das.
// Kotlin-style implicit treap; 1M ops (insert/erase/lookup) over a 10K-key
// universe. Original source:
// https://github.com/frol/completely-unscientific-benchmarks
options gen2
options persistent_heap = true
options solid_context

require daslib/random

struct Node {
    x, y : int
    left, right : Node?
}

var {
    seed : int4 = int4(1, 1, 1, 1)
}

[unsafe_deref, hint(alwaysinline, hot)]
def merge(var lower, greater : Node?; var result : Node?&) {
    if (lower == null) {
        result = greater
    } elif (greater == null) {
        result = lower
    } elif (lower.y < greater.y) {
        result = lower
        merge(lower.right, greater, lower.right)
    } else {
        result = greater
        merge(lower, greater.left, greater.left)
    }
}

[unsafe_deref, hint(alwaysinline, hot)]
def split(var orig : Node?; var lower : Node?&; var greaterOrEqual : Node?&; value : int) {
    if (orig == null) {
        lower = null
        greaterOrEqual = null
    } elif (orig.x < value) {
        lower = orig
        split(lower.right, lower.right, greaterOrEqual, value)
    } else {
        greaterOrEqual = orig
        split(greaterOrEqual.left, lower, greaterOrEqual.left, value)
    }
}

[unsafe_deref, hint(alwaysinline, hot)]
def hasValue(var orig : Node?&; x : int) : bool {
    var lower, equal, greater : Node?
    var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, x)
    split(equalOrGreater, equal, greater, x + 1)
    let res = equal != null
    var tmp : Node?
    merge(lower, equal, tmp)
    merge(tmp, greater, orig)
    return res
}

[unsafe_deref, hint(alwaysinline, hot)]
def insert(var orig : Node?&; x : int) {
    var lower, equal, greater : Node?
    var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, x)
    split(equalOrGreater, equal, greater, x + 1)
    if (equal == null) {
        equal = new Node(x = x, y = random_big_int(seed))
    }
    var tmp : Node?
    merge(lower, equal, tmp)
    merge(tmp, greater, orig)
}

[unsafe_deref, hint(alwaysinline, hot)]
def erase_raw(var orig : Node?&; x : int) {
    var lower, equal, greater : Node?
    var equalOrGreater : Node?
    split(orig, lower, equalOrGreater, x)
    split(equalOrGreater, equal, greater, x + 1)
    merge(lower, greater, orig)
    unsafe {
        delete equal
    }
}

def mainTreeNode {
    var tree : Node?
    var cur = 5
    var res = 0
    for (i in range(1, 1000000)) {
        let a = i % 3
        cur = (cur * 57 + 43) % 10007
        if (a == 0) {
            tree |> insert(cur)
        } elif (a == 1) {
            tree |> erase_raw(cur)
        } else {
            if (tree |> hasValue(cur)) {
                res ++
            }
        }
    }
    unsafe {
        delete tree
    }
    return res
}

[export]
def main {
    profile(10, "tree") {
        mainTreeNode()
    }
}
