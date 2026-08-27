# daslib architecture notes

Design rationale a maintainer cannot recover from the code alone. One numbered section per
module; entries are anchored to symbols.

## 1. perf_lint

- **Two parallel loop spaces.** `loop_depth`/`in_closure` (+ `VarStackEntry.depth`) is the
  contract for every loop rule: any closure body is deferred, hence not "in" the loop.
  PERF031 needs the opposite for inline block arguments (`peek_data(s) $(d) { ... }` runs
  in place), so `inline_aware_loop_depth`/`in_deferred` (+ `inline_aware_depth`) count
  loops inside inline block-argument bodies as real loop levels and defer only lambdas,
  local functions, generators, and generated blocks. `inline_block_entry_depth` records
  the loop depth at each inline-block entry because a `return` there unwinds only to that
  depth: loops at or below it keep re-invoking the block, so only barrier 0 proves the
  body runs once.
- **Closure-guard placement is load-bearing, per rule.** Four idioms coexist -
  `in_closure > 0` early return, `in_closure == 0` gate, `in_deferred == 0` gate, no guard -
  and the guard's POSITION inside a visitor method encodes which rules are meaningful
  inside a lambda. PERF001 deliberately has no guard: `loop_depth` never advances inside
  closures, so its loop check is already scoped; syntactic rules (PERF003, PERF021) fire
  anywhere because the pattern is wrong regardless of where it runs. Moving a guard by two
  statements changes which rules fire.
