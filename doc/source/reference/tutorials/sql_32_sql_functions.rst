.. _tutorial_sql_register_function:

============================================
SQL-32 --- User-defined SQL scalar functions
============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; register_function
    single: Tutorial; user-defined function
    single: Tutorial; UDF
    single: Tutorial; sqlite3_create_function

SQL is fluent at filtering and grouping but speaks a small built-in
vocabulary. When an expression doesn't fit (a custom hash, a
domain-specific scoring formula, a string normalizer),
``register_function`` binds an ordinary daslang function as a SQL
scalar so it can be used inside any expression --- ``WHERE``,
projection, ``ORDER BY``, indexes, view bodies, anywhere a built-in
scalar would go.

The contract
============

.. code-block:: das

    db |> register_function(name : string, fn : @@<...>;
                            deterministic : bool = false;
                            directonly    : bool = false)

* ``name`` --- the SQL identifier the function will be invoked under.
  Re-registering the same name replaces the prior binding.
* ``fn`` --- a daslang **function pointer** (``@@`` form). The macro
  inspects the pointer's signature at compile time, derives the per-arg
  + return type tags, and ships them to the C++ trampoline.
* ``deterministic = true`` --- same inputs always yield the same
  output. Required to use the fn inside ``CREATE INDEX ... ON
  tbl(myfn(col))`` or in a generated column.
* ``directonly = true`` --- the fn cannot be called from triggers,
  views, or ``CHECK(...)`` constraints. Useful to lock down
  side-effecting helpers that should run only from application SQL.

**Lifetime.** Registration is per-connection. The function is available
until the connection closes (or until ``DROP FUNCTION`` if exposed in
a future chunk).

**NULL handling.** If any argument is SQL ``NULL``, the trampoline
emits ``NULL`` without invoking the daslang function --- the standard
SQL behavior of built-in scalars (``abs(NULL) -> NULL``,
``length(NULL) -> NULL``). Explicit ``Option<T>`` arg types for
in-function NULL handling are planned but not yet shipped.

**Panic recovery.** A panic inside the daslang function is caught and
surfaced to SQLite as an error on that statement; ``try_query``
returns ``Err`` carrying the panic message. The connection itself is
unaffected and the next statement runs normally.

Supported types
===============

``register_function`` maps daslang scalar types to SQLite cells:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - daslang type
     - SQLite cell
   * - ``int`` / ``int64`` / ``bool``
     - INTEGER
   * - ``float`` / ``double``
     - REAL
   * - ``string``
     - TEXT

Unsupported types (structs, arrays, pointers, lambdas, classes) fail
at compile time with a per-position diagnostic. Up to 4 arguments are
supported in v1.

End-to-end
==========

A scoring formula, a string hasher, a 2-D distance: three real-shaped
SQL helpers, all marked deterministic so SQLite is allowed to factor
them out of inner loops or use them as the indexed expression in a
``CREATE INDEX``.

.. code-block:: das

    require sqlite/sqlite_boost
    require sqlite/sqlite_linq
    require math

    def damage_formula(base : float; armor : int; mult : float) : float {
        return base * (1.0f - float(armor) / 100.0f) * mult
    }

    def short_hash(s : string) : int64 {
        var h = 1469598103934665603l
        for (c in s) {
            h = (h ^ int64(c)) * 1099511628211l
        }
        return h
    }

    def euclid(x1 : float; y1 : float; x2 : float; y2 : float) : float {
        let dx = x2 - x1
        let dy = y2 - y1
        return sqrt(dx * dx + dy * dy)
    }

    [sql_table(name = "Enemies")]
    struct Enemy {
        @sql_primary_key Id : int
        Name : string
        Hp : float
        Armor : int
    }

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            db |> create_table(type<Enemy>)
            // ... insert rows ...

            db |> register_function("damage_formula", @@damage_formula, true)
            db |> register_function("short_hash",     @@short_hash,     true)
            db |> register_function("euclid",         @@euclid,         true)
        }
    }

Once registered, the function is just another SQL scalar:

**Inside a projection** (per-row SELECT result) ---

.. code-block:: das

    let dmg <- (db |> try_run_select(
        "SELECT damage_formula(100.0, Armor, 1.5) FROM Enemies ORDER BY Id",
        $(var stmt : sqlite3_stmt?) {},
        $(stmt : sqlite3_stmt?) : float {
            return float(sqlite3_column_double(stmt, 0))
        })) |> unwrap

**Inside a WHERE clause** (filter predicate) ---

.. code-block:: das

    let weak_count : int = db |> query_scalar(
        "SELECT COUNT(*) FROM Enemies WHERE damage_formula(100.0, Armor, 1.5) > 50",
        type<int>)

**Inside ORDER BY** (sort key) ---

.. code-block:: das

    let first_by_hash = db |> query_scalar(
        "SELECT Name FROM Enemies ORDER BY short_hash(Name) LIMIT 1",
        type<string>)

**Inside an aggregate** (multi-arg numeric fn) ---

.. code-block:: das

    let total = db |> query_scalar(
        "SELECT SUM(euclid(StartX, StartY, EndX, EndY)) FROM Trips",
        type<double>)

Provider-specific
=================

``register_function`` is **SQLite-provider-specific**. Other backends
have their own user-defined-function registration rails (PostgreSQL's
``CREATE FUNCTION`` with PL/pgSQL, MySQL's UDF interface, MSSQL's
``CREATE FUNCTION`` + assemblies). A future cross-provider abstraction
would have to round-trip across all of them; for now, the daslang
function lives behind ``sqlite/sqlite_boost``.

Deferred
========

* **Aggregate functions** (``sqlite3_create_function`` with step +
  finalize callbacks). Useful for custom ``GROUP BY`` aggregates ---
  ``MEDIAN``, ``STDEV``, percentile families. Defer until a real
  consumer asks.
* **Window functions** (``sqlite3_create_window_function``). Specialized
  enough to wait for a real use case.
* **Explicit** ``Option<T>`` **arg types.** Today the trampoline
  short-circuits ``NULL`` arguments; an explicit ``Option<T>`` arg
  would let the function decide what ``NULL`` means. Planned.
* **Variadic + > 4 args.** v1 caps at 4 positional args. Lift the cap
  when something real wants it.

.. seealso::

    Full source: :download:`tutorials/sql/32-sql_functions.das <../../../../tutorials/sql/32-sql_functions.das>`

    Previous tutorial: :ref:`tutorial_sql_views`

    Next tutorial: :ref:`tutorial_sql_pragma`
