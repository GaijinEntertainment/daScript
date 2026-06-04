.. _linq_das:

============================================================
C#-style LINQ query syntax (``%linq!``)
============================================================

``daslib/linq_das`` adds a C#-like query-expression form through the
``%linq! … %%`` inline reader macro. A query is a purely mechanical,
compile-time rewrite into a ``_fold(...)`` chain, so it rides the same fused
execution engine as the pipe-form LINQ operators (see
:ref:`linq_fold_patterns`) with no runtime cost over the hand-written chain.

.. code-block:: das

    require daslib/linq_das

    var names <- %linq! from c in cars where c.price > 100 select c.name %%

rewrites to, and is re-parsed in place as:

.. code-block:: das

    var names <- ( _fold( each(cars) |> _where($(c) => c.price > 100) |> _select($(c) => c.name) |> to_array() ) )

The macro lives in the lexer's inline reader-macro slot (``%name!``), so a
query is an ordinary expression — it can be assigned, passed as an argument, or
embedded in a larger expression.

Clauses
-------

A query is ``from <var> [ : <Row> ] in <src> ( where <pred> )* [ ( join <var2>
[ : <Row2> ] in <src2> on <keyA> equals <keyB> [ into <g> ] | from <var2> [ : <Row2> ] in
<src2> ) ] ( where <pred> )* [ orderby <expr> [ascending|descending] (, <expr> [ascending|descending])* ] ( select <proj> |
group <var> by <key> ) [ into <var> <continuation> ] [ iterator ]`` — a second
range variable comes from **either** a ``join`` **or** a second ``from`` (never
both); each ``where`` slot accepts any number of predicates (each AND-folds in
source order). ``into`` has two forms: ``join … equals … into <g>`` is
a **group join** (``g`` = the array of matching right rows, in scope with the
left variable — see :ref:`linq_das_join`), while a trailing ``into <var>`` after
the terminal is a **query continuation** that rebinds the stage's output and
continues from there (single-source only — see :ref:`linq_das_into`).
Separately, a ``let <name> = <expr>`` binding may appear **any number of times
between body clauses** — it is inlined away before the rest is parsed (see
:ref:`linq_das_let`):

- ``from <var> in <source>`` — the element bind ``<var>`` names the per-row
  value. With no type annotation, ``<source>`` is an ``array<T>``.
- ``let <name> = <expr>`` — optional, repeatable, and free to appear between any
  body clauses; binds a computed value reused in the clauses that follow it (see
  :ref:`linq_das_let`).
- ``where <predicate>`` — optional and **repeatable**. A ``where`` **before** the
  ``join`` / second ``from`` filters the left source (single range var); a
  ``where`` **after** it sees both range variables. Several ``where`` clauses may
  appear in either slot — each emits its own filter, AND-folded in source order
  (over a SQL source they push down as one ANDed ``WHERE``). A ``where`` written
  after the ``orderby`` filters the sorted sequence — identical, for a total
  order, to filtering first — so it emits ahead of the sort.
- ``join <var2> in <src2> on <keyA> equals <keyB>`` — optional, a single inner
  equi-join introducing a second range variable (see :ref:`linq_das_join`).
- ``from <var2> in <src2>`` — optional, a second ``from`` introducing a second
  range variable — SelectMany: an **independent** source is the cross product, a
  source that is a field of the first range variable (``from l in o.lines``) is
  the correlated flatten (see :ref:`linq_das_multifrom`).
- ``orderby <expr> [descending], …`` — optional; one or more comma-separated sort
  keys, each with its own direction (see :ref:`linq_das_ordering`). Omitted when absent.
- ``select <projection>`` — ``select <var>`` (the identity projection) returns
  the rows unchanged; any other projection emits ``_select(...)``.
- ``group <var> by <key>`` — the alternative terminal to ``select`` (see
  :ref:`linq_das_grouping`).
- ``into <var>`` — optional query continuation after the terminal: rebinds the
  prior stage's output to ``<var>`` and continues with more clauses, all on the
  same fused chain (see :ref:`linq_das_into`).

Each stage ends with **either** ``select`` **or** ``group … by`` — exactly one.
Clauses may span multiple lines inside the ``%linq! … %%`` body.

Sources
-------

