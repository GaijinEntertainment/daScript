options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_source_kernels

require dastest/testing_boost public
require math
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost

//! The TTS tower's decoder tail on Vulkan against the CPU chain (a Vulkan device, else the cells skip): the stage sum's
//! axpy and the last stage's reflect pad, exact; the harmonic source on both resample laws - the phase frames, the mixed
//! signal, the captured noise and the source's own draw - held to the CPU `sine_source`; the STFT on both pad laws and the
//! inverse STFT with and without the envelope held to their double-precision forms; every output under a NaN fill,
//! every compare with its poisoned element.

let private POISON = 7.5   //! the slots a kernel must never read or write: a large finite value
let private SLACK = 5      //! the elements past a run, left at their fill

//! `n` f16-exact patterned values: (i * seed_mul) % 47 scaled and biased, rounded through f16 - the Metal gates' fixtures
def private f16_fill(n, seed_mul : int; scale, bias : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float(float16(scale * float((i * seed_mul) % 47) + bias))
    }
    return <- a
}

//! `n` slots of `POISON`
def private poison_plane(n : int) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        v = POISON
    }
    return <- a
}

//! `a` written into `plane` from element `off`
def private put_at(var plane : array<float>; off : int; a : array<float>) {
    for (i in range(length(a))) {
        plane[off + i] = a[i]
    }
}

//! within `absf` absolute or `rel` relative of the larger magnitude (floored at 1) - the Metal gates' two-bar test
def private bar_ok(g, w, rel, absf : float) : bool => abs(g - w) <= absf || abs(g - w) <= rel * max(1.0, max(abs(g), abs(w)))

//! `w` moved by a hundredth of its magnitude (floored at 1): outside every bar in this file
def private poison_of(w : float) : float => w + max(1.0, abs(w)) * 0.01

//! the elements of `got` outside the bars, the first six logged, and the widest miss logged with its row and column in
//! rows `width` wide
def private bars_bad(tag : string; got, want : array<float>; rel, absf : float; width : int) : int {
    var bad = 0
    var worst = 0.0
    var at = 0
    for (i in range(length(want))) {
        let d = abs(got[i] - want[i])
        if (d > worst) {
            worst = d
            at = i
        }
        if (!bar_ok(got[i], want[i], rel, absf)) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  {tag} mismatch [{i}] gpu {got[i]} cpu {want[i]}\n")
            }
            bad++
        }
    }
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {worst} at row {at / width} column {at % width}\n")
    return bad
}

//! the NaN elements of `a`: the sentinel words a dispatch left
def private nan_count(a : array<float>) : int {
    var n = 0
    for (v in a) {
        if (is_nan(v)) {
            n++
        }
    }
    return n
}

//! true when every element of `a` is bit-equal to `v`
def private elem_all(a : array<float>; v : float) : bool {
    for (e in a) {
        if (e != v) {
            return false
        }
    }
    return true
}

//! a device buffer holding `a` whole
def private dev_upload(a : array<auto(TT)>) : uint64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let bytes = long_length(a) * int64(typeinfo sizeof(type<TT>))
        let dev = make_device_buf(bytes)
        upload_region_at(dev, 0l, unsafe(addr<uint8 const?>(a[0])), bytes)
        return dev
    } else {
        return 0ul
    }
}

// ===== the stage sum's axpy =====

let private AX_N = 1030        //! four whole workgroups and six more
let private AX_OOFF = 3
let private AX_YOFF = 7
let private AX_SCALE = 0.25    //! a power of two: scale * y is exact, so the fused and the unfused form both round once, as the f32 form does

