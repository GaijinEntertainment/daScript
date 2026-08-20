# Queries — Comprehensions and LINQ

Preference order for filter / map / sort / group / aggregate / materialize:

1. **Comprehension** — one `[for (x in src); expr; where cond]` (or its table form) covers it all.
2. **LINQ** — multi-step chains, lazy iterators, set operations, joins, aggregations.
3. **Plain `for`** — side effect in the body, or `break` / `continue`.

`daslib/functional` (`map`, `filter`, `each`, `to_array`) is legacy — not integrated with the chain
macros, does not fuse. Only for code already built on it.

## Comprehensions

```das
let names <- [for (x in func.arguments); string(x.name)]
let positive <- [for (x in arr); x; where x > 0]
let pairs <- [for (k, v in keys(t), values(t)); Pair(k = k, v = v)]
let lookup <- {for (x in items); x.key => x.value}
```

One fused loop, no intermediate iterator. It wins whenever **all** hold: one source (or two iterated
in parallel), a single-expression body, at most one `where` clause, an `array<T>` / `table<K;V>`
result.

## LINQ

```das
require daslib/linq_boost        // re-exports daslib/linq and the fold family — never require both
```

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

The fold machinery selects the `*_inplace` family on its own for a mutable local source; don't call
those by hand.

**Two-source operators come in four shapes** (iterator/array × iterator/array). **Never append
`_to_array` to an array+array call:** the base overload already returns `array<T>`, so
`union_to_array(a, b)` on two arrays is `error[30341]`. `zip` is N-ary up to 8 sources; for three or
more, mix operands with `each(arr)`.

### `_fold` — the default terminator

`_fold` rewrites a chain to stay in **array form** end to end, cheaper than the lazy form on
essentially every input. **It goes last, as a trailing call:**

```das
let names <- arr._where(_.flag)._select(_.name)._fold()
```

The wrapping form `_fold(chain)` needs a chain already ending in a terminator (`to_array` / `count`
/ `to_table` / …) — over a bare trailing `_select` it fails to infer the selector; trailing position
has no such restriction. Skip `_fold` only for a lazy `iterator<T>` composed into a larger pipeline.

`linq_boost`'s `_<op>(iter, expr)` shorthand expands to `<op>(iter, $(_) => expr)` for the common
operators, `_` the implicit element; most have a `_<op>_to_array` twin that materializes in one step.

### Writing a chain that compiles

```das
let s    = (arr._select("{_:d}")._fold()) |> join(", ")
let dims = (each(range(N))._select("{_:d}").to_array()._fold()) |> join(", ")
```

- **The shorthands need an iterator or an array receiver.** `range(N)` is a `range` value, not an
  iterator: `error[50503] expecting iterator or array` — write `each(range(N))._select(…)`. Same for
  a C++-bound vector field.
- **`each(...)` is `[unsafe_outside_of_for]`**, arrays included. A `_fold` chain peels it before
  inference; a plain LINQ call needs `unsafe(each(arr))`.
- **`_fold`'s output type follows the source, not the spelling** — an iterator source folds to
  `iterator<T>`; add `.to_array()` before `._fold()`.
- **A multi-line chain needs surrounding parentheses.**
- **`_` is local to the closest enclosing `_<op>(...)`.** Name inner closures (`@@(x) => …`); don't
  nest the placeholder. It does substitute inside string interpolation and inside a
  `build_string() $(w) { … }` body.
- **String `join(arr, sep)` lives in `strings` / `strings_boost`**; LINQ's `join` is the SQL-style
  two-source equi-join. The typer picks by argument types — "missing argument blk" on a join call
  usually means the string module is not required.
- **Join selectors need typed parameters** — bare `@@(c) => c.id` gives `error[30341]` plus
  `error[30928] can't get field 'id' of auto const&`; struct construction inside one takes **named**
  arguments (`Pair(c = c, o = o)`, never `Pair(c, o)`).

### Table sources and the `to_table` sink

A `table<K;V>` (or `table<K>` set) is a first-class source; its head needs the same `unsafe` wrap.

```das
let pricey = _fold(unsafe(each_kv(cars)) |> _where(_.value.price > 500) |> count())
let ids <- _fold(unsafe(keys(cars)) |> _where(_ > 100) |> to_array())

var byId <- _fold(each(orders) |> _select(_.id => _.total) |> to_table())
```

`to_table()` gives `table<K;V>` from a kv (or any `k => v` tuple) chain, the `table<K>` set form
from a scalar chain; duplicate keys keep the last occurrence. Slot order is unspecified — never
write an order-sensitive expectation over a table chain.

## What fuses, and what falls back

`_fold` splices each recognized shape into one specialized loop; an unrecognized shape still gives
the right answer, materializing.

**Fuses** — filters and projections into the loop body; every aggregate, element and test
terminator; `take` / `skip` / `take_while` / `skip_while`; `to_array` and the selector-free
`to_table()`; `order_by |> first` as a streaming minimum, no buffer; `order_by |> take(N)` as a
bounded heap of N; `distinct` / `distinct_by` as one hash-set lane, also as a gate on that heap;
`group_by` with a reducing `select`, plus a HAVING filter and a trailing `order_by`; equi-joins on a
**primitive** key (`group_join` on array and table leads only); `zip` over 2–8 sources. Sources fuse
symmetrically — arrays, tables (`each_kv` / `keys` / `values`), decs templates, `from_json`,
`from_xml_node` — each pruned to the fields and lanes the chain reads.

**Falls back** — `left_join` / `right_join` / `full_outer_join` / `cross_join`; any join whose key
is not primitive (tuple keys); mixed-source `union` / `except` / `intersect` / `concat` once the
first source has been transformed; the 3-argument `to_table(key, elementSelector)`; a chained
`_select(f) |> _select(g)` whose inner selector has side effects; aggregations over `group_by_lazy`
with a non-reducing `select`.

**Some shapes fuse only in one order** — reorder rather than accept the fallback:
`reverse().take(N)._select(F)` fuses where `reverse()._select(F).take(N)` does not; `distinct`
combines with `order_by` only when a `take` bounds it, and `take(N).distinct()` bails;
`_order_by(K2)._distinct_by(K1)` bails while the distinct-first order fuses; a `_select` placed
*before* `_skip_while` / `_take_while` blocks the match — push it past them, those predicates
running on the source element, not on the projection.

A chain over a decs-template source that no arm claims warns at compile time, naming the call site.
Suppress it per file with `options _no_linq_perf_warn = true`.

## Don't mix styles

One style per transformation. A comprehension is complete: don't drop `_select` / `_where` / `_fold`
onto its result, don't feed it into a sequence-consuming primitive (LINQ `join` / `concat` / `zip` /
`aggregate`, or string `join(arr, sep)`) — the moment one appears, the upstream should be LINQ too —
and don't drop a comprehension mid-chain.

```das
// Bad:
let pairs <- join([for (x in customers); x], orders, …)

// Good:
let pairs <- customers |> join(orders,
    @@(c : Cust) => c.id,
    @@(o : Ord) => o.customer_id,
    @@(c : Cust; o : Ord) => Pair(c = c, o = o))
```

`_select` lambdas are meant to be pure; a side effect inside a chain observes an evaluation order
you did not choose.
