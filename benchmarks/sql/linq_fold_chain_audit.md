# `_fold` chain coverage audit — silent fall-off catalog

Generated 2026-05-23 from `0a2da407f`. Probe files live under
`/tmp/audit_probes/` (re-runnable; see "How to re-run" at the bottom).

## Status — what this audit has closed

**Theme 1 (terminal `_select` extension) — landed 2026-05-24** (`59c4f3f98`):

- **1a, 1e + motivating** (`plan_order_family` / `plan_decs_order_family`): terminal `_select` accepted after `take(N)`. Bounded-heap holds the raw element; projection runs ≤K times at return.
- **plan_reverse / plan_decs_reverse**: terminal `_select` accepted after `reverse [+ take(N)]`. Closes the natural "filter, reverse for newest-first, take K, project" idiom. NOT closed: the `reverse + _select + take` ordering (2c / 2e exact shape) — user must reorder to `reverse + take + _select`.
- **8b** (`plan_decs_join`): single trailing `_select` between `_join` and the implicit `to_array`. Substitutes via a let-bound join-result + projection.
- **7a** (`plan_zip`): 3-arg `zip(a, b, sel)` pre-lowered to 2-arg `zip(a, b) |> _select(sel-as-tuple)` — the natural dot-product idiom now splices.

Coverage extension: 1395 → 1415 linq tests (10 new tests in `tests/linq/test_linq_fold_terminal_select.das`).

**Theme 2 (trailing `_where` / HAVING) — landed 2026-05-24**:

- **8a, C6** (`plan_decs_join`): single trailing `_where` between `_join` and the terminator. Predicate references join-result fields; emission binds the result once per pair and gates `count++` / `push_clone`. Composes with the terminal `_select` from Theme 1.
- **4a** (`plan_group_by`) + **4e** (`plan_decs_group_by` via shared `plan_group_by_core`): trailing `_where` AFTER `_select(reducer)`, i.e. SQL HAVING on the post-aggregate tuple. Binds the constructed output once per bucket and gates buf-emit / count-emit. Distinct from the existing `having_` slot (which is pre-select and can lift hidden reducer slots) — both can fire on the same chain.
- **5c** (`plan_loop_or_count` across all 4 lanes — counter / accumulator / early-exit / array): `take(N)._where(p).terminator` accepted. Take cap ticks unconditionally per element; trailing `_where` gates only the per-element contribution, preserving the "first N elements, then keep matching" semantic that auto-rewriting can't reproduce.

Coverage extension: 1415 → 1437 linq tests (12 new tests in `tests/linq/test_linq_fold_theme2_trailing_where.das`).

**Theme 4 (2-arg terminator predicates) — landed 2026-05-24** (bundled with Theme 5 in the same PR):

- **3c** (`plan_distinct` + `plan_decs_distinct`): trailing `_distinct().count(P)` / `_distinct_by(K).count(P)` / `.long_count(P)`. Dedup remains unconditional so `distinct_by` keeps FIRST occurrence per key; a separate counter increments only when `P` matches that first occurrence — matches tier-2 `distinct.count(P)` semantics (distinct-then-filter, not filter-then-distinct). When a `_select` precedes `_distinct`, the predicate peels against the post-projection element so `_select(F)._distinct().count(P)` binds `P`'s parameter to the projected value.
- **`plan_zip`**: trailing `zip(a, b)[._select(F)].count(P)` / `.long_count(P)`. Predicate AND's into `whereCond` after the chain walk so the counter loop runs (length-shortcut is suppressed once `P` is present).
- **`plan_decs_unroll` bare-chain `count()` / `count(P)`** (separate root-cause fix to `extract_decs_bridge`): `from_decs_template(...).count()` / `.count(P)` over a no-chain source was bailing because `forExpr.iteratorVariables` is unpopulated when no chain op forces an inference pass over the bridge's inner for-loop. The bridge now recovers iter names from `mkTup.values` (peeling the `ExprRef2Value` wrap), so both the Slice 1 `arch.size` shortcut and the `emit_decs_accumulator` 2-arg `count(P)` path are reachable on bare chains.

**Theme 5 (`_order_by(K).reverse()` normalization) — landed 2026-05-24** (same PR):

- **1b, 2b**: pre-planner `normalize_order_reverse` pass runs from every `plan_*order_family` / `plan_*reverse` planner immediately after `flatten_linq`. Adjacent `(order_by|order_by_descending|order|order_descending, reverse)` pairs swap to the flipped variant and the `reverse` element is erased. The `ExprCall` arg list is identical between ascending/descending order variants, so no AST clone is needed — only the `LinqCall` metadata pointer is repointed to the flipped registry entry. Iterative: `_order_by(K).reverse().reverse()` collapses to `_order_by(K)` in two passes; `_order_by_descending(K).reverse()` flips to `_order_by(K)`.

Coverage extension across both themes: 1437 → 1463 linq tests (14 new in `tests/linq/test_linq_fold_theme45_quick_wins.das`); the existing `test_unroll5c_select_distinct_count_pred_parity` + `_long_count_pred_parity` parity tests in `test_linq_from_decs.das` now exercise the splice path instead of bailing.

Still open (queued for the next session per the cross-cutting findings below):

- Theme 3 — cross-arm composition (5 of 6 composition probes; HIGHEST impact, LARGE effort).
- Themes 6, 7, 8 — see "Cross-cutting findings" section.


The audit catalogs **silent fall-off** in `daslib/linq_fold.das`: chains where a
natural user phrasing makes the splice arm return null and the planner falls
back to the slow default cascade (`fold_linq_default`) — without any warning.
Every row below shows the post-macro `ast_dump` of one probe, classified as
SPLICE-FIRES (single-pass specialized loop) or FALLS-OFF (cascade of
`__::linq\`helper\`` calls plus intermediate `array<...>` allocations).

Each "FALLS-OFF" row names the bail line in `linq_fold.das` and proposes
either a cheap user-side rewrite or an arm extension. The audit does NOT
change any code — every finding is a follow-up TODO.

---

## Motivating example — closest 10 sounds

The audit was prompted by this user scenario: "I have an array of sounds with
`(id, position)` and a head position. Give me the ids of the 10 closest." The
natural translation looks like:

```das
let closest_ids <- _fold(each(sounds)
    ._order_by(distance(_.position, head))
    .take(10)
    ._select(_.id)
    .to_array())
```

That chain SILENTLY FALLS OFF the splice. The arm that should fire is
`plan_order_family` (linq_fold.das:1234), which emits a bounded-heap walk
holding at most N elements — but its accept list is `[where_*] order_*
[take|first]?`, and the `._select(_.id)` between `take` and `to_array` is not
in that list. The chain falls through line 1284's `else { return null }` and
into the default 3-pass cascade.

**With `_select` (the natural form)** — `/tmp/audit_probes/motivating_with_select.das`:

