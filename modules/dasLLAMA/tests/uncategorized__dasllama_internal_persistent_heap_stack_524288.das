options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require daslib/fio   // the temp dump directory the read test writes; the wipe cell's planted files
require ../harness/vk_spv_diff.das
require dasllama/dasllama_parity
require dasllama/dasllama_image
require dasllama/dasllama_tune
require dasllama/dasllama_core   // kernel_report, prepared_image_alignment: a host's two questions
require dasllama/dasllama_vad   // load_vad_model, vad_probs_cpu: the load from bytes
require dasllama/dasllama_math   // with_dasllama_jobque_
require _image_memory   // read_page_aligned, zero_bytes: a host's own copy of the bin
require strings   // find: a refusal's words
require math

// The SPIR-V dump differ (harness/vk_spv_diff.das) - the identity gate every kernel fold's
// "byte-identical" claim rests on: its four bins over two in-memory dumps, and the verdict's refusal
// of a side with nothing in it.

def private dump_of(stamps : table<string; array<uint8>>) : table<string; array<uint8>> =>
    <- clone_to_move(stamps)

[test(description = "`diff_dumps` bins stamps into same, moved (byte or length change), appeared and vanished in name order, and `dumps_identical` passes only two non-empty dumps whose every stamp matches")]
def test_vk_spv_diff_bins(t : T?) {
    t |> run("the differ bins every stamp: identical, moved, appeared, vanished - sorted by name") @(t : T?) {
        var base <- dump_of({ "b_cls_spv" => [uint8(1), uint8(2), uint8(3)], "a_cls_spv" => [uint8(9)], "gone_cls_spv" => [uint8(4), uint8(4)], "len_cls_spv" => [uint8(7), uint8(7)] })
        var fresh <- dump_of({ "b_cls_spv" => [uint8(1), uint8(2), uint8(3)], "a_cls_spv" => [uint8(8)], "new_cls_spv" => [uint8(5)], "len_cls_spv" => [uint8(7), uint8(7), uint8(7)] })
        let d <- diff_dumps(base, fresh)
        t |> equal(d.same, ["b_cls_spv"], "the byte-identical stamp is the one whose every byte matches")
        t |> equal(d.moved, ["a_cls_spv", "len_cls_spv"], "a changed byte and a changed length both read as moved, in name order")
        t |> equal(d.appeared, ["new_cls_spv"], "a stamp only the fresh dump holds appeared")
        t |> equal(d.vanished, ["gone_cls_spv"], "a stamp only the base dump holds vanished")
        t |> success(!dumps_identical(d, length(base), length(fresh)), "the verdict fails on any moved, appeared or vanished stamp")
    }
    t |> run("the verdict passes only two non-empty dumps whose every stamp matches") @(t : T?) {
        var base <- dump_of({ "a_cls_spv" => [uint8(1)], "b_cls_spv" => [uint8(2), uint8(3)] })
        var fresh <- dump_of({ "b_cls_spv" => [uint8(2), uint8(3)], "a_cls_spv" => [uint8(1)] })
        let d <- diff_dumps(base, fresh)
        t |> equal(d.same, ["a_cls_spv", "b_cls_spv"], "both stamps match")
        t |> success(dumps_identical(d, length(base), length(fresh)), "two matching dumps pass")
        var none : table<string; array<uint8>>
        let e <- diff_dumps(none, fresh)
        t |> equal(e.appeared, ["a_cls_spv", "b_cls_spv"], "against an empty base every fresh stamp appeared")
        let both <- diff_dumps(none, none)
        t |> success(empty(both.same) && empty(both.moved) && empty(both.appeared) && empty(both.vanished), "two empty dumps bin nothing")
        t |> success(!dumps_identical(both, 0, 0), "two empty dumps do not pass - nothing was compared")
    }
}

[test(description = "`read_dump` reads only the .spv files of a directory, keyed by stem with the file's exact bytes, and ignores other files")]
def test_vk_spv_diff_reads_a_directory(t : T?) {
    //! read_dump takes the .spv files of a directory by stem and ignores the rest
    let dir_r = create_temp_directory_result("vk_spv_diff")
    t |> success(dir_r is value, "temp dump dir created")
    if (!(dir_r is value)) return
    let dirp = unsafe(dir_r.value)
    t |> success(fwrite(path_join(dirp, "one_cls_spv.spv"), "abc"), "a stamp written")
    t |> success(fwrite(path_join(dirp, "notes.txt"), "x"), "a non-stamp written")
    var d <- read_dump(dirp)
    t |> equal(length(d), 1, "one stamp read")
    t |> success(key_exists(d, "one_cls_spv"), "the stamp is keyed by its stem")
    t |> equal(length(d["one_cls_spv"]), 3, "the stamp's bytes are the file's")
    t |> success(rmdir_rec_result(dirp) is value, "temp dump dir removed")
}

// The parity ref-cache codec (model-free — runs everywhere incl. CI): a saved cache round-trips,
// and a corrupt/truncated cache REJECTS as a whole (load returns false, output left empty) so the
// caller recomputes — a silent-zero parse here would poison every parity truth downstream.

def private probe_path(tag : string) : string {
    var terr : string
    let tmp = temp_directory(terr)   // portable (no /tmp on Windows); cwd as a last resort
    return path_join(empty(tmp) ? "." : tmp, "dasllama_parity_probe_{tag}_{ref_time_ticks()}.tsv")
}

[test(description = "Saved parity ref-cache ids (negatives and large values included) and float vals load back with the same length and bit-exact values")]
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

