options gen2
require dastest/testing_boost
require daslib/fio

// fmap releases its mapping when the block panics: the panic unwinds
// past the builtin by longjmp, so the builtin catches it, unmaps and
// raises it again. A live mapping refuses the file's next open for
// write on win32, which left a refused file unwritable.

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
