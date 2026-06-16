options gen2
options persistent_heap

// Reverb tail-skip gating (now inside conv_reverb_process). A partitioned convolver runs the
// full FFT + MACs every block regardless of input, so an idle orbit bus (silent input, tail
// decayed) would waste a whole convolution per tick. conv_reverb_process gates this: it scans
// the input, and when the bus is silent and its tail has decayed it skips the convolution and
// returns 0 (zeroing the output).
//
//   active  - a bus fed signal every block: always runs (full convolution cost). This is also
//             what an idle bus WOULD cost without the gate.
//   idle    - a bus fed silence: the gate skips immediately (scan + zero + return).
// The ratio is the per-bus saving; it multiplies by the number of idle buses on a track.

require dastest/testing_boost
require audio
require math

let SR    = 48000
let BLK   = 1024
let DECAY = 2.0

def fill_noise(var buf : array<float>) {
    var seed = 0x12345678u
    for (i in range(length(buf))) {
        seed = seed * 1664525u + 1013904223u
        buf[i] = float(seed >> 9u) / float(0x7FFFFF) * 2.0 - 1.0
    }
}

def run_bus(b : B?; name : string; active : bool) {
    var rev = new ConvolutionReverb
    conv_reverb_init(rev, SR, DECAY, 15000.0, 1000.0, 0.1, ConvReverbQuality.high, 0)
    var input : array<float>
    input |> resize(BLK * 2)
    if (active) {
        fill_noise(input)
    }
    var output : array<float>
    output |> resize(BLK * 2)
    b |> run(name, BLK) {
        unsafe { conv_reverb_process(rev, addr(input[0]), addr(output[0]), BLK) }
    }
    conv_reverb_uninit(rev)
    delete input
    delete output
    unsafe { delete rev }
}

[benchmark]
def reverb_active(b : B?) {
    run_bus(b, "reverb/active/{BLK}", true)
}

[benchmark]
def reverb_idle_skipped(b : B?) {
    run_bus(b, "reverb/idle_skipped/{BLK}", false)
}
