---
slug: daslang-sort-is-qsort-not-stable
title: Is daslang's `sort()` stable, and how do I make a multi-key sort deterministic?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**daslang's built-in `sort()` is implemented via `das_qsort_r` — qsort, NOT stable.** Equal-key ordering is unspecified and may be platform-dependent (libc-qsort differs across glibc / musl / Windows CRT).

Confirmed by reading the runtime: `daslib/builtin.das:1623` calls into `__builtin_sort_array_any_cblock` which is `das_qsort_r` on the C++ side.

**Implication:** Don't claim "stable sort" in docstrings or rely on tie-order in tests. If a regression test asserts sibling ordering on equal keys, it can be flaky in CI even if it always passes locally.

**Deterministic-tiebreak pattern** when stability actually matters:

```das
// Decorate with index, sort by (primary, idx), keep ordering on ties.
struct private SortKey {
    primary : int
    idx     : int
}
def sort_stable(var arr : array<auto(T)>; key : block<(x : T) : int>) {
    var keys : array<SortKey>
    keys |> reserve(length(arr))
    for (i in range(length(arr))) {
        keys |> push(SortKey(primary = invoke(key, arr[i]), idx = i))
    }
    keys |> sort() $(a, b : SortKey) {
        return a.primary < b.primary if (a.primary != b.primary)
        return a.idx < b.idx
    }
    // ... reorder arr by keys ...
}
```

**Easier alternative when you control the test:** use distinct keys (no ties) and assert only the monotonic invariant the production code actually depends on. For example, `mouse_play` only walks forward — the production invariant is non-decreasing `t_ms`, not stable ordering. The regression test under `tests/dasglfw/test_mouse_play_sort.das` uses distinct `t_ms` values for exactly this reason.

PR #2630 round 2 trail: claimed "Stable-sort" in `sort_play_events` docstring, Copilot caught it, dropped the claim + dropped the tie test.

## Questions
- Is daslang's `sort()` stable, and how do I make a multi-key sort deterministic?
