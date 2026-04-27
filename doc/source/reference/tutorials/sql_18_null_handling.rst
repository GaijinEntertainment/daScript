.. _tutorial_sql_null_handling:

==================================================
SQL-18 --- NULL Handling: ``Option<T>`` Everywhere
==================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; NULL
    single: Tutorial; Option
    single: Tutorial; is_some
    single: Tutorial; is_none
    single: Tutorial; unwrap_or
    single: Tutorial; COALESCE
    single: Tutorial; IS NULL

Nullability is **type-driven**. The struct field's daslang type is
the single source of truth --- there is no ``@sql_nullable``
annotation. The same ``[sql_table]`` struct doubles as the row shape
and the schema.

==============================================  ==================================================
Field type                                      DDL emitted
==============================================  ==================================================
``T``                                           ``COLUMN TYPE NOT NULL``
``Option<T>``                                   ``COLUMN TYPE`` (no ``NOT NULL``)
==============================================  ==================================================

The ``[sql_table]`` macro inspects each field at compile time and
emits the matching DDL, INSERT/UPDATE binds, and SELECT readers.

DDL example
===========

.. code-block:: das

    [sql_table(name="Users")]
    struct User {
        @sql_primary_key Id : int
        Name      : string
        @safe_when_uninitialized Age       : Option<int>
        @safe_when_uninitialized Nick      : Option<string>
        @safe_when_uninitialized DeletedAt : Option<int64>
    }

Generated DDL:

.. code-block:: sql

    CREATE TABLE "Users"(
        "Id" INTEGER PRIMARY KEY,
        "Name" TEXT NOT NULL,
        "Age" INTEGER,
        "Nick" TEXT,
        "DeletedAt" INTEGER
    )

The ``@safe_when_uninitialized`` annotation on each ``Option<T>``
field is required under ``strict_smart_pointers`` so the macro can
default-initialize a row in the row-builder. The Option's internal
``_value`` is already so-marked inside the template; the wrapping
struct field still needs its own annotation.

Round-trip --- ``some(v)`` and ``none()``
=========================================

INSERT/UPDATE bind code branches per field type at compile time.
``is_some(field)`` binds the underlying value; ``is_none()`` binds
``sqlite3_bind_null``. SELECT readers check
``sqlite3_column_type == SQLITE_NULL`` and wrap accordingly.

.. code-block:: das

    db |> insert(User(
        Id = 1, Name = "alice",
        Age = some(30), Nick = none(type<string>), DeletedAt = none(type<int64>)))

    let everyone <- db |> select_from(type<User>)
    for (u in everyone) {
        if (u.Age |> is_some) {
            to_log(LOG_INFO, "{u.Name}: age {u.Age |> unwrap}\n")
        } else {
            to_log(LOG_INFO, "{u.Name}: age unknown\n")
        }
    }

NULL predicates in ``_where``
=============================

Three Option methods translate to SQL null operators:

==============================================  ==================================================
Source shape                                    Emitted SQL
==============================================  ==================================================
``_.Col |> is_some``                            ``"Col" IS NOT NULL``
``_.Col |> is_none``                            ``"Col" IS NULL``
``_.Col |> unwrap_or(default)``                 ``COALESCE("Col", ?)``  (``default`` binds)
==============================================  ==================================================

``is_some`` / ``is_none``:

.. code-block:: das

    let known_age <- _sql(db |> select_from(type<User>)
                            |> _where(_.Age |> is_some))
    // ... WHERE "Age" IS NOT NULL

    let alive <- _sql(db |> select_from(type<User>)
                        |> _where(_.DeletedAt |> is_none))
    // ... WHERE "DeletedAt" IS NULL

``unwrap_or`` --- COALESCE with a fallback:

.. code-block:: das

    let cutoff = 18
    let adults_or_unknown <- _sql(db |> select_from(type<User>)
                                    |> _where(_.Age |> unwrap_or(0) >= cutoff))
    // ... WHERE COALESCE("Age", ?) >= ?     binds: [0, 18]

The ``unwrap_or`` default (``0`` here) routes through the predicate
rewriter --- both captured locals and literals are emitted as ``?``
bind parameters (the analyzer doesn't bother distinguishing; keeping
the SQL parameterized either way is the safe-by-default behavior;
see :ref:`tutorial_sql_anatomy`).

Option<T> in projections
========================

Full-row projection (no ``_select``) preserves each field's exact
type --- ``Option<T>`` stays ``Option<T>`` in the result struct.
There is no implicit unwrap. Users see the same nullability
discipline at projection sites as at row sites.

Three-valued-logic gotcha
=========================

In SQL, ``NULL = NULL`` is ``NULL``, never ``TRUE``. ``NULL <> x``
is also ``NULL``. ``WHERE`` treats ``NULL``-valued predicates as
false, so ``WHERE Col = NULL`` matches **nothing**. The Option API
steers users away from that footgun: use ``_.Col |> is_none()``
(emits ``IS NULL``) or ``_.Col |> unwrap_or(d) == x`` (emits
``COALESCE`` then compare).

Direct ``_.Col == none()`` in a predicate is intentionally not
translated this chunk. A future revision may either lower it to
``IS NULL`` automatically or raise a ``macro_error`` with a fix-it
pointing to ``|> is_none()`` --- the leaning is toward the explicit
diagnostic so the user has to confront three-valued logic head-on.

``_try_sql`` composes
=====================

The non-panicking ``_try_sql`` form composes with all of the above
--- the Option methods are predicate-translation rules, orthogonal
to the outer success/failure wrapper:

.. code-block:: das

    let attempt <- _try_sql(db |> select_from(type<User>)
                              |> _where(_.Age |> is_some))
    if (attempt |> is_ok) {
        let rows <- attempt |> unwrap
        // ...
    }

.. seealso::

    Full source: :download:`tutorials/sql/18-null_handling.das <../../../../tutorials/sql/18-null_handling.das>`

    Previous tutorial: :ref:`tutorial_sql_subqueries`
