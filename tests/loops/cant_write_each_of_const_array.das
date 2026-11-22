//! writing an element of a const dynamic, temp or fixed array through
//! the loop variable of each() does not compile
options gen2
expect 30952:6

require daslib/array_boost

struct Holder {
    items : array<int>
}

def write_argument(arr : array<int>) {
    for (x in each(arr)) {
        x = 7
    }
}

def write_field(holder : Holder) {
    for (x in each(holder.items)) {
        x = 7
    }
}

[export]
def test() {
    let dynamic <- [10, 20, 30]
    for (x in each(dynamic)) {
        x = 7
    }
    let nested <- [[1, 2], [3]]
    for (inner in each(nested)) {
        inner[0] = 7
    }
    let fixed = fixed_array(10, 20, 30)
    for (x in each(fixed)) {
        x = 7
    }
    array_view(dynamic, 0, 3) $(view) {
        for (x in each(view)) {
            x = 7
        }
    }
    write_argument(dynamic)
    var holder = Holder(items <- [1, 2])
    write_field(holder)
}
