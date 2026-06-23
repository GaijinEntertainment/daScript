.. _tutorial_dasMinfft_dct_basics:

===================================
MINFFT-02 — DCT Basics
===================================

.. index::
    single: Tutorial; DCT
    single: Tutorial; Energy Compaction
    single: Tutorial; dasMinfft

The Discrete Cosine Transform (DCT-II) concentrates a signal's energy into a
few low-frequency coefficients. Lossy codecs exploit this: keep the big low
coefficients, throw away the small high ones. This tutorial covers the 1D DCT;
the next one builds JPEG on top of it.

Plans
=====

A *plan* holds the twiddle tables for a given length. Build it once and reuse it
for every transform of that length — building allocates, so reuse matters in a
loop. The same plan drives both ``dct`` (forward, DCT-II) and ``idct``
(inverse, DCT-III). Free it with ``delete`` (it is a managed handle):

.. code-block:: das

   var plan = make_dct_plan_1d(8)
   // ... dct / idct calls ...
   unsafe { delete plan }

A plan holds internal scratch state, so it is **not thread-safe** — build one
plan per thread (or serialize access) if you transform in parallel.

Forward DCT and energy compaction
=================================

A smooth signal collapses to a dominant DC coefficient plus a few small ones:

.. code-block:: das

   let signal <- [for (i in range(8)); float(i)]   // a ramp 0..7
   var coeff : array<float>
   dct(signal, coeff, plan)
   // coefficients: 56 -25 0 -2 0 0 0 0  -> the DC term dominates

Inverse DCT and the 2N factor
=============================

Like the FFT, minfft's DCT is unnormalized: ``idct(dct(x)) == 2N*x`` for a
length-``N`` signal. Divide by ``2N`` to recover the original:

.. code-block:: das

   var back : array<float>
   idct(coeff, back, plan)
   // back[i] / float(2 * N) == signal[i]

Lossy truncation
================

Energy compaction in action — zero every coefficient past the first ``K``, then
reconstruct. Because the discarded coefficients were tiny, the error stays
small. This is the essence of lossy coding:

.. code-block:: das

   for (k in range(keep, n)) {
       trunc[k] = 0.0f
   }
   idct(trunc, back, plan)
   // keep 8/32 coefficients -> max error ~0.16

.. seealso::

   Full source: :download:`tutorials/dasMinfft/02_dct_basics.das <../../../../tutorials/dasMinfft/02_dct_basics.das>`

   Next tutorial: :ref:`tutorial_dasMinfft_dct_image_compression`
