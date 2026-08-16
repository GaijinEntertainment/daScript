# dasMetal — native Metal compute: platform-gated binding + pure-daslang MSL backend

> **This file is the durable masterplan and implementation history for dasMetal.**
> It is the canonical, reviewable record — not Claude memory, not a scratch plan.
> Each phase appends a dated entry to the **Implementation log** at the bottom as it lands.

## Why

dasVulkan owns graphics on every platform (including macOS via MoltenVK). What it cannot
give on Apple Silicon is the **native compute fast path**: no Vulkan-emulation layer under
every dispatch, unified-memory `MTLBuffer`s that are plain pointers on both CPU and GPU,
and Metal-only features (simdgroup reductions, `simdgroup_matrix`). The target profile is
dasLLAMA-class GPU compute offload on the M-boxes. Scope is **compute only** — graphics
stays dasVulkan's job.

The shader-language facts that shape the design: **LLVM cannot emit MSL** (no backend; AIR,
Metal's binary form, is a private version-unstable LLVM-IR dialect) and **Metal does not
ingest SPIR-V** — its sanctioned inputs are MSL source and DXIL. Translation libraries
exist (SPIRV-Cross is MoltenVK's own shader stage), but vendoring one is a third-party C++
dependency that contradicts this module's thesis: *Metal.framework is just there*.

So dasMetal uses the house pattern, already proven three times: dasGlsl emits **GLSL text**
from the daslang AST (`GlslExport`), dasSpirv emits **SPIR-V words**, and `daslib/aot_cpp.das`
emits **C++ for the entire language**. MSL is a C++14 dialect; a compute-only MSL text
emitter is squarely inside the established pattern. Just as `[compute_shader]` lowers
daslang→SPIR-V→Vulkan, `[metal_kernel]` lowers daslang→MSL→Metal.

What makes it cheap: Metal's in-process runtime compiler (`newLibraryWithSource`) does all
GPU optimization; text is the easiest backend to emit and debug; and the authoring frontend
(module globals + `@ssbo`/`@binding` field annotations + builtin globals) already exists in
dasSpirv and is reused as-is.

## Settled decisions

1. **Lives in the main tree as `modules/dasMetal`.** The C++ binding is APPLE-gated; the
   `metal/` daslang files (emitter + annotation) are **pure das and registered on ALL
   platforms** — emitter/text tests run on every CI lane; only GPU execution is Apple-only.
2. **Compute only.** No render pipelines, no drawables, no CAMetalLayer. Graphics = dasVulkan.
3. **Zero third-party code.** No metal-cpp, no SPIRV-Cross, no MoltenVK, no committed
   `.msl`/`.metallib`, no external SDK. The binding is a hand-written Obj-C++ shim
   (`src/dasMetal.mm`) over the system Metal.framework — ~18 externs for the compute subset.
   (metal-cpp + dasClangBind rejected: vendored headers and binder churn for an ~18-extern
   surface; revisit only if the surface outgrows hand maintenance.)
4. **Class-based authoring: kernels are class methods, resources are class members.** A
   compute pass is a `class` whose `@ssbo @binding = N` members declare the buffers and
   whose `[metal_kernel]` methods are the kernels — no module-scope resource globals (they
   pollute the namespace and don't scale to two kernels with different buffer sets in one
   file), and members→kernel-parameters is exactly MSL's own model. Multiple kernels over
   one buffer set = multiple methods in one class. Builtin globals
   (`gl_GlobalInvocationID`, …) are still reused via `require spirv/spirv_builtins public`
   (dasSpirv is unconditional in-tree pure das — always resolves). Because the body is
   ordinary daslang, the same method also executes on the CPU — the primary correctness
   oracle (Test architecture). **Zero edits to shipped dasSpirv/dasGlsl.**
5. **Naming.** C++ module `das_metal` (`Module("das_metal")`, class `Module_DasMetal`). das
   files under `metal/`: `require metal/msl_shader`, `require metal/das_metal_boost`.
   Consumers guard: `require ?das_metal metal/das_metal_boost` +
   `static_if (typeinfo builtin_module_exists(das_metal))` (the sql_boost provider pattern).
   If a top-level registration proves workable in Phase 0, the spelling may shorten to a
   bare `das_metal_boost`; the two-segment guarded form is the safe default.
6. **Test-per-construct is a hard requirement**, enforced by a construct census (gate B) +
   the real-frontend compile gate. LCOV covers runtime files only — the emitter runs at
   compile time, invisible to line coverage (dasSpirv finding; census is the proxy).
7. **PoC before breadth.** The earliest milestone is `c[i] = a[i]*b[i]` through the REAL
   emitter on a real M-series GPU. The only hand-written MSL ever permitted is the Phase-0
   dev-scaffold **inline string** proving the binding, deleted when Phase 1 lands (the
   `_handbuild_square.das` pattern). **No `.msl` file is ever committed.**
8. **`fastmath` is a `[metal_kernel]` property, ON by default.** daslang's own posture is
   fastmath-on, so we are not chasing bit-exact float parity; Metal's runtime compiler also
   defaults fast-math ON and we keep it. Float oracles compare with tolerance (ints
   bit-exact). `[metal_kernel(fastmath=false)]` per kernel when isolating a divergence
   needs strict IEEE.

## Architecture

`modules/dasMetal` = one APPLE-gated Obj-C++ shim + pure-daslang emitter files:

| File | Gen/Hand | Purpose |
|---|---|---|
| `src/dasMetal.mm` | hand | `Module("das_metal")` — Obj-C++ shim over Metal.framework. Opaque annotated handles (device, queue, command buffer, compute encoder, pipeline state, library, function, buffer) + the extern surface below. Compiled with ARC; handles cross to das as `__bridge_retained void*`; `metal_release` = `__bridge_transfer`. Shim-side live-object counter for the leak gate. APPLE-only; links `-framework Metal -framework Foundation`. |
| `metal/msl_types.das` | hand | daslang `TypeDecl` → MSL type name (32-bit scalars/bool + the 16/8-bit lattice — MSL has native `half`/`short`/`char` — and their 2/3/4 vectors, classified via the shared `daslib/shader_block_layout` rails). `msl_buffer_elem_name` gives the layout-bearing spelling: 3-lane elements take MSL's `packed_T3` (das packs tightly; unified memory means the das array IS the buffer). |
| `metal/metal_builtins.das` | hand | Metal-only builtin surface over the shared lingua franca (the spirv_builtins pattern): re-exports `daslib/shader_lingua_franca` (whence `gl_WorkGroupSize` and the four `gl_Subgroup*` IDs, shared with dasSpirv) and adds the `simd_sum`/`simd_shuffle*` intrinsics (Metal spellings, float/int/uint). Identity stub bodies = width-1 simdgroup CPU semantics. |
| `metal/msl_emit.das` | hand | The text emitter: `[macro_function] generate_msl(fn, var errors, cfg, var census) : string`. Manual recursion (`emit_value`/`emit_stmt`, mirroring `spirv_emit`). Kernel-signature synthesis from `@ssbo` globals (the one structural novelty — below). Records the construct census at every emit site. |
| `metal/msl_shader.das` | hand | `[metal_kernel]` function-macro (`MetalKernel : AstFunctionAnnotation`, modeled on `SpirvShader`), applied to a **class method**; args: `name`, `fastmath` (default **true**, surfaced to the host via a companion ``{name}`msl_fastmath : bool`` global feeding the pipeline-compile options). `apply` declares the public ``{name}`msl : string`` global; **`fixup` fills `glob.init = new ExprConstString(...)`** — string capture is call-free, so fixup suffices (dasGlsl precedent; the patch/astChanged dance dasSpirv needed for `array<uint>` does not apply). Does `require spirv/spirv_builtins public`. |
| `metal/das_metal_boost.das` | hand | Host sugar over `das_metal`: `with_metal_device`, `pipeline_from_kernel` (compile + error surfacing), unified-memory buffer helpers, `run_compute_1d` one-liner, live-object leak assert. `require das_metal` → usable only where the C++ module exists. |
| `CMakeLists.txt` | hand | `ADD_MODULE_DAS(metal metal …)` unconditional (emitter everywhere); `IF(APPLE)`: `ADD_MODULE_CPP(DasMetal)` + `ADD_MODULE_LIB` + frameworks. Install rule mirrors dasSpirv's. |

**Extern surface (PoC-complete).** `metal_create_system_default_device`; device name +
unified-memory query; `metal_new_command_queue`; `metal_new_library_from_source(dev, src,
fastmath; var error)`; `metal_new_function(lib, name)`; `metal_new_compute_pipeline(dev,
fn; var error)`; `metal_new_buffer(dev, bytes)` (storageModeShared);
`metal_buffer_contents(buf) : void?` (the unified-memory pointer — host reads/writes it
directly, no map/unmap); `metal_new_command_buffer(q)`; `metal_new_compute_encoder(cb)`;
`metal_set_pipeline`; `metal_set_buffer(enc, buf, offset, index)`;
`metal_dispatch_threadgroups(enc, groups, threads_per_group)` (+`metal_dispatch_threads`
as the Apple-silicon exact-grid fast path); `metal_end_encoding`; `metal_commit`;
`metal_wait_until_completed`; `metal_command_buffer_error`; `metal_release`;
`metal_live_object_count`.

**Kernel-signature lowering (the one structural novelty).** MSL has no module-scope device
globals — and the authoring class's members map onto its model exactly: each `@ssbo`
member lowers to a kernel parameter `device T* name [[buffer(N)]]` (`device const T*` when
no kernel body writes it; member write-set collected in a pre-scan), and each referenced
builtin global lowers to a builtin-attributed parameter (`gl_GlobalInvocationID` →
`uint3 gl_GlobalInvocationID [[thread_position_in_grid]]`). Member access in the method
body (bare `a` / `self.a`) emits as the bare parameter name, so the body needs no other
rewriting. `@binding` = the flat `[[buffer(N)]]` index; `@set` must be absent or 0 (clean
error — Metal has no descriptor sets); duplicate bindings within one class are a clean
error. Identifiers colliding with MSL keywords (`kernel`, `device`, `constant`, `thread`,
`half`, …) are mangled. The exact AST shape of method-member access (`ExprField` over
`self`, constness, `ExprRef2Value` wrapping at fixup time) is dumped and recorded as the
first Phase-1 step — the same discipline as dasSpirv's square AST dump.

**Capture + cache.** Capture mirrors dasGlsl exactly (`ExprConstString` in fixup). A
companion reflection global is deferred until the boost needs auto-binding (mirror
`spirv_reflect` then). Text emission is fast enough to run every compile; if pipeline
compilation latency ever matters, an `MTLBinaryArchive` cache keyed by
`MSL_CODEGEN_VERSION` + `get_function_aot_hash(fn)` is the llvm_jit-pattern answer.

## PoC feature set (Phase-1 milestone)

The a*b kernel, authored:

```das
require metal/msl_shader

class MulAB {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    [metal_kernel]
    def mul_ab { let i = gl_GlobalInvocationID.x; c[i] = a[i] * b[i] }
}
// host reads mul_ab`msl : string → pipeline_from_kernel → run_compute_1d
```

Expected emission (the construct inventory that seeds the test matrix):

```
#include <metal_stdlib>
using namespace metal;
kernel void mul_ab(device const float* a [[buffer(0)]],
                   device const float* b [[buffer(1)]],
                   device float* c [[buffer(2)]],
                   uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) {
    uint i = gl_GlobalInvocationID.x;
    c[i] = a[i] * b[i];
}
```

Constructs: preamble; kernel signature (device-pointer param, const-ness inference,
`[[buffer(N)]]`, builtin param); value-`let`; single-component swizzle; array index →
pointer index; `ExprOp2 *` (float); `ExprCopy` → assignment.

## Test architecture — "every emitted construct has a test"

Three behavioral layers + enforcement gates:

1. **Text-assertion units** (`tests/msl/`, ALL platforms). Each test compiles a tiny
   `[metal_kernel]` fixture, calls `generate_msl`, asserts structural facts (signature
   shape, attribute per binding, const-ness, statement forms), plus a golden snapshot of
   *our own* emitted text as a forward regression guard (dasSpirv's byte-snapshot amendment).
2. **Compile gate** (the spirv-val analog). Every emitted MSL must compile through the
   **real Metal frontend**: in-process `metal_new_library_from_source` where `das_metal`
   exists (no Xcode required — the OS MTLCompilerService), soft-skip elsewhere,
   hard-required on the macOS CI lane. (`xcrun metal` offline compile is a secondary local
   oracle only — it needs full Xcode, not CLT.)
3. **Real-GPU behavioral gate vs the CPU-reference oracle — as early as Phase 1**
   (`tests/metal/`, Apple-only). The kernel body is ordinary daslang, so the **same method
   runs on the CPU**: a driver loop sets `gl_GlobalInvocationID` and calls the method on a
   class instance whose members are plain arrays — that CPU run (interp/JIT) produces the
   expected buffer contents with zero second-source effort. GPU results compare against
   it: ints bit-exact, floats with tolerance (fastmath on both sides — settled decision 8;
   the PoC's `a[i]*b[i]` on small ints is exact regardless). Files are `require ?das_metal`
   + `static_if builtin_module_exists` guarded, so they compile and no-op cleanly on
   non-Apple lanes. Primary gate = **local M-boxes**; CI = the macOS lane's paravirtual
   device (Phase-0 probe decides behavioral vs compile-only).

- **Gate A — LCOV** on runtime-reached files (`das_metal_boost`, `msl_types` where runtime
  code exists). The emitter is compile-time → census is its coverage proxy.
- **Gate B — construct census.** `generate_msl` records every construct kind it emits
  (`table<string>` set: `"kernel_sig"`, `"param.buffer"`,
  `"param.builtin.thread_position_in_grid"`, `"op.mul.f32"`, …). A meta-test unions the
  census across all fixtures and asserts equality with the declared supported set, both
  directions. Census-record and emit are fused in one helper so they cannot drift; the
  golden-snapshot layer catches emissions that bypass it.
- **Leak gate.** Every `tests/metal/` file asserts `metal_live_object_count() == 0` at
  exit — Metal objects are invisible to all six das leak detectors, so the shim counts.

## Phasing (independently verifiable; no sizing)

- **Phase 0 — binding bring-up + probes.** This file; `src/dasMetal.mm` (module +
  externs); the CMake APPLE gate; `_handrolled_mul.das` dev scaffold with an **inline** MSL
  string (never a committed `.msl`; deleted at Phase 1) proving
  device→queue→pipeline→dispatch→readback + error surfacing on an M-box. Probes:
  (a) **macOS CI lane** — does the paravirtual device execute the scaffold? decides CI
  behavioral vs compile-only; (b) bad MSL surfaces the compiler log as a clean das error;
  (c) live-object counter round-trips to zero.
- **Phase 1 — a*b through the emitter (the PoC milestone).** `msl_types` + `msl_emit` PoC
  subset + `[metal_kernel]` + string capture + `das_metal_boost` v1 + census + compile gate
  + behavioral gate green on an M-box (+CI per the Phase-0 probe). Delete the Phase-0
  scaffold string. Register `tests/msl` in `tests/aot/CMakeLists.txt` (5-site pattern).
- **Phase 2 — arithmetic + control-flow breadth.** Mirror dasSpirv Phase 2's inventory:
  full scalar/vector arithmetic, comparisons, logical ops, mutable locals, if/else, while,
  range-for, break/continue, compound assignment. MSL is C-family — mostly emitter
  table-fill + tests; no structured-CFG hazard. The in-kernel bounds guard
  (`if (i >= n) return` for non-exact grids) lands here. Every fixture regresses against
  its CPU-reference run.
- **Phase 3 — threadgroup memory + barriers + simdgroup ops.** `threadgroup` address
  space, `threadgroup_barrier(mem_flags)`, `simd_sum`/`simd_shuffle*`, the remaining
  builtin attributes (`thread_position_in_threadgroup`, `threadgroup_position_in_grid`,
  `threads_per_threadgroup`). The Metal value-add real kernels need.
- **Phase 4 — 16/8-bit lattice.** `half` (native in MSL), short/ushort/uchar vectors —
  extend `msl_types`, conformance fixtures parity vs the CPU lattice and the
  dasSpirv/dasGlsl shader-lattice rails (`tests/type_lattice` discipline).
- **Phase 5 — `simdgroup_matrix` + host perf plumbing.** `simdgroup_float8x8` et al. (the
  dasLLAMA prize), buffer pooling, `MTLSharedEvent` sync, `MTLBinaryArchive` pipeline
  cache. Sized when we get there; a dasLLAMA offload experiment is the driver.
- **Phase 6 — full-GPU-resident prefill for ONE model (Llama-3.2-1B), chase the 3960.**
  Mandate (Boris, 2026-07-11, after the Phase-5 clean round): cover the one model end to end
  and see how close we get to llama.cpp full-Metal (pp512 3960 t/s vs our hybrid's 1055 —
  the gap is residency: per-GEMM sync submits + activation/output memcpys + CPU attention).
  Sized at session start; expected legs: emitter math-builtin whitelist growth (exp/sqrt/
  rsqrt/min/max — rmsnorm + softmax kernels need them), attention/norm/rope kernels over the
  Phase-3/5 constructs, activations resident in MTLBuffers across the layer loop (one encoder
  per prefill via `with_compute_encoder`, unified memory hands the KV cache back to the CPU
  decode path), integration above the batch-slot seam (the per-arch prefill blocks —
  `ArchBlocks` — or a whole-prefill override behind the same env-pin discipline).
  **Sizing (2026-07-11 session start, from the forward-path map).** The 7 per-layer q8 GEMMs
  (Q/K/V/O/W1/W3/W2) already route through the batch slot; the CPU remainder is `rms_batch`,
  `rope_batch_tab` (TABLE-driven — cos/sin precomputed per prefill, so the GPU rope kernel
  needs no trig), the per-GEMM activation requant, `prefill_attention`, `gate_batch`
  (swiglu), `add_inplace`, `kv_store_batch`; final norm + classifier is last-row-only GEMV
  and stays CPU. Legs, in order:
  - **6.0 — detector first.** `metal_command_buffer_gpu_start_time/_end_time` externs
    (`GPUStartTime`/`GPUEndTime`) + structured per-stage host timing in the driver — the
    GPU-busy vs wall vs upload/readback split steers every later leg.
  - **6a — emitter math builtins.** Value-call whitelist exp/sqrt/rsqrt/min/max/abs/
    saturate/tanh/sin/cos (das math-module names ARE the MSL spellings; float scalar +
    vector classes); census + msl goldens + a metal behavioral fixture.
  - **6b — kernels** (each: msl golden + census + CPU-reference metal test): `quantize_q8`
    (f32 → int8 + scale per 32-group, writing the padded-M layout the GEMM reads), `rmsnorm`
    (simd_sum + threadgroup reduction, rsqrt), `rope` (reads the uploaded cos/sin tables),
    `swiglu`, residual `add`, and the attention pair — `attn_scores` (scaled q·kT + causal
    mask + row softmax) and `attn_av` (P·V), GQA kv_mul=4, f32. Naive attention first:
    ~17 GMAC vs ~630 GMAC of projections at N=512 — flash fusion only if the detector says
    so. Scores scratch (heads x N x N f32 ≈ 32 MB) reused across layers.
  - **6c — resident driver.** dasllama_math_metal.das grows a whole-prefill path: weights
    already lazily resident (+ the small per-layer norm-weight vectors); pooled resident
    activation set (x/xb/xb2/q/k/v/hb/hb2/xq/xs/scores + per-layer K/V outs); ONE command
    buffer per prefill (~10 dispatches x 16 layers, default hazard tracking orders). Host:
    embed rows (CPU) → upload x_b + rope tables → encode → one wait → per-layer K/V
    readback → `kv_store_batch` (session KV codec stays CPU — any KV dtype works) →
    last-row readback → CPU final norm + classifier.
  - **6d — integration + parity + measurement.** Whole-prefill override seam (chosen over
    per-block ArchBlocks twins: one seam owns encode→wait→readback): fn-ptr slot +
    select-by-name + `DASLLAMA_PIN_PREFILL` env in the harnesses, same pin discipline;
    fall back to the hybrid/CPU path for non-q8, start_pos>0, small npos, or non-std arch
    shapes. Gates: kernel unit parity tests, 40/40 greedy parity (tolerance path), then the
    clean round (Parsec OFF — Boris window): CPU vs hybrid vs resident vs lcpp `-ngl 99`.

## Cross-backend parity — the kernel-model asymmetry ledger

The mirror rule (REVIEW.md, both emitters) records deliberate or pending kernel-model
asymmetries HERE — one ledger for both backends (dasSpirv's MASTERPLAN points at this
section). The primary correctness oracle remains the CPU-reference run of the same das body,
cheaper and stricter than a second GPU; cross-GPU parity of one source is a nice-to-have now
that both emitters read class members. Current entries:

- **Class-member authoring is symmetric now.** dasSpirv gained `[spirv_kernel]` classes (this
  emitter's model, ported wholesale in the vulkan class-kernel arc) — the original blocker on
  stacking both shader annotations on one body is gone. Free-function callees and
  devirtualized overrides are symmetric too (msl_emit's devirtualization table was the donor).
- **Multi-kernel-per-class is symmetric now; `family=` surface sharing stays Vulkan-only.**
  Both lenses accept N kernel methods per class with `kernel=` picking per instance
  (`_mtl_toy.das` / `_vkd_toy.das` are the sibling fixtures). `family=` remains Vulkan's
  alone — Metal's `enc_*` builder is the whole generated surface, so there is nothing for a
  family to share.
- **Method flattening, scope splicing, and plain-member thread-local STATE are Metal-only.**
  `msl_emit` splices method bodies inline (statically devirtualized, value and statement
  position) and lowers plain class members to kernel-entry thread-locals (scalar-with-init,
  fixed array, carried pointer walk — the state contract is per-thread); `spirv_emit` still
  rejects an unannotated member and lowers method calls as calls. If a joined or
  state-carrying class ever crosses backends, the same design ports — deliberate asymmetry
  until then.
- **Inheritance in the kernel corpus: Vulkan leans on it, Metal does not yet.** The vulkan
  classes are base+leaf families (kq GEMV/batch, flash attention, deltanet); the metal
  classes are flat, with at least one base duplicated inline (MetalMoeMulMmK6). Dedup
  opportunity, not a correctness gap.
- **Grid-literal validation: Vulkan rejects non-int64 ceil-div grid params at macro apply;
  the metal lens still carries that silent infer trap.** Pending metal-side fix.
- **cm2 decode-in-load (and its class-method decode form) is Vulkan-only by hardware.**
  `[spirv_decode]` — including the method form, where a das-level self is erased from the
  rigid 3-param signature so the decode body reads class members (a separate scale plane,
  push constants, @workgroup staging) — lowers SPV_NV_cooperative_matrix2 tensor loads.
  Metal-4 tensors have no decode-callback analogue; the Metal quant GEMMs stage dequant
  through threadgroup memory instead. Deliberate, target-specific — not a pending port.

## Top risks

1. **macOS CI paravirtual Metal.** GitHub's macOS runners expose an "Apple Paravirtual
   device"; whether it executes our compute reliably is unproven → Phase-0 probe. Fallback:
   CI = compile gate only (still the real Metal frontend), behavioral = local M-boxes with
   a run book (the `tests/opengl_gpu/_lattice_gpu_parity.das` precedent).
2. **Obj-C lifetime discipline.** Retained-handle convention + explicit `metal_release`,
   enforced by the live-object leak gate; the boost's `with_`/finally wrappers are the
   user-facing rail so bare handles rarely cross user code.
3. **Kernel-arg lowering completeness.** Const-ness inference (write-set pre-scan), MSL
   keyword mangling, duplicate `@binding` = clean error, `@set` rejection. Fail-closed rule
   inherited from dasSpirv review hardening: every dispatch validates its input and errors
   cleanly — never a silent bad kernel, never a panic.
4. **Class-method annotation plumbing.** `[metal_kernel]` must fire on a class method
   (function_macro apply/fixup on methods; `@ssbo` field annotations on class members) —
   unproven; probed first thing in Phase 1 before any emitter work. Fallback if it fights:
   the struct-param form (`[metal_kernel] def mul_ab(var k : MulAB)` — free function,
   resources still grouped in one type), same emitter, only the frontend scan differs.
5. **Runtime-compile latency.** Per-pipeline `newLibraryWithSource` at startup is fine for
   tests/PoC; `MTLBinaryArchive` + codegen-version key is the escape hatch (Phase 5).
6. **Census honesty for a text emitter.** No disassembler exists to recount text (dasSpirv
   could re-derive opcodes from the blob). Mitigations in Test architecture (fused
   record+emit helper; golden snapshots).
7. **Dispatch model.** `dispatch_threads` (exact grid) needs Apple-family GPUs — fine on
   M-boxes, unproven on paravirtual. PoC uses exact-multiple sizes +
   `dispatch_threadgroups`; the guarded form arrives with Phase 2's bounds-guard construct.

## Verification (end to end)

- **Phase 0:** scaffold green on an M-box (readback exact, live objects 0, bad-MSL error
  surfaced with the compiler log); CI-probe result recorded in the log below.
- **Per phase (main tree):**
  `daslang dastest/dastest.das -- --test tests/msl --isolated-mode` green on every
  platform; `tests/metal` green on Apple; census == declared set both directions; every
  emitted kernel compile-gate clean; no `GC APP LEAK`; `metal_live_object_count()` 0.
- **M-box run book (behavioral + parity):** `bin/daslang tests/metal/...` on M1/M3 Air;
  the zen2 side runs the same parity fixtures through dasVulkan.
- **Lint/format:** MCP `format_file` + `lint` on every new `.das`; `cpp_format_file` on the
  `.mm`. PR-mode throughout.

---

# Implementation log

**2026-07-11 — masterplan.** Authored; revised same day per review: class-based authoring
(kernels = methods, resources = `@ssbo` members — no module-scope resource globals),
`fastmath` a `[metal_kernel]` property defaulting ON, CPU-reference oracle promoted to the
primary correctness gate, cross-backend GPU parity demoted to a deferred nice-to-have.

**2026-07-11 — Phase 0 landed: binding bring-up + probes.** `src/dasMetal.mm`
(`Module("das_metal")`, class `Module_DasMetal`, ARC-compiled, 8 `DummyTypeAnnotation`
opaque handles, the full PoC extern surface incl. 8 `metal_release` overloads +
`metal_live_object_count`) + `src/dasMetal.h` (pure-C++ decls for `aotRequire`);
`CMakeLists.txt` APPLE-gated (`ADD_MODULE_CPP(DasMetal)` + `ADD_MODULE_LIB`, `-fobjc-arc`
on the `.mm`, Metal+Foundation frameworks); `.das_module` descriptor (silent 2-arg
`register_dynamic_module` — skips cleanly on non-Apple); root `DAS_METAL_DISABLED` option
(default OFF). Scaffold `_handrolled_mul.das` green on M1 Max (macOS 26.4, arm64):
- **Readback exact** — a*b over 256 floats via BOTH `metal_dispatch_threadgroups` and
  `metal_dispatch_threads` (exact-grid fast path confirmed working on Apple-family GPU —
  risk 7 half-resolved; paravirtual still unknown).
- **Bad-MSL probe** — `undeclared_identifier` fixture surfaces the real Metal compiler
  log (with source line + caret) as a clean das error string; no crash, no nil deref.
- **Leak probe** — live-object counter round-trips to 0 after releasing all 12 handles.
- **Fail-closed probe** (beyond plan) — null handle into any extern throws a clean,
  `recover`-able das error via `throw_error_at` (every extern null-guards its handles).
Implementation notes: `char *&` error out-params follow the fio pattern
(`SideEffects::modifyArgumentAndExternal`); handles cross as `__bridge_retained void*`,
`metal_release` = `__bridge_transfer`, counted by a shim-side atomic;
`MTLCompileOptions.mathMode` (macOS 15+ SDK) with `fastMathEnabled` fallback. das-side
gotcha for the Phase-1 emitter: `label` and `expect` are reserved words; literal `{`/`}`
in MSL strings must be escaped `\{`/`\}` (string interpolation).
**Probe (a) — macOS CI paravirtual execute — PENDING:** `build.yml` runs on
PR/master-push/dispatch only and the branch is local; the macOS lanes are `macos-15` /
`macos-26` Apple-Silicon (M2) runners with the Apple Paravirtual device. Resolve when the
arc branch first goes to CI; until then CI posture = compile gate, behavioral = M-boxes.

**2026-07-11 — Phase 1 landed: a*b through the REAL emitter (the PoC milestone).**
`metal/msl_types.das` (32-bit scalar + 2/3/4-vector map), `metal/msl_emit.das`
(`generate_msl` — member scan with duplicate-binding/`@set`/non-`@ssbo` fail-closed errors,
write-set pre-scan for `device` vs `device const`, builtin classifier, manual-recursion
statement/expression emit with fused census recording, MSL keyword mangling),
`metal/msl_shader.das` (`[metal_kernel]` — apply declares 4 companion globals
`N`/`N_entry`/`N_fastmath`/`N_census`, fixup fills all as constant nodes),
`metal/das_metal_boost.das` v1 (`with_metal_device`, `pipeline_from_source`,
`run_compute_1d`, `assert_no_metal_leaks`). Emitted text matches this file's expected
emission byte-for-byte (modulo param-wrap indent + defensive parens). Gates green on M1 Max:
`tests/msl` (shape asserts + golden snapshot + census == declared set both directions, 8/8)
and `tests/metal` (emitted MSL on GPU == CPU-reference run of the same method — the driver
loop feeds `gl_GlobalInvocationID` — plus leak gate, 2/2); both suites pass through the
dynamic AND static binaries. Phase-0 scaffold deleted on schedule. Wiring: unconditional
`ADD_MODULE_DAS` + dasSpirv-style install rule; `.das_module` `register_native_path` for
the four metal files; `tests/msl` + `tests/metal` registered in `tests/aot/CMakeLists.txt`
(fixture `_msl_common` AOT_LIB'd per the `_spirv_common` precedent; macro-only metal
modules deliberately not AOT'd, per the spirv_emit precedent).
Probe results (annotation plumbing — risk 4 CLOSED, no struct-param fallback needed):
`[function_macro]` fires on class methods. The apply/fixup division is dasGlsl's, strictly:
`apply` is pre-infer (types unresolved — aliases still aliases), so it does ONLY the two
type-independent actions: `exports = true` (the method is otherwise culled before `fixup`
runs — probe-verified) and declaring the fixed-type (`tString`/`tBool`) companion globals so
consumers resolve in the first inference pass. ALL type-dependent work — member scan,
write-set, emission — runs in `fixup`, post-infer, where `fld._type` is resolved;
`@ssbo`/`@binding` field annotations read via `field.annotation |> find_arg`; the member
scan skips `__rtti`/`__finalize` + function-typed fields (classes store methods as fields);
by fixup time the `with(self)` wrapper is optimizer-dropped and access is
`ExprAt(ExprField(ExprVar(self), name), index)` — but under `no_optimizations` compiles
(lint/LSP) the wrapper + non-folded shapes survive, so the emitter treats `ExprWith` as
transparent (census-silent, or the gate would depend on optimization level).
Plan deviations, both deliberate: builtins come from `require daslib/shader_lingua_franca
public` (where the compute IDs actually live — `spirv/spirv_builtins` merely re-exports
them and adds VK-specific surface dasMetal doesn't want); the declared census lives in the
test fixture `_msl_common.das`, not the emitter (the emitter modules are macro-only and
never AOT'd — a runtime `declared_msl_census()` there would 50101 under `test_aot`).
das gotcha for kernel authors: emitter-facing das reserved words `label`/`expect` (now in
CLAUDE.md gotchas). CI paravirtual probe still pending first push.

**2026-07-11 — Phase 2 landed: arithmetic + control-flow breadth.** The emitter now covers the
full dasSpirv-Phase-2-equivalent inventory: scalar arithmetic + bitwise + shifts for
uint/int/float (float `%` → `fmod` — MSL `%` is integral-only), componentwise vector arithmetic
incl. `vec*scalar` broadcast and vector negate, comparisons per scalar class, das's scalar-bool
vector `==`/`!=` (→ `all(...)`/`any(...)` — MSL's own vector compare yields `boolN`), logical
`&&`/`||`/`!` (short-circuit matches C on both sides), ternary, pre/post `++`/`--` (statement-level
das postfix arrives as prefix in the AST; value-level postfix survives), compound assignment
(arithmetic + bitwise), mutable locals + das zero-init locals (`var x : T` → `T x = {};`),
if/elif/else chains (emitted flat as `} else if`), `while`, `break`/`continue`, void `return`, and
`for (i in range/urange(...))` — with the das-semantics subtlety that range endpoints evaluate
ONCE: a non-constant end is hoisted into a loop-scoped comma-declared local
(`for (int k = lo, k_end = expr; ...)`), proven behaviorally by a fixture that mutates the end's
source variable inside the body. New frontend surface: **`@uniform @binding = N` scalar/vector
members** → `constant T& name [[buffer(N)]]` (read-only — a written uniform is a clean error;
arrays stay `@ssbo`), which is what the **in-kernel bounds guard** (`if (i >= n) return`) reads.
Literal fixes: das prints integral floats bare ("2" + `f` is invalid MSL → `.0` appended,
non-finite = error) and uints as hex (decimal via int64 detour). Census grew 13 → 106 kinds.
Tests: `tests/msl` 37/37 — per-class shape asserts + goldens (uarith/iarith/farith/vecarith/
control/loops), census union == declared both directions, and a fail-closed suite
(`_fail_closed/` + compile_issues, mirroring tests/spirv): written `@uniform`, array `@uniform`,
value `return`, unsupported call. `tests/metal` 14/14 on M1 Max — every fixture regresses GPU vs
the CPU-reference oracle (ints bit-exact incl. wrap/truncation, floats fastmath-tolerance), with
the loops kernel dispatched on a deliberately NON-exact grid (1000 threads @ 64/tg → 1024
launched) over sentinel-filled buffers compared across the FULL padded range — a broken guard
shows as clobbered sentinels. Shared GPU helpers live in `tests/metal/_metal_common.das` as
GENERIC functions (untyped `dev`/`buf` params): generics only instantiate at call sites inside
`static_if (builtin_module_exists(das_metal))`, so the fixture compiles on every platform;
AOT-wired as `test_aot_metal_modules` (msl-modules pattern; the metal glob gained the `/_` filter).
Green: interp + `-jit` + static binary, MCP lint + CI lint clean. das gotchas hit: `expect` is
reserved (fixture param renamed `want`); `float4 + float` does NOT broadcast in das (only `*`/`/`).

**2026-07-11 — Phase 3 landed: threadgroup memory + barriers + simdgroup ops.** New frontend
surface, dasSpirv-aligned where the construct is shared: **`@workgroup` class members**
(scalar/vector or single-dimension fixed array, e.g. `@workgroup tile : uint[64]`) lower to
`threadgroup T name[N];` declarations at the top of the kernel body (MSL has no module-scope
threadgroup variables; name-sorted, never parameters, @binding on one is a clean error, dynamic
`array<T>` is a clean error); **`barrier()` / `memoryBarrierShared()`** (lingua franca) both lower
to `threadgroup_barrier(mem_flags::mem_threadgroup)` — MSL has no execution-free threadgroup fence
pre-MSL-3.2, so the memory-only form strengthens to the full rendezvous, the same lowering
SPIRV-Cross ships under MoltenVK. New module **`metal/metal_builtins.das`** (pure das, ALL
platforms — the spirv_builtins pattern: re-export lingua franca + add the Metal-only surface):
`gl_WorkGroupSize : uint3` (a layout CONSTANT in GLSL, a real `[[threads_per_threadgroup]]` param
in Metal), the four simdgroup IDs under their GLSL subgroup spellings (`gl_SubgroupInvocationID`/
`gl_SubgroupSize`/`gl_NumSubgroups`/`gl_SubgroupID`), and the simd intrinsics under their Metal
spellings — `simd_sum` + `simd_shuffle`/`_up`/`_down`/`_xor` × float/int/uint (15 overloads;
identity stub bodies = the degenerate-but-valid width-1 simdgroup semantics for CPU replay). The
builtin classifier now spans 10 attributes (`thread_position_in_threadgroup`,
`threadgroup_position_in_grid`, `threadgroups_per_grid`, `thread_index_in_threadgroup`,
`threads_per_threadgroup`, `thread_index_in_simdgroup`, `threads_per_simdgroup`,
`simdgroups_per_threadgroup`, `simdgroup_index_in_threadgroup` + Phase-1's grid ID). Census
106 → 143 kinds. Tests: `tests/msl` 48/48 — threadgroup + simd shape/golden fixtures, census both
directions, fail-closed grew `_fc_wg_binding` (bound @workgroup) + `_fc_wg_type` (dynamic-array
@workgroup) and the user-call needle moved to the new whitelist error. `tests/metal` 20/20 on
M1 Max — **the CPU-reference oracle splits three ways by construct**: (a) `grid_ids` kernel (no
barrier/simd) keeps the same-body sequential replay, driver sets all six builtin globals per
thread, bit-exact; (b) the barrier tree-reduction kernels (uint wrap-exact + float
fastmath-tolerance) compare against DIRECTLY COMPUTED per-group sums — a barrier kernel cannot be
sequentially replayed (thread 0 would reduce before the other lanes load), so the honest oracle is
the arithmetic itself; (c) the simd kernel uses a READBACK-DRIVEN oracle — the kernel reports each
thread's simdgroup width/lane/id, the CPU recomputes every expected value from reported
membership, layout-agnostic (M1 Max reports width 32; shuffles execute in simdgroup-uniform flow
with undefined edge lanes selected away on the VALUE — a shuffle under divergence reads inactive
lanes). Green: interp + `-jit` + static binary; MCP lint + CI lint clean. das gotcha hit: a value
witness param (`witness : auto(TT)`) binds TT WITH the parameter's appended const (yields
`array<T const>`) — even via `type<auto(TT)>`; the fix is `-const` at every use site
(`array<TT -const>`, `_metal_common.das` `buf_download`).

**2026-07-11 — Phase 4 landed: the 16/8-bit lattice.** `msl_types` rewritten on the shared
`daslib/shader_block_layout` classifiers (`scalar_class`/`scalar_width`/`component_count`) — one
stroke covers all 20 shader-legal lattice types (`float16`→`half`, `int8`→`char`, `uint8`→`uchar`,
`int16`→`short`, `uint16`→`ushort`, + their 2/3/4 vectors; das `byte` is SIGNED int8 = MSL `char`).
The 8/16-lane CPU-only forms get the dedicated dasGlsl-worded diagnostic
(`cpu_only_lattice_width`) at every type-bearing site: @ssbo element, @uniform, @workgroup, local
declaration, and ctor calls. **The packed 3-lane rail** (the one layout novelty): das packs
vectors tightly (`float3` = 12B, `half3` = 6B, `byte3` = 3B) while non-packed MSL 3-vectors
stride padded (16B/8B/4B) — and with unified memory the das array IS the buffer, so 3-lane
buffer/uniform elements lower to MSL's byte-identical `packed_T3` spelling
(`msl_buffer_elem_name`). This also fixes the latent Phase-1 `array<float3>`/`array<int3>` stride
bug (never exercised — no fixture used a 3-lane buffer). Deliberate divergence from dasSpirv,
which strides vec3 SSBO elements at std430's 16: Metal's contract here is "das layout, exactly".
Loads wrap back into the plain type (`float3(pf[i])` — packed types have no arithmetic of their
own), whole-element stores emit raw (packed = plain assignment is implicit), and anything
reaching INTO a packed element (component/swizzle/compound-assign) fails closed. Emitter breadth:
`ExprConstFloat16` → `h`-suffix literals; **folded 32-bit vector constants** (`int4(100)` with
const args folds to `ExprConstInt4` at infer — a Phase-2 blind spot no fixture had hit) emit as
ctor spellings; the ctor/convert/saturating-narrow call family (call name == das spelling of the
result type) — widen `int4(byte4)`, truncating narrow `byte4(int4)` (MSL conversion-ctor
C-truncation matches das), splat `half3(scalar)`, fp16 converts `half2(float2)`/`float2(half2)`,
and `*_sat` → `char4(clamp(v, -128, 127))`-style clamp+convert; fp16 closed arithmetic and the
f16 broadcast ride the existing operator paths via lattice-aware census classes
({f,i,u}{32,16,8}[v]). Census 143 → 178 kinds. Tests: `tests/msl` 48 → 57 (three fixtures —
HalfArith / LatticeConvert / PackedVec3 — shape asserts + goldens + census both directions;
fail-closed grew `_fc_lattice_wide` (@ssbo half8) + `_fc_lattice_wide_local` (short8 local) +
`_fc_packed_partial` (component store into packed)); `tests/metal` 20 → 26 on M1 Max, all
BIT-EXACT (no tolerance anywhere): latconv (int-lattice widen/narrow/sat + fp16↔f32 converts,
default fastmath — conversions and single ops are deterministic) and packed3 (byte3/half3/
float3/int3 das-stride round-trips + packed uniform + `constant half&` scalar-f16 uniform, with
float ops chosen exact — ×2.0 power-of-two scale — so even fma contraction cannot diverge).
**Measured Metal fact that shaped the fp16 gate:** with `fastmath=false` (MTLMathModeSafe), Metal
still keeps fp-contraction/mixed-precision freedom across multi-op chains — a 4-op half4 chain
drifted 1–64 ULP off per-op RNE on M1 Max — but SINGLE ops are correctly rounded,
**including half division**. So the halfarith GPU kernel stores one op per buffer (add/sub/mul/
div + an exact-by-construction scalar chain + compare flags), pinning every rounding point, and
matches the das CPU replay (promote-to-f32-round-back == per-op RNE) bit-exactly. Green: interp +
`-jit` + static binary on both suites; MCP lint + CI lint clean. New emitter surface as a side
effect: classic 32-bit vector lane/splat/convert ctors now lower too (Phase 2 had none).

**2026-07-11 — Phase 5 landed: simdgroup_matrix + host plumbing + the dasLLAMA offload
experiment.** Three legs. **(5a) simdgroup_matrix through the emitter** — the first STRUCT-typed
builtins: `simdgroup_float8x8`/`simdgroup_half8x8` in `metal_builtins.das` hold a row-major 8x8
reference tile; every op (`make_filled_*`, `simdgroup_load`/`_store` with runtime offset+stride,
`simdgroup_multiply`, `simdgroup_multiply_accumulate` — all-f32, all-f16, and the MIXED
f32-acc/f16-operand ggml combo, Metal-frontend-probe-verified on M1) is the FULL cooperative tile
op per call, so width-1 CPU replay stays idempotent and the same-body oracle works. Emitter:
sgmat locals lower with `make_filled` zero fills (`= {}` does not construct the opaque type);
loads/stores lower as member-pointer + offset (@ssbo device both ways, @workgroup threadgroup as
load source); the load/store das overloads take an untyped buffer param, so their call names
arrive as GENERIC instances (`` __::module`name`hash ``) — new `call_base_name` strips the
qualifier + hash at every call-name site; `simdgroup_store` marks its buffer written in the
write-set pre-scan. Fail-closed: `.data` access in a kernel, @ssbo of tiles, non-member load
source, threadgroup store. Census 178→192. tests/msl 65/65 (SgMatTile + SgMatTiled fixtures);
tests/metal 32/32 on M1 Max — single-tile f32+f16 vs sequential replay AND the
threadgroup-staged mixed-mac tiled GEMM vs a direct CPU matmul, all BIT-EXACT
(exact-by-construction integer values). **(5b) host plumbing, zero new externs** —
`MetalPipelineCache` (hit allocates nothing), `MetalBufferPool` (pow2 buckets; re-acquire reuses),
`with_compute_encoder` (N dispatches, one commit+wait; default hazard tracking orders them) —
all proven through the live-object counter in `test_metal_plumbing.das`. MTLSharedEvent +
MTLBinaryArchive DEFERRED by measurement: prefill offload is ~112 synchronous dispatches per
multi-second prefill (~30 ms round-trip tax) and the das-side cache covers the kernel count.
**(5c) the dasLLAMA offload experiment** — `dasllama/dasllama_math_metal.das` (guarded
`require ?das_metal`, non-Apple lanes never see it): a 32x32-C-block kernel (128 threads = 4
simdgroups, per-32-k in-kernel dequant of Q8 weights+activations into f16 threadgroup tiles with
scales folded, mixed 8x8 macs, f32 out) + a self-contained lazy driver (weight regions upload
once keyed by base address; pooled padded activation/output buffers — the kernel has no edge
masking, pads are zeroed) registered as a BATCH-ONLY `KernelBackend("metal")` at priority -1
(reachable only via `select_batch_backend`; small-ntok/odd shapes delegate to the portable CPU
kernel through new `kernel_backend_batch_fn`). Recipe: `DASLLAMA_PIN_BACKEND=arm64-sdot` (the
donor needs the row-major world) + `DASLLAMA_WSCALE_F16=0` (the override swaps only the f32
batch slot) + `DASLLAMA_PIN_BATCH_BACKEND=metal`. Gates: `modules/dasLLAMA/tests/test_metal_gemm.das`
(pow2 scales = BIT-EXACT vs portable — int8 quants and pow2 products are exact in half; arbitrary
scales = the f16-dequant envelope, bounded 2e-3 × the dot-magnitude sum, NOT |y| — cancellation
makes |y|-relative bounds dishonest); dasLLAMA suite 368 tests dormant-green; 40/40 greedy
token parity vs the CPU control with the GPU engaged (informational — tolerance path).
**Clean measurement round (M1 Max, Parsec OFF, interleaved 3-rep A/B, 2026-07-11):** iso
ntok=512 llama1b shapes — GPU 1481–1992 GMAC/s (q 1505 / w13 1817 / w2 1784 / cls 1992 ≈ 4
TFLOPS f16-effective) vs the tuned arm64-gen CPU tier's 884–1001 = **1.5–2.0x on every big
shape** (small kv 2048x512 at parity — per-region upload amortization). End-to-end
`prefill_perf` 1B Q8: **N=512 1054–1055 vs CPU 757–768 tok/s (+37% best-vs-best, 2 of 3 reps
within 0.1%)**; N=256 parity (792 vs 786 — the e2e crossover sits between 256 and 512 on 1B);
N=64 is a harness artifact (the one-time pipeline compile + full lazy weight upload lands in
each process's first measurement). Anchors: llama.cpp CPU pp512 813 t/s (das CPU 768 ✓ sane
window); **llama.cpp full-Metal pp512 3960 t/s** — 3.75x our hybrid, and the honest headroom
map: their forward is fully GPU-resident (attention + norms + no per-GEMM round-trips) near the
M1 Max roofline, while this experiment pays a synchronous submit + activation/output memcpy per
projection GEMM and runs attention on the CPU. Ledger from the round: per-layer encoder batching
/ GPU-resident activations (the residency gap is the big lever), kernel iteration
(double-buffered staging, tile shapes — ~2x kernel-level headroom to roofline), min_ntok
threshold per box/model (crossover ~256–512 on 1B). das-side findings recorded: `select_batch_backend("")` clears the pin but the slot
restores only at the next whole-backend activation (tests must pin "portable" explicitly for
oracle runs); the pointer-keyed weight cache assumes load-once-stable weights (refilling the
same arrays needs `metal_gemm_shutdown` — the documented cache contract).

**2026-07-11 — Phase 6 landed (6.0–6d): full-GPU-resident 1B prefill.** Four legs in three
commits (`f9c849107` 6.0+6a, `7664e6e6c` 6b, `df01a9366` 6c+6d + tiled attention).
**(6.0 detector)** `metal_command_buffer_gpu_start_time/_end_time` externs + `with_compute_
encoder_timed` — the GPU-busy vs host-wall split that steered everything below. **(6a emitter
math)** value-call whitelist exp/sqrt/rsqrt/sin/cos/tanh/abs/saturate/round (float classes; abs
also signed-int) + min/max (numeric) — das math-module names ARE the MSL spellings (`round` is
half-away on both sides, probe-verified); + the quantize-kernel converts cvt.f32.i8/cvt.i8.f32.
Census 192→221; msl 70/70, metal 34/34 (math fixture pins fastmath=false — fast::sin/cos's
~2^-11 ABSOLUTE envelope swamps the 1e-4 oracle). **(6b kernels)** `dasllama_metal_prefill.das`:
metal_q8_quant / metal_rmsnorm (simd_sum + tg partials) / metal_rope (table-driven NORM pairs —
build_rope_table already hoists all trig) / metal_swiglu / metal_add + attention; unit tests vs
the dasLLAMA CPU twins on-device (quantize bit-exact under planted ±127 amax). **(6c driver)**
ONE command buffer per prefill — 21 dispatches/layer, default hazard tracking orders, one
commit+wait; weights ride the 5c lazy region cache; pooled resident activations (+ per-layer
K/V outs, np32-padded per-head score slabs); readback hands x_b + roped-K/raw-V to
`kv_store_batch` (KV codec stays CPU — any dtype). **(6d seam + gates)** whole-prefill override
registry in dasllama_common (`PrefillOverrideFn` + `select_prefill_override`, dormant default;
hook in forward_prefill_body's layer loop; registration rides the dasllama_transformer umbrella
— a direct common require would cycle); `DASLLAMA_PIN_PREFILL` env rail; end-to-end parity test
(40-token greedy continuation GPU vs CPU control on the real 1B, token-for-token) + leak gate.
🔑 **The parity failure that mattered: ROW-MAJOR weights are the donor contract** — the default
M1 load picks a repack backend whose interleaved qblob the GPU kernel reads as garbage
(stage-probe-bisected: rmsnorm/quant exact, GEMM rel~2); the driver now declines via
`kernel_backend_needs_repack(active_kernel_backend())`, recipe pins arm64-sdot/portable.
**Attention iterated once by the detector's verdict:** knockout attribution
(`DASLLAMA_METAL_PREFILL_SKIP=attn|gemm|ew|nongemm`) showed the naive fused per-row pair at
~80 GMAC/s owning 44% of GPU time → replaced with a tiled sgmat trio (QK^T 32x32 C-blocks,
causal early-out → in-row softmax, tails zeroed → P·V whole-block GEMM) — attention now ~19 ms.
**Numbers (M1 Max, DIRTY, same-window interleaved): pp512 1594 tok/s** (hybrid clean 1054; lcpp
full-Metal 3960 = 2.5x away), N=256 1421. Honest GPU budget at N=512: 289 ms = GEMM chain 252
(497 GMAC — 0.97 GMAC/tok — at ~1970 GMAC/s, i.e. ALREADY at the Phase-5 iso kernel rate;
gemm-only knockout == full-chain delta, so inter-dispatch drains cost ~nothing) + attention 19
+ elementwise 18. **The remaining lever is the Q8 GEMM kernel itself** (~4 TFLOPS-f16-eff vs
the ~2x-to-roofline headroom the clean round mapped): double-buffered staging, bigger C tiles
per threadgroup, wider k-steps — iterate via bench_gemm_iso, not full prefills. Secondary
(measured small): concurrent-dispatch encoder + explicit barriers for q/k/v + w1/w3 overlap.
*(Done — see the 2026-07-12 Phase 6e entry: the winning levers turned out to be run-staging +
64x64, and the intuition list above all lost individually.)*

**2026-07-11 — pipeline: fixup-set global inits now infer; dasSpirv blob fill patch→fixup**
(rides this branch — surfaced reviewing dasMetal's apply/fixup model). Boris called dasSpirv's
patch+astChanged blob fill a workaround for a compiler gap; confirmed and fixed. Diagnosis
(both repro'd): `fixupAnnotations()` ran post-optimize and nothing inferred what fixup created —
a call-shaped init (`to_array_move` from the `array<uint>` literal) died with 50607; and a naive
re-infer at that old position re-tripped already-folded unsafe (31013 in daslib generics) because
`foldUnsafe` strips the wrappers — the exact ordering constraint the scope_free comment documents.
Fix (ast_parse.cpp): `fixupAnnotations()` moved to right after infer converges — BEFORE
lint/foldUnsafe/optimize — followed by a dirty re-infer gated on any global init left uninferred;
fixup output now flows through the whole back half like ordinary code. dasSpirv's blob+reflection
fill moved patch→fixup, astChanged dance deleted (also deletes one full-program re-infer restart
per shader-bearing module — those restarts marked EVERY function notInferred). The dasVulkan
backend body-fill hooks stay in patch (function bodies genuinely need the in-loop re-infer).
Consequence for annotation authors: fixup now sees PRE-optimize AST (dasMetal's emitter already
handles both shapes; dasGlsl goldens unchanged — 98/98). Regression test independent of dasSpirv:
`tests/language/test_fixup_global_init.das` + `_fixup_init_macro.das`. Green: tests/spirv 238/238
(generation in fixup), glsl, msl, metal, language — dynamic + static binaries.

**2026-07-12 — Phase 6e: Q8 GEMM kernel iteration — pp512 1594 → 2398 tok/s (DIRTY).** The
promised kernel round, run entirely in a new in-process lab
(`modules/dasLLAMA/benchmarks/matmul/bench_metal_gemm_kernels.das`): candidate `[metal_kernel]`
variants head-to-head against the production MSL in ONE process, GPU-timestamped
(`with_compute_encoder_timed`), interleaved best-of-N rounds, bit-exact-by-construction data
(quants in [-8,7], pow2 scales ⇒ every f32 partial sum exact ⇒ any accumulation order gives
identical bits; each variant compares bit-exact vs v0, v0 vs a CPU reference). **The knockout
pair re-aimed the whole round:** staging-removed vs math-removed timing attributed **55% of
kernel time to dequant staging** (per-element scale reloads + scalar loads), NOT to tile loads
(tile-major and pad-36 layouts ≈ neutral ⇒ no meaningful bank-conflict tax) and NOT to barriers
(~2%). Every intuition variant tried FIRST lost: bigger C tiles (0.75–0.95x), double buffering
(0.86–0.99x), k=64 (0.51x), int4-shift-unpack (worse than byte4), register prefetch (worse),
256 threads (worse) — on M1 Max occupancy is brutally sensitive to threadgroup-memory growth
*while staging is expensive*. The two changes that compose into the win: **run-staging** (one
thread = one contiguous 16-element run = ONE scale load + four `byte4` device loads, +40%) and
THEN the **64x64 C block** (halves both device re-read directions, 16 macs per 8 tile loads —
the tile-size rematch only wins once staging is cheap: +64% total, 3390–3650 GMAC/s vs the old
kernel's 2090–2200; math-only ceiling ~4900). **Shipped as a two-kernel dispatch** in
dasllama_math_metal: `metal_q8_gemm` (32x32, run-staged vec4) + `metal_q8_gemm64` (64x64),
`gemm_use64(mp, d)` picks 64 when `d % 64 == 0` and the grid is ≥128 threadgroups (below that
the 32-kernel's occupancy wins — the kv 2048x512 shape); both drivers (5c batch donor + 6c
resident prefill) pad M to 64 (prefill's mp doubles as attention np32 — its kernels only assume
%32; pad rows stay garbage-by-design, contained by GEMM row independence). Quant buffers are
`array<byte4>` VIEWS in the kernel classes only — host buffers unchanged. **End-to-end (M1 Max,
DIRTY): pp512 2398 tok/s (was 1594; lcpp full-Metal 3960 = 1.65x away, was 2.5x), N=256 2229
(was 1421); N=512 GPU-busy 289 → 188 ms** — matches the iso ratio, GEMM chain ~151 ms of it.
Donor-path iso (through the synchronous submit+memcpy tax): big shapes 2072–3112 GMAC/s
(+40–56%); kv stays donor-tax-bound at ~1000. Gates green: test_metal_gemm (extended with a
D=8192 case so BOTH dispatch paths get the bit-exact compare), prefill kernel units, 40-token
greedy 1B parity, leak gates, full dasLLAMA suite. das gotcha hit: the MSL emitter flattens
bare `{ }` blocks (locals collide in one MSL scope — rename or use a real `if` scope).
Emitter-extension ideas the lab priced but did not take (ledger): `half4` threadgroup stores +
a `simdgroup_load` transpose-flag overload (each ~<10% at high risk). Remaining from here:
the clean measurement round (Parsec OFF) + concurrent-dispatch encoder (secondary, measured
small), then PR.

**2026-07-12 — Phase 6e clean measurement round (M1 Max, Parsec OFF, interleaved 3-rep A/B).**
Kernel-level (lab, 5 interleaved rounds, bit-exact): 32-kernel kv 2737 / q 2928 / w13 3003 /
w2 2958 / cls 3024 GMAC/s; 64-kernel q 3344 / w13 3594 / w2 3393 / cls 3646 (kv correctly stays
on the 32-kernel per gemm_use64). End-to-end prefill_perf 1B Q8, GPU-resident vs the box-tuned
CPU control (arm64-gen), best-of-3: **N=512 2395 vs 823 tok/s = 2.91x; N=256 2003 vs 849 =
2.36x**; N=64 336 vs 798 — CPU keeps small batches (each fresh process pays the one-time PSO
compile + lazy weight upload there; min_npos=64 default unchanged since a served process pays
it once). Anchors (stored from the 2026-07-11 clean window, per the no-baseline-reprofiling
rule): llama.cpp CPU pp512 813 (das CPU 823 ✓ sane window); **llama.cpp full-Metal pp512 3960 —
das GPU-resident is now 1.65x away** (hybrid clean was 3.75x, resident-with-old-kernel dirty
2.5x). Clean best == the dirty single-shot (2395 vs 2398) — the box was quiet.

**2026-07-13 — Phase 7 (session 1): llama-3B unlock + the overhead hunt — pp512 111 (CPU
fallback) -> ~880-960 tok/s DIRTY; lcpp full-Metal 3B = 1402 (1.5-1.6x away).** The 3B
(dim 3072, hs 128, 28L) now serves: MetalAttnQK walks head_size in 64-wide k-slabs through the
same 32x64 staging tiles (hs=64 one slab bit-identical, 128 two; gate's hs<=64 dropped;
hs=128 unit arm + 1B parity green). Overhead work, measured by the new encode-vs-gpu split
(with_compute_encoder_timed 4-out overload; encode is ~0.15ms = FREE): (1) weight region
buffers HAZARD-UNTRACKED (new metal_new_buffer_untracked extern) — the scheduler's
per-resource analysis over ~450 tracked weights was the dominant slack, 72 -> 32ms on its
first read (876 -> 962 t/s), though later reps put the slack band at ~50-70ms (box wobble;
re-read on a clean window); (2) uniforms + rope tables moved to an UNTRACKED pool
(pool_acquire_untracked) — measured NO-OP on top of (1); the residual commit-to-done slack is
NOT per-resource analysis; (3) fused dispatch set 21 -> 15/layer (metal_add_rms_quant folds
the pending residual add into the next norm with a threadgroup row stage — dim <= 3072 gates
it; metal_swiglu_quant recomputes silu on the quant pass, gate never written back;
metal_rope_qk one grid) — interleaved ABBA says NEUTRAL (the "ew = launch gaps" hypothesis was
WRONG — Metal pipelines back-to-back dispatches; the ew bucket is real kernel time and the
fused rms+quant's 12KB tg stage gives the saving back in occupancy); kept for the smaller
graph; DASLLAMA_METAL_FUSE=0 is the A/B rail. GEMM kernel round 2 (lab, 3B shapes kv3b/q3b/
w13_3b/w2_3b added): **three structural variants ALL LOST to v13** — v18 32-elem runs (-1%),
v19 row-major-B + transposed sgload + tg_store_half4 (-1.3%; the two NEW emitter constructs —
simdgroup_load transpose-flag overload and tg_store_half4 — are GPU-validated bit-exact and
stay as surface), v20 device-direct f16 B panels (-6-9%; strided transposed device loads beat
by staging). v13 (run-staging + 64x64) holds a ~3480-3590 GMAC/s plateau vs the staging-
removed ceiling 4600-4950 at 3B shapes; lcpp's implied kernel rate ~4400-4500 (from their own
1B/3B walls). 3B GPU budget: gemm ~415ms, attn ~25, ew ~28; slack ~50-70; readback ~14.
**Parity ledger (needs BOTH):** kernel 3590 -> ~4400 (next ideas: faithful ggml
kernel_mul_mm geometry port — 64x32, 2x4 sg layout, their exact staging shape — as a lab
variant; simdgroup async device->tg copies; f16-accumulate risk analysis) AND the ~90ms
non-GEMM tail -> ~35 (next: commit-to-done slack attribution beyond resource tracking —
retainedReferences=NO, split command buffers, concurrent encoder + coarse barriers; attention
f16; readback overlap). (Labeled DIRTY when logged; Boris 2026-07-13: Parsec was OFF for
these windows — treat the numbers as clean. The ~50-70ms slack band variance is real
box-side variance, not Parsec.)

**2026-07-13 — Phase 7 (session 2): llama.cpp's kernel VERBATIM in the lab — their geometry is
worth +16-21% over our best, measured head-to-head.** `kernel_mul_mm_q8_0_f32` (classic
simdgroup path, ggml @ ebd048f) now runs as lab variant `lcpp_mulmm` — byte-verbatim MSL
(`benchmarks/matmul/_lcpp_mul_mm_q8.das`, function constants baked to their same-shape
specialization), on their own production contract: interleaved block_q8_0 blobs, RAW F32
activations (4x our activation bytes — they still win), kargs struct, 6144 B dynamic tg
scratch via new `metal_set_threadgroup_memory_length` extern. Bit-exact vs v0 on all 9 shapes.
M1 Max, ntok=512, CLEAN window (Parsec off — Boris), interleaved best-of-3 (GMAC/s):

| shape | v0_prod32 | v0b_prod64 | lcpp_mulmm | lcpp vs our best |
|---|---|---|---|---|
| kv 2048x512 | 2742 | 2516 | 3330 | 1.21x |
| q 2048x2048 | 2932 | 3345 | 4001 | 1.20x |
| w13 2048x8192 | 3004 | 3595 | 4180 | 1.16x |
| w2 8192x2048 | 2958 | 3393 | 4074 | 1.20x |
| cls 2048x128256 | 3024 | 3648 | 4245 | 1.16x |
| kv3b 3072x1024 | 2854 | 3220 | 3884 | 1.21x |
| q3b 3072x3072 | 2969 | 3463 | 4105 | 1.19x |
| w13_3b 3072x8192 | 3008 | 3573 | 4199 | 1.18x |
| w2_3b 8192x3072 | 2984 | 3485 | 4134 | 1.19x |

These are clean numbers: lcpp's real kernel rate on M1 Max is ~3900-4250 GMAC/s — slightly
BELOW the ~4400-4500 implied-from-their-walls estimate, so the kernel gap to close is
1.16-1.21x, not 1.25x+. Settles the Phase-7 kernel question: the gap is their 64x32 tile +
2x4 sg layout + tile-major 8x8
shared blocks + dequant-to-registers-then-scatter staging, not their data format (they pay
MORE device traffic). Next lever: port that geometry faithfully as a das lab variant (the
masterplan's step-2 idea, now de-risked — the win is proven in OUR harness, not inferred from
their walls). Repro: `DASMETAL_LAB_VARIANTS=v0_prod32,v0b_prod64,lcpp_mulmm bin/daslang -jit
dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_metal_gemm_kernels.das`.

**2026-07-13 — Phase 7 (session 2b): the ggml geometry port (v21) + the attribution ledger —
+19% over v0_prod32, beats BOTH production kernels, still 13-19% behind verbatim.**
`v21_ggmlgeo` (+`v21sct` scale-transpose flavor): ggml's exact geometry on our planar contract
— 64-output x 32-token tile, tile-major 8x8 shared blocks (every sgload = one contiguous
64-half run), W transposed within tiles at staging, byte4 device loads to registers before the
pre-staging barrier, fully-unrolled math (4 slabs x 6 loads + 8 macs), 2D grid tokens-fast,
tg_store_half4 B stores. Bit-exact. CLEAN M1 Max (GMAC/s): kv 2948 / q 3474 / w13 3633 / w2
3512 / cls 3684 / kv3b 3268 / q3b 3555 / w13_3b 3639 / w2_3b 3577 — beats v0b_prod64 on every
shape (+1-4% big, and it WINS the kv shapes where v0b loses to v0_prod32, so one kernel could
replace both + the gemm_use64 heuristic). Verbatim lcpp still +13-19% ahead. **Attribution
(q3b, knockouts + string-surgery on the verbatim source):** total gap 0.186 ms/dispatch =
math-loop codegen ~0.045 (their staging-stripped ceiling 4936 vs v21's 4719 GMAC/s) + staging
~0.144 (theirs 0.197 vs ours 0.341). Within staging: dequant multiplies + dependency chain
~0.076 (scales-baked-to-1.0 probe), B int-dequant ~0.024 (f32-activation-panel probe v21f32x:
their B contract reads raw f32 — no dequant at all), residual staging codegen ~0.044. RULED
OUT by measurement: simdgroup_barrier(mem_none) hints (lcpp_nosgbar identical), rasterization
order (2D grid no change), scale-stream cacheline scatter (v21sct [block][row] transpose
+0.4%). **Upshot: the remaining lcpp edge is their DATA CONTRACT (f32 activations, inline
scales) plus better-scheduled staging/math MSL — no single structural lever left on the planar
format.** Production fork to decide: (A) adopt their contract for GPU prefill — one-time W
repack to interleaved blobs at region-cache upload + feed f32 activations directly (also
deletes the X-quant dispatches from the graph) → the lab-proven 4000-4250 rate; (B) ship v21
as the planar harvest (+2-4% big shapes, kv fix, kernel unification) and put the effort into
the ~90ms non-GEMM tail; (C) f32-X feed only (cheap, +2%, kills quant dispatches). All probe
variants live in the lab (lcpp_nosgbar/lcpp_nostage/vk21_*/v21f32x/v21sct).

**2026-07-13 — Phase 7 (session 2c): Option A SHIPPED — verbatim mul_mm + blob repack + f32-X
in the resident prefill. 3B pp512 850 -> 1053 t/s (+24%); gap to lcpp 1402 now 1.33x.** Boris
picked (A). First tried the das-native blob kernel (lab v22_blob36: das-friendly 36B blocks,
f32 scale + byte4-aligned quants, two typed views of one buffer) — 3623 GMAC/s, only +2.3%
over v21: the residual ~13% vs verbatim is emitter/MSL codegen, not format. So production runs
the VERBATIM kernel: `dasllama/dasllama_lcpp_mulmm.das` (moved from the bench fixture, in
.das_module), compiled alongside the kernel set. Driver (dasllama_metal_prefill.das):
`upload_blob_region` lazily repacks each (quants, scales) weight region into resident
block_q8_0 blobs — one-time CPU pass, 235ms 1B / 686ms 3B, f32->f16 scale exact for
GGUF-q8_0-sourced weights; `uniform_mm_args` pooled 88-byte kargs images; `enc_gemm_lcpp`
(kargs/blob/f32-X/dst, 6144B dynamic tg scratch, grid (mp/32, d/64) x (32,4,1)); mode gate
`lcppmm` = default when every GEMM output dim % 64 == 0 (bc_out=false specialization), rails:
`DASLLAMA_METAL_MULMM=0` env + `set_metal_prefill_mulmm_legacy()` setter. The lcpp path runs
the UNFUSED elementwise set minus ALL X-quant dispatches (17/layer; rms/swiglu feed f32
straight to mul_mm). Legacy quantized path fully preserved (donor/batch path untouched).
Gates: mulmm_gate unit (verbatim PSO on exact-arithmetic blob W x f32 X vs CPU ref, BIT-exact,
2 shapes — also the kargs layout oracle), parity test now runs BOTH modes — 40-token greedy
matches the CPU control EXACTLY on the f32-X path too (counting-prompt family robust as
designed), leak gates green. E2E A/B (clean, same binary, 3B): N=256 748.9 -> 899.4 t/s,
N=512 849.8 -> 1053.0 t/s (GPU 508 -> 396 ms, -22%); 1B parity-run GPU 150.9 -> 116.9 ms
(-22.5%). NEXT: the ~80ms non-GEMM tail (slack attribution / split command buffers /
retainedReferences; f16 attention; readback overlap) — at gpu=396ms the tail is now ~17% of
total; then the das-emitter codegen chase to retire the verbatim kernel (ledger: math-loop
scheduling ~4.5%, staging codegen ~0.04ms/dispatch).

**2026-07-13 — Phase 7 (session 2d): the tail is DEAD — chunked command buffers + interleaved
KV readback. 3B pp512 1053 -> 1219.6 t/s (+16%); gap to lcpp 1402 now 1.15x.**
metal_prefill_forward now splits the layer loop across N command buffers (default ~4
layers/chunk = 7 on the 3B; DASLLAMA_METAL_NCB overrides, =1 restores single-buffer), each
committed the moment it finishes encoding — the scheduler analyzes chunk k while chunk k-1
executes (same-queue commit order + hazard tracking on the activation buffers carries
cross-chunk ordering; untracked weights/uniforms are CPU-written before their chunk commits).
Measured (3B pp512): slack 57ms -> 9ms, 1069 -> 1176 t/s. Then completion+readback
interleave: wait per chunk in commit order, stream that chunk's K/V rows into the CPU KV
codec while later chunks run — the 13ms store cost disappears inside the GPU window; residual
stream copies after the last chunk. 1176 -> 1219.6 t/s; total-minus-GPU-window is now ~11ms
(was ~80). Bonus: the one-time blob repack ALSO overlaps (encode of later chunks proceeds
while earlier chunks execute — 1B first-prefill total 277ms w/ 242ms repack inside), and the
legacy q8 path gains equally (1B GPU-run total 243 -> 173ms). RULED OUT by measurement:
commandBufferWithUnretainedReferences (new extern metal_new_command_buffer_unretained, rail
DASLLAMA_METAL_UNRETAINED=1) — neutral at every chunk count, kept as surface. Gates: parity
BOTH modes green on the chunked driver, mulmm unit, donor gemm, leak gates. **Remaining vs
lcpp 1402: the 398ms GPU window itself** — gemm ~343 (verbatim-rate; emitter chase is the
lever), attn ~25 (f16 candidate), ew ~28.

**2026-07-13 — Phase 7 (session 3): ggml-geometry attention pair — 3B pp512 1220 -> 1251 t/s;
we now beat llama.cpp's fa=0 wall.** Window re-attribution first (new attn_qk/attn_sm/attn_av
sub-skip rails): gemm ~350 / attn 28 (QK 14.2 + AV 10.2 + softmax 3.2) / ew only ~9 (the
quant deletions already took the old 28ms bucket). llama-bench 3B: fa=0 1350.5, fa=1 1391.4
— their OWN flash attention nets +3%, and their fa=0 attention is mul_mm + masked softmax =
exactly our structure, so the trio's kernels were the outlier (32x32 C tiles, scalar f32
staging, the pre-run-staging style — ~60x below mul_mm rate). Rewrote QK/AV with the v21
lessons (MetalAttnQKMm/MetalAttnAVMm): 32x64 C tiles, tile-major 8x8 shared blocks staged
HALF via float4 loads (lcpp's f32_f32 mul_mm stages half from f32 too), 2x4 sg quadrants,
unrolled math, per-head z grid, QK causal block early-exit, AV causal k-limit, V pad rows
zeroed at staging (0*NaN guard); K pads need NO guard (their scores land in columns the
causal softmax zeroes). Softmax unchanged. Default at head_size % 64 == 0; rail
DASLLAMA_METAL_ATTN=0 pins the trio (also serves other hs). Attention 28 -> 14.2ms
(QK 5.2 + AV 6.3 + softmax 3.5); GPU window ~374ms; our wall 376ms vs their fa=0 379ms.
Gates: causal-GQA oracle hs64-GQA2 + hs128-GQA3 within the half-staging envelope (2e-2 —
the honest envelope of half-staged scores through softmax, same as lcpp's), parity 40/40
exact both modes. Framing re-measured same-run: ~10ms (logits 3.7 + embed 0.7 + alloc ~5),
NOT the 33ms cross-run wobble suggested. GEMM chain check: 1.44 TMAC / 343ms = 4190 GMAC/s =
AT the verbatim kernel's lab rate — that bucket is done. **NEXT (the one big lever left):
verbatim lcpp kernel_flash_attn_ext port — collapses QK+softmax+AV (~14 -> ~6ms, +2.5-3%)
AND kills the heads x np32^2 score slab (the g_max_npos=2048 cap). Needs their KV-dtype
contract read first (f16 K/V vs our f32 buffers). Micro ledger: softmax restructure ~2ms,
logits-on-GPU ~3ms, prefill alloc caching ~5ms.**

**2026-07-13 — Phase 7 (session 4): verbatim flash-attn ported — CORRECT but LOSES to the trio,
kept as an opt-in rail.** New module `dasllama/dasllama_lcpp_flash_attn.das` = `kernel_flash_attn_ext`
+ `_blk` brought VERBATIM (byte round-trip-diffed; 33 KB MSL das-escaped) with three mechanical
deviations (baked `constant` function-constants for our fixed specialization: causal mask, no
bias/sinks/softcap, has_kvpad=false, nsg=4; NS10/NS20 from args so one entry pair serves every
kv_dim; f32 AND f16 KV entries for dk64/dk128), plus one non-lcpp `metal_fa_causal_mask` kernel
(ggml builds KQ_mask host-side). KEY layout fact: lcpp's KV-pad kernel assumes ggml's per-head-
contiguous KV (nb11=head_size); OURS is interleaved [token][kv_head][d] (nb11=kv_dim), so the pad
kernel is unusable — instead ne11=mp (64-padded => has_kvpad=false) + the causal mask covers the pad
columns [npos,mp). V pad rows are safe (the mul_mm GEMM writes ALL mp rows finite => 0*finite=0). The
flash impl is otherwise stride-generic so our strides drop straight into nb11/nb12/ns10; GQA via
ne_12_2=n_kv_heads; ne1=n_heads lands dst in our [token][head][d] bxb. CORRECTNESS (both KV dtypes):
1B parity exact vs CPU control; 3B (hs128) token-for-token vs the trio. **PERF (2 clean Parsec-off
same-session A/B, flash-MINUS-trio delta rock-consistent): flash attention costs ~3ms MORE than the
ggml-geometry trio — f32 KV +3.3ms (13.9 vs 10.6), f16 KV +3.1ms (17.4 vs 14.3).** f16 KV (lcpp's
contract, via a per-layer f32->f16 pack) did NOT help — the pack overhead (56 dispatches + KV
bandwidth) cancels the f16 kernel's bandwidth saving. WHY flash loses: the trio already runs QK/AV as
mul_mm-rate GEMMs (session 3, 4190 GMAC/s) + causal skips, so the score-slab device round-trip flash
eliminates is NOT the bottleneck (M1 Max bandwidth ample); meanwhile flash RE-READS K/V once per
query block (~32x after the causal skip vs the trio's 1x), costing more than the fusion saves. lcpp's
fa=1 +3% over fa=0 comes from their WHOLE f16 pipeline, not graftable via a KV-only pack. DECISION
(Boris): keep the trio as default; flash is an OPT-IN rail (`DASLLAMA_METAL_FLASH=1`, KV dtype via
`DASLLAMA_METAL_FLASH_KV=f16|f32`) with lazy pipeline compile (default never pays the 33 KB compile).
Residual flash value = drops the heads x mp^2 score slab (memory) => a future lever to lift
g_max_npos for long-context prefill. **NEXT: das-emitter codegen chase (retire the verbatim mul_mm),
logits-on-GPU ~3ms, prefill-alloc caching ~5ms. Then PR the arc (preflight --full).**

**2026-07-13 — Phase 7 (session 5): the emitter codegen chase — das kernel from 87% to ~98% of
verbatim; the gap was REGISTER PRESSURE from frontend-unrolled statements.** Method: AIR-level
IR diff (new Metal offline toolchain, `xcrun metal -S -emit-llvm`) + the new occupancy
introspection externs (`metal_pipeline_max_total_threads` / `_thread_execution_width`, printed
per variant by the lab) + hand-MSL morph probes bisected one spelling at a time. ROOT CAUSE:
the das emitter's hand-unrolled statement blocks reach LLVM as one giant SSA body; LLVM hoists
every loop-invariant tile address (~42 pointers: 16 A-store + 2 B-store + 24 sgmat-load) into
long-lived registers -> max_threads 1024 -> 832 (an occupancy tier) -> flat -13% vs the verbatim
lcpp kernel on every shape. llama.cpp's kernel keeps its loops ROLLED with
`_Pragma("clang loop unroll(full)")` + two bumped tile POINTERS; the AGX backend unrolls late,
register-budget-aware. Bisect verdicts (all on the 34B-blob contract, bit-exact, interleaved):
34B-blob/byte loads on the OLD unrolled shape = neutral (v23); rolled math = +10-11% and 1024
back (v24_roll); rolled A-staging = neutral; device-pointer k-loop bumps = -1%; vectorized
half2x4 B move = -1%; ushort prologue = -1%; int-vs-short loop vars = FREE; index-form tile
addressing (`aB + ik*512`) REGRESSES to 896/-12% even inside pragma'd loops (v24h1, kept in lab
as the negative control) — the load operand must be a loop-carried POINTER; A-staging via a
hoisted device pointer = the last +1.5%. SHIPPED (emitter + builtins, pure das, no C++ rebuild
for the das half): `unroll_range(n)` (rolled-with-pragma for; const bound), fixed-array locals
(scalar `half va[16] = {};` + sgmat arrays w/ pragma'd zero-fill loops), indexed sgmat args
(`mc[i]`), `tg_cursor(member, base)` / `dev_cursor(member, base)` (CPU = plain uint offset;
MSL = threadgroup/device const pointer local; sgmat-load offsets and element reads led by a
cursor lower through the pointer; advance = `cur += N`). New dasMetal externs (dasMetal.mm):
pipeline occupancy introspection (the register-pressure oracle — a reg-limited PSO reports
max_threads < 1024). Tests: tests/msl census +8 tags + SgMatRolled fixture + golden (73/73);
tests/metal test_metal_sgmat_rolled GPU-vs-CPU bit-exact (36/36). Lab: `v25_das_rolled` = the
das-AUTHORED kernel on the production 34B contract — q3b 4028 / w13_3b 4112 / w2_3b 4034 GMAC/s
= 97.6-98.6% of verbatim (4101/4201/4132), == the hand-MSL v24g reference, +11-12% over v22.
One-shot morph probes pruned post-verdict; v22/v23/v24_roll/v24g/v24h1/v25 kept as the
reproducible A/B family. PRODUCTION CALL (for PR review): verbatim stays the default GEMM —
the last ~2% is AGX-backend scheduling even hand-MSL in the same shape can't reach, and ~2%
GEMM ~= ~1% pp512 = the margin by which das beats lcpp fa=0. The das v25 kernel + constructs
are the retirement PATH once the last 2% closes (ISA-level chase, priced separately).
**NEXT: logits-on-GPU ~3ms, prefill-alloc caching ~5ms. Then PR the arc (preflight --full).**

**2026-07-13 — Phase 7 (session 5b): logits-on-GPU — 3B pp512 -5.5ms (interleaved, rock-steady
across 5 rounds: 377.3 vs 382.9 ms/prefill, ~+1.5%).** The final rmsnorm (one row, bx bound at
the last position's byte offset into the existing rms PSO) + a new classifier GEMV kernel
(`metal_q8_gemv`: one simdgroup per output row, lanes stride byte4s, 4-blocks-in-flight unroll —
the rolled x1 form measured only ~140 GB/s; per-byte4 scale product, simd_sum reduce, row < ddim
tail guard) ride the END of the last command buffer; s.logits reads back with the residual copy.
The saving EXCEEDS the ~3.7ms CPU logits it replaces because the GEMV overlaps the interleaved
KV-readback tail instead of serializing after the wait. Contract: the override calls the new
`prefill_override_logits_done()` (dasllama_common) after filling s.logits; forward_prefill_body
then skips its CPU final-norm/classifier step. Gates mirror the CPU q8 `mm` branch only — softcap,
suppressed-token pins, non-q8 / tied-f32 classifiers decline to the CPU step (semantics preserved
by construction). Cls weights ride the existing upload_region cache (tied-q8 = the embedding
region, one-time). Rails: `DASLLAMA_METAL_LOGITS=0` env + `set_metal_prefill_logits_cpu()` (the
in-process A/B setter). Tests: gemv_gate x2 shapes in test_metal_prefill_kernels (exact-arithmetic
BIT-exact, incl. the guarded-tail grid), 1B parity token-for-token with GPU logits active, leak
gates green. Attribution rail: g_skip modes keep logits OFF so knockout deltas stay comparable.
**NEXT: prefill-alloc caching ~5ms. Then PR the arc (preflight --full).**

**2026-07-13 — Phase 7 (session 5c): prefill scratch alloc — framing 5.7ms -> 1.1ms.** The
"alloc 5ms" was forward_prefill_alloc's batched-scratch GROWTH cost on the first prefill at a
new npos (~70 MB across 10 arrays at 512 rows): das `resize` zero-fills the grown span AND the
realloc copies the old block. Both are pure waste here — the scratch is stage output, fully
written before any read. Fix = `overwrite_resize` (dasllama_common): grow-only, contents-
DISCARDING (`delete` on grow skips the realloc's old-block copy), `resize_no_init` (skips the
zero-fill). Measured (3B pp512, per-bucket profile): alloc 4969us -> 55us; embed +0.2ms
(absorbs the first-touch page faults); framing TOTAL 5728 -> 1075us. A new `alloc` prof bucket
makes the cost visible in every profile run. Exotic scratch (ple/deltanet/att_b/fa_*) stays on
zeroing resize — only the 10 always-fully-written arrays switch. Gate: full modules/dasLLAMA/tests
suite (model oracles token-for-token).
**NEXT: PR the arc — preflight --full + clean Parsec-off measurement round (announce, wait for go).**

**2026-07-13 — Phase 7 CLEAN MEASUREMENT ROUND (Parsec off, Boris-confirmed window, best-of-3
process-fresh prefill_perf runs + one interleaved lab sweep).** llama-3.2-3B pp512: **1300.5 t/s
best** (median 1256; GPU window 375.4-378.9 ms, tight; e2e spread is commit-wait slack 15-35 ms);
pp256 best 1256. Anchors: lcpp -ngl 99 fa=1 = 1402 (0.93x), fa=0 = ~1351 (0.96x) — GPU-window-vs-
GPU-window we hold the session-3 fa=0 win (375-379 vs their 379). llama-3.2-1B pp512: **3467 t/s
best** (median 3440; GPU 137.1-138.4 ms); pp256 best 3173; lcpp full-Metal anchor 3960 (0.88x) —
vs Phase-6's 1054 clean hybrid = 3.3x across Phase 7. Lab GEMM (GMAC/s, das v25 vs verbatim lcpp):
kv 3197/3326, q 3959/3999, w13 4099/4184, w2 4010/4069, cls 4161/4245, kv3b 3898/3890 (das WINS),
q3b 4028/4102, w13_3b 4115/4200, w2_3b 4047/4132 — the das-authored kernel at 96-100% of verbatim
everywhere, >= the hand-MSL v24g reference on most shapes. NOTE: 1B GPU window vs session 2c's
dirty 116.9 ms is not comparable — the chunked driver's window now spans first-chunk-start to
last-chunk-end (includes inter-chunk gaps) and sessions 3-5 added the attention pair + logits
dispatches; the t/s headline is the honest cross-session metric.

**2026-07-13 — Phase 7 REWORK step 1 (PR #3456 closed for redesign): `unroll_range` deleted,
loop hints ride das's existing `for [hint] (...)` annotation surface.** Boris's call: the
iterator-function spelling was a construct where the language already had the right one —
`ExprFor/ExprWhile.annotations` (the per-loop hint list the JIT lowers to !llvm.loop.*
metadata). The emitter now lowers the SAME vocabulary to clang loop pragmas: `[unroll]` ->
unroll(enable), `[unroll_full]` -> unroll(full), `[unroll_disable]` -> unroll(disable),
`[unroll_count=N]` -> unroll_count(N); unknown hints fail closed. Works on `for` and `while`.
Kernels/fixtures rewritten to `for [unroll_full] (i in range(N))`; emitted MSL is byte-identical
(msl goldens unchanged, 73/73). One spelling, per-backend lowering — CPU JIT gets LLVM metadata,
Metal gets pragmas, interp ignores. REMAINING REWORK (design session): tg_cursor/dev_cursor —
likely emitter-automatic LSR so natural source gets the pointer form; then das GEMM as default.

**2026-07-13 — Phase 7 REWORK step 2: @workgroup -> dynamic shmem lowering — the das GEMM now
BEATS verbatim llama.cpp on every shape.** The ISA rail (applegpu + MTLBinaryArchive harvest;
`compiler_explorer.py` flow, MCT_MAX_THREADS env patch) settled the residual ~2%: registers a
wash (r49/r50), heavy work identical, the whole gap = integer address materialization (40 vs 24
iadd/iter, 23 of ours inside the fmadd-bound math phase vs their 5). Nulls along the way:
pipeline-descriptor knobs (maxTotalThreads=128 + multiple-of-width — byte-identical ISA),
unsigned-vs-signed offsets (identical), loop-carried device cursors (44->40 only). ROOT CAUSE:
STATIC `threadgroup T name[N]` arrays — the AGX backend constant-folds carried tile pointers
into their absolute addresses and the large offsets exceed the load immediate field; an opaque
dynamic [[threadgroup(0)]] base keeps small carried offsets (loop 185 -> 156 instructions,
SHORTER than verbatim's 165). Emitter change: @workgroup members lower to ONE dynamic
threadgroup(0) param + derived pointers (scalars deref a derived pointer), name-sorted
16B-aligned layout, total published as the `_tgmem` companion; `run_compute_1d` takes it,
drivers pass it per dispatch. Every wg-bearing kernel inherits the codegen with zero source
changes. Lab (interleaved, bit-exact): das v25 4128-4292 GMAC/s vs verbatim 4002-4201 —
**+2.2-3.2% on all six big shapes**; emitted == hand morph exactly. Parity token-for-token.
**NEXT: swap the production GEMM default to the das kernel (retiring the 33KB verbatim MSL to a
rail), re-run the clean round, reopen the PR.**

**2026-07-13 — Phase 7 REWORK step 3 (Boris's call: "get rid of abstraction — das is das is
das"): tg_cursor/dev_cursor DELETED; the pointer form is spelled as a real das pointer.** The
post-shmem re-measure proved BOTH spellings still matter (index-form math = max_threads 896,
-10%, even on dynamic shmem), so the choice must stay visible in source. New spelling — plain
das: `var p = unsafe(addr(member[expr]))` declares a pointer into a kernel member (emitter
recognizes ExprRef2Ptr(ExprAt(member, idx)) by SHAPE, no magic names; address space from the
member: @workgroup -> `threadgroup T *`, read-only @ssbo -> `device const T *`, written ->
`device T *`); reads are das pointer indexing `unsafe(p[i])` (generic ExprAt emission);
advance is das pointer arithmetic `p = unsafe(p + n)` (the i_das_ptr_add intrinsic lowers to
element-scaled MSL pointer addition); a pointer is accepted as a simdgroup_load source (loads
only — pointer stores fail closed). CPU semantics are das's REAL pointer semantics — the
reference bodies walk actual memory (the simdgroup_load builtin bodies gained `unsafe()` on
their indexing for pointer instantiations). Index form stays the safe spelling; pointer form
is explicitly unsafe — exactly das's own contract. v25 + fixtures rewritten; emitted MSL and
rates identical to the cursor world (v25 4133/4277 GMAC/s == v26, > verbatim's 3996/4184).
Census: decl.ptr.tg/dev, op.ptr_add, call.sgmat_load_ptr.f16 (cursor tags gone).

**2026-07-13 — Phase 7 REWORK step 4: the das mul_mm IS the production GEMM; verbatim + flash
plumbing out of the driver; lessons swept across the kernel set (clean Parsec-off round).**
Production: new `MetalQ8MulMm` (`metal_q8_mulmm_msl`) — the v25 shape on the driver's existing
34B q8-block repack (GGUF q8_0's own layout), bindings blob x2 (half-scale/byte views) + f32 X
+ y + kdim/ndim; `enc_gemm_mm` replaces the verbatim dispatch at all five GEMM sites; the flash
opt-in rail (verbatim lcpp MSL) is DELETED from the driver along with the kargs plumbing (rail
findings preserved here as prose). The lab's v25 variant now references the production
companions (zero copy drift, like v0). Lessons sweep via the new
`benchmarks/matmul/occupancy_report.das` (permanent tool — run after any kernel change):
found attn_qk_mm/attn_av_mm at 704 (the hot pair!) and q8_gemm64 at 704, everything else 1024.
Rolled rework (matrix arrays + [unroll_full] + das-pointer tile bumps + QK staging via a
register array): attention pair 704 -> 896 (residual = f32 source values live across the
staging barrier; diminishing returns), gemm64 rolled but stays 704 — 16 accumulators = 32
regs/lane of C is that geometry's floor (off the resident default path; the batch donor's
call). CLEAN ROUND (best-of-3, process-fresh): 3B pp512 GPU window **367.5-372.0 ms** (was
375.4-378.9 pre-swap) — **matches llama.cpp's fa=1 best mode (368 ms) head-on**; e2e best
1299.3 t/s, median 1287.5 (spread = commit-wait slack). Parity token-for-token; all gates
green. NEXT: pre-PR scrub (delete both verbatim modules + lab lcpp variants, squash to one
fresh commit off origin/master, delete the old remote branch) -> final clean round -> reopen PR.

**2026-07-14 — Phase 8 (emission arc P3): GPU decode hits the llama.cpp bar — 3B tg128
87.1-87.3 t/s (clean Parsec-off round, best-of-3 process-fresh, ≥ lcpp Metal's 87.0).**
Deep row: ctx 512 -> 640 at 80.4-80.7 t/s. Protocol: decode_prof, DASLLAMA_PIN_DECODE=metal +
portable backend, wscale-f16 1, f16 KV, -p 8 -n 128. From P2's 69.9/46.3 baseline via the P3
chase (attribution harness `history/dasLLAMA/benchmarks/decode_metal_chase.das`, knockout arms + per-stage
driver split): (1) coalesced simdgroup-cooperative attention scoring + hoisted q + 4-row-ILP V
pass + 64-row chunked split-K above depth 64 (deep attention 7.5ms -> ~0.8ms/token); (2) layer
dispatch set 15 -> 7 levels: concat-region fused QKV and W1|W3 GEMVs, then 2-ROWS-PER-SIMDGROUP
epilogue fusion — element pairs (2r, 2r+1) ARE weight-row pairs, so rope+KV-store rides the QKV
dispatch (q ropes to the buffer, k ropes straight into the mirror, v stores raw) and
silu(gate)·up rides the W1|W3 dispatch (gate row r + up row r+hidden in one simdgroup); fused
add+rms feeding the NEXT layer's norm; (3) f16 weight-scale GEMV twins (qscales16, lcpp q8_0
byte parity, -5.6% stream); (4) 1024-wide 1-tg rms/addrms; (5) SPECULATIVE GREEDY CHAINING —
the predicted next step's cb commits during the current GPU run and OPENS with GPU argmax
(CPU tie-break) + the winner's embed gather (cls_q8 linear plane) + a driver-built rope row
(`build_decode_rope_row` seam), so the GPU flows cb-to-cb with no CPU in the gap; mispredicts
verify against the caller's token and rerun through the watermark/rewind contract (exponential
backoff, `set_metal_decode_speculate` rail); unretained cbs. Negative results on record:
concurrent-encoder+per-dispatch-barriers LOSES to the serial encoder on M1 (framing lab
barrier mode — ggml wins by eliding, our chain can't); uint4 wide-load GEMV +0.5ms vs byte4;
single-tg attention above depth 64 loses to the chunked pair. All parity gates green
(test_metal_decode_parity 8 arms incl. wscale16 + hybrid + paged + rewind + churn + leak).

**2026-07-14 — Phase 8 (emission arc P4): BATCHED GPU decode — eval_batch rides the resident
driver. CLEAN ROUND (Parsec-off, best-of-3 process-fresh): 3B d512 batch curve
105.9/106.5/150.5/280.9 t/s at B=2/4/8/16 (vs llama-batched-bench Metal 134/219/250/363 =
0.79/0.49/0.60/0.77x; B=1 fixed-token 74.9 vs 85); single-stream tg128 re-confirmed at
88.36-88.37 t/s (UP from the P3 gate's 87.1-87.3 — the watchdog fix below deleted the event
machinery) with deep ctx512 80.2-80.7 unchanged.** The batch override claims the whole layer
stack + logits: KV mirrors became slices of ONE arena buffer (first-fit free list; LRU +
batch-uid pinning), so per-row attention and rope+KV-store reach every row's slice through one
bind + a per-row element-offset table (rt[b] = k/v base, cap, cnt — per-row capacities differ,
the layer index is a uniform); GEMV sites pick FIXED-B batched GEMV forms (B <= 4 — the P1a
lab winners; scale planes via dual-view binds, one kernel for f32 + wscale16) or the M-pad-32
GEMM twin (B >= 5, B-independent ~51-54ms step floor, rides the prefill quant tails
add+rms+quant / swiglu+quant); batched final rms + classifier on the same cb (the CPU tail's
classifier was ~6ms of every small-B step — eval_batch grew
`batch_decode_override_logits_done`, the decode seam's batch twin). Protocol: batch_decode_perf
`--fixed-token 100` (llama-batched-bench feeds fixed tokens; our greedy argmax is
~0.25ms/row/step of harness cost), -p 512 -n 128, f16 KV, wscale-f16 1,
DASLLAMA_METAL_KV_MIRROR_MB=3072 for 16 streams at depth. Steps are GPU-BOUND (CPU side
< 1.5ms incl. per-row KV writeback): gpu/step 17.5 (B=2, gemv_b2) / 35.9 (B=4, gemv_b4 — the
ALU-ceiling valley) / 51.3 (B=8) / 54.4ms (B=16, gemm32 floor) — all matching the P1a lab site
costs, so the remaining gap is pure skinny-M kernel efficiency (lcpp mul_mv at ne11 2-8) — the
P4-chase ledger row in API_REWORK (x-staged GEMV / gemm16 / mul_mm@64, lab rows first). FOUND
+ FIXED en route: a pre-encoded next-step cb parked COMMITTED across an idle gap (stream
switch, interactive pause) trips the GPU watchdog and the OS then ignores the process's
submissions — pre-steps now stay uncommitted until poked (spec steps still commit at encode;
the event-park mode measured ~neutral in P3 and is deleted). Gates: test_metal_decode_parity
grew the batch arms — B=3 GEMV + mid-run shrink, B=6 GEMM, mixed batch/single schedule (arena
handoff), one-step logits+KV tolerance x {B3,B6}x{f16,f32}, paged multi-run gathers, wscale16
both paths, leak gate; CPU batch suite 18/18.
NEXT: P4 batched decode (gemm32 crossover, batch seam driver), P5 ship.

**2026-07-30 — Phase 0 (dedup arc): FUNCTION + CLASS LOWERING in msl_emit — design (Boris-ruled).**
Goal: kernels feel like normal das code — free functions, class methods, and inheritance lower
1:1; no more "a kernel is a single function". Ruled: option 1 (proper MSL classes), tests are
Phase 0's own gate set. Mechanics:
1. **Free functions** lower as plain MSL functions (`def add(a, b : int) : int` →
   `int add(int a, int b)`), any module the body reaches, one MSL function per GENERIC
   instance (mangled name sanitized). Prototypes emit before bodies; recursion is a compile
   error. Param mapping by das type: `array<T>` → `device T*` (`device const T*` without
   `var`), fixed array `T[N]` → `threadgroup T*`, `var T&` → `thread T&` (thread-space
   binders only), scalars/vectors by value; value returns legal (kernel entry stays void).
   Free functions reading gl_* builtins get hidden trailing by-value params, threaded
   automatically at call sites.
2. **The kernel class emits as ONE FLAT MSL struct** — inherited members AND methods flatten
   into the concrete class; an override wins; no MSL inheritance, no vtables. das inheritance
   is source reuse; emission is always the concrete class. Struct fields: @ssbo → device
   pointer, @uniform → BY-VALUE copy (sidesteps address-space reference members), @workgroup
   → threadgroup pointer, used gl_* builtins → by-value fields. Methods emit as member
   functions — member access needs zero rewriting.
3. **The entry keeps today's ABI exactly** (same [[buffer(N)]] params, builtin params, shmem +
   `_tgmem` companion — encoders and the [metal_dispatch] lens untouched): it fills `self`
   (pointers, uniform copies, builtin copies, derived tgmem pointers) and calls the kernel
   method.
4. **Method calls devirtualize statically against the concrete kernel class.** Typed AST shape
   (probe-verified 2026-07-30): `ExprInvoke` with `isInvokeMethod=true`, arguments[0] =
   `ExprField(value = ExprTypeDecl, name = <method>)`, arguments[1] = (possibly ExprCast-
   upcast) receiver, rest = params. Resolution: method name → the CONCRETE class's field-init
   function — identical to CPU vtable results for kernel objects (one exact instance).
   Reassigning a method field in kernel code is a compile error.
5. **Write-set goes interprocedural by SIGNATURE at call sites** (a member passed to a
   `var array<T>` param is written; const param = read) **and by recursive body scan for
   methods** (they touch members directly).
Census grows matching kinds (gate B both-directions discipline). Tests:
`tests/metal/test_metal_functions.das` — CPU-oracle-vs-GPU for free fn / method / inheritance
+ override (base skeleton calls a derived-overridden stage) / generic ×2 instances / ref arg /
tgmem param / builtin-using helper / uniform-reading method; negative gates for recursion,
method-field reassignment, unsupported param types. First consumer: the dasLLAMA SqAttn family
dedup (16-of-20 twins share one skeleton; stage helpers + one uniform struct per family).

## @uniform structs — the kargs form

A kernel with a dozen scalars used to cost a dozen binds. `@uniform @binding = N ka : KArgs`,
where `KArgs` is a plain das struct, costs one: the struct definition emits into the MSL
preamble, the member becomes `constant KArgs& ka [[buffer(N)]]`, and `ka.field` is one hop.

**Fields are `int`/`uint`/`float` only.** That restriction is the whole feature: a struct of
4-byte scalars is the same bytes in das and in MSL, with no padding rules to keep in sync, so
the host writes its own struct straight through `setBytes` (`metal_set_bytes`, already bound —
Metal caps it at 4KB and a kargs struct is well under). A `float4` field would be 16-byte
aligned in MSL and shift every field after it; a nested struct brings its own alignment. Both
are refused (`tests/msl/_fail_closed/_fc_ustruct_{field,nested}.das`).

Host side: `run_compute_1d_kargs` for tests, and dasLLAMA's `kn_bytes` / `kn_kargs` on the
capture rail — under graph capture the bytes are COPIED into a pool, since the caller's kargs is
a stack local long gone by replay time.

**It costs the GPU nothing.** Every field was already a runtime `constant uint&`; the fold trades
N bindings for one argument-buffer read and the kernel branches on exactly the values it did
before. Nothing about the *shape* of a kernel may become a kargs field, though: a block stride, a
lane width, a codec selector belongs in a per-codec overload or a monomorphized generic, where it
stays a literal in the emitted MSL. Passing one as a value and trusting Metal to inline-and-fold
it back is an assumption, not a guarantee — and the assumption is worth nothing in the kernels
that matter.

Tests: `tests/msl/test_msl_uniform_struct.das` (definition placement, the single `constant&`
parameter, no per-scalar parameter survives) and `tests/metal/test_metal_uniform_struct.das`
(GPU vs CPU-oracle — the only thing that can prove the layout claim; mutation-verified by
inserting one pad field into the emitted struct).

## Phase 0 follow-on: pointer parameters

A helper takes a raw pointer: `def stage(var p : half4 const?; n : uint)`. This is what lets the
kernels that stream `unsafe(addr(buf[i]))` through a loop — the split-K "D" attention family, the
K-quant mul_mm trio — share a skeleton at all, since the advancing pointer is the shape and
rewriting it as index math changes AGX register allocation.

**MSL requires an address space in the signature** (`device half4*` vs `threadgroup half4*`) and
the das type `half4?` carries none — the same type can name an @ssbo interior or a @workgroup
interior. The parameter declares it and the call site proves it:

- **Unmarked is `device`.** Every pointer stream in the zoo is device, so the common case is
  plain das with no marker to learn.
- **`@threadgroup p : T?`** opts into threadgroup memory. Parameter annotations already parse
  and keep their values, so this needs no grammar.
- **The emitter derives the space from the argument's provenance and cross-checks it** — an
  inline `addr(member[i])`, a pointer local, a pointer parameter being forwarded, or `p + n`
  over any of those. Disagreement is a das error naming both sides; provenance it cannot trace
  (a thread-space local) is refused rather than guessed. The declared space is never trusted on
  its own, so a wrong marker cannot reach the runtime MSL compile as a null pipeline.
- **The POINTEE's const is the MSL const.** `T const?` lowers `device const T*` and reads only;
  `T?` lowers `device T*` and marks the source member written, exactly as a `var array<T>`
  parameter does. (Writing through it in das additionally needs the handle non-const — `var o :
  T?` — because das flows the handle's const onto the dereference.)

Long-term this is a shortcut for **pointer families** as a language-level feature — a real type
axis rather than an annotation. Parked on `modules/dasLLAMA/followup_general.md`.

Tests: `tests/msl/_msl_common.das` + `test_msl_functions.das` (signatures, forwarding, inline
`addr` arguments, census), `tests/metal/test_metal_functions.das` (GPU advancing dot + threadgroup
slab max vs a directly-computed expectation), `tests/msl/_fail_closed/_fc_ptr_space_{tg,dev}.das`
and `_fc_ptr_untraceable.das` (both mismatch directions + the untraceable case).
