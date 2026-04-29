.. _tutorial_sql_pragma:

==========================================
SQL-33 --- PRAGMA tuning
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; pragma
    single: Tutorial; set_pragma
    single: Tutorial; WAL
    single: Tutorial; journal_mode

PRAGMA is SQLite's per-connection control surface. It covers
performance toggles (``journal_mode``, ``synchronous``, ``cache_size``,
``mmap_size``), behavior switches (``foreign_keys``,
``recursive_triggers``, ``trusted_schema``), and diagnostics
(``integrity_check`` / ``quick_check`` --- see
:ref:`tut 34 <tutorial_sql_backup_vacuum>`).

Some PRAGMAs are connection-scoped (``foreign_keys``,
``busy_timeout``); others are database-scoped and persist on disk
(``journal_mode``, ``page_size`` after VACUUM). dasSQLITE's helpers
make no distinction --- they emit ``PRAGMA "<name>" = <value>`` and
let SQLite handle the rest.

The contract
============

Three typed ``value`` overloads cover the common shapes:

.. code-block:: das

    set_pragma(db; name; value : string)    // 'WAL', 'NORMAL', 'utf8'
    set_pragma(db; name; value : int64)     // 5000, 50000, 4096
    set_pragma(db; name; value : bool)      // ON / OFF

Each has a ``try_set_pragma`` sibling returning ``SqlError`` for
non-panic recovery. PRAGMA values can't be bound with ``?`` ---
SQLite parses them at prepare time, so the helpers format the value
into the SQL inline. String values are single-quoted with ``''``
doubling for safety. PRAGMA names + values are admin-controlled (not
user input), so this is fine.

Reading PRAGMAs back uses the typed ``query_scalar`` rail
(:ref:`tut 13 <tutorial_sql_aggregates>`):

.. code-block:: das

    let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
    let busy = db |> query_scalar("PRAGMA busy_timeout", type<int>)
    let fks  = db |> query_scalar("PRAGMA foreign_keys", type<int>)

End-to-end
==========

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            // Strict variants --- panic on a malformed pragma.
            db |> set_pragma("journal_mode", "WAL")     // string
            db |> set_pragma("busy_timeout", 5000l)     // int64 (note `l` suffix)
            db |> set_pragma("foreign_keys", true)      // bool

            let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
            to_log(LOG_INFO, "journal_mode = {mode}\n")
        }
    }

Recovery: ``try_set_pragma``
============================

SQLite rejects ``journal_mode`` changes inside an open transaction.
The strict form would panic; the ``try_`` form returns ``SqlError``:

.. code-block:: das

    db |> with_transaction() {
        let err = db |> try_set_pragma("journal_mode", "DELETE")
        if (err |> is_some) {
            to_log(LOG_INFO, "journal_mode rejected mid-tx: {err |> unwrap}\n")
        }
    }

``apply_recommended_pragmas``
=============================

For typical app-server workloads dasSQLITE ships a curated bundle ---
call it right after ``with_sqlite`` opens the connection:

.. code-block:: das

    let setup_err = db |> try_apply_recommended_pragmas()
    if (setup_err |> is_some) {
        to_log(LOG_ERROR, "failed to apply pragmas: {setup_err |> unwrap}\n")
    }

The current bundle is documented in ``sqlite_boost.das``; today it is:
``journal_mode = WAL``, ``busy_timeout = 5000``,
``foreign_keys = ON``, ``synchronous = NORMAL``. Workloads with
different needs (durability-first servers want ``synchronous = FULL``;
ROM-like data wants ``journal_mode = OFF``) call ``set_pragma``
directly.

Multi-row PRAGMAs
=================

PRAGMAs that emit one row per item (``table_info``, ``index_list``,
``integrity_check``, ``foreign_key_list``) ride the ``query`` family
with a typed row struct:

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

See :ref:`tut 29 <tutorial_sql_column_names>` for ``column_info`` ---
the compile-time alternative when the schema is in your code.

The unknown-PRAGMA quirk
========================

SQLite is famously permissive with PRAGMA: typos succeed silently
(no error), and the corresponding ``query_scalar`` read returns ``0``
/ ``""`` / row count ``0`` depending on shape. The lookup table is
closed; sqlite3 just ignores names it doesn't recognize. Verify
spelling by reading the value back via ``query_scalar``.

Deferred
========

* **Compile-time enum surface for booleans + well-known string
  PRAGMAs** (e.g. ``set_journal_mode(JournalMode.WAL)``). Adds typo
  protection at the cost of needing maintenance every time SQLite
  ships a new mode. Defer until the surface stabilizes.
* **Open-time PRAGMA bundle on** ``with_sqlite``
  (``with_sqlite(path, pragmas = (journal_mode = "WAL", ...))``)
  --- shaves a few lines per call site. Defer until a real consumer
  asks for it.

.. seealso::

    Full source: :download:`tutorials/sql/33-pragma.das <../../../../tutorials/sql/33-pragma.das>`

    Previous tutorial: :ref:`tutorial_sql_register_function`

    Next tutorial: :ref:`tutorial_sql_backup_vacuum`
