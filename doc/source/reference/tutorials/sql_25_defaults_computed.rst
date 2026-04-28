.. _tutorial_sql_defaults_computed:

==========================================
SQL-25 --- Defaults + computed columns
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; default
    single: Tutorial; DEFAULT
    single: Tutorial; computed column
    single: Tutorial; GENERATED ALWAYS AS
    single: Tutorial; sql_default_fn
    single: Tutorial; sql_computed
    single: Tutorial; sql_stored

Three ways to default a column, plus generated columns via
``@sql_computed``.

Three default sources
=====================

==============================================  ===================================================
Form                                            Behavior
==============================================  ===================================================
``Active : bool = true``                        native daslang field-init becomes ``DEFAULT 1``
``@sql_default_fn = "CURRENT_TIMESTAMP"``       SQL built-in (whitelist below)
``@sql_computed = "Quantity * 2"``              generated column --- excluded from INSERT/UPDATE bind
==============================================  ===================================================

Native field-init handles bool / int / int64 / float / double /
string literals. Non-literal initializers (e.g. ``some(5)``) are
silently dropped from the DDL but still construct in daslang.

Whitelisted ``@sql_default_fn`` values: ``CURRENT_TIMESTAMP``,
``CURRENT_DATE``, ``CURRENT_TIME``. Anything else is a compile
error.

VIRTUAL vs STORED
=================

``@sql_computed`` defaults to ``VIRTUAL`` (recomputed on every
``SELECT``). Add ``@sql_stored = true`` for ``STORED``
(materialized on ``INSERT`` / ``UPDATE``, persisted on disk, read
straight off disk on ``SELECT``).

Use ``STORED`` when the expression is expensive and reads dominate
writes. Default to ``VIRTUAL`` otherwise.

Schema example
==============

.. code-block:: das

    [sql_table(name = "Items")]
    struct Item {
        @sql_primary_key Id : int

        Name : string

        Active : bool = true                     // -> DEFAULT 1
        Quantity : int = 100                     // -> DEFAULT 100
        Tag : string = "unknown"                 // -> DEFAULT 'unknown'

        @sql_default_fn = "CURRENT_TIMESTAMP"
        CreatedAt : string

        @sql_computed = "Quantity * 2"
        DoubleQty : int                          // VIRTUAL

        @sql_computed = "Quantity + 1"
        @sql_stored = true
        QtyPlusOne : int                         // STORED
    }

Macro INSERT binds every NON-COMPUTED column
============================================

The struct field's value is what gets written; defaults never fire
through this path. Computed columns are skipped --- SQLite computes
them itself, so any value the struct holds for ``DoubleQty`` /
``QtyPlusOne`` is ignored.

.. code-block:: das

    db |> insert(Item(Id = 1, Name = "thing", Active = true, Quantity = 7,
                      Tag = "t", CreatedAt = "",
                      DoubleQty = 9999, QtyPlusOne = 9999))
    // INSERT INTO "Items" ("Id","Name","Active","Quantity","Tag","CreatedAt")
    //   VALUES (?,?,?,?,?,?)

Defaults fire when the column is omitted
========================================

The macro ``INSERT`` always names every non-computed column, so the
``DEFAULT`` clause only fires from raw SQL or column-list forms that
omit the column.

.. code-block:: das

    db |> exec("INSERT INTO Items (Id, Name) VALUES (2, 'defaulted')")
    // -> "Active" DEFAULT 1, "Quantity" DEFAULT 100,
    //    "Tag" DEFAULT 'unknown', "CreatedAt" DEFAULT CURRENT_TIMESTAMP all fire

UPDATE re-derives computed columns
==================================

Mutating ``Quantity`` is enough --- the SET clause omits any
``@sql_computed`` column (typo at the macro level produces a
``do_update column "X" is @sql_computed`` error). ``DoubleQty`` /
``QtyPlusOne`` update behind the scenes when SQLite re-evaluates
the expression.

.. code-block:: das

    db |> _sql_update(type<Item>, _.Id == 1, (Quantity = 11))
    // UPDATE "Items" SET "Quantity" = ? WHERE "Id" = ?

Validation rules
================

``[sql_table]`` rejects each of the following at compile time:

* ``@sql_primary_key`` + ``@sql_computed``
* ``@sql_computed`` + field initializer
* ``@sql_computed`` + ``@sql_default_fn``
* field initializer + ``@sql_default_fn``
* ``@sql_default_fn`` value outside the whitelist

Minimum SQLite version
======================

Generated columns require SQLite 3.31+ (2019-10-10).

.. seealso::

    Full source: :download:`tutorials/sql/25-defaults_computed.das <../../../../tutorials/sql/25-defaults_computed.das>`

    Previous tutorial: :ref:`tutorial_sql_indexes`
