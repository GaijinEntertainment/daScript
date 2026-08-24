options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math
require dasllama/dasllama_plane public   // PlaneF/PlaneU16 — plane forms of the blob helpers
require dasllama/dasllama_par
require dasllama/dasllama_gguf   // read_clamp reads sidecar scalar tensors

// The family-neutral encoder-tower home: the row math, activations, GEMM wrappers, clamp
// sidecars, patch geometry, blocked bidirectional attention, and the encode-stage prof rail
// that every audio AND vision tower composes (ARCHITECTURE.md §1.7). One home: a family
// file that re-implements a shape found here is a defect; nothing here names a family type.

// erfc rational approximation (Numerical Recipes 6.2), |fractional error| < 1.2e-7 —
// float-level agreement with the reference erff. Double internally.
def private erf_nr(x : double) : double {
    let z = abs(x)
    let t = 1.0lf / (1.0lf + 0.5lf * z)
    let erfc = t * exp(-z * z - 1.26551223lf + t * (1.00002368lf + t * (0.37409196lf + t * (0.09678418lf +
        t * (-0.18628806lf + t * (0.27886807lf + t * (-1.13520398lf + t * (1.48851587lf +
        t * (-0.82215223lf + t * 0.17087277lf)))))))))
    return x >= 0.0lf ? 1.0lf - erfc : erfc - 1.0lf
}

//! gelu_erf(x) = 0.5·x·(1 + erf(x/√2)) — the EXACT erf form,
//! not the tanh approximation ``gelu`` uses. Whisper's conv frontend and encoder FFN both
//! use this flavor. In place over the first ``size`` elements.
def gelu_erf(var x : float?; size : int64) {
    let inv_sqrt2 = 0.7071067811865476lf
    unsafe {
        for (i in range64(size)) {
            let v = double(x[i])
            x[i] = float(0.5lf * v * (1.0lf + erf_nr(v * inv_sqrt2)))
        }
    }
}

def gelu_erf(var x : array<float> | #; size : int64) {
    if (size <= 0l) return
    gelu_erf(unsafe(addr(x[0])), size)
}

//! The fp16-LUT gelu flavor: tanh approximation evaluated through an
//! fp16 lookup table — input and output both round-trip through binary16, with ±10 clamps
//! applied BEFORE the conversion. Bit-exact vs the reference CPU path. In place over ``size``.
def gelu_tanh_lut(var x : float?; size : int64) {
    unsafe {
        for (i in range64(size)) {
            let v = x[i]
            if (v <= -10.0) {
                x[i] = 0.0
            } elif (v < 10.0) {  // v >= 10 passes through unchanged
                let xr = f16_to_f32(f32_to_f16(v))
                let g = 0.5 * xr * (1.0 + tanh(0.79788456080286535587989211986876 * xr * (1.0 + 0.044715 * xr * xr)))
                x[i] = f16_to_f32(f32_to_f16(g))
            }
        }
    }
}

def gelu_tanh_lut(var x : array<float> | #; size : int64) {
    if (size <= 0l) return
    gelu_tanh_lut(unsafe(addr(x[0])), size)
}

// The f16 lookup table for gelu_tanh_lut's middle branch: pure f16-domain (idx = f32_to_f16(v)),
// bit-exact vs the computed form by construction; replaces a tanh + two f16 round-trips per element.
// Built once, on first batch call, main thread — before any dispatch captures the pointer.
var private g_gelu_lut : array<float>

[cold_path]
def private gelu_lut_ensure() {
    if (!empty(g_gelu_lut)) {
        return
    }
    g_gelu_lut |> resize(65536)
    for (h in range(65536)) {
        let xr = f16_to_f32(uint(h))
        // every finite pattern gets a value: fp32 inputs just under 10.0 round up to f16's 10.0 pattern, so boundary entries must be built too
        if ((unsafe(reinterpret<uint>(xr)) & 0x7F800000u) != 0x7F800000u) {   // finite (not inf/nan)
            let g = 0.5 * xr * (1.0 + tanh(0.79788456080286535587989211986876 * xr * (1.0 + 0.044715 * xr * xr)))
            g_gelu_lut[h] = f16_to_f32(f32_to_f16(g))
        }
    }
}

//! gelu_tanh_lut over a token-major image (npos rows × d): the middle branch reads the
//! precomputed f16 table, rows split across lanes. Elementwise ⇒ bit-identical to the
//! whole-image computed form at any lane count.
def gelu_tanh_lut_batch(var x : array<float> | #; d, npos : int64) {
    gelu_lut_ensure()
    unsafe {
        var xp = addr(x[0])
        let lp = addr<float const?>(g_gelu_lut[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 8l, 0)) $(rb, re) {
            unsafe {
                for (i in range64(int64(rb) * d, int64(re) * d)) {
                    let v = xp[i]
                    if (v <= -10.0) {
                        xp[i] = 0.0
                    } elif (v < 10.0) {   // v >= 10 passes through unchanged
                        xp[i] = lp[int(f32_to_f16(v))]
                    }
                }
            }
        }
    }
}

//! gelu_erf over a token-major image (npos rows × d), rows split across lanes. Elementwise
//! ⇒ bit-identical to the whole-image call at any lane count.
def gelu_erf_batch(var x : array<float> | #; d, npos : int64) {
    unsafe {
        var xp = addr(x[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 16l, 0)) $(rb, re) {
            gelu_erf(unsafe(xp + int64(rb) * d), int64(re - rb) * d)
        }
    }
}

