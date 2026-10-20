options gen2

require dastest/testing_boost public
require daslib/cmp
require daslib/strings_boost
require daslib/algorithm
require daslib/sort_boost

[ordered]
struct Span {
    from : int
    to : int
}

[ordered(score = greater, name)]
struct Player {
    name : string
    score : int
    tag : string
}

def name_length(w : Word) : int => length(w.name)

[ordered(key = name_length, name = less)]
struct Word {
    name : string
}

[ordered(to)]
struct Pick {
    from : int
    to : int
}

struct Pos {
    x : int
    y : int
}

struct Marker {
    pos : Pos
    tag : string
}

[ordered(id)]
struct Bag {
    id : int
    items : array<int>
}

var g_key_calls = 0

def counted(x : int) : int {
    g_key_calls++
    return x
}

def spans_of(var arr : array<Span>) : string => join([for (s in arr); "{s.from}:{s.to}"], " ")

def players_of(var arr : array<Player>) : string => join([for (p in arr); "{p.name}{p.score}"], " ")

[test]
def test_ordered_annotation(t : T?) {
    t |> run("every field in declaration order, ascending") @(t : T?) {
        var arr <- [Span(from = 2, to = 1), Span(from = 1, to = 5), Span(from = 1, to = 3)]
        sort(arr)
        t |> equal(spans_of(arr), "1:3 1:5 2:1")
    }
    t |> run("equal on every field is not less") @(t : T?) {
        t |> equal(Span(from = 1, to = 2) < Span(from = 1, to = 2), false)
        t |> equal(Span(from = 1, to = 2) < Span(from = 1, to = 3), true)
    }
    t |> run("listed fields only, per-field direction") @(t : T?) {
        var arr <- [
            Player(name = "b", score = 1, tag = "z"),
            Player(name = "a", score = 1, tag = "y"),
            Player(name = "c", score = 7, tag = "x")
        ]
        sort(arr)
        t |> equal(players_of(arr), "c7 a1 b1")
        let tagged = Player(name = "a", score = 1, tag = "x")
        t |> equal(tagged < Player(name = "a", score = 1, tag = "y"), false)
    }
    t |> run("a key function, then a field") @(t : T?) {
        var arr <- [Word(name = "ccc"), Word(name = "bb"), Word(name = "ab")]
        sort(arr)
        t |> equal(join([for (w in arr); w.name], " "), "ab bb ccc")
    }
    t |> run("an unlisted field does not take part") @(t : T?) {
        t |> equal(Pick(from = 9, to = 1) < Pick(from = 0, to = 2), true)
        t |> equal(Pick(from = 0, to = 1) < Pick(from = 9, to = 1), false)
    }
}

[test]
def test_compare_by(t : T?) {
    t |> run("fields, a descending key") @(t : T?) {
        var arr <- [
            Player(name = "b", score = 1),
            Player(name = "a", score = 1),
            Player(name = "c", score = 7)
        ]
        sort(arr) <| cmp_by(-_.score, _.name)
        t |> equal(players_of(arr), "c7 a1 b1")
    }
    t |> run("a computed key reads the caller's locals") @(t : T?) {
        let bias = 10
        var arr <- [
            Player(name = "aaa", score = 1),
            Player(name = "b", score = 25),
            Player(name = "cc", score = 2)
        ]
        sort(arr) <| cmp_by(length(_.name) * bias + _.score)
        t |> equal(players_of(arr), "cc2 aaa1 b25")
    }
    t |> run("the element itself as the key") @(t : T?) {
        var arr <- [3, 1, 2]
        sort(arr) <| cmp_by(-_)
        t |> equal(arr, [3, 2, 1])
    }
    t |> run("an index key") @(t : T?) {
        let weight <- [5, 1, 3]
        var order <- [0, 1, 2]
        sort(order) <| cmp_by(weight[_])
        t |> equal(order, [1, 2, 0])
    }
    t |> run("ties keep comparing") @(t : T?) {
        var arr <- [(2, "b"), (1, "z"), (2, "a")]
        sort(arr) <| cmp_by(_._0, -_._1 |> length(), _._1)
        t |> equal(join([for (e in arr); "{e._0}{e._1}"], " "), "1z 2a 2b")
    }
}

[test]
def test_cmp_more(t : T?) {
    t |> run("a computed key that is not the last is evaluated once per side") @(t : T?) {
        let one <- [3]
        g_key_calls = 0
        let at = lower_bound(one, 4) <| cmp_by(counted(_), -_)
        t |> equal(at, 1)
        t |> equal(g_key_calls, 2)
    }
    t |> run("the element itself, ascending and descending") @(t : T?) {
        var arr <- [3, 1, 2]
        sort(arr) <| cmp_by(_)
        t |> equal(arr, [1, 2, 3])
        var names <- ["b", "c", "a"]
        sort(names) <| cmp_by(-_)
        t |> equal(join(names, ""), "cba")
    }
    t |> run("nested fields") @(t : T?) {
        var arr <- [
            Marker(pos = Pos(x = 2, y = 0), tag = "b"),
            Marker(pos = Pos(x = 1, y = 5), tag = "a"),
            Marker(pos = Pos(x = 1, y = 2), tag = "c")
        ]
        sort(arr) <| cmp_by(_.pos.x, -_.pos.y)
        t |> equal(join([for (m in arr); m.tag], ""), "acb")
    }
    t |> run("a listed field orders a structure with a non-copyable field") @(t : T?) {
        var arr <- [Bag(id = 2, items <- [1]), Bag(id = 1, items <- [2, 3])]
        sort(arr)
        t |> equal(arr[0].id, 1)
        t |> equal(length(arr[0].items), 2)
    }
    t |> run("searches and partial sorts take the same orderings") @(t : T?) {
        let spans <- [Span(from = 1, to = 1), Span(from = 2, to = 0), Span(from = 2, to = 5)]
        t |> equal(lower_bound(spans, Span(from = 2, to = 0)), 1)
        t |> equal(binary_search(spans, Span(from = 2, to = 5)), true)
        var arr <- [5, 1, 4, 2, 3]
        partial_sort(arr, 2) <| cmp_by(-_)
        t |> equal(arr[0], 5)
        t |> equal(arr[1], 4)
        let players <- [Player(name = "a", score = 1), Player(name = "b", score = 9)]
        let at = lower_bound(players, Player(name = "x", score = 9)) <| cmp_by(-_.score)
        t |> equal(at, 0)
    }
}
