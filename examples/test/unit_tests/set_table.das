options gen2
[export]
def test {
    var tab : table<int>    // same as table<int;void>
    assert(length(tab) == 0)
    /*
    tab[1] = 1  // 30110: can't index into table<...;void>
    unsafe
        let q = tab?[1] ?? 13   // 30110: can't safe-index into table<...;void>
    for v in tab |> values  // 30118: unsupported iteration type for the loop variable v, iterating over void
        print("v = {v}\n")
    */
    for (i in 3..6) {
        tab |> insert(i)
    }
    for (k, i in keys(tab), array<int>(5, 4, 3)) {
        assert(k == i)
    }
    tab |> erase(4)
    for (k, i in keys(tab), array<int>(5, 3)) {
        assert(k == i)
    }
    var tc := tab
    for (k, i in keys(tc), array<int>(5, 3)) {
        assert(k == i)
    }
    assert(tc |> key_exists(3))
    assert(!tc |> key_exists(4))
    /*
    tc |> find(3) <| $ ( p )    // 40103: find(table<keyT;void>,...) is not supported
        print("p = {p}\n")

    tc |> find_if_exists(3) <| $ ( p )  // 40103: find_if_exists(table<keyT;void>,...) is not supported
        print("p = {p}\n")
    */
    var ttt <- { 1, 2, 3, 4 }
    for (k, i in keys(ttt), array<int>(4, 3, 2, 1)) {
        assert(k == i)
    }
    var qqq <- to_table(fixed_array<int>(1, 2, 3, 4))
    print("\n")
    for (k, i in keys(qqq), array<int>(4, 3, 2, 1)) {
        assert(k == i)
    }
    return true
}
