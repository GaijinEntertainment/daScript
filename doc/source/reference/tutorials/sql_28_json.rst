.. _tutorial_sql_json:

==========================================
SQL-28 --- JSON and BLOB columns
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; JSON
    single: Tutorial; @sql_json
    single: Tutorial; @sql_blob

Two complementary annotations let a struct or tuple value live in a single
column without leaving the typed-row world:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - Annotation
     - ``@sql_json``
     - ``@sql_blob``
   * - Storage
     - ``TEXT`` via ``daslib/json`` + ``daslib/json_boost``
     - ``BLOB`` via ``daslib/archive``
   * - Queryable from SQL
     - yes (``json_extract``)
     - no (opaque)
   * - Walker descent in ``_sql``
     - dotted path (pred and projection)
     - compile error
   * - Schema-drift tolerance
     - high --- missing fields default, extras ignored
     - low --- positional binary, adding a field breaks old rows
   * - Encode / decode speed
     - slower
     - faster
   * - Human-readable in ``sqlite3`` CLI
     - yes
     - no

Both ride the :ref:`tut-26 adapter rail <tutorial_sql_custom_types>`: when
``[sql_table]`` sees the annotation on a struct/tuple-typed field, it
generates the matching ``sql_bind`` / ``sql_extract`` pair at module
scope. No new storage class, no registration table.

Without an annotation, a struct/tuple-typed column is still a compile
error. "Is this column queryable JSON or opaque binary?" is a
load-bearing decision (schema-drift story + SQL queryability) ---
daslang refuses to guess.

@sql_json: TEXT-backed, queryable
=================================

.. code-block:: das

    [sql_table(name = "Users")]
    struct User {
        @sql_primary_key Id : int
        Name                : string
        @sql_json
        Prefs : tuple<theme : string; lang : string; notify : bool>
    }

The macro emits, at module scope:

.. code-block:: das

    def sql_bind    (v : tuple<...>) : string {
        return write_json(JV(v))
    }
    def sql_extract (v : string; t : type<tuple<...>>) : tuple<...> {
        var err = ""
        let jv = read_json(v, err)
        if (err != "") {
            panic("sql_extract: malformed JSON in column data: {err}")
        }
        return from_JV(jv, default<tuple<...>>)
    }

The DDL emits ``"Prefs" TEXT NOT NULL`` --- the ``@sql_json`` short-circuit
bypasses ``sql_storage_type_for(type<T>)`` (which would fail for tuple
or struct ``T`` anyway).

JSON-path descent in ``_sql``
=============================

``_.<json_col>.<path…>`` on a ``@sql_json`` field lowers to
``json_extract("<col>", '$.<path…>')``, both in WHERE clauses and in
SELECT projections. The leaf type drives result-side adapter dispatch
(``string`` reads back as string, ``bool`` as bool, etc.).

.. code-block:: das

    // WHERE descent
    let dark_users <- _sql(db |> select_from(type<User>)
        |> _where(_.Prefs.theme == "dark"))
    //   SELECT Id, Name, Prefs FROM "Users"
    //   WHERE json_extract("Prefs", '$.theme') = ?

    // SELECT projection descent
    let themes <- _sql(db |> select_from(type<User>)
        |> _select(_.Prefs.theme))
    //   SELECT json_extract("Prefs", '$.theme') FROM "Users"

    // Mixed projection (plain column + JSON path)
    let mixed <- _sql(db |> select_from(type<User>)
        |> _select((Name = _.Name, Theme = _.Prefs.theme)))
    //   SELECT "Name", json_extract("Prefs", '$.theme') FROM "Users"

Descent is arbitrary depth --- nested struct paths concatenate dotted:

