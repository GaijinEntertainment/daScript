options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strudel_sf2_voice shared public

require strudel/strudel_sf2
require audio
require math

let SF2_SAMPLE_RATE = 48000.0

// SF2 default velocity→attenuation curve (concave, 960 cB range)
// See SF2 spec section 8.4.2 — default modulator #1
def public sf2_velocity_attenuation(velocity : int) : float {
    if (velocity <= 0) {
        return 0.0
    }
    if (velocity >= 127) {
        return 1.0
    }
    // concave curve: atten_cB = 960 * (-20 / (96 * ln2)) * ln(v/127)
    let v_norm = float(velocity) / 127.0
    let concave = -20.0 / (96.0 * 0.6931472) * log(v_norm)  // 0..1 range, 0 at v=127
    let atten_cb = clamp(concave, 0.0, 1.0) * 480.0
    return centibels_to_linear(int(atten_cb))
}

// ─── Modulator Transform Curves ───

// SF2 concave curve: (-200*2/960) * log10((127-i)/127) for i in 0..127
// From SF2 spec page 73. FluidSynth uses the same formula.
let private PEAK_ATTEN = 960.0

def private sf2_concave(val : float) : float {
    if (val <= 0.0) {
        return 0.0
    }
    if (val >= 127.0) {
        return 1.0
    }
    return clamp((-200.0 * 2.0 / PEAK_ATTEN) * log((127.0 - val) / 127.0) / 2.302585, 0.0, 1.0)
}

def private sf2_convex(val : float) : float {
    if (val <= 0.0) {
        return 0.0
    }
    if (val >= 127.0) {
        return 1.0
    }
    return clamp(1.0 - (-200.0 * 2.0 / PEAK_ATTEN) * log(val / 127.0) / 2.302585, 0.0, 1.0)
}

// ─── Modulator Source Value ───

// Get raw source value for a modulator source operand.
// Returns the value and the range (typically 128.0, but 0x4000 for pitch wheel).
// Returns (value, range) tuple. Range is typically 128.0, but 16384.0 for pitch wheel.
def private sf2_mod_source_value(src : int; velocity, key : int; cc_values : int[128]) : tuple<float; float> {
    if (sf2_mod_src_cc(src)) {
        return float(cc_values[sf2_mod_src_index(src)]) => 128.0
    }
    let idx = sf2_mod_src_index(src)
    if (idx == 0) {        // NONE — value is range (produces 1.0 after normalization)
        return 128.0 => 128.0
    } elif (idx == 2) {    // noteOnVelocity
        return float(velocity) => 128.0
    } elif (idx == 3) {    // noteOnKey
        return float(key) => 128.0
    } elif (idx == 10) {   // keyPressure — not implemented
        return 0.0 => 128.0
    } elif (idx == 13) {   // channelPressure — not implemented
        return 0.0 => 128.0
    } elif (idx == 14) {   // pitchWheel
        return float(cc_values[127]) => 16384.0  // last CC slot as pitchwheel placeholder
    } elif (idx == 16) {   // pitchWheelSensitivity — default 2 semitones
        return 2.0 => 128.0
    }
    return 0.0 => 128.0
}

// Transform a raw source value into [0..1] (unipolar) or [-1..1] (bipolar).
// Applies direction (positive/negative), polarity (unipolar/bipolar), and
// mapping function (linear/concave/convex/switch).
def private sf2_mod_transform_source(src : int; val, src_range : float) : float {
    let idx = sf2_mod_src_index(src)
    if (idx == 0 && !sf2_mod_src_cc(src)) {
        return 1.0  // NONE source always returns 1.0
    }
    let val_norm = val / src_range
    let inv_norm = 1.0 - 1.0 / src_range - val_norm
    let is_negative = sf2_mod_src_negative(src)
    let is_bipolar = sf2_mod_src_bipolar(src)
    let map_type = sf2_mod_src_type(src)
    let max_norm = (src_range - 1.0) / src_range
    var v = 0.0
    if (!is_bipolar) {
        // unipolar [0..1]
        v = is_negative ? inv_norm : val_norm
        if (map_type == 3) {        // switch
            v = v >= 0.5 ? 1.0 : 0.0
        } elif (map_type == 1) {    // concave
            v = min(sf2_concave(128.0 * v), max_norm)
        } elif (map_type == 2) {    // convex
            v = min(sf2_convex(128.0 * v), max_norm)
        }
        // else linear — keep as is
    } else {
        // bipolar [-1..1]
        let base = is_negative ? inv_norm : val_norm
        v = base == max_norm ? base : -1.0 + 2.0 * base
        if (map_type == 3) {        // switch
            v = v >= 0.0 ? 1.0 : -1.0
        } elif (map_type == 1) {    // concave
            v = v >= 0.0 ? min(sf2_concave(128.0 * v), max_norm) : -sf2_concave(128.0 * -v)
        } elif (map_type == 2) {    // convex
            v = v >= 0.0 ? min(sf2_convex(128.0 * v), max_norm) : -sf2_convex(128.0 * -v)
        }
        // else linear — keep as is
    }
    return v
}

