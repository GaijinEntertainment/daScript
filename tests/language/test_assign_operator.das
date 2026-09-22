options gen2
require dastest/testing_boost public

typedef distinct SoundHandle = uint

def operator =(var dst : SoundHandle&; src : int) {
    dst !== SoundHandle(uint(src))
}

def operator =(var dst : uint&; src : SoundHandle) {
    dst !== *src
}

def operator !=(h : SoundHandle; z : int) : bool => *h != uint(z)

struct Sfx {
    hit : uint
    current : SoundHandle = 5
}

var g_handle : SoundHandle = 7

struct Counted {
    value : int
    copies : int
}

def operator =(var dst : Counted&; src : Counted) {
    dst !== src
    dst.copies++
}

def operator :=(var dst : Counted&; src : Counted) {
    dst !:= src
    dst.copies += 100
}

struct Box {
    items : array<int>
    moves : int
}

def operator <-(var dst : Box&; var src : array<int>) {
    dst.items <- src
    dst.moves++
}

class Holder {
    h : uint
    def operator =(src : int) {
        h = uint(src) * 2u
    }
}

[test]
def test_distinct_assign_operator(t : T?) {
    t |> run("init at every site goes through operator =") @(t : T?) {
        let local : SoundHandle = 3
        t |> equal(*local, 3u)
        t |> equal(*g_handle, 7u)
        let sfx = Sfx()
        t |> equal(*sfx.current, 5u)
        let made = Sfx(hit = 1u, current = 9)
        t |> equal(*made.current, 9u)
    }
    t |> run("assignment and comparison") @(t : T?) {
        var current : SoundHandle = 3
        t |> success(current != 0)
        current = 0
        t |> success(!(current != 0))
        t |> equal(*current, 0u)
    }
    t |> run("either side may be the distinct type") @(t : T?) {
        var sfx = Sfx()
        sfx.hit = SoundHandle(11u)
        t |> equal(sfx.hit, 11u)
        let raw : uint = SoundHandle(12u)
        t |> equal(raw, 12u)
    }
}

[test]
def test_same_type_overload_and_raw_forms(t : T?) {
    t |> run("an overload on the exact type wins over the builtin copy") @(t : T?) {
        var src : Counted
        src.value = 1
        var dst : Counted
        dst = src
        t |> equal(dst.value, 1)
        t |> equal(dst.copies, 1)
        let init : Counted = src
        t |> equal(init.copies, 1)
    }
    t |> run("init from a make-struct temporary runs the operator as well") @(t : T?) {
        let made = Counted(value = 4, copies = 0)
        t |> equal(made.copies, 1)
    }
    t |> run("the raw copy is the builtin copy") @(t : T?) {
        var src : Counted
        src.value = 2
        var dst : Counted
        dst !== src // nolint:STYLE011 the raw copy has no declaration form; the statement is the subject
        t |> equal(dst.value, 2)
        t |> equal(dst.copies, 0)
    }
    t |> run("clone overload and its raw form") @(t : T?) {
        var src : Counted
        src.value = 3
        var viaOp : Counted
        viaOp := src
        t |> equal(viaOp.copies, 100)
        var raw : Counted
        raw !:= src // nolint:STYLE011 the raw clone has no declaration form; the statement is the subject
        t |> equal(raw.copies, 0)
    }
}

[test]
def test_move_operator(t : T?) {
    t |> run("<- dispatches to operator <-, at init and at assignment") @(t : T?) {
        var first <- [1, 2, 3]
        var box : Box <- first
        t |> equal(length(box.items), 3)
        t |> equal(box.moves, 1)
        t |> equal(length(first), 0)
        var second <- [4]
        box <- second
        t |> equal(length(box.items), 1)
        t |> equal(box.moves, 2)
    }
    t |> run("the raw move is the builtin move") @(t : T?) {
        var a : Box
        var b : Box
        b.moves = 5
        a !<- b // nolint:PERF030 a is fresh; the raw move has no declaration form to fold into
        t |> equal(a.moves, 5)
        t |> equal(b.moves, 0)
    }
}

[test]
def test_class_operator(t : T?) {
    t |> run("an operator = declared in a class takes self as the destination") @(t : T?) {
        var holder = new Holder()
        *holder = 21
        t |> equal(holder.h, 42u)
        unsafe {
            delete holder
        }
    }
}
