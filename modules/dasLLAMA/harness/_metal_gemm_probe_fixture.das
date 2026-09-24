options gen2
options indenting = 4

module _metal_gemm_probe_fixture shared public

require ?das_metal metal/das_metal_boost

//! block_q8_0 34B blocks (scale half at +0, 32 quants at +2), scales 1.0 and quants -8..7 -
//! every product is an integer, so any accumulation order is bit-identical
def fill_q8_blob(buf; n, kdim : int64) {
    var p = unsafe(reinterpret<uint8?>(metal_buffer_contents(buf)))
    var seed = 12345u
    for (blk in range64(n * (kdim / 32l))) {
        let base = blk * 34l
        unsafe(p[base]) = uint8(0x00)
        unsafe(p[base + 1l]) = uint8(0x3c)   //! the high byte of half 1.0
        for (i in range64(32l)) {
            seed = seed * 1664525u + 1013904223u
            unsafe(p[base + 2l + i]) = uint8(int8(int(seed >> 16u) % 16 - 8))
        }
    }
}

def time_gemm(queue; pso; bw, bx, by, bk, bn; gx, gy : uint; tgmem : uint64; reps : int) : double {
    var err : string
    var gpu_ms = 0.0lf
    let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc) {
        metal_set_pipeline(enc, pso)
        metal_set_buffer(enc, bw, 0ul, 0)   //! wsh - half view of the blob
        metal_set_buffer(enc, bw, 0ul, 1)   //! wqb - byte view of the SAME blob
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
