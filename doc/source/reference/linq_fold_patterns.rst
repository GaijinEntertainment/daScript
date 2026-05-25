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

Pre-dispatch normalizations
===========================

A few chain rewrites fire at the start of the relevant planners (right
after ``flatten_linq``, before the per-arm pattern-match) so the rest of
that planner's logic sees the normalized shape and a single arm covers
what would otherwise be many lookalike chains:

- ``_order_by(K).reverse()`` → ``_order_by_descending(K)``
  (and the three symmetric flips: ``order_by_descending → order_by``,
  ``order → order_descending``, ``order_descending → order``). Applied
  by ``normalize_order_reverse``, called from every ``plan_*order_family``
  and ``plan_*reverse`` planner right after ``flatten_linq``. The
  registry pointer is swapped to the flipped variant in place — the
  ``ExprCall`` arg list is identical for ascending/descending order
  variants, so no AST clone is needed. Iterative: a chain like
  ``_order_by(K).reverse().reverse()`` collapses to ``_order_by(K)`` in
  two passes.

- ``_select(f) |> _select(g)`` (N consecutive) → ``_select(g(f(_)))``.
  Applied by ``collapse_chained_selects``, called from
  ``plan_zip``, ``plan_distinct``, ``plan_decs_distinct``,
  ``plan_reverse``, ``plan_decs_reverse``, ``plan_decs_join``, and
  defensively from ``plan_order_family`` / ``plan_decs_order_family``
  (which don't currently accept any leading ``_select`` but would
  inherit collapse if they ever did). Mirrors how chained ``_where``
  already compose via ``&&``. Composition takes the INNER lambda's
  structure (preserves param type), renames its bound param to a
  fresh ``qn("cs", at)`` name to avoid ``apply_template`` recursive
  substitution when both lambdas share the boost-side ``_`` desugar,
  then overwrites its body with outer's body where outer's param is
  substituted by the renamed-inner body. Chain backlink rewired so
  subsequent planner passes see the shortened AST.
  Gated on ``!has_sideeffects(innerBody)`` — collapsing would shift
  evaluation count when outer references its param zero or many times
  (cascade always evaluates inner once per element). Chains with
  ``%`` / ``/`` / user-call inner cascade to tier-2 (output remains
  correct). Also bails (cascades) when either selector is not a
  peelable single-arg, single-return ``ExprMakeBlock`` lambda —
  multi-statement projection bodies, captured/non-trivial lambda
  shapes, and function-pointer arguments all skip collapse.
  ``plan_loop_or_count``, ``plan_group_by_core``, and ``plan_decs_unroll``
  already handle chained selects natively via their ``intermediateBinds`` /
  chain-info machinery and don't need the pre-pass.

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
   * - ``zip(a, b)`` / ``zip(a, b, sel)``
     - ``plan_zip``
     - Two-source zip. The three-argument form ``zip(a, b, sel)`` is pre-lowered to ``zip(a, b) |> _select(sel-as-tuple)`` so the standard zip+select fusion fires (closes the dot-product idiom).
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
   * - ``.take(N)._where(P).<terminator>`` (counter / accumulator / early-exit / array)
     - ``plan_loop_or_count`` (``postTakeWhereCond`` gate)
     - Take cap ticks unconditionally; ``where`` gates only the per-element contribution. Preserves the "first N elements, then keep matching" semantic that ``where.take`` cannot express. Single trailing ``where`` only — skip / skip_while / take_while + where still cascade.
   * - ``._where(P).take_while(P2).<...>`` / ``.skip_while(P2).<...>``
     - ``plan_loop_or_count`` (predicate-driven ranges)
     - ``take_while`` exits on first non-match; ``skip_while`` toggles state.
   * - ``._order_by(K).first()`` / ``.first_or_default()``
     - ``plan_order_family`` (streaming-min)
     - Single ``var best`` + ``var seen``, no buffer; one comparison per element.
   * - ``._order_by(K).take(N).to_array()``
     - ``plan_order_family`` (bounded-heap)
     - ``spliced_push_heap`` fill + replace, ``spliced_pop_heap`` on replace, ``order_inplace`` at end. Buffer of size N.
   * - ``._distinct_by(K1)._order_by(K2).take(N).to_array()`` / ``._order_by(K2).distinct().take(N).to_array()`` (plain ``distinct()`` mirror order accepted)
     - ``plan_order_family`` (bounded-heap + set-gate)
     - Theme 3 Phase 3 (audit C1/C5). The bounded-heap path gains a leading or middle ``distinct[_by]`` recognizer; per-element push/pop is gated by a set-insert on the distinct key (or whole element for plain ``distinct``). Single source pass, no full distinct materialization. Position of ``distinct`` in the chain (before vs after ``_order_by``) has no bearing on emission for the safe shapes — the set just gates the same heap update. **Bails** (cascades) on ``_order_by(K2).distinct_by(K1)`` because cascade semantics ("min-K2 per K1" — first K1 occurrence in sort order) cannot be honored by a source-walk set-gate, which would keep an arbitrary K1 representative; on ``distinct[_by]`` without ``take`` (would be silently dropped); and on ``take(N).distinct[_by]()`` (would dedup pre-take instead of post-take). Inline-able order key required (cascades otherwise). Composes with ``where_`` (filter before distinct gate) and terminal ``_select`` (project ≤N heap survivors at return).
   * - ``._order_by(K).take(N)._select(F).to_array()`` / ``.first()._select(F)`` / ``.first_or_default()._select(F)``
     - ``plan_order_family`` (terminal ``_select``)
     - Bounded-heap / streaming-min holds the raw element; projection ``F`` runs ≤K times at return. Closes the natural "take top-K then project" idiom.
   * - ``._order_by(K).to_array()`` / ``.order_by_descending(K).to_array()`` / ``.order(K).to_array()`` / ``.order_descending(K).to_array()``
     - ``plan_order_family`` (full-sort fallback)
     - Materializes + sorts. No bounded-heap shortcut.
   * - ``._distinct()`` / ``._distinct_by(K)`` followed by ``.count()`` / ``.to_array()``
     - ``plan_distinct``
     - Single-hash set lane; ``count`` reads ``length(set)``.
   * - ``._distinct()`` / ``._distinct_by(K)`` followed by ``.count(P)`` / ``.long_count(P)``
     - ``plan_distinct`` (predicate counter)
     - Dedup table is built unconditionally so ``distinct_by`` semantics keep FIRST occurrence per key; a separate ``var acc`` increments only when ``P`` matches that first occurrence. Mirrors tier-2 ``distinct.count(P)`` semantics (distinct-then-filter, not filter-then-distinct).
   * - ``._group_by(K)._select(reduce).to_array()``
     - ``plan_group_by_core`` → ``emit_reducer_branches``
     - Per-key bucket reducer; single hash, one entry per group.
   * - ``._group_by(K)._having(P)._select(...).to_array()``
     - ``plan_group_by`` → ``plan_group_by_core``
     - HAVING filter on the bucket reference (pre-aggregate); can lift hidden reducer slots referenced by ``P`` but absent from the select.
   * - ``._group_by(K)._select(reduce)._where(P).to_array()`` / ``.count()``
     - ``plan_group_by`` → ``plan_group_by_core`` (trailing ``where`` as HAVING)
     - HAVING filter on the constructed post-aggregate tuple (predicate references ``_.AggField`` by name). Distinct from ``_having(P)`` and orthogonal — both can fire on the same chain.
   * - ``._group_by(K)._select(reduce)._order_by(K2).to_array()`` / ``._order_by_descending(K2).to_array()``
     - ``plan_group_by`` → ``plan_group_by_core`` (trailing ``order_by`` as ORDER BY)
     - Theme 3 Phase 2 (audit C2). Inline-cmp ``sort(buf, ...)`` after the bucket-fill mutates the same output buffer in place — vs the tier-2 cascade's separate ``order_by_inplace`` over a fresh allocation. v1: ``_order_by(K2)`` / ``_order_by_descending(K2)`` with inline-able key only; non-inline keys (side-effects, multi-stmt body) cascade. Composes with HAVING / ``_having(P)``.
   * - ``.reverse().take(N).to_array()`` (with no ``where`` / ``select``)
     - ``plan_reverse`` (two-pass)
     - Sum archetype sizes, then walk tail-first with skip-counter and early-exit.
   * - ``.reverse().take(N)._select(F).to_array()`` / ``.reverse()._select(F).first()``
     - ``plan_reverse`` (terminal ``_select``)
     - Projection runs ≤K times at return on the R1-R4 buffer or on the surviving ``last`` value. NOT accepted: ``reverse._select.take`` — user must reorder to ``reverse.take._select``.

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
   * - ``from_decs_template(...)._distinct_by(K1)._order_by(K2).take(N).to_array()`` / ``._order_by(K2).distinct().take(N).to_array()`` (plain ``distinct()`` mirror order accepted)
     - ``plan_decs_order_family`` (bounded-heap + set-gate)
     - Decs mirror of the array-side ``plan_order_family`` distinct extension (Theme 3 Phase 3, audit C1/C5). The decs hoisted-buffer bounded-heap path gains the same set-gate inside the per-archetype loop body. Same bail conditions as the array variant: ``_order_by(K2).distinct_by(K1)``, ``distinct[_by]`` without ``take``, and ``take(N).distinct[_by]()`` all cascade.
   * - ``from_decs_template(...)._order_by(K).take(N)._select(F).to_array()``
     - ``plan_decs_order_family`` (terminal ``_select``)
     - Decs mirror of ``plan_order_family``'s terminal ``_select`` — heap holds raw element, projection runs ≤K times at return.
   * - ``from_decs_template(...).min_by(K)`` / ``.max_by(K)``
     - ``plan_decs_unroll`` → ``emit_decs_min_max_by``
     - Streaming-min/max with key.
   * - ``from_decs_template(...)._distinct()`` / ``._distinct_by(K)``
     - ``plan_decs_distinct``
     - Single-hash set lane mirroring ``plan_distinct``.
   * - ``from_decs_template(...)._distinct()`` / ``._distinct_by(K)`` followed by ``.count(P)`` / ``.long_count(P)``
     - ``plan_decs_distinct`` (predicate counter)
     - Decs mirror of the array-side predicate-distinct splice. Same dedup-unconditional / counter-gated-on-P shape across archetypes.
   * - ``from_decs_template(...).reverse().take(N).to_array()``
     - ``plan_decs_reverse``
     - Whole-archetype skip + partial-archetype skip-counter + early-exit.
   * - ``from_decs_template(...).reverse().take(N)._select(F).to_array()`` / ``.reverse()._select(F).first()``
     - ``plan_decs_reverse`` (terminal ``_select``)
     - Decs mirror of ``plan_reverse``'s terminal ``_select``. Skip-into-tail fast path is gated off when ``_select`` is present.
   * - ``from_decs_template(...)._group_by(K)._select(reduce).to_array()``
     - ``plan_decs_group_by`` → ``plan_group_by_core``
     - Shared bucket-reducer with the array path; differs only in the per-element source.
   * - ``from_decs_template(...)._group_by(K)._select(reduce)._where(P).to_array()`` / ``.count()``
     - ``plan_decs_group_by`` → ``plan_group_by_core`` (trailing ``where`` as HAVING)
     - Decs mirror of the array-side post-aggregate HAVING. Same predicate-on-output-tuple semantics.
   * - ``from_decs_template(...)._group_by(K)._select(reduce)._order_by(K2).to_array()`` / ``._order_by_descending(K2).to_array()``
     - ``plan_decs_group_by`` → ``plan_group_by_core`` (trailing ``order_by`` as ORDER BY)
     - Decs mirror of the array-side ORDER BY splice (Theme 3 Phase 2 C2). Shares the same in-place inline-cmp sort tail; only the bucket-fill source differs.
   * - ``from_decs_template(A)._join(from_decs_template(B), ka, kb, result)._group_by(K)._select(reduce).to_array()`` / ``.count()``
     - ``plan_decs_group_by`` (``isDecsJoin`` adapter; cross-arm — see *Decs-decs equi-join*)
     - Theme 3 Phase 1 cross-arm composition. ``plan_decs_join``'s hashB-collect + srcA-probe feeds ``plan_group_by_core``'s bucket update directly — one pass, no intermediate join array. Composes with the C2 trailing ``order_by`` extension above when applied to the join+group_by output.
   * - ``from_decs_template(...)._take_while(P).<...>`` / ``._skip_while(P).<...>``
     - ``plan_decs_unroll`` (predicate-driven ranges)
     - Hoists ``skippingName`` state across archetypes.

