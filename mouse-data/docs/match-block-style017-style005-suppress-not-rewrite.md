---
slug: match-block-style017-style005-suppress-not-rewrite
title: match block triggers STYLE017 / STYLE005 — should I rewrite the arms?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

**No. Suppress with `// nolint`, don't rewrite.**

daslang's `match` block parses as a chain of nested `ExprIfThenElse`. The wildcard arm (`if (_) { ... }`) becomes the innermost `else`. From the lint visitor's perspective, the AST is indistinguishable from a hand-written `if/elif/else` chain — but rewriting it back to the "equivalent" form **breaks the match-specific code path** (match has narrowing semantics that `if/elif` doesn't).

**The auto-fixer (`utils/fix-lint-errors`) correctly skips this case** — see the STYLE017(a) handler:
```das
// STYLE017(a) is intentionally NOT auto-fixed: match blocks parse as a
// chain of ExprIfThenElse where the wildcard arm's body becomes the
// innermost else, making them indistinguishable from a real if/else
// at the AST level. Auto-fixing them produces broken match code.
```

But the LINT pass **still reports** the false positive. So when running `utils/lint/main.das` on a file with match blocks you'll get spurious STYLE017 + STYLE005 hits on the arms.

**Suppression pattern** — line-local `// nolint`:

```das
def isConstRedundantForCpp(typeDecl : TypeDeclPtr) {
    if (!empty(typeDecl.dim)) return false;
    if (typeDecl.isVectorType) return true;
    match (typeDecl.baseType) {
        if (Type.tBool) { return true; }
        // ...
        if (Type.tBitfield64) { return true; }  // nolint:STYLE017 (match arm chain)
        if (_) { return false; }
    }
}
```

- **STYLE017** fires on the LAST non-wildcard arm (it sees `if (cond) return true else return false` after the wildcard collapses into the else position). Put `nolint:STYLE017` on that line.
- **STYLE005** fires on every `if (cond) { return X }` arm. If many arms in one match, add `nolint:STYLE005` to each — or refactor to single-line `if (cond) return X` per arm (still parses inside `match`).

**Where this matters:** `daslib/aot_cpp.das` `isConstRedundantForCpp`, `isPolicyBasedCall` — both wrap a `match` on `Type.tXXX` enum cases. Any similar enum-dispatch match pattern.

**Reminder of correct match grammar:** the wildcard arm uses `if (_) { ... }`, not `else { ... }` or `default { ... }`. There's exactly one wildcard, always last.

## Questions
- match block triggers STYLE017 / STYLE005 — should I rewrite the arms?
