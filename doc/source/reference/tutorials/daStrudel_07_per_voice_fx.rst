.. _tutorial_dastrudel_per_voice_fx:

=======================================
STRUDEL-07 — Per-Voice FX & Combinators
=======================================

.. index::
    single: Tutorial; Strudel; jux
    single: Tutorial; Strudel; off
    single: Tutorial; Strudel; chunk
    single: Tutorial; Strudel; per-voice FX

This tutorial covers the *transforming* combinators — those that take a
``@(Pattern) => Pattern`` lambda and use it to derive a second copy of
the input — and the daslang-specific *per-voice FX* group (phaser,
tremolo, compressor, shape, crush).

Part A: ``jux`` — stereo splitting via a transform
==================================================

``jux(pat, fn)`` plays the original pattern in the left channel and
``fn(pat)`` in the right.  The classic move is to reverse the right
side, producing the trademark live-coding stereo wobble:

.. code-block:: das

    let pat <- jux(note("c4 e4 g4 c5", "sine") |> sustain(0.4), @(p) => rev(p))

Or transpose the right side up a fifth for instant harmony:

.. code-block:: das

    let pat <- jux(note("c4 e4 g4 c5", "sine") |> sustain(0.4),
                   @(p) => transpose(p, 7.0))

Any function ``Pattern → Pattern`` works as the transform — including
combinators built out of ``stack``, ``cat`` or ``fast``.

Part B: ``off`` — canon, delayed transformed copy
=================================================

``off(pat, time, fn)`` plays the original AND a copy of ``fn(pat)``
delayed by ``time`` cycles.  With ``time = 0.25`` and an octave-up
transpose you get a textbook canon at the octave:

.. code-block:: das

    let pat <- off(note("c4 e4 g4 b4", "sine") |> sustain(0.4), 0.25lf,
                   @(p) => transpose(p, 12.0))

The delayed voice obeys whatever the transform does to it — try
``@(p) => fast(p, 2.0lf)`` to layer a faster echo, or ``rev`` for a
retrograde answer.

Part C: ``chunk`` — apply a transform to one slice at a time
============================================================

``chunk(pat, n, fn)`` divides each cycle into ``n`` equal slices and
applies ``fn`` to slice 0 on cycle 0, slice 1 on cycle 1, and so on.
Use it to add evolving variation without rewriting the pattern:

.. code-block:: das

    let pat <- chunk(note("c4 e4 g4 c5", "sine") |> sustain(0.4), 4,
                     @(p) => fast(p, 2.0lf))

Each beat in turn doubles in speed for one cycle, then settles back.

Part D: per-voice FX (a daslang divergence)
===========================================

Most live-coding systems treat ``phaser``, ``tremolo``, ``compressor``,
``shape``, ``crush`` as effects on a shared bus.  In daslang these are
*per-voice* — each playing voice carries its own FX chain, applied
**before** the voice mix is summed into the orbit.  Two simultaneous
notes in a single ``stack`` can therefore use entirely different FX
parameters without any cross-talk:

.. code-block:: das

    let pat <- stack([
        note("c4", "sawtooth") |> sustain(1.0) |> phaser(0.5) |> gain(0.4),
        note("g4", "sawtooth") |> sustain(1.0) |> phaser(2.0) |> gain(0.4)
    ])

Above, the C voice carries a slow phaser sweep while the G voice
carries a fast one.  On a shared-bus system both voices would have to
share the same rate.

The per-voice setters available today: ``lpf``, ``hpf``, ``phaser`` /
``ph``, ``tremolo`` / ``trem``, ``compressor``, ``shape``, ``crush``,
``coarse``.  The shared-bus setters (``room``, ``delay``, ``chorus``)
are covered in the next tutorial.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_07_per_voice_fx.das <../../../../tutorials/daStrudel/daStrudel_07_per_voice_fx.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_stacking_combining`

   Next tutorial: :ref:`tutorial_dastrudel_effects_filters`

   Related: :ref:`tutorial_lambdas`, :ref:`tutorial_function_pointers`
