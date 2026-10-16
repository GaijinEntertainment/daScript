options gen2
//! The kernels both GPU homes run, written once: a `class template` here carries a kernel's buffers, its
//! argument struct and its body, and each home stamps it under its own dispatch name.
module dasllama_gpu_kernels_common shared public
require dasllama/dasllama_lint public
require daslib/typemacro_boost
require math
require spirv/spirv_shader   // [spirv_inline]: the helpers the SPIR-V emitter visits at their call sites

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
    @template_constant RSQRT : bool = false   //!< `wg_rms_inv_into`'s inverse norm: `rsqrt`, else the CPU's `1 / sqrt`

    //! the inverse rms of the squares summed into `ss0` over `n` elements, every thread alike, its spelling the stamp's (`RSQRT`)
    [arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#gpu-kernel-pairs")]
    def wg_rms_inv_into(ss0 : float; n : uint; eps : float; slot : uint; var inv : float&) {
        var tot = 0.0
        wg_sum_into(ss0, slot, tot)
        inv = rms_inv(tot / float(n) + eps, RSQRT)
    }

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

let GK_PK_GEMV_MAX_NIN = 4096u   //! the x the row GEMV stages, the body's literal: a wider input declines at the driver

struct GkPkGemvArgs {
    nin : uint
    nout : uint
    stride : uint   //! the weight rows' stride
    eps : float     //! the input norm's epsilon, read under NORM_IN or NORM_MOD_IN
    woff : uint     //! the weight rows [nout][stride] in wn
    boff : uint     //! the bias [nout] in wn
    xoff : uint     //! the input row in x
    yoff : uint     //! the output row in y
    moff : uint     //! the modulation rows in mod: shift [nin], scale [nin], gate [nout]
    lwoff : uint    //! the input norm's scale row in wn
    lboff : uint    //! the input norm's shift row in wn
    aoff : uint     //! the slab vector in wn: the add-SiLU's [nout], the tail's scale [nout] then shift [nout], the residual's scale [nout]
    x0off : uint    //! the tail's noise row in x0
    loff : uint     //! the tail's latent row in lat
    span : uint     //! the rope-store epilogue: the q, k and v spans' width (the backbone's d), the caches' row stride
    dh : uint       //! ... the head size
    p : uint        //! ... the position: the tables' row and the caches' row
    coff : uint     //! ... the cos table [pos][dh / 2] in tabs
    soff : uint     //! ... the sin table, the same layout
}

//! The Pocket frame's row GEMV: y [nout] = W . x [nin] + b at one row, x staged in the workgroup - as it is, under NORM_IN through a
//! LayerNorm (lw, lb), under NORM_MOD_IN through it and the adaLN modulation; EPI the epilogue (`ARCHITECTURE_GPU_SHARED_KERNELS.md`);
//! 32 lanes a row folded by shuffle, four rows a workgroup; a stamp supplies the row's dot
[ |> template_struct_instance]
class template GkPkGemv : GkWgReduce {
    @template_constant NORM_IN : bool = false       //!< the input through the LayerNorm (lw, lb) alone
    @template_constant NORM_MOD_IN : bool = false
    @template_constant MOD_IN : bool = false   //!< the modulation rows are read: the normed input or the gate epilogue
    @template_constant AUX_IN : bool = false   //!< the slab vector is read: the add-SiLU, the tail or the residual's scale
    @template_constant TAIL : bool = false     //!< the frame tail: the noise row in, the latent row out
    @template_constant ROPE_STORE : bool = false   //!< the qkv epilogue: the tables and the two caches bound
    @template_constant EPI : int = 0           //!< the epilogue, spelled as its literal: a template constant folds only a literal
    @template_constant QUAD : bool = true      //!< the f32 rows read as float4 on the four lattice (an alias of the rows' binding)
    @ssbo @binding = 0 @role = "weight" wn : array<float>
    @ssbo @binding = 0 @role = "alias" @template_gate = QUAD wn4 : array<float4>   //! the same rows as float4: the quad dot's loads
    @ssbo @binding = 1 @role = "read" x : array<float>
    @ssbo @binding = 2 @role = "readwrite" y : array<float>
    @ssbo @binding = 3 @role = "read" @template_gate = MOD_IN mod : array<float>
    @ssbo @binding = 4 @role = "read" @template_gate = TAIL x0 : array<float>
    @ssbo @binding = 5 @role = "write" @template_gate = TAIL lat : array<float>
    @ssbo @binding = 6 @role = "read" @template_gate = ROPE_STORE tabs : array<float>
    @ssbo @binding = 7 @role = "write" @template_gate = ROPE_STORE kc : array<float>
    @ssbo @binding = 8 @role = "write" @template_gate = ROPE_STORE vc : array<float>
    @kargs pa : GkPkGemvArgs
    @workgroup xs : float[int(GK_PK_GEMV_MAX_NIN)]
    @workgroup @template_gate = ROPE_STORE dq : float[4]   //! the rope-store stamp: the workgroup's four dots side by side, so an even row turns its pair

    def abstract row_dot_into(row, lane : uint; var acc : float&) : void   //! row `row`'s dot with the staged x over its 32 lanes

    //! the 32 lanes of a row folded: a butterfly over the row's aligned lane group, every lane holding the row's sum
    [spirv_inline]
    def row_sum_into(var s : float&) {
        var a = s
        a += gk_subgroup_xor(a, 16u)
        a += gk_subgroup_xor(a, 8u)
        a += gk_subgroup_xor(a, 4u)
        a += gk_subgroup_xor(a, 2u)
        a += gk_subgroup_xor(a, 1u)
        s = a
    }

    //! whether the rows' base, stride and width sit on the four lattice, under `QUAD`
    [spirv_inline]
    def quad_rows : bool {
        static_if (QUAD) {
            return ((pa.woff | pa.stride | pa.nin) & 3u) == 0u
        } else {
            return false
        }
    }

    //! the f32 rows' dot over lanes: under `QUAD` float4 steps on the four lattice, floats otherwise
    [spirv_inline]
    def f32_row_dot_into(row, lane : uint; var acc : float&) {
        let wrow = pa.woff + row * pa.stride
        var a = acc
        static_if (QUAD) {
            if (quad_rows()) {
                let w4 = wrow >> 2u
                let n4 = pa.nin >> 2u
                var j = lane
                while (j < n4) {
                    let w = wn4[w4 + j]
                    let xb = j * 4u
                    a += w.x * xs[xb]
                    a += w.y * xs[xb + 1u]
                    a += w.z * xs[xb + 2u]
                    a += w.w * xs[xb + 3u]
                    j += 32u
                }
            } else {
                scalar_dot_add(wrow, lane, a)
            }
        } else {
            scalar_dot_add(wrow, lane, a)
        }
        acc = a
    }

    //! the float rows' dot over lanes onto `a`
    [spirv_inline]
    def scalar_dot_add(wrow, lane : uint; var a : float&) {
        var s = a
        var j = lane
        while (j < pa.nin) {
            s += wn[wrow + j] * xs[j]
            j += 32u
        }
        a = s
    }

    [spirv_inline]
    def aux_at(i : uint) : float {
        static_if (AUX_IN) {
            return wn[pa.aoff + i]
        } else {
            return 0.0
        }
    }

    //! the input row staged into xs through the LayerNorm (lw, lb), then under NORM_MOD_IN the adaLN modulation; the inverse norm the
    //! stamp's spelling (`RSQRT`)
    [spirv_inline]
    def stage_normed(tid : uint) {
        var k = tid
        var s = 0.0
        while (k < pa.nin) {
            s += x[pa.xoff + k]
            k += 128u
        }
        var total = 0.0
        wg_sum_into(s, 0u, total)
        let mean = total / float(pa.nin)
        var vs = 0.0
        k = tid
        while (k < pa.nin) {
            let dv = x[pa.xoff + k] - mean
            vs += dv * dv
            k += 128u
        }
        var inv = 0.0
        wg_rms_inv_into(vs, pa.nin, pa.eps, 1u, inv)
        k = tid
        while (k < pa.nin) {
            let nv = tts_ln_affine(x[pa.xoff + k], mean, inv, wn[pa.lwoff + k], wn[pa.lboff + k])
            static_if (NORM_MOD_IN) {
                xs[k] = tts_modulate(nv, mod[pa.moff + k], mod[pa.moff + pa.nin + k])
            } else {
                xs[k] = nv
            }
            k += 128u
        }
    }

    //! workgroup `wg`'s four rows, thread `tid` of its 128
    [spirv_inline]
    def pk_gemv_body(wg, tid : uint) {
        static_if (NORM_MOD_IN || NORM_IN) {
            stage_normed(tid)
        } else {
            var k = tid
            while (k < pa.nin) {
                xs[k] = x[pa.xoff + k]
                k += 128u
            }
        }
        barrier()
        let row = wg * 4u + tid / 32u
        let lane = tid % 32u
        let live = row < pa.nout
        var sum = 0.0
        if (live) {
            row_dot_into(row, lane, sum)
        }
        row_sum_into(sum)
        if (live && lane == 0u) {
            let d = sum + wn[pa.boff + row]
            static_if (EPI == 0) {
                y[pa.yoff + row] = d
            }
            static_if (EPI == 1) {
                y[pa.yoff + row] = silu_f32(d)
            }
            static_if (EPI == 2) {
                y[pa.yoff + row] = y[pa.yoff + row] + mod[pa.moff + 2u * pa.nin + row] * d
            }
            static_if (EPI == 3) {
                y[pa.yoff + row] = silu_f32(d + aux_at(row))
            }
            static_if (EPI == 4) {
                let tv = d + x0[pa.x0off + row]
                y[pa.yoff + row] = tv
                lat[pa.loff + row] = tv * aux_at(row) + aux_at(pa.nout + row)
            }
            static_if (EPI == 5) {
                dq[tid / 32u] = d
            }
            static_if (EPI == 6) {
                y[pa.yoff + row] = y[pa.yoff + row] + d * aux_at(row)
            }
            static_if (EPI == 7) {
                y[pa.yoff + row] = y[pa.yoff + row] + d
            }
            static_if (EPI == 8) {
                y[pa.yoff + row] = tts_gelu_tanh(d)
            }
        }
        static_if (EPI == 5) {
            barrier()
            if (live && lane == 0u && (row & 1u) == 0u) {
                rope_store(row, dq[tid / 32u], dq[tid / 32u + 1u])
            }
        }
    }

    //! the rope-store epilogue over the pair (row, row + 1): the q span turned into y, the k span turned into the k cache's
    //! row p, the v span copied into the v cache's
    [spirv_inline]
    def rope_store(row : uint; r0, r1 : float) {
        static_if (ROPE_STORE) {
            let span = max(pa.span, 1u)
            let dh = max(pa.dh, 2u)
            let sp = row / span
            let c = row % span
            if (sp < 2u) {
                let hh = dh / 2u
                let tb = pa.p * hh + (c % dh) / 2u
                let cv = tabs[pa.coff + tb]
                let sv = tabs[pa.soff + tb]
                let o0 = r0 * cv - r1 * sv
                let o1 = r0 * sv + r1 * cv
                if (sp == 0u) {
                    y[pa.yoff + c] = o0
                    y[pa.yoff + c + 1u] = o1
                } else {
                    kc[pa.p * pa.span + c] = o0
                    kc[pa.p * pa.span + c + 1u] = o1
                }
            } else {
                vc[pa.p * pa.span + c] = r0
                vc[pa.p * pa.span + c + 1u] = r1
            }
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

struct GkPatchIm2colArgs {
    w : uint          //! the canvas in pixels
    h : uint
    p : uint          //! the patch side
    row_dim : uint    //! a column row's padded width; the columns past 3 x p x p read zero
    npos : uint       //! the patch rows; the rows past them up to `total` read zero (the GEMM's row pad)
    total : uint      //! the rows written x row_dim
    scale : float     //! the pixel map ahead of the conv: x . scale + shift (gemma4v's [0, 1] -> [-1, 1])
    shift : float
}

//! the vision stems' patch im2col over interleaved RGB planes [h][w][3]: row pi = patch (pi / n_cols, pi % n_cols) in row-major
//! order, column c x p x p + ky x p + kx = channel c's pixel (ky, kx) of the patch, the CPU `im2col_rgb_patches`' layout; one
//! element an invocation, the pad columns and the pad rows written zero
[ |> template_struct_instance]
class template GkPatchIm2col {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>
    @kargs pa : GkPatchIm2colArgs

    def patch_im2col_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let pi = gid / pa.row_dim
            let j = gid % pa.row_dim
            let psq = pa.p * pa.p
            var v = 0.0
            if (pi < pa.npos && j < 3u * psq) {
                let c = j / psq
                let k = j % psq
                let n_cols = max(1u, pa.w / pa.p)
                let py = (pi / n_cols) * pa.p + k / pa.p
                let px = (pi % n_cols) * pa.p + k % pa.p
                v = src[(py * pa.w + px) * 3u + c] * pa.scale + pa.shift
            }
            dst[gid] = v
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

struct GkRopeKvArgs {
    qd : uint          //! the q row's width; the k projection sits at `qd` of a ksrc row, the v projection at `qd + kvd` of a vsrc row
    kvd : uint         //! the k and v rows' width (the mirror row's)
    hs : uint
    npairs_q : uint    //! the q pairs of a row; the k pairs follow them
    npairs : uint      //! the pairs a row, q's and k's
    neox : uint        //! the pair select: 0 = NORM (2j, 2j+1), else NEOX (j, j+half)
    has_bias : uint
    rot : uint         //! the rotated width of a head; the pairs past it copy through
    qrow : uint        //! a row's stride in qsrc
    kvrow : uint       //! a row's stride in ksrc and vsrc
    boff : uint        //! the layer's [q | k | v] row base in bias (read where the stamp binds the planes whole)
    cstride : uint     //! a row's stride in tcos and tsin (0: every row reads the one table row)
    ccos : uint        //! the cos row's element base in tcos; the sin row sits a half head past it (read where the stamp binds the planes whole)
    rows : uint        //! the rows a dispatch covers (0 reads as one)
    mirbase : uint     //! the layer's base in the mirror planes, ahead of a row's own (the Vulkan token block's; Metal binds the row), or the
                       //! layer index where a row table carries the per-layer stride (Metal's window form)
}

//! the rope-and-store args a home builds: the one-row defaults (one row, every stride 0, the bias row, the cos row and the mirror
//! base bound at their own offsets), and the N-row form's strides and bases where a home passes them
def gk_rope_kv_args(qd, kvd, hs, npairs_q, npairs : int64; neox, has_bias : bool; rot : int64; boff : int64 = 0l; cbase : int64 = 0l;
                    rows : int64 = 1l; kvrow : int64 = 0l; qrow : int64 = 0l; cosrow : int64 = 0l; mirbase : int64 = 0l) : GkRopeKvArgs
    => GkRopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), npairs_q = uint(npairs_q), npairs = uint(npairs),
        neox = neox ? 1u : 0u, has_bias = has_bias ? 1u : 0u, rot = uint(rot),
        qrow = uint(qrow), kvrow = uint(kvrow), boff = uint(boff), cstride = uint(cosrow), ccos = uint(cbase),
        rows = uint(rows), mirbase = uint(mirbase))

//! The fused rope and K/V store, an invocation a rotation pair of a row: a q pair ropes in place, a k pair ropes into the mirror
//! K row and copies its v pair into the V row, the bias rows added first, the f16 stamps clamped into the half range, the pairs
//! past `rot` copied through; a stamp's kernel hands `rope_kv_pair` the row's mirror bases, resolved its home's way
[ |> template_struct_instance]
class template GkRopeKvStore {
    @ssbo @binding = 0 @role = "readwrite" qsrc : array<float>       //! the q rows, roped in place
    @ssbo @binding = 1 @role = "read" ksrc : array<float>            //! the rows the k projection sits in
    @ssbo @binding = 2 @role = "read" @off = "vsoff" vsrc : array<float>   //! ... and the v projection
    @ssbo @binding = 3 @role = "write" @off = "koff" kd : array<KT>  //! the mirror K plane
    @ssbo @binding = 4 @role = "write" @off = "voff" vd : array<KT>  //! the mirror V plane
    @ssbo @binding = 5 @role = "weight" bias : array<float>          //! the [q | k | v] projection biases (dummy-bound without `has_bias`)
    @ssbo @binding = 6 @role = "read" @off = "toff" tcos : array<float>   //! the rope table's cos rows (the Vulkan command refills its plane a step)
    @ssbo @binding = 7 @role = "read" @off = "toff" tsin : array<float>   //! ... and sin rows (one plane on Vulkan, a half head past the cos row)
    @kargs pa : GkRopeKvArgs
    @template_constant CLAMP16 : bool = true
    @template_constant BOUND_VIEWS : bool = false   //!< the stamp binds the v projection, the bias row and the table rows at their offsets, so their element bases are zero
    @template_constant SPLIT_TABLES : bool = false  //!< the sin rows are their own plane (bound at `tsin`), not a half head past the cos rows

    //! one mirror K element, clamped into the half range on the f16 stamp
    def st_k(i : uint; v : float) {
        static_if (CLAMP16) {
            kd[i] = float16(min(max(v, -65504.0), 65504.0))   // nolint:STYLE019,LINT019 - the MSL subset has no clamp lowering; the rule fires where the stamp lands
        } else {
            kd[i] = v
        }
    }

    //! ... and one V element
    def st_v(i : uint; v : float) {
        static_if (CLAMP16) {
            vd[i] = float16(min(max(v, -65504.0), 65504.0))   // nolint:STYLE019,LINT019 - the same
        } else {
            vd[i] = v
        }
    }

    //! an element base inside a plane bound whole, or zero where the stamp binds the view at its offset
    def base_in_plane(e : uint) : uint {
        static_if (BOUND_VIEWS) {
            return 0u
        } else {
            return e
        }
    }

    //! the sin row's element base: a half head past the cos row in the one plane, or zero in a plane of its own
    def sin_base(half : uint) : uint {
        static_if (BOUND_VIEWS || SPLIT_TABLES) {
            return 0u
        } else {
            return pa.ccos + half
        }
    }

    //! the one-row form a kernel over `npairs` invocations takes: pair `gid` of row 0, the mirror rows bound at their own offsets
    def rope_kv_one(gid : uint) {
        if (gid < pa.npairs) {
            rope_kv_pair(0u, gid, 0u, 0u)
        }
    }

    //! the row-table form a kernel over (`npairs` x rows) invocations takes: pair `gid` of row `row`, whose table entry `rrow` holds
    //! its mirror K and V bases, the per-layer stride `pa.mirbase` layers of which sit ahead, and its depth (the row lands at depth - 1)
    def rope_kv_tabled(gid, row : uint; rrow : uint4) {
        if (gid < pa.npairs) {
            let ahead = pa.mirbase * rrow.z + (rrow.w - 1u) * pa.kvd
            rope_kv_pair(row, gid, rrow.x + ahead, rrow.y + ahead)
        }
    }

    //! pair `lp` of row `row`: a q pair in place, a k pair into the mirror row at element `mirk` of kd and its v pair at `mirv` of vd
    def rope_kv_pair(row, lp, mirk, mirv : uint) {
        let isk = lp >= pa.npairs_q
        let lgid = isk ? lp - pa.npairs_q : lp
        let half = max(1u, pa.hs / 2u)
        let halfr = pa.rot / 2u
        let j = lgid % half
        let hbase = (lgid / half) * pa.hs
        let pair_rotates = j < halfr
        let i = pa.neox != 0u ? (pair_rotates ? hbase + j : hbase + pa.rot + (j - halfr)) : hbase + 2u * j
        let i2 = pa.neox != 0u ? (pair_rotates ? i + halfr : i + (half - halfr)) : i + 1u
        let cb = row * pa.cstride
        let fcr = pair_rotates ? tcos[base_in_plane(pa.ccos) + cb + j] : 1.0
        let fci = pair_rotates ? tsin[sin_base(half) + cb + j] : 0.0
        let boff = base_in_plane(pa.boff)
        if (isk) {
            let kb = row * pa.kvrow + pa.qd
            let bk = boff + pa.qd
            let v0 = ksrc[kb + i] + (pa.has_bias != 0u ? bias[bk + i] : 0.0)
            let v1 = ksrc[kb + i2] + (pa.has_bias != 0u ? bias[bk + i2] : 0.0)
            st_k(mirk + i, v0 * fcr - v1 * fci)
            st_k(mirk + i2, v0 * fci + v1 * fcr)
            let vb = row * pa.kvrow + base_in_plane(pa.qd + pa.kvd)
            let bv = boff + pa.qd + pa.kvd
            st_v(mirv + i, vsrc[vb + i] + (pa.has_bias != 0u ? bias[bv + i] : 0.0))
            st_v(mirv + i2, vsrc[vb + i2] + (pa.has_bias != 0u ? bias[bv + i2] : 0.0))
        } else {
            let qb = row * pa.qrow
            let v0 = qsrc[qb + i] + (pa.has_bias != 0u ? bias[boff + i] : 0.0)
            let v1 = qsrc[qb + i2] + (pa.has_bias != 0u ? bias[boff + i2] : 0.0)
            qsrc[qb + i] = v0 * fcr - v1 * fci
            qsrc[qb + i2] = v0 * fci + v1 * fcr
        }
    }
}

struct GkQknRopeArgs {
    qd : uint
    kvd : uint
    hs : uint
    half : uint        //! rotation pairs a head: hs / 2, or rot / 2 on a partial-rope model (the rest of the head passes through)
    neox : uint
    nh : uint          //! the q heads; the head workgroups past them are the k heads
    layerbase : uint   //! the layer's K/V mirror base (Vulkan), or the per-layer stride a row table's layer index steps (Metal's rows form)
    qwoff : uint       //! the q norm-weight row's base in wn
    kwoff : uint
    eps : float
    qstride : uint     //! a q head's stride in qrows: 0 = hs; 2 hs on a gated-q model ([q | gate] a head, roped in place)
    boff : uint        //! the layer's row base in the bias buffer ([q | k | v] rows, one layer after another)
    hasb : uint
    cbase : uint       //! the layer's rope class's row base in the cos plane
    vfk : uint         //! V is the raw k row scaled by its own rms (a layer with no v plane: gemma-4's global layers)
    vnorm : uint       //! V is its own projection scaled by its rms, no weight (gemma-4's v-norm)
    rowwg : uint       //! head workgroups a row (q heads + k heads); 0 = the whole dispatch is row 0
    kvrow : uint       //! a row's stride in the projection plane
    qrow : uint        //! a row's stride in qrows
    cosrow : uint      //! a row's stride in the cos plane
    win : uint         //! the window form: row r sits at position `pos0 + r` in the mirror at `layerbase`, its rows `row0 + r` of the planes, its k
                       //! row at `kbase` and its v row at `vbase` past the row's start in `kvsrc` (`kvrow` apart); 0 = the token rows' form
    pos0 : uint
    row0 : uint
    kbase : uint
    vbase : uint
    nreal : uint       //! the live rows: past them a q head copies its row into its twin and a k head stores nothing; 0 = every row
    skipnorm : uint    //! the rope alone, no per-head norm
    hscale : float     //! the q twin's scale (`HX` stamps)
}

//! The fused per-head q/k RMSNorm, rope and K/V store, a workgroup of `LANES` a head (`ARCHITECTURE_GPU_SHARED_KERNELS.md`):
//! q roped in place, k into the mirror, its workgroup storing the V row (plain, `vnorm`, or `vfk` the k row under k's rms);
//! a stamp supplies the row's mirror bases and the sin row's read
[ |> template_struct_instance]
class template GkQknRopeKv : GkWgReduce {
    @ssbo @binding = 0 @role = "readwrite" qrows : array<float>          //! the q rows, normed and roped in place
    @ssbo @binding = 1 @role = "read" kvsrc : array<float>               //! the projection rows: [q | k | v]
    @ssbo @binding = 2 @role = "write" @off = "koff" kmir : array<KT>    //! the mirror K plane
    @ssbo @binding = 3 @role = "write" @off = "voff" vmir : array<KT>    //! the mirror V plane
    @ssbo @binding = 4 @role = "read" @off = "toff" cossin : array<float>   //! the rope table's cos rows (and on Vulkan the sin rows a half past them)
    @ssbo @binding = 5 @role = "weight" wn : array<float>                //! the q and k norm-weight rows
    @ssbo @binding = 7 @role = "weight" bias : array<float>              //! the [q | k | v] projection biases (`hasb`), else a placeholder
    @ssbo @binding = 9 @role = "write" @template_gate = HX qh : array<float16>   //! the q rows' half twin, under `hscale`
    @kargs pa : GkQknRopeArgs
    @template_constant CLAMP16 : bool = true
    @template_constant LANES : uint = 256u
    @template_constant EPT : int = 1   //!< the rotation pairs a thread holds: LANES x EPT covers the widest head's, 256
    @template_constant V_WG : bool = false   //!< the V rows take workgroups of their own past the k heads; else a k head's stores its V row
    @template_constant HX : bool = false

    def abstract mir_base(rowi : uint) : uint2           //! row rowi's mirror K and V row bases, ahead of the head's own
    def abstract rope_sin(cb, j : uint) : float          //! pair j's sin off the cos row at cb

    //! whether head workgroup `wid` is a V row's (`V_WG`: past the q and the `nkh` k heads)
    def v_head(wid, nkh : uint) : bool {
        static_if (V_WG) {
            return wid >= pa.nh + nkh
        } else {
            return false
        }
    }

    def st_q(i : uint; v : float) {
        qrows[i] = v
        static_if (HX) {
            qh[i] = half_sat(v * pa.hscale)
        }
    }

    def st_k(i : uint; v : float) {
        static_if (CLAMP16) {
            kmir[i] = half_sat(v)
        } else {
            kmir[i] = v
        }
    }

    def st_v(i : uint; v : float) {
        static_if (CLAMP16) {
            vmir[i] = half_sat(v)
        } else {
            vmir[i] = v
        }
    }

    //! rotation pair p's two elements of a head: NEOX (p, p + half) inside the rotated span, NORM (2p, 2p + 1), and past the span
    //! the identity pairs that cover [2 half, hs)
    def pair_ix(p : uint) : uint2 => (pa.neox != 0u ? (p < pa.half ? uint2(p, p + pa.half) : uint2(p + pa.half, p + pa.hs / 2u))
        : uint2(2u * p, 2u * p + 1u))

    //! head workgroup `wid` of row `rowi`, thread `tid` holding `EPT` of the head's rotation pairs in registers
    def qkn_head(rowi, wid, tid : uint) {   // nolint:STYLE037,STYLE038 - the norm, the rotation and both stores ride the pairs' registers
        let nkh = pa.kvd / max(1u, pa.hs)
        let is_v = v_head(wid, nkh)
        let is_k = wid >= pa.nh && !is_v
        let h = is_v ? wid - pa.nh - nkh : (is_k ? wid - pa.nh : wid)
        let qstride = pa.qstride == 0u ? pa.hs : pa.qstride
        let prow = pa.win != 0u ? pa.row0 + rowi : rowi
        let qb = prow * pa.qrow + h * qstride
        let kvb = prow * pa.kvrow
        let vb = pa.qd + pa.kvd + h * pa.hs   //! the V head's offset in the projection row and in the layer's bias block
        let vi = pa.win != 0u ? pa.vbase + kvb + h * pa.hs : kvb + vb
        let vplain = is_v && pa.vfk == 0u   //! a V workgroup reading its own projection
        let rdk = is_k || (is_v && !vplain)
        let sbase = rdk ? kvb + (pa.win != 0u ? pa.kbase : pa.qd) + h * pa.hs : (vplain ? vi : qb)
        let bbase = rdk ? pa.qd + h * pa.hs : (vplain ? vb : h * pa.hs)
        let cb = prow * pa.cosrow + pa.cbase
        let mb = mir_base(rowi)
        let mk = mb.x + h * pa.hs
        let mv = mb.y + h * pa.hs
        let npair = pa.hs / 2u
        var a, b : float[EPT]
        var ss = 0.0
        for [unroll_full] (c in range(EPT)) {
            let p = tid + uint(c) * LANES
            a[c] = 0.0
            b[c] = 0.0
            if (p < npair) {
                let ix = pair_ix(p)
                a[c] = (rdk || vplain ? kvsrc[sbase + ix.x] : qrows[sbase + ix.x]) + (pa.hasb != 0u ? bias[pa.boff + bbase + ix.x] : 0.0)
                b[c] = (rdk || vplain ? kvsrc[sbase + ix.y] : qrows[sbase + ix.y]) + (pa.hasb != 0u ? bias[pa.boff + bbase + ix.y] : 0.0)
                ss += a[c] * a[c] + b[c] * b[c]
            }
        }
        if (pa.nreal != 0u && rowi >= pa.nreal) {   //! a pad row
            if (!is_k && !is_v) {
                for [unroll_full] (c in range(EPT)) {
                    let p = tid + uint(c) * LANES
                    if (p < npair) {
                        let ix = pair_ix(p)
                        st_q(qb + ix.x, a[c])
                        st_q(qb + ix.y, b[c])
                    }
                }
            }
            return
        }
        var inv = 1.0
        if (pa.skipnorm == 0u && (!vplain || pa.vnorm != 0u)) {
            wg_rms_inv_into(ss, pa.hs, pa.eps, 0u, inv)
        }
        let woff = is_k ? pa.kwoff : pa.qwoff
        for [unroll_full] (c in range(EPT)) {
            let p = tid + uint(c) * LANES
            if (p < npair) {
                let ix = pair_ix(p)
                if (is_v) {
                    st_v(mv + ix.x, a[c] * inv)
                    st_v(mv + ix.y, b[c] * inv)
                } else {
                    let a0 = pa.skipnorm != 0u ? a[c] : wn[woff + ix.x] * (a[c] * inv)
                    let a1 = pa.skipnorm != 0u ? b[c] : wn[woff + ix.y] * (b[c] * inv)
                    var r0 = a0
                    var r1 = a1
                    if (p < pa.half) {
                        let fcr = cossin[cb + p]
                        let fci = rope_sin(cb, p)
                        r0 = a0 * fcr - a1 * fci
                        r1 = a0 * fci + a1 * fcr
                    }
                    if (is_k) {
                        st_k(mk + ix.x, r0)
                        st_k(mk + ix.y, r1)
                    } else {
                        st_q(qb + ix.x, r0)
                        st_q(qb + ix.y, r1)
                    }
                }
            }
        }
        static_if (!V_WG) {
            if (is_k) {
                var wa, wb : float[EPT]
                var ws = 0.0
                for [unroll_full] (c in range(EPT)) {
                    let p = tid + uint(c) * LANES
                    wa[c] = 0.0
                    wb[c] = 0.0
                    if (pa.vfk != 0u) {
                        wa[c] = a[c] * inv
                        wb[c] = b[c] * inv
                    } elif (p < npair) {
                        let ix = pair_ix(p)
                        wa[c] = kvsrc[vi + ix.x] + (pa.hasb != 0u ? bias[pa.boff + vb + ix.x] : 0.0)
                        wb[c] = kvsrc[vi + ix.y] + (pa.hasb != 0u ? bias[pa.boff + vb + ix.y] : 0.0)
                        ws += wa[c] * wa[c] + wb[c] * wb[c]
                    }
                }
                if (pa.vfk == 0u && pa.vnorm != 0u) {
                    var vinv = 0.0
                    wg_rms_inv_into(ws, pa.hs, pa.eps, 1u, vinv)
                    for [unroll_full] (c in range(EPT)) {
                        wa[c] *= vinv
                        wb[c] *= vinv
                    }
                }
                for [unroll_full] (c in range(EPT)) {
                    let p = tid + uint(c) * LANES
                    if (p < npair) {
                        let ix = pair_ix(p)
                        st_v(mv + ix.x, wa[c])
                        st_v(mv + ix.y, wb[c])
                    }
                }
            }
        }
    }
}

struct GkRouterArgs {
    ne : uint            //! the expert rows
    dim : uint           //! the row width
    woff : uint          //! the layer's row base in the router plane (elements)
    obase : uint         //! the logits' base
    norm_on : uint       //! x read as its weightless rms under the norm row at `nwoff` (the gemma-4 router's input; `NORM` stamps)
    nwoff : uint
    eps : float
    ncols : uint         //! the x rows the dispatch serves past `col0` (0 reads as one): column c's logits at `obase + c ne`, or under a
                         //! split at `obase + c ne_split` and past `ne_split` at `obase_split + c (ne - ne_split)`
    ne_split : uint      //! the row the second output base takes over at (the deltanet beta and alpha rows in one dispatch); 0 = one base
    obase_split : uint
    col0 : uint          //! the dispatch's first column
    xs : uint            //! a column's stride in x, elements (0 = dim)
    hasb : uint          //! the bias row at `boff` added (`BIAS` stamps: gpt-oss)
    boff : uint
}

//! The router GEMV: expert row e's logit for every column, the row's weights read once for all of them; `ROW_WG` gives a row a
//! workgroup of `LANES` (the subgroups' partials crossing the workgroup slab), else a subgroup (`gl_NumSubgroups` rows a
//! workgroup); a lane steps `VW` elements a load: 1 (the f32 rows), 2 (the f16 rows as half pairs), 4 (f32 rows and x as float4)
[ |> template_struct_instance]
class template GkRouterGemv {
    @ssbo @binding = 0 @role = "weight" w : array<KT>                                 //! the rows, row-major [ne x dim] a layer, VW elements a word
    @ssbo @binding = 1 @role = "read" x : array<XT>                                   //! the columns' rows, VW elements a word
    @ssbo @binding = 2 @role = "write" logits : array<float>
    @ssbo @binding = 3 @role = "weight" @template_gate = NORM wn : array<float>       //! the norms plane: the row at `nwoff` scales x under `norm_on`
    @ssbo @binding = 4 @role = "weight" @template_gate = BIAS rb : array<float>       //! the bias rows
    @kargs pa : GkRouterArgs
    @workgroup @template_gate = ROW_WG part : float[256]   //! a subgroup's NC column partials at `sg * 8`: the dot's in 0..127, the norm's in 128..255
    @template_constant ROW_WG : bool = true
    @template_constant VW : uint = 1u
    @template_constant NC : int = 8
    @template_constant COL_CLAMP : bool = false   //!< the columns past the last read it again and store nothing, no branch in the dot; else they skip
    @template_constant NORM : bool = false
    @template_constant BIAS : bool = false

    //! x's element i of the column at element xb (VW 1 and 2), under the norm row when `norm_on`
    [spirv_inline]
    def xn(xb, i : uint) : float {
        static_if (VW == 4u) {
            return 0.0
        } else {
            static_if (NORM) {
                return pa.norm_on != 0u ? x[xb + i] * wn[pa.nwoff + i] : x[xb + i]
            } else {
                return x[xb + i]
            }
        }
    }

    //! word i of the row dotted with the column at xb into a scalar - an accumulator pointer down the call stalls the Vulkan driver
    [spirv_inline]
    def word_dot_into(wv : KT; xb, i : uint; var d : float&) {
        static_if (VW == 4u) {
            d = dot4_seq(wv, x[xb / 4u + i])
        } else {
            static_if (VW == 2u) {
                d = unpackHalf2x16(wv).x * xn(xb, 2u * i) + unpackHalf2x16(wv).y * xn(xb, 2u * i + 1u)
            } else {
                d = float(wv) * xn(xb, i)   // nolint:PERF020,LINT019 - KT converts; the f32 stamp's cast is free, the rule fires where it lands
            }
        }
    }

    [spirv_inline]
    def bias_at(e : uint) : float {
        static_if (BIAS) {
            return pa.hasb != 0u ? rb[pa.boff + e] : 0.0
        } else {
            return 0.0
        }
    }

    //! row e's logit of column cc
    [spirv_inline]
    def store_logit(e, cc : uint; v : float) {
        var o = pa.obase + cc * (pa.ne_split != 0u ? pa.ne_split : pa.ne) + e
        if (pa.ne_split != 0u && e >= pa.ne_split) {
            o = pa.obase_split + cc * (pa.ne - pa.ne_split) + (e - pa.ne_split)
        }
        logits[o] = v + bias_at(e)
    }

    //! a column's lane partial `v` summed over the subgroup in place, and under `ROW_WG` published at slot `slot` x 128 of the slab
    [spirv_inline]
    def col_part_into(var v : float&; c, slot : uint) {
        v = gk_subgroup_add(v)
        static_if (ROW_WG) {
            if (gl_SubgroupInvocationID == 0u) {
                part[slot * 128u + gl_SubgroupID * 8u + c] = v
            }
        }
    }

    //! under `ROW_WG`, column c's workgroup sum off the slab, after the barrier that follows the partials
    [spirv_inline]
    def col_total_into(c, slot : uint; var tot : float&) {
        static_if (ROW_WG) {
            var a = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                a += part[slot * 128u + sgi * 8u + c]
                sgi++
            }
            tot = a
        }
    }

    //! the sum of squares of the column at element xb over the row's lanes' elements, onto `s`
    [spirv_inline]
    def col_sq_into(xb, lane, nl : uint; var s : float&) {
        static_if (NORM) {
            var a = 0.0
            var j = lane
            while (j < pa.dim) {
                a += x[xb + j] * x[xb + j]
                j += nl
            }
            s = a
        }
    }

    //! row e's logits over the dispatch's columns, the row's lane `lane` of `nl`; under `COL_CLAMP` every one of NC columns unrolled, else
    //! a loop over the live ones
    [spirv_inline]
    def router_row(e, lane, nl : uint) {   // nolint:STYLE038 - the norm, the dot, the partials and the stores share the column registers
        let cb = gl_WorkGroupID.y * uint(NC)
        let nct = max(pa.ncols, 1u)
        let ncv = min(uint(NC), nct - cb)
        let xs = pa.xs == 0u ? pa.dim : pa.xs
        let xc0 = (pa.col0 + cb) * xs
        let nw = pa.dim / VW
        let wb = (pa.woff + e * pa.dim) / VW
        var inv, acc : float[NC]
        for [unroll_full] (c in range(NC)) {
            inv[c] = 1.0
            acc[c] = 0.0
        }
        static_if (NORM) {
            if (pa.norm_on != 0u) {
                var c = 0u
                while (c < ncv) {
                    var v = 0.0
                    col_sq_into(xc0 + c * xs, lane, nl, v)
                    inv[c] = v
                    c++
                }
                c = 0u
                while (c < ncv) {
                    var v = inv[c]
                    col_part_into(v, c, 1u)
                    inv[c] = v
                    c++
                }
                static_if (ROW_WG) {
                    barrier()
                    c = 0u
                    while (c < ncv) {
                        var v = 0.0
                        col_total_into(c, 1u, v)
                        inv[c] = v
                        c++
                    }
                }
                c = 0u
                while (c < ncv) {
                    inv[c] = rms_inv(inv[c] / float(pa.dim) + pa.eps, false)
                    c++
                }
            }
        }
        var i = lane
        while (i < nw) {
            let wv = w[wb + i]
            static_if (COL_CLAMP) {
                for [unroll_full] (c in range(NC)) {
                    var d = 0.0
                    word_dot_into(wv, (pa.col0 + min(cb + uint(c), nct - 1u)) * xs, i, d)
                    acc[c] += d
                }
            } else {
                var c = 0u
                while (c < ncv) {
                    var d = 0.0
                    word_dot_into(wv, xc0 + c * xs, i, d)
                    acc[c] += d
                    c++
                }
            }
            i += nl
        }
        var c = 0u
        while (c < ncv) {
            var v = acc[c]
            col_part_into(v, c, 0u)
            acc[c] = v
            c++
        }
        static_if (ROW_WG) {
            barrier()
        }
        if (lane == 0u) {
            c = 0u
            while (c < ncv) {
                var v = acc[c]
                col_total_into(c, 0u, v)
                acc[c] = v
                store_logit(e, pa.col0 + cb + c, acc[c] * inv[c])
                c++
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

struct GkBiasGateArgs {
    d : uint        //! the row width
    nelem : uint
    goff : uint     //! the gate bias row's base in wn
    uoff : uint     //! the up bias row's base in wn
}

//! a gated hidden from two GEMM outputs with their bias rows: g = silu(g + bg) . (u + bu), in place over g (OUT32) or as saturated
//! halves (OUT16, the f16 tile's feed); four elements an invocation, each guarded against the count
[ |> template_struct_instance]
class template GkBiasGate {
    @template_constant OUT16 : bool = false
    @template_constant OUT32 : bool = true
    @template_constant ACT : uint = 3u   //!< the gate's activation: `BIAS_ACT_SILU`, spelled as its literal - every tower this stamps for gates its hidden so
    @ssbo @binding = 0 @role = "readwrite" g : array<float>
    @ssbo @binding = 1 @role = "read" u : array<float>
    @ssbo @binding = 2 @role = "weight" wn : array<float>
    @ssbo @binding = 3 @role = "write" @template_gate = OUT16 outh : array<float16>
    @kargs pa : GkBiasGateArgs

    def gate_at(e : uint) {
        let c = e % max(1u, pa.d)
        let v = act_f32(g[e] + wn[pa.goff + c], ACT) * (u[e] + wn[pa.uoff + c])
        static_if (OUT16) {
            outh[e] = half_sat(v)
        } else {
            static_if (OUT32) {
                g[e] = v
            }
        }
    }

    def bias_gate_body {
        let e = gl_GlobalInvocationID.x * 4u
        if (e < pa.nelem) {
            gate_at(e)
            if (e + 1u < pa.nelem) {
                gate_at(e + 1u)
            }
            if (e + 2u < pa.nelem) {
                gate_at(e + 2u)
            }
            if (e + 3u < pa.nelem) {
                gate_at(e + 3u)
            }
        }
    }
}

struct GkWinAttnArgs {
    nheads : uint
    hs : uint       //! the head width (read on a stamp whose HS is 0)
    scale : float
}

//! the block-diagonal window attention of the qwen2.5 ViT over compact [npos x nheads . hs] rows, plain f32 softmax, no mask: one
//! workgroup a (window, head), 256 threads = 64 query rows x 4 lanes - the lanes split the keys for the scores and the head's
//! dims for the output; window w's rows are wlo[w] .. wlo[w + 1], 64 at most, the score block staged in workgroup memory
[ |> template_struct_instance]
class template GkWinAttn {
    @template_constant HS : uint = 0u   //! the head width the stamp is pinned to; 0 reads it from the arguments
    @ssbo @binding = 0 @role = "read" q : array<float>
    @ssbo @binding = 1 @role = "read" k : array<float>
    @ssbo @binding = 2 @role = "read" v : array<float>
    @ssbo @binding = 3 @role = "write" att : array<float>
    @ssbo @binding = 4 @role = "read" wlo : array<uint>
    @workgroup sc : float[4096]
    @kargs pa : GkWinAttnArgs

    def win_attn_body {
        let wid = gl_WorkGroupID.x
        let hs = gk_fixed_or(HS, pa.hs)
        let nheads = max(1u, pa.nheads)
        let d = nheads * hs
        let w = wid / nheads
        let hh = wid - w * nheads
        let lo = wlo[w]
        let cnt = wlo[w + 1u] - lo
        let tid = gl_LocalInvocationID.x
        let r = tid >> 2u
        let lane = tid & 3u
        let live = r < cnt
        let qb = (lo + r) * d + hh * hs
        if (live) {
            var j = lane
            while (j < cnt) {
                let kb = (lo + j) * d + hh * hs
                var s = 0.0
                var i = 0u
                while (i < hs) {
                    s += q[qb + i] * k[kb + i]
                    i++
                }
                sc[r * 64u + j] = s * pa.scale
                j += 4u
            }
        }
        barrier()
        if (live) {
            var m = -3.4028235e38
            var j = 0u
            while (j < cnt) {
                m = max(m, sc[r * 64u + j])
                j++
            }
            j = lane
            while (j < cnt) {
                sc[r * 64u + j] = exp(sc[r * 64u + j] - m)
                j += 4u
            }
        }
        barrier()
        if (live) {
            var l = 0.0
            var j = 0u
            while (j < cnt) {
                l += sc[r * 64u + j]
                j++
            }
            let inv = 1.0 / l
            var dd = lane
            while (dd < hs) {
                var acc = 0.0
                j = 0u
                while (j < cnt) {
                    acc += sc[r * 64u + j] * v[(lo + j) * d + hh * hs + dd]
                    j++
                }
                att[qb + dd] = acc * inv
                dd += 4u
            }
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
            let ow = max(1u, pa.gw / max(1u, pa.k))
            let oy = o / ow
            let ox = o % ow
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
    boff : uint    //! the standardize bias row's element base in b, the scale row's in m
    moff : uint
    std : uint     //! 0: the rows take the scale alone (a tower with no standardize rows)
}

//! a vision tail's row standardize, in place: x = (x * scale - b[c]) * m[c] over rows of `width` (x * scale alone under `std` = 0),
//! one element an invocation
[ |> template_struct_instance]
class template GkAffineRows {
    @ssbo @binding = 0 @role = "readwrite" x : array<float>
    @ssbo @binding = 1 @role = "weight" b : array<float>
    @ssbo @binding = 2 @role = "weight" m : array<float>
    @kargs pa : GkAffineRowsArgs

    def affine_rows_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.total) {
            let c = gid % pa.width
            let v = x[gid] * pa.scale
            x[gid] = pa.std != 0u ? (v - b[pa.boff + c]) * m[pa.moff + c] : v
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

//! the rms rows alone (the ADD-off RMS stamps): `rows` of width `d` against the weight row at `woff` in the norms plane
def gk_rms_args(d, rows : uint; eps : float; woff : uint) : GkPostAddLnArgs => gk_ln_args(d, rows, eps, woff, 0u)

//! the biased residual seam: x += y + the bias row at `boff`, then under `ln_on` the next pre-norm against the rows at `lnoff` / `lnboff`
def gk_add_ln_args(d, rows : uint; eps : float; boff : uint; ln_on : bool; lnoff, lnboff : uint) : GkPostAddLnArgs
    => GkPostAddLnArgs(d = d, rows = rows, woff = 0u, boff = boff, eps = eps, ln_on = ln_on ? 1u : 0u, lnoff = lnoff, lnboff = lnboff, ascale = 1.0, xboff = 0u)

//! the post-normed residual seam (the gemma towers): x += rms(y) . the post-norm row at `woff`, then under `ln_on` the next rms against the row at `lnoff`
def gk_add_post_rms_args(d, rows : uint; eps : float; woff : uint; ln_on : bool; lnoff : uint) : GkPostAddLnArgs
    => GkPostAddLnArgs(d = d, rows = rows, woff = woff, boff = 0u, eps = eps, ln_on = ln_on ? 1u : 0u, lnoff = lnoff, lnboff = 0u, ascale = 1.0, xboff = 0u)

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
                let xv = x[base + i]
                static_if (RMS) {
                    s1 += xv * xv
                } else {
                    s1 += xv
                }
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

struct GkHcInitArgs {
    dim : uint
    hc : uint
    rows : uint
}

//! the wide residual opens as hc copies of every embedded row, one element an invocation
[ |> template_struct_instance]
class template GkHcInit {
    @ssbo @binding = 0 @role = "read" x : array<float>     //! the embedded rows [rows x dim]
    @ssbo @binding = 1 @role = "write" res : array<float>  //! the wide rows [rows x hc x dim]
    @kargs pa : GkHcInitArgs

    def hc_init_body {
        let e = gl_GlobalInvocationID.x
        let hcd = pa.hc * pa.dim
        if (e < pa.rows * hcd) {
            let r = e / hcd
            let i = (e - r * hcd) % max(1u, pa.dim)
            res[e] = x[r * pa.dim + i]
        }
    }
}

struct GkCat2Args {
    d : uint
    rows : uint    //! the cat rows written, each [a row ; b row] at 2d
    rep : uint     //! the streams a position spans (0 reads as one): cat row r is position p's stream c
    smaj : uint    //! the cat rows stream-major (r = c * np + p), else position-major (r = p * rep + c)
    np : uint      //! the positions, under `smaj`
    a0 : uint      //! a's first row, its rows `as_` apart (0 = d): every stream of a position reads the position's a row
    as_ : uint
    b0 : uint      //! b's first row: position p's stream c reads b row `b0 + p * rep + c`
}

//! the MTP head's eh_proj input: every cat row the concatenation [a row ; b row], one element pair an invocation - the
//! prompt warm's and the draft prelude's cat on both homes
[ |> template_struct_instance]
class template GkCat2 {
    @ssbo @binding = 0 @role = "write" y : array<float>
    @ssbo @binding = 1 @role = "read" a : array<float>
    @ssbo @binding = 2 @role = "read" b : array<float>
    @kargs pa : GkCat2Args

    def cat2_body {
        let e = gl_GlobalInvocationID.x
        if (e < pa.rows * pa.d) {
            let r = e / pa.d
            let i = e - r * pa.d
            let rep = max(pa.rep, 1u)
            let p = pa.smaj != 0u ? r % max(pa.np, 1u) : r / rep
            let c = pa.smaj != 0u ? r / max(pa.np, 1u) : r % rep
            let ast = pa.as_ == 0u ? pa.d : pa.as_
            y[r * 2u * pa.d + i] = a[(pa.a0 + p) * ast + i]
            y[r * 2u * pa.d + pa.d + i] = b[(pa.b0 + p * rep + c) * pa.d + i]
        }
    }
}

struct GkHcNormArgs {
    dim : uint
    hc : uint     //! the residual streams: workgroup g norms stream g % hc of row row0 + g / hc
    woff : uint   //! the site's gamma rows in wn: stream c's at woff + c * dim
    row0 : uint   //! the first wide row read; the normed rows land from row 0 (the head mixer norms the window's last row alone)
    eps : float
}

//! the grouped rms of the wide residual rows: stream c of a row normalizes over dim and scales by gamma row c - one workgroup
//! a stream row, the CPU `rmsnorm`'s (x . inv) . w order; the inverse norm's spelling is the stamp's (`RSQRT`)
[ |> template_struct_instance]
class template GkHcNorm : GkWgReduce {
    @ssbo @binding = 0 @role = "read" res : array<float>    //! the wide rows, [rows x hc x dim]
    @ssbo @binding = 1 @role = "write" xn : array<float>    //! the normed streams, the same layout
    @ssbo @binding = 2 @role = "weight" wn : array<float>
    @kargs pa : GkHcNormArgs
    @template_constant TG : uint = 256u   //!< the workgroup's lanes, the row loops' stride

    def hc_norm_body {
        let g = gl_WorkGroupID.x
        let src = pa.row0 * pa.hc * pa.dim + g * pa.dim
        let base = g * pa.dim
        let wb = pa.woff + (g % max(1u, pa.hc)) * pa.dim
        let tid = gl_LocalInvocationID.x
        var ss = 0.0
        var i = tid
        while (i < pa.dim) {
            let v = res[src + i]
            ss += v * v
            i += TG
        }
        var inv = 0.0
        wg_rms_inv_into(ss, pa.dim, pa.eps, 0u, inv)
        i = tid
        while (i < pa.dim) {
            xn[base + i] = (res[src + i] * inv) * wn[wb + i]
            i += TG
        }
    }
}

struct GkHcMixArgs {
    dim : uint
    hc : uint
    rows : uint
}

//! the mixer's tail: the block input row xb = the mean over the streams of the normed stream gated by the sigmoid of its
//! up-projection row (the CPU `hc_mix_tail`'s x . sigmoid(g) and its 1 / hc scale), one element an invocation over the rows
[ |> template_struct_instance]
class template GkHcMix {
    @ssbo @binding = 0 @role = "read" xn : array<float>     //! the normed streams [rows x hc x dim]
    @ssbo @binding = 1 @role = "read" gate : array<float>   //! the up projection [rows x hc x dim]
    @ssbo @binding = 2 @role = "write" xb : array<float>    //! the block input rows [rows x dim]
    @kargs pa : GkHcMixArgs

    def hc_mix_body {
        let e = gl_GlobalInvocationID.x
        if (e < pa.rows * pa.dim) {
            let r = e / pa.dim
            let i = e - r * pa.dim
            let rb = r * pa.hc * pa.dim
            var acc = 0.0
            var c = 0u
            while (c < pa.hc) {
                let x = rb + c * pa.dim + i
                acc += xn[x] * sigmoid_f32(gate[x])
                c++
            }
            xb[e] = acc * (1.0 / float(pa.hc))
        }
    }
}

struct GkHcCombArgs {
    dim : uint
    hc : uint
    moe_on : uint    //! the block output is the routed sum in `acc`, plus the shared expert's row under `sh_on` (the `MOE` stamp's)
    sh_on : uint     //! ... the shared row `outr` at its gate joins the routed sum (0 = a routed block with no shared expert)
    gated : uint     //! ... the gate: the sigmoid of the router logit at the row's last column, `lstride - 1` (0 = the shared row at unit weight)
    rows : uint      //! the rows scattered, from `row0` (a window's last-layer slice scatters the rows the classifier reads)
    row0 : uint
    lstride : uint   //! the logits row stride (elements)
}

//! the scatter of a block's output into the streams: res_c += out x 2 sigmoid(inject_c / hc), one element an invocation over
//! the rows; on the `MOE` stamp a MoE layer's output row is the routed experts' sum landed in `acc`, plus the shared expert's
//! row at its gate
[ |> template_struct_instance]
class template GkHcCombine {
    @ssbo @binding = 0 @role = "readwrite" res : array<float>                   //! the wide residual [rows x hc x dim], updated in place
    @ssbo @binding = 1 @role = "read" outr : array<float>                       //! the block output rows [rows x dim]
    @ssbo @binding = 2 @role = "read" inj : array<float>                        //! the site's hc scatter logits [rows x hc]
    @ssbo @binding = 3 @role = "read" @template_gate = MOE acc : array<float>   //! the routed sums [rows x dim], read under `moe_on`
    @ssbo @binding = 4 @role = "read" @template_gate = MOE logits : array<float>   //! the router logits rows, read under `gated`
    @kargs pa : GkHcCombArgs
    @template_constant MOE : bool = false

    def hc_combine_body {
        let g = gl_GlobalInvocationID.x
        let hcd = pa.hc * pa.dim
        if (g < pa.rows * hcd) {
            let r = pa.row0 + g / hcd
            let ee = g % hcd
            let c = ee / pa.dim
            let i = ee - c * pa.dim
            let w = 2.0 / (1.0 + exp(-inj[r * pa.hc + c] / float(pa.hc)))
            var v = outr[r * pa.dim + i]
            static_if (MOE) {
                if (pa.moe_on != 0u) {
                    let gt = pa.gated != 0u ? sigmoid_f32(logits[r * pa.lstride + pa.lstride - 1u]) : 1.0
                    v = acc[r * pa.dim + i] + (pa.sh_on != 0u ? v * gt : 0.0)
                }
            }
            res[r * hcd + ee] += v * w
        }
    }
}

struct GkPleGateArgs {
    dim : uint
    hc : uint
    nrows : uint    //! the conv ring's rows: (taps - 1) x dilation + 1
    rows : uint     //! the Vulkan stamp's row form: 0 = the recorded token's row into its ring slot, else a window's rows into the panel
    pos0 : uint     //! the position of row 0
    eps : float
    inv_sq : float  //! 1 / sqrt(dim): the key . query dot's scale, the CPU chain's own
    direct : uint   //! the Metal stamp's: 1 = row r's conv input into panel row r, 0 = into the ring slot of position pos0 + r
    roff : uint     //! the element bases of the residual, key, value, gated and conv-input rows (a joint verify's stream; 0 elsewhere)
    koff : uint
    voff : uint
    goff : uint
    coff : uint
}

//! The n-gram side input's gate, one workgroup a stream row: the normed key against the normed residual stream, the signed square
//! root of their scaled dot through a sigmoid, the value row scaled by it into `gated`, and the grouped-normed gated row as the
//! conv input row, landed where the stamp's `cin_base` / `cin_store` put it; `wn` holds the key, query and conv-input norm rows
[ |> template_struct_instance]
class template GkPleGate : GkWgReduce {
    @ssbo @binding = 0 @role = "read" key : array<float>      //! the key projections [rows x hc x dim]
    @ssbo @binding = 1 @role = "read" res : array<float>      //! the wide residual [rows x hc x dim]
    @ssbo @binding = 2 @role = "read" val : array<float>      //! the value projections [rows x dim]
    @ssbo @binding = 3 @role = "write" gated : array<float>   //! val x the stream's gate, [rows x hc x dim]
    @ssbo @binding = 5 @role = "weight" wn : array<float>     //! [key norms | query norms | conv-input norms | conv taps]
    @kargs pa : GkPleGateArgs
    @template_constant TG : uint = 256u   //!< the workgroup's lanes, the row loops' stride

    def abstract cin_base(r, c : uint) : uint             //! the element base of stream c's conv input row of row r
    def abstract cin_store(e : uint; v : float) : void    //! ... one element of it

    def ple_gate_body {
        let wg = gl_WorkGroupID.x
        let r = wg / max(1u, pa.hc)
        let c = wg - r * pa.hc
        let base = wg * pa.dim
        let vbase = r * pa.dim
        let hcd = pa.hc * pa.dim
        let tid = gl_LocalInvocationID.x
        var sk = 0.0
        var sq = 0.0
        var i = tid
        while (i < pa.dim) {
            let kv = key[pa.koff + base + i]
            let qv = res[pa.roff + base + i]
            sk += kv * kv
            sq += qv * qv
            i += TG
        }
        var ink = 0.0
        wg_rms_inv_into(sk, pa.dim, pa.eps, 0u, ink)
        var inq = 0.0
        wg_rms_inv_into(sq, pa.dim, pa.eps, 1u, inq)
        let cb = c * pa.dim
        var pd = 0.0
        i = tid
        while (i < pa.dim) {
            let kn = (key[pa.koff + base + i] * ink) * wn[cb + i]
            let qn = (res[pa.roff + base + i] * inq) * wn[hcd + cb + i]
            pd += kn * qn
            i += TG
        }
        var sd = 0.0
        wg_sum_into(pd, 0u, sd)
        let sv = sd * pa.inv_sq
        let mag = sqrt(max(abs(sv), 1.0e-6))
        let sg = sv > 0.0 ? mag : (sv < 0.0 ? -mag : 0.0)
        let g = sigmoid_f32(sg)
        var sg2 = 0.0
        i = tid
        while (i < pa.dim) {
            let gv = val[pa.voff + vbase + i] * g
            gated[pa.goff + base + i] = gv
            sg2 += gv * gv
            i += TG
        }
        var ing = 0.0
        wg_rms_inv_into(sg2, pa.dim, pa.eps, 1u, ing)
        let cbase = cin_base(r, c)
        i = tid
        while (i < pa.dim) {
            cin_store(cbase + i, ((val[pa.voff + vbase + i] * g) * ing) * wn[2u * hcd + cb + i])
            i += TG
        }
    }
}

struct GkPleConvArgs {
    dim : uint
    hc : uint
    kern : uint     //! the conv's taps
    dil : uint      //! ... dilated by the n-gram size
    rows : uint     //! the rows (the Vulkan stamp's 0 = the recorded token's row off its token block)
    pos0 : uint     //! the position of row 0
    direct : uint   //! the Metal stamp's: 1 = a tap at or past pos0 reads the window panel, 0 = every tap the ring
    roff : uint     //! the element bases of the residual, gated, panel and ring rows (a joint verify's stream; 0 elsewhere)
    goff : uint
    coff : uint
    ringoff : uint
}

//! The side input's landing, one element an invocation over the rows: the dilated depthwise causal conv (a tap before the sequence
//! reads zero) over the taps the stamp's `tap` reads off its ring and panel, its silu and the gated value added to the residual
//! stream in the CPU chain's order, (res + gated) + silu(conv); the taps sit in `wn` past the three norm rows
[ |> template_struct_instance]
class template GkPleConv {
    @ssbo @binding = 0 @role = "readwrite" res : array<float>   //! the wide residual [rows x hc x dim], updated in place
    @ssbo @binding = 1 @role = "read" gated : array<float>
    @ssbo @binding = 3 @role = "weight" wn : array<float>       //! [key norms | query norms | conv-input norms | conv taps]
    @kargs pa : GkPleConvArgs

    def abstract row_pos(r : uint) : int                     //! row r's position
    def abstract tap(e : uint; src_pos : int; nrows : uint) : float   //! channel e's conv input at position src_pos, off a ring of nrows rows or the panel

    def ple_conv_body {
        let g = gl_GlobalInvocationID.x
        let hcd = pa.hc * pa.dim
        let nrows = max(1u, (pa.kern - 1u) * pa.dil + 1u)
        let conv_off = 3u * hcd
        if (g < max(pa.rows, 1u) * hcd) {
            let r = g / hcd
            let e = g - r * hcd
            let pos = row_pos(r)
            var co = 0.0
            var k = 0u
            while (k < pa.kern) {
                let src_pos = pos - int((pa.kern - 1u - k) * pa.dil)
                if (src_pos >= 0) {
                    co += wn[conv_off + e * pa.kern + k] * tap(e, src_pos, nrows)
                }
                k++
            }
            res[pa.roff + g] = (res[pa.roff + g] + gated[pa.goff + g]) + silu_f32(co)
        }
    }
}

struct GkClsEpiArgs {
    vocab : uint
    cap : float     //! the final logit softcap; 0 = none
    nsupp : uint    //! suppressed token ids in `supp`
    rows : uint     //! logits rows in `y`, `vocab` apart
}

//! The classifier tail's epilogue in place, an invocation a logit over `rows` rows: the final logit softcap (`softcap_exp`), then
//! the checkpoint's suppressed ids pinned to -1e30 - after the cap, so the pin survives it; each logit scans the few ids
[ |> template_struct_instance]
class template GkClsEpilogue {
    @ssbo @binding = 0 @role = "readwrite" y : array<float>   //! the logits rows
    @ssbo @binding = 1 @role = "weight" supp : array<uint>    //! the suppressed ids (a placeholder when nsupp is 0)
    @kargs pa : GkClsEpiArgs

    def epi_elem(gid : uint) {
        if (gid < pa.vocab * pa.rows) {
            let id = pa.nsupp > 0u ? gid % pa.vocab : 0u   //! the row's id, wanted by the pins alone
            var v = y[gid]
            if (pa.cap > 0.0) {
                v = softcap_exp(v, pa.cap)
            }
            var j = 0u
            while (j < pa.nsupp) {
                if (supp[j] == id) {
                    v = -1.0e30
                }
                j++
            }
            y[gid] = v
        }
    }
}

let GK_NO_DSCALE = 0xFFFFFFFFu   //! `GkTopKArgs.dsoff`'s "no per-expert down scale" sentinel

struct GkTopKArgs {
    ne : uint
    k : uint
    norm : uint      //! renormalize the selected weights to sum 1
    wscale : float   //! 0 or 1 = none
    base1 : uint     //! the gate stack's expert-0 weight block (stack-relative, in the plane's units) - the Vulkan decode's slot records
    stride1 : uint   //! ... blocks per expert
    base3 : uint
    stride3 : uint
    base2 : uint
    stride2 : uint
    xnb2 : uint      //! the down feed's blocks per hidden row: slot s reads row s (gate/up read their row's feed)
    dsoff : uint     //! this layer's row base in the per-expert down-scale plane (gemma-4: ffn_down_exps.scale folds into the routing weight); GK_NO_DSCALE = none
    lstride : uint   //! the logits row stride (elements): row p's logits sit at p * lstride
    xnb1 : uint      //! the feed's blocks per row - one feed row serves the gate and the up: row p's slots read it at p * xnb1
    rbias_on : uint  //! the router bias (gpt-oss): logit e takes rb[rboff + e] before the pick
    rboff : uint     //! ... this layer's row base in the router-bias plane ([ne] a layer)
    raw : uint       //! select on the raw logits, no softmax (the Metal stamp's gpt-oss softmax_weight: the softmax runs over the picks)
}

//! The router's select over one row of `ne` logits: the softmax (max, exp floored at -80, sum) unless `raw`, then k picks of the
//! largest, the lowest expert on a tie, landing in `pick` / `pickw` for the stamp to write; lane `tid` holds experts tid + i x
//! LANES for i < DEPTH (a workgroup of LANES lanes - one simdgroup on Metal, 256 on Vulkan); `pick_weight` is the renorm
[ |> template_struct_instance]
class template GkTopKRow : GkWgReduce {
    @kargs pa : GkTopKArgs
    @template_constant LANES : uint = 256u   //!< the workgroup's lanes
    @template_constant DEPTH : int = 1       //!< the logits a lane holds
    @template_constant DSCALE : bool = false   //!< the per-expert down scale row folds into the picked weights where `dsoff` names one
    @workgroup parti : uint[64]
    @workgroup pick : uint[64]
    @workgroup pickw : float[64]

    //! row `p`'s logit of expert `e` as the select reads it; a stamp reads the row's plane
    def abstract logit_at(p, e : uint) : float

    //! expert `e`'s down scale in the layer's row at `dsoff`; a DSCALE stamp reads its plane
    def abstract down_scale(e : uint) : float

    [arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-topk-depth")]
    def select_row(row, ne, k : uint; raw : bool) {   // nolint:STYLE038 - the softmax and the k argmax passes over the lane's register logits, one pass a pick
        let tid = gl_LocalInvocationID.x
        var lg : float[DEPTH]
        var lmax = -3.0e38
        for [unroll_full] (i in range(DEPTH)) {
            let e = tid + uint(i) * LANES
            lg[i] = e < ne ? logit_at(row, e) : -3.0e38
            lmax = max(lmax, lg[i])
        }
        if (!raw) {
            var wmax = 0.0
            static_if (LANES <= 32u) {
                wmax = gk_subgroup_max(lmax)
            } else {
                wg_max_into(lmax, 0u, wmax)
            }
            var esum = 0.0
            for [unroll_full] (i in range(DEPTH)) {
                let ev = tid + uint(i) * LANES < ne ? exp(max(lg[i] - wmax, -80.0)) : 0.0
                lg[i] = ev
                esum += ev
            }
            var wsum = 0.0
            static_if (LANES <= 32u) {
                wsum = gk_subgroup_add(esum)
            } else {
                wg_sum_into(esum, 1u, wsum)
            }
            for [unroll_full] (i in range(DEPTH)) {
                lg[i] = tid + uint(i) * LANES < ne ? lg[i] / wsum : -3.0e38
            }
        }
        var j = 0u
        while (j < k) {
            var p = lg[0]
            var idx = tid
            for [unroll_full] (i in range(1, DEPTH)) {
                if (lg[i] > p) {   //! strictly: a tie keeps the lower expert
                    p = lg[i]
                    idx = tid + uint(i) * LANES
                }
            }
            let vm = gk_subgroup_max(p)
            let im = gk_subgroup_min_u(p == vm ? idx : 0xFFFFFFFFu)
            var bsel = im
            static_if (LANES <= 32u) {   //! one subgroup: its pick is the workgroup's, every lane holds it
                if (tid == 0u) {
                    pick[j] = im
                    pickw[j] = vm
                }
            } else {
                if (gl_SubgroupInvocationID == 0u) {
                    part[gl_SubgroupID] = vm
                    parti[gl_SubgroupID] = im
                }
                barrier()
                if (tid == 0u) {
                    var bv = part[0u]
                    var bi = parti[0u]
                    var sgi = 1u
                    while (sgi < gl_NumSubgroups) {
                        let pv = part[sgi]
                        let pi = parti[sgi]
                        if (pv > bv || (pv == bv && pi < bi)) {
                            bv = pv
                            bi = pi
                        }
                        sgi++
                    }
                    pick[j] = bi
                    pickw[j] = bv
                }
                barrier()
                bsel = pick[j]
            }
            for [unroll_full] (i in range(DEPTH)) {
                if (tid + uint(i) * LANES == bsel) {
                    lg[i] = -3.0e38
                }
            }
            j++
        }
    }

    //! the k picks' weight sum their renorm divides by, floored (`moe_renorm_sum`), into `ws`
    def pick_wsum_into(k : uint; var ws : float&) {
        ws = 0.0
        var i = 0u
        while (i < k) {
            ws += pickw[i]
            i++
        }
        ws = moe_renorm_sum(ws)
    }

    //! pick jj (expert `ex`)'s routing weight into `wj`: renormalized over the sum `ws` when norm is set, then scaled
    def pick_weight_into(jj, ex, norm : uint; ws, wscale : float; var wj : float&) {
        wj = pickw[jj]
        if (norm != 0u) {
            wj /= ws
        }
        scale_weight(ex, wscale, wj)
    }

    //! a picked weight of expert `ex` under the model's global scale, then its down scale - the CPU select's order
    [arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#moe-wscale")]
    def scale_weight(ex : uint; wscale : float; var wj : float&) {
        if (wscale != 0.0 && wscale != 1.0) {
            wj *= wscale
        }
        static_if (DSCALE) {
            if (pa.dsoff != 0xFFFFFFFFu) {   //! GK_NO_DSCALE: the Metal emitter reads no module global
                wj *= down_scale(ex)
            }
        }
    }
}

struct GkQkRmsArgs {
    hs : uint
    nh : uint        //! the q head groups (first)
    nkvh : uint      //! the k head groups after them
    qstride : uint   //! a row's stride in qrows
    kstride : uint   //! ... and in krows
    qbase : uint     //! the q rows' element base in qrows
    kbase : uint     //! the k rows' base in krows
    kwd : uint       //! the k groups' write displacement past their read rows: 0 in place, else where a pre-norm K row lands normed (the Metal no-wv V fill)
    qwoff : uint     //! the q and k norm weight rows' bases in wn
    kwoff : uint
    eps : float
    qhs : uint       //! a q head's stride: hs, or 2 * hs on a gated model (0 reads as hs)
    vfk : uint       //! the k groups also write the V row: the raw k row scaled by its rms (a layer with no v plane)
    vnorm : uint     //! nkvh more groups norm the V rows in place, weightless (gemma-4's v-norm)
    vbase : uint     //! the V rows' base in krows (kstride apart), read under vnorm and written under vfk
}

//! The per-head RMSNorm of the q and k rows between the projections and rope (the qk-norm models), one workgroup a head row of
//! 256 lanes, in place: a head past 256 (gemma-4's global layers, 512) gives each lane a second element at lane + 256; under
//! `vfk` the k groups write V beside their norm, under `vnorm` the groups past the k ones scale the V rows by their own rms
[ |> template_struct_instance]
class template GkQkRms : GkWgReduce {
    @ssbo @binding = 0 @role = "readwrite" qrows : array<float>   //! the q rows, normed in place
    @ssbo @binding = 1 @role = "readwrite" krows : array<float>   //! the k rows, normed in place (the V rows sit at vbase in the same buffer)
    @ssbo @binding = 2 @role = "weight" wn : array<float>         //! the q/k norm weight rows
    @kargs pa : GkQkRmsArgs

    //! head group `hh` of row `r`: the q groups, then the k groups, then under `vnorm` the V groups
    def qk_rms_group(r, hh : uint) {   // nolint:STYLE037 - one head-row's norm: the q, k and v group selects and the two-element wide arm, one pass
        let is_q = hh < pa.nh
        let is_v = hh >= pa.nh + pa.nkvh
        let qhs = pa.qhs == 0u ? pa.hs : pa.qhs
        let kh = is_v ? hh - pa.nh - pa.nkvh : hh - pa.nh
        let rb = is_q ? pa.qbase + r * pa.qstride + hh * qhs : (is_v ? pa.vbase + r * pa.kstride + kh * pa.hs : pa.kbase + r * pa.kstride + kh * pa.hs)
        let wb = (is_q || is_v) ? rb : rb + pa.kwd
        let woff = is_q ? pa.qwoff : pa.kwoff
        let tid = gl_LocalInvocationID.x
        let t1 = tid + 256u
        let two = t1 < pa.hs
        var v = 0.0
        var v1 = 0.0
        if (tid < pa.hs) {
            v = is_q ? qrows[rb + tid] : krows[rb + tid]
        }
        if (two) {
            v1 = is_q ? qrows[rb + t1] : krows[rb + t1]
        }
        let ss = tid < pa.hs ? v * v + v1 * v1 : 0.0
        var inv = 0.0
        wg_rms_inv_into(ss, pa.hs, pa.eps, 0u, inv)
        if (tid < pa.hs) {
            let w = is_v ? 1.0 : wn[woff + tid]
            if (is_q) {
                qrows[wb + tid] = w * (v * inv)
            } else {
                krows[wb + tid] = w * (v * inv)
            }
        }
        if (two) {
            let w = is_v ? 1.0 : wn[woff + t1]
            if (is_q) {
                qrows[wb + t1] = w * (v1 * inv)
            } else {
                krows[wb + t1] = w * (v1 * inv)
            }
        }
        if (pa.vfk != 0u && !is_q) {
            let vb = pa.vbase + r * pa.kstride + kh * pa.hs
            if (tid < pa.hs) {
                krows[vb + tid] = v * inv
            }
            if (two) {
                krows[vb + t1] = v1 * inv
            }
        }
    }
}

struct GkDnConvArgs {
    npos : uint       //! the window's rows (the Metal groups' own counts ride the group table)
    cd : uint         //! conv width (channels)
    kd : uint         //! q/k region width: 2 kd channels L2-normed in ds blocks
    ds : uint         //! d_state, the norm block
    dconv : uint      //! conv taps + 1
    hist_mode : uint  //! 1 = per-channel history rows, else per-position
    taps_off : uint   //! the taps' and the history's float offsets in their planes
    hist_off : uint
    eps : float
    qscale : float
    row0 : uint       //! the dispatch's first row: a segmented window convolves a segment's rows over its own region's history
}

//! The deltanet causal conv + SiLU with the q/k L2 norm and q pre-scale folded in: a workgroup `PB` positions of 256 channels,
//! the taps a register ring of `RING` (the leading `RING - dconv` weigh zero), a head's squares an xor fold over its 32-lane
//! block and the blocks' partials off the slab; the rows, their history and the taps are the stamp's
[ |> template_struct_instance]
class template GkDnConv {
    @ssbo @binding = 0 @role = "read" src : array<float>    //! the projected rows, position-major
    @ssbo @binding = 1 @role = "write" outr : array<float>  //! the conv rows, normed
    @kargs pa : GkDnConvArgs
    @workgroup part : float[256]   //! the eight 32-lane blocks' per-position sums of squares
    @template_constant RING : int = 4
    @template_constant PB : int = 32

    def abstract seg_row0 : uint
    def abstract seg_npos : uint
    //! channel ch's history row hidx (0 the oldest of `taps`)
    def abstract hist_at(ch, taps, hidx : uint) : float
    def abstract tap_at(ch, j : uint) : float

    //! channel ch's input at segment row sp: a row, or the history row sp + taps below the segment (both reads in bounds, the
    //! select's arms evaluated alike)
    def xrow(sp : int; ch, taps : uint) : float => (sp >= 0 ? src[(seg_row0() + uint(max(sp, 0))) * pa.cd + ch]
        : hist_at(ch, taps, uint(max(sp + int(taps), 0))))

    def dn_conv_body(wgx, lid : uint) {   // nolint:STYLE037 - the tap ring, the per-position fold and the slab's head sums ride one block's registers
        let npos = seg_npos()
        let cblocks = (pa.cd + 255u) / 256u
        let p0 = (wgx / cblocks) * uint(PB)
        let ch = (wgx % cblocks) * 256u + lid
        let live = ch < pa.cd
        let cch = live ? ch : 0u
        let taps = pa.dconv - 1u
        let lead = uint(RING) - pa.dconv
        var wt, xr : float[RING]
        for [unroll_full] (j in range(RING)) {
            wt[j] = uint(j) >= lead ? tap_at(cch, uint(j) - lead) : 0.0
            let sp = int(p0) - (RING - 1) + j
            xr[j] = j < RING - 1 ? xrow(sp, cch, taps) : 0.0
        }
        let normed = live && ch < 2u * pa.kd
        var y, s : float[PB]
        for [unroll] (i in range(PB)) {
            let p = p0 + uint(i)
            let sp = int(p)
            xr[RING - 1] = p < npos ? xrow(sp, cch, taps) : 0.0
            var acc = 0.0
            for [unroll_full] (j in range(RING)) {
                acc += wt[j] * xr[j]
            }
            y[i] = (live && p < npos) ? silu_f32(acc) : 0.0
            for [unroll_full] (j in range(RING - 1)) {
                xr[j] = xr[j + 1]
            }
            //! the head's sum of squares over an aligned 32-lane block (ds lanes below 32): the fold's masks stay inside the block on any subgroup width
            var v = normed ? y[i] * y[i] : 0.0
            if (pa.ds > 16u) {
                v += gk_subgroup_xor(v, 16u)
            }
            v += gk_subgroup_xor(v, 8u)
            v += gk_subgroup_xor(v, 4u)
            v += gk_subgroup_xor(v, 2u)
            v += gk_subgroup_xor(v, 1u)
            s[i] = v
        }
        let blk = lid >> 5u   //! the lane block by the invocation id, never the subgroup: a 64-lane subgroup is two blocks
        if ((lid & 31u) == 0u) {
            for [unroll] (i in range(PB)) {
                part[blk * uint(PB) + uint(i)] = s[i]
            }
        }
        barrier()
        let blocks_per_head = pa.ds > 32u ? pa.ds / 32u : 1u
        let wb = (blk / blocks_per_head) * blocks_per_head
        let qs = ch < pa.kd ? pa.qscale : 1.0
        for [unroll] (i in range(PB)) {
            let p = p0 + uint(i)
            var tot = s[i]
            if (blocks_per_head > 1u) {
                tot = 0.0
                var ww = 0u
                while (ww < blocks_per_head) {
                    tot += part[(wb + ww) * uint(PB) + uint(i)]
                    ww++
                }
            }
            let sc = normed ? (1.0 / max(sqrt(tot), pa.eps)) * qs : 1.0
            if (live && p < npos) {
                outr[(seg_row0() + p) * pa.cd + ch] = y[i] * sc
            }
        }
    }
}

struct GkDnGateArgs {
    nvh : uint
    di : uint
    ds : uint
    o_off : uint       //! the raw o rows' base in orows (the Vulkan work surface's o region; the Metal window's first row)
    z_off : uint       //! the z rows' base in zrows
    wnorm_off : uint   //! the out-norm weight row's base in wn
    eps : float
}

//! The deltanet gated out-norm in place, one subgroup a (value head, row): the head's rms over ds, the out-norm weight row,
//! the z gate (`dn_zgate`, sigmoid under `ZSIG`); the lanes stride the head by the subgroup's width, the inverse norm's
//! spelling the stamp's (`RSQRT`)
[ |> template_struct_instance]
class template GkDnGate {
    @ssbo @binding = 0 @role = "read" zrows : array<float>        //! the z gate rows [rows x di]
    @ssbo @binding = 1 @role = "readwrite" orows : array<float>   //! the raw o rows [rows x di], gated in place
    @ssbo @binding = 2 @role = "weight" wn : array<float>         //! the shared out-norm weight row [ds]
    @kargs pa : GkDnGateArgs
    @template_constant ZSIG : bool = false
    @template_constant RSQRT : bool = false
    @template_constant EPL : int = 4   //!< the elements a lane holds: a head of up to EPL x the subgroup's width

    def dn_gate_head(h, p : uint) {
        let lane = gl_SubgroupInvocationID
        let ob = pa.o_off + p * pa.di + h * pa.ds
        let zb = pa.z_off + p * pa.di + h * pa.ds
        var v : float[EPL]
        var ss = 0.0
        for [unroll_full] (c in range(EPL)) {
            let e = lane + uint(c) * gl_SubgroupSize
            v[c] = e < pa.ds ? orows[ob + e] : 0.0
            ss += v[c] * v[c]
        }
        let inv = rms_inv(gk_subgroup_add(ss) / float(pa.ds) + pa.eps, RSQRT)
        for [unroll_full] (c in range(EPL)) {
            let e = lane + uint(c) * gl_SubgroupSize
            if (e < pa.ds) {
                orows[ob + e] = ((v[c] * inv) * wn[pa.wnorm_off + e]) * dn_zgate(zrows[zb + e], ZSIG)
            }
        }
    }
}

struct GkMoeCombArgs {
    n : uint         //! the row width
    k : uint         //! the slots a row
    r0 : uint        //! the slot-row window [r0, r1) a dispatch sums: r0 0 starts the sum, else it adds onto `acc_rows`
    r1 : uint
    wstride : uint   //! a row's stride in `slot_w` (0 reads as k; the Metal selection rows' 2k)
    woff : uint      //! the weights' base in a row (the Metal selection rows' k, past the picks)
    gstride : uint   //! the shared expert's gate logit: a row's stride in `gate` and its index there (`SH` stamps)
    goff : uint
    eps : float      //! the gemma-4 step's (`GkMoeCombG4`): the norms' epsilon
    ascale : float   //! ... the layer's output scale over the updated residual
    presummed : uint //! ... the routed rows arrive summed under their weights, one row a position (the down GEMV folded them)
    rpost_off : uint //! ... the norms plane's rows: the routed post norm, the dense post norm, the post-FFN norm, the next norm
    dpost_off : uint
    spost_off : uint
    nwoff : uint
}

//! The routed experts' combine, row p's element i its k slots' down rows under their weights in slot order (the CPU axpy chain's);
//! `WINDOW` skips a slot outside the dispatch's window or at zero weight, `GATHERED` reads the slot rows at `slot_idx`, `SH` adds the
//! shared expert's row under its gate logit's sigmoid (`ARCHITECTURE_GPU_SHARED_KERNELS.md`)
[ |> template_struct_instance]
class template GkMoeCombine : GkQ8BlockStore {
    @ssbo @binding = 0 @role = "readwrite" acc_rows : array<float>                     //! [rows x n] the combined rows (in and out)
    @ssbo @binding = 1 @role = "read" @template_gate = GATHERED slot_idx : array<uint>  //! row -> its k slot rows
    @ssbo @binding = 2 @role = "read" slot_w : array<float>                            //! ... and their routing weights
    @ssbo @binding = 3 @role = "read" down_rows : array<float>                         //! the slots' down-GEMM rows
    @ssbo @binding = 4 @role = "read" @template_gate = SH sh_rows : array<float>        //! [rows x n] the shared expert's rows
    @ssbo @binding = 5 @role = "read" @template_gate = SH gate : array<float>           //! its raw gate logits
    @kargs pa : GkMoeCombArgs
    @template_constant GATHERED : bool = true
    @template_constant SH : bool = false
    @template_constant WINDOW : bool = true   //!< the [r0, r1) window, the accumulate and the zero-weight skip (the Vulkan chunked chain); off, every slot sums from zero
    @template_constant SLOT_X4 : bool = true  //!< off the window, four slots' loads in flight a step (the sum's order the same)

    def slot_row(p, j : uint) : uint {
        static_if (GATHERED) {
            return slot_idx[p * pa.k + j]
        } else {
            return p * pa.k + j
        }
    }

    //! the sum's start at element `e`: zero, or under `WINDOW` past the first chunk the accumulated row
    def acc_start(e : uint) : float {
        static_if (WINDOW) {
            return pa.r0 == 0u ? 0.0 : acc_rows[e]
        } else {
            return 0.0
        }
    }

    //! row p's k slots at element i added onto `acc` under their weights, in slot order
    [spirv_inline]
    def slot_sum_into(p, i : uint; var acc : float&) {
        let n = pa.n
        let k = pa.k
        let wb = p * (pa.wstride == 0u ? k : pa.wstride) + pa.woff
        var j = 0u
        static_if (!WINDOW && SLOT_X4) {
            while (j + 4u <= k) {
                let d0 = down_rows[slot_row(p, j) * n + i]
                let d1 = down_rows[slot_row(p, j + 1u) * n + i]
                let d2 = down_rows[slot_row(p, j + 2u) * n + i]
                let d3 = down_rows[slot_row(p, j + 3u) * n + i]
                acc += slot_w[wb + j] * d0
                acc += slot_w[wb + j + 1u] * d1
                acc += slot_w[wb + j + 2u] * d2
                acc += slot_w[wb + j + 3u] * d3
                j += 4u
            }
        }
        while (j < k) {
            let jb = slot_row(p, j)
            let wj = slot_w[wb + j]
            static_if (WINDOW) {
                if (jb >= pa.r0 && jb < pa.r1 && wj != 0.0) {
                    acc += wj * down_rows[(jb - pa.r0) * n + i]
                }
            } else {
                acc += wj * down_rows[jb * n + i]
            }
            j++
        }
    }

    def comb_elem(p, i : uint) {
        let n = pa.n
        var acc = acc_start(p * n + i)
        slot_sum_into(p, i, acc)
        static_if (SH) {
            acc += sig_gate(sh_rows[p * n + i], gate[p * pa.gstride + pa.goff])
        }
        acc_rows[p * n + i] = acc
    }
}

//! The gemma-4 MoE layer's residual step, a workgroup a row (the CPU `gemma4_moe_ffn`'s tail): the routed sum and the dense row
//! each under its post norm, their sum under the post-FFN norm, x = (x + that) . ascale, then x's next norm out in the stamp's
//! form - f32 (`HX`: its f16 twin beside), f16, Q8_0 blocks, or none; the stamp supplies the row stash and its lanes (`TG`)
[ |> template_struct_instance]
class template GkMoeCombG4 : GkMoeCombine {
    @ssbo @binding = 6 @role = "read" dense : array<float>      //! [rows x n] the dense branch's down rows
    @ssbo @binding = 7 @role = "weight" wn : array<float>       //! the norms plane
    @ssbo @binding = 8 @role = "write" @template_gate = OUT_F32 yo : array<float>
    @ssbo @binding = 8 @role = "write" @template_gate = OUT_F16 outh : array<float16>
    @ssbo @binding = 8 @role = "write" @template_gate = OUT_Q8 outq : array<int8>
    @ssbo @binding = 9 @role = "write" @template_gate = OUT_Q8 outs : array<float>
    @ssbo @binding = 10 @role = "write" @template_gate = HX yh : array<float16>
    @template_constant TG : uint = 256u          //!< the workgroup's lanes, the row loops' stride
    @template_constant OUT_F32 : bool = false
    @template_constant OUT_F16 : bool = false
    @template_constant HX : bool = false
    @template_constant OUT_X4 : bool = false     //!< the next norm's stores four elements in flight a lane
    override WINDOW = false

    def abstract stash(k : uint; v : float) : void   //! column `k` of the row kept for the lane's own re-read
    def abstract stashed(k : uint) : float

    //! the routed rows of row `p` at element `k`
    def routed_into(p, k : uint; var r : float&) {
        if (pa.presummed != 0u) {
            r = down_rows[p * pa.n + k]
        } else {
            r = 0.0
            slot_sum_into(p, k, r)
        }
    }

    def override normed_at(e : uint; mean, inv : float) : float => wn[pa.nwoff + e] * (stashed(e) * inv)

    //! row `r`'s step: four reductions over the stash (slots 0, 1, 0, then the next norm's 1 - each reduce's barrier sits every
    //! thread past the readers of the slot the next one writes)
    [arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-gemma4-moe-block")]
    def comb_g4_row(r : uint) {
        let n = pa.n
        let base = r * n
        let tid = gl_LocalInvocationID.x
        var sr = 0.0
        var k = tid
        while (k < n) {
            var rv = 0.0
            routed_into(r, k, rv)
            stash(k, rv)
            sr += rv * rv
            k += TG
        }
        var inv_r = 0.0
        wg_rms_inv_into(sr, n, pa.eps, 0u, inv_r)
        var sd = 0.0
        k = tid
        while (k < n) {
            let a = dense[base + k]
            sd += a * a
            k += TG
        }
        var inv_d = 0.0
        wg_rms_inv_into(sd, n, pa.eps, 1u, inv_d)
        var sc = 0.0
        k = tid
        while (k < n) {
            let c = wn[pa.rpost_off + k] * (stashed(k) * inv_r) + wn[pa.dpost_off + k] * (dense[base + k] * inv_d)
            stash(k, c)
            sc += c * c
            k += TG
        }
        var inv_c = 0.0
        wg_rms_inv_into(sc, n, pa.eps, 0u, inv_c)
        var ss = 0.0
        k = tid
        while (k < n) {
            let vv = (acc_rows[base + k] + wn[pa.spost_off + k] * (stashed(k) * inv_c)) * pa.ascale
            acc_rows[base + k] = vv
            stash(k, vv)
            ss += vv * vv
            k += TG
        }
        store_next(r, ss)
    }

    //! row `r`'s next norm out of the residual's sum of squares `ss`, in the stamp's form (Q8_0 blocks, or the element stores)
    def store_next(r : uint; ss : float) {
        let n = pa.n
        static_if (OUT_F32 || OUT_F16 || OUT_Q8) {
            var inv = 0.0
            wg_rms_inv_into(ss, n, pa.eps, 1u, inv)
            static_if (OUT_Q8) {
                blk_pass(0u, n / KQ_B32_ELEMS_U, r * (n / KQ_B32_ELEMS_U), 0.0, inv)
            } else {
                let base = r * n
                var k = gl_LocalInvocationID.x
                static_if (OUT_X4) {
                    let last = n - 1u
                    while (k < n) {
                        let k1 = min(k + TG, last)
                        let k2 = min(k + 2u * TG, last)
                        let k3 = min(k + 3u * TG, last)
                        let n0 = normed_at(k, 0.0, inv)
                        let n1 = normed_at(k1, 0.0, inv)
                        let n2 = normed_at(k2, 0.0, inv)
                        let n3 = normed_at(k3, 0.0, inv)
                        put_norm(base + k, n0)
                        if (k + TG < n) {
                            put_norm(base + k1, n1)
                        }
                        if (k + 2u * TG < n) {
                            put_norm(base + k2, n2)
                        }
                        if (k + 3u * TG < n) {
                            put_norm(base + k3, n3)
                        }
                        k += 4u * TG
                    }
                } else {
                    while (k < n) {
                        put_norm(base + k, normed_at(k, 0.0, inv))
                        k += TG
                    }
                }
            }
        }
    }

    //! element `i` of the next norm stored in the stamp's out forms
    def put_norm(i : uint; v : float) {
        static_if (OUT_F32) {
            yo[i] = v
        }
        static_if (OUT_F16) {
            outh[i] = float16(v)
        }
        static_if (HX) {
            yh[i] = float16(v)
        }
    }
}
