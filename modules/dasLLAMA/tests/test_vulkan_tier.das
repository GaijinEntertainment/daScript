options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, hook install, dispatch, CPU-reference compare
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math
require ?vulkan dasllama/dasllama_rope   // build_rope_row_packed: the rope cells' cos|sin row
require ?vulkan dasllama/dasllama_math_vulkan  // the MoE GPU tier under test; the test installs its hooks explicitly (no env gate)
require ?vulkan dasllama/dasllama_vulkan_seams  // the single-op seams (vk_add_rms / rope_kv / decode_attn) on the class rail
require math
require _vk_kq_fixtures   // the synthetic kq stacks + pow2 scale sets (shared with test_vulkan_moe_cm2)
require _kq_fixtures
require _kernel_oracles
require _compares   // check: every compare's rel_of_max bar, largest-difference log and poisoned-element control
require _attn_oracle   // attn_row_oracle: the attention cells' softmax rows

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
// A check skips without dasVulkan in the build, with no Vulkan device, or on an upload the
// probe or budget refused.

// The CPU-reference arms' tolerances were derived against the sdot4 reference kernels — pin
// that mode ahead of the router's resolved default (cm2 or mm) so the oracles stay deterministic.
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

//! `nrows` activation rows `width` wide: quants (i * mul + add) % 255 - 127, a `pow2_small` scale per `blk` from `sseed`
def private fill_acts_of(var xq : array<int8>; var xs : array<float>; nrows, width, blk, mul, add, sseed : int64) {
    xq |> resize(nrows * width)
    xs |> resize(nrows * (width / blk))
    for (i in range64(nrows * width)) {
        xq[i] = int8((i * mul + add) % 255l - 127l)
    }
    for (i in range64(nrows * (width / blk))) {
        xs[i] = pow2_small(i + sseed)
    }
}

def private fill_acts(var xq : array<int8>; var xs : array<float>; nrows : int64) {
    fill_acts_of(xq, xs, nrows, N, 32l, 11l, 3l, 2l)
}

//! a stack triple's base element offsets: gate, up, down
struct private Bases {
    gate : int64
    up : int64
    down : int64
}

let WOFFS = Bases(gate = WOFF1, up = WOFF3, down = WOFF2)

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

struct private OffsTriple {
    gate : array<int64>
    up : array<int64>
    down : array<int64>
}

//! the gate, up and down region lists of one expert / row partition over `woffs`' stacks (gate and up nfe x n, down n x nfe)
def private offs_triple(experts, row0s, cnts : array<int64>; woffs : Bases; n, nfe : int64) : OffsTriple {
    return <- OffsTriple(gate <- make_offs(experts, row0s, cnts, woffs.gate, nfe * n), up <- make_offs(experts, row0s, cnts, woffs.up, nfe * n),
        down <- make_offs(experts, row0s, cnts, woffs.down, n * nfe))
}

//! the two FFN arm shapes' regions: decode = 2 rows, one each over experts 1 and 2 (the engine's k = 2 GEMV shape); batch = 40
//! rows (>= the 32-row tile, deliberately unpadded), 24 over expert 0 and 16 over expert 2
def private region_triple(decode : bool; woffs : Bases; n, nfe : int64) : OffsTriple {
    return <- decode ? offs_triple([1l, 2l], [0l, 1l], [1l, 1l], woffs, n, nfe) : offs_triple([0l, 2l], [0l, 24l], [24l, 16l], woffs, n, nfe)
}

//! One FFN arm through matmul_moe_gpu_ffn: the decode or the batch shape over the stacks at `woffs` in formats `fmts` (gate, up,
//! down; the activation rows follow the gate's form), against `refer`'s reference over the same offsets. The device rows.
def private ffn_arm(t : T?; tag : string; decode : bool; woffs : Bases; n, nfe : int64; fmts : int3;
                    refer : block<(offs1, offs3, offs2 : array<int64>; xq : array<int8>; xs : array<float>; nrows : int64; var refv : array<float>) : void>) : array<float> {
    let nrows = decode ? 2l : 40l
    var xq : array<int8>
    var xs : array<float>
    if (fmts.x == 0) {
        fill_acts(xq, xs, nrows)
    } else {
        fill_acts_kq(xq, xs, nrows)
    }
    var o <- region_triple(decode, woffs, n, nfe)
    var gout : array<float>
    gout |> resize(nrows * n)
    matmul_moe_gpu_ffn(gout, o.gate, o.up, o.down, 2l, xq, xs, n, nfe, nrows, fmts.x, fmts.y, fmts.z, false)
    var refv : array<float>
    invoke(refer, o.gate, o.up, o.down, xq, xs, nrows, refv)
    let arm = decode ? "decode" : "batch"
    held_nontrivial(t, "{tag} {arm} arm", gout, refv, REL_TOL, n)
    delete xq
    delete xs
    delete o
    delete refv
    return <- gout
}

//! position p's combined row: the weighted sum of its k = 4 slots' rows of `gout`, slot j at row inv[p * 4 + j] with weight cw[p * 4 + j]
def private combine_ref(gout, cw : array<float>; cinv : array<uint>; npos : int64) : array<float> {
    var combref : array<float>
    combref |> resize(npos * N)
    for (p in range64(npos)) {
        for (i in range64(N)) {
            var acc = 0.0
            for (j in range64(4l)) {
                acc += cw[p * 4l + j] * gout[int64(cinv[p * 4l + j]) * N + i]
            }
            combref[p * N + i] = acc
        }
    }
    return <- combref
}

//! the CPU reference of the tier's fused-FFN contract; a region's expert row is its offset past its stack's base in `bases`
def private ref_ffn(offs1, offs3, offs2 : array<int64>; bases : Bases; nreg : int;
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
    gate |> resize(NFE)
    up |> resize(NFE)
    h |> resize(NFE)
    for (r in range(nreg)) {
        let rb1 = int((offs1[r * 3] - bases.gate) / N)      // first gate-plane row of this region's expert
        let rb3 = int((offs3[r * 3] - bases.up) / N)
        let rb2 = int((offs2[r * 3] - bases.down) / NFE)
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
                h[i] = silu_f32(gate[i]) * up[i]
            }
            var hr <- q8_roundtrip(h, NFE, false)
            for (o in range(nn)) {
                var y = 0.0
                for (b in range(nbh)) {
                    var dd = 0
                    for (j in range(32)) {
                        dd += int(wq2[(rb2 + o) * int(NFE) + b * 32 + j]) * int(hr.xq[b * 32 + j])
                    }
                    y += float(dd) * ws2[(rb2 + o) * nbh + b] * hr.xs[b]
                }
                gout[rw * int(N) + o] = y
            }
            delete hr
        }
    }
}

//! `got` within `rel` of `want`'s largest magnitude, plus `abs_add` absolute where a site's bar carries one, its largest
//! difference logged by row (`row_width` wide); the control moves the last expected element by 1 + 2 x that magnitude. The count off.
def private held(t : T?; tag : string; got, want : array<float>; rel : float; row_width : int64; abs_add : float = 0.0) : int {
    let bar = (abs_add == 0.0 ? Bar(kind = BarKind.rel_of_max, rel = rel, poison = 2.0, poison_of_max = true, poison_abs = 1.0)
        : Bar(kind = BarKind.rel_of_max, abs_floor = rel * logits_maxabs(want) + abs_add, poison = 2.0, poison_of_max = true, poison_abs = 1.0))
    return check(t, tag, got, want, bar, Shape(row_width = int(row_width)))
}