//! Per-row swiglu with swapped halves: out[r][i] = silu(x[r][half+i]) · x[r][i]
//! (silu on the SECOND half, passthrough on the first - the swapped convention).
def swiglu_swapped_rows(var o : array<float>; x : array<float>; half, rows : int64) {
    for (r in range64(rows)) {
        for (i in range64(half)) {
            o[r * half + i] = x[r * 2l * half + half + i]
        }
    }
    silu(o, rows * half)
    for (r in range64(rows)) {
        for (i in range64(half)) {
            o[r * half + i] *= x[r * 2l * half + i]
        }
    }
}

//! LayerNorm row: o = (x − mean)/√(var + eps)·w + b, mean/var double-accumulated over the
//! row (the decoder only has RMS so this is new). Population var.
def layernorm(var o : float?; x, w, b : float const?; size : int64; eps : float) {
    unsafe {
        var mean = 0.0lf
        for (i in range64(size)) {
            mean += double(x[i])
        }
        mean /= double(size)
        var variance = 0.0lf
        for (i in range64(size)) {
            let dv = double(x[i]) - mean
            variance += dv * dv
        }
        variance /= double(size)
        let scale = 1.0 / sqrt(float(variance) + eps)
        let fmean = float(mean)
        for (i in range64(size)) {
            o[i] = (x[i] - fmean) * scale * w[i] + b[i]
        }
    }
}

//! Threaded per-position layernorm over token-major rows (rms_batch's shape, LN math),
//! weight/bias read from ``blob`` at the given offsets.
def private layernorm_batch_at(var out : array<float>; src : array<float>; wp, bp : float const?; dim, npos : int64; eps : float) {
    unsafe {
        var op = addr(out[0])
        let ip = addr<float const?>(src[0])
        maybe_parallel_for(0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * dim
                    layernorm(op + off, ip + off, wp, bp, dim, eps)
                }
            }
        }
    }
}

def layernorm_batch(var out : array<float>; src : array<float>; blob : array<float>; w_off, b_off, dim, npos : int64; eps : float) {
    unsafe {
        layernorm_batch_at(out, src, addr<float const?>(blob[w_off]), addr<float const?>(blob[b_off]), dim, npos, eps)
    }
}

//! Borrowed-plane form — the weight/bias rows live in a prepared image.
def layernorm_batch(var out : array<float>; src : array<float>; blob : PlaneF; w_off, b_off, dim, npos : int64; eps : float) {
    layernorm_batch_at(out, src, plane_at(blob, w_off), plane_at(blob, b_off), dim, npos, eps)
}

//! y[p] += blob[b_off .. b_off+d) for every row (projection bias). float4 bulk + scalar
//! tail, threaded over rows (gemv lane cap — pure streaming) — elementwise adds in the
//! same per-element order, bit-exact vs the scalar loop at any lane count.
def add_bias_rows(var y : array<float>; blob : array<float>; b_off, d, npos : int64) {
    add_bias_rows_at(y, unsafe(addr<float const?>(blob[b_off])), d, npos)
}

//! Borrowed-plane form — the bias row lives in a prepared image.
def add_bias_rows(var y : array<float>; blob : PlaneF; b_off, d, npos : int64) {
    add_bias_rows_at(y, plane_at(blob, b_off), d, npos)
}

def private add_bias_rows_at(var y : array<float>; bp : float const?; d, npos : int64) {
    if (npos <= 0l || d <= 0l) return
    let d4 = d - d % 4l
    unsafe {
        var yp = addr(y[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d, get_gemv_lane_cap())) $(rb, re) {
            unsafe {
                for (p in range64(int64(rb), int64(re))) {
                    let row = p * d
                    var i = 0l
                    while (i < d4) {
                        *reinterpret<float4?>(yp + row + i) = (*reinterpret<float4 const?>(yp + row + i)) + (*reinterpret<float4 const?>(bp + i))
                        i += 4l
                    }
                    while (i < d) {
                        yp[row + i] += bp[i]
                        i++
                    }
                }
            }
        }
    }
}

//! x += r, elementwise (residual). float4 bulk in 4-aligned threaded chunks + scalar tail
//! (gemv lane cap — pure streaming) — bit-exact vs the scalar loop at any lane count.
def add_inplace_rows(var x : array<float>; r : array<float>; n : int64) {
    if (n <= 0l) return
    let nblk = n / 4l
    unsafe {
        var xp = addr(x[0])
        let rp = addr<float const?>(r[0])
        maybe_parallel_for(0, int(nblk), lanes_for_work(n, get_gemv_lane_cap())) $(bb, be) {
            unsafe {
                for (b in range64(int64(bb), int64(be))) {
                    let i = b * 4l
                    *reinterpret<float4?>(xp + i) = (*reinterpret<float4 const?>(xp + i)) + (*reinterpret<float4 const?>(rp + i))
                }
            }
        }
        for (i in range64(nblk * 4l, n)) {
            xp[i] += rp[i]
        }
    }
}

//! fp32 matmul against a blob region: Y [npos×d] = W[d×n]·X[npos×n] (threaded matmul_batch,
//! blob+offset form — no array_view).
def mm_blob_b(var y : array<float>; blob : array<float>; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_batch(y, blob, woff, x, n, d, npos)
}

