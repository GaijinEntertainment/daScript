options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_kernels

require dastest/testing_boost public
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require dasllama/dasllama_math
require dasllama/dasllama_tts_blocks   // nolint:STYLE030,LINT019 — attention_rows, leaky_relu: the CPU oracles, inside the vulkan arms alone
require daslib/jobque_boost
require math

//! The TTS tower's Vulkan kernel classes against their CPU oracles (a Vulkan device, else the
//! cells skip): the f32-exact tile GEMM the front end feeds on, in both its stamps, at shapes off
//! every tile and workgroup multiple, and the PL-BERT seat's row classes - the embedding gather, the
//! tanh GELU, the leaky ReLU and the bidirectional attention - with the poisoned-element control on
//! every compare.

def private row_fill(n : int; mul, add : int; scale, off : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % 97) * scale - off
    }
    return <- a
}

//! y[rows x d] = x[rows x n] w[d x n]^T (+ b) summed in k order - the tile's own order, so the compare is tight
def private f32_gemm_ref(x, w, b : array<float>; rows, d, n, wstride, xstride : int; bias : bool) : array<float> {
    var y : array<float>
    y |> resize(rows * d)
    for (p in range(rows)) {
        for (o in range(d)) {
            var acc = 0.0
            for (k in range(n)) {
                acc += x[p * xstride + k] * w[o * wstride + k]
            }
            y[p * d + o] = bias ? acc + b[o] : acc
        }
    }
    return <- y
}

let private G_ROWS = 37     //! positions: off the 64-position tile
let private G_D = 100       //! weight rows: off the 64-row tile, so the last tile masks 28 rows
let private G_N = 96        //! K: three 32-wide steps, the slab's lattice
let private G_WSTRIDE = 128 //! the slab's padded row: the pad past n never reads
let private G_XSTRIDE = 96