//! `held` against a reference that must be non-trivial
def private held_nontrivial(t : T?; tag : string; got, want : array<float>; rel : float; row_width : int64) : int {
    t |> success(logits_maxabs(want) > 0.0, "{tag}: the reference is non-trivial")
    return held(t, tag, got, want, rel, row_width)
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
            fill_q8_quants(wq1, ws1, NE * NFE, N, 0l, true)
            fill_q8_quants(wq3, ws3, NE * NFE, N, 5l, true)
            fill_q8_quants(wq2, ws2, NE * N, NFE, 9l, false)
            if (!q8_upload(wq1, ws1, WOFF1, N, NE * NFE)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            t |> success(q8_upload(wq3, ws3, WOFF3, N, NE * NFE), "up stack uploaded")
            t |> success(q8_upload(wq2, ws2, WOFF2, NFE, NE * N), "down stack uploaded")
            var gout : array<float>   //! the batch arm's device rows: the combined arms' input
            for (decode in fixed_array(true, false)) {
                var arm_gout <- ffn_arm(t, "q8", decode, WOFFS, N, NFE, int3(0)) $(o1, o3, o2, xq, xs, nrows, refv) {
                    ref_ffn(o1, o3, o2, WOFFS, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, nrows, refv)
                }
                delete gout
                gout <- arm_gout
            }

            // combined arm: same batch fixture, the routed weighted-sum reduce on the device —
            // an npos=10 x k=4 grid over the 40 bucket rows via a bijective inv map, judged
            // against the same combine of the GPU's OWN per-row gout (isolates the combine op)
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 40l)
            var bo <- region_triple(false, WOFFS, N, NFE)
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
            matmul_moe_gpu_ffn_combined(comb, bo.gate, bo.up, bo.down, 2l, xq, xs, cw, cinv, N, NFE, 40l, 10l, 0, 0, 0, false)
            var combref <- combine_ref(gout, cw, cinv, 10l)
            held_nontrivial(t, "combined arm", comb, combref, REL_TOL, N)

            // two-chunk combined arm: 8200 rows overflow the 8192-row chunk cap, so the combine
            // accumulates across two dispatches AND region B straddles the chunk boundary; a
            // position's slots land in both chunks (the cross-chunk r0 > 0 path)
            let CNK = 8200l
            fill_acts(xq, xs, CNK)
            var co <- offs_triple([0l, 2l], [0l, 4096l], [4096l, CNK - 4096l], WOFFS, N, NFE)
            gout |> resize(int(CNK * N))
            matmul_moe_gpu_ffn(gout, co.gate, co.up, co.down, 2l, xq, xs, N, NFE, CNK, 0, 0, 0, false)
            cw |> resize(int(CNK))
            cinv |> resize(int(CNK))
            for (g in range(int(CNK))) {
                cw[g] = 0.5 + 0.0001 * float(g)
                cinv[g] = uint((g * 7 + 3) % int(CNK))
            }
            let CNP = CNK / 4l
            comb |> resize(int(CNP * N))
            matmul_moe_gpu_ffn_combined(comb, co.gate, co.up, co.down, 2l, xq, xs, cw, cinv, N, NFE, CNK, CNP, 0, 0, 0, false)
            delete combref
            combref <- combine_ref(gout, cw, cinv, CNP)
            held_nontrivial(t, "two-chunk combined arm", comb, combref, REL_TOL, N)
            delete cw
            delete cinv
            delete comb
            delete combref
            delete co
            delete bo
            delete gout
            delete xq
            delete xs
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
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
            var streamed_ok = true
            for (g in range(3)) {
                fill_q8_quants(wq1[g], ws1[g], NE * NFE, N, int64(g) * 31l + 2l, true)
                fill_q8_quants(wq3[g], ws3[g], NE * NFE, N, int64(g) * 31l + 7l, true)
                fill_q8_quants(wq2[g], ws2[g], NE * N, NFE, int64(g) * 31l + 11l, false)
                let base = SWOFF + int64(g) * 3l * SSTRIDE
                let ok1 = q8_upload(wq1[g], ws1[g], base, N, NE * NFE, true)
                let ok3 = q8_upload(wq3[g], ws3[g], base + SSTRIDE, N, NE * NFE, true)
                let ok2 = q8_upload(wq2[g], ws2[g], base + 2l * SSTRIDE, NFE, NE * N, true)
                streamed_ok = streamed_ok && ok1 && ok3 && ok2
            }
            if (!streamed_ok) {
                t |> skip("no Vulkan device (or probe refused the stream uploads)")
                return
            }
            for (pass_i in [2, 1, 0, 2]) {   // runtime order + an eviction reload of group 2
                let g = pass_i
                let base = SWOFF + int64(g) * 3l * SSTRIDE
                let woffs = Bases(gate = base, up = base + SSTRIDE, down = base + 2l * SSTRIDE)
                var gout <- ffn_arm(t, "streamed group {g}", false, woffs, N, NFE, int3(0)) $(o1, o3, o2, xq, xs, nrows, refv) {
                    ref_ffn(o1, o3, o2, woffs, 2, wq1[g], ws1[g], wq3[g], ws3[g], wq2[g], ws2[g], xq, xs, nrows, refv)
                }
                delete gout
            }
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
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
            set_gpu_tier_want_(want)
            var wq1 : array<int8>
            var ws1 : array<float>
            var wq3 : array<int8>
            var ws3 : array<float>
            var wq2 : array<int8>
            var ws2 : array<float>
            fill_q8_quants(wq1, ws1, NE * NFE, N, 51l, true)
            fill_q8_quants(wq3, ws3, NE * NFE, N, 57l, true)
            fill_q8_quants(wq2, ws2, NE * N, NFE, 61l, false)
            let ok1 = q8_upload(wq1, ws1, HWOFF, N, NE * NFE, true)
            let ok3 = q8_upload(wq3, ws3, HWOFF + HSTRIDE, N, NE * NFE, true)
            let ok2 = q8_upload(wq2, ws2, HWOFF + 2l * HSTRIDE, NFE, NE * N, true)
            if (!(ok1 && ok3 && ok2)) {
                t |> skip("no Vulkan device (or probe refused the stream uploads)")
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
            var ro <- offs_triple([0l, 2l], [0l, 1l], [1l, 1l], WOFFS, N, NFE)
            var refv : array<float>
            ref_ffn(ro.gate, ro.up, ro.down, WOFFS, 2, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 2l, refv)
            var gout : array<float>
            gout |> resize(int(2l * N))
            matmul_moe_gpu_ffn_join(gout)
            held_nontrivial(t, "async hit dispatch", gout, refv, REL_TOL, N)
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
            var po <- offs_triple([1l], [0l], [1l], WOFFS, N, NFE)
            ref_ffn(po.gate, po.up, po.down, WOFFS, 1, wq1, ws1, wq3, ws3, wq2, ws2, xq, xs, 1l, refv)
            gout |> resize(int(N))
            matmul_moe_gpu_ffn_join(gout)
            held_nontrivial(t, "post-eviction hit dispatch", gout, refv, REL_TOL, N)
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
            delete ro
            delete po
            delete refv
            delete gout
            delete wq1
            delete ws1
            delete wq3
            delete ws3
            delete wq2
            delete ws2
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
        }
    }
}

