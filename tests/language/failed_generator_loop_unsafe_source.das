//! a yielding generator loop over a call of an `[unsafe_operation]`
//! function needs `unsafe`, also when the function is
//! `[unsafe_outside_of_for]`
expect 31013

options gen2

[unsafe_operation, unsafe_outside_of_for]
def all_items(var items : array<int>) : iterator<int&> =>
    <- unsafe(each(items))

[export]
def main {
    var total = 0
    var gen <- generator<int> {
        var owner <- [1, 2, 3]
        for (v in all_items(owner)) {
            yield v
        }
        return false
    }
    for (v in gen) {
        total += v
    }
    print("{total}\n")
}