- **`perf024_skip_iptr` is a one-slot handshake** between `preVisitExprCallArgument`
  (classifies an annotated-arg use as safe, stashes the variable's identity) and the next
  `preVisitExprVar` (consumes the stash instead of disqualifying). It is sound because the
  classifier accepts only a bare `ExprVar` after one `ExprRef2Value` peel, so the very
  next variable the walker reaches is that argument's own. Widening the peel set breaks
  the adjacency and mis-applies the stash.
- **`add_ptr_ref` is the qmacro-splice marker.** PERF023 rests on `apply_qrules` wrapping
  every `$e(...)` tag in `add_ptr_ref(...)`; matching the name suffices because the
  templates_boost wrapper is its only producer post-expansion. Its depth tracking fires
  before the `in_closure` gate - the rules block is itself a closure.
- **Hot-path walk (PERF026-028).** `HotCache` keeps per-function scan results sliced into
  two flat arrays; `sinks_span_by_fn` doubles as the presence key and the two tables are
  strictly co-populated (one insert site each, adjacent). The visited set keys on
  (callee hash, `nearest_root_file_site` pointer): reaching one callee from two root-file
  call sites must report twice, so a function is re-walked once per distinct anchor -
  a cost taken for per-call-site diagnostics. Generated/generic frames re-anchor to the
  caller side because their own `at` points into the template. A call to any function
  with a `@scratch` parameter prunes that callee's whole subtree from the walk - the prune
  is deliberately TRANSITIVE, removing the callee's whole subtree and not just the sink,
  because narrowing the prune to the sink buries scratch-heavy callers in deep-resize
  findings on buffers the helper family sizes correctly.
- **PERF030 release credits are consume-once**: a `delete x` licenses exactly one
  subsequent `x <- ...`, then the warning re-arms; the lowered delete family is matched
  by name in its four spellings (`_::finalize`, `finalize`, ``builtin`finalize``,
  ``builtin`finalize_dim``).
- **`"$"` in module allowlists is the builtin module's name** - `Module_BuiltIn` registers
  as `Module("$")` (`src/builtin/module_builtin.cpp`), so macro-time reflection reports
  core builtins under `"$"` beside `"builtin"` and `"strings"`.
- **`Function.tempStringResult` marks a FRESH per-call string allocation** - never a view
  or a passthrough (`include/daScript/ast/ast.h`). A temp-string builtin (`to_char`,
  `repeat`) on a hot path IS heap traffic; the PERF026 string-returning catch-all
  deliberately has no temporary exemption. PERF020's `_type.flags.temporary` check is a
  different axis: a `#`-typed VALUE being cast, where `string(x#)` is a load-bearing
  temporary-to-permanent clone.
- **PERF021's tail-argument comparison passes `require_pure = false`** - workhorse-cast
  tails are flags and literals after the injected Fake* skips, so purity has nothing to
  protect there; the default protects rules that collapse duplicated subexpressions from
  changing evaluation counts.
- **`report_key` bit layout**: line in the low 20 bits, column shifted above it, XOR'd with
  the FNV-64-prime-mixed `fileInfo` pointer and the rule-code hash. Lines past 2^20 alias -
  accepted; a collision silently drops one warning.
- **`parse_range_leg` operator polarity**: `upper_op` is the var-on-left operator marking
  the upper-bound leg (`<=` closed-in-range, `>` open-out-of-range), `lower_op` the lower
  (`>=` / `<`). Yoda (const-on-left) forms flip, so the const-on-left arm tests against
  `lower_op` - the `bounds_above` assignments differ between the two arms by design.
- **PERF022 is a three-function protocol over one stack.** `perf022_push_sentinel` pushes a
  frame in `preVisitExprFor` unconditionally, so the stack stays lock-step with for-loop
  nesting; `preVisitExprForBody` may flip the frame's `matched`; `perf022_pop_and_check`
  fires when a frame survives matched-and-undisqualified - at the for-loop's `at`, the
  actionable line a reader rewrites, not the push site.
- **A generic-body exemption exists exactly where the remedy is not instantiation-safe.**
  PERF020's fix (delete the cast) is wrong for the sibling instantiations of the same
  source line, so it bails on `fromGeneric`; PERF019/PERF021 rewrites (fuse under one
  cast, hoist over the ternary) stay valid for every instantiation, so they fire
  everywhere.

## 2. lint_config

- **`.lint_config` is parsed once per process.** `[lint_macro]` apply() fires once per
  module in the require chain, so an uncached read re-parses the repo file N times per
  compilation; the file is repo policy and cannot change mid-compilation, so
  `ensure_cache_loaded` splits it once into off/on/path-exclude caches.
- **`LINT019` in a line's nolint list exempts the WHOLE line from stale-checking** -
  chosen tradeoff: the in-tree escape hatch is `nolint:PERF006,LINT019` on lines whose
  code fires only in downstream compiles; per-code vouching would need new syntax.

## 3. lint

- **`lint_error` reports only nodes whose lineinfo belongs to the current function's own
  file.** An inliner splice or macro-manufactured node from another file gets its
  diagnostics from that file's own compile; reporting it here would fail a module on
  idioms it cannot edit.
- **LINT022 collects references from three sources because none alone sees them all**:
  the ordinary walk (resolved pointers); a latched second walk over generic bodies
  (`visit(prog)` skips generics, and uninferred bodies carry only spelled alias names);
  and declarations plus string literals (signatures and name-based lookups leave only a
  literal). Self-reference is pre-inserted so it cannot keep a private type alive.
  Functions are checked only when inlining is off - inlining splices small private
  callees away and leaves a body whose only callers were inlined away, indistinguishable
  from dead code. Public symbols, lifecycle/macro hooks, virtual methods, and generic
  bodies are skipped: their callers are invisible to lint.
- **Argument rules skip what the author does not own, and converge leaf-first.**
  LINT012/013/014/023 stay silent on class methods, `[extern]` stubs, `finalize`
  overloads, and address-taken functions; LINT014 also skips a parameter whose mutability
  a callee demands - the leaf is flagged first and each fix re-exposes the next caller.
  A returned `var` argument keeps its `var` (a non-copyable result cannot move from
  const); a used `_name` parameter is never LINT004'd (interface and keyword-clash names).
- **Closure bodies are per-rule, not global.** LINT010 counts a closure body as a branch
  (it may run later or never - writes inside must not kill outside stores, reads inside
  must not keep an outside init live); LINT021 counts the same body as an escape - a
  captured 64-bit local is never a narrow.
- **`expr_equal` compares `describe(a) == describe(b)`, never pointers** - every
  Expression has exactly one parent, so sibling pointer equality is effectively never
  true. LINT007 excludes `ExprConst*` operands: identical constants are macro-splice
  residue, not copy-paste typos.
- **LINT015 fires on `+` and `-` only** - the only operators both binary and unary, so a
  split continuation line re-parses as a valid discarded unary statement; every other
  operator produces a loud parse error instead.
- **LINT023's store set extends the compiler's.** `under_clone` marks only bare-variable
  `=`/`:=` targets; `<-`, swizzle LHS, and statement-level `++`/`--` are marked by the
  visitor. A pointer parameter stays a candidate: a write through the pointee reaches the
  caller and counts as a use, not a store.
- **The 64-bit family fires only where the overflow hazard survives.** LINT021 treats
  only unbounded-top math as transparent (`min`/`clamp`/`sign` cap the value - deliberate
  saturation); LINT024 requires a call among the product's leaves - that is the unbounded
  factor, a product of plain locals is tile geometry.
- **`canVisitWithAliasSubexpression` returns false on purpose** - `assume` keeps the
  template block at the definition site and clones it per use; the block is never invoked
  there, so descending would report its arguments unused.
- **C++ mirror pairs**: `lint022_optimized` / `lint022_calls_may_be_inlined` <->
  `Program::getOptimize` / `Program::patchInline`; `stale_scan_line` <->
  `rtti_is_nolint_suppressed`; `is_inline_temp_name` <-> `INLINE_TEMP_PREFIX`. Nothing
  fails when one side moves alone.

## 4. style_lint

- **Lint runs under policies that skip const-folding and leave some types unset.** Take a
  variable's type from `Variable._type`, not an `ExprVar`'s; expect `T('c')` to stay an
  `ExprCall`; expect `-1` as an `ExprOp1` over a constant; when a type is missing, bias
  conservative ("needs unsafe").
- **Rules whose forms are identical in the AST read the source span instead** - `defer <|`,
  generators, STYLE003's `$()`, STYLE028's three invoke spellings. STYLE005 uses LineInfo
  identity: the parser gives braceless and postfix forms a synthetic block sharing the
  terminator's LineInfo, so `blk.at != inner.at` <=> the user wrote braces. Both of these
  tests - the source-span read and the LineInfo identity check - are the rule's only
  evidence for the distinction, and both break silently when lowering or LineInfo
  assignment changes.
- **STYLE024/025 mirror the compiler's unsafe rules with a deliberate asymmetry: in doubt,
  mark "needs unsafe".** A false "needs" costs a missed note; a false "redundant" tells
  the user to delete a wrap the compiler requires. `let v & = unsafe(expr)` is the one
  recorded exception - the wrap is required by the reference binding, not the inner
  expression. STYLE025 narrows only when the single statement has an expression form.
- **A require counts as used when its symbols cannot be traced, not only when seen.**
  Generic instances attribute to `fromGeneric._module`; bare names in never-instantiated
  generic bodies keep their providers alive; re-export chains are followed; a module
  providing any compile-time macro or lifecycle hook is used - it leaves no AST reference.
  STYLE029 fires only when exactly one re-exported module is used: two or more is an
  aggregation facade.
- **Metric rules attribute code to the module that can fix it.** STYLE037/038 walk the
  `fromGeneric` chain to its root (`[template]` generics instantiate INTO the caller);
  generated carriers stay unmetered; closures are not length-checked separately - the
  host trips first.
- **STYLE041 is sound because of a reference count, not the structural scan.** The scan
  classifies writes, plumb reads, and the single consuming `if`, arming the candidate with
  that count; `preVisitExprVar` counts every reference in the function, and the warning
  fires only when the counts match. Anything the scan does not descend into therefore
  rejects the candidate instead of being assumed harmless.
- **STYLE036 keys on a flag the compiler clears.** Infer strips a substitution contract
  when a generic binds; one still set on a resolved cast target was never consumed - but
  an `auto`/unresolved-alias target has simply not been substituted yet, hence the
  `isAutoOrAlias` bail.
- **STYLE039 requires non-ASCII in both the value and its source span** - escape-spelled
  bytes in ASCII source are log-safe, and span-slicing keeps a neighbouring comment's
  typography from firing.
- **The unsafe-frame walk**: `preVisitExpression` pushes one frame per expression node,
  `visitExpression` pops it - the popped frame is the subtree's summary, stored in
  `unsafeExprs` for `visitExprUnsafe` (STYLE024 vs STYLE025), propagated to the parent
  slot, and fired as STYLE024 when an `unsafe(...)` target's subtree had count 0. The
  balance panic in each entry point is the guard against a `canVisit*` override skipping a
  subtree the frame walk still counts.
- **Two compiler regimes run this visitor.** The `[lint_macro]` path runs inside a normal
  compile (const-folding ON - `T('c')` folds to a typed const); the standalone runner and
  MCP path run under lint policies (folding OFF - `T('c')` stays an `ExprCall`, `-1` stays
  an `ExprOp1`). Shape-sensitive rules carry both arms (`numeric_char_cast_target`'s
  ExprCall branch + source-span re-parse); deleting the "redundant" second arm kills the
  rule in the other regime.
- **`expr.canShadow` on a for-variable exempts it from LINT002/003/004** - the flag marks
  macro-generated loops, and lint does not judge names the user never spelled.
- **The `expect` probe scans the whole file while `lint-skip-file` is header-capped** -
  an `expect` directive anywhere makes the file a compile-error fixture (its position is
  semantic to dastest), whereas `lint-skip-file` deeper than the header would let quoted
  prose unlint a file.
- **The `blk.list` index rescans are O(block^2) by choice** - the per-statement visitor
  callback carries no index, and a shared index stack would have to push/pop with nested
  blocks; block statement lists are short.
- **Threshold provenance**: STYLE038's 80 ~ p97 of this tree's function lengths; STYLE041
  caps report-arg recursion at 8 and fails closed (deeper is unknown shape);
  `LINT_SKIP_HEADER_LINES = 16` exists so prose quoting the directive cannot unlint a
  file; STYLE037's 20 and `note_type`'s depth 24 have no recorded derivation.
- **C++ mirror pairs**: the STYLE024/025 unsafe map <-> infer's `unsafe_*` rules;
  `style036_inert_contract` <-> infer's contract clearing. Nothing fails when one side
  moves alone.

## 5. aot_cpp

- **C++ identifier mangling**: `aotSuffixNameEx` prepends `_S`/`_E`/`_V`/`_f_` when a das
  name is a C++ keyword, holds a non-alnum char, or is `DELETE` (winnt.h). Structs and
  enums share ONE C++ namespace while daslang keeps separate tables, so `struct X` +
  `enum X` in one module is legal das and a C++ redefinition - `buildStructEnumCollisions`
  finds the pairs per module and forces both suffixes.
- **The emitter targets the MSVC/GCC/Clang intersection; the redundant-looking spellings
  are the portable ones** - `INT64_C`, the explicit `0x8000000000000000` enum arm,
  `to_cpp_double`'s named non-finite tokens (mirrors `src/simulate/runtime_string.cpp` and
  moves with it), elaborated `struct X`, double parens around `das_iterator` sources
  (most vexing parse), non-const value loop variables (`first()`/`next()` write into it).
