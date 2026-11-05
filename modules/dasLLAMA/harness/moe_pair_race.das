options gen2
options persistent_heap
options _dasllama_internal = true

// MOE PAIR RACE - the routed experts' eight-row pair on the 35B's shapes (256 experts, 8 picks a row, K 2048 gate and up of
// 512 rows, K 512 down of 2048 rows, a 512-wide shared down), iq4xs split planes over NL layer copies so a layer's experts
// stream past the SLC, the rows' picks drawn to the verify's overlap. The raw MSL (moe_pair.metal) holds the engine's two
// kernels as generated (`gu_base`, `dn_base`) and the raced forms; each form is checked bit for bit against its base and timed
// as a chain of CH layers. DASLLAMA_MOE_PAIR_ARMS (comma list of arm names) narrows a run, DASLLAMA_MOE_PAIR_REUSE sets the
// rows' overlap, DASLLAMA_MOE_PAIR_ROWS the row count.
//
// DASLLAMA_MOE_PAIR_FMT=mlx4b races the engine's own stamps instead, through their generated builders, on the Flash-Next MLX
// pack's routed block (512 experts, 10 picks a row, K 2560 gate and up of 640 rows, K 640 down of 2560 rows, a 640-wide shared
// down, four hyper-connection streams, group-64 affine 4-bit BF16 planes) at one row and at eight: the per-stack route (the
// wide gathered GEMVs, the act, the shared down, the combine) against the fused pair (`MetalB32GuT`, `MetalB32Down2T`), each
// stage timed alone as a chain of CH steps whose picks rotate over NLS selection sets so the experts stream from DRAM.
//
//   bin/daslang -jit modules/dasLLAMA/harness/moe_pair_race.das

require daslib/fio
require daslib/strings_boost
require daslib/math_bits
require math
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_kernels
require dasllama/dasllama_math
require dasllama/dasllama_env

let NE = 256
let KP = 8
let SS = 16
var NST = 8   //! the rows; MOE_PAIR_ROWS sets it (1 to 8)
let DIM = 2048
let NFE = 512
let NSH = 512
let NL = 6
let CH = 12
let ROUNDS = 5

//! one expert stack over the NL layers: the quant plane (128 B a superblock), the 16-byte strips, the f16 d plane
struct Stack {
    q : MetalBuffer?
    s : MetalBuffer?
    d : MetalBuffer?
    lblk : int64   //! a layer's superblocks
}

struct Arm {
    name : string
    entry : string
    gu : bool
    flags : string = ""
    rows_tg : int = 4   //! output rows a threadgroup (grid x = rows / rows_tg)
    tg : int = 64
    flat : bool = false   //! one grid dimension, the kernel derives (stream, slot, row block) - `FLAT`
    one : bool = false    //! a threadgroup a row block over every stream
}

def private lcg(var s : uint&) : uint {
    s = s * 1664525u + 1013904223u
    return s >> 8u
}

def private make_stack(dev : MetalDevice?; lblk : int64; seed : uint) : Stack {
    let nblk = lblk * int64(NL)
    var q : array<uint>
    q |> reserve_resize(nblk * 32l)
    var h = seed
    for (i in range64(nblk * 32l)) {
        h = h * 2654435761u + 40503u
        q[i] = h
    }
    var s : array<int8>
    s |> reserve_resize(nblk * 16l)
    for (b in range64(nblk)) {
        for (j in range(16)) {
            s[b * 16l + int64(j)] = int8(j < 8 ? int((b * 5l + int64(j) * 3l) % 23l) - 11 : 0)
        }
    }
    var d : array<float16>
    d |> reserve_resize(nblk)
    for (b in range64(nblk)) {
        d[b] = float16(0.0009765625 * float(1 << int(b % 3l)))
    }
    var st = Stack(lblk = lblk)
    st.q = metal_buffer_from(dev, uint64(nblk * 128l), unsafe(addr<void?>(q[0])))
    st.s = metal_buffer_from(dev, uint64(nblk * 16l), unsafe(addr<void?>(s[0])))
    st.d = metal_buffer_from(dev, uint64(nblk * 2l), unsafe(addr<void?>(d[0])))
    delete q
    delete s
    delete d
    return st
}

//! layer l's selection rows: NST rows of KP picks then KP weights, the picks distinct a row, a later row reusing an earlier
//! row's expert at `reuse` per mille; returns the distinct count
def private make_sel(var sel : array<uint>; l : int; reuse : uint) : int {
    sel |> resize(NST * SS)
    var h = uint(l * 7919 + 17)
    var seen : array<uint>
    for (r in range(NST)) {
        for (j in range(KP)) {
            var e = 0u
            while (true) {
                if (r > 0 && lcg(h) % 1000u < reuse && !empty(seen)) {
                    e = seen[int(lcg(h) % uint(length(seen)))]
                } else {
                    e = lcg(h) % uint(NE)
                }
                var dup = false
                for (j2 in range(j)) {
                    dup = dup || sel[r * SS + j2] == e
                }
                break if (!dup)
            }
            sel[r * SS + j] = e
            if (find_index(seen, e) < 0) {
                seen |> push(e)
            }
            sel[r * SS + KP + j] = float_bits_to_uint(0.05 + 0.01 * float(j))
        }
    }
    return length(seen)
}

