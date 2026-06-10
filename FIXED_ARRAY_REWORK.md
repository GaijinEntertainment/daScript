# Fixed Array Rework — `tFixedArray` as a structural type

Master plan for replacing the flattened `dim`/`dimExpr` vectors on `TypeDecl` with a
structural `Type::tFixedArray` node, so fixed arrays resolve through the same recursive
machinery as every other container type. Living document on the
`bbatkin/fixed-array-type` branch; every stage is review-gated (plan first, then commits).

## Problem

`dim` is a qualifier vector bolted onto every `TypeDecl` instead of type structure.
Verified consequences on master:

```
take_plain(a : auto(TT))   <- int[4]      =>  TT = int          // array-ness silently stripped
take_dim1(a : auto(TT)[])  <- float[4][4] =>  NO MATCH          // dim.size() 1 != 2
take_dim1(a : auto(TT)[])  <- M4[10]      =>  NO MATCH          // M4=float[4][4] flattens to float[10][4][4]
var a : M4[3]              =>  a is float[3][4][4]              // M4 identity destroyed at declaration
a[0]                       =>  float[4][4]                      // anonymous; should be M4
```

Root causes:

- `updateAliasMap` clears `dim` when recording `auto(TT)` — dim sits in the qualifier set
  next to `constant`/`ref`/`temporary`, so the deduced alias lies about the type
  (body sees `int const[4]`, `TT` claims `int`, `default<TT>` gives `int`).
- `inferGenericType` requires exact dim-count match; `auto[]` can never peel one level off
  a multi-dim array because there is no "rest of the type" node to bind.
- Alias expansion concatenates dim vectors, destroying typedef boundaries irrecoverably.
- Every `isX()` predicate pays a `dim.size()==0` tax; daslib carries ~25 workaround
  overloads (`table<K; auto(valT)[]>` get/get_value/insert/insert_clone/emplace/values/get_key
  families, `array<auto(numT)[]>` push/emplace/push_clone siblings), each reconstructing the
  thrown-away type via `valT[typeinfo dim_table_value(Tab)]` — and covering only ONE `[]` level.
  `apply.das`/`contracts.das` simply reject fixed arrays.

Meanwhile `array<T>`, `table<K;V>`, pointers, iterators all infer correctly — they are
structural nodes with `firstType` recursion. Fixed arrays are the only container that isn't.

## Settled design

- **`Type::tFixedArray`**, appended at the END of the `Type` enum (existing values stay
  stable for serialized data). Element type in `firstType`.
- **Strict nesting, one size per node**, outermost first:
  - `float[4][4]` = `FA(4, FA(4, float))`
  - `M4[3]` (M4 = `float[4][4]`) = `FA(3, FA(4, FA(4, float)) alias="M4")`
  - Alias is a display label (same as `aka` today); identity stays plain structural
    recursion. One type, one shape — no flatten-canonicalization anywhere.
- **`TypeDecl` fields**: `dim` and `dimExpr` deleted. Added:
  - `int32_t fixedDim` — size; `dimAuto`/`dimConst` sentinels kept (`int[]` param =
    `FA(dimAuto, int)`); meaningful only on tFixedArray nodes
  - `ExpressionPtr fixedDimExpr` — size expression when `fixedDim == dimConst`
  - `vector<ExpressionPtr> typeMacroExpr` — typeMacro/typeDecl/tag payload, contents and
    conventions byte-identical to today's dimExpr usage (typeMacro: `[0]`=name string,
    `[1..]`=args, type-args arrive as `ExprTypeDecl`; typeDecl/tag: `[0]`=the expr).
    No typemacro argument-model redesign in this train.
  - Net ~−12B per node; every predicate drops the `dim.size()` branch.
- `isNativeDim` moves onto the FA node; `typeFactory<TT[dim]>` wraps instead of pushes.
- **Qualifier discipline (canonical form, settled at 1a)**: `constant`/`ref`/`temporary`
  live ONLY on the outermost FA node; inner chain nodes are bare — same rule as `tArray`
  elements. Peeling a level (`a[i]`, iteration, `auto(TT)[]`) = `firstType` plus the same
  constness propagation tArray element access uses. One type, one shape; Stage 1d enforces
  at construction sites, a debug assert in `isSameType` catches violations.
