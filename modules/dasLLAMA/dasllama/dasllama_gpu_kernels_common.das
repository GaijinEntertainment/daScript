options gen2
//! The kernels both GPU homes run, written once: a `class template` here carries a kernel's buffers, its
//! argument struct and its body, and each home stamps it under its own dispatch name.
module dasllama_gpu_kernels_common shared public
require dasllama/dasllama_lint public
require daslib/typemacro_boost
require math

struct GkAxpyArgs {
    nelem : uint
    scale : float
    zero : uint    //! 1 = o's old contents ignored
}

//! o = (zero ? 0 : o) + scale * y over nelem elements, one element an invocation
[ |> template_struct_instance]
class template GkAxpy {
    @ssbo @binding = 0 @role = "readwrite" o : array<float>
    @ssbo @binding = 1 @role = "read" y : array<float>
    @kargs ka : GkAxpyArgs

    def axpy_body {
        let i = gl_GlobalInvocationID.x
        if (i < ka.nelem) {
            let base = ka.zero != 0u ? 0.0 : o[i]
            o[i] = base + ka.scale * y[i]
        }
    }
}

struct GkRopeTabArgs {
    rows : uint   //! the rows roped, row r at the tables' row r past their element bases
    qs : uint     //! x's row stride
    col : uint    //! the roped span's first column (a fused [q | k | v] row's q or k slot)
    d : uint      //! the span's width, its heads side by side
    dh : uint     //! the head size
    coff : uint   //! the cos table [pos][dh / 2]'s element base in its binding
    soff : uint   //! the sin table's element base in its binding, laid out as the cos table
}

//! the rotary embedding over a span of rows in place from per-position tables: row r turns each pair of each head by
//! the tables' angle j at row r - NEOX the pairs (j, j + dh / 2), the CPU `rope_neox_tab_rows` (the vision mrope tables),
//! else the adjacent pairs (2j, 2j + 1), the CPU `rope_rows`; one pair an invocation over rows * d / 2
[ |> template_struct_instance]
class template GkRopeTab {
    @template_constant NEOX : bool = false
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" cos_t : array<float>
    @ssbo @binding = 2 @role = "read" sin_t : array<float>
    @kargs pa : GkRopeTabArgs

    def pair_first(hb, j : uint) : uint {
        static_if (NEOX) {
            return hb + j
        } else {
            return hb + 2u * j
        }
    }

    def pair_partner_dist(hh : uint) : uint {
        static_if (NEOX) {
            return hh
        } else {
            return 1u
        }
    }

    def rope_tab_body {
        let gid = gl_GlobalInvocationID.x
        let half = pa.d / 2u
        if (gid < pa.rows * half) {
            let r = gid / max(half, 1u)
            let pi = gid % max(half, 1u)
            let hh = max(pa.dh / 2u, 1u)
            let j = pi % hh
            let i0 = pair_first(r * pa.qs + pa.col + (pi / hh) * pa.dh, j)
            let i1 = i0 + pair_partner_dist(hh)
            let tb = r * hh + j
            let cv = cos_t[pa.coff + tb]
            let sv = sin_t[pa.soff + tb]
            let v0 = x[i0]
            let v1 = x[i1]
            x[i0] = v0 * cv - v1 * sv
            x[i1] = v0 * sv + v1 * cv
        }
    }
}

struct GkConcatArgs {
    c : uint          //! x's row width
    rc : uint         //! the residual columns, read only under `with_res`
    rs : uint         //! the residual rows' stride: 0 broadcasts one row to every position
    f0s : uint        //! the F0 column's row stride
    ns : uint         //! the noise column's row stride
    with_res : uint   //! 1 = the residual columns follow x
    width : uint      //! the output row width, the columns past the last input zero
    rows : uint
}

//! o [rows][width] = cat(x [rows][c], res at stride rs under `with_res`, f0 at stride f0s, nd at stride ns), zero-padded to
//! width - the CPU `concat_rows`; one element an invocation
[ |> template_struct_instance]
class template GkConcat {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "read" res : array<float>
    @ssbo @binding = 2 @role = "read" f0 : array<float>
    @ssbo @binding = 3 @role = "read" nd : array<float>
    @ssbo @binding = 4 @role = "write" o : array<float>
    @kargs ka : GkConcatArgs

    def concat_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < ka.rows * ka.width) {
            let w = max(ka.width, 1u)
            let r = gid / w
            let j = gid % w
            let rc = ka.with_res != 0u ? ka.rc : 0u
            var v = 0.0
            if (j < ka.c) {
                v = x[r * ka.c + j]
            } elif (j < ka.c + rc) {
                v = res[r * ka.rs + (j - ka.c)]
            } elif (j == ka.c + rc) {
                v = f0[r * ka.f0s]
            } elif (j == ka.c + rc + 1u) {
                v = nd[r * ka.ns]
            }
            o[gid] = v
        }
    }
}

struct GkPkRowsArgs {
    rows : uint
    width : uint   //! the floats a row copies
    ss : uint      //! src's row stride
    sc : uint      //! src's first column
    sr0 : uint     //! src's first row
    ds : uint      //! dst's row stride
    dc : uint      //! dst's first column
    dr0 : uint     //! dst's first row
}

//! `rows` rows of `width` floats from src [.][ss] at column sc, rows sr0.., into dst [.][ds] at column dc, rows dr0.. - the
//! Pocket chains' row copy (a conv's window behind its carry, a carry's tail to its head, the K and V columns into their
//! caches, the sample column out); the ELU stamp maps each value on the way (the CPU `elu_rows` before a conv); one element an invocation
[ |> template_struct_instance]
class template GkPkRows {
    @template_constant ELU : bool = false
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>
    @kargs pa : GkPkRowsArgs

    def pk_rows_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.rows * pa.width) {
            let w = max(pa.width, 1u)
            let r = gid / w
            let j = gid % w
            var v = src[(pa.sr0 + r) * pa.ss + pa.sc + j]
            static_if (ELU) {
                v = max(v, 0.0) + min(exp(v) - 1.0, 0.0)
            }
            dst[(pa.dr0 + r) * pa.ds + pa.dc + j] = v
        }
    }
}

struct GkSigSumArgs {
    t : uint       //! the rows
    nb : uint      //! the logits summed a row
    ls : uint      //! the logit rows' stride
    speed : float  //! the divisor of each row's sum
}

//! raw[r] = (the sum of sigmoid(logits[r][j]) over j < nb, in j order) / speed - the CPU `styletts2_durations`' raw
//! durations (the CPU divides; a reciprocal multiply rounds a raw duration apart from it); one row an invocation
[ |> template_struct_instance]
class template GkSigSum {
    @ssbo @binding = 0 @role = "read" logits : array<float>
    @ssbo @binding = 1 @role = "write" raw : array<float>
    @kargs pa : GkSigSumArgs

    def sigsum_body {
        let r = gl_GlobalInvocationID.x
        if (r < pa.t) {
            var acc = 0.0
            var j = 0u
            while (j < pa.nb) {
                acc += sigmoid_f32(logits[r * pa.ls + j])
                j++
            }
            raw[r] = acc / pa.speed
        }
    }
}

struct GkSourceArgs {
    n : uint            //! the samples, frames * up
    nlow : uint         //! the phase frames, `resize_len(n, 1 / up)`
    nh : uint           //! the harmonics
    up : uint
    sr : float
    sine_amp : float
    noise_std : float
    voiced_thr : float
    seed : uint         //! the own noise draw's key
    woff : uint         //! the source linear's weight row [nh] in the slab
    boff : uint         //! its bias [1] in the slab
}

//! the phase per frame lowc [nh][nlow] as the running sum of the increments - TORCH: the double accumulator as a two-float
//! sum carrying its rounding error, else the plain float sum; one workgroup, invocation h < nh walks its harmonic
[ |> template_struct_instance]
class template GkSrcCumsum {
    @template_constant TORCH : bool = false
    @ssbo @binding = 0 @role = "read" low : array<float>
    @ssbo @binding = 1 @role = "write" lowc : array<float>
    @kargs pa : GkSourceArgs

    def src_cumsum_body {
        let h = gl_LocalInvocationID.x
        if (h < pa.nh) {
            var acc = 0.0
            var lo = 0.0
            var k = 0u
            while (k < pa.nlow) {
                let x = low[h * pa.nlow + k]
                static_if (TORCH) {
                    tts_two_sum_add(acc, lo, x)
                } else {
                    acc += x
                }
                lowc[h * pa.nlow + k] = tts_phase_rad(acc, pa.up)
                k++
            }
        }
    }
}

//! the source's own noise draw noise_n [n][nh], `tts_hash_normal` at the seed - the captured stream's seat when no stream
//! was captured; one element an invocation
[ |> template_struct_instance]
class template GkSrcNoise {
    @ssbo @binding = 0 @role = "write" noise_n : array<float>
    @kargs pa : GkSourceArgs

    def src_noise_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.n * pa.nh) {
            noise_n[gid] = tts_hash_normal(pa.seed, gid)
        }
    }
}

struct GkStftArgs {
    n : uint        //! the signal's samples
    frames : uint
    bins : uint
    k : uint        //! the frame length, each basis row's taps
    hop : uint
    pad : uint      //! the samples padded on each side
    eps : float
    reoff : uint    //! the real basis wre [bins][k] in wn
    imoff : uint    //! the imaginary basis, the same layout
}

//! the STFT as two convs over the padded signal - REFLECT mirrors the signal past its ends, else the edge sample repeats -
//! into spec [frames][2 bins], the magnitudes sqrt(re^2 + im^2 + eps) then the phases, a zero imaginary part on the
//! negative real axis reading +pi (the CPU `magnitude_phase`); one (frame, bin) an invocation
[ |> template_struct_instance]
class template GkStft {
    @template_constant REFLECT : bool = false
    @ssbo @binding = 0 @role = "read" har : array<float>
    @ssbo @binding = 1 @role = "weight" wn : array<float>
    @ssbo @binding = 2 @role = "write" spec : array<float>
    @kargs pa : GkStftArgs

    def stft_body {
        let gid = gl_GlobalInvocationID.x
        let bins = max(pa.bins, 1u)
        if (gid < pa.frames * bins) {
            let f = gid / bins
            let b = gid % bins
            var re = 0.0
            var im = 0.0
            var tap = 0u
            while (tap < pa.k) {
                let p = int(f * pa.hop + tap) - int(pa.pad)
                var src = 0
                static_if (REFLECT) {
                    src = tts_pad_reflect(p, int(pa.n))
                } else {
                    src = clamp(p, 0, int(pa.n) - 1)
                }
                let v = har[uint(src)]
                re += wn[pa.reoff + b * pa.k + tap] * v
                im += wn[pa.imoff + b * pa.k + tap] * v
                tap++
            }
            let row = f * 2u * pa.bins
            spec[row + b] = tts_stft_mag(re, im, pa.eps)
            spec[row + pa.bins + b] = tts_stft_phase(re, im)
        }
    }
}

