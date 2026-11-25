options gen2
options persistent_heap

require dastest/testing_boost
require strings
require daslib/strings_boost
require daslib/md_boost
require daslib/fio
require daslib/random

// temp-string reclaim: [temp_string_result] calls consumed directly by a non-capturing call
// are wrapped in _temp_string_result and ride the 1-slot dispose queue (persistent heap only).
// Also covers the single-site-per-call soundness rule for the older builder temp marking.

def make_str(x : int) : string => string(x)  // return position - never wrapped

// [never_inline] pins the propagation path - auto-inline would splice the body and the
// call site would reclaim via the direct-arg rule instead of the propagated flag

[never_inline]
def wrap_conv(x : int) : string => string(x)  // every return is a fresh producer -> flag propagates

[never_inline]
def wrap_pick(c : bool; x : int; y : int) : string =>
    c ? to_upper("v{x}") : string(y)  // ternary of fresh producers qualifies

[never_inline]
def wrap_pass(s : string) : string => s  // passthrough - must NOT be flagged

[test(description = "string temporaries consumed directly by a call, alone or chained, are reclaimed so the string heap stays flat over a long loop")]
def test_temp_string_reclaim_flatness(t : T?) {
    t |> run("conversion consumed in a call is reclaimed") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(100000)) {
            summ += length(string(i))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        // only the final parked temp may remain
        t |> success(int64(after) - int64(before) < 256l, "string heap should stay flat")
    }

    t |> run("flagged-call chain is reclaimed link by link") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(50000)) {
            summ += length(to_upper(strip("  padded{i}  ")))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 256l, "chain temps should all be reclaimed")
    }
}

[test(description = "reclaiming temporaries keeps chain values correct, queues only the rightmost flagged argument, protects sibling sites and capturing consumers, and never wraps a return-position conversion")]
def test_temp_string_reclaim_correctness(t : T?) {
    t |> run("chain values stay correct") @@(t) {
        for (i in range(100)) {
            let expected_len = length("PADDED{i}")
            t |> equal(length(to_upper(strip("  padded{i}  "))), expected_len)
        }
    }

    t |> run("two flagged args - only the rightmost is queued") @@(t) {
        for (i in range(10000)) {
            let a = 100000 + i
            let b = 900000 + i
            // if string(a) were also queued, string(b)'s flush would free it mid-call
            t |> success(compare_ignore_case(string(a), string(b)) != 0)
        }
    }

    t |> run("nested builder cannot flush a sibling site (the compare/to_upper corruption)") @@(t) {
        for (i in range(10000)) {
            let a = 100000 + (i % 80000)
            let b = 300000 + (i % 70000)
            // regression: the old rightmost-builder rule freed "{a}" mid-call via the nested
            // builder's flush under right-to-left extern arg evaluation - equal on every iteration
            t |> success(compare_ignore_case(to_upper("{b}"), "{a}") != 0)
        }
    }

    t |> run("return-position conversion is not wrapped") @@(t) {
        let keep = make_str(424242)
        var churn = 0
        for (i in range(1000)) {
            churn += length(string(7777000 + i))
        }
        t |> success(churn > 0)
        t |> equal(keep, "424242")
    }

    t |> run("capturing consumers keep their strings") @@(t) {
        let tab <- {for (i in range(100)); string(i) => i}
        let arr <- [for (i in range(100)); string(1000000 + i)]
        var churn = 0
        for (i in range(1000)) {
            churn += length(string(8888000 + i))
        }
        t |> success(churn > 0)
        for (i in range(100)) {
            let expected_key = string(i)
            t |> success(key_exists(tab, expected_key), "table keys must survive temp churn")
            let expected_elem = string(1000000 + i)
            t |> equal(arr[i], expected_elem)
        }
    }

    t |> run("wrapper passthrough on non-heap strings is a no-op") @@(t) {
        t |> equal(_temp_string_result("literal"), "literal")
    }
}

