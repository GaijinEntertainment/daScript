options gen2
options indenting = 4

module sfx_analysis shared
//! Lightweight audio feature extraction + a signal-fit metric for the SFX lab.
//! Used to score how close a synthesized sound is to a loaded reference (e.g. a
//! soundfont drum), and to surface the features that would feed a later analysis pass.

require minfft
require math

let private TWO_PI = 6.28318530717958647692
let private FFT_N = 4096

struct SfxFeatures {
    peak     : float    //!< max |sample|, 0..1
    rms      : float    //!< overall RMS level
    centroid : float    //!< spectral centroid (brightness), Hz
    decay_ms : float    //!< time from envelope peak down to -20 dB, milliseconds
}

struct FitScore {
    spectral : float    //!< 0..1 timbre match (cosine of normalized magnitude spectra)
    envelope : float    //!< 0..1 amplitude-shape + decay-time match
    overall  : float    //!< 0..1 weighted blend (0.6 spectral + 0.4 envelope)
}

// ===== primitives =====

def private peak_of(buf : array<float>) : float {
    var pk = 0.0
    for (s in buf) {
        pk = max(pk, abs(s))
    }
    return pk
}

def private onset_index(buf : array<float>; pk : float) : int {
    let thresh = pk * 0.02
    for (i in range(length(buf))) {
        if (abs(buf[i]) > thresh) return i
    }
    return 0
}

// Hann-windowed magnitude spectrum of FFT_N samples starting at `start`.
def private mag_spectrum(buf : array<float>; start : int; var mags : array<float>) {
    var win : array<float>
    win |> resize(FFT_N)
    let avail = length(buf) - start
    for (i in range(FFT_N)) {
        if (i < avail) {
            let w = 0.5 - 0.5 * cos(TWO_PI * float(i) / float(FFT_N - 1))
            win[i] = buf[start + i] * w
        } else {
            win[i] = 0.0
        }
    }
    var cplx : array<float2>
    fft_real_forward(win, cplx)
    fft_calculate_magnitudes(cplx, mags)
    delete win
    delete cplx
}

// L2-normalized onset-window magnitude spectrum (level-invariant timbre signature).
def private norm_mag(buf : array<float>) : array<float> {
    let pk = peak_of(buf)
    let start = onset_index(buf, pk)
    var mags : array<float>
    mag_spectrum(buf, start, mags)
    var nrm = 0.0
    for (m in mags) {
        nrm += m * m
    }
    nrm = sqrt(nrm)
    if (nrm > 1.0e-12) {
        for (m in mags) {
            m /= nrm
        }
    }
    return <- mags
}

// npts RMS samples across an absolute window of L samples (zero beyond the buffer),
// peak-normalized — captures decay SHAPE and absolute decay TIME, level-invariant.
def private rms_env_abs(buf : array<float>; L, npts : int) : array<float> {
    var env : array<float>
    env |> resize(npts)
    let n = length(buf)
    let win = max(L / npts, 1)
    for (k in range(npts)) {
        let c = k * L / npts
        let lo = max(c - win / 2, 0)
        let hi = min(c + win / 2, n)
        var acc = 0.0
        var cnt = 0
        for (j in range(lo, hi)) {
            acc += buf[j] * buf[j]
            cnt++
        }
        env[k] = cnt > 0 ? sqrt(acc / float(cnt)) : 0.0
    }
    var pk = 0.0
    for (e in env) {
        pk = max(pk, e)
    }
    if (pk > 1.0e-9) {
        for (i in range(npts)) {
            env[i] /= pk
        }
    }
    return <- env
}

// ===== public API =====

def public analyze(buf : array<float>; rate : int) : SfxFeatures {
    var f = SfxFeatures()
    if (empty(buf)) return f
    f.peak = peak_of(buf)
    var acc = 0.0
    for (s in buf) {
        acc += s * s
    }
    f.rms = sqrt(acc / float(length(buf)))
    let start = onset_index(buf, f.peak)
    var mags : array<float>
    mag_spectrum(buf, start, mags)
    var num = 0.0
    var den = 0.0
    for (i in range(length(mags))) {
        let hz = float(i) * float(rate) / float(FFT_N)
        num += hz * mags[i]
        den += mags[i]
    }
    f.centroid = den > 1.0e-12 ? num / den : 0.0
    delete mags
    var env <- rms_env_abs(buf, length(buf), 256)
    var peak_k = 0
    var ep = 0.0
    for (k in range(256)) {
        if (env[k] > ep) {
            ep = env[k]
            peak_k = k
        }
    }
    var dk = 255
    for (k in range(peak_k, 256)) {
        if (env[k] < 0.1) {
            dk = k
            break
        }
    }
    f.decay_ms = float(dk - peak_k) * float(length(buf)) / 256.0 / float(rate) * 1000.0
    delete env
    return f
}

def public spectral_fit(a, b : array<float>) : float {
    if (empty(a) || empty(b)) return 0.0
    var ma <- norm_mag(a)
    var mb <- norm_mag(b)
    let n = min(length(ma), length(mb))
    var dot = 0.0
    for (i in range(n)) {
        dot += ma[i] * mb[i]
    }
    delete ma
    delete mb
    return clamp(dot, 0.0, 1.0)
}

def public envelope_fit(a, b : array<float>; rate : int) : float {
    if (empty(a) || empty(b)) return 0.0
    let longer = max(length(a), length(b))
    let L = min(longer, 2 * rate)   // shared absolute time base, capped at 2 s
    let N = 128
    var ea <- rms_env_abs(a, L, N)
    var eb <- rms_env_abs(b, L, N)
    var sad = 0.0
    for (i in range(N)) {
        sad += abs(ea[i] - eb[i])
    }
    delete ea
    delete eb
    return clamp(1.0 - sad / float(N), 0.0, 1.0)
}

def public fit(ref, mix : array<float>; rate : int) : FitScore {
    let sp = spectral_fit(ref, mix)
    let en = envelope_fit(ref, mix, rate)
    return FitScore(spectral = sp, envelope = en, overall = 0.6 * sp + 0.4 * en)
}
