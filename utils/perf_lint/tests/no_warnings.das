options gen2
// No warnings test
//
// This file exercises all the patterns that perf_lint checks for, but uses
// the correct alternatives. It should produce zero performance warnings.

require daslib/perf_lint
require strings

def good_build_string() {
    let result = build_string() <| $(var writer) {
        for (i in range(100)) {
            write(writer, "item {i}\n")
        }
    }
}

def good_peek_data_iteration() {
    let s = "hello world"
    peek_data(s) <| $(arr) {
        var count = 0
        for (i in range(length(arr))) {
            if (int(arr[i]) == 'o') {
                count ++
            }
        }
    }
}

def good_modify_data() {
    var s = "hello"
    s = modify_data(s) <| $(var arr) {
        for (i in range(length(arr))) {
            arr[i] = uint8(int(arr[i]) - 32)
        }
    }
}

def good_cached_length_while() {
    let s = "hello world"
    let slen = length(s)
    var i = 0
    while (i < slen) {
        i ++
    }
}

def good_for_range_length() {
    let s = "test"
    for (i in range(length(s))) {
        pass
    }
}

def good_string_concat_outside_loop() {
    var s = "hello"
    s += " world"      // Not in a loop — no warning
}

def good_string_builder_no_self_ref() {
    var result = ""
    let prefix = "item"
    for (i in range(10)) {
        result = "{prefix}_{i}"   // Does not reference `result` — no PERF004
    }
}
