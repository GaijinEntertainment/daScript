.. _tutorial_dastrudel_time:

==========================================
STRUDEL-04 — Time Manipulation
==========================================

.. index::
    single: Tutorial; Strudel; Time Manipulation
    single: Tutorial; Strudel; fast
    single: Tutorial; Strudel; slow
    single: Tutorial; Strudel; rev
    single: Tutorial; Strudel; hurry

So far every Pattern played at one cycle per cycle, in forward order.
This tutorial introduces the **time algebra**: four functions that take
a Pattern and return a new Pattern by rewriting the query timeline.
``fast`` and ``slow`` change density, ``rev`` mirrors per-cycle, and
``hurry`` couples speed with pitch. They all compose freely, and they
work on any Pattern — mini-notation, ``pure``, or signals.

The mental model: a combinator wraps a Pattern with a function that
**rewrites the query span on the way in** and **rewrites the result
haps on the way out**. ``fast(2)`` queries the inner pattern with
``span * 2``, then divides every returned hap's timestamps by 2.
``slow(N)`` is exactly ``fast(1/N)``. ``rev`` mirrors the span within
each cycle and mirrors the haps back. That is the entire framework.

Part A: ``fast(N)``
===================

``fast(N)`` repeats the pattern N times per cycle:

.. code-block:: das

    let pat <- s("c4 e4 g4 c5") |> fast(2.0lf)
    play(pat, 4.0)

The four-note arpeggio plays twice per cycle. ``N`` is a ``double``,
so non-integer values work — ``fast(1.5lf)`` gives 3 repetitions every
2 cycles, useful for polyrhythms (covered in tutorial 05).

Part B: ``slow(N)``
===================

``slow(N)`` stretches one pass of the pattern over N cycles. It is
defined as ``fast(1.0lf / n)``:

.. code-block:: das

    let pat <- s("bd sd hh cp") |> slow(2.0lf)
    play(pat, 6.0)

The four hits now span two cycles instead of one. Use this to stretch
melodies, make pad-style sustains, or build a slow LFO out of a signal
pattern.

Part C: ``rev``
===============

``rev`` reverses event positions **within each cycle**:

.. code-block:: das

    let pat <- note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> rev
    play(pat, 4.0)

The arpeggio plays backward as ``c5 g4 e4 c4``. This is per-cycle
mirroring — not a global reverse. Each cycle stands alone, so
``rev |> rev`` is the identity. Combine it with ``jux`` to get instant
stereo widening (tutorial 06 covers combinators).

``rev`` is callable bare or with parentheses: ``pat |> rev`` and
``pat |> rev()`` are equivalent.

Part D: ``hurry(N)``
====================

``hurry(N)`` speeds time up by N **and** multiplies the per-event
playback ``speed`` by N. For sample-based sounds (drum hits,
field-recordings), speed scales playback rate, which means pitch goes
up by ``log2(N)`` octaves:

.. code-block:: das

    let pat <- s("c3 e3 g3 c4") |> hurry(2.0lf)
    play(pat, 4.0)

The pattern plays twice as fast and an octave higher. This is a single
function call away from the classic chipmunk effect. For synth voices
(``sine``, ``sawtooth``, ``supersaw``), ``hurry`` and ``fast`` differ
only in the ``speed`` field — pitch comes from ``note``, not ``speed``.

Part E: Composition
===================

The four functions all return Patterns, so they compose with the pipe
operator like any other transform:

.. code-block:: das

    let pat <- note("c4 e4 g4 b4", "sine")
        |> sustain(0.4)
        |> rev
        |> slow(2.0lf)
    play(pat, 6.0)

Order matters — ``rev |> slow(2)`` reverses then stretches; ``slow(2) |>
rev`` stretches then reverses (and the two produce different rhythms in
general because ``rev`` operates per-cycle).

A note on ``early`` and ``late``
================================

You may have seen ``early(N)`` / ``late(N)`` in other Tidal/Strudel
documentation — they shift the pattern earlier or later by N cycles.
In this build they exist but are **private** to the strudel module
(used internally by ``off``, the canon-style overlay combinator). To
shift a pattern, the public route is to chain ``slow`` and ``cat``, or
to wait for them to be exported in a future release.

Where next
==========

Tutorial 05 covers **Euclidean rhythms** — the ``euclid(pat, k, n)``
combinator that distributes ``k`` events evenly across ``n`` steps and
produces traditional drum patterns from many cultures with a single
line.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_04_time_manipulation.das <../../../../tutorials/daStrudel/daStrudel_04_time_manipulation.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_mini_advanced`

   Next tutorial: :ref:`tutorial_dastrudel_euclid`

   Related: :ref:`tutorial_lambdas` — Patterns are lambdas, and so are the wrappers ``fast``/``slow``/``rev`` build

   Related: :ref:`tutorial_functions` — pipe operator ``|>`` is just function call syntax
