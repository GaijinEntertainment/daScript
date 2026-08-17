# Macro & AST programming — applied

Read before writing a compile-time macro, AST-manipulating code, or `qmacro` codegen. Language
truth — pipeline, macro kinds and hooks, registration, AST-pointer rules, the reification and
`qmatch` tag tables — is `skills/daslang/references/macros.md`. This file carries what that
reference does not: the failure modes, the daslib helpers, the emission cookbook.

## Debug with the AST verifier — FIRST, not last

`daslib/ast_verify` reports at the offending node instead of crashing passes later, so reach for it
the moment a macro misbehaves. Which entry point depends on how much of the tree you can see:

| Call | Use when |
|---|---|
| `daslang --ast-verify file.das` | first move on any unexplained crash — checks every non-builtin module before each infer pass. `require daslib/ast_verify` pins that sweep in the source; `verify_module(prog, mod)` runs it at the end of your `apply()` |
| `verify_expression(expr)` | you BUILD and RETURN a subtree — call / for-loop / variant / reader macros run inside inference, where the module-level form cannot see your result yet |
| `verify_function(fn)` | you BUILD a function — `add_function` mangles the signature immediately, so a malformed result or argument type crashes there, before any pass could run |

`options _ast_verify_unique = false` turns off the one-parent check for a module that shares nodes
on purpose. A macro that re-breaks the same node every pass never converges: the verifier repairs,
you break it again, and the compile ends in `error[30507]: type inference exceeded maximum allowed
number of passes`. That is your bug — break the node once.

## Emitted nodes the typer will not repair

**Pre-set `_type` on an `ExprVar` that flows into a generic call.** A null `_type` propagates
through every `clone_expression`, and the generic-instantiation pass — which runs *before*
local-variable resolution — commits to `auto`: `error[30165] cannot infer ... return type with
'auto'`, cascading through every consumer. Fix at emission time, not later:

```das
var pvar = new ExprVar(at = at, name := boundName)
pvar._type = clone_type(boundElementType)
pvar._type.flags.ref = true
```

**The malformed-AST family** — required-but-nullable fields, repaired in place so infer keeps
reporting, but the error is sticky and the compile still fails: a `Variable` with null `_type`
(`error[50640]`, `50608` on a structure field); a for loop's null `body`, or `iteratorsAt` /
`iteratorsAka` left shorter than `iterators` when rewriting a loop (`50607` — keep the parallel
vectors the same length); null ENTRIES in `variables` / `arguments` / `sources`; a generated
`Function` with null `result` (`50609`). Null expression CHILDREN (`ExprOp2.left`, an `if`
condition) are NOT guarded — they crash inside the visitor walk, and the stack is your diagnostic.

**`macro_sticky_error(prog, at, message)`** (ast module, beside `macro_error`) — for a macro that
detects damage, repairs it so infer can proceed, but must still fail the compile. A plain
`macro_error` recorded on a pass that also changed the AST is discarded by the next infer pass.

## Macro modules each compile into their own context

Compile-time globals a macro mutates — a pattern registry, an adapter list, any `[_macro]`-populated
state — live in *that module's* context, so a macro in module B cannot register into module A's
registry, and B calling A's accessor reads **B's** copy: silently empty, no error. Anything another
macro module must read travels through the **AST** (annotations and stamps a pass can walk),
**files**, or **env** — never a shared macro global.

To contribute macro-time state into A's registry the contributor must compile into A's context —
**A `require`s the contributor**, never the reverse. Make that inverted dependency conditional with
two pieces: an optional require (`require ?<guard> <target>`; a plain `require` resolves before
`static_if`, so `static_if` alone cannot gate it), plus
`static_if (typeinfo builtin_module_exists(<guard>))` around the registration call and any
dispatcher branch naming the contributor's symbols. Adapters may still *emit* code referencing
those symbols by name — that resolves at the user's splice site and is unaffected.

## Pass macros — which hook sees what

- `[infer_macro]` fires only AFTER inference succeeds — it never sees a program with errors, so it
  instruments well-typed trees. `[dirty_infer_macro]` fires after EVERY infer pass, errors present
  or not: the hook for augmenting a not-yet-inferable program. Each pass starts by clearing errors
  and the loop continues only while a macro returns true or inference progresses, so a late binder
  **must** return true when it changes the tree, or the current errors become final.
