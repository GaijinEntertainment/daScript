// The silero VAD kernels' CPU references and their synthetic fixture, shared by the Metal and the Vulkan kernel cells: the
// encoder (window, STFT magnitudes, the four convs, the LSTM's input gates) and the recurrence with its decoder, each in the
// kernels' own summation order in f32, over a weight slab laid at the offsets `GkVadArgs` names behind a lead of NaN.

options gen2
options _dasllama_internal = true

module _vad_oracle shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_gpu_kernels_common   // GkVadArgs
require _compares   // NAN_SENTINEL, hash_weight / put_hashed: the slab's weights

let VAD_LEAD = 7   //! NaN floats ahead of the slab's first tensor: a body that ignored an offset reads them

//! a synthetic weight slab and its offsets
struct VadFixture {
    slab : array<float>
    pa : GkVadArgs
}

//! the synthetic slab and its offsets, `nch` chunks in the args
def vad_fixture(nch : int) : VadFixture {
    var slab <- [for (_i in range(VAD_LEAD)); NAN_SENTINEL]
    var pa = GkVadArgs(n = uint(nch))
    pa.stft = put_hashed(slab, 258 * 256, 1, 0.06)
    pa.c1w = put_hashed(slab, 129 * 3 * 128, 2, 0.05)
    pa.c1b = put_hashed(slab, 128, 3, 0.1)
    pa.c2w = put_hashed(slab, 128 * 3 * 64, 4, 0.06)
    pa.c2b = put_hashed(slab, 64, 5, 0.1)
    pa.c3w = put_hashed(slab, 64 * 3 * 64, 6, 0.1)
    pa.c3b = put_hashed(slab, 64, 7, 0.1)
    pa.c4w = put_hashed(slab, 64 * 3 * 128, 8, 0.1)
    pa.c4b = put_hashed(slab, 128, 9, 0.1)
    pa.ih = put_hashed(slab, 512 * 128, 10, 0.15)
    pa.hh = put_hashed(slab, 512 * 128, 11, 0.15)
    pa.lb = put_hashed(slab, 512, 12, 0.3)
    pa.fw = put_hashed(slab, 128, 13, 0.4)
    pa.fb = put_hashed(slab, 1, 14, 0.2)
    return <- VadFixture(slab <- slab, pa = pa)
}

//! the samples the encoder reads: the 64-sample context, then `nch` chunks
def vad_signal(nch : int) : array<float> {
    var x : array<float>
    x |> resize(64 + nch * 512)
    for (i in range(length(x))) {
        x[i] = 0.4 * sin(float(i) * 0.031) + hash_weight(i, 21, 0.2)
    }
    return <- x
}

//! the encoder's gx [nch][512] in the kernel's order
def vad_enc_oracle(x, wn : array<float>; pa : GkVadArgs) : array<float> {
    let nch = int(pa.n)
    var gx : array<float>
    gx |> resize(nch * 512)
    var win, mag, e1, e2, e3, feat : array<float>
    win |> resize(640)
    mag |> resize(516)
    e1 |> resize(512)
    e2 |> resize(128)
    e3 |> resize(64)
    feat |> resize(128)
    for (n in range(nch)) {
        for (p in range(640)) {
            win[p] = x[n * 512 + (p < 576 ? p : 1150 - p)]
        }
        for (o in range(516)) {
            let fb = o / 4
            let wb = (o % 4) * 128
            var re = 0.0
            var im = 0.0
            for (k in range(256)) {
                re += wn[int(pa.stft) + k * 258 + fb] * win[wb + k]
                im += wn[int(pa.stft) + k * 258 + 129 + fb] * win[wb + k]
            }
            mag[o] = sqrt(re * re + im * im)
        }
        vad_conv(e1, mag, wn, int(pa.c1w), int(pa.c1b), 129, 128, 4, 1)
        vad_conv(e2, e1, wn, int(pa.c2w), int(pa.c2b), 128, 64, 4, 2)
        vad_conv(e3, e2, wn, int(pa.c3w), int(pa.c3b), 64, 64, 2, 2)
        vad_conv(feat, e3, wn, int(pa.c4w), int(pa.c4b), 64, 128, 1, 1)
        for (r in range(512)) {
            var g = 0.0
            for (i in range(128)) {
                g += wn[int(pa.ih) + i * 512 + r] * feat[i]
            }
            gx[n * 512 + r] = g
        }
    }
    return <- gx
}

//! a k3 pad-1 conv + ReLU, [ch][time] rows, taps [cin][3][cout] at `w`, the in-range taps summed from the bias in (cin, tap) order
def private vad_conv(var y : array<float>; inp, wn : array<float>; w, b, cin, cout, ti, stride : int) {
    let tout = (ti - 1) / stride + 1
    for (oc in range(cout)) {
        for (ot in range(tout)) {
            var acc = wn[b + oc]
            for (ic in range(cin)) {
                for (k in range(3)) {
                    let src = ot * stride + k - 1
                    if (src >= 0 && src < ti) {
                        acc += wn[w + (ic * 3 + k) * cout + oc] * inp[ic * ti + src]
                    }
                }
            }
            y[oc * tout + ot] = max(acc, 0.0)
        }
    }
}

//! the recurrence's reference: the probabilities, the state [h | c] left, and the largest |gate pre-activation| the walk met
struct VadLstmRef {
    probs : array<float>
    st : array<float>
    max_gate : float
}

//! the recurrence over `pa.n` chunks of gx from st [h | c], the decoder's probability a step
def vad_lstm_oracle(gx, wn, st0 : array<float>; pa : GkVadArgs) : VadLstmRef {
    var h, c : array<float>
    h |> resize(128)
    c |> resize(128)
    for (j in range(128)) {
        h[j] = st0[j]
        c[j] = st0[128 + j]
    }
    var probs, hh : array<float>
    probs |> reserve(int(pa.n))
    hh |> resize(512)
    var max_gate = 0.0
    for (step in range(int(pa.n))) {
        for (r in range(512)) {
            var half : float[2]
            for (hf in range(2)) {
                var acc : float[4]
                for (i in range(hf * 64, hf * 64 + 64)) {
                    acc[i % 4] += wn[int(pa.hh) + i * 512 + r] * h[i]
                }
                half[hf] = (acc[0] + acc[1]) + (acc[2] + acc[3])
            }
            hh[r] = half[0] + half[1]
        }
        var dec = 0.0
        for (j in range(128)) {
            let gb = step * 512 + j
            let gi = gx[gb] + hh[j] + wn[int(pa.lb) + j]
            let gf = gx[gb + 128] + hh[128 + j] + wn[int(pa.lb) + 128 + j]
            let gg = gx[gb + 256] + hh[256 + j] + wn[int(pa.lb) + 256 + j]
            let go = gx[gb + 384] + hh[384 + j] + wn[int(pa.lb) + 384 + j]
            max_gate = max(max_gate, max(max(abs(gi), abs(gf)), max(abs(gg), abs(go))))
            c[j] = 1.0 / (1.0 + exp(-gf)) * c[j] + 1.0 / (1.0 + exp(-gi)) * tanh(gg)
            h[j] = 1.0 / (1.0 + exp(-go)) * tanh(c[j])
            dec += wn[int(pa.fw) + j] * max(h[j], 0.0)
        }
        probs |> push(1.0 / (1.0 + exp(-(dec + wn[int(pa.fb)]))))
    }
    var st <- clone_to_move(h)
    st |> push_from(c)
    return <- VadLstmRef(probs <- probs, st <- st, max_gate = max_gate)
}
