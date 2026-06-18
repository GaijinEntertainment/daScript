options gen2
options indenting = 4

// Smoke test for sfx_analysis (run: daslang examples/daStrudel/sfx_lab/test_analysis.das).
// Not in the CI suite yet — promote alongside sfx_analysis when the metric stabilizes.

require sfx_analysis
require math

let RATE = 44100

def gen_sine(freq, dur, decay : float) : array<float> {
    let n = int(dur * float(RATE))
    var buf : array<float>
    buf |> resize(n)
    for (i in range(n)) {
        let t = float(i) / float(RATE)
        buf[i] = sin(6.28318530717958 * freq * t) * exp(-decay * t)
    }
    return <- buf
}

[export]
def main {
    let s200 <- gen_sine(200.0, 1.0, 3.0)
    let s4000 <- gen_sine(4000.0, 1.0, 3.0)
    let fast <- gen_sine(440.0, 1.0, 12.0)
    let slow <- gen_sine(440.0, 1.0, 1.5)

    let self_sp = spectral_fit(s200, s200)
    let diff_sp = spectral_fit(s200, s4000)
    let c_lo = analyze(s200, RATE).centroid
    let c_hi = analyze(s4000, RATE).centroid
    let self_env = envelope_fit(fast, fast, RATE)
    let diff_env = envelope_fit(fast, slow, RATE)
    let f_self = fit(s200, s200, RATE)
    let d_fast = analyze(fast, RATE).decay_ms
    let d_slow = analyze(slow, RATE).decay_ms
    let evo_self = spectral_evo_fit(s200, s200, RATE)
    let evo_diff = spectral_evo_fit(s200, s4000, RATE)

    to_log(LOG_INFO, "spectral self={self_sp} diff(200v4000)={diff_sp}\n")
    to_log(LOG_INFO, "centroid lo={c_lo} hi={c_hi}\n")
    to_log(LOG_INFO, "envelope self={self_env} diff(fast v slow)={diff_env}\n")
    to_log(LOG_INFO, "evo self={evo_self} diff(200v4000)={evo_diff}\n")
    to_log(LOG_INFO, "fit.overall(self)={f_self.overall}\n")
    to_log(LOG_INFO, "decay fast={d_fast}ms slow={d_slow}ms\n")

    verify(self_sp > 0.99)
    verify(diff_sp < 0.3)          // fails if magnitude normalization didn't write back
    verify(c_hi > c_lo * 3.0)
    verify(self_env > 0.99)
    verify(diff_env < 0.95)
    verify(evo_self > 0.99)        // identical buffers -> perfect time-resolved match
    verify(evo_diff < 0.6)         // low vs high tone -> different spectrograms
    verify(f_self.overall > 0.99)
    verify(d_fast < d_slow)
    to_log(LOG_INFO, "ALL CHECKS PASSED\n")
}
