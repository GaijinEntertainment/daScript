options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_gpu_host_twin shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require dasllama/dasllama_math_default   // dot_q8q8_f16s - the q8 twin's block dot
require dasllama/dasllama_gpu_kernels_discrete
require spirv/spirv_shader   // the templates' builtins (sdot4, unpackHalf2x16, the subgroup folds), run here on the CPU
require daslib/math_bits     // uint_bits_to_float: the nv4 leaf's site word, read by its decode stamped here
require daslib/templates
require daslib/typemacro_boost
require math

// The host twin both discrete homes register: the GEMV templates' own block decode run on the CPU over a streamed stack's
// device-layout planes - the test oracle of that layout (`ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena`). The
// leaves below stamp the templates with no dispatch home: no kernel, no `run`, the decode methods alone.

class private HostKqGemvK4 : K4GemvT {}
class private HostKqGemvQ40 : Q40GemvT {}
class private HostKqGemvK5 : K5GemvT {}
class private HostKqGemvIq4xs : Iq4xsGemvT {}
class private HostKqGemvIq3s4 : Iq4xsGemvT {
    override IQODD = true
}
class private HostKqGemvK3 : K3GemvT {}
class private HostKqGemvIq3s : Iq3sGemvT {}
class private HostKqGemvIq3xxs : Iq3xxsGemvT {}
class private HostKqGemvIq4nl : Iq4nlGemvT {}
class private HostKqGemvK2 : K2GemvT {}
class private HostKqGemvIq2s : Iq2sGemvT {}
class private HostKqGemvIq2xs : Iq2xsGemvT {}
class private HostKqGemvIq2xxs : Iq2xxsGemvT {}
class private HostKqGemvK6 : K6GemvT {}
class private HostKqGemvNv4 : Nv4GemvT {}

class private HostQ51Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q51
    override Q51 = true
}

class private HostIq4nl32Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_iq4nl32
}

class private HostQ4032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q4032
}

class private HostQ5032Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_q5032
}

class private HostMx4Gemv : Blk32GemvRunT {
    typedef FMT = KqTag_mx4
    override MX4 = true
}

//! a grid format's superblock class with its staged codebook filled from the main-context table, since no kernel head runs
def private host_grid_cls(var k : auto(TK)?; grid : auto) : KqGemvBase? {
    for (i in range(length(grid))) {
        k.gridw[i] = grid[i]
    }
    return k
}

//! the kq GEMV class of a superblock member on the CPU - its blk_contrib IS the device's block decode, bit for bit; null for a
//! member the superblock family does not stamp (`kq_fmt_stamp` resolves the member's overload at compile time)
def private host_kq_new(_t : KqTag_k4) : KqGemvBase? => new HostKqGemvK4()
def private host_kq_new(_t : KqTag_q40) : KqGemvBase? => new HostKqGemvQ40()
def private host_kq_new(_t : KqTag_k5) : KqGemvBase? => new HostKqGemvK5()
def private host_kq_new(_t : KqTag_iq4xs) : KqGemvBase? => new HostKqGemvIq4xs()
def private host_kq_new(_t : KqTag_iq3s4) : KqGemvBase? => new HostKqGemvIq3s4()
def private host_kq_new(_t : KqTag_k3) : KqGemvBase? => new HostKqGemvK3()
def private host_kq_new(_t : KqTag_iq3s) : KqGemvBase? => host_grid_cls(new HostKqGemvIq3s(), IQ3S_GRID)
def private host_kq_new(_t : KqTag_iq3xxs) : KqGemvBase? => host_grid_cls(new HostKqGemvIq3xxs(), IQ3XXS_GRID)
def private host_kq_new(_t : KqTag_iq4nl) : KqGemvBase? => new HostKqGemvIq4nl()
def private host_kq_new(_t : KqTag_k2) : KqGemvBase? => new HostKqGemvK2()
def private host_kq_new(_t : KqTag_iq2s) : KqGemvBase? => host_grid_cls(new HostKqGemvIq2s(), IQ2S_GRID)
def private host_kq_new(_t : KqTag_iq2xs) : KqGemvBase? => host_grid_cls(new HostKqGemvIq2xs(), IQ2XS_GRID)
def private host_kq_new(_t : KqTag_iq2xxs) : KqGemvBase? => host_grid_cls(new HostKqGemvIq2xxs(), IQ2XXS_GRID)
def private host_kq_new(_t : KqTag_k6) : KqGemvBase? => new HostKqGemvK6()
def private host_kq_new(_t : KqTag_q8) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_q51) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_mx4) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_iq4nl32) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_q4032) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_q5032) : KqGemvBase? => null
def private host_kq_new(_t : KqTag_nv4) : KqGemvBase? => new HostKqGemvNv4()

