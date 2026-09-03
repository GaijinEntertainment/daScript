# dasSpirv - pure-daslang daslang->SPIR-V shader backend

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklists that bind a diff here are `REVIEW.md` (this folder) and
`modules/REVIEW_SHADER_EMITTERS.md`. The plan sections and the implementation log this
document grew from are archived at `history/dasSpirv/MASTERPLAN_LOG.md`.

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
3. The opcode census declares the supported opcode set and is checked against the fixtures in
   both directions; LCOV covers the runtime-reached files beside it (sec.4).
4. **SPIR-V 1.3 is the default header version; a feature that needs more raises it.** 1.3
   gives StorageBuffer storage class + `Block` (not the deprecated 1.0 BufferBlock+Uniform
   path), lavapipe advertises >= 1.2, and at `<= 1.3` the entry-point interface lists only
   Input/Output globals (1.4 requires *all* globals, which would churn every stage's
   interface). The version is a per-module field, `SpirvModule.version`: a mesh or task stage
   raises it to 1.4 because `SPV_EXT_mesh_shader` requires it, and a few subgroup and
   cooperative-matrix ops raise it to 1.5. Every other stage stays at 1.3.

## 3. Files and emission mechanism {#files-and-emission}

`modules/dasSpirv` is **pure daslang** (mirrors dasGlsl: a `spirv/` subdir of `.das` files +
CMake resolver rows derived from `.das_module`; no `.shared_module`, no C++). dasVulkan
consumes it via `require spirv/...` and feeds the emitted `array<uint>` (SPIR-V words) to
`create_shader_module`.

