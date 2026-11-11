options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require strudel/strudel_sf2
require dastest/testing_boost
require daslib/fio
require math
require strudel/strudel_sf2_voice
require audio

let SF2_DIR = "{get_das_root()}/examples/daStrudel/midi_sf2_demo"


[test(description = "timecents convert to seconds, with -32768 meaning instantaneous")]
def test_timecent_conversion(t : T?) {
    t |> run("zero timecents = 1 second") <| @(t : T?) {
        t |> equal(1.0, timecents_to_seconds(0))
    }
    t |> run("1200 timecents = 2 seconds") <| @(t : T?) {
        t |> roughly_equal(2.0, timecents_to_seconds(1200), 0.001)
    }
    t |> run("-1200 timecents = 0.5 seconds") <| @(t : T?) {
        t |> roughly_equal(0.5, timecents_to_seconds(-1200), 0.001)
    }
    t |> run("-32768 timecents = instantaneous") <| @(t : T?) {
        t |> equal(0.0, timecents_to_seconds(-32768))
    }
}

[test(description = "cents convert to Hz with 6900 cents as 440 Hz")]
def test_cents_to_hz(t : T?) {
    t |> run("6900 cents = 440 Hz (A4)") <| @(t : T?) {
        t |> roughly_equal(440.0, cents_to_hz(6900), 1.0)
    }
    t |> run("0 cents = 8.176 Hz") <| @(t : T?) {
        t |> roughly_equal(8.176, cents_to_hz(0), 0.01)
    }
}

[test(description = "centibels convert to a linear gain")]
def test_centibels_to_linear(t : T?) {
    t |> run("0 cb = 1.0") <| @(t : T?) {
        t |> equal(1.0, centibels_to_linear(0))
    }
    t |> run("200 cb ~ 0.1") <| @(t : T?) {
        t |> roughly_equal(0.1, centibels_to_linear(200), 0.001)
    }
}

[test(description = "sf2_load returns null for an empty or missing path")]
def test_load_empty_path(t : T?) {
    t |> run("sf2_load(\"\") returns null instead of panicking in fopen") <| @(t : T?) {
        // a missing-soundfont probe hands "" here; fopen("") is a panic, so the guard must catch it first
        t |> success(sf2_load("") == null)
    }
    t |> run("sf2_load on a missing file returns null") <| @(t : T?) {
        t |> success(sf2_load("{get_das_root()}/tests/strudel/fixtures/does_not_exist.sf2") == null)
    }
}

[test(description = "a compressed SF3 file loads, its OGG streams decode to PCM and its headers are rewritten in bounds")]
def test_load_sf3_compressed(t : T?) {
    // mini.sf3: a hand-built minimal SF3 holding one OGG-Vorbis sample (a 0.06s 440Hz
    // mono sine, 22050Hz). Exercises the SF3 decode path deterministically — no external asset.
    let path = "{get_das_root()}/tests/strudel/fixtures/mini.sf3"
    var sf2 = sf2_load(path)
    t |> run("compressed SF3 loads (not rejected)") <| @(t : T?) {
        t |> success(sf2 != null)
    }
    if (sf2 == null) return
    t |> run("OGG stream decoded to PCM frames") <| @(t : T?) {
        // 3553-byte OGG decodes to ~1323 frames; just assert a plausible decoded length
        t |> success(length(sf2.sample_data) > 1000)
    }
    t |> run("sample header rewritten in-bounds") <| @(t : T?) {
        let n = length(sf2.sample_data)
        let s = sf2.samples[0]
        t |> success(int(s.end) <= n && int(s.start) < int(s.end))
    }
    t |> run("OGG-Vorbis flag stripped after decode") <| @(t : T?) {
        t |> success((sf2.samples[0].sample_type & SF2_SAMPLE_OGG_VORBIS) == 0u)
    }
    t |> run("preset resolves to a playable zone") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        t |> success(idx >= 0)
        if (idx >= 0) {
            let zones = sf2_find_zones(*sf2, idx, 60, 127)
            t |> success(!empty(zones))
        }
    }
    unsafe { delete sf2; }
}

