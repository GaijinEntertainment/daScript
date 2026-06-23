options gen2

require dastest/testing_boost public
require minfft
require math

def private close(t : T?; a, b, eps : float; what : string) {
    t |> success(abs(a - b) < eps, "{what}: |{a} - {b}| >= {eps}")
}

// DCT-II of a constant signal: all energy in the DC term, AC coefficients zero.
[test]
def test_dct_constant(t : T?) {
    t |> run("dct of constant -> DC only") <| @(t : T?) {
        let n = 8
        var plan = make_dct_plan_1d(n)
        let signal <- [for (_i in range(n)); 1.0f]
        var coeff : array<float>
        dct(signal, coeff, plan)
        // minfft DCT-II DC term = 2 * sum(x) = 2 * 8 = 16
        close(t, coeff[0], 16.0f, 1.0e-4f, "DC")
        for (k in range(1, n)) {
            close(t, coeff[k], 0.0f, 1.0e-4f, "AC[{k}]")
        }
        unsafe { delete plan }
    }
}

// idct(dct(x)) == 2N * x  (minfft is unnormalized).
[test]
def test_dct_roundtrip_1d(t : T?) {
    t |> run("1d round-trip / 2N") <| @(t : T?) {
        let n = 16
        var plan = make_dct_plan_1d(n)
        let signal <- [for (i in range(n)); sin(float(i) * 0.37f) + 0.25f * float(i)]
        var coeff : array<float>
        dct(signal, coeff, plan)
        var back : array<float>
        idct(coeff, back, plan)
        let factor = float(2 * n)
        for (i in range(n)) {
            close(t, back[i] / factor, signal[i], 1.0e-4f, "x[{i}]")
        }
        unsafe { delete plan }
    }
}

// 2D 8x8 block (the JPEG case): idct(dct(x)) == (2*rows)*(2*cols) * x.
[test]
def test_dct_roundtrip_2d(t : T?) {
    t |> run("2d 8x8 round-trip / 256") <| @(t : T?) {
        let rows = 8
        let cols = 8
        var plan = make_dct_plan_2d(rows, cols)
        let blk <- [for (i in range(rows * cols)); float((i * 7) % 23) - 11.0f]
        var coeff : array<float>
        dct(blk, coeff, plan)
        var back : array<float>
        idct(coeff, back, plan)
        let factor = float((2 * rows) * (2 * cols))
        for (i in range(rows * cols)) {
            close(t, back[i] / factor, blk[i], 1.0e-3f, "x[{i}]")
        }
        unsafe { delete plan }
    }
}

// Real FFT pair: invrealdft(realdft(x)) == N * x, and a pure tone lands on one bin.
[test]
def test_real_fft(t : T?) {
    t |> run("real fft round-trip / N + single bin") <| @(t : T?) {
        let n = 32
        let two_pi = 6.2831853f
        let signal <- [for (i in range(n)); cos(two_pi * 3.0f * float(i) / float(n))]
        var spectrum : array<float2>
        fft_real_forward(signal, spectrum)
        t |> equal(length(spectrum), n / 2 + 1)

        var mag : array<float>
        fft_calculate_magnitudes(spectrum, mag)
        // a unit cosine at bin 3 -> raw magnitude N/2 there, ~0 elsewhere
        close(t, mag[3], float(n) * 0.5f, 1.0e-2f, "bin 3 magnitude")
        close(t, mag[7], 0.0f, 1.0e-2f, "bin 7 magnitude")

        var back : array<float>
        fft_real_inverse(spectrum, back)
        for (i in range(n)) {
            close(t, back[i] / float(n), signal[i], 1.0e-4f, "x[{i}]")
        }
    }
}
