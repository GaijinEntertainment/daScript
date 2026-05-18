---
slug: inlining-a-sort-comparator-key-body-does-it-double-the-work-for-expensive-keys-compared-to-keeping-the-key-lambda-call
title: inlining a sort comparator key body — does it double the work for expensive keys compared to keeping the key lambda call
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**No. Both forms evaluate the key body twice per comparison.** The intuition "if I inline the body twice, expensive keys do double work" is wrong — the original `$(v1, v2) => _::less(key(v1), key(v2))` already calls `key` twice per comparison (once for each side).

**What inlining actually saves:** indirect lambda-dispatch overhead. Concretely, for each comparison `cmp(v1, v2)`:

| Step | Before inline (key-taking) | After inline (body spliced) |
|---|---|---|
| comparator call | 1 indirect call (block dispatch) | 1 indirect call (block dispatch) |
| `key(v1)` | 1 indirect call (lambda dispatch) | 0 — body[v1] inlined |
| `key(v2)` | 1 indirect call (lambda dispatch) | 0 — body[v2] inlined |
| body evaluation | 2× (once per `key(v)`) | 2× (inlined into both sides) |
| `_::less` | 1 direct call | 1 direct call |
| **Net dispatches** | **3** | **1** |

So the win is **2 fewer indirect dispatches per comparison**. For trivial keys (`$(_) => _.price`) where the body IS a single load, the dispatch IS the dominant cost — and the savings are big (PR #2714: sort_take m3f 56 → 27 ns/op, ~2.1×). For expensive keys (`$(_) => some_real_work(_)`) the body still runs twice either way, so the relative win shrinks to the dispatch overhead alone — small win compared to the work.

**`has_sideeffects` is a SEMANTIC gate, not a perf gate.** It blocks substitution when the body has observable side effects, because `replaceVariable` doesn't preserve the typer-inserted ExprRef2Value wrappers and ordering guarantees that side-effecty expressions rely on — moving them around via macro substitution can change observable behavior. Has nothing to do with "would double the expensive work."

**Schwartzian transform is the orthogonal optimization** that DOES help expensive keys: precompute `key(v)` once per element into a paired array, then sort the paired array by the precomputed scalar. Neither path here does that — both the original keyed top_n_by and the inlined `top_n_by_with_cmp` re-evaluate the key per comparison. Schwartzian would be a separate library entry / macro layer; not in scope for PR #2714.

**Canonical example:** `try_make_inline_cmp` in `daslib/linq_fold.das` (PR #2714). The `try_make_inline_cmp` helper's docstring spells this out in detail with the dispatch-count table.

Linked: [[pr-2714-linq-splice-single-eval]], [[qmacro-multi-arg-block-declaration-with-i-name-splices-fails-with-error-30701]].

## Questions
- inlining a sort comparator key body — does it double the work for expensive keys compared to keeping the key lambda call
