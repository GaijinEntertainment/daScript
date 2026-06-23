options gen2

require minfft
require math

// Tutorial MINFFT-02: DCT basics — plans, the round-trip factor, energy compaction
//
// This tutorial covers:
//   - make_dct_plan_1d: a reusable plan (twiddle tables) for the DCT
//   - dct  (forward, DCT-II) and idct (inverse, DCT-III)
//   - the 2N round-trip factor
//   - energy compaction: why the DCT is the workhorse of JPEG and MPEG
//
// The DCT-II concentrates a signal's energy into a few low-frequency
// coefficients. Lossy codecs exploit this: keep the big low coefficients,
// throw away the small high ones. Tutorial 03 builds JPEG on top of this.
//
// Run: daslang.exe tutorials/dasMinfft/02_dct_basics.das

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Forward DCT and energy compaction
// ──────────────────────────────────────────────────────────────────────────
//
// A plan is built once for a given length and reused for every transform of
// that length (building it allocates twiddle tables, so reuse matters in a
// loop). The same plan drives both dct (forward) and idct (inverse). A plan is
// not thread-safe (it holds scratch state) — use one per thread if parallel.

def energy_compaction() {
    print("=== Section 1: forward DCT, energy compaction ===\n")
    let n = 8
    var plan = make_dct_plan_1d(n)

    // A smooth ramp — almost all of its energy is low-frequency.
    let signal <- [for (i in range(n)); float(i)]
    var coeff : array<float>
    dct(signal, coeff, plan)

    print("  signal:      ")
    for (x in signal) {
        print("{x} ")
    }
    print("\n  coefficients:")
    for (c in coeff) {
        print(" {int(c)}")
    }
    print("\n  -> the first (DC) coefficient dominates; the rest are small.\n")

    unsafe { delete plan }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Inverse DCT and the 2N factor
// ──────────────────────────────────────────────────────────────────────────
//
// Like the FFT, minfft's DCT is unnormalized: idct(dct(x)) == 2N*x for a
// length-N signal. Divide by 2N to recover the original.

def roundtrip() {
    print("\n=== Section 2: inverse round-trip ===\n")
    let n = 16
    var plan = make_dct_plan_1d(n)
    let signal <- [for (i in range(n)); sin(float(i) * 0.4f) + 0.2f * float(i)]

    var coeff : array<float>
    dct(signal, coeff, plan)
    var back : array<float>
    idct(coeff, back, plan)

    let factor = float(2 * n)
    var max_err = 0.0f
    for (i in range(n)) {
        max_err = max(max_err, abs(back[i] / factor - signal[i]))
    }
    print("  max round-trip error after dividing by 2N={int(factor)}: {max_err}\n")
    unsafe { delete plan }
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Lossy truncation: keep K coefficients
// ──────────────────────────────────────────────────────────────────────────
//
// Energy compaction in action: zero every coefficient past the first K, then
// reconstruct. Because the high coefficients were tiny, the error stays small
// even though we discarded most of them. This is the essence of lossy coding.

def truncate() {
    print("\n=== Section 3: keep only K coefficients ===\n")
    let n = 32
    var plan = make_dct_plan_1d(n)
    let signal <- [for (i in range(n)); sin(float(i) * 0.3f) + 0.5f * cos(float(i) * 0.1f)]

    var coeff : array<float>
    dct(signal, coeff, plan)
    let factor = float(2 * n)

    for (keep in [n, 8, 4, 2]) {
        var trunc <- clone_to_move(coeff)
        for (k in range(keep, n)) {
            trunc[k] = 0.0f
        }
        var back : array<float>
        idct(trunc, back, plan)
        var max_err = 0.0f
        for (i in range(n)) {
            max_err = max(max_err, abs(back[i] / factor - signal[i]))
        }
        print("  keep {keep}/{n} coefficients -> max error {max_err}\n")
    }
    unsafe { delete plan }
}

[export]
def main() {
    energy_compaction()
    roundtrip()
    truncate()
}
