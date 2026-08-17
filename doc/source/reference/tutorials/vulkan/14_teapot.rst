14 - GPU Teapot: Mesh-Shader Tessellation
=========================================

Tutorial 13 introduced mesh shaders with a flat coloured grid. This one puts
them to real work: the classic Newell **Utah teapot**, tessellated entirely on
the GPU. Its 32 bicubic Bezier patches are uploaded once as control points --
there is no vertex buffer and no index buffer -- and the mesh-shader pipeline
amplifies them into a smooth surface every frame, on a grounded stage with a
reflective floor, an environment sky, and a mesh-shader-cast shadow.

The headline rails:

- **On-GPU Bezier tessellation.** Each mesh workgroup owns one patch, reads its
  16 control points from a UBO, and evaluates the bicubic Bezier surface on a
  ``TESS x TESS`` (7x7) grid -- emitting ``(TESS+1)^2 = 64`` vertices and
  ``2*TESS^2 = 98`` triangles, with a finite-difference surface normal per
  vertex. This is the flagship mesh-shader technique: parametric geometry
  expanded on the GPU, no fixed-function tessellator and no vertex buffer.
- **Task shader = patch backface cull.** One 32-wide task workgroup tests each
  patch's facing as the teapot rotates and writes only the front-facing patch
  indices into the ``@task_payload``. ``EmitMeshTasksEXT(count, 1, 1)`` then
  dispatches one mesh workgroup per surviving patch -- the GPU-side proof that
  the task stage removed geometry (roughly half the patches at any angle).
- **One patch = one mesh workgroup.** The fragment shader carries an optional
  per-patch debug tint that colours each patch distinctly, making the
  workgroup-to-patch mapping visible; the recording auto-toggles it against the
  lit ceramic glaze.
- **Grounded stage.** The teapot doesn't float in a void: a second mesh-shader
  pass renders the teapot's depth from the light's point of view into a shadow
  map, a reflective checkerboard floor samples that shadow (and reflects an
  analytic environment), and a fullscreen environment sky fills the background
  -- so the GPU-tessellated teapot reads as a solid object on a surface.

Every line of every shader is daslang, lowered to SPIR-V at compile time by
dasSpirv (no glslang). The control points live in ``teapot_data.das``,
generated from the original Newell ``teapotCGA.bpt``.

.. video:: vulkan_teapot.mp4

The clip above is the headless recording: 30 seconds, 30 fps, captured into an
APNG and ffmpeg-muxed with a daStrudel music bed + voiceover. The teapot spins
through two full turns while the per-patch debug tint auto-toggles -- watch the
smooth glazed ceramic dissolve into the rainbow "1 patch = 1 mesh workgroup"
decomposition and back. The ``[test]`` pixel-oracle checks the grounded scene at
a fixed frame: a lit ceramic teapot in the centre, sky in the corners, floor
below, and a box-averaged shadow darker than the open floor. To see it live, run
the windowed viewer (see `See it live`_ below).

.. note::

   Mesh shaders require ``VK_EXT_mesh_shader``. The tutorial soft-skips on
   devices without it (lavapipe in CI lacks the extension, so the test passes as
   skipped) -- the recording here is from a mesh-shader-capable GPU.

The shaders
-----------

No vertex shader. The task stage culls back-facing patches; the mesh stage
evaluates the Bezier surface and emits the tessellated grid; the fragment stage
shades it (view-facing Lambert + rim + the optional per-patch tint). The same
module also carries the ground-floor and env-sky shaders.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/14_teapot/teapot_tut_shaders.das
   :language: das
   :start-at: module teapot_tut_shaders

The render (headless)
---------------------

``record_teapot_render_pass`` records two passes per frame: a depth-only
mesh-shader shadow pass (the teapot's silhouette from the light's view), then
the colour pass -- env sky, the reflective floor (sampling the shadow map), and
the tessellated teapot.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/14_teapot/teapot_tut.das
   :language: das
   :start-at: def public record_teapot_render_pass
   :end-before: def public render_teapot_frame

Self-verifying
--------------

The pixel-oracle (the CI regression gate) renders one frame and asserts the
grounded scene: a lit ceramic teapot in the centre, environment sky in the
corners, a checkerboard floor below, and a box-averaged shadow region darker
than the open floor. On a device without ``VK_EXT_mesh_shader`` it skips
cleanly.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/14_teapot/test_teapot.das
   :language: das
   :start-at: [test]

See it live
-----------

``window/show_teapot.das`` opens a GLFW window and runs the same two-pass
mesh-shader pipeline per frame with ``time`` from wall-clock: the teapot spins
in place while the camera holds, so the GPU-tessellated surface is the visual
story.

.. literalinclude:: ../../../../../modules/dasVulkan/tutorials/14_teapot/window/show_teapot.das
   :language: das
   :start-at: require glfw/glfw_boost

Running it
----------

.. code-block:: bash

   # the CI pixel-oracle gate (skips cleanly without VK_EXT_mesh_shader)
   daslang.exe dastest/dastest.das -- --test modules/dasVulkan/tutorials/14_teapot

   # watch it live in a window (needs the glfw module + a mesh-shader GPU)
   daslang.exe modules/dasVulkan/tutorials/14_teapot/window/show_teapot.das

   # regenerate the recording (needs stbimage + audio + ffmpeg locally)
   daslang.exe modules/dasVulkan/tutorials/14_teapot/recording/record_teapot.das

:doc:`15_raytracing` leaves the rasterizer behind entirely: the scene goes
into acceleration structures and a ``VK_KHR_ray_tracing_pipeline`` traces one
camera ray per pixel -- raygen, miss, and closest-hit shaders all authored in
daslang, with a real traced shadow ray per hit.
