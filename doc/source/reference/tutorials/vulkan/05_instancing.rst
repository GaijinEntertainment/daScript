05 - Instancing: A Thousand Cubes (one draw call)
==================================================

The first four tutorials drew at most one piece of geometry per draw call. This
one draws **a thousand cubes from one indexed-instanced draw**: a single
``vkCmdDrawIndexed(N_INDICES, N_INSTANCES, ...)`` fans the same 36 cube indices
over 1000 instances, and each cube reads its world offset, colour tint, and
breathing phase from a **per-instance vertex attribute stream**. The headline
rails:

- **two vertex bindings, two input rates** -- binding 0 stays per-vertex
  (``VkVertexInputRate.VERTEX``), binding 1 is per-instance
  (``VkVertexInputRate.INSTANCE``). Same pipeline, two strides, two memory
  buffers, one draw call;
- **per-instance vertex attributes** at ``@in @location = 2/3/4`` (offset,
  colour, phase) -- ordinary daslang stage inputs, the instancing is purely a
  host-side binding decision;
- a **static instance buffer** built once at init from a 3D Lissajous-style
  sweep (7 azimuthal lobes against 3 elevation lobes) so no per-frame buffer
  rewrite is needed -- the swarm animates entirely in the vertex shader by
  mixing the instance offset with ``cam.cam_time.w``;
- a wider draw budget than the cube tutorial -- 1000 cubes
  × 36 indices × 4 attributes interleaved through one pipeline, still well
  under any practical limit on a desktop GPU.

Every line of the shader is daslang, lowered to SPIR-V at compile time by dasSpirv.

.. video:: vulkan_instancing.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel music bed. The
camera orbits the swarm once per 30 seconds; each cube breathes at its own
per-instance phase so the swarm pulses like a slow chorus rather than a unison
heartbeat. See ``skills/internal/vulkan_recording.md``. To watch it live on your own GPU, run
the windowed viewer (see `See it live`_ below).

The shaders
-----------

The vertex shader reads per-vertex ``(a_pos, a_normal)`` from binding 0 and
per-instance ``(a_offset, a_color, a_phase)`` from binding 1. It applies a
per-instance breathing scale, a per-instance Y rotation desynchronised by
``a_phase``, the instance offset, and the camera transform. The fragment
shader does a simple key-light + rim shade modulated by the per-instance
colour -- no texture, no sampler, the headline is per-instance attributes,
not lighting. ``[vulkan_*_shader]`` synthesises ``instancing_vs_bind_uniform``
from ``var @uniform cam : Camera``; there are no push constants this round
because the per-instance buffer carries everything else.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/05_instancing/instancing_tut_shaders.das
   :language: das
   :start-at: module instancing_tut_shaders

The render (headless)
---------------------

Per-frame work splits into the same two halves as tutorial 04. ``update_instancing_uniforms``
rewrites the UBO with the new camera matrices + ``time``. ``record_instancing_render_pass``
binds the pipeline + descriptor set, **binds both vertex buffers in one
``vkCmdBindVertexBuffers`` call** (binding 0 is per-vertex, binding 1 is
per-instance), binds the index buffer, and issues
``vkCmdDrawIndexed(N_INDICES, N_INSTANCES, ...)``. The instance buffer is
static, set up once in ``build_instancing_resources`` from the Lissajous sweep
``gen_instance_data`` produces.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/05_instancing/instancing_tut.das
   :language: das
   :start-at: def public update_instancing_uniforms
   :end-before: //! Per-frame work

``render_instancing_frame`` is what the headless path uses: call the two
helpers inside a ``run_cmd_sync``, then copy the colour attachment into the
readback buffer and clone it out. ``render_instance_swarm`` is the one-shot
wrapper the test calls; the recording driver builds the context once and
calls ``render_instancing_frame`` in a loop.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/05_instancing/instancing_tut.das
   :language: das
   :start-at: def public render_instancing_frame
   :end-before: //! One-shot:

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). It
asserts the swarm is **scattered** (frame corners are clear-colour
background), **dense** (the central 400×400 region is at least a quarter cube
hits), and **palette-cycling** (a warm-red instance and a cool-blue instance
both exist somewhere in the central band, proving the hue rotation across the
swarm reached the rendered pixels).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/05_instancing/test_instancing.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_instancing.das`` opens a GLFW window with a Vulkan swapchain and
presents the swarm every frame. It owns its own instance (with surface
extensions) + device (with ``VK_KHR_swapchain``), then calls
``build_instancing_resources`` to share the offscreen render pass, framebuffer
and graphics pipeline with the headless oracle. Each frame it runs
``update_instancing_uniforms`` + ``record_instancing_render_pass`` into the
present command buffer, then blits the colour attachment onto the swapchain
image. It needs a display and the ``glfw`` module, so it lives in a
``window/`` subfolder that the tutorial's CI gate skips (CI is headless and
built without GLFW).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/05_instancing/window/show_instancing.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/05_instancing

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/05_instancing/window/show_instancing.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/05_instancing/recording/record_instancing.das

Next
----

:doc:`06_skybox` wraps the scene in an **environment** — a 6-face cubemap
rendered as a unit cube behind everything else. The shader uses a
``samplerCube`` (a daslang sampler type that lowers to ``OpTypeImage`` with
``Cube`` dim), and a depth-test-only / depth-write-disabled rail keeps the
sky from clipping the geometry.
