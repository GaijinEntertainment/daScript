# dasVulkan original design plan (historical)

> Preserved verbatim for history. This is the plan that drove the boost-layer
> build-out (it followed an earlier round that already had the raw `vulkan`
> binding + offscreen triangle working). **Almost all of it is now implemented** —
> all 9 boost phases, the examples, the dastest suite, and the Linux lavapipe CI
> shipped, followed by array-of-struct marshalling, a resizable swapchain, the
> independent-count model, and the documentation site. For what remains, see
> [`ROADMAP.md`](../../modules/dasVulkan/ROADMAP.md); for current working
> instructions, see [`CLAUDE.md`](../../modules/dasVulkan/CLAUDE.md). The text
> below is unedited, and predates the move in-tree to `modules/dasVulkan/` — its
> paths are relative to the old standalone repository root.

---

# dasVulkan — Boost layer, examples, tests, CI

## Context

The raw `vulkan` binding works end to end: a daslang generator (`generator/*.das`, parses `vk.xml` via dasPUGIXML) emits ~3 MB of C++ over volk — 1279 structs (`ManagedStructureAnnotation`), 158 enums, 214 VkFlags as daslang `bitfield` types, ~820 command wrappers. `examples/offscreen_triangle.das` renders the canonical RGB triangle offscreen on a real GPU and self-verifies six structural pixel checks.

But raw Vulkan from daslang is verbose and sharp-edged (observed while writing the triangle): `sType` set by hand on every CreateInfo; single out-handles are by-ref while `len`-array out-handles are double-pointers; handles from `let` can't go in struct fields; pointer fields set under `unsafe { ci.pX = addr(local) }`; manual two-call enumeration; no RAII (every `vkCreate` needs a hand-matched `vkDestroy`). The **boost layer** erases all of this.

**Decisions (Boris, this session):** generate everything now (full olegus8-style — all-handle ownership, vk_view marshalling, full command wrappers); defer docs; Linux CI first (Windows follow-on); ship triangle + compute + enumerate examples. Offscreen-first — no window/surface/swapchain this round.

The boost is a **parallel idiomatic API**: the user writes in boost types (`Device`, `Image`, `SubmitInfo` with `array<Semaphore>`) and never touches raw `Vk*` except via escape hatches. It is generated uniformly from the same `VkRegistry` the raw layer already builds, so "generate everything" is the same emission code applied to all types — not N× the work. The long tail of irregular types is handled with logged skips + a small override table, exactly as the raw layer skips 130 structs / 13 commands.

## Target API — the boost triangle (the acceptance showcase)

~150 lines (from 470), no hand-set `sType`, no user `unsafe`/`addr`, no manual destroy:

```das
var inscope instance <- create_instance(application_name = "triangle", api_version = make_api_version(1,3,0))
volkLoadInstance(instance |> raw)
let phys = select_physical_device(instance)
let gfx  = select_graphics_queue_family(phys)
var inscope device <- create_device(phys, gfx); volkLoadDevice(device |> raw)
let queue = get_device_queue(device, gfx, 0u)
var inscope target <- build_offscreen_target(device, phys, WIDTH, HEIGHT, FORMAT)   // image+mem+view, one owner
var inscope rp   <- create_render_pass_single_color(device, FORMAT)
var inscope fb   <- create_framebuffer(device, render_pass = rp, attachments = [target.view], width = WIDTH, height = HEIGHT)
var inscope pipe <- create_graphics_pipeline_simple(device, rp, vert_spirv, frag_spirv, WIDTH, HEIGHT)
var inscope readback <- create_host_buffer(device, phys, buf_size)
var inscope pool <- create_command_pool(device, gfx)
run_cmd_sync(device, pool, queue) <| $(cmd) {
    record_render_pass(cmd, rp, fb, full_area(WIDTH,HEIGHT), clear_color(0.1,0.1,0.15)) <| {
        cmd_bind_pipeline(cmd, pipe); cmd_draw(cmd, 3u)
    }
    copy_image_to_buffer(cmd, target.image, readback, WIDTH, HEIGHT)
}
var pixels : array<uint8>
map_memory_to_array(device, readback) <| $(m) { pixels := m }
// PPM + 6 structural checks unchanged; finalizers destroy in reverse order — no cleanup block
```

