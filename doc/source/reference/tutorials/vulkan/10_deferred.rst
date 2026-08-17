10 - Deferred Shading: Putting It All Together
==============================================

Tutorials 1-9 each introduced one rail (triangle, mandelbrot SDF, cube, instancing,
skybox, particles, shadow map, MSAA + dynamic rendering). This one is the
**payoff scene**: a cube on a brick floor, lit by ONE shadowed directional sun
plus THREE orbiting coloured point lights, composed via **deferred shading** --
all in pure daslang lowered to SPIR-V at compile time.

The headline rails:

- **G-buffer + multi-subpass render pass.** ONE render pass with SIX
  attachments and THREE subpasses. Subpass 0 (the G-buffer pass) writes
  THREE colour outputs simultaneously -- albedo (RGBA8 with specular
  intensity in alpha), perturbed world-space normal (RGBA16F with Blinn
  shininess in alpha), and world position (RGBA16F with material tag in
  alpha) -- plus depth. Subpass 1 (SSAO) reads the G-buffer normal +
  world-pos as ``subpassInput`` and writes a single-channel R8 AO factor.
  Subpass 2 (lighting) reads ALL FOUR G-buffer attachments as
  ``subpassInput`` and writes the final composed colour. Two explicit
  ``VkSubpassDependency`` arcs order the writes-then-reads. The lighting
  pass additionally samples the shadow map and reads a UBO of point lights.
- **MRT (multiple render targets) in one shader.** The G-buffer fragment
  shader declares three ``@out @location`` outputs and writes them all in
  one ``main`` -- the dasSpirv emitter rail PR #3195 enables this. The
  three colour-blend attachments on the pipeline match the three render
  pass attachment indices.
- **``subpassInput`` + ``subpassLoad``.** The SSAO + lighting fragment
  shaders declare opaque ``subpassInput`` globals and sample them with
  ``subpassLoad`` -- the dasSpirv emitter rail PR #3196 enables this. Each
  ``subpassInput`` lowers to ``OpTypeImage Dim=SubpassData`` (no format),
  decorated with ``InputAttachmentIndex``, requiring the ``InputAttachment``
  SPIR-V capability. The descriptor type is
  ``VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT``.
- **Shadow map from :doc:`08_shadow`.** The shadow pass + sampler2DShadow
  + 5×5 PCF rail is lifted verbatim from tutorial 8. The 1024² shadow map
  is rendered ONCE per frame at the start, transitions to
  ``DEPTH_STENCIL_READ_ONLY_OPTIMAL`` at end-of-pass, and is sampled in
  the lighting subpass via a regular ``COMBINED_IMAGE_SAMPLER`` descriptor.
- **Three orbiting coloured point lights.** Packed in the scene UBO
  (six float4 fields: position+range, colour+falloff exponent, ×3). The
  lighting fragment evaluates them in a small helper function with a
  branchless attenuation curve. **The point lights are the WHY of
  deferred:** per-pixel cost is O(lights), independent of scene geometry.
  Adding more lights doesn't make the cube fragments any more expensive.
- **Per-fragment normal mapping (derivative-based TBN).** No vertex
  tangents required. The G-buffer fragment computes a tangent basis on the
  fly via ``dFdx``/``dFdy`` of world-position and procedural-UV, then
  rotates a tangent-space bump-derived normal into world space. The result
  feeds the G-buffer ``g_normal`` attachment and propagates through SSAO +
  lighting -- the floor's brick-mortar grooves and the cube's brushed
  micro-grain emerge from one procedural bump field per material.
- **6-mode debug toggle.** A push constant on the lighting fragment flips
  between the composed lit result (mode 0) and the raw G-buffer
  visualisations (1 = albedo, 2 = normal, 3 = world-pos, 4 = SSAO, 5 =
  lighting-only -- accumulated light contribution without the albedo
  multiply). The window viewer's M-key cycles through 7 toggle states
  (AUTO + 6 forced); the recording auto-rotates every 2.5 seconds.
