.. _tutorial_sql_custom_types:

==========================================
SQL-26 --- Custom type adapters
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; sql_bind
    single: Tutorial; sql_extract
    single: Tutorial; custom type
    single: Tutorial; type adapter
    single: Tutorial; DateTime
    single: Tutorial; Guid
    single: Tutorial; enum

Convention-based type adapters: every SQL-addressable user type ``T``
defines a bidirectional pair of named functions, and the
``[sql_table]`` macro picks them up automatically. No registration
step.

The two-function pair
=====================

.. code-block:: das

    def sql_bind    (v : T)            : P              // T -> primitive
    def sql_extract (v : P; t : type<T>) : T            // primitive -> T

``P`` must be one of the four SQLite storage primitives:

==================   ====================
Return type          SQLite column type
==================   ====================
``int64``            ``INTEGER``
``double``           ``REAL``
``string``           ``TEXT``
``array<uint8>``     ``BLOB``
==================   ====================

The **return type of** ``sql_bind`` **is the storage type**. The
``[sql_table]`` macro reads it via ``typedecl`` at expansion time and
emits the matching ``sqlite3_bind_*`` / ``sqlite3_column_*`` calls,
plus the matching DDL column type.

NULL is handled orthogonally by ``Option<T>`` from tut 18; this rail
covers four storage forms, not five.

How dispatch works
==================

The ``[sql_table]`` macro emits ``_::sql_bind(...)`` and
``_::sql_extract(...)``. ``_::`` is "calling-module name lookup", so
every ``sql_bind`` overload in scope at the call site --- including
the user's type-specific pair --- participates in overload
resolution. Same mechanism as ``_::clone`` and ``_::finalize``.

Built-in adapters ship in ``sqlite_boost`` for:

* The four primitives (passthrough).
* Stdlib widenings: ``int`` / ``int8`` / ``int16`` / ``uint`` /
  ``uint8`` / ``uint16`` / ``uint64`` round-trip through ``int64``;
  ``float`` round-trips through ``double``; ``bool`` round-trips
  through ``int64`` (``true`` -> ``1``).
* A single enum generic ``def sql_bind $T (e : T) : int64 where T :
  enum`` (and the matching ``sql_extract``) so any enum auto-
  round-trips through ``INTEGER``.

User code only writes adapters for domain types.

Example: DateTime as INTEGER
============================

.. code-block:: das

    struct DateTime {
        unix_seconds : int64
    }

    def sql_bind(dt : DateTime) : int64 {
        return dt.unix_seconds
    }

    [unused_argument(t)]
    def sql_extract(v : int64; t : type<DateTime>) : DateTime {
        return DateTime(unix_seconds = v)
    }

The ``[unused_argument(t)]`` annotation is there because the
``type<DateTime>`` parameter is a compile-time tag for overload
discrimination only; the function body never reads it.

Storage choice rationale: integer is indexable, compact, and
math-friendly. For ISO8601 text storage instead, write a pair whose
``sql_bind`` returns ``string`` --- one storage form per type at
module level (``@sql_as(type<P>)`` per-field override is deferred).

Example: Guid as BLOB
=====================

Same pattern, different primitive. ``array<uint8>`` selects the
``BLOB`` column type:

.. code-block:: das

    struct Guid {
        @safe_when_uninitialized bytes : array<uint8>
    }

    def sql_bind(g : Guid) : array<uint8> {
        var copy : array<uint8>
        copy := g.bytes
        return <- copy
    }

    [unused_argument(t)]
    def sql_extract(var v : array<uint8>; t : type<Guid>) : Guid {
        return Guid(bytes <- v)
    }

For multi-MB asset blobs, prefer SQLite's ``sqlite3_blob_open``
streaming API over an adapter that copies on every bind.

Schema using all three custom types
===================================

.. code-block:: das

    enum OrderStatus {
        Pending
        Paid
        Shipped
        Cancelled
    }

    [sql_table(name = "Orders")]
    struct Order {
        @sql_primary_key Id : int
        ExternalId : Guid           // BLOB column
        PlacedAt   : DateTime       // INTEGER column
        Status     : OrderStatus    // INTEGER column (enum generic)
        Total      : float          // REAL column (stdlib widening)
    }

Emitted DDL:

.. code-block:: sql

    CREATE TABLE "Orders"(
        "Id"         INTEGER PRIMARY KEY,
        "ExternalId" BLOB    NOT NULL,
        "PlacedAt"   INTEGER NOT NULL,
        "Status"     INTEGER NOT NULL,
        "Total"      REAL    NOT NULL
    )

``Option<T>`` composes automatically
====================================

``Option<DateTime>`` works as long as ``DateTime`` has the adapter
pair. The macro unwraps the ``Option`` at runtime: ``some(dt)`` binds
through ``sql_bind(dt)``; ``none()`` binds NULL. Read-side, ``NULL``
becomes ``none()`` and a present value is decoded through
``sql_extract``.

.. code-block:: das

    [sql_table(name = "Events")]
    struct Event {
        @sql_primary_key Id : int
        At : DateTime
        @safe_when_uninitialized StartsAt : Option<DateTime>
    }

Missing-adapter compile error
=============================

If a ``[sql_table]`` field has no ``sql_bind`` / ``sql_extract`` pair
in scope, overload resolution fails at the macro-emitted ``_::sql_bind``
call:

.. code-block:: das

    struct Color { r, g, b : float }

    [sql_table(name = "Styles")]
    struct Style {
        @sql_primary_key Id : int
        Bg : Color           // no sql_bind(Color) - compile error
    }

Compiler message names the offending struct + field type. No runtime
"type not registered" error --- this is all compile-time.

.. seealso::

    Full source: :download:`tutorials/sql/26-custom_types.das <../../../../tutorials/sql/26-custom_types.das>`

    Previous tutorial: :ref:`tutorial_sql_defaults_computed`

    Next tutorial: :ref:`tutorial_sql_blob`
