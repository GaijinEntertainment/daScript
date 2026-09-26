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
//! tanh GELU, the leaky ReLU and the bidirectional attention - and the predictor and decoder row classes - the concat,
//! the duration sigmoid sums, the column statistics with the fused AdaIN stamps and the residual join - with the
//! poisoned-element control on every compare.

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
                out_off = uint(GA_OUT), adds = 1u)
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
            // the text encoder's form: the word rows alone, the position and type rows left out of the sum
            nan_sentinel_upload(outd, bout)
            pa.adds = 0u
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_row_gather_cls(raw, h, s, pa, tts_row_gather_wgs(int64(total)))
                cmd_copy_planes(raw, h, fixed_array(outd), host, sizes, B_OUT)
            }
            let got_plain <- staged(host.mapped, at[0] + int64(GA_OUT) * 4l, total, type<float>)
            var want_plain : array<float>
            want_plain |> resize(total)
            for (r in range(rows)) {
                for (j in range(c)) {
                    want_plain[r * c + j] = slab[GA_WORD + ids[r] * c + j]
                }
            }
            t |> equal(mismatch_exact(got_plain, want_plain), 0, "tts_row_gather adds = 0: every element == the word row alone")
            t |> success(mismatch_exact(got_plain, want) > 0, "tts_row_gather adds = 0 control: the position rows are out of the sum")
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

// ===== the concat rows =====

let private CC_ROWS = 37
let private CC_C = 40
let private CC_RC = 13      //! the style row's columns: the duration encoder appends one row to every position
let private CC_F0S = 3      //! the F0 column's row stride
let private CC_NS = 2       //! the noise column's row stride
let private CC_WIDTH = 48   //! the padded arm's width: six zero columns past x, f0 and n

//! the CPU `concat_rows`: x, the residual columns under `with_res` (row stride rs, 0 = one row broadcast), f0, n, zeros
def private concat_ref(x, res, f0, nd : array<float>; rows, c, rc, rs, f0s, ns, width : int; with_res : bool) : array<float> {
    let rcw = with_res ? rc : 0
    var out : array<float>
    out |> resize(rows * width)
    for (r in range(rows)) {
        for (j in range(width)) {
            var v = 0.0
            if (j < c) {
                v = x[r * c + j]
            } elif (j < c + rcw) {
                v = res[r * rs + j - c]
            } elif (j == c + rcw) {
                v = f0[r * f0s]
            } elif (j == c + rcw + 1) {
                v = nd[r * ns]
            }
            out[r * width + j] = v
        }
    }
    return <- out
}

