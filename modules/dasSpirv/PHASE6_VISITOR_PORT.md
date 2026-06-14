# Phase 6 — emitter → `AstVisitor` (clean, behavior-preserving port)

Conversion plan, written *before* touching emitter code (per the Phase 6 hard rule). Scope: **100 %
clean port, zero new features.** The only additions allowed are **tests** (where porting a construct
reveals it was never covered). Proof of behavior-preservation = the existing harness (opcode census
both directions + spirv-val + interp/AOT + dasVulkan GPU gate), plus a new normalized-disassembly
golden compare added up front as the safety net.

---

## 1. What is being ported (and what is NOT)

`generate_spirv(fn, errors, stage, local_size, reflection) : array<uint>` is the single `[macro_function]`
entry, called by the shader macro `patch()` (spirv_shader.das) and — via the captured `{name}_spv`
globals — by every test. **Its signature and the entire capture mechanism stay byte-for-byte unchanged.**
The whole test suite and the macro are untouched.

`generate_spirv`'s body splits into two halves:

| Half | Functions | Phase-6 disposition |
|---|---|---|
| **Module assembly** (not AST-walking) | `make_module`, caps/memmodel emit, `classify_global` loop over `collect_dependencies`, entry-point + exec-mode, `OpFunction`/`OpLabel`, trailing `OpReturn`/`OpFunctionEnd`, `module_words` | **Keep as-is.** This is not where the fail-closed liability lives. |
| **Body codegen** (the hand-walker) | `emit_body` → `emit_stmt` → `emit_value` / `emit_ptr` / `emit_const` / `emit_call` / `emit_mul` / `emit_if` / `emit_while` / `emit_for` / `emit_load_op_store`; plus the `collect_locals` pre-pass | **Replace with `SpirvEmit : AstVisitor`.** |

The pure helper tables (`binop_code`, `cmp_code`, `unop_code`, `glsl_ext_op`, `vector_ctor`,
`scalar_class`, `component_count`, `build_block_struct`, `std140_*`, `builtin_info`, `sampler_info`,
`field_index_by_name`, `global_var_of`) are **reused verbatim** — they are stateless lowering logic, not
tree-walk control flow.

## 2. The stage constraint that shapes everything

The emitter runs at the annotation's **`patch()` (pre-fold)** stage, NOT fixup. Confirmed from source:
the capture global's `init` is a `to_array_move(...)` **call**, which must be set in `patch` with
`astChanged = true` so the re-infer resolves it (the spirv_shader.das header comment documents that
setting it in `fixup` "is never resolved and crashes the alias pass"). dasGlsl can use `fixup` only
because its init is a plain `ExprConstString` (no re-infer needed). **We cannot move to fixup without
changing the capture representation — that would be a feature change. So we stay at patch.**

Consequences the visitor must honor (all already handled by today's hand-walker — we port the handling,
not remove it):
- **`ExprRef2Value` wrappers are still present** (fold has not collapsed them). The "load this lvalue as
  a value" point is exactly where an `ExprRef2Value` sits.
- **`ExprField.fieldIndex == -1`** — resolve member position by name via `field_index_by_name`, never
  `fieldIndex`.
- **Constants may be unfolded** — `-0.6` can arrive as `ExprOp1("-", const)`; folded vector constants
  (`ExprConstFloat3`, …) and constructor calls both appear. `emit_const`'s existing dual handling ports
  across unchanged.

## 3. `SpirvEmit : AstVisitor` — shape

Modeled on `LlvmJitVisitor` (SSA, same value-threading, same basic-block emission). State:

```
class SpirvEmit : AstVisitor {
    adapter    : VisitorAdapter?
    m          : SpirvModule? -const          // the module being built (borrowed, not owned)
    ctx        : EmitCtx? -const              // globals/locals/local_vars/loop_stack/terminated/...
    e2id       : table<Expression?; uint>     // result-id of each expression (SSA side-map, post-order)
    e2ptr      : table<Expression?; uint>     // pointer-id of each LVALUE expression (OpAccessChain base / store target)
    e2ptr_ty   : table<Expression?; uint>     // pointee type-id paired with e2ptr (what an OpLoad through it yields)
    ite_blocks : table<Expression?; IteIds>   // per ExprIfThenElse: {merge, then, else} label ids
    loop_ids   : table<Expression?; LoopIds>  // per ExprWhile/ExprFor: {header, cond, body, cont, merge}
    for_src    : table<Expression?>           // for-source range() calls to intercept (not emit as a call)
    errors     : array<string>               // same error channel generate_spirv already returns
}
```

