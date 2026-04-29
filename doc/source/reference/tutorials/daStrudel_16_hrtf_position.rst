.. _tutorial_dastrudel_hrtf_position:

====================================================
STRUDEL-16 — HRTF: 3D Positional Override for Pan
====================================================

.. index::
    single: Tutorial; Strudel; HRTF
    single: Tutorial; Strudel; 3D positioning
    single: Tutorial; Strudel; hrtf_azimuth
    single: Tutorial; Strudel; hrtf_elevation

daslang exposes a per-event HRTF position (azimuth, elevation) on
top of the standard mini-notation.  When set, the equal-power
stereo ``pan`` render is replaced by a **binaural-stereo HRTF**:
the dry signal and the orbit-effect sends are run through the same
CIPIC-based ``ma_hrtf`` engine that ``audio_boost`` uses for 3D
sources.  Pan still shapes source width / L/R balance feeding the
spatialiser; HRTF places the source in 3D.

Why bother — equal-power ``pan`` only places sources on a horizontal
stereo line between the speakers.  HRTF adds:

- depth / externalisation (sources feel "outside the head" on
  headphones),
- front-back disambiguation (centre-front is no longer ambiguous with
  centre-rear),
- elevation cues.

Cost — under the binaural-stereo render, each HRTF-positioned voice
carries **two** ``ma_hrtf`` instances (``hrtf_l`` + ``hrtf_r``,
lazy-allocated on first render: one per virtual-speaker channel at
azimuth -/+ 30°).  Practical for 3-8 simultaneous HRTF voices on
modern hardware.  Plain ``pan`` is far cheaper; reach for HRTF only
when you want spatial cues plain pan can't provide.

API
===

.. code-block:: das

    pat |> hrtf_azimuth(deg)        // numeric or pattern-valued, -180..180
    pat |> hrtf_elevation(deg)      // numeric or pattern-valued, -90..90
    pat |> hrtf(az, el)             // combined numeric setter

Setting any of the three flips the event's ``hrtf_active`` flag.
**Pan and HRTF are orthogonal under the binaural-stereo render**:
``pan`` controls the inherent stereo width / image of the source (it
shapes the L/R balance of the input feeding the spatialiser), and
``hrtf`` positions the resulting source in 3D.  Best heard on
headphones.

Part A: Static positions
========================

``hrtf(az, el)`` takes both axes at once.  ``0/0`` is directly ahead,
``+90/0`` is right, ``-90/0`` is left, ``0/+45`` is up-front:

.. code-block:: das

    let pat <- stack([
        note("c4", "sine") |> hrtf(0.0,    0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("e4", "sine") |> hrtf(-90.0,  0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("g4", "sine") |> hrtf(+90.0,  0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("c5", "sine") |> hrtf(0.0,   45.0) |> attack(0.01) |> release(0.5) |> gain(0.4)
    ])

Four notes from four directions: ahead, hard-left, hard-right,
up-front.

Part B: Animated azimuth
========================

The setters accept patterns.  Each event samples the modulation
pattern at its onset to get its azimuth.  With
``sine() |> range(-180, 180) |> slow(4)``, the source orbits the
listener once every 8 cycles (on a 0.5 cps stream):

.. code-block:: das

    var pat <- (
        note("c4 e4 g4 c5", "triangle")
        |> hrtf_azimuth(sine() |> range(-180.0, 180.0) |> slow(4.0lf))
        |> attack(0.005) |> release(0.3) |> gain(0.4)
    )

``hrtf_elevation`` works the same way; combine the two for full
spherical motion.

Part C: Pan and HRTF combine — HRTF positions, pan widens
=========================================================

The render branch is selected per voice.  When ``hrtf_active`` is
true the equal-power L/R mixer is replaced by a binaural-stereo HRTF:
the L and R input channels become virtual sources at azimuth - 30°
and azimuth + 30°, each through its own HRTF instance, summed at the
output.  The pre-pan stereo image (whatever ``pan`` produced) is
preserved as source width; HRTF places the source in 3D.  The same
binaural render is reused for the reverb / delay / chorus sends.

.. code-block:: das

    var pat <- stack([
        note("c4", "sine") |> pan(0.5)                     |> gain(0.4),
        note("e4", "sine") |> pan(0.5) |> hrtf(-90.0, 0.0) |> gain(0.4)
    ])

The second voice has both ``pan(0.5)`` and ``hrtf(-90, 0)``.  The
voice is positioned to the left by HRTF; the slight L/R imbalance
that ``pan(0.5)`` produced is preserved as source width inside that
3D position (rather than being thrown away).

Part D: Mixing HRTF and plain pan
=================================

HRTF is heavier than pan and works best on headphones.  For drum
machines or beats where you just want left/right separation, plain
``pan`` is cheaper and works fine on speakers.  Mix-and-match in the
same stack: HRTF the lead voice, pan the rhythm section:

.. code-block:: das

    var pat <- stack([
        s("bd ~ sd ~")                  |> orbit(0) |> gain(0.6),
        s("hh*8")                       |> orbit(0) |> pan(0.7) |> gain(0.3),
        note("c4 e4 g4 e4", "triangle") |> orbit(1)
            |> hrtf_azimuth(sine() |> range(-60.0, 60.0) |> slow(2.0lf))
            |> room(0.4) |> roomsize(2.0) |> gain(0.35)
    ])

Because the lead is on a separate orbit, its reverb stays distinct
from the (dry) drum bus.  The reverb send for the lead voice is
itself HRTF-positioned, so the room follows the source through space.

.. seealso::

   Full source: :download:`tutorials/daStrudel/daStrudel_16_hrtf_position.das <../../../../tutorials/daStrudel/daStrudel_16_hrtf_position.das>`

   Standalone examples: ``examples/daStrudel/features/hrtf_basic.das``,
   ``examples/daStrudel/features/hrtf_animated.das``,
   ``examples/daStrudel/features/hrtf_overrides_pan.das``

   Previous tutorial: :ref:`tutorial_dastrudel_live_reloading`

   Mini-notation compatibility: :ref:`mini-notation compatibility <strudel_vs_strudel_cc>`

   Full daStrudel reference: :ref:`stdlib_strudel_section`
