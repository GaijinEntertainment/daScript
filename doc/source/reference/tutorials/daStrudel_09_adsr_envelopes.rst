.. _tutorial_dastrudel_adsr_envelopes:

====================================
STRUDEL-09 — ADSR & Envelope Shaping
====================================

.. index::
    single: Tutorial; Strudel; ADSR
    single: Tutorial; Strudel; attack
    single: Tutorial; Strudel; decay
    single: Tutorial; Strudel; sustain
    single: Tutorial; Strudel; release
    single: Tutorial; Strudel; tempo-aware defaults

An ADSR envelope shapes a note's amplitude over time:

==========  ============================================================
Attack      time from key-down to peak amplitude
Decay       time to fall from the peak to the sustain level
Sustain     held level (0..1) while the note is on
Release     time to fade to silence after key-up
==========  ============================================================

daslang's strudel uses **tempo-aware defaults** for ADSR.  Instead of
fixed numeric defaults, each field defaults to ``-1`` — a sentinel
meaning *unset*.  At playback time a resolver fills in sensible values
based on which fields the user actually touched:

============================== ===========================================
Nothing set                    held tone — sustain = 1, tiny attack/release
Only ``decay`` set             percussion — sustain auto-zeroes
``sustain`` set explicitly     always wins
============================== ===========================================

The same idea applies to ``delaytime``: it defaults to ``-1`` and is
derived from ``delaysync`` (in cycles) and the current cps so the echo
locks to tempo automatically.

Part A: default envelope — held tone
====================================

With no ADSR setters at all, the resolver picks a held-tone envelope:
the note rings for its full scheduled duration, instead of decaying to
silence in 50ms as in older defaults.

.. code-block:: das

    let pat <- note("c4 e4 g4 c5", "sine")

This is the *unset → held* branch of the resolver.

Part B: pluck — decay only
==========================

Set ``decay`` alone and the resolver assumes a percussive envelope:
sustain drops to 0 so each note plucks then decays away.  This is the
idiom for short, rhythmic synth blips:

.. code-block:: das

    let pat <- note("c4 e4 g4 c5", "sine") |> decay(0.15)

No need to spell out ``sustain(0.0)`` — the resolver does it because
*"only decay was set"*.

Part C: pad — explicit ADSR
===========================

Setting all four fields gives full manual control.  A pad-style
envelope uses a long attack so the note swells in, plus a long release
so it fades out gradually after key-up:

.. code-block:: das

    let pat <- note("c3", "triangle") |> attack(0.5) |> decay(0.2) |> sustain(0.6) |> release(0.5)

Because ``sustain`` was set explicitly, the resolver leaves all four
values untouched.

Part D: tempo-aware delay defaults
==================================

The "unset sentinel + resolver" idea also applies to ``delaytime``.
Plain ``delay(0.6)`` without ``delaytime(...)`` derives ``delaytime``
from ``delaysync`` (3/16 cycle by default) and the current cps.  This
gives a dotted-eighth feel that **automatically tracks tempo
changes**:

.. code-block:: das

    let pat_slow <- note("c4 ~ e4 ~ g4 ~", "sine") |> decay(0.15) |> delay(0.6)
    play(pat_slow, 0.5lf, 4.0lf)

    let pat_fast <- note("c4 ~ e4 ~ g4 ~", "sine") |> decay(0.15) |> delay(0.6)
    play(pat_fast, 1.0lf, 4.0lf)

Same pattern, two different cps — the echo timing follows the tempo.
Setting ``delaytime`` (or ``delaysync``) explicitly bypasses the
resolver.

Part E: velocity via gain
=========================

``gain`` multiplies the envelope output.  Use it for a velocity curve
over a pattern: alternating loud and soft hits while the envelope shape
stays constant.

.. code-block:: das

    let pat <- stack([
        note("c4", "sine") |> decay(0.15) |> gain(0.9),
        note("e4", "sine") |> decay(0.15) |> gain(0.4),
        note("g4", "sine") |> decay(0.15) |> gain(0.7),
        note("c5", "sine") |> decay(0.15) |> gain(0.3)
    ])

Each voice keeps the same plucky envelope but a different peak level.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_09_adsr_envelopes.das <../../../../tutorials/daStrudel/daStrudel_09_adsr_envelopes.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_effects_filters`

   Next tutorial: :ref:`tutorial_dastrudel_scales_music_theory`