- **Sequenced binary operands**: any op2 whose operands are not both side-effect-free is
  emitted through `das_ordered2{ L, R }` plus an immediately-invoked lambda. The struct and
  its member names live in `include/daScript/simulate/aot.h` and move with the emitter's
  `__lr.left`/`__lr.right` spellings.
- **Stack-frame `new`/ascend**: per-block storage declared once, the USE site
  re-initializes per evaluation (memset for `new`, whole-value overwrite for ascend) -
  dropping the reinit reuses the previous iteration's value.
- **fp16 and the 8/16-bit lattice go through the vec4f policy ABI**: `SimPolicy_HalfVec` is
  vec4f-shaped even at width 1, so scalar fp16 always casts across the policy boundary,
  and a lattice-vector policy result must come back typed for `das_equ_val` deduction.
  Lattice swizzles use element-typed helpers - generic `das_swizzle` caps at 4 fields,
  `v_extract` is 32-bit-lane only.
- **A struct needs a COMPLETE C++ type more often than field access suggests** - globals
  need `sizeof` even untouched, and by-value field types emit depth-first before their
  owner, or the type degrades to a forward declaration.
- **`aotModuleName` has a C++ twin that moves in lockstep** -
  `src/ast/ast_typedecl.cpp`'s `aotModuleName`/`describeCppTypeEx` spell the same type
  names into the same TU through `DebugInfoHelper`'s cached cpp names
  (`debug_helper_find_*_cppname`, the cross-platform `TypeSize<...>` sinks). The
  main-module namespace override is therefore one knob with two ends:
  `set_aot_main_module_name` writes the daslib global AND forwards to
  `set_aot_main_module_name_cpp`; a spelling rule changed on one side only produces a TU
  where the definition and its debug-info references disagree.

## 6. aot_standalone

- **The generated constructor IS the init protocol** - a standalone context never calls
  `Context::runInitScript`, so the ctor reproduces its observable semantics inline:
  `memset(context.globals, 0, context.getGlobalSize())` mirrors runInitScript's globals
  memset (`src/runtime/context.cpp`), then the same-TU `__init_script(&context, true)`,
  then each `[init]` function directly. `aot_cpp.das`'s `preVisitGlobalLet` writes the
  `__init_script` signature - a daslib-to-daslib pairing nothing checks.
- **`__init_shared` is hardcoded `true`** - a fresh standalone context always owns its
  shared globals.
- **The ctor's base `Context(N)` folds the init headroom in, because there is no separate
  init stack** - init locals are C++ locals in AOT. A script that sets `options stack` to a
  positive value gets `N = options stack + globalInitStackSize`, honored exactly, the
  interpreter's floor included: the floor protects INTERPRETED init, while standalone init
  is AOT, so an embedded target's stack budget wins. Zero and negative fall through to
  `N = max(policies.stack, 16384) + globalInitStackSize`; `Context(0)` owns no stack, so any
  prologue would assert. The fall-through arm mirrors the interpreter's init-stack formula
  in `src/ast/ast_simulate.cpp` - a pair, deliberately divergent on the explicit-option arm.
  `globalInitStackSize` reaches the emitter through the rtti `Program` binding in
  `src/builtin/module_builtin_rtti.cpp`, so an `invoke` during init still has das stack for
  its prologue.
- **There is no `!stopFlags` guard between `[init]` calls** - a panic propagates out of the
  ctor instead of soft-stopping the sequence.
- **Global init order is fixed by `var->index` assignment**: `StandaloneContextGen`'s
  `preVisitGlobalLet` emits required modules' globals via ordered `for_each_module`
  before the adapter walks the entry module's own - correct only because
  `ast_allocate_stack.cpp` assigns `var->index` through the same dependency-first
  module order with the entry module last, and `runInitScript` executes ascending
  index. `[init]` function order is not re-derived at all: the emitter reads the
  simulated context's list through rtti `for_each_init_function`, so the C++ late-init
  sort stays the single source of truth.
- **Cross-module limits fail loud at emit time**: only main-module, AOT-emitted `[init]`
  functions can be called from the ctor (required-module and `[no_aot]` ones are collected
  emit errors with the reason), because the standalone TU only emits the entry module's
  function bodies.