[test]
def test_vkt_axpy(t0 : T?) {
    t0 |> run("the axpy == o + 0.25 y in f32, exact, accumulating (zero = 0) and overwriting (zero = 1), at offsets") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_axpy_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_OA = 1u
            let B_OZ = 2u
            let n = AX_N
            let ob <- f16_fill(n, 7, 0.05, -1.1)
            let yv <- f16_fill(n, 5, 0.04, -0.9)
            var op <- poison_plane(AX_OOFF + n + SLACK)
            put_at(op, AX_OOFF, ob)
            var yp <- poison_plane(AX_YOFF + n)
            put_at(yp, AX_YOFF, yv)
            let oad = dev_upload(op)
            let yd = dev_upload(yp)
            let bo = long_length(op) * 4l
            let by = long_length(yp) * 4l
            let ozd = make_device_buf(bo)
            nan_sentinel_upload(ozd, bo)
            let sizes = fixed_array(bo, bo)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            var s_a = set_tts_axpy_cls(fixed_array(oad, yd), fixed_array(bo, by), fixed_array(B_OA, 0u))
            var s_z = set_tts_axpy_cls(fixed_array(ozd, yd), fixed_array(bo, by), fixed_array(B_OZ, 0u))
            var pa = TtsAxpyArgs(nelem = uint(n), scale = AX_SCALE, zero = 0u, ooff = uint(AX_OOFF), yoff = uint(AX_YOFF))
            var pz = pa
            pz.zero = 1u
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_axpy_cls(raw, h, s_a, pa, tts_axpy_wgs(int64(n)))
                enc_tts_axpy_cls(raw, h, s_z, pz, tts_axpy_wgs(int64(n)))
                cmd_copy_planes(raw, h, fixed_array(oad, ozd), host, sizes, B_OA | B_OZ)
            }
            let got_a <- staged(host.mapped, at[0] + int64(AX_OOFF) * 4l, n, type<float>)
            let pre_a <- staged(host.mapped, at[0], AX_OOFF, type<float>)
            let tail_a <- staged(host.mapped, at[0] + int64(AX_OOFF + n) * 4l, SLACK, type<float>)
            let got_z <- staged(host.mapped, at[1] + int64(AX_OOFF) * 4l, n, type<float>)
            let pre_z <- staged(host.mapped, at[1], AX_OOFF, type<float>)
            let tail_z <- staged(host.mapped, at[1] + int64(AX_OOFF + n) * 4l, SLACK, type<float>)
            let want_a <- [for (i in range(n)); ob[i] + AX_SCALE * yv[i]]
            let want_z <- [for (i in range(n)); AX_SCALE * yv[i]]
            let bad_a = mismatch_exact(got_a, want_a)
            t |> success(bad_a == 0, "tts_axpy zero = 0: every element == o + 0.25 y ({bad_a} of {n} off)")
            t |> success(got_a[n - 1] != poison_of(want_a[n - 1]), "tts_axpy zero = 0: the bar reds a poisoned element")
            let bad_z = mismatch_exact(got_z, want_z)
            t |> success(bad_z == 0, "tts_axpy zero = 1: every element == 0.25 y over a NaN-filled o ({bad_z} of {n} off)")
            t |> success(got_z[0] != poison_of(want_z[0]), "tts_axpy zero = 1: the bar reds a poisoned element")
            t |> success(ob[0] != 0.0 && got_a[0] != got_z[0], "tts_axpy control: the same dispatch at zero = 0 adds o's old contents")
            t |> success(elem_all(pre_a, POISON) && elem_all(tail_a, POISON), "tts_axpy zero = 0: the elements outside the run are untouched")
            t |> equal(nan_count(pre_z) + nan_count(tail_z), AX_OOFF + SLACK, "tts_axpy zero = 1: the elements outside the run keep their sentinel")
            destroy_device_buf(oad)
            destroy_device_buf(ozd)
            destroy_device_buf(yd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the last stage's reflect pad =====

let private RF_T = 37
let private RF_C = 24

[test]
def test_vkt_reflect1(t0 : T?) {
    t0 |> run("the reflect pad y [38][24] == x [37][24] behind x's row 1, exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_reflect1_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_Y = 1u
            let total = (RF_T + 1) * RF_C
            let x <- f16_fill(RF_T * RF_C, 11, 0.03, -0.7)
            let xd = dev_upload(x)
            let bx = long_length(x) * 4l
            let by = int64(total + SLACK) * 4l
            let yd = make_device_buf(by)
            nan_sentinel_upload(yd, by)
            var host = make_host_buf(by, true, [cached = true])
            var s = set_tts_reflect1_cls(fixed_array(xd, yd), fixed_array(bx, by), fixed_array(0u, B_Y))
            var pa = TtsReflectArgs(c = uint(RF_C), total = uint(total))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_reflect1_cls(raw, h, s, pa, tts_reflect1_wgs(int64(total)))
                cmd_copy_planes(raw, h, fixed_array(yd), host, fixed_array(by), B_Y)
            }
            let got <- staged(host.mapped, 0l, total, type<float>)
            let tail <- staged(host.mapped, int64(total) * 4l, SLACK, type<float>)
            let want <- [for (i in range(total)); x[(i / RF_C == 0 ? 1 : i / RF_C - 1) * RF_C + i % RF_C]]
            let bad = mismatch_exact(got, want)
            t |> success(bad == 0, "tts_reflect1: row 0 == x's row 1, row r == x's row r - 1 over {RF_T + 1} x {RF_C} ({bad} of {total} off)")
            t |> success(got[RF_C + 1] != poison_of(want[RF_C + 1]), "tts_reflect1: the bar reds a poisoned element")
            t |> success(got[0] != x[0], "tts_reflect1 control: row 0 is x's row 1, not x's row 0")
            t |> equal(nan_count(tail), SLACK, "tts_reflect1: the elements past (t + 1) * c keep their sentinel")
            destroy_device_buf(xd)
            destroy_device_buf(yd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the harmonic source =====

let private SRC_NH = 3
let private SRC_SR = 24000.0
let private SRC_WOFF = 5        //! the source linear's weight row in the slab plane, poisoned slots before it
let private SRC_GAP = 3         //! poisoned slots between the weight row and the bias
let private LOWC_REL = 1.2e-7   //! one ulp of a phase frame: the device's increments and sums in the CPU carry's operation order
let private HAR_REL = 2e-6      //! the mixed signal: the device sin, tanh and the linear's reassociation against the CPU's
let private HAR_ABS = 2e-7
let private NOISE_REL = 1e-5    //! the own draw: the device log, sqrt and cos against the CPU's on the same hashed uniforms, under 5 in magnitude
let private NOISE_ABS = 1e-6

struct private SrcFixture {
    up : int
    frames : int
    n : int
    nlow : int
    rows : int64                //! the samples the CPU source mixed
    f0 : array<float>
    nu : array<float>
    noise : array<float>        //! the captured noise rows [n][nh]
    wn : array<float>           //! the slab plane: lw [nh] at SRC_WOFF, lb after SRC_GAP poisoned slots, four poisoned past it
    want_lowc : array<float>    //! the CPU carry's phase frames [nh][nlow]
    want_har : array<float>     //! the CPU sines through the source linear and tanh
}

struct private SrcOut {
    lowc : array<float>
    har : array<float>
    har_tail : array<float>     //! the `SLACK` elements past n
    noise : array<float>
}

def private src_boff : int => SRC_WOFF + SRC_NH + SRC_GAP

//! the Metal `st2_source_gate`'s fixture at `up` x `frames`, with the CPU chain's phase frames and mixed signal
def private src_fixture(torch : bool; up, frames : int) : SrcFixture {
    var f = SrcFixture(up = up, frames = frames, n = frames * up)
    f.nlow = int(resize_len(int64(f.n), 1.0 / float(up)))
    f.f0 |> resize(frames)
    for (i in range(frames)) {
        f.f0[i] = i % 3 == 2 ? 0.0 : float(float16(180.0 + 0.3 * float(i % 97)))
    }
    f.nu <- f16_fill(SRC_NH, 3, 0.1, 0.2)
    f.noise <- f16_fill(f.n * SRC_NH, 7, 0.4, -0.9)
    var lin = TtsLinear(nin = int64(SRC_NH), nout = 1l)
    lin.w <- f16_fill(SRC_NH, 5, 0.3, 0.2)
    lin.b |> push(0.05)
    f.wn <- poison_plane(src_boff() + 1 + 4)
    put_at(f.wn, SRC_WOFF, lin.w)
    f.wn[src_boff()] = lin.b[0]
    let cfg = SineSourceCfg(upsample = int64(up), sample_rate = SRC_SR, n_harm = int64(SRC_NH), sine_amp = 0.1, noise_std = 0.003,
        voiced_thr = 10.0, torch_math = torch)
    var cr = SineSourceCarry()
    var sines : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        sine_carry_reset(cr, int64(SRC_NH), int64(f.nlow))
        f.rows = sine_source(cfg, f.f0, int64(frames), f.nu, 0l, int64(f.nlow), cr, f.noise, 0l, sines)
        linear_rows(lin, sines, int64(f.n), f.want_har)
        tanh_inplace(f.want_har, int64(f.n))
    }
    f.want_lowc |> resize(SRC_NH * f.nlow)
    for (h in range(SRC_NH)) {
        for (k in range(f.nlow)) {
            f.want_lowc[h * f.nlow + k] = cr.lowc[int64(h) * cr.stride + int64(k)]
        }
    }
    delete cr
    delete sines
    delete lin
    return <- f
}

//! the source chain on the device - the own draw at `seed` first when `own`, else the captured rows - low, cumsum and
//! sines on the law's stamps, every written buffer under a NaN fill first
def private src_run(f : SrcFixture; torch : bool; f0 : array<float>; own : bool; seed : uint) : SrcOut {
    var o : SrcOut
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_LOW = 1u
        let B_LOWC = 2u
        let B_HAR = 4u
        let B_NZ = 8u
        let f0d = dev_upload(f0)
        let nud = dev_upload(f.nu)
        let wnd = dev_upload(f.wn)
        let bnz = int64(f.n * SRC_NH) * 4l
        let nzd = own ? make_device_buf(bnz) : dev_upload(f.noise)
        if (own) {
            nan_sentinel_upload(nzd, bnz)
        }
        let blow = int64(SRC_NH * f.nlow) * 4l
        let bhar = int64(f.n + SLACK) * 4l
        let lowd = make_device_buf(blow)
        let lowcd = make_device_buf(blow)
        let hard = make_device_buf(bhar)
        nan_sentinel_upload(lowd, blow)
        nan_sentinel_upload(lowcd, blow)
        nan_sentinel_upload(hard, bhar)
        let bf0 = long_length(f0) * 4l
        let bnu = long_length(f.nu) * 4l
        let bwn = long_length(f.wn) * 4l
        let sizes = fixed_array(blow, bhar, bnz)
        let at <- staged_offsets(sizes)
        var host = make_host_buf(at[length(sizes)], true, [cached = true])
        var pa = TtsSourceArgs(n = uint(f.n), nlow = uint(f.nlow), nh = uint(SRC_NH), up = uint(f.up), sr = SRC_SR, sine_amp = 0.1,
            noise_std = 0.003, voiced_thr = 10.0, seed = seed, woff = uint(SRC_WOFF), boff = uint(src_boff()))
        var s_nz = set_tts_src_noise_cls(fixed_array(nzd), fixed_array(bnz), fixed_array(B_NZ))
        let low_devs = fixed_array(f0d, nud, lowd)
        let low_sizes = fixed_array(bf0, bnu, blow)
        let low_bits = fixed_array(0u, 0u, B_LOW)
        var s_low = torch ? set_tts_src_low_torch_cls(low_devs, low_sizes, low_bits) : set_tts_src_low_onnx_cls(low_devs, low_sizes, low_bits)
        let cum_devs = fixed_array(lowd, lowcd)
        let cum_sizes = fixed_array(blow, blow)
        let cum_bits = fixed_array(B_LOW, B_LOWC)
        var s_cum = torch ? set_tts_src_cumsum_torch_cls(cum_devs, cum_sizes, cum_bits) : set_tts_src_cumsum_onnx_cls(cum_devs, cum_sizes, cum_bits)
        let sin_devs = fixed_array(f0d, lowcd, nzd, wnd, hard)
        let sin_sizes = fixed_array(bf0, blow, bnz, bwn, bhar)
        let sin_bits = fixed_array(0u, B_LOWC, B_NZ, 0u, B_HAR)
        var s_sin = torch ? set_tts_src_sines_torch_cls(sin_devs, sin_sizes, sin_bits) : set_tts_src_sines_onnx_cls(sin_devs, sin_sizes, sin_bits)
        let low_wgs = tts_src_low_wgs(int64(SRC_NH * f.nlow))
        let sin_wgs = tts_src_sines_wgs(int64(f.n))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            if (own) {
                enc_tts_src_noise_cls(raw, h, s_nz, pa, tts_src_noise_wgs(int64(f.n * SRC_NH)))
            }
            if (torch) {
                enc_tts_src_low_torch_cls(raw, h, s_low, pa, low_wgs)
                enc_tts_src_cumsum_torch_cls(raw, h, s_cum, pa, 1l)
                enc_tts_src_sines_torch_cls(raw, h, s_sin, pa, sin_wgs)
            } else {
                enc_tts_src_low_onnx_cls(raw, h, s_low, pa, low_wgs)
                enc_tts_src_cumsum_onnx_cls(raw, h, s_cum, pa, 1l)
                enc_tts_src_sines_onnx_cls(raw, h, s_sin, pa, sin_wgs)
            }
            cmd_copy_planes(raw, h, fixed_array(lowcd, hard, nzd), host, sizes, B_LOWC | B_HAR | B_NZ)
        }
        o.lowc <- staged(host.mapped, at[0], SRC_NH * f.nlow, type<float>)
        o.har <- staged(host.mapped, at[1], f.n, type<float>)
        o.har_tail <- staged(host.mapped, at[1] + int64(f.n) * 4l, SLACK, type<float>)
        o.noise <- staged(host.mapped, at[2], f.n * SRC_NH, type<float>)
        destroy_device_buf(f0d)
        destroy_device_buf(nud)
        destroy_device_buf(wnd)
        destroy_device_buf(nzd)
        destroy_device_buf(lowd)
        destroy_device_buf(lowcd)
        destroy_device_buf(hard)
        destroy_host_buf(host)
    }
    return <- o
}