[test]
def test_vkt_concat(t0 : T?) {
    t0 |> run("the concat rows == the CPU concat, exact: one style row broadcast, then f0 and n at strides and a zero pad") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_concat_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_OR = 1u
            let B_OP = 2u
            let rows = CC_ROWS
            let c = CC_C
            let wr = c + CC_RC
            var x <- row_fill(rows * c, 41, 7, 0.05, 2.1)
            var style <- row_fill(CC_RC, 29, 11, 0.06, 2.8)
            var f0 <- row_fill(rows * CC_F0S, 17, 3, 0.04, 1.9)
            var nd <- row_fill(rows * CC_NS, 53, 1, 0.11, 5.3)
            let xd = dev_upload(x)
            let sd = dev_upload(style)
            let f0d = dev_upload(f0)
            let ndd = dev_upload(nd)
            let bor = int64(rows * wr) * 4l
            let bop = int64(rows * CC_WIDTH) * 4l
            let ord = make_device_buf(bor)
            let opd = make_device_buf(bop)
            let sizes = fixed_array(bor, bop)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(ord, bor)
            nan_sentinel_upload(opd, bop)
            let bx = long_length(x) * 4l
            let bf = long_length(f0) * 4l
            let bn = long_length(nd) * 4l
            var s_res = set_tts_concat_cls(fixed_array(xd, sd, f0d, ndd, ord), fixed_array(bx, long_length(style) * 4l, bf, bn, bor), fixed_array(0u, 0u, 0u, 0u, B_OR))
            var s_pad = set_tts_concat_cls(fixed_array(xd, xd, f0d, ndd, opd), fixed_array(bx, bx, bf, bn, bop), fixed_array(0u, 0u, 0u, 0u, B_OP))
            var pr = TtsConcatArgs(c = uint(c), rc = uint(CC_RC), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(wr), rows = uint(rows))
            var pp = TtsConcatArgs(c = uint(c), rc = uint(CC_RC), rs = uint(CC_RC), f0s = uint(CC_F0S), ns = uint(CC_NS), with_res = 0u,
                width = uint(CC_WIDTH), rows = uint(rows))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_concat_cls(raw, h, s_res, pr, tts_concat_wgs(int64(rows * wr)))
                enc_tts_concat_cls(raw, h, s_pad, pp, tts_concat_wgs(int64(rows * CC_WIDTH)))
                cmd_copy_planes(raw, h, fixed_array(ord, opd), host, sizes, B_OR | B_OP)
            }
            let nr = rows * wr
            let np = rows * CC_WIDTH
            let got_r <- staged(host.mapped, at[0], nr, type<float>)
            let got_p <- staged(host.mapped, at[1], np, type<float>)
            let want_r <- concat_ref(x, style, f0, nd, rows, c, CC_RC, 0, 0, 0, wr, true)
            let want_p <- concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, CC_F0S, CC_NS, CC_WIDTH, false)
            let bad_r = mismatch_exact(got_r, want_r)
            t |> success(bad_r == 0, "tts_concat with_res rs 0: every element == the CPU concat ({bad_r} of {nr} off)")
            t |> success(got_r[nr - 1] + 0.5 != want_r[nr - 1], "tts_concat with_res: the bar reds a poisoned element")
            let bad_p = mismatch_exact(got_p, want_p)
            t |> success(bad_p == 0, "tts_concat f0/n at strides, zero pad: every element == the CPU concat ({bad_p} of {np} off)")
            t |> success(got_p[0] + 0.5 != want_p[0], "tts_concat padded: the bar reds a poisoned element")
            let want_s1 <- concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, 1, 1, CC_WIDTH, false)
            t |> success(mismatch_exact(got_p, want_s1) > 0, "tts_concat control: the f0 and n columns follow their strides")
            destroy_device_buf(xd)
            destroy_device_buf(sd)
            destroy_device_buf(f0d)
            destroy_device_buf(ndd)
            destroy_device_buf(ord)
            destroy_device_buf(opd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the duration sigmoid sums =====

let private SG_T = 37
let private SG_NB = 50      //! the duration bins summed
let private SG_LS = 64      //! the logit rows' stride: the columns past SG_NB carry GARBAGE
let private SG_SPEED = 1.3

//! the CPU `styletts2_durations`' raw sum in f32: the sigmoids of a row's first `nb` logits in column order, over `speed`
def private sigsum_ref(logits : array<float>; rows, nb, ls : int; speed : float) : array<float> {
    var raw : array<float>
    raw |> resize(rows)
    for (r in range(rows)) {
        var acc = 0.0
        for (j in range(nb)) {
            acc += 1.0 / (1.0 + exp(-logits[r * ls + j]))
        }
        raw[r] = acc / speed
    }
    return <- raw
}

[test]
def test_vkt_sigsum(t0 : T?) {
    t0 |> run("the sigmoid sums == the CPU raw durations over 50 of 64 columns at speed 1.3") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_sigsum_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_RAW = 1u
            var logits <- row_fill(SG_T * SG_LS, 41, 7, 0.1, 4.8)
            for (r in range(SG_T)) {
                for (j in range(SG_NB, SG_LS)) {
                    logits[r * SG_LS + j] = GARBAGE
                }
            }
            let ld = dev_upload(logits)
            let braw = int64(SG_T) * 4l
            let rawd = make_device_buf(braw)
            let sizes = fixed_array(braw)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(rawd, braw)
            var s = set_tts_sigsum_cls(fixed_array(ld, rawd), fixed_array(long_length(logits) * 4l, braw), fixed_array(0u, B_RAW))
            var pa = TtsSigSumArgs(t = uint(SG_T), nb = uint(SG_NB), ls = uint(SG_LS), speed = SG_SPEED)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_sigsum_cls(raw, h, s, pa, tts_sigsum_wgs(int64(SG_T)))
                cmd_copy_planes(raw, h, fixed_array(rawd), host, sizes, B_RAW)
            }
            let got <- staged(host.mapped, at[0], SG_T, type<float>)
            let want <- sigsum_ref(logits, SG_T, SG_NB, SG_LS, SG_SPEED)
            let bad = mismatch_approx(got, want)
            let mi = maxdiff_at(got, want)
            to_log(LOG_INFO, "tts_sigsum: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi}\n")
            t |> success(bad == 0, "tts_sigsum: every row == the CPU sum ({bad} of {SG_T} off)")
            t |> success(!approx(got[SG_T - 1], want[SG_T - 1] + 0.5), "tts_sigsum: the bar reds a poisoned element")
            let want_all <- sigsum_ref(logits, SG_T, SG_LS, SG_LS, SG_SPEED)
            t |> success(mismatch_approx(got, want_all) > 0, "tts_sigsum control: the columns past nb stay out of the sum")
            destroy_device_buf(ld)
            destroy_device_buf(rawd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the AdaIN column statistics and the fused AdaIN stamps =====

//! the channel means, then the sums of squared deviations from them, of x [rows][c], in double
def private colstats_ref(x : array<float>; rows, c : int) : array<float> {
    var st : array<float>
    st |> resize(2 * c)
    for (ch in range(c)) {
        var s = 0.0lf
        for (r in range(rows)) {
            s += double(x[r * c + ch])
        }
        let mean = s / double(rows)
        var q = 0.0lf
        for (r in range(rows)) {
            let dv = double(x[r * c + ch]) - mean
            q += dv * dv
        }
        st[ch] = float(mean)
        st[c + ch] = float(q)
    }
    return <- st
}

//! one statistics shape: x [rows][c] off a mean, the stats plane under a NaN fill
def private colstats_arm(t : T?; rows, c : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_colstats t{rows} c{c}"
        let B_ST = 1u
        var x <- row_fill(rows * c, 53, 11, 0.05, 1.3)
        let xd = dev_upload(x)
        let bst = int64(2 * c) * 4l
        let std = make_device_buf(bst)
        let sizes = fixed_array(bst)
        let at <- staged_offsets(sizes)
        let host = make_host_buf(at[length(sizes)], true, [cached = true])
        nan_sentinel_upload(std, bst)
        var s = set_tts_colstats_cls(fixed_array(xd, std), fixed_array(long_length(x) * 4l, bst), fixed_array(0u, B_ST))
        var pa = TtsStatsArgs(t = uint(rows), c = uint(c))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_colstats_cls(raw, h, s, pa, int64(c))
            cmd_copy_planes(raw, h, fixed_array(std), host, sizes, B_ST)
        }
        let got <- staged(host.mapped, at[0], 2 * c, type<float>)
        let want <- colstats_ref(x, rows, c)
        let bad = mismatch_approx(got, want)
        let mi = maxdiff_at(got, want)
        let half = mi < c ? "mean" : "ssq"
        to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at {half} channel {mi % c}\n")
        t |> success(bad == 0, "{tag}: the means and squared deviations == the double-precision sums ({bad} of {2 * c} off)")
        t |> success(!approx(got[2 * c - 1], want[2 * c - 1] + 0.5), "{tag}: the bar reds a poisoned element")
        destroy_device_buf(xd)
        destroy_device_buf(std)
        destroy_host_buf(host)
    }
}

