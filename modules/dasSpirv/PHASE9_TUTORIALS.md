# Phase 9–11 — docs, foundation rails, tutorials (plan)

> Working plan written *before* code (the discipline that paid off in Phases 6/7/8). The durable record
> is `MASTERPLAN.md`; this is the reviewable design sketch for the post-plumbing arc: reference docs,
> the emitter **foundation rails** the tutorials need, and the **tutorials** themselves. Approved
> decisions (Boris, 2026-06-15) are marked **[decided]**.

## Why this is one plan, not three

The emitter language surface (Phases 0–8) is complete, but "complete" means *every shader-legal daslang
expression lowers* — it does **not** mean every GPU *feature* has a rail. An SDK-quality tutorial set is
the forcing function that surfaces the missing feature rails: each tutorial picks the canonical
application of a feature, and a tutorial we *can't write* names a rail we're missing. So docs + rails +
tutorials are planned together: **tutorials-as-rail-detector**, foundation rails first, then the
tutorials that consume them, then (Phase 12) the demo-scene that consumes the whole finished surface.

## Decisions

- **[decided] Placement.** SPIR-V *reference docs* live in the **main tree** (`doc/source/`) and link
  out to the runnable tutorials. The *runnable tutorials* live in the **dasVulkan repo** (`tutorials/`)
  — that is the only place with the GPU runtime, lavapipe CI, and the offscreen-readback infra. A
  tutorial that displays needs dasVulkan to dispatch the blob; dasSpirv only emits it, and the main tree
  cannot take a daspkg dependency (wrong direction). This revises the original Phase-9 note, which had
  tutorials as source+disassembly listings in the main tree (a "code you read" bar); the new bar is
  "demo you run."
- **[decided] Foundation-first.** Land the missing emitter rails as their own main-tree-gated PRs
  *before* writing the dependent tutorials, rather than interleaving one-rail-one-tutorial. Tutorials
  then consume a finished, fully-gated surface.
- **[decided] Compute tier in scope.** Shared/workgroup memory + barriers + atomics are in scope for
  the rail+tutorial set — the "compute done right" tier is dasSpirv's differentiator (compute and
  graphics in one language). Without it, the only buildable compute tutorial is embarrassingly-parallel
  Mandelbrot, which undersells the pitch.
- **[decided] Ray tracing is its own late phase.** RT is a *track*, not a rail (new stages, storage
  classes, acceleration-structure type, `OpTraceRayKHR`, a large dasVulkan-side RT-pipeline + SBT lift).
  It lands *after* everything else (own masterplan section, own working doc). **lavapipe does not do RT**
  — RT has no CI gate, local-GPU-only. That CI gap is the reason it is parked last.

## Reference docs (main tree, mechanical) — Phase 9

**Autogen mechanism = `doc/reflections/das2rst.das` module-hooking.** That is the existing daslang
auto-doc generator: it `require`s every documentable module and, per module, a `document_module_X(root)`
function groups the public symbols by regex (`group_by_regex`) and emits a `.rst` via `document(...)`
(symbols, signatures, `//!` docstrings, grouped sections — all from RTTI reflection). Phase-9 autogen is
therefore mostly **"add a `document_module_spirv`"**: hook the dasSpirv public surface (the `[*_shader]`
annotations, the `spirv_reflect` types/functions, the `spirv_grammar` enums if we want them) into
das2rst with grouped regexes, the same way `document_module_pugixml`/`_openai`/`_stbimage` do. This
shrinks Phase 9 to: (a) the das2rst hook (the bulk, mechanical), plus two hand-written prose pages —
(b) overview and (c) the mapping page. dasVulkan keeps its **own** `vulkan2rst` for its API; the
main-tree dasSpirv reference links out to the dasVulkan tutorials.

