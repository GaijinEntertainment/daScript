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

Each fragment maps its clip-space position to a complex number *c*, iterates
*z = z² + c* until \|z\| > 2 or it hits ``MAX_ITER``, and colors the pixel from
the escape count. A per-frame ``u_zoom`` uniform breathes the view in and out so
the loop has a real GPU parameter; ``u_aspect`` keeps the set from stretching.

.. literalinclude:: ../../../../tutorials/opengl/02_mandelbrot/02_mandelbrot.das
   :language: das
   :start-at: var @in @location

The fullscreen quad
-------------------

There is no geometry to speak of: four clip-space corners ``(±1, ±1)`` and two
triangles' worth of indices cover the viewport edge to edge. The vertex stage
does nothing but pass the clip-space position through as a varying ``f_uv`` and
write ``gl_Position``; all the work happens per fragment.

The escape-time loop
--------------------

The fragment shader is the whole tutorial. It reconstructs *c* from the
interpolated ``f_uv`` (scaled by ``u_zoom`` and aspect-corrected so circles stay
round), then runs the classic ``while`` / ``break`` escape loop in daslang. This
is the first rung whose shader body is real control flow and scalar float
arithmetic rather than a single expression -- and it is exactly what surfaced a
GLSL-emitter gap: integer-valued float literals (``0.0``, ``4.0``, ``256.0``)
must keep their decimal point, because GLSL ES 3.00 (WebGL2) is strict about
``float`` vs ``int`` in scalar and binary-operator positions where the desktop
driver had been lenient.

The loop
--------

``update()`` advances a phase, ping-pongs ``u_zoom`` between a wide view and deep
filigree (easing toward the close-up so it dwells on the detail), reads the live
framebuffer size for the aspect correction, clears, binds the program and its
uniforms, and draws the two triangles. The fixed center -- the seahorse valley,
near the boundary where the structure lives -- is an inline constant in the
shader; only the zoom animates.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/02_mandelbrot/02_mandelbrot.das

In the browser, it runs live in the daslang playground -- the same ``.das``,
lowered to WebGL2, the escape loop running on your GPU.
