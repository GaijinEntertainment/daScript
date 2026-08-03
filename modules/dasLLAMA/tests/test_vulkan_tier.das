options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, hook install, dispatch, CPU-reference compare
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_math
require ?vulkan dasllama/dasllama_math_vulkan  // the MoE GPU tier under test; the test installs its hooks explicitly (no env gate)
require ?vulkan dasllama/dasllama_vulkan_seams  // the single-op seams (vk_add_rms / rope_kv / decode_attn) on the class rail
require llvm/daslib/f16_cvt
require math

// The Vulkan MoE GPU tier (dasllama_math_vulkan) vs a hand-rolled CPU reference of the same
// fused FFN contract: region-triple gate+up GEMMs -> silu(g)*u -> requant -> down GEMM,
// on synthetic stacks uploaded through the public hook surface (install_moe_gpu_tier /
// moe_gpu_upload_stack / matmul_moe_gpu_ffn). Q8 arms run both dispatch shapes: decode GEMV
// (nrows < 32) and the tiled prefill batch (nrows >= 32, deliberately unpadded). The kq arms
// (native K-quant stacks — k4 gate/up with a k5 and a k6 down, the 35B Q4_K_M shapes) drive
// both shapes too: the per-format GEMV kernels + the Q8_K requant mid-chain at decode row
// counts, the per-format batch tiles at 40 rows. Weight scales are powers of two
// (f16-exact), so the only drift left is the GPU's exp() in silu and round-halfway splits in
// the requant — the compare is relative-tolerance, not bit-exact.
// The gate/up scale planes are SMALL pow2 (2^-7..2^-9) on purpose: they keep |gate| in the
// sane silu regime (~±25). Full-size scales drive |gate| to ~3e5, where CPU exp() and GPU
// exp() saturate DIFFERENTLY (probe-verified 60%+ output divergence) — a regime no real
// model's activations enter. The gelu arm stays untested until a gelu-MoE q8 fixture exists.
// Without dasVulkan (or with no Vulkan device) every check feints cleanly.

// The CPU-reference arms' tolerances were derived against the sdot4 reference kernels — pin
// that mode ahead of the router's fastest-path default (mm) so the oracles stay deterministic.
// The coopmat kernels' numeric envelope is validated by the drift-class model gates, not here.
[init]
def private pin_reference_gemm_mode {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vk_force_coopmat_mode(0)
    }
}

let N = 96l         // model dim (x row length), %32
let NFE = 64l       // expert FFN dim, %32
let NE = 3l         // experts in each synthetic stack
let WOFF1 = 1000000l   // gate / up / down stack base element offsets (arbitrary distinct keys —
let WOFF3 = 2000000l   // the tier resolves regions by [base, base+elems) containment)
let WOFF2 = 3000000l
let REL_TOL = 1e-3
// the q8 arena is process-global and shared by every arena arm — reserve once, generously
let ARENA_BLOCKS = 4096l

def private pow2_scale(i : int64) : float {
    let p = i % 5l
    return p == 0l ? 1.0 : (p == 1l ? 0.5 : (p == 2l ? 2.0 : (p == 3l ? 0.25 : 0.125)))
}

// the small set for gate/up/x scale planes — keeps |gate| in the sane silu regime (see header)
def private pow2_small(i : int64) : float {
    let p = i % 3l
    return p == 0l ? 0.00390625 : (p == 1l ? 0.001953125 : 0.0078125)   // 2^-8 / 2^-9 / 2^-7
}

def private fill_stack(var wq : array<int8>; var ws : array<float>; rows, n : int64; seed : int64; small_scales : bool) {
    wq |> resize(rows * n)
    ws |> resize(rows * (n / 32l))
    for (i in range64(rows * n)) {
        wq[i] = int8((i * 7l + seed) % 255l - 127l)
    }
    for (i in range64(rows * (n / 32l))) {
        ws[i] = small_scales ? pow2_small(i + seed) : pow2_scale(i + seed)
    }
}

// the upload wire format: row-major int8 quants as bytes + f16 halfword scales as bytes (LE)
def private pack_upload(wq : array<int8>; ws : array<float>; var wqb : array<uint8>; var wsb : array<uint8>) {
    wqb |> resize(length(wq))
    for (q, b in wq, wqb) {
        b = uint8(q)
    }
    wsb |> resize(length(ws) * 2)
    for (i in range(length(ws))) {
        let h = f32_to_f16(ws[i])
        wsb[i * 2] = uint8(h & 0xFFu)
        wsb[i * 2 + 1] = uint8((h >> 8u) & 0xFFu)
    }
}

def private fill_acts(var xq : array<int8>; var xs : array<float>; nrows : int64) {
    xq |> resize(nrows * N)
    xs |> resize(nrows * (N / 32l))
    for (i in range64(nrows * N)) {
        xq[i] = int8((i * 11l + 3l) % 255l - 127l)
    }
    for (i in range64(nrows * (N / 32l))) {
        xs[i] = pow2_small(i + 2l)
    }
}

// one region-triple list over experts[], rows partitioning [0, nrows)
def private make_offs(experts : array<int64>; row0s : array<int64>; cnts : array<int64>; woff : int64; ege : int64) : array<int64> {
    var offs : array<int64>
    offs |> reserve(length(experts) * 3)
    for (e, r0, cn in experts, row0s, cnts) {
        offs |> push(woff + e * ege)
        offs |> push(r0)
        offs |> push(cn)
    }
    return <- offs
}

// the CPU reference of the tier's fused-FFN contract (same formulas as the kernels' CPU mirrors)
def private ref_ffn(offs1, offs3, offs2 : array<int64>; nreg : int;
                    wq1 : array<int8>; ws1 : array<float>; wq3 : array<int8>; ws3 : array<float>;
                    wq2 : array<int8>; ws2 : array<float>;
                    xq : array<int8>; xs : array<float>; nrows : int64; var gout : array<float>) {
    gout |> resize(nrows * N)
    let nb = int(N / 32l)
    let nbh = int(NFE / 32l)
    let nn = int(N)
    let nfe = int(NFE)
    var gate : array<float>
    var up : array<float>
    var h : array<float>
    var hq : array<int>
    var hs : array<float>
    gate |> resize(NFE)
    up |> resize(NFE)
    h |> resize(NFE)
    hq |> resize(NFE)
    hs |> resize(nbh)
    for (r in range(nreg)) {
        let rb1 = int((offs1[r * 3] - WOFF1) / N)      // first gate-plane row of this region's expert
        let rb3 = int((offs3[r * 3] - WOFF3) / N)
        let rb2 = int((offs2[r * 3] - WOFF2) / NFE)
        let row0 = int(offs1[r * 3 + 1])
        let cnt = int(offs1[r * 3 + 2])
        for (rw in range(row0, row0 + cnt)) {
            for (i in range(nfe)) {
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
            for (i in range(nfe)) {
                h[i] = (gate[i] / (1.0 + exp(-gate[i]))) * up[i]
            }
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
            for (o in range(nn)) {
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
        }
    }
}

// max |gpu - ref| over the touched rows, and the max |ref| it is judged against
def private compare_gout(gpu, refv : array<float>) : tuple<maxdiff : float; maxref : float> {
    var md = 0.0
    var mr = 0.0
    for (gv, rv in gpu, refv) {
        md = max(md, abs(gv - rv))
        mr = max(mr, abs(rv))
    }
    return (maxdiff = md, maxref = mr)
}

[test]
def test_vulkan_moe_ffn_vs_ref(t : T?) {
    t |> run("vulkan MoE FFN tier == CPU reference (decode GEMV + prefill batch arms)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            // synthetic stacks: gate/up are (NE*NFE) x N, down is (NE*N) x NFE
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_stack(wq1, ws1, NE * NFE, N, 0l, true)
            fill_stack(wq3, ws3, NE * NFE, N, 5l, true)
            fill_stack(wq2, ws2, NE * N, NFE, 9l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wq1, ws1, wqb, wsb)
            var up_ok = moe_gpu_upload_stack(wqb, wsb, WOFF1, N, NE * NFE, 0)
            if (!up_ok) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            pack_upload(wq3, ws3, wqb, wsb)
            up_ok = moe_gpu_upload_stack(wqb, wsb, WOFF3, N, NE * NFE, 0)
            t |> success(up_ok, "up stack uploaded")
            pack_upload(wq2, ws2, wqb, wsb)
            up_ok = moe_gpu_upload_stack(wqb, wsb, WOFF2, NFE, NE * N, 0)
            t |> success(up_ok, "down stack uploaded")
            let ege1 = NFE * N     // one expert's element span in the gate/up planes
            let ege2 = N * NFE     // ... and the down plane

            // decode arm: 2 rows, 1 row per region (the engine's k=2 GEMV shape)
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 2l)
            var offs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF1, ege1)
            var offs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF3, ege1)
            var offs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], WOFF2, ege2)
            var gout : array<float>
            gout |> resize(int(2l * N))
            matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, N, NFE, 2l, 0, 0, 0, false)
            var refv : array<float>
            ref_ffn(offs1, offs3, offs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 2l, refv)
            var c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "decode arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "decode GEMV arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            // batch arm: 40 rows (>= the 32-row tile, deliberately unpadded), 2 regions
            fill_acts(xq, xs, 40l)
            var boffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF1, ege1)
            var boffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF3, ege1)
            var boffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF2, ege2)
            gout |> resize(int(40l * N))
            matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, N, NFE, 40l, 0, 0, 0, false)
            ref_ffn(boffs1, boffs3, boffs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 40l, refv)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "batch arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "prefill batch arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            // combined arm: same batch fixture, the routed weighted-sum reduce on the device —
            // an npos=10 x k=4 grid over the 40 bucket rows via a bijective inv map, judged
            // against the same combine of the GPU's OWN per-row gout (isolates the combine op)
            var cw : array<float>
            var cinv : array<uint>
            cw |> resize(40)
            cinv |> resize(40)
            for (g in range(40)) {
                cw[g] = 0.5 + 0.01 * float(g)
                cinv[g] = uint((g * 7 + 3) % 40)
            }
            var comb : array<float>
            comb |> resize(int(10l * N))
            matmul_moe_gpu_ffn_combined(comb, boffs1, boffs3, boffs2, 2l, xq, xs, cw, cinv, N, NFE, 40l, 10l, 0, 0, 0, false)
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
            var cc = compare_gout(comb, combref)
            t |> success(cc.maxref > 0.0, "combined arm produced non-trivial outputs")
            t |> success(cc.maxdiff <= float(REL_TOL) * cc.maxref,
                "combined arm within tolerance (maxdiff {cc.maxdiff} vs {float(REL_TOL) * cc.maxref} bar, maxref {cc.maxref})")

            // two-chunk combined arm: 8200 rows overflow the 8192-row chunk cap, so the combine
            // accumulates across two dispatches AND region B straddles the chunk boundary; a
            // position's slots land in both chunks (the cross-chunk r0 > 0 path)
            let CNK = 8200l
            fill_acts(xq, xs, CNK)
            var coffs1 <- make_offs([0l, 2l], [0l, 4096l], [4096l, CNK - 4096l], WOFF1, ege1)
            var coffs3 <- make_offs([0l, 2l], [0l, 4096l], [4096l, CNK - 4096l], WOFF3, ege1)
            var coffs2 <- make_offs([0l, 2l], [0l, 4096l], [4096l, CNK - 4096l], WOFF2, ege2)
            gout |> resize(int(CNK * N))
            matmul_moe_gpu_ffn(gout, coffs1, coffs3, coffs2, 2l, xq, xs, N, NFE, CNK, 0, 0, 0, false)
            cw |> resize(int(CNK))
            cinv |> resize(int(CNK))
            for (g in range(int(CNK))) {
                cw[g] = 0.5 + 0.0001 * float(g)
                cinv[g] = uint((g * 7 + 3) % int(CNK))
            }
            let CNP = CNK / 4l
            comb |> resize(int(CNP * N))
            matmul_moe_gpu_ffn_combined(comb, coffs1, coffs3, coffs2, 2l, xq, xs, cw, cinv, N, NFE, CNK, CNP, 0, 0, 0, false)
            combref |> resize(int(CNP * N))
            for (p in range(int(CNP))) {
                for (i in range(int(N))) {
                    var acc = 0.0
                    for (j in range(4)) {
                        acc += cw[p * 4 + j] * gout[int(cinv[p * 4 + j]) * int(N) + i]
                    }
                    combref[p * int(N) + i] = acc
                }
            }
            cc = compare_gout(comb, combref)
            t |> success(cc.maxref > 0.0, "two-chunk combined arm produced non-trivial outputs")
            t |> success(cc.maxdiff <= float(REL_TOL) * cc.maxref,
                "two-chunk combined arm within tolerance (maxdiff {cc.maxdiff} vs {float(REL_TOL) * cc.maxref} bar, maxref {cc.maxref})")
            delete cw
            delete cinv
            delete comb
            delete combref
            delete coffs1
            delete coffs3
            delete coffs2
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== streamed-prefill arm (pinned mirrors + slot rotation) =====

// three q8 "layers" uploaded stream=true in loader order (descending layers => group 0 is the
// runtime-LAST layer), dispatched in runtime order 2,1,0 then 2 again — covers the cold copy,
// the unfenced prefetch hit, slot rotation, and an eviction reload, each vs the CPU reference
let SWOFF = 20000000l
let SSTRIDE = 100000l   // per-stack offset stride inside the streamed fixture (%32 == 0)

[test]
def test_vulkan_moe_ffn_streamed(t : T?) {
    t |> run("vulkan MoE streamed prefill (pinned mirrors, 2 slots) == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            let ege1 = NFE * N
            let ege2 = N * NFE
            var wq1 : array<array<int8>>
            var ws1 : array<array<float>>
            var wq3 : array<array<int8>>
            var ws3 : array<array<float>>
            var wq2 : array<array<int8>>
            var ws2 : array<array<float>>
            wq1 |> resize(3)
            ws1 |> resize(3)
            wq3 |> resize(3)
            ws3 |> resize(3)
            wq2 |> resize(3)
            ws2 |> resize(3)
            var wqb : array<uint8>
            var wsb : array<uint8>
            var streamed_ok = true
            for (g in range(3)) {
                fill_stack(wq1[g], ws1[g], NE * NFE, N, int64(g) * 31l + 2l, true)
                fill_stack(wq3[g], ws3[g], NE * NFE, N, int64(g) * 31l + 7l, true)
                fill_stack(wq2[g], ws2[g], NE * N, NFE, int64(g) * 31l + 11l, false)
                let base = SWOFF + int64(g) * 3l * SSTRIDE
                pack_upload(wq1[g], ws1[g], wqb, wsb)
                let ok1 = moe_gpu_upload_stack(wqb, wsb, base, N, NE * NFE, 0, [stream = true])
                pack_upload(wq3[g], ws3[g], wqb, wsb)
                let ok3 = moe_gpu_upload_stack(wqb, wsb, base + SSTRIDE, N, NE * NFE, 0, [stream = true])
                pack_upload(wq2[g], ws2[g], wqb, wsb)
                let ok2 = moe_gpu_upload_stack(wqb, wsb, base + 2l * SSTRIDE, NFE, NE * N, 0, [stream = true])
                streamed_ok = streamed_ok && ok1 && ok3 && ok2
            }
            if (!streamed_ok) {
                feint("no Vulkan device (or probe refused the stream uploads); skipping\n")
                return
            }
            var xq : array<int8>
            var xs : array<float>
            var gout : array<float>
            var refv : array<float>
            for (pass_i in [2, 1, 0, 2]) {   // runtime order + an eviction reload of group 2
                let g = pass_i
                let base = SWOFF + int64(g) * 3l * SSTRIDE
                fill_acts(xq, xs, 40l)
                var offs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], base, ege1)
                var offs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], base + SSTRIDE, ege1)
                var offs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], base + 2l * SSTRIDE, ege2)
                gout |> resize(int(40l * N))
                matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, N, NFE, 40l, 0, 0, 0, false)
                var roffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF1, ege1)
                var roffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF3, ege1)
                var roffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], WOFF2, ege2)
                ref_ffn(roffs1, roffs3, roffs2, 2, wq1[g], ws1[g], wq3[g], ws3[g], wq2[g], ws2[g], xq, xs, 40l, refv)
                let c = compare_gout(gout, refv)
                t |> success(c.maxref > 0.0, "streamed group {g} produced non-trivial outputs")
                t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                    "streamed group {g} within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")
                delete offs1
                delete offs3
                delete offs2
                delete roffs1
                delete roffs3
                delete roffs2
            }
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
            delete wqb
            delete wsb
            delete xq
            delete xs
            delete gout
            delete refv
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== heat-cache arm (slot pools, eviction, async split dispatch) =====

