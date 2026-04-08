options gen2
options indenting = 4

module strudel_synth shared

require strudel/strudel_event
require strudel/strudel_samples
require math

let SAMPLE_RATE = 48000
let TWO_PI = 3.14159265358979323846 * 2.0

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
    let noteOff = duration - release
    if (t < noteOff) {
        return sustain
    }
    let t3 = t - noteOff
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
    }
    return OscType.osc_unknown
}

def render_oscillator(sound : string; duration : float; freq : float; gain : float; event : Event) : array<float> {
    let nsamples = int(duration * float(SAMPLE_RATE))
    var samples : array<float>
    samples |> resize(nsamples)
    let iSr = 1.0 / float(SAMPLE_RATE)
    let phaseInc = freq * iSr
    let tp = float(TWO_PI)
    let att = event.attack
    let dec = event.decay
    let sus = event.sustain
    let rel = event.release
    let oscType = to_osc_type(sound)
    var phase = 0.0
    if (oscType == OscType.osc_sine) {
        for (i in range(nsamples)) {
            let env = adsr(float(i) * iSr, att, dec, sus, rel, duration)
            samples[i] = sin(phase * tp) * env * gain
            phase += phaseInc
            phase -= floor(phase)
        }
    } elif (oscType == OscType.osc_sawtooth) {
        for (i in range(nsamples)) {
            let env = adsr(float(i) * iSr, att, dec, sus, rel, duration)
            samples[i] = (phase * 2.0 - 1.0) * env * gain
            phase += phaseInc
            phase -= floor(phase)
        }
    } elif (oscType == OscType.osc_square) {
        for (i in range(nsamples)) {
            let env = adsr(float(i) * iSr, att, dec, sus, rel, duration)
            samples[i] = (phase < 0.5 ? 1.0 : -1.0) * env * gain
            phase += phaseInc
            phase -= floor(phase)
        }
    } elif (oscType == OscType.osc_triangle) {
        for (i in range(nsamples)) {
            let env = adsr(float(i) * iSr, att, dec, sus, rel, duration)
            samples[i] = (abs(phase * 4.0 - 2.0) - 1.0) * env * gain
            phase += phaseInc
            phase -= floor(phase)
        }
    }
    return <- samples
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
        mono <- render_bd(0.2, 1.0)
    } elif (s == "sd") {
        mono <- render_sd(0.15, 1.0)
    } elif (s == "hh") {
        mono <- render_hh(0.05, 1.0)
    } elif (s == "cp") {
        mono <- render_cp(0.1, 1.0)
    } else {
        let freq = note_to_freq(event.note)
        if (s == "sine" || s == "sawtooth" || s == "square" || s == "triangle") {
            mono <- render_oscillator(s, duration_sec, freq, 1.0, event)
        } else {
            mono |> resize(int(duration_sec * float(SAMPLE_RATE)))
        }
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
