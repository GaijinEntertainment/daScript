.. _linq_fold_patterns:

==================================================================
LINQ-fold patterns — what ``_fold(...)`` recognizes
==================================================================

This is a catalog of every chain shape that the ``_fold`` macro
splices into a specialized loop. Each row names a chain pattern, the
splice arm in ``daslib/linq_fold.das`` it dispatches to, and a short
note on the emitted loop's shape.

Pipelines that don't match any row here fall through to the default
``fold_linq_default`` path, which lowers via the standard
iterator-materializing surface (the same code path as the
non-spliced linq calls).

For the corresponding tutorial, see :ref:`tutorial_linq_decs`. For
nanosecond-level cost comparisons across the SQL / Array / Decs lanes,
see ``benchmarks/sql/results.md`` in the source tree.

How dispatch works
==================

``_fold`` walks the chain inside-out (terminator first), flattens the
``ExprCall`` spine via ``flatten_linq``, and hands the whole thing to
each ``plan_*`` function below in turn. Each ``plan_*`` either returns
a specialized expression (the splice) or ``null`` (defer to the next).
The first non-null wins.

The dispatch order in ``LinqFold.visit`` (``daslib/linq_fold.das``) is:

1. ``plan_decs_order_family`` — decs source with order family.
2. ``plan_order_family`` — array source with order family.
3. ``plan_decs_reverse`` — decs source with ``reverse``.
4. ``plan_reverse`` — array source with ``reverse``.
5. ``plan_decs_distinct`` — decs source with ``distinct``/``distinct_by``.
6. ``plan_distinct`` — array source with ``distinct``/``distinct_by``.
7. ``plan_decs_group_by`` — decs source with ``group_by`` (dispatches
   to the shared ``plan_group_by_core``).
8. ``plan_group_by`` — array source with ``group_by`` (same shared core).
9. ``plan_decs_unroll`` — generic decs walk
   (``where``/``select``/``skip``/``take`` + counter / accumulator /
   early-exit / walk lane).
10. ``plan_zip`` — ``zip(a, b)`` source.
11. ``plan_loop_or_count`` — generic array walk
    (``where``/``select``/``skip``/``take`` + counter / accumulator /
    early-exit terminator).
12. ``fold_linq_default`` — fallback, no splice.

The decs and array variants are interleaved (each decs plan runs
before its array counterpart) because a chain starting with
``from_decs_template(...)`` must hit the decs splice — falling through
to the array plan would not match and would force the default. The
generic decs walk (``plan_decs_unroll``) runs after the specialized
decs plans so it doesn't shadow them.

Source-side entry points
========================

.. list-table::
   :header-rows: 1
   :widths: 30 25 45

   * - Source
     - Recognized by
     - Note
   * - ``each(array<T>)``
     - ``peel_each``
     - Strips the ``each`` wrapper; subsequent chain plans see the raw ``array<T>`` source.
   * - ``zip(a, b)`` / ``zip(a, b, c)``
     - ``plan_zip``
     - Two- or three-source zip. Splice fuses zip + select + aggregate.
   * - ``from_decs_template(type<T>)``
     - ``plan_decs_unroll`` etc.
     - Surfaces a ``[decs_template]`` schema. Decs splices fire.
   * - ``from_decs(...)``
     - ``plan_decs_unroll`` etc.
     - Runtime component-name list form. Same decs splices as the template form.