// one q8 "layer" mirrored stream=true; advise pins a subset, a mixed top-k query splits, the
// hits ride the async begin/join chain vs the CPU reference; a re-advise then flips the pinned
// set (eviction + slice re-upload into freed slots) and the new hits re-verify
let HWOFF = 40000000l
let HSTRIDE = 100000l
let HLAYER = 7l

def private heat_hit_offs(o : array<int64>; nh : int64) : array<int64> {
    var offs : array<int64>
    offs |> reserve(int(nh) * 3)
    for (i in range64(nh)) {
        offs |> push(o[i])
        offs |> push(i)
        offs |> push(1l)
    }
    return <- offs
}

[test]
def test_vulkan_moe_heat_cache(t : T?) {
    t |> run("vulkan heat-pinned expert cache == CPU reference (pins, eviction, split hits)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            set_moe_gpu_heat_hooks(@@vk_moe_heat_query, @@vk_moe_heat_advise, @@vk_moe_ffn_begin, @@vk_moe_ffn_join)
            var want = GpuTierWant()
            want.heat = 2l   // two slots: the eviction/reuse choreography below assumes C = 2
            set_gpu_tier_want(want)
            let ege1 = NFE * N
            let ege2 = N * NFE
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_stack(wq1, ws1, NE * NFE, N, 51l, true)
            fill_stack(wq3, ws3, NE * NFE, N, 57l, true)
            fill_stack(wq2, ws2, NE * N, NFE, 61l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wq1, ws1, wqb, wsb)
            let ok1 = moe_gpu_upload_stack(wqb, wsb, HWOFF, N, NE * NFE, 0, [stream = true])
            pack_upload(wq3, ws3, wqb, wsb)
            let ok3 = moe_gpu_upload_stack(wqb, wsb, HWOFF + HSTRIDE, N, NE * NFE, 0, [stream = true])
            pack_upload(wq2, ws2, wqb, wsb)
            let ok2 = moe_gpu_upload_stack(wqb, wsb, HWOFF + 2l * HSTRIDE, NFE, NE * N, 0, [stream = true])
            if (!(ok1 && ok3 && ok2)) {
                feint("no Vulkan device (or probe refused the stream uploads); skipping\n")
                return
            }
            // pin experts {0, 2}; route the top-k [1, 0, 2] — expect 2 hits, expert 1 missing
            var ids <- [0l, 2l]
            moe_gpu_heat_advise(HLAYER, HWOFF, HWOFF + HSTRIDE, HWOFF + 2l * HSTRIDE, 0, 0, 0, N, NFE, ids, 2l)
            var idx <- [1l, 0l, 2l]
            var hit : array<int8>
            var o1 : array<int64>
            var o3 : array<int64>
            var o2 : array<int64>
            hit |> resize(3)
            o1 |> resize(3)
            o3 |> resize(3)
            o2 |> resize(3)
            var nh = matmul_moe_gpu_heat_query(HLAYER, idx, 3l, hit, o1, o3, o2)
            t |> equal(2l, nh, "two of three routed experts hit the cache")
            t |> success(hit[0] == int8(0) && hit[1] == int8(1) && hit[2] == int8(1), "hit mask matches the pinned set")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 2l)
            var hoffs1 <- heat_hit_offs(o1, nh)
            var hoffs3 <- heat_hit_offs(o3, nh)
            var hoffs2 <- heat_hit_offs(o2, nh)
            matmul_moe_gpu_ffn_begin(hoffs1, hoffs3, hoffs2, nh, xq, xs, N, NFE, nh, 0, 0, 0, false)
            // the CPU reference runs BETWEEN begin and join — the overlap the split arm relies on
            var roffs1 <- make_offs([0l, 2l], [0l, 1l], [1l, 1l], WOFF1, ege1)
            var roffs3 <- make_offs([0l, 2l], [0l, 1l], [1l, 1l], WOFF3, ege1)
            var roffs2 <- make_offs([0l, 2l], [0l, 1l], [1l, 1l], WOFF2, ege2)
            var refv : array<float>
            ref_ffn(roffs1, roffs3, roffs2, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 2l, refv)
            var gout : array<float>
            gout |> resize(int(2l * N))
            matmul_moe_gpu_ffn_join(gout)
            var c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "hit dispatch produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "async hit dispatch within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")
            // flip the pinned set to {1}: evicts both slots, uploads expert 1 into a freed one
            var ids2 <- [1l]
            moe_gpu_heat_advise(HLAYER, HWOFF, HWOFF + HSTRIDE, HWOFF + 2l * HSTRIDE, 0, 0, 0, N, NFE, ids2, 1l)
            nh = matmul_moe_gpu_heat_query(HLAYER, idx, 3l, hit, o1, o3, o2)
            t |> equal(1l, nh, "re-pin leaves one hit")
            t |> success(hit[0] == int8(1) && hit[1] == int8(0) && hit[2] == int8(0), "hit mask follows the re-pin")
            fill_acts(xq, xs, 1l)
            var eoffs1 <- heat_hit_offs(o1, nh)
            var eoffs3 <- heat_hit_offs(o3, nh)
            var eoffs2 <- heat_hit_offs(o2, nh)
            matmul_moe_gpu_ffn_begin(eoffs1, eoffs3, eoffs2, nh, xq, xs, N, NFE, nh, 0, 0, 0, false)
            var poffs1 <- make_offs([1l], [0l], [1l], WOFF1, ege1)
            var poffs3 <- make_offs([1l], [0l], [1l], WOFF3, ege1)
            var poffs2 <- make_offs([1l], [0l], [1l], WOFF2, ege2)
            ref_ffn(poffs1, poffs3, poffs2, 1, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 1l, refv)
            gout |> resize(int(N))
            matmul_moe_gpu_ffn_join(gout)
            c = compare_gout(gout, refv)
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "post-eviction hit dispatch within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")
            delete ids
            delete ids2
            delete idx
            delete hit
            delete o1
            delete o3
            delete o2
            delete xq
            delete xs
            delete hoffs1
            delete hoffs3
            delete hoffs2
            delete eoffs1
            delete eoffs3
            delete eoffs2
            delete roffs1
            delete roffs3
            delete roffs2
            delete poffs1
            delete poffs3
            delete poffs2
            delete refv
            delete gout
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
            delete wqb
            delete wsb
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== native K-quant arms =====

let KN = 512l        // kq model dim (TWO superblocks per row — nsb = 1 hides stride bugs)
let KNFE = 512l      // kq expert FFN dim (two superblocks; the requant/down walk crosses one)
let KWOFF1 = 5000000l
let KWOFF3 = 6000000l
let KWOFF2 = 7000000l

// tiny pow2 set for the kq gate/up d planes — with 6-bit sc (<=63) and 4/5-bit quants the
// per-weight magnitude lands where |gate| stays in the sane silu regime (see header)
def private pow2_tiny(i : int64) : float {
    let p = i % 3l
    return p == 0l ? 0.000244140625 : (p == 1l ? 0.0001220703125 : 0.00048828125)   // 2^-12/-13/-11
}

def private lcg_byte(var st : uint64&) : uint8 {
    st = st * 6364136223846793005ul + 1442695040888963407ul
    return uint8((st >> 33ul) & 255ul)
}

// synthesize one kq stack directly in the tier's DEVICE layout: quant payload (any bytes are
// valid nibble/qh payloads) + 20B scale rows (f16 d/dmin from the pow2 sets, 6-bit sc/mn,
// signed i8 sub-scales, or q40's 8 per-block f16 d)
def private fill_kq_stack(fmt : int; rows, n : int64; seed : int64; tiny_d : bool;
                          var wqb : array<uint8>; var wsb : array<uint8>) {
    let nsb = n / 256l
    let qsb = fmt == 2 ? 160l : (fmt == 3 ? 192l : 128l)
    wqb |> resize(rows * nsb * qsb)
    wsb |> resize(rows * nsb * 20l)
    var st = uint64(seed) * 2654435761ul + 12345ul
    for (c in wqb) {
        c = lcg_byte(st)
    }
    var sb = 0l
    while (sb < rows * nsb) {
        let so = int(sb * 20l)
        if (fmt == 3) {
            for (i in range(16)) {   // signed i8 sub-scales in a modest range
                wsb[so + i] = uint8(int(lcg_byte(st)) % 63 - 31)
            }
            let h = f32_to_f16(tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            wsb[so + 16] = uint8(h & 0xFFu)
            wsb[so + 17] = uint8((h >> 8u) & 0xFFu)
            wsb[so + 18] = uint8(0)
            wsb[so + 19] = uint8(0)
        } elif (fmt == 4) {
            for (blk in range(8)) {   // one f16 d per 32-block, words 0..3; 16..19 pad
                let h = f32_to_f16(tiny_d ? pow2_tiny(sb + seed + int64(blk)) : pow2_small(sb + seed + int64(blk)))
                wsb[so + blk * 2] = uint8(h & 0xFFu)
                wsb[so + blk * 2 + 1] = uint8((h >> 8u) & 0xFFu)
            }
            for (i in range(16, 20)) {
                wsb[so + i] = uint8(0)
            }
        } else {
            let hd = f32_to_f16(tiny_d ? pow2_tiny(sb + seed) : pow2_small(sb + seed))
            let hm = f32_to_f16(tiny_d ? pow2_tiny(sb + seed + 1l) : pow2_small(sb + seed + 1l))
            wsb[so] = uint8(hd & 0xFFu)
            wsb[so + 1] = uint8((hd >> 8u) & 0xFFu)
            wsb[so + 2] = uint8(hm & 0xFFu)
            wsb[so + 3] = uint8((hm >> 8u) & 0xFFu)
            for (i in range(16)) {   // 8 sc + 8 mn, 6-bit
                wsb[so + 4 + i] = uint8(int(lcg_byte(st)) & 63)
            }
        }
        sb++
    }
}

// dequantize one device-layout stack to floats — the same extraction the GPU kernels do,
// validated bit-exact against the plane-dequant oracles by the kq gather probe
def private dequant_kq_stack(fmt : int; wqb : array<uint8>; wsb : array<uint8>; rows, n : int64;
                             var w : array<float>) {
    let nsb = n / 256l
    let qsb = fmt == 2 ? 160l : (fmt == 3 ? 192l : 128l)
    w |> resize(rows * n)
    for (rr in range64(rows)) {
        for (sbi in range64(nsb)) {
            let qo = (rr * nsb + sbi) * qsb
            let so = (rr * nsb + sbi) * 20l
            let wo = rr * n + sbi * 256l
            if (fmt == 4) {
                for (blk in range64(8l)) {
                    let d = f16_to_f32(uint(wsb[so + blk * 2l]) | (uint(wsb[so + blk * 2l + 1l]) << 8u))
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        w[wo + blk * 32l + m] = d * float((b & 15) - 8)
                        w[wo + blk * 32l + 16l + m] = d * float((b >> 4) - 8)
                    }
                }
            } elif (fmt == 3) {
                let d = f16_to_f32(uint(wsb[so + 16l]) | (uint(wsb[so + 17l]) << 8u))
                for (blk in range64(8l)) {
                    let shift = int(2l * (blk % 4l))
                    let vlo = int(wsb[so + 2l * blk])
                    let vhi = int(wsb[so + 2l * blk + 1l])
                    let slo = float(vlo >= 128 ? vlo - 256 : vlo)
                    let shi = float(vhi >= 128 ? vhi - 256 : vhi)
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        let hb = int(wqb[qo + 128l + (blk / 4l) * 32l + m])
                        let hb2 = int(wqb[qo + 128l + (blk / 4l) * 32l + 16l + m])
                        let qlo = ((b & 15) | (((hb >> shift) & 3) << 4)) - 32
                        let qhi = ((b >> 4) | (((hb2 >> shift) & 3) << 4)) - 32
                        w[wo + blk * 32l + m] = d * slo * float(qlo)
                        w[wo + blk * 32l + 16l + m] = d * shi * float(qhi)
                    }
                }
            } else {
                let d = f16_to_f32(uint(wsb[so]) | (uint(wsb[so + 1l]) << 8u))
                let dmin = f16_to_f32(uint(wsb[so + 2l]) | (uint(wsb[so + 3l]) << 8u))
                for (blk in range64(8l)) {
                    let d1 = d * float(uint(wsb[so + 4l + blk]))
                    let m1 = dmin * float(uint(wsb[so + 12l + blk]))
                    for (m in range64(16l)) {
                        let b = int(wqb[qo + blk * 16l + m])
                        var qlo = b & 15
                        var qhi = b >> 4
                        if (fmt == 2) {
                            let hb = int(wqb[qo + 128l + blk * 4l + m / 4l])
                            let tt = int(m % 4l)
                            qlo |= ((hb >> tt) & 1) << 4
                            qhi |= ((hb >> (4 + tt)) & 1) << 4
                        }
                        w[wo + blk * 32l + m] = d1 * float(qlo) - m1
                        w[wo + blk * 32l + 16l + m] = d1 * float(qhi) - m1
                    }
                }
            }
        }
    }
}

def private fill_acts_kq(var xq : array<int8>; var xs : array<float>; nrows : int64) {
    xq |> resize(nrows * KN)
    xs |> resize(nrows * (KN / 256l))
    for (i in range64(nrows * KN)) {
        xq[i] = int8((i * 13l + 5l) % 255l - 127l)
    }
    for (i in range64(nrows * (KN / 256l))) {
        xs[i] = pow2_small(i + 4l)
    }
}

