options gen2
require dastest/testing_boost public
require daslib/fio
require dasllama/dasllama_image

// dlim_wipe is verdict- and lane-blind BY DESIGN (the batch-lifecycle owner carve-out,
// ARCHITECTURE.md 2.1) — but its reach must be exact: a model path wipes only that model's
// images, a directory path wipes every image in it, and non-.dlim files are never touched.

let TMP = "modules/dasLLAMA/tests/_test_wipe"

def private plant(name : string) {
    fopen("{TMP}/{name}", "wb") $(f) {
        if (f != null) {
            f |> fwrite("not a real image")
        }
    }
}

[test]
def test_dlim_wipe(t : T?) {
    mkdir_rec(TMP)
    plant("a.gguf")
    plant("a.gguf.0xdead.dlim")
    plant("a.gguf.0xbeef.dlim")
    plant("b.gguf")
    plant("b.gguf.0xdead.dlim")
    t |> run("model-path wipe takes only that model's images") @(t : T?) {
        let r = dlim_wipe("{TMP}/a.gguf")
        t |> equal(2, r.removed)
        t |> success(stat("{TMP}/a.gguf").is_valid, "gguf untouched")
        t |> success(stat("{TMP}/b.gguf.0xdead.dlim").is_valid, "sibling model's image untouched")
    }
    t |> run("directory wipe takes every image, nothing else") @(t : T?) {
        let r = dlim_wipe(TMP)
        t |> equal(1, r.removed)
        t |> success(stat("{TMP}/a.gguf").is_valid && stat("{TMP}/b.gguf").is_valid, "ggufs untouched")
        t |> equal(0, dlim_wipe(TMP).removed)
    }
    remove("{TMP}/a.gguf")
    remove("{TMP}/b.gguf")
    rmdir_result(TMP)
}
