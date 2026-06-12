# Quote lowering: AOT + JIT support for `quote` / `qmacro`

**Goal:** `ExprQuote` (and therefore the whole qmacro family) compiles under AOT and JIT.
Mechanism: revive the existing infer-time lowering in `daslib/quote.das` that replaces
`quote(tree)` with generated AST-reconstruction code (`ExprMakeStruct`/`ExprAscend`/…),
which is plain das code and compiles in all three tiers.

## Decisions (settled)

- **Revive, don't rewrite.** `d38cd728f` ("enable ast quote in aot", Jul 2025) is the right
  design: reflection-driven converter walking the quoted tree via `BasicStructureAnnotation`
  field enumeration + `walk_and_convert` for leaf data. No per-node emitter to maintain.
- **Stays in `daslib/quote.das`.**
- **Wiring = option (a):** static `require daslib/quote` from `daslib/templates_boost.das`.
  Activation stays cop-driven via the existing `policies.aot_macros` check
  (quote.das:410) — the pass is inert when the policy is off; `module quote shared` makes
  the inert require near-free. (Rejected: root-level `addExtraModule` injection — extras l
  and on the program root only, but QuotePass must fire inside each macro module's own
  compilation, where `[infer_macro]` visibility follows that module's require chain.)
- **Tests are mandatory before anything ships.** Basic suite first; lifting `no_aot`/jit
  gates on existing tests is *extra* coverage afterwards, not a substitute.

## Current-state map (verified 2026-06-11)

- `ExprQuote` infers to `Expression?` handle; quoted subtree is never inferred
  (`canVisitQuoteSubexpression` defaults false). Infer marks the enclosing function
  `noAot` unless `policies.aot_macros` (src/ast/ast_infer_type.cpp:1440).
- Interpreter: `SimNode_AstGetExpression` holds a raw `Expression*` and returns
  `expr->clone()` per evaluation (src/ast/ast_simulate.cpp:1280).
- qmacro expands to `apply_qmacro(quote(tree), rules-block)` (templates_boost.das:1179),
  so lowering ExprQuote covers qmacro; the rules block is ordinary code.
- `daslib/quote.das`: `QuotePass` (`[infer_macro]` AstPassMacro) + `QuoteConverter`
  replaces each ExprQuote via `convert_quote_to_expression`. Managed entities resolve
  by name at runtime (`get_module` / `module_find_annotation` / `find_call_macro` /
  `find_unique_function_ptr` / `module_find_structure` / `clone_file_info`).
  **Orphaned:** nothing requires it (docs tooling only), zero tests.
- Lossiness blacklist (quote.das:151): `Function.annotations`, `EnumEntry.value/.at`,
  `Enumeration.list`, `CaptureEntry.at`, `MakeFieldDecl.at`, `AnnotationArgument.at`;
  `AnnotationArgumentInitData` carries only string/bool/int/float; FileInfo rebuilt by
  name+tabSize (loses identity of `LineInfo.fileInfo`).
- AOT: `daslib/aot_cpp.das` has **no** ExprQuote handling — with `-aot-macros` set but
  the pass not required, codegen emits garbage with no diagnostic.
- JIT: no `visitExprQuote` in `LlvmJitVisitor` → parent `getE` → `failed_E "unresolved
  expression"` (llvm_jit.das:7017) → `g_errors` → `panic("Internal jit error…")`
  (llvm_jit_run.das:295). Loud but misleading; all-or-nothing.
- JIT of macro contexts is separately disabled: `jit_llvm` simulate-macro skips when
  `is_compiling_macros()` (llvm_macro.das:49).
- CLI: `daslang -aot … -aot-macros` sets `aot_macros` + `export_all` + 1MB stack
  (utils/daScript/main.cpp:65).
- Policy-option promotion precedent: `policies.threadlock_context |= options.getBoolOption(…)`
  (src/ast/ast_parse.cpp:1416).
- Blast radius: every in-tree quote user requires templates_boost (defer, decs_boost,
  lpipe, static_let, typemacro_boost, interfaces, linq_fold_common, type_traits, …).
  41 test files use quote/qmacro (ast_match suite, linq_fold, 13× dasPEG, flatten/no_aot,
  dasSQLITE/test_07_macros, fake_numeric, …), heavily overlapping the `no_aot`-gated set.

## Phase 0 — empirical baseline (in progress)

Worktree build (fresh master; both existing binaries are skewed vs today's
annotation-info merge), then:

- [x] Interpreted probes: raw `quote`, `qmacro` incl. `$c` splice — green
      (`SimNode_AstGetExpression` path intact).
- [x] Lowered probes (`options aot_macros`, interpreted): green after the bitrot fixes
      below — consts, op2, calls, strings, `$c` splice, `qmacro_block`, `qmacro_type`,
      make-struct, lambda. Probes in `probe_quote/` (become `tests/quote/` in Phase 3).
- [x] A/B: identical `describe_expression` output lowered vs unlowered (6 shapes).
- [x] `daslang -aot -aot-macros`: emits real reconstruction C++
      (`das_ascend_handle<ExprOp2*>::make`, `clone_line_info`/`clone_file_info`), exit 0.
- [x] Reach probe: root-level `require daslib/quote` does NOT lower quotes inside a
      required module (pass-macro visibility = the module's own require chain) — wiring
      (b) rejected on evidence; with the templates_boost require the same module lowers.
- [x] Regression smoke (unlowered path + new require wiring): ast_match 380/380,
      template 10/10, linq 1971/1971.

### Bitrot fixed during Phase 0

1. **`apply_to_vec` gc_node spellings** (src/builtin/module_builtin_ast.cpp): dispatched
   on smart_ptr-era `describe()` strings (`"ast::MakeStruct*"` — dead since the ast-module
   removal). Fixed: one generic `tPointer` → `vector<void*>` branch (post-gc_node every
   AST node vector is a plain raw-pointer vector); `"ast_core::MakeFieldDecl?"` keeps the
   MakeStruct multiple-inheritance base-adjust special case.
2. **`convert_vec_expr` retarget stomp** (daslib/quote.das): `dst_tp != src_tp` was a
   POINTER compare — always true after `clone_type` — so for pointer-element vectors the
   code reinterpreted an `ExprAscend` as `ExprMakeStruct` and wrote `makeType` through the
   wrong layout (memory stomp → gc_collect AV; survived 2025 only by layout luck). Fixed:
   semantic compare (`describe(dst) != describe(src)`) + `arg_expr is ExprMakeStruct` guard.
3. **`cvt_to_mks` move_new** (daslib/quote.das): smart_ptr-era `move_new((*res)[i]) <| arg`
   on what is now a raw-pointer slot. Fixed: plain assignment.

## Phase 1 — plumbing + loud diagnostics (DONE 2026-06-11)

- [x] `require daslib/quote public` in templates_boost (public: the generated
      reconstruction calls — `cvt_to_mks`, `clone_line_info`, … — must resolve in the
      qmacro-using module's scope).
- [x] `options aot_macros` per-module trigger: the option name is already valid
      (CodeOfPolicies fields are auto-registered option names via
      `getCodeOfPolicyOptions()`). QuotePass gate reads
      `policies.aot_macros || prog options aot_macros`; the infer-time noAot-skip
      (ast_infer_type.cpp) got the identical option clause — no parseDaScript
      promotion needed.
- [x] QuotePass gate stays `aot_macros`-only (JIT extension deferred to Phase 5).
- [x] `aot_cpp.das` diagnostics — NOT a panic: a panic inside a make_visitor-driven
      visitor is swallowed by `runMacroFunction`/`runWithCatch` (verified — error filed
      on a program nobody reads post-compile, tool exits 0). Instead: `NoAotMarker`
      gets `preVisitExprQuote` → marks the function `noAot` + `to_log(LOG_ERROR, …)`
      naming file:line, cause, and fix (silent when the function is already noAot, i.e.
      the normal no-aot_macros flow). Function falls back to interpreter;
      `fail_on_lack_of_aot_export` escalates for exports. CppAot keeps a backstop
      `visitExprQuote` that writes `#error` into the generated C++ (deterministic,
      unlike panic). Verified on a negative probe: clean LOG_ERROR, broken emission gone.
- [x] `llvm_jit.das`: `preVisitExprQuote` → `unsupported(expr, "quote( ) is not jit-able
      without aot_macros lowering (daslib/quote)")`, modeled on the try-catch case.
      Lints clean; behavioral check needs an LLVM-enabled build (Phase 5).
- [x] Formatter verified + lint clean on all four changed das files (llvm_jit.das lint
      needs `lib/LLVM.dll` present — dasbind resolves `getDasRoot()/lib/<dll>`).

## Phase 2 — make the lowering correct (IN PROGRESS)

### Done so far (2026-06-11)

- **Suite-wide forcing flags**: `daslang -aot-macros <script>` (normal run, main.cpp) and
  `dastest --aot-macros` (dastest_clargs/suite.das → `cop.aot_macros`, + 1MB stack like
  the `-aot -aot-macros` flow). These are the audit hammer.
- **Function-per-quote**: QuoteConverter now wraps each lowered construction in a
  generated `` `quote`lowered`<n> `` private function and replaces the quote with a call.
  Without this, every lowered qmacro callsite inflates its CALLER's stack frame (sum of
  construction temporaries), which is fatal in recursive macros — flatten's `lower_stmt`
  overflowed any reasonable stack. With it, the big frame is a single non-recursive leaf.
- **FULL tests/ tree, lowering forced: 10103 tests, 10097 passed, 0 failed, 6 skipped —
  byte-identical verdict to the unlowered baseline.** (92.6s vs 57.7s wall — the forced
  mode pays QuotePass + giant-tree inference; fine for a debug/audit mode.)
- **resolve_file_info**: per-file interned dummy FileInfo (module-global table in quote.das,
  per macro context) replaces a fresh allocation per LineInfo per evaluation. The
  live-FileInfo upgrade is parked: AOT emits calls with null LineInfoArg and Program does
  not retain its FileAccess, so there is no reliable runtime handle to the live file.
- **Anonymous-module entities**: quoted `type<LocalEnum>` / `type<LocalStruct>` (entity
  lives in the program's unnamed main module) generated `get_module("")` → null-deref at
  reconstruction. Fixed: `Enumeration` joined the managed by-name set (new C++ builtin
  `module_find_enumeration`), and TypeDecls referencing anonymous-module enums/structs
  reconstruct as `make_alias_type_decl("<name>")` — the exact alias shape the parser
  yields for unresolved names, so splice-site re-infer resolves them.

### Phase 2 addendum (later same day)

- Extended corpus (probe_quote/ab2_*): enum consts, escaped + non-ASCII strings,
  `finally` blocks, capture-clause lambdas, table literals, tuple literals, generators,
  double arithmetic — all A/B-identical; `daslang -aot-macros` (CLI) ==
  `options aot_macros` (module) == unlowered reference.
- Final full-tree lowered re-run after all Phase 2 changes: SUCCESS, identical verdict.

### Blacklist audit — remaining suspects

Known suspects, each needs a verdict (legit-to-drop because infer rebuilds it, e.g.
`ExprVar.pBlock` / `ExprReturn._block` backlinks — vs lossy-and-matters):

- [ ] `ExprBlock` annotations + `blockFlags`, `finally` lists.
- [ ] Capture lists (`CaptureEntryInitData` drops `at`).
- [ ] `Enumeration.list` ("too huge in TypeDecl") — does `qmacro(MyEnum.Value)` survive?
- [ ] `FileInfo`: replace per-call `clone_file_info` (fresh dummy per evaluation,
      module_builtin_ast.cpp:1038) with a `resolve_file_info(name, tabSize)` builtin:
      if compiling → look up `name` in the compiling program's `FileAccess` (which
      caches FileInfo by name, so the dominant macro case returns the EXACT live
      FileInfo — macro-module source is always parsed even under AOT; AOT only
      carries the name string); else → per-name interned dummy (stable identity,
      no source). Needs one small C++ binding; `Program.access` is not currently
      exposed to das, and that's fine — resolve inside the builtin.
- [ ] `AnnotationArgumentInitData`: missing double / multi-value coverage?
- [ ] `Function.annotations` blacklist entry — reachable at all in quoted trees, or dead?
- [ ] String escapes / non-ASCII round-trip through `ExprConstString` reconstruction.

Acceptance bar: lowered tree ≡ `expr->clone()` for every construct the basic suite
covers (modulo agreed-legit blacklist entries, each documented in quote.das).

## Phase 3 — basic tests (`tests/quote/`) — LANDED 2026-06-11 (20/20)

Implemented as twin fixture modules with identical bodies — `_quote_shapes.das`
(SimNode path) vs `_quote_shapes_lowered.das` (`options aot_macros`) — plus shared
entities in `_quote_entities.das` (one named module so describes are identically
qualified). `test_quote_lowering.das` asserts per-shape describe equality;
`test_quote_lowered_main.das` covers lowered quotes in the anonymous main module
(alias fallback) and splice behavior. Registered in tests/aot/CMakeLists.txt
(test_aot_quote + test_aot_quote_modules). Original checklist:

Self-contained via `options aot_macros` (Phase 1). For each category, A/B: expand the
same qmacro with lowering on vs off, compare `describe_expression` output, plus a
behavioral check that the spliced result still compiles/runs via `apply_template`:

- consts (int/uint/float/double/string incl. escapes, enum refs, bitfields)
- op1/op2/op3, field chains, index, calls incl. named args + pipe
- blocks: statements, `finally`, block annotations, nested blocks
- make-struct / make-array / make-tuple / make-variant (move/clone semantics flags)
- lambdas + capture modes, generators, `qmacro_block` / `qmacro_expr` /
  `qmacro_block_to_array`
- `qmacro_type` / `ExprTypeDecl`, fixed-array types (post-FA-rework shapes)
- tag splices: `$v $e $a $b $i $f $c $t` through the lowered path
- LineInfo: `force_at` behavior, error-reporting fidelity from a lowered macro
- negative: `-aot-macros` without lowering available → Phase 1 hard error fires;
  jit + unlowered quote → clean `unsupported` message (not "Internal jit error")

## Phase 4 — AOT lane (IN PROGRESS)

No `-aot-macros` harness wiring turned out to be needed: the per-module
`options aot_macros` in the lowered fixture triggers lowering identically during dasAot
generation and at test_aot runtime, so hashes line up by construction. Registered as
test_aot_quote + test_aot_quote_modules (fixtures via DAS_AOT_LIB, linq/_common pattern).

Findings from the first test_aot build of the lowered fixture's generated C++:

- `module_find_enumeration` needed a `DAS_CC_API` declaration in
  include/daScript/simulate/aot_builtin_ast.h (AOT emits direct calls by C++ name).
- `ExprBlock.stackCleanVars` (vector<pair<uint32,uint32>>) reached the reconstruction:
  post-infer allocateStack bookkeeping, always empty in quoted trees, and its type has
  no AOT mapping (C2665 on das_arg<vector<TTuple<8,…>>>). Blacklisted — correct
  semantically AND drops dead runtime work.

- [x] Register tests/quote in tests/aot/CMakeLists.txt.
- [x] **Gate correction:** running `test_aot.exe dastest -- --test …` WITHOUT the
      explicit `-use-aot … --use-aot` pair does NOT enable AOT linking for test files
      (auto-detect didn't fire) — early "test_aot green" results were largely
      interpreted. The CI-form invocation is the real gate and exposed:
- [x] **Path-spelling hash divergence:** the lowered code bakes the source file path as
      a string constant (FileInfoInitData.name). dasAot generation opened the file as
      `D:/…` (forward slashes), runtime as `d:\…` → different constant → different
      own-hash → error[50101] on every `` `quote`lowered`N ``. Fixed at the point the
      constant enters the tree: `normalize_source_path` in quote.das (separators → `/`,
      drive letter uppercased). Exactly the skill-documented mismatch class.
- [x] CI-form full tree under test_aot after the fix: **9439 tests, 9433 passed,
      0 failed, 0 errors, 6 skipped** — tests/quote linking and passing under real AOT.
- [ ] Hash-mismatch fallback test: AOT objects built with `-aot-macros`, runtime without
      (and vice versa) must fall back to interpreter cleanly, not link garbage.
      (Deferred — exercise alongside the `-aot-macros` default-on decision; the noAot
      exemption side is already covered by the unlowered fixture under test_aot.)

## Phase 5 — JIT lane (FOLLOW-UP: starts only after Phase 4 is green)

Decision 2026-06-11: JIT is a follow-up effort, not part of the initial push. The only
JIT work in scope before then is the Phase 1 diagnostic (clean `unsupported` message
replacing the misleading "Internal jit error" panic). The QuotePass gate stays
`aot_macros`-only until this phase; extending it to `jit_enabled` lands here.

- [ ] Extend QuotePass gate to `jit_enabled`.
- [ ] `tests/jit_tests/` coverage for quote under `options jit` (lowered path through
      LlvmJitVisitor — make-struct of handled types, `to_array_move`, by-name lookups).
- [ ] Decide + implement JIT-of-macro-contexts: lift the `is_compiling_macros()` skip in
      llvm_macro.das behind a policy. This is the compile-time payoff twin of AOT'd
      macro modules. Separate PR; needs its own bake time.
- [ ] `LLVM_JIT_CODEGEN_VERSION` bump only if emitters change (diagnostic-only changes
      don't).

## Phase 6 — extra coverage + payoff measurement

- [ ] Sweep: lift `options no_aot` from tests gated only because of quote/qmacro
      (candidates from the 41-file list; case-by-case — some are no_aot for other
      reasons). Same for jit exclusions (e.g. `failed_jit_abi`-style gates).
- [ ] daslib sweep: any module carrying `no_aot` purely for quote.
- [ ] Measure: imgui_demo compile profiling harness (D:\Work\daScript-profile playbook)
      with AOT'd / jitted macro modules — this is the number that justifies the work.

## Open questions

1. ~~Gate predicate spelling~~ — RESOLVED: `aot_macros` only for the initial push;
   `jit_enabled` extension lands in Phase 5.
2. ~~JIT-of-macro-contexts scope~~ — RESOLVED 2026-06-11: JIT is a follow-up phase,
   after the AOT lane is green.
3. ~~FileInfo identity~~ — RESOLVED direction (2026-06-11): per-file interned dummy as
   the floor (Boris), upgraded to the real live FileInfo when compiling — macro-module
   source is parsed even under AOT, so the compiling program's FileAccess has the
   actual FileInfo cached by name; AOT carries only the name string. See Phase 2.
4. `-aot-macros` default: once tests are green, does `daslang -aot` flip it on by
   default (with `noAot` fallback removed), or stay opt-in for a release cycle?
