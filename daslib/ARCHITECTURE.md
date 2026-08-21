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

## lint_config

- **`.lint_config` is parsed once per process.** `[lint_macro]` apply() fires once per
  module in the require chain, so an uncached read re-parses the repo file N times per
  compilation; the file is repo policy and cannot change mid-compilation, so
  `ensure_cache_loaded` splits it once into off/on/path-exclude caches.
- **`LINT019` in a line's nolint list exempts the WHOLE line from stale-checking** —
  chosen tradeoff: the in-tree escape hatch is `nolint:PERF006,LINT019` on lines whose
  code fires only in downstream compiles; per-code vouching would need new syntax.

## lint

- **`lint_error` reports only nodes whose lineinfo belongs to the current function's own
  file.** An inliner splice or macro-manufactured node from another file gets its
  diagnostics from that file's own compile; reporting it here would fail a module on
  idioms it cannot edit.
- **LINT022 collects references from three sources because none alone sees them all**:
  the ordinary walk (resolved pointers); a latched second walk over generic bodies
  (`visit(prog)` skips generics, and uninferred bodies carry only spelled alias names);
  and declarations plus string literals (signatures and name-based lookups leave only a
  literal). Self-reference is pre-inserted so it cannot keep a private type alive.
  Functions are checked only when inlining is off — inlining splices small private
  callees away and leaves husks indistinguishable from dead code. Public symbols,
  lifecycle/macro hooks, virtual methods, and generic bodies are skipped: their callers
  are invisible to lint.
- **Argument rules skip what the author does not own, and converge leaf-first.**
  LINT012/013/014/023 stay silent on class methods, `[extern]` stubs, `finalize`
  overloads, and address-taken functions; LINT014 also skips a parameter whose mutability
  a callee demands — the leaf is flagged first and each fix re-exposes the next caller.
  A returned `var` argument keeps its `var` (a non-copyable result cannot move from
  const); a used `_name` parameter is never LINT004'd (interface and keyword-clash names).
- **Closure bodies are per-rule, not global.** LINT010 counts a closure body as a branch
  (it may run later or never — writes inside must not kill outside stores, reads inside
  must not keep an outside init live); LINT021 counts the same body as an escape — a
  captured 64-bit local is never a narrow.
- **`expr_equal` compares `describe(a) == describe(b)`, never pointers** — every
  Expression has exactly one parent, so sibling pointer equality is effectively never
  true. LINT007 excludes `ExprConst*` operands: identical constants are macro-splice
  residue, not copy-paste typos.
- **LINT015 fires on `+` and `-` only** — the only operators both binary and unary, so a
  split continuation line re-parses as a valid discarded unary statement; every other
  operator produces a loud parse error instead.
- **LINT023's store set extends the compiler's.** `under_clone` marks only bare-variable
  `=`/`:=` targets; `<-`, swizzle LHS, and statement-level `++`/`--` are marked by the
  visitor. A pointer parameter stays a candidate: a write through the pointee reaches the
  caller and counts as a use, not a store.
- **The 64-bit family fires only where the overflow hazard survives.** LINT021 treats
  only unbounded-top math as transparent (`min`/`clamp`/`sign` cap the value — deliberate
  saturation); LINT024 requires a call among the product's leaves — that is the unbounded
  factor, a product of plain locals is tile geometry.
- **`canVisitWithAliasSubexpression` returns false on purpose** — `assume` keeps the
  template block at the definition site and clones it per use; the block is never invoked
  there, so descending would report its arguments unused.

## style_lint

- **Lint runs under policies that skip const-folding and leave some types unset.** Take a
  variable's type from `Variable._type`, not an `ExprVar`'s; expect `T('c')` to stay an
  `ExprCall`; expect `-1` as an `ExprOp1` over a constant; when a type is missing, bias
  conservative ("needs unsafe").
- **Rules whose forms are identical in the AST read the source span instead** — `defer <|`,
  generators, STYLE003's `$()`, STYLE028's three invoke spellings. STYLE005 uses LineInfo
  identity: the parser gives braceless and postfix forms a synthetic block sharing the
  terminator's LineInfo, so `blk.at != inner.at` ⇔ the user wrote braces. Both oracles
  break silently when lowering or LineInfo assignment changes.
- **STYLE024/025 mirror the compiler's unsafe rules with a deliberate asymmetry: in doubt,
  mark "needs unsafe".** A false "needs" costs a missed note; a false "redundant" tells
  the user to delete a wrap the compiler requires. `let v & = unsafe(expr)` is the one
  recorded exception — the wrap is required by the reference binding, not the inner
  expression. STYLE025 narrows only when the single statement has an expression form.
- **A require counts as used when its symbols cannot be traced, not only when seen.**
  Generic instances attribute to `fromGeneric._module`; bare names in never-instantiated
  generic bodies keep their providers alive; re-export chains are followed; a module
  providing any compile-time macro or lifecycle hook is used — it leaves no AST reference.
  STYLE029 fires only when exactly one re-exported module is used: two or more is an
  aggregation facade.
- **Metric rules attribute code to the module that can fix it.** STYLE037/038 walk the
  `fromGeneric` chain to its root (`[template]` generics instantiate INTO the caller);
  generated carriers stay unmetered; closures are not length-checked separately — the
  host trips first. The 80-line default ≈ p97 of this tree's function-length distribution.
- **STYLE041 is sound because of a reference count, not the structural scan.** The scan
  classifies writes, plumb reads, and the single consuming `if`, arming the candidate with
  that count; `preVisitExprVar` counts every reference in the function, and the warning
  fires only when the counts match. Anything the scan does not descend into therefore
  rejects the candidate instead of being assumed harmless.
- **STYLE036 keys on a flag the compiler clears.** Infer strips a substitution contract
  when a generic binds; one still set on a resolved cast target was never consumed — but
  an `auto`/unresolved-alias target has simply not been substituted yet, hence the
  `isAutoOrAlias` bail.
- **STYLE039 requires non-ASCII in both the value and its source span** — escape-spelled
  bytes in ASCII source are log-safe, and span-slicing keeps a neighbouring comment's
  typography from firing.
