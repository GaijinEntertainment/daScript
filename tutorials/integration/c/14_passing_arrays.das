options gen2

// Companion script for the "Passing Arrays from C" tutorial.
// The C host constructs `array<int>` and `table<int; int>` values via the new
// das_array / das_table C API and hands them to these functions. The functions
// read or mutate them in place; results flow back through the same struct.

// Sum the elements of a borrowed array. The `array<int>#` type is daslang's
// "temporary" array — it cannot be retained or extended, so the runtime
// will refuse any growth attempt. Matches the C side's das_array_init_borrowed.
[export]
def sum_array(arr : array<int>#) : int {
    var total = 0
    for (v in arr) {
        total += v
    }
    return total
}

// Fill `arr` with the first n perfect squares (0, 1, 4, 9, ...). Resizing
// from C-side empty to n elements drives growth on the context heap.
[export]
def fill_squares(var arr : array<int>; n : int) {
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = i * i
    }
}

// Count occurrences of each value in `data`, accumulating into `counts`.
// `data` is a borrowed view (array<int>#); `counts` is a mutable owned table.
[export]
def count_values(var counts : table<int; int>; data : array<int>#) {
    for (v in data) {
        if (key_exists(counts, v)) {
            counts[v] = counts[v] + 1
        } else {
            counts[v] = 1
        }
    }
}
