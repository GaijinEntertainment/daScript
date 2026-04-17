.. _tutorial_dastrudel_samples:

==========================
STRUDEL-12 — Samples
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

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_12_samples.das <../../../../tutorials/daStrudel/daStrudel_12_samples.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_synthesis`

   Next tutorial: :ref:`tutorial_dastrudel_sf2_soundfont`