.. code-block:: das

    struct Address { City : string; Zip : string }
    struct Profile { Addr : Address; Bio : string }

    [sql_table(name = "Accounts")]
    struct Account {
        @sql_primary_key Id : int
        @sql_json Profile   : Profile
    }

    let in_ny <- _sql(db |> select_from(type<Account>)
        |> _where(_.Profile.Addr.City == "NY"))
    //   SELECT Id, Profile FROM "Accounts"
    //   WHERE json_extract("Profile", '$.Addr.City') = ?

@sql_blob: opaque binary archive
================================

.. code-block:: das

    struct SessionSnapshot {
        last_scene : string
        unlocks : array<int>
        last_tick_ms : int64
    }

    [sql_table(name = "Sessions")]
    struct SessionRow {
        @sql_primary_key UserId : int
        StartedAt : int64
        @sql_blob State : SessionSnapshot
    }

The macro emits the archive variant of the adapter pair. The parameter
is ``const`` because the catch-all binder passes ``v`` non-\ ``var``;
``mem_archive_save`` / ``mem_archive_load`` need a mutable reference, so
the body clones into a local through ``clone_to_move``:

.. code-block:: das

    def sql_bind    (v : T) : array<uint8> {
        var v_local <- clone_to_move(v)
        return <- mem_archive_save(v_local)
    }
    def sql_extract (v : array<uint8>; t : type<T>) : T {
        var r = default<T>
        var v_local <- clone_to_move(v)
        mem_archive_load(v_local, r)        // panics on malformed bytes
        return <- r
    }

Storage is ``BLOB``. ``State.last_scene == "intro"`` inside ``_sql`` is a
**compile error** --- the column is opaque to the walker. Filter in
daslang after round-trip, or use raw ``query`` SQL for one-offs.

Choosing between them
=====================

Rule of thumb:

* **Shape will evolve** --- ``@sql_json``. Missing fields default,
  extras ignored, future additions are a no-op for old rows.
* **Shape is pinned / versioned** --- ``@sql_blob`` is fine.
  Smaller on disk, faster encode/decode.
* **Need SQL-side filter inside the column** --- ``@sql_json`` is
  the only option. ``@sql_blob`` cannot be inspected from SQL.

Both forms panic on bad stored data --- malformed JSON trips
``read_json`` (the generated ``sql_extract`` checks the parser error
and panics), corrupt archive bytes trip ``mem_archive_load``.
``try_select_from`` / ``try_query`` only catch sqlite3-level prepare
and step errors; they do NOT convert adapter panics to ``Err``.
Defensive reads should validate JSON shape externally (a CHECK
constraint, a ``json_valid`` filter in the SELECT) before the row
reaches daslang.

Cross-provider portability
==========================

The walker rule is universal --- "field descent on an ``@sql_json``
column lowers to a JSON-path extract." The per-provider spelling is the
swap-out point:

* SQLite: ``json_extract(col, '$.path')``
* PostgreSQL: ``col->>'path'`` (or ``->`` for JSONB)
* MySQL: ``col->>'$.path'``
* MSSQL: ``JSON_VALUE(col, '$.path')``

dasSQLITE emits SQLite syntax in v1; the same daslang-side query
translates to the other backends when their boost modules ship.

Deferred
========

* ``@sql_json_index("$.path")`` --- typed expression index. Cross-provider
  DDL diverges; raw ``CREATE INDEX`` until a second backend lands.
* ``_json_extract<T>(col, path)`` --- explicit escape for paths the
  walker can't reach (array subscripts, custom JSON operators).
* ``@sql_json(fast)`` --- ``json_sprint`` / ``json_sscan`` fast path.
* JSON patch / merge operators inside ``_update`` chains.
* JSONB binary storage (SQLite 3.45+) via ``@sql_json(binary)``.
* ``@sql_blob`` compression (zstd / lz4) and versioned-format headers.

.. seealso::

    Full source: :download:`tutorials/sql/28-json.das <../../../../tutorials/sql/28-json.das>`

    Previous tutorial: :ref:`tutorial_sql_blob`

    Next tutorial: :ref:`tutorial_sql_column_names`
