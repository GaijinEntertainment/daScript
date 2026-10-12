# daslib architecture notes - the linq family

Companion to `ARCHITECTURE.md` in this folder. This document covers the in-memory family a
user writes against: `linq` (the overload surface), `linq_boost` (the `_`-shorthand call
macros) and `linq_das` (the `%linq!` query reader). The `_fold` splice that fuses a chain is
`ARCHITECTURE_LINQ_FOLD.md`; the SQL lowering is `ARCHITECTURE_SQL_LINQ.md`.

## linq

### top_n with a spliced comparator {#linq-top-n-with-cmp}

- **`top_n_by_with_cmp` exists for the fold splice: no daslib function calls it, only code
  that `linq_fold_common` emits.** When an order-by key body is pure and inlineable,
  `linq_fold_common` emits it with that body spliced into BOTH sides of the comparator -
  removing the per-comparison comparator dispatch and the per-side `key(v)` dispatch the
  key-lambda overloads pay. It is not a redundant twin of `top_n_by`.

## linq_boost

### Call-macro argument forms {#linq-boost-argument-forms}

- **A lambda-taking call macro accepts a bare expression or a ready one-parameter block.** A
  bare expression over `_` (`_where(src, _.x > 1)`) is wrapped into `$(_ : T) => expr`, where
  `T` is the source element type made const and non-ref. A block (`$(c) => c.x > 1`) is passed
  through unwrapped: its parameter gets `T` only when it is untyped, and its name stays as
  written. linq_das emits the block form, spelling the query's range variable as the
  parameter, so the range variable reaches the generated call verbatim. `_order_by_keys`
  accepts the same two forms for its key-tuple argument, and the two-source `_*_by` macros
  accept them for their third argument.

### Bucket-lambda stamping {#linq-boost-bucket-lambda-stamping}

- **`BucketLambdaStamper` exists because a bucket-surface lambda cannot infer on its own.**
  On the `group_by_lazy` element shape `tuple<K; array<E>>`, an untyped lambda in
  `<bind>._1 |> select/sum/...(<lambda>)` has nothing to bind against in the fully generic
  tier-2 parameters (error 30303); the chain type knows `E`, so the macro stamps the
  lambda's parameter before the rewrite - the same move as the outer-parameter injection in
  `visit()`.

### Join result-lambda auto-typing {#linq-boost-result-lambda-typing}

- **An untyped two-parameter result lambda takes its parameter types from the join
  sources.** `on` already accepts an untyped `(l, r)`, and the result lambda matches it:
  neither linq_das, which emits `$(c, d) => ...` for every join it lowers, nor a user spells
  an element type - for a `from_decs_template(type<T>)` source that type is an unnamed tuple.
  The stamp fires only when the lambda has exactly two parameters and both are untyped; a
  lambda with either one typed is left as written.
- **`result_param1_type` is the join result-lambda auto-typing hook.** param0 is always TA;
  param1 is whatever the override returns - `_join` -> TB, `_group_join` -> a bare `array<TB>`
  with const/ref cleared to match the bucket type. `_left_join` / `_right_join` /
  `_full_outer_join` deliberately do not override: their result parameter is `$Option<T>` on
  one or both sides, so those result lambdas stay user-spelled.
- **`_cross_join` stamps both parameters itself, TA and TB, with no hook** - neither side of a
  cross join is ever an `$Option<T>`. linq_das lowers an uncorrelated multiple-`from` to
  `_cross_join` with an untyped result lambda, so that lowering depends on this stamp.

### Two-stage _select_many {#linq-boost-select-many-stages}

- **Stage 1 hands off to `_select_many_2` through `make_call`, not `qmacro`.** `make_call`
  returns an `ExprCallMacro` when a call macro of that name is registered, so the next infer
  pass, by which time the collection selector's body has inferred, dispatches stage 2 as a
  macro. A `qmacro`-built `_select_many_2(...)` is a plain `ExprCall`, which infer resolves as
  a call to a function that does not exist.

## linq_das

### The reader pipeline {#linq-das-reader-pipeline}