An **untyped** ``from c in <arr>`` is an array source. A **typed** range
variable ``from c : Row in <src>`` selects a non-array source — the row type
``Row`` is supplied on the range variable (C#-faithful ``from Type c in src``)
because the source value alone does not carry it:

.. code-block:: das

    // array (untyped) — `each(arr)`
    var a <- %linq! from c in cars where c.price > 100 select c.name %%

    // decs — the `decs` keyword marker → `from_decs_template(type<CarComp>)`
    var d <- %linq! from c : CarComp in decs where c.price > 100 select c.name %%

    // SQL — a SqlRunner value → `select_from`, pushed down by `_sql`
    var s <- %linq! from c : Car in db where c.price > 100 select c.name %%

    // XML — an xml_node value → `from_xml_node`, fused by the XmlAdapter
    var x <- %linq! from c : Car in doc.document_element where c.price > 100.0 select c.brand %%

    // JSON — a JsonValue? array → `from_json`, fused by the JsonAdapter
    var j <- %linq! from c : Car in carsJson where c.price > 100 select c.name %%

For value sources (SQL, XML, JSON) the reader emits
``from_in(<src>, type<Row>)``; the ``from_in`` call macro dispatches on the
source value's type to the concrete builder (so a new backend is a new
``from_in`` branch, never a parser change). ``decs`` has no source value, so it
is emitted directly as ``from_decs_template`` and never goes through
``from_in``. The row type's required annotation depends on the source —
``[decs_template]`` for decs, ``[sql_table]`` / ``[sql_view]`` for SQL, a plain
struct for XML and JSON. The JSON source is a ``JsonValue?`` holding a JSON
**array** of objects (``from c : Car in jv["cars"]`` descends into a nested
array first); like the XML source, each element materializes **by name** —
top-level fields read by key, field-pruned to just the keys the chain reads. A
custom whole-row ``from_JV(Row)`` override is **not** honored (this is a flat
query source, not a deserializer); to query through ``from_JV`` instead,
materialize the array first
(``[for (e in jv.value as _array); from_JV(e, type<Row>)]``) and query that.

Range variable
--------------

The range variable is spliced **verbatim** as the lambda parameter — the
predicate becomes ``_where($(c) => …)`` and the projection ``_select($(c) =>
…)``, keeping the range-variable name; the predicate and projection text is passed
through unchanged. Any identifier name works.

The ``_fold`` operator DSL accepts a named-variable ``$(x) => …`` block
directly. For the SQL source, ``_sql`` resolves a single source against the
placeholder ``_``; the macro normalizes the single-source lambda parameter to
``_`` internally, so the range-variable name is still spliced verbatim at the
surface.

.. _linq_das_filtering:

Filtering (``where``)
---------------------

