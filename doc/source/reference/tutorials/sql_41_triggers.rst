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
  :ref:`tutorial_sql_migrations` covers this
  pattern: each migration can ``exec`` a ``CREATE TRIGGER`` so
  the trigger ships with the schema version that needs it.

Audit-log pattern via raw ``exec``
==================================

A common use case: stamp ``UpdatedAt`` and write an audit-log
row whenever an ``Articles`` row changes. Two triggers, one for
INSERT and one for UPDATE:

.. das-doc: given [sql_table(name="Articles")] struct Article { @sql_primary_key Id : int; Title : string; UpdatedAt : int64 }
.. das-doc: given var inscope db = open_sqlite(":memory:")

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
which is why :ref:`tutorial_sql_migrations`
is the natural home for trigger DDL.

The typical anti-pattern
========================

"Trigger that calls back into application code." It can't.
SQLite triggers run inside the engine; there is no
``RAISE_EVENT`` that the daslang side observes. If you need a
side effect outside the DB, commit and queue the work in your
application code on the path that wrote the row --- don't try
to do it from a trigger.

Trigger cascades (gotcha)
=========================

``PRAGMA recursive_triggers`` (OFF by default) controls one thing
only: whether a trigger may re-enter **itself**. It does *not* stop
a trigger body from firing a **different** trigger on the same
table. The two triggers above cascade: the ``AFTER INSERT`` trigger
updates ``Articles`` to stamp ``UpdatedAt``, that nested UPDATE
fires ``articles_audit_update``, and every INSERT therefore lands
**two** audit rows --- the nested ``UPDATE`` row first, then the
``INSERT`` row.

Filter the cascade in the trigger itself. A ``WHEN`` clause that
only fires on a real content change makes each statement write
exactly one audit row, while ``UpdatedAt`` still gets stamped on
both paths:

.. code-block:: das

    db |> exec(
        "CREATE TRIGGER articles_audit_update
         AFTER UPDATE ON \"Articles\"
         WHEN OLD.\"Title\" IS NOT NEW.\"Title\"
         BEGIN
            UPDATE \"Articles\" SET \"UpdatedAt\" = strftime('%s', 'now')
                WHERE \"Id\" = NEW.\"Id\";
            INSERT INTO \"AuditLog\" (\"Op\", \"ArticleId\", \"AtUnix\")
                VALUES ('UPDATE', NEW.\"Id\", strftime('%s', 'now'));
         END")

The trigger's own stamping UPDATE does not re-fire it --- that
*is* self-recursion, which the default ``recursive_triggers = OFF``
suppresses. Turning the pragma ON removes that last guard too, so a
self-touching trigger body then needs its own ``WHEN`` filter.

.. seealso::

    Full source: :download:`tutorials/sql/41-triggers.das <../../../../tutorials/sql/41-triggers.das>`

    Previous tutorial: :ref:`tutorial_sql_fts5`

    Next tutorial: :ref:`tutorial_sql_schema_evolution`
