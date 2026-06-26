04 - The Synthwave Cube
=======================

The first **3D-geometry** rung. After three fullscreen-fragment tutorials, this
one draws an actual mesh: a textured, lit, breathing cube spinning while the
camera orbits it. It introduces everything 01-03 skipped -- a real vertex buffer
with per-vertex ``(pos, normal, uv)``, the model / view / projection matrix chain
(built host-side with ``daslib/math_boost`` and packed into a **std140 uniform
block**), depth testing + back-face culling, and a texture sampled in the fragment
shader.

It is a faithful port of the dasVulkan rung, with the same look: the cube wears a
procedural **synthwave horizon** texture -- a deep-purple-to-magenta sky with a
banded neon sun, a magenta horizon line, and a dark-indigo ground with a cyan
perspective grid -- generated entirely CPU-side at init (no asset file). The
fragment scrolls the UV vertically over time so the texture drifts like a CRT, and
a key + rim light pair shades it.

.. literalinclude:: ../../../../tutorials/opengl/04_cube/04_cube.das
   :language: das
   :start-at: var @in @location

Geometry, matrices, depth
-------------------------

The cube is 24 vertices (four per face, so each face gets its own normal and a
full ``0..1`` UV square) and 36 indices. ``bind_vertex_buffer(null, type<Vertex>)``
wires the interleaved ``(pos, normal, uv)`` layout to the three ``@in @location``
attributes by declaration order. The vertex shader transforms the position by
``proj * view * model`` and writes the world-space position and normal as
varyings; ``update()`` rebuilds the three matrices every frame with
``look_at_rh`` / ``perspective_rh_opengl`` / a quaternion ``compose``.
``glEnable(GL_DEPTH_TEST)`` plus a ``GL_DEPTH_BUFFER_BIT`` clear keep the back
faces hidden (the default clear-depth of 1.0 needs no explicit ``glClearDepth`` --
which is desktop-only anyway; GLES uses ``glClearDepthf``).

The camera uniform block
------------------------

The matrices and lighting constants are not separate uniforms -- they live in one
``struct Camera`` declared ``@uniform``::

    struct Camera {
        model   : float4x4
        view    : float4x4
        proj    : float4x4
        cam_pos : float3
        time    : float
    }
    var @uniform u_cam : Camera

dasGlsl lowers that to a ``layout(std140) uniform u_cam_ubo { ... } u_cam;``
interface block and the host bind uploads it through a single **uniform buffer**,
the GL/WebGL2-portable way to feed a shader a block of constants -- and the same
std140 path the dasVulkan rail uses, so the byte layout matches across both
backends. ``std140`` is a fixed cross-vendor packing rule, which is why
``cam_pos`` (a ``float3``) sitting right before ``time`` (a ``float``) lands at the
correct offsets even though their natural alignments differ; the host and shader
agree on the layout because they derive it from the same struct. The sampler
``u_tex`` stays a plain ``uniform sampler2D`` -- opaque types can't live in a UBO.

The texture
-----------

``gen_synthwave_texture`` builds a 256×256 ``RGBA8`` image in an
``array<uint8>`` -- pure arithmetic per pixel, no asset -- and
``load_image_from_bytes`` uploads it via ``glTexImage2D``. On the web this is the
first texture to cross into WebGL2: the CPU-generated pixels upload through the
same generated gl-call wrappers, and the camera block through the WebGL2
uniform-buffer calls (``glBindBufferRange`` / ``glUniformBlockBinding``), with no
special-casing.

The fragment lighting
---------------------

``fs_main`` normalizes the interpolated world normal, computes a warm key light
with an ambient floor and a cool-cyan rim term from the view direction, scrolls
the UV by ``u_cam.time``, samples the texture, and combines. The camera world
position (for the rim) and the time (for the scroll) are read as fields of the
same ``u_cam`` block the vertex stage uses -- the linker merges the two stages'
declarations into one buffer binding.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/04_cube/04_cube.das

In the browser, it runs live in the daslang playground -- the same ``.das``,
lowered to WebGL2, the textured cube spinning on your GPU.
