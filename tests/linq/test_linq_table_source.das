options gen2

require dastest/testing_boost public
require daslib/linq_boost

// Tier-2 LINQ over a table source via each_kv (the fused TableAdapter lands separately).
// each_kv is [unsafe_outside_of_for], so a chain head needs the explicit unsafe(...) wrap —
// same contract as each(arr) outside a fused chain.

[test]
def test_each_kv_tier2(t : T?) {
    t |> run("where/select/to_array over each_kv") @(t : T?) {
        var tab : table<string; int>
        tab |> insert("a", 1)
        tab |> insert("b", 2)
        tab |> insert("c", 3)
        var vals <- unsafe(each_kv(tab)) |> _where(_.value > 1) |> _select(_.value) |> to_array()
        vals |> sort()  // slot order is unspecified
        t |> equal(length(vals), 2)
        t |> equal(vals[0], 2)
        t |> equal(vals[1], 3)
        delete vals
    }
    t |> run("keys participate in the chain") @(t : T?) {
        var tab : table<int; int>
        for (i in range(6)) {
            tab |> insert(i, i * i)
        }
        let n = unsafe(each_kv(tab)) |> _where(_.key % 2 == 0) |> count()
        t |> equal(n, 3)
    }
}
