# Macro & AST Programming

Read this skill file before writing compile-time macros, AST manipulation
code, structure macros, qmacro/quote code generation, or any code that
manipulates AST node pointers (e.g., `TypeDeclPtr`, `ExpressionPtr`,
`FunctionPtr`, `StructurePtr`).

## AST nodes are gc_node — unique, no `inscope`, no `<-` for assignment

Every AST type — `TypeDecl`, `Expression` (every subclass: `ExprBlock`,
`ExprCall`, `ExprFor`, …), `Function`, `Structure`, `Enumeration`,
`Variable`, `MakeFieldDecl`, `MakeStruct`, every `Annotation` subclass —
is a **plain raw pointer** (`gc_node`), not a smart_ptr. Their lifetime
is owned by a `gc_root` (typically the Module), and the GC sweeps
unreachable nodes.

**Rules:**

- **No `var inscope`** for AST pointer types. Plain `var x = new ExprConstInt(...)` or `var x = clone_expression(e)`.
- **No `<-` for assignment** to AST node fields. Use plain `=`:
  - `fn.body = newBlock`
  - `td.firstType = elemType`
  - `field |> move_new <| expr` is also legacy — write `field = expr`.
- **No `move_new`, no `move`, no `add_ptr_ref` on AST pointers** — those were smart_ptr-era helpers. `add_ptr_ref` still exists for bridging to the residual `smart_ptr<T>` types (e.g., `smart_ptr<Program>`), not for AST.
- **Don't call `get_ptr(x)` on AST pointers** — `x` is already a raw pointer. `expr is ExprVar` works directly, `x.field` works directly, `x == null` works directly.
- **AST is unique — clone, don't copy.** Each AST node lives at exactly one location. Inserting the same `ExpressionPtr` into two parent expressions creates aliasing — both think they own the child, gc_collect walks it twice, mutations leak across, and the AST validator complains. To duplicate, use the matching `clone_*`:

  | Type | Clone fn | Notes |
  |---|---|---|
  | `TypeDeclPtr` | `clone_type(t)` | |
  | `ExpressionPtr` | `clone_expression(e)` | recursive deep clone |
  | `FunctionPtr` | `clone_function(f)` | returned via move: `var x <- clone_function(f)` |
  | `VariablePtr` | `clone_variable(v)` | |
  | `StructurePtr` | `clone_structure(s)` | (no `get_ptr(st)` wrapping needed) |

- **Don't substitute `clone_to_move`** for the type-specific `clone_*` — `clone_to_move` is the generic copy-then-move helper for non-copyable values like `array<T>`. AST pointers want the type-specific clone.
- **Tools/utilities that build AST at runtime** (outside the normal compile pipeline) must wrap the scope in `ast_gc_guard() { ... }` from `daslib/ast`, otherwise the leak detector reports `GC APP LEAK` at exit.

If you find yourself reading older guidance about `var inscope`, `<-`,
`move_new`, `add_ptr_ref` for AST types, the source is pre-migration.
The post-migration rules above are correct as of daslang 0.6.x.

### Pre-set `_type` on emitted `ExprVar` (and similar nodes) that flow into typed positions

