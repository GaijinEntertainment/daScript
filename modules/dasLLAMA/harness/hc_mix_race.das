options gen2
options persistent_heap
options _dasllama_internal = true

// HC MIX RACE - the hyper-connection mixer site over eight wide rows on Flash-Next's shapes (4 streams of 2560, low rank 320):
// the grouped norm, the q8 down (K 10240, 320 rows), the f32 inject rows, silu(lo / hc), the q8 up (K 320, 10240 rows) and the
// sigmoid-gated stream mean, NL site copies of the planes so a site's bytes stream past the SLC. The raw MSL (hc_mix.metal)
// holds the engine's one-row pair and today's eight-row chain as generated, and the raced eight-row forms; every form is
// checked against the chain (the max difference over the max magnitude) and timed as a chain of CH sites. A timed row drops
// 40 warmup chains and reports the mean and cv of the ROUNDS chains after them; a failed chain refuses the row.
// DASLLAMA_HC_MIX_ARMS (comma list of arm names) narrows a run.
//
//   bin/daslang -jit modules/dasLLAMA/harness/hc_mix_race.das

require daslib/fio
require daslib/math_bits
require math
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_env
require dasllama/dasllama_math
require _race_fixture   // race_lcg, arm_wanted
require _metal_race_fixture   // the chain clock, the bandwidth reference, the read-back compare

let DIM = 2560
let HC = 4
let HCD = DIM * HC
let LR = 320
let NR = 8
let NL = 12
let CH = 12
let ROUNDS = 5
let EPS = 1.0e-6
let CLOCK = RaceClock(sites = CH, rounds = ROUNDS, burn = 40)
var g_half = 0   //! the halves a form encodes: 0 both, 1 the down half alone, 2 the up half alone (the per-half timings)

struct HcMix8Args {
    dim : uint
    hc : uint
    lr : uint
    eps : float
}

struct Arm {
    name : string
    rs : int = 4    //! the down's rows a simdgroup
    nsg : int = 4   //! the down's simdgroups a threadgroup
    cols : int = 1  //! the up's columns a simdgroup
    fin : bool = false   //! the partials' finish as its own dispatch
    mu : bool = false    //! the matrix-unit form: prep, the stream-split rows8 down, the finish, the rows8 up with the mix
    wfin : bool = false  //! the matrix-unit form's finish a block a threadgroup
    ksp : int = 0        //! the matrix-unit up's K split over simdgroups a stream tile (0: none, `r8up_mix`)
    nf : bool = false    //! the matrix-unit form with the norm first: the prep stores the normed table (`hcprep8n`, `lofin8n`)
}

struct Bench {
    down : MetalBuffer?   //! NL q8 planes [LR][HCD], `dstride` bytes apart
    up : MetalBuffer?     //! NL q8 planes [HCD][LR]
    dstride : uint64
    ustride : uint64
    wi : MetalBuffer?     //! NL f32 inject planes [HC][HCD]
    wn : MetalBuffer?     //! NL gamma rows [HCD]
    res : MetalBuffer?    //! the eight wide rows [NR][HCD]
    xn, xt, xs, lo, gate, xb, inj : MetalBuffer?   //! the chain's scratch and outputs
    part, ipart, ssq, y, inj8, lox, invx, xt2, xs2 : MetalBuffer?   //! the raced forms'
    u : table<string; MetalBuffer?>   //! the uniforms by name
    ka : MetalBuffer?
}

//! NL q8 planes of `rows` x `k`, each `scale` (a power of two) times int8 codes off a hash; the plane stride rounded to 4 KiB
def private make_q8(dev : MetalDevice?; rows, k : int; scale : float; seed : uint; var stride : uint64&) : MetalBuffer? {
    let nblk = rows * k / 32
    stride = (uint64(nblk * 34) + 4095ul) & ~4095ul
    var blob : array<uint8>
    blob |> reserve_resize(int64(stride) * int64(NL))
    var h = seed
    for (l in range(NL)) {
        let base = int64(l) * int64(stride)
        for (b in range(nblk)) {
            let o = base + int64(b) * 34l
            let sc = float16(scale * float(1 << int(race_lcg(h) % 3u)))
            *unsafe(addr<float16?>(blob[o])) = sc
            for (j in range(32)) {
                blob[o + 2l + int64(j)] = uint8(int(race_lcg(h) % 255u) & 255)
            }
        }
    }
    var buf = metal_buffer_from(dev, uint64(long_length(blob)), unsafe(addr<void?>(blob[0])))
    delete blob
    return buf
}

