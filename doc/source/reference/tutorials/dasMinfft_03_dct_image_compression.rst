.. _tutorial_dasMinfft_dct_image_compression:

==========================================
MINFFT-03 — 2D DCT and JPEG Compression
==========================================

.. index::
    single: Tutorial; JPEG
    single: Tutorial; 2D DCT
    single: Tutorial; Image Compression
    single: Tutorial; dasMinfft

JPEG and MPEG intra frames split an image into 8x8 blocks and code each block
with a 2D DCT, quantization, and the inverse. This tutorial walks one 8x8 block
through that pipeline; for a full image-in / image-out demo with a PSNR sweep,
see ``examples/minfft/dct_jpeg.das``.

2D plans
========

``make_dct_plan_2d(rows, cols)`` builds a plan for ``rows x cols`` blocks. The
data is a row-major array of ``rows*cols`` floats. ``idct(dct(x))`` scales by
``2N`` per axis, so an 8x8 block round-trips at ``256 = (2*8)*(2*8)``:

.. code-block:: das

   var plan = make_dct_plan_2d(8, 8)
   dct(blk, coeff, plan)        // forward
   idct(coeff, back, plan)      // inverse; back[i] / 256.0f recovers blk[i]

Orthonormal scaling
===================

The standard JPEG quantization table is designed for the *orthonormal* DCT.
minfft's DCT-II is unnormalized, so each coefficient is rescaled by a per-axis
factor (``s[0] = 1/(4*sqrt2)``, else ``1/4``) before quantizing, and unscaled
afterwards. The scale cancels on a clean round-trip; it only changes how the
quant table weights each frequency.

The pipeline
============

Forward DCT, quantize against the JPEG luminance table, dequantize, inverse DCT:

.. code-block:: das

   let level = round(ortho[i] / JPEG_LUMA[i])   // quantize (nearest integer)
   deq[i] = (level * JPEG_LUMA[i]) / (s[u] * s[v])      // dequantize, back to minfft scale

Most coefficients quantize to zero, yet the block reconstructs closely:

.. code-block:: text

   -> 4 of 64 coefficients survive quantization
   max per-pixel error: 3.1

Those zeros are what an entropy (Huffman) coder stores cheaply — the only stage
this tutorial omits from baseline JPEG.

.. seealso::

   Full source: :download:`tutorials/dasMinfft/03_dct_image_compression.das <../../../../tutorials/dasMinfft/03_dct_image_compression.das>`

   Full demo: :download:`examples/minfft/dct_jpeg.das <../../../../examples/minfft/dct_jpeg.das>`
