# Temp-string reclaim for conversion calls

Make `string(x)`-style conversions (and eventually any fresh-string-returning function)
ride the existing 1-slot temp-string dispose queue, the way `ExprStringBuilder` already
does. Fixes the front-page float2string row (f2s retains ~130MB today; builder-direct
shape measures −36B vs +1.6MB for every conversion shape, 200k calls).

## Sizing (grep sweep 2026-08-08)

- 1503 `string(` call sites / 244 files total. Majority is macro/codegen/tooling
  (cbind_boost 56, sql_linq 34+41, templates_boost 27, rst 47, perf_lint 38, llvm_jit 45,
  msl_emit 37) — compile-time heat, but note lint/LSP/MCP are long-running processes, so
  string-heap flatness still pays there.
- **419 sites are the let-local form** `let s = string(...)` — this is what f2s does
  (`web/examples/ui/samples/examples/f2s.das:19`): NOT direct-arg. The motivating benchmark
  is only caught by the live-range phase (P3), not the direct-arg phase.
- ~57 sites are the direct-nested form `find(string(...))` / `length(string(...))` —
  mostly dasHV/dasllama-server tests (`find(string(resp.body), ...)`); a few daslib/utils.
  Many of those are `string(das_string)` clones = PERF007/PERF012 territory (drop the
  conversion entirely) — not counted as wrapper gain.
- Pipe form ~2, concat-operand form ~4 — noise.
- Hot-runtime candidates beyond f2s: dasLLAMA tokenizer/bpe/unicode (per-token),
  imgui_boost_v2 (per-frame labels, 15 let-form sites), dasTerminal, dasHV/dasllama servers.
- Manual rail `daslib/temp_strings.das` (callback + delete_string) predates this; the
  automatic wrapper subsumes most of its use cases — leave it alone, revisit later.

## Design (settled in session 2026-08-08)

- **1-slot queue stays** (`Context::stringDisposeQue`, `freeTempString` at simulate.h:427).
- The `tempString` argument on allocate/free notifications is telemetry-only — no
  allocation-path changes needed; a fresh heap string can be queued post-hoc.
- **Wrapper extern** `_temp_string_result(s : string) : string` = `freeTempString(s); return s`.
  Inserted as a REAL ExprCall in the AST → interp/AOT/JIT all compile it as an ordinary
  call, zero per-backend machinery. Fail-safe: flush goes through `freeString` →
  `isOwnPtr` check no-ops on literals/interned.
- **Function flag** (one bit next to `captureString`, ast.h:983): "my result is always a
  FRESH allocation (or null), never a passthrough of an input, never retained by me."
  NOT a SideEffects member — no effect-lattice machinery.
  - C++ externs: "I promise" at registration (same trust model as sideEffects).
  - das functions: COMPUTED, bottom-up over the call graph in the same pass that
    propagates captureString (ast_unused.cpp:476-520). Rule: flag iff every `return`
    expression is itself a fresh producer — flagged extern call, string builder, call to
    an already-flagged das fn, or ternary of qualifying branches. **No locals in v1**:
    `return s` never qualifies — that single restriction buys the no-retention property
    without escape analysis. Pessimistic on recursion/invoke/fn-pointers/unknown.
  - `[temp_string_result]` annotation = manual override, symmetric with `[capture_string]`.
  - Bind the bit in the das-side flags table (module_builtin_ast_flags.cpp:205, slot
    ordering discipline). Rides the existing flags word → serialization/shared-modules free.
