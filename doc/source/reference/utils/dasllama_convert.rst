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
image's lane (``model.gguf.metal.dlim``, ``model.gguf.cpu-arm-i8mm.dlim``,
``model.gguf.vulkan.dlim``, a ``-mtp`` suffix when a split head rides the
load) and is exactly what the normal dasLLAMA load path looks for; the
image header carries the full bake identity, so a stale lane re-bakes in
place and the lanes coexist on one disk.

Quick start
===========

::

   daslang -jit utils/dasllama-convert/main.das -- -m <model.gguf> [-o <out.dlim>] [-q q8]
   daslang -jit utils/dasllama-convert/main.das -- -m <tts.gguf> --gguf <out.gguf> [--recipe]

- ``--flavor`` picks which image a multi-flavor config emits (planar
  CPU, metal blob, vulkan-baked).  ``-f vulkan`` bakes offline — no VRAM
  touched.
- ``--dump-config`` prints this box's ``DlimConfiguration`` as JSON;
  ``--config x.json`` bakes against a supplied one, verified fail-closed
  against the resulting identity — a knob that did not take is an error,
  never a silently different image.

- ``--gguf <out>`` on a KittenTTS or Kokoro GGUF whose weights are F32
  writes another GGUF in place of an image.  The new file stores the
  weights already quantized: as Q8_0, the form a load serves by default,
  or with ``--recipe`` in the smaller mix of Q4_K, Q6_K and Q8_0 the
  recipe names for each weight group.  A load of that file quantizes
  nothing and serves the form the file stores, whatever
  ``set_tts_q8`` asks for; :ref:`tutorial_dasLLAMA_text_to_speech`
  describes the lanes.

Conversion is I/O bound at both ends; per-stage timings are reported so
a slow box is diagnosable.

.. seealso::

   :ref:`utils_dasllama_server` -- the inference server that loads the images
