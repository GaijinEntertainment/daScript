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
exist (SPIRV-Cross is MoltenVK's own shader stage), but vendoring one is a third-party C++
dependency that contradicts this module's thesis: *Metal.framework is just there*.

So dasMetal uses the house pattern, already proven three times: dasGlsl emits **GLSL text**
from the daslang AST (`GlslExport`), dasSpirv emits **SPIR-V words**, and `daslib/aot_cpp.das`
emits **C++ for the entire language**. MSL is a C++14 dialect; a compute-only MSL text
emitter is squarely inside the established pattern. Just as `[compute_shader]` lowers
daslang->SPIR-V->Vulkan, `[metal_kernel]` lowers daslang->MSL->Metal.

What makes it cheap: Metal's in-process runtime compiler (`newLibraryWithSource`) does all
GPU optimization; text is the easiest backend to emit and debug; and the authoring frontend
(`@ssbo`/`@binding` field annotations + builtin globals) already exists in dasSpirv and is
reused as-is.

## 2. Settled decisions

1. **Lives in the main tree as `modules/dasMetal`.** The C++ binding is APPLE-gated; the
   `metal/` daslang files (emitter + annotation) are **pure das and registered on ALL
   platforms** - emitter/text tests run on every CI lane; only GPU execution is Apple-only.
2. **Compute only.** No render pipelines, no drawables, no CAMetalLayer. Graphics = dasVulkan.
3. **Zero third-party code.** No metal-cpp, no SPIRV-Cross, no MoltenVK, no committed
   `.msl`/`.metallib`, no external SDK. The binding is a hand-written Obj-C++ shim
   (`src/dasMetal.mm`) over the system Metal.framework - the compute subset only (sec.3).
   (metal-cpp + dasClangBind rejected: vendored headers and binder churn for a surface this
   small; revisit only if it outgrows hand maintenance.)
4. **Class-based authoring: kernels are class methods, resources are class members.** A
   compute pass is a `class` whose `@ssbo @binding = N` members declare the buffers and
   whose `[metal_kernel]` methods are the kernels - no module-scope resource globals (they
   pollute the namespace and don't scale to two kernels with different buffer sets in one
   file), and members->kernel-parameters is exactly MSL's own model. Multiple kernels over
   one buffer set = multiple methods in one class. Builtin globals
   (`gl_GlobalInvocationID`, ...) come from the shared `daslib/shader_lingua_franca`, which
   `metal/metal_builtins.das` re-exports. Because the body is ordinary daslang, the same
   method also executes on the CPU - the primary correctness oracle (sec.4). **Zero edits to
   shipped dasSpirv/dasGlsl.**
5. **Naming.** C++ module `das_metal` (`Module("das_metal")`, class `Module_DasMetal`). das
   files under `metal/`: `require metal/msl_shader`, `require metal/das_metal_boost`.
   Consumers guard: `require ?das_metal metal/das_metal_boost` +
   `static_if (typeinfo builtin_module_exists(das_metal))` (the sql_boost provider pattern).
6. **Test-per-construct is a hard requirement**, enforced by a construct census (gate B) +
   the real-frontend compile gate. LCOV covers runtime files only - the emitter runs at
   compile time, invisible to line coverage (dasSpirv finding; census is the proxy).
7. **No `.msl` file is committed and no MSL is hand-written.** MSL exists only as text the
   emitter produces at compile time.
8. **`fastmath` is a `[metal_kernel]` property, ON by default.** daslang's own posture is
   fastmath-on, so we are not chasing bit-exact float parity; Metal's runtime compiler also
   defaults fast-math ON and we keep it. Float oracles compare with tolerance (ints
   bit-exact). `[metal_kernel(fastmath=false)]` per kernel when isolating a divergence
   needs strict IEEE.
9. **Every `matmul2d_descriptor` sets `relaxed_precision = true`.** RP = false keeps the
   op off the M5 tensor unit's fast path - measured 2-3x across the tmm2d families; the
   cooperative-tensor register layout the fast path uses is what RP licenses. `REVIEW.das`
   enforces the emitter's descriptor sites.

## 3. Files and lowering mechanism

`modules/dasMetal` = one APPLE-gated Obj-C++ shim + pure-daslang emitter files:

| File | Gen/Hand | Purpose |
|---|---|---|
| `src/dasMetal.mm` | hand | `Module("das_metal")` - Obj-C++ shim over Metal.framework. Opaque annotated handles (device, queue, command buffer, compute encoder, pipeline state, library, function, buffer) + the extern surface below. Compiled with ARC; handles cross to das as `__bridge_retained void*`; `metal_release` = `__bridge_transfer`. Shim-side live-object counter for the leak gate. APPLE-only; links `-framework Metal -framework Foundation`. |
| `metal/msl_types.das` | hand | daslang `TypeDecl` -> MSL type name (32-bit scalars/bool + the 16/8-bit lattice - MSL has native `half`/`short`/`char` - and their 2/3/4 vectors, classified via the shared `daslib/shader_block_layout` rails). `msl_buffer_elem_name` gives the layout-bearing spelling: 3-lane elements take MSL's `packed_T3` (das packs tightly; unified memory means the das array IS the buffer). |
| `metal/metal_builtins.das` | hand | Metal-only builtin surface over the shared lingua franca (the spirv_builtins pattern): re-exports `daslib/shader_lingua_franca` (whence `gl_WorkGroupSize` and the four `gl_Subgroup*` IDs, shared with dasSpirv) and adds the `simd_sum`/`simd_shuffle*` intrinsics (Metal spellings, float/int/uint). Identity stub bodies = width-1 simdgroup CPU semantics. |
| `metal/msl_emit.das` | hand | The text emitter: `generate_msl(fn, var errors, cfg, var census, var tgmem) : string`. Manual recursion (`emit_value`/`emit_stmt`, mirroring `spirv_emit`). Kernel-signature synthesis from `@ssbo` globals (the one structural novelty - below). Records the construct census at every emit site. |
| `metal/msl_shader.das` | hand | `[metal_kernel]` function-macro (`MetalKernel : AstFunctionAnnotation`, modeled on `SpirvShader`), applied to a **class method**; args: `name`, `fastmath` (default **true**). `apply` declares the public MSL-text global - the `name=` argument, or `<Class>_<method>_msl` derived from the method - plus a `<name>_fastmath : bool` companion feeding the pipeline-compile options; **`fixup` fills `glob.init = new ExprConstString(...)`** - string capture is call-free, so fixup suffices (dasGlsl precedent). Does `require msl_emit public` + `require metal_builtins public`. |
| `metal/das_metal_boost.das` | hand | Host sugar over `das_metal`: `with_metal_device`, `pipeline_from_kernel` (compile + error surfacing), unified-memory buffer helpers, `run_compute_1d` one-liner, live-object leak assert. `require das_metal` -> usable only where the C++ module exists. |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS_FROM_DESCRIPTOR(metal metal)` unconditional (rows derived from `.das_module`, emitter everywhere); `IF(APPLE)`: `ADD_MODULE_CPP(DasMetal)` + `ADD_MODULE_LIB` + frameworks. Install rule mirrors dasSpirv's. |

**Extern surface.** `src/dasMetal.mm` binds the compute subset of Metal.framework, in
families:

- **Device + queue:** `metal_create_system_default_device`, `metal_device_name`,
  `metal_device_has_unified_memory`, `metal_max_buffer_length`, `metal_new_command_queue`.
- **Library + pipeline:** `metal_new_library_from_source(dev, src, fastmath; var error)`,
  `metal_new_function`, `metal_new_compute_pipeline`, `metal_pipeline_max_total_threads`,
  `metal_pipeline_thread_execution_width`.
- **Buffers:** `metal_new_buffer` (storageModeShared) and its `_untracked` / `_no_copy` /
  `_no_copy_untracked` variants; `metal_buffer_contents(buf) : void?` - the unified-memory
  pointer the host reads and writes directly, with no map/unmap.
- **Encoding + dispatch:** `metal_new_command_buffer` (+ `_unretained`),
  `metal_new_compute_encoder` (+ `_concurrent`), `metal_set_pipeline`, `metal_set_buffer`,
  `metal_set_bytes`, `metal_set_threadgroup_memory_length`, `metal_dispatch_threadgroups`,
  `metal_dispatch_threads` (the Apple-silicon exact-grid fast path), `metal_memory_barrier`,
  `metal_end_encoding`, `metal_commit`, `metal_wait_until_completed`,
  `metal_dispatch_call_count`.
- **Errors + timing:** `metal_command_buffer_error`, `metal_command_buffer_failed`,
  `metal_command_buffer_gpu_start_time` / `_end_time`, `metal_command_buffer_kernel_start_time`
  / `_end_time`.
- **Events:** `metal_new_shared_event`, `metal_shared_event_signal`, `metal_cb_wait_for_event`.
- **Residency:** `metal_new_residency_set`, `metal_residency_set_add_buffer`,
  `metal_residency_set_commit`, `metal_residency_set_request`, `metal_residency_set_heartbeat`,
  `metal_residency_heartbeat_live` / `_sets` / `_ticks`.
- **Lifetime:** `metal_release` (one overload per handle type), `metal_live_object_count`,
  `metal_live_object_report`.

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

## 4. Test architecture - "every emitted construct has a test"

Three behavioral layers + enforcement gates:

1. **Text-assertion units** (`tests/msl/`, ALL platforms). Each test compiles a tiny
   `[metal_kernel]` fixture, calls `generate_msl`, asserts structural facts (signature
   shape, attribute per binding, const-ness, statement forms), plus a golden snapshot of
   *our own* emitted text as a forward regression guard (dasSpirv's byte-snapshot amendment).
2. **Compile gate** (the spirv-val analog). Every emitted MSL must compile through the
   **real Metal frontend**: in-process `metal_new_library_from_source` where `das_metal`
   exists (no Xcode required - the OS MTLCompilerService), soft-skip elsewhere,
   hard-required on the macOS CI lane. (`xcrun metal` offline compile is a secondary local
   oracle only - it needs full Xcode, not CLT.)
3. **Real-GPU behavioral gate vs the CPU-reference oracle** (`tests/metal/`, Apple-only).
   The kernel body is ordinary daslang, so the **same method runs on the CPU**: a driver loop
   sets `gl_GlobalInvocationID` and calls the method on a class instance whose members are
   plain arrays - that CPU run (interp/JIT) produces the expected buffer contents with zero
   second-source effort. GPU results compare against it: ints bit-exact, floats with tolerance
   (fastmath on both sides - sec.2 decision 8). Files are `require ?das_metal` +
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
- **Leak gate.** Every `tests/metal/` file asserts `metal_live_object_count() == 0` at
  exit - Metal objects are invisible to all six das leak detectors, so the shim counts.

## 5. Cross-backend parity - the kernel-model asymmetry ledger

The mirror rule (REVIEW.md, both emitters) records deliberate or pending kernel-model
asymmetries HERE - one ledger for both backends (dasSpirv's `ARCHITECTURE.md` points at this
section). The primary correctness oracle remains the CPU-reference run of the same das body,
cheaper and stricter than a second GPU; cross-GPU parity of one source is a nice-to-have now
that both emitters read class members. Current entries:

- **Class-member authoring is symmetric now.** dasSpirv gained `[spirv_kernel]` classes (this
  emitter's model, ported wholesale in the vulkan class-kernel arc) - the original blocker on
  stacking both shader annotations on one body is gone. Free-function callees and
  devirtualized overrides are symmetric too (msl_emit's devirtualization table was the donor).
- **Multi-kernel-per-class is symmetric now; `family=` surface sharing stays Vulkan-only.**
  Both lenses accept N kernel methods per class with `kernel=` picking per instance
  (`_mtl_toy.das` / `_vkd_toy.das` are the sibling fixtures). `family=` remains Vulkan's
  alone - Metal's `enc_*` builder is the whole generated surface, so there is nothing for a
  family to share.
- **Method flattening, scope splicing, and plain-member thread-local STATE are Metal-only.**
  `msl_emit` splices method bodies inline (statically devirtualized, value and statement
  position) and lowers plain class members to kernel-entry thread-locals (scalar-with-init,
  fixed array, carried pointer walk - the state contract is per-thread); `spirv_emit` still
  rejects an unannotated member and lowers method calls as calls. If a joined or
  state-carrying class ever crosses backends, the same design ports - deliberate asymmetry
  until then.
- **The tmm2d tensor-GEMM builtin family is Metal-only (pending, not deliberate).** The
  whole-GEMM helpers (`tmm2d_f32_bf16_f32`/`_f16w_f32`/`_q8*_f32` with the bk staging
  depths) and the staged tg protocol (`begin/step` in `deva`/`nt`/`devant` flavors,
  `tile_tg`, `rowscale`, the `store`/`store_half`/`store_bias` tails, `sgmat_to_half`)
  lower MPP `matmul2d` cooperative tensors; the Vulkan analogue would sit on the coopmat2
  classes when a consumer needs it. Until then the CPU-replay bodies are the only
  cross-checked twin.
- **Inheritance in the kernel corpus: Vulkan leans on it, Metal does not yet.** The vulkan
  classes are base+leaf families (kq GEMV/batch, flash attention, deltanet); the metal
  classes are flat, with at least one base duplicated inline (MetalMoeMulMmK6). Dedup
  opportunity, not a correctness gap.
- **Grid-literal validation: Vulkan rejects non-int64 ceil-div grid params at macro apply;
  the metal lens still carries that silent infer trap.** Pending metal-side fix.
- **cm2 decode-in-load (and its class-method decode form) is Vulkan-only by hardware.**
  `[spirv_decode]` - including the method form, where a das-level self is erased from the
  rigid 3-param signature so the decode body reads class members (a separate scale plane,
  push constants, @workgroup staging) - lowers SPV_NV_cooperative_matrix2 tensor loads.
  Metal-4 tensors have no decode-callback analogue; the Metal quant GEMMs stage dequant
  through threadgroup memory instead. Deliberate, target-specific - not a pending port.

## 6. Verification

- **Standing per-change gate (main tree):**
  `daslang dastest/dastest.das -- --test tests/msl --isolated-mode` green on every platform;
  `tests/metal` green on Apple; census == declared set both directions; every emitted kernel
  compile-gate clean; no `GC APP LEAK`; `metal_live_object_count()` 0.
- **M-box run book (behavioral + parity):** `bin/daslang tests/metal/...` on the M-boxes; the
  zen2 side runs the same parity fixtures through dasVulkan.
- **Lint/format:** MCP `format_file` + `lint` on every new `.das`; `cpp_format_file` on the
  `.mm`. PR-mode throughout.

## 7. `@uniform` structs - the kargs form

A kernel with a dozen scalars used to cost a dozen binds. `@uniform @binding = N ka : KArgs`,
where `KArgs` is a plain das struct, costs one: the struct definition emits into the MSL
preamble, the member becomes `constant KArgs& ka [[buffer(N)]]`, and `ka.field` is one hop.

**Fields are `int`/`uint`/`float` only.** That restriction is the whole feature: a struct of
4-byte scalars is the same bytes in das and in MSL, with no padding rules to keep in sync, so
the host writes its own struct straight through `setBytes` (`metal_set_bytes`, already bound -
Metal caps it at 4KB and a kargs struct is well under). A `float4` field would be 16-byte
aligned in MSL and shift every field after it; a nested struct brings its own alignment. Both
are refused (`tests/msl/_fail_closed/_fc_ustruct_{field,nested}.das`).

Host side: `run_compute_1d_kargs` for tests, and dasLLAMA's `kn_bytes` / `kn_kargs` on the
capture rail - under graph capture the bytes are COPIED into a pool, since the caller's kargs is
a stack local long gone by replay time.

**It costs the GPU nothing.** Every field was already a runtime `constant uint&`; the fold trades
N bindings for one argument-buffer read and the kernel branches on exactly the values it did
before. Nothing about the *shape* of a kernel may become a kargs field, though: a block stride, a
lane width, a codec selector belongs in a per-codec overload or a monomorphized generic, where it
stays a literal in the emitted MSL. Passing one as a value and trusting Metal to inline-and-fold
it back is an assumption, not a guarantee - and the assumption is worth nothing in the kernels
that matter.

Tests: `tests/msl/test_msl_uniform_struct.das` (definition placement, the single `constant&`
parameter, no per-scalar parameter survives) and `tests/metal/test_metal_uniform_struct.das`
(GPU vs CPU-oracle - the only thing that can prove the layout claim; mutation-verified by
inserting one pad field into the emitted struct).

## 8. Pointer parameters

A helper takes a raw pointer: `def stage(var p : half4 const?; n : uint)`. This is what lets the
kernels that stream `unsafe(addr(buf[i]))` through a loop - the split-K "D" attention family, the
K-quant mul_mm trio - share a skeleton at all, since the advancing pointer is the shape and
rewriting it as index math changes AGX register allocation.

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
- **The POINTEE's const is the MSL const.** `T const?` lowers `device const T*` and reads only;
  `T?` lowers `device T*` and marks the source member written, exactly as a `var array<T>`
  parameter does. (Writing through it in das additionally needs the handle non-const - `var o :
  T?` - because das flows the handle's const onto the dereference.)

Long-term this is a shortcut for **pointer families** as a language-level feature - a real type
axis rather than an annotation. Parked on `modules/dasLLAMA/followup_general.md`.

Tests: `tests/msl/_msl_common.das` + `test_msl_functions.das` (signatures, forwarding, inline
`addr` arguments, census), `tests/metal/test_metal_functions.das` (GPU advancing dot + threadgroup
slab max vs a directly-computed expectation), `tests/msl/_fail_closed/_fc_ptr_space_{tg,dev}.das`
and `_fc_ptr_untraceable.das` (both mismatch directions + the untraceable case).
