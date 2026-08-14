# Performance Lint Rules (`daslib/perf_lint.das`)

## Overview

The `perf_lint` module detects common performance anti-patterns in daslang code at compile time. It uses a `[lint_macro]` AST pass that walks the compiled program looking for known-slow patterns and reports them as `CompilationError::runtime_macro_performance` (error code 31208).

## Architecture

- **Module:** `daslib/perf_lint.das` — `module perf_lint shared private`
- **Entry point:** `[lint_macro] class PerfLintMacro : AstPassMacro` calls `perf_lint(prog, true, build_lint_macro_disabled(prog))` — the policy table comes from `daslib/lint_config` (default-off seeds + `.lint_config` + `$DAS_LINT_DISABLE`)
- **Visitor:** `class PerfLintVisitor : AstVisitor` — walks the AST with loop depth tracking
- **Error reporting:** `macro_performance_warning(compiling_program(), at, message)` — reports as error code 31208
- **Utility:** `utils/lint/main.das` — unified lint checker (all 3 passes: paranoid, perf, style)
- **Tests:** `utils/lint/tests/` — one file per rule with `expect 31208:N`

## Lint policy and entry points

Rule enablement is not per-rule bespoke logic — it goes through the shared policy layer in
`daslib/lint_config.das`, which every runner uses:

- `seed_default_disabled(disabled)` — seeds the canonical default-off set. A rule that should
  ship silent goes here, nowhere else.
- `load_lint_config(...)` — layers the repo `.lint_config` TOML `[rules]` table on top, so a
  `PERF007 = false` / `STYLE005 = true` directive can override either direction.
- `build_lint_macro_disabled(prog)` — what the three `[lint_macro]` `apply()` methods call. It
  honors the module-local `options _enable_default_off_rules = true` opt-in — fixture mode: skips
  BOTH the default-off seed and the repo config's `= false` directives, so a lint fixture always
  exercises its rule regardless of repo policy — then layers `.lint_config` `= true` re-enables
  and `$DAS_LINT_DISABLE` last.
- `$DAS_LINT_DISABLE` — a whole-run denylist, no source edit.
- CLI: `--disable CODE,...` / `--enable CODE,...` on `utils/lint/main.das`. `--enable` is a
  whitelist (only listed rules run); on overlap `--disable` wins.

Five public entry points, all in `daslib/perf_lint.das`:

```das
def public perf_lint(prog; compile_time_errors) : int
def public perf_lint(prog; compile_time_errors; disabled_codes : table<string>) : int
def public perf_lint_collect(prog; var warnings : array<string>) : int
def public perf_lint_collect(prog; var warnings; disabled_codes, enabled_codes : table<string>) : int
def public perf_lint_collect_issues(prog; var issues : array<LintIssue>; disabled_codes, enabled_codes) : int
```

The two-argument forms are thin wrappers; every real caller (the MCP subtool, `utils/lint/main.das`,
the LSP) uses a filtered overload. `perf_lint_collect_issues` is the structured form — `LintIssue`
(defined in `daslib/lint_config.das`) carries the rule code and position, which is what the LSP
needs to place a diagnostic.

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
perf_warning("PERFxxx: description; suggested fix", expr.at)
```

Call it bare — the compiler promotes a bare method call inside a class method, and `self->perf_warning(...)` trips this tree's own STYLE028.

The `perf_warning` method handles both compile-time (`macro_performance_warning`) and runtime (`print`) modes.

### 4. Write the test file

Create `utils/lint/tests/perfXXX_rule_name.das`:

```das
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
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

Nearly every fixture under `utils/lint/tests/` carries the `auto_inline_functions` line — without it, inline splices rewrite the shapes the fixture asserts and the warning counts go flaky.

### 5. Update documentation

Add the rule to `doc/source/reference/language/lint.rst` — one file covers LINT, PERF and STYLE — with a brief example (repo-only).

## Visitor State

The visitor tracks:

