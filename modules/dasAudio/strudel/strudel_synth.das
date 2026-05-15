options gen2
options indenting = 4

module strudel_synth shared public
//! Audio synthesis and effects for the strudel pattern engine.
//! Houses oscillators (sine/saw/square/triangle/supersaw), drum voices (bd/sd/hh/tom/ride/...),
//! biquad/formant filters, the per-voice FX chain and the top-level `render_event_stereo` entry point.

require strudel/strudel_event
require strudel/strudel_samples
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

// ─── Drum Room (convolution reverb baked into rendered buffer) ───

// Apply convolution reverb to a mono buffer. amount=0.0 is dry, 1.0 is full wet.
// Uses a synthetic impulse response (noise with exponential decay + lowpass sweep).
def private apply_room(var samples : array<float>; amount : float = 0.3, decay_sec : float = 0.4) {
    if (amount <= 0.0) return
    let n = length(samples)
    // Create a short convolution reverb for the drum room
    // decay=0.4s gives a nice small room, lpFreqStart=12kHz→lpFreqEnd=2kHz darkens the tail
    var rev = new ConvolutionReverb
    conv_reverb_init(rev, SAMPLE_RATE, decay_sec, 12000.0, 2000.0, 0.005)
    // Process mono → mono
    var wet : array<float>
    wet |> resize(n)
    unsafe {
        conv_reverb_process_mono(rev, addr(samples[0]), addr(wet[0]), n)
    }
    conv_reverb_uninit(rev)
    unsafe { delete rev; }
    // Mix wet into dry
    for (i in range(n)) {
        samples[i] += wet[i] * amount
    }
    delete wet
}

// Add a single slapback echo at a given delay (in seconds) with given gain.
def private apply_slapback(var samples : array<float>; delay_sec : float = 0.035; echo_gain : float = 0.4) {
    let delay_samples = int(delay_sec * float(SAMPLE_RATE))
    let n = length(samples)
    for (i in range(delay_samples, n)) {
        samples[i] += samples[i - delay_samples] * echo_gain
    }
}

// ─── Drum Synthesis ───

def render_bd(duration : float; gain : float) : array<float> {
    //! Render an 808-style kick drum as a mono buffer at SAMPLE_RATE.
    //! Combines a pitched sine body with a fast pitch sweep, a bandpass beater click and a rimshot-like snap, then applies a short room.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase = 0.0
    var seed = 22222u
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    // bandpass for subtle beater click
    var attack_bp = BiquadState()
    biquad_bandpass(attack_bp, 1500.0, 1.5, float(SAMPLE_RATE))
    // rimshot-style snap for beater attack
    var snap_bp = BiquadState()
    biquad_bandpass(snap_bp, 800.0, 0.8, float(SAMPLE_RATE))
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // --- body: sine with very fast pitch sweep ---
        let freq = 58.0 + 80.0 * exp(-t * 300.0)
        phase += tp * freq * iSr
        let osc = sin(phase)
        // clean sine body
        let body = osc * exp(-t * 10.0) * 1.1
        // --- beater click at 1.5kHz ---
        let n = noise(seed)
        let click = biquad_process(attack_bp, n) * exp(-t * 250.0) * 0.25
        // --- rimshot-style snap, a third longer than actual rimshot ---
        let impulse = biquad_process(snap_bp, n) * exp(-t * 40.0) * 1.05
        // combine
        samples[i] = (body + click + impulse) * gain
    }
    apply_room(samples, 0.15)
    return <- samples
}

def render_sd(duration : float; gain : float) : array<float> {
    //! Render a snare drum as a mono buffer: tonal body (~220/330 Hz) plus high-passed noise for the wires, with a short room tail.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase = 0.0
    var seed = 12345u
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    // snare wires: HPF at 1.5kHz to get the sizzle
    var wire_hpf = BiquadState()
    biquad_highpass(wire_hpf, 1500.0, float(SAMPLE_RATE))
    var phase2 = 0.0
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // two tonal bodies — batter head (~220Hz) and resonant head (~330Hz)
        let freq1 = 220.0 + 60.0 * exp(-t * 200.0)
        phase += tp * freq1 * iSr
        let freq2 = 330.0 + 40.0 * exp(-t * 200.0)
        phase2 += tp * freq2 * iSr
        let tone = (sin(phase) * 0.5 + sin(phase2) * 0.1) * exp(-t * 12.0)
        // snare wires: HPF'd noise, longer decay than tone
        let n = noise(seed)
        let wires = biquad_process(wire_hpf, n) * exp(-t * 10.0) * 0.5
        // initial stick hit: broadband impulse
        let hit = n * exp(-t * 150.0) * 0.4
        samples[i] = (tone + wires + hit) * gain
    }
    apply_room(samples, 0.05, 0.5)
    return <- samples
}

