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
                        s_cls = set_kq_gemv_k4_cls(cb, cs, cg)
                    } elif (fmt == int(KqFmt.q40)) {
                        s_cls = set_kq_gemv_q40_cls(cb, cs, cg)
                    } elif (fmt == int(KqFmt.k5)) {
                        s_cls = set_kq_gemv_k5_cls(cb, cs, cg)
                    } else {
                        s_cls = set_kq_gemv_k6_cls(cb, cs, cg)
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
