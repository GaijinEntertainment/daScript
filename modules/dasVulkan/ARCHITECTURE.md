# dasVulkan - the Vulkan binding and its boost layer

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklist that binds a diff here is `REVIEW.md` (this folder); `generator/` and `tutorials/`
carry their own. Planned work: `ROADMAP.md`. Agent instructions - build, run, test, where
things live: `CLAUDE.md`. The original boost-layer design plan is archived at
`history/dasVulkan/ORIGINAL_PLAN.md`.

## 1. The two layers

dasVulkan binds [Vulkan](https://www.vulkan.org/) from the Khronos `vk.xml` registry and lives
in-tree at `modules/dasVulkan/`. It builds by default: the root CMake option
`DAS_VULKAN_DISABLED` defaults to `OFF`, and the headers plus volk are vendored, so building
needs no Vulkan SDK.

- **`vulkan`** - the raw binding: the full API, core and extensions, generated as a daslang
  C++ module dispatching through [volk](https://github.com/zeux/volk). It mirrors the C API
  1:1.
- **`vulkan_boost`** - the ergonomic layer, pure daslang: RAII handle wrappers, view structs
  taking `array<T>` with auto-filled `sType`, named and defaulted arguments, block brackets,
  and windowing.

The split is what keeps the ergonomic half soft. Everything hard - ABI, dispatch, extension
loading - is generated C++ behind a rebuild; everything ergonomic is `.das` a user reads and
edits in place, with no rebuild at all.

One build produces both halves: `libDasModuleVulkan` (static, for `daslang_static` and
embedders) and the `dasModuleVulkan.shared_module` twin that the dynamic `daslang` /
`daslang-live` host loads through `.das_module`. The shared twin has ~55 generated translation
units that are template-heavy enough to exhaust a 7 GB CI runner at full parallelism, so every
lane builds that one target at `--parallel 2`.

`daspkg` treats `require_package("dasVulkan")` as in-tree and reports *part of this daslang
tree - nothing to install*, so in-repo example `.das_package` manifests do not declare it.

## 2. The generator and the skip ratchet

`generator/*.das` parses the `vk.xml` vendored under `vendor/` at its SDK tag, using
`dasPUGIXML`, and emits both layers: C++ into `src/*.gen.*` and boost `.das` into
`daslib/vulkan_*.das`. Both are committed, following the dasGlfw and dasSQLITE convention, so
a checkout builds without running the generator.

A full run also writes `generator/skip_report.txt`: every struct and command the emitter could
not handle, with its reason, sorted. The committed copy is the golden baseline. CI regenerates
into scratch directories and diffs the fresh report against it, so an emitter change that
quietly grows the skip tail - dropping bound surface - turns the lane red. The generated
sources themselves are not byte-diffed, because their churn is large and uninteresting; the
skip tail is the part whose growth is a loss.

The generated files are lint-clean by construction: the emitter writes conforming code, so a
lint finding in one of them points at the emitter rather than at the file.

## 3. Boost file layout

The boost `.das` files form an acyclic graph. `vulkan_runtime` (hand-written) and
`vulkan_ctors` (generated) are the roots; `vulkan_handles` sits on `vulkan_runtime`;
`vulkan_structs` on `vulkan_ctors` and `vulkan_handles`; `vulkan_commands` (generated
creators) and `vulkan_cmds` (generated plain commands) on `vulkan_handles` and
`vulkan_structs`; `vulkan_boost` (hand-written) requires all of them and re-exports them;
`vulkan_window` (hand-written) sits on `vulkan_boost`. Every file declares `module <name>` and
`require vulkan public`.

`vulkan_runtime` exists so the generated files can reach `vk_check`, `array_addr` and
`weak_copy` without a cycle: `vulkan_boost` re-exports the generated files, so it sits above
them and cannot also sit below them.

A daslib file registers in exactly one place - the `boost_paths` list in `.das_module`. CMake
derives the compiled-in resolver rows from that descriptor at configure time
(`ADD_MODULE_DAS_FROM_DESCRIPTOR(vulkan daslib)` in `CMakeLists.txt`), and `REVIEW.das`
censuses the descriptor against the folder, so a file the list misses and a listed name whose
file is gone both fail review.

## 4. Handles are stored as uint64 {#handles-uint64}

A boost wrapper stores its Vulkan handle as `uint64`, not as the handle pointer type, and
reinterprets at the C boundary. Vulkan handles are const-tracked pointers, so copying a const
handle into a non-const struct slot raises `error[30915]`; `uint64` is their ABI form and
copies without friction. This is the systemic fix for every const-pointer-copy problem in the
layer, not a local workaround.

A wrapper holds its parent the same way - a raw `uint64` field such as `Buffer._device` -
never as a nested wrapper, so a child never owns its parent.

## 5. Ownership {#ownership}

A boost wrapper carries `_needs_delete`, and its generated `finalize` destroys the handle only
when that flag is set. Ownership travels with the value and the scope machinery does the
freeing; there is no GC safety net behind a Vulkan handle, because a handle is a raw pointer.

An owner is declared `var inscope`, which runs `finalize` at scope exit in reverse declaration
order - the order Vulkan requires, children before parents. A plain `var x <- create_*()`
leaks.

`weak_copy(x)` is the intentional non-owning alias: it copies the wrapper and clears
`_needs_delete`, so only the original frees. The copy is a plain struct copy because a wrapper
stores its handle as `uint64` (sec.4). Handle fields inside a view struct take a `weak_copy`,
because the `create_*` that produced the handle keeps ownership.

A composite struct that owns several handles - `OffscreenTarget`, `OffscreenDepth`,
`HostBuffer`, `AddressBuffer`, `ShaderBindingTable`, `AccelStructure`, `Swapchain`,
`FrameSync` - carries a hand-written `finalize` that deletes its members in reverse dependency
order, so the composite behaves like a single owner to whoever holds it.

## 6. Filling a boost view struct

Boost view-struct fields keep the C spelling - `renderPass`, `pAttachments`,
`queueFamilyIndex`: camelCase with the Hungarian `p`, not `render_pass` or `attachments`. The
generated marshalling maps boost field to raw `Vk*` field by position, so the names are free to
change, but changing them is a churning public-API rename entangled with two other decisions
(`ROADMAP.md`). `pNext` is the one renamed field: `next : void?`, a raw escape hatch.

A CreateInfo view is filled through its named-argument constructor -
`Foo(scalar = x, handle = weak_copy(h), arr <- [..])`. Non-copyable array fields are
move-initialized with `<-`.

Two field kinds cannot go in that constructor and are assigned after it:

- **Nested raw `Vk*` struct fields.** `extent`, `subresourceRange` and `imageSubresource` are
  the native `VkExtent3D` / `VkImageSubresourceRange` types, not boost wrappers, so a nested
  `Extent3D(..)` constructor fails `error[30915]`. Write `ci.extent.width = ..` after.
- **Bitfield fields.** `usage.transfer_dst = true`, `samples._1 = true`,
  `aspectMask.color = true` - a bitfield has no named-argument constructor.

Leading with a non-empty constructor is what keeps those residual assignments lint-clean:
STYLE013 fires on field-by-field filling after an empty or default init, and not after a
constructor that already initialized something.

## 7. Count fields

Count fields are derived from array length and are not part of the public surface. The
exceptions are the optional and `noautovalidity` arrays - `descriptorCount` without samplers is
the standing case - which stay settable boost fields under the independent-count model: the
view emits `count != 0 ? count : max(length of the referencing arrays)`. An explicit non-zero
count therefore wins, and zero means "derive it".

## 8. Raw-layer out-parameters

In the raw `vulkan` binding a single out-handle - a command with no `len` parameter - is
by-reference: pass `var h`. An array out-handle - a command that has a `len`, even when the
count is 1 - is a double pointer: pass `addr(h)`. The boost creators and commands hide this
distinction; it is visible only when calling the raw layer directly.

## 9. Portability: macOS and MoltenVK {#portability-subset}

macOS works through MoltenVK with no opt-in. The host needs one setup step,
`brew install molten-vk vulkan-loader vulkan-tools`; four pieces then make it work, all of them
platform-agnostic in the code:

- **Loader discovery.** `das_volkInitialize` (`src/dasVULKAN.main.cpp`, `__APPLE__` branch)
  falls back to dlopen of the loader from `$VULKAN_SDK` or the Homebrew prefix when the
  built-in volk search misses it, then wires volk through `volkInitializeCustom`.
- **Instance portability.** A portability driver rejects `vkCreateInstance` with
  `ERROR_INCOMPATIBLE_DRIVER` unless `VK_KHR_portability_enumeration` is enabled and the
  matching create flag is set. `create_instance` adds both when the loader advertises the
  extension, so the same user code runs unchanged everywhere.
- **Device portability.** The spec requires enabling `VK_KHR_portability_subset` on any device
  that advertises it, or `vkCreateDevice` fails. `append_portability_subset` appends it, and
  every boost device creator routes through that one helper.
- **Metal surface.** `vk_surface_from_native` has a Metal arm that creates a `VkSurfaceKHR`
  from a `CAMetalLayer` through `vkCreateMetalSurfaceEXT`; the Cocoa and QuartzCore code is
  isolated to `src/dasVULKAN.metal.mm`.

A windowed application calls `glfwInitVulkanLoader(vk_get_instance_proc_addr())` before
`glfwInit`, so GLFW finds the same loader the module found. dasGlfw binds that call.

Code that calls raw `vkCreateDevice` instead of a boost creator gets none of this: nothing
appends the extension on its behalf.

## 10. The 8/16-bit storage set {#storage-8-16}

`shaderInt16` is a core `VkPhysicalDeviceFeatures` bit, set on `f2.features` directly - it is
not in the Vulkan11 or Vulkan12 chain that carries `storageBuffer16BitAccess`,
`storageBuffer8BitAccess`, `shaderFloat16` and `shaderInt8`. It belongs with them all the same:
a shader declaring an `int16` SSBO member pulls the SPIR-V `Int16` capability, which needs
`shaderInt16`. Those five bits are one set, and the layer treats them as one.

`storage_8_16_supported` probes the whole set. Each `create_device_storage_8_16*` creator
enables every bit that probe reads, threading `VkPhysicalDeviceVulkan11Features` into
`VkPhysicalDeviceVulkan12Features` through the features2 overload and setting `shaderInt16` on
the core block. `cooperative_matrix2_supported` probes the same core `shaderInt16` bit, because
the cm2 device its name gates is a superset of this one.

The probe and the creators are two halves of one claim. A bit that one side names and the other
misses either fails `vkCreateDevice` or lets a kernel use a feature the device never enabled.

## 11. The Vulkan 1.3 subgroup pair {#subgroup-pair}

`subgroupSizeControl` and `computeFullSubgroups` are a pair: a compute pipeline may set
`REQUIRE_FULL_SUBGROUPS` only on a device where both are enabled.
`compute_full_subgroups_supported` reports the pair, returning false below API 1.3 before it
reads any feature bit, because the struct carrying them is 1.3 core.

Every optional feature block joins a creator's pNext chain only when its extension is enabled
and the device reports the bit: a struct for an extension the device did not enable fails
`vkCreateDevice`, so each creator queries first and chains second.

Every ladder creator that can enable the pair does so opportunistically: it queries the pair
first, and chains a `VkPhysicalDeviceVulkan13Features` onto the tail of its pNext chain only
when the device reports both bits, after whichever optional struct that creator chained last. A
device missing the pair degrades to the same device without it rather than failing to create.
That tail position is why the chaining is a per-creator ladder: only the creator knows which of
its own optional blocks is last. `create_device_storage_8_16_int_dot_coopmat2` runs one rung
past the pair: when the device reports `VK_NV_cooperative_matrix_decode_vector` it chains
`VkPhysicalDeviceCooperativeMatrixDecodeVectorFeaturesNV` after the 1.3 block, or after
whichever optional block is last when the pair is missing. That rung is the creator's tail, so
the next optional block chains after it. `create_device_coopmat_full_subgroups`, which predates the
ladder, is the other shape: it chains its `VkPhysicalDeviceVulkan13Features` unconditionally in
mid-chain and copies the reported bits into it - a 1.3 core struct, so the chain is legal
either way, and a missing pair leaves the bits off.

`create_compute_pipeline_full_subgroups` is the consumer: `create_compute_pipeline` with
`REQUIRE_FULL_SUBGROUPS` set on the stage, which lets the shader compiler drop its
partial-subgroup guards. A pipeline it returns is valid only on a device from a creator that
enabled the pair, and only for a workgroup width that is a multiple of the subgroup size.

## 12. Documentation pipeline

`utils/vulkan2rst.das` documents the ergonomic layer into the generated stdlib pages of the
main Sphinx tree by RTTI introspection, modeled on the `imgui2rst` of dasImgui. The hand-filled
module intros are tracked; the generated pages are not. Tutorial pages live at
`doc/source/reference/tutorials/vulkan/`.

A helper reaches a page only through a `group_by_regex` group in `vulkan2rst.das`. A helper in
no group is emitted nowhere and nothing turns red - this is the one part of the pipeline with
no gate behind it, which is why `REVIEW.md` carries the duty.

A generated page emits a `:ref:` for every type it mentions. A type the boost layer does not
own needs a label in `doc/source/stdlib/vulkan_external_types.rst` (repo root) or that `:ref:`
dangles; the `-W` Sphinx build in `doc.yml` is paths-filtered on `modules/dasVulkan/**`, so it
catches a missing label per PR.

The raw `vulkan` binding and the generated `vulkan_structs` (~2000 symbols), `vulkan_cmds` and
`vulkan_ctors` mirror Vulkan 1:1 and are deliberately not re-documented: the overview page
explains the patterns and points at the spec, which stays correct as the registry grows.

Doc snippets are not compile-checked.

## 13. Tutorial units

A tutorial is a self-contained unit under `tutorials/<NN_name>/`: an offscreen module, its
`[compute_shader]` or `[shader]` blob, a pixel-oracle `[test]` that CI gates, and a
`recording/` driver. Its windowed viewer lives at `<NN_name>/window/show_<name>.das`, which is
where the tutorials `.das_test` skips it - the lavapipe CI daslang build is
`-DDAS_GLFW_DISABLED=ON`, so it has no display and no GLFW.

Shared helpers are copied per tutorial rather than factored out, because daslang `require`
cannot parse an unquoted path segment starting with a digit:
`require ../../02_mandelbrot/window/x.das` fails with `error[30151] unexpected integer
constant`. The `mandelbrot_compute` of 02 and the `resident_compute` of 03 are the same generic
resident single-float-pushconstant compute-to-image builder, held as two copies for that
reason. A non-digit shared path such as `tutorials/common/` is what a third windowed compute
tutorial would need.

## 14. CI gates

- `.github/workflows/vulkan_checks.yml` - the per-PR gate, paths-filtered to
  `modules/dasVulkan/**` so an unrelated PR pays nothing. It carries the two cheap correctness
  gates: the generator skip ratchet, and the module-wide lint over every `.das` here.
- `.github/workflows/nightly_vulkan.yml` - the render suite. Full integration plus tutorial
  pixel-oracle tests on Mesa lavapipe (a software ICD, no GPU) on Linux, plus a build and
  loader-discovery smoke on macOS. Nightly and on demand only.

Windows has no lane: it needs a software ICD wired up (`ROADMAP.md`). The macOS lane is a build
and smoke gate rather than a render gate, because GitHub-hosted macOS runners expose only a
paravirtualized GPU that MoltenVK cannot render the suite on.

`tests/integration/` is in-process dastest - offscreen render to an image with pixel readback,
and compute into a storage buffer. No window, no subprocess. A test body calls
`volkInitialize()` itself, because nothing in the harness does it.

## 15. Exception ledger

Empty. No rule in `REVIEW.md` has a ruled-acceptable case here yet.