[test]
def test_vkt_colstats(t0 : T?) {
    t0 |> run("the column statistics == the double-precision mean and squared deviations, one and two strides a lane") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_colstats_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            colstats_arm(t, 45, 72)
            colstats_arm(t, 300, 20)   // lanes 0..43 take a second row
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private AD_T = 45
let private AD_C = 72
let private AD_SD = 16      //! the style vector's width
let private AD_EPS = 1e-5
let private AD_XOFF = 5     //! each plane's element base: the prefix before it is never written
let private AD_YOFF = 9
let private AD_HOFF = 7
let private AD_GWOFF = 3    //! the slab's norm scale row, then its shift row and Snake's alpha row, GARBAGE between

//! an AdaIN over AD_C channels from an AD_SD-wide style: fc w [2c][sd] and b, the norm's scale and shift rows
def private make_adain() : TtsAdaIn {
    var a = TtsAdaIn()
    a.fc.nin = int64(AD_SD)
    a.fc.nout = int64(2 * AD_C)
    a.fc.w <- row_fill(2 * AD_C * AD_SD, 37, 5, 0.004, 0.19)
    a.fc.b <- row_fill(2 * AD_C, 29, 11, 0.01, 0.45)
    a.norm.w <- row_fill(AD_C, 17, 3, 0.01, -0.5)
    a.norm.b <- row_fill(AD_C, 53, 1, 0.02, 0.9)
    return <- a
}

struct private AdainFixture {
    x : array<float>
    style_fc : array<float>   //! fc(style): gamma [c], then beta [c]
    slab : array<float>       //! the norm rows and alpha at AD_GWOFF, `gboff`, `aoff`
    want_leaky : array<float>
    want_snake : array<float>
    gboff : int
    aoff : int
}

def private adain_fixture() : AdainFixture {
    let c = AD_C
    var f = AdainFixture(gboff = AD_GWOFF + c + 3)
    f.aoff = f.gboff + c + 4
    var a <- make_adain()
    var style <- row_fill(AD_SD, 23, 9, 0.06, 2.8)
    var alpha <- row_fill(c, 23, 9, 0.02, -0.3)
    f.x <- row_fill(AD_T * c, 41, 7, 0.05, 2.1)
    f.slab |> resize(f.aoff + c + 5)
    for (v in f.slab) {
        v = GARBAGE
    }
    for (ci in range(c)) {
        f.slab[AD_GWOFF + ci] = a.norm.w[ci]
        f.slab[f.gboff + ci] = a.norm.b[ci]
        f.slab[f.aoff + ci] = alpha[ci]
    }
    let n = AD_T * c
    with_job_que() {
        setup_dasllama_jobque_()
        linear_vec(a.fc, style, f.style_fc)
        adain_rows_into(a, f.x, f.want_leaky, int64(AD_T), int64(c), style, AD_EPS)
    }
    f.want_snake := f.want_leaky
    leaky_relu(f.want_leaky, int64(n), 0.2)
    snake_rows(f.want_snake, int64(AD_T), int64(c), alpha)
    delete a
    delete style
    delete alpha
    return <- f
}

//! a stamp's rows against the CPU form: the bar, its poisoned element, the widest miss logged with its row and channel
def private adain_asserts(t : T?; tag : string; got, want : array<float>) {
    let n = length(want)
    let bad = mismatch_approx(got, want)
    let mi = maxdiff_at(got, want)
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / AD_C} channel {mi % AD_C}\n")
    t |> success(bad == 0, "{tag}: the device == the CPU adain_rows_into and activation ({bad} of {n} off)")
    t |> success(!approx(got[n - 1], want[n - 1] + 0.5), "{tag}: the bar reds a poisoned element")
}