[test(description = "A ref cache with a garbage id token or a trailing-garbage float token fails to load as a whole and leaves the output empty, rather than parsing silent zeros or a prefix")]
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

// dlim_wipe is verdict- and lane-blind BY DESIGN (the batch-lifecycle owner carve-out,
// ARCHITECTURE_IMAGE.md 2.1) — but its reach must be exact: a model path wipes only that model's
// images, a directory path wipes every image in it, and non-.dlim files are never touched.

def private plant(root, name : string) {
    fopen(path_join(root, name), "wb") $(f) {
        if (f != null) {
            f |> fwrite("not a real image")
        }
    }
}

[test(description = "`dlim_wipe` on a model path removes exactly that model's .dlim images, sparing the gguf, a shared-prefix decoy and a sibling model's image, and on a directory removes every remaining .dlim and nothing else")]
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

def template twice_template(x : int) : int => PH(x) + PH(x)

def private inc(x : int) : int => x + 1

def private dbl(x : int) : int => x * 2

[from_template(src = "twice_template", PH = "inc")]
def private twice_inc(x : int) : int {}

[from_template(src = "twice_template", PH = "dbl")]
def private twice_dbl(x : int) : int {}

def template scale_template(ws : float const ?; bi : int64) : float => WS(ws, bi) * 2.0

def private ws_f32(ws : float const?; bi : int64) : float {
    unsafe {
        return ws[bi]
    }
}

def private ws_half(ws : uint16 const?; bi : int64) : float {
    unsafe {
        return float(ws[bi]) * 0.5
    }
}

[from_template(src = "scale_template", WS = "ws_f32")]
def private scaled(ws : float const?; bi : int64) : float {}

[from_template(src = "scale_template", WS = "ws_half")]
def private scaled(ws : uint16 const?; bi : int64) : float {}

[test(description = "`[from_template]` renames a template's placeholder call to each stub's target, and the stub's signature types the clone so one template stamps both a float and a uint16 plane overload with their own readers")]
def test_from_template(t : T?) {
    t |> run("a placeholder call renames to the annotation's target per stub") @(t : T?) {
        t |> equal(8, twice_inc(3), "PH = inc: (3 + 1) + (3 + 1)")
        t |> equal(12, twice_dbl(3), "PH = dbl: (3 * 2) + (3 * 2)")
    }
    t |> run("the stub's signature types the clone, so one template stamps both plane overloads") @(t : T?) {
        var f = [1.5, 2.5]
        var h = [uint16(3), uint16(4)]
        unsafe {
            t |> equal(5.0, scaled(addr(f[0]), 1l), "the float plane reads its scale")
            t |> equal(4.0, scaled(addr(h[0]), 1l), "the halfword plane widens through its own reader")
        }
    }
}

[test(description = "A Silero VAD bin its host holds in memory, at an odd address, loads from the bytes and keeps none of them: its CPU probabilities on a fixed signal are the file load's bit for bit after the bytes are overwritten; a copy with 4096 bytes zeroed answers other probabilities; a buffer shorter than the header is refused by name")]
def test_vad_from_memory(t : T?) {
    t |> run("the shipped bin's bytes, one byte past a page boundary") @(t : T?) {
        let signal <- [for (i in range(10 * 512)); sin(float(i) * 0.37) * 0.1]
        with_dasllama_jobque_() {
            var inscope from_file_model <- load_vad_model(vad_model_path())
            var inscope from_file <- vad_probs_cpu(from_file_model, signal)
            t |> equal(length(from_file), 10, "ten chunks, ten probabilities")
            var raw : array<uint8>
            var size = 0ul
            let base = read_page_aligned(vad_model_path(), raw, size, 1l)
            t |> success(base != null, "the bin reads into memory")
            if (base != null) {
                let short = panic_text() {
                    var inscope cut <- load_vad_model(base, 40ul)
                }
                t |> success(find(short, "is not a model") >= 0, "forty bytes are refused by name: '{short}'")
                var inscope poisoned_raw : array<uint8>
                var poisoned_size = 0ul
                let poisoned_base = read_page_aligned(vad_model_path(), poisoned_raw, poisoned_size)
                zero_bytes(poisoned_base, poisoned_size / 2ul, 4096ul)
                var inscope poisoned <- load_vad_model(poisoned_base, poisoned_size)
                var inscope other <- vad_probs_cpu(poisoned, signal)
                var inscope from_memory_model <- load_vad_model(base, size)
                zero_bytes(base, 0ul, size)   // the load copied every tensor: the host's bytes are free
                var inscope from_memory <- vad_probs_cpu(from_memory_model, signal)
                t |> equal(length(from_memory), length(from_file), "the same chunk count")
                var same = length(from_memory) == length(from_file)
                var moved = false
                for (m, f, o in from_memory, from_file, other) {
                    same &&= m == f
                    moved ||= o != f
                }
                t |> success(same, "every chunk's probability is the file load's, bit for bit")
                t |> success(moved, "4096 bytes zeroed, the load answers other probabilities (the control)")
            }
            delete raw
        }
    }
}

[test(description = "`kernel_report` names the kernels this run stamped - a count of kernel families under the JIT, the reference kernels in a build with no tune framework - and `prepared_image_alignment` is the image page")]
def test_kernel_report(t : T?) {
    let report = kernel_report()
    to_log(LOG_INFO, "kernel report: {report}\n")
    t |> success(find(report, "kernel families - ") >= 0 || find(report, "reference kernels") >= 0,
        "the report names the stamped families or the reference kernels: '{report}'")
    t |> equal(prepared_image_alignment(), IMAGE_PAGE, "an image in memory starts on the image page")
}
