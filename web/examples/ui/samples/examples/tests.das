options gen2

require dastest/testing_boost public
require strings

[test]
def test_adds(t : T?) {
    t |> equal(2 + 2, 4)
    t |> equal("hello, " + "world", "hello, world")
}

[test]
def test_subtests(t : T?) {
    t |> run("strings") <| @(t : T?) {
        t |> equal(length("daslang"), 7)
    }
    t |> run("range") <| @(t : T?) {
        var sum = 0
        for (i in range(10)) {
            sum += i
        }
        t |> equal(sum, 45)
    }
}