- **Every used function must have an AOT body** - a standalone context has no
  interpreter, so a used `noAot` function (the `[no_aot]` annotation, or `NoAotMarker`
  finding a type AOT cannot express) is a collected emit error, never a
  `fnByMangledName` call that would crash at runtime. `prepareProgramForEmission` runs
  `NoAotMarker` first (the regular AOT paths run it too; standalone must match) and
  then `checkAllUsedFunctionsCanAot` walks used, non-builtin functions.
- **Type definitions live in the header, once** - struct/enum definitions (the
  dependency dump plus the entry module's own `declarations` capture) are emitted into
  the `.das.h`, which the `.das.cpp` includes; the source never redefines them. They sit
  in module-named namespaces directly under `das`, NOT under the per-program
  `_anon_<hash>` namespace - that hash changes with the program, so it can never be a
  public name. The entry module is promoted with an empty name, which would put its
  types in an anonymous namespace (a distinct type per TU - an ODR/link break for any
  struct in an exported signature); `aot_cpp`'s `set_aot_main_module_name` names it
  after the context for the duration of standalone emission, so class signatures, the
  source TU, and the embedder all resolve the same `das::<ctx>::T`. The header carries
  `#pragma once` and the required modules' `aotRequire` includes so it stands alone in
  an embedder TU; two DIFFERENT contexts' headers sharing a das dependency still cannot
  be included in one TU (the shared types have no per-type guards).

## 7. flatten

- **Predicated lowering carries one live-mask per exit flavor** - `__flat_live` for
  return, a per-loop break mask (persists across unrolled copies) and continue mask
  (re-minted per copy). A write's predicate ANDs every active mask plus the structural
  predicate; a narrow term excludes its own mask so it self-cancels. An inlined callee
  gets a fresh live mask and lowers with `ctx.loopMasks` moved OUT, so its break/continue
  can never reach the caller's loops.
- **`flatten_preshade_cse` is a joint fixpoint, not a pipeline** - extraction, regroup,
  CSE and alias elimination mutually enable each other; the `_preshader_`/`_cse_` counters
  are owned by that loop and re-seeded from surviving suffixes (per-call numbering
  re-mints a live name).
- **A CSE/regroup tally counts exactly the regions its rewrite can change** - a duplicate
  counted where the rewrite cannot reach never drops below 2 and runs the fixpoint to its
  iteration cap.
- **`__flat_ret` carries `safeWhenUninitialized` only while every write is a
  self-referential select** - a lowering change that makes the bare-decl read observable
  turns the flag into a real uninitialized read.
- **CSE is local value numbering over one converged basic block, and it is complete** -
  pure subtrees keyed by `describe()`; value-stability = reads no reassigned name; a store
  through index/field/swizzle destabilizes its base; an unrecognized node fails closed as
  mutable-reading. Uniform duplicates route to the preshader.
- **The copy-prop/CSE walks stay O(size)** - one name-to-statement index, one structural
  walk. A `string` materialized per `ExprVar` in a visitor callback breaks that: each
  `describe()` allocates a string that lives to the end of the pass, so the walk goes
  quadratic in heap bytes, not only in time.
- **`MutCollect` is what CSE trusts to say whether a name is stable, so it counts every
  store spelling, not the one the lowering emits.** CSE treats a name outside its set as
  constant for the whole block; a missed store is a shared subexpression across a mutation.
  Copies are only the visible half - `<-` also zeroes its SOURCE, `:=` lowers to a
  `builtin`clone`(dst, src)` CALL rather than an `ExprClone`, `++`/`+=` are their own
  nodes, and a by-reference
  argument writes with no assignment node anywhere. Hence the argument arm keys on the
  callee's parameter type (non-const and `ref` or a ref type), not on a node kind.
- **`delete` on a container of `ExpressionPtr` frees the BUFFER, never the nodes** -
  `delete array<T?>` frees the pointees only for das-heap `T`, and `Expression` is a
  handled C++ type whose instances are not heap chunks at all (the
  measurement: an `array<S?>` of das structs returns its pointees to `heap_bytes_allocated`,
  an `array<ExpressionPtr>` returns only the buffer and the nodes surface in the exit GC
  report). That is why `make_float_ctor`'s const-fold early return may leave its lanes
  un-consumed while the ctor path `emplace`s them away, why every `unsafe { delete args }`
  after it is sound over borrowed tree nodes, and why a struct field holding a borrowed
  node needs no `@do_not_delete`. Node lifetime belongs to the AST GC: a lane the const
  fold drops is unreachable and collected at the enclosing `ast_gc_guard`.
- **The whitelist admits value-returning primitives only.** `lower_stmt`'s fall-through arm
  lowers an unrecognized statement for its lifted sub-lets and drops the statement itself,
  which is correct exactly while every surviving call is pure - so `lift_expr` refuses a
  whitelisted call that writes through a by-reference argument (`sincos`) rather than let
  the drop delete the store. Predicating such a write would need per-out-param temps the
  lowering does not own.

## 8. ast_verify

- **One node set answers two opposite questions**: a node reached TWICE has two parents
  (broken unique ownership); a gc-owned node reached ZERO times sits in a slot no visitor
  enters, so every visitor-driven pass edits a tree it cannot see in full. Slots C++
  `::visit` skips are walked by hand or recorded as whole trees.
- **Reporting is repair**: diagnostics go through `macro_sticky_error` (infer clears plain
  errors from a repaired tree); null entries in per-entry-dereferenced lists compact in
  preVisit; a self-reachable node is CUT, not reported - a cycle kills every later walk.
  Both passes use the sticky form, post-infer included. `Program::stickyError` records the
  error AND calls `Program::error`, so it is a strict superset of `macro_error` - the plain
  form buys nothing. A loss is unreachable while every re-entry into
  `inferTypes`/`inferTypesDirty` after a post-infer firing stays guarded by
  `!program->failed()`, but repair is what makes the guarantee necessary rather than
  redundant: the guard that found the damage cannot fire twice, so an error cleared once is
  gone for good.
- **`visitExpression` is the only place an ancestor-path entry is dropped.** The C++
  adapter's `VISIT_EXPR` macro (`aot_builtin_ast.h`) calls `visitExpression(that)` before
  every `visitExprXXX`, so a derived visit that erased again would erase nothing - a
  derived visit returns its node and leaves the path alone.
- **The two passes' skip sets are complementary**: pre-infer skips `generated` (filled in
  across passes); post-infer skips only `[template]` bodies and dasbind `[extern]` stubs -
  so generated bodies ARE checked post-infer, by nothing else.
