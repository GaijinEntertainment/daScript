# dasVulkan module instructions

dasVulkan is the daslang binding + ergonomic boost layer for [Vulkan](https://www.vulkan.org/), generated from the Khronos `vk.xml` registry, **in-tree at `modules/dasVulkan/`** and built by default (root CMake option `DAS_VULKAN_DISABLED`, default `OFF`; headers + volk are vendored, so no Vulkan SDK is needed to build). Two layers:

- **`vulkan`** — the raw binding: the full Vulkan API (core + extensions), generated as a daslang C++ module dispatching through [volk](https://github.com/zeux/volk). Mirrors the C API 1:1.
- **`vulkan_boost`** — the ergonomic layer (pure daslang, no rebuild to edit): RAII handle wrappers, idiomatic `array<T>` structs with auto-filled `sType`, named/defaulted args, block brackets, windowing.

The old standalone repo (borisbat/dasVulkan) is archived with full history. `daspkg` recognizes `require_package("dasVulkan")` as in-tree and reports *part of this daslang tree — nothing to install*; in-repo example `.das_package` manifests do NOT declare it.

Follow the daslang **gen2** conventions (the root `CLAUDE.md` rules apply to every `.das` file). This file captures only the dasVulkan-specific truths.

## Locations

- Module source: `modules/dasVulkan/` (`src/`, `daslib/`, `generator/`, `examples/`, `tutorials/`, `utils/`, `vendor/`)
- Tests: `modules/dasVulkan/tests/integration/` — nightly CI lane `.github/workflows/nightly_vulkan.yml`
- Docs: stdlib section + generated pages in the main Sphinx tree; tutorials at `doc/source/reference/tutorials/vulkan/`
- Recordings: intermediates under `tutorials/**/recording/` (gitignored); MP4 deliverables on the rolling `docs-assets` GitHub release

## Skill files (REQUIRED)

| Skill file | Read BEFORE... |
|---|---|
| `skills/internal/vulkan_recording.md` | Writing or revising any `tutorials/*/recording/record_*.das` driver |
| `skills/perf_lint.md` / `skills/style_lint.md` | Suppressing any lint finding here — the module is lint-clean and gated |
| `skills/daslang_live.md` | Touching `daslib/vulkan_live.das` (the live-command mirror of `opengl_live`) |

## Build & run

The boost layer is pure daslang — **editing `daslib/*.das` needs no rebuild**. Only C++ or generator changes need the native module rebuilt, and then it is the ordinary in-tree build:

```
cmake --build build --config Release
```

Both halves come out of that one build: `libDasModuleVulkan` (static, for `daslang_static` and embedders) and the `dasModuleVulkan.shared_module` twin the default dynamic `daslang` / `daslang-live` host loads via `.das_module`. Run anything from the repo root:

```
bin/Release/daslang -project_root . modules/dasVulkan/examples/offscreen_triangle_boost.das
```

**Build the shared twin at `--parallel 2`, not unbounded** — the ~55 template-heavy generated TUs OOM a 7 GB CI runner under full parallelism. Both dasVulkan CI lanes do exactly this (`cmake --build ./build --parallel 2 --target dasModuleVulkan`).

## The generator

`generator/*.das` parses `vk.xml` (vendored under `vendor/` at the SDK tag) with `dasPUGIXML` and emits both layers:

- C++ → `src/*.gen.*` (committed, per the dasGlfw/dasSQLITE convention).
- boost → `daslib/vulkan_*.das`.

`daslang generator/generate.das` regenerates everything; **`--no-cpp` regenerates only the boost** (the fast iteration loop — no C++ rebuild). `--boost-out` defaults to `daslib`.

**The skip ratchet.** Full runs also write `generator/skip_report.txt` — every struct/command the generator could not emit, with its reason, sorted. The committed copy is the golden baseline: CI re-runs the generator into scratch dirs and diffs the regenerated report against it, so an emitter change that silently grows the skip tail (drops generated surface) fails the gate. A justified change regenerates the report in the same commit. (Full byte-diff of the generated sources themselves is NOT gated — regen locally and check the git diff when touching the emitter.)

## Boost file layout (acyclic)

`vulkan_runtime` (hand) ← `vulkan_ctors` (gen) ← `vulkan_handles` (gen) ← `vulkan_structs` (gen) ← `vulkan_commands` (gen creators) ← `vulkan_cmds` (gen plain commands) / `vulkan_boost` (hand) / `vulkan_window` (hand). Each file is `module <name>` + `require vulkan public`.

**Every daslib file has TWO registration points**, and they must stay in sync: `ADD_MODULE_DAS(vulkan daslib <name>)` in `CMakeLists.txt` (the static in-tree build) and a `<name>` entry in the `boost_paths` list in `.das_module` (the DLL / daspkg flow). A file registered in only one resolves in only one build flavor.

## Docs

`utils/vulkan2rst.das` (RTTI introspection, modeled on dasImgui's `imgui2rst`) documents the ergonomic layer into the main Sphinx tree's generated stdlib pages; the hand-filled module intros are tracked, the generated pages are not. Tutorial pages live at `doc/source/reference/tutorials/vulkan/`.

- A new public `vulkan_boost` / `vulkan_window` helper must land in a `group_by_regex` group in `vulkan2rst.das` **in the same change**, or it is silently undocumented. A type it exposes that the boost layer does not own needs a label in `doc/source/stdlib/vulkan_external_types.rst` (repo root), or the `:ref:` dangles and the `-W` docs build goes red.
- The raw `vulkan` binding and the generated `vulkan_structs` (~2000 symbols), `vulkan_cmds`, `vulkan_ctors` mirror Vulkan 1:1 and are deliberately **not** re-documented — the overview page explains the patterns and points at the spec.
- Doc snippets are not compile-checked — verify field names against the real `examples/` before writing one (the boost field names are not what you'd guess — see below).

## Tests

`tests/integration/` is in-process dastest (offscreen render to image + pixel readback; compute to a storage buffer — no window, no subprocess). CI renders on Mesa lavapipe (software ICD, no GPU). Run from the repo root:

```
bin/Release/daslang -project_root . dastest/dastest.das -- \
  --test modules/dasVulkan/tests/integration --isolated-mode --isolated-mode-threads 4
```

Run from the repo root so the cwd-relative shader paths resolve. Test bodies must call `volkInitialize()` themselves.

## CI

- **`.github/workflows/vulkan_checks.yml`** — the per-PR gate, paths-filtered to `modules/dasVulkan/**` so unrelated PRs pay nothing. Two cheap correctness gates: the generator skip ratchet and the module-wide lint.
- **`.github/workflows/nightly_vulkan.yml`** — the render suite: full integration + tutorial pixel-oracle tests on lavapipe (Linux) plus a build + loader-discovery smoke on macOS. Nightly and on-demand only. Windows has no lane (needs lavapipe/SwiftShader wiring — see `ROADMAP.md`).

**Lint is mandatory and gated.** The whole module is lint-clean — keep it that way. The **generated** `daslib/vulkan_*.das` files are lint-clean *by construction*: fix the emitter in `generator/vk_emit_boost.das` and regenerate, never hand-edit.

## Tutorials

Each tutorial is a self-contained unit under `tutorials/<NN_name>/`: the offscreen module + its `[compute_shader]`/`[shader]` blob + a pixel-oracle `[test]` (the CI gate) + a `recording/` driver.

**Every rendering tutorial MUST also ship a windowed mode** at `tutorials/<NN_name>/window/show_<name>.das` — a real GLFW window presenting the live animation (compute→blit→present, or a swapchain render pass). The offscreen `[test]` proves correctness; the windowed driver is how a human actually *sees* it. The windowed driver:

- calls `glfwInitVulkanLoader(vk_get_instance_proc_addr())` **before** `glfwInit` so GLFW finds the same loader on every platform (see the macOS gotcha below);
- lives in `window/` so the tutorials `.das_test` skips it in CI (the lavapipe CI daslang build is `-DDAS_GLFW_DISABLED=ON` — no display, no GLFW).

**Cross-tutorial requires don't work**: daslang `require` can't parse an unquoted path segment starting with a digit, so `require ../../02_mandelbrot/window/x.das` fails with `error[30151] unexpected integer constant`. Keep shared helpers tutorial-local — 02's `mandelbrot_compute` and 03's `resident_compute` are the same generic resident single-float-pushconstant compute-to-image builder, copied per tutorial. Factor to a non-digit shared path (e.g. `tutorials/common/`) only if a third windowed compute tutorial appears.

## Key gotchas / API truths

- **Handles are stored as `uint64` inside wrappers**, not the pointer type. Vulkan handles are const-tracked pointers; copying a const handle into a non-const struct slot is `error[30915]`. `uint64` (their ABI form) copies friction-free; `reinterpret` at the C boundary. This is the systemic fix for all const-pointer-copy pain.
- **Ownership:** declare every owner `var inscope` so `finalize` destroys it in reverse order. A plain `var x <- create_*()` leaks (handles are raw pointers, no GC safety net). Parents are stored as raw handles inside wrappers, never as nested wrappers. `weak_copy(x)` makes an intentional non-owning alias (clears `_needs_delete`).
- **Boost view-struct field names keep the C spelling** — `renderPass`, `pAttachments`, `queueFamilyIndex` (camelCase + Hungarian `p`), NOT `render_pass`/`attachments`. `pNext` → `next : void?` (raw escape hatch). Stripping the `p` and typed pNext chains are deferred (see `ROADMAP.md`).
- **Filling a CreateInfo view:** use the named-argument constructor — `Foo(scalar = x, handle = weak_copy(h), arr <- [..])`. Handle fields take a `weak_copy` (a non-owning alias; the `create_*` keeps ownership); non-copyable array fields are move-initialized with `<-`. **Two field kinds cannot go in the ctor** and stay as field-assigns *after* it: (1) **nested raw `Vk*` struct fields** — `extent`, `subresourceRange`, `imageSubresource` are the native `VkExtent3D`/`VkImageSubresourceRange`/… not boost wrappers, so a nested `Extent3D(..)` ctor fails `error[30915]`; write `ci.extent.width = ..` after; (2) **bitfield fields** — `usage.transfer_dst = true`, `samples._1 = true`, `aspectMask.color = true` (bitfields have no named-arg ctor). A non-empty ctor init silences STYLE013 for those residual assigns, so the lint stays clean. Field-by-field-only (empty/default init) trips STYLE013 — the linter is a CI gate, so always lead with the ctor.
- **Count fields are mostly auto-derived** from array length. The exceptions (optional / `noautovalidity` arrays, e.g. `descriptorCount` without samplers) are settable boost fields under the independent-count model: the view sets `count != 0 ? count : max(referencing-array lengths)`.
- **Raw layer out-params:** single out-handle (no `len`) is by-ref (pass `var h`); array out-handle (has `len`, even count 1) is a double-pointer (pass `addr(h)`). The boost creators/commands hide this.
- **Block trailing syntax** is `f(...) $(cmd) { ... }` or `f(...) { ... }` — NO `<|` (STYLE001).
- **macOS works via MoltenVK** (no opt-in). One-time host setup: `brew install molten-vk vulkan-loader vulkan-tools`. `vk_surface_from_native` has a Metal arm (`src/dasVULKAN.metal.mm`, `vkCreateMetalSurfaceEXT` from a `CAMetalLayer`); `das_volkInitialize` finds the loader that volk's built-in macOS search misses, by dlopen'ing the Homebrew/SDK paths; `create_instance` auto-enables `VK_KHR_portability_enumeration`; and every boost-layer device creator auto-enables `VK_KHR_portability_subset` when the device advertises it (the spec requires enabling it, or `vkCreateDevice` fails). Raw `vkCreateDevice` call sites (some examples/tutorials) must append it themselves on portability devices. Windowed apps call `glfwInitVulkanLoader(vk_get_instance_proc_addr())` before `glfwInit`. See `ROADMAP.md`.

## Workflow

Work here goes through the repo's own PR flow (`skills/internal/make_pr.md`) and its gates; there is no module-local hook or lint recipe. `ROADMAP.md` holds postponed work, with enough context to pick each item up cold; the original boost-layer design plan is archived at `history/dasVulkan/ORIGINAL_PLAN.md`.
