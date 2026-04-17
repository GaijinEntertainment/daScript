.. _tutorial_dastrudel_sf2_soundfont:

===================================
STRUDEL-13 — SF2 SoundFont Playback
===================================

.. index::
    single: Tutorial; Strudel; SoundFont
    single: Tutorial; SF2
    single: Tutorial; General MIDI

Sample banks are great for drums but painful for pitched instruments —
a single kick sample works fine at any note, but a piano needs dozens of
samples across the keyboard.  The General MIDI SoundFont format (.sf2)
packs a whole orchestra of multi-sampled instruments into one file, with
a standard preset numbering that makes playback portable.

daStrudel loads GM SoundFonts directly and dispatches notes to SF2
presets through the usual pattern chain.

Part A: Loading a SoundFont
===========================

``strudel_load_sf2`` reads a ``.sf2`` file and activates it for the next
patterns.  It returns ``true`` on success:

.. code-block:: das

    require strudel/strudel_player

    if (!strudel_load_sf2("FluidR3_GM.sf2")) {
        print("failed to load SoundFont\n")
    }

One SoundFont is active at a time; reloading replaces the previous one.
Any General MIDI SoundFont works — FluidR3_GM, GeneralUser GS, Musyng
Kite, Stolen Soundfont.  Larger fonts have more samples and sound better.

Part B: Instruments by Name or Number
=====================================

Two ways to pick an instrument:

.. code-block:: das

    // By GM program name — looked up in the SoundFont's preset table
    let piano <- note("c3 d3 e3 f3 g3 a3 b3 c4") |> sf2("piano") |> gain(0.6)

    // By program number (0..127) — the raw GM program code
    let flute <- note("c4 e4 g4") |> sf2(73) |> gain(0.5)

Common GM program numbers:

=================  =======================
Program            Instrument
=================  =======================
0                  acoustic grand piano
24                 nylon guitar
40                 violin
56                 trumpet
73                 flute
81                 lead square
=================  =======================

Use the program number when the name lookup does not match the preset
names in your SoundFont, or when you want a specific patch.

Part C: GM Drums on Bank 128
============================

The GM percussion kit lives on **bank 128** — every MIDI note number is
a different drum.  Switch to it with ``sf2(0) |> sf2_bank(128)`` and
play the drum map directly:

.. code-block:: das

    let drums <- note("c2 ~ d2 ~, [fs2 fs2] [fs2 fs2] [fs2 fs2] [fs2 fs2]") |> sf2(0) |> sf2_bank(128) |> gain(0.7)

Key drum mappings (octave 2):

=================  =======================
Note               Drum
=================  =======================
c2 (36)            bass drum
d2 (38)            snare
eb2 (39)           hand clap
cs2 (37)           side stick
fs2 (42)           closed hi-hat
as2 (46)           open hi-hat
=================  =======================

Part D: Expression and Velocity
===============================

Pitched SF2 presets respond to two kinds of per-note dynamics:

- ``velocity(v)`` — attack intensity (0..1) — changes timbre as well as level
- ``sf2_expression(e)`` — continuous volume (0..1) — pure level control

Together they drive realistic phrasing.  This example stacks two string
layers at different expression levels to thicken the pad:

.. code-block:: das

    let pat <- stack([
        note("<[c3,e3,g3] [f3,a3,c4]>") |> sf2("string_ensemble") |> sf2_expression(0.3) |> gain(0.5),
        note("<[g3,b3,d4] [a3,c4,e4]>") |> sf2("string_ensemble") |> sf2_expression(1.0) |> gain(0.5)
    ])

SF2 playback in daStrudel is a daslang-only extension — there is no
equivalent in strudel.cc.  See :ref:`strudel_vs_strudel_cc` for the full
list of daslang-only features.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_13_sf2_soundfont.das <../../../../tutorials/daStrudel/daStrudel_13_sf2_soundfont.das>`

   Previous tutorial: :ref:`tutorial_dastrudel_samples`

   Next tutorial: :ref:`tutorial_dastrudel_midi_files`
