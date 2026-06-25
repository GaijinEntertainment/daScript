06 - The Skybox
===============

The first **cubemap** rung. A procedural sky surrounds the camera, and a mirror
sphere on a checkered floor reflects it -- so the same ``samplerCube`` is sampled
two ways: as a background (the skybox) and as an environment map (the reflection).
It is a faithful port of the dasVulkan skybox rung -- same procedural sky, same
reflective sphere and floor.

Two new ideas after tutorial 04's single 2D texture: the cubemap itself, and the
**depth=1.0 trick** that lets the skybox fill the background in one draw without
overdrawing the foreground.

.. literalinclude:: ../../../../tutorials/opengl/06_skybox/06_skybox.das
   :language: das
   :start-at: shared shader globals

The cubemap
-----------

A ``samplerCube`` is sampled by a 3-vector *direction* instead of a 2D UV. The
skybox is a unit cube centred on the camera; each vertex position doubles as the
world-space sampling direction, so ``skybox_fs`` samples the sky with
``texture(u_sky, normalize(v_dir))`` and never needs a UV. The mirror sphere
samples the *same* cubemap along the reflected view vector -- that is what makes
the reflection show the surrounding sky.

The six faces are generated on the CPU. ``sample_sky`` is a continuous function of
direction (a horizon-to-zenith gradient, a ground falloff, a sun disc and glow);
``face_direction`` maps each face's pixels to outgoing directions following the GL
cubemap convention, so the sky the shader samples is exactly the sky we baked.
``create_skybox_cubemap`` uploads each face into its own
``GL_TEXTURE_CUBE_MAP_POSITIVE_X + face`` target of one ``GL_TEXTURE_CUBE_MAP``
texture, with ``CLAMP_TO_EDGE`` on all three axes so the face edges do not bleed
across the seams.

On the host side, assigning a texture handle into a sampler global -- ``u_sky :=
sky_tex`` -- uses the ``samplerCube``-to-handle ``:=`` operator that this rung added
to ``glsl_common`` (the sampler-binding surface that the wider sampler types had
left incomplete). The generated ``*_bind_uniform`` then binds it through
``bind_sampler_cube``.

The depth=1.0 trick
-------------------

After the projection multiply, ``skybox_vs`` snaps ``clip.z = clip.w`` by rebuilding
the clip-space vector as ``float4(clip.x, clip.y, clip.w, clip.w)``. The
perspective divide then gives an NDC ``z`` of exactly ``1.0``: every skybox fragment
lives at the far plane.

The draw order does the rest. The foreground (sphere, then floor) draws first with
ordinary ``LESS`` depth testing and depth writes on. The skybox draws **last** with
the depth test set to ``LEQUAL`` and depth writes **off**, so its far-plane fragments
pass only where the depth buffer is still at its cleared ``1.0`` -- the pixels no
foreground covered. One background draw, no overdraw of what is in front of it.

(``glClear`` honours the depth write mask, so ``update`` re-enables depth writes with
``glDepthMask(true)`` before each clear, having turned them off for the skybox draw.)

Reflections
-----------

``sphere_fs`` reflects the eye ray about the surface normal with a small
``reflect_dir`` helper (emitted as a GLSL function, same as tutorial 03's SDF
helpers), samples the cubemap along it, and tints the result with a Schlick metal
Fresnel -- tinted reflectance at normal incidence ramping to white at grazing
angles. The host cycles the sphere's ``u_material`` through chrome, gold, copper and
steel over time. ``floor_fs`` is a checkerboard with a faint Fresnel sky reflection
so it reads as a polished surface grounding the sphere rather than a flat plane.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/06_skybox/06_skybox.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered
to WebGL2: a procedural cubemap sky with a mirror sphere reflecting it on your GPU.
