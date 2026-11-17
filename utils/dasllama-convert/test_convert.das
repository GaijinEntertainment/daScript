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
require dasllama/dasllama_image         // image_family_tag: the baked image's lane, read off its header
require dasllama/dasllama_styletts2     // styletts2_stored_lane: the lane a written GGUF names
require daslib/env_registry             // env_value_of: the child bake's knobs, read the sanctioned way

def private sniff_cell(t : T?; name : string; want : AsrCarrierKind) {
    let path = path_join(models_dir(), name)
    return if (!model_available(t, path))
    t |> equal(sniff_asr_kind(path), want, name)
}

// The converter under this daslang with `args` after `--`: its exit code, its output in `log`. Every child starts in one
// directory under the run's temp folder, so the first writes the module cache and the JIT cache there and the rest read them.
def private convert_run(args : array<string>; var log : string&) : int {
    let root = get_full_file_name(get_das_root())
    var inscope argv <- [get_host_binary(), "-jit", "-dasroot", root, path_join(root, "utils/dasllama-convert/main.das"), "--"]
    argv |> push_from(args)
    let cwd = path_join(test_temp_dir(), "convert-children")
    mkdir_rec(cwd)
    var inscope env : array<string>
    return run_and_capture(argv, cwd, env, log, 1200.0)
}

[test(description = "the converter sniffs a GGUF as gigaam, parakeet, decoder, qwen3a or bergamot and refuses a non-Marian item table")]
def test_sniff_asr_kind(t : T?) {
    t |> run("the four GigaAM v3 carriers sniff as gigaam") @(t : T?) {
        for (v in ["ctc", "rnnt", "e2e-ctc", "e2e-rnnt"]) {
            sniff_cell(t, "gigaam-v3-{v}-f16.gguf", AsrCarrierKind.gigaam)
        }
    }
    t |> run("the F32 and Q4_K_M Parakeet GGUFs sniff as parakeet") @(t : T?) {
        for (q in ["F32", "Q4_K_M"]) {
            sniff_cell(t, "parakeet-tdt-0.6b-v3-{q}.gguf", AsrCarrierKind.parakeet)
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

[test(description = "a Marian student bakes its lane images beside the source", timeout = 1200)]
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
            let argv <- [
                "--model", model, "--quant", lane._0]
            let rc = convert_run(argv, out)
            t |> equal(rc, 0, "the bake exits 0: {out}")
            let image = "{model}.{lane._1}.dlim"
            t |> success(stat(image).is_valid, "{base_name(image)} landed: {out}")
            remove(image)
        }
    }
    remove(path_join(dir, "model.ruen.intgemm.alphas.bin"))
    remove(path_join(dir, "vocab.ruen.spm"))
}

[test(description = "a Marian student bakes to -o with no image beside the source, and -o naming the source is refused without deleting it", timeout = 1200)]
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
        let argv <- ["--model", model, "-o", out]
        let rc = convert_run(argv, log)
        t |> equal(rc, 0, "the bake exits 0: {log}")
        t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
        t |> equal(image_family_tag(out), "bergamot-q8", "the image carries the q8 lane's tag")
        t |> success(!stat("{model}.bergamot-q8.dlim").is_valid, "nothing landed beside the source")
        remove(out)
    }
    t |> run("-o naming the source itself is refused, and --force never deletes the source") @(t : T?) {
        let size = stat(model).size
        var log = ""
        let argv <- ["--model", model, "-o", model, "--force"]
        let rc = convert_run(argv, log)
        t |> equal(rc, 2, "the bake refuses: {log}")
        t |> success(find(log, "is the source") >= 0, "and says the output is the source: {log}")
        t |> equal(stat(model).size, size, "the source is intact")
    }
    remove(path_join(dir, "model.ruen.intgemm.alphas.bin"))
    remove(path_join(dir, "vocab.ruen.spm"))
}

