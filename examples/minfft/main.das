options gen2

// This example uses the das-minfft package installed via daspkg.
// Run from this directory:
//
//   daslang ../../utils/daspkg/main.das -- install
//   daslang main.das

require minfft
require math

[export]
def main() {
    // generate a simple sine wave signal
    var signal <- [for (i in range(64)); sin(float(i) * 0.2)]

    // compute FFT
    var complex_frequencies : array<float2>
    fft_real_forward(signal, complex_frequencies)

    // compute magnitudes
    var magnitudes : array<float>
    fft_calculate_normalized_magnitudes(complex_frequencies, magnitudes)

    print("FFT of sine wave (64 samples, freq=0.2):\n")
    for (i in range(length(magnitudes))) {
        if (magnitudes[i] > 0.01) {
            print("  bin {i}: magnitude = {magnitudes[i]}\n")
        }
    }
}