// Evaluate a single SF2 modulator: amount * transform(src1) * transform(src2)
def public sf2_mod_get_value(m : SF2Modulator; velocity, key : int; cc_values : int[128]) : float {
    let s1 = sf2_mod_source_value(m.src_oper, velocity, key, cc_values)
    let v1 = sf2_mod_transform_source(m.src_oper, s1._0, s1._1)
    let s2 = sf2_mod_source_value(m.amt_src_oper, velocity, key, cc_values)
    let v2 = sf2_mod_transform_source(m.amt_src_oper, s2._0, s2._1)
    var result = float(m.amount) * v1 * v2
    if (m.transform == 2) {  // absolute value transform
        result = abs(result)
    }
    return result
}

// Apply all modulators from a zone to a generator accumulator.
// gen_offsets[oper] += sum of modulator outputs targeting that generator.
def public sf2_apply_modulators(zone : SF2Zone; velocity, key : int; cc_values : int[128]; var gen_offsets : float[64]) {
    for (m in zone.modulators) {
        let val = sf2_mod_get_value(m, velocity, key, cc_values)
        if (m.dest_oper >= 0 && m.dest_oper < 64) {
            gen_offsets[m.dest_oper] += val
        }
    }
}

// ─── SF2 Default Modulators (spec 8.4.1-8.4.10) ───

// Two modulators match identity if they have the same src1, src2, and destination.
def public sf2_mod_identity_match(a, b : SF2Modulator) : bool {
    return a.src_oper == b.src_oper && a.amt_src_oper == b.amt_src_oper && a.dest_oper == b.dest_oper
}

// Check if any modulator in the zone overrides (has same identity as) a default modulator.
def private sf2_zone_overrides(zone : SF2Zone; def_mod : SF2Modulator) : bool {
    for (m in zone.modulators) {
        if (sf2_mod_identity_match(m, def_mod)) {
            return true
        }
    }
    return false
}

// The 10 SF2 default modulators. Built from SF2.01 spec sections 8.4.1-8.4.10.
// Encoding: src = index | (CC ? 0x80 : 0) | (negative ? 0x100 : 0) | (bipolar ? 0x200 : 0) | (type << 10)
// type: 0=linear, 1=concave, 2=convex, 3=switch
let private SF2_DEFAULT_MODS = fixed_array(
    // #1: velocity → attenuation (concave, negative, unipolar) — 960 cB
    SF2Modulator(src_oper = int(0x0502), amt_src_oper = 0, dest_oper = int(SF2GenOper.initialAttenuation), amount = 960, transform = 0),
    // #2: velocity → filter cutoff (linear, negative, unipolar; switch, unipolar, positive)
    // FluidSynth disables this (returns 0). We follow FluidSynth.
    SF2Modulator(src_oper = int(0x0102), amt_src_oper = int(0x0C02), dest_oper = int(SF2GenOper.initialFilterFc), amount = 0, transform = 0),
    // #3: channel pressure → vib LFO pitch (linear, positive, unipolar) — 50 cents
    SF2Modulator(src_oper = int(0x000D), amt_src_oper = 0, dest_oper = int(SF2GenOper.vibLfoToPitch), amount = 50, transform = 0),
    // #4: CC1 mod wheel → vib LFO pitch (CC, linear, positive, unipolar) — 50 cents
    SF2Modulator(src_oper = int(0x0081), amt_src_oper = 0, dest_oper = int(SF2GenOper.vibLfoToPitch), amount = 50, transform = 0),
    // #5: CC7 volume → attenuation (CC, concave, negative, unipolar) — 960 cB
    SF2Modulator(src_oper = int(0x0587), amt_src_oper = 0, dest_oper = int(SF2GenOper.initialAttenuation), amount = 960, transform = 0),
    // #6: CC10 pan → pan (CC, linear, bipolar, positive) — 500 (0.1% units)
    SF2Modulator(src_oper = int(0x028A), amt_src_oper = 0, dest_oper = int(SF2GenOper.pan), amount = 500, transform = 0),
    // #7: CC11 expression → attenuation (CC, concave, negative, unipolar) — 960 cB
    SF2Modulator(src_oper = int(0x058B), amt_src_oper = 0, dest_oper = int(SF2GenOper.initialAttenuation), amount = 960, transform = 0),
    // #8: CC91 → reverb send (CC, linear, positive, unipolar) — 200 (0.1% units)
    SF2Modulator(src_oper = int(0x00DB), amt_src_oper = 0, dest_oper = int(SF2GenOper.reverbEffectsSend), amount = 200, transform = 0),
    // #9: CC93 → chorus send (CC, linear, positive, unipolar) — 200 (0.1% units)
    SF2Modulator(src_oper = int(0x00DD), amt_src_oper = 0, dest_oper = int(SF2GenOper.chorusEffectsSend), amount = 200, transform = 0),
    // #10: pitch wheel × pitch wheel sensitivity → fine tune (bipolar, linear × unipolar, linear) — 12700 cents
    SF2Modulator(src_oper = int(0x020E), amt_src_oper = int(0x0010), dest_oper = int(SF2GenOper.fineTune), amount = 12700, transform = 0)
)

