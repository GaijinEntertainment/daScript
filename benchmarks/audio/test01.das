options gen2
options persistent_heap

// Convolution-reverb quality tiers: per-block cost of the real conv_reverb_process.
//   high   = two decorrelated IRs (two full partitioned convolutions per block)
//   medium = one mono IR + two Schroeder allpass filters (~half the convolution cost)
// One conv_reverb_process(.., BLK) call accumulates exactly one block, so ns/op is the
// per-frame cost of one steady-state block (FFT-input + convolution(s) + IFFT + overlap).
// The FFT-input and the single IFFT are shared by both tiers, so the medium speedup here is
// diluted vs the convolution-only delta — this measures the real end-to-end block cost.

require dastest/testing_boost
require audio
require math

let SR   = 48000
let BLK  = 1024     // CONV_BLOCK_SIZE — one block per process call
let DECAY = 2.0     // 2s decay -> ~94 partitions

def make_input_block(n : int) : array<float> {
    var buf : array<float>
    buf |> resize(n * 2)   // interleaved stereo
    var seed = 0x12345678u
    for (i in range(n * 2)) {
        seed = seed * 1664525u + 1013904223u
        buf[i] = float(seed >> 9u) / float(0x7FFFFF) * 2.0 - 1.0   // [-1, 1]
    }
    return <- buf
}

// Confirm the medium path actually produces a decorrelated (non-mono) stereo image.
def assert_stereo(b : B?; var rev : ConvolutionReverb?; var input : array<float>) {
    var output : array<float>
    output |> resize(BLK * 2)
    var diff = 0.0
    for (k in range(200)) {   // settle past the one-block latency, let the tail build
        unsafe { conv_reverb_process(rev, addr(input[0]), addr(output[0]), BLK) }
    }
    for (i in range(BLK)) {
        diff += abs(output[i * 2] - output[i * 2 + 1])
    }
    if (diff <= 0.0) {
        b->fail()   // channels identical -> decorrelation not working
    }
    delete output
}

def run_quality(b : B?; qual : ConvReverbQuality; name : string) {
    var rev = new ConvolutionReverb
    conv_reverb_init(rev, SR, DECAY, 15000.0, 1000.0, 0.1, qual)
    var input <- make_input_block(BLK)
    assert_stereo(b, rev, input)
    var output : array<float>
    output |> resize(BLK * 2)
    b |> run(name, BLK) {
        unsafe { conv_reverb_process(rev, addr(input[0]), addr(output[0]), BLK) }
    }
    conv_reverb_uninit(rev)
    delete output
    delete input
    unsafe { delete rev }
}

[benchmark]
def conv_reverb_high(b : B?) {
    run_quality(b, ConvReverbQuality.high, "conv_reverb/high/{BLK}")
}

[benchmark]
def conv_reverb_medium(b : B?) {
    run_quality(b, ConvReverbQuality.medium, "conv_reverb/medium/{BLK}")
}