- `loop_depth : int` — nesting level (0 = not in a loop). Incremented for `for` and `while`.
- `var_stack : array<VarStackEntry>` — unified variable stack (`@do_not_delete v : Variable?`, `depth`, `is_iter`) tracking every declared variable with its loop depth and whether it is a loop iteration variable
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
| PERF003 | `character_at` anywhere | Info | index-0 form (`character_at(s, 0)`) → `first_character(s)`; otherwise O(n) due to strlen, consider `peek_data()` |
| PERF004 | `str = "{str}..."` in loop | High | O(n^2) string interpolation; use `build_string()` |
| PERF005 | `length(str)` in while condition | Medium | strlen recomputed each iteration; cache it |
| PERF006 | `push`/`push_clone`/`emplace` into an outer-scope array inside a **known-length** loop without a prior `reserve()` | Medium | repeated reallocations; `reserve()` before the loop. Silent inside closures, under an `if`, or when the loop has `break`/`continue` (iteration count unpredictable) |
| PERF007 | `string(das_string)` in comparison | Medium | unnecessary allocation; das_string supports == directly |
| PERF008 | `get_ptr(x)` for `is`/`as` type checks | Low | unnecessary; smart_ptr supports type checks directly |
| PERF009 | `var x <- expr; return <- x` (move-init) **or** `var x := src; return <- x` (clone-init, lowered to `<- clone_to_move(...)`) | Low | move flavor → `return <- expr`; clone flavor → `return clone_to_move(src)` (NOT `return <- src`, which would move/destroy the source). `pending_move_is_clone` flag (set when `v.init` is an `ExprCall` to `clone_to_move`) selects the message. Composes with STYLE032 (`push_from`→`:=`). |
| PERF010 | `get_ptr(x) == null` | Low | unnecessary; smart_ptr supports == null directly |
| PERF011 | `get_ptr(x).field` | Low | unnecessary; smart_ptr auto-dereferences for field access |
| PERF012 | `find(string(das_string), ...)` | Medium | unnecessary allocation; use `peek(das_string)` instead |
| PERF013 | `a += 1` / `a -= 1` (six numeric scalars) | Low | use postfix `a++` / `a--` (single SimNode, idiomatic) |
| PERF014 | **exact** char-class ranges only: digit `c >= '0' && c <= '9'` → `is_number`; both-case compound `(c >= 'a' && c <= 'z') \|\| (c >= 'A' && c <= 'Z')` → `is_alpha` (is_alpha is *defined* as this union). Plus De Morgan negations (out-of-range forms → `!is_number` / `!is_alpha`). Single-case ranges (`'a'..'z'` alone) NOT flagged — no `is_lower`/`is_upper` helper exists, so no exact rewrite. Hex (`'a'..'f'`) and `&&`-strict intersection (`c > '0' && c < '9'`) also skipped | Info | `strings::is_number` / `is_alpha` |
| PERF015 | ternary min/max (`a < b ? a : b`) | Low | use `math::min(a, b)` / `max(a, b)` |
| PERF016 | ternary abs (`x < 0 ? -x : x`) | Low | use `math::abs(x)` (negabs `x < 0 ? x : -x` not flagged) |
| PERF017 | `length(x) == 0` / `> 0` / `>= 1` etc. | Medium | use `empty(x)` / `!empty(x)`; avoids strlen on strings |
| PERF018 | `for (i in range(length(arr))) { ... arr[i] ... }` (where `i` only indexes `arr`). Outer call may be any of `range`/`urange`/`range64`/`urange64`, inner `length` or `long_length`, with one optional cast layer (`ExprCast` or 1-arg workhorse int-cast call) on either — `range64(long_length(a))` and `urange(uint(length(a)))` both fire | Medium | use `for (c in arr) { ... c ... }`; direct iteration drops the index |
| PERF019 | `int(T.a) \| int(T.b)` on the same bitfield (or enum with `operator \|` overload) | Low | collapse to `int(T.a \| T.b)` — one cast instead of two. **Const-foldable forms only fire under lint policies** (`no_optimizations`/`no_infer_time_folding`); dastest validates runtime forms only. Enum-overload probe iterates `program_for_each_module` + `for_each_function(mod, "\|")`, cached per-enum-type on the visitor |
| PERF020 | `T(x)` where `x` is already workhorse type `T` (15 names: `int`/`int8`/`int16`/`int64`, `uint`/`uint8`/`uint16`/`uint64`, `float`, `double`, `string`, `bitfield`/`bitfield8`/`bitfield16`/`bitfield64`) | Low | drop the cast — it's a no-op. Match: `call.func.fromGeneric?.name ?? call.func.name` is in the workhorse-cast set AND `arg._type.baseType` equals the cast's target type (Ref/Const/Temp qualifiers ignored). User-named bitfield/enum constructors (`MyBitfield(x)`, `MyEnum(x)`), vector constructors (`int2`/`float3`/…), and `string(das_string)` are out of scope by construction |
| PERF021 | `cond ? T(a) : T(b)` — the same workhorse cast `T` applied on both branches of a ternary | Low | hoist the cast out: `T(cond ? a : b)` — one cast instead of two. Fires anywhere, including in closures |
| PERF022 | `for (s in A) { B \|> push(s) }` / `push_clone(s)` where body is exactly that one statement, single iter-var, source is `array<T>` / C-array, destination is `array<T>` | Medium | use `B \|> push_from(A)` / `push_clone_from(A)` (bulk reserve+copy in `daslib/builtin.das`). Single-name `push`/`push_clone` is overloaded between single-element and bulk forms (ambiguous when destination is `array<T[]>`); the `_from` suffix names the bulk intent. Warns at the for-loop's `.at` rather than the inner push's `.at` to avoid colliding with PERF006 under `perf_warning`'s same-location dedup. `emplace` is out of scope: for-iter-var is const-ref, but `emplace` requires var-ref, so the hand-rolled shape doesn't compile. Range/iterator/generator sources are not flagged — no bulk overload to migrate to |
| PERF023 | `var X = clone_expression(E)` whose only uses are `$e(X)` splice tags inside `qmacro` / `qmacro_block` / `qmacro_expr` / `qmacro_block_to_array` bodies | Medium | drop the pre-clone, inline `$e(E)` at each splice site. `apply_template` (templates_boost.das:407; its substitution visitor clones at :244/:257) calls `clone_expression` on every substitution input, so the user-side pre-clone is wasted work. Detection: post-expansion, `$e(X)` becomes `add_ptr_ref(X)` inside an `ExprMakeBlock`; the visitor tracks `perf023_splice_depth++` on entry to any `add_ptr_ref` call and classifies each candidate's `ExprVar` reference as "safe" if `depth > 0` else "disqualified". Fires only when ALL uses are safe AND at least one is observed. Multi-clone-of-same-source is still flagged (e.g. `var a = clone_expression(takeExpr); var b = clone_expression(takeExpr)` for two `$e(...)` slots): inlining `$e(takeExpr)` at both slots preserves "N independent clones" semantics because apply_template clones each substitution. The lint runs unconditionally inside closures because the rules-block IS a closure (`apply_qrules` builds an `ExprBlock` with `isClosure` flag). Closure-scoped *decls* are skipped — only parent-scope `var X = clone_expression(...)` is a candidate. `clone_type` is out of scope (no matching `add_type_ptr_ref` splice wrapper in the user-facing qmacro grammar) |
| PERF024 | `func(clone_*(X))` (Arm A) or `var X = clone_*(E); func(X)` with no other uses (Arm B) where `func` carries a `[clone(paramName)]` annotation at the matching arg position | Medium | drop the outer clone wrap — the callee clones internally. `[clone(p1, p2)]` is a C++-registered metadata-only annotation (zero runtime cost); each annotated function promises to clone the named params internally. Detection in `preVisitExprCallArgument`: compute the callee's annotated param indices, identity-match the current arg's position via `intptr`, peel `ExprRef2Value` wrapper (typer inserts it whenever a ref-typed var — e.g. `var X : Expression?` — is passed to a non-ref param), then either flag `clone_*(...)` directly (Arm A) or mark a bare ExprVar candidate as safe (Arm B). Arm B reuses PERF023's seed-+-classify shape with a per-function `Perf024Candidate` array and `perf024_skip_iptr` flag. Annotation is wrong when the function MUTATES its input in place (e.g. `apply_template` / all `apply_qmacro_*` / `apply_qblock_*` go through `TemplateVisitor` which substitutes in place) — pre-clones at mutating callsites are load-bearing, NOT redundant; verify by reading the body before annotating |
| PERF025 | `"{string(x)}"` — a `string(value)` cast as a direct string-interpolation element, where `value` is stringify-equivalent: signed `int`/`int8`/`int16`/`int64`, `float`, `double`, `string`, `das_string` (`tHandle` + annotation `"das_string"`) | Low | drop the cast — interpolation already converts via the builder's `DebugDataWalker`, so the cast just allocates an intermediate string. Detection in `preVisitExprStringBuilderElement` (per-element hook): `perf025_unwrap_element` peels one `ExprRef2Value`, `perf025_string_call_value` matches a `string(...)` call (skipping explicit `string(x, true)` hex) and returns its value arg, `perf025_value_kind` classifies it. **Unsigned** ints (`uint`/`uint8`/`uint16`/`uint64`) still warn but append a `:d`-tag hint — they interpolate as **hex** by default (`"{42u}"` → `0x2a`), unlike `string()`'s decimal, so the bare drop changes output. **Skipped (kind 0):** `array<uint8>`/`uri`/`text_range` (value-shape change, not just format), `string()` nested as an arg to another call (`"{length(string(x))}"` — only direct elements are flagged), and any type with no `string()` overload. Reports at the `string` call's `.at`; for a `string(string)` arg PERF025 claims the location and the overlapping PERF020 is deduped. `string(x)` under a `_::fmt(":fmt", …)` format wrapper cannot occur — `fmt` has no string-valued overload, so `"{string(x):fmt}"` does not compile |
| PERF026 | heap traffic on a hot path — reached from a function carrying `[hot_path]` / `[no_alloc]` | High | see **Hot-path contracts** below |
| PERF027 | environment lookup on a hot path (`[hot_path]` / `[no_env]`) | High | `get_env_variable` calls `getenv` AND allocates the result in the context heap on every call; resolve at `[init]` or behind a `[cold_path]` loader and cache |
| PERF028 | console / file I/O on a hot path (`[hot_path]` / `[no_io]`) | Medium | the debug line that never got removed; this is the rule to reach for `DAS_LINT_DISABLE=PERF028` on |
| PERF029 | `for (i in range(length(X)))` where `i` also subscripts bare sibling array vars in lockstep (`ys[i] = xs[i]`); routed from the PERF018 candidate when ≥1 sibling qualifies, same range/length spellings as PERF018 | Medium | zip them: `for (x, y in xs, ys)` — lockstep by construction, no length-coupling OOB risk |
| PERF030 | move-assign `<-` onto a live heap-carrying variable (needs-release type) — drops the old contents without release, a leak on a persistent heap. Skips the `return <- r` CMRES lowering, generated vars, and moves already healed by `force_inscope_pod` (`move_flags.podDelete`) | High | live target: `delete` it first (fixes the leak) + `// nolint:PERF030 — released on the line above` (the syntactic check cannot see the delete); fresh target: fold into the declaration or suppress with the proof; or enable `force_inscope_pod` so the compiler collects it |

