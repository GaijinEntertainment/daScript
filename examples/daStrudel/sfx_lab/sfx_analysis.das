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
let private STFT_N = 1024            // smaller window for time-resolved analysis
let private INV_LN10 = 0.43429448190325176
let private SPEC_FMIN = 30.0         // spectrogram low-frequency edge (log axis)

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
    let sp = spectral_evo_fit(ref, mix, rate)   // dB / log-freq / time-resolved
    let en = envelope_fit(ref, mix, rate)
    return FitScore(spectral = sp, envelope = en, overall = 0.6 * sp + 0.4 * en)
}

// ===== time-resolved analysis (energy / centroid over time, spectrogram) =====

// Hann-windowed magnitude spectrum of `nfft` samples at `start` (zero-padded past the end).
def private stft_mag(buf : array<float>; start, nfft : int; var mags : array<float>) {
    var win : array<float>
    win |> resize(nfft)
    let n = length(buf)
    for (i in range(nfft)) {
        let idx = start + i
        if (idx < n) {
            let w = 0.5 - 0.5 * cos(TWO_PI * float(i) / float(nfft - 1))
            win[i] = buf[idx] * w
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

// peak-normalized RMS energy over `npts` frames across the whole buffer (the falloff shape).
def public energy_curve(buf : array<float>; npts : int) : array<float> {
    return <- rms_env_abs(buf, length(buf), npts)
}

// spectral centroid (Hz) per time frame — exposes a pitch sweep (centroid moving over time).
def public centroid_curve(buf : array<float>; rate, nframes : int) : array<float> {
    var out : array<float>
    out |> resize(nframes)
    let n = length(buf)
    if (n == 0) return <- out
    let hop = max((n - STFT_N) / max(nframes - 1, 1), 1)
    let last = max(n - 1, 0)
    for (f in range(nframes)) {
        let start = min(f * hop, last)
        var mags : array<float>
        stft_mag(buf, start, STFT_N, mags)
        var num = 0.0
        var den = 0.0
        for (i in range(length(mags))) {
            num += float(i) * float(rate) / float(STFT_N) * mags[i]
            den += mags[i]
        }
        out[f] = den > 1.0e-9 ? num / den : 0.0
        delete mags
    }
    return <- out
}

// STFT magnitude in dB over `win_len` samples (frames evenly spaced across [0,win_len)),
// row-major [nbins rows (freq, HIGH at row 0 / top) x nframes cols (time)], LOG-frequency from
// SPEC_FMIN..fmax Hz. Core shared by the UI spectrogram and the time-resolved fit.
def private log_spectro(buf : array<float>; win_len, rate, nframes, nbins : int; fmax : float) : array<float> {
    var out : array<float>
    out |> resize(nbins * nframes)
    if (empty(buf) || win_len <= 0) return <- out
    let hop = max(win_len / nframes, 1)
    let logmin = log(SPEC_FMIN)
    let logspan = log(fmax) - logmin
    let hz_to_bin = float(STFT_N) / float(rate)
    for (f in range(nframes)) {
        let start = f * hop
        var mags : array<float>
        stft_mag(buf, start, STFT_N, mags)
        let mlen = length(mags)
        for (r in range(nbins)) {
            // row 0 = top = high freq; log-spaced bands so the low end gets resolution
            let f_lo = exp(logmin + (1.0 - float(r + 1) / float(nbins)) * logspan)
            let f_hi = exp(logmin + (1.0 - float(r) / float(nbins)) * logspan)
            let b0 = clamp(int(f_lo * hz_to_bin), 0, mlen - 1)
            let b1 = clamp(int(f_hi * hz_to_bin), b0 + 1, mlen)
            var m = 0.0
            for (b in range(b0, b1)) {
                m = max(m, mags[b])
            }
            out[r * nframes + f] = 20.0 * log(m / float(STFT_N) + 1.0e-9) * INV_LN10
        }
        delete mags
    }
    return <- out
}

// UI spectrogram: frames spread across the whole buffer (spectrum over time, for plot_heatmap).
def public spectrogram(buf : array<float>; rate, nframes, nbins : int; fmax : float) : array<float> {
    return <- log_spectro(buf, length(buf), rate, nframes, nbins, fmax)
}

// subtract the max cell so the loudest = 0 dB (level-invariant)
def private norm_db(var s : array<float>) {
    var mx = -1.0e30
    for (v in s) {
        mx = max(mx, v)
    }
    if (mx > -1.0e29) {
        for (v in s) {
            v -= mx
        }
    }
}

// Time-resolved, log-frequency, dB-domain spectral distance over a shared absolute window.
// dB weighting makes a quiet-but-defining structure (the inharmonic mid-ladder) count as much as
// the loud fundamental, and the per-frame grid captures spectral evolution (a pitch sweep) — both
// blind spots of the single-window linear-cosine spectral_fit.
def public spectral_evo_fit(a, b : array<float>; rate : int) : float {
    if (empty(a) || empty(b)) return 0.0
    let L = min(min(length(a), length(b)), rate)   // shared absolute window, up to ~1 s
    let NF = 32
    let NB = 40
    let FMAX = 12000.0
    var sa <- log_spectro(a, L, rate, NF, NB, FMAX)
    var sb <- log_spectro(b, L, rate, NF, NB, FMAX)
    norm_db(sa)
    norm_db(sb)
    let n = NF * NB
    var acc = 0.0
    for (i in range(n)) {
        acc += abs(max(sa[i], -80.0) - max(sb[i], -80.0))   // floor silence at -80 dB
    }
    delete sa
    delete sb
    return clamp(1.0 - acc / float(n) / 45.0, 0.0, 1.0)
}