| File | Gen/Hand | Purpose |
|---|---|---|
| `spirv/spirv_grammar.das` | **generated** | Opcode + GLSL.std.450 enums + StorageClass/Decoration/BuiltIn/ExecutionMode/Capability token constants, generated from the vendored, pinned SPIRV-Headers grammar JSON. Single source of numeric truth (+ opcode->name table for the disassembler). |
| `spirv/spirv_builder.das` | hand | `SpirvModule` (per-section append buffers), monotonic id allocator, type/constant dedup pools, `emit(section, opcode, ...)`, `get_type_id`, `get_const_id`, `finalize() : array<uint>`. AST-agnostic. |
| `spirv/spirv_types.das` | hand | daslang `TypeDecl` -> SPIR-V type-id (scalars, vec/mat, fixed + runtime arrays, structs, pointers) with layout decorations. Highest-detail correctness file. |
| `spirv/spirv_builtins.das` | hand | The builtin surface the emitter recognizes by name: builtin globals (`gl_*`), the opaque resource marker structs (`sampler2D`/`image2D`/`sampler2DShadow`/...), and the intrinsic stubs (`texture`, `imageLoad`, `barrier`, the atomics, the derivatives, ...). Every stub is `[sideeffects]` so no const-fold or DCE pass can reach it before the annotation runs. |
| `spirv/spirv_emit.das` | hand | `SpirvEmit : AstVisitor` codegen visitor + the `[macro_function] generate_spirv(...)` entry point. Every construct the emitter cannot lower has a rejection override or hits the `value_of`/`ptr_of` backstop, so it becomes a clean compile error rather than a bad blob. |
| `spirv/spirv_reflect.das` | hand | `shared public` reflection vocabulary - `SpirvReflection`, `DescriptorBinding`, `PushConstantRange`, `SpirvDescriptorKind`, `SpirvStageFlags` - plus the versioned `array<uint>` encode/decode the host reads. Names no graphics API, so dasSpirv never depends on dasVulkan. |
| `spirv/spirv_shader.das` | hand | The shader annotations, each a `SpirvShader : AstFunctionAnnotation` subclass carrying its stage: `[compute_shader]`, `[spirv_kernel]` (class-method authoring), `[vertex_shader]`, `[fragment_shader]`, `[mesh_shader]`, `[task_shader]`, `[raygen_shader]`, `[miss_shader]`, `[closest_hit_shader]`; plus the `[spirv_decode]` / `[spirv_combine]` / `[spirv_per_element]` callback annotations. `apply` reserves the blob global and its `_reflect` companion; `fixup` runs dependency collection, calls `generate_spirv`, and sets both inits. |
| `spirv/spirv_dis.das` | hand | Minimal disassembler + opcode-census helper (self-delimiting walk: word0 = `(wordCount<<16)\|opcode`). Symbolic via `spirv_grammar`'s opcode->name table. |
| `generator/gen_spirv_grammar.das` | hand | The mini-generator: reads vendored grammar JSON -> emits `spirv/spirv_grammar.das`. |
| `spirv_headers/*.json` | vendored | Pinned `spirv.core.grammar.json` + `extinst.glsl.std.450.grammar.json`; license in `SPIRV_HEADERS.LICENSE`, provenance in `history/dasSpirv/MASTERPLAN_LOG.md`. |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS_FROM_DESCRIPTOR(spirv spirv)` + install rule, modeled on `modules/dasGlsl/CMakeLists.txt`. |

**SSA backend (llvm_jit template).** `SpirvEmit` carries `e2v : table<Expression?;uint>`
(Expression->result-id), `v2v : table<Variable?;uint>` (Variable->pointer-id),
`ite2blocks`/`loop_stack` for control flow, and a hand-maintained `cur_block_terminated`
(SPIR-V has no builder to query - set on every terminator, cleared on `OpLabel`; guards
mirror llvm_jit's `current_block_terminates()`). Section buffers concatenate at `finalize`
in SPIR-V's mandated order: header(5) -> capabilities -> extensions -> ext-imports ->
memory-model -> entry-points -> exec-modes -> debug -> decorations -> types+constants+global-vars
(interleaved, define-before-use, deduplicated) -> functions.

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
twin of that callback is section 3.3.

**Cooperative-matrix element loops carry `Unroll`.** `coopmatClamp` walks a coopmat local
element by element through a hand-emitted structured loop bounded by
`OpCooperativeMatrixLengthKHR`, and its `OpLoopMerge` sets loop control `Unroll` - the control
glslang emits for `[[unroll]]`. Rolled, the dynamic per-element `OpAccessChain` index demotes
the accumulator out of tensor-register form into addressable storage for the whole kernel, not
only for the loop. On an RTX 5060 Ti (driver 610.74) the cm2 l-tile min-kernel runs 34.2 TFLOP/s
rolled and 57.8 unrolled.

### 3.1 The 8/16-bit small-integer surface {#small-int-surface}

A shader reads AND writes `int8`/`uint8`/`int16`/`uint16`/`float16` SSBO elements and struct
members, and the write direction costs the emitter no arm of its own. A narrowing daslang cast
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

### 3.2 A cm2 tile shape is one struct declaration {#cm2-tile-markers}

The workgroup-scope cooperative-matrix tiles are marker structs in `spirv_builtins.das` whose
NAMES carry their geometry - `coopmatWg{A|B|Acc}_{f16|f32|s8|s32}_{R}x{C}` - and
`coopmat_wg_info` parses that name instead of looking the struct up in a table. Adding a tile
shape is therefore one struct declaration plus the overload that types the das call
(`coopmatMulAdd` for a multiply tile, `coopmatConvert` for an accumulator-only tile): no
emitter arm changes, because every cm2 arm reads rows, columns and component width out of the
parse. The tile markers are empty structs with no storage, so every builtin over them - the
tensor loads and stores, the decode forms, `coopmatMulAdd`, `coopmatConvert`, `coopmatClamp`,
the reductions - has an inert CPU body that cannot compute what the emitted form computes: a
coopmat kernel's device test takes a plain CPU reference as its oracle, the one sanctioned
exception class `modules/REVIEW_SHADER_EMITTERS.md` admits. A reduction width
known only at run time reaches a SPIR-V kernel through a `tensorLayout2D` or
`tensorLayout2DPad` whose dimension `tensorLayoutSetDimension` sets - that layout is this
emitter's runtime-extent descriptor.

### 3.3 The four-wide decode twin {#cm2-decode-vector}

`SPV_NV_cooperative_matrix_decode_vector` adds no opcode: one capability and the
`DecodeVectorFunc` bit of the tensor-load's addressing mask, whose operand names a second decode
function returning the 4-lane vector of the tile component type, called for four consecutive
elements along the block's last dimension from a multiple of four. The scalar `DecodeFunc` stays
mandatory beside it and the driver picks per call site, so one module serves devices with and
without the feature. `coopmatLoadTensorDecode`'s tenth argument selects the twin: `true` has the
emitter synthesize it as four `OpFunctionCall`s of the scalar body at `coordInBlock.y + 0..3`
composed into the vector (the driver's compiler inlines the calls and merges the loads they
share), `false` is the scalar-only load, and a `[spirv_decode]` function returning `half4`
over a `float16` decode, with the scalar contract's parameters, is a hand-laid twin (daslang
spells no int8 or 32-bit four-vector the tiles would take, so the hand-laid form is f16-only;
the synthesized twin covers every scalar type a decode may return). Twins register in their own
table keyed by the function they came from, so one scalar body yields one twin however many loads
name it, and emit after the scalar bodies they call.

The extension stands apart from the cm2 base: a module that uses the twin declares the
`CooperativeMatrixDecodeVectorNV` capability and the `SPV_NV_cooperative_matrix_decode_vector`
extension name, and the device must have its own
`VkPhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` bit enabled.

The tensor-addressing operands follow the mask word in bit order - `TensorView`, then
`DecodeFunc`, then `DecodeVectorFunc` - after the load's fixed prefix: result type, result,
pointer, object, layout, memory-access mask and that mask's own extra words. The emitter appends
them in that order and the strip counts forward to the operand it removes by the same rule.

A finished module is downgraded rather than recompiled. `strip_decode_vector` walks the word
stream and removes the capability, the extension declaration, and each
`OpCooperativeMatrixLoadTensorNV`'s `DecodeVectorFunc` bit together with its operand word; the
twin's `OpFunction` stays in the module, unreferenced, and the mandatory scalar `DecodeFunc`
serves the load. One emitted blob therefore runs on a device without the feature.

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

`modules/REVIEW_SHADER_EMITTERS.md` requires a kernel-model capability added to one emitter to
be added to the other or recorded as an asymmetry. That ledger is shared - one list for both
backends, not one per backend - and lives in `modules/dasMetal/ARCHITECTURE.md` sec.5.

## 6. Verification

- **Standing per-change gate (main tree):**
  `daslang dastest/dastest.das -- --test tests/spirv --cov-path spirv.lcov --isolated-mode`
  -> all green, opcode census == declared set both directions, every blob spirv-val-clean, no
  `GC APP LEAK`.
- **Disassembly check:** an emitted module is dumped via `spirv_dis` (symbolic) and diffed
  against external `spirv-dis` as ground truth.
- **Real-driver gate (dasVulkan):** the integration suite under lavapipe and the local real
  GPU.
