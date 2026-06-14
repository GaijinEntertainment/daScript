# dasSpirv — pure-daslang daslang→SPIR-V shader backend

> **This file is the durable masterplan and implementation history for dasSpirv.**
> It is the canonical, reviewable record — not Claude memory, not a scratch plan.
> Each phase appends a dated entry to the **Implementation log** at the bottom as it lands.

## Why

dasVulkan's shaders are authored as hand-written GLSL `.vert`/`.frag`/`.comp`, compiled
offline with glslangValidator into `.spv` blobs, *both* committed, and loaded at runtime.
Two languages, an external SDK, committed binaries, no code-sharing with the host.

dasGlsl already eliminated exactly this for OpenGL: shaders written in daslang, annotated
`[…_program]`, an `AstVisitor` (`GlslExport`) emits GLSL at compile time into a global
captured by `@@fn`, fed to `glShaderSource`. dasSpirv does the same for Vulkan, but emits
**SPIR-V binary directly from the daslang AST** — no GLSL/glslang intermediary, no LLVM.

SPIR-V is an SSA IR (typed results, basic blocks, structured control flow, a logical
builder) — the same shape `llvm_jit` already lowers daslang to. So dasSpirv is
**dasGlsl's frontend pattern + llvm_jit's SSA-backend pattern, fused into a SPIR-V emitter**.
Just as `[jit]` lowers daslang→LLVM→native, `[compute_shader]` lowers daslang→SPIR-V→GPU.

What makes it cheap: drivers do all GPU optimization (we emit naive *valid* SPIR-V — so does
glslang); SPIR-V binary is a self-delimiting word stream (trivial to emit and disassemble);
the backend-agnostic AST tooling (`collect_dependencies`, `collect_used_types`,
`make_visitor`) is reusable verbatim; and dasVulkan already has a GPU-verified `out[i]==i*i`
compute test as a ready-made end-to-end gate.

## Settled decisions

1. **Lives in the main tree as `modules/dasSpirv`** — sibling to dasGlsl/dasLLVM, in
   PR-protected GaijinEntertainment/daScript, sharing main-tree CI + `daslib/coverage` +
   `tests/`. SPIR-V is a general daslang capability (also useful for GL4.6/WebGPU), not
   Vulkan-specific.
2. **Fresh frontend** — new `[compute_shader]`/`[vertex_shader]`/`[fragment_shader]`
   annotations; dasGlsl is the *design map*, not a code dependency. Reuse only the generic
   AST tooling (none of which lives in dasGlsl). **Zero edits to the shipped dasGlsl/dasOpenGL.**
3. **Test-per-instruction is a hard requirement**, enforced by both an opcode census and the
   LCOV coverage gate.
4. **SPIR-V target 1.3 / Vulkan 1.1** — StorageBuffer storage class + `Block` (not the
   deprecated 1.0 BufferBlock+Uniform path). lavapipe advertises ≥1.2; 1.3 also dodges the
   1.4 rule that the entry-point interface must list *all* globals (≤1.3 lists only
   Input/Output). An annotation arg `spirv_version` leaves 1.0/1.4 selectable later; the
   interface builder must be made version-aware before any bump.

## Architecture

`modules/dasSpirv` is **pure daslang** (mirrors dasGlsl: a `spirv/` subdir of `.das` files +
a CMake `ADD_MODULE_DAS` rule; no `.shared_module`, no C++). dasVulkan consumes it via
`require dasSpirv/…` and feeds the emitted `array<uint>` (SPIR-V words) to `create_shader_module`.

