options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_math
require ?vulkan _vkd_toy
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_vulkan_seams
require ?vulkan dasllama/dasllama_kqformat
require daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane

// The model-less Vulkan kernel-unit suite ([vk_dispatch] bring-up): a class kernel dispatched
// through the GENERATED surface (ensure/set/enc — per-class layout, push constants, hazard rail)
// against the CPU run of the SAME method (the class body is ordinary daslang; the driver loop
// feeds gl_GlobalInvocationID). Without dasVulkan (or with no device) every check feints cleanly.

[test]
def test_vkd_class_dispatch(t0 : T?) {
    t0 |> run("class kernel through the generated [vk_dispatch] surface == CPU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_vkd_toy()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let n = 250   // deliberately not a multiple of 64 — the in-kernel guard covers the pad
            let bytes = int64(n) * 4l
            var xs : array<float>
            xs |> resize(n)
            for (i in range(n)) {
                xs[i] = float(i) * 0.25 - 7.0
            }
            // CPU oracle: the same method on an instance with plain arrays; the loop overruns the
            // logical range exactly like the padded GPU grid does, so the guard is under test too
            var k = new VkdToy()
            k.xs := xs
            k.ys |> resize(n)
            k.pa = VtArgs(n = uint(n), gain = 2.5)
            for (i in range(n + 6)) {
                gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
                k->run()
            }
            // device: SSBOs + the generated set/encoder, one submit, readback
            let xdev = make_device_buf(bytes)
            let ydev = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var gpu : array<float>
            gpu |> resize(n)
            unsafe {
                upload_region_at(xdev, 0l, addr<uint8 const?>(xs[0]), bytes)
                var s = set_vkd_toy(fixed_array(xdev, ydev), fixed_array(bytes, bytes), fixed_array(1u, 2u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = VtArgs(n = uint(n), gain = 2.5)
                enc_vkd_toy(raw, h, s, pc, int64(n))
                vhz_dep(raw, h, 2u, 0u, true)   // ys (bit 2) reads back over the copy engine
                cmd_copy_whole(raw, ydev, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(gpu[0]), host.mapped, bytes)
            }
            var bad = 0
            for (i in range(n)) {
                if (gpu[i] != k.ys[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "GPU == CPU on all {n} lanes ({bad} diverge)")
            delete xs
            delete gpu
            unsafe {
                delete k
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_ar_class(t0 : T?) {
    t0 |> run("ar_add_rms class twin == the vk_add_rms seam (bit-exact, same reduce order)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let dim = 1024
            let bytes = int64(dim) * 4l
            let eps = 1e-5
            let ascale = 1.44
            var x : array<float>
            var a : array<float>
            var w : array<float>
            x |> resize(dim)
            a |> resize(dim)
            w |> resize(dim)
            for (i in range(dim)) {
                x[i] = float((i * 37) % 101) * 0.11 - 5.0
                a[i] = float((i * 13) % 89) * 0.07 - 3.0
                w[i] = 0.5 + float(i % 17) * 0.05
            }
            // the seam is the reference: the SHIPPED kernel on the same device, same inputs
            var x2 := x
            var y2 : array<float>
            y2 |> resize(dim)
            vk_add_rms(x2, a, w, y2, int64(dim), eps, ascale, true)
            // the class twin through the generated surface
            let xd = make_device_buf(bytes)
            let ad = make_device_buf(bytes)
            let wd = make_device_buf(bytes)
            let yd = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var yc : array<float>
            var xc : array<float>
            yc |> resize(dim)
            xc |> resize(dim)
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bytes)
                upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), bytes)
                upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), bytes)
                var s = set_cls_ar(fixed_array(xd, ad, wd, yd), fixed_array(bytes, bytes, bytes, bytes), fixed_array(1u, 2u, 4u, 8u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                enc_cls_ar(raw, h, s, pc, 1l)
                vhz_dep(raw, h, 8u | 1u, 0u, true)   // y + the updated x read back over the copy engine
                cmd_copy_whole(raw, yd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(yc[0]), host.mapped, bytes)
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                cmd_copy_whole(raw2, xd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(xc[0]), host.mapped, bytes)
            }
            var bad_y = 0
            var bad_x = 0
            for (i in range(dim)) {
                if (yc[i] != y2[i]) {
                    bad_y++
                }
                if (xc[i] != x2[i]) {
                    bad_x++
                }
            }
            t |> success(bad_y == 0, "normed output bit-exact vs the seam ({bad_y} of {dim} diverge)")
            t |> success(bad_x == 0, "residual stream bit-exact vs the seam ({bad_x} of {dim} diverge)")
            delete x
            delete a
            delete w
            delete x2
            delete y2
            delete yc
            delete xc
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

// ===== the kq GEMV family: class twins vs the SHIPPED kernels, per format =====
// Synthetic planes — any bit pattern is valid K-quant data, EXCEPT the scale words, which are
// composed from finite halves so a NaN can't poison the bit-compare. Identical inputs, identical
// lane geometry (64-wide, one lane per 32-block) => the class twin must match BIT-EXACTLY.

def private safe_half(i : uint) : uint {
    let tab = fixed_array(0x3C00u, 0xB800u, 0x3400u, 0x4200u, 0x2E00u, 0xBC00u, 0x3E00u, 0xB000u)
    return tab[i % 8u]
}

def private ws_word(i : uint) : uint {
    return safe_half(i * 7u + 3u) | (safe_half(i * 3u + 1u) << 16u)
}

def private hash_word(i : uint) : uint {
    var h = i * 2654435761u
    h ^= h >> 16u
    h *= 2246822519u
    return h ^ (h >> 13u)
}

[test]
def test_vkd_kq_gemv_family(t0 : T?) {
    t0 |> run("kq GEMV class family == the shipped kernels (bit-exact, all 4 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_kq_gemv_k4_cls() && ensure_kq_gemv_q40_cls() && ensure_kq_gemv_k5_cls() && ensure_kq_gemv_k6_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nsb = 2      // superblocks per row reduction (n = 512)
            let d = 3        // rows per region
            let nreg = 2
            let rows = nreg * d
            let nfmt = 4
            let fmts = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6))
            let stride4s = fixed_array(8, 8, 10, 12)   // uint4s per superblock
            for (fi in range(nfmt)) {
                let fmt = fmts[fi]
                let stride4 = stride4s[fi]
                let nwsb = rows * nsb                       // region r's weights at r*d*nsb
                let wq_words = nwsb * stride4 * 4
                let ws_words_n = nwsb * 5
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(wq_words)
                wsh |> resize(ws_words_n)
                xqh |> resize(nsb * 64)                     // nsb superblocks x 64 uints
                xsh |> resize(nsb)
                for (i in range(wq_words)) {
                    wqh[i] = hash_word(uint(i) + uint(fmt) * 77777u)
                }
                for (i in range(ws_words_n)) {
                    wsh[i] = ws_word(uint(i) + uint(fmt) * 13u)
                }
                for (i in range(nsb * 64)) {
                    xqh[i] = hash_word(uint(i) * 31u + 5u)
                }
                for (i in range(nsb)) {
                    xsh[i] = 0.5 + float(i % 7) * 0.25
                }
                let wq_bytes = int64(wq_words) * 4l
                let ws_bytes = int64(ws_words_n) * 4l
                let xq_bytes = int64(nsb) * 256l
                let xs_bytes = int64(nsb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd1 = make_device_buf(y_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(rows)
                y_cls |> resize(rows)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    var regs : array<uint>
                    regs |> resize(nreg * 2)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = 512u
                    mp[1] = uint(d)
                    mp[2] = uint(nreg)
                    mp[3] = 0u
                    for (r in range(nreg)) {
                        mp[4 + r * 2] = uint(r * d * nsb)
                        mp[5 + r * 2] = 0u
                        regs[r * 2] = uint(r * d * nsb)
                        regs[r * 2 + 1] = 0u
                    }
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    // the SHIPPED kernel
                    var s_old = vk_set6(wqd, wsd, meta.buf, xqd, xsd, yd1,
                        wq_bytes, ws_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                        fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, gemv_pipe(fmt), s_old, uint(wgs))
                    vhz_dep(raw, h, 32u, 0u, true)
                    cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                    // the class twin — same planes, its own regions SSBO + push params
                    let cb = fixed_array(wqd, wsd, rgd, xqd, xsd, yd2)
                    let cs = fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes)
                    let cg = fixed_array(1u, 2u, 4u, 8u, 16u, 32u)
                    var s_cls : VkDescriptorSet
                    if (fmt == int(KqFmt.k4)) {
                        s_cls = set_kq_gemv_cls(cb, cs, cg)
                    } elif (fmt == int(KqFmt.q40)) {
                        s_cls = set_kq_gemv_cls(cb, cs, cg)
                    } elif (fmt == int(KqFmt.k5)) {
                        s_cls = set_kq_gemv_cls(cb, cs, cg)
                    } else {
                        s_cls = set_kq_gemv_cls(cb, cs, cg)
                    }
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = 512u, d = uint(d), nreg = uint(nreg), ybase = 0u)
                    if (fmt == int(KqFmt.k4)) {
                        enc_kq_gemv_k4_cls(raw2, h2, s_cls, pc, wgs)
                    } elif (fmt == int(KqFmt.q40)) {
                        enc_kq_gemv_q40_cls(raw2, h2, s_cls, pc, wgs)
                    } elif (fmt == int(KqFmt.k5)) {
                        enc_kq_gemv_k5_cls(raw2, h2, s_cls, pc, wgs)
                    } else {
                        enc_kq_gemv_k6_cls(raw2, h2, s_cls, pc, wgs)
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                    delete regs
                }
                var bad = 0
                for (i in range(rows)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "fmt {fmt}: class twin bit-exact vs shipped ({bad} of {rows} rows diverge)")
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_old
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_act_family(t0 : T?) {
    t0 |> run("act/requant + combine class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_q8_actrq_cls() && ensure_q8k_actrq_cls() && ensure_combine_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nelem = 1024
            var gh : array<float>
            var uh : array<float>
            gh |> resize(nelem)
            uh |> resize(nelem)
            for (i in range(nelem)) {
                gh[i] = float((i * 29) % 61) * 0.21 - 6.0
                uh[i] = float((i * 11) % 47) * 0.17 - 4.0
            }
            let e_bytes = int64(nelem) * 4l
            let gd = make_device_buf(e_bytes)
            let ud = make_device_buf(e_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            unsafe {
                upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), e_bytes)
                upload_region_at(ud, 0l, addr<uint8 const?>(uh[0]), e_bytes)
            }
            // arm 0 = q8 (32-blocks), arm 1 = q8k (256-superblocks)
            for (arm in range(2)) {
                let q8k = arm == 1
                let nblk = q8k ? nelem / 256 : nelem / 32
                let q_bytes = int64(nelem)          // 1 byte per quant either way
                let s_bytes = int64(nblk) * 4l
                let hq1 = make_device_buf(q_bytes)
                let hs1 = make_device_buf(s_bytes)
                let hq2 = make_device_buf(q_bytes)
                let hs2 = make_device_buf(s_bytes)
                var hostq = make_host_buf(q_bytes, true, [cached = true])
                var hosts = make_host_buf(s_bytes, true, [cached = true])
                var q_old : array<uint>
                var q_cls : array<uint>
                var s_old : array<float>
                var s_cls : array<float>
                q_old |> resize(nelem / 4)
                q_cls |> resize(nelem / 4)
                s_old |> resize(nblk)
                s_cls |> resize(nblk)
                let wgs = 1l   // 256 threads cover both shapes at nelem = 1024
                unsafe {
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(nelem)
                    mp[1] = 0u   // silu
                    mp[2] = uint(nblk)
                    // the SHIPPED kernel: up@0, filler@1, meta@2, hq@3, hs@4, gate@5
                    var so = vk_set6(ud, ud, meta.buf, hq1, hs1, gd,
                        e_bytes, e_bytes, META_BYTES, q_bytes, s_bytes, e_bytes,
                        fixed_array(1u, 0u, 2u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(q8k ? g_gpu.actrq_k_pipeline : g_gpu.actrq_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw, hq1, hostq.buf, q_bytes)
                    cmd_copy_whole(raw, hs1, hosts.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_old[0]), hostq.mapped, q_bytes)
                    memcpy(addr<void?>(s_old[0]), hosts.mapped, s_bytes)
                    // the class twin: gate@0, up@1, outq@2, outs@3
                    var sc : VkDescriptorSet
                    if (q8k) {
                        sc = set_actrq_cls(fixed_array(gd, ud, hq2, hs2),
                            fixed_array(e_bytes, e_bytes, q_bytes, s_bytes),
                            fixed_array(16u, 1u, 4u, 8u))
                    } else {
                        sc = set_actrq_cls(fixed_array(gd, ud, hq2, hs2),
                            fixed_array(e_bytes, e_bytes, q_bytes, s_bytes),
                            fixed_array(16u, 1u, 4u, 8u))
                    }
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = ActArgs(nelem = uint(nelem), gelu = 0u, nblk = uint(nblk))
                    if (q8k) {
                        enc_q8k_actrq_cls(raw2, h2, sc, pc, wgs)
                    } else {
                        enc_q8_actrq_cls(raw2, h2, sc, pc, wgs)
                    }
                    vhz_dep(raw2, h2, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw2, hq2, hostq.buf, q_bytes)
                    cmd_copy_whole(raw2, hs2, hosts.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), hostq.mapped, q_bytes)
                    memcpy(addr<void?>(s_cls[0]), hosts.mapped, s_bytes)
                }
                var bad = 0
                for (i in range(nelem / 4)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                for (i in range(nblk)) {
                    if (s_old[i] != s_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "{q8k ? "q8k" : "q8"} actrq: class twin bit-exact ({bad} words/scales diverge)")
                delete q_old
                delete q_cls
                delete s_old
                delete s_cls
            }
            // the combine
            {
                let n = 64
                let k = 4
                let npos = 3
                let nrows = 8
                var sidx : array<uint>
                var sw : array<float>
                var rows : array<float>
                var accs : array<float>
                sidx |> resize(npos * k)
                sw |> resize(npos * k)
                rows |> resize(nrows * n)
                accs |> resize(npos * n)
                for (i in range(npos * k)) {
                    sidx[i] = hash_word(uint(i)) % uint(nrows)
                    sw[i] = 0.125 + float(i % 5) * 0.25
                }
                for (i in range(nrows * n)) {
                    rows[i] = float((i * 7) % 93) * 0.13 - 5.5
                }
                for (i in range(npos * n)) {
                    accs[i] = 0.0
                }
                let si_bytes = int64(npos * k) * 4l
                let ro_bytes = int64(nrows * n) * 4l
                let ac_bytes = int64(npos * n) * 4l
                let sid = make_device_buf(si_bytes)
                let swd = make_device_buf(si_bytes)
                let rod = make_device_buf(ro_bytes)
                let ac1 = make_device_buf(ac_bytes)
                let ac2 = make_device_buf(ac_bytes)
                var hosta = make_host_buf(ac_bytes, true, [cached = true])
                var a_old : array<float>
                var a_cls : array<float>
                a_old |> resize(npos * n)
                a_cls |> resize(npos * n)
                unsafe {
                    upload_region_at(sid, 0l, addr<uint8 const?>(sidx[0]), si_bytes)
                    upload_region_at(swd, 0l, addr<uint8 const?>(sw[0]), si_bytes)
                    upload_region_at(rod, 0l, addr<uint8 const?>(rows[0]), ro_bytes)
                    upload_region_at(ac1, 0l, addr<uint8 const?>(accs[0]), ac_bytes)
                    upload_region_at(ac2, 0l, addr<uint8 const?>(accs[0]), ac_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[4] = uint(n)
                    mp[5] = uint(k)
                    mp[6] = 0u
                    mp[7] = uint(nrows)
                    // the SHIPPED combine: acc(upf)@0, meta@2, slot idx(xq)@3, slot w(xs)@4, rows(y)@5
                    var so = vk_set6(ac1, sid, meta.buf, sid, swd, rod,
                        ac_bytes, si_bytes, META_BYTES, si_bytes, si_bytes, ro_bytes,
                        fixed_array(1u, 0u, 2u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.comb_pipeline), so, uint(npos))
                    vhz_dep(raw, h, 1u, 0u, true)
                    cmd_copy_whole(raw, ac1, hosta.buf, ac_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(a_old[0]), hosta.mapped, ac_bytes)
                    // the class twin
                    var sc = set_combine_cls(fixed_array(ac2, sid, swd, rod),
                        fixed_array(ac_bytes, si_bytes, si_bytes, ro_bytes),
                        fixed_array(1u, 2u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = CombineArgs(n = uint(n), k = uint(k), r0 = 0u, r1 = uint(nrows))
                    enc_combine_cls(raw2, h2, sc, pc, int64(npos))
                    vhz_dep(raw2, h2, 1u, 0u, true)
                    cmd_copy_whole(raw2, ac2, hosta.buf, ac_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(a_cls[0]), hosta.mapped, ac_bytes)
                }
                var bad = 0
                for (i in range(npos * n)) {
                    if (a_old[i] != a_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "combine: class twin bit-exact ({bad} of {npos * n} diverge)")
                delete sidx
                delete sw
                delete rows
                delete accs
                delete a_old
                delete a_cls
            }
            delete gh
            delete uh
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_rope_family(t0 : T?) {
    t0 |> run("rope_kv_store + qk_rms class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_cls() && ensure_qk_rms_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let hs = 64
            let half = hs / 2
            let nh = 4
            let nkvh = 2
            let qd = nh * hs
            let kvd = nkvh * hs
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            let begin = VkCommandBufferBeginInfo()
            // ===== rope + KV store =====
            {
                let npairs = (nh + nkvh) * half
                let kpair0 = nh * half
                var qh : array<float>
                var kvh : array<float>
                var csh : array<float>
                qh |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                for (i in range(qd)) {
                    qh[i] = float((i * 23) % 71) * 0.09 - 3.0
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 19) % 83) * 0.07 - 2.9
                }
                for (i in range(half)) {
                    csh[i] = 0.3 + float(i % 9) * 0.07          // cos row
                    csh[half + i] = 0.1 + float(i % 7) * 0.11   // sin row
                }
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let m_bytes = int64(kvd) * 4l
                let cs_bytes = int64(hs) * 4l
                let kvd_dev = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let q1 = make_device_buf(q_bytes)
                let q2 = make_device_buf(q_bytes)
                let km1 = make_device_buf(m_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm1 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(kv_bytes, true, [cached = true])
                var old_q : array<float>
                var cls_q : array<float>
                var old_k : array<float>
                var cls_k : array<float>
                var old_v : array<float>
                var cls_v : array<float>
                old_q |> resize(qd)
                cls_q |> resize(qd)
                old_k |> resize(kvd)
                cls_k |> resize(kvd)
                old_v |> resize(kvd)
                cls_v |> resize(kvd)
                let wgs = int64((npairs + 255) / 256)
                unsafe {
                    upload_region_at(kvd_dev, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(q1, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(qd)
                    mp[1] = uint(kvd)
                    mp[2] = uint(hs)
                    mp[3] = uint(half)
                    mp[4] = 1u   // neox
                    mp[5] = uint(kpair0)
                    mp[6] = uint(npairs)
                    mp[7] = 0u
                    mp[8] = 0u
                    // the SHIPPED kernel: kvsrc@0, K mirror@1, meta@2, V mirror@3, cos@4, q@5
                    var so = vk_set6(kvd_dev, km1, meta.buf, vm1, csd, q1,
                        kv_bytes, m_bytes, META_BYTES, m_bytes, cs_bytes, q_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.rks_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 2u | 4u | 16u, 0u, true)
                    cmd_copy_whole(raw, q1, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(old_q[0]), host.mapped, q_bytes)
                    var rawb = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawb, begin), null)
                    cmd_copy_whole(rawb, km1, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(rawb), null)
                    submit_wait(rawb)
                    memcpy(addr<void?>(old_k[0]), host.mapped, m_bytes)
                    var rawc = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawc, begin), null)
                    cmd_copy_whole(rawc, vm1, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(rawc), null)
                    submit_wait(rawc)
                    memcpy(addr<void?>(old_v[0]), host.mapped, m_bytes)
                    // the class twin: q@0, kvsrc@1, kmir@2, vmir@3, cossin@4
                    var sc = set_rope_kv_cls(fixed_array(q2, kvd_dev, km2, vm2, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes),
                        fixed_array(16u, 1u, 2u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(half),
                        neox = 1u, kpair0 = uint(kpair0), npairs = uint(npairs), krow = 0u, vrow = 0u)
                    enc_rope_kv_cls(raw2, h2, sc, pc, wgs)
                    vhz_dep(raw2, h2, 2u | 4u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(cls_q[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, km2, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(cls_k[0]), host.mapped, m_bytes)
                    var raw2c = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2c, begin), null)
                    cmd_copy_whole(raw2c, vm2, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(raw2c), null)
                    submit_wait(raw2c)
                    memcpy(addr<void?>(cls_v[0]), host.mapped, m_bytes)
                }
                var bad = 0
                for (i in range(qd)) {
                    if (old_q[i] != cls_q[i]) {
                        bad++
                    }
                }
                for (i in range(kvd)) {
                    if (old_k[i] != cls_k[i]) {
                        bad++
                    }
                    if (old_v[i] != cls_v[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "rope_kv_store: class twin bit-exact ({bad} elements diverge)")
                delete qh
                delete kvh
                delete csh
                delete old_q
                delete cls_q
                delete old_k
                delete cls_k
                delete old_v
                delete cls_v
            }
            // ===== qk_rms =====
            {
                let rows = 2
                let qstride = qd
                let kstride = kvd
                let eps = 1e-5
                var qh : array<float>
                var kh : array<float>
                var wh : array<float>
                var eh : array<float>
                qh |> resize(rows * qd)
                kh |> resize(rows * kvd)
                wh |> resize(2 * hs)
                eh |> resize(1)
                for (i in range(rows * qd)) {
                    qh[i] = float((i * 31) % 67) * 0.08 - 2.5
                }
                for (i in range(rows * kvd)) {
                    kh[i] = float((i * 43) % 59) * 0.06 - 1.7
                }
                for (i in range(2 * hs)) {
                    wh[i] = 0.4 + float(i % 13) * 0.06
                }
                eh[0] = eps
                let q_bytes = int64(rows * qd) * 4l
                let k_bytes = int64(rows * kvd) * 4l
                let w_bytes = int64(2 * hs) * 4l
                let q1 = make_device_buf(q_bytes)
                let q2 = make_device_buf(q_bytes)
                let k1 = make_device_buf(k_bytes)
                let k2 = make_device_buf(k_bytes)
                let wd = make_device_buf(w_bytes)
                let ed = make_device_buf(4l)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var old_q : array<float>
                var cls_q : array<float>
                var old_k : array<float>
                var cls_k : array<float>
                old_q |> resize(rows * qd)
                cls_q |> resize(rows * qd)
                old_k |> resize(rows * kvd)
                cls_k |> resize(rows * kvd)
                let wgs = int64(rows * (nh + nkvh))
                unsafe {
                    upload_region_at(q1, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(k1, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(k2, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
                    upload_region_at(ed, 0l, addr<uint8 const?>(eh[0]), 4l)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(hs)
                    mp[1] = uint(nh)
                    mp[2] = uint(nkvh)
                    mp[3] = uint(qstride)
                    mp[4] = uint(kstride)
                    mp[5] = 0u
                    mp[6] = 0u
                    mp[7] = uint(hs)
                    // the SHIPPED kernel: q@0, filler@1, meta@2, weights@3, eps@4, k@5
                    var so = vk_set6(q1, wd, meta.buf, wd, ed, k1,
                        q_bytes, w_bytes, META_BYTES, w_bytes, 4l, k_bytes,
                        fixed_array(1u, 0u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.qkn_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 1u | 16u, 0u, true)
                    cmd_copy_whole(raw, q1, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(old_q[0]), host.mapped, q_bytes)
                    var rawb = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawb, begin), null)
                    cmd_copy_whole(rawb, k1, host.buf, k_bytes)
                    vk_check(vkEndCommandBuffer(rawb), null)
                    submit_wait(rawb)
                    memcpy(addr<void?>(old_k[0]), host.mapped, k_bytes)
                    // the class twin: q@0, k@1, weights@2
                    var sc = set_qk_rms_cls(fixed_array(q2, k2, wd),
                        fixed_array(q_bytes, k_bytes, w_bytes),
                        fixed_array(1u, 16u, 4u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = QkRmsArgs(hs = uint(hs), nh = uint(nh), nkvh = uint(nkvh),
                        qstride = uint(qstride), kstride = uint(kstride), kbase = 0u,
                        qwoff = 0u, kwoff = uint(hs), eps = eps)
                    enc_qk_rms_cls(raw2, h2, sc, pc, wgs)
                    vhz_dep(raw2, h2, 1u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(cls_q[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, k2, host.buf, k_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(cls_k[0]), host.mapped, k_bytes)
                }
                var bad = 0
                for (i in range(rows * qd)) {
                    if (old_q[i] != cls_q[i]) {
                        bad++
                    }
                }
                for (i in range(rows * kvd)) {
                    if (old_k[i] != cls_k[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "qk_rms: class twin bit-exact ({bad} elements diverge)")
                delete qh
                delete kh
                delete wh
                delete eh
                delete old_q
                delete cls_q
                delete old_k
                delete cls_k
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_q8_batch_tile(t0 : T?) {
    t0 |> run("q8 batch tile class twin == the shipped kernel (bit-exact, edge tiles + K tail)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let n = 96                 // nbb = 3: exercises the zero-filled K tail (nsteps = 1, 8-block pass)
            let nbb = n / 32
            let d = 40                 // wtiles = 2 with a 8-col edge tile
            let cnt0 = 35              // region 0: 2 x-tiles with a 3-row edge
            let cnt1 = 20              // region 1: 1 x-tile
            let rows = cnt0 + cnt1
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totblk = 2 * d * nbb
            var wqh : array<uint>
            var wsh : array<uint>
            var xqh : array<uint>
            var xsh : array<float>
            wqh |> resize(totblk * 8)
            wsh |> resize(totblk / 2 + 1)
            xqh |> resize(rows * nbb * 8)
            xsh |> resize(rows * nbb)
            for (i in range(totblk * 8)) {
                wqh[i] = hash_word(uint(i) + 3u)
            }
            for (i in range(totblk / 2 + 1)) {
                wsh[i] = ws_word(uint(i) + 7u)
            }
            for (i in range(rows * nbb * 8)) {
                xqh[i] = hash_word(uint(i) * 13u + 1u)
            }
            for (i in range(rows * nbb)) {
                xsh[i] = 0.375 + float(i % 9) * 0.0625
            }
            let wq_bytes = int64(totblk) * 32l
            let ws_bytes = int64(totblk) * 2l
            let xq_bytes = int64(rows * nbb) * 32l
            let xs_bytes = int64(rows * nbb) * 4l
            let y_bytes = int64(rows * d) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let yd1 = make_device_buf(y_bytes)
            let yd2 = make_device_buf(y_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_old : array<float>
            var y_cls : array<float>
            y_old |> resize(rows * d)
            y_cls |> resize(rows * d)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                // records: [wblk0, row0, cnt, wg0]; region 1's weights follow region 0's stack
                var sched : array<uint>
                sched |> resize(2 * 4 + wgs)
                sched[0] = 0u
                sched[1] = 0u
                sched[2] = uint(cnt0)
                sched[3] = 0u
                sched[4] = uint(d * nbb)
                sched[5] = uint(cnt0)
                sched[6] = uint(cnt1)
                sched[7] = uint(wgs0)
                for (w in range(wgs)) {
                    sched[8 + w] = w < wgs0 ? 0u : 1u
                }
                // the SHIPPED kernel's meta: same words behind a 4-word header (records at 4, map ptr in [3])
                var mp = reinterpret<uint?>(meta.mapped)
                mp[0] = uint(n)
                mp[1] = uint(d)
                mp[3] = uint(4 + 2 * 4)
                for (i in range(2 * 4 + wgs)) {
                    mp[4 + i] = sched[i]
                }
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                var so = vk_set6(wqd, wsd, meta.buf, xqd, xsd, yd1,
                    wq_bytes, ws_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                hz_dispatch(raw, h, weak_copy(g_gpu.batch_pipeline), so, uint(wgs))
                vhz_dep(raw, h, 32u, 0u, true)
                cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                // the class twin: records at 0, map at map_off
                var sc = set_q8_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8_batch_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                delete sched
            }
            var bad = 0
            for (i in range(rows * d)) {
                if (y_old[i] != y_cls[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "q8 batch: class twin bit-exact ({bad} of {rows * d} diverge)")
            delete wqh
            delete wsh
            delete xqh
            delete xsh
            delete y_old
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_kq_batch_family(t0 : T?) {
    t0 |> run("kq batch tile class family == the shipped kernels (bit-exact, all 4 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_kq_batch_k4_cls() && ensure_kq_batch_q40_cls() && ensure_kq_batch_k5_cls() && ensure_kq_batch_k6_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let n = 512               // nsb = 2 superblocks
            let nsb = n / 256
            let d = 40                // 2 w-tiles with an 8-col edge
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totsb = 2 * d * nsb   // superblocks across both regions
            let nfmt = 4
            let wstrides = fixed_array(32, 32, 40, 48)   // uint words per superblock: k4 q40 k5 k6
            let pipes = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6))
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            for (fi in range(nfmt)) {
                let fmt = pipes[fi]
                let wstride = wstrides[fi]
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(totsb * wstride)
                wsh |> resize(totsb * 5)
                xqh |> resize(rows * nsb * 64)
                xsh |> resize(rows * nsb)
                for (i in range(totsb * wstride)) {
                    wqh[i] = hash_word(uint(i) + uint(fmt) * 131u)
                }
                for (i in range(totsb * 5)) {
                    wsh[i] = ws_word(uint(i) + uint(fmt) * 17u)
                }
                for (i in range(rows * nsb * 64)) {
                    xqh[i] = hash_word(uint(i) * 7u + 2u)
                }
                for (i in range(rows * nsb)) {
                    xsh[i] = 0.3 + float(i % 11) * 0.05
                }
                let wq_bytes = int64(totsb * wstride) * 4l
                let ws_bytes = int64(totsb * 5) * 4l
                let xq_bytes = int64(rows * nsb) * 256l
                let xs_bytes = int64(rows * nsb) * 4l
                let y_bytes = int64(rows * d) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd1 = make_device_buf(y_bytes)
                let yd2 = make_device_buf(y_bytes)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(rows * d)
                y_cls |> resize(rows * d)
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    var sched : array<uint>
                    sched |> resize(2 * 4 + wgs)
                    sched[0] = 0u
                    sched[1] = 0u
                    sched[2] = uint(cnt0)
                    sched[3] = 0u
                    sched[4] = uint(d * nsb)   // region 1's weights follow region 0's stack (superblock units)
                    sched[5] = uint(cnt0)
                    sched[6] = uint(cnt1)
                    sched[7] = uint(wgs0)
                    for (w in range(wgs)) {
                        sched[8 + w] = w < wgs0 ? 0u : 1u
                    }
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(n)
                    mp[1] = uint(d)
                    mp[3] = uint(4 + 2 * 4)
                    for (i in range(2 * 4 + wgs)) {
                        mp[4 + i] = sched[i]
                    }
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    var so = vk_set6(wqd, wsd, meta.buf, xqd, xsd, yd1,
                        wq_bytes, ws_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, gemv_pipe_batch_for_test(fmt), so, uint(wgs))
                    vhz_dep(raw, h, 32u, 0u, true)
                    cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                    var sc = (fmt == int(KqFmt.k4) ? set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2), fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 0u, 4u, 8u, 16u)) : VkDescriptorSet())
                    if (fmt == int(KqFmt.q40)) {
                        sc = set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2), fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    } elif (fmt == int(KqFmt.k5)) {
                        sc = set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2), fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    } elif (fmt == int(KqFmt.k6)) {
                        sc = set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2), fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    }
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    if (fmt == int(KqFmt.k4)) {
                        enc_kq_batch_k4_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.q40)) {
                        enc_kq_batch_q40_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.k5)) {
                        enc_kq_batch_k5_cls(raw2, h2, sc, pc, int64(wgs))
                    } else {
                        enc_kq_batch_k6_cls(raw2, h2, sc, pc, int64(wgs))
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                    delete sched
                }
                var bad = 0
                for (i in range(rows * d)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "kq batch fmt {fmt}: class twin bit-exact ({bad} of {rows * d} diverge)")
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_old
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cm_batch_family(t0 : T?) {
    t0 |> run("coopmat batch class twins == the shipped kernels (bit-exact, per available mode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // probe via a NON-coopmat ensure — a cm pipeline build
                feint("no Vulkan device — skipping\n")   // on a no-coopmat device panics, not feints
                return
            }
            var ran = 0
            // per variant: [tile, d, cnt0, cnt1, n] — cmi8 keeps rows/cols aligned (its loads are
            // unguarded); mm/mm_m mix full and edge tiles; mm_a is the all-aligned twin
            for (v in range(5)) {
                var tile = 32
                var d = 40
                var cnt0 = 35
                var cnt1 = 20
                var n = 96
                if (v == 1) {
                    d = 64
                    cnt0 = 32
                    cnt1 = 32
                } elif (v == 2) {
                    tile = 128
                    d = 192
                    cnt0 = 140
                    cnt1 = 116
                    n = 64
                } elif (v == 3) {
                    tile = 128
                    d = 128
                    cnt0 = 128
                    cnt1 = 128
                    n = 64
                } elif (v == 4) {
                    tile = 64
                    d = 96
                    cnt0 = 80
                    cnt1 = 48
                    n = 64
                }
                if (!key_exists(g_pipe_access, intptr(boost_value_to_vk(cm_pipe_for_test(v))))) {
                    continue   // this mode's pipe not created in this process
                }
                let up = (v == 0 ? ensure_q8_batch_cmf16_cls()
                    : (v == 1 ? ensure_q8_batch_cmi8_cls()
                    : (v == 2 ? ensure_q8_batch_mm_cls()
                    : (v == 3 ? ensure_q8_batch_mm_a_cls() : ensure_q8_batch_mm_m_cls()))))
                assert(up, "class rail must engage when the shipped pipe exists")
                ran++
                let nbb = n / 32
                let rows = cnt0 + cnt1
                let wtiles = (d + tile - 1) / tile
                let wgs0 = ((cnt0 + tile - 1) / tile) * wtiles
                let wgs1 = ((cnt1 + tile - 1) / tile) * wtiles
                let wgs = wgs0 + wgs1
                let totblk = 2 * d * nbb
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(totblk * 8)
                wsh |> resize(totblk / 2 + 1)
                xqh |> resize(rows * nbb * 8)
                xsh |> resize(rows * nbb)
                for (i in range(totblk * 8)) {
                    wqh[i] = hash_word(uint(i) + 3u)
                }
                for (i in range(totblk / 2 + 1)) {
                    wsh[i] = ws_word(uint(i) + 7u)
                }
                for (i in range(rows * nbb * 8)) {
                    xqh[i] = hash_word(uint(i) * 13u + 1u)
                }
                for (i in range(rows * nbb)) {
                    xsh[i] = 0.375 + float(i % 9) * 0.0625
                }
                let wq_bytes = int64(totblk) * 32l
                let ws_bytes = int64(totblk) * 2l
                let xq_bytes = int64(rows * nbb) * 32l
                let xs_bytes = int64(rows * nbb) * 4l
                let y_bytes = int64(rows * d) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd1 = make_device_buf(y_bytes)
                let yd2 = make_device_buf(y_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(rows * d)
                y_cls |> resize(rows * d)
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    var sched : array<uint>
                    sched |> resize(2 * 4 + wgs)
                    sched[0] = 0u
                    sched[1] = 0u
                    sched[2] = uint(cnt0)
                    sched[3] = 0u
                    sched[4] = uint(d * nbb)
                    sched[5] = uint(cnt0)
                    sched[6] = uint(cnt1)
                    sched[7] = uint(wgs0)
                    for (w in range(wgs)) {
                        sched[8 + w] = w < wgs0 ? 0u : 1u
                    }
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(n)
                    mp[1] = uint(d)
                    mp[3] = uint(4 + 2 * 4)
                    for (i in range(2 * 4 + wgs)) {
                        mp[4 + i] = sched[i]
                    }
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    var so = vk_set6(wqd, wsd, meta.buf, xqd, xsd, yd1,
                        wq_bytes, ws_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, cm_pipe_for_test(v), so, uint(wgs))
                    vhz_dep(raw, h, 32u, 0u, true)
                    cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                    let bufs = fixed_array(wqd, wsd, scd, xqd, xsd, yd2)
                    let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes)
                    let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
                    var sc = (v == 0 ? set_q8_batch_cmf16_cls(bufs, sizes, gbits)
                        : (v == 1 ? set_q8_batch_cmi8_cls(bufs, sizes, gbits)
                        : (v == 2 ? set_q8_batch_mm_cls(bufs, sizes, gbits)
                        : (v == 3 ? set_q8_batch_mm_a_cls(bufs, sizes, gbits)
                        : set_q8_batch_mm_m_cls(bufs, sizes, gbits)))))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    if (v == 0) {
                        enc_q8_batch_cmf16_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 1) {
                        enc_q8_batch_cmi8_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 2) {
                        enc_q8_batch_mm_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 3) {
                        enc_q8_batch_mm_a_cls(raw2, h2, sc, pc, int64(wgs))
                    } else {
                        enc_q8_batch_mm_m_cls(raw2, h2, sc, pc, int64(wgs))
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                    delete sched
                }
                var bad = 0
                for (i in range(rows * d)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                to_log(LOG_INFO, "cm batch variant {v}: {bad} of {rows * d} diverge\n")
                t |> success(bad == 0, "cm batch variant {v}: class twin bit-exact ({bad} of {rows * d} diverge)")
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_old
                delete y_cls
            }
            if (ran == 0) {
                feint("no coopmat batch pipes in this mode — set DASLLAMA_COOPMAT to cover\n")
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_kq_q40_cm_batch(t0 : T?) {
    t0 |> run("q40 coopmat f16 batch class twin == the shipped kernel (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.batch_coopmat_q40)))) {
                feint("q40 coopmat pipe not built (DASLLAMA_COOPMAT=f16 only) — skipping\n")
                return
            }
            verify(ensure_kq_batch_q40_cmf16_cls(), "class rail must engage when the shipped pipe exists")
            let n = 256                // nsb = 1 superblock per row reduction
            let nsb = n / 256
            let d = 40                 // wtiles = 2 with a 8-col edge tile
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totsb = 2 * d * nsb
            var wqh : array<uint>
            var wsuh : array<uint>
            var xqh : array<uint>
            var xsh : array<float>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 5)
            xqh |> resize(rows * nsb * 64)
            xsh |> resize(rows * nsb)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 3u)
            }
            for (i in range(totsb * 5)) {
                wsuh[i] = ws_word(uint(i) + 7u)   // d halves live here — every u16 a finite half
            }
            for (i in range(rows * nsb * 64)) {
                xqh[i] = hash_word(uint(i) * 13u + 1u)
            }
            for (i in range(rows * nsb)) {
                xsh[i] = 0.375 + float(i % 9) * 0.0625
            }
            let wq_bytes = int64(totsb) * 128l
            let wsu_bytes = int64(totsb) * 20l
            let xq_bytes = int64(rows * nsb) * 256l
            let xs_bytes = int64(rows * nsb) * 4l
            let y_bytes = int64(rows * d) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsud = make_device_buf(wsu_bytes)
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let yd1 = make_device_buf(y_bytes)
            let yd2 = make_device_buf(y_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_old : array<float>
            var y_cls : array<float>
            y_old |> resize(rows * d)
            y_cls |> resize(rows * d)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsud, 0l, addr<uint8 const?>(wsuh[0]), wsu_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                var sched : array<uint>
                sched |> resize(2 * 4 + wgs)
                sched[0] = 0u
                sched[1] = 0u
                sched[2] = uint(cnt0)
                sched[3] = 0u
                sched[4] = uint(d * nsb)
                sched[5] = uint(cnt0)
                sched[6] = uint(cnt1)
                sched[7] = uint(wgs0)
                for (w in range(wgs)) {
                    sched[8 + w] = w < wgs0 ? 0u : 1u
                }
                var mp = reinterpret<uint?>(meta.mapped)
                mp[0] = uint(n)
                mp[1] = uint(d)
                mp[3] = uint(4 + 2 * 4)
                for (i in range(2 * 4 + wgs)) {
                    mp[4 + i] = sched[i]
                }
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                var so = vk_set6(wqd, wsud, meta.buf, xqd, xsd, yd1,
                    wq_bytes, wsu_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                hz_dispatch(raw, h, weak_copy(g_gpu.batch_coopmat_q40), so, uint(wgs))
                vhz_dep(raw, h, 32u, 0u, true)
                cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                var sc = set_kq_batch_q40_cmf16_cls(fixed_array(wqd, wsud, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, wsu_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_kq_batch_q40_cmf16_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                delete sched
            }
            var bad = 0
            for (i in range(rows * d)) {
                if (y_old[i] != y_cls[i]) {
                    bad++
                }
            }
            to_log(LOG_INFO, "q40 cm batch: {bad} of {rows * d} diverge\n")
            t |> success(bad == 0, "q40 cm batch: class twin bit-exact ({bad} of {rows * d} diverge)")
            delete wqh
            delete wsuh
            delete xqh
            delete xsh
            delete y_old
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cm2_batch(t0 : T?) {
    t0 |> run("cm2 decode-in-load batch class twin == the shipped kernel (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.batch_cm2)))) {
                feint("cm2 batch pipe not built (DASLLAMA_COOPMAT=cm2 only) — skipping\n")
                return
            }
            verify(ensure_q8n_batch_cm2_cls(), "class rail must engage when the shipped pipe exists")
            let n = 64                 // 2 q8n blocks per row reduction
            let nbb = n / 32
            let d = 96                 // one full 64-tile + a 32 edge
            let cnt0 = 140             // 128-token tiles with a 12-row edge
            let cnt1 = 116
            let rows = cnt0 + cnt1
            let wtiles = (d + 63) / 64
            let wgs0 = ((cnt0 + 127) / 128) * wtiles
            let wgs1 = ((cnt1 + 127) / 128) * wtiles
            let wgs = wgs0 + wgs1
            let totblk = 2 * d * nbb
            let wq_bytes = int64(totblk) * 34l
            let wq_words = int((wq_bytes + 3l) / 4l)
            var wqh : array<uint>
            var xfh : array<uint>
            wqh |> resize(wq_words)
            xfh |> resize(rows * n / 2)
            for (i in range(wq_words)) {
                wqh[i] = ws_word(uint(i) + 991u)   // d is read as f16 — every u16 a finite half
            }
            for (i in range(rows * n / 2)) {
                xfh[i] = ws_word(uint(i) * 3u + 5u)   // f16 activation pairs
            }
            let xf_bytes = int64(rows * n) * 2l
            let y_bytes = int64(rows * d) * 4l
            let wqd = make_device_buf(wq_bytes)
            let xfd = make_device_buf(xf_bytes)
            let yd1 = make_device_buf(y_bytes)
            let yd2 = make_device_buf(y_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_old : array<float>
            var y_cls : array<float>
            y_old |> resize(rows * d)
            y_cls |> resize(rows * d)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(xfd, 0l, addr<uint8 const?>(xfh[0]), xf_bytes)
                var sched : array<uint>
                sched |> resize(2 * 4 + wgs)
                sched[0] = 0u
                sched[1] = 0u
                sched[2] = uint(cnt0)
                sched[3] = 0u
                sched[4] = uint(d * nbb)
                sched[5] = uint(cnt0)
                sched[6] = uint(cnt1)
                sched[7] = uint(wgs0)
                for (w in range(wgs)) {
                    sched[8 + w] = w < wgs0 ? 0u : 1u
                }
                var mp = reinterpret<uint?>(meta.mapped)
                mp[0] = uint(n)
                mp[1] = uint(d)
                mp[3] = uint(4 + 2 * 4)
                for (i in range(2 * 4 + wgs)) {
                    mp[4 + i] = sched[i]
                }
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                // the SHIPPED kernel: blocks@0, meta@2, f16 acts@3, y@5; slots 1/4 = meta filler
                var so = vk_set6(wqd, meta.buf, meta.buf, xfd, meta.buf, yd1,
                    wq_bytes, META_BYTES, META_BYTES, xf_bytes, META_BYTES, y_bytes,
                    fixed_array(1u, 0u, 0u, 4u, 0u, 16u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                hz_dispatch(raw, h, weak_copy(g_gpu.batch_cm2), so, uint(wgs))
                vhz_dep(raw, h, 16u, 0u, true)
                cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                // the class twin: sparse bindings 0/2/3/5
                var sc = set_q8n_batch_cm2_cls(fixed_array(wqd, scd, xfd, yd2),
                    fixed_array(wq_bytes, sc_bytes, xf_bytes, y_bytes),
                    fixed_array(1u, 0u, 4u, 16u))
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8n_batch_cm2_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                delete sched
            }
            var bad = 0
            for (i in range(rows * d)) {
                if (y_old[i] != y_cls[i]) {
                    bad++
                }
            }
            to_log(LOG_INFO, "cm2 batch: {bad} of {rows * d} diverge\n")
            t |> success(bad == 0, "cm2 batch: class twin bit-exact ({bad} of {rows * d} diverge)")
            delete wqh
            delete xfh
            delete y_old
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

def private cm_pipe_for_test(v : int) : Pipeline {
    if (v == 0) {
        return weak_copy(g_gpu.batch_coopmat_f16)
    } elif (v == 1) {
        return weak_copy(g_gpu.batch_coopmat_i8)
    } elif (v == 2) {
        return weak_copy(g_gpu.batch_coopmat_mm)
    } elif (v == 3) {
        return weak_copy(g_gpu.batch_coopmat_mm_a)
    }
    return weak_copy(g_gpu.batch_coopmat_mm_m)
}

def private gemv_pipe_batch_for_test(fmt : int) : Pipeline {
    if (fmt == int(KqFmt.k4)) {
        return weak_copy(g_gpu.batch_k4)
    } elif (fmt == int(KqFmt.k5)) {
        return weak_copy(g_gpu.batch_k5)
    } elif (fmt == int(KqFmt.k6)) {
        return weak_copy(g_gpu.batch_k6)
    }
    return weak_copy(g_gpu.batch_q40)
}

[test]
def test_vkd_da_attn(t0 : T?) {
    t0 |> run("decode attention class twin == the shipped kernel (bit-exact, chunk boundary)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_da_attn_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let hs = 64
            let kv_mul = 2
            let nheads = 4
            let kvd = (nheads / kv_mul) * hs
            let cnt = 300   // crosses the 256-position chunk boundary: the alpha-rescale path runs
            let scale = 0.125
            var qh : array<float>
            var kh : array<float>
            var vh : array<float>
            var sh : array<float>
            qh |> resize(nheads * hs)
            kh |> resize(cnt * kvd)
            vh |> resize(cnt * kvd)
            sh |> resize(1)
            for (i in range(nheads * hs)) {
                qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            }
            for (i in range(cnt * kvd)) {
                kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                vh[i] = float((i * 53) % 89) * 0.04 - 1.7
            }
            sh[0] = scale
            let q_bytes = int64(nheads * hs) * 4l
            let m_bytes = int64(cnt * kvd) * 4l
            let qd = make_device_buf(q_bytes)
            let kd = make_device_buf(m_bytes)
            let vd = make_device_buf(m_bytes)
            let sd = make_device_buf(4l)
            let yd1 = make_device_buf(q_bytes)
            let yd2 = make_device_buf(q_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_old : array<float>
            var y_cls : array<float>
            y_old |> resize(nheads * hs)
            y_cls |> resize(nheads * hs)
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kd, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                upload_region_at(vd, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                upload_region_at(sd, 0l, addr<uint8 const?>(sh[0]), 4l)
                var mp = reinterpret<uint?>(meta.mapped)
                mp[0] = uint(cnt)
                mp[2] = uint(kvd)
                mp[3] = uint(hs)
                mp[4] = uint(kv_mul)
                mp[5] = 0u
                mp[6] = 0u
                // the SHIPPED kernel: q@0, K@1, meta@2, V@3, scale@4, y@5
                var so = vk_set6(qd, kd, meta.buf, vd, sd, yd1,
                    q_bytes, m_bytes, META_BYTES, m_bytes, 4l, q_bytes,
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                hz_dispatch(raw, h, weak_copy(g_gpu.da_pipeline), so, uint(nheads))
                vhz_dep(raw, h, 16u, 0u, true)
                cmd_copy_whole(raw, yd1, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_old[0]), host.mapped, q_bytes)
                // the class twin: q@0, K@1, V@2, y@3
                var sc = set_da_attn_cls(fixed_array(qd, kd, vd, yd2),
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes),
                    fixed_array(1u, 2u, 4u, 16u))
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = DaAttnArgs(cnt = uint(cnt), kvd = uint(kvd), hs = uint(hs),
                    kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale)
                enc_da_attn_cls(raw2, h2, sc, pc, int64(nheads))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            var bad = 0
            for (i in range(nheads * hs)) {
                if (y_old[i] != y_cls[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "da_attn: class twin bit-exact ({bad} of {nheads * hs} diverge)")
            delete qh
            delete kh
            delete vh
            delete sh
            delete y_old
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_at_attn(t0 : T?) {
    t0 |> run("prefill attention class twin == the shipped kernel (bit-exact, partial tiles, gated)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_at_attn_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nheads = 4
            let kv_mul = 2
            let rows = 13   // partial 8-tile: q0=8 has 5 valid rows — the row masks run
            let w0 = 32     // nonzero window base: kend crosses the 32-position K tile → alpha rescale runs
            let scale = 0.125
            for (hs in [64, 256]) {   // dpl 2 (o0/o1 only) and dpl 8 (the o4..o7 arm); gate the wide one
                let gated = hs == 256
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + rows   // K/V panels hold absolute positions [0, w0+rows)
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                var gh : array<float>
                qh |> resize(rows * qd)
                kh |> resize(kpos * kvd)
                vh |> resize(kpos * kvd)
                gh |> resize(rows * qd)
                for (i in range(rows * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                    gh[i] = float((i * 29) % 61) * 0.06 - 1.9
                }
                for (i in range(kpos * kvd)) {
                    kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                    vh[i] = float((i * 53) % 89) * 0.04 - 1.7
                }
                let q_bytes = int64(rows * qd) * 4l
                let m_bytes = int64(kpos * kvd) * 4l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(m_bytes)
                let vdev = make_device_buf(m_bytes)
                let gdev = make_device_buf(q_bytes)   // the shipped kernel's gate stash / out plane
                let sdev = make_device_buf(16l)
                let yd2 = make_device_buf(q_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(rows * qd)
                y_cls |> resize(rows * qd)
                let wgs = nheads * ((rows + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                    upload_region_at(gdev, 0l, addr<uint8 const?>(gh[0]), q_bytes)
                    var sm : float[4]
                    sm[1] = scale   // the shipped kernel reads scale at smalls word 1
                    upload_region_at(sdev, 0l, addr<uint8 const?>(sm[0]), 16l)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(rows)
                    mp[1] = uint(w0)
                    mp[2] = uint(qd)
                    mp[3] = uint(kvd)
                    mp[4] = uint(hs)
                    mp[8] = uint(kv_mul)
                    mp[9] = gated ? 1u : 0u
                    mp[11] = uint(hs / 32)
                    // the SHIPPED kernel: q@0, K@1, meta@2, gate/out@3, smalls@4, V@5
                    var so = vk_set6(qdev, kdev, meta.buf, gdev, sdev, vdev,
                        q_bytes, m_bytes, META_BYTES, q_bytes, 16l, m_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.at_attn_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 4u, 0u, true)
                    cmd_copy_whole(raw, gdev, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, q_bytes)
                    // the class twin: q@0, K@1, V@2, out@3 (pre-loaded with the same gates)
                    upload_region_at(yd2, 0l, addr<uint8 const?>(gh[0]), q_bytes)
                    var sc = set_at_attn_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 4u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul),
                        flags = gated ? 1u : 0u, dpl = uint(hs / 32), scale = scale)
                    enc_at_attn_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                var bad = 0
                for (i in range(rows * qd)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "at_attn hs {hs}: class twin bit-exact ({bad} of {rows * qd} diverge)")
                delete qh
                delete kh
                delete vh
                delete gh
                delete y_old
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_da_attn_b(t0 : T?) {
    t0 |> run("batched decode attention class twin == the shipped kernel (bit-exact, mirror offsets)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_da_attn_b_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nheads = 4
            let kv_mul = 2
            let npos = 13   // partial 8-tile: q0=8 has 5 valid rows — the row masks run
            let w0 = 32     // nonzero window base: kend crosses the 32-position K tile → alpha rescale runs
            let kbase = 64  // word offsets into the mirrors — the offset plumbing must survive the port
            let vbase = 160
            let scale = 0.125
            for (hs in [32, 128, 256]) {   // dpl 1 / 4 / 8 — every guarded accumulator arm
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + npos   // mirrors hold absolute positions [0, w0+npos)
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(npos * qd)
                kh |> resize(kbase + kpos * kvd)
                vh |> resize(vbase + kpos * kvd)
                for (i in range(npos * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                }
                for (i in range(kbase + kpos * kvd)) {
                    kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                }
                for (i in range(vbase + kpos * kvd)) {
                    vh[i] = float((i * 53) % 89) * 0.04 - 1.7
                }
                let q_bytes = int64(npos * qd) * 4l
                let k_bytes = int64(kbase + kpos * kvd) * 4l
                let v_bytes = int64(vbase + kpos * kvd) * 4l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(k_bytes)
                let vdev = make_device_buf(v_bytes)
                let sdev = make_device_buf(16l)
                let yd1 = make_device_buf(q_bytes)
                let yd2 = make_device_buf(q_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(npos * qd)
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                    var sm : float[4]
                    sm[0] = scale   // the shipped kernel reads scale at smalls word 0
                    upload_region_at(sdev, 0l, addr<uint8 const?>(sm[0]), 16l)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(npos)
                    mp[1] = uint(nheads)
                    mp[2] = uint(kvd)
                    mp[3] = uint(hs)
                    mp[4] = uint(kv_mul)
                    mp[5] = uint(kbase)
                    mp[6] = uint(vbase)
                    mp[7] = uint(w0)
                    // the SHIPPED kernel: q@0, K mirror@1, meta@2, V mirror@3, smalls@4, y@5
                    var so = vk_set6(qdev, kdev, meta.buf, vdev, sdev, yd1,
                        q_bytes, k_bytes, META_BYTES, v_bytes, 16l, q_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.da_b_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 16u, 0u, true)
                    cmd_copy_whole(raw, yd1, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, q_bytes)
                    // the class twin: q@0, K@1, V@2, out@3
                    var sc = set_da_attn_b_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale)
                    enc_da_attn_b_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                var bad = 0
                for (i in range(npos * qd)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "da_attn_b hs {hs}: class twin bit-exact ({bad} of {npos * qd} diverge)")
                delete qh
                delete kh
                delete vh
                delete y_old
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_da_attn_b_h128(t0 : T?) {
    t0 |> run("h128 coopmat attention class twin == the shipped kernel (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_da_attn_b_cls()) {   // probe via the NON-coopmat twin — a cm pipeline build
                feint("no Vulkan device — skipping\n")   // on a no-coopmat device panics, not feints
                return
            }
            if (!g_gpu.has_coopmat) {
                feint("no coopmat on this device — skipping\n")
                return
            }
            verify(ensure_da_attn_b_h128_cls(), "the h128 class rail must engage on a coopmat device")
            let hs = 128
            let nheads = 4
            let kv_mul = 2
            let npos = 45   // qtiles=2: the second tile has 13 valid rows — the row masks run
            let w0 = 32
            let kbase = 64
            let vbase = 160
            let scale = 0.125
            let qd = nheads * hs
            let kvd = (nheads / kv_mul) * hs
            let kpos = w0 + npos
            var qh : array<float>
            var kh : array<float>
            var vh : array<float>
            qh |> resize(npos * qd)
            kh |> resize(kbase + kpos * kvd)
            vh |> resize(vbase + kpos * kvd)
            for (i in range(npos * qd)) {
                qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            }
            for (i in range(kbase + kpos * kvd)) {
                kh[i] = float((i * 17) % 97) * 0.03 - 1.4
            }
            for (i in range(vbase + kpos * kvd)) {
                vh[i] = float((i * 53) % 89) * 0.04 - 1.7
            }
            let q_bytes = int64(npos * qd) * 4l
            let k_bytes = int64(kbase + kpos * kvd) * 4l
            let v_bytes = int64(vbase + kpos * kvd) * 4l
            let qdev = make_device_buf(q_bytes)
            let kdev = make_device_buf(k_bytes)
            let vdev = make_device_buf(v_bytes)
            let sdev = make_device_buf(16l)
            let yd1 = make_device_buf(q_bytes)
            let yd2 = make_device_buf(q_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_old : array<float>
            var y_cls : array<float>
            y_old |> resize(npos * qd)
            y_cls |> resize(npos * qd)
            let wgs = nheads * ((npos + 31) / 32)
            unsafe {
                upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                var sm : float[4]
                sm[0] = scale
                upload_region_at(sdev, 0l, addr<uint8 const?>(sm[0]), 16l)
                var mp = reinterpret<uint?>(meta.mapped)
                mp[0] = uint(npos)
                mp[1] = uint(nheads)
                mp[2] = uint(kvd)
                mp[4] = uint(kv_mul)
                mp[5] = uint(kbase)
                mp[6] = uint(vbase)
                mp[7] = uint(w0)
                // the SHIPPED kernel: q@0, K mirror@1, meta@2, V mirror@3, smalls@4, y@5
                var so = vk_set6(qdev, kdev, meta.buf, vdev, sdev, yd1,
                    q_bytes, k_bytes, META_BYTES, v_bytes, 16l, q_bytes,
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                hz_dispatch(raw, h, weak_copy(g_gpu.da_b128_pipeline), so, uint(wgs))
                vhz_dep(raw, h, 16u, 0u, true)
                cmd_copy_whole(raw, yd1, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_old[0]), host.mapped, q_bytes)
                // the class twin: q@0, K@1, V@2, out@3
                var sc = set_da_attn_b_h128_cls(fixed_array(qdev, kdev, vdev, yd2),
                    fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                    fixed_array(1u, 2u, 8u, 16u))
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                    hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                    vbase = uint(vbase), w0 = uint(w0), scale = scale)
                enc_da_attn_b_h128_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            var bad = 0
            for (i in range(npos * qd)) {
                if (y_old[i] != y_cls[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "da_attn_b_h128: class twin bit-exact ({bad} of {npos * qd} diverge)")
            delete qh
            delete kh
            delete vh
            delete y_old
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_requant_family(t0 : T?) {
    t0 |> run("fused ar+requant + q8k requant class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_cls_ar_rq() && ensure_cls_q8k_rq())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            let begin = VkCommandBufferBeginInfo()
            // ===== the fused ar + Q8_0 requant =====
            {
                let dim = 1024
                let eps = 1e-5
                let ascale = 1.25
                let nblk = dim / 32
                var x : array<float>
                var a : array<float>
                var w : array<float>
                var wpk : array<float>   // the OLD kernel's packed w buffer: weights + [eps, ascale]
                x |> resize(dim)
                a |> resize(dim)
                w |> resize(dim)
                wpk |> resize(dim + 2)
                for (i in range(dim)) {
                    x[i] = float((i * 37) % 101) * 0.09 - 4.2
                    a[i] = float((i * 13) % 89) * 0.06 - 2.6
                    w[i] = 0.5 + float(i % 17) * 0.04
                    wpk[i] = w[i]
                }
                wpk[dim] = eps
                wpk[dim + 1] = ascale
                let e_bytes = int64(dim) * 4l
                let q_bytes = int64(dim)
                let s_bytes = int64(nblk) * 4l
                let x1 = make_device_buf(e_bytes)
                let x2 = make_device_buf(e_bytes)
                let ad = make_device_buf(e_bytes)
                let wd = make_device_buf(e_bytes)
                let wpkd = make_device_buf(e_bytes + 8l)
                let q1 = make_device_buf(q_bytes)
                let q2 = make_device_buf(q_bytes)
                let s1 = make_device_buf(s_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(e_bytes, true, [cached = true])
                var q_old : array<uint>
                var q_cls : array<uint>
                var s_old : array<float>
                var s_cls : array<float>
                var x_old : array<float>
                var x_cls : array<float>
                q_old |> resize(dim / 4)
                q_cls |> resize(dim / 4)
                s_old |> resize(nblk)
                s_cls |> resize(nblk)
                x_old |> resize(dim)
                x_cls |> resize(dim)
                unsafe {
                    upload_region_at(x1, 0l, addr<uint8 const?>(x[0]), e_bytes)
                    upload_region_at(x2, 0l, addr<uint8 const?>(x[0]), e_bytes)
                    upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), e_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), e_bytes)
                    upload_region_at(wpkd, 0l, addr<uint8 const?>(wpk[0]), e_bytes + 8l)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(dim)
                    mp[1] = 1u
                    mp[2] = 0u
                    mp[3] = uint(dim)
                    // the SHIPPED kernel: x@0, a@1, meta@2, w+eps/ascale@3, outs@4, outq@5
                    var so = vk_set6(x1, ad, meta.buf, wpkd, s1, q1,
                        e_bytes, e_bytes, META_BYTES, e_bytes + 8l, s_bytes, q_bytes,
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.ar_rq_pipeline), so, 1u)
                    vhz_dep(raw, h, 1u | 8u | 16u, 0u, true)
                    cmd_copy_whole(raw, q1, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_old[0]), host.mapped, q_bytes)
                    var rawb = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawb, begin), null)
                    cmd_copy_whole(rawb, s1, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(rawb), null)
                    submit_wait(rawb)
                    memcpy(addr<void?>(s_old[0]), host.mapped, s_bytes)
                    var rawc = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawc, begin), null)
                    cmd_copy_whole(rawc, x1, host.buf, e_bytes)
                    vk_check(vkEndCommandBuffer(rawc), null)
                    submit_wait(rawc)
                    memcpy(addr<void?>(x_old[0]), host.mapped, e_bytes)
                    // the class twin: x@0, a@1, w@2, outq@3, outs@4
                    var sc = set_cls_ar_rq(fixed_array(x2, ad, wd, q2, s2),
                        fixed_array(e_bytes, e_bytes, e_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 2u, 4u, 16u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                    enc_cls_ar_rq(raw2, h2, sc, pc, 1l)
                    vhz_dep(raw2, h2, 1u | 8u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, s2, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(s_cls[0]), host.mapped, s_bytes)
                    var raw2c = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2c, begin), null)
                    cmd_copy_whole(raw2c, x2, host.buf, e_bytes)
                    vk_check(vkEndCommandBuffer(raw2c), null)
                    submit_wait(raw2c)
                    memcpy(addr<void?>(x_cls[0]), host.mapped, e_bytes)
                }
                var bad = 0
                for (i in range(dim / 4)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                for (i in range(nblk)) {
                    if (s_old[i] != s_cls[i]) {
                        bad++
                    }
                }
                for (i in range(dim)) {
                    if (x_old[i] != x_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "ar+requant fused: class twin bit-exact ({bad} elements diverge)")
                delete x
                delete a
                delete w
                delete wpk
                delete q_old
                delete q_cls
                delete s_old
                delete s_cls
                delete x_old
                delete x_cls
            }
            // ===== the Q8_K requant =====
            {
                let nsb = 4
                let nelem = nsb * 256
                var src : array<float>
                src |> resize(nelem)
                for (i in range(nelem)) {
                    src[i] = float((i * 61) % 113) * 0.05 - 2.8
                }
                let e_bytes = int64(nelem) * 4l
                let q_bytes = int64(nelem)
                let s_bytes = int64(nsb) * 4l
                let sd = make_device_buf(e_bytes)
                let q1 = make_device_buf(q_bytes)
                let q2 = make_device_buf(q_bytes)
                let s1 = make_device_buf(s_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var q_old : array<uint>
                var q_cls : array<uint>
                var s_old : array<float>
                var s_cls : array<float>
                q_old |> resize(nelem / 4)
                q_cls |> resize(nelem / 4)
                s_old |> resize(nsb)
                s_cls |> resize(nsb)
                let wgs = (nsb * 32 + 255) / 256
                unsafe {
                    upload_region_at(sd, 0l, addr<uint8 const?>(src[0]), e_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[10] = 0u
                    mp[18] = uint(nsb)
                    // the SHIPPED kernel: src@0, meta@2, outq@3, outs@4
                    var so = vk_set6(sd, sd, meta.buf, q1, s1, sd,
                        e_bytes, e_bytes, META_BYTES, q_bytes, s_bytes, e_bytes,
                        fixed_array(1u, 0u, 0u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.q8k_rq_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw, q1, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_old[0]), host.mapped, q_bytes)
                    var rawb = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(rawb, begin), null)
                    cmd_copy_whole(rawb, s1, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(rawb), null)
                    submit_wait(rawb)
                    memcpy(addr<void?>(s_old[0]), host.mapped, s_bytes)
                    // the class twin: src@0, outq@1, outs@2
                    var sc = set_rq_cls(fixed_array(sd, q2, s2),
                        fixed_array(e_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RqArgs(inbase = 0u, nblk = uint(nsb))
                    enc_cls_q8k_rq(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, s2, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(s_cls[0]), host.mapped, s_bytes)
                }
                var bad = 0
                for (i in range(nelem / 4)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                for (i in range(nsb)) {
                    if (s_old[i] != s_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "q8k requant: class twin bit-exact ({bad} elements diverge)")
                delete src
                delete q_old
                delete q_cls
                delete s_old
                delete s_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_q8_gemv_pair(t0 : T?) {
    t0 |> run("q8/q8n GEMV class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_q8_gemv_cls() && ensure_q8n_gemv_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nb = 16      // 32-blocks per row reduction (n = 512)
            let d = 3
            let nreg = 2
            let rows = nreg * d
            let totblk = rows * nb
            for (arm in range(2)) {   // 0 = q8, 1 = q8n
                let q8n = arm == 1
                if (q8n && !key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.gemv_q8n)))) {
                    feint("q8n reference pipe not built (cm2 mode only) — skipping the q8n arm\n")
                    continue
                }
                let wq_bytes = q8n ? int64(totblk) * 34l : int64(totblk) * 32l
                let ws_bytes = int64(totblk) * 2l
                let wq_words = int((wq_bytes + 3l) / 4l)
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(wq_words)
                wsh |> resize(totblk / 2 + 1)
                xqh |> resize(nb * 8)
                xsh |> resize(nb)
                for (i in range(wq_words)) {
                    // q8n reads u16[0] of each 17-u16 block as an f16 d — keep EVERY u16 a finite
                    // half on that arm; plain q8 quant words are free bits
                    wqh[i] = q8n ? ws_word(uint(i) + 991u) : hash_word(uint(i) + 55u)
                }
                for (i in range(totblk / 2 + 1)) {
                    wsh[i] = ws_word(uint(i))
                }
                for (i in range(nb * 8)) {
                    xqh[i] = hash_word(uint(i) * 17u + 9u)
                }
                for (i in range(nb)) {
                    xsh[i] = 0.25 + float(i % 5) * 0.125
                }
                let xq_bytes = int64(nb) * 32l
                let xs_bytes = int64(nb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd1 = make_device_buf(y_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_old : array<float>
                var y_cls : array<float>
                y_old |> resize(rows)
                y_cls |> resize(rows)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    var regs : array<uint>
                    regs |> resize(nreg * 2)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = 512u
                    mp[1] = uint(d)
                    mp[2] = uint(nreg)
                    mp[3] = 0u
                    for (r in range(nreg)) {
                        mp[4 + r * 2] = uint(r * d * nb)
                        mp[5 + r * 2] = 0u
                        regs[r * 2] = uint(r * d * nb)
                        regs[r * 2 + 1] = 0u
                    }
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    var s_old = vk_set6(wqd, wsd, meta.buf, xqd, xsd, yd1,
                        wq_bytes, ws_bytes, META_BYTES, xq_bytes, xs_bytes, y_bytes,
                        fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, gemv_pipe(q8n ? int(KqFmt.q8n) : int(KqFmt.q8)), s_old, uint(wgs))
                    vhz_dep(raw, h, 32u, 0u, true)
                    cmd_copy_whole(raw, yd1, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_old[0]), host.mapped, y_bytes)
                    var s_cls : VkDescriptorSet
                    if (q8n) {
                        s_cls = set_q8n_gemv_cls(fixed_array(wqd, rgd, xqd, xsd, yd2),
                            fixed_array(wq_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes),
                            fixed_array(1u, 4u, 8u, 16u, 32u))
                    } else {
                        s_cls = set_q8_gemv_cls(fixed_array(wqd, wsd, rgd, xqd, xsd, yd2),
                            fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes),
                            fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    }
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = 512u, d = uint(d), nreg = uint(nreg), ybase = 0u)
                    if (q8n) {
                        enc_q8n_gemv_cls(raw2, h2, s_cls, pc, wgs)
                    } else {
                        enc_q8_gemv_cls(raw2, h2, s_cls, pc, wgs)
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                    delete regs
                }
                var bad = 0
                for (i in range(rows)) {
                    if (y_old[i] != y_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "{q8n ? "q8n" : "q8"}: class twin bit-exact vs shipped ({bad} of {rows} rows diverge)")
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_old
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cvt_family(t0 : T?) {
    t0 |> run("dn requant + cvt/act-f16 class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_dn_rq()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            // dn_requant: src floats at an offset -> Q8_0 words + scales (device-gated on dn_shared_ok)
            if (key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.dn_rq_pipeline)))) {
                let nblk = 24
                let inbase = 64
                var srch : array<float>
                srch |> resize(inbase + nblk * 32)
                for (i in range(inbase + nblk * 32)) {
                    srch[i] = float((i * 37) % 113) * 0.04 - 2.2
                }
                let src_bytes = int64(inbase + nblk * 32) * 4l
                let q_bytes = int64(nblk) * 32l
                let s_bytes = int64(nblk) * 4l
                let srcd = make_device_buf(src_bytes)
                let qd1 = make_device_buf(q_bytes)
                let sd1 = make_device_buf(s_bytes)
                let qd2 = make_device_buf(q_bytes)
                let sd2 = make_device_buf(s_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(q_bytes + s_bytes, true, [cached = true])
                var q_old : array<uint>
                var q_cls : array<uint>
                var s_old : array<float>
                var s_cls : array<float>
                q_old |> resize(nblk * 8)
                q_cls |> resize(nblk * 8)
                s_old |> resize(nblk)
                s_cls |> resize(nblk)
                let wgs = (nblk * 8 + 255) / 256
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[10] = uint(inbase)
                    mp[18] = uint(nblk)
                    // the SHIPPED kernel: src floats@0, meta@2, quant words@3, scales@4
                    var so = vk_set6(srcd, meta.buf, meta.buf, qd1, sd1, meta.buf,
                        src_bytes, META_BYTES, META_BYTES, q_bytes, s_bytes, META_BYTES,
                        fixed_array(1u, 0u, 0u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.dn_rq_pipeline), so, uint(wgs))
                    vhz_dep(raw, h, 4u, 0u, true)
                    cmd_copy_whole(raw, qd1, host.buf, q_bytes)
                    cmd_copy_range(raw, sd1, 0l, host.buf, q_bytes, s_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_old[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(s_old[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), s_bytes)
                    var sc = set_rq_cls(fixed_array(srcd, qd2, sd2),
                        fixed_array(src_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RqArgs(inbase = uint(inbase), nblk = uint(nblk))
                    enc_cls_dn_rq(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u, 0u, true)
                    cmd_copy_whole(raw2, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw2, sd2, 0l, host.buf, q_bytes, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(s_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), s_bytes)
                }
                var bad = 0
                for (i in range(nblk * 8)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                for (i in range(nblk)) {
                    if (s_old[i] != s_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "dn_requant: class twin bit-exact ({bad} diverge)")
                delete srch
                delete q_old
                delete q_cls
                delete s_old
                delete s_cls
            } else {
                feint("dn chain declined on this device — skipping dn_requant arm\n")
            }
            // the cm2-chain converters (mode 4 pipes): xf16 dequant, act-f16, f16 convert
            if (key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.xf16_pipeline)))) {
                verify(ensure_cls_xf16() && ensure_actf16_cls() && ensure_f16cvt_cls(),
                    "class rails must engage when the shipped pipes exist")
                let nwords = 512
                let nelem = 2048
                var xqh : array<uint>
                var xsh : array<float>
                var gh : array<float>
                var uh : array<float>
                xqh |> resize(nwords)
                xsh |> resize(nwords / 8)
                gh |> resize(nelem)
                uh |> resize(nelem)
                for (i in range(nwords)) {
                    xqh[i] = hash_word(uint(i) * 7u + 11u)
                }
                for (i in range(nwords / 8)) {
                    xsh[i] = 0.25 + float(i % 7) * 0.0625
                }
                for (i in range(nelem)) {
                    gh[i] = float((i * 29) % 61) * 0.06 - 1.9
                    uh[i] = float((i * 43) % 83) * 0.05 - 2.1
                }
                let xq_bytes = int64(nwords) * 4l
                let xs_bytes = int64(nwords / 8) * 4l
                let g_bytes = int64(nelem) * 4l
                let h_bytes = int64(nelem) * 2l
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let gd = make_device_buf(g_bytes)
                let ud = make_device_buf(g_bytes)
                let hd1 = make_device_buf(h_bytes)
                let hd2 = make_device_buf(h_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(h_bytes, true, [cached = true])
                var h_old : array<uint>
                var h_cls : array<uint>
                h_old |> resize(nelem / 2)
                h_cls |> resize(nelem / 2)
                for (arm in range(3)) {   // 0 = xf16 dequant, 1 = act-f16 (gelu), 2 = f16 convert
                    unsafe {
                        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                        upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), g_bytes)
                        upload_region_at(ud, 0l, addr<uint8 const?>(uh[0]), g_bytes)
                        var mp = reinterpret<uint?>(meta.mapped)
                        mp[0] = uint(nelem)
                        mp[1] = 1u   // gelu arm of act_mul
                        mp[3] = uint(nwords)
                        var so : VkDescriptorSet
                        if (arm == 0) {
                            // xf16: out f16@0, meta@2, quant words@3, scales@4
                            so = vk_set6(hd1, meta.buf, meta.buf, xqd, xsd, meta.buf,
                                h_bytes, META_BYTES, META_BYTES, xq_bytes, xs_bytes, META_BYTES,
                                fixed_array(1u, 0u, 0u, 4u, 8u, 0u))
                        } elif (arm == 1) {
                            // actf16: up@0, meta@2, out f16@3, gate@5
                            so = vk_set6(ud, meta.buf, meta.buf, hd1, meta.buf, gd,
                                g_bytes, META_BYTES, META_BYTES, h_bytes, META_BYTES, g_bytes,
                                fixed_array(1u, 0u, 0u, 4u, 0u, 8u))
                        } else {
                            // f16cvt: src@0, meta@2, out f16@3
                            so = vk_set6(ud, meta.buf, meta.buf, hd1, meta.buf, meta.buf,
                                g_bytes, META_BYTES, META_BYTES, h_bytes, META_BYTES, META_BYTES,
                                fixed_array(1u, 0u, 0u, 4u, 0u, 0u))
                        }
                        let wgs = arm == 0 ? (nwords + 255) / 256 : (nelem / 4 + 255) / 256
                        var raw = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw, begin), null)
                        var h : VkHaz
                        hz_dispatch(raw, h,
                            arm == 0 ? weak_copy(g_gpu.xf16_pipeline)
                                : (arm == 1 ? weak_copy(g_gpu.actf16_pipeline) : weak_copy(g_gpu.f16cvt_pipeline)),
                            so, uint(wgs))
                        vhz_dep(raw, h, 4u, 0u, true)
                        cmd_copy_whole(raw, hd1, host.buf, h_bytes)
                        vk_check(vkEndCommandBuffer(raw), null)
                        submit_wait(raw)
                        memcpy(addr<void?>(h_old[0]), host.mapped, h_bytes)
                        var sc : VkDescriptorSet
                        var raw2 = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(raw2, begin), null)
                        var h2 : VkHaz
                        if (arm == 0) {
                            sc = set_cls_xf16(fixed_array(xqd, xsd, hd2),
                                fixed_array(xq_bytes, xs_bytes, h_bytes), fixed_array(1u, 2u, 4u))
                            var pc = RqArgs(inbase = 0u, nblk = uint(nwords))
                            enc_cls_xf16(raw2, h2, sc, pc, int64(wgs))
                        } elif (arm == 1) {
                            sc = set_actf16_cls(fixed_array(gd, ud, hd2),
                                fixed_array(g_bytes, g_bytes, h_bytes), fixed_array(1u, 2u, 4u))
                            var pc = ActArgs(nelem = uint(nelem), gelu = 1u, nblk = 0u)
                            enc_actf16_cls(raw2, h2, sc, pc, int64(wgs))
                        } else {
                            sc = set_f16cvt_cls(fixed_array(ud, hd2),
                                fixed_array(g_bytes, h_bytes), fixed_array(1u, 4u))
                            var pc = ActArgs(nelem = uint(nelem), gelu = 0u, nblk = 0u)
                            enc_f16cvt_cls(raw2, h2, sc, pc, int64(wgs))
                        }
                        vhz_dep(raw2, h2, 4u, 0u, true)
                        cmd_copy_whole(raw2, hd2, host.buf, h_bytes)
                        vk_check(vkEndCommandBuffer(raw2), null)
                        submit_wait(raw2)
                        memcpy(addr<void?>(h_cls[0]), host.mapped, h_bytes)
                    }
                    var bad = 0
                    for (i in range(nelem / 2)) {
                        if (h_old[i] != h_cls[i]) {
                            bad++
                        }
                    }
                    t |> success(bad == 0, "cvt arm {arm}: class twin bit-exact ({bad} of {nelem / 2} words diverge)")
                }
                delete xqh
                delete xsh
                delete gh
                delete uh
                delete h_old
                delete h_cls
            } else {
                feint("cm2 chain pipes not built (DASLLAMA_COOPMAT=cm2 only) — skipping cvt arms\n")
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_rope_b_pair(t0 : T?) {
    t0 |> run("batch rope + fused qkn-rope class twins == the shipped kernels (bit-exact)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_b_cls() && ensure_qkn_rope_cls())) {
                feint("no Vulkan device — skipping\n")
                return
            }
            let nh = 4
            let nkvh = 2
            let hs = 64
            let half = hs / 2
            let qd = nh * hs
            let kvd = nkvh * hs
            // --- rope_kv_store_b: npos positions, q in place, k roped + v raw into the mirrors ---
            {
                let neox = 1
                let npos = 5
                let pos0 = 7
                let layerbase = 256
                let voff = npos * kvd   // v rows base inside the kv projection buffer
                let ppp = (nh + nkvh) * half
                let kpair0 = nh * half
                let mirn = layerbase + (pos0 + npos) * kvd
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                q0h |> resize(npos * qd)
                kvh |> resize(voff + npos * kvd)
                csh |> resize(npos * hs)
                for (i in range(npos * qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(voff + npos * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(npos * hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                let q_bytes = int64(npos * qd) * 4l
                let kv_bytes = int64(voff + npos * kvd) * 4l
                let cs_bytes = int64(npos * hs) * 4l
                let m_bytes = int64(mirn) * 4l
                let qd1 = make_device_buf(q_bytes)
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let km1 = make_device_buf(m_bytes)
                let vm1 = make_device_buf(m_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_old : array<float>
                var q_cls : array<float>
                var m_old : array<float>
                var m_cls : array<float>
                q_old |> resize(npos * qd)
                q_cls |> resize(npos * qd)
                m_old |> resize(2 * mirn)
                m_cls |> resize(2 * mirn)
                let wgs = (npos * ppp + 255) / 256
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(qd)
                    mp[1] = uint(kvd)
                    mp[2] = uint(hs)
                    mp[3] = uint(half)
                    mp[4] = uint(neox)
                    mp[5] = uint(kpair0)
                    mp[6] = uint(ppp)
                    mp[7] = uint(layerbase)
                    mp[9] = uint(npos)
                    mp[10] = uint(pos0)
                    mp[11] = uint(voff)
                    for (side in range(2)) {   // 0 = shipped, 1 = class (fresh q plane each)
                        let qdev = side == 0 ? qd1 : qd2
                        let kmd = side == 0 ? km1 : km2
                        let vmd = side == 0 ? vm1 : vm2
                        upload_region_at(qdev, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                        var raw = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw, begin), null)
                        var h : VkHaz
                        if (side == 0) {
                            // the SHIPPED kernel: q@0, Kmir@1, meta@2, Vmir@3, cos@4, kv proj@5
                            var so = vk_set6(qdev, kmd, meta.buf, vmd, csd, kvdv,
                                q_bytes, m_bytes, META_BYTES, m_bytes, cs_bytes, kv_bytes,
                                fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                            hz_dispatch(raw, h, weak_copy(g_gpu.rks_b_pipeline), so, uint(wgs))
                        } else {
                            var sc = set_rope_kv_b_cls(fixed_array(qdev, kvdv, kmd, vmd, csd),
                                fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes),
                                fixed_array(1u, 16u, 2u, 4u, 8u))
                            var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                                half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                                ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                                pos0 = uint(pos0), voff = uint(voff))
                            enc_rope_kv_b_cls(raw, h, sc, pc, int64(wgs))
                        }
                        vhz_dep(raw, h, 8u, 0u, true)
                        cmd_copy_whole(raw, qdev, host.buf, q_bytes)
                        cmd_copy_range(raw, kmd, 0l, host.buf, q_bytes, m_bytes)
                        cmd_copy_range(raw, vmd, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                        vk_check(vkEndCommandBuffer(raw), null)
                        submit_wait(raw)
                        if (side == 0) {
                            memcpy(addr<void?>(q_old[0]), host.mapped, q_bytes)
                            memcpy(addr<void?>(m_old[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                        } else {
                            memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                            memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                        }
                    }
                }
                var bad = 0
                for (i in range(npos * qd)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                // mirrors: only the written region [layerbase + pos0*kvd, layerbase + (pos0+npos)*kvd)
                let w0 = layerbase + pos0 * kvd
                for (i in range(npos * kvd)) {
                    if (m_old[w0 + i] != m_cls[w0 + i]) {
                        bad++
                    }
                    if (m_old[mirn + w0 + i] != m_cls[mirn + w0 + i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "rope_kv_store_b: class twin bit-exact ({bad} diverge)")
                delete q0h
                delete kvh
                delete csh
                delete q_old
                delete q_cls
                delete m_old
                delete m_cls
            }
            // --- qkn_rope_kv: qk-rms + rope + kv-store in one dispatch, one wg per head-row ---
            {
                let neox = 1
                let krow = 192
                let vrow = 320
                let eps = 0.00001
                let mirn2 = vrow + nkvh * hs
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                var wnh : array<float>
                q0h |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                wnh |> resize(2 * hs)
                for (i in range(qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                for (i in range(2 * hs)) {
                    wnh[i] = 0.75 + float(i % 11) * 0.05
                }
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let cs_bytes = int64(hs) * 4l
                let wn_bytes = int64(2 * hs) * 4l
                let m_bytes = int64(mirn2) * 4l
                // the shipped kernel's xs plane: cos+sin, then the two weight rows, then eps
                var xsh : array<float>
                xsh |> resize(3 * hs + 1)
                for (i in range(hs)) {
                    xsh[i] = csh[i]
                }
                for (i in range(2 * hs)) {
                    xsh[hs + i] = wnh[i]
                }
                xsh[3 * hs] = eps
                let xs_bytes = int64(3 * hs + 1) * 4l
                let qd1 = make_device_buf(q_bytes)
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let wnd = make_device_buf(wn_bytes)
                let xsd = make_device_buf(xs_bytes)
                let km1 = make_device_buf(m_bytes)
                let vm1 = make_device_buf(m_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_old : array<float>
                var q_cls : array<float>
                var m_old : array<float>
                var m_cls : array<float>
                q_old |> resize(qd)
                q_cls |> resize(qd)
                m_old |> resize(2 * mirn2)
                m_cls |> resize(2 * mirn2)
                let wgs = nh + nkvh
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(qd)
                    mp[1] = uint(kvd)
                    mp[2] = uint(hs)
                    mp[3] = uint(half)
                    mp[4] = uint(neox)
                    mp[5] = uint(nh)
                    mp[7] = uint(krow)
                    mp[8] = uint(vrow)
                    mp[9] = uint(hs)
                    mp[10] = uint(2 * hs)
                    mp[11] = uint(3 * hs)
                    for (side in range(2)) {
                        let qdev = side == 0 ? qd1 : qd2
                        let kmd = side == 0 ? km1 : km2
                        let vmd = side == 0 ? vm1 : vm2
                        upload_region_at(qdev, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                        var raw = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw, begin), null)
                        var h : VkHaz
                        if (side == 0) {
                            // the SHIPPED kernel: kv proj@0, Kmir@1, meta@2, Vmir@3, cos+rms+eps@4, q@5
                            var so = vk_set6(kvdv, kmd, meta.buf, vmd, xsd, qdev,
                                kv_bytes, m_bytes, META_BYTES, m_bytes, xs_bytes, q_bytes,
                                fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                            hz_dispatch(raw, h, weak_copy(g_gpu.qkn_rope_pipeline), so, uint(wgs))
                        } else {
                            var sc = set_qkn_rope_cls(fixed_array(qdev, kvdv, kmd, vmd, csd, wnd),
                                fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, wn_bytes),
                                fixed_array(16u, 1u, 2u, 4u, 8u, 8u))
                            var pc = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                                half = uint(half), neox = uint(neox), nh = uint(nh),
                                krow = uint(krow), vrow = uint(vrow), qwoff = 0u,
                                kwoff = uint(hs), eps = eps)
                            enc_qkn_rope_cls(raw, h, sc, pc, int64(wgs))
                        }
                        vhz_dep(raw, h, 8u, 0u, true)
                        cmd_copy_whole(raw, qdev, host.buf, q_bytes)
                        cmd_copy_range(raw, kmd, 0l, host.buf, q_bytes, m_bytes)
                        cmd_copy_range(raw, vmd, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                        vk_check(vkEndCommandBuffer(raw), null)
                        submit_wait(raw)
                        if (side == 0) {
                            memcpy(addr<void?>(q_old[0]), host.mapped, q_bytes)
                            memcpy(addr<void?>(m_old[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                        } else {
                            memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                            memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                        }
                    }
                }
                var bad = 0
                for (i in range(qd)) {
                    if (q_old[i] != q_cls[i]) {
                        bad++
                    }
                }
                for (i in range(nkvh * hs)) {
                    if (m_old[krow + i] != m_cls[krow + i]) {
                        bad++
                    }
                    if (m_old[mirn2 + vrow + i] != m_cls[mirn2 + vrow + i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "qkn_rope_kv: class twin bit-exact ({bad} diverge)")
                delete q0h
                delete kvh
                delete csh
                delete wnh
                delete xsh
                delete q_old
                delete q_cls
                delete m_old
                delete m_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_at_prep(t0 : T?) {
    t0 |> run("attention prep class (q+k collapse) == the shipped pair (bit-exact, gated+qknorm+partial rope)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_at_prep_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.at_prep_q_pipeline)))) {
                feint("at chain declined on this device — skipping\n")
                return
            }
            let nh = 4
            let nkvh = 2
            let hs = 64
            let half = 16   // PARTIAL rope: dims [32, 64) pass through unroped
            let qd = nh * hs
            let kvd = nkvh * hs
            let rows = 3
            let w0 = 5      // the k panel lands at absolute positions
            let eps = 0.00001
            let flags = 3   // gated q + qk-norm
            // smalls layout (offsets ride meta words / push args): eps@0, rmsq@16, rmsk@16+hs,
            // cos rows, sin rows
            let rmsq_off = 16
            let rmsk_off = 16 + hs
            let cos_off = 16 + 2 * hs
            let sin_off = cos_off + rows * half
            let sm_n = sin_off + rows * half
            var srcqh : array<float>
            var srckh : array<float>
            var smh : array<float>
            srcqh |> resize(rows * 2 * qd)
            srckh |> resize(rows * kvd)
            smh |> resize(sm_n)
            for (i in range(rows * 2 * qd)) {
                srcqh[i] = float((i * 19) % 79) * 0.045 - 1.7
            }
            for (i in range(rows * kvd)) {
                srckh[i] = float((i * 27) % 59) * 0.055 - 1.4
            }
            for (i in range(sm_n)) {
                smh[i] = float((i * 11) % 37) * 0.05 - 0.8
            }
            smh[0] = eps
            let srcq_bytes = int64(rows * 2 * qd) * 4l
            let srck_bytes = int64(rows * kvd) * 4l
            let sm_bytes = int64(sm_n) * 4l
            let qp_bytes = int64(rows * qd) * 4l
            let kp_bytes = int64((w0 + rows) * kvd) * 4l
            let srcqd = make_device_buf(srcq_bytes)
            let srckd = make_device_buf(srck_bytes)
            let smd = make_device_buf(sm_bytes)
            let qp1 = make_device_buf(qp_bytes)
            let g1 = make_device_buf(qp_bytes)
            let kp1 = make_device_buf(kp_bytes)
            let qp2 = make_device_buf(qp_bytes)
            let g2 = make_device_buf(qp_bytes)
            let kp2 = make_device_buf(kp_bytes)
            var meta = make_host_buf(META_BYTES, false, [cached = false])
            var host = make_host_buf(2l * qp_bytes + kp_bytes, true, [cached = true])
            var o_old : array<float>
            var o_cls : array<float>
            o_old |> resize(2 * rows * qd + (w0 + rows) * kvd)
            o_cls |> resize(2 * rows * qd + (w0 + rows) * kvd)
            unsafe {
                upload_region_at(srcqd, 0l, addr<uint8 const?>(srcqh[0]), srcq_bytes)
                upload_region_at(srckd, 0l, addr<uint8 const?>(srckh[0]), srck_bytes)
                upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                var mp = reinterpret<uint?>(meta.mapped)
                mp[1] = uint(w0)
                mp[2] = uint(qd)
                mp[3] = uint(kvd)
                mp[4] = uint(hs)
                mp[5] = uint(half)
                mp[6] = uint(nh)
                mp[7] = uint(nkvh)
                mp[9] = uint(flags)
                mp[13] = uint(sin_off)
                mp[14] = uint(rmsq_off)
                mp[15] = uint(rmsk_off)
                mp[16] = uint(cos_off)
                for (side in range(2)) {
                    let qpd = side == 0 ? qp1 : qp2
                    let gd = side == 0 ? g1 : g2
                    let kpd = side == 0 ? kp1 : kp2
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    if (side == 0) {
                        // the SHIPPED pair: panel@0, meta@2, gate stash@3, smalls@4, GEMM rows@5
                        var soq = vk_set6(qpd, meta.buf, meta.buf, gd, smd, srcqd,
                            qp_bytes, META_BYTES, META_BYTES, qp_bytes, sm_bytes, srcq_bytes,
                            fixed_array(1u, 0u, 0u, 2u, 4u, 8u))
                        var sok = vk_set6(kpd, meta.buf, meta.buf, gd, smd, srckd,
                            kp_bytes, META_BYTES, META_BYTES, qp_bytes, sm_bytes, srck_bytes,
                            fixed_array(16u, 0u, 0u, 0u, 4u, 8u))
                        hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_q_pipeline), soq, uint(rows))
                        hz_dispatch(raw, h, weak_copy(g_gpu.at_prep_k_pipeline), sok, uint(rows))
                    } else {
                        // the class collapse: ONE kernel, two dispatches with different push values
                        var scq = set_at_prep_cls(fixed_array(srcqd, qpd, gd, smd),
                            fixed_array(srcq_bytes, qp_bytes, qp_bytes, sm_bytes),
                            fixed_array(8u, 1u, 2u, 4u))
                        var sck = set_at_prep_cls(fixed_array(srckd, kpd, gd, smd),
                            fixed_array(srck_bytes, kp_bytes, qp_bytes, sm_bytes),
                            fixed_array(8u, 16u, 0u, 4u))
                        var pcq = AtPrepArgs(d = uint(qd), nh = uint(nh), hs = uint(hs),
                            half = uint(half), flags = uint(flags), rms_off = uint(rmsq_off),
                            cos_off = uint(cos_off), sin_off = uint(sin_off), obase = 0u,
                            qsrc = 1u, eps = eps)
                        enc_at_prep_cls(raw, h, scq, pcq, int64(rows))
                        var pck = AtPrepArgs(d = uint(kvd), nh = uint(nkvh), hs = uint(hs),
                            half = uint(half), flags = uint(flags), rms_off = uint(rmsk_off),
                            cos_off = uint(cos_off), sin_off = uint(sin_off), obase = uint(w0),
                            qsrc = 0u, eps = eps)
                        enc_at_prep_cls(raw, h, sck, pck, int64(rows))
                    }
                    vhz_dep(raw, h, 1u | 2u | 16u, 0u, true)
                    cmd_copy_whole(raw, qpd, host.buf, qp_bytes)
                    cmd_copy_range(raw, gd, 0l, host.buf, qp_bytes, qp_bytes)
                    cmd_copy_range(raw, kpd, 0l, host.buf, 2l * qp_bytes, kp_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    if (side == 0) {
                        memcpy(addr<void?>(o_old[0]), host.mapped, 2l * qp_bytes + kp_bytes)
                    } else {
                        memcpy(addr<void?>(o_cls[0]), host.mapped, 2l * qp_bytes + kp_bytes)
                    }
                }
            }
            var bad = 0
            for (i in range(2 * rows * qd)) {   // q panel + gate stash
                if (o_old[i] != o_cls[i]) {
                    bad++
                }
            }
            let kb0 = 2 * rows * qd + w0 * kvd   // k panel: only the written absolute rows
            for (i in range(rows * kvd)) {
                if (o_old[kb0 + i] != o_cls[kb0 + i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "at_prep: class collapse bit-exact ({bad} diverge)")
            delete srcqh
            delete srckh
            delete smh
            delete o_old
            delete o_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_dn_family(t0 : T?) {
    t0 |> run("deltanet class twins == the shipped kernels (bit-exact: conv, fused step, 2-phase scan)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_dn_conv_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!key_exists(g_pipe_access, intptr(boost_value_to_vk(g_gpu.dn_conv_pipeline)))) {
                feint("dn chain declined on this device — skipping\n")
                return
            }
            verify(ensure_dn_step_cls() && ensure_dn_scan_p1_cls() && ensure_dn_scan_p2_cls(),
                "class rails must engage when the shipped pipes exist")
            let ds = 64
            let nkh = 2
            let nvh = 4
            let kd = nkh * ds
            let cd = 2 * kd + nvh * ds
            let dconv = 4
            let taps = dconv - 1
            let eps = 0.00001
            let qscale = 1.25
            // --- dn_conv: depthwise conv + SiLU + q/k L2-norm, one wg per position ---
            {
                let npos = 3
                let hist_mode = 1
                let taps_off = 16
                let hist_off = taps_off + cd * dconv
                let sm_n = hist_off + cd * taps
                var srch : array<float>
                var smh : array<float>
                srch |> resize(npos * cd)
                smh |> resize(sm_n)
                for (i in range(npos * cd)) {
                    srch[i] = float((i * 19) % 79) * 0.03 - 1.1
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 11) % 37) * 0.02 - 0.35
                }
                smh[0] = eps
                smh[1] = qscale
                let src_bytes = int64(npos * cd) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let out_bytes = int64(npos * cd) * 4l
                let srcd = make_device_buf(src_bytes)
                let smd = make_device_buf(sm_bytes)
                let od1 = make_device_buf(out_bytes)
                let od2 = make_device_buf(out_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(out_bytes, true, [cached = true])
                var o_old : array<float>
                var o_cls : array<float>
                o_old |> resize(npos * cd)
                o_cls |> resize(npos * cd)
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[1] = uint(cd)
                    mp[2] = uint(kd)
                    mp[6] = uint(ds)
                    mp[7] = uint(dconv)
                    mp[8] = uint(hist_mode)
                    mp[13] = uint(taps_off)
                    mp[17] = uint(hist_off)
                    // the SHIPPED kernel: conv out@0, meta@2, smalls@4, proj rows@5
                    var so = vk_set6(od1, meta.buf, meta.buf, meta.buf, smd, srcd,
                        out_bytes, META_BYTES, META_BYTES, META_BYTES, sm_bytes, src_bytes,
                        fixed_array(1u, 0u, 0u, 0u, 4u, 8u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    hz_dispatch(raw, h, weak_copy(g_gpu.dn_conv_pipeline), so, uint(npos))
                    vhz_dep(raw, h, 1u, 0u, true)
                    cmd_copy_whole(raw, od1, host.buf, out_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(o_old[0]), host.mapped, out_bytes)
                    var sc = set_dn_conv_cls(fixed_array(srcd, od2, smd),
                        fixed_array(src_bytes, out_bytes, sm_bytes),
                        fixed_array(8u, 1u, 4u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DnConvArgs(cd = uint(cd), kd = uint(kd), ds = uint(ds),
                        dconv = uint(dconv), hist_mode = uint(hist_mode), taps_off = uint(taps_off),
                        hist_off = uint(hist_off), eps = eps, qscale = qscale)
                    enc_dn_conv_cls(raw2, h2, sc, pc, int64(npos))
                    vhz_dep(raw2, h2, 1u, 0u, true)
                    cmd_copy_whole(raw2, od2, host.buf, out_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(o_cls[0]), host.mapped, out_bytes)
                }
                var bad = 0
                for (i in range(npos * cd)) {
                    if (o_old[i] != o_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "dn_conv: class twin bit-exact ({bad} of {npos * cd} diverge)")
                delete srch
                delete smh
                delete o_old
                delete o_cls
            }
            // --- dn_step_fused: the whole per-v-head decode chain, state + ring in place ---
            {
                let par = 1
                let beta_off = 16
                let g_off = beta_off + nvh
                let a_off = g_off + nvh
                let dt_off = a_off + nvh
                let taps_off = dt_off + nvh
                let wnorm_off = taps_off + cd * dconv
                let hist_off = wnorm_off + ds
                let sm_n = hist_off + 2 * cd * taps
                let nblk = nvh * ds / 32
                var projh : array<float>
                var sth : array<float>
                var smh : array<float>
                projh |> resize(cd + nvh * ds)
                sth |> resize(nvh * ds * ds)
                smh |> resize(sm_n)
                for (i in range(cd + nvh * ds)) {
                    projh[i] = float((i * 23) % 71) * 0.03 - 1.0
                }
                for (i in range(nvh * ds * ds)) {
                    sth[i] = float((i * 13) % 53) * 0.02 - 0.5
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 17) % 43) * 0.02 - 0.4
                }
                smh[0] = eps
                smh[1] = qscale
                smh[2] = float(par)
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1   // a stays negative (decay)
                }
                let proj_bytes = int64(cd + nvh * ds) * 4l
                let st_bytes = int64(nvh * ds * ds) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let sc_bytes = int64(nblk) * 4l
                let q_bytes = int64(nblk) * 32l
                let projd = make_device_buf(proj_bytes)
                let st1 = make_device_buf(st_bytes)
                let st2 = make_device_buf(st_bytes)
                let sm1 = make_device_buf(sm_bytes)
                let sm2 = make_device_buf(sm_bytes)
                let scd1 = make_device_buf(sc_bytes)
                let scd2 = make_device_buf(sc_bytes)
                let qd1 = make_device_buf(q_bytes)
                let qd2 = make_device_buf(q_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                var host = make_host_buf(st_bytes + sm_bytes + sc_bytes + q_bytes, true, [cached = true])
                var o_old : array<uint>
                var o_cls : array<uint>
                let cmp_words = int((st_bytes + sm_bytes + sc_bytes + q_bytes) / 4l)
                o_old |> resize(cmp_words)
                o_cls |> resize(cmp_words)
                unsafe {
                    upload_region_at(projd, 0l, addr<uint8 const?>(projh[0]), proj_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[1] = uint(cd)
                    mp[2] = uint(kd)
                    mp[5] = uint(nkh)
                    mp[6] = uint(ds)
                    mp[7] = uint(dconv)
                    mp[11] = uint(beta_off)
                    mp[12] = uint(g_off)
                    mp[13] = uint(taps_off)
                    mp[14] = uint(wnorm_off)
                    mp[15] = uint(a_off)
                    mp[16] = uint(dt_off)
                    mp[17] = uint(hist_off)
                    for (side in range(2)) {
                        let std = side == 0 ? st1 : st2
                        let smd = side == 0 ? sm1 : sm2
                        let scd = side == 0 ? scd1 : scd2
                        let qdv = side == 0 ? qd1 : qd2
                        upload_region_at(std, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                        upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                        var raw = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw, begin), null)
                        var h : VkHaz
                        if (side == 0) {
                            // the SHIPPED kernel: scales@0, state@1, meta@2, quants@3, smalls@4 (rw), proj@5
                            var so = vk_set6(scd, std, meta.buf, qdv, smd, projd,
                                sc_bytes, st_bytes, META_BYTES, q_bytes, sm_bytes, proj_bytes,
                                fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                            hz_dispatch(raw, h, weak_copy(g_gpu.dnd_fused_pipeline), so, uint(nvh))
                        } else {
                            var sc = set_dn_step_cls(fixed_array(projd, std, smd, scd, qdv),
                                fixed_array(proj_bytes, st_bytes, sm_bytes, sc_bytes, q_bytes),
                                fixed_array(16u, 2u, 8u, 1u, 4u))
                            var pc = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh),
                                ds = uint(ds), dconv = uint(dconv), beta_off = uint(beta_off),
                                g_off = uint(g_off), taps_off = uint(taps_off),
                                wnorm_off = uint(wnorm_off), a_off = uint(a_off),
                                dt_off = uint(dt_off), hist_off = uint(hist_off),
                                par = uint(par), eps = eps, qscale = qscale)
                            enc_dn_step_cls(raw, h, sc, pc, int64(nvh))
                        }
                        vhz_dep(raw, h, 1u | 2u | 4u | 8u, 0u, true)
                        cmd_copy_whole(raw, std, host.buf, st_bytes)
                        cmd_copy_range(raw, smd, 0l, host.buf, st_bytes, sm_bytes)
                        cmd_copy_range(raw, scd, 0l, host.buf, st_bytes + sm_bytes, sc_bytes)
                        cmd_copy_range(raw, qdv, 0l, host.buf, st_bytes + sm_bytes + sc_bytes, q_bytes)
                        vk_check(vkEndCommandBuffer(raw), null)
                        submit_wait(raw)
                        if (side == 0) {
                            memcpy(addr<void?>(o_old[0]), host.mapped, int64(cmp_words) * 4l)
                        } else {
                            memcpy(addr<void?>(o_cls[0]), host.mapped, int64(cmp_words) * 4l)
                        }
                    }
                }
                var bad = 0
                for (i in range(cmp_words)) {
                    if (o_old[i] != o_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "dn_step_fused: class twin bit-exact ({bad} of {cmp_words} words diverge)")
                delete projh
                delete sth
                delete smh
                delete o_old
                delete o_cls
            }
            // --- dn_scan p1 + p2 chained: 2 chunks (64 + a 6-row partial), state advanced through both ---
            {
                let npos = 70
                let CS = 64
                let nch = (npos + CS - 1) / CS
                let di = nvh * ds
                let slab_stride = 7 * CS * ds + 2 * CS * CS + 4 * CS
                let st_off = nvh * nch * slab_stride
                let o_off = st_off + nvh * (ds * ds + CS * ds)
                let slab_n = o_off + npos * di
                let beta_off = 16
                let g_off = beta_off + npos * nvh
                let a_off = g_off + npos * nvh
                let dt_off = a_off + nvh
                let wnorm_off = dt_off + nvh
                let sm_n = wnorm_off + ds
                var convh : array<float>
                var zh : array<float>
                var sth : array<float>
                var smh : array<float>
                convh |> resize(npos * cd)
                zh |> resize(npos * di)
                sth |> resize(nvh * ds * ds)
                smh |> resize(sm_n)
                for (i in range(npos * cd)) {
                    convh[i] = float((i * 19) % 79) * 0.025 - 0.9
                }
                for (i in range(npos * di)) {
                    zh[i] = float((i * 29) % 61) * 0.05 - 1.5
                }
                for (i in range(nvh * ds * ds)) {
                    sth[i] = float((i * 13) % 53) * 0.02 - 0.5
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 17) % 43) * 0.03 - 0.6
                }
                smh[0] = eps
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1
                }
                for (i in range(ds)) {
                    smh[wnorm_off + i] = 0.8 + float(i % 9) * 0.04
                }
                let conv_bytes = int64(npos * cd) * 4l
                let z_bytes = int64(npos * di) * 4l
                let st_bytes = int64(nvh * ds * ds) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let slab_bytes = int64(slab_n) * 4l
                let convd = make_device_buf(conv_bytes)
                let zd = make_device_buf(z_bytes)
                let st1 = make_device_buf(st_bytes)
                let st2 = make_device_buf(st_bytes)
                let smd = make_device_buf(sm_bytes)
                let sl1 = make_device_buf(slab_bytes)
                let sl2 = make_device_buf(slab_bytes)
                var meta = make_host_buf(META_BYTES, false, [cached = false])
                let o_bytes = int64(npos * di) * 4l
                var host = make_host_buf(o_bytes + st_bytes, true, [cached = true])
                var o_old : array<float>
                var o_cls : array<float>
                let cmp_n = npos * di + nvh * ds * ds
                o_old |> resize(cmp_n)
                o_cls |> resize(cmp_n)
                unsafe {
                    upload_region_at(convd, 0l, addr<uint8 const?>(convh[0]), conv_bytes)
                    upload_region_at(zd, 0l, addr<uint8 const?>(zh[0]), z_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var mp = reinterpret<uint?>(meta.mapped)
                    mp[0] = uint(npos)
                    mp[1] = uint(cd)
                    mp[2] = uint(kd)
                    mp[3] = uint(di)
                    mp[4] = uint(nvh)
                    mp[5] = uint(nkh)
                    mp[6] = uint(ds)
                    mp[9] = uint(slab_stride)
                    mp[10] = uint(o_off)
                    mp[11] = uint(beta_off)
                    mp[12] = uint(g_off)
                    mp[14] = uint(wnorm_off)
                    mp[15] = uint(a_off)
                    mp[16] = uint(dt_off)
                    mp[19] = uint(st_off)
                    for (side in range(2)) {
                        let std = side == 0 ? st1 : st2
                        let sld = side == 0 ? sl1 : sl2
                        upload_region_at(std, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                        var raw = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw, begin), null)
                        var h : VkHaz
                        if (side == 0) {
                            // one set serves both phases: conv@0, state@1, meta@2, slab@3, smalls@4, z@5
                            var so = vk_set6(convd, std, meta.buf, sld, smd, zd,
                                conv_bytes, st_bytes, META_BYTES, slab_bytes, sm_bytes, z_bytes,
                                fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                            hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p1_pipeline), so, uint(nvh * nch))
                            hz_dispatch(raw, h, weak_copy(g_gpu.dn_scan_p2_pipeline), so, uint(nvh))
                        } else {
                            var sc1 = set_dn_scan_p1_cls(fixed_array(convd, smd, sld),
                                fixed_array(conv_bytes, sm_bytes, slab_bytes),
                                fixed_array(1u, 8u, 4u))
                            var pc1 = DnScanP1Args(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                                nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                                beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                                dt_off = uint(dt_off), slab_stride = uint(slab_stride))
                            enc_dn_scan_p1_cls(raw, h, sc1, pc1, int64(nvh * nch))
                            var sc2 = set_dn_scan_p2_cls(fixed_array(zd, std, sld, smd),
                                fixed_array(z_bytes, st_bytes, slab_bytes, sm_bytes),
                                fixed_array(16u, 2u, 4u, 8u))
                            var pc2 = DnScanP2Args(npos = uint(npos), di = uint(di), ds = uint(ds),
                                o_off = uint(o_off), wnorm_off = uint(wnorm_off),
                                slab_stride = uint(slab_stride), st_off = uint(st_off), eps = eps)
                            enc_dn_scan_p2_cls(raw, h, sc2, pc2, int64(nvh))
                        }
                        vhz_dep(raw, h, 2u | 4u, 0u, true)
                        cmd_copy_range(raw, sld, int64(o_off) * 4l, host.buf, 0l, o_bytes)
                        cmd_copy_range(raw, std, 0l, host.buf, o_bytes, st_bytes)
                        vk_check(vkEndCommandBuffer(raw), null)
                        submit_wait(raw)
                        if (side == 0) {
                            memcpy(addr<void?>(o_old[0]), host.mapped, o_bytes + st_bytes)
                        } else {
                            memcpy(addr<void?>(o_cls[0]), host.mapped, o_bytes + st_bytes)
                        }
                    }
                }
                var bad = 0
                for (i in range(cmp_n)) {
                    if (o_old[i] != o_cls[i]) {
                        bad++
                    }
                }
                t |> success(bad == 0, "dn_scan p1+p2: class twins bit-exact ({bad} of {cmp_n} diverge)")
                delete convh
                delete zh
                delete sth
                delete smh
                delete o_old
                delete o_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}
