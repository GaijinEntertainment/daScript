02 - The Mandelbrot Set
=======================

The :doc:`dasVulkan Mandelbrot rung <01_triangle>` computes the set in a
**compute shader** -- one invocation per pixel, writing into a storage image.
WebGL2 has neither compute shaders nor storage images, so the portable OpenGL
idiom is a **fullscreen fragment shader**: draw two triangles covering the whole
viewport and let the *fragment* stage run the escape-time loop once per pixel.
Same math, same daslang shader language -- a different backend, and a different
"one thread per pixel" vehicle. Drawing that substitution explicitly is part of
the harmonization story.

The shader is a faithful port of the Vulkan tutorial's animated viewer, itself a
port of `Inigo Quilez's smooth-coloured Mandelbrot zoom
<https://www.shadertoy.com/view/ldf3DN>`_: a single ``u_time`` uniform drives an
oscillating zoom and a slow rotation about the seahorse-valley point
``(-0.745, 0.186)``. A large bailout radius plus a continuous (smooth) iteration
count kills the banding, an IQ cosine palette colours it, and 4×4 supersampling
antialiases the boundary.

.. literalinclude:: ../../../../tutorials/opengl/02_mandelbrot/02_mandelbrot.das
   :language: das
   :start-at: var @in @location

The fullscreen quad
-------------------

There is no geometry to speak of: four clip-space corners ``(±1, ±1)`` and two
triangles' worth of indices cover the viewport edge to edge. The vertex stage
does nothing but pass the clip-space position through as a varying ``f_uv`` and
write ``gl_Position``; all the work happens per fragment.

Smooth colouring
----------------

Each fragment reconstructs the complex point *c* from ``f_uv`` (widened by
``u_aspect``, then zoomed and rotated about the seahorse valley), and -- after
the cheap cardioid/bulb interior tests -- runs the escape loop with a large
bailout radius. The *continuous* iteration count ``n - log2(log2(|z|²)) + 4``
removes the integer banding, and an IQ cosine palette maps it to colour. This is
the first rung whose shader body is real control flow, scalar float arithmetic,
and a fistful of user-defined helper functions (``cmul`` / ``is_interior`` /
``mandel_smooth`` / ``palette`` / ``sample_at``) -- all lowered to GLSL by
dasGlsl. It is also what surfaced a GLSL-emitter gap: integer-valued float
literals (``0.0``, ``4.0``, ``256.0``) must keep their decimal point, because
GLSL ES 3.00 (WebGL2) is strict about ``float`` vs ``int`` in scalar and
binary-operator positions where the desktop driver had been lenient.

The loop
--------

``update()`` advances a time value, reads the live framebuffer size (so the
aspect correction and supersample spacing follow window/canvas resizes), clears,
binds the program and its uniforms, and draws the two triangles. The whole
animation -- the breathing zoom and the slow rotation -- is derived inside the
shader from ``u_time`` alone, so the loop has one real GPU parameter.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/02_mandelbrot/02_mandelbrot.das

In the browser, it runs live in the daslang playground -- the same ``.das``,
lowered to WebGL2, the smooth-coloured zoom running on your GPU.
