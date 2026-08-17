07 - Particles: A Compute-Driven Swarm
=======================================

The first six tutorials drew geometry the host fed in: vertices and indices uploaded
once, animated via uniforms and push constants. This one **hands the vertex stream
itself to a compute shader**: one ``VkBuffer`` (created with
``USAGE_STORAGE_BUFFER | USAGE_VERTEX_BUFFER``) holds the particle array, the
compute pipeline rewrites it every frame via 1/r² gravity, then a
**single pipeline barrier** hands the same buffer to the graphics pipeline as a
**per-vertex stream**. The headline rails:

- **``@ssbo``** -- a SPIR-V StorageBuffer-class block holding an ``array<Particle>``.
  The compute shader reads + writes through ``particles[i].field`` chained access
  chains; dasSpirv lowers them to a pair of ``OpAccessChain`` operations
  (struct-element then field-of-element);
- **``USAGE_STORAGE_BUFFER | USAGE_VERTEX_BUFFER`` on one ``VkBuffer``** -- the
  *same bytes* are bound to compute as an SSBO (`vkCmdBindDescriptorSets`) and
  to graphics as a vertex buffer (``vkCmdBindVertexBuffers``). The Particle
  struct's std430 layout (pos@0, life@12, vel@16, stride 32) is matched by the
  vertex attribute offsets;
- **A per-frame compute → graphics barrier** -- ``vkCmdPipelineBarrier`` with
  ``srcStageMask=COMPUTE_SHADER``, ``dstStageMask=VERTEX_INPUT``,
  ``srcAccessMask=SHADER_WRITE``, ``dstAccessMask=VERTEX_ATTRIBUTE_READ``,
  scoped to the particle buffer. The canonical compute → graphics handoff;
  without it the graphics pipeline can race the compute and read stale values;
- **``POINT_LIST`` topology + ``gl_PointSize``** -- one screen-space splat per
  particle, ``PARTICLE_PX`` pixels wide. The vertex shader writes
  ``gl_PointSize`` (BuiltIn ``PointSize`` Output) and the host enables the
  ``largePoints`` physical-device feature so sizes > 1 take effect; the
  graphics pipeline is built inline (v3d hard-codes ``TRIANGLE_LIST``) and
  uses ``VkPrimitiveTopology.POINT_LIST`` with no cull. No billboard math,
  no clip-space orientation pass against the projection-matrix Y-flip.
- **Pretty point sprites via ``gl_PointCoord``** -- the fragment shader reads
  ``gl_PointCoord`` (BuiltIn ``PointCoord`` Input), the UV inside the
  rasterised point primitive (``[0, 1]^2`` with ``(0.5, 0.5)`` at the centre).
  Distance from centre drives a circular ``discard()`` mask + a cubic falloff
  for the core/halo glow -- no sprite texture upload needed.
- **Additive blending + depth-test-without-write** -- the pipeline runs
  ``srcColor + dstColor`` with both factors set to ``ONE`` so overlapping
  splats accumulate into bright hot spots, and depth-write is disabled so
  particle-to-particle ordering doesn't punch holes. The fragment outputs
  premultiplied colour with ``alpha = intensity`` (consistent across colour
  + alpha channels).

Every line of the shader is daslang, lowered to SPIR-V at compile time by dasSpirv.

.. video:: vulkan_particles.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel music bed. The
1024 particles swirl around the central attractor; the colour is mapped to
instantaneous speed (cool teal at the slow end, hot magenta at the fast end).
See ``skills/internal/vulkan_recording.md``. To watch the swarm live on your own GPU, run the
windowed viewer (see `See it live`_ below).

The shaders
-----------

The compute shader integrates the gravity-toward-origin force across all
``N_PARTICLES`` particles per dispatch (one invocation per particle,
``local_size_x = 64``). The vertex shader projects each particle's position to
clip space; the fragment shader maps speed to the cool-teal → hot-magenta
palette. ``[vulkan_*_shader]`` synthesises ``particles_vs_bind_uniform`` from
the graphics shader's ``@uniform`` and ``particles_compute_push_constants``
from the compute shader's ``@push_constant``.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/07_particles/particles_tut_shaders.das
   :language: das
   :start-at: module particles_tut_shaders

The render (headless)
---------------------

The host builds **two pipelines** against one shared ``VkBuffer``. The compute
pipeline binds the buffer as an SSBO (`STORAGE_BUFFER` descriptor); the
graphics pipeline binds the *same buffer* as a per-vertex stream with attribute
offsets matching the std430-laid-out ``Particle`` struct. The compute pipeline
is built with the boost helper ``create_compute_pipeline``; the graphics
pipeline goes through a small local ``build_points_pipeline`` because v3d's
``TRIANGLE_LIST`` topology isn't what we want here.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/07_particles/particles_tut.das
   :language: das
   :start-at: def public update_particles_uniforms
   :end-before: //! Per-frame work

``record_particles_render_pass`` is the per-frame work: a compute dispatch, a
``vkCmdPipelineBarrier`` against the shared buffer (the **handoff**), and the
``POINT_LIST`` draw inside the render pass.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/07_particles/particles_tut.das
   :language: das
   :start-at: def public record_particles_render_pass
   :end-before: //! Per-frame work

Self-verifying
--------------

The test is the CI regression gate (lavapipe in CI, real GPU locally). After
one compute step it walks the whole frame counting non-background pixels,
asserting that most of the 1024 particles are visible, and that the speed
palette spans both the cool-teal and hot-magenta ends.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/07_particles/test_particles.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_particles.das`` opens a GLFW window with a Vulkan swapchain and
runs the integrator + draw every frame with ``dt`` derived from wall-clock. It
owns its own instance (with surface extensions) + device (with
``VK_KHR_swapchain``), then calls ``build_particles_resources`` to share the
offscreen render pass + both pipelines + the shared particle buffer with the
headless oracle. Each frame it runs ``update_particles_uniforms`` +
``record_particles_render_pass`` into the present command buffer, then blits
the colour attachment onto the swapchain image.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/07_particles/window/show_particles.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/07_particles

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/07_particles/window/show_particles.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/07_particles/recording/record_particles.das

Next
----

:doc:`08_shadow` runs **two** render passes per frame: a depth-only pass
from the light's POV writes a shadow map, then the main pass reads that
same image as a ``sampler2DShadow``. Same "one image, two roles" discipline
as this tutorial's compute SSBO + vertex stream, lifted to a depth texture
across passes.
