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
    errors kept verbatim. Gate (settled at landing: commit-as-is, world fully dark):
    tests-cpp parse-shape + mangled round-trip suites green. The original "dastest still
    runs -> per-test inventory" gate proved unmeetable: daslib generic SIGNATURES with
    `[]` (builtin.das bulk push/to_array_move family) are parser-produced FA now, while
    make-array literal ARG types still come from infer (dim-vector until 1d) — FA-vs-dim-
    vector is structurally unequal by design, every `.das_module` initialize meets it at
    `to_array_move`, so daslang.exe cannot boot the in-repo project until 1d. Class-level
    burndown instead: (1) FA generic signatures vs infer-made dim-vector args — module
    load/startup, 1d (make-array flip + FA rides firstType recursion; half deleted at
    Stage 4); (2) concrete FA declarations through infer (`let x : int[5]`) — 1d;
    (3) typeFactory/interop C-array types — 1c; (4) FA simulate lowering — 1e. gen1
    grammar fact recorded by the suite: `int[3][]` is a syntax error on master too
    (dim_list shift preference), so the push-at-end arm only composes via `int[]`/
    `int[][]`/`int[][3]`.
- **1c** typeFactory / interop (settled at landing). `typeFactory<TDim<TT,size>>` and
  `typeFactory<TT[dim]>` wrap FA via the shared `makeFixedArrayTypeDecl` header helper
  (hoists the canonical ref/const/temporary trio off the element); `isNativeDim` moves
  onto the FA node (only consumer is aot_cpp.das reading the indexed type's head).
  `makeArgumentType` needs no edit — composes via the FA-aware isRefType. SETTLED: the
  natural recursion fixes the latent multi-dim order bug (old: C `int[3][4]` -> dim
  [4,3] = das int[4][3], wrong row stride; believed unexercised in-tree) — fix taken,
  pinned by tests. PULLED FORWARD from 1f by necessity: the `makeTypeInfo` FA-flatten
  arm (typeFactory feeds handled-struct FIELD types and builtin signatures whose
  runtime TypeInfo must keep the exact flattened shape — without it a C-array field
  silently produces dimSize=0 TypeInfo). ManagedVector's scratch dim node
  (ast_handle.h walk) flips with it. Builtin mangled names unchanged (unaliased FA
  chains byte-identical); semantic hashes shift — moot while dark.
- **1d** Infer (sub-plan settled): inferAlias/inferPartialAliases gain the tFixedArray
  recursion arm (WRAP, never concatenate); SETTLED: on alias resolution the typedef name
  is kept as display label ONLY when the resolved type is an FA chain (M4 design; non-FA
  aliases keep today's clear — zero display churn elsewhere). inferGenericType gains the
  FA arm (fixedDim match, dimAuto wildcard, firstType recursion; plain auto(TT) binds the
  whole chain via the existing clone path). updateAliasMap needs NO edit — auto(TT)[]
  rides the firstType recursion; the dim.clear() root-cause line is a dead no-op.
  applyAutoContracts gains the FA recursion arm; the removeDim peel moves to
  pointer-returning callers (can't reseat through const TypeDeclPtr& in-place).
  moreSpecialized ranks FA above plain auto. dimConst resolution per FA node
  (inferTypeExpr walks the chain, evals fixedDimExpr); verifyType walks the chain.
  ExprAt/ExprSafeAt/for-loop peel = the existing tArray pattern (clone element, ref=true,
  constant|=head). ExprNew rebuilds the chain around the pointer node. typeinfo is_dim/
  dim read isFixedArray/head fixedDim; baseType-gated dim.size()==0 checks need no edit.
  Make-literal result typing wraps via makeFixedArrayTypeDecl (~8 sites + generate).
  SEQUENCING (settled): 1d+1e land as a two-commit train pushed together — 1d compiles
  FA but can't run it (simulate still reads dim vectors); boot gate applies at the
  train's tip: tests-cpp full green (the int[5] known-red flips), daslang boots, dastest
  runs -> per-test inventory becomes the 1f burndown.
  IMPLEMENTED. Equivalence rule that drove the edit set: dim-GATED classifiers
  (isStructure/isVariant/isString/isPointer/isVoid/isWorkhorseType/isClass-via-isStructure)
  return false on the FA wrapper exactly as they did on a dim'd node — call sites need NO
  edit; raw `baseType==`/`structType`/`annotation` reads DID see through a dim'd node, so
  those need the two-line element walk (make-struct/-variant/-array visitors, escape
  analysis ascend branch, needAvoidNullPtr allowDim). Known intentional divergence:
  `[[EnumT[2]]]` (isEnumT ignored dim, master collapsed to a single const — FA keeps the
  array). Two latent master bugs fixed by the natural wrap: gen2 fixed_array of an
  already-dim'd element appended the result dim INNER-first (same family as the 1c
  typeFactory order fix), and makeStructWhereBlock double-appended dim when makeType
  carried an explicit one. ExprNew dead dim-copy remnants removed.
  Gates: build green; tests-cpp 55/56 — the one red (int[5]) now passes COMPILE and
  throws in simulate, i.e. the failure crossed the 1d/1e boundary on schedule.
- **1e** Simulate lowering: ExprAt/iterator SimNode emission computed from nested type
  (same SimNodes), the fakeVariable flatten hack (ast_simulate.cpp ~:949), ExprNew dims.
  Note: gen2 `new Foo[3]` parses as `(new Foo)[3]` (pointer index), NOT new-dim — the
  ExprNew-with-dim / das_new_dim path is reachable via other routes (gen1 et al.);
  map its actual reachability during this sub-stage before porting it.
  IMPLEMENTED. ast_simulate.cpp: make-variant/make-struct lowering element walks
  (findArgumentIndex/getVariantFieldOffset/structType/annotation through the wrapper;
  getStride needs NO walk — FA head getStride == old dim'd getStride), fakeVariable
  wrap, ExprAscend/ExprNew dispatch walks + pointee-size via pointer-node walk,
  sv_trySimulate_At/ExprSafeAt ranges from fixedDim, for-loop fixedSize + iterator arm.
  ExprDelete needs NO edit (infer routes FA delete to finalize_dim; the total==1 assert
  proves dim'd never reached it). Boot-gate fallout fixed en route, beyond ast_simulate:
  (1) expect_dim contract + sort transformCall in src/builtin (dim reads);
  (2) isAliasOrA2A/applyRefToRef/collectAliasList/isAotAlias were MISSING their FA arms
  (1a gap — isAlias() false on FA-of-alias made ExprCast never resolve `TT -const[N]`);
  (3) inferAlias FA arm now applies+clears the head's hoisted remove* contracts (the
  dim'd alias leaf used to do this; without it `TT -const[N]` kept the contract flag and
  isSameType failed);
  (4) AST-GC HAZARD (new failure class for the whole rework): a fresh node held only in
  a C++ local across inferFunctionCall gets collected (proven via cdb: ptrType freed,
  baadf00d). Discipline: derive scratch pointers from rooted slots AFTER such calls,
  never capture across them. Second instance: gc_local frees ONLY the head — TypeDecl's
  copy ctor deep-clones children, so guarded deep clones leak their subtree (makeTypeInfo
  FA-flatten arm now uses a SHALLOW borrow scratch; ManagedVector scratch got an element
  guard, at master parity);
  (5) updateAliasMap "dead no-op" claim was WRONG for bare `auto(TT)` matched against an
  FA-typed pass — actually surfaced via daslib: ast_boost walk_and_convert_array/table
  still BUILT dim-vector types via the das-side `.dim` push — they now wrap via the new
  public ast_boost helper `make_fixed_array_type` (canonical hoist); walk_and_convert
  dispatches on tFixedArray; fixedDim/fixedDimExpr exposed on the das TypeDecl (pulled
  forward from 1f by necessity — dastest→clargs→`$v` hits this path at boot).
  Gates: tests-cpp 56/56 (int[5] flipped), daslang boots, tests/fixed_array 86/86,
  zero GC leaks; full-tree dastest inventory = the 1f burndown list.
- **1f** debug-info helper flatten; AST serializer FA fields (payload side + version bump
  landed at 1b-i); module_builtin_ast bindings: expose new fields + extend the `.dim`/
  `.dimExpr` compat properties with the flattened-array view (payload side landed at 1b-i).
Inference semantics flip in this stage; fallout fixes in tests/daslib land here.
Exit: full CI green with aot_cpp.das / llvm_jit.das / macro daslib UNMODIFIED (riding compat).
  **1f IMPLEMENTED.** The 11-file burndown collapsed into four classes:
  (1) GENERIC ALIAS DIM SEMANTICS (PUGIXML/json/decs) — settled BY BORIS, REVISED once:
  **NATURAL semantics. A generic alias binds the WHOLE matched type**: `auto(TT)` ←
  `int[4]` makes `TT = int[4]`; `TT[2]` = `int[2][4]` (array of 2 TT, natural nesting);
  `array<auto(TT)>` ← `array<int[3]>` makes `TT = int[3]` (inexpressible on master);
  `def two(a : auto(TT); b : TT)` accepts `(int[3], int[3])`. Module typedefs append as
  always. THE OLD (master/flattened-world) RULE IS DELIBERATELY DEAD: master stripped
  the bound dims at every use-site (bare `TT` ← `int[3]` was the ELEMENT `int`; `TT[4]`
  REPLACED to `int[4]`; strip applied even through `array<auto(TT)>`) — an artifact of
  one node carrying both element and dims; Boris: wrong, makes generics clumsy and
  unsupportable. (A master-compat peel was implemented first and reverted same-day —
  the probe matrix in D:/Work/fa_scratch/alias_dim_probe*.das documents both worlds.)
  IMPORTANT INVARIANT that keeps most of daslib working unchanged: `auto(TT)[]` (the
  explicit `[]` suffix) still binds TT to the ELEMENT — the `[]` eats one dim level in
  matching. So builtin.das clone/to_array/table-`[]` families and decs get_ro's
  `TT[typeinfo dim(value)]` reconstruct stay correct. Ported to the new binding: the
  bare-`auto(TT)` dim arms — json_boost from_JV (`var ret : TT -const -&`), PUGIXML
  from_XML (same), decs decs_array/get/get_default_ro/get_component/ComponentMap-get/
  make_callbacks/make_component (every `static_if is_dim` arm collapsed into its else
  arm — visibly simpler, the point of the flip). The FA-recursion arm in inferAlias
  HOISTS the resolved element's ref/const/temporary to the chain head (canonical form).
  EXTERNAL BREAKAGE accepted: code using the `TT[typeinfo dim(x)]` reconstruct on bare
  `auto(TT)` params breaks loudly at compile; bodies using bare `TT` as the element type
  change meaning silently — test suites are the net.
  (2) DASLIB `.dim` READERS (match tests, stbimage→is_local, aot_cpp): the das `.dim`
  binding is now a READ-ONLY compat property (`TypeDecl::dimCompat`) returning the
  flattened outermost-first sizes of the FA chain, recomputed on read into a per-node
  transient member (`dimCompatCache` — NOT the legacy `dim` field: warming that would
  nondeterministically revive dead C++ dim-vector loops on FA nodes). 59 daslib read
  sites ride it unmodified. das-side WRITERS break at compile by design and were ported
  now (the forced exceptions to "daslib unmodified"): match.das pop→`clone_type(what
  .firstType)`, tests/typemacro fixture + tutorials/macros twin →`make_fixed_array_type`,
  dasGlsl produce_zero, dasLLVM llvm_jit×3 dim-clear→element-walk (+ the `new Handle[N]`
  tHandle gate now walks the chain; LLVM_JIT_CODEGEN_VERSION bumped 0x22→0x23).
  (3) aot_cpp had FOUR FA gaps (all red on the 1d+1e train's AOT lane too):
  describeCppTypeEx PANICKED on FA nodes (`das_to_cppString` "Missed type tFixedArray")
  and emission SWALLOWS macro panics — struct fields/local decls silently vanish from
  generated C++; dim'd make-VARIANT field emission dropped its `das_get_variant_field::
  set` wrapper (find_argument_index on the chain head); dim'd make-STRUCT tHandle gate +
  get_tuple/variant_field helpers same class; dim'd ExprNew emitted the POINTER type
  where das_new_dim<> wants the POINTEE (TDim<T**> mismatch) and its isHandle/smartPtr
  reads missed (das_new_dim_handle silently mis-branched to das_new_dim). Fix: dims emit
  from the head's `.dim` view, all structural dispatch walks to the chain element via a
  new `fa_element` clone-walk helper (clone-walks so the result is non-const whatever
  flavor the caller holds — bare field-walks inherit const and break downstream calls).
  TDim<TDim<float,3>,2> nesting verified; full test_aot regen+compile+run green.
  BUILD GOTCHA: genaot custom-commands and the consuming compile are separate vcxprojs —
  when emitter output CHANGES, one `--target test_aot` pass can compile stale outputs;
  run the build twice (second pass converges). Plus MSBuild skipped relinking
  test_aot.exe after a deleted-output (stale .tlog) — delete build/test_aot.dir/Release/
  test_aot.tlog to force.
  (4) invalid_types ERROR CODES: verifyType's FA too-big check now multiplies element
  COUNTS across the chain (master parity: 32767² passes infer; lint reports byte-size
  with site-specific exceeds_* codes). Plus decs.das get_ro declared its dim'd return as
  `TT[typeinfo sizeof(value)]` (bytes!) — worked on master only because an unresolvable
  dim-expr in a generic result type was silently discarded in favor of the body-inferred
  type; fixed to `typeinfo dim(value)`.
  KNOWN GAP (deliberate): `-[]` (removeDim) on FA-typed generic params still erases the
  legacy vector only — zero in-tree users; revisit at Stage 4/5.
  Gates (re-run after the natural-semantics flip): probe matrix = natural binding
  everywhere, all 11 burndown files green, tests-cpp 56/56, tests/fixed_array 86/86,
  full-tree dastest 10784/10784 interpreted + 10123/10123 under test_aot -use-aot,
  zero GC leaks both modes, lint 0 errors on every changed file.

- **1g — container-FA `[]` overload deletion (Stage 4's builtin.das half, pulled forward
  per Boris's review note on the 1d–1f train: "bunch of other ones, same 'identical'
  flavor"). IMPLEMENTED.** Deleted the 22 dedicated `[]` overloads in builtin.das where
  the FA lives in a CONTAINER's element/value slot — natural binding makes the base
  generics cover them with byte-identical bodies: array<T[N]> push x2 / emplace /
  push_clone x2; table<K;V[N]> get x4 / get_value / insert x2 / insert_clone x2 /
  emplace / values x2 / get_key / clone x2; one-arg clone x2. Resolution check: PRE-
  deletion the dedicated forms still out-specialized the naturally-matching base (no
  ambiguity); post-deletion the base takes over. KEPT (C-array-as-subject, the `[]` IS
  the API): each / finalize_dim / clone_dim / sort / stable_sort / find_index(_if) /
  subarray / to_array(_move) / to_table(_move) / strings_boost join / decs get_ro /
  llvm_boost array_data_ptr. archive.das: the 6 stacked [][]..[] serialize overloads
  collapsed to ONE natural recursion (auto(TT)[] peels a level per call); bulk-memcpy
  gate = `is_raw && (is_workhorse || is_dim)` — exact wire-format parity incl. master's
  quirk (1-D raw STRUCT arrays go element-wise, 2-D+ memcpy), and >6-D now works.
  CAPABILITY GAINS: push_clone of clone-only rows (array<int>[2]) — the dedicated form
  gated can_copy and concept_assert'd; its const-src sibling's body was un-instantiable
  rot (pushed scalar elements into FA slots); at-index push of FA rows; size mismatch
  is now a no-match type error instead of concept_assert.
  `default<T[N]>` FIXED (was broken on master too, just unreachable — TT never bound
  to FA): ExprMakeStruct with 0 provided elements on an FA makeType now means zero-init
  the whole declared shape, any depth. Two sites: infer dimension check tolerates 0
  (ast_infer_type_make.cpp ~:537), simulate zero-fill uses getSizeOf() when total==0
  (ast_simulate.cpp simulateExprMakeStruct — stride is ONE OUTER ELEMENT and
  under-zeroed). AOT side already correct (das_zero on the whole TDim).
  Target spec ENABLED: `_target_inference.das` → `test_target_inference.das` (expect
  line dropped; picked up by the AOT glob automatically). Settled decoration: plain
  `auto(TT)` ← int[4] binds `int const[4]` (param constness rides TT); `auto(TT)[]` ←
  float[4][4] binds `float[4]` UNQUALIFIED — the `[]` consumed the chain head where
  qualifiers live. Asymmetric but harmless (generics strip with -const anyway).
  get_key subtest is in (Stage 4 gate satisfied); note get_key is ADDRESS-based — the
  value must reference the table slot (values() iteration), not a local copy.

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
~~Delete the `[]` workaround families in builtin.das~~ + ~~get_key subtest enablement~~ —
DONE EARLY in 1g (see above), archive.das collapse included. Remaining here: generalize
`each`/`sort`/`find_index`/`subarray`/`finalize_dim` (multi-dim/typedef'd sources through
one overload where natural nesting allows). Un-reject fixed arrays in apply.das /
contracts.das where they now just work.
Exit: new multi-dim/typedef'd-array tests pass through the generic stdlib; LOC visibly
negative.

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
