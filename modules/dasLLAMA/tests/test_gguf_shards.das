options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gguf
require daslib/fio

// gguf_shard_paths: GGUF split-name expansion ("<prefix>-NNNNN-of-NNNNN.gguf"), the
// non-split passthrough arms, the missing_ok presence gate model_available rides (incomplete
// split yields [] instead of the loader's panic), and the split NextN head sidecar that rides the
// same walk as its LAST shard (mtp-<trunk> sibling, or DASLLAMA_MTP_HEAD). Name/filesystem only.

[test]
def test_shard_name_passthrough(t : T?) {
    t |> run("non-split shapes yield just [path]") @(t : T?) {
        for (p in ["model.gguf", "m-1-of-3.gguf", "m-00000-of-00003.gguf",
                   "m-00004-of-00003.gguf", "m-abcde-of-00003.gguf", "m-00001-of-00003.bin"]) {
            var shards <- gguf_shard_paths(p)
            t |> equal(length(shards), 1, "single path for '{p}'")
            t |> equal(shards[0], p, "path unchanged for '{p}'")
            delete shards
        }
    }
}

[test]
def test_shard_split_expansion(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let names <- [for (i in range(1, 4)); "dasllama_shard_toy-0000{i}-of-00003.gguf"]
    let paths <- [for (n in names); path_join(dir, n)]
    for (p in paths) {
        t |> success(fwrite(p, "x"), "shard written: {base_name(p)}")
    }
    // complete split: expands from ANY shard index, normalized and in order
    var shards <- gguf_shard_paths(paths[1])
    t |> equal(length(shards), 3, "3 shards from the shard-2 path")
    for (i in range(3)) {
        t |> equal(base_name(shards[i]), names[i], "shard {i + 1} in order")
        t |> success(fexist(shards[i]), "shard {i + 1} exists")
    }
    delete shards
    // incomplete split (partial download): the presence-gate arm yields [] instead of a panic
    remove(paths[2])
    var missing <- gguf_shard_paths(paths[0], [missing_ok = true])
    t |> equal(length(missing), 0, "missing sibling with missing_ok yields empty")
    delete missing
    remove(paths[0])
    remove(paths[1])
}

[test]
def test_shard_mtp_head_sidecar(t : T?) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let trunk = path_join(dir, "dasllama_sidecar_toy-Q4_K_M.gguf")
    let head = path_join(dir, "mtp-dasllama_sidecar_toy-Q4_K_M.gguf")
    let other = path_join(dir, "mtp-dasllama_sidecar_toy-Q8_0.gguf")
    t |> success(fwrite(trunk, "x") && fwrite(head, "x") && fwrite(other, "x"), "toy trunk + two heads written")
    set_mtp_head_path("")   // pin the override empty: the box env must not leak into the arms
    t |> run("adjacency: the mtp-<trunk basename> sibling rides as the last shard") @(t : T?) {
        var sh <- gguf_shard_paths(trunk)
        t |> equal(length(sh), 2, "trunk + head")
        t |> equal(sh[0], trunk, "trunk first")
        t |> equal(base_name(sh[1]), base_name(head), "the same-named head last")
        delete sh
    }
    t |> run("override: DASLLAMA_MTP_HEAD names the head regardless of its name") @(t : T?) {
        set_mtp_head_path(other)
        var sh <- gguf_shard_paths(trunk)
        t |> equal(length(sh), 2, "trunk + override head")
        t |> equal(base_name(sh[1]), base_name(other), "the override head, not the sibling")
        set_mtp_head_path(path_join(dir, "does-not-exist.gguf"))
        var none <- gguf_shard_paths(trunk)
        t |> equal(length(none), 1, "a missing override attaches nothing (the sibling is NOT a fallback)")
        set_mtp_head_path("")
        delete sh
        delete none
    }
    t |> run("a head file loaded on its own never recurses onto itself") @(t : T?) {
        var sh <- gguf_shard_paths(head)
        t |> equal(length(sh), 1, "just the head")
        delete sh
    }
    t |> run("no sibling, no override: the plain single path") @(t : T?) {
        remove(head)
        var sh <- gguf_shard_paths(trunk)
        t |> equal(length(sh), 1, "trunk alone")
        delete sh
    }
    clear_mtp_head_path()
    remove(trunk)
    remove(other)
}