// the CPU reference of the kq FFN contract: dequantized weights against the Q8_K activation
// image (per-256 scales), silu, per-256 Q8_K requant, dequantized-down GEMM — double
// accumulation, judged by the same relative-tolerance bar as the q8 arms
// midblk = the INTERMEDIATE's requant block: 256 when down is a K-quant, 32 when down is q8.
// The gate/up dots always ride the kq 256-block activation image; only the h -> down hop changes.
def private ref_ffn_kq(offs1, offs3, offs2 : array<int64>; nreg : int;
                       w1, w3, w2 : array<float>;
                       xq : array<int8>; xs : array<float>; nrows : int64; var gout : array<float>;
                       midblk : int = 256) {
    let kn = int(KN)
    let knfe = int(KNFE)
    // a midblk that does not divide KNFE would silently drop h's tail and produce a WRONG
    // baseline — masking a real kernel failure. verify, not assert: this must hold in release too
    verify(knfe % midblk == 0, "ref_ffn_kq: midblk must divide KNFE")
    gout |> resize(nrows * KN)
    let nsb = int(KN / 256l)
    let nsbh = knfe / midblk
    var gate : array<float>
    var up : array<float>
    var h : array<float>
    var hq : array<int>
    var hs : array<float>
    gate |> resize(KNFE)
    up |> resize(KNFE)
    h |> resize(KNFE)
    hq |> resize(KNFE)
    hs |> resize(nsbh)
    for (r in range(nreg)) {
        let rb1 = int((offs1[r * 3] - KWOFF1) / KN)
        let rb3 = int((offs3[r * 3] - KWOFF3) / KN)
        let rb2 = int((offs2[r * 3] - KWOFF2) / KNFE)
        let row0 = int(offs1[r * 3 + 1])
        let cnt = int(offs1[r * 3 + 2])
        for (rw in range(row0, row0 + cnt)) {
            for (i in range(knfe)) {
                var ag = 0.0lf
                var au = 0.0lf
                for (sb in range(nsb)) {
                    var dg = 0.0lf
                    var du = 0.0lf
                    for (j in range(256)) {
                        let xv = double(xq[rw * int(KN) + sb * 256 + j])
                        dg += double(w1[(rb1 + i) * int(KN) + sb * 256 + j]) * xv
                        du += double(w3[(rb3 + i) * int(KN) + sb * 256 + j]) * xv
                    }
                    let xscl = double(xs[rw * nsb + sb])
                    ag += dg * xscl
                    au += du * xscl
                }
                gate[i] = float(ag)
                up[i] = float(au)
            }
            for (i in range(knfe)) {
                h[i] = (gate[i] / (1.0 + exp(-gate[i]))) * up[i]
            }
            for (sb in range(nsbh)) {
                var amax = 0.0
                for (j in range(midblk)) {
                    amax = max(amax, abs(h[sb * midblk + j]))
                }
                let d = amax / 127.0
                let id = d != 0.0 ? 1.0 / d : 0.0
                hs[sb] = d
                for (j in range(midblk)) {
                    hq[sb * midblk + j] = int(round(h[sb * midblk + j] * id))
                }
            }
            for (o in range(kn)) {
                var y = 0.0lf
                for (sb in range(nsbh)) {
                    var dd = 0.0lf
                    for (j in range(midblk)) {
                        dd += double(w2[(rb2 + o) * int(KNFE) + sb * midblk + j]) * double(hq[sb * midblk + j])
                    }
                    y += dd * double(hs[sb])
                }
                gout[rw * int(KN) + o] = float(y)
            }
        }
    }
}

[test]
def test_vulkan_moe_ffn_kq(t : T?) {
    t |> run("vulkan MoE kq tier (k4 gate/up + k5/k6 down) == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            // k4 gate/up stacks + BOTH down formats in one down stack region set is not a real
            // shape — use two arms sharing the gate/up stacks: down = k6, then down = k5 at a
            // fresh offset (the 35B carries k5 downs on most layers, k6 on the last two)
            var wq1 : array<uint8>
            var ws1 : array<uint8>
            var wq3 : array<uint8>
            var ws3 : array<uint8>
            fill_kq_stack(1, NE * KNFE, KN, 21l, true, wq1, ws1)
            fill_kq_stack(1, NE * KNFE, KN, 37l, true, wq3, ws3)
            var up_ok = moe_gpu_upload_stack(wq1, ws1, KWOFF1, KN, NE * KNFE, 1)
            if (!up_ok) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            up_ok = moe_gpu_upload_stack(wq3, ws3, KWOFF3, KN, NE * KNFE, 1)
            t |> success(up_ok, "k4 up stack uploaded")
            var w1 : array<float>
            var w3 : array<float>
            dequant_kq_stack(1, wq1, ws1, NE * KNFE, KN, w1)
            dequant_kq_stack(1, wq3, ws3, NE * KNFE, KN, w3)
            let ege1 = KNFE * KN
            let ege2 = KN * KNFE
            var xq : array<int8>
            var xs : array<float>
            var gout : array<float>
            var refv : array<float>
            for (dfmt in [3, 2]) {   // k6 down, then k5 down (fresh stack, same gate/up)
                let dwoff = KWOFF2 + int64(3 - dfmt) * 10000000l
                var wq2 : array<uint8>
                var ws2 : array<uint8>
                fill_kq_stack(dfmt, NE * KN, KNFE, 53l + int64(dfmt), false, wq2, ws2)
                up_ok = moe_gpu_upload_stack(wq2, ws2, dwoff, KNFE, NE * KN, dfmt)
                t |> success(up_ok, "k{dfmt == 3 ? 6 : 5} down stack uploaded")
                var w2 : array<float>
                dequant_kq_stack(dfmt, wq2, ws2, NE * KN, KNFE, w2)

                // decode arm: 2 rows, 1 per region
                fill_acts_kq(xq, xs, 2l)
                var offs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF1, ege1)
                var offs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF3, ege1)
                var offs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], dwoff, ege2)
                gout |> resize(int(2l * KN))
                matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, KN, KNFE, 2l, 1, 1, dfmt, false)
                var refoffs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF2, ege2)
                ref_ffn_kq(offs1, offs3, refoffs2, 2, w1, w3, w2, xq, xs, 2l, refv)
                var c = compare_gout(gout, refv)
                t |> success(c.maxref > 0.0, "kq decode arm produced non-trivial outputs")
                t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                    "kq decode arm (down k{dfmt == 3 ? 6 : 5}) within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

                // batch arm (the kq tile): 40 rows (>= the 32-row tile, unpadded), 2 regions
                fill_acts_kq(xq, xs, 40l)
                var boffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF1, ege1)
                var boffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF3, ege1)
                var boffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], dwoff, ege2)
                gout |> resize(int(40l * KN))
                matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, KN, KNFE, 40l, 1, 1, dfmt, false)
                var refboffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF2, ege2)
                ref_ffn_kq(boffs1, boffs3, refboffs2, 2, w1, w3, w2, xq, xs, 40l, refv)
                c = compare_gout(gout, refv)
                t |> success(c.maxref > 0.0, "kq batch-rows arm produced non-trivial outputs")
                t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                    "kq batch-rows arm (down k{dfmt == 3 ? 6 : 5}) within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")
                delete wq2
                delete ws2
                delete w2
                delete offs1
                delete offs3
                delete offs2
                delete refoffs2
                delete boffs1
                delete boffs3
                delete boffs2
                delete refboffs2
            }

            // classifier arm (the head offload's dispatch): one whole-plane k6 GEMV — ONE
            // region covering every row, device meta + pre-recorded cmd. Two passes with
            // perturbed activations exercise the recorded-resubmit path.
            let CWOFF = 9000000l
            let CVOCAB = 512l
            var wqc : array<uint8>
            var wsc : array<uint8>
            fill_kq_stack(3, CVOCAB, KN, 71l, false, wqc, wsc)
            up_ok = moe_gpu_upload_stack(wqc, wsc, CWOFF, KN, CVOCAB, 3)
            t |> success(up_ok, "k6 classifier plane uploaded")
            var wc : array<float>
            dequant_kq_stack(3, wqc, wsc, CVOCAB, KN, wc)
            var ylog : array<float>
            ylog |> resize(int(CVOCAB))
            let nsbc = int(KN / 256l)
            for (cpass in range(2)) {
                fill_acts_kq(xq, xs, 1l)
                xq[0] = int8(int(xq[0]) + cpass * 3)
                matmul_moe_gpu_cls(ylog, CWOFF, xq, xs, KN, CVOCAB, 3)
                var yref : array<float>
                yref |> resize(int(CVOCAB))
                for (r in range(int(CVOCAB))) {
                    var acc = 0.0lf
                    for (sb in range(nsbc)) {
                        var dd = 0.0lf
                        for (j in range(256)) {
                            dd += double(wc[r * int(KN) + sb * 256 + j]) * double(xq[sb * 256 + j])
                        }
                        acc += dd * double(xs[sb])
                    }
                    yref[r] = float(acc)
                }
                let cc = compare_gout(ylog, yref)
                t |> success(cc.maxref > 0.0, "cls arm pass {cpass} produced non-trivial outputs")
                t |> success(cc.maxdiff <= float(REL_TOL) * cc.maxref,
                    "cls GEMV arm pass {cpass} within tolerance (maxdiff {cc.maxdiff} vs {float(REL_TOL) * cc.maxref} bar, maxref {cc.maxref})")
                delete yref
            }
            delete wqc
            delete wsc
            delete wc
            delete ylog
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete w1
            delete w3
            delete xq
            delete xs
            delete gout
            delete refv
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== q40 arms (native Q4_0 stacks — the all-q40 triple a Q4_0 file ships) =====

let QWOFF1 = 41000000l
let QWOFF3 = 42000000l
let QWOFF2 = 43000000l

// Q4_0 rides the k4 nibble tiling with one f16 d per 32-block and no min plane, so its quants
// carry an implicit -8: the kernel folds that as an integer subtract against the block sums,
// and a sign error there is exactly what this arm catches.
[test]
def test_vulkan_moe_ffn_q40(t : T?) {
    t |> run("vulkan MoE q40 tier (all-Q4_0 triple) == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var wq1 : array<uint8>
            var ws1 : array<uint8>
            var wq3 : array<uint8>
            var ws3 : array<uint8>
            var wq2 : array<uint8>
            var ws2 : array<uint8>
            fill_kq_stack(4, NE * KNFE, KN, 83l, false, wq1, ws1)
            fill_kq_stack(4, NE * KNFE, KN, 97l, false, wq3, ws3)
            fill_kq_stack(4, NE * KN, KNFE, 101l, false, wq2, ws2)
            var up_ok = moe_gpu_upload_stack(wq1, ws1, QWOFF1, KN, NE * KNFE, 4)
            if (!up_ok) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            up_ok = moe_gpu_upload_stack(wq3, ws3, QWOFF3, KN, NE * KNFE, 4)
            t |> success(up_ok, "q40 up stack uploaded")
            up_ok = moe_gpu_upload_stack(wq2, ws2, QWOFF2, KNFE, NE * KN, 4)
            t |> success(up_ok, "q40 down stack uploaded")
            var w1 : array<float>
            var w3 : array<float>
            var w2 : array<float>
            dequant_kq_stack(4, wq1, ws1, NE * KNFE, KN, w1)
            dequant_kq_stack(4, wq3, ws3, NE * KNFE, KN, w3)
            dequant_kq_stack(4, wq2, ws2, NE * KN, KNFE, w2)
            let ege1 = KNFE * KN
            let ege2 = KN * KNFE
            var xq : array<int8>
            var xs : array<float>
            var gout : array<float>
            var refv : array<float>

            // decode arm: 2 rows, 1 per region (the GEMV kernel)
            fill_acts_kq(xq, xs, 2l)
            var offs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], QWOFF1, ege1)
            var offs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], QWOFF3, ege1)
            var offs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], QWOFF2, ege2)
            gout |> resize(int(2l * KN))
            matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, KN, KNFE, 2l, 4, 4, 4, false)
            // ref_ffn_kq derives its row bases against the KWOFF* constants
            var refoffs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF1, ege1)
            var refoffs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF3, ege1)
            var refoffs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF2, ege2)
            ref_ffn_kq(refoffs1, refoffs3, refoffs2, 2, w1, w3, w2, xq, xs, 2l, refv)
            var c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "q40 decode arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "q40 decode arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            // batch arm (the q40 tile): 40 rows (>= the 32-row tile, unpadded), 2 regions
            fill_acts_kq(xq, xs, 40l)
            var boffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], QWOFF1, ege1)
            var boffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], QWOFF3, ege1)
            var boffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], QWOFF2, ege2)
            gout |> resize(int(40l * KN))
            matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, KN, KNFE, 40l, 4, 4, 4, false)
            var rboffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF1, ege1)
            var rboffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF3, ege1)
            var rboffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF2, ege2)
            ref_ffn_kq(rboffs1, rboffs3, rboffs2, 2, w1, w3, w2, xq, xs, 40l, refv)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "q40 batch arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "q40 batch arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
            delete w1
            delete w3
            delete w2
            delete xq
            delete xs
            delete gout
            delete refv
            delete offs1
            delete offs3
            delete offs2
            delete refoffs1
            delete refoffs3
            delete refoffs2
            delete boffs1
            delete boffs3
            delete boffs2
            delete rboffs1
            delete rboffs3
            delete rboffs2
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== dense-GEMM rail arms (one-region ride on the batch tiles) =====

let DWOFF = 30000000l   // q8 dense plane base (q8 offset space, clear of WOFF*/SWOFF)
let KDWOFF = 31000000l  // k4 dense plane base (kq offset space, clear of KWOFF*)
let DROP_WOFF = 32000000l   // drop-arm scratch plane (q8 space)
let DD = 160l           // dense output dim (%32)
let DNR = 40l           // batch rows — one full 32-tile plus an unpadded remainder

// llama.cpp's Q4_K_M MoE recipe bumps ffn_down_exps to Q5_0/Q8_0 while gate/up stay Q4_K, so
// k4/k4/q8 is the shape real files ship (glm4moe, gemma). gate/up must agree (one activation
// image); down is free — the chain requants the intermediate in the DOWN stack's form.
let MXOFF1 = 210000000l
let MXOFF3 = 220000000l
let MXOFF2 = 230000000l

def private dequant_q8_stack(wq : array<int8>; ws : array<float>; rows, n : int64; var w : array<float>) {
    let nb = int(n / 32l)
    w |> resize(rows * n)
    for (r in range64(rows)) {
        for (i in range64(n)) {
            w[int(r * n + i)] = float(wq[int(r * n + i)]) * ws[int(r) * nb + int(i / 32l)]
        }
    }
}

