.. _tutorial_dasStbImage_hdr:

==================================
STBIMAGE-07 — HDR Images
==================================

.. index::
    single: Tutorial; HDR
    single: Tutorial; load_hdr
    single: Tutorial; float pixels
    single: Tutorial; dasStbImage

This tutorial covers high-dynamic-range images: float pixel buffers whose
values are not clamped to ``[0, 1]``, saved and loaded through the Radiance
``.hdr`` format.

Creating a Float Image
======================

HDR images store float pixels (``bpc = 4``), so a channel can be brighter than
full white. ``make_image(w, h, channels, 4)`` allocates the float buffer; the
float overload of ``with_pixels`` exposes it as ``array<float>#``:

.. code-block:: das

   require stbimage/stbimage_boost

   var img = make_image(32, 32, 3, 4)
   img |> with_pixels() $(var px : array<float>#) {
       for (i in range(32 * 32)) {
           px[i * 3 + 0] = 2.5        // red brighter than full white
           px[i * 3 + 1] = 1.0
           px[i * 3 + 2] = 0.5
       }
   }

Saving and Loading
==================

``save()`` selects the encoder from the file extension; ``.hdr`` writes
Radiance RGBE. ``load_hdr()`` reads it back as float and sets ``bpc = 4``.
``is_hdr()`` reports whether a file on disk is HDR:

.. code-block:: das

   let (ok, err) = img.save("out.hdr")
   let detected = is_hdr("out.hdr")        // true

   var reloaded : Image
   let (ok2, err2) = reloaded.load_hdr("out.hdr")
   // reloaded.bpc == 4, reloaded.channels == 3

Round-Trip Accuracy
===================

Radiance ``.hdr`` is RGBE: an 8-bit mantissa per channel plus one **shared**
exponent. It is therefore slightly lossy — per-channel precision is about
``max_channel / 256`` — but HDR magnitudes (values above ``1.0``) survive:

.. code-block:: das

   var maxerr = 0.0
   img |> with_pixels() $(var a : array<float>#) {
       reloaded |> with_pixels() $(var b : array<float>#) {
           for (i in range(length(a))) {
               maxerr = max(maxerr, abs(a[i] - b[i]))
           }
       }
   }
   // maxerr stays within the RGBE tolerance

Values that are exact multiples of a power of two (like the ``2.5 / 1.0 / 0.5``
above) reconstruct exactly; arbitrary values pick up a small RGBE rounding error.

.. seealso::

   Full source: :download:`tutorials/dasStbImage/07_hdr.das <../../../../tutorials/dasStbImage/07_hdr.das>`

   Previous tutorial: :ref:`tutorial_dasStbImage_truetype_fonts`
