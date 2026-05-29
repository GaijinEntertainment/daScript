# Performance Lint Rules (`daslib/perf_lint.das`)

## Overview

The `perf_lint` module detects common performance anti-patterns in daslang code at compile time. It uses a `[lint_macro]` AST pass that walks the compiled program looking for known-slow patterns and reports them as `CompilationError::runtime_macro_performance` (error code 31208).

## Architecture

- **Module:** `daslib/perf_lint.das` — `module perf_lint shared private`
- **Entry point:** `[lint_macro] class PerfLintMacro : AstPassMacro` calls `perf_lint(prog, true)`
- **Visitor:** `class PerfLintVisitor : AstVisitor` — walks the AST with loop depth tracking
- **Error reporting:** `macro_performance_warning(compiling_program(), at, message)` — reports as error code 31208
- **Utility:** `utils/lint/main.das` — unified lint checker (all 3 passes: paranoid, perf, style)
- **Tests:** `utils/lint/tests/` — one file per rule with `expect 31208:N`

## How to Add a New Rule

### 1. Choose a rule ID

Rules are numbered sequentially: `PERF001`, `PERF002`, etc. Pick the next available number.

### 2. Add the detection logic to `PerfLintVisitor`

Override the appropriate visitor method(s). Common patterns:

| What you're looking for | Override method |
|---|---|
| Function call (e.g., `character_at`) | `preVisitExprCall` |
| Binary operator (e.g., `+=`) | `preVisitExprOp2` (check `expr.op`) |
| Assignment/copy | `preVisitExprCopy`, `preVisitExprMove` |
| String interpolation | `preVisitExprStringBuilder` |
| While condition | Set `in_while_cond` flag in `preVisitExprWhile` |
| Inside a loop | Check `loop_depth > 0` |
| Loop iteration variable | Check `loop_variables` stack |
| Variable defined outside loop | Compare variable's scope with current loop scope |

### 3. Report the warning

```das
self->perf_warning("PERFxxx: description; suggested fix", expr.at)
```

The `perf_warning` method handles both compile-time (`macro_performance_warning`) and runtime (`print`) modes.

### 4. Write the test file

Create `utils/lint/tests/perfXXX_rule_name.das`:

```das
options gen2
// PERF0xx: Brief rule title
//
// Problem: What the bad pattern does and why it's slow.
//
// Bad pattern:
//   var result = ""
//   for (i in range(100)) { result += "x" }  // O(n^2)
//
// Good pattern:
//   var result = build_string() <| $(var w) {
//       for (i in range(100)) { write(w, "x") }
//   }

expect 31208:N  // where N = number of warnings this file should produce

require daslib/perf_lint

def bad_example() {
    // ... code that triggers the warning
}

def good_example() {
    // ... correct code, no warning
}
```

### 5. Update documentation

Add the rule to `doc/source/reference/language/perf_lint.rst` with a brief example.

## Visitor State

The visitor tracks:

- `loop_depth : int` — nesting level (0 = not in a loop). Incremented for `for` and `while`.
- `var_stack : array<tuple<v:Variable?; depth:int; is_iter:bool>>` — unified variable stack tracking all declared variables with their loop depth and whether they are loop iteration variables
- `scope_stack : array<int>` — saves `var_stack` length on block/loop entry for pop-on-exit
- `in_while_cond : bool` — true while visiting the condition expression of a `while` loop
- `in_closure : int` — closure depth, to avoid false positives from lambdas/blocks defined inside loops (they execute later, not in the loop)

## Key AST Patterns

### Checking if a variable is defined outside the current loop

Scan `var_stack` for the matching `Variable?` pointer — compare `entry.depth < loop_depth`. Variables not on the stack (function arguments, globals) are treated as outside.

### Checking if an expression references a specific variable

Walk the expression tree (or check `ExprVar.variable` pointer equality) to match against known loop variables or outer-scope string variables.

### Checking expression types

After compilation, `Expression._type` is resolved. Check `expr._type.baseType == Type.tString` for string-typed expressions.

## Existing Rules Reference