[test(description = "the q4_k GigaAM carrier bakes each lane's image to -o and a destination mint reaps no stale image", timeout = 1200)]
def test_bake_gigaam_lanes(t : T?) {
    let src = path_join(models_dir(), "gigaam-v3-e2e-rnnt-q4_k.gguf")
    return if (!model_available(t, src))
    for (lane in [("q8", "gigaam-kq"), ("fp32", "gigaam-f16")]) {
        t |> run("the q4_k carrier at -q {lane._0} bakes the {lane._1} image to -o") @(t : T?) {
            let out = path_join(test_temp_dir(), "gigaam-{lane._0}.dlim")
            remove(out)
            var log = ""
            let argv <- ["--model", src, "--quant", lane._0, "-o", out]
            let gpu_was = env_value_of("DASLLAMA_GPU")
            set_env_variable("DASLLAMA_GPU", "1")   // the child's GPU tier armed: the planar bake is the CPU image whatever the box serves
            let rc = convert_run(argv, log)
            set_env_variable("DASLLAMA_GPU", gpu_was)
            t |> equal(rc, 0, "the bake exits 0: {log}")
            t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
            t |> equal(image_family_tag(out), lane._1, "the image carries the lane's tag")
            remove(out)
        }
    }
    t |> run("-o writes with the image rail off, and the source's stale lane images stay - a destination mint reaps nothing") @(t : T?) {
        let dir = path_join(test_temp_dir(), "gigaam-dest")
        let copy = path_join(dir, "gigaam-v3-e2e-rnnt-q4_k.gguf")
        var err = ""
        t |> success(mkdir_rec(dir) && copy_file(src, copy, true, err), "copied the carrier: {err}")
        let stale = "{copy}.gigaam-kq.dlim"
        fopen(stale, "wb") $(f) {
            fwrite(f, "not an image")
        }
        let out = path_join(test_temp_dir(), "gigaam-dest.dlim")
        remove(out)
        var log = ""
        let argv <- ["--model", copy, "--quant", "q8", "-o", out]
        let was = env_value_of("DASLLAMA_IMAGE")
        set_env_variable("DASLLAMA_IMAGE", "0")   // the child's image rail off: only the destination may sink the mint
        let rc = convert_run(argv, log)
        set_env_variable("DASLLAMA_IMAGE", was)
        t |> equal(rc, 0, "the bake exits 0: {log}")
        t |> success(stat(out).is_valid, "the image landed at -o with the rail off: {log}")
        t |> success(stat(stale).is_valid && stat(stale).size == 12ul, "the stale lane image beside the source survived untouched")
        remove(out)
        remove(stale)
        remove(copy)
    }
}


[test(description = "the 18m K-quant Pocket TTS file bakes each lane's image to -o", timeout = 1200)]
def test_bake_pocket_lanes(t : T?) {
    let src = path_join(models_dir(), "pocket-tts-en-18m-kq.gguf")
    return if (!model_available(t, src))
    for (lane in [("q8", "pocket-native"), ("fp32", "pocket-f32")]) {
        t |> run("the K-quant Pocket file at -q {lane._0} bakes the {lane._1} image to -o") @(t : T?) {
            let out = path_join(test_temp_dir(), "pocket-{lane._0}.dlim")
            remove(out)
            var log = ""
            let argv <- ["--model", src, "--quant", lane._0, "-o", out]
            let rc = convert_run(argv, log)
            t |> equal(rc, 0, "the bake exits 0: {log}")
            t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
            t |> equal(image_family_tag(out), lane._1, "the image carries the lane's tag")
            remove(out)
        }
    }
    t |> run("--recipe on a Pocket file is refused: it serves the formats it stores") @(t : T?) {
        var log = ""
        let out = path_join(test_temp_dir(), "pocket-recipe.dlim")
        let argv <- ["--model", src, "--recipe", "-o", out]
        t |> equal(convert_run(argv, log), 2, "the bake exits 2: {log}")
        t |> success(find(log, "--recipe bakes a KittenTTS or Kokoro source") >= 0 && !stat(out).is_valid, "and names the flag, with nothing written: {log}")
    }
}

