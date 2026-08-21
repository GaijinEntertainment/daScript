# daslib architecture notes

Design rationale a maintainer cannot recover from the code alone. One section per
module; entries are anchored to symbols. Grown by the comment-sweep rescue passes —
an entry lands here only when no name, shape, or test can carry it.

## perf_lint

- **Two parallel loop spaces.** `loop_depth`/`in_closure` (+ `VarStackEntry.depth`) is the
  contract for every loop rule: any closure body is deferred, hence not "in" the loop.
  PERF031 needs the opposite for inline block arguments (`peek_data(s) $(d) { ... }` runs
  in place), so `inline_aware_loop_depth`/`in_deferred` (+ `inline_aware_depth`) count
  loops inside inline block-argument bodies as real loop levels and defer only lambdas,
  local functions, generators, and generated blocks. `inline_block_entry_depth` records
  the loop depth at each inline-block entry because a `return` there unwinds only to that
  depth: loops at or below it keep re-invoking the block, so only barrier 0 proves the
  body runs once.
- **Closure-guard placement is load-bearing, per rule.** Four idioms coexist —
  `in_closure > 0` early return, `in_closure == 0` gate, `in_deferred == 0` gate, no guard —
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
  templates_boost wrapper is its only producer post-expansion. Its depth tracking must
  fire before the `in_closure` gate — the rules block is itself a closure.
- **Hot-path walk (PERF026-028).** `HotCache` keeps per-function scan results sliced into
  two flat arrays; `sinks_span_by_fn` doubles as the presence key and the two tables are
  strictly co-populated (one insert site each, adjacent). The visited set keys on
  (callee hash, `nearest_root_file_site` pointer): reaching one callee from two root-file
  call sites must report twice, so a function is re-walked once per distinct anchor —
  a cost taken for per-call-site diagnostics. Generated/generic frames re-anchor to the
  caller side because their own `at` points into the template. A call to any function
  with a `@scratch` parameter prunes that callee's whole subtree from the walk — the
  blessing is deliberately TRANSITIVE: a scratch-taking helper is trusted wholesale
  (narrowing it to the sink floods real scratch-heavy code — dasLLAMA — with hundreds
  of deep-resize findings on buffers the helper family sizes correctly).
- **PERF030 release credits are consume-once**: a `delete x` licenses exactly one
  subsequent `x <- ...`, then the warning re-arms; the lowered delete family is matched
  by name in its four spellings (`_::finalize`, `finalize`, ``builtin`finalize``,
  ``builtin`finalize_dim``).
- **`"$"` in module allowlists is the builtin module's name** — `Module_BuiltIn` registers
  as `Module("$")` (`src/builtin/module_builtin.cpp`), so macro-time reflection reports
  core builtins under `"$"` beside `"builtin"` and `"strings"`.
- **`Function.tempStringResult` marks a FRESH per-call string allocation** — never a view
  or a passthrough (`include/daScript/ast/ast.h`). A temp-string builtin (`to_char`,
  `repeat`) on a hot path IS heap traffic; the PERF026 string-returning catch-all
  deliberately has no temporary exemption. PERF020's `_type.flags.temporary` check is a
  different axis: a `#`-typed VALUE being cast, where `string(x#)` is a load-bearing
  temporary-to-permanent clone.
- **PERF021's tail-argument comparison passes `require_pure = false`** — workhorse-cast
  tails are flags and literals after the injected Fake* skips, so purity has nothing to
  protect there; the default protects rules that collapse duplicated subexpressions from
  changing evaluation counts.
- **`report_key` bit layout**: line in the low 20 bits, column shifted above it, XOR'd with
  the FNV-64-prime-mixed `fileInfo` pointer and the rule-code hash. Lines past 2^20 alias —
  accepted; a collision silently drops one warning.
- **`parse_range_leg` operator polarity**: `upper_op` is the var-on-left operator marking
  the upper-bound leg (`<=` closed-in-range, `>` open-out-of-range), `lower_op` the lower
  (`>=` / `<`). Yoda (const-on-left) forms flip, so the const-on-left arm tests against
  `lower_op` — the `is_hi_out` assignments differ between the two arms by design.
- **A generic-body exemption exists exactly where the remedy is not instantiation-safe.**
  PERF020's fix (delete the cast) is wrong for the sibling instantiations of the same
  source line, so it bails on `fromGeneric`; PERF019/PERF021 rewrites (fuse under one
  cast, hoist over the ternary) stay valid for every instantiation, so they fire
  everywhere.