```das
return <- invoke($(var source : iterator<Sound&>) : array<int> {
    var pass_0 <- __::linq`order_by_to_array(source, $(_) { return distance(_.position, head); });
    __::linq`take_inplace(pass_0, 10);
    var pass_2 <- __::linq`select(pass_0, $(_) { return _.id; });
    __::builtin`finalize(pass_0);
    return <- pass_2;
}, __::builtin`each(sounds))
```

Default cascade: full sort over N elements + allocation, then truncate, then
another allocation for the projection.

**Without `_select` (the splice-eligible form)** — `/tmp/audit_probes/motivating_without_select.das`:

```das
return <- invoke($(source : array<Sound>) : array<Sound> {
    var order_buf : array<Sound>;
    for (it in source)
        if (length(order_buf) < 10)
            push_clone(order_buf, it)
            spliced_push_heap(order_buf, $(v1, v2) => less(distance(v1.position, head), distance(v2.position, head)))
        elif (less(distance(it.position, head), distance(order_buf[0].position, head)))
            spliced_pop_heap(order_buf, ...)
            order_buf[length(order_buf) - 1] = it
            spliced_push_heap(order_buf, ...)
    order_inplace(order_buf, ...)
    return <- order_buf
}, sounds)
```

Single-pass bounded heap holding ≤10 elements. No N-sized allocation.

The rest of this document is the systematic version of that comparison
across every `plan_*` arm in the splice machinery.

---

## Chain 1 — `plan_order_family` / `plan_decs_order_family`

**Accepts**: `[where_*] order_* [take(N)|first|first_or_default]?` (linq_fold.das:1234 array, :4547 decs)
**Common bails**: select-anywhere (line 1284 / 4594 fall-through), where-after-order (line 1252 / 4566), reverse-in-chain (line 1284 / 4594 fall-through), explicit comparator on bare `order` / `order_descending` (line 1264 / 4575)

### 1a — Closest 10 sounds, return ids (array)

**Probe** (`/tmp/audit_probes/chain1_1a.das`):
```das
def probe_1a(sounds : array<Sound>; head : float3) : array<int> {
    unsafe {
        return <- _fold(each(sounds)._order_by(distance(_.position, head)).take(10)._select(_.id).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Sound&>) : array<int> {
    var pass_0 <- __::linq`order_by_to_array(source, $(_) { return distance(_.position, head); });
    __::linq`take_inplace(pass_0, 10);
    var pass_2 <- __::linq`select(pass_0, $(_) { return _.id; });
    __::builtin`finalize(pass_0);
    return <- pass_2;
}, __::builtin`each(sounds))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_select(_.id)` between `take(10)` and `to_array()` is not in plan_order_family's allowed call list (linq_fold.das:1284 fall-through). The cascade fully sorts N elements into `pass_0`, then truncates, then allocates a fresh `array<int>` for the projection. Rewrite: split into two steps — `let top <- _fold(...take(10).to_array()); let ids <- [for (s in top); s.id]`. Extending the arm to accept a terminal `_select` after `take(N)` / `first` / `first_or_default` is the highest-impact fix since the bounded heap already holds ≤N elements; a final projection at emission time is essentially free.

### 1b — Top 5 scores descending via order + reverse (array)

**Probe** (`/tmp/audit_probes/chain1_1b.das`):
```das
def probe_1b(scores : array<Score>) : array<Score> {
    unsafe {
        return <- _fold(each(scores)._order_by(_.score).reverse().take(5).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Score&>) : array<Score> {
    var pass_0 <- __::linq`order_by_to_array(source, $(_) { return _.score; });
    __::linq`reverse_inplace(pass_0);
    __::linq`take_inplace(pass_0, 5);
    return <- pass_0;
}, __::builtin`each(scores))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `reverse()` between `_order_by` and `take` is not in the accepted vocabulary (line 1284 fall-through). The natural way for a user to ask for "top 5 descending" via the ascending key is `order_by(...).reverse().take(5)`; the splice path requires the user to know `_order_by_descending(...).take(5)` instead. The arm could recognize `_order_by(k).reverse()` and rewrite it to the `_order_by_descending(k)` form before the comparator-emission step.

### 1c — Where-after-order (array)

**Probe** (`/tmp/audit_probes/chain1_1c.das`):
```das
def probe_1c(employees : array<Employee>) : array<Employee> {
    unsafe {
        return <- _fold(each(employees)._order_by(_.salary)._where(_.dept == "eng").take(10).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Employee&>) : array<Employee> {
    var pass_0 <- __::linq`order_by_to_array(source, $(_) { return _.salary; });
    var pass_1 <- __::linq`where_(pass_0, $(_) { return _.dept == "eng"; });
    __::builtin`finalize(pass_0);
    __::linq`take_inplace(pass_1, 10);
    return <- pass_1;
}, __::builtin`each(employees))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_where` after `_order_by` is explicitly rejected by `if (hasOrder) return null` at line 1252. Sorting first and then filtering is genuinely wasteful (sorts ~N elements just to drop most of them); the rewrite is mechanical — move the `_where` before the `_order_by`. Worth a lint suggestion rather than a splice extension: keeping the post-sort `_where` semantically correct in the splice would require either re-running the filter inside the bounded heap or re-allocating after the sort, both of which lose the cascade's correctness while not actually buying anything over the trivial user rewrite.

### 1d — Closest 10 sounds, baseline (array)

**Probe** (`/tmp/audit_probes/chain1_1d.das`):
```das
def probe_1d(sounds : array<Sound>; head : float3) : array<Sound> {
    unsafe {
        return <- _fold(each(sounds)._order_by(distance(_.position, head)).take(10).to_array())
    }
}
```

**Generated** (trimmed):
```das
return <- invoke($(source : array<Sound>) : array<Sound> {
    var order_buf : array<Sound>;
    for (it in source)
        if (length(order_buf) < 10)
            push_clone(order_buf, it)
            spliced_push_heap(order_buf, ...)
        elif (less(distance(it.position, head), distance(order_buf[0].position, head)))
            spliced_pop_heap(order_buf, ...)
            order_buf[length(order_buf) - 1] = it
            spliced_push_heap(order_buf, ...)
    order_inplace(order_buf, ...)
    return <- order_buf;
}, sounds)
```

**Classification**: SPLICE-FIRES — bounded-heap arm (line 1300 `useBoundedHeap`).

**Conclusion**: Baseline confirms the bounded-heap splice path — O(N log K) heap maintenance over the walk, no full-N sort, no full-N allocation.

### 1e — Closest 10 sounds, return ids (decs)

**Probe** (`/tmp/audit_probes/chain1_1e.das`): same shape as 1a over `from_decs_template(type<DecsSound>)`.

**Generated** (trimmed):
```das
return <- invoke($(var source : iterator<tuple<id:int; position:float3>>) : array<int> {
    var pass_0 <- __::linq`order_by_to_array(source, ...);
    __::linq`take_inplace(pass_0, 10);
    var pass_2 <- __::linq`select(pass_0, ...);
    __::builtin`finalize(pass_0);
    return <- pass_2;
}, invoke($() : iterator<tuple<...>> {
    var res : array<tuple<...>>;
    for_each_archetype(..., $(arch) {
        for (ds_id, ds_position in get_ro(arch, "ds_id", type<int>), get_ro(arch, "ds_position", type<float3>))
            push(res, tuple(ds_id, ds_position));
    });
    return <- __::linq`to_sequence(res);
}))
```

**Classification**: FALLS-OFF — default cascade, with the **double penalty** that the decs source bridge eagerly materializes ALL rows into `res` before the array cascade even starts.

**Conclusion**: Same trailing `_select` mismatch as 1a (line 4594 fall-through in `plan_decs_order_family`). Worse than 1a because when `plan_decs_order_family` returns null, the `from_decs_template` bridge has no other splice to bind it to — it degenerates to full materialization of every archetype row into a temp `res`, wrapped in `to_sequence` for the array cascade. Two allocations of the full row set + one projection. Extension fix: same as 1a — accept a terminal `_select` after `take(N)` / `first` / `first_or_default` in `plan_decs_order_family`.

### 1f — Closest 10 sounds, baseline (decs)

**Probe** (`/tmp/audit_probes/chain1_1f.das`): same shape as 1d over decs.

**Generated** (trimmed):
```das
return <- invoke($() : array<tuple<id:int; position:float3>> {
    var decs_buf : array<tuple<...>>;
    for_each_archetype(..., $(arch) {
        for (ds_id, ds_position in get_ro(...), get_ro(...))
            if (length(decs_buf) < 10)
                push_clone(decs_buf, tuple(ds_id, ds_position))
                spliced_push_heap(decs_buf, ...)
            elif (less(distance(ds_position, head), distance(decs_buf[0].position, head)))
                spliced_pop_heap(decs_buf, ...)
                decs_buf[length(decs_buf) - 1] = tuple(...)
                spliced_push_heap(decs_buf, ...)
    });
    order_inplace(decs_buf, ...);
    return <- decs_buf;
})
```

**Classification**: SPLICE-FIRES — bounded-heap arm fused into a single `for_each_archetype` (line 4609 `useBoundedHeap`).

**Conclusion**: Decs baseline confirms the bounded-heap path through the archetype walk: ≤10 push_clones to `decs_buf`, no eager materialization.

### Chain 1 — follow-up TODOs

- **Highest impact**: extend `plan_order_family` (line 1234) and `plan_decs_order_family` (line 4547) to accept a terminal `_select` after `take(N)` / `first` / `first_or_default`. The bounded-heap arm already holds at most N elements; emitting the projection during the final `order_inplace` walk is essentially free. The "closest N, return projected field" idiom is extremely natural.
- Recognize `_order_by(k).reverse()` and rewrite to `_order_by_descending(k)` (and dual) before the comparator-emission step. Currently `reverse()` mid-chain is a hard bail (line 1284 / 4594).
- Lint suggestion (style rule, not a splice extension): `_order_by(...)._where(...)` should reorder to `_where(...)._order_by(...)`. Sorting before filtering is wasteful in any execution mode.
- Decs FALLS-OFF cases are doubly penalized — the bridge's eager-materialize default lands behind every `plan_decs_order_family` bail. Worth a dedicated diagnostic when this path is hit.

---

## Chain 2 — `plan_reverse` / `plan_decs_reverse`

**Accepts**: `[where_*][select?] reverse [take(N)]? [count|first|first_or_default]?` (linq_fold.das:1764 array, :4802 decs)
**Common bails**: `where_` / `select` AFTER reverse or AFTER select (line 1797-1800 / 4833-4838 — `seenSelect || hasReverse` guards), order-anywhere (line 1813 / 4851 fall-through), double-reverse (line 1804 / 4843), `take(N)` paired with a separate terminator (line 1816 / 4854 bail).

Note: `_where → _select → reverse → take → to_array` (select BEFORE reverse, no further select/where after) is ACCEPTED — the guards prevent `where AFTER select` and `select AFTER select`, not `select BEFORE reverse`.

### 2a — Reverse + distinct_by (array)

**Probe** (`/tmp/audit_probes/chain2_2a.das`):
```das
def probe_2a(events : array<Event>) : array<Event> {
    unsafe {
        return <- _fold(each(events).reverse()._distinct_by(_.kind).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Event&>) : array<Event> {
    var pass_0 <- __::linq`reverse_to_array(source);
    __::linq`distinct_by_inplace(pass_0, $(_) { return _.kind; });
    return <- pass_0;
}, __::builtin`each(events))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `distinct_by` after `reverse` is not in plan_reverse's vocabulary (line 1813 fall-through), and the call appears before any recognized terminator so plan_reverse cannot peel one. plan_distinct in turn doesn't model `reverse` in its prelude either (line 1999 fall-through). Result: full `reverse_to_array` allocation + `distinct_by_inplace`. Cheap user rewrite: `_distinct_by` is order-stable, so the user could write `_distinct_by(_.kind).reverse()` — but that's a behavior change (different element survives per kind). A real splice extension would need `plan_reverse` to recognize `reverse + distinct_by` and emit a single-pass walk that retains the LAST element per key, then reverses.

### 2b — Order then reverse (array)

**Probe** (`/tmp/audit_probes/chain2_2b.das`):
```das
def probe_2b(events : array<Event>) : array<Event> {
    unsafe {
        return <- _fold(each(events)._order_by(_.ts).reverse().to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Event&>) : array<Event> {
    var pass_0 <- __::linq`order_by_to_array(source, $(_) { return _.ts; });
    __::linq`reverse_inplace(pass_0);
    return <- pass_0;
}, __::builtin`each(events))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: Symmetric to 1b. `plan_reverse` bails at `_order_by` (line 1813 fall-through); `plan_order_family` bails at `reverse()` (line 1284 fall-through). Neither splice fires. User rewrite: `_order_by_descending(_.ts).to_array()`. Same TODO as chain 1: recognize `_order_by(k).reverse()` and normalize to `_order_by_descending(k)` before either planner gets it.

### 2c — Select-after-reverse (array)

**Probe** (`/tmp/audit_probes/chain2_2c.das`):
```das
def probe_2c(users : array<User>) : array<string> {
    unsafe {
        return <- _fold(each(users)._where(_.active).reverse()._select(_.name).take(5).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<User&>) : array<string> {
    var pass_0 <- __::linq`where_to_array(source, $(_) { return _.active; });
    __::linq`reverse_inplace(pass_0);
    var pass_2 <- __::linq`select(pass_0, $(_) { return _.name; });
    __::builtin`finalize(pass_0);
    __::linq`take_inplace(pass_2, 5);
    return <- pass_2;
}, __::builtin`each(users))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_select` AFTER `reverse` trips the `hasReverse` half of the guard at line 1800 (`if (hasReverse || seenSelect) return null` inside the select arm). User rewrite to fire the splice is non-obvious — move the select before reverse: `_where(_.active)._select(_.name).reverse().take(5).to_array()` (which DOES splice, since select BEFORE reverse is accepted). Extension fix: in `plan_reverse`, when a terminal `_select(...)` follows `reverse` (or `reverse + take(N)`), treat it as a projection applied on the buffer return — same shape as the chain 1 terminal-projection extension.

### 2d — Where + reverse + take (array baseline)

**Probe** (`/tmp/audit_probes/chain2_2d.das`):
```das
def probe_2d(events : array<Event>) : array<Event> {
    unsafe {
        return <- _fold(each(events)._where(_.active).reverse().take(5).to_array())
    }
}
```

**Generated** (trimmed):
```das
return <- invoke($(source : array<Event>) : array<Event> {
    var buf : array<Event>;
    for (it in source)
        if (it.active)
            push_clone(buf, it);
    reverse_inplace(buf);
    resize(buf, length(buf) > 5 ? 5 : length(buf));
    return <- buf;
}, events)
```

**Classification**: SPLICE-FIRES — R5 buffer + reverse_inplace + resize arm.

**Conclusion**: Baseline confirms the standard plan_reverse buffer arm — one filtered push pass, one in-place reverse, one resize to take(N).

### 2e — Select-after-reverse (decs)

**Probe** (`/tmp/audit_probes/chain2_2e.das`): same shape as 2c over decs.

**Generated** (trimmed): identical structure to 2c, plus eager decs bridge materializing `res` first.

**Classification**: FALLS-OFF — default cascade, doubled by eager decs materialization.

**Conclusion**: Same root cause as 2c (line 4838 in plan_decs_reverse). Three full-N allocations: `res`, `pass_0` (where filtered), `pass_2` (selected). Same extension fix as 2c.

### 2f — Where + reverse + take (decs baseline)

**Probe** (`/tmp/audit_probes/chain2_2f.das`): same shape as 2d over decs.

**Generated** (trimmed):
```das
return <- invoke($() : array<tuple<...>> {
    var decs_buf : array<tuple<...>>;
    for_each_archetype(..., $(arch) {
        for (de_id, de_kind, de_ts, de_active in get_ro(...), get_ro(...), get_ro(...), get_ro(...))
            if (de_active)
                push_clone(decs_buf, tuple(de_id, de_kind, de_ts, de_active));
    });
    reverse_inplace(decs_buf);
    resize(decs_buf, 5 < length(decs_buf) ? 5 : length(decs_buf));
    return <- decs_buf;
})
```

**Classification**: SPLICE-FIRES — plan_decs_reverse buffer + reverse_inplace + resize.

**Conclusion**: Decs-side mirror of 2d.

### Chain 2 — follow-up TODOs

- **Highest impact**: accept a terminal `_select(...)` after `reverse` (and after `reverse + take(N)`) in BOTH `plan_reverse` (line 1764) and `plan_decs_reverse` (line 4802). The current bail catches the natural "filter-then-reverse-for-newest-first-then-project" idiom.
- Recognize `reverse + distinct_by` as a fused walk retaining the LAST element per key, single archetype walk for decs and single source walk + buffer for arrays.
- Recognize `_order_by(k).reverse()` and rewrite to `_order_by_descending(k)` (chain 1 also wants this).
- Decs FALLS-OFF cases hit the same eager-materialize double penalty as chain 1.

---

## Chain 3 — `plan_distinct` / `plan_decs_distinct`

**Accepts**: `[where_*][select?] (distinct|distinct_by) [take(N)]? [count|long_count|sum|to_array]?` (linq_fold.das:1945 array, :5049 decs)
**Common bails**: `where_` AFTER `select` or `distinct` (line 1979 / 5085), second `select` (line 1982 / 5090, seenSelect), order-anywhere (line 1998 / 5108 fall-through), reverse-anywhere (same), 2-arg `count` / `long_count` / `sum` with predicate (line 1953-1957 / 5057-5063 — terminator-peel only fires for 1-arg form), distinct-after-distinct (line 1986 / 5095), `take(N)` paired with non-implicit terminator (line 2002 / 5112).

Note: `_select(_.field)._where(...) → distinct → count` flips the order to `_where AFTER _select` which DOES hit the seenSelect bail (line 1979). `_where → _select → distinct → count` is ACCEPTED.

### 3a — Select then where then distinct then count (array)

**Probe** (`/tmp/audit_probes/chain3_3a.das`):
```das
def probe_3a(events : array<EventA>) : int {
    unsafe {
        return _fold(each(events)._select(_.email)._where(length(_) > 0).distinct().count())
    }
}
```

**Generated**:
```das
return invoke($(var source : iterator<EventA&>) : int {
    var pass_0 <- __::linq`select_to_array(source, $(_) { return _.email; });
    var pass_1 <- __::linq`where_(pass_0, $(_) { return length(_) > 0; });
    __::builtin`finalize(pass_0);
    __::linq`distinct_inplace(pass_1);
    var pass_3 = __::linq`count(pass_1);
    __::builtin`finalize(pass_1);
    return <- pass_3;
}, __::builtin`each(events))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_where` AFTER `_select` trips the seenSelect bail at line 1979. 4-pass cascade for what could be a single-walk streaming dedup (no buffer at all because terminator is `count`). `_where → _select → distinct → count` is splice-eligible; the user just has to swap the where and select.

### 3b — Distinct then order (array)

**Probe** (`/tmp/audit_probes/chain3_3b.das`):
```das
def probe_3b(rows : array<Row>) : array<Row> {
    unsafe {
        return <- _fold(each(rows)._distinct_by(_.user_id)._order_by(_.ts).to_array())
    }
}
```

**Generated**:
```das
return <- invoke($(var source : iterator<Row&>) : array<Row> {
    var pass_0 <- __::linq`distinct_by_to_array(source, $(_) { return _.user_id; });
    __::linq`order_by_inplace(pass_0, $(_) { return _.ts; });
    return <- pass_0;
}, __::builtin`each(rows))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_order_by` after `_distinct_by` is unrecognized in plan_distinct (line 1998 fall-through), and plan_order_family doesn't model `_distinct_by` as an upstream call (line 1284). Cascade materializes the distinct-by result, then in-place sorts. The two ops don't commute (distinct-then-sort ≠ sort-then-distinct), so no obvious user rewrite. Extension fix: plan_order_family could recognize an upstream `_distinct_by(keyFn)` and emit a fused walk that hash-tracks seen keys while feeding survivors into the bounded heap.

### 3c — Distinct then predicated count (array)

**Probe** (`/tmp/audit_probes/chain3_3c.das`):
```das
def probe_3c(events : array<EventR>) : int {
    unsafe {
        return _fold(each(events)._distinct_by(_.region)._count(_.active))
    }
}
```

**Generated**:
```das
return invoke($(var source : iterator<EventR&>) : int {
    var pass_0 <- __::linq`distinct_by_to_array(source, $(_) { return _.region; });
    var pass_1 = __::linq`count(pass_0, $(_) { return _.active; });
    __::builtin`finalize(pass_0);
    return <- pass_1;
}, __::builtin`each(events))
```

**Classification**: FALLS-OFF — default cascade (tier-2 helpers).

**Conclusion**: `_count(predicate)` is the 2-arg form, and the terminator-peel at line 1953 requires `length(calls.back()._0.arguments) == 1` — bails BY DESIGN since the 1-arg splice template would silently drop the predicate (emit `length(seen)` instead of counting predicate-true survivors). Extension fix: extend plan_distinct's terminator branch to recognize 2-arg `_count(p)` and `_long_count(p)` and emit `if (p(it)) cnt++` at the fresh-key site.

### 3d — Where + distinct_by + count (array baseline)

**Probe** (`/tmp/audit_probes/chain3_3d.das`):
```das
def probe_3d(events : array<EventD>) : int {
    return _fold(each(events)._where(_.recent)._distinct_by(_.user_id).count())
}
```

**Generated** (trimmed):
```das
return invoke($(source : array<EventD>) : int {
    var inscope seen : table<int; void>;
    for (it in source)
        if (it.recent) {
            let k = unique_key(it.user_id);
            if (!key_exists(seen, k))
                insert(seen, k);
        }
    return length(seen);
}, events)
```

**Classification**: SPLICE-FIRES — buffer-free count arm (terminator is `length(seen)`).

**Conclusion**: Streaming-dedup arm — single source walk, hashed dedup, return length. No element buffer (line 2014-2016, `needBuffer = false` when terminator is count).

### 3e — Select then where then distinct then count (decs)

**Probe** (`/tmp/audit_probes/chain3_3e.das`): same shape as 3a over decs.

**Classification**: FALLS-OFF — default cascade, doubled by eager decs materialization.

**Conclusion**: plan_decs_distinct bails at line 5085. Three full-N allocations (`res` + `pass_0` + `pass_1`) to compute a scalar count. Same user rewrite (swap where and select) and same extension fix as 3a.

### 3f — Where + distinct_by + count (decs baseline)

**Probe** (`/tmp/audit_probes/chain3_3f.das`):

**Generated** (trimmed):
```das
return invoke($() : int {
    var inscope decs_seen : table<int; void>;
    for_each_archetype(..., $(arch) {
        for (ded_user_id, ded_recent in get_ro(...), get_ro(...))
            if (ded_recent) {
                let decs_k = unique_key(ded_user_id);
                if (!key_exists(decs_seen, decs_k))
                    insert(decs_seen, decs_k);
            }
    });
    return length(decs_seen);
})
```

**Classification**: SPLICE-FIRES — plan_decs_distinct streaming-dedup arm with hoisted `decs_seen` table.

**Conclusion**: Hoisted seen-table spans archetypes, single walk with where + key insert, return length.

### Chain 3 — follow-up TODOs

- **Highest impact**: extend the 1-arg terminator peel in plan_distinct (line 1953) and plan_decs_distinct (line 5057) to accept 2-arg `_count(p)` / `_long_count(p)`. Emit predicate as a gate at the fresh-key site.
- Document (possibly as a STYLE lint) that `_select → _where → distinct → terminator` should be rewritten to `_where → _select → distinct → terminator` — the pre-select form is splice-eligible.
- Niche: recognize `_distinct_by(keyFn) + _order_by(otherKey) + to_array` and emit a fused hash-track + bounded sort walk.
- Decs FALLS-OFF inherits the eager-materialize double penalty from the bridge.

---

## Chain 4 — `plan_group_by` / `plan_decs_group_by`

**Accepts**: `[where_*][select*] group_by_lazy(key) [having_]? select(reducer) [count]?` (linq_fold.das:3030 array, :4500 decs, shared core :2729)
**Common bails**: missing terminal select (line 3046 / 4516), missing group_by_lazy (line 3056 / 4526), unrecognized reducer specs (line 2808), bare reducer + hidden HAVING slots (line 2821).

### 4a — Inventory: sum price per category, keep categories totaling >1000

**Probe** (`/tmp/audit_probes/chain4_4a.das`):
```das
return <- _fold(items
    ._group_by(_.category)
    ._select((C = _._0, T = _._1 |> select(@(i : Item) => i.price) |> sum))
    ._where(_.T > 1000))
```

**Generated**:
```das
var pass_0 <- __::linq`group_by_lazy(source, $(_) { return _.category; });
var pass_1 <- __::linq`select(pass_0, $(_) {
    return tuple(_._0, __::linq`sum(__::linq`select(_._1, <lambda i.price>)));
});
finalize(pass_0);
var pass_2 <- __::linq`where_(pass_1, $(_) { return _.T > 1000; });
finalize(pass_1);
return <- pass_2;
```

**Classification**: FALLS-OFF (default cascade — three eager array allocations).

**Conclusion**: A `_where` AFTER `_select(reducer)` lives outside the group_by recognizer (linq_fold.das:3046 demands `select` to be the immediate tail, optionally with one `having_` between it and `group_by_lazy`). The user's "post-aggregate HAVING" is exactly what the optional `having_` slot is for — rewrite to `._group_by(_.category)._having(_._1 |> select(@(i:Item)=>i.price) |> sum > 1000)._select(...)` and the splice fires. Arm extension: peel a single trailing `_where` and translate it to a `having_` slot when its predicate references only post-projection field names.

### 4b — Brands sorted by count

**Probe** (`/tmp/audit_probes/chain4_4b.das`):
```das
return <- _fold(items
    ._group_by(_.brand)
    ._select((B = _._0, C = _._1 |> length))
    ._order_by(_.C))
```

**Generated**:
```das
var pass_0 <- __::linq`group_by_lazy(source, $(_) { return _.brand; });
var pass_1 <- __::linq`select(pass_0, $(_) { return tuple(_._0, length(_._1)); });
finalize(pass_0);
__::linq`order_by_inplace(pass_1, $(_) { return _.C; });
return <- pass_1;
```

**Classification**: FALLS-OFF (default cascade).

**Conclusion**: Any post-`select(reducer)` op forces the recognizer to bail (line 3046). `plan_group_by_core` finishes first; `_order_by` then operates on the bucket-array shape via tier-2. No clean rewrite — this is a genuine two-stage pipeline. Arm extension: after `plan_group_by_core` emits its table, peel a trailing `_order_by` / `_reverse` / `take` cascade on the bucket output as a buffer-required post-pass.

### 4c — Distinct names per brand

**Probe** (`/tmp/audit_probes/chain4_4c.das`):
```das
return <- _fold(items
    ._group_by(_.brand)
    ._select((B = _._0, Names = _._1 |> select(@(i : Item) => i.name) |> distinct)))
```

**Generated**:
```das
var pass_0 <- __::linq`group_by_lazy(source, $(_) { return _.brand; });
var pass_1 <- __::linq`select(pass_0, $(_) {
    return tuple(_._0, __::linq`distinct(__::linq`select(_._1, <lambda i.name>)));
});
finalize(pass_0);
return <- pass_1;
```

**Classification**: FALLS-OFF (default cascade).

**Conclusion**: `recognize_reducer_specs` (line 2807) only knows count / length / long_count / sum / min / max / first / average + their `select(...) |> reducer` variants. `distinct` is not a recognized reducer spec — `specs` comes back empty and we bail at line 2808. Extension: accept `distinct[_by]` / `reverse` / `to_array` as reducer ends, accumulating to `array<T>` slot type (table-of-arrays accumulator pattern already exists).

### 4d — Baseline: count per brand

**Probe** (`/tmp/audit_probes/chain4_4d.das`):
```das
return <- _fold(items
    ._group_by(_.brand)
    ._select((B = _._0, C = _._1 |> length)))
```

**Generated** (trimmed):
```das
var inscope tab : table<string const; tuple<B; C>>
var dummy : tuple<B; C>
for (it in source) {
    let k = it.brand
    let uk = unique_key(k)
    var entry & = tab?[uk] ?? dummy
    if (addr(entry) == addr(dummy)) {
        entry._1 = 1
        dummy._0 = k; tab[uk] = dummy; dummy = default
    } else {
        ++entry._1
    }
}
var buf; reserve(buf, length(tab))
for (kv in values(tab)) { buf |> push_clone(kv) }
return <- buf
```

**Classification**: SPLICE-FIRES (`plan_group_by_core` table-state arm — line 2853).

**Conclusion**: Reference arm — table-of-accumulators + addr-compare first-key-wins state machine.

### 4e — DECS variant of 4a (post-aggregate HAVING)

Same shape as 4a over `from_decs_template`. Decs bridge unrolls (good) but bucket then materializes through the standard array cascade — worst of both worlds (for_each_archetype expansion + three array allocations + per-bucket reducer invoke).

**Classification**: FALLS-OFF (line 4516 mirrors line 3046).

**Conclusion**: `plan_group_by_core` is shared, so one extension covers both planners.

### 4f — DECS baseline (count per brand)

**Generated** (trimmed):
```das
var inscope decs_tab : table<string const; tuple<B; C>>
var decs_dummy : tuple<B; C>
for_each_archetype(<hash>, <erq>, $(arch) {
    for (e_brand in get_ro(arch,"e_brand",type<string>))
        let decs_k = e_brand
        let decs_uk = unique_key(decs_k)
        var decs_entry & = decs_tab?[decs_uk] ?? decs_dummy
        if (addr(decs_entry) == addr(decs_dummy)) {
            decs_entry._1 = 1
            decs_dummy._0 = decs_k; decs_tab[decs_uk] = decs_dummy
            decs_dummy = default
        } else { ++decs_entry._1 }
})
... reserve(decs_buf); for (kv in values(decs_tab)); push_clone ...
```

**Classification**: SPLICE-FIRES (`plan_decs_group_by` → shared core with decs adapter).

**Conclusion**: Decs adapter routes the table-accumulator through `for_each_archetype`. User MUST write `.to_array()` explicitly here.

### Chain 4 — follow-up TODOs

- **HAVING-shaped trailing `_where`**: post-aggregate filter after `_select(reducer)` is the natural shape for "GROUP BY ... HAVING SUM(x) > N". Peel one trailing `_where` and translate to synthetic `having_`.
- **`_order_by` / `_reverse` / `take` on group buckets**: very common SQL shape. Add a post-pass to `plan_group_by_core` that inlines these into the buf-emit loop.
- **`distinct[_by]` as a per-bucket reducer**: extend `recognize_reducer_specs` with array-shaped reducers; slot type becomes `array<T>`.

---

## Chain 5 — `plan_loop_or_count`

**Accepts**: `[where_*][select*][skip?][skip_while?][take_while?][take?] [terminator]?` over 17 terminator names (count / long_count / sum / min / max / average / first / first_or_default / any / all / contains / element_at / element_at_or_default / last / last_or_default / single / single_or_default / aggregate). Source must be array-typed via `each(...)` (linq_fold.das:1563).
**Common bails**: where-after-range (line 1603), select-after-range (line 1630), impure select before where (line 1607), duplicate range ops (lines 1647/1654/1662/1669), buffer-required op (line 1674 — order_by/distinct/group_by/reverse all bail here to their planners), unknown op (line 1678), identity ARRAY chain (line 1748).

### 5a — Two `where_`s around a pure `_select`

**Probe** (`/tmp/audit_probes/chain5_5a.das`):
```das
return _fold(each(items)._where(_.active)._select(_.score)._where(_ > 100).count())
```

**Generated**:
```das
return invoke($(source) {
    var acc = 0;
    for (it in source)
        if (it.active && (it.score > 100)) ++acc
    return acc;
}, items)
```

**Classification**: SPLICE-FIRES (counter lane with merged predicate).

**Conclusion**: where-after-select is HANDLED by the planner (linq_fold.das:1605-1620) — when the projection is pure, the second `where_` substitutes the projection into the predicate and merges with the first via `&&`. Zero allocation. Pure-select fast path does exactly what users expect.

### 5b — Select after a range op

**Probe** (`/tmp/audit_probes/chain5_5b.das`):
```das
return <- _fold(each(items)._select((x = _.score, y = _.active)).skip(5)._select(_.x).to_array())
```

**Generated**:
```das
var pass_0 <- __::linq`select_to_array(source, <projection lambda>);
__::linq`skip_inplace(pass_0, 5);
var pass_2 <- __::linq`select(pass_0, $(_) { return _.x; });
finalize(pass_0);
return <- pass_2;
```

**Classification**: FALLS-OFF (default cascade).

**Conclusion**: linq_fold.das:1630 — the second `_select` arrives with `seenSkip == true` and the planner bails. A `_select` after any range op is structurally incompatible with the single-pass shape because the projection identity shifts mid-chain. User rewrite: collapse to a single projection. Arm extension would require multi-segment shape with per-segment binds — almost a new planner.

### 5c — Where after take

**Probe** (`/tmp/audit_probes/chain5_5c.das`):
```das
return _fold(each(items).take(100)._where(_.active).count())
```

**Generated**:
```das
var pass_0 <- __::linq`take_to_array(source, 100);
var pass_1 <- __::linq`where_(pass_0, $(_) { return _.active; });
finalize(pass_0);
var pass_2 = __::linq`count(pass_1);
finalize(pass_1);
return <- pass_2;
```

**Classification**: FALLS-OFF (default cascade).

**Conclusion**: linq_fold.das:1603 — `where_` arrives with `seenTake == true`. Semantically distinct from `where.take`: `take(100)._where(...)` = "first 100 elements, then keep active ones" (count ≤ 100); `_where(...).take(100)` = "first 100 active ones" (count exactly 100 if there are ≥100 active). No automatic rewrite is safe. Extension: counter lane with take-cap that ticks BEFORE the where filter.

### 5d — Aggregate terminator

**Probe** (`/tmp/audit_probes/chain5_5d.das`):
```das
return _fold(each(items).aggregate(0, $(acc : int; x : Item) => acc + x.score))
```

**Generated**:
```das
return invoke($(source) {
    var agg = 0;
    for (it in source)
        agg = agg + it.score
    return agg;
}, items)
```

**Classification**: SPLICE-FIRES (walk lane, Slice 5).

**Conclusion**: `aggregate` is a recognized walk-lane terminator. Seed and reducer block are inlined; per-element body is just `agg = agg + body`. No invoke into `aggregate_impl`.

### 5e — Baseline: where + select + take + sum

**Probe** (`/tmp/audit_probes/chain5_5e.das`):
```das
return _fold(each(items)._where(_.active)._select(_.score).take(10).sum())
```

**Generated**:
```das
return invoke($(source) {
    var tc = 0;
    var acc = 0;
    for (it in source)
        if (it.active)
            if (tc >= 10) break
            else { ++tc; acc += it.score }
    return acc;
}, items)
```

**Classification**: SPLICE-FIRES (accumulator lane + take cap).

**Conclusion**: Canonical happy path — where → select fuses, take adds a counter, sum is an accumulator. Reference arm.

### Chain 5 — follow-up TODOs

- **`where` after `take` / `take_while`**: not algebraically equivalent so can't auto-reorder, but the counter-lane shape could handle it manually.
- **`select` after `skip` / `take` / `take_while` / `skip_while`**: requires per-segment bind handling. Probably better to document canonical order in `skills/linq.md` and lint-warn.
- Unrecognized op cascade (order/distinct/group/reverse): the bail at line 1674 is the explicit handoff to per-family planners, not a fall-off.

---

## Chain 6 — `plan_decs_unroll`

**Accepts**: same shape as `plan_loop_or_count` (count/long_count/sum/min/max/average/first/first_or_default/any/all/contains/element_at/element_at_or_default/last/last_or_default/single/single_or_default/aggregate/min_by/max_by + implicit-to_array) over `from_decs_template(...)` bridges. Delegates to plan_decs_order_family / plan_decs_reverse / plan_decs_distinct / plan_decs_group_by / plan_decs_join for buffer-required shapes.
**Common bails**: source not a decs bridge (line 4455), no recognized terminator + no implicit to_array (line 4493), range extraction failed (line 4476), chain info failed (line 4478), sum/min/max/average over tuple element (line 4483), select before predicate-driven range (line 3568).

### 6a — Select before predicate-driven range

**Probe** (`/tmp/audit_probes/chain6_6a.das`):
```das
return _fold(from_decs_template(type<DecsItem>)._select(_.score)._skip_while(_ < 0).count())
```

**Generated**:
```das
var pass_0 <- __::linq`select_to_array(source, $(_) { return _.score; });
var pass_1 <- __::linq`skip_while(pass_0, $(_) { return _ < 0; });
finalize(pass_0);
var pass_2 = __::linq`count(pass_1);
finalize(pass_1);
return <- pass_2;
```

**Classification**: FALLS-OFF (default cascade for linq chain; bridge IS unrolled but doesn't connect to the rest).

**Conclusion**: linq_fold.das:3568 — when suffix contains `skip_while` / `take_while`, prefix must be select-free (predicates use source tuple, not projected scalar). User rewrite: drop the `_select`, move comparison into `_skip_while`: `._skip_while(_.score < 0).count()`. Make the rule explicit in `skills/linq.md`. Arm extension would require predicate rewriting through projection.

### 6b — Aggregate over decs

**Probe** (`/tmp/audit_probes/chain6_6b.das`):
```das
return _fold(from_decs_template(type<DecsItem>).aggregate(0, $(acc : int; x : auto) => acc + x.score))
```

**Generated**:
```das
return invoke($() {
    var decs_agg = 0;
    for_each_archetype(<hash>, <erq>, $(arch) {
        for (e_score in get_ro(arch, "e_score", type<int>))
            decs_agg = decs_agg + e_score
    });
    return decs_agg;
})
```

**Classification**: SPLICE-FIRES (`emit_decs_walk_lane`, Slice 5f).

**Conclusion**: Aggregate is in the `isWalk` set. Bridge fuses into accumulator loop — pruner trimmed to ONLY `e_score` reads (no `e_id`/`e_active`). Best-in-class shape.

### 6c — min_by with where

**Probe** (`/tmp/audit_probes/chain6_6c.das`):
```das
let r = _fold(from_decs_template(type<DecsItem>)._where(_.active)._min_by(_.score))
```

**Generated**:
```das
return invoke($() {
    var decs_first = true;
    var decs_bkey : int;
    var decs_belem : tuple<id; score; active>;
    for_each_archetype(..., $(arch) {
        for (e_id, e_score, e_active in get_ro(...), get_ro(...), get_ro(...))
            if (e_active)
                let decs_key = e_score
                if (decs_first)
                    decs_bkey = decs_key
                    decs_belem = tuple(e_id, e_score, e_active)
                    decs_first = false
                elif (decs_key < decs_bkey)
                    decs_bkey = decs_key
                    decs_belem = tuple(e_id, e_score, e_active)
    });
    return decs_belem;
})
```

**Classification**: SPLICE-FIRES (`emit_decs_min_max_by`, streaming single-best state).

**Conclusion**: Canonical streaming-min shape on decs. All three columns read since `min_by` returns the full element.

### 6d — element_at with where

**Probe** (`/tmp/audit_probes/chain6_6d.das`):
```das
let r = _fold(from_decs_template(type<DecsItem>)._where(_.active).element_at(3))
```

**Generated**:
```das
return invoke($() {
    var decs_ec = 0;
    var decs_found = false;
    var decs_res : tuple<...>;
    for_each_archetype_find(..., $(arch) : bool {
        for (e_id, e_score, e_active in get_ro(...), get_ro(...), get_ro(...))
            if (e_active)
                if (decs_ec == 3)
                    decs_res = tuple(e_id, e_score, e_active)
                    decs_found = true
                    return true
                else { ++decs_ec }
        return false;
    });
    if (!decs_found) panic("element index out of range", ...)
    return <- decs_res;
})
```

**Classification**: SPLICE-FIRES (`emit_decs_element_at`, Slice 5f).

**Conclusion**: `for_each_archetype_find` outer (returns bool to break early across archetypes) + counter inside, then panics if not found. Reference arm.

### 6e — reverse + take + to_array (delegates to plan_decs_reverse)

**Probe** (`/tmp/audit_probes/chain6_6e.das`):
```das
return <- _fold(from_decs_template(type<DecsItem>).reverse().take(10).to_array())
```

**Generated**:
```das
return invoke($() {
    var decs_total : int64 = 0;
    for_each_archetype(..., $(arch) { decs_total += arch.size; });
    let decs_actual = (decs_total > 10) ? 10 : decs_total;
    let decs_skip = decs_total - decs_actual;
    var decs_buf;
    if (decs_actual == 0) return <- decs_buf
    reserve(decs_buf, int(decs_actual));
    var decs_seen : int64 = 0;
    for_each_archetype_find(..., $(arch) : bool {
        if (decs_seen + arch.size <= decs_skip)
            decs_seen += arch.size; return false
        var decs_skips = (decs_skip > decs_seen) ? (decs_skip - decs_seen) : 0;
        for (e_id, e_score, e_active in get_ro(...), get_ro(...), get_ro(...))
            if (decs_skips > 0) { --decs_skips; continue }
            else
                var decs_tup = tuple(e_id, e_score, e_active)
                push_clone(decs_buf, decs_tup)
                if (int64(length(decs_buf)) >= decs_actual) break
        decs_seen += arch.size;
        return int64(length(decs_buf)) >= decs_actual;
    });
    __::linq`reverse_inplace(decs_buf);
    return <- decs_buf;
})
```

**Classification**: SPLICE-FIRES (`plan_decs_reverse` — PR #2834 reverse skip-into-tail pattern).

**Conclusion**: `plan_decs_unroll` does NOT handle `reverse` itself — dispatch happens earlier through `plan_decs_reverse`. 2-pass shape (sum sizes → skip into tail → reverse_inplace) is exactly the PR #2834 win. Dispatch works as designed.

### Chain 6 — follow-up TODOs

- **`select` before `skip_while` / `take_while`**: same root cause as Chain 5 5b (predicate semantics differ pre- vs post-projection). Document canonical order.
- **sum/min/max/average over tuple element without `_select`**: line 4483 bail is correct but silent — emit a planner diagnostic when `isAccum && selectCount == 0`.
- **Implicit to_array gate**: line 4493 requires `expr._type.isGoodArrayType`. Failure mode for "no terminator at all" is opaque.

---

## Chain 7 — `plan_zip`

**Accepts**: `zip(srcB) [where_*][select?][skip?][skip_while?][take_while?][take?] [terminator]?` STRICTLY 2-arg zip (linq_fold.das:5395)
**Common bails**: 3-arg result-selector zip (line 5402), unrecognized intermediate op (line 5528), chained selects (line 5486)

### 7a — `zip(srcB, result_selector)` 3-arg form + `sum()`

**Probe** (`/tmp/audit_probes/chain7_7a.das`):
```das
return _fold(each(a) |> zip(each(b), $(x, y : int) => x + y) |> sum())
```

**Generated**:
```das
return invoke($(var source : iterator<int&>) : int {
    var pass_0 <- __::linq`zip_to_array(source, each(b), $(x,y:int) => x+y);
    var pass_1 = __::linq`sum(pass_0);
    finalize(pass_0);
    return <- pass_1;
}, each(a))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: The natural "sum of (a[i] op b[i])" — the dot-product idiom — bails at line 5402 because the result-selector lives inside `zip(...)`. To recover splice, rewrite as `zip(b) |> _select(_._0 * _._1) |> sum()` (probe 7d). Either lower the 3-arg form to 2-arg `zip + _select` inside the macro before reaching `plan_zip`, or extend `plan_zip` to peel a 3-arg zip's result_selector into the chain `projection` slot.

### 7b — `zip` + `_order_by` terminator

**Probe** (`/tmp/audit_probes/chain7_7b.das`):
```das
return <- _fold(each(a) |> zip(each(b)) |> _order_by(_._0) |> to_array())
```

**Generated**:
```das
return <- invoke($(var source : iterator<int&>) : array<tuple<int;int>> {
    var pass_0 <- __::linq`zip_to_array(source, each(b));
    __::linq`order_by_inplace(pass_0, $(_) { return _._0; });
    return <- pass_0;
}, each(a))
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: `_order_by` after `zip` is unrecognized intermediate op (line 5528). A targeted "zip-then-order-by-then-take" arm would be the right next splice.

### 7c — `zip` + chained `_select`s

**Probe** (`/tmp/audit_probes/chain7_7c.das`):
```das
return <- _fold(each(a) |> zip(each(b)) |> _select(_._0) |> _select(_ * 2) |> to_array())
```

**Generated**:
```das
var pass_0 <- __::linq`zip_to_array(source, each(b));
var pass_1 <- __::linq`select(pass_0, $(_) { return _._0; });
finalize(pass_0);
var pass_2 <- __::linq`select(pass_1, $(_) { return _ * 2; });
finalize(pass_1);
return <- pass_2;
```

**Classification**: FALLS-OFF — default cascade (3 buffer allocations + 2 finalize calls).

**Conclusion**: Two `_select`s in a row bail at line 5486. Collapse N consecutive `_select`s into a single projection via repeated `peel_lambda_rename_var` + body composition. Same shape unblocks plan_loop_or_count.

### 7d — Baseline: `zip` + `_select` + `sum`

**Probe** (`/tmp/audit_probes/chain7_7d.das`):
```das
return _fold(each(a) |> zip(each(b)) |> _select(_._0 * _._1) |> sum())
```

**Generated**:
```das
return invoke($(srcA : array<int>; srcB : array<int>) : int {
    var acc = 0;
    for (itA, itB in srcA, srcB)
        let it : tuple<int;int> = tuple(itA, itB)
        acc += (it._0 * it._1)
    return acc;
}, a, b)
```

**Classification**: SPLICE-FIRES — inline parallel `for` + accumulator, zero intermediate buffers.

**Conclusion**: User-facing gap: 7d's wording (`zip(b) |> _select(_._0 * _._1) |> sum()`) is strictly less readable than 7a's `zip(b, $(x,y) => x*y) |> sum()` form, yet the latter falls off. Splice ergonomics suffer when the "fast path" requires the awkward spelling.

### Chain 7 — follow-up TODOs

- Pre-lower 3-arg `zip(a, b, sel)` to 2-arg `zip(a, b) |> _select(...)` inside `LinqFold.visit` (or hoist the selector into `projection` directly inside `plan_zip`). Closes 7a.
- Extend `plan_zip` to accept `_order_by` / `reverse` between zip and a terminator that needs full materialization anyway. Closes 7b and unblocks "top-K of zip" patterns.
- Collapse N consecutive `_select` projections (line 5486 + plan_loop_or_count's analog) — symmetric with how N consecutive `where_` already compose via `&&`. Closes 7c.

---

## Chain 8 — `plan_decs_join`

**Accepts**: `_join(srcA, srcB, on, into) [count]?` strictly binary, primitive keys, no intermediate chain ops (linq_fold.das:5267)
**Common bails**: post-join chain op of ANY kind (line 5284), non-primitive key type (lines 5296-5303), keya/keyb untyped (line 5293).

### 8a — `_join` + post-join `_where` + `count`

**Probe** (`/tmp/audit_probes/chain8_8a.das`):
```das
return _fold(from_decs_template(type<DecsCar>) |> _join(from_decs_template(type<DecsDealer>),
                                                         $(l, r) => l.dealer_id == r.id,
                                                         $(l, r) => (CarName = l.name, DealerName = r.name))
                                                |> _where(_.CarName != "")
                                                |> count())
```

**Generated** (trimmed):
```das
var pass_0 <- __::linq`join_to_array(source, invoke($() : iterator<...> {
    var res : array<...>; for_each_archetype(...) { ... push(...) }
    return <- to_sequence(res);
}), keya_block, keyb_block, result_block);
var pass_1 <- __::linq`where_(pass_0, predicate);
finalize(pass_0);
var pass_2 = __::linq`count(pass_1);
finalize(pass_1);
```

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: Bails at line 5284 because `_where` sits between `_join` and `count`. Worst-case: full materialization of BOTH dealer and car archetypes into per-iterator buffers before `join_impl` runs, plus a second `where_to_array` pass and a third `count`. Fix: wrap the count-bump in `if (predicate) { ... }` inside the probe loop.

### 8b — `_join` + post-join `_select` + `to_array`

**Probe** (`/tmp/audit_probes/chain8_8b.das`):
```das
return <- _fold(from_decs_template(type<DecsCar>) |> _join(from_decs_template(type<DecsDealer>), ..., ...)
                                                  |> _select(_.CarName)
                                                  |> to_array())
```

**Classification**: FALLS-OFF — default cascade (3 buffer allocations).

**Conclusion**: Same bail (line 5284). The natural projection-shaping idiom — produce the full join row then project — is universally faster as inline projection. Fix is symmetric with 8a: accept single trailing `_select` and substitute body into the result lambda position.

### 8c — Composite (tuple) join key

**Probe** (`/tmp/audit_probes/chain8_8c.das`):
```das
return _fold(_join(srcA, srcB, $(l, r) => (l.dealer_id, l.id) == (r.region, r.id), ...) |> count())
```

**Generated** (trimmed): `join_to_array` instantiated with `tuple<int;int>` keys; `unique_key(tuple<int;int>) : string` invoked by `join_impl`.

**Classification**: FALLS-OFF — default cascade.

**Conclusion**: Bails at primitive-key gate (lines 5296-5303): `keyType.baseType == Type.tTuple` not in whitelist. The `_join` macro itself accepts tuple-equi form, so the gate is the ONLY reason this falls off. Two fixes: (a) plumb `unique_key(keyBody)` into probe/insert sites of the splice (matches `join_impl`); or (b) accept tuples-of-primitives directly as `table<tuple<...>; array<...>>` key, since daslang tables hash tuples natively. (b) is cleaner.

### 8d — Baseline: `_join` + `count`

**Generated**:
```das
return invoke($() : int {
    var decs_jcnt = 0;
    var decs_hash : table<int; array<tuple<id:int;name:string>>>;
    for_each_archetype(<dealer_hash>, <dealer_erq>, $(arch) {
        for (dealer_id, dealer_name in get_ro(arch,"dealer_id",type<int>), get_ro(arch,"dealer_name",type<string>))
            var decs_tup_b = tuple(dealer_id, dealer_name)
            push_clone(decs_hash[decs_tup_b.id], decs_tup_b)
    });
    for_each_archetype(<car_hash>, <car_erq>, $(arch) {
        for (car_id, car_dealer_id, car_name in get_ro(...) [...3 cols])
            var decs_tup_a = tuple(car_id, car_dealer_id, car_name)
            get(decs_hash, decs_tup_a.dealer_id, $(var decs_jarr) {
                decs_jcnt += length(decs_jarr);
            })
    });
    return decs_jcnt;
})
```

**Classification**: SPLICE-FIRES — single hash, two `for_each_archetype` passes, count bumped by bucket-length.

**Conclusion**: Confirms hashed-join splice fires for bench-supported shape. Narrow surface is the issue: any meaningful post-processing reverts to full materialization.

### Chain 8 — follow-up TODOs

- Add a single-trailing-`_where` arm (mirror plan_zip's `whereCond` slot). Closes 8a + C6.
- Add a single-trailing-`_select` arm: substitute select's lambda body into result-push position. Closes 8b.
- Accept tuples-of-primitives as keys directly (`table<tuple<int;int>; array<...>>`); cascade non-primitive structs to `unique_key`. Closes 8c.
- Document the splice's narrow shape in `LinqJoin`'s docstring.

---

## Composition probes

When a user chain combines two splice families, dispatch order (linq_fold.das:5700-5727) claims one and the other op bails the whole arm. Six obvious user-natural compositions, all FALLS-OFF:

### C1 — Distinct + order + take

**Why interesting**: "Top-K most recent distinct users" — both `_distinct_by` and `_order_by` have splice arms, neither tolerates the other op.

**Probe** (`/tmp/audit_probes/comp_C1.das`):
```das
return <- _fold(each(items) |> _distinct_by(_.user) |> _order_by(_.ts) |> take(10) |> to_array())
```

**Generated**:
```das
var pass_0 <- __::linq`distinct_by_to_array(source, $(_) { return _.user; });
__::linq`order_by_inplace(pass_0, $(_) { return _.ts; });
__::linq`take_inplace(pass_0, 10);
return <- pass_0;
```

**Classification**: FALLS-OFF — `plan_distinct` runs first (dispatch line 5712), sees non-distinct trailing op, returns null; `plan_order_family` runs second, sees `distinct_by` upstream, returns null. Tier-2 cascade.

**Conclusion**: Two splice arms exist but the planner picks neither because each insists on owning the whole chain. Fast shape would be bounded-heap of size 10 keyed on `(seen_users_set, _.ts)` — collect into heap during single source pass, gated by set-insert success. Cross-splice composition is the obvious gap.

### C2 — Group-by + select + order-by + to_array

**Why interesting**: "Brands sorted by frequency" — canonical SQL `GROUP BY ... ORDER BY COUNT(*)`.

**Probe** (`/tmp/audit_probes/comp_C2.das`):
```das
return <- _fold(each(items)
    |> _group_by(_.brand)
    |> _select((B = _._0, C = _._1 |> count()))
    |> _order_by(_.C)
    |> to_array())
```

**Generated**:
```das
var pass_0 <- __::linq`group_by_lazy_to_array(source, $(_) { return _.brand; });
var pass_1 <- __::linq`select(pass_0, $(_) { return tuple(_._0, __::linq`count(_._1)); });
finalize(pass_0);
__::linq`order_by_inplace(pass_1, $(_) { return _.C; });
return <- pass_1;
```

**Classification**: FALLS-OFF — `plan_group_by` bails because trailing op is `_order_by`.

**Conclusion**: `plan_group_by_core` already builds the bucket map directly. Letting `_select` + `_order_by` consume the bucket inside the same emission would give 1 hashmap walk + 1 inplace sort but skip the intermediate `array<tuple<string;array<Item>>>` materialization. Cross-cuts with 7b/C1 observation.

### C3 — Decs join + select + group_by + select

**Why interesting**: "Join cars onto dealers, group by region, count" — universal BI shape on decs.

**Probe** (`/tmp/audit_probes/comp_C3.das`):
```das
return <- _fold(_join(decsCars, decsDealers, on=..., into=(Region=r.region, CarName=l.name))
                |> _group_by(_.Region)
                |> _select((R = _._0, N = _._1 |> count()))
                |> to_array())
```

**Classification**: FALLS-OFF — `plan_decs_join` bails at 5284 (trailing chain ops); `plan_decs_group_by` requires a decs source on top, not a `_join` invoke; default cascade builds dealer-array → join-array → group-map → select-array. Three intermediate allocations.

**Conclusion**: This is the "killer demo" composition. The structural fix is to refactor `plan_decs_join` so its emission integrates with `plan_decs_group_by`'s bucket-fill — instead of `push_clone(buf, result_lam(...))` in the probe loop, emit `bucket[keyExpr] |> push_clone(...)` directly. Largest single architectural change suggested by the audit.

### C4 — Zip + reverse + to_array

**Why interesting**: "Pair two parallel sequences, walk backward" — natural for time-reversed analyses.

**Probe** (`/tmp/audit_probes/comp_C4.das`):
```das
return <- _fold(each(a) |> zip(each(b)) |> reverse() |> to_array())
```

**Generated**:
```das
var pass_0 <- __::linq`zip_to_array(source, each(b));
__::linq`reverse_inplace(pass_0);
return <- pass_0;
```

**Classification**: FALLS-OFF — `plan_zip` lists `reverse` as unrecognized op (line 5528); `plan_reverse` doesn't recognize a 2-source zip head.

**Conclusion**: Cheapest fall-off in absolute cost (1 buffer + 1 inplace), but trivial to absorb: zip's natural emission can be `for i in length downto 0` parallel `for` — 1-line change when `reverse` is the only intermediate. Bundle with the 7b TODO.

### C5 — Order-by + distinct + take + to_array

**Why interesting**: Variant on C1 with order-first.

**Probe** (`/tmp/audit_probes/comp_C5.das`):
```das
return <- _fold(each(items) |> _order_by(_.score) |> distinct() |> take(10) |> to_array())
```

**Classification**: FALLS-OFF — `plan_order_family` doesn't recognize `distinct`; `plan_distinct` doesn't recognize `_order_by` upstream.

**Conclusion**: Identical reasoning to C1, operator-order swapped. Confirms the "two splice families never cooperate" pattern is symmetric — not a property of which arm runs first.

### C6 — Decs_join + post-join filter

**Why interesting**: Composition view of 8a — confirms the failure mode is the same whether reached via "splice arm couldn't extend" or "two arms collide".

**Probe** (`/tmp/audit_probes/comp_C6.das`): same as 8a.

**Classification**: FALLS-OFF — same root cause as 8a (linq_fold.das:5284).

**Conclusion**: Listed here to make the symmetry explicit. Closing 8a TODO closes this row too.

### Composition — cross-cutting observation

Five of six composition probes (C1, C2, C3, C5, C6) are blocked by the same architectural pattern: each splice arm currently requires `flatten_linq` to yield a contiguous run of recognized ops, with the planner pipeline trying one arm at a time and falling to tier-2 the moment ANY arm refuses. There is no cross-arm composition mechanism. The highest-leverage next investment isn't another arm — it's a "compose-aware" planner step that walks the call chain once, attributes each op to a candidate arm (or "boundary op" like `_where`/`_select` that any arm can host), and stitches the emissions. C4 is the lone outlier where one arm could absorb the second op trivially; the other five point at the same missing infrastructure.

---

## Cross-cutting findings

Synthesizing the per-chain TODOs into prioritized themes:

### Theme 1 — Terminal `_select` extension (HIGH impact, MEDIUM effort)

Recurs in: **chains 1, 2, 7, 8**. Almost every arm that produces a buffer or holds a bounded-K state could accept a terminal `_select` that projects during the emission/return — currently bails almost universally. The bounded-heap, R5 buffer, and join probe-loop arms all hold `≤K` or per-element values they then need to discard or project; absorbing the projection is a small qmacro splice each.

Specific arms to extend:
- `plan_order_family` line 1234 + `plan_decs_order_family` line 4547 — accept terminal `_select` after `take(N)` / `first` / `first_or_default`. Closes 1a, 1e.
- `plan_reverse` line 1764 + `plan_decs_reverse` line 4802 — accept terminal `_select` after `reverse [take(N)]`. Closes 2c, 2e.
- `plan_decs_join` line 5267 — accept single trailing `_select` substituting into result lambda. Closes 8b.
- `plan_zip` line 5395 — pre-lower 3-arg `zip(a, b, sel)` to 2-arg `zip(a, b) |> _select(sel)`. Closes 7a.

### Theme 2 — Trailing `_where` / HAVING (HIGH impact, MEDIUM effort)

Recurs in: **chains 4, 5, 8**. The "trailing post-aggregate filter" idiom is universal in SQL-like usage and falls off whenever it appears in a splice arm:
- `plan_group_by_core` — peel trailing `_where` to synthetic `having_` slot (closes 4a, 4e).
- `plan_decs_join` — accept single trailing `_where` mirroring plan_zip's `whereCond` (closes 8a, C6).
- `plan_loop_or_count` — counter-lane with take-cap that ticks BEFORE the where filter (closes 5c).

### Theme 3 — Cross-arm composition (HIGHEST impact, LARGE effort)

Recurs in: **5 of 6 composition probes** (C1, C2, C3, C5, C6). The planner pipeline tries arms in order and fails to tier-2 if any arm refuses; there is no mechanism for two arms to share a chain. The structural fix is a "compose-aware" planner step that:
1. Walks the call chain once
2. Attributes each call to a candidate arm or to a "boundary op" (`_where` / `_select` are universal)
3. Stitches arm emissions at boundary points (e.g. plan_decs_join emits `bucket[keyExpr] |> push_clone(...)` directly into plan_decs_group_by's bucket-fill loop)

This is the largest architectural change suggested by the audit but unlocks the most common BI-style queries. Closes C1, C2, C3, C5.

### Theme 4 — 2-arg terminator predicates (LOW effort, MEDIUM impact)

Recurs in: **chain 3, chain 5, chain 7**. Several splice arms only accept 1-arg `count()` / `long_count()` / `sum()` etc., and silently bail when the user adds a predicate. The extension is trivial: emit `if (p(it)) cnt++` at the existing increment site.

- `plan_distinct` line 1953, `plan_decs_distinct` line 5057 — accept 2-arg `count(p)` / `long_count(p)`. Closes 3c.
- `plan_zip` lines 5412-5436 — same shape. (Not probed explicitly but observed in agent 1 inventory.)
- `plan_decs_unroll` line 4458 — same shape.

### Theme 5 — `_order_by(k).reverse()` → `_order_by_descending(k)` normalization

Recurs in: **chains 1, 2**. Pure rewrite at the macro level, before any planner sees the chain. Closes 1b, 2b. Trivial to implement; sized like a half-day.

### Theme 6 — Decs-bridge double penalty (MEDIUM impact, LOW effort)

Whenever a `plan_decs_*` arm bails, the `from_decs_template` bridge degenerates to full `for_each_archetype` materialization into a temp `res` array, which is then wrapped in `to_sequence` for the array-side cascade. This costs an EXTRA allocation on top of whatever cascade follows.

Fix: in `FromDecsMacro` (or at the `_fold` dispatch point), emit a diagnostic (`compile_warning` style) when the bridge survives without any decs-side splice arm claiming it. Doesn't fix the underlying chain but tells the user where the perf cliff is.

### Theme 7 — Chained `_select` collapse

Recurs in: **chain 5 (5b), chain 7 (7c)**. N consecutive `_select` projections should collapse into a single projection via repeated `peel_lambda_rename_var` + body composition — symmetric with how N consecutive `_where` already compose via `&&`. Same mechanism unblocks both plan_loop_or_count and plan_zip.

### Theme 8 — Specialized fusion arms (low priority)

Recurs in: **chains 2, 3, C4**. Several "two specific arms could fuse" cases:
- `reverse + distinct_by` (chain 2a) — single walk retaining LAST element per key.
- `_distinct_by(keyFn) + _order_by(otherKey)` (chain 3b) — hash-track + bounded sort walk.
- `zip + reverse` (C4) — emit `for i in length downto 0`.

Each is small and self-contained. Lower priority than themes 1-3 but cheap follow-ups when in the area.

### Out-of-scope observations

- **`linq_fold_patterns.rst` cross-check**: this audit did NOT systematically verify that every "splice arm exists" claim in the RST page is reachable via the canonical chain shape. A future doc-only PR should walk the RST table row-by-row and probe each shape (most are covered above; rows not represented are likely doc-only fictions).
- **JIT verification**: all probes here are INTERP-only. The JIT lane may behave differently — e.g. the bounded-heap arm's `spliced_push_heap` may or may not optimize well under llvm_jit.
- **Bench impact quantification**: the cross-cutting findings are ordered by "how natural is the user phrasing" + "how expensive is the cascade", not by measured ns/op. A follow-up bench round (writing N FALLS-OFF chains as new benches, measuring fall-off cost) would sharpen the prioritization.

---

## How to re-run

The audit is reproducible. Per-probe workflow:

```bash
# Single probe: compile + dump
mcp__daslang__compile_check /tmp/audit_probes/chain1_1a.das
mcp__daslang__ast_dump file=/tmp/audit_probes/chain1_1a.das function=probe_1a mode=source

# Whole audit: compile all probes
for f in /tmp/audit_probes/*.das; do
    mcp__daslang__compile_check "$f"
done
```

To re-create the probe set after deleting `/tmp/audit_probes/`, follow each probe's "Probe" code block — each is self-contained (`options gen2` + `require` lines + struct + one `[export] def probe_NX` + stub `def main(){}`). The audit doesn't depend on any fixture outside the probe files themselves.

Classification rules:
- `for_each_archetype` + inline state (heap, accumulator, counter, table) → **SPLICE-FIRES**
- `__::linq\`*_to_array\`` / `__::linq\`*_inplace\`` / cascade of `pass_0 → pass_1 → ...` → **FALLS-OFF**
- Direct `min_by_impl` / `top_n_by_impl` invocation without inlining → **BAILS-TO tier-2**