// Apply default modulators that are not overridden by any zone modulator.
// Call this AFTER applying zone modulators to gen_offsets.
def public sf2_apply_default_modulators(izone : SF2Zone; global_izone : SF2Zone const?;
                                        pzone : SF2Zone const?; global_pzone : SF2Zone const?;
                                        velocity, key : int; cc_values : int[128]; var gen_offsets : float[64]) {
    for (dm in SF2_DEFAULT_MODS) {
        if (dm.amount == 0) {
            continue  // disabled default (#2 vel→filter)
        }
        // check if any zone overrides this default
        var overridden = sf2_zone_overrides(izone, dm)
        if (!overridden && global_izone != null) {
            overridden = sf2_zone_overrides(*global_izone, dm)
        }
        if (!overridden && pzone != null) {
            overridden = sf2_zone_overrides(*pzone, dm)
        }
        if (!overridden && global_pzone != null) {
            overridden = sf2_zone_overrides(*global_pzone, dm)
        }
        if (!overridden) {
            let val = sf2_mod_get_value(dm, velocity, key, cc_values)
            if (dm.dest_oper >= 0 && dm.dest_oper < 64) {
                gen_offsets[dm.dest_oper] += val
            }
        }
    }
}

// ─── Envelope ───

enum SF2EnvStage {
    env_delay
    env_attack
    env_hold
    env_decay
    env_sustain
    env_release
    env_finished
}

struct SF2Envelope {
    stage : SF2EnvStage = SF2EnvStage.env_delay
    time_in_stage : float = 0.0
    level : float = 0.0
    release_level : float = 1.0     // captured at note-off
    slope : float = 0.0             // per-sample slope (linear or exponential base)
    is_exponential : bool = false   // current segment uses exponential curve
    is_amp_env : bool = false       // amplitude envelope uses exponential decay/release
    midi_velocity : int = 127       // for velocity-scaled attack
    // parameters (seconds)
    delay_sec : float = 0.0
    attack_sec : float = 0.001
    hold_sec : float = 0.0
    decay_sec : float = 0.001
    sustain_level : float = 1.0     // 0.0 to 1.0
    release_sec : float = 0.001
}