Array-source patterns
=====================

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape
     - Splice arm
     - Notes
   * - ``.count()`` / ``.long_count()``
     - ``emit_length_shortcut``
     - O(1) length read when the source has a known length.
   * - ``._where(P).count()`` / ``.long_count()``
     - ``plan_loop_or_count`` → ``emit_counter_lane``
     - Single counter, no allocation; one pass over the array.
   * - ``._where(P)._select(F).sum()`` / ``.average()`` / ``.min()`` / ``.max()`` / ``.aggregate(seed, op)``
     - ``plan_loop_or_count`` → ``emit_accumulator_lane``
     - Single-pass scalar reduce; ``where`` and ``select`` fuse into the body.
   * - ``._where(P)._select(F).min_by(K)`` / ``.max_by(K)`` / ``.min_max()`` / ``.min_max_by(K)`` / ``.min_max_average()`` / ``.min_max_average_by(K)``
     - ``plan_loop_or_count`` → ``emit_accumulator_lane``
     - Multi-output reduce; one scalar per output kept in the loop's state.
   * - ``._where(P).first()`` / ``.first_or_default()`` / ``.last()`` / ``.last_or_default()`` / ``.single()`` / ``.single_or_default()`` / ``.element_at(N)`` / ``.element_at_or_default(N)`` / ``.aggregate(...)``
     - ``plan_loop_or_count`` → ``emit_early_exit_lane``
     - Early-exit terminator; loop breaks on first match (``first``), counts to N (``element_at``), or carries-last (``last`` / ``aggregate``).
   * - ``._where(P).any()`` / ``.all(P)`` / ``.contains(V)``
     - ``plan_loop_or_count`` → ``emit_any_empty_shortcut``
     - Boolean fast-path; loop exits on first hit (``any``/``contains``) or first miss (``all``).
   * - ``._where(P).take(N).count()`` / ``.sum()``
     - ``plan_loop_or_count`` (counter / accumulator with ``takeExpr``)
     - Bounded counter/accumulator; loop exits at N matches.
   * - ``._where(P).take_while(P2).<...>`` / ``.skip_while(P2).<...>``
     - ``plan_loop_or_count`` (predicate-driven ranges)
     - ``take_while`` exits on first non-match; ``skip_while`` toggles state.
   * - ``._order_by(K).first()`` / ``.first_or_default()``
     - ``plan_order_family`` (streaming-min)
     - Single ``var best`` + ``var seen``, no buffer; one comparison per element.
   * - ``._order_by(K).take(N).to_array()``
     - ``plan_order_family`` (bounded-heap)
     - ``spliced_push_heap`` fill + replace, ``spliced_pop_heap`` on replace, ``order_inplace`` at end. Buffer of size N.
   * - ``._order_by(K).to_array()`` / ``.order_by_descending(K).to_array()`` / ``.order(K).to_array()`` / ``.order_descending(K).to_array()``
     - ``plan_order_family`` (full-sort fallback)
     - Materializes + sorts. No bounded-heap shortcut.
   * - ``._distinct()`` / ``._distinct_by(K)`` followed by ``.count()`` / ``.to_array()``
     - ``plan_distinct``
     - Single-hash set lane; ``count`` reads ``length(set)``.
   * - ``._group_by(K)._select(reduce).to_array()``
     - ``plan_group_by_core`` → ``emit_reducer_branches``
     - Per-key bucket reducer; single hash, one entry per group.
   * - ``._group_by(K)._having(P)._select(...).to_array()``
     - ``plan_group_by`` → ``plan_group_by_core``
     - HAVING filter applied after the per-key reduce.
   * - ``.reverse().take(N).to_array()`` (with no ``where`` / ``select``)
     - ``plan_reverse`` (two-pass)
     - Sum archetype sizes, then walk tail-first with skip-counter and early-exit.

Decs-source patterns
====================

