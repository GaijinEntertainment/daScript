13 - Mesh Shaders: GPU Cluster Culling
======================================

Tutorials 1-12 fed the GPU geometry from vertex + index buffers. This one
removes them entirely. ``VK_EXT_mesh_shader`` replaces the whole vertex-input
stage with a two-stage, compute-like pipeline that *amplifies* geometry on the
GPU: a **task** shader decides what to draw, and a **mesh** shader generates the
vertices and primitives for it -- no vertex buffer, no index buffer, no
draw-call geometry at all.

The headline rails:

- **No geometry input.** ``cmd_draw_mesh_tasks_e_x_t(1, 1, 1)`` dispatches a
  single task workgroup; everything downstream is GPU-generated. There is no
  vertex shader, no ``VkPipelineVertexInputState``, and no bound vertex or
  index buffer.
- **Task shader = cluster cull.** One workgroup decides which of
  ``MAX_MESHLETS = 8`` "meshlets" survive and writes the survivor *count* +
  indices into a ``@task_payload`` struct. ``EmitMeshTasksEXT(count, 1, 1)``
  then dispatches exactly ``count`` mesh workgroups and carries the payload to
  them. The cull here is a deterministic stand-in -- it drops meshlets 3 and 4
  to carve a visible gap in the grid -- so the pixel-oracle can prove the task
  stage actually removed geometry on the GPU.
- **Mesh shader = geometry generation.** One workgroup per surviving meshlet
  reads its meshlet index from the payload (keyed by ``gl_WorkGroupID``, a
  workgroup-grid builtin valid in mesh shaders), calls ``SetMeshOutputsEXT(4,
  2)`` to declare four vertices + two triangles, and emits one colored quad at
  that meshlet's grid cell through ``gl_MeshVerticesEXT``.
- **Payload handoff.** The ``MeshletPayload`` struct -- a ``count`` plus a
  fixed ``survivors`` array -- is the task-to-mesh contract, declared
  ``@task_payload`` in daslang (the shared task/mesh payload storage).

Every line of every shader is daslang, lowered to SPIR-V at compile time by
dasSpirv (no glslang).

.. image:: /_static/vulkan/mesh.png
   :alt: A 2x4 meshlet grid with cells 3 and 4 culled by the task shader.

The figure above is the offscreen render: a 2x4 grid where meshlet ``i`` sits
at cell ``(i % 4, i / 4)``. Meshlets 3 (top-right) and 4 (bottom-left) are
missing -- the task shader culled them, and the gap is the GPU-side proof. The
``[test]`` pixel-oracle keys on exactly that: cells 3 and 4 must read the
background colour, every surviving cell must read its meshlet colour. To see it
live on your own GPU, run the windowed viewer (see `See it live`_ below).

.. note::

   Mesh shaders require ``VK_EXT_mesh_shader``. The tutorial soft-skips on
   devices without it (lavapipe in CI lacks the extension, so the test passes
   as skipped) -- the figure here is from a mesh-shader-capable GPU.

The shaders
-----------

No vertex shader -- just task, mesh, and fragment. The task stage culls; the
mesh stage amplifies one quad per survivor; the fragment stage outputs the
interpolated meshlet colour.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/13_mesh/mesh_tut_shaders.das
   :language: das
   :start-at: module mesh_tut_shaders

The render (headless)
---------------------

``render_mesh`` builds the offscreen colour target and the mesh-shader
pipeline, records a single ``cmd_draw_mesh_tasks_e_x_t``, and reads the result
back through an sRGB intermediate so the readback bytes are gamma-encoded
(matching the windowed view).

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/13_mesh/mesh_tut.das
   :language: das
   :start-at: def public render_mesh
   :end-before: //! Sample the RGB

Self-verifying
--------------

The pixel-oracle (the CI regression gate) renders one frame and asserts the
cull happened: cells 3 and 4 show the background colour, while the other six
show their distinct meshlet colours. On a device without ``VK_EXT_mesh_shader``
it skips cleanly.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/13_mesh/test_mesh.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_mesh.das`` opens a GLFW window with a Vulkan swapchain and runs
the same single ``cmd_draw_mesh_tasks_e_x_t`` per frame. The grid is static --
the visual story is the GPU-generated geometry and the culled gap, not motion.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/13_mesh/window/show_mesh.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (skips cleanly without VK_EXT_mesh_shader)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/13_mesh

   # watch it live in a window (needs the glfw module + a mesh-shader GPU)
   daslang.exe modules/dasVulkan/tutorials/13_mesh/window/show_mesh.das
