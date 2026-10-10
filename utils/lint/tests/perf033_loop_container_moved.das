options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
options _nolint = "STYLE042,PERF006,PERF022"
// PERF033: a container declared inside a loop leaks its buffer every iteration unless it is
// inscope - or moved out, in which case the taker frees it. The rule reports at the function's
// end, after the walk has seen every use.

expect 31208:4

require daslib/perf_lint
require daslib/array_boost

struct Bag {
    items : array<int>
}

variant Payload {
    items : array<int>
    none : int
}

def take(var items : array<int>) : int {
    let n = length(items)
    delete items
    return n
}

def peek(items : array<int>) : int => length(items)

def peek_bytes(bytes : array<uint8>) : int => length(bytes)

// --- Bad patterns (warnings expected) ---

def bad_array_in_loop() {
    for (i in range(3)) {
        var items : array<int>                  // PERF033
        items |> push(i)
    }
}

def bad_table_in_loop() {
    for (i in range(3)) {
        var seen : table<int>                   // PERF033
        seen |> insert(i)
    }
}

def bad_passed_by_const() : int {
    var total = 0
    for (i in range(3)) {
        var items : array<int>                  // PERF033 - peek takes a const reference, nothing frees it
        items |> push(i)
        total += peek(items)
    }
    return total
}

def bad_struct_field_clone() {
    var bags : array<Bag>
    for (i in range(3)) {
        var items : array<int>                  // PERF033 - the field is a clone, the source stays alive
        items |> push(i)
        bags |> emplace(Bag(items := items))
    }
    delete bags
}

// --- Good patterns (no warnings) ---

def good_temp_array_view(var data : uint8?) {
    for (i in range(3)) {
        let view = unsafe(temp_array(data, 2, type<uint8>))  // inscope here would delete the borrowed pointee
        peek_bytes(view)
    }
}

def good_inscope() {
    for (i in range(3)) {
        var inscope items : array<int>
        items |> push(i)
    }
}

def good_deleted() {
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        delete items
    }
}

def good_moved_into_var_parameter() : int {
    var total = 0
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        total += take(items)
    }
    return total
}

def good_emplaced() {
    var rows : array<array<int>>
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        rows |> emplace(items)
    }
    delete rows
}

def good_moved_by_arrow() {
    var keep : array<int>
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        delete keep
        keep <- items
    }
    delete keep
}

def good_returned_by_move() : array<int> {
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        return <- items
    }
    return <- [for (x in range(0)); x]
}

def good_struct_field_move() {
    var bags : array<Bag>
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        bags |> emplace(Bag(items <- items))
    }
    delete bags
}

def good_variant_field_move() {
    var payloads : array<Payload>
    for (i in range(3)) {
        var items : array<int>
        items |> push(i)
        payloads |> emplace(Payload(items <- items))
    }
    delete payloads
}

[export]
def main() { }
