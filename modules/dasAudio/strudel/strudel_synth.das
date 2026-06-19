options gen2
options indenting = 4

module strudel_synth shared public
//! Audio synthesis and effects for the strudel pattern engine.
//! Houses oscillators (sine/saw/square/triangle/supersaw), drum voices (bd/sd/hh/tom/ride/...),
//! biquad/formant filters, the per-voice FX chain and the top-level `render_event_stereo` entry point.

require strudel/strudel_event
require strudel/strudel_samples
require strudel/strudel_sfx        // generated drum render_* call the sfx engine
require audio
require math

let SAMPLE_RATE = 48000
let TWO_PI = 3.14159265358979323846 * 2.0
let OSC_TURN_DOWN = 0.3  // oscillator turn-down — only oscillators, not samples/drums

def note_to_freq(note : float) : float {
    //! Convert a MIDI note number to frequency in Hz (A4 = 69 = 440 Hz, middle C = 60).
    return 440.0 * pow(2.0, (note - 69.0) / 12.0)
}

def noise(var seed : uint&) : float {
    //! Linear congruential pseudo-random noise in [-1.0, 1.0]. Advances the seed in place.
    seed = seed * 1664525u + 1013904223u
    return float(seed >> 9u & 0x7FFFFFu) / float(0x3FFFFFu) - 1.0
}

// Polynomial band-limited step. Smooths the discontinuity at phase 0 / 1
// of a naive sawtooth or square to eliminate aliases above Nyquist.
// Ref: https://www.kvraudio.com/forum/viewtopic.php?t=375517
// t = phase [0,1), dt = freq / sample_rate.
def private poly_blep(t, dt : float) : float {
    var dt2 = dt
    if (dt2 > 0.5) { dt2 = 0.5; }
    if (dt2 < 0.0001) return 0.0;
    if (t < dt2) {
        let x = t / dt2
        return x + x - x * x - 1.0
    } elif (t > 1.0 - dt2) {
        let x = (t - 1.0) / dt2
        return x * x + x + x + 1.0
    }
    return 0.0
}

// ADSR envelope. Returns amplitude 0.0–1.0 for a given time position.
def private adsr(t, attack, decay, sustain, release, duration : float) : float {
    if (t < 0.0) return 0.0
    if (t < attack) return t / attack
    let t2 = t - attack
    if (t2 < decay) return 1.0 - (1.0 - sustain) * (t2 / decay)
    // release starts at note-off (= duration), extends PAST the slot boundary
    // total sound length = duration + release
    if (t < duration) return sustain
    let t3 = t - duration
    if (t3 < release) return sustain * (1.0 - t3 / release)
    return 0.0
}


// ─── Drum Synthesis ───
// Generated from the sfx_lab .sfx sounds by emit_das (examples/daStrudel/sfx_lab). Each builds
// its layer stack + post and calls the sfx engine, conformed to (duration, gain) @ 48 kHz.
// To re-tune a drum: edit its sounds/<name>.sfx in the lab, then File -> Export .das render function.

def render_bd(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("kick body", ModalVoice(pitch = 92.19005, spread = 1, n_modes = 1, ring = 0.10411456, brightness = 0.0003617294, damp_skew = 0.54129374, transient = 0.3057839, transient_rate = 746.62, echo_amount = 0.084564, echo_delay = 0.04126666, duration = 0.62035507))
    l0.gain = 2
    layers |> emplace(l0)
    var l1 = sfxr_layer("noise top", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.8125142, env_attack = 0.19729611, env_sustain = 0, env_punch = 0.019216271, env_decay = 0, base_freq = 0.19114257, freq_limit = 0.008770883, freq_ramp = -0.69378364, freq_dramp = 0.16200626, vib_strength = 0.15152553, vib_speed = 0.27849677, arp_mod = 1, arp_speed = 0.17929535, duty = 0.91913927, duty_ramp = 0.0121489465, repeat_speed = 0.16007021, pha_offset = 0.24446183, pha_ramp = -0.23262447, lpf_freq = 0.40080133, lpf_ramp = 0.58750004, lpf_resonance = 0.27288863, hpf_freq = 0.034394234, hpf_ramp = 0.8949298))
    l1.gain = 0.378
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(reverb = FxReverb(enabled = true, wet = 0.4114426, decay = 1.8850778, lp_start = 10776.892, lp_end = 1141.2772)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.716628)
}

def render_sd(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("snare body", ModalVoice(pitch = 132.6518, spread = 2.5196228, n_modes = 2, ring = 0.041626297, brightness = 0.5621451, damp_skew = 0.91521007, transient = 0.04814431, transient_rate = 1089.8298, echo_delay = 0.016666325, duration = 0.92530364))
    l0.gain = 0.918
    layers |> emplace(l0)
    var l1 = sfxr_layer("snare wires", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.3353221, env_attack = 0.100997776, env_sustain = 0.013236732, env_punch = 0.69624764, env_decay = 0.263, base_freq = 0.32443383, freq_limit = 0.07678777, freq_ramp = 0.26109394, freq_dramp = -0.16964206, vib_strength = 0.19371475, vib_speed = 0.12917374, arp_mod = 0.57104796, arp_speed = 0.3427923, duty = 0.0036578178, duty_ramp = 0.09432789, pha_offset = 0.33797556, pha_ramp = -0.05845314, lpf_freq = 0.9671906, lpf_ramp = -0.098088905, lpf_resonance = 0.07413985, hpf_freq = 0.064527355, hpf_ramp = -0.030188851))
    l1.gain = 0.527
    layers |> emplace(l1)
    var l2 = modal_layer("snare ring", ModalVoice(pitch = 907.405, spread = 1.1649027, n_modes = 3, ring = 0.05589971, brightness = 0.58914346, damp_skew = 1.4808201, transient = 0.49322706, transient_rate = 1017.0395, echo_amount = 0.26977468, echo_delay = 0.09883809, duration = 0.7440903))
    l2.gain = 0.209
    layers |> emplace(l2)
    var out <- render_sound(layers)
    apply_post(out, PostChain(comp = FxComp(enabled = true, threshold = -18.498344, ratio = 16.246166, knee = 15.021414, attack = 0.026926743, release = 0.2628373), reverb = FxReverb(enabled = true, wet = 0.44400942, decay = 0.81689435, lp_start = 2000, lp_end = 5640.131)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.5571)
}