## Hot-path contracts (PERF026-028)

Unlike every other rule here, these three are **annotation-gated**: nothing is checked until a
function declares a contract. From each annotated root the scan follows **direct** calls
transitively, so a sink several frames deep is still reported.

**Declaring a contract is free.** The five annotations are registered by the compiler itself as
metadata-only markers (the same shape as `[clone]`), so a file under contract requires *nothing* — no module compiles, no build time is paid. `@scratch` is a
field/parameter/global annotation, which is free-form anyway. The verification lives here, in
`daslib/perf_lint`, which code under contract does **not** require: the checker is heavy (it pulls
`ast_boost`, `lint_config`, `toml`, `json`), and lint already runs where lint belongs —
`utils/lint/main.das`, the MCP `lint` subtool, CI — none of which ever needed the target to require
`perf_lint`. A plain build stays silent and pays nothing.

| Annotation | Bans |
|---|---|
| `[hot_path]` | all three |
| `[no_alloc]` | heap traffic (PERF026) |
| `[no_env]` | environment lookups (PERF027) |
| `[no_io]` | console / file I/O (PERF028) |
| `[cold_path]` | *prunes the walk* — a one-time init or opt-in leg reached from a hot function |

Combine on one line: `[no_alloc, no_env, no_io]`. Two stacked annotation blocks do not parse.

