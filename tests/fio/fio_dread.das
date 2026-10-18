options gen2
require dastest/testing_boost
require daslib/fio

// dread is the cache-bypassing positional reader: any offset, any size, any destination, the
// unaligned head and tail of a span re-blocked through the handle's band and an aligned middle
// read straight into the destination. These are mechanics tests over a file dwrite wrote -
// content at odd offsets and sizes, an aligned destination and an offset one, a span past the
// end of the file reading short, a span starting past it reading nothing - not timing.

def private dread_pattern(i : int) : uint8 => uint8((i * 11 + 5) % 256)

def private make_probe_path(tag : string) : string {
    var terr : string
    let tmp = temp_directory(terr)
    return path_join(empty(tmp) ? "." : tmp, "fio_dread_{tag}_{ref_time_ticks()}.bin")
}

//! `total` pattern bytes written through dwrite, the file's path
def private write_pattern_file(t : T?; total : int) : string {
    let fname = make_probe_path("src")
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

//! `bytes` at `off` read into `dst` from `dst_off` on, held byte for byte to the pattern; the count read is returned
def private read_span(t : T?; r : void?; off, bytes : int; var dst : array<uint8>; dst_off : int; tag : string) : int64 {
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

[test]
def test_dread(t : T?) {
    t |> run("spans at odd offsets and sizes, into an aligned and an offset destination, read the file's bytes") @(t : T?) {
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
        // a span inside one sector, a sector-crossing one, a whole-band one, one past the band, the odd shapes at both ends
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
        let none = unsafe(dread_at(r, 0ul, unsafe(addr(dst[0])), 0ul))
        t |> equal(none, 0l, "an empty span reads nothing")
        t |> success(unsafe(dread_close(r)), "the reader closed")
        remove(fname)
    }
    t |> run("a file that does not exist opens no reader") @(t : T?) {
        let r = unsafe(dread_open(make_probe_path("missing"), 0ul))
        t |> success(r == null, "no reader on a missing file")
    }
}