struct GkIstftArgs {
    tt : uint         //! the frames
    stride : uint     //! y's row width
    bins : uint
    k : uint          //! the frame length
    hop : uint
    pad : uint        //! the samples trimmed from each end
    envelope : uint   //! 1 = the summed squared window divided out where it is not negligible
    n : uint          //! the samples out
    reoff : uint      //! the real basis wre [k][bins] in wn - the transposed conv's operand layout
    imoff : uint      //! the imaginary basis, the same layout
    wnoff : uint      //! the window [k] in wn
}

//! the inverse STFT from conv_post's rows y [tt][stride] - a bin's log magnitude, then its phase - as the overlap-added
//! transposed conv, a non-negligible envelope divided out under `envelope`; one sample an invocation
[ |> template_struct_instance]
class template GkIstft {
    @ssbo @binding = 0 @role = "read" y : array<float>
    @ssbo @binding = 1 @role = "weight" wn : array<float>
    @ssbo @binding = 2 @role = "write" wave : array<float>
    @kargs pa : GkIstftArgs

    def istft_body {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            let m = i + pa.pad
            let hop = max(pa.hop, 1u)
            var acc = 0.0
            var env = 0.0
            var tap = 0u
            while (tap < pa.k) {
                if (m >= tap && (m - tap) % hop == 0u) {
                    let q = (m - tap) / hop
                    if (q < pa.tt) {
                        var b = 0u
                        while (b < pa.bins) {
                            let wa = tap * pa.bins + b
                            acc += tts_istft_term(y[q * pa.stride + b], y[q * pa.stride + pa.bins + b], wn[pa.reoff + wa], wn[pa.imoff + wa])
                            b++
                        }
                        let wv = wn[pa.wnoff + tap]
                        env += wv * wv
                    }
                }
                tap++
            }
            wave[i] = (pa.envelope != 0u && env > 1e-11) ? acc / env : acc
        }
    }
}

struct GkAddScaleArgs {
    nelem : uint
    scale : float
}

//! o = (a + b) * scale over nelem elements - the residual join's 1 / sqrt(2) and the averaged stage sums; o may be a;
//! one element an invocation
[ |> template_struct_instance]
class template GkAddScale {
    @ssbo @binding = 0 @role = "read" a : array<float>
    @ssbo @binding = 1 @role = "read" b : array<float>
    @ssbo @binding = 2 @role = "write" o : array<float>
    @kargs pa : GkAddScaleArgs

    def add_scale_body {
        let i = gl_GlobalInvocationID.x
        if (i < pa.nelem) {
            o[i] = (a[i] + b[i]) * pa.scale
        }
    }
}

//! the source's resample law - TORCH: torch's taps and mix, else onnxruntime's - the base of the source stamps; the taps'
//! quotients divide through `tts_div`
[ |> template_struct_instance]
class template GkSrcLaw {
    @template_constant TORCH : bool = false

    def law_taps(n, to : uint; scale : float; var i0, i1 : uint&; var l0, l1 : float&) {
        static_if (TORCH) {
            tts_resize_taps_torch(n, to, tts_div(1.0, scale), i0, i1, l0, l1)
        } else {
            tts_resize_taps_onnx(n, tts_div(float(to) + 0.5, scale), i0, i1, l0, l1)
        }
    }

    def law_mix(x0, x1, l0, l1 : float; last : bool) : float {
        static_if (TORCH) {
            return tts_resize_mix_torch(x0, x1, l0, l1)
        } else {
            return tts_resize_mix_onnx(x0, x1, l0, l1, last)
        }
    }
}

//! the low-rate phase increments low [nh][nlow] on the stamp's resample law, the first sample's initial phases folded in;
//! one element an invocation
[ |> template_struct_instance]
class template GkSrcLow : GkSrcLaw {
    @ssbo @binding = 0 @role = "read" f0 : array<float>
    @ssbo @binding = 1 @role = "read" noise_u : array<float>
    @ssbo @binding = 2 @role = "write" low : array<float>
    @kargs pa : GkSourceArgs

    def src_low_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.nh * pa.nlow) {
            let nlow = max(pa.nlow, 1u)
            let up = max(pa.up, 1u)
            let h = gid / nlow
            var i0 = 0u
            var i1 = 0u
            var l0 = 0.0
            var l1 = 0.0
            law_taps(pa.n, gid % nlow, tts_div(1.0, float(pa.up)), i0, i1, l0, l1)
            let r0 = tts_src_rad(tts_div(f0[i0 / up] * float(h + 1u), pa.sr), noise_u[h], i0, h)
            let r1 = tts_src_rad(tts_div(f0[i1 / up] * float(h + 1u), pa.sr), noise_u[h], i1, h)
            low[gid] = law_mix(r0, r1, l0, l1, h + 1u == pa.nh)
        }
    }
}

//! the mixed source signal har [n] on the stamp's resample law: the harmonics' sines at the interpolated phases, the noise
//! rows scaled by voicing, the source linear (its weight row [nh] at woff, its bias [1] at boff in wn) and its tanh; one
//! sample an invocation
[ |> template_struct_instance]
class template GkSrcSines : GkSrcLaw {
    @ssbo @binding = 0 @role = "read" f0 : array<float>
    @ssbo @binding = 1 @role = "read" lowc : array<float>
    @ssbo @binding = 2 @role = "read" noise_n : array<float>
    @ssbo @binding = 3 @role = "weight" wn : array<float>
    @ssbo @binding = 4 @role = "write" har : array<float>
    @kargs pa : GkSourceArgs

    def src_sines_body {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            var i0 = 0u
            var i1 = 0u
            var l0 = 0.0
            var l1 = 0.0
            law_taps(pa.nlow, i, float(pa.up), i0, i1, l0, l1)
            let uv = f0[i / max(pa.up, 1u)] > pa.voiced_thr ? 1.0 : 0.0
            let noise_amp = tts_noise_amp(uv, pa.noise_std, pa.sine_amp)
            var acc = wn[pa.boff]
            var h = 0u
            while (h < pa.nh) {
                let phase = law_mix(lowc[h * pa.nlow + i0], lowc[h * pa.nlow + i1], l0, l1, h + 1u == pa.nh)
                acc += tts_src_term(wn[pa.woff + h], tts_sin_big(phase) * pa.sine_amp, uv, noise_amp, noise_n[i * pa.nh + h])
                h++
            }
            har[i] = tts_tanh(acc)
        }
    }
}

let GK_ADAIN_EPS = 1e-5   //! the instance norms' eps - the body spells the literal (a kernel reads no module global); a driver holds it to its model's

struct GkAdainArgs {
    c : uint       //! the channels
    t : uint       //! the rows the statistics ran over
    hoff : uint    //! the style fc rows' base in h: gamma [c], then beta [c]
    gwoff : uint   //! the norm's scale row in wn
    gboff : uint   //! the norm's shift row in wn
    aoff : uint    //! Snake's alpha row in wn, read by the Snake stamp alone
}

//! AdaIN as the CPU `adain_affine` folds it - the column statistics, the norm's own scale and shift, then (1 + gamma) and
//! beta - followed by Snake or LeakyReLU(0.2); y may be x itself; one element an invocation over t * c
[ |> template_struct_instance]
class template GkAdain {
    @template_constant SNAKE : bool = false
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "read" stats : array<float>
    @ssbo @binding = 3 @role = "read" h : array<float>
    @ssbo @binding = 4 @role = "weight" wn : array<float>
    @kargs pa : GkAdainArgs

    def adain_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.t * pa.c) {
            let ci = gid % max(pa.c, 1u)
            var scale = 0.0
            var shift = 0.0
            tts_adain_affine(stats[ci], stats[pa.c + ci], float(pa.t), 1e-5, wn[pa.gwoff + ci], wn[pa.gboff + ci], h[pa.hoff + ci],
                h[pa.hoff + pa.c + ci], scale, shift)
            let v = mad(x[gid], scale, shift)
            static_if (SNAKE) {
                let a = wn[pa.aoff + ci]
                let sn = sin(a * v)
                y[gid] = mad(1.0 / a, sn * sn, v)
            } else {
                y[gid] = v > 0.0 ? v : v * 0.2
            }
        }
    }
}

struct GkDwConvRowsArgs {
    c : uint       //! the channels, one tap row each
    k : uint       //! the taps, where the stamp does not fix them
    stride : uint  //! the transposed stamp's stride
    pad_l : uint   //! the transposed stamp's cropped left edge
    dil : uint     //! the transposed stamp's dilation
    t_in : uint    //! the input rows, and the output rows of the causal and the centered stamps
    t_out : uint   //! the transposed stamp's output rows
    woff : uint    //! w [c][k]'s element base in wn
    boff : uint    //! the transposed stamp's bias row in wn
    bnwoff : uint  //! the centered stamp's folded BatchNorm scale row in wn
    bnboff : uint  //! the centered stamp's folded BatchNorm shift row in wn
}

//! a depthwise conv on rows, y [t_out][c] from x [t_in][c] and w [c][k] (w [k][c] under TAP_MAJOR, parakeet's load-time repack), one
//! element an invocation; FORM is the source rule and the epilogue: 0 the transposed conv gathered, bias first and one mad a tap (the CPU
//! `conv1d_rows_transposed_depthwise`); 1 the causal conv, a row reading the k - 1 rows before it (gemma4a); 2 the centered conv, folded BatchNorm and silu after it
[ |> template_struct_instance]
class template GkDwConvRows {
    @template_constant FORM : int = 0   //!< spelled as its literal: a template constant folds only a literal
    @template_constant K : uint = 0u    //!< the taps where the stamp fixes them, else 0 and the taps are `pa.k`
    @template_constant TAP_MAJOR : bool = false
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "weight" wn : array<float>
    @kargs pa : GkDwConvRowsArgs

    def rows_out : uint {
        static_if (FORM == 0) {
            return pa.t_out
        } else {
            return pa.t_in
        }
    }

    def taps : uint => gk_fixed_or(K, pa.k)

    def src_row(t, kk : uint) : int {
        static_if (FORM == 0) {
            return conv_src_tr_row(t, kk, 0u, pa.pad_l, pa.dil, max(pa.stride, 1u), pa.t_in)
        } else {
            static_if (FORM == 1) {
                return conv_src_fwd_row(t, kk, 0u, taps() - 1u, 1u, 1u, pa.t_in)
            } else {
                return conv_src_fwd_row(t, kk, 0u, (taps() - 1u) / 2u, 1u, 1u, pa.t_in)
            }
        }
    }