//! the phase frames and the mixed signal against the CPU chain at their bars, the poisoned element on each
def private src_asserts(t : T?; tag : string; f : SrcFixture; o : SrcOut) {
    let nl = SRC_NH * f.nlow
    let bad_l = bars_bad("{tag} lowc", o.lowc, f.want_lowc, LOWC_REL, 0.0, f.nlow)
    t |> success(bad_l == 0, "{tag}: the phase frames within one ulp of the CPU carry ({bad_l} of {nl} off)")
    t |> success(!bar_ok(o.lowc[nl - 1], poison_of(f.want_lowc[nl - 1]), LOWC_REL, 0.0), "{tag}: the lowc bar reds a poisoned element")
    let bad_h = bars_bad("{tag} har", o.har, f.want_har, HAR_REL, HAR_ABS, f.up)
    t |> success(bad_h == 0, "{tag}: the mixed signal within 2e-6 of the CPU sines through the linear and tanh ({bad_h} of {f.n} off)")
    t |> success(!bar_ok(o.har[f.n - 1], poison_of(f.want_har[f.n - 1]), HAR_REL, HAR_ABS), "{tag}: the har bar reds a poisoned element")
    t |> equal(nan_count(o.har_tail), SLACK, "{tag}: the elements past n keep their sentinel")
}

