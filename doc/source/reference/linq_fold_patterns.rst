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
``ExprCall`` spine via ``flatten_linq``, normalizes adjacent
where/select pairs and ``order |> reverse`` shapes via the pre-passes
described below, and hands the result to ``try_splice_patterns`` in
``daslib/linq_fold.das``. That dispatcher walks a single global
``splice_patterns`` table (17 rows, one per arm listed in this
document) twice:

1. **Decs adapter pass.** Runs only when
   ``extract_decs_bridge(top) != null`` (i.e. the source is
   ``from_decs_template(...)``). Each row's ``requires`` predicates
   gate the match; rows with an ``array_source`` predicate fail
   here and fall through. First emit that returns non-null wins.
2. **Array adapter pass.** Runs only when ``extract_decs_bridge(top)
   == null``, i.e. the source is **not** a decs eager bridge. Top is
   first ``peel_each``-unwrapped. Decs chains never reach this pass:
   if the Decs pass above cascades on every row, control falls
   through to ``fold_linq_default`` instead. (This is deliberate —
   ``peel_each`` does not strip the eager-bridge ``ExprInvoke``, so
   without this gate the ``decs_source`` predicate would still
   succeed on a decs source in the Array pass and decs-only rows
   could match and emit via ``SourceAdapter::Array``, silently
   dropping adapter-specific captures like ``upstream_join``.)

If neither pass emits, the Theme 6 perf-warn fires for
``from_decs_template`` chains (telling the user the bridge will
materialize and tier-2 will run on the buffer), then control falls
through to ``fold_linq_default`` (the iterator-materializing tier-2
path) and finally to a raw passthrough.

.. note::

   Labels of the form ``plan_<X>`` (e.g. ``plan_distinct``,
   ``plan_decs_reverse``) in the catalog below refer to the
   pre-PR-E per-planner stubs that were collapsed into the unified
   dispatcher. Each label now corresponds to a row (or pair of rows)
   in ``splice_patterns`` whose ``requires`` predicates and ``emit``
   function carry the same logic. The names are kept here because
   they read more naturally than row names like
   ``order_buffer_helper_dispatch`` — see ``daslib/linq_fold.das``
   for the precise row each catalog entry maps to.

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
  ``plan_reverse``, ``plan_decs_reverse``, ``plan_decs_join`` (also
  ``collapse_chained_wheres`` per PR D2), and from
  ``plan_order_family`` / ``plan_decs_order_family`` (which now accept
  a leading ``_select`` — see the next bullet). Mirrors how chained ``_where``
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

