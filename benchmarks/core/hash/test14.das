options gen2
options persistent_heap

// Large (open-addressed) string-keyed builtin table vs an int reference, same SSIZE and ops.
// All builtin-table find/at are inlined under JIT now (non-string, packed-string and large-string
// keys); this sizes the large-string path against the int reference to confirm the rewrite holds.
// get_sum uses the block-callback get() so the lookup is NOT dead-code-eliminated — a plain
// sum/key_exists whose result is unused folds to 0 ns under JIT (see test08 vs test09). The
// _fresh lane queries with clone_string copies so the find runs a real strcmp behind the 32-bit
// hashKey pre-filter (the realistic case: a key from I/O is never the stored pointer); _same
// reuses the stored pointers and hits KeyCompare's a==b shortcut, isolating the strcmp cost.

require dastest/testing_boost

require _common

let SSIZE = TOTAL_RANDOM_NUMBERS

struct Adder {
    val : uint64
}

def make_keys(nums : array<int>) : array<string> {
    return <- [for (n in nums); "key_{n}"]
}

def clone_all(keys : array<string>) : array<string> {
    return <- [for (k in keys); clone_string(k)]
}

[sideeffects]
def fill_int(var tab : table<int; int>; nums : array<int>) {
    for (n in nums) {
        unsafe(tab[n]) = -n
    }
}

[sideeffects]
def fill_str(var tab : table<string; int>; keys : array<string>; nums : array<int>) {
    for (k, n in keys, nums) {
        unsafe(tab[k]) = -n
    }
}

[sideeffects]
def sum_int(tab : table<int; int>; nums : array<int>) {
    var adder : Adder
    for (n in nums) {
        get(tab, n) $(val) {
            adder.val += uint64(val)
        }
    }
}

[sideeffects]
def sum_str(tab : table<string; int>; qkeys : array<string>) {
    var adder : Adder
    for (k in qkeys) {
        get(tab, k) $(val) {
            adder.val += uint64(val)
        }
    }
}

[benchmark]
def builtin_table_int(b : B?) {
    let nums <- generate_unique_positive_random_numbers()
    b |> run("insert/{SSIZE}", SSIZE) {
        var tab : table<int; int>
        fill_int(tab, nums)
    }
    var rtab : table<int; int>
    fill_int(rtab, nums)
    b |> run("get_sum/{SSIZE}", SSIZE) {
        sum_int(rtab, nums)
    }
}

[benchmark]
def builtin_table_string(b : B?) {
    let nums <- generate_unique_positive_random_numbers()
    let keys <- make_keys(nums)
    let qkeys <- clone_all(keys)
    b |> run("insert/{SSIZE}", SSIZE) {
        var tab : table<string; int>
        fill_str(tab, keys, nums)
    }
    var rtab : table<string; int>
    fill_str(rtab, keys, nums)
    b |> run("get_sum_fresh/{SSIZE}", SSIZE) {
        sum_str(rtab, qkeys)
    }
    b |> run("get_sum_same/{SSIZE}", SSIZE) {
        sum_str(rtab, keys)
    }
}
