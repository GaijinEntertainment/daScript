.. _tutorial_sql_column_names:

==========================================
SQL-29 --- Column metadata
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; column_info
    single: Tutorial; schema introspection
    single: Tutorial; PRAGMA

Two bands cover the schema-introspection question in increasing order
of how dynamic the answer needs to be.

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Band
     - Use it when…
   * - **Band 1: ``column_info(type<T>)``**
     - the schema is in your code (a ``[sql_table]`` struct) and you
       want to drive UI / codegen / validation off it. Free relative
       to declaring the struct; no DB round-trip.
   * - **Band 3: ``query("PRAGMA table_info(...)", type<Row>)``**
     - the DB is the source of truth (admin tools, migrations,
       reverse-engineering). One ``PRAGMA`` per call; SQLite-specific
       spelling.

Band 2 (design-time scaffolding that reverse-engineers a DB into
``[sql_table]`` structs) is out of scope --- it is dev-time tooling,
not user API. EF Core only ships it as ``dotnet ef dbcontext scaffold``.

Band 1 --- ``column_info(type<T>)``
====================================

``column_info`` is the first ``typeinfo``-over-``[sql_table]`` user-facing
API. ``[sql_table]`` already walks struct fields at compile time to emit
DDL and bind/column code; ``column_info`` is a view over that same walk:

.. code-block:: das

    enum SqlType {
        Integer            // INTEGER (SQLite) / INT (MySQL, MSSQL) / INT4 (PG)
        Real               // REAL    / DOUBLE / FLOAT / FLOAT8
        Text               // TEXT    / VARCHAR / NVARCHAR / TEXT
        Blob               // BLOB    / BLOB    / VARBINARY / BYTEA
        Null
    }
    struct ColumnInfo {
        name : string
        data_type : SqlType
        is_pk : bool
        is_nullable : bool
        default_expr : string   // "" if none
    }

``SqlType`` is **abstract** --- it lives in ``daslib/sql``. Provider
helpers render the dialect-specific spelling:

.. code-block:: das

    def sqlite_sql_type(t : SqlType) : string

Future ``postgres_sql_type``, ``mysql_sql_type``, … live next to their
respective boost modules. The same ``ColumnInfo`` round-trips across
backends without ambiguity.

End-to-end
==========

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost

    [sql_table(name = "Cars")]
    struct Car {
        @sql_primary_key Id : int
        Name : string
        Price : float = 9.99
        @safe_when_uninitialized Tag : Option<string>
        @sql_default_fn = "CURRENT_TIMESTAMP"
        CreatedAt : string
    }

    [export]
    def main {
        for (c in column_info(type<Car>)) {
            var pk = c.is_pk ? " PK" : ""
            var nn = c.is_nullable ? "" : " NOT NULL"
            var df = empty(c.default_expr) ? "" : " DEFAULT {c.default_expr}"
            to_log(LOG_INFO, "{c.name} {sqlite_sql_type(c.data_type)}{pk}{nn}{df}\n")
        }
    }

Output::

    Id INTEGER PK NOT NULL
    Name TEXT NOT NULL
    Price REAL NOT NULL DEFAULT 9.99
    Tag TEXT
    CreatedAt TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP

The array is plain data --- it composes with every daslang idiom (linq
comprehensions, ``for``-in, filters) without further ceremony:

.. code-block:: das

    let pk_cols = [for (c in column_info(type<Car>)); c; where c.is_pk]

``@sql_json`` and ``@sql_blob`` short-circuit the witness lookup ---
``column_info`` reports ``SqlType.Text`` for JSON columns and
``SqlType.Blob`` for archive columns regardless of the daslang field
type. Computed columns appear in the array with empty ``default_expr``.

Band 3 --- raw PRAGMA via ``query``
====================================

``[sql_table]`` on a read-only row shape is the idiomatic way to opt
into the typed materializer without committing to a CREATE TABLE:

.. code-block:: das

    [sql_table(name = "pragma_columns")]
    struct PragmaColumn {
        cid : int
        name : string
        @sql_column = "type"
        col_type : string
        notnull : int
        dflt_value : string
        pk : int
    }

    for (col in db |> query("PRAGMA table_info(Cars)", type<PragmaColumn>)) {
        to_log(LOG_INFO, "{col.name} : {col.col_type} pk={col.pk}\n")
    }

The ``query`` family rides the same machinery as any other typed read
--- 0 to 3 positional bind args, ``try_query`` for ``Result``-flavored
error handling.

``@sql_column = "<sql_name>"`` renames the on-disk column without
changing the daslang identifier. Pick it up when the SQL column name is
a daslang or SQL keyword (e.g. ``type``, ``for``, ``if``), or when your
code style favors ``snake_case`` while the schema disagrees. The rename
flows uniformly through DDL, ``_sql`` predicates / projections /
``_order_by`` / ``_group_by``, ``column_info(type<T>).name``, INSERT /
UPDATE / DELETE / upsert SQL, RETURNING clauses, ``[sql_index]``, and
``@sql_references`` to a renamed parent PK. ``query()`` materializes
positionally, so the rename is invisible there --- but anywhere else
the daslang struct touches SQL, the SQL identifier reflects
``@sql_column``.

When to reach for which band
============================

* **Band 1** (``column_info``) is the answer for app code and tools
  that own their schema. Most code never needs Band 3.
* **Band 3** (``query`` + PRAGMA) shows up in migration scripts and
  inspection CLIs --- places where the DB is the source of truth and
  you genuinely don't have a daslang struct describing the columns.

Why no abstract ``db |> schema(name) : array<ColumnInfo>``
==========================================================

A name-keyed runtime helper that returns the same ``ColumnInfo`` shape
--- pulling data from PRAGMA instead of the macro walk --- is a
tempting symmetry. Two reasons it does not ship:

1. ``dflt_value`` from PRAGMA comes back as the SQL-side default
   AS STORED. Subtle round-trip mismatch with Band 1's daslang-init
   form (``9.99`` vs ``"9.9899997711182"``-style PRAGMA artifacts).
2. PRAGMA exposes a different subset than the daslang macro can see.
   CHECK constraints, FOREIGN KEY referent tables, expression-index
   columns, view bindings --- backends spell these differently. A
   "uniform" ``ColumnInfo`` over runtime PRAGMA would either lie or
   grow per-provider escape hatches.

The honest path: Band 1 stays compile-time-typed, Band 3 stays the
raw-SQL escape hatch, and the boundary is explicit.

.. seealso::

    Full source: :download:`tutorials/sql/29-column_names.das <../../../../tutorials/sql/29-column_names.das>`

    Previous tutorial: :ref:`tutorial_sql_json`

    Next tutorial: :ref:`tutorial_sql_list_tables`