`EmitCtx` (globals, locals, local_vars, loop_stack, terminated, glsl_ext, stage) is unchanged and is
referenced (not copied) by the visitor. `terminated` and `loop_stack` migrate from "threaded through
emit_* args" to visitor fields — same semantics.

### 3.1 SSA threading (the value map)

Exactly llvm_jit's mechanism. `setE(e, id)` / `getE(e)`; every `visitExpr*` runs post-order, reads its
children's ids out of `e2id`, emits its op, stores its own id. `getE` on an expression with no recorded
id pushes a clean `"unresolved expression {e.__rtti}"` error (this is half the fail-closed net — §6).

### 3.2 Lvalue vs rvalue — the one genuinely tricky part

Today `emit_ptr` (pointer) and `emit_value` (loaded value) are two functions. In the visitor it becomes
parent-driven, and **pre-fold `ExprRef2Value` is the explicit load marker** — cleaner than the fixup-stage
`r2v`-flag approach because the AST literally tells us where loads happen:

- **lvalue nodes** — `ExprField`/`ExprAt`/`ExprSwizzle`(single-component)/`ExprVar`(of a memory-backed
  local or global) — emit `OpAccessChain` (or yield the existing var-id) and record into `e2ptr`/`e2ptr_ty`,
  NOT `e2id`. Their "natural result" is a **pointer**.
- **`visitExprRef2Value`** — looks up the child's `e2ptr`/`e2ptr_ty`, emits `OpLoad`, records the loaded
  value into `e2id`. Their "natural result" is a **value**.
- **value nodes** — consts/ops/calls/composites — record into `e2id` directly.

A parent that needs a **value** (op operand, call arg, store RHS, branch cond) reads `e2id` of the child;
the child is either already a value, or is the `ExprRef2Value` that loaded it. A parent that needs a
**pointer** (store target `ExprCopy.left`, `ExprAt` base, `ExprField` base) reads `e2ptr` of the child —
and the AST guarantees store targets / access-chain bases are NOT wrapped in `ExprRef2Value`, so the
pointer is there. This is the precise pre-fold analogue of llvm_jit's `r2v`-flag load/pointer split.

Value-swizzle of a non-global (`p.x` / `p.xy` on a let/expr) stays a **value** path
(`OpCompositeExtract`/`OpVectorShuffle`) — it never produced a pointer today and won't here.

### 3.3 Function-local OpVariables