- `-[]` (`removeDim`) = unwrap one FA level; kept parsing, mostly obsolete.
- **New inference semantics (no compat shim)**:
  - `auto(TT)` <- `int[4]`        => `TT = int[4]` (whole array, consistent with `array<int>`)
  - `auto(TT)[]` <- `float[4][4]` => `TT = float[4]` (new match)
  - `auto(TT)[]` <- `M4[10]`      => `TT = M4` (alias preserved on the node)
  - `a[0]` on `M4[3]`             => `M4`
  - Overload ranking must keep `auto[]` more specific than plain `auto` so existing call
    sites don't go ambiguous.
- **Runtime `TypeInfo` stays flattened forever** — `dim`/`dimSize` describe memory layout,
  and the flattened form IS the layout. Single AST->runtime conversion point
  (`ast_debug_info_helper.cpp`) walks the nested chain and emits `dim[]={3,4,4}` exactly as
  today. GC walker, data walker, debug print, JIT TypeInfo globals, `daslib/rtti` untouched.
- **Mangled name: natural recursion (settled at 1a)**. Each FA node emits its own
  qualifiers + `[d]` + alias slot, recursing into `firstType`; parse rebuilds the exact
  structure including alias placement (clean round-trip). Unaliased chains — the
  overwhelming majority, including every C++ binding — stay byte-identical to master
  (`[3][4][4]f`). Mid-chain aliases shift the `Y<>` slot to the level it labels
  (`[3][4]Y<M4>[4]f` vs master's flattened `[3][4][4]Y<M4>f`); the already-planned
  one-time semantic-hash / AST-serializer / `LLVM_JIT_CODEGEN_VERSION` bumps cover that
  churn. Text is self-consistent within a build and stable going forward.
- **Same SimNodes emitted** — interpreter/runtime untouched; stride/count computed from the
  nested type instead of the dim vector.
- **das macro API**: during migration, read-only computed `.dim`/`.dimExpr` compat
  properties (flattened view) keep aot_cpp.das / llvm_jit.das / macros green while they
  port to the structural API (`baseType==tFixedArray`, `.fixedDim`, `.fixedDimExpr`,
  `.firstType`, `.typeMacroExpr`). Compat properties are DELETED after the sweep
  (optionally keep a `dims(td)` helper in ast_boost for code that wants the flattened list).
  The compat `.dimExpr` property must be **baseType-aware** — on typeMacro/typeDecl/tag
  nodes it returns `typeMacroExpr` (so typemacro_boost works unmodified until its Stage-5
  rename); on tFixedArray nodes it returns the size-expr view. "Whatever dimExpr used to
  hold for this baseType."
- Parser keeps the existing `MyMacro(...)[N]` / `typedecl(...)[N]` errors (behavior-
  preserving); lifting them becomes representable and is a separate later decision.

## Stages

Each stage: plan discussion -> implementation -> commit review. CI + /test green at every gate.

### Stage 0 — Characterization & target tests
Pin current must-not-change behavior as a dastest suite (`tests/fixed_array/`) run against
master semantics: layout (sizeof/alignof/stride/field offsets incl. C++ handled types,
tuples, variants with FA fields), copy/clone/init semantics, indexing + bounds, iteration,
`table<K;V[]>` / `array<T[]>` operations, serialization round-trip, AOT, `typeinfo dim`
traits, current overload preference (`auto` vs `auto[]` with FA arg). Plus the
TARGET-behavior tests (initially disabled via leading-`_` filenames) encoding the new
inference semantics and M4 preservation.
Also `tests/typemacro/test_basic.das` — direct coverage of the typeMacro payload surface
that Stage 1b migrates (all four grammar forms; `typemacro_argument` const-extraction for
int/bool/string; `typeMacroName`; `typedecl(expr)`). Deep indirect coverage already exists
via tests/option + tests/hash_map + tests/delegate (Option/Result/hash tables are
typemacro_boost-based), but the raw grammar forms and const-arg extraction had none.
Known-broken-on-master, expected fixed by this rework (target spec, NOT characterization):
`get_key(table<K;V[]>, v)` fails to resolve its own `valT const[-2]` contortion
(builtin.das:1407).
Exit: suite green on the branch point; target tests reviewed as the spec.

### Stage 1 — Core representation flip
The indivisible piece, sub-staged for review:
- **1a** (settled: ADDITIVE, byte-neutral, full CI green standalone) — `Type::tFixedArray`
  appended + new TypeDecl fields (`fixedDim`/`fixedDimExpr`/`typeMacroExpr`, dormant) +
  predicates + clone/visit/gc + describe + mangled name EMIT + `isSameType` +
  semantic/lookup hash + getSizeOf/stride/align family. Everything gated on
  `baseType==tFixedArray`; nothing produces FA nodes yet; `dim`/`dimExpr` stay live until
  end of Stage 1. Dead arms proven by a hand-built-nodes tests-cpp doctest suite asserting
  FA-vs-old-node equivalence (text, sizes, identity, clone, gc, hashes). Mangled-name
  PARSE flip deferred to 1b — `[N]` text is identical in both worlds, so the parser must
  build whichever representation the program uses, and that flips with the world.
- **1b** (settled: TWO commits) Parsers (ds2 + ds1 + parser_impl) + the payload move.
  - **1b-i — payload move, CI-green standalone.** typeMacro/typeDecl/tag payload moves
    `dimExpr` -> `typeMacroExpr` atomically: parser writes, all C++ reads (typeMacroName,
    describe, mangled emit, moreSpecialized, inferPartialAliases, inferTypeExpr, validator),
    PLUS two pieces pulled forward from 1f because atomicity forces them: the das-side
    `.dimExpr` binding becomes a read-only compat property ("whatever dimExpr used to hold
    for this node": non-empty `typeMacroExpr` wins, covers the tag payload on autoinfer
    nodes), and the AST serializer carries `typeMacroExpr` (+version bump). typemacro_boost /
    clargs / ast_match / templates_boost keep working unmodified. Mangled text byte-identical.
  - **1b-ii — FA parser flip, breaks the FA world.** `appendDimExpr` becomes an FA-chain
    builder + `attachDimChain` (attach element bare at chain end; hoist the qualifier flags
    the old world fused onto the dim-carrying node — const/ref/temporary, their remove*,
    implicit/explicitConst/explicitRef/isExplicit/autoToAlias — to the chain head; `alias`
    NEVER hoists: at parse it is the unresolved name on the element). dim_list/splice arms
    in both grammars, gen1's push-at-end `[]` arm and `{{ }}` synthesized `auto[]`, mangled
    PARSE `case '['` builds FA — with the rule that a `Y<name>` immediately following `[d]`
    labels THAT FA node (known cosmetic asymmetry: `[3]Y<I>i` re-parses with the label on
    the FA node even if it was on the element; structural identity unaffected). Grammar
    errors kept verbatim. Gate: tests-cpp parse-shape + mangled round-trip suites green;
    dastest framework still runs (concrete FA in daslib is only decs/faker/profiler/regex)
    and its FA-failure inventory becomes the 1c/1d/1e burndown list.
- **1c** typeFactory / interop (`TT[dim]`, `TDim<>`, `isNativeDim`, makeArgumentType,
  ast_handle).
- **1d** Infer: inferAlias (WRAP, don't concatenate — alias label preserved),
  inferGenericType / updateAliasMap / applyAutoContracts (dim arms deleted; FA rides the
  firstType recursion), ExprAt/ExprSafeAt result typing, for-loop sources, make-array /
  initializers, inferTypeExpr (dimConst resolution per node), variable checks.
- **1e** Simulate lowering: ExprAt/iterator SimNode emission computed from nested type
  (same SimNodes), the fakeVariable flatten hack (ast_simulate.cpp ~:949), ExprNew dims.
  Note: gen2 `new Foo[3]` parses as `(new Foo)[3]` (pointer index), NOT new-dim — the
  ExprNew-with-dim / das_new_dim path is reachable via other routes (gen1 et al.);
  map its actual reachability during this sub-stage before porting it.
- **1f** debug-info helper flatten; AST serializer FA fields (payload side + version bump
  landed at 1b-i); module_builtin_ast bindings: expose new fields + extend the `.dim`/
  `.dimExpr` compat properties with the flattened-array view (payload side landed at 1b-i).
Inference semantics flip in this stage; fallout fixes in tests/daslib land here.
Exit: full CI green with aot_cpp.das / llvm_jit.das / macro daslib UNMODIFIED (riding compat).

### Stage 2 — AOT emitter
`daslib/aot_cpp.das` ports to the structural API; nested `TDim` emission becomes natural
recursion; AOT version bump. Also fixes issue #3077 (iterate/pass/copy of a NATIVE C-array
field emits non-compiling C++ — found by Stage 0, test_interop.das AOT-excluded since) if
not fixed earlier; re-include test_interop in the AOT glob to verify.
Exit: tests/aot + AOT CI green, test_interop back in the AOT set.

### Stage 3 — JIT
`modules/dasLLVM/daslib/llvm_jit.das` port (element type, bounds checks, loop ranges from
nesting; TypeInfo globals unchanged — flattened); bump `LLVM_JIT_CODEGEN_VERSION`.
Exit: JIT tests green.

### Stage 4 — daslib payoff
Delete the `[]` workaround families in builtin.das (table get x4 / get_value / insert x2 /
insert_clone x2 / emplace / values x2 / get_key; array push/emplace/push_clone siblings);
verify base generics cover them. Generalize `each`/`sort`/`find_index`/`subarray`/`clone`/
`finalize_dim`. Un-reject fixed arrays in apply.das / contracts.das where they now just work.
The broken-on-master get_key(table<K;V[N]>) overload is NOT fixed in place (it gets deleted
here) — the get_key target subtest in tests/fixed_array stays disabled until THIS stage and
its enablement is part of this stage's exit gate.
Exit: new multi-dim/typedef'd-array tests pass through the generic stdlib (incl. get_key on
FA values); LOC visibly negative.

