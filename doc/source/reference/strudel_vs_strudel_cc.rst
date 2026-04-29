.. _strudel_vs_strudel_cc:

====================================================
daslang strudel vs strudel.cc — Feature Comparison
====================================================

This page compares the daslang ``strudel`` library
(``modules/dasAudio/strudel/``) to its inspiration, the strudel.cc
live-coding system.  It is the **only** documentation page that names
strudel.cc explicitly — all other pages should reference this one for
anything comparative.

Read this before porting a strudel.cc pattern to daslang. The
Claude-assistant porting workflow lives in the repository at
``skills/strudel_port.md``. For the
generated reference of every strudel symbol, see
:ref:`stdlib_strudel_section`. For the tutorial series, see
:ref:`tutorials_dastrudel`.

.. contents:: Contents
   :local:
   :depth: 2


.. _strudel_cc_overview:

Scope and philosophy
====================

The daslang port is **feature-focused, not code-ported**.  We implement
the musical primitives of strudel.cc on top of daslang's compiler,
pattern matcher, and audio engine — but with no JavaScript runtime, no
web UI, no REPL, and no external DSP graph.  Everything runs in one
daslang context, with a per-voice synthesis pipeline and per-orbit
effect busses.

Two consequences of that design:

* **The core pattern algebra, mini-notation, scales, SF2/MIDI, and
  live-reload are first-class.**  For the vast majority of strudel.cc
  patterns you can rewrite the code almost verbatim — same names,
  same mini-notation, same combinators.
* **The synth engine and effect routing are daslang-native and
  sometimes diverge.**  Per-voice FX, orbit busses, and ADSR defaults
  all have specific semantics that do not match strudel.cc exactly.
  Those cases are enumerated below.


.. _strudel_cc_have:

What we have
============

Parity or near-parity with strudel.cc:

.. list-table::
   :header-rows: 1
   :widths: 30 20 50

   * - Feature area
     - daslang module
     - Notes
   * - Mini-notation
     - ``strudel_mini``
     - ``"bd sd ~ cp"`` literals via ``s``, ``n``, ``note``, ``seq``;
       subdivisions ``[a b]``, repeats ``a*4``, rests ``~``, angle
       brackets ``<a b c>``, elongation ``@``, degrade ``?``,
       replicate ``!``
   * - Pattern algebra
     - ``strudel_pattern``
     - ``Pattern`` as a time-span → haps function; ``pure``,
       ``silence``, ``stack``, ``cat``, ``fastcat``, ``fmap``
   * - Time combinators
     - ``strudel_pattern``
     - ``fast``, ``slow``, ``rev``, ``hurry``, ``compress``,
       ``linger``, ``palindrome``, ``ply``, ``iter``, ``iterBack``,
       ``chunk``, ``striate``, ``when_cycle``, ``every``
   * - Per-voice combinators
     - ``strudel_pattern``
     - ``jux``, ``off``, ``layer``, ``superimpose``, ``echo``,
       ``stut``, ``transpose``, ``add``
   * - Choice combinators
     - ``strudel_pattern``
     - ``choose``, ``wchoose``, ``randcat``, ``chooseCycles``,
       ``shuffle``, ``scramble``, ``sometimes``, ``often``,
       ``sometimesby``, ``degrade``, ``degradeBy``
   * - Euclidean rhythms
     - ``strudel_pattern``
     - ``euclid(k, n)``, ``bjorklund``.  (``euclidRot`` and
       mini-notation ``bd(3,8)`` forms are gaps — see below.)
   * - Scales
     - ``strudel_scales``
     - ``scale("c:minor")``, ``note`` with scale context, modes
       (dorian, mixolydian, …), pentatonic, blues, major/minor
   * - Signals
     - ``strudel_pattern``
     - ``sine``/``cosine``/``saw``/``tri``/``square``/``isaw``/``itri``
       and their bipolar ``*2`` variants, ``perlin``, ``rand``/``irand``,
       ``run``, ``range``
   * - Synthesis
     - ``strudel_synth``
     - oscillators (``sine``, ``sawtooth``, ``square``, ``triangle``,
       ``supersaw``), noise (``white``, ``pink``), FM via ``fm`` +
       ``fmh``, vowel/formant filter, per-voice biquads. Note
       ``sawtooth`` is spelled out — there is no ``saw`` alias (unlike
       strudel.cc)
   * - Samples
     - ``strudel_samples``
     - WAV / MP3 / FLAC / OGG loading via ``load_audio_file`` /
       ``strudel_load_sound`` / ``strudel_load_sample_dir``; sample
       banks, per-pattern speed/pitch via ``speed``
   * - SF2 soundfonts
     - ``strudel_sf2`` + ``strudel_sf2_voice``
     - Full SF2 parser (format 0/1 meta-events, generators,
       modulators), per-voice envelope / LFO / biquad, GM drum map
       compatibility, expression and mod-wheel CCs
   * - MIDI playback
     - ``strudel_midi`` + ``strudel_midi_player``
     - Format 0/1 parser, merged-track playback, GM preset mapping,
       integration with synth or SF2 backend
   * - Live-reload
     - ``strudel_live``
     - Persistent state across source reload via
       ``save_strudel_state`` / ``restore_strudel_state`` and the
       ``daslang-live`` host; ``[live_command]``,
       ``[before_reload]``, ``[after_reload]``
   * - Effects (send busses)
     - ``strudel_scheduler``
     - Per-orbit ``room`` / ``delay`` / ``chorus``; each orbit
       number routes to one ``OrbitBus``
   * - Effects (per-voice)
     - ``strudel_synth`` (``VoiceFX``)
     - Phaser, tremolo, compressor, waveshaper, DJ filter, bitcrush,
       sample-rate reduction — all applied per voice before the
       orbit mix