[test]
def test_vulkan_moe_ffn_mixed(t : T?) {
    t |> run("vulkan MoE mixed triple (k4 gate/up + q8 down) == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var wq1 : array<uint8>
            var ws1 : array<uint8>
            var wq3 : array<uint8>
            var ws3 : array<uint8>
            fill_kq_stack(1, NE * KNFE, KN, 61l, true, wq1, ws1)
            fill_kq_stack(1, NE * KNFE, KN, 67l, true, wq3, ws3)
            var up_ok = moe_gpu_upload_stack(wq1, ws1, MXOFF1, KN, NE * KNFE, 1)
            if (!up_ok) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            up_ok = moe_gpu_upload_stack(wq3, ws3, MXOFF3, KN, NE * KNFE, 1)
            t |> success(up_ok, "k4 up stack uploaded")
            // down: plain q8, the format the Q5_0/Q8_0 downs land in after the loader's transcode
            var dq2 : array<int8>
            var ds2 : array<float>
            fill_stack(dq2, ds2, NE * KN, KNFE, 71l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(dq2, ds2, wqb, wsb)
            up_ok = moe_gpu_upload_stack(wqb, wsb, MXOFF2, KNFE, NE * KN, 0)
            t |> success(up_ok, "q8 down stack uploaded")

            var w1 : array<float>
            var w3 : array<float>
            var w2 : array<float>
            dequant_kq_stack(1, wq1, ws1, NE * KNFE, KN, w1)
            dequant_kq_stack(1, wq3, ws3, NE * KNFE, KN, w3)
            dequant_q8_stack(dq2, ds2, NE * KN, KNFE, w2)
            let ege1 = KNFE * KN
            let ege2 = KN * KNFE

            var xq : array<int8>
            var xs : array<float>
            fill_acts_kq(xq, xs, 2l)
            var offs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], MXOFF1, ege1)
            var offs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], MXOFF3, ege1)
            var offs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], MXOFF2, ege2)
            var gout : array<float>
            gout |> resize(int(2l * KN))
            matmul_moe_gpu_ffn(gout, offs1, offs3, offs2, 2l, xq, xs, KN, KNFE, 2l, 1, 1, 0, false)
            // ref_ffn_kq derives its row bases against the KWOFF* constants, so the reference
            // gets its own offsets over the same regions (the GPU side rides the MXOFF* stacks)
            let refoffs1 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF1, ege1)
            let refoffs3 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF3, ege1)
            let refoffs2 <- make_offs([1l, 2l], [0l, 1l], [1l, 1l], KWOFF2, ege2)
            var refv : array<float>
            ref_ffn_kq(refoffs1, refoffs3, refoffs2, 2, w1, w3, w2, xq, xs, 2l, refv, 32)
            var c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "mixed decode arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "mixed k4/k4/q8 decode arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")

            // batch arm: 40 rows (>= the 32-row tile, unpadded) — vk_moe_ffn_batch carries its own
            // xunit split and chunk sizing, so the decode arm above does NOT cover it
            fill_acts_kq(xq, xs, 40l)
            var boffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], MXOFF1, ege1)
            var boffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], MXOFF3, ege1)
            var boffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], MXOFF2, ege2)
            gout |> resize(int(40l * KN))
            matmul_moe_gpu_ffn(gout, boffs1, boffs3, boffs2, 2l, xq, xs, KN, KNFE, 40l, 1, 1, 0, false)
            let rboffs1 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF1, ege1)
            let rboffs3 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF3, ege1)
            let rboffs2 <- make_offs([0l, 2l], [0l, 24l], [24l, 16l], KWOFF2, ege2)
            ref_ffn_kq(rboffs1, rboffs3, rboffs2, 2, w1, w3, w2, xq, xs, 40l, refv, 32)
            c = compare_gout(gout, refv)
            t |> success(c.maxref > 0.0, "mixed batch arm produced non-trivial outputs")
            t |> success(c.maxdiff <= float(REL_TOL) * c.maxref,
                "mixed k4/k4/q8 batch arm within tolerance (maxdiff {c.maxdiff} vs {float(REL_TOL) * c.maxref} bar, maxref {c.maxref})")
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete dq2
            delete ds2
            delete wqb
            delete wsb
            delete w1
            delete w3
            delete w2
            delete xq
            delete xs
            delete gout
            delete refv
        } else {
            feint("vulkan module not built; skipping\n")
        }
    }
}

[test]
def test_vulkan_dense_gemm(t : T?) {
    t |> run("vulkan dense GEMM rail == CPU reference (q8 + k4, one-region batch tiles)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            // q8 arm: [DD x N] plane, Q8_0 acts
            var wq : array<int8>
            var ws : array<float>
            fill_stack(wq, ws, DD, N, 17l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wq, ws, wqb, wsb)
            if (!moe_gpu_upload_stack(wqb, wsb, DWOFF, N, DD, 0)) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, DNR)
            var y : array<float>
            y |> resize(int(DNR * DD))
            matmul_gpu_dense(y, DWOFF, xq, xs, N, DD, DNR, 0)
            var refy : array<float>
            refy |> resize(int(DNR * DD))
            let nb = int(N / 32l)
            for (r in range(int(DNR))) {
                for (c in range(int(DD))) {
                    var acc = 0.0lf
                    for (b in range(nb)) {
                        var id = 0
                        for (k in range(32)) {
                            id += int(xq[r * int(N) + b * 32 + k]) * int(wq[c * int(N) + b * 32 + k])
                        }
                        acc += double(ws[c * nb + b]) * double(xs[r * nb + b]) * double(id)
                    }
                    refy[r * int(DD) + c] = float(acc)
                }
            }
            let c1 = compare_gout(y, refy)
            t |> success(c1.maxdiff <= float(REL_TOL) * c1.maxref,
                "dense q8 arm within tolerance (maxdiff {c1.maxdiff} vs {float(REL_TOL) * c1.maxref} bar, maxref {c1.maxref})")
            // k4 arm: [DD x KN] plane, Q8_K acts through the same one-region rail
            var kwqb : array<uint8>
            var kwsb : array<uint8>
            fill_kq_stack(1, DD, KN, 23l, false, kwqb, kwsb)
            if (!moe_gpu_upload_stack(kwqb, kwsb, KDWOFF, KN, DD, 1)) {
                feint("kq dense upload refused (budget); skipping the k4 arm\n")
                return
            }
            var w : array<float>
            dequant_kq_stack(1, kwqb, kwsb, DD, KN, w)
            var kxq : array<int8>
            var kxs : array<float>
            fill_acts_kq(kxq, kxs, DNR)
            var ky : array<float>
            ky |> resize(int(DNR * DD))
            matmul_gpu_dense(ky, KDWOFF, kxq, kxs, KN, DD, DNR, 1)
            var krefy : array<float>
            krefy |> resize(int(DNR * DD))
            let nsb = int(KN / 256l)
            for (r in range(int(DNR))) {
                for (c in range(int(DD))) {
                    var acc = 0.0lf
                    for (sb in range(nsb)) {
                        var dd = 0.0lf
                        for (j in range(256)) {
                            dd += double(w[c * int(KN) + sb * 256 + j]) * double(kxq[r * int(KN) + sb * 256 + j])
                        }
                        acc += dd * double(kxs[r * nsb + sb])
                    }
                    krefy[r * int(DD) + c] = float(acc)
                }
            }
            let c2 = compare_gout(ky, krefy)
            t |> success(c2.maxdiff <= float(REL_TOL) * c2.maxref,
                "dense k4 arm within tolerance (maxdiff {c2.maxdiff} vs {float(REL_TOL) * c2.maxref} bar, maxref {c2.maxref})")
            // drop arm: roll back a fresh upload (the partial-triple path), then prove the
            // registry survived — the earlier q8 stack still serves, and the dropped offset
            // re-uploads and serves (buffer free + budget refund didn't corrupt anything)
            var wq3 : array<int8>
            var ws3 : array<float>
            fill_stack(wq3, ws3, DD, N, 29l, false)
            var wqb3 : array<uint8>
            var wsb3 : array<uint8>
            pack_upload(wq3, ws3, wqb3, wsb3)
            t |> success(moe_gpu_upload_stack(wqb3, wsb3, DROP_WOFF, N, DD, 0), "doomed upload lands")
            moe_gpu_drop_stacks(1, false)
            t |> success(moe_gpu_upload_stack(wqb3, wsb3, DROP_WOFF, N, DD, 0, [stream = true]), "stream mirror lands")
            moe_gpu_drop_stacks(1, true)
            y |> resize(0)
            y |> resize(int(DNR * DD))
            matmul_gpu_dense(y, DWOFF, xq, xs, N, DD, DNR, 0)
            let c3 = compare_gout(y, refy)
            t |> success(c3.maxdiff <= float(REL_TOL) * c3.maxref,
                "q8 stack still serves after a later stack dropped (maxdiff {c3.maxdiff})")
            t |> success(moe_gpu_upload_stack(wqb3, wsb3, DROP_WOFF, N, DD, 0), "dropped offset re-uploads")
            var y3 : array<float>
            y3 |> resize(int(DNR * DD))
            matmul_gpu_dense(y3, DROP_WOFF, xq, xs, N, DD, DNR, 0)
            var refy3 : array<float>
            refy3 |> resize(int(DNR * DD))
            for (r in range(int(DNR))) {
                for (c in range(int(DD))) {
                    var acc = 0.0lf
                    for (b in range(nb)) {
                        var id = 0
                        for (k in range(32)) {
                            id += int(xq[r * int(N) + b * 32 + k]) * int(wq3[c * int(N) + b * 32 + k])
                        }
                        acc += double(ws3[c * nb + b]) * double(xs[r * nb + b]) * double(id)
                    }
                    refy3[r * int(DD) + c] = float(acc)
                }
            }
            let c4 = compare_gout(y3, refy3)
            t |> success(c4.maxdiff <= float(REL_TOL) * c4.maxref,
                "re-uploaded stack serves after drop (maxdiff {c4.maxdiff})")
            delete wq3
            delete ws3
            delete wqb3
            delete wsb3
            delete y3
            delete refy3
            delete wq
            delete ws
            delete wqb
            delete wsb
            delete xq
            delete xs
            delete y
            delete refy
            delete kwqb
            delete kwsb
            delete w
            delete kxq
            delete kxs
            delete ky
            delete krefy
        } else {
            feint("dasVulkan not installed; vulkan MoE tier untestable here\n")
        }
    }
}

// ===== the deltanet chain arm =====
// The whole recurrent-attention chain (qkv/z GEMMs -> conv+SiLU+L2-norm -> chunked delta-rule
// scan -> gated out-norm -> Q8_0 requant -> out projection) vs a CPU mirror of the same math.
// Two fixtures: npos = 70 crosses the 64-chunk boundary; npos = 1100 crosses the 1024-row
// window (device-side tail chaining + state carry across submits). Random-ish L2-normed k rows
// keep |k_i . k_j| ~ 0.1, so the unit-lower solve stays conditioned (the real-model regime).
// The recurrence compounds GPU-vs-CPU exp/f32-accumulation drift, so the bar is wider than the
// GEMM arms' 1e-3.

let DND = 64l          // model dim
let DNDS = 128l        // d_state
let DNKH = 1l          // k-heads
let DNVH = 2l          // v-heads
let DNKD = DNKH * DNDS
let DNDI = DNVH * DNDS
let DNCD = 2l * DNKD + DNDI
let DNCONV = 4l
let DNTAPS = DNCONV - 1l
let DN_EPS = 1e-6
let DNQKV_WOFF = 7000000l
let DNZ_WOFF = 8000000l
let DNO_WOFF = 9000000l
let DN_REL_TOL = 3e-3

def private synth_f(i, seed : int64; amp : float) : float {
    let v = (i * 2654435761l + seed * 40503l) % 2048l
    return (float(v) / 1024.0 - 1.0) * amp
}

// q8 image GEMM mirror (int block dots, double-accumulated): y [nrows x d] = X . W^T
def private ref_q8_gemm(wq : array<int8>; ws : array<float>; xq : array<int8>; xs : array<float>;
                        n, d, nrows : int64; var y : array<float>) {
    let nri = int(nrows)
    let di = int(d)
    y |> resize(nrows * d)
    let nb = int(n / 32l)
    for (r in range(nri)) {
        for (c in range(di)) {
            var acc = 0.0lf
            for (b in range(nb)) {
                var id = 0
                for (k in range(32)) {
                    id += int(xq[r * int(n) + b * 32 + k]) * int(wq[c * int(n) + b * 32 + k])
                }
                acc += double(ws[c * nb + b]) * double(xs[r * nb + b]) * double(id)
            }
            y[r * int(d) + c] = float(acc)
        }
    }
}

