options gen2
options _function_length = 0   // test bodies enumerate the fixture arms

require dastest/testing_boost
require daslib/fio

// Regression: file IO past 2 GiB. The pre-fix rail computed byte counts as int32
// `length * sizeof`, which wraps negative past INT_MAX; the C runtime sign-extends that to
// a huge size_t and streams out-of-bounds heap garbage into the file (or back over the heap
// on read) with no diagnostic. Now: long_fwrite/long_fread carry int64 counts end to end,
// the int32 fwrite/fread PANIC past INT_MAX, and ftell/fseek use the 64-bit C forms (plain
// ftell/fseek take `long` = 32-bit on Windows).
//
// Costs ~2.2 GB of RAM and of disk for a few seconds, so it only runs when DAS_BIG_IO=1 —
// the dasLLAMA image suite exercises the same rail end-to-end on real >2 GiB weight planes.

let BIG = int64(0x80000000) + 65536l   // just past the int32 wrap
let PROBE_STEP = int64(0x2000000)      // spot-check every 32 MiB — full compare would double the cost

// portable probe path: temp_directory + path_join (cwd as a last resort — never a bare "/name")
def private big_io_path(name : string) : string {
    var terr : string
    let tmp = temp_directory(terr)
    return path_join(empty(tmp) ? "." : tmp, name)
}

[test(description = "long_fwrite, long_fread and fseek work past 2 GiB, while the int32 fwrite and fread panic there", timeout = 1200)]
def test_big_write_read(t : T?) {
    if (get_env_variable("DAS_BIG_IO") != "1") {
        t |> skip("big-IO regression needs DAS_BIG_IO=1 (2.2 GB RAM + disk)")
        return
    }
    t |> run("long_fwrite/long_fread past 2 GiB") @@(t : T?) {
        let path = big_io_path("das_big_io_test.bin")
        var buf : array<uint8>
        buf |> resize(BIG)
        for (k in range64(0l, BIG / PROBE_STEP)) {
            buf[k * PROBE_STEP] = uint8(k + 1l)
        }
        buf[BIG - 1l] = uint8(0xAB)
        fopen(path, "wb") $(f) {
            t |> success(f != null)
            t |> equal(f |> long_fwrite(buf), BIG)
        }
        t |> equal(int64(stat(path).size), BIG)   // pre-fix: garbage-padded file, wrong size
        for (k in range64(0l, BIG / PROBE_STEP)) {
            buf[k * PROBE_STEP] = uint8(0)
        }
        buf[BIG - 1l] = uint8(0)
        fopen(path, "rb") $(f) {
            t |> success(f != null)
            t |> equal(f |> long_fread(buf), BIG)
        }
        for (k in range64(0l, BIG / PROBE_STEP)) {
            t |> equal(buf[k * PROBE_STEP], uint8(k + 1l))
        }
        t |> equal(buf[BIG - 1l], uint8(0xAB))
        delete buf
        t |> success(remove(path))   // a stuck multi-GiB temp file poisons later runs
    }
    t |> run("int32 fwrite/fread panic past 2 GiB") @@(t : T?) {
        let path = big_io_path("das_big_io_panic.bin")
        var buf : array<uint8>
        buf |> resize(BIG)
        var wrote_panicked = false
        fopen(path, "wb") $(f) {
            t |> success(f != null)
            try {
                f |> fwrite(buf)
            } recover {
                wrote_panicked = true
            }
        }
        t |> success(wrote_panicked)
        var read_panicked = false
        fopen(path, "rb") $(f) {
            t |> success(f != null)
            try {
                f |> fread(buf)
            } recover {
                read_panicked = true
            }
        }
        t |> success(read_panicked)
        delete buf
        t |> success(remove(path))   // a stuck multi-GiB temp file poisons later runs
    }
    t |> run("fseek/ftell past 2 GiB") @@(t : T?) {
        let path = big_io_path("das_big_io_seek.bin")
        let pos = int64(0xC0000000)   // 3 GiB — sparse on every filesystem we test on
        fopen(path, "wb") $(f) {
            t |> success(f != null)
            fseek(f, pos, seek_set)
            f |> fwrite(uint8(7))
            t |> equal(ftell(f), pos + 1l)   // pre-fix on Windows: 32-bit ftell truncates
        }
        t |> equal(int64(stat(path).size), pos + 1l)
        t |> success(remove(path))   // a stuck multi-GiB temp file poisons later runs
    }
}