//! the long cumsum: 40000 low-rate frames of one harmonic, where a plain f32 sum and a double one part by cycles
def private long_cumsum_arm(t : T?; tag : string; torch : bool; up : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_LC = 1u
        let nlong = 40000
        var lowl : array<float>
        var wantl : array<float>
        lowl |> resize(nlong)
        wantl |> resize(nlong)
        var acc64 = 0.0lf
        var accf = 0.0
        var accp = 0.0
        for (k in range(nlong)) {
            lowl[k] = float(float16(0.0071 + 0.0003 * float(k % 13)))
            acc64 += double(lowl[k])
            accf = torch ? float(acc64) : accf + lowl[k]
            accp += lowl[k]
            wantl[k] = ((accf * 2.0) * PI) * float(up)
        }
        t |> success(abs(float(acc64) - accp) > 0.01, "{tag} long cumsum: the two laws part by {abs(float(acc64) - accp)} cycles over {nlong} frames (the leg's control)")
        let ld = dev_upload(lowl)
        let bl = long_length(lowl) * 4l
        let lcd = make_device_buf(bl)
        nan_sentinel_upload(lcd, bl)
        var host = make_host_buf(bl, true, [cached = true])
        let devs = fixed_array(ld, lcd)
        let szs = fixed_array(bl, bl)
        let bits = fixed_array(0u, B_LC)
        var s = torch ? set_tts_src_cumsum_torch_cls(devs, szs, bits) : set_tts_src_cumsum_onnx_cls(devs, szs, bits)
        var pa = TtsSourceArgs(n = uint(nlong * up), nlow = uint(nlong), nh = 1u, up = uint(up))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            if (torch) {
                enc_tts_src_cumsum_torch_cls(raw, h, s, pa, 1l)
            } else {
                enc_tts_src_cumsum_onnx_cls(raw, h, s, pa, 1l)
            }
            cmd_copy_planes(raw, h, fixed_array(lcd), host, fixed_array(bl), B_LC)
        }
        let got <- staged(host.mapped, 0l, nlong, type<float>)
        let bad = bars_bad("{tag} long cumsum", got, wantl, 2e-7, 1e-3, nlong)
        t |> success(bad == 0, "{tag} long cumsum: every frame within the bars of the law's sum ({bad} of {nlong} off)")
        t |> success(!bar_ok(got[nlong - 1], wantl[nlong - 1] + 2.0, 2e-7, 1e-3), "{tag} long cumsum: a moved last frame reds the compare")
        destroy_device_buf(ld)
        destroy_device_buf(lcd)
        destroy_host_buf(host)
    }
}

