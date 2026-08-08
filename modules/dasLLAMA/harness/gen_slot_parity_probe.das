options gen2
options persistent_heap = true

// Production-SLOT parity for the arm64-gen backend: portable reference vs the gen
// backend through the real dispatch rail — pin_kernel_backend + repack_q8q8_weight (the
// backend's own repack, layout from the stamped q8q8_layout_gen) + the matmul_q8q8 /
// matmul_q8q8_batch / matmul_q8q8_group3 wrappers. This is the layer the kernel-level
// probes (gen_parity_probe / gen_tune_probe) sit BELOW: they proved tile+repack in
// isolation while the slice C mr8 model run generated garbage — whatever breaks between
// the stamped layout and the production slots shows up HERE, per slot, with a maxdiff.
//
//   bin/daslang -jit modules/dasLLAMA/harness/gen_slot_parity_probe.das            (mr4 fallback)
//   DAS_TUNE_MANIFEST=<mr8 manifest> bin/daslang -jit .../gen_slot_parity_probe.das (stamped perm)
//
// Shapes cover the tails: d % mr rows (row-major tail), ntok % 4 (token tail), and a
// group3 triple with unequal region sizes. Tolerance = the gen_tune_probe gates
// (CLOSE_REL 1e-4 / CLOSE_ABS 1e-5; fast-math regroup drift is ~1e-6).

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require daslib/jobque_boost
require daslib/random
require math

let CLOSE_REL = 1e-4
let CLOSE_ABS = 1e-5

var private g_seed = int4(20260704, 17, 3, 1)

def private rand_q8(n : int64) : array<int8> {
    return <- [for (_i in range64(n)); int8(random_int(g_seed) % 256)]
}

// ACTIVATION bytes are [-127, 127] by construction in production (the Q8 quantizer's
// d = amax/127) — and the x64 sign-trick kernels (hand dot32 tiers and the generated
// maddubs/vpdpbusd legs) are exact ONLY under that precondition (PSIGNB of -128 wraps; see
// x64_avx.das dot32). Weights stay full-range rand_q8: w = -128 is legal and must be covered.
def private rand_q8_activation(n : int64) : array<int8> {
    return <- [for (_i in range64(n)); int8(random_int(g_seed) % 255 - 127)]
}

def private rand_scales(n : int64) : array<float> {
    // positive, spread over a couple of octaves — like real Q8 block scales
    return <- [for (_i in range64(n)); 0.002 + 0.01 * random_float(g_seed)]
}

def private close_enough(a, b : float) : bool {
    let d = abs(a - b)
    return d <= CLOSE_ABS || d <= CLOSE_REL * max(abs(a), abs(b))
}

def private compare(tag : string; y : array<float>; yref : array<float>; var fails : int&) {
    var maxd = 0.0
    var bad = 0
    for (i in range(length(yref))) {
        maxd = max(maxd, abs(y[i] - yref[i]))
        if (!close_enough(y[i], yref[i])) {
            bad++
        }
    }
    if (bad == 0) {
        print("{tag}: ok (maxdiff {maxd})\n")
    } else {
        fails++
        print("{tag}: FAIL {bad}/{length(yref)} outside tolerance, maxdiff {maxd}\n")
    }
}

