.. _tutorial_sql_parametrized:

==================================
SQL-05 --- Parameter Binding
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; bind parameters
    single: Tutorial; query_one
    single: Tutorial; _first
    single: Tutorial; _first_opt

Parameter binding has two stories in dasSQLITE, and the chunk-3 design
is to keep the everyday one **invisible**.

LINQ form: capture is binding
=============================

Inside ``_sql(...)``, the macro's expression walker classifies each
name in the chain as either a **column reference** (``_.Field``) or a
**captured value** (anything else). Captured values are lowered to
``?`` placeholders and bound automatically --- you never type ``?`` or
``:name``:

.. code-block:: das

    let target = 3
    let car = _sql(db |> select_from(type<Car>)
                     |> _where(_.Id == target)
                     |> _first())
    // emits:  SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Id" = ? LIMIT 1
    // binds:  [target]

``_first()`` is the single-row terminal: it adds ``LIMIT 1`` and
panics if no row matches. ``_first_opt()`` is the non-panic sibling
--- same SQL, but the result is ``Option<T>`` (``none`` on empty):

.. code-block:: das

    let maybe = _sql(db |> select_from(type<Car>)
                       |> _where(_.Id == 999)
                       |> _first_opt())
    // maybe : Option<Car>;  is_some -> false here

Tutorial 5's whole conceptual surface (parameter binding) becomes
invisible at the call site. That's the design win.

Raw-SQL escape hatch: ``query_one`` with positional ``?``
=========================================================

For migrations, ad-hoc queries, or anything ``_sql`` can't translate,
the ``query_one`` family takes positional ``?`` arguments via
variadic trailing args:

.. code-block:: das

    let raw = db |> query_one(
        "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ?",
        type<Car>, target)

The trailing args bind to ``?`` placeholders in declaration order.
Mixed types are fine --- daslang's overload resolution dispatches to
the right ``sqlite_bind`` for each arg:

.. code-block:: das

    let multi = db |> query_one(
        "SELECT \"Id\", \"Name\", \"Price\" FROM Cars WHERE Id = ? AND Name = ?",
        type<Car>, 2, "Audi")

Chunk 3 ships 0..3 positional args. Named-tuple bind for ``:name``
placeholders ships in a follow-up.

Strict / Result / Option triples
================================

Every read helper has the same three flavours --- pick by what
"no row" or "error" should mean at the call site:

==============================================================  ==========================================================
Form                                                            Behaviour
==============================================================  ==========================================================
``query_one(...)``                                              Strict; panics on prepare/step error or 0 rows
``try_query_one(...) : Result<T, string>``                      Returns ``Err`` on prepare/step error or 0 rows
``query_one_opt(...) : Option<T>``                              Panics on error; ``none`` on 0 rows
``_sql(... |> _first())``                                       Strict; panics on 0 rows or step failure
``_try_sql(... |> _first()) : Result<T, string>``               Returns ``Err`` on 0 rows or step failure
``_sql(... |> _first_opt()) : Option<T>``                       Panics on error; ``none`` on 0 rows
==============================================================  ==========================================================

Tutorial 6 walks the full matrix with worked examples.

Quick reference
===============

==============================================================  ==========================================================
Form                                                            Description
==============================================================  ==========================================================
``... |> _first()``                                             Single-row terminal; ``LIMIT 1`` + panic on empty
``... |> _first_opt()``                                         Single-row terminal; ``Option<T>``, ``none`` on empty
``db |> query_one(sql, type<T>, args...)``                      Raw-SQL one-row read; up to 3 positional ``?`` binds
``db |> try_query_one(sql, type<T>, args...)``                  Same; returns ``Result<T, string>``
``db |> query_one_opt(sql, type<T>, args...)``                  Same; returns ``Option<T>`` (``none`` on 0 rows)
==============================================================  ==========================================================

.. seealso::

    Full source: :download:`tutorials/sql/05-parametrized.das <../../../../tutorials/sql/05-parametrized.das>`

    Previous tutorial: :ref:`tutorial_sql_select_all`

    Next tutorial: :ref:`tutorial_sql_error_handling`
