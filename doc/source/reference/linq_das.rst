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

A query is ``from <var> [ : <Row> ] in <src> [ where <pred> ] [ ( join <var2>
[ : <Row2> ] in <src2> on <keyA> equals <keyB> | from <var2> [ : <Row2> ] in
<src2> ) ] [ where <pred> ] [ orderby <expr> [descending] ] ( select <proj> |
group <var> by <key> ) [ iterator ]`` — a second range variable comes from
**either** a ``join`` **or** a second ``from`` (never both), and at most one of
the two ``where`` slots may appear (before *or* after that clause, never both):

- ``from <var> in <source>`` — the element bind ``<var>`` names the per-row
  value. With no type annotation, ``<source>`` is an ``array<T>``.
- ``where <predicate>`` — optional. A ``where`` **before** the ``join`` / second
  ``from`` filters the left source (single range var); a ``where`` **after** it
  sees both range variables. At most one ``where`` per query.
- ``join <var2> in <src2> on <keyA> equals <keyB>`` — optional, a single inner
  equi-join introducing a second range variable (see :ref:`linq_das_join`).
- ``from <var2> in <src2>`` — optional, a second ``from`` introducing a second
  range variable — SelectMany: an **independent** source is the cross product, a
  source that is a field of the first range variable (``from l in o.lines``) is
  the correlated flatten (see :ref:`linq_das_multifrom`).
- ``orderby <expr> [descending]`` — optional, a **single** sort key (see
  :ref:`linq_das_ordering`). Omitted when absent.
- ``select <projection>`` — ``select <var>`` (the identity projection) returns
  the rows unchanged; any other projection emits ``_select(...)``.
- ``group <var> by <key>`` — the alternative terminal to ``select`` (see
  :ref:`linq_das_grouping`).

A query ends with **either** ``select`` **or** ``group … by`` — exactly one.
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

For value sources (SQL, XML, and later JSON) the reader emits
``from_in(<src>, type<Row>)``; the ``from_in`` call macro dispatches on the
source value's type to the concrete builder (so a new backend is a new
``from_in`` branch, never a parser change). ``decs`` has no source value, so it
is emitted directly as ``from_decs_template`` and never goes through
``from_in``. The row type's required annotation depends on the source —
``[decs_template]`` for decs, ``[sql_table]`` / ``[sql_view]`` for SQL, a plain
struct for XML.

Range variable
--------------

The range variable is spliced **verbatim** as the lambda parameter — the
predicate becomes ``_where($(c) => …)`` and the projection ``_select($(c) =>
…)``, keeping the C# variable name; the predicate and projection text is passed
through unchanged. Any identifier name works.

The ``_fold`` operator DSL accepts a named-variable ``$(x) => …`` block
directly. For the SQL source, ``_sql`` resolves a single source against the
placeholder ``_``; the macro normalizes the single-source lambda parameter to
``_`` internally, so the C# variable name is still spliced verbatim at the
surface.

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

``orderby <expr> [descending]`` sorts by a **single** key, emitting
``_order_by($(c) => <expr>)`` (or ``_order_by_descending(...)``) between the
``where`` and the ``select``. ``descending`` (and the default-explicit
``ascending``) are recognized as trailing keywords:

.. code-block:: das

    // ascending (default)
    var byPrice <- %linq! from c in cars orderby c.price select c.name %%

    // descending, after a where
    var top <- %linq! from c in cars where c.price > 100 orderby c.price descending select c.name %%

Works over all four sources (SQL emits ``ORDER BY … [DESC]``; array / decs / XML
sort the materialized rows). **Multi-key** ordering (``orderby a, b descending``)
is not yet supported — there is no ``_then_by`` operator; use a single key for
now.

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

A ``where`` may precede the ``group``; ``orderby`` may not (ordering the groups
needs the deferred ``into`` continuation). The group element must be the range
variable (``group c by …``) — element selectors are not yet supported.

**Grouping is an in-memory feature** (array / decs / XML). Over a **SQL** source
it is rejected at compile time: SQL ``GROUP BY`` has no all-rows-per-group form,
only aggregates. Use the aggregate pipe form for SQL grouping —
``db |> select_from(type<Car>) |> _group_by(_.brand) |> _select((B = _._0, N = _._1 |> count())) |> _sql()``
— until the ``group … into`` continuation lands. (Over decs these minimal
``orderby`` / ``group`` chains currently materialize rather than fuse — correct,
but a ``_fold`` perf advisory fires; a decs-adapter gap, not a query-syntax one.)

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
also pushes down — over an array/decs/XML source it fuses into the join's probe
loop (no intermediate filtered array):

.. code-block:: das

    var rows <- %linq! from c in cars where c.price >= 150 join b in brands
                       on c.brand equals b.brand select (Name = c.name, Country = b.country) %%

**Transparent identifier** — a post-join ``where`` / ``orderby``, or a ``group``
terminal. The join carries ``(c, b)`` as a pair so the later clauses can address
both variables; the reader rewrites ``c`` / ``b`` to the carried fields. This is
**in-memory only** (array / decs / XML) — over a SQL source the carried
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

Only a **single equi-key** is supported — composite keys (``a equals b && c
equals d``), group-joins (``join … into g``), multiple joins, and ``orderby``
before ``group`` are rejected at compile time.

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
subset:

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
collection before flattening (identical semantics, no carry)::

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

- **JSON** as a source (the planned 5th adapter).
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
- **``let`` bindings** and ``into`` continuations — not yet supported.