.. _strudel_cc_missing:

What we don't have (yet)
========================

Features that are present in strudel.cc and absent in the daslang port:

* **``euclidRot``** — Euclidean rotation helper.  Not implemented;
  compose ``euclid`` with ``rev`` or ``off`` for a workaround.
* **Mini-notation ``bd(3,8)`` form** — tokens ``(`` / ``)`` are
  lexed but not parsed into Euclidean rhythms.  Use the
  ``euclid(pat, 3, 8)`` function form instead.
* **``jux_rev`` / ``chop`` / other convenience aliases** — the
  parametric forms (``jux(pat, fn)``, ``stutter``, etc.) are
  public; a few one-letter convenience wrappers were not ported.
* **Web sample packs** — strudel.cc streams sample packs from
  the web at runtime; daslang loads local samples from disk (via
  ``strudel_load_sample_dir``).  Bring your own sample library.
* **REPL UI** — strudel.cc has a browser REPL with visualisation;
  daslang uses ``daslang-live`` for hot-reload and a separate
  visualiser example (``examples/daStrudel/strudel_visualizer/``).
* **OSC / tidalcycles output** — the daslang port renders audio
  directly.  There is no equivalent of strudel.cc's SuperDirt /
  OSC backend.
* **Hydra-style visual patterns** — out of scope; daslang is
  audio-only.
* **Some mini-notation modifiers** — a handful of exotic forms
  (e.g. some of the richer polymeter notations) may be missing;
  check ``strudel_mini.das`` for the authoritative list of
  supported tokens.


.. _strudel_cc_differences:

Behavioural differences
=======================

Cases where a primitive with the **same name** behaves differently
in the daslang port.  Before porting a pattern that uses one of
these, read the table — the output may differ even if the code
looks identical.

ADSR defaults are tempo-aware
-----------------------------

**Divergence:** daslang scales the default attack / decay / sustain /
release values with the current CPS so that a default envelope feels
right at any tempo.  strudel.cc uses fixed-millisecond defaults.

**Why:** When you change tempo with ``strudel_set_cps`` mid-pattern,
you don't want the envelope to suddenly overpower the note or shorten
to a click.  The defaults are defined so that a whole-note envelope
fills one cycle at the current tempo.

**How to apply:**

* For patterns that relied on strudel.cc's fixed ADSR, pass explicit
  values: ``pat |> attack(0.01) |> release(0.5)``.
* If you want the daslang behaviour in strudel.cc, multiply the
  attack / release by the period (1 / CPS).

See the ``adsr-defaults`` branch history for the full introduction.

Per-voice FX chain
------------------

**Divergence:** ``phaser``, ``tremolo``, ``compressor``, ``shape``,
``crush``, ``coarse``, and ``djf`` are applied **per voice**, inside
the voice renderer, before the orbit mix.  strudel.cc applies most
of these after the send-bus mix.

