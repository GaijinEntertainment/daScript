08 - Shadow Mapping: Two Passes, One Depth Image
================================================

:doc:`04_cube` introduced the depth attachment as a back-face hider;
:doc:`07_particles` introduced two pipelines sharing one ``VkBuffer`` with a
single barrier between them. This tutorial composes both: it runs **two**
render passes per frame and uses **one image in two roles** — depth
attachment in pass 1, sampled depth texture in pass 2. Concretely:
**Pass 1** renders the scene's depth from the light's point of view into an
offscreen ``D32_SFLOAT`` image; **pass 2** renders the scene from the camera,
and at every fragment projects the world position into light space to ask the
shadow map *"is anything closer to the light here than I am?"* If yes, the
fragment is in shadow. The headline rails:

- **``sampler2DShadow`` + ``textureCompare(s, uv, ref) : float``** -- dasSpirv
  emits ``OpTypeImage`` with the ``Depth=1`` flag (depth-comparison image) and
  lowers the call to ``OpImageSampleDrefImplicitLod``. With a
  ``compareEnable=true`` + ``LINEAR`` filter sampler the result is the
  hardware-PCF average over the 2x2 footprint, returned in ``[0, 1]`` (1 =
  fully lit, 0 = fully shadowed). ``create_sampler_shadow`` is the host-side
  preset (``compareOp=LESS``, white border so off-shadow-map fragments default
  to lit).
- **Depth-only render pass + one image, two roles** -- the shadow map is
  created ``USAGE_DEPTH_STENCIL_ATTACHMENT | USAGE_SAMPLED``;
  ``create_render_pass_depth_only`` makes a pass with no color attachments and
  ``finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL``. The same image is the
  depth attachment in pass 1 and the ``sampler2DShadow`` descriptor in pass 2,
  with no explicit barrier between the passes.
- **Vertex-only graphics pipeline for the shadow pass** -- ``stageCount = 1``
  (no fragment shader), ``cullMode = back`` flips for casters, and
  ``colorBlendStateCreateInfo.attachmentCount = 0`` because the pass has no
  color attachments. Valid Vulkan; the rasterizer drives depth from
  fixed-function.
- **Slope-scaled depth bias + N.L-scaled fragment bias** -- the pipeline
  enables ``depthBiasEnable`` with ``slopeFactor = 3.0``. The fragment shader
  adds a per-pixel ``(1 - N.L)``-scaled bias on top of that and gates on
  ``N.L < 0.05`` to avoid the bias artifacts at grazing angles. Without
  these the cube self-shadows ("acne") on every face nearly parallel to the
  sun.
- **5x5 PCF soft shadows** -- single ``textureCompare`` is binary; averaging
  25 taps over a small radius gives a soft penumbra. Each tap is already
  hardware-PCF'd, so the effective sample count is ~100. Silky shadow edges
  with minimal cost.
- **Procedural floor + tri-planar brushed cube** -- the floor branches by
  ``world_pos.y < 0.5`` and gets a procedural checker-on-tile pattern, a
  fresnel-like grazing-angle sheen, and a warm bounce tint near the cube's
  footprint. The cube gets a tri-planar brushed-metal micro-texture
  (projection blended by ``abs(n)``) plus Blinn-Phong specular. No textures
  uploaded; pure shader math.
- **Hemisphere ambient** -- ``lerp(ground_color, sky_color, n.y * 0.5 + 0.5)``.
  Surfaces facing up pick up warm sky, surfaces facing down pick up the cool
  ground bounce. Reads as if the scene is actually lit by an environment,
  not a single bulb.

Every line of every shader is daslang, lowered to SPIR-V at compile time.

.. video:: vulkan_shadow.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel music bed. The
camera orbits while the sun rotates faster, so the cube's shadow sweeps over
the floor and the cube's lit-face glint rotates around the geometry. The
``[test]`` checks shadow contrast on the floor at a fixed time -- the CI
regression gate. See ``skills/internal/vulkan_recording.md``. To watch the scene live on your
own GPU, run the windowed viewer (see `See it live`_ below).

The shaders
-----------

Two vertex shaders share one push-constant model matrix + the shared UBO
(camera view/proj + light view-projection + light direction + camera position).
The shadow vertex shader has no fragment shader -- the pipeline is built with
``stageCount = 1`` against the depth-only render pass.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/08_shadow/shadow_tut_shaders.das
   :language: das
   :start-at: module shadow_tut_shaders

The render (headless)
---------------------

The host builds **two render passes**, **two pipelines** (shadow + main), the
shadow map (D32_SFLOAT, attachment + sampled), one shared descriptor set
(UBO + shadow sampler), and the geometry buffers for the cube + floor.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/08_shadow/shadow_tut.das
   :language: das
   :start-at: def public build_shadow_resources
   :end-before: def public build_shadow_context

``record_shadow_render_pass`` is the per-frame work: pass 1 records cube +
floor depth into the shadow map; pass 2 records cube + floor color, with the
fragment reading the shadow map.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/08_shadow/shadow_tut.das
   :language: das
   :start-at: def public record_shadow_render_pass

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). It
renders one frame at fixed ``time = 0`` + ``camera_t = 0.5`` and checks the
floor has a clear shadowed pixel and a clear lit pixel -- shadow contrast on
the floor is the load-bearing visual element.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/08_shadow/test_shadow.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_shadow.das`` opens a GLFW window with a Vulkan swapchain and
runs both render passes every frame with ``time`` derived from wall-clock.
It owns its own instance (with surface extensions) + device (with
``VK_KHR_swapchain``), then calls ``build_shadow_resources`` to share the
two-pass setup with the headless oracle. Each frame it runs
``update_shadow_uniforms`` + ``record_shadow_render_pass`` into the present
command buffer, then blits the color attachment onto the swapchain image.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/08_shadow/window/show_shadow.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/08_shadow

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/08_shadow/window/show_shadow.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/08_shadow/recording/record_shadow.das

Next
----

:doc:`09_msaa` drops ``VkRenderPass`` and ``VkFramebuffer`` entirely in
favour of Vulkan 1.3 **dynamic rendering** (``cmd_begin_rendering`` +
``VkPipelineRenderingCreateInfo``), and turns on **4× MSAA** with an
auto-resolve attachment so the cube's silhouette stops being jaggy. A
runtime indicator strip + AUTO-toggle lets the recording show 4× MSAA
and 1× rasterization side-by-side in one clip.
