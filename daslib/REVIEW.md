# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_CAPI.md`, `ARCHITECTURE_CURSOR.md`, `ARCHITECTURE_EMIT.md`,
`ARCHITECTURE_JOBQUE.md`, `ARCHITECTURE_LINQ.md`, `ARCHITECTURE_LINQ_FOLD.md`,
`ARCHITECTURE_LINQ_FOLD_SOURCES.md`, `ARCHITECTURE_LINT.md`, `ARCHITECTURE_MEMORY.md`,
`ARCHITECTURE_SHADER.md`, `ARCHITECTURE_SQL_LINQ.md`, `ARCHITECTURE_SYNTAX.md`. A diff touching the
linq family - `linq*.das`, `sql_*.das` - applies `REVIEW_LINQ.md` too. A diff that changes
compile-time AST-building code - a class deriving an `Ast*Annotation` or `Ast*Macro`, an
`AstVisitor` a macro or an annotation runs at compile time, a `[macro_function]`, or any function
that calls `qmacro*`, `apply_template`, `macro_error`, or `macro_sticky_error` - applies
`REVIEW_MACROS.md` too. A diff touching `daspkg.das` - the functions a `.das_package` manifest body
calls - applies `utils/daspkg/REVIEW.md` (repo root) too. A diff touching `shader_lingua_franca.das`
or `shader_emit_common.das` - the declarations and helpers the shader emitters share - applies
`modules/REVIEW_SHADER_EMITTERS.md` (repo root) too. A diff touching a file listed in `RULE_MODULES`
of `utils/lint/REVIEW.das` (repo root) applies `utils/lint/REVIEW.md` (repo root) too.

**A diff that changes a `.das` here that an `AOT_*_MODULE_FILES` variable of
`tests/aot/CMakeLists.txt` (repo root) picks up, and that is not marked `options no_aot`, states in
the PR body that the full `test_aot` lane (`preflight --full`, or a manual dispatch of
`.github/workflows/nightly.yml` (repo root) on the branch) ran green on the diff's head commit.**
Per-PR CI compiles only the language subset.

**A diff that changes the body of `perf_warning` (`perf_lint.das`), `style_warning`
(`style_lint.das`), or `lint_error` (`lint.das`) runs the suppression check before the body records
or reports the finding.** A finding that a `// nolint` suppresses, once recorded, holds its (rule,
location) entry and silences another rule's finding at the same location.

**A diff that changes `report_key` (`perf_lint.das`, `style_lint.das`) - the key that drops a
repeated finding - keeps the rule code in it.** A key without the code collides across rules.

**Never narrow PERF026-028's `// nolint` check to the line where the reported call chain ends - a
`// nolint` on any call site along the chain suppresses the finding.** The chain often ends inside
daslib, while the line the author can change is a call site partway along it.

**A diff that moves an `in_closure` / `in_deferred` guard to a different statement position inside a
lint visitor method is a defect.** The guard's position decides which rules apply inside a lambda;
the per-rule policy is in `ARCHITECTURE_LINT.md#perf-lint`.

**Never move PERF023's `add_ptr_ref` splice-depth tracking (`perf_lint.das`) behind the `in_closure`
gate.** The rules block `apply_qrules` runs is itself a closure, so a tracker behind the gate never
sees the splice.

**A lint warning whose reported location is anywhere but the code its fix rewrites is a defect.** A
remedy that deletes a statement reports the statement; a remedy that edits one variable's
initializer reports the variable.

**Never exit a construct's visitor on a path that skips resetting or restoring the visitor state
scoped to that construct.** State an early return leaves set poisons the next construct's visit.

**Visitor state that a construct's exit restores to its value at the construct's entry is saved on a
stack - a diff that saves it in one scalar field is a defect.** A saved value kept in a scalar
survives into the sibling construct's exit path and unbalances it.

**A diff that adds or changes a daslib fact (code or a `//!` contract), or what a C++ name
`aot_cpp.das` writes into generated code does or returns, that code at a second site must agree
with records the pair in an `ARCHITECTURE*.md` of this folder, naming both sides by symbol name,
when nothing fails on a mismatch: no lint, no compile error, no default-suite test (what `dastest`
runs over `tests/` (repo root) with no flags, an environment-gated or nightly-only test
excluded).** The second site can sit inside or outside this repository; a statement about a family
of symbols is not a pair.