def render_hh(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("body", ModalVoice(pitch = 83.480896, spread = 1.875554, n_modes = 6, ring = 0.13597746, brightness = 0.9672969, damp_skew = 0.46194708, transient = 0.9719281, transient_rate = 1041.986, echo_amount = 0.273, echo_delay = 0.067, duration = 0.57802224))
    l0.gain = 0.391
    layers |> emplace(l0)
    var l1 = sfxr_layer("noise", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.7981047, env_attack = 0.035719402, env_sustain = 0.017502397, env_punch = 0.39985386, env_decay = 0.203, base_freq = 0.6757881, freq_limit = 0.046278007, freq_ramp = 0.809726, freq_dramp = -0.42988, vib_strength = 0.014422701, vib_speed = 0.32112327, arp_mod = 0.011038069, arp_speed = 0.016891815, duty = 0.70870906, duty_ramp = -0.1761131, repeat_speed = 0.13810815, pha_offset = 0.17985052, pha_ramp = 0.6097818, lpf_freq = 0.7834988, lpf_ramp = 0.40151978, lpf_resonance = 0.065687366, hpf_freq = 0.29279754, hpf_ramp = 0.7709224))
    l1.gain = 0.8433876
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(filter = FxFilter(enabled = true, position = 0.8385528), phaser = FxPhaser(enabled = true, rate = 7.753262, depth = 0.23392326, center = 100, sweep = 2185.921), reverb = FxReverb(enabled = true, wet = 0.18446012, decay = 0.8129592, lp_start = 10354.649, lp_end = 2772.1028)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.1005718)
}

def render_oh(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("body", ModalVoice(pitch = 262.051, spread = 1.265, n_modes = 8, ring = 0.16779672, brightness = 1, damp_skew = 1.003, transient = 0.76751196, transient_rate = 587.8487, echo_amount = 0.031908553, echo_delay = 0.117, duration = 0.6184743))
    l0.gain = 0.853
    layers |> emplace(l0)
    var l1 = sfxr_layer("noise", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.6028352, env_attack = 0.06774744, env_sustain = 0.09199185, env_punch = 0.44888717, env_decay = 0.18753469, base_freq = 0.41506052, freq_limit = 0.35775602, freq_ramp = 0.39196032, freq_dramp = -0.20300886, vib_strength = 0.2862102, vib_speed = 0.63817817, arp_mod = -0.36608765, arp_speed = 0.09586534, duty = 0.6536455, duty_ramp = -0.03319117, repeat_speed = 0.12912789, pha_offset = 0.56369853, pha_ramp = 0.1393801, lpf_freq = 0.82449687, lpf_ramp = -0.3242288, lpf_resonance = 0.117011495, hpf_freq = 0.5928842, hpf_ramp = 0.48898622))
    l1.gain = 0.881
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(filter = FxFilter(enabled = true, position = 0.41517052), drive = FxDrive(enabled = true, drive = 0.8361627), phaser = FxPhaser(enabled = true, rate = 5.297644, depth = 0.22305954, center = 576.8732, sweep = 2080.2576), reverb = FxReverb(enabled = true, wet = 0.7004094, decay = 0.9636621, lp_start = 13392, lp_end = 2913.0295)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.0696671)
}

def render_cp(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = sfxr_layer("clap", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.50202537, env_attack = 0.04863984, env_sustain = 0, env_punch = 0.36572674, env_decay = 0.22804458, base_freq = 0.4899924, freq_ramp = -0.10181666, freq_dramp = 0.06617436, vib_strength = 0.011753256, vib_speed = 0.0029255762, arp_mod = 0.25824764, arp_speed = 0.009729162, duty = 0.0016653084, duty_ramp = 0.27126336, repeat_speed = 0.04119529, pha_offset = -0.04200024, pha_ramp = 0.012046436, lpf_ramp = -0.07116463, hpf_freq = 0.044381835, hpf_ramp = 0.05262112))
    layers |> emplace(l0)
    var out <- render_sound(layers)
    apply_post(out, PostChain(bandpass = FxBandpass(enabled = true, freq = 1070.3005, q = 1.7945414), reverb = FxReverb(enabled = true, wet = 0.241, decay = 0.91671735, lp_start = 5777.9414, lp_end = 1906.6681)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.5125848)
}

def render_tom(duration, gain, base_freq : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("body", ModalVoice(pitch = 1 * base_freq, spread = 1.5352107, n_modes = 3, ring = 0.13079655, brightness = 0.223, damp_skew = 0.5194714, transient = 0.44619122, transient_rate = 338.28003, echo_amount = 0.12818907, echo_delay = 0.050528653, duration = 0.6532905))
    l0.gain = 0.374
    layers |> emplace(l0)
    var l1 = sfxr_layer("click", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.68069726, env_attack = 0.017013319, env_sustain = 0, env_punch = 0.31063506, env_decay = 0.122, base_freq = 0.2995053, freq_limit = 0.03445269, freq_ramp = -0.010150269, freq_dramp = 0.116527304, vib_strength = 0.016013723, vib_speed = 0.22915553, arp_mod = 0.45189676, arp_speed = 0.00081056263, duty = 0.22757596, duty_ramp = -0.096907295, repeat_speed = 0.32849845, pha_offset = 0.40356445, pha_ramp = -0.1761775, lpf_freq = 0.39883548, lpf_ramp = 0.2316376, lpf_resonance = 0.067041874, hpf_freq = 0.004074513, hpf_ramp = 0.45968696))
    l1.gain = 0.093
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(comp = FxComp(enabled = true, threshold = -11.062325, ratio = 3.2656035, knee = 12.965193, attack = 0.001, release = 0.07535656), reverb = FxReverb(enabled = true, wet = 0.18924627, decay = 2.7724564, lp_start = 9273.734, lp_end = 574.92474)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.71737325)
}

def render_crash(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("bell", ModalVoice(pitch = 343.965, spread = 1.2489982, n_modes = 2, ring = 0.27278978, brightness = 0.77337474, damp_skew = 0.23000792, transient = 0.45617875, transient_rate = 801.6456, echo_amount = 0.009444692, echo_delay = 0.032232385, duration = 1.7827468))
    l0.gain = 0.419
    layers |> emplace(l0)
    var l1 = sfxr_layer("wash", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.36487252, env_attack = 0.08991354, env_sustain = 0.26309496, env_punch = 0.074205264, env_decay = 0.4370825, base_freq = 0.2836753, freq_limit = 0.038351007, freq_ramp = -0.01767661, freq_dramp = 0.06892679, vib_strength = 0.036719434, vib_speed = 0.028221305, arp_mod = 0.23257561, arp_speed = 0.22157943, duty = 0.09078055, duty_ramp = 0.0073036714, repeat_speed = 0.11616688, pha_offset = -0.10262833, pha_ramp = 0.32091796, lpf_freq = 0.7735541, lpf_ramp = -0.07247121, lpf_resonance = 0.18061708, hpf_freq = 0.46778846, hpf_ramp = 0.17977096))
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(reverb = FxReverb(enabled = true, wet = 0.790473, decay = 2.0615287, lp_start = 5006.515, lp_end = 3199.1077)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.16021894)
}