def private f32_buf(dev : MetalDevice?; n : int; seed : uint; lo, span : float) : MetalBuffer? {
    var h = seed
    var inscope a <- [for (_i in range(n)); lo + span * float(race_lcg(h) % 10007u) / 10007.0]
    return metal_buffer_from(dev, uint64(n * 4), unsafe(addr<void?>(a[0])))
}

def private out_buf(dev : MetalDevice?; n : int) : MetalBuffer? => metal_buffer_from(dev, uint64(n * 4), null)

def private make_bench(dev : MetalDevice?) : Bench {
    var dstride, ustride : uint64
    var dn = make_q8(dev, LR, HCD, 1.0 / 8192.0, 11u, dstride)   //! the scales put lo and the gates near the unit, where the sigmoid is not saturated
    var up = make_q8(dev, HCD, LR, 1.0 / 1024.0, 23u, ustride)
    var b = Bench(down = dn, up = up, dstride = dstride, ustride = ustride,
                  wi = f32_buf(dev, NL * HC * HCD, 5u, -0.02, 0.04), wn = f32_buf(dev, NL * HCD, 7u, 0.6, 0.8), res = f32_buf(dev, NR * HCD, 9u, -2.0, 4.0),
                  xn = out_buf(dev, NR * HCD), xt = out_buf(dev, NR * HCD / 2), xs = out_buf(dev, NR * HCD / 32), lo = out_buf(dev, NR * LR),
                  gate = out_buf(dev, NR * HCD), xb = out_buf(dev, NR * DIM), inj = out_buf(dev, NR * HC), part = out_buf(dev, HC * NR * LR),
                  ipart = out_buf(dev, HC * NR * HC), ssq = out_buf(dev, HC * NR), y = out_buf(dev, NR * DIM), inj8 = out_buf(dev, NR * HC),
                  lox = out_buf(dev, NR * LR), invx = out_buf(dev, NR * HC), xt2 = out_buf(dev, NR * LR / 2), xs2 = out_buf(dev, NR * LR / 32))
    for (kv in [("hcd", HCD), ("lr", LR), ("hc", HC), ("lrtot", NR * LR), ("kseg", DIM / 32)]) {
        b.u |> insert(kv._0, race_uniform_u32(dev, uint(kv._1)))
    }
    var ka = HcMix8Args(dim = uint(DIM), hc = uint(HC), lr = uint(LR), eps = EPS)
    b.ka = metal_buffer_from(dev, 16ul, unsafe(addr<void?>(ka)))
    return <- b
}

struct Psos {
    hcn, sglo, hcnorm, router, prep, r8q8, hclo, hcmix : MetalComputePipeline?
    pnorm, prouter, pmix : MetalBuffer?   //! the chain's argument blocks
    one_q8hc : MetalBuffer?
}

def private set_plane(enc : MetalComputeEncoder?; buf : MetalBuffer?; off : uint64) {
    metal_set_buffer(enc, buf, off, 0)
    metal_set_buffer(enc, buf, off, 1)
}

def private grid1(enc : MetalComputeEncoder?; n, tg : int) {
    metal_dispatch_threadgroups(enc, uint3(uint(n), 1u, 1u), uint3(uint(tg), 1u, 1u))
}

//! the engine's one-row pair over row 0 of site `l`
def private enc_one(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    if (g_half != 2) {
        enc_one_dn(enc, p, b, l)
    }
    if (g_half != 1) {
        enc_one_up(enc, p, b, l)
    }
}

def private enc_one_dn(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    metal_set_pipeline(enc, p.hcn)
    set_plane(enc, b.down, uint64(l) * b.dstride)
    metal_set_buffer(enc, b.res, 0ul, 2)
    metal_set_buffer(enc, b.lo, 0ul, 3)
    metal_set_buffer(enc, b.u?["hcd"] ?? null, 0ul, 4)
    metal_set_buffer(enc, b.u?["lr"] ?? null, 0ul, 5)
    metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 6)
    metal_set_buffer(enc, b.xn, 0ul, 7)
    metal_set_buffer(enc, b.wi, uint64(l * HC * HCD * 4), 8)
    metal_set_buffer(enc, b.inj, 0ul, 9)
    metal_set_buffer(enc, p.one_q8hc, 0ul, 10)
    metal_set_threadgroup_memory_length(enc, 1024ul, 0)
    grid1(enc, LR / 2 + (HC + 1) / 2, 128)
    metal_memory_barrier(enc)
}

