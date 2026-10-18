options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - the one-pass rare rail's per-expert chain (gate, up, act, requant, down over one activation
// row) in Flash-Next's shape (dim 2560, nfe 640; gate/up iq3s4, down iq4nl32) on synthetic experts, the set sized
// past the L3 so every plane comes from DRAM: one lane walking experts in sequence (the chain's own cost, one
// stream), then every lane walking its own experts in one fork (the rail's shape: the DRAM share a lane gets),
// and the single-lane chain split by step. Microseconds an expert. Args: [experts] [rows an expert] (default 256 1;
// the rail's shape is 8 rows an expert, a lane's experts in sequence, every row of one before the next).

require strings
require math
require daslib/jobque_boost
require dasllama/dasllama_math
require dasllama/dasllama_math_gen
require dasllama/dasllama_kqformat
require dasllama/dasllama_convert
require dasllama/dasllama_par
require ../tests/_kq_fixtures.das

let private ROUNDS = 20
let private DIM = 2560l
let private NFE = 640l
let private FMT_GU_IQ3S4 = 35

var private g_exit = 1

//! one expert's three planes, repacked into the backend's serving layout
struct private Expert {
    gq, gs : array<uint8>
    uq, us : array<uint8>
    dq, ds : array<uint8>
}

//! the lane-local rows of one chain
struct private Rows {
    g, u, o : array<float>
    hq : array<int8>
    hs : array<float>
    hbs : array<int>
}

def private make_rows(ntok : int64) : Rows {
    var r : Rows
    r.g |> resize(ntok * NFE)
    r.u |> resize(ntok * NFE)
    r.o |> resize(ntok * DIM)
    r.hq |> resize(ntok * NFE)
    r.hs |> resize(ntok * NFE / 32l)
    r.hbs |> resize(ntok * NFE / 16l)
    return <- r
}

def private fill_random(var a : array<uint8>; seed : uint) {
    var st = seed
    for (i in range(length(a))) {
        st = st * 1664525u + 1013904223u
        a[i] = uint8(st >> 16u)
    }
}

def private make_expert(e : int64) : Expert {
    var x : Expert
    synth_kq_region(FMT_GU_IQ3S4, x.gq, x.gs, NFE, DIM, 0x1234u + uint(e) * 7919u, 0x85EBCA6Bu)
    repack_kq_weight(FMT_GU_IQ3S4, x.gq, x.gs, 0l, DIM, NFE)
    synth_kq_region(FMT_GU_IQ3S4, x.uq, x.us, NFE, DIM, 0x4321u + uint(e) * 7919u, 0x85EBCA6Bu)
    repack_kq_weight(FMT_GU_IQ3S4, x.uq, x.us, 0l, DIM, NFE)
    let nblk = DIM * NFE / 32l
    x.dq |> resize(nblk * IQ4NL32_QB)
    x.ds |> resize(nblk * IQ4NL32_SB)
    fill_random(x.dq, 0x9876u + uint(e) * 7919u)
    for (b in range64(nblk)) {   //! f16 d = 1.0 a block: no denormal arithmetic in the dot
        x.ds[b * 2l] = uint8(0)
        x.ds[b * 2l + 1l] = uint8(0x3C)
    }
    return <- x
}

//! the activation row as the lanes read it
struct private Acts {
    xq : int8 const?
    xs : float const?
    xbs : int const?
}

//! an expert's planes and a lane's rows as the lanes read them: plain pointers, the shape a fork captures
struct private ExpertP {
    gq, gs, uq, us, dq, ds : uint8 const?
}

struct private RowsP {
    g, u, o : float?
    hq : int8?
    hs : float?
    hbs : int?
}

def private expert_p(x : Expert) : ExpertP {
    unsafe {
        return ExpertP(gq = addr<uint8 const?>(x.gq[0]), gs = addr<uint8 const?>(x.gs[0]), uq = addr<uint8 const?>(x.uq[0]),
            us = addr<uint8 const?>(x.us[0]), dq = addr<uint8 const?>(x.dq[0]), ds = addr<uint8 const?>(x.ds[0]))
    }
}

def private rows_p(var r : Rows) : RowsP {
    unsafe {
        return RowsP(g = addr(r.g[0]), u = addr(r.u[0]), o = addr(r.o[0]), hq = addr(r.hq[0]), hs = addr(r.hs[0]), hbs = addr(r.hbs[0]))
    }
}

