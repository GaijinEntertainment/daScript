options gen2
options stack = 524288

require dastest/testing_boost public
require dasllama/dasllama_math
require ?vulkan dasllama/dasllama_math_vulkan  // nolint:STYLE030 — the MoE GPU tier under test; the test installs its hooks explicitly (no env gate)
require llvm/daslib/f16_cvt
require math

// The cm2 (NV_cooperative_matrix2) twin vs a CPU reference — fmt-6 q8n stacks (gguf-native 34B
// interleaved blocks, NO scale plane) through the same public hook surface as the q8 suite:
// decode rides the q8n native-block GEMV (Q8_0 activations, requant mid-chain — int-exact like
// the q8 arms), prefill rides the decode-in-load tensor-tile twin with f16 activations and the
// no-requant f16 mid-chain. Weight and activation scales are powers of two, so every f16 tile
// element is EXACT — the only prefill drift is the f16 accumulate, judged at CM2_TOL. Runs in
// its own process because the suite pin is per-device: this file pins mode 4 (cm2) where
// test_vulkan_tier pins the sdot4 reference mode. Without dasVulkan, a Vulkan device, or the
// cm2 extension, every check feints cleanly.

[init]
def private pin_cm2_gemm_mode {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_force_coopmat_mode(4)   // downgrades to mm on a non-cm2 device; the probe below gates
    }
}

let N = 96l         // model dim (x row length), %32; the down GEMM's d = 96 = 64 + 32 tail tile
let NFE = 64l       // expert FFN dim — gate/up d = exactly one 64-wide weight tile
let NE = 3l         // experts in each synthetic stack
let WOFF1 = 1000000l   // gate / up / down stack base element offsets (arbitrary distinct keys)
let WOFF3 = 2000000l
let WOFF2 = 3000000l
let WOFFQ1 = 4000000l  // the mixed arm's fmt-0 gate/up twins
let WOFFQ3 = 5000000l
let SWOFF = 20000000l  // the streamed fixture's layer base
let REL_TOL = 1e-3     // int-exact arms (decode GEMV chain)
let CM2_TOL = 2e-2     // f16-accumulate arms (the twin's tensor tiles)

def private pow2_scale(i : int64) : float {
    let p = i % 5l
    return p == 0l ? 1.0 : (p == 1l ? 0.5 : (p == 2l ? 2.0 : (p == 3l ? 0.25 : 0.125)))
}

// small pow2 scales keep |gate| in the sane silu regime AND make every dequantized f16 exact
def private pow2_small(i : int64) : float {
    let p = i % 3l
    return p == 0l ? 0.00390625 : (p == 1l ? 0.001953125 : 0.0078125)   // 2^-8 / 2^-9 / 2^-7
}

def private fill_stack(var wq : array<int8>; var ws : array<float>; rows, n : int64; seed : int64; small_scales : bool) {
    wq |> resize(int(rows * n))
    ws |> resize(int(rows * (n / 32l)))
    for (i in range(int(rows * n))) {
        wq[i] = int8((int64(i) * 7l + seed) % 255l - 127l)
    }
    for (i in range(int(rows * (n / 32l)))) {
        ws[i] = small_scales ? pow2_small(int64(i) + seed) : pow2_scale(int64(i) + seed)
    }
}

// the q8n wire format: per row per 32-block — [f16 d LE][32 int8 quants] = 34 bytes, ONE plane
def private pack_q8n(wq : array<int8>; ws : array<float>; rows, n : int64; var wqb : array<uint8>) {
    let nb = n / 32l
    wqb |> resize(int(rows * nb * 34l))
    for (r in range64(rows)) {
        for (b in range64(nb)) {
            let o = int((r * nb + b) * 34l)
            let h = f32_to_f16(ws[int(r * nb + b)])
            wqb[o] = uint8(h & 0xFFu)
            wqb[o + 1] = uint8((h >> 8u) & 0xFFu)
            for (j in range(32)) {
                wqb[o + 2 + j] = uint8(wq[int(r * n + b * 32l) + j])
            }
        }
    }
}