- StructureAnnotation `finish` / `patch` do NOT run for a struct whose inference failed; `apply` is
  the only guaranteed hook, so a deferred better-error cannot live in `finish`. (probe-verified
  2026-08-07)
- **A `[simulate_macro]` in a shared macro module NEVER fires for the user's program** — it only
  sees the program its own module was compiled as. The hooks that do see the user program:
  `[pre_infer_macro]`, `[infer_macro]`, `[dirty_infer_macro]`, `[post_infer_macro]`,
  `[lint_macro]`, `[global_lint_macro]`, `[post_compile_macro]`.
- `[post_compile_macro]` is the only hook running after the module's gc root is collected —
  everything earlier still sees the garbage inference left behind, so orphan hunting and gc
  bookkeeping belong there. Use its `prog` argument; `compiling_program()` throws by then.

## Structure macros — applied notes

`apply` runs at parse time, in written order: a later annotation in the same bracket group sees
every mutation the earlier one made. Several structure macros on one struct are fine — the
only-one restriction is for handled-type annotations.

- **`[|> foo]` marks an annotation inherited** — copied to every DERIVED struct down the parent
  chain and PREPENDED before the child's own, so it always applies first. It fires on the carrying
  struct too; discriminate with `st.flags.isTemplate`. (probe-verified 2026-08-08)
- **`typedef NAME = <type>` inside a struct body is grammar** — it registers a structure alias
  readable at apply time via `get_structure_alias(st, name)`; infer resolves alias-typed fields
  against the owning struct's aliases.
- **Annotation argument VALUES accept bare identifiers**: `@dflt = g_one` ≡ `@dflt = "g_one"` (the
  NAME stores as `tString`, so `find_arg(...) ?as tString` reads both), and `= @@name` for a
  function reference. Prefer unquoted when the value IS an identifier. The formatter never re-quotes.
- **Annotation argument NAMES accept only three keywords** — `type`, `in`, `default`. Any other
  keyword or type token is `error[30151] syntax error, unexpected <token>, expecting type or in or
  default or name`, and the same whitelist governs field annotations, so `@range = 5` fails too —
  pick a synonym (`span`, `kind`, `fallback`). (probe-verified 2026-08-16)
  Extending the whitelist is one `src/parser/ds2_parser.ypp` line plus the twin `_annotation_argument_name` line in `tree-sitter-daslang/grammar.js`; keep both grammars in lockstep. (repo-only)
- **`add_structure_field(st, name, type, default)`** (in `daslib/templates_boost`, not `ast`)
  returns the new field's **index** — keep it when you must touch the field again.
- **`ExprFieldFieldFlags.no_promotion` / `ExprAtFlags.no_promotion`** stop the compiler promoting a
  generated field or index access to a different type.
- **`add_generic` over `add_function`** when the generated function must participate in overload
  resolution or needs inference (a per-field `push` resolving to clone or copy); `add_function` is
  for the concrete, fully typed case.
- **Never certify a macro rail green while ANY other compile error is present** — final-verify
  errors are masked when an earlier unrelated error stops compilation first, so two runs differing
  only by that error can flip a green macro red. (probe-verified 2026-08-08)
- **Generated functions take `fn.flags.generated = true`** — the field form;
  `fn.flags |= FunctionFlags.generated` is what STYLE022 flags.
- **Per-field operations must assume non-copyable fields** (`array<T>`, `table<K;V>`, lambdas):
  `push_clone` per field for clone semantics, `emplace` per field for move semantics, `:=` for
  assignment. Keep one test struct with an `array<T>` field — that is what catches a plain copy.

### Deriving facts from method bodies — bodies are PRE-infer