def render_hh(duration : float; gain : float) : array<float> {
    //! Render a closed hi-hat: metallic oscillator bank layered with a short tonal bell (~180 Hz), plus room.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples <- render_metallic(duration, gain * 0.7, 18.0)
    // add tonal component (~200Hz) like SF2 — the "bell" of the hi-hat
    var phase = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    let inc = tp * 180.0 * iSr
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        phase += inc
        samples[i] += sin(phase) * exp(-t * 30.0) * gain * 0.25
    }
    apply_room(samples, 0.1)
    return <- samples
}

def render_cp(duration : float; gain : float) : array<float> {
    //! Render a hand-clap: a sharp bandpassed noise burst (~1.1 kHz) with a metallic bright edge and a long room tail.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var seed = 11111u
    let iSr = 1.0 / float(SAMPLE_RATE)
    var bpf = BiquadState()
    biquad_bandpass(bpf, 1100.0, 2.0, float(SAMPLE_RATE))
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // sharp attack + medium decay
        let env = exp(-t * 29.0)
        let n = noise(seed)
        let main = biquad_process(bpf, n) * 2.8
        let bright = n * exp(-t * 40.0) * 0.25  // metallic edge
        samples[i] = (main + bright) * env * gain
    }
    apply_room(samples, 0.15, 1.0)
    return <- samples
}

// ─── Biquad Filter ───

struct private BiquadState {
    x1 : float = 0.0
    x2 : float = 0.0
    y1 : float = 0.0
    y2 : float = 0.0
    a0 : float = 1.0
    a1 : float = 0.0
    a2 : float = 0.0
    b1 : float = 0.0
    b2 : float = 0.0
}

def private biquad_highpass(var bq : BiquadState; freq, sample_rate : float) {
    let w0 = float(TWO_PI) * freq / sample_rate
    let cs = cos(w0)
    let sn = sin(w0)
    let alpha = sn / (2.0 * 0.707)  // Q = 0.707 (Butterworth)
    let norm = 1.0 / (1.0 + alpha)
    bq.a0 = (1.0 + cs) * 0.5 * norm
    bq.a1 = -(1.0 + cs) * norm
    bq.a2 = bq.a0
    bq.b1 = -2.0 * cs * norm
    bq.b2 = (1.0 - alpha) * norm
}

def private biquad_bandpass(var bq : BiquadState; freq, q, sample_rate : float) {
    let w0 = float(TWO_PI) * freq / sample_rate
    let cs = cos(w0)
    let sn = sin(w0)
    let alpha = sn / (2.0 * q)
    let norm = 1.0 / (1.0 + alpha)
    bq.a0 = alpha * norm
    bq.a1 = 0.0
    bq.a2 = -alpha * norm
    bq.b1 = -2.0 * cs * norm
    bq.b2 = (1.0 - alpha) * norm
}

def private biquad_process(var bq : BiquadState; x : float) : float {
    let y = bq.a0 * x + bq.a1 * bq.x1 + bq.a2 * bq.x2 - bq.b1 * bq.y1 - bq.b2 * bq.y2
    bq.x2 = bq.x1
    bq.x1 = x
    bq.y2 = bq.y1
    bq.y1 = y
    return y
}

// ─── Metallic Oscillator Bank (808-style hi-hat/cymbal) ───

// 6 square-wave oscillators at inharmonic ratios produce metallic timbre.
// Used by hi-hat, ride, crash, and tambourine.
let private HH_FREQS = fixed_array(205.3, 304.4, 369.6, 522.7, 540.0, 800.0)

def private render_metallic(duration : float; gain : float; decay_rate : float; hpf_freq : float = 5000.0; noise_mix : float = 0.3) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phases : float[6]
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    // two bandpass stages at ~3.4kHz and ~7.1kHz (808 circuit) + HPF
    var bp1 = BiquadState()
    biquad_bandpass(bp1, 3400.0, 1.5, float(SAMPLE_RATE))
    var bp2 = BiquadState()
    biquad_bandpass(bp2, 7100.0, 2.0, float(SAMPLE_RATE))
    var hpf = BiquadState()
    biquad_highpass(hpf, hpf_freq, float(SAMPLE_RATE))
    var seed = 67890u
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env = exp(-t * decay_rate)
        var mix = 0.0
        for (j in range(6)) {
            phases[j] += tp * HH_FREQS[j] * iSr
            mix += (sin(phases[j]) >= 0.0 ? 1.0 : -1.0)  // square wave
        }
        mix *= (1.0 / 6.0)
        // filter through bandpass stages (parallel, like 808 circuit)
        let filtered = biquad_process(bp1, mix) + biquad_process(bp2, mix) * 0.8
        // add noise for body
        let n = noise(seed) * noise_mix
        // HPF the combined signal
        let out = biquad_process(hpf, filtered + n)
        samples[i] = out * env * gain * 2.5  // boost to compensate for filtering
    }
    return <- samples
}

