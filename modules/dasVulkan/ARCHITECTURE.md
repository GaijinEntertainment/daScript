# dasVulkan - the Vulkan binding and its boost layer

**Read `ARCHITECTURE_COMMON.md` (repo root) first - its contract binds this document.** The
checklist that binds a diff here is `REVIEW.md` (this folder); `generator/` and `tutorials/`
carry their own; how a frame is drawn and synchronized: the companion `ARCHITECTURE_RENDERING.md`; the device a process's contexts share: `ARCHITECTURE_SHARED_DEVICE.md`; the feature sets a compute device is created with and the pipeline creators that consume them: `ARCHITECTURE_DEVICE_FEATURES.md`. Planned work:
`ROADMAP.md`. Agent instructions - build, run, test, where things live: `CLAUDE.md`.

## The two layers {#two-layers}

dasVulkan binds [Vulkan](https://www.vulkan.org/) from the Khronos `vk.xml` registry and lives
in-tree at `modules/dasVulkan/`. It builds by default on Linux and Windows: the root CMake option
`DAS_VULKAN_DISABLED` defaults to `OFF` there and to `ON` on Apple (`ARCHITECTURE.md#portability-subset`), and the headers plus
volk are vendored, so building needs no Vulkan SDK.

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

## The generator and the skip ratchet

`generator/*.das` parses the `vk.xml` vendored under `vendor/` at its SDK tag, using
`dasPUGIXML`, and emits both layers: C++ into `src/*.gen.*` and boost `.das` into
`daslib/vulkan_*.das`. Both are committed, following the dasGlfw and dasSQLITE convention, so
a checkout builds without running the generator.

A full run also writes `generator/skip_report.txt` - every struct and command the emitter could
not handle, with its reason, sorted - and `generator/copyability_report.txt`, the boost structs
a caller may not reuse by value because an array literal moves them. The committed copies are
the golden baseline. CI (`generator/check_generated.cmake`) regenerates into a scratch directory
and diffs both reports and every generated source against the committed files, so an emitter
change that quietly grows the skip tail - dropping bound surface - or leaves stale output in
the tree turns the lane red.

The generated files are lint-clean by construction: the emitter writes conforming code, so a
lint finding in one of them points at the emitter rather than at the file.

## Boost file layout

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

## Handles are stored as uint64 {#handles-uint64}

A boost wrapper stores its Vulkan handle as `uint64`, not as the handle pointer type, and
reinterprets at the C boundary. Vulkan handles are const-tracked pointers, so copying a const
handle into a non-const struct slot raises `error[30915]`; `uint64` is their ABI form and
copies without friction. This is the systemic fix for every const-pointer-copy problem in the
layer, not a local workaround.

A wrapper holds its parent the same way - a raw `uint64` field such as `Buffer._device` -
never as a nested wrapper, so a child never owns its parent.

## Ownership {#ownership}

A boost wrapper carries `_needs_delete`, and its generated `finalize` destroys the handle only
when that flag is set. Ownership travels with the value and the scope machinery does the
freeing; there is no GC safety net behind a Vulkan handle, because a handle is a raw pointer.

An owner is declared `var inscope`, which runs `finalize` at scope exit in reverse declaration
order - the order Vulkan requires, children before parents. A plain `var x <- create_*()`
leaks.

`weak_copy(x)` is the intentional non-owning alias: it copies the wrapper and clears
`_needs_delete`, so only the original frees. The copy is a plain struct copy because a wrapper
stores its handle as `uint64` (`ARCHITECTURE.md#handles-uint64`). Handle fields inside a view struct take a `weak_copy`,
because the `create_*` that produced the handle keeps ownership.

A composite struct that owns several handles - `OffscreenTarget`, `OffscreenDepth`,
`HostBuffer`, `AddressBuffer`, `ShaderBindingTable`, `AccelStructure`, `Swapchain`,
`FrameSync` - carries a hand-written `finalize` that deletes its members in reverse dependency
order, so the composite behaves like a single owner to whoever holds it.

## Filling a boost view struct

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

## Host-address fields {#host-address-fields}

Every `void?` field other than `next` - `pHostPointer`, `pPlacedAddress`, a
`HostAddressRangeEXT.address`, a `pUserData` - is a host address the caller owns: a mapped
range, an imported allocation, an opaque blob handed back to the caller. The view copies the
pointer, never the bytes, so whatever the pointer names stays the caller's to keep alive for as
long as the driver may touch it. The generator emits that field for a bare `void*` member with
no `len`, and for a non-const `void*` with a `len`: the caller sizes that range by hand, so the
member the `len` names stays a plain settable field instead of a count derived from an array.
A `const void*` with a byte-count `len` stays an `array<uint8>` the view copies.

Two required arrays that share one count member are asserted equal in length by the view and
by the command wrapper, which otherwise take the count from the first required array - a
shorter second array would let the driver read past it.

## Count fields

Count fields are derived from array length and are not part of the public surface. The
exceptions are the optional and `noautovalidity` arrays - `descriptorCount` without samplers is
the standing case - which stay settable boost fields under the independent-count model: the
view emits `count != 0 ? count : max(length of the referencing arrays)`. An explicit non-zero
count therefore wins, and zero means "derive it".

## Raw-layer out-parameters

In the raw `vulkan` binding a single out-handle - a command with no `len` parameter - is
by-reference: pass `var h`. An array out-handle - a command that has a `len`, even when the
count is 1 - is a double pointer: pass `addr(h)`. The boost creators and commands hide this
distinction; it is visible only when calling the raw layer directly.

## Portability: macOS and MoltenVK {#portability-subset}

macOS runs through MoltenVK as an opt-in: an Apple build leaves the module out unless it is
configured with `-DDAS_VULKAN_DISABLED=OFF`, because MoltenVK's compute limits (a 32 KB
workgroup memory cap) decline the kernel classes the module's consumers serve, and what
MoltenVK proves does not carry to a native Vulkan device - so a Mac pays the module's build
and compile time for nothing by default. The nightly Vulkan lane's macOS job opts in and
proves the loader path. With the opt-in the host needs one setup step, `brew install molten-vk
vulkan-loader vulkan-tools`; four pieces then make it work, all of them platform-agnostic in
the code:

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