def private enc_one_up(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    metal_set_pipeline(enc, p.sglo)
    set_plane(enc, b.up, uint64(l) * b.ustride)
    metal_set_buffer(enc, b.lo, 0ul, 2)
    metal_set_buffer(enc, b.xn, 0ul, 3)
    metal_set_buffer(enc, b.u?["lr"] ?? null, 0ul, 4)
    metal_set_buffer(enc, b.u?["hcd"] ?? null, 0ul, 5)
    metal_set_buffer(enc, b.xb, 0ul, 6)
    metal_set_buffer(enc, b.u?["hc"] ?? null, 0ul, 7)
    metal_set_threadgroup_memory_length(enc, 64ul, 0)
    grid1(enc, HCD / 8, 256)
}

//! a rows8 q8 site: the prep of `bx` (k wide) into the table, then the plane's rows into `by`
def private enc_r8(enc : MetalComputeEncoder?; p : Psos; b : Bench; plane : MetalBuffer?; off : uint64; bx, by : MetalBuffer?; k, n : int) {
    metal_set_pipeline(enc, p.prep)
    metal_set_buffer(enc, bx, 0ul, 0)
    metal_set_buffer(enc, b.xt, 0ul, 1)
    metal_set_buffer(enc, b.xs, 0ul, 2)
    metal_set_buffer(enc, k == HCD ? (b.u?["hcd"] ?? null) : (b.u?["lr"] ?? null), 0ul, 3)
    grid1(enc, (k / 32 * 8 + 255) / 256, 256)
    metal_memory_barrier(enc)
    metal_set_pipeline(enc, p.r8q8)
    set_plane(enc, plane, off)
    metal_set_buffer(enc, b.xt, 0ul, 2)
    metal_set_buffer(enc, by, 0ul, 3)
    metal_set_buffer(enc, b.xs, 0ul, 4)
    metal_set_buffer(enc, k == HCD ? (b.u?["hcd"] ?? null) : (b.u?["lr"] ?? null), 0ul, 5)
    metal_set_buffer(enc, n == HCD ? (b.u?["hcd"] ?? null) : (b.u?["lr"] ?? null), 0ul, 6)
    metal_set_threadgroup_memory_length(enc, 8192ul, 0)
    grid1(enc, n / 16, 256)
}

//! today's eight-row chain over site `l`
def private enc_chain(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    if (g_half != 2) {
        enc_chain_dn(enc, p, b, l)
    }
    if (g_half != 1) {
        enc_chain_up(enc, p, b, l)
    }
}

def private enc_chain_dn(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    metal_set_pipeline(enc, p.hcnorm)
    metal_set_buffer(enc, b.res, 0ul, 0)
    metal_set_buffer(enc, b.xn, 0ul, 1)
    metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 2)
    metal_set_buffer(enc, p.pnorm, 0ul, 3)
    metal_set_threadgroup_memory_length(enc, 256ul, 0)
    grid1(enc, NR * HC, 1024)
    metal_memory_barrier(enc)
    metal_set_pipeline(enc, p.router)
    metal_set_buffer(enc, b.wi, uint64(l * HC * HCD * 4), 0)
    metal_set_buffer(enc, b.xn, 0ul, 1)
    metal_set_buffer(enc, b.inj, 0ul, 2)
    metal_set_buffer(enc, p.prouter, 0ul, 5)
    metal_set_threadgroup_memory_length(enc, 512ul, 0)
    metal_dispatch_threadgroups(enc, uint3(uint(HC), uint(NR), 1u), uint3(256u, 1u, 1u))
    enc_r8(enc, p, b, b.down, uint64(l) * b.dstride, b.xn, b.lo, HCD, LR)
    metal_memory_barrier(enc)
}