// ─── Additional Drum Synthesis ───

def render_oh(duration : float; gain : float) : array<float> {
    //! Render an open hi-hat: the same metallic oscillator bank as hh but with a much slower decay.
    var samples <- render_metallic(duration, gain, 8.0)
    apply_room(samples, 0.1)
    return <- samples
}

def render_tom(duration : float; gain : float; base_freq : float) : array<float> {
    //! Render a tom drum at `base_freq` with a BD-style body + beater click + impulse + resonant-head overtones and a short room.
    //! Used for tom_low (~80 Hz) and tom_high (~175 Hz).
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase = 0.0
    var seed = 33333u
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    var attack_bp = BiquadState()
    biquad_bandpass(attack_bp, 1500.0, 1.5, float(SAMPLE_RATE))
    var impulse_lp = BiquadState()
    biquad_bandpass(impulse_lp, 1200.0, 0.5, float(SAMPLE_RATE))
    var phase2 = 0.0
    var phase3 = 0.0
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // body: sine with fast pitch sweep
        let freq = base_freq + base_freq * 1.2 * exp(-t * 200.0)
        phase += tp * freq * iSr
        // second partial: drumhead overtone at ~1.6x (inharmonic)
        let freq2 = base_freq * 1.6 + base_freq * 0.45 * exp(-t * 200.0)
        phase2 += tp * freq2 * iSr
        // third partial: higher drumhead mode at ~2.5x, faster decay
        let freq3 = base_freq * 1.9 + base_freq * 0.14 * exp(-t * 200.0)
        phase3 += tp * freq3 * iSr
        // resonant head arrives ~15ms after batter head — creates distinct 2nd peak
        let p2_onset = 0.045
        let p2_env = t > p2_onset ? exp(-(t - p2_onset) * 10.0) : 0.0
        let body = (sin(phase) * 0.7 * exp(-t * 8.0) + sin(phase2) * 0.25 * p2_env + sin(phase3) * 0.2 * exp(-t * 12.0))
        // beater click
        let n = noise(seed)
        let click = biquad_process(attack_bp, n) * exp(-t * 250.0) * 0.3
        // stick impulse
        let impulse = biquad_process(impulse_lp, n) * exp(-t * 200.0) * 0.8
        samples[i] = (body + click + impulse) * gain
    }
    apply_room(samples, 0.05, 0.7)
    return <- samples
}

def render_ride(duration : float; gain : float) : array<float> {
    //! Render a ride cymbal: two bell partials (~340/387 Hz) plus a metallic shimmer, with a long sustain.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phases : float[6]
    var bell_phase1 = 0.0
    var bell_phase2 = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    var hpf = BiquadState()
    biquad_highpass(hpf, 200.0, float(SAMPLE_RATE))
    let bell_inc1 = tp * 340.0 * iSr
    let bell_inc2 = tp * 387.0 * iSr
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // bell tone: two inharmonic frequencies (~340 + ~387 Hz), long sustain
        bell_phase1 += bell_inc1
        bell_phase2 += bell_inc2
        let bell = (sin(bell_phase1) * 0.5 + sin(bell_phase2) * 0.4) * exp(-t * 15.0)
        // metallic shimmer: oscillator bank through HPF, faster decay
        var mix = 0.0
        for (j in range(6)) {
            phases[j] += tp * HH_FREQS[j] * iSr
            mix += (sin(phases[j]) >= 0.0 ? 1.0 : -1.0)
        }
        mix *= (1.0 / 6.0)
        let shimmer = biquad_process(hpf, mix) * exp(-t * 3.0) * 0.25
        samples[i] = (bell + shimmer) * gain * 0.33
    }
    apply_room(samples, 0.10, 0.3)
    return <- samples
}