    def tap(j, kk, nk : uint) : float {
        static_if (TAP_MAJOR) {
            return wn[pa.woff + kk * pa.c + j]
        } else {
            return wn[pa.woff + j * nk + kk]
        }
    }

    def dw_conv_rows_body {
        let e = gl_GlobalInvocationID.x
        if (e < rows_out() * pa.c) {
            let c = max(pa.c, 1u)
            let t = e / c
            let j = e % c
            let nk = taps()
            var acc = 0.0
            static_if (FORM == 0) {
                acc = wn[pa.boff + j]
            }
            var kk = 0u
            while (kk < nk) {
                let src = src_row(t, kk)
                if (src >= 0) {
                    static_if (FORM == 0) {
                        acc = mad(tap(j, kk, nk), x[uint(src) * pa.c + j], acc)
                    } else {
                        acc += tap(j, kk, nk) * x[uint(src) * pa.c + j]
                    }
                }
                kk++
            }
            static_if (FORM == 2) {
                y[e] = silu_f32(acc * wn[pa.bnwoff + j] + wn[pa.bnboff + j])
            } else {
                y[e] = acc
            }
        }
    }
}

//! the workgroup reductions every row kernel of either home shares: one barrier a reduce, every thread summing the subgroup
//! partials in the same order, so every thread holds the same sum or max; the lane primitives are each home's
//! `gk_subgroup_add` / `gk_subgroup_max`, and the result lands by reference, the statement form the MSL emitter splices
[ |> template_struct_instance]
class template GkWgReduce {
    @workgroup part : float[128]   //! two slots of 64 subgroup partials, one slot a reduce

    //! the workgroup sum every thread holds alike; `slot` alternates 0 / 1 between consecutive reduces; a statement with its
    //! result by reference, the form the MSL emitter inlines
    def wg_sum_into(v0 : float; slot : uint; var tot : float&) {
        let ss = gk_subgroup_add(v0)
        if (gl_SubgroupInvocationID == 0u) {
            part[slot * 64u + gl_SubgroupID] = ss
        }
        barrier()
        tot = 0.0
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            tot += part[slot * 64u + sgi]
            sgi++
        }
    }

    //! the workgroup max every thread holds alike
    def wg_max_into(v0 : float; slot : uint; var m : float&) {
        let sm = gk_subgroup_max(v0)
        if (gl_SubgroupInvocationID == 0u) {
            part[slot * 64u + gl_SubgroupID] = sm
        }
        barrier()
        m = -3.0e38
        var sgi = 0u
        while (sgi < gl_NumSubgroups) {
            m = max(m, part[slot * 64u + sgi])
            sgi++
        }
    }
}

let GK_PK_ATTN_HS = 64u            //! the head size the attention row is stamped for, the body's literal: another head size declines at the driver
let GK_PK_ATTN_MAX_KEYS = 2048u    //! the scores a row stages, the body's literal: a longer key range declines at the driver

struct GkPkAttnArgs {
    t : uint      //! the query rows
    qs : uint     //! q's row stride
    qcol : uint   //! the query span's first column in q's rows
    d : uint      //! the heads' width, the cache rows' and o's row stride
    pos0 : uint   //! the first query's position
    ctx : uint    //! the keys a query sees at most, 0 = every key up to itself
}

//! the causal cached attention of one query row over the caches' rows (the CPU `attention_causal_rows`): query i at position
//! pos0 + i sees the keys up to itself, the last `ctx` of them when ctx is nonzero; the scaled query staged, the scores staged
//! and softmaxed in two passes, the value sum in four parts of 64 columns; a workgroup of 256 a (head, row), reductions as statement methods
[ |> template_struct_instance]
class template GkPkAttn : GkWgReduce {
    @ssbo @binding = 0 @role = "read" q : array<float>
    @ssbo @binding = 1 @role = "read" k : array<float>
    @ssbo @binding = 2 @role = "read" v : array<float>
    @ssbo @binding = 3 @role = "write" o : array<float>
    @kargs pa : GkPkAttnArgs
    @workgroup qh : float[64]
    @workgroup sc : float[2048]   //! == GK_PK_ATTN_MAX_KEYS, the array's literal
    @workgroup po : float[256]
    //! a column's quarter of the value sum: the keys `quarter`, `quarter` + 4, ... in order, eight a step with their loads
    //! in flight together, the column at `vb` in the first key's row
    def value_sum_into(vb, quarter, nk : uint; var acc : float&) {
        acc = 0.0
        var jj = quarter
        while (jj + 28u < nk) {
            let v0 = v[vb + jj * pa.d]
            let v1 = v[vb + (jj + 4u) * pa.d]
            let v2 = v[vb + (jj + 8u) * pa.d]
            let v3 = v[vb + (jj + 12u) * pa.d]
            let v4 = v[vb + (jj + 16u) * pa.d]
            let v5 = v[vb + (jj + 20u) * pa.d]
            let v6 = v[vb + (jj + 24u) * pa.d]
            let v7 = v[vb + (jj + 28u) * pa.d]
            acc += sc[jj] * v0
            acc += sc[jj + 4u] * v1
            acc += sc[jj + 8u] * v2
            acc += sc[jj + 12u] * v3
            acc += sc[jj + 16u] * v4
            acc += sc[jj + 20u] * v5
            acc += sc[jj + 24u] * v6
            acc += sc[jj + 28u] * v7
            jj += 32u
        }
        while (jj < nk) {
            acc += sc[jj] * v[vb + jj * pa.d]
            jj += 4u
        }
    }

    def pk_attn_body {
        let wg = gl_WorkGroupID.x
        let t = max(pa.t, 1u)
        let h = wg / t
        let i = wg % t
        let lid = gl_LocalInvocationID.x
        let p = pa.pos0 + i
        let j0 = tts_window_start(p, pa.ctx)
        let nk = p + 1u - j0
        if (lid < 64u) {
            qh[lid] = q[i * pa.qs + pa.qcol + h * 64u + lid] / sqrt(float(64u))
        }
        barrier()
        var m = -1.0e30
        var j = lid
        while (j < nk) {
            let krow = (j0 + j) * pa.d + h * 64u
            var sdot = 0.0
            var e = 0u
            while (e < 64u) {
                let k0 = k[krow + e]
                let k1 = k[krow + e + 1u]
                let k2 = k[krow + e + 2u]
                let k3 = k[krow + e + 3u]
                sdot += qh[e] * k0
                sdot += qh[e + 1u] * k1
                sdot += qh[e + 2u] * k2
                sdot += qh[e + 3u] * k3
                e += 4u
            }
            sc[j] = sdot
            m = max(m, sdot)
            j += 256u
        }
        var mall = 0.0
        wg_max_into(m, 0u, mall)
        var l = 0.0
        j = lid
        while (j < nk) {
            let pw = exp(sc[j] - mall)
            sc[j] = pw
            l += pw
            j += 256u
        }
        var lsum = 0.0
        wg_sum_into(l, 1u, lsum)
        let inv = 1.0 / lsum
        j = lid
        while (j < nk) {
            sc[j] = sc[j] * inv
            j += 256u
        }
        barrier()
        let col = lid % 64u
        let quarter = lid / 64u
        var vs = 0.0
        value_sum_into(j0 * pa.d + h * 64u + col, quarter, nk, vs)
        po[quarter * 64u + col] = vs
        barrier()
        if (lid < 64u) {
            var tot = 0.0
            var pp = 0u
            while (pp < 4u) {
                tot += po[pp * 64u + lid]
                pp++
            }
            o[i * pa.d + h * 64u + lid] = tot
        }
    }
}

struct GkGatedRowsArgs {
    d : uint        //! the output row width; the input rows are 2 d wide, the gate in their second half
    nelem : uint    //! rows x d
}

//! the gated halves of a row, one element an invocation: FORM 0 the conv module's GLU, first . sigmoid(second) (gemma4a, the
//! FastConformers); FORM 1 the ultravox projector's swapped SwiGLU, silu(second) . first
[ |> template_struct_instance]
class template GkGatedRows {
    @template_constant FORM : int = 0
    @ssbo @binding = 0 @role = "read" h : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @kargs pa : GkGatedRowsArgs

    def gated_rows_body {
        let e = gl_GlobalInvocationID.x
        if (e < pa.nelem) {
            let dd = max(1u, pa.d)
            let t = e / dd
            let c = e % dd
            let a = h[t * 2u * pa.d + c]
            let b = h[t * 2u * pa.d + pa.d + c]
            static_if (FORM == 0) {
                y[e] = sig_gate(a, b)
            } else {
                y[e] = silu_f32(b) * a
            }
        }
    }
}

struct GkPoolRowsArgs {
    width : uint   //! the OUTPUT row's width
    total : uint   //! output elements
}

//! the whisper-class projector tails' pair pool: y [rows x width] the mean of x's row pairs [2 rows x width]
[ |> template_struct_instance]
class template GkPoolRows {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @kargs pa : GkPoolRowsArgs

    def pool_rows_body {
        let e = gl_GlobalInvocationID.x
        if (e < pa.total) {
            let w = max(1u, pa.width)
            let at = (e / w) * 2u * w + e % w
            y[e] = 0.5 * (x[at] + x[at + w])
        }
    }
}

struct GkLnReluRowsArgs {
    cout : uint    //! the channels a row normalizes over
    ldin : uint    //! x's row stride (the conv GEMM's padded width)
    ldout : uint   //! y's row stride
    rows : uint
    woff : uint    //! the weight row's base in wn
    eps : float
}

//! the subsample convs' LayerNorm over a row's channels (weight only, biased variance) then ReLU, one thread a row: y's compact
//! rows off x's padded ones, or in place at one stride (gemma4a's front on both homes)
[ |> template_struct_instance]
class template GkLnReluRows {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "weight" wn : array<float>
    @kargs pa : GkLnReluRowsArgs

    def ln_relu_rows_body {
        let row = gl_GlobalInvocationID.x
        if (row < pa.rows) {
            let base = row * pa.ldin
            let n = float(max(1u, pa.cout))
            var mean = 0.0
            for (c in urange(pa.cout)) {
                mean += x[base + c]
            }
            mean /= n
            var vv = 0.0
            for (c in urange(pa.cout)) {
                let dv = x[base + c] - mean
                vv += dv * dv
            }
            let inv = rsqrt(vv / n + pa.eps)
            let obase = row * pa.ldout
            for (c in urange(pa.cout)) {
                y[obase + c] = max((x[base + c] - mean) * inv * wn[pa.woff + c], 0.0)
            }
        }
    }
}

struct GkFeatShuffleArgs {
    w : uint         //! the frequency rows a time step keeps
    ch : uint        //! the channels
    t : uint         //! the time steps
    rows_pad : uint  //! the rows written: t, or past it the GEMM's zero row pad
}

