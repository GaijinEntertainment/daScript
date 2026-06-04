.. _tutorial_dastrudel_signals_modulation:

=================================
STRUDEL-09 — Signals & Modulation
=================================

.. index::
    single: Tutorial; Strudel; Signals
    single: Tutorial; Strudel; Modulation
    single: Tutorial; Strudel; sine
    single: Tutorial; Strudel; Pattern-valued setter

So far every effect parameter has been a constant — ``lpf(800)``,
``gain(0.5)``. This tutorial introduces **signals**: patterns whose Haps
carry *numbers* instead of sounds, and the **pattern-valued setters** that
let a signal drive any effect parameter over time. A filter that sweeps, a
gain that swells, a pan that drifts — all of it is one idea: feed a signal
where you used to write a constant.

A signal is just a pattern
==========================

A signal is an ordinary ``Pattern``; the difference is what its Haps carry.
Query one by hand and you get a number, not a drum hit. ``sine()`` is a
*continuous* signal — one query window yields one Hap holding the wave's
value at the start of that window:

.. code-block:: das

    let sig <- sine()
    var h <- invoke(sig, TimeSpan(start = 0.0lf, stop = 1.0lf))
    print("value = {h[0].value.note}\n")
    // output: value = 0.5

At phase 0 a strudel sine sits at its midpoint, ``0.5`` — it sweeps
``0 -> 1 -> 0`` across each cycle. ``run(n)`` is a *discrete* signal: ``n``
equal steps valued ``0, 1, ... n-1``:

.. code-block:: das

    var hr <- invoke(run(4), TimeSpan(start = 0.0lf, stop = 1.0lf))
    // four haps: 0  1  2  3   (one per quarter cycle)

The full signal zoo: continuous ``sine``, ``cosine``, ``saw``, ``isaw``,
``tri``, ``itri``, ``square``, ``perlin``, ``rand`` (all running ``0..1``);
discrete ``run(n)`` and ``irand(n)``.

range: remap into a useful span
===============================

The continuous signals all run ``0..1``. ``range(lo, hi)`` stretches that
into the span you actually want — a cutoff in Hz, a gain, a pan position.
``run()`` can drive pitch directly; ``run(8) |> add(48)`` is a chromatic
ramp from MIDI 48 (C3) upward:

.. code-block:: das

    let pat <- run(8) |> add(48.0) |> sound("sine") |> sustain(0.3)
    play(pat, 0.5lf)

Pattern-valued setters: a moving filter
=======================================

This is the headline. Every per-event setter (``lpf``, ``gain``, ``pan``,
``speed``, ...) has an overload that takes a **Pattern** instead of a
constant. Feed it a signal and the parameter tracks that signal over time:

.. code-block:: das

    let pat <- (
        atom("sawtooth") |> note(48.0) |> sustain(1.0)
        |> lpf(sine() |> range(200.0, 4000.0) |> slow(4.0lf))
    )
    play(pat, 0.5lf, 8.0lf)

``sine() |> range(200, 4000) |> slow(4)`` sweeps the cutoff from 200 Hz to
4 kHz and back over four cycles — the classic filter-sweep pad. Note the
surrounding parens: a multi-line pipe chain at statement level must be
wrapped in ``(...)``.

Modulating gain and pan
=======================

The same trick works for any setter. A slow sine on ``gain`` is a smooth
volume swell; a slow sine on ``pan`` drifts the sound across the stereo
field. Stack them and one sustained tone breathes and moves:

.. code-block:: das

    let pat <- (
        atom("sine") |> note(48.0) |> sustain(1.0)
        |> gain(sine() |> slow(2.0lf))
        |> pan(sine() |> slow(4.0lf))
    )
    play(pat, 0.5lf, 8.0lf)

The noisy signals: perlin and rand
==================================

``rand()`` is white-noise random — a fresh value every query, good for
scattering pan or velocity. ``perlin()`` is smooth/organic random — it
wanders instead of jumping, ideal for slow, living modulation:

.. code-block:: das

    // perlin drives a slow, unpredictable gain swell on a pad
    let pad <- (
        atom("sawtooth") |> note(48.0) |> sustain(1.0) |> lpf(800.0)
        |> gain(perlin() |> range(0.1, 0.8) |> slow(4.0lf))
    )
    // rand scatters each note to a random stereo position
    let mel <- note("c4 e4 g4 a4 g4 e4", "sine") |> sustain(0.5) |> pan(rand())

Where next
==========

You now know:

- A signal is a ``Pattern`` whose Haps carry numbers (in the ``note`` field)
- ``range(lo, hi)`` remaps a ``0..1`` signal into any span
- Every setter has a **pattern-valued** overload — feed it a signal to make
  the parameter move over time
- ``slow()`` stretches a signal's motion across several cycles
- ``perlin`` wanders smoothly; ``rand`` jumps

Tutorial 10 covers **ADSR envelopes** — shaping the amplitude of each note
over its lifetime, the other half of "sound that changes over time".

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_09_signals_modulation.das <../../../../tutorials/daStrudel/daStrudel_09_signals_modulation.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_effects_filters`

   Next tutorial: :ref:`tutorial_dastrudel_adsr_envelopes`

   Related: :ref:`tutorial_dastrudel_effects_filters` — the constant-valued setters these signals replace