## Design

### Boost generation architecture

New `generator/vk_emit_boost.das` (siblings `vk_emit.das`; `require vk_model, vk_naming, vk_emit`). It reuses the existing `EmitModel` (`build_emit_model`), `member_type_known`, `FlagsInfo`, `enum_short_name`/`das_safe_name`, and the same guard/protect-supported + enabled-set filtering — so boost is emitted **only for types the raw layer actually registered** (daslang has no `#if`; referencing an unregistered symbol fails). Refactor `emit_all` so `build_emit_model` is called once and the model is shared by both the C++ emitter and the new boost emitter.

`generate.das` gains a `--boost-out` clarg (default `daslib`) and calls `emit_boost(reg, em, boost_out_dir)` after `emit_all`.

**Generated pure-das files** (into `daslib/`, "DO NOT EDIT" banner, chunked like the C++):
- `daslib/vulkan_ctors.das` — sType-filling constructors for every struct with an sType member.
- `daslib/vulkan_handles.das` — per-handle ownership wrapper struct + `finalize` + `boost_value_to_vk`/`vk_value_to_boost` + generated create/allocate wrappers.
- `daslib/vulkan_structs.das` — vk_view idiomatic boost structs (array<T> fields + scratch + `vk_view_create_unsafe`/`vk_view_destroy`) for every struct that carries arrays/handles/pointers.
- `daslib/vulkan_commands.das` — defaulted + named-arg + optional `VkResult?` wrappers for the non-create command set (`vkCmd*`, `vkGet*`, `vkQueue*`, etc.).

**Hand-written** `daslib/vulkan_boost.das` (the file `.das_module` already names): `require` the four generated files `public`, plus the judgement-bearing pieces — `weak_copy` (generic), `vk_check`, version helpers, selection helpers (`select_physical_device`, `select_graphics_queue_family`, `find_memory_type`, `find_host_visible_memory`), block brackets (`run_cmd_sync`, `record_render_pass`, `map_memory_to_array`), combined builders (`build_offscreen_target`, `create_host_buffer`, `create_render_pass_single_color`, `create_graphics_pipeline_simple`), and `raw`/`full_area`/`clear_color`/`make_api_version` conveniences.

### Tier 1 — ownership wrappers (`vulkan_handles.das`)

Per owned handle `H` (raw handles are plain pointers, so ownership lives on a struct):
```das
struct Semaphore { _vk : VkSemaphore; _needs_delete : bool; _device : VkDevice }
def finalize(var self : Semaphore) {
    if (self._needs_delete) { vkDestroySemaphore(self._device, self._vk, null) }
    self._needs_delete = false
}
def boost_value_to_vk(self : Semaphore) : VkSemaphore { return self._vk }
def vk_value_to_boost(v : VkSemaphore; device : VkDevice) : Semaphore { var s; s._vk = v; s._device = device; return s }  // non-owning
```
Parent fields store **raw handles** (`VkDevice`), never wrapper structs — this sidesteps copy-ownership entirely (a parent is only ever needed as the raw arg to the destroy call). `weak_copy` is one hand-written generic using `static_if (typeinfo has_field<_needs_delete>(dst)) { dst._needs_delete = false }` (idiom confirmed in `decs_boost.das:242`). No-destroy handles (`VkPhysicalDevice`, `VkQueue`) get a plain non-owning wrapper.

**Ownership derivation algorithm** (in `vk_emit_boost.das` over `VkRegistry`): for each handle, find the creator (`vkCreate{H}` / `vkAllocate*` whose last non-const `H*`-typed param is the out-handle, confirmed by param scan), the destroyer (mirror name, leading handle params a prefix of the creator's), and the parents (the destroyer's leading handle-typed params before the destroyed handle and `pAllocator`). ~80% derive cleanly. A small hand-curated **override table** (`let OWNER_OVERRIDES`, shaped like `SUPPORTED_PROTECTS`) covers the irregular ~20%:
- Allocate/Free where the parent lives in the AllocateInfo, not the signature (`vkAllocateCommandBuffers` pool = `pAllocateInfo.commandPool`; `vkAllocateDescriptorSets` pool = `pAllocateInfo.descriptorPool`) → override sources the parent from an info-struct path.
- Batch creation (`vkCreateGraphicsPipelines`/`ComputePipelines` create count-N, destroy singular `vkDestroyPipeline`) → override: count=1 creator, wrapper destroys its single element.
- Pool-reclaimed handles (pooled command buffers; descriptor sets from a non-`FREE_DESCRIPTOR_SET` pool) → override: `default_needs_delete = false`, pool finalize reclaims.