// ===== native K-quant arms =====

let KN = 512l        // kq model dim (TWO superblocks per row — nsb = 1 hides stride bugs)
let KNFE = 512l      // kq expert FFN dim (two superblocks; the requant/down walk crosses one)
let KWOFF1 = 5000000l
let KWOFF3 = 6000000l
let KWOFF2 = 7000000l

def private fill_acts_kq(var xq : array<int8>; var xs : array<float>; nrows : int64) {
    fill_acts_of(xq, xs, nrows, KN, 256l, 13l, 5l, 4l)
}

//! A dequantized-weight GEMM mirror (`dense_ref_rows` in double over the dequantized activations): y [nrows x d] = X . W^T over
//! `nrows` activation rows of `n` quants `xq` (a scale per 256 in `xs`) and `d` dequantized weight rows `w`.
def private ref_deq_gemm(w : array<float>; xq : array<auto(QT)>; xs : array<float>; n, d, nrows : int64) : array<float> {
    let x <- [for (i in range(int(nrows * n))); float(xq[i]) * xs[i / 256]]
    var r <- dense_ref_rows(w, x, int(nrows), int(n), int(d))
    return <- r.want
}

// the CPU reference of the kq FFN contract: dequantized weights against the Q8_K activation
// image (per-256 scales), silu, per-256 Q8_K requant, dequantized-down GEMM — double
// accumulation, judged by the same relative-tolerance bar as the q8 arms
// midblk = the INTERMEDIATE's requant block: 256 when down is a K-quant, 32 when down is q8.
// The gate/up dots always ride the kq 256-block activation image; only the h -> down hop changes.
// Each region's expert row is its offset past its stack's base in `bases`.
def private ref_ffn_kq(offs1, offs3, offs2 : array<int64>; bases : Bases; nreg : int;
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
    gate |> resize(KNFE)
    up |> resize(KNFE)
    h |> resize(KNFE)
    for (r in range(nreg)) {
        let rb1 = int((offs1[r * 3] - bases.gate) / KN)
        let rb3 = int((offs3[r * 3] - bases.up) / KN)
        let rb2 = int((offs2[r * 3] - bases.down) / KNFE)
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
                h[i] = silu_f32(gate[i]) * up[i]
            }
            var hr <- q8_roundtrip(h, KNFE, midblk == 256)
            for (o in range(kn)) {
                var y = 0.0lf
                for (sb in range(nsbh)) {
                    var dd = 0.0lf
                    for (j in range(midblk)) {
                        dd += double(w2[(rb2 + o) * int(KNFE) + sb * midblk + j]) * double(hr.xq[sb * midblk + j])
                    }
                    y += dd * double(hr.xs[sb])
                }
                gout[rw * int(KN) + o] = float(y)
            }
            delete hr
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
            var w1 : array<float>
            var w3 : array<float>
            if (!plane_up(1, NE * KNFE, KN, 21l, KWOFF1, w1)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            t |> success(plane_up(1, NE * KNFE, KN, 37l, KWOFF3, w3), "k4 up stack uploaded")
            for (dfmt in [3, 2]) {   // k6 down, then k5 down (fresh stack, same gate/up)
                let dwoff = KWOFF2 + int64(3 - dfmt) * 10000000l
                var w2 : array<float>
                t |> success(plane_up(dfmt, NE * KN, KNFE, 53l + int64(dfmt), dwoff, w2, false), "k{dfmt == 3 ? 6 : 5} down stack uploaded")
                let woffs = Bases(gate = KWOFF1, up = KWOFF3, down = dwoff)
                for (decode in fixed_array(true, false)) {
                    var gout <- ffn_arm(t, "kq (down k{dfmt == 3 ? 6 : 5})", decode, woffs, KN, KNFE, int3(1, 1, dfmt)) $(o1, o3, o2, xq, xs, nrows, refv) {
                        ref_ffn_kq(o1, o3, o2, woffs, 2, w1, w3, w2, xq, xs, nrows, refv)
                    }
                    delete gout
                }
                delete w2
            }
            var xq : array<int8>
            var xs : array<float>

            // classifier arm (the head offload's dispatch): one whole-plane k6 GEMV — ONE
            // region covering every row, device meta + pre-recorded cmd. Two passes with
            // perturbed activations exercise the recorded-resubmit path.
            let CWOFF = 9000000l
            let CVOCAB = 512l
            var wc : array<float>
            t |> success(plane_up(3, CVOCAB, KN, 71l, CWOFF, wc, false), "k6 classifier plane uploaded")
            var ylog : array<float>
            ylog |> resize(int(CVOCAB))
            for (cpass in range(2)) {
                fill_acts_kq(xq, xs, 1l)
                xq[0] = int8(int(xq[0]) + cpass * 3)
                matmul_moe_gpu_cls(ylog, CWOFF, xq, xs, KN, CVOCAB, 3)
                var yref <- ref_deq_gemm(wc, xq, xs, KN, CVOCAB, 1l)
                held_nontrivial(t, "cls GEMV arm pass {cpass}", ylog, yref, REL_TOL, CVOCAB)
                delete yref
            }
            delete wc
            delete ylog
            delete w1
            delete w3
            delete xq
            delete xs
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
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
            var w1 : array<float>
            var w3 : array<float>
            var w2 : array<float>
            if (!plane_up(4, NE * KNFE, KN, 83l, QWOFF1, w1, false)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            t |> success(plane_up(4, NE * KNFE, KN, 97l, QWOFF3, w3, false), "q40 up stack uploaded")
            t |> success(plane_up(4, NE * KN, KNFE, 101l, QWOFF2, w2, false), "q40 down stack uploaded")
            let woffs = Bases(gate = QWOFF1, up = QWOFF3, down = QWOFF2)
            for (decode in fixed_array(true, false)) {   // the GEMV kernel, then the q40 tile
                var gout <- ffn_arm(t, "q40", decode, woffs, KN, KNFE, int3(4)) $(o1, o3, o2, xq, xs, nrows, refv) {
                    ref_ffn_kq(o1, o3, o2, woffs, 2, w1, w3, w2, xq, xs, nrows, refv)
                }
                delete gout
            }
            delete w1
            delete w3
            delete w2
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
        }
    }
}

// ===== dense-GEMM rail arms (one-region ride on the batch tiles) =====

let DWOFF = 30000000l   // q8 dense plane base (q8 offset space, clear of WOFF*/SWOFF)
let KDWOFF = 31000000l  // k4 dense plane base (kq offset space, clear of KWOFF*)
let DROP_WOFF = 32000000l   // drop-arm scratch plane (q8 space)
let DD = 160l           // dense output dim (%32)
let DNR = 40l           // batch rows — one full 32-tile plus an unpadded remainder

