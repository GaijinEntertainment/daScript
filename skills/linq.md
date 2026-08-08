# LINQ and comprehensions in daslang

Read this skill before writing any `.das` code that transforms a sequence — filtering, mapping, sorting, grouping, aggregating, materializing into an array/table. The fixed preference order is:

1. **Comprehension** — if a single `[for (x in src); expr; where cond]` (or `{... => ...}` for tables) covers the whole transformation
2. **LINQ** — for everything else (multi-step chains, lazy iterators, set ops, joins, aggregations)
3. **Plain `for` loop** — when there is a side effect (writing to a builder, mutating shared state) or an early `break`

Do NOT use `daslib/functional` (`map`, `filter`, `each`, `to_array`) for new code. The linq operators (`select`, `where_`, `_select`, `_where`, `to_array`, …) are the supported chainable ones; `daslib/functional` is older, less integrated with the pipe-syntax macros, and patterns built on it have been the source of fusion-runtime bugs.

## When comprehension wins

Comprehensions are tighter and faster (one fused loop, no intermediate iterators) for the cases they cover.

```das
// Map only:
let names <- [for (x in func.arguments); string(x.name)]

// Map + where:
let positive <- [for (x in arr); x; where x > 0]

// Map + transform:
let bytes <- [for (c in s); uint8(c)]

// Range source:
let squares <- [for (i in range(10)); i * i]

// Multiple sources zipped pair-wise (parallel iteration):
let pairs <- [for (k, v in keys(t), values(t)); CapturedVariable(variable=k, expression=v)]

// Tables:
let lookup <- {for (x in items); x.key => x.value}
```

Use a comprehension when **all** of these hold:
- One source iterable (or two iterated in parallel — see `keys/values` example).
- The body is a single expression.
- The filter, if any, fits a single `where` clause.
- You want an `array<T>` (or `table<K;V>`) result.

If you need to chain (filter, then sort, then take), or you want a lazy iterator out, jump to LINQ.

## When to use LINQ

LINQ is the chainable iterator surface in `daslib/linq` plus the pipe/dot-syntax shorthand macros in `daslib/linq_boost`. The `_fold` machinery is its own 7-file family (`daslib/linq_fold.das` + `linq_fold_common` / `_array` / `_decs` / `_json` / `_table` / `_sql`), which `linq_boost` re-exports publicly — so `require daslib/linq_boost` still pulls everything. LINQ covers everything comprehension can't:

- Multiple wheres, multiple selects, sorts, groupings, joins, set operations.
- Aggregations: `count`, `long_count`, `sum`, `average`, `min`, `max`, `min_by`, `max_by`, `min_max`, `min_max_by`, `min_max_average`, `min_max_average_by`, `aggregate`.
- Set ops: `distinct`, `distinct_by`, `union`, `union_by`, `except`, `except_by`, `intersect`, `intersect_by`, `unique`, `unique_by`.
- Element ops: `first`, `first_or_default`, `last`, `last_or_default`, `single`, `single_or_default`, `element_at`, `element_at_or_default`.
- Querying: `any`, `all`, `none`, `contains`, `sequence_equal`, `sequence_equal_by`.
- Partitioning: `skip`, `skip_while`, `take`, `take_while`, `skip_last`, `take_last`, `chunk`, and the `top_n` family (`top_n`, `top_n_by`, `top_n_descending`, `top_n_by_descending`, `top_n_by_with_cmp`).
- Ordering: `order_by`, `order_by_descending`, `order`, `order_descending`, `order_by_keys`, `reverse`.
- Joins/groups: `join`, `group_join`, `group_by`, `group_by_lazy`, `having_`.
- Transforms: `select`, `select_to_array`, `select_many`, `select_many_pair`, `zip`, `prepend`, `append`.
- Materialize: `*_to_array` variants for any of the above.
- Mutating twins: an `*_inplace` family (`order_by_inplace`, `distinct_inplace`, `reverse_inplace`, `take_inplace`, …) that `_fold` selects automatically when the source is a mutable local array. Don't call these by hand — let `_fold` pick them.

The filter operator is spelled **`where_`** with a trailing underscore (and the group filter `having_` likewise) — `where` is the reserved comprehension keyword. The `_where` / `_having` shorthands have no underscore problem.

