.. _tutorial_dastrudel_one_shots:

========================================================
STRUDEL-19 — One-Shots: Fire-and-Forget Stings on Events
========================================================

.. index::
    single: Tutorial; Strudel; one-shot
    single: Tutorial; Strudel; strudel_one_shot
    single: Tutorial; Strudel; playFor
    single: Tutorial; Strudel; once

A strudel pattern is normally an **endless loop**.  daStrudel adds the
piece a game or tool needs for event audio: a pattern that plays **once
and then stops and dies**.  This is what lets a pattern be used as a
sound effect — a win sting, a pickup chime, an impact — fired from host
state, with no baked or shipped audio asset.

There are two halves:

- **stop** — ``once(pat)`` / ``playFor(pat, n)`` gate a pattern to its
  first cycle (or first ``n`` cycles) and emit silence forever after.
  These are pure ``Pattern`` combinators; they compose with everything.
- **die** — ``strudel_one_shot(pat, gain, quant_cycles, len_cycles)``
  fires ``pat`` as a **self-retiring track**: it plays, goes silent, and
  the track removes itself from the mix once every voice has finished
  ringing (so the release tail is never clipped).

Part A: ``once`` / ``playFor`` — the *stop* half
================================================

You can see the *stop* without listening: query the gated pattern and
watch the onsets vanish past the window.  ``once`` keeps cycle 0;
``playFor(pat, n)`` keeps the first ``n`` cycles:

.. code-block:: das

    let phrase <- once(note("c5 e5 g5 c6", "triangle"))
    var c0 <- phrase(TimeSpan(start = 0.0lf, stop = 1.0lf))   // 4 onsets — plays
    var c1 <- phrase(TimeSpan(start = 1.0lf, stop = 2.0lf))   // 0 onsets — silent forever

Because they are ordinary combinators, the gate composes with the rest
of the pattern algebra (``fast``, ``stack``, the fluent setters, …)
before you ever turn it into a one-shot.

Part B: ``strudel_one_shot`` — the *die* half
=============================================

Fire a sting from the host loop.  The track is live while it sounds,
then auto-removes once its voices stop ringing — you never call
``strudel_remove_track``.  ``strudel_active_tracks()`` reports how many
tracks are live, so the host (or a test) can ask "is the sting still
ringing?":

.. code-block:: das

    let sting <- (note("c5 e5 g5 c6", "triangle")
        |> gain(0.8) |> room(0.5) |> orbit(2) |> attack(0.005) |> release(0.4))
    strudel_one_shot(sting, 1.0, 0.0, 0.5)   // gain 1.0, immediate, half a cycle long
    // ... keep calling strudel_tick() in the host loop; the track retires itself

Compose ``orbit`` / ``room`` / ``gain`` into ``pat`` to give the sting
its own reverb bus, independent of the music tracks.

Part C: Quantize to the beat
============================

``quant_cycles > 0`` snaps the start to the global beat grid: ``0`` is
immediate, ``0.25`` is the next quarter-cycle beat.  A sting triggered at
an arbitrary instant then waits a few milliseconds so it lands musically
in time instead of on the exact frame of the event:

.. code-block:: das

    strudel_one_shot(sting, 1.0, 0.25, 0.5)   // snap to the next 1/4-cycle beat

Part D: Parametric SFX — a different sting per event
=======================================================

Each one-shot is a fresh, independent track, so the pattern can vary per
event.  Fire a higher sting as "events" intensify — pitch by impact,
instrument by surface — all without a single audio file:

.. code-block:: das

    let phrases <- ["c4 e4 g4", "e4 g4 c5", "g4 c5 e5", "c5 e5 g5"]
    // on event i:
    let sting <- (note(phrases[i], "triangle")
        |> gain(0.7) |> room(0.4) |> orbit(2) |> attack(0.005) |> release(0.3))
    strudel_one_shot(sting, 1.0, 0.0, 0.5)

This is the *discrete-event* complement to the *continuous* adaptive
control (``signal`` + orbit fades) covered in the comparison page:
sustained layers crossfade with signals and orbit levels; momentary
events fire one-shots.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_19_one_shots.das <../../../../tutorials/daStrudel/daStrudel_19_one_shots.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_sfx_lab`

   How daStrudel differs from strudel.cc (one-shots are a daStrudel
   extension): :ref:`strudel_vs_strudel_cc`

   Full daStrudel reference: :ref:`stdlib_strudel_section`