//! Borrowed-plane form — the weights live in a prepared image.
def mm_blob_b(var y : array<float>; blob : PlaneF; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_batch(y, blob, woff, x, n, d, npos)
}

//! Both sides borrowed — the activation is itself a baked table.
def mm_blob_b(var y : array<float>; blob : PlaneF; woff : int64; x : PlaneF; n, d, npos : int64) {
    matmul_batch(y, blob, woff, x, n, d, npos)
}

//! `mm_blob_b`'s BF16-weight twin — the tower form of `matmul_bf16_batch`, weights borrowed from
//! a prepared image. Bit-for-bit the fp32 expand at half the weight read, so a tower can hold
//! its GEMMs in bf16 with no parity move.
def mm_bf16_b(var y : array<float>; blob : PlaneU16; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_bf16_batch(y, blob, woff, x, n, d, npos)
}

//! One GEMM on whichever plane a per-tensor flag picked at load: the bf16 halfword plane when
//! ``bf16``, the fp32 plane otherwise — the same arithmetic both ways (dot_bf16's widen is an
//! exact high-half shift), so a mixed-type mmproj serves each tensor as the file holds it.
def mm_plane_b(var y : array<float>; bf16 : bool; wblob : PlaneU16; blob : PlaneF; off : int64;
               x : array<float>; n, d, npos : int64) {
    if (bf16) {
        mm_bf16_b(y, wblob, off, x, n, d, npos)
    } else {
        mm_blob_b(y, blob, off, x, n, d, npos)
    }
}

//! Read one GEMM weight into the plane its per-tensor type picked: the bf16 halfword plane when
//! ``bf16``, the fp32 plane otherwise (a shipped mmproj mixes the two per tensor).
def tower_read_gemm(m : GGUFMeta; b : array<uint8> | #; name : string; var wblob : array<uint16>;
                    var blob : array<float>; bf16 : bool; off, n_elem : int64) {
    if (bf16) {
        gguf_transcode_bf16(m, b, name, wblob, off, n_elem)
    } else {
        gguf_read_tensor_f32(m, b, name, blob, off, n_elem)
    }
}

//! Read one GEMM weight transcoded to Q8_0 — the q8 serving lane's read: the file's tensor
//! (any f32-widenable type) widens into ``wscratch``, then quantizes into qblob/qscales at the
//! 32-aligned region (``wo/32`` indexes the scales). ``family`` names the panic.
def tower_read_gemm_q8(m : GGUFMeta; b : array<uint8> | #; name : string; var qblob : array<int8>;
                       var qscales : array<float>; @scratch var wscratch : array<float>;
                       wo, n_elem : int64; family : string) {
    if (n_elem % 32l != 0l || wo % 32l != 0l) {
        panic("{family}: GEMM tensor '{name}' ({n_elem} elements at {wo}) is not 32-aligned")
    }
    if (long_length(wscratch) < n_elem) {   // inline: PERF026 can't see @scratch through reserve_resize
        wscratch |> reserve(n_elem)
        wscratch |> resize(n_elem)
    }
    gguf_read_tensor_f32(m, b, name, wscratch, 0l, n_elem)
    quantize_q8_0_into(wscratch, n_elem, qblob, qscales, wo, wo / 32l)
}

//! Stage a [file_rows × width] GEMM into a q8 region padded to [pad_rows × width] with zero
//! rows appended — their quantized outputs are exactly zero, so a non-32-aligned FFN's up/gate
//! half can emit the padded width its down partner consumes.
def tower_stage_q8_zero_rows(m : GGUFMeta; b : array<uint8> | #; name : string; var qblob : array<int8>;
                             var qscales : array<float>; @scratch var wscratch : array<float>;
                             wo, file_rows, pad_rows, width : int64; family : string) {
    let file_n = file_rows * width
    let served = pad_rows * width
    // the row width itself must quantize: repack and the row requant both take n % 32 == 0
    if (width % 32l != 0l || served % 32l != 0l || wo % 32l != 0l) {
        panic("{family}: GEMM tensor '{name}' ({served} served elements, width {width}, at {wo}) is not 32-aligned")
    }
    if (long_length(wscratch) < served) {
        wscratch |> reserve(served)
        wscratch |> resize(served)
    }
    gguf_read_tensor_f32(m, b, name, wscratch, 0l, file_n)
    tower_zero_span(wscratch, file_n, served - file_n)
    quantize_q8_0_into(wscratch, served, qblob, qscales, wo, wo / 32l)
}

//! Stage a [rows × ff] GEMM into a q8 region with each row zero-padded to [rows × fp] — the
//! down half: the pad columns multiply the up pad's zero outputs, so the padded dot is exact.
def tower_stage_q8_pad_cols(m : GGUFMeta; b : array<uint8> | #; name : string; var qblob : array<int8>;
                            var qscales : array<float>; @scratch var wscratch : array<float>;
                            @scratch var rowscratch : array<float>; wo, rows, ff, fp : int64; family : string) {
    let served = rows * fp
    // the padded row width itself must quantize: repack and the row requant both take n % 32 == 0
    if (fp % 32l != 0l || served % 32l != 0l || wo % 32l != 0l) {
        panic("{family}: GEMM tensor '{name}' ({served} served elements, padded width {fp}, at {wo}) is not 32-aligned")
    }
    if (long_length(rowscratch) < rows * ff) {
        rowscratch |> reserve(rows * ff)
        rowscratch |> resize(rows * ff)
    }
    gguf_read_tensor_f32(m, b, name, rowscratch, 0l, rows * ff)
    if (long_length(wscratch) < served) {
        wscratch |> reserve(served)
        wscratch |> resize(served)
    }
    unsafe {
        var dp = addr(wscratch[0])
        let sp = addr(rowscratch[0])
        for (r in range64(rows)) {
            memcpy(dp + r * fp, sp + r * ff, ff * 4l)
            for (i in range64(ff, fp)) {
                dp[r * fp + i] = 0.0
            }
        }
    }
    quantize_q8_0_into(wscratch, served, qblob, qscales, wo, wo / 32l)
}