def private args_buf(dev : MetalDevice?; gu : bool) : MetalBuffer? {
    var w : uint[12]
    if (gu) {
        w = fixed_array<uint>(uint(DIM), uint(NFE), uint(NFE * DIM / 256), 0u, uint(SS), uint(DIM), uint(KP * NFE), 0u, 0u, 0u, 0u, 0u)
    } else {
        w = fixed_array<uint>(uint(NFE), uint(DIM), uint(NFE * DIM / 256), uint(NFE), uint(SS), uint(KP * NFE), uint(DIM), uint(NST), uint(NSH), uint(NE + 1), uint(NE), 0u)
    }
    return metal_buffer_from(dev, 48ul, unsafe(addr<void?>(w[0])))
}

struct Bench {
    gate, up, down : Stack
    shd : Stack                 //! the shared expert's down, one plane a layer
    sel : array<MetalBuffer?>   //! a layer's selection rows
    x : MetalBuffer?            //! the gate/up x rows
    act : array<MetalBuffer?>   //! a layer's activated rows (gu_base's), the down's x
    xsh : MetalBuffer?
    lg : MetalBuffer?
    ygu : MetalBuffer?
    ydn : MetalBuffer?
    kgu : MetalBuffer?
    kdn : MetalBuffer?
    distinct : int   //! the distinct experts over the NL layers
}

def private enc_arm(enc : MetalComputeEncoder?; pso : MetalComputePipeline?; b : Bench; a : Arm; l : int; ygu, ydn : MetalBuffer?) {
    metal_set_pipeline(enc, pso)
    let sk = a.gu ? b.gate : b.down
    let ql = uint64(l) * uint64(sk.lblk)
    metal_set_buffer(enc, sk.d, ql * 2ul, 0)
    metal_set_buffer(enc, sk.s, ql * 16ul, 1)
    metal_set_buffer(enc, sk.q, ql * 128ul, 2)
    metal_set_buffer(enc, a.gu ? b.x : b.act[l], 0ul, 3)
    metal_set_buffer(enc, a.gu ? ygu : ydn, 0ul, 4)
    metal_set_buffer(enc, a.gu ? b.kgu : b.kdn, 0ul, 5)
    metal_set_buffer(enc, b.sel[l], 0ul, 7)
    let s2 = a.gu ? b.up : b.shd
    let q2 = uint64(l) * uint64(s2.lblk)
    metal_set_buffer(enc, s2.d, q2 * 2ul, 8)
    metal_set_buffer(enc, s2.s, q2 * 16ul, 9)
    metal_set_buffer(enc, s2.q, q2 * 128ul, 10)
    metal_set_buffer(enc, b.xsh, 0ul, 11)
    metal_set_buffer(enc, b.lg, 0ul, 12)
    metal_set_buffer(enc, b.sel[l], 0ul, 13)
    let rows = a.gu ? NFE : DIM
    let gx = uint((rows + a.rows_tg - 1) / a.rows_tg)
    if (a.one) {
        metal_dispatch_threadgroups(enc, uint3(gx, 1u, 1u), uint3(uint(a.tg), 1u, 1u))
    } elif (a.flat) {
        metal_dispatch_threadgroups(enc, uint3(gx * uint(NST) * (a.gu ? uint(KP) : 1u), 1u, 1u), uint3(uint(a.tg), 1u, 1u))
    } elif (a.gu) {
        metal_dispatch_threadgroups(enc, uint3(gx, uint(KP), uint(NST)), uint3(uint(a.tg), 1u, 1u))
    } else {
        metal_dispatch_threadgroups(enc, uint3(gx, uint(NST), 1u), uint3(uint(a.tg), 1u, 1u))
    }
}

def private chain_ms(queue : MetalCommandQueue?; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : double {
    var err = ""
    var gpu = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu) $(enc) {
        for (i in range(CH)) {
            invoke(step, enc, i)
            metal_memory_barrier(enc)
        }
    }
    if (!ok) {
        print("chain: {err}\n")
    }
    return ok ? gpu : -1.0lf
}

