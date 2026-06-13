.. _tutorial_toml:

========================
TOML
========================

.. index::
    single: Tutorial; TOML
    single: Tutorial; TOML Parsing
    single: Tutorial; read_toml
    single: Tutorial; toml

This tutorial covers ``daslib/toml`` — parsing `TOML 1.0
<https://toml.io/en/v1.0.0>`_ configuration files in daslang.

``read_toml`` decodes TOML into the **same** ``JsonValue?`` tree that
``daslib/json`` produces, so every ``json_boost`` accessor (``?.``, ``?[]``,
``??``, ``from_JV``) works on a parsed TOML document with no extra code.

Parsing
=======

``read_toml`` takes a string and an out error string, and returns
``JsonValue?``. A TOML document is always a table, so a successful parse is
always an ``_object``. On malformed input it returns ``null`` and fills the
error — the same fail-fast contract as ``read_json``::

  var error : string
  var doc = read_toml("title = \"daslang\"
  version = 3", error)
  if (doc != null) {
      print("title: {doc?.title ?? "?"}\n")   // daslang
      print("version: {doc?.version ?? 0}\n") // 3
  }

Type mapping
============

Each TOML value maps to one ``JsValue`` case:

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - TOML
     - ``JsValue``
   * - table / inline table
     - ``_object`` (``table<string; JsonValue?>``)
   * - array / array-of-tables
     - ``_array`` (``array<JsonValue?>``)
   * - string (all four forms, already unescaped)
     - ``_string``
   * - integer (decimal / hex / octal / binary)
     - ``_longint`` (``int64``)
   * - float (incl. ``inf`` / ``nan``)
     - ``_number`` (``double``)
   * - boolean
     - ``_bool``
   * - date-time / local-date / local-time
     - ``_string`` (RFC-3339 lexical form preserved)

JSON has no native date type, so TOML date-times are kept verbatim as
strings — you get back the exact text the document used.

Nested tables and dotted keys
=============================

``[section]`` headers and dotted keys both build nested ``_object`` tables.
Reach into them with chained ``?.`` or ``?[]``, exactly like JSON::

  var doc = read_toml("[server]
  host = \"localhost\"
  port = 8080

  [server.tls]
  enabled = true", error)

  print(doc?.server?.host ?? "?")          // localhost
  print(doc?["server"]?["port"] ?? 0)      // 8080
  print(doc?.server?.tls?.enabled ?? false)// true
  print(doc?.server?.timeout ?? -1)        // -1  (missing path -> default)

Arrays and arrays-of-tables
===========================

A bare array becomes an ``_array`` of scalars. A repeated ``[[name]]``
header builds an ``_array`` of ``_object`` tables — one element per block::

  var doc = read_toml("ports = [8000, 8001, 8002]

  [[user]]
  name = \"alice\"

  [[user]]
  name = \"bob\"", error)

  print(doc?.ports?[1] ?? 0)              // 8001
  print(doc?.user?[0]?.name ?? "?")       // alice

  // Walk the array of tables
  let users = doc?.user
  if (users != null && users is _array) {
      for (u in users as _array) {
          print("{u?.name ?? "?"}\n")
      }
  }

Inline tables
=============

``{ k = v, ... }`` on one line is an inline table — also an ``_object``::

  var doc = read_toml("point = \{ x = 1, y = 2 \}", error)
  print(doc?.point?.x ?? 0)   // 1

Integers and floats
===================

TOML accepts decimal, ``0x`` hex, ``0o`` octal and ``0b`` binary integers
(plus ``_`` digit separators), and floats with exponents, ``inf`` and
``nan``. All integers decode to ``_longint``, all floats to ``_number``::

  var doc = read_toml("hex = 0xFF
  oct = 0o755
  big = 6.022e23
  huge = inf", error)

  print(doc?.hex ?? 0)   // 255
  print(doc?.oct ?? 0)   // 493

Error handling
==============

``read_toml`` is fail-fast — malformed input returns ``null`` with a
descriptive message in the error out-param::

  var doc = read_toml("key = = broken", error)
  print(doc == null)        // true
  print(!empty(error))      // true

Deserializing into a struct
===========================

Because the result is the same tree as JSON, ``from_JV`` fills a struct by
compile-time reflection — turning a config file into typed data::

  struct ServerConfig {
      host : string
      port : int
      workers : int
  }

  var doc = read_toml("host = \"0.0.0.0\"
  port = 9090
  workers = 4", error)
  let cfg = from_JV(doc, type<ServerConfig>)
  // cfg.host = "0.0.0.0", cfg.port = 9090, cfg.workers = 4

.. seealso::

   Full source: :download:`tutorials/language/57_toml.das <../../../../tutorials/language/57_toml.das>`

   Previous tutorial: :ref:`LINQ query syntax <tutorial_linq_query>`.

   Next tutorial: :ref:`Logger <tutorial_logger>`.

   :ref:`JSON tutorial <tutorial_json>` — the ``JsonValue?`` tree and ``json_boost`` accessors used here.

   :doc:`/stdlib/generated/toml` — TOML module reference.