[test(description = "FluidR3 loads with presets, instruments, samples and sample data and resolves piano, guitar and the drum bank")]
def test_load_fluidr3(t : T?) {
    var st : FStat
    if (!stat("{SF2_DIR}/FluidR3_GM.sf2", st)) {
        t->skip("FluidR3_GM.sf2 not available")
        return
    }
    var sf2 = sf2_load("{SF2_DIR}/FluidR3_GM.sf2")
    if (sf2 == null) {
        t->skip("FluidR3_GM.sf2 not available")
        return
    }
    t |> run("has presets") <| @(t : T?) {
        t |> success(length(sf2.presets) >= 128)
    }
    t |> run("has instruments") <| @(t : T?) {
        t |> success(!empty(sf2.instruments))
    }
    t |> run("has samples") <| @(t : T?) {
        t |> success(!empty(sf2.samples))
    }
    t |> run("has sample data") <| @(t : T?) {
        t |> success(length(sf2.sample_data) > 1000000)
    }
    t |> run("preset 0 is piano") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        t |> success(idx >= 0)
        if (idx >= 0) {
            let name = sf2.presets[idx].name
            t |> success(!empty(name))
        }
    }
    t |> run("preset lookup for guitar (bank 0 program 25)") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 25)
        t |> success(idx >= 0)
    }
    t |> run("drum bank 128 exists") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 128, 0)
        t |> success(idx >= 0)
    }
    t |> run("zone lookup for piano C4") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        if (idx >= 0) {
            let zones = sf2_find_zones(*sf2, idx, 60, 127)
            t |> success(!empty(zones))
        }
    }
    t |> run("zone lookup for drum kick") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 128, 0)
        if (idx >= 0) {
            let zones = sf2_find_zones(*sf2, idx, 36, 127)
            t |> success(!empty(zones))
        }
    }
    t |> run("sample headers have valid data") <| @(t : T?) {
        if (!empty(sf2.samples)) {
            let s = sf2.samples[0]
            t |> success(s.sample_rate > 0u)
            t |> success(s.end > s.start)
            t |> success(!empty(s.name))
        }
    }
    unsafe { delete sf2; }
}

def roughly_equal(t : T?; expected, actual, tolerance : float; msg : string = "") {
    let diff = abs(expected - actual)
    if (diff > tolerance) {
        t |> failure("{msg}expected {expected} +/- {tolerance}, got {actual} (diff={diff})")
    } else {
        t |> success(true)
    }
}

// ─── Envelope Tests ───

[test(description = "the SF2 volume envelope goes through delay, attack, decay, sustain and release")]
def test_envelope_stages(t : T?) {   // nolint:STYLE038 flat list of independent envelope-stage arms
    t |> run("delay then attack") <| @(t : T?) {
        var env = SF2Envelope()
        env.delay_sec = 0.001  // ~48 samples at 48kHz
        env.attack_sec = 0.01
        env.hold_sec = 0.0
        env.decay_sec = 0.1
        env.sustain_level = 0.5
        env.release_sec = 0.1
        env.is_amp_env = true
        env.midi_velocity = 127
        sf2_env_start(env, 48000.0)
        // tick through delay
        var level = 0.0
        for (_ in range(100)) {
            level = sf2_env_tick(env, false, 48000.0)
        }
        // should be in attack, level > 0
        t |> success(level > 0.0)
        t |> success(env.stage == SF2EnvStage.env_attack || env.stage == SF2EnvStage.env_hold || env.stage == SF2EnvStage.env_decay)
    }
    t |> run("attack reaches 1.0") <| @(t : T?) {
        var env = SF2Envelope()
        env.delay_sec = 0.0
        env.attack_sec = 0.01
        env.hold_sec = 0.0
        env.decay_sec = 1.0
        env.sustain_level = 0.5
        env.release_sec = 0.1
        env.is_amp_env = false
        sf2_env_start(env, 48000.0)
        var level = 0.0
        for (_ in range(1000)) {
            level = sf2_env_tick(env, false, 48000.0)
        }
        // should have passed through attack, now in decay
        t |> success(env.stage == SF2EnvStage.env_decay || env.stage == SF2EnvStage.env_sustain)
    }
    t |> run("decay reaches sustain level") <| @(t : T?) {
        var env = SF2Envelope()
        env.delay_sec = 0.0
        env.attack_sec = 0.001
        env.hold_sec = 0.0
        env.decay_sec = 0.01
        env.sustain_level = 0.3
        env.release_sec = 0.1
        env.is_amp_env = false
        sf2_env_start(env, 48000.0)
        var level = 0.0
        for (_ in range(2000)) {
            level = sf2_env_tick(env, false, 48000.0)
        }
        t |> roughly_equal(0.3, level, 0.05, "sustain: ")
        t |> success(env.stage == SF2EnvStage.env_sustain)
    }
    t |> run("release drops to zero") <| @(t : T?) {
        var env = SF2Envelope()
        env.delay_sec = 0.0
        env.attack_sec = 0.001
        env.hold_sec = 0.0
        env.decay_sec = 0.001
        env.sustain_level = 1.0
        env.release_sec = 0.01
        env.is_amp_env = true
        env.midi_velocity = 127
        sf2_env_start(env, 48000.0)
        // reach sustain
        for (_ in range(500)) {
            sf2_env_tick(env, false, 48000.0)
        }
        // release
        for (_ in range(2000)) {
            sf2_env_tick(env, true, 48000.0)
        }
        t |> success(env.stage == SF2EnvStage.env_finished)
        t |> success(env.level < 0.001)
    }
    t |> run("instantaneous delay (-32768 timecents)") <| @(t : T?) {
        var env = SF2Envelope()
        env.delay_sec = timecents_to_seconds(-32768)  // 0.0
        env.attack_sec = 0.001
        sf2_env_start(env, 48000.0)
        // should skip delay immediately
        t |> success(env.stage == SF2EnvStage.env_attack)
    }
}

