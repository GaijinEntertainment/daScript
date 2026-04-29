# Copy-Pasting strudel.cc Patterns into daslang

Read this skill when the user has a pattern from strudel.cc — a line or
block of JavaScript like ``s("bd sd").fast(2).jux(rev)`` — and wants
to run it under daslang's strudel library. This is about **translating
user patterns**, not porting library features. For the library-side
"implement this primitive in daslang" workflow, the public API already
covers almost everything strudel.cc users rely on; use
:ref:`strudel_vs_strudel_cc` to check what's available.

## Where strudel.cc patterns come from

- The REPL at https://strudel.cc — everything on the front page is a
  pattern
- Example gallery: https://strudel.cc/workshop/
- Upstream source, for behavioural ground-truth:
  https://github.com/tidalcycles/strudel — use `WebFetch` on specific
  files rather than cloning

## The five mechanical rewrites

Doing these five in order turns almost every strudel.cc pattern into a
valid daslang expression. Apply them top-down on the pasted snippet.

### 1. Method chain → pipe chain

```
// strudel.cc
s("bd sd").fast(2).jux(rev).gain(0.8)

// daslang
s("bd sd") |> fast(2.0) |> jux(@(x) => rev(x)) |> gain(0.8)
```

Every ``.name(args)`` becomes ``|> name(args)``. Chains of four or
five stages are normal.

Note that daslang **does** support ``.method()`` as sugar for
``method(receiver, ...)`` — but only on **struct** types. ``Pattern``
is a ``lambda<...>`` typedef, not a struct, so ``s("bd").fast(2.0)``
fails to compile. Pipe (``|>``) is the correct form for strudel
chains, and it is faster to compile than method syntax anyway.

To break a long chain across lines, wrap it in ``(...)`` — daslang
permits newlines **anywhere inside** parens, square brackets, and
table-literal braces:

```das
let pat <- (
    s("bd sd hh cp")
    |> fast(2.0)
    |> jux(@(x) => rev(x))
    |> gain(0.8)
    |> room(0.3)
)
```

Without the surrounding parens the chain must be on one line
(statement-level parsing is line-oriented).

### 2. Numeric literals need a type

strudel.cc accepts bare integers everywhere; daslang is typed:

```
.fast(2)        →  |> fast(2.0)
.gain(0.5)      →  |> gain(0.5)     // already OK
.note(60)       →  |> note(60.0)
.delay(0.25)    →  |> delay(0.25)
.orbit(1)       →  |> orbit(1)      // orbit is int — OK
```

Rule: when in doubt, write the float with an explicit ``.0``. The
compiler error for ``int`` where ``float`` is expected is loud and
immediate.

### 3. Bare identifiers as transforms → lambda literals

strudel.cc passes **function names** to combinators; daslang wants a
typed lambda:

```
.jux(rev)              →  |> jux(@(x) => rev(x))
.off(1/8, fast(2))     →  |> off(0.125, @(x) => fast(x, 2.0))
.every(4, rev)         →  |> every(4, @(x) => rev(x))
```

The ``@(x) => ...`` form is an inline lambda. See
:ref:`tutorial_lambdas` for the syntax. If the transform itself is a
chain, build it inside the lambda body:

```
// strudel.cc
.jux(x => x.fast(2).rev())

// daslang
|> jux(@(x) => x |> fast(2.0) |> rev())
```

### 4. Top-level combinators aren't methods

``stack``, ``cat``, ``fastcat`` take patterns as **arguments**, not
``this``:

```
// strudel.cc
stack(s("bd"), s("hh*4"))

// daslang
stack([s("bd"), s("hh*4")])     // array of patterns — note the brackets
```

Same for ``cat``, ``fastcat``, ``randcat``, ``choose``, ``wchoose``.

### 5. Playback is explicit

strudel.cc auto-plays whatever the last line evaluates to. daslang
needs a harness:

```das
require strudel/strudel public
require audio/audio_boost
require daslib/fio

def play(var pat : Pattern; seconds : float; cps : double = 0.5lf) {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        strudel_add_track(pat)
        let t0 = ref_time_ticks()
        while (float(get_time_usec(t0)) / 1000000.0 < seconds) {
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

[export]
def main() {
    let pat <- s("bd sd") |> fast(2.0) |> jux(@(x) => rev(x))
    play(pat, 8.0)
}
```

Every tutorial under ``tutorials/daStrudel/`` uses this shape — copy
the harness from the closest one. For offline WAV rendering instead of
live audio, see ``examples/daStrudel/features/feature_common.das``
(``play_feature_cps`` with ``--wav PATH``).

## Samples, synths, and soundfonts

daslang ships with **built-in drum synthesis**. The following sample
names are synthesised out of the box — no sample pack needed:

``bd``, ``sd``, ``hh`` / ``hh_pedal``, ``oh`` / ``hh_open``, ``cp``,
``tom_low``, ``tom_high``, ``ride`` / ``ride_bell``, ``crash``,
``sidestick``, ``cowbell`` / ``cb``, ``tambourine`` / ``tamb``.

So ``s("bd sd hh cp")`` pasted from strudel.cc produces audio
immediately — an 808-style kick, a snare with wires, a noise-burst
hi-hat, a clap. The synth is "decent", not sample-accurate; if the
user wants the exact strudel.cc timbre, load the upstream sample
pack (option 1 below).

Dispatch rule in ``strudel_synth.render_event_stereo``: if a
``SampleBank`` has a sample with the event's name, use the sample;
otherwise if the name matches a built-in drum, synthesise it;
otherwise silence.

Three ways to add anything beyond the built-in drums, in order of
effort:

1. **Load a local sample bank.** Download
   https://github.com/tidalcycles/dirt-samples
   (or any pack you own), then before ``strudel_add_track``::

       strudel_load_sample_dir("/path/to/dirt-samples")

   Every top-level folder becomes a sound name; ``s("bd")`` will
   then play the sample (the loaded sample overrides the built-in
   synth).

2. **Use the oscillator voices.** For melodic material, the synth
   engine exposes named oscillators::

       s("sine sawtooth")                 // pure tones
       s("sine") |> note("c4 e4 g4")

   See :ref:`tutorial_dastrudel_synthesis` for the full list:
   ``sine``, ``sawtooth``, ``square``, ``triangle``, ``supersaw``,
   ``pink``, ``white``. **Note the exact names** — strudel.cc aliases
   ``sawtooth`` as ``saw``; daslang does not. ``s("saw")`` falls
   through to silence.

3. **Load a SoundFont.** If the strudel.cc pattern used a melodic
   instrument::

       strudel_load_sf2("/path/to/gm.sf2")
       ...
       s("sf2") |> note("c4 e4 g4") |> sf2_bank(0) |> sf2(40)   // violin

   See :ref:`tutorial_dastrudel_sf2_soundfont`.

## Orbit semantics (per-orbit FX)

In daslang, ``orbit`` selects a per-orbit effect bus with **independent
reverb / delay / chorus** instances. ``room`` sends to the orbit's
reverb, ``delay`` to its delay, ``chorus`` to its chorus. Voices on
different orbits don't share FX state — splitting orbits is how you
get separate reverb tails or chorus rates for the kick vs the lead.
``orbit(0)`` is the default. See ``examples/daStrudel/features/orbit_*.das``
for canonical patterns.

## HRTF position (daslang extension)

Per-event 3D positioning on top of stereo ``pan``:

```das
pat |> hrtf_azimuth(deg)       // numeric or pattern-valued
pat |> hrtf_elevation(deg)     // numeric or pattern-valued
pat |> hrtf(az, el)            // combined numeric setter
```

