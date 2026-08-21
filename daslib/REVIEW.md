# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`daslib/ARCHITECTURE.md`.

**A lint report path checks suppression BEFORE its dedup insert, and the rule code stays part
of the dedup key.** A nolint'd finding that takes the (rule, location) slot silences a
co-located sibling rule; a key without the code collides across rules.

**PERF026-028 honor a `// nolint` at ANY frame of the reported chain.** The sink often
bottoms out in daslib while the actionable line is an intermediate call site — narrowing the
check to the sink line is a defect.

**Moving an `in_closure` / `in_deferred` guard inside a lint visitor method is a semantic
change, never cleanup.** The guard's position encodes which rules are meaningful inside a
lambda; the per-rule policy is in `daslib/ARCHITECTURE.md`.

**A lint warning anchors at the code its fix rewrites.** A remedy that deletes a statement
reports the statement; a remedy that edits one variable's initializer reports the variable —
an anchor chosen for implementation convenience instead is a defect.

**Per-loop visitor state is saved and restored per loop — a stack, never a bare scalar.**
A scalar survives into the sibling loop's exit path and unbalances its counter for the rest
of the walk.

**A daslib predicate that replicates a C++ compiler decision changes in lockstep with its
C++ side.** The pairs: `lint022_optimized` / `lint022_calls_may_be_inlined` ↔
`Program::getOptimize` / `Program::patchInline`; `stale_scan_line` ↔
`rtti_is_nolint_suppressed`; `is_inline_temp_name` ↔ `INLINE_TEMP_PREFIX`; the
STYLE024/025 unsafe map ↔ infer's `unsafe_*` rules; `style036_inert_contract` ↔ infer's
contract clearing. Nothing fails when one side moves alone.

**A lint directive is recognized only where prose cannot reach it — widening any of the
three windows is a defect.** `nolint:` must be the comment's first token, `@nolint` the
first token after `//` / `//!`, and `// lint-skip-file` is honored only within the header
window; otherwise a doc comment, a URL, or a commented-out example silently unlints code.

**`build_lint_macro_disabled` layers four sources in order — defaults, repo `off`, repo
`on`, environment — and fixture mode skips BOTH the default seeding and the repo `off`
directives.** Env last lets a one-run `DAS_LINT_DISABLE` beat a `CODE = true`; repo policy
must not silence the rule a fixture exists to exercise.

**LINT010 records a store in the POST-visit of `ExprCopy`/`ExprClone`/`ExprMove`, never in
`preVisitExprVar`.** The LHS's variable fires before the RHS is walked; an early record
lets the RHS's own read clear the store it just recorded.

**A rule identifying a callee by name matches the root generic, not the instance, and
never fires inside its own advertised module.** Instantiation mangles names and strips
witness arguments; a rule advertising module M's idiom firing on M's own dispatch is a
defect.

**A collapse suggestion compiles and preserves semantics for the exact shape reported, and
exactly one sibling rule fires on it.** Gate on receiver type, arity, cloneability, and
duplicate constant keys; overlaps stay partitioned (STYLE021 owns the const-key run over
STYLE031, STYLE032 owns fresh-empty declarations over STYLE033, chains yield plain
receivers to the variable form).

**The lint message text is the rule-code transport: the code is everything before the
FIRST colon.** Suppression, disable lists, and dedup all parse it from the message —
rewording a message so the code is not the leading colon-delimited token makes the rule
unsuppressable and self-colliding.

**style_lint overrides no `canVisit*` method — adding one is a defect** until the
unsafe-frame push/pop (one frame per expression) is taught about the skipped subtree;
the balance panic in each entry point is the tripwire.

**The arity caps mirror overload sets that live elsewhere** — `MAX_CONCAT_ARITY` ↔ linq's
`concat` variadics, `MAX_VARIADIC_PUSH_ARITY` ↔ builtin `push_from`/`push_clone_from`;
growing either overload set without the cap is a silently missed finding, the reverse a
suggestion that does not compile.

**AOT emit is fail-closed: every entry point tests `macroException`/`failToCompile` BEFORE
materializing output.** A codegen exception mid-visit leaves partial C++; the emitter never
reports through `panic` (`runMacroFunction` swallows it) — an unreachable emit state writes
`#error` into the output instead.

