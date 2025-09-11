options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq
require math


require _common

[test]
def test_select_transform(t : T?) {
    t |> run("basic select_transform") <| @(t : T?) {
        var chars = ["a", "b", "c", "d", "e"]
        var query = select(
            chars.to_sequence(),
            $(s : string) => (s, s + s)
        )
        for (i, ch, q in 0..5, chars, query) {
            t |> equal(ch, q._0)
            t |> equal(ch + ch, q._1)
        }
    }
    t |> run("complex select_transform") <| @(t : T?) {
        var qcomplex = select(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => (c.a[0], c.a[1])
        )
        for (i, q in 0..5, qcomplex) {
            t |> equal(q._0, i)
            t |> equal(q._1, i * 10)
        }
    }
}

[test]
def test_select(t : T?) {
    t |> run("basic select") <| @(t : T?) {
        var chars = ["a", "b", "c", "d", "e"]
        var query = select(chars.to_sequence())
        for (i, ch, q in 0..5, chars, query) {
            t |> equal(i, q._0)
            t |> equal(ch, q._1)
        }
    }
    t |> run("complex select") <| @(t : T?) {
        var qcomplex = select(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])]
        )
        for (i, q in 0..5, qcomplex) {
            t |> equal(q._0, i)
            t |> equal(q._1.a[0], i)
            t |> equal(q._1.a[1], i * 10)
        }
    }
}
