02 - The Mandelbrot Set (compute)
=================================

The first tutorial drew a triangle with the graphics pipeline. This one computes
an image with the **compute** pipeline: a ``[compute_shader]`` runs one invocation
per pixel, maps the pixel to a complex number *c*, iterates ``z = z*z + c`` until it
escapes or hits an iteration cap, and writes a colour into a storage image. Every
line is **daslang**, lowered to SPIR-V at compile time by dasSpirv -- the escape-time
loop exercises the compute tier's control flow (``while`` + ``break``) and float
arithmetic. Two newer dasSpirv rails show up here: ``imageSize`` (the shader sizes its
pixel→complex mapping off the bound image) and module-scope ``let`` constants (the
view rectangle and iteration cap live at module scope and are shared with the host).

.. video:: vulkan_mandelbrot.mp4

The clip above is the **animated zoom** viewer (``window/show_mandelbrot_blit.das``)
rendered headlessly into an APNG and muxed with a strudel music bed -- one
``[vulkan_compute_shader]`` dispatch per frame, a single ``time`` push constant
driving both zoom and rotation about the seahorse-valley point.

The shader
----------

A single ``[compute_shader]`` with an ``8x8`` local size. The output storage image is a
``var @binding = 0 out_img : image2D``; ``imageStore`` writes one texel per invocation.
The escape-time loop is ordinary daslang.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/mandelbrot_tut_shaders.das
   :language: das
   :start-at: module mandelbrot_tut_shaders

The render (headless)
---------------------

``render_mandelbrot()`` feeds the emitted ``mandelbrot_spv`` blob to dasVulkan's
``compute_to_storage_image`` boost helper: it creates the storage image, the
descriptor set and the compute pipeline, dispatches ``DIM/8 x DIM/8``, and reads the
result back to RGBA8 pixels on the host -- a pure ``() -> image``. This is what the
CI test checks; no window required.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/mandelbrot_tut.das
   :language: das
   :start-at: def public render_mandelbrot

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, a real GPU locally). Instead of a
golden image it uses an **analytic oracle**: points inside the set never escape (black),
points well outside escape (coloured), and because *c* and its conjugate escape
identically, the image is mirror-symmetric across the real axis -- a float-robust check
on deep in/out pairs and reflected coordinates.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/test_mandelbrot.das
   :language: das
   :start-at: [test]

See it live -- an animated zoom, two ways to present it
-------------------------------------------------------

The static render above is the verified core. The live viewers are livelier: an animated,
antialiased **zoom** -- a port of Inigo Quilez's smooth-coloured Mandelbrot
(`ldf3DN <https://www.shadertoy.com/view/ldf3DN>`_). The whole animation is a single
``@push_constant`` ``float`` -- a ``time`` the viewer feeds from wall-clock each frame -- from
which the shader derives an oscillating zoom and a slow rotation about the seahorse-valley point
``(-0.745, 0.186)``. Crucially the fractal is **recomputed every frame**, so the zoom reveals real
new detail rather than scaling a static texture; 2x2 supersampling antialiases it. It is its own
viewer-only dasSpirv shader, so the verified static core stays untouched, and it exercises
``imageSize``, module-scope ``let`` constants, a compute ``@push_constant`` and the GLSL.std.450
math rail (``cos`` / ``sin`` / ``pow`` / ``log2``):

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/window/mandelbrot_zoom_shaders.das
   :language: das
   :start-at: [vulkan_compute_shader

The compute result still lives in a *storage image*, off-screen, and still has to reach the
swapchain -- a triangle draws straight into it through a render pass, a compute result does not.
There are two standard ways, and a runnable viewer for each. Both build the resident compute
resources once (``build_mandel_compute``) and re-dispatch them each frame with the new ``time``
(``record_compute``); they live in a ``window/`` subfolder the CI gate skips (CI is headless and
built without GLFW). ``record_compute`` writes ``pc.time = time`` and calls the macro-generated
``mandelbrot_zoom_push_constants(cmd, mc.pipe_layout)``; ``[vulkan_compute_shader]`` synthesised
that helper from the shader's ``@push_constant pc : Push`` declaration.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/window/mandelbrot_compute.das
   :language: das
   :start-at: def public record_compute

Method 1 -- blit
~~~~~~~~~~~~~~~~~

The most direct route: ``vkCmdBlitImage`` copies the storage image straight onto the acquired
swapchain image (scaled to the window, linear filter) -- no graphics pipeline, no render pass, no
fragment shader. Both the per-frame compute dispatch and the blit are recorded into vulkan_window's
``present_frame`` (the non-render-pass sibling of ``draw_frame``):

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/window/show_mandelbrot_blit.das
   :language: das
   :start-at: let ok = present_frame
   :end-before: if (!ok)

Method 2 -- sample as a texture
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The other route reuses the entire graphics path: draw one full-screen triangle whose fragment
shader **samples** the storage image as a texture -- the canonical "compute writes, graphics reads"
pattern, through the ordinary render pass, reusing ``draw_frame``, the framebuffers and the graphics
pipeline wholesale. Because a compute dispatch cannot run inside a render pass, each frame runs the
compute as its own submit first, then ``draw_frame`` samples. The two view shaders:

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/02_mandelbrot/window/mandelbrot_view_shaders.das
   :language: das
   :start-at: var @out @location = 0 v_uv

Blit is fewer moving parts; sample-as-texture is the one to grow from (a real post-process or UI
pass samples the same way).

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/02_mandelbrot

   # watch it live -- method 1: blit straight to the swapchain
   daslang.exe modules/dasVulkan/tutorials/02_mandelbrot/window/show_mandelbrot_blit.das

   # watch it live -- method 2: sample the result as a texture
   daslang.exe modules/dasVulkan/tutorials/02_mandelbrot/window/show_mandelbrot_sampled.das

Next
----

:doc:`03_sdf` stays on the compute pipeline but trades the 2D fractal for a
real 3D scene: a ray-marched signed distance field rendering primitives,
soft shadows, and ambient occlusion from a fragment-style shader.
