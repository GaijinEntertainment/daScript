options gen2

require dastest/testing_boost public

require sfx_gen
require math

def private zero_crossings(samples : array<float>; from_i, to_i : int) : int {
    var n = 0
    for (i in range(max(from_i, 1), min(to_i, length(samples)))) {
        if ((samples[i - 1] < 0.0) != (samples[i] < 0.0)) {
            n++
        }
    }
    return n
}

def private legacy_sweep(freq_start, freq_end : float; duration, volume : float) : array<float> {
    let num_samples = int(float(AUDIO_RATE) * duration)
    var samples : array<float>
    samples |> resize(num_samples)
    for (i in range(num_samples)) {
        let t = float(i) / float(AUDIO_RATE)
        let frac = t / duration
        let freq = freq_start + (freq_end - freq_start) * frac
        samples[i] = sin(2.0 * PI * freq * t) * (1.0 - frac) * volume
    }
    return <- samples
}

[test]
def test_sweep_endpoints_are_what_is_heard(t : T?) {
    t |> run("the heard frequency across the sweep matches the named endpoints") @(tt : T?) {
        let f0 = 400.0
        let f1 = 800.0
        let dur = 0.5
        let win_lo = 0.6
        let win_hi = 0.9
        var s <- gen_sine_sweep(f0, f1, dur, 0.3)
        let i0 = int(float(AUDIO_RATE) * dur * win_lo)
        let i1 = int(float(AUDIO_RATE) * dur * win_hi)
        let window = float(i1 - i0) / float(AUDIO_RATE)
        let heard = float(zero_crossings(s, i0, i1)) / (2.0 * window)
        let named = f0 + (f1 - f0) * (win_lo + win_hi) * 0.5
        tt |> success(abs(heard - named) < named * 0.08,
            "heard {heard} Hz across the measurement window; named average is {named} Hz")
        delete s
    }
    t |> run("a constant tone stays at its named pitch") @(tt : T?) {
        let dur = 0.25
        var s <- gen_tone(440.0, dur, 0.3)
        let i0 = int(float(AUDIO_RATE) * dur * 0.1)
        let i1 = int(float(AUDIO_RATE) * dur * 0.9)
        let window = float(i1 - i0) / float(AUDIO_RATE)
        let heard = float(zero_crossings(s, i0, i1)) / (2.0 * window)
        tt |> success(abs(heard - 440.0) < 440.0 * 0.05, "heard {heard} Hz, named 440")
        delete s
    }
}

[test]
def test_bank_remap_preserves_audio(t : T?) {
    t |> run("legacy (fs, fe) equals corrected (fs, 2*fe - fs) sample for sample") @(tt : T?) {
        var cases <- [(180.0, 90.0, 0.05, 0.2), (980.0, 1560.0, 0.09, 0.28), (440.0, 80.0, 0.5, 0.3)]
        for (cs in cases) {
            var old_s <- legacy_sweep(cs._0, cs._1, cs._2, cs._3)
            var new_s <- gen_sine_sweep(cs._0, 2.0 * cs._1 - cs._0, cs._2, cs._3)
            tt |> equal(length(new_s), length(old_s), "same sample count")
            var max_diff = 0.0
            for (a, b in old_s, new_s) {
                max_diff = max(max_diff, abs(a - b))
            }
            tt |> success(max_diff < 1e-3, "remapped sweep ({cs._0} -> {cs._1}) is audio-identical, max diff {max_diff}")
            delete old_s
            delete new_s
        }
        delete cases
    }
}