**A diff that records a pair puts it in the section that holds its daslib file (for a C++ name, the
emitter file that writes it), or in a new section of `ARCHITECTURE.md` or a companion when no
section holds it; a pair whose two sides are spellings of one syntax goes in
`ARCHITECTURE_SYNTAX.md`.**

**When a diff changes the daslib side of a recorded pair so the two no longer match, it changes the
other side in the same diff; when the other side sits outside this repository, the PR body names it
and the change it needs.**

**A diff that renames or deletes a daslib name an `ARCHITECTURE*.md` in this folder spells, or
changes what that name names so that the entry's claim stops holding, changes that entry in the same
change.** The doc is read as the current tree, and a name that no longer resolves or a claim that no
longer holds is trusted until someone reads the code.

**A diff adds an architecture-doc entry only for a fact that no lint, compile error, or
default-suite test (what `dastest` runs over `tests/` (repo root) with no flags, an
environment-gated or nightly-only test excluded) reports when it goes false, and that the code's own
names do not make self-evident.**

**A diff that changes `build_lint_macro_disabled` (`lint_config.das`) keeps its steps in this order,
each overriding the ones before it: the default-off seeding, the repo `.lint_config` `off`
directives, its `on` directives, the `DAS_LINT_DISABLE` environment variable, and last the file's
`options _nolint` / `_lint`.** With the environment after the `on` directives, one run's
`DAS_LINT_DISABLE` silences a rule the repo forces on.

**A diff that changes `options _enable_default_off_rules` keeps it skipping BOTH the default seeding
and the repo `off` directives.** Repo policy must not silence the rule a fixture exists to exercise.

**A daslib module that emits a lint rule id joins `RULE_MODULES` in `utils/lint/REVIEW.das` in the
same change.** An unlisted module's ids are never scanned, so its fixture-and-rst check never runs.

**A diff that deletes a rule id's last emitting spelling from a rule module also removes that id's
fixture and its `doc/source/reference/language/lint.rst` (repo root) section, in the same change** -
the gate checks only ids it can still find, so a dead id's fixture and doc section stay behind
unreported.

**Never record a LINT010 store in `preVisitExprVar` - record it in the POST-visit of
`ExprCopy`/`ExprClone`/`ExprMove`.** The LHS's variable fires before the RHS is walked, so an early
record lets the RHS's own read clear the store it just recorded.

**Never match a lint rule's callee name against the instantiated function's name - match the name of
the generic it was instantiated from.** Instantiation mangles the name and drops type-only
arguments.

**A lint rule that fires on the source of the module that implements the idiom it suggests is a
defect - skip that module's own source in the rule.**

**A diff that adds or changes a collapse check - a lint rule that suggests replacing a run of
statements with one expression - gates the suggestion on each of receiver type, arity, cloneability
and duplicate constant keys that can make the suggested expression fail to compile or behave
differently.**

**A diff that adds or changes the rewrite a lint message suggests adds that rewrite to the rule's
fixture (under `utils/lint/tests/` or `tests/lint/`, repo root) as a good shape that compiles and
does not fire.**

**A diff that makes two collapse checks (lint rules suggesting one expression for a run of
statements) fire on one shape is a defect - gate one of them off that shape.**

**A lint message starts with its rule code, and the message's first colon ends that code
(`STYLE031: ...`).** Suppression, disable lists, and dedup all parse the code from the message, so a
message that leads with anything else makes the rule unsuppressable and breaks its dedup.

**A diff that adds or changes a `canVisit*` override in style_lint keeps the unsafe-frame count
balanced across the subtree the override skips.** The walk pushes one frame per expression and pops
it, so a skipped subtree unbalances the count; each entry point panics when the count ends
unbalanced.

**Never cache `blk.list` index state across style_lint visitor callbacks.** The per-statement
callback carries no index and a shared stack would have to push/pop with nested blocks; block
statement lists are short, so the rescan is the cheap form.

**A diff that changes an arity cap or the overload set it mirrors - `MAX_CONCAT_ARITY` <-> linq's
`concat` variadics, `MAX_VARIADIC_PUSH_ARITY` <-> builtin `push_from`/`push_clone_from` - changes
both sides in the same change.** Growing an overload set without the cap is a silently missed
finding; raising a cap without the overloads is a suggestion that does not compile.