[test]
def test_vkt_f32_gemm(t0 : T?) {
    t0 |> run("the f32 tile GEMM, plain and biased, == the k-ordered fma sum at off-tile shapes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_Y = 1u
            let B_YB = 2u
            let rows = G_ROWS
            let d = G_D
            let n = G_N
            let nx = rows * G_XSTRIDE
            let nw = d * G_WSTRIDE
            let ny = rows * d
            var x <- row_fill(nx, 41, 7, 0.05, 2.1)
            var w <- row_fill(nw, 17, 3, 0.04, 1.9)
            var b <- row_fill(d, 29, 11, 0.06, 2.8)
            for (o in range(d)) {   // the pad past n carries garbage: a read past n lands it in the sum
                for (k in range(n, G_WSTRIDE)) {
                    w[o * G_WSTRIDE + k] = 7.5
                }
            }
            let bx = int64(nx) * 4l
            let bw = int64(nw) * 4l
            let bb = int64(d) * 4l
            let by = int64(ny) * 4l
            let xd = make_device_buf(bx)
            let wd = make_device_buf(bw)
            let bd = make_device_buf(bb)
            let yd = make_device_buf(by)
            let ybd = make_device_buf(by)
            let sizes = fixed_array(by, by)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bx)
                upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), bw)
                upload_region_at(bd, 0l, addr<uint8 const?>(b[0]), bb)
            }
            nan_sentinel_upload(yd, by)
            nan_sentinel_upload(ybd, by)
            var s_plain = set_f32_gemm_cls(fixed_array(wd, xd, yd), fixed_array(bw, bx, by), fixed_array(0u, 0u, B_Y))
            var s_bias = set_f32_gemm_b_cls(fixed_array(wd, xd, ybd, bd), fixed_array(bw, bx, by, bb), fixed_array(0u, 0u, B_YB, 0u))
            var pa = F32GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, wstride = uint(G_WSTRIDE), boff = 0u,
                xoff = 0u, xstride = uint(G_XSTRIDE), yoff = 0u, ystride = uint(d))
            let wgs = f32_gemm_wgs(int64(rows), int64(d))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_f32_gemm_cls(raw, h, s_plain, pa, wgs)
                enc_f32_gemm_b_cls(raw, h, s_bias, pa, wgs)
                cmd_copy_planes(raw, h, fixed_array(yd, ybd), host, sizes, B_Y | B_YB)
            }
            let y_dev <- staged(host.mapped, at[0], ny, type<float>)
            let yb_dev <- staged(host.mapped, at[1], ny, type<float>)
            let y_ref <- f32_gemm_ref(x, w, b, rows, d, n, G_WSTRIDE, G_XSTRIDE, false)
            let yb_ref <- f32_gemm_ref(x, w, b, rows, d, n, G_WSTRIDE, G_XSTRIDE, true)
            let bad = mismatch_approx(y_dev, y_ref)
            t |> success(bad == 0, "f32_gemm: the plain stamp matches the k-ordered fma sum ({bad} of {ny} off)")
            t |> success(!approx(y_dev[ny - 1], y_ref[ny - 1] + 0.5), "f32_gemm: the bar reds a poisoned element")
            let badb = mismatch_approx(yb_dev, yb_ref)
            t |> success(badb == 0, "f32_gemm_b: the biased stamp matches the sum plus the bias row ({badb} of {ny} off)")
            t |> success(!approx(yb_dev[0], yb_ref[0] + 0.5), "f32_gemm_b: the bar reds a poisoned element")
            t |> equal(nan_count(y_dev), 0, "every output element is stored (no sentinel survives)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private TTS_REL = 1e-5      //! the f32 row classes' bar: the device's reassociation and transcendentals sit well inside it, the f16 LUT GELU does not
let private GARBAGE = 7.5       //! the prefix before a plane's element base: a read that ignores the base lands it in the result

//! within `bar` relative of the larger magnitude, floored at 1 - `mismatch_rel`'s test on one element
def private rel_ok(x, y, bar : float) : bool => abs(x - y) <= bar * max(1.0, max(abs(x), abs(y)))

//! the index of the largest |got - want| (0 when every element agrees)
def private maxdiff_at(got, want : array<float>) : int {
    var m = 0.0
    var at = 0
    for (i in range(length(got))) {
        let dd = abs(got[i] - want[i])
        if (dd > m) {
            m = dd
            at = i
        }
    }
    return at
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

//! `a` behind `base` elements of `GARBAGE`: a device plane whose rows start at an element base
def private with_base(a : array<float>; base : int) : array<float> {
    var out : array<float>
    out |> resize(base + length(a))
    for (i in range(base)) {
        out[i] = GARBAGE
    }
    for (i in range(length(a))) {
        out[base + i] = a[i]
    }
    return <- out
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

// ===== the PL-BERT embedding gather =====

let private GA_C = 72       //! the row width: a row straddles the 256-element workgroups
let private GA_ROWS = 23
let private GA_VOCAB = 40
let private GA_WORD = 5     //! the word rows' element base in the slab
let private GA_OUT = 9      //! the output rows' element base: the prefix before it is never written

//! the CPU `styletts2_albert`'s embedding sum: word + position + type, added in that order
def private gather_ref(slab : array<float>; ids : array<int>; c, word_off, pos_off, type_off : int) : array<float> {
    var out : array<float>
    out |> resize(length(ids) * c)
    for (r in range(length(ids))) {
        for (j in range(c)) {
            out[r * c + j] = slab[word_off + ids[r] * c + j] + slab[pos_off + r * c + j] + slab[type_off + j]
        }
    }
    return <- out
}

[test]
def test_vkt_row_gather(t0 : T?) {
    t0 |> run("the embedding gather == the CPU word + position + type sum, exact, at element bases off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_row_gather_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_OUT = 1u
            let c = GA_C
            let rows = GA_ROWS
            let total = rows * c
            let pos_off = GA_WORD + GA_VOCAB * c
            let type_off = pos_off + rows * c
            var slab <- row_fill(type_off + c, 37, 5, 0.03, 1.4)
            var ids <- [for (r in range(rows)); (r * 7 + 3) % GA_VOCAB]
            ids[1] = ids[0]   // two positions on one word: only the position rows part them
            let bout = int64(GA_OUT + total) * 4l
            let slabd = dev_upload(slab)
            let idsd = dev_upload(ids)
            let outd = make_device_buf(bout)
            let sizes = fixed_array(bout)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(outd, bout)
            var s = set_tts_row_gather_cls(fixed_array(slabd, idsd, outd), fixed_array(long_length(slab) * 4l, long_length(ids) * 4l, bout), fixed_array(0u, 0u, B_OUT))
            var pa = TtsGatherArgs(c = uint(c), rows = uint(rows), word_off = uint(GA_WORD), pos_off = uint(pos_off), type_off = uint(type_off),
                out_off = uint(GA_OUT))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_row_gather_cls(raw, h, s, pa, tts_row_gather_wgs(int64(total)))
                cmd_copy_planes(raw, h, fixed_array(outd), host, sizes, B_OUT)
            }
            let prefix <- staged(host.mapped, at[0], GA_OUT, type<float>)
            let got <- staged(host.mapped, at[0] + int64(GA_OUT) * 4l, total, type<float>)
            let want <- gather_ref(slab, ids, c, GA_WORD, pos_off, type_off)
            let bad = mismatch_exact(got, want)
            let mi = maxdiff_at(got, want)
            to_log(LOG_INFO, "tts_row_gather: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / c} (id {ids[mi / c]}) col {mi % c}\n")
            t |> success(bad == 0, "tts_row_gather: every element == the CPU sum ({bad} of {total} off)")
            t |> success(got[total - 1] + 0.5 != want[total - 1], "tts_row_gather: the bar reds a poisoned element")
            var zero_ids : array<int>
            zero_ids |> resize(rows)
            let want0 <- gather_ref(slab, zero_ids, c, GA_WORD, pos_off, type_off)
            t |> success(mismatch_exact(got, want0) > 0, "tts_row_gather control: the rows follow their ids (the id-0 sum misses the device)")
            var parted = 0
            for (j in range(c)) {
                if (got[j] != got[c + j]) {
                    parted++
                }
            }
            t |> success(parted > 0, "tts_row_gather control: two positions on one word differ by their position rows")
            t |> equal(nan_count(prefix), GA_OUT, "tts_row_gather: the prefix before out_off keeps its sentinel")
            destroy_device_buf(slabd)
            destroy_device_buf(idsd)
            destroy_device_buf(outd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the elementwise maps: the tanh GELU and the leaky ReLU =====

let private EL_OFF = 7      //! the mapped run's element base: the prefix before it stays as it was
let private EL_N = 5203     //! five whole 1024-element workgroups and 83 more: the last live invocation maps the three-element tail
let private EL_SLACK = 5    //! elements past the run, left as they were
let private EL_SLOPE = 0.2

//! the elementwise cells' plane: x[EL_OFF] and x[EL_OFF + 1] at +-30, past the tanh clamp
def private elem_fixture() : array<float> {
    var x <- row_fill(EL_OFF + EL_N + EL_SLACK, 53, 1, 0.11, 5.3)
    x[EL_OFF] = 30.0
    x[EL_OFF + 1] = -30.0
    return <- x
}

//! true when every element outside [EL_OFF, EL_OFF + EL_N) of `got` is bit-equal to `x`
def private elem_outside_kept(got, x : array<float>) : bool {
    for (i in range(length(x))) {
        if ((i < EL_OFF || i >= EL_OFF + EL_N) && got[i] != x[i]) {
            return false
        }
    }
    return true
}

[test]
def test_vkt_gelu_tanh(t0 : T?) {
    t0 |> run("the tanh GELU == the CPU gelu at the f32 bar, the run's tail and the tanh clamp included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_gelu_tanh_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            var x <- elem_fixture()
            let n = length(x)
            let bx = int64(n) * 4l
            let xd = dev_upload(x)
            let sizes = fixed_array(bx)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            var s = set_tts_gelu_tanh_cls(fixed_array(xd), fixed_array(bx), fixed_array(B_X))
            var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = 0.0)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_gelu_tanh_cls(raw, h, s, pa, tts_elem_wgs(int64(EL_N)))
                cmd_copy_planes(raw, h, fixed_array(xd), host, sizes, B_X)
            }
            let got <- staged(host.mapped, at[0], n, type<float>)
            var want := x
            gelu(unsafe(addr(want[EL_OFF])), int64(EL_N))
            let bad = mismatch_rel(got, want, TTS_REL)
            let mi = maxdiff_at(got, want)
            to_log(LOG_INFO, "tts_gelu_tanh: max |gpu - cpu| {abs(got[mi] - want[mi])} at element {mi} (input {x[mi]})\n")
            t |> success(bad == 0, "tts_gelu_tanh: the device == the CPU gelu ({bad} of {n} off)")
            t |> success(!rel_ok(got[n - EL_SLACK - 1] + 1e-4, want[n - EL_SLACK - 1], TTS_REL), "tts_gelu_tanh: the bar reds a poisoned element")
            t |> success(elem_outside_kept(got, x), "tts_gelu_tanh: the elements outside the run are untouched")
            var lut_off = 0
            for (i in range(EL_OFF, EL_OFF + EL_N)) {
                if (!rel_ok(tower_gelu_tanh(x[i]), want[i], TTS_REL)) {
                    lut_off++
                }
            }
            t |> success(lut_off > 0, "tts_gelu_tanh control: the bar tells the f32 tanh form from the f16 LUT form ({lut_off} of {EL_N} apart)")
            destroy_device_buf(xd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_leaky(t0 : T?) {
    t0 |> run("the leaky ReLU == the CPU leaky_relu, exact, the run's tail included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_leaky_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            var x <- elem_fixture()
            let n = length(x)
            let bx = int64(n) * 4l
            let xd = dev_upload(x)
            let sizes = fixed_array(bx)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            var s = set_tts_leaky_cls(fixed_array(xd), fixed_array(bx), fixed_array(B_X))
            var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = EL_SLOPE)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_leaky_cls(raw, h, s, pa, tts_elem_wgs(int64(EL_N)))
                cmd_copy_planes(raw, h, fixed_array(xd), host, sizes, B_X)
            }
            let got <- staged(host.mapped, at[0], n, type<float>)
            var run <- [for (i in range(EL_N)); x[EL_OFF + i]]
            leaky_relu(run, int64(EL_N), EL_SLOPE)
            var want := x
            for (i in range(EL_N)) {
                want[EL_OFF + i] = run[i]
            }
            let bad = mismatch_exact(got, want)
            let mi = maxdiff_at(got, want)
            to_log(LOG_INFO, "tts_leaky: max |gpu - cpu| {abs(got[mi] - want[mi])} at element {mi} (input {x[mi]})\n")
            t |> success(bad == 0, "tts_leaky: the device == the CPU leaky_relu ({bad} of {n} off)")
            t |> success(got[n - EL_SLACK - 1] + 0.5 != want[n - EL_SLACK - 1], "tts_leaky: the bar reds a poisoned element")
            t |> success(elem_outside_kept(got, x), "tts_leaky: the elements outside the run are untouched")
            let neg = EL_OFF + 1
            t |> success(got[neg] == x[neg] * EL_SLOPE && got[neg] != x[neg], "tts_leaky control: a negative element is scaled by the slope")
            t |> success(got[EL_OFF] == x[EL_OFF], "tts_leaky control: a positive element passes unchanged")
            destroy_device_buf(xd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the bidirectional attention =====

//! one attention shape: q, k, v and out at element bases off zero, dispatched over the keys as they are and again with
//! the last key row zeroed, each against the CPU `attention_rows`
def private attn_arm(t : T?; rows, heads, hs : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_attn t{rows} {heads}x{hs}"
        let B_O = 1u
        let B_OZ = 2u
        let d = heads * hs
        let n = rows * d
        let qoff = 5
        let koff = 7
        let voff = 11
        let ooff = 13
        var q <- row_fill(n, 41, 7, 0.02, 0.95)
        var k <- row_fill(n, 17, 3, 0.02, 0.97)
        var v <- row_fill(n, 53, 11, 0.04, 1.9)
        var kz := k
        for (e in range(d)) {
            kz[(rows - 1) * d + e] = 0.0
        }
        var qp <- with_base(q, qoff)
        var kp <- with_base(k, koff)
        var kzp <- with_base(kz, koff)
        var vp <- with_base(v, voff)
        let qd = dev_upload(qp)
        let kd = dev_upload(kp)
        let kzd = dev_upload(kzp)
        let vd = dev_upload(vp)
        let bo = int64(ooff + n) * 4l
        let od = make_device_buf(bo)
        let ozd = make_device_buf(bo)
        let sizes = fixed_array(bo, bo)
        let at <- staged_offsets(sizes)
        let host = make_host_buf(at[length(sizes)], true, [cached = true])
        nan_sentinel_upload(od, bo)
        nan_sentinel_upload(ozd, bo)
        let bq = long_length(qp) * 4l
        let bk = long_length(kp) * 4l
        let bv = long_length(vp) * 4l
        var s = set_tts_attn_cls(fixed_array(qd, kd, vd, od), fixed_array(bq, bk, bv, bo), fixed_array(0u, 0u, 0u, B_O))
        var sz = set_tts_attn_cls(fixed_array(qd, kzd, vd, ozd), fixed_array(bq, bk, bv, bo), fixed_array(0u, 0u, 0u, B_OZ))
        var pa = TtsAttnArgs(t = uint(rows), d = uint(d), hs = uint(hs), heads = uint(heads), scale = 1.0 / sqrt(float(hs)),
            qoff = uint(qoff), koff = uint(koff), voff = uint(voff), ooff = uint(ooff))
        let wgs = tts_attn_wgs(int64(rows), int64(heads))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_attn_cls(raw, h, s, pa, wgs)
            enc_tts_attn_cls(raw, h, sz, pa, wgs)
            cmd_copy_planes(raw, h, fixed_array(od, ozd), host, sizes, B_O | B_OZ)
        }
        let got <- staged(host.mapped, at[0] + int64(ooff) * 4l, n, type<float>)
        let gotz <- staged(host.mapped, at[1] + int64(ooff) * 4l, n, type<float>)
        let prefix <- staged(host.mapped, at[0], ooff, type<float>)
        var want : array<float>
        var wantz : array<float>
        attention_rows(q, k, v, int64(rows), int64(d), int64(heads), want)
        attention_rows(q, kz, v, int64(rows), int64(d), int64(heads), wantz)
        let bad = mismatch_rel(got, want, TTS_REL)
        let mi = maxdiff_at(got, want)
        to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / d} head {(mi % d) / hs} col {mi % hs}\n")
        t |> success(bad == 0, "{tag}: the device == the CPU attention_rows ({bad} of {n} off)")
        t |> success(!rel_ok(got[n - 1] + 1e-4, want[n - 1], TTS_REL), "{tag}: the bar reds a poisoned element")
        let badz = mismatch_rel(gotz, wantz, TTS_REL)
        t |> success(badz == 0, "{tag}: the last key zeroed, the device == the CPU attention_rows ({badz} of {n} off)")
        t |> success(!rel_ok(gotz[0] + 1e-4, wantz[0], TTS_REL), "{tag}: the zeroed-key bar reds a poisoned element")
        var moved = 0
        for (e in range(d)) {
            if (!rel_ok(got[e], gotz[e], TTS_REL)) {
                moved++
            }
        }
        t |> success(moved > 0, "{tag} control: zeroing the last key moves the first query row (every key is read, not a causal prefix)")
        t |> equal(nan_count(prefix), ooff, "{tag}: the prefix before ooff keeps its sentinel")
        destroy_device_buf(qd)
        destroy_device_buf(kd)
        destroy_device_buf(kzd)
        destroy_device_buf(vd)
        destroy_device_buf(od)
        destroy_device_buf(ozd)
        destroy_host_buf(host)
    }
}

[test]
def test_vkt_attn(t0 : T?) {
    t0 |> run("the bidirectional attention == the CPU attention_rows at hs 64 and 128, one and two keys an invocation") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_job_que() {   // attention_rows threads its heads
                setup_dasllama_jobque_()
                attn_arm(t, 37, 4, 64)
                attn_arm(t, 130, 2, 128)
                attn_arm(t, 512, 2, 64)   // the staged score row full: every invocation scores two keys
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