// Initialize envelope for a specific stage transition
def private sf2_env_next_segment(var env : SF2Envelope&; sample_rate : float) {
    if (env.stage == SF2EnvStage.env_delay) {
        // -> attack (velocity-scaled for amp env)
        var attack_samples = env.attack_sec * sample_rate
        if (env.is_amp_env) {
            attack_samples *= float(145 - env.midi_velocity) / 144.0
        }
        env.stage = SF2EnvStage.env_attack
        env.is_exponential = false
        env.level = 0.0
        env.slope = attack_samples > 0.0 ? 1.0 / attack_samples : 1.0
    } elif (env.stage == SF2EnvStage.env_attack) {
        // -> hold
        env.stage = SF2EnvStage.env_hold
        env.is_exponential = false
        env.level = 1.0
        env.slope = 0.0
        env.time_in_stage = env.hold_sec * sample_rate  // countdown
    } elif (env.stage == SF2EnvStage.env_hold) {
        // -> decay
        let decay_samples = env.decay_sec * sample_rate
        env.stage = SF2EnvStage.env_decay
        if (env.is_amp_env && decay_samples > 0.0) {
            // exponential decay (concave dB-linear curve)
            let mystery_slope = -9.226 / decay_samples
            env.slope = exp(mystery_slope)
            env.is_exponential = true
            if (env.sustain_level > 0.0) {
                // calculate number of samples to reach sustain level
                // following LinuxSampler: target slightly lower, then clamp
            }
        } else {
            env.is_exponential = false
            env.slope = decay_samples > 0.0 ? -1.0 / decay_samples : -1.0
        }
    } elif (env.stage == SF2EnvStage.env_decay) {
        // -> sustain
        env.stage = SF2EnvStage.env_sustain
        env.level = env.sustain_level
        env.slope = 0.0
        env.is_exponential = false
    } elif (env.stage == SF2EnvStage.env_sustain) {
        // -> release (note-off)
        let release_samples = env.release_sec * sample_rate
        env.stage = SF2EnvStage.env_release
        if (env.is_amp_env && release_samples > 0.0) {
            let mystery_slope = -9.226 / release_samples
            env.slope = exp(mystery_slope)
            env.is_exponential = true
        } else {
            env.is_exponential = false
            env.slope = release_samples > 0.0 ? -env.level / release_samples : -1.0
        }
    } elif (env.stage == SF2EnvStage.env_release) {
        // -> done
        env.stage = SF2EnvStage.env_finished
        env.level = 0.0
        env.slope = 0.0
        env.is_exponential = false
    }
}

def sf2_env_start(var env : SF2Envelope&; sample_rate : float) {
    // start delay (or skip to attack if delay is 0)
    env.stage = SF2EnvStage.env_delay
    env.level = 0.0
    let delay_samples = env.delay_sec * sample_rate
    if (delay_samples <= 0.0) {
        sf2_env_next_segment(env, sample_rate)
    } else {
        env.slope = 0.0
        env.is_exponential = false
        env.time_in_stage = delay_samples  // countdown
    }
}

def sf2_env_tick(var env : SF2Envelope&; released : bool; sample_rate : float) : float {
    if (released && env.stage != SF2EnvStage.env_release && env.stage != SF2EnvStage.env_finished) {
        // jump to release from current level
        env.release_level = env.level
        let release_samples = env.release_sec * sample_rate
        env.stage = SF2EnvStage.env_release
        if (env.is_amp_env && release_samples > 0.0) {
            let mystery_slope = -9.226 / release_samples
            env.slope = exp(mystery_slope)
            env.is_exponential = true
        } else {
            env.is_exponential = false
            env.slope = release_samples > 0.0 ? -env.level / release_samples : -1.0
        }
    }

    if (env.stage == SF2EnvStage.env_finished) {
        env.level = 0.0
        return 0.0
    }

    // advance level
    if (env.is_exponential) {
        env.level *= env.slope
    } else {
        env.level += env.slope
    }
    env.level = clamp(env.level, 0.0, 1.0)

    // check stage transitions
    if (env.stage == SF2EnvStage.env_delay) {
        env.time_in_stage -= 1.0
        if (env.time_in_stage <= 0.0) {
            sf2_env_next_segment(env, sample_rate)
        }
    } elif (env.stage == SF2EnvStage.env_attack) {
        if (env.level >= 1.0) {
            env.level = 1.0
            sf2_env_next_segment(env, sample_rate)
            if (env.stage == SF2EnvStage.env_hold && env.hold_sec <= 0.0) {
                sf2_env_next_segment(env, sample_rate)  // skip hold
            }
        }
    } elif (env.stage == SF2EnvStage.env_hold) {
        env.time_in_stage -= 1.0
        if (env.time_in_stage <= 0.0) {
            sf2_env_next_segment(env, sample_rate)
        }
    } elif (env.stage == SF2EnvStage.env_decay) {
        if (env.level <= env.sustain_level) {
            env.level = env.sustain_level
            sf2_env_next_segment(env, sample_rate)
        }
    } elif (env.stage == SF2EnvStage.env_release) {
        if (env.level <= 0.001) {
            env.level = 0.0
            env.stage = SF2EnvStage.env_finished
        }
    }

    return env.level
}

