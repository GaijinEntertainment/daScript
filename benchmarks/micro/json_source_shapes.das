options gen2
options persistent_heap

require daslib/json_boost
require daslib/linq_boost
require dastest/testing_boost
require strings

// Phase-0 micro-bench for the linq_json arc: the candidate emit shapes for a filter+project
// JSON-source chain — `from c : Car in carsJson where c.price > THRESHOLD select c.name`.
// Pre-build sign-off per the profile-hand-coded-before-macro methodology, deciding which lane
// the future JsonAdapter should emit (does field-pruning pay off the way it does for XML's string
// clones, and is the fused `invoke` loop worth it over a from_json generator + lazy chain).
//
//   A — field-pruned fused loop (the JsonAdapter target): reads only price + name per element.
//   B — full-row fused: from_JV materializes all 6 fields (incl. name clone) per element, then filter.
//   C — the shipped JsonAdapter via `_fold` — a regression check that the macro fuses to shape A.
//       Pre-adapter (the original Phase-0 measurement) this was the 3.8×-slower unfused generator path;
//       building daslib/linq_fold_json.das collapsed it onto A. A vs B (180 vs 651 ns/op INTERP) is the
//       stable field-pruning headline; C should now track A.
//
// Self-contained (no _common dependency) so it stays out of the results.md sweep.

let N = 100000
let BRAND_COUNT = 5
let DEALER_COUNT = 100
let THRESHOLD = 500

struct Car {
    id        : int
    name      : string
    price     : int
    brand     : int
    year      : int
    dealer_id : int
}

// Same deterministic row generator as fixture_array / fixture_xml_string in benchmarks/sql/_common.das,
// so the JSON lane is directly comparable. Built as an array<JsonValue?> of reflective JV(Car) objects.
def fixture_json(n : int) : JsonValue? {
    var arr : array<JsonValue?>
    arr |> reserve(n)
    for (i in range(n)) {
        arr |> push(JV(Car(
            id = i + 1,
            name = "Car{i}",
            price = (i * 37) % 1000,
            brand = i % BRAND_COUNT,
            year = 2010 + (i * 7) % 16,
            dealer_id = (i % DEALER_COUNT) + 1
        )))
    }
    return JV(arr)
}

// A — field-pruned fused (the JsonAdapter target): read price + name only.
def shape_A(jv : JsonValue?) : array<string> {
    var result : array<string>
    if (jv != null && jv.value is _array) {
        for (elem in jv.value as _array) {
            if ((elem?["price"] ?? 0) > THRESHOLD) {
                result |> push_clone(elem?["name"] ?? "")
            }
        }
    }
    return <- result
}

// B — full-row fused: from_JV reads all 6 fields per element (incl. the name clone), then filter.
def shape_B(jv : JsonValue?) : array<string> {
    var result : array<string>
    if (jv != null && jv.value is _array) {
        for (elem in jv.value as _array) {
            let c = from_JV(elem, type<Car>)
            if (c.price > THRESHOLD) {
                result |> push_clone(c.name)
            }
        }
    }
    return <- result
}

// C — the shipped JsonAdapter via _fold: the macro fuses this chain to shape A's field-pruned loop.
def shape_C(jv : JsonValue?) : array<string> {
    return <- _fold(unsafe(from_json(jv, type<Car>))._where(_.price > THRESHOLD)._select(_.name).to_array())
}

// Touch the whole result so the projection can't be elided.
def checksum(rows : array<string>) : int {
    var s = 0
    for (r in rows) {
        s += length(r)
    }
    return s
}

def run_A(b : B?) {
    let jv = fixture_json(N)
    b |> run("A_pruned_fused/{N}", N) {
        let r <- shape_A(jv)
        if (checksum(r) == 0) {
            b->failNow()
        }
    }
}

def run_B(b : B?) {
    let jv = fixture_json(N)
    b |> run("B_fullrow_fused/{N}", N) {
        let r <- shape_B(jv)
        if (checksum(r) == 0) {
            b->failNow()
        }
    }
}

def run_C(b : B?) {
    let jv = fixture_json(N)
    b |> run("C_fold_adapter/{N}", N) {
        let r <- shape_C(jv)
        if (checksum(r) == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def probe_A_pruned_fused(b : B?) {
    run_A(b)
}

[benchmark]
def probe_B_fullrow_fused(b : B?) {
    run_B(b)
}

[benchmark]
def probe_C_fold_adapter(b : B?) {
    run_C(b)
}