A structure macro that *reads* its struct's method bodies — deriving a contract instead of making
the author declare it — walks un-inferred AST: a field access is a bare `ExprVar` carrying the name,
member access is a parser-level `ExprField` whose base is untyped, so "is this base my struct?" is
unanswerable and you match on names. Name-only `ExprField` matching collides with swizzles (`wv.y`
on a `float4` local is an `ExprField` named `y`), so match bare names plus explicit `self.<name>`
and nothing else, do not recurse into free callees, and refuse a whole `self` passed as an argument
rather than guessing. Key every derived claim by source location, not node pointer — infer clones
subtrees, and `LineInfo` is what survives. Keep the hand-written declarations in place while the
derivation lands: each becomes an assertion against the derived answer, which is how stale
annotations surface.

## AST introspection

- **`typeDecl.argTypes`** — for a function-typed field, `argTypes[0]` is the `self` parameter, and
  `.isConst` on it says whether the method is const. A const getter that must mutate a cache
  declares `self : T ==const`, then `var pS = unsafe(addr<T? -const>(self))` — the interior
  mutability `daslib/interfaces.das` uses for const-only interface proxy caching.
- **`get_const_expr(program, expr)`** — the compiler's own demand fold, for the VALUE of a const the
  policy left unfolded (a lint run, `options infer_time_folding = false`). It returns a fresh
  constant expression or `null`, folding arithmetic and constant constructor calls (`A * 4`,
  `float4(1,2,3,4) / 2`, a const named off another const) that a hand-rolled is-this-constant test
  does not, and it records no access-fold, so asking never suppresses unused-variable lint. The
  answer lands on whichever gc root is in force: from application code wrap the ask in
  `ast_gc_guard()`, or the node dangles and crashes the exit leak report, nowhere near the ask.
- **Fixed-array chains** carry sentinels before inference: `fixedDim` is **-1** for `[]` and **-2**
  while `fixedDimExpr` awaits folding (das macros compare the literals — the C++ enum names are not
  bound). Post-inference both are resolved, and `fixedDim <= 0` reaching final verify is an error.

## Shared AST-match helpers — `daslib/ast_match`

Reach for these before writing a new `is X` / `as X` cascade; each captures semantics that were
hand-rolled and got subtly wrong.

| Helper | Purpose |
|---|---|
| `match_call_in_module(expr, name, mod)` → `ExprCall?` | match a call by name and module, transparent to generic instantiation (falls back to `func.fromGeneric`); `match_call_in_linq` is the `mod="linq"` wrapper |
| `peel_lambda_single_return(lam)` | `@(x : T) => expr` → `expr`; null when the shape differs |
| `peel_lambda_rename_var` / `_replace_var` / `_rename_2vars` | peel and rebind the lambda argument. `rename_var` falls back to `invoke(expr, name)` when non-peelable, so callers can splice unconditionally; the 2-var form returns null and lets the caller decide |
| `peel_tuple_field_read(expr, bind, idx)` | true when `expr` is `<bind>._<idx>` |
| `extract_const_string(e)` → `tuple<bool; string>` | consume a compile-time string literal threaded through macro arguments |
| `qn(prefix, at)` | deterministic qualified name for a macro-emitted local. A synthesized `LineInfo()` (0, 0) WILL collide across synth sites sharing a prefix — build a distinct name when that matters |
| `qm_peel_ref2value(var e)` | the single home of `ExprRef2Value` peeling — never hand-roll `while (… is ExprRef2Value)` |
| `push_block_list(var stmts, var blk)` (`daslib/templates_boost`) | splice a `qmacro_block` result into an `array<ExpressionPtr>`, cloning each statement |

