options gen2
options persistent_heap
options _jit_fast_math = true   // match gemm_1core_probe / the end-to-end config

// M3/M4 grid gates (gemm_generator_plan.md): run with DAS_TUNE_MODE=test or =tune, -jit.
// Layout is per-variant: each variant's mr comes off its q8q8_layout_gen__<suffix>
// companion (declined perms report the reference grp4 — the lockstep rail), and the probe
// repacks a fresh copy of the row-major master into that variant's grp<mr> before running it.
// Since slice D the grid stamps a THIRD function — the q8q8_gemv_gen rows core — and both
// modes cover it alongside the tile. The kq kernels (k4/k5/k6 gemv + tile, kq stage 4/5) are
// their OWN three [tune] families with per-format layout companions: synthetic disk
// superblocks -> transcode -> per-variant grp<mr> repack, each family's grid gated against
// the scalar kq_grp_row_dot oracle in test mode and iso-benched (tile batch / gemv stream /
// gemv hot) in tune mode; tune writes one manifest entry per format (tile-best row — the M1
// finding that crowned this split: the q8 grp8/nrsplit2 winner cost the k5 tile 30%).
//   test: every [tune_perm] variant of q8q8_tile_gen AND q8q8_gemv_gen (dasllama_math_gen)
//         must match the hand laneq oracle (computed once, from a grp4 copy — the fold order
//         is layout-independent; the gemv compares the token-0 row) within
//         CLOSE_REL/CLOSE_ABS on nb-even, nb-odd and a production-ish slice. Tolerance, not
//         equality: variants run under full fast-math, so the backend may legally regroup a
//         fold per loop shape (~1e-6 relative; llama.cpp holds no cross-kernel bit-parity
//         either) — real generator bugs (wrong stride/lane/scale) are orders louder and
//         still die here. Declining perms (mr8_budget / mr2_lanes / dot_vpdpbusd) run the
//         reference bodies.
//   tune: iso-bench every variant (interleaved best-of) on its own repacked buffers — the
//         tile on a batch shape, the gemv core on a single-token decode shape — print both
//         tables, and tune_manifest_set the merged winner: the row whose TILE knobs win the
//         batch bench and whose gkstep wins the decode bench among rows with those tile
//         knobs (one manifest entry = one coherent layout+tile+gemv decision). A following
//         normal run stamps it (the round-trip; point DAS_TUNE_MANIFEST somewhere writable
//         first).

require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_aarch64_neon
require dasllama/dasllama_math_gen
require dasllama/dasllama_repack
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_convert
require dasllama/dasllama_gguf   // transcode_q*k_superblock — the kq fixture pack rail
require llvm/daslib/f16_cvt   // f32_to_f16 — the kq fixtures pack disk superblock headers
require llvm/daslib/llvm_tune
require daslib/random
require daslib/json   // JsonValue — the null runtime arg of the deferred sidecar merge
require daslib/json_boost   // JV over tuples — the per-variant race rows
require daslib/fio   // fread/fopen — the probe's transcript capture
require daslib/strings_boost   // split — the confirm pass parses child output
require daslib/command_line    // get_das_exe — the confirm pass spawns daslang children
require tuner_cli              // --tune-paranoid, shared with tune_kernels
require tuner_noise            // the noise gate, shared with tune_kernels
require math
require strings
require dasllama/dasllama_env

// This half is a flat best-of-N; paranoid triples the round count. The confirm pass is never
// cut: it is a correctness gate, and skipping it ships an unconfirmed crown (the SPR amx lesson).
let ROUNDS_FULL = 6
var ROUNDS = ROUNDS_FULL

struct Fixture {
    n, d, ntok, nb : int64
    q : array<int8>       // row-major master — repacked per variant into a copy
    s : array<float>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>      // −128·Σx per (token, block) — the bias128 tile's bsums plane
    yref : array<float>
}

def build_fixture(n, d, ntok : int64) : Fixture {
    var fx = Fixture(n = n, d = d, ntok = ntok, nb = n / 32l)
    var w : array<float>
    w |> resize(n * d)
    for (i in range64(n * d)) {
        w[i] = float(i % 17l) * 0.13 - 1.1
    }
    let qt <- quantize_q8_0(w, n * d)
    fx.q |> resize(n * d)
    fx.s |> resize(d * fx.nb)
    for (i in range64(n * d)) {
        fx.q[i] = qt.q[i]
    }
    for (i in range64(d * fx.nb)) {
        fx.s[i] = qt.scales[i]
    }

    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(ntok * n)
    fx.xs |> resize(ntok * fx.nb)
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xrow[i] = float((i + tk * 7l) % 23l) * 0.07 - 0.8
        }
        quantize_q8_0_into(xrow, n, fx.xq, fx.xs, tk * n, tk * fx.nb)
    }
    fx.xbs |> resize(ntok * fx.nb)
    for (tk in range64(ntok)) {
        for (bi in range64(fx.nb)) {
            var bsum = 0
            for (j in range64(32l)) {
                bsum += int(fx.xq[tk * n + bi * 32l + j])
            }
            fx.xbs[tk * fx.nb + bi] = -128 * bsum
        }
    }

    // the oracle: hand laneq semantics off a grp4 copy — the per-block fold order is the same
    // in every variant, so one oracle serves all layouts
    fx.yref |> resize(ntok * d)
    var q4 := fx.q
    var s4 := fx.s
    unsafe {
        repack_q8q8_grp(addr(q4[0]), addr(s4[0]), n, d, 4l)
        let wp = addr(q4[0])
        let sp = addr(s4[0])
        let xqp = addr(fx.xq[0])
        let xsp = addr(fx.xs[0])
        var yrp = addr(fx.yref[0])
        for (g in range64(d / 4l)) {
            let wgp = wp + g * 4l * n
            let sgp = sp + g * 4l * fx.nb
            for (tk in range64(ntok)) {
                let f = dot_q8q8_laneq4(wgp, sgp, xqp + tk * n, xsp + tk * fx.nb, n)
                yrp[tk * d + g * 4l + 0l] = f.x
                yrp[tk * d + g * 4l + 1l] = f.y
                yrp[tk * d + g * 4l + 2l] = f.z
                yrp[tk * d + g * 4l + 3l] = f.w
            }
        }
    }
    delete q4
    delete s4
    return <- fx
}

// suffix -> repack interleave, off the layout companion's variants registry (lockstep with
// the tile variants by construction — same suffixes, same decline predicate)
def variant_mrs() : table<string; int> {
    var mrs : table<string; int>
    var lvs <- q8q8_layout_gen_variants()
    for (v in lvs) {
        mrs[v._0] = invoke(v._1)
    }
    delete lvs
    return <- mrs
}

// suffix -> weight-plane bias, off the wbias companion's variants registry (same lockstep)
def variant_wbias() : table<string; int> {
    var wbs : table<string; int>
    var bvs <- q8q8_wbias_gen_variants()
    for (v in bvs) {
        wbs[v._0] = invoke(v._1)
    }
    delete bvs
    return <- wbs
}

// suffix -> plane kgroup, off the kgroup companion's variants registry (same lockstep; 8 on
// the smmla rows — the repack below must write the row-pair plane those tiles read)
def variant_kgroups() : table<string; int> {
    var kgs : table<string; int>
    var kvs <- q8q8_kgroup_gen_variants()
    for (v in kvs) {
        kgs[v._0] = invoke(v._1)
    }
    delete kvs
    return <- kgs
}

// suffix -> tokens per tile call, off the tokstep companion's variants registry (same
// lockstep; 16/32 on the amx rows, whose macro also spans ts/16 row-groups — the walk below
// must match the stamped shape)
def variant_toksteps() : table<string; int> {
    var tss : table<string; int>
    var tvs <- q8q8_tokstep_gen_variants()
    for (v in tvs) {
        tss[v._0] = invoke(v._1)
    }
    delete tvs
    return <- tss
}

// one full tile sweep: every (ts-token, gs-group) unit through `tile`, on the variant's own
// repacked buffers (q/s); the fixture's bsums plane rides along (biased variants read it,
// everything else ignores the parameter). Fixtures divide evenly (ntok % ts == 0, groups %
// gs == 0) — production tails are the batch wrapper's job, not the tile's.
def run_tile(tile, cfg; fx : Fixture; q : array<int8>; s : array<float>; mr, ts, gs : int64; var y : array<float>) {
    let ng = fx.d / mr
    verify(fx.ntok % ts == 0l && ng % gs == 0l)
    invoke(cfg)     // latch rows: load the TMUL palette (the wrapper's per-chunk call)
    unsafe {
        let wp = addr(q[0])
        let sp = addr(s[0])
        let xqp = addr(fx.xq[0])
        let xsp = addr(fx.xs[0])
        let xbsp = addr(fx.xbs[0])
        var yp = addr(y[0])
        for (u in range64(ng / gs)) {
            let g = u * gs
            let wgp = wp + g * mr * fx.n
            let sgp = sp + g * mr * fx.nb
            var t0 = 0l
            while (t0 < fx.ntok) {
                invoke(tile, yp, wgp, sgp, xqp, xsp, xbsp, fx.n, fx.d, g, t0)
                t0 += ts
            }
        }
    }
}

