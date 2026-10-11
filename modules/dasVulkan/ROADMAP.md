# dasVulkan - roadmap / postponed work

The raw binding, the boost layer (handles, vk_view structs, sType ctors, command
wrappers, windowing), array-of-struct marshalling, the resizable swapchain, the
independent-count model, the Linux lavapipe test suite, and the documentation
site all shipped. What's deliberately left, with enough context to pick each up
cold:

## Tutorial arc 09-13 (current)

Tutorials 01-08 shipped (triangle, mandelbrot, sdf, cube, instancing, skybox,
particles, shadow mapping). Tutorials 09-13 cover the non-esoteric remainder of
Vulkan with five tutorials, each unblocking a discrete capability gap. Deferred:
tessellation, geometry shaders (obviated by mesh), push descriptors / timeline
semaphores / sync2 (correctness rails, not visual), sparse, multiview,
conservative raster (esoteric), hardware ray tracing (booked separately).

| # | Tutorial | New rails |
|---|---|---|
| 09 | MSAA | MSAA `samples` on color + depth, `resolveMode` + `resolveImageView` on the rendering attachment, a 1x resolve target |
| 10 | Deferred shading (subpasses + input attachments) | Multi-subpass render pass, `VkSubpassDependency`, input-attachment image usage, MRT, `subpassInput` / `subpassLoad` |
| 11 | HDR + bloom (post-processing chain) | Ping-pong offscreen targets, `R16G16B16A16_SFLOAT`, fullscreen-triangle pattern, tonemap |
| 12 | GPU-driven rendering (indirect + bindless) | `vkCmdDrawIndexedIndirectCount`, `VkDrawIndexedIndirectCommand`, `UPDATE_AFTER_BIND` + `descriptorBindingVariableDescriptorCount`, `nonuniformEXT()`, `gl_DrawID` |
| 13 | Mesh shaders | `VK_EXT_mesh_shader`, `vkCmdDrawMeshTasksEXT`, mesh/task execution models, `gl_MeshVerticesEXT[]`, `gl_PrimitiveTriangleIndicesEXT[]`, `SetMeshOutputsEXT`, `EmitMeshTasksEXT` |

### Implementation gap map (from the 2026-06-17 audit)

**dasVulkan side** (this repo) is mostly there - gaps are quick wrappers:

- 09: `cmd_begin_rendering` wrapper missing (raw `vkCmdBeginRendering` bound in
  `src/dasVULKAN.gen_funcs_15.cpp`; `cmd_end_rendering` already wrapped at
  `vulkan_cmds.das:2347`). `VkRenderingInfo` / `VkPipelineRenderingCreateInfo`
  ctors + boost views exist (`vulkan_ctors.das:5022`+, `vulkan_structs.das`).
  MSAA `samples` plumbing exists (`vulkan_boost.das:238,396,490`).
- 10: All bound; no boost helper for multi-subpass render passes. Doable
  manually via `VkSubpassDependency` array + multi-`VkSubpassDescription`.
- 11: All bound; HDR formats work like any format. No fullscreen-triangle
  helper (trivial 3-vertex pattern).
- 12: All bound; `cmd_draw_indexed_indirect_count` at `vulkan_cmds.das:1352`,
  `VkDrawIndexedIndirectCommand` view at `vulkan_structs.das:2096`,
  `VkDescriptorSetLayoutBindingFlagsCreateInfo` ctor at `vulkan_ctors.das:2082`.
  Friction: bindless pNext chain is raw `void?` today (the typed-pNext gap
  below pinches here for the first time).
- 13: `cmd_draw_mesh_tasks_ext` already wrapped at `vulkan_cmds.das:1445`!
  `cmd_draw_mesh_tasks_indirect_ext` missing wrapper (raw call bound).
  `VkPhysicalDeviceMeshShaderFeaturesEXT` ctor at `vulkan_ctors.das:2488`.
  Same pNext friction for feature-enable.

**dasSpirv side** (daslang main tree, `modules/dasSpirv/`) holds the heavier
lifts:

- 09: nothing emitter-side.
- 10: MRT - emitter enforces single `@location` per global at
  `spirv_emit.das:372-374`; needs array-of-locations support. `subpassInput`
  type missing entirely; needs `OpTypeImage Dim=SubpassData`, `subpassLoad`
  intrinsic, `InputAttachmentIndex` decoration surface (grammar has the
  decoration enum at `spirv_grammar.das:43`, never emitted).
- 11: nothing emitter-side.
- 12: `nonuniformEXT()` intrinsic + `NonUniform` decoration emission missing
  (grammar has `NonUniform = 5300` and `RuntimeDescriptorArray = 5302`
  capability, never emitted). `gl_DrawID` builtin missing entirely
  (`spirv_builtins.das`); needs `DrawParameters` capability.
- 13: grammar 70% ready (`TaskEXT = 5364`, `MeshEXT = 5365`, `MeshShadingEXT =
  5283`, `PerPrimitiveEXT = 5271` present in `spirv_grammar.das`); pinned
  SPIRV-Headers version still needs `OpEmitMeshTasksEXT` confirmed. Emitter
  side everything missing: execution-model dispatch hardcoded to
  vertex/fragment/compute at `spirv_emit.das:2818-2822`,
  `[vulkan_mesh_shader]` / `[vulkan_task_shader]` annotation classes,
  `gl_MeshVerticesEXT[]` / `gl_PrimitiveTriangleIndicesEXT[]` builtins,
  `SetMeshOutputsEXT` / `EmitMeshTasksEXT` intrinsics. The mesh-shader emitter
  surface is the single biggest piece of the arc - likely 2-3 sub-PRs.

### PR-by-PR plan

Each line is one PR. dasVulkan PRs land in this repo; dasSpirv PRs land in
daslang main tree (`modules/dasSpirv/`). Tutorial PRs land here, gated on their
impl prereqs. Mark `[merged]` and the PR number inline as each lands.

1. dasVulkan - `cmd_begin_rendering` wrapper.
2. dasVulkan - **Tutorial 09** (MSAA on the existing cube scene).
3. daslang - dasSpirv MRT: multiple `@location` outputs from fragment.
4. daslang - dasSpirv subpass inputs: `subpassInput` + `OpTypeImage
   Dim=SubpassData` + `subpassLoad` + `InputAttachmentIndex` decoration.
5. dasVulkan - **Tutorial 10** (deferred shading - G-buffer + lighting subpass).
6. dasVulkan - **Tutorial 11** (HDR + bloom; no impl prereq).
7. daslang - dasSpirv `nonuniformEXT()` + `gl_DrawID` builtin.
8. dasVulkan - bindless helper (descriptor-set-layout builder with
   `VkDescriptorSetLayoutBindingFlagsCreateInfo` pNext typed in).
9. dasVulkan - **Tutorial 12** (GPU-driven culling + indirect + bindless).
10. daslang - dasSpirv mesh-shader surface (annotations + execution model
    dispatch + builtins + intrinsics). Likely splits into 2-3 sub-PRs.
11. dasVulkan - mesh extension feature-enable helper +
    `cmd_draw_mesh_tasks_indirect_ext` wrapper.
12. dasVulkan - **Tutorial 13** (cluster culling task -> meshlet mesh -> fragment).

About 80% of the implementation work concentrates in PRs 3, 4, and 10. The
others are quick. Tutorials write themselves once their impl PRs land.

### Polish pass on tutorials (parked)

After the arc closes (tutorial 13), revisit all of `tutorials/09_msaa` ..
`tutorials/13_*` with a "make the WHY visible" lens. The mechanics rail
landed correctly in each, but the demo scenes are minimal and several rails
don't show up strongly in the rendered output. Known soft spots:

- **Tutorial 10 SSAO**: visible only at grazing-angle silhouettes (the
  curvature stand-in lights up there). A richer scene (cluttered objects /
  concave corners / overlapping geometry) would surface the AO contribution
  much more obviously. Could also revisit promoting the G-buffer to sampled
  images alongside the input-attachment rail to do real screen-space
  hemisphere sampling.
