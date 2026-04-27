# LINQ and comprehensions in daslang

Read this skill before writing any `.das` code that transforms a sequence — filtering, mapping, sorting, grouping, aggregating, materializing into an array/table. The fixed preference order is:

1. **Comprehension** — if a single `[for (x in src); expr; where cond]` (or `{... => ...}` for tables) covers the whole transformation
2. **LINQ** — for everything else (multi-step chains, lazy iterators, set ops, joins, aggregations)
3. **Plain `for` loop** — when there is a side effect (writing to a builder, mutating shared state) or an early `break`

Do NOT use `daslib/functional` (`map`, `filter`, `each`, `to_array`) for new code. The linq operators (`select`, `where_`, `_select`, `_where`, `to_array`, …) are the supported chainable ones; `daslib/functional` is older, less integrated with the pipe-syntax macros, and patterns built on it have been the source of fusion-runtime bugs (see issue #2505).

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

LINQ is the chainable iterator surface in `daslib/linq` plus the pipe/dot-syntax shorthand macros in `daslib/linq_boost`. It covers everything comprehension can't:

- Multiple wheres, multiple selects, sorts, groupings, joins, set operations.
- Aggregations: `count`, `sum`, `min`, `max`, `min_max`, `min_max_average`, `aggregate`.
- Set ops: `distinct`, `distinct_by`, `union`, `union_by`, `except`, `except_by`, `intersect`, `intersect_by`, `unique`, `unique_by`.
- Element ops: `first`, `first_or_default`, `last`, `last_or_default`, `single`, `element_at`, `element_at_or_default`.
- Querying: `any`, `all`, `none`, `contains`, `sequence_equal`, `sequence_equal_by`.
- Partitioning: `skip`, `skip_while`, `take`, `take_while`, `skip_last`, `take_last`, `chunk`.
- Joins/groups: `join`, `group_join`, `group_by`, `group_by_lazy`.
- Transforms: `select`, `select_to_array`, `select_many`, `zip`.
- Materialize: `*_to_array` variants for any of the above.

```das
require daslib/linq_boost
```

This re-exports `daslib/linq` publicly. Don't `require` both.

### `_fold` — the default wrapper for any chain

Wrap every LINQ chain in `_fold(...)`. It rewrites the chain so it stays in **array form** end-to-end: each step becomes a call against an intermediate `array<T>`, picking `*_inplace` ops where the source is a mutable local and `*_to_array` for the materialize step. No lazy iterators, just array-against-array passes that the CPU and AOT codegen are good at — cheaper than the lazy form on essentially every input, with the largest win when the chain already starts from an `array<T>` (the common case) and the first step turns into an inplace mutation.

Skip `_fold` only when you genuinely need a lazy `iterator<T>` out (because the chain is being composed into a larger pipeline that consumes one element at a time).

The shorthand operators (`_<op>`) come from `linq_boost`: they expand `_<op>(iter, expr)` to `<op>(iter, $(_) => expr)` with `_` as the implicit element name. Available: `_where`, `_select`, `_min_by`, `_max_by`, `_min_max_by`, `_min_max_average_by`, `_skip_while`, `_take_while`, `_all`, `_any`, `_none`, `_count`, `_unique_by`, `_distinct_by`, `_order_by`, `_order_by_descending`, `_sequence_equal_by`, `_except_by`, `_intersect_by`, `_union_by`, `_group_by_lazy`, `_having`.

### How to actually write a chain — the working shape

Use **dot form for the chain steps**, end with `.to_array()._fold()`, and pipe-into any non-linq consumer (like string `join`):

```das
// For an array<T> source — call _select / _where directly:
let s = (arr._select("{_:d}")._fold()) |> join(", ")
// or with a filter, materializing to array:
let names = (arr._where(_.flag)._select(_.name).to_array()._fold()) |> join(", ")

// For an iterator source (range(N), or anything returning iterator<T>) —
// prepend each() so dot-form works:
let dims = (each(range(N))._select("{_:d}").to_array()._fold()) |> join(", ")

// For a C++-bound dasvector / vector field — also each(...):
let types = (each(typeDecl.argTypes)._select(describeCppTypeEx(_, ...)).to_array()._fold()) |> join(",")
```

There are several tripwires to know about — they're not arbitrary, they fall out of how the call macros and the dot-pseudo-pipe interact. Treat the dot-form rules below as load-bearing:

- **The `_select` / `_where` / ... shorthand needs an iterator or array input.** The `linq_boost` macros validate `arg0type.isIterator || arg0type.isGoodArrayType` on the receiver, so `arr._select(...)` works because `arr : array<T>` is an accepted input. `range(N)._select(...)` does NOT — `range(N)` produces a `range` value (a separate kind of iterable), not an iterator or array, and the typer fails with "type<auto> can't be fully inferred". Convert with `each(range(N))._select(...)` so `each()` produces `iterator<int>`. Same for any C++-bound `dasvector` / sequence-like field when you want dot-form chaining — start with `each(...)`.
- **Pipe form does not compose with `_fold`.** A chain like `arr |> _select(...) |> to_array() |> _fold()` fails with "expecting linq expression" — the `_fold` macro needs to see the chain in the dot/call shape, and pipe rewrites confuse its pattern match. Always use dot form for the chain and switch to pipe only at the boundary into the final consumer (e.g. `(chain) |> join(",")`).
- **Multi-line chains need surrounding parens.** Statement-level newlines are significant outside brackets — `let x = a._foo()._bar()` on one line is fine, but breaking across lines requires `let x = (a._foo()._bar())`. The trailing `|> join(...)` can go on the same closing-paren line.
- **`_fold` defaults to iterator output unless you ask for array.** End with `.to_array()._fold()` to get `array<T>` out. End with just `._fold()` (no preceding `.to_array()`) and you get `iterator<T>` — fine if the next step accepts iterators (string `join` does), but you give up the array-form optimization that's the whole point of `_fold`.
- **Inside `_<op>` bodies, `_` is the element.** `_select("{unsafe(info.dim[_]):d}")` substitutes `_` correctly inside string interpolation. Multi-statement bodies via `build_string() $(w) { ... use _ ... }` work too — the macro substitutes `_` throughout the literal AST you pass.
- **String `join(arr, sep)` lives in `strings` / `strings_boost`** — `linq` itself has a different `join` (SQL-style two-iterator inner join with key + result projection). They coexist; the typer picks the right one by argument types. If you see "module strings_boost is not visible" and "missing argument blk" pointed at your join call, you're missing `require daslib/strings_boost` (or `require strings`).
- **The `_` placeholder is local to the closest enclosing `_<op>(...)`.** If you nest, give inner closures explicit names (`@@(x) => ...`). Don't try to shadow `_` between outer and inner shorthand calls.

## Don't mix styles

Pick **one** style per transformation and stay in it:

- A comprehension is a complete transformation. Don't drop `_select` / `_where` / `_fold` calls onto its result, and don't feed it into another sequence operator like `join` (linq's SQL-style join), `aggregate`, `concat`, etc. — those are linq primitives, and the moment you reach for one, the upstream chain should be linq too.
- A linq chain is a complete transformation. Don't drop a `[for (x in iter); ...]` comprehension into the middle — the surrounding `_select` / `_where` / `_to_array` / `_fold` cover everything a comprehension can express, and using both in one expression makes the optimizer's job harder while making the code harder to read.

```das
// Bad — comprehension feeding a linq sequence operator (join is high-order):
let pairs <- join_to_array(
    [for (x in customers); x],
    orders,
    @@(c) => c.id,
    @@(o) => o.customer_id,
    @@(c, o) => Pair(c, o))

// Good — pure linq:
let pairs <- _fold(customers |> join_to_array(orders, @@(c) => c.id, @@(o) => o.customer_id, @@(c, o) => Pair(c, o)))

// Bad — linq chain handing off to a comprehension:
let names <- [for (x in arr |> _where(_.flag)); x.name]

// Good — pure linq:
let names <- _fold(arr |> _where(_.flag) |> _select(_.name))
```

The rule applies to any sequence-consuming high-order primitive, including string `join`. If a single comprehension suffices and is fed only to free functions like `length` or basic terminus calls, that's still one style. The split happens when an operator that itself walks a sequence (linq `join` / `concat` / `zip` / `aggregate` / string `join(arr, sep)`) shows up — at that point the upstream should be linq, not a comprehension.

## When to use a plain `for` loop

Despite all the above, sometimes the right answer is `for`:

```das
for (itd in typeDecl.dim) {
    write(writer, ",{itd}>")
}
```

Use a `for` loop when:
- The body has a side effect: writing to a `StringBuilderWriter`, mutating an external map, calling a function with a `var` ref-out parameter.
- You need `break` / `continue` / multi-line control flow.
- You're driving an iteration purely for its side effect with no result value.

Do NOT shoehorn side effects into `_select` lambdas — they're meant to be pure. Mixing `each() + map() + to_array()` to drive side effects is what produced the iterator-fusion UAF documented in issue #2505.

## Anti-patterns

**Don't:**

```das
// daslib/functional 'map' / 'each' / 'to_array' chain — old style:
let xs <- to_array(map(each(arr), @(x) { return f(x); }))

// Functional + iterator surgery in one expression:
let args <- to_array(map(each(typeDecl.dim).reverse(), @(itd) { return ",{itd}>"; }))
reverse(args)
write(writer, "{join(args, "")}")
```

**Do:**

```das
// Comprehension — the transformation fits in one expression:
let xs <- [for (x in arr); f(x)]

// LINQ — chain of operators in dot form, ending in .to_array()._fold():
let xs <- arr._where(_.flag)._select(_.name).to_array()._fold()

// LINQ piping into a non-linq consumer (string join here):
let csv = (arr._select(_.name).to_array()._fold()) |> join(", ")

// Plain for-loop — the body has a side effect:
for (itd in typeDecl.dim) {
    write(writer, ",{itd}>")
}
```

## Quick decision flow

1. Result is a value derived from one source by one expression (with optional filter), and the result is consumed plainly (no further sequence operator)? → **comprehension**.
2. Body has side effects or needs `break`? → **plain `for`**.
3. Chain of operators (multi-where, sort, group, set-op, aggregation), OR a transformation feeding a sequence-consuming primitive like `join` / `concat` / `aggregate` / string `join(arr, sep)`? → **`_fold(arr |> _<op>(...) |> ...)`** with the `_<op>` shorthand. Drop `_fold` only if the result must be a lazy `iterator<T>` for downstream composition.
4. Tempted to import `daslib/functional`? → almost always one of (1), (2), (3) is the right answer.
