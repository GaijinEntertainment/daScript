.. _tutorial_dastrudel_synthesis:

==========================
STRUDEL-11 — Synthesis
==========================

.. index::
    single: Tutorial; Strudel; Synthesis
    single: Tutorial; Oscillator
    single: Tutorial; FM

daStrudel ships five built-in oscillators, two noise generators, and a
two-operator FM engine — enough to produce most classic subtractive and
FM timbres without loading a single sample.  Every source is selected the
same way: ``s("name")`` picks the voice, and the usual ADSR / filter
chain shapes the result.

Part A: Oscillators
===================

``s("sine" | "triangle" | "square" | "sawtooth" | "supersaw")`` picks the
oscillator.  They share one API — the same ``attack`` / ``decay`` /
``sustain`` / ``release`` and filter words apply to all of them:

.. code-block:: das

    require strudel/strudel

    let pat <- s("<sine triangle square sawtooth>") |> note(48.0) |> attack(0.02) |> decay(0.1) |> sustain(0.7) |> release(0.3) |> lpf(2000.0)

Tonal character:

- ``sine`` — pure, no harmonics
- ``triangle`` — soft, odd harmonics, gentle roll-off
- ``square`` — hollow and woody, odd harmonics only
- ``sawtooth`` — bright and buzzy, all harmonics

Part B: Supersaw
================

``supersaw`` is seven detuned saws stacked into one voice — the classic
"trance" pad sound.  It is much wider and fatter than a single saw; a
low-pass filter tames the top end:

.. code-block:: das

    let pat <- note("c3,eb3,g3,c4", "supersaw") |> attack(0.1) |> decay(0.1) |> sustain(0.8) |> release(0.6) |> lpf(3500.0)

Commas inside ``note()`` build chords — all four notes sound simultaneously
per step.

Part C: Noise
=============

``"white"`` and ``"pink"`` are pitch-less noise generators.  Pink noise
has more energy in the low end — perceptually "warmer" than white.  Run
them through filters to shape specific colours (ocean, wind, static,
hi-hat):

.. code-block:: das

    let pat <- atom("pink") |> sustain(1.0) |> lpf(400.0) |> gain(0.4)

``atom()`` wraps a plain string as a one-step pattern — useful when you
want a continuous source with no rhythm.

Part D: FM Synthesis
====================

FM modulates a sine carrier's frequency with a second sine.  Two knobs
control the timbre:

- ``fm(index)`` — modulation depth: higher = more harmonics, brighter sound
- ``fmh(harmonicity)`` — ratio of modulator to carrier frequency

Integer harmonicity values (1, 2, 3) sound musical; non-integer values
produce inharmonic, bell- or clang-like timbres:

.. code-block:: das

    // Metallic bell — high inharmonic ratio
    let bells <- note("c5", "sine") |> fm(1.0) |> fmh(7.0) |> decay(0.3) |> release(0.8)

    // Gentle brass-like tone — integer ratio
    let brass <- note("<c3 e3 g3 c4>", "sine") |> fm(2.0) |> fmh(2.0) |> attack(0.02) |> decay(0.2) |> sustain(0.6) |> release(0.3)

The ``note("c5", "sine")`` form is shorthand for
``s("sine") |> note("c5")`` — the second argument picks the oscillator.

See :ref:`tutorial_lambdas` for how ``|>`` threads a pattern through a
chain of modifiers — each ``|>`` returns a new ``Pattern``.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_11_synthesis.das <../../../../tutorials/daStrudel/daStrudel_11_synthesis.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_scales_music_theory`

   Next tutorial: :ref:`tutorial_dastrudel_samples`
