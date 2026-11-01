# dasVulkan Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_RENDERING.md`. Planned work: `ROADMAP.md`. A tutorial - a
`tutorials/<NN_name>/` unit, and any `record_*.das` recording driver wherever the diff puts it -
answers to the `tutorials/` subfolder's checklist. A generator source or a committed generator
report, wherever the diff puts it, answers to the `generator/` subfolder's checklist. A `[test]`
file, wherever the diff puts it, answers to the `tests/` subfolder's checklist. A kernel body - a
function annotated `[spirv_kernel]`, `[metal_kernel]`, or with an annotation whose name ends in
`_shader` - answers to `modules/REVIEW_SHADER_EMITTERS.md` as well, wherever the diff puts it.

**Weakening `REVIEW.das` (beside this file) is a defect.**

**A diff that adds a feature bit or a device extension that a `create_device*` creator in
`daslib/vulkan_boost.das` always enables adds it, in the same change, to that creator's
`*_supported` probe, with every feature and extension the creator enables because of it; a
creator with no probe gets one in that change.** A bit or extension the probe misses either
fails `vkCreateDevice` or lets a kernel use a feature the device never enabled.

**A diff that adds to a `create_device*` creator a feature bit from a
`VkPhysicalDeviceVulkan1NFeatures` struct, or meets an extension's dependency with core version
1.N instead of the extension, has that creator's `*_supported` probe check API version 1.N
against both the version the instance was created with and the device's `apiVersion`.** A
device-only check answers true under an instance the struct is invalid for.

**A diff that adds a device extension to a `create_device*` creator in `daslib/vulkan_boost.das`
meets the added extension's `depends` expression in `vendor/Vulkan-Headers/registry/vk.xml` (`,`
is or, `+` is and) at every API version the creator runs on - its probe's minimum (1.0 when the
probe checks none) and above - with extensions the creator enables or a core version its probe
checks.** An unmet dependency makes the `vkCreateDevice` call invalid, even on a driver that
accepts it.

**A diff that adds a feature bit or a device extension that a `create_device*` creator in
`daslib/vulkan_boost.das` enables only when the device reports it gates what it adds - the
`VkPhysicalDevice*Features` struct it chains and the extension name it enables, whichever of the
two the addition has - on a `*_supported` probe: an existing one, or one the diff adds.** Without
that probe, the code that uses the feature cannot tell whether the device it was given has it.

**A diff that adds a public helper to a module `utils/vulkan2rst.das` documents, or that edits
`utils/vulkan2rst.das`, leaves every public helper of the modules it documents in a
`group_by_regex` group whose title names what the helper does.** A helper filed under a title that
does not cover it escapes the `Uncategorized` check in `doc/check_docs_fresh.cmake` (repo
root), and no other check reports it.

**A diff that adds or edits a call to raw `vkCreateDevice` appends `VK_KHR_portability_subset`
to that call's extension list when the device advertises it - or calls a `create_device*` boost
creator instead, which appends it.** The Vulkan spec fails `vkCreateDevice` on a device that
advertises the extension without enabling it.

**A diff that puts the result of a `create_*` call into a local the function neither returns
nor moves into a container it deletes declares that local `var inscope`.** A plain
`var x <- create_*()` leaks: the wrapper owns a raw Vulkan handle, and nothing frees it without
the scope-exit `finalize`.

**A diff to a `daslib/vulkan_window.das` or `daslib/vulkan_live.das` function that takes no
`RenderPass` never calls `record_render_pass` - it records its draws inside `record_rendering` or
`record_rendering_depth_only`.** The render-pass object is the pre-1.3 form of dynamic rendering
(`ARCHITECTURE_RENDERING.md#dynamic-rendering`).

**A diff to a `daslib/vulkan_window.das` or `daslib/vulkan_live.das` function that takes no
`RenderPass` never calls `transition_image`, `transition_depth_image` or `transition_image_aspect` -
it calls `transition_image2`, `transition_depth_image2` or `transition_image_aspect2`.** Those three
take the 32-bit stage and access flags that synchronization2 replaces.