- **Curvature-based AO (a noted simplification).** True screen-space AO
  needs to sample neighbouring fragments, which input attachments cannot
  do -- they only see the current fragment. The SSAO subpass uses a
  ``|n · view|`` curvature darkening as a stand-in. The pedagogical clarity
  is the multi-subpass + input-attachment rail; a production SSAO would
  attach the G-buffer as sampled images alongside, sampling a hemisphere
  of offset depth samples around the world position.

Every line of every shader is daslang, lowered to SPIR-V at compile time.

.. video:: vulkan_deferred.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into
an APNG and ffmpeg-muxed with a daStrudel music bed.
The 16-row mode strip at the top cycles every 2.5 s -- watch the scene
decompose itself into albedo, normal, world-pos, SSAO, and lighting-only,
then back to the lit composition. The ``[test]`` checks the structural
deferred signal at a fixed frame -- the CI regression gate. To watch the
same scene live on your own GPU with M-key control, run the windowed
viewer (see `See it live`_ below).

The shaders
-----------

Seven shader entry points across four pairs: ``shadow_vs`` (depth-only),
``gbuffer_vs`` + ``gbuffer_fs`` (MRT writes), ``ssao_vs`` + ``ssao_fs``
(fullscreen tri + curvature AO), ``lighting_vs`` + ``lighting_fs``
(fullscreen tri + subpass-input compose). All share one ``xform`` UBO
(view/proj/light_vp) for the geometry passes and one ``scene`` UBO
(camera_pos + light_dir + light_vp + three packed point lights) for the
post-geometry passes.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/10_deferred/deferred_tut_shaders.das
   :language: das
   :start-at: module deferred_tut_shaders

The render (headless)
---------------------

The host builds FOUR pipelines (shadow / gbuffer / ssao / lighting), TWO
render passes (single-subpass shadow / three-subpass deferred), ONE
framebuffer per render pass, six descriptor sets, and an internal helper
that draws the cube + floor with per-object push constants. The per-frame
``record_deferred_render`` records the shadow pass, then the multi-subpass
deferred pass with ``cmd_next_subpass`` advancing through the three
subpasses.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/10_deferred/deferred_tut.das
   :language: das
   :start-at: def public record_deferred_render
   :end-before: //! Draw cube + floor

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). It
renders frames in LIT, ALBEDO, and SSAO modes and checks the structural
properties: indicator-strip colour per mode, sky background at top, lit
cube area, orange-brick pixels on the floor in BOTH lit and albedo modes
(proving the G-buffer wrote albedo, not just the final colour).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/10_deferred/test_deferred.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_deferred.das`` opens a GLFW window with a Vulkan swapchain
and runs the scene per frame with ``time`` derived from wall-clock. The
window title shows the current toggle state (e.g. ``M = AUTO | active:
lit``); pressing **M** cycles through 7 states: ``AUTO`` (rotates every
2.5 s), ``FORCE LIT``, ``FORCE ALBEDO``, ``FORCE NORMAL``, ``FORCE
WORLD-POS``, ``FORCE SSAO``, ``FORCE LIGHTING-ONLY``. The 16-row
indicator strip at the top of the frame is the same one the recording
uses.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/10_deferred/window/show_deferred.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/10_deferred

   # watch it live in a window with the M-key cycle (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/10_deferred/window/show_deferred.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/10_deferred/recording/record_deferred.das

Next
----

:doc:`11_hdr` switches the offscreen target to **16-bit float HDR**, adds
a **Karis-style five-level bloom pyramid** (downsample + tent upsample
with additive blend), and tone-maps the result back to LDR with the
**ACES** fitted approximation. Same "render scene → post-process chain →
present" structure as deferred, but with eleven render-pass instances
chained through three render-pass *objects* and dynamic viewport so the
same pipeline runs at every mip resolution.
