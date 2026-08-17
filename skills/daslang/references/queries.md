# Queries — Comprehensions and LINQ

Transforming a sequence — filter, map, sort, group, aggregate, materialize. The preference order is
fixed:

1. **Comprehension** — when one `[for (x in src); expr; where cond]` (or its table form) covers the
   whole transformation.
2. **LINQ** — multi-step chains, lazy iterators, set operations, joins, aggregations.
3. **Plain `for`** — when the body has a side effect or needs `break`.

`daslib/functional` (`map`, `filter`, `each`, `to_array`) is the legacy surface: it is not
integrated with the chain macros and does not fuse. Use it only in code already built on it.

## Comprehensions

```das
let names <- [for (x in func.arguments); string(x.name)]        // map
let positive <- [for (x in arr); x; where x > 0]                // map + filter
let squares <- [for (i in range(10)); i * i]                    // range source
let pairs <- [for (k, v in keys(t), values(t)); Pair(k = k, v = v)]   // parallel sources
let lookup <- {for (x in items); x.key => x.value}              // table form
```

A comprehension is one fused loop with no intermediate iterator, so it wins whenever **all** of
these hold: one source (or two iterated in parallel), a single-expression body, at most one `where`
clause, and an `array<T>` / `table<K;V>` result. Chaining (filter, then sort, then take) or a lazy
iterator result means LINQ.

## LINQ

```das
require daslib/linq_boost        // re-exports daslib/linq and the fold family — never require both
```

Everything a comprehension cannot express:

| Group | Operators |
|---|---|
| Filter / project | `where_`, `having_`, `select`, `select_many`, `select_many_pair`, `zip`, `prepend`, `append` |
| Aggregate | `count`, `long_count`, `sum`, `average`, `min`, `max`, `min_by`, `max_by`, `min_max*`, `aggregate` |
| Set | `distinct`, `union`, `except`, `intersect`, `unique` and their `*_by` twins |
| Element | `first`, `last`, `single`, `element_at`, each with an `*_or_default` twin |
| Test | `any`, `all`, `none`, `contains`, `sequence_equal`, `sequence_equal_by` |
| Partition | `skip`, `take`, `skip_while`, `take_while`, `skip_last`, `take_last`, `chunk`, the `top_n` family |
| Order | `order_by`, `order_by_descending`, `order`, `order_descending`, `order_by_keys`, `reverse` |
| Join / group | `join`, `group_join`, `left_join`, `right_join`, `full_outer_join`, `cross_join`, `group_by`, `group_by_lazy` |
| Materialize | `to_array`, `to_table`, and a `*_to_array` twin for most of the above |

The filter is spelled **`where_`** with a trailing underscore (and the group filter `having_`
likewise), because `where` is the comprehension keyword. There is also an `*_inplace` family that
the fold machinery selects on its own for a mutable local source — don't call those by hand.

**Two-source operators come in four shapes** — iterator+iterator, iterator+array, array+iterator,
array+array — so a lazy source can meet an in-memory array without materializing. **Do not append
`_to_array` to an array+array call:** the base overload already returns `array<T>`, only the
iterator-carrying shapes are lazy, so `union_to_array(a, b)` on two arrays is `error[30341]`. Call
the bare op. `zip` is N-ary up to 8 sources; for three or more, mix operands with `each(arr)`.

### `_fold` — the default terminator

`_fold` rewrites a chain to stay in **array form** end to end: each step becomes a pass against an
intermediate `array<T>`, picking in-place ops where the source is a mutable local. That is cheaper
than the lazy form on essentially every input. **It goes last, as a trailing call:**

```das
let names <- arr._where(_.flag)._select(_.name)._fold()
let xs    <- arr |> _select(_ * 2) |> to_array() |> _fold()
```

The wrapping form `_fold(chain)` works only when the chain already ends in a terminator
(`to_array` / `count` / `to_table` / …); over a bare trailing `_select` it fails to infer the
selector. Trailing position has no such restriction. Skip `_fold` only when you genuinely need a
lazy `iterator<T>` to compose into a larger pipeline.

The `_<op>` shorthands come from `linq_boost`: `_<op>(iter, expr)` expands to
`<op>(iter, $(_) => expr)` with `_` as the implicit element. They cover the common operators
(`_where`, `_select`, `_count`, `_any`, `_order_by`, `_group_by`, `_distinct_by`, `_join`, …), and
most have a `_<op>_to_array` twin that materializes in one step.

### Writing a chain that compiles

Use dot form for the steps, end with a trailing `._fold()`, and pipe into any non-LINQ consumer.

```das
let s     = (arr._select("{_:d}")._fold()) |> join(", ")
let names = (arr._where(_.flag)._select(_.name)._fold()) |> join(", ")
let dims  = (each(range(N))._select("{_:d}").to_array()._fold()) |> join(", ")
```

- **The shorthands need an iterator or an array receiver.** `arr._select(…)` works; `range(N)`
  produces a `range` value, not an iterator, and fails with `error[50503] expecting iterator or
  array` — write `each(range(N))._select(…)`. Same for a C++-bound vector field.
- **`each(...)` is `[unsafe_outside_of_for]`**, arrays included. Inside a `_fold` chain the macro
  peels it before inference, so no wrap is needed there; feeding `each(arr)` to a plain LINQ call
  needs `unsafe(each(arr))`.