// the CPU mirror of the deltanet chain past the qkv/z GEMMs: conv + SiLU + q/k L2-norm +
// q pre-scale, the per-head chunked delta rule, the gated out-norm, the o requant, and the
// out projection. state advances in place; qkvy/zy are the GEMM outputs the caller made.
def private ref_dn_core(qkvy, zy : array<float>; bts, gts, tapsv, wnormv, av, dtv : array<float>;   // nolint:STYLE037 — one flat CPU mirror of the deltanet chain; its stages share the chunk buffers
                        var state : array<float>; hist : array<float>; npos : int64;
                        wo : array<int8>; wso : array<float>; var yout : array<float>; var conv : array<float>) {
    conv |> resize(npos * DNCD)
    for (p in range64(npos)) {
        for (ch in range64(DNCD)) {
            var acc = 0.0
            for (k in range64(DNCONV)) {
                let sp = p - DNTAPS + k
                let xv = sp >= 0l ? qkvy[int(sp * DNCD + ch)] : hist[int(ch * DNTAPS + p + k)]
                acc += xv * tapsv[int(ch * DNCONV + k)]
            }
            conv[int(p * DNCD + ch)] = acc / (1.0 + exp(-acc))
        }
        for (blk in range64(2l * DNKD / DNDS)) {   // q/k L2 norm; q folds the pre-scale
            var sum = 0.0lf
            for (j in range64(DNDS)) {
                let v = conv[int(p * DNCD + blk * DNDS + j)]
                sum += double(v * v)
            }
            var scale = 1.0 / max(sqrt(float(sum)), DN_EPS)
            if (blk < DNKD / DNDS) {
                scale *= 1.0 / sqrt(float(DNDS))
            }
            for (j in range64(DNDS)) {
                conv[int(p * DNCD + blk * DNDS + j)] *= scale
            }
        }
    }
    let CS = 64l
    var o : array<float>
    o |> resize(npos * DNDI)
    var p_qc : array<float>
    var p_kc : array<float>
    var p_kb : array<float>
    var p_vb : array<float>
    var p_w : array<float>
    var p_vn : array<float>
    var p_oc : array<float>
    var p_tm : array<float>
    var p_am : array<float>
    var p_kq : array<float>
    var gcum : array<float>
    var egc : array<float>
    var bv : array<float>
    p_qc |> resize(CS * DNDS); p_kc |> resize(CS * DNDS); p_kb |> resize(CS * DNDS)
    p_vb |> resize(CS * DNDS); p_w |> resize(CS * DNDS); p_vn |> resize(CS * DNDS)
    p_oc |> resize(CS * DNDS); p_tm |> resize(CS * CS); p_am |> resize(CS * CS)
    p_kq |> resize(CS * CS); gcum |> resize(CS); egc |> resize(CS); bv |> resize(CS)
    for (h in range64(DNVH)) {
        let kh = h % DNKH
        let qoff = kh * DNDS
        let koff = DNKD + kh * DNDS
        let voff = 2l * DNKD + h * DNDS
        let sbase = int(h * DNDS * DNDS)
        var c0 = 0l
        while (c0 < npos) {
            let cl = min(CS, npos - c0)
            var gacc = 0.0
            for (i in range64(cl)) {
                bv[int(i)] = 1.0 / (1.0 + exp(-bts[int((c0 + i) * DNVH + h)]))
                let graw = gts[int((c0 + i) * DNVH + h)] + dtv[int(h)]
                gacc += av[int(h)] * (graw > 20.0 ? graw : log(1.0 + exp(graw)))
                gcum[int(i)] = gacc
                egc[int(i)] = exp(gacc)
                for (j in range64(DNDS)) {
                    let crow = (c0 + i) * DNCD
                    p_qc[int(i * DNDS + j)] = conv[crow + qoff + j]
                    p_kc[int(i * DNDS + j)] = conv[crow + koff + j]
                    p_kb[int(i * DNDS + j)] = conv[crow + koff + j] * bv[int(i)]
                    p_vb[int(i * DNDS + j)] = conv[crow + voff + j] * bv[int(i)]
                }
            }
            for (i in range64(cl)) {   // A = kb k^T (decay-scaled below); KQ keeps its diagonal
                for (jc in range64(cl)) {
                    var sa = 0.0
                    var sq = 0.0
                    for (j in range64(DNDS)) {
                        sa += p_kb[int(i * DNDS + j)] * p_kc[int(jc * DNDS + j)]
                        sq += p_qc[int(i * DNDS + j)] * p_kc[int(jc * DNDS + j)]
                    }
                    p_am[int(i * CS + jc)] = sa
                    p_kq[int(i * CS + jc)] = sq
                }
            }
            for (i in range64(cl)) {
                for (jc in range64(cl)) {
                    if (jc < i) {
                        let d = exp(gcum[int(i)] - gcum[int(jc)])
                        p_am[int(i * CS + jc)] = -p_am[int(i * CS + jc)] * d
                        p_kq[int(i * CS + jc)] *= d
                    } elif (jc == i) {
                        p_am[int(i * CS + jc)] = 1.0
                    } else {
                        p_am[int(i * CS + jc)] = 0.0
                        p_kq[int(i * CS + jc)] = 0.0
                    }
                }
            }
            // unit-lower solve off the A' originals. The copy MUST be CS-strided: the w/d GEMMs
            // below read T's diagonal and upper entries too, which the solve never writes — a
            // linear cl*cl copy misses them for cl < CS (masked before this fix because every
            // fixture's FIRST chunk was exactly CS, pre-seeding diag = 1 / upper = 0)
            for (i in range64(cl)) {
                for (jc in range64(cl)) {
                    p_tm[int(i * CS + jc)] = p_am[int(i * CS + jc)]
                }
            }
            for (i in range64(cl)) {
                for (jc in range64(i)) {
                    var acc = p_am[int(i * CS + jc)]
                    for (tt in range64(jc + 1l, i)) {
                        acc += p_am[int(i * CS + tt)] * p_tm[int(tt * CS + jc)]
                    }
                    p_tm[int(i * CS + jc)] = acc
                }
            }
            for (i in range64(cl)) {
                for (j in range64(DNDS)) {
                    p_kb[int(i * DNDS + j)] *= -egc[int(i)]
                }
            }
            for (i in range64(cl)) {   // w = T kb_scaled; d = T vb + w S
                for (j in range64(DNDS)) {
                    var sw = 0.0
                    var sv = 0.0
                    for (tt in range64(cl)) {
                        sw += p_tm[int(i * CS + tt)] * p_kb[int(tt * DNDS + j)]
                        sv += p_tm[int(i * CS + tt)] * p_vb[int(tt * DNDS + j)]
                    }
                    p_w[int(i * DNDS + j)] = sw
                    p_vn[int(i * DNDS + j)] = sv
                }
            }
            for (i in range64(cl)) {
                for (j in range64(DNDS)) {
                    var sv = p_vn[int(i * DNDS + j)]
                    for (d in range64(DNDS)) {
                        sv += p_w[int(i * DNDS + d)] * state[sbase + int(d * DNDS + j)]
                    }
                    p_vn[int(i * DNDS + j)] = sv
                }
            }
            for (i in range64(cl)) {   // o = (q e^gcum) S + KQ d
                for (j in range64(DNDS)) {
                    p_qc[int(i * DNDS + j)] *= egc[int(i)]
                }
            }
            for (i in range64(cl)) {
                for (j in range64(DNDS)) {
                    var so = 0.0
                    for (d in range64(DNDS)) {
                        so += p_qc[int(i * DNDS + d)] * state[sbase + int(d * DNDS + j)]
                    }
                    for (tt in range64(cl)) {
                        so += p_kq[int(i * CS + tt)] * p_vn[int(tt * DNDS + j)]
                    }
                    p_oc[int(i * DNDS + j)] = so
                }
            }
            for (i in range64(cl)) {   // gated out-norm into the o rows
                var sum = 0.0
                for (j in range64(DNDS)) {
                    let v = p_oc[int(i * DNDS + j)]
                    sum += v * v
                }
                let scale = 1.0 / sqrt(sum / float(DNDS) + DN_EPS)
                for (j in range64(DNDS)) {
                    let zv = zy[int((c0 + i) * DNDI + h * DNDS + j)]
                    let xn = p_oc[int(i * DNDS + j)] * scale * wnormv[int(j)]
                    o[int((c0 + i) * DNDI + h * DNDS + j)] = xn * (zv / (1.0 + exp(-zv)))
                }
            }
            let glast = gcum[int(cl - 1l)]
            let egl = exp(glast)
            for (i in range64(DNDS * DNDS)) {
                state[sbase + int(i)] *= egl
            }
            for (d in range64(DNDS)) {   // S += (k e^{glast - gcum})^T d
                for (j in range64(DNDS)) {
                    var sv = state[sbase + int(d * DNDS + j)]
                    for (i in range64(cl)) {
                        sv += p_kc[int(i * DNDS + d)] * exp(glast - gcum[int(i)]) * p_vn[int(i * DNDS + j)]
                    }
                    state[sbase + int(d * DNDS + j)] = sv
                }
            }
            c0 += cl
        }
    }
    // Q8_0 requant of the o rows, then the out projection off the quantized image
    var oq : array<int8>
    var os : array<float>
    oq |> resize(npos * DNDI)
    os |> resize(npos * DNDI / 32l)
    for (b in range64(npos * DNDI / 32l)) {
        var amax = 0.0
        for (j in range64(32l)) {
            amax = max(amax, abs(o[int(b * 32l + j)]))
        }
        let d = amax / 127.0
        let id = d != 0.0 ? 1.0 / d : 0.0
        os[int(b)] = d
        for (j in range64(32l)) {
            oq[int(b * 32l + j)] = int8(int(round(o[int(b * 32l + j)] * id)))
        }
    }
    ref_q8_gemm(wo, wso, oq, os, DNDI, DND, npos, yout)
    delete o
    delete oq
    delete os
    delete p_qc
    delete p_kc
    delete p_kb
    delete p_vb
    delete p_w
    delete p_vn
    delete p_oc
    delete p_tm
    delete p_am
    delete p_kq
    delete gcum
    delete egc
    delete bv
}

[test]
def test_vulkan_deltanet(t : T?) {
    t |> run("vulkan deltanet chain == CPU reference (chunk-boundary + multi-window arms)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var wqkv : array<int8>
            var wsqkv : array<float>
            var wz : array<int8>
            var wsz : array<float>
            var wo : array<int8>
            var wso : array<float>
            fill_stack(wqkv, wsqkv, DNCD, DND, 31l, true)
            fill_stack(wz, wsz, DNDI, DND, 37l, true)
            fill_stack(wo, wso, DND, DNDI, 41l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wqkv, wsqkv, wqb, wsb)
            if (!moe_gpu_upload_stack(wqb, wsb, DNQKV_WOFF, DND, DNCD, 0)) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            pack_upload(wz, wsz, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, DNZ_WOFF, DND, DNDI, 0), "z stack uploaded")
            pack_upload(wo, wso, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, DNO_WOFF, DNDI, DND, 0), "out stack uploaded")
            for (arm_npos in [70l, 1100l]) {
                let npos = arm_npos
                var xq : array<int8>
                var xs : array<float>
                xq |> resize(int(npos * DND))
                xs |> resize(int(npos * (DND / 32l)))
                for (i in range(int(npos * DND))) {
                    xq[i] = int8((int64(i) * 13l + 7l) % 255l - 127l)
                }
                for (i in range(int(npos * (DND / 32l)))) {
                    xs[i] = pow2_small(int64(i) + 4l)
                }
                var bts : array<float>
                var gts : array<float>
                bts |> resize(int(npos * DNVH))
                gts |> resize(int(npos * DNVH))
                for (i in range(int(npos * DNVH))) {
                    bts[i] = synth_f(int64(i), 3l, 1.5)
                    gts[i] = synth_f(int64(i), 11l, 1.5)
                }
                var tapsv : array<float>
                tapsv |> resize(int(DNCD * DNCONV))
                for (i in range(int(DNCD * DNCONV))) {
                    tapsv[i] = synth_f(int64(i), 17l, 0.25)
                }
                var wnormv : array<float>
                wnormv |> resize(int(DNDS))
                for (i in range(int(DNDS))) {
                    wnormv[i] = 1.0 + synth_f(int64(i), 19l, 0.3)
                }
                var av : array<float>
                var dtv : array<float>
                av |> resize(int(DNVH))
                dtv |> resize(int(DNVH))
                for (i in range(int(DNVH))) {
                    av[i] = -(0.3 + abs(synth_f(int64(i), 23l, 1.2)))
                    dtv[i] = synth_f(int64(i), 29l, 0.4)
                }
                var hist : array<float>
                hist |> resize(int(DNCD * DNTAPS))
                for (i in range(int(DNCD * DNTAPS))) {
                    hist[i] = synth_f(int64(i), 43l, 0.5)
                }
                var gstate : array<float>
                var rstate : array<float>
                gstate |> resize(int(DNVH * DNDS * DNDS))
                rstate |> resize(int(DNVH * DNDS * DNDS))
                for (i in range(int(DNVH * DNDS * DNDS))) {
                    gstate[i] = synth_f(int64(i), 47l, 0.1)
                    rstate[i] = gstate[i]
                }
                var y : array<float>
                var tail : array<float>
                y |> resize(int(npos * DND))
                tail |> resize(int(DNTAPS * DNCD))
                unsafe {
                    matmul_moe_gpu_dn(y, DNQKV_WOFF, DNZ_WOFF, DNO_WOFF, xq, xs, bts, gts,
                        addr<float const?>(tapsv[0]), addr<float const?>(wnormv[0]),
                        addr<float const?>(av[0]), addr<float const?>(dtv[0]),
                        addr(gstate[0]), tail, addr<float const?>(hist[0]),
                        npos, DND, DNCD, DNKD, DNDI, DNVH, DNKH, DNDS, DNCONV, DN_EPS)
                }
                var qkvy : array<float>
                var zy : array<float>
                ref_q8_gemm(wqkv, wsqkv, xq, xs, DND, DNCD, npos, qkvy)
                ref_q8_gemm(wz, wsz, xq, xs, DND, DNDI, npos, zy)
                var refy : array<float>
                var refconv : array<float>
                ref_dn_core(qkvy, zy, bts, gts, tapsv, wnormv, av, dtv, rstate, hist, npos, wo, wso, refy, refconv)
                let cy = compare_gout(y, refy)
                t |> success(cy.maxref > 0.0, "npos={npos}: chain produced non-trivial outputs")
                t |> success(cy.maxdiff <= float(DN_REL_TOL) * cy.maxref,
                    "npos={npos}: out rows within tolerance (maxdiff {cy.maxdiff} vs {float(DN_REL_TOL) * cy.maxref} bar, maxref {cy.maxref})")
                let cs = compare_gout(gstate, rstate)
                t |> success(cs.maxdiff <= float(DN_REL_TOL) * cs.maxref,
                    "npos={npos}: final state within tolerance (maxdiff {cs.maxdiff} vs {float(DN_REL_TOL) * cs.maxref} bar, maxref {cs.maxref})")
                var reft : array<float>
                reft |> resize(int(DNTAPS * DNCD))
                for (k in range64(DNTAPS)) {
                    for (ch in range64(DNCD)) {
                        reft[int(k * DNCD + ch)] = qkvy[int((npos - DNTAPS + k) * DNCD + ch)]
                    }
                }
                let ct = compare_gout(tail, reft)
                t |> success(ct.maxdiff <= float(REL_TOL) * ct.maxref,
                    "npos={npos}: conv-tail rows within tolerance (maxdiff {ct.maxdiff} vs {float(REL_TOL) * ct.maxref} bar)")
                delete xq
                delete xs
                delete bts
                delete gts
                delete tapsv
                delete wnormv
                delete av
                delete dtv
                delete hist
                delete gstate
                delete rstate
                delete y
                delete tail
                delete qkvy
                delete zy
                delete refy
                delete refconv
                delete reft
            }
            delete wqkv
            delete wsqkv
            delete wz
            delete wsz
            delete wo
            delete wso
            delete wqb
            delete wsb
        } else {
            feint("dasVulkan not installed; vulkan deltanet chain untestable here\n")
        }
    }
}

// ===== the deltanet decode-step arm =====
// npos == 1 through the same MoeGpuDnFn: per-token submits with DEVICE-resident state and conv
// ring, vs the same CPU mirror run once over all tokens (chunking is an algebraic identity —
// only f32 drift separates cl = 1 steps from one cl = T chunk). The flush at token 4 exercises
// the whole residency seam: device -> CPU sync, then a cold re-upload from the flushed state,
// and the final outputs must still match the straight-through reference.

let DNSQ_WOFF = 17000000l
let DNSZ_WOFF = 18000000l
let DNSO_WOFF = 19000000l
let DNSTEPS = 8l