**Why:** Per-voice FX gives a cleaner sound for polyphonic patterns
(one voice's compressor can't duck another voice's transient) and
integrates tightly with the ``VoiceFX`` struct, which the per-voice
synth engine already carries.

**How to apply:** for patterns that rely on strudel.cc's post-bus
phaser, use ``room`` / ``delay`` bus routing (per-orbit) instead.
Per-voice combinators like ``jux`` interact with per-voice FX
differently — each transformed voice carries its own FX chain.

Orbit bus model
---------------

**Divergence:** daslang has an explicit ``OrbitBus`` per orbit
number.  ``room`` sends to that orbit's reverb, ``delay`` to that
orbit's delay, and ``chorus`` to that orbit's chorus — each FX
instance is allocated lazily on first send.  strudel.cc routes
reverb/delay through SuperDirt differently.

**How to apply:** if your strudel.cc pattern mixes two orbits to
share a reverb, in daslang they need the same ``orbit`` number.
If you want independent reverbs/choruses per voice, split orbits.

Mini-notation parsing
---------------------

**Divergence:** mini-notation strings are **only parsed** by
``parse_pattern`` / ``s`` / ``n`` / ``note`` / ``seq`` — not by
any implicit string coercion.  ``pat = "bd sd"`` is a string
literal, not a ``Pattern``.

**Why:** daslang is statically typed; implicit coercion from
``string`` to ``Pattern`` would require custom conversion at every
call site.  Explicit constructors are clearer and play better with
type-dispatched combinators.

**How to apply:** always wrap mini-notation in ``s("bd sd")``,
``n("0 2 4")``, ``note("c4 e4 g4")``, or ``parse_pattern("...")``.

``fast`` at non-integer ratios
------------------------------

**Divergence:** both engines allow fractional factors (``fast(1.5)``),
but the underlying hap-slicing uses floor-division in daslang's
``splitSpans`` helper.  Edge cases where the speedup causes a hap to
straddle a cycle boundary are resolved by keeping the earlier half;
strudel.cc may keep the later half.

**How to apply:** for rhythms near integer factors, behaviour is
identical.  For oddly fractional ``fast`` values, compare audibly
and adjust.

Scheduler and voice pool
------------------------

**Divergence:** daslang has a fixed voice-pool size (set at
``Scheduler`` init); when exceeded, the oldest voice is stolen.
strudel.cc allocates voices dynamically per hap.

**How to apply:** very dense polyphony may drop notes in daslang.
Raise the pool size at ``Scheduler`` construction or thin the
pattern.


.. _strudel_cc_extensions:

Extensions over strudel.cc
==========================

daslang adds a few primitives strudel.cc does not have.  These ride
on top of the underlying ``audio_boost`` engine, so the existing
strudel.cc syntax keeps working — the extensions only activate when
you reach for them.

HRTF positional override
------------------------

**Extension:** per-event ``hrtf_azimuth`` / ``hrtf_elevation`` (and
the combined ``hrtf(az, el)``) replace the equal-power stereo
``pan`` *render path* with a binaural-stereo HRTF: each input
channel becomes a virtual source at azimuth -/+ 30° (the standard
ITU listening triangle), each through its own ``ma_hrtf``
instance, summed at the output.  Pan and HRTF become orthogonal —
``pan`` shapes the inherent stereo width / image of the source,
``hrtf`` positions the source in 3D.  Same CIPIC-based dataset
``audio_boost`` uses; baked into the binary at compile time, no
extra setup needed.

**API:**

.. code-block:: das

   pat |> hrtf_azimuth(deg)        // numeric, -180..180
   pat |> hrtf_elevation(deg)      // numeric, -90..90
   pat |> hrtf(az, el)             // combined numeric

   // pattern-valued (animated):
   pat |> hrtf_azimuth(sine() |> range(-90.0, 90.0) |> slow(4.0lf))

Setting any of the three flips the event's ``hrtf_active`` flag.
The two HRTF outputs are summed and re-used for the
reverb / delay / chorus sends, so the entire wet path follows the
spatialised source (full physical accuracy) before being scaled
into the orbit effect buses.

**Cost:** each HRTF voice carries *two* ``ma_hrtf`` instances
(one per input channel, both allocated lazily on first render).
Practical for a handful of simultaneous voices; reach for plain
``pan`` when you don't need externalisation, depth, or front/back
disambiguation.

**See also:**
- Tutorial: :ref:`tutorial_dastrudel_hrtf_position`
- Demo: ``examples/daStrudel/hrtf/``
- Engine reference: :ref:`ma_hrtf <handle-audio-ma_hrtf>`


.. _strudel_cc_naming:

Naming map (quick reference)
============================

For porting at speed: the daslang name is the **same** as strudel.cc
for ~95% of primitives.  When it differs:

.. list-table::
   :header-rows: 1
   :widths: 35 35 30

   * - strudel.cc
     - daslang
     - Notes
   * - ``s("bd sd")``
     - ``s("bd sd")``
     - identical
   * - ``n("0 2 4")``
     - ``n("0 2 4")``
     - identical
   * - ``note("c4")``
     - ``note("c4")``
     - identical; for scale-degree use ``note(0.0)`` after ``scale(...)``
   * - ``.fast(2)``
     - ``|> fast(2.0)``
     - same name; pipe syntax preferred but method-chain also works
   * - ``.jux(rev)``
     - ``|> jux(@(x) => rev(x))``
     - daslang expects a typed fn; lambda literal is normal
   * - ``.velocity(0.5)`` / ``.vel(...)``
     - ``|> velocity(0.5)`` / ``|> vel(...)``
     - both aliases exist
   * - ``.stack(a, b)``
     - ``stack(a, b)``
     - top-level function, not a method
   * - ``.euclidRot(3, 8, 1)``
     - *not available*
     - use ``euclid(pat, 3, 8) |> off(1.0/8.0, @(x) => x)``
   * - ``"bd(3,8)"``
     - ``euclid(s("bd"), 3, 8)``
     - mini-notation Euclid form not parsed

For everything else, assume the name is identical.  Use the MCP
``list_module_api`` tool on ``strudel_pattern`` /
``strudel_synth`` / ``strudel_scales`` to confirm.


.. seealso::

   :ref:`stdlib_strudel_section` — generated reference for every
   strudel symbol.

   :ref:`tutorials_dastrudel` — 15-tutorial numbered series covering
   patterns, mini-notation, effects, synthesis, samples, MIDI, and
   live-reload.

   :ref:`tutorial_dastrudel_hello_pattern` — start here for a tour.
