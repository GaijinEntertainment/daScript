.. _stdlib_vulkan_external_types_section:

##############
External types
##############

Types defined by the raw ``vulkan`` binding and the generated ``vulkan_structs``
view module that the documented boost surface references but does not own. The
raw layer mirrors the Vulkan C API 1:1 and is not re-documented here — see the
`Vulkan specification <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_
for the authoritative field-by-field reference. This page anchors the labels so
the generated boost pages can link without dangling references.

Core enums
==========

.. _enum-vulkan-vkresult:

``vulkan::VkResult``
--------------------

Return code for almost every Vulkan command. ``VK_SUCCESS`` is ``0``; negative
values are errors. The boost ``create_*`` / command wrappers run it through
``vk_check`` — left to default they panic on a non-success code, or forward it
to a caller-supplied ``var result : VkResult?``.

.. _enum-vulkan-vkformat:

``vulkan::VkFormat``
--------------------

Pixel / vertex-attribute format enum (``VK_FORMAT_R8G8B8A8_UNORM``, …). Passed to
image, image-view, render-pass, and swapchain creation.

.. _enum-vulkan-vkcolorspacekhr:

``vulkan::VkColorSpaceKHR``
---------------------------

Color space of a presentation surface (``VK_COLOR_SPACE_SRGB_NONLINEAR_KHR``, …).
Reported by surface-format queries and recorded on the swapchain.

.. _enum-vulkan-vkimagelayout:

``vulkan::VkImageLayout``
-------------------------

Image memory layout (``VK_IMAGE_LAYOUT_UNDEFINED``,
``VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL``, …) used in render passes and
barriers.

.. _enum-vulkan-vkpipelinebindpoint:

``vulkan::VkPipelineBindPoint``
-------------------------------

Selects which bind point a pipeline / descriptor set binds to —
``GRAPHICS`` or ``COMPUTE``. The trailing ``bind_point`` argument to
``cmd_bind_pipeline`` (defaults to ``GRAPHICS``).

.. _enum-vulkan-vkcompareop:

``vulkan::VkCompareOp``
-----------------------

Per-fragment comparison operator (``LESS``, ``LESS_OR_EQUAL``, ``EQUAL``,
``GREATER_OR_EQUAL``, …). Used by the depth-stencil state on a graphics
pipeline — the ``depth_compare_op`` argument to
``create_graphics_pipeline_v3d`` (defaults to ``LESS_OR_EQUAL`` for the
standard z-buffer convention).

Core handles and aliases
========================

.. _alias-vkphysicaldevice:

``vulkan::VkPhysicalDevice``
----------------------------

Raw non-owning handle for a physical GPU, returned by ``select_physical_device``
and consumed by device creation and memory queries. Has no RAII wrapper (it is
owned by the instance, not created or destroyed).

.. _alias-vkqueue:

``vulkan::VkQueue``
-------------------

Raw non-owning handle for a device queue, returned by ``get_device_queue``.
Owned by the device; no separate destroy.

.. _alias-vkmemorypropertyflags:

``vulkan::VkMemoryPropertyFlags``
---------------------------------

Bitfield of memory-property bits (``DEVICE_LOCAL``, ``HOST_VISIBLE``,
``HOST_COHERENT``, …) used by ``find_memory_type`` to pick a heap.

.. _alias-vkimage:

``vulkan::VkImage``
-------------------

Raw non-owning handle for an image. The owning RAII wrapper is ``Image``;
the swapchain's images and the raw handle handed to ``present_frame``'s
record block are bare ``VkImage`` handles.

.. _alias-vkaccessflags:

``vulkan::VkAccessFlags``
-------------------------

Bitfield of memory-access bits (``shader_write``, ``transfer_read``,
``transfer_write``, …). The source/destination access masks of a pipeline
barrier; consumed by ``transition_image``.

.. _alias-vkpipelinestageflags:

``vulkan::VkPipelineStageFlags``
--------------------------------

Bitfield of pipeline-stage bits (``top_of_pipe``, ``compute_shader``,
``transfer``, ``fragment_shader``, …). The source/destination stage masks of
a pipeline barrier; consumed by ``transition_image``.

.. _alias-vkbufferusageflags:

``vulkan::VkBufferUsageFlags``
------------------------------

Bitfield of buffer-usage bits (``vertex_buffer``, ``index_buffer``,
``uniform_buffer``, ``storage_buffer``, ``transfer_src``, ``transfer_dst``, …).
Picks how the GPU consumes a ``VkBuffer``; consumed by ``create_host_buffer``
and ``create_host_buffer_from_bytes``.

.. _alias-vkshaderstageflags:

``vulkan::VkShaderStageFlags``
------------------------------

Bitfield of shader-stage bits (``vertex``, ``fragment``, ``compute``, …). The
stage mask telling Vulkan which shader stages read a push-constant update;
consumed by ``cmd_push_constants``.

.. _alias-vkimageaspectflags:

``vulkan::VkImageAspectFlags``
------------------------------

Bitfield of image-aspect bits (``color``, ``depth``, ``stencil``). Selects
which aspect of an image a layout transition or descriptor applies to;
consumed by ``transition_image_aspect`` (the depth / stencil counterpart of
the color-by-default ``transition_image``).

.. _alias-vkimageusageflags:

``vulkan::VkImageUsageFlags``
-----------------------------

Bitfield of image-usage bits (``color_attachment``, ``depth_stencil_attachment``,
``sampled``, ``transfer_src``, ``transfer_dst``, ``input_attachment``, …). The
``extra_usage`` parameter the offscreen-builder overloads
(``build_offscreen_target``, ``build_offscreen_depth``) take on top of the
helper's always-set base usage.

.. _alias-vksamplecountflags:

``vulkan::VkSampleCountFlags``
------------------------------

Bitfield of sample-count bits (``_1``, ``_2``, ``_4``, ``_8``, …). The MSAA
sample count of an image or pipeline; consumed by ``build_offscreen_target``,
``build_offscreen_depth``, and ``create_graphics_pipeline_dyn``.

.. _handle-vulkan-vkclearvalue:

``vulkan::VkClearValue``
------------------------

Union of a clear color or depth/stencil value. ``clear_color`` builds one;
``record_render_pass`` consumes it.

.. _handle-vulkan-vkrect2d:

``vulkan::VkRect2D``
--------------------

An ``offset`` + ``extent`` rectangle. ``full_area`` builds one covering the
whole target; used as the render-pass render area.

.. _handle-vulkan-vkvertexinputbindingdescription:

``vulkan::VkVertexInputBindingDescription``
-------------------------------------------

One per vertex buffer bound by ``vkCmdBindVertexBuffers``: the binding slot,
the stride between vertices, and whether the rate is per-vertex or
per-instance. Consumed by ``create_graphics_pipeline_v3d`` as
``vertex_bindings``.

.. _handle-vulkan-vkvertexinputattributedescription:

``vulkan::VkVertexInputAttributeDescription``
---------------------------------------------

One per per-vertex attribute the vertex shader reads (``@in @location = N``):
the location, the source binding slot, the format, and the byte offset
within the vertex. Consumed by ``create_graphics_pipeline_v3d`` as
``vertex_attributes``.

.. _handle-vulkan-vkphysicaldevicefeatures2:

``vulkan::VkPhysicalDeviceFeatures2``
-------------------------------------

The chained feature struct a device is created with: core features in
``features``, every extension feature struct hung off ``pNext``. The
``create_device`` overloads that take one let a caller assemble an arbitrary
chain, which is how the compute-tier creators enable coopmat, memory-priority
and timeline-semaphore features in a single ``vkCreateDevice``.

``vulkan_structs`` view structs
===============================

.. the anchor block is generated by utils/vulkan2rst.das — one label per view
   struct, so a signature reference can never dangle when the generator
   unlocks new creators.

.. include:: generated/vulkan_structs_anchors.inc

``vulkan_structs::*``
---------------------

The idiomatic *view* structs accepted by the creators and command wrappers on
the :doc:`generated/vulkan_commands` page. Each shadows the matching Vulkan
struct with daslang-friendly fields: ``array<T>`` instead of count+pointer
pairs, ``string`` instead of ``const char*``, boost handle types instead of
raw ones, and ``sType`` filled automatically. Because they are generated 1:1
from ``vk.xml``, their fields are documented field-by-field in the
`Vulkan specification <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_;
see :doc:`vulkan_overview` for the view-struct mechanism.
