.. _tutorial_sql_triggers:

============================================
SQL-41 --- Triggers: DB-level callbacks
============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; trigger
    single: Tutorial; audit log

Concept-only tutorial --- no new API. SQLite triggers are SQL
statements that fire automatically before/after INSERT, UPDATE,
or DELETE on a table. dasSQLITE deliberately does **not** add a
daslang-side trigger DSL. Reasons:

* **Triggers run inside the SQL engine.** There is no daslang
  callback, no row marshalling, no daslang-side type checking.
  A typed wrapper would be a thin facade over
  ``exec("CREATE TRIGGER ...")`` --- extra surface, no extra
  safety.
* **Trigger bodies are SQL fragments.** Cross-provider
  semantics diverge sharply (PostgreSQL trigger functions,
  MySQL ``BEFORE`` vs ``AFTER`` reloading, MSSQL
  ``inserted``/``deleted`` pseudo-tables). A portable
  abstraction would lose the parts people actually use
  triggers for.
* **Schema migrations are the right home for trigger DDL** ---
  ``tut 32 (migrations, shipping in a later chunk)`` covers this
  pattern: each migration can ``exec`` a ``CREATE TRIGGER`` so
  the trigger ships with the schema version that needs it.

Audit-log pattern via raw ``exec``
==================================

A common use case: stamp ``UpdatedAt`` and write an audit-log
row whenever an ``Articles`` row changes. Two triggers, one for
INSERT and one for UPDATE:

.. code-block:: das

    db |> exec(
        "CREATE TRIGGER articles_audit_insert
         AFTER INSERT ON \"Articles\"
         BEGIN
            UPDATE \"Articles\" SET \"UpdatedAt\" = strftime('%s', 'now')
                WHERE \"Id\" = NEW.\"Id\";
            INSERT INTO \"AuditLog\" (\"Op\", \"ArticleId\", \"AtUnix\")
                VALUES ('INSERT', NEW.\"Id\", strftime('%s', 'now'));
         END")

    db |> exec(
        "CREATE TRIGGER articles_audit_update
         AFTER UPDATE ON \"Articles\"
         BEGIN
            UPDATE \"Articles\" SET \"UpdatedAt\" = strftime('%s', 'now')
                WHERE \"Id\" = NEW.\"Id\";
            INSERT INTO \"AuditLog\" (\"Op\", \"ArticleId\", \"AtUnix\")
                VALUES ('UPDATE', NEW.\"Id\", strftime('%s', 'now'));
         END")

``OLD`` and ``NEW`` are SQLite's row-shape pseudonyms (no
quoting --- they're SQL keywords). Once installed, the triggers
fire transparently when daslang code hits the table:

.. code-block:: das

    db |> insert(Article(Id = 1, Title = "Hello", UpdatedAt = 0l))
    db |> _sql_update(type<Article>, _.Id == 1, (Title = "Hello (revised)"))
    // both calls fired the audit triggers; daslang never saw them.

Drop / replace
==============

.. code-block:: das

    db |> exec("DROP TRIGGER IF EXISTS articles_audit_insert")

Replace = drop + recreate. The lifecycle is migration-shaped,
which is why migrations (``tut 32``, shipping in a later chunk)
is the natural home for trigger DDL.

The typical anti-pattern
========================

"Trigger that calls back into application code." It can't.
SQLite triggers run inside the engine; there is no
``RAISE_EVENT`` that the daslang side observes. If you need a
side effect outside the DB, commit and queue the work in your
application code on the path that wrote the row --- don't try
to do it from a trigger.

Trigger recursion (gotcha)
==========================

SQLite's default is ``PRAGMA recursive_triggers = OFF`` --- a write
performed inside a trigger body does **not** fire other triggers on
the same table. The audit-log example above relies on that default:
the ``AFTER INSERT`` trigger updates ``Articles`` (to stamp
``UpdatedAt``), and that nested update is intentionally swallowed
so the ``AFTER UPDATE`` trigger does not also write a row.

If you opt into ``PRAGMA recursive_triggers = ON`` you must design
the trigger body so it doesn't re-touch the source table, or filter
the recursive case explicitly --- otherwise the audit log doubles
(or loops) on every write.

.. seealso::

    Full source: :download:`tutorials/sql/41-triggers.das <../../../../tutorials/sql/41-triggers.das>`

    Previous tutorial: :ref:`tutorial_sql_fts5`