SPIR-V requires every Function-storage `OpVariable` at the **top of the entry block**, before any body
code. A single forward visitor pass emits body code as it goes, so it cannot also emit var decls at the
top — unless they go to a separate buffer. **Decision: keep the existing `collect_locals` pre-pass
unchanged.** It only *scans* for `ExprLet`/`ExprFor` to pre-declare `OpVariable`s; it emits no body ops,
so it is not part of the fail-closed liability (a missed control-flow node there degrades to a later
"var used before declared" error, not a silent wrong blob). Keeping it is the lower-risk clean-port
choice and avoids adding a function-prologue buffer to the builder (which would be a structural change).
*(Alternative considered and rejected for this port: fold locals into `preVisitExprLet` + a prologue
sub-buffer. More "pure visitor," but it's a builder change with no behavior benefit — defer.)*

## 4. Hook map (every current `emit_*` → its visitor hook)

| Current | Visitor hook(s) | Behavior (ported verbatim) |
|---|---|---|
| `emit_value` ExprConst{U}Int/Float | `visitExprConstInt`/`UInt`/`Float` | `const_int/uint/float`, record `e2id` |
| `emit_value` folded vector consts + `emit_const` | `visitExprConst{Float,Int,UInt}{2,3,4}`, `visitExprMakeArray` (fixed-array only), const-ctor calls | `OpConstantComposite` (deduped); fixed-array → composite; reject dynamic `array<T>` literal |
| `emit_value` ExprOp1 | `visitExprOp1` | `!`→LogicalNot; `-`/`~`→`unop_code`; statement-level `++`/`--` handled in §5 |
| `emit_value` ExprOp2 | `visitExprOp2` | `emit_mul` (matrix/vec·scalar) → cmp → logical and/or → `binop_code`; operands via `getE` |
| `emit_call` | `preVisitExprCall` (mark for-source skip / detect kind) + `visitExprCall` | texture / vector ctor / dot / GLSL.std.450 ext-inst; args via `getE`. Arg visiting is automatic |
| `emit_ptr` ExprAt | `visitExprAt` | local fixed-array or ssbo `OpAccessChain` → `e2ptr` |
| `emit_ptr` ExprField | `visitExprField` | block-global member `OpAccessChain` by name-index → `e2ptr` |
| `emit_ptr`/`emit_value` ExprSwizzle | `visitExprSwizzle` | global single-comp → `e2ptr`; local value → `OpCompositeExtract`/`OpVectorShuffle` → `e2id` |
| `emit_value` ExprVar | `visitExprVar` | local memory → `e2ptr` (load happens at the wrapping Ref2Value); value-let → `e2id`; global → `e2ptr` |
| `emit_value` `_type.flags.ref` load / ExprRef2Value | `visitExprRef2Value` | `OpLoad` through child `e2ptr` → `e2id` (§3.2) |
| `emit_stmt` ExprCopy (store) | `visitExprCopy` | `OpStore`(child `e2ptr` of left, `e2id` of right) |
| `emit_stmt` compound `op=` / `++`/`--` | `visitExprOp2`(void) / `visitExprOp1`(stmt) | `emit_load_op_store` logic |
| `emit_stmt` ExprLet | `preVisitExprLetVariable` + init hook | mutable → `OpStore` into pre-declared var; value-let → bind `e2id`; reject uninitialized |
| `emit_stmt` ExprReturn | `visitExprReturn` | `OpReturn`; set `terminated` |
| `emit_if` | `preVisitExprIfThenElse` / `…IfBlock` / `…ElseBlock` / `visitExprIfThenElse` | §5 |
| `emit_while` | `preVisitExprWhile` / `…WhileBody` / `visitExprWhile` | §5 |
| `emit_for` | `preVisitExprFor` / `…ForSource` / `…ForBody` / `visitExprFor` | §5 |
| `emit_stmt` ExprBreak/Continue | `visitExprBreak`/`visitExprContinue` | branch to `loop_stack` top merge/cont; set `terminated` |
| `emit_stmt`/`emit_body` ExprBlock | `preVisitExprBlock`/`visitExprBlockExpression` | inline; respect `terminated` (skip after a terminator) |

## 5. Control flow — multi-phase hooks + terminator tracking

The exact instruction order today is preserved, redistributed across the push/pop hooks (mirrors
llvm_jit). Terminator state lives in `ctx.terminated`, set/cleared at the same points.

- **if/elif/else:** `preVisitExprIfThenElse` allocs `{merge, then, else}` into `ite_blocks`. The cond is a
  child → already visited. `preVisitExprIfThenElseIfBlock` emits `OpSelectionMerge merge` +
  `OpBranchConditional (getE cond) then else` + `OpLabel then` (SelectionMerge must immediately precede
  the conditional branch — this ordering is exactly why it goes in the IfBlock pre-hook). Then-body
  visited; `preVisitExprIfThenElseElseBlock` branches to merge (if not terminated) + `OpLabel else`.
  `visitExprIfThenElse` branches to merge (if not terminated) + `OpLabel merge`. **`elif` chains** fall out
  naturally: `if_false` being another `ExprIfThenElse` just recurses the same hooks.
- **while:** `preVisitExprWhile` allocs the 4-block skeleton, emits `Branch header`/`OpLabel header`/
  `OpLoopMerge merge cont`/`Branch cond`/`OpLabel cond`. `preVisitExprWhileBody` emits
  `OpBranchConditional (getE cond) body merge` + `OpLabel body` and pushes `loop_stack`.
  `visitExprWhile` pops, branches to cont, emits `OpLabel cont`/`Branch header`/`OpLabel merge`.
- **for `i in range(...)`:** the intricate one. `preVisitExprFor` reads the single iterator var + single
  source, computes `lo`/`hi` (`ExprConstRange` → constants; `range(n)`/`range(lo,hi)` → from arg ids),
  `OpStore lo`, emits header/LoopMerge/cond skeleton. `preVisitExprForBody` loads `i`, emits
  `i < hi` + `OpBranchConditional` + `OpLabel body`, pushes `loop_stack`. `visitExprFor` pops, branches to
  cont, emits the `i += 1` update, backedge, merge.
  - **For-source `range()` interception (the sharp edge):** the visitor would otherwise visit the
    `range(...)` call and `visitExprCall` would error ("unsupported call 'range'"). Mitigation modeled on
    llvm_jit: in `preVisitExprFor` record `fr.sources[0]` into `for_src`; let the visitor visit the call's
    **arguments** normally (so `range(lo,hi)` endpoint ids land in `e2id` and runtime bounds Just Work —
    a case the constant-only fixtures don't currently cover, see §7), but in `visitExprCall` short-circuit
    when the call is in `for_src` (emit nothing). `ExprConstRange` has no sub-calls; read its `int2`
    value directly in `preVisitExprFor`.

## 6. Fail-closed gates (the whole point of Phase 6)

Two layers, together giving: a future AST node type or Visitor-interface method change surfaces as a
**clean dasSpirv compile error**, never a silent wrong/empty blob.

1. **Explicit rejection list** (dasGlsl's discipline, trimmed to shader-relevant constructs): a
   `preVisit*` override for each unsupported `Expr*` that pushes a clear `"unsupported <construct> in
   SPIR-V shader"` error — e.g. `new`/ascend, `delete`, string builder, `try`/`recover`, `with`, tuples,
   variants, `?as`/`is`/`as`, `invoke`, closures/`MakeBlock`, table ops (`insert`/`erase`/`find`/
   `key_exists`), `yield`, `goto`/`label`, `typeinfo`, `memzero`, named call, deref, safe-field/safe-at,
   clone/move-to-make. This is the surface that makes "unsupported" loud.
2. **`getE`-unresolved backstop:** any expression that was walked but produced no id (the genuine
   silent-skip case — including a brand-new AST node we have no hook for) errors the moment a parent reads
   it. This catches what the explicit list doesn't enumerate.

Because `SpirvEmit` subclasses `AstVisitor`, if the Visitor interface itself gains a new `canVisit*` /
abstract method upstream, daslang forces the subclass to confront it at compile time — the exact contract
every other backend already has, which dasSpirv joins here.

## 7. Tests to add

### A. Port-safety net — add FIRST, before any emitter edit
- **Normalized-disassembly golden compare.** For every fixture (`square_spv` … `tex_frag_spv`), snapshot
  the current emitted blob as a *normalized* `spirv_dis` stream (result-ids canonicalized to sequential
  placeholders, so it is insensitive to id-numbering differences the visitor walk may introduce) and
  assert it is unchanged after the port. This is the primary behavior-preservation proof beyond
  census+val. (Byte-identical is attempted too; if id-allocation order diverges, normalized-dis is the
  contract.)

### B. Coverage gaps revealed while porting (each lands with its construct)
- **Runtime-bound for-loop** — `for (j in range(0, n))` with `n` a loaded/push-constant value. The
  `range(lo,hi)`-with-runtime-args path (`emit_value` on src args, `ExprCall` not `ExprConstRange`) is
  **currently unexercised** — all `floop` ranges are constant. Direct hit on the §5 for-source handling.
- **Nested loops** — `for` inside `while` (or `for` inside `for`) with `break`/`continue` in the *inner*
  loop, asserting the branch targets the inner merge/cont (loop_stack push/pop correctness — the #1 port
  risk).
- **`elif` chain** — `if {} elif {} elif {} else {}` (nested `ite_blocks` recursion).
- **Early `return` inside control flow** — `return` in an `if` branch inside a loop. Classic structured-CFG
  validity trap (block already terminated + merge handling); no current fixture returns early.
- **Cross-coverage** — `continue` in a `while`, `break` in a `for` (today: break only in while, continue
  only in for).
- **Bool local** — `var flag = a > b; if (flag) {…}` (bool-typed `OpVariable` store/load path).

### C. Fail-closed negative tests (the Phase-6 deliverable, locks in §6)
- Expect-error fixtures (dastest `// expect` directive — same mechanism `has_expect_directive` keys on)
  asserting `generate_spirv` fails **cleanly** (errors non-empty, no crash, no blob) for: an unsupported
  **statement** (e.g. a `table`/`new`), an unsupported **call** (a non-builtin function), an unsupported
  **global** (neither builtin/ssbo/uniform/push_constant/sampler), and an unsupported **local type**.
  These also guard the gates: a future regression that silently skips a node would flip a negative test
  from "compile fails as expected" to "compiles" — detectable.

## 8. Sub-phase ordering (each keeps the suite green)

- **6.0 Spike (GATE — de-risks the whole port).** Minimal `SpirvEmit` handling only the `square` subset
  (var/let/at/store/IMul/builtin-load/return), driven via `make_visitor` at **patch stage** over
  `fn.body`. Proves the two open risks: (a) an `AstVisitor` can run during `patch()` and (b) the
  pre-fold node shapes (`ExprRef2Value` present, `fieldIndex == -1`, r2v) are walkable as designed —
  confirm `visit`/`visit_expression` over just the body (not the whole function, so `generate_spirv`
  keeps owning the OpFunction scaffold). Output must be census+spirv-val-equal to today's `square`.
  **If the spike fails (visitor unrunnable at patch / shapes wrong), STOP and report — the hand-walker may
  be the correct design and we say so, rather than forcing it.**
- **6.1 Leaf + arithmetic.** Full value/pointer hooks for consts, op1/op2, var, ref2value, let, copy/store,
  compound-assign, builtin loads. Add the §7-A golden net. Get `square`+`*arith` equal.
- **6.2 Control flow.** if/elif/else, while, for (incl. runtime bound), break/continue via the multi-phase
  hooks + terminator + loop_stack. Add §7-B tests. Get `*ctrl`+`*loop` equal.
- **6.3 Resources + composites.** field/swizzle/at lvalues, vector ctors, dot, ext-inst math, texture,
  matrix ops, const arrays. Get `raster`/`ubo`/`matrix`/`pc`/`sampler`/`catri` equal.
- **6.4 Fail-closed.** Explicit rejection list + `getE`-unresolved backstop; add §7-C negative tests.
- **6.5 Prove + clean up.** Full suite (census both directions, spirv-val, LCOV ~100 % of the new emit
  dispatch), interp+AOT (purge `_aot_generated` to dodge stale-hash), dasVulkan GPU gate (cross-repo:
  blobs still render `i*i` + the textured quad), §7-A golden compare green, lint/format. **Delete the old
  `emit_*` functions.** Append the Phase-6 landing record to `MASTERPLAN.md`.

## 9. Spike (6.0) results — GO

Ran a read-only probe visitor over the `square` body at patch stage. Both gating unknowns resolved
positively:

1. **Visitor at patch stage — RESOLVED ✓.** `make_visitor(*v) { visit(fn.body, adapter) }` runs cleanly
   during the annotation's `patch()`; all `preVisit*` hooks fire in correct pre-order over the whole body.
   The port is viable; the hand-walker fallback is not needed.
2. **Pre-fold shapes — RESOLVED ✓, as designed.** For `data[i] = i * i`: the store-target `data[i]` is a
   bare `ExprAt` (NOT wrapped), while the index `i` and both `i*i` operands are each wrapped in
   `ExprRef2Value` over an `ExprVar`. This is exactly the lvalue/rvalue split §3.2 builds on —
   `ExprRef2Value` is the load marker; unwrapped lvalue nodes are pointers. `gl_GlobalInvocationID` reads
   as a global (`local=arg=_block=false`), `i` as a local; swizzle-of-global has `ref=true`.

**New finding (folded into the design):** at patch the body nodes are **const** (`ExprVar? const`, …), so
a `visit*` hook cannot `return expr` directly (the return slot is non-const `ExpressionPtr`). Emission of
value-producing nodes must happen in `visit*` (post-order — children ids ready) and the hook returns the
**same** node through a one-line const-cast helper (`return unsafe(reinterpret<ExpressionPtr>(expr))` — safe
because we never mutate the AST, only read it and emit into the side module). `preVisit*` (void) hooks need
no such cast and are used for the pre-order work (block setup, control-flow header emission, var
pre-declare). llvm_jit returns `expr` directly only because it runs at true codegen where nodes are
non-const; at patch we const-cast. *(The probe itself sidestepped this by using `preVisit`-only hooks.)*

## 10. Remaining risks (post-spike)

1. **For-source `range()` interception** — the sharpest construct; covered in §5 + tested in §7-B.
2. **Id-numbering divergence vs byte-identical** — handled by normalized-dis golden (§7-A); byte-identical
   is best-effort, not the gate.
3. **`canVisit*` granularity** — confirm which interface hooks exist for clean rejection vs relying on the
   `getE`-unresolved backstop; finalize the §6 list during 6.4 against the real `AstVisitor` surface.