The **binary two-source** ops — `join` / `group_join` / `left_join` / `right_join` / `full_outer_join` / `cross_join`, `union` / `union_by` / `intersect` / `intersect_by` / `except` / `except_by` / `concat`, and `sequence_equal` / `sequence_equal_by` — come in four shapes: **iterator + iterator**, **iterator + array**, **array + iterator**, and **array + array**. The mixed forms let an XML/decs iterator meet a bare in-memory array without materializing (e.g. `from_xml_node(...) |> _join(dealersArray, ...)` or `eachIter |> union(otherArray)`).

**Do not append `_to_array` to a both-array call.** The array+array base overload already returns `array<T>` (only the iterator-carrying shapes are lazy), so the `*_to_array` twins carry just the three iterator shapes — `union_to_array(a, b)` / `join_to_array(a, b, ...)` on two arrays is `error[30341] no matching functions or generics`. Call the bare op: `union(a, b)`, `join(a, b, keya, keyb, result)`. Wrap a side in `unsafe(each(arr))` only when you actually want the lazy iterator result.

`zip` accepts a mixed source in its **2-source** forms too — `zip(xmlIter, arr)` / `zip(arr, xmlIter)`, with or without a result selector (`zip_to_array` likewise). `zip` is N-ary (up to 8 sources), and full mixed coverage across all arities is combinatorial; for **3-or-more-source** `zip`, mix the operands with `each(arr)` rather than a per-combination overload.

```das
require daslib/linq_boost
```

This re-exports `daslib/linq` publicly. Don't `require` both.

### `_fold` — the default terminator for any chain

Fold every LINQ chain. It rewrites the chain so it stays in **array form** end-to-end: each step becomes a call against an intermediate `array<T>`, picking `*_inplace` ops where the source is a mutable local and `*_to_array` for the materialize step. No lazy iterators, just array-against-array passes that the CPU and AOT codegen are good at — cheaper than the lazy form on essentially every input, with the largest win when the chain already starts from an `array<T>` (the common case) and the first step turns into an inplace mutation.

**`_fold` goes last, as a trailing call on the chain** — `chain._fold()` (or `chain |> _fold()`):

```das
let names <- arr._where(_.flag)._select(_.name)._fold()
let xs    <- arr |> _select(_ * 2) |> to_array() |> _fold()
```

The wrapping form `_fold(chain)` works only when `chain` ends in a terminator (`to_array` / `count` / `to_table` / …). A wrapping `_fold` over a chain that ends in a bare `_select` currently fails to infer the selector — `_fold(arr |> _where(_.flag) |> _select(_.name))` reports `error[30341] no matching functions or generics: result_selector(...)`, with follow-on errors pointing into `daslib/linq_fold_common.das`. Trailing position has no such restriction, so write it that way and the question never comes up.

Skip `_fold` only when you genuinely need a lazy `iterator<T>` out (because the chain is being composed into a larger pipeline that consumes one element at a time).

The shorthand operators (`_<op>`) come from `linq_boost`: they expand `_<op>(iter, expr)` to `<op>(iter, $(_) => expr)` with `_` as the implicit element name. Available: `_where`, `_having`, `_select`, `_select_many`, `_select_many_2`, `_min_by`, `_max_by`, `_min_max_by`, `_min_max_average_by`, `_skip_while`, `_take_while`, `_all`, `_any`, `_none`, `_count`, `_long_count`, `_in`, `_not_in`, `_unique_by`, `_distinct_by`, `_order_by`, `_order_by_descending`, `_order_by_keys`, `_group_by`, `_group_by_lazy`, `_sequence_equal_by`, `_except_by`, `_intersect_by`, `_union_by`, and the join family `_join`, `_group_join`, `_left_join`, `_right_join`, `_full_outer_join`, `_cross_join`. Most also have a `_<op>_to_array` twin that materializes in one step (`_where_to_array`, `_select_to_array`, `_order_by_to_array`, `_group_by_to_array`, …).

### How to actually write a chain — the working shape

Use **dot form for the chain steps**, end with a trailing `._fold()`, and pipe-into any non-linq consumer (like string `join`):

```das
// For an array<T> source — call _select / _where directly; the result is already array<T>:
let s = (arr._select("{_:d}")._fold()) |> join(", ")
// or with a filter:
let names = (arr._where(_.flag)._select(_.name)._fold()) |> join(", ")

// For an iterator source (range(N), or anything returning iterator<T>) —
// prepend each() so dot-form works:
let dims = (each(range(N))._select("{_:d}").to_array()._fold()) |> join(", ")

// For a C++-bound dasvector / vector field — also each(...):
let types = (each(typeDecl.argTypes)._select(describeCppTypeEx(_, ...)).to_array()._fold()) |> join(",")
```

