options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_source_kernels

require dastest/testing_boost public
require math
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_gpu_math   // tts_hash_normal: the own noise draw's CPU form
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require _tts_kernel_oracles   // nolint:STYLE030,LINT019 — the fixtures and double forms the Metal gate shares, inside the vulkan arms alone
require dasllama/dasllama_tts_blocks

//! The TTS tower's decoder tail on Vulkan against the CPU chain (a Vulkan device, else the cells skip): the stage sum's
//! axpy and the last stage's reflect pad, exact; the harmonic source on both resample laws - the phase frames, the mixed
//! signal, the captured noise and the source's own draw - held to the CPU `sine_source`; the STFT on both pad laws and the
//! inverse STFT with and without the envelope held to their double-precision forms; every output under a NaN fill,
//! every compare with its poisoned element.

let private SLACK = 5      //! the elements past a run, left at their fill

//! `w` moved by a hundredth of its magnitude (floored at 1): outside every bar in this file
def private poison_of(w : float) : float => w + max(1.0, abs(w)) * 0.01

//! the elements of `got` outside the bars, the first six and the widest miss logged (its row and column in rows `width` wide)
def private bars_bad(tag : string; got, want : array<float>; rel, absf : float; width : int) : int {
    let mi = maxdiff_at(got, want)
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / width} column {mi % width}\n")
    return mismatch_rel(got, want, rel, absf, tag)
}

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
            let op <- at_base(ob, AX_OOFF, SLACK)
            let yp <- at_base(yv, AX_YOFF)
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
                enc_tts_axpy_cls(raw, h, s_a, pa, int64(n))
                enc_tts_axpy_cls(raw, h, s_z, pz, int64(n))
                cmd_copy_planes(raw, h, fixed_array(oad, ozd), host, sizes, B_OA | B_OZ)
            }
            let whole_a <- staged(host.mapped, at[0], length(op), type<float>)
            let got_a <- subarray(whole_a, range(AX_OOFF, AX_OOFF + n))
            let whole_z <- staged(host.mapped, at[1], length(op), type<float>)
            let got_z <- subarray(whole_z, range(AX_OOFF, AX_OOFF + n))
            let want_a <- [for (i in range(n)); ob[i] + AX_SCALE * yv[i]]
            let want_z <- [for (i in range(n)); AX_SCALE * yv[i]]
            let bad_a = mismatch_exact(got_a, want_a)
            t |> success(bad_a == 0, "tts_axpy zero = 0: every element == o + 0.25 y ({bad_a} of {n} off)")
            t |> success(got_a[n - 1] != poison_of(want_a[n - 1]), "tts_axpy zero = 0: the bar reds a poisoned element")
            let bad_z = mismatch_exact(got_z, want_z)
            t |> success(bad_z == 0, "tts_axpy zero = 1: every element == 0.25 y over a NaN-filled o ({bad_z} of {n} off)")
            t |> success(got_z[0] != poison_of(want_z[0]), "tts_axpy zero = 1: the bar reds a poisoned element")
            t |> success(ob[0] != 0.0 && got_a[0] != got_z[0], "tts_axpy control: the same dispatch at zero = 0 adds o's old contents")
            t |> success(outside_run(whole_a, AX_OOFF, n, POISON), "tts_axpy zero = 0: the elements outside the run are untouched")
            t |> success(outside_run(whole_z, AX_OOFF, n, NAN_SENTINEL), "tts_axpy zero = 1: the elements outside the run keep their sentinel")
            destroy_device_buf(oad)
            destroy_device_buf(ozd)
            destroy_device_buf(yd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private RF_T = 37
let private RF_C = 24

[test]
def test_vkt_reflect1(t0 : T?) {
    t0 |> run("the reflect pad as the driver's two row copies: y [38][24] == x [37][24] behind x's row 1, exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_rows_cls()) {
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
            var s = set_tts_pk_rows_cls(fixed_array(xd, yd), fixed_array(bx, by), fixed_array(0u, B_Y))
            var shift = TtsPkRowsArgs(rows = uint(RF_T), width = uint(RF_C), ss = uint(RF_C), sc = 0u, sr0 = 0u, ds = uint(RF_C), dc = 0u, dr0 = 1u)
            var head = TtsPkRowsArgs(rows = 1u, width = uint(RF_C), ss = uint(RF_C), sc = 0u, sr0 = 1u, ds = uint(RF_C), dc = 0u, dr0 = 0u)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_rows_cls(raw, h, s, shift, int64(RF_T * RF_C))
                enc_tts_pk_rows_cls(raw, h, s, head, int64(RF_C))
                cmd_copy_planes(raw, h, fixed_array(yd), host, fixed_array(by), B_Y)
            }
            let got <- staged(host.mapped, 0l, total, type<float>)
            let tail <- staged(host.mapped, int64(total) * 4l, SLACK, type<float>)
            var want := x
            pad_reflect_left_rows_inplace(want, int64(RF_T), int64(RF_C), 1l)
            let bad = mismatch_exact(got, want)
            t |> success(bad == 0, "reflect pad: row 0 == x's row 1, row r == x's row r - 1 over {RF_T + 1} x {RF_C} ({bad} of {total} off)")
            t |> success(got[RF_C + 1] != poison_of(want[RF_C + 1]), "reflect pad: the bar reds a poisoned element")
            t |> success(got[0] != x[0], "reflect pad control: row 0 is x's row 1, not x's row 0")
            t |> equal(nan_count(tail), SLACK, "reflect pad: the elements past (t + 1) * c keep their sentinel")
            destroy_device_buf(xd)
            destroy_device_buf(yd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private SRC_WOFF = 5       //! the source linear's weight row in the slab plane, poisoned slots before it
let private SRC_GAP = 3         //! poisoned slots between the weight row and the bias
let private LOWC_REL = 1.2e-7   //! one ulp of a phase frame: the device's increments and sums in the CPU carry's operation order
let private HAR_REL = 2e-6      //! the mixed signal: the device sin, tanh and the linear's reassociation against the CPU's
let private HAR_ABS = 2e-7
let private NOISE_REL = 1e-4    //! the own draw: the device log, sqrt and cos - the driver's approximations, 2.8e-5 at most on the RTX PRO 4500 - against the CPU's on the same hashed uniforms, under 5 in magnitude
let private NOISE_ABS = 1e-5

struct private SrcOut {
    lowc : array<float>
    har : array<float>
    har_tail : array<float>     //! the `SLACK` elements past n
    noise : array<float>
}

def private src_boff : int => SRC_WOFF + TTS_SRC_NH + SRC_GAP

//! the source chain on the device (the own draw at `seed` when `own`, else the captured rows), every written buffer under a
//! NaN fill; the source linear's weight row at SRC_WOFF and its bias at `src_boff` in one slab plane between poisoned slots
def private src_run(f : TtsSrcFixture; torch : bool; f0 : array<float>; own : bool; seed : uint) : SrcOut {
    var o : SrcOut
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_LOW = 1u
        let B_LOWC = 2u
        let B_HAR = 4u
        let B_NZ = 8u
        var wn : array<float>
        put(wn, f.lw, SRC_WOFF)
        put(wn, f.lb, SRC_GAP)
        wn |> push_from(poison_plane(4))
        let f0d = dev_upload(f0)
        let nud = dev_upload(f.nu)
        let wnd = dev_upload(wn)
        let bnz = int64(f.n * TTS_SRC_NH) * 4l
        let nzd = own ? make_device_buf(bnz) : dev_upload(f.noise)
        if (own) {
            nan_sentinel_upload(nzd, bnz)
        }
        let blow = int64(TTS_SRC_NH * f.nlow) * 4l
        let bhar = int64(f.n + SLACK) * 4l
        let lowd = make_device_buf(blow)
        let lowcd = make_device_buf(blow)
        let hard = make_device_buf(bhar)
        nan_sentinel_upload(lowd, blow)
        nan_sentinel_upload(lowcd, blow)
        nan_sentinel_upload(hard, bhar)
        let bf0 = long_length(f0) * 4l
        let bnu = long_length(f.nu) * 4l
        let bwn = long_length(wn) * 4l
        let sizes = fixed_array(blow, bhar, bnz)
        let at <- staged_offsets(sizes)
        var host = make_host_buf(at[length(sizes)], true, [cached = true])
        var pa = TtsSourceArgs(n = uint(f.n), nlow = uint(f.nlow), nh = uint(TTS_SRC_NH), up = uint(f.up), sr = TTS_SRC_SR, sine_amp = 0.1,
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
        let low_total = int64(TTS_SRC_NH * f.nlow)
        let sin_n = int64(f.n)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            if (own) {
                enc_tts_src_noise_cls(raw, h, s_nz, pa, int64(f.n * TTS_SRC_NH))
            }
            if (torch) {
                enc_tts_src_low_torch_cls(raw, h, s_low, pa, low_total)
                enc_tts_src_cumsum_torch_cls(raw, h, s_cum, pa, 1l)
                enc_tts_src_sines_torch_cls(raw, h, s_sin, pa, sin_n)
            } else {
                enc_tts_src_low_onnx_cls(raw, h, s_low, pa, low_total)
                enc_tts_src_cumsum_onnx_cls(raw, h, s_cum, pa, 1l)
                enc_tts_src_sines_onnx_cls(raw, h, s_sin, pa, sin_n)
            }
            cmd_copy_planes(raw, h, fixed_array(lowcd, hard, nzd), host, sizes, B_LOWC | B_HAR | B_NZ)
        }
        o.lowc <- staged(host.mapped, at[0], TTS_SRC_NH * f.nlow, type<float>)
        o.har <- staged(host.mapped, at[1], f.n, type<float>)
        o.har_tail <- staged(host.mapped, at[1] + int64(f.n) * 4l, SLACK, type<float>)
        o.noise <- staged(host.mapped, at[2], f.n * TTS_SRC_NH, type<float>)
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
def private src_asserts(t : T?; tag : string; f : TtsSrcFixture; o : SrcOut) {
    let nl = TTS_SRC_NH * f.nlow
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
        let lc <- tts_long_cumsum(torch, up, nlong)
        t |> success(lc.parted > 0.01, "{tag} long cumsum: the two laws part by {lc.parted} cycles over {nlong} frames (the leg's control)")
        let ld = dev_upload(lc.low)
        let bl = long_length(lc.low) * 4l
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
        let bad = bars_bad("{tag} long cumsum", got, lc.want, 2e-7, 1e-3, nlong)
        t |> success(bad == 0, "{tag} long cumsum: every frame within the bars of the law's sum ({bad} of {nlong} off)")
        t |> success(!bar_ok(got[nlong - 1], lc.want[nlong - 1] + 2.0, 2e-7, 1e-3), "{tag} long cumsum: a moved last frame reds the compare")
        destroy_device_buf(ld)
        destroy_device_buf(lcd)
        destroy_host_buf(host)
    }
}

//! the own draw at seed 77 against the CPU hash: finite, in tanh's range, repeatable, apart from the captured signal, moved by seeds 78 and 81 (77 xor 4)
def private own_draw_arm(t : T?; tag : string; torch : bool; f : TtsSrcFixture; cap : SrcOut) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nz = f.n * TTS_SRC_NH
        var own1 <- src_run(f, torch, f.f0, true, 77u)
        let want_noise <- [for (i in range(nz)); tts_hash_normal(77u, uint(i))]
        let bad = bars_bad("{tag} own draw", own1.noise, want_noise, NOISE_REL, NOISE_ABS, TTS_SRC_NH)
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

//! one source shape on one law against the CPU chain, a poisoned f0 frame that must red both, under `full` the long cumsum and own draw
def private source_arm(t : T?; torch : bool; up, frames : int; full : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_source {torch ? "torch" : "onnx"} law up{up} x{frames}"
        var f <- tts_src_fixture(torch, up, frames)
        t |> equal(f.rows, int64(f.n), "{tag}: the CPU source mixes every sample")
        var cap <- src_run(f, torch, f.f0, false, 0u)
        src_asserts(t, tag, f, cap)
        var f0p := f.f0
        f0p[4] += 40.0   //! a voiced frame: every later phase moves
        var moved <- src_run(f, torch, f0p, false, 0u)
        let nl = TTS_SRC_NH * f.nlow
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

//! one pad law: the spectrum against the double form, then the first frame's reflected sample moved, which must red the compare
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
        var wn : array<float>
        put(wn, wre, ST_REOFF)
        let imoff = put(wn, wim, ST_GAP)
        wn |> push_from(poison_plane(4))
        let want <- stft_ref(har, wre, wim, ST_N, ST_K, ST_HOP, ST_PAD, ST_BINS, reflect)
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
        let total = int64(frames * ST_BINS)
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            if (reflect) {
                enc_tts_stft_reflect_cls(raw, h, s, pa, total)
                enc_tts_stft_reflect_cls(raw, h, sp, pa, total)
            } else {
                enc_tts_stft_edge_cls(raw, h, s, pa, total)
                enc_tts_stft_edge_cls(raw, h, sp, pa, total)
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

let private IS_TT = 9
let private IS_STRIDE = 8     //! the bins' magnitude and phase columns and two past them
let private IS_BINS = 3
let private IS_K = 8
let private IS_HOP = 4
let private IS_PAD = 2
let private IS_REOFF = 5      //! the real basis in the slab plane, poisoned slots before it
let private IS_GAP = 3        //! poisoned slots between the planes
let private WAVE_REL = 5e-4   //! the device exp, sin, cos (the driver's approximations: 1.2e-4 at most with the envelope, 3.7e-4 without, on the RTX PRO 4500) and f32 overlap-add against the double sum rounded once
let private WAVE_ABS = 5e-5

def private is_n : int => (IS_TT - 1) * IS_HOP + IS_K - 2 * IS_PAD

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
        var wn : array<float>
        put(wn, wre, IS_REOFF)
        let imoff = put(wn, wim, IS_GAP)
        let wnoff = put(wn, window, IS_GAP)
        wn |> push_from(poison_plane(4))
        var ref <- istft_ref(y, wre, wim, window, IS_TT, IS_STRIDE, IS_BINS, IS_K, IS_HOP, IS_PAD, envelope)
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
            enc_tts_istft_cls(raw, h, s, pa, int64(n))
            enc_tts_istft_cls(raw, h, sp, pa, int64(n))
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