| File | Gen/Hand | Purpose |
|---|---|---|
| `spirv/spirv_grammar.das` | **generated** | Opcode + GLSL.std.450 enums + StorageClass/Decoration/BuiltIn/ExecutionMode/Capability token constants, generated from the vendored, pinned SPIRV-Headers grammar JSON. Single source of numeric truth (+ opcode→name table for the disassembler). |
| `spirv/spirv_builder.das` | hand | `SpirvModule` (per-section append buffers), monotonic id allocator, type/constant dedup pools, `emit(section, opcode, …)`, `get_type_id`, `get_const_id`, `finalize() : array<uint>`. AST-agnostic. |
| `spirv/spirv_types.das` | hand | daslang `TypeDecl` → SPIR-V type-id (scalars, vec/mat, fixed + runtime arrays, structs, pointers) with layout decorations. Highest-detail correctness file. |
| `spirv/spirv_emit.das` | hand | `SpirvEmit : AstVisitor` codegen visitor + `[macro_function] generate_spirv(fn, var errors) : array<uint>`. Override set mirrors `GlslExport`; rejection list mirrors its error list. |
| `spirv/spirv_shader.das` | hand | `[compute_shader]`/`[vertex_shader]`/`[fragment_shader]` function-macros (subclass `SpirvShader : AstFunctionAnnotation`, modeled on dasGlsl's `GlslShader`). `apply` reserves an `array<uint>` global; `fixup` runs dependency collection, calls `generate_spirv`, sets `glob.init`. |
| `spirv/spirv_dis.das` | hand | Minimal disassembler + opcode-census helper (self-delimiting walk: word0 = `(wordCount<<16)\|opcode`). Symbolic via `spirv_grammar`'s opcode→name table. |
| `generator/gen_spirv_grammar.das` | hand | The mini-generator: reads vendored grammar JSON → emits `spirv/spirv_grammar.das`. |
| `spirv_headers/*.json` | vendored | Pinned `spirv.core.grammar.json` + `extinst.glsl.std.450.grammar.json` (provenance + commit recorded below). |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS(spirv …)` + install rule, modeled on `modules/dasGlsl/CMakeLists.txt`. |

**SSA backend (llvm_jit template).** `SpirvEmit` carries `e2v : table<Expression?;uint>`
(Expression→result-id), `v2v : table<Variable?;uint>` (Variable→pointer-id),
`ite2blocks`/`loop_stack` for control flow, and a hand-maintained `cur_block_terminated`
(SPIR-V has no builder to query — set on every terminator, cleared on `OpLabel`; guards
mirror llvm_jit's `current_block_terminates()`). Section buffers concatenate at `finalize`
in SPIR-V's mandated order: header(5) → capabilities → extensions → ext-imports →
memory-model → entry-points → exec-modes → debug → decorations → types+constants+global-vars
(interleaved, define-before-use, deduplicated) → functions.

**Capture mechanism (dasGlsl analog).** dasGlsl's `fixup` sets
`glob.init = new ExprConstString(value := text)`; ours builds an `ExprMakeArray` of
`ExprConstUInt` (one per SPIR-V word) for an `array<uint>` global named `"{func.name}`spirv"`.
`generate_spirv` is a standalone `[macro_function]` called by **both** `fixup` and the unit
tests — so opcode assertions hit the real codegen path without macro plumbing.

**Cache.** Mirror llvm_jit's content-addressed pattern: a `SPIRV_CODEGEN_VERSION` constant
FNV-folded with `get_function_aot_hash(fn)` (and the pinned SPIRV-Headers version), so
unchanged shaders skip recompile and codegen changes self-invalidate.

## Compute-first instruction set (Phase-1 milestone)

Replace dasVulkan's `examples/shaders/square.comp` (+committed `.spv`) with a daslang
`[compute_shader]` — SSBO `data[]` at set=0/binding=0, `gl_GlobalInvocationID`, body
`data[i] = i*i`, `local_size_x=64`. Opcode inventory (seeds the test matrix):

- **Preamble:** `OpCapability Shader`; `OpMemoryModel Logical GLSL450`;
  `OpEntryPoint GLCompute %main "main" %gid`; `OpExecutionMode %main LocalSize 64 1 1`.
- **Decorations:** `BuiltIn GlobalInvocationId`; `ArrayStride 4`; member `Offset 0`; `Block`;
  `DescriptorSet 0`; `Binding 0`.
- **Types/consts/globals:** `OpTypeVoid`/`OpTypeFunction`; `OpTypeInt 32 0`;
  `OpTypeVector %uint 3`; pointer types (Input, StorageBuffer); `OpConstant %uint 0`;
  `OpTypeRuntimeArray`; `OpTypeStruct`; global `OpVariable`s.
- **Body:** `OpFunction`/`OpLabel`; `OpAccessChain`; `OpLoad`; `OpIMul`; `OpStore`;
  `OpReturn`; `OpFunctionEnd`.

## Test architecture — "every emitted instruction has a test"

Three behavioral layers + two enforcement gates (all in main-tree `tests/spirv/` except GPU):

1. **Opcode-assertion units.** Each test compiles a tiny `[compute_shader]` fixture, calls
   `generate_spirv`, runs `spirv_dis` to a structured instruction list, and asserts the
   expected numeric opcode present/absent/operand-at-offset/decoration present. One test per
   emittable instruction kind.
2. **`spirv-val` gate.** Every blob the suite produces is validated via SPIRV-Tools
   `spirv-val` (`C:\VulkanSDK\1.4.350.0\Bin`). Soft-skip if absent locally, hard-required in
   CI — the real correctness oracle for structured-CFG and define-before-use bugs.
3. **Real-driver behavioral regression — as early as Phase 1.** A one-call framework in
   dasVulkan, `run_compute_spirv(words, n) : array<uint>` over the existing `compute_boost`,
   runs any emitted blob in ~2 lines. Primary gate = **local real GPU** (SDK 1.4.350),
   `out[i]==i*i`, every iteration. CI = lavapipe software (dasVulkan CI is lavapipe-only on
   `ubuntu-latest`; GitHub-hosted runners have no GPU, so no real-GPU CI lane — covered
   locally; a self-hosted GPU runner would wire straight in). Triangle pixels at Phase 3.

- **Gate A — LCOV coverage.** Run layer 1 under `dastest --cov-path spirv.lcov`
  (`daslib/coverage.das`); require ~100% of emission-dispatch lines in `spirv_emit.das` +
  `spirv_builder.das`.
- **Gate B — opcode census (stronger).** `spirv_dis` aggregates the opcode set across *all*
  fixtures; a meta-test asserts it equals the declared "supported opcodes" set. Catches the
  gap LCOV can't — an opcode whose emit line ran but was never *asserted*.

## Phasing (independently verifiable; no sizing)

- **Phase 0 — scaffolding + grammar + builder + disassembler.** This file; the generator →
  `spirv_grammar.das`; `spirv_builder.das`; `spirv_dis.das`. Phase-0 test: **hand-build the
  square module via builder primitives**, pass `spirv-val`, **byte-match the existing
  reference `.spv`**, eyeball disassembly vs external `spirv-dis`. Verify SPIR-V 1.3 on
  lavapipe + local GPU. Proves builder + validator + disassembler before any AST work.
- **Phase 1 — compute square end-to-end (milestone).** `spirv_types.das` + `spirv_emit.das`
  square subset + the three annotations + uint[]-global capture. Opcode tests + census +
  spirv-val + LCOV (main tree). dasVulkan: `run_compute_spirv`, replace `square.comp`,
  regress `out[i]==i*i` on local real GPU + lavapipe.
- **Phase 2 — control flow + arithmetic.** if/else (`OpSelectionMerge`), while + range-for
  (`OpLoopMerge`, 4-block), break/continue, full scalar/vector ops + comparisons,
  structured-return normalization decision.
- **Phase 3 — vertex/fragment.** Input/Output `Location` decorations, GLSL.std.450 math via
  `OpExtInst`, rasterizer builtins (`gl_Position`/`gl_VertexIndex`/`gl_FragCoord`),
  entry-point-interface correctness. Triangle on lavapipe + local GPU.
- **Phase 4 — resource breadth.** UBOs (Uniform+Block), push constants (PushConstant),
  combined image samplers (`OpTypeImage`/`OpTypeSampledImage`/`OpImageSample*`).

## dasVulkan-side changes (separate repo, PR-mode)

- Add `require dasSpirv` dependency (`.das_package`/`.das_module`).
- Add a `create_shader_module(device, code : array<uint>)` overload taking SPIR-V **words**
  directly (`codeSize = 4*length`, `pCode = addr(code[0])`).
- Phase 1: port `square.comp` → a daslang `[compute_shader]`, delete the committed `.spv`,
  point `test_compute` at the emitted blob.
- Phase 3: port `triangle.vert`/`.frag`, delete their `.spv`.

## Top risks

1. **Type/constant dedup correctness** — keys must be injective + complete (pointer key
   includes storage class; array key includes stride/length; struct key includes decorated
   layout), built from already-allocated constituent ids. SPIR-V *requires* dedup. Guarded
   by spirv-val.
2. **SSBO decoration completeness** — missing ArrayStride/Offset/Block/DescriptorSet/Binding
   either fails validation or reads garbage on-GPU. Guarded by `.spv` byte-match + spirv-val
   + GPU test together.
3. **Terminator tracking** — one terminator per block, nothing after it. The hand-maintained
   `cur_block_terminated` + merge handling for early returns in loops/selections is the
   likeliest invalid-CFG source. Mirror llvm_jit's close-paths.
4. **daslang→SPIR-V type/layout mapping** — column-major + MatrixStride, std430 vec3
   alignment, fixed vs runtime arrays, `float3`/`int3` handle-types.
5. **uint[]-global capture compiling** — `ExprMakeArray` needs explicit `makeType`; whether a
   module-scope const `array<uint>` initialized by `ExprMakeArray of ExprConstUInt` is usable
   must be smoke-tested early. Fallbacks: an `[export]` accessor, or fixed `uint[N]`.
6. **Entry-point interface version rule** — 1.3 sidesteps the 1.4 "all globals" rule for now;
   the interface builder must be version-aware before any bump (Phase 3/4).
7. **lavapipe SPIR-V version + generator drift** — verify lavapipe advertises 1.3 in Phase 0
   (else fall back to 1.0 BufferBlock); pin the SPIRV-Headers commit and fold its version into
   `SPIRV_CODEGEN_VERSION`.

## Verification (end to end)

- **Phase 0:** the generator regenerates `spirv_grammar.das`; the hand-built-square test
  byte-matches `square.comp.spv` and passes `spirv-val`; `vulkaninfo` confirms SPIR-V 1.3 on
  both lavapipe and the local GPU.
- **Per phase (main tree):**
  `daslang dastest/dastest.das -- --test tests/spirv --cov-path spirv.lcov --isolated-mode`
  → all green, opcode census == declared set, LCOV ~100% emission dispatch, every blob
  spirv-val-clean, no `GC APP LEAK`.
- **Disassembly eyeballing (every phase):** each emitted module is dumped via `spirv_dis`
  (symbolic) and diffed against external `spirv-dis` as ground truth.
- **Real-driver gate (dasVulkan, Phase 1+):** the integration suite under lavapipe + the
  local real GPU → `out[i]==i*i` (Phase 1), triangle pixel checks (Phase 3).
- **Lint/format:** MCP `format_file` on every new `.das`; `lint` clean. Both repos PR-mode.

---

# Implementation log

### Phase 0 — started 2026-06-13 (branch `bbatkin/dasspirv-backend`)

- Module home confirmed: `modules/dasSpirv` (main tree); branch cut fresh off `origin/master`
  (`40146ecd2`). dasGlsl/dasLLVM confirmed present as sibling references.
- Reference `.spv` for the byte-match oracle: `D:/DASPKG/dasVulkan/examples/shaders/square.comp.spv`
  (916 bytes).
- SPIRV-Tools located: `C:\VulkanSDK\1.4.350.0\Bin\spirv-val.exe`, `spirv-dis.exe`.
- SPIRV-Headers grammar (core 1.6 rev4, 871 instructions) + GLSL.std.450 grammar located in
  the SDK at `Include/spirv-headers/`; to be vendored + pinned under `spirv_headers/`.
- Dev loop: pure-das modules are interpreted, so a daslang binary is used read-only with
  **same-dir requires** during development; CMake `ADD_MODULE_DAS` registration (needed only for
  bare-name `require dasSpirv`) is deferred to end of Phase 0/1.
- **Dev-binary correction (Phase 1):** the `E:\daslang\bin\Release\daslang.exe` binary is **0.6.2**
  (pre-tFixedArray-rework) — its daslib has no `make_fixed_array_type` and TypeDecl has no
  `fixedDim` field (it uses the old `.dim` array). The worktree is **0.6.3**. Phase-0 code happened
  not to touch fixed-array AST construction so 0.6.2 sufficed, but the Phase-1 `array<uint>` capture
  (`make_fixed_array_type` + `to_array_move`) is 0.6.3-only. So Phase-1 dev requires the
  **worktree's own built `build/.../daslang.exe`** (0.6.3), NOT E:\daslang. Don't code to 0.6.2
  spellings — they'd break the 0.6.3 CI build.

**Done this session:**
- `spirv_headers/` vendored: core 1.6 rev4 (871 instructions) + GLSL.std.450 v100 rev2 (81),
  from Vulkan SDK 1.4.350.0.
- `generator/gen_spirv_grammar.das` → `spirv/spirv_grammar.das`: typed `enum : uint` —
  `SpvOp` (871) + `SPV_OP_NAMES` table (871, for the disassembler) + 57 operand enums
  (`SpvStorageClass`/`SpvDecoration`/`SpvBuiltIn`/`SpvCapability`/… = 1077 enumerants) +
  `GLSLstd450` (81). Compiles + loads clean. **Correction:** an earlier pass used `let`
  constants on the (wrong) assumption that SPIR-V's enumerant aliases — many names sharing one
  value, e.g. the `*KHR`/`*EXT` promotions — would collide in an enum. Probed the compiler:
  **daslang typed enums permit duplicate member values**, so typed enums are used (Boris's
  call — namespacing + type-safety + `for x in type<E>`). Enum *type* names are `Spv`-prefixed
  to avoid collision when `spirv_emit` requires both `daslib/ast` and the grammar; the 4
  digit-leading enumerants (`SpvDim._1D`, …) get a `_` prefix. `emit`/`has_op`/`count_op` take
  the typed `SpvOp`; operand enums are `uint()`-cast at the call site.
- `spirv/spirv_builder.das`: `SpirvModule` (10 section buffers in mandated layout order),
  id allocator, arity-overloaded `emit` (0–4 operands) + `emit_n` (variadic), `string_words`
  (LSB-first NUL-terminated zero-padded), `module_words` finalize, `finalize`/`delete`.
- `spirv/spirv_dis.das`: self-delimiting `foreach_inst` + `has_op`/`count_op`/`opcode_set`/
  `op_has_operand`/`dump`.
- `spirv/_handbuild_square.das` (dev scaffold, `_`-prefixed so dastest skips it): hand-builds
  the `square.comp` compute module via builder primitives.

**Phase-0 GATE RESULT (the key unknown — resolved):** the hand-built module passes
`spirv-val --target-env vulkan1.1` (exit 0) and `spirv-dis` round-trips it. Structurally it is
*cleaner* than glslang's reference — SPIR-V 1.3 + StorageBuffer/`Block` vs glslang's 1.0 +
Uniform/`BufferBlock`, and it omits glslang's redundant legacy `gl_WorkGroupSize` composite.
Pure-daslang emission of valid Vulkan SPIR-V is proven. No `GC APP LEAK`.

**Byte-match correction (plan amendment):** byte-matching glslang's `.spv` is infeasible —
glslang stamps a different generator-magic word, SPIR-V version, and id-numbering, and adds
debug `OpName`/`OpSource`. The honest Phase-0 oracle is **spirv-val clean + structural
disassembly equivalence + (Phase 1) GPU `out[i]==i*i`**, plus a byte-snapshot of *our own*
validated output as a forward regression guard. This replaces "byte-match the reference".

**Still open in Phase 0:** convert `_handbuild_square.das` into a kept `tests/spirv/` dastest
(overlaps the Phase-1 harness + `--cov-path` wiring); CMake `ADD_MODULE_DAS` registration;
confirm SPIR-V 1.3 on lavapipe + local GPU (lands with the Phase-1 dasVulkan GPU gate).

### Phase 1 — started 2026-06-13

**Type/constant dedup (the #1 risk) — proven.** Added cached `type_void/bool/int/uint/float/
vector/pointer/function_void/runtime_array/struct_block` + `const_uint` to `spirv_builder.das`
(0-sentinel pool lookup → emit-once → cache). `_square_typed.das` rebuilds the square module
through them: byte-identical to the hand-built module (bound 20, 125 words), `spirv-val` clean,
and assertions confirm dedup (1 `OpTypeInt`/1 `OpConstant`/4 `OpTypePointer` despite repeated
requests; id-bound doesn't grow on re-request).

**Authoring surface — DECIDED: mirror dasGlsl (Boris's call).** Same pattern as dasGlsl:
builtins are plain `gl_*` globals recognized by name; I/O globals carry `@`-stacked field
annotations; function macros `[compute_shader]`/`[vertex_shader]`/`[fragment_shader]` (mirroring
`[…_program]`) with `local_size_x/y/z`; `@@fn` captures the emitted blob. Annotation set,
identical to dasGlsl where semantics match, Vulkan-specific where they don't:
`@in`/`@out`/`@inout` (stage I/O → `Location`), `@location(N)`, `@uniform` (UBO → Uniform/Block,
Phase 4), `@ssbo` (storage buffer → StorageBuffer/Block), plus the Vulkan descriptor model
`@binding(N)` + `@set(N)` (default 0) — OpenGL has no descriptor sets, so this is the one
faithful-but-Vulkan-correct deviation from dasGlsl's flat `@stage`. A bare `var @ssbo data :
array<uint>` auto-wraps in the required Block-decorated struct (member 0 = the runtime array).
Target authoring form for square:
```das
var @ssbo @binding(0) data : array<uint>
[compute_shader(local_size_x=64)]
def square { let i = gl_GlobalInvocationID.x; data[i] = i * i }
```

**`spirv_types.das` done + validated** — `emit_type(TypeDecl) -> id` for scalars + 2/3/4
vectors, dedup-backed (`_types_unit.das`: uint emitted once across scalar + uint3-component +
re-request; bound stays 5). `emit_component_type` for vector lanes.

**Square AST shape (dumped via a throwaway `[dump_ast]` macro overriding the generic
`preVisitExpression` in `fixup`) — the reference for writing the emitter:**
```
body statements (ExprBlock.list):
  let i = gl_GlobalInvocationID.x
      init = ExprSwizzle (uint&)   over  ExprVar gl_GlobalInvocationID (uint3& -const)
             single-component swizzle "x"
  data[i] = i * i
      ExprCopy (void):
        L = ExprAt (uint&)  [ ExprVar data (array<uint>& -const), ExprVar i (uint const) ]
        R = ExprOp2 "*" (uint)  [ ExprVar i (uint const), ExprVar i (uint const) ]
```
Key facts driving codegen: **no `ExprRef2Value` nodes are inserted** — lvalue-ness is read
straight from the type's `&` (ref) flag. `let i` is a pure value (`uint const`, bare `ExprVar`);
memory/refs (`data`, `gid`, `data[i]`, `gid.x`) carry `&`. `ExprLet` is *not* delivered to
`preVisitExpression` (handle it by iterating `ExprBlock.list` / `preVisitExprLet`).

**Emitter design — DECIDED: manual recursion (not the auto AstVisitor).** SPIR-V's
value/pointer model maps cleanly to two mutually-recursive functions; the auto-visitor's
bottom-up traversal fights lvalue/rvalue and (later) structured control flow. Deviation from
"SpirvEmit : AstVisitor" in the plan — revisit for Phase 2 if a visitor proves better.
- `emit_value(m, ctx, e) : uint` — rvalue/SSA id. `ExprVar` of a `let` → recorded SSA id;
  `ExprVar`/`ExprAt`/`ExprSwizzle` with `&` → `emit_ptr` then `OpLoad`; `ExprOp2 "*"` (uint) →
  `OpIMul`; `ExprConst*` → `const_*`.
- `emit_ptr(m, ctx, e) : {id, pointee_type, storage}` — lvalue. `ExprVar` global → its
  `OpVariable` id; `ExprAt` on an SSBO array → `OpAccessChain(var, const0 /*member*/, idx)`;
  single-component `ExprSwizzle` → `OpAccessChain(var, const(component))`.
- `emit_stmt(ExprCopy)` → `ptr = emit_ptr(L); val = emit_value(R); OpStore`.
- block driver: iterate `fn.body` (ExprBlock) statements; `ExprLet` → bind each var →
  `emit_value(init)`; else `emit_stmt`.
- global model: `GlobalInfo { var_id, storage, is_ssbo_array, elem_type }` keyed by Variable*;
  classify `gl_*` → builtin Input var + `BuiltIn` deco; `@ssbo` array → StorageBuffer wrapper
  Block-struct + `DescriptorSet`/`Binding`. First codegen pass may classify by name/type, then
  switch to reading `@ssbo`/`@binding`/`@set` annotations (mirror dasGlsl's `find_arg`).
- `generate_spirv(fn, var errors) : array<uint>` orchestrates: `collect_dependencies` → emit
  globals → entry point (+ interface = Input/Output vars) → `OpExecutionMode LocalSize` →
  function body. Oracle: must match the hand-built `_square_typed.spv` (spirv-val + `out[i]==i*i`).

**EMITTER WORKS END-TO-END — the project's hardest unknowns are retired (2026-06-13).**
`spirv_emit.das` (manual `emit_value`/`emit_ptr` recursion + `generate_spirv`), `spirv_shader.das`
(the `[compute_shader]` annotation), and `spirv_builtins.das` (`gl_*` globals) are written; the
real `[compute_shader] def square` fixture (`_emit_square.das`) lowers to a **125-word, id-bound-20
module that passes `spirv-val --target-env vulkan1.1` (exit 0)** and disassembles cleanly under
external `spirv-dis` (SPIR-V 1.3, the exact `data[gid.x] = gid.x*gid.x` shape). Same word count +
id bound as the hand-built `_square_typed.spv`; bytes differ only by section/decoration *ordering*
(both valid — byte-identity was never the oracle).

**Hard-won findings this session (all load-bearing):**

1. **Dev binary must be the worktree's own 0.6.3 build, not `E:\daslang` (0.6.2).** See the
   dev-binary correction in the Phase-0 log. Built `build/` + `bin/Release/daslang.exe` (0.6.3) in
   the worktree; all dev runs use it.
2. **`array<uint>` literal AST = `to_array_move(ExprMakeArray{makeType=ELEMENT(uint), gen2=true})`.**
   The `gen2=true` flag is mandatory — it drives the fixed→dynamic conversion. The pre-gen2
   `make_fixed_array_type(...)`-wrap pattern (from `ast_boost`'s older `walk_and_convert`) is WRONG
   for a gen2 makeType and produced a malformed node.
3. **Capture = global declared in `apply` + init filled in `patch` (NOT `fixup`).** Two independent
   constraints force this:
   - The capture symbol must exist after `apply` so consumer code referencing it resolves on the
     first infer. Declaring it later (patch/fixup) **deadlocks**: the consumer fails inference,
     and `program->patchAnnotations()` is gated behind `!program->failed()` (ast_parse.cpp:920) — so
     patch never runs. (An accessor *function* with an `array<uint>` body also fails: an empty body
     is rejected as "does not return a value" before any hook fills it.)
   - The init (a `to_array_move` **call**) must be set in `patch` with `astChanged=true`, which
     triggers `restartInfer` so the call is resolved. Setting it in `fixup` (no further infer)
     leaves the call's `func` null → **crash in `deriveAliases`/`ExprCall::visit`**. (dasGlsl gets
     away with `fixup` only because `ExprConstString` is self-typed and call-free.)
4. **Running `generate_spirv` in `patch` sees a LESS-FOLDED AST than `fixup`** — `ExprRef2Value`
   wrapper nodes are still present (the Phase-1 AST dump was taken in fixup, where they're folded
   away). The emitter handles them: `emit_value(ExprRef2Value)` recurses on `.subexpr`. This
   corrects the Phase-1-dump claim "no ExprRef2Value nodes are inserted" — true at fixup, false at
   patch.
5. **Field-annotation args use `@name = value`, NOT `@name(value)`** (ds2_parser `annotation_argument`).
   So `var @ssbo @binding = 0 data : array<uint>`. (The earlier `@binding(0)` in this doc was wrong.)
6. **`require daslib/fio`, not bare `require fio`** in 0.6.3 (bare form was a 0.6.2 affordance).

Corrected authoring form:
```das
var @ssbo @binding = 0 data : array<uint>
[compute_shader(local_size_x=64, name="square_spv")]
def square { let i = gl_GlobalInvocationID.x; data[i] = i * i }
// host reads the `square_spv : array<uint>` global -> vkCreateShaderModule
```

## Phase 1 — test suite + module registration LANDED (2026-06-13)

The emitter is now wired into the main-tree test + AOT machinery. `modules/dasSpirv/CMakeLists.txt`
registers the seven `spirv/*.das` files via `ADD_MODULE_DAS(spirv spirv <name>)`, so consumers
`require spirv/spirv_shader` etc. `tests/spirv/` holds the kept dastests (the dev scaffolds
`_emit_square` / `_square_typed` / `_types_unit` / `_handbuild_square` were converted and deleted):

- `_spirv_common.das` — shared module: the `[compute_shader] square` fixture (→ public `square_spv`
  global), `validate_spirv()` (shells out to SPIRV-Tools `spirv-val`, soft-skips if absent), and
  `phase1_emitter_opcodes()` (the declared census set).
- `test_compute_square.das` — opcode-shape asserts on the captured blob + spirv-val gate.
- `test_census.das` — gate B: emitted opcode set == declared set, both directions.
- `test_builder.das` — hand-built square via builder primitives: dedup asserts + spirv-val.
- `test_types.das` — `emit_type` dedup unit.

All 10 subtests green across all three tiers (interpreter, JIT, AOT); the spirv-val gate genuinely
runs (exit 0) with `VULKAN_SDK` set. Registered in `tests/aot/CMakeLists.txt` (5-site pattern:
`test_aot_spirv` for the test files, `test_aot_spirv_modules` via `DAS_AOT_LIB` for `_spirv_common.das`).

**Findings (load-bearing):**

1. **`require spirv/X` resolution needs a `daslang.exe` rebuild.** `ADD_MODULE_DAS` appends to
   `external_resolve.inc`, which is `#include`d by `src/simulate/fs_file_info.cpp` — so registering a
   module recompiles that TU. The `.temp` is written at configure; the real `.inc` updates via the
   `need_and_resolve` target at build. Reconfigure (`cmake -B build`) then rebuild `daslang`.
2. **The captured global is PUBLIC** (`add_global_var(..., priv=false)`), so the fixture can live in a
   shared module and the tests `require` it to read `square_spv` — single emit source.
3. **LCOV (gate A) cannot measure the emitter.** `generate_spirv` runs at COMPILE time (the
   `[compute_shader]` macro's `patch`); LCOV measures RUNTIME line hits. So `spirv_emit.das` shows
   0/N under `--cov-path` — that is expected, not a gap. The **census gate (B) is the emitter's
   coverage proxy.** LCOV meaningfully covers the runtime-exercised files (`spirv_builder`,
   `spirv_types`, `spirv_dis`) — their numbers grow as later phases add tests.
4. **Coverage instrumentation breaks shader bodies → `[marker(no_coverage)]` required.**
   `daslib/coverage` injects a `coverageData` global + `ExprCall` increment statements into every
   instrumented function; the SPIR-V emitter can't lower those (annotation `patch` fails under
   `--cov-path`). Mark shader fixtures `[marker(no_coverage)]`. Annotations must share one bracket:
   `[compute_shader(...), marker(no_coverage)]` — stacked `[...][...]` is a parse error (30151).
5. **Scaffold footgun (now removed):** `get_command_line_arguments()` returns the *script path* as the
   last arg, so a scaffold that derived its `.spv` output path from argv overwrote its own source with
   binary. The dastests never derive paths from argv (they use `create_temp_file_result`).
6. `emplace` needs a reference-type value (31400); strings (workhorse) use `push`.
7. **A shared test module named `_common` collides under AOT.** `tests/linq/_common.das` already
   declares `module _common`; a second `module _common` (in `tests/spirv/`) makes `test_aot` link
   fail with `LNK2005: das::impl_aot__common already defined` — the AOT-lib registrar symbol is
   `impl_aot_<module-name>`. Renamed to `_spirv_common` (file + module). daslang requires
   **require-string == module-name == file-basename** to all agree (else `error[20605] wrong module
   name`), so the file had to be renamed too, not just the `module` line.
8. **JIT/AOT need the LLVM-enabled build config.** A bare local configure leaves
   `DAS_LLVM_DISABLED=ON` (and `DAS_SQLITE_DISABLED=ON`); `-jit` then dies at `can't load LLVM.dll`.
   Configure to match `ci/release_modules.txt`: `-DDAS_LLVM_DISABLED=OFF -DDAS_SQLITE_DISABLED=OFF`.
   dasLLVM's CMake provisions `lib/LLVM.dll` itself (find_package → falls back to the pinned
   `llvm-v22.1.5/win64_llvm.tar.gz` prebuilt; copies `LLVM.dll`+`clang-cl.exe`). Toggling LLVM flips
   the exceptions→setjmp compile def → full daslang rebuild. JIT then runs end-to-end (no manual
   `lld-link` copy needed; clang-cl finds its linker).
9. **The runtime spirv modules must be in `test_aot`'s `DAS_AOT_LIB` set, not just the test fixture.**
   Listing only `tests/spirv/_spirv_common.das` left the tests' transitive builtin-generic
   instantiations namespaced to `spirv_builder` (array/table `finalize`, `to_array_move`, `resize`,
   `push_from`) with no AOT stub → `error[50101]`. Fix mirrors dasSQLITE/pugixml: add
   `modules/dasSpirv/spirv/{spirv_grammar,spirv_builder,spirv_types,spirv_dis,spirv_builtins}.das`
   to `AOT_SPIRV_MODULE_FILES` (macro-only `spirv_emit`/`spirv_shader` excluded — no runtime funcs).
   Local gotcha: DAS_AOT custom commands don't track transitive module-`.das` deps, so after editing
   a spirv module file, `rm -rf tests/spirv/_aot_generated` before rebuilding `test_aot` (CI builds
   clean, so it never hits this). All three tiers verified green (interp/JIT/AOT 10/10) +
   `spirv-val` clean. The only full-suite failure is the pre-existing, unrelated
   `tests/fio/test_format_time.das` (time/locale).
10. **`DASLANG` env points preflight at the wrong binary.** `utils/preflight` resolves `$DASLANG`
   first; on this machine it's the main checkout (`d:/Work/daScript/...`, no dasSpirv), so every
   gate ran against a binary lacking the spirv module → false `spirv/* file not found`. Run preflight
   with `DASLANG` set inline to the worktree's `bin/Release/daslang.exe`.

**NEXT:** dasVulkan `run_compute_spirv` GPU gate (`out[i]==i*i` on local real GPU + lavapipe CI) —
the cross-repo Phase-1 piece. Then Phase 2 (control flow + arithmetic). Dev binary: the worktree's
LLVM-enabled `bin/Release/daslang.exe`.

## Phase 1 — CROSS-REPO GPU GATE GREEN, milestone fully closed (2026-06-14)

daslang #3133 merged to master (`6caf9bf59`). The dasVulkan side ([borisbat/dasVulkan#3](https://github.com/borisbat/dasVulkan/pull/3))
replaces the committed `square.comp(.spv)` with a daslang `[compute_shader]` lowered by dasSpirv:

- New `create_shader_module(device, code : array<uint>)` overload (SPIR-V words; `codeSize = 4·len`).
- The `[compute_shader] square` fixture emits the public `square_spv : array<uint>` global; the raw
  (`run_compute`) and boost (`run_compute_spirv`/`run_compute_boost`) descriptor paths feed it.
- `examples/compute.das` authors its own inline `[compute_shader]`.

**Result:** `out[i]==i*i` green on the **local real GPU** (integration suite 5/5) AND **lavapipe CI**
(PR #3 `build`+`integration` pass). dasVulkan CI builds daslang from `master`, which compiles dasSpirv
in unconditionally (top-level CMake auto-includes every `modules/*/CMakeLists.txt`); the macro runs
interpreted, so the `-DDAS_LLVM_DISABLED=ON` CI build is unaffected. Pure daslang→SPIR-V on a real GPU,
no GLSL/glslang/committed `.spv` anywhere.

### Phase 2 — arithmetic breadth (in progress, branch `bbatkin/dasspirv-phase2`)

First Phase-2 slice: full scalar binary + unary arithmetic. `emit_value`'s `ExprOp2` went from
`*`-only to a `binop_code(op, scalar_class)` table; added an `ExprOp1` handler via `unop_code`.

- **Binary** (`+ - * / %`): `IAdd`/`ISub`/`IMul` (sign-agnostic), `SDiv`/`UDiv`/`FDiv`, `SRem`/`UMod`/`FRem`.
- **Unary** (`- ~`): `SNegate`/`FNegate`, `Not`.
- **Op-semantics probe (load-bearing):** daslang `/` truncates toward zero (`-7/3 == -2`) and `%`
  takes the **dividend's** sign (`-7%3 == -1`, `7%-3 == 1`) — C semantics. So int `%` → `OpSRem`
  (dividend's sign), NOT `OpSMod` (divisor's sign); int `/` → `OpSDiv` (truncating). uint → `UDiv`/`UMod`;
  float → `FDiv`/`FRem`.
- **Node shapes (probed):** `+ - * / %` are all `ExprOp2`; unary `-`/`~` are `ExprOp1` (`{op, subexpr}`).
  No `ExprCall` lowering for these on builtin scalars.
- **Tests:** `tests/spirv/test_arith.das` — uint/int/float fixtures in `_spirv_common.das` (operands are
  loaded SSBO values + the uint index, so no constants/conversions needed yet), per-op opcode-shape
  asserts + spirv-val clean. Interp + JIT green (16/16); `marker(no_coverage)` on the new fixtures.
  Constants (`const_int`/`const_float`/`const_bool`), comparisons (bool result), and control flow
  (mutable locals as `Function` `OpVariable`, `OpSelectionMerge`/`OpLoopMerge`) are the next slices.

**Reserved word:** `label` is a daslang keyword — a parameter named `label` is `error[30151]`. Use `lbl`.

### Phase 2 — control flow LANDED (2026-06-14, branch `bbatkin/dasspirv-phase2`)

Second Phase-2 slice: mutable locals + comparisons + logical ops + the full structured-CFG set
(if/else, while, range-for, break/continue). All three tiers green (interp/JIT/AOT 26/26),
spirv-val clean, external `spirv-dis` confirms textbook structured CFG, lint + format clean.

- **Mutable locals as Function `OpVariable`s.** A pre-scan (`collect_locals`) walks the WHOLE body
  once before any body code and declares an `OpVariable` (Function storage) for every mutable local
  (`var`) and loop-induction variable — SPIR-V requires all Function-storage OpVariables to lead the
  entry block. Reads → `OpLoad`, writes → `OpStore`; the driver mem2reg's them (NO hand-rolled
  `OpPhi`). Immutable value-lets (`let`) stay pure SSA ids (the Phase-1 path) — so the square's
  `let i` is unchanged and the census/exact-count asserts still hold. Discriminator (probed):
  `var._type.flags.constant` — `false` for `var` (→ memory), `true` for `let` and loop-iter vars.
- **Compound assignment** (`+= -= *= /= %=`, void-typed `ExprOp2`) and **`++`/`--`** (statement-level
  `ExprOp1`) → load-modify-store via `emit_load_op_store`.
- **Comparisons** (`== != < > <= >=`, `ExprOp2` → bool): `cmp_code` table picks I/S/U/FOrd opcode by
  operand class. Equality is sign-agnostic (`IEqual`/`INotEqual`); ordering splits signed/unsigned/
  float; floats use the **ordered** (non-NaN) forms.
- **Logical** `&&`/`||` (`ExprOp2` → bool) → `OpLogicalAnd`/`OpLogicalOr`; `!` (`ExprOp1`) →
  `OpLogicalNot`. **Eager, NOT short-circuit** — valid because shader condition operands are
  side-effect free (documented in-code); true short-circuit would need extra control flow.
- **if/else** → `OpSelectionMerge` + `OpBranchConditional`, then/else/merge blocks. Early-`return`
  in a branch is handled by the per-block `terminated` flag (skip the trailing `OpBranch merge`).
- **while + range-for** → the canonical 4-block `OpLoopMerge` shape: header (LoopMerge → branch to
  cond) / cond (`OpBranchConditional body merge`) / body / continue (→ header), + merge. **break →
  `OpBranch merge`, continue → `OpBranch continue`** off a `loop_stack`. range-for's `i < hi` test
  and `i += 1` update are synthesized directly against the induction `OpVariable` (no daslang AST
  node exists for them); bounds are evaluated ONCE in the pre-header (range captures at entry).
- **Tests:** `tests/spirv/test_control.das` (uint/int/float fixtures — every comparison flavor +
  logical + if/else) and `test_loops.das` (while+break, range-for+continue, both `range(n)` and
  `range(lo,hi)`). Per-op opcode-shape asserts + spirv-val on every blob.

**Findings (load-bearing):**

1. **Constant-bound `range()` folds to `ExprConstRange`, NOT an `ExprCall`.** `range(10)` / `range(2,8)`
   with literal bounds arrive as `ExprConstRange` (a half-open `[from,to)` value); only
   *runtime*-bound ranges stay an `ExprCall` to `range`/`urange`. The emitter handles both. Read the
   const bounds via `unsafe(reinterpret<int2>(cr.value))` → `(from, to)` (daslang `range` is a
   vector-like pair; `.from`/`.to` parse as swizzle masks and fail — `reinterpret` is the way).
2. **`continue if (cond)` desugars to a plain `if (cond) { continue }`** — no special postfix handling
   needed; `ExprBreak`/`ExprContinue` are leaf statements.
3. **At patch (pre-fold) stage, mutable-local reads are `ExprVar : T&` (non-const ref) wrapped in
   `ExprRef2Value`; value-let / loop-var reads are `T const&`.** The const flag on the *variable's*
   `_type` (not the read site) is the stable mutable-vs-SSA discriminator at the `ExprLet` site.

**Census gate (B) now aggregates across ALL fixtures.** `test_census.das` unions `opcode_set` over
every fixture (square + arith + control + loops) and asserts it equals the declared
`phase2_emitter_opcodes()` set in both directions (failures resolve the numeric opcode to its OpName).
This caught a real gap: float `%` (FRem) was reachable in the emitter but no fixture exercised it, so
a `fdata[i] % 2.0f` line was added to `farith` (daslang float `%` is fmod) — test-per-instruction
restored. The declared set lists only the comparison flavors the fixtures actually emit (e.g. no
unsigned `>=`/`<=`), so the both-directions check stays exact.

**Phase 2 COMPLETE** — arithmetic (commit 1) + control flow (commit 2) + aggregate census (commit 3).
All three tiers 28/28, spirv-val clean, lint + format clean.

**Review hardening (#3137).** Copilot review surfaced an "unvalidated scalar class → silently emits an
invalid op" class of latent bug. Fixed across the dispatch surface and audited for completeness:
`const_float` keys by raw IEEE-754 bits (not stringified — `-0.0`/`0.0`/NaN payloads stay distinct;
regression test added); `binop_code`/`unop_code` reject `cls < 0` (and `~` is gated to int/uint) so an
unsupported operand type becomes a clean error instead of a masquerading integer op; `emit_for` guards
the induction class; `declare_local_var` rejects unsupported local types (e.g. `var d : double`) with a
clean error instead of letting `emit_type` panic. Net rule for the emitter: **every type/opcode
dispatch validates its input and produces a clean `error[...]`, never a silent bad blob or a panic.**
The remaining `emit_type`/`emit_component_type` panics are now unreachable for valid daslang (all
callers pre-validate); they stand as internal invariants.

### Phase 3 — vertex/fragment shaders LANDED (2026-06-14, branch `bbatkin/dasspirv-phase3`)

The emitter goes from compute-only to all three rasterization-relevant stages. All three tiers green
(interp/JIT/AOT 36/36), every emitted blob spirv-val clean, external `spirv-dis` confirms textbook
vertex/fragment structure, lint + format clean.

- **Stage parameterization.** `generate_spirv` gained a `stage : ShaderStage` (Compute/Vertex/Fragment)
  argument driving the entry-point execution model (`GLCompute`/`Vertex`/`Fragment`) and execution mode:
  Compute → `LocalSize`, Fragment → `OriginUpperLeft`, Vertex → none. The annotation classes carry the
  stage via a virtual `shader_stage()` method — `SpirvComputeShader`/`SpirvVertexShader`/
  `SpirvFragmentShader` are now all `[function_macro]` subclasses of `SpirvShader` (the base defaults to
  Compute, vertex/fragment override). New annotations: `[vertex_shader]`, `[fragment_shader]`.
- **Stage I/O (`@in`/`@out` + `@location`).** `classify_global` now recognizes `@in`/`@out` user
  variables: Input/Output `OpVariable` + a `Location` decoration, listed in the entry-point interface.
  `@location` is **required and must be non-negative** (fail-closed: a missing/negative location is a
  clean error, not a silent collide-at-0). `@in`+`@out` together is rejected.
- **Rasterizer builtins.** `builtin_info` extends the compute builtin table with `gl_Position` (Output,
  BuiltIn Position), `gl_VertexIndex` / `gl_InstanceIndex` (Input, int) and `gl_FragCoord` (Input,
  float4). The storage class is now part of the builtin record (compute IDs + the indices/FragCoord are
  Input; only `gl_Position` is Output) — `classify_global` emits the right pointer storage + lists both
  Input and Output builtins in the interface. **`gl_Position` is a standalone Output decorated `BuiltIn
  Position`** (no `gl_PerVertex` block) — spirv-val `--target-env vulkan1.1` accepts it; the GLSL ABI
  block is not a SPIR-V requirement.
- **Entry-point interface (SPIR-V ≤ 1.3).** The interface lists exactly the Input/Output vars (stage I/O
  + Input/Output builtins); SSBO StorageBuffer globals are correctly excluded (the ≤1.3 rule).
- **Vector constructors → `OpCompositeConstruct`.** `float2/3/4`, `int2/3/4`, `uint2/3/4` arrive as
  `ExprCall`. One scalar arg → splat (repeat the lane N times); one same-width vector arg → identity
  (pass through); multiple args → CompositeConstruct of the constituents, each contributing its lanes
  (a `vec2` contributes 2, so `float4(vec2, s, s)` works). **Lane count is validated** (constituent
  lanes must sum to N) — a malformed constructor is a clean error.
- **`dot` → core `OpDot`.** Not an ext-inst — the core op (vector·vector → scalar).
- **GLSL.std.450 math → `OpExtInst`.** `OpExtInstImport "GLSL.std.450"` is emitted lazily (once, cached
  in `ctx.glsl_ext`) the first time a math builtin is used. `glsl_ext_op` maps daslang math names to the
  ext-inst opcode, **class-aware** for `abs`/`min`/`max`/`clamp`/`sign` (F/S/U variant by operand class)
  with the rest float-only; an unsupported name or wrong class is a clean error (fail-closed).
- **Tests:** `tests/spirv/test_raster.das` — vertex passthrough (`@in`/`@out` + Location, `gl_Position`,
  `float4(vec2, s, s)`), fragment passthrough (`OriginUpperLeft`, `float4(vec3, s)`), vertex
  builtins (`gl_VertexIndex`/`gl_InstanceIndex`), fragment math (`gl_FragCoord` + dot/sqrt/clamp). The
  EntryPoint execution model is asserted via a new `op_first_operand` helper in `spirv_dis` (positional —
  `op_has_operand` is unreliable because `Vertex == 0` collides with the name's NUL word). Census gate
  extended to `phase3_emitter_opcodes` = Phase-2 set + `CompositeConstruct`/`Dot`/`ExtInstImport`/
  `ExtInst`, unioned over the four new fixtures.

**Findings (load-bearing):**

1. **`dot`/`sqrt`/`clamp`/etc. need `require math` in the fixture file.** They're `math` builtins; without
   the require they fail typecheck (`module math is not visible directly`) before the emitter ever runs.
2. **Class virtual method call: drop `self->`.** `self->shader_stage()` trips STYLE028 — call
   `shader_stage()` unqualified inside the method; the compiler auto-promotes to the same virtual invoke.
3. **`gl_Position` standalone Output works** (no `gl_PerVertex` block needed for Vulkan ≤1.3) — confirmed
   by spirv-val. Keeps the emitter simple; the block wrapper can come later only if a stage needs
   `gl_PointSize`/`gl_ClipDistance`.
4. **Local-value swizzles (`p.x` on a `let`) are NOT yet supported** — only single-component swizzle of a
   *global* (via OpAccessChain). Reading a component of a local SSA value needs `OpCompositeExtract`;
   deferred. The Phase-3 fixtures avoid it (they swizzle only globals, e.g. `gl_GlobalInvocationID.x`).
5. **Vector·scalar ops (`vec * float`) are NOT yet supported** — would need `OpVectorTimesScalar` (the
   plain `ExprOp2` path emits `OpFMul`, which requires matching operand types). Deferred to a later slice;
   the fixtures avoid it.

**Phase 3 COMPLETE** — stages + I/O + builtins + interface (commit 1) + constructors/dot/ext-inst
(commit 2) + tests/census/masterplan (commit 3). The cross-repo triangle-on-GPU gate (dasVulkan: port
`triangle.vert`/`.frag` to `[vertex_shader]`/`[fragment_shader]`, delete the committed `.spv`, regress
the triangle PPM on lavapipe + local GPU) is the follow-on dasVulkan PR, mirroring Phase 1's GPU gate.
