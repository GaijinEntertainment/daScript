options gen2
//! The GPU kernel helpers both kernel homes splice into their shader bodies: pure ALU, no table, no backend
//! lowering - the Metal and the Vulkan emitters each carry a call into its shader as a function of that shader.
module dasllama_gpu_math shared public
require dasllama/dasllama_lint public
require math
require daslib/math_bits

//! ksigns_iq2xs without the table: bit 7 of the sign byte is the EVEN PARITY of the 7-bit index - five ALU ops, nothing staged.
def ksign7(v : uint) : uint {
    var t = v ^ (v >> 4u)
    t = t ^ (t >> 2u)
    t = t ^ (t >> 1u)
    return v | ((t & 1u) << 7u)
}

//! the iq3s sign-apply over a grid word by xor-and-add: the magnitudes are odd, so no byte carries into the next
def iq3s_signed(g, nib : uint) : uint {
    let m1 = (nib * 0x00204081u) & 0x01010101u
    let mask = m1 * 255u
    return (g ^ mask) + m1
}

//! gemma's soft cap cap * tanh(v / cap) in softcap4's exp form: a shader tanh NaNs past ~44, while exp(INF) zeroes the term and saturates.
def softcap_exp(v, cap : float) : float => cap * (1.0 - 2.0 / (1.0 + exp(2.0 * v / cap)))

//! the row a decode GEMV lane serves in row group `grp` (a workgroup, or one turn of one): `lpr` lanes a row, `sgs` lanes a subgroup,
//! `nsg` subgroups a group
def gemv_lane_row(grp, sg, lane, lpr, sgs, nsg : uint) : uint => (grp * nsg + sg) * (sgs / lpr) + lane / lpr

//! the slot (`sg * sgs + lane`) of the first lane serving row `j` in row group `grp` (`gemv_lane_row`), for `j` among that group's rows
def gemv_lane_row_slot(j, grp, lpr, sgs, nsg : uint) : uint {
    let per_sg = sgs / lpr
    let q = j - grp * nsg * per_sg
    return (q / per_sg) * sgs + (q % per_sg) * lpr
}

def gemv_wgs_of(rows : int64; lanes_per_row : uint; subgroup_size, rows_per_wg : int64) : int64 {
    //! `lanes_per_row` 0 is a whole subgroup a row; a workgroup's `rows_per_wg` subgroups each take `subgroup_size / lanes_per_row` rows
    let l = lanes_per_row == 0u ? subgroup_size : int64(lanes_per_row)
    let rpw = rows_per_wg * (subgroup_size / l)
    return (rows + rpw - 1l) / rpw
}

//! the residual epilogue's workgroups over `rows` rows: four rows a subgroup
def ar_epilogue_wgs(rows, rows_per_wg : int64) : int64 => (rows + 4l * rows_per_wg - 1l) / (4l * rows_per_wg)

