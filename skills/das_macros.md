# Macro & AST Programming

Read this skill file before writing compile-time macros, AST manipulation
code, structure macros, qmacro/quote code generation, or any code that
manipulates AST node pointers (e.g., `TypeDeclPtr`, `ExpressionPtr`,
`FunctionPtr`, `StructurePtr`).

## Debug with the AST verifier — FIRST, not last

A macro that builds slightly-wrong AST does not fail where the mistake is. It
segfaults several passes later inside inference or codegen, with no line number
and no clue which macro did it. `daslib/ast_verify` turns that into a diagnostic
at the offending node, so reach for it the moment a macro misbehaves:

```
daslang --ast-verify foo.das
```

```
CRASH: SIGSEGV (Segmentation fault) (signal 11) at address 0x30      # without
AST verify: let variable 'i' has no type (Variable._type is null)    # with
```

It checks shapes the compiler dereferences unguarded: null required children,
null elements in child vectors, statements in value slots, slots reached through
an unchecked cast, malformed `TypeDecl`s (element-less containers, payload-less
named types), declaration types (function result, arguments, struct fields), and
flag pairings the parser always establishes.

Three ways in, by how much of the tree you can see:

| Call | Use when |
|---|---|
| `daslang --ast-verify file.das` | first move on any unexplained crash; force-includes the module and checks every non-builtin module before each inference pass |
| `require daslib/ast_verify` | same, pinned in the source |
| `verify_module(prog, mod)` | at the end of your `apply()`, once the tree is installed |
| `verify_expression(expr)` | you BUILD and RETURN a subtree — for-loop / call / variant / reader macros run inside inference, where the module-level form cannot see your result yet |
| `verify_function(fn)` | you BUILD a function. `add_function` mangles the signature immediately, so a malformed result or argument type crashes there, before any pass could run — verify before you register |

Each returns the violation count, and each REPAIRS what it reports, substituting
a placeholder so the walk finishes and surfaces every diagnostic rather than
dying on the first. That repair is not a nicety: the verifier walks with the same
visitor the compiler uses, and `Expr*::visit` dereferences children unguarded, so
reporting without repairing gives you the diagnostic AND the segfault.

Two things it will not do for you. It is a denylist of shapes that CRASH — a
shape the compiler tolerates, or legitimately produces, is deliberately not
flagged, so silence is not proof your tree is right. And a macro that re-breaks
the same node every pass never converges: the verifier repairs, you break it
again, and the compile ends in `error[30507]: type inference exceeded maximum
allowed number of passes`. That is your bug — break the node once.

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

