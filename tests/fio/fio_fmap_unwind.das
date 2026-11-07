options gen2
require dastest/testing_boost
require daslib/fio

// fmap and fopen's block form release what they hold when the block
// panics: the panic unwinds past the builtin by longjmp, so the
// builtin catches it, unmaps or closes, and raises it again. A live
// mapping refuses the file's next open for write on win32, which left
// a refused file unwritable; an unclosed file never flushes its
// buffer.

[test]
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

[test]
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
