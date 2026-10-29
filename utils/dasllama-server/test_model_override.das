options gen2
options persistent_heap

require dastest/testing_boost public
require main                                            // the program root: ModelEntry, apply_model_override, boot_default_index (sibling by bare name)
require daslib/fio                                      // temp_directory / fwrite / remove / path_join

// Model-free unit coverage for the --model-over-roster override (the match arm, the append
// arm, and the two warn-and-degrade arms: missing file, route-name collision) and for the
// boot's default pick past embedding models. Runs fully in CI — no GGUF, no JIT.

def private plant(name : string) : string {
    var terr = ""
    let td = temp_directory(terr)
    let p = path_join(empty(td) ? "/tmp" : td, name)
    fwrite(p, "x")
    return p
}

[test]
def test_model_override(t : T?) {
    t |> run("--model over a roster: match, append, and the warn-and-degrade arms") @(t : T?) {
        let a = plant("dasllama_ovr_a.gguf")
        let b = plant("dasllama_ovr_b.gguf")
        var models <- [ModelEntry(path = a, is_default = true), ModelEntry(path = b)]
        apply_model_override(models, path_join(dir_name(a), "dasllama_ovr_missing.gguf"))
        t |> success(length(models) == 2 && models[0].is_default && !models[1].is_default,
                     "a missing --model leaves the roster untouched")
        apply_model_override(models, to_generic_path(b))
        t |> success(length(models) == 2 && !models[0].is_default && models[1].is_default,
                     "a spelling-variant match flips the default onto the matched entry")
        let c = plant("dasllama_ovr_c.gguf")
        apply_model_override(models, c)
        t |> success(length(models) == 3 && models[2].is_default && !models[0].is_default && !models[1].is_default,
                     "an unlisted --model joins the roster AS the default")
        var collide <- [ModelEntry(path = a, name = "kept", is_default = true),
                        ModelEntry(path = b, name = "dasllama_ovr_c.gguf")]
        apply_model_override(collide, c)
        t |> success(length(collide) == 2 && collide[0].is_default && !collide[1].is_default,
                     "a route-name collision leaves the roster untouched")
        remove(a)
        remove(b)
        remove(c)
        delete models
        delete collide
    }
}

[test]
def test_boot_default_index(t : T?) {
    t |> run("the default slot is a chat entry: an embedding model never takes the mark") @(t : T?) {
        var inscope models <- [ModelEntry(path = "chat_a.gguf"), ModelEntry(path = "emb.gguf", is_default = true), ModelEntry(path = "chat_b.gguf")]
        t |> equal(boot_default_index(models, [false, false, false]), 1, "a marked chat entry is the default")
        t |> equal(boot_default_index(models, [false, true, false]), 0, "a marked embedding model hands the default to the first chat entry")
        t |> equal(boot_default_index(models, [true, true, false]), 2, "the first chat entry, past every embedding model")
        t |> equal(boot_default_index(models, [true, true, true]), -1, "an all-embedding roster has no default slot")
        var inscope unmarked <- [ModelEntry(path = "emb.gguf"), ModelEntry(path = "chat.gguf")]
        t |> equal(boot_default_index(unmarked, [true, false]), 1, "with no mark, the first chat entry")
    }
}