def render_ride(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("body", ModalVoice(pitch = 260.59442, spread = 1.9402999, n_modes = 6, ring = 0.72374016, brightness = 0.91424954, damp_skew = 0.39226624, transient = 0.9157878, transient_rate = 1428.7157, echo_amount = 0.3951541, echo_delay = 0.12949422, duration = 2.91224))
    l0.gain = 1.732
    layers |> emplace(l0)
    var l1 = sfxr_layer("noise", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.85335475, env_attack = 0.01885143, env_sustain = 0.64399797, env_punch = 0.8266965, env_decay = 0.9358731, base_freq = 0.94963217, freq_limit = 0.16316509, freq_ramp = -0.9105514, freq_dramp = -0.49644652, vib_strength = 0.6346296, vib_speed = 0.037927777, arp_mod = 0.59178555, duty = 0.8468766, duty_ramp = -0.57930696, repeat_speed = 0.080499634, pha_offset = 0.8598438, pha_ramp = 0.07920793, lpf_freq = 0.50055075, lpf_ramp = -0.72643226, lpf_resonance = 0.2777733, hpf_freq = 0.7852516, hpf_ramp = 1))
    l1.gain = 1.254
    layers |> emplace(l1)
    var l2 = modal_layer("discover", ModalVoice(pitch = 375.35406, spread = 1.6241328, n_modes = 3, ring = 0.26532784, brightness = 0.8192871, damp_skew = 0.16306561, transient = 0.64571667, transient_rate = 117.49198, echo_amount = 0.9544771, echo_delay = 0.06452382, duration = 0.17131767))
    l2.gain = 1.23
    layers |> emplace(l2)
    var out <- render_sound(layers)
    apply_post(out, PostChain(phaser = FxPhaser(enabled = true, rate = 3.8757129, depth = 0.22463433, center = 2308.9856, sweep = 1507.2904), tremolo = FxTremolo(enabled = true, rate = 8.172751, depth = 0.89493424), reverb = FxReverb(enabled = true, wet = 0.6909106, decay = 2.96094, lp_start = 15040.056, lp_end = 2945.922)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.26458737)
}

def render_rimshot(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = sfxr_layer("clap", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.50202537, env_attack = 0.046, env_sustain = 0, env_punch = 0.371, env_decay = 0.22804458, base_freq = 0.984, freq_limit = 0.117, freq_ramp = 1, freq_dramp = 0.972, vib_strength = 0.011753256, vib_speed = 0.0029255762, arp_mod = 0.25824764, arp_speed = 0.009729162, duty = 0.0016653084, duty_ramp = 0.27126336, repeat_speed = 0.04119529, pha_offset = -0.04200024, pha_ramp = 0.012046436, lpf_ramp = -0.07116463, hpf_freq = 0.044381835, hpf_ramp = 0.05262112))
    l0.gain = 1.005
    layers |> emplace(l0)
    var out <- render_sound(layers)
    apply_post(out, PostChain(filter = FxFilter(enabled = true, position = 0.869), bandpass = FxBandpass(enabled = true, freq = 118.12814, q = 3.7761607), reverb = FxReverb(enabled = true, wet = 0.258, decay = 0.692, lp_start = 7656.8604, lp_end = 500)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 22.587774)
}

