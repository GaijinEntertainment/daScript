options gen2
options indenting = 4
options _dasllama_internal = true

module _metal_race_fixture shared public

//! The raw-MSL Metal races' shared scaffold: a chain of sites in one timed command buffer, its per-site mean and cv over
//! rounds, the read-bandwidth reference over one plane, and a device output read back and held to a reference.
//! A figure drops the caller's `RaceClock.burn` warmup chains and is computed over the `rounds` timed after them.

require math
require metal/das_metal_boost
require dasllama/dasllama_metal_common   // race_uniform_u32
require _race_fixture   // held: the largest difference and its magnitude bound
require _row_stats   // spread_of: the mean and cv of the timed chains

//! a timed figure's shape: `rounds` chains timed after `burn` untimed ones
struct RaceClock {
    sites : int
    rounds : int
    burn : int
}

//! the GPU ms of one command buffer encoding `step(i)` for `ch` sites, a barrier after each; -1 where it failed
def chain_ms(queue : MetalCommandQueue?; ch : int; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : double {
    var err = ""
    var gpu = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu) $(enc) {
        for (i in range(ch)) {
            invoke(step, enc, i)
            metal_memory_barrier(enc)
        }
    }
    if (!ok) {
        print("chain: {err}\n")
    }
    return ok ? gpu : -1.0lf
}

typedef SiteTime = tuple<us : double; cv : double; ok : bool>

//! us a site: the mean of the clock's timed chains after its burn, and the chains' cv in percent; not `ok` (no figure)
//! where a timed chain failed
def site_us(queue : MetalCommandQueue?; clk : RaceClock; step : block<(enc : MetalComputeEncoder?; i : int) : void>) : SiteTime {
    for (_w in range(clk.burn)) {
        chain_ms(queue, clk.sites, step)
    }
    var ms <- [for (_r in range(clk.rounds)); chain_ms(queue, clk.sites, step)]
    for (m in ms) {
        return (us = 0.0lf, cv = 0.0lf, ok = false) if (m < 0.0lf)
    }
    var t <- [for (m in ms); float(m * 1000.0lf / double(clk.sites))]
    let s = spread_of(t)
    return (us = double(s.mean), cv = double(s.cv), ok = true)
}

//! the report row of `name` whose timed chains failed: a refusal, no figure
def refused_row(name : string) {
    print("{name}\trefused: a timed chain failed\n")
}

//! the read-bandwidth reference: `bw_read` of `src` over one `stride`-byte plane of `plane` a site, site i reading copy i % `copies`
def bw_read_us(dev : MetalDevice?; queue : MetalCommandQueue?; clk : RaceClock; src : string; plane : MetalBuffer?; stride : uint64; copies : int;
               out : MetalBuffer?; threadgroups : uint) : SiteTime {
    var err = ""
    var pbw = pso_from_source(dev, src, "bw_read", true, err)
    var bn4 = race_uniform_u32(dev, uint(stride / 16ul))
    return site_us(queue, clk) $(enc; i) {
        metal_set_pipeline(enc, pbw)
        metal_set_buffer(enc, plane, uint64(i % copies) * stride, 2)
        metal_set_buffer(enc, out, 0ul, 4)
        metal_set_buffer(enc, bn4, 0ul, 6)
        metal_dispatch_threadgroups(enc, uint3(threadgroups, 1u, 1u), uint3(256u, 1u, 1u))
    }
}

//! the largest difference of `by`'s first floats from `want`, over want's largest magnitude
def rel_of(want : array<float>; by : MetalBuffer?) : float {
    let h = held(buf_download(by, length(want), type<float>), want, 1.0)
    return h.diff / max(h.bound, 1e-30)
}
