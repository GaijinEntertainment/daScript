.. _tutorial_sql_error_handling:

==================================
SQL-06 --- Error Handling
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; error handling
    single: Tutorial; Result
    single: Tutorial; Option
    single: Tutorial; try_open_sqlite
    single: Tutorial; try_insert
    single: Tutorial; query_scalar_opt
    single: Tutorial; _try_sql

dasSQLITE follows a uniform naming convention. Pick the variant by how
you want errors and "no row" to surface at the call site:

==============================  ==================================================================
Suffix                          Meaning
==============================  ==================================================================
``foo(...)``                    Strict; panics on error (programmer-error default)
``try_foo(...)``                ``Result<T, string>`` for write-side errors you want to handle
``foo_opt(...)``                ``Option<T>`` for read-side empty results (0 rows is a normal outcome)
==============================  ==================================================================

The same convention extends to the ``_sql`` macro: ``_sql(chain)`` is
strict, ``_try_sql(chain)`` is the non-panicking ``Result`` sibling.

Opening: ``try_open_sqlite``
============================

Use ``try_open_sqlite`` when the path is user-supplied or otherwise
unreliable. The strict ``open_sqlite`` / ``with_sqlite`` forms panic
on the same failure:

.. code-block:: das

    var open_result <- try_open_sqlite(":memory:")
    if (open_result |> is_err) {
        to_log(LOG_ERROR, "could not open db: {open_result |> unwrap_err}\n")
        return
    }
    var inscope db <- open_result._value

``SqlRunner`` finalizes on scope exit (closes the SQLite handle), so
it must be ``var inscope`` and bound by move (``<-``). Plain
``unwrap`` would fail because the type isn't copyable.

Writes: ``try_insert``
======================

``insert`` panics on a constraint violation. ``try_insert`` returns
``Result<int64, string>`` carrying the libsqlite3 error message:

.. code-block:: das

    let first = db |> try_insert(User(Id = 1, Name = "alice"))
    // first : Result<int64, string>;  rowid on success

    // Explicit collision --- strict insert() would abort here.
    let dup = db |> try_insert(User(Id = 1, Name = "alice-dup"))
    if (dup |> is_err) {
        // dup |> unwrap_err == "insert step failed: UNIQUE constraint failed: ..."
    }

Reads where 0 rows is fine: ``query_scalar_opt``
================================================

When 0 rows is a normal outcome (key not in table, lookup miss), the
``*_opt`` variant returns ``Option<T>``. Combine with ``??`` for a
default:

.. code-block:: das

    let lookup = db |> query_scalar_opt(
        "SELECT \"Name\" FROM \"Users\" WHERE \"Id\" = 42", type<string>)
    to_log(LOG_INFO, "user 42 is {lookup ?? "unknown"}\n")

``_try_sql``: ``_sql`` that doesn't panic
=========================================

``_try_sql`` is the non-panic sibling of ``_sql``. Same chain
analyzer, same emitted SQL --- the only difference is the runtime
helper used and the return type:

.. code-block:: das

    // _try_sql(... |> _first())     : Result<T, string>
    // _try_sql(... |> _first_opt()) : Result<Option<T>, string>
    // _try_sql(... |> count())      : Result<int64, string>

    let res = _try_sql(db |> select_from(type<User>) |> _first())
    if (res |> is_ok) {
        let u = res |> unwrap
    }

The ``_first`` vs ``_first_opt`` distinction still applies under
``_try_sql``: ``_first`` reports 0 rows as ``Err``; ``_first_opt``
reports them as ``ok(none)``. Pick the one that matches your domain.

Quick reference
===============

==============================================================  ==========================================================
Form                                                            When to use
==============================================================  ==========================================================
``open_sqlite(path)``                                           Strict open; panic on failure
``try_open_sqlite(path) : Result<SqlRunner, string>``           Open with handleable error
``insert(row)``                                                 Strict; panic on constraint violation
``try_insert(row) : Result<int64, string>``                     Returns rowid on success or libsqlite3 errmsg
``query_scalar(sql, type<T>)``                                  Strict; panic on 0 rows
``query_scalar_opt(sql, type<T>) : Option<T>``                  ``none`` on 0 rows; panic only on prepare/step error
``_sql(chain)``                                                 Strict ``_sql``; panic on prepare/step error
``_try_sql(chain) : Result<..., string>``                       Non-panic sibling; same SQL, wrapped result
==============================================================  ==========================================================

.. seealso::

    Full source: :download:`tutorials/sql/06-error_handling.das <../../../../tutorials/sql/06-error_handling.das>`

    Previous tutorial: :ref:`tutorial_sql_parametrized`

    Next tutorial: :ref:`tutorial_sql_anatomy`