**A diff that adds or changes an emit entry point - a function that runs the emit visitor (`CppAot`
or any subclass of it) and then writes the generated C++ to a file - keeps the error check ahead of
that file write.** The error check reads the program's failed state: `program.failed`, both
`program.flags.macroException` and `program.flags.failToCompile`, or `log_aot_emit_errors`, which
reads those two flags. A codegen exception mid-visit leaves partial C++.

**Never gate a visitor override in `CppAot` or any subclass of it on the program's failed state -
`program.failed`, `program.flags.macroException`, `program.flags.failToCompile`, or
`log_aot_emit_errors` - the function that runs the visitor owns that check.** An override that
returns early on the error state emits truncated C++ that the caller still writes out as complete.

**Never report an unreachable emit state with `panic` - write `#error` into the output.**
`runMacroFunction` catches a panic and drops it, so the emitter never reports through it.

**A diff that changes how a struct's C++ name is built - the module namespace (`aotModuleName`, or
`aotModuleNameFromString`) plus the `_S` suffix - changes every `aot_cpp.das` site that builds it,
in the same change.** One site changed alone writes `offsetof`s that name a struct declared under a
different name.

**A diff that adds or changes a function that emits a struct or enum C++ name - anything reaching
`aotStructName` / `aotEnumName`, a `CppAot` subclass and `ArgsConverter` alike - keeps
`buildStructEnumCollisions` running before it, directly or in a helper it calls.**
`buildStructEnumCollisions` fills the collision table that decides when a name gets its collision
suffix, and a run that skips it spells structs differently from a run that filled it.

**Never index a UTF-8 byte-class table with a raw `ch` - index through `uint(uint8(ch))`.**
`for (ch in string)` yields a SIGNED byte under JIT, so a raw index reads out of bounds for every
byte >= 0x80.

**A diff that adds or changes a parser result that borrows a view into a tree the parser allocated
names the field that owns the tree and ships the function the caller runs to free it.** The tree
must outlive the caller's read, and daslang finalizes neither a raw pointer field nor a local
container at scope exit, so a borrowed view with no named owner leaks the whole document per call.

**Never use the throwing, one-argument string forms of `int64`, `uint64` or `double` in a lexer or
decoder reachable from a file, a socket, or a model file - use the overload that returns
`ConversionResult` and the consumed count, or the non-throwing `to_*` twin after validating the
input.**

**A lexer or decoder reachable from a file, a socket, or a model file reports a conversion failure
through its parser's error channel - never a panic.**

**Never branch a parser on the text of its own diagnostics - branch on the token that caused the
failure.** A message carries user data, so `starts_with` on an error string is an input-controlled
branch.

**A diff that adds or changes a flatten_opt rewrite arm ships, with that arm, a residual oracle - a
read-only check that no shape the arm rewrites is left in the tree after the pass, reached from
`flatten_opt_residuals` or `flatten_fold_residuals` (`flatten.das`), which prove the pass
complete.** The oracle never calls the transform: that would rewrite the tree it is checking.

**A residual oracle takes its arm's gate - the condition that decides whether the arm fires -
unchanged: a gate the transform receives as a parameter is passed to the oracle, never re-spelled as
a default, and a test that calls the oracle passes the value the transform ran with.** A narrower
gate passes a pass that left work undone; a wider one fails a pass that finished.

**A diff that adds a rewrite arm to a flatten pass (`flatten_opt_*.das`) gates it on the pass's
`no_fast_math` parameter when the rewrite changes Inf/NaN, rounding, or association, and leaves it
ungated when the rewrite is bit-exact per lane.** An ungated value-changing arm silently changes
output under `_flatten_no_fast_math`.

**Never emit a fused call - one call that replaces several operations - unless its function resolves
in the program being flattened; emit the unfused operations when it does not.** Otherwise the pass
turns a shader that compiled into an unresolvable call on a narrower backend.

**Never build a `string` from an `ExprVar`'s name in a flatten visitor callback - use the
`das_string` name or the variable pointer in place.**

**A diff that teaches the flatten pipeline a new expression form that writes a variable adds that
form to `MutCollect` (`flatten_opt_common.das`) in the same change.** CSE reads a name outside the
mutable set as constant for the whole block, so an uncollected store is a silently shared
subexpression.

**Never drop a statement the lowering cannot rewrite into lane-masked form - refuse the function
with an error.** `lower_stmt` drops whatever falls through its arms, which is safe only because
every call left after lowering is pure.

