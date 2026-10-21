options gen2
require dastest/testing_boost public
require daslib/rtti

struct Pair {
    a : int
    b : string
}

struct Fixed {
    ints : int[3]
    grid : int[2][3]
    names : string[2]
    pairs : Pair[2]
}

[test]
def test_fixed_array_fields(t : T?) {
    t |> run("round trip") @(t : T?) {
        var src = Fixed()
        src.ints = fixed_array(1, 2, 3)
        src.grid[0] = fixed_array(1, 2, 3)
        src.grid[1] = fixed_array(4, 5, 6)
        src.names = fixed_array("x", "y")
        src.pairs[1] = Pair(a = 7, b = "p")
        var dst = Fixed()
        t |> success(sscan_json(sprint_json(src, false), dst))
        t |> equal(dst.ints[0], 1)
        t |> equal(dst.ints[2], 3)
        t |> equal(dst.grid[0][2], 3)
        t |> equal(dst.grid[1][0], 4)
        t |> equal(dst.grid[1][2], 6)
        t |> equal(dst.names[1], "y")
        t |> equal(dst.pairs[1].a, 7)
        t |> equal(dst.pairs[1].b, "p")
    }
    t |> run("short and long lists") @(t : T?) {
        var dst = Fixed()
        dst.ints = fixed_array(7, 8, 9)
        dst.pairs[0] = Pair(a = 11, b = "p0")
        dst.pairs[1] = Pair(a = 12, b = "p1")
        let json = ("\{\"ints\":[1],\"names\":[\"a\",\"b\",\"c\"],"
            + "\"grid\":[[1,2,3],[4,5,6],[7,8,9]]\}")
        t |> success(sscan_json(json, dst))
        t |> equal(dst.ints[0], 1)
        t |> equal(dst.ints[1], 8)
        t |> equal(dst.ints[2], 9)
        t |> equal(dst.grid[0][0], 1)
        t |> equal(dst.grid[1][2], 6)
        t |> equal(dst.names[0], "a")
        t |> equal(dst.names[1], "b")
        t |> equal(dst.pairs[0].a, 11)
        t |> equal(dst.pairs[0].b, "p0")
        t |> equal(dst.pairs[1].a, 12)
        t |> equal(dst.pairs[1].b, "p1")
    }
    t |> run("through sscan_json_at") @(t : T?) {
        var dst = Fixed()
        let grid = unsafe(addr(dst.grid))
        t |> success(sscan_json_at("[[1,2,3],[4,5,6]]", grid, typeinfo rtti_typeinfo(dst.grid)))
        t |> equal(dst.grid[0][0], 1)
        t |> equal(dst.grid[1][2], 6)
    }
}