- **`_fold`'s output type follows the source, not the spelling.** An `array<T>` source stays an
  array; an iterator source folds to `iterator<T>`, so add `.to_array()` before `._fold()`.
- **A multi-line chain needs surrounding parentheses** — statement-level newlines are significant
  outside brackets.
- **`_` is local to the closest enclosing `_<op>(...)`.** Give inner closures explicit names
  (`@@(x) => …`) rather than nesting the placeholder. It substitutes correctly inside string
  interpolation and inside a `build_string() $(w) { … }` body.
- **String `join(arr, sep)` lives in `strings` / `strings_boost`**; LINQ's `join` is the SQL-style
  two-source equi-join. They coexist and the typer picks by argument types — an error about a
  "missing argument blk" on a join call usually means the string module is not required.
- **Join selectors need typed parameters** — bare `@@(c) => c.id` gives `error[30341]` plus
  `error[30928] can't get field 'id' of auto const&` — and struct construction inside one takes
  **named** arguments (`Pair(c = c, o = o)`, never `Pair(c, o)`).

### Table sources and the `to_table` sink

A `table<K;V>` (or a `table<K>` set) is a first-class source; wrap the head in `unsafe` because the
sources are `[unsafe_outside_of_for]`.

```das
let pricey = _fold(unsafe(each_kv(cars)) |> _where(_.value.price > 500) |> count())
let ids <- _fold(unsafe(keys(cars)) |> _where(_ > 100) |> to_array())

var byId <- _fold(each(orders) |> _select(_.id => _.total) |> to_table())
```

`to_table()` lands a chain in a table — a kv (or any `k => v` tuple) chain gives `table<K;V>`, a
scalar chain gives the `table<K>` set form, and duplicate keys keep the last occurrence. Table slot
order is unspecified, so never write an order-sensitive expectation over a table chain.

## What fuses, and what falls back

`_fold` recognizes chain shapes and splices each into one specialized loop; a shape it does not
recognize still produces the right answer, just through the ordinary materializing surface.

**Fuses** — filters and projections into the loop body; every aggregate, element, and test
terminator; `take` / `skip` / `take_while` / `skip_while`; `to_array` and the selector-free
`to_table()`; `order_by |> first` as a streaming minimum with no buffer; `order_by |> take(N)` as a
bounded heap of N; `distinct` / `distinct_by` as a single hash-set lane, including as a gate on the
bounded heap; `group_by` with a reducing `select`, plus a HAVING filter and a trailing `order_by`;
equi-joins on a **primitive** key (`group_join` on array and table leads only); `zip` over 2–8
sources. Sources fuse symmetrically: arrays, tables (`each_kv` / `keys` / `values`), decs
templates, `from_json`, and `from_xml_node` — each pruned to the fields and lanes the chain
actually reads, so an unread string field is never materialized.

**Falls back** — `left_join` / `right_join` / `full_outer_join` / `cross_join`; any join whose key
is not primitive (tuple keys); mixed-source `union` / `except` / `intersect` / `concat` once the
first source has been transformed; the 3-argument `to_table(key, elementSelector)`; a chained
`_select(f) |> _select(g)` whose inner selector has side effects; aggregations over
`group_by_lazy` with a non-reducing `select`.

**Some shapes fuse only in one order** — reorder rather than accept the fallback:
`reverse().take(N)._select(F)` fuses where `reverse()._select(F).take(N)` does not; `distinct`
combines with `order_by` only when a `take` bounds it, and `take(N).distinct()` bails because it
would dedup before the take; `_order_by(K2)._distinct_by(K1)` bails while the distinct-first order
fuses; a `_select` placed *before* `_skip_while` / `_take_while` blocks the match — push it past
them, since those predicates run on the source element, not on the projection.

A chain over a decs-template source that no arm claims warns at compile time, naming the call site,
because the bridge materializes a temporary array before the fallback runs. Suppress it per file
with `options _no_linq_perf_warn = true` when the cascade is intentional.

## Don't mix styles

Pick one style per transformation. A comprehension is a complete transformation — don't drop
`_select` / `_where` / `_fold` onto its result, and don't feed it into a sequence-consuming
primitive (LINQ `join` / `concat` / `zip` / `aggregate`, or string `join(arr, sep)`): the moment
one appears, the upstream should be LINQ too. Equally, don't drop a comprehension mid-chain.

```das
// Bad — comprehension feeding a sequence operator, and LINQ handing off to a comprehension:
let pairs <- join([for (x in customers); x], orders, …)
let names <- [for (x in arr |> _where(_.flag)); x.name]

// Good:
let pairs <- customers |> join(orders,
    @@(c : Cust) => c.id,
    @@(o : Ord) => o.customer_id,
    @@(c : Cust; o : Ord) => Pair(c = c, o = o))
let names <- arr._where(_.flag)._select(_.name)._fold()
```

## When a plain `for` is right

Use a loop when the body has a side effect (writing to a builder, mutating shared state, calling
something with a `var` out-parameter), when you need `break` / `continue`, or when the iteration
exists only for its effect. Do not push side effects into `_select` lambdas — they are meant to be
pure, and a side effect inside a chain observes an evaluation order you did not choose.
