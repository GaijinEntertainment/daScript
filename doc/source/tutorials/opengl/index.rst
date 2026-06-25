OpenGL / WebGL2 tutorials
=========================

A ladder of runnable OpenGL tutorials that mirror the
`dasVulkan <https://github.com/borisbat/dasVulkan>`_ series shader-for-shader, to
the degree the WebGL2 portable floor allows. Every shader is written in
**daslang** -- the same shared shader builtins (``shader_lingua_franca``) the
Vulkan rail uses -- and lowered to GLSL (GLSL ES 3.00 on the web) at compile time
by dasGlsl. The same daslang shader language, lowered to a third backend.

Each tutorial is **one homogeneous program**: ``init`` / ``update`` /
``shutdown`` plus a ``main`` driver. On the desktop ``main()`` runs the loop in a
GLFW window; on the web the wasm run path drives the three lifecycle functions
from the browser's animation frame (the daslang ``Context`` persists across
frames), so the very same ``.das`` runs live in the playground. That live,
click-to-run version in the browser is what the OpenGL rail offers over the
Vulkan tutorials' recorded video.

The series builds **graphics → fragment compute → 3D scene → instancing →
environment → multi-pass → modern post**. WebGL2 has no compute / SSBO / indirect
draw / mesh shaders, so the Vulkan rungs that depend on those (gpu-driven,
mesh-shader) have no WebGL2 mirror -- and drawing that boundary explicitly is part
of the harmonization story: one shader language, one ladder, three backends, with
the portable floor made visible.

.. toctree::
   :maxdepth: 1

   01_triangle.rst