- **A structure's field-default init tree is a declaration, not code**: it never simulates -
  the generated ctor carries its own inferred clone, which stays fully checked. Infer types
  the decl init in place but binds its calls only when construction generates the ctor, so a
  struct nothing constructs legitimately keeps a typed-but-unbound cross-module ctor call
  there. The post-infer func invariant therefore tolerates calls inside field inits
  (`in_field_init`, set/cleared around the field walk); every other invariant (types, ats)
  still applies to them. The same-module shape resolves eagerly and never had the hole -
  the selftest fixture is cross-module for exactly that reason.
- **Each check's licensing C++ site**: `ExprOp1.subexpr` -
  `SimulateVisitor::visit(ExprOp1*)` dereferences;
  `TypeDecl.dim` entries - `TypeDecl::dimConst` sentinel (`ast_typedecl.h`); `ExprFor`
  sources/body - `ExprFor::visit` walks sources and body, never the iterator tags;
  `ExprBlock.arguments` - `ExprBlock::visit` walks `arguments` only when `isClosure`;
  `ExprBlock.returnType` - `ExprBlock::visit` walks it only for a closure.

## 9. quote

- **A lowered quote is a generated leaf function, not an inline expression** - one frame
  per quote instead of inflating every caller's; the result type clones the quote's static
  type because `autoinfer` would leak the concrete node type and break `Expression?`
  identity consumers.
- **`blacklist` entries naming back-references break reconstruction cycles - removing one
  hangs the walk** (`Function.classParent`, `ExprReturn._block`, `ExprVar.pBlock`,
  `EnumEntry.value`); the rest are post-infer bookkeeping absent from quoted trees.

## 10. templates_boost

- **A `Template`'s substitution tables are freed once, by whichever mechanism the local
  declares** - `var inscope rules` finalizes at scope exit; a plain `var rules` does not and
  needs an explicit `delete rules`. Declaring both double-finalizes, and the double is
  silent: inscope-only, inscope+delete and plain+delete all leak nothing, so a heap count
  cannot tell a double-finalize from a correct form; only a plain local with no delete
  leaks. Both `apply_qmacro_template_*` appliers are inscope-only; the three plain-local
  `apply_template` overloads pair the declaration with the delete.
- **`apply_template` returns a possibly-NEW root.** `apply_template(rules, at, expr,
  forceAt)` takes `expr` by value, and the block-form overloads take `Expression?&` only to
  forward it - none writes the new root back. A substitution that replaces the ROOT node -
  an identity key body, `_order_by(_)` - is visible only through the return value; non-root
  replacements leave the pointer unchanged.
- **`stamp_missing_at` fills only MISSING locations**, unlike the force-at of `$e()`;
  `carry_tag_safe_flags` copies the parser's unsafe-wrap flags across `$c` substitution or
  the safety is lost where the result lands.

## 11. linq

- **`top_n_by_with_cmp` exists for the fold splice and has no daslib call site.** When an
  order-by key body is pure and inlineable, `linq_fold_common` emits it with that body
  spliced into BOTH sides of the comparator - killing the per-comparison comparator dispatch
  and the per-side `key(v)` dispatch the key-lambda overloads pay. It is not a redundant
  twin of `top_n_by`.

## 12. linq_boost

- **`BucketLambdaStamper` exists because a bucket-surface lambda cannot infer on its own.**
  On the `group_by_lazy` element shape `tuple<K; array<E>>`, an untyped lambda in
  `<bind>._1 |> select/sum/...(<lambda>)` has nothing to bind against in the fully generic
  tier-2 parameters (error 30303); the chain type knows `E`, so the macro stamps the
  lambda's parameter before the rewrite - the same move as the outer-parameter injection in
  `visit()`.
- **`result_param1_type` is the join result-lambda auto-typing hook.** param0 is always TA;
  param1 is whatever the override returns - `_join` -> TB, `_group_join` -> a bare `array<TB>`
  with const/ref cleared to match the bucket type. `_left_join` / `_right_join` /
  `_full_outer_join` deliberately do not override: their result parameter is `$Option<T>` on
  one or both sides, so those result lambdas stay user-spelled.

## 13. linq_das

- **The substituting scanners share one token model.** `substitute_idents`,
  `mentions_ident` and `rewrite_group_var` all read it: plain `"..."` content is verbatim, a
  `{...}` interpolation body is code that is scanned and substituted, one level of nested
  string literal inside an interpolation is verbatim again, and identifier position
  classifies the same way everywhere - after-`.` field access and before-single-`=` labels
  are not references, with `is_label_position` the shared judge.
- **`JOIN_TI` is a spellable identifier on purpose.** `__`-prefixed names are reserved by
  the compiler, so the transparent-identifier carrier cannot be hidden outside the user's
  namespace and must stay a single plain token - hence the explicit rejection of a range
  variable named `linq_join_ti` instead of an unspellable name.
- **`build_src`'s two arms are a fusion decision, not a formatting one.** A fused source
  (chain head, join) is built as `from_in(...)` so `_fold` can route it; an UNFUSED operator
  argument - the uncorrelated multiple-`from`, whose `_cross_join` runs at tier-3
  passthrough - is passed as the bare parenthesized expression, so `each`'s
  `[unsafe_outside_of_for]` does not trip and the arrayxarray overload is selected.
- **The correlated-flatten collection selector borrows, it never copies.** `unsafe(each(tail))`
  is chosen over `to_sequence` (whose const-array overload clones per row) and over
  `where_`'s array overload (which materializes per row). Borrowing a TEMPORARY tail
  (`from x in [c.id]`, a function result) is safe because daslang heap-allocates arrays and
  does not finalize them at scope exit - the backing storage outlives the borrow and is
  reclaimed by GC.
- **A `FromInMacro` reject returns null so infer can stabilize.** Errors clear on every pass,
  so the not-yet-inferred-source arm's error survives only when the source never infers at
  all - that is what makes that arm a deferral rather than a final verdict.
- **`from_in` identifies typed sources by matching module + type NAME as strings**, not by
  requiring the provider modules: `sqlite_boost::SqlRunner` -> `select_from`, pugixml
  `xml_node` -> `from_xml_node`, `json::JsonValue?` -> `from_json` - linq_das stays
  dependency-free on sqlite/pugixml/json. Both node builders are `[unsafe_outside_of_for]`,
  so the emitted call carries the `unsafe` wrap a `_fold` chain requires. A table source is
  rejected with a fix message before those arms: it already carries its row shape, and the
  typed builders would all mis-fire.