A ``where`` clause is optional and **repeatable** (as in C#) — each emits its
own ``_where`` filter, AND-folded in source order:

.. code-block:: das

    // two predicates — both apply
    var names <- %linq! from c in cars where c.price > 100 where c.brand == "eco" select c.name %%
    // expands to: _fold( each(cars) |> _where($(c) => c.price > 100) |> _where($(c) => c.brand == "eco") |> _select($(c) => c.name) |> to_array() )

Over a **SQL** source the predicates push down as one ANDed ``WHERE`` (a single
statement, no intermediate materialize). On a two-source query (``join`` / second
``from``) the slot still applies: ``where``\ s **before** the second source filter
the left source (and push to SQL), ``where``\ s **after** it filter the carried
pair. A ``where`` written **after** the ``orderby`` filters the sorted sequence —
for a total order that is identical to filtering first, so it emits ahead of the
sort.

.. _linq_das_let:

Let bindings
------------

``let <name> = <expr>`` introduces a computed value (what C# calls a new range variable)
that is reused in the clauses that follow it:

.. code-block:: das

    var rows <- %linq! from c in cars
        let net = c.price - tax(c)
        where net < 100
        orderby net
        select (Name = c.name, Net = net) %%

The binding is **inlined textually**: every later reference to ``net`` is
replaced with ``(c.price - tax(c))``, so the query is exactly equivalent to
writing the expression out at each use site. Bindings are repeatable and chain —
a later ``let`` may reference an earlier one — and they may appear in any body
context (a single ``from``, after a ``join`` referencing both range variables,
or in a ``from … from`` SelectMany):

.. code-block:: das

    // chained — `net` uses the earlier `disc`
    var rows <- %linq! from c in cars
        let disc = c.price / 10
        let net  = c.price - disc
        orderby net descending select (N = c.name, Net = net) %%

Because the binding is inlined, a ``let`` over a **SQL** source pushes its
computed expression down: a binding used in ``where`` / ``orderby`` / ``select``
becomes the computed predicate / key / column ``_sql`` renders directly (the
whole query stays a single ``SELECT``). The binding name must differ from every
range variable (including a second one from a ``join`` / second ``from``) and
from any earlier binding, a ``let`` must precede the ``select`` / ``group``
terminal, and an inlined expression is re-evaluated at each use site (a textual
inline, not a cached temporary).

.. _linq_das_projections:

Projections
-----------

The ``select`` clause projects each (joined) row into the result element. Four
projection forms are supported:

- **Scalar** — a single field or expression: ``select c.name`` →
  ``array<string>``; ``select c.price * 2`` → ``array<int>``.
- **Named tuple** — the C# ``select new { … }`` analog, and the way to return
  more than one column: ``select (Name = c.name, City = c.city)`` → an array of
  ``tuple<Name:string; City:string>``; read fields by name (``row.Name``).
- **Whole-row / identity** — ``select c`` returns the rows unchanged
  (``array<Row>``); it emits no ``_select`` stage. Over a join it yields the left
  row. Over a **SQL** source whole-row ``select c`` is **in-memory only** — a row
  has no column form, so project columns (scalar or named tuple) to push down.
- **String interpolation** — ``select "{c.name}:{b.country}"`` →
  ``array<string>``. The range variables are rewritten **inside** the ``{ … }``
  interpolation (so both ``c`` and ``b`` resolve), letting you format a row to a
  string in one step.

.. code-block:: das

    // scalar
    var names  <- %linq! from c in cars select c.name %%
    // named tuple — two columns, read row.Name / row.Price
    var rows   <- %linq! from c in cars select (Name = c.name, Price = c.price) %%
    // whole row (identity) — the filtered structs, unchanged
    var kept   <- %linq! from c in cars where c.price > 100 select c %%
    // string interpolation — range vars rewritten inside {…}
    var labels <- %linq! from c in cars select "{c.name} @ {c.price}" %%

.. _linq_das_ordering:

Ordering
--------

``orderby <expr> [descending|ascending], …`` sorts by one **or more** keys, each
with its own direction. A single key emits ``_order_by($(c) => <expr>)`` (or
``_order_by_descending(...)``); **multiple** comma-separated keys emit one
``_order_by_keys($(c) => (k1, k2, …), <descMask>)`` — a single composite **stable**
sort, where ``descMask`` bit *i* (LSB = first key) marks key *i* descending.
``descending`` (and the default-explicit ``ascending``) are recognized as trailing
keywords per key:

.. code-block:: das

    // single key, ascending (default)
    var byPrice <- %linq! from c in cars orderby c.price select c.name %%

    // single key, descending, after a where
    var top <- %linq! from c in cars where c.price > 100 orderby c.price descending select c.name %%

    // multi-key with mixed directions: brand ascending, then price descending
    var rows <- %linq! from c in cars orderby c.brand, c.price descending select c %%

Works over all five sources: SQL emits ``ORDER BY c1, c2 DESC, …``; array / decs / XML / JSON
sort the materialized rows. Multi-key ordering is **stable** (C# ``OrderBy`` / ``ThenBy``
parity — rows equal on every key keep input order) and supports **at most four keys**.
Single-key ordering is unchanged — it keeps its existing (unstable) sort, so there is
no performance regression on the common single-key case.

.. _linq_das_grouping:

Grouping
--------

``group <var> by <key>`` is a terminal (it replaces ``select``). It emits
``_group_by_lazy($(c) => <key>)`` and yields one ``tuple<key; array<elem>>``
bucket per distinct key — the C# *IGrouping* shape. Read the key as ``._0`` and
the group's elements as ``._1``:

.. code-block:: das

    var byBrand <- %linq! from c in cars group c by c.brand %%
    for (g in byBrand) {
        print("{g._0}: {g._1 |> length} cars\n")   // key, then count of that bucket
    }

A ``where`` may precede the ``group``; ``orderby`` may not directly (order the
groups in an ``into`` continuation instead — see :ref:`linq_das_into`). The group
element must be the range variable (``group c by …``) — element selectors are not
yet supported.

A **bare** ``group … by`` (no continuation) keeps the whole ``(key, [rows])``
group, so it is an **in-memory feature** (array / decs / XML / JSON); over a **SQL**
source it is rejected (SQL ``GROUP BY`` has no all-rows-per-group form). To
aggregate per group — the common case — add an ``into`` continuation
(``group c by k into g select (…, g |> length, g |> select(…) |> sum)``), which
**does** push down to SQL ``GROUP BY`` (see :ref:`linq_das_into`). (Over decs
these minimal ``orderby`` / ``group`` chains currently materialize rather than
fuse — correct, but a ``_fold`` perf advisory fires; a decs-adapter gap, not a
query-syntax one.)

.. _linq_das_into:

Query continuation (``into``)
-----------------------------

``into <var>`` rebinds the prior stage's output to a new range variable and
continues the query with more clauses — all on the **same** fused ``_fold``
chain, with no materialization between stages. It is supported on single-source
queries. C# uses it for grouped aggregation and to chain query stages.

**Group continuation** — after ``group c by k into g``, the new range variable
``g`` is the group. With the *A2* convention, ``g.key`` is the key and a bare
``g`` is the member collection:

.. code-block:: das

    var report <- %linq! from c in cars
                         group c by c.brand into g
                         select (brand = g.key,
                                 count = g |> length,
                                 total = g |> select($(u : Car) => u.price) |> sum) %%

A continuation may itself contain ``where`` / ``orderby`` / ``select`` / ``group``
over the groups — e.g. ``where g |> length > 1`` (drop singleton buckets) or
``orderby g |> select($(u : Car) => u.price) |> sum descending`` (order buckets by
total).

**Select continuation** — ``select <proj> into n`` rebinds the projected value to
``n`` and continues:

.. code-block:: das

    var kept <- %linq! from c in cars select c.price into p where p > 100 select p %%

Continuations chain (``… into x … into y …``), so a query can group, aggregate,
then filter/order the aggregated rows in one fused pass.

**SQL pushdown.** A group continuation whose select is **aggregate-only** —
``g.key`` plus ``g |> length`` (→ ``COUNT(*)``), ``g |> select(…) |> sum/average/min/max``
(→ ``SUM/AVG/MIN/MAX``), or ``g |> first()`` — pushes down to a SQL ``GROUP BY``
over a SQL source, exactly like the hand-written ``_group_by`` pipe form. A
**member-keeping** continuation (identity ``select g``, which keeps the whole
``(key, [rows])`` group) has no SQL form and is **in-memory only** (array / decs /
XML / JSON) — over a SQL source it is rejected, like a bare ``group … by``.

.. _linq_das_join:

Join
----

``join <var2> [ : <Row2> ] in <src2> on <keyA> equals <keyB>`` adds a single
**inner equi-join** — one new range variable, one equality key. The second
source is built exactly like the first (untyped → array, typed → the
``from_in`` dispatch), so it may be a different kind of source than the left.

The reader picks one of two emit shapes from the **post-join** clauses (it
transpiles before type inference and cannot see the source, so it decides
textually):

**Select-terminal** — no post-join ``where`` / ``orderby``, terminal is
``select``. The ``select`` projection *is* the join's result row (both range
variables are in scope), so it splices verbatim. A scalar or named-tuple
projection **pushes down to SQL**; a whole-row ``select c`` is in-memory only
(over SQL it has no column form — project columns instead):

.. code-block:: das

    var rows <- %linq! from c in cars join b in brands on c.brand equals b.brand
                       select (Name = c.name, Country = b.country) %%

A ``where`` *before* the ``join`` filters the left source (single range var) and
also pushes down — over an array/decs/XML/JSON source it fuses into the join's probe
loop (no intermediate filtered array). Several pre-join ``where``\ s AND-fold
(see :ref:`linq_das_filtering`):

.. code-block:: das

    var rows <- %linq! from c in cars where c.price >= 150 join b in brands
                       on c.brand equals b.brand select (Name = c.name, Country = b.country) %%

**Transparent identifier** — a post-join ``where`` (one or more) / ``orderby``,
or a ``group`` terminal. The join carries ``(c, b)`` as a pair so the later
clauses can address both variables; the reader rewrites ``c`` / ``b`` to the
carried fields, and each post-join ``where`` becomes its own filter. This is
**in-memory only** (array / decs / XML / JSON) — over a SQL source the carried
whole-row tuple has no column form and ``_sql`` rejects it (project columns in a
select-terminal join, or filter pre-join, to push down):

.. code-block:: das

    // post-join where sees the joined row (both c and b)
    var usa <- %linq! from c in cars join b in brands on c.brand equals b.brand
                      where b.country == "USA" select c.name %%

    // group the joined pairs by the right-side key → IGrouping whose elements
    // are the (c, b) pairs (read g._0 = key, g._1 = array of pairs)
    var byCountry <- %linq! from c in cars join b in brands on c.brand equals b.brand
                            group c by b.country %%

**Group join** (``join … equals … into <g>``) — C# ``GroupJoin``. ``into g``
binds ``g`` to the *array of matching right rows* alongside the left range
variable; the terminal ``select`` reads both. It is **outer** — every left row
surfaces, an unmatched one paired with an **empty** group. The reader emits
``_group_join``, which fuses through the same join splice (a pre-join ``where``
included), so the per-group aggregate runs in one hash-build + probe with no
intermediate:

.. code-block:: das

    // every brand with how many cars it has — a carless brand surfaces with 0
    var perBrand <- %linq! from b in brands join c in cars on b.brand equals c.brand into g
                           select (Brand = b.brand, N = g |> length) %%

    // aggregate over the group (sum of the matching cars' prices)
    var totals <- %linq! from b in brands join c in cars on b.brand equals c.brand into g
                         select (Brand = b.brand, Total = g |> select($(c : Car) => c.price) |> sum) %%

``join … into`` is **select-terminal + a pre-join ``where`` + a trailing
``iterator``** only, and **array sources only**: ``_group_join`` has no SQL
push-down (over a SQL source it rejects — write the aggregate in raw SQL
instead), and decs / XML / JSON group-joins are not yet fused. A post-``into`` ``where``
/ ``orderby`` / ``group`` over the ``(left, g)`` pair is rejected — ``g`` is a
non-copyable array that can't ride the transparent-identifier carry; materialize
then transform, or drop to the pipe-form ``_group_join``.

Only a **single equi-key** is supported — composite keys (``a equals b && c
equals d``), multiple joins, and ``orderby`` before ``group`` are rejected at
compile time.

.. _linq_das_multifrom:

Multiple ``from`` (SelectMany)
------------------------------

A second ``from`` introduces a second range variable over an **independent**
source — the cross product, C#'s *SelectMany*::

    from c in cars from b in brands select …

is every ``(c, b)`` pair. It shares the whole post-source clause grammar with
``join`` (pre/post ``where``, ``orderby``, ``select`` / ``group``, transparent
identifier, ``iterator``) — it is exactly a ``join`` with no ``on … equals`` key,
and emits ``_cross_join`` instead of ``_join``. The same two emit shapes apply:

**Select-terminal** — the ``select`` projection *is* the cross's result row (both
range variables in scope). Over a SQL source it **pushes down to a SQL CROSS
JOIN**; a scalar / named-tuple projection has a column form, a whole-row
``select c`` is in-memory only:

.. code-block:: das

    // 3 cars × 2 brands = 6 rows
    var rows <- %linq! from c in cars from b in brands
                       select (Name = c.name, Country = b.country) %%

A ``where`` *before* the second ``from`` filters the left source (single range
var) and pushes down; cross-then-filter on a key equality is the equi-join
subset. Both slots are repeatable (see :ref:`linq_das_filtering`):

.. code-block:: das

    // pre-from where filters cars, then crosses
    var rows <- %linq! from c in cars where c.price >= 150 from b in brands
                       select (Name = c.name, Country = b.country) %%

    // post-from where sees both vars (transparent identifier) — cross-then-filter
    var matched <- %linq! from c in cars from b in brands
                          where c.brand == b.brand select c.name %%

**Transparent identifier** — a post-from ``where`` / ``orderby`` or a ``group``
terminal carries ``(c, b)`` as a pair, in-memory only (same SQL boundary as
``join``).

Correlated ``from`` (flatten)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When the second source is a **field of the first** range variable
(``from l in o.lines``), the ``from … from`` *flattens* each outer row's
collection — C#'s correlated *SelectMany* — and emits ``_select_many`` (rather
than the cross's ``_cross_join``)::

    // one row per (order, line) pair across all orders
    var rows <- %linq! from o in orders from l in o.lines
                       select (Id = o.id, Sku = l.sku) %%

