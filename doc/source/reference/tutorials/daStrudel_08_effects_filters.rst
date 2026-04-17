.. _tutorial_dastrudel_effects_filters:

==============================
STRUDEL-08 — Effects & Filters
==============================

.. index::
    single: Tutorial; Strudel; lpf
    single: Tutorial; Strudel; hpf
    single: Tutorial; Strudel; reverb
    single: Tutorial; Strudel; delay
    single: Tutorial; Strudel; orbit
    single: Tutorial; Strudel; phaser

Strudel splits effects into two groups, distinguished by where they live
in the signal graph:

================== =========================== ================================================
Group              Setters                     Lifetime
================== =========================== ================================================
Per-orbit bus FX   ``room``, ``delay``,        one shared instance per orbit; voices send wet
                   ``chorus``                  / dry into it
Per-voice FX       ``lpf``, ``hpf``,           baked into each individual voice; runs before
                   ``phaser``, ``tremolo``,    mixdown into the orbit
                   ``compressor``, ``shape``,
                   ``crush``, ``coarse``
================== =========================== ================================================

This split lets you give each layer of a stack its own reverb and delay
character (per-orbit), while still personalising every note's filter and
modulation (per-voice).

Part A: per-voice filters — ``lpf`` and ``hpf``
===============================================

``lpf(freq)`` cuts harmonics above ``freq`` Hz; ``hpf(freq)`` cuts below.
Compare a muffled sawtooth (``lpf 200``) against a thin one
(``hpf 2000``):

.. code-block:: das

    // Dark, all-bass:
    let pat_lpf <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(200.0)

    // Thin, all-treble:
    let pat_hpf <- atom("sawtooth") |> note(48.0) |> sustain(1.0) |> hpf(2000.0)

Each voice carries its own filter coefficients, so two stacked voices
can be filtered differently.

Part B: per-orbit reverb — ``room`` and ``roomsize``
====================================================

``room(amount)`` sets the wet send to the orbit's reverb bus;
``roomsize(N)`` controls the room dimensions.  Both live on the bus, so
all voices on the same orbit share one reverb instance:

.. code-block:: das

    let pat <- note("c4 ~ c4 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(2.0)

Part C: per-orbit delay — tempo-aware by default
================================================

``delay(amount)``, ``delaytime(seconds)`` and ``delayfeedback(0..1)``
drive the orbit's delay line.  ``delaytime`` defaults to a tempo-aware
3/16 cycle (resolved from ``delaysync`` and the current cps), so plain
``delay(0.5)`` already locks to the tempo even without a ``delaytime``
setter.  Tutorial 09 covers the resolver in detail.

.. code-block:: das

    let pat <- note("c4 ~ e4 ~ g4 ~ c5 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> delay(0.5) |> delaytime(0.5) |> delayfeedback(0.4)

Part D: per-voice phaser
========================

``phaser(rateHz)`` sweeps a notch through the spectrum at the given
rate.  Because it is per-voice, every note carries its own phase — two
stacked voices stay independent, which is **not** how shared-bus
live-coding systems behave.

.. code-block:: das

    let pat <- run(8) |> scale("D:pentatonic") |> sound("sawtooth") |> sustain(1.0) |> release(0.5) |> phaser(2.0)

Part E: orbits — give each layer its own bus FX
===============================================

Different orbits get independent bus FX instances.  Below, orbit 0 sits
in a tight small room while orbit 1 sits inside a cathedral:

.. code-block:: das

    let pat <- stack([
        note("c4 ~ e4 ~", "sine") |> attack(0.001) |> decay(0.15) |> sustain(0.0) |> release(0.01) |> room(0.8) |> roomsize(0.5) |> orbit(0),
        note("c3", "triangle") |> attack(0.5) |> decay(0.2) |> sustain(0.6) |> release(0.5) |> room(0.8) |> roomsize(8.0) |> orbit(1)
    ])

The pluck on orbit 0 sounds tight; the pad on orbit 1 swims in
reverb — neither bleeds into the other.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_08_effects_filters.das <../../../../tutorials/daStrudel/daStrudel_08_effects_filters.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_per_voice_fx`

   Next tutorial: :ref:`tutorial_dastrudel_adsr_envelopes`