//! Repack every block-GEMM region of a staged q8 layout for the active backend's q8q8 kernels —
//! per-row alignment (each region's input width, ``widths``/``sizes`` in the family's GEMM
//! order) keeps blocks from straddling rows; no repack backend selected = the flat image serves.
def tower_repack_q8_blocks(var qblob : array<int8>; var qscales : array<float>;
                           layer_w, layer_w_stride, n_layer : int64;
                           widths : array<int64>; sizes : array<int64>) {
    if (!select_matmul_backend_for_load_()) {
        return
    }
    for (l in range64(n_layer)) {
        var wo = layer_w + l * layer_w_stride
        for (w, sz in widths, sizes) {
            repack_q8q8_weight(qblob, qscales, wo, w, sz / w)
            wo += sz
        }
    }
}

//! The f32 twin of ``tower_stage_q8_zero_rows``: stage a ``[file_rows × width]`` GEMM into its
//! padded region — the file's rows land first, then ``pad_rows − file_rows`` zero weight rows
//! (the pad outputs are exactly zero).
def tower_stage_f32_zero_rows(m : GGUFMeta; b : array<uint8> | #; name : string; var blob : array<float>;
                              wo, file_rows, pad_rows, width : int64) {
    gguf_read_tensor_f32(m, b, name, blob, wo, file_rows * width)
    tower_zero_span(blob, wo + file_rows * width, (pad_rows - file_rows) * width)
}

//! The f32 twin of ``tower_stage_q8_pad_cols``: stage a ``[rows × ff]`` GEMM into its padded
//! ``[rows × fp]`` f32 region — each file row lands first, then ``fp − ff`` zero tail columns
//! (the pads multiply the up GEMM's zero pad outputs, bit-identical).
def tower_stage_f32_pad_cols(m : GGUFMeta; b : array<uint8> | #; name : string; var blob : array<float>;
                             @scratch var rowscratch : array<float>; wo, rows, ff, fp : int64) {
    if (long_length(rowscratch) < rows * ff) {
        rowscratch |> reserve(rows * ff)
        rowscratch |> resize(rows * ff)
    }
    gguf_read_tensor_f32(m, b, name, rowscratch, 0l, rows * ff)
    unsafe {
        var dp = addr(blob[wo])
        let sp = addr(rowscratch[0])
        for (r in range64(rows)) {
            memcpy(dp + r * fp, sp + r * ff, ff * 4l)
            for (i in range64(ff, fp)) {
                dp[r * fp + i] = 0.0
            }
        }
    }
}

//! Zero ``n`` floats at ``at`` — pad tails of staged planes.
def tower_zero_span(var a : array<float>; at, n : int64) {
    if (n <= 0l) {
        return
    }
    unsafe {
        var p = addr(a[at])
        for (i in range64(n)) {
            p[i] = 0.0
        }
    }
}

//! Read a temporal patch-conv PAIR folded for still images: conv0(img) + conv1(img) = (W0+W1)·img,
//! so ``base`` lands at ``off`` and ``base.1`` accumulates into the same plane region.
def tower_read_conv_pair_folded(m : GGUFMeta; b : array<uint8> | #; base : string;
                                var blob : array<float>; off, n_elem : int64) {
    gguf_read_tensor_f32(m, b, base, blob, off, n_elem)
    var w1 : array<float>
    w1 |> reserve(n_elem)
    w1 |> resize(n_elem)
    gguf_read_tensor_f32(m, b, "{base}.1", w1, 0l, n_elem)
    for (i in range64(n_elem)) {
        blob[off + i] += w1[i]
    }
    delete w1
}

//! Per-op activation-quant calibration range: clamp(x, imin, imax) before
//! the matmul, clamp the result to (omin, omax) after. ``active`` false = no sidecar scalars on
//! disk, which means ±FLT_MAX — an identity clamp, so callers may skip the pass.
[safe_when_uninitialized]
struct Clamp {
    imin : float = -3.4028235e38
    imax : float = 3.4028235e38
    omin : float = -3.4028235e38
    omax : float = 3.4028235e38
    active : bool = false
}

def private read_scalar_tensor(m : GGUFMeta; bytes : array<uint8> | #; name : string) : float {
    var tmp : array<float>
    tmp |> resize(1)
    gguf_read_tensor_f32(m, bytes, name, tmp, 0l, 1l)
    return tmp[0]
}

//! The four sidecar scalars beside ``<base>.weight`` (``.input_min/.input_max/.output_min/
//! .output_max``). A file without them yields the inactive (identity) clamp.
def read_clamp(m : GGUFMeta; bytes : array<uint8> | #; base : string) : Clamp {
    var c = Clamp()
    if (gguf_find_tensor(m, "{base}.input_max") >= 0) {
        c.imax = read_scalar_tensor(m, bytes, "{base}.input_max")
        c.imin = read_scalar_tensor(m, bytes, "{base}.input_min")
        c.omax = read_scalar_tensor(m, bytes, "{base}.output_max")
        c.omin = read_scalar_tensor(m, bytes, "{base}.output_min")
        c.active = true
    }
    return c
}

