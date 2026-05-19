---
slug: group-by-first-key-wins-single-hash-tab-dummy-addr-compare
title: How do I splice an incremental-aggregate group_by with first-key-wins semantics in ONE hash op per element on hits?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Pattern**: `tab?[uk] ?? dummy` ref-binding + `addr(entry) == addr(dummy)` miss detection. Single hash op per element on hits, two on misses (insert). Used by `plan_group_by` in `daslib/linq_fold.das` (PR #2721).

### The shape

```das
// OUT of the for-loop: per-table dummy of the value type.
var dummy : tuple<KT; AccT>

for (it in src) {
    let k = <key-expr>
    let uk = _::unique_key(k)
    unsafe {
        // ?[uk] returns a ref to existing entry; ?? falls back to dummy on miss.
        var entry & = tab?[uk] ?? dummy
        // Aggregator update runs unconditionally — on a hit it writes through to
        // the table entry, on a miss it writes to dummy (then we transfer below).
        entry._1 += <update-expr>
        if (addr(entry) == addr(dummy)) {
            // Miss: this is a fresh key. Stamp the key (first-key-wins) and
            // promote dummy into the table at uk, then RESET dummy for next miss.
            dummy._0 = k
            tab[uk] = dummy
            dummy = default<typedecl(dummy)>
        }
    }
}
```

### Why each piece

- **`tab?[uk] ?? dummy`** — `?[k]` is the safe lookup (no auto-insert; returns a ref to the entry or null). `??` substitutes `dummy` when null. Net cost: ONE hash op per element on the hot path (the cache-hit case).
- **`addr(entry) == addr(dummy)`** — ref-binding identity check (idiom from `apply_in_context.das:99`). True iff the lookup missed and we got the fallback. Avoids a second `key_exists` call.
- **`var entry & =`** the ampersand is critical: `var entry = tab?[uk] ?? dummy` would copy; `& =` rebinds to the same storage.
- **`dummy._0 = k` only on miss** — first-key-wins matches reference `group_by_lazy_impl` (key stored once per bucket, on creation only). Re-hits do not touch `_0`.
- **`tab[uk] = dummy` on miss** — the auto-insert form. This IS the second hash op on the miss path; acceptable because miss count == |distinct keys|, not |elements|.
- **`dummy = default<typedecl(dummy)>`** — reset BEFORE the next miss so accumulator state doesn't bleed across keys. Cheap (stack zero), and only fires per miss. `typedecl(dummy)` extracts the type from the var so the planner doesn't need to thread the type expression separately.
- **`var dummy : tuple<KT; AccT>` OUT of the for-loop** — declaring inside the loop would cost a stack-zero per element. Hoisted = one zero at function entry plus per-miss resets.

### Performance vs naive approaches

| Pattern | Hash ops per ELEMENT (hot path) | Notes |
|---|---:|---|
| `if (!key_exists(uk)) tab[uk] = …; tab[uk]._1 += …` | 2-3 | extra `key_exists` + auto-insert + read |
| `let isNew = !key_exists(uk); var e & = tab[uk]; if (isNew) e._0 = k; e._1 += …` | 2 | key_exists + auto-insert |
| **`tab?[uk] ?? dummy` + addr-compare** | **1** | safe-lookup only; auto-insert ONLY on miss |

Concrete impact on `groupby_count` at 100K rows: pattern 2 cost 43 ns/op (regression from 33 baseline); this pattern recovered to 37 ns/op while keeping first-key-wins correctness.

### When this pattern applies

- Per-element incremental aggregate fusion where update is idempotent on hits (`+=`, `++`, `max`, `min` once an init flag isn't needed).
- Any AST shape that needs "lookup-or-create with side-channel write to per-key state" with minimal hot-path overhead.
- NOT for `_._1 |> first` / `min`/`max`/`average` reducers that need a "is this the first time?" branch — those need an explicit init-flag tuple slot.

### Test guard

In emitter tests, assert `count_call(body, "key_exists") == 0` and `count_call(body, "unique_key") == 1` per fresh-key branch — proves the single-hash hot path didn't regress.

See [[daslib-macro-boost-has-sideeffects-predicate]] for the macro-time purity check that decides whether the key expression can be safely evaluated once per element vs needing a bind.

## Questions
- How do I splice an incremental-aggregate group_by with first-key-wins semantics in ONE hash op per element on hits?
