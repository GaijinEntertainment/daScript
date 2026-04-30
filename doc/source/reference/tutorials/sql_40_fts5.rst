.. _tutorial_sql_fts5:

================================================
SQL-40 --- FTS5 full-text search
================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; FTS5
    single: Tutorial; full-text search
    single: Tutorial; text_match

SQLite-only. The portable string predicates (``starts_with``,
``ends_with``, ``contains``) emit ``LIKE ? ESCAPE '\'`` patterns
and work on any backend. They're fine for short columns, but
``LIKE '%x%'`` can't use a B-tree index --- full scan at scale
is painful. FTS5 is the specialized tool: a virtual table that
builds an inverted index over text columns and exposes
relevance ranking + Boolean / phrase / prefix / NEAR query
operators.

API surface added in this chunk
===============================

* ``[sql_fts5(name = "...")]`` --- struct annotation: the type
  is materialized as a SQLite FTS5 virtual table.
* ``@sql_fts_rank`` --- read-only ``float`` column annotation:
  maps to FTS5's hidden ``rank`` column on SELECT, skipped on
  INSERT.
* ``text_match(col, query)`` --- usable inside ``_where``;
  emits ``col MATCH ?``. Compile error on a non-FTS5 column,
  with a fix-it pointing at ``contains`` or ``[sql_fts5]``.

The same ``text_match`` is also a Phase 0.4 ``daslib/fts5_query``
predicate (full FTS5-grammar parser, in-memory matcher); user
code gets in-memory and SQL-side matching from one call site.

Declaring the virtual table
===========================

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost
    require sqlite/sqlite_linq

    [sql_fts5(name = "docs_idx")]
    struct Doc {
        Body : string
        @sql_fts_rank Rank : float
    }

    db |> create_table(type<Doc>)
    // emits:
    //   CREATE VIRTUAL TABLE IF NOT EXISTS "docs_idx"
    //       USING fts5("Body", tokenize='unicode61')

INSERT works like any ``[sql_table]``; the ``Rank`` column is
skipped automatically (FTS5 has no user-writable rank).

.. code-block:: das

    db |> insert(Doc(Body = "The quick brown fox jumps over the lazy dog"))
    db |> insert(Doc(Body = "A swift red fox escaped at dawn"))

Querying
========

Use ``text_match`` inside ``_where``. ``_order_by(_.Rank)`` gives
ascending BM25 score (lower = more relevant first):

.. code-block:: das

    let foxes <- _sql(db |> select_from(type<Doc>)
                        |> _where(_.Body |> text_match("quick fox"))
                        |> _order_by(_.Rank))
    // emits:
    //   SELECT "Body", rank FROM "docs_idx"
    //   WHERE "Body" MATCH ? ORDER BY rank

FTS5 query syntax (passed through to ``MATCH``)
===============================================

================================ ====================================================
Pattern                          Meaning
================================ ====================================================
``quick fox``                    whitespace-AND --- both tokens (any order, any distance)
``quick OR sleepy``              Boolean OR
``quick AND NOT fox``            Boolean AND NOT
``quick*``                       prefix match
``"quick brown"``                phrase match (adjacent tokens, in order)
``NEAR(quick fox, 3)``           proximity --- both within 3 tokens
================================ ====================================================

Compile-time rejection on non-FTS5 columns
==========================================

The translator refuses to silently emit ``MATCH`` against a
non-FTS5 column --- it has no way to give useful behavior. The
error message points at ``contains`` (LIKE-based substring) or
adding ``[sql_fts5]``:

.. code-block:: das

    [sql_table(name = "Articles")]
    struct Article { Title : string }

    _sql(db |> select_from(type<Article>)
        |> _where(_.Title |> text_match("foo")))
    // ^^^ compile error:
    //     _sql: text_match() requires an [sql_fts5] struct, got 'Article'.
    //     Options: use contains(_.Col, q) for LIKE-based substring match,
    //     or annotate the struct with [sql_fts5] for indexed full-text
    //     search.

In-memory ``text_match`` (outside SQL)
======================================

The same query syntax works in-memory via ``daslib/fts5_query``.
Useful for log scanning, in-memory filtering before bulk
insert, custom highlighters:

.. code-block:: das

    require daslib/fts5_query

    let lines = ["The quick brown fox", "the lazy dog", "quicksand boot"]
    var hits : array<string>
    for (line in lines) {
        if (line |> text_match("quick OR fox")) {
            hits |> push_clone(line)
        }
    }

The in-memory matcher supports the **full** FTS5 grammar
(phrases, AND/OR/NOT, NEAR, prefix, parens). For repeated
matching against the same query, compile once with
``compile_text_match(query)`` and reuse the ``Fts5Query`` value.

Choosing the right predicate
============================

============================== ================ ===================================
Need                           Predicate        SQL emission
============================== ================ ===================================
Exact equality                 ``==``           ``col = ?``
Starts with prefix             ``starts_with``  ``col LIKE 'x%' ESCAPE '\'``
Ends with suffix               ``ends_with``    ``col LIKE '%x' ESCAPE '\'``
Contains substring             ``contains``     ``col LIKE '%x%' ESCAPE '\'``
Full-text + relevance rank     ``text_match``   ``col MATCH ?`` (+ ``[sql_fts5]``)
============================== ================ ===================================

Rule of thumb:

* **Short-column prefix lookup** (names, SKUs, tags):
  ``starts_with`` + a B-tree index on the column (:ref:`tut 24
  <tutorial_sql_indexes>`). LIKE with a literal leading prefix
  uses the index.
* **Substring search on small/medium tables:** ``contains``
  is fine; full scan at small scale is free.
* **Big document bodies, relevance ranking, query operators:**
  ``text_match`` + ``[sql_fts5]``.

v1 limitations
==============

* **Self-contained mode only.** The FTS5 table holds both
  content and index. External-content mode (FTS5 indexing rows
  of a separate ``[sql_table]``) is deferred.
* **Default tokenizer is unicode61** (case-fold + Unicode word
  boundaries). Custom tokenizers (``porter``, ``ascii``,
  ``unicode61 remove_diacritics 2``) need raw DDL.
* **No typed UPDATE / DELETE.** Drop and re-INSERT, or use raw
  ``db |> exec("DELETE FROM docs_idx WHERE rowid = ?", id)``.
* **BM25 weighting, snippet/highlight helpers, per-column query
  filters (``Body:fox``)** all work via the FTS5 query string
  but don't have typed wrappers in v1.

LIKE escaping (portable trio)
=============================

The portable trio --- ``starts_with`` / ``ends_with`` /
``contains`` --- automatically escapes ``%``, ``_``, ``\`` in
the bound value via SQL ``ESCAPE '\'``. A user typing
``"50%"`` matches literal ``"50%"`` (not "50" + anything). User
code never sees escape sequences:

.. code-block:: das

    let prefix = "50%"
    let hits <- _sql(db |> select_from(type<Item>)
                       |> _where(_.Code |> starts_with(prefix)))
    // emits LIKE ? ESCAPE '\' with the bound value pre-escaped
    // and pre-wildcarded ("50\%%").

.. seealso::

    Full source: :download:`tutorials/sql/40-fts5.das <../../../../tutorials/sql/40-fts5.das>`

    Previous tutorial: :ref:`tutorial_sql_concurrency`