**A diff that reorders `flatten_function`'s passes is a defect - the order is `dse_pass` ->
`copy_prop_pass` -> `mask_const_prop_pass` -> `dse_pass` -> `ssa_rename_pass`.** Each pass produces
the next one's input, and a reorder leaves scaffolding in the generated `<name>_flat` twin.

**Never use `make_visitor` in `lift_expr` or the lowering it drives - use plain recursion.**
Inlining recurses back through the pipeline, and a nested visitor traversal corrupts the visitor
machinery.

**A diff that adds a daslib function that reads an environment variable adds its call text to
`ENV_READ_MARKERS` (`env_registry.das`) in the same change.** The scanners match daslang source
text; a missing marker makes every enforcement test pass vacuously.

**A diff that adds or changes an `ast_verify` check names, in `ARCHITECTURE.md#ast-verify`, the C++
`::visit` site that dereferences the slot unguarded.** A check with no such site invents an
invariant the compiler does not hold; a slot that infer fills in is checkable only after infer.

**A diff that adds or changes an `ast_verify` check repairs the slot it reports, not only reports
it.** A broken slot left in place trips every later pass that walks the same node.

**Never sum a range's start and count as `uint64` before proving both non-negative** - a signed sum
wraps, the bounds test passes, and the walk leaves the array silently.

**A diff that adds a function to `cross_context.das` wrapping `invoke_in_context` - taking the
`unsafe` and the `addr` off the caller - is a defect; the caller writes the call itself.** The
wrapper's signature checks nothing the call does not: the target is found by name and argument
count, and a pointer crosses into a context that holds its lock for the whole run, so the `unsafe`
belongs at the site that hands the pointer over.

**A diff that adds or changes a function that takes `addr(buf[0])` of an array or string argument
returns before taking it on an empty buffer.** The address is out of bounds and the call sits inside
`unsafe`, so an overload without the guard passes every non-empty test.

**Never decide lvalue-ness in `apply`'s inline path from a ref flag - decide it by node KIND.** An
rvalue stays materialized, because a reference to a temporary dangles.

**Never generate a `let` on `apply`'s inline path without `alwaysSafe`** - that flag, not an
`unsafe` block, licenses the variant access.

**Never reuse a swizzle rewrite's source node in a second output lane without cloning it** - the
first appearance moves it, and skipping the clone gives one node two parents.

**A diff that adds or changes a site in `rst.das` that spells a symbol's RST label (the `.. _name:`
target) or topic key (the `|detail/...|` / `|handmade/...|` substitution name) keeps every site in
`rst.das` that spells the label or key of that same symbol byte-for-byte equal.** When two such
sites differ, the page prints a bare signature and the generator writes a fresh empty stub for the
symbol.

**A diff that changes the category or the name a topic key carries - in `rst.das`'s `topic()` sites
or in `rst_comment.das`'s `write_to_detail` names - makes the same change on the other side, in the
same change.** `rst_comment.das` writes a detail file under one key and `rst.das`'s page reads it
under the other; a mismatch prints a bare signature.

**A diff that changes what `rst.das` or `rst_comment.das` places on a page states, in the PR body,
that every name the generator newly places names a symbol, module or file that exists in the
repository at merge, and where that was checked** - a name the generator lists is a name a reader
will type.

**A diff that adds a numeric value form to the toml lexer routes it through `rewind_to_bare` on a
bare-key character.** Without the rewind, a bare key that starts like a number lexes as a value.

**A diff that adds or renames a public intrinsic in `aarch64_neon.das`, `x64_avx.das` or
`f16_cvt.das` changes its row in the lookup table of `modules/dasLLVM/daslib/llvm_jit_intrin.das`
(repo root) that is gated on the CPU feature the instruction needs, in the same change.** The JIT
matches these calls by function name alone: a missing row silently runs the portable fallback, and a
row gated on a weaker feature emits an instruction LLVM cannot select for a CPU without that
feature.

**A diff that adds or changes a function in `aarch64_neon.das`, `x64_avx.das` or `f16_cvt.das` that
computes the same math as another function in that file on a wider or newer instruction set makes
its fallback - the body that runs when the CPU lacks that set - call that function, never a
hand-written scalar loop.** A CPU without the newer set then runs the recognized narrower path
instead of scalar code.