//! `blk` once a tile of `total` columns, `tile` wide, with `pc.col0` / `pc.ncols` naming the tile; `pc` holds all `total` after
[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-col-tiles")]
def each_col_tile(var pc : auto(PC)&; total, tile : uint; blk : block<() : void>) {
    var c0 = 0u
    while (c0 < total) {
        pc.col0 = c0
        pc.ncols = min(total - c0, tile)
        invoke(blk)
        c0 += tile
    }
    pc.col0 = 0u
    pc.ncols = total
}

let DA_NSPLIT_MAX = 16l   //! the decode attention's key splits a head, at most (the partials plane sizes to it)

def da_nsplit(wgs, sm_count : int64) : int64 {
    //! enough key splits a head to cover the SMs twice, at most DA_NSPLIT_MAX; one where the SM count is unknown or `wgs` covers it
    if (sm_count <= 0l || wgs <= 0l || wgs >= 2l * sm_count) return 1l
    return min((2l * sm_count + wgs - 1l) / wgs, DA_NSPLIT_MAX)
}

//! the shadow starts on a 64-key step under the window from `pos0`; 0 on a flat layer or a window not yet past its span
def kv_shadow_pos0(window, pos0 : int64) : int64 => window != 0l && pos0 > window ? ((pos0 - window) / 64l) * 64l : 0l

//! A split-K knee: the grid target in threadgroups (0 keeps every tile whole), the least K a share walks, the K granularity a
//! share's start keeps (the home's staging chunk), and whether the shares divide K evenly (`exact`) or the last runs short
struct SplitKnee {
    tgs : int64
    min_k : int64
    align : int64 = 64l
    exact : bool = true
}

//! the share of `kdim` over `ways`: an even split's, or the `align`-rounded share whose last one runs short; 0 where none fits
def split_k_share(kdim, ways : int64; knee : SplitKnee) : int64 {
    if (knee.exact) {
        return kdim % (ways * knee.align) == 0l ? kdim / ways : 0l
    }
    let ck = ((kdim + ways - 1l) / ways + knee.align - 1l) / knee.align * knee.align
    return ck * (ways - 1l) < kdim ? ck : 0l
}

//! the 256-aligned chunk `nsplit` pieces cut K into; 0 when the last piece would start past K
def cm2_chunk(n, nsplit : int64) : int64 => split_k_share(n, nsplit, SplitKnee(align = 256l, exact = false))

//! the most k chunks, up to `want` and eight, `cm2_chunk` admits over K `n` whose partial planes of `plane_bytes` each fit
//! `scratch_bytes` (1: whole)
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_split_fit(n, want : int64; plane_bytes : int64 = 0l; scratch_bytes : int64 = 9_223_372_036_854_775_807l) : int64 {
    var sk = min(want, 8l)
    while (sk > 1l && (cm2_chunk(n, sk) == 0l || sk * plane_bytes > scratch_bytes)) {
        sk--
    }
    return max(sk, 1l)
}

//! workgroups a (d, cnt) GEMM takes at the token column `tc`
def cm2_tiles_at(d, cnt, tc : int64) : int64 {
    verify(tc >= 32l, "cm2_tiles_at got the e column's class key (CM2_TC_E), which is no column width")
    return ((d + 127l) / 128l) * ((cnt + tc - 1l) / tc)
}

//! batch GEMM schedule record `rid` = [wblk0, row0, rows, 0] at word 4 rid and its map of `groups` words at the cursor;
//! returns the map's word offset and advances the cursor
def batch_sched_map(var mp : uint?; var cur : int64&; rid : int64; wblk0, rows, groups : int64; row0 : int64 = 0l) : uint {
    unsafe {
        mp[rid * 4l] = uint(wblk0)
        mp[rid * 4l + 1l] = uint(row0)
        mp[rid * 4l + 2l] = uint(rows)
        mp[rid * 4l + 3l] = 0u
        let at = cur
        for (g in range64(groups)) {
            mp[at + g] = uint(rid)
        }
        cur += groups
        return uint(at)
    }
}

//! the workgroups a region of `cnt` rows takes at the tile column `tc` over `wtiles` weight tiles
def sched_region_wgs(cnt, tc, wtiles : int64) : int64 => ((cnt + tc - 1l) / tc) * wtiles

//! the batch GEMM schedule over the regions `blks` / `row0s` / `cnts` (a region's weight block base, its window-local row base
//! and its rows) at the tile column `tc` over `wtiles` weight tiles: the 4-word records [wblk, row0, cnt, wg0] at word 4 i, then
//! at word 4 nb the per-workgroup map, region i's `sched_region_wgs` words in order; returns the workgroup count (the map's length)
def sched_regions(var mp : uint?; blks, row0s, cnts : array<int64>; tc, wtiles : int64) : int64 {
    let nb = long_length(cnts)
    let map_off = nb * 4l
    var wg = 0l
    unsafe {
        for (i in range64(nb)) {
            mp[i * 4l] = uint(blks[i])
            mp[i * 4l + 1l] = uint(row0s[i])
            mp[i * 4l + 2l] = uint(cnts[i])
            mp[i * 4l + 3l] = uint(wg)
            let rwg = sched_region_wgs(cnts[i], tc, wtiles)
            for (g in range64(rwg)) {
                mp[map_off + wg + g] = uint(i)
            }
            wg += rwg
        }
    }
    return wg
}

//! the regions of the offset triples `offs` ([weight offset, row base, rows] a region) that keep a row inside the window [r0, r1):
//! each kept region's index in `regs`, its window-local row base in `row0s` and its rows in `cnts`, in order; the lists emptied first
def sched_clip(offs : int64 const?; nregions, r0, r1 : int64; var regs, row0s, cnts : array<int64>) {
    regs |> clear()
    row0s |> clear()
    cnts |> clear()
    regs |> reserve(nregions)
    row0s |> reserve(nregions)
    cnts |> reserve(nregions)
    unsafe {
        for (r in range64(nregions)) {
            let b0 = max(offs[r * 3l + 1l], r0)
            let b1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            continue if (b1 <= b0)
            regs |> push(r)
            row0s |> push(b0 - r0)
            cnts |> push(b1 - b0)
        }
    }
}

//! the units a grid of `wgs` workgroups costs over `slots` wave slots at the column's step weight (l 16, m 10, s 5) a `steps`-deep k walk
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_wave_units(wgs, steps, slots, tc : int64) : int64 => ((wgs + slots - 1l) / slots) * steps * (tc == 256l ? 16l : (tc == 128l ? 10l : 5l))

//! the cm2 tile column for `cnt` activation rows against a `d`-row plane on a card of `cores` SMs and `slots` wave slots (the
//! blocks an SM the home runs): the s column (32) under 64 rows and where the m tiles would leave three quarters of the SMs
//! idle, the m column (128) under 128 rows, else whichever of the l and m columns costs fewer whole waves of the slots (a tie
//! to l); 256 where the SM count is unknown
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_wave_col(d, cnt, cores, slots : int64) : int64 {
    if (cnt <= 64l) return 32l
    if (cnt <= 128l) return 128l
    if (cores == 0l) return 256l
    let tiles_m = cm2_tiles_at(d, cnt, 128l)
    if (tiles_m * 4l <= cores) return 32l
    return cm2_wave_units(tiles_m, 1l, slots, 128l) < cm2_wave_units(cm2_tiles_at(d, cnt, 256l), 1l, slots, 256l) ? 128l : 256l
}

//! a K/V mirror position's row in its layer's slab: the position itself, or its slot of a sliding layer's ring of `ring` rows; the
//! modulo never divides by zero, since a shader compiler may take a division as proof its divisor is not zero and drop the flat arm
[arch(at="../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring")]
def kv_ring_row(pos, ring : uint) : uint => ring == 0u ? pos : pos % max(ring, 1u)

//! ... the host's form, for the row movers and the plan
[arch(at="../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring")]
def kv_ring_row(pos, ring : int64) : int64 => ring == 0l ? pos : pos % ring

let RD_UNSPLIT_POS = 512l   //! a whole-model driver's token step runs its attention's unsplit form under this position (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-decode-attn-split`)

//! positions [lo, hi) of a layer whose mirror rows ring at `ring` (0 = flat) as runs back to back in its slab, `blk(p0, n, slot)` a
//! run of n positions from p0 at row `slot`: the span whole on a flat layer; on a ring its last `ring` positions, cut where the slots wrap
[arch(at="../ARCHITECTURE_GPU_KV_RING.md#kv-sliding-ring")]
def kv_ring_runs(lo, hi, ring : int64; blk : block<(p0, n, slot : int64) : void>) {
    return if (hi <= lo)
    if (ring == 0l) {
        invoke(blk, lo, hi - lo, lo)
        return
    }
    var p = max(lo, hi - ring)
    while (p < hi) {
        let slot = kv_ring_row(p, ring)
        let n = min(hi - p, ring - slot)
        invoke(blk, p, n, slot)
        p += n
    }
}

//! tanh clamped to +-15, where f32 tanh already saturates: a device tanh built from exp answers NaN past about +-44, the CPU +-1
def tts_tanh(v : float) : float => tanh(clamp(v, -15.0, 15.0))

//! the logistic sigmoid, one form for the CPU chains and both kernel homes
def sigmoid_f32(v : float) : float => 1.0 / (1.0 + exp(-v))

//! SiLU as the one quotient v / (1 + exp(-v)), the CPU `silu`'s form
def silu_f32(v : float) : float => v / (1.0 + exp(-v))

//! `x` gated by the logistic of `g` as the one quotient x / (1 + exp(-g)), not x times `sigmoid_f32(g)`
def sig_gate(x, g : float) : float => x / (1.0 + exp(-g))

//! the inverse rms of a mean square `ms` (eps added): `rsqrt` where `rsq`, else the CPU rmsnorm's `1 / sqrt`
def rms_inv(ms : float; rsq : bool) : float => rsq ? rsqrt(ms) : 1.0 / sqrt(ms)

//! a logit as the classifier tail's lanes read it: NaN and anything under the lowest finite read as the lowest finite (-3.4e38, the
//! literal because a kernel body reads no module constant) - the kernel's lane and the CPU oracle's one reading
def sel_lane_val(v : float) : float => !(v == v) || v < -3.4e38 ? -3.4e38 : v   // nolint:LINT007 — the NaN test: a NaN is the one float that is not equal to itself

//! the selected routing weights' sum as their renorm divides by it: clamped at f16's smallest normal, the CPU select's floor
def moe_renorm_sum(ws : float) : float => max(ws, 6.103515625e-5)

//! the deltanet out-gate of `z`: sigmoid(z) where `sig` (qwen4exp), else silu(z) (qwen35)
def dn_zgate(z : float; sig : bool) : float => sig ? sigmoid_f32(z) : silu_f32(z)

//! the dot of two quads summed left to right, each step a `mad`
def dot4_seq(a, b : float4) : float => mad(a.w, b.w, mad(a.z, b.z, mad(a.y, b.y, a.x * b.x)))

//! Softplus with the standard large-x shortcut: x > 20 → x, else log(1 + exp(x)).
def softplus(x : float) : float => x > 20.0 ? x : log(1.0 + exp(x))

//! the gated delta rule's decay for a head's `a` = -exp(A_log) and its raw gate plus dt_bias
def dn_decay(a, g : float) : float => exp(a * softplus(g))

//! the CPU `gelu`'s tanh form in f32 (not the f16 LUT form the vision towers read), its tanh clamped by `tts_tanh`
def tts_gelu_tanh(v : float) : float => 0.5 * v * (1.0 + tts_tanh(0.7978845608028654 * mad(0.044715 * v * v, v, v)))

let ACT_SILU = 0u
let ACT_GELU = 1u   //! tanh-gelu
let ACT_OAI = 2u    //! gpt-oss's clamped swiglu (`swiglu_oai`): the clamps and the +1 are the trained activation, not a guard

//! the act kernels' `act` word from a model's flags
def act_code(oai, gelu : bool) : uint => oai ? ACT_OAI : (gelu ? ACT_GELU : ACT_SILU)

//! the gated FFN's one activation formula on both homes - `act` 0 silu, 1 tanh-gelu, 2 the clamped swiglu on v, times u
//! (`ACT_*`; spelled as literals, which the MSL emitter reads where it reads no global)
def gk_act_mul(v, u : float; act : uint) : float {
    if (act == 2u) {
        let x = min(v, 7.0)
        let y = clamp(u, -7.0, 7.0)
        return (x / (1.0 + exp(-1.702 * x))) * (y + 1.0)
    }
    return (act == 1u ? tts_gelu_tanh(v) : silu_f32(v)) * u
}

//! the erf GELU in the CPU `gelu_erf`'s form - the Numerical Recipes erfc - in f32, the one both homes evaluate
def gelu_erf_f32(v : float) : float {
    let z = abs(v) * 0.7071067811865476
    let t = 1.0 / (1.0 + 0.5 * z)
    let erfc = t * exp(-z * z - 1.26551223 + t * (1.00002368 + t * (0.37409196 + t * (0.09678418 +
        t * (-0.18628806 + t * (0.27886807 + t * (-1.13520398 + t * (1.48851587 +
        t * (-0.82215223 + t * 0.17087277)))))))))
    let half_v_erfc = 0.5 * v * erfc
    return v >= 0.0 ? v - half_v_erfc : half_v_erfc
}

//! the fp16-LUT tanh GELU in the CPU table's form (`gelu_tanh_lut`): the +-10 clamps and both f16 round trips in the kernel, the
//! tanh through the exp identity (Metal's tanh NaNs past about 44; the clamped argument reaches 43.7)
def gelu_tanh_lut_f32(v : float) : float {
    if (v <= -10.0) {
        return 0.0
    }
    if (v >= 10.0) {
        return v
    }
    let xr = float(float16(v))
    let gv = 0.5 * xr * (2.0 - 2.0 / (1.0 + exp(2.0 * (0.79788456080286535587989211986876 * xr * (1.0 + 0.044715 * xr * xr)))))
    return float(float16(gv))
}

//! GELU-quick in the CPU table's form (`geglu_quick_lut_rows`): both f16 round trips in the kernel, so the residual stream sees the
//! same activation bits as the CPU lane
def gelu_quick_lut_f32(v : float) : float {
    let xr = float(float16(v))
    let gv = xr * (1.0 / (1.0 + exp(-1.702 * xr)))
    return float(float16(gv))
}

//! a template constant's value where the stamp fixed it (a nonzero literal), else the run-time field: the head width or tap count a stamp serves
def gk_fixed_or(fixed, run : uint) : uint => fixed != 0u ? fixed : run

//! A forward conv's source row for output row r at tap - (r0 + r) * stride + tap * dil - pad_l - or -1 off the input.
def conv_src_fwd_row(r, tap, r0, pad_l, dil, stride, t_in : uint) : int {
    let st = int((r0 + r) * stride + tap * dil) - int(pad_l)
    return (st >= 0 && uint(st) < t_in) ? st : -1
}

//! The transposed conv's: the source row whose stride lands the tap on output row r, or -1; stride at least one.
def conv_src_tr_row(r, tap, r0, pad_l, dil, stride, t_in : uint) : int {
    let q = int(r0 + r + pad_l) - int(tap * dil)
    return (q >= 0 && uint(q) % stride == 0u && uint(q) / stride < t_in) ? int(uint(q) / stride) : -1
}

def tts_resize_mix_torch(x0, x1, l0, l1 : float) : float => mad(l0, x0, l1 * x1)

//! the trilinear mix the onnx-law stamps follow, in its operation order: the last channel through all seven fma terms, the rest their two nonzero ones
def tts_resize_mix_onnx(x0, x1, l0, l1 : float; last : bool) : float {
    if (last) {
        let q0 = (l0 * 0.5) * 0.5
        let q1 = (l1 * 0.5) * 0.5
        var acc = mad(q0, x0, q1 * x1)
        acc = mad(q0, x0, acc)
        acc = mad(q1, x1, acc)
        acc = mad(q0, x0, acc)
        acc = mad(q1, x1, acc)
        acc = mad(q0, x0, acc)
        return mad(q1, x1, acc)
    }
    let w0 = l0 * 0.5
    let w1 = l1 * 0.5
    var acc = mad(w0, x0, w1 * x1)
    acc = mad(w0, x0, acc)
    return mad(w1, x1, acc)
}

//! the CPU `sine_phase_frames`' rad of cycles `cyc` (divided by the caller): its fraction, `nu` folded into the overtones' sample 0
def tts_src_rad(cyc, nu : float; i, h : uint) : float {
    let r = cyc - floor(cyc)
    return (i == 0u && h > 0u) ? r + nu : r
}

//! the linear resample taps the torch-law stamps follow, at output `to` over `n` inputs; `inv_scale` is 1 / scale, divided by the caller its home's way
def tts_resize_taps_torch(n, to : uint; inv_scale : float; var i0, i1 : uint&; var l0, l1 : float&) {
    let src = max(0.0, mad(inv_scale, float(to) + 0.5, -0.5))
    i0 = min(uint(src), n - 1u)
    i1 = min(i0 + 1u, n - 1u)
    l1 = src - float(i0)
    l0 = 1.0 - l1
}

//! the taps the onnx-law stamps follow, at `q` = (to + 0.5) / scale, divided by the caller its home's way; equal weights where the taps coincide
def tts_resize_taps_onnx(n : uint; q : float; var i0, i1 : uint&; var l0, l1 : float&) {
    let in_x = clamp(q - 0.5, 0.0, float(n - 1u))
    i0 = min(uint(in_x), n - 1u)
    i1 = min(i0 + 1u, n - 1u)
    if (i0 == i1) {
        l0 = 0.5
        l1 = 0.5
    } else {
        l0 = abs(in_x - float(i1))
        l1 = abs(in_x - float(i0))
    }
}

//! sin of a phase past a million radians: 2 pi in pieces of five significant bits, so k times a piece is exact
def tts_sin_big(phase : float) : float {
    let k = round(phase * 0.15915493667125702)
    var r = mad(-k, 6.25, phase)
    r = mad(-k, 0.033203125, r)
    r = mad(-k, -1.781781975296326e-05, r)
    r = mad(-k, -6.608047442568932e-13, r)
    return sin(r)
}

def tts_noise_amp(uv, noise_std, sine_amp : float) : float => uv * noise_std + (1.0 - uv) * sine_amp / 3.0

def tts_src_term(w, s, uv, noise_amp, nz : float) : float => w * (s * uv + noise_amp * nz)

//! one two-float step of the torch law's double accumulator, x into `acc` and its rounding error into `lo`; needs a `precise` body
def tts_two_sum_add(var acc, lo : float&; x : float) {
    let s = acc + x
    let bv = s - acc
    lo += (acc - (s - bv)) + (x - bv)
    let s2 = s + lo
    lo = lo - (s2 - s)
    acc = s2
}

//! the phase in radians of the cycles `acc` summed at the low rate, `up` samples a frame
def tts_phase_rad(acc : float; up : uint) : float => ((acc * 2.0) * 3.14159265358979) * float(up)

//! the reflect pad's source index of padded position p in a signal of n samples, mirrored past either end
def tts_pad_reflect(p, n : int) : int {
    if (p < 0) {
        return -p
    }
    return p >= n ? 2 * (n - 1) - p : p
}

//! the CPU `magnitude_phase`'s magnitude
def tts_stft_mag(re, im, eps : float) : float => sqrt(re * re + im * im + eps)

//! the CPU `magnitude_phase`'s phase in the reference's angle convention: a zero imaginary part reads +pi on a negative real, where atan2 reads -pi
def tts_stft_phase(re, im : float) : float => im == 0.0 ? (re < 0.0 ? 3.14159265358979 : 0.0) : atan2(im, re)

//! one bin's inverse-STFT term: magnitude exp(logmag) at phase sin(phrow) through the transposed conv's two basis weights
def tts_istft_term(logmag, phrow, wre, wim : float) : float {
    let mag = exp(logmag)
    let ph = sin(phrow)
    return mag * cos(ph) * wre - mag * sin(ph) * wim
}

//! the CPU `adain_affine`'s fold of the column stats over `tn` rows, the norm's `g` / `gb` and the style's gamma / beta
def tts_adain_affine(mean, sqsum, tn, eps, g, gb, gamma, beta : float; var scale, shift : float&) {
    let rs = 1.0 / sqrt(sqsum / tn + eps)
    let g1 = 1.0 + gamma
    scale = rs * g * g1
    shift = (gb - mean * rs * g) * g1 + beta
}

def tts_ln_affine(x, mean, inv, w, b : float) : float => (x - mean) * inv * w + b

def tts_modulate(v, shift, scale : float) : float => v * (1.0 + scale) + shift

//! the causal window's first key for the query at position p: the last `ctx` keys up to itself, every key at ctx 0
def tts_window_start(p, ctx : uint) : uint => (ctx != 0u && p + 1u > ctx) ? p + 1u - ctx : 0u

//! the source's own noise draw's counter hash
def tts_hash_u32(x : uint) : uint {
    let a = x * 0x9E3779B1u
    let b = (a ^ (a >> 15u)) * 0x85EBCA77u
    let c = (b ^ (b >> 13u)) * 0xC2B2AE3Du
    return c ^ (c >> 16u)
}

//! a counter-keyed Box-Muller normal, not the CPU's PCG32 stream; the seed hashed first, else seeds four apart alias
def tts_hash_normal(seed, idx : uint) : float {
    let sh = tts_hash_u32(seed)
    let a = tts_hash_u32(sh ^ (idx * 2u + 1u))
    let b = tts_hash_u32(sh ^ (idx * 2u + 2u) ^ 0x6A09E667u)
    let u1 = (float(a >> 8u) + 1.0) * (1.0 / 16777217.0)
    let u2 = float(b >> 8u) * (1.0 / 16777216.0)
    return sqrt(-2.0 * log(u1)) * cos(6.28318530717959 * u2)
}

//! a / b rounded as the CPU's IEEE divide: a SPIR-V divide sits up to 2.5 ulp off, which the phase sums carry; the correction lands the same quotient over Metal's IEEE divide
def tts_div(a, b : float) : float {
    var r = 1.0 / b
    r = mad(mad(-b, r, 1.0), r, r)
    let q = a * r
    return mad(mad(-q, b, a), r, q)
}

//! an MXFP4 block's E8M0 scale byte as 2^(e - 128) - half its power of two, paired with the doubled e2m1 magnitudes - built as
//! the f32 bits, the denormal pair e < 2 included; the CPU `e8m0_half`'s value
def e8m0_half_f32(ex : uint) : float => uint_bits_to_float(ex < 2u ? (0x00200000u << ex) : ((ex - 1u) << 23u))

//! an NVFP4 UE4M3 scale byte at HALF its value (`mx4_lut`'s magnitudes are doubled): e = 0 is the subnormal, m x 2^-10 at half, the 0x7F nan code reads as zero
def ue4m3_half_f32(b : uint) : float {
    let e = (b >> 3u) & 15u
    let m = b & 7u
    if (b == 0x7Fu) {
        return 0.0
    }
    if (e == 0u) {
        return float(m) * 0.0009765625
    }
    return uint_bits_to_float(((e + 119u) << 23u) | (m << 20u))
}

//! `v` stored as a half, saturated at the half's largest finite magnitude instead of overflowing to infinity
def half_sat(v : float) : float16 => float16(clamp(v, -65504.0, 65504.0))

let BIAS_ACT_NONE = 0u        //! `GkBiasActArgs.act`: the bias row alone
let BIAS_ACT_GELU_TANH = 1u   //! ... then the tanh-LUT GELU (the vision towers, the whisper family)
let BIAS_ACT_GELU_ERF = 2u    //! ... then the erf GELU (qwen2audio, voxtral, ultravox, the Omni audio towers, Qwen3-ASR)
let BIAS_ACT_SILU = 3u        //! ... then silu (the FastConformer's FFN)
let BIAS_ACT_RELU = 4u        //! ... then relu (the FastConformer's subsample stack)

//! the tower activations by `GkBiasActArgs.act` code - the row stamps' ladder, one free function so either emitter inlines it; the codes
//! are spelled as their literals (the Metal lens admits no global in a kernel): 0 none, 1 the tanh-LUT GELU, 2 the erf GELU, 3 silu, 4 relu
def act_f32(v : float; act : uint) : float {
    if (act == 0u) {
        return v
    }
    if (act == 2u) {
        return gelu_erf_f32(v)
    }
    if (act == 3u) {
        return silu_f32(v)
    }
    if (act == 4u) {
        return max(v, 0.0)
    }
    return gelu_tanh_lut_f32(v)
}

//! the experts one select workgroup reaches: 256 threads, `TOPK_DEPTH_MAX` logits a thread
let TOPK_DEPTH_MAX = 2
let TOPK_EXPERTS_MAX = 256l * int64(TOPK_DEPTH_MAX)

//! the select stamps' depth for `ne` experts: one logit a thread to 256, two past it
def topk_depth(ne : int64) : int => ne > 256l ? 2 : 1

let ROUTER_TP = 64u   //! the router tile's positions per workgroup
let ROUTER_TE = 32u   //! ... and router rows
let ROUTER_TH = ROUTER_TE / 2u   //! an invocation's two rows sit a half-tile apart: e and e + ROUTER_TH
let ROUTER_K4 = 16u   //! a 64-wide K step in float4
let ROUTER_LD = ROUTER_K4 + 1u   //! the stage's row stride in float4: the step plus one of pad, so the sixteen rows a warp reads (and a fetch writes) spread the eight bank groups

//! the router GEMM's workgroups over `npos` rows and `ne` router rows: one per 64 rows x 32 router rows
def router_gemm_wgs(npos, ne : int64) : int64 => ((npos + int64(ROUTER_TP) - 1l) / int64(ROUTER_TP)) * ((ne + int64(ROUTER_TE) - 1l) / int64(ROUTER_TE))

let DN_MAX_DS = 128l
let DN_MAX_HEADS = 64l
let DN_MAX_CD = 16_384l
let DN_MAX_QK = 8_192l   //! the conv row's q + k region (2 kd): the channels the conv's per-head norm covers
let DN_MAX_DCONV = 4l
let DN_WINDOW = 4_096l
//! the scan workspace holds the raw o rows alone: position-major, di wide, rewritten in place by the out-norm
let DN_O_OFF = 0l
let DN_WS_BYTES = DN_WINDOW * DN_MAX_HEADS * DN_MAX_DS * 4l
let DN_STATE_BYTES = DN_MAX_HEADS * DN_MAX_DS * DN_MAX_DS * 4l
let DN_META_BYTES = 128l
let DN_SM_BETA = 16l
let DN_SM_G = DN_SM_BETA + DN_WINDOW * DN_MAX_HEADS
let DN_SM_TAPS = DN_SM_G + DN_WINDOW * DN_MAX_HEADS
let DN_SM_WNORM = DN_SM_TAPS + DN_MAX_CD * DN_MAX_DCONV
let DN_SM_A = DN_SM_WNORM + DN_MAX_DS
let DN_SM_DT = DN_SM_A + DN_MAX_HEADS
let DN_SM_HIST = DN_SM_DT + DN_MAX_HEADS
let DN_SM_SLACK = 64l   //! floats past the last ring image, so a ring's last row never sits at the buffer's end
let DN_SMALLS_BYTES = (DN_SM_HIST + DN_MAX_CD * (DN_MAX_DCONV - 1l) + DN_SM_SLACK) * 4l
let DN_TAIL_BYTES = DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

let DND_BG_BYTES = (2l * DN_MAX_HEADS + 4l) * 4l
let DND_OQ_BYTES = DN_MAX_HEADS * DN_MAX_DS
let DND_OS_BYTES = (DN_MAX_HEADS * DN_MAX_DS / 32l) * 4l
let DND_SMALLS_BYTES = DN_SMALLS_BYTES + DN_MAX_CD * (DN_MAX_DCONV - 1l) * 4l

//! a deltanet geometry inside the kernels' capacities: the state width a power of two of whole 32-lane groups, the heads, conv channels, q/k region and taps under their caps, the widths consistent, the o row inside `DND_OQ_BYTES`
def dn_geom_fits(ds, nvh, nkh, kd, di, cd, dconv : int64) : bool {
    return (ds <= DN_MAX_DS && ds % 32l == 0l && (ds & (ds - 1l)) == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
        && 2l * kd <= DN_MAX_QK && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
        && di % 32l == 0l && di <= DND_OQ_BYTES)
}

[arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
def dn_scan_wgs(nvh, ds : int64) : int64 {
    let cpg = 4l * (32l / (ds / 16l))
    return nvh * ((ds + cpg - 1l) / cpg)
}

let DN_CONV_PB = 32u   //! the conv's positions per workgroup
[arch(at="../ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels")]
def dn_conv_wgs(cd, npos : int64) : int64 => ((cd + 255l) / 256l) * ((npos + int64(DN_CONV_PB) - 1l) / int64(DN_CONV_PB))

//! The prefill's beta/alpha tile GEMM's workgroups: 16-position tiles x 16-output groups over the 2 x nvh rows.
def dn_ba_wgs(npos, nvh : int64) : int64 => ((npos + 15l) / 16l) * ((2l * nvh + 15l) / 16l)

