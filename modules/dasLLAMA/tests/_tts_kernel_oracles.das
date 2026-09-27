// The TTS chain's kernel fixtures and CPU references the Vulkan and the Metal kernel cells share - the f16-lattice fills,
// the harmonic source's fixture and CPU chain, the long cumsum's two laws, the STFT and inverse STFT in double, and the
// row classes' in-test forms - with no device type and no backend require, so a build without either GPU module reads it.

options gen2
options _dasllama_internal = true

module _tts_kernel_oracles shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_tts_blocks
require daslib/jobque_boost

//! an f16-exact value off the seed: every product of two is exact in f32
def f16_at(seed : int; scale, bias : float) : float => float(float16(scale * float(seed) + bias))

//! `n` f16-exact patterned values: seed (i * seed_mul) % 47 through `f16_at`
def f16_fill(n, seed_mul : int; scale, bias : float) : array<float> {
    return <- [for (i in range(n)); f16_at((i * seed_mul) % 47, scale, bias)]
}

let TTS_SRC_NH = 3          //! the harmonics the source cells mix
let TTS_SRC_SR = 24000.0

//! the harmonic source cells' inputs and the CPU chain's outputs over them
struct TtsSrcFixture {
    up : int
    frames : int
    n : int
    nlow : int
    rows : int64                //! the samples the CPU source mixed
    f0 : array<float>
    nu : array<float>
    noise : array<float>        //! the captured noise rows [n][nh]
    lw : array<float>           //! the source linear's weight row [nh]
    lb : array<float>           //! its bias
    want_lowc : array<float>    //! the CPU carry's phase frames [nh][nlow]
    want_har : array<float>     //! the CPU sines through the source linear and tanh
}

//! the source fixture at `up` x `frames` on either resample law: an F0 with unvoiced frames, f16-exact initial phases, noise
//! rows and source linear, and the CPU `sine_source` chain's phase frames and mixed signal
def tts_src_fixture(torch : bool; up, frames : int) : TtsSrcFixture {
    var f = TtsSrcFixture(up = up, frames = frames, n = frames * up)
    f.nlow = int(resize_len(int64(f.n), 1.0 / float(up)))
    f.f0 <- [for (i in range(frames)); i % 3 == 2 ? 0.0 : float(float16(180.0 + 0.3 * float(i % 97)))]
    f.nu <- f16_fill(TTS_SRC_NH, 3, 0.1, 0.2)
    f.noise <- f16_fill(f.n * TTS_SRC_NH, 7, 0.4, -0.9)
    f.lw <- f16_fill(TTS_SRC_NH, 5, 0.3, 0.2)
    f.lb |> push(0.05)
    var lin = TtsLinear(nin = int64(TTS_SRC_NH), nout = 1l)
    lin.w := f.lw
    lin.b := f.lb
    let cfg = SineSourceCfg(upsample = int64(up), sample_rate = TTS_SRC_SR, n_harm = int64(TTS_SRC_NH), sine_amp = 0.1, noise_std = 0.003,
        voiced_thr = 10.0, torch_math = torch)
    var cr = SineSourceCarry()
    var sines : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        sine_carry_reset(cr, int64(TTS_SRC_NH), int64(f.nlow))
        f.rows = sine_source(cfg, f.f0, int64(frames), f.nu, 0l, int64(f.nlow), cr, f.noise, 0l, sines)
        linear_rows(lin, sines, int64(f.n), f.want_har)
        tanh_inplace(f.want_har, int64(f.n))
    }
    f.want_lowc <- [for (e in range(TTS_SRC_NH * f.nlow)); cr.lowc[int64(e / f.nlow) * cr.stride + int64(e % f.nlow)]]
    delete cr
    delete sines
    delete lin
    return <- f
}

//! the long cumsum's operands and the law's sum
struct TtsLongCumsum {
    low : array<float>    //! the low-rate phase increments of one harmonic, f16-exact
    want : array<float>   //! the law's running sum in radians at the source's upsample
    parted : float        //! the double sum and a plain f32 sum apart over the run, in cycles - the leg's control
}

//! `nlong` low-rate frames where a plain f32 sum and a double one part by whole cycles, summed on the torch law (the double
//! carry rounded once a frame) or the ONNX law (the plain f32 sum)
def tts_long_cumsum(torch : bool; up, nlong : int) : TtsLongCumsum {
    var r : TtsLongCumsum
    r.low <- [for (k in range(nlong)); float(float16(0.0071 + 0.0003 * float(k % 13)))]
    r.want |> resize(nlong)
    var acc64 = 0.0lf
    var accf = 0.0
    var accp = 0.0
    for (k in range(nlong)) {
        acc64 += double(r.low[k])
        accf = torch ? float(acc64) : accf + r.low[k]
        accp += r.low[k]
        r.want[k] = ((accf * 2.0) * PI) * float(up)
    }
    r.parted = abs(float(acc64) - accp)
    return <- r
}

