options gen2
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost
require daslib/decs_boost
require daslib/linq
require daslib/linq_boost

// Bare-count benchmark. Validates Slice 1 of Approach Z: _fold(from_decs_template(type<Foo>).count())
// should splice to the arch.size shortcut — sum of per-archetype arch.size in a single for_each_archetype walk.

let N = 100000

[decs_template(prefix = "bench_")]
struct BenchCountRow {
    val : int
}

def fixture(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, BenchCountRow(val = i))
    }
}

// m1: hand-written for_each_archetype + arch.size — the ideal target.
[benchmark]
def from_decs_count_m1_hand(b : B?) {
    fixture(N)
    b |> run("m1_hand_arch_size/{N}", N) {
        var erq : EcsRequest
        erq.req |> push("bench_val")
        var total = 0
        for_each_archetype(erq) $(arch : Archetype) {
            total += arch.size
        }
        if (total == 0) {
            b->failNow()
        }
    }
}

// m2: from_decs_template.count() bare — eager bridge baseline (no splice).
[benchmark]
def from_decs_count_m2_eager(b : B?) {
    fixture(N)
    b |> run("m2_eager_bridge/{N}", N) {
        let total = from_decs_template(type<BenchCountRow>).count()
        if (total == 0) {
            b->failNow()
        }
    }
}

// m3: _fold(from_decs_template.count()) — Slice 1 splice target.
[benchmark]
def from_decs_count_m3_fold(b : B?) {
    fixture(N)
    b |> run("m3_fold_splice/{N}", N) {
        let total = _fold(from_decs_template(type<BenchCountRow>).count())
        if (total == 0) {
            b->failNow()
        }
    }
}