### What counts as heap traffic

Detection is structural rather than a list of surface names. Every array/table heap operation
bottoms out in a `__builtin_array_*` / `__builtin_table_*` extern, so the rule matches the prefix
(minus the lock/probe forms) — `push` / `reserve` / `resize` / `erase` / `insert` / `delete` are all
covered without naming one generic, and a **newly added builtin is caught by default**. On top of
that: `ExprNew`, `ExprAscend` (`new Foo(f = v)` lowers to move-to-heap, *not* `ExprNew`),
`ExprDelete`, string interpolation, lambda capture frames, table index (`t[k]` inserts on read),
and any builtin extern returning a freshly allocated string.

### `@scratch` — declaring a reused buffer

A buffer that is sized to the current step's geometry and reused is not an accident. Say so once,
at the buffer, rather than with a `nolint` per call site (which buries the next real finding and
does not survive the code moving):

```das
struct Session {
    @scratch attq : array<float>     // reused per step; sizing it is the owner's strategy
    logits : array<float>            // unmarked: sizing this on a hot path is reported
}

// a helper that sizes a caller's buffer marks the PARAMETER — the call site cannot see
// which field arrived, because the destination comes in by reference
def scratch_resize(@scratch var a : array<numT>; need : int64) { ... }
```

A sizing call whose destination reaches a `@scratch` declaration is not descended into — a
struct field, a by-ref helper parameter, or a **module global** (`var @scratch g : array<T>`;
the annotation goes AFTER `var` — before it is a syntax error). Field/variable annotations are
free-form (`@name`, no registration), so `@scratch` costs nothing to parse.

