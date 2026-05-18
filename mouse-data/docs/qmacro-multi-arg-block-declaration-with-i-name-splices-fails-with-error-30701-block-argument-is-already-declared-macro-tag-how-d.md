---
slug: qmacro-multi-arg-block-declaration-with-i-name-splices-fails-with-error-30701-block-argument-is-already-declared-macro-tag-how-d
title: qmacro multi-arg block declaration with $i name splices fails with error 30701 block argument is already declared MACRO TAG how do I emit a typed block with multiple distinct argument names
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Root cause:** `src/parser/parser_impl.cpp:ast_makeBlock` does a parse-time dup check on block-arg names via `closure->findArgument(name_at.name)`. The parser stamps every `$i(expr)` in block-arg-decl position with the literal placeholder name `"``MACRO``TAG``"` (the actual name is resolved later, post-parse, by the macro tag visitor). So `qmacro($($i(a) : $t(T), $i(b) : $t(T)) { ... })` parses BOTH args with that same placeholder string and the dup check fires before macro substitution gets a chance to assign distinct names.

**Diagnostic:** error `30701: block argument is already declared ``MACRO``TAG``` while compiling a `[macro_function]` that emits a typed multi-arg block via `qmacro($($i(a) : ..., $i(b) : ...) { ... })`.

**Two workarounds + one upstream fix:**

1. **Upstream fix** (PR #2714 commit `bd76c588f`): in `parser_impl.cpp:885`, skip the dup check when `name_at.tag != nullptr` — tagged names are deferred-resolved by the macro processor, and genuine post-resolution dups surface as ordinary local-lookup conflicts during type inference. Three lines + a comment. General-purpose.

2. **Hybrid emission** (works pre-fix): emit the block via qmacro with ONE typed `$i`-named arg, then append the second as a fresh `Variable` manually:
   ```das
   var cmpMake = qmacro($(v1 : $t(elemType) -&) {
       return $e(cmpExpr)
   }) as ExprMakeBlock
   var cmpBody = cmpMake._block as ExprBlock
   var v2Var = new Variable(at = at, name := "v2", _type = clone_type(elemType))
   v2Var._type.flags.removeConstant = true
   v2Var._type.flags.ref = true
   cmpBody.arguments |> emplace_new(v2Var)
   return cmpMake
   ```
   Fragile because the flag bookkeeping for the second var must match what qmacro's `$t(T) -&` produces for the first — different modifier ordering produces different rendered shapes (`Car -&` vs `Car&` vs `Car const -&`).

3. **Untyped block args** (the cleanest for most use cases — what PR #2714 ships): drop type annotations on both args and let the typer infer from the call-site signature:
   ```das
   return qmacro($(v1, v2) {
       return $e(cmpExpr)
   })
   ```
   The typer resolves `v1`/`v2` types from the function the block gets passed to (e.g. `block<(v1 : TT -&, v2 : TT -&) : bool>` in `top_n_by_with_cmp(arr, n, cmp)`). Works because the block-arg-decl path here is a SOURCE-LEVEL block expression in qmacro, NOT an `$i`-tagged form, so the parser sees distinct `v1` and `v2` identifiers and the dup check passes naturally.

**When to pick which:** untyped (#3) sidesteps both the dup-check bug AND the const-ref flag-bookkeeping fragility — recommended for any case where the typer can infer from context. Typed multi-arg `$i` (#1, post-fix) is for cases where you genuinely need the spliced name from a string variable (gensym from `at.line`/`at.column`) AND need explicit typing.

See PR #2714 and the `try_make_inline_cmp` helper in `daslib/linq_fold.das` for the canonical example.

## Questions
- qmacro multi-arg block declaration with $i name splices fails with error 30701 block argument is already declared MACRO TAG how do I emit a typed block with multiple distinct argument names