- **CodeOfPolicies gate, like dse** (`ast.h:1610` neighborhood): `disable_temp_string_reclaim`,
  default-ON (false). **CORRECTION (P1 impl, 2026-08-07): wrappers ARE keyed to the
  persistent string heap after all** — Boris's original instinct was right and the plan's
  earlier counter-argument was factually wrong. The daslang CLI forces
  `policies.persistent_heap = true` (utils/daScript/main.cpp:433), so every measurement
  (−36B builder-direct, +1.6MB conversion shapes, f2s's 130MB) was taken on the persistent
  SHOE allocator, where free works anywhere and the freed cell is immediately reusable. On
  the default linear heap `HeapChunk::free` reclaims ONLY the top-of-chunk block, and the
  queue flush order (allocate new temp → free old temp) means the freed string is never on
  top — reclaim is structurally a no-op there. Insertion rule:
  `!disable_temp_string_reclaim && persistent_heap && !intern_strings` (intern no-ops in
  freeTempString at runtime anyway; skipping just saves the wrapper call). Builder temp
  MARKING stays unconditional on all heaps — existing behavior, harmless no-op frees.
- **One insertion pass, one scan, post-optimization** — runs just before allocate_stack
  (captureString is final by then; tree is stable; no optimizer interference with a
  side-effecting wrapper). It takes over the builder scan currently in
  ast_allocate_stack.cpp:604-620 (delete it there) so that the rightmost rule is GLOBAL:
  per consuming call (`!policyBased && !invoke && !captureString`), mark/wrap only the
  **rightmost eligible argument overall** — builder OR flagged call. Two scans would
  dangle: wrapper on arg0 conversion + builder temp on arg1 flushes arg0 before the call.
- **P1 FINDING (2026-08-07): the old per-call rightmost-builder rule was ALREADY unsound —
  a live silent miscompile on master.** Extern (interop) argument evaluation order is
  UNSPECIFIED (the templated pack expansion — right-to-left on MSVC; ast_allocate_stack's
  own "order of evaluation for interop functions is not specified" comment). So a nested
  builder in a SIBLING argument can queue after the marked builder and flush it while its
  pointer is already parked in the arg slot; the persistent shoe reissues the cell to the
  very next same-size allocation. Repro: `compare_ignore_case(to_upper("{b}"), "{a}")`
  compares a string against itself — **200,000 of 200,000 iterations corrupted** on the
  stock CLI. Fix (rides P1, all three backends, since it is marking-level): ONE queue site
  per consuming call, chosen rightmost-first among top-level-eligible args (builder /
  flagged call / existing wrapper), and site creation INHIBITED in every sibling argument
  subtree (`MarkTempStrings` in ast_allocate_stack.cpp — inhibit counter around
  preVisitCallArg/visitCallArg). Sites nested deeper in a SITELESS call's args stay legal:
  such a site's value dies inside its own arg subtree before any sibling can evaluate.
  Regression test: "nested builder cannot flush a sibling site" row in
  tests/strings/temp_string_reclaim.das.
- **Unifying invariant** (also enables P3): queue a fresh string at creation iff no other
  queue-site can execute while the value is still live. Rightmost-direct-arg is the
  degenerate case (live until enclosing call returns; nothing eligible evaluates after).

## Phases

**P0 — flag + wrapper + bindings.** Function bit, addExtern setter, `[temp_string_result]`
annotation, ast_flags table entry, `_temp_string_result` extern. No behavior change yet.

Flag list (impl-audited 2026-08-08, module_builtin_string.cpp + runtime_string.cpp):

FLAG — verified always-fresh-or-null:
- `string(number)`: das_lexical_cast_int_* ×8, das_lexical_cast_fp_f/d (runtime_string.cpp:210-260)
- strip / strip_left / strip_right (:127-150 — all allocate or return null; initial
  suspicion wrong)
- chop (:206), slice ×2 (:215/:224), reverse (:232)
- to_lower (:247), to_upper (:274) — trailing `stringHeap->intern(ret,len)` is a pure
  LOOKUP with discarded result (heap.cpp:239; no-op unless needIntern, registers nothing);
  intern-mode heaps no-op freeTempString anyway
- ltrim (:717) — always allocates (asymmetric with rtrim!)
- escape (:625), unescape (:630), safe_unescape (:638)
- to_char (:483), repeat (:489), string(array<uint8>) (:666)
- build_string (:443), to_cpp_float (:439)
- modify_data (:780) — fresh result; block sees a locked temp array view, can't retain
- format<T> — pending one impl check (likely fresh)

NEVER FLAG — passthrough paths verified:
- replace (:600 — `return (char*)str` when toSearch is empty)
- rtrim (:741 — returns INPUT when nothing trimmed), rtrim_ts (:754/:767), trim (:713 → rtrim)
- to_lower_in_place / to_upper_in_place (return input by design)
- reserve_string_buffer (:698 — allocates on the REGULAR heap, not the string heap)
(Boris ruled 2026-08-08: trims stay as-is — no always-allocate conversion; they remain
unflagged.)

AUDIT LATER (sweep-stage batches, same impl-by-impl discipline):
- module_builtin_runtime / misc: clone_string, string(das_string), das_root, …
- module_builtin_fio: base_name/dir_name/path_join/… — hot in tool loops, big win pool
- module_builtin_ast / rtti / debugger: describe*/mangled-name getters (macro-time, cold)
- external modules: dasHV header/body getters, dasLLAMA, dasImgui

**P1 — insertion pass, direct-arg rule.** Move the builder scan out of allocate_stack into
the new pass; extend eligibility to flagged calls; wrap via ExprCall insertion. Tests
(tests/strings/temp_string_reclaim.das, rides normal interp/jit/aot lanes):
- loop N × `consume(string(i))` → string-heap flat (string_heap_bytes_allocated / temp_string_size).
- mixed `foo(string(a), "{b}")` and `foo(string(a), string(b))` → only rightmost queued,
  values correct (the dangle case).
- `return string(x)` NOT wrapped; capturing consumer (table insert) NOT wrapped.
- intern-strings context: the gate skips wrapper insertion outright (freeTempString no-ops
  at runtime under intern anyway) — covered by the gate condition, no dedicated test file.
- builder behavior unchanged for single-site shapes (existing tests keep passing); the
  multi-site sibling shapes CHANGE deliberately — that was the miscompile.

**P2 — das-side propagation.** Return-expr rule in the captureString pass + annotation
override. Pilot: verify `format`/`build_string`-style daslib fns pick the flag up
transitively. Tests: `def f(x) => string(x)` consumed in loop → flat; passthrough
`def g(s : string) => s` NOT flagged; recursion/ternary cases.

**P3 — let-local live range (the f2s shape).** Conservative block-local analysis:
`let s = <flagged call>` where every use precedes the next potential queue-site in the
same block, no escape (no return/store/capture/addr/move/reassign; only const-string args
to non-capturing calls) → wrap the initializer. Catches `let str = string(s); use(str)`.
Tests: f2s shape flat heap; `let a = string(x); let b = string(y); use(a, b)` NOT wrapped
(intervening queue-site); escape shapes NOT wrapped.

**P4 — measure + ship the number.** f2s benchmark before/after locally (interp + jit +
aot), then the dasweb pipeline regenerates the front-page row. Opportunistic: check LSP/MCP
RSS over a long session.

P4 local numbers (2026-08-07, CLI = persistent heap, gate toggled via
`options disable_temp_string_reclaim`):
- **heap retained by f2s: 129,231,632 bytes → 8 bytes** (interp and JIT identical) — the
  front-page retention mystery is closed.
- time on the pure conversion loop: interp 0.108s → 0.121s (+12%), JIT 0.081s → 0.094s
  (+16%) — the wrapper interop call costs ~13ns/conversion. Native chunk growth is cheap,
  so natively the row trades time for memory. The PLAYGROUND verdict is still open: wasm
  memory.grow for 129MB is far more expensive than native mallocs, so the live row may
  still win time — measure there before deciding. Overhead-reduction candidates if needed:
  a `stringDisposeQueLen` slot beside the queue pointer (kills the per-flush strlen; Context
  layout/JIT-offset implications), or builder-style direct temp allocation for conversions
  (per-backend special machinery — previously rejected).
- builder-INIT let-locals (`let s = "{x}"`) are marked instead of wrapped — self-queueing,
  zero wrapper overhead. Only call-inits pay the wrapper.

**P5 — the SWEEP (planned agent fan-out, NOT a surprise).** Once machinery + pilot are
green, parallel **Opus** agents extend the flags and clean the ecosystem. We WILL touch
many files and lint errors WILL surface in them (whoever-stumbles-fixes) — that is
budgeted work, not a blocker discovered mid-PR.

Batches (non-overlapping file ownership, one Opus agent each):
1. C++ flag batches, each = audit-then-flag every string-returning extern in its files
   (the replace/rtrim passthrough check is THE step that cannot be skipped):
   a. module_builtin_runtime + misc_types remainder;  b. module_builtin_fio (path helpers —
   hot in tool loops);  c. module_builtin_ast + rtti + debugger;  d. external modules
   (dasHV response/header getters, dasLLAMA, dasImgui, dasTerminal).
2. das-side `[temp_string_result]` annotations in daslib (strings_boost format/join etc.) —
   but FIRST probe what the propagation already proves; annotate only actual gaps.
3. Site cleanups: the ~57 direct-arg `string(das_string)` hits = PERF007/PERF012 fixes
   (drop the conversion), mostly dasHV/dasllama-server tests.

Per-agent contract: edit → cpp_compile_check / compile_check → MCP lint on EVERY changed
.das (absolute paths, exact changed set) → MCP format_file → run the owning dir's tests →
report files touched + lint fixed + functions flagged/declined WITH the decline reason.
Declines (passthrough found) come back to the coordinator — they are findings, not
failures.

Verification that scales with the sweep: the tests/strings/temp_string_reclaim.das test is
TABLE-DRIVEN — every flagged function adds two rows: (a) transform case consumed in a loop
→ string heap flat; (b) identity-shaped input (nothing to strip/replace/convert) in temp
position → the SOURCE string is still intact afterwards. Row (b) is the passthrough
regression trap: it fails loudly if an impl later grows a `return str` shortcut.

Coordinator finishes: one full preflight + test_aot_subset, batched into the arc PR
(one PR per arc; sweep = final commit series, reviewed once at landing).

## Follow-up arc (Boris, 2026-08-08): concat-to-builder rewrite

Rewrite resolved builtin string+string `ExprOp2` into `ExprStringBuilder` post-infer
(`foo() + "bar"` → `"{foo()}bar"`), splicing nested builders so `(a+b)+c` collapses to one
flat builder = ONE allocation instead of one per `+` (intermediates are never born — beats
reclaiming them). Retires the "SimPolicy_String::Add is fresh but unflaggable" gap (P5
batch A finding): rewritten sites plug into the existing rightmost-builder temp marking for
free. Semantics: fires only for the resolved builtin (no promotion in das ⇒ no formatting
divergence), L2R order preserved, both-empty→null agrees. Placement: post-infer, before
optimization (all-const builders still fold). Carry-over rule: builder eval computes ALL
element values before writing any, so only the LAST element could ever host a queue site —
v1 keeps element interiors site-free.

## Risks / notes

- Everything rides captureString propagation being correct (the BBATKIN comment at
  ast_allocate_stack.cpp:610) — same exposure the builder already has, now wider. The
  conservative default direction is always "don't wrap".
- Telemetry asymmetry: wrapper-queued strings notify alloc with temp=false but free with
  temp=true. Either thread the flag or document; nothing depends on it today.
- AOT semantic hash: wrapper is inserted pre-emission by the same pipeline → hashes
  consistently. No LLVM_JIT_CODEGEN_VERSION bump (no emitter change).
- PERF012/PERF007 overlap — **CORRECTED by the P5 sweep (batch D)**: the headline
  `find(string(resp.body))` shape is NOT a redundant clone — `find`/`read_json`/`to_int`
  take `string const implicit`, which does not bind `$::das_string`, so those ~40 sites
  are genuine conversions AND genuine wrapper-gain sites. The actually-redundant shape was
  `string(<already-string>)` — dasHV getters and `json ?? ""` no-op casts (103 sites
  dropped). PERF020 never caught those because its guard required exactly ONE argument
  while the string overloads bind {str,context,at} — fixed in daslib/perf_lint.das in this
  arc (temporary-string args stay exempt: `string(msg#)` is a load-bearing clone).

## P5 sweep results (2026-08-08, five Opus batches)

- Batch A (runtime+fio): 42 flagged (21 fio path/file helpers incl. base_name/dir_name/
  path_join/getcwd/fread/fgets/temp-file family; 21 runtime incl. sprint/sprint_json/
  fmt x10/clone_string/string(das_string)). 7 declined: pass_string is a passthrough,
  compiling_* return environment pointers, platform names are static literals, the
  wrapper itself. Finding: string `+` (SimPolicy_String::Add) is verified fresh but
  registered via the shared addFunctionConcat template — unreachable for the flag;
  retired by the concat-to-builder follow-up arc instead.
- Batch B (ast/rtti/debugger): 28 flagged, 0 declined — all single-expression
  allocateString of locally-built strings (describe*/get_mangled_name*/sprint_data/
  get_stackwalk). rtti_get_source_line copies a span out of FileInfo's long-lived buffer
  — it gets the row-(b) trap treatment in spirit (macro-time only).
- Batch C (external modules): 11 flagged (8 dasHV getters, 2 dasImgui in the hand-written
  main.cpp that survives binder regen, 1 dasTerminal). 6 declined — ImGui statics/
  engine-owned pointers, all in GENERATED binder files. dasLLAMA has NO C++ externs
  (pure das module); its vocab-string hazard is das-side and the propagation rule
  covers it conservatively.
- Batch D (site cleanups): 103 no-op casts dropped across 13 files (see the corrected
  note above); let-local `string(resp.body)` forms left for P3.
- Batch E (daslib gaps): 14 [temp_string_result] annotations across 8 files, each
  probe-verified (AST flag probe + runtime heap check); ~40 functions confirmed covered
  by propagation with no annotation; declines recorded (pad/trim/wide/ansi passthroughs,
  cast_to_string bit-pun). Finding: strings_boost.das:97 join(var iterator; sep; blk)
  overload is uncompilable today (error 30939 via join_implement's non-var param) —
  pre-existing, reported, not fixed in this arc.
- Sweep verification rows: tests/strings/temp_string_reclaim_sweep.das (flatness per
  batch family + identity-input passthrough traps for md_escape/to_generic_path/
  base_name/join).
