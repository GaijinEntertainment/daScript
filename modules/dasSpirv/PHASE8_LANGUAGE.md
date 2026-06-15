# Phase 8 — language completeness (plan)

> Working plan for the Phase-8 PR — the **last emitter-plumbing phase** (9 docs / 10 examples /
> 11 lint are not plumbing). The durable record is `MASTERPLAN.md`; this is the reviewable design
> sketch written *before* code (the discipline that paid off in Phases 6 and 7).

## Goal

Close the daslang→SPIR-V *language surface*: lower every shader-legal daslang expression/statement
that the emitter still rejects, and clear the two lowering gaps Phase 7 deferred. After this phase the
emitter covers what daslang can express for a shader; what remains (9–11) is docs, demos, and lint.

## Scope reconciliation (grammar-checked)

The original masterplan Phase-8 list named `OpSwitch`, `do { } while`, and labeled break/continue.
**daslang has none of these, by design** (Boris, 2026-06-15) — the only loop/branch keywords are
`while`/`for`/`break`/`continue` (Phase 2, done), plus the ternary `?:` and unstructured
`goto`+numeric `label:`. `goto`/`label` are already fail-closed-rejected and cannot lower to SPIR-V's
*structured* CFG. So those three bullets have no source construct and are dropped (not deferred —
they will never exist). The real Phase-8 content is the four items below.

## Scope (four items)

1. **Ternary `?:` → `OpSelect`.** Parses to `ExprOp3("?", cond, a, b)`. No `visitExprOp3` handler
   today → currently caught by the `value_of` backstop. Add the handler: evaluate cond/a/b, emit
   `OpSelect` (branchless) for scalar **and** vector results. Eager evaluation of both arms is correct
   for the same reason `&&`/`||` are eager — shader operands are side-effect-free (documented in-code).
   A vector condition selects per-lane (SPIR-V `OpSelect` allows a bvecN condition matching the result
   width); a scalar condition selects the whole value. Operand/result type must agree (fail-closed
   else).
2. **Vector arithmetic** (Phase-7 deferred gap A). `visitExprOp2` derives `cls` from
   `scalar_class(expr.left._type.baseType)`, which returns -1 for a vector base type → "unsupported
   binary op". Fix: derive the **component** class for vector base types so `+ - / %` emit
   `OpFAdd`/`OpIAdd`/`OpFSub`/… with the vector result type (these ops are component-wise on vectors
   natively). `*` is already routed through `emit_mul` (same-shape vector·vector → component-wise);
   this extends `+ - / %` (and unary `-` via `ExprOp1`) to vectors. Comparisons/logical on vectors stay
   out of scope (rarely used in shaders, produce bvecN) — rejected cleanly, not silently mis-emitted.
3. **Folded const-vector operand** (Phase-7 deferred gap B). `float2(0.5, 0.5)` as a call/op argument
   folds to `ExprConstFloat2` (fold-state-dependent — same class as the const-array `float2` finding),
   which `value_of` doesn't lower. `emit_const` already produces the correct `OpConstantComposite` for
   `ExprConst{Float,Int,UInt}{2,3,4}`; wire `value_of` → `emit_const` for those folded nodes so a
   const vector is usable as an rvalue anywhere (not only inside a const-array initializer).
4. **Broaden GLSL.std.450 math.** Extend `glsl_ext_op` beyond Phase 3's set to the common shader math:
   `mix`/`step`/`smoothstep`/`pow`/`exp`/`log`/`exp2`/`log2`/`fract`/`mod`/`atan2`/`reflect`/`refract`/
   `length`/`distance`/`normalize`/`cross`/`inversesqrt`. Each maps to its ext-inst opcode (a few are
   class-aware where a signed/unsigned/float variant exists; the rest float-only); `cross`/`reflect`/
   `refract`/`normalize`/`length`/`distance` are vector-signatured. `dot` stays core `OpDot` (already
   done). One census entry + spirv-val per added op; an unsupported name/class stays a clean error.