// ─── LFO ───

struct SF2LFO {
    delay_sec : float = 0.0
    freq : float = 8.176       // Hz
    phase : float = 0.0
    elapsed : float = 0.0
}

def sf2_lfo_tick(var lfo : SF2LFO&; dt : float) : float {
    lfo.elapsed += dt
    if (lfo.elapsed < lfo.delay_sec) {
        return 0.0
    }
    // triangle wave: -1 to 1
    lfo.phase += lfo.freq * dt
    if (lfo.phase > 1.0) {
        lfo.phase -= floor(lfo.phase)
    }
    return abs(lfo.phase * 4.0 - 2.0) - 1.0
}

// ─── Biquad Low-Pass Filter ───

// Transposed direct form II biquad low-pass filter
struct SF2Biquad {
    q_inv : double = 0.0lf
    a0 : double = 0.0lf
    a1 : double = 0.0lf
    b1 : double = 0.0lf
    b2 : double = 0.0lf
    z1 : double = 0.0lf
    z2 : double = 0.0lf
    active : bool = false
}

def sf2_biquad_setup(var bq : SF2Biquad&; fc_normalized : float) {
    // fc_normalized = cutoff_hz / sample_rate (0 to 0.5)
    // QInv must be set before calling this
    if (fc_normalized >= 0.499) {
        bq.active = false
        return
    }
    bq.active = true
    let K = tan(double(PI) * double(fc_normalized))
    let KK = K * K
    let norm = 1.0lf / (1.0lf + K * bq.q_inv + KK)
    bq.a0 = KK * norm
    bq.a1 = 2.0lf * bq.a0
    bq.b1 = 2.0lf * (KK - 1.0lf) * norm
    bq.b2 = (1.0lf - K * bq.q_inv + KK) * norm
}

def sf2_biquad_tick(var bq : SF2Biquad&; input : float) : float {
    if (!bq.active) {
        return input
    }
    let inp = double(input)
    let output = inp * bq.a0 + bq.z1
    bq.z1 = inp * bq.a1 + bq.z2 - bq.b1 * output
    bq.z2 = inp * bq.a0 - bq.b2 * output
    return float(output)
}

// ─── Generator Resolution ───

def private get_gen(izone : SF2Zone; global_izone : SF2Zone const?; pzone : SF2Zone const?; global_pzone : SF2Zone const?; oper : int; def_val : int) : int {
    // instrument zone overrides instrument global, then preset zone adds
    var val = def_val
    if (global_izone != null) {
        val = get_generator(*global_izone, oper, val)
    }
    val = get_generator(izone, oper, val)
    // preset-level generators are additive (except ranges and terminal gens)
    if (oper != int(SF2GenOper.keyRange) && oper != int(SF2GenOper.velRange) &&
        oper != int(SF2GenOper.sampleID) && oper != int(SF2GenOper.instrument)) {
        var preset_add = 0
        if (global_pzone != null) {
            preset_add += get_generator(*global_pzone, oper, 0)
        }
        if (pzone != null) {
            preset_add += get_generator(*pzone, oper, 0)
        }
        val += preset_add
    }
    return val
}

// ─── C Voice Creation (populates ma_sf2_voice from SF2 generators) ───