**Future — `@scratch` as an optimization hint, not just a lint marker.** Today the declaration only
tells the linter "this buffer is reused". The same statement is exactly the precondition a
*compiler* would need to do better than a general `resize`:

- **Keep capacity across a shrink.** A general `resize` down-then-up must assume the array may be
  handed off or freed; `@scratch` says it will not, so the down-size can retain the block and the
  up-size becomes a no-op instead of a realloc.
- **Skip the zero-fill.** `resize` init-fills new elements; a scratch buffer is fully rewritten by
  the step that sizes it, so `@scratch` licenses `resize_no_init` implicitly. dasLLAMA already
  hand-rolls exactly this in `scratch_resize` (delete-on-grow, exact `reserve`, then
  `resize_no_init`) — the annotation could make that the default rather than a hand-written idiom.
- **Hoist the sizing.** With the reuse promise, a `resize(n)` whose `n` is loop-invariant can be
  lifted out of the step loop entirely.

None of that is implemented. The annotation is deliberately named for the property (`scratch`)
rather than for the lint, so it can carry the optimization meaning later without a rename.

### Escape hatches, in order of preference

1. `[cold_path]` on the callee — the honest fix when the leg genuinely runs once (lazy init,
   PSO compile, opt-in bookkeeping, a reference-check path behind a debug flag).
