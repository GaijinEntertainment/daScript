//fmt:ignore-file
options gen2
options auto_inline_functions = false

require daslib/linq

class Names {
    names : table<string>

    def contains(k : string) : bool {
        return names |> key_exists(k)
    }
}

def two { return 2 }

def tight(){ return 6 }

def take(var a : array<int>) : array<int> {
    return <- a
}

def long_signature_and_body(first_argument, second_argument : int; third_argument : float) : float {
    return float(first_argument * second_argument) + third_argument * 2.0
}

def commented() : int {
    return 3    // a single comment moves to the end of the new line
}

def two_comments() : int {   // two comments
    return 4    // keep the block
}

def sizes() : int {
    var kq : array<uint8>
    var ks : array<uint8>
    var x : int
    var y : int    // a comment keeps the run apart
    return length(kq) + length(ks) + x + y
}

def rows(n : int) : int {
    var total = 0
    for (i in range(n)) {
        var row : array<int>
        let seen <- { i => i }
        var left, right : array<int>
        total += length(row) + length(seen) + length(left) + length(right)
    }
    return total
}

def each_once(blk : block<():void>) {
    invoke(blk)
}

def block_in_while(n : int) : int {
    var total = 0
    var i = 0
    while (i++ < n) {
        each_once() {
            var seen : table<int>
            total += length(seen)
        }
    }
    return total
}

def no_leak(n : int; var rows : array<array<int>>) : int {
    var total = 0
    var outside : array<int>
    for (i in range(n)) {
        var inscope kept : array<int>
        var row & = unsafe(rows[i])
        var fixed : int[4]
        var picked : array<int?>
        var fns : array<lambda<():int>>
        total += length(kept) + length(row) + fixed[0] + length(picked) + length(fns)
    }
    return total + length(outside)
}

def foreign_generic(a, b : array<int>) : int =>
    length(group_join_to_array(a.to_sequence(), b.to_sequence(), $(x : int) => x, $(y : int) => y,
        $(x : int; ys : array<int>) => x + length(ys)))
