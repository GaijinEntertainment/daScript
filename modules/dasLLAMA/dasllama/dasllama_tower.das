options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tower shared public

require dasllama/dasllama_lint public
require math
require dasllama/dasllama_math
require dasllama/dasllama_math_gen   // matmul_f16_batch: the f16-plane GEMM under mm_f16_b
require dasllama/dasllama_gpu_math
require dasllama/dasllama_plane public   // PlaneF/PlaneU16 — plane forms of the blob helpers
require dasllama/dasllama_par
require dasllama/dasllama_gguf   // read_clamp reads sidecar scalar tensors
require dasllama/dasllama_common   // requant_rows_q8_sized: tower_mm_rq's q8 feed
require dasllama/dasllama_gemm_schema   // Q8K_SUM_ELEMS: the kq lane's Q8_K image
require dasllama/dasllama_math_default   // matmul_kq_batch_disk_at: the portable K-quant batch GEMM over disk-order planes
require dasllama/dasllama_repack   // repack_conv_out_tap_in: the conv reader's tap-major order
require daslib/cmp

// The family-neutral encoder-tower home: the row math, activations, GEMM wrappers, clamp
// sidecars, patch geometry, blocked bidirectional attention, and the encode-stage prof rail
// that every audio AND vision tower composes (ARCHITECTURE_MEDIA.md §1.7). One home: a family
// file that re-implements a shape found here is a defect; nothing here names a family type.

// Per-layer tensor offsets inside fblob, in layout order. conv weights are stored PERMUTED
// for the im2col column order (dk-major, then channel): W'[o][dk·C + c] = torch_w[o][c][dk].
struct LayerOffs {
    ln1_w : int64
    ln1_b : int64
    q_w : int64
    q_b : int64
    k_w : int64
    k_b : int64   // whisper towers have no k bias — the slot stays zeroed and is skipped
    v_w : int64
    v_b : int64
    o_w : int64
    o_b : int64
    ln2_w : int64
    ln2_b : int64
    fc1_w : int64
    fc1_b : int64
    fc2_w : int64
    fc2_b : int64
}

//! one block GEMM's region in a tower's q8 planes: its element offset, its input width and its rows - the unit a GPU driver
//! repacks and the site index its chain dispatches by
struct Q8Region {
    woff : int64
    n : int64
    rows : int64
}

//! a family's GPU tower seat as the lane policy reads it: registered, whether the driver's f32 tiles are its faster lane (the
//! policy then prefers the file's planes over the q8 image), and whether it serves on this box
struct GpuLaneSeat {
    set : bool
    f32_lane : bool = true
    serves : function<() : bool>
    kq_lane : bool = false   //! the driver's chain serves the kq lane - a quantized carrier's own planes (GigaAM's `gemm_seats`)
}

//! the GPU tower would serve this encode on the file's planes - the lane policy's clause: with it false the policy's default is the q8 image
def gpu_exact_serves(seat : GpuLaneSeat) : bool => seat.set && seat.f32_lane && invoke(seat.serves)

//! the lane policy's reason under `seat` and the pin, the load's announce line
def lane_why(seat : GpuLaneSeat; pinned : bool) : string {
    if (pinned) {
        return "pinned"
    } elif (gpu_exact_serves(seat)) {
        return "the GPU tower serves the file's planes (its f32 tiles are the faster lane)"
    } elif (seat.set && invoke(seat.serves)) {
        return "the GPU tower serves the q8 image (its q8 tiles are the faster lane)"
    } elif (float_batch_override_active()) {
        return "DASLLAMA_ACCEL on: the float-batch tier serves the file's planes"
    }
    return "the CPU default (no GPU tower, no accelerate tier)"
}

//! A q8-lane family load's lane line: the lane taken (`q8`, else the `exact` planes) and why - the pin `setter` made, a GPU
//! tower serving the exact planes, or the CPU default
[arch(at="../ARCHITECTURE_MEDIA.md#tower-weight-lane")]
def announce_lane(family : string; q8 : bool; exact, setter : string; pinned, gpu_serves : bool) {
    let why = pinned ? "pinned via {setter}" : (gpu_serves ? "a GPU tower serves the {exact}" : "the CPU default (no GPU tower serving the exact planes)")
    to_log(LOG_INFO, "dasLLAMA {family}: GEMM lane {q8 ? "q8" : exact} - {why}\n")
}

let LN_BLOCK_GEMMS = 6l   //! a pre-LN block's GEMM regions, in the order below
let LN_GEMM_Q = 0l
let LN_GEMM_K = 1l
let LN_GEMM_V = 2l
let LN_GEMM_O = 3l
let LN_GEMM_FC1 = 4l
let LN_GEMM_FC2 = 5l

//! a pre-LN block's GEMM regions in record order - the site order both tower drivers read
def ln_block_regions(o : LayerOffs; d, ff : int64; var out : Q8Region[int(LN_BLOCK_GEMMS)]) {
    out[LN_GEMM_Q] = Q8Region(woff = o.q_w, n = d, rows = d)
    out[LN_GEMM_K] = Q8Region(woff = o.k_w, n = d, rows = d)
    out[LN_GEMM_V] = Q8Region(woff = o.v_w, n = d, rows = d)
    out[LN_GEMM_O] = Q8Region(woff = o.o_w, n = d, rows = d)
    out[LN_GEMM_FC1] = Q8Region(woff = o.fc1_w, n = d, rows = ff)
    out[LN_GEMM_FC2] = Q8Region(woff = o.fc2_w, n = ff, rows = d)
}

//! gelu_erf(x) = 0.5·x·(1 + erf(x/√2)) — the erf form, not the tanh approximation ``gelu`` uses
//! (the kernel: dasllama_math); whisper's conv frontend and encoder FFN use it. In place over ``size``.
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

//! Threaded per-position layernorm over token-major rows (rms_batch's shape, LN math), the
//! weight and bias at `wp` / `bp`; `out` may be `src`, and a single row runs inline.
def layernorm_batch_at(var out : array<float>; src : array<float>; wp, bp : float const?; dim, npos : int64; eps : float) {
    unsafe {
        var op = addr(out[0])
        let ip = addr<float const?>(src[0])
        maybe_parallel_for(0, int(npos), min(int(npos), get_dispatch_lanes())) $(rb, re) {
            unsafe {
                for (p in range(rb, re)) {
                    let off = int64(p) * dim
                    layernorm(op + off, ip + off, wp, bp, dim, eps)
                }
            }
        }
    }
}

//! `layernorm_batch_at` with the weight and bias read from `blob` at the given offsets.
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

