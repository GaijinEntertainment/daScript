options gen2

require minfft
require math

// Tutorial MINFFT-01: Real FFT — spectrum and round-trip
//
// This tutorial covers:
//   - fft_real_forward: real signal -> complex spectrum
//   - the magnitude helpers (raw / normalized / log)
//   - fft_real_inverse and the N normalization factor
//
// minfft works on power-of-two lengths. A real signal of length N transforms
// to N/2+1 complex bins (the spectrum of a real signal is symmetric, so the
// upper half is redundant and not stored).
//
// Run: daslang.exe tutorials/dasMinfft/01_real_fft.das

let TWO_PI = 6.2831853f

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Forward transform: find the tones in a signal
// ──────────────────────────────────────────────────────────────────────────
//
// We build a signal that is the sum of two tones at bins 4 and 12, then let
// the FFT recover them. A "bin k" tone completes exactly k cycles over the N
// samples, so it lands cleanly on a single spectrum bin. A tone of amplitude A
// shows raw magnitude N/2 * A at its bin (here: 32 at bin 4, 16 at bin 12).

def forward_spectrum() {
    print("=== Section 1: forward spectrum ===\n")
    let n = 64
    let signal <- [for (i in range(n));
        sin(TWO_PI * 4.0f * float(i) / float(n)) +
        0.5f * sin(TWO_PI * 12.0f * float(i) / float(n))]

    var spectrum : array<float2>
    fft_real_forward(signal, spectrum)
    print("  {n} samples -> {length(spectrum)} complex bins (N/2+1)\n")

    var mag : array<float>
    fft_calculate_magnitudes(spectrum, mag)
    for (k in range(length(mag))) {
        if (mag[k] > 1.0f) {
            print("  bin {k}: magnitude {mag[k]}\n")   // expect peaks at 4 and 12
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Magnitude helpers
// ──────────────────────────────────────────────────────────────────────────
//
// A complex bin (re, im) carries amplitude sqrt(re*re + im*im). Three helpers
// fill a float array from the complex spectrum:
//   - fft_calculate_magnitudes            raw amplitude
//   - fft_calculate_normalized_magnitudes raw amplitude scaled by 1/(N/2+1)
//   - fft_calculate_log_magnitudes        log scale, handy for spectrum displays

def magnitude_helpers() {
    print("\n=== Section 2: magnitude helpers ===\n")
    let n = 16
    let signal <- [for (i in range(n)); cos(TWO_PI * 2.0f * float(i) / float(n))]
    var spectrum : array<float2>
    fft_real_forward(signal, spectrum)

    var raw : array<float>
    var norm : array<float>
    var logm : array<float>
    fft_calculate_magnitudes(spectrum, raw)
    fft_calculate_normalized_magnitudes(spectrum, norm)
    fft_calculate_log_magnitudes(spectrum, logm)
    print("  bin 2:  raw={raw[2]}  normalized={norm[2]}  log={logm[2]}\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Inverse transform and normalization
// ──────────────────────────────────────────────────────────────────────────
//
// minfft transforms are UNNORMALIZED: fft_real_inverse(fft_real_forward(x))
// returns N*x, not x. Divide by N to recover the original signal. (This mirrors
// minfft's whole design — the DCT pair in tutorial 02 scales by 2N per axis.)

def inverse_roundtrip() {
    print("\n=== Section 3: inverse round-trip ===\n")
    let n = 8
    let signal <- [for (i in range(n)); float(i) * 0.5f - 1.0f]
    var spectrum : array<float2>
    fft_real_forward(signal, spectrum)

    var back : array<float>
    fft_real_inverse(spectrum, back)

    var max_err = 0.0f
    for (i in range(n)) {
        max_err = max(max_err, abs(back[i] / float(n) - signal[i]))
    }
    print("  max round-trip error after dividing by N={n}: {max_err}\n")
}

[export]
def main() {
    forward_spectrum()
    magnitude_helpers()
    inverse_roundtrip()
}