[test(description = "a das function with fresh returns, or a ternary of fresh producers, is reclaimed transitively while a passthrough function is not flagged and its source survives")]
def test_temp_string_reclaim_propagation(t : T?) {
    t |> run("das function with fresh returns is reclaimed transitively") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(50000)) {
            summ += length(wrap_conv(i))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 256l, "propagated flag should reclaim like the extern")
    }

    t |> run("ternary of fresh producers qualifies") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(50000)) {
            summ += length(wrap_pick((i & 1) == 0, i, i))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 256l, "ternary branches are both fresh")
        for (i in range(20)) {
            let expected_len = (i & 1) == 0 ? length("V{i}") : length("{i}")
            t |> equal(length(wrap_pick((i & 1) == 0, i, i)), expected_len)
        }
    }

    t |> run("passthrough function is NOT flagged - source survives") @@(t) {
        let keep = make_str(555555)
        var churn = 0
        for (_i in range(1000)) {
            // if wrap_pass were wrongly flagged, this would queue keep's own pointer
            // and the churn below would reuse its cell
            churn += length(wrap_pass(keep))
        }
        for (i in range(1000)) {
            churn += length(string(9999000 + i))
        }
        t |> success(churn > 0)
        t |> equal(keep, "555555")
    }
}

[test(description = "a passthrough callee does not launder a queued slice temporary, whether passed directly or held in a let-local")]
def test_temp_string_reclaim_passthrough_alias(t : T?) {
    t |> run("passthrough callee must not launder a queued slice temp (direct-arg)") @@(t) {
        var stored : array<string>
        stored |> reserve(6)
        for (i in range(6)) {
            let ln = "require mod_{i}/sub_{i}"
            // wrap_pass returns its own argument; if slice's temp is queued here,
            // stored aliases a freed cell
            stored |> push(wrap_pass(slice(ln, length("require "))))
        }
        // same-size churn: the persistent shoe reissues a freed cell to the next
        // allocation of the same size class, so the corruption is deterministic
        var churn = 0
        for (i in range(50)) {
            churn += length(wrap_pass(slice("require xod_{i % 10}/yub_{i % 10}", 8)))
        }
        t |> success(churn > 0)
        for (i in range(6)) {
            let expected_elem = "mod_{i}/sub_{i}"
            t |> equal(stored[i], expected_elem)
        }
    }

    t |> run("passthrough callee must not launder a wrapped let-local slice (let-form)") @@(t) {
        var stored : array<string>
        stored |> reserve(6)
        for (i in range(6)) {
            let ln = "require mod_{i}/sub_{i}"
            let sliced = slice(ln, 8)
            let passed = wrap_pass(sliced)      // "safe" use of sliced that aliases it
            stored |> push(passed)
        }
        var churn = 0
        for (i in range(50)) {
            churn += length(wrap_pass(slice("require xod_{i % 10}/yub_{i % 10}", 8)))
        }
        t |> success(churn > 0)
        for (i in range(6)) {
            let expected_elem = "mod_{i}/sub_{i}"
            t |> equal(stored[i], expected_elem)
        }
    }
}

[test(description = "let-local conversions are reclaimed, builder let-locals are marked rather than wrapped, adjacent let-locals do not wrap the earlier one and an escaping let-local is not wrapped")]
def test_temp_string_reclaim_let_local(t : T?) {
    t |> run("f2s shape: let-local conversion is reclaimed") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(100000)) {
            let str = string(i)
            summ += length(str)
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 256l, "let-local temps should be reclaimed")
    }

    t |> run("builder let-local is marked, not wrapped - reclaimed for free") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(100000)) {
            let str = "v{i}"
            summ += length(str)
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 256l, "builder let-local temps should be reclaimed")
    }

    t |> run("adjacent let-locals: the earlier one must not be wrapped") @@(t) {
        for (i in range(2000)) {
            let a = string(100000 + i)
            let b = string(500000 + i)
            // b's wrapped let is a queue site between a's init and a's use,
            // so a must stay unwrapped or its cell would be flushed mid-range
            t |> success(compare_ignore_case(a, b) != 0)
        }
    }

    t |> run("escaping let-local is not wrapped") @@(t) {
        var arr : array<string>
        arr |> reserve(100)
        for (i in range(100)) {
            let s = string(3000000 + i)
            arr |> push(s)
        }
        var churn = 0
        for (i in range(1000)) {
            churn += length(string(4444000 + i))
        }
        t |> success(churn > 0)
        for (i in range(100)) {
            let expected_elem = string(3000000 + i)
            t |> equal(arr[i], expected_elem)
        }
    }
}

// Sweep-batch coverage for [temp_string_result]: flatness rows per flagged family, and
// row-(b) passthrough traps - identity-shaped inputs whose freshness rides an argument
// precondition; a future `return input` shortcut in the impl fails these loudly.
//
// Rows measure RESULT reclaim - the arc's contract. Two shapes deliberately NOT asserted
// flat: a call with TWO builder args queues only the rightmost (one site per call, arg
// order is unspecified for externs), and a builder passed to a string-returning das
// function is never temp-marked (such functions are conservatively captureString).