// one gemv rows-core sweep over all group rows for token 0 (the group loop is inside the
// generated function — one call, like a production dispatch chunk)
def run_gemv(gemv; fx : Fixture; q : array<int8>; s : array<float>; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(q[0]), addr(s[0]), addr(fx.xq[0]), addr(fx.xs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

// suffix -> gemv rows-core variant, keyed like the tile registry (same perm rows by
// construction)
def gemv_variants_by_suffix() : table<string; MatmulQ8Q8RowsFn> {
    var tab : table<string; MatmulQ8Q8RowsFn>
    var gvs <- q8q8_gemv_gen_variants()
    for (v in gvs) {
        tab[v._0] = v._1
    }
    delete gvs
    return <- tab
}

// suffix -> witness variant (the five-function stamp's availability bit), keyed like the
// tile registry
def witness_variants_by_suffix() : table<string; function<() : bool>> {
    var tab : table<string; function<() : bool>>
    var wvs <- q8q8_family_live_variants()
    for (v in wvs) {
        tab[v._0] = v._1
    }
    delete wvs
    return <- tab
}

// suffix -> tile-config companion (slice K I1), keyed like the tile registry. This harness
// invokes stamped tiles directly, so it mirrors the batch wrapper: cfg before every tile
// sweep — an internal-substrate amx tile TILERELEASEs at exit, which would leave a following
// latch (raw-tmm) tile with no palette. No-op for every non-latch row.
def cfg_variants_by_suffix() : table<string; function<() : void>> {
    var tab : table<string; function<() : void>>
    var cvs <- q8q8_amx_cfg_gen_variants()
    for (v in cvs) {
        tab[v._0] = v._1
    }
    delete cvs
    return <- tab
}

// the row's tile knobs: the suffix minus its gemv piece — rows carrying gkstep spell the
// tile knobs explicitly, so "kstep2_gkstep2" keys with "kstep2"
def tile_key(suffix : string) : string {
    let i = find(suffix, "_gkstep")
    return i < 0 ? suffix : slice(suffix, 0, i)
}

// ===== the mx4 gemv companion (slice E) =====

typedef Mx4RowsFn = function<(var yp : float?; wn : uint8 const?; we : uint8 const?; xqp : int8 const?; xsp : float const?; n : int64; rb : int64; re : int64) : void>

struct Mx4Fixture {
    n, d, nb : int64
    wn : array<uint8>     // row-major nibble plane master — repacked per variant into a copy
    we : array<uint8>     // row-major E8M0 exponent plane master
    xq : array<int8>
    xs : array<float>
    yref : array<float>
}

def build_mx4_fixture(n, d : int64) : Mx4Fixture {
    var fx = Mx4Fixture(n = n, d = d, nb = n / 32l)
    var seed = int4(20260704, 5, 9, 2)
    fx.wn <- [for (_i in range64(d * n / 2l)); uint8(random_int(seed) % 256)]
    fx.we <- [for (_i in range64(d * fx.nb)); uint8(118 + random_int(seed) % 14)]
    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(n)
    fx.xs |> resize(fx.nb)
    for (i in range64(n)) {
        xrow[i] = float(i % 23l) * 0.07 - 0.8
    }
    quantize_q8_0_into(xrow, n, fx.xq, fx.xs, 0l, 0l)
    // the oracle: hand interleaved-mx4 semantics off a grp4 plane copy (fold order is
    // layout-independent, like the Q8 oracle)
    fx.yref |> resize(d)
    var n4 := fx.wn
    var e4 := fx.we
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    unsafe {
        repack_mx4_grp(addr(n4[0]), addr(e4[0]), n, d, 4l)
        let lutp = addr(lut[0])
        let np = addr(n4[0])
        let ep = addr(e4[0])
        var yrp = addr(fx.yref[0])
        for (g in range64(d / 4l)) {
            let f = dot_mx4q8_laneq4(lutp, np + g * 4l * (n / 2l), ep + g * 4l * fx.nb, addr(fx.xq[0]), addr(fx.xs[0]), n)
            yrp[g * 4l + 0l] = f.x
            yrp[g * 4l + 1l] = f.y
            yrp[g * 4l + 2l] = f.z
            yrp[g * 4l + 3l] = f.w
        }
    }
    delete n4
    delete e4
    return <- fx
}

def mx4_variants_by_suffix() : table<string; Mx4RowsFn> {
    var tab : table<string; Mx4RowsFn>
    var gvs <- mx4q8_gemv_gen_variants()
    for (v in gvs) {
        tab[v._0] = v._1
    }
    delete gvs
    return <- tab
}

def run_mx4_gemv(gemv; fx : Mx4Fixture; wn : array<uint8>; we : array<uint8>; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(wn[0]), addr(we[0]), addr(fx.xq[0]), addr(fx.xs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

def close_to_mx4_ref(fx : Mx4Fixture; y : array<float>; var maxdiff : float&) : bool {
    var ok = true
    for (i in range(length(y))) {
        let d = abs(y[i] - fx.yref[i])
        maxdiff = max(maxdiff, d)
        ok &&= d <= max(CLOSE_ABS, CLOSE_REL * abs(fx.yref[i]))
    }
    return ok
}

// ===== the kq companions (kq stage 4/5 — k4/k5/k6 gemv + tile) =====

typedef KqRowsFn = function<(var yp : float?; kqp : uint8 const?; ksp : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; rb : int64; re : int64) : void>
typedef KqTileFn = function<(var yp : float?; kqg : uint8 const?; ksg : uint8 const?; xqp : int8 const?; xsp : float const?; xbsp : int const?; n : int64; d : int64; g : int64; t0 : int64) : void>

struct KqFixture {
    fmt : int64           // 4 / 5 / 6 / 40
    n, d, ntok, nsb : int64
    kq : array<uint8>     // row-major master planes — repacked per variant into a copy
    ks : array<uint8>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>      // plain per-16 quant sums (the kq bsums contract)
    yref : array<float>
}

// int64 forwards to the ONE stride source (dasllama_gemm_schema) — the fixtures carry fmt as
// int64; the schema helpers keep the panic-on-unknown contract
def kq_qsb(fmt : int64) : int64 => dasllama_gemm_schema::kq_qsb(int(fmt))
def kq_ssb(fmt : int64) : int64 => dasllama_gemm_schema::kq_ssb(int(fmt))

// synthetic disk superblocks, varied per (row, superblock) so no two blocks repeat: any byte
// is a legal quant, and the small d/dmin keep the f16 (s, o) pairs finite and row sums tame
def pack_kq_scale_header(var blkb : array<uint8>; base : int) {
    let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
    let mbits = f32_to_f16(0.001 + 0.0001 * float(base % 7))
    blkb[0] = uint8(dbits & 0xFF)
    blkb[1] = uint8(dbits >> 8u)
    blkb[2] = uint8(mbits & 0xFF)
    blkb[3] = uint8(mbits >> 8u)
    for (j in range(4)) {
        let sc0 = (base * 5 + j * 11) % 64
        let sc4 = (base * 5 + (j + 4) * 11) % 64
        let mn0 = (base * 3 + j * 13) % 64
        let mn4 = (base * 3 + (j + 4) * 13) % 64
        blkb[4 + j] = uint8((sc0 & 63) | (((sc4 >> 4) & 3) << 6))
        blkb[8 + j] = uint8((mn0 & 63) | (((mn4 >> 4) & 3) << 6))
        blkb[12 + j] = uint8((sc4 & 15) | ((mn4 & 15) << 4))
    }
}

def pack_kq_disk_block(fmt : int64; var blkb : array<uint8>; base : int) {
    if (fmt == 40l) {
        for (blk in range(8)) {   // 8 x 18B q4_0 disk blocks: f16 d + 16 nibble bytes
            let dbits = f32_to_f16(0.002 + 0.0001 * float((base + blk) % 13))
            blkb[blk * 18] = uint8(dbits & 0xFF)
            blkb[blk * 18 + 1] = uint8(dbits >> 8u)
            for (i in range(16)) {
                blkb[blk * 18 + 2 + i] = uint8((base * 37 + blk * 59 + i * 101 + 29) % 256)
            }
        }
    } elif (fmt == 4l) {
        pack_kq_scale_header(blkb, base)
        for (i in range(128)) {
            blkb[16 + i] = uint8((base * 37 + i * 101 + 29) % 256)
        }
    } elif (fmt == 5l) {
        pack_kq_scale_header(blkb, base)
        for (i in range(32)) {
            blkb[16 + i] = uint8((base * 53 + i * 89 + 17) % 256)   // qh
        }
        for (i in range(128)) {
            blkb[48 + i] = uint8((base * 37 + i * 101 + 29) % 256)  // qs
        }
    } else {
        for (i in range(128)) {
            blkb[i] = uint8((base * 37 + i * 101 + 29) % 256)       // ql
        }
        for (i in range(64)) {
            blkb[128 + i] = uint8((base * 53 + i * 89 + 17) % 256)  // qh
        }
        for (i in range(16)) {
            blkb[192 + i] = uint8((base * 29 + i * 71 + 5) % 256)   // signed sub-scales
        }
        let dbits = f32_to_f16(0.002 + 0.0001 * float(base % 13))
        blkb[208] = uint8(dbits & 0xFF)
        blkb[209] = uint8(dbits >> 8u)
    }
}

def repack_kq_grp_fmt(fmt : int64; var kq : uint8?; var ks : uint8?; n, d, mr : int64) {
    if (fmt == 4l) {
        repack_k4_grp(kq, ks, n, d, mr)
    } elif (fmt == 5l) {
        repack_k5_grp(kq, ks, n, d, mr)
    } elif (fmt == 40l) {
        repack_q40_grp(kq, ks, n, d, mr)
    } else {
        repack_k6_grp(kq, ks, n, d, mr)
    }
}

def build_kq_fixture(fmt : int64; n, d, ntok : int64) : KqFixture {
    var fx = KqFixture(fmt = fmt, n = n, d = d, ntok = ntok, nsb = n / 256l)
    let qsb = kq_qsb(fmt)
    let ssb = kq_ssb(fmt)
    fx.kq |> resize(d * fx.nsb * qsb)
    fx.ks |> resize(d * fx.nsb * ssb)
    var blkb : array<uint8>
    blkb |> resize(fmt == 4l || fmt == 40l ? 144l : (fmt == 5l ? 176l : 210l))
    for (r in range64(d)) {
        for (s in range64(fx.nsb)) {
            let base = int(r * 31l + s * 7l)
            pack_kq_disk_block(fmt, blkb, base)
            let sb = r * fx.nsb + s
            if (fmt == 4l) {
                transcode_q4k_superblock(blkb, 0l, fx.kq, sb * qsb, fx.ks, sb * ssb)
            } elif (fmt == 5l) {
                transcode_q5k_superblock(blkb, 0l, fx.kq, sb * qsb, fx.ks, sb * ssb)
            } elif (fmt == 40l) {
                transcode_q40_superblock(blkb, 0l, fx.kq, sb * qsb, fx.ks, sb * ssb)
            } else {
                transcode_q6k_superblock(blkb, 0l, fx.kq, sb * qsb, fx.ks, sb * ssb)
            }
        }
    }
    delete blkb
    // activations: the production quantizer, whose bsums plane IS the kq contract (per-16
    // sums). kq v2: every format takes the Q8_K form — ONE scale per 256-superblock
    var xrow : array<float>
    xrow |> resize(n)
    fx.xq |> resize(ntok * n)
    fx.xs |> resize(ntok * (n / 256l))
    fx.xbs |> resize(ntok * (n / 16l))
    for (tk in range64(ntok)) {
        for (i in range64(n)) {
            xrow[i] = float((i + tk * 7l) % 23l) * 0.07 - 0.8
        }
        quantize_q8_k_into(xrow, n, fx.xq, fx.xs, fx.xbs, tk * n, tk * (n / 256l), tk * (n / 16l))
    }
    delete xrow
    // the oracle: the scalar grp walk off a grp4 copy (fold order is layout-independent)
    fx.yref |> resize(ntok * d)
    var q4 := fx.kq
    var s4 := fx.ks
    unsafe {
        repack_kq_grp_fmt(fmt, addr(q4[0]), addr(s4[0]), n, d, 4l)
        for (g in range64(d / 4l)) {
            let kqg = addr(q4[0]) + g * 4l * fx.nsb * qsb
            let ksg = addr(s4[0]) + g * 4l * fx.nsb * ssb
            for (tk in range64(ntok)) {
                for (r in range64(4l)) {
                    fx.yref[tk * d + g * 4l + r] = (fmt == 40l
                        ? q40_grp_row_dot(kqg, ksg, r, 4l,
                            addr(fx.xq[0]) + tk * n, addr(fx.xs[0]) + tk * (n / 256l), addr(fx.xbs[0]) + tk * (n / 16l), n)
                        : kq_grp_row_dot(fmt, kqg, ksg, r, 4l,
                            addr(fx.xq[0]) + tk * n, addr(fx.xs[0]) + tk * (n / 256l), addr(fx.xbs[0]) + tk * (n / 16l), n))
                }
            }
        }
    }
    delete q4
    delete s4
    return <- fx
}

// each format is its own [tune] family since the per-format split — the fmt pickers below
// keep the gate/bench loops family-generic
def kq_tile_variants(fmt : int64) : array<tuple<string; KqTileFn>> {
    if (fmt == 4l) {
        return <- k4q8_tile_gen_variants()
    }
    if (fmt == 5l) {
        return <- k5q8_tile_gen_variants()
    }
    if (fmt == 40l) {
        return <- q40q8_tile_gen_variants()
    }
    return <- k6q8_tile_gen_variants()
}

def kq_gemv_variants_by_suffix(fmt : int64) : table<string; KqRowsFn> {
    var tab : table<string; KqRowsFn>
    if (fmt == 4l) {
        var gvs <- k4q8_gemv_gen_variants()
        for (v in gvs) {
            tab[v._0] = v._1
        }
        delete gvs
    } elif (fmt == 5l) {
        var gvs <- k5q8_gemv_gen_variants()
        for (v in gvs) {
            tab[v._0] = v._1
        }
        delete gvs
    } elif (fmt == 40l) {
        var gvs <- q40q8_gemv_gen_variants()
        for (v in gvs) {
            tab[v._0] = v._1
        }
        delete gvs
    } else {
        var gvs <- k6q8_gemv_gen_variants()
        for (v in gvs) {
            tab[v._0] = v._1
        }
        delete gvs
    }
    return <- tab
}

// suffix -> plane interleave off the family's OWN layout companion registry (per-format
// layouts since the family split — declined rows report the reference grp4)
def kq_layout_mrs(fmt : int64) : table<string; int> {
    var mrs : table<string; int>
    if (fmt == 4l) {
        var lvs <- k4q8_layout_gen_variants()
        for (v in lvs) {
            mrs[v._0] = invoke(v._1)
        }
        delete lvs
    } elif (fmt == 5l) {
        var lvs <- k5q8_layout_gen_variants()
        for (v in lvs) {
            mrs[v._0] = invoke(v._1)
        }
        delete lvs
    } elif (fmt == 40l) {
        var lvs <- q40q8_layout_gen_variants()
        for (v in lvs) {
            mrs[v._0] = invoke(v._1)
        }
        delete lvs
    } else {
        var lvs <- k6q8_layout_gen_variants()
        for (v in lvs) {
            mrs[v._0] = invoke(v._1)
        }
        delete lvs
    }
    return <- mrs
}

// one kq tile sweep: every (4-token, group) unit through `tile` on the variant's grp<mr>
// planes — the kq tile contract is always 4 tokens per call (no amx tokstep: the kq planes
// have no TMUL form, amx perms ride the busd lattice). k5/k6 tiles read the byte-expanded
// panel unpacked per group (the batch cell's amortized shape — the bench cost is honest)
def run_kq_tile(tile; fx : KqFixture; kq : array<uint8>; ks : array<uint8>; mr : int64; var y : array<float>) {
    let ng = fx.d / mr
    verify(fx.ntok % 4l == 0l)
    let packed = fx.fmt == 4l || fx.fmt == 40l   // pure-nibble planes: the tile reads them directly
    var panel : array<uint8>
    if (!packed) {
        panel |> resize(int(mr * fx.nsb * 256l))
    }
    unsafe {
        let kqp = addr(kq[0])
        let ksp = addr(ks[0])
        let xqp = addr(fx.xq[0])
        let xsp = addr(fx.xs[0])
        let xbsp = addr(fx.xbs[0])
        var yp = addr(y[0])
        for (g in range64(ng)) {
            var kqg = kqp + g * mr * fx.nsb * kq_qsb(fx.fmt)
            let ksg = ksp + g * mr * fx.nsb * kq_ssb(fx.fmt)
            if (!packed) {
                unpack_kq_panel_grp(fx.fmt, kqg, addr(panel[0]), mr, fx.nsb)
                kqg = addr(panel[0])
            }
            var t0 = 0l
            while (t0 < fx.ntok) {
                invoke(tile, yp, kqg, ksg, xqp, xsp, xbsp, fx.n, fx.d, g, t0)
                t0 += 4l
            }
        }
    }
    delete panel
}

// one kq gemv rows-core sweep over all rows for token 0 (the group walk is inside the
// generated function — one call, like a production dispatch chunk)
def run_kq_gemv(gemv; fx : KqFixture; kq : array<uint8>; ks : array<uint8>; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(kq[0]), addr(ks[0]), addr(fx.xq[0]), addr(fx.xs[0]), addr(fx.xbs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

// k6's signed sub-scales cancel within a row, so element-relative tolerance is too strict at
// production shapes (intermediate terms are large, the sum is small) — gate against the
// fixture's magnitude instead; a real generator bug (wrong stride/lane/scale) is orders louder
// than fast-math fold-order noise
def kq_tol(fx : KqFixture; count : int) : float {
    var mag = 0.0
    for (i in range(count)) {
        mag = max(mag, abs(fx.yref[i]))
    }
    return max(CLOSE_ABS, CLOSE_REL * mag)
}

def close_to_kq_ref(fx : KqFixture; y : array<float>; var maxdiff : float&) : bool {
    let tol = kq_tol(fx, length(y))
    var ok = true
    for (i in range(length(y))) {
        let d = abs(y[i] - fx.yref[i])
        maxdiff = max(maxdiff, d)
        ok &&= d <= tol
    }
    return ok
}

// one variant's kq gate over a format's fixture set: repack per-variant copies at the
// variant's mr, run tile + gemv (token 0), compare against the scalar oracle
def kq_gate_variant(suffix : string; tile : KqTileFn; gemv : KqRowsFn;
                    kfxs : array<KqFixture>; mr : int64; var maxdiff : float&) : bool {
    var ok = true
    for (kfx in kfxs) {
        var kqv := kfx.kq
        var ksv := kfx.ks
        unsafe {
            repack_kq_grp_fmt(kfx.fmt, addr(kqv[0]), addr(ksv[0]), kfx.n, kfx.d, mr)
        }
        var yk : array<float>
        yk |> resize(kfx.ntok * kfx.d)
        run_kq_tile(tile, kfx, kqv, ksv, mr, yk)
        let tok = close_to_kq_ref(kfx, yk, maxdiff)
        if (!tok) {
            let tol = kq_tol(kfx, length(yk))
            var shown = 0
            for (i in range(length(yk))) {
                if (abs(yk[i] - kfx.yref[i]) > tol && shown < 6) {
                    tune_detail("  DBG k{kfx.fmt} tile {suffix} n={kfx.n}: y[{i}] (tok {i / int(kfx.d)} col {i % int(kfx.d)}) = {yk[i]} want {kfx.yref[i]}\n")
                    shown++
                }
            }
        }
        ok = ok && tok
        var ykg : array<float>
        ykg |> resize(kfx.d)
        run_kq_gemv(gemv, kfx, kqv, ksv, mr, ykg)
        let gok = close_to_kq_ref(kfx, ykg, maxdiff)
        if (!gok) {
            let tol = kq_tol(kfx, length(ykg))
            var shown = 0
            for (i in range(length(ykg))) {
                if (abs(ykg[i] - kfx.yref[i]) > tol && shown < 6) {
                    tune_detail("  DBG k{kfx.fmt} gemv {suffix} n={kfx.n}: y[{i}] = {ykg[i]} want {kfx.yref[i]}\n")
                    shown++
                }
            }
        }
        ok = ok && gok
        delete yk
        delete ykg
        delete kqv
        delete ksv
    }
    return ok
}

// one kq family's test pass: every variant of the format's OWN grid against the scalar oracle
// ===== the q51 GEMV family (self-anchored, sdot-only grid) =====

struct Q51Fixture {
    n, d : int64
    nb : int64
    qq : array<uint8>       // disk-order 20B/block quant plane
    qs : array<uint8>       // disk-order 4B/block f16 (d, m) scale plane
    xq : array<int8>
    xs : array<float>
    xbs : array<int>        // per-32 activation sums (the q51 gemv contract)
    yref : array<float>
}

def build_q51_fixture(n, d : int64) : Q51Fixture {
    var fx = Q51Fixture(n = n, d = d, nb = n / 32l)
    fx.qq |> resize(d * fx.nb * 20l)
    fx.qs |> resize(d * fx.nb * 4l)
    // valid small f16 scale bit patterns (d, m) cycled per block; quant/qh bytes an LCG walk
    let f16s = fixed_array(0x2E66, 0x3266, 0xB18F, 0x2C00, 0x3000, 0xB266, 0x2A8F, 0x3400)
    var seed = 0x12345
    for (i in range64(length(fx.qq))) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff
        fx.qq[i] = uint8(seed >> 16)
    }
    for (r in range64(d)) {
        for (bi in range64(fx.nb)) {
            let sb = (r * fx.nb + bi) * 4l
            let dv = f16s[int(r * 3l + bi) % 8]
            let mv = f16s[int(r * 5l + bi * 7l + 3l) % 8]
            fx.qs[sb] = uint8(dv & 255)
            fx.qs[sb + 1l] = uint8(dv >> 8)
            fx.qs[sb + 2l] = uint8(mv & 255)
            fx.qs[sb + 3l] = uint8(mv >> 8)
        }
    }
    var xrow : array<float>
    xrow |> resize(n)
    for (i in range64(n)) {
        xrow[i] = float((i * 13l) % 37l) * 0.05 - 0.9
    }
    fx.xq |> resize(n)
    fx.xs |> resize(fx.nb)
    quantize_q8_0_into(xrow, n, fx.xq, fx.xs, 0l, 0l)
    delete xrow
    fx.xbs |> resize(fx.nb)
    for (bi in range64(fx.nb)) {
        var s = 0
        for (j in range64(32l)) {
            s += int(fx.xq[bi * 32l + j])
        }
        fx.xbs[bi] = s
    }
    fx.yref |> resize(d)
    unsafe {
        for (r in range64(d)) {
            fx.yref[r] = dot_q51q8_scalar(addr(fx.qq[0]) + r * fx.nb * 20l, addr(fx.qs[0]) + r * fx.nb * 4l,
                                          addr(fx.xq[0]), addr(fx.xs[0]), n)
        }
    }
    return <- fx
}

def run_q51_gemv(gemv; fx : Q51Fixture; qq : array<uint8>; qs : array<uint8>; mr : int64; var y : array<float>) {
    unsafe {
        invoke(gemv, addr(y[0]), addr(qq[0]), addr(qs[0]), addr(fx.xq[0]), addr(fx.xs[0]), addr(fx.xbs[0]), fx.n, 0l, (fx.d / mr) * mr)
    }
}

def close_to_q51_ref(fx : Q51Fixture; y : array<float>; mr : int64; var maxdiff : float&) : bool {
    var mag = 0.0
    let rows = int((fx.d / mr) * mr)
    for (i in range(rows)) {
        mag = max(mag, abs(fx.yref[i]))
    }
    let tol = max(1.0e-3, mag * 5.0e-5)
    var ok = true
    for (i in range(rows)) {
        let df = abs(y[i] - fx.yref[i])
        maxdiff = max(maxdiff, df)
        ok = ok && df <= tol
    }
    return ok
}

// per-variant plane interleave off the layout companion registry — declined perms report the
// reference resolve (the lockstep rail). Trusting the SUFFIX instead repacks planes to a grp
// the declined-to-reference kernel then walks at the companion mr — the zen2 maxdiff-120 trap.
def private q51_layout_mrs() : table<string; int> {
    var mrs : table<string; int>
    var lvs <- q51q8_layout_gen_variants()
    for (v in lvs) {
        mrs[v._0] = invoke(v._1)
    }
    delete lvs
    return <- mrs
}

// The q51 family tune pass: gate every variant against the scalar disk oracle on repacked
// planes, iso-bench the streamed decode shape, return the best suffix ("" = a gate failed).
def q51_tune_family(var rows_out : table<string; JsonValue?>) : string {
    var sfx <- build_q51_fixture(704l, 32768l)   // streamed: the 26B expert-down K at DRAM scale
    var hfx <- build_q51_fixture(704l, 2816l)    // hot: one expert's down stack
    var tvs <- q51q8_gemv_gen_variants()
    var mrs <- q51_layout_mrs()
    var best = ""
    var bestDt = 2147483647
    var y : array<float>
    y |> resize(sfx.d)
    var yh : array<float>
    yh |> resize(hfx.d)
    for (v in tvs) {
        let mr = int64(mrs?[v._0] ?? q51q8_layout_gen())
        var sq := sfx.qq
        var ss := sfx.qs
        var hq := hfx.qq
        var hs := hfx.qs
        unsafe {
            repack_q51_grp(addr(sq[0]), addr(ss[0]), sfx.n, sfx.d, mr)
            repack_q51_grp(addr(hq[0]), addr(hs[0]), hfx.n, hfx.d, mr)
        }
        var maxdiff = 0.0
        run_q51_gemv(v._1, sfx, sq, ss, mr, y)
        var ok = close_to_q51_ref(sfx, y, mr, maxdiff)
        run_q51_gemv(v._1, hfx, hq, hs, mr, yh)
        ok = close_to_q51_ref(hfx, yh, mr, maxdiff) && ok
        if (!ok) {
            // best = "" rejects the family; the shared cleanup past the loop frees everything
            // (deleting tvs HERE, mid-iteration, is the locked-array trap)
            print("q51 {v._0} (mr={mr}): MISMATCH (maxdiff {maxdiff}) - aborting tune\n")
            delete sq; delete ss; delete hq; delete hs
            best = ""
            break
        }
        var dt = 2147483647
        for (_rep in range(3)) {
            let t0 = ref_time_ticks()
            run_q51_gemv(v._1, sfx, sq, ss, mr, y)
            dt = min(dt, get_time_usec(t0))
        }
        let gb = double(sfx.d * sfx.nb * 24l) / double(max(dt, 1)) / 1000.0lf
        rows_out[v._0] = JV((streamed_us = double(dt)))
        tune_detail("q51 {v._0} (mr={mr}): ok (maxdiff {maxdiff}), streamed {dt} us ({gb:0.1f} GB/s)\n")
        if (dt < bestDt) {
            bestDt = dt
            best = v._0
        }
        delete sq; delete ss; delete hq; delete hs
    }
    delete sfx
    delete hfx
    delete tvs
    delete mrs
    delete y
    delete yh
    return best
}

def kq_test_family(fmt : int64; kfxs : array<KqFixture>) : bool {
    var tvs <- kq_tile_variants(fmt)
    var gtab <- kq_gemv_variants_by_suffix(fmt)
    var mrs <- kq_layout_mrs(fmt)
    var allok = true
    for (v in tvs) {
        let mr = int64(mrs?[v._0] ?? 4)
        verify(gtab |> key_exists(v._0))
        var maxdiff = 0.0
        let ok = kq_gate_variant(v._0, v._1, gtab[v._0], kfxs, mr, maxdiff)
        print("k{fmt} {v._0} (mr={mr}): {ok ? "ok" : "MISMATCH"} (maxdiff {maxdiff})\n")
        allok = allok && ok
    }
    delete mrs
    delete gtab
    delete tvs
    return allok
}

// one kq family's tune pass: correctness-gate every variant, iso-bench the tile (batch shape)
// and the gemv (streamed + hot decode shapes), print the table, and return the tile-best
// suffix ("" = a gate failed). The kq gemv is nrsplit-independent — same-mr rows share the
// plane and the gemv shape — so the tile bench decides the family entry.
def kq_tune_family(fmt : int64; var rows_out : table<string; JsonValue?>) : string { // nolint:STYLE038 — one family race: shared repacked buffers couple gate, bench and pick
    // batch shape: fat STREAMING ffn (weights > the L2 budget, multi-token-block walk) — the shape
    // class that carries ~90% of prefill time. The old 2048x512x64 kv-projection probe was L2-hot
    // and crowned hot-shape winners that lose double-digit % at model scale (mr4 vs mr8, M1).
    var kbfx <- build_kq_fixture(fmt, 2048l, 8192l, 256l)
    var kgfx <- build_kq_fixture(fmt, 2048l, 8192l, 1l)      // streamed decode shape (DRAM-bound ffn)
    var khfx <- build_kq_fixture(fmt, 2048l, 512l, 1l)       // hot decode shape (cache-resident)
    var tvs <- kq_tile_variants(fmt)
    var gtab <- kq_gemv_variants_by_suffix(fmt)
    var mrs <- kq_layout_mrs(fmt)
    let nv = length(tvs)
    var vmr : array<int64>
    vmr |> resize(nv)
    var kmrs : array<int64>   // kq planes depend only on (format, mr) — dedupe repacks by mr
    for (vi in range(nv)) {
        vmr[vi] = int64(mrs?[tvs[vi]._0] ?? 4)
        var seen = false
        for (m in kmrs) {
            seen ||= m == vmr[vi]
        }
        if (!seen) {
            kmrs |> push(vmr[vi])
        }
    }
    let nmr = length(kmrs)
    var bbuf : array<KqBuf>
    var gbuf : array<KqBuf>
    var hbuf : array<KqBuf>
    bbuf |> resize(nmr)
    gbuf |> resize(nmr)
    hbuf |> resize(nmr)
    for (mi in range(nmr)) {
        bbuf[mi].q := kbfx.kq
        bbuf[mi].s := kbfx.ks
        gbuf[mi].q := kgfx.kq
        gbuf[mi].s := kgfx.ks
        hbuf[mi].q := khfx.kq
        hbuf[mi].s := khfx.ks
        unsafe {
            repack_kq_grp_fmt(fmt, addr(bbuf[mi].q[0]), addr(bbuf[mi].s[0]), kbfx.n, kbfx.d, kmrs[mi])
            repack_kq_grp_fmt(fmt, addr(gbuf[mi].q[0]), addr(gbuf[mi].s[0]), kgfx.n, kgfx.d, kmrs[mi])
            repack_kq_grp_fmt(fmt, addr(hbuf[mi].q[0]), addr(hbuf[mi].s[0]), khfx.n, khfx.d, kmrs[mi])
        }
    }
    var ky : array<float>
    ky |> resize(kbfx.ntok * kbfx.d)
    var kyg : array<float>
    kyg |> resize(kgfx.d)
    var kyh : array<float>
    kyh |> resize(khfx.d)
    // correctness first — a tune run must never crown a broken variant
    for (vi in range(nv)) {
        verify(gtab |> key_exists(tvs[vi]._0))
        let mi = kq_mr_index(kmrs, vmr[vi])
        run_kq_tile(tvs[vi]._1, kbfx, bbuf[mi].q, bbuf[mi].s, vmr[vi], ky)
        var maxdiff = 0.0
        if (!close_to_kq_ref(kbfx, ky, maxdiff)) {
            print("k{fmt} {tvs[vi]._0}: tile MISMATCH (maxdiff {maxdiff}) - aborting tune\n")
            return ""
        }
        run_kq_gemv(gtab[tvs[vi]._0], khfx, hbuf[mi].q, hbuf[mi].s, vmr[vi], kyh)
        var gmaxdiff = 0.0
        if (!close_to_kq_ref(khfx, kyh, gmaxdiff)) {
            print("k{fmt} {tvs[vi]._0}: gemv MISMATCH (maxdiff {gmaxdiff}) - aborting tune\n")
            return ""
        }
    }
    var best : array<double>
    var gbest : array<double>
    var hbest : array<double>
    best |> resize(nv)
    gbest |> resize(nv)
    hbest |> resize(nv)
    for (i in range(nv)) {
        best[i] = 1.0e30lf
        gbest[i] = 1.0e30lf
        hbest[i] = 1.0e30lf
    }
    let HOT_REPS = 16
    for (r in range(ROUNDS)) {   // interleaved best-of within the family
        tune_progress_kernel_step(r + 1, "best-of", length(tvs))
        for (vi in range(nv)) {
            let mi = kq_mr_index(kmrs, vmr[vi])
            let t0 = ref_time_ticks()
            run_kq_tile(tvs[vi]._1, kbfx, bbuf[mi].q, bbuf[mi].s, vmr[vi], ky)
            let us = double(get_time_usec(t0))
            if (us < best[vi]) {
                best[vi] = us
            }
            let g0 = ref_time_ticks()
            run_kq_gemv(gtab[tvs[vi]._0], kgfx, gbuf[mi].q, gbuf[mi].s, vmr[vi], kyg)
            let gus = double(get_time_usec(g0))
            if (gus < gbest[vi]) {
                gbest[vi] = gus
            }
            let h0 = ref_time_ticks()
            for (_h in range(HOT_REPS)) {
                run_kq_gemv(gtab[tvs[vi]._0], khfx, hbuf[mi].q, hbuf[mi].s, vmr[vi], kyh)
            }
            let hus = double(get_time_usec(h0)) / double(HOT_REPS)
            if (hus < hbest[vi]) {
                hbest[vi] = hus
            }
        }
    }
    let macs = double(kbfx.n * kbfx.d * kbfx.ntok)
    let gmacs = double(kgfx.n * kgfx.d)
    let hmacs = double(khfx.n * khfx.d)
    let gwbytes = double(kgfx.d * kgfx.nsb * (kq_qsb(fmt) + kq_ssb(fmt)))
    var winner = 0
    for (vi in range(nv)) {
        if (best[vi] < best[winner]) {
            winner = vi
        }
        rows_out[tvs[vi]._0] = JV((tile_us = best[vi], gemv_us = gbest[vi], hot_us = hbest[vi]))
        tune_detail("k{fmt} {tvs[vi]._0} (mr={vmr[vi]}): tile {macs / best[vi] / 1000.0lf} GMAC/s, gemv stream {gmacs / gbest[vi] / 1000.0lf} GMAC/s ({gwbytes / gbest[vi] / 1000.0lf} GB/s w), hot {hmacs / hbest[vi] / 1000.0lf} GMAC/s (best of {ROUNDS})\n")
    }
    let wname = tvs[winner]._0
    delete ky
    delete kyg
    delete kyh
    delete best
    delete gbest
    delete hbest
    delete bbuf
    delete gbuf
    delete hbuf
    delete kmrs
    delete vmr
    delete mrs
    delete gtab
    delete tvs
    delete kbfx
    delete kgfx
    delete khfx
    return wname
}

let CLOSE_REL = 1e-4
let CLOSE_ABS = 1e-5

// family entry -> rows object for the sidecar "race" section, filled as each family races and
// flushed with the winners — margins must be answerable from the file, not the scrollback
var private g_rows : table<string; JsonValue?>

def close_to_ref(fx : Fixture; y : array<float>; var maxdiff : float&) : bool {
    var ok = true
    for (i in range(length(y))) {
        let d = abs(y[i] - fx.yref[i])
        maxdiff = max(maxdiff, d)
        ok &&= d <= max(CLOSE_ABS, CLOSE_REL * abs(fx.yref[i]))
    }
    return ok
}

def test_mode : bool { // nolint:STYLE037,STYLE038 — flat per-variant correctness ladder; each desync check is one arm
    // nb=2 (even main loop), nb=3 (remainder path), production-ish slice — d divides every
    // grid mr (4/8 on arm64, 16/32 on the x64 rows when live); ntok=32 covers one full amx
    // 2×2 macro (and divides the 4/16-token steps of every other row)
    var fxs <- [ <- build_fixture(64l, 32l, 32l), <- build_fixture(96l, 32l, 32l), <- build_fixture(2048l, 64l, 32l)]
    var mfxs <- [ <- build_mx4_fixture(96l, 32l), <- build_mx4_fixture(2048l, 64l)]
    // kq shapes: nsb=1 (single superblock), nsb=3 (odd walk), production-ish; d divides every
    // grid mr, ntok covers two 4-token tile calls
    var kfxs4 <- [ <- build_kq_fixture(4l, 256l, 32l, 8l), <- build_kq_fixture(4l, 768l, 32l, 8l), <- build_kq_fixture(4l, 2048l, 32l, 8l)]
    var kfxs5 <- [ <- build_kq_fixture(5l, 256l, 32l, 8l), <- build_kq_fixture(5l, 768l, 32l, 8l), <- build_kq_fixture(5l, 2048l, 32l, 8l)]
    var kfxs6 <- [ <- build_kq_fixture(6l, 256l, 32l, 8l), <- build_kq_fixture(6l, 768l, 32l, 8l), <- build_kq_fixture(6l, 2048l, 32l, 8l)]
    var kfxs40 <- [ <- build_kq_fixture(40l, 256l, 32l, 8l), <- build_kq_fixture(40l, 768l, 32l, 8l), <- build_kq_fixture(40l, 2048l, 32l, 8l)]
    var vs <- q8q8_tile_gen_variants()
    var mrs <- variant_mrs()
    var wbs <- variant_wbias()
    var kgs <- variant_kgroups()
    var tss <- variant_toksteps()
    var gtab <- gemv_variants_by_suffix()
    var mtab <- mx4_variants_by_suffix()
    var wtab <- witness_variants_by_suffix()
    var ctab <- cfg_variants_by_suffix()
    var allok = true
    for (v in vs) {
        let mr = int64(mrs?[v._0] ?? 4)
        let wbias = int64(wbs?[v._0] ?? 0)
        let kgv = int64(kgs?[v._0] ?? 4)
        let tsv = int64(tss?[v._0] ?? 4)
        let gsv = max(tsv / 16l, 1l)
        verify(gtab |> key_exists(v._0))
        verify(mtab |> key_exists(v._0))
        verify(wtab |> key_exists(v._0))
        verify(ctab |> key_exists(v._0))
        // witness-lockstep invariants: the reference row is never "live", and a
        // non-reference layout, a biased plane, a kg8 plane or a non-4 tokstep proves the
        // family emitted — the witness must agree
        let live = invoke(wtab[v._0])
        if (v._0 == "reference") {
            verify(!live)
        }
        if (int(mr) != GEMM_REFERENCE_MR && !live) {
            print("  WITNESS DESYNC {v._0}: layout mr={mr} but witness says not live\n")
            allok = false
        }
        if (wbias != 0l && !live) {
            print("  WBIAS DESYNC {v._0}: wbias={wbias} but witness says not live\n")
            allok = false
        }
        if (kgv != 4l && !live) {
            print("  KGROUP DESYNC {v._0}: kgroup={kgv} but witness says not live\n")
            allok = false
        }
        if (tsv != 4l && !live) {
            print("  TOKSTEP DESYNC {v._0}: tokstep={tsv} but witness says not live\n")
            allok = false
        }
        var ok = true
        var gok = true
        var maxdiff = 0.0
        var gmaxdiff = 0.0
        for (fx in fxs) {
            var qv := fx.q
            var sv := fx.s
            unsafe {
                repack_q8q8_grp(addr(qv[0]), addr(sv[0]), fx.n, fx.d, mr, wbias, kgv)
            }
            var y : array<float>
            y |> resize(fx.ntok * fx.d)
            run_tile(v._1, ctab[v._0], fx, qv, sv, mr, tsv, gsv, y)
            let fxok = close_to_ref(fx, y, maxdiff)
            if (!fxok) {
                var shown = 0
                for (i in range(length(y))) {
                    if (abs(y[i] - fx.yref[i]) > max(CLOSE_ABS, CLOSE_REL * abs(fx.yref[i])) && shown < 6) {
                        tune_detail("  DBG {v._0} n={fx.n}: y[{i}] (tok {i / int(fx.d)} col {i % int(fx.d)}) = {y[i]} want {fx.yref[i]}\n")
                        shown++
                    }
                }
            }
            ok = ok && fxok
            // the gemv rows core on the same buffers: token 0's row of the oracle
            var yg : array<float>
            yg |> resize(fx.d)
            run_gemv(gtab[v._0], fx, qv, sv, mr, yg)
            let gfxok = close_to_ref(fx, yg, gmaxdiff)
            if (!gfxok) {
                var shown = 0
                for (i in range(length(yg))) {
                    if (abs(yg[i] - fx.yref[i]) > max(CLOSE_ABS, CLOSE_REL * abs(fx.yref[i])) && shown < 6) {
                        tune_detail("  DBG gemv {v._0} n={fx.n}: y[{i}] = {yg[i]} want {fx.yref[i]}\n")
                        shown++
                    }
                }
            }
            gok = gok && gfxok
            delete y
            delete yg
            delete qv
            delete sv
        }
        // the mx4 gemv companion on its own plane fixtures (repacked per variant)
        var mok = true
        var mmaxdiff = 0.0
        for (mfx in mfxs) {
            var nv := mfx.wn
            var ev := mfx.we
            unsafe {
                repack_mx4_grp(addr(nv[0]), addr(ev[0]), mfx.n, mfx.d, mr)
            }
            var ym : array<float>
            ym |> resize(mfx.d)
            run_mx4_gemv(mtab[v._0], mfx, nv, ev, mr, ym)
            let mfxok = close_to_mx4_ref(mfx, ym, mmaxdiff)
            if (!mfxok) {
                var shown = 0
                for (i in range(length(ym))) {
                    if (abs(ym[i] - mfx.yref[i]) > max(CLOSE_ABS, CLOSE_REL * abs(mfx.yref[i])) && shown < 6) {
                        tune_detail("  DBG mx4 {v._0} n={mfx.n}: y[{i}] = {ym[i]} want {mfx.yref[i]}\n")
                        shown++
                    }
                }
            }
            mok = mok && mfxok
            delete ym
            delete nv
            delete ev
        }
        let biasTag = wbias != 0l ? " bias{wbias}" : ""
        let kgTag = kgv != 4l ? " kg{kgv}" : ""
        print("{v._0} (mr={mr}{biasTag}{kgTag}, {live ? "live" : "ref"}): tile {ok ? "ok" : "MISMATCH"} (maxdiff {maxdiff}), gemv {gok ? "ok" : "MISMATCH"} (maxdiff {gmaxdiff}), mx4 {mok ? "ok" : "MISMATCH"} (maxdiff {mmaxdiff})\n")
        allok = allok && ok && gok && mok
    }
    // the kq families gate their OWN grids (per-format [tune] entries since the family split)
    allok = kq_test_family(4l, kfxs4) && allok
    allok = kq_test_family(5l, kfxs5) && allok
    allok = kq_test_family(6l, kfxs6) && allok
    allok = kq_test_family(40l, kfxs40) && allok
    delete wtab
    delete mtab
    delete gtab
    delete ctab
    delete tss
    delete kgs
    delete wbs
    delete mrs
    delete vs
    delete mfxs
    delete fxs
    delete kfxs4
    delete kfxs5
    delete kfxs6
    delete kfxs40
    return allok
}

struct VBuf {
    q : array<int8>
    s : array<float>
}

struct KqBuf {
    q : array<uint8>
    s : array<uint8>
}

def kq_mr_index(kmrs : array<int64>; mr : int64) : int {
    for (i in range(length(kmrs))) {
        if (kmrs[i] == mr) {
            return i
        }
    }
    return 0
}

// ===== the e2e confirm pass (the SPR amx lesson: a 1-core crown must prove itself) =====
// The 1-core fixtures rank kernels in a regime real prefill never runs (one lane, no
// dispatch rendezvous, cache-warm weights) — on SPR they crowned amx_int8 while the same
// stamp lost end-to-end prefill 1.7-2.2x at every T. So a winner that DIVERGES from the
// per-ISA fallback default must beat it in a real-model prefill A/B (production stamps +
// box_profile knobs, interleaved, a crashing challenger = a rejection) before
// tune_manifest_set ships it. A winner that IS the fallback writes as before — the
// microbench keeps doing the ranking; the confirm only gates the unusual crown.

let CONFIRM_MARGIN = 1.02lf   // the challenger must beat the fallback e2e by >2%
let CONFIRM_ROUNDS = 2        // interleaved fallback/winner rounds, best-of per arm

def private env_cmd(envs : array<tuple<string; string>>; cmd : string) : string {
    return build_string() $(w) {
        let win = get_platform_name() == "windows"
        for (e in envs) {
            w |> write(win ? "set {e._0}={e._1}&& " : "{e._0}={e._1} ")
        }
        w |> write(cmd)
    }
}

// daspkg's run_cmd shape: tmp-file redirect so the child's stdout survives cmd.exe quirks.
// The command never starts with a quote (the env prefix leads), so no sacrificial-quote wrap.
def private run_child(cmd : string) : string {
    let tmp = "{get_das_root()}/_tune_confirm_out.txt"
    let full = "{cmd} > \"{tmp}\" 2>&1"
    var shellNoise = ""
    unsafe(popen(full) $(f) {
        if (f != null) {
            shellNoise = fread(f)
        }
    })
    var outp = ""
    if (fexist(tmp)) {
        fopen(tmp, "rb") $(f) {
            if (f != null) {
                outp = fread(f)
            }
        }
        remove(tmp)
    }
    return empty(outp) ? shellNoise : outp
}

def private harness_dir() : string => "{get_das_root()}/modules/dasLLAMA/harness"

def private parse_stamp_suffix(outp : string) : string {
    let tag = "llvm_tune: q8q8_tile_gen <- "
    for (ln in split(outp, "\n")) {
        let ix = find(ln, tag)
        if (ix >= 0) {
            let rest = slice(ln, ix + length(tag))
            let sp = find(rest, " ")
            return sp > 0 ? slice(rest, 0, sp) : rest
        }
    }
    return ""
}

def private parse_confirm_pp(outp : string) : double {
    let tag = "CONFIRM_PP: "
    for (ln in split(outp, "\n")) {
        let ix = find(ln, tag)
        if (ix >= 0) {
            return double(to_float(slice(ln, ix + length(tag))))
        }
    }
    return 0.0lf
}

def private compile_fallback(fname : string) : string {
    for (entry in split(g_env_core.tune_compile_fallbacks, ";")) {
        let eq = find(entry, "=")
        if (eq > 0 && slice(entry, 0, eq) == fname) {
            return slice(entry, eq + 1)
        }
    }
    return ""
}

// A release compile reports the fallback it just selected on this same host. Direct/manual
// tuner runs have no such handoff, so retain the compile-only child as their safe fallback.
def private discover_fallback_stamp() : string {
    let inherited = compile_fallback("q8q8_tile_gen")
    if (!empty(inherited)) {
        print("confirm: using q8q8 fallback '{inherited}' from the release compile on this host\n")
        return inherited
    }
    // verbose is MANDATORY on this child: the `llvm_tune: <fn> <- <perm>` stamp line this
    // parses is verbose-only, so at inherited-normal the discovery reads nothing and the
    // confirm gate silently degrades to unguarded-keep
    var envs <- [("DAS_TUNE_MODE" => "normal"), ("DAS_TUNE_MANIFEST" => ""),
        ("GEN_TUNE_COMPILE_ONLY" => "1"), ("DAS_TUNE_VERBOSITY" => "verbose")]
    let cmd = env_cmd(envs, "\"{get_das_exe()}\" -jit \"{harness_dir()}/gen_tune_probe.das\"")
    delete envs
    return parse_stamp_suffix(run_child(cmd))
}

// One confirm arm: real-model prefill under the given manifest (empty = the fallback chain).
// The stamp line doubles as the plumbing check — a wrong stamp scores 0 (= a rejection).
def private confirm_pp_run(model, manifest_path, expected : string) : double {
    // verbose for the same reason as the discovery child: the arm's stamp line IS the
    // plumbing check, and without it both arms score 0 and the confirm can never pass
    var envs <- [("DAS_TUNE_MODE" => "normal"), ("DAS_TUNE_MANIFEST" => manifest_path),
        ("DAS_TUNE_VERBOSITY" => "verbose")]
    let cmd = env_cmd(envs, "\"{get_das_exe()}\" -jit \"{harness_dir()}/tune_confirm_prefill.das\" -- \"{model}\"")
    delete envs
    let outp = run_child(cmd)
    let stamped = parse_stamp_suffix(outp)
    if (stamped != expected) {
        print("confirm: arm expected stamp '{expected}' but the child stamped '{stamped}' - scoring 0\n")
        return 0.0lf
    }
    let pp = parse_confirm_pp(outp)
    if (pp <= 0.0lf) {   // surface WHY (a swallowed 'cannot open gguf' once cost a debug round)
        var lns <- split(outp, "\n")
        let n = length(lns)
        print("confirm: arm '{expected}' produced no CONFIRM_PP - child output tail:\n")
        for (i in range(max(0, n - 6), n)) {
            if (!empty(lns[i])) print("confirm:   | {lns[i]}\n")
        }
        delete lns
    }
    return pp
}

// The confirm-model ladder: the env wins (the A/B rail); else the box's own models dir serves
// a q8 carrier — the historical confirm carrier first, else the largest present q8 (bigger
// matmuls = cleaner e2e margins). This is the LOCAL backup plan for unattended mints — a
// served per-architecture sidecar is the regular user's path — and it exists because every
// env-less mint used to pin the fallback silently (~4-6% pp on q8 models, twice in one day).
def private resolve_confirm_model() : string {
    if (!empty(g_env_harness.confirm_model)) {
        return g_env_harness.confirm_model
    }
    let md = models_dir_resolved()
    let preferred = "{md}gemma-4-E4B-it-Q8_0.gguf"
    if (stat(preferred).is_valid) {
        return preferred
    }
    var best = ""
    var bestBytes = 0ul
    dir(md) $(name : string) {
        return if ((!ends_with(name, "-Q8_0.gguf") && !ends_with(name, "-q8_0.gguf"))
            || find(name, "mmproj") >= 0 || starts_with(name, "ggml-vocab"))
        let sz = stat("{md}{name}").size
        if (sz > bestBytes) {
            bestBytes = sz
            best = "{md}{name}"
        }
    }
    return best
}

// the confirmed q8q8 crown to QUEUE for the end-of-run write ("" = hard failure — write
// nothing, fail the mint). All writes are deferred behind the end-of-run noise gate.
def private confirm_winner(winner : string) : string {
    let fb = discover_fallback_stamp()
    if (empty(fb)) {
        print("confirm: fallback-discovery child failed - keeping the winner unguarded (pre-confirm behavior)\n")
        return winner
    }
    if (fb == winner) {
        print("confirm: winner '{winner}' IS the per-ISA fallback - no e2e confirm needed\n")
        return winner
    }
    let model = resolve_confirm_model()
    if (empty(model)) {
        print("confirm: winner '{winner}' DIVERGES from the per-ISA fallback '{fb}' - a 1-core crown must confirm end-to-end before it ships (the SPR amx lesson)\n")
        print("confirm: NO q8 model on this box to confirm with - pinning the fallback '{fb}'. Provision one (fetch_models.das --fetch) or set DASLLAMA_CONFIRM_MODEL=<q8 gguf>\n")
        // pin the fallback so the sidecar is COMPLETE — leaving the entry missing makes the
        // startup completeness gate re-tune on every run (the runtime rides the fallback either way)
        return fb
    }
    print("confirm: model {model}{empty(g_env_harness.confirm_model) ? " (auto-resolved)" : " (env)"}\n")
    let tmpm = "{get_das_root()}/_tune_confirm_manifest.json"
    // the stamps read the manifest's "kernels" SECTION (read_manifest) — a flat root object
    // parses found-but-empty. And provenance.box is MANDATORY: the box-identity staleness rail
    // reads a box-less manifest as foreign -> absent, the challenger arm stamps the fallback,
    // and the confirm can never pass (both arms score 0 -> auto-reject)
    if (!fwrite(tmpm, "\{\"kernels\": \{\"q8q8_tile_gen\": \"{winner}\"\}, \"provenance\": \{\"box\": \"{tune_box_identity()}\"\}\}")) {
        print("confirm: cannot write {tmpm} - is das_root writable?\n")
        return ""
    }
    var bestF = 0.0lf
    var bestW = 0.0lf
    for (_r in range(CONFIRM_ROUNDS)) {
        bestF = max(bestF, confirm_pp_run(model, "", fb))
        bestW = max(bestW, confirm_pp_run(model, tmpm, winner))
    }
    remove(tmpm)
    if (bestF <= 0.0lf) {
        print("confirm: the FALLBACK arm produced no pp number - not writing anything (fix the box, rerun)\n")
        return ""
    }
    if (bestW > bestF * CONFIRM_MARGIN) {
        print("confirm: challenger '{winner}' {bestW} tok/s BEATS fallback '{fb}' {bestF} tok/s - CONFIRMED\n")
        return winner
    }
    print("confirm: challenger '{winner}' {bestW} tok/s does NOT beat fallback '{fb}' {bestF} tok/s by x{CONFIRM_MARGIN} - REJECTED, pinning the fallback\n")
    return fb
}

def tune_mode_run : bool { // nolint:STYLE037,STYLE038 — flat family-by-family race rail; gates + deferred flush in call order
    let tune_total_t0 = ref_time_ticks()
    // refuse before burning minutes measuring noise; every winner below queues into `pending`
    // and nothing reaches the sidecar until the end-of-run gate passes
    if (!noise_gate("start")) {
        return false
    }
    var pending : table<string; string>
    // q8q8 + four kq formats + q51 + the end-to-end confirm
    tune_progress_plan("dasllama_generators", 7)
    tune_progress_kernel_begin("q8q8_tile_gen", ROUNDS)
    // batch shape: fat STREAMING ffn like the kq fixture — the old 2048x512x64 kv-projection was
    // L2-hot; divergent crowns still pass the e2e confirm gate before shipping
    var fx <- build_fixture(2048l, 8192l, 256l)
    var gfx <- build_fixture(2048l, 8192l, 1l)     // streamed decode shape (DRAM-bound ffn GEMV)
    var hfx <- build_fixture(2048l, 512l, 1l)      // hot decode shape (cache-resident — the
                                                   // only regime where gemv knobs can show)
    var vs <- q8q8_tile_gen_variants()
    var mrs <- variant_mrs()
    var wbs <- variant_wbias()
    var kgs <- variant_kgroups()
    var tss <- variant_toksteps()
    var gtab <- gemv_variants_by_suffix()
    var ctab <- cfg_variants_by_suffix()
    // TMUL executes only after the per-process XTILEDATA grant — the witness IS the enable
    // (slice I), and this harness invokes stamped tiles directly (production consults the
    // witness at backend selection), so consume every row's witness up front like test_mode
    // does. One grant covers the whole process. Found the hard way: first-ever amx tune run
    // SIGILL'd on the first tdpbssd while the test grid (which checks witnesses) was green.
    var wtab <- witness_variants_by_suffix()
    for (v in vs) {
        verify(wtab |> key_exists(v._0))
        invoke(wtab[v._0])
    }
    // Declined perms all run the SAME reference body — benching each at best-of-ROUNDS is
    // duplicate measurement, and N identical rows hand the winner-picker N noise-lottery
    // tickets for one body. Keep a single declined representative (the explicit "reference"
    // row when the grid carries one), erase the rest. On a foreign-ISA box (e.g. arm64
    // skipping the x64 legs) this cuts the grid from ~34 rows to the live handful.
    var rep = -1
    for (i in range(length(vs))) {
        if (vs[i]._0 == "reference") {
            rep = i
        }
    }
    if (rep < 0) {
        for (i in range(length(vs))) {
            if (!invoke(wtab[vs[i]._0])) {
                rep = i
                break
            }
        }
    }
    let repname = rep >= 0 ? vs[rep]._0 : ""
    var skipped = 0
    let nvs0 = length(vs)
    for (k in range(nvs0)) {
        let i = nvs0 - 1 - k
        if (i != rep && !invoke(wtab[vs[i]._0])) {
            vs |> erase(i)
            skipped++
        }
    }
    if (skipped > 0) {
        tune_detail("skipped {skipped} declined perm(s) - the shared reference body benches once as '{repname}'\n")
    }
    delete wtab
    let nv = length(vs)
    // per-variant repacked buffers, built once outside the timing loop
    var bufs : array<VBuf>
    var gbufs : array<VBuf>
    var hbufs : array<VBuf>
    bufs |> resize(nv)
    gbufs |> resize(nv)
    hbufs |> resize(nv)
    var vmr : array<int64>
    var vts : array<int64>
    var vgs : array<int64>
    vmr |> resize(nv)
    vts |> resize(nv)
    vgs |> resize(nv)
    for (vi in range(nv)) {
        vmr[vi] = int64(mrs?[vs[vi]._0] ?? 4)
        vts[vi] = int64(tss?[vs[vi]._0] ?? 4)
        vgs[vi] = max(vts[vi] / 16l, 1l)
        let wb = int64(wbs?[vs[vi]._0] ?? 0)
        let kgv = int64(kgs?[vs[vi]._0] ?? 4)
        verify(gtab |> key_exists(vs[vi]._0))
        verify(ctab |> key_exists(vs[vi]._0))
        bufs[vi].q := fx.q
        bufs[vi].s := fx.s
        gbufs[vi].q := gfx.q
        gbufs[vi].s := gfx.s
        hbufs[vi].q := hfx.q
        hbufs[vi].s := hfx.s
        unsafe {
            repack_q8q8_grp(addr(bufs[vi].q[0]), addr(bufs[vi].s[0]), fx.n, fx.d, vmr[vi], wb, kgv)
            repack_q8q8_grp(addr(gbufs[vi].q[0]), addr(gbufs[vi].s[0]), gfx.n, gfx.d, vmr[vi], wb, kgv)
            repack_q8q8_grp(addr(hbufs[vi].q[0]), addr(hbufs[vi].s[0]), hfx.n, hfx.d, vmr[vi], wb, kgv)
        }
    }
    var best : array<double>
    var gbest : array<double>
    var hbest : array<double>
    best |> resize(nv)
    gbest |> resize(nv)
    hbest |> resize(nv)
    for (i in range(nv)) {
        best[i] = 1.0e30lf
        gbest[i] = 1.0e30lf
        hbest[i] = 1.0e30lf
    }
    var y : array<float>
    y |> resize(fx.ntok * fx.d)
    var yg : array<float>
    yg |> resize(gfx.d)
    var yh : array<float>
    yh |> resize(hfx.d)
    // correctness first — a tune run must never crown a broken variant
    for (vi in range(nv)) {
        run_tile(vs[vi]._1, ctab[vs[vi]._0], fx, bufs[vi].q, bufs[vi].s, vmr[vi], vts[vi], vgs[vi], y)
        var maxdiff = 0.0
        if (!close_to_ref(fx, y, maxdiff)) {
            print("{vs[vi]._0}: tile MISMATCH (maxdiff {maxdiff}) - aborting tune\n")
            return false
        }
        run_gemv(gtab[vs[vi]._0], gfx, gbufs[vi].q, gbufs[vi].s, vmr[vi], yg)
        var gmaxdiff = 0.0
        if (!close_to_ref(gfx, yg, gmaxdiff)) {
            print("{vs[vi]._0}: gemv MISMATCH (maxdiff {gmaxdiff}) - aborting tune\n")
            return false
        }
    }
    // the hot shape runs HOT_REPS back-to-back per timing sample: 1MB of weights stays
    // cache-resident, so the sample measures compute, not the first-touch stream
    let HOT_REPS = 16
    for (r in range(ROUNDS)) {   // interleaved best-of: variant order inside each round
        tune_progress_kernel_step(r + 1, "best-of", length(vs))
        for (vi in range(nv)) {
            let t0 = ref_time_ticks()
            run_tile(vs[vi]._1, ctab[vs[vi]._0], fx, bufs[vi].q, bufs[vi].s, vmr[vi], vts[vi], vgs[vi], y)
            let us = double(get_time_usec(t0))
            if (us < best[vi]) {
                best[vi] = us
            }
            let g0 = ref_time_ticks()
            run_gemv(gtab[vs[vi]._0], gfx, gbufs[vi].q, gbufs[vi].s, vmr[vi], yg)
            let gus = double(get_time_usec(g0))
            if (gus < gbest[vi]) {
                gbest[vi] = gus
            }
            let h0 = ref_time_ticks()
            for (_h in range(HOT_REPS)) {
                run_gemv(gtab[vs[vi]._0], hfx, hbufs[vi].q, hbufs[vi].s, vmr[vi], yh)
            }
            let hus = double(get_time_usec(h0)) / double(HOT_REPS)
            if (hus < hbest[vi]) {
                hbest[vi] = hus
            }
        }
    }
    let macs = double(fx.n * fx.d * fx.ntok)
    let gmacs = double(gfx.n * gfx.d)
    let hmacs = double(hfx.n * hfx.d)
    var winner = 0
    var q8rows : table<string; JsonValue?>
    for (vi in range(nv)) {
        if (best[vi] < best[winner]) {
            winner = vi
        }
        q8rows[vs[vi]._0] = JV((tile_us = best[vi], gemv_us = gbest[vi], hot_us = hbest[vi]))
        tune_detail("{vs[vi]._0} (mr={vmr[vi]}): tile {macs / best[vi] / 1000.0lf} GMAC/s, gemv stream {gmacs / gbest[vi] / 1000.0lf} / hot {hmacs / hbest[vi] / 1000.0lf} GMAC/s (best of {ROUNDS})\n")
    }
    g_rows["q8q8_tile_gen"] = JV(q8rows)
    // the merged winner: the batch bench picks the tile knobs (and with them the layout);
    // the HOT decode bench picks gkstep among the rows sharing those tile knobs (the
    // streamed shape is DRAM-bound and cannot tell kernels apart) — one entry, one
    // coherent layout+tile+gemv decision
    let wkey = tile_key(vs[winner]._0)
    var merged = winner
    for (vi in range(nv)) {
        if (tile_key(vs[vi]._0) == wkey && hbest[vi] < hbest[merged]) {
            merged = vi
        }
    }
    if (merged != winner) {
        tune_detail("tile winner {vs[winner]._0} + hot decode winner within '{wkey}' -> {vs[merged]._0}\n")
    }
    // the kq families tune separately — one manifest entry per format, tile-best wins (the
    // gemv is nrsplit-independent, so same-mr rows share its plane and kernel shape). No
    // child-process confirm gate here: the kq stamp only moves the kq planes' interleave,
    // and the e2e exposure (a tile-optimal mr shaving decode kernel rate) is bounded by the
    // decode path staying DRAM-bound — validated end-to-end when the entries first landed.
    tune_detail("TUNE_GEN_TIME q8q8_family {get_time_usec(tune_total_t0) / 1000} ms\n")
    tune_progress_kernel_end("q8q8_tile_gen", vs[merged]._0, "beats")
    if (!noise_gate("mid1")) {
        print("aborting mid-run - no sidecar written\n")
        return false
    }
    for (fmt in fixed_array(4l, 5l, 6l, 40l)) {
        let kq_t0 = ref_time_ticks()
        tune_progress_kernel_begin("k{fmt}q8_tile_gen", ROUNDS)
        var krows : table<string; JsonValue?>
        let w = kq_tune_family(fmt, krows)
        tune_detail("TUNE_GEN_TIME k{fmt}_family {get_time_usec(kq_t0) / 1000} ms\n")
        tune_progress_kernel_end("k{fmt}q8_tile_gen", w, empty(w) ? "rejected" : "beats")
        let entry = fmt == 40l ? "q40q8_tile_gen" : "k{fmt}q8_tile_gen"
        // gate failure (the broken perm is named above): queue the always-correct reference
        // body so the scope still COMPLETES — a missing key re-tunes every start
        pending[entry] = empty(w) ? "reference" : w
        print("{entry} winner: {empty(w) ? "reference (gate-failure fallback)" : w}\n")
        g_rows[entry] = JV(krows)
    }
    if (!noise_gate("mid2")) {
        print("aborting mid-run - no sidecar written\n")
        return false
    }
    let q51_t0 = ref_time_ticks()
    // the manifest key must match the [tune] fn (the TILE) or the stamp misses its own crown;
    // the race still benches the gemv shape
    tune_progress_kernel_begin("q51q8_tile_gen", ROUNDS)
    var qrows : table<string; JsonValue?>
    let qw = q51_tune_family(qrows)
    tune_detail("TUNE_GEN_TIME q51_family {get_time_usec(q51_t0) / 1000} ms\n")
    tune_progress_kernel_end("q51q8_tile_gen", qw, empty(qw) ? "rejected" : "beats")
    // same gate-failure fallback as the kq loop: complete the scope on the reference body
    pending["q51q8_tile_gen"] = empty(qw) ? "reference" : qw
    g_rows["q51q8_tile_gen"] = JV(qrows)
    print("q51q8_tile_gen winner: {empty(qw) ? "reference (gate-failure fallback)" : qw}\n")
    let confirm_t0 = ref_time_ticks()
    tune_progress_kernel_begin("confirm_e2e_prefill", 0)
    let cw = confirm_winner(vs[merged]._0)
    var ok = !empty(cw)
    if (ok) {
        pending["q8q8_tile_gen"] = cw
        print("q8q8_tile_gen winner: {cw}\n")
    }
    tune_detail("TUNE_GEN_TIME confirm {get_time_usec(confirm_t0) / 1000} ms\n")
    tune_progress_kernel_end("confirm_e2e_prefill", "", ok ? "holds" : "rejected")
    tune_progress_finish()
    // the only write, behind the last gate: a noisy end-probe means NO entry is written — the
    // winners above were raced in conditions the probe just proved untrustworthy
    if (ok && !noise_gate("end")) {
        print("aborting at the end-of-run gate - no sidecar written\n")
        ok = false
    }
    if (ok) {
        noise_provenance_stamp()
        var nort : JsonValue?
        ok = tune_sidecar_merge(pending, nort)
        var race : table<string; JsonValue?>
        for (k, v in keys(pending), values(pending)) {
            if (g_rows |> key_exists(k)) {
                var obj <- { "winner" => JV(v), "rows" => g_rows[k] }
                race[k] = JV(obj)
            }
        }
        g_rows |> clear()   // pointees transferred into race (clear does not finalize)
        ok = tune_sidecar_section_merge("race", race) && ok
        print(ok ? "wrote {length(pending)} generator entries -> {tune_manifest_path()}\n"
            : "FAILED to write {tune_manifest_path()} - if the app dir is read-only, set DAS_TUNE_MANIFEST to a writable path\n")
    }
    delete pending
    tune_detail("TUNE_GEN_TIME total {get_time_usec(tune_total_t0) / 1000} ms\n")
    delete y
    delete yg
    delete yh
    delete bufs
    delete gbufs
    delete hbufs
    delete vmr
    delete vts
    delete vgs
    delete best
    delete gbest
    delete hbest
    delete gtab
    delete ctab
    delete tss
    delete kgs
    delete wbs
    delete mrs
    delete vs
    delete fx
    delete gfx
    delete hfx
    return ok
}

// fallback: the tuner rail drives this via DAS_TUNE_MODE=tune explicitly; the auto policy
// must not fire when this runs as the compile-only fallback-discovery child (recursion)
[export, tune_policy(missing = "fallback")]
def main : int {
    if (!jit_enabled()) {
        print("run with -jit - the generator only fires under the LLVM JIT\n")
        return 1
    }
    if (tuner_paranoid_requested()) {
        ROUNDS = ROUNDS_FULL * 3
        noise_set_paranoid(true)
    }
    // pre-i8mm mint rail (the slice J no-linker-on-target recipe): the JIT has already
    // linked + installed the full grid DLL by the time main runs — this gate skips
    // EXECUTION only (forced smmla variants would SIGILL on a dotprod-only host). Ship
    // .jitted_scripts to the i8mm box and run there with the same env minus this one —
    // the cache key matches, dlopen, no linker needed on the target.
    if (g_env_harness.gen_tune_compile_only) {
        var vs <- q8q8_tile_gen_variants()
        print("compile-only: DLL linked, {length(vs)} tile variants, execution skipped\n")
        delete vs
        return 0
    }
    // select the gen backend where it exists (arm64) — test/tune drive the variant
    // registries directly, so an x64 box runs fine with no gen backend registered
    // (slice F: the x64 grid rows light per cpuid; x64-gen registration is queued)
    for (nm in kernel_backend_names()) {
        if (nm == "arm64-gen") {
            select_kernel_backend("arm64-gen")
        }
    }
    let mode = tune_mode()
    if (mode == "test") {
        let tok = test_mode()
        print(tok ? "GEN TUNE TEST OK\n" : "GEN TUNE TEST FAIL\n")
        return tok ? 0 : 1
    }
    if (mode == "tune") {
        let rok = tune_mode_run()
        print(rok ? "GEN TUNE RUN OK\n" : "GEN TUNE RUN FAIL\n")
        return rok ? 0 : 1
    }
    print("set DAS_TUNE_MODE=test or =tune (compile-time env: it stamps the variant grid)\n")
    return 1
}
