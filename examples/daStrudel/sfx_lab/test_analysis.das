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

var g_nseed = 12345u
def nrand() : float {
    g_nseed = g_nseed * 1664525u + 1013904223u
    return float((g_nseed >> 9u) & 0x7FFFFFu) / 4194303.0 - 1.0
}

// sharp transient: a noise burst that decays fast (high crest, early temporal centroid)
def gen_decay_noise(dur, decay : float) : array<float> {
    let n = int(dur * float(RATE))
    var buf : array<float>
    buf |> resize(n)
    for (i in range(n)) {
        let t = float(i) / float(RATE)
        buf[i] = nrand() * exp(-decay * t)
    }
    return <- buf
}

// the exploit: a sustained broadband noise wash (low crest, late centroid, long duration)
def gen_wash(dur : float) : array<float> {
    let n = int(dur * float(RATE))
    var buf : array<float>
    buf |> resize(n)
    for (i in range(n)) {
        buf[i] = nrand() * 0.5
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

    // the noise-wash regression: a sustained wash must score LOW against a sharp transient
    let tref <- gen_decay_noise(1.0, 18.0)
    let twash <- gen_wash(1.0)
    let fit_self_t = fit(tref, tref, RATE).overall
    let fit_wash = fit(tref, twash, RATE)
    let sh_ref = temporal_shape(tref, RATE)
    let sh_wash = temporal_shape(twash, RATE)

    to_log(LOG_INFO, "spectral self={self_sp} diff(200v4000)={diff_sp}\n")
    to_log(LOG_INFO, "centroid lo={c_lo} hi={c_hi}\n")
    to_log(LOG_INFO, "envelope self={self_env} diff(fast v slow)={diff_env}\n")
    to_log(LOG_INFO, "evo self={evo_self} diff(200v4000)={evo_diff}\n")
    to_log(LOG_INFO, "fit.overall(self)={f_self.overall}\n")
    to_log(LOG_INFO, "decay fast={d_fast}ms slow={d_slow}ms\n")
    to_log(LOG_INFO, "transient self={fit_self_t}  wash overall={fit_wash.overall} (spec={fit_wash.spectral} shape={fit_wash.shape} atk={fit_wash.attack})\n")
    to_log(LOG_INFO, "shape tc: ref={sh_ref.tc} wash={sh_wash.tc}  ed: ref={sh_ref.ed} wash={sh_wash.ed}  crest: ref={sh_ref.crest} wash={sh_wash.crest}\n")

    verify(self_sp > 0.99)
    verify(diff_sp < 0.3)          // fails if magnitude normalization didn't write back
    verify(c_hi > c_lo * 3.0)
    verify(self_env > 0.99)
    verify(diff_env < 0.95)
    verify(evo_self > 0.99)        // identical buffers -> perfect time-resolved match
    verify(evo_diff < 0.6)         // low vs high tone -> different spectrograms
    verify(f_self.overall > 0.99)
    verify(d_fast < d_slow)
    verify(fit_self_t > 0.99)              // identical transient -> perfect
    verify(fit_wash.overall < 0.6)         // THE fix: the sustained wash must NOT score high
    verify(fit_wash.shape < 0.5)           // shape is the structural anti-wash wall
    to_log(LOG_INFO, "ALL CHECKS PASSED\n")
}
