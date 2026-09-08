# dasSpirv - pure-daslang daslang->SPIR-V shader backend

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklists that bind a diff here are `REVIEW.md` (this folder) and
`modules/REVIEW_SHADER_EMITTERS.md`. Sections 3.2, 3.3, 3.5, 3.6 and 3.7 - cooperative matrix and
emission-time unrolling - are in `ARCHITECTURE_COOPMAT.md` beside this file.

## 1. Why

The shape dasSpirv exists to replace: shaders authored as hand-written GLSL
`.vert`/`.frag`/`.comp`, compiled offline with glslangValidator into `.spv` blobs, *both*
committed, and loaded at runtime - two languages, an external SDK, committed binaries, no
code-sharing with the host. dasVulkan carries none of that: no GLSL, no committed `.spv`, and
no glslang or SDK dependency.

dasGlsl does this for OpenGL: shaders written in daslang, annotated
`[..._program]`, an `AstVisitor` (`GlslExport`) emits GLSL at compile time into a global
captured by `@@fn`, fed to `glShaderSource`. dasSpirv does the same for Vulkan, but emits
**SPIR-V binary directly from the daslang AST** - no GLSL/glslang intermediary, no LLVM.

SPIR-V is an SSA IR (typed results, basic blocks, structured control flow, a logical
builder) - the same shape `llvm_jit` lowers daslang to. So dasSpirv is
**dasGlsl's frontend pattern + llvm_jit's SSA-backend pattern, fused into a SPIR-V emitter**.
Just as `[jit]` lowers daslang->LLVM->native, `[compute_shader]` lowers daslang->SPIR-V->GPU.

What makes it cheap: drivers do all GPU optimization (we emit naive *valid* SPIR-V - so does
glslang); SPIR-V binary is a self-delimiting word stream (trivial to emit and disassemble);
the backend-agnostic AST tooling (`collect_dependencies`, `collect_used_types`,
`make_visitor`) is reusable verbatim; and dasVulkan has a GPU-verified `out[i]==i*i`
compute test as a ready-made end-to-end gate.

## 2. Settled decisions

1. **Lives in the main tree as `modules/dasSpirv`** - sibling to dasGlsl/dasLLVM, in
   PR-protected GaijinEntertainment/daScript, sharing main-tree CI + `daslib/coverage` +
   `tests/`. SPIR-V is a general daslang capability (also useful for GL4.6/WebGPU), not
   Vulkan-specific.
2. **Fresh frontend** - its own shader annotations (sec.3). dasSpirv has no code dependency on
   dasGlsl or dasOpenGL; it reuses only the generic AST tooling, none of which lives in
   dasGlsl.
4. **SPIR-V 1.3 is the default header version; a feature that needs more raises it.** 1.3
   gives StorageBuffer storage class + `Block` (not the deprecated 1.0 BufferBlock+Uniform
   path), lavapipe advertises >= 1.2, and at `<= 1.3` the entry-point interface lists only
   Input/Output globals (1.4 requires *all* globals, which would churn every stage's
   interface). The version is a per-module field, `SpirvModule.version`: a mesh or task stage
   raises it to 1.4 because `SPV_EXT_mesh_shader` requires it, a call that re-types a
   Block-laid-out struct with `OpCopyLogical` (`ARCHITECTURE_COOPMAT.md` sec.3.5) raises it to
   1.4 because that opcode requires it, a few subgroup ops raise it to 1.5, and the
   cooperative-matrix ops raise it to 1.6. Every other stage stays at 1.3.

## 3. Files and emission mechanism {#files-and-emission}

`modules/dasSpirv` is **pure daslang** (mirrors dasGlsl: a `spirv/` subdir of `.das` files +
CMake resolver rows derived from `.das_module`; no `.shared_module`, no C++). dasVulkan
consumes it via `require spirv/...` and feeds the emitted `array<uint>` (SPIR-V words) to
`create_shader_module`.