## Fail-closed (the #3137/#3139 rule holds)

Every new dispatch validates input and emits a clean `error[...]`: `OpSelect` rejects mismatched
arm/condition types; vector arithmetic rejects an unsupported component class or a vector comparison;
the math table rejects an unknown name or wrong operand class. No silent bad blob, no panic.

## Slices (one commit each, suite green after each)

### 8.1 Ternary → OpSelect
- `visitExprOp3` handler; scalar + vector select. Fixture: scalar `a > b ? a : b` and a vector
  `cond ? v0 : v1`. Asserts `OpSelect` present + spirv-val. Census += `Select`.

### 8.2 Vector arithmetic + folded const-vector operand
- Component-class derivation in `visitExprOp2`/`ExprOp1`; `value_of` → `emit_const` for folded vector
  consts. Fixture: `va + vb`, `va - vb`, `va / vb`, `-va`, and `vuv + float2(0.5, 0.5)` (the exact
  Phase-7 texlod shape that forced the deferral — re-add it as the regression). Asserts component-wise
  `OpFAdd`/`OpFSub`/`OpFDiv`/`OpFNegate` on vector types + the `OpConstantComposite` operand +
  spirv-val. No new census opcodes (FAdd/FNegate/ConstantComposite already declared) — the fixtures
  union in and must stay within the set.
- **Retire the two Phase-7 deferral notes** in the masterplan once green.

### 8.3 GLSL.std.450 math breadth
- Extend `glsl_ext_op`; one fixture per op family (scalar math, vector math). Asserts each `OpExtInst`
  with the right ext-opcode + spirv-val. Census += each new ext-inst (ext-inst opcodes are tracked by
  the GLSL.std.450 enumerant, asserted via the existing ext-inst census path).

## Sequencing

8.1 → 8.2 → 8.3, each its own commit keeping all three tiers (interp/JIT/AOT) + census (both
directions) + spirv-val + golden-disassembly green. One daslang PR (emitter + main-tree tests). No
dasVulkan GPU gate is *required* (these are language-surface, spirv-val + census are the oracle), but a
small optional dasVulkan compute fixture exercising ternary + vector math + a GLSL.std.450 call gives a
real-driver belt-and-suspenders — decide at 8.3 whether it earns its keep. Standing gates: lint +
format, Copilot-to-dry, no GC leak. AOT requires `rm -rf tests/spirv/_aot_generated` before rebuilding
`test_aot` (transitive-dep tracking gap).

## Outcome (refinements found during implementation — authoritative record: MASTERPLAN Phase-8 LANDED)

- **Item 2 premise was wrong: vector arithmetic already worked.** `scalar_class` already maps
  `tFloatN -> 2`, so `visitExprOp2` already emits component-wise `OpFAdd`/`OpFSub`/`OpFDiv` (and unary
  `-` -> `OpFNegate`) on the vector result type. The only real gap was item 3 (folded const-vector
  operand). 8.2 closed that and added a `vecarith` test to lock the (already-working) vector ops in.
- **Item 4 (math) became a name-correctness audit, not a "broaden".** The table was already broad but
  keyed on GLSL names while the emitter dispatches by daslang names: `mix`->`lerp`, `inversesqrt`->`rsqrt`,
  `refract` added; `step`/`smoothstep` removed (no daslang fn); `radians`/`degrees` removed (math_boost
  das functions with a `PI` global dependency the emitter can't lower). `mod` was not added (daslang `%`
  already covers float mod via `OpFRem`).
- **No GPU gate shipped** (language-surface; spirv-val + census + the per-ext-inst sub-opcode asserts are
  the oracle).
- **Out of scope, surfaced:** multi-component swizzle of a *global* (`fin.xyz`) is still unsupported — a
  separate swizzle gap, not control-flow/math.