// ─── LFO Tests ───

[test(description = "the SF2 LFO is silent during its delay and oscillates afterwards")]
def test_lfo(t : T?) {
    t |> run("LFO outputs zero during delay") <| @(t : T?) {
        var lfo = SF2LFO()
        lfo.delay_sec = 1.0
        lfo.freq = 5.0
        let val = sf2_lfo_tick(lfo, 0.01)
        t |> equal(0.0, val)
    }
    t |> run("LFO oscillates after delay") <| @(t : T?) {
        var lfo = SF2LFO()
        lfo.delay_sec = 0.0
        lfo.freq = 10.0
        // tick for a while and check we get values in [-1, 1]
        var min_val = 1.0
        var max_val = -1.0
        for (_ in range(4800)) {  // 0.1 seconds = 1 full cycle at 10Hz
            let val = sf2_lfo_tick(lfo, 1.0 / 48000.0)
            if (val < min_val) { min_val = val; }
            if (val > max_val) { max_val = val; }
        }
        t |> success(min_val < -0.5)
        t |> success(max_val > 0.5)
    }
}

// ─── Voice Creation Tests ───

[test(description = "a voice at the root key plays at unit phase increment and an octave up doubles it")]
def test_voice_pitch(t : T?) {
    var st : FStat
    if (!stat("{SF2_DIR}/FluidR3_GM.sf2", st)) {
        t->skip("FluidR3_GM.sf2 not available")
        return
    }
    var sf2 = sf2_load("{SF2_DIR}/FluidR3_GM.sf2")
    if (sf2 == null) {
        t->skip("FluidR3_GM.sf2 failed to load")
        return
    }
    t |> run("piano C4 on root key has phase_inc near 1.0") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        let zones = sf2_find_zones(*sf2, idx, 60, 100)
        if (!empty(zones)) {
            let preset = sf2.presets[idx]
            var pzone : SF2Zone const?
            for (pz in preset.zones) {
                if (get_generator(*pz, int(SF2GenOper.instrument), -1) == zones[0].x) {
                    pzone = pz
                    break
                }
            }
            var cc_def : int[128]; sf2_default_cc_values(cc_def)
            var cv : ma_sf2_voice; var as_ = SF2VoiceAttenState()
            if (sf2_create_c_voice(*sf2, zones[0].x, zones[0].y, pzone, preset.global_zone, 60, 100, 0, cc_def, cv, as_)) {
                t |> success(cv.phase_inc > 0.1lf)
                t |> success(cv.phase_inc < 4.0lf)
            }
        }
    }
    t |> run("12 semitones apart on same zone doubles phase_inc") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        if (idx >= 0) {
            let zones_lo = sf2_find_zones(*sf2, idx, 55, 100)
            if (!empty(zones_lo)) {
                let preset = sf2.presets[idx]
                let z = zones_lo[0]
                var pzone : SF2Zone const?
                for (pz in preset.zones) {
                    if (get_generator(*pz, int(SF2GenOper.instrument), -1) == z.x) {
                        pzone = pz
                        break
                    }
                }
                var cc_def2 : int[128]; sf2_default_cc_values(cc_def2)
                var cv_lo : ma_sf2_voice; var asl = SF2VoiceAttenState()
                var cv_hi : ma_sf2_voice; var ash = SF2VoiceAttenState()
                if (sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 55, 100, 0, cc_def2, cv_lo, asl) && sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 67, 100, 0, cc_def2, cv_hi, ash)) {
                    let ratio = cv_hi.phase_inc / cv_lo.phase_inc
                    t |> roughly_equal(2.0, float(ratio), 0.01, "octave ratio: ")
                }
            }
        }
    }
    unsafe { delete sf2; }
}

