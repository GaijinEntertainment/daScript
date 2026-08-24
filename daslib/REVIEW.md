# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`daslib/ARCHITECTURE.md`. Planned work: `daslib/followup_comment_sweep.md`.
A diff touching the linq family - `linq*.das`, `sql_*.das` - applies `REVIEW_LINQ.md` too.

**A lint report path (`perf_warning` / `style_warning` / `lint_error`) checks suppression BEFORE
its dedup insert.** A nolint'd finding that takes the (rule, location) slot silences a
co-located sibling rule.

**The rule code stays part of the dedup key.** A key without the code collides across rules.

**PERF026-028 honor a `// nolint` at ANY frame of the reported chain.** The sink often
bottoms out in daslib while the actionable line is an intermediate call site - narrowing the
check to the sink line is a defect.

**Moving an `in_closure` / `in_deferred` guard inside a lint visitor method is a semantic
change, never cleanup.** The guard's position encodes which rules are meaningful inside a
lambda; the per-rule policy is in `ARCHITECTURE.md` sec. perf_lint.

**A lint warning anchors at the code its fix rewrites.** A remedy that deletes a statement
reports the statement; a remedy that edits one variable's initializer reports the variable -
an anchor chosen for implementation convenience instead is a defect.

**Visitor state scoped to a construct is reset or restored on every exit path of that
construct.**

**Per-loop visitor state is a stack, never a bare scalar.** A scalar survives into the
sibling loop's exit path and unbalances its counter.

**Per-function visitor state resets in `preVisitFunction`, ahead of any early return.**
A latch that skips the template early-return poisons the next function.

**A diff that makes a daslib predicate or emitted identifier depend on a C++-side
definition records the pair in `ARCHITECTURE.md`, in its module's section, naming both
sides.** Nothing catches it when one side later moves alone.

**When a diff changes one side of a recorded daslib/C++ pair so the two no longer match,
it changes the other side and updates the pair's `ARCHITECTURE.md` entry in the same
diff.**

**Weakening the nolint-window tests is a defect** - `tests/lint/test_nolint_suppression.das`
pins that a string literal, a URL, and a mid-comment `nolint:` do not suppress while a
first-token directive after `//` or `//!` does, and `tests/lint/test_stale_nolint.das` pins
that a `lint-skip-file` past the header window is prose.

**`build_lint_macro_disabled` layers four sources in this order: defaults, repo `off`, repo
`on`, environment.** Env last lets a one-run `DAS_LINT_DISABLE` beat a `CODE = true`.

**`options _enable_default_off_rules` skips BOTH the default seeding and the repo `off`
directives.** Repo policy must not silence the rule a fixture exists to exercise.

**Weakening `tests/lint/test_lint_config.das` is a defect** - it pins that each `[format]`
key resolves nearest-wins independently up to the `.git` root, so a nearer `.lint_config`
declaring one key does not reset the other.

**Weakening the kept-comment cases in `utils/mcp/test_tools.das` is a defect** - they pin
the formatter's kept set.

**A daslib module that emits a lint rule id joins `RULE_MODULES` in `utils/lint/REVIEW.das`
in the same change.** An unlisted module's ids are never scanned, so its fixture-and-rst
check never runs.

**LINT010 records a store in the POST-visit of `ExprCopy`/`ExprClone`/`ExprMove`, never in
`preVisitExprVar`.** The LHS's variable fires before the RHS is walked; an early record
lets the RHS's own read clear the store it just recorded.

**A rule identifying a callee by name matches the root generic, not the instance** -
instantiation mangles names and strips witness arguments.

**A rule never fires inside the module whose idiom it advertises** - a rule advertising
module M's idiom firing on M's own dispatch is a defect.

**A collapse suggestion compiles and preserves semantics for the exact shape reported** -
gate on receiver type, arity, cloneability, and duplicate constant keys.