[test]
def test_vulkan_deltanet_step(t : T?) {
    t |> run("vulkan deltanet decode step == CPU reference (state carry + mid-sequence flush)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var wqkv : array<int8>
            var wsqkv : array<float>
            var wz : array<int8>
            var wsz : array<float>
            var wo : array<int8>
            var wso : array<float>
            fill_stack(wqkv, wsqkv, DNCD, DND, 61l, true)
            fill_stack(wz, wsz, DNDI, DND, 67l, true)
            fill_stack(wo, wso, DND, DNDI, 71l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wqkv, wsqkv, wqb, wsb)
            if (!moe_gpu_upload_stack(wqb, wsb, DNSQ_WOFF, DND, DNCD, 0)) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            pack_upload(wz, wsz, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, DNSZ_WOFF, DND, DNDI, 0), "z stack uploaded")
            pack_upload(wo, wso, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, DNSO_WOFF, DNDI, DND, 0), "out stack uploaded")
            set_moe_gpu_dn_layer(0l, DNSQ_WOFF)   // the flush hook resolves layer -> tier state by this
            var xq : array<int8>
            var xs : array<float>
            xq |> resize(int(DNSTEPS * DND))
            xs |> resize(int(DNSTEPS * (DND / 32l)))
            for (i in range(int(DNSTEPS * DND))) {
                xq[i] = int8((int64(i) * 13l + 5l) % 255l - 127l)
            }
            for (i in range(int(DNSTEPS * (DND / 32l)))) {
                xs[i] = pow2_small(int64(i) + 2l)
            }
            var bts : array<float>
            var gts : array<float>
            bts |> resize(int(DNSTEPS * DNVH))
            gts |> resize(int(DNSTEPS * DNVH))
            for (i in range(int(DNSTEPS * DNVH))) {
                bts[i] = synth_f(int64(i), 3l, 1.5)
                gts[i] = synth_f(int64(i), 11l, 1.5)
            }
            var tapsv : array<float>
            tapsv |> resize(int(DNCD * DNCONV))
            for (i in range(int(DNCD * DNCONV))) {
                tapsv[i] = synth_f(int64(i), 17l, 0.25)
            }
            var wnormv : array<float>
            wnormv |> resize(int(DNDS))
            for (i in range(int(DNDS))) {
                wnormv[i] = 1.0 + synth_f(int64(i), 19l, 0.3)
            }
            var av : array<float>
            var dtv : array<float>
            av |> resize(int(DNVH))
            dtv |> resize(int(DNVH))
            for (i in range(int(DNVH))) {
                av[i] = -(0.3 + abs(synth_f(int64(i), 23l, 1.2)))
                dtv[i] = synth_f(int64(i), 29l, 0.4)
            }
            var state0 : array<float>
            state0 |> resize(int(DNVH * DNDS * DNDS))
            for (i in range(int(DNVH * DNDS * DNDS))) {
                state0[i] = synth_f(int64(i), 47l, 0.1)
            }
            var hist0 : array<float>
            hist0 |> resize(int(DNCD * DNTAPS))
            for (i in range(int(DNCD * DNTAPS))) {
                hist0[i] = synth_f(int64(i), 43l, 0.5)
            }
            // references: the straight-through 8-token mirror and the 4-token prefix (flush point)
            var qkvy : array<float>
            var zy : array<float>
            ref_q8_gemm(wqkv, wsqkv, xq, xs, DND, DNCD, DNSTEPS, qkvy)
            ref_q8_gemm(wz, wsz, xq, xs, DND, DNDI, DNSTEPS, zy)
            var rstate : array<float>
            var rstate4 : array<float>
            rstate := state0
            rstate4 := state0
            var refy : array<float>
            var refconv : array<float>
            ref_dn_core(qkvy, zy, bts, gts, tapsv, wnormv, av, dtv, rstate, hist0, DNSTEPS, wo, wso, refy, refconv)
            var qkvy4 : array<float>
            var zy4 : array<float>
            var bts4 : array<float>
            var gts4 : array<float>
            qkvy4 |> resize(int(4l * DNCD))
            zy4 |> resize(int(4l * DNDI))
            bts4 |> resize(int(4l * DNVH))
            gts4 |> resize(int(4l * DNVH))
            for (i in range(int(4l * DNCD))) {
                qkvy4[i] = qkvy[i]
            }
            for (i in range(int(4l * DNDI))) {
                zy4[i] = zy[i]
            }
            for (i in range(int(4l * DNVH))) {
                bts4[i] = bts[i]
                gts4[i] = gts[i]
            }
            var refy4 : array<float>
            var refconv4 : array<float>
            ref_dn_core(qkvy4, zy4, bts4, gts4, tapsv, wnormv, av, dtv, rstate4, hist0, 4l, wo, wso, refy4, refconv4)
            var reft : array<float>
            var reft4 : array<float>
            reft |> resize(int(DNTAPS * DNCD))
            reft4 |> resize(int(DNTAPS * DNCD))
            for (k in range64(DNTAPS)) {   // ring references, CPU channel-major
                for (ch in range64(DNCD)) {
                    reft[int(ch * DNTAPS + k)] = qkvy[int((DNSTEPS - DNTAPS + k) * DNCD + ch)]
                    reft4[int(ch * DNTAPS + k)] = qkvy[int((4l - DNTAPS + k) * DNCD + ch)]
                }
            }
            // the step arm: gstate/gring are THE engine-side CPU state — cold-uploaded at token 0,
            // flushed back (and re-uploaded from) at token 4, flushed again after token 7
            var gstate : array<float>
            var gring : array<float>
            gstate := state0
            gring := hist0
            var y : array<float>
            y |> resize(int(DNSTEPS * DND))
            var y1 : array<float>
            var xq1 : array<int8>
            var xs1 : array<float>
            var b1 : array<float>
            var g1 : array<float>
            var tail1 : array<float>
            y1 |> resize(int(DND))
            xq1 |> resize(int(DND))
            xs1 |> resize(int(DND / 32l))
            b1 |> resize(int(DNVH))
            g1 |> resize(int(DNVH))
            tail1 |> resize(int(DNTAPS * DNCD))
            for (tok in range64(DNSTEPS)) {
                for (i in range64(DND)) {
                    xq1[int(i)] = xq[int(tok * DND + i)]
                }
                for (i in range64(DND / 32l)) {
                    xs1[int(i)] = xs[int(tok * (DND / 32l) + i)]
                }
                for (i in range64(DNVH)) {
                    b1[int(i)] = bts[int(tok * DNVH + i)]
                    g1[int(i)] = gts[int(tok * DNVH + i)]
                }
                unsafe {
                    matmul_moe_gpu_dn(y1, DNSQ_WOFF, DNSZ_WOFF, DNSO_WOFF, xq1, xs1, b1, g1,
                        addr<float const?>(tapsv[0]), addr<float const?>(wnormv[0]),
                        addr<float const?>(av[0]), addr<float const?>(dtv[0]),
                        addr(gstate[0]), tail1, addr<float const?>(gring[0]),
                        1l, DND, DNCD, DNKD, DNDI, DNVH, DNKH, DNDS, DNCONV, DN_EPS)
                }
                for (i in range64(DND)) {
                    y[int(tok * DND + i)] = y1[int(i)]
                }
                if (tok == 3l) {
                    unsafe {
                        moe_gpu_dn_flush(0l, addr(gstate[0]), addr(gring[0]))
                    }
                    let cs4 = compare_gout(gstate, rstate4)
                    t |> success(cs4.maxdiff <= float(DN_REL_TOL) * cs4.maxref,
                        "flushed state at token 4 within tolerance (maxdiff {cs4.maxdiff} vs {float(DN_REL_TOL) * cs4.maxref} bar)")
                    let cr4 = compare_gout(gring, reft4)
                    t |> success(cr4.maxdiff <= float(REL_TOL) * cr4.maxref,
                        "flushed conv ring at token 4 within tolerance (maxdiff {cr4.maxdiff} vs {float(REL_TOL) * cr4.maxref} bar)")
                }
            }
            unsafe {
                moe_gpu_dn_flush(0l, addr(gstate[0]), addr(gring[0]))
            }
            let cy = compare_gout(y, refy)
            t |> success(cy.maxref > 0.0, "step chain produced non-trivial outputs")
            t |> success(cy.maxdiff <= float(DN_REL_TOL) * cy.maxref,
                "step out rows across the flush within tolerance (maxdiff {cy.maxdiff} vs {float(DN_REL_TOL) * cy.maxref} bar, maxref {cy.maxref})")
            let cs = compare_gout(gstate, rstate)
            t |> success(cs.maxdiff <= float(DN_REL_TOL) * cs.maxref,
                "final state within tolerance (maxdiff {cs.maxdiff} vs {float(DN_REL_TOL) * cs.maxref} bar)")
            let cr = compare_gout(gring, reft)
            t |> success(cr.maxdiff <= float(REL_TOL) * cr.maxref,
                "final conv ring within tolerance (maxdiff {cr.maxdiff} vs {float(REL_TOL) * cr.maxref} bar)")
            reset_moe_gpu_dn_layers()
            delete wqkv
            delete wsqkv
            delete wz
            delete wsz
            delete wo
            delete wso
            delete wqb
            delete wsb
            delete xq
            delete xs
            delete bts
            delete gts
            delete tapsv
            delete wnormv
            delete av
            delete dtv
            delete state0
            delete hist0
            delete qkvy
            delete zy
            delete rstate
            delete rstate4
            delete refy
            delete refconv
            delete qkvy4
            delete zy4
            delete bts4
            delete gts4
            delete refy4
            delete refconv4
            delete reft
            delete reft4
            delete gstate
            delete gring
            delete y
            delete y1
            delete xq1
            delete xs1
            delete b1
            delete g1
            delete tail1
        } else {
            feint("dasVulkan not installed; vulkan deltanet decode step untestable here\n")
        }
    }
}

// ===== attention chain arms =====
// The whole full-attention chain (q|gate/k/v GEMMs -> per-head qk-rms -> partial NEOX rope ->
// flash-style causal attention with the sigmoid out-gate -> requant -> wo GEMM) vs a CPU mirror
// of the same math (straight softmax — the online reorder is fp-equivalent within tolerance).
// Arms: gated partial-rope, ungated full-rope, a k4 wo (the mixed-class Q8_K requant path), and
// npos = 1100 (multi-window: absolute-position K/V accumulation + cross-window attention).

let ATD = 64l          // model dim
let ATHS = 128l        // head size (the kernel's dpl=4 shape)
let ATNH = 4l          // q heads
let ATNKV = 2l         // kv heads (GQA kv_mul = 2)
let ATQD = ATNH * ATHS
let ATKVD = ATNKV * ATHS
let AT_EPS = 1e-6
let ATQ_WOFF = 21000000l
let ATK_WOFF = 22000000l
let ATV_WOFF = 23000000l
let ATO_WOFF = 24000000l
let ATO4_WOFF = 25000000l
let AT_REL_TOL = 3e-3

// deinterleave (gated) + per-head rms + partial NEOX rope of one projection plane — the CPU
// mirror of at_prep. rows out d-wide; a gated q also returns the raw gate plane.
def private ref_at_prep(y : array<float>; rmsw : array<float>; cosr, sinr : array<float>;
                        npos, d, rot : int64; gated : bool;
                        var outp : array<float>; var gatep : array<float>) {
    let half = rot / 2l
    outp |> resize(npos * d)
    if (gated) {
        gatep |> resize(npos * d)
    }
    var row : array<float>
    row |> resize(d)
    for (p in range64(npos)) {
        for (i in range64(d)) {
            let h = i / ATHS
            let j = i % ATHS
            if (gated) {
                row[i] = y[p * 2l * d + h * 2l * ATHS + j]
                gatep[p * d + i] = y[p * 2l * d + h * 2l * ATHS + ATHS + j]
            } else {
                row[i] = y[p * d + i]
            }
        }
        for (h in range64(d / ATHS)) {   // per-head rms (f32 square-sum, the kernel's order)
            var ss = 0.0
            for (j in range64(ATHS)) {
                let v = row[h * ATHS + j]
                ss += v * v
            }
            let scale = 1.0 / sqrt(ss / float(ATHS) + AT_EPS)
            for (j in range64(ATHS)) {
                row[h * ATHS + j] = rmsw[j] * (row[h * ATHS + j] * scale)
            }
        }
        for (h in range64(d / ATHS)) {   // NEOX pairs (j, j+half) over the roped span
            for (j in range64(half)) {
                let v0 = row[h * ATHS + j]
                let v1 = row[h * ATHS + j + half]
                let cr = cosr[p * half + j]
                let ci = sinr[p * half + j]
                row[h * ATHS + j] = v0 * cr - v1 * ci
                row[h * ATHS + j + half] = v0 * ci + v1 * cr
            }
        }
        for (i in range64(d)) {
            outp[p * d + i] = row[i]
        }
    }
    delete row
}

// straight-softmax causal attention + out-gate — the CPU mirror of at_attn's online form
def private ref_at_attn(qp, kp, vp, gatep : array<float>; npos : int64; gated : bool;
                        var op : array<float>) {
    let scale = 1.0 / sqrt(float(ATHS))
    let kv_mul = ATNH / ATNKV
    op |> resize(npos * ATQD)
    var sc : array<float>
    sc |> resize(npos)
    for (p in range64(npos)) {
        for (h in range64(ATNH)) {
            let kvh = h / kv_mul
            var mx = -3.0e38
            for (j in range64(p + 1l)) {
                var s = 0.0
                for (dd in range64(ATHS)) {
                    s += qp[p * ATQD + h * ATHS + dd] * kp[j * ATKVD + kvh * ATHS + dd]
                }
                sc[j] = s * scale
                mx = max(mx, sc[j])
            }
            var lsum = 0.0
            for (j in range64(p + 1l)) {
                sc[j] = exp(sc[j] - mx)
                lsum += sc[j]
            }
            for (dd in range64(ATHS)) {
                var acc = 0.0
                for (j in range64(p + 1l)) {
                    acc += sc[j] * vp[j * ATKVD + kvh * ATHS + dd]
                }
                var v = acc / lsum
                if (gated) {
                    let g = gatep[p * ATQD + h * ATHS + dd]
                    v = v / (1.0 + exp(-g))
                }
                op[p * ATQD + h * ATHS + dd] = v
            }
        }
    }
    delete sc
}