//! the source's own draw at seed 77: the rows against the CPU hash, the signal finite and in tanh's range, repeatable,
//! apart from the captured rows' signal, and moved by seeds 78 and 81 (81 = 77 xor 4, which a raw-seed xor would alias)
def private own_draw_arm(t : T?; tag : string; torch : bool; f : SrcFixture; cap : SrcOut) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nz = f.n * SRC_NH
        var own1 <- src_run(f, torch, f.f0, true, 77u)
        let want_noise <- [for (i in range(nz)); tts_hash_normal(77u, uint(i))]
        let bad = bars_bad("{tag} own draw", own1.noise, want_noise, NOISE_REL, NOISE_ABS, SRC_NH)
        t |> success(bad == 0, "{tag}: the own draw == the CPU hash at seed 77 ({bad} of {nz} off)")
        t |> success(!bar_ok(own1.noise[nz - 1], poison_of(want_noise[nz - 1]), NOISE_REL, NOISE_ABS), "{tag}: the own-draw bar reds a poisoned element")
        var in_range = true
        for (v in own1.har) {
            in_range = in_range && abs(v) <= 1.0
        }
        t |> success(in_range, "{tag}: the own draw's signal is finite and within tanh's range")
        var apart = 0
        for (i in range(f.n)) {
            if (!bar_ok(own1.har[i], cap.har[i], HAR_REL, HAR_ABS)) {
                apart++
            }
        }
        t |> success(apart > 0, "{tag}: the own draw's signal differs from the captured rows' ({apart} of {f.n} samples apart)")
        var own2 <- src_run(f, torch, f.f0, true, 77u)
        t |> equal(mismatch_exact(own2.har, own1.har), 0, "{tag}: the own draw repeats bit for bit at one seed")
        for (other in fixed_array(78u, 81u)) {
            var own3 <- src_run(f, torch, f.f0, true, other)
            var moved = 0
            for (i in range(f.n)) {
                if (!bar_ok(own3.har[i], own1.har[i], HAR_REL, HAR_ABS)) {
                    moved++
                }
            }
            t |> success(moved > 0, "{tag}: seed {other} draws other noise than seed 77 ({moved} of {f.n} samples moved)")
            delete own3
        }
        delete own1
        delete own2
    }
}

//! one source shape on one law: the captured rows against the CPU chain, a poisoned f0 frame that must red both compares,
//! and under `full` the long cumsum and the own draw
def private source_arm(t : T?; torch : bool; up, frames : int; full : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_source {torch ? "torch" : "onnx"} law up{up} x{frames}"
        var f <- src_fixture(torch, up, frames)
        t |> equal(f.rows, int64(f.n), "{tag}: the CPU source mixes every sample")
        var cap <- src_run(f, torch, f.f0, false, 0u)
        src_asserts(t, tag, f, cap)
        var f0p := f.f0
        f0p[4] += 40.0   //! a voiced frame: every later phase moves
        var moved <- src_run(f, torch, f0p, false, 0u)
        let nl = SRC_NH * f.nlow
        t |> success(bars_bad("{tag} lowc poison (MUST mismatch)", moved.lowc, f.want_lowc, LOWC_REL, 0.0, f.nlow) > 0, "{tag}: a poisoned f0 frame reds the lowc compare over {nl} frames")
        t |> success(bars_bad("{tag} har poison (MUST mismatch)", moved.har, f.want_har, HAR_REL, HAR_ABS, f.up) > 0, "{tag}: a poisoned f0 frame reds the har compare")
        if (full) {
            long_cumsum_arm(t, tag, torch, up)
            own_draw_arm(t, tag, torch, f, cap)
        }
        delete f
        delete cap
        delete moved
        delete f0p
    }
}