`Expression::clone` deep-copies `_type` faithfully ([ast.cpp:1138](src/ast/ast.cpp#L1138), repo-only: `expr->type = type ? new TypeDecl(*type) : nullptr`). So whatever you put on the source propagates to every consumer. The trap is the **source**: `new ExprVar(at = at, name := wbName)` leaves `_type` null, every `clone_expression` of it inherits the null, and if any of those clones flows into a generic call (`push_clone`, `sum`, etc.) the typer fails with `30165: cannot infer ... return type with 'auto'`.

Don't rely on the typer's later local-variable-resolution pass to fix this — its generic-instantiation pass runs **first** and commits to `auto`, cascading errors up through every downstream consumer.

Fix at emission time:

```das
var pvar = new ExprVar(at = at, name := boundName)
pvar._type = clone_type(boundElementType)
pvar._type.flags.ref = true
// now any clone_expression(pvar) downstream carries the type into push_clone et al.
```

Same family of trap as the [ExprRef2Value blocker](#peel-exprref2value-before-qmatch) — the typer doesn't repair what macro substitution introduces, when the substitution lands in an already-typed AST fragment.

**A `Variable` you emit MUST have `_type` set** — unlike `ExprVar` (where a null `_type` means "let the typer fill it"), a `new Variable(name := n, at = at, init = e)` destined for an `ExprLet` (or a global, function/block argument, structure field) with null `_type` is a malformed tree: infer reports `error[50640] malformed AST, variable '<n>' is missing its type` (`50608` for structure fields) and fails the compile. For type inference from the initializer, set `_type = new TypeDecl(baseType = Type.autoinfer, at = at)`.

The same `malformed AST, ...` detection family covers the other required-but-nullable fields a macro-built tree can miss: a for loop's `body` (`50607`), `iteratorsAt`/`iteratorsAka` left shorter than `iterators` when rewriting a loop (`50607` — keep all the parallel vectors the same length, like the tutorial's table_kv macro does), null ENTRIES in `variables`/`arguments`/`sources` lists (`50640`/`50609`/`50607`), and a generated `Function` with null `result` (`50609` — set `result` to an auto `TypeDecl`). These repair the tree in place so compilation can continue reporting, but the error is sticky — the compile still fails. Null *expression children* (`ExprOp2.left`, an `if` condition) are NOT guarded — those crash in the visitor walk itself; the crash-capture stack trace is the diagnostic there.

**`macro_sticky_error(prog, at, message)`** — the sticky-error primitive itself, next to `macro_error` in the ast module. Use it when your macro detects damage, repairs it so infer can proceed, but the program must still fail to compile: a plain `macro_error` recorded on a pass that also changed the AST is discarded by the next infer pass, a sticky one survives to the end.

Canonical example: `try_make_inline_cmp` and the `_where`-arm projection-bind rewrite in `daslib/linq_fold_common.das` (PR #2714).

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
- an **optional require** — `require ?<guard> <target>` (gen2) requires `<target>` only when module `<guard>` is available, and skips silently otherwise (a plain top-level `require` resolves at module-resolution time, *before* `static_if`, so `static_if` alone can't gate it — this is what the `?guard` form solves). See [Modules](https://daslang.io/doc/reference/language/modules.html) § "Optional requires".
- a **`static_if (typeinfo builtin_module_exists(<guard>))`** guard around the registration call and any dispatcher branch that names the contributor's symbols (the `static_if` drops its untaken branch before name resolution, so the symbols resolve only when the module is present).

Note adapters can still *emit* code referencing the contributor's symbols by name (resolved at the user's splice site, like linq_fold_decs emitting `for_each_archetype` without requiring decs) — that's orthogonal to *registering* into the consumer's macro state, which is the part bound by the context model.

The trap also bites READS: module B calling A's accessor function reads **B's context copy** of A's globals — silently empty, no error (2026-07-12: llvm_exe calling llvm_tune's scope bank got `{}` while llvm_tune's own annotations saw it filled). Anything another macro module must read goes through the **AST** (annotations/stamps — `tune_scopes_status(prog)` walks `[tune_scope]` declarations instead of the bank), **files**, or **env** — never a shared macro global.

## Pass macros — `[infer_macro]` needs a CLEAN tree; `[dirty_infer_macro]` runs every pass

- `[infer_macro]` (AstPassMacro) fires only AFTER inference succeeds — `inferTypes` bails
  to `failed_to_infer` before its loop, so it never sees a program with errors. Use it to
  instrument well-typed trees (coverage, heartbeat, quote).
- `[dirty_infer_macro]` fires after EVERY infer pass, errors present or not ("assume
  half-way-there tree") — the hook for augmenting a not-yet-inferable program, e.g.
  adding a structure alias mid-infer. Each pass starts by clearing `program->errors`, and
  the loop continues only while a macro returns true (astChanged) or inference
  progresses — a late-binder must return true when it changes the tree, or the loop stops
  and the current errors become final. (C++ naming crossover: `[infer_macro]` registers
  into `Module::macros`, `[dirty_infer_macro]` into `Module::inferMacros`.)
- StructureAnnotation `finish`/`patch` do NOT run for a struct whose inference failed —
  `apply` is the only hook guaranteed to fire, so a deferred "better error" for a failing
  struct cannot live in `finish` (probe-verified 2026-08-07).

## Structure macros — generating types and functions

- **`[structure_macro(name=foo)]`** — annotation on a class inheriting `AstStructureAnnotation`; the `apply` method runs at compile time when a struct has `[foo]`. Use to generate companion types, operators, and functions.
- **`apply` runs at parse time, in written order, sequentially** (repo-only: `parser_impl.cpp` `ast_structureDeclaration`) — a later annotation in the same bracket group sees every mutation the earlier one made (added fields, methods). Multiple structure macros on one struct are fine; the "only one" restriction is for handled-type annotations.
- **`[|> foo]` marks an annotation inherited** (probe-verified 2026-08-08): it is copied to every DERIVED struct/class down the parent chain and PREPENDED before the child's own annotations, so it always applies first. It also fires on the carrying struct itself — discriminate with `st.flags.isTemplate` / other flags when the parent is a `struct template`/`class template`.
- **`typedef NAME = <type>` inside a struct/class body is grammar** — it registers a structure alias (same slot as `add_structure_alias`), readable at apply time via `get_structure_alias(st, name)`; infer resolves alias-typed fields against the owning struct's aliases.
- **Annotation argument values accept bare identifiers** (grammar `ds2_parser.ypp:1179`, probe-verified 2026-08-08): `@dflt = g_one` ≡ `@dflt = "g_one"` — the unquoted NAME stores as `tString`, so `find_arg(...) ?as tString` reads both spellings identically. Also `= @@name` for function references. Works on field annotations and `[bracket]` annotations alike; prefer the unquoted form when the value IS an identifier (a global, a sibling field) rather than free text — it reads as a reference, not data. The formatter never re-quotes (token-based).
- **Annotation argument NAMES reject most keywords** — the grammar whitelists only `type`, `in`, and `default` back to argument-name position (`annotation_argument_name`); any other keyword as an argument name (`@capture = ...`, `@template = ...`) is a syntax error. Extending the whitelist is one `.ypp` line + the same one-liner in `tree-sitter-daslang/grammar.js` `_annotation_argument_name` (keep both grammars in lockstep).
- **Never certify a macro rail green while ANY other compile error is present** — final-verify errors (e.g. an unresolved `cast<auto>` from generated class machinery) are silently masked when an earlier unrelated error stops compilation first; two runs differing only by the unrelated error can flip a "green" macro red (probe-verified 2026-08-08).
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
- **Annotation argument names can't be grammar keywords.** `[myanno(default = "x")]` is `error[30151] syntax error, unexpected default` — the arg-list parser takes a `name`, and keywords (`default`, `type`, `in`, …) don't reduce to one. Pick a synonym (`fallback`, `kind`); verified 2026-07-02 on `[tuned]`.
- **`[for_loop_macro(name=foo)]`** on a class inheriting `AstForLoopMacro` — intercepts `for` loops whose source is a matching type. Override `visitExprFor` to rewrite the loop AST (e.g., SOA for-loop expands `for (it in soa)` into per-field array iteration).

### Deriving facts from method bodies — bodies are PRE-infer here

A structure macro that *reads* the bodies of the struct's methods (deriving a contract instead of making the author declare it — the pattern behind dasLLAMA's GPU access lenses) is walking **un-inferred** AST. Three consequences, each learned the hard way:

- **No types, no resolution.** A field access is a bare `ExprVar` carrying the field's name, and member access is parser-level `ExprField` whose base is untyped. You cannot ask "is this base my struct?" — match on names.
- **Name-only `ExprField` matching collides with swizzles.** `wv.y` on a `float4` local is an `ExprField` named `y`, indistinguishable from a field named `y` if you only look at the name. So match bare names plus explicit `self.<name>` and nothing else; do not recurse into free callees (their locals collide too), and refuse a whole `self` passed as an argument rather than guessing.
- **Key any derived claim by source location, not node pointer.** Infer clones subtrees, so a pointer identifying a node during the macro pass need not identify the same node later. `LineInfo` survives; the pointer does not.

The payoff is worth the care: keep the declarations in place while the derivation lands, and every existing declaration becomes an assertion against the derived answer — which is how stale hand-written annotations surface.

## AST type introspection

- **`typeDecl.isConst`** — `true` if a `TypeDecl` has the `const` modifier. Useful for checking whether function parameters (e.g., `self` in interface methods) are declared const.
- **`typeDecl.argTypes`** — array of `TypeDeclPtr` representing function-type arguments (indices: 0 = first parameter). For interface method fields, `argTypes[0]` is the `self` parameter — check `.isConst` to determine if the method is const.
- **Interior mutability pattern** — when a const getter needs to lazily mutate a cache: declare param as `self : T ==const`, then `var pS = unsafe(reinterpret<T? -const>(addr(self)))` to strip const for cache mutation. Used in `daslib/interfaces.das` for const-only interface proxy caching.

### Fixed arrays in the AST (tFixedArray, since 0.6.3)

`int[3][4]` is a chain of `TypeDecl` nodes, NOT a `dim` vector on the element — the `dim`/`dimExpr` fields are **deleted**:

- One node per dimension: `baseType == Type.tFixedArray`, element in `firstType`, size in `fixedDim`, **outermost first** (`int[3][4]` = FA(3, FA(4, int))). Operate on the head's `fixedDim`/`firstType` and recurse — never assume one node covers all dims (the one-peel rule).
- `fixedDim` sentinels pre-inference: `TypeDecl.dimAuto` (-1) for `[]`, `TypeDecl.dimConst` (-2) while `fixedDimExpr` awaits constant folding. Post-inference both are resolved; `fixedDim <= 0` reaching final verify is an error.
- ref/const/temporary qualifiers live on the **chain head only**. Build chains with `make_fixed_array_type(total, element)` from `daslib/ast_boost` — it hoists the element's qualifiers onto the new head for you.
- **Typemacro payloads moved**: `$mytag(args...)` argument expressions are in `typeMacroExpr`, not `dimExpr`. Update any pre-0.6.3 macro that read `t.dimExpr` for tag payloads.
- Walking to the element: `var leaf = t; while (leaf.baseType == Type.tFixedArray && leaf.firstType != null) { leaf = leaf.firstType; }` — collect `fixedDim` per level if you need the flattened dims (runtime `TypeInfo.dim[]` stays flattened; only the AST is structural).

## Shared AST-match helpers

`daslib/ast_match.das` exposes a small set of public helpers harvested from `linq_fold` + `sql_linq` (then `sqlite_linq`) during the 2026-05 refactor. Reach for these BEFORE writing a new `is X / as X` cascade — they capture the exact semantics each pattern was hand-rolling, with module-gating and generic-instantiation transparency baked in.

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

**When the patterns apply (and when they don't).** These helpers earn their keep in files that **probe AST shape** to route macro emission — `linq_fold`, `sql_linq`, `ast_match` itself. Files that only **emit code** without introspecting it — `decs_boost`, the emitter half of `templates_boost` — won't find adoption sites. Audit before mechanically searching: if a file has zero hand-rolled `is X / as X` call-cascades and zero qname construction, the patterns don't apply there.

## `qmatch` — predicate-style pattern matching

Prefer `qmatch(expr, <pattern>).matched` over hand-rolled `is X / as X` cascades when matching structural AST shapes. `ExprRef2Value` wrappers are transparent on both sides (pattern + source) — see ast_match.das's header; `$e` captures bind the peeled node.

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

Canonical examples in `daslib/sql_linq.das` — search for `qmatch(` for 37+ adoption sites. Tests (repo-only) in `tests/ast_match/test_qmatch_*.das` + `test_capture_*.das` exercise every tag and grammar form. Full pattern grammar lives in `daslib/ast_match.das`.

**Not every probe fits qmatch.** Shapes with cross-statement constraints (e.g., "3 statements with specific types where push target equals res var and recordNames count matches sources count") exceed qmatch's grammar — fall back to hand-rolled `is X / as X` for those. Self-circular file dependencies are also out: `ast_match.das` itself can't use `qmatch` to define its own grammar.

## `match` (daslib/match) — node-class destructuring; pairs with `qmatch`

`daslib/match` is the OTHER pattern matcher, and for AST work the two divide cleanly. Pick by what the pattern looks like:

- **`qmatch`** when the pattern is *daslang source syntax* — `qmatch(that, $e(fa) * $e(fb) + $e(other))`. Operator trees, call shapes, field chains spelled as code.
- **`match`** when the pattern is *node classes and fields* — `match (e) { if (ExprSwizzle(mask = "xy", value = $v(v))) … }` — or plain value dispatch (`match (op) { if ("*") … }`, enum tables like flatten's `zero_const_of`).

What `match` does that hand-rolled ladders and qmatch don't (all test-pinned in `tests/match/`, repo-only):

```das
match (keySide) {
    // nested class patterns, literal field values, capture; null guard + is/as + ExprRef2Value
    // peel are all emitted for you
    if (ExprField(name = "key", value = ExprVar(name = match_expr(bindName)))) {
        return lane == TableLane.KV
    }
    if (ExprVar(name = match_expr(bindName))) {
        return lane != TableLane.KV
    }
    if (_) {
        return false
    }
}
return false   // match is statement-shaped; flow analysis wants the trailing return
```

- **Alternation `||` works in field position** — `ExprOp2(op = "+" || "-")` — and at arm level; guards compose with `&&` referencing captures and locals.
- **`match_expr(localVar)`** compares a field against a runtime expression (das_string fields compare against `string` locals directly).
- **das-vector fields can NOT be destructured** — `ExprCall.arguments` / `ExprBlock.list` are `dasvector`-backed and the array-pattern arm rejects them ("is not an array"). Capture the node and index/length-check manually; this is why deep block-shape probes (`extract_decs_bridge`) stay hand-rolled.
- **Statement-shaped, not expression-shaped** — a tuple-returning recognizer that mixes name dispatch with structural probes (`is_bucket_reducer_call`) usually reads better hand-rolled; convert only when the ladder is the function.

Canonical conversions: `is_key_ref` / `join_keyb_is_bare_key` (daslib/linq_fold_table.das / linq_fold_common.das), flatten_opt's `component_read_of` + `zero_const_of`. flatten_opt and the linq_fold family require BOTH libraries and use each where it fits — do the same.

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

`qmacro` / `qmacro_block` / `qmacro_expr` / `qmacro_block_to_array` all go through [`apply_template`](../daslib/templates_boost.das#L418), whose substitution visitor calls `clone_expression` on every `$e(...)` substitution input (templates_boost.das:252). So this is wasted work:

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

PERF024 catches both shapes. Canonical annotated set (grows over time): `peel_lambda_rename_var`/`_replace_var`/`_rename_2vars` + `qm_extract_stmts` in `ast_match`, `push_block_list` + `apply_qmacro_template_function` in `templates_boost`, the `emit_*`/`finalize_emission_stmts` family in `linq_fold`, `push_bind`/`push_inline_id`/`push_inline_lit` in `sql_linq`.

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
documented in `history/dasSQLITE/API_REWORK.md` chunk-2 section is the
cautionary tale: `_sql` shipped briefly with `canVisitArgument → false`
overrides because a synthetic test seemed to show inner expansion not
firing. The real cause was a missing `require daslib/linq_boost public`
in `sql_linq.das` — without that, `_where` / `_select` weren't
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

### `ExprRef2Value` transparency (qmatch + match)

Post-typer AST walking sees field reads wrapped in `ExprRef2Value` (no surface syntax). Both matchers peel it automatically: `qmatch` strips it on the pattern AND source side at every dispatch (ast_match.das header; `$e` captures bind the peeled node), and `match` peels it for AST class patterns + `$v` captures via `match_peel_r2v` (an explicit `ExprRef2Value(...)` pattern in `match` still matches the wrapper itself — only `match` can spell it). Hand-written analyzers that DON'T go through a matcher still need `qm_peel_ref2value(node)` (the in-place helper in `daslib/ast_match.das`) at their entry — never hand-roll the `while (… is ExprRef2Value)` loop.

### When you really do need raw arguments

If a macro genuinely needs un-expanded raw AST (rare — mostly for
qmacro-only sugar that builds new code without inspecting it), override
`canVisitArgument` to return `false` for those argument indices. Pair
with a unit test that exercises the inner-macro composition case
specifically — that's the path that breaks first when the override is
wrong.