- **`%linq! ... %%` is a reader macro.** `LinqDasReader.suffix` passes the query text to
  `transpile_query`, and the daslang source it returns replaces the macro, starting on the
  macro's line. Comments inside the query are blanked to spaces and their newlines are kept,
  so the spliced text keeps the query's lines and columns. A malformed query reports
  `macro_error` and splices the literal `(0)`. The statement around the macro still parses,
  and the macro error is the only diagnostic.

### The substituting scanners' token model {#linq-das-token-model}

- **The substituting scanners share one token model.** `substitute_idents`,
  `mentions_ident` and `rewrite_group_var` all read it: plain `"..."` content is verbatim, a
  `{...}` interpolation body is code that is scanned and substituted, one level of nested
  string literal inside an interpolation is verbatim again, and identifier position
  classifies the same way everywhere - after-`.` field access and before-single-`=` labels
  are not references, with `is_label_position` the shared judge.

### let inlining {#linq-das-let-inlining}

- **`let n = <expr>` is inlined as text before anything else is parsed**: each later
  reference to `n` becomes `(<expr>)`. The rest of the reader therefore sees a query with no
  `let` in every form (single source, `join`, second `from`), and an inlined computed column
  or key pushes down to SQL like one written out by hand. Chained lets resolve in order,
  because each right-hand side is inlined with the earlier bindings. Scope is forward-only, as
  in C#: the source expression and the text before a binding stay as written, so an outer
  name that a later binding shadows keeps its meaning up to the binding. A `let` may not
  reuse either range variable's name. After the second variable's clause it would shadow that
  variable, and before the clause the substitution would rewrite the variable's declaration.

### Query stages {#linq-das-query-stages}

- **A single-source query is a chain of stages on one `_fold` chain.** A stage is
  `[where]* [orderby] (select <proj> | group <var> by <key>) [iterator]`. An `into <var>`
  after the terminal starts the next stage, with `<var>` bound to this stage's output (C#
  query continuation). Every stage appends to the same `_fold` chain, with no array between
  stages, so fusion and SQL's single-statement pushdown hold across `into`. When a stage's
  range variable came from `group ... into g`, the stage's clause text goes through
  `rewrite_group_var`.
- **A stage's `where`s all emit ahead of its order op, in source order.** As in C#, there may
  be several of them anywhere before the terminal. A `where` written after `orderby` filters
  the sorted sequence, and when the keys give a total order that is the same result as
  filtering first.

### orderby lowering {#linq-das-orderby}

- **`orderby` lowers to one of two operators.** The clause body `k1 [ascending|descending],
  k2 ...` is split on commas at bracket depth zero, into at most `ORDERBY_MAX_KEYS` keys. One
  key emits `_order_by` or `_order_by_descending`. Several keys emit `_order_by_keys` over a
  key tuple and a `uint` mask in which bit i marks key i as descending, with the least
  significant bit for the first key. This is the encoding that `less_masked` (`linq.das`)
  reads. `ORDERBY_MAX_KEYS` equals the largest key-tuple arity among the `less_masked`
  overloads, and the two change together: no test runs a query at the cap.

### Two-source forms {#linq-das-two-source}

- **`join` and a second `from` share one emit with two shapes, picked from the query text
  alone because the reader runs before type inference.** *Select-terminal* (no `where` or
  `orderby` after the second source, and a `select` terminal): the projection is the
  operator's result lambda, spliced verbatim with both range variables in scope. This shape
  pushes down to SQL. *Transparent identifier* (a `where` or `orderby` after the second
  source, or a `group` terminal): the result lambda builds the pair `(c = c, d = d)`, and
  every later clause is rewritten to read `linq_join_ti.c` / `linq_join_ti.d` from the one
  carried parameter `JOIN_TI`. The pairs are filtered, sorted and grouped in the same `_fold`
  chain, with no array in between, and a `group` terminal yields `(key, array<pair>)` per
  bucket. This shape runs in memory only, because over SQL the carried whole-row tuple fails
  `_sql`'s column-reference check.
- **`JOIN_TI` is a spellable identifier on purpose.** `__`-prefixed names are reserved by
  the compiler, so the transparent-identifier carrier cannot be hidden outside the user's
  namespace and must stay a single plain token - hence the explicit rejection of a range
  variable named `linq_join_ti` instead of an unspellable name.
