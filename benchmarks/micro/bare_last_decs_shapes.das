options gen2
options persistent_heap

require daslib/linq_fold public
require daslib/linq_boost public
require daslib/decs_boost public
require dastest/testing_boost public
require strings

// Micro-bench: bare `last()` (no predicate, no range, no projection) over a decs / array source.
// Question: is decs `last()` an O(N) full forward walk today, and how much would a tail fast path save?
//
//   C (current)  — `source.last()`: the no-predicate-last fast path is unimplemented for decs, so it
//                  falls through emit_early_exit_lane to the generic full walk, rebinding the whole row
//                  on EVERY element. O(N).
//   A (fast path) — `source.reverse().take(1).to_array()[0]`: rides the SHIPPED skip-into-tail machinery
//                   (emit_decs_reverse_skip_into_tail / array equivalent) — sums archetype sizes, skips
//                   whole archetypes to the boundary, collects only the last element. O(num_archetypes).
//                   A faithful proxy for what a wired-up `last()` fast path would emit (it would return the
//                   scalar directly instead of a 1-element array, so it'd be marginally faster still).
//   count        — `source.count()`: the arch.size O(1) shortcut. Touch-no-entities floor for the decs walk.
//
// Self-contained (no _common dependency) so it stays out of the results.md sweep over benchmarks/sql/.

let N = 100000
let BRAND_COUNT = 5
let DEALER_COUNT = 100

struct Car {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

[decs_template(prefix = "car_")]
struct DecsCar {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

def fixture_array(n : int) : array<Car> {
    var arr : array<Car>
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = Car(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        )
    }
    return <- arr
}

def fixture_decs(n : int) {
    restart()
    create_entities(n) $(_eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, DecsCar(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        ))
    }
}

// touch the whole survivor (id + name length) so the name clone can't be elided.
// generic so it accepts both the array Car struct and the decs source's anonymous named tuple.
def check(c) : int {
    return c.id + length(c.name)
}

// ===== decs lanes =====

def run_decs_last_current(b : B?) {
    fixture_decs(N)
    b |> run("C_decs_last_current/{N}", N) {
        let r = _fold(from_decs_template(type<DecsCar>).last())
        if (check(r) == 0) { b->failNow() }
    }
}

def run_decs_last_tail(b : B?) {
    fixture_decs(N)
    b |> run("A_decs_last_tail/{N}", N) {
        unsafe {
            let rows <- _fold(from_decs_template(type<DecsCar>).reverse().take(1).to_array())
            if (empty(rows) || check(rows[0]) == 0) { b->failNow() }
        }
    }
}

def run_decs_count_ref(b : B?) {
    fixture_decs(N)
    b |> run("R_decs_count_archsize/{N}", N) {
        let c = _fold(from_decs_template(type<DecsCar>).count())
        if (c != N) { b->failNow() }
    }
}

// B (true floor) — random-index the last entity of the last non-empty archetype. for_each_archetype
// visits matching archetypes in order, skipping empties, so the last one visited holds the global-last;
// read entity [size-1] directly via get_ro (no per-entity walk). O(num_archetypes), NOT O(N). This is
// what a proper decs `last()` fast path would emit — the existing reverse-into-tail can't reach it
// because it continue-walks the boundary archetype instead of indexing into it.
def run_decs_last_floor(b : B?) {
    fixture_decs(N)
    var erq : EcsRequest
    erq.req <- ["car_id", "car_name", "car_price", "car_brand", "car_year", "car_dealer_id"]
    compile_request(erq)
    b |> run("B_decs_last_floor/{N}", N) {
        var res : Car
        var any = false
        for_each_archetype(erq) $(arch : Archetype) {
            unsafe {
                let li = int(arch.size) - 1
                res.id = get_ro(arch, "car_id", 0)[li]
                res.name := get_ro(arch, "car_name", "")[li]
                res.price = get_ro(arch, "car_price", 0)[li]
                res.brand = get_ro(arch, "car_brand", 0)[li]
                res.year = get_ro(arch, "car_year", 0)[li]
                res.dealer_id = get_ro(arch, "car_dealer_id", 0)[li]
            }
            any = true
        }
        if (!any || check(res) == 0) { b->failNow() }
    }
}

// ===== array lanes (context) =====

def run_array_last_current(b : B?) {
    let arr <- fixture_array(N)
    b |> run("C_array_last_current/{N}", N) {
        let r = _fold(each(arr).last())
        if (check(r) == 0) { b->failNow() }
    }
}

def run_array_last_tail(b : B?) {
    let arr <- fixture_array(N)
    b |> run("A_array_last_tail/{N}", N) {
        unsafe {
            let rows <- _fold(each(arr).reverse().take(1).to_array())
            if (empty(rows) || check(rows[0]) == 0) { b->failNow() }
        }
    }
}

[benchmark]
def probe_decs_last_C_current(b : B?) {
    run_decs_last_current(b)
}

[benchmark]
def probe_decs_last_A_tail(b : B?) {
    run_decs_last_tail(b)
}

[benchmark]
def probe_decs_count_R_ref(b : B?) {
    run_decs_count_ref(b)
}

[benchmark]
def probe_decs_last_B_floor(b : B?) {
    run_decs_last_floor(b)
}

[benchmark]
def probe_array_last_C_current(b : B?) {
    run_array_last_current(b)
}

[benchmark]
def probe_array_last_A_tail(b : B?) {
    run_array_last_tail(b)
}