[test(description = "kitten-nano bakes each lane's image to -o: the Q8_0 lane at -q q8, the recipe lane with --recipe, the reference lane at -q fp32; --recipe at -q fp32 is refused by name with nothing written", timeout = 1200)]
def test_bake_kitten_lanes(t : T?) {
    let src = path_join(models_dir(), "kitten-nano.gguf")
    return if (!model_available(t, src))
    for (lane in [("q8", "", "tts-q8"), ("q8", "--recipe", "tts-kq"), ("fp32", "", "tts-f32")]) {
        t |> run("kitten-nano at -q {lane._0} {lane._1} bakes the {lane._2} image to -o") @(t : T?) {
            let out = path_join(test_temp_dir(), "kitten-{lane._2}.dlim")
            remove(out)
            var log = ""
            var argv <- ["--model", src, "--quant", lane._0, "-o", out]
            if (!empty(lane._1)) {
                argv |> push(lane._1)
            }
            let rc = convert_run(argv, log)
            t |> equal(rc, 0, "the bake exits 0: {log}")
            t |> success(stat(out).is_valid, "{base_name(out)} landed: {log}")
            t |> equal(image_family_tag(out), lane._2, "the image carries the lane's tag")
            remove(out)
        }
    }
    t |> run("--recipe at -q fp32 is refused: the recipe lane mints at q8") @(t : T?) {
        var log = ""
        let out = path_join(test_temp_dir(), "kitten-recipe-f32.dlim")
        let argv <- ["--model", src, "--quant", "fp32", "--recipe", "-o", out]
        t |> equal(convert_run(argv, log), 2, "the bake exits 2: {log}")
        t |> success(find(log, "--recipe bakes a KittenTTS or Kokoro source") >= 0 && !stat(out).is_valid, "and names the flag, with nothing written: {log}")
    }
}