Setters flip an ``hrtf_active`` flag on the event; the scheduler
routes the dry signal and the orbit-FX sends through **two**
per-voice ``ma_hrtf`` instances (``hrtf_l`` + ``hrtf_r``) for
binaural-stereo HRTF: each input channel becomes a virtual source
at azimuth -/+ 30°, summed at the output. Pan and HRTF are
orthogonal — pan controls source width, HRTF controls position.
Best on headphones; cheaper to skip and use ``pan`` alone when
externalisation isn't needed. See
:ref:`tutorial_dastrudel_hrtf_position` and
``examples/daStrudel/hrtf/`` for the bumblebee demo.

## Common friction points when pasting

### Mini-notation ``bd(3,8)`` Euclidean form

strudel.cc parses the integer-pair inside the string as Euclidean
rhythm; daslang's mini-notation lexes the parens but does not parse
them. **Rewrite** as an explicit combinator call::

    // strudel.cc
    s("bd(3,8)")

    // daslang
    euclid(s("bd"), 3, 8)

### ``euclidRot``

Not implemented. Compose ``euclid`` with rotation via ``off`` or
``rev``. See :ref:`strudel_vs_strudel_cc` for the full gap list.

### Method aliases that don't exist

``jux_rev``, ``chop``, and a few one-letter convenience wrappers are
absent. Expand to the parametric form:

```
.jux_rev()   →  |> jux(@(x) => rev(x))
.chop(4)     →  |> striate(4)       // nearest equivalent
```

### Backtick-heavy mini-notation

strudel.cc mini-notation works identically for subdivisions ``[a b]``,
repeats ``a*4``, rests ``~``, angle-brackets ``<a b c>``, elongation
``@``, degrade ``?``, replicate ``!``. Paste them verbatim inside
``s("...")`` / ``n("...")`` / ``note("...")``.

### ADSR sounds different

daslang's ADSR defaults are tempo-aware; strudel.cc uses fixed-ms
values. If the pasted pattern sounds too soft/harsh at the attack or
release, set values explicitly::

    |> attack(0.01) |> decay(0.1) |> sustain(0.7) |> release(0.3)

Details in :ref:`strudel_vs_strudel_cc`.

### Pattern-valued setters

strudel.cc accepts a pattern as the argument to a setter::

    s("bd*4").gain("1 0.5")

daslang has pattern-valued overloads for every setter — the same
syntax works::

    s("bd*4") |> gain(s("1 0.5"))

If ``gain("1 0.5")`` doesn't compile, wrap the pattern literal in
``s(...)`` or the appropriate parser call.

## Verification loop

1. **Compile-check** — `bin/Release/daslang.exe -compile-only <file>.das`
   exits 0. Syntax and type errors surface here.
2. **Listen** — run the ported pattern for 8-16 seconds. Compare to a
   mental image of the strudel.cc playback (or load the strudel.cc
   REPL side-by-side).
3. **If it sounds wrong:** check the three most common culprits in
   order — (a) missing samples falling back to silence, (b)
   tempo-aware ADSR making the envelope feel different, (c) a
   combinator rewrite (``jux_rev``, ``chop``, ``euclidRot``) that
   silently did nothing because the rewrite was wrong.

## Quick reference card

.. list-table::
   :header-rows: 1
   :widths: 50 50

   * - strudel.cc
     - daslang
   * - ``s("bd").fast(2)``
     - ``s("bd") \|> fast(2.0)``
   * - ``.jux(rev)``
     - ``\|> jux(@(x) => rev(x))``
   * - ``.every(4, fast(2))``
     - ``\|> every(4, @(x) => fast(x, 2.0))``
   * - ``stack(a, b, c)``
     - ``stack([a, b, c])``
   * - ``"bd(3,8)"``
     - ``euclid(s("bd"), 3, 8)``
   * - ``.gain("1 0.5")``
     - ``\|> gain(s("1 0.5"))``
   * - ``.note(60)``
     - ``\|> note(60.0)``
   * - ``s("saw")``
     - ``s("sawtooth")`` — no ``saw`` alias
   * - auto-play
     - ``play(pat, seconds)`` harness + ``[export] def main``