//! dst[i] = clamp(src[i], lo, hi) over the first ``n`` elements, rows split across lanes.
//! ``dst`` may be ``src`` (in place). Elementwise ⇒ bit-identical at any lane count.
def clamp_rows(var dst : array<float>; src : array<float>; n : int64; lo, hi : float) {
    if (n <= 0l) return
    unsafe {
        var dp = addr(dst[0])
        let sp = addr<float const?>(src[0])
        let nblk = int((n + 1023l) / 1024l)
        maybe_parallel_for(0, nblk, lanes_for_work(n, get_gemv_lane_cap())) $(bb, be) {
            unsafe {
                for (i in range64(int64(bb) * 1024l, min(int64(be) * 1024l, n))) {
                    dp[i] = clamp(sp[i], lo, hi)
                }
            }
        }
    }
}

//! Threaded per-row RMSNorm against a weight row in a prepared image: out[r] = rmsnorm(src[r])·w
//! (``rmsnorm``'s (x·scale)·w order — normalize, then multiply). ``out`` may be ``src``. A
//! per-head norm is this with ``d`` = head width and ``npos`` = tokens × heads.
def rms_rows(var out : array<float>; src : array<float>; w : PlaneF; woff, d, npos : int64; eps : float) {
    if (npos <= 0l || d <= 0l) return
    unsafe {
        var op = addr(out[0])
        let sp = addr<float const?>(src[0])
        let wp = plane_at(w, woff)
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 2l, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    rmsnorm(op + r * d, sp + r * d, wp, d, eps)
                }
            }
        }
    }
}

//! Weightless RMS norm per row, in place: x ⊙ 1/√(mean(x²) + eps), the mean accumulated in
//! double (gemma4uv's pre-projection norm; gemma4v's V-norm with ``d`` = head width).
def rms_rows_weightless(var x : array<float>; d, npos : int64; eps : float) {
    if (npos <= 0l || d <= 0l) return
    unsafe {
        var xp = addr(x[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 2l, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    var row = xp + r * d
                    var s = 0.0lf
                    for (i in range64(0l, d)) {
                        s += double(row[i]) * double(row[i])
                    }
                    let scale = float(1.0lf / sqrt(s / double(d) + double(eps)))
                    for (i in range64(0l, d)) {
                        row[i] *= scale
                    }
                }
            }
        }
    }
}

// The f16 table for geglu_quick_lut_rows, matching the reference CPU LUT:
// idx = f32_to_f16(x), value = f16(x·σ(1.702x)) widened. Built once, main thread.
var private g_gelu_quick_lut : array<float>

[cold_path]
def private gelu_quick_lut_ensure() {
    if (!empty(g_gelu_quick_lut)) {
        return
    }
    g_gelu_quick_lut |> resize(65536)
    for (h in range(65536)) {
        let xr = f16_to_f32(uint(h))
        if ((unsafe(reinterpret<uint>(xr)) & 0x7F800000u) != 0x7F800000u) {   // finite (not inf/nan)
            let g = xr * (1.0 / (1.0 + exp(-1.702 * xr)))
            g_gelu_quick_lut[h] = f16_to_f32(f32_to_f16(g))
        }
    }
}

//! GEGLU-quick over token-major rows: h = gelu_quick(gate) ⊙ up, gelu_quick evaluated through
//! the f16 table — input and output both round-trip through binary16, matching the CPU
//! oracle bit-for-bit rather than the exact x·σ(1.702x). Elementwise ⇒ lane-count invariant.
def geglu_quick_lut_rows(var h : array<float>; gate, up : array<float>; ff, npos : int64) {
    if (npos <= 0l || ff <= 0l) return
    gelu_quick_lut_ensure()
    unsafe {
        var hp = addr(h[0])
        let gp = addr<float const?>(gate[0])
        let up_ = addr<float const?>(up[0])
        let lp = addr<float const?>(g_gelu_quick_lut[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * ff * 4l, 0)) $(rb, re) {
            unsafe {
                for (i in range64(int64(rb) * ff, int64(re) * ff)) {
                    hp[i] = lp[int(f32_to_f16(gp[i]))] * up_[i]
                }
            }
        }
    }
}

//! im2col over an interleaved-RGB [h × w × 3] canvas into token-major rows in [C][KH][KW]
//! order (row index c·p² + ky·p + kx), one row per non-overlapping p×p patch walking the canvas
//! row-major (pos_x = i % n_cols, pos_y = i / n_cols). Returns npos; ``x0`` grows to npos × 3p².
def im2col_rgb_patches(planes : array<float>; w, h, p : int64; @scratch var x0 : array<float>) : int64 {
    let n_cols = w / p
    let npos = n_cols * (h / p)
    let psq = p * p
    let row_dim = 3l * psq
    if (long_length(x0) < npos * row_dim) {
        x0 |> reserve(npos * row_dim)
        x0 |> resize(npos * row_dim)
    }
    unsafe {
        let src = addr<float const?>(planes[0])
        var dst = addr(x0[0])
        for (pi in range64(0l, npos)) {
            let py = (pi / n_cols) * p
            let px = (pi % n_cols) * p
            var row = dst + pi * row_dim
            for (c in range64(0l, 3l)) {
                for (ky in range64(0l, p)) {
                    let sline = src + ((py + ky) * w + px) * 3l + c
                    var dline = row + c * psq + ky * p
                    for (kx in range64(0l, p)) {
                        dline[kx] = sline[kx * 3l]
                    }
                }
            }
        }
    }
    return npos
}

