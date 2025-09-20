options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq_boost

require _common

[test]
def test_comprehension_fold(t : T?) {
    t |> run("basic comprehension") <| @(t : T?) {
        var t1 <- ([1, 2, 3, 4, 5].to_sequence()
            ._where(_ <= 3)
            .to_array()
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t1), "array<int>")
        for (i, v in 0..3, t1) {
            t |> equal(i + 1, v)
        }
    }

    t |> run("comprehension with select") <| @(t : T?) {
        var t2 <- ([1, 2, 3, 4, 5].to_sequence()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t2), "iterator<int>")
        for (i, v in 0..3, t2) {
            t |> equal((i + 1) * 2, v)
        }
    }

    t |> run("comprehension with select to array") <| @(t : T?) {
        var t3 <- ([1, 2, 3, 4, 5].to_sequence()
            ._select(_ * 2)
            .to_array()
            ._fold()) // note - if fold fails, it won't compile
        t |> equal(typeinfo typename(t3), "array<int>")
        for (i, v in 1..6, t3) {
            t |> equal(i * 2, v)
        }
    }
}

[test]
def test_generic_fold(t : T?) {
    t |> run("default array to array") <| @(t : T?) {
        var t1 <- ([1, 2, 3, 4, 5]
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t1), "array<int>")
        for (i, v in 0..3, t1) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default iterator to iterator") <| @(t : T?) {
        var t2 <- ([1, 2, 3, 4, 5].to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t2), "iterator<int>")
        for (i, v in 0..3, t2) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("reverse_to_array") <| @(t : T?) {
        var iterv <- [iterator for(x in 1..6); x]
        var t2a <- (iterv
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t2a), "iterator<int>")
        for (i, v in 0..3, t2a) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default array to iterator") <| @(t : T?) {
        var t3 <- ([1, 2, 3, 4, 5]
            .to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            ._fold()
            )
        t |> equal(typeinfo typename(t3), "iterator<int>")
        for (i, v in 0..3, t3) {
            t |> equal((3 - i) * 2, v)
        }
    }
    t |> run("default iterator to array") <| @(t : T?) {
        var t4 <- ([1, 2, 3, 4, 5].to_sequence()
            .reverse()
            ._where(_ <= 3)
            ._select(_ * 2)
            .to_array()
            ._fold()
            )
        t |> equal(typeinfo typename(t4), "array<int>")
        for (i, v in 0..3, t4) {
            t |> equal((3 - i) * 2, v)
        }
    }
}

