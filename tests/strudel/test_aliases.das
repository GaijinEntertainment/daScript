options gen2
options indenting = 4

require dastest/testing_boost
require strudel/strudel
require math

// ─── Bare-name setter aliases (scalar) ───

[test(description = "the gain alias sets the event gain")]
def test_gain_alias(t : T?) {
    let pat <- atom("bd") |> gain(0.5)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> success(abs(haps[0].value.gain - 0.5) < 0.001)
}

[test(description = "the velocity alias sets the event velocity")]
def test_velocity_alias(t : T?) {
    let pat <- atom("bd") |> velocity(0.7)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.velocity - 0.7) < 0.001)
}

[test(description = "vel is a short alias for velocity")]
def test_vel_short_alias(t : T?) {
    let pat <- atom("bd") |> vel(0.3)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.velocity - 0.3) < 0.001)
}

[test(description = "the pan alias sets the event pan")]
def test_pan_alias(t : T?) {
    let pat <- atom("bd") |> pan(0.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.pan - 0.0) < 0.001)
}

[test(description = "the speed alias sets the event speed")]
def test_speed_alias(t : T?) {
    let pat <- atom("bd") |> speed(2.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.speed - 2.0) < 0.001)
}

[test(description = "note with a float sets the event note number")]
def test_note_float_alias(t : T?) {
    let pat <- atom("sine") |> note(72.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.note - 72.0) < 0.001)
}

[test(description = "sound sets the event sound name")]
def test_sound_alias(t : T?) {
    let pat <- atom("bd") |> sound("sawtooth")
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.s, "sawtooth")
}

[test(description = "cut sets the event cut group")]
def test_cut_alias(t : T?) {
    let pat <- atom("bd") |> cut(3)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.cut, 3)
}

[test(description = "room sets the event reverb room amount")]
def test_room_alias(t : T?) {
    let pat <- atom("bd") |> room(0.5)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.room - 0.5) < 0.001)
}

[test(description = "roomsize sets the event reverb room size")]
def test_roomsize_alias(t : T?) {
    let pat <- atom("bd") |> roomsize(3.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.roomsize - 3.0) < 0.001)
}

[test(description = "size is a short alias for roomsize")]
def test_size_short_alias(t : T?) {
    let pat <- atom("bd") |> size(4.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.roomsize - 4.0) < 0.001)
}

[test(description = "delay sets the event delay amount")]
def test_delay_alias(t : T?) {
    let pat <- atom("bd") |> delay(0.8)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.delay_amount - 0.8) < 0.001)
}

[test(description = "delaytime sets the event delay time")]
def test_delaytime_alias(t : T?) {
    let pat <- atom("bd") |> delaytime(0.25)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.delaytime - 0.25) < 0.001)
}

[test(description = "dt is a short alias for delaytime")]
def test_dt_short_alias(t : T?) {
    let pat <- atom("bd") |> dt(0.125)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.delaytime - 0.125) < 0.001)
}

[test(description = "delayfeedback sets the event delay feedback")]
def test_delayfeedback_alias(t : T?) {
    let pat <- atom("bd") |> delayfeedback(0.6)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.delayfeedback - 0.6) < 0.001)
}

[test(description = "dfb is a short alias for delayfeedback")]
def test_dfb_short_alias(t : T?) {
    let pat <- atom("bd") |> dfb(0.9)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.delayfeedback - 0.9) < 0.001)
}

[test(description = "orbit sets the event orbit index")]
def test_orbit_alias(t : T?) {
    let pat <- atom("bd") |> orbit(2)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.orbit, 2)
}

[test(description = "fm sets the event frequency-modulation depth")]
def test_fm_alias(t : T?) {
    let pat <- atom("sine") |> fm(3.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.fm - 3.0) < 0.001)
}

[test(description = "fmh sets the event frequency-modulation harmonic ratio")]
def test_fmh_alias(t : T?) {
    let pat <- atom("sine") |> fmh(2.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.fmh - 2.0) < 0.001)
}

[test(description = "lpf sets the event low-pass cutoff")]
def test_lpf_alias(t : T?) {
    let pat <- atom("bd") |> lpf(2000.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.lpf - 2000.0) < 0.001)
}

[test(description = "lp is a short alias for lpf")]
def test_lp_short_alias(t : T?) {
    let pat <- atom("bd") |> lp(1500.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.lpf - 1500.0) < 0.001)
}

[test(description = "hpf sets the event high-pass cutoff")]
def test_hpf_alias(t : T?) {
    let pat <- atom("bd") |> hpf(300.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.hpf - 300.0) < 0.001)
}

