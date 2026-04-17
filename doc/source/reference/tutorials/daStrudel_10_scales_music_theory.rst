.. _tutorial_dastrudel_scales_music_theory:

==================================
STRUDEL-10 — Scales & Music Theory
==================================

.. index::
    single: Tutorial; Strudel; scale
    single: Tutorial; Strudel; modes
    single: Tutorial; Strudel; transposition

Writing melodies in absolute pitches (``c4``, ``e4``, ``g4``) gets old
fast — every key change means rewriting every note.  Strudel scales let
you write in *degrees* (0, 1, 2, …) against a named scale.  Change the
scale and the same degree pattern produces a new key, mode, or mood.

Part A: ``scale_pattern`` — degrees against a scale
===================================================

``scale_pattern(notation, scale_def, sound)`` is a shorthand for
``n(notation) |> scale(scale_def) |> sound(sound)``.  The same
``"0 2 4 6"`` against C major and C minor is a one-line A/B test:

.. code-block:: das

    let pat <- stack([
        scale_pattern("0 2 4 6", "C4:major") |> decay(0.2) |> gain(0.5),
        scale_pattern("0 2 4 6", "C4:minor", "sawtooth") |> sustain(0.3) |> release(0.2) |> gain(0.3) |> lpf(2000.0)
    ])

Major sounds bright; minor sounds dark — same degrees, different
intervals.

Part B: pentatonic — only "safe" notes
======================================

``"C4:major:pentatonic"`` maps degrees 0..4 onto C, D, E, G, A — a
five-note scale with no semitone clashes, so any degree pattern always
sounds consonant.  Useful when you want guaranteed-pretty arpeggios
without thinking about voice leading:

.. code-block:: das

    let pat <- scale_pattern("0 1 2 3 4", "C4:major:pentatonic") |> decay(0.1) |> release(0.2)

Part C: modes — same root, different intervals
==============================================

Modes are seven-note scales that share the white-keys interval pattern
but start on a different scale degree.  Two of the most distinctive:

- ``dorian`` — minor with a *raised* sixth.  Jazzy, folksy.
- ``mixolydian`` — major with a *flattened* seventh.  Bluesy.

.. code-block:: das

    let dorian_pat <- scale_pattern("0 1 2 3 4 5 6 7", "D4:dorian") |> decay(0.15) |> release(0.2)
    let mixo_pat   <- scale_pattern("0 1 2 3 4 5 6 7", "G4:mixolydian") |> decay(0.15) |> release(0.2)

The other supported modes are ``phrygian``, ``lydian`` and ``locrian``;
all share the seven-note interval shape but rotate the starting degree.

Part D: transposition
=====================

Two ways to transpose:

1. **Change the scale root.**  ``"C4:major" → "E4:major"`` — degrees
   stay the same; pitches shift.
2. **Add ``transpose(semitones)``.**  Applied after pitch resolution,
   shifts every note by the given number of semitones.

.. code-block:: das

    let pat <- stack([
        scale_pattern("0 2 4 2", "C4:major") |> decay(0.15) |> gain(0.5),
        scale_pattern("0 2 4 2", "C4:major") |> transpose(7.0) |> decay(0.15) |> gain(0.5)
    ])

Above, the second voice plays the same melody a perfect fifth higher.

Part E: two octaves, same scale
===============================

Stack two ``scale_pattern`` voices at different octaves to build a
bass + lead pair from one scale — switch the scale root (``C4`` vs
``C5``) to move between octaves while keeping the degree pattern:

.. code-block:: das

    let pat <- stack([
        scale_pattern("0 2 4 3", "C4:major:pentatonic", "sawtooth") |> sustain(0.3) |> release(0.1) |> lpf(500.0) |> gain(0.4),
        scale_pattern("4 2 3 0", "C5:major:pentatonic") |> decay(0.05) |> release(0.2) |> delay(0.3) |> delayfeedback(0.3) |> gain(0.5)
    ])

Same scale, two octaves, two different envelopes — instant counterpoint.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_10_scales_music_theory.das <../../../../tutorials/daStrudel/daStrudel_10_scales_music_theory.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_adsr_envelopes`

   Next tutorial: :ref:`tutorial_dastrudel_synthesis`
