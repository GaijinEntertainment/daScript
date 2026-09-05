# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_LINT.md`, `ARCHITECTURE_EMIT.md`, `ARCHITECTURE_LINQ.md`.
Planned work: `plans/daslib_comment_sweep.md` (repo root).
A diff touching the linq family - `linq*.das`, `sql_*.das` - applies `REVIEW_LINQ.md` too.

**A diff that changes the body of `perf_warning` (`perf_lint.das`), `style_warning`
(`style_lint.das`), or `lint_error` (`lint.das`) runs the suppression check before the body
records or reports the finding.** A nolint'd finding that takes the (rule, location) slot
silences a co-located sibling rule.

**A diff that changes the dedup key keeps the rule code in it.** A key without the code
collides across rules.

**Never narrow PERF026-028's `// nolint` check to the reported sink line - honor a
`// nolint` at any frame of the reported chain.** The sink often bottoms out in daslib while
the actionable line is an intermediate call site.

**A diff that moves an `in_closure` / `in_deferred` guard to a different statement position
inside a lint visitor method is a defect.** The guard's position decides which rules apply
inside a lambda; the per-rule policy is in `ARCHITECTURE_LINT.md` sec. 1.

**Never move PERF023's `add_ptr_ref` splice-depth tracking (`perf_lint.das`) behind the
`in_closure` gate.** The rules block is itself a closure, so a gated tracker never sees the
splice.

**A lint warning that anchors anywhere but the code its fix rewrites is a defect.** A remedy
that deletes a statement reports the statement; a remedy that edits one variable's
initializer reports the variable.

**Never exit a construct's visitor on a path that skips resetting or restoring the visitor
state scoped to that construct.** State an early return leaves set poisons the next
construct's visit.

**Never keep per-loop visitor state in a bare scalar - keep it in a stack.** A scalar
survives into the sibling loop's exit path and unbalances its counter.

**A diff that adds or changes a daslib fact - code or a `//!` contract - whose truth is
decided by a C++-side definition, with no test, lint, or compile error failing when the two
sides no longer match, records the pair in the architecture doc, in its module's section,
naming both sides.**

**When a diff changes one side of a recorded daslib/C++ pair so the two no longer match, it
changes the other side and updates the pair's architecture-doc entry in the same diff.**

**A diff that adds an architecture-doc entry adds it only where no name, shape, or test can
carry the fact.**

**A diff that changes daslib's nolint scanning - suppression parsing, staleness detection, or
the header window - keeps `tests/lint/test_nolint_suppression.das` and
`tests/lint/test_stale_nolint.das` green without weakening them.** They pin that a string
literal, a URL, and a mid-comment `nolint:` do not suppress while a first-token directive
after `//` or `//!` does, and that a `lint-skip-file` past the header window is prose.

**A diff that changes `build_lint_macro_disabled` keeps its four sources layered in this
order: defaults, repo `off`, repo `on`, environment.** Env last lets a one-run
`DAS_LINT_DISABLE` beat a `CODE = true`.

**A diff that changes `options _enable_default_off_rules` keeps it skipping BOTH the default
seeding and the repo `off` directives.** Repo policy must not silence the rule a fixture
exists to exercise.

**A diff that changes daslib's `.lint_config` resolution keeps `tests/lint/test_lint_config.das`
green without weakening it** - it pins that each `[format]` key resolves nearest-wins
independently up to the `.git` root, so a nearer `.lint_config` declaring one key does not
reset the other.

**A daslib module that emits a lint rule id joins `RULE_MODULES` in `utils/lint/REVIEW.das`
in the same change.** An unlisted module's ids are never scanned, so its fixture-and-rst
check never runs.

**A diff that deletes a rule id's last emitting spelling from a rule module also removes that
id's fixture and its `doc/source/reference/language/lint.rst` (repo root) section, in the same
change** - the gate checks only ids it can still find, so a dead id's residue outlives it
silently.

**Never record a LINT010 store in `preVisitExprVar` - record it in the POST-visit of
`ExprCopy`/`ExprClone`/`ExprMove`.** The LHS's variable fires before the RHS is walked, so
an early record lets the RHS's own read clear the store it just recorded.

**Never match a lint rule's callee name against the instance - match the root generic.**
Instantiation mangles names and strips witness arguments.

**A lint rule that fires on the source of the module that implements the idiom it suggests
is a defect.**

**A diff that adds or changes a collapse check - a lint rule that suggests replacing a run of
statements with one expression - gates the suggestion on receiver type, arity, cloneability,
and duplicate constant keys.**

**A diff that adds or changes the rewrite a lint message suggests adds that rewrite to the
rule's fixture as a good shape that compiles and does not fire.**