// the fmt-0 upload wire format (the mixed arm's gate/up planes)
def private pack_upload(wq : array<int8>; ws : array<float>; var wqb : array<uint8>; var wsb : array<uint8>) {
    wqb |> resize(length(wq))
    for (i in range(length(wq))) {
        wqb[i] = uint8(wq[i])
    }
    wsb |> resize(length(ws) * 2)
    for (i in range(length(ws))) {
        let h = f32_to_f16(ws[i])
        wsb[i * 2] = uint8(h & 0xFFu)
        wsb[i * 2 + 1] = uint8((h >> 8u) & 0xFFu)
    }
}

def private fill_acts(var xq : array<int8>; var xs : array<float>; nrows : int64) {
    xq |> resize(int(nrows * N))
    xs |> resize(int(nrows * (N / 32l)))
    for (i in range(int(nrows * N))) {
        xq[i] = int8((int64(i) * 11l + 3l) % 255l - 127l)
    }
    for (i in range(int(nrows * (N / 32l)))) {
        xs[i] = pow2_small(int64(i) + 2l)
    }
}

def private make_offs(experts : array<int64>; row0s : array<int64>; cnts : array<int64>; woff : int64; ege : int64) : array<int64> {
    var offs : array<int64>
    offs |> reserve(length(experts) * 3)
    for (r in range(length(experts))) {
        offs |> push(woff + experts[r] * ege)   // nolint:STYLE012 — triple-stride append
        offs |> push(row0s[r])
        offs |> push(cnts[r])
    }
    return <- offs
}

// The CPU reference of the FFN contract. requant=true models the decode chain (Q8_0 actrq
// between the halves, the q8 suite's formula); requant=false models the cm2 prefill chain —
// act(g)*u feeds the down GEMM as floats (the GPU rounds them to f16; CM2_TOL covers it).
def private ref_ffn(offs1, offs3, offs2 : array<int64>; nreg : int;
                    wq1 : array<int8>; ws1 : array<float>; wq3 : array<int8>; ws3 : array<float>;
                    wq2 : array<int8>; ws2 : array<float>;
                    xq : array<int8>; xs : array<float>; nrows : int64; requant : bool;
                    w1off, w3off, w2off : int64; var gout : array<float>) {
    gout |> resize(int(nrows * N))
    let nb = int(N / 32l)
    let nbh = int(NFE / 32l)
    var gate : array<float>
    var up : array<float>
    var h : array<float>
    var hq : array<int>
    var hs : array<float>
    gate |> resize(int(NFE))
    up |> resize(int(NFE))
    h |> resize(int(NFE))
    hq |> resize(int(NFE))
    hs |> resize(nbh)
    for (r in range(nreg)) {
        let rb1 = int((offs1[r * 3] - w1off) / N)      // first gate-plane row of this region's expert
        let rb3 = int((offs3[r * 3] - w3off) / N)
        let rb2 = int((offs2[r * 3] - w2off) / NFE)
        let row0 = int(offs1[r * 3 + 1])
        let cnt = int(offs1[r * 3 + 2])
        for (rw in range(row0, row0 + cnt)) {
            for (i in range(int(NFE))) {
                var ag = 0.0
                var au = 0.0
                for (b in range(nb)) {
                    var dg = 0
                    var du = 0
                    for (j in range(32)) {
                        let xv = int(xq[rw * int(N) + b * 32 + j])
                        dg += int(wq1[(rb1 + i) * int(N) + b * 32 + j]) * xv
                        du += int(wq3[(rb3 + i) * int(N) + b * 32 + j]) * xv
                    }
                    let xscl = xs[rw * nb + b]
                    ag += float(dg) * ws1[(rb1 + i) * nb + b] * xscl
                    au += float(du) * ws3[(rb3 + i) * nb + b] * xscl
                }
                gate[i] = ag
                up[i] = au
            }
            for (i in range(int(NFE))) {
                h[i] = (gate[i] / (1.0 + exp(-gate[i]))) * up[i]
            }
            if (requant) {
                for (b in range(nbh)) {
                    var amax = 0.0
                    for (j in range(32)) {
                        amax = max(amax, abs(h[b * 32 + j]))
                    }
                    let d = amax / 127.0
                    let id = d != 0.0 ? 1.0 / d : 0.0
                    hs[b] = d
                    for (j in range(32)) {
                        hq[b * 32 + j] = int(round(h[b * 32 + j] * id))
                    }
                }
                for (o in range(int(N))) {
                    var y = 0.0
                    for (b in range(nbh)) {
                        var dd = 0
                        for (j in range(32)) {
                            dd += int(wq2[(rb2 + o) * int(NFE) + b * 32 + j]) * hq[b * 32 + j]
                        }
                        y += float(dd) * ws2[(rb2 + o) * nbh + b] * hs[b]
                    }
                    gout[rw * int(N) + o] = y
                }
            } else {
                for (o in range(int(N))) {
                    var y = 0.0
                    for (i in range(int(NFE))) {
                        y += float(wq2[(rb2 + o) * int(NFE) + i]) * ws2[(rb2 + o) * nbh + i / 32] * h[i]
                    }
                    gout[rw * int(N) + o] = y
                }
            }
        }
    }
}

