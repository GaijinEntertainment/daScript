---
slug: qmacro-gensym-per-callsite-via-lineinfo
title: How do I generate a uniquely-named gensym inside an AstCallMacro for a per-call-site variable, using LineInfo?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

Use the call site's `LineInfo.line` + `.column` interpolated into a backtick-prefixed identifier string. Backtick-prefixed names live in a separate namespace so they don't collide with user-typed identifiers and they survive lint/style passes.

```das
def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
    let at = call.at  // LineInfo of the call site
    let accName = "`acc`{at.line}`{at.column}"
    let itName  = "`it`{at.line}`{at.column}"
    let srcName = "`src`{at.line}`{at.column}"

    var res = qmacro(invoke($($i(srcName) : typedecl($e(src))) {
        var $i(accName) = 0
        for ($i(itName) in $i(srcName)) {
            // ...
        }
        return $i(accName)
    }, $e(src)))
    res.force_at(at)
    res.force_generated(true)
    return res
}
```

Two follow-up steps you almost always want:

1. `res.force_at(at)` + `res.force_generated(true)` — sets `at = call.at` on every emitted node and marks them macro-generated. The latter bypasses lint rules that would otherwise fire on synthesized code (e.g. STYLE001, LINT002 "unused variable").
2. `(blk._block as ExprBlock).arguments[0].flags.can_shadow = true` on the bound let-variable — quiets shadow warnings if the user already has an `acc`/`it`/`src` in scope. Reach for `.flags.can_shadow` on any qmacro-bound name that might collide with caller context.

**Why include both line AND column:** macros emitted from nested helpers can have several emission sites on the same line (e.g. piped chains where each `|>` step emits a separate gensym). Line alone is not unique.

**Why backtick prefix:** the backtick is a daslang lexer hint that this is an internal/synthesized name. Without it, very-long generated names sometimes clash with user identifiers or trip naming rules (the formatter, the auto-rename tools).

**Worked example:** `daslib/linq_fold.das` `plan_loop_or_count` — multiple gensyms per emission site (accumulator, iterator, source, bound projection). Variants per fold-helper too (`fold_where_count` uses `nName` over `accName`).

## Questions
- How do I generate a uniquely-named gensym inside an AstCallMacro for a per-call-site variable, using LineInfo?