There are several tripwires to know about — they're not arbitrary, they fall out of how the call macros and the dot-pseudo-pipe interact. Treat the dot-form rules below as load-bearing:

- **The `_select` / `_where` / ... shorthand needs an iterator or array input.** The `linq_boost` macros validate `arg0type.isIterator || arg0type.isGoodArrayType` on the receiver, so `arr._select(...)` works because `arr : array<T>` is an accepted input. `range(N)._select(...)` does NOT — `range(N)` produces a `range` value (a separate kind of iterable), not an iterator or array, and the typer fails with `error[50503] expecting iterator or array` (plus a follow-on `error[30838] can't locate variable '_'`). Convert with `each(range(N))._select(...)` so `each()` produces `iterator<int>`. Same for any C++-bound `dasvector` / sequence-like field when you want dot-form chaining — start with `each(...)`.
- **Pipe form composes with `_fold` fine.** `arr |> _select(...) |> to_array() |> _fold()` works. What matters is `_fold`'s **position**, not pipe vs dot — see the `_fold` section above. Dot form still reads better for a multi-step chain; switch to pipe at the boundary into the final consumer (e.g. `(chain) |> join(",")`).
- **`each(...)` is `[unsafe_outside_of_for]` for arrays too**, not just for `each_kv` / `keys`. Inside a `_fold` chain the macro peels the `each` before infer, so no wrap is needed there; feeding `each(arr)` straight to a plain linq function needs `unsafe(each(arr))` or you get `error[31013] ... is unsafe, when not source of the for-loop`.
- **Multi-line chains need surrounding parens.** Statement-level newlines are significant outside brackets — `let x = a._foo()._bar()` on one line is fine, but breaking across lines requires `let x = (a._foo()._bar())`. The trailing `|> join(...)` can go on the same closing-paren line.
- **`_fold`'s output type follows the source, not the spelling.** An `array<T>` source stays `array<T>` either way — `arr._select(...)._fold()` is already `array<T>`, because linq's array overloads return arrays. An **iterator** source (`each(...)`, `range(...)`) folds to `iterator<T>`, so add `.to_array()` before the `._fold()` when you need a materialized array out of one.
- **Inside `_<op>` bodies, `_` is the element.** `_select("{unsafe(info.dim[_]):d}")` substitutes `_` correctly inside string interpolation. Multi-statement bodies via `build_string() $(w) { ... use _ ... }` work too — the macro substitutes `_` throughout the literal AST you pass.
- **String `join(arr, sep)` lives in `strings` / `strings_boost`** — `linq` itself has a different `join` (SQL-style two-iterator inner join with key + result projection). They coexist; the typer picks the right one by argument types. If you see "module strings_boost is not visible" and "missing argument blk" pointed at your join call, you're missing `require daslib/strings_boost` (or `require strings`).
- **The `_` placeholder is local to the closest enclosing `_<op>(...)`.** If you nest, give inner closures explicit names (`@@(x) => ...`). Don't try to shadow `_` between outer and inner shorthand calls.

### Table sources and the `to_table` sink

A `table<K;V>` (or `table<K>` set) is a first-class chain source — no key/value arrays needed:

```das
// each_kv yields (key, value) named tuples; keys/values give one lane.
// Wrap the head in unsafe(...) — the sources are [unsafe_outside_of_for].
let pricey = _fold(unsafe(each_kv(cars)) |> _where(_.value.price > 500) |> count())
let ids <- _fold(unsafe(keys(cars)) |> _where(_ > 100) |> to_array())

// to_table() lands a chain in a table: kv (or any (k => v) tuple) chain → table<K;V>,
// scalar chain → table<K> set. Duplicate keys keep the last occurrence.
var byId <- _fold(each(orders) |> _select(_.id => _.total) |> to_table())
var index <- _fold(unsafe(each_kv(cars)) |> _where(_.value.in_stock) |> to_table())
```

The fused emitter walks only the iterators the chain touches (a `.value`-only chain never
touches keys), folds `where(kv.key == X) … first/any/count` to an O(1) probe, joins on a bare
table key by probing the table instead of hashing, and inserts straight into the `to_table`
result with no intermediate array. `%linq!` queries dispatch table sources automatically
(`from kv in tab`). Slot order is unspecified — don't write order-sensitive expectations over
table chains. The 3-arg `to_table(it, keyBlock, elementSelectorBlock)` ToDictionary form also
exists (tier-2 only). Full pattern reference: `skills/linq_fold_patterns.md`.