## 14. linq_fold_common

- **group_by's per-key update is a dummy plus an address compare, not a `key_exists`
  probe.** `entry &= tab?[uk] ?? dummy` costs one hash op per element on hits;
  `addr(entry) == addr(dummy)` identifies the miss, which then fills the dummy, inserts it,
  and re-defaults it - the second hash op is paid on misses only. Average slots hold a
  `(sum, count)` accumulator and divide with no zero guard: sound because a key enters the
  table only on its first element.
- **`hoist_prelude` hand-builds a flat block on purpose.** The materializer prelude - the
  hoisted default-row declaration on the field-prune path - must be a SIBLING of the walk; a
  `$b(prelude)` splice nests it in its own scope, where the per-element reads cannot see it.
- **One `at` can reach two materializers.** A `group_by` chain closed by
  `first`/`order`/`distinct`/`take` emits through both the array-walk materializer and the
  handle materializer at the same source location, so `qn` - which keys on (prefix, at) -
  keeps the two sites' binds apart only while each site carries its own prefix.
- **An `ExprFor` the typer has not re-inferred carries an EMPTY `iteratorVariables`.** A
  decs bridge feeding a no-chain terminator (`from_decs_template(...).count()`) has no chain
  op to force the second inference pass, so the iterator names are recovered from the push
  tuple's values - each references its iter var by name under an `ExprRef2Value` wrap - and
  match what the loop binds once a later pass infers it.

## 15. linq_fold_sql

- **`extract_sql_source` walks the call's own `arguments[0]` spine, not linq_fold's
  flattened `top`** - the SQL DSL ops are not `linqCalls`, so `flatten_linq` stops at the
  first one and never reaches the `select_from` source. `sql_linq_loaded(prog)` then gates
  on `_sql` being in THIS program: a consumer can get `select_from` from a provider boost
  without `daslib/sql_linq`, and that chain must fall through to the in-memory path.

## 16. linq_fold_decs

- **Column pruning is a four-step pipeline over the `decs_tup` bind, and every step exists
  to reach bind elision.** Scan the body for `decs_tup.<field>` uses; if the bind is
  referenced as a WHOLE var, rewrite each whole-var use into a synthesized
  `(userName1 = iter1, ...)` literal - semantically identical - and re-scan; then flatten the
  surviving field reads to the iter vars and drop the bind entirely. Unused `get_ro` slots
  disappear along with one tuple-make plus N field reads per iteration. The unpruned bind
  stays only for whole-var refs that survive the rewrite.
- **The decs random-index fast paths rest on these source facts.** A plain `[decs_template]`
  field compiles to `get_ro` (an indexable array) while a default-init field compiles to
  `get_default_ro` - an iterator, not indexable - so `decs_can_random_index` bails and the
  walk stands. `get_ro` is `[unsafe_outside_of_for]`, so each cloned source AND the index
  read are marked `alwaysSafe` (mirroring `decs_boost`'s `append_index_lookup`).
  `for_each_archetype` visits archetypes in order and skips empty ones, so a no-predicate
  `last()` overwriting a result per archetype ends holding the global last in
  O(num_archetypes), and `reverse |> take(N)` collects only the tail with the boundary
  archetype's head indexed past rather than walked.
- **Trivial-select elision removes the copy a `_select(_.userName)` would emit.** The
  pruned inner-for already keeps that component's iter var, so the synthetic bind would
  flatten to a pure copy; renaming the chain's `finalBind` to the iter var skips the binding
  entirely. Detecting the shape needs TWO `ExprRef2Value` peels: the typer wraps both the
  projection root and the `ExprVar` inside the `ExprField`.
- **The decs join hook keeps hash-collect and probe inline rather than routing through
  `wrap_source_loop`** - that is what preserves the count-no-`where` fast path, which
  answers from bucket lengths instead of walking pairs. Its key lambdas are synthesized
  upstream by the LinqJoin macro as 1-arg single-return blocks, so the BLOCK's return type
  is the key type; a change to that synthesized shape breaks the decs join silently.

## 17. sql_linq

- **Equal phase numbers are deliberate.** `PHASE_ORDER_BY == PHASE_SELECT` - ORDER BY
  commutes with the projection in SQL (it may reference projected aliases or source
  columns) - and `PHASE_GROUP_BY == PHASE_DISTINCT == PHASE_SET_OP`. Giving either member
  its own number spuriously diverts the canonical `_group_by |> _order_by |> _select` into
  a nested SELECT.
- **Emitted SQL is a template over two in-band markers.** `?` is a bind, `\x01` a runtime
  identifier (a dynamic `_order_by` column name); `next_placeholder` scans outside
  single-quoted literals and always resumes just past a placeholder, so the scan starts
  with no quote open. `sql_to_frags_ex` pairs each marker with `orderedBinds` /
  `orderedInlineIds` in occurrence order and preserves a marker as text once its list is
  exhausted. Nothing reaches the text unquoted: a compile-time constant folds in with SQL
  doubling, a non-constant emits a runtime `sql_quote_id` / `sql_quote_lit` call, and every
  other runtime value stays a `?`.
- **Key contexts render with constants inlined because their fragment is re-used.** A
  computed `_group_by` / `_order_by` key fragment is emitted at several SQL positions
  (SELECT, GROUP BY, ORDER BY) while a placeholder can be bound only once, so
  `render_inlined_key_sql` sets `q.inlineConstants`, admits only `ExprConst*` (strings
  through `sql_quote_lit`), and rejects any key that pushed a bind.
- **`_distinct_by(K)` is dialect-routed on `caps.distinct_on`.** Without it, SQLite's
  bare-aggregate form `SELECT *, MIN/MAX(pk) FROM t GROUP BY K` keeps the `*` columns of
  the min-pk (or max-pk) row per K - SQLite-only semantics; strict engines reject bare
  columns beside GROUP BY. With it, `SELECT DISTINCT ON (K) * ... ORDER BY K, pk [DESC]` - a
  PostgreSQL extension DuckDB also implements. MIN/ASC is linq's "first row per K", MAX/DESC
  is `reverse() |> _distinct_by(K)` "last row per K" - both only while pk is monotonic with
  insertion order.
- **A join's `into` projection registry is snapshotted because the outer projection
  clobbers the live one.** `process_join_call` copies the live projection into
  `joinProjRecordNames` + the `joinSelectCol*` arrays, which survive
  `analyze_grouped_projection`'s clear-and-repopulate. Post-join `_.<alias>` resolves
  through that snapshot in every post-join consumer - `pred_to_sql`'s column-ref arm,
  `collect_one_order_key`, `push_group_key`, `try_translate_group_aggregate`,
  `try_translate_global_aggregate` - all through `find_projection_alias` +
  `render_projection_alias_sql`; a new post-join alias consumer goes through that pair too.
  A registry miss is rejected loudly everywhere alike: falling back to base-table resolution
  would leak the unqualified base-table namespace into post-join predicates.
- **`normalize_single_source_arg_names` exists for the linq_das front end.** `_sql`
  resolves a single source against the placeholder `_`, but LINQ-syntax lowering splices
  the user's range variable verbatim (`$(c) => c.field`), so a single-parameter chain-op
  lambda's bound variable is renamed to `_`. Gated on a pure single-source chain - join and
  set-op key lambdas bind distinct sources and are left alone.

## 18. dupe_detect

- **A run is unextractable when its meaning depends on the scope around it** - the gate
  counts `unsafe`-authorized operations, `assume` aliases, `defer` (lowered away), and
  `break`/`continue` targeting a loop the run does not own.
- **The index is a preorder node array over the LIVE AST** - records keep node ids and
  read `.at` off the node; `ExprConst*` re-enters the base pre-visit hook and the second
  entry pushes a `-1` marker - lose it and every preorder interval is wrong.

## 19. only_nttp

- **C++ mirror pair**: `only_nttp`'s `moreFlags2.nttp` read <-> `ExternalFnInline`'s
  `this->nttp = true` (`include/daScript/ast/ast_interop.h`), plus the positional
  `MoreFunctionFlags2` argNames list (`src/builtin/module_builtin_ast_flags.cpp`) that
  gives the bit its daslang name. Nothing fails when one side moves alone.

## 20. interfaces

- **The implements-marker IS the generated getter field** - `is`/`as`/`?as` key purely on
  its presence; parent interfaces get their own deduped getter fields.
- **The const getter's `unsafe(addr<$t(st)? -const>(self))` is the one sanctioned
  const-strip write**, and the dead-store eliminator cannot remove it on two counts, not on
  the `unsafe`: the constness sits on the PARAMETER BINDING while the object behind it is an
  ordinary mutable allocation, and the store is re-read through the same pointer two lines
  later while the new proxy escapes into it - no tier can prove it dead. Emitted AOT C++
  keeps the cast and the store verbatim. Caching into anything the caller owns by const
  VALUE would not survive this.

## 21. flat_hash_table

- **`hashes[i]` is the slot state** - 0 never-used (probe stops), 1 tombstone (probe
  continues), above 1 live; a hash function that can return 0 or 1 loses entries silently.

## 22. coverage

- **Instrumentation is `generated`** (lint-invisible, inliner-safe) and asserts become
  verify so their spliced counters survive release.

## 23. regex

- **Zero-width nodes (lookahead, Bos/Eos, word boundaries) leave `subexpr.next` null on
  purpose** - chaining them would consume the continuation.
- **The literal matchers compare byte-by-byte and return on the first mismatch** - a
  fixed-`textLen` block compare (memcmp) reads past the terminator whenever the terminator
  falls before the literal's last byte (the tail at least two bytes shorter than the
  literal).

