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
    single: Tutorial; Strudel; Euclidean

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

.. das-doc: given require strudel/strudel public
.. das-doc: given def play(var pat : Pattern; seconds : float = 4.0; cps : double = 0.5lf) { }

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
varying from cycle to cycle. The drop is decided by hashing each event's
start time, so a given pattern loses the same hats on every run. It
feels human, but it is reproducible — not random.

You can also write ``hh?0.25`` to use a different drop probability —
the default ``?`` is shorthand for ``?0.5``.

Part D: Replicate with ``!N``
=============================

Postfix ``!N`` repeats an element N times. The parser reads it as a
modifier on the element it follows, exactly like ``*N``, so the repeats
land **inside that element's own slot**:

.. code-block:: das

    let pat <- s("bd!3 sd")
    play(pat, 4.0)

``"bd!3 sd"`` is two slots: three kicks packed into the first half, one
snare in the second. It produces the same haps as ``"bd*3 sd"`` — query
both by hand and the timestamps match.

To give each repeat a slot of its own — four equal slots, three kicks
then a snare — write the element out:

.. code-block:: das

    let pat <- s("bd bd bd sd")
    play(pat, 4.0)

Part E: Euclidean rhythms with ``(k,n)`` and ``(k,n,rot)``
==========================================================

Postfix ``(k,n)`` spreads ``k`` onsets as evenly as possible over ``n``
steps. ``bd(3,8)`` is the classic tresillo — three kicks across eight
slots:

.. code-block:: das

    var pat <- s("bd(3,8)")
    var haps <- invoke(pat, TimeSpan(start = 0.0lf, stop = 1.0lf))
    for (h in haps) {
        print("  onset at {h.whole.start}\n")
    }
    play(pat, 4.0)

Querying one cycle ``[0,1)`` by hand and printing ``h.whole.start`` shows
exactly where the hits land — ``0``, ``0.375``, ``0.75`` for the 3-in-8
tresillo. The parser rewrites ``(k,n)`` to the ``euclid(pat, k, n)``
combinator.

The three-argument form ``(k,n,rot)`` rotates the onset pattern left by
``rot`` steps — the rhythm is the same, only its starting offset moves:

.. code-block:: das

    var pat <- s("bd(3,8,2)")
    play(pat, 4.0)

This rewrites to ``euclidRot(pat, k, n, rot)``. The function forms
``euclid`` and ``euclidRot`` are public and covered in tutorial 05 — use
them directly when ``k``/``n`` come from variables rather than a literal
string.

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
