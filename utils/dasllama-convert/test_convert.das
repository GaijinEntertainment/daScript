options gen2
options persistent_heap
options _dasllama_internal = true

// dasllama-convert's carrier sniff: the kind a source file routes
// to, off its magic and its GGUF metadata. The model cells skip
// through the suites' gate (`model_available`): a file absent from
// DASLLAMA_MODELS_DIR, a split with a missing shard, or a
// large-tier file without DASLLAMA_PARITY_FULL=1. The absent-file
// cell needs nothing. The bake cell runs main.das as a child on a
// temp copy of the ru-en student, so the models dir is never written.

require dastest/testing_boost public
require convert_sniff
// models_dir / model_available: the suites' presence and tier gate
require ../../modules/dasLLAMA/tests/_model_tier.das
require daslib/fio
require daslib/clargs                    // get_host_binary: the child bake runs under this daslang
require daslib/module_path               // get_this_module_dir: main.das sits beside this test
require dasllama/dasllama_image         // image_family_tag: the baked image's lane, read off its header

def private sniff_cell(t : T?; name : string; want : AsrCarrierKind) {
    let path = path_join(models_dir(), name)
    return if (!model_available(t, path))
    t |> equal(sniff_asr_kind(path), want, name)
}

[test]
def test_sniff_asr_kind(t : T?) {
    t |> run("the four GigaAM v3 carriers sniff as gigaam") @(t : T?) {
        for (v in ["ctc", "rnnt", "e2e-ctc", "e2e-rnnt"]) {
            sniff_cell(t, "gigaam-v3-{v}-f16.gguf", AsrCarrierKind.gigaam)
        }
    }
    t |> run("a decoder GGUF with no projector sniffs as a decoder") @(t : T?) {
        sniff_cell(t, "SmolLM2-135M-Instruct-Q8_0.gguf", AsrCarrierKind.decoder)
    }
    t |> run("a Qwen3-ASR audio mmproj sniffs as qwen3a") @(t : T?) {
        sniff_cell(t, "mmproj-Qwen3-ASR-0.6B-bf16.gguf", AsrCarrierKind.qwen3a)
    }
    t |> run("the ru-en and en-ru Bergamot students sniff as bergamot") @(t : T?) {
        for (m in ["ru-en-base-memory/model.ruen", "en-ru-base-memory/model.enru"]) {
            sniff_cell(t, "bergamot/{m}.intgemm.alphas.bin", AsrCarrierKind.bergamot)
        }
    }
    t |> run("a version-1 item table without the embedded model.yml is not Marian") @(t : T?) {
        let path = path_join(test_temp_dir(), "not_marian.bin")
        fopen(path, "wb") $(f) {
            let head = [1ul, 1ul, 5ul, 0x0404ul, 1ul, 4ul]
            var b <- [for (i in range(48)); uint8((head[i / 8] >> uint64(8 * (i % 8))) & 0xfful)]
            b |> push_from([for (c in "Wemb"); uint8(c)])
            b |> push(0u8)
            fwrite(f, b)
        }
        t |> equal(sniff_asr_kind(path), AsrCarrierKind.decoder)
        remove(path)
    }
    t |> run("no file is a decoder - the Model flow reports the missing file") @(t : T?) {
        let absent = path_join(test_temp_dir(), "absent.gguf")
        t |> equal(sniff_asr_kind(absent), AsrCarrierKind.decoder)
    }
}

[test]
def test_bake_bergamot(t : T?) {
    let src_dir = path_join(models_dir(), "bergamot/ru-en-base-memory")
    let src = path_join(src_dir, "model.ruen.intgemm.alphas.bin")
    return if (!model_available(t, src))
    let dir = path_join(test_temp_dir(), "ru-en-base-memory")
    let model = path_join(dir, "model.ruen.intgemm.alphas.bin")
    var err = ""
    t |> success(mkdir_rec(dir), "made the copy's directory")
    for (name in ["model.ruen.intgemm.alphas.bin", "vocab.ruen.spm"]) {
        let copied = copy_file(path_join(src_dir, name), path_join(dir, name), true, err)
        t |> success(copied, "copied {name}: {err}")
    }
    for (lane in [("q8", "bergamot-q8"), ("fp32", "bergamot-f32")]) {
        t |> run("a Marian student bakes its {lane._0} image beside the source") @(t : T?) {
            var out = ""
            let main = path_join(get_this_module_dir(), "main.das")
            let argv <- [get_host_binary(), "-jit", main, "--",
                "--model", model, "--quant", lane._0]
            let rc = run_and_capture(argv, out, 300.0)
            t |> equal(rc, 0, "the bake exits 0: {out}")
            let image = "{model}.{lane._1}.dlim"
            t |> success(stat(image).is_valid, "{base_name(image)} landed: {out}")
            remove(image)
        }
    }
    remove(path_join(dir, "model.ruen.intgemm.alphas.bin"))
    remove(path_join(dir, "vocab.ruen.spm"))
}

[test]
def test_bake_to_destination(t : T?) {
    let src_dir = path_join(models_dir(), "bergamot/ru-en-base-memory")
    let src = path_join(src_dir, "model.ruen.intgemm.alphas.bin")
    return if (!model_available(t, src))
    let dir = path_join(test_temp_dir(), "ru-en-dest")
    let model = path_join(dir, "model.ruen.intgemm.alphas.bin")
    var err = ""
    t |> success(mkdir_rec(dir), "made the copy's directory")
    for (name in ["model.ruen.intgemm.alphas.bin", "vocab.ruen.spm"]) {
        let copied = copy_file(path_join(src_dir, name), path_join(dir, name), true, err)
        t |> success(copied, "copied {name}: {err}")
    }
    t |> run("a Marian student bakes to -o, and no image lands beside the source") @(t : T?) {
        let out = path_join(test_temp_dir(), "ru-en-dest-out.dlim")
        remove(out)
        var log = ""
        let main = path_join(get_this_module_dir(), "main.das")
        let argv <- [get_host_binary(), "-jit", main, "--", "--model", model, "-o", out]
        let rc = run_and_capture(argv, log, 300.0)
        t |> equal(rc, 0, "the bake exits 0: {log}")
        t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
        t |> equal(image_family_tag(out), "bergamot-q8", "the image carries the q8 lane's tag")
        t |> success(!stat("{model}.bergamot-q8.dlim").is_valid, "nothing landed beside the source")
        remove(out)
    }
    remove(path_join(dir, "model.ruen.intgemm.alphas.bin"))
    remove(path_join(dir, "vocab.ruen.spm"))
}

[test]
def test_bake_gigaam_lanes(t : T?) {
    let src = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
    return if (!model_available(t, src))
    for (lane in [("q8", "gigaam-kq"), ("fp32", "gigaam-f16")]) {
        t |> run("the q4_k carrier at -q {lane._0} bakes the {lane._1} image to -o") @(t : T?) {
            let out = path_join(test_temp_dir(), "gigaam-{lane._0}.dlim")
            remove(out)
            var log = ""
            let main = path_join(get_this_module_dir(), "main.das")
            let argv <- [get_host_binary(), "-jit", main, "--", "--model", src, "--quant", lane._0, "-o", out]
            let rc = run_and_capture(argv, log, 600.0)
            t |> equal(rc, 0, "the bake exits 0: {log}")
            t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
            t |> equal(image_family_tag(out), lane._1, "the image carries the lane's tag")
            remove(out)
        }
    }
}
