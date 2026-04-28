.. _tutorial_sql_blob:

==========================================
SQL-27 --- BLOB round-trip
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; BLOB
    single: Tutorial; binary data
    single: Tutorial; array<uint8>

``array<uint8>`` is one of the four primitive storage types in the
:ref:`tut-26 adapter rail <tutorial_sql_custom_types>`, so a
``[sql_table]`` field of that type round-trips through a ``BLOB``
column with no extra annotation:

.. code-block:: das

    [sql_table(name = "Images")]
    struct Image {
        @sql_primary_key Id : int
        Name : string
        @safe_when_uninitialized Data : array<uint8>
    }

The ``[sql_table]`` macro:

* Emits the ``BLOB`` column type in the DDL.
* Routes ``INSERT`` through ``sqlite3_bind_blob`` with
  ``SQLITE_TRANSIENT`` (SQLite copies into its own buffer).
* Materializes a fresh ``array<uint8>`` per row on ``SELECT``,
  copying from the column pointer (which is only valid until the
  next ``step`` / ``reset``).

Three tradeoffs to keep in mind
===============================

* **Two copies on insert.** ``SQLITE_TRANSIENT`` makes SQLite's own
  copy after the bind call. For multi-MB asset blobs, prefer
  SQLite's ``sqlite3_blob_open`` streaming API or chunked storage.
* **One copy on read.** Each row materializes a fresh
  ``array<uint8>`` --- the column pointer can't escape the cursor
  position. Streaming reads need ``sqlite3_blob_read`` directly.
* **Nullable BLOB columns** use ``Option<array<uint8>>``, the same
  pattern as nullable scalars in :ref:`tut 18 <tutorial_sql_null_handling>`.

End-to-end example
==================

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost
    require sqlite/sqlite_linq

    [sql_table(name = "Images")]
    struct Image {
        @sql_primary_key Id : int
        Name : string
        @safe_when_uninitialized Data : array<uint8>
    }

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            db |> create_table(type<Image>)

            var hello : array<uint8>
            hello |> resize(11)
            for (i in range(11)) {
                hello[i] = uint8(0x40 + i)
            }
            db |> insert(Image(Id = 1, Name = "hello", Data <- hello))

            // SELECT: read_via_adapter routes through the BLOB
            // primitive case, allocates a fresh array per row.
            let images <- _sql(db |> select_from(type<Image>))
            for (img in images) {
                to_log(LOG_INFO, "{img.Name} ({length(img.Data)} bytes)\n")
            }

            // query_scalar over a BLOB function (LENGTH() returns int64).
            let total = db |> query_scalar(
                "SELECT SUM(LENGTH(Data)) FROM Images", type<int64>)
            to_log(LOG_INFO, "total bytes: {total}\n")
        }
    }

Empty BLOB vs NULL
==================

An empty ``array<uint8>`` (length 0) binds as a zero-length BLOB ---
distinct from NULL. SQLite's ``LENGTH()`` on an empty BLOB returns 0;
``LENGTH()`` on NULL returns NULL. Use ``Option<array<uint8>>`` to
represent the absent-value case.

Internally the macro guards length-zero arrays and emits
``sqlite3_bind_zeroblob(stmt, idx, 0)`` to avoid a panic on
``addr(data[0])`` for empty arrays. The result is observationally
identical to a non-zero-length empty BLOB.

Future: streaming BLOB I/O
==========================

SQLite offers ``sqlite3_blob_open`` / ``sqlite3_blob_read`` /
``sqlite3_blob_write`` for incremental access without materializing
the whole blob in daslang's heap. Likely shape:

.. code-block:: das

    db |> open_blob(type<Image>, rowid, "Data") <| $(blob) {
        var buf : array<uint8>
        buf |> resize(4096)
        blob |> read(offset, buf)
    }

EF Core doesn't expose this --- game asset and large-binary use cases
typically live outside the ORM in dedicated APIs. dasSQLITE keeps the
streaming path on the deferred list until a real workload demands it.

.. seealso::

    Full source: :download:`tutorials/sql/27-blob.das <../../../../tutorials/sql/27-blob.das>`

    Previous tutorial: :ref:`tutorial_sql_custom_types`
