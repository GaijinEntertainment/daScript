options gen2
options indenting = 4
options solid_context

// CPU reference renderer: traces the shared pt_core scene on host threads and writes
// pt_reference.png to the current directory. Run under `daslang -jit ...` for the fast-CPU tier;
// the GPU engines in main.das lower the very same functions to SPIR-V.

require math
require stbimage
require daslib/jobque_boost

require pt_core

let {
    WIDTH = 512
    HEIGHT = 512
    SPP = 16
}

[export]
def main {
    var accum : array<float3>
    accum |> resize(WIDTH * HEIGHT)
    let nThreads = get_total_hw_threads()
    let t0 = ref_time_ticks()
    var pbb : array<float3>?
    unsafe {
        pbb = addr(accum)
    }
    with_job_status(nThreads) $(status) {
        let chunk = (HEIGHT + nThreads - 1) / nThreads
        for (c in range(nThreads)) {
            let ymin = c * chunk
            let ymax = min((c + 1) * chunk, HEIGHT)
            new_thread <| @ {
                pt_init_scene()
                for (sample in range(SPP)) {
                    pt_render_rows(sample, WIDTH, HEIGHT, ymin, ymax, *pbb)
                }
                status |> notify_and_release
            }
        }
        status |> join
    }
    let usec = get_time_usec(t0)
    let paths = double(WIDTH * HEIGHT) * double(SPP)
    print("{WIDTH}x{HEIGHT} @ {SPP}spp on {nThreads} threads: {double(usec) / 1000000.0lf} sec, {fmt(":.3f", paths / double(usec))} Mpaths/s\n")
    var pixels : array<uint>
    pixels |> reserve(WIDTH * HEIGHT)
    let inv = 1.0 / float(SPP)
    for (i in range(WIDTH * HEIGHT)) {
        let srgb = pt_tonemap(accum[i] * inv)
        let r = uint(clamp(srgb.x, 0.0, 1.0) * 255.0 + 0.5)
        let g = uint(clamp(srgb.y, 0.0, 1.0) * 255.0 + 0.5)
        let b = uint(clamp(srgb.z, 0.0, 1.0) * 255.0 + 0.5)
        pixels |> push(r | (g << 8u) | (b << 16u) | 0xFF000000)
    }
    unsafe {
        stbi_write_png("pt_reference.png", WIDTH, HEIGHT, 4, addr(pixels[0]), WIDTH * 4)
    }
    print("wrote pt_reference.png\n")
}
