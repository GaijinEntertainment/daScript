options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_source_kernels

require dastest/testing_boost public
require math   // nolint:STYLE030,LINT019 — the oracles' scalar math inside the vulkan arms alone; the no-Vulkan lane sees it unused
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_gpu_math   // tts_hash_normal: the own noise draw's CPU form
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require _tts_kernel_oracles   // nolint:STYLE030,LINT019 — the fixtures and double forms the Metal gate shares, inside the vulkan arms alone
require dasllama/dasllama_tts_blocks   // nolint:STYLE030,LINT019 — the CPU source chain the cells hold against, inside the vulkan arms alone

//! The TTS tower's decoder tail on Vulkan against the CPU chain (a Vulkan device, else the cells skip): the stage sum's
//! axpy and the last stage's reflect pad, exact; the harmonic source on both resample laws - the phase frames, the mixed
//! signal, the captured noise and the source's own draw - held to the CPU `sine_source`; the STFT on both pad laws and the
//! inverse STFT with and without the envelope held to their double-precision forms; every output under a NaN fill,
//! every compare with its poisoned element.

let private SLACK = 5      //! the elements past a run, left at their fill

let private BAR_EXACT_POISON_PCT = Bar(exact = true, poison = 0.01, poison_scaled = true)   //! bit-exact, its poison a hundredth of the magnitude (floored at 1): outside every bar in this file

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
            let n = AX_N
            let ob <- f16_fill(n, 7, 0.05, -1.1)
            let yv <- f16_fill(n, 5, 0.04, -0.9)
            with_cell_planes() $(var cp : CellPlanes) {
                let oap = cp |> io_(ob, AX_OOFF, SLACK)
                let ozp = cp |> out_(n, AX_OOFF, SLACK)
                let yp = cp |> in_(yv, AX_YOFF)
                var s_a = cp |> cls_set(fixed_array(oap, yp), @@set_tts_axpy_cls)
                var s_z = cp |> cls_set(fixed_array(ozp, yp), @@set_tts_axpy_cls)
                var pa = TtsAxpyArgs(nelem = uint(n), scale = AX_SCALE, zero = 0u, ooff = uint(AX_OOFF), yoff = uint(AX_YOFF))
                var pz = pa
                pz.zero = 1u
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_axpy_cls(raw, h, s_a, pa, int64(n))
                    enc_tts_axpy_cls(raw, h, s_z, pz, int64(n))
                }
                let got_a <- cp |> plane_run(oap)
                let got_z <- cp |> plane_run(ozp)
                check_rows(t, "tts_axpy zero = 0 vs o + 0.25 y", got_a, [for (i in range(n)); ob[i] + AX_SCALE * yv[i]], BAR_EXACT_POISON_PCT, FLAT_ROWS)
                check_rows(t, "tts_axpy zero = 1 vs 0.25 y over a NaN-filled o", got_z, [for (i in range(n)); AX_SCALE * yv[i]], BAR_EXACT_POISON_PCT, FLAT_ROWS)
                t |> success(ob[0] != 0.0 && got_a[0] != got_z[0], "tts_axpy control: the same dispatch at zero = 0 adds o's old contents")
                t |> success(outside_run(cp |> plane_whole(oap), AX_OOFF, n, POISON), "tts_axpy zero = 0: the elements outside the run are untouched")
                t |> success(outside_run(cp |> plane_whole(ozp), AX_OOFF, n, NAN_SENTINEL), "tts_axpy zero = 1: the elements outside the run keep their sentinel")
            }
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
            let total = (RF_T + 1) * RF_C
            let x <- f16_fill(RF_T * RF_C, 11, 0.03, -0.7)
            var want := x
            pad_reflect_left_rows_inplace(want, int64(RF_T), int64(RF_C), 1l)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let yp = cp |> out_(total, 0, SLACK)
                var s = cp |> cls_set(fixed_array(xp, yp), @@set_tts_pk_rows_cls)
                var shift = TtsPkRowsArgs(rows = uint(RF_T), width = uint(RF_C), ss = uint(RF_C), sc = 0u, sr0 = 0u, ds = uint(RF_C), dc = 0u, dr0 = 1u)
                var head = TtsPkRowsArgs(rows = 1u, width = uint(RF_C), ss = uint(RF_C), sc = 0u, sr0 = 1u, ds = uint(RF_C), dc = 0u, dr0 = 0u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_pk_rows_cls(raw, h, s, shift, int64(RF_T * RF_C))
                    enc_tts_pk_rows_cls(raw, h, s, head, int64(RF_C))
                }
                let got <- cp |> plane_run(yp)
                check_rows(t, "reflect pad [{RF_T + 1}][{RF_C}] vs x behind its row 1", got, want, BAR_EXACT_POISON_PCT, RF_C)
                t |> success(got[0] != x[0], "reflect pad control: row 0 is x's row 1, not x's row 0")
                t |> success(outside_run(cp |> plane_whole(yp), 0, total, NAN_SENTINEL), "reflect pad: the elements past (t + 1) * c keep their sentinel")
            }
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

