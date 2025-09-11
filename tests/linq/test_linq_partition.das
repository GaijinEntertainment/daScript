options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq_boost

require _common

[test]
def test_skip(t : T?) {
    t |> run("basic skip") <| @(t : T?) {
        var query = skip(
            [iterator for(x in 0..5); x],
            2
        )
        for (c, i in query, 0..3) {
            t |> equal(c, i + 2)
        }
    }
    t |> run("skip while") <| @(t : T?) {
        var query_skip = _skip_while(
            [iterator for(x in 0..5); x],
            _ < 3
        )
        for (c, i in query_skip, 0..2) {
            t |> equal(c, i + 3)
        }
    }
    t |> run("complex skip") <| @(t : T?) {
        var qcomplex = skip(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            2
        )
        for (c, i in qcomplex, 0..3) {
            t |> success(c.a.Equal([i + 2, (i + 2) * 10]))
        }
    }
    t |> run("complex skip while") <| @(t : T?) {
        var qcomplex_skip_while = _skip_while(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            _.a[0] < 3
        )
        for (c, i in qcomplex_skip_while, 0..2) {
            t |> success(c.a.Equal([i + 3, (i + 3) * 10]))
        }
    }
}

[test]
def test_take(t : T?) {
    t |> run("basic take") <| @(t : T?) {
        var query = take(
            [iterator for(x in 0..5); x],
            3
        )
        for (c, i in query, 0..2) {
            t |> equal(c, i)
        }
    }
    t |> run("complex take while") <| @(t : T?) {
        var query_take_while = _take_while(
            [iterator for(x in 0..5); x],
            _ < 3
        )
        for (c, i in query_take_while, 0..2) {
            t |> equal(c, i)
        }
    }
    t |> run("complex take range") <| @(t : T?) {
        var query_take_range = take(
            [iterator for(x in 0..5); x],
            1..4
        )
        for (c, i in query_take_range, 0..2) {
            t |> equal(c, i + 1)
        }
    }
    t |> run("complex take") <| @(t : T?) {
        var qcomplex = take(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            3
        )
        for (c, i in qcomplex, 0..2) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
    t |> run("complex take while") <| @(t : T?) {
        var qcomplex_take_while = _take_while(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            _.a[0] < 3
        )
        for (c, i in qcomplex_take_while, 0..2) {
            t |> success(c.a.Equal([i, i * 10]))
        }
    }
}

[test]
def text_chunk(t : T?) {
    t |> run("basic chunk") <| @(t : T?) {
        var query = chunk(
            [iterator for(x in 0..6); x],
            2
        )
        var expected = [
            [0, 1],
            [2, 3],
            [4, 5]
        ]
        for (c, e in query, expected) {
            for (i, v in c, e) {
                t |> equal(i, v)
            }
        }
    }
    t |> run("complex chunk") <| @(t : T?) {
        var qcomplex = chunk(
            [iterator for(x in 0..6); ComplexType(a = [x, x * 10])],
            2
        )
        var expected_complex = [
            [(0, 0), (1, 10)],
            [(2, 20), (3, 30)],
            [(4, 40), (5, 50)]
        ]
        for (c, e in qcomplex, expected_complex) {
            for (i, v in c, e) {
                t |> equal(i.a[0], v._0)
                t |> equal(i.a[1], v._1)
            }
        }
    }
}
