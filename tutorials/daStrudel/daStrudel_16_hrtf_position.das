// Tutorial DASTRUDEL-16: HRTF — 3D positional override for stereo pan
//
// daslang exposes a per-event HRTF position (azimuth, elevation). When set,
// HRTF positioning REPLACES the equal-power stereo pan path: the dry signal
// and the orbit-effect sends are run through the same CIPIC-based HRTF
// engine that audio_boost uses for 3D sources.
//
// Why bother — equal-power pan only places sources on a horizontal stereo
// line between the speakers. HRTF adds:
//   • depth / externalisation (sources feel "outside the head" on headphones),
//   • front-back disambiguation (centre-front is no longer ambiguous with
//     centre-rear),
//   • elevation cues.
//
// Cost — under the binaural-stereo render, each HRTF-positioned voice carries
// two ma_hrtf instances (hrtf_l + hrtf_r, lazy-allocated on first render: one
// per virtual-speaker channel at azimuth -/+ 30°). Practical for 3-8
// simultaneous HRTF voices on modern hardware. Plain pan is far cheaper;
// reach for HRTF only when you want spatial cues plain pan can't provide.
//
// API:
//   pat |> hrtf_azimuth(deg)        -- numeric or pattern-valued
//   pat |> hrtf_elevation(deg)      -- numeric or pattern-valued
//   pat |> hrtf(az, el)             -- combined numeric setter
//
// Setting any of the three flips the event's `hrtf_active = true`. Pan
// and HRTF are orthogonal: pan controls the inherent stereo width / image
// of the source (L/R balance feeding the spatialiser), HRTF positions the
// source in 3D.
//
// Run: daslang.exe tutorials/daStrudel/daStrudel_16_hrtf_position.das

options gen2
options persistent_heap
options indenting = 4

require strudel/strudel public
require strudel/strudel_player public
require audio/audio_boost
require daslib/fio

def play(var pat : Pattern; cps : double = 0.5lf; seconds : double = 4.0lf) {
    with_audio_system() {
        strudel_create_channel()
        strudel_set_cps(cps)
        strudel_add_track(pat)
        let t0 = ref_time_ticks()
        while (double(get_time_usec(t0)) / 1000000.0lf < seconds) {
            strudel_tick()
            sleep(5u)
        }
        strudel_shutdown()
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Static positions: ahead, left, right, above
// ──────────────────────────────────────────────────────────────────────────
//
// hrtf(az, el) takes both axes at once. 0/0 = directly ahead, +90/0 = right,
// -90/0 = left, 0/+45 = up-front. Try headphones for the strongest effect.

def example_static_positions() {
    print("\n=== Section 1: four static HRTF positions ===\n")
    let pat <- stack([
        note("c4", "sine") |> hrtf(0.0,  0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("e4", "sine") |> hrtf(-90.0,  0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("g4", "sine") |> hrtf(+90.0,  0.0) |> attack(0.01) |> release(0.5) |> gain(0.4),
        note("c5", "sine") |> hrtf(0.0, 45.0) |> attack(0.01) |> release(0.5) |> gain(0.4)
    ])
    play(pat, 0.5lf, 6.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Animated azimuth: sine sweep
// ──────────────────────────────────────────────────────────────────────────
//
// The setters accept patterns. Each event samples the modulation pattern
// at its onset to get its azimuth. With sine().range(-180, 180).slow(4),
// the source orbits the listener once every 8 cycles (slow(4) on a 0.5cps
// stream).

def example_animated_azimuth() {
    print("\n=== Section 2: azimuth follows a sine sweep ===\n")
    let pat <- (
        note("c4 e4 g4 c5", "triangle")
        |> hrtf_azimuth(sine() |> range(-180.0, 180.0) |> slow(4.0lf))
        |> attack(0.005)
        |> release(0.3)
        |> gain(0.4)
    )
    play(pat, 0.5lf, 8.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Pan and HRTF combine — HRTF positions, pan widens
// ──────────────────────────────────────────────────────────────────────────
//
// When hrtf_active is true the equal-power L/R mixer is replaced by a
// binaural-stereo HRTF: L becomes a virtual source at (azimuth - 30°) and
// R at (azimuth + 30°), each through its own HRTF instance. The pre-pan
// stereo image is preserved as source width; HRTF places the source in 3D.
// The same binaural render is reused for the reverb/delay/chorus sends.
//
// Below, the second voice has BOTH pan(0.5) and hrtf(-90, 0). HRTF places
// the voice to the left; pan(0.5)'s slight L/R imbalance is preserved as
// source width inside that 3D position.

def example_hrtf_combines_with_pan() {
    print("\n=== Section 3: hrtf places, pan widens ===\n")
    let pat <- stack([
        note("c4", "sine") |> pan(0.5)                       |> attack(0.01) |> release(0.4) |> gain(0.4),
        note("e4", "sine") |> pan(0.5) |> hrtf(-90.0, 0.0)   |> attack(0.01) |> release(0.4) |> gain(0.4)
    ])
    play(pat, 0.5lf, 4.0lf)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — When NOT to use HRTF
// ──────────────────────────────────────────────────────────────────────────
//
// HRTF is heavier than pan and works best on headphones. For drum machines
// or beats where you just want left/right separation, plain pan is cheaper
// and works fine on speakers. Mix-and-match in the same stack: HRTF the
// "lead voice" and pan the rhythm section.
//
// In this example the kick/snare go through plain pan; the lead pluck
// gets HRTF positioning. Because the pluck is on a different orbit, its
// reverb stays distinct from the (dry) drum bus.

def example_mixed() {
    print("\n=== Section 4: mixing HRTF and plain pan in one stack ===\n")
    let pat <- stack([
        s("bd ~ sd ~")                  |> orbit(0) |> gain(0.6),
        s("hh*8")                       |> orbit(0) |> pan(0.7) |> gain(0.3),
        note("c4 e4 g4 e4", "triangle") |> orbit(1) |> hrtf_azimuth(sine() |> range(-60.0, 60.0) |> slow(2.0lf))
                                         |> room(0.4) |> roomsize(2.0) |> attack(0.005) |> release(0.25) |> gain(0.35)
    ])
    play(pat, 0.5lf, 8.0lf)
}

[export]
def main() {
    print("DASTRUDEL-16: HRTF — 3D positional override for stereo pan\n")
    print("Best heard on headphones.\n")
    example_static_positions()
    example_animated_azimuth()
    example_hrtf_combines_with_pan()
    example_mixed()
    print("\nDone.\n")
}
