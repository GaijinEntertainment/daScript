# dasVulkan — project instructions

Vulkan bindings for [daslang](https://dascript.org/), generated from the Khronos
`vk.xml` registry. Public repo `github.com/borisbat/dasVulkan` (MIT), registered
in the daspkg index. Two layers:

- **`vulkan`** — the raw binding: the full Vulkan API (core + extensions),
  generated as a daslang C++ module dispatching through
  [volk](https://github.com/zeux/volk). Mirrors the C API 1:1.
- **`vulkan_boost`** — the ergonomic layer (pure daslang, no rebuild to edit):
  RAII handle wrappers, idiomatic `array<T>` structs with auto-filled `sType`,
  named/defaulted args, block brackets, windowing.

Follow the daslang **gen2** conventions (the global daslang `CLAUDE.md` rules
apply to every `.das` file). This file captures only the dasVulkan-specific
truths.

## Build & run

The boost layer is pure daslang — **editing `daslib/*.das` needs no rebuild**.
Only C++ or generator changes need the native module rebuilt.

Build the native module (no Vulkan SDK needed — vendored headers + volk runtime
loading cover it):

```
cmake -S . -B _build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDASLANG_DIR=<daslang-root>
cmake --build _build --config Release --parallel 2
```

Output lands at `<repo>/dasModuleVulkan.shared_module` (CMake `LIBRARY_OUTPUT_DIRECTORY`).
Use `--parallel 2`, not unbounded — the ~55 template-heavy generated TUs OOM a
CI runner under unlimited `make -j` (this is why CI bypasses `daspkg install`).

Run an example or tool:

```
daslang -load_module <repo> examples/offscreen_triangle_boost.das
```

**The daslang binary must be a DLL build** (`das_is_dll_build()` true) — the
`.das_module` `initialize` only calls `register_dynamic_module` under that flag.
A non-DLL daslang fails with `error[20605] missing prerequisite 'vulkan'`. On the
dev Windows box that binary is `d:/Work/daScript/bin/Release/daslang.exe`.

## The generator

`generator/*.das` parses `vk.xml` (vendored under `vendor/` at the SDK tag) with
`dasPUGIXML` and emits both layers:

- C++ → `src/*.gen.*` (committed, per the dasGlfw/dasSQLITE convention).
- boost → `daslib/vulkan_*.das`.

`daslang generator/generate.das` regenerates everything; **`--no-cpp` regenerates
only the boost** (the fast iteration loop — no C++ rebuild). `--boost-out`
defaults to `daslib`.

**The skip ratchet.** Full runs also write `generator/skip_report.txt` — every
struct/command the generator could not emit, with its reason, sorted. The
committed copy is the golden baseline: CI re-runs the generator into scratch
dirs and diffs the regenerated report against it, so an emitter change that
silently grows the skip tail (drops generated surface) fails the gate. A
justified change regenerates the report in the same commit. (Full byte-diff of
the generated sources themselves is NOT gated — regen locally and check the
git diff when touching the emitter.)

## Boost file layout (acyclic)

`vulkan_runtime` (hand) ← `vulkan_ctors` (gen) ← `vulkan_handles` (gen) ←
`vulkan_structs` (gen) ← `vulkan_commands` (gen creators) ← `vulkan_cmds` (gen
plain commands) / `vulkan_boost` (hand) / `vulkan_window` (hand). All are
`register_native_path`'d under the `vulkan` prefix in `.das_module`; each file is
`module <name>` + `require vulkan public`.

## Docs

`utils/vulkan2rst.das` (RTTI introspection, modeled on dasImgui's `imgui2rst`)
documents the ergonomic layer into `doc/source/stdlib/generated/*.rst`. Sphinx
builds the site; `.github/workflows/docs.yml` regenerates + builds (`sphinx-build
-W`) + deploys to Pages on master. Published at
https://borisbat.github.io/dasVulkan/.

- `doc/source/stdlib/generated/` is **gitignored** (regenerated in CI so committed
  intros can't drift from code). The hand-filled `stdlib/handmade/module-*.rst`
  intros ARE tracked (vulkan2rst only stubs them `if !stat`).
- The raw `vulkan` binding and the generated `vulkan_structs` (~2000 symbols),
  `vulkan_cmds`, `vulkan_ctors` mirror Vulkan 1:1 and are deliberately **not**
  re-documented — `overview.rst` explains the patterns and points at the spec.
- Doc snippets are not compile-checked — verify field names against the real
  `examples/` before writing one (the boost field names are not what you'd guess
  — see below).

## Tests

`tests/integration/` is in-process dastest (offscreen render to image + pixel
readback; compute to a storage buffer — no window, no subprocess). CI renders on
Mesa lavapipe (software ICD, no GPU). Run:

```
daslang -load_module <repo> <daslang-root>/dastest/dastest.das -- \
  --test <repo>/tests/integration --isolated-mode --isolated-mode-threads 4
```

Run from the repo root so the cwd-relative shader paths resolve. Test bodies must
call `volkInitialize()` themselves.

## Tutorials

Each tutorial is a self-contained unit under `tutorials/<NN_name>/`: the offscreen
module + its `[compute_shader]`/`[shader]` blob + a pixel-oracle `[test]` (the CI
gate) + a `recording/` driver.

**Every rendering tutorial MUST also ship a windowed mode** at
`tutorials/<NN_name>/window/show_<name>.das` — a real GLFW window presenting the
live animation (compute→blit→present, or a swapchain render pass). The offscreen
`[test]` proves correctness; the windowed driver is how a human actually *sees* it
(01_triangle, 02_mandelbrot, 03_sdf all have one). The windowed driver:

- calls `glfwInitVulkanLoader(vk_get_instance_proc_addr())` **before** `glfwInit`
  so GLFW finds the same loader on every platform (see the macOS gotcha below);
- lives in `window/` so the tutorials `.das_test` skips it in CI (the lavapipe CI
  daslang build is `-DDAS_GLFW_DISABLED=ON` — no display, no GLFW).

**Cross-tutorial requires don't work**: daslang `require` can't parse an unquoted
path segment starting with a digit, so `require ../../02_mandelbrot/window/x.das`
fails with `error[30151] unexpected integer constant`. Keep shared helpers
tutorial-local — 02's `mandelbrot_compute` and 03's `resident_compute` are the same
generic resident single-float-pushconstant compute-to-image builder, copied per
tutorial. Factor to a non-digit shared path (e.g. `tutorials/common/`) only if a
third windowed compute tutorial appears.

## Key gotchas / API truths

- **Handles are stored as `uint64` inside wrappers**, not the pointer type.
  Vulkan handles are const-tracked pointers; copying a const handle into a
  non-const struct slot is `error[30915]`. `uint64` (their ABI form) copies
  friction-free; `reinterpret` at the C boundary. This is the systemic fix for
  all const-pointer-copy pain.
- **Ownership:** declare every owner `var inscope` so `finalize` destroys it in
  reverse order. A plain `var x <- create_*()` leaks (handles are raw pointers,
  no GC safety net). Parents are stored as raw handles inside wrappers, never as
  nested wrappers. `weak_copy(x)` makes an intentional non-owning alias (clears
  `_needs_delete`).
- **Boost view-struct field names keep the C spelling** — `renderPass`,
  `pAttachments`, `queueFamilyIndex` (camelCase + Hungarian `p`), NOT
  `render_pass`/`attachments`. `pNext` → `next : void?` (raw escape hatch).
  Stripping the `p` and typed pNext chains are deferred (see `ROADMAP.md`).
- **Filling a CreateInfo view:** use the named-argument constructor —
  `Foo(scalar = x, handle = weak_copy(h), arr <- [..])`. Handle fields take a
  `weak_copy` (a non-owning alias; the `create_*` keeps ownership); non-copyable
  array fields are move-initialized with `<-`. **Two field kinds cannot go in the
  ctor** and stay as field-assigns *after* it: (1) **nested raw `Vk*` struct
  fields** — `extent`, `subresourceRange`, `imageSubresource` are the native
  `VkExtent3D`/`VkImageSubresourceRange`/… not boost wrappers, so a nested
  `Extent3D(..)` ctor fails `error[30915]`; write `ci.extent.width = ..` after;
  (2) **bitfield fields** — `usage.transfer_dst = true`, `samples._1 = true`,
  `aspectMask.color = true` (bitfields have no named-arg ctor). A non-empty ctor
  init silences STYLE013 for those residual assigns, so the lint stays clean.
  Field-by-field-only (empty/default init) trips STYLE013 — the linter is a CI
  gate now (see below), so always lead with the ctor.
- **Count fields are mostly auto-derived** from array length. The exceptions
  (optional / `noautovalidity` arrays, e.g. `descriptorCount` without samplers)
  are settable boost fields under the independent-count model: the view sets
  `count != 0 ? count : max(referencing-array lengths)`.
- **Raw layer out-params:** single out-handle (no `len`) is by-ref (pass `var h`);
  array out-handle (has `len`, even count 1) is a double-pointer (pass
  `addr(h)`). The boost creators/commands hide this.
- **Block trailing syntax** is `f(...) $(cmd) { ... }` or `f(...) { ... }` — NO
  `<|` (STYLE001).
- **macOS works via MoltenVK** (no opt-in). One-time host setup:
  `brew install molten-vk vulkan-loader vulkan-tools`. `vk_surface_from_native`
  has a Metal arm (`src/dasVULKAN.metal.mm`, `vkCreateMetalSurfaceEXT` from a
  `CAMetalLayer`); `das_volkInitialize` finds the loader that volk's built-in
  macOS search misses, by dlopen'ing the Homebrew/SDK paths; `create_instance`
  auto-enables `VK_KHR_portability_enumeration`; and every boost-layer device
  creator auto-enables `VK_KHR_portability_subset` when the device advertises
  it (the spec requires enabling it, or `vkCreateDevice` fails). Raw
  `vkCreateDevice` call sites (some examples/tutorials) must append it
  themselves on portability devices. Windowed apps call
  `glfwInitVulkanLoader(vk_get_instance_proc_addr())` before `glfwInit`. See
  `ROADMAP.md`.
- **Commit messages:** the Bash-tool shell wrapper eval's backticks in a
  `git commit -F -` heredoc body. Write the message to a temp file and
  `git commit -F file`.

## Workflow

The whole initial build (raw binding → boost → examples → tests → Linux CI →
array-of-struct → resizable swapchain → independent-count model → docs) was
committed **direct to master**. That phase is over: **new work goes through PRs**.

**Lint is mandatory and gated.** `.github/workflows/tests.yml` runs daslang's
`utils/lint/main.das` (paranoid + perf + style) over the whole tree on every PR;
any warning or compile error fails CI. The whole tree is lint-clean — keep it
that way. Mirror the gate locally before pushing:

```
git config core.hooksPath utils/git-hooks   # one-time
DASLANG=<daslang binary> DASLANG_ROOT=<daslang checkout> git push
```

(`utils/git-hooks/pre-push` runs the same lint; a full dev build also covers the
`window/` + `recording/` drivers, which the headless CI build skips.) To lint by
hand: `daslang -load_module <repo> <daslang>/utils/lint/main.das -- <repo> -q`.
The **generated** `daslib/vulkan_*.das` files are lint-clean *by construction* —
fix the emitter in `generator/vk_emit_boost.das` and regenerate, never hand-edit.

- `ROADMAP.md` — postponed work, with enough context to pick each up cold.
- `ORIGINAL_PLAN.md` — the original boost-layer design plan, preserved verbatim
  for history (most of it is now implemented).
