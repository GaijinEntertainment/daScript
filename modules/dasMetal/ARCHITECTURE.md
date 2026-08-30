# dasMetal - native Metal compute: platform-gated binding + pure-daslang MSL backend

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklists that bind a diff here are `REVIEW.md` (this folder) and
`modules/REVIEW_SHADER_EMITTERS.md`. The plan sections and the implementation log this
document grew from are archived at `history/dasMetal/MASTERPLAN_LOG.md`.

## 1. Why

dasVulkan owns graphics on every platform (including macOS via MoltenVK). What it cannot
give on Apple Silicon is the **native compute fast path**: no Vulkan-emulation layer under
every dispatch, unified-memory `MTLBuffer`s that are plain pointers on both CPU and GPU,
and Metal-only features (simdgroup reductions, `simdgroup_matrix`). The target profile is
dasLLAMA-class GPU compute offload on the M-boxes. Scope is **compute only** - graphics
stays dasVulkan's job.

The shader-language facts that shape the design: **LLVM cannot emit MSL** (no backend; AIR,
Metal's binary form, is a private version-unstable LLVM-IR dialect) and **Metal does not
ingest SPIR-V** - its sanctioned inputs are MSL source and DXIL. Translation libraries
exist (SPIRV-Cross is the shader translator MoltenVK itself uses), but vendoring one is a
third-party C++ dependency that contradicts this module's premise - Metal.framework ships with
the OS, so there is nothing to vendor.

So dasMetal uses the house pattern: dasGlsl emits **GLSL text** from the daslang AST
(`GlslExport`), dasSpirv emits **SPIR-V words**, and `daslib/aot_cpp.das`
emits **C++ for the entire language**. MSL is a C++14 dialect; a compute-only MSL text
emitter is squarely inside the established pattern. Just as `[compute_shader]` lowers
daslang->SPIR-V->Vulkan, `[metal_kernel]` lowers daslang->MSL->Metal.

What makes it cheap: Metal's in-process runtime compiler (`newLibraryWithSource`) does all
GPU optimization; text is the easiest backend to emit and debug; and the authoring frontend
(`@ssbo`/`@binding` field annotations + builtin globals) already exists in dasSpirv and is
reused as-is.

## 2. Settled decisions

1. **Lives in the main tree as `modules/dasMetal`.** The C++ binding is APPLE-gated; the
   `metal/` daslang files (emitter + annotation) are **pure das and registered on every
   platform** - emitter/text tests run on every CI lane; only GPU execution is Apple-only.
2. **Compute only.** No render pipelines, no drawables, no CAMetalLayer. Graphics = dasVulkan.
3. **Zero third-party code.** No metal-cpp, no SPIRV-Cross, no MoltenVK, no committed
   `.msl`/`.metallib`, no external SDK. The binding is a hand-written Obj-C++ shim
   (`src/dasMetal.mm`) over the system Metal.framework - the compute subset only (sec.3).
   (metal-cpp + dasClangBind rejected: vendored headers and binder churn for a surface this
   small.)
4. **Class-based authoring: kernels are class methods, resources are class members.** A
   compute pass is a `class` whose `@ssbo @binding = N` members declare the buffers and
   whose `[metal_kernel]` methods are the kernels - no module-scope resource globals (they
   pollute the namespace and don't scale to two kernels with different buffer sets in one
   file), and members->kernel-parameters is exactly MSL's own model. Multiple kernels over
   one buffer set = multiple methods in one class. Builtin globals
   (`gl_GlobalInvocationID`, ...) come from the shared `daslib/shader_lingua_franca`, which
   `metal/metal_builtins.das` re-exports. Because the body is ordinary daslang, the same
   method also executes on the CPU - the primary correctness oracle (sec.4).
5. **Naming.** C++ module `das_metal` (`Module("das_metal")`, class `Module_DasMetal`). das
   files under `metal/`: `require metal/msl_shader`, `require metal/das_metal_boost`.
   Consumers guard: `require ?das_metal metal/das_metal_boost` +
   `static_if (typeinfo builtin_module_exists(das_metal))` (the sql_boost provider pattern).
6. **Coverage of the emitter is the construct census (gate B), not LCOV.** LCOV covers runtime
   files only - the emitter runs at compile time, invisible to line coverage.
7. **MSL exists only as text the emitter produces at compile time.**
8. **`fastmath` is a `[metal_kernel]` property, ON by default.** daslang's own posture is
   fastmath-on, so we are not chasing bit-exact float parity; Metal's runtime compiler also
   defaults fast-math ON and we keep it. Float oracles compare with tolerance (ints
   bit-exact). `[metal_kernel(fastmath=false)]` per kernel when isolating a divergence
   needs strict IEEE.
9. **`relaxed_precision = false` keeps a `matmul2d` off the M5 tensor unit's fast path** -
   2-3x across the tmm2d families. The cooperative-tensor register layout the fast path uses
   is what relaxed precision licenses.

## 3. Files and lowering mechanism

`modules/dasMetal` = one APPLE-gated Obj-C++ shim + pure-daslang emitter files:

| File | Gen/Hand | Purpose |
|---|---|---|
| `src/dasMetal.h` | hand | The C++ header `src/dasMetal.mm` and any embedder share: the opaque handle struct declarations and the `DAS_MOD_API` prototype of every extern below. Declarations only - no implementation, no Obj-C, no binding registration. |
| `src/dasMetal.mm` | hand | `Module("das_metal")` - Obj-C++ shim over Metal.framework. Opaque annotated handles (device, queue, command buffer, compute encoder, pipeline state, library, function, buffer) + the extern surface below. Compiled with ARC; handles cross to das as `__bridge_retained void*`; `metal_release` = `__bridge_transfer`. Shim-side live-object counter for the leak gate. APPLE-only; links `-framework Metal -framework Foundation`. |
| `metal/msl_types.das` | hand | daslang `TypeDecl` -> MSL type name: 32-bit scalars/bool plus the 16- and 8-bit scalars (`half`/`short`/`char` in MSL) and their 2/3/4 vectors, classified via the shared `daslib/shader_block_layout` rails. `msl_buffer_elem_name` gives the layout-bearing spelling: 3-lane elements take MSL's `packed_T3` (das packs tightly; unified memory means the das array IS the buffer). |
| `metal/metal_builtins.das` | hand | Metal-only builtin surface over the shared lingua franca (the spirv_builtins pattern): re-exports `daslib/shader_lingua_franca` (whence `gl_WorkGroupSize` and the four `gl_Subgroup*` IDs, shared with dasSpirv) and adds the `simd_sum`/`simd_max`/`simd_shuffle*` intrinsics (Metal spellings, float/int/uint). The CPU bodies return their argument unchanged - on the CPU a simdgroup is one lane. |
| `metal/msl_emit.das` | hand | The text emitter: `generate_msl(fn, var errors, cfg, var census, var tgmem) : string`. Manual recursion (`emit_value`/`emit_stmt`, mirroring `spirv_emit`). Kernel-signature synthesis from `@ssbo` globals (the one structural novelty - below). Records the construct census at every emit site. |
| `metal/msl_shader.das` | hand | `[metal_kernel]` function-macro (`MetalKernel : AstFunctionAnnotation`, modeled on `SpirvShader`), applied to a **class method**; args: `name`, `fastmath` (default **true**). `apply` declares the public MSL-text global - the `name=` argument, or `<Class>_<method>_msl` derived from the method - plus a `<name>_fastmath : bool` companion feeding the pipeline-compile options; **`fixup` fills `glob.init = new ExprConstString(...)`** - the initializer is a constant string, so nothing has to be called to build it and the late `fixup` pass is enough. Does `require msl_emit public` + `require metal_builtins public`. |
| `metal/das_metal_boost.das` | hand | Host sugar over `das_metal`: `with_metal_device`, `pipeline_from_kernel` (compile + error surfacing), unified-memory buffer helpers, `run_compute_1d` one-liner, live-object leak assert. `require das_metal` -> usable only where the C++ module exists. |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS_FROM_DESCRIPTOR(metal metal)` unconditional (rows derived from `.das_module`, emitter everywhere); `IF(APPLE)`: `ADD_MODULE_CPP(DasMetal)` + `ADD_MODULE_LIB` + frameworks. Install rule mirrors dasSpirv's. |

**Extern surface.** `src/dasMetal.mm` binds the compute subset of Metal.framework in eight
families: device+queue, library+pipeline, buffers, encoding+dispatch, errors+timing, events,
residency, lifetime. Two carry non-obvious contracts: `metal_buffer_contents(buf) : void?` is
the unified-memory pointer the host reads and writes directly, with no map/unmap;
`metal_dispatch_threads` is the Apple-silicon exact-grid fast path.

**Kernel-signature lowering (the one structural novelty).** MSL has no module-scope device
globals - and the authoring class's members map onto its model exactly: each `@ssbo`
member lowers to a kernel parameter `device T* name [[buffer(N)]]` (`device const T*` when
no kernel body writes it; member write-set collected in a pre-scan), and each referenced
builtin global lowers to a builtin-attributed parameter (`gl_GlobalInvocationID` ->
`uint3 gl_GlobalInvocationID [[thread_position_in_grid]]`). Member access in the method
body (bare `a` / `self.a`) emits as the bare parameter name, so the body needs no other
rewriting. `@binding` = the flat `[[buffer(N)]]` index; `@set` must be absent or 0 (clean
error - Metal has no descriptor sets); duplicate bindings within one class are a clean
error. Identifiers colliding with MSL keywords (`kernel`, `device`, `constant`, `thread`,
`half`, ...) are mangled. A `@uniform` member lowers to one `constant KArgs&` parameter
(sec.7) and a `@workgroup` member to a threadgroup pointer derived from the entry's `_tgmem`
companion.

## 4. Test architecture

Three behavioral layers + enforcement gates:

1. **Text-assertion units** (`tests/msl/`, repo root, every platform). Each test compiles a tiny
   `[metal_kernel]` fixture, calls `generate_msl`, asserts structural facts (signature
   shape, attribute per binding, const-ness, statement forms), plus a golden snapshot of
   *our own* emitted text as a forward regression guard.
2. **Compile gate** (the spirv-val analog). Every emitted MSL must compile through the
   **real Metal frontend**: in-process `metal_new_library_from_source` where `das_metal`
   exists (no Xcode required - the OS MTLCompilerService), soft-skip elsewhere,
   hard-required on the macOS CI lane. (`xcrun metal` offline compile is a secondary local
   oracle only - it needs full Xcode, not CLT.)
3. **Real-GPU behavioral gate vs the CPU-reference oracle** (`tests/metal/`, repo root,
   Apple-only). The kernel body is ordinary daslang, so the **same method runs on the CPU**:
   a driver loop sets `gl_GlobalInvocationID` and calls the method on a class instance whose
   members are plain arrays - that CPU run (interp/JIT) produces the expected buffer contents
   with zero second-source effort. GPU results compare against it: ints bit-exact, floats with
   tolerance (fastmath on both sides - sec.2 decision 8). Files are `require ?das_metal` +
   `static_if builtin_module_exists` guarded, so they compile and no-op cleanly on non-Apple
   lanes. Primary gate = **local M-boxes**; the macOS CI lane runs the compile gate, because
   its Apple Paravirtual device is unproven for execution.

- **Gate A - LCOV** on runtime-reached files (`das_metal_boost`, `msl_types` where runtime
  code exists). The emitter is compile-time -> census is its coverage proxy.
- **Gate B - construct census.** `generate_msl` records every construct kind it emits
  (`table<string>` set: `"kernel_sig"`, `"param.buffer"`,
  `"param.builtin.thread_position_in_grid"`, `"op.mul.f32"`, ...). A meta-test unions the
  census across all fixtures and asserts equality with the declared supported set, both
  directions. Census-record and emit are fused in one helper so they cannot drift; the
  golden-snapshot layer catches emissions that bypass it.
- **Leak gate.** Metal objects are invisible to the das leak detectors, so the shim counts live
  objects - `metal_live_object_count()`.

## 5. Cross-backend parity - the kernel-model asymmetry ledger

`modules/REVIEW_SHADER_EMITTERS.md` requires a diff that adds a kernel-model capability to one
emitter to add it to the other or record the asymmetry here - one ledger for both backends. The
primary correctness oracle is the CPU-reference run of the same das body - cheaper and stricter
than a second GPU. Cross-GPU parity of one source is secondary. Current entries:

- **`family=` surface sharing is Vulkan-only.** Both emitters accept N kernel methods per class
  with `kernel=` picking per instance (the sibling fixtures are
  `modules/dasLLAMA/tests/_mtl_toy.das` and `modules/dasLLAMA/tests/_vkd_toy.das`); `family=`
  has no Metal counterpart because Metal's `enc_*` builder is the whole generated surface, so a
  family has nothing to share.
- **Method flattening, scope splicing, and plain-member thread-local state are Metal-only.**
  `msl_emit` splices method bodies inline (statically devirtualized, value and statement
  position) and lowers plain class members to kernel-entry thread-locals (scalar-with-init,
  fixed array, carried pointer walk - the state contract is per-thread); `spirv_emit` rejects
  an unannotated member and lowers method calls as calls. The asymmetry is deliberate.
- **The tmm2d tensor-GEMM builtin family is Metal-only; the asymmetry is pending, not
  deliberate.** The family - whole-GEMM helpers plus the staged threadgroup protocol - lowers
  MPP `matmul2d` cooperative tensors, and the CPU-replay bodies are its only cross-checked
  twin.
- **Grid-literal validation is asymmetric; pending, not deliberate.** Vulkan rejects a
  non-int64 ceil-div grid parameter at macro apply; the Metal emitter accepts it and infers a
  type silently.
- **cm2 decode-in-load is Vulkan-only by hardware.** `[spirv_decode]` lowers
  SPV_NV_cooperative_matrix2 tensor loads. Metal-4 tensors have no decode-callback analogue, so
  the Metal quant GEMMs stage dequant through threadgroup memory instead. Deliberate,
  target-specific - not a pending port.

## 7. `@uniform` structs - the kargs form

A kernel with a dozen scalars would otherwise cost a dozen binds.
`@uniform @binding = N ka : KArgs`, where `KArgs` is a plain das struct, costs one: the struct
definition emits into the MSL preamble, the member becomes `constant KArgs& ka [[buffer(N)]]`,
and `ka.field` is one hop.

**Fields are `int`/`uint`/`float` only.** That restriction is the whole feature: a struct of
4-byte scalars is the same bytes in das and in MSL, with no padding rules to keep in sync, so
the host writes its own struct straight through `setBytes` (`metal_set_bytes`, already bound -
Metal caps it at 4KB and a kargs struct is well under). A `float4` field would be 16-byte
aligned in MSL and shift every field after it; a nested struct brings its own alignment. Both
are refused (`tests/msl/_fail_closed/_fc_ustruct_{field,nested}.das`, repo root).

Host side: `run_compute_1d_kargs` for tests, and dasLLAMA's `kn_bytes` / `kn_kargs`, which copy
the bytes into a pool under graph capture - the caller's kargs is a stack local, long gone by
replay time.

**It costs the GPU nothing.** Every field was already a runtime `constant uint&`; the fold trades
N bindings for one argument-buffer read and the kernel branches on exactly the values it did
before. Metal inlining a passed shape value back into a constant is an assumption, not a
guarantee.

Tests: `tests/msl/test_msl_uniform_struct.das` (repo root - definition placement, the single
`constant&` parameter, no per-scalar parameter survives) and
`tests/metal/test_metal_uniform_struct.das` (repo root - GPU vs CPU-oracle, the only thing that
can prove the layout claim; mutation-verified by inserting one pad field into the emitted
struct).

## 8. Pointer parameters

A helper takes a raw pointer: `def stage(var p : half4 const?; n : uint)`. This is what lets the
kernels that stream `unsafe(addr(buf[i]))` through a loop share one skeleton: the advancing
pointer is the shape, and rewriting it as index math changes AGX register allocation.

**MSL requires an address space in the signature** (`device half4*` vs `threadgroup half4*`) and
the das type `half4?` carries none - the same type can name an @ssbo interior or a @workgroup
interior. The parameter declares it and the call site proves it:

- **Unmarked is `device`.** Every pointer stream in the zoo is device, so the common case is
  plain das with no marker to learn.
- **`@threadgroup p : T?`** opts into threadgroup memory. Parameter annotations already parse
  and keep their values, so this needs no grammar.
- **The emitter derives the space from the argument's provenance and cross-checks it** - an
  inline `addr(member[i])`, a pointer local, a pointer parameter being forwarded, or `p + n`
  over any of those. Disagreement is a das error naming both sides; provenance it cannot trace
  (a thread-space local) is refused rather than guessed. The declared space is never trusted on
  its own, so a wrong marker cannot reach the runtime MSL compile as a null pipeline.
- **The pointee's const is the MSL const.** `T const?` lowers `device const T*` and reads only;
  `T?` lowers `device T*` and marks the source member written, exactly as a `var array<T>`
  parameter does. (Writing through it in das additionally needs the handle non-const - `var o :
  T?` - because das flows the handle's const onto the dereference.)

Tests (all repo root): `tests/msl/_msl_common.das` + `tests/msl/test_msl_functions.das`
(signatures, forwarding, inline `addr` arguments, census), `tests/metal/test_metal_functions.das`
(GPU advancing dot + threadgroup slab max vs a directly-computed expectation),
`tests/msl/_fail_closed/_fc_ptr_space_{tg,dev}.das` and
`tests/msl/_fail_closed/_fc_ptr_untraceable.das` (both mismatch directions + the untraceable
case).

## 9. tmm2d staged-W threadgroup tiles {#tmm2d-staging}

The q8u/q8uh GEMM helpers dequantize the interleaved-q8_0 W tile into threadgroup memory and
stream activations from device memory. The staged tile's threadgroup layout is a contract with
two parties, and `tmm2d_q8u_f32` is neither of them - its das body never touches `wt`, so it
replays the GEMM and says nothing about staging. The first party is `tmm2d_tg_step_deva`, whose
das body carries an `ldb` that must equal the emitted step's B extent. The second is every
caller: the `@workgroup` array handed in as `wt` is sized by the caller against a stride only
this document and the emitter know.

**Staged rows are padded, not packed.** A 64-deep row of halves is 128 bytes, which is the
threadgroup bank-conflict worst case: every row then starts in the same bank. Rows pad to 72
halves, and a 32-deep tail row pads to 40, so consecutive rows walk the banks instead. The pad
is the row stride the `matmul2d` tensor extent carries, so the emitter passes the stride
explicitly rather than letting the extent default to the packed width.

**`bk` names the resident staging footprint, not the chunk depth.** `bk = 32` stages one
32-deep chunk between barriers. `bk = 64` stages one 64-deep chunk and halves the barriers per
K walk (raced 5-11% per GEMM on M5). `bk = 128` is 64-deep double-buffered: two ping-pong
tiles in an `n*144`-half `wt`, costing ONE barrier per chunk. That single barrier does both
jobs - it orders the previous `op.run` against the overwrite of the tile that run consumed,
and it publishes the tile staged during the previous iteration. Staging chunk `b+1` therefore
overlaps `op.run` on chunk `b`, because the two touch disjoint halves of `wt`. A
`kk % 64 == 32` remainder runs one 32-deep tail chunk in every mode.

**`bk = 128`'s initial preload is guarded on `nb > 0`.** A `kk` under 64 has no 64-deep chunk to
preload, and running the preload anyway reads W blocks past the panel's `ldwb` stride and writes
`wt` bytes the tail chunk then writes again with no barrier between the two.