**Exactly one sibling rule fires on a shape several collapse rules can match.** Overlaps
stay partitioned: STYLE021 owns the const-key run over STYLE031, STYLE032 owns fresh-empty
declarations over STYLE033, and STYLE033's chain-receiver check leaves a plain-variable
receiver to STYLE033's variable-form check, so the two never double-fire.

**The lint message text is the rule-code transport: the code is everything before the
FIRST colon.** Suppression, disable lists, and dedup all parse it from the message -
rewording a message so the code is not the leading colon-delimited token makes the rule
unsuppressable and self-colliding.

**A `canVisit*` override in style_lint ships with the unsafe-frame push/pop taught about
the subtree it skips.** The walk pushes one frame per expression and pops it, so a skipped
subtree unbalances the count; the balance panic in each entry point is the tripwire.

**The arity caps mirror overload sets that live elsewhere** - `MAX_CONCAT_ARITY` <-> linq's
`concat` variadics, `MAX_VARIADIC_PUSH_ARITY` <-> builtin `push_from`/`push_clone_from`;
growing either overload set without the cap is a silently missed finding, the reverse a
suggestion that does not compile.

**A diff that adds or changes an emit entry point - a function that runs the emit visitor
(`CppAot` or its subclass `StandaloneContextGen`, the visitors that write C++) and then
returns or writes the generated C++ - keeps the error check ahead of that return
or write.** The error check is the program's `macroException`/`failToCompile` state, or
`log_aot_emit_errors` where the emit runs standalone; a codegen exception mid-visit
leaves partial C++.

**A visitor override in the emit visitor never gates on `macroException`/`failToCompile`.**
The entry point owns that check; an override that returns early on the error state emits
truncated C++ that the entry point still writes out as complete.

**An unreachable emit state writes `#error` into the output, never `panic`** -
`runMacroFunction` swallows a panic, so the emitter never reports through it.

**A diff that changes how a struct's C++ name is built changes every site that spells
that name the same way.** The name is the module namespace (`aotModuleName`, or
`aotModuleNameFromString` on the `VarInfo` path) plus the `_S` suffix; the sites that
spell it are `aotStructName` and the `VarInfo` emitter's inline
`aotSuffixNameEx(info.name, "_S", ...)`. One site changed alone writes `offsetof`s that
name a struct declared under a different name.

**A diff that adds or changes a function that runs the emit visitor keeps
`buildStructEnumCollisions` running before that visitor runs.** It may run in a helper
(`dumpDependencies` seeds it today): the table decides when a name gets its collision
suffix, and a run that skips the seeding spells structs differently from the one that
seeded it.

**`match_error` stores a BORROWED `LineInfo` pointer - pass a pattern node's location,
never a synthesized access expression's.** Access nodes are cloned per field inside a bare
scope and die with it.

**A macro that lowers lambdas reports through `macro_sticky_error`, never `macro_error`.**
A later pass lowers the lambda to a plain function and clears non-sticky errors.

**UTF-8 byte-class tables are indexed through `uint(uint8(ch))`** - `for (ch in string)`
yields a SIGNED byte under JIT; a raw index reads out of bounds for every byte >= 0x80.

**A parser result that borrows a view into a tree the parser allocated names the owning
field and ships the scope-ender that frees it.** Nothing else can free it - the tree must
outlive the caller's read, and daslang finalizes neither a raw pointer field nor a local
container at scope exit - so a borrowed view with no named owner leaks the whole document
per call, invisibly until the process is long-lived.

**A conversion that throws on out-of-range input is a panic on untrusted bytes.** `int64` /
`uint64` / `double` on a string throw; a lexer or decoder reachable from a file, a socket,
or a model reaches for the non-throwing `to_*` twin and reports through its own error
channel.

**A parser never re-derives control flow from the text of its own diagnostics.** A message
carries user data, so `starts_with` on an error string is an input-controlled branch; the
token that caused the failure is what the decision reads.

**Every flatten_opt rewrite arm ships a read-only residual predicate** - the oracle the
residual visitors call to prove the pass complete.