| ID | Pattern | Severity | Message |
|---|---|---|---|
| PERF001 | `str += "..."` in loop | High | O(n^2) string allocation; use `build_string()` |
| PERF002 | `character_at(s, i)` in loop with loop var index | High | O(n) per call; use `peek_data()` |
| PERF003 | `character_at` anywhere | Info | O(n) due to strlen; consider `peek_data()` |
| PERF004 | `str = "{str}..."` in loop | High | O(n^2) string interpolation; use `build_string()` |
| PERF005 | `length(str)` in while condition | Medium | strlen recomputed each iteration; cache it |
| PERF006 | `push`/`emplace` in loop without `reserve()` | Medium | repeated reallocations; `reserve()` before loop |
| PERF007 | `string(das_string)` in comparison | Medium | unnecessary allocation; das_string supports == directly |
| PERF008 | `get_ptr(x)` for `is`/`as` type checks | Low | unnecessary; smart_ptr supports type checks directly |
| PERF009 | `var x <- expr; return <- x` | Low | redundant move-init; use `return <- expr` directly |
| PERF010 | `get_ptr(x) == null` | Low | unnecessary; smart_ptr supports == null directly |
| PERF011 | `get_ptr(x).field` | Low | unnecessary; smart_ptr auto-dereferences for field access |
| PERF012 | `find(string(das_string), ...)` | Medium | unnecessary allocation; use `peek(das_string)` instead |
| PERF013 | `a += 1` / `a -= 1` (six numeric scalars) | Low | use postfix `a++` / `a--` (single SimNode, idiomatic) |
| PERF014 | **exact** char-class ranges only: digit `c >= '0' && c <= '9'` → `is_number`; both-case compound `(c >= 'a' && c <= 'z') \|\| (c >= 'A' && c <= 'Z')` → `is_alpha` (is_alpha is *defined* as this union). Plus De Morgan negations (out-of-range forms → `!is_number` / `!is_alpha`). Single-case ranges (`'a'..'z'` alone) NOT flagged — no `is_lower`/`is_upper` helper exists, so no exact rewrite. Hex (`'a'..'f'`) and `&&`-strict intersection (`c > '0' && c < '9'`) also skipped | Info | `strings::is_number` / `is_alpha` |
| PERF015 | ternary min/max (`a < b ? a : b`) | Low | use `math::min(a, b)` / `max(a, b)` |
| PERF016 | ternary abs (`x < 0 ? -x : x`) | Low | use `math::abs(x)` (negabs `x < 0 ? x : -x` not flagged) |
| PERF017 | `length(x) == 0` / `> 0` / `>= 1` etc. | Medium | use `empty(x)` / `!empty(x)`; avoids strlen on strings |
| PERF018 | `for (i in range(length(arr))) { ... arr[i] ... }` (where `i` only indexes `arr`) | Medium | use `for (c in arr) { ... c ... }`; direct iteration drops the index |
| PERF019 | `int(T.a) \| int(T.b)` on the same bitfield (or enum with `operator \|` overload) | Low | collapse to `int(T.a \| T.b)` — one cast instead of two. **Const-foldable forms only fire under lint policies** (`no_optimizations`/`no_infer_time_folding`); dastest validates runtime forms only. Enum-overload probe iterates `program_for_each_module` + `for_each_function(mod, "\|")`, cached per-enum-type on the visitor |
| PERF020 | `T(x)` where `x` is already workhorse type `T` (15 names: `int`/`int8`/`int16`/`int64`, `uint`/`uint8`/`uint16`/`uint64`, `float`, `double`, `string`, `bitfield`/`bitfield8`/`bitfield16`/`bitfield64`) | Low | drop the cast — it's a no-op. Match: `call.func.fromGeneric?.name ?? call.func.name` is in the workhorse-cast set AND `arg._type.baseType` equals the cast's target type (Ref/Const/Temp qualifiers ignored). User-named bitfield/enum constructors (`MyBitfield(x)`, `MyEnum(x)`), vector constructors (`int2`/`float3`/…), and `string(das_string)` are out of scope by construction |
| PERF022 | `for (s in A) { B \|> push(s) }` / `push_clone(s)` where body is exactly that one statement, single iter-var, source is `array<T>` / C-array, destination is `array<T>` | Medium | use `B \|> push_from(A)` / `push_clone_from(A)` (bulk reserve+copy in `daslib/builtin.das`). Single-name `push`/`push_clone` is overloaded between single-element and bulk forms (ambiguous when destination is `array<T[]>`); the `_from` suffix names the bulk intent. Warns at the for-loop's `.at` rather than the inner push's `.at` to avoid colliding with PERF006 under `perf_warning`'s same-location dedup. `emplace` is out of scope: for-iter-var is const-ref, but `emplace` requires var-ref, so the hand-rolled shape doesn't compile. Range/iterator/generator sources are not flagged — no bulk overload to migrate to |
| PERF023 | `var X = clone_expression(E)` whose only uses are `$e(X)` splice tags inside `qmacro` / `qmacro_block` / `qmacro_expr` / `qmacro_block_to_array` bodies | Medium | drop the pre-clone, inline `$e(E)` at each splice site. `apply_template` (templates_boost.das:251) calls `clone_expression` on every substitution input, so the user-side pre-clone is wasted work. Detection: post-expansion, `$e(X)` becomes `add_ptr_ref(X)` inside an `ExprMakeBlock`; the visitor tracks `perf023_splice_depth++` on entry to any `add_ptr_ref` call and classifies each candidate's `ExprVar` reference as "safe" if `depth > 0` else "disqualified". Fires only when ALL uses are safe AND at least one is observed. Multi-clone-of-same-source is still flagged (e.g. `var a = clone_expression(takeExpr); var b = clone_expression(takeExpr)` for two `$e(...)` slots): inlining `$e(takeExpr)` at both slots preserves "N independent clones" semantics because apply_template clones each substitution. The lint runs unconditionally inside closures because the rules-block IS a closure (`apply_qrules` builds an `ExprBlock` with `isClosure` flag). Closure-scoped *decls* are skipped — only parent-scope `var X = clone_expression(...)` is a candidate. `clone_type` is out of scope (no matching `add_type_ptr_ref` splice wrapper in the user-facing qmacro grammar) |
| PERF024 | `func(clone_*(X))` (Arm A) or `var X = clone_*(E); func(X)` with no other uses (Arm B) where `func` carries a `[clone(paramName)]` annotation at the matching arg position | Medium | drop the outer clone wrap — the callee clones internally. `[clone(p1, p2)]` is a C++-registered metadata-only annotation (zero runtime cost); each annotated function promises to clone the named params internally. Detection in `preVisitExprCallArgument`: compute the callee's annotated param indices, identity-match the current arg's position via `intptr`, peel `ExprRef2Value` wrapper (typer inserts it whenever a ref-typed var — e.g. `var X : Expression?` — is passed to a non-ref param), then either flag `clone_*(...)` directly (Arm A) or mark a bare ExprVar candidate as safe (Arm B). Arm B reuses PERF023's seed-+-classify shape with a per-function `Perf024Candidate` array and `perf024_skip_iptr` flag. Annotation is wrong when the function MUTATES its input in place (e.g. `apply_template` / all `apply_qmacro_*` / `apply_qblock_*` go through `TemplateVisitor` which substitutes in place) — pre-clones at mutating callsites are load-bearing, NOT redundant; verify by reading the body before annotating |
| PERF025 | `"{string(x)}"` — a `string(value)` cast as a direct string-interpolation element, where `value` is stringify-equivalent: signed `int`/`int8`/`int16`/`int64`, `float`, `double`, `string`, `das_string` (`tHandle` + annotation `"das_string"`) | Low | drop the cast — interpolation already converts via the builder's `DebugDataWalker`, so the cast just allocates an intermediate string. Detection in `preVisitExprStringBuilderElement` (per-element hook): `perf025_unwrap_element` peels one `ExprRef2Value`, `perf025_string_call_value` matches a `string(...)` call (skipping explicit `string(x, true)` hex) and returns its value arg, `perf025_value_kind` classifies it. **Unsigned** ints (`uint`/`uint8`/`uint16`/`uint64`) still warn but append a `:d`-tag hint — they interpolate as **hex** by default (`"{42u}"` → `0x2a`), unlike `string()`'s decimal, so the bare drop changes output. **Skipped (kind 0):** `array<uint8>`/`uri`/`text_range` (value-shape change, not just format), `string()` nested as an arg to another call (`"{length(string(x))}"` — only direct elements are flagged), and any type with no `string()` overload. Reports at the `string` call's `.at`; for a `string(string)` arg PERF025 claims the location and the overlapping PERF020 is deduped. `string(x)` under a `_::fmt(":fmt", …)` format wrapper cannot occur — `fmt` has no string-valued overload, so `"{string(x):fmt}"` does not compile |

## Visitor gotchas

- **`in_closure > 0` is NOT a useful guard in `preVisitExprOp2`** — `loop_depth` already doesn't increment inside closure bodies (`preVisitExprFor` / `While` gate on `in_closure == 0`), so PERF001's `loop_depth > 0` correctly excludes closure-internal loops without a separate skip. An `in_closure` early-return at the top of `preVisitExprOp2` hides syntactic patterns (PERF007/008/010/013/014/017) inside the natural `build_string() $(var w) { ... }` idiom and is a bug, not a feature.
- **Macro-generated functions need `current_function.flags.generated`-suppression** — `[CommandLineArgs]`-style codegen synthesizes AST that the user never wrote. Both `perf_warning` and `style_warning` should early-return when `current_function.flags.generated` is true. Otherwise warnings surface at the source-struct's line with no clear way to fix them.
- **Self-implementation suppression** — when a rule's suggested replacement is itself implemented in terms of the pattern (e.g. `empty(arr)`'s body is literally `length(arr) == 0`), gate the rule with `current_function.name == "<callee>" || (fromGeneric != null && fromGeneric.name == "<callee>")`. The generic-instantiation arm catches `empty<int>`, `empty<MyType>`, etc.