def private compare_gout(gpu, refv : array<float>) : tuple<maxdiff : float; maxref : float> {
    var md = 0.0
    var mr = 0.0
    for (i in range(length(refv))) {
        md = max(md, abs(gpu[i] - refv[i]))
        mr = max(mr, abs(refv[i]))
    }
    return (maxdiff = md, maxref = mr)
}

[test]
def test_vulkan_moe_ffn_cm2(t : T?) {
    t |> run("vulkan cm2 twin == CPU reference (q8n decode GEMV + decode-in-load prefill)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            if (!moe_gpu_expert_q8n()) {
                feint("no Vulkan device or no NV_cooperative_matrix2; cm2 twin untestable here\n")
                return
            }
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_stack(wq1, ws1, NE * NFE, N, 0l, true)
            fill_stack(wq3, ws3, NE * NFE, N, 5l, true)
            fill_stack(wq2, ws2, NE * N, NFE, 9l, true)
            var wqb : array<uint8>
            var no_ws : array<uint8>   // q8n stacks upload with an EMPTY scale plane
            pack_q8n(wq1, ws1, NE * NFE, N, wqb)
            let up_ok = moe_gpu_upload_stack(wqb, no_ws, WOFF1, N, NE * NFE, 6)
            if (!up_ok) {
                feint("q8n upload refused (probe/budget); skipping\n")
                return
            }
            pack_q8n(wq3, ws3, NE * NFE, N, wqb)
            t |> success(moe_gpu_upload_stack(wqb, no_ws, WOFF3, N, NE * NFE, 6), "q8n up stack uploaded")
            pack_q8n(wq2, ws2, NE * N, NFE, wqb)
            t |> success(moe_gpu_upload_stack(wqb, no_ws, WOFF2, NFE, NE * N, 6), "q8n down stack uploaded")
            let ege1 = NFE * N
            let ege2 = N * NFE

            // decode arm: 2 rows, 1 per region — the q8n native-block GEMV + the Q8_0 requant
            // mid-chain; int-exact like the q8 suite's decode arm
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 2l)
            var offs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF1, ege1)
            var offs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF3, ege1)
            var offs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF2, ege2)
            var gout : array<float>
            gout |> resize(int(2l * N))
            matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, N, NFE, 2l, 6, 6, 6, false)
            var refv : array<float>
            ref_ffn(offs1, offs3, offs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 2l, true,
                WOFF1, WOFF3, WOFF2, refv)
            var c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "decode arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "q8n decode GEMV within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            // batch arm: 40 rows, 2 regions, deliberately unpadded — underfilled 128-wide token
            // tiles + the 96 = 64+32 weight-tail tile on the down twin (clamp/discard coverage)
            fill_acts(xq, xs, 40l)
            var boffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF1, ege1)
            var boffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF3, ege1)
            var boffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF2, ege2)
            gout |> resize(int(40l * N))
            matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, N, NFE, 40l, 6, 6, 6, false)
            ref_ffn(boffs1, boffs3, boffs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 40l, false,
                WOFF1, WOFF3, WOFF2, refv)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "batch arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(CM2_TOL) * c.maxref,
                "cm2 prefill batch within tolerance (maxdiff {c.maxdiff} vs {float(CM2_TOL) * c.maxref} bar, maxref {c.maxref})")

            // multi-tile arm: a 130-row region spans two token tiles (128 + 2-row tail)
            fill_acts(xq, xs, 200l)
            var moffs1 <- make_offs([0l, 1l], [0l, 130l], [130l, 70l], WOFF1, ege1)
            var moffs3 <- make_offs([0l, 1l], [0l, 130l], [130l, 70l], WOFF3, ege1)
            var moffs2 <- make_offs([0l, 1l], [0l, 130l], [130l, 70l], WOFF2, ege2)
            gout |> resize(int(200l * N))
            matmul_moe_gpu_ffn(gout, moffs1, moffs3, moffs2, 2l, xq, xs, N, NFE, 200l, 6, 6, 6, false)
            ref_ffn(moffs1, moffs3, moffs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 200l, false,
                WOFF1, WOFF3, WOFF2, refv)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "multi-tile arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(CM2_TOL) * c.maxref,
                "cm2 multi-token-tile batch within tolerance (maxdiff {c.maxdiff} vs {float(CM2_TOL) * c.maxref} bar, maxref {c.maxref})")

            // combined arm: the routed weighted-sum reduce composed onto the twin chain, judged
            // against the same combine of the GPU's OWN per-row gout (isolates the combine op)
            var cw : array<float>
            var cinv : array<uint>
            cw |> resize(40)
            cinv |> resize(40)
            for (g in range(40)) {
                cw[g] = 0.5 + 0.01 * float(g)
                cinv[g] = uint((g * 7 + 3) % 40)
            }
            fill_acts(xq, xs, 40l)
            gout |> resize(int(40l * N))
            matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, N, NFE, 40l, 6, 6, 6, false)
            var comb : array<float>
            comb |> resize(int(10l * N))
            matmul_moe_gpu_ffn_combined(comb, boffs1, boffs3, boffs2, 2l, xq, xs, cw, cinv, N, NFE, 40l, 10l, 6, 6, 6, false)
            var combref : array<float>
            combref |> resize(int(10l * N))
            for (p in range(10)) {
                for (i in range(int(N))) {
                    var acc = 0.0
                    for (j in range(4)) {
                        acc += cw[p * 4 + j] * gout[int(cinv[p * 4 + j]) * int(N) + i]
                    }
                    combref[p * int(N) + i] = acc
                }
            }
            let cc = compare_gout(comb, combref)
            t |> success(cc.maxref > 0.0, "combined arm produced non-trivial outputs")
            t |> success(cc.maxdiff <= float(REL_TOL) * cc.maxref,
                "cm2 combined arm within tolerance (maxdiff {cc.maxdiff} vs {float(REL_TOL) * cc.maxref} bar, maxref {cc.maxref})")

            // mixed arm: fmt-0 gate/up (the mm/sdot4 rail) + q8n down — the actf16 mid-step
            // keyed on the DOWN stack composes with a non-cm2 front half (the gemma triple shape)
            var wsb : array<uint8>
            pack_upload(wq1, ws1, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, WOFFQ1, N, NE * NFE, 0), "mixed-arm q8 gate uploaded")
            pack_upload(wq3, ws3, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, WOFFQ3, N, NE * NFE, 0), "mixed-arm q8 up uploaded")
            var xoffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFFQ1, ege1)
            var xoffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFFQ3, ege1)
            fill_acts(xq, xs, 40l)
            gout |> resize(int(40l * N))
            matmul_moe_gpu_ffn(gout, xoffs1, xoffs3, boffs2, 2l, xq, xs, N, NFE, 40l, 0, 0, 6, false)
            ref_ffn(xoffs1, xoffs3, boffs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 40l, false,
                WOFFQ1, WOFFQ3, WOFF2, refv)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "mixed arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(CM2_TOL) * c.maxref,
                "q8-gate/up + q8n-down mixed chain within tolerance (maxdiff {c.maxdiff} vs {float(CM2_TOL) * c.maxref} bar, maxref {c.maxref})")

            delete cw
            delete cinv
            delete comb
            delete combref
            delete xoffs1
            delete xoffs3
        } else {
            feint("dasVulkan not installed; vulkan cm2 tier untestable here\n")
        }
    }
}