// the Q4_K_M MoE recipe bumps ffn_down_exps to Q5_0/Q8_0 while gate/up stay Q4_K, so
// k4/k4/q8 is the shape real files ship (glm4moe, gemma). gate/up must agree (one activation
// image); down is free — the chain requants the intermediate in the DOWN stack's form.
let MXOFF1 = 210000000l
let MXOFF3 = 220000000l
let MXOFF2 = 230000000l

[test]
def test_vulkan_moe_ffn_mixed(t : T?) {
    t |> run("vulkan MoE mixed triple (k4 gate/up + q8 down) == CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            var w1 : array<float>
            var w3 : array<float>
            if (!plane_up(1, NE * KNFE, KN, 61l, MXOFF1, w1)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            t |> success(plane_up(1, NE * KNFE, KN, 67l, MXOFF3, w3), "k4 up stack uploaded")
            // down: plain q8, the format the Q5_0/Q8_0 downs land in after the loader's transcode
            var dq2 : array<int8>
            var ds2 : array<float>
            fill_q8_quants(dq2, ds2, NE * KN, KNFE, 71l, false)
            t |> success(q8_upload(dq2, ds2, MXOFF2, KNFE, NE * KN), "q8 down stack uploaded")

            var w2 : array<float>
            dequant_q8_rows(dq2, ds2, NE * KN, KNFE, w2)
            let woffs = Bases(gate = MXOFF1, up = MXOFF3, down = MXOFF2)
            //! the batch arm: vk_moe_ffn_batch carries its own xunit split and chunk sizing, so the decode arm does NOT cover it
            for (decode in fixed_array(true, false)) {
                var gout <- ffn_arm(t, "mixed k4/k4/q8", decode, woffs, KN, KNFE, int3(1, 1, 0)) $(o1, o3, o2, xq, xs, nrows, refv) {
                    ref_ffn_kq(o1, o3, o2, woffs, 2, w1, w3, w2, xq, xs, nrows, refv, 32)
                }
                delete gout
            }
            delete dq2
            delete ds2
            delete w1
            delete w3
            delete w2
        } else {
            t |> skip("vulkan module not built")
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
            fill_q8_quants(wq, ws, DD, N, 17l, false)
            if (!q8_upload(wq, ws, DWOFF, N, DD)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, DNR)
            var y : array<float>
            y |> resize(int(DNR * DD))
            matmul_gpu_dense(y, DWOFF, xq, xs, N, DD, DNR, 0)
            var refy : array<float>
            ref_q8_gemm(wq, ws, xq, xs, N, DD, DNR, refy)
            held(t, "dense q8 arm", y, refy, REL_TOL, DD)
            // k4 arm: [DD x KN] plane, Q8_K acts through the same one-region rail
            var w : array<float>
            if (!plane_up(1, DD, KN, 23l, KDWOFF, w, false)) {
                t |> skip("kq dense upload refused (budget); skipping the k4 arm")
                return
            }
            var kxq : array<int8>
            var kxs : array<float>
            fill_acts_kq(kxq, kxs, DNR)
            var ky : array<float>
            ky |> resize(int(DNR * DD))
            matmul_gpu_dense(ky, KDWOFF, kxq, kxs, KN, DD, DNR, 1)
            var krefy <- ref_deq_gemm(w, kxq, kxs, KN, DD, DNR)
            held(t, "dense k4 arm", ky, krefy, REL_TOL, DD)
            // drop arm: roll back a fresh upload (the partial-triple path), then prove the
            // registry survived — the earlier q8 stack still serves, and the dropped offset
            // re-uploads and serves (buffer free + budget refund didn't corrupt anything)
            var wq3 : array<int8>
            var ws3 : array<float>
            fill_q8_quants(wq3, ws3, DD, N, 29l, false)
            t |> success(q8_upload(wq3, ws3, DROP_WOFF, N, DD), "doomed upload lands")
            moe_gpu_drop_stacks(1, false)
            t |> success(q8_upload(wq3, ws3, DROP_WOFF, N, DD, true), "stream mirror lands")
            moe_gpu_drop_stacks(1, true)
            y |> resize(0)
            y |> resize(int(DNR * DD))
            matmul_gpu_dense(y, DWOFF, xq, xs, N, DD, DNR, 0)
            held(t, "q8 stack still serves after a later stack dropped", y, refy, REL_TOL, DD)
            t |> success(q8_upload(wq3, ws3, DROP_WOFF, N, DD), "dropped offset re-uploads")
            var y3 : array<float>
            y3 |> resize(int(DNR * DD))
            matmul_gpu_dense(y3, DROP_WOFF, xq, xs, N, DD, DNR, 0)
            var refy3 : array<float>
            ref_q8_gemm(wq3, ws3, xq, xs, N, DD, DNR, refy3)
            held(t, "re-uploaded stack serves after drop", y3, refy3, REL_TOL, DD)
            delete wq3
            delete ws3
            delete y3
            delete refy3
            delete wq
            delete ws
            delete xq
            delete xs
            delete y
            delete refy
            delete w
            delete kxq
            delete kxs
            delete ky
            delete krefy
        } else {
            t |> skip("dasVulkan not installed; vulkan MoE tier untestable here")
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
    y |> resize(nrows * d)
    for (r in range64(nrows)) {
        for (c in range64(d)) {
            y[r * d + c] = float(ref_dot_q8_f64(wq, ws, c * n, xq, xs, r * n, n).dot)
        }
    }
}

// the CPU mirror of the deltanet chain past the qkv/z GEMMs: conv + SiLU + q/k L2-norm +
// q pre-scale, the per-head chunked delta rule, the gated out-norm, the o requant, and the
// out projection. state advances in place; qkvy/zy are the GEMM outputs the caller made.
def private ref_dn_core(qkvy, zy : array<float>; bts, gts, tapsv, wnormv, av, dtv : array<float>;   // nolint:STYLE037 — one flat CPU mirror of the deltanet chain; its stages share the chunk buffers
                        var state : array<float>; hist : array<float>; npos : int64;
                        wo : array<int8>; wso : array<float>; var yout : array<float>; var conv : array<float>) {
    delete conv
    conv <- dn_conv_ref(qkvy, hist, 0, tapsv, 0, int(npos), int(DNCD), int(DNCONV), int(DNKD), int(DNDS), DN_EPS, 1.0 / sqrt(float(DNDS)))
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
                bv[int(i)] = sigmoid_f32(bts[int((c0 + i) * DNVH + h)])
                let graw = gts[int((c0 + i) * DNVH + h)] + dtv[int(h)]
                gacc += av[int(h)] * softplus(graw)
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
                    o[int((c0 + i) * DNDI + h * DNDS + j)] = xn * silu_f32(zv)
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
    var orq <- q8_roundtrip(o, npos * DNDI, false)
    ref_q8_gemm(wo, wso, orq.xq, orq.xs, DNDI, DND, npos, yout)
    delete o
    delete orq
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

//! A deltanet fixture's float planes over `npos` positions: the beta and gate rows, the conv taps, the out-norm row, the decay
//! and dt rows, the conv history and the starting state
struct private DnFix {
    bts : array<float>
    gts : array<float>
    tapsv : array<float>
    wnormv : array<float>
    av : array<float>
    dtv : array<float>
    hist : array<float>
    state : array<float>
}

def private dn_fixture(npos : int64) : DnFix {
    return <- DnFix(bts <- [for (i in range64(npos * DNVH)); synth_f(i, 3l, 1.5)],
        gts <- [for (i in range64(npos * DNVH)); synth_f(i, 11l, 1.5)],
        tapsv <- [for (i in range64(DNCD * DNCONV)); synth_f(i, 17l, 0.25)],
        wnormv <- [for (i in range64(DNDS)); 1.0 + synth_f(i, 19l, 0.3)],
        av <- [for (i in range64(DNVH)); -(0.3 + abs(synth_f(i, 23l, 1.2)))],
        dtv <- [for (i in range64(DNVH)); synth_f(i, 29l, 0.4)],
        hist <- [for (i in range64(DNCD * DNTAPS)); synth_f(i, 43l, 0.5)],
        state <- [for (i in range64(DNVH * DNDS * DNDS)); synth_f(i, 47l, 0.1)])
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
            fill_q8_quants(wqkv, wsqkv, DNCD, DND, 31l, true)
            fill_q8_quants(wz, wsz, DNDI, DND, 37l, true)
            fill_q8_quants(wo, wso, DND, DNDI, 41l, false)
            if (!q8_upload(wqkv, wsqkv, DNQKV_WOFF, DND, DNCD)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn/attention chains declined on this device (shared-memory cap)")
                return
            }
            t |> success(q8_upload(wz, wsz, DNZ_WOFF, DND, DNDI), "z stack uploaded")
            t |> success(q8_upload(wo, wso, DNO_WOFF, DNDI, DND), "out stack uploaded")
            for (arm_npos in [70l, 1100l]) {
                let npos = arm_npos
                var xq : array<int8>
                var xs : array<float>
                fill_acts_of(xq, xs, npos, DND, 32l, 13l, 7l, 4l)
                var fx <- dn_fixture(npos)
                var gstate := fx.state
                var rstate := fx.state
                var y : array<float>
                var tail : array<float>
                y |> resize(int(npos * DND))
                tail |> resize(int(DNTAPS * DNCD))
                unsafe {
                    matmul_moe_gpu_dn(y, DNQKV_WOFF, DNZ_WOFF, DNO_WOFF, xq, xs, fx.bts, fx.gts,
                        addr<float const?>(fx.tapsv[0]), addr<float const?>(fx.wnormv[0]),
                        addr<float const?>(fx.av[0]), addr<float const?>(fx.dtv[0]),
                        addr(gstate[0]), tail, addr<float const?>(fx.hist[0]),
                        npos, DND, DNCD, DNKD, DNDI, DNVH, DNKH, DNDS, DNCONV, DN_EPS)
                }
                var qkvy : array<float>
                var zy : array<float>
                ref_q8_gemm(wqkv, wsqkv, xq, xs, DND, DNCD, npos, qkvy)
                ref_q8_gemm(wz, wsz, xq, xs, DND, DNDI, npos, zy)
                var refy : array<float>
                var refconv : array<float>
                ref_dn_core(qkvy, zy, fx.bts, fx.gts, fx.tapsv, fx.wnormv, fx.av, fx.dtv, rstate, fx.hist, npos, wo, wso, refy, refconv)
                held_nontrivial(t, "npos={npos}: out rows", y, refy, DN_REL_TOL, DND)
                held(t, "npos={npos}: final state", gstate, rstate, DN_REL_TOL, DNDS)
                var reft : array<float>
                reft |> resize(int(DNTAPS * DNCD))
                for (k in range64(DNTAPS)) {
                    for (ch in range64(DNCD)) {
                        reft[int(k * DNCD + ch)] = qkvy[int((npos - DNTAPS + k) * DNCD + ch)]
                    }
                }
                held(t, "npos={npos}: conv-tail rows", tail, reft, REL_TOL, DNCD)
                delete xq
                delete xs
                delete fx
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
        } else {
            t |> skip("dasVulkan not installed; vulkan deltanet chain untestable here")
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
            fill_q8_quants(wqkv, wsqkv, DNCD, DND, 61l, true)
            fill_q8_quants(wz, wsz, DNDI, DND, 67l, true)
            fill_q8_quants(wo, wso, DND, DNDI, 71l, false)
            if (!q8_upload(wqkv, wsqkv, DNSQ_WOFF, DND, DNCD)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn/attention chains declined on this device (shared-memory cap)")
                return
            }
            t |> success(q8_upload(wz, wsz, DNSZ_WOFF, DND, DNDI), "z stack uploaded")
            t |> success(q8_upload(wo, wso, DNSO_WOFF, DNDI, DND), "out stack uploaded")
            set_moe_gpu_dn_layer(0l, DNSQ_WOFF)   // the flush hook resolves layer -> tier state by this
            var xq : array<int8>
            var xs : array<float>
            fill_acts_of(xq, xs, DNSTEPS, DND, 32l, 13l, 5l, 2l)
            var fx <- dn_fixture(DNSTEPS)
            // references: the straight-through 8-token mirror and the 4-token prefix (flush point)
            var qkvy : array<float>
            var zy : array<float>
            ref_q8_gemm(wqkv, wsqkv, xq, xs, DND, DNCD, DNSTEPS, qkvy)
            ref_q8_gemm(wz, wsz, xq, xs, DND, DNDI, DNSTEPS, zy)
            var rstate := fx.state
            var rstate4 := fx.state
            var refy : array<float>
            var refconv : array<float>
            ref_dn_core(qkvy, zy, fx.bts, fx.gts, fx.tapsv, fx.wnormv, fx.av, fx.dtv, rstate, fx.hist, DNSTEPS, wo, wso, refy, refconv)
            var qkvy4 <- copy_span(qkvy, 0l, 4l * DNCD)
            var zy4 <- copy_span(zy, 0l, 4l * DNDI)
            var bts4 <- copy_span(fx.bts, 0l, 4l * DNVH)
            var gts4 <- copy_span(fx.gts, 0l, 4l * DNVH)
            var refy4 : array<float>
            var refconv4 : array<float>
            ref_dn_core(qkvy4, zy4, bts4, gts4, fx.tapsv, fx.wnormv, fx.av, fx.dtv, rstate4, fx.hist, 4l, wo, wso, refy4, refconv4)
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
            var gstate := fx.state
            var gring := fx.hist
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
                    b1[int(i)] = fx.bts[int(tok * DNVH + i)]
                    g1[int(i)] = fx.gts[int(tok * DNVH + i)]
                }
                unsafe {
                    matmul_moe_gpu_dn(y1, DNSQ_WOFF, DNSZ_WOFF, DNSO_WOFF, xq1, xs1, b1, g1,
                        addr<float const?>(fx.tapsv[0]), addr<float const?>(fx.wnormv[0]),
                        addr<float const?>(fx.av[0]), addr<float const?>(fx.dtv[0]),
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
                    held(t, "flushed state at token 4", gstate, rstate4, DN_REL_TOL, DNDS)
                    held(t, "flushed conv ring at token 4", gring, reft4, REL_TOL, DNTAPS)
                }
            }
            unsafe {
                moe_gpu_dn_flush(0l, addr(gstate[0]), addr(gring[0]))
            }
            held_nontrivial(t, "step out rows across the flush", y, refy, DN_REL_TOL, DND)
            held(t, "final state", gstate, rstate, DN_REL_TOL, DNDS)
            held(t, "final conv ring", gring, reft, REL_TOL, DNTAPS)
            reset_moe_gpu_dn_layers()
            delete wqkv
            delete wsqkv
            delete wz
            delete wsz
            delete wo
            delete wso
            delete xq
            delete xs
            delete fx
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
            t |> skip("dasVulkan not installed; vulkan deltanet decode step untestable here")
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
//! of the chain's max |out|: the cm2 flash tile accumulates O in f16 under the row-max bias (every P at an eighth or under, the
//! row a convex combination of f16-rounded V rows), so its rounding rides the running sum's magnitude - 2^-11 relative a step,
//! a few steps a row - and the wo GEMM then quantizes that out through Q8 blocks; the bias arm reads 3.4e-3 (RTX 5060 Ti,
//! maxref 9641) where the f32 accumulator read under 3e-3. The tile itself is held to 2e-3 by test_vulkan_kernels.das's fa cells.
let AT_REL_TOL = 5e-3

// deinterleave (gated) + per-head rms + partial NEOX rope of one projection plane — the CPU
// mirror of at_prep. rows out d-wide; a gated q also returns the raw gate plane.
def private ref_at_prep(y : array<float>; rmsw : array<float>; cosr, sinr : array<float>;
                        npos, d, rot : int64; gated : bool;
                        var outp : array<float>; var gatep : array<float>) {
    let half = int(rot / 2l)
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
        rope_tab_ref(row, 0, int(ATHS), int(rot), int(d), true, cosr, sinr, int(p) * half)   // NEOX pairs (j, j+half) over the roped span
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
    let kv_mul = int(ATNH / ATNKV)
    let hs = int(ATHS)
    op |> resize(npos * ATQD)
    for (p in range64(npos)) {
        for (h in range64(ATNH)) {
            let ob = int(p * ATQD + h * ATHS)
            attn_row_oracle(qp, ob, kp, 0, vp, 0, int(ATKVD), int(h), hs, kv_mul, int(p + 1l), scale, op, ob)
            if (gated) {
                for (dd in range(hs)) {
                    op[ob + dd] = sig_gate(op[ob + dd], gatep[ob + dd])
                }
            }
        }
    }
}

//! the o rows requantized in the wo stack's form - blocks of 256 for the k4 wo (dequantized `wo4`), Q8_0 for the q8 one
//! (`wo`, `wso`) - then the wo GEMM
def private ref_wo_gemm(op : array<float>; npos : int64; wo_k4 : bool; wo4 : array<float>; wo : array<int8>; wso : array<float>) : array<float> {
    var orq <- q8_roundtrip(op, npos * ATQD, wo_k4)
    var refy : array<float>
    if (wo_k4) {
        refy <- ref_deq_gemm(wo4, orq.xq, orq.xs, ATQD, ATD, npos)
    } else {
        ref_q8_gemm(wo, wso, orq.xq, orq.xs, ATQD, ATD, npos, refy)
    }
    delete orq
    return <- refy
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
            fill_q8_quants(wqg, wsqg, 2l * ATQD, ATD, 51l, true)
            fill_q8_quants(wqp, wsqp, ATQD, ATD, 53l, true)
            fill_q8_quants(wk, wsk, ATKVD, ATD, 57l, true)
            fill_q8_quants(wv, wsv, ATKVD, ATD, 59l, true)
            fill_q8_quants(wo, wso, ATD, ATQD, 61l, false)
            if (!q8_upload(wqg, wsqg, ATQ_WOFF, ATD, 2l * ATQD)) {
                t |> skip("no Vulkan device (or probe/budget refused the upload)")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn/attention chains declined on this device (shared-memory cap)")
                return
            }
            t |> success(q8_upload(wqp, wsqp, ATQ_WOFF + 500000l, ATD, ATQD), "plain q stack uploaded")
            t |> success(q8_upload(wk, wsk, ATK_WOFF, ATD, ATKVD), "k stack uploaded")
            t |> success(q8_upload(wv, wsv, ATV_WOFF, ATD, ATKVD), "v stack uploaded")
            t |> success(q8_upload(wo, wso, ATO_WOFF, ATQD, ATD), "wo stack uploaded")
            var wo4 : array<float>
            t |> success(plane_up(1, ATD, ATQD, 67l, ATO4_WOFF, wo4, false), "k4 wo stack uploaded")
            var rmsq : array<float>
            var rmsk : array<float>
            rmsq |> resize(int(ATHS))
            rmsk |> resize(int(ATHS))
            for (i in range(int(ATHS))) {
                rmsq[i] = 1.0 + synth_f(int64(i), 71l, 0.3)
                rmsk[i] = 1.0 + synth_f(int64(i), 73l, 0.3)
            }
            var bq : array<float>
            var bk : array<float>
            var bv : array<float>
            bq |> resize(int(ATQD))
            bk |> resize(int(ATKVD))
            bv |> resize(int(ATKVD))
            for (i in range(int(ATQD))) {
                bq[i] = synth_f(int64(i), 83l, 0.4)
            }
            for (i in range(int(ATKVD))) {
                bk[i] = synth_f(int64(i), 89l, 0.4)
                bv[i] = synth_f(int64(i), 97l, 0.4)
            }
            for (arm in [(npos = 70l, rot = 64l, gated = true, wo_k4 = false, bias = false),
                         (npos = 70l, rot = 128l, gated = false, wo_k4 = false, bias = false),
                         (npos = 70l, rot = 64l, gated = true, wo_k4 = true, bias = false),
                         (npos = 1100l, rot = 64l, gated = true, wo_k4 = false, bias = false),
                         (npos = 70l, rot = 128l, gated = false, wo_k4 = false, bias = true)]) {
                let npos = arm.npos
                let rot = arm.rot
                let half = rot / 2l
                var xq : array<int8>
                var xs : array<float>
                fill_acts_of(xq, xs, npos, ATD, 32l, 13l, 7l, 6l)
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
                        arm.wo_k4 ? ATO4_WOFF : ATO_WOFF,
                        0, 0, 0, arm.wo_k4 ? 1 : 0, xq, xs,
                        addr<float const?>(rmsq[0]), addr<float const?>(rmsk[0]),
                        arm.bias ? addr<float const?>(bq[0]) : null, arm.bias ? addr<float const?>(bk[0]) : null, arm.bias ? addr<float const?>(bv[0]) : null,
                        addr<float const?>(cosr[0]), addr<float const?>(sinr[0]),
                        npos, ATD, ATQD, ATKVD, ATHS, rot, ATNH,
                        1.0 / sqrt(float(ATHS)), AT_EPS, arm.gated, true)
                }
                // the CPU mirror: GEMMs (+ the projection bias), prep, attention, requant, wo
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
                if (arm.bias) {   //! ungated: the q rows are plain [npos x ATQD]
                    for (p in range64(npos)) {
                        for (i in range64(ATQD)) {
                            qgy[int(p * ATQD + i)] += bq[int(i)]
                        }
                        for (i in range64(ATKVD)) {
                            ky[int(p * ATKVD + i)] += bk[int(i)]
                            vy[int(p * ATKVD + i)] += bv[int(i)]
                        }
                    }
                }
                var qp : array<float>
                var gatep : array<float>
                var kp : array<float>
                var kgate_unused : array<float>
                ref_at_prep(qgy, rmsq, cosr, sinr, npos, ATQD, rot, arm.gated, qp, gatep)
                ref_at_prep(ky, rmsk, cosr, sinr, npos, ATKVD, rot, false, kp, kgate_unused)
                var op : array<float>
                ref_at_attn(qp, kp, vy, gatep, npos, arm.gated, op)
                var refy <- ref_wo_gemm(op, npos, arm.wo_k4, wo4, wo, wso)
                let tag = "npos={npos} rot={rot} gated={arm.gated} o4={arm.wo_k4} bias={arm.bias}"
                held_nontrivial(t, "{tag}: out rows", y, refy, AT_REL_TOL, ATD)
                if (held(t, "{tag}: roped k rows", kout, kp, AT_REL_TOL, ATKVD) > 0) {
                    // a run of bad rows at a window boundary names the racing dispatch
                    let kbar = AT_REL_TOL * logits_maxabs(kp)
                    var frow = -1l
                    var lrow = -1l
                    var nbad = 0l
                    for (rw in range64(npos)) {
                        var bad = false
                        for (c in range64(ATKVD)) {
                            if (!(abs(kout[int(rw * ATKVD + c)] - kp[int(rw * ATKVD + c)]) <= kbar)) {
                                bad = true
                                break
                            }
                        }
                        if (bad) {
                            if (frow < 0l) {
                                frow = rw
                            }
                            lrow = rw
                            nbad++
                        }
                    }
                    to_log(LOG_ERROR, "roped-k forensics: bad rows {nbad} first {frow} last {lrow} (npos {npos})\n")
                    if (frow >= 0l) {
                        let first_bad_off = frow * ATKVD
                        to_log(LOG_ERROR, "roped-k row {frow} gpu [{kout[int(first_bad_off)]}, {kout[int(first_bad_off + 1l)]}] ref [{kp[int(first_bad_off)]}, {kp[int(first_bad_off + 1l)]}]\n")
                    }
                }
                held(t, "{tag}: v rows", vout, vy, AT_REL_TOL, ATKVD)
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
            delete wo4
            delete rmsq
            delete rmsk
        } else {
            t |> skip("dasVulkan not installed; vulkan attention chain untestable here")
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
                t |> skip("no Vulkan device; rope_kv_store untestable here")
                return
            }
            var no_freqs : array<float>
            for (arm in range(2)) {
                let neox = arm == 1
                var qkv : array<float>
                qkv |> resize(int(QD + 2l * KVD))
                for (i in range(int(QD + 2l * KVD))) {
                    qkv[i] = float((i % 23) - 11) * 0.1
                }
                var cossin : array<float>
                cossin |> resize(int(HS))
                build_rope_row_packed(cossin, 0l, POS, HS, 10000.0, 1.0, 1.0, no_freqs)
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
                // rope rq (NH heads) and rk (NKV heads) in place through the engine's table rope
                rope_tab_ref(rq, 0, int(HS), 0, int(QD), neox, cossin, cossin, 0, int(half))
                rope_tab_ref(rk, 0, int(HS), 0, int(KVD), neox, cossin, cossin, 0, int(half))
                let tag = neox ? "NEOX" : "NORM"
                held(t, "rope_kv {tag} q roped", qout, rq, REL_TOL, HS, 1e-6)
                held(t, "rope_kv {tag} K stored roped", krow, rk, REL_TOL, HS, 1e-6)
                held(t, "rope_kv {tag} V stored raw", vrow, rv, 0.0, HS, 1e-6)
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
            t |> skip("dasVulkan not installed; rope_kv_store untestable here")
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
                t |> skip("no Vulkan device; decode_attn untestable here")
                return
            }
            var no_freqs : array<float>
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
                build_rope_row_packed(cossin, 0l, int64(p), HS, 10000.0, 1.0, 1.0, no_freqs)
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
            attn_heads_oracle(q, 0, cpuK, 0, cpuV, 0, int(KVD), int(NH), int(HS), int(kv_mul), int(CNT), SCALE, refo, 0)
            held(t, "decode attention", out, refo, REL_TOL, HS, 1e-5)
            delete cpuK
            delete cpuV
            delete q
            delete out
            delete refo
        } else {
            t |> skip("dasVulkan not installed; decode_attn untestable here")
        }
    }
}

