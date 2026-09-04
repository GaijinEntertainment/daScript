# Const-init / extra-module errors / literal promotion follow-ups

Ledger for the findings of the review round on PR #3930 (variant/tuple `:=` clone-init,
readable extra-module diagnostics, literal-only binary-operator promotion) that the PR did not
act on. Each entry names the site, the defect, and the fix the round proposed. None is
scheduled; the next arc touching the area takes what it can.

## Compiler

### parse_file tolerates a failing extra module

`parseDaScriptWithPrerequisits` (`src/ast/ast_parse.cpp`) still ignores the extra-module loop's
return, so `parse_file` returns success with that module dropped from `req` and stale records in
the accumulators, while `compile_file` on the same input reports the failure. Consumers:
`utils/gen1-to-gen2/convert.das`, `utils/internal/lineinfo-audit/main.das`. Fix: route the
failure through `reportPrerequisitesErrors` there too, after checking that the converter can
live with a hard failure on a broken extra module.

### A non-clonable variant or tuple arm reports inside builtin.das

`convertCloneFieldToMove` (`src/ast/ast_infer_type_make.cpp`) wraps the value in
`builtin::clone_to_move`, so `type ... can't be cloned` lands at `daslib/builtin.das`'s
`clone_dest := clone_src` with the user's site only in the `instanced from` trailer; the
struct path keeps the make site because the where-block spells `self.field := value`. Fix:
check `canClone` against the arm type before the rewrite and report at `decl->at`.

### Prerequisite-error anchor and caret

`reportPrerequisitesErrors`: the anchor is chosen by record category (missing, circular,
not-allowed, case-conflict), not by discovery order, so with several failing extra modules the
caret can land on a later file than the first message names. `addRequirements` records carry
the resolved module name, so the caret under-spans a path-spelled require
(`require daslib/strings_boost` underlines thirteen characters). Fix: a discovery sequence on
the records, and the source spelling on native-dependency records.

### The extra-module walk is logged nowhere

The extra-module walk passes a null log in `compileDaScript`, so `-log` shows no walk for
extra modules while `parseDaScriptWithPrerequisits` still logs its own. The readable records
replace the graph for the failure case; the `-log` visibility is what went with it. Fix: gate the
extra-module walk log on the same `options log_require` the main file uses.

### utils/lint classifies extra-module failures as an environment skip

`utils/lint/main.das` treats `error[20605]` as "missing prerequisite - install daspkg deps"
and skips the file. Extra-module failures now carry 20605 instead of the old 20901, so a file
whose registered extra module has a broken require drops out of the lint's failure set. Fix:
distinguish a blocked or broken require from a missing package in the skip heuristic.

### Stale prebuilt modules read padding as the new ExprConst flag

`isConstLiteral` lands in `ExprConst`'s alignment padding: `sizeof` is unchanged, so
`das_abi_vintage()` (folds `sizeof(Expression)`, not `sizeof(ExprConst)`) and the
version-derived `DAS_BUILD_ID` both accept a module built against the old header, whose
constructors never write that byte. Fix: fold a layout hash of the AST node structs into the
vintage stamp, or bump the version with any AST node layout change.

### The operator mismatch hint offers the lossy cast first

`src/ast/ast_infer_type_report.cpp` prints both directions for a `uint * int` mismatch, and the
second line (`int(x) * 8`, `int(f) * 2`) narrows the wide operand. Pre-existing; the literal
rule makes it fire on `typeinfo` operands more often. Fix: print only the widening cast, or
print it first.

### Promotion is not gated for the six non-int literal kinds

The parser marks all eight numeric literal kinds, but `tryPromoteConstInt` reads the flag only
for `tInt` and `tUInt` sources, so the marks on int64, uint64, uint8, float, float16 and double
literals are inert and untestable. Either drop the marks from those six actions or give the
flag a reader for them.

### No CI lane runs -module-cache