Decs-decs equi-join
-------------------

``plan_decs_join`` is the hashed equi-join splice over two
``from_decs_template`` sources. It collects the right side into a
``table<KEY; array<TUPB>>`` in one ``for_each_archetype`` pass, then
walks the left side and probes via ``table.get``. The key must be a
primitive (``int*`` / ``uint*`` / ``float`` / ``double`` / ``bool`` /
``string``); tuple keys cascade to the standard ``join_impl``.

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape
     - Splice arm
     - Notes
   * - ``from_decs_template(A) |> _join(from_decs_template(B), ka, kb, result) |> count()``
     - ``plan_decs_join``
     - Hash-fill + probe; ``count`` bumped by bucket length per hit. No per-pair invoke.
   * - ``from_decs_template(A) |> _join(...) |> to_array()``
     - ``plan_decs_join``
     - Hash-fill + probe; ``result`` lambda inlined at the push site (no per-pair invoke into ``join_impl``).
   * - ``from_decs_template(A) |> _join(...) |> _select(F) |> to_array()``
     - ``plan_decs_join`` (terminal ``_select``)
     - Single bind of the join result per matched pair, then projection.
   * - ``from_decs_template(A) |> _join(...) |> _where(P) |> count() / to_array()``
     - ``plan_decs_join`` (trailing ``_where``)
     - Bind join result, evaluate predicate, gate ``count++`` / ``push_clone``. Composes with the trailing ``_select`` form (filter then project, single bind per pair).
   * - ``from_decs_template(A) |> _join(...) |> _group_by(K) |> _select(reduce) |> count() / to_array()``
     - ``plan_decs_group_by`` (``isDecsJoin`` adapter, Theme 3 C3)
     - Cross-arm composition. ``plan_decs_group_by`` recognizes a trailing
       ``join`` upstream of ``group_by_lazy`` and builds an adapter that
       emits hashB-collect + srcA-probe + per-pair result-lam bind as the
       per-element source loop; that bind feeds
       ``plan_group_by_core``'s ``tab?[uk] ?? dummy`` bucket update
       directly. Single pass, no intermediate ``join`` array. v1
       constraints: ``count`` / ``to_array`` terminator only;
       primitive equi-key (same guard as ``plan_decs_join``); no segments
       between ``join`` and ``group_by_lazy``; HAVING (trailing ``_where``
       after the reducer ``_select``) defers to v2.

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
   * - ``zip(a, b)._select(F).count(P)`` / ``.long_count(P)``
     - ``plan_zip`` (counter with separate predicate gate)
     - The 2-arg ``count(P)`` / ``long_count(P)`` form is captured into a dedicated counter-predicate gate emitted around ``acc++`` *inside* the upstream where/select wrap, so eager ``where(W).select(F).count(P)`` ordering is preserved (W filters first, then F runs once per surviving element, then P decides whether to count). With ``_select``, the predicate peels against the projected value via a ``vproj`` bind. Length-shortcut is suppressed when ``P`` is present (the counter loop runs).

