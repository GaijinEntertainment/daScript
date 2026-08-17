09 - MSAA + Dynamic Rendering: No Render Pass, Smooth Edges
===========================================================

:doc:`08_shadow` was the last tutorial built on ``VkRenderPass`` +
``VkFramebuffer``. This one drops both: the pipeline carries a
``VkPipelineRenderingCreateInfo`` in its pNext chain (color + depth format +
sample count), and each frame opens a ``VkRenderingInfo`` inline with
``cmd_begin_rendering`` / ``cmd_end_rendering``. Vulkan 1.3 core. While we're
re-doing the cube scene from :doc:`04_cube`, we also turn on **4× MSAA** with
an auto-resolve target -- the GPU averages 4 samples per pixel into a 1×
image at the end of the render. The headline rails:

- **No more ``VkRenderPass`` / ``VkFramebuffer``.** The pipeline carries
  ``VkPipelineRenderingCreateInfo`` via pNext (one color format, one depth
  format, sample count). Per-frame, ``record_rendering`` opens an inline
  ``VkRenderingInfo`` with attachment views + clear values and the body
  records draws. ``create_graphics_pipeline_dyn`` is the new boost helper;
  the existing ``create_graphics_pipeline_v3d`` (render-pass-bound) stays
  available for legacy code.
- **4× MSAA + auto-resolve.** The color attachment is built at samples = 4
  with ``build_offscreen_target(..., samples, extra_usage)``. The
  ``RenderingAttachmentInfo`` carries ``resolveMode = AVERAGE`` and a
  ``resolveImageView`` pointing at a separate 1× image -- the driver does
  the box filter for free at end-of-render. The 1× target is what gets
  blitted into the readback buffer or swapchain target. The depth
  attachment matches at samples = 4 (the spec requires color + depth to
  agree).
- **Vulkan 1.3 feature opt-in.** Dynamic rendering became core in 1.3 but
  is still a feature toggle; the device-create call goes through a new
  ``create_device`` overload that takes
  ``VkPhysicalDeviceVulkan13Features`` and threads it via the
  ``pNext`` chain. Caller sets ``f13.dynamicRendering = 1u`` and we're done.
- **Built-in side-by-side comparison.** The cube uses **two** pipelines
  built from the same shader modules -- one 4× MSAA, one 1× -- each with
  matching color + depth attachments. ``record_msaa_render(res, cmd,
  use_msaa)`` picks which to use per frame. A 16-row mode-indicator strip
  cleared via a second ``record_rendering`` pass (no shader, no draw -- the
  CLEAR loadOp does all the work) sits at the top of every frame: bright
  green = 4× MSAA, bright red = 1× rasterization. The recording driver
  defaults to **AUTO** mode (flip every 2 s) so the comparison shows up
  automatically; the windowed viewer adds an M-key cycle (AUTO → OFF → ON).
- **Pixel oracle that proves MSAA actually averages.** The test scans 21
  horizontal lines across the cube's vertical extent and counts pixels in
  the partial-coverage gradient band (B ∈ [18, 200]). 1× rasterization
  produces **zero** such pixels by construction; 4× MSAA produces dozens.
  Floor on the count is the structural assert.

Every line of every shader is daslang, lowered to SPIR-V at compile time.
The shaders themselves are unchanged from :doc:`04_cube` -- MSAA is a
pipeline + attachment concern, the fragment program doesn't know it's being
sampled four times.

.. video:: vulkan_msaa.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into
an APNG and ffmpeg-muxed with a daStrudel music bed.
The 16-row mode strip at the top flips every 2 s -- watch the cube's edges
go from jaggy (red strip) to smooth (green strip) and back. The ``[test]``
checks the structural MSAA signal at a fixed frame -- the CI regression gate.
To watch the same scene live on your own GPU with M-key control, run the
windowed viewer (see `See it live`_ below).

The shaders
-----------

Reused from tutorial 04 -- a vertex shader pulling (pos, uv, normal) per-vertex
and writing world-space outputs, plus a fragment shader doing texture +
key+rim lighting. The MSAA happens at the pipeline + attachment level, the
fragment program doesn't see it.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/09_msaa/msaa_tut_shaders.das
   :language: das
   :start-at: module msaa_tut_shaders

The render (headless)
---------------------

The host builds **two** pipelines (4× MSAA + 1×), three color/depth attachment
sets (MSAA + resolve + 1×), and one descriptor set; the per-frame call
``record_msaa_render(res, cmd, use_msaa)`` picks the path. No
``VkRenderPass`` and no ``VkFramebuffer`` ever -- both rails replaced by
``VkPipelineRenderingCreateInfo`` on the pipeline and ``cmd_begin_rendering``
in the command buffer.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/09_msaa/msaa_tut.das
   :language: das
   :start-at: def public build_msaa_resources
   :end-before: def public build_msaa_context

``record_msaa_render`` is the per-frame work: it picks the 4× MSAA path or
the 1× path based on ``use_msaa``, then draws the indicator strip via a
second ``record_rendering`` block (CLEAR loadOp -- no draws inside).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/09_msaa/msaa_tut.das
   :language: das
   :start-at: def public record_msaa_render
   :end-before: //! Per-frame work

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). It
renders one frame at fixed ``time = 1.0`` + ``camera_t = 0.18`` and checks
the four basic structural properties (corner background, central cube
hits, magenta + cyan texture sampling) **plus** the MSAA-specific edge-mix
count -- the load-bearing visual element of this tutorial.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/09_msaa/test_msaa.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_msaa.das`` opens a GLFW window with a Vulkan swapchain and
runs the scene per frame with ``time`` derived from wall-clock. The window
title shows the current toggle state ("M = AUTO/OFF/ON | drawing: MSAA 4x
/ 1x (no AA)"); pressing **M** cycles between the three states. Default
is AUTO, which flips MSAA on and off every 2 seconds. The indicator strip
at the top of the frame is the same one the recording uses.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/09_msaa/window/show_msaa.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/09_msaa

   # watch it live in a window with the M-key cycle (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/09_msaa/window/show_msaa.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/09_msaa/recording/record_msaa.das

Next
----

:doc:`10_deferred` is the **putting-it-all-together** payoff scene. ONE
multi-subpass render pass with SIX attachments and THREE subpasses:
G-buffer (MRT writes albedo + normal + world-pos), SSAO (reads
subpassInputs, writes AO), and lighting (composes everything plus the
:doc:`08_shadow` shadow rail and three orbiting coloured point lights).
Per-pixel cost stays constant in light count -- the deferred payoff. The
M-key cycles 7 toggle states to decompose the final pixel into its
G-buffer, SSAO, and lighting-only components.