- ``source |> _select(f) |> <order/distinct/take>`` — a leading ``_select(f)``
  that projects the source is absorbed into the source walk. An optional leading
  ``srcsel`` slot on the four ``wrap_source_loop``-based order rows
  (``streaming_min`` / ``bounded_heap`` / ``order_then_plain_distinct`` /
  ``fused_prefilter`` — not the source-direct ``buffer_helper_dispatch``)
  captures the ``_select``; ``run_splice_adapter`` then wraps the source adapter
  in ``ProjectedSourceAdapter`` (``linq_fold_common``), which binds
  ``projName = f(rawElem)`` atop the per-element body and delegates
  ``wrap_source_loop`` / ``wrap_invoke`` to the inner adapter. Every emit sees
  ``f(rawElem)`` as the element with no emit-fn change, and XML field-pruning is
  preserved (``f``'s ``it.<field>`` reads reach the inner materializer). Without
  it the chain bails to tier-2 (materialize-all + sort-all); the bench m3f/m4
  lanes hid the gap by pre-projecting their source array, the XML lane can't.
  The slot is optional — chains with no leading ``_select`` are byte-identical.

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
     - pattern ``zip_general`` (emit fn ``emit_zip``)
     - Two-source zip. The three-argument form ``zip(a, b, sel)`` is pre-lowered to ``zip(a, b) |> _select(sel-as-tuple)`` so the standard zip+select fusion fires (closes the dot-product idiom).
   * - ``from_decs_template(type<T>)``
     - ``plan_decs_unroll`` etc.
     - Surfaces a ``[decs_template]`` schema. Decs splices fire.
   * - ``from_decs(...)``
     - ``plan_decs_unroll`` etc.
     - Runtime component-name list form. Same decs splices as the template form.
   * - ``unsafe(from_xml_node(node[, name], type<Row>))``
     - ``extract_xml_source`` (``XmlAdapter``, ``modules/dasPUGIXML/daslib/linq_fold_xml.das``)
     - Optional source — only when the ``pugixml`` module is linked (``require ?pugixml`` + ``static_if (typeinfo builtin_module_exists(pugixml))``). Emits an inlined DOM child-element walk replacing the generator, and **field-prunes** the per-element materialization (pass 2b): the chain body is scanned for the ``Row`` fields it reads, and only those attributes are read via ``read_xml_field`` into scalar locals — unread fields (notably ``string`` fields, whose ``clone_string`` is the alloc cost) are never touched, so a float-only chain runs alloc-free and JIT beats the equivalent SQLite query. A whole-row escape (``to_array`` / identity ``_select(_)`` / pass-to-fn) routes to the full ``build_xml_row`` instead. The ``XmlAdapter`` **rides every pattern row** (``try_splice_patterns`` runs with no ``onlyRow`` restriction); per-row ``requires`` predicates and the adapter's capability hooks (``can_join`` / ``can_group_by`` / ``defers_materialization`` / the ``non_array_source`` gate) decide what fuses, and a shape it can't fuse cascades to tier-2 — see :ref:`linq_fold_xml_patterns` for the full fuse/defer breakdown. ``unsafe`` is required (the source is ``[unsafe_outside_of_for]``) and the node is passed by value (``var root`` — ``_fold``'s macro-arg inference skips the const&→value copy).

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
     - ``plan_order_family`` (streaming-min) → ``emit_streaming_min``
     - Single ``var best`` + ``var seen``, no buffer; one comparison per element.
   * - ``._order_by(K).take(N).to_array()``
     - ``plan_order_family`` (bounded-heap) → ``emit_bounded_heap``
     - ``spliced_push_heap`` fill + replace, ``spliced_pop_heap`` on replace, ``order_inplace`` at end. Buffer of size N.
   * - ``._distinct_by(K1)._order_by(K2).take(N).to_array()`` / ``._order_by(K2).distinct().take(N).to_array()`` (plain ``distinct()`` mirror order accepted)
     - ``plan_order_family`` (bounded-heap + set-gate) → ``emit_bounded_heap``
     - Theme 3 Phase 3 (audit C1/C5). The bounded-heap path gains a leading or middle ``distinct[_by]`` recognizer; per-element push/pop is gated by a set-insert on the distinct key (or whole element for plain ``distinct``). Single source pass, no full distinct materialization. Position of ``distinct`` in the chain (before vs after ``_order_by``) has no bearing on emission for the safe shapes — the set just gates the same heap update. **Bails** (cascades) on ``_order_by(K2).distinct_by(K1)`` because cascade semantics ("min-K2 per K1" — first K1 occurrence in sort order) cannot be honored by a source-walk set-gate, which would keep an arbitrary K1 representative; on ``distinct[_by]`` without ``take`` (would be silently dropped); and on ``take(N).distinct[_by]()`` (would dedup pre-take instead of post-take). Inline-able order key required (cascades otherwise). Composes with ``where_`` (filter before distinct gate) and terminal ``_select`` (project ≤N heap survivors at return).
   * - ``._order_by(K).take(N)._select(F).to_array()`` / ``.first()._select(F)`` / ``.first_or_default()._select(F)``
     - ``plan_order_family`` (terminal ``_select``) → ``emit_bounded_heap`` / ``emit_streaming_min``
     - Bounded-heap / streaming-min holds the raw element; projection ``F`` runs ≤K times at return. Closes the natural "take top-K then project" idiom.
   * - ``._order_by(K).to_array()`` / ``.order_by_descending(K).to_array()`` / ``.order(K).to_array()`` / ``.order_descending(K).to_array()``
     - ``plan_order_family`` (full-sort fallback) → ``emit_buffer_helper_dispatch``
     - Materializes + sorts. No bounded-heap shortcut.
   * - ``._order_by_keys((K1, K2, …), descMask).to_array()`` / ``._where(P)._order_by_keys((K1, K2), m).to_array()``
     - ``plan_order_family`` (multi-key composite stable sort)
     - Multi-key orderby with per-key direction (``descMask`` bit *i* → key *i* DESC; LSB = first key). With an inline-able tuple key + an upstream ``where`` (no ``take``/``first``/``distinct``), ``emit_fused_prefilter`` builds one composite if-chain comparator (``try_make_inline_cmp_keys``) and emits a **single** ``stable_sort(buf, cmp)`` on the fused buffer — C# ``OrderBy`` / ``ThenBy`` parity, stable on full ties. Bare ``order_by_keys`` (no ``where``) cascades to the eager ``order_by_keys`` op (also ``stable_sort``-backed). **Single-key ``_order_by`` is unchanged** — it keeps the unstable ``order_inplace`` / ``sort`` path (no regression). ``take`` / ``first`` over a multi-key chain cascade to the eager op (multi-key is gated out of the bounded-heap and streaming-min rows, whose min/max-by-first-key collapse is wrong for a composite key). Capped at 4 keys (eager ``less_masked`` ≤ 4-arity).
   * - ``._distinct()`` / ``._distinct_by(K)`` followed by ``.count()`` / ``.to_array()``
     - ``plan_distinct`` → ``emit_hashtable_dedup``
     - Single-hash set lane; ``count`` reads ``length(set)``.
   * - ``._distinct()`` / ``._distinct_by(K)`` followed by ``.count(P)`` / ``.long_count(P)``
     - ``plan_distinct`` (predicate counter) → ``emit_hashtable_dedup``
     - Dedup table is built unconditionally so ``distinct_by`` semantics keep FIRST occurrence per key; a separate ``var acc`` increments only when ``P`` matches that first occurrence. Mirrors tier-2 ``distinct.count(P)`` semantics (distinct-then-filter, not filter-then-distinct).
   * - ``._distinct[_by](K1)._order_by[_descending](K2).to_array()`` / ``._where(P)._distinct[_by](K1)._order_by(K2).to_array()``
     - ``plan_order_family`` (fused-loop + set-gate) → ``emit_fused_prefilter``
     - Theme 8 (audit 3b). The where_+order fused-loop path generalizes: when upstream ``distinct[_by]`` is present, declare ``var order_dset : table<...>`` and wrap the per-element ``push_clone`` with a set-gated ``if (!key_exists(...))`` block. Single source pass + in-place sort, no ``distinct_by_to_array`` intermediate iterator setup. Composes with ``where_`` (filter before distinct gate) and terminal ``_select`` (project at return). **Bails** (cascades) on ``distinct[_by] + order_by + first[_or_default]`` (streaming-min path has no dset hook) and on chains where ``take(N)`` is present (use the bounded-heap path via Theme 3 Phase 3 instead).
   * - ``._group_by(K)._select(reduce).to_array()``
     - pattern ``group_by_array`` (sub-codegen ``plan_group_by_core`` → ``reducer_emitters`` lookup)
     - Per-key bucket reducer; single hash, one entry per group. PR D1: reducer dispatch is now a ``table<string; ReducerEmitterFn>`` lookup into named ``mk_reducer_*`` fns.
   * - ``._group_by(K)._having(P)._select(...).to_array()``
     - pattern ``group_by_array`` (sub-codegen ``plan_group_by_core``)
     - HAVING filter on the bucket reference (pre-aggregate); can lift hidden reducer slots referenced by ``P`` but absent from the select.
   * - ``._group_by(K)._select(reduce)._where(P).to_array()`` / ``.count()``
     - pattern ``group_by_array`` (sub-codegen ``plan_group_by_core``, trailing ``where`` as HAVING)
     - HAVING filter on the constructed post-aggregate tuple (predicate references ``_.AggField`` by name). Distinct from ``_having(P)`` and orthogonal — both can fire on the same chain.
   * - ``._group_by(K)._select(reduce)._order_by(K2).to_array()`` / ``._order_by_descending(K2).to_array()``
     - pattern ``group_by_array`` (sub-codegen ``plan_group_by_core``, trailing ``order_by`` as ORDER BY)
     - Theme 3 Phase 2 (audit C2). Inline-cmp ``sort(buf, ...)`` after the bucket-fill mutates the same output buffer in place — vs the tier-2 cascade's separate ``order_by_inplace`` over a fresh allocation. v1: ``_order_by(K2)`` / ``_order_by_descending(K2)`` with inline-able key only; non-inline keys (side-effects, multi-stmt body) cascade. Composes with HAVING / ``_having(P)``.
   * - ``.reverse().take(N)[._select(F)].to_array()`` (with no pre-reverse ``where`` / ``select``)
     - ``plan_reverse`` R6 (backward-index walk) → ``emit_reverse_backward_index_walk``
     - Single loop ``for k in 0..K`` indexes ``arr[len-1-k]`` and K push_clones into a srcElem-typed scratch buffer. When ``_select(F)`` is captured, ``build_terminal_select_tail`` then performs a post-loop projection pass into a separate projElem-typed output buffer (K projection push_clones). Two-buffer/two-pass mirrors the decs sibling ``emit_decs_reverse_skip_into_tail`` (PR #2915) and the R1-R4 catch-all: all source reads complete before any projection-side-effect runs, so impure ``_select`` behaves identically across the three paths. Skips the catch-all's full-source ``push_clone`` walk (N → K raws) + ``reverse_inplace`` + ``resize``. Fast path bails (cascades to R1-R4) when termsel's call-result element type is unresolved at macro stage.
   * - ``[._where(P)][._select(f)].reverse().take(N)._select(F).to_array()`` / ``.reverse()._select(F).first()``
     - ``plan_reverse`` R1-R4 (terminal ``_select`` on catch-all) → ``emit_reverse_buffer_inplace`` / Rb (walk-and-overwrite scalar) → ``emit_reverse_walk_overwrite_scalar``
     - Catch-all path for chains with pre-reverse ``_where`` / ``_select`` (R6 doesn't accept those slots, cascades here). Projection runs ≤K times at return on the R1-R4 buffer or on the surviving ``last`` value. NOT accepted: ``reverse._select.take`` — user must reorder to ``reverse.take._select``.
   * - ``each(arr).reverse()._distinct[_by](K).to_array()`` (array source)
     - ``plan_reverse`` R-2a (backward index walk + set-gate) → ``emit_reverse_backward_walk_dset_gate``
     - Theme 8 (audit 2a). Array source only (``array_source`` predicate). Walks source backward via index (``arr[len-1-k]``), maintains ``var rev_dset : table<...>`` and gates push by set-insert on the dedup key (or whole element for plain ``distinct``). LAST-per-key semantics preserved: backward walk picks first-seen-in-reversed-order = last-in-source occurrence, matching tier-2 ``reverse.distinct_by``. Saves cascade's ``reverse_to_array`` allocation AND second ``distinct_by_inplace`` pass. v1 implicit ``to_array`` only; pre-reverse ``_where`` / ``_select`` / ``take`` bail to cascade. Non-array (forward) sources take R-2b below.
   * - ``src.reverse()._distinct[_by](K).to_array()`` (XML / decs / iterator source)
     - ``plan_reverse`` R-2b (forward keep-last table-overwrite) → ``emit_reverse_distinct_forward_keeplast``
     - The exact complement of R-2a (``non_array_source`` predicate): forward-only sources have no random index for the backward walk. One forward pass OVERWRITES ``var rdb_tab : table<key; (seq, val)>`` per element (so the slot ends at the last forward occurrence + its monotonic seq), then sorts survivors by **descending seq** and emits — output-identical to R-2a (descending forward-index of each last occurrence). Source-generic via ``emit_terminator_lane`` + ``wrap_source_loop``: an XML source **defers** (``val`` is the ``xml_node`` handle; ``build_xml_row`` runs only for the K survivors, field-pruned to the key), while decs / iterator store the full element and still win single-pass over the cascade's reverse-buffer + second walk. Closes the decs ``m4`` cell for this shape (D6).
   * - ``[._where(P)][._select(F)].reverse().count()``
     - ``plan_reverse`` Ra (counter) → ``emit_reverse_counter``
     - Reverse is identity for a count, so one forward pass increments a counter — no buffer, no reverse. The projection still fires per match (side-effect parity). Works on iterator and array sources (for-loop body, no indexed access).

Decs-source patterns
====================

Every array pattern above has a decs mirror that walks each archetype
of ``T``'s template instead of iterating the array. The body shape is
identical — only the source iteration changes.

.. note::

   As of PR C, the four ``plan_decs_*`` planners (``_reverse`` /
   ``_distinct`` / ``_order_family`` / ``_unroll``) are thin pattern-table
   stubs that reuse the same pattern rows as their array-side siblings
   (``plan_reverse_patterns`` / ``plan_distinct_patterns`` /
   ``plan_order_family_patterns`` / ``plan_loop_or_count_patterns``)
   with a ``SourceAdapter::Decs`` adapter swap. The 7 array-side emit
   archetypes consume the adapter (``adapter_bind_name`` selects
   ``it`` vs ``decs_tup`` for lambda peeling; ``adapter_wrap_source_loop``
   dispatches ``for (it in src)`` vs ``for_each_archetype + build_decs_inner_for_pruned``;
   ``adapter_wrap_invoke`` dispatches the outer invoke wrap). For
   ``plan_decs_unroll`` (which feeds ``emit_loop_or_count_lane``), the
   Decs-arm dispatch (``emit_loop_or_count_lane_decs``) reconstructs a
   calls array from captures and routes to the existing
   ``emit_decs_*`` lane fns unchanged (state hoist above
   ``for_each_archetype`` stays per-adapter; see masterplan D1).

   Two decs-specific fast paths preserved: ``emit_decs_count_archsize``
   (bare ``count()``) and ``emit_decs_reverse_skip_into_tail``
   (``reverse |> take(N) |> to_array``). Row 4 of
   ``plan_order_family_patterns`` (``buffer_helper_dispatch``) is gated
   to Array adapter via ``array_source`` — decs cascades to Row 3
   (``fused_prefilter``) which materializes the buffer, matching the
   imperative decs behavior. ``reverse |> distinct[_by]`` on decs
   sources now fuses via the source-generic R-2b forward keep-last row
   (``emit_reverse_distinct_forward_keeplast``, gated ``non_array_source``)
   — one table-overwrite emit shared by decs / XML / iterators, not a
   parallel decs fn (closes masterplan D6).

   As of PR D3, the ``GroupBySourceAdapter`` shim (a parallel adapter
   used only by ``plan_group_by_core``) is gone — group_by's three
   source shapes (``Array`` / ``Decs`` / ``DecsJoin``) all flow
   through the same ``SourceAdapter`` variant as every other planner.
   ``plan_group_by_core`` calls ``adapter_wrap_source_loop`` and
   ``adapter_wrap_invoke`` directly. The decs-join branch of
   ``adapter_wrap_source_loop`` carries the inline hash-collect +
   probe + per-pair result-lam bind body shared with
   ``emit_decs_join``.

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape (decs source)
     - Splice arm
     - Notes
   * - ``from_decs_template(type<T>).count()`` (bare, no chain ops, no predicate)
     - ``plan_decs_unroll`` → ``emit_decs_count_archsize``
     - Sums ``arch.size`` across archetypes; skips the per-entity walk entirely. Returns ``int`` — the ``+=`` site truncates past INT_MAX, so chain ``long_count()`` instead (different splice arm — see next row) when an int64-safe total is required.
   * - ``from_decs_template(type<T>).long_count()`` (bare); ``from_decs_template(type<T>)._where(P).count()`` / ``.long_count()``; ``from_decs_template(type<T>).count(P)`` / ``.long_count(P)``
     - ``plan_decs_unroll`` → ``emit_decs_accumulator``
     - Counter loop over the per-archetype walk. The bare ``long_count()`` shape does NOT use the ``arch.size`` shortcut above — that emitter returns ``int`` only. The ``count(P)`` / ``long_count(P)`` forms reach this arm via the Theme 4 root-cause fix to ``extract_decs_bridge``: ``forExpr.iteratorVariables`` is unpopulated when no chain op forces an inference pass over the bridge's inner for-loop, so previously bailed. The bridge now recovers iter names from ``mkTup.values`` (peeling the ``ExprRef2Value`` wrap).
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
   * - ``from_decs_template(...).reverse().take(N)[._select(F)].to_array()``
     - ``plan_decs_reverse`` (skip-into-tail; extended for terminal ``_select`` in PR #2915)
     - Whole-archetype skip + partial-archetype skip-counter + early-exit. When trailing ``_select(F)`` is captured (no pre-reverse ``_where`` / ``_select``), the K reversed survivors are projected into a separate buffer typed by termsel's call-result element type — saves the catch-all's N push_clones + full reverse_inplace + project pass. Bails (cascades to R1-R4) when termsel's call-result element type is unresolved at macro stage.
   * - ``from_decs_template(...).reverse()._select(F).first()``
     - ``plan_decs_reverse`` (Rb walk-and-overwrite scalar with terminal ``_select``)
     - Decs mirror of ``plan_reverse``'s Rb walk-and-overwrite scalar. Projection applies to the surviving ``last`` value at return.
   * - ``from_decs_template(...)._group_by(K)._select(reduce).to_array()``
     - pattern ``group_by_decs`` (sub-codegen ``plan_group_by_core``)
     - Shared bucket-reducer with the array path; differs only in the per-element source.
   * - ``from_decs_template(...)._group_by(K)._select(reduce)._where(P).to_array()`` / ``.count()``
     - pattern ``group_by_decs`` (sub-codegen ``plan_group_by_core``, trailing ``where`` as HAVING)
     - Decs mirror of the array-side post-aggregate HAVING. Same predicate-on-output-tuple semantics.
   * - ``from_decs_template(...)._group_by(K)._select(reduce)._order_by(K2).to_array()`` / ``._order_by_descending(K2).to_array()``
     - pattern ``group_by_decs`` (sub-codegen ``plan_group_by_core``, trailing ``order_by`` as ORDER BY)
     - Decs mirror of the array-side ORDER BY splice (Theme 3 Phase 2 C2). Shares the same in-place inline-cmp sort tail; only the bucket-fill source differs.
   * - ``from_decs_template(A)._join(from_decs_template(B), ka, kb, result)._group_by(K)._select(reduce).to_array()`` / ``.count()``
     - pattern ``group_by_decs`` with ``upstream_join`` slot (``isDecsJoin`` adapter; cross-arm — see *Decs-decs equi-join*)
     - Theme 3 Phase 1 cross-arm composition. ``emit_decs_join``'s hashB-collect + srcA-probe feeds ``plan_group_by_core``'s bucket update directly — one pass, no intermediate join array. Composes with the C2 trailing ``order_by`` extension above when applied to the join+group_by output.
   * - ``from_decs_template(...)._take_while(P).<...>`` / ``._skip_while(P).<...>``
     - ``plan_decs_unroll`` (predicate-driven ranges)
     - Hoists ``skippingName`` state across archetypes.
   * - ``from_decs_template(...).take(N)._where(P).<terminator>`` (counter / accumulator / early-exit / array / walk)
     - ``plan_decs_unroll`` (``postTakeWhereCond`` gate in ``emit_decs_terminator_lane``)
     - Decs mirror of the array-side ``postTakeWhereCond`` gate (Theme 2 5c). Take cap ticks unconditionally per element of the per-archetype walk; the trailing ``where`` gates only the per-element contribution. Predicate peels against ``chainInfo.finalBind`` so it composes with leading ``_where`` / ``_select`` chains and skip / skip_while / take_while ranges. Lands uniformly across all 6 ``emit_decs_*`` paths because the gate wraps ``spec.perElement`` once in the shared lane.

.. note::

   Several decs rows above (including this one) label the splice arm as ``plan_decs_unroll`` even though, post-PR-C, dispatch flows through the shared ``plan_loop_or_count`` pattern row (``emit_loop_or_count_lane`` → ``emit_loop_or_count_lane_decs`` → ``emit_decs_*``). The ``plan_decs_unroll`` label is retained for table-row consistency across the pre- and post-unification decs entries.

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
     - pattern ``decs_join_general`` (emit fn ``emit_decs_join``)
     - Hash-fill + probe; ``count`` bumped by bucket length per hit. No per-pair invoke.
   * - ``from_decs_template(A) |> _join(...) |> to_array()``
     - pattern ``decs_join_general`` (emit fn ``emit_decs_join``)
     - Hash-fill + probe; ``result`` lambda inlined at the push site (no per-pair invoke into ``join_impl``).
   * - ``from_decs_template(A) |> _join(...) |> _select(F) |> to_array()``
     - pattern ``decs_join_general`` (terminal ``_select``)
     - Single bind of the join result per matched pair, then projection.
   * - ``from_decs_template(A) |> _join(...) |> _where(P) |> count() / to_array()``
     - pattern ``decs_join_general`` (trailing ``_where``)
     - Bind join result, evaluate predicate, gate ``count++`` / ``push_clone``. Composes with the trailing ``_select`` form (filter then project, single bind per pair).
   * - ``from_decs_template(A) |> _where(P) |> _join(...)`` (leading ``_where``)
     - pattern ``decs_join_general`` (leading ``_where`` slot)
     - Pre-join filter on srcA, fused into the per-archetype probe as ``if (P(a)) { <probe> }`` — no intermediate filtered array. Same shared wrap as the array side (``build_join_standalone_pieces``).
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

Array-array equi-join
---------------------

``emit_array_join`` is the array-source mirror of ``emit_decs_join`` —
hashed equi-join over two array / iterator sources. Algorithm is
identical (collect srcb into ``table<KEY; array<TUPB>>`` in one pass,
then walk srca and probe via ``table.get``) but the per-source
iteration is a plain ``for (elem in src) { ... }`` loop instead of
``for_each_archetype + build_decs_inner_for``. Both sources bind as
invoke parameters (2-source wrap, mirrors ``Zip``). Same primitive
equi-key gate as the decs side; non-primitive keys cascade to
``join_impl_const``.

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape
     - Splice arm
     - Notes
   * - ``arrA |> _join(arrB, on, into) |> count()``
     - pattern ``array_join_general`` (emit fn ``emit_array_join``)
     - Hash-fill + probe; ``count`` bumped by bucket length per hit.
       No per-pair invoke (count-no-where bucket-length fast path).
   * - ``arrA |> _join(arrB, on, into)`` (no explicit terminator)
     - pattern ``array_join_general`` (emit fn ``emit_array_join``)
     - Implicit ``to_array`` lane: hash-fill + probe; ``result`` lambda
       inlined at the push site. Note: ``select``'s array overload
       returns ``array<...>`` directly, so the chain types as an array
       without a trailing ``to_array()`` call.
   * - ``arrA |> _join(arrB, ...) |> _select(F)`` or with trailing ``|> to_array()``
     - pattern ``array_join_general`` (terminal ``_select``)
     - Single bind of the join result per matched pair, then
       projection. ``resultType`` extracted from the select lambda's
       body type (not from ``selCall._type.firstType``, which may
       stay as an unresolved ``typedecl(...)`` when no ``to_array()``
       forces resolution).
   * - ``arrA |> _join(arrB, ...) |> _where(P) |> count() / to_array()``
     - pattern ``array_join_general`` (trailing ``_where``)
     - Bind join result, evaluate predicate, gate ``count++`` /
       ``push_clone``. Composes with the trailing ``_select`` form
       (filter then project, single bind per pair).
   * - ``arrA |> _where(P) |> _join(arrB, ...)`` (leading ``_where``)
     - pattern ``array_join_general`` (leading ``_where`` slot)
     - Pre-join filter on srcA, **fused into the probe loop** as
       ``if (P(a)) { <probe> }`` — no intermediate filtered-srcA array
       (vs. the tier-2 fallback, which materializes one). The optional
       ``lead_where`` slot precedes the ``join`` slot; a ``where`` after
       ``join`` is the separate trailing slot. Composes with the trailing
       ``_where`` / ``_select`` forms. Wrapping lives in the shared
       ``build_join_standalone_pieces``, so decs / XML inherit it.
   * - ``arrA |> _group_join(arrB, on, into)`` (+ optional leading ``_where``)
     - pattern ``join_general`` with the ``group_join`` literal (``isGroupJoin``)
     - C# GroupJoin (**outer**): one result row per srcA row — ``result(a,
       bucket)`` pushed once (no per-pair loop), plus ``if (!get(...)) { var
       empty; push result(a, empty) }`` so an unmatched srcA still surfaces
       (empty group). The join slot matcher is ``one_of ["join",
       "group_join"]``; ``isGroupJoin`` threads through
       ``build_join_standalone_pieces``, which rebinds the result lambda's 2nd
       param to the whole bucket (``array<TUPB>``) so the per-group aggregate
       runs inside the result. **Array sources only** — decs / XML group joins
       defer to tier-2 (their ``emit_join_hook`` returns ``null`` for
       ``group_join``); a trailing ``where`` / ``select`` / ``count`` over the
       group rows also defers.
   * - ``arrA |> _join(arrB, ...) |> _group_by(K) |> _select(reduce) |> count() / to_array()``
     - ``plan_group_by_core`` via ``SourceAdapter.ArrayJoin`` (chunk N+2)
     - Cross-arm composition. ``emit_group_by``'s Array branch
       recognizes a trailing ``join`` upstream of ``group_by_lazy``
       and builds an ``ArrayJoin`` adapter; ``plan_group_by_core``
       consumes it via ``adapter_wrap_source_loop``'s ``ArrayJoin``
       branch (plain ``for`` loops in lieu of ``for_each_archetype``).
       Same v1 constraints as the decs-side cross-arm: primitive
       equi-key, no segments between ``join`` and ``group_by_lazy``,
       HAVING defers to v2.

.. _linq_fold_xml_patterns:

XML-source patterns
===================

An ``unsafe(from_xml_node(node[, name], type<Row>))`` source folds through
``XmlAdapter`` (``modules/dasPUGIXML/daslib/linq_fold_xml.das``), loaded only when
the ``pugixml`` module is linked. Unlike a hard-coded source row, the adapter
**rides every pattern row** the array / decs planners expose — ``try_splice_patterns``
runs with no ``onlyRow`` restriction, and per-row ``requires`` predicates plus the
adapter's capability hooks decide what fuses. Three mechanics make the emitted loop
differ from the array and decs lanes.

**Single flat DOM walk, forward-only.** The adapter emits one ``while`` over the
node's child elements (``first_child`` / ``next_sibling``, or ``child(node, name)``
for the 3-arg named overload), like the array lane and unlike decs's two-level
archetype walk. But an XML node has **no random index**, so XML matches the
``non_array_source`` rows (e.g. the R-2b forward keep-last reverse-distinct) and is
**excluded** from the ``array_source``-only rows (Row 5 ``buffer_helper_dispatch``
direct-helper, R-2a backward-index reverse-distinct). Bare ``order_by`` / ``order``
therefore cascades to the ``fused_prefilter`` row (materialized buffer), not the
direct daslib helper — the same fall-through decs takes.

**Field-pruning (pass 2b).** Before emitting the per-element body the chain is
scanned (``XmlRowUsageScanner``) for the ``Row`` fields it actually reads. Only
those attributes are read — each via ``read_xml_field`` into a scalar local, with
the body's ``it.<field>`` rewritten to that local and the ``Row`` struct dropped
entirely. Unread fields are never touched, so a chain that reads only numeric
fields runs **alloc-free** (the per-string ``clone_string`` is the materialization
cost). Three outcomes:

- **Pruned** — body reads only ``it.<field>`` scalars: one ``let xf_<f> =
  read_xml_field(...)`` per referenced field, struct dropped.
- **Whole-row escape** — body references the bind ``it`` as a whole value
  (``to_array``, identity ``_select(_)``, pass-to-user-fn): falls back to the full
  ``build_xml_row``.
- **Guarded escape** — the whole row escapes only inside a bare ``if (cond) { … }``:
  the predicate's fields are read cheaply via ``peek_xml_field`` (borrowed
  ``string#``, no clone) and the full ``build_xml_row`` runs **only for matching
  elements**.

**Deferred materialization.** A buffered reducer (order / take, ``distinct_by``)
holds ``(key, xml_node)`` *handle surrogates* rather than built rows, and runs
``build_xml_row`` only for the K survivors at return — ``defers_materialization()``
is true, ``current_handle_expr`` is the per-element ``xcur`` node, and
``materialize_handle`` emits the deferred ``build_xml_row``. A 1000-element document
feeding ``order_by(K).take(10)`` builds 10 rows, not 1000.

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape (XML source)
     - Splice arm
     - Notes
   * - ``…<terminator>`` with ``_where`` / ``_select`` (count / long_count / sum / average / min / max / any / all / contains / first / last / single / element_at / take / take_while / skip_while / to_array)
     - ``loop_or_count_general`` (``XmlAdapter`` swap)
     - The base lane — the same emit archetypes as the array side, over the field-pruned DOM walk. ``take`` / ``where`` / ``select`` fuse into the body.
   * - ``._order_by(K).first()`` / ``.first_or_default()``
     - ``plan_order_family`` (streaming-min) + deferral
     - One handle held in ``var best``; ``build_xml_row`` runs once at return.
   * - ``._order_by(K).take(N).to_array()`` / ``…._select(F).to_array()``
     - ``plan_order_family`` (bounded-heap) + deferral
     - Heap of N ``(key, node)`` surrogates; ``build_xml_row`` (and any terminal ``_select``) runs ≤N times at return.
   * - ``._order_by(K).to_array()`` / ``._where(P)._order_by(K).to_array()``
     - ``plan_order_family`` (``fused_prefilter``) + deferral
     - ``array_source`` Row 5 excludes XML, so bare order cascades to the materialized-buffer row — the buffer holds node handles, rows built for survivors only.
   * - ``._distinct_by(K).to_array()`` / ``._distinct_by(K)._order_by(K2)…``
     - ``plan_distinct`` / ``plan_order_family`` + deferral
     - Dedup set over the key; the kept slot stores the node handle (``distinct_by`` defers; plain ``distinct`` over the whole row materializes per element).
   * - ``.reverse()._distinct[_by](K).to_array()``
     - ``plan_reverse`` R-2b (``non_array_source``) → ``emit_reverse_distinct_forward_keeplast``
     - Forward keep-last table-overwrite (no backward index); the slot stores the ``xml_node`` handle, ``build_xml_row`` (field-pruned to the key) runs for the K survivors. The one row shared by decs / iterators / XML.
   * - ``from_xml_node(…) |> _join(arrB, ka, kb, result)`` (+ optional leading / trailing ``_where``, trailing ``_select``; count / to_array / iterator)
     - pattern ``join_general`` → ``XmlAdapter.emit_join_hook``
     - Hashed equi-join: srcB (an **in-memory array**) collected into ``table<KEY; array<TUPB>>``, probed from the field-pruned DOM walk. Primitive equi-key only. Mirrors ``emit_array_join`` with srcA = the XML node.
   * - ``from_xml_node(…) |> _join(arrB, …) |> _group_by(K) |> _select(reduce) |> to_array()`` / ``.count()``
     - ``plan_group_by_core`` via ``XmlJoinAdapter``
     - Cross-arm: the join's per-pair result feeds the bucket update directly — one pass, no intermediate join array. Same v1 constraints as the array / decs cross-arm (primitive key, no segments between ``join`` and ``group_by``, HAVING defers to v2).
   * - ``from_xml_node(…) |> _group_by(K) |> _select(reduce) |> to_array()`` (+ HAVING / trailing ``order_by``)
     - ``plan_group_by_core`` (``XmlAdapter`` via ``build_group_by_adapter``)
     - Per-key bucket reducer over the DOM walk; shares the array path's reducer dispatch and the trailing HAVING / ORDER BY extensions.
   * - ``source |> _select(f) |> <order/distinct/take>``
     - leading-``_select`` absorption (``ProjectedSourceAdapter`` wrap)
     - The leading projection is absorbed into the source walk and **field-pruning is preserved** — ``f``'s ``it.<field>`` reads still reach the materializer (see *Pre-dispatch normalizations*).

**Defers to tier-2** (the ``XmlAdapter`` hook returns null, so the chain cascades):

- **Group join** (``_group_join`` / ``join … into``) — ``emit_join_hook`` returns
  null for the ``group_join`` literal.
- **Non-primitive join keys** / **non-array srcB** — the same gate as the array /
  decs join (tuple keys cascade to ``join_impl``).
- **Correlated nested-collection flatten** (``from o … from l in o.lines``) —
  ``from_xml_node`` reads scalar attributes only; there is no nested collection to
  flatten.
- **Mixed-source operators** (``union`` / ``except`` / ``intersect`` / ``concat``)
  — fall back exactly as for array / decs sources.

Zip patterns
============

.. list-table::
   :header-rows: 1
   :widths: 35 25 40

   * - Chain shape
     - Splice arm
     - Notes
   * - ``zip(a, b)._select(F).sum()`` / ``.count()`` / ``.average()``
     - pattern ``zip_general`` (emit fn ``emit_zip``)
     - Fuses to a single index-loop over the shorter side.
   * - ``zip(a, b, c)._select(F).<terminator>``
     - pattern ``zip_general`` (emit fn ``emit_zip``)
     - Three-source zip; same loop shape with three reads per iteration.
   * - ``zip(a, b, sel).<terminator>`` (3-arg, with selector lambda)
     - pattern ``zip_general`` (3-arg pre-lowered)
     - Theme 1 (audit 7a). The 3-arg form ``zip(a, b, sel)`` is pre-lowered by ``plan_zip`` to ``zip(a, b) |> _select(sel-as-tuple)`` before per-arm matching, so the standard zip+select fusion fires — the natural ``zip(xs, ys, $(x, y) => x * y) |> sum()`` dot-product idiom splices instead of cascading.
   * - ``zip(a, b)._where(P)._select(F).<terminator>``
     - pattern ``zip_general`` (chain ops via head c_chain + range slots)
     - ``where`` / ``select`` / ``take`` / ``skip`` / ``take_while`` / ``skip_while`` between zip and the terminator are all fused.
   * - ``zip(a, b).first()`` / ``.first_or_default()`` / ``.aggregate(...)``
     - pattern ``zip_general`` (early-exit / accumulator lanes delegate to emit_early_exit_lane / emit_accumulator_lane)
     - Early-exit terminator on the zipped pair.
   * - ``zip(a, b)._select(F).count(P)`` / ``.long_count(P)``
     - pattern ``zip_general`` (counter with separate predicate gate)
     - The 2-arg ``count(P)`` / ``long_count(P)`` form is captured into a dedicated counter-predicate gate emitted around ``acc++`` *inside* the upstream where/select wrap, so eager ``where(W).select(F).count(P)`` ordering is preserved (W filters first, then F runs once per surviving element, then P decides whether to count). With ``_select``, the predicate peels against the projected value via a ``vproj`` bind. Length-shortcut is suppressed when ``P`` is present (the counter loop runs).
   * - ``zip(a, b)[._select(F)|._where(P)|...].reverse().<terminator>``
     - pattern ``zip_general`` (trailing ``reverse`` slot)
     - Theme 8 (audit C4). ``reverse`` accepted as the last chain op between zip's chain and the terminator. Array lane emits ``_::reverse_inplace($i(bufName))`` before return; counter / accumulator (sum/min/max/avg) / ``any`` / ``all`` / ``contains`` lanes treat reverse as a no-op (mathematical identity). **Bails** (cascades) on ``first`` / ``first_or_default`` (NOT identity under reverse) and when ``reverse`` is not the last chain op (anything after would see the reversed stream and change semantics vs cascade).

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

Decs-bridge fall-off diagnostic
-------------------------------

When a ``from_decs_template`` source survives ``_fold`` dispatch
without any tier-1 planner (decs or array-side) claiming it, the
bridge materializes into a temp ``res`` array before
``fold_linq_default`` runs on top — an EXTRA allocation beyond
whatever cascade follows. ``LinqFold.visit`` detects this case right
before falling through to ``fold_linq_default``: it destructures
``flatten_linq(call.arguments[0])`` into ``(top, calls)`` and fires
only when ``calls`` is non-empty (a real cascade is about to run) and
``extract_decs_bridge(top)`` is non-null (the source IS a
``from_decs_template`` bridge). Bare
``_fold(from_decs_template(...))`` with no chain ops is skipped —
there's no cascade, just the bridge's own materialization. When
fired, a ``*warning*`` goes to the compiler log naming the call
site::

    user.das:42:8: *warning* `_fold`: from_decs_template source
    survived dispatch — no `plan_decs_*` arm claimed this chain, so
    the bridge materializes a temp `res` array and the tier-2 cascade
    runs on the materialized buffer. Rewrite the chain to a
    recognized decs shape (see
    doc/source/reference/linq_fold_patterns.rst), or suppress with
    `options _no_linq_perf_warn = true`.

The fix is usually to reorder ops so the chain matches a row in the
Decs section above (e.g. push ``_select`` past ``_skip_while`` /
``_take_while`` since their predicates run on the source tuple, not
the projected value). Suppress per file with ``options
_no_linq_perf_warn = true`` for tests that intentionally exercise
cascade behavior as regression guards.

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
