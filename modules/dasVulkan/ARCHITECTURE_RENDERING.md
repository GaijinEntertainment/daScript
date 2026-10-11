# dasVulkan architecture - the frame shape

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`): how a frame is drawn
and synchronized in the boost layer, the tutorials and the examples.

## Mechanisms

### Dynamic rendering and synchronization2 {#dynamic-rendering}

The frame shape the tutorials, the boost examples and the window layer teach is Vulkan 1.3's; the
raw-binding references `examples/offscreen_triangle.das` and `examples/compute.das` keep the raw
API they exist to show. A pipeline names the formats it renders into - `RenderingFormats`, chained
at the head of the create info's `pNext` by `attach_rendering_formats`, which the boost pipeline
creators take as a format overload beside their `RenderPass` one: `color_format` alone for
`create_graphics_pipeline_simple` and `imgui_vk_create`, which draw with no depth attachment,
`color_format` + `depth_format` for the v3d and mesh creators. `record_rendering` opens the pass on
image views, and every layout move is a `transition_image2`-family barrier on the 64-bit stage and
access flags, naming on each side the stage that produced the data and the stage that consumes
it. The window-layer forms are `draw_frame` without a render pass over
`record_swapchain_rendering`, `blit_to_swapchain` for a frame rendered offscreen, and
`vk_live_draw_frame` without a render pass; `submit2_and_present` is their `vkQueueSubmit2`.

The plain `create_device` creators enable `dynamicRendering` + `synchronization2` whenever the
instance that `create_instance` made is 1.3 and the device reports the pair
(`dynamic_rendering_supported`); the features12, mesh-shader, draw-parameters and ray-tracing
creators chain the same pair at the tail of their own chains. The instance version is part of
the gate because a 1.3 feature struct is not legal in a device chain below a 1.3 instance, and
because the validation layer resolves the `_2` entry points to null there: a
`vkCmdPipelineBarrier2` recorded on a 1.1 instance works on the bare driver and crashes with the
layer loaded. A physical device does not report the version of the instance it came from, so
`create_instance` records the `api_version` it asked for in a module global and
`dynamic_rendering_supported` reads that record. The record is the latest `create_instance`
call's: an instance made by raw `vkCreateInstance` leaves it unchanged - 0 when no
`create_instance` ran - and the plain creators then leave the pair off. The features12 and
mesh-shader overloads chain the pair only when the caller's `pNext` is null.

An image a pass renders into starts each frame `UNDEFINED`. The barrier into
`COLOR_ATTACHMENT_OPTIMAL` or `DEPTH_ATTACHMENT_OPTIMAL` names as its source the last stage of the
frame being recorded that touched the image - `fragment_shader` for one an earlier pass of the
frame sampled, `PIPELINE_STAGE_2_NONE` for the frame's first touch, because every window-layer
present and every `run_cmd_sync` ends with a queue-idle wait that completes the previous frame's
reads - and the barrier out of the pass names the consumer: `copy | blit` with `transfer_read` for
a readback or present blit, `fragment_shader` with `shader_sampled_read` for the next pass's
sampler, `compute_shader` for a compute reader. A swapchain image's first barrier names the stage
the acquire semaphore wait lands on - `color_attachment_output` in `record_swapchain_rendering`,
`blit` in `blit_to_swapchain` under `present_frame`'s transfer wait - so the layout change is
ordered after the acquire. Two renderings into one image within a frame are not ordered against
each other: a `color_attachment_output` to `color_attachment_output` barrier with the layout
unchanged sits between them.

The render-pass helpers - `create_render_pass_*`, `record_render_pass`,
`build_swapchain_framebuffers`, the `RenderPass` overloads of the pipeline creators, of
`draw_frame` and of `vk_live_draw_frame`, the legacy-flag `transition_image` family - stay beside
the new ones: Vulkan removes nothing and external modules compile unchanged. A pipeline built for
a render pass binds only inside `record_render_pass`, and one built for formats only inside
`record_rendering`; the two frame shapes do not mix within a pass.

A creator that asks for the pair records whether it got it, and `require_dynamic_rendering` panics
with that record's reason from `record_rendering`, the `transition_image2` family and
`submit2_and_present`: the `_2` entry points are null on a device without the pair, and a call into
one is a crash with no message. A device from raw `vkCreateDevice` or from a creator that takes the
caller's whole chain leaves the record alone.

### Vulkan 1.4 {#vulkan-14}

A 1.4 device comes from `create_device_vulkan14`, which enables the three promotions that change how
a frame is put together - `pushDescriptor`, `hostImageCopy`, `dynamicRenderingLocalRead` - behind the
1.3 pair, and panics unless `vulkan14_supported` holds: the `create_instance` instance and the device
both 1.4, the device reporting the three features. Tutorial 16 is the consumer; its oracle skips with
a message where the probe fails, because lavapipe reports 1.4 only from Mesa 25.0.

Push descriptors need a set layout flagged `push_descriptor` - `build_push_descriptor_set_layouts`
over the reflections `build_descriptor_set_layouts` takes - and are written per draw with
`cmd_push_descriptor_set`; no descriptor pool exists. Host image copy fills an image created with
usage `host_transfer` through `upload_image_from_host`: a host-side layout transition out of
`UNDEFINED` into the layout `host_copy_layout` picked - the wanted one when the device lists it among
its copy destinations, else `GENERAL`, which every device lists - then one `vkCopyMemoryToImage`; no
staging buffer, no command buffer, no queue. Local read keeps the attachments one pass writes in
`RENDERING_LOCAL_READ` and lets a later pass of the same rendering read them as input attachments:
each pipeline carries a `VkRenderingAttachmentLocationInfo` (fragment location to attachment) and a
`VkRenderingInputAttachmentIndexInfo` (attachment to `subpassInput` index) on its chain, the command
buffer sets the same two maps before that pass's draws, and a by-region memory barrier from
color-attachment writes to fragment-shader input-attachment reads sits between the passes.

### Shader objects {#shader-objects}

`VK_EXT_shader_object` is an opt-in beside the pipeline path, never the path the tutorials teach: it
is an extension in every Vulkan version, absent on Android and partial on MoltenVK, and dasLLAMA's
compute path is a `VkPipeline`. `create_device_shader_objects` appends the extension string and chains
its feature, `examples/shader_objects.das` is the one consumer, and `tests/integration` checks it draws
the pipeline path's triangle. A draw with shader objects has no pipeline to fix the fixed-function
state, so `cmd_set_shader_object_baseline` sets every piece of it the extension requires before a
draw - viewport and scissor with count, rasterization, sample, depth, stencil, vertex-input, blend and
write-mask state - in the shape a pipeline would have frozen, and `cmd_bind_graphics_shaders` binds the
vertex and fragment objects with the tessellation and geometry slots emptied, as the extension
requires of a graphics draw.