let private LOWC_BAR = Bar(rel = LOWC_REL, poison = 0.01, poison_scaled = true)
let private HAR_BAR = Bar(rel = HAR_REL, abs_floor = HAR_ABS, poison = 0.01, poison_scaled = true)
let private NOISE_BAR = Bar(rel = NOISE_REL, abs_floor = NOISE_ABS, poison = 0.01, poison_scaled = true)

struct private SrcOut {
    lowc : array<float>
    har : array<float>
    har_whole : array<float>    //! the mixed signal's plane with the `SLACK` elements past n
    noise : array<float>
}

def private src_boff : int => SRC_WOFF + TTS_SRC_NH + SRC_GAP

//! Where the source chain's noise rows come from.
enum private SrcNoise {
    captured   //! the fixture's rows, uploaded
    own        //! the device's own draw at the run's seed
}

//! the device source chain on `noise`'s rows under NaN fills, its linear at SRC_WOFF / `src_boff`
def private src_run(f : TtsSrcFixture; torch : bool; f0 : array<float>; noise : SrcNoise; seed : uint) : SrcOut {
    var o : SrcOut
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var wn : array<float>
        put_run(wn, f.lin_w, SRC_WOFF)
        put_run(wn, f.lin_b, SRC_GAP)
        wn |> push_from(poison_plane(4))
        let nz = f.n * TTS_SRC_NH
        let nlow = TTS_SRC_NH * f.nlow
        with_cell_planes() $(var cp : CellPlanes) {
            let f0p = cp |> in_(f0)
            let nup = cp |> in_(f.nu)
            let wp = cp |> in_(wn)
            let nzp = noise == SrcNoise.own ? cp |> out_(nz) : cp |> io_(f.noise)
            let lowp = cp |> out_(nlow)
            let lowcp = cp |> out_(nlow)
            let harp = cp |> out_(f.n, 0, SLACK)
            var pa = TtsSourceArgs(n = uint(f.n), nlow = uint(f.nlow), nh = uint(TTS_SRC_NH), up = uint(f.up), sr = TTS_SRC_SR, sine_amp = 0.1,
                noise_std = 0.003, voiced_thr = 10.0, seed = seed, woff = uint(SRC_WOFF), boff = uint(src_boff()))
            var s_nz = cp |> cls_set(fixed_array(nzp), @@set_tts_src_noise_cls)
            let low_ix = fixed_array(f0p, nup, lowp)
            var s_low = torch ? cp |> cls_set(low_ix, @@set_tts_src_low_torch_cls) : cp |> cls_set(low_ix, @@set_tts_src_low_onnx_cls)
            let cum_ix = fixed_array(lowp, lowcp)
            var s_cum = torch ? cp |> cls_set(cum_ix, @@set_tts_src_cumsum_torch_cls) : cp |> cls_set(cum_ix, @@set_tts_src_cumsum_onnx_cls)
            let sin_ix = fixed_array(f0p, lowcp, nzp, wp, harp)
            var s_sin = torch ? cp |> cls_set(sin_ix, @@set_tts_src_sines_torch_cls) : cp |> cls_set(sin_ix, @@set_tts_src_sines_onnx_cls)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (noise == SrcNoise.own) {
                    enc_tts_src_noise_cls(raw, h, s_nz, pa, int64(nz))
                }
                if (torch) {
                    enc_tts_src_low_torch_cls(raw, h, s_low, pa, int64(nlow))
                    enc_tts_src_cumsum_torch_cls(raw, h, s_cum, pa, 1l)
                    enc_tts_src_sines_torch_cls(raw, h, s_sin, pa, int64(f.n))
                } else {
                    enc_tts_src_low_onnx_cls(raw, h, s_low, pa, int64(nlow))
                    enc_tts_src_cumsum_onnx_cls(raw, h, s_cum, pa, 1l)
                    enc_tts_src_sines_onnx_cls(raw, h, s_sin, pa, int64(f.n))
                }
            }
            o.lowc <- cp |> plane_run(lowcp)
            o.har <- cp |> plane_run(harp)
            o.har_whole <- cp |> plane_whole(harp)
            o.noise <- cp |> plane_run(nzp)
        }
    }
    return <- o
}

//! the phase frames and the mixed signal against the CPU chain at their bars, the poisoned element on each
def private src_asserts(t : T?; tag : string; f : TtsSrcFixture; o : SrcOut) {
    check_rows(t, "{tag} phase frames within one ulp of the CPU carry", o.lowc, f.want_lowc, LOWC_BAR, f.nlow)
    check_rows(t, "{tag} mixed signal within 2e-6 of the CPU sines through the linear and tanh", o.har, f.want_har, HAR_BAR, f.up)
    t |> success(outside_run(o.har_whole, 0, f.n, NAN_SENTINEL), "{tag}: the elements past n keep their sentinel")
}