Every array pattern above has a decs mirror that walks each archetype
of ``T``'s template instead of iterating the array. The body shape is
identical — only the source iteration changes.

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape (decs source)
     - Splice arm
     - Notes
   * - ``from_decs_template(type<T>)._where(P).count()``
     - ``plan_decs_unroll`` → ``emit_decs_count_archsize``
     - Sums archetype sizes when ``where`` is absent; counter loop otherwise.
   * - ``from_decs_template(...)._select(F).sum()`` / ``.average()`` / ``.min()`` / ``.max()`` / ``.aggregate(...)``
     - ``plan_decs_unroll`` → ``emit_decs_accumulator``
     - Per-archetype accumulator; pruner keeps only the components read by ``F``.
   * - ``from_decs_template(...).first()`` / ``.first_or_default()`` / ``.last()`` / ``.last_or_default()`` / ``.single()`` / ``.single_or_default()`` / ``.element_at(N)`` / ``.element_at_or_default(N)`` / ``.aggregate(...)``
     - ``plan_decs_unroll`` → ``emit_decs_walk_lane`` / ``emit_decs_element_at``
     - Walk lane reads one component per loop iteration; element_at uses cumulative-size short-circuit.
   * - ``from_decs_template(...).any()`` / ``.all(P)`` / ``.contains(V)``
     - ``plan_decs_unroll`` → ``emit_decs_early_exit``
     - Boolean fast-path; walks until first match or end.
   * - ``from_decs_template(...).to_array()``
     - ``plan_decs_unroll`` → ``emit_decs_to_array``
     - Concatenates archetype slices; one allocation sized by ``sum(arch.size)``.
   * - ``from_decs_template(...)._order_by(K).first()`` / ``.first_or_default()``
     - ``plan_decs_order_family`` (streaming-min)
     - Single ``var best`` + ``var seen`` across archetypes.
   * - ``from_decs_template(...)._order_by(K).take(N).to_array()``
     - ``plan_decs_order_family`` (bounded-heap)
     - Same heap pattern as the array variant; buffer size N.
   * - ``from_decs_template(...).min_by(K)`` / ``.max_by(K)``
     - ``plan_decs_unroll`` → ``emit_decs_min_max_by``
     - Streaming-min/max with key.
   * - ``from_decs_template(...)._distinct()`` / ``._distinct_by(K)``
     - ``plan_decs_distinct``
     - Single-hash set lane mirroring ``plan_distinct``.
   * - ``from_decs_template(...).reverse().take(N).to_array()``
     - ``plan_decs_reverse``
     - Whole-archetype skip + partial-archetype skip-counter + early-exit.
   * - ``from_decs_template(...)._group_by(K)._select(reduce).to_array()``
     - ``plan_decs_group_by`` → ``plan_group_by_core``
     - Shared bucket-reducer with the array path; differs only in the per-element source.
   * - ``from_decs_template(...)._take_while(P).<...>`` / ``._skip_while(P).<...>``
     - ``plan_decs_unroll`` (predicate-driven ranges)
     - Hoists ``skippingName`` state across archetypes.

Zip patterns
============

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape
     - Splice arm
     - Notes
   * - ``zip(a, b)._select(F).sum()`` / ``.count()`` / ``.average()``
     - ``plan_zip``
     - Fuses to a single index-loop over the shorter side.
   * - ``zip(a, b, c)._select(F).<terminator>``
     - ``plan_zip``
     - Three-source zip; same loop shape with three reads per iteration.
   * - ``zip(a, b)._where(P)._select(F).<terminator>``
     - ``plan_zip`` (chain ops)
     - ``where`` / ``select`` / ``take`` / ``skip`` / ``take_while`` / ``skip_while`` between zip and the terminator are all fused.
   * - ``zip(a, b).first()`` / ``.first_or_default()`` / ``.aggregate(...)``
     - ``plan_zip`` (early-exit / accumulator)
     - Early-exit terminator on the zipped pair.

What falls back
===============

The default path (``fold_linq_default``) fires when none of the
``plan_*`` arms recognize the chain. This is the standard linq surface
— iterators materialize, callbacks fire, and there is no fusion.

Common cases that fall back:

- **Mixed-source operators** like ``union(a, b)``, ``except(a, b)``,
  ``intersect(a, b)``, ``concat(a, b)`` after the first source has
  been transformed (e.g. ``each(a)._select(F).union(b)``).
- **Join terminators**: ``_join`` / ``_left_join`` / ``_right_join`` /
  ``_full_outer_join`` / ``_cross_join``. The join itself does not yet
  splice; downstream ``.count()`` / ``.sum()`` chains fall back.
- **Aggregations on lazy groupings**: ``_group_by_lazy(K)._select(F)``
  with a non-bucket-reducing ``_select``.
- **Materialization-only chains** that the standard linq surface
  already lowers efficiently — e.g. ``to_table()`` on a finite array.

When a chain falls back, behavior is identical to writing the same
chain without ``_fold`` — correct, but not splice-fused.

See also
========

* :ref:`tutorial_linq_decs` — using ``from_decs_template`` and
  ``_fold`` over decs entities.
* :ref:`tutorial_linq` — the underlying linq surface (``where``,
  ``select``, ``order_by``, etc.) and the ``_fold`` macro.
* :doc:`/stdlib/generated/linq_fold` — the ``linq_fold`` module
  reference (the two public macros ``_fold`` and ``_old_fold``).
* :doc:`/stdlib/generated/linq` — the full linq surface.
* :doc:`/stdlib/generated/linq_boost` — the shorthand macros
  (``_where``, ``_select``, ``_order_by``, etc.) and the surrounding
  macro family.
