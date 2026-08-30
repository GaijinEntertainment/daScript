options gen2
options persistent_heap
options _dasllama_internal = true

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_prefill    // production MetalQ8MulMmTH128 MSL
require llvm/daslib/f16_cvt   // f32_to_f16 - the blob's f16 scale halfwords
require strings

// Phase-2 #16 sizing probe: the w2 (down-projection) shape at Qwen3-8B/512 geometry on the
// PRODUCTION staged-q8 tall kernel - whole-K (256 tgs) vs split-K x2/x4 (independent halves
// on a CONCURRENT encoder, the production dispatch regime). If the split arms do not beat
// whole-K by clearly more than the reduce cost (~43us/site), split-K is refuted at this
// shape and the wave-quantization hypothesis dies with it.

let M = 512
let KDIM = 12288
let NDIM = 4096

def q8_blob(dev; d, kdim : int) {
    let nblk = d * (kdim / 32)
    var buf = metal_new_buffer(dev, uint64(nblk * 34))
    var p = unsafe(reinterpret<uint8?>(metal_buffer_contents(buf)))
    unsafe {
        for (b in range64(int64(nblk))) {
            let dbits = f32_to_f16(0.25)
            p[b * 34l] = uint8(dbits & 0xFFu)
            p[b * 34l + 1l] = uint8(dbits >> 8u)
            for (c in range64(32l)) {
                p[b * 34l + 2l + c] = uint8(int((b * 5l + c * 11l) % 15l) - 7)
            }
        }
    }
    return buf
}

def xh_panel(dev; m, kdim : int) {
    var buf = metal_new_buffer(dev, uint64(m * kdim * 2))
    var p = unsafe(reinterpret<uint16?>(metal_buffer_contents(buf)))
    unsafe {
        for (i in range64(int64(m) * int64(kdim))) {
            p[i] = uint16(f32_to_f16(0.01 * float(i % 37) - 0.15))
        }
    }
    return buf
}

def upload_u32(dev; v : uint) {
    var buf = metal_new_buffer(dev, 4ul)
    var p = unsafe(reinterpret<uint?>(metal_buffer_contents(buf)))
    unsafe(p[0]) = v
    return buf
}

//! one timed CONCURRENT encoder: `parts` independent buffer sets dispatched back-to-back per
//! rep (disjoint buffers overlap; reps serialize on the shared Y hazards). us per rep.
def time_split(dev, queue, pso; parts : int; reps : int; tgmem : uint64) : double {
    let kpart = KDIM / parts
    var bw : array<MetalBuffer?>
    var bx : array<MetalBuffer?>
    var by : array<MetalBuffer?>
    var bk : array<MetalBuffer?>
    var bn : array<MetalBuffer?>
    bw |> reserve(parts)
    bx |> reserve(parts)
    by |> reserve(parts)
    bk |> reserve(parts)
    bn |> reserve(parts)
    for (_i in range(parts)) {
        bw |> push(q8_blob(dev, NDIM, kpart))
        bx |> push(xh_panel(dev, M, kpart))
        by |> push(metal_new_buffer(dev, uint64(M * NDIM * 4)))
        bk |> push(upload_u32(dev, uint(kpart)))
        bn |> push(upload_u32(dev, uint(NDIM)))
    }
    let cb = metal_new_command_buffer(queue)
    let enc = metal_new_compute_encoder_concurrent(cb)
    metal_set_pipeline(enc, pso)
    metal_set_threadgroup_memory_length(enc, tgmem, 0)
    for (_r in range(reps)) {
        for (i in range(parts)) {
            metal_set_buffer(enc, bw[i], 0ul, 0)
            metal_set_buffer(enc, bw[i], 0ul, 1)
            metal_set_buffer(enc, bx[i], 0ul, 2)
            metal_set_buffer(enc, by[i], 0ul, 3)
            metal_set_buffer(enc, bk[i], 0ul, 4)
            metal_set_buffer(enc, bn[i], 0ul, 5)
            metal_dispatch_threadgroups(enc, uint3(uint(M / 128), uint(NDIM / 64), 1u), uint3(128u, 1u, 1u))
        }
    }
    metal_end_encoding(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    let err = metal_command_buffer_error(cb)
    let gpu_ms = (metal_command_buffer_gpu_end_time(cb) - metal_command_buffer_gpu_start_time(cb)) * 1000.0lf
    metal_release(cb)
    for (i in range(parts)) {
        metal_release(bw[i])
        metal_release(bx[i])
        metal_release(by[i])
        metal_release(bk[i])
        metal_release(bn[i])
    }
    if (!empty(err)) {
        print("p2 skw2 parts={parts}: FAILED {err}\n")
        return -1.0lf
    }
    return gpu_ms * 1000.0lf / double(reps)
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_metal_device() $(dev : MetalDevice?) {
            if (dev == null) {
                panic("p2 skw2: no Metal device - nothing to time, and a 0-row run must not read as a clean one")
            }
            var queue = metal_new_command_queue(dev)
            var err : string
            var pso = pipeline_from_source(dev, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_fastmath, err)
            if (pso == null) {
                panic("p2 skw2: PSO compile failed ({err}) - nothing to time, and a 0-row run must not read as a clean one")
            }
            let tgm = MetalQ8MulMmTH128_metal_q8_mulmm_t_msl_tgmem
            let reps = 20
            for (warm in range(2)) {   //! pass 0 warms PSO/pages; pass 1 is the report
                let t1 = time_split(dev, queue, pso, 1, reps, tgm)
                let t2 = time_split(dev, queue, pso, 2, reps, tgm)
                let t4 = time_split(dev, queue, pso, 4, reps, tgm)
                if (warm == 1) {
                    print("p2 skw2 w2-shape m={M} k={KDIM} n={NDIM}: whole={t1}us sk2={t2}us sk4={t4}us  (sk2 net {t2 + 43.0lf}us, sk4 net {t4 + 43.0lf}us incl reduce)\n")
                }
            }
            metal_release(pso)
        }
    } else {
        panic("p2 skw2: no das_metal in this build - nothing to time, and a 0-row run must not read as a clean one")
    }
}