//! The pointer core: the bias row at `bp` onto `npos` rows of y from row `r0` on.
def add_bias_rows_at(var y : array<float>; bp : float const?; d, npos : int64; r0 : int64 = 0l) {
    if (npos <= 0l || d <= 0l) return
    let d4 = d - d % 4l
    unsafe {
        var yp = addr(y[r0 * d])
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

//! y[p] = max(y[p] + blob[b_off .. b_off+d), 0) for `npos` rows: the conv epilogue's bias and ReLU fused into one float4
//! pass, bit-exact vs `add_bias_rows` then a ReLU pass; `npos` is explicit because the scratch rows are oversized
def bias_relu_rows(var y : array<float>; blob : PlaneF; b_off, d, npos : int64) {
    let d4 = d - d % 4l
    unsafe {
        var yp = addr(y[0])
        let bp = plane_at(blob, b_off)
        let zero = float4(0.0)
        maybe_parallel_for(0, int(npos), lanes_for_work(npos * d, 0)) $(rb, re) {
            unsafe {
                for (p in range64(int64(rb), int64(re))) {
                    let row = p * d
                    var i = 0l
                    while (i < d4) {
                        let v = (*reinterpret<float4 const?>(yp + row + i)) + (*reinterpret<float4 const?>(bp + i))
                        *reinterpret<float4?>(yp + row + i) = max(v, zero)
                        i += 4l
                    }
                    while (i < d) {
                        yp[row + i] = max(yp[row + i] + bp[i], 0.0)
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

//! A kq-lane tower's block GEMM seat: the format its weights serve in - `q8` the Q8_0 planes ``qblob``/``qscales`` at the
//! element offset `qoff`, every other member the byte planes ``kq``/``ks`` at the byte offsets `qoff`/`soff` - three words a
//! GEMM in the model's ``gemm_seats`` plane, in the block GEMMs' layout order.
struct TowerGemmSeat {
    fmt : KqFmt
    qoff : int64
    soff : int64
}

let TOWER_SEAT_WORDS = 3l   //! a seat's words in the ``gemm_seats`` plane: the format slot, the quant offset, the scale offset
let FC_BLOCK_GEMMS = 10l    //! a FastConformer block's GEMMs, in the layout order `fc_gemm_seat_index` walks

//! The seat index of the FastConformer block GEMM at the chain's virtual element offset `woff` (canary's offsets record over a
//! `gemm_off` base and a `gemm_stride` a block): the block from the stride, the GEMM from its place among the ten element counts
//! in layout order - ff1's pair, q, k, v, out, pw1 (2d rows), pw2, ff2's pair.
def fc_gemm_seat_index(d, ff, gemm_off, gemm_stride, woff : int64) : int64 {
    let rel = woff - gemm_off
    let l = rel / gemm_stride
    var rem = rel - l * gemm_stride
    let sizes = fixed_array(ff * d, d * ff, d * d, d * d, d * d, d * d, 2l * d * d, d * d, ff * d, d * ff)
    var i = 0
    while (i + 1 < int(FC_BLOCK_GEMMS) && rem >= sizes[i]) {
        rem -= sizes[i]
        i++
    }
    return l * FC_BLOCK_GEMMS + int64(i)
}

//! The seat of the kq-lane block GEMM at the chain's virtual element offset `woff`.
def tower_gemm_seat(m : auto(MT); woff : int64) : TowerGemmSeat {
    let i = fc_gemm_seat_index(m.d_model, m.n_ff, m.gemm_off, m.gemm_stride, woff) * TOWER_SEAT_WORDS
    return TowerGemmSeat(fmt = kq_fmt_of_slot(int(m.gemm_seats[i])), qoff = m.gemm_seats[i + 1l], soff = m.gemm_seats[i + 2l])
}

//! the kq lane's GEMM: a Q8_0 seat on the q8 batch GEMM over the Q8_0 image; a K-quant seat on the backend's kq batch tile
//! over the repacked planes and the Q8_K image, else its GEMV a row at a time - the backend's over repacked planes, the
//! portable one over disk order
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def private tower_mm_kq(m : auto(MT); var y : array<float>; s : TowerGemmSeat; xq : array<int8>; xs : array<float>; xbs : array<int>; n, d, npos : int64) {
    if (s.fmt == KqFmt.q8) {
        matmul_q8q8_batch(y, m.qblob, m.qscales, s.qoff, xq, xs, n, d, npos)
        return
    }
    let fmt = kq_schema_id(s.fmt)
    let nsb = n / KQ_SB_ELEMS
    let nh = n / Q8K_SUM_ELEMS
    unsafe {
        let kqp = plane_at(m.kq, s.qoff)
        let ksp = plane_at(m.ks, s.soff)
        if (!m.kq_repacked) {
            matmul_kq_batch_disk_at(fmt, addr(y[0]), kqp, ksp, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<int const?>(xbs[0]), n, d, npos)
            return
        }
        if (kernel_backend_has_kq_batch()) {
            matmul_kq_batch_at(fmt, addr(y[0]), kqp, ksp, addr<int8 const?>(xq[0]), addr<float const?>(xs[0]), addr<int const?>(xbs[0]), n, d, npos)
            return
        }
        for (r in range64(npos)) {
            matmul_kq_active_at(fmt, addr(y[r * d]), kqp, ksp, addr<int8 const?>(xq[r * n]), addr<float const?>(xs[r * nsb]), addr<int const?>(xbs[r * nh]), n, d)
        }
    }
}

//! A tower family's weight GEMM over the model `m` (any record carrying ``q8``, ``qblob``/``qscales`` and ``blob``): on the kq
//! lane (a record carrying ``kq_lane``) the GEMM's seat decides the planes and the image (`xbs` the Q8_K image's per-16 sums);
//! on the q8 lane the Q8_0 planes against the image (`xq`, `xs`) the caller quantized once for every consumer; else `mm_blob_b`
def tower_mm(m : auto(MT); var y : array<float>; woff : int64; x : auto(XT); xq : array<int8>; xs : array<float>; xbs : array<int>; n, d, npos : int64) {
    static_if (typeinfo has_field<kq_lane>(m)) {
        if (m.kq_lane) {
            tower_mm_kq(m, y, tower_gemm_seat(m, woff), xq, xs, xbs, n, d, npos)
            return
        }
    }
    static_if (typeinfo has_field<gemm_f16>(m)) {
        if (m.gemm_f16) {
            mm_f16_b(y, m.wblob, woff, x, n, d, npos)
            return
        }
    }
    if (m.q8) {
        matmul_q8q8_batch(y, m.qblob, m.qscales, woff, xq, xs, n, d, npos)
    } else {
        mm_blob_b(y, m.blob, woff, x, n, d, npos)
    }
}

//! `tower_mm` for a GEMM no kq lane reaches (a position table's, a front's): the Q8_K sums never read
def tower_mm(m : auto(MT); var y : array<float>; woff : int64; x : auto(XT); xq : array<int8>; xs : array<float>; n, d, npos : int64) {
    static_if (typeinfo has_field<kq_lane>(m)) {
        if (m.kq_lane) {
            panic("dasLLAMA tower: a kq-lane GEMM takes the tower_mm form with the Q8_K sums")
        }
    }
    let none : array<int>
    tower_mm(m, y, woff, x, xq, xs, none, n, d, npos)
}

//! The activation image the GEMM at `woff` reads, quantized from `x` into the scratch: the Q8_K form (per-superblock scales in
//! `xs`, per-16 sums in `xbs`) for a K-quant seat of the kq lane, the Q8_0 form on the q8 lane and for a Q8_0 seat, nothing
//! for the float planes; the GEMMs that share one image are one format class (the kq-lane planner holds them to it)
def tower_requant(m : auto(MT); woff : int64; x : array<float>; n, npos : int64; @scratch var xq : array<int8>;
                  @scratch var xs : array<float>; @scratch var xbs : array<int>) {
    static_if (typeinfo has_field<kq_lane>(m)) {
        if (m.kq_lane) {
            let s = tower_gemm_seat(m, woff)
            if (s.fmt != KqFmt.q8) {
                let need = npos * n
                ensure_length(xq, need)
                ensure_length(xs, need / KQ_SB_ELEMS)
                ensure_length(xbs, need / Q8K_SUM_ELEMS)
                requant_rows_q8k_bs(x, n, npos, xq, xs, xbs)
                return
            }
            requant_rows_q8_sized(x, n, npos, xq, xs)
            return
        }
    }
    if (m.q8) {
        requant_rows_q8_sized(x, n, npos, xq, xs)
    }
}

//! `tower_mm` for a single-consumer activation: the quantizing lanes requantize `x` into the scratch first
def tower_mm_rq(m : auto(MT); var y : array<float>; woff : int64; x : array<float>; @scratch var xq : array<int8>;
                @scratch var xs : array<float>; @scratch var xbs : array<int>; n, d, npos : int64) {
    tower_requant(m, woff, x, n, npos, xq, xs, xbs)
    tower_mm(m, y, woff, x, xq, xs, xbs, n, d, npos)
}

//! `tower_mm_rq` for a GEMM no kq lane reaches
def tower_mm_rq(m : auto(MT); var y : array<float>; woff : int64; x : array<float>; @scratch var xq : array<int8>;
                @scratch var xs : array<float>; n, d, npos : int64) {
    if (m.q8) {
        requant_rows_q8_sized(x, n, npos, xq, xs)
    }
    tower_mm(m, y, woff, x, xq, xs, n, d, npos)
}

//! `mm_blob_b`'s BF16-weight twin — the tower form of `matmul_bf16_batch`, weights borrowed from
//! a prepared image. Bit-for-bit the fp32 expand at half the weight read, so a tower can hold
//! its GEMMs in bf16 with no parity move.
def mm_bf16_b(var y : array<float>; blob : PlaneU16; woff : int64; x : array<float>; n, d, npos : int64) {
    matmul_bf16_batch(y, blob, woff, x, n, d, npos)
}

//! `mm_blob_b`'s f16-weight twin: the GEMM off the carrier's own halfwords, each lane widening a four-row group as it
//! reads it, so the outputs are `mm_blob_b`'s over the widened plane at half the weight bytes.
def mm_f16_b(var y : array<float>; blob : PlaneU16; woff : int64; x : array<float>; n, d, npos : int64) {
    unsafe {
        matmul_f16_batch(addr(y[0]), plane_at(blob, woff), addr<float const?>(x[0]), n, d, npos)
    }
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

//! Re-stride ``rows`` already-read f32 rows from ``src`` (``src_stride`` wide, packed) into
//! ``blob`` at ``wo`` on a wider ``dst_stride`` — the padded patch-conv GEMM: each row lands at
//! its padded pitch and the pad columns stay whatever the region was sized to (zero).
def tower_stage_f32_pad_row_stride(var blob : array<float>; src : array<float>;
                                   wo, rows, src_stride, dst_stride : int64) {
    unsafe {
        var dp = addr(blob[wo])
        let sp = addr<float const?>(src[0])
        for (r in range64(rows)) {
            copy_floats(dp + r * dst_stride, sp + r * src_stride, src_stride)
        }
    }
}

//! The plane form: ``rows`` rows of ``src_stride`` floats read from ``src`` at ``src_off``, each landing at its padded pitch.
def tower_stage_f32_pad_row_stride(var blob : array<float>; src : PlaneF; src_off,
                                   wo, rows, src_stride, dst_stride : int64) {
    unsafe {
        var dp = addr(blob[wo])
        let sp = plane_at(src, src_off)
        for (r in range64(rows)) {
            copy_floats(dp + r * dst_stride, sp + r * src_stride, src_stride)
        }
    }
}

//! The fronts' first conv as the im2col GEMM's weight panel [nch][kdp]: the nine taps of each channel on the lattice, zero past
//! them - the file's taps [ch][9] (canary), or [9][ch] under ``tap_major`` (parakeet).
def tower_stage_conv0_panel(var slab : array<float>; blob : PlaneF; w_off, nch, kdp : int64; tap_major : bool) {
    slab |> reserve_resize(nch * kdp)
    if (tap_major) {
        let src = plane_at(blob, w_off)
        for (o in range64(nch)) {
            for (k in range64(9l)) {
                slab[o * kdp + k] = unsafe(src[k * nch + o])
            }
        }
    } else {
        tower_stage_f32_pad_row_stride(slab, blob, w_off, 0l, nch, 9l, kdp)
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

//! A GGUF conv1d weight into `dst` at `off` in `repack_conv_out_tap_in`'s tap-major order, [nout][k][cin]. Panics, naming `who`, on an element count off the shape.
def tower_read_conv_tap_major(m : GGUFMeta; b : array<uint8> | #; name : string; var dst : array<float>;
                              off, nout, cin, k : int64; who : string) {
    let raw <- gguf_read_f32_arr(m, b, name, who)
    if (long_length(raw) != nout * cin * k) {
        panic("dasLLAMA {who}: tensor '{name}' has {long_length(raw)} elements, expected {nout * cin * k}")
    }
    repack_conv_out_tap_in(raw, dst, off, nout, cin, k)
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
            let g = xr * sigmoid_f32(1.702 * xr)
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

//! Bake a tower's halfword GEMM twin off its staged f32 blob - the file's own bits (an f16
//! source round-trips exactly; bf16 = the f32 word's top halfword). Whole-blob at the same
//! element offsets; ride-along f32 regions clamp on the f16 form and the route never reads them.
[arch(at="../ARCHITECTURE_IMAGE.md#image-tower-twin-plane")]
def tower_bake_half_twin(blob : array<float>; var wblob : array<uint16>; bf16 : bool) {
    let n = long_length(blob)
    if (long_length(wblob) < n) {
        wblob |> reserve(n)
    }
    wblob |> resize(n)
    unsafe {
        if (bf16) {
            let up = addr<uint const?>(blob[0])
            var dp = addr(wblob[0])
            for (i in range64(n)) {
                dp[i] = uint16(up[i] >> 16u)
            }
        } else {
            let sp = addr<float const?>(blob[0])
            var dp = addr<float16?>(wblob[0])
            for (i in range64(n)) {
                dp[i] = float16(clamp(sp[i], -65504.0, 65504.0))
            }
        }
    }
}

//! the output length of a k3 / stride-2 / pad-1 conv stage over an axis of `l`
def conv_s2_out(l : int64) : int64 => (l - 1l) / 2l + 1l

//! The 3x3, stride-2, pad-1 im2col every audio front runs (qwen3a, gemma4a, canary): input rows
//! [ih][iw][cin] (cin fastest) to output rows [oh*ow][cin*9] with column (ic*3 + kh)*3 + kw, zero
//! past the edges; the output rows split across lanes - pure copies, the same bytes under any split.
def im2col_k3s2p1(var colp : float?; xp : float const?; iw, ih, cin : int64) {
    let ow = conv_s2_out(iw)
    let oh = conv_s2_out(ih)
    let kdim = cin * 9l
    maybe_parallel_for(0, int(oh), lanes_for_work(oh * ow * kdim, 0)) $(rb, re) {
        unsafe {
            for (oy in range64(int64(rb), int64(re))) {
                for (ox in range64(ow)) {
                    let r = (oy * ow + ox) * kdim
                    for (ic in range64(cin)) {
                        for (kh in range64(3l)) {
                            let iy = oy * 2l - 1l + kh
                            for (kw in range64(3l)) {
                                let ix = ox * 2l - 1l + kw
                                colp[r + (ic * 3l + kh) * 3l + kw] = (iy >= 0l && iy < ih && ix >= 0l && ix < iw
                                    ? xp[(iy * iw + ix) * cin + ic]
                                    : 0.0)
                            }
                        }
                    }
                }
            }
        }
    }
}

//! The im2col of a conv1d of odd kernel `k` and centered pad at `stride` over rows [tin][cin] (whisper's conv2, gigaam's
//! front): output row t holds the k input rows stride * t - pad .. stride * t - pad + k - 1 back to back (column tap * cin + c),
//! zeros past the edges; the rows split across lanes - pure copies, the same bytes under any split.
def im2col_conv1d_rows(var colp : float?; xp : float const?; tin, cin, tout, k, stride : int64) {
    let pad = (k - 1l) / 2l
    maybe_parallel_for(0, int(tout), lanes_for_work(tout * k * cin, 0)) $(rb, re) {
        unsafe {
            for (t in range64(int64(rb), int64(re))) {
                for (tap in range64(k)) {
                    let src = stride * t - pad + tap
                    var dst = colp + (t * k + tap) * cin
                    if (src >= 0l && src < tin) {
                        copy_floats(dst, xp + src * cin, cin)
                    } else {
                        for (c in range64(cin)) {
                            dst[c] = 0.0
                        }
                    }
                }
            }
        }
    }
}

//! the Transformer-XL rel-position table [ww x d] into `pos_emb`: row w at relative position tt - 1 - w, (sin, cos) interleaved
def rel_pos_table(@scratch var pos_emb : array<float>; d, tt, ww : int64) {
    ensure_length(pos_emb, ww * d)
    for (k in range64(d / 2l)) {
        let freq = exp(-(double(k * 2l) * log(10000.0lf) / double(d)))
        for (w in range64(ww)) {
            let theta = float(freq * double(tt - 1l - w))
            pos_emb[w * d + 2l * k] = sin(theta)
            pos_emb[w * d + 2l * k + 1l] = cos(theta)
        }
    }
}

//! im2col over an interleaved-RGB [h × w × 3] canvas into token-major rows in [C][KH][KW]
//! order (row index c·p² + ky·p + kx), one row per non-overlapping p×p patch walking the canvas
//! row-major (pos_x = i % n_cols, pos_y = i / n_cols). Returns npos; ``x0`` grows to npos × 3p².
[arch(at="../ARCHITECTURE_MEDIA.md#tower-padded-widths")]
def im2col_rgb_patches(planes : array<float>; w, h, p : int64; @scratch var x0 : array<float>; row_stride : int64 = 0l) : int64 {
    let n_cols = w / p
    let npos = n_cols * (h / p)
    let psq = p * p
    let row_dim = row_stride > 0l ? row_stride : 3l * psq
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

// ===== the CPU FastConformer block (canary, parakeet, gigaam) =====

//! the rel-pos attention's slab height in rows: 4-aligned (the gemm_f32 row-group invariant keeps a slab start bit-exact)
//! and tall enough that the K^T and pos panel re-streams stay a few percent of the GEMM time
let FC_ATN_BLOCK = 64l

//! a stage-scan hook: the block's residual stream after stage `tag` of block `l`
typedef FcScanFn = function<(tag : string; l : int64; x : array<float>; n : int64) : void>

//! one macaron FFN half's stage buckets
struct FcFfnProf {
    ln : string
    a : string
    silu : string
    b : string
    res : string
}

//! the profile buckets a family's CPU block reports under; an empty name records nothing
struct FcCpuProf {
    ffn1 : FcFfnProf
    ffn : string
    proj_ln : string
    proj_rq : string
    proj_q : string
    proj_k : string
    proj_v : string
    proj_pos : string         //!< the position stage: the rel-pos table's projection, or the rope over the normed rows
    proj : string
    heads_pack : string
    heads_pos : string
    heads_blocks : string
    heads : string
    attn_out : string
    conv : string
}

//! What a FastConformer family's CPU block runs - the CPU twin of the Metal driver's `FcForm`: the geometry, the GEMM
//! biases, the depthwise taps' layout, the conv module's norm, the float4 lane, the profile buckets and the stage-scan
//! hook. The attention's position form is the type of `fc_block_cpu`'s `pos`: `FcRelPos` or `FcRotary`.
struct FcCpuForm {
    d : int64
    ff : int64
    n_head : int64
    nk : int64                //!< the depthwise taps
    biased : bool = true      //!< parakeet's GEMMs carry no bias
    tap_major : bool = true   //!< depthwise taps [k][d], or canary's channel-major [d][k]
    conv_ln : bool            //!< gigaam: the biased depthwise conv, then a LayerNorm and the SiLU; else the BatchNorm folded into the depthwise, fused with the SiLU
    dw_taps : PlaneU16 = PlaneU16()   //!< the conv_ln form's taps as the carrier's f16 halfwords, the layer offsets indexing it; empty: the taps are f32 in ``blob``
    vec4 : bool = true        //!< the float4 GLU and k9 depthwise; canary keeps the scalar sigmoid forms
    prof : FcCpuProf
    scan_on : bool
    scan : FcScanFn
}

//! the Transformer-XL rel-pos form: the `ww` = 2 tt - 1 rows of ``st.pos_emb`` (its q8 image ``st.posq``/``st.poss``), keys at
//! or past `n_real` masked
struct FcRelPos {
    ww : int64
    n_real : int64
}

//! the rotary form: the rope from ``st.rope_cos``/``st.rope_sin`` over the attention's normed input rows, then `attention_bidir`
struct FcRotary {
    rope : bool = true
}

def private fc_scan(form : FcCpuForm; tag : string; l : int64; x : array<float>; n : int64) {
    if (form.scan_on) {
        invoke(form.scan, tag, l, x, n)
    }
}

def private fc_size_block_scratch(var st : auto(ST); form : FcCpuForm; tt : int64) {
    let d = form.d
    ensure_length(st.xb, tt * d)
    ensure_length(st.h, tt * max(form.ff, 2l * d))
    ensure_length(st.qq, tt * d)
    ensure_length(st.kk, tt * d)
    ensure_length(st.vv, tt * d)
    ensure_length(st.ao, tt * d)
}

def private fc_ffn_half_cpu(m : auto(MT); var st : auto(ST); form : FcCpuForm; p : FcFfnProf; ln_w, ln_b, wa, ba, wb, bb, tt : int64) {
    let d = form.d
    let ff = form.ff
    var tf = ref_time_ticks()
    layernorm_batch(st.xb, st.x, m.blob, ln_w, ln_b, d, tt, m.eps)
    asr_prof_add(p.ln, tf)
    tf = ref_time_ticks()
    tower_mm_rq(m, st.h, wa, st.xb, st.xqi, st.xsi, st.xbsi, d, ff, tt)
    if (form.biased) {
        add_bias_rows(st.h, m.blob, ba, ff, tt)
    }
    asr_prof_add(p.a, tf)
    tf = ref_time_ticks()
    silu4_batch(st.h, ff, tt)
    asr_prof_add(p.silu, tf)
    tf = ref_time_ticks()
    tower_mm_rq(m, st.xb, wb, st.h, st.xqi, st.xsi, st.xbsi, ff, d, tt)
    if (form.biased) {
        add_bias_rows(st.xb, m.blob, bb, d, tt)
    }
    asr_prof_add(p.b, tf)
    tf = ref_time_ticks()
    unsafe {
        var xp = addr(st.x[0])
        let rp = addr<float const?>(st.xb[0])
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * d, 0)) $(rb, re) {
            unsafe {
                for (i in range64(int64(rb) * d, int64(re) * d)) {
                    xp[i] += 0.5 * rp[i]
                }
            }
        }
    }
    asr_prof_add(p.res, tf)
}

//! The rel-pos attention: q, k, v off one normed activation (one q8 requant feeds all three) and the layer's projection of
//! the pos table, then per head the content and rel scores over `FC_ATN_BLOCK`-row slabs, the mask, softmax and P V into
//! ``st.ao``. Each job owns one slab; 4-aligned row groups keep `gemm_f32_jo` bit-exact.
def private fc_attention_cpu(m : auto(MT); var st : auto(ST); form : FcCpuForm; lo : auto(LO); pos : FcRelPos; tt : int64) { // nolint:STYLE038 — the head loop's three phases share the pack pointers
    let d = form.d
    let n_head = form.n_head
    let dh = d / n_head
    let ww = pos.ww
    let n_real = pos.n_real
    ensure_length(st.posl, ww * d)
    ensure_length(st.qu, tt * dh)
    ensure_length(st.qv, tt * dh)
    ensure_length(st.vh, tt * dh)
    ensure_length(st.kt, dh * tt)
    let atn_jobs = min(int64(lanes_for_work(tt * dh * (2l * tt + ww), 0)), (tt + FC_ATN_BLOCK - 1l) / FC_ATN_BLOCK)
    ensure_length(st.sc, atn_jobs * FC_ATN_BLOCK * tt)
    ensure_length(st.rl, atn_jobs * FC_ATN_BLOCK * ww)
    ensure_length(st.oh, atn_jobs * FC_ATN_BLOCK * dh)
    ensure_length(st.posh, dh * ww)
    let scale = 1.0 / sqrt(float(dh))
    var tp = ref_time_ticks()
    var tq = tp
    layernorm_batch(st.xb, st.x, m.blob, lo.nat_w, lo.nat_b, d, tt, m.eps)
    asr_prof_add(form.prof.proj_ln, tq)
    tq = ref_time_ticks()
    tower_requant(m, lo.q_w, st.xb, d, tt, st.xqi, st.xsi, st.xbsi)
    asr_prof_add(form.prof.proj_rq, tq)
    tq = ref_time_ticks()
    tower_mm(m, st.qq, lo.q_w, st.xb, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_q, tq)
    tq = ref_time_ticks()
    tower_mm(m, st.kk, lo.k_w, st.xb, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_k, tq)
    tq = ref_time_ticks()
    tower_mm(m, st.vv, lo.v_w, st.xb, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_v, tq)
    if (form.biased) {
        add_bias_rows(st.qq, m.blob, lo.q_b, d, tt)
        add_bias_rows(st.kk, m.blob, lo.k_b, d, tt)
        add_bias_rows(st.vv, m.blob, lo.v_b, d, tt)
    }
    tq = ref_time_ticks()
    tower_mm(m, st.posl, lo.pos_w, st.pos_emb, st.posq, st.poss, d, d, ww)
    asr_prof_add(form.prof.proj_pos, tq)
    asr_prof_add(form.prof.proj, tp)
    tp = ref_time_ticks()
    let nblkb = (tt + FC_ATN_BLOCK - 1l) / FC_ATN_BLOCK
    let npj = (nblkb + atn_jobs - 1l) / atn_jobs
    for (hi in range64(n_head)) {
        let hoff = hi * dh
        unsafe {
            var qup = addr(st.qu[0])
            var qvp = addr(st.qv[0])
            var ktp = addr(st.kt[0])
            var vhp = addr(st.vh[0])
            var pop = addr(st.posh[0])
            var scp = addr(st.sc[0])
            var rlp = addr(st.rl[0])
            var ohp = addr(st.oh[0])
            var aop = addr(st.ao[0])
            let qqp = addr<float const?>(st.qq[0])
            let kkp = addr<float const?>(st.kk[0])
            let vvp = addr<float const?>(st.vv[0])
            let plp = addr<float const?>(st.posl[0])
            let bup = plane_at(m.blob, lo.bias_u + hoff)
            let bvp = plane_at(m.blob, lo.bias_v + hoff)
            var th = ref_time_ticks()
            maybe_parallel_for(0, int(tt), lanes_for_work(tt * dh * 4l, 0)) $(rb, re) {
                unsafe {
                    for (t in range64(int64(rb), int64(re))) {
                        for (j in range64(dh)) {
                            let qv = qqp[t * d + hoff + j]
                            qup[t * dh + j] = qv + bup[j]
                            qvp[t * dh + j] = qv + bvp[j]
                            ktp[j * tt + t] = kkp[t * d + hoff + j]
                            vhp[t * dh + j] = vvp[t * d + hoff + j]
                        }
                    }
                }
            }
            asr_prof_add(form.prof.heads_pack, th)
            th = ref_time_ticks()
            maybe_parallel_for(0, int(ww), lanes_for_work(ww * dh, 0)) $(rb, re) {
                unsafe {
                    for (w in range64(int64(rb), int64(re))) {
                        for (j in range64(dh)) {
                            pop[j * ww + w] = plp[w * d + hoff + j]
                        }
                    }
                }
            }
            asr_prof_add(form.prof.heads_pos, th)
            th = ref_time_ticks()
            maybe_parallel_for(0, int(atn_jobs), int(atn_jobs)) $(jb, je) {
                unsafe {
                    for (li in range64(int64(jb), int64(je))) {
                        var scs = scp + li * FC_ATN_BLOCK * tt
                        var rls = rlp + li * FC_ATN_BLOCK * ww
                        var ohs = ohp + li * FC_ATN_BLOCK * dh
                        for (blk in range64(li * npj, min((li + 1l) * npj, nblkb))) {
                            let r0 = blk * FC_ATN_BLOCK
                            let r1 = min(r0 + FC_ATN_BLOCK, tt)
                            let nr = r1 - r0
                            for (i in range64(nr * tt)) {
                                scs[i] = 0.0
                            }
                            for (i in range64(nr * ww)) {
                                rls[i] = 0.0
                            }
                            gemm_f32_jo(scs, qup + r0 * dh, ktp, nr, dh, tt)
                            gemm_f32_jo(rls, qvp + r0 * dh, pop, nr, dh, ww)
                            for (q in range64(r0, r1)) {
                                let base = tt - 1l - q
                                var qs = scs + (q - r0) * tt
                                let rs = rls + (q - r0) * ww
                                for (k in range64(tt)) {
                                    var s = (qs[k] + rs[base + k]) * scale
                                    if (k >= n_real) {
                                        s = -1e30
                                    }
                                    qs[k] = s
                                }
                                softmax(qs, tt)
                            }
                            for (i in range64(nr * dh)) {
                                ohs[i] = 0.0
                            }
                            gemm_f32_jo(ohs, scs, vhp, nr, tt, dh)
                            for (t in range64(r0, r1)) {
                                for (j in range64(dh)) {
                                    aop[t * d + hoff + j] = ohs[(t - r0) * dh + j]
                                }
                            }
                        }
                    }
                }
            }
            asr_prof_add(form.prof.heads_blocks, th)
        }
    }
    asr_prof_add(form.prof.heads, tp)
}

//! The rotary attention: the rope over a copy of the normed rows feeds q and k (one q8 requant of the roped rows), v reads
//! the unroped rows, then `attention_bidir` into ``st.ao``.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-rotary-input")]
def private fc_attention_cpu(m : auto(MT); var st : auto(ST); form : FcCpuForm; lo : auto(LO); pos : FcRotary; tt : int64) {
    let d = form.d
    let hs = d / form.n_head
    var tp = ref_time_ticks()
    var tq = tp
    layernorm_batch(st.xb, st.x, m.blob, lo.nat_w, lo.nat_b, d, tt, m.eps)
    asr_prof_add(form.prof.proj_ln, tq)
    tq = ref_time_ticks()
    ensure_length(st.xr, tt * d)
    unsafe {
        copy_floats(addr(st.xr[0]), addr<float const?>(st.xb[0]), tt * d)
    }
    rope_neox_tab_rows(st.xr, tt, form.n_head, hs, st.rope_cos, st.rope_sin)
    asr_prof_add(form.prof.proj_pos, tq)
    tq = ref_time_ticks()
    tower_requant(m, lo.q_w, st.xr, d, tt, st.xqi, st.xsi, st.xbsi)
    asr_prof_add(form.prof.proj_rq, tq)
    tq = ref_time_ticks()
    tower_mm(m, st.qq, lo.q_w, st.xr, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_q, tq)
    tq = ref_time_ticks()
    tower_mm(m, st.kk, lo.k_w, st.xr, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_k, tq)
    tq = ref_time_ticks()
    tower_mm_rq(m, st.vv, lo.v_w, st.xb, st.xqi, st.xsi, st.xbsi, d, d, tt)
    asr_prof_add(form.prof.proj_v, tq)
    if (form.biased) {
        add_bias_rows(st.qq, m.blob, lo.q_b, d, tt)
        add_bias_rows(st.kk, m.blob, lo.k_b, d, tt)
        add_bias_rows(st.vv, m.blob, lo.v_b, d, tt)
    }
    asr_prof_add(form.prof.proj, tp)
    tp = ref_time_ticks()
    attention_bidir(st.ao, st.qq, st.kk, st.vv, st.hp, st.att, tt, d, form.n_head, 1.0 / sqrt(float(hs)))
    asr_prof_add(form.prof.heads, tp)
}

//! The GLU over pw1's [tt][2d] rows into `g` [tt][d]: signal (first half) times the sigmoid of the gate (second half) -
//! the float4 `exp4` form when `vec4` and d % 4 == 0, else the scalar `sigmoid_f32` form.
def private fc_glu_rows(var g : array<float>; h : array<float>; d, tt : int64; vec4 : bool) {
    let n4 = d / 4l
    let d4ok = vec4 && n4 * 4l == d
    unsafe {
        var gp = addr(g[0])
        let hp = addr<float const?>(h[0])
        let one = float4(1.0)
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * d * 4l, 0)) $(rb, re) {
            unsafe {
                for (t in range64(int64(rb), int64(re))) {
                    if (d4ok) {
                        let sgp = reinterpret<float4 const?>(hp + t * 2l * d)
                        let gtp = reinterpret<float4 const?>(hp + t * 2l * d + d)
                        var o4 = reinterpret<float4?>(gp + t * d)
                        for (k in range64(n4)) {
                            let arg = clamp(-gtp[k], EXP4_LO, EXP4_HI)
                            o4[k] = sgp[k] / (one + exp4(arg))
                        }
                    } else {
                        for (j in range64(d)) {
                            gp[t * d + j] = hp[t * 2l * d + j] * sigmoid_f32(hp[t * 2l * d + d + j])
                        }
                    }
                }
            }
        }
    }
}

//! The depthwise conv1d over time (centered pad, tap-major weights [k][d] in ``blob``) plus its bias: the LayerNorm form's conv.
def fc_dw_bias_rows(var o : array<float>; g : array<float>; blob : PlaneF; w_off, b_off, d, tt, k : int64) {
    fc_dw_bias_rows_at(o, g, plane_at(blob, w_off), plane_at(blob, b_off), d, tt, k)
}

//! The same conv over taps the carrier holds as f16 halfwords in `taps` at `w_off`: the [k][d] taps widened once a call
//! (exact) into the caller's scratch `wide`, then the f32 walk - every output the f32 taps' to the bit.
[arch(at="../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def fc_dw_bias_rows_f16(var o : array<float>; g : array<float>; taps : PlaneU16; w_off : int64; blob : PlaneF; b_off, d, tt, k : int64;
                        @scratch var wide : array<float>) {
    ensure_length(wide, k * d)
    unsafe {
        cvt_f16_to_f32(addr(wide[0]), plane_at(taps, w_off), k * d)
        fc_dw_bias_rows_at(o, g, addr<float const?>(wide[0]), plane_at(blob, b_off), d, tt, k)
    }
}

def private fc_dw_bias_rows_at(var o : array<float>; g : array<float>; wp, bp : float const?; d, tt, k : int64) {
    let pad = (k - 1l) / 2l
    unsafe {
        var op = addr(o[0])
        let gp = addr<float const?>(g[0])
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * d * k, 0)) $(rb, re) {
            unsafe {
                for (t in range64(int64(rb), int64(re))) {
                    var orow = op + t * d
                    copy_floats(orow, bp, d)
                    for (tap in range64(k)) {
                        let src = t - pad + tap
                        if (src >= 0l && src < tt) {
                            let grow = gp + src * d
                            let wrow = wp + tap * d
                            for (c in range64(d)) {
                                orow[c] += wrow[c] * grow[c]
                            }
                        }
                    }
                }
            }
        }
    }
}

//! The depthwise conv1d over time (centered pad) with the BatchNorm folded to a scale/shift, fused with the SiLU. Taps are
//! tap-major [k][d] or channel-major [d][k]; `vec4` on tap-major k9 rows with d % 4 == 0 runs the float4 `exp4` form,
//! everything else the scalar `sigmoid_f32` form.
def private fc_dw_bn_silu_rows(var o : array<float>; g : array<float>; blob : PlaneF; w_off, sc_off, sh_off, d, tt, nk : int64;
                               tap_major, vec4 : bool) {
    let kpad = (nk - 1l) / 2l
    let one = float4(1.0, 1.0, 1.0, 1.0)
    let f4ok = vec4 && tap_major && nk == 9l && (d / 4l) * 4l == d
    let wt = tap_major ? d : 1l    // weight stride per tap
    let wc = tap_major ? 1l : nk   // weight stride per channel
    unsafe {
        let xbp = addr<float const?>(g[0])
        var aop = addr(o[0])
        let dwp = plane_at(blob, w_off)
        let scp = plane_at(blob, sc_off)
        let shp = plane_at(blob, sh_off)
        maybe_parallel_for(0, int(tt), lanes_for_work(tt * d * (nk + 4l), 0)) $(rb, re) {
            unsafe {
                for (t in range64(int64(rb), int64(re))) {
                    if (f4ok && t >= kpad && t < tt - kpad) {
                        var j = 0l
                        while (j < d) {
                            let base = (t - 4l) * d + j
                            var acc = (*reinterpret<float4 const?>(xbp + base)) * (*reinterpret<float4 const?>(dwp + j))
                            acc = mad(*reinterpret<float4 const?>(xbp + base + d), *reinterpret<float4 const?>(dwp + d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 2l * d), *reinterpret<float4 const?>(dwp + 2l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 3l * d), *reinterpret<float4 const?>(dwp + 3l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 4l * d), *reinterpret<float4 const?>(dwp + 4l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 5l * d), *reinterpret<float4 const?>(dwp + 5l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 6l * d), *reinterpret<float4 const?>(dwp + 6l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 7l * d), *reinterpret<float4 const?>(dwp + 7l * d + j), acc)
                            acc = mad(*reinterpret<float4 const?>(xbp + base + 8l * d), *reinterpret<float4 const?>(dwp + 8l * d + j), acc)
                            let bv4 = mad(acc, *reinterpret<float4 const?>(scp + j), *reinterpret<float4 const?>(shp + j))
                            let arg = clamp(-bv4, EXP4_LO, EXP4_HI)
                            *reinterpret<float4?>(aop + t * d + j) = bv4 / (one + exp4(arg))
                            j += 4l
                        }
                    } elif (f4ok) {
                        var j = 0l
                        while (j < d) {
                            var acc = float4(0.0, 0.0, 0.0, 0.0)
                            for (k in range64(9l)) {
                                let src = t + k - kpad
                                if (src >= 0l && src < tt) {
                                    acc = mad(*reinterpret<float4 const?>(xbp + src * d + j), *reinterpret<float4 const?>(dwp + k * d + j), acc)
                                }
                            }
                            let bv4 = mad(acc, *reinterpret<float4 const?>(scp + j), *reinterpret<float4 const?>(shp + j))
                            let arg = clamp(-bv4, EXP4_LO, EXP4_HI)
                            *reinterpret<float4?>(aop + t * d + j) = bv4 / (one + exp4(arg))
                            j += 4l
                        }
                    } else {
                        for (j in range64(d)) {
                            var acc = 0.0
                            for (k in range64(nk)) {
                                let src = t + k - kpad
                                if (src >= 0l && src < tt) {
                                    acc += dwp[k * wt + j * wc] * xbp[src * d + j]
                                }
                            }
                            let bv = acc * scp[j] + shp[j]
                            aop[t * d + j] = bv * sigmoid_f32(bv)
                        }
                    }
                }
            }
        }
    }
}

//! The conv module: LayerNorm, pw1, GLU, the depthwise conv and its norm, the SiLU, pw2, onto the residual stream.
def private fc_conv_module_cpu(m : auto(MT); var st : auto(ST); form : FcCpuForm; lo : auto(LO); tt : int64) {
    let d = form.d
    layernorm_batch(st.xb, st.x, m.blob, lo.ncv_w, lo.ncv_b, d, tt, m.eps)
    tower_mm_rq(m, st.h, lo.pw1_w, st.xb, st.xqi, st.xsi, st.xbsi, d, 2l * d, tt)
    if (form.biased) {
        add_bias_rows(st.h, m.blob, lo.pw1_b, 2l * d, tt)
    }
    if (form.conv_ln) {
        fc_glu_rows(st.xb, st.h, d, tt, form.vec4)
        if (form.dw_taps.n > 0l) {
            static_if (typeinfo has_field<dw_wide>(st)) {
                fc_dw_bias_rows_f16(st.ao, st.xb, form.dw_taps, lo.dw_w, m.blob, lo.dw_b, d, tt, form.nk, st.dw_wide)
            } else {
                panic("dasLLAMA tower: a family with f16 depthwise taps carries the widened taps' scratch (`dw_wide`) in its state")
            }
        } else {
            fc_dw_bias_rows(st.ao, st.xb, m.blob, lo.dw_w, lo.dw_b, d, tt, form.nk)
        }
        layernorm_batch(st.xb, st.ao, m.blob, lo.bn_w, lo.bn_b, d, tt, m.eps)
        silu4_batch(st.xb, d, tt)
    } else {
        fc_glu_rows(st.ao, st.h, d, tt, form.vec4)
        fc_dw_bn_silu_rows(st.xb, st.ao, m.blob, lo.dw_w, lo.bn_w, lo.bn_b, d, tt, form.nk, form.tap_major, form.vec4)
    }
    tower_mm_rq(m, st.ao, lo.pw2_w, st.xb, st.xqi, st.xsi, st.xbsi, d, d, tt)
    if (form.biased) {
        add_bias_rows(st.ao, m.blob, lo.pw2_b, d, tt)
    }
    add_inplace_rows(st.x, st.ao, tt * d)
}

//! One CPU FastConformer block `l` over the `tt` rows of ``st.x``, in place: the macaron FFN halves around the attention
//! (its position form is `pos`'s type) and the conv module, then the output LayerNorm; `lo` is canary's offsets record,
//! -1 in a slot the form never reads.
def fc_block_cpu(m : auto(MT); var st : auto(ST); form : FcCpuForm; pos : auto(PT); lo : auto(LO); l, tt : int64) {
    let d = form.d
    fc_size_block_scratch(st, form, tt)
    var tp = ref_time_ticks()
    fc_ffn_half_cpu(m, st, form, form.prof.ffn1, lo.nf1_w, lo.nf1_b, lo.ff1a_w, lo.ff1a_b, lo.ff1b_w, lo.ff1b_b, tt)
    fc_scan(form, "ffn1", l, st.x, tt * d)
    asr_prof_add(form.prof.ffn, tp)
    fc_attention_cpu(m, st, form, lo, pos, tt)
    tp = ref_time_ticks()
    tower_mm_rq(m, st.xb, lo.o_w, st.ao, st.xqi, st.xsi, st.xbsi, d, d, tt)
    if (form.biased) {
        add_bias_rows(st.xb, m.blob, lo.o_b, d, tt)
    }
    add_inplace_rows(st.x, st.xb, tt * d)
    fc_scan(form, "attn", l, st.x, tt * d)
    asr_prof_add(form.prof.attn_out, tp)
    tp = ref_time_ticks()
    fc_conv_module_cpu(m, st, form, lo, tt)
    fc_scan(form, "conv", l, st.x, tt * d)
    asr_prof_add(form.prof.conv, tp)
    tp = ref_time_ticks()
    fc_ffn_half_cpu(m, st, form, FcFfnProf(), lo.nf2_w, lo.nf2_b, lo.ff2a_w, lo.ff2a_b, lo.ff2b_w, lo.ff2b_b, tt)
    layernorm_batch(st.x, st.x, m.blob, lo.nout_w, lo.nout_b, d, tt, m.eps)
    fc_scan(form, "out", l, st.x, tt * d)
    asr_prof_add(form.prof.ffn, tp)
}

//! the tower driver's held-rows hook: the generation of the FastConformer encoder rows the driver holds on the device for a
//! transducer or CTC decode, 0 where it holds none
typedef TowerRowsGenFn = function<() : uint64>
var private g_tower_rows_gen : TowerRowsGenFn
var private g_tower_rows_gen_set = false

[arch(at="../ARCHITECTURE_MEDIA.md#tower-gpu-hook")]
def register_tower_rows_gen(fn : TowerRowsGenFn) {
    g_tower_rows_gen = fn
    g_tower_rows_gen_set = true
}

//! The generation of the encoder rows a tower driver holds on the device, 0 with no driver or no rows - a family session
//! records it right after its encode, and its decode hands it to the device seats as the held rows' key.
def tower_rows_gen() : uint64 => g_tower_rows_gen_set ? invoke(g_tower_rows_gen) : 0ul

// ===== encode-stage profiling rail =====
// set_asr_prof(true) → the encode/decode paths accumulate wall us per stage label (asr.* and g4v.* buckets alike);
// asr_prof_report() prints totals sorted desc and resets. Harness-only, off by default.
var g_asr_prof_on = false
var private g_asr_prof : table<string; int64>
var private g_asr_prof_hits : table<string; int64>

def set_asr_prof(on : bool) {
    g_asr_prof_on = on
}

//! Whether the profiling rail is on - the setting ``set_asr_prof`` last made, for a caller that puts it back.
def asr_prof_on() : bool => g_asr_prof_on

//! Accumulate ``get_time_usec(t0)`` under ``name`` (no-op unless set_asr_prof(true), and on an empty name).
def asr_prof_add(name : string; t0 : int64) {
    if (g_asr_prof_on) {
        asr_prof_accumulate(name, int64(get_time_usec(t0)))
    }
}

//! Accumulate `ms` read off another clock - a command buffer's GPU time - under ``name`` (no-op unless set_asr_prof(true)).
def asr_prof_add_ms(name : string; ms : double) {
    if (g_asr_prof_on) {
        asr_prof_accumulate(name, int64(ms * 1000.0lf))
    }
}

[cold_path]
def private asr_prof_accumulate(name : string; us : int64) {
    return if (empty(name))
    g_asr_prof[name] += us
    g_asr_prof_hits[name]++
}

//! One bucket's accumulated ms (0 when absent) — the store's encode-split reader.
def asr_prof_ms(name : string) : double => double(g_asr_prof?[name] ?? 0l) / 1000.0lf

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
    rows |> sort() <| cmp_by(-_.us)
    for (r in rows) {
        let hits = g_asr_prof_hits?[r.name] ?? 0l
        let pct = total > 0l ? double(r.us) * 100.0lf / double(total) : 0.0lf
        print("PROF\t{r.name}\t{double(r.us) / 1000.0lf}\t{hits}\t{pct}\n")
    }
    print("PROF\ttotal\t{double(total) / 1000.0lf}\t\t\n")
    clear(g_asr_prof)
    clear(g_asr_prof_hits)
}