//! us a layer: the mean of ROUNDS chains after a burn, and the chains' cv
def private site_us(queue : MetalCommandQueue?; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : tuple<us : double; cv : double> {
    for (_w in range(10)) {
        chain_ms(queue, step)
    }
    var t <- [for (_r in range(ROUNDS)); chain_ms(queue, step) * 1000.0lf / double(CH)]
    var sum = 0.0lf
    for (v in t) {
        sum += v
    }
    let mean = sum / double(ROUNDS)
    var d2 = 0.0lf
    for (v in t) {
        d2 += (v - mean) * (v - mean)
    }
    return (us = mean, cv = sqrt(d2 / double(ROUNDS)) / mean * 100.0lf)
}

def private copy_f(by : MetalBuffer?; n : int) : array<float> {
    var r : array<float>
    r |> resize(n)
    unsafe(memcpy(unsafe(addr(r[0])), metal_buffer_contents(by), uint64(n * 4)))
    return <- r
}

def private diff_of(a : array<float>; by : MetalBuffer?) : tuple<bad : int; maxd : float> {
    var bad = 0
    var maxd = 0.0
    unsafe {
        let p = reinterpret<float?>(metal_buffer_contents(by))
        for (i in range(length(a))) {
            if (float_bits_to_uint(a[i]) != float_bits_to_uint(p[i])) {
                bad++
                maxd = max(maxd, abs(a[i] - p[i]))
            }
        }
    }
    return (bad = bad, maxd = maxd)
}

def private wanted(list : string; name : string) : bool {
    return true if (empty(list))
    for (w in split(list, ",")) {
        return true if (w == name)
    }
    return false
}

def private arms() : array<Arm> {
    return <- [Arm(name = "gu_base", entry = "gu_base", gu = true),
               Arm(name = "dn_base", entry = "dn_base", gu = false),
               Arm(name = "gu_dd", entry = "gu_dd", gu = true),
               Arm(name = "gu_r2", entry = "gu_rows", gu = true, flags = "#define GR 2\n", rows_tg = 4),
               Arm(name = "gu_r4", entry = "gu_rows", gu = true, flags = "#define GR 4\n", rows_tg = 8),
               Arm(name = "gu_r8", entry = "gu_rows", gu = true, flags = "#define GR 8\n", rows_tg = 16),
               Arm(name = "dn_unroll", entry = "dn_unroll", gu = false),
               Arm(name = "dn_r2", entry = "dn_rows", gu = false, flags = "#define DR 2\n", rows_tg = 4),
               Arm(name = "dn_r4", entry = "dn_rows", gu = false, flags = "#define DR 4\n", rows_tg = 8),
               Arm(name = "dn_r8", entry = "dn_rows", gu = false, flags = "#define DR 8\n", rows_tg = 16),
               Arm(name = "gu_f2", entry = "gu_rows", gu = true, flags = "#define GR 2\n#define FLAT 1\n", rows_tg = 4, flat = true),
               Arm(name = "gu_f4", entry = "gu_rows", gu = true, flags = "#define GR 4\n#define FLAT 1\n", rows_tg = 8, flat = true),
               Arm(name = "dn_f2", entry = "dn_rows", gu = false, flags = "#define DR 2\n#define FLAT 1\n", rows_tg = 4, flat = true),
               Arm(name = "dn_f4", entry = "dn_rows", gu = false, flags = "#define DR 4\n#define FLAT 1\n", rows_tg = 8, flat = true),
               Arm(name = "dn_dd2", entry = "dn_dd", gu = false, flags = "#define XR 2\n", rows_tg = 2, tg = 256, one = true),
               Arm(name = "dn_dd4", entry = "dn_dd", gu = false, flags = "#define XR 4\n", rows_tg = 4, tg = 256, one = true),
               Arm(name = "dn_dd8", entry = "dn_dd", gu = false, flags = "#define XR 8\n", rows_tg = 8, tg = 256, one = true),
               Arm(name = "dn_base nosum", entry = "dn_base", gu = false, flags = "#define DIAG_NOSUM 1\n"),
               Arm(name = "dn_r4 nosum", entry = "dn_rows", gu = false, flags = "#define DR 4\n#define DIAG_NOSUM 1\n", rows_tg = 8),
               Arm(name = "gu_base nolut", entry = "gu_base", gu = true, flags = "#define DIAG_NOLUT 1\n"),
               Arm(name = "gu_r4 nolut", entry = "gu_rows", gu = true, flags = "#define GR 4\n#define DIAG_NOLUT 1\n", rows_tg = 8),
               Arm(name = "dn_base nolut", entry = "dn_base", gu = false, flags = "#define DIAG_NOLUT 1\n"),
               Arm(name = "dn_r4 nolut", entry = "dn_rows", gu = false, flags = "#define DR 4\n#define DIAG_NOLUT 1\n", rows_tg = 8),
               Arm(name = "dn_ks2", entry = "dn_ks", gu = false, flags = "#define KR 2\n", rows_tg = 2, tg = 256),
               Arm(name = "dn_ks4", entry = "dn_ks", gu = false, flags = "#define KR 4\n", rows_tg = 4, tg = 256),
               Arm(name = "dn_ks8", entry = "dn_ks", gu = false, flags = "#define KR 8\n", rows_tg = 8, tg = 256)]
}

//! the stacks, NL layers' picks at `reuse` per mille, the input rows and the outputs
def private make_bench(dev : MetalDevice?; reuse : uint) : Bench {
    var b = Bench(gate = make_stack(dev, int64(NE * NFE * DIM / 256), 1u), up = make_stack(dev, int64(NE * NFE * DIM / 256), 2u),
                  down = make_stack(dev, int64(NE * DIM * NFE / 256), 3u), shd = make_stack(dev, int64(DIM * NSH / 256), 4u))
    b.sel |> reserve(NL)
    for (l in range(NL)) {
        var inscope sel : array<uint>
        b.distinct += make_sel(sel, l, reuse)
        b.sel |> push(metal_buffer_from(dev, uint64(NST * SS * 4), unsafe(addr<void?>(sel[0]))))
    }
    print("experts {NE}, {NST} rows x {KP} picks, distinct a layer {double(b.distinct) / double(NL):.1f} of {NST * KP} (reuse {int(reuse)} per mille)\n")
    var inscope xa <- [for (i in range(NST * DIM)); 0.03 * float(i % 17 - 8)]
    b.x = metal_buffer_from(dev, uint64(NST * DIM * 4), unsafe(addr<void?>(xa[0])))
    var inscope xs <- [for (i in range(NST * NSH)); 0.02 * float(i % 13 - 6)]
    b.xsh = metal_buffer_from(dev, uint64(NST * NSH * 4), unsafe(addr<void?>(xs[0])))
    var inscope lga <- [for (i in range(NST * (NE + 1))); 0.1 * float(i % 7 - 3)]
    b.lg = metal_buffer_from(dev, uint64(NST * (NE + 1) * 4), unsafe(addr<void?>(lga[0])))
    b.ygu = metal_buffer_from(dev, uint64(NST * KP * NFE * 4), null)
    b.ydn = metal_buffer_from(dev, uint64(NST * DIM * 4), null)
    b.kgu = args_buf(dev, true)
    b.kdn = args_buf(dev, false)
    b.act <- [for (_l in range(NL)); metal_buffer_from(dev, uint64(NST * KP * NFE * 4), null)]
    return <- b
}

//! the read-bandwidth reference over one layer's gate quant plane
def private print_bw(dev : MetalDevice?; queue : MetalCommandQueue?; b : Bench; src : string) {
    var err = ""
    var pbw = pso_from_source(dev, src, "bw_read", true, err)
    var bn4 = race_uniform_u32(dev, uint(b.gate.lblk * 8l))
    let tbw = site_us(queue) $(enc; i) {
        metal_set_pipeline(enc, pbw)
        metal_set_buffer(enc, b.gate.q, uint64(i % NL) * uint64(b.gate.lblk) * 128ul, 2)
        metal_set_buffer(enc, b.ydn, 0ul, 4)
        metal_set_buffer(enc, bn4, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(2048u, 1u, 1u), uint3(256u, 1u, 1u))
    }
    print("bw_read\t{tbw.us:7.2f} us a {b.gate.lblk * 128l / 1048576l} MB layer plane  {double(b.gate.lblk * 128l) / (tbw.us * 1000.0lf):6.1f} GB/s\n")
}

//! the bases' outputs a layer (gu_base's act rows, the down's x; dn_base's stream rows), then every wanted arm checked against
//! its base bit for bit and timed
def private race_arms(queue : MetalCommandQueue?; b : Bench; all : array<Arm>; psos : array<MetalComputePipeline?>) {
    var err = ""
    var ref_gu, ref_dn : array<array<float>>
    ref_gu |> reserve(NL)
    ref_dn |> reserve(NL)
    for (l in range(NL)) {
        var gms = 0.0lf
        with_compute_encoder_timed(queue, err, gms) $(enc) {
            enc_arm(enc, psos[0], b, all[0], l, b.act[l], b.ydn)
            metal_memory_barrier(enc)
            enc_arm(enc, psos[1], b, all[1], l, b.ygu, b.ydn)
        }
        ref_gu |> emplace(copy_f(b.act[l], NST * KP * NFE))
        ref_dn |> emplace(copy_f(b.ydn, NST * DIM))
    }
    let per = double(b.distinct) / double(NL)
    let gu_bytes = per * double(2 * NFE * DIM / 256 * 146)
    let dn_bytes = per * double(DIM * NFE / 256 * 146) + double(DIM * NSH / 256 * 146)
    for (a, ai in all, range(length(all))) {
        continue if (psos[ai] == null || !wanted(g_env_harness.moe_pair_arms, a.name))
        var bad = 0
        var maxd = 0.0
        for (l in range(NL)) {
            var gms = 0.0lf
            with_compute_encoder_timed(queue, err, gms) $(enc) {
                enc_arm(enc, psos[ai], b, a, l, b.ygu, b.ydn)
            }
            let dd = diff_of(a.gu ? ref_gu[l] : ref_dn[l], a.gu ? b.ygu : b.ydn)
            bad += dd.bad
            maxd = max(maxd, dd.maxd)
        }
        let t = site_us(queue) $(enc; i) {
            enc_arm(enc, psos[ai], b, a, i % NL, b.ygu, b.ydn)
        }
        let gbs = (a.gu ? gu_bytes : dn_bytes) / (t.us * 1000.0lf)
        print("{a.name}\t{t.us:7.2f} us a layer  cv {t.cv:4.1f}%  {gbs:6.1f} GB/s (distinct bytes)  vs base: {bad} floats differ, max {maxd}{t.cv > 3.0lf ? "  VOID" : ""}\n")
    }
}

let AFF_NE = 512
let AFF_K = 10
let AFF_DIM = 2560
let AFF_NFE = 640
let AFF_NSH = 640
let AFF_HC = 4
let AFF_NLS = 6   //! the selection sets a chain rotates over: six steps' experts (~150 MB) stream past the SLC

//! an affine member's plane pair: the codes (16 B a 32-block) and the BF16 halfwords (a 64-group's scale at its even block,
//! its bias at the odd one), filled in place
struct AffPlanes {
    q : MetalBuffer?
    s : MetalBuffer?
}

//! `nblk` blocks of hashed codes under small pow2-ish scales and negative biases
def private aff_planes(dev : MetalDevice?; nblk : int64; seed : uint) : AffPlanes {
    var p = AffPlanes(q = metal_buffer_from(dev, uint64(nblk * 16l), null), s = metal_buffer_from(dev, uint64(nblk * 2l), null))
    unsafe {
        var pq = reinterpret<uint?>(metal_buffer_contents(p.q))
        var h = seed
        for (i in range64(nblk * 4l)) {
            h = h * 2654435761u + 40503u
            pq[i] = h
        }
        var ps = reinterpret<uint16?>(metal_buffer_contents(p.s))
        for (b in range64(nblk)) {
            let v = int((b >> 1l) % 8l)
            ps[b] = uint16((b & 1l) == 0l ? 0x3C00 + v : 0xBC80 + v)
        }
    }
    return p
}

//! `nst` rows of AFF_K distinct picks then their weights, a later row's pick drawn from the earlier rows' experts at `reuse`
//! per mille; set `l` seeds the draw. Returns the buffer and its distinct expert count
def private aff_sel(dev : MetalDevice?; nst, l : int; reuse : uint) : tuple<b : MetalBuffer?; distinct : int> {
    let ss = 2 * AFF_K
    var inscope sel : array<uint>
    sel |> resize(nst * ss)
    var h = uint(l * 7919 + 17)
    var seen : array<uint>
    for (r in range(nst)) {
        for (j in range(AFF_K)) {
            var e = 0u
            while (true) {
                e = (r > 0 && lcg(h) % 1000u < reuse) ? seen[int(lcg(h) % uint(length(seen)))] : lcg(h) % uint(AFF_NE)
                var dup = false
                for (j2 in range(j)) {
                    dup = dup || sel[r * ss + j2] == e
                }
                break if (!dup)
            }
            sel[r * ss + j] = e
            if (find_index(seen, e) < 0) {
                seen |> push(e)
            }
            sel[r * ss + AFF_K + j] = float_bits_to_uint(0.05 + 0.01 * float(j))
        }
    }
    return (b = metal_buffer_from(dev, uint64(nst * ss * 4), unsafe(addr<void?>(sel[0]))), distinct = length(seen))
}

//! the routed block's operands at `nst` rows: the three stacks and the shared down (built once, `aff_stacks`), the picks' sets,
//! the rows and every landing buffer, and the uniforms the builders bind
struct AffBench {
    nst : int
    sel : array<MetalBuffer?>
    distinct : int      //! the distinct experts over the AFF_NLS sets
    x, bg, bu, bh, bdn, bshr, bst, bhc, xsh, lg, inj, uhc, btot, un_sh, ud_dim : MetalBuffer?
}

struct AffStacks {
    gate, up, down, shd : AffPlanes
}

def private aff_stacks(dev : MetalDevice?) : AffStacks {
    let eg = int64(AFF_NFE * AFF_DIM / 32)
    return AffStacks(gate = aff_planes(dev, eg * int64(AFF_NE), 1u), up = aff_planes(dev, eg * int64(AFF_NE), 2u),
        down = aff_planes(dev, eg * int64(AFF_NE), 3u), shd = aff_planes(dev, int64(AFF_DIM * AFF_NSH / 32), 4u))
}

def private aff_floats(dev : MetalDevice?; n : int; amp : float) : MetalBuffer? {
    var inscope v <- [for (i in range(n)); amp * float(i % 17 - 8)]
    return metal_buffer_from(dev, uint64(n * 4), unsafe(addr<void?>(v[0])))
}

def private aff_bench(dev : MetalDevice?; nst : int; reuse : uint) : AffBench {
    var b = AffBench(nst = nst)
    b.sel |> reserve(AFF_NLS)
    for (l in range(AFF_NLS)) {
        var s = aff_sel(dev, nst, l, reuse)
        b.sel |> push(s.b)
        b.distinct += s.distinct
    }
    let nk = nst * AFF_K
    b.x = aff_floats(dev, nst * AFF_DIM, 0.03)
    b.bg = metal_buffer_from(dev, uint64(nk * AFF_NFE * 4), null)
    b.bu = metal_buffer_from(dev, uint64(nk * AFF_NFE * 4), null)
    b.bh = aff_floats(dev, nk * AFF_NFE, 0.02)
    b.bdn = metal_buffer_from(dev, uint64(nk * AFF_DIM * 4), null)
    b.bshr = metal_buffer_from(dev, uint64(nst * AFF_DIM * 4), null)
    b.bst = metal_buffer_from(dev, uint64(nst * AFF_DIM * 4), null)
    b.bhc = aff_floats(dev, nst * AFF_HC * AFF_DIM, 0.1)
    b.xsh = aff_floats(dev, nst * AFF_NSH, 0.02)
    b.lg = aff_floats(dev, nst * (AFF_NE + 1), 0.1)
    b.inj = aff_floats(dev, nst * AFF_HC, 0.2)
    b.uhc = race_uniform_u32(dev, uint(AFF_HC))
    b.btot = race_uniform_u32(dev, uint(nk * AFF_NFE))
    b.un_sh = race_uniform_u32(dev, uint(AFF_NSH))
    b.ud_dim = race_uniform_u32(dev, uint(AFF_DIM))
    return <- b
}

//! the stamps the two routes dispatch, compiled off the engine's own generated sources
struct AffPsos {
    gemv_w, gemv2, gu, swiglu, down2, down2_hc, sh_w, sh_mvb8, comb_sh, comb_sh_hc, hc_comb : MetalComputePipeline?
}

def private aff_psos(dev : MetalDevice?; var err : string&) : AffPsos {
    return AffPsos(
        gemv_w = pipeline_from_source(dev, MetalMoeGemvMlx4bW_metal_moe_gemv_w_msl, MetalMoeGemvMlx4bW_metal_moe_gemv_w_msl_entry, MetalMoeGemvMlx4bW_metal_moe_gemv_w_msl_fastmath, err),
        gemv2 = pipeline_from_source(dev, metal_moe_gemv_mlx4b_msl, metal_moe_gemv_mlx4b_msl_entry, metal_moe_gemv_mlx4b_msl_fastmath, err),
        gu = pipeline_from_source(dev, MetalMoeGuMlx4b_metal_b32_gu_msl, MetalMoeGuMlx4b_metal_b32_gu_msl_entry, MetalMoeGuMlx4b_metal_b32_gu_msl_fastmath, err),
        swiglu = pipeline_from_source(dev, MetalSwiglu_metal_ew2_msl, MetalSwiglu_metal_ew2_msl_entry, MetalSwiglu_metal_ew2_msl_fastmath, err),
        down2 = pipeline_from_source(dev, metal_moe_down2_mlx4b_msl, metal_moe_down2_mlx4b_msl_entry, metal_moe_down2_mlx4b_msl_fastmath, err),
        down2_hc = pipeline_from_source(dev, metal_moe_down2_mlx4b_hc_msl, metal_moe_down2_mlx4b_hc_msl_entry, metal_moe_down2_mlx4b_hc_msl_fastmath, err),
        sh_w = pipeline_from_source(dev, MetalKqGemvMlx4bW_metal_b32_gemv_w_msl, MetalKqGemvMlx4bW_metal_b32_gemv_w_msl_entry, MetalKqGemvMlx4bW_metal_b32_gemv_w_msl_fastmath, err),
        sh_mvb8 = pipeline_from_source(dev, MetalB32MvB8Mlx4b_metal_b32_mvb_msl, MetalB32MvB8Mlx4b_metal_b32_mvb_msl_entry, MetalB32MvB8Mlx4b_metal_b32_mvb_msl_fastmath, err),
        comb_sh = pipeline_from_source(dev, metal_moe_combine_sh_msl, metal_moe_combine_sh_msl_entry, metal_moe_combine_sh_msl_fastmath, err),
        comb_sh_hc = pipeline_from_source(dev, metal_moe_combine_sh_hc_msl, metal_moe_combine_sh_hc_msl_entry, metal_moe_combine_sh_hc_msl_fastmath, err),
        hc_comb = pipeline_from_source(dev, metal_hc_combine_msl, metal_hc_combine_msl_entry, metal_hc_combine_msl_fastmath, err))
}

def private aff_psos_ok(p : AffPsos) : bool {
    return (p.gemv_w != null && p.gemv2 != null && p.gu != null && p.swiglu != null && p.down2 != null && p.down2_hc != null
        && p.sh_w != null && p.sh_mvb8 != null && p.comb_sh != null && p.comb_sh_hc != null && p.hc_comb != null)
}

def private aff_gu_args : MoeGemvArgs {
    return MoeGemvArgs(ndim = uint(AFF_DIM), ddim = uint(AFF_NFE), eplane = uint(AFF_NFE * AFF_DIM / 32), xs = 0u, ss = uint(2 * AFF_K),
        bxs = uint(AFF_DIM), bys = uint(AFF_K * AFF_NFE), hasb = 0u)
}

def private aff_dn_args(fused : bool) : MoeGemvArgs {
    var a = MoeGemvArgs(ndim = uint(AFF_NFE), ddim = uint(AFF_DIM), eplane = uint(AFF_DIM * AFF_NFE / 32), xs = uint(AFF_NFE),
        ss = uint(2 * AFF_K), bxs = uint(AFF_K * AFF_NFE), bys = uint(AFF_K * AFF_DIM), hasb = 0u)
    if (fused) {
        a.bys = uint(AFF_DIM)
        a.nsh = uint(AFF_NSH)
        a.gstr = uint(AFF_NE + 1)
        a.goff = uint(AFF_NE)
        a.yadd = 0u
    }
    return a
}

//! the per-stack route's gate and up: two gathered GEMVs (`wide` the wide form the route takes on 640 rows, else the two-row
//! form the pair's lane map shares) and the swiglu over the slots' rows
def private aff_gu_stack(enc : MetalComputeEncoder?; p : AffPsos; st : AffStacks; b : AffBench; sel : MetalBuffer?; wide : bool) {
    var ka = aff_gu_args()
    let rows = int64(AFF_NFE)
    let k = int64(AFF_K)
    let nst = int64(b.nst)
    if (wide) {
        enc_moe_gemv_mlx4b_w_c_pso(enc, p.gemv_w, st.gate.q, 0ul, st.gate.s, 0ul, b.x, b.bg, 0ul, ka, sel, rows, k, nst)
        enc_moe_gemv_mlx4b_w_c_pso(enc, p.gemv_w, st.up.q, 0ul, st.up.s, 0ul, b.x, b.bu, 0ul, ka, sel, rows, k, nst)
    } else {
        enc_moe_gemv_mlx4b_c_pso(enc, p.gemv2, st.gate.q, 0ul, st.gate.s, 0ul, b.x, b.bg, 0ul, ka, sel, rows, k, nst)
        enc_moe_gemv_mlx4b_c_pso(enc, p.gemv2, st.up.q, 0ul, st.up.s, 0ul, b.x, b.bu, 0ul, ka, sel, rows, k, nst)
    }
    metal_memory_barrier(enc)
    enc_swiglu_pso(enc, p.swiglu, b.bg, 0ul, b.bu, 0ul, b.btot, nst * k * rows)
}

//! the pair's gate + up + act, one dispatch
def private aff_gu_pair(enc : MetalComputeEncoder?; p : AffPsos; st : AffStacks; b : AffBench; sel : MetalBuffer?) {
    var ka = aff_gu_args()
    enc_moe_gu_mlx4b_c_pso(enc, p.gu, st.gate.q, 0ul, st.gate.s, 0ul, b.x, b.bg, 0ul, ka, sel, st.up.q, 0ul, st.up.s, 0ul,
        int64(AFF_NFE), int64(AFF_K), int64(b.nst))
}

//! the per-stack route's layer end at `nst` rows: the routed down over every slot (`wide` as the route takes it), the shared
//! down, and the combine - scattered into the streams at one row (`MetalMoeCombineShHc`), else onto the FFN-out row and the
//! streams' combine after it
def private aff_dn_stack(enc : MetalComputeEncoder?; p : AffPsos; st : AffStacks; b : AffBench; sel : MetalBuffer?; wide : bool) {
    var ka = aff_dn_args(false)
    let dim = int64(AFF_DIM)
    let nst = int64(b.nst)
    if (wide) {
        enc_moe_gemv_mlx4b_w_c_pso(enc, p.gemv_w, st.down.q, 0ul, st.down.s, 0ul, b.bh, b.bdn, 0ul, ka, sel, dim, int64(AFF_K), nst)
    } else {
        enc_moe_gemv_mlx4b_c_pso(enc, p.gemv2, st.down.q, 0ul, st.down.s, 0ul, b.bh, b.bdn, 0ul, ka, sel, dim, int64(AFF_K), nst)
    }
    if (b.nst == 1) {
        enc_kq_mlx4b_w_c_pso(enc, p.sh_w, st.shd.q, 0ul, st.shd.s, 0ul, b.xsh, 0ul, b.bshr, 0ul, b.un_sh, b.ud_dim, dim, int64(AFF_NSH))
    } else {
        var mv = KqMvArgs(ndim = uint(AFF_NSH), ddim = uint(AFF_DIM), ys = uint(AFF_DIM), nr = uint(b.nst))
        enc_b32_mvb8_mlx4b_c_pso(enc, p.sh_mvb8, st.shd.q, 0ul, st.shd.s, 0ul, b.xsh, 0ul, b.bshr, 0ul, b.un_sh, b.ud_dim, mv, dim, 1l)
    }
    metal_memory_barrier(enc)
    var ca = moe_comb_args(dim, int64(AFF_K), int64(AFF_NE + 1), int64(AFF_NE))
    if (b.nst == 1) {
        enc_moe_combine_sh_hc_pso(enc, p.comb_sh_hc, b.bhc, sel, b.bdn, b.bshr, b.lg, b.inj, b.uhc, ca, dim, nst)
    } else {
        enc_moe_combine_sh_pso(enc, p.comb_sh, b.bst, sel, b.bdn, b.bshr, b.lg, ca, dim, nst)
        metal_memory_barrier(enc)
        aff_hc_comb(enc, p, b)
    }
}

def private aff_hc_comb(enc : MetalComputeEncoder?; p : AffPsos; b : AffBench) {
    var ha = GkHcCombArgs(dim = uint(AFF_DIM), hc = uint(AFF_HC), rows = uint(b.nst))
    enc_hc_combine_pso(enc, p.hc_comb, b.bhc, b.bst, b.inj, ha, int64(b.nst * AFF_HC * AFF_DIM))
}

//! the pair's layer end: the down with the shared down and the combine one dispatch, landed on the FFN-out row and the streams'
//! combine after it (the shipped route: the pair's scatter landing serves iq4nl32 alone), or scattered by the `_hc` stamp
def private aff_dn_pair(enc : MetalComputeEncoder?; p : AffPsos; st : AffStacks; b : AffBench; sel : MetalBuffer?; scatter : bool) {
    var ka = aff_dn_args(true)
    let dim = int64(AFF_DIM)
    let nsh_off = 0ul
    if (scatter) {
        enc_moe_down2_mlx4b_hc_c_pso(enc, p.down2_hc, st.down.q, 0ul, st.down.s, 0ul, b.bh, b.bhc, 0ul, ka, sel,
            st.shd.q, nsh_off, st.shd.s, nsh_off, b.xsh, b.lg, sel, b.inj, b.uhc, dim, int64(b.nst), int64(AFF_HC))
        return
    }
    enc_moe_down2_mlx4b_c_pso(enc, p.down2, st.down.q, 0ul, st.down.s, 0ul, b.bh, b.bst, 0ul, ka, sel,
        st.shd.q, nsh_off, st.shd.s, nsh_off, b.xsh, b.lg, sel, dim, int64(b.nst))
    metal_memory_barrier(enc)
    aff_hc_comb(enc, p, b)
}

//! one arm's step `i` over its set's picks
def private aff_step(enc : MetalComputeEncoder?; p : AffPsos; st : AffStacks; b : AffBench; arm : string; i : int) {
    let sel = b.sel[i % AFF_NLS]
    if (arm == "gu_stack_w") {
        aff_gu_stack(enc, p, st, b, sel, true)
    } elif (arm == "gu_stack_2r") {
        aff_gu_stack(enc, p, st, b, sel, false)
    } elif (arm == "gu_pair") {
        aff_gu_pair(enc, p, st, b, sel)
    } elif (arm == "dn_stack_w") {
        aff_dn_stack(enc, p, st, b, sel, true)
    } elif (arm == "dn_stack_2r") {
        aff_dn_stack(enc, p, st, b, sel, false)
    } elif (arm == "dn_pair") {
        aff_dn_pair(enc, p, st, b, sel, false)
    } elif (arm == "block_stack") {   //! the whole routed block as each route encodes it
        aff_gu_stack(enc, p, st, b, sel, true)
        metal_memory_barrier(enc)
        aff_dn_stack(enc, p, st, b, sel, true)
    } elif (arm == "block_pair") {
        aff_gu_pair(enc, p, st, b, sel)
        metal_memory_barrier(enc)
        aff_dn_pair(enc, p, st, b, sel, false)
    } else {
        aff_dn_pair(enc, p, st, b, sel, true)
    }
}

//! `by`'s first `n` floats against `ref`: the largest difference over the largest magnitude
def private rel_diff(ref : array<float>; by : MetalBuffer?; n : int) : float {
    var maxd = 0.0
    var mag = 0.0
    unsafe {
        let p = reinterpret<float?>(metal_buffer_contents(by))
        for (i in range(n)) {
            maxd = max(maxd, abs(ref[i] - p[i]))
            mag = max(mag, abs(ref[i]))
        }
    }
    return maxd / max(mag, 1e-30)
}

//! the arms at `nst` rows: each stage's per-stack forms and the pair's, its outputs held against the route's, then timed
def private aff_race_rows(queue : MetalCommandQueue?; dev : MetalDevice?; p : AffPsos; st : AffStacks; nst : int) {
    var b <- aff_bench(dev, nst, uint(clamp(g_env_harness.moe_pair_reuse, 0, 1000)))
    let per = double(b.distinct) / double(AFF_NLS)
    let blk_b = 18.0lf   //! a block's 16 code bytes and its 2 scale bytes
    let gu_bytes = per * 2.0lf * double(AFF_NFE * AFF_DIM / 32) * blk_b
    let dn_bytes = per * double(AFF_DIM * AFF_NFE / 32) * blk_b + double(AFF_DIM * AFF_NSH / 32) * blk_b
    print("\nmlx4b routed block, {nst} row(s) x {AFF_K} picks of {AFF_NE}: distinct a step {per:.1f}\n")
    var err = ""
    var gms = 0.0lf
    with_compute_encoder_timed(queue, err, gms) $(enc) {
        aff_gu_stack(enc, p, st, b, b.sel[0], true)
    }
    var ref_gu <- copy_f(b.bg, nst * AFF_K * AFF_NFE)
    for (_w in range(3)) {   //! the clocks up before the first timed arm
        site_us(queue) $(enc; i) {
            aff_step(enc, p, st, b, "block_stack", i)
        }
    }
    for (arm in ["gu_stack_w", "gu_stack_2r", "gu_pair", "dn_stack_w", "dn_stack_2r", "dn_pair", "dn_pair_hc", "block_stack", "block_pair"]) {
        continue if (!wanted(g_env_harness.moe_pair_arms, arm))
        let gu = starts_with(arm, "gu")
        var rd = 0.0
        if (gu) {
            with_compute_encoder_timed(queue, err, gms) $(enc) {
                aff_step(enc, p, st, b, arm, 0)
            }
            rd = rel_diff(ref_gu, b.bg, nst * AFF_K * AFF_NFE)
        }
        let t = site_us(queue) $(enc; i) {
            aff_step(enc, p, st, b, arm, i)
        }
        let gbs = (gu ? gu_bytes : dn_bytes) / (t.us * 1000.0lf)
        let held = gu ? "  vs the wide route: rel {rd}" : ""
        print("{arm}\t{t.us:7.2f} us a step  cv {t.cv:4.1f}%  {gbs:6.1f} GB/s (distinct bytes){held}{t.cv > 3.0lf ? "  VOID" : ""}\n")
    }
    delete ref_gu
}

//! the affine race: the stacks once, then one row and eight
def private aff_race(dev : MetalDevice?; queue : MetalCommandQueue?) : int {
    var err = ""
    let p = aff_psos(dev, err)
    if (!aff_psos_ok(p)) {
        print("moe_pair_race mlx4b: pso - {err}\n")
        return 1
    }
    let st = aff_stacks(dev)
    for (nst in [1, 8]) {
        aff_race_rows(queue, dev, p, st, nst)
    }
    return 0
}

[export]
def main() : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        if (g_env_harness.moe_pair_fmt == "mlx4b") {
            return aff_race(dev, queue)
        }
        var err = ""
        var src = ""
        fopen("modules/dasLLAMA/harness/moe_pair.metal", "rb") $(f) {
            if (f != null) {
                src = fread(f)
            }
        }
        if (empty(src)) {
            print("moe_pair_race: run from the repo root\n")
            return 1
        }
        NST = clamp(g_env_harness.moe_pair_rows, 1, 8)
        let b <- make_bench(dev, uint(clamp(g_env_harness.moe_pair_reuse, 0, 1000)))
        var inscope all <- arms()
        var psos : array<MetalComputePipeline?>
        psos |> reserve(length(all))
        for (a in all) {
            var p : MetalComputePipeline? = null
            if (wanted(g_env_harness.moe_pair_arms, a.name) || a.name == "gu_base" || a.name == "dn_base") {   //! the bases at 0 and 1 make the reference
                p = pso_from_source(dev, a.flags + src, a.entry, true, err)
                if (p == null) {
                    print("{a.name}: pso - {err}\n")
                    return 1
                }
            }
            psos |> push(p)
        }
        print_bw(dev, queue, b, src)
        race_arms(queue, b, all, psos)
        return 0
    } else {
        print("moe_pair_race: no das_metal\n")
        return 1
    }
}