The result selector sees **both** range variables. A ``where`` after the second
``from`` that references **only the inner** variable is pushed into the
collection before flattening (identical semantics, no carry); several such
``where``\ s chain onto the collection::

    var rows <- %linq! from o in orders from l in o.lines where l.qty > 1
                       select (Id = o.id, Sku = l.sku) %%

A pre-``from`` ``where`` filters the outer rows, and ``iterator`` output works as
elsewhere.

Correlated flatten is an **in-memory array** feature: the outer row holds the
very collection being flattened, so it is non-copyable. A SQL source rejects it
(the inner collection is a per-row value, not a ``[sql_table]`` — use a ``join``
on a related table), an XML source cannot materialize nested collections
(``from_xml_node`` reads scalar attributes only), and decs rows have no nested
collection. Because the non-copyable outer cannot ride the transparent-identifier
carry, ``orderby`` / ``group`` over a correlated flatten, and a post-``from``
``where`` that references the **outer** variable, are rejected with a clear
message — project the fields you need in the ``select``, filter the outer with a
pre-``from`` ``where``, or restructure as a ``join``.

The second source of an **uncorrelated** ``from`` must be independent of the
first range variable. Only one extra ``from`` is supported (no N-ary
``from … from … from``), and ``from … from`` cannot be combined with ``join`` in
the same query.