// dread is the cache-bypassing positional reader: any offset, any
// size, any destination, the unaligned head and tail of a span
// re-blocked through the handle's band and an aligned middle read
// straight into the destination. These are mechanics tests over a
// file dwrite wrote - content at odd offsets and sizes, an aligned
// destination and an offset one, a span past the end of the file
// reading short, a span starting past it reading nothing; not timing.

def private dread_pattern(i : int) : uint8 => uint8((i * 11 + 5) % 256)

def private dread_probe_path(tag : string) : string =>
    path_join(test_temp_dir(), "fio_dread_{tag}.bin")

//! `total` pattern bytes written through dwrite, the file's path
def private write_pattern_file(t : T?; total : int) : string {
    let fname = dread_probe_path("src")
    var w = unsafe(dwrite_open(fname, uint64(total), 65536ul))
    t |> success(w != null, "the source file opened for writing")
    return "" if (w == null)
    var payload : array<uint8>
    payload |> resize(total)
    for (i in range(total)) {
        payload[i] = dread_pattern(i)
    }
    let ok = unsafe(dwrite_append(w, unsafe(addr(payload[0])), uint64(total)))
    t |> success(ok && unsafe(dwrite_close(w)), "the source file's {total} bytes landed")
    return fname
}

//! `bytes` at `off` read into `dst` from `dst_off` on, held byte for
//! byte to the pattern; the count read is returned
def private read_span(t : T?; r : void?; off, bytes : int; var dst : array<uint8>;
                      dst_off : int; tag : string) : int64 {
    for (i in range(bytes)) {
        dst[dst_off + i] = uint8(0xEE)
    }
    let got = unsafe(dread_at(r, uint64(off), unsafe(addr(dst[dst_off])), uint64(bytes)))
    t |> equal(got, int64(bytes), "{tag}: {bytes} bytes at {off} read whole")
    var intact = got == int64(bytes)
    if (intact) {
        for (i in range(bytes)) {
            if (dst[dst_off + i] != dread_pattern(off + i)) {
                intact = false
                break
            }
        }
    }
    t |> success(intact, "{tag}: the bytes are the file's at {off}")
    return got
}