## Don't mix styles

Pick **one** style per transformation and stay in it:

- A comprehension is a complete transformation. Don't drop `_select` / `_where` / `_fold` calls onto its result, and don't feed it into another sequence operator like `join` (linq's SQL-style join), `aggregate`, `concat`, etc. — those are linq primitives, and the moment you reach for one, the upstream chain should be linq too.
- A linq chain is a complete transformation. Don't drop a `[for (x in iter); ...]` comprehension into the middle — the surrounding `_select` / `_where` / `_to_array` / `_fold` cover everything a comprehension can express, and using both in one expression makes the optimizer's job harder while making the code harder to read.

```das
// Bad — comprehension feeding a linq sequence operator (join is high-order):
let pairs <- join(
    [for (x in customers); x],
    orders,
    @@(c : Cust) => c.id,
    @@(o : Ord) => o.customer_id,
    @@(c : Cust; o : Ord) => Pair(c = c, o = o))

// Good — pure linq (both sources are arrays, so plain `join` already returns array<Pair>):
let pairs <- customers |> join(orders,
    @@(c : Cust) => c.id,
    @@(o : Ord) => o.customer_id,
    @@(c : Cust; o : Ord) => Pair(c = c, o = o))

// Bad — linq chain handing off to a comprehension:
let names <- [for (x in arr |> _where(_.flag)); x.name]

// Good — pure linq, `_fold` trailing:
let names <- arr._where(_.flag)._select(_.name)._fold()
```

Two things the join example is load-bearing about. The key/result selectors need **typed** parameters — bare `@@(c) => c.id` gives `error[30341]` plus `error[30928] can't get field 'id' of auto const&`. And struct construction takes **named** arguments — `Pair(c, o)` is `error[30160] can only generate default structure constructor without arguments`.

The rule applies to any sequence-consuming high-order primitive, including string `join`. If a single comprehension suffices and is fed only to free functions like `length` or basic terminus calls, that's still one style. The split happens when an operator that itself walks a sequence (linq `join` / `concat` / `zip` / `aggregate` / string `join(arr, sep)`) shows up — at that point the upstream should be linq, not a comprehension.

## When to use a plain `for` loop

Despite all the above, sometimes the right answer is `for`:

```das
for (argT in typeDecl.argTypes) {
    write(writer, ",{describe(argT)}>")
}
```

Use a `for` loop when:
- The body has a side effect: writing to a `StringBuilderWriter`, mutating an external map, calling a function with a `var` ref-out parameter.
- You need `break` / `continue` / multi-line control flow.
- You're driving an iteration purely for its side effect with no result value.

Do NOT shoehorn side effects into `_select` lambdas — they're meant to be pure. Mixing `each() + map() + to_array()` to drive side effects has historically produced iterator-fusion use-after-free bugs.

## Anti-patterns

**Don't:**

```das
// daslib/functional 'map' / 'each' / 'to_array' chain — old style:
let xs <- to_array(map(each(arr), @(x) { return f(x); }))

// Functional + iterator surgery in one expression:
let args <- to_array(map(each(typeDecl.argTypes).reverse(), @(argT) { return ",{describe(argT)}>"; }))
reverse(args)
write(writer, "{join(args, "")}")
```

**Do:**

```das
// Comprehension — the transformation fits in one expression:
let xs <- [for (x in arr); f(x)]

// LINQ — chain of operators in dot form, ending in a trailing ._fold():
let xs <- arr._where(_.flag)._select(_.name)._fold()

// LINQ piping into a non-linq consumer (string join here):
let csv = (arr._select(_.name)._fold()) |> join(", ")

// Plain for-loop — the body has a side effect:
for (argT in typeDecl.argTypes) {
    write(writer, ",{describe(argT)}>")
}
```

## Quick decision flow

1. Result is a value derived from one source by one expression (with optional filter), and the result is consumed plainly (no further sequence operator)? → **comprehension**.
2. Body has side effects or needs `break`? → **plain `for`**.
3. Chain of operators (multi-where, sort, group, set-op, aggregation), OR a transformation feeding a sequence-consuming primitive like `join` / `concat` / `aggregate` / string `join(arr, sep)`? → **`arr._<op>(...)._<op>(...)._fold()`** with the `_<op>` shorthand and `_fold` trailing. Drop `_fold` only if the result must be a lazy `iterator<T>` for downstream composition.
4. Tempted to import `daslib/functional`? → almost always one of (1), (2), (3) is the right answer.
