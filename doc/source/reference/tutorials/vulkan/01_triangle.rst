01 - The Rotating Triangle
==========================

The canonical "hello triangle" -- three vertices, per-vertex red/green/blue,
interpolated across the face -- but with every line of the shader written in
**daslang** and lowered to SPIR-V at compile time by dasSpirv. No GLSL, no
glslang, no committed ``.spv``. A push-constant ``angle`` spins it, so the
recording has a real per-frame GPU parameter to drive.

.. video:: vulkan_triangle.mp4

The clip above is the offscreen render -- the same pixels the CI test checks --
recorded to a video. To watch it spin live in a resizable window on your own GPU,
run the windowed viewer (see `See it live`_ below).

See it live
-----------

``window/show_triangle.das`` opens a GLFW window with a Vulkan swapchain and
presents the spinning triangle every frame (FIFO present), driving the same
``SpinPush`` angle from wall-clock time. It reuses the exact ``tri_spin_*_spv``
blobs the offscreen render and the CI test use -- the only additions are the
window, surface and swapchain. It needs a display and the ``glfw`` module, so it
lives in a ``window/`` subfolder that the tutorial's CI gate skips (CI is headless
and built without GLFW); it is the run-and-watch companion to the headless oracle.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/01_triangle/window/show_triangle.das
   :language: das
   :start-at: require glfw/glfw_boost

The shader
----------

Both stages are plain daslang functions tagged ``[vulkan_vertex_shader]`` /
``[vulkan_fragment_shader]``. The emitted ``tri_spin_vert_spv`` /
``tri_spin_frag_spv`` are ``array<uint>`` SPIR-V word blobs, fed straight to
``create_shader_module``. The vertex stage reads the rotation angle from a
``@push_constant`` struct, builds a 2D rotation, and spins the hardcoded
clip-space positions; the fragment stage just writes the interpolated varying.
The ``vulkan_*_shader`` annotations also synthesise a per-shader
``<shader>_push_constants(cb, layout)`` helper that does the
``vkCmdPushConstants`` call for the host -- see *The render* below.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/01_triangle/triangle_tut_shaders.das
   :language: das

The render
----------

The offscreen render is the dasVulkan boost path -- an offscreen color target, a
single-color render pass, a graphics pipeline -- with one addition over a static
triangle: a vertex **push-constant range** carrying the angle, pushed each draw
through the macro-generated ``tri_spin_vert_push_constants(cmd, layout)``. The
host just writes ``pc.angle = angle`` to the shader's ``@push_constant`` global;
``[vulkan_vertex_shader]`` synthesised the rest of the upload at compile time.
``render_spin_triangle(angle)`` returns the RGBA8 pixels, a pure parametric
``frame(angle) -> image``.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/01_triangle/triangle_tut.das
   :language: das
   :start-at: def public render_spin_triangle

Self-verifying
--------------

The tutorial's test is the CI regression gate -- it runs on lavapipe in CI and a
real GPU locally. At ``angle = 0`` the geometry matches the classic static
triangle, so the proven sample points hold (red top, green/blue bottom corners,
an interpolated centroid). At ``angle = pi`` the red vertex must rotate to
bottom-center and leave the top sample, proving the push-constant actually drives
the vertex shader on the GPU.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/01_triangle/test_triangle.das
   :language: das
   :start-at: [test]

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/01_triangle

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/01_triangle/window/show_triangle.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/01_triangle/recording/record_triangle.das

Next
----

:doc:`02_mandelbrot` swaps the graphics pipeline for the **compute** pipeline:
the shader writes pixel colors directly to a storage image via ``imageStore``,
no rasterizer involved. Same daslang→SPIR-V rails, different GPU stage.
