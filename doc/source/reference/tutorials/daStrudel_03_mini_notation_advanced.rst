.. _tutorial_dastrudel_mini_advanced:

==========================================
STRUDEL-03 — Mini-Notation Advanced
==========================================

.. index::
    single: Tutorial; Strudel; Mini-Notation
    single: Tutorial; Strudel; Alternation
    single: Tutorial; Strudel; Elongation
    single: Tutorial; Strudel; Degrade
    single: Tutorial; Strudel; Replicate

The four operators in this tutorial — ``<...>``, ``@N``, ``?``, and
``!N`` — are what take you from drum-machine patterns to genuinely
musical phrases. They also exist purely inside the mini-notation parser:
each one rewrites the parsed pattern into the same combinators (``cat``,
``weighted_fastcat``, ``degrade``, ``fast``) you can call directly from
daslang. Knowing both forms is useful: mini-notation for terseness,
combinators when the input is dynamic.

Part A: Alternation with ``< >``
================================

Angle brackets pick **one element per cycle**, advancing on each cycle
and looping when it runs out:

.. code-block:: das

    let pat <- s("<bd sd cp>")
    play(pat, 6.0)

You hear ``bd`` on cycle 0, ``sd`` on cycle 1, ``cp`` on cycle 2, then
back to ``bd``. This is the ``cat`` combinator under the hood: the angle
group spans one full cycle each iteration.

Inside a sequence the angle group still occupies one **slot**:

.. code-block:: das

    let pat <- s("bd <sd cp> hh hh")
    play(pat, 4.0)

The ``<sd cp>`` slot alternates each cycle — snare on cycle 0, clap on
cycle 1. This pattern is how you write "second beat changes" in a single
line.

Part B: Elongation with ``@N``
==============================

Postfix ``@N`` makes an element occupy **N units** of the parent
sequence (default is 1). It is implemented via ``weighted_fastcat`` —
the parser tracks weights for each token and divides the cycle
proportionally:

.. code-block:: das

    let pat <- s("bd@3 sd") |> sustain(0.5)
    play(pat, 4.0)

Without ``@3`` each token would get half a cycle. With it the kick gets
3/4 and the snare gets 1/4. This is how you write swung or front-loaded
phrases without nested brackets.

Underscore ``_`` is the equivalent for **the previous element**:
``"bd _ _ sd"`` is the same as ``"bd@3 sd"``. Use whichever reads better
for your phrase.

Part C: Degrade with ``?``
==========================

Postfix ``?`` randomly drops events at the marked element with 50%
probability. Hi-hats are the canonical use:

.. code-block:: das

    let pat <- s("bd hh? sd hh?")
    play(pat, 6.0)

The kick and snare are reliable; the hats appear roughly half the time,
randomly per cycle. Run it twice and you get different results — the
RNG seed is tied to the cycle position so the rhythm is deterministic
within one play, but the pattern feels human.

You can also write ``hh?0.25`` to use a different drop probability —
the default ``?`` is shorthand for ``?0.5``.

Part D: Replicate with ``!N``
=============================

Postfix ``!N`` is like ``*N`` from tutorial 02 but expands the element
into N **parent slots** instead of squeezing them into one:

.. code-block:: das

    let pat <- s("bd!3 sd")
    play(pat, 4.0)

This expands to four equal slots ``bd bd bd sd`` — three kicks then a
snare. Compare to ``bd*3 sd`` which packs three kicks into one slot
followed by a snare in the second.

The mental model: ``*N`` divides time, ``!N`` adds slots. Use ``*`` to
make things faster within one slot, ``!`` to repeat the same element
across the parent sequence.

Where next
==========

You now know the full mini-notation surface. Tutorial 04 leaves the
parser behind and shows the **time algebra** — ``fast``, ``slow``,
``rev``, ``hurry`` — which transforms any Pattern (mini-notation or
not) by rewriting query spans.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_03_mini_notation_advanced.das <../../../../tutorials/daStrudel/daStrudel_03_mini_notation_advanced.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_mini_fundamentals`

   Next tutorial: :ref:`tutorial_dastrudel_time`

   Related: :ref:`tutorial_random` — how degrade's randomness is seeded