Three pieces under `doc/source/`:
1. **Auto-generated reference (das2rst hook).** The `[compute_shader]`/`[vertex_shader]`/
   `[fragment_shader]` surface, `@set`/`@binding`/`@uniform`/`@push_constant`/`@in`/`@out`/`@location`,
   the reflection API, and the **supported-opcode matrix** sourced from the per-phase census set (can't
   drift from what's emitted — same single-source discipline as the census tests).
2. **Overview page** (hand). The pitch: daslang→SPIR-V direct, no glslang/LLVM; "drivers optimize, we
   emit naive valid SPIR-V"; compute and graphics in one language; reflection→layout single source.
3. **The mapping page** (hand, the one with real content). daslang type → SPIR-V type/layout (std140
   UBO, std430 SSBO, ColMajor matrices, vec3 alignment), daslang builtin → GLSL.std.450 / core opcode.
   Each foundation rail (Phase 10) appends its row; the autogen reference self-updates.

Docs land first against the merged 0–8 surface, then extend incrementally per rail wave.

## Screenshot / recording / regression mechanism (the SDK-quality bar)

Each tutorial **renders and captures its own image** — a **PNG** for static demos, an **APNG** (animated
PNG, multi-frame) for animated ones, **not GIF** (Boris, 2026-06-15). The image is **simultaneously the
doc figure and the regression oracle**: procedural/analytic content is CPU-computable, so the check is
`assert_pixels_exact` (the Phase-7 exact-analytic oracle) or a tolerance compare for fuzzy frames. No
committed-then-stale screenshots — the doc image is CI-regenerated and proven every run (lavapipe in CI,
real GPU locally). The self-verifying-tutorial ethos applied to graphics.

**Reuse the existing recording rails, don't reinvent:**
- **APNG writer is shared code** — `stbimage`'s `stbi_apng_begin`/`stbi_apng_frame`/`stbi_apng_end`
  (`modules/dasStbImage/src/apng_write_impl.h`; `tests/stbimage/test_apng.das`). Same writer the dasImgui
  GL tutorials use.
- **GLFW is fine for tutorial windowing** (Boris) — `dasGlfw`'s `glfw_live` already drives APNG recording
  for the GL tutorials (`live_begin_frame`/`live_end_frame`, `--no-hdpi-framebuffer` to keep capture
  pixel-exact). The Vulkan side reuses the *writer*; capture is a Vulkan `vkCmdCopyImageToBuffer` readback
  (the GL `glReadPixels` path is GL-specific) feeding the same APNG frames.
- **mp4 conversion already exists in dasImgui** — review that process (ffmpeg APNG→mp4) and reuse it for
  the video pages, rather than authoring a new one.

**Soundtrack (when producing video) — decide per-tutorial** (Boris): either a **voiced narrative** (the
way the dasImgui tutorials do it — `say()` ASCII-only) for a guided walkthrough, or **music-only** (a
`strudel` track — we can reuse the dasImgui one) when the visuals carry it. Not blocking; chosen as each
tutorial's video is cut. See [[project_recording_soundtrack]] for the voiced+bed pipeline.

## The rail-gap map (grounded in the emitter source)

What's **not** a rail (correctly rejected at [spirv_emit.das fail-closed block], never coming): `new`/
`delete`/variant/tuple/struct-construction/`try`/`with`/`:=`/`<-`/string-builder/`typeinfo`/`??`/`?.`/
`goto`/`label`/named-call/`@@`/cast. Not shader constructs.

Cross-cutting: most rails need a **daslang intrinsic surface** (a function name the emitter intercepts by
name, like `texture`/`textureLod`/`dot` today) so user shaders can spell the operation. **[decided]** the
dummy intrinsics stay in `spirv_builtins.das` **for now** (Boris) — each rail slice decides its
name+signature there. Forward note: dasOpenGL/dasGlsl already has a parallel set of shader intrinsics, so
a later **Phase 15 — intrinsic unification + OpenGL cleanup** (shared shader-intrinsic surface across the
GLSL and SPIR-V backends) is worth a think; not scoped now.

### Tier 1 — blocks the confirmed ladder (#1–6)

| Rail | SPIR-V mechanism | Status | Unblocks |
|---|---|---|---|
| **SSBO of vec/struct (std430)** | layout on `array<float4>`/`array<Particle>` | ❌ **hard-blocked** — emitter rejects any non-scalar SSBO element | #6 particles (dead today), any real compute data |
| **Composite/global swizzle** | swizzle on a *loaded* vec (`OpVectorShuffle`/`OpCompositeExtract`), not just locals | ❌ known gap (`cam.pos.xyz`) | #3 raymarch, every UBO with vec3 fields |
| **Depth-compare sampling** | `sampler2DShadow` → `OpImageSampleDref*` + Depth image flag + compare-sampler | ❌ no Dref anywhere | #5 shadow (hardware-PCF variant) |
| **discard / alpha-test** | `OpKill` / `OpDemoteToHelperInvocation` | ❌ not dispatched | cutout, any "real" #4/scene |
| **derivatives** | `dFdx/dFdy/fwidth` → `OpDPdx/DPdy/Fwidth` | ❌ not dispatched (enum exists) | procedural AA in #3, mip selection |

Manual-compare shadow (#5) works *now* (sample depth as a plain `sampler2D`, compare with a ternary), so
#5 ships either way; Dref is the quality upgrade.

### Tier 2 — needed for a credible demo-scene + the expanded tutorials

| Rail | Mechanism | Status | Unblocks |
|---|---|---|---|
| **Shared/workgroup memory** | `Workgroup` storage class + `@shared` global | ❌ no `classify_global` branch | parallel reduction, tiled blur, prefix-sum |
| **Barriers** | `barrier()`/`memoryBarrier()` → `OpControlBarrier/OpMemoryBarrier` | ❌ not dispatched (enum exists) | pairs with shared mem |
| **Atomics** | `atomicAdd…` → `OpAtomic*` | ❌ not dispatched (enum exists) | histogram, counters, append buffers |
| **Texture queries** | `textureSize` → `OpImageQuerySizeLod` | ❌ | size-aware sampling, post-fx |
| **gl_FragDepth write** | Output builtin (only `gl_Position` is written today) | ❌ | raymarch→depth, soft particles |
| **Texture/descriptor arrays (bindless)** | indexed descriptors | ❌ | multi-material scene (Phase-12 demand) |

Already present (no new rail): compute-tiling + instancing builtins (`gl_LocalInvocationID`/`WorkGroupID`/
`NumWorkGroups`/`LocalInvocationIndex`/`InstanceIndex`) in `spirv_builtins.das`; `samplerCube`/`sampler3D`/
`sampler2DArray`, storage images (Rgba8), `textureLod`/`texelFetch` (Phase 7).

### Tier 3 — big-ticket, later

Ray tracing (own late phase, local-GPU-only); mesh/task shaders; geometry/tessellation; subgroup ops;
64/16-bit types. None block the tutorial set; scope per demand.

## Foundation rail waves — Phase 10 (emitter PRs, main-tree gated)

Each slice = its own PR, all three tiers (interp/JIT/AOT) + census + spirv-val + LCOV green, lint+format,
Copilot-to-dry, no GC leak. Each adds its daslang intrinsic + one fixture per op + a mapping-page row.

- **10.1 SSBO std430 vec/struct.** ArrayStride from element size/align; member offsets for struct
  elements; lifts the scalar-only restriction. The single biggest unblocker (particles + all real
  compute data).
- **10.2 Composite/global swizzle.** Swizzle on a loaded composite (UBO/SSBO/sampler-result), not just a
  local — `OpCompositeExtract` (single component) / `OpVectorShuffle` (multi). Closes the Phase-8
  out-of-scope note.
- **10.3 Fragment realism rails.** `discard` (intrinsic + `OpKill`/demote), derivatives (`dFdx/dFdy/
  fwidth`), `textureSize` query, `gl_FragDepth` write. Batchable — small, same stage.
- **10.4 Depth-compare sampling.** `sampler2DShadow` marker + a compare-sample intrinsic →
  `OpImageSampleDref*`, Depth image flag, comparison sampler reflection kind.
- **10.5 Compute tier.** `@shared` Workgroup-storage globals; `barrier()`/`memoryBarrier()` intrinsics →
  `OpControlBarrier`/`OpMemoryBarrier`; atomics (`atomicAdd`/`atomicMin`/… on SSBO + shared) →
  `OpAtomic*`. The differentiator tier.

## Tutorials — Phase 11 (dasVulkan repo, each consumes finished rails)

Feature → canonical demo, self-screenshotting, each also a pixel/analytic regression test. Confirmed
ladder #1–6 plus the expansion set; ordering follows rail availability.

| # | Tutorial | Headline feature(s) | Rails needed | Image |
|---|---|---|---|---|
| 1 | Triangle (the one allowed "hello") | vert+frag, Location I/O, interpolation | none (merged) | static PNG |
| 2 | Mandelbrot/Julia — 100% compute | iteration loop, ternary/break, `imageStore` | none (merged) | zoom GIF |
| 3 | Fullscreen SDF raymarch | UBO+`gl_FragCoord`+heavy GLSL.std.450+march loop+soft shadow | 10.2 swizzle (10.3 derivs for AA) | rotate GIF |
| 4 | Textured spinning cube | UBO mat4 MVP + `texture()` + depth + push-constant model | none (merged); 10.3 discard for cutout | spin GIF |
| 5 | Shadow mapping | two-pass depth render + sample-and-compare | manual: merged; hardware PCF: 10.4 Dref | GIF |
| 6 | Compute particles / boids | compute writes SSBO positions → graphics draws | **10.1 SSBO struct** (hard dep) | swirl GIF |
| 7 | Procedural noise / patterns | derivative-AA'd procedural fragment | 10.3 derivatives | static PNG |
| 8 | Parallel reduction / tiled blur | shared memory + barriers ("compute done right") | 10.5 shared+barriers | before/after PNG |
| 9 | Histogram / OIT counter | atomics into an SSBO | 10.5 atomics (+10.1) | chart PNG |
| 10 | Instanced field of objects | instancing | none (`gl_InstanceIndex` exists) | static PNG |
| 11 | Cubemap skybox + env reflection | `samplerCube` sampling | none (Phase 7) | static PNG |
| 12 | Post-fx ping-pong chain | storage-image read→write passes | none (Phase 7) | before/after PNG |

(#6, #8, #9 are the ones that were *impossible* before the rails — they are the proof the foundation
waves earned their keep. #11/#12 use already-merged Phase-7 surface and can land anytime.)

## Sequencing

Docs (9) land first against 0–8. Foundation rails (10.1→10.5) land as gated emitter PRs. Tutorials (11)
follow rail availability: #1/#2/#4/#10/#11/#12 can start immediately (no new rail); #3 after 10.2; #6
after 10.1; #5-hardware after 10.4; #7 after 10.3; #8/#9 after 10.5. Then Phase 12 demo-scene consumes
the finished surface; Phase 13 vulkan_lint; Phase 14 ray tracing (last, local-GPU-only).

## Gates (standing)

Emitter rails: opcode tests + census (both directions) + spirv-val + LCOV ~100% dispatch (main tree),
lint+format, Copilot-to-dry, no GC leak. Tutorials: render offscreen → self-screenshot → exact/tolerance
pixel oracle on lavapipe (CI) + local GPU; the generated image is the committed doc figure. Each phase is
its own PR (daslang master, dasVulkan, dasImgui all PR-protected).