They earn their keep in files that **probe** AST shape to route emission — `linq_fold`, `sql_linq`,
`ast_match` itself; files that only **emit** (`decs_boost`, the emitter half of `templates_boost`)
have no adoption sites. `qmatch` limits: shapes with cross-statement constraints ("three statements
where the push target equals the result var and the recordNames count matches the sources count")
exceed its grammar — hand-roll those, and `ast_match.das` cannot use `qmatch` to define its own
grammar. Canonical adoption is `daslib/sql_linq.das` (37+ sites).

## `match` (daslib/match) — node-class destructuring; pairs with `qmatch`

For AST work the two matchers divide by what the pattern looks like: **`qmatch`** when it is
*daslang source syntax* (`qmatch(that, $e(fa) * $e(fb))`) — operator trees, call shapes, field
chains spelled as code; **`match`** when it is *node classes and fields*, or plain value dispatch
(`match (op) { if ("*") … }`, enum tables).

```das
match (keySide) {                   // null guard, is/as and ExprRef2Value peel are emitted for you
    if (ExprField(name = "key", value = ExprVar(name = match_expr(bindName)))) { return true }
    if (_) { return false }
}
return false   // match is statement-shaped; flow analysis wants the trailing return
```

- **Alternation `||` works in field position** — `ExprOp2(op = "+" || "-")` — and at arm level;
  guards compose with `&&` over captures and locals. **`match_expr(localVar)`** compares a field
  against a runtime expression (a `das_string` field compares against a `string` local directly).
- **das-vector fields can NOT be destructured** — `ExprCall.arguments` and `ExprBlock.list` are
  `dasvector`-backed and the array-pattern arm rejects them ("is not an array"). Capture the node
  and index it by hand; this is why deep block-shape probes stay hand-rolled.
- **Statement-shaped, not expression-shaped** — a tuple-returning recognizer mixing name dispatch
  with structural probes usually reads better hand-rolled; convert only when the ladder *is* the
  function.

Canonical conversions: `component_read_of` + `zero_const_of` in `daslib/flatten_opt_common.das`.
flatten_opt and the linq_fold family require BOTH libraries and use each where it fits.

## Emission cookbook

**Don't pre-clone splice inputs** (PERF023, PERF024 — `skills/perf_lint.md`). Every `$e(...)`
input is cloned for you, independently per slot, so one source feeding three slots still needs no
pre-clone; the same holds at calls to a function whose parameter carries `[clone(p)]` (grep
`[clone(` in `daslib/` for the current set). If the source has side effects — rare, most are bare
variable reads — bind it once with a plain `let` and splice the local.

Before putting `[clone(p)]` on your own function, verify it CONSUMES `p` cleanly: every path either
ignores it, clones the pieces it needs into its output, or forwards it to another `[clone]`
function — never mutates it, never retains an alias outliving the call. `apply_template` and every
`apply_qmacro_*` / `apply_qblock_*` variant fail that test: they rewrite `p` in place.

**Collapse push clusters.** Two or more consecutive `arr |> push <| qmacro_expr() { ... }` into the
same array become one emission — `arr |> push_from <| qmacro_block_to_array() { ...all statements... }`
(preferred: pure stdlib composition, no per-element clone) or
`arr |> push_block_list(qmacro_block() { ... })` (clones each element; right when the source block
stays alive after the push). Only PURE consecutive runs collapse: pushes interleaved with
`if`/`elif` branches or with non-push statements do not, because the conditionality lives outside
the qmacro.

## `[call_macro]` entry-guard contract

Arguments are visited before `visit(call)` fires (`canVisitArgument = true`, the default you almost
never override), so field reads may arrive wrapped in `ExprRef2Value`, and an argument that
genuinely cannot type yet — `_.Field` outside a lambda — stays raw with a null `_type`, tolerated
because the outer macro rewrites it anyway.

**If inner macros are not expanding, look for a missing `require` before touching visit semantics.**
A null `_type` on a chain argument almost always means the user did not require the module defining
the inner macro. `_sql` once shipped `canVisitArgument → false` overrides for exactly this
misdiagnosis; the real fix was a missing `require daslib/linq_boost public`. Put a
`{describe(arg)}` in every `macro_verify` message so that diagnosis is one compile away.

If a macro genuinely needs un-expanded raw AST (rare, mostly for qmacro-only sugar that builds code
without inspecting it), return `false` from `canVisitArgument` for those indices and pair it with a
test exercising inner-macro composition — the path that breaks first when the override is wrong.

## Never pattern-match a leading `!`

The reference states the rule; the convention it implies is that the negative form of a macro call
takes a `_not_` or `_no` prefix on the positive name — `_any` / `_none`, `_in` / `_not_in`,
`_is_null` / `_is_not_null`, `_between` / `_not_between`. It binds macro-expansion-time matching
only: plain runtime `!any(...)` with no macro routing through it is fine. When documenting a macro
with negated forms, list the pair together — never "negate with `!`".
