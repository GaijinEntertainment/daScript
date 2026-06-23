.. _tutorial_dasMinfft_real_fft:

===================================
MINFFT-01 — Real FFT
===================================

.. index::
    single: Tutorial; FFT
    single: Tutorial; Spectrum
    single: Tutorial; dasMinfft

The ``minfft`` module wraps the `minfft <https://github.com/aimukhin/minfft>`_
library — a small, fast FFT/DCT kernel. It works on **power-of-two** lengths.
This tutorial covers the real FFT: turning a real signal into its frequency
spectrum and back.

Forward transform
=================

A real signal of length ``N`` transforms to ``N/2+1`` complex bins (the
spectrum of a real signal is symmetric, so the upper half is redundant and not
stored). A tone that completes exactly ``k`` cycles over the ``N`` samples lands
on bin ``k`` with raw magnitude ``N/2`` times its amplitude:

.. code-block:: das

   let signal <- [for (i in range(64));
       sin(TWO_PI * 4.0f * float(i) / 64.0f) +
       0.5f * sin(TWO_PI * 12.0f * float(i) / 64.0f)]

   var spectrum : array<float2>
   fft_real_forward(signal, spectrum)   // 64 samples -> 33 complex bins

   var mag : array<float>
   fft_calculate_magnitudes(spectrum, mag)   // peaks: 32 at bin 4, 16 at bin 12

Magnitude helpers
=================

Each complex bin ``(re, im)`` carries amplitude ``sqrt(re*re + im*im)``. Three
helpers fill a ``float`` array from the complex spectrum:

* ``fft_calculate_magnitudes`` — raw amplitude.
* ``fft_calculate_normalized_magnitudes`` — raw amplitude scaled by ``1/(N/2+1)``.
* ``fft_calculate_log_magnitudes`` — log scale, handy for spectrum displays.

Inverse transform and normalization
====================================

minfft transforms are **unnormalized**: ``fft_real_inverse(fft_real_forward(x))``
returns ``N*x``, not ``x``. Divide by ``N`` to recover the original signal:

.. code-block:: das

   var back : array<float>
   fft_real_inverse(spectrum, back)
   // back[i] / float(N) == signal[i]

This unnormalized convention runs through the whole library — the DCT pair in
the next tutorial scales by ``2N`` per axis.

.. seealso::

   Full source: :download:`tutorials/dasMinfft/01_real_fft.das <../../../../tutorials/dasMinfft/01_real_fft.das>`

   Next tutorial: :ref:`tutorial_dasMinfft_dct_basics`