The serializer stream (version 200) is exercised only by local `-module-cache` runs and the
`--ser`/`--deser` sweep; nothing in `.github/workflows` builds and re-reads a module cache. Fix:
a cold-then-warm `-module-cache` pair in `extended_checks` over `tests/module_cache`.

## daslib and tests

### unroll's index mark has no test

`daslib/unroll.das` returns the plain loop under `is_in_lint_check()`, so any body that only
type-checks unrolled (`acc * 2l + i`) fails the lint rail; `tests/language/unroll_literal.das`
was dropped for that reason. Fix: a lint-mode escape for one test file, or an unroll test that
asserts the mark through `daslib/ast` on the substituted node instead of compiling it.

### flatten's typed_one duplicates zero_const_of's table

`daslib/flatten.das` `typed_one` and `daslib/flatten_opt_common.das` `zero_const_of` are the
same per-`baseType` constant table for `1` and `0`. Fix: a `one_const_of` beside
`zero_const_of` and `typed_one` deleted.

### Fixture resolvers carry a dead branch

`if (length(rs) == 0)` after `split_by_chars` is unreachable (the split always yields one
element) in all four `tests/module_tests/_modules/**/*.das_project` resolvers. One sweep.

### Untested branches the tdd audit named

`visitMakeStructureField`'s fixed-array unwrap for a `:=` make (`tests/language/clone.das`
covers `fixed_array(TBar(...))`, which builds two makes, not one dim make); the unknown-field
arm of a `:=` variant or tuple make; every gen1 (`ds_parser.ypp`) literal arm, since no CI lane
parses under `-v1syntax`.

## Checklists and gates

### REVIEW.das gate candidates the dragons proposed

- `src/builtin/REVIEW.das`: hash the ordered `ser <<` operand sequence of
  `module_builtin_ast_serialize.cpp` and compare it with a value recorded beside `getVersion()`;
  a layout change that leaves the pair unmoved is the finding.
- `include/daScript/simulate/REVIEW.das` (none exists yet): every data member of each
  `debug_info.h` struct the pin file covers has an `offsetof` line in
  `tests-cpp/small/test_debug_info_layout_pin.cpp`.
- A CI assertion, not a gate: a PR that touches the pin file names all four `debug_info.h`
  consumers in its body.

### Self-review findings on the audited checklists

- `src/parser/REVIEW.md`: no gen1/gen2 parity duty is stated, and the architecture-doc slot
  points at `skills/internal/cpp_codebase_notes.md`, which carries one table row and no charter.
  (A regeneration gate was ruled unnecessary: cmake regenerates.)
- `src/builtin/REVIEW.md`: rule 1 restates what `review_nttp.das` enforces and could shrink to
  the "weakening the gate is a defect" residue, but `REVIEW.das` parses the Inline-module
  sentence out of it; rules 2 and 3 split one property (the require list names every module).
- `include/daScript/simulate/REVIEW.md`: the hot-path rule fuses three checks in nine lines.
- `daslib/REVIEW.md`: "never drop a statement the lowering cannot predicate" names one failure
  mechanism where the property is "never emit or drop a wrong twin"; the collapse-pair rule
  enumerates today's partitions instead of stating the property; the pair rule and its
  "only where no name, shape, or test can carry the fact" neighbor overlap on a name-carried
  pair (ruled good enough).

## Tooling observations

- The Windows lint rails skip `modules/dasSMT` (no `libz3.dll`), so a one-line edit there
  drags linux-only findings into CI; the pre-push gate cannot see them.
- A case-conflict fixture needs both casings as real files (a subfolder), or Linux reports
  "file not found".
- `preflight --list-gates` printed nothing in this session.
- One JIT codegen stall in `tests/strudel/test_hrtf_pos.das` during a full sweep (a zero-byte
  `.o`, 1200 s timeout); the file passes alone and in the folder rerun. Observed once.