struct private ArenaQ8 {
    wq : array<int8>
    ws : array<float>
    blk : int64
}

//! a [rows x n] q8 tensor at `seed` placed in the resident arena, its quants and scales kept for the reference; blk < 0 = refused
def private arena_place_q8(rows, n, seed : int64; small_scales : bool = false) : ArenaQ8 {
    var a : ArenaQ8
    static_if (typeinfo builtin_module_exists(vulkan)) {
        fill_q8_quants(a.wq, a.ws, rows, n, seed, small_scales)
        var pq : array<uint8>
        var ps : array<uint8>
        pack_q8_wire(a.wq, a.ws, pq, ps)
        a.blk = vk_arena_place(pq, ps, n, rows, 0)
        delete pq
        delete ps
    }
    return <- a
}

//! the arena GEMV (`npos` 1) or batch GEMM over `a`'s block against the CPU q8 reference on the same activations
def private arena_serves(t : T?; tag : string; a : ArenaQ8; rows, npos : int64; xq : array<int8>; xs : array<float>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var y : array<float>
        y |> resize(npos * rows)
        if (npos == 1l) {
            vk_arena_gemv(y, a.blk, N, rows, 0, xq, xs)
        } else {
            vk_arena_gemm(y, a.blk, N, rows, npos, 0, xq, xs)
        }
        var refy : array<float>
        ref_q8_gemm(a.wq, a.ws, xq, xs, N, rows, npos, refy)
        held(t, tag, y, refy, REL_TOL, rows)
        delete y
        delete refy
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
                t |> skip("no Vulkan device; arena batch GEMM untestable here")
                return
            }
            var ta <- arena_place_q8(GROWS, N, 41l)
            var tb <- arena_place_q8(GROWS, N, 67l)
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, GNP)
            arena_serves(t, "arena GEMM arm 0", ta, GROWS, GNP, xq, xs)
            arena_serves(t, "arena GEMM arm 1", tb, GROWS, GNP, xq, xs)
            delete ta
            delete tb
            delete xq
            delete xs
        } else {
            t |> skip("dasVulkan not installed; arena batch GEMM untestable here")
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
                t |> skip("no Vulkan device (or the arena reserve was refused)")
                return
            }
            var t1 <- arena_place_q8(NFE, N, 11l, true)    // gate/up small scales — the sane silu regime
            var t3 <- arena_place_q8(NFE, N, 23l, true)
            var t2 <- arena_place_q8(N, NFE, 37l)
            t |> success(t1.blk >= 0l && t3.blk >= 0l && t2.blk >= 0l, "arena admitted the FFN triple (blocks {t1.blk}/{t3.blk}/{t2.blk})")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 1l)
            var y : array<float>
            y |> resize(int(N))
            vk_arena_ffn(y, t1.blk, t3.blk, t2.blk, N, NFE, 0, 0, 0, xq, xs, false)
            // CPU mirror: the fused-FFN reference over one region, the triple's only expert
            var ro <- offs_triple([0l], [0l], [1l], WOFFS, N, NFE)
            var refy : array<float>
            ref_ffn(ro.gate, ro.up, ro.down, WOFFS, 1, t1.wq, t1.ws, t3.wq, t3.ws, t2.wq, t2.ws, xq, xs, 1l, refy)
            held(t, "arena FFN", y, refy, REL_TOL, N)
            delete t1
            delete t3
            delete t2
            delete xq
            delete xs
            delete y
            delete ro
            delete refy
        } else {
            t |> skip("dasVulkan not installed; arena FFN untestable here")
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
                held(t, "add_rms arm {arm} y", y, refy, REL_TOL, AD)
                // x IS the residual stream — the kernel must have updated it in place
                held(t, "add_rms arm {arm} residual updated in place", x, vv, REL_TOL, AD)
                delete x
                delete a
                delete w
                delete y
                delete vv
                delete refy
            }
        } else {
            t |> skip("dasVulkan not installed; add_rms untestable here")
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
                tt |> skip("no Vulkan device (or the arena reserve was refused)")
                return
            }
            var ta <- arena_place_q8(AROWS, N, 31l)
            var tb <- arena_place_q8(AROWS, N, 57l)   // different seed => different content
            // relative, not absolute: the arena is process-global, so another arm may have placed
            // into it first. Back-to-back is the actual invariant.
            tt |> success(ta.blk >= 0l && tb.blk == ta.blk + ABLK,
                "arena bump cursor packs tensors back to back (a={ta.blk}, b={tb.blk}, delta expected {ABLK})")
            var xq : array<int8>
            var xs : array<float>
            fill_acts(xq, xs, 1l)
            arena_serves(tt, "arena tensor 0", ta, AROWS, 1l, xq, xs)
            arena_serves(tt, "arena tensor 1", tb, AROWS, 1l, xq, xs)
            delete ta
            delete tb
            delete xq
            delete xs
        } else {
            tt |> skip("dasVulkan not installed; resident arena untestable here")
        }
    }
}

