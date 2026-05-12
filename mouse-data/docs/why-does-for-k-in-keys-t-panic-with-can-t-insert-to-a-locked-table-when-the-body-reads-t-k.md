---
slug: why-does-for-k-in-keys-t-panic-with-can-t-insert-to-a-locked-table-when-the-body-reads-t-k
title: Why does `for (k in keys(t))` panic with "can't insert to a locked table" when the body reads `t[k]`?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**Iterating `keys(table)` (or `values(table)`) takes a read-lock on the table for the iterator's lifetime. Any `t[k]` access inside the body is a potential auto-insert (table subscript inserts a default if the key is absent), and auto-insert during the iteration lock panics.**

Even when the key DOES exist and the access is logically read-only, daslang's table subscript path runs through the same insertion machinery — there's no "lookup-only" overload. The iterator lock catches it as a write attempt.

Symptom seen 2026-05-12 in dasImgui indexed-form widgets:

```das
for (k in keys(CHANNEL_VOL)) {
    slider_float(CHANNEL_VOL[k], (text = "ch{k}"))   // panic: can't insert to a locked table
}
```

`slider_float` is a `[widget]` call; the indexed-form wrapper does `CHANNEL_VOL[k]` internally to pass the state ref. That counts as auto-insert and trips the lock.

**Fix: snapshot the keys to a local `array<K>` first, then iterate the array.**

```das
var live_keys : array<int>
for (k in keys(CHANNEL_VOL)) {
    live_keys |> push(k)
}
for (k in live_keys) {
    slider_float(CHANNEL_VOL[k], (text = "ch{k}"))   // ok — no lock held
}
```

This generalizes — any `for (k in keys(t)) / for (v in values(t)) / for (k, v in keys(t), values(t))` body that needs to subscript `t` again has the same problem. Snapshot first, then loop.

`erase(t, k)` and `t |> erase(k)` inside a `keys()` iteration also panic for the same reason — same workaround.</body>
<parameter name="slug">daslang-for-keys-table-locked-cant-insert-snapshot-keys-array-first

## Questions
- Why does `for (k in keys(t))` panic with "can't insert to a locked table" when the body reads `t[k]`?
