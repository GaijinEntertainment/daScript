.. _tutorial_dastrudel_euclid:

==========================================
STRUDEL-05 — Euclidean Rhythms
==========================================

.. index::
    single: Tutorial; Strudel; Euclidean
    single: Tutorial; Strudel; Bjorklund
    single: Tutorial; Strudel; Polyrhythm

Euclidean rhythms are produced by **distributing k onsets as evenly as
possible across n discrete steps**. The algorithm — Bjorklund's, named
after the physicist who first wrote it down for a particle-collider
problem — turns out to generate many traditional drum patterns from
around the world. Toussaint's 2005 paper *The Euclidean Algorithm
Generates Traditional Musical Rhythms* catalogs the matches: the Cuban
*tresillo* is ``(3, 8)``; the Cumbia bell is ``(5, 8)``; the West-
African ``(4, 9)`` shows up in the Aka Pygmy ``mbira`` patterns; and
so on.

In daStrudel this is one function: ``euclid(pat, k, n)``.

Part A: ``bjorklund(k, n)`` — the underlying rhythm
====================================================

Before the combinator, here is the raw algorithm. ``bjorklund(3, 8)``
returns a length-8 ``array<bool>`` with three ``true`` entries
distributed evenly:

.. code-block:: das

    let r <- bjorklund(3, 8)
    for (b in r) {
        let glyph = b ? "x" : "."
        print("{glyph} ")
    }
    // output: x . . x . . x .

The implementation is a tight integer loop — no floating point, no
fractional accumulation, just ``(i * k) / n`` integer division. This
matters when you need to call it for big ``n`` values (per-event in a
signal, for example) — it allocates one boolean array and is otherwise
allocation-free.

Part B: ``euclid(pat, k, n)``
==============================

``euclid`` first speeds the pattern up by ``n`` (so each input cycle now
contains ``n`` repetitions), then keeps only the haps falling on the
``k`` Bjorklund onsets. The classic Cuban tresillo:

.. code-block:: das

    let pat <- atom("bd") |> euclid(3, 8)
    play(pat, 6.0)

Three kicks over eight evenly-spaced steps — ``x . . x . . x .``.
Replace ``atom("bd")`` with any Pattern and the same selection rule
applies. ``s("bd sd")`` would alternate kick / snare across the kept
onsets.

Part C: A small library of classic rhythms
==========================================

Most of the patterns Toussaint catalogs are one ``euclid`` call:

===========  ==================================
``(k, n)``   Pattern
===========  ==================================
``(3, 4)``   Three-against-four feel
``(3, 8)``   Cuban tresillo / Bossa kick
``(5, 8)``   Cumbia bell, West-African
``(7, 8)``   Dense, almost-straight
``(2, 5)``   Persian *Khafif-e-ramal*
``(3, 7)``   Bulgarian, *ruchenitza*
``(5, 16)``  Bossa Nova clave (rotated form)
===========  ==================================

Try them by changing the integers:

.. code-block:: das

    let pat <- atom("hh") |> euclid(7, 8)
    play(pat, 4.0)

Part D: Polyrhythm via ``stack``
================================

``stack`` layers patterns simultaneously. Stack two Euclidean patterns
with different ``k`` and you have a polyrhythm:

.. code-block:: das

    var kick <- atom("bd") |> euclid(3, 8)
    var hat  <- atom("hh") |> euclid(5, 8)
    var layers : array<Pattern>
    layers |> emplace <| kick
    layers |> emplace <| hat
    let pat <- stack(layers)
    play(pat, 6.0)

Both layers complete every cycle (``n = 8`` in both), but the **3
onsets** of the kick and the **5 onsets** of the hat sit at different
grid positions, so the perceived feel is polyrhythmic. Use mismatched
``n`` values (e.g. ``(3, 8)`` against ``(2, 5)``) and the cycle length
becomes ``lcm(8, 5) = 40`` — the patterns realign every 40 steps.

The ``var`` (not ``let``) on ``kick`` and ``hat`` matters: ``stack``
takes ``array<Pattern>`` and ``emplace`` needs to **move** the lambda
(Patterns are non-copyable lambdas) — that requires a mutable
binding.

Part E: ``euclidRot(pat, k, n, rot)`` — rotate the onsets
=========================================================

``euclidRot`` is ``euclid`` with the Bjorklund pattern rotated **left by
rot steps**. The density is unchanged — still ``k`` onsets over ``n``
steps — but the accents land on a different set of grid positions, which
shifts the *feel* without adding or removing hits. ``euclid(3, 8)`` hits
steps 0, 3, 6; ``euclidRot(3, 8, 2)`` shifts that window two steps:

.. code-block:: das

    let kick <- atom("bd") |> euclid(3, 8)
    play(kick, 4.0)

    let hat <- atom("hh") |> euclidRot(5, 8, 2)
    play(hat, 4.0)

Query one cycle by hand to see the onset positions move:

.. code-block:: das

    var rotd <- atom("bd") |> euclidRot(3, 8, 2)
    var haps <- invoke(rotd, TimeSpan(start = 0.0lf, stop = 1.0lf))
    for (h in haps) {
        print("{h.whole.start} ")
    }
    delete haps

Note on the ``bd(3,8)`` mini-notation form
==========================================

In the original Tidal/Strudel mini-notation, ``"bd(3,8)"`` is an inline
shorthand for euclid and ``"bd(3,8,2)"`` adds rotation. daStrudel
supports both — :ref:`tutorial 03 <tutorial_dastrudel_mini_advanced>`
(Section 5) demonstrates ``s("bd(3,8)")`` and ``s("bd(3,8,2)")``, which
the parser expands directly to the ``euclid()`` / ``euclidRot()``
function forms shown above. The function forms are the explicit
equivalents and read well in a pipe:

.. code-block:: das

    s("bd sd") |> euclid(5, 8)         // same as s("bd sd(5,8)")
    atom("hh") |> euclidRot(5, 8, 2)   // same as s("hh(5,8,2)")

Where next
==========

You now have the four building-block topics every strudel-style
language needs: the data model (01), the mini-notation surface (02–03),
the time algebra (04), and Euclidean rhythms (05). Tutorials 06+
introduce **combinators** — ``jux``, ``every``, ``off``,
``superimpose``, ``palindrome``, ``ply``, ``echo``, ``chunk`` — which
turn one-line patterns into full musical phrases.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_05_euclidean_rhythms.das <../../../../tutorials/daStrudel/daStrudel_05_euclidean_rhythms.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_time`

   Related: :ref:`tutorial_lambdas` — Patterns are non-copyable lambdas (hence ``var`` + ``emplace``)

   Related: :ref:`tutorial_arrays` — ``array<Pattern>`` and ``emplace`` semantics
