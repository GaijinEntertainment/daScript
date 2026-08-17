12 - GPU-driven: Hi-Z occlusion culling + indirect-count + bindless
===================================================================

Tutorials 1-11 all share one assumption: the **CPU** decides what to draw
and how many. This one hands that decision to the **GPU**. A compute
shader tests every instance against a hierarchical-Z depth pyramid,
compacts the survivors into an indirect-draw buffer plus a GPU-written
count, and the draw reads that count -- the CPU never learns how many
objects survive. Each survivor then selects its material from a
**bindless** ``sampler2D[]`` array via ``nonuniformEXT``, keyed by
``gl_DrawID``. This is the production GPU-driven loop in miniature.

Occlusion culling is *invisible by construction*: from the camera, a
culled object was hidden anyway, so its absence looks identical to
"drew everything." A single view can't show that culling happened. So
the frame renders the scene **twice, side by side**:

- **Left -- camera view.** A moving wall sits between the camera and a
  field of cubes. We draw **only the survivors** with
  ``cmd_draw_indexed_indirect_count``. It looks correct, because the
  culled cubes were behind the wall. *That is the whole value prop: a
  perf win that is visually transparent.*
- **Right -- god view.** An overhead/offset camera that sees the wall
  from the side plus the cubes behind it. It draws **all N** cubes and
  tints each by what the GPU decided: full material if visible, a
  translucent **red** ghost if occlusion-culled, a translucent **blue**
  ghost if frustum-culled. This reveals what the GPU skipped.

The wall sweeps each frame, so the culled set sweeps in the god view
while the camera view stays correct.

The headline rails:

- **Hierarchical-Z (HZB) depth pyramid (R32f storage image).** An
  occluder depth pre-pass renders the wall into a depth image; a compute
  shader copies that into HZB mip 0, then **max-downsamples** the
  pyramid (each level is the farthest of the four texels below it). 8-bit
  ``Rgba8`` is unusable for occlusion depth, so the storage image
  declares ``@format = "r32f"`` -- a single-channel 32-bit float target.
- **Conservative occlusion test.** For each instance the cull shader
  projects the eight world-AABB corners, derives a screen-space AABB +
  nearest depth, picks the HZB mip where the AABB spans roughly 2x2
  texels, and samples the farthest occluder over that footprint. The
  instance is culled **iff its nearest point is farther than the
  farthest occluder in its footprint** -- it is then fully hidden. The
  max-downsample makes this conservative: a cube whose footprint straddles
  the wall edge is kept (its boundary texel reads "background").
- **Atomic stream compaction.** A survivor does
  ``k = atomicAdd(drawCount, 1)`` and writes ``drawCmds[k]`` (a
  ``VkDrawIndexedIndirectCommand``) + ``drawInstance[k]`` (its original
  instance id). The draw count is a GPU-written buffer.
- **Indirect-count draw.** ``cmd_draw_indexed_indirect_count`` reads the
  command buffer and the count buffer; the GPU issues exactly
  ``drawCount`` draws with no CPU round-trip. The main vertex shader maps
  ``gl_DrawID`` back to its instance via ``drawInstance[gl_DrawID]``.
- **Bindless materials.** Each survivor's fragment shader indexes a
  ``sampler2D[N]`` descriptor array with
  ``samplers[nonuniformEXT(matIdx)]`` -- one descriptor binding, N
  textures, no per-material descriptor set or pipeline switch.
- **A five-pass barrier chain.** occluder depth pre-pass → HZB build
  (one dispatch per mip, compute→compute barriers) → cull
  (compute→indirect + compute→vertex barriers) → main indirect-count
  draw → god all-N instanced draw. The barrier chain is the real
  correctness risk; it is verified by the oracle reading back the GPU's
  decision, not just by a green fence.

Every line of every shader is daslang, lowered to SPIR-V at compile
time. Authoring the cull shader is what surfaced (and fixed, in dasSpirv)
the two emitter prerequisites this tutorial needs: the ``@format``
storage-image qualifier and member access on a local struct value
(``let inst = instances[i]; inst.pos_scale``).

