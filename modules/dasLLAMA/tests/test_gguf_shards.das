options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_gguf
require daslib/fio

// gguf_shard_paths: llama.cpp split-name expansion ("<prefix>-NNNNN-of-NNNNN.gguf"), the
// non-split passthrough arms, and the missing_ok presence gate model_available rides
// (incomplete split yields [] instead of the loader's panic). Name/filesystem only — no models.

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
