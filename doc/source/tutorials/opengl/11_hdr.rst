11 - HDR + Bloom
================

The ladder's first **floating-point render target**. Every rung so far drew into an
8-bit-per-channel framebuffer, where colour is clamped to ``[0, 1]`` -- a value of
4.0 and a value of 1.0 are stored identically. This tutorial renders the scene into a
16-bit-float target (``RGBA16F``) where bright surfaces keep their true, above-one
intensity, builds a Karis-style **bloom** from those bright pixels, and tonemaps the
result back down to the display. It is a faithful port of the dasVulkan HDR rung,
expressed with WebGL2 framebuffers.

The scene is tutorial 05's thousand-cube swarm, but roughly one cube in nine is
flagged *emissive* and multiplies its lit colour by ``EMISSIVE_BOOST`` (= 8x) -- well
above 1.0. In an LDR target those cubes would clip to flat white; in the HDR target
they carry their real brightness into the post-processing chain, where the bloom
turns them into glowing light sources.

.. literalinclude:: ../../../../tutorials/opengl/11_hdr/11_hdr.das
   :language: das
   :start-at: scene shader globals

The float framebuffer
---------------------

``make_hdr_color`` allocates a colour texture with internal format ``GL_RGBA16F`` and
type ``GL_HALF_FLOAT``, then attaches it to a fresh framebuffer object. On WebGL2 a
float colour target needs the ``EXT_color_buffer_float`` extension to be
*color-renderable*; emscripten enables it automatically, so the framebuffer reports
``GL_FRAMEBUFFER_COMPLETE`` and the scene's 8x-boosted emissive values survive instead
of clamping. The scene framebuffer adds a depth renderbuffer so the 1000 cubes
depth-test correctly; the bloom mips are colour-only.

Five passes
-----------

Each pass except the first is a fullscreen post step -- one screen-covering triangle
through a shared pass-through vertex shader (``post_vs``):

1. **Scene** into ``hdr_color``. The instanced swarm, drawn exactly as in tutorial 05
   but writing HDR values into the float target.
2. **Bright pass** into bloom mip 0 (half resolution). A Frostbite soft-knee threshold
   isolates the bright pixels; the **Karis weight** ``1/(1+luma)`` crushes an 8x
   emissive value back near 1.0, so the bloom pyramid stays in a range bilinear
   filtering can smooth (without it, a single bright cube becomes a hard white square
   instead of a soft halo).
3. **Downsample** mip 0 → 1 → 2 → 3 → 4. A 5-tap "Karis-bilinear" filter halves the
   resolution each step, growing the blur radius cheaply.
4. **Upsample** mip 4 → 3 → 2 → 1 → 0, **additively blended**
   (``glBlendFunc(GL_ONE, GL_ONE)``). A 9-tap tent upsamples each smaller mip onto the
   larger one, accumulating a wide, smooth glow back up the pyramid.
5. **Composite** to the screen. Add bloom mip 0 onto the HDR scene, ACES-tonemap the
   sum into ``[0, 1]``, and gamma-encode.

The bloom mip chain is built once at init as a small array of ``RGBA16F`` textures and
framebuffers, each half the size of the previous; the per-pass ``u_src_rcp`` uniform
(``1 / source-mip size``) lets the one downsample shader and the one upsample shader
run at every level.

Two samplers at once
--------------------

The composite pass is the only one that reads two textures simultaneously -- the HDR
scene and the finished bloom. The GL bind for a ``sampler2D`` picks its texture unit
from the marker's ``@stage`` annotation, so ``src0`` (``@stage = 0``) and ``src_bloom``
(``@stage = 1``) land on distinct units and ``composite_fs`` reads both in one draw.

The GL-vs-Vulkan delta
----------------------

The Vulkan rung wrote *linear* values from the composite and relied on an sRGB swap
target to apply gamma encoding in hardware. WebGL2's default framebuffer does no sRGB
encode, so ``composite_fs`` gamma-encodes explicitly (``pow(mapped, 1/2.2)``) after the
ACES tonemap. That one line is the only real difference between this rung's shaders and
the Vulkan original.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/11_hdr/11_hdr.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered
to WebGL2: a swarm of cubes with glowing emissive members, every glow a real bloom
built from a float render target on your GPU.
