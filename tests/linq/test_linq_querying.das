options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq_boost

require _common

[test]
def test_any(t : T?) {
    t |> run("basic any") <| @(t : T?) {
        var query = any(
            [iterator for(x in 0..5); x]
        )
        t |> success(query)
    }
    t |> run("complex any") <| @(t : T?) {
        var qcomplex = any(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        t |> success(qcomplex)
    }
}

[test]
def test_all(t : T?) {
    t |> run("basic all") <| @(t : T?) {
        var query = _all(
            [iterator for(x in 0..5); x],
            _ < 5
        )
        t |> success(query)
    }
    t |> run("complex all") <| @(t : T?) {
        var qcomplex = _all(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            _.a[0] < 5
        )
        t |> success(qcomplex)
    }
}

[test]
def test_contains(t : T?) {
    t |> run("basic contains") <| @(t : T?) {
        var query = contains(
            [iterator for(x in 0..5); x],
            3
        )
        t |> success(query)
    }
    t |> run("complex contains") <| @(t : T?) {
        var qcomplex = contains(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            ComplexType(a = [3, 30])
        )
        t |> success(qcomplex)
    }
}

