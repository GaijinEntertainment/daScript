dasVulkan
=========

dasVulkan is the daslang binding for `Vulkan <https://www.vulkan.org/>`_,
generated from the Khronos ``vk.xml`` registry. It ships the raw 1:1 API surface
over `volk <https://github.com/zeux/volk>`_ plus an ergonomic boost layer:
RAII-owning handle wrappers, idiomatic structs (``array<T>`` instead of
count+pointer pairs, auto-filled ``sType``), named/defaulted arguments, and
block-bracketing helpers that collapse the begin/record/submit boilerplate.

Where to go
-----------

* **Documentation**: https://borisbat.github.io/dasVulkan/
* **Repository**: https://github.com/borisbat/dasVulkan

What's there
------------

* ``vulkan`` — the raw binding: the full Vulkan API (core + extensions),
  generated from ``vk.xml`` as a daslang C++ module dispatching through volk.
  Every ``Vk*`` struct, enum, and ``VkFlags`` (as a daslang ``bitfield``).
* ``vulkan_boost`` — the ergonomic layer: ``create_*`` creators returning
  RAII wrappers that destroy in reverse order under ``var inscope``, combined
  builders (``build_offscreen_target``, ``create_graphics_pipeline_simple``),
  block brackets (``run_cmd_sync``, ``record_render_pass``,
  ``map_memory_to_array``), and a windowing tier (surface, resizable swapchain,
  the present loop).
* Examples under ``examples/`` — offscreen triangle (raw reference + boost
  twin), compute, device enumeration, and a resizable GLFW windowed triangle.
* An in-process ``dastest`` suite that renders offscreen and reads back pixels,
  running headless on Mesa lavapipe in CI (no GPU needed).

Versioning
----------

The bindings are generated against the Vulkan SDK **1.4.350.0** headers
(vendored). The documentation site covers the boost layer; the raw ``vulkan``
symbols mirror the Vulkan C API 1:1, so the
`Vulkan specification <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_
is their reference.
