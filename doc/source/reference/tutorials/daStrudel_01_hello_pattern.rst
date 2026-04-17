.. _tutorial_dastrudel_hello_pattern:

==========================
STRUDEL-01 — Hello Pattern
==========================

.. index::
    single: Tutorial; Strudel; Hello Pattern
    single: Tutorial; Strudel; Pattern
    single: Tutorial; Strudel; Hap

This tutorial is the entry point for the daStrudel series. It introduces the
two central concepts of the library — the ``Pattern`` type and the ``Hap``
event — and shows three different ways to create your first pattern.
By the end you will have queried a pattern by hand, played a single drum
hit through the audio system, and played a single sine note. No
mini-notation, no combinators — just the bare data model and the playback
loop.

What is a Pattern?
==================

A ``Pattern`` is **a pure function from a query window to a list of events**.
Concretely:

.. code-block:: das

    typedef Pattern = lambda<(span : TimeSpan) : array<Hap>>

You give it a ``TimeSpan`` (a half-open interval ``[start, stop)`` measured
in *cycles* — not seconds), and it returns every ``Hap`` whose lifetime
intersects that span. The function has no side effects, allocates only
the result array, and can be queried any number of times. This is the
single design idea that makes the rest of the library work — every
combinator (``fast``, ``slow``, ``rev``, ``euclid``, ``jux``, ...) just
wraps an inner pattern with a function that rewrites query spans on the
way in and result haps on the way out.

What is a Hap?
==============

A ``Hap`` is one event with two timestamps and a value:

.. code-block:: das

    struct Hap {
        whole : TimeSpan      // the event's full lifetime
        part  : TimeSpan      // the slice that fell inside the query
        has_whole : bool
        value : Event         // sound name, note number, gain, pan, ...
    }

``whole`` is when the event would naturally start and stop. ``part`` is
the portion that actually lies inside the query span. They differ when
your query cuts an event in half — the audio engine uses ``part`` to
decide what to schedule and ``whole`` to compute envelope phase.

Part A: Query a Pattern by hand
===============================

The simplest constructor is ``pure(Event)`` — it lifts an event into a
pattern that emits it once per cycle. We can call the pattern lambda
directly with ``invoke`` to see what it produces:

.. code-block:: das

    let pat <- pure(Event(s = "bd"))
    var haps <- invoke(pat, TimeSpan(start = 0.0lf, stop = 1.0lf))
    for (h in haps) {
        print("whole=[{h.whole.start}, {h.whole.stop})  s='{h.value.s}'\n")
    }
    // output: whole=[0, 1)  s='bd'

That single hap is what the audio engine will receive once per cycle and
turn into a kick-drum sample trigger. No audio yet — just data.

Part B: Play a single drum sound
================================

To turn haps into audio you need three things: an audio system, a strudel
channel, and a tick loop. The minimal harness used throughout this
tutorial series:

.. code-block:: das

    require strudel/strudel public
    require strudel/strudel_player
    require audio/audio_boost
    require daslib/fio

    def play(var pat : Pattern; seconds : float = 4.0; cps : double = 0.5lf) {
        with_audio_system() {
            strudel_create_channel()
            strudel_set_cps(cps)
            strudel_add_track(pat)
            let t0 = ref_time_ticks()
            while (float(get_time_usec(t0)) / 1000000.0 < seconds) {
                strudel_tick()
                sleep(5u)
            }
            strudel_shutdown()
        }
    }

The fully featured version of this harness — with WAV rendering,
``--duration`` and ``--track-memory`` flags — lives in
``examples/daStrudel/features/feature_common.das``. The minimal form
above is enough to get sound out and is what every tutorial in the
``01..05`` set uses.

``s("bd")`` is the shortest mini-notation: a single token. It parses to
the same Pattern as ``pure(Event(s = "bd"))`` but goes through the
mini-notation parser:

.. code-block:: das

    let pat <- s("bd")
    play(pat, 4.0)

``cps = 0.5`` means **half a cycle per second** — one cycle every two
seconds. So you hear "bd" played twice over a four-second run.

Part C: Play a single synth note
================================

For pitched sound, ``note()`` is the synth counterpart of ``s()``. It
parses note names ("c4", "eb3", "g#5") into MIDI numbers, and the
optional second argument picks an oscillator:

.. code-block:: das

    let pat <- note("c4", "sine") |> sustain(0.5)
    play(pat, 4.0)

``sustain(0.5)`` says each note holds for half its slot. Without it the
default ADSR envelope would cut the note very short.

The pipe operator ``|>`` is just function call with the left side as the
first argument — ``pat |> sustain(0.5)`` is identical to
``sustain(pat, 0.5)``. Every per-event setter (``gain``, ``pan``,
``lpf``, ``room``, ...) follows this Pattern-in / Pattern-out shape, so
you build effect chains by piping.

Where next
==========

You now know:

- A Pattern is a pure function ``TimeSpan -> array<Hap>``
- A Hap is an event with two timestamps and a value
- ``pure``, ``s``, and ``note`` are three constructors
- The minimal playback harness is ``with_audio_system + create_channel +
  set_cps + add_track + tick loop + shutdown``

Tutorial 02 introduces real mini-notation — sequences, rests, and
subdivisions — so a single ``s(...)`` call can describe a full drum
pattern.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_01_hello_pattern.das <../../../../tutorials/daStrudel/daStrudel_01_hello_pattern.das>`

   Next tutorial: :ref:`tutorial_dastrudel_mini_fundamentals`

   Related: :ref:`tutorial_lambdas` — the Pattern type is a typed lambda

   Related: :ref:`tutorial_modules` — ``require strudel/strudel`` and other module forms