def render_cowbell(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("body", ModalVoice(pitch = 411.399, spread = 1.9724455, n_modes = 6, ring = 0.06443338, brightness = 0.6154117, damp_skew = 0.5370055, transient = 0.99766755, transient_rate = 925.15546, echo_amount = 0.020447377, echo_delay = 0.011550919, duration = 0.82429314))
    layers |> emplace(l0)
    var l1 = sfxr_layer("noise", SfxrSound(wave = SfxrWave.noise, sound_vol = 0.7673511, env_attack = 0.06156981, env_sustain = 0.00881725, env_punch = 0.3309498, env_decay = 0, base_freq = 0.23155648, freq_limit = 0.04794487, freq_ramp = -0.19637704, freq_dramp = 0.049527712, vib_strength = 0.010859551, vib_speed = 0.0027843416, arp_mod = 0.20936257, arp_speed = 0.27919853, duty = 0.117745265, duty_ramp = -0.16573964, repeat_speed = 0.26919103, pha_offset = -0.3462789, pha_ramp = 0.30785185, lpf_freq = 0.7143182, lpf_ramp = -0.21776688, lpf_resonance = 0.00679336, hpf_freq = 0.06575774, hpf_ramp = -0.2670746))
    l1.gain = 0.20742378
    layers |> emplace(l1)
    var out <- render_sound(layers)
    apply_post(out, PostChain(bandpass = FxBandpass(enabled = true, freq = 2241.643, q = 0.6167674), tremolo = FxTremolo(enabled = true, rate = 0.5114572, depth = 0.861706), reverb = FxReverb(enabled = true, wet = 0.74340045, decay = 0.8050676, lp_start = 14859.577, lp_end = 530.3483)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 1.4125484)
}

def render_tambourine(duration, gain : float) : array<float> {
    var layers : array<Layer>
    var l0 = modal_layer("tamb jingle", ModalVoice(pitch = 4000, spread = 1.6022093, n_modes = 6, ring = 0.019, brightness = 0.937, damp_skew = 0.084890395, transient = 0.48216662, transient_rate = 139.9605, echo_amount = 0.42, duration = 1.345))
    l0.gain = 0.972
    layers |> emplace(l0)
    var out <- render_sound(layers)
    apply_post(out, PostChain(bandpass = FxBandpass(enabled = true, freq = 2948.5562, q = 0.7541728), crush = FxCrush(enabled = true, bits = 4.0084352, coarse = 0), reverb = FxReverb(enabled = true, wet = 0.247, decay = 0.992, lp_start = 12394.796, lp_end = 500)), 44100.0)
    return <- sfx_conform(out, 44100, 48000, duration, gain * 0.62163943)
}



// ─── Oscillators ───

//! Oscillator waveform selector for an `OscVoice`.
enum OscType {
    osc_sine
        //! Pure sine wave.
    osc_sawtooth
        //! Band-limited sawtooth (poly-BLEP anti-aliased).
    osc_square
        //! Band-limited square wave (poly-BLEP anti-aliased).
    osc_triangle
        //! Triangle wave derived from the phase.
    osc_supersaw
        //! Unison stack of detuned band-limited sawtooths, stereo-spread.
    osc_pink_noise
        //! Pink noise via Paul Kellet's 7-stage cascaded IIR.
    osc_white_noise
        //! Uniform white noise.
    osc_unknown
        //! Unrecognised sound name — used as a sentinel by `to_osc_type`.
}

def to_osc_type(sound : string) : OscType {
    //! Map a strudel sound name ("sine", "sawtooth", ...) to an `OscType`; unknown names return `osc_unknown`.
    if (sound == "sine") {
        return OscType.osc_sine
    } elif (sound == "sawtooth") {
        return OscType.osc_sawtooth
    } elif (sound == "square") {
        return OscType.osc_square
    } elif (sound == "triangle") {
        return OscType.osc_triangle
    } elif (sound == "supersaw") {
        return OscType.osc_supersaw
    } elif (sound == "pink") {
        return OscType.osc_pink_noise
    } elif (sound == "white") return OscType.osc_white_noise
    return OscType.osc_unknown
}

let private SUPERSAW_VOICES = 5       // default unison voice count
let private SUPERSAW_DETUNE = 0.18    // default frequency spread in semitones

//! Pink noise state — Paul Kellet's 7-stage cascaded IIR filter.
struct PinkNoiseState {
    b0 : float = 0.0
    b1 : float = 0.0
    b2 : float = 0.0
    b3 : float = 0.0
    b4 : float = 0.0
    b5 : float = 0.0
    b6 : float = 0.0
    seed : uint = 54321u
}

def private pink_noise_tick(var st : PinkNoiseState) : float {
    let white = noise(st.seed)
    st.b0 = 0.99886 * st.b0 + white * 0.0555179
    st.b1 = 0.99332 * st.b1 + white * 0.0750759
    st.b2 = 0.969   * st.b2 + white * 0.153852
    st.b3 = 0.8665  * st.b3 + white * 0.3104856
    st.b4 = 0.55    * st.b4 + white * 0.5329522
    st.b5 = -0.7616 * st.b5 - white * 0.016898
    let out = st.b0 + st.b1 + st.b2 + st.b3 + st.b4 + st.b5 + st.b6 + white * 0.5362
    st.b6 = white * 0.115926
    return out * 0.11
}

// ─── Vowel Formant Filter ───
// 5 parallel bandpass filters using standard formant tables.
// Each formant has frequency, gain, and Q. The outputs are summed with a makeup gain of 8.0.

//! General-purpose bandpass biquad (transposed direct form II) with all three numerator coefficients.
//! Unlike `ma_sf2_biquad` (LPF/HPF only, `a2 == a0`), this stores an explicit `a2`, allowing true BPF/BRF shapes.
struct FormantBiquad {
    a0 : double = 0.0lf
        //! Feed-forward coefficient 0.
    a1 : double = 0.0lf
        //! Feed-forward coefficient 1 (0 for BPF).
    a2 : double = 0.0lf
        //! Feed-forward coefficient 2 (`-a0` for BPF).
    b1 : double = 0.0lf
        //! Feedback coefficient 1.
    b2 : double = 0.0lf
        //! Feedback coefficient 2.
    z1 : double = 0.0lf
        //! Internal delay-line state 1.
    z2 : double = 0.0lf
        //! Internal delay-line state 2.
    gain : float = 1.0          // per-formant amplitude scaling
        //! Per-formant amplitude scaling applied to the filter output by the vowel filter.
    active : int = 0
        //! Non-zero when the filter has valid coefficients and should process samples.
}

def formant_biquad_setup_bpf(var bq : FormantBiquad; freq, q, sample_rate : float) {
    //! Compute bandpass coefficients for the given centre frequency and Q.
    //! Deactivates the filter if the cutoff is outside the usable range (near 0 or Nyquist).
    let fc_norm = freq / sample_rate
    if (fc_norm <= 0.001 || fc_norm >= 0.499) {
        bq.active = 0
        return
    }
    bq.active = 1
    let K = tan(3.14159265358979323846lf * double(fc_norm))
    let KK = K * K
    let q_inv = 1.0lf / double(max(q, 0.1))
    let norm = 1.0lf / (1.0lf + K * q_inv + KK)
    bq.a0 = K * q_inv * norm    // BPF numerator
    bq.a1 = 0.0lf               // BPF: middle coeff is 0
    bq.a2 = 0.0lf - bq.a0      // BPF: a2 = -a0
    bq.b1 = 2.0lf * (KK - 1.0lf) * norm
    bq.b2 = (1.0lf - K * q_inv + KK) * norm
    bq.z1 = 0.0lf
    bq.z2 = 0.0lf
}

def formant_biquad_tick(var bq : FormantBiquad; input : float) : float {
    //! Process one sample through the biquad and return the output.
    let inp = double(input)
    let out = inp * bq.a0 + bq.z1
    bq.z1 = inp * bq.a1 + bq.z2 - bq.b1 * out
    bq.z2 = inp * bq.a2 - bq.b2 * out
    return float(out)
}

let VOWEL_NUM_FORMANTS = 5
let VOWEL_MAKEUP_GAIN = 8.0    // makeup gain for vowel formant filter

//! Five parallel bandpass filters that impose a vowel-like formant shape on an input signal.
struct VowelFilter {
    @safe_when_uninitialized formants : FormantBiquad[5]
        //! Five formant bandpass biquads processed in parallel and summed.
    active : bool = false
        //! True once the filter has been set up for a known vowel; otherwise ``tick`` is bypassed by callers.
}

//! Table row of formant parameters for a single vowel (5 formants × freq/gain/Q).
struct FormantData {
    freqs : float[5]
        //! Centre frequency of each formant in Hz.
    gains : float[5]
        //! Linear gain of each formant (formant 0 is usually the loudest).
    qs    : float[5]
        //! Q (resonance) of each formant bandpass.
}

def vowel_get_formant_data(vowel : string; var data : FormantData) : bool {
    //! Look up formant parameters for a vowel name ("a", "e", "i", "o", "u", "ae", "aa", "oe", "ue", "y", "uh", "un", "en", "an", "on").
    //! Returns false if the vowel is unknown.
    if (vowel == "a") {
        data.freqs = fixed_array(660.0, 1120.0, 2750.0, 3000.0, 3350.0)
        data.gains = fixed_array(1.0, 0.5012, 0.0708, 0.0631, 0.0126)
        data.qs    = fixed_array(80.0, 90.0, 120.0, 130.0, 140.0)
    } elif (vowel == "e") {
        data.freqs = fixed_array(440.0, 1800.0, 2700.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.1995, 0.1259, 0.1, 0.1)
        data.qs    = fixed_array(70.0, 80.0, 100.0, 120.0, 120.0)
    } elif (vowel == "i") {
        data.freqs = fixed_array(270.0, 1850.0, 2900.0, 3350.0, 3590.0)
        data.gains = fixed_array(1.0, 0.0631, 0.0631, 0.0158, 0.0158)
        data.qs    = fixed_array(40.0, 90.0, 100.0, 120.0, 120.0)
    } elif (vowel == "o") {
        data.freqs = fixed_array(430.0, 820.0, 2700.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.3162, 0.0501, 0.0794, 0.01995)
        data.qs    = fixed_array(40.0, 80.0, 100.0, 120.0, 120.0)
    } elif (vowel == "u") {
        data.freqs = fixed_array(370.0, 630.0, 2750.0, 3000.0, 3400.0)
        data.gains = fixed_array(1.0, 0.1, 0.0708, 0.0316, 0.01995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "ae") {
        data.freqs = fixed_array(650.0, 1515.0, 2400.0, 3000.0, 3350.0)
        data.gains = fixed_array(1.0, 0.5, 0.1008, 0.0631, 0.0126)
        data.qs    = fixed_array(80.0, 90.0, 120.0, 130.0, 140.0)
    } elif (vowel == "aa") {
        data.freqs = fixed_array(560.0, 900.0, 2570.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.5, 0.0708, 0.0631, 0.0126)
        data.qs    = fixed_array(80.0, 90.0, 120.0, 130.0, 140.0)
    } elif (vowel == "oe") {
        data.freqs = fixed_array(500.0, 1430.0, 2300.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.2, 0.0708, 0.0316, 0.01995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "ue") {
        data.freqs = fixed_array(250.0, 1750.0, 2150.0, 3200.0, 3300.0)
        data.gains = fixed_array(1.0, 0.1, 0.0708, 0.0316, 0.01995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "y") {
        data.freqs = fixed_array(400.0, 1460.0, 2400.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.2, 0.0708, 0.0316, 0.02995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "uh") {
        data.freqs = fixed_array(600.0, 1250.0, 2100.0, 3100.0, 3500.0)
        data.gains = fixed_array(1.0, 0.3, 0.0608, 0.0316, 0.01995)
        data.qs    = fixed_array(40.0, 70.0, 100.0, 120.0, 130.0)
    } elif (vowel == "un") {
        data.freqs = fixed_array(500.0, 1240.0, 2280.0, 3000.0, 3500.0)
        data.gains = fixed_array(1.0, 0.1, 0.1708, 0.0216, 0.02995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "en") {
        data.freqs = fixed_array(600.0, 1480.0, 2450.0, 3200.0, 3300.0)
        data.gains = fixed_array(1.0, 0.15, 0.0708, 0.0316, 0.02995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "an") {
        data.freqs = fixed_array(700.0, 1050.0, 2500.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.1, 0.0708, 0.0316, 0.02995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } elif (vowel == "on") {
        data.freqs = fixed_array(500.0, 1080.0, 2350.0, 3000.0, 3300.0)
        data.gains = fixed_array(1.0, 0.1, 0.0708, 0.0316, 0.02995)
        data.qs    = fixed_array(40.0, 60.0, 100.0, 120.0, 120.0)
    } else {
        return false
    }
    return true
}

def vowel_filter_init(var vf : VowelFilter; vowel : string) {
    //! Configure a VowelFilter for the named vowel, deactivating it if the vowel is unknown.
    var data : FormantData
    if (!vowel_get_formant_data(vowel, data)) {
        vf.active = false
        return
    }
    let sr = float(SAMPLE_RATE)
    for (i in range(VOWEL_NUM_FORMANTS)) {
        formant_biquad_setup_bpf(vf.formants[i], data.freqs[i], data.qs[i], sr)
        vf.formants[i].gain = data.gains[i]
    }
    vf.active = true
}

def vowel_filter_tick(var vf : VowelFilter; input : float) : float {
    //! Process one sample through the vowel filter: sum the 5 parallel formant bandpass outputs and apply the makeup gain.
    var sum = 0.0
    for (i in range(VOWEL_NUM_FORMANTS)) {
        if (vf.formants[i].active != 0) {
            sum += formant_biquad_tick(vf.formants[i], input) * vf.formants[i].gain
        }
    }
    return sum * VOWEL_MAKEUP_GAIN
}

//! Per-voice effect chain: bitcrush, waveshaper, DJ filter, bandpass, phaser, tremolo and compressor.
//! Wraps the C++ `ma_*` effect structs and is initialised from Event fields; a default-constructed
//! VoiceFX with no Event-driven parameters active is a cheap no-op.
[safe_when_uninitialized]
struct VoiceFX {
    @safe_when_uninitialized bc   : ma_bitcrush
        //! Bitcrusher state (bit-depth + sample-rate reduction).
    @safe_when_uninitialized ws   : ma_waveshaper
        //! Waveshaper state (drive-based soft distortion).
    @safe_when_uninitialized dj   : ma_djfilter
        //! DJ filter state (single-knob low-pass/high-pass blend).
    @safe_when_uninitialized bp   : ma_bandpass
        //! Bandpass filter state (centre + Q).
    @safe_when_uninitialized ph   : ma_phaser
        //! Phaser state (rate/depth/centre/sweep).
    @safe_when_uninitialized trem : ma_tremolo
        //! Tremolo state (amplitude modulation rate/depth).
    @safe_when_uninitialized comp : ma_compressor
        //! Compressor state (threshold/ratio/knee/attack/release).
    has_crush : bool = false
        //! Bitcrush is enabled and will be applied to the voice.
    has_shape : bool = false
        //! Waveshaper is enabled.
    has_djf   : bool = false
        //! DJ filter is enabled (position != 0.5).
    has_bpf   : bool = false
        //! Bandpass filter is enabled.
    has_phaser : bool = false
        //! Phaser is enabled.
    has_tremolo : bool = false
        //! Tremolo is enabled.
    has_compressor : bool = false
        //! Compressor is enabled.
    active    : bool = false
        //! True if any of the above effects is enabled; skip `fx_apply` entirely when false.
}

def fx_init_from_event(var fx : VoiceFX; event : Event; sample_rate : float) {
    //! Configure a VoiceFX chain from the relevant Event fields (crush/coarse/shape/djf/bpf/phaser/tremolo/compressor).
    //! Leaves everything disabled when no Event field is active, so the chain can stay at `fx.active == false`.
    if (event.crush > 0.0 || event.coarse > 0) {
        unsafe { bitcrush_init(addr(fx.bc)); }
        fx.bc.bits = event.crush
        fx.bc.coarse = event.coarse
        fx.has_crush = true
    }
    if (event.shape > 0.0) {
        unsafe { waveshaper_init(addr(fx.ws)); }
        fx.ws.drive = event.shape
        fx.has_shape = true
    }
    if (abs(event.djf - 0.5) > 0.001) {
        unsafe { djfilter_init(addr(fx.dj), sample_rate); }
        fx.dj.position = event.djf
        fx.has_djf = true
    }
    if (event.bpf > 0.0) {
        unsafe { bandpass_init(addr(fx.bp), sample_rate); }
        unsafe { bandpass_setup(addr(fx.bp), event.bpf, max(event.bpq, 0.01)); }
        fx.has_bpf = true
    }
    if (event.phaser > 0.0) {
        unsafe { phaser_init(addr(fx.ph), sample_rate); }
        fx.ph.rate = event.phaser
        fx.ph.depth = clamp(event.phaserdepth, 0.0, 1.0)
        fx.ph.center = event.phasercenter
        fx.ph.sweep = event.phasersweep
        fx.has_phaser = true
    }
    if (event.tremolo > 0.0 && event.tremolodepth > 0.0) {
        unsafe { tremolo_init(addr(fx.trem), sample_rate); }
        fx.trem.rate = event.tremolo
        fx.trem.depth = clamp(event.tremolodepth, 0.0, 1.0)
        fx.has_tremolo = true
    }
    if (event.compressor < 0.0) {
        unsafe { compressor_init(addr(fx.comp), sample_rate); }
        fx.comp.threshold_db = event.compressor
        fx.comp.ratio = max(event.compressor_ratio, 1.0)
        fx.comp.knee_db = max(event.compressor_knee, 0.0)
        fx.comp.attack = max(event.compressor_attack, 0.0)
        fx.comp.release = max(event.compressor_release, 0.0)
        fx.has_compressor = true
    }
    fx.active = fx.has_crush || fx.has_shape || fx.has_djf || fx.has_bpf || fx.has_phaser || fx.has_tremolo || fx.has_compressor
}

def fx_apply(var fx : VoiceFX; var buf : array<float>; n_frames : int) {
    //! Apply the enabled effects to an interleaved stereo buffer in place.
    //! Order: crush -> shape -> djfilter -> bandpass -> phaser -> tremolo -> compressor
    //! (phaser/tremolo modulate the finished tone, compressor is last so it sees the post-FX signal).
    if (!fx.active || n_frames <= 0 || length(buf) < n_frames * 2) return
    if (fx.has_crush)      { unsafe { bitcrush_process(addr(fx.bc), addr(buf[0]), n_frames); } }
    if (fx.has_shape)      { unsafe { waveshaper_process(addr(fx.ws), addr(buf[0]), n_frames); } }
    if (fx.has_djf)        { unsafe { djfilter_process(addr(fx.dj), addr(buf[0]), n_frames); } }
    if (fx.has_bpf)        { unsafe { bandpass_process(addr(fx.bp), addr(buf[0]), n_frames); } }
    if (fx.has_phaser)     { unsafe { phaser_process(addr(fx.ph), addr(buf[0]), n_frames); } }
    if (fx.has_tremolo)    { unsafe { tremolo_process(addr(fx.trem), addr(buf[0]), n_frames); } }
    if (fx.has_compressor) { unsafe { compressor_process(addr(fx.comp), addr(buf[0]), n_frames); } }
}

def fx_apply_to_samples(event : Event; var samples : array<float>; sample_rate : float) {
    //! Apply Event-driven effects to a pre-rendered stereo buffer as a one-shot.
    //! Effect state is local to this call, so effects reset every render; used for drum/sample voices that don't stream.
    let n_frames = length(samples) / 2
    if (n_frames <= 0) return
    var fx : VoiceFX
    fx_init_from_event(fx, event, sample_rate)
    fx_apply(fx, samples, n_frames)
}

//! Streaming oscillator voice rendered per audio chunk rather than pre-rendered for the full duration.
//! Holds oscillator state, ADSR, filters, FM, supersaw unison, noise, effect sends, vowel filter and per-voice FX.
struct OscVoice {
    osc_type        : OscType = OscType.osc_sine
        //! Which waveform this voice produces.
    freq            : float = 440.0
        //! Base frequency in Hz (before FM and supersaw detune).
    phase           : float = 0.0       // oscillator phase [0,1)
        //! Main oscillator phase in [0, 1).
    duration        : float = 1.0       // total note duration in seconds
        //! Total note duration in seconds (release stage extends beyond this).
    attack          : float = 0.001
        //! ADSR attack time in seconds.
    decay           : float = 0.05
        //! ADSR decay time in seconds.
    sustain         : float = 0.0
        //! ADSR sustain level in [0, 1].
    release_sec     : float = 0.01
        //! ADSR release time in seconds.
    samples_elapsed : int = 0           // samples rendered so far
        //! Number of samples rendered so far; used to compute the envelope time.
    finished        : bool = false
        //! Set to true when the ADSR has fully decayed; scheduler then retires the voice.
    gain            : float = 1.0       // event.gain * event.velocity
        //! Effective voice gain (event.gain * event.velocity).
    pan             : float = 0.0       // -1=left, 0=center, 1=right
        //! Stereo pan: -1 = full left, 0 = centre, +1 = full right.
    cut             : int = 0           // cut group
        //! Cut group — non-zero values cut off any previous voice with the same cut value.
    offset_frames   : int = 0           // sub-chunk onset delay (frames)
        //! Sub-chunk onset delay in frames, consumed at the start of the first render chunk.
    lpf_biquad      : ma_sf2_biquad     // low-pass filter state
        //! Low-pass biquad filter state (initialised by `osc_voice_init_filters`).
    hpf_biquad      : ma_sf2_biquad     // high-pass filter state
        //! High-pass biquad filter state.
    // FM synthesis state
    fm_depth        : float = 0.0       // FM modulation index (0 = off)
        //! FM modulation index (0 disables FM).
    fm_harmonicity  : float = 1.0       // modulator freq = carrier freq * harmonicity
        //! Ratio of modulator frequency to carrier frequency.
    fm_mod_phase    : float = 0.0       // modulator oscillator phase [0,1)
        //! FM modulator oscillator phase in [0, 1).
    // supersaw state — 7 detuned sawtooth phases
    supersaw_phases : float[7]
        //! Independent phases for each detuned sawtooth voice in the supersaw stack.
    // noise state
    @safe_when_uninitialized pink : PinkNoiseState
        //! Pink-noise generator state (Paul Kellet's 7-stage IIR).
    white_seed : uint = 12345u
        //! LCG seed used by the white-noise generator.
    // reverb send
    reverb_send : float = 0.0   // reverb send amount from Event.room (0.0–1.0)
        //! Send amount into the reverb bus (driven by Event.room).
    // chorus send
    chorus_send : float = 0.0   // chorus send amount from Event.chorus (0.0–1.0)
        //! Send amount into the chorus bus (driven by Event.chorus).
    // delay send
    delay_send : float = 0.0    // delay send amount from Event.delay_amount (0.0–1.0)
        //! Send amount into the delay bus (driven by Event.delay_amount).
    // orbit
    orbit : int = 0             // effect bus routing
        //! Orbit index used by the scheduler to route this voice into a shared effect bus.
    // vowel formant filter
    @safe_when_uninitialized vowel_filter : VowelFilter
        //! Optional vowel formant filter applied after the biquad filters.
    // per-voice audio effects (crush/coarse/shape/djf/bpf/bpq)
    @safe_when_uninitialized fx : VoiceFX
        //! Per-voice FX chain (bitcrush/waveshaper/DJ filter/bandpass/phaser/tremolo/compressor).
    // HRTF positional override (when active, places this voice in 3D via virtual-stereo HRTF;
    // L/R channels become virtual sources at azimuth -/+ 30°, each through its own HRTF instance).
    hrtf_active    : bool  = false
        //! True when this voice uses HRTF positioning. Pan still controls stereo width / image.
    hrtf_azimuth   : float = 0.0
        //! HRTF azimuth in degrees (-180..180; 0 = ahead).
    hrtf_elevation : float = 0.0
        //! HRTF elevation in degrees (-90..90; 0 = horizontal).
    hrtf_l         : ma_hrtf?
        //! HRTF state for the left input channel (virtual source at azimuth - 30°); allocated lazily.
    hrtf_r         : ma_hrtf?
        //! HRTF state for the right input channel (virtual source at azimuth + 30°); allocated lazily.
}

def osc_voice_init_supersaw(var voice : OscVoice) {
    //! Seed the supersaw unison phases with randomised offsets so the detuned voices don't start in phase.
    var seed = 99999u
    for (i in range(SUPERSAW_VOICES)) {
        voice.supersaw_phases[i] = (noise(seed) * 0.5 + 0.5)  // [0,1)
    }
}

def osc_voice_init_filters(var voice : OscVoice; lpf, hpf, lpq, hpq : float) {
    //! Configure the voice's low-pass and high-pass biquads from Event lpf/hpf cutoffs and lpq/hpq resonances.
    //! Cutoffs <= 0 or >= Nyquist leave the corresponding filter inactive.
    let sr = float(SAMPLE_RATE)
    // LPF: active if cutoff > 0 and below Nyquist
    if (lpf > 0.0 && lpf < sr * 0.5) {
        voice.lpf_biquad.q_inv = 1.0lf / double(max(lpq, 0.1))
        unsafe { ma_sf2_biquad_setup(addr(voice.lpf_biquad), lpf / sr); }
    }
    // HPF: proper high-pass biquad coefficients
    if (hpf > 0.0 && hpf < sr * 0.5) {
        voice.hpf_biquad.q_inv = 1.0lf / double(max(hpq, 0.1))
        unsafe { ma_sf2_biquad_setup_hpf(addr(voice.hpf_biquad), hpf / sr); }
    }
}

// ─── Per-oscillator-type render loops ───
// Each function is a tight loop with no osc_type branching.
// Common invariants are hoisted; FM, filter, and send checks remain
// (they're simple booleans — acceptable for the interpreter).

def private osc_chunk_sine(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var fmOffset = 0.0
        if (hasFm) {
            fmOffset = sin(voice.fm_mod_phase * tp) * fmScale
            voice.fm_mod_phase += fmModPhaseInc
            voice.fm_mod_phase -= floor(voice.fm_mod_phase)
        }
        var sample = sin((voice.phase + fmOffset) * tp) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_sawtooth(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var fmOffset = 0.0
        if (hasFm) {
            fmOffset = sin(voice.fm_mod_phase * tp) * fmScale
            voice.fm_mod_phase += fmModPhaseInc
            voice.fm_mod_phase -= floor(voice.fm_mod_phase)
        }
        var p = voice.phase + fmOffset
        p -= floor(p)
        // Band-limited sawtooth via polyBLEP to suppress aliasing above Nyquist.
        var sample = (p * 2.0 - 1.0 - poly_blep(p, phaseInc)) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_square(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var fmOffset = 0.0
        if (hasFm) {
            fmOffset = sin(voice.fm_mod_phase * tp) * fmScale
            voice.fm_mod_phase += fmModPhaseInc
            voice.fm_mod_phase -= floor(voice.fm_mod_phase)
        }
        var p = voice.phase + fmOffset
        p -= floor(p)
        // Band-limited square: subtract BLEP at rising edge (p=0), add BLEP at falling edge (p=0.5).
        var p_half = p + 0.5
        p_half -= floor(p_half)
        var sample = ((p < 0.5 ? 1.0 : -1.0) - poly_blep(p, phaseInc) + poly_blep(p_half, phaseInc)) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_triangle(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var fmOffset = 0.0
        if (hasFm) {
            fmOffset = sin(voice.fm_mod_phase * tp) * fmScale
            voice.fm_mod_phase += fmModPhaseInc
            voice.fm_mod_phase -= floor(voice.fm_mod_phase)
        }
        var p = voice.phase + fmOffset
        p -= floor(p)
        var sample = (abs(p * 4.0 - 2.0) - 1.0) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_supersaw(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    let gainAdj = 1.0 / sqrt(float(SUPERSAW_VOICES))
    let filterScale = 2.0 * float(SUPERSAW_VOICES) / float(SUPERSAW_VOICES + 1)
    // precompute per-voice phase increments (detune is constant)
    var voicePhaseInc : float[7]
    for (v in range(SUPERSAW_VOICES)) {
        let detune = float(v) * SUPERSAW_DETUNE / float(SUPERSAW_VOICES - 1) - SUPERSAW_DETUNE * 0.5
        voicePhaseInc[v] = voice.freq * pow(2.0, detune / 12.0) * iSr
    }
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var fmOffset = 0.0
        if (hasFm) {
            fmOffset = sin(voice.fm_mod_phase * tp) * fmScale
            voice.fm_mod_phase += fmModPhaseInc
            voice.fm_mod_phase -= floor(voice.fm_mod_phase)
        }
        var mixL = 0.0
        var mixR = 0.0
        for (v in range(SUPERSAW_VOICES)) {
            var p = voice.supersaw_phases[v] + fmOffset
            p -= floor(p)
            // polyBLEP-corrected sawtooth per detuned voice
            let saw = (p * 2.0 - 1.0 - poly_blep(p, voicePhaseInc[v])) * gainAdj
            if (v % 2 == 0) { mixL += saw; } else { mixR += saw; }
            voice.supersaw_phases[v] += voicePhaseInc[v]
            voice.supersaw_phases[v] -= floor(voice.supersaw_phases[v])
        }
        var sampleL = mixL * env
        var sampleR = mixR * env
        if (hasLpf) {
            var filtered : float
            unsafe { filtered = ma_sf2_biquad_tick(addr(voice.lpf_biquad), (sampleL + sampleR) * 0.5); }
            sampleL = filtered * filterScale; sampleR = filtered * filterScale
        }
        if (hasHpf) {
            var filtered : float
            unsafe { filtered = ma_sf2_biquad_tick(addr(voice.hpf_biquad), (sampleL + sampleR) * 0.5); }
            sampleL = filtered * filterScale; sampleR = filtered * filterScale
        }
        if (hasVowel) {
            let filtered = vowel_filter_tick(voice.vowel_filter, (sampleL + sampleR) * 0.5)
            sampleL = filtered; sampleR = filtered
        }
        output[f * 2]     += sampleL * lGain
        output[f * 2 + 1] += sampleR * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sampleL * sendL; reverb_send_buf[f * 2 + 1] += sampleR * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sampleL * delaySendL; delay_send_buf[f * 2 + 1] += sampleR * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sampleL * chorusSendL; chorus_send_buf[f * 2 + 1] += sampleR * chorusSendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_pink_noise(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel : bool) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var sample = pink_noise_tick(voice.pink) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.samples_elapsed ++
    }
}

def private osc_chunk_white_noise(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; startFrame, chunkFrames : int; iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR : float; hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel : bool) {
    let att = voice.attack; let dec = voice.decay; let sus = voice.sustain
    let rel = voice.release_sec; let dur = voice.duration
    for (f in range(startFrame, chunkFrames)) {
        let t = float(voice.samples_elapsed) * iSr
        let env = adsr(t, att, dec, sus, rel, dur)
        if (env <= 0.001 && t >= dur) { voice.finished = true; break; }
        var sample = noise(voice.white_seed) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        if (hasChorusSend) { chorus_send_buf[f * 2] += sample * chorusSendL; chorus_send_buf[f * 2 + 1] += sample * chorusSendR; }
        voice.samples_elapsed ++
    }
}

def osc_render_chunk(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; var chorus_send_buf : array<float>; chunkFrames : int) {
    //! Render one chunk of the oscillator voice additively into the output buffer and the reverb/delay/chorus send buffers.
    //! Hoists invariants out of the inner loop and dispatches to the per-oscillator-type render function.
    // consume sub-chunk onset offset
    if (voice.offset_frames >= chunkFrames) {
        voice.offset_frames -= chunkFrames
        return
    }
    let startFrame = voice.offset_frames
    voice.offset_frames = 0
    // hoist invariants
    let iSr = 1.0 / float(SAMPLE_RATE)
    let phaseInc = voice.freq * iSr
    let tp = TWO_PI
    let pan = clamp(voice.pan, -1.0, 1.0)
    let angle = (pan + 1.0) * 0.25 * (TWO_PI / 2.0)
    let lGain = voice.gain * OSC_TURN_DOWN * cos(angle)
    let rGain = voice.gain * OSC_TURN_DOWN * sin(angle)
    let hasSend = voice.reverb_send > 0.0 && length(reverb_send_buf) >= chunkFrames * 2
    let sendL = lGain * voice.reverb_send
    let sendR = rGain * voice.reverb_send
    let hasDelaySend = voice.delay_send > 0.0 && length(delay_send_buf) >= chunkFrames * 2
    let delaySendL = lGain * voice.delay_send
    let delaySendR = rGain * voice.delay_send
    let hasChorusSend = voice.chorus_send > 0.0 && length(chorus_send_buf) >= chunkFrames * 2
    let chorusSendL = lGain * voice.chorus_send
    let chorusSendR = rGain * voice.chorus_send
    let hasLpf = voice.lpf_biquad.active != 0
    let hasHpf = voice.hpf_biquad.active != 0
    let hasVowel = voice.vowel_filter.active
    let hasFm = voice.fm_depth > 0.0
    let fmScale = voice.fm_depth / tp
    let fmModPhaseInc = voice.freq * voice.fm_harmonicity * iSr
    if (voice.osc_type == OscType.osc_sine) {
        osc_chunk_sine(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_sawtooth) {
        osc_chunk_sawtooth(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_square) {
        osc_chunk_square(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_triangle) {
        osc_chunk_triangle(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_supersaw) {
        osc_chunk_supersaw(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_pink_noise) {
        osc_chunk_pink_noise(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel)
    } elif (voice.osc_type == OscType.osc_white_noise) {
        osc_chunk_white_noise(voice, output, reverb_send_buf, delay_send_buf, chorus_send_buf, startFrame, chunkFrames, iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, chorusSendL, chorusSendR, hasSend, hasDelaySend, hasChorusSend, hasLpf, hasHpf, hasVowel)
    }
}

// ─── Main render entry point ───

def render_event_stereo(event : Event; duration_sec : float) : array<float> {
    //! Render an Event into an interleaved stereo PCM buffer using the built-in drum synths.
    //! Returns raw audio with no gain/pan applied — the scheduler's `ma_volume_mixer` applies those during the mix pass.
    let s = event.s
    var mono : array<float>
    // drum sounds have fixed durations, rendered at unit gain
    if (s == "bd") {
        mono <- render_bd(duration_sec, 1.0)
    } elif (s == "sd") {
        mono <- render_sd(duration_sec, 1.0)
    } elif (s == "hh" || s == "hh_pedal") {
        mono <- render_hh(duration_sec, 1.0)
    } elif (s == "oh" || s == "hh_open") {
        mono <- render_oh(duration_sec, 1.0)
    } elif (s == "cp") {
        mono <- render_cp(duration_sec, 1.0)
    } elif (s == "tom_low") {
        mono <- render_tom(duration_sec, 1.0, 80.0)
    } elif (s == "tom_high") {
        mono <- render_tom(duration_sec, 1.0, 175.0)
    } elif (s == "ride" || s == "ride_bell") {
        mono <- render_ride(duration_sec, 1.0)
    } elif (s == "crash") {
        mono <- render_crash(duration_sec, 1.0)
    } elif (s == "sidestick") {
        mono <- render_rimshot(duration_sec, 1.0)
    } elif (s == "cowbell" || s == "cb") {
        mono <- render_cowbell(duration_sec, 1.0)
    } elif (s == "tambourine" || s == "tamb") {
        mono <- render_tambourine(duration_sec, 1.0)
    } else {
        mono |> resize(int(duration_sec * float(SAMPLE_RATE)))
    }
    return <- mono_to_stereo(mono)
}

def render_event_stereo(event : Event; duration_sec : float; bank : SampleBank) : array<float> {
    //! Render an Event into stereo PCM, preferring a sample from the bank when present and falling back to synthesis.
    //! Returns raw stereo — gain and pan are applied later by the scheduler.
    if (key_exists(bank.sounds, event.s)) {
        var samples <- render_sample(bank, event.s, event.n, event.speed, event.begin, event.end_pos)
        if (!empty(samples)) return <- samples
    }
    return <- render_event_stereo(event, duration_sec)
}

def mono_to_stereo(var mono : array<float>) : array<float> {
    //! Upmix a mono buffer to interleaved stereo by duplicating each sample to both channels.
    //! Consumes `mono` (moves and frees it).
    var stereo : array<float>
    let n = length(mono)
    stereo |> resize(n * 2)
    for (i in range(n)) {
        stereo[i * 2] = mono[i]
        stereo[i * 2 + 1] = mono[i]
    }
    delete mono
    return <- stereo
}