//! the subsample stack's feature rows: out[t][f + w c] = in[(t w + f) ch + c] - the [time][freq][ch] image as [time][ch][freq] rows,
//! the rows past t zero
[ |> template_struct_instance]
class template GkFeatShuffle {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>
    @kargs pa : GkFeatShuffleArgs

    def feat_shuffle_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.rows_pad * pa.w * pa.ch) {
            let wc = max(1u, pa.w * pa.ch)
            let w = max(1u, pa.w)
            let t = gid / wc
            if (t >= pa.t) {
                dst[gid] = 0.0
            } else {
                let rem = gid % wc
                let c = rem / w
                let f = rem % w
                dst[gid] = src[(t * pa.w + f) * pa.ch + c]
            }
        }
    }
}

struct GkRelSinCosArgs {
    d : uint         //! the row width, the (sin, cos) pairs interleaved
    tt : uint        //! the encoder rows: the table's 2 tt - 1 rows, row w carrying the relative position tt - 1 - w
    rows_pad : uint  //! the rows written: 2 tt - 1, or past it the GEMM's zero row pad
}

//! the Transformer-XL rel-position table [ww x d]: row w = the sinusoid of tt - 1 - w at the d / 2 frequencies, (sin, cos)
//! interleaved - the CPU `rel_pos_table`'s form; the rows past 2 tt - 1 zero
[ |> template_struct_instance]
class template GkRelSinCos {
    @ssbo @binding = 0 @role = "write" tab : array<float>
    @kargs pa : GkRelSinCosArgs

    def rel_sincos_body {
        let gid = gl_GlobalInvocationID.x
        let d = max(1u, pa.d)
        if (pa.tt > 0u && gid < pa.rows_pad * pa.d) {
            let w = gid / d
            if (w >= 2u * pa.tt - 1u) {
                tab[gid] = 0.0
            } else {
                let e = gid % d
                let k = e / 2u
                let rel = float(int(pa.tt) - 1 - int(w))
                let freq = exp(-(float(k * 2u) * 9.210340371976184 / float(d)))
                let theta = freq * rel
                tab[gid] = (e & 1u) == 0u ? sin(theta) : cos(theta)
            }
        }
    }
}

struct GkDwConv2dArgs {
    iw : uint        //! the source block's width, height and channels ([ih x iw x ch], c fastest)
    ih : uint
    ih_valid : uint  //! the rows a tap may read - the length mask: the rows past them add nothing
    ch : uint
    woff : uint    //! the taps [ch x 9] in wn, (kh 3 + kw), or [9 x ch] under TAP_MAJOR
    boff : uint    //! the bias row [ch] in wn
}

//! a depthwise conv2d k3 s2 p1 with its bias (the FastConformer subsample stacks), bias first, one thread an output element;
//! a tap off the image adds nothing
[ |> template_struct_instance]
class template GkDwConv2d {
    @template_constant TAP_MAJOR : bool = false
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @ssbo @binding = 2 @role = "weight" wn : array<float>
    @kargs pa : GkDwConv2dArgs

    def tap2d(c, k9 : uint) : float {
        static_if (TAP_MAJOR) {
            return wn[pa.woff + k9 * pa.ch + c]
        } else {
            return wn[pa.woff + c * 9u + k9]
        }
    }

    def dw_conv2d_body {
        let gid = gl_GlobalInvocationID.x
        let ch = max(1u, pa.ch)
        let ow = (pa.iw - 1u) / 2u + 1u
        let oh = (pa.ih - 1u) / 2u + 1u
        if (gid < oh * ow * pa.ch) {
            let c = gid % ch
            let o = gid / ch
            let oy = o / ow
            let ox = o % ow
            var acc = wn[pa.boff + c]
            var kh = 0u
            while (kh < 3u) {
                let iy = int(oy * 2u + kh) - 1
                if (iy >= 0 && iy < int(pa.ih_valid)) {
                    var kw = 0u
                    while (kw < 3u) {
                        let ix = int(ox * 2u + kw) - 1
                        if (ix >= 0 && ix < int(pa.iw)) {
                            acc += tap2d(c, kh * 3u + kw) * x[(uint(iy) * pa.iw + uint(ix)) * ch + c]
                        }
                        kw++
                    }
                }
                kh++
            }
            y[gid] = acc
        }
    }
}

let GK_G4A_ATTN_PAST = 12u   //! gemma4a's causal window - self and eleven past rows: `GkG4aAttn`'s PAST default spells it, each driver verifies the family's value against it
let GK_G4A_ATTN_CAP = 50.0   //! gemma4a's score soft cap, verified the same way

struct GkG4aAttnArgs {
    d : uint        //! the row width (compact [npos x d] rows)
    npos : uint
    hs : uint       //! the head width where the stamp leaves HS at 0
    pdsoff : uint   //! the per-dim query scale row's base in wn
    cap : float     //! the score soft cap
    qscale : float  //! the query scale ahead of the per-dim scale, (1 / sqrt(hs)) / log 2 - the CPU chain's log2-domain softmax
    kscale : float  //! the key scale, log(1 + e) / log 2
}

//! gemma4a's causal sliding-window attention with the layer's projected RPE table: one invocation a (query row, head), the
//! window sixteen rows at most (the score array; gemma4a's window is twelve), the head width the stamp's HS or `pa.hs`;
//! the query and key scales come from the host (`gemma4a_attn_scales`), the one rounding of them both homes share
[ |> template_struct_instance]
class template GkG4aAttn {
    @template_constant HS : uint = 0u     //!< the head width served, 0 for `pa.hs`
    @template_constant PAST : uint = 12u  //!< the causal window: `GK_G4A_ATTN_PAST`, spelled as its literal
    @ssbo @binding = 0 @role = "read" q : array<float>
    @ssbo @binding = 1 @role = "read" k : array<float>
    @ssbo @binding = 2 @role = "read" v : array<float>
    @ssbo @binding = 3 @role = "write" ao : array<float>
    @ssbo @binding = 4 @role = "read" rpe : array<float>
    @ssbo @binding = 5 @role = "weight" wn : array<float>
    @kargs pa : GkG4aAttnArgs

    def head_width : uint => gk_fixed_or(HS, pa.hs)

    def g4a_attn_body {
        let gid = gl_GlobalInvocationID.x
        let npos = max(1u, pa.npos)
        let i = gid % npos
        let h = gid / npos
        let hs = head_width()
        if (h * hs >= pa.d) {
            return
        }
        let d = pa.d
        let hoff = h * hs
        let qscale = pa.qscale
        let kscale = pa.kscale
        let j0 = i >= PAST - 1u ? i - (PAST - 1u) : 0u
        var sarr : float[16]
        var m = -1.5e38
        var j = j0
        while (j <= i) {
            let rr = (PAST - (i - j)) * d
            var cs = 0.0
            var rs = 0.0
            var n = 0u
            while (n < hs) {
                let qv = q[i * d + hoff + n] * qscale * wn[pa.pdsoff + n]
                cs += qv * (k[j * d + hoff + n] * kscale)
                rs += qv * rpe[rr + hoff + n]
                n++
            }
            let sc = softcap_exp(cs + rs, pa.cap)
            sarr[j - j0] = sc
            m = max(m, sc)
            j++
        }
        var stot = 0.0
        j = j0
        while (j <= i) {
            let p = exp(sarr[j - j0] - m)
            sarr[j - j0] = p
            stot += p
            j++
        }
        let inv = 1.0 / stot
        var n = 0u
        while (n < hs) {
            var acc = 0.0
            j = j0
            while (j <= i) {
                acc += sarr[j - j0] * v[j * d + hoff + n]
                j++
            }
            ao[i * d + hoff + n] = acc * inv
            n++
        }
    }
}

struct GkRope2dArgs {
    nheads : uint
    hs : uint       //! the head width: the compact row is nheads x hs wide, the rotated pairs per axis hs / 4
    rows : uint
}

//! the 2D neox rope over q or k rows in place, the CPU `rope_neox_2d_rows`'s form: per head the x axis rotates the
//! pairs (j, j + pairs) of its first half against the row's x table, the y axis the same pairs of its second half
//! against the y table (one packed table row is [cx | sx | cy | sy], `pairs` each) - one thread per (row, head, pair), both axes
[ |> template_struct_instance]
class template GkRope2d {
    @ssbo @binding = 0 @role = "readwrite" v : array<float>
    @ssbo @binding = 1 @role = "read" tabs : array<float>
    @kargs pa : GkRope2dArgs

    def rope2d_body {
        let gid = gl_GlobalInvocationID.x
        let pairs = pa.hs / 4u
        let d = pa.nheads * pa.hs
        let per_row = max(1u, pa.nheads * pairs)
        if (gid < pa.rows * per_row) {
            let row = gid / per_row
            let hp = gid % per_row
            let base = row * d + (hp / max(1u, pairs)) * pa.hs
            let j = hp % max(1u, pairs)
            let tb = row * 4u * pairs
            let x0 = v[base + j]
            let x1 = v[base + j + pairs]
            let cx = tabs[tb + j]
            let sx = tabs[tb + pairs + j]
            v[base + j] = x0 * cx - x1 * sx
            v[base + j + pairs] = x0 * sx + x1 * cx
            let yb = base + pa.hs / 2u
            let y0 = v[yb + j]
            let y1 = v[yb + j + pairs]
            let cy = tabs[tb + 2u * pairs + j]
            let sy = tabs[tb + 3u * pairs + j]
            v[yb + j] = y0 * cy - y1 * sy
            v[yb + j + pairs] = y0 * sy + y1 * cy
        }
    }
}

struct GkWdecAttnArgs {
    dim : uint       //! a q row's width
    tstride : uint   //! the K/V planes' positions a head
    len0 : uint      //! the keys row 0 attends; a causal dispatch adds the row index
    causal : uint
    nheads : uint
    nrows : uint
    koff : uint      //! the layer's element base in kk / vv
    voff : uint
    qboff : uint     //! the q bias row's base in wn, folded at the load
    hs : uint        //! the head width where the stamp leaves HS at 0
    scale : float    //! the score scale, 1 / sqrt(hs)
}

//! the whisper decoder's chunked attention, the partials: one workgroup a (chunk, head, row) over 256 keys - the q bias folded
//! into the dot, the chunk's scores staged, their max and exp-sum landed as the (m, s) slot, the unnormalized weighted V as the
//! slot's row; the combine below merges the chunks by log-sum-exp
[ |> template_struct_instance]
class template GkWdecAttnPart : GkWgReduce {
    @template_constant HS : uint = 0u   //!< the head width served, 0 for `pa.hs`
    @ssbo @binding = 0 @role = "read" q : array<float>
    @ssbo @binding = 1 @role = "read" kk : array<float16>
    @ssbo @binding = 2 @role = "read" vv : array<float16>
    @ssbo @binding = 3 @role = "write" pms : array<float>    //! [nch][heads][rows][2]: the chunk's max and sum
    @ssbo @binding = 4 @role = "write" py : array<float>     //! [nch][heads][rows][hs]
    @ssbo @binding = 5 @role = "weight" wn : array<float>
    @kargs pa : GkWdecAttnArgs
    @workgroup scores : float[256]