def private enc_chain_up(enc : MetalComputeEncoder?; p : Psos; b : Bench; l : int) {
    metal_set_pipeline(enc, p.hclo)
    metal_set_buffer(enc, b.lo, 0ul, 0)
    metal_set_buffer(enc, b.u?["lrtot"] ?? null, 0ul, 1)
    metal_set_buffer(enc, b.u?["hc"] ?? null, 0ul, 2)
    grid1(enc, (NR * LR + 63) / 64, 64)
    metal_memory_barrier(enc)
    enc_r8(enc, p, b, b.up, uint64(l) * b.ustride, b.lo, b.gate, LR, HCD)
    metal_memory_barrier(enc)
    metal_set_pipeline(enc, p.hcmix)
    metal_set_buffer(enc, b.xn, 0ul, 0)
    metal_set_buffer(enc, b.gate, 0ul, 1)
    metal_set_buffer(enc, b.xb, 0ul, 2)
    metal_set_buffer(enc, p.pmix, 0ul, 3)
    grid1(enc, NR * DIM / 64, 64)
}

struct ArmPsos {
    dn, up, fin : MetalComputePipeline?
    prep, mdn, mfin, mup : MetalComputePipeline?   //! the matrix-unit form's four
}

//! the matrix-unit form over site `l`: the prep and the stream-split down (the down half), the finish and the up with the mix
def private enc_mu(enc : MetalComputeEncoder?; ap : ArmPsos; a : Arm; b : Bench; l : int) {
    if (g_half != 2) {
        metal_set_pipeline(enc, ap.prep)
        metal_set_buffer(enc, b.res, 0ul, 0)
        metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 1)
        metal_set_buffer(enc, b.wi, uint64(l * HC * HCD * 4), 2)
        metal_set_buffer(enc, b.xt, 0ul, 3)
        metal_set_buffer(enc, b.xs, 0ul, 4)
        metal_set_buffer(enc, a.nf ? b.invx : b.ssq, 0ul, 5)
        metal_set_buffer(enc, b.ipart, 0ul, 6)
        metal_set_buffer(enc, b.ka, 0ul, 7)
        metal_dispatch_threadgroups(enc, uint3(uint(HC), uint(NR), 1u), uint3(a.nf ? 1024u : 256u, 1u, 1u))
        metal_memory_barrier(enc)
        metal_set_pipeline(enc, ap.mdn)
        set_plane(enc, b.down, uint64(l) * b.dstride)
        metal_set_buffer(enc, b.xt, 0ul, 2)
        metal_set_buffer(enc, b.part, 0ul, 3)
        metal_set_buffer(enc, b.xs, 0ul, 4)
        metal_set_buffer(enc, b.u?["hcd"] ?? null, 0ul, 5)
        metal_set_buffer(enc, b.u?["lr"] ?? null, 0ul, 6)
        metal_set_buffer(enc, b.u?["kseg"] ?? null, 0ul, 7)
        metal_set_threadgroup_memory_length(enc, 8192ul, 0)
        metal_dispatch_threadgroups(enc, uint3(uint(LR / 16), uint(HC), 1u), uint3(256u, 1u, 1u))
        metal_memory_barrier(enc)
    }
    if (g_half != 1) {
        metal_set_pipeline(enc, ap.mfin)
        metal_set_buffer(enc, b.part, 0ul, 2)
        metal_set_buffer(enc, b.ssq, 0ul, 3)
        metal_set_buffer(enc, b.ipart, 0ul, 4)
        metal_set_buffer(enc, b.inj8, 0ul, 8)
        metal_set_buffer(enc, b.ka, 0ul, 9)
        metal_set_buffer(enc, b.xt2, 0ul, 10)
        metal_set_buffer(enc, b.xs2, 0ul, 11)
        if (!a.nf) {
            metal_set_buffer(enc, b.invx, 0ul, 12)
        }
        grid1(enc, a.wfin ? LR / 32 : 1, 256)
        metal_memory_barrier(enc)
        metal_set_pipeline(enc, ap.mup)
        set_plane(enc, b.up, uint64(l) * b.ustride)
        metal_set_buffer(enc, b.xt2, 0ul, 2)
        metal_set_buffer(enc, b.y, 0ul, 3)
        metal_set_buffer(enc, b.xs2, 0ul, 4)
        metal_set_buffer(enc, b.res, 0ul, 5)
        metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 6)
        metal_set_buffer(enc, b.ka, 0ul, 9)
        metal_set_buffer(enc, b.invx, 0ul, 12)
        grid1(enc, DIM / 16, 128 * max(a.ksp, 1))
    }
}