def render_crash(duration : float; gain : float) : array<float> {
    //! Render a crash cymbal: lower-pitched bell partials plus a broadband metallic wash with a medium-fast decay.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phases : float[6]
    var bell_phase1 = 0.0
    var bell_phase2 = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    var hpf = BiquadState()
    biquad_highpass(hpf, 4000.0, float(SAMPLE_RATE))
    let bell_inc1 = tp * 180.0 * iSr
    let bell_inc2 = tp * 248.0 * iSr
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        // bell tone: short attack, medium decay
        bell_phase1 += bell_inc1
        bell_phase2 += bell_inc2
        let bell = (sin(bell_phase1) * 0.25 + sin(bell_phase2) * 0.15) * exp(-t * 8.0)
        // metallic wash: shimmer with faster decay than ride
        var mix = 0.0
        for (j in range(6)) {
            phases[j] += tp * HH_FREQS[j] * iSr
            mix += (sin(phases[j]) >= 0.0 ? 1.0 : -1.0)
        }
        mix *= (1.0 / 6.0)
        let shimmer = biquad_process(hpf, mix) * exp(-t * 4.0) * 0.8
        samples[i] = (bell + shimmer) * gain * 0.5
    }
    apply_room(samples, 0.1, 0.2)
    return <- samples
}

def render_rimshot(duration : float; gain : float) : array<float> {
    //! Render a rimshot/side-stick: a short woody body (~200 Hz) plus a bandpassed noise snap and a high transient click.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase1 = 0.0
    var seed = 44444u
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    let inc1 = tp * 200.0 * iSr
    var snap_bp = BiquadState()
    biquad_bandpass(snap_bp, 800.0, 0.8, float(SAMPLE_RATE))
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let n = noise(seed)
        // sharp filtered noise snap — the main sound
        let snap = biquad_process(snap_bp, n) * exp(-t * 60.0) * 1.8
        // very short tonal ping for pitch
        phase1 += inc1
        let body = sin(phase1) * exp(-t * 80.0) * 0.6
        // high transient click
        let click = n * exp(-t * 400.0) * 0.8
        samples[i] = (snap + body + click) * gain
    }
    apply_room(samples, 0.1, 0.7)
    return <- samples
}

def render_cowbell(duration : float; gain : float) : array<float> {
    //! Render a cowbell: two detuned square-wave tones through a narrow bandpass, with a second quieter strike 8 ms later.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var phase1 = 0.0
    var phase2 = 0.0
    let iSr = 1.0 / float(SAMPLE_RATE)
    let tp = float(TWO_PI)
    let inc1 = tp * 375.0 * iSr
    let inc2 = tp * 504.0 * iSr
    var bpf = BiquadState()
    biquad_bandpass(bpf, 850.0, 4.0, float(SAMPLE_RATE))
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        phase1 += inc1
        phase2 += inc2
        let sq1 = sin(phase1) >= 0.0 ? 1.0 : -1.0
        let sq2 = sin(phase2) >= 0.0 ? 1.0 : -1.0
        // first strike: both tones
        let hit1 = biquad_process(bpf, (sq1 + sq2) * 0.5) * exp(-t * 20.0)
        // second strike: higher tone only, ~8ms later
        let onset2 = 0.008
        let hit2 = t > onset2 ? sq2 * exp(-(t - onset2) * 40.0) * 0.4 : 0.0
        samples[i] = (hit1 + hit2) * gain * 0.5
    }
    apply_room(samples, 0.1, 0.3)
    return <- samples
}

def render_tambourine(duration : float; gain : float) : array<float> {
    //! Render a tambourine: high-passed noise with two narrow bandpass jingle peaks (~3.8 kHz and ~8.8 kHz) and a delayed second hit.
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    var seed = 55555u
    let iSr = 1.0 / float(SAMPLE_RATE)
    var hpf = BiquadState()
    biquad_highpass(hpf, 6300.0, float(SAMPLE_RATE))
    var bp1 = BiquadState()
    biquad_bandpass(bp1, 3800.0, 8.0, float(SAMPLE_RATE))
    var bp2 = BiquadState()
    biquad_bandpass(bp2, 8800.0, 8.0, float(SAMPLE_RATE))
    for (i in range(nsamples)) {
        let t = float(i) * iSr
        let env = exp(-t * 15.0)
        let n = noise(seed)
        let hp = biquad_process(hpf, n)
        let jingle = biquad_process(bp1, n) * 0.3 + biquad_process(bp2, n) * 0.3
        // two hits: initial strike + delayed jingle at ~30ms
        let onset2 = 0.2
        let env2 = t > onset2 ? exp(-(t - onset2) * 20.0) : 0.0
        let hit1 = (hp * 0.4 + jingle) * env * 0.65
        let hit2 = (hp * 0.3 + jingle * 0.7) * env2 * 0.3
        samples[i] = (hit1 + hit2) * gain
    }
    apply_room(samples, 0.5)
    return <- samples
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
        voice.supersaw_phases[i] = float(noise(seed) * 0.5 + 0.5)  // [0,1)
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