**A diff that makes two collapse checks fire on one shape is a defect.** Overlaps stay
partitioned: STYLE021 owns the const-key run over STYLE031, STYLE032 owns fresh-empty
declarations over STYLE033, and STYLE033's chain-receiver check leaves a plain-variable
receiver to STYLE033's variable-form check.

**Never reword a lint message so the rule code stops being everything before the FIRST
colon.** Suppression, disable lists, and dedup all parse the code from the message, so a
message that leads with anything else makes the rule unsuppressable and self-colliding.

**A diff that adds or changes a `canVisit*` override in style_lint keeps the unsafe-frame
count balanced across the subtree the override skips.** The walk pushes one frame per
expression and pops it, so a skipped subtree unbalances the count; the balance panic in each
entry point is the tripwire.

**Never cache `blk.list` index state across style_lint visitor callbacks.** The
per-statement callback carries no index and a shared stack would have to push/pop with
nested blocks; block statement lists are short, so the rescan is the cheap form.

**A diff that changes an arity cap or the overload set it mirrors - `MAX_CONCAT_ARITY` <->
linq's `concat` variadics, `MAX_VARIADIC_PUSH_ARITY` <-> builtin
`push_from`/`push_clone_from` - changes both sides in the same change.** Growing an overload
set without the cap is a silently missed finding; raising a cap without the overloads is a
suggestion that does not compile.

**A diff that adds or changes an emit entry point - a function that runs the emit visitor
(`CppAot` or any subclass of it) and then returns or writes the generated C++ - keeps the
error check ahead of that return or write.** The error check is the program's
`macroException`/`failToCompile` state, read directly or through `log_aot_emit_errors`; a
codegen exception mid-visit leaves partial C++.

**Never gate a visitor override in `CppAot` or any subclass of it on
`macroException`/`failToCompile` - the function that runs the visitor owns that check.** An
override that returns early on the error state emits truncated C++ that the caller still
writes out as complete.

**Never report an unreachable emit state with `panic` - write `#error` into the output.**
`runMacroFunction` swallows a panic, so the emitter never reports through it.

**A diff that changes how a struct's C++ name is built changes every site that spells
that name the same way.** The name is the module namespace (`aotModuleName`, or
`aotModuleNameFromString` on the `VarInfo` path) plus the `_S` suffix; the sites that
spell it are `aotStructName` and the `VarInfo` emitter's inline
`aotSuffixNameEx(info.name, "_S", ...)`. One site changed alone writes `offsetof`s that
name a struct declared under a different name.

**A diff that adds or changes a function that runs `CppAot` or any subclass of it
keeps `buildStructEnumCollisions` running before that visitor runs - directly or in a
helper it calls.** The table decides when a name gets its collision suffix, and a run
that skips the seeding spells structs differently from the run that seeded it.

**Never pass a synthesized access expression's location to `match_error` - pass a pattern
node's location.** `match_error` stores the `LineInfo` pointer BORROWED, and access nodes
are cloned per field inside a bare scope and die with it.

**Never report an error with `macro_error` from a macro that lowers lambdas - report with
`macro_sticky_error`.** A later pass lowers the lambda to a plain function and clears
non-sticky errors.

**Never index a UTF-8 byte-class table with a raw `ch` - index through `uint(uint8(ch))`.**
`for (ch in string)` yields a SIGNED byte under JIT, so a raw index reads out of bounds for
every byte >= 0x80.

**A diff that adds or changes a parser result that borrows a view into a tree the parser
allocated names the field that owns the tree and ships the function the caller runs to free
it.** The tree must outlive the caller's read, and daslang finalizes neither a raw pointer
field nor a local container at scope exit, so a borrowed view with no named owner leaks the
whole document per call.

**Never convert a string with `int64` / `uint64` / `double` in a lexer or decoder reachable
from a file, a socket, or a model - use the non-throwing `to_*` twin and report through the
code's own error channel.** Those three throw on out-of-range input, and a throw on
untrusted bytes is a panic.

**Never branch a parser on the text of its own diagnostics - branch on the token that caused
the failure.** A message carries user data, so `starts_with` on an error string is an
input-controlled branch.

**A diff that adds or changes a flatten_opt rewrite arm ships a read-only residual predicate
with that arm** - the oracle the residual visitors call to prove the pass complete.

**Never let a residual oracle's gate differ from its arm's gate.** A gate the transform
takes as a parameter is threaded into the oracle too, and the tests that call the oracle
pass the same value; a narrower oracle is a false pass, a wider one a false miss, a
re-spelled parameter default a false pass on every non-default run.

**Never call the transform from an oracle** - calling it aliases the live tree.

**A diff that adds a fold/fuse arm gates it on `no_fast_math` when the rewrite changes
Inf/NaN, rounding, or association, and leaves it ungated when the rewrite is bit-exact per
lane.** An ungated value-changing arm silently changes output under `_flatten_no_fast_math`.