[test(description = "hp is a short alias for hpf")]
def test_hp_short_alias(t : T?) {
    let pat <- atom("bd") |> hp(500.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.hpf - 500.0) < 0.001)
}

[test(description = "lpq sets the event low-pass resonance")]
def test_lpq_alias(t : T?) {
    let pat <- atom("bd") |> lpq(5.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.lpq - 5.0) < 0.001)
}

[test(description = "resonance is an alias for lpq")]
def test_resonance_alias(t : T?) {
    let pat <- atom("bd") |> resonance(8.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.lpq - 8.0) < 0.001)
}

[test(description = "hpq sets the event high-pass resonance")]
def test_hpq_alias(t : T?) {
    let pat <- atom("bd") |> hpq(3.0)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.hpq - 3.0) < 0.001)
}

[test(description = "attack sets the event envelope attack")]
def test_attack_alias(t : T?) {
    let pat <- atom("sine") |> attack(0.1)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.attack - 0.1) < 0.001)
}

[test(description = "att is a short alias for attack")]
def test_att_short_alias(t : T?) {
    let pat <- atom("sine") |> att(0.2)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.attack - 0.2) < 0.001)
}

[test(description = "decay sets the event envelope decay")]
def test_decay_alias(t : T?) {
    let pat <- atom("sine") |> decay(0.3)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.decay - 0.3) < 0.001)
}

[test(description = "dec is a short alias for decay")]
def test_dec_short_alias(t : T?) {
    let pat <- atom("sine") |> dec(0.4)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.decay - 0.4) < 0.001)
}

[test(description = "sustain sets the event envelope sustain")]
def test_sustain_alias(t : T?) {
    let pat <- atom("sine") |> sustain(0.7)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.sustain - 0.7) < 0.001)
}

[test(description = "sus is a short alias for sustain")]
def test_sus_short_alias(t : T?) {
    let pat <- atom("sine") |> sus(0.8)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.sustain - 0.8) < 0.001)
}

[test(description = "release sets the event envelope release")]
def test_release_alias(t : T?) {
    let pat <- atom("sine") |> release(0.5)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.release - 0.5) < 0.001)
}

[test(description = "rel is a short alias for release")]
def test_rel_short_alias(t : T?) {
    let pat <- atom("sine") |> rel(0.6)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.release - 0.6) < 0.001)
}

[test(description = "chorus sets the event chorus send")]
def test_chorus_alias(t : T?) {
    let pat <- atom("sine") |> chorus(0.4)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.chorus - 0.4) < 0.001)
}

// ─── SF2 aliases ───

[test(description = "sf2 with a number sets the SoundFont program")]
def test_sf2_program_alias(t : T?) {
    let pat <- atom("bd") |> sf2(42)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.sf2_program, 42)
}

[test(description = "sf2 with an instrument name resolves to program zero for piano")]
def test_sf2_name_alias(t : T?) {
    let pat <- atom("bd") |> sf2("piano")
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.sf2_program, 0)
}

[test(description = "sf2_bank sets the SoundFont bank")]
def test_sf2_bank_alias(t : T?) {
    let pat <- atom("bd") |> sf2_bank(128)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(haps[0].value.sf2_bank, 128)
}

[test(description = "sf2_expression sets the SoundFont expression level")]
def test_sf2_expression_alias(t : T?) {
    let pat <- atom("bd") |> sf2_expression(0.9)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.sf2_expression - 0.9) < 0.001)
}

// ─── Pattern-modulated aliases ───

[test(description = "gain accepts a signal pattern and samples it at the event time")]
def test_gain_pattern_alias(t : T?) {
    let pat <- atom("bd") |> gain(sine())
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    // gain should be ~0.5 at t=0 (sine starts at midpoint)
    t |> success(abs(haps[0].value.gain - 0.5) < 0.1)
}

[test(description = "lpf accepts a ranged signal pattern and samples it at the event time")]
def test_lpf_pattern_alias(t : T?) {
    let pat <- atom("sawtooth") |> lpf(sine() |> range(200.0, 4000.0))
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    // At t=0, sine=0.5, mapped to 200+0.5*3800=2100
    t |> success(abs(haps[0].value.lpf - 2100.0) < 100.0)
}

// ─── Signal aliases ───

[test(description = "sine() peaks at 1.0 a quarter of the way through the cycle")]
def test_sine_alias(t : T?) {
    let pat <- sine()
    let haps <- pat(TimeSpan(start = 0.25lf, stop = 0.251lf))
    t |> success(abs(haps[0].value.note - 1.0) < 0.01)
}

[test(description = "saw() reads 0.5 half-way through the cycle")]
def test_saw_alias(t : T?) {
    let pat <- saw()
    let haps <- pat(TimeSpan(start = 0.5lf, stop = 0.501lf))
    t |> success(abs(haps[0].value.note - 0.5) < 0.01)
}

