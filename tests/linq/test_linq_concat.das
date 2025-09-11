options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq

require _common


[test]
def test_append(t : T?) {
    t |> run("basic append") <| @(t : T?) {
        var query = append(
            [iterator for(x in 0..5); x],
            5
        )
        for (c, i in query, 0..6) {
            t |> equal(c, i)
        }
    }
    t |> run("complex type append") <| @(t : T?) {
        var qcomplex = append(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [5, 50])
        )
        for (c, i in qcomplex, 0..6) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
}

[test]
def test_prepend(t : T?) {
    t |> run("basic prepend") <| @(t : T?) {
        var query = prepend(
            [iterator for(x in 0..5); x],
            -1
        )
        for (c, i in query, -1..5) {
            t |> equal(c, i)
        }
    }
    t |> run("complex type prepend") <| @(t : T?) {
        var qcomplex = prepend(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [-1, -10])
        )
        for (c, i in qcomplex, -1..5) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
}

[test]
def test_concat(t : T?) {
    t |> run("basic concat") <| @(t : T?) {
        var query = concat(
            [iterator for(x in 0..5); x],
            [iterator for(x in 5..10); x]
        )
        for (c, i in query, 0..10) {
            t |> equal(c, i)
        }
    }
    t |> run("complex type concat") <| @(t : T?) {
        var qcomplex = concat(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 5..10); ComplexType(a = [x, x * 10])]
        )
        for (c, i in qcomplex, 0..10) {
            assert(c.a.Equal([i, i * 10]))
        }
    }
}