// ===== whole-model drop/re-arm (the multi-model switch primitive) =====

// vk_drop_model_state (via the math-side hook) destroys EVERY model-owned object — stacks,
// arenas, lazy dispatch families, their descriptor sets and command buffers — while the device
// survives. Two full cycles re-upload the SAME offsets with DIFFERENT content: a stale stack
// registry or arena would resolve the old plane and fail the cycle's own reference. Declared
// after every stack-serving test on purpose: the drop nukes every stack earlier tests uploaded
// (the slab test below shares that property).
let DMWOFF = 50000000l
let DMROWS = 64l

[test]
def test_vulkan_drop_model(t : T?) {
    t |> run("vulkan whole-model drop - stacks + arena rebuild and serve across two cycles") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            for (cycle in range(2)) {
                // per-op stack rail: upload -> dense GEMV -> parity vs THIS cycle's content
                var wq : array<int8>
                var ws : array<float>
                fill_q8_quants(wq, ws, DMROWS, N, 17l + int64(cycle) * 31l, false)
                if (!q8_upload(wq, ws, DMWOFF, N, DMROWS)) {
                    t |> skip("no Vulkan device (or probe/budget refused the upload)")
                    return
                }
                var xq : array<int8>
                var xs : array<float>
                fill_acts(xq, xs, 2l)
                var y : array<float>
                y |> resize(int(2l * DMROWS))
                matmul_gpu_dense(y, DMWOFF, xq, xs, N, DMROWS, 2l, 0)
                var refy : array<float>
                ref_q8_gemm(wq, ws, xq, xs, N, DMROWS, 2l, refy)
                held(t, "cycle {cycle} stack serves its own content", y, refy, REL_TOL, DMROWS)
                // arena rail: reserve (fresh after a drop) -> place -> GEMV -> parity
                t |> success(vk_arena_reserve(0, ARENA_BLOCKS / 2l), "cycle {cycle} arena reserve")
                var at <- arena_place_q8(DMROWS, N, 91l + int64(cycle) * 13l)
                t |> success(at.blk >= 0l, "cycle {cycle} arena placement (block {at.blk})")
                var axq : array<int8>
                var axs : array<float>
                fill_acts(axq, axs, 1l)
                arena_serves(t, "cycle {cycle} arena serves its own content", at, DMROWS, 1l, axq, axs)
                // the whole-model drop: every stack (this file's earlier tests included), the
                // arena, and all lazy dispatch state die; cycle 1 re-runs the ladder from bare
                moe_gpu_drop_model_state()
                t |> equal(0l, gpu_tier_status_().vram_bytes, "cycle {cycle} drop left zero resident bytes")
                delete wq
                delete ws
                delete xq
                delete xs
                delete y
                delete refy
                delete at
                delete axq
                delete axs
            }
        } else {
            t |> skip("dasVulkan not installed; whole-model drop untestable here")
        }
    }
}

