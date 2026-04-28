.. _tutorial_sql_list_tables:

==========================================
SQL-30 --- Listing tables
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; sqlite_master
    single: Tutorial; catalog

"What tables exist in this DB?" is genuinely provider-specific --- the
catalog spelling and the columns it surfaces (system tables, virtual
tables, partitions, schemas) diverge in non-trivial ways:

.. list-table::
   :header-rows: 1

   * - Backend
     - Catalog query
   * - SQLite
     - ``SELECT name FROM sqlite_master WHERE type='table'``
   * - PostgreSQL
     - ``SELECT tablename FROM pg_tables WHERE schemaname='public'``
   * - MySQL
     - ``SELECT table_name FROM information_schema.tables``
   * - MSSQL
     - ``SELECT name FROM sys.tables``

dasSQLITE deliberately does **not** ship a
``db |> tables() : array<string>`` abstract helper for this reason:
a uniform-subset contract would either lie about the differences or
grow per-provider escape hatches. Listing tables uses the same raw-SQL
escape hatch as :ref:`tut 29 Band 3 <tutorial_sql_column_names>` ---
the typed ``query(sql, type<T>, …)`` family.

End-to-end
==========

``[sql_table]`` on a read-only row shape opts into the materializer rail
without claiming a real underlying table:

.. code-block:: das

    [sql_table(name = "sqlite_master_rows")]
    struct MasterTable {
        @sql_column = "type"
        row_type : string
        name : string
    }

    for (t in db |> query(
            "SELECT type, name FROM sqlite_master WHERE type='table' ORDER BY name",
            type<MasterTable>)) {
        to_log(LOG_INFO, "{t.row_type}: {t.name}\n")
    }

Drop the ``WHERE type='table'`` filter to see the whole catalog
(indexes, views, triggers --- anything SQLite has stored under that
schema):

.. code-block:: das

    [sql_table(name = "sqlite_master_full")]
    struct MasterFull {
        @sql_column = "type"
        row_type : string
        name : string
        tbl_name : string
        rootpage : int
        sql : string
    }

    for (m in db |> query(
            "SELECT type, name, tbl_name, rootpage, sql FROM sqlite_master ORDER BY type, name",
            type<MasterFull>)) {
        to_log(LOG_INFO, "{m.row_type} {m.name} (tbl={m.tbl_name})\n")
    }

Parameterized listing rides the standard ``query`` positional-bind
overloads:

.. code-block:: das

    let prefix = "C"
    for (t in db |> query(
            "SELECT type, name FROM sqlite_master WHERE type='table' AND name LIKE ?",
            type<MasterTable>, "{prefix}%")) {
        to_log(LOG_INFO, "user table starting with '{prefix}': {t.name}\n")
    }

Why no abstract ``list_tables``
================================

EF Core does not expose a runtime "what tables physically exist" call
either: ``context.Model.GetEntityTypes()`` lists the
``[sql_table]``-equivalents *your code declares*, not what the DB
actually stores. Genuine DB-driven listing drops to design-time
scaffolding or raw ADO.NET. Same trade-off here.

* If your code knows the schema, drive off the daslang types ---
  :ref:`column_info <tutorial_sql_column_names>` and the
  ``[sql_table]`` structs you already declared.
* If the schema lives in the DB and you're admin-tooling against it,
  ``query(provider_specific_sql, type<Row>)`` is the honest spelling.

Deferred
========

* **Cross-provider catalog query helper** (tables / indexes / views).
  Could ship as ``daslib/sql_admin`` once a second provider lands and
  the lowest-common-denominator subset is concrete. Premature to
  design with one backend in hand.
* **Schema-diff helpers** (compare DB catalog ↔ declared
  ``[sql_table]`` types). Useful for migration tooling but pushes
  design choices around what counts as a "diff" (column renames vs.
  drop+add, index name normalization, view body equivalence). Defer
  until a real migration story is on the table.

.. seealso::

    Full source: :download:`tutorials/sql/30-list_tables.das <../../../../tutorials/sql/30-list_tables.das>`

    Previous tutorial: :ref:`tutorial_sql_column_names`