[test(description = "fio path helpers, sprint, fmt and propagated daslib producers reclaim their temporaries so the string heap stays flat in a loop")]
def test_sweep_flag_flatness(t : T?) {
    t |> run("fio path helpers reclaim in a loop") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(20000)) {
            summ += length(base_name("/tmp/dir{i}/file.txt"))
            summ += length(dir_name("/tmp/dir{i}/file.txt"))
            summ += length(path_join("root", "leaf{i}.das"))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 512l, "path helper temps should be reclaimed")
    }

    t |> run("sprint and fmt reclaim in a loop") @@(t) {
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(20000)) {
            summ += length(sprint(i, print_flags.singleLine))
            summ += length(fmt(":d", i)) // fmt wraps its arg in {} - fmtlib syntax, not printf
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 512l, "sprint/fmt temps should be reclaimed")
    }

    t |> run("propagated daslib producers reclaim in a loop") @@(t) {
        let parts <- [ "alpha", "beta", "gamma" ]
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (_i in range(5000)) {
            summ += length(join(parts, ","))
            summ += length(md_escape("plain [text] with no pipes"))
            summ += length(to_generic_path("a\\b\\c.txt"))
        }
        let after = string_heap_bytes_allocated()
        t |> success(summ > 0)
        t |> success(int64(after) - int64(before) < 512l, "daslib producer temps should be reclaimed")
    }
}

[test(description = "identity-shaped calls such as md_escape, to_generic_path, join and base_name leave their source string intact under heavy temporary churn")]
def test_sweep_passthrough_traps(t : T?) {
    t |> run("identity inputs in temp position leave the source intact") @@(t) {
        // md_escape/to_generic_path freshness rides replace()'s non-empty search
        // argument; join's rides its builder. If any impl grows a `return input`
        // shortcut, the source below gets queued and the churn reuses its cell.
        let keep_md = clone_string("no pipes here")
        let keep_path = clone_string("already/generic.txt")
        let parts <- [ clone_string("solo") ]
        var summ = 0
        for (_i in range(500)) {
            summ += length(md_escape(keep_md))
            summ += length(to_generic_path(keep_path))
            summ += length(join(parts, ","))
        }
        for (i in range(2000)) {
            summ += length(string(7000000 + i))
        }
        t |> success(summ > 0)
        t |> equal(keep_md, "no pipes here")
        t |> equal(keep_path, "already/generic.txt")
        t |> equal(parts[0], "solo")
    }

    t |> run("base_name identity shape leaves the source intact") @@(t) {
        // base_name of a bare name (no separators) must still be a fresh copy
        let keep = clone_string("bare_name.txt")
        var summ = 0
        for (_i in range(500)) {
            summ += length(base_name(keep))
        }
        for (i in range(2000)) {
            summ += length(string(8000000 + i))
        }
        t |> success(summ > 0)
        t |> equal(keep, "bare_name.txt")
    }
}

[test(description = "under persistent heap delete_string frees a heap string, leaves a constant or empty string alone and clears the argument")]
def test_delete_string_persistent(t : T?) {
    var constStr = "123"
    {
        let removed = unsafe(delete_string(constStr))
        t.equal(removed, false) // const heap string should not be removed
        t.equal(constStr, "123")
    }

    var seed = random_seed(0)
    var str = "test{random_float(seed)}"
    {
        let removed = unsafe(delete_string(str))
        t.equal(removed, true) // heap string should be removed
        t.equal(str, "")
    }

    {
        var emptyStr : string
        let removed = unsafe(delete_string(emptyStr))
        t.equal(removed, false) // corner case: empty string should not be removed
        t.equal(emptyStr, "")
    }
}