// ─── Loop Mode Tests ───

def private test_loop_mode_0(t : T?) {
    var sample_data : array<int16>
    sample_data |> resize(1000)
    for (i in range(1000)) {
        sample_data[i] = int16(sin(float(i) * 0.1) * 16000.0)
    }
    var cv : ma_sf2_voice
    ma_sf2_voice_init(unsafe(addr(cv)), 48000.0)
    cv.sample_start = 0
    cv.sample_end = 100
    cv.loop_start = 0
    cv.loop_end = 100
    cv.loop_mode = 0
    cv.phase_inc = 1.0lf
    cv.attenuation = 1.0
    cv.vol_env.stage = 4 // sustain
    cv.vol_env.level = 1.0
    cv.vol_env.sustain_level = 1.0
    cv.vol_env.release_sec = 0.1
    var output : array<float>
    output |> resize(400)
    unsafe { ma_sf2_voice_render(addr(cv), addr(sample_data[0]), length(sample_data), addr(output[0]), 0, 200); }
    t |> success(cv.finished != 0)
}

def private test_loop_mode_1(t : T?) {
    var sample_data : array<int16>
    sample_data |> resize(1000)
    for (i in range(1000)) {
        sample_data[i] = int16(sin(float(i) * 0.1) * 16000.0)
    }
    var cv : ma_sf2_voice
    ma_sf2_voice_init(unsafe(addr(cv)), 48000.0)
    cv.sample_start = 0
    cv.sample_end = 500
    cv.loop_start = 100
    cv.loop_end = 200
    cv.loop_mode = 1
    cv.phase_inc = 1.0lf
    cv.attenuation = 1.0
    cv.vol_env.stage = 4 // sustain
    cv.vol_env.level = 1.0
    cv.vol_env.sustain_level = 1.0
    cv.vol_env.release_sec = 0.1
    var output : array<float>
    output |> resize(1000)
    unsafe { ma_sf2_voice_render(addr(cv), addr(sample_data[0]), length(sample_data), addr(output[0]), 0, 500); }
    t |> success(cv.finished == 0)
    t |> success(cv.position < 200.0lf)
}

def private render_stereo_loop_voice(loop_end_r : int) : ma_sf2_voice {
    var sample_data : array<int16>
    sample_data |> resize(1000)
    for (i in range(1000)) {
        sample_data[i] = int16(sin(float(i) * 0.1) * 16000.0)
    }
    var cv : ma_sf2_voice
    ma_sf2_voice_init(unsafe(addr(cv)), 48000.0)
    cv.stereo = 1
    cv.sample_start = 0
    cv.sample_end = 500
    cv.loop_start = 100
    cv.loop_end = 200
    cv.sample_start_r = 500
    cv.sample_end_r = 1000
    cv.loop_start_r = 600
    cv.loop_end_r = loop_end_r
    cv.loop_mode = 1
    cv.phase_inc = 1.0lf
    cv.attenuation = 1.0
    cv.vol_env.stage = 4 // sustain
    cv.vol_env.level = 1.0
    cv.vol_env.sustain_level = 1.0
    cv.vol_env.release_sec = 0.1
    var output : array<float>
    output |> resize(1600)
    unsafe {
        ma_sf2_voice_render(addr(cv), addr(sample_data[0]), length(sample_data),
                            addr(output[0]), 0, 800)
    }
    return cv
}

[test(description = "a stereo voice loops only when both channels have a loop region, so a zero-length right loop plays through to the sample end instead of dividing by zero")]
def test_stereo_loop_needs_both_channels(t : T?) {
    t |> run("both loop regions valid: the voice loops continuously") <| @(t : T?) {
        let cv = render_stereo_loop_voice(700)
        t |> success(cv.finished == 0)
    }
    t |> run("right loop region empty: the voice does not loop and finishes") <| @(t : T?) {
        let cv = render_stereo_loop_voice(600)
        t |> success(cv.finished != 0)
    }
}

[test(description = "loop mode 0 ends the voice at the sample end and loop mode 1 loops it")]
def test_loop_modes(t : T?) {
    t |> run("loop mode 0: voice finishes at sample end") <| @(t : T?) {
        test_loop_mode_0(t)
    }
    t |> run("loop mode 1: voice loops continuously") <| @(t : T?) {
        test_loop_mode_1(t)
    }
}

// ─── C Voice Tests ───