//! every source stamp's pipeline, the device first
def private src_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return (vk_moe_init() && ensure_tts_src_low_torch_cls() && ensure_tts_src_low_onnx_cls() && ensure_tts_src_cumsum_torch_cls()
            && ensure_tts_src_cumsum_onnx_cls() && ensure_tts_src_noise_cls() && ensure_tts_src_sines_torch_cls() && ensure_tts_src_sines_onnx_cls())
    } else {
        return false
    }
}

[test]
def test_vkt_source(t0 : T?) {
    for (torch in fixed_array(true, false)) {
        let law = torch ? "torch" : "onnx"
        t0 |> run("the harmonic source, {law} law, up 300 x 700 frames: phase frames, mixed signal, long cumsum, own draw") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!src_ready()) {
                    t |> skip("no Vulkan device")
                    return
                }
                source_arm(t, torch, 300, 700, true)
            } else {
                t |> skip("dasVulkan not present")
            }
        }
        t0 |> run("the harmonic source, {law} law, up 2 x 37 frames: the first taps read sample 0, the initial phases in") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!src_ready()) {
                    t |> skip("no Vulkan device")
                    return
                }
                source_arm(t, torch, 2, 37, false)
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

// ===== the STFT =====

let private ST_N = 40
let private ST_K = 8
let private ST_HOP = 4
let private ST_PAD = 2
let private ST_BINS = 4        //! the Metal gate's three bins and a fourth whose imaginary row is zero: the zero-imaginary phase rule
let private ST_REOFF = 6       //! the real basis in the slab plane, poisoned slots before it
let private ST_GAP = 3         //! poisoned slots between the two bases
let private SPEC_REL = 1e-4    //! the device atan2, sqrt and f32 tap sums against the double sums rounded once
let private SPEC_ABS = 1e-5

def private st_frames : int => (ST_N + 2 * ST_PAD - ST_K) / ST_HOP + 1

//! the STFT in double - the taps summed over the padded signal, the magnitude and phase rounded to f32 as the Metal gate holds it
def private stft_ref(har, wre, wim : array<float>; reflect : bool) : array<float> {
    let frames = st_frames()
    var want : array<float>
    want |> resize(frames * 2 * ST_BINS)
    for (f in range(frames)) {
        for (b in range(ST_BINS)) {
            var re = 0.0lf
            var im = 0.0lf
            for (tap in range(ST_K)) {
                var src = f * ST_HOP + tap - ST_PAD
                if (src < 0) {
                    src = reflect ? -src : 0
                } elif (src >= ST_N) {
                    src = reflect ? 2 * (ST_N - 1) - src : ST_N - 1
                }
                re += double(wre[b * ST_K + tap]) * double(har[src])
                im += double(wim[b * ST_K + tap]) * double(har[src])
            }
            want[f * 2 * ST_BINS + b] = float(sqrt(re * re + im * im + 1e-9lf))
            let imf = float(im)
            let ref = float(re)
            want[f * 2 * ST_BINS + ST_BINS + b] = imf == 0.0 ? (ref < 0.0 ? PI : 0.0) : atan2(imf, ref)
        }
    }
    return <- want
}