[test(description = "dread reads spans at odd offsets and sizes into aligned and offset destinations, and a missing file opens no reader", timeout = 1200)]
def test_dread(t : T?) {
    t |> run("spans at odd offsets and sizes, into aligned and offset destinations") @(t : T?) {
        let total = 3 * 1048576 + 777
        let fname = write_pattern_file(t, total)
        return if (empty(fname))
        var r = unsafe(dread_open(fname, 65536ul))
        t |> success(r != null, "the reader opened")
        if (r == null) {
            remove(fname)
            return
        }
        var dst : array<uint8>
        dst |> resize(total + 4096)
        read_span(t, r, 5, 100, dst, 0, "inside a sector")
        read_span(t, r, 4090, 20, dst, 0, "crossing a sector")
        read_span(t, r, 4096, 4096, dst, 0, "one sector aligned at both ends")
        read_span(t, r, 4096, 65536, dst, 0, "one band aligned at both ends")
        read_span(t, r, 12345, 200000, dst, 0, "two bands odd at both ends")
        read_span(t, r, 0, total, dst, 0, "the whole file")
        read_span(t, r, 8192, 70000, dst, 3, "an offset destination, aligned file span")
        read_span(t, r, 8195, 70000, dst, 7, "an offset destination, odd file span")
        read_span(t, r, 1048576, 1048576 + 1, dst, 1, "a megabyte and one, offset destination")
        let tail = unsafe(dread_at(r, uint64(total - 100), unsafe(addr(dst[0])), 1000ul))
        t |> equal(tail, 100l, "a span past the end of the file reads short, the bytes to the end")
        var intact = tail == 100l
        if (intact) {
            for (i in range(100)) {
                if (dst[i] != dread_pattern(total - 100 + i)) {
                    intact = false
                }
            }
        }
        t |> success(intact, "... and those bytes are the file's last")
        let past = unsafe(dread_at(r, uint64(total + 4096), unsafe(addr(dst[0])), 4096ul))
        t |> equal(past, 0l, "a span starting past the end reads nothing")
        let edge_off = total - 777 - 4096
        let edge = unsafe(dread_at(r, uint64(edge_off), unsafe(addr(dst[0])), uint64(3 * 4096)))
        t |> equal(edge, int64(4096 + 777),
            "an aligned start whose span runs a sector and more past the end reads to the end")
        var edge_intact = edge == int64(4096 + 777)
        if (edge_intact) {
            for (i in range(4096 + 777)) {
                if (dst[i] != dread_pattern(edge_off + i)) {
                    edge_intact = false
                }
            }
        }
        t |> success(edge_intact, "... and those bytes are the file's")
        let none = unsafe(dread_at(r, 0ul, unsafe(addr(dst[0])), 0ul))
        t |> equal(none, 0l, "an empty span reads nothing")
        t |> success(unsafe(dread_close(r)), "the reader closed")
        remove(fname)
    }
    t |> run("a page-aligned destination: the middle straight, the ends via the band") @(ct : T?) {
        let total = 2 * 1048576 + 333
        let fname = write_pattern_file(ct, total)
        return if (empty(fname))
        var r = unsafe(dread_open(fname, 65536ul))
        ct |> success(r != null, "the reader opened")
        if (r == null) {
            remove(fname)
            return
        }
        var dst : array<uint8>
        dst |> resize(total + 8192)
        let base = unsafe(reinterpret<uint64>(unsafe(addr(dst[0]))))
        let skew = int((4096ul - (base & 4095ul)) & 4095ul)   //! the first page-aligned byte
        read_span(ct, r, 4096, 131072, dst, skew, "aligned destination, aligned file span")
        read_span(ct, r, 4099, 131072, dst, skew, "aligned destination, odd file offset")
        read_span(ct, r, 4096, 131075, dst, skew, "aligned destination, odd file length")
        read_span(ct, r, 7, total - 7, dst, skew, "aligned destination, the file from byte seven")
        ct |> success(unsafe(dread_close(r)), "the reader closed")
        remove(fname)
    }
    t |> run("a file that does not exist opens no reader") @(t : T?) {
        let r = unsafe(dread_open(dread_probe_path("missing"), 0ul))
        t |> success(r == null, "no reader on a missing file")
    }
}

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