2. `@scratch` on the destination when it is a reused buffer — a struct field, a by-ref helper
   parameter, or a module global. It covers the sizing set (`resize`/`reserve`/`push*`/`emplace`/
   `insert`/`erase`/`pop`/`clear`), **table indexing** (`t[k]` on a `@scratch` table is the
   pool / residency-cache shape — insert on first touch, steady-state hit), and it follows local
   reference bindings (`var lst & = pool.free_bufs[b]` carries the mark).
3. `// nolint:PERF026` on the line, **with a reason**. Honored **anywhere along the chain** —
   the anchor line, the sink line, or any intermediate call site. The sink often bottoms out in
   `daslib/builtin.das` (whose lines are nobody's to annotate), so the honest suppression line is
   usually the call site in the module that owns the decision, and that works.
4. `DAS_LINT_DISABLE=PERF028` for a whole run — no source edit, which is the point when you are
   adding a log line to chase a bug. With all three codes disabled the closure walk is skipped
   entirely, so this buys compile time and not just quiet.

### What the scan deliberately does not report

- **Anything under a `panic(...)` argument.** A panic is fatal in daslang, not an exception, so its
  interpolated message is on the abort path by construction.
- **Macro-generated subtrees.** A rewritten stub (`maybe_parallel_for`) `force_at`-stamps the
  caller's line onto its splice, so its job-dispatch machinery would otherwise be blamed on every
  kernel that dispatches. Macros mark only the *root* of their output generated, so the flag has to
  propagate down — the scan tracks a depth, not a per-node flag test.
- **Indirect calls.** `ExprInvoke` through a function pointer or lambda cannot be resolved
  statically; annotate the implementations it reaches instead. `ExprAddr` is deliberately not an
  edge — taking a function's address is not calling it.

### Cost

Measured on dasLLAMA (`-compile-only examples/dasLLAMA/run.das`, 5 samples), 18 annotated roots
over a ~66k-line engine: **5.55s unannotated, 5.60s annotated** — noise. That is the whole point of
registering the markers C++-side.

An earlier arrangement had the code under contract `require daslib/perf_lint` so the `[lint_macro]`
would fire on every build. It cost +0.58s (+10%), and splitting the declarations into a das-side
leaf module recovered only 0.16s of that — `daslib/ast`, which any das-side annotation module must
pull to subclass `AstFunctionAnnotation`, is itself ~0.4s. Hence compiler-side registration.

The trade: a plain build no longer fails on a contract violation. Lint does, everywhere lint runs.

## Visitor gotchas

- **`in_closure > 0` is NOT a useful guard in `preVisitExprOp2`** — `loop_depth` already doesn't increment inside closure bodies (`preVisitExprFor` / `While` gate on `in_closure == 0`), so PERF001's `loop_depth > 0` correctly excludes closure-internal loops without a separate skip. An `in_closure` early-return at the top of `preVisitExprOp2` hides syntactic patterns (PERF007/008/010/013/014/017) inside the natural `build_string() $(var w) { ... }` idiom and is a bug, not a feature.
- **Macro-generated functions need `current_function.flags.generated`-suppression** — `[CommandLineArgs]`-style codegen synthesizes AST that the user never wrote. Both `perf_warning` and `style_warning` should early-return when `current_function.flags.generated` is true. Otherwise warnings surface at the source-struct's line with no clear way to fix them.
- **Self-implementation suppression** — when a rule's suggested replacement is itself implemented in terms of the pattern (e.g. `empty(arr)`'s body is literally `length(arr) == 0`), gate the rule with `current_function.name == "<callee>" || (fromGeneric != null && fromGeneric.name == "<callee>")`. The generic-instantiation arm catches `empty<int>`, `empty<MyType>`, etc.