// streamed arm: one q8n layer group in pinned mirrors (NO scale plane — the ws-skip rails),
// dispatched through the slot rail at batch rows vs the same no-requant reference
[test]
def test_vulkan_moe_ffn_cm2_streamed(t : T?) {
    t |> run("vulkan cm2 twin, streamed q8n mirrors == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            if (!moe_gpu_expert_q8n()) {
                feint("no Vulkan device or no NV_cooperative_matrix2; cm2 twin untestable here\n")
                return
            }
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_stack(wq1, ws1, NE * NFE, N, 21l, true)
            fill_stack(wq3, ws3, NE * NFE, N, 26l, true)
            fill_stack(wq2, ws2, NE * N, NFE, 30l, true)
            var wqb : array<uint8>
            var no_ws : array<uint8>
            pack_q8n(wq1, ws1, NE * NFE, N, wqb)
            let up_ok = moe_gpu_upload_stack(wqb, no_ws, SWOFF, N, NE * NFE, 6, [stream = true])
            if (!up_ok) {
                feint("q8n stream upload refused (probe/budget); skipping\n")
                return
            }
            pack_q8n(wq3, ws3, NE * NFE, N, wqb)
            t |> success(moe_gpu_upload_stack(wqb, no_ws, SWOFF + 1000000l, N, NE * NFE, 6, [stream = true]), "streamed q8n up mirrored")
            pack_q8n(wq2, ws2, NE * N, NFE, wqb)
            t |> success(moe_gpu_upload_stack(wqb, no_ws, SWOFF + 2000000l, NFE, NE * N, 6, [stream = true]), "streamed q8n down mirrored")
            let ege1 = NFE * N
            let ege2 = N * NFE
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 40l)
            var offs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], SWOFF, ege1)
            var offs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], SWOFF + 1000000l, ege1)
            var offs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], SWOFF + 2000000l, ege2)
            var gout : array<float>
            gout |> resize(int(40l * N))
            matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, N, NFE, 40l, 6, 6, 6, false)
            var refv : array<float>
            ref_ffn(offs1, offs3, offs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 40l, false,
                SWOFF, SWOFF + 1000000l, SWOFF + 2000000l, refv)
            let c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "streamed arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(CM2_TOL) * c.maxref,
                "streamed q8n batch within tolerance (maxdiff {c.maxdiff} vs {float(CM2_TOL) * c.maxref} bar, maxref {c.maxref})")
        } else {
            feint("dasVulkan not installed; vulkan cm2 tier untestable here\n")
        }
    }
}