[test(description = "tri() peaks at 1.0 half-way through the cycle")]
def test_tri_alias(t : T?) {
    let pat <- tri()
    let haps <- pat(TimeSpan(start = 0.5lf, stop = 0.501lf))
    t |> success(abs(haps[0].value.note - 1.0) < 0.01)
}

[test(description = "cosine() reads 1.0 at the start of the cycle")]
def test_cosine_alias(t : T?) {
    let pat <- cosine()
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 0.001lf))
    t |> success(abs(haps[0].value.note - 1.0) < 0.01)
}

[test(description = "perlin() yields one value within 0 to 1")]
def test_perlin_alias(t : T?) {
    let pat <- perlin()
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 0.001lf))
    t |> equal(length(haps), 1)
    t |> success(haps[0].value.note >= 0.0 && haps[0].value.note <= 1.0)
}

[test(description = "range maps a unit signal onto the given interval")]
def test_range_alias(t : T?) {
    let pat <- saw() |> range(100.0, 200.0)
    let haps <- pat(TimeSpan(start = 0.5lf, stop = 0.501lf))
    // saw at 0.5 = 0.5, mapped to 100 + 0.5*100 = 150
    t |> success(abs(haps[0].value.note - 150.0) < 1.0)
}

// ─── note() mini-notation alias ───

[test(description = "note with a mini-notation string yields one event per note with MIDI note numbers and the default sine sound")]
def test_note_string_alias(t : T?) {
    let pat <- note("c4 e4 g4")
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 3)
    t |> success(abs(haps[0].value.note - 60.0) < 0.01)  // C4 = 60
    t |> success(abs(haps[1].value.note - 64.0) < 0.01)  // E4 = 64
    t |> success(abs(haps[2].value.note - 67.0) < 0.01)  // G4 = 67
    // default sound is "sine"
    t |> equal(haps[0].value.s, "sine")
}

// ─── scale() alias ───

[test(description = "scale maps scale degrees of a number pattern onto notes of the named scale")]
def test_scale_alias(t : T?) {
    let pat <- n("0 2 4") |> scale("C4:major")
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 3)
    t |> success(abs(haps[0].value.note - 60.0) < 0.01)  // C4
    t |> success(abs(haps[1].value.note - 64.0) < 0.01)  // E4
    t |> success(abs(haps[2].value.note - 67.0) < 0.01)  // G4
}

// ─── degradeBy alias ───

[test(description = "degradeBy never adds events and keeps at most the original count")]
def test_degradeBy_alias(t : T?) {
    let pat <- s("bd sd hh cp") |> degradeBy(0.5)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    // With 50% degradation, should have fewer than 4 events (probabilistic but deterministic)
    t |> success(length(haps) <= 4)
    t |> success(length(haps) >= 0)
}

// ─── Chain multiple bare-name aliases (integration test) ───

[test(description = "bare-name aliases chain, each setting its own field on the same event")]
def test_chain_bare_names(t : T?) {
    // Chained bare-name aliases: note("c4").s("sawtooth").gain(0.5).lpf(2000).attack(0.01).room(0.3)
    let pat <- note("c4", "sawtooth") |> gain(0.5) |> lpf(2000.0) |> att(0.01) |> room(0.3)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> equal(length(haps), 1)
    t |> equal(haps[0].value.s, "sawtooth")
    t |> success(abs(haps[0].value.note - 60.0) < 0.01)
    t |> success(abs(haps[0].value.gain - 0.5) < 0.001)
    t |> success(abs(haps[0].value.lpf - 2000.0) < 0.001)
    t |> success(abs(haps[0].value.attack - 0.01) < 0.001)
    t |> success(abs(haps[0].value.room - 0.3) < 0.001)
}

[test(description = "begin sets the sample start position")]
def test_begin_alias(t : T?) {
    let pat <- atom("bd") |> begin(0.25)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.begin - 0.25) < 0.001)
}

[test(description = "end_pos sets the sample end position")]
def test_end_pos_alias(t : T?) {
    let pat <- atom("bd") |> end_pos(0.75)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.end_pos - 0.75) < 0.001)
}

[test(description = "begin and end_pos chain without overwriting each other")]
def test_begin_end_chain(t : T?) {
    let pat <- atom("bd") |> begin(0.1) |> end_pos(0.6)
    let haps <- pat(TimeSpan(start = 0.0lf, stop = 1.0lf))
    t |> success(abs(haps[0].value.begin - 0.1) < 0.001)
    t |> success(abs(haps[0].value.end_pos - 0.6) < 0.001)
}
