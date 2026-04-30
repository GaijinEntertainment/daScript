.. _tutorial_sql_attach:

================================================
SQL-36 --- ``ATTACH DATABASE``: cross-DB queries
================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; ATTACH
    single: Tutorial; with_schema

SQLite-only. ``ATTACH`` lets a single connection see multiple
database files at once, each addressed by a schema qualifier.
Common cases: per-tenant DBs (``users/<id>.db``) attached at
request time; hot data in ``app.db`` and historical data in
``archive.db``; import / export flows that copy rows server-side
without round-tripping through CSV.

The dasSQLITE surface lives in ``sqlite/sqlite_boost``:

* ``attach`` / ``try_attach`` --- runtime ``ATTACH DATABASE``;
* ``detach`` / ``try_detach`` --- runtime ``DETACH DATABASE``;
* ``with_schema(db, name) $(scoped) { ... }`` --- block-form
  that invokes the body with a runner emitting
  ``"<name>"."<table>"`` on every query. Block-scoped so the
  qualified runner cannot outlive its lexical scope (it shares
  ``db``'s libsqlite3 handle; only the original owns lifetime).
* ``with_attached(db, path, as_name) $(scoped) { ... }`` ---
  attach + invoke + detach in ``finally`` (covers panic paths).

``:memory:`` is a valid attach target: each ``:memory:`` attach
creates a fresh, separate in-memory DB. The runnable example
uses that so it has no external file dependencies.

End-to-end
==========

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost
    require sqlite/sqlite_linq

    [sql_table(name = "Users")]
    struct User {
        @sql_primary_key Id : int
        Name                : string
        Active              : bool
    }

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            db |> create_table(type<User>)
            db |> insert(User(Id = 1, Name = "Alice", Active = true))

            // Manual attach + with_schema -------------------------
            db |> attach(":memory:", "archive")
            db |> with_schema("archive") $(arch) {
                arch |> create_table(type<User>)
                arch |> insert(User(Id = 100, Name = "Carol", Active = false))

                for (u in _sql(arch |> select_from(type<User>)
                                 |> _where(!_.Active))) {
                    to_log(LOG_INFO, "archived: {u.Name}")
                }
            }
            db |> detach("archive")

            // with_attached block (one-shot attach + with_schema) -
            db |> with_attached(":memory:", "tenant_42") $(tenant) {
                tenant |> create_table(type<User>)
                tenant |> insert(User(Id = 1, Name = "Tenant42", Active = true))
            }   // auto-detach, even on panic
        }
    }

How ``with_schema`` qualifies SQL
=================================

``with_schema(db, name) $(scoped) { ... }`` invokes ``scoped``
with a runner whose ``schema_name`` field is set --- no DB
round-trip. Every SQL string the macro emits passes through a
runtime rewriter that prepends ``"<name>".`` to every table
reference, including DDL:

* ``FROM "Users"`` --> ``FROM "archive"."Users"``
* ``INTO "Users"`` --> ``INTO "archive"."Users"``
* ``UPDATE "Users"`` --> ``UPDATE "archive"."Users"``
* ``JOIN "Users"`` --> ``JOIN "archive"."Users"``
* ``CREATE TABLE "Users"`` --> ``CREATE TABLE "archive"."Users"``
* ``CREATE INDEX "ix" ON "Users"`` --> ``CREATE INDEX "archive"."ix" ON "Users"``
  (per SQLite syntax the schema qualifies the index name; the table on the
  right of ``ON`` resolves within the same schema)
* ``DROP TABLE IF EXISTS "Users"`` --> ``DROP TABLE IF EXISTS "archive"."Users"``

This means the entire typed CRUD surface (``create_table``,
``insert``, ``_sql_update``, ``_sql_delete``, ``_sql``,
``_each_sql``) works against an attached schema with no
per-call argument changes --- the schema is carried on the
runner.

Cross-schema queries (raw SQL)
==============================

v1 ships **one schema per** ``_sql`` **chain**. Cross-schema
``UNION`` / ``JOIN`` / ``INSERT ... SELECT`` flows drop to raw
``query`` / ``exec`` with explicitly-qualified table names.
This is a deferred typed-API item; raw SQL covers v1.

.. code-block:: das

    let merged <- db |> query(
        "SELECT \"Id\", \"Name\", \"Active\" FROM \"Users\"
         UNION
         SELECT \"Id\", \"Name\", \"Active\" FROM \"archive\".\"Users\"
         ORDER BY \"Id\"",
        type<User>)

Failure modes
=============

* ``try_attach(db, path, "main")`` --- ``main`` and ``temp`` are
  reserved schema names; both surface as ``some(errmsg)``.
* ``try_attach(db, "missing.db", "x")`` --- nonexistent file
  surfaces as ``some(errmsg)`` containing ``"unable to open
  database file"``.
* ``with_schema(db, "nonexistent") $(...)`` --- no validation
  at the call site. The error surfaces on the **first** query
  against the runner inside the block: ``"no such table:
  nonexistent.Users"``. Attach first, then ``with_schema``.
* SQLite defaults to a max of 10 attached DBs per connection
  (``SQLITE_MAX_ATTACHED``, compile-time). Batch flows that
  attach dozens should detach aggressively.

When NOT to use ATTACH
======================

* **Process-level fan-out.** A new SqlRunner per worker thread
  is the right unit of parallelism (see :ref:`tut 38
  <tutorial_sql_concurrency>`); ATTACH is not a substitute.
  Attachments are connection-bound, so a worker thread that
  opens its own ``with_sqlite`` does **not** see the parent's
  attachments --- it must re-attach.
* **Cross-DB transactions in legacy journal mode.** WAL
  (set by ``apply_recommended_pragmas``) handles cross-DB
  atomic commits transparently. Without WAL, each attached DB
  has its own journal file and atomicity becomes journal-mode
  dependent.

.. seealso::

    Full source: :download:`tutorials/sql/36-attach.das <../../../../tutorials/sql/36-attach.das>`

    Previous tutorial: :ref:`tutorial_sql_streaming`
