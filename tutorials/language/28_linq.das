// Tutorial 28: LINQ — Language-Integrated Query
//
// This tutorial covers:
//   - require daslib/linq and daslib/linq_boost
//   - Building queries with iterators and arrays
//   - Filtering: where_
//   - Projection: select, select_many
//   - Sorting: order, order_by, order_descending
//   - Partitioning: skip, take, skip_last, take_last, chunk
//   - Aggregation: count, sum, average, min, max, aggregate
//   - Element access: first, last, single, element_at
//   - Set operations: distinct, union, except, intersect
//   - Querying: any, all, none, contains
//   - Joining: zip, join, left_join, group_by
//   - Grouping macros: _group_by, _group_by_lazy, _having
//   - Joining macros: _join, _left_join (with Option<T> for unmatched rows)
//   - Subquery membership: _in, _not_in
//   - The _fold macro for composing pipelines
//   - Shorthand macros: _where, _select, _order_by, _count, _none, etc.
//
// Run: daslang.exe tutorials/language/28_linq.das

options gen2

require daslib/linq_boost

struct Person {
    name : string
    age  : int
}

[export]
def main {
    print("=== Tutorial 28: LINQ ===\n\n")

    // === Overview ===
    // daslang's LINQ module (daslib/linq) provides C#-style query
    // operations for iterators and arrays. daslib/linq_boost adds
    // shorthand macros (_where, _select, etc.) and the _fold macro
    // for composing multi-step pipelines.
    //
    // Most functions come in several flavors:
    //   func(iterator, ...)            → returns lazy iterator
    //   func(array, ...)               → returns new array
    //   func_to_array(iterator, ...)   → materializes into array
    //   func_inplace(var array, ...)   → mutates in place

    // === 1. Filtering — where_ ===
    // where_ filters elements by a predicate.
    // (Named "where_" with a trailing underscore to avoid
    // collision with the built-in "where" keyword.)

    print("--- 1. Filtering ---\n")

    var numbers = [3, 1, 4, 1, 5, 9, 2, 6, 5]

    // Using where_ with a predicate block
    let evens = where_to_array(numbers.to_sequence(), $(x) => x % 2 == 0)
    print("evens: {evens}\n")
    // output: evens: [[ 4; 2; 6]]

    // Shorthand: _where uses _ as the element placeholder
    let big = _where_to_array(numbers.to_sequence(), _ > 4)
    print("big (>4): {big}\n")
    // output: big (>4): [[ 5; 9; 6; 5]]

    // === 2. Projection — select ===
    // select transforms each element through a function.

    print("\n--- 2. Projection ---\n")

    let doubled = _select_to_array(numbers.to_sequence(), _ * 2)
    print("doubled: {doubled}\n")
    // output: doubled: [[ 6; 2; 8; 2; 10; 18; 4; 12; 10]]

    // select_many flattens nested sequences into one
    var nested = [
        ["a", "b", "c"].to_sequence(),
        ["d", "e", "f"].to_sequence()
    ]
    let flat = select_many_to_array(
        nested.to_sequence(),
        $(s : string) => "{s}!"
    )
    print("select_many: {flat}\n")
    // output: select_many: [[ a!; b!; c!; d!; e!; f!]]

    // === 3. Sorting ===
    // order — ascending (by default key or custom comparator)
    // order_by — ascending by a key expression
    // order_descending — descending

    print("\n--- 3. Sorting ---\n")

    let unsorted = [5, 3, 8, 1, 4]

    // Default ascending sort (uses built-in unique_key)
    let asc = order(unsorted)
    print("ascending: {asc}\n")
    // output: ascending: [[ 1; 3; 4; 5; 8]]

    // Default descending sort
    let desc = order_descending(unsorted)
    print("descending: {desc}\n")
    // output: descending: [[ 8; 5; 4; 3; 1]]

    // Sorting structs by a field with _order_by shorthand
    var team = [
        Person(name="Charlie", age=30),
        Person(name="Alice", age=25),
        Person(name="Bob", age=35)
    ]
    var by_age = _order_by(team.to_sequence(), _.age)
    for (p in by_age) {
        print("  {p.name} ({p.age})\n")
    }
    // output:   Alice (25)
    // output:   Charlie (30)
    // output:   Bob (35)

    // === 4. Partitioning ===
    // skip / take — from the front
    // skip_last / take_last — from the end
    // skip_while / take_while — by predicate
    // chunk — split into groups of N

    print("\n--- 4. Partitioning ---\n")

    let seq = [for (x in 0..8); x]

    let skipped = skip(seq, 3)
    print("skip 3: {skipped}\n")
    // output: skip 3: [[ 3; 4; 5; 6; 7]]

    let taken = take(seq, 3)
    print("take 3: {taken}\n")
    // output: take 3: [[ 0; 1; 2]]

    let tail = take_last(seq, 3)
    print("take last 3: {tail}\n")
    // output: take last 3: [[ 5; 6; 7]]

    let head = skip_last(seq, 3)
    print("skip last 3: {head}\n")
    // output: skip last 3: [[ 0; 1; 2; 3; 4]]

    // take_while / skip_while with shorthand
    var tw = _take_while(numbers.to_sequence(), _ < 5)
    let tw_arr = to_array(tw)
    print("take while <5: {tw_arr}\n")
    // output: take while <5: [[ 3; 1; 4; 1]]

    // chunk splits into groups
    var chunks = chunk_to_array(
        [iterator for(x in 0..7); x],
        3
    )
    print("chunks of 3: ")
    for (ch in chunks) {
        print("{ch} ")
    }
    print("\n")
    // output: chunks of 3: [[ 0; 1; 2]] [[ 3; 4; 5]] [[ 6]]

    // === 5. Aggregation ===
    // count, sum, average, min, max, aggregate

    print("\n--- 5. Aggregation ---\n")

    var vals = [10, 20, 30, 40, 50]

    print("count: {count(vals)}\n")
    // output: count: 5

    // count with a predicate
    print("count >25: {count(vals, $(v) => v > 25)}\n")
    // output: count >25: 3

    // _count shorthand
    let cnt = _count(vals.to_sequence(), _ >= 30)
    print("count >=30: {cnt}\n")
    // output: count >=30: 3

    print("sum: {sum(vals.to_sequence())}\n")
    // output: sum: 150

    print("average: {average(vals.to_sequence())}\n")
    // output: average: 30

    print("min: {min(vals.to_sequence())}\n")
    // output: min: 10

    print("max: {max(vals.to_sequence())}\n")
    // output: max: 50

    // aggregate — custom fold with a seed value
    let product = aggregate(vals.to_sequence(), 1, $(acc, v) => acc * v)
    print("product: {product}\n")
    // output: product: 12000000

    // === 6. Element access ===
    // first, last, single, element_at — and _or_default variants

    print("\n--- 6. Element access ---\n")

    print("first: {first(vals.to_sequence())}\n")
    // output: first: 10

    print("last: {last(vals.to_sequence())}\n")
    // output: last: 50

    print("element_at(2): {element_at(vals.to_sequence(), 2)}\n")
    // output: element_at(2): 30

    // _or_default returns a default value when out of range
    print("element_at_or_default(99): {element_at_or_default(vals.to_sequence(), 99)}\n")
    // output: element_at_or_default(99): 0

    // === 7. Querying — any, all, contains ===

    print("\n--- 7. Querying ---\n")

    print("any >40: {any(vals.to_sequence(), $(v) => v > 40)}\n")
    // output: any >40: true

    print("all >5: {all(vals.to_sequence(), $(v) => v > 5)}\n")
    // output: all >5: true

    print("contains 30: {contains(vals.to_sequence(), 30)}\n")
    // output: contains 30: true

    // none — mirror of any, true when no element matches
    print("none >100: {none(vals.to_sequence(), $(v) => v > 100)}\n")
    // output: none >100: true

    // _none — shorthand macro form (mirrors _any)
    print("_none <0: {_none(vals.to_sequence(), _ < 0)}\n")
    // output: _none <0: true

    // === 8. Set operations ===
    // distinct, union, except, intersect

    print("\n--- 8. Set operations ---\n")

    var a = [1, 2, 2, 3, 3, 3] // nolint:LINT003
    var b = [2, 3, 4, 5] // nolint:LINT003

    let dist = distinct(a)
    print("distinct: {dist}\n")
    // output: distinct: [[ 1; 2; 3]]

    let uni = union(a, b)
    print("union: {uni}\n")
    // output: union: [[ 1; 2; 3; 4; 5]]

    let exc = except(a, b)
    print("except: {exc}\n")
    // output: except: [[ 1]]

    let inter = intersect(a, b)
    print("intersect: {inter}\n")
    // output: intersect: [[ 2; 3]]

    // === 9. Zip — combining sequences ===
    // zip merges two (or three) sequences into tuples

    print("\n--- 9. Zip ---\n")

    let names = ["Alice", "Bob", "Charlie"]
    let ages  = [25, 35, 30]

    var zipped = zip(names, ages)
    for (z in zipped) {
        print("  {z._0}: {z._1}\n")
    }
    // output:   Alice: 25
    // output:   Bob: 35
    // output:   Charlie: 30

    // Three-source zip
    let scores = [95, 87, 91]
    var zipped3 = zip(names, ages, scores)
    for (z in zipped3) {
        print("  {z._0} (age {z._1}): score {z._2}\n")
    }
    // output:   Alice (age 25): score 95
    // output:   Bob (age 35): score 87
    // output:   Charlie (age 30): score 91

    // === 10. group_by ===

    print("\n--- 10. Group by ---\n")

    var items = [
        Person(name="Engineering", age=1),
        Person(name="Engineering", age=2),
        Person(name="Marketing", age=3),
        Person(name="Marketing", age=4),
        Person(name="Marketing", age=5)
    ]
    // Group by department (name), select the id (age), produce (dept, count)
    var groups = group_by_to_array(
        items.to_sequence(),
        $(it : Person) => it.name,
        $(it : Person) => it.age,
        $(dept : string, var ids : iterator<int>) => (dept, ids.count())
    )
    for (g in groups) {
        print("  {g._0}: {g._1} members\n")
    }
    // output:   Engineering: 2 members
    // output:   Marketing: 3 members

    // === 11. Group-by macros — _group_by, _group_by_lazy, _having ===
    // _group_by_lazy / _group_by produce buckets as tuple<key, group>
    // where the next stage reads `_._0` for the key and `_._1` for the
    // group's members. _having filters the buckets by a predicate over
    // the bucket itself (typical SQL "GROUP BY ... HAVING ..." shape).
    //
    // _group_by_lazy and _group_by are aliases — same in-memory result
    // (each bucket is a tuple<Key, array<T>>). The two names exist so
    // dasSQLITE's _sql translator can tell intent apart later: "lazy"
    // means user wants the IGrouping shape; "_group_by" tags the shape
    // that will be consumed by an aggregate-shaped _select / _having.

    print("\n--- 11. Group-by macros ---\n")

    let stream = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    // _group_by_lazy: buckets keyed by `x % 3` (lazy/IGrouping shape).
    // Bucket iteration order isn't guaranteed, so we read sizes by key.
    var by_mod3 <- stream._group_by_lazy(_ % 3)
    var bucket_size : table<int; int>
    for (g in by_mod3) {
        bucket_size[g._0] = g._1 |> length
    }
    print("  bucket 0: {bucket_size[0]} elements\n")
    print("  bucket 1: {bucket_size[1]} elements\n")
    print("  bucket 2: {bucket_size[2]} elements\n")
    // output:   bucket 0: 3 elements
    // output:   bucket 1: 4 elements
    // output:   bucket 2: 3 elements

    // _having: keep only buckets of size >= 4 (chained method-style)
    var popular <- (stream
        ._group_by(_ % 3)
        ._having(_._1 |> length >= 4))
    for (g in popular) {
        print("  popular bucket {g._0}: {g._1 |> length} elements\n")
    }
    // output:   popular bucket 1: 4 elements

    // _group_by_to_array — materialize into a plain array of buckets
    let arr_groups <- [iterator for(x in 0..6); x]._group_by_to_array(_ % 2)
    print("group_by_to_array buckets: {arr_groups |> length}\n")
    // output: group_by_to_array buckets: 2

    // === 12. Joining macros — _join, _left_join, _right_join, _full_outer_join, _cross_join ===
    // _join takes the join condition as an `on` lambda — typically an
    // equality between key expressions on the two sides. Only equi-join
    // shapes (`a == b`) are accepted; theta inequalities and `&&`/`||`
    // chains are rejected with macro_error.
    //
    // The four equi-joins (_join, _left_join, _right_join, _full_outer_join)
    // share the 4-arg shape (other, on, into); only the Option-ness of
    // `into`'s args differs:
    //
    //   _join              (a : TA,         b : TB)         — matched pairs only
    //   _left_join         (a : TA,         b : Option<TB>) — every TA, optional TB
    //   _right_join        (a : Option<TA>, b : TB)         — every TB, optional TA
    //   _full_outer_join   (a : Option<TA>, b : Option<TB>) — every TA + every TB
    //
    // _cross_join is the exception — 3-arg form (other, into), no `on`,
    // every (TA, TB) pair surfaces.

    print("\n--- 12. Joining macros ---\n")

    // _join — equi-join on a primitive key
    let ids = [1, 2, 3, 4, 5]
    let matches = [2, 3, 3, 4]   // 3 has two matches; 1 and 5 are unmatched

    var inner = _join(
        ids,
        matches,
        $(la : int, lb : int) => la == lb,
        $(la : int, lb : int) => (la, lb)
    )
    for (row in inner) {
        print("  inner: {row._0} == {row._1}\n")
    }
    // output:   inner: 2 == 2
    // output:   inner: 3 == 3
    // output:   inner: 3 == 3
    // output:   inner: 4 == 4

    // _left_join — keep unmatched left rows; right side is Option<int>
    var lj = _left_join(
        ids,
        matches,
        $(la : int, lb : int) => la == lb,
        $(la : int, lb : Option<int>) => (la, lb |> unwrap_or(-1))
    )
    for (row in lj) {
        print("  left_join: {row._0} -> {row._1}\n")
    }
    // output:   left_join: 1 -> -1
    // output:   left_join: 2 -> 2
    // output:   left_join: 3 -> 3
    // output:   left_join: 3 -> 3
    // output:   left_join: 4 -> 4
    // output:   left_join: 5 -> -1

    // The function form left_join() / left_join_to_array() takes
    // explicit per-side key selectors:
    var lj_arr = left_join_to_array(
        [1, 2, 3].to_sequence_move(),
        [2, 3].to_sequence_move(),
        $(la : int) => la,
        $(lb : int) => lb,
        $(la : int, lb : Option<int>) => (la, lb |> is_some)
    )
    for (row in lj_arr) {
        print("  fn-form: {row._0} matched={row._1}\n")
    }
    // output:   fn-form: 1 matched=false
    // output:   fn-form: 2 matched=true
    // output:   fn-form: 3 matched=true

    // _right_join — keep unmatched right rows; left side is Option<int>
    var rj = _right_join(
        ids,
        matches,
        $(la : int, lb : int) => la == lb,
        $(la : Option<int>, lb : int) => (la |> unwrap_or(-1), lb)
    )
    for (row in rj) {
        print("  right_join: {row._0} -> {row._1}\n")
    }
    // output:   right_join: 2 -> 2
    // output:   right_join: 3 -> 3
    // output:   right_join: 3 -> 3
    // output:   right_join: 4 -> 4

    // _full_outer_join — both sides surface; either may be none
    var fj = _full_outer_join(
        ids,
        matches,
        $(la : int, lb : int) => la == lb,
        $(la : Option<int>, lb : Option<int>) => (la |> unwrap_or(-1), lb |> unwrap_or(-1))
    )
    for (row in fj) {
        print("  full_outer: {row._0} <-> {row._1}\n")
    }
    // output:   full_outer: 1 <-> -1   (left-only)
    // output:   full_outer: 2 <-> 2
    // output:   full_outer: 3 <-> 3
    // output:   full_outer: 3 <-> 3
    // output:   full_outer: 4 <-> 4
    // output:   full_outer: 5 <-> -1   (left-only)

    // _cross_join — Cartesian product, no `on` lambda
    var cj = _cross_join(
        [1, 2],
        ["a", "b", "c"],
        $(la : int, lb : string) => (la, lb)
    )
    for (row in cj) {
        print("  cross: {row._0} x {row._1}\n")
    }
    // output:   cross: 1 x a
    // output:   cross: 1 x b
    // output:   cross: 1 x c
    // output:   cross: 2 x a
    // output:   cross: 2 x b
    // output:   cross: 2 x c

    // === 13. Subquery membership — _in, _not_in ===
    // _in(x, sub) is shorthand for contains(sub, x); _not_in is its
    // negation. The subquery side may be an array or any iterator
    // (including an inline LINQ pipeline).

    print("\n--- 13. Subquery membership ---\n")

    let haystack = [1, 2, 3]
    print("3 _in   haystack: {_in(3, haystack)}\n")
    print("42 _in  haystack: {_in(42, haystack)}\n")
    print("42 _not_in haystack: {_not_in(42, haystack)}\n")
    // output: 3 _in   haystack: true
    // output: 42 _in  haystack: false
    // output: 42 _not_in haystack: true

    // Subquery side can be a fresh iterator
    print("2 in 0..5: {_in(2, [iterator for(x in 0..5); x])}\n")
    // output: 2 in 0..5: true

    // === 14. The _fold macro ===
    // _fold composes multiple LINQ operations into an optimized pipeline.
    // It uses dot-chaining: array.op1().op2()...._fold()
    // The _fold macro rewrites the chain into efficient imperative code.

    print("\n--- 14. _fold pipeline ---\n")

    // Find the sum of squares of even numbers
    let sum_of_even_squares = (
        [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
        ._where(_ % 2 == 0)
        ._select(_ * _)
        .sum()
        ._fold()
    )
    print("sum of even squares: {sum_of_even_squares}\n")
    // output: sum of even squares: 220

    // Get the 3 largest values, deduplicated
    let top3 = (
        [5, 3, 8, 1, 4, 8, 3, 9, 2, 9]
        .order_descending()
        .distinct()
        .take(3)
        ._fold()
    )
    print("top 3: {top3}\n")
    // output: top 3: [[ 9; 8; 5]]

    // Complex pipeline: filter → transform → sort → take
    let result = (
        [
            Person(name="Alice", age=25),
            Person(name="Bob", age=17),
            Person(name="Charlie", age=30),
            Person(name="Diana", age=15),
            Person(name="Eve", age=22)
        ]
        ._where(_.age >= 18)
        ._select(_.name)
        ._order_by(_)
        ._fold()
    )
    print("adults sorted: {result}\n")
    // output: adults sorted: [[ Alice; Charlie; Eve]]

    print("\n=== End of Tutorial 28 ===\n")
}