// The arena's slab split: maxStorageBufferRange caps one binding, so a format's arena carries
// multiple plane pairs and the encoded block index picks the slab (tensors never straddle a
// boundary). A forced tiny slab cap stands in for an over-4GiB dense model (the 8B-class
// shape). Drops the whole model state first for a FRESH arena — the reserve must not reuse a
// default-cap one — so it sits with the drop test, after every stack-serving test.
[test]
def test_vulkan_arena_slabs(t : T?) {
    t |> run("vulkan arena slabs - placement crosses the slab boundary, both slabs serve gemv") @(tt : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let SROWS = 64l
            let SBLK = SROWS * (N / 32l)   // blocks one [SROWS x N] q8 tensor occupies
            moe_gpu_drop_model_state()
            vk_arena_test_slab_cap(SBLK * 5l / 2l)   // 2.5 tensors per slab: the third place crosses
            {
                if (!vk_arena_reserve(0, 4l * SBLK)) {
                    tt |> skip("no Vulkan device (or the arena reserve was refused)")
                    return
                }
                var ts <- [for (arm in range64(3l)); arena_place_q8(SROWS, N, 7l + arm * 29l)]
                // A and B pack into slab 0 (B at a nonzero local base); C crosses into slab 1 at local 0
                tt |> equal(0l, ts[0].blk, "tensor A at slab 0, local 0")
                tt |> equal(SBLK, ts[1].blk, "tensor B at slab 0, local {SBLK}")
                tt |> equal(1l << 32l, ts[2].blk, "tensor C crosses into slab 1, local 0")
                var xq : array<int8>
                var xs : array<float>
                fill_acts(xq, xs, 1l)
                for (ta, arm in ts, count()) {
                    arena_serves(tt, "slab tensor {arm}", ta, SROWS, 1l, xq, xs)
                }
                delete ts
                delete xq
                delete xs
            } finally {
                vk_arena_test_slab_cap(0l)   // later reserves must not inherit the tiny cap
                moe_gpu_drop_model_state()   // nor the tiny-slab arena itself
            }
        } else {
            tt |> skip("dasVulkan not installed; arena slabs untestable here")
        }
    }
}