[test(description = "the C voice renders non-zero output, releases on note-off and ends quickly on end_quick")]
def test_c_voice_render(t : T?) {   // nolint:STYLE038 flat list of arms sharing one loaded-sf2 fixture and its teardown
    var st : FStat
    if (!stat("{SF2_DIR}/FluidR3_GM.sf2", st)) {
        t->skip("FluidR3_GM.sf2 not available")
        return
    }
    var sf2 = sf2_load("{SF2_DIR}/FluidR3_GM.sf2")
    if (sf2 == null) {
        t->skip("FluidR3_GM.sf2 failed to load")
        return
    }
    t |> run("C voice produces non-zero output") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        let zones = sf2_find_zones(*sf2, idx, 60, 127)
        if (!empty(zones)) {
            let preset = sf2.presets[idx]
            var pzone : SF2Zone const?
            for (pz in preset.zones) {
                if (get_generator(*pz, int(SF2GenOper.instrument), -1) == zones[0].x) {
                    pzone = pz
                    break
                }
            }
            var cc3 : int[128]; sf2_default_cc_values(cc3)
            var cv : ma_sf2_voice; var as3 = SF2VoiceAttenState()
            if (sf2_create_c_voice(*sf2, zones[0].x, zones[0].y, pzone, preset.global_zone, 60, 127, 0, cc3, cv, as3)) {
                var output : array<float>
                output |> resize(2000)
                unsafe {
                    ma_sf2_voice_render(addr(cv), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), 0, 1000)
                }
                var max_val = 0.0
                for (v in output) {
                    if (abs(v) > max_val) { max_val = abs(v); }
                }
                t |> success(max_val > 0.01)
            }
        }
    }
    t |> run("C voice note-off triggers release and finishes") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        let zones = sf2_find_zones(*sf2, idx, 60, 127)
        if (!empty(zones)) {
            let preset = sf2.presets[idx]
            var pzone : SF2Zone const?
            for (pz in preset.zones) {
                if (get_generator(*pz, int(SF2GenOper.instrument), -1) == zones[0].x) {
                    pzone = pz
                    break
                }
            }
            var cc4 : int[128]; sf2_default_cc_values(cc4)
            var cv : ma_sf2_voice; var as4 = SF2VoiceAttenState()
            if (sf2_create_c_voice(*sf2, zones[0].x, zones[0].y, pzone, preset.global_zone, 60, 127, 0, cc4, cv, as4)) {
                var output : array<float>
                output |> resize(192000 * 2)  // 4 seconds
                // render 1 second, then note-off, render 3 more seconds
                unsafe {
                    ma_sf2_voice_render(addr(cv), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), 0, 48000)
                    ma_sf2_voice_note_off(addr(cv))
                    ma_sf2_voice_render(addr(cv), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), 48000 * 2, 144000)
                }
                t |> success(cv.finished != 0)
            }
        }
    }
    t |> run("C voice end_quick kills fast") <| @(t : T?) {
        let idx = sf2_find_preset(*sf2, 128, 0)  // drums
        let zones = sf2_find_zones(*sf2, idx, 36, 127)
        if (!empty(zones)) {
            let preset = sf2.presets[idx]
            var pzone : SF2Zone const?
            for (pz in preset.zones) {
                if (get_generator(*pz, int(SF2GenOper.instrument), -1) == zones[0].x) {
                    pzone = pz
                    break
                }
            }
            var cc5 : int[128]; sf2_default_cc_values(cc5)
            var cv : ma_sf2_voice; var as5 = SF2VoiceAttenState()
            if (sf2_create_c_voice(*sf2, zones[0].x, zones[0].y, pzone, preset.global_zone, 36, 127, 9, cc5, cv, as5)) {
                ma_sf2_voice_end_quick(unsafe(addr(cv)))
                var output : array<float>
                output |> resize(96000)  // 1 second
                unsafe {
                    ma_sf2_voice_render(addr(cv), addr(sf2.sample_data[0]), length(sf2.sample_data), addr(output[0]), 0, 48000)
                }
                // should be finished after 10ms quick release + render
                t |> success(cv.finished != 0)
            }
        }
    }
    unsafe { delete sf2; }
}

// ─── Pan Tests ───

