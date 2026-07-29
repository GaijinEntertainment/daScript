options gen2
require dastest/testing_boost
require daslib/fio

// dwrite is the cache-bypassing sequential appender (bounce bands re-block any caller size to
// the sector alignment NO_BUFFERING-class paths demand; full bands flush on a writer thread).
// These are mechanics tests — round-trip content, the append-only + close-truncate contract,
// the band/commit producer path, stat counters — not timing.

def private dwrite_pattern(i : int) : uint8 => uint8((i * 7 + 13) % 256)

def private make_probe_path(tag : string) : string {
    var terr : string
    let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
    return path_join(empty(tmp) ? "." : tmp, "fio_dwrite_{tag}_{ref_time_ticks()}.bin")
}

def private verify_file_bytes(t : T?; fname : string; total : int; tag : string) {
    var checked = false
    fopen(fname, "rb") $(f) {
        if (f == null) {
            return
        }
        fmap(f) $(var bytes : array<uint8>#) {
            var intact = long_length(bytes) == int64(total)
            t |> success(intact, "{tag}: file is exactly the appended size")
            if (intact) {
                for (i in range(total)) {
                    if (bytes[i] != dwrite_pattern(i)) {
                        intact = false
                        break
                    }
                }
            }
            t |> success(intact, "{tag}: content round-trips byte-for-byte")
            checked = true
        }
    }
    t |> success(checked, "{tag}: mapped the written file back")
}

[test]
def test_dwrite(t : T?) {
    t |> run("odd-size appends round-trip and close truncates the preallocation") @(t : T?) {
        let fname = make_probe_path("odd")
        // preallocate far more than we append — close must cut the file back
        var w = unsafe(dwrite_open(fname, 1048576ul, 8192ul))
        t |> success(w != null, "writer opened")
        return if (w == null)
        // odd sizes on both sides of the band: sub-sector, near-band, multi-band
        var sizes <- [1, 3, 4095, 8192, 65537, 12345, 100000]
        var payload : array<uint8>
        var total = 0
        var ok = true
        for (sz in sizes) {
            payload |> resize(sz)
            for (i in range(sz)) {
                payload[i] = dwrite_pattern(total + i)
            }
            ok = ok && unsafe(dwrite_append(w, unsafe(addr<void?>(payload[0])), uint64(sz)))
            total += sz
        }
        t |> success(ok, "every append accepted")
        // stat accounting: the direct-vs-bounce split is platform-dependent (win32 demands
        // sector-aligned sources, the POSIX arm takes any band-sized run direct) — the portable
        // contract is conservation, plus the sub-sector pieces must stage through the band
        t |> equal(uint64(total), unsafe(dwrite_stat(w, 2)) + unsafe(dwrite_stat(w, 3)), "direct + bounce == appended bytes")
        t |> success(unsafe(dwrite_stat(w, 3)) > 0ul, "sub-sector pieces staged through the band")
        t |> success(unsafe(dwrite_close(w)), "close flushed the tail and truncated")
        verify_file_bytes(t, fname, total, "odd-size appends")
        delete payload
        delete sizes
        remove(fname)
    }
    t |> run("the band/commit producer path crosses band boundaries intact") @(t : T?) {
        let fname = make_probe_path("band")
        var w = unsafe(dwrite_open(fname, 0ul, 8192ul))
        t |> success(w != null, "writer opened")
        return if (w == null)
        // build bytes straight into the writer's own staging band, committing uneven runs so
        // several commits land exactly on a band boundary (the async flush + swap point)
        var total = 0
        var ok = true
        while (total < 50000) {
            var avail = 0ul
            var band = unsafe(dwrite_band(w, unsafe(addr(avail))))
            if (band == null || avail == 0ul) {
                ok = false
                break
            }
            var take = 3000
            if (uint64(take) > avail) {
                take = int(avail)   // top the band off exactly — forces the boundary flush
            }
            var bp = unsafe(reinterpret<uint8?>(band))
            for (i in range(take)) {
                unsafe {
                    bp[i] = dwrite_pattern(total + i)
                }
            }
            ok = ok && unsafe(dwrite_commit(w, uint64(take)))
            total += take
        }
        t |> success(ok, "every band commit accepted")
        t |> success(unsafe(dwrite_close(w)), "close flushed the tail")
        verify_file_bytes(t, fname, total, "band/commit")
        remove(fname)
    }
    t |> run("append and band/commit interleave in order") @(t : T?) {
        let fname = make_probe_path("mixed")
        var w = unsafe(dwrite_open(fname, 0ul, 8192ul))
        t |> success(w != null, "writer opened")
        return if (w == null)
        var payload : array<uint8>
        var total = 0
        var ok = true
        for (round in range(6)) {
            let sz = 5000 + round * 777
            payload |> resize(sz)
            for (i in range(sz)) {
                payload[i] = dwrite_pattern(total + i)
            }
            ok = ok && unsafe(dwrite_append(w, unsafe(addr<void?>(payload[0])), uint64(sz)))
            total += sz
            var avail = 0ul
            var band = unsafe(dwrite_band(w, unsafe(addr(avail))))
            if (band == null || avail < 100ul) {
                ok = false
                break
            }
            var bp = unsafe(reinterpret<uint8?>(band))
            for (i in range(100)) {
                unsafe {
                    bp[i] = dwrite_pattern(total + i)
                }
            }
            ok = ok && unsafe(dwrite_commit(w, 100ul))
            total += 100
        }
        t |> success(ok, "interleaved appends and commits accepted")
        t |> success(unsafe(dwrite_close(w)), "close flushed the tail")
        verify_file_bytes(t, fname, total, "interleaved")
        delete payload
        remove(fname)
    }
    t |> run("degenerate opens decline instead of crashing") @(t : T?) {
        // a directory that cannot exist — open returns null, no throw
        let w = unsafe(dwrite_open("Z:/no/such/dir/ever/fio_dwrite_probe.bin", 0ul, 0ul))
        t |> success(w == null, "impossible path declines with null")
    }
}