**The two places that build a struct's C++ identifier agree via `buildStructEnumCollisions`,
seeded before any AOT name is emitted.** An emit path that skips the seeding, or a suffix
rule changed on one side, produces `offsetof`s naming a struct declared under a different
identifier.

**`match_error` stores a BORROWED `LineInfo` pointer — pass a pattern node's location,
never a synthesized access expression's.** Access nodes are cloned per field inside a bare
scope and die with it.

**A macro that lowers lambdas reports through `macro_sticky_error`, never `macro_error`.**
A later pass lowers the lambda to a plain function and clears non-sticky errors.

**UTF-8 byte-class tables are indexed through `uint(uint8(ch))`** — `for (ch in string)`
yields a SIGNED byte under JIT; a raw index reads out of bounds for every byte >= 0x80.

**A residual oracle mirrors its transform's gate exactly and never calls the transform.**
Every flatten_opt rewrite arm pairs with a read-only predicate the residual visitors use to
prove the pass complete; a narrower oracle is a false pass, a wider one a false miss, and
calling the transform from an oracle aliases the live tree.

**Every new fold/fuse arm declares its float class.** Inf/NaN/rounding/association changes
are fast-math-only; bit-exact per-lane rewrites are never gated. An arm added without that
decision silently changes output under `_flatten_no_fast_math`.

**A fuse arm emits a call only after proving the target module can resolve it**, and the
miss path falls back to the unfused shape — the pass must never turn a shader that compiled
into an unresolvable call on a narrower backend.

**`flatten_function`'s pass order is load-bearing** (dse → copy-prop → mask-const-prop →
dse → ssa-rename): each pass produces the next one's input; reordering leaves scaffolding
in a twin that must verify branchless and call-free.

**`lift_expr` and the lowering it drives use plain recursion, never `make_visitor`** —
inlining recurses back through the pipeline, and a nested visitor traversal corrupts the
visitor machinery.

**A macro that splices the same subexpression more than once pre-binds it to a local ref
first** — re-splicing re-evaluates a call once per splice; a lock/unlock pair then releases
a different temporary than it took.

**Every `[sql_table]` helper pair registers its 2-arg form BEFORE the 1-arg form** —
`find_struct_helper_fn` keeps the LAST match, and the finish pass rewrites the helper it
returns; swapped, index DDL attaches to the wrong overload.

**A generated SQL statement's column list and its bind function change in one edit.** Bind
indices are placeholder positions over bindable columns, never struct field positions;
drift is silently wrong data, not an error.

**A new environment-reading spelling joins the env_registry marker lists in the same
change.** The scanners match source text; a missing spelling makes every enforcement test
pass vacuously.

**`ast_verify` checks name the C++ site that dereferences the slot unguarded, and repair
the slot as well as reporting.** A check with no such site invents an invariant the
compiler does not hold (`if_false`, `ExprFor.body`, `with (module x)` are legitimately
null).

**64-bit range sums prove both operands non-negative BEFORE summing as `uint64`**
(`erase(at, count)`): a signed sum wraps, the bounds test passes, and the walk leaves the
array silently.

**Macro-built AST carries no branch a macro-time value can decide** — nothing folds at
macro-application time, so a generated `if ($v(flag))` keeps its dead arm and type-checks
it. Branch in daslang and emit only the taken arm.

**Every buffer-I/O overload returns before taking `addr(buf[0])` on an empty buffer** —
the address is out of bounds and the call sits inside `unsafe`; a new overload without the
guard passes every non-empty test.

**`apply`'s inline path decides lvalue-ness by node KIND, never by a ref flag**, and the
generated let carries `alwaysSafe` — that, not an `unsafe` block, licenses the variant
access; an rvalue stays materialized because a reference to a temporary dangles.

**A swizzle rewrite reuses each source node once and clones every repeat** — the first
appearance moves, a second output lane MUST clone, or one node gets two parents.

**The RST label and topic key are computed twice — pre-infer and post-infer — and must
agree byte-for-byte**, or the page prints a bare signature and the symbol re-stubs.
