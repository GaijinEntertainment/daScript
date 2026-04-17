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

Note on the ``bd(3,8)`` mini-notation form
==========================================

In the original Tidal/Strudel mini-notation, ``"bd(3,8)"`` is an inline
shorthand for euclid. **This daStrudel build does not parse parentheses
inside mini-notation strings** — the parser tokenises ``(`` and ``)``
but does not assemble them into the euclid form. Use the function
directly:

.. code-block:: das

    s("bd sd") |> euclid(5, 8)
    atom("hh") |> euclid(3, 8)

Rotation (Tidal's ``bd(3,8,1)``, the JS Strudel ``euclidRot``) is
likewise not exposed as a standalone function in this build. To get a
rotated rhythm you can reorder the elements inside the pattern fed to
``euclid``, or chain ``rev`` / ``slow`` against a control pattern.

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
