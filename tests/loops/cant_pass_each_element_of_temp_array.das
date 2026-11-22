//! an element each() yields from a temporary array is temporary,
//! const array or var: a plain array parameter rejects it
options gen2
expect 30341:2

require daslib/array_boost

def take(a : array<int>) : int {
    return length(a)
}

[export]
def test() {
    var arr <- [[1, 2], [3]]
    var total = 0
    array_view(arr, 0, 2) $(var view) {
        for (inner in each(view)) {
            total += take(inner)
        }
    }
    let frozen <- [[1, 2], [3]]
    array_view(frozen, 0, 2) $(view) {
        for (inner in each(view)) {
            total += take(inner)
        }
    }
}
