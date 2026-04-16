options gen2
options indenting = 4

module strudel_synth shared

require strudel/strudel_event
require strudel/strudel_samples
require audio
require math

let SAMPLE_RATE = 48000
let TWO_PI = 3.14159265358979323846 * 2.0
let OSC_TURN_DOWN = 0.3  // oscillator turn-down — only oscillators, not samples/drums

// Convert MIDI note number to frequency in Hz.
// 60 = middle C = 261.63 Hz
def note_to_freq(note : float) : float {
    return 440.0 * pow(2.0, (note - 69.0) / 12.0)
}

// Simple pseudo-random noise from a seed. Returns -1.0 to 1.0.
def noise(var seed : uint&) : float {
    seed = seed * 1664525u + 1013904223u
    return float(seed >> 9u & 0x7FFFFFu) / float(0x3FFFFFu) - 1.0
}

// ADSR envelope. Returns amplitude 0.0–1.0 for a given time position.
def adsr(t, attack, decay, sustain, release, duration : float) : float {
    if (t < 0.0) {
        return 0.0
    }
    if (t < attack) {
        return t / attack
    }
    let t2 = t - attack
    if (t2 < decay) {
        return 1.0 - (1.0 - sustain) * (t2 / decay)
    }
    // release starts at note-off (= duration), extends PAST the slot boundary
    // total sound length = duration + release
    if (t < duration) {
        return sustain
    }
    let t3 = t - duration
    if (t3 < release) {
        return sustain * (1.0 - t3 / release)
    }
    return 0.0
}

// ─── Drum Synthesis ───

// Kick drum: sine with pitch sweep 150Hz→55Hz, fast exponential decay
def render_bd(duration : float; gain : float) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env = exp(-t * 15.0)
        let freq = 55.0 + 95.0 * exp(-t * 30.0)
        phase += tp * freq * iSr
        samples[i] = sin(phase) * env * gain
    }
    return <- samples
}

// Snare: noise burst + sine at 180Hz, medium decay
def render_sd(duration : float; gain : float) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase = 0.0
    var seed = 12345u
    let iSr = 1.0 / float(SAMPLE_RATE)
    let phaseInc = float(TWO_PI) * 180.0 * iSr
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env = exp(-t * 20.0)
        let tone = sin(phase) * 0.4
        phase += phaseInc
        let n = noise(seed) * 0.6
        samples[i] = (tone + n) * env * gain
    }
    return <- samples
}

// Hi-hat: high-pass noise, very short decay
def render_hh(duration : float; gain : float) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var seed = 67890u
    var prev = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env = exp(-t * 60.0)
        let n = noise(seed)
        let hp = n - prev
        prev = n
        samples[i] = hp * env * gain
    }
    return <- samples
}

// Clap: noise burst with slight resonance, short decay
def render_cp(duration : float; gain : float) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var seed = 11111u
    let iSr = 1.0 / float(SAMPLE_RATE)
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env1 = exp(-t * 40.0)
        let env2 = exp(-t * 15.0) * 0.7
        let env = max(env1, env2)
        samples[i] = noise(seed) * env * gain
    }
    return <- samples
}

// ─── Oscillators ───

enum OscType {
    osc_sine
    osc_sawtooth
    osc_square
    osc_triangle
    osc_supersaw
    osc_pink_noise
    osc_white_noise
    osc_unknown
}

def to_osc_type(sound : string) : OscType {
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
    } elif (sound == "white") {
        return OscType.osc_white_noise
    }
    return OscType.osc_unknown
}

let SUPERSAW_VOICES = 7
let SUPERSAW_DETUNE = 0.2  // detune spread in semitones (+/- from center)

// Pink noise state — Paul Kellet's 7-stage cascaded IIR filter
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

