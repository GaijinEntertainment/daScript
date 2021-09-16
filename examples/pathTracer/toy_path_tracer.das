// options log
// options logCpp
// options log_nodes

module tpt

require path_tracer

require math
require random
require strings
require stbimage
require daslib/jobque_boost
require daslib/tga

let
    USE_JOB_QUE = true
    USE_THREADS = false

[export]
def main
    let width = 320
    let height = 240
    let kFrameCount = 16
    var backbuffer : array<float3>
    resize(backbuffer,width*height)
    print("\ntracing...\n")
    var i = 0
    if USE_JOB_QUE
        with_job_que <|
            var t0 = ref_time_ticks()
            var totalRays = 0
            profile(kFrameCount,"ray-trace frame (with JOB-QUE)") <|
                var pbb : array<float3>?
                var ptr : int?
                unsafe
                    pbb = addr(backbuffer)                  // NOTE: we actually share data here
                    ptr = addr(totalRays)
                with_job_status(height) <| $ ( status )
                    for y in range(height)
                        new_job <| @
                            prepare()
                            var rayCount = 0
                            draw(i, width, height, y, y+1, *pbb, rayCount)
                            *ptr += rayCount
                            status |> notify
                    status |> join
                    i ++
            var dt = get_time_usec(t0)
            var sec = double(dt)/1000000.0lf
            print("{totalRays} in {sec} sec, {format("%.3f",double(totalRays)/double(dt))} mrays/sec\n")
    elif USE_THREADS
        var t0 = ref_time_ticks()
        var totalRays = 0
        profile(kFrameCount,"ray-trace frame (with THREADS)") <|
            var pbb : array<float3>?
            var ptr : int?
            unsafe
                pbb = addr(backbuffer)                  // NOTE: we actually share data here
                ptr = addr(totalRays)
            let hwThreads = get_total_hw_threads()
            with_job_status(hwThreads) <| $ ( status )
                let chunk = (height+hwThreads-1) / hwThreads
                for c in range(hwThreads)
                    let yMin = c * chunk
                    let yMax = min((c + 1)*chunk, height)
                    new_thread <| @
                        prepare()
                        var rayCount = 0
                        draw(i, width, height, yMin, yMax, *pbb, rayCount)
                        *ptr += rayCount
                        status |> notify
                status |> join
                i ++
        var dt = get_time_usec(t0)
        var sec = double(dt)/1000000.0lf
        print("{totalRays} in {sec} sec, {format("%.3f",double(totalRays)/double(dt))} mrays/sec\n")
    else
        prepare()
        profile(kFrameCount,"ray-trace frame") <|
            var rayCount = 0
            draw(i, width, height, 0, height, backbuffer, rayCount)
            i ++

    var pixels : array<uint>
    pixels |> reserve(width*height)
    for y in range(height)
        for x in range(width)
            pixels |> push <| linear_to_SRGB(backbuffer[x + (height-1-y)*width])
    unsafe
        stbi_write_png( "{get_das_root()}/examples/pathTracer/path_tracer.png", width, height, 4, addr(pixels[0]), width*4  )


options debugger
require daslib/debug