    def head_width : uint => gk_fixed_or(HS, pa.hs)

    def wdec_attn_part_body {
        let hs = head_width()
        let nhr = max(1u, pa.nheads * pa.nrows)
        let nr = max(1u, pa.nrows)
        let c = gl_WorkGroupID.x / nhr
        let rem = gl_WorkGroupID.x % nhr
        let h = rem / nr
        let r = rem % nr
        let lid = gl_LocalInvocationID.x
        let len = pa.causal != 0u ? pa.len0 + r : pa.len0
        let tb = c * 256u
        let te = min(tb + 256u, len)
        let nloc = te > tb ? te - tb : 0u
        let slot = (c * pa.nheads + h) * pa.nrows + r
        let qbase = r * pa.dim + h * hs
        let kbase = pa.koff + h * pa.tstride * hs
        let vbase = pa.voff + h * pa.tstride * hs
        var t = lid
        while (t < nloc) {
            var s = 0.0
            var j = 0u
            while (j < hs) {
                s += (q[qbase + j] + wn[pa.qboff + h * hs + j]) * float(kk[kbase + (tb + t) * hs + j])
                s += (q[qbase + j + 1u] + wn[pa.qboff + h * hs + j + 1u]) * float(kk[kbase + (tb + t) * hs + j + 1u])
                s += (q[qbase + j + 2u] + wn[pa.qboff + h * hs + j + 2u]) * float(kk[kbase + (tb + t) * hs + j + 2u])
                s += (q[qbase + j + 3u] + wn[pa.qboff + h * hs + j + 3u]) * float(kk[kbase + (tb + t) * hs + j + 3u])
                j += 4u
            }
            scores[t] = s * pa.scale
            t += gl_WorkGroupSize.x
        }
        barrier()
        var mx = -3.0e38
        t = lid
        while (t < nloc) {
            mx = max(mx, scores[t])
            t += gl_WorkGroupSize.x
        }
        var m = 0.0
        wg_max_into(mx, 0u, m)
        var ss = 0.0
        t = lid
        while (t < nloc) {
            let e = exp(scores[t] - m)
            scores[t] = e
            ss += e
            t += gl_WorkGroupSize.x
        }
        var tot = 0.0
        wg_sum_into(ss, 1u, tot)   // its barrier also lands every lane's exponentials before the output loop reads them across lanes
        if (lid == 0u) {
            pms[slot * 2u] = nloc > 0u ? m : -3.0e38
            pms[slot * 2u + 1u] = tot
        }
        var j = lid
        while (j < hs) {
            var acc = 0.0
            var tt = 0u
            while (tt < nloc) {
                acc += scores[tt] * float(vv[vbase + (tb + tt) * hs + j])
                tt++
            }
            py[slot * hs + j] = acc
            j += gl_WorkGroupSize.x
        }
    }
}

//! the Q8_0 block store every requant stamp shares - under OUT_Q8 the stamp binds `outq` and `outs`, and the store's text lives
//! (a stamp off the gate carries no such binding and none of the text): eight consecutive lanes hold a block, `blk_pass` quantizing
//! a row's blocks from `normed_at`, the stamp's value for a plane element under the row's statistics
[ |> template_struct_instance]
class template GkQ8BlockStore : GkWgReduce {
    @template_constant OUT_Q8 : bool = false   //!< the stamp quantizes its rows into Q8_0 blocks: `outq` and the block scales `outs`

    [arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def blk_store(b, lane8 : uint; v : float4) {
        static_if (OUT_Q8) {
            let m = q8_amax3(max(max(abs(v.x), abs(v.y)), max(abs(v.z), abs(v.w))))
            let d = m / 127.0
            let id = d != 0.0 ? 1.0 / d : 0.0
            if (lane8 == 0u) {
                outs[b] = d
            }
            let qb = b * KQ_B32_ELEMS_U + lane8 * 4u
            outq[qb] = q8_quant1(v.x, id)
            outq[qb + 1u] = q8_quant1(v.y, id)
            outq[qb + 2u] = q8_quant1(v.z, id)
            outq[qb + 3u] = q8_quant1(v.w, id)
        }
    }

    def normed_at(e : uint; mean, inv : float) : float => 0.0   //! the normed value at plane element `e` of the row whose statistics are `mean` and `inv`

    //! the Q8_K superblock `b` from `normed_at` under `inv`: a 32-lane subgroup the superblock, lane `lane` its eight elements from `e0`, 32 apart
    [arch(at="../ARCHITECTURE_GPU_VULKAN.md#q8-requant-byte-store")]
    def sblk_store(b, lane, e0 : uint; inv : float) {
        static_if (OUT_Q8) {
            var vals : float[8]
            var m = 0.0
            for [unroll] (r in range(8)) {
                let v = normed_at(e0 + uint(r) * 32u, 0.0, inv)
                vals[r] = v
                m = max(m, abs(v))
            }
            let d = q8k_amax5(m) / 127.0
            if (lane == 0u) {
                outs[b] = d
            }
            let id = d != 0.0 ? 1.0 / d : 0.0
            for [unroll] (r in range(8)) {
                outq[e0 + uint(r) * 32u] = q8_quant1(vals[r], id)
            }
        }
    }

    //! the row's `nblk` blocks from plane element `base` quantized from `normed_at` into the blocks from `qb0` (32 blocks a pass over 256 lanes)
    def blk_pass(base, nblk, qb0 : uint; mean, inv : float) {
        static_if (OUT_Q8) {
            let tid = gl_LocalInvocationID.x
            let lane = tid % 32u
            var b = (tid / 32u) * 4u + lane / 8u
            while (b < nblk) {
                let e = base + b * KQ_B32_ELEMS_U + (lane % 8u) * 4u
                let v = float4(normed_at(e, mean, inv), normed_at(e + 1u, mean, inv), normed_at(e + 2u, mean, inv), normed_at(e + 3u, mean, inv))
                blk_store(qb0 + b, lane % 8u, v)
                b += 32u
            }
        }
    }
}

struct GkWdecCombArgs {
    dim : uint
    nchunks : uint
    nheads : uint
    nrows : uint
    hs : uint        //! the head width where the stamp leaves HS at 0
}

//! the chunked attention's combine over one row's head slice: the chunks' (m, s) slots merged by log-sum-exp, the lanes
//! striding the head's columns; the OUT_Q8 stamp (Vulkan's decoder feed, a head of 64 over 64 lanes) quantizes the slice into its
//! two Q8_0 blocks in place of the f32 store
[ |> template_struct_instance]
class template GkWdecAttnComb : GkQ8BlockStore {
    @template_constant HS : uint = 0u   //!< the head width served, 0 for `pa.hs`
    @ssbo @binding = 0 @role = "read" pms : array<float>
    @ssbo @binding = 1 @role = "read" py : array<float>
    @ssbo @binding = 2 @role = "write" @template_gate = "!OUT_Q8" y : array<float>
    @ssbo @binding = 6 @role = "write" @template_gate = OUT_Q8 outq : array<int8>
    @ssbo @binding = 7 @role = "write" @template_gate = OUT_Q8 outs : array<float>
    @kargs pa : GkWdecCombArgs
    @workgroup @template_gate = OUT_Q8 slice : float[64]   //! the OUT_Q8 stamp's row slice ahead of its block store: 64 lanes, a head of 64

    def head_width : uint => gk_fixed_or(HS, pa.hs)

    def wdec_attn_comb_body {
        let hs = head_width()
        let nr = max(1u, pa.nrows)
        let h = gl_WorkGroupID.x / nr
        let r = gl_WorkGroupID.x % nr
        let lid = gl_LocalInvocationID.x
        var mg = -3.0e38
        var c = 0u
        while (c < pa.nchunks) {
            mg = max(mg, pms[((c * pa.nheads + h) * pa.nrows + r) * 2u])
            c++
        }
        var tot = 0.0
        c = 0u
        while (c < pa.nchunks) {
            let slot = (c * pa.nheads + h) * pa.nrows + r
            tot += pms[slot * 2u + 1u] * exp(pms[slot * 2u] - mg)
            c++
        }
        let inv = 1.0 / max(tot, 1.0e-30)
        var j = lid
        while (j < hs) {
            var acc = 0.0
            c = 0u
            while (c < pa.nchunks) {
                let slot = (c * pa.nheads + h) * pa.nrows + r
                acc += py[slot * hs + j] * exp(pms[slot * 2u] - mg)
                c++
            }
            static_if (OUT_Q8) {
                slice[j] = acc * inv
            } else {
                y[r * pa.dim + h * hs + j] = acc * inv
            }
            j += gl_WorkGroupSize.x
        }
        static_if (OUT_Q8) {
            barrier()
            if (lid < hs / 4u) {
                let b0 = (r * pa.dim + h * hs) / KQ_B32_ELEMS_U
                let v = float4(slice[lid * 4u], slice[lid * 4u + 1u], slice[lid * 4u + 2u], slice[lid * 4u + 3u])
                blk_store(b0 + lid / 8u, lid % 8u, v)
            }
        }
    }
}

struct GkRestrideArgs {
    rows : uint
    rows_pad : uint //! the padded panel's row cap (the scratch's, a multiple of the l column - `vt_cap_rows`): PAD writes zero rows past `rows` up to it, the unpad reads `rows`
    nheads : uint
    hs : uint
    hsp : uint
    sstride : uint  //! the compact rows' stride and the column their heads start at (a fused [q | k | v] row: 3d and 0 / d / 2d)
    soff : uint
    boff : uint     //! the BIAS stamps: the projection's bias row in wn, added as the compact row is read
    doff : uint     //! the padded panel's element base in its binding (the pad direction)
}

//! the head restride between the compact rows [rows x heads x hs] (at `sstride` / `soff`) and the attention tile's padded panel
//! [rows_pad x heads x hsp], one thread an element of the padded space: PAD writes the panel at `doff` with zero lanes and zero rows
//! past `rows` (f16 saturated under OUT16, the BIAS stamps adding the bias row as they read); the other way reads the compact rows back
[ |> template_struct_instance]
class template GkRestride {
    @template_constant PAD : bool = true
    @template_constant OUT16 : bool = false
    @template_constant OUT32 : bool = true   //! the f32 destination's gate: the f16 stamp clears it as it sets OUT16
    @template_constant BIAS : bool = false   //! the pad adds the bias row at `boff` (the whisper-class q / k / v biases fold into the restride)
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" @template_gate = OUT32 dst : array<float>
    @ssbo @binding = 2 @role = "write" @template_gate = OUT16 dsth : array<float16>
    @ssbo @binding = 3 @role = "weight" @template_gate = BIAS wn : array<float>
    @kargs pa : GkRestrideArgs

