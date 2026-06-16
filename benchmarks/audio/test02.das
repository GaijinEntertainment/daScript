options gen2
options persistent_heap

// Reverb tail-skip gating: a partitioned convolver runs the full FFT + MACs every
// block regardless of input, so an idle orbit bus (silent input, reverb tail long
// decayed) wastes a whole convolution per tick. A typical strudel session has many
// orbit buses but only one or two active at a time.
//
// This models a scheduler tick over NBUS orbit reverbs with only NACTIVE fed signal:
//   always  - call conv_reverb_process on every bus (current behavior)
//   gated   - skip a bus when its input is silent AND its tail budget has decayed
// The gate is a cheap O(block) activity scan; the win scales with the idle fraction.

require dastest/testing_boost
require audio
require math

let SR   = 48000
let BLK  = 1024
let DECAY = 2.0
let NBUS    = 8     // orbit buses present
let NACTIVE = 1     // how many are actually fed signal this run

struct BenchBus {
    rev    : ConvolutionReverb?
    inbuf  : array<float>   // interleaved stereo send buffer
    outbuf : array<float>
    tail   : int            // remaining reverb-tail frames (gated path)
}

// Reverb-tail budget in frames: mirrors conv_reverb_init (IR = decay * 1.5, capped at
// 8s) plus two blocks of partition latency. Over-estimating only delays the skip.
let TAIL_FRAMES = int(min(DECAY * 1.5, 8.0) * float(SR)) + 2 * BLK

def fill_noise(var buf : array<float>) {
    var seed = 0x12345678u
    for (i in range(length(buf))) {
        seed = seed * 1664525u + 1013904223u
        buf[i] = float(seed >> 9u) / float(0x7FFFFF) * 2.0 - 1.0
    }
}

def has_input(buf : array<float>) : bool {
    for (i in range(length(buf))) {
        if (abs(buf[i]) > 0.00001) return true
    }
    return false
}

def setup_buses(var buses : array<BenchBus>) {
    for (k in range(NBUS)) {
        var bb : BenchBus
        bb.rev = new ConvolutionReverb
        conv_reverb_init(bb.rev, SR, DECAY, 15000.0, 1000.0, 0.1, ConvReverbQuality.high)
        bb.inbuf |> resize(BLK * 2)
        bb.outbuf |> resize(BLK * 2)
        if (k < NACTIVE) {
            fill_noise(bb.inbuf)   // active bus: signal; others stay silent (zeros)
        }
        bb.tail = 0
        buses |> emplace(bb)
    }
}

def teardown_buses(var buses : array<BenchBus>) {
    for (bb in buses) {
        conv_reverb_uninit(bb.rev)
        unsafe { delete bb.rev }
        delete bb.inbuf
        delete bb.outbuf
    }
    unsafe { delete buses }
}

[benchmark]
def reverb_buses_always(b : B?) {
    var buses : array<BenchBus>
    setup_buses(buses)
    b |> run("reverb_buses/always/{NBUS}", NBUS) {
        for (k in range(NBUS)) {
            unsafe { conv_reverb_process(buses[k].rev, addr(buses[k].inbuf[0]), addr(buses[k].outbuf[0]), BLK) }
        }
    }
    teardown_buses(buses)
}

[benchmark]
def reverb_buses_gated(b : B?) {
    var buses : array<BenchBus>
    setup_buses(buses)
    b |> run("reverb_buses/gated/{NBUS}", NBUS) {
        for (k in range(NBUS)) {
            if (has_input(buses[k].inbuf)) {
                buses[k].tail = TAIL_FRAMES
            }
            if (buses[k].tail > 0) {
                unsafe { conv_reverb_process(buses[k].rev, addr(buses[k].inbuf[0]), addr(buses[k].outbuf[0]), BLK) }
                buses[k].tail -= BLK
            }
        }
    }
    teardown_buses(buses)
}