//! Two-axis NEOX RoPE over token-major rows [npos × n_head·hs], in place: each head's first hs/2
//! dims rotate by the row's x-position table entry, the second hs/2 by its y entry (two half-width
//! rotations concatenated). Tables are ``build_rope_tabs_rows`` outputs at head_size = hs/2.
def rope_neox_2d_rows(var v : array<float>; npos, n_head, hs : int64;
                      cos_x, sin_x, cos_y, sin_y : array<float>) {
    if (npos <= 0l) return
    let d = n_head * hs
    let half = hs / 2l          // one axis's rotary width
    let pairs = half / 2l       // its rotated pairs = one table row (cos or sin)
    unsafe {
        var vp = addr(v[0])
        let cx = addr<float const?>(cos_x[0])
        let sx = addr<float const?>(sin_x[0])
        let cy = addr<float const?>(cos_y[0])
        let sy = addr<float const?>(sin_y[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 4l, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    rope_scaled_neox_tab_part(vp + r * d, hs, half, d, cx + r * pairs, sx + r * pairs)
                    rope_scaled_neox_tab_part(vp + r * d + half, hs, half, d, cy + r * pairs, sy + r * pairs)
                }
            }
        }
    }
}

//! Full-head NEOX RoPE over token-major rows [npos × n_head·hs], in place, ONE table pair per
//! row (every head of a row shares its angles — the qwen3vl ViT's vision-mrope, tables from
//! ``build_rope_tabs_vision``): row r applies cos/sin row r across all heads.
def rope_neox_tab_rows(var v : array<float>; npos, n_head, hs : int64; cos_t, sin_t : array<float>) {
    if (npos <= 0l) return
    let d = n_head * hs
    let half = hs / 2l
    unsafe {
        var vp = addr(v[0])
        let ct = addr<float const?>(cos_t[0])
        let st = addr<float const?>(sin_t[0])
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d * 4l, 0)) $(rb, re) {
            unsafe {
                for (r in range64(int64(rb), int64(re))) {
                    rope_scaled_neox_tab(vp + r * d, hs, d, ct + r * half, st + r * half)
                }
            }
        }
    }
}

//! Antialiased bilinear resize (the pos-embed resize): resample a
//! channel-inner grid [sh×sw rows × d] to [dh×dw rows × d] — triangle filter, support
//! max(1, 1/scale), weights normalized per pixel, f32 accumulation in (sy, sx) order.
def interpolate_grid_bilinear_aa(@scratch var out : array<float>; src : PlaneF; src_off : int64;
                                 sw, sh, dw, dh, d : int64) {
    if (long_length(out) < dw * dh * d) {
        out |> reserve(dw * dh * d)
        out |> resize(dw * dh * d)
    }
    let sf0 = float(dw) / float(sw)
    let sf1 = float(dh) / float(sh)
    let support0 = max(1.0, 1.0 / sf0)
    let support1 = max(1.0, 1.0 / sf1)
    let invscale0 = 1.0 / support0
    let invscale1 = 1.0 / support1
    unsafe {
        let sp = plane_at(src, src_off)
        var op = addr(out[0])
        maybe_parallel_for(0, int(dh), lanes_for_work(dw * dh * d * 8l, 0)) $(yb, ye) {
            unsafe {
                for (i1 in range64(int64(yb), int64(ye))) {
                    let y = (float(i1) + 0.5) / sf1
                    let y_min = max(int64(y - support1 + 0.5), 0l)
                    let y_max = min(int64(y + support1 + 0.5), sh)
                    for (i0 in range64(dw)) {
                        let x = (float(i0) + 0.5) / sf0
                        let x_min = max(int64(x - support0 + 0.5), 0l)
                        let x_max = min(int64(x + support0 + 0.5), sw)
                        var orow = op + (i1 * dw + i0) * d
                        for (c in range64(d)) {
                            orow[c] = 0.0
                        }
                        var total_weight = 0.0
                        for (sy in range64(y_min, y_max)) {
                            let weight_y = max(1.0 - abs((float(sy) - y + 0.5) * invscale1), 0.0)
                            for (sx in range64(x_min, x_max)) {
                                let weight = max(1.0 - abs((float(sx) - x + 0.5) * invscale0), 0.0) * weight_y
                                if (weight <= 0.0) {
                                    continue
                                }
                                let srow = sp + (sy * sw + sx) * d
                                for (c in range64(d)) {
                                    orow[c] += srow[c] * weight
                                }
                                total_weight += weight
                            }
                        }
                        if (total_weight > 0.0) {
                            for (c in range64(d)) {
                                orow[c] /= total_weight
                            }
                        }
                    }
                }
            }
        }
    }
}

