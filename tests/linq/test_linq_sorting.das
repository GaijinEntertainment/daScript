options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq

require _common

[test]
def test_reverse(t : T?) {
    t |> run("basic reverse") <| @(t : T?) {
        var query = reverse(
            [iterator for(x in 0..5); x]
        )
        for (c, i in query, 0..5) {
            t |> equal(c, 4 - i)
        }
    }
    t |> run("complex type reverse") <| @(t : T?) {
        var qcomplex = reverse(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

[test]
def test_orderby(t : T?) {
    t |> run("basic orderby") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = order_by(
            numbers.to_sequence(),
            $(n : int) => n
        )
        var sorted_n = [1, 3, 4, 5, 8]
        for (n, i in query_n, 0..4) {
            t |> equal(n, sorted_n[i])
        }
    }
    t |> run("complex orderby") <| @(t : T?) {
        var query = order_by(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        for (p, i in query, 0..5) {
            t |> equal(p.name, sorted[i].name)
            t |> equal(p.age, sorted[i].age)
        }
    }
    t |> run("reverse orderby") <| @(t : T?) {
        var rquery = order_by_descending(
            people.to_sequence(),
            $(p : Person) => (p.name, p.age)
        )
        for (p, i in rquery, 0..5) {
            t |> equal(p.name, sorted_r[i].name)
            t |> equal(p.age, sorted_r[i].age)
        }
    }
    t |> run("complex orderby") <| @(t : T?) {
        var qcomplex = order_by(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("reverse complex orderby") <| @(t : T?) {
        var qcomplex_r = order_by_descending(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (c, i in qcomplex_r, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

[test]
def test_order(t : T?) {
    t |> run("basic order") <| @(t : T?) {
        var numbers = [
            5, 3, 8, 1, 4
        ]
        var query_n = order(
            numbers.to_sequence(),
            $(a, b) => a < b
        )
        var sorted_n = [1, 3, 4, 5, 8]
        for (n, i in query_n, 0..4) {
            t |> equal(n, sorted_n[i])
        }
    }
    t |> run("complex order") <| @(t : T?) {
        var query = order(
            people.to_sequence(),
            $(a, b) => less((a.name, a.age), (b.name, b.age))
        )
        for (p, i in query, 0..5) {
            t |> equal(p.name, sorted[i].name)
            t |> equal(p.age, sorted[i].age)
        }
    }
    t |> run("reverse order") <| @(t : T?) {
        var rquery = order_descending(
            people.to_sequence(),
            $(a, b) => less((a.name, a.age), (b.name, b.age))
        )
        for (p, i in rquery, 0..5) {
            t |> equal(p.name, sorted_r[i].name)
            t |> equal(p.age, sorted_r[i].age)
        }
    }
    t |> run("complex order") <| @(t : T?) {
        var qcomplex = order(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(a, b) => less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
        )
        for (c, i in qcomplex, 0..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("reverse complex order") <| @(t : T?) {
        var qcomplex_r = order_descending(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(a, b) => less((a.a[0], a.a[1]), (b.a[0], b.a[1]))
        )
        for (c, i in qcomplex_r, 0..5) {
            t |> success(c.a.Equal([4 - i, (4 - i) * 10]))
        }
    }
}

