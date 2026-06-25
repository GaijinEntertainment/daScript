01 - The Rotating Triangle
==========================

The canonical "hello triangle" -- three vertices, per-vertex black/yellow/red,
interpolated across the face -- but with every line of the shader written in
**daslang** and lowered to GLSL (GLSL ES 3.00 on the web) at compile time by
dasGlsl. No hand-written GLSL, no committed shader strings. A per-frame
``u_angle`` uniform spins it, so the loop has a real GPU parameter, and
``u_aspect`` keeps it square at any window or canvas size.

This is the first rung of an OpenGL / WebGL2 tutorial ladder that mirrors the
:doc:`dasVulkan series </tutorials/opengl/index>` shader-for-shader: the same
daslang shader language, the same modern builtins, lowered to a *third* backend.

One homogeneous program
-----------------------

Unlike the Vulkan tutorials -- which need a separate headless render and a
windowed viewer -- an OpenGL tutorial is **one** file that runs on both the
desktop and the web. It exposes the three lifecycle functions ``init`` /
``update`` / ``shutdown`` plus a ``main`` driver:

* On the **desktop**, ``main()`` runs ``init(); while(update()){}; shutdown()``
  in a GLFW window.
* On the **web**, the wasm run path calls ``init()`` once, then drives
  ``update()`` from the browser's animation frame (``requestAnimationFrame``),
  and the daslang ``Context`` persists across frames. ``main()`` is never called.

The same ``.das``, byte-for-byte, both places.

.. literalinclude:: ../../../../tutorials/opengl/01_triangle/01_triangle.das
   :language: das
   :start-at: var @in @location

The shader
----------

Both stages are plain daslang functions tagged ``[vertex_program]`` /
``[fragment_program]``. ``u_angle`` and ``u_aspect`` are ``@uniform`` globals;
the annotations synthesise a per-shader ``<shader>_bind_uniform(program)`` helper
that uploads them for the host each frame -- you set the global, the generated
bind does the ``glUniform`` call. The vertex stage reads ``u_angle``, builds a 2D
rotation, spins the hardcoded clip-space positions, and divides ``x`` by
``u_aspect`` so the triangle stays square; the fragment stage just writes the
interpolated varying.

The loop
--------

``update()`` advances the angle, reads the live framebuffer size (so the aspect
correction follows window/canvas resizes), clears, binds the program and its
uniforms, and draws the three vertices. It returns ``true`` to keep going;
``false`` (the window was asked to close) ends the loop on the desktop. On the
web a ``void`` or ``true``-returning ``update`` simply runs until the page is
closed.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/01_triangle/01_triangle.das

In the browser, it runs live in the daslang playground (the canvas-backed
graphics samples) -- the same ``.das``, lowered to WebGL2.