//! the chain over expert `x` with activation `a`, on the calling lane; `step` > 0 runs that many of its five steps only
def private chain(x : ExpertP; a : Acts; var r : RowsP; gu_fn : MatmulKqRowsFn; step : int) {
    unsafe {
        var g = r.g
        var u = r.u
        var o = r.o
        var hq = r.hq
        var hs = r.hs
        var hbs = r.hbs
        invoke(gu_fn, g, x.gq, x.gs, a.xq, a.xs, a.xbs, DIM, 0l, NFE)
        return if (step == 1)
        invoke(gu_fn, u, x.uq, x.us, a.xq, a.xs, a.xbs, DIM, 0l, NFE)
        return if (step == 2)
        swiglu4(g, u, NFE)
        return if (step == 3)
        quantize_q8_0_bs_into_ptr(g, NFE, hq, hs, hbs, 0l, 0l, 0l)
        return if (step == 4)
        iq4nl32q8_gemv_gen(o, x.dq, x.ds, hq, hs, hbs, NFE, 0l, DIM)
    }
}

//! the chain over expert `x` with `ntok` contiguous activation rows at `a` through the batched expert cores: gate and up
//! tiles over every row, act and requant a row, the down tile; `tb` the token block, taken in the main context
def private chain_batched(x : ExpertP; a : Acts; var r : RowsP; ntok, tb : int64) {
    unsafe {
        var g = r.g
        var u = r.u
        var o = r.o
        var hq = r.hq
        var hs = r.hs
        var hbs = r.hbs
        kq_expert_batch_core(FMT_GU_IQ3S4, g, x.gq, x.gs, a.xq, a.xs, a.xbs, DIM, NFE, ntok, tb, null)
        kq_expert_batch_core(FMT_GU_IQ3S4, u, x.uq, x.us, a.xq, a.xs, a.xbs, DIM, NFE, ntok, tb, null)
        for (t in range64(ntok)) {
            swiglu4(g + t * NFE, u + t * NFE, NFE)
            quantize_q8_0_bs_into_ptr(g + t * NFE, NFE, hq, hs, hbs, t * NFE, t * (NFE / 32l), t * (NFE / 16l))
        }
        b32_expert_batch_core(KqFmt.iq4nl32, o, x.dq, x.ds, hq, hs, NFE, DIM, ntok, tb, null)
    }
}

