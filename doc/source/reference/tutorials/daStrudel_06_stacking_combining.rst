.. _tutorial_dastrudel_stacking_combining:

=================================
STRUDEL-06 — Stacking & Combining
=================================

.. index::
    single: Tutorial; Strudel; Stacking
    single: Tutorial; Strudel; stack
    single: Tutorial; Strudel; cat
    single: Tutorial; Strudel; layer
    single: Tutorial; Strudel; superimpose

Strudel patterns are values, not statements — and once you have a few of
them, the next question is how to combine them.  This tutorial walks
through four *structural combinators* that take patterns as input and
return a new pattern:

================  ==============  ===================================================
``stack``         vertical        all patterns play simultaneously
``cat``           horizontal      pattern *i* plays during cycle (i mod N)
``layer``         vertical        a base pattern plus stacked transforms of itself
``superimpose``   vertical        overlay one transformed copy of a pattern on top
================  ==============  ===================================================

All four return a fresh ``Pattern``, so you can pipe them through
setters (``gain``, ``room``, ``orbit``, …) just like any other pattern.

Part A: ``stack`` — play patterns at the same time
==================================================

``stack`` takes an array of patterns and merges their events into the
same cycle.  Use it to combine independent voices (bass + lead + drums):

.. code-block:: das

    let pat <- stack([
        note("c3 e3 g3 c4", "sine") |> sustain(0.5),
        note("c5 ~ ~ g4 ~ ~ e4 ~", "triangle") |> sustain(0.3) |> gain(0.4)
    ])

Each element of the array is treated as one voice that runs in parallel.
Note that ``stack`` consumes its argument array (``var pats`` parameter),
so you cannot reuse the inner patterns afterwards.

Part B: ``cat`` — sequence patterns across cycles
=================================================

``cat`` is the horizontal counterpart.  With *N* patterns in the array,
pattern *i* plays during cycle (i mod N), each stretched to fill exactly
one cycle.  This is the standard way to glue short building blocks into
longer arrangements:

.. code-block:: das

    let pat <- cat([
        note("c4 e4 g4 c5", "sine") |> sustain(0.4),
        note("c5 g4 e4 c4", "sine") |> sustain(0.4),
        note("c4 e4 g4 b4", "sine") |> sustain(0.4)
    ])

Three cycles of melody, then it loops.

Part C: ``layer`` — chord-from-melody
=====================================

``layer(pats, base)`` stacks ``base`` plus every pattern in ``pats``.
The idiom is to derive every layer from the same source so that one
melody plays as a chord:

.. code-block:: das

    let pat <- layer([
        note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> transpose(4.0),  // major third
        note("c4 e4 g4 c5", "sine") |> sustain(0.4) |> transpose(7.0)   // perfect fifth
    ], note("c4 e4 g4 c5", "sine") |> sustain(0.4))                      // root

Each note plays as a triad: root + third + fifth.

Part D: ``superimpose`` — overlay a transformed copy
====================================================

``superimpose(pat, fn)`` is equivalent to ``stack([pat, fn(pat)])`` but
reads more naturally because it takes the transform as a lambda.  Below
we add a faster octave-up echo on top:

.. code-block:: das

    let pat <- superimpose(note("c4 e4 g4 c5", "sine") |> sustain(0.4),
                           @(p) => gain(fast(transpose(p, 12.0), 2.0lf), 0.4))

The lambda has type ``@(Pattern) => Pattern`` — the same shape every
transforming combinator (``jux``, ``off``, ``superimpose``, …) expects.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_06_stacking_combining.das <../../../../tutorials/daStrudel/daStrudel_06_stacking_combining.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_euclid`

   Next tutorial: :ref:`tutorial_dastrudel_per_voice_fx`

   Related: :ref:`tutorial_lambdas`, :ref:`tutorial_functions`
