.. _tutorial_dastrudel_mini_fundamentals:

==========================================
STRUDEL-02 — Mini-Notation Fundamentals
==========================================

.. index::
    single: Tutorial; Strudel; Mini-Notation
    single: Tutorial; Strudel; Sequence
    single: Tutorial; Strudel; Subdivision
    single: Tutorial; Strudel; Rest

The mini-notation is a tiny string DSL for describing rhythmic patterns.
A line like ``"bd sd ~ cp"`` reads almost like a drum-kit transcription
and parses to a real ``Pattern``. This tutorial covers the four
primitives you will use in nearly every pattern you write: **sequences**,
**rests**, **subdivisions**, and **repeats**.

Part A: Sequences and rests
===========================

The simplest mini-notation is a space-separated list of tokens. Each
token gets an equal slice of the cycle:

.. code-block:: das

    let pat <- s("bd sd ~ cp")
    play(pat, 4.0)

Four tokens, each occupies 1/4 of a cycle. The kick lands at 0/4, the
snare at 1/4, **silence** at 2/4 (``~`` is the rest token), and the clap
at 3/4. Replace ``~`` with another sound and the rhythm fills in.

This is the canonical four-on-the-floor skeleton you will see throughout
the strudel ecosystem. ``s()`` does all the work — it tokenises the
string, builds a parse-tree-free Pattern via recursive descent, and
returns it.

Part B: Subdivisions with [ ]
=============================

Square brackets group tokens into a sub-sequence that fits inside **one
step** of the parent sequence:

.. code-block:: das

    let pat <- s("bd [sd sd] ~ cp")
    play(pat, 4.0)

Now slot two contains two snares packed together — the same total time
that a single snare took before. This is recursive: ``[a [b c] d]`` works
exactly as you expect, with ``b`` and ``c`` sharing the slot that ``b``
alone would have occupied.

Subdivision is the operation that gives mini-notation its expressive
power. With nothing else, you can already write any rational rhythm.

Part C: Repeats with \*N
========================

The ``*N`` postfix is a shorthand for "repeat this token N times within
its step":

.. code-block:: das

    let pat <- s("bd hh*4 sd hh")
    play(pat, 4.0)

``hh*4`` is exactly equivalent to ``[hh hh hh hh]`` — four hi-hats packed
into the second slot. ``*N`` is read first, so ``hh*4*2`` packs eight
hats. There is also ``/N`` for the inverse (slow down by N), but for
in-cycle repetition ``*N`` is the workhorse.

Part D: Combining the basics
============================

All three primitives nest and combine freely:

.. code-block:: das

    let pat <- s("bd [sd sd*2] ~ hh*4")
    play(pat, 4.0)

Reading left to right: kick, then a sub-sequence containing one snare
and a doubled snare, then a rest, then four hats. This single string
describes a busy backbeat — and it is still just data, just a Pattern,
ready to be transformed by ``fast``, ``rev``, ``euclid``, or any other
combinator.

Where next
==========

Tutorial 03 covers the four **advanced** mini-notation operators that
the simple sequencer does not give you: angle brackets ``<a b c>`` for
per-cycle alternation, ``@N`` for elongation, ``?`` for random degrade,
and ``!N`` for parent-level replication.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_02_mini_notation_fundamentals.das <../../../../tutorials/daStrudel/daStrudel_02_mini_notation_fundamentals.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_hello_pattern`

   Next tutorial: :ref:`tutorial_dastrudel_mini_advanced`

   Related: :ref:`tutorial_strings` — string interpolation behaves like mini-notation parsing