//! one pad law: the spectrum against the double form, then the signal with the first frame's reflected sample moved, which
//! must red the compare
def private stft_arm(t : T?; reflect : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = reflect ? "tts_stft reflect" : "tts_stft edge"
        let B_S = 1u
        let frames = st_frames()
        let nb = ST_BINS * ST_K
        let nspec = frames * 2 * ST_BINS
        var har <- f16_fill(ST_N, 7, 0.05, -0.9)
        let wre <- f16_fill(nb, 5, 0.2, -0.7)
        var wim <- f16_fill(nb, 3, 0.15, -0.5)
        wim[2 * ST_K + 5] = 0.0
        for (tap in range(ST_K)) {
            wim[(ST_BINS - 1) * ST_K + tap] = 0.0
        }
        let imoff = ST_REOFF + nb + ST_GAP
        var wn <- poison_plane(imoff + nb + 4)
        put_at(wn, ST_REOFF, wre)
        put_at(wn, imoff, wim)
        let want <- stft_ref(har, wre, wim, reflect)
        var neg = 0
        var pos = 0
        for (f in range(frames)) {
            let ph = want[f * 2 * ST_BINS + 2 * ST_BINS - 1]
            neg += ph == PI ? 1 : 0
            pos += ph == 0.0 ? 1 : 0
        }
        t |> success(neg > 0 && pos > 0, "{tag}: the zero-imaginary bin reads +pi on {neg} frames and 0 on {pos}")
        var harp := har
        harp[1] += 2.0   //! the first frame's reflected tap reads it
        let hd = dev_upload(har)
        let hpd = dev_upload(harp)
        let wd = dev_upload(wn)
        let bh = long_length(har) * 4l
        let bw = long_length(wn) * 4l
        let bs = int64(nspec + SLACK) * 4l
        let sd = make_device_buf(bs)
        let spd = make_device_buf(bs)
        nan_sentinel_upload(sd, bs)
        nan_sentinel_upload(spd, bs)
        let sizes = fixed_array(bs, bs)
        let at <- staged_offsets(sizes)
        var host = make_host_buf(at[length(sizes)], true, [cached = true])
        let bits = fixed_array(0u, 0u, B_S)
        let szs = fixed_array(bh, bw, bs)
        var s = reflect ? set_tts_stft_reflect_cls(fixed_array(hd, wd, sd), szs, bits) : set_tts_stft_edge_cls(fixed_array(hd, wd, sd), szs, bits)
        var sp = reflect ? set_tts_stft_reflect_cls(fixed_array(hpd, wd, spd), szs, bits) : set_tts_stft_edge_cls(fixed_array(hpd, wd, spd), szs, bits)
        var pa = TtsStftArgs(n = uint(ST_N), frames = uint(frames), bins = uint(ST_BINS), k = uint(ST_K), hop = uint(ST_HOP), pad = uint(ST_PAD),
            eps = 1e-9, reoff = uint(ST_REOFF), imoff = uint(imoff))
        let wgs = tts_stft_wgs(int64(frames * ST_BINS))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            if (reflect) {
                enc_tts_stft_reflect_cls(raw, h, s, pa, wgs)
                enc_tts_stft_reflect_cls(raw, h, sp, pa, wgs)
            } else {
                enc_tts_stft_edge_cls(raw, h, s, pa, wgs)
                enc_tts_stft_edge_cls(raw, h, sp, pa, wgs)
            }
            cmd_copy_planes(raw, h, fixed_array(sd, spd), host, sizes, B_S)
        }
        let got <- staged(host.mapped, at[0], nspec, type<float>)
        let tail <- staged(host.mapped, at[0] + int64(nspec) * 4l, SLACK, type<float>)
        let gotp <- staged(host.mapped, at[1], nspec, type<float>)
        let bad = bars_bad(tag, got, want, SPEC_REL, SPEC_ABS, 2 * ST_BINS)
        t |> success(bad == 0, "{tag}: {frames} frames x {ST_BINS} bins of magnitude and phase == the double form ({bad} of {nspec} off)")
        t |> success(!bar_ok(got[nspec - 1], poison_of(want[nspec - 1]), SPEC_REL, SPEC_ABS), "{tag}: the bar reds a poisoned element")
        t |> success(bars_bad("{tag} poison (MUST mismatch)", gotp, want, SPEC_REL, SPEC_ABS, 2 * ST_BINS) > 0, "{tag}: a poisoned sample reds the compare")
        t |> equal(nan_count(tail), SLACK, "{tag}: the elements past the spectrum keep their sentinel")
        destroy_device_buf(hd)
        destroy_device_buf(hpd)
        destroy_device_buf(wd)
        destroy_device_buf(sd)
        destroy_device_buf(spd)
        destroy_host_buf(host)
    }
}