def private host_kq_cls(f : KqFmt) : KqGemvBase? {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    var k : KqGemvBase?
    kq_fmt_stamp(f) $(F) {
        k = host_kq_new(F)
    }
    return k
}

enum private TwinFamily {
    superblock
    block32
    q8
}

//! a lane's host twin: the format's GEMV class - the superblock family through its base, a per-32 or mx4 format its own
//! class, q8 none (its dot is spelled here) - and the device plane strides it reads at
struct private HostTwin {
    fam : TwinFamily
    fmt : KqFmt   //! the lane's format: the per-32 family's rows core stamps on it
    k : KqGemvBase?
    q51 : HostQ51Gemv?
    iq4nl32 : HostIq4nl32Gemv?
    q4032 : HostQ4032Gemv?
    q5032 : HostQ5032Gemv?
    mx4 : HostMx4Gemv?
    qsb : int64   //! device bytes a block (a superblock, or a 32-block)
    ssb : int64
}

//! the per-32 and mx4 members' own classes on the twin (`kq_fmt_stamp` resolves the member's overload; a member outside the family
//! makes none)
def private host_b32_new(var tw : HostTwin; _t : KqTag_q51) { tw.q51 = new HostQ51Gemv() }
def private host_b32_new(var tw : HostTwin; _t : KqTag_iq4nl32) { tw.iq4nl32 = new HostIq4nl32Gemv() }
def private host_b32_new(var tw : HostTwin; _t : KqTag_q4032) { tw.q4032 = new HostQ4032Gemv() }
def private host_b32_new(var tw : HostTwin; _t : KqTag_q5032) { tw.q5032 = new HostQ5032Gemv() }
def private host_b32_new(var tw : HostTwin; _t : KqTag_mx4) { tw.mx4 = new HostMx4Gemv() }
[unused_argument(tw, _t)]
def private host_b32_new(var tw : HostTwin; _t : auto) {}

//! MoeGpuHostTwinFn: a per-lane host twin for `fmt` - null for a format the twin does not serve (`expert_chain_slot_ok`, the
//! seat's serves predicate)
[arch(at="../ARCHITECTURE_GPU_VULKAN_STREAM.md#streamed-layer-arena")]
def host_twin(fmt : int) : void? {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    return null if (!expert_chain_slot_ok(fmt))
    let f = kq_fmt_of_slot(fmt)
    var tw = new HostTwin()
    tw.fmt = f
    let b = dev_block_bytes(f)
    tw.qsb = b.qsb
    tw.ssb = b.ssb
    if (kq_sb(f)) {
        tw.fam = TwinFamily.superblock
        tw.k = host_kq_cls(f)
    } elif (f == KqFmt.q8) {
        tw.fam = TwinFamily.q8
    } else {
        tw.fam = TwinFamily.block32
        kq_fmt_stamp(f) $(F) {
            host_b32_new(*tw, F)
        }
    }
    return unsafe(reinterpret<void?>(tw))
}

//! `a` as a borrowed view of `count` elements at `p` (the earlier view forgotten)
def private bind_view(var a : array<auto(TT)>; p : void?; count : int64) {
    unsafe {
        if (lock_count(a) != 0) {
            _builtin_forget_temp_array(a)
        }
        _builtin_make_temp_array_i64(a, p, count)
    }
}