[test]
def test_vulkan_attention(t : T?) {
    t |> run("vulkan attention chain == CPU reference (gated/ungated/kq-wo/multi-window arms)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var wqg : array<int8>       // gated q|gate plane (2*ATQD rows)
            var wsqg : array<float>
            var wqp : array<int8>       // plain q plane (ungated arm)
            var wsqp : array<float>
            var wk : array<int8>
            var wsk : array<float>
            var wv : array<int8>
            var wsv : array<float>
            var wo : array<int8>
            var wso : array<float>
            fill_stack(wqg, wsqg, 2l * ATQD, ATD, 51l, true)
            fill_stack(wqp, wsqp, ATQD, ATD, 53l, true)
            fill_stack(wk, wsk, ATKVD, ATD, 57l, true)
            fill_stack(wv, wsv, ATKVD, ATD, 59l, true)
            fill_stack(wo, wso, ATD, ATQD, 61l, false)
            var wqb : array<uint8>
            var wsb : array<uint8>
            pack_upload(wqg, wsqg, wqb, wsb)
            if (!moe_gpu_upload_stack(wqb, wsb, ATQ_WOFF, ATD, 2l * ATQD, 0)) {
                feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                return
            }
            pack_upload(wqp, wsqp, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, ATQ_WOFF + 500000l, ATD, ATQD, 0), "plain q stack uploaded")
            pack_upload(wk, wsk, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, ATK_WOFF, ATD, ATKVD, 0), "k stack uploaded")
            pack_upload(wv, wsv, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, ATV_WOFF, ATD, ATKVD, 0), "v stack uploaded")
            pack_upload(wo, wso, wqb, wsb)
            t |> success(moe_gpu_upload_stack(wqb, wsb, ATO_WOFF, ATQD, ATD, 0), "wo stack uploaded")
            var wo4b : array<uint8>
            var wso4b : array<uint8>
            fill_kq_stack(1, ATD, ATQD, 67l, false, wo4b, wso4b)
            t |> success(moe_gpu_upload_stack(wo4b, wso4b, ATO4_WOFF, ATQD, ATD, 1), "k4 wo stack uploaded")
            var wo4 : array<float>
            dequant_kq_stack(1, wo4b, wso4b, ATD, ATQD, wo4)
            var rmsq : array<float>
            var rmsk : array<float>
            rmsq |> resize(int(ATHS))
            rmsk |> resize(int(ATHS))
            for (i in range(int(ATHS))) {
                rmsq[i] = 1.0 + synth_f(int64(i), 71l, 0.3)
                rmsk[i] = 1.0 + synth_f(int64(i), 73l, 0.3)
            }
            // arms: (npos, rot, gated, k4 wo)
            for (arm in [(npos = 70l, rot = 64l, gated = true, o4 = false),
                         (npos = 70l, rot = 128l, gated = false, o4 = false),
                         (npos = 70l, rot = 64l, gated = true, o4 = true),
                         (npos = 1100l, rot = 64l, gated = true, o4 = false)]) {
                let npos = arm.npos
                let rot = arm.rot
                let half = rot / 2l
                var xq : array<int8>
                var xs : array<float>
                xq |> resize(int(npos * ATD))
                xs |> resize(int(npos * (ATD / 32l)))
                for (i in range(int(npos * ATD))) {
                    xq[i] = int8((int64(i) * 13l + 7l) % 255l - 127l)
                }
                for (i in range(int(npos * (ATD / 32l)))) {
                    xs[i] = pow2_small(int64(i) + 6l)
                }
                var cosr : array<float>
                var sinr : array<float>
                cosr |> resize(int(npos * half))
                sinr |> resize(int(npos * half))
                for (p in range64(npos)) {
                    for (j in range64(half)) {
                        let freq = 1.0 / pow(10000.0, float(2l * j) / float(rot))
                        cosr[int(p * half + j)] = cos(float(p) * freq)
                        sinr[int(p * half + j)] = sin(float(p) * freq)
                    }
                }
                var y : array<float>
                var kout : array<float>
                var vout : array<float>
                y |> resize(int(npos * ATD))
                kout |> resize(int(npos * ATKVD))
                vout |> resize(int(npos * ATKVD))
                unsafe {
                    matmul_moe_gpu_attn(y, kout, vout,
                        arm.gated ? ATQ_WOFF : ATQ_WOFF + 500000l, ATK_WOFF, ATV_WOFF,
                        arm.o4 ? ATO4_WOFF : ATO_WOFF,
                        0, 0, 0, arm.o4 ? 1 : 0, xq, xs,
                        addr<float const?>(rmsq[0]), addr<float const?>(rmsk[0]),
                        addr<float const?>(cosr[0]), addr<float const?>(sinr[0]),
                        npos, ATD, ATQD, ATKVD, ATHS, rot, ATNH, ATNH / ATNKV,
                        1.0 / sqrt(float(ATHS)), float(AT_EPS), arm.gated, true)
                }
                // the CPU mirror: GEMMs, prep, attention, requant, wo
                var qgy : array<float>
                var ky : array<float>
                var vy : array<float>
                if (arm.gated) {
                    ref_q8_gemm(wqg, wsqg, xq, xs, ATD, 2l * ATQD, npos, qgy)
                } else {
                    ref_q8_gemm(wqp, wsqp, xq, xs, ATD, ATQD, npos, qgy)
                }
                ref_q8_gemm(wk, wsk, xq, xs, ATD, ATKVD, npos, ky)
                ref_q8_gemm(wv, wsv, xq, xs, ATD, ATKVD, npos, vy)
                var qp : array<float>
                var gatep : array<float>
                var kp : array<float>
                var kgate_unused : array<float>
                ref_at_prep(qgy, rmsq, cosr, sinr, npos, ATQD, rot, arm.gated, qp, gatep)
                ref_at_prep(ky, rmsk, cosr, sinr, npos, ATKVD, rot, false, kp, kgate_unused)
                var op : array<float>
                ref_at_attn(qp, kp, vy, gatep, npos, arm.gated, op)
                // requant the o rows in the wo stack's form, then the wo GEMM
                var refy : array<float>
                if (arm.o4) {
                    var oq : array<int>
                    var os : array<float>
                    oq |> resize(int(npos * ATQD))
                    os |> resize(int(npos * ATQD / 256l))
                    for (b in range64(npos * ATQD / 256l)) {
                        var amax = 0.0
                        for (j in range64(256l)) {
                            amax = max(amax, abs(op[int(b * 256l + j)]))
                        }
                        let dq = amax / 127.0
                        let id = dq != 0.0 ? 1.0 / dq : 0.0
                        os[int(b)] = dq
                        for (j in range64(256l)) {
                            oq[int(b * 256l + j)] = int(round(op[int(b * 256l + j)] * id))
                        }
                    }
                    refy |> resize(int(npos * ATD))
                    let nsbh = int(ATQD / 256l)
                    for (rw in range(int(npos))) {
                        for (o in range(int(ATD))) {
                            var acc = 0.0lf
                            for (sb in range(nsbh)) {
                                var dd = 0.0lf
                                for (j in range(256)) {
                                    dd += double(wo4[o * int(ATQD) + sb * 256 + j]) * double(oq[rw * int(ATQD) + sb * 256 + j])
                                }
                                acc += dd * double(os[rw * nsbh + sb])
                            }
                            refy[rw * int(ATD) + o] = float(acc)
                        }
                    }
                    delete oq
                    delete os
                } else {
                    var oq : array<int8>
                    var os : array<float>
                    oq |> resize(int(npos * ATQD))
                    os |> resize(int(npos * ATQD / 32l))
                    for (b in range64(npos * ATQD / 32l)) {
                        var amax = 0.0
                        for (j in range64(32l)) {
                            amax = max(amax, abs(op[int(b * 32l + j)]))
                        }
                        let dq = amax / 127.0
                        let id = dq != 0.0 ? 1.0 / dq : 0.0
                        os[int(b)] = dq
                        for (j in range64(32l)) {
                            oq[int(b * 32l + j)] = int8(int(round(op[int(b * 32l + j)] * id)))
                        }
                    }
                    ref_q8_gemm(wo, wso, oq, os, ATQD, ATD, npos, refy)
                    delete oq
                    delete os
                }
                let tag = "npos={npos} rot={rot} gated={arm.gated} o4={arm.o4}"
                let cy = compare_gout(y, refy)
                t |> success(cy.maxref > 0.0, "{tag}: chain produced non-trivial outputs")
                t |> success(cy.maxdiff <= float(AT_REL_TOL) * cy.maxref,
                    "{tag}: out rows within tolerance (maxdiff {cy.maxdiff} vs {float(AT_REL_TOL) * cy.maxref} bar, maxref {cy.maxref})")
                let ck = compare_gout(kout, kp)
                t |> success(ck.maxdiff <= float(AT_REL_TOL) * ck.maxref,
                    "{tag}: roped k rows within tolerance (maxdiff {ck.maxdiff} vs {float(AT_REL_TOL) * ck.maxref} bar)")
                let cv = compare_gout(vout, vy)
                t |> success(cv.maxdiff <= float(AT_REL_TOL) * cv.maxref,
                    "{tag}: v rows within tolerance (maxdiff {cv.maxdiff} vs {float(AT_REL_TOL) * cv.maxref} bar)")
                delete xq
                delete xs
                delete cosr
                delete sinr
                delete y
                delete kout
                delete vout
                delete qgy
                delete ky
                delete vy
                delete qp
                delete gatep
                delete kp
                delete kgate_unused
                delete op
                delete refy
            }
            delete wqg
            delete wsqg
            delete wqp
            delete wsqp
            delete wk
            delete wsk
            delete wv
            delete wsv
            delete wo
            delete wso
            delete wqb
            delete wsb
            delete wo4b
            delete wso4b
            delete wo4
            delete rmsq
            delete rmsk
        } else {
            feint("dasVulkan not installed; vulkan attention chain untestable here\n")
        }
    }
}

// Fused rope + KV-row store, BOTH rope arms — llama's NORM (adjacent pairs) and qwen/gemma's NEOX
// (half-offset). This is the family-ledger kernel: llama is non-NEOX, which the attention rail's
// existing rope arm does not cover. Verifies roped q + the stored K row (roped) + V row (raw).
[test]
def test_vulkan_rope_kv_store(t : T?) {
    t |> run("vulkan rope+kv-store == CPU reference (NORM + NEOX arms)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let HS = 64l        // llama-family head size (the closed-rail case)
            let NH = 4l
            let NKV = 2l
            let QD = NH * HS
            let KVD = NKV * HS
            let half = HS / 2l
            let POS = 7l
            if (!vk_device_ready()) {
                feint("no Vulkan device; rope_kv_store untestable here\n")
                return
            }
            for (arm in range(2)) {
                let neox = arm == 1
                var qkv : array<float>
                qkv |> resize(int(QD + 2l * KVD))
                for (i in range(int(QD + 2l * KVD))) {
                    qkv[i] = float((i % 23) - 11) * 0.1
                }
                var cossin : array<float>
                cossin |> resize(int(HS))
                for (j in range(int(half))) {
                    let ang = float(POS) * (1.0 / pow(10000.0, float(2l * int64(j)) / float(HS)))
                    cossin[j] = cos(ang)
                    cossin[int(half) + j] = sin(ang)
                }
                var qout : array<float>
                var krow : array<float>
                var vrow : array<float>
                qout |> resize(int(QD))
                krow |> resize(int(KVD))
                vrow |> resize(int(KVD))
                vk_rope_kv_store(qkv, qout, krow, vrow, cossin, QD, KVD, HS, POS, neox)
                // CPU reference
                var rq : array<float>
                var rk : array<float>
                var rv : array<float>
                rq |> resize(int(QD))
                rk |> resize(int(KVD))
                rv |> resize(int(KVD))
                for (i in range(int(QD))) { rq[i] = qkv[i] }
                for (i in range(int(KVD))) {
                    rk[i] = qkv[int(QD) + i]
                    rv[i] = qkv[int(QD + KVD) + i]
                }
                // rope rq (NH heads) and rk (NKV heads) in place; same pair rule as the kernel
                for (hh in range(int(NH + NKV))) {
                    let is_q = hh < int(NH)
                    let h = is_q ? hh : hh - int(NH)
                    for (j in range(int(half))) {
                        let e0 = neox ? h * int(HS) + j : h * int(HS) + j * 2
                        let e1 = neox ? h * int(HS) + j + int(half) : h * int(HS) + j * 2 + 1
                        let a0 = is_q ? rq[e0] : rk[e0]
                        let a1 = is_q ? rq[e1] : rk[e1]
                        let r0 = a0 * cossin[j] - a1 * cossin[int(half) + j]
                        let r1 = a0 * cossin[int(half) + j] + a1 * cossin[j]
                        if (is_q) {
                            rq[e0] = r0
                            rq[e1] = r1
                        } else {
                            rk[e0] = r0
                            rk[e1] = r1
                        }
                    }
                }
                let cq = compare_gout(qout, rq)
                let ck = compare_gout(krow, rk)
                let cv = compare_gout(vrow, rv)
                let tag = neox ? "NEOX" : "NORM"
                t |> success(cq.maxdiff <= float(REL_TOL) * cq.maxref + 1e-6,
                    "rope_kv {tag} q roped (maxdiff {cq.maxdiff})")
                t |> success(ck.maxdiff <= float(REL_TOL) * ck.maxref + 1e-6,
                    "rope_kv {tag} K stored roped (maxdiff {ck.maxdiff})")
                t |> success(cv.maxdiff <= 1e-6,
                    "rope_kv {tag} V stored raw (maxdiff {cv.maxdiff})")
                delete qkv
                delete cossin
                delete qout
                delete krow
                delete vrow
                delete rq
                delete rk
                delete rv
            }
        } else {
            feint("dasVulkan not installed; rope_kv_store untestable here\n")
        }
    }
}

// Decode attention over the device K/V mirror. Populates the mirror through vk_rope_kv_store (the
// real producer), capturing each stored row for the CPU reference, then attends with a fresh query
// and checks GQA + online softmax against a plain CPU attention over the captured cache.
[test]
def test_vulkan_decode_attn(t : T?) {
    t |> run("vulkan decode attention == CPU reference (GQA over the device KV mirror)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let HS = 64l
            let NH = 4l
            let NKV = 2l
            let QD = NH * HS
            let KVD = NKV * HS
            let CNT = 5l
            let SCALE = 1.0 / sqrt(float(HS))
            let kv_mul = NH / NKV
            if (!vk_device_ready()) {
                feint("no Vulkan device; decode_attn untestable here\n")
                return
            }
            var cpuK : array<float>
            var cpuV : array<float>
            cpuK |> resize(int(CNT * KVD))
            cpuV |> resize(int(CNT * KVD))
            // build the cache one position at a time via the real rope_kv_store producer
            for (p in range(int(CNT))) {
                var qkv : array<float>
                qkv |> resize(int(QD + 2l * KVD))
                for (i in range(int(QD + 2l * KVD))) {
                    qkv[i] = float(((i + p * 13) % 19) - 9) * 0.1
                }
                var cossin : array<float>
                cossin |> resize(int(HS))
                for (j in range(int(HS / 2l))) {
                    let ang = float(p) * (1.0 / pow(10000.0, float(2 * j) / float(HS)))
                    cossin[j] = cos(ang)
                    cossin[int(HS / 2l) + j] = sin(ang)
                }
                var qout : array<float>
                var krow : array<float>
                var vrow : array<float>
                qout |> resize(int(QD))
                krow |> resize(int(KVD))
                vrow |> resize(int(KVD))
                vk_rope_kv_store(qkv, qout, krow, vrow, cossin, QD, KVD, HS, int64(p), false)
                for (i in range(int(KVD))) {
                    cpuK[p * int(KVD) + i] = krow[i]
                    cpuV[p * int(KVD) + i] = vrow[i]
                }
                delete qkv
                delete cossin
                delete qout
                delete krow
                delete vrow
            }
            // a fresh query, then decode attention over the cnt cached rows
            var q : array<float>
            q |> resize(int(QD))
            for (i in range(int(QD))) {
                q[i] = float((i % 7) - 3) * 0.2
            }
            var out : array<float>
            out |> resize(int(QD))
            vk_decode_attn(q, out, CNT, QD, KVD, HS, kv_mul, SCALE)
            // CPU reference: per head, softmax(q.k*scale) then weighted V
            var refo : array<float>
            refo |> resize(int(QD))
            for (h in range(int(NH))) {
                let kvh = h / int(kv_mul)
                var sc : array<float>
                sc |> resize(int(CNT))
                var mx = -1.0e30
                for (p in range(int(CNT))) {
                    var s = 0.0
                    for (d in range(int(HS))) {
                        s += q[h * int(HS) + d] * cpuK[p * int(KVD) + kvh * int(HS) + d]
                    }
                    s *= SCALE
                    sc[p] = s
                    mx = max(mx, s)
                }
                var den = 0.0
                for (p in range(int(CNT))) {
                    sc[p] = exp(sc[p] - mx)
                    den += sc[p]
                }
                for (d in range(int(HS))) {
                    var acc = 0.0
                    for (p in range(int(CNT))) {
                        acc += sc[p] * cpuV[p * int(KVD) + kvh * int(HS) + d]
                    }
                    refo[h * int(HS) + d] = float(acc / den)
                }
                delete sc
            }
            let ca = compare_gout(out, refo)
            t |> success(ca.maxdiff <= float(REL_TOL) * ca.maxref + 1e-5,
                "decode attention within tolerance (maxdiff {ca.maxdiff} vs {float(REL_TOL) * ca.maxref} bar, maxref {ca.maxref})")
            delete cpuK
            delete cpuV
            delete q
            delete out
            delete refo
        } else {
            feint("dasVulkan not installed; decode_attn untestable here\n")
        }
    }
}

// Arena batch GEMM (prefill twin of the arena GEMV): npos rows through q8_moe_batch against an
// arena-resident plane, vs a CPU reference. Two tensors again so the block offset must be honored.
[test]
def test_vulkan_arena_gemm(t : T?) {
    t |> run("vulkan arena batch GEMM == CPU reference (npos rows, distinct blocks)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let GROWS = 64l
            let GNP = 40l    // npos, matches the existing batch arms (>= BATCH_TILE, unpadded)
            if (!vk_arena_reserve(0, ARENA_BLOCKS)) {
                feint("no Vulkan device; arena batch GEMM untestable here\n")
                return
            }
            var wqa : array<int8>
            var wsa : array<float>
            var wqbv : array<int8>
            var wsbv : array<float>
            fill_stack(wqa, wsa, GROWS, N, 41l, false)
            fill_stack(wqbv, wsbv, GROWS, N, 67l, false)
            var pqa : array<uint8>
            var psa : array<uint8>
            var pqb : array<uint8>
            var psb : array<uint8>
            pack_upload(wqa, wsa, pqa, psa)
            pack_upload(wqbv, wsbv, pqb, psb)
            let gblk_a = vk_arena_place(pqa, psa, N, GROWS, 0)
            let gblk_b = vk_arena_place(pqb, psb, N, GROWS, 0)
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, GNP)
            let nb = int(N / 32l)
            for (arm in range(2)) {
                let blk = arm == 0 ? gblk_a : gblk_b
                var y : array<float>
                y |> resize(int(GNP * GROWS))
                vk_arena_gemm(y, blk, N, GROWS, GNP, 0, xq, xs)
                var refy : array<float>
                refy |> resize(int(GNP * GROWS))
                for (r in range(int(GNP))) {
                    for (col in range(int(GROWS))) {
                        var acc = 0.0lf
                        for (b in range(nb)) {
                            var id = 0
                            for (k in range(32)) {
                                let wi = col * int(N) + b * 32 + k
                                id += int(xq[r * int(N) + b * 32 + k]) * int(arm == 0 ? wqa[wi] : wqbv[wi])
                            }
                            let si = col * nb + b
                            acc += double(arm == 0 ? wsa[si] : wsbv[si]) * double(xs[r * nb + b]) * double(id)
                        }
                        refy[r * int(GROWS) + col] = float(acc)
                    }
                }
                let cg = compare_gout(y, refy)
                t |> success(cg.maxdiff <= float(REL_TOL) * cg.maxref,
                    "arena GEMM arm {arm} within tolerance (maxdiff {cg.maxdiff} vs {float(REL_TOL) * cg.maxref} bar)")
                delete y
                delete refy
            }
            delete wqa
            delete wsa
            delete wqbv
            delete wsbv
            delete pqa
            delete psa
            delete pqb
            delete psb
            delete xq
            delete xs
        } else {
            feint("dasVulkan not installed; arena batch GEMM untestable here\n")
        }
    }
}