//! Average pool k×k, stride k, over a patch grid held token-major (row = gy·gw + gx, d channels):
//! out row (oy·(gw/k) + ox) = mean of the k×k window (ky outer, kx inner accumulation
//! order, then one divide). Ragged edges are dropped. Returns the output row count.
def avg_pool2d_rows(@scratch var out : array<float>; x : array<float>; gw, gh, d, k : int64) : int64 {
    let ow = gw / k
    let oh = gh / k
    let nout = ow * oh
    if (nout <= 0l) return 0l
    if (long_length(out) < nout * d) {
        out |> reserve(nout * d)
        out |> resize(nout * d)
    }
    unsafe {
        var op = addr(out[0])
        let xp = addr<float const?>(x[0])
        maybe_parallel_for(0, int(nout), lanes_for_work(nout * d * k * k, 0)) $(rb, re) {
            unsafe {
                for (o in range64(int64(rb), int64(re))) {
                    let oy = o / ow
                    let ox = o % ow
                    var orow = op + o * d
                    for (c in range64(0l, d)) {
                        orow[c] = 0.0
                    }
                    for (ky in range64(0l, k)) {
                        for (kx in range64(0l, k)) {
                            let irow = xp + ((oy * k + ky) * gw + ox * k + kx) * d
                            for (c in range64(0l, d)) {
                                orow[c] += irow[c]
                            }
                        }
                    }
                    for (c in range64(0l, d)) {
                        orow[c] /= float(k * k)
                    }
                }
            }
        }
    }
    return nout
}

let ATTN_ENC_QB = 128l  // query rows per attention score block (15 MB att scratch at T=1500)

//! Blocked bidirectional attention over token-major q/k/v [tt × d], ``n_head`` heads: out = softmax
//! (scale·QKᵀ)·V per head, no cache, no mask; flattened over head × row-block units so 6 heads
//! don't idle 8 lanes. ``hp``/``att`` are caller scratch grown here; ``scale`` is the caller's.
def attention_bidir(var out : array<float>; qq, kk, vv : array<float>; @scratch var hp : array<float>; // nolint:STYLE038 — pack + blocked-attention phases share the per-head pack pointers; a split helper would take six raw pointers
                    @scratch var att : array<float>; tt, d, n_head : int64; scale : float) {
    let hs = d / n_head
    let nqb = int((tt + ATTN_ENC_QB - 1l) / ATTN_ENC_QB)
    let nu = int(n_head) * nqb
    // que-less-creation guard: `att` may have been sized under a smaller slot bound
    let attNeed = int64(get_dispatch_slot_bound()) * ATTN_ENC_QB * tt
    if (long_length(att) < attNeed) {   // inline: PERF026 can't see @scratch through reserve_resize
        att |> reserve(attNeed)
        att |> resize(attNeed)
    }
    let hpNeed = 4l * tt * d
    if (long_length(hp) < hpNeed) {
        hp |> reserve(hpNeed)
        hp |> resize(hpNeed)
    }
    unsafe {
        let qp = addr<float const?>(qq[0])
        let kp = addr<float const?>(kk[0])
        let vp = addr<float const?>(vv[0])
        var hpp = addr(hp[0])
        var attp = addr(att[0])
        var aop = addr(out[0])
        var tp = ref_time_ticks()
        maybe_parallel_for(0, nu, min(get_dispatch_lanes(), nu)) $(ub, ue) {
            unsafe {
                for (u in range(ub, ue)) {
                    let h = u / nqb
                    let hoff = int64(h) * hs
                    // per-head pack region: q [tt×hs], kT [hs×tt], v [tt×hs], oh [tt×hs]
                    var qh = hpp + int64(h) * 4l * tt * hs
                    var kt = qh + tt * hs
                    var vh = kt + tt * hs
                    let p0 = int64(u % nqb) * ATTN_ENC_QB
                    let p1 = min(p0 + ATTN_ENC_QB, tt)
                    for (p in range64(p0, p1)) {
                        for (j in range64(hs)) {
                            qh[p * hs + j] = qp[p * d + hoff + j]
                            kt[j * tt + p] = kp[p * d + hoff + j]
                            vh[p * hs + j] = vp[p * d + hoff + j]
                        }
                    }
                }
            }
        }
        asr_prof_add("enc.attn.pack", tp)
        tp = ref_time_ticks()
        maybe_parallel_for_indexed(0, nu, nu) $(slot, ub, ue) {
            unsafe {
                var ah = attp + int64(slot) * ATTN_ENC_QB * tt
                for (u in range(ub, ue)) {
                    let h = u / nqb
                    let hoff = int64(h) * hs
                    var qh = hpp + int64(h) * 4l * tt * hs
                    var kt = qh + tt * hs
                    var vh = kt + tt * hs
                    var oh = vh + tt * hs
                    let qb0 = int64(u % nqb) * ATTN_ENC_QB
                    let qbn = min(ATTN_ENC_QB, tt - qb0)
                    for (i in range64(qbn * tt)) {
                        ah[i] = 0.0
                    }
                    gemm_f32(ah, qh + qb0 * hs, kt, qbn, hs, tt)
                    for (r in range64(qbn)) {
                        var row = ah + r * tt
                        for (i in range64(tt)) {
                            row[i] *= scale
                        }
                        softmax(row, tt)
                    }
                    for (i in range64(qbn * hs)) {
                        oh[qb0 * hs + i] = 0.0
                    }
                    gemm_f32(oh + qb0 * hs, ah, vh, qbn, tt, hs)
                    // scatter this unit's rows back to token-major out
                    for (p in range64(qb0, qb0 + qbn)) {
                        for (j in range64(hs)) {
                            aop[p * d + hoff + j] = oh[p * hs + j]
                        }
                    }
                }
            }
        }
        asr_prof_add("enc.attn.blocks", tp)
    }
}

