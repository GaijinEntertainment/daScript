08 - The Shadow Map
===================

The canonical **two-pass shadow-map** dance, ported from the dasVulkan shadow rung.
A rotating cube hovers over a checkered floor while a directional light orbits the
scene, and the cube casts a real, soft-edged shadow that sweeps across the floor as
the sun moves.

The whole trick is to render the scene **twice**. The first pass -- the *shadow
pass* -- renders only depth, from the light's point of view, into an offscreen
texture: for every light-space pixel, how far away is the closest caster. The
second pass -- the *main pass* -- renders the scene from the camera, and for each
fragment asks the shadow map "is anything closer to the light here than I am?" If
yes, the fragment is in shadow.

.. literalinclude:: ../../../../tutorials/opengl/08_shadow/08_shadow.das
   :language: das
   :start-at: shared shader globals

The depth texture, two roles
----------------------------

One depth texture serves both passes. ``create_shadow_target`` allocates a
``GL_DEPTH_COMPONENT24`` texture and -- the key line -- sets
``GL_TEXTURE_COMPARE_MODE`` to ``GL_COMPARE_REF_TO_TEXTURE``. That flips the texture
into *comparison* mode: sampling it through a ``sampler2DShadow`` does not return the
stored depth, it returns the result of comparing your reference value against the
stored depth (``GL_LEQUAL``), already 2x2-filtered by the hardware into a ``[0, 1]``
result. That is what ``textureCompare(shadow_map, uv, ref)`` lowers to. In pass 1
the same texture is the framebuffer's depth attachment; in pass 2 it is a sampler.

A **depth-only framebuffer** holds it: a framebuffer object with a depth attachment
and the colour draw buffer set to ``GL_NONE`` (``glDrawBuffers`` + ``glReadBuffer``).
The shadow pass writes no colour at all -- ``shadow_vs`` is the whole geometry stage,
and ``shadow_fs`` writes a colour that is simply discarded.

The shadow lookup
-----------------

``main_vs`` passes each vertex's position projected into light space
(``u_light_vp * world``) forward as a varying. In ``main_fs`` the perspective divide
gives light-space NDC. OpenGL NDC spans ``[-1, 1]`` on all three axes, so the ``xy``
is remapped to ``[0, 1]`` for the shadow-map UV and the ``z`` is remapped to
``[0, 1]`` for the depth reference -- matching the ``[0, 1]`` depth the shadow pass
stored. (The Vulkan rung leaves ``z`` as-is because Vulkan's NDC ``z`` is already
``[0, 1]``; this remap is the one real GL-vs-Vulkan difference in the shader.)

``pcf_shadow`` then averages a 3x3 grid of ``textureCompare`` taps. Each tap is
already a hardware 2x2 depth compare, so the effective footprint is ~36 samples --
soft penumbra edges instead of a hard binary line.

Killing shadow acne
-------------------

Without a depth bias, every surface slightly self-shadows: the depth it stored in
pass 1 and the depth it computes in pass 2 differ by tiny amounts, and the
comparison flickers. Two biases fix it: ``glPolygonOffset`` during the shadow pass
pushes caster depth away from the light, and the fragment shader adds a small
N·L-scaled bias (grazing faces need more) with a floor so even head-on faces get a
touch. Faces turned away from the light (``N·L < 0.05``) skip the lookup entirely
and are simply dark, avoiding bias artifacts where the comparison is meaningless.

An emitter note: GLSL ES 3.00 predeclares a default precision for ``sampler2D`` and
``samplerCube`` in fragment shaders, but **not** for ``sampler2DShadow`` (nor
``sampler3D`` / ``sampler2DArray``). dasGlsl emits a ``precision highp
sampler2DShadow;`` line in the ES 3.00 preamble so the shadow sampler compiles on
the web -- a hole this tutorial found and the emitter now plugs.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/08_shadow/08_shadow.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered
to WebGL2: a cube casting a soft shadow that sweeps the floor as the light orbits,
rendered with a real depth-texture shadow map on your GPU.