    def restride_body {
        let gid = gl_GlobalInvocationID.x
        let per_row = pa.nheads * pa.hsp
        let rows_out = PAD ? pa.rows_pad : pa.rows
        if (gid < rows_out * per_row) {
            let row = gid / per_row
            let hp = gid % per_row
            let head = hp / pa.hsp
            let lane = hp % pa.hsp
            let ci = row * pa.sstride + pa.soff + head * pa.hs + lane
            static_if (PAD) {
                var v = 0.0
                if (lane < pa.hs && row < pa.rows) {
                    v = src[ci]
                    static_if (BIAS) {
                        v += wn[pa.boff + head * pa.hs + lane]
                    }
                }
                static_if (OUT16) {
                    dsth[pa.doff + gid] = half_sat(v)
                } else {
                    dst[pa.doff + gid] = v
                }
            } else {
                if (lane < pa.hs) {
                    static_if (OUT16) {
                        dsth[ci] = half_sat(src[gid])
                    } else {
                        dst[ci] = src[gid]
                    }
                }
            }
        }
    }
}


struct GkMelNormArgs {
    n_mel : uint
    n_frames : uint   //! the frames the mel holds
    valid : uint      //! the real frames: the statistics run over them, the frames past them read zero
    mfloor : float    //! the LOG_IN stamp's log guard
}

//! canary's per-feature normalization of a frame-major mel in place, one thread a mel column: the column's mean and unbiased
//! deviation over the valid frames, every valid frame normalized, the tail frames zero; the LOG_IN stamp takes the log of
//! the mel sums first (Metal's mel GEMM leaves the sums, Vulkan's mel kernel the logs)
[ |> template_struct_instance]
class template GkMelNorm {
    @template_constant LOG_IN : bool = false
    @ssbo @binding = 0 @role = "readwrite" mel : array<float>
    @kargs pa : GkMelNormArgs

    def mel_norm_body {
        let j = gl_GlobalInvocationID.x
        if (j < pa.n_mel) {
            let nm = pa.n_mel
            var sum = 0.0
            for (i in urange(pa.valid)) {
                static_if (LOG_IN) {
                    let v = log(mel[i * nm + j] + pa.mfloor)
                    mel[i * nm + j] = v
                    sum += v
                } else {
                    sum += mel[i * nm + j]
                }
            }
            let mean = sum / float(max(1u, pa.valid))
            var sq = 0.0
            for (i in urange(pa.valid)) {
                let dv = mel[i * nm + j] - mean
                sq += dv * dv
            }
            let denom = sqrt(sq / max(float(pa.valid) - 1.0, 1.0)) + 1e-5
            for (i in urange(pa.valid)) {
                mel[i * nm + j] = (mel[i * nm + j] - mean) / denom
            }
            for (i in urange(pa.valid, pa.n_frames)) {
                mel[i * nm + j] = 0.0
            }
        }
    }
}

struct GkQ3aShuffleArgs {
    nch : uint          //! the conv channel count
    ncp : uint          //! a conv3 row's stride (the channels, or Metal's 64-padded ones)
    chunk_stride : uint //! a chunk's elements in y3 (16 x 13 rows of ncp, or Metal's row-padded chunk)
    npos : uint         //! the window's positions
}

//! the qwen3a feature rows off the third conv stage, the conv_out GEMM's feed: feat[ci][ow][h + 16 c] = y3[ci][(h x 13 + ow) x ncp + c],
//! thirteen positions a chunk; the stamp names the feed's element type `OT` (the f16 tile's halves, the f32 tile's floats)
[ |> template_struct_instance]
class template GkQ3aShuffle {
    @ssbo @binding = 0 @role = "read" y3 : array<float>
    @ssbo @binding = 1 @role = "write" feat : array<OT>
    @kargs pa : GkQ3aShuffleArgs

    def q3a_shuffle_body {
        let gid = gl_GlobalInvocationID.x
        let feat_w = max(1u, 16u * pa.nch)
        if (gid < pa.npos * feat_w) {
            let tok = 13u
            let row = gid / feat_w
            let j = gid % feat_w
            let h = j % 16u
            let c = j / 16u
            let ci = row / tok
            let ow = row % tok
            feat[gid] = OT(y3[ci * pa.chunk_stride + (h * tok + ow) * pa.ncp + c])   // nolint:PERF020,LINT019 — the f16 stamp narrows here; on the f32 stamp the cast is the identity
        }
    }
}

struct GkIm2col2dArgs {
    iw : uint         //! the source block's width (frames), height (mel rows / channels' rows) and channel count
    ih : uint
    cin : uint
    sstride : uint    //! a source pixel's stride: cin, or the padded channel rows a GEMM wrote
    kdp : uint        //! the column row's padded width, a tile multiple; the columns past cin x 9 read zero
    rows : uint       //! the output rows of every chunk: chunks x oh x ow
    soff : uint       //! the source's element base in its binding
}

//! the im2col of a k3 s2 p1 conv2d over a window's chunks, the conv GEMM's feed: row r = (chunk, oy, ox), column k = (ic x 3 + kh)
//! x 3 + kw - the file's weight order, so the weights need no permute; the source rows are [chunk][ih][iw][sstride]; the stamp
//! names the feed's element type `OT`
[ |> template_struct_instance]
class template GkIm2col2d {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" col : array<OT>
    @kargs pa : GkIm2col2dArgs

    def im2col2d_body {
        let gid = gl_GlobalInvocationID.x
        let kdp = max(1u, pa.kdp)
        if (gid < pa.rows * kdp) {
            let r = gid / kdp
            let k = gid % kdp
            var v = 0.0
            if (k < pa.cin * 9u) {
                let ow = (pa.iw - 1u) / 2u + 1u
                let oh = (pa.ih - 1u) / 2u + 1u
                let per_chunk = max(1u, oh * ow)
                let ci = r / per_chunk
                let rr = r % per_chunk
                let ic = k / 9u
                let kh = (k % 9u) / 3u
                let kw = k % 3u
                let iy = int((rr / ow) * 2u + kh) - 1
                let ix = int((rr % ow) * 2u + kw) - 1
                if (iy >= 0 && iy < int(pa.ih) && ix >= 0 && ix < int(pa.iw)) {
                    v = src[pa.soff + ci * (pa.ih * pa.iw * pa.sstride) + (uint(iy) * pa.iw + uint(ix)) * pa.sstride + ic]
                }
            }
            col[gid] = OT(v)   // nolint:PERF020,LINT019 — the f16 stamp narrows here; on the f32 stamp the cast is the identity
        }
    }
}

struct GkBiasActArgs {
    d : uint        //! the row width, a 4-multiple
    nelem : uint
    boff : uint     //! the bias row's base in wn
    act : uint
    xoff : uint     //! the rows' base in x (a Metal site inside a larger buffer; 0 elsewhere)
}

//! a GEMM output's bias row added in place, then the activation `pa.act` names (`act_f32`); the SCALE stamp multiplies the
//! rows by the row at `boff` instead (TtsPkRowScale's layer scale)
[ |> template_struct_instance]
class template GkBiasAct {
    @template_constant OUT16 : bool = false   //! the biased, activated rows stored as f16 into `outh` (the cm2 tile's feed), x read alone; else in place
    @template_constant BIAS : bool = true     //! the bias row added ahead of the activation; false = the activation alone (the Conformer's silu rows)
    @template_constant SCALE : bool = false   //! the row at `boff` multiplies the rows instead (a layer scale), no activation; BIAS stays true for the binding
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "weight" @template_gate = BIAS wn : array<float>
    @ssbo @binding = 2 @role = "write" @template_gate = OUT16 outh : array<float16>
    @kargs pa : GkBiasActArgs

    def store(e : uint; v : float) : void {
        static_if (OUT16) {
            outh[e] = half_sat(v)
        } else {
            x[pa.xoff + e] = v
        }
    }

    //! one element: the bias (or the scale row) at its column, then the activation
    def bias_act_at(e : uint) {
        let c = e % max(1u, pa.d)
        var v = x[pa.xoff + e]
        static_if (SCALE) {
            v *= wn[pa.boff + c]
        } else {
            static_if (BIAS) {
                v += wn[pa.boff + c]
            }
            v = act_f32(v, pa.act)
        }
        store(e, v)
    }

    def bias_act_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            bias_act_at(e)
            if (e + 1u < pa.nelem) {
                bias_act_at(e + 1u)
            }
            if (e + 2u < pa.nelem) {
                bias_act_at(e + 2u)
            }
            if (e + 3u < pa.nelem) {
                bias_act_at(e + 3u)
            }
        }
    }

}

//! the clamp record at the half range: the plain f32 -> f16 rows (`scale` on the way in, the halves at `hoff`)
def gk_f16_rows_args(nelem : uint; scale : float = 1.0; hoff : uint = 0u) : GkClampArgs
    => GkClampArgs(nelem = nelem, lo = -65504.0, hi = 65504.0, scale = scale, hoff = hoff)

struct GkClampArgs {
    nelem : uint          //! a 4-multiple (every tower width is a 64-multiple)
    lo : float
    hi : float
    scale : float         //! a multiplier ahead of the clamp (the softmax scale a K twin carries; 1 elsewhere)
    xoff : uint           //! the rows' base in x
    hoff : uint           //! the rows' base in the half plane or the copy
}

//! rows clamped to [lo, hi] - in place, into `dst` (COPY: the clamp ahead of an f32 GEMM on Metal), or as halves into `outh` with
//! the bounds cut to the half range (OUT16: the f16 feed of a clamped GEMM input, or at the half range's bounds the plain
//! f32 -> f16 rows); four elements an invocation
[ |> template_struct_instance]
class template GkClampRows {
    @template_constant OUT16 : bool = false
    @template_constant COPY : bool = false
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "write" @template_gate = OUT16 outh : array<float16>
    @ssbo @binding = 2 @role = "write" @template_gate = COPY dst : array<float>
    @kargs pa : GkClampArgs

    def store_clamped(e : uint) : void {
        let v = x[pa.xoff + e] * pa.scale
        static_if (OUT16) {
            outh[pa.hoff + e] = float16(min(max(v, max(pa.lo, -65504.0)), min(pa.hi, 65504.0)))   // nolint:STYLE019,LINT019 - the MSL subset has no clamp lowering; the rule fires where the stamp lands
        } else {
            static_if (COPY) {
                dst[pa.hoff + e] = min(max(v, pa.lo), pa.hi)   // nolint:STYLE019,LINT019 - the same
            } else {
                x[pa.xoff + e] = min(max(v, pa.lo), pa.hi)   // nolint:STYLE019,LINT019 - the same
            }
        }
    }