[test(description = "dwrite round-trips odd-size appends and band/commit writes, closes with the preallocation truncated, and declines degenerate opens", timeout = 1200)]
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
                unsafe(bp[i]) = dwrite_pattern(total + i)
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
                unsafe(bp[i]) = dwrite_pattern(total + i)
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
    t |> run("a preallocation no volume can hold declines and leaves no file (Apple)") @(t : T?) {
        // the Apple arm RESERVES total_bytes up front (F_PREALLOCATE + F_ALLOCATEALL), so an
        // impossible reservation fails the open — and the open unlinks the file it just created
        // rather than leaving an empty stub behind. The POSIX and Windows arms take the
        // preallocation as a hint and keep going, so only this arm has a decline to assert.
        if (get_platform_name() != "darwin") {
            t |> success(true, "skipped — only the Apple arm reserves the preallocation up front")
            return
        }
        let fname = make_probe_path("prealloc")
        let w = unsafe(dwrite_open(fname, 1ul << 50ul, 0ul))   // 1 PiB
        t |> success(w == null, "a preallocation past the volume declines with null")
        t |> success(!stat(fname).is_valid, "and the declined open leaves no file at the path")
        if (w != null) {
            unsafe(dwrite_close(w))
            remove(fname)
        }
    }
    t |> run("a single append past 2 GiB survives (DASTEST_BIG_IO=1)") @(t : T?) {
        // macOS write(2) REJECTS nbyte > INT_MAX with EINVAL (Linux merely truncates), so an
        // unchunked raw writer dies exactly on a big single append — an 11 GB image plane read
        // as "disk full". A das array caps at int elements (why no unit ever hit this), so the
        // >2 GiB source comes from an fmap of a staged file. ~4.2 GB of disk traffic — opt-in;
        // the M-box bring-ups run it.
        if (empty(get_env_variable("DASTEST_BIG_IO"))) {
            t |> success(true, "skipped — set DASTEST_BIG_IO=1 (writes ~4.2 GB)")
            return
        }
        let half = 1073743872l                  // 1 GiB + 2048: two of these cross INT_MAX
        let big = 2l * half
        let src_name = make_probe_path("bigsrc")
        var chunk : array<uint8>
        chunk |> resize(int(half))
        for (i in range(4096)) {
            chunk[i] = dwrite_pattern(i)
            chunk[int(half) - 1 - i] = dwrite_pattern(i)
        }
        var sw = unsafe(dwrite_open(src_name, uint64(big), 8ul * 1024ul * 1024ul))
        t |> success(sw != null, "stage writer opened")
        if (sw == null) {
            delete chunk
            return
        }
        var staged = unsafe(dwrite_append(sw, unsafe(addr<void?>(chunk[0])), uint64(half)))
        staged = staged && unsafe(dwrite_append(sw, unsafe(addr<void?>(chunk[0])), uint64(half)))
        staged = staged && unsafe(dwrite_close(sw))
        delete chunk
        t |> success(staged, "staged the 2.1 GiB source in sub-2GiB appends")
        if (!staged) {
            remove(src_name)
            return
        }
        // the actual regression: ONE append of the whole >2 GiB mapping
        let dst_name = make_probe_path("bigdst")
        var appended = false
        var closed = false
        fopen(src_name, "rb") $(f) {
            if (f == null) {
                return
            }
            fmap(f) $(var bytes : array<uint8>#) {
                var w = unsafe(dwrite_open(dst_name, uint64(big), 8ul * 1024ul * 1024ul))
                if (w == null) {
                    return
                }
                appended = unsafe(dwrite_append(w, unsafe(addr<void?>(bytes[0])), uint64(big)))
                closed = unsafe(dwrite_close(w))
            }
        }
        t |> success(appended, ">2 GiB single append accepted")
        t |> success(closed, "close flushed and truncated")
        t |> equal(int64(stat(dst_name).size), big)
        var seams_ok = true
        fopen(dst_name, "rb") $(f) {
            if (f == null) {
                seams_ok = false
                return
            }
            fmap(f) $(var bytes : array<uint8>#) {
                // the two halves are identical staged chunks: check both copies' seams
                for (i in range(4096)) {
                    seams_ok = seams_ok && bytes[i] == dwrite_pattern(i)
                    unsafe {
                        let bp = addr(bytes[0])
                        seams_ok = seams_ok && bp[big - 1l - int64(i)] == dwrite_pattern(i)
                        seams_ok = seams_ok && bp[half - 1l - int64(i)] == dwrite_pattern(i)
                        seams_ok = seams_ok && bp[half + int64(i)] == dwrite_pattern(i)
                    }
                }
            }
        }
        t |> success(seams_ok, "all four half-seams intact after the big append")
        remove(src_name)
        remove(dst_name)
    }
}

