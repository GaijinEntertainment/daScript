.. _tutorial_dastrudel_samples:

==========================
STRUDEL-13 — Samples
==========================

.. index::
    single: Tutorial; Strudel; Samples
    single: Tutorial; Sample bank
    single: Tutorial; Pitch shift

Oscillators cover a lot of ground but most modern music is built from
samples.  daStrudel loads sample banks from disk and indexes them by
name, so mini-notation patterns like ``s("bd sd hh cp")`` just work.

Part A: Sample Banks
====================

A sample bank is a directory of folders — one folder per named sound.
Every audio file inside the folder becomes an indexed variation:

::

   drums/
     bd/            ← "bd" sound
       BT0A0A7.wav  ← variation 0 (bd:0)
       BT0AADA.wav  ← variation 1 (bd:1)
       ...
     sd/
     hh/
     cp/

``strudel_load_sound`` loads one folder under a given name.  Files are
sorted alphabetically, so the ``:0``, ``:1``, ... indices are stable:

.. code-block:: das

    require strudel/strudel_player

    strudel_load_sound("{MEDIA}/drums/bd", "bd")
    strudel_load_sound("{MEDIA}/drums/sd", "sd")
    strudel_load_sound("{MEDIA}/drums/hh", "hh")
    strudel_load_sound("{MEDIA}/drums/cp", "cp")

Once loaded, the sounds are visible to every pattern through the
mini-notation:

.. code-block:: das

    let pat <- s("bd sd hh cp") |> gain(0.7)

Part B: Variations
==================

Multiple files in one sound folder are addressable as ``name:index``:

.. code-block:: das

    // Alternate two kick variations per cycle
    let pat <- s("bd:0 bd:1 bd:0 bd:1") |> gain(0.7)

Omitting the index (just ``"bd"``) always picks ``:0``.  Use variations
to keep machine-gun patterns from sounding mechanical — even a pair of
slightly different kicks feels more human than a single repeated file.

Part C: Pitch Shifting with ``speed``
=====================================

``speed(factor)`` scales the sample playback rate.  ``speed(2.0)`` is one
octave up and twice as fast; ``speed(0.5)`` is one octave down and half
as fast.  It is tape-style pitch shifting — shorter or longer than the
original:

.. code-block:: das

    let normal <- s("bd bd bd bd") |> speed(1.0) |> gain(0.6)
    let pitched <- s("bd bd bd bd") |> speed(2.0) |> gain(0.6)

``speed`` only affects sample playback; oscillator voices (``sine``,
``sawtooth``, ...) ignore it and use ``note()`` for pitch instead.

Part D: Single-file Decoding with ``load_audio_file``
=====================================================

Sometimes you just want to decode one audio file — to inspect it, to
feed it into your own DSP chain, or to build a custom sample bank.
``load_audio_file`` decodes WAV / MP3 / FLAC / OGG into a ``Sample``
struct:

.. code-block:: das

    require strudel/strudel_samples

    let sample <- load_audio_file("{MEDIA}/drums/bd/BT0A0A7.wav")
    print("channels: {sample.channels}\n")
    print("sample_rate: {sample.sample_rate} Hz\n")
    print("frames: {length(sample.data) / sample.channels}\n")

An empty ``Sample`` (``length(data) == 0``) signals a decode failure —
always check before using the data.

Part E: Playing a Slice with ``begin`` / ``end_pos``
====================================================

``begin(x)`` and ``end_pos(x)`` take normalized positions in 0..1 and
play only the ``[begin, end)`` window of a sample.  ``begin(0.3)`` skips
the first 30 % of the file; ``end_pos(0.8)`` trims the last 20 %.  (The
setter is ``end_pos`` because ``end`` is a reserved word.)

.. code-block:: das

    let pat <- s("gong") |> begin(0.3) |> end_pos(0.8) |> gain(0.6)

This skips the gong's bright attack and exposes its softer middle decay.

Part F: Granulation — ``chop`` / ``striate`` / ``slice``
========================================================

Three ways to cut a sample into ``n`` grains:

- ``chop(n)`` cuts each event in place into ``n`` back-to-back slices —
  the slices stay inside that event's own time slot.
- ``striate(n)`` spreads ``n`` slices across the whole cycle, so
  successive events reveal progressively later parts of the sample.
- ``slice(n, p)`` cuts into ``n`` slices and plays them in the order and
  rhythm of index pattern ``p`` — structure comes from ``p``, the sound
  from the source pattern.

.. code-block:: das

    let chopped  <- s("gong") |> chop(8) |> gain(0.6)
    let striated <- s("bd sd hh cp") |> striate(4) |> gain(0.4)
    let sliced   <- s("gong") |> slice(8, note("0 2 4 6 1 3 5 7")) |> gain(0.6)

Part G: Loading a Whole Directory with ``strudel_load_sample_dir``
==================================================================

``strudel_load_sample_dir(root)`` loads every audio file directly under
``root``, using each filename as the sound name.  It is the quick way to
bring in an external pack such as `tidalcycles/dirt-samples
<https://github.com/tidalcycles/dirt-samples>`_.  The built-in drums
need no loading at all, so reach for this only when you want sounds
beyond ``bd`` / ``sd`` / ``hh`` / ``cp``:

.. code-block:: das

    strudel_load_sample_dir("{MEDIA}/audio")
    let pat <- s("gong") |> gain(0.6)

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_13_samples.das <../../../../tutorials/daStrudel/daStrudel_13_samples.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_synthesis`

   Next tutorial: :ref:`tutorial_dastrudel_sf2_soundfont`
