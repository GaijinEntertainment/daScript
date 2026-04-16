options gen2
options indenting = 4
options persistent_heap

// SF2 ambient piece — 8 layers using SoundFont instruments for natural, organic textures.
// Inspired by integration_full_ambient.das but using SF2 for richer timbres.
// Arrhythmic, slow harmonic drift, large reverb spaces, modal harmony in C minor.
// SF2 is daslang-only — no strudel.cc equivalent.

require feature_common
require daslib/fio

def find_sf2() : string {
    let dir = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"
    var st : FStat
    // prefer larger soundfonts — more samples, better quality
    let stolen = "{dir}/Stolen Soundfont v2.08.2 (rev.3).sf2"
    if (stat(stolen, st)) {
        return stolen
    }
    let musyng = "{dir}/Musyng Kite.sf2"
    if (stat(musyng, st)) {
        return musyng
    }
    let fluid = "{dir}/FluidR3_GM.sf2"
    if (stat(fluid, st)) {
        return fluid
    }
    let generaluser = "{dir}/GeneralUser GS v1.471.sf2"
    if (stat(generaluser, st)) {
        return generaluser
    }
    return ""
}

[export]
def main() {
    // Layer 1: Deep cello drone — slow root movement, huge reverb tail
    var layer1 <- note_pattern("<c2 g2 f2 eb2 bb1 f2>") |> set_sf2("cello") |> slow(32.0lf) |> set_gain(0.15) |> set_sf2_expression(0.6) |> set_room(0.8) |> set_roomsize(10.0) |> set_orbit(1)

    // Layer 2: String ensemble pad — warm sustained chords, gentle sway
    let pan2 <- signal_sine() |> signal_range(0.3, 0.7) |> slow(20.0lf)
    var layer2 <- note_pattern("<[c3,eb3,g3] [eb3,g3,bb3] [f3,ab3,c4] [bb2,eb3,g3] [ab2,c3,eb3] [g2,bb2,eb3]>") |> set_sf2("string_ensemble") |> slow(24.0lf) |> set_gain(0.25) |> set_sf2_expression(0.5) |> set_sf2_mod_wheel(0.3) |> set_room(0.7) |> set_roomsize(8.0) |> set_pan(pan2) |> set_orbit(2)

    // Layer 3: Choir — ethereal texture, very slow, drenched in reverb+delay
    var layer3 <- note_pattern("<[g3,c4,eb4] [f3,bb3,eb4] [ab3,c4,f4] [g3,bb3,eb4]>") |> set_sf2("choir") |> slow(28.0lf) |> set_gain(0.15) |> set_sf2_expression(0.4) |> set_sf2_mod_wheel(0.5) |> set_room(0.9) |> set_roomsize(10.0) |> set_delay(0.25) |> set_delaytime(0.8) |> set_delayfeedback(0.4) |> set_orbit(3)

    // Layer 4: Vibraphone arpeggios — delicate bell-like bubbles
    let gain4 <- signal_perlin() |> signal_range(0.08, 0.25) |> slow(10.0lf)
    let pan4 <- signal_sine() |> signal_range(0.15, 0.85) |> slow(7.0lf)
    var layer4 <- note_pattern("c5 ~ ~ eb5 ~ g4 ~ ~ bb4 ~ f5 ~") |> set_sf2("vibraphone") |> slow(6.0lf) |> set_gain(gain4) |> set_pan(pan4) |> set_room(0.7) |> set_roomsize(6.0) |> set_delay(0.4) |> set_delaytime(0.5) |> set_delayfeedback(0.55) |> set_orbit(4)

    // Layer 5: Flute — high, sparse, breathy melody floating above everything
    let gain5 <- signal_perlin() |> signal_range(0.05, 0.18) |> slow(14.0lf)
    let pan5 <- signal_sine() |> signal_range(0.1, 0.9) |> slow(11.0lf)
    var layer5 <- note_pattern("~ g5 ~ ~ c5 ~ ~ ~ eb5 ~ ~ bb5") |> set_sf2("flute") |> slow(8.0lf) |> set_gain(gain5) |> set_sf2_mod_wheel(0.6) |> set_pan(pan5) |> set_room(0.8) |> set_roomsize(9.0) |> set_delay(0.3) |> set_delaytime(0.75) |> set_delayfeedback(0.45) |> set_orbit(4)

    // Layer 6: Harp — occasional gentle plucks, wide stereo, adding sparkle
    let gain6 <- signal_perlin() |> signal_range(0.06, 0.2) |> slow(12.0lf)
    let pan6 <- signal_sine() |> signal_range(0.1, 0.9) |> slow(9.0lf)
    var layer6 <- note_pattern("~ ~ c4 ~ ~ ~ g4 ~ ~ eb4 ~ ~") |> set_sf2("harp") |> slow(10.0lf) |> set_gain(gain6) |> set_pan(pan6) |> set_room(0.6) |> set_roomsize(5.0) |> set_delay(0.35) |> set_delaytime(0.6) |> set_delayfeedback(0.5) |> set_orbit(5)

    // Layer 7: Contrabass — sub-harmonic anchor, very slow, barely there
    let gain7 <- signal_sine() |> signal_range(0.05, 0.2) |> slow(16.0lf)
    var layer7 <- note_pattern("<c1 c1 f1 f1>") |> set_sf2("contrabass") |> slow(48.0lf) |> set_gain(gain7) |> set_sf2_expression(0.4) |> set_room(0.5) |> set_roomsize(6.0) |> set_orbit(1)

    // Layer 8: Heartbeat — slow kick + soft rimshot, distant and hypnotic
    let gain8 <- signal_sine() |> signal_range(0.06, 0.15) |> slow(20.0lf)
    var layer8 <- note_pattern("c2 ~ ~ ~ ~ ~ cs2 ~ ~ ~ ~ ~") |> set_sf2(0) |> set_sf2_bank(128) |> slow(4.0lf) |> set_gain(gain8) |> set_room(0.5) |> set_roomsize(4.0) |> set_orbit(5)

    let pat <- stack([layer1, layer2, layer3, layer4, layer5, layer6, layer7, layer8])

    default_feature_duration = 60.0  // override default duration for this feature
    play_feature_cps(pat, 15.0lf / 60.0lf) <| $ {
        strudel_load_sf2(find_sf2())
    }
}