//! a raced form over site `l`: the stream-split down, the finish where the arm dispatches it, the up with the mix
def private enc_arm(enc : MetalComputeEncoder?; ap : ArmPsos; a : Arm; b : Bench; l : int) {
    if (a.mu) {
        enc_mu(enc, ap, a, b, l)
        return
    }
    if (g_half != 2) {
        enc_arm_dn(enc, ap, a, b, l)
    }
    if (g_half != 1) {
        enc_arm_up(enc, ap, a, b, l)
    }
}

def private enc_arm_dn(enc : MetalComputeEncoder?; ap : ArmPsos; a : Arm; b : Bench; l : int) {
    metal_set_pipeline(enc, ap.dn)
    set_plane(enc, b.down, uint64(l) * b.dstride)
    metal_set_buffer(enc, b.res, 0ul, 2)
    metal_set_buffer(enc, b.part, 0ul, 3)
    metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 4)
    metal_set_buffer(enc, b.wi, uint64(l * HC * HCD * 4), 5)
    metal_set_buffer(enc, b.ipart, 0ul, 6)
    metal_set_buffer(enc, b.ssq, 0ul, 7)
    metal_set_buffer(enc, b.ka, 0ul, 8)
    metal_dispatch_threadgroups(enc, uint3(uint(LR / (a.rs * a.nsg) + 1), uint(HC), 1u), uint3(uint(32 * a.nsg), 1u, 1u))
    metal_memory_barrier(enc)
}

def private enc_arm_up(enc : MetalComputeEncoder?; ap : ArmPsos; a : Arm; b : Bench; l : int) {
    if (a.fin) {
        metal_set_pipeline(enc, ap.fin)
        metal_set_buffer(enc, b.part, 0ul, 2)
        metal_set_buffer(enc, b.ssq, 0ul, 3)
        metal_set_buffer(enc, b.ipart, 0ul, 4)
        metal_set_buffer(enc, b.inj8, 0ul, 8)
        metal_set_buffer(enc, b.ka, 0ul, 9)
        metal_set_buffer(enc, b.lox, 0ul, 10)
        metal_set_buffer(enc, b.invx, 0ul, 11)
        grid1(enc, 1, 1024)
        metal_memory_barrier(enc)
    }
    metal_set_pipeline(enc, ap.up)
    set_plane(enc, b.up, uint64(l) * b.ustride)
    metal_set_buffer(enc, b.part, 0ul, 2)
    metal_set_buffer(enc, b.ssq, 0ul, 3)
    metal_set_buffer(enc, b.ipart, 0ul, 4)
    metal_set_buffer(enc, b.res, 0ul, 5)
    metal_set_buffer(enc, b.wn, uint64(l * HCD * 4), 6)
    metal_set_buffer(enc, b.y, 0ul, 7)
    metal_set_buffer(enc, b.inj8, 0ul, 8)
    metal_set_buffer(enc, b.ka, 0ul, 9)
    metal_set_buffer(enc, b.lox, 0ul, 10)
    metal_set_buffer(enc, b.invx, 0ul, 11)
    grid1(enc, DIM / (8 * a.cols), 256)
}

let HALF_TAG = fixed_array<string>("", " dn", " up")

//! the read-bandwidth reference over one site's down plane
def private print_bw(dev : MetalDevice?; queue : MetalCommandQueue?; b : Bench; src : string) {
    let tbw = bw_read_us(dev, queue, CLOCK, src, b.down, b.dstride, NL, b.y, 1024u)
    if (!tbw.ok) {
        refused_row("bw_read")
        return
    }
    print("bw_read\t{tbw.us:7.2f} us a {int64(b.dstride) / 1024l} KB plane  {double(b.dstride) / (tbw.us * 1000.0lf):6.1f} GB/s; a site's q8 bytes {int64(b.dstride + b.ustride) / 1024l} KB\n")
}

