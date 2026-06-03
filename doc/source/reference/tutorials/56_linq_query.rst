.. _tutorial_linq_query:

==========================================
C#-style LINQ query syntax (``%linq!``)
==========================================

.. index::
    single: Tutorial; LINQ query syntax
    single: Tutorial; linq_das
    single: Tutorial; query continuation
    single: Tutorial; group into

This tutorial covers ``daslib/linq_das`` — the ``%linq! ... %%`` reader macro
that adds a C#-like query-expression form on top of the ``_fold`` engine. A
query is a purely compile-time rewrite into a fused ``_fold(...)`` chain, so it
costs nothing over the hand-written pipe form, and the same query runs over
arrays, decs, SQL, and XML.

It assumes you have read :ref:`tutorial_linq` (the pipe-form linq surface and the
``_fold`` macro). The full clause grammar and per-source details live in the
:ref:`linq_das` reference page.

from / where / select
=====================

The range variable (``c``) is spliced verbatim as the lambda parameter.
``select c`` (the variable alone) is the identity projection; any other
projection emits a ``_select``.

.. code-block:: das

    let names <- %linq! from c in cars where c.price > 100 select c.name %%
    print("expensive: {names}\n")
    // output: expensive: [ m3, a4, a8]

orderby (multi-key)
===================

Comma-separated keys, each with its own optional ``ascending`` / ``descending``.
More than one key emits a single composite stable sort.

.. code-block:: das

    let sorted <- %linq! from c in cars orderby c.brand, c.price descending
                         select (b = c.brand, p = c.price) %%
    // audi 400, audi 200, bmw 250, bmw 100, kia 50

group ... by  (IGrouping)
=========================

A bare ``group c by k`` yields one ``(key, [members])`` tuple per bucket. Read
the key as ``._0`` and the members as ``._1``.

.. code-block:: das

    let buckets <- %linq! from c in cars group c by c.brand %%
    for (g in buckets) {
        print("{g._0}: {g._1 |> length} cars\n")
    }

group ... into g ... select  (grouped aggregation)
==================================================

``into g`` rebinds the group to a new range variable and continues the query.
With the *A2* convention ``g.key`` is the bucket key and a bare ``g`` is the
member collection, so the familiar LINQ aggregates read naturally. This is the
canonical C# grouped-aggregation idiom, and over a SQL source it pushes straight
down to ``GROUP BY`` + ``COUNT/SUM/AVG/...``.

.. code-block:: das

    let report <- %linq! from c in cars
                         group c by c.brand into g
                         select (brand = g.key,
                                 count = g |> length,
                                 total = g |> select($(u : Car) => u.price) |> sum) %%
    // audi: count=2 total=600 ; kia: count=1 total=50 ; bmw: count=2 total=350

The continuation may itself filter / order the groups:

.. code-block:: das

    let bigBrands <- %linq! from c in cars
                            group c by c.brand into g
                            where g |> length > 1
                            orderby g |> select($(u : Car) => u.price) |> sum descending
                            select g.key %%
    // output: [ audi, bmw]

select ... into n  (projection continuation)
============================================

``select <proj> into n`` rebinds the projected value. Continuations chain, so a
query can project, filter, then re-order, all fused on one pass with no
intermediate array.

.. code-block:: das

    let dear <- %linq! from c in cars
                       select c.price into p
                       where p >= 200
                       orderby p descending
                       select p %%
    // output: [ 400, 250, 200]

let bindings and join
=====================

``let`` introduces a computed value reused downstream (inlined at compile time).
``join ... on ... equals ...`` is a single inner equi-join introducing a second
range variable.

.. code-block:: das

    let located <- %linq! from c in cars join h in hqs on c.brand equals h.brand
                          select (car = c.name, country = h.country) %%

.. seealso::

   :ref:`linq_das` — the full ``%linq!`` clause grammar, the four sources, and
   the aggregate-vs-member-keeping rules for ``into`` over SQL.

   :doc:`/reference/linq_fold_patterns` — catalog of chain shapes that ``_fold``
   recognizes, and the splice arm each one fires.

   :ref:`tutorial_linq` — the underlying pipe-form linq surface and the
   ``_fold`` macro.

   Full source: :download:`tutorials/language/56_linq_query.das <../../../../tutorials/language/56_linq_query.das>`

   Previous tutorial: :ref:`tutorial_linq_decs`