// arena arm: the resident driver's fmt-6 rails at the seam — a q8n plane placed with an EMPTY
// scale plane into the fmt-6 arena (the ws-floor guards), read back by the q8n GEMV over the
// arena block index. Q8_0 activations, so the chain is int-exact like the decode arm.
[test]
def test_vulkan_arena_cm2(t : T?) {
    t |> run("vulkan cm2 arena: q8n place + arena GEMV == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            if (!moe_gpu_expert_q8n()) {
                feint("no Vulkan device or no NV_cooperative_matrix2; cm2 arena untestable here\n")
                return
            }
            var wq : array<int8>
            var ws : array<float>
            fill_stack(wq, ws, NFE, N, 13l, false)
            var wqb : array<uint8>
            var no_ws : array<uint8>
            pack_q8n(wq, ws, NFE, N, wqb)
            if (!vk_arena_reserve(6, NFE * (N / 32l))) {
                feint("fmt-6 arena reserve refused (no device); skipping\n")
                return
            }
            let blk = vk_arena_place(wqb, no_ws, N, NFE, 6)
            t |> success(blk >= 0l, "q8n arena placement accepted an empty scale plane")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 1l)
            var y : array<float>
            y |> resize(int(NFE))
            vk_arena_gemv(y, blk, N, NFE, 6, xq, xs)
            let nb = int(N / 32l)
            var md = 0.0
            var mr = 0.0
            for (r in range(int(NFE))) {
                var acc = 0.0
                for (b in range(nb)) {
                    var dd = 0
                    for (j in range(32)) {
                        dd += int(wq[r * int(N) + b * 32 + j]) * int(xq[b * 32 + j])
                    }
                    acc += float(dd) * ws[r * nb + b] * xs[b]
                }
                md = max(md, abs(y[r] - acc))
                mr = max(mr, abs(acc))
            }
            t |> success(mr > 0.0, "arena arm produced non-trivial outputs")
            t |> success(md <= float(REL_TOL) * mr,
                "q8n arena GEMV within tolerance (maxdiff {md} vs {float(REL_TOL) * mr} bar, maxref {mr})")
        } else {
            feint("dasVulkan not installed; vulkan cm2 tier untestable here\n")
        }
    }
}
