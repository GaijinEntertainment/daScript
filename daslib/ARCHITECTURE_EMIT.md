# daslib architecture notes - emission: AOT C++, standalone contexts, shaders

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

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
- **`fromExtraDependency` describes the host process, not the program being emitted.** The
  flag is set when the host first loaded a shared module as an extra dependency (a `-jit`
  host loads `strings`, `fio_core` and `math` that way) and stays on the shared module for
  every later compile in the process. `getRequiredModulesFor` skips such modules only on the
  regular AOT path; a standalone context decides by its linked set alone, otherwise a
  generator running under a `-jit` host (the JIT test lane) prunes the modules the program
  calls.
- **`DEFAULT_MODULE_ORDER` mirrors `register_builtin_modules_impl`** in
  `src/builtin/modules.cpp` - the same builtin C++ modules in the same order, a
  daslib-to-C++ pair `tests/aot/test_standalone_emit.das` reads back from the C++ source and
  pins. A standalone context registers these first because
  module constructors `Module::require` earlier ones by name (`fio_core` takes `strings`,
  dasHV takes `rtti_core`). A module missing from the daslib list still registers, only in
  the dependencies-first pass that follows; a module added to the C++ side joins the list.

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
- **The function table is dense** - `addFunctionInfo` numbers the emitted rows in emission
  order and the ctor sizes `context.functions` by that count, not by the program's
  `totalFunctions`. The program count includes every function the interpreter simulated
  (externs among them), while the context emits only what it still reaches; a table sized
  by the larger count leaves rows of uninitialized memory that the destructor's shutdown
  walk (`runShutdownScript`) reads. Nothing reads a function by program index: AOT calls
  are direct, function pointers resolve through `fnByMangledName`, and `FillFunction`
  matches rows by AOT hash.
- **A context links the modules the program reaches, and registers the C++ ones itself.**
  `aot_cpp.das`'s `collectLinkedModules` marks the module of every used function, of every
  function a used function or global calls (`UseTypeMarker` reads the callee of each call,
  operator and `@@` address - externs carry no `used` flag of their own), and of every
  struct, enum and handled type; a program that reaches no C++ module beyond the builtin one
  links exactly that set. A nano program reaches none by contract (every builtin module is
  absent there), so its output stays registry-free.
  Once it reaches one, the default C++ modules the compiler loaded and every linked C++
  module's `module_for_each_dependency` closure join the set, because module constructors
  `Module::require` those by name (dasHV takes `rtti_core` this way). The pruned modules
  (`compile time only, not linked`) get no `aotRequire` include and no registration.
  `standaloneModuleRegistration` orders the C++ subset and ranks it: `DEFAULT_MODULE_ORDER`
  first (the pair it mirrors is recorded in sec. 5), then the rest dependencies-first, ranked
  by dependency depth. Each generated TU emits its list as a `StandaloneModule` table added to
  the process-wide list before main, and the context class takes `StandaloneModuleScope`
  (`include/daScript/simulate/standalone_modules.h`, inline so every TU shares one copy) as
  its FIRST base, ahead of `Context`: constructed before it, destroyed after it. The first
  scope constructed registers the union, rank order, and initializes once; the last scope
  destroyed shuts down - inside main, after the last context's own teardown and while the
  runtime's statics still stand. A static destructor cannot do this: it runs after main,
  when statics constructed later than it (libhv's, the runtime's own) are already gone, and
  on Windows that shutdown corrupts the heap - the exit-time race the `-exe` path avoids by
  shutting down before main returns. Two contexts with different module sets in one binary
  therefore share one lifetime. A host that registered the builtin module owns the registry
  instead: the context registers nothing and stops the program, by name, on a linked module
  the host did not register - the runtime cannot add to an initialized registry and keep
  `Initialize`/`Shutdown` balanced, and a second shutdown would walk a registry the first
  one deleted. The shutdown is `Module::ShutdownStandalone`, never `Module::Shutdown`: the
  latter resets the fusion engine through a function pointer only the interpreter's
  `simulate_fusion.cpp` installs, and a standalone binary links no interpreter. Handled
  types need the registry because `TypeInfo::resolveAnnotation` walks the bound
  environment's module list; a context with none dereferences a null environment.
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

## 28. shader_block_layout

- **Two rails, deliberately separate** - the LAYOUT rail admits int64/uint64 as block
  members (`compute_block_layout` special-cases them) while the ARITHMETIC rail rejects
  64-bit INT (`arith_width_ok` allows width 64 only for floats); `cpu_only_lattice_width`
  keys both emitters' fail-closed diagnostic.

## 29. shader_lingua_franca {#shader-lingua-franca}

- **Every symbol is either an exact CPU mirror of its GPU semantics or a `[sideeffects]`
  dummy every rail lowers by name** - the dummies return zero on the host, so a CPU replay
  reproduces GPU semantics only for the real-bodied set. Unsigned overloads never fold into
  signed twins (glslang picks the unsigned opcode).
- **A width-variant of a lowered-by-name symbol is one more overload here, never an emitter
  arm.** `unpack8` carries `int16 -> byte2` and `uint16 -> ubyte2` beside the 32-bit pair; every
  overload is the same `reinterpret` on the host and the same single `OpBitcast` on the SPIR-V
  rail, so the emitter matches the name and reads the width off the operand type.
