options gen2
options persistent_heap
options _dasllama_internal = true

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_prefill   // the production TH128 q8 MSL text
require strings

// ATTRIBUTION SWEEP - its arms attribute GEMM time across threadgroup-memory REQUEST sizes with
// the indexed footprint held fixed, never selecting between implementations.
// GEMM-dig gate for the double-buffered staging variant: the tgmem allocation law's unmeasured
// (16384, 20480) gap, where a bk=64 ping-pong pair lands (2 x 9216 = 18432B). The production
// kernel indexes only its 6144 halves, so the REQUEST size alone varies.

def upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def fill_q8_blob(buf; n, kdim : int64) {
    var p = unsafe(reinterpret<uint8?>(metal_buffer_contents(buf)))
    var seed = 12345u
    for (blk in range64(n * (kdim / 32l))) {
        let base = blk * 34l
        unsafe(p[base]) = uint8(0x00)
        unsafe(p[base + 1l]) = uint8(0x3c)
        for (i in range64(32l)) {
            seed = seed * 1664525u + 1013904223u
            unsafe(p[base + 2l + i]) = uint8(int8(int(seed >> 16u) % 16 - 8))
        }
    }
}

def fill_x_f16(buf; total : int64) {
    var p = unsafe(reinterpret<float16?>(metal_buffer_contents(buf)))
    var seed = 777u
    for (i in range64(total)) {
        seed = seed * 1664525u + 1013904223u
        unsafe(p[i]) = float16(float(int(seed >> 16u) % 9 - 4))
    }
}

def time_gemm(queue; pso; bw, bx, by, bk, bn; gx, gy : uint; tgmem : uint64; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)
        metal_set_buffer(enc, bw, 0ul, 1)
        metal_set_buffer(enc, bx, 0ul, 2)
        metal_set_buffer(enc, by, 0ul, 3)
        metal_set_buffer(enc, bk, 0ul, 4)
        metal_set_buffer(enc, bn, 0ul, 5)
        metal_set_threadgroup_memory_length(enc, tgmem, 0)
        for (_i in range(reps)) {
            metal_dispatch_threadgroups(enc, uint3(gx, gy, 1u), uint3(128u, 1u, 1u))
        }
    }
    if (!ok) {
        print("dispatch failed: {err}\n")
        return -1.0lf
    }
    return gpu_ms * 1000.0lf / double(reps)
}

[export, arch(at="../ARCHITECTURE_GPU_PREFILL.md#prefill-gemm-ladder")]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        if (dev == null) {
            panic("p2alloc: no metal device")
        }
        var queue = metal_new_command_queue(dev)
        var err : string
        var pso = pipeline_from_source(dev, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_entry, true, err)
        if (pso == null) {
            panic("p2alloc: PSO - {err}")
        }
        let m = 512l
        let allocs <- [10240ul, 12288ul, 14336ul, 16384ul, 18432ul, 20480ul, 24576ul]
        for (g in [(k = 4096l, n = 12288l), (k = 12288l, n = 4096l)]) {
            var bw = metal_new_buffer(dev, uint64(g.n * (g.k / 32l) * 34l))
            var bx = metal_new_buffer(dev, uint64(m * g.k * 2l))
            var by = metal_new_buffer(dev, uint64(m * g.n * 4l))
            fill_q8_blob(bw, g.n, g.k)
            fill_x_f16(bx, m * g.k)
            var u_k = upload_u32(dev, uint(g.k))
            var u_n = upload_u32(dev, uint(g.n))
            let gx = uint(m / 128l)
            let gy = uint(g.n / 64l)
            for (warm in range(2)) {
                let line = build_string() $(var w) {
                    w |> write("p2alloc k={g.k} n={g.n}:")
                    for (a in allocs) {
                        let us = time_gemm(queue, pso, bw, bx, by, u_k, u_n, gx, gy, a, 12)
                        w |> write(" {int(a)}B={us}us")
                    }
                }
                if (warm == 1) {
                    print("{line}\n")
                }
            }
            metal_release(bw)
            metal_release(bx)
            metal_release(by)
            metal_release(u_k)
            metal_release(u_n)
        }
        metal_release(pso)
    } else {
        panic("p2_alloc_gap: no das_metal in this build - no GEMM to time across the tgmem gap, and a 0-row run must not read as a clean one")
    }
}
