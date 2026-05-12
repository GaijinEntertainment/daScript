---
slug: style005-fires-after-const-folded-dead-branch
title: Why does STYLE005 fire on a multi-statement block where one statement is `if (CONST) ...`?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

**Compile-time const folding eliminates the dead branch, leaving a single-statement block.**

Reproducer:
```das
let verbose = false  // compile-time const

def onInner(var root) {
    // ...
    if (skip) {
        if (verbose) print("// skipping {decl[\"name\"] as _string}\n")
        continue
    }
}
```

The block `{ if (verbose) print(...); continue }` has two statements in source. But `verbose` is a `let` initialized to `false`, so const-folding drops the `if (verbose) print(...)` entirely — the block becomes `{ continue }`, a single-terminator block, and STYLE005 fires.

**Reported through a generic instantiation:** the lint warning shows up via `instanced from <some_generic>` chain, not at the source `if (skip)` line directly. That makes it look like a downstream consumer's bug — but the issue is in *your* file's `let CONST = false` + conditional-print pattern.

**Symptom on `if`-postfix form too:**
```das
if (skip) {
    print(...) if (verbose)   // postfix-if; still desugars to ExprIf, still folded
    continue
}
```
Postfix `if` parses to the same ExprIfThenElse shape, so const-folding still eliminates it. STYLE005 still fires.

**Fix:** suppress at the call-site line with inline `// nolint:STYLE005`:
```das
if (skip) {  // nolint:STYLE005 (const-folded verbose-print leaves single continue)
    print("// skipping {decl[\"name\"] as _string}\n") if (verbose)
    continue
}
```

Don't rewrite to `if (skip) continue` — that loses the `verbose=true` path. Don't move the print outside the `if (skip)` — that changes semantics (you'd print before the skip happens).

**Why this isn't a lint bug:** STYLE005 inspects the post-folding AST shape, which is the right thing for most cases. The const-fold case is rare enough that a per-line suppression is cleaner than complicating the rule.

Seen in: `daslib/cpp_gen.das:225` (`let verbose = false` at module top + `if (verbose) print(...)` inside `if (skip) { ... continue }` body).

## Questions
- Why does STYLE005 fire on a multi-statement block where one statement is `if (CONST) ...`?