[test]
def test_vkt_stft(t0 : T?) {
    for (reflect in fixed_array(true, false)) {
        t0 |> run("the STFT, {reflect ? "reflect" : "edge"} pad, 10 frames x 4 bins == the double form") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!vk_moe_init() || !ensure_tts_stft_reflect_cls() || !ensure_tts_stft_edge_cls()) {
                    t |> skip("no Vulkan device")
                    return
                }
                t |> equal(st_frames(), 10, "the frame count the name claims")
                stft_arm(t, reflect)
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

// ===== the inverse STFT =====

let private IS_TT = 9
let private IS_STRIDE = 8     //! the bins' magnitude and phase columns and two past them
let private IS_BINS = 3
let private IS_K = 8
let private IS_HOP = 4
let private IS_PAD = 2
let private IS_REOFF = 5      //! the real basis in the slab plane, poisoned slots before it
let private IS_GAP = 3        //! poisoned slots between the planes
let private WAVE_REL = 1e-4   //! the device exp, sin, cos and f32 overlap-add against the double sum rounded once
let private WAVE_ABS = 1e-5

def private is_n : int => (IS_TT - 1) * IS_HOP + IS_K - 2 * IS_PAD

struct private IstftRef {
    want : array<float>
    bare : int          //! the samples whose envelope is negligible, left undivided
}

//! the overlap-added inverse in double as the Metal gate holds it, the envelope divided out under `envelope`
def private istft_ref(y, wre, wim, window : array<float>; envelope : bool) : IstftRef {
    let n = is_n()
    var r : IstftRef
    r.want |> resize(n)
    for (i in range(n)) {
        let m = i + IS_PAD
        var acc = 0.0lf
        var env = 0.0lf
        for (tap in range(IS_K)) {
            continue if (m < tap || (m - tap) % IS_HOP != 0)
            let q = (m - tap) / IS_HOP
            continue if (q >= IS_TT)
            for (b in range(IS_BINS)) {
                let mag = exp(y[q * IS_STRIDE + b])
                let ph = sin(y[q * IS_STRIDE + IS_BINS + b])
                acc += double(mag * cos(ph) * wre[tap * IS_BINS + b]) - double(mag * sin(ph) * wim[tap * IS_BINS + b])
            }
            env += double(window[tap] * window[tap])
        }
        r.bare += env > 1e-11lf ? 0 : 1
        r.want[i] = float((envelope && env > 1e-11lf) ? acc / env : acc)
    }
    return <- r
}

//! one envelope mode: the wave against the double form, then a moved log magnitude, which must red the compare
def private istft_arm(t : T?; envelope : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = envelope ? "tts_istft envelope" : "tts_istft plain"
        let B_W = 1u
        let n = is_n()
        let nb = IS_K * IS_BINS
        var y <- f16_fill(IS_TT * IS_STRIDE, 7, 0.15, -1.2)
        let wre <- f16_fill(nb, 5, 0.2, -0.7)
        let wim <- f16_fill(nb, 3, 0.15, -0.5)
        var window <- f16_fill(IS_K, 11, 0.1, 0.3)
        window[1] = 0.0   //! taps 1 and 5 carry every sample m = 1 mod 4: their envelope is zero
        window[5] = 0.0
        let imoff = IS_REOFF + nb + IS_GAP
        let wnoff = imoff + nb + IS_GAP
        var wn <- poison_plane(wnoff + IS_K + 4)
        put_at(wn, IS_REOFF, wre)
        put_at(wn, imoff, wim)
        put_at(wn, wnoff, window)
        var ref <- istft_ref(y, wre, wim, window, envelope)
        t |> success(ref.bare > 0 && ref.bare < n, "{tag}: {ref.bare} of {n} samples carry a negligible envelope")
        var yp := y
        yp[4 * IS_STRIDE + 1] += 2.0   //! a log magnitude: every sample its frame overlaps moves
        let yd = dev_upload(y)
        let ypd = dev_upload(yp)
        let wd = dev_upload(wn)
        let byy = long_length(y) * 4l
        let bw = long_length(wn) * 4l
        let bo = int64(n + SLACK) * 4l
        let od = make_device_buf(bo)
        let opd = make_device_buf(bo)
        nan_sentinel_upload(od, bo)
        nan_sentinel_upload(opd, bo)
        let sizes = fixed_array(bo, bo)
        let at <- staged_offsets(sizes)
        var host = make_host_buf(at[length(sizes)], true, [cached = true])
        var s = set_tts_istft_cls(fixed_array(yd, wd, od), fixed_array(byy, bw, bo), fixed_array(0u, 0u, B_W))
        var sp = set_tts_istft_cls(fixed_array(ypd, wd, opd), fixed_array(byy, bw, bo), fixed_array(0u, 0u, B_W))
        var pa = TtsIstftArgs(tt = uint(IS_TT), stride = uint(IS_STRIDE), bins = uint(IS_BINS), k = uint(IS_K), hop = uint(IS_HOP),
            pad = uint(IS_PAD), envelope = envelope ? 1u : 0u, n = uint(n), reoff = uint(IS_REOFF), imoff = uint(imoff), wnoff = uint(wnoff))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_istft_cls(raw, h, s, pa, tts_istft_wgs(int64(n)))
            enc_tts_istft_cls(raw, h, sp, pa, tts_istft_wgs(int64(n)))
            cmd_copy_planes(raw, h, fixed_array(od, opd), host, sizes, B_W)
        }
        let got <- staged(host.mapped, at[0], n, type<float>)
        let tail <- staged(host.mapped, at[0] + int64(n) * 4l, SLACK, type<float>)
        let gotp <- staged(host.mapped, at[1], n, type<float>)
        let bad = bars_bad(tag, got, ref.want, WAVE_REL, WAVE_ABS, IS_HOP)
        t |> success(bad == 0, "{tag}: {n} samples == the double overlap-add ({bad} of {n} off)")
        t |> success(!bar_ok(got[n - 1], poison_of(ref.want[n - 1]), WAVE_REL, WAVE_ABS), "{tag}: the bar reds a poisoned element")
        t |> success(bars_bad("{tag} poison (MUST mismatch)", gotp, ref.want, WAVE_REL, WAVE_ABS, IS_HOP) > 0, "{tag}: a poisoned magnitude reds the compare")
        t |> equal(nan_count(tail), SLACK, "{tag}: the elements past n keep their sentinel")
        destroy_device_buf(yd)
        destroy_device_buf(ypd)
        destroy_device_buf(wd)
        destroy_device_buf(od)
        destroy_device_buf(opd)
        destroy_host_buf(host)
        delete ref
    }
}

[test]
def test_vkt_istft(t0 : T?) {
    for (envelope in fixed_array(true, false)) {
        t0 |> run("the inverse STFT, envelope {envelope ? "on" : "off"}, 9 frames x 3 bins into 36 samples == the double overlap-add") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                if (!vk_moe_init() || !ensure_tts_istft_cls()) {
                    t |> skip("no Vulkan device")
                    return
                }
                t |> equal(is_n(), 36, "the sample count the name claims")
                istft_arm(t, envelope)
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}