.. video:: vulkan_gpu_driven.mp4

The clip above is the headless recording: the wall sweeps across the
field; in the left/camera panel the survivors are drawn via the
indirect-count path and the scene always looks complete, while the
right/god panel reveals the red occlusion ghosts (and blue frustum
ghosts at the edges) sweeping with the wall. To see it live on your own
GPU, run the windowed viewer (see `See it live`_ below).

The shaders
-----------

Ten shaders: the HZB build pair (``hzb_mip0`` copies the depth buffer,
``hzb_down`` max-downsamples), the cull compute shader (the heart of the
tutorial), the main camera vertex/fragment pair (survivors via
``gl_DrawID`` + bindless), the god vertex/fragment pair (all N, tinted by
cull reason), and the occluder wall's vertex shader + its two fragment
shaders (opaque for the camera view, translucent for the god view). The
shared ``Scene`` UBO carries both cameras' view-projection matrices; the
``CubeInstance`` SSBO is the static field.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/12_gpu_driven/gpu_driven_tut_shaders.das
   :language: das
   :start-at: module gpu_driven_tut_shaders

The render (headless)
---------------------

The host builds the R32f HZB image with per-mip storage views + one
full-chain sampled view, the occluder depth image, the wide side-by-side
target, the cube geometry + six bindless material textures, the SSBOs
(instances, draw commands, draw count, draw-instance map, cull reasons),
and the eight pipelines (three compute, five graphics). ``record_frame``
records the five-pass chain into one command buffer, with the explicit
barriers between every stage.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/12_gpu_driven/gpu_driven_tut.das
   :language: das
   :start-at: def public record_frame
   :end-before: //! bind a single descriptor set for a compute pipeline

Self-verifying
--------------

The pixel oracle is the CI regression gate (lavapipe in CI, real GPU
locally). With the wall pinned at the centre the scene is deterministic:
the central columns of the field sit behind the wall, the outer ring is
off-screen, the rest is clear. The test reads the GPU's per-instance
``cull_reason`` straight from the SSBO and asserts the decision three
ways -- one occlusion-culled instance, one visible, one frustum-culled --
then asserts the GPU-written survivor count equals the visible count, and
that the *rendering* reflects the decision: the occluded instance shows
the occluder (not its cube) in the camera view and a red ghost in the
god view, while the visible instance shows its material.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/12_gpu_driven/test_gpu_driven.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_gpu_driven.das`` opens a GLFW window with a Vulkan
swapchain and runs the full five-pass GPU-driven frame per frame, blitting
the wide side-by-side target onto the swapchain image. The wall sweeps
with wall-clock time, so the red ghosts in the god panel sweep while the
camera panel stays correct.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/12_gpu_driven/window/show_gpu_driven.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (lavapipe in CI, real GPU locally)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/12_gpu_driven

   # watch it live in a window (needs the glfw module + a display)
   daslang.exe modules/dasVulkan/tutorials/12_gpu_driven/window/show_gpu_driven.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/12_gpu_driven/recording/record_gpu_driven.das

Going further
-------------

This is **single-pass** Hi-Z: a dedicated occluder pass builds the HZB
this frame, then the cull tests against it. Production engines use
**two-pass temporal reprojection** instead -- draw last frame's visible
set, build the HZB from *that*, re-test all instances, then draw the
newly-disoccluded ones -- which removes the dedicated occluder pass and
handles disocclusion as the camera moves. The depth convention here is
standard (0 = near, 1 = far) with the HZB storing the max; a reverse-Z
buffer storing the min gives markedly better precision and is the
production default.

:doc:`13_mesh` keeps the GPU in charge of the draw but goes further: it
drops the vertex + index buffers entirely for ``VK_EXT_mesh_shader``,
where a task shader culls clusters and a mesh shader amplifies the
surviving geometry on the GPU.