def public sf2_create_c_voice(sf2 : SF2File; inst_idx, izone_idx : int;
                              pzone : SF2Zone const?; global_pzone : SF2Zone const?;
                              note, velocity, channel : int; var out_voice : ma_sf2_voice) : bool {
    let inst = sf2.instruments[inst_idx]
    let izone = inst.zones[izone_idx]
    let global_izone = inst.global_zone
    let sr = float(SF2_SAMPLE_RATE)

    let sid = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.sampleID), -1)
    if (sid < 0 || sid >= length(sf2.samples)) {
        return false
    }
    let shdr = sf2.samples[sid]

    unsafe { ma_sf2_voice_init(addr(out_voice), sr); }

    // sample addressing
    let start_offset = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.startAddrsOffset), 0) + get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.startAddrsCoarseOffset), 0) * 32768
    let end_offset = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.endAddrsOffset), 0) + get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.endAddrsCoarseOffset), 0) * 32768
    let loop_start_offset = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.startloopAddrsOffset), 0) + get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.startloopAddrsCoarseOffset), 0) * 32768
    let loop_end_offset = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.endloopAddrsOffset), 0) + get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.endloopAddrsCoarseOffset), 0) * 32768

    out_voice.sample_start = int(shdr.start) + start_offset
    out_voice.sample_end = int(shdr.end) + end_offset
    out_voice.loop_start = int(shdr.loop_start) + loop_start_offset
    out_voice.loop_end = int(shdr.loop_end) + loop_end_offset
    out_voice.loop_mode = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.sampleModes), 0)

    // stereo-linked sample detection
    // Resolve the paired sample: use sample_link if it points to the matching type,
    // otherwise fall back to adjacent sample (many SF2 editors leave sample_link=0
    // but place L/R pairs adjacently in the shdr array).
    if (shdr.sample_type == SF2_SAMPLE_LEFT || shdr.sample_type == SF2_SAMPLE_RIGHT) {
        var link_idx = int(shdr.sample_link)
        let expected_type = shdr.sample_type == SF2_SAMPLE_LEFT ? SF2_SAMPLE_RIGHT : SF2_SAMPLE_LEFT
        // validate sample_link
        if (link_idx < 0 || link_idx >= length(sf2.samples) || sf2.samples[link_idx].sample_type != expected_type) {
            // fallback: adjacent sample (L at sid, R at sid+1; or R at sid, L at sid-1)
            if (shdr.sample_type == SF2_SAMPLE_LEFT && sid + 1 < length(sf2.samples) && sf2.samples[sid + 1].sample_type == SF2_SAMPLE_RIGHT) {
                link_idx = sid + 1
            } elif (shdr.sample_type == SF2_SAMPLE_RIGHT && sid - 1 >= 0 && sf2.samples[sid - 1].sample_type == SF2_SAMPLE_LEFT) {
                link_idx = sid - 1
            } else {
                link_idx = -1  // no valid pair found — stay mono
            }
        }
        if (link_idx >= 0) {
            let link_shdr = sf2.samples[link_idx]
            out_voice.stereo = 1
            if (shdr.sample_type == SF2_SAMPLE_LEFT) {
                // main fields already set from left sample; populate right from link
                out_voice.sample_start_r = int(link_shdr.start) + start_offset
                out_voice.sample_end_r = int(link_shdr.end) + end_offset
                out_voice.loop_start_r = int(link_shdr.loop_start) + loop_start_offset
                out_voice.loop_end_r = int(link_shdr.loop_end) + loop_end_offset
            } else {
                // rightSample: swap — main becomes link (left), right becomes this sample
                out_voice.sample_start_r = out_voice.sample_start
                out_voice.sample_end_r = out_voice.sample_end
                out_voice.loop_start_r = out_voice.loop_start
                out_voice.loop_end_r = out_voice.loop_end
                out_voice.sample_start = int(link_shdr.start) + start_offset
                out_voice.sample_end = int(link_shdr.end) + end_offset
                out_voice.loop_start = int(link_shdr.loop_start) + loop_start_offset
                out_voice.loop_end = int(link_shdr.loop_end) + loop_end_offset
            }
        }
    }

    // root key + pitch
    let override_root = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.overridingRootKey), -1)
    let root_key = override_root >= 0 ? override_root : shdr.original_pitch
    let coarse_tune = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.coarseTune), 0)
    let fine_tune = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.fineTune), 0)
    let scale_tuning = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.scaleTuning), 100)
    let pitch_cents = float(note - root_key) * float(scale_tuning) + float(coarse_tune) * 100.0 + float(fine_tune)
    out_voice.phase_inc = double(float(shdr.sample_rate) / sr) * double(pow(2.0, pitch_cents / 1200.0))

    // volume envelope
    out_voice.vol_env.delay_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.delayVolEnv), -12000))
    out_voice.vol_env.attack_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.attackVolEnv), -12000))
    out_voice.vol_env.hold_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.holdVolEnv), -12000))
    out_voice.vol_env.decay_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.decayVolEnv), -12000))
    let sustain_cb = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.sustainVolEnv), 0)
    out_voice.vol_env.sustain_level = sustain_cb >= 1000 ? 0.0 : centibels_to_linear(sustain_cb)
    out_voice.vol_env.release_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.releaseVolEnv), -12000))
    let key_to_hold = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.keynumToVolEnvHold), 0)
    let key_to_decay = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.keynumToVolEnvDecay), 0)
    if (key_to_hold != 0) {
        out_voice.vol_env.hold_sec *= pow(2.0, float(key_to_hold) * float(note - 60) / 1200.0)
    }
    if (key_to_decay != 0) {
        out_voice.vol_env.decay_sec *= pow(2.0, float(key_to_decay) * float(note - 60) / 1200.0)
    }
    out_voice.vol_env.is_amp_env = 1
    out_voice.vol_env.midi_velocity = velocity
    unsafe { ma_sf2_envelope_start(addr(out_voice.vol_env), sr); }

    // modulation envelope
    out_voice.mod_env.delay_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.delayModEnv), -12000))
    out_voice.mod_env.attack_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.attackModEnv), -12000))
    out_voice.mod_env.hold_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.holdModEnv), -12000))
    out_voice.mod_env.decay_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.decayModEnv), -12000))
    let mod_sustain = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.sustainModEnv), 0)
    out_voice.mod_env.sustain_level = clamp(1.0 - float(mod_sustain) / 1000.0, 0.0, 1.0)
    out_voice.mod_env.release_sec = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.releaseModEnv), -12000))
    out_voice.mod_env.is_amp_env = 0
    unsafe { ma_sf2_envelope_start(addr(out_voice.mod_env), sr); }

    // LFOs
    out_voice.mod_lfo_delay = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.delayModLFO), -12000))
    out_voice.mod_lfo_freq = cents_to_hz(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.freqModLFO), 0))
    out_voice.vib_lfo_delay = timecents_to_seconds(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.delayVibLFO), -12000))
    out_voice.vib_lfo_freq = cents_to_hz(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.freqVibLFO), 0))

    // modulation routing
    out_voice.mod_env_to_pitch = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.modEnvToPitch), 0))
    out_voice.mod_env_to_filter_fc = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.modEnvToFilterFc), 0))
    out_voice.mod_lfo_to_pitch = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.modLfoToPitch), 0))
    out_voice.mod_lfo_to_filter_fc = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.modLfoToFilterFc), 0))
    out_voice.mod_lfo_to_volume = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.modLfoToVolume), 0))
    out_voice.vib_lfo_to_pitch = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.vibLfoToPitch), 0))

    // filter
    let filter_fc_cents = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.initialFilterFc), 13500))
    out_voice.initial_filter_fc = filter_fc_cents
    out_voice.initial_filter_q = float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.initialFilterQ), 0)) / 10.0
    out_voice.filter.q_inv = 1.0lf / pow(10.0lf, double(out_voice.initial_filter_q) / 20.0lf)
    let fc_hz = filter_fc_cents <= 13500.0 ? cents_to_hz(int(filter_fc_cents)) : sr * 0.5
    let fc_norm = fc_hz / sr
    out_voice.filter.active = fc_norm < 0.499 ? 1 : 0
    if (out_voice.filter.active != 0) {
        unsafe { ma_sf2_biquad_setup(addr(out_voice.filter), fc_norm); }
    }

    // volume — SF2 spec centibels + concave velocity curve
    let atten_raw = get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.initialAttenuation), 0)
    // velocity→attenuation is now handled by default modulator #1 (concave curve, 960 cB)
    out_voice.attenuation = centibels_to_linear(atten_raw)
    out_voice.pan = clamp(float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.pan), 0)) / 500.0, -1.0, 1.0)

    // ─── Apply modulators ───
    // Compute modulator offsets from instrument zone, global instrument zone,
    // preset zone, and global preset zone. All offsets are additive to the
    // generator values already set above.
    var cc_values : int[128]
    // default CC values: CC7 (volume)=127, CC10 (pan)=64, CC11 (expression)=127
    cc_values[7] = 127
    cc_values[10] = 64
    cc_values[11] = 127
    var gen_offsets : float[64]
    sf2_apply_modulators(*izone, velocity, note, cc_values, gen_offsets)
    if (global_izone != null) {
        // global zone modulators apply unless overridden by zone modulator with same src/dest
        sf2_apply_modulators(*global_izone, velocity, note, cc_values, gen_offsets)
    }
    if (pzone != null) {
        sf2_apply_modulators(*pzone, velocity, note, cc_values, gen_offsets)
    }
    if (global_pzone != null) {
        sf2_apply_modulators(*global_pzone, velocity, note, cc_values, gen_offsets)
    }
    // Apply default modulators (SF2 spec 8.4.1-8.4.10) unless overridden by zone modulators
    sf2_apply_default_modulators(*izone, global_izone, pzone, global_pzone, velocity, note, cc_values, gen_offsets)
    // Apply offsets to voice parameters
    // Pitch (cents)
    let mod_pitch = gen_offsets[int(SF2GenOper.coarseTune)] * 100.0 + gen_offsets[int(SF2GenOper.fineTune)]
    if (abs(mod_pitch) > 0.01) {
        out_voice.phase_inc *= double(pow(2.0, mod_pitch / 1200.0))
    }
    // Filter cutoff (cents from 8.176 Hz)
    let mod_fc = gen_offsets[int(SF2GenOper.initialFilterFc)]
    if (abs(mod_fc) > 0.01) {
        let new_fc_cents = filter_fc_cents + mod_fc
        let new_fc_hz = new_fc_cents <= 13500.0 ? cents_to_hz(int(new_fc_cents)) : sr * 0.5
        let new_fc_norm = new_fc_hz / sr
        out_voice.filter.active = new_fc_norm < 0.499 ? 1 : 0
        if (out_voice.filter.active != 0) {
            unsafe { ma_sf2_biquad_setup(addr(out_voice.filter), new_fc_norm); }
        }
    }
    // Filter Q (centibels)
    let mod_q = gen_offsets[int(SF2GenOper.initialFilterQ)]
    if (abs(mod_q) > 0.01) {
        out_voice.initial_filter_q += mod_q / 10.0
        out_voice.filter.q_inv = 1.0lf / pow(10.0lf, double(out_voice.initial_filter_q) / 20.0lf)
    }
    // Attenuation (centibels) — additive to existing
    let mod_atten = gen_offsets[int(SF2GenOper.initialAttenuation)]
    if (abs(mod_atten) > 0.01) {
        out_voice.attenuation *= centibels_to_linear(int(mod_atten))
    }
    // Pan (0.1% units, ±500 = ±full)
    let mod_pan = gen_offsets[int(SF2GenOper.pan)]
    if (abs(mod_pan) > 0.01) {
        out_voice.pan = clamp(out_voice.pan + mod_pan / 500.0, -1.0, 1.0)
    }
    // Modulation routing offsets (cents)
    out_voice.mod_env_to_pitch += gen_offsets[int(SF2GenOper.modEnvToPitch)]
    out_voice.mod_env_to_filter_fc += gen_offsets[int(SF2GenOper.modEnvToFilterFc)]
    out_voice.mod_lfo_to_pitch += gen_offsets[int(SF2GenOper.modLfoToPitch)]
    out_voice.mod_lfo_to_filter_fc += gen_offsets[int(SF2GenOper.modLfoToFilterFc)]
    out_voice.mod_lfo_to_volume += gen_offsets[int(SF2GenOper.modLfoToVolume)]
    out_voice.vib_lfo_to_pitch += gen_offsets[int(SF2GenOper.vibLfoToPitch)]
    // Reverb/chorus send (0.1% units)
    // These are handled externally by sf2_get_reverb_send, modulators add to the generator value

    // exclusive class — stored externally by the caller since ma_sf2_voice doesn't have it
    return true
}