    def clamp_rows_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            store_clamped(e)
            if (e + 1u < pa.nelem) {   //! the tail past a count off the four lattice (a whole blob's halves) stops at the count
                store_clamped(e + 1u)
            }
            if (e + 2u < pa.nelem) {
                store_clamped(e + 2u)
            }
            if (e + 3u < pa.nelem) {
                store_clamped(e + 3u)
            }
        }
    }
}

struct GkGegluArgs {
    nelem : uint
    glo : float     //! the gate GEMM's output bounds
    ghi : float
    ulo : float     //! the up GEMM's
    uhi : float
    dlo : float     //! the down GEMM's input bounds
    dhi : float
}

//! GEGLU-quick with the gemma4v bounds: g = clamp(gelu_quick(clamp(g)) * clamp(u)) in place over the gate rows, the activation in
//! the CPU table's f16-rounded form (`gelu_quick_lut_f32`); four elements an invocation
[ |> template_struct_instance]
class template GkGeglu {
    @ssbo @binding = 0 @role = "readwrite" g : array<float>
    @ssbo @binding = 1 @role = "read" u : array<float>
    @kargs pa : GkGegluArgs

    def geglu_at(e : uint) : void {
        let gv = min(max(g[e], pa.glo), pa.ghi)   // nolint:STYLE019,LINT019 - the MSL subset has no clamp lowering
        let uv = min(max(u[e], pa.ulo), pa.uhi)   // nolint:STYLE019,LINT019 - the same
        g[e] = min(max(gelu_quick_lut_f32(gv) * uv, pa.dlo), pa.dhi)   // nolint:STYLE019,LINT019 - the same
    }

    def geglu_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            geglu_at(e)
            if (e + 1u < pa.nelem) {
                geglu_at(e + 1u)
            }
            if (e + 2u < pa.nelem) {
                geglu_at(e + 2u)
            }
            if (e + 3u < pa.nelem) {
                geglu_at(e + 3u)
            }
        }
    }
}

let WDEC_KV_F16 = 0u   //! `GkWdecKvArgs.mode`: the resident f16 store, [heads][tstride][hs] at `dst_off`
let WDEC_KV_KX = 1u    //! ... the CPU chain's pre-scaled f32 keys, [heads][hs][tstride]
let WDEC_KV_VX = 2u    //! ... the CPU chain's f32 values, [heads][tstride][hs]

struct GkWdecKvArgs {
    rows : uint
    sstride : uint   //! a source row's width (d, or 3 d off the fused [q | k | v] row)
    soff : uint      //! the source column the head 0 starts at
    nheads : uint
    tstride : uint   //! the destination's positions a head
    p0 : uint        //! the destination position of source row 0
    dst_off : uint   //! the destination's element base (the layer's plane)
    boff : uint      //! the bias row's base in wn; read under bias_on
    bias_on : uint
    mode : uint      //! a WDEC_KV_* code
    hs : uint        //! the head width
    kscale : float   //! the keys' pre-scale, hs^-0.25 computed on the host (the family's `wdec_kscale`), so both homes round it alike
}

//! the whisper decoder's K/V stores: a source row's head slices into the resident f16 plane (OUT16; the mode's literal 0) or the CPU
//! chain's f32 layouts - the keys transposed and pre-scaled hs^-0.25 (mode 1), the values (mode 2) - the bias row folded in under
//! `bias_on`; one element an invocation
[ |> template_struct_instance]
class template GkWdecKv {
    @template_constant OUT16 : bool = false
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" @template_gate = OUT16 dsth : array<float16>
    @ssbo @binding = 2 @role = "write" @template_gate = "!OUT16" dstf : array<float>
    @ssbo @binding = 3 @role = "weight" wn : array<float>
    @kargs pa : GkWdecKvArgs

    def wdec_kv_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.rows * pa.nheads * pa.hs) {
            let nh = max(1u, pa.nheads)
            let hs = max(1u, pa.hs)
            let j = gid % hs
            let hr = gid / hs
            let h = hr % nh
            let r = hr / nh
            var v = src[r * pa.sstride + pa.soff + h * hs + j]
            if (pa.bias_on != 0u) {
                v += wn[pa.boff + h * hs + j]
            }
            let t = pa.p0 + r
            static_if (OUT16) {
                dsth[pa.dst_off + (h * pa.tstride + t) * hs + j] = half_sat(v)
            } else {
                if (pa.mode == 1u) {
                    dstf[pa.dst_off + (h * hs + j) * pa.tstride + t] = v * pa.kscale
                } else {
                    dstf[pa.dst_off + (h * pa.tstride + t) * hs + j] = v
                }
            }
        }
    }
}

struct GkIm2col1dArgs {
    cin : uint          //! the input channels
    tin : uint          //! the input positions
    tout : uint         //! the output positions, the live column rows
    k : uint            //! the taps
    stride : uint
    pad_l : uint        //! the forward conv's left pad; the transposed conv's cropped left edge
    dilation : uint
    transposed : uint   //! 1 = the transposed conv's source positions
    kdim : uint         //! the column row's width: k * cin on the GEMM's 32 lattice, the columns past k * cin zero
    xoff : uint         //! x's element base
    coloff : uint       //! col's element base
    row_major : uint    //! 1 = x is row-major [tin][cin] (the tower's rows form), 0 = channel-major [cin][tin]
    r0 : uint           //! the chunk's first output row: row r of the panel is output position r0 + r
    mp : uint           //! the panel's rows: tout, or tout on the GEMM's row lattice where the tile reads the pad rows (zero)
}

//! the 1-D conv's im2col, the GEMM's feed: col [mp][kdim] at coloff, column j = tap * cin + ci (the slab's weight-row order),
//! zero where the tap falls off x, past k * cin and past tout; a forward tap reads x at (r0 + t) * stride + tap * dilation - pad_l,
//! a transposed one the position whose stride lands the tap on r0 + t; one element an invocation, four as f16 under HALF
[ |> template_struct_instance]
class template GkIm2col1d {
    @template_constant HALF : bool = false
    @template_constant FULL : bool = true
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" @template_gate = FULL col : array<float>
    @ssbo @binding = 2 @role = "write" @template_gate = HALF col16 : array<float16>
    @kargs pa : GkIm2col1dArgs

    //! the input position tap `tap` reads for output row r, or -1 where it falls off x
    def src_pos(r, tap : uint) : int => (pa.transposed != 0u
        ? conv_src_tr_row(r, tap, pa.r0, pa.pad_l, pa.dilation, max(pa.stride, 1u), pa.tin)
        : conv_src_fwd_row(r, tap, pa.r0, pa.pad_l, pa.dilation, pa.stride, pa.tin))

    def put16(e : uint; v : float) {
        static_if (HALF) {
            col16[pa.coloff + e] = half_sat(v)
        }
    }

    //! column j of output row r: the source element the tap reads, zero off x, past k * cin and past tout
    def elem_into(r, j : uint; var v : float&) {
        v = 0.0
        if (r < pa.tout && j < pa.k * pa.cin) {
            let cin = max(pa.cin, 1u)
            let tap = j / cin
            let st = src_pos(r, tap)
            if (st >= 0) {
                let ci = j % cin
                v = pa.row_major != 0u ? x[pa.xoff + uint(st) * pa.cin + ci] : x[pa.xoff + ci * pa.tin + uint(st)]
            }
        }
    }

    def im2col1d_body {
        let kdim = max(pa.kdim, 1u)
        let cin = max(pa.cin, 1u)
        static_if (HALF) {
            // four columns an invocation: on a cin four-lattice they share one tap and source position, one 16-byte read
            let e = gl_GlobalInvocationID.x * 4u
            if (e < pa.mp * kdim) {
                let r = e / kdim
                let j0 = e % kdim
                if (pa.cin % 4u == 0u) {
                    var v0 = 0.0
                    var v1 = 0.0
                    var v2 = 0.0
                    var v3 = 0.0
                    if (r < pa.tout && j0 < pa.k * pa.cin) {
                        let tap0 = j0 / cin
                        let st = src_pos(r, tap0)
                        if (st >= 0) {
                            let ci0 = j0 % cin
                            if (pa.row_major != 0u) {
                                let b = pa.xoff + uint(st) * pa.cin + ci0
                                v0 = x[b]
                                v1 = x[b + 1u]
                                v2 = x[b + 2u]
                                v3 = x[b + 3u]
                            } else {
                                let b = pa.xoff + ci0 * pa.tin + uint(st)
                                v0 = x[b]
                                v1 = x[b + pa.tin]
                                v2 = x[b + 2u * pa.tin]
                                v3 = x[b + 3u * pa.tin]
                            }
                        }
                    }
                    put16(e, v0)
                    put16(e + 1u, v1)
                    put16(e + 2u, v2)
                    put16(e + 3u, v3)
                } else {
                    for [unroll_full] (q in range(4)) {
                        var v = 0.0
                        elem_into(r, j0 + uint(q), v)
                        put16(e + uint(q), v)
                    }
                }
            }
        } else {
            let gid = gl_GlobalInvocationID.x
            if (gid < pa.mp * kdim) {
                var v = 0.0
                elem_into(gid / kdim, gid % kdim, v)
                col[pa.coloff + gid] = v
            }
        }
    }
}

struct GkRowGatherArgs {
    c : uint         //! the row width
    rows : uint      //! the output rows
    aoff : uint      //! the gathered rows' element base in a
    boff : uint      //! the [rows x c] add plane's element base in b, read under add_b
    coff : uint      //! the [c] add row's element base in cvec, read under add_c
    out_off : uint   //! the output rows' element base
    add_b : uint
    add_c : uint
    scale : float    //! the gathered row's scale (the greedy chain's embed scale; 1 elsewhere)
}

//! out[r][j] = scale * a[idx[r]][j] (+ b[r][j] under add_b) (+ cvec[j] under add_c), added in that order: a stem's window-sort
//! reorder and the nearest-x2 upsample with the adds off, the PL-BERT embedding sum (the CPU `styletts2_albert`) with the position
//! and type rows on, the greedy chain's one-row gather from the fp32 token table under the embed scale; one element an invocation
[ |> template_struct_instance]
class template GkRowGather {
    @ssbo @binding = 0 @role = "read" a : array<float>
    @ssbo @binding = 1 @role = "read" idx : array<uint>
    @ssbo @binding = 2 @role = "weight" b : array<float>
    @ssbo @binding = 3 @role = "weight" cvec : array<float>
    @ssbo @binding = 4 @role = "write" out : array<float>
    @kargs pa : GkRowGatherArgs

