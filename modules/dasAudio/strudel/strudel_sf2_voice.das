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
    out_voice.attenuation = centibels_to_linear(atten_raw) * sf2_velocity_attenuation(velocity)
    out_voice.pan = clamp(float(get_gen(*izone, global_izone, pzone, global_pzone, int(SF2GenOper.pan), 0)) / 500.0, -1.0, 1.0)

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
