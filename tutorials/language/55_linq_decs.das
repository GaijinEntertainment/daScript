// Tutorial 55: LINQ over DECS
//
// This tutorial covers:
//   - from_decs_template — exposes a [decs_template] type as a linq source
//   - from_decs — same, but driven by a runtime list of component names
//   - _fold over decs — single-pass per-archetype walks (no intermediate array)
//   - Canonical fold shapes: where + count, order_by + first, group_by + select
//   - When to reach for the decs lane vs an in-memory array lane
//
// Prerequisites: tutorials/language/28_linq.das (the linq surface);
//                tutorials/language/34_decs.das (decs entities/components).
//
// Run: daslang.exe tutorials/language/55_linq_decs.das

options gen2
options persistent_heap

require daslib/decs_boost
require daslib/linq
require daslib/linq_boost
require daslib/linq_fold

// === Schema ===
//
// A [decs_template] struct gives the entity rows a stable column shape.
// The `prefix` argument controls the component-name prefix decs uses
// internally — here every Car field becomes a `car_*` component
// (car_id, car_price, car_brand, ...). The prefix is what lets
// `from_decs_template` find the right components per archetype.

[decs_template(prefix = "car_")]
struct Car {
    id    : int
    price : int
    brand : int
}

def seed(n : int) {
    restart()
    create_entities(n) $(eid : EntityId; i : int; var cmp : ComponentMap) {
        apply_decs_template(cmp, Car(
            id    = i + 1,
            price = (i * 37) % 100,
            brand = i % 5
        ))
    }
}

[export]
def main {
    print("=== Tutorial 55: LINQ over DECS ===\n\n")

    seed(20)

    // ─── Section 1 — from_decs_template ─────────────────────────────────
    //
    // from_decs_template(type<T>) opens the live decs state as a linq source
    // whose element type is a tuple matching T's field shape. The macro
    // walks every archetype that carries the [decs_template]'s components,
    // so adding new components later is fine — old archetypes still match.

    print("--- 1. from_decs_template ---\n")
    let total = _fold(from_decs_template(type<Car>).count())
    print("total cars: {total}\n")
    // output: total cars: 20

    // ─── Section 2 — _fold splices the chain ────────────────────────────
    //
    // _fold rewrites a chain like `from_decs_template(...)._where(...).count()`
    // into a single per-archetype walk. There is no intermediate array,
    // no iterator allocation — the splice runs `for_each_archetype` and
    // accumulates inline. Whatever a chain looks like, if there is a
    // matching splice arm in linq_fold.das, the cost stays bounded.

    print("\n--- 2. _fold + _where + count ---\n")
    let pricy = _fold(from_decs_template(type<Car>)._where(_.price > 50).count())
    print("cars with price > 50: {pricy}\n")
    // output: cars with price > 50: 9

    // ─── Section 3 — order_by + first (streaming-min) ───────────────────
    //
    // `_order_by(KEY).first()` is recognized as a streaming-min: the splice
    // keeps a single `var best` + `var seen` per walk, no buffer at all.
    // The dual `_order_by(KEY).take(N)` recognizes a bounded heap of N.

    print("\n--- 3. _fold + _order_by + first ---\n")
    let cheapest = _fold(from_decs_template(type<Car>)._order_by(_.price).first())
    print("cheapest car: id={cheapest.id} price={cheapest.price}\n")
    // output: cheapest car: id=1 price=0

    print("\n--- 3b. _fold + _order_by + take(N) ---\n")
    let top3 <- _fold(from_decs_template(type<Car>)._order_by(_.price).take(3).to_array())
    for (c in top3) {
        print("  id={c.id} price={c.price}\n")
    }
    // output:
    //   id=1 price=0
    //   id=20 price=3
    //   id=12 price=7

    // ─── Section 4 — group_by + select ──────────────────────────────────
    //
    // _group_by + _select fuses into a single-hash group + reduce. With
    // a follow-up `to_array` the splice writes each bucket out once.

    print("\n--- 4. _fold + _group_by + _select(first) ---\n")
    //
    // `_group_by(KEY)` produces one bucket per distinct key. A follow-up
    // `_select` reads `_._0` (the key) and `_._1` (the bucket) — typical
    // SQL "GROUP BY brand, take one example row" shape.
    let bybrand <- _fold(from_decs_template(type<Car>)
                          ._group_by(_.brand)
                          ._select((Brand = _._0, FirstCar = _._1 |> first()))
                          .to_array())
    for (g in bybrand) {
        print("  brand {g.Brand}: first id={g.FirstCar.id} price={g.FirstCar.price}\n")
    }
    // output (order may vary across hash iterations):
    //   brand 0: first id=1 price=0
    //   brand 4: first id=5 price=48
    //   brand 3: first id=4 price=11
    //   brand 2: first id=3 price=74
    //   brand 1: first id=2 price=37

    // ─── Section 5 — when to reach for the decs lane ────────────────────
    //
    // Use `from_decs_template` (the decs lane) when the data already lives
    // as decs entities — you avoid the array copy. Use `each(arr)` (the
    // array lane) when you're starting from an `array<T>` you already
    // built. The fold splice is shape-aware: it picks the per-element
    // representation that matches the source, so the per-element cost
    // is similar across the two lanes for most chains.
    //
    // For the actual SQL / Array / Decs numbers across ~50 chain shapes,
    // see benchmarks/sql/results.md.
    //
    // For the full catalog of what `_fold` recognizes (and what falls back
    // to the iterator-materializing default), see
    // doc/source/reference/linq_fold_patterns.rst.

    print("\n=== End of Tutorial 55 ===\n")
}