def private arms() : array<Arm> {
    return <- [Arm(name = "mu", mu = true), Arm(name = "mu_w", mu = true, wfin = true), Arm(name = "mu_k2", mu = true, ksp = 2),
               Arm(name = "mu_w_k2", mu = true, wfin = true, ksp = 2), Arm(name = "mu_n_k2", mu = true, wfin = true, ksp = 2, nf = true), Arm(name = "mu_w_k5", mu = true, wfin = true, ksp = 5), Arm(name = "r4n4c1"), Arm(name = "r4n4c2", cols = 2), Arm(name = "r4n4c4", cols = 4),
               Arm(name = "r2n4c2", rs = 2, cols = 2), Arm(name = "r2n8c2", rs = 2, nsg = 8, cols = 2),
               Arm(name = "r4n2c2", nsg = 2, cols = 2), Arm(name = "r4n8c2", nsg = 8, cols = 2),
               Arm(name = "f_r4n4c1", fin = true), Arm(name = "f_r4n4c2", cols = 2, fin = true), Arm(name = "f_r4n4c4", cols = 4, fin = true)]
}

def private pso_of(dev : MetalDevice?; src, flags, entry : string) : MetalComputePipeline? {
    var err = ""
    var p = pso_from_source(dev, flags + src, entry, true, err)
    if (p == null) {
        print("{entry} {flags}: pso - {err}\n")
    }
    return p
}

//! the bases' pipelines and argument blocks
def private make_psos(dev : MetalDevice?; src : string) : Psos {
    var nargs = fixed_array<uint>(uint(DIM), uint(HC), 0u, 0u, float_bits_to_uint(EPS))
    var rargs : uint[15]
    rargs[0] = uint(HC)
    rargs[1] = uint(HCD)
    rargs[7] = uint(NR)
    var margs = fixed_array<uint>(uint(DIM), uint(HC), uint(NR))
    var oargs = fixed_array<uint>(uint(DIM), uint(HC), float_bits_to_uint(EPS))
    return Psos(hcn = pso_of(dev, src, "", "hcn_base"), sglo = pso_of(dev, src, "", "sglo_base"), hcnorm = pso_of(dev, src, "", "hcnorm_base"),
                router = pso_of(dev, src, "", "router_base"), prep = pso_of(dev, src, "", "prep_base"), r8q8 = pso_of(dev, src, "", "r8q8_base"),
                hclo = pso_of(dev, src, "", "hclo_base"), hcmix = pso_of(dev, src, "", "hcmix_base"),
                pnorm = metal_buffer_from(dev, 20ul, unsafe(addr<void?>(nargs[0]))), prouter = metal_buffer_from(dev, 60ul, unsafe(addr<void?>(rargs[0]))),
                pmix = metal_buffer_from(dev, 12ul, unsafe(addr<void?>(margs[0]))), one_q8hc = metal_buffer_from(dev, 12ul, unsafe(addr<void?>(oargs[0]))))
}

//! the references and the bases' times: the chain's eight rows, the one-row pair's row 0, each base whole and by half
struct Refs {
    xb : array<float>
    inj : array<float>
    one_xb : array<float>
    tone : double[3]
    tch : double[3]
}

def private race_bases(dev : MetalDevice?; queue : MetalCommandQueue?; p : Psos; b : Bench; src : string) : Refs {
    var err = ""
    var gms = 0.0lf
    var r : Refs
    g_half = 0
    with_compute_encoder_timed(queue, err, gms) $(enc) {
        enc_chain(enc, p, b, 0)
    }
    r.xb <- buf_download(b.xb, NR * DIM, type<float>)
    r.inj <- buf_download(b.inj, NR * HC, type<float>)
    with_compute_encoder_timed(queue, err, gms) $(enc) {
        enc_one(enc, p, b, 0)
    }
    var inscope row0 <- [for (i in range(DIM)); r.xb[i]]
    var inscope inj0 <- [for (i in range(HC)); r.inj[i]]
    r.one_xb <- buf_download(b.xb, DIM, type<float>)
    print("one-row pair vs the chain's row 0: xb {rel_of(row0, b.xb)}, inj {rel_of(inj0, b.inj)}\n")
    print_bw(dev, queue, b, src)
    for (h in range(3)) {
        g_half = h
        let t = site_us(queue, CLOCK) $(enc; i) {
            enc_one(enc, p, b, i % NL)
        }
        if (!t.ok) {
            refused_row("one_row{HALF_TAG[h]}")
            continue
        }
        r.tone[h] = t.us
        print("one_row{HALF_TAG[h]}\t{t.us:7.2f} us a site  cv {t.cv:4.1f}%{t.cv > 3.0lf ? "  VOID" : ""}\n")
    }
    for (h in range(3)) {
        g_half = h
        let t = site_us(queue, CLOCK) $(enc; i) {
            enc_chain(enc, p, b, i % NL)
        }
        if (!t.ok || r.tone[h] <= 0.0lf) {   //! the row's ratio reads the one-row base's figure
            refused_row("chain8{HALF_TAG[h]}")
            continue
        }
        r.tch[h] = t.us
        print("chain8{HALF_TAG[h]}\t{t.us:7.2f} us a site  cv {t.cv:4.1f}%  {t.us / r.tone[h]:5.2f} of one row{t.cv > 3.0lf ? "  VOID" : ""}\n")
    }
    return <- r
}