// prefetch_map is ADVISORY readahead — these are contract tests (acceptance + intact reads +
// degenerate-argument declines), not timing: the OS is free to prefetch lazily or not at all.

[test(description = "prefetch accepts a real mapping without disturbing its reads and declines degenerate arguments")]
def test_prefetch_map(t : T?) {
    t |> run("prefetch over a real mapping is accepted and reads stay intact") @(t : T?) {
        var terr : string
        let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
        let fname = path_join(empty(tmp) ? "." : tmp, "fio_prefetch_{ref_time_ticks()}.bin")
        let n = 65536
        var payload : array<uint8>
        payload |> resize(n)
        for (i in range(n)) {
            payload[i] = uint8(i % 251)
        }
        var wrote = false
        fopen(fname, "wb") $(f) {
            if (f != null) {
                fwrite(f, payload)
                wrote = true
            }
        }
        t |> success(wrote, "wrote the probe file")
        var mapped = false
        fopen(fname, "rb") $(f) {
            if (f == null) {
                return
            }
            fmap(f) $(var bytes : array<uint8>#) {
                mapped = true
                var armed = false
                unsafe {
                    armed = prefetch_map(addr<void?>(bytes[0]), uint64(long_length(bytes)))
                }
                t |> success(armed, "the OS accepted the advisory prefetch")
                var intact = length(bytes) == n
                for (i in range(n)) {
                    if (bytes[i] != uint8(i % 251)) {
                        intact = false
                        break
                    }
                }
                t |> success(intact, "mapped content intact after prefetch")
            }
        }
        t |> success(mapped, "mapped the probe file")
        delete payload
        remove(fname)
    }
    t |> run("degenerate arguments decline instead of crashing") @(t : T?) {
        t |> success(!unsafe(prefetch_map(null, 0ul)), "null base + zero bytes declines")
    }
}

// fmap_open_rw is the writable SHARED twin of fmap_open (PAGE_READWRITE / PROT_WRITE): writes
// through the mapping go back to the file via the page cache. Contract tests — write-through
// persistence, read parity with the read-only twin, decline shapes.

def private rw_pattern(i : int) : uint8 => uint8((i * 11 + 5) % 256)

[test(description = "Writes through a read-write mapping persist to the file, and missing or empty files map as null")]
def test_fmap_rw(t : T?) {
    t |> run("writes through the mapping persist to the file") @(t : T?) {
        var terr : string
        let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
        let fname = path_join(empty(tmp) ? "." : tmp, "fio_fmap_rw_{ref_time_ticks()}.bin")
        let n = 65536
        var payload : array<uint8>
        payload |> resize(n)
        for (i in range(n)) {
            payload[i] = rw_pattern(i)
        }
        var wrote = false
        fopen(fname, "wb") $(f) {
            if (f != null) {
                fwrite(f, payload)
                wrote = true
            }
        }
        t |> success(wrote, "wrote the probe file")
        // map writable: verify content, then flip a sparse set of bytes through the mapping
        var msize = 0ul
        var base : void?
        unsafe {
            base = fmap_open_rw(fname, addr(msize))
        }
        t |> success(base != null, "fmap_open_rw maps")
        return if (base == null)
        t |> equal(uint64(n), msize, "mapped size is the file size")
        var bp = unsafe(reinterpret<uint8?>(base))
        var intact = true
        for (i in range(n)) {
            if (unsafe(bp[i]) != rw_pattern(i)) {
                intact = false
                break
            }
        }
        t |> success(intact, "mapped content matches what was written")
        var at = 0
        while (at < n) {
            unsafe(bp[at]) = uint8(255 - int(rw_pattern(at)))
            at += 4099
        }
        unsafe(fmap_close(base, msize))
        // reopen READ-ONLY: the flips must have landed in the file itself
        var persisted = false
        fopen(fname, "rb") $(f) {
            if (f == null) {
                return
            }
            fmap(f) $(var bytes : array<uint8>#) {
                var ok = long_length(bytes) == int64(n)
                if (ok) {
                    for (i in range(n)) {
                        let want = i % 4099 == 0 ? uint8(255 - int(rw_pattern(i))) : rw_pattern(i)
                        if (bytes[i] != want) {
                            ok = false
                            break
                        }
                    }
                }
                persisted = ok
            }
        }
        t |> success(persisted, "writes persisted to the file after unmap")
        delete payload
        remove(fname)
    }
    t |> run("decline shapes: missing and empty files map as null") @(t : T?) {
        var msize = 1ul
        var base : void?
        unsafe {
            base = fmap_open_rw("no_such_fio_fmap_rw_probe.bin", addr(msize))
        }
        t |> success(base == null && msize == 0ul, "missing file declines with null, size 0")
        var terr : string
        let tmp = temp_directory(terr)
        let fname = path_join(empty(tmp) ? "." : tmp, "fio_fmap_rw_empty_{ref_time_ticks()}.bin")
        fopen(fname, "wb") $(f) {
            feint("{f != null}")   // an empty file: created, nothing written
        }
        unsafe {
            base = fmap_open_rw(fname, addr(msize))
        }
        t |> success(base == null, "empty file declines with null (mmap(0) is invalid)")
        remove(fname)
    }
}