- **A `where` is placed by its position.** Before the second source it filters the left
  source. It reads only `c`, needs no rewrite, and pushes down to SQL, with several such
  `where`s joined by AND. After the second source it filters the carried pair. Between the
  start of the second clause and its tail (inside `join ... equals`, or between the second
  range variable and `in`) it is a grammar error. Each `where` runs to the next clause
  keyword.
- **A select-terminal lambda underscores any range variable its projection does not use.**
  `mentions_ident` checks the projection, and an unused parameter is emitted as `_c` / `_d`.
  The generated lambda then raises no LINT013 (unused variable) in the user's code. For
  example, `select c.name` over a cross product leaves `d` unused.
- **A second `from` is correlated when its source text mentions the first range variable.**
  An uncorrelated one is the cross product `_cross_join`, which pushes down to a SQL CROSS
  JOIN. A correlated one (`from d in c.items`) is the flatten `_select_many` and runs in
  memory only, because its second source is a per-row collection, not a table. The outer row
  holds the collection being flattened, so the row is non-copyable and the transparent
  identifier cannot carry it. A correlated query is therefore always select-terminal:
  `orderby` and `group` are rejected, and a `where` after the second `from` may read only
  `d`. That `where` is pushed into the collection selector ahead of the flatten, which yields
  the same rows as filtering the pairs.
- **The correlated-flatten collection selector borrows, it never copies.** `unsafe(each(tail))`
  is chosen over `to_sequence` (whose const-array overload clones per row) and over
  `where_`'s array overload (which materializes per row). Borrowing a TEMPORARY tail
  (`from x in [c.id]`, a function result) is safe because daslang heap-allocates arrays and
  does not finalize them at scope exit - the backing storage outlives the borrow and is
  reclaimed by GC. Each pushed-in `where` chains onto the borrowed iterator as `_where`, whose
  iterator overload stays lazy. A typed `from d : RowB in c.items` builds
  `from_in(<tail>, type<RowB>)` in place of the `each` borrow.
- **`join ... into g` is a group-join.** It emits `_group_join`, whose result lambda takes
  `g`, the ARRAY of matches, as its second parameter, and its key lambda is the `_join` one.
  It is select-terminal only, because the `(c, g)` pair cannot be carried while `g` is a
  non-copyable array. It runs in memory only, because `_sql` rejects `_group_join`.
- **`build_src`'s two arms are a fusion decision, not a formatting one.** A fused source -
  the chain head, both join sources, the second `from`'s source - is built as `from_in(...)`
  so `_fold` can route it. The one bare arm is the left source of a two-`from` query, correlated
  or not: `_cross_join` and `_select_many` both run at tier-3 passthrough, so that source is
  passed as the bare parenthesized expression, where `each`'s `[unsafe_outside_of_for]` does not
  trip and the array overloads are selected. `transpile_cross` builds it before correlation is
  known, which is decided inside `emit_two_source`.

### from_in dispatch {#linq-das-from-in}

- **`from_in` identifies typed sources by matching module + type NAME as strings**, not by
  requiring the provider modules: `sqlite_boost::SqlRunner` -> `select_from`, pugixml
  `xml_node` -> `from_xml_node`, `json::JsonValue?` -> `from_json` - linq_das stays
  dependency-free on sqlite/pugixml/json. Both node builders are `[unsafe_outside_of_for]`,
  so the emitted call carries the `unsafe` wrap a `_fold` chain requires. A table source is
  rejected with a fix message before those arms: it already carries its row shape, and the
  typed builders would all mis-fire.
- **`from_in` emits each builder as a bare `alwaysSafe` call.** In the qmacro,
  `unsafe($c(...)(...))` becomes the `alwaysSafe` flag on the call itself, not a wrapper
  node, so linq_fold's source extractors and `peel_each` see a bare `ExprCall`. An
  `[unsafe_outside_of_for]` head is therefore fine both fused and unfused. The untyped form
  adds no check of its own for a source that cannot be iterated: `each` reports that error.
- **A `FromInMacro` reject returns null so infer can stabilize.** Errors clear on every pass,
  so the not-yet-inferred-source arm's error survives only when the source never infers at
  all - that is what makes that arm a deferral rather than a final verdict.