[test(description = "kitten-nano at --gguf writes the file that stores the Q8_0 lane's weights, and with --recipe the recipe lane's, each naming its lane and carrying its source's licence sidecar; that file bakes its own lane's image to -o, while --gguf over it, --recipe on it, --gguf at -q fp32, --gguf on a Pocket file and --gguf naming the source are refused by name with nothing written; --gguf onto a file already there leaves it, and onto an unwritable path exits 1", timeout = 1200)]
def test_write_kitten_stored(t : T?) {
    let src = path_join(models_dir(), "kitten-nano.gguf")
    return if (!model_available(t, src))
    let stored = path_join(test_temp_dir(), "convert-kitten-kq.gguf")
    for (lane in [("", "q8", "convert-kitten-q8.gguf"), ("--recipe", "kq", "convert-kitten-kq.gguf")]) {
        t |> run("kitten-nano at --gguf {lane._0} writes the file that stores the {lane._1} lane") @(t : T?) {
            let out = path_join(test_temp_dir(), lane._2)
            remove(out)
            var log = ""
            var argv <- ["--model", src, "--gguf", out]
            if (!empty(lane._0)) {
                argv |> push(lane._0)
            }
            t |> equal(convert_run(argv, log), 0, "the write exits 0: {log}")
            t |> success(stat(out).is_valid && stat(out).size < stat(src).size, "{lane._2} landed smaller than its source: {log}")
            t |> equal(styletts2_stored_lane(out), lane._1, "the file names the lane it stores")
            if (stat("{src}.LICENSE").is_valid) {   // a source with a licence sidecar hands its terms to the file written from it
                t |> success(fread("{out}.LICENSE") |> starts_with("{lane._2} - the weights of kitten-nano.gguf"), "the licence sidecar follows under the file's own name")
            }
            remove("{out}.LICENSE")
        }
    }
    t |> run("the stored file bakes its own lane's image, and takes no --gguf") @(t : T?) {
        return if (!stat(stored).is_valid)
        var log = ""
        let image = path_join(test_temp_dir(), "convert-kitten-kq.dlim")
        remove(image)
        let bake <- ["--model", stored, "-o", image]
        t |> equal(convert_run(bake, log), 0, "the bake exits 0: {log}")
        t |> equal(image_family_tag(image), "tts-kq", "the image carries the lane the file names, with no --recipe asked")
        remove(image)
        let again = path_join(test_temp_dir(), "convert-kitten-again.gguf")
        let rewrite <- ["--model", stored, "--gguf", again]
        t |> equal(convert_run(rewrite, log), 2, "--gguf over a stored file exits 2: {log}")
        t |> success(find(log, "--gguf take a file of floats") >= 0 && !stat(again).is_valid, "and names the flags, with nothing written: {log}")
    }
    t |> run("--gguf at -q fp32 and on a Pocket file is refused") @(t : T?) {
        var log = ""
        let out = path_join(test_temp_dir(), "convert-refused.gguf")
        let f32 <- ["--model", src, "--quant", "fp32", "--gguf", out]
        t |> equal(convert_run(f32, log), 2, "--gguf at -q fp32 exits 2: {log}")
        t |> success(find(log, "--gguf writes a KittenTTS or Kokoro source's Q8_0 or recipe lane at -q q8") >= 0 && !stat(out).is_valid, "and names the lane: {log}")
        let pocket = path_join(models_dir(), "pocket-tts-en-18m-kq.gguf")
        if (stat(pocket).is_valid) {
            let pk <- ["--model", pocket, "--gguf", out]
            t |> equal(convert_run(pk, log), 2, "--gguf on a Pocket file exits 2: {log}")
            t |> success(find(log, "a Pocket file is minted by harness/convert_pocket.py") >= 0 && !stat(out).is_valid, "and names its converter: {log}")
        }
    }
    t |> run("--gguf onto the source, onto a file already there, onto a path that cannot be written, and --recipe on a stored file") @(t : T?) {
        return if (!stat(stored).is_valid)
        var log = ""
        let onto_source <- ["--model", src, "--gguf", src]
        t |> equal(convert_run(onto_source, log), 2, "--gguf naming the source exits 2: {log}")
        t |> success(find(log, "--gguf '{src}' is the source") >= 0, "and names the flag and the path: {log}")
        let present = path_join(test_temp_dir(), "convert-kitten-q8.gguf")
        let before = stat(present).mtime
        let again <- ["--model", src, "--gguf", present]
        t |> equal(convert_run(again, log), 0, "--gguf onto a file already there exits 0: {log}")
        t |> equal(stat(present).mtime, before, "and leaves it as it was, with no --force")
        let nowhere = path_join(test_temp_dir(), "convert-no-such-dir/kitten.gguf")
        let unwritable <- ["--model", src, "--gguf", nowhere]
        t |> equal(convert_run(unwritable, log), 1, "--gguf onto a path that cannot be written exits 1: {log}")
        t |> success(find(log, "did not land") >= 0 && !stat(nowhere).is_valid, "and says it did not land: {log}")
        let image = path_join(test_temp_dir(), "convert-kitten-recipe.dlim")
        let recipe <- ["--model", stored, "--recipe", "-o", image]
        t |> equal(convert_run(recipe, log), 2, "--recipe on a stored file exits 2: {log}")
        t |> success(find(log, "bakes that lane's image alone") >= 0 && !stat(image).is_valid, "the file names its own lane, and nothing is written: {log}")
    }
    for (name in fixed_array("convert-kitten-q8.gguf", "convert-kitten-kq.gguf")) {
        remove(path_join(test_temp_dir(), name))
    }
}

[test(description = "kitten-nano copied away from its licence sidecar and written at --gguf lands with no sidecar beside it", timeout = 1200)]
def test_write_kitten_without_sidecar(t : T?) {
    let src = path_join(models_dir(), "kitten-nano.gguf")
    return if (!model_available(t, src))
    t |> run("a source with no licence sidecar hands none to the file written from it") @(t : T?) {
        var why : string
        let bare = path_join(test_temp_dir(), "convert-kitten-bare.gguf")
        let out = path_join(test_temp_dir(), "convert-kitten-bare-q8.gguf")
        remove(out)
        remove("{out}.LICENSE")
        t |> success(copy_file(src, bare, true, why), "the source is copied away from its sidecar: {why}")
        var log = ""
        let argv <- ["--model", bare, "--gguf", out]
        t |> equal(convert_run(argv, log), 0, "the write exits 0: {log}")
        t |> success(stat(out).is_valid && !stat("{out}.LICENSE").is_valid, "the file lands with no sidecar beside it")
        remove(bare)
        remove(out)
    }
}
