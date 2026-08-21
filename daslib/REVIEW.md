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
