options gen2
options persistent_heap

module sfx_gen public

require math

let AUDIO_RATE = 48000
let AUDIO_CHANNELS = 1

//! endpoints are instantaneous frequencies of a linear ramp; a negative end frequency sweeps through zero and back up
def gen_sine_sweep(freq_start, freq_end : float; duration : float; volume : float = 0.3) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    for (i in range(num_samples)) {
        let t = float(i) / float(AUDIO_RATE)
        let frac = t / duration
        let phase = freq_start * t + (freq_end - freq_start) * t * t / (2.0 * duration)
        samples[i] = sin(2.0 * PI * phase) * (1.0 - frac) * volume
    }
    return <- samples
}

def gen_noise_burst(duration : float; volume : float = 0.2) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    var seed = 12345u
    for (i in range(num_samples)) {
        let frac = float(i) / float(num_samples)
        seed = seed * 1103515245u + 12345u
        let noise = float(int(seed >> 16u) % 1000) / 500.0 - 1.0
        samples[i] = noise * (1.0 - frac) * volume
    }
    return <- samples
}

def gen_tone(freq, duration, volume : float) : array<float> {
    return <- gen_sine_sweep(freq, freq, duration, volume)
}

def overlay_at(var dst : array<float>; src : array<float>; at : int) {
    let need = at + length(src)
    if (length(dst) < need) {
        dst |> resize(need)
    }
    for (i in range(length(src))) {
        dst[at + i] += src[i]
    }
}

def gen_fanfare(base : float; vol : float = 0.28) : array<float> {
    var out : array<float>
    let step = int(float(AUDIO_RATE) * 0.11)
    let n1 <- gen_tone(base, 0.09, vol)
    let n2 <- gen_tone(base * 1.25, 0.09, vol)
    let n3 <- gen_tone(base * 1.5, 0.10, vol * 1.05)
    let n4 <- gen_tone(base * 2.0, 0.28, vol * 1.15)
    overlay_at(out, n1, 0)
    overlay_at(out, n2, step)
    overlay_at(out, n3, step * 2)
    overlay_at(out, n4, step * 3)
    return <- out
}
