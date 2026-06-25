10 - Deferred Shading + MRT
===========================

The capstone rung, and the last one that renders: a faithful port of the dasVulkan
deferred tutorial. A **concrete cat statue** (CC0) sits on a **red brick floor**,
deferred-shaded with a real shadow map, screen-space ambient occlusion, and HDR
image-based lighting. Its headline is **multiple render targets** (MRT): a single
geometry pass writes *three* colour attachments at once -- the **G-buffer** -- and later
fullscreen passes read them back. Decoupling geometry from lighting is what lets deferred
shading afford many lights and many screen-space effects: the expensive per-pixel material
work runs once, then every light and every post pass is a cheap read over the screen.

This is also the first OpenGL rung to load **external assets** -- a Wavefront OBJ mesh and
PBR texture sets. The loading code is byte-identical to the desktop path; only the web
needs one extra step to make those files reachable (covered below).

.. literalinclude:: ../../../../tutorials/opengl/10_deferred/10_deferred.das
   :language: das
   :start-at: shared mesh inputs

Loading external assets
-----------------------

Every prior rung generated its geometry and textures procedurally. This one reads them
from disk::

    var frag <- load_obj_mesh(CAT_OBJ, false)
    ...
    tex_cat_diff = load_image_from_file(CAT_DIFF, false)
    tex_env      = load_hdr_texture(HDRI)        // stbi_loadf -> GL_RGB16F

``load_obj_mesh`` (from ``geometry/geom_gen``) and ``load_image_from_file`` (from
``opengl/opengl_boost``) are plain ``fopen`` underneath -- the *same* calls on desktop and
in the browser. The one web-vs-desktop difference is that the browser's in-memory
filesystem starts **empty**, so the asset bytes have to be written into it before the
program's ``fopen`` runs. The playground does that from a sidecar manifest
(``gl_10_deferred.das.assets.json``, an array of repo-relative paths): it fetches each file
over HTTP and writes it into the virtual filesystem at the exact path the tutorial expects.
The daslang code never changes -- the asset rail is entirely on the harness side.

The G-buffer and the MRT write
------------------------------

``setup_targets`` builds the G-buffer framebuffer with three colour attachments plus a
depth renderbuffer:

* attachment 0 -- ``GL_RGBA8``: albedo in ``.rgb``, specular intensity in ``.a``;
* attachment 1 -- ``GL_RGBA16F``: world-space (normal-mapped) normal in ``.xyz``, Blinn
  shininess in ``.a``;
* attachment 2 -- ``GL_RGBA16F``: world-space position in ``.xyz``, material tag in ``.a``.

Normals are signed and positions run outside ``[0, 1]``, so those two targets need the
float format (``GL_RGBA16F`` is colour-renderable on WebGL2, as tutorial 11 established).
The three attachments are enabled together::

    var draw_bufs = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2)
    glDrawBuffers(3, safe_addr(draw_bufs[0]))

Without ``glDrawBuffers`` only attachment 0 would receive output. With it, the geometry
fragment shader's three ``@out @location = N`` writes land in their matching attachments in
a single draw -- one geometry pass, the whole G-buffer.

That ``@location`` on a *fragment* output is load-bearing here. WebGL2 / GLSL ES 3.00
**requires** an explicit ``layout(location = N)`` on every fragment output once there is
more than one; a multi-output fragment shader without them is a compile error in the
browser (desktop ``#version 330`` is lenient and accepts the omission). The GLSL emitter
now carries the ``@location`` qualifier through to fragment outputs, not just vertex ones --
so ``@out @location = 0 g_albedo_out`` lowers to ``layout(location = 0) out vec4
g_albedo_out``.

Normals come from the textures, not the mesh: ``perturb_normal`` reconstructs a tangent
frame from the screen-space derivatives (``dFdx`` / ``dFdy``, from the shared shader module)
of world position and uv, so no per-vertex tangent buffer is needed.

Four passes
-----------

The frame is four passes, the same architecture the dasVulkan rung uses -- every piece
WebGL2-portable:

1. **Shadow** -- a depth-only render from the sun's point of view into a depth texture
   (the tutorial-08 pattern: ``GL_DEPTH_COMPONENT24``, ``COMPARE_REF_TO_TEXTURE``, colour
   draw buffer ``GL_NONE``, a polygon-offset to push back the depth and kill acne).
2. **Geometry** -- the cat and floor drawn once into the 3-attachment G-buffer.
3. **SSAO** -- a fullscreen pass that samples the G-buffer normal and position *as
   textures*, so it can read **neighbours**, and computes a hemisphere-kernel occlusion
   factor that darkens crevices and the cat-floor contact shadow.
4. **Lighting** -- a fullscreen pass that samples everything and composites the frame.

Passes 3 and 4 are fullscreen and share ``post_vs`` (a single covering triangle from a
small position VBO -- not ``gl_VertexID``, kept explicit for portability).

The lighting pass
-----------------

The lighting fragment shader reads the G-buffer back and reconstructs each surface --
albedo, world normal, shininess, world position -- then accumulates:

* one **shadowed directional sun**, sampled through a 5x5 PCF kernel
  (``textureCompare`` on the ``sampler2DShadow``). OpenGL's clip-space ``z`` is ``[-1, 1]``,
  so the light-space depth reference is ``ndc.z * 0.5 + 0.5`` -- the one genuine
  GL-vs-Vulkan shader delta (the Vulkan rung uses ``ndc.z`` directly, since its ``z`` is
  already ``[0, 1]``);
* three **orbiting coloured point lights**, computed procedurally from ``u_time`` and the
  loop index, so no per-light uniforms are needed -- this is the deferred dividend: each
  light is a cheap add over the screen, independent of scene complexity;
* **hemisphere ambient** blended with **HDR image-based lighting** -- the equirectangular
  environment sampled along the normal for diffuse and along the reflected view vector for
  a Fresnel-weighted specular term;
* the **SSAO** factor, modulating ambient and IBL.

Lighting is done in linear space: the sRGB albedo textures are decoded with ``pow(tex,
2.2)`` on the way in, and the final colour is gamma-encoded with ``pow(c, 0.4545)`` on the
way out, because the default GL framebuffer does no sRGB encode of its own.

Seeing the G-buffer
-------------------

A four-tile filmstrip along the bottom of the lighting pass previews the raw buffers the
earlier passes produced: albedo, the world normal remapped to ``n * 0.5 + 0.5``, world
position scaled into a visible range, and the SSAO factor. It is drawn by remapping
``l_uv`` into each tile and sampling the matching texture -- a direct window onto what the
geometry and SSAO passes wrote.

Where the portable floor ends
-----------------------------

This is the last rung that renders. The dasVulkan ladder continues into GPU-driven culling
and mesh shaders, which depend on compute and indirect draw -- capabilities WebGL2 has no
answer for at all. Deferred shading, by contrast, ports in full: shadow maps, float
targets, and neighbour-sampling SSAO are all things the earlier rungs already proved
WebGL2 can do, which is why the whole pipeline made the crossing intact. This is where the
portable floor ends.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/10_deferred/10_deferred.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered to
WebGL2, with the cat mesh and PBR textures fetched into the virtual filesystem first: a
concrete cat on a brick floor, lit by a shadow-casting sun and three drifting coloured
lights, with the raw G-buffer and SSAO channels previewed along the bottom.
