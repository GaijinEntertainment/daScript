options gen2
options persistent_heap

// Benchmark: get_key(tab, v) vs keys(tab)+values(tab) for table iteration.
// get_key computes key from value address via O(1) pointer arithmetic,
// avoiding the overhead of a second iterator.

require dastest/testing_boost

def fill_table(var tab : table<int; int>; n : int) {
    for (i in range(n)) {
        tab[i] = i * 3
    }
}

let TABLE_SIZE_SMALL = 1000
let TABLE_SIZE_MEDIUM = 10000
let TABLE_SIZE_LARGE = 100000

// --- keys + values (two iterators) ---

[sideeffects]
def iterate_keys_values(tab : table<int; int>) : int {
    var sum = 0
    for (k, v in keys(tab), values(tab)) {
        sum += k + v
    }
    return sum
}

// --- values + get_key (one iterator) ---

[sideeffects]
def iterate_get_key(tab : table<int; int>) : int {
    var sum = 0
    for (v in values(tab)) {
        let k = get_key(tab, v)
        sum += k + v
    }
    return sum
}

[benchmark]
def keys_values_small(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_SMALL)
    b |> run("keys+values/{TABLE_SIZE_SMALL}", TABLE_SIZE_SMALL) {
        iterate_keys_values(tab)
    }
}

[benchmark]
def get_key_small(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_SMALL)
    b |> run("get_key/{TABLE_SIZE_SMALL}", TABLE_SIZE_SMALL) {
        iterate_get_key(tab)
    }
}

[benchmark]
def keys_values_medium(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_MEDIUM)
    b |> run("keys+values/{TABLE_SIZE_MEDIUM}", TABLE_SIZE_MEDIUM) {
        iterate_keys_values(tab)
    }
}

[benchmark]
def get_key_medium(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_MEDIUM)
    b |> run("get_key/{TABLE_SIZE_MEDIUM}", TABLE_SIZE_MEDIUM) {
        iterate_get_key(tab)
    }
}

[benchmark]
def keys_values_large(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_LARGE)
    b |> run("keys+values/{TABLE_SIZE_LARGE}", TABLE_SIZE_LARGE) {
        iterate_keys_values(tab)
    }
}

[benchmark]
def get_key_large(b : B?) {
    var tab : table<int; int>
    fill_table(tab, TABLE_SIZE_LARGE)
    b |> run("get_key/{TABLE_SIZE_LARGE}", TABLE_SIZE_LARGE) {
        iterate_get_key(tab)
    }
}
