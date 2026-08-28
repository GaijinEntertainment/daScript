options gen2
options persistent_heap
options _dasllama_internal = true

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_prefill   // the production TH128 q8 MSL text
require strings

// Phase-1 #1 race instrument: staged-W tile row stride 72 halves (production, 144B rows)
// vs the unpadded 64-half control (128B rows - the tg bank-conflict worst case). The control
// is the production MetalQ8MulMmTH128 MSL with the stage index and the W tensor strides
// rewritten back - nothing else differs, so outputs must match bit-exact.
// Interleaved race at the Qwen3-8B q8 GEMM shapes, m=512 (pp512 chunk).

def upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

def fill_q8_blob(buf; n, kdim : int64) {
    // block_q8_0 34B blocks: scale half at +0, 32 quants at +2; scales 1.0, quants -8..7 -
    // every product integer, any accumulation order bit-identical
    var p = unsafe(reinterpret<uint8?>(metal_buffer_contents(buf)))
    var seed = 12345u
    for (blk in range64(n * (kdim / 32l))) {
        let base = blk * 34l
        unsafe(p[base]) = uint8(0x00)
        unsafe(p[base + 1l]) = uint8(0x3c)   // half 1.0
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
        unsafe(p[i]) = float16(float(int(seed >> 16u) % 9 - 4))   // small ints - exact f16
    }
}

def time_gemm(queue; pso; bw, bx, by, bk, bn; gx, gy : uint; tgmem : uint64; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   // wsh - half view of the blob
        metal_set_buffer(enc, bw, 0ul, 1)   // wqb - byte view of the same blob
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

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        if (dev == null) {
            panic("p1pad: no metal device")
        }
        var queue = metal_new_command_queue(dev)
        let src_pad = MetalQ8MulMmTH128_metal_q8_mulmm_t_msl
        var src_base = replace(src_pad, "wt + col * 72u", "wt + col * 64u")
        src_base = replace(src_base, "wt + col * 40u + kh", "wt + col * 32u + kh")
        src_base = replace(src_base, "(threadgroup half *)wt, dextents<int32_t, 2>(64, 64), metal::array<int32_t, 2>\{1, 72\})", "(threadgroup half *)wt, dextents<int32_t, 2>(64, 64))")
        src_base = replace(src_base, "(threadgroup half *)wt, dextents<int32_t, 2>(32, 64), metal::array<int32_t, 2>\{1, 40\})", "(threadgroup half *)wt, dextents<int32_t, 2>(32, 64))")
        if (src_base == src_pad) {
            panic("p1pad: un-padding rewrite matched nothing - emitted MSL shape changed")
        }
        var err : string
        var pso_base = pipeline_from_source(dev, src_base, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_entry, true, err)
        if (pso_base == null) {
            panic("p1pad: base PSO - {err}")
        }
        var pso_pad = pipeline_from_source(dev, src_pad, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_entry, true, err)
        if (pso_pad == null) {
            panic("p1pad: padded PSO - {err}")
        }
        let m = 512l
        for (g in [(k = 4096l, n = 4096l), (k = 4096l, n = 12288l), (k = 12288l, n = 4096l)]) {
            var bw = metal_new_buffer(dev, uint64(g.n * (g.k / 32l) * 34l))
            var bx = metal_new_buffer(dev, uint64(m * g.k * 2l))
            var by = metal_new_buffer(dev, uint64(m * g.n * 4l))
            var by2 = metal_new_buffer(dev, uint64(m * g.n * 4l))
            fill_q8_blob(bw, g.n, g.k)
            fill_x_f16(bx, m * g.k)
            var u_k = upload_u32(dev, uint(g.k))
            var u_n = upload_u32(dev, uint(g.n))
            let gx = uint(m / 128l)
            let gy = uint(g.n / 64l)
            // numerics gate: identical accumulation order -> bit-exact match required
            time_gemm(queue, pso_base, bw, bx, by, u_k, u_n, gx, gy, 8192ul, 1)
            time_gemm(queue, pso_pad, bw, bx, by2, u_k, u_n, gx, gy, 9216ul, 1)
            var pa = unsafe(reinterpret<float?>(metal_buffer_contents(by)))
            var pb = unsafe(reinterpret<float?>(metal_buffer_contents(by2)))
            var bad = 0
            for (i in range64(m * g.n)) {
                if (unsafe(pa[i]) != unsafe(pb[i])) {
                    bad++
                }
            }
            if (bad != 0) {
                panic("p1pad: k={g.k} n={g.n} MISMATCH {bad} elements")
            }
            // tgmem request size ALONE moves the clock (8192/9216/16384 non-monotone), so the
            // race is a 2x2: stride x allocation, padded judged at equal allocation
            let reps = 30
            for (round in range(3)) {
                let b8 = time_gemm(queue, pso_base, bw, bx, by, u_k, u_n, gx, gy, 8192ul, reps)
                let b16 = time_gemm(queue, pso_base, bw, bx, by, u_k, u_n, gx, gy, 16384ul, reps)
                let p9 = time_gemm(queue, pso_pad, bw, bx, by2, u_k, u_n, gx, gy, 9216ul, reps)
                let p16 = time_gemm(queue, pso_pad, bw, bx, by2, u_k, u_n, gx, gy, 16384ul, reps)
                let b8r = time_gemm(queue, pso_base, bw, bx, by, u_k, u_n, gx, gy, 8192ul, reps)
                print("p1pad k={g.k} n={g.n} round={round} base8192={b8:.1f}/{b8r:.1f} base16384={b16:.1f} pad9216={p9:.1f} pad16384={p16:.1f} eq_alloc_ratio={b16 / p16:.4f} (>1 = stride-72 wins at equal alloc)\n")
            }
        }
        print("p1pad: numerics gate PASSED on all shapes (bit-exact)\n")
    } else {
        print("p1pad: no das_metal on this build\n")
    }
}
