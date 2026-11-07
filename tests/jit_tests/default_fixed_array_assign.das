options gen2
require dastest/testing_boost

def zero_through_reference(var a : uint[8]) {
    a = default<uint[8]>
}

[test]
def test_assigning_default_zeroes_the_whole_fixed_array(t : T?) {
    t |> run("a local") @(t : T?) {
        var a = fixed_array<int>(1, 2, 3, 4)
        a = default<int[4]>
        t |> equal(a[0], 0)
        t |> equal(a[1], 0, "every element, not only the first")
        t |> equal(a[3], 0)
    }
    t |> run("through a var parameter") @(t : T?) {
        var a : uint[8]
        for (w in a) {
            w = 0xffffffffu
        }
        zero_through_reference(a)
        t |> equal(a[0], 0u)
        t |> equal(a[7], 0u, "the last element too")
    }
    t |> run("a tuple element and a nested dimension") @(t : T?) {
        var ts : tuple<int; float>[2]
        ts[1]._0 = 3
        ts = default<tuple<int; float>[2]>
        t |> equal(ts[1]._0, 0)
        var ss : int[2][3]
        ss[1][2] = 9
        ss = default<int[2][3]>
        t |> equal(ss[1][2], 0)
    }
    t |> run("initializing from default is the control") @(t : T?) {
        let a = default<int[4]>
        t |> equal(a[3], 0)
    }
}
