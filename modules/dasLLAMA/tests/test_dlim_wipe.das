options gen2
options _dasllama_internal = true
require dastest/testing_boost public
require daslib/fio
require dasllama/dasllama_image

// dlim_wipe is verdict- and lane-blind BY DESIGN (the batch-lifecycle owner carve-out,
// ARCHITECTURE.md 2.1) — but its reach must be exact: a model path wipes only that model's
// images, a directory path wipes every image in it, and non-.dlim files are never touched.

def private plant(root, name : string) {
    fopen(path_join(root, name), "wb") $(f) {
        if (f != null) {
            f |> fwrite("not a real image")
        }
    }
}

[test]
def test_dlim_wipe(t : T?) {
    var terr : string
    let root = create_temp_directory("dasllama_wipe_test", terr)
    if (!empty(terr)) {
        t |> success(false, "cannot create a temp directory: {terr}")
        return
    }
    plant(root, "a.gguf")
    plant(root, "a.gguf.0xdead.dlim")
    plant(root, "a.gguf.0xbeef.dlim")
    plant(root, "a.gguf2.0xdead.dlim")   // shared-prefix decoy: must survive a wipe of a.gguf
    plant(root, "b.gguf")
    plant(root, "b.gguf.0xdead.dlim")
    t |> run("model-path wipe takes only that model's images") @(tt : T?) {
        let r = dlim_wipe(path_join(root, "a.gguf"))
        tt |> equal(2, r.removed)
        tt |> success(stat(path_join(root, "a.gguf")).is_valid, "gguf untouched")
        tt |> success(stat(path_join(root, "a.gguf2.0xdead.dlim")).is_valid, "shared-prefix sibling untouched")
        tt |> success(stat(path_join(root, "b.gguf.0xdead.dlim")).is_valid, "sibling model's image untouched")
    }
    t |> run("directory wipe takes every image, nothing else") @(tt : T?) {
        let r = dlim_wipe(root)
        tt |> equal(2, r.removed)
        tt |> success(stat(path_join(root, "a.gguf")).is_valid && stat(path_join(root, "b.gguf")).is_valid, "ggufs untouched")
        tt |> equal(0, dlim_wipe(root).removed)
    }
    var rerr : string
    rmdir_rec(root, rerr)
}
