---
slug: why-does-my-4-ary-block-call-return-stale-first-param-values-when-the-lambda-uses-param-names-p-q-r-s
title: Why does my 4-ary block call return stale first-param values when the lambda uses param names `p, q, r, s`?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Status:** INTERMITTENT — observed 3x consistently during initial PR-1 work, but became non-reproducible after unrelated intermediate edits (notably adding a `static_if` reserve in `select_many_impl`). Standalone reproductions outside the test file with various complexity (multi-iter for + lambda directly, public-wrapper shape, nested in test-framework lambda) all returned correct values. Likely a daslang generic-instantiation order / codegen issue with preconditions that can't be pinned down from outside the failing context.

**Symptom (when triggered):** Calling a `block<(l: TT-&; r: UU-&; w: WW-&; x: XX-&)>` with a lambda `$(p: int; q: int; r: int; s: int) => p + q + r + s` returns wrong sums — the first lambda param `p` evaluates to its initial value (0) on EVERY iteration even though the block call site passes it correctly each time. Probes of `=> p` alone return the correct sequence `[0, 1, 2]`; only the multi-param expression `p + q + r + s` exhibited the bug.

**Repro:** Surfaced in `daslib/linq.das` 4-ary `zip` impl during PR-1 of the linq zip extension. Calling the result-selector form with `$(p: int; q: int; r: int; s: int) => p + q + r + s` over four iterators yielding `[0,1,2]`, `[10,11,12]`, `[100,101,102]`, `[1000,1001,1002]` returns `[1110, 1113, 1116]` instead of `[1110, 1114, 1118]`. Delta-per-row is 3 instead of 4 — consistent with first source stuck at 0.

**Workaround:** Rename the lambda params away from `p, q, r, s`. Both `(l: int; r: int; w: int; x: int)` (identical-to-block-sig names) AND `(aa: int; bb: int; cc: int; dd: int)` (totally distinct names) work fine. The bug is SPECIFIC to the name set `p, q, r, s` — not a generic shadowing issue.

**Root cause:** UNKNOWN. Confirmed it is NOT:
- Lambda multi-param shared-type `(p, q, r, s : int)` (explicit per-param annotations fail identically)
- Block-vs-lambda param name shadowing (3-ary with identical names works fine; 4-ary with identical-to-block names ALSO works fine)
- Block param name `x` colliding with `xx` witness or `[iterator for (x in ...)]` comprehension var (rename `x → m` doesn't fix it)
- Arity (3-ary works with any names; 4-ary breaks with `p,q,r,s` only)
- Source iterator type (probes via `=> p` etc. return correct sequences per source)

Likely some interaction between daslang's lambda body evaluation order, ref binding to block ref-params, and the specific identifier set `p/q/r/s`. Possibly related to closures, register allocation, or symbol lookup in the lambda body. Needs a minimal repro outside the linq layer + a daslang issue.

**Test discipline:** Avoid `p, q, r, s` as lambda param names in tests for zip-family functions of arity ≥ 4 until the daslang bug is identified and fixed. Use `aa, bb, cc, dd` or descriptive names. Same caution likely applies to higher arities until investigated.

**Discovered:** 2026-05-19, PR-1 of linq zip extension (branch `bbatkin/linq-zip-n-ary`).

## Questions
- Why does my 4-ary block call return stale first-param values when the lambda uses param names `p, q, r, s`?