What falls back
===============

The default path (``fold_linq_default``) fires when none of the
``plan_*`` arms recognize the chain. This is the standard linq surface
— iterators materialize, callbacks fire, and there is no fusion.

Common cases that fall back:

- **Mixed-source operators** like ``union(a, b)``, ``except(a, b)``,
  ``intersect(a, b)``, ``concat(a, b)`` after the first source has
  been transformed (e.g. ``each(a)._select(F).union(b)``).
- **Joins other than decs-decs equi-join**: ``_left_join`` /
  ``_right_join`` / ``_full_outer_join`` / ``_cross_join`` don't splice;
  array-source ``_join`` also falls back. Only the decs-decs primitive-key
  ``_join`` shape catalogued above splices (via ``plan_decs_join``);
  tuple keys, non-primitive keys, mixed array/decs sources, or chain ops
  beyond a single trailing ``_where`` / ``_select`` all cascade to
  ``join_impl``.
- **Aggregations on lazy groupings**: ``_group_by_lazy(K)._select(F)``
  with a non-bucket-reducing ``_select``.
- **Materialization-only chains** that the standard linq surface
  already lowers efficiently — e.g. ``to_table()`` on a finite array.
- **Chained ``_select(f) |> _select(g)`` with an impure inner**
  (``_ % N``, ``_ / N``, user-call inner that the typer can't prove
  pure). The ``collapse_chained_selects`` pre-pass is gated on
  ``!has_sideeffects(innerBody)`` because collapsing would shift
  evaluation count when outer references its param zero or many times.
  Pure inner (``_._field``, ``_ + K``, ``_ * K``, etc.) collapses
  transparently and the downstream planner sees a single ``_select``.

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