    def row_gather_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.rows * pa.c) {
            let c = max(pa.c, 1u)
            let r = gid / c
            let ci = gid % c
            var v = a[pa.aoff + idx[r] * pa.c + ci] * pa.scale
            if (pa.add_b != 0u) {
                v += b[pa.boff + gid]
            }
            if (pa.add_c != 0u) {
                v += cvec[pa.coff + ci]
            }
            out[pa.out_off + gid] = v
        }
    }
}

struct GkPosAdd2DArgs {
    dim : uint
    ncols : uint         //! the patch grid's width in rows
    pos_entries : uint   //! the x rows of the table; the y rows follow them
    total : uint
    tboff : uint         //! the table's element base in tbl
}

//! the gemma4uv 2-axis position add: row p gains its x-table row (p % ncols) and its y-table row (pos_entries + p / ncols), one
//! element an invocation, the CPU loop's (row + ex) + ey order
[ |> template_struct_instance]
class template GkPosAdd2D {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "weight" tbl : array<float>
    @kargs pa : GkPosAdd2DArgs

    def pos_add_2d_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let p = gid / pa.dim
            let i = gid % pa.dim
            let ex = tbl[pa.tboff + (p % pa.ncols) * pa.dim + i]
            let ey = tbl[pa.tboff + (pa.pos_entries + p / pa.ncols) * pa.dim + i]
            x[gid] = (x[gid] + ex) + ey
        }
    }
}

struct GkStemAsmArgs {
    dim : uint
    pw : uint      //! the patch grid's width
    total : uint
    boff : uint    //! the conv bias row's element base in b
    poff : uint    //! the position rows' element base in pos
}

//! the qwen3v stem assemble: residual row r of the merge-adjacent walk (2x2 partners adjacent, groups row-major) gathers its
//! natural-order conv row plus the conv bias plus that row's position, one element an invocation, the CPU stem's (crow + bp) + prow order
[ |> template_struct_instance]
class template GkStemAsm {
    @ssbo @binding = 0 @role = "read" xn : array<float>
    @ssbo @binding = 1 @role = "weight" b : array<float>
    @ssbo @binding = 2 @role = "read" pos : array<float>
    @ssbo @binding = 3 @role = "write" x : array<float>
    @kargs pa : GkStemAsmArgs

    def stem_asm_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let r = gid / pa.dim
            let i = gid % pa.dim
            let g = r / 4u
            let sub = r % 4u
            let gcols = max(1u, pa.pw / 2u)
            let src = ((g / gcols) * 2u + sub / 2u) * pa.pw + (g % gcols) * 2u + sub % 2u
            x[gid] = (xn[src * pa.dim + i] + b[pa.boff + i]) + pos[pa.poff + src * pa.dim + i]
        }
    }
}

struct GkPool2dArgs {
    width : uint   //! a row's width
    gw : uint      //! the source grid's width, in rows
    k : uint       //! the pool's side
    ow : uint      //! the pooled grid's width
    total : uint   //! output elements
}

//! a vision tail's mean pool: y [oh*ow x width] the mean of each k x k block of the row grid x [gh*gw x width], one element an invocation
[ |> template_struct_instance]
class template GkPool2d {
    @ssbo @binding = 0 @role = "read" x : array<float>
    @ssbo @binding = 1 @role = "write" y : array<float>
    @kargs pa : GkPool2dArgs

    def pool_2d_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let o = gid / pa.width
            let c = gid % pa.width
            let oy = o / max(1u, pa.ow)
            let ox = o % max(1u, pa.ow)
            var sum = 0.0
            var dy = 0u
            while (dy < pa.k) {
                var dx = 0u
                while (dx < pa.k) {
                    sum += x[((oy * pa.k + dy) * pa.gw + ox * pa.k + dx) * pa.width + c]
                    dx++
                }
                dy++
            }
            y[gid] = sum / float(pa.k * pa.k)
        }
    }
}

struct GkAffineRowsArgs {
    width : uint   //! a row's width
    total : uint
    scale : float
}

//! a vision tail's row standardize, in place: x = (x * scale - b[c]) * m[c] over rows of `width`, one element an invocation
[ |> template_struct_instance]
class template GkAffineRows {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" b : array<float>
    @ssbo @binding = 2 @role = "read" m : array<float>
    @kargs pa : GkAffineRowsArgs

    def affine_rows_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let c = gid % pa.width
            x[gid] = (x[gid] * pa.scale - b[c]) * m[c]
        }
    }
}

struct GkPostAddLnArgs {
    d : uint
    rows : uint
    woff : uint     //! the branch's post-norm weight row's base in wn, its output bias row at boff
    boff : uint
    eps : float
    ln_on : uint
    lnoff : uint    //! the layernorm's weight row's base in wn, its bias row at lnboff
    lnboff : uint
    ascale : float  //! the post-add's residual weight (canary's macaron halves add 0.5 of the branch)
    xboff : uint    //! the normed rows' element base in xb / xbh (a Metal site inside a larger buffer; 0 elsewhere)
}

//! the layernorm alone (the ADD-off stamps): `rows` of width `d`, the weight row at `lnoff` and the bias row at `lnboff` in the norms plane
def gk_ln_args(d, rows : uint; eps : float; lnoff, lnboff : uint; xboff : uint = 0u) : GkPostAddLnArgs
    => GkPostAddLnArgs(d = d, rows = rows, woff = 0u, boff = 0u, eps = eps, ln_on = 1u, lnoff = lnoff, lnboff = lnboff, ascale = 0.0, xboff = xboff)

//! the residual seam of a block, one workgroup a row: x[r] += ascale . branch (y + bias under BIAS, rmsnorm(y) . wpost under POST_NORM, y
//! as it is), then under `ln_on` the next pre-norm - layernorm or rms (RMS) of x[r] against the rows at lnoff / lnboff - into xb, xbh (OUT16) or
//! Q8_0 blocks (OUT_Q8); ADD off is the norm alone (always on); xb may alias x - a lane re-reads only its own elements after both reduces
[ |> template_struct_instance]
class template GkPostAddLn : GkQ8BlockStore {
    @template_constant ADD : bool = true   //! the seam's add before the norm
    @template_constant RMS : bool = false   //! the next pre-norm is the weighted rms (qwen25v, the gemma towers), not the layernorm
    @template_constant POST_NORM : bool = false  //! the branch rows carry their post-norm before the add
    @template_constant BIAS : bool = true   //! the branch rows carry their output bias before the add
    @template_constant OUT16 : bool = false   //! the normed rows as f16 (the cm2 tile's feed), else f32
    @template_constant OUT32 : bool = true
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "read" @template_gate = ADD y : array<float>
    @ssbo @binding = 2 @role = "write" @template_gate = OUT32 xb : array<float>
    @ssbo @binding = 3 @role = "weight" wn : array<float>
    @ssbo @binding = 4 @role = "write" @template_gate = OUT16 xbh : array<float16>
    @ssbo @binding = 6 @role = "write" @template_gate = OUT_Q8 outq : array<int8>
    @ssbo @binding = 7 @role = "write" @template_gate = OUT_Q8 outs : array<float>
    @kargs pa : GkPostAddLnArgs

    def store(i : uint; v : float) : void {
        static_if (OUT16) {
            xbh[pa.xboff + i] = half_sat(v)
        } else {
            static_if (OUT32) {
                xb[pa.xboff + i] = v
            }
        }
    }

    //! the branch's element `i` of the row at `base` as the add takes it
    def branch(base, i : uint; inv_post : float) : float {
        static_if (!ADD) {
            return 0.0
        } else {
            static_if (POST_NORM) {
                return (y[base + i] * inv_post) * wn[pa.woff + i]
            } else {
                static_if (BIAS) {
                    return y[base + i] + wn[pa.boff + i]
                } else {
                    return y[base + i]
                }
            }
        }
    }

    //! the branch's post-norm inverse over the row at `base` (slot 0), 1 on a stamp without the post-norm
    def post_inv_into(base : uint; var inv : float&) {
        static_if (POST_NORM) {
            var ss = 0.0
            var i = gl_LocalInvocationID.x
            while (i < pa.d) {
                let v = y[base + i]
                ss += v * v
                i += 256u
            }
            var sw = 0.0
            wg_sum_into(ss, 0u, sw)
            inv = 1.0 / sqrt(sw / float(pa.d) + pa.eps)
        } else {
            inv = 1.0
        }
    }

    def post_add_ln_body {
        let base = gl_WorkGroupID.x * pa.d
        let tid = gl_LocalInvocationID.x
        var inv_post = 1.0
        post_inv_into(base, inv_post)
        var s1 = 0.0
        var i = tid
        static_if (ADD) {
            while (i < pa.d) {
                let xv = x[base + i] + pa.ascale * branch(base, i, inv_post)
                x[base + i] = xv
                static_if (RMS) {
                    s1 += xv * xv
                } else {
                    s1 += xv
                }
                i += 256u
            }
        } else {
            while (i < pa.d) {
                s1 += x[base + i]
                i += 256u
            }
        }
        let ln_on = ADD ? pa.ln_on != 0u : true
        if (ln_on) {
            static_if (RMS) {
                var sw = 0.0
                wg_sum_into(s1, POST_NORM ? 1u : 0u, sw)
                let inv = 1.0 / sqrt(sw / float(pa.d) + pa.eps)
                i = tid
                while (i < pa.d) {
                    store(base + i, (x[base + i] * inv) * wn[pa.lnoff + i])
                    i += 256u
                }
            } else {
                var s1w = 0.0
                wg_sum_into(s1, 0u, s1w)
                let mean = s1w / float(pa.d)
                var s2 = 0.0
                i = tid
                while (i < pa.d) {
                    let dv = x[base + i] - mean
                    s2 += dv * dv
                    i += 256u
                }
                var s2w = 0.0
                wg_sum_into(s2, 1u, s2w)
                let inv = 1.0 / sqrt(s2w / float(pa.d) + pa.eps)
                static_if (OUT_Q8) {
                    static_if (ADD) {
                        memoryBarrierBuffer()   // every lane's in-place x writes visible before the block pass reads the row across lanes
                        barrier()
                    }
                    blk_pass(base, pa.d / KQ_B32_ELEMS_U, gl_WorkGroupID.x * (pa.d / KQ_B32_ELEMS_U), mean, inv)
                } else {
                    i = tid
                    while (i < pa.d) {
                        store(base + i, mad((x[base + i] - mean) * inv, wn[pa.lnoff + i], wn[pa.lnboff + i]))
                        i += 256u
                    }
                }
            }
        }
    }

    def override normed_at(e : uint; mean, inv : float) : float => mad((x[e] - mean) * inv, wn[pa.lnoff + e % max(1u, pa.d)], wn[pa.lnboff + e % max(1u, pa.d)])
}