Iterator vs array output
------------------------

By default a query materializes to an ``array<T>`` (via ``to_array()``). A
trailing ``iterator`` keyword yields an ``iterator<T>`` instead (via
``to_sequence()``), for feeding a ``for`` loop or another pipeline without
binding a stored array (the chain still materializes internally — see below):

.. code-block:: das

    // array (default)
    var names <- %linq! from c in cars where c.price > 100 select c.name %%

    // iterator — consume without binding a stored array
    for (nm in %linq! from c in cars where c.price > 100 select c.name iterator %%) {
        print("{nm}\n")
    }

The ``iterator`` form is an iterator over the optimized (fused / pushed-down)
result — it preserves each source's fusion, not lazy per-element streaming.
``T`` is the projection type, or the source element type for an identity
``select``.

Comments in the body
--------------------

Line (``// …``) and block (``/* … */``) comments inside the query body are
stripped before parsing (replaced with spaces, newlines preserved), so a
keyword or the range variable mentioned inside a comment never confuses the
clause splitter and never leaks into the spliced chain. String literals are not
treated as comments.

Current limitations
-------------------

The following are not yet supported:

- **Multi-key ``orderby``** (``orderby a, b descending``) — a single sort key
  only, for now.
- **``group … by`` over a SQL source**, and the ``group … into`` aggregate
  continuation (SQL grouping rides ``into``).
- **Composite / multiple / group joins** — ``join`` is a single inner equi-join
  with one key; composite keys (``a equals b && c equals d``), ``join … into``
  group-joins, and more than one ``join`` per query are rejected.
- **Post-join ``where`` / ``orderby`` / ``group`` over a SQL source** — these use
  the transparent-identifier carry, which is in-memory only; select-terminal
  joins and pre-join ``where`` push down.
- **Correlated multiple ``from`` over a non-array source** — the *flattening*
  SelectMany (``from l in o.lines``) is supported for **in-memory array** sources;
  a SQL source rejects it (no table for the per-row collection), and XML / decs
  have no nested-collection shape. ``orderby`` / ``group`` over a correlated
  flatten, and a post-``from`` ``where`` referencing the **outer** variable, are
  rejected (the non-copyable outer can't ride the transparent-identifier carry);
  an inner-only post-``from`` ``where`` is pushed into the collection. The
  **uncorrelated** form (independent second source → cross product) is supported
  on all sources (see :ref:`linq_das_multifrom`). N-ary ``from … from … from`` and
  ``from … from`` combined with ``join`` are also rejected.
- **``into`` continuations** — not yet supported (``let`` bindings *are*
  supported; see :ref:`linq_das_let`).
