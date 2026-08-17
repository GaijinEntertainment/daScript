04 - The Synthwave Cube (graphics + depth)
==========================================

:doc:`01_triangle` drew with the **graphics** pipeline; :doc:`02_mandelbrot`
and :doc:`03_sdf` drew with the **compute** pipeline. This one is the first to
need the whole graphics-with-depth surface: a textured 3D cube, lit, with a depth attachment
so back faces are correctly hidden behind front faces. The headline rails:

- a **per-vertex buffer** with position + uv + normal -- ``a_pos`` / ``a_uv`` /
  ``a_normal`` flow in through ``@in @location`` stage inputs instead of being
  fabricated from ``gl_VertexIndex``;
- a **uniform buffer** with the view and projection matrices plus the camera
  position (and time);
- a **push constant** carrying the per-frame model matrix;
- a **combined image sampler** of a procedural synthwave horizon texture
  generated on the host at init;
- **depth test and write** (``D32_SFLOAT`` attachment, ``LESS_OR_EQUAL``);
- ``normalize`` / ``dot`` / ``pow`` from GLSL.std.450 for the rim + key lighting.

Every line of the shader is daslang, lowered to SPIR-V at compile time by dasSpirv.

.. video:: vulkan_cube.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel supersaw-pad bed.
The camera orbits the cube on a lemniscate (small vertical figure-8 over the
revolution) while the cube breathes via the push-constant scale; the texture's
sun-disk drifts via a UV scroll keyed off ``cam_time.w``. See
``skills/internal/vulkan_recording.md``. To watch it spin live in a resizable window on your own
GPU, run the windowed viewer (see `See it live`_ below).

The shaders
-----------

A struct UBO with two ``float4x4`` and one ``float4`` member is the canonical
std140 layout. The push constant is a single ``float4x4`` model matrix at offset
0 -- vertex-only, 64 bytes (well under the 128-byte push-constant minimum
guarantee). The fragment shader reads ``cam.cam_time`` to fold the camera
position into the view direction and the time into the UV scroll. The
``[vulkan_*_shader]`` annotations synthesise two host-side helpers from
``var @uniform cam : Camera`` and ``var @push_constant pc : ModelPC``:
``cube_vs_bind_uniform(device, memory)`` writes each std140 field at its
computed offset into a mapped UBO, and ``cube_vs_push_constants(cmd, layout)``
does the ``vkCmdPushConstants`` call. The host writes ``cam.* = ...`` and
``pc.model = ...`` and calls the helpers -- one source of truth for the
layout, no manual ``push_from`` / ``upload_bytes`` packing.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/04_cube/cube_tut_shaders.das
   :language: das
   :start-at: module cube_tut_shaders

The render (headless)
---------------------

The per-frame work is split into two reusable halves so the windowed viewer can
share the same render with the offscreen test. ``update_cube_uniforms`` writes
the new view / projection / camera position into ``cam`` (the UBO) and the new
model matrix into ``pc`` (the push constant) -- host-only work, no command
buffer touched. ``record_cube_render_pass`` records the pipeline bind, the
descriptor set, the push-constants upload, and the indexed draw into a caller-
supplied command buffer; the render pass's ``finalLayout`` leaves the colour
attachment in ``TRANSFER_SRC_OPTIMAL`` so a swapchain blit is one command away.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/04_cube/cube_tut.das
   :language: das
   :start-at: def public update_cube_uniforms
   :end-before: //! Per-frame work

``render_cube_frame`` is what the headless path uses: call the two helpers
inside a ``run_cmd_sync``, then copy the colour attachment into the readback
buffer and clone it out. ``render_synthwave_cube`` is the one-shot wrapper
(``build_cube_context`` + ``render_cube_frame``) the test calls; the recording
driver builds the context once and calls ``render_cube_frame`` in a loop.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/04_cube/cube_tut.das
   :language: das
   :start-at: def public render_cube_frame
   :end-before: //! One-shot:

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, a real GPU locally). The
cube has no analytic-symmetry shortcut the way Mandelbrot did; instead the
oracle asserts a handful of structural properties at a fixed (``time``,
``camera_t``): the frame corners are the dark clear colour, the central 200x200
box is dominantly the cube, and the lit region carries the synthwave palette
(at least one magenta-dominant pixel from the sky band, at least one
cyan-dominant pixel from the perspective grid).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/04_cube/test_cube.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_cube.das`` opens a GLFW window with a Vulkan swapchain and presents
the spinning cube every frame. It owns its own instance (with surface extensions)
and device (with ``VK_KHR_swapchain``), then calls ``build_cube_resources`` to
share the offscreen render pass, framebuffer, geometry, texture and graphics
pipeline with the headless oracle. Each frame it runs ``update_cube_uniforms`` +
``record_cube_render_pass`` into the present command buffer, then blits the
colour attachment onto the swapchain image. It needs a display and the ``glfw``
module, so it lives in a ``window/`` subfolder that the tutorial's CI gate skips
(CI is headless and built without GLFW); it is the run-and-watch companion to
the headless oracle.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/04_cube/window/show_cube.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/04_cube

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/04_cube/window/show_cube.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/04_cube/recording/record_cube.das

Next
----

:doc:`05_instancing` keeps the cube's graphics+depth pipeline but draws **a
thousand of them in a single draw call** — a second vertex binding with
``INPUT_RATE_INSTANCE`` feeds per-instance offset/color, and
``vkCmdDrawIndexed`` gains an ``instanceCount`` argument the vertex shader
reads via ``gl_InstanceIndex``.
