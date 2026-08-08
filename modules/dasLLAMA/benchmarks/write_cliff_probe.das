options gen2

// Isolated sustained-write probe: fill ONE buffer, write it N times, report the rolling rate.
// Nothing else — no model, no JIT, no tune, no transcode. The point is to watch a consumer SSD's
// SLC cache exhaust as a time series (the "cliff"), instead of inferring it from a conversion
// total. The .dlim writer is the caller that hits this: 75 GB at 275 MB/s where a 2.5 GB image
// writes at ~1 GB/s.
//   bin/daslang modules/dasLLAMA/benchmarks/write_cliff_probe.das
//   PROBE_GB=50 PROBE_PATH=/mnt/nvme/_wcliff.bin bin/daslang .../write_cliff_probe.das

require daslib/fio
require math
require dasllama/dasllama_env

let CHUNK_MB = 16l
let WINDOW_CHUNKS = 32l   // one report line per 512 MB

[export]
def main() {
    let gb = max(g_env_bench.probe_gb, 1l)
    // cwd-relative so the default works on every host; point it at the drive under test
    let path = g_env_bench.probe_path
    let chunk_bytes = CHUNK_MB * 1024l * 1024l
    let chunks = max(gb * 1024l / CHUNK_MB, 1l)
    var buf : array<uint8>
    buf |> resize(chunk_bytes)
    // a varying pattern, not zeros — zero pages can be swallowed by controller compression or
    // sparse-file handling and would read as infinite bandwidth
    for (i in range64(chunk_bytes)) {
        buf[i] = uint8((i * 31l + 17l) & 255l)
    }
    print("write-cliff probe: {gb} GB as {chunks} x {CHUNK_MB} MB -> {path}\n")
    print("      done | window MB/s | avg MB/s\n")
    var t_close = 0l
    var slowest = 0.0lf
    var fastest = 0.0lf
    fopen(path, "wb") $(f) {
        if (f == null) {
            panic("write-cliff probe: cannot open {path} for writing")
        }
        let t_start = ref_time_ticks()
        var t_win = t_start
        var written = 0l
        for (c in range64(chunks)) {
            fwrite(f, buf)
            written += chunk_bytes
            if ((c + 1l) % WINDOW_CHUNKS == 0l) {
                let win_s = double(get_time_nsec(t_win)) / 1.0e9lf
                let tot_s = double(get_time_nsec(t_start)) / 1.0e9lf
                let win_mbs = double(WINDOW_CHUNKS * CHUNK_MB) / win_s
                let avg_mbs = double(written / 1048576l) / tot_s
                slowest = slowest == 0.0lf ? win_mbs : min(slowest, win_mbs)
                fastest = max(fastest, win_mbs)
                print("  {written / 1073741824l:5} GB | {win_mbs:10.0f} | {avg_mbs:8.0f}\n")
                t_win = ref_time_ticks()
            }
        }
        // fflush pushes stdio's buffer to the OS; the REAL flush to media happens in fclose at
        // block exit, and on a big write that stall is part of the cost — so it is timed too
        fflush(f)
        t_close = ref_time_ticks()
    }
    print("close/flush: {get_time_nsec(t_close) / 1000000l} ms\n")
    print("window rate: fastest {fastest:.0f} MB/s, slowest {slowest:.0f} MB/s")
    print(fastest > 0.0lf ? " - cliff {fastest / max(slowest, 1.0lf):.1f}x\n" : "\n")
    delete buf
    if (!remove(path)) {
        print("warning: could not remove {path} - delete it by hand\n")
    }
}
