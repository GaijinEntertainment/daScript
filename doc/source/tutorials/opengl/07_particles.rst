07 - The Particle Swarm
=======================

The first **GL-idiom-substitution** rung. The dasVulkan particle tutorial runs its
simulation in a *compute shader*: a storage buffer of particles is rewritten every
frame (a gravity pull toward the origin, explicit-Euler integrated), then the same
buffer is drawn as point sprites. WebGL2 has no compute shaders and no storage
buffers -- so this rung shows the canonical GL substitute: **transform feedback**. A
vertex shader reads each particle's state as vertex attributes, integrates it, and
its outputs are captured straight back into a buffer instead of being rasterised. Two
buffers ping-pong, and a second program draws the freshly-written buffer as glowing
point sprites.

.. literalinclude:: ../../../../tutorials/opengl/07_particles/07_particles.das
   :language: das
   :start-at: shared state attributes

A vertex shader as a compute step
---------------------------------

``sim_vs`` reads ``a_pos`` / ``a_vel`` / ``a_life`` (the particle state, as vertex
attributes) and writes ``tf_pos`` / ``tf_vel`` / ``tf_life``. Three rails turn that
draw into a compute pass:

* **Transform-feedback varyings.** Before the sim program is *linked*,
  ``glTransformFeedbackVaryings(prog, 3, ["tf_pos","tf_vel","tf_life"],
  GL_INTERLEAVED_ATTRIBS)`` registers those outputs for capture. The low-level
  ``create_shader`` / ``create_shader_program(vs, fs)`` / ``link_shader`` API exists
  precisely so this call can be inserted between attach and link -- the one-shot
  ``create_shader_program(vsrc, fsrc)`` helper links immediately and can't.
* **Rasterizer discard.** ``glEnable(GL_RASTERIZER_DISCARD)`` around the sim draw
  means the vertices are processed and captured but nothing is drawn -- a pure compute
  step.
* **Ping-pong.** A buffer can't be read and written in the same draw, so two state
  buffers alternate roles: frame N reads A and writes B, frame N+1 reads B and writes
  A.

One ``[vertex_buffer] struct Particle`` (pos, vel, life) defines the state layout --
attribute offsets 0 / 12 / 24, stride 28. The transform-feedback capture order matches
that interleave exactly, so the same VAO reads the captured bytes back as the next
frame's simulation input *and* as the render input.

A WebGL2 transform-feedback rule
--------------------------------

WebGL2 forbids a buffer bound to ``GL_TRANSFORM_FEEDBACK_BUFFER`` from also being bound
to ``GL_ARRAY_BUFFER``. Buffer setup leaves the generic ``GL_ARRAY_BUFFER`` binding
pointing at a state buffer, so the sim pass clears it (``glBindBuffer(GL_ARRAY_BUFFER,
0)``) before binding the feedback target -- otherwise the capture draw raises
``GL_INVALID_OPERATION`` and silently writes nothing.

The point sprites
-----------------

``render_vs`` projects each particle and sets ``gl_PointSize`` (the vertex-stage
builtin the rasteriser reads for ``GL_POINTS``); ``render_fs`` uses ``gl_PointCoord``
-- the ``[0,1]^2`` UV inside the rasterised point -- to carve a circular disc with a
soft cubic falloff and ``discard()`` outside it, colours by speed (cool teal to hot
magenta), and premultiplies by the glow so a ``GL_ONE``/``GL_ONE`` additive blend
gives a pure additive glow. (This rung is the first to use ``discard()`` on the GL
rail -- it surfaced an emitter hole: ``discard()`` was emitted as an undefined
``_discard()`` call in GLSL versions below 400, including the ES 3.00 / 330 the
tutorials use; dasGlsl now emits the ``void _discard() { discard; }`` bridge for every
fragment shader.)

Run it
------

Locally, in a window::

    daslang tutorials/opengl/07_particles/07_particles.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered
to WebGL2: a thousand particles seeded on a tilted ring, the swarm simulated entirely
on the GPU by transform feedback and flowing under gravity as the camera orbits.
