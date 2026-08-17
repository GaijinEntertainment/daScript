.. _utils_dasllama_convert:

.. index::
   single: Utils; dasllama-convert
   single: Utils; Model conversion

============================================
 dasllama-convert --- Offline Model Prep
============================================

dasllama-convert turns a GGUF model file into a prepared ``.dlim`` image
offline, so the first real load maps the image instead of converting.
The image is box- and knob-specific: the default output name carries the
identity hash and is exactly what the normal dasLLAMA load path looks
for.

Quick start
===========

::

   daslang -jit utils/dasllama-convert/main.das -- -m <model.gguf> [-o <out.dlim>] [-q q8]

- ``--flavor`` picks which image a multi-flavor config emits (planar
  CPU, metal blob, vulkan-baked).  ``-f vulkan`` bakes offline — no VRAM
  touched.
- ``--dump-config`` prints this box's ``DlimConfiguration`` as JSON;
  ``--config x.json`` bakes against a supplied one, verified fail-closed
  against the resulting identity — a knob that did not take is an error,
  never a silently different image.

Conversion is I/O bound at both ends; per-stage timings are reported so
a slow box is diagnosable.

.. seealso::

   :ref:`utils_dasllama_server` -- the inference server that loads the images