[test(description = "SF2 pan -500, 0 and +500 give full left, equal-power center and full right")]
def test_pan(t : T?) {
    t |> run("pan -500 = full left") <| @(t : T?) {
        let pan = clamp(-500.0 / 500.0, -1.0, 1.0)
        let pan05 = pan * 0.5
        t |> roughly_equal(1.0, sqrt(0.5 - pan05), 0.001, "left gain: ")
        t |> roughly_equal(0.0, sqrt(0.5 + pan05), 0.001, "right gain: ")
    }
    t |> run("pan 0 = center, equal power") <| @(t : T?) {
        let pan05 = 0.0
        let l = sqrt(0.5 - pan05)
        let r = sqrt(0.5 + pan05)
        t |> roughly_equal(l, r, 0.001, "center balance: ")
        t |> roughly_equal(0.707, l, 0.01, "center gain: ")
    }
    t |> run("pan +500 = full right") <| @(t : T?) {
        let pan = clamp(500.0 / 500.0, -1.0, 1.0)
        let pan05 = pan * 0.5
        t |> roughly_equal(0.0, sqrt(0.5 - pan05), 0.001, "left gain: ")
        t |> roughly_equal(1.0, sqrt(0.5 + pan05), 0.001, "right gain: ")
    }
}

def try_load_sf2(path : string; t : T?) : SF2File const? {
    var st : FStat
    if (!stat(path, st)) {
        t->skip("{path} not available")
        return null
    }
    let sf2 = sf2_load(path)
    if (sf2 == null) {
        t->skip("failed to load {path}")
    }
    return sf2
}


// ─── Modulator Source Unpacking ───

[test(description = "a modulator source word unpacks into its CC flag, controller, curve, polarity and direction")]
def test_mod_source_unpacking(t : T?) {
    t |> run("CC flag") @(t : T?) {
        // CC1 (mod wheel), positive, unipolar, linear = 0x0001 | 0x0080 = 0x0081 = 129
        let src = int(0x81)
        t |> equal(sf2_mod_src_index(src), 1)
        t |> success(sf2_mod_src_cc(src))
        t |> success(!sf2_mod_src_negative(src))
        t |> success(!sf2_mod_src_bipolar(src))
        t |> equal(sf2_mod_src_type(src), 0)  // linear
    }
    t |> run("general controller velocity, concave, negative, unipolar") @(t : T?) {
        // velocity=2, GC=0, concave=1<<10, negative=1<<8, unipolar=0
        // 0x0002 | 0x0100 | 0x0400 = 0x0502
        let src = int(0x502)
        t |> equal(sf2_mod_src_index(src), 2)  // velocity
        t |> success(!sf2_mod_src_cc(src))
        t |> success(sf2_mod_src_negative(src))
        t |> success(!sf2_mod_src_bipolar(src))
        t |> equal(sf2_mod_src_type(src), 1)  // concave
    }
    t |> run("pitch wheel, bipolar, positive, linear") @(t : T?) {
        // pitchWheel=14, GC=0, bipolar=0x200, positive=0, linear=0
        let src = int(0x20E)
        t |> equal(sf2_mod_src_index(src), 14)  // pitch wheel
        t |> success(!sf2_mod_src_cc(src))
        t |> success(!sf2_mod_src_negative(src))
        t |> success(sf2_mod_src_bipolar(src))
        t |> equal(sf2_mod_src_type(src), 0)  // linear
    }
    t |> run("NONE source") @(t : T?) {
        let src = 0
        t |> equal(sf2_mod_src_index(src), 0)
        t |> success(!sf2_mod_src_cc(src))
    }
}

// ─── Modulator Evaluation ───

