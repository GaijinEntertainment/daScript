options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require math
require dasllama/dasllama_math
require ?vulkan _vkd_toy
require ?vulkan _vkd_oracles
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_vulkan_seams
require ?vulkan dasllama/dasllama_kqformat
require ?vulkan daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane (only vulkan-guarded arms replay)

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

// ===== CPU oracles =====
// The reference side of every arm: plain sequential math (the Metal kernel-unit pattern), or the
// class's own per-block methods driven lane-free. Device subgroup/tile folds reassociate float
// sums, so float compares take the relative bar; integer packs and max folds are order-free and
// stay exact, except quants downstream of exp/tanh (GPU transcendentals differ by ulps — those
// compare as signed bytes within +-1).

def private approx(x, y : float) : bool => abs(x - y) <= 2e-3 * max(1.0, max(abs(x), abs(y)))

def private mismatch_rel(got, want : array<float>; bar : float) : int {
    var bad = 0
    for (g, w in got, want) {
        if (abs(g - w) > bar * max(1.0, max(abs(g), abs(w)))) {
            bad++
        }
    }
    return bad
}

def private mismatch_approx(got, want : array<float>) : int => mismatch_rel(got, want, 2e-3)

// relative bar + an absolute floor — for f16-accumulating kernels, whose error scales with the
// running-sum magnitude, not the (possibly cancelled-small) result
def private mismatch_bars(got, want : array<float>; rel, absf : float) : int {
    var bad = 0
    for (g, w in got, want) {
        if (abs(g - w) > absf && abs(g - w) > rel * max(1.0, max(abs(g), abs(w)))) {
            bad++
        }
    }
    return bad
}

def private max_abs(a : array<float>) : float {
    var m = 0.0
    for (v in a) {
        m = max(m, abs(v))
    }
    return m
}

