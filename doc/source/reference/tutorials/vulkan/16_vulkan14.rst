16 - Vulkan 1.4: Push Descriptors, Host Image Copy, Local Read
==============================================================

Vulkan 1.4 adds no new way to draw a frame -- dynamic rendering and
synchronization2, the shape every earlier tutorial uses, are 1.3. What 1.4
promotes to core are three things that change how a frame is *put together*,
and this tutorial uses all three on one small scene: three textured cubes lit
through a two-pass G-buffer.

.. video:: vulkan_vulkan14.mp4

The headline rails:

- **Host image copy.** Each cube's texture goes from host memory straight into
  the device image -- no staging buffer, no transfer command, no queue.
  ``upload_image_from_host`` is a host-side layout transition out of
  ``UNDEFINED`` plus one ``vkCopyMemoryToImage``; the image is ready to sample
  when the call returns. The device says which layouts it copies into
  (``host_copy_layout`` asks for ``SHADER_READ_ONLY_OPTIMAL`` and takes
  ``GENERAL`` when that is refused), and the image is created with the
  ``host_transfer`` usage bit.
- **Push descriptors.** Every descriptor the frame binds -- the camera UBO,
  each cube's texture, the two input attachments -- is *pushed* into the
  command buffer with ``cmd_push_descriptor_set`` right before the draw that
  reads it. There is no descriptor pool and no set allocation anywhere; the
  set layouts come from the shaders' reflection through
  ``build_push_descriptor_set_layouts``, which is ``build_descriptor_set_layouts``
  with the ``push_descriptor`` flag.
- **Dynamic rendering local read.** The geometry pass writes albedo and the
  world normal into two attachments; the lighting pass reads both back *at the
  same fragment* through ``subpassInput`` / ``subpassLoad`` and composes the lit
  color into a third attachment -- inside **one** ``record_rendering``, with no
  render pass and no subpass. Two maps make it work, set once at pipeline
  creation and again in the command buffer before each pass's draws:
  ``vkCmdSetRenderingAttachmentLocations`` says which fragment output location
  lands in which attachment, ``vkCmdSetRenderingInputAttachmentIndices`` says
  which attachment a ``subpassInput`` index reads. A by-region memory barrier
  between the passes orders the attachment writes before the input reads, and
  the G-buffer attachments stay in the ``RENDERING_LOCAL_READ`` layout, which
  is legal for both.

A 1.4 device comes from ``create_device_vulkan14``, which turns the three
features on behind the 1.3 pair; ``vulkan14_supported`` is the probe, and the
oracle skips with a message where it fails. Lavapipe reports 1.4 from Mesa 25.0,
which the nightly lane installs; a local GPU with a current driver reports it.

See it live
-----------

``window/show_vulkan14.das`` opens a GLFW window on a 1.4 instance and device and
blits the lit frame onto the swapchain every frame, the camera orbiting and the
light swinging with wall-clock time. It needs a display, the ``glfw`` module and
a Vulkan 1.4 driver, so it lives in the ``window/`` subfolder the tutorial's CI
gate skips.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/16_vulkan14/window/show_vulkan14.das
   :language: das
   :start-at: require glfw/glfw_boost

The shaders
-----------

Four stages, all daslang. The geometry vertex stage transforms the cube and
passes the world normal and UV on; its fragment stage writes the texture's color
to location 0 and the packed normal to location 1. The lighting vertex stage is
the fullscreen triangle; its fragment stage declares the two attachments as
``subpassInput`` globals with ``@input_attachment_index`` and reads them with
``subpassLoad``. One ``@push_constant`` struct serves both passes.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/16_vulkan14/vulkan14_tut_shaders.das
   :language: das
   :start-at: // ===== shared =====

The textures and the resources
------------------------------

``create_host_copied_texture`` is the host-image-copy rail: an image with
``sampled`` + ``host_transfer`` usage, device-local memory, and one
``upload_image_from_host`` call that leaves it in the layout the device chose.
``build_vulkan14_resources`` then builds everything else -- the three
attachments and the depth buffer, the cube geometry, the camera UBO, the two
pipelines on push-descriptor set layouts. Each pipeline is built by hand because
it names three color attachments and carries its own location and input-index
maps on the create info's ``pNext`` chain (``pass_layout``).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/16_vulkan14/vulkan14_tut.das
   :language: das
   :start-at: //! One sampled texture
   :end-before: //! Self-contained one-shot context

The frame
---------

``record_vulkan14_frame`` moves the two G-buffer attachments into
``RENDERING_LOCAL_READ`` and the color target into ``COLOR_ATTACHMENT_OPTIMAL``,
then opens one rendering over all three plus depth. Inside it: the attachment
maps for the geometry pass, the three cube draws (a push-constant model matrix
and two pushed descriptors each), the by-region barrier, the maps for the
lighting pass, two pushed input attachments, and the fullscreen draw.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/16_vulkan14/vulkan14_tut.das
   :language: das
   :start-at: //! The frame's descriptors for cube
   :end-before: //! One frame at `time`

Self-verifying
--------------

The oracle renders one frame at time 0 and samples it: the sky between the cubes
is the lighting pass's gradient (not the cleared black of the third attachment),
each cube shows the hue of the texture host image copy uploaded, and a face
toward the light is brighter than one facing away -- which only the normal
attachment, read back through local read, can produce. It skips where
``vulkan14_supported`` is false.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/16_vulkan14/test_vulkan14.das
   :language: das
   :start-at: [test]

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe from Mesa 25.0 in CI, a 1.4 GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/16_vulkan14

   # watch it live in a window (needs the glfw module, a display and a 1.4 driver)
   daslang.exe modules/dasVulkan/tutorials/16_vulkan14/window/show_vulkan14.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/16_vulkan14/recording/record_vulkan14.das

Beside the tutorials
--------------------

``examples/shader_objects.das`` draws the hello triangle with **no pipeline at
all**: ``VK_EXT_shader_object`` binds a shader object per stage and takes every
piece of fixed-function state from the command buffer
(``cmd_set_shader_object_baseline`` sets the lot). It is an opt-in beside the
pipeline path -- an extension in every Vulkan version, absent on Android and
partial on MoltenVK -- and ``tests/integration/test_shader_objects.das`` checks
it draws the same triangle.