[test(description = "a modulator evaluates to the right amount for its source, curve and value")]
def test_mod_evaluation(t : T?) {
    var cc_values : int[128]
    cc_values[7] = 127   // volume
    cc_values[10] = 64   // pan
    cc_values[11] = 127  // expression
    cc_values[1] = 0     // mod wheel

    t |> run("NONE src always produces amount") @(t : T?) {
        // src1=NONE (0), src2=NONE (0), amount=100
        let m = SF2Modulator(src_oper = 0, amt_src_oper = 0, dest_oper = 0, amount = 100, transform = 0)
        let val = sf2_mod_get_value(m, 127, 60, cc_values)
        t |> roughly_equal(100.0, val, 0.1)
    }
    t |> run("velocity src, linear positive unipolar, full velocity") @(t : T?) {
        // src1 = velocity(2), GC, linear, positive, unipolar = 0x0002
        // src2 = NONE
        let m = SF2Modulator(src_oper = 2, amt_src_oper = 0, dest_oper = 0, amount = 960, transform = 0)
        let val = sf2_mod_get_value(m, 127, 60, cc_values)
        // velocity 127 / range 128 = 0.9921875, * 1.0 (NONE) * 960 = ~952.5
        t |> roughly_equal(952.5, val, 1.0)
    }
    t |> run("velocity src, linear positive unipolar, low velocity") @(t : T?) {
        let m = SF2Modulator(src_oper = 2, amt_src_oper = 0, dest_oper = 0, amount = 960, transform = 0)
        let val = sf2_mod_get_value(m, 32, 60, cc_values)
        // velocity 32 / 128 = 0.25, * 960 = 240
        t |> roughly_equal(240.0, val, 1.0)
    }
    t |> run("CC7 volume, concave negative unipolar (default modulator #5)") @(t : T?) {
        // CC7, CC flag=0x80, concave=0x400, negative=0x100, unipolar=0
        // src_oper = 7 | 0x80 | 0x100 | 0x400 = 0x587
        let m = SF2Modulator(src_oper = int(0x587), amt_src_oper = 0, dest_oper = int(SF2GenOper.initialAttenuation), amount = 960, transform = 0)
        // CC7=127: negative unipolar → inv_norm = 1 - 1/128 - 127/128 = 0
        // concave(0) = 0 → result = 960 * 0 * 1 = 0
        let val_max = sf2_mod_get_value(m, 127, 60, cc_values)
        t |> roughly_equal(0.0, val_max, 1.0, "CC7=127: ")
        // CC7=0: inv_norm = 1 - 1/128 - 0 ≈ 0.992
        // concave(128*0.992) ≈ concave(127) = 1.0 → result ≈ 960
        var cc0 : int[128]
        cc0[7] = 0
        let val_min = sf2_mod_get_value(m, 127, 60, cc0)
        t |> roughly_equal(960.0, val_min, 10.0, "CC7=0: ")
    }
    t |> run("absolute value transform") @(t : T?) {
        // bipolar source can produce negative, abs transform makes it positive
        // src = velocity, bipolar(0x200), positive, linear
        let m = SF2Modulator(src_oper = int(0x202), amt_src_oper = 0, dest_oper = 0, amount = -100, transform = 2)
        let val = sf2_mod_get_value(m, 64, 60, cc_values)
        t |> success(val >= 0.0)
    }
    t |> run("two-source modulator (pitch bend * sensitivity)") @(t : T?) {
        // src1 = pitchWheel(14), GC, bipolar(0x200), positive, linear = 0x20E
        // src2 = pitchWheelSens(16), GC, unipolar, positive, linear = 0x0010
        let m = SF2Modulator(src_oper = int(0x20E), amt_src_oper = int(0x10), dest_oper = 0, amount = 12700, transform = 0)
        // pitchWheel=0 (center) → bipolar → 0 (or near 0), result ≈ 0
        let val_center = sf2_mod_get_value(m, 127, 60, cc_values)
        t |> roughly_equal(0.0, val_center, 200.0, "center: ")
    }
}

// ─── Parse Count Tests ───

[test(description = "FluidR3 parses to 746 instrument modulators and none at preset level")]
def test_modulator_parse_counts(t : T?) {
    let sf2 = try_load_sf2("{SF2_DIR}/FluidR3_GM.sf2", t)
    if (sf2 == null) {
        return
    }
    t |> run("FluidR3 has 746 instrument modulators") @(t : T?) {
        var total_imods = 0
        for (inst in sf2.instruments) {
            if (inst.global_zone != null) {
                total_imods += length(inst.global_zone.modulators)
            }
            for (z in inst.zones) {
                total_imods += length(z.modulators)
            }
        }
        t |> equal(total_imods, 746)
    }
    t |> run("FluidR3 has 0 preset modulators") @(t : T?) {
        var total_pmods = 0
        for (p in sf2.presets) {
            if (p.global_zone != null) {
                total_pmods += length(p.global_zone.modulators)
            }
            for (z in p.zones) {
                total_pmods += length(z.modulators)
            }
        }
        t |> equal(total_pmods, 0)
    }
}

// ─── Modulator Effect on Voice ───