//! one raced form: checked against the chain and the one-row pair, then timed whole and by half
def private race_arm(dev : MetalDevice?; queue : MetalCommandQueue?; b : Bench; src : string; a : Arm; refs : Refs) {
    let fl = "#define RS {a.rs}\n#define NSG {a.nsg}\n#define COLS {a.cols}\n#define KSP {max(a.ksp, 1)}\n"
    var ap = ArmPsos(dn = pso_of(dev, src, fl, "hcn8"), up = pso_of(dev, src, fl, a.fin ? "sglo8f" : "sglo8"), fin = pso_of(dev, src, fl, "lofin8"))
    return if (ap.dn == null || ap.up == null || ap.fin == null)
    if (a.mu) {
        ap.prep = pso_of(dev, src, fl, a.nf ? "hcprep8n" : "hcprep8")
        ap.mdn = pso_of(dev, src, fl, "r8q8s")
        ap.mfin = pso_of(dev, src, fl, a.nf ? "lofin8n" : (a.wfin ? "lofin8w" : "lofin8t"))
        ap.mup = pso_of(dev, src, fl, a.ksp > 0 ? "r8up_mixk" : "r8up_mix")
        return if (ap.prep == null || ap.mdn == null || ap.mfin == null || ap.mup == null)
    }
    var err = ""
    var gms = 0.0lf
    g_half = 0
    with_compute_encoder_timed(queue, err, gms) $(enc) {
        enc_arm(enc, ap, a, b, 0)
    }
    let ry = rel_of(refs.xb, b.y)
    let ri = rel_of(refs.inj, b.inj8)
    let r1 = rel_of(refs.one_xb, b.y)
    for (h in range(3)) {
        g_half = h
        let t = site_us(queue, CLOCK) $(enc; i) {
            enc_arm(enc, ap, a, b, i % NL)
        }
        if (!t.ok || refs.tone[h] <= 0.0lf || refs.tch[h] <= 0.0lf) {   //! the row's ratios read both bases' figures
            refused_row("{a.name}{HALF_TAG[h]}")
            continue
        }
        print("{a.name}{HALF_TAG[h]}\t{t.us:7.2f} us a site  cv {t.cv:4.1f}%  {t.us / refs.tone[h]:5.2f} of one row  {t.us / refs.tch[h]:5.2f} of the chain")
        if (h == 0) {
            print("  vs chain: xb {ry} inj {ri}; row 0 vs one row {r1}")
        }
        print("{t.cv > 3.0lf ? "  VOID" : ""}\n")
    }
}

[export]
def main() : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        let src = fread("modules/dasLLAMA/harness/hc_mix.metal")
        if (empty(src)) {
            print("hc_mix_race: run from the repo root\n")
            return 1
        }
        let b <- make_bench(dev)
        var p = make_psos(dev, src)
        return 1 if (p.hcn == null || p.sglo == null || p.hcnorm == null || p.router == null || p.prep == null || p.r8q8 == null || p.hclo == null || p.hcmix == null)
        let refs <- race_bases(dev, queue, p, b, src)
        var inscope all <- arms()
        for (a in all) {
            if (arm_wanted(g_env_harness.hc_mix_arms, a.name)) {
                race_arm(dev, queue, b, src, a, refs)
            }
        }
        return 0
    } else {
        print("hc_mix_race: no das_metal\n")
        return 1
    }
}