[export]
def main {
    if (!jit_enabled()) {
        print("SKIP: -jit only (the gen backends need the LLVM JIT)\n")
        return
    }
    let genbe = get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen"
    var found = false
    for (nm in kernel_backend_names()) {
        found ||= nm == genbe
    }
    if (!found) {
        print("SKIP: {genbe} not registered on this box\n")
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        let mr = int64(q8q8_layout_gen())
        print("stamped layout mr = {mr}\n")

        // one blob holding three regions (a group3 triple with a row-tail region): the
        // wrappers take absolute element offsets, like the loader's weight blob
        let n = 256l
        let d0 = 64l
        let d1 = 40l   // d % 8 == 0 but d % 16 != 0 — small-region chunking
        let d2 = 44l   // d % 8 == 4 -> exercises the row-major row tail at mr 8
        let woff0 = 0l
        let woff1 = woff0 + d0 * n
        let woff2 = woff1 + d1 * n
        let total = woff2 + d2 * n
        var wq <- rand_q8(total)
        var ws <- rand_scales(total / 32l)
        let ntok = 5l   // 4-token tile + a 1-token tail
        var xq <- rand_q8_activation(ntok * n)
        var xs <- rand_scales(ntok * n / 32l)

        // reference: the portable row-major backend
        pin_kernel_backend("portable")
        var yref0 : array<float>
        var yref1 : array<float>
        var yref2 : array<float>
        yref0 |> resize(int(d0))
        yref1 |> resize(int(d1))
        yref2 |> resize(int(d2))
        matmul_q8q8_group3(yref0, yref1, yref2, wq, ws, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        var yref_b : array<float>
        yref_b |> resize(int(ntok * d0))
        matmul_q8q8_batch(yref_b, wq, ws, xq, xs, n, d0, ntok)

        // the gen backend: one flavor since slice E — every slot reads the stamped layout.
        // The pin honors the witness (`available`): on x64 a declined stamp keeps the pin
        // on the current backend, and portable-vs-portable would be a vacuous pass — skip
        // loudly instead.
        pin_kernel_backend(genbe)
        if (active_kernel_backend() != genbe) {
            print("SKIP: {genbe} unavailable (declined stamp - no manifest for this box?)\n")
            return
        }
        repack_q8q8_weight(wq, ws, woff0, n, d0)
        repack_q8q8_weight(wq, ws, woff1, n, d1)
        repack_q8q8_weight(wq, ws, woff2, n, d2)

        var fails = 0
        var y : array<float>
        y |> resize(int(d0))
        matmul_q8q8(y, wq, ws, xq, xs, n, d0)   // region 0 GEMV (mm slot)
        compare("mm d={d0}", y, yref0, fails)

        var yb : array<float>
        yb |> resize(int(ntok * d0))
        matmul_q8q8_batch(yb, wq, ws, xq, xs, n, d0, ntok)   // batch slot: tile + token tail
        compare("batch d={d0} ntok={ntok}", yb, yref_b, fails)

        var y0 : array<float>
        var y1 : array<float>
        var y2 : array<float>
        y0 |> resize(int(d0))
        y1 |> resize(int(d1))
        y2 |> resize(int(d2))
        matmul_q8q8_group3(y0, y1, y2, wq, ws, woff0, woff1, woff2, xq, xs, n, d0, d1, d2)
        compare("group3 d0={d0}", y0, yref0, fails)
        compare("group3 d1={d1}", y1, yref1, fails)
        compare("group3 d2={d2} (row tail)", y2, yref2, fails)

        // mx4 slots (slice E): random nibble/exponent planes — portable row-major reference
        // vs the gen backend's interleaved planes (repack_mx4_weight -> mm_mx4 / groupn_mx4
        // traversals over the generated mx4 core). Two-region blob for groupn, both regions
        // repacked independently per the loader's per-expert shape (d2 rows each — the
        // row-major row tail at mr 8).
        var mxq <- [for (_i in range64(d0 * n / 2l)); uint8(random_int(g_seed) % 256)]
        var mxs <- [for (_i in range64(d0 * n / 32l)); uint8(120 + random_int(g_seed) % 12)]
        var gxq <- [for (_i in range64(2l * d2 * n / 2l)); uint8(random_int(g_seed) % 256)]
        var gxs <- [for (_i in range64(2l * d2 * n / 32l)); uint8(120 + random_int(g_seed) % 12)]
        var moffs <- [0l, 0l, d2 * n, 0l]
        var mboffs <- [0l, d2]
        var bias <- rand_scales(d2 * 2l)

        // batch-groupn triples (woff, row0, cnt): the two regions partition the 5-token image —
        // region 0 gets a whole 4-token tile call, region 1 rides the token-tail rows core
        var mtoffs <- [0l, 0l, 4l, d2 * n, 4l, 1l]

        pin_kernel_backend("portable")
        var mref : array<float>
        mref |> resize(int(d0))
        matmul_mx4q8(mref, mxq, mxs, xq, xs, n, d0)
        var mgref : array<float>
        mgref |> resize(int(d2 * 2l))
        matmul_mx4q8_groupn(mgref, gxq, gxs, moffs, 2l, xq, xs, bias, mboffs, n, d2)
        var mbref : array<float>
        mbref |> resize(int(ntok * d0))
        matmul_mx4q8_batch(mbref, mxq, mxs, xq, xs, n, d0, ntok)
        var mgbref : array<float>
        mgbref |> resize(int(ntok * d2))
        matmul_mx4q8_batch_groupn(mgbref, gxq, gxs, mtoffs, 2l, xq, xs, bias, mboffs, n, d2)

        pin_kernel_backend(genbe)
        repack_mx4_weight(mxq, mxs, 0l, n, d0)
        repack_mx4_weight(gxq, gxs, 0l, n, d2)
        repack_mx4_weight(gxq, gxs, d2 * n, n, d2)
        var my : array<float>
        my |> resize(int(d0))
        matmul_mx4q8(my, mxq, mxs, xq, xs, n, d0)
        compare("mm_mx4 d={d0}", my, mref, fails)
        var mgy : array<float>
        mgy |> resize(int(d2 * 2l))
        matmul_mx4q8_groupn(mgy, gxq, gxs, moffs, 2l, xq, xs, bias, mboffs, n, d2)
        compare("groupn_mx4 2x d={d2} (+bias, row tail)", mgy, mgref, fails)
        var mby : array<float>
        mby |> resize(int(ntok * d0))
        matmul_mx4q8_batch(mby, mxq, mxs, xq, xs, n, d0, ntok)   // mx4_batch slot: tile + token tail
        compare("mx4_batch d={d0} ntok={ntok}", mby, mbref, fails)
        var mgby : array<float>
        mgby |> resize(int(ntok * d2))
        matmul_mx4q8_batch_groupn(mgby, gxq, gxs, mtoffs, 2l, xq, xs, bias, mboffs, n, d2)
        compare("mx4_batch_groupn 2x d={d2} (+bias, row tail)", mgby, mgbref, fails)
        delete mxq
        delete mxs
        delete gxq
        delete gxs
        delete moffs
        delete mboffs
        delete mtoffs
        delete bias
        delete mref
        delete mgref
        delete mbref
        delete mgbref
        delete my
        delete mgy
        delete mby
        delete mgby

        delete wq
        delete ws
        delete xq
        delete xs
        delete yref0
        delete yref1
        delete yref2
        delete yref_b
        delete y
        delete yb
        delete y0
        delete y1
        delete y2
        if (fails == 0) {
            print("GEN SLOT PARITY OK (mr = {mr})\n")
        } else {
            print("GEN SLOT PARITY FAILED: {fails} slot(s)\n")
        }
    }
}
