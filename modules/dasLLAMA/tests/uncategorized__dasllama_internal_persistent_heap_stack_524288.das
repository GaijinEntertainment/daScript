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
require _kv_prefix_cells   // the context file's cells on a synthetic Config
require _tower_twin   // twin_rows_held: a twin's held verdict on synthetic rows
require _compares   // probe_fails, probe_counts: a verdict's failed asserts and skips counted
require strings   // find: a refusal's words; starts_with: a blind entry no other opens
require dasllama/dasllama_gpu_rail   // RailProfSums, rail_prof_lines: the homes' one profile report
require daslib/enum_trait   // every RailProfRole walked against the ledger's width
require dasllama/dasllama_env   // g_env_gpu: the knob the ledger reads with the pin off
require math
require _kernel_census   // the census files a run's processes write, merged, and the verdicts
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - the census counter

// The SPIR-V dump differ (harness/vk_spv_diff.das) - the identity gate every kernel fold's
// "byte-identical" claim rests on: its four bins over two in-memory dumps, and the verdict's refusal
// of a side with nothing in it.

def private dump_of(stamps : table<string; array<uint8>>) : table<string; array<uint8>> =>
    <- clone_to_move(stamps)

[test(description = "`diff_dumps` bins stamps into same, moved (byte or length change), appeared and vanished in name order, and `dumps_identical` passes only two non-empty dumps whose every stamp matches")]
def test_vk_spv_diff_bins(t : T?) {
    t |> run("the differ bins every stamp: identical, moved, appeared, vanished - sorted by name") @(t : T?) {
        var base <- dump_of({ "b_cls_spv" => [1u8, 2u8, 3u8], "a_cls_spv" => [9u8], "gone_cls_spv" => [4u8, 4u8], "len_cls_spv" => [7u8, 7u8] })
        var fresh <- dump_of({ "b_cls_spv" => [1u8, 2u8, 3u8], "a_cls_spv" => [8u8], "new_cls_spv" => [5u8], "len_cls_spv" => [7u8, 7u8, 7u8] })
        let d <- diff_dumps(base, fresh)
        t |> equal(d.same, ["b_cls_spv"], "the byte-identical stamp is the one whose every byte matches")
        t |> equal(d.moved, ["a_cls_spv", "len_cls_spv"], "a changed byte and a changed length both read as moved, in name order")
        t |> equal(d.appeared, ["new_cls_spv"], "a stamp only the fresh dump holds appeared")
        t |> equal(d.vanished, ["gone_cls_spv"], "a stamp only the base dump holds vanished")
        t |> success(!dumps_identical(d, length(base), length(fresh)), "the verdict fails on any moved, appeared or vanished stamp")
    }
    t |> run("the verdict passes only two non-empty dumps whose every stamp matches") @(t : T?) {
        var base <- dump_of({ "a_cls_spv" => [1u8], "b_cls_spv" => [2u8, 3u8] })
        var fresh <- dump_of({ "b_cls_spv" => [2u8, 3u8], "a_cls_spv" => [1u8] })
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

//! a synthetic twin whose three clean legs ran: the device chain the CPU q8 chain, both a step off the exact chain;
//! `poisoned` its poison leg's rows where it took one
def private fx_twin(owed, poisoned : bool) : TwinRows {
    var k = TwinRows(poison_owed = owed, legs = 3)
    k.spec = TwinSpec(family = "fx", tag = "fx twin", n_layer = 2l, metric = TwinMetric.rel_l2,
        seat_family = TowerFamily.whisper)
    k.exact <- [1.0, 2.0, 3.0, 4.0]
    k.cpu <- [1.01, 2.0, 3.0, 4.0]
    k.dev <- [1.01, 2.0, 3.0, 4.0]
    if (poisoned) {
        k.zero_block = 1l
        k.poisoned <- [-4.0, 0.0, 9.0, 0.0]
    }
    return <- k
}

[test(description = "`twin_rows_held` reds a twin whose clean legs ran and whose owed input poison did not, and holds a twin that kept its poison and one that owes none")]
def test_twin_rows_owed_poison(t : T?) {
    for (owed, poisoned, fails in fixed_array(true, true, false), fixed_array(true, false, false),
            fixed_array(0, 1, 0)) {
        var inscope k <- fx_twin(owed, poisoned)
        let got = probe_fails() $(p) {
            twin_rows_held(p, k) $(legs) {
                p |> equal(legs.n_dev, 0l, "the legs' counters reach the block")
            }
        }
        t |> equal(got, fails, "owed {owed}, poisoned {poisoned}: the held verdict's failed asserts")
    }
}

[test(description = "A context file carries a cached prefix between caches: a dense session's rows, and a recurrent checkpoint's rows and state, restore bit for bit into a pool of another page size - a recurrent request's every checkpoint, the stop its prefill left and its finished turn, each with its own state - where a prompt opening with the file's tokens attaches all of them, and the save leaves the cache holding the context; a context the cache does not hold, a path that does not open either way, a file that is not a context file, another model's, another codec's, one past the server's context, a cut one, one naming a token past the vocabulary, a dense file offered to a recurrent model, a file cut inside a checkpoint, a checkpoint of another model's shape and one the cache declines each refuse by name and leave no page held")]
def test_prefix_context_file(t : T?) {
    let c = mk_config(2l, 4l, 2l, 16l, 32l)
    let dense_path = path_join(test_temp_dir(), "dasllama_ctx_dense.bin")
    let state_path = path_join(test_temp_dir(), "dasllama_ctx_state.bin")
    t |> run("a dense context: saved off one pool, restored into a pool of another page size, attached whole with the donor's bytes") @(t : T?) {
        context_dense_cell(t, c, dense_path)
    }
    t |> run("a recurrent checkpoint: its rows and its state come back, the state standing at the checkpoint") @(t : T?) {
        context_recurrent_cell(t, c, state_path)
    }
    t |> run("a recurrent file cut inside a checkpoint, and one whose checkpoint is another model's, refuse by name") @(t : T?) {
        context_cut_cells(t, c, state_path)
    }
    t |> run("the refusals, each by name, none leaving a page held") @(t : T?) {
        context_refusal_cells(t, c, dense_path, state_path)
    }
    t |> run("a save picks the checkpoints its tokens open with, shortest first, whatever order the cache holds them in") @(t : T?) {
        context_pick_cell(t, c, path_join(test_temp_dir(), "dasllama_ctx_pick.bin"))
    }
}

[test(description = "The homes' one profile report charges a stamp to its role, prints the chain's line under the home's tag and a line a stamped role in role order with its share, leaves an unstamped role out, keeps the reported ledger as the last, and every profile role fits the ledger's width")]
def test_rail_prof_report(t : T?) {
    var s : RailProfSums
    rail_prof_add(s, RailProfRole.tts_lstm, 50.0lf)
    rail_prof_add(s, RailProfRole.tts_gemm, 30.0lf)
    rail_prof_add(s, RailProfRole.tts_gemm, 20.0lf)
    var inscope lines <- rail_prof_lines(s, "cuda", "decode", 3l, 128l, 900)
    t |> equal(length(lines), 3)
    let head = "cuda tower decode gpu: 3 blocks x 128 rows, 3 stamps"
    t |> equal(lines[0], "{head}, 100 us on the device, 900 us host wall")
    t |> equal(lines[1], "  tts_gemm: 50 us over 2 stamps (50%)")
    t |> equal(lines[2], "  tts_lstm: 50 us over 1 stamps (50%)")
    var inscope none <- rail_prof_lines(RailProfSums(), "vk", "albert", 1l, 1l, 0)
    t |> equal(length(none), 1)   // control: an empty ledger prints its chain line alone
    rail_prof_report(s, "cuda", "decode", 3l, 128l, 900)
    t |> equal(rail_prof_last().stamps, 3)
    t |> equal(rail_prof_last().n[int(RailProfRole.tts_gemm)], 2)
    for (r in type<RailProfRole>) {
        t |> success(int(r) < RAIL_PROF_ROLES, "{r} fits the ledger's {RAIL_PROF_ROLES} roles")
    }
    let was = set_rail_prof_pin(true)
    t |> success(rail_prof_on(), "the pin turns the ledger on")
    set_rail_prof_pin(false)
    t |> equal(rail_prof_on(), g_env_gpu.gpu_prof)   // the pin off, the knob decides
    set_rail_prof_pin(was)
}

//! a census file of `name` in `folder`, written as a process writes
//! one
def private census_file_at(folder, name : string; cov : table<string; int64>) {
    fopen(path_join(folder, name), "wb") $(f) {
        fwrite(f, sprint_json(cov, false))
    }
}

var private g_census_verdict_ran = true   //! whether the skip case's verdict call answered true

[test(description = "A home's census files merge into one table - every key summed over the files, the keys' union, a note flag set in one file set in the merge - with another home's file and a file of no home left out; the verdict gets the merged table and the whole flag the run passes; a named folder holding no file of a home reads as a failed verdict; an unset folder skips loudly and its verdict never runs")]
def test_kernel_census_merge(t : T?) {
    let folder = path_join(test_temp_dir(), "kernel_census")
    mkdir_rec(folder)
    census_file_at(folder, "metal_1.json", { "k_a" => 2l, "k_b" => 0l, "@metal_crown.x" => 1l })
    census_file_at(folder, "metal_2.json", { "k_a" => 3l, "k_c" => 5l, "@vk_routed" => 2l })
    census_file_at(folder, "vulkan_1.json", { "k_a" => 100l })
    census_file_at(folder, "notes.json", { "k_a" => 1000l })
    var inscope m <- kernel_census_merged(folder, CENSUS_METAL)
    t |> equal(m.files, 2, "the two Metal files merge")
    t |> equal(m.cov?["k_a"] ?? -1l, 5l, "a key both files hold sums; no other file adds to it")
    t |> equal(m.cov?["k_c"] ?? -1l, 5l, "a key one file holds keeps its count")
    t |> success(key_exists(m.cov, "k_b"), "a kernel at zero stays a key: the union")
    t |> equal(m.cov?["@metal_crown.x"] ?? 0l, 1l, "a flag one file set is set")
    t |> equal(m.cov?["@vk_routed"] ?? 0l, 2l, "a note sums")
    var wholes : array<bool>
    for (whole in [false, true]) {
        with_kernel_census(t, folder, CENSUS_METAL, whole) $(cov, got) {
            t |> equal(cov?["k_a"] ?? -1l, 5l, "the verdict reads the merged table")
            wholes |> push(got)
        }
    }
    t |> equal(wholes, [false, true], "the verdict gets the whole flag the run passes")
    var inscope none <- kernel_census_merged(folder, "cuda")
    t |> equal(none.files, 0, "a home with no census file in the folder merges none")
    t |> equal(census_read_of(folder, none.files, true), CensusRead.fail_empty,
        "a home with no census file in a whole run's folder fails the verdict")
    t |> run("an unset census folder skips loudly") @(t : T?) {
        g_census_verdict_ran = with_kernel_census(t, "", CENSUS_METAL, false) $(_cov, _whole) {
            t |> failure("the verdict ran with no census folder")
        }
    }
    t |> success(!g_census_verdict_ran, "the skip answers false")
}

[test(description = "A census folder holding no file of the home fails the verdict of a whole run through `with_kernel_census` itself and skips a narrowed run's, and a failure too is a census file that does not parse: it adds no key, counts as no file, and is named in the failure")]
def test_kernel_census_fail_and_corrupt(t : T?) {
    let folder = path_join(test_temp_dir(), "kernel_census_corrupt")
    mkdir_rec(folder)
    var ran = false
    let whole = probe_counts() $(p) {
        ran = with_kernel_census(p, folder, CENSUS_METAL, true) $(_cov, _whole) {
            p |> failure("the verdict ran over an empty folder")
        }
    }
    t |> equal(whole, int2(1, 0), "a whole run's empty folder is one failure, the verdict unrun")
    t |> success(!ran, "and the read answers false")
    let narrowed = probe_counts() $(p) {
        ran = with_kernel_census(p, folder, CENSUS_METAL, false) $(_cov, _whole) {
            p |> failure("the verdict ran over an empty folder")
        }
    }
    t |> equal(narrowed, int2(0, 1), "a narrowed run's empty folder is one skip, the verdict unrun")
    t |> success(!ran, "and that read answers false too")
    census_file_at(folder, "metal_1.json", { "k_a" => 2l })
    t |> success(fwrite(path_join(folder, "metal_2.json"), "\{\"k_a\": 40, oops"), "a cut file is planted")
    var inscope m <- kernel_census_merged(folder, CENSUS_METAL)
    t |> equal(m.files, 1, "the corrupt file counts as no file")
    t |> equal(m.cov?["k_a"] ?? -1l, 2l, "and adds no key")
    t |> equal(length(m.corrupt), 1, "and is named as corrupt")
    var judged = false
    let cut = probe_fails() $(p) {
        judged = with_kernel_census(p, folder, CENSUS_METAL, false) $(_cov, _whole) {
            p |> failure("the verdict ran over an undercounted census")
        }
    }
    t |> equal(cut, 1, "a corrupt file beside a good one is one failure, the verdict unrun")
    t |> success(!judged, "and the read answers false")
}

[test(description = "A census reset under a named folder keeps the dispatches before it in the process's sum, which `kernel_census_write_in` writes with the notes and the dispatches since into one file a home that merges back; a reset with no folder drops them, and a home the build lacks writes no file")]
def test_kernel_census_write(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let folder = path_join(test_temp_dir(), "kernel_census_write")
        var why : string
        rmdir_rec(folder, why)
        g_kcov_folded["fx_kernel"] = 3l
        kernel_census_reset_metal_in(folder)
        t |> equal(metal_kernel_coverage()?["fx_kernel"] ?? 0l, 0l, "the reset clears the engine's count")
        g_kcov_folded["fx_kernel"] = 2l
        kernel_census_note_add("@fx_note", 4l)
        kernel_census_write_in(folder)
        kernel_census_write_in(folder)
        var inscope m <- kernel_census_merged(folder, CENSUS_METAL)
        t |> equal(m.files, 1, "one file a home, written over")
        t |> equal(m.cov?["fx_kernel"] ?? -1l, 5l, "the dispatches before the reset and since")
        t |> equal(m.cov?["@fx_note"] ?? -1l, 4l, "the notes beside them")
        static_if (!typeinfo builtin_module_exists(vulkan)) {
            var inscope vk <- kernel_census_merged(folder, CENSUS_VULKAN)
            t |> equal(vk.files, 0, "a home the build lacks writes no file")
        }
        g_kcov_folded["fx_kernel"] = 7l
        kernel_census_reset_metal_in("")
        g_kcov_folded["fx_other"] = 1l
        kernel_census_write_in(folder)
        var inscope dropped <- kernel_census_merged(folder, CENSUS_METAL)
        t |> equal(dropped.cov?["fx_kernel"] ?? -1l, 3l, "a reset with no folder drops its dispatches")
        kernel_census_reset_metal_in("")
        kernel_census_notes_reset()
    } else {
        t |> skip("das_metal is not built: no census counter to drive")
    }
}

//! `n` kernels counted once each, named apart from every census
//! entry, beside every one of `blinds` at zero
def private planted(blinds : array<string>; n : int) : table<string; int64> {
    var cov <- {for (b in blinds); b => 0l}
    for (i in range(n)) {
        cov["fx_kernel_{i}"] = 1l
    }
    return <- cov
}

//! the first of `entries` that no other entry opens and that opens
//! none, of the ones `keep` holds: its hits count for itself alone
def private lone_entry(entries : array<string>; keep : block<(e : string) : bool>) : string {
    for (e in entries) {
        continue if (!invoke(keep, e))
        var alone = true
        for (o in entries) {
            alone &&= o == e || (!starts_with(o, e) && !starts_with(e, o))
        }
        return e if (alone)
    }
    return ""
}

//! the Metal verdict's failed asserts and skips on `cov`
def private metal_probe(cov : table<string; int64>; whole : bool) : int2 {
    return probe_counts() $(p) {
        metal_census_verdict(p, cov, whole)
    }
}

[test(description = "The Metal census verdict on planted tables: a blind entry a cell dispatched fails, a crown-conditional one is skipped under its `@metal_crown.` note and fails without it, the floor of 40 dispatched kernels asserts only on a whole run and counts no note key")]
def test_metal_census_verdict_branches(t : T?) {
    var inscope crowns <- metal_census_crown_entries()
    var inscope blinds <- metal_census_blind_entries()
    let plain = lone_entry(blinds) $(e) => !key_exists(crowns, e)
    let crowned = lone_entry(blinds) $(e) => key_exists(crowns, e)
    t |> success(!empty(plain) && !empty(crowned), "a plain and a crowned blind entry stand alone")
    var inscope floor <- planted(blinds, 40)
    t |> equal(metal_probe(floor, true), int2(0, 0), "40 kernels on a whole run hold the floor")
    var inscope short <- planted(blinds, 39)
    t |> equal(metal_probe(short, true).x, 1, "39 kernels on a whole run fail the floor")
    t |> equal(metal_probe(short, false).x, 0, "a run that is not whole asserts no floor")
    short["@fx_note"] = 1l
    t |> equal(metal_probe(short, true).x, 1, "a note key is no dispatched kernel")
    var inscope hit <- clone_to_move(floor)
    hit[plain] = 2l
    t |> equal(metal_probe(hit, false).x, 1, "a dispatched blind entry fails: {plain}")
    var inscope crown_hit <- clone_to_move(floor)
    crown_hit[crowned] = 2l
    t |> equal(metal_probe(crown_hit, false), int2(1, 0), "a crowned entry dispatched off its crown fails")
    crown_hit["@metal_crown.{crowns?[crowned] ?? ""}"] = 1l
    t |> equal(metal_probe(crown_hit, false), int2(0, 1), "under its crown's note it skips: {crowned}")
}

//! the Vulkan verdict's failed asserts on `cov`
def private vk_fails(cov : table<string; int64>; whole : bool) : int {
    return probe_counts($(p) {
        vulkan_census_verdict(p, cov, whole)
    }).x
}

[test(description = "The Vulkan census verdict on planted tables: a dispatched blind entry fails, a pick-conditional entry is held to a dispatch only where its pick's note holds (`@vk_int8_columns`, read over every routed row) - and the floor of 24 asserts only on a whole run and counts no note key")]
def test_vulkan_census_verdict_branches(t : T?) {
    var inscope blinds <- vulkan_census_blind_entries()
    var inscope picks <- vulkan_census_pick_entries()
    var inscope floor <- planted(blinds, 24)
    t |> equal(vk_fails(floor, true), 0, "24 kernels on a whole run hold the floor")
    var inscope short <- planted(blinds, 23)
    t |> equal(vk_fails(short, true), 1, "23 kernels on a whole run fail the floor")
    t |> equal(vk_fails(short, false), 0, "a run that is not whole asserts no floor")
    short["@vk_routed"] = 4l
    t |> equal(vk_fails(short, true), 1, "a note key is no dispatched kernel")
    let plain = lone_entry(blinds) $(e) => !key_exists(picks, e)
    var inscope hit <- clone_to_move(floor)
    hit[plain] = 2l
    t |> equal(vk_fails(hit, false), 1, "a dispatched blind entry fails: {plain}")
    var inscope served <- clone_to_move(floor)
    for (k in keys(picks)) {
        served[k] = 1l
    }
    served["@vk_routed"] = 4l
    t |> equal(vk_fails(served, false), length(picks), "no pick holds: every conditional entry dispatched fails")
    served["@vk_int8_columns"] = 1l
    t |> equal(vk_fails(served, false), 0, "the pick holds: every conditional entry may dispatch")
}
