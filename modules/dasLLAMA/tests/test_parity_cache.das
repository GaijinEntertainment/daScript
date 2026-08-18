options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_parity
require daslib/fio

// The parity ref-cache codec (model-free — runs everywhere incl. CI): a saved cache round-trips,
// and a corrupt/truncated cache REJECTS as a whole (load returns false, output left empty) so the
// caller recomputes — a silent-zero parse here would poison every parity truth downstream.

def private probe_path(tag : string) : string {
    var terr : string
    let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
    return path_join(empty(tmp) ? "." : tmp, "dasllama_parity_probe_{tag}_{ref_time_ticks()}.tsv")
}

[test]
def test_parity_cache_roundtrip(t : T?) {
    t |> run("ids + vals round-trip through the ref cache") @(t : T?) {
        let ipath = probe_path("ids")
        var ids <- [1l, 42l, -7l, 1000000007l]
        save_ref_ids(ipath, ids)
        var back : array<int64>
        t |> success(load_ref_ids(ipath, back), "ids cache loads")
        t |> equal(length(back), length(ids))
        for (a, b in ids, back) {
            t |> equal(a, b)
        }
        remove(ipath)
        let vpath = probe_path("vals")
        var vals <- [0.5f, -1.25f, 3.14159274f, 1.0e-8f]
        save_ref_vals(vpath, vals)
        var vback : array<float>
        t |> success(load_ref_vals(vpath, vback), "vals cache loads")
        t |> equal(length(vback), length(vals))
        for (a, b in vals, vback) {
            t |> equal(a, b)
        }
        remove(vpath)
    }
}

[test]
def test_parity_cache_rejects_corruption(t : T?) {
    t |> run("a corrupt cache rejects whole (no silent zeros)") @(t : T?) {
        let ipath = probe_path("corrupt_ids")
        fopen(ipath, "wb") $(f) {
            fwrite(f, "ids 1 2 garbage 4\n")
        }
        var ids : array<int64>
        t |> success(!load_ref_ids(ipath, ids), "corrupt ids cache reports failure")
        t |> success(empty(ids), "corrupt ids cache leaves the output empty")
        remove(ipath)
        let vpath = probe_path("corrupt_vals")
        fopen(vpath, "wb") $(f) {
            fwrite(f, "vals 0.5 1.5 12abc\n")   // trailing-garbage token — to_float would silently read 12
        }
        var vals : array<float>
        t |> success(!load_ref_vals(vpath, vals), "corrupt vals cache reports failure")
        t |> success(empty(vals), "corrupt vals cache leaves the output empty")
        remove(vpath)
    }
}
