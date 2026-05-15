---
slug: lint-runner-disables-folding-but-dastest-does-not
title: Why doesn't my [lint_macro] AST visitor see `int(BfT.a) | int(BfT.b)` (or any constant-only expression) — the ExprOp2 seems to be gone before my visitor runs?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Because **dastest runs with normal compile policies**, where the optimizer constant-folds `int(BfT.a) | int(BfT.b)` to a single `ExprConstInt` before the lint pass. Your visitor never sees the un-folded form.

**The lint runner (`utils/lint/main.das`) sets three `CodeOfPolicies` flags that preserve the AST:**

```das
cop.lint_check = true              // mark program as lint-only
cop.no_optimizations = true        // skip the optimizer (REF/CONST/COND/BLOCK folding)
cop.no_infer_time_folding = true   // skip infer-time const-folding too
```

When you run via `bin/daslang utils/lint/main.das -- file.das`, those flags fire and your `preVisitExprOp2` sees the real `ExprOp2(op="|", left=ExprCall, right=ExprCall)`. When dastest runs the file (no lint policies), folding flattens it.

**Implication for `utils/lint/tests/`**: dastest tests under that directory can't validate rules whose pattern only survives under lint policies. Two options:

1. Test runtime variants in dastest (`int(m1) | int(m2)` where `m1`/`m2` are runtime vars survive folding) and rely on the CI lint-runner gate for constant-case coverage. PERF019's test does this (`expect 31208:3` on three runtime cases).
2. Write a separate test file in `tests/lint_under_policies/` and a runner that compiles via `compile_file(... cop ...)`, but the existing dastest-driven tests don't have this hook.

The `CodeOfPolicies` knobs are bound to RTTI in `src/builtin/module_builtin_rtti.cpp` and called from `lint_file` in `utils/lint/main.das:118-128`.

## Questions
- Why doesn't my [lint_macro] AST visitor see `int(BfT.a) | int(BfT.b)` (or any constant-only expression) — the ExprOp2 seems to be gone before my visitor runs?
