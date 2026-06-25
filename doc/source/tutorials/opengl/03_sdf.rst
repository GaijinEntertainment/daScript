03 - Signed-Distance-Field Raymarcher
=====================================

The :doc:`dasVulkan SDF rung <02_mandelbrot>` raymarches the scene in a
**compute shader**, one invocation per pixel, into a storage image. WebGL2 has no
compute, so -- exactly as for the :doc:`Mandelbrot rung <02_mandelbrot>` -- the
portable GL idiom is a **fullscreen fragment shader**: a clip-space quad, with the
whole raymarch running once per fragment.

The scene is pure math -- a smooth-blended sphere and an orbiting torus floating
above a checkered plane -- shaded with key light + hemisphere ambient + IQ soft
shadow + a rim term + atmospheric fog, then Reinhard tone-mapped. The colour
scheme and shading are a **faithful port of the Vulkan rung**; only the vehicle
(compute → fragment) and the pixel→ray mapping change.

.. literalinclude:: ../../../../tutorials/opengl/03_sdf/03_sdf.das
   :language: das
   :start-at: var @in @location

A shader that reads like ordinary code
--------------------------------------

This rung is the one that really exercises the GLSL emitter. The shader is
authored as a dozen-plus ordinary daslang functions -- SDF primitives
(``sd_sphere`` / ``sd_torus`` / ``sd_plane``), IQ's smooth-union ``smin``, the
scene ``map``, a forward-difference ``get_normal``, the ``march`` and
``soft_shadow`` loops, and the ``sky_color`` / ``ground_color`` / ``blob_color`` /
``tonemap`` shading -- all of which dasGlsl emits as GLSL functions (with forward
declarations) ahead of ``main()``. It leans on a wide intrinsic surface
(``length`` / ``normalize`` / ``cross`` / ``dot`` / ``floor`` / ``pow`` / ``exp`` /
``cos`` / ``sin`` / ``clamp`` / ``min`` / ``max`` / ``mix``), 2- and 3-component
swizzles, and ``for`` / ``break`` control flow -- the daslang ``for (i in
range(N))`` lowering to a C-style GLSL loop.

The raymarch
------------

``fs_main`` builds a camera that orbits the origin (the orbit and the blob's
motion are derived from ``u_time``), shoots one ray per fragment through a pinhole
lens, and marches it through ``map`` until it hits a surface or escapes to the far
plane. On a hit it computes the normal, picks the material's albedo, and shades;
on a miss it samples the sky gradient. Distance fog blends the two, and the result
is tone-mapped to ``[0,1]``.

The loop
--------

``update()`` advances ``u_time``, reads the live framebuffer size for the aspect
correction, clears, binds the program and its uniforms, and draws the quad. The
whole animation -- camera orbit, torus rotation, sphere bob, palette pulse -- is
driven from ``u_time`` inside the shader.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/03_sdf/03_sdf.das

In the browser, it runs live in the daslang playground -- the same ``.das``,
lowered to WebGL2, the raymarch running once per fragment on your GPU.