[test(description = "under persistent heap delete_string on a temp string frees a heap string and leaves a constant string alone")]
def test_delete_temp_string_persistent(t : T?) {
    var tempString = unsafe(reinterpret<string#>("123"))
    {
        let removed = unsafe(delete_string(tempString))
        t.equal(removed, false) // const string should not be removed
        t.equal(tempString, "123")
    }

    var seed = random_seed(0)
    var tempString2 = unsafe(reinterpret<string#>("test{random_float(seed)}"))
    {
        let removed = unsafe(delete_string(tempString2))
        t.equal(removed, true) // heap string should be removed
        t.equal(tempString2, "") // temp string should be empty after removal
    }
}

[never_inline]
def first_byte(s : string) : int => first_character(s)

[test(description = "a reclaimed temporary with an embedded NUL byte, parked between temporaries of its real size class, does not free the wrong chunk")]
def test_temp_string_reclaim_embedded_nul(t : T?) {
    let withNul <- [for (i in range(20)); i == 4 ? 0u8 : uint8('A' + i)]
    let plain <- [for (i in range(20)); uint8('A' + i)]
    var total = 0
    for (_ in range(100)) {
        total += first_byte(string(withNul))
        total += first_byte(string(plain))
    }
    t |> equal(total, 200 * 'A')
}

[no_capture_string]
def drop_key(var tab : table<string; int>; key : string) : bool => erase(tab, key)

//! a global, so no key sliced from it is folded to a constant
var key_word = "hello"

def word_table() : table<string; int> {
    return <- { "he" => 1, "llo" => 2, "k7" => 7 }
}

[test(description = "a fresh string used as the key of tab?[k], key_exists, get, erase or a [no_capture_string] call is reclaimed, so the string heap stays flat over a long loop")]
def test_temp_string_table_key_flatness(t : T?) {
    t |> run("tab?[slice] and tab?[builder]") @@(t) {
        var inscope tab <- word_table()
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(20000)) {
            summ += tab?[slice(key_word, 0, 2)] ?? 0   // nolint:PERF031 the key under test
            summ += tab?["k{i % 10}"] ?? 0
            let named = "k{i % 10}"
            summ += tab?[named] ?? 0
        }
        t |> equal(summ, 20000 + 4000 * 7)
        t |> success(int64(string_heap_bytes_allocated()) - int64(before) < 256l,
            "lookup keys should be reclaimed")
    }
    t |> run("key_exists and get") @@(t) {
        var inscope tab <- word_table()
        let before = string_heap_bytes_allocated()
        var summ = 0
        for (i in range(20000)) {
            if (key_exists(tab, slice(key_word, 2))) {   // nolint:PERF031 the key under test
                summ++
            }
            get(tab, "k{i % 10}") $(v) {
                summ += v
            }
            if (key_exists(tab, "k{i % 10}")) {
                summ++
            }
        }
        t |> equal(summ, 20000 + 2000 * 7 + 2000)
        t |> success(int64(string_heap_bytes_allocated()) - int64(before) < 256l,
            "lookup keys should be reclaimed")
    }
    t |> run("erase and a [no_capture_string] function") @@(t) {
        var inscope tab <- word_table()
        let before = string_heap_bytes_allocated()
        var misses = 0
        for (i in range(20000)) {
            if (!erase(tab, "miss{i}")) {
                misses++
            }
            if (!drop_key(tab, slice(key_word, 1, 3))) {   // nolint:PERF031 the key under test
                misses++
            }
            if (!erase(tab, slice(key_word, 1, 4))) {   // nolint:PERF031 the key under test
                misses++
            }
        }
        t |> equal(misses, 60000)
        t |> equal(length(tab), 3)
        t |> success(int64(string_heap_bytes_allocated()) - int64(before) < 256l,
            "erase keys should be reclaimed")
    }
}

[test(description = "a reclaimed table key still finds, misses and erases the right entry, a key the table stores survives the churn, and a lookup whose table is itself looked up by a fresh key reads the right value")]
def test_temp_string_table_key_correctness(t : T?) {
    t |> run("hits, misses and erases") @@(t) {
        var inscope tab : table<string; int>
        for (i in range(200)) {
            tab |> insert("key{i}", i)
        }
        var bad = 0
        for (i in range(200)) {
            bad += (tab?["key{i}"] ?? -1) == i ? 0 : 1
            bad += (tab?["nokey{i}"] ?? -1) == -1 ? 0 : 1
            bad += key_exists(tab, "key{i}") ? 0 : 1
        }
        t |> equal(bad, 0)
        for (half in range(100)) {
            t |> success(erase(tab, "key{half * 2}"), "an even key erases")
        }
        t |> equal(length(tab), 100)
        t |> success(!key_exists(tab, "key{10}") && key_exists(tab, "key{11}"))
    }
    t |> run("a table found by a fresh key, then read by another") @@(t) {
        var inscope tabs : table<string; table<string; int>>
        for (j in range(4)) {
            var inner <- { "v{j}" => j, "w{j}" => 10 * j }
            tabs |> emplace("t{j}", inner)
        }
        var bad = 0
        for (i in range(2000)) {
            let j = i % 4
            bad += ((tabs?["t{j}"])?["w{j}"] ?? -1) == 10 * j ? 0 : 1
            bad += ((tabs?["t{j}"])?["v{j + 1}"] ?? -1) == -1 ? 0 : 1
        }
        t |> equal(bad, 0)
    }
}
