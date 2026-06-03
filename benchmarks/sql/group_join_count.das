options gen2
options persistent_heap

require _common public

// group_join (C# GroupJoin / outer) over arrays: `dealers |> _group_join(cars)` groups the cars under each
// dealer, one result row per dealer — an empty group when a dealer has no cars. Fused array splice (the PR
// that taught the _fold join splice to handle group_join). Two lanes isolate the outer-semantics cost:
//   m3f_matched   — 100 dealers (ids 1..100), every one matched by cars → 100 non-empty buckets
//   m3f_unmatched — 100 dealers (ids 101..200), none matched → 100 empty groups (the outer-empty emit path)
// Same left size (100) and right size (n cars) in both, so the delta is purely matched-bucket vs empty-group.
// No SQL lane: `_group_join` has no SQL push-down (it rejects — in-memory only). No decs/xml lane: their
// adapters route group joins to tier-2; the array adapter owns the fused splice for now.

let GJ_DEALERS = 100

def private dealers_with_offset(off : int) : array<Dealer> {
    var arr : array<Dealer>
    arr |> resize(GJ_DEALERS)
    for (i in range(GJ_DEALERS)) {
        arr[i] = Dealer(id = off + i + 1, name = "D{off + i}")
    }
    return <- arr
}

def run_matched(b : B?; n : int) {
    let cars <- fixture_array(n)            // dealer_id in 1..100
    let dealers <- dealers_with_offset(0)   // ids 1..100 — every dealer matched
    b |> run("m3f_matched/{n}", n) {
        let rows <- _fold(dealers |> _group_join(cars,
                                                 $(d : Dealer, c : Car) => d.id == c.dealer_id,
                                                 $(d : Dealer, cs : array<Car>) => (id = d.id, n = length(cs))) |> to_array())
        b |> accept(length(rows))
        if (length(rows) != GJ_DEALERS) {
            b->failNow()
        }
    }
}

def run_unmatched(b : B?; n : int) {
    let cars <- fixture_array(n)                     // dealer_id in 1..100
    let dealers <- dealers_with_offset(GJ_DEALERS)   // ids 101..200 — none matched → all empty groups
    b |> run("m3f_unmatched/{n}", n) {
        let rows <- _fold(dealers |> _group_join(cars,
                                                 $(d : Dealer, c : Car) => d.id == c.dealer_id,
                                                 $(d : Dealer, cs : array<Car>) => (id = d.id, n = length(cs))) |> to_array())
        b |> accept(length(rows))
        if (length(rows) != GJ_DEALERS) {
            b->failNow()
        }
    }
}

[benchmark]
def group_join_count_matched(b : B?) {
    run_matched(b, 100000)
}

[benchmark]
def group_join_count_unmatched(b : B?) {
    run_unmatched(b, 100000)
}
