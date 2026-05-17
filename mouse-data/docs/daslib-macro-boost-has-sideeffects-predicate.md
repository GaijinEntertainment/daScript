---
slug: daslib-macro-boost-has-sideeffects-predicate
title: Is there a conservative side-effect detector for Expression nodes in daslib macro_boost — something I can call from a call_macro to know if it's safe to elide an evaluation at macro time?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Yes — `has_sideeffects(expr : Expression?) : bool` in `daslib/macro_boost` (added in PR #2691, follow-up to Phase 2A loop planner). Returns `true` if the expression has or **might have** side effects; `false` ONLY when provably pure.

```das
require daslib/macro_boost public

if (has_sideeffects(projection)) {
    // Emit the bind — projection must run for its side effects.
    sideEffectStmts |> push <| qmacro_expr() {
        var $i(finalBindName) = $e(projection)
    }
} else {
    // Skip the bind — pure projection, no observable effect.
}
```

**Conservative — false is a promise:**

- SAFE leaves: `ExprVar`, all `ExprConst*`, `ExprAddr`, `ExprTypeInfo/Decl/Tag`.
- SAFE via recursion: `ExprField`, `ExprSafeField`, `ExprSwizzle`, `ExprRef2Value/Ptr`, `ExprPtr2Ref`, `ExprIs`, `ExprAsVariant`, `ExprIsVariant`, `ExprSafeAsVariant`, `ExprCast`, `ExprNullCoalescing`, `ExprStringBuilder` (string heap is no-op per compiler), `ExprKeyExists` (pure container read).
- `ExprAt`: safe when `subexpr._type` is NOT `isGoodTableType` (tables auto-insert default on missing key — a write). `ExprSafeAt` (`?[...]`) always safe.
- `ExprOp1/Op2/Op3`: op-name allowlist for pure ops on workhorse types — `+ - * == != < <= > >= & | ^ << >> && || ?:` (Op2), `- ! ~ +` (Op1). Falls back to `func.flags.builtIn && !knownSideEffects && !unsafeOperation`. `/` and `%` BLACKLISTED (div-by-zero panic).
- `ExprCall`/`ExprCallFunc`: allowed when `func.flags.builtIn && !knownSideEffects && !unsafeOperation`, then recurse args.
- Everything else (including `ExprNew`, all `ExprMake*`, user-defined calls, `ExprInvoke`, `ExprYield`, statement-context exprs): UNSAFE.

**Known limitations / when it returns conservative-unsafe:**

- daslang-generic helpers like `length(arr)` and `key_exists(tab, k)` — the resolved `func.name` is the mangled instance, and the typer hasn't always reached the `flags.builtIn=true` C++ overload before the call_macro fires. They show up as user-call shapes and get rejected. Workaround: don't rely on this for length/key_exists in projections (they appear in `has_sideeffects` tests as `target_generic_length_unresolved` / `target_key_exists_unresolved` returning `true`).
- User-defined pure helpers — there's no `[no_side_effects]` annotation yet. The compiler's `expr.flags.noSideEffects` fast path catches some cases (set during infer), but anything the typer didn't tag falls through to UNSAFE.

**Tests:** `tests/macro_boost/test_has_sideeffects.das` has 24 cases (17 safe + 5 unsafe + 2 conservative-unsafe) wired via a `_test_has_sideeffects` probe `call_macro` ([`tests/macro_boost/_has_sideeffects_probe.das`](../../tests/macro_boost/_has_sideeffects_probe.das)) that runs the predicate at macro time and emits `ExprConstBool` of the result. Use the same probe pattern when testing any new predicate that needs to run at macro time but be exercised via runtime tests.

**Real use:** `daslib/linq_fold.das` `plan_loop_or_count` uses it for three optimizations: discardable `var vfinal =` bind elision, count→length shortcut gate (whole loop elided when no filter + all projections pure + source has length), and tracking `allProjectionsPure` across chained selects. select_count benchmark went from 2 → 0 ns/op.

## Questions
- Is there a conservative side-effect detector for Expression nodes in daslib macro_boost — something I can call from a call_macro to know if it's safe to elide an evaluation at macro time?