### Stage 5 — Macro/tooling ports
ast_match.das (structural FA matching), match.das, typemacro_boost (field rename), lints
(perf_lint/style_lint), is_local, templates_boost, decs_boost, clargs, rst.das; MCP
describe_type sanity.
Exit: no in-tree consumers of the compat properties left.

### Stage 6 — Externals + compat removal
Sweep D:\DASPKG modules (dasImgui, dasSQLITE, ...); delete `.dim`/`.dimExpr` compat
properties; optional `dims(td)` helper lands in ast_boost.
Exit: grep-clean everywhere.

### Stage 7 — Docs + merge
RST (type system, generic programming), CHANGELOG, version bump; final full-matrix CI +
AOT + JIT verification; merge to master. Optional separate decision: lift the
`MyMacro(...)[N]` parser restriction.

THE THIRD PARSER (settled: very last item, after everything else is done): fix
`utils/dasFormatter/ds_parser.ypp` — the gen1->gen2 conversion util vendors its own copy
of the gen1 grammar, writing the real `TypeDecl::dim`/`dimExpr` through the same 10
site-shapes as the in-tree gen1 parser. Caveat: deleting dim/dimExpr at the end of
Stage 1 breaks its COMPILE, so the field-deletion commit carries the minimal mechanical
flip (same edit as the in-tree gen1 parser got in 1b — by then the helpers exist);
the full port/verification of the converter is what lands here at the end.

## Standing risk ledger (checked at every stage gate)

1. **const/ref/temporary propagation through FA levels** — the subtle-bug budget; audited
   with targeted tests per stage.
2. **Layout invariance** — Stage 0 is the gate; any diff is a stop-the-line bug
   (C++ interop depends on identical layout).
3. **Overload-resolution shifts** — new `auto[]` matches that didn't exist before;
   explicit ranking rule + tests.
4. Known generic-instance mangling collisions (error 50609) may surface new shapes.
5. AOT hash desync / serialization — version bumps; skills/aot_hash_desync_debugging.md.
6. gen1 parser (`ds_parser.ypp`) parity with gen2.