[test(description = "modulators change the voice attenuation")]
def test_modulators_affect_voice(t : T?) {
    let sf2 = try_load_sf2("{SF2_DIR}/FluidR3_GM.sf2", t)
    if (sf2 == null) { return ; }
    t |> run("modulators change voice attenuation") @(t : T?) {
        // find piano preset
        let idx = sf2_find_preset(*sf2, 0, 0)
        if (idx < 0) {
            t->skip("no piano preset")
            return
        }
        let zones = sf2_find_zones(*sf2, idx, 60, 127)
        if (empty(zones)) {
            t->skip("no zones for piano C4")
            return
        }
        let z = zones[0]
        let preset = sf2.presets[idx]
        var pzone : SF2Zone const?
        for (pz in preset.zones) {
            let inst_gen = get_generator(*pz, int(SF2GenOper.instrument), -1)
            if (inst_gen == z.x) {
                pzone = pz
                break
            }
        }
        // create voice at high velocity
        var voice_loud : ma_sf2_voice
        var cc_defaults_1 : int[128]; sf2_default_cc_values(cc_defaults_1); var as_1 = SF2VoiceAttenState()
        sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 60, 127, 0, cc_defaults_1, voice_loud, as_1)
        // create voice at low velocity
        var voice_soft : ma_sf2_voice
        var as_2 = SF2VoiceAttenState()
        sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 60, 30, 0, cc_defaults_1, voice_soft, as_2)
        // with 2445 modulators, velocity should significantly affect attenuation
        t |> success(voice_loud.attenuation > voice_soft.attenuation)
        if (voice_loud.attenuation > 0.001 && voice_soft.attenuation > 0.001) {
            let ratio = voice_loud.attenuation / voice_soft.attenuation
            // loud should be notably louder (higher attenuation = more signal)
            t |> success(ratio > 1.5)
        }
    }
}

// ─── Default Modulator Tests ───

[test(description = "the default velocity modulator gives dynamics and a zone modulator overrides it")]
def test_default_modulators(t : T?) {
    let sf2 = try_load_sf2("{SF2_DIR}/FluidR3_GM.sf2", t)
    if (sf2 == null) { return ; }
    t |> run("default velocity→attenuation produces dynamics without hardcoded curve") @(t : T?) {
        let idx = sf2_find_preset(*sf2, 0, 0)
        if (idx < 0) { t->skip("no piano"); return ; }
        let zones = sf2_find_zones(*sf2, idx, 60, 100)
        if (empty(zones)) { t->skip("no zones"); return ; }
        let z = zones[0]
        let preset = sf2.presets[idx]
        var pzone : SF2Zone const?
        for (pz in preset.zones) {
            if (get_generator(*pz, int(SF2GenOper.instrument), -1) == z.x) {
                pzone = pz
                break
            }
        }
        var cc_def : int[128]; sf2_default_cc_values(cc_def)
        var v_loud : ma_sf2_voice; var asl = SF2VoiceAttenState()
        sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 60, 127, 0, cc_def, v_loud, asl)
        var v_mid : ma_sf2_voice; var asm_ = SF2VoiceAttenState()
        sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 60, 64, 0, cc_def, v_mid, asm_)
        var v_soft : ma_sf2_voice; var ass_ = SF2VoiceAttenState()
        sf2_create_c_voice(*sf2, z.x, z.y, pzone, preset.global_zone, 60, 20, 0, cc_def, v_soft, ass_)
        // velocity 127 > 64 > 20 in attenuation
        t |> success(v_loud.attenuation > v_mid.attenuation)
        t |> success(v_mid.attenuation > v_soft.attenuation)
        // the range should be significant (concave curve over 960 cB)
        if (v_soft.attenuation > 0.0001) {
            let dynamic_range = v_loud.attenuation / v_soft.attenuation
            t |> success(dynamic_range > 5.0)
        }
    }
    t |> run("default modulator overridden by zone modulator") @(t : T?) {
        // If a zone has a modulator with same identity as a default,
        // the default should NOT be applied (zone takes precedence).
        var cc_values : int[128]
        cc_values[7] = 127   // volume max → 0 attenuation from default #5
        cc_values[11] = 127  // expression max → 0 attenuation from default #7
        var gen_offsets : float[64]
        // Create a zone with a custom vel→atten modulator (amount=0 = disable)
        var zone = SF2Zone()
        zone.modulators |> push(SF2Modulator(
            src_oper = int(0x0502),  // same as default #1: velocity, concave, negative, unipolar
            amt_src_oper = 0,
            dest_oper = int(SF2GenOper.initialAttenuation),
            amount = 0,  // zone disables it
            transform = 0
        ))
        sf2_apply_modulators(zone, 127, 60, cc_values, gen_offsets)
        sf2_apply_default_modulators(zone, null, null, null, 127, 60, cc_values, gen_offsets)
        // Default #1 should be overridden (amount=0), so attenuation offset should be ~0
        // (only the zone's amount=0 contributes, not the default's 960)
        t |> roughly_equal(0.0, gen_offsets[int(SF2GenOper.initialAttenuation)], 1.0)
    }
}