//! the per-32 and mx4 twin: the class's own block decode and dot (`blk_words`, `blk_xdot`), its planes and the Q8_0
//! activation bound as borrowed views, the row's blocks summed in order
def private b32_rows(var k : auto(TK)?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re, qsb, ssb : int64) {
    let nb = n / KQ_B32_ELEMS
    unsafe {
        bind_view(k.wq, reinterpret<void?>(q), re * nb * qsb / 4l)
        bind_view(k.wsu, reinterpret<void?>(s), (re * nb * ssb + 3l) / 4l)
        bind_view(k.xqw, reinterpret<void?>(xq), n / 4l)
        bind_view(k.axs, reinterpret<void?>(xs), nb)
        for (r in range64(rb, re)) {
            var acc = 0.0
            let wb0 = uint(r * nb)
            for (b in range64(nb)) {
                var wl = uint4(0u)
                var wh = uint4(0u)
                let sc = k->blk_words(wb0 + uint(b), wl, wh)
                acc = k->blk_xdot(wl, wh, sc, uint(b), acc)
            }
            yp[r] = acc
        }
    }
}

//! the q8 twin: the device's int8 rows and f16 block scales, a block's dot in integers, (dot x d) x the activation scale
//! - the `Q8Gemv` class's fold order
def private q8_rows(var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    let nb = n / KQ_B32_ELEMS
    unsafe {
        let wq = reinterpret<int8 const?>(q)
        let ws = reinterpret<uint16 const?>(s)
        for (r in range64(rb, re)) {
            yp[r] = dot_q8q8_f16s(wq + r * n, ws + r * nb, xq, xs, n)
        }
    }
}

//! the per-32 and mx4 members' rows through the twin's own class (`kq_fmt_stamp` resolves the member's overload; a member outside
//! the family folds nothing)
def private host_b32_rows(var tw : HostTwin; _t : KqTag_q51; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    b32_rows(tw.q51, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
}
def private host_b32_rows(var tw : HostTwin; _t : KqTag_iq4nl32; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    b32_rows(tw.iq4nl32, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
}
def private host_b32_rows(var tw : HostTwin; _t : KqTag_q4032; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    b32_rows(tw.q4032, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
}
def private host_b32_rows(var tw : HostTwin; _t : KqTag_q5032; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    b32_rows(tw.q5032, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
}
def private host_b32_rows(var tw : HostTwin; _t : KqTag_mx4; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {
    b32_rows(tw.mx4, yp, q, s, xq, xs, n, rb, re, tw.qsb, tw.ssb)
}
[unused_argument(tw, _t, yp, q, s, xq, xs, n, rb, re)]
def private host_b32_rows(var tw : HostTwin; _t : auto; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {}

//! MoeGpuHostRowsFn over the kq GEMV classes: the rows' superblocks folded a block at a time through the class's own decode
def host_rows(twin : void?; var yp : float?; q : uint8 const?; s : uint8 const?; xq : int8 const?; xs : float const?; n, rb, re : int64) {   // nolint:STYLE037 - kq_fmt_stamp's generated arms, one a format
    var tw = unsafe(reinterpret<HostTwin?>(twin))
    if (tw.fam == TwinFamily.q8) {
        q8_rows(yp, q, s, xq, xs, n, rb, re)
        return
    }
    if (tw.fam == TwinFamily.block32) {
        let f = tw.fmt
        kq_fmt_stamp(f) $(F) {
            host_b32_rows(*tw, F, yp, q, s, xq, xs, n, rb, re)
        }
        return
    }
    var k = tw.k
    let nsb = n / KQ_SB_ELEMS
    let qsb = tw.qsb
    let ssb = tw.ssb
    unsafe {
        bind_view(k.wq, reinterpret<void?>(q), re * nsb * qsb / 4l)
        bind_view(k.wq4, reinterpret<void?>(q), re * nsb * qsb / 16l)
        bind_view(k.wsu, reinterpret<void?>(s), re * nsb * ssb / 4l)
        bind_view(k.xq4, reinterpret<void?>(xq), n / 16l)
        bind_view(k.axs, reinterpret<void?>(xs), nsb)
        for (r in range64(rb, re)) {
            var acc = 0.0
            let wsb0 = uint(r * nsb)
            for (sb in range64(nsb)) {
                for (blk in range64(KQ_SB_ELEMS / KQ_B32_ELEMS)) {
                    acc += k->blk_contrib(wsb0 + uint(sb), uint(blk), uint(sb))
                }
            }
            yp[r] = acc
        }
    }
}

//! the twin's seats, the same on either home: the twin, its rows core and the formats it serves (every expert format the discrete homes stream)
def install_host_twin {
    set_moe_gpu_host_twin_fns(@@host_twin, @@host_rows, @@expert_chain_slot_ok)
}