## The OS's video memory numbers {#os-video-memory}

`vk_os_video_memory` (`src/dasVULKAN.main.cpp`, bound by hand) answers what Vulkan cannot: how
much of the adapter's dedicated memory every process holds right now, and how much system memory
the adapter maps, beside this process's ceiling and its own usage. Its query is one of four
(`OsVideoMemoryQuery`): the process's budget, the process's usage, the adapter's dedicated usage
across every process, and the adapter's shared usage across every process. On Windows the ceiling
and the usage come from three gdi32 exports (`D3DKMTOpenAdapterFromLuid`,
`D3DKMTQueryVideoMemoryInfo`, `D3DKMTCloseAdapter`) over the LOCAL segment group, and the
adapter-wide numbers from pdh's "GPU Adapter Memory" Dedicated Usage and Shared Usage counters
summed over the instances named by the adapter's LUID (`luid_0x<hi>_0x<lo>_phys_<n>`, one per
physical segment); every entry point is resolved by name (no import library, no WDK, the structs
spelled from `d3dkmthk.h` and `pdh.h`), and everywhere else, or whenever a call fails, the answer
is 0, which a caller reads as "no answer". The boost helper `os_video_memory` takes the LUID from
`VkPhysicalDeviceIDProperties` and returns the four. Neither `VK_EXT_memory_budget` nor the
process's own WDDM budget carries the desktop: on the NVIDIA Windows driver both read the card
minus the OS reserve whatever other processes hold.

## Documentation pipeline

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

## Tutorial units

A tutorial is a self-contained unit under `tutorials/<NN_name>/`: an offscreen module, its
`[compute_shader]` or `[shader]` blob, a pixel-oracle `[test]` that CI gates, and a
`recording/` driver. Its windowed viewer lives at `<NN_name>/window/show_<name>.das` - one per
way the output reaches the swapchain, `show_<name>_<way>.das` each once there are two - which is
where the tutorials `.das_test` skips it - the lavapipe CI daslang build is
`-DDAS_GLFW_DISABLED=ON`, so it has no display and no GLFW.

Shared helpers are copied per tutorial rather than factored out: daslang `require` cannot parse
an unquoted path segment starting with a digit (`require ../../02_mandelbrot/window/x.das` fails
with `error[30151] unexpected integer constant`). The `mandelbrot_compute` of 02 and the
`resident_compute` of 03 are the same resident compute-to-image builder held as two copies for
that reason; a non-digit shared path such as `tutorials/common/` is what a third would need.

## CI gates

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

## Exception ledger

- `tutorials/10_deferred` keeps its render passes and its `transition_image` calls (the legacy
  frame-shape gate in `REVIEW.das` ignores the folder): the pass-to-pass hand-off through
  attachment layouts is its lesson, and the dynamic-rendering form of it, local read, is Vulkan
  1.4 core - tutorial 16.
