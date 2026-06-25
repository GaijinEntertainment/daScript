05 - The Cube Swarm
===================

A thousand cubes from **one draw call**. Tutorial 04 drew a single cube; this rung
fans that same 36-index cube over ``N_INSTANCES`` copies with
``glDrawElementsInstanced``, each copy reading its own world offset, colour and
animation phase from a second, *per-instance* vertex-attribute stream. It is a
faithful port of the dasVulkan instancing rung -- same Lissajous swarm, same
per-instance breathing.

The headline is the **vertex attribute divisor**. Two attribute streams share one
VAO: the per-vertex geometry (binding 0, locations 0-1) steps once per vertex as
usual, while the per-instance data (binding 1, locations 2-4) steps once per
*instance* because ``glVertexAttribDivisor(loc, 1)`` is set on it. The shader sees
all five as plain ``@in @location`` inputs -- there is no instancing keyword in the
shader at all; instancing lives entirely on the host side, in that divisor.

.. literalinclude:: ../../../../tutorials/opengl/05_instancing/05_instancing.das
   :language: das
   :start-at: per-vertex inputs

Two attribute streams, one VAO
------------------------------

The cube geometry (24 vertices, ``pos + normal``) goes into ``vbo`` and is wired by
``bind_vertex_buffer(null, type<Vertex>)`` to locations 0-1. The instance buffer is
built once at init -- ``gen_instances`` samples a 3D Lissajous curve into
``N_INSTANCES`` rows of ``(offset, colour, phase)`` -- uploaded into ``inst_vbo``,
and wired by ``bind_vertex_buffer(null, type<Instance>, 2u)``. The ``2u`` is the
macro's ``vindex`` argument: it offsets the generated attribute locations to 2-4 so
the two structs do not collide. Each ``glVertexAttribPointer`` the macro emits
records *whichever* array buffer is bound at that moment into the VAO, so the two
streams coexist; the three ``glVertexAttribDivisor(loc, 1)`` calls then mark the
instance attributes to advance once per instance instead of once per vertex.

Explicit ``@location = N`` on the shader inputs lowers to
``layout(location = N) in ...`` in GLSL, pinning each input to the exact location
the host binds -- important here, where the locations are split across two structs.

Animated on the GPU
-------------------

The instance buffer never changes after init. The swarm's motion is entirely in the
vertex shader: each cube breathes (``0.7 + 0.2 * sin``) and spins about Y, both
keyed off ``u_time`` and the cube's own ``a_phase`` so the thousand cubes are
desynchronised. Only the camera matrices and ``u_time`` move per frame -- one small
uniform update drives a thousand independently-animating cubes.

Run it
------

Locally, in a window::

    daslang tutorials/opengl/05_instancing/05_instancing.das

In the browser, it runs live in the daslang playground -- the same ``.das``, lowered
to WebGL2, a thousand cubes swarming from one instanced draw on your GPU.
