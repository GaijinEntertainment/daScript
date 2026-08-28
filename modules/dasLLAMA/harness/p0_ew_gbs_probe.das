options gen2
options persistent_heap
options _dasllama_internal = true

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_kernels    // production MetalAdd / MetalRmsNorm MSL
require ?das_metal dasllama/dasllama_metal_prefill    // production MetalCvtHalf MSL
require math
require strings

// Phase-0b: achieved GB/s on single production ew dispatches at the Qwen3-8B prefill shape
// (512 rows x dim 4096, f32). Confirms or kills the traffic model for the 11.2ms ew pool:
// bandwidth-bound (hundreds of GB/s) vs dispatch-latency-bound (tens). Back-to-back reps on
// the same buffers recycle SLC - which is also the production regime (panels are written and
// re-read within one layer). A total=64 add chain floors the per-dispatch latency.

def upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def upload_f32(dev; v : float) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def fill_ones(buf; n : int64) {
    var p = unsafe(reinterpret<float?>(metal_buffer_contents(buf)))
    for (i in range64(n)) {
        unsafe(p[i]) = 1.0
    }
}

// one timed encoder: set state once, `reps` back-to-back dispatches (hazard tracking on the
// shared buffers serializes them); returns per-dispatch GPU microseconds, or -1 on failure
def time_reps(queue; pso; bufs; groups : uint3; tgsz : uint; tgmem : uint64; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        for (bi in range(length(bufs))) {
            metal_set_buffer(enc, bufs[bi], 0ul, bi)
        }
        if (tgmem > 0ul) {
            metal_set_threadgroup_memory_length(enc, tgmem, 0)
        }
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, groups, uint3(tgsz, 1u, 1u))
        }
    }
    if (!ok) {
        print("dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms * 1000.0lf / double(reps)
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        if (dev == null) {
            panic("p0b: no metal device")
        }
        var queue = metal_new_command_queue(dev)
        var err : string
        var pso_add = pipeline_from_source(dev, MetalAdd_metal_ew2_msl, MetalAdd_metal_ew2_msl_entry, MetalAdd_metal_ew2_msl_fastmath, err)
        var pso_rms = pipeline_from_source(dev, MetalRmsNorm_metal_rmsnorm_msl, MetalRmsNorm_metal_rmsnorm_msl_entry, MetalRmsNorm_metal_rmsnorm_msl_fastmath, err)
        var pso_cvt = pipeline_from_source(dev, metal_cvt_half_msl, metal_cvt_half_msl_entry, metal_cvt_half_msl_fastmath, err)
        if (pso_add == null || pso_rms == null || pso_cvt == null) {
            panic("p0b: PSO compile failed - {err}")
        }
        let rows = 512l
        let dim = 4096l
        let total = rows * dim   // 2.10M elements, 8.39MB f32
        var bg = metal_new_buffer(dev, uint64(total * 4l))
        var bu = metal_new_buffer(dev, uint64(total * 4l))
        var bw = metal_new_buffer(dev, uint64(dim * 4l))
        var by = metal_new_buffer(dev, uint64(total * 4l))
        var bh = metal_new_buffer(dev, uint64(total * 2l))
        fill_ones(bg, total)
        fill_ones(bu, total)
        fill_ones(bw, dim)
        var u_total = upload_u32(dev, uint(total))
        var u_dim = upload_u32(dev, uint(dim))
        var u_eps = upload_f32(dev, 1e-6)
        var u_one = upload_f32(dev, 1.0)
        var u_64 = upload_u32(dev, 64u)
        let reps = 200
        let mb = 1.0lf / (1024.0lf * 1024.0lf)
        let add_bufs <- [bg, bu, u_total, u_one]           // residual add: g += u in place - r 2 panels, w 1 (25.2MB)
        let rms_bufs <- [bg, bw, by, u_dim, u_eps]         // rms norm: r 1 panel + weight, w 1 (16.8MB)
        let cvt_bufs <- [bg, bh, u_total]                  // f32->f16 convert: r 1 f32 panel, w 1 f16 (12.6MB)
        let tiny_bufs <- [bg, bu, u_64, u_one]             // latency floor: total=64 add, one tg, dependent chain
        for (round in range(3)) {
            let add_us = time_reps(queue, pso_add, add_bufs, uint3(uint((total + 63l) / 64l), 1u, 1u), 64u, 0ul, reps)
            let add_gbs = double(total * 4l * 3l) * mb / 1024.0lf / (add_us / 1.0e6lf)
            let rms_us = time_reps(queue, pso_rms, rms_bufs, uint3(uint(rows), 1u, 1u), 256u, MetalRmsNorm_metal_rmsnorm_msl_tgmem, reps)
            let rms_gbs = double(total * 4l * 2l + dim * 4l) * mb / 1024.0lf / (rms_us / 1.0e6lf)
            let cvt_us = time_reps(queue, pso_cvt, cvt_bufs, uint3(uint((total + 255l) / 256l), 1u, 1u), 256u, 0ul, reps)
            let cvt_gbs = double(total * 4l + total * 2l) * mb / 1024.0lf / (cvt_us / 1.0e6lf)
            let tiny_us = time_reps(queue, pso_add, tiny_bufs, uint3(1u, 1u, 1u), 64u, 0ul, reps * 4)
            print("p0b round={round} add_us={add_us:.1f} add_gbs={add_gbs:.0f} rms_us={rms_us:.1f} rms_gbs={rms_gbs:.0f} cvt_us={cvt_us:.1f} cvt_gbs={cvt_gbs:.0f} tiny_add_us={tiny_us:.2f}\n")
        }
    } else {
        print("p0b: no das_metal on this build\n")
    }
}
