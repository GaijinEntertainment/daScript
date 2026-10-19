//fmt:ignore-file
options gen2
options auto_inline_functions = false

require daslib/defer
require daslib/ast_boost
require strings

struct Point {
    x : int
    y : int
}

struct Heavy {
    id : int
    items : array<int>
}

def takes_block(blk : block) {
    invoke(blk)
}

def takes_int_block(blk : block<(a : int) : void>) {
    invoke(blk, 1)
}

def pipes() : string {
    var total = 0
    takes_block() <| $() {
        total++
    }
    takes_int_block() <| $(a) {
        total += a
    }
    defer <| $() {
        total = 0
    }
    takes_block() $() {
        total++
    }
    takes_block() <| $() {   // a trailing comment stays
        total++
    }
    var gen <- generator<int>() <| $() {
        yield 1
        return false
    }
    for (g in gen) {
        total += g
    }
    return build_string() <| $(var w) {
        w |> write("{total}")
    }
}

def braces(x : int) : int {
    if (x > 1) {
        return x
    }
    if (x > 2) {
        return x + 1    // the comment keeps the braces
    }
    for (i in range(x)) {
        if (i == 3) {
            break
        }
    }
    return 0
}

def rtti(e : Expression?) : bool {
    let named = string(e.__rtti) == "ExprVar" || "ExprCall" == string(e.__rtti)
    return named || string(e.__rtti) == "not a type"
}

def always(x : int) : int {
    var y = x
    if (true) {
        y++
    }
    if ( true ) y++
    if (true)
    {
        y++
    }
    return y
}

def declared(x : int) : int {
    var y : int
    y = x + 1
    var moved : array<int>
    moved <- [x]
    var self_ref : int
    self_ref = x + 1    // the comment keeps the declaration apart
    return y + length(moved) + self_ref
}

def literal(x : int) : int {
    var a : array<int>
    a |> push(x)
    a |> push(x + 1)
    var names : array<string>
    names |> push("a")
    names |> push("{x}")
    var grows : array<int>
    grows |> push(1)
    grows |> push(length(grows))
    var first, second : array<int>
    var rows : array<array<int>>
    rows |> emplace(first)
    rows |> emplace(second)
    var c, d : array<int>
    c |> push(x)
    c |> push(2)
    return length(a) + length(names) + length(grows) + length(rows) + length(c) + length(d)
}

def structs(x : int) : int {
    var p = Point()
    p.x = x
    p.y = x * 2
    var q = new Point()
    q.x = 1
    q.y := x
    var h = Heavy()
    h.id = x
    h.items <- [x]
    var r = Point()
    r.x = x
    r.y = r.x
    var s, u = Point()
    s.x = x
    s.y = 2
    return p.x + q.y + h.id + r.y + s.y + u.y
}