[test]
def test_vkt_adain(t0 : T?) {
    t0 |> run("the column stats + fused AdaIN, leaky and Snake stamps, == the CPU adain_rows_into, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_colstats_cls() || !ensure_tts_adain_leaky_cls() || !ensure_tts_adain_snake_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_ST = 1u
            let B_YL = 2u
            let B_YS = 4u
            let B_XI = 8u
            let c = AD_C
            let n = AD_T * c
            var f <- adain_fixture()
            var xp <- with_base(f.x, AD_XOFF)
            var hp <- with_base(f.style_fc, AD_HOFF)
            let x0d = dev_upload(f.x)
            let xpd = dev_upload(xp)
            let xid = dev_upload(xp)
            let hd = dev_upload(hp)
            let wnd = dev_upload(f.slab)
            let bst = int64(2 * c) * 4l
            let by = int64(AD_YOFF + n) * 4l
            let bxp = long_length(xp) * 4l
            let bh = long_length(hp) * 4l
            let bwn = long_length(f.slab) * 4l
            let std = make_device_buf(bst)
            let yld = make_device_buf(by)
            let ysd = make_device_buf(by)
            let sizes = fixed_array(by, by, bxp)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(std, bst)
            nan_sentinel_upload(yld, by)
            nan_sentinel_upload(ysd, by)
            var s_st = set_tts_colstats_cls(fixed_array(x0d, std), fixed_array(long_length(f.x) * 4l, bst), fixed_array(0u, B_ST))
            var s_l = set_tts_adain_leaky_cls(fixed_array(xpd, yld, std, hd, wnd), fixed_array(bxp, by, bst, bh, bwn), fixed_array(0u, B_YL, B_ST, 0u, 0u))
            var s_s = set_tts_adain_snake_cls(fixed_array(xpd, ysd, std, hd, wnd), fixed_array(bxp, by, bst, bh, bwn), fixed_array(0u, B_YS, B_ST, 0u, 0u))
            var s_i = set_tts_adain_leaky_cls(fixed_array(xid, xid, std, hd, wnd), fixed_array(bxp, bxp, bst, bh, bwn), fixed_array(B_XI, B_XI, B_ST, 0u, 0u))
            var ps = TtsStatsArgs(t = uint(AD_T), c = uint(c))
            var pa = TtsAdainArgs(c = uint(c), t = uint(AD_T), eps = AD_EPS, xoff = uint(AD_XOFF), yoff = uint(AD_YOFF), hoff = uint(AD_HOFF),
                gwoff = uint(AD_GWOFF), gboff = uint(f.gboff), aoff = uint(f.aoff))
            var pi = pa
            pi.yoff = uint(AD_XOFF)
            let wgs = tts_adain_wgs(int64(n))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_colstats_cls(raw, h, s_st, ps, int64(c))
                enc_tts_adain_leaky_cls(raw, h, s_l, pa, wgs)
                enc_tts_adain_snake_cls(raw, h, s_s, pa, wgs)
                enc_tts_adain_leaky_cls(raw, h, s_i, pi, wgs)
                cmd_copy_planes(raw, h, fixed_array(yld, ysd, xid), host, sizes, B_YL | B_YS | B_XI)
            }
            let got_l <- staged(host.mapped, at[0] + int64(AD_YOFF) * 4l, n, type<float>)
            let got_s <- staged(host.mapped, at[1] + int64(AD_YOFF) * 4l, n, type<float>)
            let got_i <- staged(host.mapped, at[2] + int64(AD_XOFF) * 4l, n, type<float>)
            let pre_l <- staged(host.mapped, at[0], AD_YOFF, type<float>)
            let pre_s <- staged(host.mapped, at[1], AD_YOFF, type<float>)
            let pre_i <- staged(host.mapped, at[2], AD_XOFF, type<float>)
            adain_asserts(t, "tts_adain_leaky", got_l, f.want_leaky)
            adain_asserts(t, "tts_adain_snake", got_s, f.want_snake)
            adain_asserts(t, "tts_adain_leaky in place", got_i, f.want_leaky)
            t |> success(got_i[0] != f.x[0], "tts_adain_leaky in place control: the input row is overwritten")
            t |> equal(mismatch_exact(got_i, got_l), 0, "tts_adain_leaky: in place == out of place, bit for bit")
            t |> equal(nan_count(pre_l) + nan_count(pre_s), 2 * AD_YOFF, "tts_adain: the prefix before yoff keeps its sentinel")
            t |> success(elem_all(pre_i, GARBAGE), "tts_adain in place: the prefix before xoff is untouched")
            destroy_device_buf(x0d)
            destroy_device_buf(xpd)
            destroy_device_buf(xid)
            destroy_device_buf(hd)
            destroy_device_buf(wnd)
            destroy_device_buf(std)
            destroy_device_buf(yld)
            destroy_device_buf(ysd)
            destroy_host_buf(host)
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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

// ===== the residual join =====

let private AS_N = 1030     //! four whole workgroups and six more
let private AS_AOFF = 3
let private AS_BOFF = 7
let private AS_OOFF = 11
let private AS_SLACK = 5    //! elements past the run, left at the sentinel

[test]
def test_vkt_add_scale(t0 : T?) {
    t0 |> run("the add-and-scale == (a + b) * 1/sqrt(2) in f32, exact, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_add_scale_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_O = 1u
            let B_AI = 2u
            let n = AS_N
            let scale = 1.0 / sqrt(2.0)
            let a <- row_fill(n, 41, 7, 0.05, 2.1)
            let b <- row_fill(n, 17, 3, 0.04, 1.9)
            var ap <- with_base(a, AS_AOFF)
            var bp <- with_base(b, AS_BOFF)
            let ad = dev_upload(ap)
            let aid = dev_upload(ap)
            let bd = dev_upload(bp)
            let bo = int64(AS_OOFF + n + AS_SLACK) * 4l
            let bap = long_length(ap) * 4l
            let bbp = long_length(bp) * 4l
            let od = make_device_buf(bo)
            let sizes = fixed_array(bo, bap)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(od, bo)
            var s_o = set_tts_add_scale_cls(fixed_array(ad, bd, od), fixed_array(bap, bbp, bo), fixed_array(0u, 0u, B_O))
            var s_i = set_tts_add_scale_cls(fixed_array(aid, bd, aid), fixed_array(bap, bbp, bap), fixed_array(B_AI, 0u, B_AI))
            var po = TtsAddScaleArgs(nelem = uint(n), scale = scale, aoff = uint(AS_AOFF), boff = uint(AS_BOFF), ooff = uint(AS_OOFF))
            var pi = po
            pi.ooff = uint(AS_AOFF)
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_add_scale_cls(raw, h, s_o, po, tts_add_scale_wgs(int64(n)))
                enc_tts_add_scale_cls(raw, h, s_i, pi, tts_add_scale_wgs(int64(n)))
                cmd_copy_planes(raw, h, fixed_array(od, aid), host, sizes, B_O | B_AI)
            }
            let got_o <- staged(host.mapped, at[0] + int64(AS_OOFF) * 4l, n, type<float>)
            let pre_o <- staged(host.mapped, at[0], AS_OOFF, type<float>)
            let tail_o <- staged(host.mapped, at[0] + int64(AS_OOFF + n) * 4l, AS_SLACK, type<float>)
            let got_i <- staged(host.mapped, at[1] + int64(AS_AOFF) * 4l, n, type<float>)
            let pre_i <- staged(host.mapped, at[1], AS_AOFF, type<float>)
            let want <- [for (i in range(n)); (a[i] + b[i]) * scale]
            let bad = mismatch_exact(got_o, want)
            t |> success(bad == 0, "tts_add_scale: every element == (a + b) * scale ({bad} of {n} off)")
            t |> success(got_o[n - 1] + 0.5 != want[n - 1], "tts_add_scale: the bar reds a poisoned element")
            let bad_i = mismatch_exact(got_i, want)
            t |> success(bad_i == 0, "tts_add_scale in place: every element == (a + b) * scale ({bad_i} of {n} off)")
            t |> success(got_i[0] + 0.5 != want[0], "tts_add_scale in place: the bar reds a poisoned element")
            t |> success(got_i[0] != a[0], "tts_add_scale in place control: the input row is overwritten")
            t |> equal(nan_count(pre_o) + nan_count(tail_o), AS_OOFF + AS_SLACK, "tts_add_scale: the elements outside the run keep their sentinel")
            t |> success(elem_all(pre_i, GARBAGE), "tts_add_scale in place: the prefix before aoff is untouched")
            destroy_device_buf(ad)
            destroy_device_buf(aid)
            destroy_device_buf(bd)
            destroy_device_buf(od)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