- **Tutorial 10 normal mapping**: derivative-based TBN works but the
  procedural bump fields are very subtle on a flat floor / box. A richer
  texture (bricks with deeper mortar, knurled metal on the cube) would
  push the per-fragment perturbation visibly into the lighting.
- **Tutorial 10 point lights**: visible with the current 4x-boosted
  intensity but only on flat ground. A scene with vertical surfaces /
  multiple receivers would show off the per-pixel-cost-constant-in-light
  -count payoff more obviously.
- **Tutorials 11-13** (HDR / GPU-driven / mesh shaders): expect similar.
  Each will land the rail correctly first; the visual polish is the second
  pass.

The push-constant trailing-scalar drop is RESOLVED: pushes are staged
through the shared std140 walk (`std140_block_writes`; regression gate
`tests/integration/test_push_std140.das`), so tutorial 10's `gbuffer_fs`
positional material hack is now redundant and can be swept.

Per-shader `bind_uniform` binds only the FIRST `@uniform` block -- by
design, not a failure. A shader may legitimately reach more than one UBO
block (e.g. `xform` at set 0 + `scene` at set 1); the host owns each
additional one. A `with_mapped_memory` write is valid only for
host-visible memory, so a host-coherent UBO must never be conflated with
a device-local (staged, NOT mappable) one -- `deferred_tut.das`'s
`write_scene_ubo_manual` is the sanctioned pattern, not a workaround.
Per-binding multi-UBO auto-bind stays unimplemented.

## Vulkan 1.4 tutorial, shader objects, the 1.4 CI lane

The tutorials and the window layer draw with dynamic rendering and
synchronization2, the Vulkan 1.3 idiom (`ARCHITECTURE_RENDERING.md#dynamic-rendering`).
1.4 adds no new way to draw a frame; its promotions that touch us are
`dynamic_rendering_local_read` (the subpass replacement), `push_descriptor`,
`host_image_copy`, `maintenance5/6`. Shader objects (`VK_EXT_shader_object`) are
an extension in every version. What remains:

- **Tutorial 16 "Vulkan 1.4"** - the three promotions that change a workflow:
  push descriptors (per-draw data written into the command buffer, no set
  allocation), host image copy (texture upload with no staging buffer or
  transfer command), local read (the subpass replacement, on the deferred
  scene). `create_device` gains an optional requested API version + feature
  list, defaulting to today's 1.3 behavior; the tutorial asks for 1.4 + the
  three features and skips with a message when refused.
- **Shader objects** - one example under `examples/` + test, opt-in only:
  absent on Android, partial on MoltenVK, and dasllama's compute path is
  `VkPipeline`. Lavapipe is the extension's reference implementation, so the
  nightly lane runs it.
- **The nightly render lane** installs `mesa-vulkan-drivers` from Ubuntu 24.04
  (Mesa 24.0-24.2, lavapipe at 1.3); lavapipe reports 1.4 from Mesa 25.0, so the
  lane needs a Mesa PPA or tutorial 16 skips on the only lane that renders. The
  macOS lane is build + loader smoke only; Windows has no lane - a local 1.4 GPU
  is the proof there.
- **A `vulkan_lint` rule for the frame-shape mix** - a pipeline built for a render pass bound
  inside `record_rendering`, or one built for formats bound inside `record_render_pass`. A
  `[lint_macro]` sees the consumer after inlining: the `record_rendering` call and its block are
  spliced away, so the nesting that would anchor the rule is gone, and the spliced boost bodies
  surface their raw `vkCmd*` calls at the call site's line, which is also why VK001 fires on a
  `record_render_pass` call today. The rule needs an inline-aware anchor (the begin/end pair that
  survives splicing) and the same fix for VK001.
- **A loud failure below 1.3** - on an instance or device below 1.3 the plain
  creators leave the pair off and say nothing, and a tutorial then crashes on a
  null `vkCmdBeginRendering` entry point. `record_rendering`,
  `cmd_pipeline_barrier2` and `queue_submit2` panic with a message instead when
  `dynamic_rendering_supported` is false for the device they record on.