//! the long cumsum: 40000 low-rate frames of one harmonic, where a plain f32 sum and a double one part by cycles
def private long_cumsum_arm(t : T?; tag : string; torch : bool; up : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nlong = 40000
        let lc <- tts_long_cumsum(torch, up, nlong)
        t |> success(lc.f32_drift_cycles > 0.01, "{tag} long cumsum: the two laws part by {lc.f32_drift_cycles} cycles over {nlong} frames (the leg's control)")
        with_cell_planes() $(var cp : CellPlanes) {
            let lp = cp |> in_(lc.low)
            let cp_out = cp |> out_(nlong)
            let ix = fixed_array(lp, cp_out)
            var s = torch ? cp |> cls_set(ix, @@set_tts_src_cumsum_torch_cls) : cp |> cls_set(ix, @@set_tts_src_cumsum_onnx_cls)
            var pa = TtsSourceArgs(n = uint(nlong * up), nlow = uint(nlong), nh = 1u, up = uint(up))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (torch) {
                    enc_tts_src_cumsum_torch_cls(raw, h, s, pa, 1l)
                } else {
                    enc_tts_src_cumsum_onnx_cls(raw, h, s, pa, 1l)
                }
            }
            check_rows(t, "{tag} long cumsum vs the law's sum", cp |> plane_run(cp_out), lc.want, Bar(rel = 2e-7, abs_floor = 1e-3, poison = 2.0), FLAT_ROWS)   // two radians on the last frame: the bars are a few ulps of a phase in the thousands
        }
    }
}