// fmap and fopen's block form release what they hold when the block
// panics: the panic unwinds past the builtin by longjmp, so the
// builtin catches it, unmaps or closes, and raises it again. A live
// mapping refuses the file's next open for write on win32, which left
// a refused file unwritable; an unclosed file never flushes its
// buffer.

[test(description = "A panic inside fmap's block reaches the caller and leaves the file writable")]
def test_fmap_unwinds_on_panic(t : T?) {
    t |> run("a panic inside fmap's block reaches the caller, the file stays writable") @(t : T?) {
        let fname = path_join(test_temp_dir(), "fio_fmap_unwind.bin")
        var payload : array<uint8>
        payload |> resize(4096)
        for (i in range(4096)) {
            payload[i] = uint8(i % 251)
        }
        var wrote = false
        fopen(fname, "wb") $(f) {
            if (f != null) {
                fwrite(f, payload)
                wrote = true
            }
        }
        t |> success(wrote, "wrote the probe file")
        let f = fopen(fname, "rb")
        t |> success(f != null, "the probe file opens for reading")
        return if (f == null)
        var entered = false
        var raised = false
        try {
            fmap(f) $(var bytes : array<uint8>#) {
                entered = length(bytes) == 4096
                panic("inside the map: {length(bytes)} bytes")
            }
        } recover {
            raised = true
        }
        fclose(f)
        t |> success(entered, "the block ran over the mapped bytes")
        t |> success(raised, "the block's panic reaches the caller")
        var reopened = false
        fopen(fname, "wb") $(w) {
            if (w != null) {
                fwrite(w, payload)
                reopened = true
            }
        }
        t |> success(reopened, "the file opens for write after the panic: the mapping was released")
    }
}

[test(description = "A panic inside fopen's block reaches the caller and the file is closed")]
def test_fopen_block_closes_on_panic(t : T?) {
    t |> run("a panic inside fopen's block reaches the caller, the file is closed") @(t : T?) {
        let fname = path_join(test_temp_dir(), "fio_fopen_unwind.bin")
        var payload : array<uint8>
        payload |> resize(4096)
        var entered = false
        var raised = false
        try {
            fopen(fname, "wb") $(f) {
                entered = f != null
                if (f != null) {
                    fwrite(f, payload)
                }
                panic("inside the block")
            }
        } recover {
            raised = true
        }
        t |> success(entered, "the block ran over an open file")
        t |> success(raised, "the block's panic reaches the caller")
        t |> equal(4096ul, stat(fname).size,
            "the bytes written before the panic reached the disk: the file was closed")
    }
}