def private mismatch_exact(got, want : array<float>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

// packed quant words as signed bytes within +-tol (act chains carry transcendental ulp noise)
def private mismatch_qbytes(got, want : array<uint>; tol : int) : int {
    var bad = 0
    for (g, w in got, want) {
        for (sh in range(4)) {
            let a = int(g >> uint(sh * 8)) & 0xFF
            let b = int(w >> uint(sh * 8)) & 0xFF
            let sa = a >= 128 ? a - 256 : a
            let sb = b >= 128 ? b - 256 : b
            if (abs(sa - sb) > tol) {
                bad++
            }
        }
    }
    return bad
}

// the one activation formula (act_mul_sel's shape) — definitional, kept in sync by the arm bars
def private act_mul_ref(v, u : float; gelu : bool) : float {
    return (gelu ? (0.5 * v * (1.0 + tanh(0.7978845608028654 * (v + 0.044715 * v * v * v)))) * u
        : (v / (1.0 + exp(-v))) * u)
}

def private pack4_ref(v0, v1, v2, v3, id : float) : uint {
    let q0 = int(round(v0 * id))
    let q1 = int(round(v1 * id))
    let q2 = int(round(v2 * id))
    let q3 = int(round(v3 * id))
    return ((uint(q0) & 0xFFu) | ((uint(q1) & 0xFFu) << 8u)
        | ((uint(q2) & 0xFFu) << 16u) | ((uint(q3) & 0xFFu) << 24u))
}

// Q8_0 / Q8_K quantize of act(gate)*up rows: sequential amax (max is order-free), linear pack
def private actrq_oracle(gh, uh : array<float>; nelem : int; q8k, gelu : bool;
                         var outq : array<uint>; var outs : array<float>) {
    let bsz = q8k ? 256 : 32
    let nblk = nelem / bsz
    outq |> resize(nelem / 4)
    outs |> resize(nblk)
    for (b in range(nblk)) {
        var m = 0.0
        for (e in range(bsz)) {
            m = max(m, abs(act_mul_ref(gh[b * bsz + e], uh[b * bsz + e], gelu)))
        }
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        outs[b] = d
        for (wj in range(bsz / 4)) {
            let e0 = b * bsz + wj * 4
            outq[b * (bsz / 4) + wj] = pack4_ref(
                act_mul_ref(gh[e0], uh[e0], gelu), act_mul_ref(gh[e0 + 1], uh[e0 + 1], gelu),
                act_mul_ref(gh[e0 + 2], uh[e0 + 2], gelu), act_mul_ref(gh[e0 + 3], uh[e0 + 3], gelu), id)
        }
    }
}

// plain Q8_0 / Q8_K requant of f32 rows (no activation — exact modulo round ties)
def private requant_oracle(src : array<float>; base, nelem : int; q8k : bool;
                           var outq : array<uint>; var outs : array<float>) {
    let bsz = q8k ? 256 : 32
    let nblk = nelem / bsz
    outq |> resize(nelem / 4)
    outs |> resize(nblk)
    for (b in range(nblk)) {
        var m = 0.0
        for (e in range(bsz)) {
            m = max(m, abs(src[base + b * bsz + e]))
        }
        let d = m / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        outs[b] = d
        for (wj in range(bsz / 4)) {
            let e0 = base + b * bsz + wj * 4
            outq[b * (bsz / 4) + wj] = pack4_ref(src[e0], src[e0 + 1], src[e0 + 2], src[e0 + 3], id)
        }
    }
}

// single-query causal attention oracle: one output row per (query row, head) — plain two-pass
// softmax over [0, cnt); the device's online form reassociates, so approx bars
def private attn_row_oracle(q : array<float>; qbase : int; kk : array<float>; kbase0 : int;
                            vv : array<float>; vbase0 : int; kvd, hh, hs, kv_mul, cnt : int;
                            scale : float; var out : array<float>; obase : int) {
    let khoff = (hh / kv_mul) * hs
    var sc : array<float>
    sc |> resize(cnt)
    var mm = -3.0e38
    for (j in range(cnt)) {
        var s = 0.0
        for (e in range(hs)) {
            s += q[qbase + e] * kk[kbase0 + j * kvd + khoff + e]
        }
        sc[j] = s * scale
        mm = max(mm, sc[j])
    }
    var tot = 0.0
    for (j in range(cnt)) {
        sc[j] = exp(sc[j] - mm)
        tot += sc[j]
    }
    for (e in range(hs)) {
        var acc = 0.0
        for (j in range(cnt)) {
            acc += sc[j] * vv[vbase0 + j * kvd + khoff + e]
        }
        out[obase + e] = acc / tot
    }
    delete sc
}

[test]
def test_vkd_kq_gemv_family(t0 : T?) {
    t0 |> run("kq GEMV class family == the CPU oracle (all 4 formats)") <| @(t : T?) {
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
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nsb)
                    regs[r * 2 + 1] = 0u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    // the class kernel — regions SSBO + push params (one family set serves all four)
                    let cb = fixed_array(wqd, wsd, rgd, xqd, xsd, yd2)
                    let cs = fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes)
                    let cg = fixed_array(1u, 2u, 4u, 8u, 16u, 32u)
                    var s_cls = set_kq_gemv_cls(cb, cs, cg)
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                }
                var y_ref <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, regs, 512, d, nreg)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "fmt {fmt}: class kernel matches the CPU oracle ({bad} of {rows} rows off)")
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_act_family(t0 : T?) {
    t0 |> run("act/requant + combine class kernels == the CPU oracle") <| @(t : T?) {
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
                let hq2 = make_device_buf(q_bytes)
                let hs2 = make_device_buf(s_bytes)
                var hostq = make_host_buf(q_bytes, true, [cached = true])
                var hosts = make_host_buf(s_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nelem / 4)
                s_cls |> resize(nblk)
                let wgs = 1l   // 256 threads cover both shapes at nelem = 1024
                unsafe {
                    // the class kernel: gate@0, up@1, outq@2, outs@3
                    var sc = set_actrq_cls(fixed_array(gd, ud, hq2, hs2),
                        fixed_array(e_bytes, e_bytes, q_bytes, s_bytes),
                        fixed_array(16u, 1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                var q_ref : array<uint>
                var s_ref : array<float>
                actrq_oracle(gh, uh, nelem, q8k, false, q_ref, s_ref)
                // exp/tanh differ by ulps between backends: quants compare as bytes +-1, scales approx
                let badq = mismatch_qbytes(q_cls, q_ref, 1)
                let bads = mismatch_approx(s_cls, s_ref)
                t |> success(badq == 0 && bads == 0, "{q8k ? "q8k" : "q8"} actrq matches the CPU oracle ({badq} bytes / {bads} scales off)")
                delete q_ref
                delete s_ref
                delete q_cls
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
                let ac2 = make_device_buf(ac_bytes)
                var hosta = make_host_buf(ac_bytes, true, [cached = true])
                var a_cls : array<float>
                a_cls |> resize(npos * n)
                unsafe {
                    upload_region_at(sid, 0l, addr<uint8 const?>(sidx[0]), si_bytes)
                    upload_region_at(swd, 0l, addr<uint8 const?>(sw[0]), si_bytes)
                    upload_region_at(rod, 0l, addr<uint8 const?>(rows[0]), ro_bytes)
                    upload_region_at(ac2, 0l, addr<uint8 const?>(accs[0]), ac_bytes)
                    var sc = set_combine_cls(fixed_array(ac2, sid, swd, rod),
                        fixed_array(ac_bytes, si_bytes, si_bytes, ro_bytes),
                        fixed_array(1u, 2u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                // oracle: same slot order, but the device contracts mul+add to fma — approx bar
                var a_ref : array<float>
                a_ref |> resize(npos * n)
                for (p in range(npos)) {
                    for (i in range(n)) {
                        var acc = 0.0
                        for (j in range(k)) {
                            acc += sw[p * k + j] * rows[int(sidx[p * k + j]) * n + i]
                        }
                        a_ref[p * n + i] = acc
                    }
                }
                let bad = mismatch_approx(a_cls, a_ref)
                t |> success(bad == 0, "combine matches the CPU oracle ({bad} of {npos * n} off)")
                delete sidx
                delete sw
                delete rows
                delete accs
                delete a_ref
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
    t0 |> run("rope_kv_store + qk_rms class kernels == the CPU oracle") <| @(t : T?) {
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
                let q2 = make_device_buf(q_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(kv_bytes, true, [cached = true])
                var cls_q : array<float>
                var cls_k : array<float>
                var cls_v : array<float>
                cls_q |> resize(qd)
                cls_k |> resize(kvd)
                cls_v |> resize(kvd)
                let wgs = int64((npairs + 255) / 256)
                unsafe {
                    upload_region_at(kvd_dev, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    // the class kernel: q@0, kvsrc@1, kmir@2, vmir@3, cossin@4, tok@5 (pos 0 -> row 0)
                    var tokb = make_host_buf(16l, true)
                    var tm = reinterpret<TokMeta?>(tokb.mapped)
                    tm.pos = 0u
                    var sc = set_rope_kv_cls(fixed_array(q2, kvd_dev, km2, vm2, csd, tokb.buf),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, 16l),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 0u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(half),
                        neox = 1u, kpair0 = uint(kpair0), npairs = uint(npairs), layerbase = 0u)
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
                // oracle: the same per-pair rotation sequentially (NEOX pairs (j, j+half));
                // v copies raw — approx bars ride out the device's fma contraction
                var ref_q : array<float>
                var ref_k : array<float>
                var ref_v : array<float>
                ref_q |> resize(qd)
                ref_k |> resize(kvd)
                ref_v |> resize(kvd)
                for (i in range(qd)) {
                    ref_q[i] = qh[i]
                }
                for (gid in range(npairs)) {
                    let is_k = gid >= kpair0
                    let lp = is_k ? gid - kpair0 : gid
                    let hh = lp / half
                    let j = lp % half
                    let fcr = csh[j]
                    let fci = csh[half + j]
                    if (is_k) {
                        let a0 = kvh[qd + hh * hs + j]
                        let a1 = kvh[qd + hh * hs + j + half]
                        ref_k[hh * hs + j] = a0 * fcr - a1 * fci
                        ref_k[hh * hs + j + half] = a0 * fci + a1 * fcr
                        ref_v[hh * hs + j] = kvh[qd + kvd + hh * hs + j]
                        ref_v[hh * hs + j + half] = kvh[qd + kvd + hh * hs + j + half]
                    } else {
                        let a0 = qh[hh * hs + j]
                        let a1 = qh[hh * hs + j + half]
                        ref_q[hh * hs + j] = a0 * fcr - a1 * fci
                        ref_q[hh * hs + j + half] = a0 * fci + a1 * fcr
                    }
                }
                let bad = mismatch_approx(cls_q, ref_q) + mismatch_approx(cls_k, ref_k) + mismatch_approx(cls_v, ref_v)
                t |> success(bad == 0, "rope_kv_store matches the CPU oracle ({bad} elements off)")
                delete qh
                delete kvh
                delete csh
                delete ref_q
                delete ref_k
                delete ref_v
                delete cls_q
                delete cls_k
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
                let q2 = make_device_buf(q_bytes)
                let k2 = make_device_buf(k_bytes)
                let wd = make_device_buf(w_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var cls_q : array<float>
                var cls_k : array<float>
                cls_q |> resize(rows * qd)
                cls_k |> resize(rows * kvd)
                let wgs = int64(rows * (nh + nkvh))
                unsafe {
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(k2, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
                    // the class kernel: q@0, k@1, weights@2
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
                // oracle: per-head rms (1/sqrt(mean(v^2)+eps)) times the shared weight row —
                // the device reduces sum-of-squares in tree order, so approx bars
                var ref_q : array<float>
                var ref_k : array<float>
                ref_q |> resize(rows * qd)
                ref_k |> resize(rows * kvd)
                for (r in range(rows)) {
                    for (hh in range(nh + nkvh)) {
                        let is_q = hh < nh
                        let base = is_q ? r * qstride + hh * hs : r * kstride + (hh - nh) * hs
                        let wbase = is_q ? 0 : hs
                        var ss = 0.0
                        for (i in range(hs)) {
                            let v = is_q ? qh[base + i] : kh[base + i]
                            ss += v * v
                        }
                        let inv = 1.0 / sqrt(ss / float(hs) + eps)
                        for (i in range(hs)) {
                            if (is_q) {
                                ref_q[base + i] = wh[wbase + i] * (qh[base + i] * inv)
                            } else {
                                ref_k[base + i] = wh[wbase + i] * (kh[base + i] * inv)
                            }
                        }
                    }
                }
                let bad = mismatch_approx(cls_q, ref_q) + mismatch_approx(cls_k, ref_k)
                t |> success(bad == 0, "qk_rms matches the CPU oracle ({bad} elements off)")
                delete qh
                delete kh
                delete wh
                delete eh
                delete ref_q
                delete ref_k
                delete cls_q
                delete cls_k
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_q8_batch_tile(t0 : T?) {
    t0 |> run("q8 batch tile class kernel == the CPU oracle (edge tiles + K tail)") <| @(t : T?) {
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
            let yd2 = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
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
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                // the class kernel: records at 0, map at map_off
                var sc = set_q8_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8_batch_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            q8_gemm_oracle(wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
            let bad = mismatch_approx(y_cls, y_ref)
            t |> success(bad == 0, "q8 batch matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete wsh
            delete xqh
            delete xsh
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_kq_batch_family(t0 : T?) {
    t0 |> run("kq batch tile class family == the CPU oracle (all 4 formats)") <| @(t : T?) {
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
                let yd2 = make_device_buf(y_bytes)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * d)
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
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    // one family set serves all four formats
                    var sc = set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                }
                var y_ref : array<float>
                y_ref |> resize(rows * d)
                kq_gemm_oracle(fmt, wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "kq batch fmt {fmt} matches the CPU oracle ({bad} of {rows * d} off)")
                delete sched
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cm_batch_family(t0 : T?) {
    t0 |> run("coopmat batch class kernels == the CPU oracle (per available mode)") <| @(t : T?) {
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
                    d = 128   // row-edge tiles only: non-128-multiple d ZEROES the whole mm
                    cnt0 = 140   // dispatch (old kernel identical — latent, ledgered) — real model
                    cnt1 = 116   // dims are 128-multiples, so serving never routes that shape
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
                // the router picks this variant only in its coopmat mode — building a cm class
                // pipeline on a device whose mode never routes it is wasted (and panics no-cm)
                let mode = g_gpu.coopmat_mode
                let on = (v == 0 && mode == 1) || (v == 1 && mode == 2) || (v >= 2 && mode >= 3)
                if (!on) {
                    continue
                }
                let up = (v == 0 ? ensure_q8_batch_cmf16_cls()
                    : (v == 1 ? ensure_q8_batch_cmi8_cls()
                    : (v == 2 ? ensure_q8_batch_mm_cls()
                    : (v == 3 ? ensure_q8_batch_mm_a_cls() : ensure_q8_batch_mm_m_cls()))))
                assert(up, "class rail must engage in its coopmat mode")
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
                    // 16x cooler than the sdot4 arms: the mm family ACCUMULATES in f16, and the
                    // hot form's |y| ~7e4 overflows f16's 65504 to inf
                    xsh[i] = (0.375 + float(i % 9) * 0.0625) * 0.0625
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
                let yd2 = make_device_buf(y_bytes)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * d)
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
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    let bufs = fixed_array(wqd, wsd, scd, xqd, xsd, yd2)
                    let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes)
                    let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
                    var sc = (v == 0 ? set_q8_batch_cmf16_cls(bufs, sizes, gbits)
                        : (v == 1 ? set_q8_batch_cmi8_cls(bufs, sizes, gbits)
                        : (v == 2 ? set_q8_batch_mm_cls(bufs, sizes, gbits)
                        : (v == 3 ? set_q8_batch_mm_a_cls(bufs, sizes, gbits)
                        : set_q8_batch_mm_m_cls(bufs, sizes, gbits)))))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                }
                // the cm kernels stage AND accumulate through f16, so their error scales with
                // the running-sum magnitude (cancelled-small results included) — relative bar
                // plus an absolute floor off the oracle's peak; wiring breaks are peak-sized
                var y_ref : array<float>
                y_ref |> resize(rows * d)
                q8_gemm_oracle(wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
                let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
                to_log(LOG_INFO, "cm batch variant {v}: {bad} of {rows * d} off the oracle\n")
                t |> success(bad == 0, "cm batch variant {v} matches the CPU oracle ({bad} of {rows * d} off)")
                delete sched
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
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
    t0 |> run("q40 coopmat f16 batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                feint("no Vulkan device — skipping\n")
                return
            }
            if (g_gpu.coopmat_mode != 1) {
                feint("q40 rides the f16 coopmat twin only in mode 1 (DASLLAMA_COOPMAT=f16) — skipping\n")
                return
            }
            verify(ensure_kq_batch_q40_cmf16_cls(), "class rail must engage in f16 coopmat mode")
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
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
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
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsud, 0l, addr<uint8 const?>(wsuh[0]), wsu_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                var sc = set_kq_batch_q40_cmf16_cls(fixed_array(wqd, wsud, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, wsu_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_kq_batch_q40_cmf16_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            // f16-staged dequant on the MMA path — relative bar + peak-scaled floor
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            kq_gemm_oracle(int(KqFmt.q40), wqh, wsuh, xqh, xsh, sched, 2, n, d, y_ref)
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            to_log(LOG_INFO, "q40 cm batch: {bad} of {rows * d} off the oracle\n")
            t |> success(bad == 0, "q40 cm batch matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete wsuh
            delete xqh
            delete xsh
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cm2_batch(t0 : T?) {
    t0 |> run("cm2 decode-in-load batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!(g_gpu.coopmat_mode == 4 && g_gpu.has_coopmat2)) {
                feint("the cm2 twin serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2) — skipping\n")
                return
            }
            verify(ensure_q8n_batch_cm2_cls(), "class rail must engage in cm2 mode")
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
            let yd2 = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
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
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(xfd, 0l, addr<uint8 const?>(xfh[0]), xf_bytes)
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                // the class kernel: sparse bindings 0/2/3/5
                var sc = set_q8n_batch_cm2_cls(fixed_array(wqd, scd, xfd, yd2),
                    fixed_array(wq_bytes, sc_bytes, xf_bytes, y_bytes),
                    fixed_array(1u, 0u, 4u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8n_batch_cm2_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            // in-load q8n dequant + f16 MMA — relative bar + peak-scaled floor
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            q8n_gemm_oracle(wqh, xfh, sched, 2, n, d, y_ref)
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            to_log(LOG_INFO, "cm2 batch: {bad} of {rows * d} off the oracle\n")
            t |> success(bad == 0, "cm2 batch matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete xfh
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_da_attn(t0 : T?) {
    t0 |> run("decode attention class kernel == the CPU oracle (chunk boundary)") <| @(t : T?) {
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
            let yd2 = make_device_buf(q_bytes)
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(nheads * hs)
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kd, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                upload_region_at(vd, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                // the class kernel: q@0, K@1, V@2, y@3, tok@4 (cnt rides TokMeta)
                var tokb = make_host_buf(16l, true)
                var tmc = reinterpret<TokMeta?>(tokb.mapped)
                tmc.cnt = uint(cnt)
                var sc = set_da_attn_cls(fixed_array(qd, kd, vd, yd2, tokb.buf),
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l),
                    fixed_array(1u, 2u, 4u, 16u, 0u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                    kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale)
                enc_da_attn_cls(raw2, h2, sc, pc, int64(nheads))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            var y_ref : array<float>
            y_ref |> resize(nheads * hs)
            for (hh in range(nheads)) {
                attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_ref, hh * hs)
            }
            let bad = mismatch_approx(y_cls, y_ref)
            t |> success(bad == 0, "da_attn matches the CPU oracle ({bad} of {nheads * hs} off)")
            delete qh
            delete kh
            delete vh
            delete sh
            delete y_ref
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_at_attn(t0 : T?) {
    t0 |> run("prefill attention class kernel == the CPU oracle (partial tiles, gated)") <| @(t : T?) {
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
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * qd)
                let wgs = nheads * ((rows + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                    // the class kernel: q@0, K@1, V@2, out@3 (pre-loaded with the gates)
                    upload_region_at(yd2, 0l, addr<uint8 const?>(gh[0]), q_bytes)
                    var sc = set_at_attn_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 4u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                // oracle: causal rows (row r attends [0, w0 + r]); gated = out * sigmoid(gate)
                var y_ref : array<float>
                y_ref |> resize(rows * qd)
                for (r in range(rows)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul,
                            w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                if (gated) {
                    for (i in range(rows * qd)) {
                        y_ref[i] = y_ref[i] / (1.0 + exp(-gh[i]))
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "at_attn hs {hs} matches the CPU oracle ({bad} of {rows * qd} off)")
                delete qh
                delete kh
                delete vh
                delete gh
                delete y_ref
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_da_attn_b(t0 : T?) {
    t0 |> run("batched decode attention class kernel == the CPU oracle (mirror offsets)") <| @(t : T?) {
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
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                    // the class kernel: q@0, K@1, V@2, out@3
                    var sc = set_da_attn_b_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                // oracle: causal rows over the offset mirrors (row r attends [0, w0 + r])
                var y_ref : array<float>
                y_ref |> resize(npos * qd)
                for (r in range(npos)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_b hs {hs} matches the CPU oracle ({bad} of {npos * qd} off)")
                delete qh
                delete kh
                delete vh
                delete y_ref
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_da_attn_b_h128(t0 : T?) {
    t0 |> run("h128 coopmat attention class kernel == the CPU oracle") <| @(t : T?) {
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
            let yd2 = make_device_buf(q_bytes)
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(npos * qd)
            let wgs = nheads * ((npos + 31) / 32)
            unsafe {
                upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                // the class kernel: q@0, K@1, V@2, out@3
                var sc = set_da_attn_b_h128_cls(fixed_array(qdev, kdev, vdev, yd2),
                    fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                    fixed_array(1u, 2u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
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
            // the h128 twin stages q/K/V through f16 pairs for the MMAs — floor-barred like the
            // cm GEMMs (softmax weights bound the V fold, so the peak floor is generous enough)
            var y_ref : array<float>
            y_ref |> resize(npos * qd)
            for (r in range(npos)) {
                for (hh in range(nheads)) {
                    attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                        kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                }
            }
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            t |> success(bad == 0, "da_attn_b_h128 matches the CPU oracle ({bad} of {npos * qd} off)")
            delete qh
            delete kh
            delete vh
            delete y_ref
            delete y_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_requant_family(t0 : T?) {
    t0 |> run("fused ar+requant + q8k requant class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_cls_ar_rq() && ensure_cls_q8k_rq())) {
                feint("no Vulkan device — skipping\n")
                return
            }
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
                x |> resize(dim)
                a |> resize(dim)
                w |> resize(dim)
                for (i in range(dim)) {
                    x[i] = float((i * 37) % 101) * 0.09 - 4.2
                    a[i] = float((i * 13) % 89) * 0.06 - 2.6
                    w[i] = 0.5 + float(i % 17) * 0.04
                }
                let e_bytes = int64(dim) * 4l
                let q_bytes = int64(dim)
                let s_bytes = int64(nblk) * 4l
                let x2 = make_device_buf(e_bytes)
                let ad = make_device_buf(e_bytes)
                let wd = make_device_buf(e_bytes)
                let q2 = make_device_buf(q_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(e_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                var x_cls : array<float>
                q_cls |> resize(dim / 4)
                s_cls |> resize(nblk)
                x_cls |> resize(dim)
                unsafe {
                    upload_region_at(x2, 0l, addr<uint8 const?>(x[0]), e_bytes)
                    upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), e_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), e_bytes)
                    // the class kernel: x@0, a@1, w@2, outq@3, outs@4
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
                // oracle: x' = (x+a)*ascale in place, rms over the row, y = w*(x'*inv), Q8_0 pack —
                // the wg reduce reassociates the sum of squares, so quants ride the byte bar
                var x_ref : array<float>
                var y_ref : array<float>
                x_ref |> resize(dim)
                y_ref |> resize(dim)
                var ss = 0.0
                for (i in range(dim)) {
                    x_ref[i] = (x[i] + a[i]) * ascale
                    ss += x_ref[i] * x_ref[i]
                }
                let inv = 1.0 / sqrt(ss / float(dim) + eps)
                for (i in range(dim)) {
                    y_ref[i] = w[i] * (x_ref[i] * inv)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(y_ref, 0, dim, false, q_ref, s_ref)
                let bad = (mismatch_qbytes(q_cls, q_ref, 1) + mismatch_approx(s_cls, s_ref)
                    + mismatch_approx(x_cls, x_ref))
                t |> success(bad == 0, "ar+requant fused matches the CPU oracle ({bad} elements off)")
                delete x
                delete a
                delete w
                delete x_ref
                delete y_ref
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
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
                let q2 = make_device_buf(q_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nelem / 4)
                s_cls |> resize(nsb)
                let wgs = (nsb * 32 + 255) / 256
                unsafe {
                    upload_region_at(sd, 0l, addr<uint8 const?>(src[0]), e_bytes)
                    // the class kernel: src@0, outq@1, outs@2
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
                // plain requant of f32 rows — amax is order-free, the pack deterministic: exact
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(src, 0, nelem, true, q_ref, s_ref)
                let bad = mismatch_qbytes(q_cls, q_ref, 0) + mismatch_exact(s_cls, s_ref)
                t |> success(bad == 0, "q8k requant matches the CPU oracle bit-exact ({bad} elements off)")
                delete src
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_q8_gemv_pair(t0 : T?) {
    t0 |> run("q8/q8n GEMV class kernels == the CPU oracle") <| @(t : T?) {
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
            for (arm in range(2)) {   // 0 = q8, 1 = q8n (plain sdot4 — no mode gate needed)
                let q8n = arm == 1
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
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nb)
                    regs[r * 2 + 1] = 0u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
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
                    let begin = VkCommandBufferBeginInfo()
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
                }
                // oracle: per-block int dot x per-block scales, sequential block order
                var y_ref : array<float>
                y_ref |> resize(rows)
                for (r in range(nreg)) {
                    for (row in range(d)) {
                        let w0 = r * d * nb + row * nb
                        var acc = 0.0
                        for (b in range(nb)) {
                            var idot = 0
                            if (q8n) {
                                let bb = (w0 + b) * 34
                                for (j in range(32)) {
                                    let xw = xqh[b * 8 + j / 4]
                                    let xq8 = int(xw << uint((3 - j % 4) * 8)) >> 24
                                    idot += byte_s8(wqh, bb + 2 + j) * xq8
                                }
                                acc += half_bytes(wqh, bb) * float(idot) * xsh[b]
                            } else {
                                for (j in range(8)) {
                                    idot += sdot4(wqh[(w0 + b) * 8 + j], xqh[b * 8 + j])
                                }
                                let ws16 = ((w0 + b) % 2 == 0 ? unpackHalf2x16(wsh[(w0 + b) / 2]).x
                                    : unpackHalf2x16(wsh[(w0 + b) / 2]).y)
                                acc += float(idot) * ws16 * xsh[b]
                            }
                        }
                        y_ref[r * d + row] = acc
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "{q8n ? "q8n" : "q8"} GEMV matches the CPU oracle ({bad} of {rows} rows off)")
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_cvt_family(t0 : T?) {
    t0 |> run("dn requant + cvt/act-f16 class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_dn_rq()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            // dn_requant: src floats at an offset -> Q8_0 words + scales
            {
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
                let qd2 = make_device_buf(q_bytes)
                let sd2 = make_device_buf(s_bytes)
                var host = make_host_buf(q_bytes + s_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nblk * 8)
                s_cls |> resize(nblk)
                let wgs = (nblk * 8 + 255) / 256
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    var sc = set_rq_cls(fixed_array(srcd, qd2, sd2),
                        fixed_array(src_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                // plain Q8_0 requant with an input offset — exact
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(srch, inbase, nblk * 32, false, q_ref, s_ref)
                let bad = mismatch_qbytes(q_cls, q_ref, 0) + mismatch_exact(s_cls, s_ref)
                t |> success(bad == 0, "dn_requant matches the CPU oracle bit-exact ({bad} off)")
                delete srch
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
            }
            // the cm2-chain converters: xf16 dequant, act-f16, f16 convert (mode-free classes)
            {
                verify(ensure_cls_xf16() && ensure_actf16_cls() && ensure_f16cvt_cls(),
                    "the cvt class rails must engage on a live device")
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
                let hd2 = make_device_buf(h_bytes)
                var host = make_host_buf(h_bytes, true, [cached = true])
                var h_cls : array<uint>
                h_cls |> resize(nelem / 2)
                for (arm in range(3)) {   // 0 = xf16 dequant, 1 = act-f16 (gelu), 2 = f16 convert
                    unsafe {
                        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                        upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), g_bytes)
                        upload_region_at(ud, 0l, addr<uint8 const?>(uh[0]), g_bytes)
                        let wgs = arm == 0 ? (nwords + 255) / 256 : (nelem / 4 + 255) / 256
                        var sc : VkDescriptorSet
                        var raw2 = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
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
                    // oracle in f32, compared through the f16 halves the kernels emit (the gelu
                    // arm carries transcendental ulps — approx absorbs both)
                    var got : array<float>
                    var want : array<float>
                    got |> resize(nelem)
                    want |> resize(nelem)
                    for (i in range(nelem)) {
                        got[i] = half_at(h_cls, i)
                        if (arm == 0) {
                            let w = i / 4
                            let xq8 = int(xqh[w] << uint((3 - i % 4) * 8)) >> 24
                            want[i] = float(xq8) * xsh[w / 8]
                        } elif (arm == 1) {
                            want[i] = act_mul_ref(gh[i], uh[i], true)
                        } else {
                            want[i] = uh[i]
                        }
                    }
                    let bad = mismatch_approx(got, want)
                    t |> success(bad == 0, "cvt arm {arm} matches the CPU oracle ({bad} of {nelem} off)")
                    delete got
                    delete want
                }
                delete xqh
                delete xsh
                delete gh
                delete uh
                delete h_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_rope_b_pair(t0 : T?) {
    t0 |> run("batch rope + fused qkn-rope class kernels == the CPU oracle") <| @(t : T?) {
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
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_cls : array<float>
                var m_cls : array<float>
                q_cls |> resize(npos * qd)
                m_cls |> resize(2 * mirn)
                let wgs = (npos * ppp + 255) / 256
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    var sc = set_rope_kv_b_cls(fixed_array(qd2, kvdv, km2, vm2, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes),
                        fixed_array(1u, 16u, 2u, 4u, 8u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                        ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                        pos0 = uint(pos0), voff = uint(voff))
                    enc_rope_kv_b_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                // oracle: per-position NEOX pairs off per-position cos rows; k roped + v raw at
                // absolute mirror rows — only the written region compares
                var q_ref : array<float>
                var mk_ref : array<float>
                var mv_ref : array<float>
                q_ref |> resize(npos * qd)
                mk_ref |> resize(mirn)
                mv_ref |> resize(mirn)
                for (i in range(npos * qd)) {
                    q_ref[i] = q0h[i]
                }
                for (p in range(npos)) {
                    for (lp in range(ppp)) {
                        let is_k = lp >= kpair0
                        let pr = is_k ? lp - kpair0 : lp
                        let hh = pr / half
                        let j = pr % half
                        let fcr = csh[p * hs + j]
                        let fci = csh[p * hs + half + j]
                        if (is_k) {
                            let kb = p * kvd + hh * hs
                            let a0 = kvh[kb + j]
                            let a1 = kvh[kb + j + half]
                            let mo = layerbase + (pos0 + p) * kvd + hh * hs
                            mk_ref[mo + j] = a0 * fcr - a1 * fci
                            mk_ref[mo + j + half] = a0 * fci + a1 * fcr
                            mv_ref[mo + j] = kvh[voff + kb + j]
                            mv_ref[mo + j + half] = kvh[voff + kb + j + half]
                        } else {
                            let qb = p * qd + hh * hs
                            let a0 = q0h[qb + j]
                            let a1 = q0h[qb + j + half]
                            q_ref[qb + j] = a0 * fcr - a1 * fci
                            q_ref[qb + j + half] = a0 * fci + a1 * fcr
                        }
                    }
                }
                var bad = mismatch_approx(q_cls, q_ref)
                let w0 = layerbase + pos0 * kvd
                for (i in range(npos * kvd)) {
                    if (!approx(m_cls[w0 + i], mk_ref[w0 + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn + w0 + i], mv_ref[w0 + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "rope_kv_store_b matches the CPU oracle ({bad} off)")
                delete q0h
                delete kvh
                delete csh
                delete q_ref
                delete mk_ref
                delete mv_ref
                delete q_cls
                delete m_cls
            }
            // --- qkn_rope_kv: qk-rms + rope + kv-store in one dispatch, one wg per head-row ---
            {
                let neox = 1
                // ONE mirror row both planes: the class twin composes it as layerbase + pos*kvd
                // (the serving ladder always wrote equal K/V rows; the independent-rows freedom
                // was the old kernel's meta accident, not a contract)
                let krow = 192
                let vrow = 192
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
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let wnd = make_device_buf(wn_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_cls : array<float>
                var m_cls : array<float>
                q_cls |> resize(qd)
                m_cls |> resize(2 * mirn2)
                let wgs = nh + nkvh
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmq = reinterpret<TokMeta?>(tokb.mapped)
                    tmq.pos = 0u   // mirror row = layerbase alone
                    var sc = set_qkn_rope_cls(fixed_array(qd2, kvdv, km2, vm2, csd, wnd, tokb.buf),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, wn_bytes, 16l),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 8u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), nh = uint(nh),
                        layerbase = uint(krow), qwoff = 0u,
                        kwoff = uint(hs), eps = eps)
                    enc_qkn_rope_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                // oracle: per-head rms-norm with its weight row, then NEOX rope off the normed
                // row; k lands at the mirror row, v copies raw
                var q_ref : array<float>
                var mk_ref : array<float>
                var mv_ref : array<float>
                q_ref |> resize(qd)
                mk_ref |> resize(mirn2)
                mv_ref |> resize(mirn2)
                for (wid in range(nh + nkvh)) {
                    let is_k = wid >= nh
                    let hh = is_k ? wid - nh : wid
                    let sbase = is_k ? qd + hh * hs : hh * hs
                    var ss = 0.0
                    for (i in range(hs)) {
                        let v = is_k ? kvh[sbase + i] : q0h[sbase + i]
                        ss += v * v
                    }
                    let inv = 1.0 / sqrt(ss / float(hs) + eps)
                    var nrow : array<float>
                    nrow |> resize(hs)
                    for (i in range(hs)) {
                        let v = is_k ? kvh[sbase + i] : q0h[sbase + i]
                        nrow[i] = wnh[(is_k ? hs : 0) + i] * (v * inv)
                    }
                    for (j in range(half)) {
                        let fcr = csh[j]
                        let fci = csh[half + j]
                        let r0 = nrow[j] * fcr - nrow[j + half] * fci
                        let r1 = nrow[j] * fci + nrow[j + half] * fcr
                        if (is_k) {
                            mk_ref[krow + hh * hs + j] = r0
                            mk_ref[krow + hh * hs + j + half] = r1
                        } else {
                            q_ref[hh * hs + j] = r0
                            q_ref[hh * hs + j + half] = r1
                        }
                    }
                    if (is_k) {
                        for (i in range(hs)) {
                            mv_ref[krow + hh * hs + i] = kvh[qd + kvd + hh * hs + i]
                        }
                    }
                    delete nrow
                }
                var bad = mismatch_approx(q_cls, q_ref)
                for (i in range(nkvh * hs)) {
                    if (!approx(m_cls[krow + i], mk_ref[krow + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn2 + vrow + i], mv_ref[krow + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "qkn_rope_kv matches the CPU oracle ({bad} off)")
                delete q0h
                delete kvh
                delete csh
                delete wnh
                delete q_ref
                delete mk_ref
                delete mv_ref
                delete q_cls
                delete m_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_at_prep(t0 : T?) {
    t0 |> run("attention prep class (q+k collapse) == the CPU oracle (gated+qknorm+partial rope)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_at_prep_cls()) {
                feint("no Vulkan device — skipping\n")
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
            let qp2 = make_device_buf(qp_bytes)
            let g2 = make_device_buf(qp_bytes)
            let kp2 = make_device_buf(kp_bytes)
            var host = make_host_buf(2l * qp_bytes + kp_bytes, true, [cached = true])
            var o_cls : array<float>
            o_cls |> resize(2 * rows * qd + (w0 + rows) * kvd)
            unsafe {
                upload_region_at(srcqd, 0l, addr<uint8 const?>(srcqh[0]), srcq_bytes)
                upload_region_at(srckd, 0l, addr<uint8 const?>(srckh[0]), srck_bytes)
                upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                // the class collapse: ONE kernel, two dispatches with different push values
                var scq = set_at_prep_cls(fixed_array(srcqd, qp2, g2, smd),
                    fixed_array(srcq_bytes, qp_bytes, qp_bytes, sm_bytes),
                    fixed_array(8u, 1u, 2u, 4u))
                var sck = set_at_prep_cls(fixed_array(srckd, kp2, g2, smd),
                    fixed_array(srck_bytes, kp_bytes, qp_bytes, sm_bytes),
                    fixed_array(8u, 16u, 0u, 4u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
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
                vhz_dep(raw, h, 1u | 2u | 16u, 0u, true)
                cmd_copy_whole(raw, qp2, host.buf, qp_bytes)
                cmd_copy_range(raw, g2, 0l, host.buf, qp_bytes, qp_bytes)
                cmd_copy_range(raw, kp2, 0l, host.buf, 2l * qp_bytes, kp_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(o_cls[0]), host.mapped, 2l * qp_bytes + kp_bytes)
            }
            // oracle: deinterleave (gated q), per-head rms with the pass's weight row, partial
            // NEOX rope over dims [0, 2*half), panel rows at obase + p
            var o_ref : array<float>
            o_ref |> resize(2 * rows * qd + (w0 + rows) * kvd)
            let kb0 = 2 * rows * qd + w0 * kvd
            for (p in range(rows)) {
                for (leg in range(2)) {
                    let isq = leg == 0
                    let d = isq ? qd : kvd
                    let nhh = isq ? nh : nkvh
                    let rms_off = isq ? rmsq_off : rmsk_off
                    var rrow : array<float>
                    rrow |> resize(d)
                    for (i in range(d)) {
                        let hh = i / hs
                        let j = i % hs
                        if (isq) {
                            rrow[i] = srcqh[p * 2 * qd + hh * 2 * hs + j]
                            o_ref[rows * qd + p * qd + i] = srcqh[p * 2 * qd + hh * 2 * hs + hs + j]
                        } else {
                            rrow[i] = srckh[p * kvd + i]
                        }
                    }
                    for (hh in range(nhh)) {
                        var ss = 0.0
                        for (j in range(hs)) {
                            ss += rrow[hh * hs + j] * rrow[hh * hs + j]
                        }
                        let inv = 1.0 / sqrt(ss / float(hs) + eps)
                        for (j in range(hs)) {
                            rrow[hh * hs + j] = smh[rms_off + j] * (rrow[hh * hs + j] * inv)
                        }
                    }
                    for (i in range(d)) {
                        let hh = i / hs
                        let j = i % hs
                        var outv = rrow[i]
                        if (j < half) {
                            outv = rrow[i] * smh[cos_off + p * half + j] - rrow[hh * hs + j + half] * smh[sin_off + p * half + j]
                        } elif (j < 2 * half) {
                            let jj = j - half
                            outv = rrow[hh * hs + jj] * smh[sin_off + p * half + jj] + rrow[i] * smh[cos_off + p * half + jj]
                        }
                        if (isq) {
                            o_ref[p * qd + i] = outv
                        } else {
                            o_ref[kb0 + p * kvd + i] = outv
                        }
                    }
                    delete rrow
                }
            }
            var bad = 0
            for (i in range(2 * rows * qd)) {   // q panel + gate stash
                if (!approx(o_cls[i], o_ref[i])) {
                    bad++
                }
            }
            for (i in range(rows * kvd)) {   // k panel: only the written absolute rows
                if (!approx(o_cls[kb0 + i], o_ref[kb0 + i])) {
                    bad++
                }
            }
            t |> success(bad == 0, "at_prep class collapse matches the CPU oracle ({bad} off)")
            delete srcqh
            delete srckh
            delete smh
            delete o_ref
            delete o_cls
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}

[test]
def test_vkd_dn_family(t0 : T?) {
    t0 |> run("deltanet class kernels == the CPU oracle (conv, fused step, 2-phase scan)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_dn_conv_cls()) {
                feint("no Vulkan device — skipping\n")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                feint("dn chain declined on this device (shared-memory cap) — skipping\n")
                return
            }
            verify(ensure_dn_step_cls() && ensure_dn_scan_p1_cls() && ensure_dn_scan_p2_cls(),
                "the dn class rails must engage on a device over the shared cap")
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
                let od2 = make_device_buf(out_bytes)
                var host = make_host_buf(out_bytes, true, [cached = true])
                var o_cls : array<float>
                o_cls |> resize(npos * cd)
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var sc = set_dn_conv_cls(fixed_array(srcd, od2, smd),
                        fixed_array(src_bytes, out_bytes, sm_bytes),
                        fixed_array(8u, 1u, 4u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
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
                // oracle: depthwise conv (history rows below position 0) + SiLU, then per-ds-block
                // L2 norm over the q/k regions with qscale on the q blocks
                var o_ref : array<float>
                o_ref |> resize(npos * cd)
                for (p in range(npos)) {
                    for (ch in range(cd)) {
                        var acc = 0.0
                        for (k in range(dconv)) {
                            let sp = p + k - taps
                            var xv = 0.0
                            if (sp >= 0) {
                                xv = srch[sp * cd + ch]
                            } else {
                                xv = smh[hist_off + ch * taps + p + k]   // hist_mode 1
                            }
                            acc += xv * smh[taps_off + ch * dconv + k]
                        }
                        o_ref[p * cd + ch] = acc / (1.0 + exp(-acc))
                    }
                    for (b in range(2 * kd / ds)) {
                        var sum = 0.0
                        for (j in range(ds)) {
                            sum += o_ref[p * cd + b * ds + j] * o_ref[p * cd + b * ds + j]
                        }
                        let s = (1.0 / max(sqrt(sum), eps)) * (b < kd / ds ? qscale : 1.0)
                        for (j in range(ds)) {
                            o_ref[p * cd + b * ds + j] *= s
                        }
                    }
                }
                let bad = mismatch_approx(o_cls, o_ref)
                t |> success(bad == 0, "dn_conv matches the CPU oracle ({bad} of {npos * cd} off)")
                delete srch
                delete smh
                delete o_ref
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
                let st2 = make_device_buf(st_bytes)
                let sm2 = make_device_buf(sm_bytes)
                let scd2 = make_device_buf(sc_bytes)
                let qd2 = make_device_buf(q_bytes)
                var host = make_host_buf(st_bytes + sm_bytes + sc_bytes + q_bytes, true, [cached = true])
                var st_cls : array<float>
                var sm_cls : array<float>
                var sc_cls : array<float>
                var q_cls : array<uint>
                st_cls |> resize(nvh * ds * ds)
                sm_cls |> resize(sm_n)
                sc_cls |> resize(nblk)
                q_cls |> resize(nblk * 8)
                unsafe {
                    upload_region_at(projd, 0l, addr<uint8 const?>(projh[0]), proj_bytes)
                    upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                    upload_region_at(sm2, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmd = reinterpret<TokMeta?>(tokb.mapped)
                    tmd.parity = uint(par)
                    var sc = set_dn_step_cls(fixed_array(projd, st2, sm2, scd2, qd2, tokb.buf),
                        fixed_array(proj_bytes, st_bytes, sm_bytes, sc_bytes, q_bytes, 16l),
                        fixed_array(16u, 2u, 8u, 1u, 4u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh),
                        ds = uint(ds), dconv = uint(dconv), beta_off = uint(beta_off),
                        g_off = uint(g_off), taps_off = uint(taps_off),
                        wnorm_off = uint(wnorm_off), a_off = uint(a_off),
                        dt_off = uint(dt_off), hist_off = uint(hist_off),
                        eps = eps, qscale = qscale)
                    enc_dn_step_cls(raw, h, sc, pc, int64(nvh))
                    vhz_dep(raw, h, 1u | 2u | 4u | 8u, 0u, true)
                    cmd_copy_whole(raw, st2, host.buf, st_bytes)
                    cmd_copy_range(raw, sm2, 0l, host.buf, st_bytes, sm_bytes)
                    cmd_copy_range(raw, scd2, 0l, host.buf, st_bytes + sm_bytes, sc_bytes)
                    cmd_copy_range(raw, qd2, 0l, host.buf, st_bytes + sm_bytes + sc_bytes, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(st_cls[0]), host.mapped, st_bytes)
                    memcpy(addr<void?>(sm_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes)), sm_bytes)
                    memcpy(addr<void?>(sc_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes + sm_bytes)), sc_bytes)
                    memcpy(addr<void?>(q_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes + sm_bytes + sc_bytes)), q_bytes)
                }
                // oracle: the whole fused chain — ring conv + silu, q/k L2, chunk-length-1 delta
                // rule in place, gated out-norm, the head's Q8_0 blocks
                var sm_ref : array<float>
                sm_ref := smh
                let rrb = hist_off + par * cd * taps
                let wrb = hist_off + (1 - par) * cd * taps
                for (ch in range(cd)) {
                    for (k in range(taps - 1)) {
                        sm_ref[wrb + ch * taps + k] = smh[rrb + ch * taps + k + 1]
                    }
                    sm_ref[wrb + ch * taps + taps - 1] = projh[ch]
                }
                var st_ref : array<float>
                st_ref := sth
                var out_ref : array<float>
                out_ref |> resize(nvh * ds)
                for (hh in range(nvh)) {
                    let kh = hh % nkh
                    var dq : array<float>
                    var dk : array<float>
                    var dv : array<float>
                    dq |> resize(ds)
                    dk |> resize(ds)
                    dv |> resize(ds)
                    for (sl in range(3)) {
                        for (j in range(ds)) {
                            let ch = (sl == 0 ? kh * ds : (sl == 1 ? kd + kh * ds : 2 * kd + hh * ds)) + j
                            var acc = projh[ch] * smh[taps_off + ch * dconv + taps]
                            for (k in range(taps)) {
                                acc += smh[rrb + ch * taps + k] * smh[taps_off + ch * dconv + k]
                            }
                            let sv = acc / (1.0 + exp(-acc))
                            if (sl == 0) {
                                dq[j] = sv
                            } elif (sl == 1) {
                                dk[j] = sv
                            } else {
                                dv[j] = sv
                            }
                        }
                    }
                    var sumq = 0.0
                    var sumk = 0.0
                    for (j in range(ds)) {
                        sumq += dq[j] * dq[j]
                        sumk += dk[j] * dk[j]
                    }
                    let sq = (1.0 / max(sqrt(sumq), eps)) * qscale
                    let sk = 1.0 / max(sqrt(sumk), eps)
                    let graw = smh[g_off + hh] + smh[dt_off + hh]
                    let eg = exp(smh[a_off + hh] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
                    let bt = 1.0 / (1.0 + exp(-smh[beta_off + hh]))
                    var qk = 0.0
                    for (j in range(ds)) {
                        dq[j] *= sq
                        dk[j] *= sk
                        qk += dq[j] * dk[j]
                    }
                    let sbase = hh * ds * ds
                    var dvn : array<float>
                    var dout : array<float>
                    dvn |> resize(ds)
                    dout |> resize(ds)
                    for (j in range(ds)) {
                        var ks = 0.0
                        var qs = 0.0
                        for (i in range(ds)) {
                            ks += dk[i] * sth[sbase + i * ds + j]
                            qs += dq[i] * sth[sbase + i * ds + j]
                        }
                        dvn[j] = bt * (dv[j] - eg * ks)
                        dout[j] = eg * qs + qk * dvn[j]
                    }
                    for (e in range(ds * ds)) {
                        st_ref[sbase + e] = sth[sbase + e] * eg + dk[e / ds] * dvn[e % ds]
                    }
                    var sum = 0.0
                    for (j in range(ds)) {
                        sum += dout[j] * dout[j]
                    }
                    let inv = 1.0 / sqrt(sum / float(ds) + eps)
                    for (j in range(ds)) {
                        let zv = projh[cd + hh * ds + j]
                        out_ref[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * (zv / (1.0 + exp(-zv)))
                    }
                    delete dq
                    delete dk
                    delete dv
                    delete dvn
                    delete dout
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(out_ref, 0, nvh * ds, false, q_ref, s_ref)
                let bad = (mismatch_approx(st_cls, st_ref) + mismatch_approx(sm_cls, sm_ref)
                    + mismatch_approx(sc_cls, s_ref) + mismatch_qbytes(q_cls, q_ref, 1))
                t |> success(bad == 0, "dn_step_fused matches the CPU oracle ({bad} elements off)")
                delete projh
                delete sth
                delete smh
                delete sm_ref
                delete st_ref
                delete out_ref
                delete q_ref
                delete s_ref
                delete st_cls
                delete sm_cls
                delete sc_cls
                delete q_cls
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
                let st2 = make_device_buf(st_bytes)
                let smd = make_device_buf(sm_bytes)
                let sl2 = make_device_buf(slab_bytes)
                let o_bytes = int64(npos * di) * 4l
                var host = make_host_buf(o_bytes + st_bytes, true, [cached = true])
                var o_cls : array<float>
                let cmp_n = npos * di + nvh * ds * ds
                o_cls |> resize(cmp_n)
                unsafe {
                    upload_region_at(convd, 0l, addr<uint8 const?>(convh[0]), conv_bytes)
                    upload_region_at(zd, 0l, addr<uint8 const?>(zh[0]), z_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                    var sc1 = set_dn_scan_p1_cls(fixed_array(convd, smd, sl2),
                        fixed_array(conv_bytes, sm_bytes, slab_bytes),
                        fixed_array(1u, 8u, 4u))
                    var sc2 = set_dn_scan_p2_cls(fixed_array(zd, st2, sl2, smd),
                        fixed_array(z_bytes, st_bytes, slab_bytes, sm_bytes),
                        fixed_array(16u, 2u, 4u, 8u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc1 = DnScanP1Args(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                        nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                        beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                        dt_off = uint(dt_off), slab_stride = uint(slab_stride))
                    enc_dn_scan_p1_cls(raw, h, sc1, pc1, int64(nvh * nch))
                    var pc2 = DnScanP2Args(npos = uint(npos), di = uint(di), ds = uint(ds),
                        o_off = uint(o_off), wnorm_off = uint(wnorm_off),
                        slab_stride = uint(slab_stride), st_off = uint(st_off), eps = eps)
                    enc_dn_scan_p2_cls(raw, h, sc2, pc2, int64(nvh))
                    vhz_dep(raw, h, 2u | 4u, 0u, true)
                    cmd_copy_range(raw, sl2, int64(o_off) * 4l, host.buf, 0l, o_bytes)
                    cmd_copy_range(raw, st2, 0l, host.buf, o_bytes, st_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(o_cls[0]), host.mapped, o_bytes + st_bytes)
                }
                // oracle: the SEQUENTIAL delta rule — the chunked WY form equals it in exact
                // arithmetic, so the drift is pure reassociation; peak-floored bars absorb it
                var o_ref : array<float>
                o_ref |> resize(cmp_n)
                for (hh in range(nvh)) {
                    let kh = hh % nkh
                    var st_h : array<float>
                    st_h |> resize(ds * ds)
                    for (e in range(ds * ds)) {
                        st_h[e] = sth[hh * ds * ds + e]
                    }
                    var gcum = 0.0
                    for (tt in range(npos)) {
                        let crow = tt * cd
                        let graw = smh[g_off + tt * nvh + hh] + smh[dt_off + hh]
                        let gt = smh[a_off + hh] * (graw > 20.0 ? graw : log(1.0 + exp(graw)))
                        gcum += gt
                        let eg = exp(gt)
                        let bt = 1.0 / (1.0 + exp(-smh[beta_off + tt * nvh + hh]))
                        var dvn : array<float>
                        dvn |> resize(ds)
                        for (j in range(ds)) {
                            var ks = 0.0
                            for (i in range(ds)) {
                                ks += convh[crow + kd + kh * ds + i] * st_h[i * ds + j]
                            }
                            dvn[j] = bt * (convh[crow + 2 * kd + hh * ds + j] - eg * ks)
                        }
                        for (e in range(ds * ds)) {
                            st_h[e] = st_h[e] * eg + convh[crow + kd + kh * ds + e / ds] * dvn[e % ds]
                        }
                        var orow : array<float>
                        orow |> resize(ds)
                        var sum = 0.0
                        for (j in range(ds)) {
                            var qs = 0.0
                            for (i in range(ds)) {
                                qs += convh[crow + kh * ds + i] * st_h[i * ds + j]
                            }
                            orow[j] = qs
                            sum += qs * qs
                        }
                        let inv = 1.0 / sqrt(sum / float(ds) + eps)
                        for (j in range(ds)) {
                            let zv = zh[tt * di + hh * ds + j]
                            o_ref[tt * di + hh * ds + j] = orow[j] * inv * smh[wnorm_off + j] * (zv / (1.0 + exp(-zv)))
                        }
                        delete dvn
                        delete orow
                    }
                    for (e in range(ds * ds)) {
                        o_ref[npos * di + hh * ds * ds + e] = st_h[e]
                    }
                    delete st_h
                }
                let bad = mismatch_bars(o_cls, o_ref, 1e-2, 1e-2 * max_abs(o_ref))
                t |> success(bad == 0, "dn_scan p1+p2 matches the CPU oracle ({bad} of {cmp_n} off)")
                delete convh
                delete zh
                delete sth
                delete smh
                delete o_ref
                delete o_cls
            }
        } else {
            feint("dasVulkan not present — skipping\n")
        }
    }
}