//! the STFT in double - `bins` rows of `k` taps over the signal padded by `pad` on the reflect or the edge law, the
//! magnitude and phase rounded to f32 with the zero-imaginary phase rule: [frames][magnitudes | phases]
def stft_ref(har, wre, wim : array<float>; n, k, hop, pad, bins : int; reflect : bool) : array<float> {
    let frames = (n + 2 * pad - k) / hop + 1
    var want : array<float>
    want |> resize(frames * 2 * bins)
    for (f in range(frames)) {
        for (b in range(bins)) {
            var re = 0.0lf
            var im = 0.0lf
            for (tap in range(k)) {
                var src = f * hop + tap - pad
                if (src < 0) {
                    src = reflect ? -src : 0
                } elif (src >= n) {
                    src = reflect ? 2 * (n - 1) - src : n - 1
                }
                re += double(wre[b * k + tap]) * double(har[src])
                im += double(wim[b * k + tap]) * double(har[src])
            }
            want[f * 2 * bins + b] = float(sqrt(re * re + im * im + 1e-9lf))
            let imf = float(im)
            let ref = float(re)
            want[f * 2 * bins + bins + b] = imf == 0.0 ? (ref < 0.0 ? PI : 0.0) : atan2(imf, ref)
        }
    }
    return <- want
}

//! the inverse STFT's samples and how many of them carry a negligible envelope
struct TtsIstftRef {
    want : array<float>
    bare : int          //! the samples whose envelope is negligible, left undivided
}

//! the overlap-added inverse STFT in double over `tt` frames of log magnitudes and phases (`stride` apart, `bins` each),
//! the [k][bins] bases and the window, the envelope divided out under `envelope`
def istft_ref(y, wre, wim, window : array<float>; tt, stride, bins, k, hop, pad : int; envelope : bool) : TtsIstftRef {
    let n = (tt - 1) * hop + k - 2 * pad
    var r : TtsIstftRef
    r.want |> resize(n)
    for (i in range(n)) {
        let m = i + pad
        var acc = 0.0lf
        var env = 0.0lf
        for (tap in range(k)) {
            continue if (m < tap || (m - tap) % hop != 0)
            let q = (m - tap) / hop
            continue if (q >= tt)
            for (b in range(bins)) {
                let mag = exp(y[q * stride + b])
                let ph = sin(y[q * stride + bins + b])
                acc += double(mag * cos(ph) * wre[tap * bins + b]) - double(mag * sin(ph) * wim[tap * bins + b])
            }
            env += double(window[tap] * window[tap])
        }
        r.bare += env > 1e-11lf ? 0 : 1
        r.want[i] = float((envelope && env > 1e-11lf) ? acc / env : acc)
    }
    return <- r
}

//! the CPU concat rows `width` wide: x [rows][c], the residual columns under `with_res` (row stride rs, 0 = one row
//! broadcast), the f0 and noise columns at their row strides, zeros past them
def concat_ref(x, res, f0, nd : array<float>; rows, c, rc, rs, f0s, ns, width : int; with_res : bool) : array<float> {
    let rcw = with_res ? rc : 0
    var out : array<float>
    out |> resize(rows * width)
    for (r in range(rows)) {
        for (j in range(width)) {
            var v = 0.0
            if (j < c) {
                v = x[r * c + j]
            } elif (j < c + rcw) {
                v = res[r * rs + j - c]
            } elif (j == c + rcw) {
                v = f0[r * f0s]
            } elif (j == c + rcw + 1) {
                v = nd[r * ns]
            }
            out[r * width + j] = v
        }
    }
    return <- out
}

//! the CPU `styletts2_albert`'s embedding sum: word + position + type, added in that order, off slab element bases
def gather_ref(slab : array<float>; ids : array<int>; c, word_off, pos_off, type_off : int) : array<float> {
    var out : array<float>
    out |> resize(length(ids) * c)
    for (r in range(length(ids))) {
        for (j in range(c)) {
            out[r * c + j] = slab[word_off + ids[r] * c + j] + slab[pos_off + r * c + j] + slab[type_off + j]
        }
    }
    return <- out
}

//! the CPU `styletts2_durations`' raw sum in f32: the sigmoids of a row's first `nb` logits in column order, over `speed`
def sigsum_ref(logits : array<float>; rows, nb, ls : int; speed : float) : array<float> {
    var raw : array<float>
    raw |> resize(rows)
    for (r in range(rows)) {
        var acc = 0.0
        for (j in range(nb)) {
            acc += 1.0 / (1.0 + exp(-logits[r * ls + j]))
        }
        raw[r] = acc / speed
    }
    return <- raw
}
