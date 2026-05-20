options gen2
options persistent_heap
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost
require daslib/decs_boost
require daslib/linq
require daslib/linq_boost

// Pattern 3 motivation benchmark: from_decs* + linq chain vs hand-written query.
// Establishes the current (eager-bridge + to_sequence + lambda-per-element) cost
// so the new splice can be measured against a clear baseline + the hand-written
// query ideal.

let N = 100000
let THRESHOLD = 50

[decs_template(prefix = "bench_")]
struct BenchSumRow {
    val : int
}

def fixture(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, BenchSumRow(val = (i * 37) % 1000))
    }
}

// m1: hand-written query — the splice's ideal target (no iterator, no lambda,
// per-element body inlined into the archetype walk).
[benchmark]
def from_decs_sum_m1_query_hand(b : B?) {
    fixture(N)
    b |> run("m1_query_hand/{N}", N) {
        var total = 0
        query() $(bench_val : int) {
            if (bench_val > THRESHOLD) {
                total += bench_val
            }
        }
        if (total == 0) {
            b->failNow()
        }
    }
}

// m2: from_decs_template eager bridge (current).
// from_decs_template(type<Foo>)._where(p)._select(s).sum() →
//   eager bridge materializes array<tuple<val:int>>, wraps with to_sequence,
//   downstream chain pays lambda-per-element on top of materialization.
[benchmark]
def from_decs_sum_m2_template_eager(b : B?) {
    fixture(N)
    b |> run("m2_from_decs_template_eager/{N}", N) {
        let total = (
            from_decs_template(type<BenchSumRow>)
            ._where(_.val > THRESHOLD)
            ._select(_.val)
            .sum()
        )
        if (total == 0) {
            b->failNow()
        }
    }
}

// m3: from_decs block-form eager bridge (same path as m2 but via the older,
// non-template macro). Same cost class.
[benchmark]
def from_decs_sum_m3_block_eager(b : B?) {
    fixture(N)
    b |> run("m3_from_decs_block_eager/{N}", N) {
        let total = (
            from_decs($(bench_val : int){})
            ._where(_.bench_val > THRESHOLD)
            ._select(_.bench_val)
            .sum()
        )
        if (total == 0) {
            b->failNow()
        }
    }
}

// m4: from_decs_template + _fold(...) (post-Phase 2/3 splice may or may not
// fire here depending on planner-cascade coverage). Reveals whether the existing
// linq_fold splice handles the from_decs* eager bridge at all on master baseline.
[benchmark]
def from_decs_sum_m4_template_fold(b : B?) {
    fixture(N)
    b |> run("m4_from_decs_template_fold/{N}", N) {
        let total = _fold(
            from_decs_template(type<BenchSumRow>)
            ._where(_.val > THRESHOLD)
            ._select(_.val)
            .sum()
        )
        if (total == 0) {
            b->failNow()
        }
    }
}
