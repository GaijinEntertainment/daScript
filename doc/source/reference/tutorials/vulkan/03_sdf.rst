03 - SDF raymarch (compute, no geometry)
========================================

Tutorial 02 used the compute pipeline to iterate ``z = z*z + c`` per pixel
across the Mandelbrot set. This one uses the same compute pipeline to walk a
ray through a 3D scene defined by **signed distance functions** -- no vertex
buffer, no triangles, no textures; the whole scene is math evaluated at every
pixel. The headline rails:

- a **storage image** at ``set=0/binding=0`` the compute shader writes to
  (same descriptor shape as Mandelbrot);
- a **push constant** carrying one float, ``time``, which drives the camera
  orbit, the torus rotation, the sphere bob, and the cosine palette;
- **user-defined helpers** (smin, sd_sphere, sd_torus, sd_plane, rot2, map,
  get_normal, march, soft_shadow, ...) lowered to SPIR-V ``OpFunctionCall``
  -- the raymarcher reads like CPU code;
- **3-vector swizzle and arithmetic** (``p.xz``, ``p - float3(...)``, ``ro + rd * dist``);
- a wide **GLSL.std.450** breadth: ``length`` / ``normalize`` / ``cross`` /
  ``dot`` / ``floor`` / ``lerp`` (lowered to ``FMix``) / ``clamp`` / ``cos`` /
  ``sin`` / ``pow`` / ``exp`` -- everything the IQ-style raymarch recipe needs.
  The smooth-union ``smin`` is a private polynomial helper in the shader, not
  a GLSL.std.450 instruction; it lowers to ``OpFunctionCall``.

Every line of the shader is daslang, lowered to SPIR-V at compile time by dasSpirv.

.. video:: vulkan_sdf.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel ambient pad bed.
The camera completes one full orbit around the scene (period chosen so the
loop closes seamlessly); the torus rotates around the sphere, the sphere
gently bobs, and the cosine-palette pulse cycles the blob hue across the orbit
(cream -> blue -> copper). See ``skills/internal/vulkan_recording.md``. To watch the raymarch
update live on your own GPU, run the windowed viewer (see `See it live`_ below).

The shader
----------

The whole pipeline lives in one ``[vulkan_compute_shader]`` function plus a
handful of private helpers. ``map(p, t)`` returns
``(distance_to_nearest_surface, material_id)`` for any point in world space;
``march`` walks the ray; ``get_normal`` does a 4-tap forward difference (1
baseline + 3 axis-displaced taps -- the cheaper asymmetric cousin of the
classic 6-tap central difference, visually identical at this eps + lighting);
``soft_shadow`` is IQ's penumbra estimate. The ``[vulkan_compute_shader]``
annotation also synthesises ``sdf_main_push_constants(cmd, layout)`` from
``var @push_constant pc : Push`` -- the host just writes ``pc.time = time``
and calls it.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/03_sdf/sdf_tut_shaders.das
   :language: das
   :start-at: module sdf_tut_shaders

The render (headless)
---------------------

``render_sdf(time)`` is the one-shot path used by the test: build a fresh
``SdfRenderer``, dispatch one frame, tear down. The recording driver uses
``build_sdf_renderer()`` once and calls ``render_sdf_frame(r, t)`` in a loop --
the long-lived state (instance, device, storage image + memory + view,
descriptor set + pipeline layout + compute pipeline, readback buffer) is
rebuilt once; the per-frame work is just pushing the new ``time``, dispatching
the compute shader at one workgroup per 8x8 pixel tile, and copying the image
back to the host. No window required.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/03_sdf/sdf_tut.das
   :language: das
   :start-at: def public render_sdf_frame
   :end-before: //! One-shot

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, a real GPU locally). The
SDF scene is deterministic at a fixed ``time``, so the oracle samples a few
pixels at known positions and asserts palette and structure: sky-blue at the
top corners, mid-tone non-saturated values along the bottom row (the
checkered ground hit), and a warm-toned hero blob in the central band.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/03_sdf/test_sdf.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_sdf.das`` opens a GLFW window with a Vulkan swapchain and
recomputes the raymarch every frame -- the time push constant comes from
wall-clock, so the camera orbit and the cosine palette pulse update live as
the window stays open. It reuses the same ``sdf_spv`` blob the headless render
uses; the only additions are the surface, swapchain, and a per-frame blit of
the compute storage image onto the swap image. It needs a display and the
``glfw`` module, so it lives in a ``window/`` subfolder that the tutorial's CI
gate skips (CI is headless and built without GLFW); it is the run-and-watch
companion to the headless oracle.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/03_sdf/window/show_sdf.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/03_sdf

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/03_sdf/window/show_sdf.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/03_sdf/recording/record_sdf.das

Next
----

:doc:`04_cube` switches back to the **graphics** pipeline and adds the rest of
a "real" 3D scene: per-vertex attributes, a depth attachment, a procedurally
generated texture, an MVP matrix carried in a UBO, and proper Blinn-Phong-ish
lighting.
