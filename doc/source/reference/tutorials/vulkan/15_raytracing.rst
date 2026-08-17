15 - Hardware Ray Tracing
=========================

Everything so far rasterized. This tutorial fires real rays: the scene -- a box
on a ground plane -- is built into **acceleration structures**, a
``VK_KHR_ray_tracing_pipeline`` traces one camera ray per pixel into it, and
each hit casts a second **shadow ray** toward the light. No render pass, no
vertex input, no rasterizer: the raygen shader writes the frame directly into a
storage image.

The headline rails:

- **Three new shader stages, all daslang.** ``[vulkan_raygen_shader]`` fires
  the pinhole-camera rays and ``imageStore``\ s the result;
  ``[vulkan_miss_shader]`` paints the sky; ``[vulkan_closest_hit_shader]``
  flat-shades the hit. dasSpirv lowers all three to SPIR-V 1.4 with the
  ``RayTracingKHR`` capability -- ``@ray_payload`` / ``@incoming_ray_payload``
  globals carry data between them, and ``traceRayEXT(...)`` is the daslang
  spelling of ``OpTraceRayKHR``.
- **BLAS + TLAS in two calls.** ``build_blas`` uploads the triangle soup and
  runs the GPU build; ``build_tlas`` places it with ``make_accel_instance``
  (the hand-packed stand-in for ``VkAccelerationStructureInstanceKHR``'s C
  bitfields). Both block until built, so the resulting ``AccelStructure``\ s
  are immediately traceable.
- **The shader binding table, assembled for you.**
  ``create_ray_tracing_pipeline`` lays the groups out as
  ``[raygen, miss..., hit]`` and ``build_shader_binding_table`` fetches the
  opaque group handles, applies the device's alignment rules, and returns the
  four strided regions ``cmd_trace_rays`` consumes.
- **Shadow without recursion into shading.** The closest-hit's shadow ray uses
  ``terminate-on-first-hit + skip-closest-hit`` flags and ``miss_index = 1``:
  no hit shader ever runs for it -- only the *second* miss shader, which flips
  the payload to "lit" when the ray reaches the light unobstructed.
- **The descriptor side is reflection-driven.** The TLAS binding reflects as
  ``VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR`` and the layouts come
  straight from ``build_descriptor_set_layouts`` /
  ``build_pipeline_layout`` -- the same reflection path every raster tutorial
  uses, now RT-stage-aware.

.. video:: vulkan_raytracing.mp4

The clip above is the headless recording: 30 seconds, 30 fps, one full camera
orbit -- every frame is a real ``vkCmdTraceRaysKHR``, and the traced shadow
swings around the box as the camera circles. The ``[test]`` pixel-oracle is the
strongest in the series: the *same* daslang scene tables and shading functions
also run in the interpreter as a CPU reference tracer, and the GPU frame must
match it pixel-for-pixel (up to a small silhouette budget where hardware
traversal and the CPU intersector round the boundary differently). One source.
Two execution tiers. Identical pixels.

.. note::

   Hardware ray tracing requires ``VK_KHR_ray_tracing_pipeline`` +
   ``VK_KHR_acceleration_structure`` with the ``bufferDeviceAddress`` feature
   (``rt_supported`` probes all of it). The tutorial soft-skips on devices
   without RT support (lavapipe in CI, so the test passes as skipped) -- the
   recording here is from an RT-capable GPU.

The shaders
-----------

Four entry points: raygen (camera + ``imageStore``), the sky miss, the shadow
miss (the "lit" flip), and the closest-hit (flat normal by ``gl_PrimitiveID``,
lambert, one shadow ray). The scene tables and shading functions they call are
plain daslang functions -- the CPU reference tracer in the test calls the very
same ones.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/15_raytracing/raytracing_tut_shaders.das
   :language: das
   :start-at: module raytracing_tut_shaders

The host side
-------------

``build_rt_view`` assembles the resident RT stack -- BLAS, TLAS,
reflection-built layouts, the pipeline, its shader binding table, and the
storage image -- and ``record_trace`` re-fires the whole frame from one push
constant (the camera orbit angle).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/15_raytracing/raytracing_tut.das
   :language: das
   :start-at: struct RtView
   :end-before: struct RtOffscreen

Self-verifying
--------------

The pixel-oracle traces the same frame twice -- hardware RT on the GPU, the
Moller-Trumbore reference in the interpreter -- and asserts they agree
pixel-for-pixel within a small silhouette budget, after first proving the CPU
frame contains all four regimes (sky, lit ground, the cast shadow, the box).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/15_raytracing/test_raytracing.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_raytracing.das`` opens a GLFW window and traces the orbit live:
``build_rt_view`` once, then every frame is one push constant + one
``vkCmdTraceRaysKHR`` + a blit onto the swapchain image.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/15_raytracing/window/show_raytracing.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (skips cleanly without RT support)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/15_raytracing

   # watch it live in a window (needs the glfw module + an RT-capable GPU)
   daslang.exe modules/dasVulkan/tutorials/15_raytracing/window/show_raytracing.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/15_raytracing/recording/record_raytracing.das