### Tier 2 — vk_view idiomatic structs (`vulkan_structs.das`)

For every struct carrying arrays/handles/pointers, a boost struct with idiomatic fields (`array<T>` replacing count+pointer pairs, `string` replacing `const char*`, boost handle types replacing raw, `sType`/`pNext`/`pAllocator` erased) plus hidden `_vk_view_*` scratch arrays + an `_vk_view__active` reentrancy guard:
```das
struct SubmitInfo {
    wait_semaphores : array<Semaphore>; command_buffers : array<CommandBuffer>; signal_semaphores : array<Semaphore>
    _vk_view_wait_semaphores : array<VkSemaphore>; ...; _vk_view__active : bool
}
def vk_view_create_unsafe(var b : SubmitInfo) : VkSubmitInfo {
    b._vk_view_wait_semaphores <- [{for s in b.wait_semaphores; s |> boost_value_to_vk}]
    return <- VkSubmitInfo(sType = ..., waitSemaphoreCount = uint(length(...)), pWaitSemaphores = array_addr(b._vk_view_wait_semaphores), ...)
}
def vk_view_destroy(var b : SubmitInfo) { delete b._vk_view_wait_semaphores; ...; b._vk_view__active = false }
```
Scalar-only structs get a trivial direct field copy (no scratch). Nested struct fields recurse (`SubpassDescription` views its `AttachmentReference` arrays). `pNext` is exposed as a raw `void?` escape-hatch field in this round; **typed pNext chains are deferred** (the registry's `struct_extends` metadata is already parsed, so typed chains are a clean follow-on). Generated command wrappers do `var vk = vk_view_create_unsafe(info); defer() { vk_view_destroy(info) }`.

### Tier 2 — command wrappers (`vulkan_handles.das` creators + `vulkan_commands.das`)

Creators (in `vulkan_handles.das`) return an owning wrapper:
```das
def create_semaphore(device : Device; create_info : SemaphoreCreateInfo = SemaphoreCreateInfo(); var result : VkResult? = [[VkResult?]]) : Semaphore {
    var vci = vk_view_create_unsafe(create_info); defer() { vk_view_destroy(create_info) }
    var h : VkSemaphore
    vk_check(vkCreateSemaphore(device |> boost_value_to_vk, vci, null, h), result)
    var b = vk_value_to_boost(h, device |> boost_value_to_vk); b._needs_delete = true
    return <- b
}
```
Non-create commands (`vulkan_commands.das`) take boost types, default every param (enabling named-arg calls), and accept optional `var result : VkResult?` (null ⇒ `vk_check` asserts SUCCESS; non-null ⇒ caller handles — used by the swapchain OUT_OF_DATE path later). Two-call enumeration is auto-wrapped to return `array<T>`. `vk_check` is hand-written: `out == null` ⇒ panic with the result name; else `*out = r`.

### Hand-written helpers (`vulkan_boost.das`)

Pure judgement, not generated: `find_memory_type`/`find_host_visible_memory` (lift from the example), `select_physical_device` (prefer discrete), `select_graphics_queue_family`, `run_cmd_sync(device,pool,queue) <| $(cmd){...}` (allocate one-shot buffer, record block, submit, wait idle — allocates/frees internally because `defer` can't do per-iteration teardown in a loop), `record_render_pass(cmd,rp,fb,area,clears) <| {...}`, `map_memory_to_array(device, buffer) <| $(arr){...}`, and the combined builders (`build_offscreen_target` = image+memory+view as one ownership struct; `create_render_pass_single_color`; `create_graphics_pipeline_simple` — the one place that encodes too much policy to generate, hand-written using the generated `GraphicsPipelineCreateInfo` view).

## Examples (all become CI tests)

- `examples/offscreen_triangle_boost.das` — the showcase above; **must produce the identical PPM + pass the same 6 structural checks** as the raw triangle (regression gate). Keep `examples/offscreen_triangle.das` as the raw reference.
- `examples/compute.das` — offscreen, no graphics: a compute shader fills a storage buffer (e.g. `out[i] = i*i`); read back and verify values. Exercises `create_compute_pipeline_simple`, descriptor set layout/pool/set + update, `vkCmdDispatch`, storage-buffer path — a different code path from the triangle.
- `examples/enumerate.das` — no rendering: list GPUs, queue families, memory heaps, key `VkPhysicalDeviceLimits`. A gentle intro that smoke-tests the raw enumeration helpers + boost selection.
- New shaders under `examples/shaders/`: `compute.comp` (+ `.spv`), committed like the triangle's.

## Tests (`tests/integration/`)

In-process dastest — simpler than dasImguiImplot (no GUI subprocess; offscreen render runs in-process). `require dastest/testing_boost public` + `require vulkan/vulkan_boost`. Each test renders/computes offscreen and asserts with `t |> success(cond, "msg")` / `t |> equal(a,b)`:
- `test_triangle.das` — render the boost triangle in-process, sample the three vertex regions + centroid + corners (the 6 structural checks as `t |> success`).
- `test_compute.das` — dispatch, read back, assert `out[i] == i*i` for sampled `i`.
- `test_enumerate.das` — assert ≥1 physical device, ≥1 graphics queue family, ≥1 memory heap, sane limits.
- `test_handles.das` — RAII round-trips: create/finalize each core handle, assert no validation errors and `is_alive`-style checks; verify `weak_copy` doesn't double-destroy (create owner, weak-copy, drop copy, owner still valid).

Each test file is self-contained (in its own dir, bare same-dir `require` for any shared fixture). Runner:
`daslang -load_module D:/DASPKG/dasVulkan dastest/dastest.das -- --test <pkg>/tests/integration --isolated-mode --isolated-mode-threads 4` (no `--headless` flag needed — that's a GUI concept; offscreen Vulkan needs only the lavapipe ICD in CI). Use `to_log`, not `print`, in test bodies.

## CI — `.github/workflows/tests.yml` (Linux first)

Mirror dasImguiImplot's tests.yml structure: trigger on push/PR/dispatch; checkout `GaijinEntertainment/daScript@master` + this repo; build daslang from source (cmake Ninja, `-DDAS_PUGIXML_DISABLED=OFF` is NOT needed for the package build — only the generator needs pugixml, and generated output is committed; build `daslang` + the package module). Add the Vulkan-specific steps:
- Install lavapipe via `jakoch/install-vulkan-sdk-action@v2` (`install-runtime: true`, `install_lavapipe: true`), or apt `mesa-vulkan-drivers libvulkan1 vulkan-tools`.
- Build the package module: `cmake -B _build -DDASLANG_DIR=<daslang-root>` + `cmake --build _build --config Release` (uses the committed generated sources + vendored volk/Vulkan-Headers; no SDK needed).
- Run dastest with `env: VK_DRIVER_FILES: /usr/share/vulkan/icd.d/lvp_icd.x86_64.json` and **not under sudo** (the loader ignores `VK_DRIVER_FILES` when elevated). volk finds the software ICD via the standard loader automatically.
- Sanity step: `vulkaninfo --summary` (expect `llvmpipe`, Vulkan 1.3).
Windows lane is added later (same `jakoch` action, `VK_DRIVER_FILES` → `C:\Lavapipe\lvp_icd.json`); macOS skipped.

## Packaging

`.das_package`/`.das_module` already correct (tags graphics/vulkan/gpu, `cmake_build()`, registers `vulkan/vulkan_boost`). Extend `.das_module` to also `register_native_path` the generated `vulkan_ctors`/`vulkan_handles`/`vulkan_structs`/`vulkan_commands` files. After CI is green, `daslang utils/daspkg/main.das -- introduce github.com/borisbat/dasVulkan` (outward-facing — surfaced to Boris to run on his index, like the implot registration).

## Generator polish (task #7)

Add a content-diff skip to the emitters: each generated file is written to a buffer and only replaces the on-disk file when content differs, so the regen → rebuild loop recompiles only what actually changed (today every regen bumps all 51+ mtimes and forces a full ~4-min rebuild). Do this early — it pays for itself across the many boost-emission iterations.

## Build order (each phase independently verifiable)

1. **Generator diff-skip** (task #7) — speeds every subsequent iteration.
2. **sType ctors** (`vulkan_ctors.das`) — smallest, unblocks; verify by rewriting `smoke.das` to use `VkInstanceCreateInfo()`.
3. **Hand-written `vulkan_boost.das` core** — `weak_copy`, `vk_check`, selection/memory helpers, the three block brackets, `raw`/version conveniences.
4. **Ownership** — derivation + override table + `vulkan_handles.das` creators/finalizers; validate on instance/device/image/buffer/etc. round-trips (`test_handles.das`).
5. **vk_view structs** (`vulkan_structs.das`) — start with the structs the three examples touch; the emitter then covers the rest, logging skips for the irregular tail.
6. **Command wrappers** (`vulkan_commands.das`) — same: cover the example surface, let the rest fall out.
7. **Combined builders** (`build_offscreen_target`, `create_graphics_pipeline_simple`, `create_compute_pipeline_simple`).
8. **Examples** — triangle-boost (regression gate vs raw PPM), compute, enumerate.
9. **Tests** — the four dastest files; run locally green via `-load_module`.
10. **CI** — Linux lavapipe workflow; iterate to green.
11. **daspkg introduce** (Boris-run) — after CI green.

## Verification (end to end)

- **Per-phase local**: `daslang generator/generate.das` regenerates without skips-regressions (watch the logged skip count); rebuild `dasModuleVulkan` only if C++ changed (boost is pure-das — no rebuild needed to test boost edits). Run each example via `daslang -load_module D:/DASPKG/dasVulkan examples/<x>.das`.
- **Regression gate**: `offscreen_triangle_boost.das` must emit a byte-identical-shape PPM and pass all 6 structural checks the raw triangle does; diff the two PPMs.
- **Test suite**: `daslang -load_module D:/DASPKG/dasVulkan D:/Work/daScript/dastest/dastest.das -- --test D:/DASPKG/dasVulkan/tests/integration --isolated-mode --isolated-mode-threads 4` → all green, 0 failures/errors, no `GC APP LEAK`.
- **Lint/format gate** (pre-push): MCP `format_file` on every changed `.das`; `daslang -load_module D:/DASPKG/dasVulkan utils/lint/main.das -- <files> --quiet` → `0 issue(s), 0 error(s)`.
- **CI**: GitHub Actions Linux job green — module builds, `vulkaninfo` shows llvmpipe, dastest passes headless under lavapipe.
- **Visual spot-check**: convert the triangle PPM to PNG and eyeball (as this session did).

## Risks

- **The ownership invariant**: wrappers are value types; a plain `=` copy duplicates `_needs_delete=true` → double-destroy. Mitigated by storing raw parent handles (not wrappers) + `weak_copy` for intentional non-owning copies + `var inscope` everywhere + creators returning by `<-`.
- **No GC safety net**: handles are raw pointers; a wrapper made with plain `var x <- create_*()` (no `inscope`) won't finalize → leak. Generated creators carry a doc-comment; examples use `var inscope` uniformly.
- **vk_view recursion correctness** (arrays of structs, nested views) is the hardest emission — validated against the example surface first, irregular tail skipped+logged.
- **Naming hazards generating das-from-das**: reuse `das_safe_name`/`enum_short_name` for every emitted identifier (the `module_` keyword-suffix already bit the raw layer); validate type-named constructor functions, fall back to `mk`-prefix if rejected.
- **`defer` can't do per-iteration teardown in a loop** — bracketing helpers allocate/free internally (already planned).

## Deferred (explicitly out of scope this round)

Docs (vulkan2rst + Sphinx + Pages — revisit once the boost API stabilizes); windowing/surface/swapchain + lifetime-split app state; Windows/macOS CI; typed pNext chains (raw `void?` escape this round); the flags-param refinement (raw layer types flags params as generic `das::Bitfield` — works, aliased bitfields pass — a cosmetic polish).