## 24. debugger

- **`g_installed_agents` is the GC root for every installed agent** - the C++ adapter holds
  a raw classPtr the das GC cannot see.

## 25. typemacro_boost

- **The parser does not run annotation `apply` for macro-added functions** - the add/erase
  pair in `tsi_stamp_methods` IS the trigger, not bookkeeping.

## 26. constant_expression

- **The generated specialization is called through `__::`** - a plain name resolves in the
  call site's DEFINING module, where it does not exist.

## 27. toml

- **The writer is hand-rolled because the builtins do not round-trip** (`\v`, raw 0x7f,
  float exponent thresholds); a value scan that runs into a bare-key char rewinds and
  re-lexes as a key - a new numeric form needs the same rewind.

## 28. shader_block_layout

- **Two rails, deliberately separate** - the LAYOUT rail admits int64/uint64 as block
  members (`compute_block_layout` special-cases them) while the ARITHMETIC rail rejects
  64-bit INT (`arith_width_ok` allows width 64 only for floats); `cpu_only_lattice_width`
  keys both emitters' fail-closed diagnostic.

## 29. shader_lingua_franca

- **Every symbol is either an exact CPU mirror of its GPU semantics or a `[sideeffects]`
  dummy every rail lowers by name** - the dummies return zero on the host, so a CPU replay
  reproduces GPU semantics only for the real-bodied set. Unsigned overloads never fold into
  signed twins (glslang picks the unsigned opcode).

## 30. archive

- **`MemSerializer.write` grows capacity eagerly** because under a very_safe_context each
  doubling generation is abandoned, not reused; no alias into `data` survives a write.

## 31. json

- **`is_json_white_space` is deliberately not the shared `is_white_space`** - RFC 8259
  admits exactly space/tab/CR/LF.

## 32. jsonrpc

- **Parsing a request is a SCOPE, not a value.** `ParsedRequest.params` is a borrowed view
  into the parse tree, so the tree has to outlive the handler and cannot be freed inside the
  parser - instead the owner is named in the result (`document` on `ParsedRequest` for
  `parse_request`, on `ParsedBatch` for `parse_batch`, where the entries borrow and own
  nothing) and the scope is closed by `free_request` / `free_batch`; `dispatch_line` owns
  the whole scope and frees after the dispatcher has run over every entry. Freeing is safe
  for the string fields because daslang never finalizes a `string` field
  (`TypeDecl::needDelete` is false for `tString`), so `method` / `id_str` / `params_json` /
  `error_envelope` outlive the tree - only the `params` pointer dangles, which is why the
  free nulls it. The response side has no such scope: `parse_one_response` copies everything
  into strings, so `parse_response` / `parse_response_batch` free their own tree before
  returning.

## 33. sql_migrate

- **The audit table is provider-neutral by construction** (client-side epoch seconds,
  BIGINT); duplicate versions are caught in two layers because neither sees everything; the
  `struct_convert_field` overload set is a specificity ladder - deleting a "duplicate"
  silently re-routes conversions.

## 34. fio