//! ``attention_bidir`` restricted to CONTIGUOUS row windows: ``win_lo`` = each window's first
//! row plus a total-row sentinel; every window attends only within itself (the qwen2.5 ViT's
//! block-diagonal mask in its window-sorted row order). Units = (window, head), per-slot scratch.
def attention_bidir_windows(var out : array<float>; qq, kk, vv : array<float>; @scratch var hp : array<float>;
                            @scratch var att : array<float>; win_lo : array<int64>; d, n_head : int64; scale : float) {
    let hs = d / n_head
    let nwin = long_length(win_lo) - 1l
    var max_wr = 0l
    for (i in range64(nwin)) {
        max_wr = max(max_wr, win_lo[i + 1l] - win_lo[i])
    }
    let nu = int(nwin * n_head)
    // per-slot scratch: q [wr×hs], kT [hs×wr], v [wr×hs], o [wr×hs] + the att block [wr×wr]
    let slot_hp = 4l * max_wr * hs
    let hpNeed = int64(get_dispatch_slot_bound()) * slot_hp
    if (long_length(hp) < hpNeed) {   // inline: PERF026 can't see @scratch through reserve_resize
        hp |> reserve(hpNeed)
        hp |> resize(hpNeed)
    }
    let attNeed = int64(get_dispatch_slot_bound()) * max_wr * max_wr
    if (long_length(att) < attNeed) {
        att |> reserve(attNeed)
        att |> resize(attNeed)
    }
    unsafe {
        let qp = addr<float const?>(qq[0])
        let kp = addr<float const?>(kk[0])
        let vp = addr<float const?>(vv[0])
        let wp = addr<int64 const?>(win_lo[0])
        var hpp = addr(hp[0])
        var attp = addr(att[0])
        var aop = addr(out[0])
        maybe_parallel_for_indexed(0, nu, nu) $(slot, ub, ue) {
            unsafe {
                var qh = hpp + int64(slot) * slot_hp
                var ah = attp + int64(slot) * max_wr * max_wr
                for (u in range(ub, ue)) {
                    let wi = int64(u) / n_head
                    let h = int64(u) % n_head
                    let hoff = h * hs
                    let r0 = wp[wi]
                    let wr = wp[wi + 1l] - r0
                    var kt = qh + wr * hs
                    var vh = kt + wr * hs
                    var oh = vh + wr * hs
                    for (p in range64(wr)) {
                        for (j in range64(hs)) {
                            qh[p * hs + j] = qp[(r0 + p) * d + hoff + j]
                            kt[j * wr + p] = kp[(r0 + p) * d + hoff + j]
                            vh[p * hs + j] = vp[(r0 + p) * d + hoff + j]
                        }
                    }
                    for (i in range64(wr * wr)) {
                        ah[i] = 0.0
                    }
                    gemm_f32(ah, qh, kt, wr, hs, wr)
                    for (r in range64(wr)) {
                        var row = ah + r * wr
                        for (i in range64(wr)) {
                            row[i] *= scale
                        }
                        softmax(row, wr)
                    }
                    for (i in range64(wr * hs)) {
                        oh[i] = 0.0
                    }
                    gemm_f32(oh, ah, vh, wr, wr, hs)
                    for (p in range64(wr)) {
                        for (j in range64(hs)) {
                            aop[(r0 + p) * d + hoff + j] = oh[p * hs + j]
                        }
                    }
                }
            }
        }
    }
}

// ===== encode-stage profiling rail =====
// set_asr_prof(true) → the encode/decode paths accumulate wall us per stage label (asr.* and g4v.* buckets alike);
// asr_prof_report() prints totals sorted desc and resets. Harness-only, off by default.
var g_asr_prof_on = false
var private g_asr_prof : table<string; int64>
var private g_asr_prof_hits : table<string; int64>

def set_asr_prof(on : bool) {
    g_asr_prof_on = on
}

//! Accumulate ``get_time_usec(t0)`` under ``name`` (no-op unless set_asr_prof(true)).
[cold_path]
def asr_prof_add(name : string; t0 : int64) {
    if (!g_asr_prof_on) {
        return
    }
    g_asr_prof[name] += int64(get_time_usec(t0))
    g_asr_prof_hits[name]++
}

//! One bucket's accumulated ms (0 when absent) — the store's encode-split reader.
def asr_prof_ms(name : string) : double {
    return double(g_asr_prof?[name] ?? 0l) / 1000.0lf
}

//! Reset the accumulators without printing — per-rep sampling (asr_prof_report also clears).
def asr_prof_reset() {
    clear(g_asr_prof)
    clear(g_asr_prof_hits)
}

def asr_prof_report() {
    var rows : array<tuple<name : string; us : int64>>
    var total = 0l
    for (k, v in keys(g_asr_prof), values(g_asr_prof)) {
        rows |> push((name = k, us = v))
        total += v
    }
    rows |> sort() $(a, b) => a.us > b.us
    for (r in rows) {
        let hits = g_asr_prof_hits?[r.name] ?? 0l
        let pct = total > 0l ? double(r.us) * 100.0lf / double(total) : 0.0lf
        print("PROF\t{r.name}\t{double(r.us) / 1000.0lf}\t{hits}\t{pct}\n")
    }
    print("PROF\ttotal\t{double(total) / 1000.0lf}\t\t\n")
    clear(g_asr_prof)
    clear(g_asr_prof_hits)
}
