options gen2
options indenting = 4

//! a program root `llvm_second_context.das` simulates twice: its
//! `[init]` fills three globals from nested literals, and
//! `literal_holes` counts the fields a context's init left zero

enum Fmt {
    a
    b
    c
}

struct Six {
    b2, b2h : int
    b4, b4h, b8, b8h : int
}

struct Outer {
    tag : int
    inner : Six
}

var private TAB : table<Fmt; Six>
var private TUP : array<tuple<int; Six>>
var private OUT : array<Outer>

def private build_tab : table<Fmt; Six> {
    return <- {
    Fmt.a => Six(b2 = 1, b4 = 3, b8 = 5),
    Fmt.b => Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6),
    Fmt.c => Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6)
    }
}

def private build_tup : array<tuple<int; Six>> {
    return <- [(1, Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6)),
        (2, Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6))]
}

def private build_out : array<Outer> {
    return <- [Outer(tag = 1, inner = Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6)),
        Outer(tag = 2, inner = Six(b2 = 1, b2h = 2, b4 = 3, b4h = 4, b8 = 5, b8h = 6))]
}

[init]
def private literals_boot {
    delete TAB
    TAB <- build_tab()
    delete TUP
    TUP <- build_tup()
    delete OUT
    OUT <- build_out()
}

def private holes_of(s : Six) : int =>
    (s.b2 == 0 ? 1 : 0) + (s.b4 == 0 ? 1 : 0) + (s.b8 == 0 ? 1 : 0)

[export]
def literal_holes(var holes : int?) {
    var n = 0
    for (v in values(TAB)) {
        n += holes_of(v)
    }
    for (p in TUP) {
        n += holes_of(p._1)
    }
    for (o in OUT) {
        n += holes_of(o.inner)
    }
    *holes = n
}