`Expression::clone` deep-copies `_type` faithfully ([ast.cpp:1094](src/ast/ast.cpp#L1094): `expr->type = type ? new TypeDecl(*type) : nullptr`). So whatever you put on the source propagates to every consumer. The trap is the **source**: `new ExprVar(at = at, name := wbName)` leaves `_type` null, every `clone_expression` of it inherits the null, and if any of those clones flows into a generic call (`push_clone`, `sum`, etc.) the typer fails with `30165: cannot infer ... return type with 'auto'`.

Don't rely on the typer's later local-variable-resolution pass to fix this — its generic-instantiation pass runs **first** and commits to `auto`, cascading errors up through every downstream consumer.

Fix at emission time:

```das
var pvar = new ExprVar(at = at, name := boundName)
pvar._type = clone_type(boundElementType)
pvar._type.flags.ref = true
// now any clone_expression(pvar) downstream carries the type into push_clone et al.
```

Same family of trap as the [ExprRef2Value blocker](#peel-exprref2value-before-qmatch) — the typer doesn't repair what macro substitution introduces, when the substitution lands in an already-typed AST fragment.

Canonical example: `try_make_inline_cmp` and the `_where`-arm projection-bind rewrite in `daslib/linq_fold.das` (PR #2714).

## The few residual smart_ptr types — `Program`, `Context`, `FileAccess`

A small set of types are still `smart_ptr<T>` (refcounted with manual
addRef/releaseRef on the C++ side). These DO follow the older
`var inscope` / `<-` patterns:

- `ProgramPtr` = `smart_ptr<Program>`
- `ContextPtr` = `smart_ptr<Context>`
- `FileAccessPtr` = `smart_ptr<FileAccess>`
- `DebugAgentPtr`, `VisitorAdapterPtr` (from `make_visitor`) — internal

For these:

- **`<-` operator**: ALWAYS `memcpy(dest, src) + memset(src, 0)` — it is a raw memory operation, NOT smart_ptr-aware. It zeros the source regardless of type.
- **`move` function**: Bound via C++ `builtin_smart_ptr_move*` family in `module_builtin_runtime.cpp` — proper smart pointer move with reference counting. Use `move(dest, src)` for `smart_ptr<T>` transfers when the refcount needs to be tracked.
- **`return <- expr`**: Moves value to return slot and zeroes `expr`. If `expr` is a `&` ref parameter, this zeroes the *caller's* variable since they share memory.
- **Visitor adapters** (`make_visitor` returns `VisitorAdapterPtr`) need `var inscope adapter <- make_visitor(*v)` and an `unsafe` block at the call site — see `daslib/ast.das` for examples.

## Macro modules each compile into their own context — cross-module registration is intra-context only

Each macro-bearing module is `simulate`d into its **own context** at compile time; its macros run via `invoke_in_context`. Compile-time globals a macro mutates (a pattern/registry table, an adapter list, any `[_macro]`-populated state) live in **that module's** context. A macro in module B **cannot** push state into module A's compile-time registry by calling A's functions — the two run in **separate contexts**, so it would require marshalling the value across the context boundary through the `invoke_in_context` transport layer (copying data between heaps, no shared pointers). Registering a closure/adapter/AST builder that way is impractical: **there is no "self-registration" of macro state across modules.**

The practical pattern: to contribute macro-time state into module A's registry, the contributing code must **compile into A's context** — i.e. **A `require`s the contributor**, pulling its definitions in. Direction is *consumer → contributor*, never contributor-self-registers-into-consumer. (Example: the linq_fold engine's `splice_patterns` table lives in linq_fold's macro context, so a source adapter for an external module is pulled in via `linq_fold` requiring the adapter file — not the module registering itself.)

This forces a hard `require` even when the dependency is logically optional / inverted (core requiring an external module). Make it conditional with two pieces (both available):
- an **optional require** — `require ?<guard> <target>` (gen2) requires `<target>` only when module `<guard>` is available, and skips silently otherwise (a plain top-level `require` resolves at module-resolution time, *before* `static_if`, so `static_if` alone can't gate it — this is what the `?guard` form solves). See `doc/source/reference/language/modules.rst` "Optional requires".
- a **`static_if (typeinfo builtin_module_exists(<guard>))`** guard around the registration call and any dispatcher branch that names the contributor's symbols (the `static_if` drops its untaken branch before name resolution, so the symbols resolve only when the module is present).

Note adapters can still *emit* code referencing the contributor's symbols by name (resolved at the user's splice site, like linq_fold_decs emitting `for_each_archetype` without requiring decs) — that's orthogonal to *registering* into the consumer's macro state, which is the part bound by the context model.

## Structure macros — generating types and functions

- **`[structure_macro(name=foo)]`** — annotation on a class inheriting `AstStructureAnnotation`; the `apply` method runs at compile time when a struct has `[foo]`. Use to generate companion types, operators, and functions.
- **`clone_structure(st)`** — deep-copies a `StructurePtr` for creating modified companion types (e.g., SOA layout where every field becomes `array<FieldType>`). Pass the pointer directly — no `get_ptr` wrapping.
- **`compiling_module() |> add_function(fn)`** — registers a concrete function in the current module
- **`compiling_module() |> add_generic(fn)`** — registers a generic function (instanced per call site)
- **`compiling_module() |> add_structure(st)`** — registers a generated struct
- **`compiling_module() |> add_alias(tdef)`** — registers a type alias
- **`fn.flags |= FunctionFlags.generated`** — marks a function as compiler-generated (suppresses "unused" warnings, enables special error messages)
- **`add_structure_field(st, name, typeDeclPtr, defaultExprPtr)`** — adds a field. Pass `clone_type(qmacro_type(type<int>))` for the type and `default<ExpressionPtr>` for "no default value":
  ```das
  st |> add_structure_field("count", clone_type(qmacro_type(type<int>)), default<ExpressionPtr>)
  ```
- **`ExprFieldFieldFlags.no_promotion`** / **`ExprAtFlags.no_promotion`** — prevent the compiler from promoting field access or index access to a different type; needed in generated AST to preserve exact types
- **`[tag_function(tag_name)]`** on a function + **`[tag_function_macro(tag="tag_name")]`** on a class — intercepts calls to the tagged function and rewrites them in the `transform` method. Used for compile-time call rewriting (e.g., SOA `operator .` rewrites `soa[i].field` → `soa.field[i]`).
- **`[for_loop_macro(name=foo)]`** on a class inheriting `AstForLoopMacro` — intercepts `for` loops whose source is a matching type. Override `visitExprFor` to rewrite the loop AST (e.g., SOA for-loop expands `for (it in soa)` into per-field array iteration).

## AST type introspection

- **`typeDecl.isConst`** — `true` if a `TypeDecl` has the `const` modifier. Useful for checking whether function parameters (e.g., `self` in interface methods) are declared const.
- **`typeDecl.argTypes`** — array of `TypeDeclPtr` representing function-type arguments (indices: 0 = first parameter). For interface method fields, `argTypes[0]` is the `self` parameter — check `.isConst` to determine if the method is const.
- **Interior mutability pattern** — when a const getter needs to lazily mutate a cache: declare param as `self : T ==const`, then `var pS = unsafe(reinterpret<T? -const>(addr(self)))` to strip const for cache mutation. Used in `daslib/interfaces.das` for const-only interface proxy caching.

## Shared AST-match helpers

`daslib/ast_match.das` exposes a small set of public helpers harvested from `linq_fold` + `sqlite_linq` during the 2026-05 refactor. Reach for these BEFORE writing a new `is X / as X` cascade — they capture the exact semantics each pattern was hand-rolling, with module-gating and generic-instantiation transparency baked in.

| Helper | Signature | Purpose |
|---|---|---|
| `match_call_in_module` | `(expr, name, modName) → ExprCall?` | Match an `ExprCall` to `(name, modName)`, transparent to generic instantiation (consults `func.fromGeneric` as fallback). |
| `match_call_in_linq` | `(expr, name) → ExprCall?` | Thin wrapper on `match_call_in_module` with `modName="linq"`. |
| `peel_lambda_single_return` | `(lam) → Expression?` | For `@(x : T) => expr`, return `expr`. `null` if shape doesn't match. |
| `peel_lambda_rename_var` | `(expr, argName) → Expression?` | Peel + rename the bound variable. Falls back to `invoke(expr, argName)` when non-peelable so callers can splice the result unconditionally. |
| `peel_lambda_replace_var` | `(expr, replacement) → Expression?` | Variant of `peel_lambda_rename_var` that substitutes the bound variable with an arbitrary expression. Uses the unified peel-aware `replaceVariable` (single rule strips typer-inserted `ExprRef2Value` on already-typed AST). |
| `peel_lambda_rename_2vars` | `(expr, a, b) → Expression?` | 2-arg form for `aggregate`-style `block<(acc, x) : AGG>` lambdas. Returns `null` on shape mismatch — caller decides fallback. |
| `peel_tuple_field_read` | `(expr, bindName, fieldIndex) → bool` | `true` when `expr` matches `<bindName>._<fieldIndex>` — tuple-slot read on a named bind. Single-level `ExprRef2Value` peel on each side. |
| `extract_const_string` | `(e) → tuple<bool; string>` | For `ExprConstString` returns `(true, value)`, else `(false, "")`. Use to consume compile-time string literals threaded through macro args. |
| `qn` | `(prefix, at) → string` | Synthesizes ``` `<prefix>`<at.line>`<at.column> ``` — qualified-name helper for macro-emitted locals. Deterministic per `(prefix, at)`; synthesized `LineInfo()` (line=0, col=0) WILL collide across distinct synth sites with the same prefix — build a synth-specific name if it matters. |
| `qm_peel_ref2value` | `(var e : Expression?&) → void` | Single source of truth for `ExprRef2Value` peeling. Always call this instead of hand-rolling `while (... is ExprRef2Value)` or `if`-peel — see ["Peel ExprRef2Value before qmatch"](#peel-exprref2value-before-qmatch). |
| `push_block_list` | `(var stmts, var blockExpr)` in `daslib/templates_boost.das` | Splices every statement from a `qmacro_block(...)` result into `stmts`, cloning each. See ["Push cluster consolidation"](#push-cluster-consolidation). |

**When the patterns apply (and when they don't).** These helpers earn their keep in files that **probe AST shape** to route macro emission — `linq_fold`, `sqlite_linq`, `ast_match` itself. Files that only **emit code** without introspecting it — `decs_boost`, the emitter half of `templates_boost` — won't find adoption sites. Audit before mechanically searching: if a file has zero hand-rolled `is X / as X` call-cascades and zero qname construction, the patterns don't apply there.

## `qmatch` — predicate-style pattern matching

Prefer `qmatch(expr, <pattern>).matched` over hand-rolled `is X / as X` cascades when matching structural AST shapes. `qmatch` is RTTI-strict and won't traverse `ExprRef2Value` — peel first via `qm_peel_ref2value`.

```das
// HAND-ROLLED (avoid)
if (node is ExprOp2) {
    let op = node as ExprOp2
    if (op.op == "+" && op.left is ExprVar && op.right is ExprVar) {
        // ...
    }
}

// qmatch (prefer) — bind variables are declared BEFORE the call
// and filled when the match succeeds
var lhs, rhs : ExpressionPtr
let r = qmatch(node, $e(lhs) + $e(rhs))
if (r.matched) {
    // lhs, rhs are now bound
}
```

Pattern tags inside `qmatch(expr, <pattern>)`:

- `$e(name)` — bind an arbitrary sub-expression to outer `var name : ExpressionPtr`
- `$f(name)` — bind a field name to outer `var name : string`
- `$v(name)` — bind a constant value to a typed outer var
- `$i(name)` — bind an identifier name to outer `var name : string`
- `_` — anonymous wildcard (no bind)
- Concrete operators (`&&`, `||`, `+`, `==`, `<`, dot-field, function-call) and literals match literally

Result is `QMatchResult` with `.matched : bool` and `.error : QMatchError` — captured bindings live in the pre-declared outer variables, NOT on the result struct. On match failure the bindings are left untouched.

Canonical examples in `modules/dasSQLITE/daslib/sqlite_linq.das` — search for `qmatch(` for 37+ adoption sites. Tests in `tests/ast_match/test_qmatch_*.das` + `test_capture_*.das` exercise every tag and grammar form. Full pattern grammar lives in `daslib/ast_match.das`.

**Not every probe fits qmatch.** Shapes with cross-statement constraints (e.g., "3 statements with specific types where push target equals res var and recordNames count matches sources count") exceed qmatch's grammar — fall back to hand-rolled `is X / as X` for those. Self-circular file dependencies are also out: `ast_match.das` itself can't use `qmatch` to define its own grammar.

## `qmacro` vs `quote` (code generation)

- **`qmacro(expr)`** — quasi-quote with reification splices (`$v()`, `$e()`, `$c()`, `$t()`, `$i()`, `$f()`, `$a()`, `$b()` etc.). Use when the generated code contains interpolated values.
- **`qmacro_function("name") $(args) { body }`** — generates an entire `FunctionPtr` with spliced arguments/body. The `$t(typeExpr)` splice in the signature sets parameter/return types from `TypeDeclPtr` variables.
- **`qmacro_expr(${ statement; })`** — generates a statement-level expression (e.g., assignment). The `${ }` block allows semicolons.
- **`quote(expr)`** — plain quote with NO reification. Use when the expression is a simple literal or constant with no splices — e.g. `quote(true)`, `quote(false)`, `quote(0)`.
- **Rule**: if the expression contains no `$…()` reification operators, prefer `quote()` over `qmacro()`.

### Reification operators (inside `qmacro`)

- **`$v(daslangVar)`** — splice the runtime **value** of a variable into the generated code
- **`$e(exprPtr)`** — splice an `ExpressionPtr` as a sub-expression
- **`$c(stringVar)`** — splice a `string` as a **call name** (function name). Example: `$c(callName)(arr, val)` generates a call to whatever function name `callName` holds
- **`$t(typeDeclPtr)`** — splice a `TypeDeclPtr` as a type annotation in signatures or declarations
- **`$i(stringVar)`** — splice a string as an **identifier** (variable name)
- **`$f(stringVar)`** — splice a string as a **field name**. Example: `st.$f(fieldName)` becomes `st.x` when `fieldName="x"`
- **`$a(arrayOfExprPtr)`** — splice an `array<ExpressionPtr>` as function call **arguments**
- **`$b(arrayOfExprPtr)`** — splice an `array<ExpressionPtr>` as a **block body** (sequence of statements). Build the array with `push <| qmacro_expr(...)`, then `$b(bodyExprs)` inlines all statements into the function body

### Splice inputs are cloned for you — don't pre-clone with `clone_expression`

`qmacro` / `qmacro_block` / `qmacro_expr` / `qmacro_block_to_array` all go through [`apply_template`](../daslib/templates_boost.das#L249), which calls `clone_expression` on every `$e(...)` substitution input. So this is wasted work:

```das
// WRONG — clones twice (once explicitly, once inside apply_template)
var defaultExpr = clone_expression(terminatorCall.arguments[1])
preludeStmts |> push <| qmacro_expr() {
    let $i(defaultName) = $e(defaultExpr)
}
```

```das
// RIGHT — inline the source; apply_template clones during substitution
preludeStmts |> push <| qmacro_expr() {
    let $i(defaultName) = $e(terminatorCall.arguments[1])
}
```

**Multi-splice cases.** Even when the same source feeds N `$e(...)` slots in one qmacro, you don't need to pre-clone. `apply_template` clones each substitution independently:

```das
// WRONG — three pre-clones for three splice slots
var takeA = clone_expression(takeExpr)
var takeB = clone_expression(takeExpr)
var takeC = clone_expression(takeExpr)
body = qmacro_block() {
    let $i(takeNName) = $e(takeA) <= 0 ? 0 : ($e(takeB) < $i(lenName) ? $e(takeC) : $i(lenName))
}
```

```das
// RIGHT — inline takeExpr at each slot; apply_template gives you 3 independent clones
body = qmacro_block() {
    let $i(takeNName) = $e(takeExpr) <= 0 ? 0 : ($e(takeExpr) < $i(lenName) ? $e(takeExpr) : $i(lenName))
}
```

If the source expression has side effects (rare in AST-building code — most sources are bare variable reads), bind once via plain `let baseE = E` (no clone) and splice the local:

```das
let baseE = make_side_effecty_expr()  // 1 eval, no clone
body = qmacro_block() { let _x = $e(baseE) + $e(baseE) }
```

The lint rule `PERF023` (see `skills/perf_lint.md`) catches the wasted-pre-clone shape automatically.

### `[clone(...)]`-annotated functions clone for you too

The same wasted-pre-clone shape exists at **direct calls** to functions that promise to clone internally — e.g. `peel_lambda_rename_var`, `apply_qmacro_template_function`, `push_inline_id`. Each carries a `[clone(paramName, ...)]` annotation declaring which params it clones:

```das
// WRONG — peel_lambda_rename_var has [clone(expr)], clones internally
var pred = peel_lambda_rename_var(clone_expression(terminatorCall.arguments[1]), valueName)

// RIGHT
var pred = peel_lambda_rename_var(terminatorCall.arguments[1], valueName)
```

Var-init-then-pass form is also flagged when every use is into an annotated arg position:

```das
// WRONG — topClone's only use is annotated arg of finalize_emission_stmts
var topClone = clone_expression(adapter.arrayTop)
return finalize_emission_stmts(topClone, ...)

// RIGHT
return finalize_emission_stmts(adapter.arrayTop, ...)
```

PERF024 catches both shapes. Canonical annotated set (grows over time): `peel_lambda_rename_var`/`_replace_var`/`_rename_2vars` + `qm_extract_stmts` in `ast_match`, `push_block_list` + `apply_qmacro_template_function` in `templates_boost`, the `emit_*`/`finalize_emission_stmts` family in `linq_fold`, `push_bind`/`push_inline_id`/`push_inline_lit` in `sqlite_linq`.

**To mark your own function** — add `[clone(p1, p2)]` (one annotation per function, comma-separated param names). The annotation is registered C++-side, no `require` needed.

**Before annotating, verify the function CONSUMES `p` cleanly — does not MUTATE `p` and does not retain shared aliases.** The correct contract is: every code path either ignores `p`, clones (or deep-iterates-and-clones) the pieces of `p` it needs into its output, OR forwards `p` to another `[clone(p)]`-annotated function — and never mutates `p` or stores raw aliases that outlive the call. `clone_expression(p)` directly is the common case; cloning sub-pieces is also fine (e.g. `push_block_list` clones each element of `blockExpr.list`, `qm_extract_stmts` clones each element of `blk_expr.list`). What MUST NOT happen: `apply_template(rules, at, p)` — that mutates `p` in place via `TemplateVisitor`. Same for every `apply_qmacro_*` / `apply_qblock_*` variant; pre-clones at their callsites are **load-bearing**, not redundant. Marking them `[clone(...)]` would make PERF024 flag callers who are doing the right thing.

### Default-initializing generated struct variables

In macro-generated code, `var x : $t(st)` fails with "uninitialized variable" for structs without field defaults. Use `default<T>` instead:

```das
// WRONG — fails if struct has uninitialized fields
blk |> push <| qmacro_block() { var entity : $t(st) }

// CORRECT — default-initializes all fields
blk |> push <| qmacro_block() { var entity := default<$t(st)> }
```

### Push cluster consolidation

When building an `array<Expression?>` for a `$b(...)` splice via two or more consecutive `arr |> push <| qmacro_expr() { ... }` calls into the same array, collapse the runs into a single emission. Two equivalent forms:

**Form A** — `push_from` + `qmacro_block_to_array` (preferred — pure stdlib composition, no per-element clone):

```das
// BEFORE
stmts |> push <| qmacro_expr() { var $i(accName) = 0 }
stmts |> push <| qmacro_expr() { for ($i(itName) in $i(srcName)) { $e(loopBody) } }
stmts |> push <| qmacro_expr() { return $i(accName) }

// AFTER
stmts |> push_from <| qmacro_block_to_array() {
    var $i(accName) = 0
    for ($i(itName) in $i(srcName)) { $e(loopBody) }
    return $i(accName)
}
```

**Form B** — `push_block_list` + `qmacro_block` (clones each element, right choice when the source block stays alive after the push):

```das
stmts |> push_block_list(qmacro_block() {
    var $i(accName) = 0
    for ($i(itName) in $i(srcName)) { $e(loopBody) }
    return $i(accName)
})
```

**When NOT to collapse.** Runs of pushes interleaved with `if`/`elif` branches (each branch contributing one push) cannot collapse into a single block — the conditionality lives outside the qmacro. Same for pushes interleaved with non-push statements. Only PURE consecutive runs into the same array, at the same indent, with no intermediate statements collapse cleanly.

## Pattern-matching call shapes — never rely on leading `!`

When a macro walks an AST and pattern-matches specific call shapes to
route them differently (e.g. `_sql` detects nested
`select_from(...)._any(...)` and emits SQL `EXISTS`, vs. `_none(...)`
emitting `NOT EXISTS`), **do not rely on detecting `!expr` to flip the
emitted output**. Standing rule: AST walkers do not have leading-`!`
support. The `!` often sits across intermediate AST nodes —
parentheses, `if` expressions, local `let` bindings, constant-folded
wrappers — that break a naive pattern match. Pattern-matching `!any(...)`
is correct for some cases, silently wrong for any user that
parenthesizes or refactors the condition.

**Ship explicit positive/negative name pairs instead.** Convention:
the negative form gets a `_not_` or `_no` prefix on the positive name.

| Positive | Negative | Emits |
|---|---|---|
| `_any` | `_none` | `EXISTS` / `NOT EXISTS` |
| `_in` | `_not_in` | `IN` / `NOT IN` |
| `_is_null` | `_is_not_null` | `IS NULL` / `IS NOT NULL` |
| `_between` | `_not_between` | `BETWEEN` / `NOT BETWEEN` |

Same rule applies outside `_sql` — any macro that needs to discriminate
"do X" vs "do NOT X" call shapes ships matched name pairs.

**Does NOT apply to runtime `!`.** Plain daslang code with `!any(...)`
or `!x._in(...)` compiles and evaluates fine when there's no
AST-walking macro routing through that expression. The constraint is
specifically for macro-expansion-time pattern matching.

**Docs:** when documenting a macro with negated forms, list the
positive/negative pair together — do not say "negate with `!`".
Discoverability is part of the UX.

## `[call_macro]` entry-guard contract

Every `[call_macro]` `class : AstCallMacro` declares an implicit contract
about how its `visit()` sees its arguments. Get this wrong and you'll
either expand against unresolved AST (and produce gibberish) or miss
inner macro expansions entirely.

### `canVisitArgument=true` is the default — inner-first expansion

Daslang's macro pass walks each call_macro argument before firing the
outer macro's `visit`. The walk runs the typer **and** any inner
call_macros recursively. By the time `visit(call)` runs:

- inner `_where(...)`, `_select(...)`, etc. — and any user-defined
  wrapper that expands to one of them — have **already cascaded** into
  their canonical `where_(...,$(_:T)=>...)` / `select(...)` shapes;
- type-checked sub-expressions carry resolved `_type`;
- field reads on those resolved types may be wrapped in
  `ExprRef2Value` (the value-from-ref adapter the typer inserts);
- raw arguments that genuinely can't type yet (e.g. `_.Field` outside
  a lambda, where `_` is unbound) stay as raw AST with `null _type` —
  the macro pass tolerates these because the outer macro is going to
  rewrite them anyway.

You almost never want to override this default. The Mode-1/Mode-2 story
documented in `modules/dasSQLITE/API_REWORK.md` chunk-2 section is the
cautionary tale: `_sql` shipped briefly with `canVisitArgument → false`
overrides because a synthetic test seemed to show inner expansion not
firing. The real cause was a missing `require daslib/linq_boost public`
in `sqlite_linq.das` — without that, `_where` / `_select` weren't
registered as call_macros at user-file scope, so they had nothing to
expand into. Removing the override + adding the require made everything
work. **If inner macros aren't expanding, look for a missing require
before changing visit semantics.**

### Entry guards: `macro_verify`, not `return null`

When `visit()` must assume a property of its arguments (typed input,
non-auto chain, expected arity), check it loudly with `macro_verify`,
including a `{describe(arg)}` in the message:

```
[call_macro(name="_sql")]
class private SqlMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 1, prog, call.at,
            "_sql expects one argument: a chain expression")
        var argT = call.arguments[0]._type
        macro_verify(argT != null, prog, call.at,
            "_sql: chain argument has null _type — inner call_macros did not expand " +
            "before this fired. Arg: {describe(call.arguments[0])}")
        macro_verify(!argT.isAutoOrAlias, prog, call.at,
            "_sql: chain type is auto/alias after _type is set — compiler bug")
        // ... real work
    }
}
```

A null `_type` on the chain argument doesn't mean "Mode 2 is broken" —
it means an inner call_macro that should have expanded didn't, almost
always because the user didn't `require` the module that defines it.
Loud failure tells the user that immediately; silent `return null`
re-queues the macro and lets the daslang pipeline emit a confusing
infer-time cascade instead.

### Peel `ExprRef2Value` before `qmatch`

Post-Mode-2-expansion AST walking will see field reads wrapped in `ExprRef2Value`. `qmatch` is RTTI-strict — it matches `ExprField` but not `ExprRef2Value(ExprField(...))`. **Route through `qm_peel_ref2value`** (the single source of truth in `daslib/ast_match.das`) instead of hand-rolling either a `while`-peel or an `if`-peel:

```das
require daslib/ast_match

qm_peel_ref2value(node)
if (node == null) {
    macro_error(prog, at, "_where: ExprRef2Value with null subexpr")
    return ""
}
// now `qmatch(node, _.$f(name))` etc. work as expected
```

`qm_peel_ref2value` currently uses `while (e is ExprRef2Value)` rather than single-`if`-peeling. The conservative loop is intentional until block-folding is fully audited — `tests/ast_match/test_ref2value_skip.das` exercises a triple-wrap shape, and `src/ast/ast_block_folding.cpp` synthesis paths could theoretically produce a nested wrapper. Once that audit lands, the helper switches to single-`if` peel in one place and every consumer follows automatically.

Auto-peel **inside** `qmatch` itself remains a TODO documented in `daslib/ast_match.das`. Until then, every analyzer entry point that takes an expression coming out of post-expansion (predicate body, projection body, classifier helpers like `is_const_or_captured_var`) needs the `qm_peel_ref2value` call at the top.

### When you really do need raw arguments

If a macro genuinely needs un-expanded raw AST (rare — mostly for
qmacro-only sugar that builds new code without inspecting it), override
`canVisitArgument` to return `false` for those argument indices. Pair
with a unit test that exercises the inner-macro composition case
specifically — that's the path that breaks first when the override is
wrong.