**Never emit a fused call before proving the target module can resolve it - fall back to the
unfused shape on a miss.** Otherwise the pass turns a shader that compiled into an
unresolvable call on a narrower backend.

**Never materialize a `string` per `ExprVar` in a flatten visitor callback.**

**A new store spelling joins `MutCollect` in the same change.** CSE reads a name outside the
mutable set as constant for the whole block, so an uncollected store is a silently shared
subexpression.

**Never drop a statement the lowering cannot predicate - refuse it.** `lower_stmt`'s
fall-through drop is licensed only because every call that survives lowering is pure.

**A diff that reorders `flatten_function`'s passes is a defect - the order is dse ->
copy-prop -> mask-const-prop -> dse -> ssa-rename.** Each pass produces the next one's
input, and a reorder leaves scaffolding in the generated `<name>_flat` twin, which must come
out branchless and call-free.

**Never use `make_visitor` in `lift_expr` or the lowering it drives - use plain recursion.**
Inlining recurses back through the pipeline, and a nested visitor traversal corrupts the
visitor machinery.

**Never splice the same subexpression more than once in a macro - pre-bind it to a local ref
and splice that.** Re-splicing re-evaluates a call once per splice, so a lock/unlock pair
releases a different temporary than it took.

**A diff that adds a `delete` for a `Template`'s substitution tables reads the declaration
first.** An `inscope` local already finalizes, so the added delete is a silent
double-finalize.

**A call to `apply_template` assigns the result back into the expression it passed.** A
root-node substitution is visible only through the return value, so a discarded result
silently keeps the unsubstituted node.

**A new daslib spelling that reads the environment joins the env_registry marker lists in
the same change.** The scanners match daslang source text; a missing spelling makes every
enforcement test pass vacuously.

**A diff that adds or changes an `ast_verify` check names, in `ARCHITECTURE.md` sec. 8, the
C++ `::visit` site that dereferences the slot unguarded.** A check with no such site invents
an invariant the compiler does not hold, and the two passes disagree - a slot infer fills in
is checkable only post-infer.

**A diff that adds or changes an `ast_verify` check repairs the slot it reports, not only
reports it.** A broken slot left in place trips every later pass that walks the same node.

**Never sum a range's start and count as `uint64` before proving both non-negative** - a
signed sum wraps, the bounds test passes, and the walk leaves the array silently.

**Never put a branch a macro-time value can decide into macro-built AST - branch in daslang
and emit only the taken arm.** Nothing folds at macro-application time, so a generated
`if ($v(flag))` keeps its dead arm and type-checks it.

**A diff that adds or changes a buffer-I/O overload returns before taking `addr(buf[0])` on
an empty buffer.** The address is out of bounds and the call sits inside `unsafe`, so an
overload without the guard passes every non-empty test.

**Never decide lvalue-ness in `apply`'s inline path from a ref flag - decide it by node
KIND.** An rvalue stays materialized, because a reference to a temporary dangles.

**Never generate a `let` on `apply`'s inline path without `alwaysSafe`** - that flag, not an
`unsafe` block, licenses the variant access.

**Never reuse a swizzle rewrite's source node in a second output lane without cloning it** -
the first appearance moves it, and skipping the clone gives one node two parents.

**A diff that changes how the RST label or topic key is computed makes the same change in
every place `rst.das` computes it - the stub pass (`generate_module_stubs`) and the
documenting pass (`documents`) - keeping them byte-for-byte equal.** When the two diverge, the page
prints a bare signature and the symbol re-stubs.

**A diff that adds a numeric value form to the toml lexer routes it through `rewind_to_bare`
on a bare-key character.** Without the rewind, a bare key that starts like a number lexes as
a value.

**A structure macro that instances inherit through `[|> name]` is idempotent: it appends a
marker annotation to the instance it finishes, and returns unchanged when it sees that marker.**
A chain of template ancestors delivers one copy of the annotation per level, so a macro without
the marker runs a second time over a structure it already rewrote.

**A diff that adds or renames a public intrinsic in `aarch64_neon.das`, `x64_avx.das` or
`f16_cvt.das` changes the matching row of the lookup tables in
`modules/dasLLVM/daslib/llvm_jit_intrin.das` (repo root) in the same change.** The JIT recognizes
these calls by function name alone, so a missing row runs the portable fallback on every target
with every test green - the `tests/jit_tests` twins compare hardware against a reference body and
catch a wrong emitter, never a missing one.

**A function in `x64_avx.das` that exists to reach a wider or newer CPU tier than another
function in that file computes its result by calling that function - never by a hand-written
scalar loop.** A box whose CPU lacks the tier then runs the recognized narrower path instead of
scalar code.