**A residual oracle mirrors its arm's gate exactly and never calls the transform.** A
narrower oracle is a false pass, a wider one a false miss; calling the transform from an
oracle aliases the live tree. A gate the transform takes as a parameter is threaded into the
oracle too - re-spelling its default as a constant makes every non-default run a false pass,
and the tests that call the oracle pass the same value.

**Every new fold/fuse arm declares its float class.** Inf/NaN/rounding/association changes
are fast-math-only; bit-exact per-lane rewrites are never gated. An arm added without that
decision silently changes output under `_flatten_no_fast_math`.

**A fuse arm emits a call only after proving the target module can resolve it**, and the
miss path falls back to the unfused shape - the pass must never turn a shader that compiled
into an unresolvable call on a narrower backend.

**A new store spelling joins `MutCollect` in the same change.** CSE reads a name outside the
mutable set as constant for the whole block, so an uncollected store is a silently shared
subexpression.

**A statement the lowering cannot predicate is refused, never dropped.** `lower_stmt`'s
fall-through drop is licensed only because every call that survives lowering is pure.

**`flatten_function` runs its passes in this order - dse -> copy-prop -> mask-const-prop ->
dse -> ssa-rename - and a diff that reorders them is a defect.** Each pass produces the next
one's input; a reorder leaves scaffolding in the generated `<name>_flat` twin, which must
come out branchless and call-free.

**`lift_expr` and the lowering it drives use plain recursion, never `make_visitor`** -
inlining recurses back through the pipeline, and a nested visitor traversal corrupts the
visitor machinery.

**A macro that splices the same subexpression more than once pre-binds it to a local ref
first** - re-splicing re-evaluates a call once per splice; a lock/unlock pair then releases
a different temporary than it took.

**Every `[sql_table]` helper pair registers its 2-arg form BEFORE the 1-arg form** -
`find_struct_helper_fn` keeps the LAST match, and the finish pass rewrites the helper it
returns; swapped, index DDL attaches to the wrong overload.

**A generated SQL statement's column list and its bind function change in one edit.** Bind
indices are placeholder positions over bindable columns, never struct field positions;
drift is silently wrong data, not an error.

**A new daslib spelling that reads the environment joins the env_registry marker lists in
the same change.** The scanners match daslang source text; a missing spelling makes every
enforcement test pass vacuously.

**An `ast_verify` check names, in `ARCHITECTURE.md` sec. ast_verify, the C++ `::visit` site
that dereferences the slot unguarded.** A check with no such site invents an invariant the
compiler does not hold, and the two passes disagree - a slot infer fills in is checkable
only post-infer.

**An `ast_verify` check repairs the slot it reports, not only reports it.** A broken slot
left in place trips every later pass that walks the same node.

**64-bit range sums prove both operands non-negative BEFORE summing as `uint64`**
(`erase(at, count)`): a signed sum wraps, the bounds test passes, and the walk leaves the
array silently.

**Macro-built AST carries no branch a macro-time value can decide** - nothing folds at
macro-application time, so a generated `if ($v(flag))` keeps its dead arm and type-checks
it. Branch in daslang and emit only the taken arm.

**Every buffer-I/O overload returns before taking `addr(buf[0])` on an empty buffer** -
the address is out of bounds and the call sits inside `unsafe`; a new overload without the
guard passes every non-empty test.

**`apply`'s inline path decides lvalue-ness by node KIND, never by a ref flag** - an rvalue
stays materialized, because a reference to a temporary dangles.

**A `let` generated by `apply`'s inline path carries `alwaysSafe`** - that flag, not an
`unsafe` block, licenses the variant access.

**A swizzle rewrite reuses each source node once and clones every repeat** - the first
appearance moves, a second output lane MUST clone, or one node gets two parents.

**A diff that changes how the RST label or topic key is computed makes the same change in
every place `rst.das` computes it - the stub pass (`generate_module_stubs`) and the
documenting pass (`documents`) - keeping them byte-for-byte equal.** Diverged, the page
prints a bare signature and the symbol re-stubs.
