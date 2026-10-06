options gen2
//! The kernels both GPU homes run, written once: a `class template` here carries a kernel's buffers, its
//! argument struct and its body, and each home stamps it under its own dispatch name.
module dasllama_gpu_kernels_common shared public
require dasllama/dasllama_lint public
require daslib/typemacro_boost

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
    @ssbo @binding = 1 @role = "read" wn : array<float>
    @ssbo @binding = 2 @role = "write" spec : array<float>
    @kargs pa : GkStftArgs

    def stft_body {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.frames * pa.bins) {
            let f = gid / pa.bins
            let b = gid % pa.bins
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
    @ssbo @binding = 1 @role = "read" wn : array<float>
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
