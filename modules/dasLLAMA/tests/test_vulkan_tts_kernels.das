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
require daslib/jobque_boost

//! The TTS tower's Vulkan kernel classes against their CPU oracles (a Vulkan device, else the
//! cells skip): the f32-exact tile GEMM the front end feeds on, in both its stamps, at shapes off
//! every tile and workgroup multiple, with the poisoned-element control on every compare.

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
            var nan_count = 0
            for (v in y_dev) {
                if (v != v) {
                    nan_count++
                }
            }
            t |> equal(nan_count, 0, "every output element is stored (no sentinel survives)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
