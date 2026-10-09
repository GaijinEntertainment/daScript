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
    takes_block() {
        total++
    }
    takes_int_block() $(a) {
        total += a
    }
    defer() {
        total = 0
    }
    takes_block() $ {
        total++
    }
    takes_block() {   // a trailing comment stays
        total++
    }
    var gen <- generator<int>() <| $() {
        yield 1
        return false
    }
    for (g in gen) {
        total += g
    }
    return build_string() $(var w) {
        w |> write("{total}")
    }
}

def braces(x : int) : int {
    if (x > 1) return x
    if (x > 2) {
        return x + 1    // the comment keeps the braces
    }
    for (i in range(x)) {
        if (i == 3) break
    }
    return 0
}

def rtti(e : Expression?) : bool {
    return e is ExprVar || e is ExprCall || string(e.__rtti) == "not a type"
}

def always(x : int) : int {
    var y = x
    {
        y++
    }
    y++
    if (true)
    {
        y++
    }
    return y
}

def declared(x : int) : int {
    var y : int = x + 1
    var moved : array<int> <- [x]
    var self_ref : int
    self_ref = x + 1    // the comment keeps the declaration apart
    return y + length(moved) + self_ref
}

def literal(x : int) : int {
    var a : array<int> <- [x, x + 1]
    var names : array<string> <- ["a", "{x}"]
    var grows : array<int>
    grows |> push(1)
    grows |> push(length(grows))
    var first, second : array<int>
    var rows : array<array<int>> <- [<-first, <-second]
    return length(a) + length(names) + length(grows) + length(rows)
}

def structs(x : int) : int {
    var p = Point(x = x, y = x * 2)
    var q = new Point(x = 1, y := x)
    var h <- Heavy(id = x, items <- [x])
    var r = Point()
    r.x = x
    r.y = r.x
    return p.x + q.y + h.id + r.y
}
