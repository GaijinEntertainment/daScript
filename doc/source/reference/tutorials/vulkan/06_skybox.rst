06 - Skybox: A Procedural Cubemap Sky
======================================

The first five tutorials rendered geometry placed in front of the camera. This
one fills **everything behind it**: a cubemap skybox surrounds the camera at
infinity, sampled by a 3D direction vector. The headline rails:

- **VK_IMAGE_VIEW_TYPE_CUBE** -- one ``VkImage`` with ``arrayLayers = 6`` and
  ``flags.cube_compatible = true``, viewed as ``VkImageViewType.CUBE``;
- **``samplerCube`` in the shader** -- dasSpirv emits an ``OpTypeImage`` with
  ``Dim = Cube``, and ``texture(samplerCube, vec3)`` lowers to the same
  ``OpImageSampleImplicitLod`` you saw in tutorial 04, with a 3-vector
  direction instead of a 2D UV;
- **Six procedural faces, one continuous sky** -- the host evaluates one
  ``sample_sky(direction)`` function across all six face patches. The faces
  pack contiguously into a single staging buffer; one
  ``vkCmdCopyBufferToImage`` with 6 per-layer regions uploads the whole
  cubemap;
- **The depth=1.0 trick** -- ``gl_Position.z = gl_Position.w`` (rebuilt via
  the float4 constructor since dasSpirv has no write-swizzle) makes every
  skybox fragment land at NDC ``z = 1.0`` (the far plane). Combined with
  ``LESS_OR_EQUAL`` depth test + ``depthWriteEnable = false``, the skybox
  passes against a depth buffer cleared to 1.0 but never updates depth, so a
  future foreground pass in the same render pass will properly draw in front
  of it without modification;
- **Rotation-only view** -- the host strips the translation column from the
  view matrix so the cube stays glued to the camera. Camera position cancels;
  the skybox always appears at infinity.

Every line of the shader is daslang, lowered to SPIR-V at compile time by dasSpirv.

.. video:: vulkan_skybox.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into
an APNG and ffmpeg-muxed with a daStrudel music bed.
The camera completes one full orbit and the sun glow tracks across the sky as
it does. See ``skills/internal/vulkan_recording.md``. To watch it live on your own GPU, run
the windowed viewer (see `See it live`_ below).

The shaders
-----------

The vertex shader passes the cube position along as the per-fragment sampling
direction (``v_dir = a_pos``). Standard ``proj * view * vec4(pos, 1)``, then
the float4 is rebuilt with ``z = w`` to snap NDC z to 1.0 after the
perspective divide. The fragment shader normalises the interpolated direction
and feeds it to ``texture(sky_tex, dir)`` -- the cubemap face + intra-face
UVs are computed by the sampler from this 3-vector. ``[vulkan_*_shader]``
synthesises ``skybox_vs_bind_uniform`` from ``var @uniform cam : Camera``;
there is no push constant -- the skybox is a static cubemap, animation comes
from the camera moving in the host.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/06_skybox/skybox_tut_shaders.das
   :language: das
   :start-at: module skybox_tut_shaders

The render (headless)
---------------------

The host builds the 6-face cubemap from one ``sample_sky(direction)``
function: zenith blue, warm horizon, dark ground hemisphere, and a sun disc +
Mie-style glow along ``SUN_DIR``. Each face's pixel-to-direction mapping
follows the Vulkan cubemap face order (+X, -X, +Y, -Y, +Z, -Z = layers 0..5),
so the same procedural function evaluated on each face produces a continuous
sky with no visible seams.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/06_skybox/skybox_tut.das
   :language: das
   :start-at: def public update_skybox_uniforms
   :end-before: //! Per-frame work

``render_skybox_frame`` runs ``update_skybox_uniforms`` + ``record_skybox_render_pass``
inside a ``run_cmd_sync``, copies the colour attachment to the readback buffer,
and clones it out. The pipeline is built with ``depth_write_enable=false`` so
the depth=1.0 fragments don't overwrite depth -- a future combined scene can
drop foreground geometry into the same pipeline and the skybox automatically
fills the gaps.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/06_skybox/skybox_tut.das
   :language: das
   :start-at: def public render_skybox_frame
   :end-before: //! One-shot:

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). At
``camera_t = 0.75, time = 0`` the camera sits at ``(0, 0, -4)`` looking +Z,
so the sun-bearing ``(+X, +Y, +Z)`` octant is in front. The oracle asserts a
handful of structural properties: the mean luminance is well above the clear
colour, somewhere in the frame is a warm sun-tone pixel (sun glow visible),
somewhere is a blue-dominant zenith pixel (zenith band sampled), and the top
band is brighter on average than the bottom band (ground darker than sky).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/06_skybox/test_skybox.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_skybox.das`` opens a GLFW window with a Vulkan swapchain and
presents the orbiting skybox every frame. It owns its own instance (with
surface extensions) + device (with ``VK_KHR_swapchain``), then calls
``build_skybox_resources`` to share the offscreen render pass, framebuffer,
cubemap and pipeline with the headless oracle. Each frame it runs
``update_skybox_uniforms`` + ``record_skybox_render_pass`` into the present
command buffer, then blits the colour attachment onto the swapchain image.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/06_skybox/window/show_skybox.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/06_skybox

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/06_skybox/window/show_skybox.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/06_skybox/recording/record_skybox.das

Next
----

:doc:`07_particles` hands the **vertex stream itself** to a compute shader:
one ``VkBuffer`` lives as a storage-buffer SSBO for compute and as a
per-vertex stream for graphics, with a single pipeline barrier between the
passes. The first GPU-driven scene of the series.
