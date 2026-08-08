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
- **CodeOfPolicies gate, like dse** (`ast.h:1610` neighborhood): `/*option*/ bool` so the
  measurable analysis cost is skippable. NOT auto-keyed to `persistent_heap`: f2s runs
  without it and still needs the reclaim (within-run growth; the 1-slot queue frees the
  last-allocated string = top-of-chunk pop, which works on the default linear string heap —
  that's the measured −36B builder number). "Non-persistent ⇒ unneeded" is the
  host-restarts-contexts-per-frame case — those hosts opt out in their policies. Polarity:
  `disable_temp_string_reclaim = false` (default-ON, mirrors dse) unless Boris overrides
  at implementation time. Cost mitigation
  regardless of default: skip the walk outright when no call to a flagged function exists
  in the program (bit set during the existing captureString walk — near-zero when unused).
- **One insertion pass, one scan, post-optimization** — runs just before allocate_stack
  (captureString is final by then; tree is stable; no optimizer interference with a
  side-effecting wrapper). It takes over the builder scan currently in
  ast_allocate_stack.cpp:604-620 (delete it there) so that the rightmost rule is GLOBAL:
  per consuming call (`!policyBased && !invoke && !captureString`), mark/wrap only the
  **rightmost eligible argument overall** — builder OR flagged call. Two scans would
  dangle: wrapper on arg0 conversion + builder temp on arg1 flushes arg0 before the call.
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
- intern-strings context: no-op, values correct.
- builder behavior unchanged (existing tests keep passing — the scan moved, semantics identical).

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

## Risks / notes

- Everything rides captureString propagation being correct (the BBATKIN comment at
  ast_allocate_stack.cpp:610) — same exposure the builder already has, now wider. The
  conservative default direction is always "don't wrap".
- Telemetry asymmetry: wrapper-queued strings notify alloc with temp=false but free with
  temp=true. Either thread the flag or document; nothing depends on it today.
- AOT semantic hash: wrapper is inserted pre-emission by the same pipeline → hashes
  consistently. No LLVM_JIT_CODEGEN_VERSION bump (no emitter change).
- PERF012/PERF007 overlap: direct-arg `string(das_string)` hits should be lint-fixed
  (drop the conversion), not celebrated as wrapper wins.