- **The glob matcher follows POSIX fnmatch on degenerate patterns** (unterminated `[`, `]`
  as first class member, `**/` slash rules) - conformance, not quirks.

## 35. decs

- **Component finalizers are lambdas over the component's own untyped storage** - deleting
  the lambda IS the finalization event; capacity checks compare the highest allocated value
  (`base + count - 1`), not the exclusive end.

## 36. builtin

- **`_table_index_and_init` exists for infer's `default_init_containers` rewrite of
  non-store `tab[key]`** - it has no daslib call site and is not dead.

## 37. linq_fold module layout and the SourceAdapter contract

- **Charters.** `linq_fold` is dispatch only: source recognition (`try_splice_patterns`),
  adapter construction, the tier-2 `fold_linq_default` cascade, the `LinqFold` call macro,
  and the single registrar `register_all_linq_fold_rows`. `linq_fold_common` holds what no
  source can influence - the pattern-table types, the walker, the predicate library, the
  chain pre-passes, the generic emit lanes, the `splice_patterns` registry, and the abstract
  `SourceAdapter`. Each remaining file owns one source: `linq_fold_array` (array, zip,
  array-join), `linq_fold_decs`, `linq_fold_json`, `linq_fold_table`, and pugixml's
  `linq_fold_xml`. `linq_fold_sql` is the exception - it recognizes a `[sql_table]` source
  and hands the whole chain to the `_sql` macro, so it has no adapter and no emit. A source
  file requires `linq_fold_common` and never a sibling source.
- **`try_splice_patterns` tries recognizers in a fixed order** - sql, decs, xml (behind the
  pugixml `static_if`), json, table - and the array arm runs last with no recognizer of its
  own: it is what claims a chain nobody else claimed.
- **The adapter contract is four abstract methods; everything else on the base has a
  default.** `bind_name`, `element_type`, `wrap_source_loop` and `wrap_invoke` are abstract,
  so every source answers them. The per-operation hooks (`emit_loop_or_count`,
  `emit_reverse_skip_into_tail`, `emit_reverse_last_backward`, `emit_distinct_take_loop`,
  `build_group_by_adapter`, `emit_join_hook`) return null on the base, and a null sends the
  caller down its generic path. The capability methods (`can_group_by`, `can_join`,
  `can_reserve_by_length`, `has_own_loop_or_count_lane`, `supports_direct_return`,
  `effective_dispatch`, `defers_materialization`, `count_shortcut`, `invoke_param_type`,
  `name_prefix`) are the source answering questions about itself. daslang classes have no
  `is`/`as` downcast, so an emit fn holding a `SourceAdapter?` reaches source-specific
  behavior only through these methods - there is no place to write a per-source branch, and
  adding a source needs no edit to a central enum or switch.
- **The capability defaults are fail-closed, and `supports_direct_return` is the one that
  matters.** A wrong `true` emits a mid-loop `return` that escapes a nested-callback walk
  (decs's `for_each_archetype`); a wrong `false` only costs the slower state-var +
  find-stop + tail path.
- **`loop_source_expr` / `loop_source_name` is what lights up the array-shaped lanes.** The
  shared lanes spell their loop and their `length(...)` reads with the name, and read the
  expr for compile-time facts - the `type_has_length` reserve gate and the join's srcA
  element-type check. Array and table override both, xml and json override the name only,
  decs overrides neither.
- **`ProjectedSourceAdapter` absorbs a leading source projection.** When a row captures the
  optional `srcsel` slot (`source |> _select(f) |> order/distinct/take`), the dispatcher
  wraps the real adapter in this decorator: it binds `projName = f(rawElem)` on top of the
  per-element body and delegates loop and invoke to the inner adapter, so every emit sees the
  projected element unchanged. It leaves `loop_source_expr` / `loop_source_name` /
  `can_reserve_by_length` at the base defaults, which disables the source-direct fast paths -
  those would bypass the projection.
- **Rows are per operator family, not per source.** The seven `build_*_rows` fns
  (order_family, loop_or_count, reverse, distinct, group_by, join, zip) emplace 17 rows into
  the one `splice_patterns` table; only zip's builder lives outside `linq_fold_common`.
  Source gating happens inside a row - through a `RequiresPredicate` that asks the adapter
  (`can_group_by_source`, `can_join_source`, `array_source`, `non_array_source`) or through
  a hook that returns null. Registration order is match priority: the dispatcher walks the
  table top to bottom and the first row whose chain matches and whose emit returns non-null
  wins, so more specific chains are declared first. A row whose slot chain is a strict prefix
  of a later row's shadows that row (`chain_prefix_of`).
- **The walker consumes the whole call list or fails.** `match_pattern` walks the flattened
  calls left to right against the row's slots: `c_one` must match and advances both cursors,
  `c_opt` advances both on a match and skips the slot otherwise, `c_chain` matches greedily
  while the call is in the slot's name set and always succeeds - including with zero matches,
  which still creates the (empty) `captures.many` entry, so an emit fn can rely on the key
  existing. Any call left unconsumed after the last slot is a no-match. The `requires`
  predicates run last, against the populated captures.
- **`Captures.single_name` exists because a pre-pass renames the op without touching the
  AST.** `normalize_order_reverse` folds a trailing `reverse` into the order op by swapping
  the tuple's `LinqCall` record, leaving the `ExprCall` alone; deriving the name back from
  the `ExprCall` (`call_norm_name`) would silently undo the swap. The walker therefore
  records the `LinqCall.name` at match time, and any emit fn that cares about the
  post-normalize name reads `single_name`.
- **`alias_table` names the op-name groups a `m_alias` slot admits, and a missing name fails
  silently.** A terminator the lanes handle but `loop_terminator_family` omits never matches
  the terminator slot, so the chain leaves the splice and lands on the tier-2 cascade with no
  diagnostic - the symptom is a slow chain, not an error.
- **Predicates and emit fns are named module-level functions, referenced as
  `@@<RequiresPredicate> name` / `@@<EmitFn> name`.** An anonymous `@@(...)` lambda compiles
  to a `_localfunction_*` symbol the LLVM JIT pass cannot resolve; a named function has a
  stable address.
- **The table lane's key and value binds are asymmetric on purpose.** `keys(tab)` yields
  non-const elements (writable temp copies), so the key lanes rebind the loop var through a
  `let` before the body sees it; `values(tab)` over the const table param already yields
  `V& const` and binds directly. Keys are workhorse types, so the copy is free, and
  downstream `==const` composition (a `push_clone` of a bare projected key) needs the const.