| File | Gen/Hand | Purpose |
|---|---|---|
| `spirv/spirv_grammar.das` | **generated** | Opcode + GLSL.std.450 enums + StorageClass/Decoration/BuiltIn/ExecutionMode/Capability token constants, generated from the vendored, pinned SPIRV-Headers grammar JSON. Single source of numeric truth (+ opcode->name table for the disassembler). |
| `spirv/spirv_builder.das` | hand | `SpirvModule` (per-section append buffers), monotonic id allocator, type/constant dedup pools (`type_pool` / `const_pool`, behind the `type_*` and `const_*` helpers), `emit(section, opcode, ...)`, and `module_words() : array<uint>`, which prepends the five-word header and concatenates the sections. `finalize` frees the buffers and the pools. AST-agnostic. |
| `spirv/spirv_types.das` | hand | daslang `TypeDecl` -> SPIR-V type-id (scalars, vec/mat, fixed + runtime arrays, structs, pointers) with layout decorations. Highest-detail correctness file. |
| `spirv/spirv_builtins.das` | hand | The builtin surface the emitter recognizes by name: builtin globals (`gl_*`), the opaque resource marker structs (`sampler2D`/`image2D`/`sampler2DShadow`/...), and the intrinsic stubs (`texture`, `imageLoad`, `barrier`, the atomics, the derivatives, ...). Every stub is `[sideeffects]` so no const-fold or DCE pass can reach it before the annotation runs. |
| `spirv/spirv_emit.das` | hand | `SpirvEmit : AstVisitor` codegen visitor + the `[macro_function] generate_spirv(...)` entry point. Every construct the emitter cannot lower has a rejection override or hits the `value_of`/`ptr_of` backstop, so it becomes a clean compile error rather than a bad blob. |
| `spirv/spirv_reflect.das` | hand | `shared public` reflection vocabulary - `SpirvReflection`, `DescriptorBinding`, `PushConstantRange`, `SpirvDescriptorKind`, `SpirvStageFlags` - plus the versioned `array<uint>` encode/decode the host reads. Names no graphics API, so dasSpirv never depends on dasVulkan. |
| `spirv/spirv_shader.das` | hand | The shader annotations, each a `SpirvShader : AstFunctionAnnotation` subclass carrying its stage: `[compute_shader]`, `[spirv_kernel]` (class-method authoring), `[vertex_shader]`, `[fragment_shader]`, `[mesh_shader]`, `[task_shader]`, `[raygen_shader]`, `[miss_shader]`, `[closest_hit_shader]`; plus the `[spirv_decode]` / `[spirv_combine]` / `[spirv_per_element]` callback annotations. `apply` reserves the blob global and its `_reflect` companion; `fixup` runs dependency collection, calls `generate_spirv`, and sets both inits. |
| `spirv/spirv_dis.das` | hand | Minimal disassembler + opcode-census helper (self-delimiting walk: word0 = `(wordCount<<16)\|opcode`). Symbolic via `spirv_grammar`'s opcode->name table. |
| `generator/gen_spirv_grammar.das` | hand | The mini-generator: reads vendored grammar JSON -> emits `spirv/spirv_grammar.das`. |
| `spirv_headers/*.json` | vendored | The pinned Khronos SPIRV-Headers grammars: `spirv.core.grammar.json` at SPIR-V 1.6 revision 7, and `extinst.glsl.std.450.grammar.json` at version 100 revision 2. License in `SPIRV_HEADERS.LICENSE`. |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS_FROM_DESCRIPTOR(spirv spirv)` + install rule, modeled on `modules/dasGlsl/CMakeLists.txt`. |

**SSA backend (llvm_jit template).** `SpirvEmit` carries `e2id` (intptr(Expression) -> rvalue
result-id), `e2ptr`/`e2pty` (intptr(Expression) -> lvalue pointer-id + pointee type-id),
`ctx.local_vars` (intptr(Variable) -> the Function-storage `OpVariable` backing a local),
`ite_ids`/`loop_ids` and `ctx.loop_stack` for control flow, and a hand-maintained
`ctx.terminated` (SPIR-V has no builder to query - set on every terminator, cleared on
`OpLabel`; guards mirror llvm_jit's `current_block_terminates()`). `module_words`
concatenates the section buffers behind the five-word header, in SPIR-V's mandated order:
capabilities -> extensions -> ext-imports -> memory-model -> entry-points -> exec-modes ->
debug -> decorations -> types+constants+global-vars (interleaved, define-before-use,
deduplicated) -> functions.

**Capture mechanism (dasGlsl analog).** dasGlsl's `fixup` sets
`glob.init = new ExprConstString(value := text)`; ours builds an `ExprMakeArray` of
`ExprConstUInt` (one per SPIR-V word) for an `array<uint>` global named by the annotation's
`name=` argument, or `` "{func.name}`spirv" `` when it has none, beside a `{name}_reflect`
companion holding the encoded reflection. `generate_spirv` is a standalone `[macro_function]`
called by **both** `fixup` and the unit tests - so opcode assertions hit the real codegen path
without macro plumbing.

**`[spirv_decode]` method form.** The decode callback's SPIR-V signature is a rigid three
parameters. The method form erases the das-level `self` from it, so the decode body still reads
its class members - a separate scale plane, push constants, `@workgroup` staging. The four-wide
twin of that callback is `ARCHITECTURE_COOPMAT.md` section 3.3; a kernel body calling the method
directly is that document's section 3.5.

### 3.1 The 8/16-bit small-integer surface {#small-int-surface}

A shader reads AND writes `int8`/`uint8`/`int16`/`uint16`/`float16` SSBO elements and struct
members, and the write direction costs the emitter no *arm* of its own. An **arm** is one branch
of the emitter that handles one construct. A narrowing daslang cast
(`int8(v)`, `uint16(u)`) is one of the conversion opcodes `convert_op` already picks for any
narrowing pair, and a store reaches its element through the same width-aware std430 access chain
a load reads - so `ensure_member_storage_caps`, pulling `StorageBuffer8BitAccess` /
`StorageBuffer16BitAccess` per member width, serves both directions from one call site. That is
what lets a shader write quantized data - int8 quants beside an f16 scale - instead of packing
32-bit words by hand.