def private run_probe(ne, rpe : int64) {
    select_matmul_backend_for_load_()
    let gu_fn = kq_rows_fn(FMT_GU_IQ3S4)
    let lanes = get_dispatch_lanes()
    var experts : array<Expert>
    for (e in range64(ne)) {
        experts |> emplace(make_expert(e))
    }
    let bytes = long_length(experts[0].gq) + long_length(experts[0].gs) + long_length(experts[0].uq) + long_length(experts[0].us) + long_length(experts[0].dq) + long_length(experts[0].ds)
    print("backend '{active_kernel_backend()}', {lanes} lanes, {ne} experts x {bytes} bytes = {bytes * ne / 1048576l} MB\n")
    var inscope acts0 <- build_q8k_acts(rpe, DIM, Wave(a = 0.7, fa = 0.013, b = 0.4, fb = 0.031))
    var actv : array<Acts>
    unsafe {
        for (r in range64(rpe)) {
            actv |> push(Acts(xq = addr<int8 const?>(acts0.xq[r * DIM]), xs = addr<float const?>(acts0.xs[r * (DIM / 256l)]), xbs = addr<int const?>(acts0.xbs[r * (DIM / 16l)])))
        }
    }
    let acts = actv[0]
    var app : Acts const?
    unsafe {
        app = addr<Acts const?>(actv[0])
    }
    var rows : array<Rows>
    for (i in range(lanes)) {
        rows |> emplace(make_rows(rpe))
    }
    let tb = effective_token_block(get_q8_token_block(), DIM)
    var xp : array<ExpertP>
    for (x in experts) {
        xp |> push(expert_p(x))
    }
    var rp : array<RowsP>
    for (r in rows) {
        rp |> push(rows_p(r))
    }
    var xpp : ExpertP const?
    var rpp : RowsP?
    unsafe {
        xpp = addr<ExpertP const?>(xp[0])
        rpp = addr<RowsP?>(rp[0])
    }
    // one lane, every expert in sequence, whole chain and then each step's prefix
    var cursor = 0l
    var prefix : array<double>
    for (step in [0, 1, 2, 3, 4]) {
        for (_w in range(2)) {
            unsafe {
                chain(xpp[cursor % ne], acts, rpp[0], gu_fn, step)
            }
            cursor++
        }
        let t0 = ref_time_ticks()
        for (_r in range(ROUNDS * lanes)) {
            unsafe {
                chain(xpp[cursor % ne], acts, rpp[0], gu_fn, step)
            }
            cursor++
        }
        prefix |> push(double(get_time_usec(t0)) / double(ROUNDS * lanes))
    }
    print("one lane: chain {prefix[0]} us/expert | gate {prefix[1]} | +up {prefix[2]} | +act {prefix[3]} | +requant {prefix[4]} | down = {prefix[0] - prefix[4]}\n")
    // every lane its own experts, one fork a round, PER lanes experts a lane
    for (per in [1, 4, 16]) {
        var base = 0l
        var us = 0.0lf
        for (round in range(ROUNDS + 2)) {
            let t0 = ref_time_ticks()
            maybe_parallel_for(true, 0, lanes, lanes) $(rb, re) {
                unsafe {
                    for (i in range(rb, re)) {
                        for (k in range(per)) {
                            for (r in range64(rpe)) {   //! the rail's chunk: a lane's experts in sequence, every row of one before the next
                                chain(xpp[(base + int64(i * per + k)) % ne], app[r], rpp[i], gu_fn, 0)
                            }
                        }
                    }
                }
            }
            if (round >= 2) {   //! two warm forks
                us += double(get_time_usec(t0))
            }
            base += int64(lanes * per)
        }
        us /= double(ROUNDS)
        print("{lanes} lanes x {per} experts x {rpe} rows a fork: {us} us a fork = {us / double(lanes * per)} us/expert, {us / double(lanes * per) / double(rpe)} us/pair, {double(bytes) * double(lanes * per) / us / 1048.576lf} GB/s of planes\n")
    }
    // the same, every expert's rows through the batched cores: the plane read once a tile row-group, not once a row
    for (per in [1, 4, 16]) {
        var base = 0l
        var us = 0.0lf
        for (round in range(ROUNDS + 2)) {
            let t0 = ref_time_ticks()
            maybe_parallel_for(true, 0, lanes, lanes) $(rb, re) {
                unsafe {
                    for (i in range(rb, re)) {
                        for (k in range(per)) {
                            chain_batched(xpp[(base + int64(i * per + k)) % ne], app[0], rpp[i], rpe, tb)
                        }
                    }
                }
            }
            if (round >= 2) {
                us += double(get_time_usec(t0))
            }
            base += int64(lanes * per)
        }
        us /= double(ROUNDS)
        print("batched: {lanes} lanes x {per} experts x {rpe} rows a fork: {us} us a fork = {us / double(lanes * per)} us/expert, {us / double(lanes * per) / double(rpe)} us/pair, {double(bytes) * double(lanes * per) / us / 1048.576lf} GB/s of planes\n")
    }
    // the rows agree: the batched chain's rows against the per-row chain's, one expert
    unsafe {
        chain_batched(xpp[0], app[0], rpp[0], rpe, tb)
        var worst = 0.0
        for (r in range64(rpe)) {
            chain(xpp[0], app[r], rpp[1], gu_fn, 0)
            var scale = 1e-6
            for (i in range64(DIM)) {
                scale = max(scale, abs(rpp[1].o[i]))
            }
            for (i in range64(DIM)) {
                worst = max(worst, abs(rpp[0].o[r * DIM + i] - rpp[1].o[i]) / scale)
            }
        }
        print("rows agree: batched vs per-row max rel {worst}\n")
    }
    g_exit = 0
}

[export]
def main : int {
    var ne = 256l
    var rpe = 1l
    var i = 0
    for (a in get_command_line_arguments()) {
        let v = to_int(a)
        if (v > 0) {
            if (i == 0) {
                ne = int64(v)
            } else {
                rpe = int64(v)
            }
            i++
        }
    }
    with_job_que() {
        setup_dasllama_jobque_()
        run_probe(ne, rpe)
    }
    return g_exit
}