// The whole dense FFN against arena-resident planes: gate GEMV -> up GEMV -> fused act+requant ->
// down GEMV, ONE submit. No new kernels — q8_moe_actrq is already region-free, so this is the
// per-op chain with the weight base coming from an arena block index instead of a stack.
[test]
def test_vulkan_arena_ffn(t : T?) {
    t |> run("vulkan arena FFN chain == CPU reference (dense gate/up/down triple)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_arena_reserve(0, ARENA_BLOCKS)) {
                feint("no Vulkan device (or the arena reserve was refused); skipping\n")
                return
            }
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_stack(wq1, ws1, NFE, N, 11l, true)    // gate/up small scales — the sane silu regime
            fill_stack(wq3, ws3, NFE, N, 23l, true)
            fill_stack(wq2, ws2, N, NFE, 37l, false)
            var p1 : array<uint8>
            var s1 : array<uint8>
            var p3 : array<uint8>
            var s3 : array<uint8>
            var p2 : array<uint8>
            var s2 : array<uint8>
            pack_upload(wq1, ws1, p1, s1)
            pack_upload(wq3, ws3, p3, s3)
            pack_upload(wq2, ws2, p2, s2)
            let b1 = vk_arena_place(p1, s1, N, NFE, 0)
            let b3 = vk_arena_place(p3, s3, N, NFE, 0)
            let b2 = vk_arena_place(p2, s2, NFE, N, 0)
            t |> success(b1 >= 0l && b3 >= 0l && b2 >= 0l, "arena admitted the FFN triple (blocks {b1}/{b3}/{b2})")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 1l)
            var y : array<float>
            y |> resize(int(N))
            vk_arena_ffn(y, b1, b3, b2, N, NFE, 0, 0, 0, xq, xs, false)
            // CPU mirror: gate/up GEMV -> silu(g)*u -> q8_0 requant -> down GEMV
            let nb = int(N / 32l)
            let nbh = int(NFE / 32l)
            var h : array<float>
            var hq : array<int>
            var hs : array<float>
            h |> resize(int(NFE))
            hq |> resize(int(NFE))
            hs |> resize(nbh)
            for (i in range(int(NFE))) {
                var ag = 0.0
                var au = 0.0
                for (b in range(nb)) {
                    var dg = 0
                    var du = 0
                    for (j in range(32)) {
                        let xv = int(xq[b * 32 + j])
                        dg += int(wq1[i * int(N) + b * 32 + j]) * xv
                        du += int(wq3[i * int(N) + b * 32 + j]) * xv
                    }
                    let xscl = xs[b]
                    ag += float(dg) * ws1[i * nb + b] * xscl
                    au += float(du) * ws3[i * nb + b] * xscl
                }
                h[i] = (ag / (1.0 + exp(-ag))) * au
            }
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
            var refy : array<float>
            refy |> resize(int(N))
            for (o in range(int(N))) {
                var acc = 0.0
                for (b in range(nbh)) {
                    var dd = 0
                    for (j in range(32)) {
                        dd += int(wq2[o * int(NFE) + b * 32 + j]) * hq[b * 32 + j]
                    }
                    acc += float(dd) * ws2[o * nbh + b] * hs[b]
                }
                refy[o] = acc
            }
            let cf = compare_gout(y, refy)
            t |> success(cf.maxdiff <= float(REL_TOL) * cf.maxref,
                "arena FFN within tolerance (maxdiff {cf.maxdiff} vs {float(REL_TOL) * cf.maxref} bar, maxref {cf.maxref})")
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
            delete p1
            delete s1
            delete p3
            delete s3
            delete p2
            delete s2
            delete xq
            delete xs
            delete y
            delete h
            delete hq
            delete hs
            delete refy
        } else {
            feint("dasVulkan not installed; arena FFN untestable here\n")
        }
    }
}

// The resident driver's fused residual-add + RMSNorm — the kernel that collapses the layer
// boundary to one dispatch. ascale != 1 and a non-zero addend on purpose: with ascale = 1 or a = 0
// a dropped term would still land inside tolerance.
// The in-place residual check is NOT redundant with the y check and must not be dropped: RMSNorm
// is SCALE-INVARIANT, so a lost uniform factor (ascale) divides straight back out of y and is
// invisible there — mutation-probed, y stayed in tolerance while x was off by 1.125 on a 0.003 bar.
[test]
def test_vulkan_add_rms(t : T?) {
    t |> run("vulkan add+rms == CPU reference (fused arm + norm-only arm)") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let AD = 512l
            let EPS = 1e-5
            let ASCALE = 0.75
            for (arm in range(2)) {
                let add_on = arm == 0
                var x : array<float>
                var a : array<float>
                var w : array<float>
                var y : array<float>
                x |> resize(int(AD))
                a |> resize(int(AD))
                w |> resize(int(AD))
                y |> resize(int(AD))
                for (i in range(int(AD))) {
                    x[i] = float((i % 17) - 8) * 0.25
                    a[i] = float((i % 11) - 5) * 0.5
                    w[i] = 1.0 + float(i % 7) * 0.125
                }
                // CPU reference over the pre-call x
                var vv : array<float>
                vv |> resize(int(AD))
                var ss = 0.0lf
                for (i in range(int(AD))) {
                    vv[i] = add_on ? (x[i] + a[i]) * ASCALE : x[i]
                    ss += double(vv[i]) * double(vv[i])
                }
                let inv = 1.0lf / sqrt(ss / double(AD) + double(EPS))
                var refy : array<float>
                refy |> resize(int(AD))
                for (i in range(int(AD))) {
                    refy[i] = float(double(w[i]) * double(vv[i]) * inv)
                }
                vk_add_rms(x, a, w, y, AD, EPS, ASCALE, add_on)
                let cy = compare_gout(y, refy)
                t |> success(cy.maxdiff <= float(REL_TOL) * cy.maxref,
                    "add_rms arm {arm} y within tolerance (maxdiff {cy.maxdiff} vs {float(REL_TOL) * cy.maxref} bar)")
                // x IS the residual stream — the kernel must have updated it in place
                let cx = compare_gout(x, vv)
                t |> success(cx.maxdiff <= float(REL_TOL) * cx.maxref,
                    "add_rms arm {arm} residual updated in place (maxdiff {cx.maxdiff} vs {float(REL_TOL) * cx.maxref} bar)")
                delete x
                delete a
                delete w
                delete y
                delete vv
                delete refy
            }
        } else {
            feint("dasVulkan not installed; add_rms untestable here\n")
        }
    }
}

// The resident driver's weight arena: many tensors in ONE buffer pair per format, addressed by
// block index, so a whole dense model costs one descriptor set per format instead of one per plane
// (the MAX_STACKS cap). TWO tensors on purpose — a single one lands at block 0 and would pass even
// if the arena base were ignored entirely; the second only matches if its offset is honored.
[test]
def test_vulkan_arena(t : T?) {
    t |> run("vulkan resident arena == CPU reference (two q8 tensors, distinct block offsets)") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let AROWS = 64l
            let ABLK = AROWS * (N / 32l)   // blocks one [AROWS x N] q8 tensor occupies
            if (!vk_arena_reserve(0, ARENA_BLOCKS)) {
                feint("no Vulkan device (or the arena reserve was refused); skipping\n")
                return
            }
            var wqa : array<int8>
            var wsa : array<float>
            var wqbv : array<int8>
            var wsbv : array<float>
            fill_stack(wqa, wsa, AROWS, N, 31l, false)
            fill_stack(wqbv, wsbv, AROWS, N, 57l, false)   // different seed => different content
            var pqa : array<uint8>
            var psa : array<uint8>
            var pqb : array<uint8>
            var psb : array<uint8>
            pack_upload(wqa, wsa, pqa, psa)
            pack_upload(wqbv, wsbv, pqb, psb)
            let blk_a = vk_arena_place(pqa, psa, N, AROWS, 0)
            let blk_b = vk_arena_place(pqb, psb, N, AROWS, 0)
            // relative, not absolute: the arena is process-global, so another arm may have placed
            // into it first. Back-to-back is the actual invariant.
            tt |> success(blk_a >= 0l && blk_b == blk_a + ABLK,
                "arena bump cursor packs tensors back to back (a={blk_a}, b={blk_b}, delta expected {ABLK})")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 1l)
            let nb = int(N / 32l)
            for (arm in range(2)) {
                let blk = arm == 0 ? blk_a : blk_b
                var y : array<float>
                y |> resize(int(AROWS))
                vk_arena_gemv(y, blk, N, AROWS, 0, xq, xs)
                var refy : array<float>
                refy |> resize(int(AROWS))
                for (c in range(int(AROWS))) {
                    var acc = 0.0lf
                    for (b in range(nb)) {
                        var id = 0
                        for (k in range(32)) {
                            let wi = c * int(N) + b * 32 + k
                            id += int(xq[b * 32 + k]) * int(arm == 0 ? wqa[wi] : wqbv[wi])
                        }
                        let si = c * nb + b
                        acc += double(arm == 0 ? wsa[si] : wsbv[si]) * double(xs[b]) * double(id)
                    }
                    refy[c] = float(acc)
                }
                let cc = compare_gout(y, refy)
                tt |> success(cc.maxdiff <= float(REL_TOL) * cc.maxref,
                    "arena tensor {arm} within tolerance (maxdiff {cc.maxdiff} vs {float(REL_TOL) * cc.maxref} bar, maxref {cc.maxref})")
                delete y
                delete refy
            }
            delete wqa
            delete wsa
            delete wqbv
            delete wsbv
            delete pqa
            delete psa
            delete pqb
            delete psb
            delete xq
            delete xs
        } else {
            feint("dasVulkan not installed; resident arena untestable here\n")
        }
    }
}

// ===== whole-model drop/re-arm (the multi-model switch primitive) =====

// vk_drop_model_state (via the math-side hook) destroys EVERY model-owned object — stacks,
// arenas, lazy dispatch families, their descriptor sets and command buffers — while the device
// survives. Two full cycles re-upload the SAME offsets with DIFFERENT content: a stale stack
// registry or arena would resolve the old plane and fail the cycle's own reference. Declared
// LAST in the file on purpose: the drop nukes every stack earlier tests uploaded.
let DMWOFF = 50000000l
let DMROWS = 64l

[test]
def test_vulkan_drop_model(t : T?) {
    t |> run("vulkan whole-model drop — stacks + arena rebuild and serve across two cycles") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            let nb = int(N / 32l)
            for (cycle in range(2)) {
                // per-op stack rail: upload -> dense GEMV -> parity vs THIS cycle's content
                var wq : array<int8>
                var ws : array<float>
                fill_stack(wq, ws, DMROWS, N, 17l + int64(cycle) * 31l, false)
                var wqb : array<uint8>
                var wsb : array<uint8>
                pack_upload(wq, ws, wqb, wsb)
                if (!moe_gpu_upload_stack(wqb, wsb, DMWOFF, N, DMROWS, 0)) {
                    feint("no Vulkan device (or probe/budget refused the upload); skipping\n")
                    return
                }
                var xq : array<int8>
                var xs : array<float>
                fill_acts(xq, xs, 2l)
                var y : array<float>
                y |> resize(int(2l * DMROWS))
                matmul_gpu_dense(y, DMWOFF, xq, xs, N, DMROWS, 2l, 0)
                var refy : array<float>
                refy |> resize(int(2l * DMROWS))
                for (r in range(2)) {
                    for (c in range(int(DMROWS))) {
                        var acc = 0.0lf
                        for (b in range(nb)) {
                            var id = 0
                            for (k in range(32)) {
                                id += int(xq[r * int(N) + b * 32 + k]) * int(wq[c * int(N) + b * 32 + k])
                            }
                            acc += double(ws[c * nb + b]) * double(xs[r * nb + b]) * double(id)
                        }
                        refy[r * int(DMROWS) + c] = float(acc)
                    }
                }
                let cs = compare_gout(y, refy)
                t |> success(cs.maxdiff <= float(REL_TOL) * cs.maxref,
                    "cycle {cycle} stack serves its own content (maxdiff {cs.maxdiff} vs {float(REL_TOL) * cs.maxref} bar)")
                // arena rail: reserve (fresh after a drop) -> place -> GEMV -> parity
                t |> success(vk_arena_reserve(0, ARENA_BLOCKS / 2l), "cycle {cycle} arena reserve")
                var awq : array<int8>
                var aws : array<float>
                fill_stack(awq, aws, DMROWS, N, 91l + int64(cycle) * 13l, false)
                var apq : array<uint8>
                var aps : array<uint8>
                pack_upload(awq, aws, apq, aps)
                let blk = vk_arena_place(apq, aps, N, DMROWS, 0)
                t |> success(blk >= 0l, "cycle {cycle} arena placement (block {blk})")
                var axq : array<int8>
                var axs : array<float>
                fill_acts(axq, axs, 1l)
                var ay : array<float>
                ay |> resize(int(DMROWS))
                vk_arena_gemv(ay, blk, N, DMROWS, 0, axq, axs)
                var aref : array<float>
                aref |> resize(int(DMROWS))
                for (c in range(int(DMROWS))) {
                    var acc = 0.0lf
                    for (b in range(nb)) {
                        var id = 0
                        for (k in range(32)) {
                            id += int(axq[b * 32 + k]) * int(awq[c * int(N) + b * 32 + k])
                        }
                        acc += double(aws[c * nb + b]) * double(axs[b]) * double(id)
                    }
                    aref[c] = float(acc)
                }
                let ca = compare_gout(ay, aref)
                t |> success(ca.maxdiff <= float(REL_TOL) * ca.maxref,
                    "cycle {cycle} arena serves its own content (maxdiff {ca.maxdiff} vs {float(REL_TOL) * ca.maxref} bar)")
                // the whole-model drop: every stack (this file's earlier tests included), the
                // arena, and all lazy dispatch state die; cycle 1 re-runs the ladder from bare
                moe_gpu_drop_model_state()
                t |> equal(0l, gpu_tier_status().vram_bytes, "cycle {cycle} drop left zero resident bytes")
                delete wq
                delete ws
                delete wqb
                delete wsb
                delete xq
                delete xs
                delete y
                delete refy
                delete awq
                delete aws
                delete apq
                delete aps
                delete axq
                delete axs
                delete ay
                delete aref
            }
        } else {
            feint("dasVulkan not installed; whole-model drop untestable here\n")
        }
    }
}