//! the own draw at seed 77 against the CPU hash: finite, in tanh's range, repeatable, apart from the captured signal, moved by seeds 78 and 81 (77 xor 4)
def private own_draw_arm(t : T?; tag : string; torch : bool; f : TtsSrcFixture; cap : SrcOut) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nz = f.n * TTS_SRC_NH
        var own1 <- src_run(f, torch, f.f0, SrcNoise.own, 77u)
        check_rows(t, "{tag} own draw vs the CPU hash at seed 77", own1.noise, [for (i in range(nz)); tts_hash_normal(77u, uint(i))], NOISE_BAR, TTS_SRC_NH)
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
        var own2 <- src_run(f, torch, f.f0, SrcNoise.own, 77u)
        t |> equal(mismatch_exact(own2.har, own1.har), 0, "{tag}: the own draw repeats bit for bit at one seed")
        for (other in fixed_array(78u, 81u)) {
            var own3 <- src_run(f, torch, f.f0, SrcNoise.own, other)
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
        var cap <- src_run(f, torch, f.f0, SrcNoise.captured, 0u)
        src_asserts(t, tag, f, cap)
        var f0p := f.f0
        f0p[4] += 40.0   // a voiced frame: every later phase moves
        var moved <- src_run(f, torch, f0p, SrcNoise.captured, 0u)
        let nl = TTS_SRC_NH * f.nlow
        t |> success(mismatch_rel("{tag} lowc poison (MUST mismatch)", moved.lowc, f.want_lowc, LOWC_REL) > 0, "{tag}: a poisoned f0 frame reds the lowc compare over {nl} frames")
        t |> success(mismatch_rel("{tag} har poison (MUST mismatch)", moved.har, f.want_har, HAR_REL, HAR_ABS) > 0, "{tag}: a poisoned f0 frame reds the har compare")
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
let private SPEC_BAR = Bar(rel = SPEC_REL, abs_floor = SPEC_ABS, poison = 0.01, poison_scaled = true)

def private st_frames : int => (ST_N + 2 * ST_PAD - ST_K) / ST_HOP + 1

//! one pad law: the spectrum against the double form, then the first frame's reflected sample moved, which must red the compare
def private stft_arm(t : T?; reflect : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = reflect ? "tts_stft reflect" : "tts_stft edge"
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
        put_run(wn, wre, ST_REOFF)
        let imoff = put_run(wn, wim, ST_GAP)
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
        harp[1] += 2.0   // the first frame's reflected tap reads it
        with_cell_planes() $(var cp : CellPlanes) {
            let hp = cp |> in_(har)
            let hpp = cp |> in_(harp)
            let wp = cp |> in_(wn)
            let sp = cp |> out_(nspec, 0, SLACK)
            let spp = cp |> out_(nspec, 0, SLACK)
            var s = reflect ? cp |> cls_set(fixed_array(hp, wp, sp), @@set_tts_stft_reflect_cls) : cp |> cls_set(fixed_array(hp, wp, sp), @@set_tts_stft_edge_cls)
            var sq = reflect ? cp |> cls_set(fixed_array(hpp, wp, spp), @@set_tts_stft_reflect_cls) : cp |> cls_set(fixed_array(hpp, wp, spp), @@set_tts_stft_edge_cls)
            var pa = TtsStftArgs(n = uint(ST_N), frames = uint(frames), bins = uint(ST_BINS), k = uint(ST_K), hop = uint(ST_HOP), pad = uint(ST_PAD),
                eps = 1e-9, reoff = uint(ST_REOFF), imoff = uint(imoff))
            let total = int64(frames * ST_BINS)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (reflect) {
                    enc_tts_stft_reflect_cls(raw, h, s, pa, total)
                    enc_tts_stft_reflect_cls(raw, h, sq, pa, total)
                } else {
                    enc_tts_stft_edge_cls(raw, h, s, pa, total)
                    enc_tts_stft_edge_cls(raw, h, sq, pa, total)
                }
            }
            check_rows(t, "{tag} {frames} frames x {ST_BINS} bins of magnitude and phase vs the double form", cp |> plane_run(sp), want, SPEC_BAR, 2 * ST_BINS)
            t |> success(mismatch_rel("{tag} poison (MUST mismatch)", cp |> plane_run(spp), want, SPEC_REL, SPEC_ABS) > 0, "{tag}: a poisoned sample reds the compare")
            t |> success(outside_run(cp |> plane_whole(sp), 0, nspec, NAN_SENTINEL), "{tag}: the elements past the spectrum keep their sentinel")
        }
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
let private WAVE_BAR = Bar(rel = WAVE_REL, abs_floor = WAVE_ABS, poison = 0.01, poison_scaled = true)

def private is_n : int => (IS_TT - 1) * IS_HOP + IS_K - 2 * IS_PAD

//! one envelope mode: the wave against the double form, then a moved log magnitude, which must red the compare
def private istft_arm(t : T?; envelope : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = envelope ? "tts_istft envelope" : "tts_istft plain"
        let n = is_n()
        let nb = IS_K * IS_BINS
        var y <- f16_fill(IS_TT * IS_STRIDE, 7, 0.15, -1.2)
        let wre <- f16_fill(nb, 5, 0.2, -0.7)
        let wim <- f16_fill(nb, 3, 0.15, -0.5)
        var window <- f16_fill(IS_K, 11, 0.1, 0.3)
        window[1] = 0.0   // taps 1 and 5 carry every sample m = 1 mod 4: their envelope is zero
        window[5] = 0.0
        var wn : array<float>
        put_run(wn, wre, IS_REOFF)
        let imoff = put_run(wn, wim, IS_GAP)
        let wnoff = put_run(wn, window, IS_GAP)
        wn |> push_from(poison_plane(4))
        var ref <- istft_ref(y, wre, wim, window, IS_TT, IS_STRIDE, IS_BINS, IS_K, IS_HOP, IS_PAD, envelope)
        t |> success(ref.undivided > 0 && ref.undivided < n, "{tag}: {ref.undivided} of {n} samples carry a negligible envelope")
        var yp := y
        yp[4 * IS_STRIDE + 1] += 2.0   // a log magnitude: every sample its frame overlaps moves
        with_cell_planes() $(var cp : CellPlanes) {
            let yq = cp |> in_(y)
            let ypq = cp |> in_(yp)
            let wq = cp |> in_(wn)
            let oq = cp |> out_(n, 0, SLACK)
            let opq = cp |> out_(n, 0, SLACK)
            var s = cp |> cls_set(fixed_array(yq, wq, oq), @@set_tts_istft_cls)
            var sq = cp |> cls_set(fixed_array(ypq, wq, opq), @@set_tts_istft_cls)
            var pa = TtsIstftArgs(tt = uint(IS_TT), stride = uint(IS_STRIDE), bins = uint(IS_BINS), k = uint(IS_K), hop = uint(IS_HOP),
                pad = uint(IS_PAD), envelope = envelope ? 1u : 0u, n = uint(n), reoff = uint(IS_REOFF), imoff = uint(imoff), wnoff = uint(wnoff))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_istft_cls(raw, h, s, pa, int64(n))
                enc_tts_istft_cls(raw, h, sq, pa, int64(n))
            }
            check_rows(t, "{tag} {n} samples vs the double overlap-add", cp |> plane_run(oq), ref.want, WAVE_BAR, IS_HOP)
            t |> success(mismatch_rel("{tag} poison (MUST mismatch)", cp |> plane_run(opq), ref.want, WAVE_REL, WAVE_ABS) > 0, "{tag}: a poisoned magnitude reds the compare")
            t |> success(outside_run(cp |> plane_whole(oq), 0, n, NAN_SENTINEL), "{tag}: the elements past n keep their sentinel")
        }
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
