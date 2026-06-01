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

Clauses (v1)
------------

Version 1 supports ``from`` / ``where`` / ``select`` over an array source:

- ``from <var> in <source>`` — ``<source>`` is any expression evaluating to an
  ``array<T>``. The element bind ``<var>`` names the per-row value.
- ``where <predicate>`` — optional. Omitted when absent.
- ``select <projection>`` — required. ``select <var>`` (the identity
  projection) returns the filtered elements unchanged; any other projection is
  emitted as ``_select(...)``.

The clauses may span multiple lines inside the ``%linq! … %%`` body.

Range variable
--------------

The range variable is spliced **verbatim** as the lambda parameter — the
predicate becomes ``_where($(c) => …)`` and the projection ``_select($(c) =>
…)``, keeping the C# variable name. The predicate and projection text is passed
through unchanged (no token rewriting). Any identifier name works, and it may
appear any number of times.

The ``_fold`` operator DSL accepts a named-variable ``$(x) => …`` block
directly: ``_where`` / ``_select`` (and the rest of the placeholder operators)
inject the element type onto the parameter and pass the block through. The
``_`` placeholder form (``_where(_ < 5)``) remains available for hand-written
chains.

Result
------

The query materializes to an ``array<T>`` (via ``to_array()``), where ``T`` is
the projection type — or the source element type for an identity ``select``.

Current limitations (v1)
------------------------

The following are not yet supported and produce a compile error:

- **Typed sources** — ``from c : Car in db`` (the annotated range variable that
  selects a non-array source such as SQL, decs, or JSON).
- **Additional clauses** — ``orderby`` / ``group by`` / ``join``.
- **Multiple ``from``** (SelectMany), ``let`` bindings, and ``into``
  continuations.

v1 parses a single ``from``, so a query has one range variable. The operator
DSL now accepts named-variable blocks (so multiple range variables are
mechanically expressible), but the multi-variable query forms — joins and
multiple ``from`` — are not yet parsed by the reader macro.