def pink_noise_tick(var st : PinkNoiseState) : float {
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

// Bandpass biquad filter (transposed direct form II, 3 numerator coefficients).
// Unlike ma_sf2_biquad which hardcodes a2=a0 (LPF/HPF only), this has explicit a2.
struct FormantBiquad {
    a0 : double = 0.0lf
    a1 : double = 0.0lf
    a2 : double = 0.0lf
    b1 : double = 0.0lf
    b2 : double = 0.0lf
    z1 : double = 0.0lf
    z2 : double = 0.0lf
    gain : float = 1.0          // per-formant amplitude scaling
    active : int = 0
}

def formant_biquad_setup_bpf(var bq : FormantBiquad; freq, q, sample_rate : float) {
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
    let inp = double(input)
    let out = inp * bq.a0 + bq.z1
    bq.z1 = inp * bq.a1 + bq.z2 - bq.b1 * out
    bq.z2 = inp * bq.a2 - bq.b2 * out
    return float(out)
}

let VOWEL_NUM_FORMANTS = 5
let VOWEL_MAKEUP_GAIN = 8.0    // makeup gain for vowel formant filter

struct VowelFilter {
    @safe_when_uninitialized formants : FormantBiquad[5]
    active : bool = false
}

// Formant data: [freq, gain, Q] per formant, 5 formants per vowel.
// Standard formant tables.
struct FormantData {
    freqs : float[5]
    gains : float[5]
    qs    : float[5]
}

def vowel_get_formant_data(vowel : string; var data : FormantData) : bool {
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

// Apply vowel filter: 5 parallel bandpass filters, sum outputs, scale by makeup gain.
def vowel_filter_tick(var vf : VowelFilter; input : float) : float {
    var sum = 0.0
    for (i in range(VOWEL_NUM_FORMANTS)) {
        if (vf.formants[i].active != 0) {
            sum += formant_biquad_tick(vf.formants[i], input) * vf.formants[i].gain
        }
    }
    return sum * VOWEL_MAKEUP_GAIN
}

// Streaming oscillator voice — renders per-chunk instead of pre-rendering the full duration.
struct OscVoice {
    osc_type        : OscType = OscType.osc_sine
    freq            : float = 440.0
    phase           : float = 0.0       // oscillator phase [0,1)
    duration        : float = 1.0       // total note duration in seconds
    attack          : float = 0.001
    decay           : float = 0.05
    sustain         : float = 0.0
    release_sec     : float = 0.01
    samples_elapsed : int = 0           // samples rendered so far
    finished        : bool = false
    gain            : float = 1.0       // event.gain * event.velocity
    pan             : float = 0.0       // -1=left, 0=center, 1=right
    cut             : int = 0           // cut group
    offset_frames   : int = 0           // sub-chunk onset delay (frames)
    lpf_biquad      : ma_sf2_biquad     // low-pass filter state
    hpf_biquad      : ma_sf2_biquad     // high-pass filter state
    // FM synthesis state
    fm_depth        : float = 0.0       // FM modulation index (0 = off)
    fm_harmonicity  : float = 1.0       // modulator freq = carrier freq * harmonicity
    fm_mod_phase    : float = 0.0       // modulator oscillator phase [0,1)
    // supersaw state — 7 detuned sawtooth phases
    supersaw_phases : float[7]
    // noise state
    @safe_when_uninitialized pink : PinkNoiseState
    white_seed : uint = 12345u
    // reverb send
    reverb_send : float = 0.0   // reverb send amount from Event.room (0.0–1.0)
    // delay send
    delay_send : float = 0.0    // delay send amount from Event.delay_amount (0.0–1.0)
    // orbit
    orbit : int = 0             // effect bus routing
    // vowel formant filter
    @safe_when_uninitialized vowel_filter : VowelFilter
}

// Initialize supersaw phases with random offsets for natural unison spread.
def osc_voice_init_supersaw(var voice : OscVoice) {
    var seed = 99999u
    for (i in range(SUPERSAW_VOICES)) {
        voice.supersaw_phases[i] = float(noise(seed) * 0.5 + 0.5)  // [0,1)
    }
}

// Initialize OscVoice filter state from Event lpf/hpf/lpq/hpq values.
def osc_voice_init_filters(var voice : OscVoice; lpf, hpf, lpq, hpq : float) {
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

def private osc_chunk_sine(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
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
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_sawtooth(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
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
        var sample = (p * 2.0 - 1.0) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_square(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
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
        var sample = (p < 0.5 ? 1.0 : -1.0) * env
        if (hasLpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.lpf_biquad), sample); } }
        if (hasHpf) { unsafe { sample = ma_sf2_biquad_tick(addr(voice.hpf_biquad), sample); } }
        if (hasVowel) { sample = vowel_filter_tick(voice.vowel_filter, sample); }
        output[f * 2]     += sample * lGain
        output[f * 2 + 1] += sample * rGain
        if (hasSend) { reverb_send_buf[f * 2] += sample * sendL; reverb_send_buf[f * 2 + 1] += sample * sendR; }
        if (hasDelaySend) { delay_send_buf[f * 2] += sample * delaySendL; delay_send_buf[f * 2 + 1] += sample * delaySendR; }
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_triangle(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
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
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_supersaw(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm : bool; fmScale, fmModPhaseInc : float) {
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
            let saw = (p * 2.0 - 1.0) * gainAdj
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
        voice.phase += phaseInc; voice.phase -= floor(voice.phase)
        voice.samples_elapsed ++
    }
}

def private osc_chunk_pink_noise(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel : bool) {
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
        voice.samples_elapsed ++
    }
}

def private osc_chunk_white_noise(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; startFrame, chunkFrames : int; iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR : float; hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel : bool) {
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
        voice.samples_elapsed ++
    }
}

// Render one chunk of an oscillator voice directly into the output buffer (additive).
// Dispatches to per-type loop with invariants hoisted out.
def osc_render_chunk(var voice : OscVoice; var output : array<float>; var reverb_send_buf : array<float>; var delay_send_buf : array<float>; chunkFrames : int) {
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
    let tp = float(TWO_PI)
    let pan = clamp(voice.pan, -1.0, 1.0)
    let angle = (pan + 1.0) * 0.25 * float(TWO_PI / 2.0)
    let lGain = voice.gain * OSC_TURN_DOWN * cos(angle)
    let rGain = voice.gain * OSC_TURN_DOWN * sin(angle)
    let hasSend = voice.reverb_send > 0.0 && length(reverb_send_buf) >= chunkFrames * 2
    let sendL = lGain * voice.reverb_send
    let sendR = rGain * voice.reverb_send
    let hasDelaySend = voice.delay_send > 0.0 && length(delay_send_buf) >= chunkFrames * 2
    let delaySendL = lGain * voice.delay_send
    let delaySendR = rGain * voice.delay_send
    let hasLpf = voice.lpf_biquad.active != 0
    let hasHpf = voice.hpf_biquad.active != 0
    let hasVowel = voice.vowel_filter.active
    let hasFm = voice.fm_depth > 0.0
    let fmScale = voice.fm_depth / tp
    let fmModPhaseInc = voice.freq * voice.fm_harmonicity * iSr
    if (voice.osc_type == OscType.osc_sine) {
        osc_chunk_sine(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_sawtooth) {
        osc_chunk_sawtooth(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_square) {
        osc_chunk_square(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_triangle) {
        osc_chunk_triangle(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_supersaw) {
        osc_chunk_supersaw(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, phaseInc, tp, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel, hasFm, fmScale, fmModPhaseInc)
    } elif (voice.osc_type == OscType.osc_pink_noise) {
        osc_chunk_pink_noise(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel)
    } elif (voice.osc_type == OscType.osc_white_noise) {
        osc_chunk_white_noise(voice, output, reverb_send_buf, delay_send_buf, startFrame, chunkFrames, iSr, lGain, rGain, sendL, sendR, delaySendL, delaySendR, hasSend, hasDelaySend, hasLpf, hasHpf, hasVowel)
    }
}

// ─── Main render entry point ───

// Render an Event into interleaved stereo PCM.
// Returns raw audio — no gain or pan applied. The scheduler's
// ma_volume_mixer handles gain and pan during the mix pass.
def render_event_stereo(event : Event; duration_sec : float) : array<float> {
    let s = event.s
    var mono : array<float>
    // drum sounds have fixed durations, rendered at unit gain
    if (s == "bd") {
        mono <- render_bd(duration_sec, 1.0)
    } elif (s == "sd") {
        mono <- render_sd(duration_sec, 1.0)
    } elif (s == "hh") {
        mono <- render_hh(duration_sec, 1.0)
    } elif (s == "cp") {
        mono <- render_cp(duration_sec, 1.0)
    } else {
        mono |> resize(int(duration_sec * float(SAMPLE_RATE)))
    }
    return <- mono_to_stereo(mono)
}

// Render with sample bank — tries bank first, falls back to synthesis.
// Returns raw stereo — no gain or pan.
def render_event_stereo(event : Event; duration_sec : float; bank : SampleBank) : array<float> {
    if (key_exists(bank.sounds, event.s)) {
        var samples <- render_sample(bank, event.s, event.n, event.speed)
        if (length(samples) > 0) {
            return <- samples
        }
    }
    return <- render_event_stereo(event, duration_sec)
}

// Convert mono buffer to interleaved stereo (duplicate to both channels).
def mono_to_stereo(var mono : array<float>) : array<float> {
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
