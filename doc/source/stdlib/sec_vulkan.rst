.. _stdlib_vulkan_section:

***********************
GPU (Vulkan)
***********************

dasVulkan — the daslang binding for `Vulkan <https://www.vulkan.org/>`_,
generated from the Khronos ``vk.xml`` registry, documented here at its ergonomic
``vulkan_boost`` tier: the RAII handle wrappers and their creators, the
hand-written builder/bracket core, and the windowing helpers. The raw ``vulkan``
symbols (generated 1:1 from ``vk.xml``) are not duplicated here — see the
`Vulkan specification <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_
for the C API surface, and :doc:`vulkan_overview` for how the boost layer maps
onto it.

Start with :doc:`/reference/tutorials/vulkan/01_triangle`: shaders authored in
daslang, lowered to SPIR-V at compile time, drawn through the boost layer.

.. toctree::

   vulkan_overview.rst
   generated/vulkan_runtime.rst
   generated/vulkan_handles.rst
   generated/vulkan_commands.rst
   generated/vulkan_boost.rst
   generated/vulkan_window.rst
   vulkan_external_types.rst