- **dasLLAMA** keeps its legacy barriers until its own arc: four hot-path files,
  so a `vulkan_lint` nudge from the legacy flags to the `_2` ones waits for it.
  Legacy barriers are not slower - drivers lower both forms to the same work;
  the only win is the stage choice, which `_2` makes easier to express. The same
  arc moves `shaderIntegerDotProduct` out of its own struct and into the
  `VkPhysicalDeviceVulkan13Features` the `create_device_storage_8_16_int_dot*`
  creators already chain: a chain holding both is invalid
  (`VUID-VkDeviceCreateInfo-pNext-06532`).
- **Pre-existing validation-layer findings** the flip left in place, each a
  shader or descriptor shape, not a barrier: 08, 10 and 14 declare the shadow
  sampler as `SAMPLER` where the shader consumes a combined image sampler
  (`VUID-VkWriteDescriptorSet-descriptorType-00319`, `-layout-07990`); 11's
  readback intermediate gets an image view its usage does not allow
  (`VUID-VkImageViewCreateInfo-image-04441`); 12's cull shader writes a
  `NonWritable` storage member (`VUID-RuntimeSpirv-NonWritable-06341`); 10
  carries one synchronization hazard inside its render pass.

## p-prefix strip on boost field names

The boost view structs keep Vulkan's C field names verbatim - `pAttachments`,
`pApplicationInfo`, `renderPass`, `queueFamilyIndex` - even though in daslang
`pAttachments` is just `array<ImageView>`, not a pointer. The `p` / `pp`
Hungarian prefix is meaningless on the boost side.

A p-strip pass would rename the boost field to drop the prefix (`pAttachments` ->
`attachments`). It's purely cosmetic: the generated marshalling maps boost-field
-> raw `Vk*`-field by position, so the boost-side name is free to change without
touching the C side.

Deferred because: (1) it's ergonomics, not function; (2) it's a churning
public-API rename touching every example/test that sets a field by name; (3) it's
entangled with two related decisions best made in the same pass - whether to drop
the auto-derived `...Count` fields from the public surface entirely, and whether to
go full snake_case. Do it once, decisively, with the convention nailed down.

## Typed pNext chains

Nearly every Vulkan struct begins `VkStructureType sType; const void* pNext;`.
`pNext` points at another extension struct (also starting sType+pNext), forming a
linked "chain" the driver walks to read optional/extension parameters without
changing the base struct's ABI.

Today this is a raw escape hatch: the boost field is `next : void?`, copied
straight to `vk.pNext`. Attaching an extension struct means manually setting its
`sType`, taking `unsafe(addr(...))`, and keeping it alive across the call.

A typed-chain API would take a list of extension structs, auto-fill each `sType`,
link the `pNext` pointers in order, manage their lifetime via the same scratch
mechanism `vk_view` already uses for arrays, and validate against the registry's
`structextends` metadata (already parsed by the generator) that each struct may
legally extend the base.

Deferred because: the raw `void?` is sufficient for everything the current
examples do (offscreen triangle, compute, windowed present need no chains), and a
typed API needs the `structextends` data wired through the emitter, a
scratch/lifetime mechanism for chained structs, and an API-shape decision
(fluent builder vs. array-of-variant).

## macOS - **DONE** (MoltenVK)

macOS works as an opt-in (`-DDAS_VULKAN_DISABLED=OFF`; the module is off by default on Apple):
`brew install molten-vk vulkan-loader vulkan-tools`, and the offscreen suite + windowed
examples run on Apple GPUs via MoltenVK. Three
pieces made it work, all platform-agnostic:

- **Loader discovery** - `das_volkInitialize` (`src/dasVULKAN.main.cpp`, `__APPLE__`)
  falls back to dlopen'ing the loader from `$VULKAN_SDK` / the Homebrew prefix when
  volk's built-in search misses it, then wires volk via `volkInitializeCustom`.
- **Portability** - `create_instance` (`daslib/vulkan_boost.das`) auto-enables
  `VK_KHR_portability_enumeration` + the create flag when the loader advertises it
  (else MoltenVK returns `ERROR_INCOMPATIBLE_DRIVER`). No-op on Win/Linux.