def public sf2_get_exclusive_class(sf2 : SF2File; inst_idx, izone_idx : int;
                                   pzone : SF2Zone const?; global_pzone : SF2Zone const?) : int {
    let inst = sf2.instruments[inst_idx]
    let izone = inst.zones[izone_idx]
    return get_gen(*izone, inst.global_zone, pzone, global_pzone, int(SF2GenOper.exclusiveClass), 0)
}

def public sf2_get_reverb_send(sf2 : SF2File; inst_idx, izone_idx : int;
                               pzone : SF2Zone const?; global_pzone : SF2Zone const?) : float {
    let inst = sf2.instruments[inst_idx]
    let izone = inst.zones[izone_idx]
    let send = get_gen(*izone, inst.global_zone, pzone, global_pzone, int(SF2GenOper.reverbEffectsSend), 0)
    return clamp(float(send) / 1000.0, 0.0, 1.0)
}

def public sf2_get_chorus_send(sf2 : SF2File; inst_idx, izone_idx : int;
                               pzone : SF2Zone const?; global_pzone : SF2Zone const?) : float {
    let inst = sf2.instruments[inst_idx]
    let izone = inst.zones[izone_idx]
    let send = get_gen(*izone, inst.global_zone, pzone, global_pzone, int(SF2GenOper.chorusEffectsSend), 0)
    return clamp(float(send) / 1000.0, 0.0, 1.0)
}