`unpack8` extends the same way. The `daslib/shader_lingua_franca.das` overloads add
`int16 -> byte2` and `uint16 -> ubyte2` beside the 32-bit pair, and every one of them lowers
through the single `OpBitcast` the emitter already emits for the name, so a 16-bit quant read
needs no emitter change at all. The `byte4`/`ubyte4` type factory pulls the `Int8` capability,
and widening an unpacked lane (`int4(b4)`) is a same-class `OpSConvert`, which gives sign
extension for free.

### 3.4 Operand laziness follows the language {#operand-laziness}

`cond ? a : b`, `&&` and `||` lower to `OpSelect` / `OpLogicalAnd` / `OpLogicalOr` while both
operands are pure - branchless, both evaluated. An operand that indexes a global-rooted array (an
ssbo, a block field, a `@workgroup` array) lowers as a branch instead: `SpirvTempAlloc` hoists a
Function-storage temp per such node (`ctx.lazy_temps`), the operand the condition admits stores
through it, and the merge block reloads it as the value. For `&&` and `||` the left operand's
answer stores before the branch, so the edge that skips the right operand runs straight to the
merge. The branch labels are allocated after the condition is visited, in the visitor's if/else
hook order (the left- and right-operand pre-visit hooks).

`OpSelect` evaluates both operands. The operand a condition rules out is the one whose index that
condition guards. On the device an out-of-range load faults only when the overshoot leaves mapped
memory, so whether it faults follows the allocation layout, not the kernel's inputs. A local fixed
array stays eager: its index is register arithmetic, not a device address.

## 4. Test architecture - "every emitted instruction has a test"

The behavioral layers, then the enforcement gates (all in main-tree `tests/spirv/` except the
real-driver layer, which lives in dasVulkan):

1. **Opcode-assertion units.** Each test compiles a tiny shader fixture, calls
   `generate_spirv`, runs `spirv_dis` to a structured instruction list, and asserts the
   expected numeric opcode present/absent/operand-at-offset/decoration present. One test per
   emittable instruction kind. A `_golden/` disassembly snapshot of our own validated output
   is the forward regression guard - byte-identical where id allocation is stable,
   id-isomorphic (`check_iso`) where it is not. A new capability's fixture is a kernel in
   `_spirv_common.das` with a row in `_gen_golden.das` and its `_golden/<name>.txt`, or a
   test file of its own asserting on the emitted words; an extension that adds an operand
   bit rather than an opcode is invisible to the census and takes the words form.
2. **`spirv-val` witness.** Every blob the suite produces is validated with the SPIRV-Tools
   `spirv-val` resolved through `VULKAN_SDK` where one is present and knows the blob's
   extensions - a local witness for structured-CFG and define-before-use bugs. It skips where
   the tool is absent or predates an extension, and CI carries none, so the words assertions
   are the gate and a spirv-val-only claim checks nothing there.
3. **Real-driver behavioral regression.** A one-call framework in dasVulkan,
   `run_compute_spirv(words, n) : array<uint>` over `compute_boost`, runs any emitted blob;
   `compute_image_rgba8` + `assert_pixels_exact` are its image-readback twin. The
   content is procedural, so the expected pixel is CPU-computable and the gate asserts exact
   pixels rather than inequalities. Primary gate = local real GPU; CI = lavapipe software
   (GitHub-hosted runners have no GPU, so there is no real-GPU CI lane).

- **Fail-closed fixtures.** One fixture per rejection path lives in `tests/spirv/_fail_closed/`
  and its error text is asserted in `tests/spirv/test_fail_closed.das`. The subdir is not
  globbed, so the AOT build never tries to compile a fixture whose whole point is to fail.
- **Gate A - LCOV coverage.** `dastest --cov-path spirv.lcov` (`daslib/coverage.das`) measures
  the runtime-reached files (`spirv_builder`, `spirv_types`, `spirv_dis`). It cannot measure
  the emitter: `generate_spirv` runs at COMPILE time and LCOV counts runtime line hits, so
  `spirv_emit.das` reads 0/N by construction. Shader fixtures carry `[marker(no_coverage)]` -
  coverage instrumentation injects calls the emitter cannot lower.
- **Gate B - opcode census (the emitter's coverage proxy).** `spirv_dis` aggregates the opcode
  set across *all* fixtures; a meta-test asserts it equals the declared "supported opcodes"
  set, both directions. This catches what LCOV cannot - an opcode whose emit line ran but was
  never *asserted*.

## 5. Cross-backend parity - the kernel-model asymmetry ledger

`modules/REVIEW_SHADER_EMITTERS.md` requires one kernel-model asymmetry ledger for both emitters.
That list lives in `modules/dasMetal/ARCHITECTURE.md` sec.5.