- **Metal surface** - `vk_surface_from_native` creates a `VkSurfaceKHR` from a
  `CAMetalLayer` (`vkCreateMetalSurfaceEXT`); the Cocoa/QuartzCore code is isolated
  to `src/dasVULKAN.metal.mm`. Windowed apps call
  `glfwInitVulkanLoader(vk_get_instance_proc_addr())` before `glfwInit` so GLFW
  finds the same loader (dasGlfw binds `glfwInitVulkanLoader`).

Known limitation: the `CAMetalLayer.contentsScale` is set once at attach time
(retina-correct for the window's current display). A window dragged to a
different-DPI display mid-session keeps its original scale - a proper fix needs a
display-change hook reconciling `contentsScale` alongside the existing
swapchain-extent recreation. Deferred (single-display is the common case).

A macOS CI lane is wired (`tests.yml` `build_macos`, Apple-Silicon runner), but it
is **build + loader-discovery only**, not a render gate. GitHub-hosted macOS
runners expose only a paravirtualized GPU (`AppleParavirtGPU`): stock MoltenVK
crashes during device init, and even with `MVK_CONFIG_USE_METAL_ARGUMENT_BUFFERS=0`
the render tests fail with `ERROR_OUT_OF_DEVICE_MEMORY` allocating targets (the
same suite passes on real Apple GPUs). So the lane gates the macOS build (the CMake
`APPLE` branch, `dasVULKAN.metal.mm`, Cocoa/QuartzCore linking) and a no-render
smoke (module load + Homebrew loader discovery). Running the offscreen render suite
on macOS needs real hardware (local) or a future GPU-backed runner; a SwiftShader
(CPU ICD) lane is the other option if render coverage on macOS CI becomes worth the
setup.

## Windows CI

`tests.yml` and `docs.yml` are Linux-only. A Windows lane needs a software Vulkan
ICD (lavapipe or SwiftShader, via `jakoch/install-vulkan-sdk-action`) with
`VK_DRIVER_FILES` pointed at the ICD JSON. The native-module build already works
on Windows (MSVC) locally; this is purely the CI wiring.

## oldSwapchain reuse

`recreate_swapchain` destroys the old swapchain *before* creating the new one
(destroy-first), because move-assigning the new one while the old still owns the
surface trips `ERROR_NATIVE_WINDOW_IN_USE_KHR`. That leaves a momentary gap where
no swapchain exists. The proper fix passes the old swapchain as
`VkSwapchainCreateInfoKHR.oldSwapchain` so the driver can recycle resources and
the handoff is seamless, then destroys the old one after.

## The skipped command / struct tail

The struct emitter skips ~182 composites and the command emitter ~124 commands -
the irregular long tail: structs/commands whose params are flags-output, raw
`PFN_*` function pointers, foreign/opaque types, or other shapes the uniform
vk_view / classifier rules don't cover. Each is logged at generation time. Most
are exotic extensions; revisit case-by-case if a consumer needs one.

## Delete the GLSL compile helper (decided 2026-07-16, do ~2026-07-30)

`cmake/DasVulkanCompileShader.cmake` (+ its `include` at `CMakeLists.txt:21`) is
the last GLSL-era artifact: a consumer-facing glslangValidator/spirv-opt macro,
unused by the repo itself since the dasSpirv migration ("ZERO GLSL, ZERO
committed .spv" - `modules/dasSpirv/ARCHITECTURE.md`). Decision (2026-07-16): shaders are
always das-authored, the helper goes; deletion deferred ~2 weeks in case a
consumer objects. Delete the file + the include line; nothing else references
it.

The generator emits every pointer-to-struct command parameter as an out-parameter it zeroes
and fills, so a command whose struct is IN/OUT loses its input: `vkSetGpaDeviceClockModeAMD`
(vulkan-sdk-1.4.357.0's AMD GPA extension) can only request the default clock mode through the
boost wrapper. Teach the generator the in/out class (the registry does not mark it; a per-command
list is the honest form) or hand-write the wrapper in `vulkan_boost.das`.
