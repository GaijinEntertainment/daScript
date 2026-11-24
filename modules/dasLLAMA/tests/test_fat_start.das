options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math   // the knob getters/setters the snapshot records
require dasllama/dasllama_fat_start
require dasllama/dasllama_tune   // box_profile_path
require llvm/daslib/llvm_tune    // set_tune_manifest_runtime_path, tune_sidecar_merge
require daslib/json_boost
require daslib/jobque_boost   // with_job_que - the snapshot's lane count and the race both need a queue
require daslib/fio
require daslib/env_registry
require strings
require _compares   // full_suite_only
require _child_run   // the per-box file's policy stand-down reads the tune policy at context init, so it is a child's to show
require ../performance/box_ident.das   // probe_cpu_brand: this box's CPU, the exe key's
require dasllama/dasllama_gpu_knee   // the split rule and the pricing both homes' knee races share
require ?das_metal dasllama/dasllama_metal_kernels   // metal_knee_tiles: a Metal rung's grid
require ?das_metal dasllama/dasllama_metal_prefill   // metal_devw_race_grids, METAL_N32_ROWS: the dev-W and one-way races' candidates
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: whether the device the Vulkan knee race ran on serves the cm2 s column
require ?cuda cuda/das_cuda_boost   // cuda_device_bytes_allocated: the CUDA knee race's planes come back
require dasllama/dasllama_env   // dasllama_home_dir_from - the box file's path under a child's redirected home

// The runtime section's writer and first-start race (dasllama_fat_start), model-free: the
// snapshot carries the crowns and the knobs; the writer writes the app sidecar's runtime section
// and the checked apply reads it back. The cells that run a race itself skip outside the full suite
// (DASLLAMA_PARITY_FULL=1): the first-start race and the Metal knee race on a das_metal build - the
// twins and knees race their whole ladders, past the test wall - and the box file's race with its two
// children on every box; without a Metal device the knee race's cell skips. The box-profile apply fires an installed race hook exactly when
// no section of this box's own exists; and outside a fat exe the fat hook does nothing. The
// sidecar path is redirected into a temp directory for every case, so the app's real sidecar is
// never touched.

def private with_temp_sidecar(t : T?; blk : block<(path : string) : void>) {
    var terr = ""
    let dir = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let path = path_join(dir, "dasllama_test_fat_start_{get_time_usec(0l)}.tune.json")
    remove(path)
    set_tune_manifest_runtime_path(path)
    invoke(blk, path)
    set_tune_manifest_runtime_path("")
    remove(path)
}

[test(description = "`dasllama_runtime_snapshot` carries the crown list as metal_tensor, the current (not default) q8_token_block value, the lane count as threads, and the Metal prefill constants")]
def test_runtime_snapshot_carries_the_crowns_and_knobs(t : T?) {
    with_job_que() {
        let saved = get_q8_token_block()
        set_q8_token_block(192l)
        var js = dasllama_runtime_snapshot("mulmm_q8,attn_qkmm")
        t |> equal(read_json_field(js, "metal_tensor", ""), "mulmm_q8,attn_qkmm", "the crown list rides as metal_tensor")
        t |> equal(read_json_field(js, "q8_token_block", -1l), 192l, "the snapshot records the current knob, not a default")
        t |> success(read_json_field(js, "threads", -1l) >= 1l, "threads is the total lane count")
        t |> success(read_json_field(js, "metal_tall_floor", -1l) >= 1l, "the Metal prefill constants ride along")
        delete_json(js)
        set_q8_token_block(saved)
    }
}

// a fat exe's first start runs before the application opens its job queue (the bench applies the
// box profile first, with_job_que later), so the snapshot cannot ask the queue for anything
[test(description = "The runtime snapshot builds with no job queue open, recording threads as 0 instead of throwing and still carrying the knob values")]
def test_the_snapshot_needs_no_job_queue(t : T?) {
    t |> run("the runtime snapshot is written with no job queue, its lanes receipt 0") @(t : T?) {
        var snap = dasllama_runtime_snapshot("")
        t |> success(snap != null, "the snapshot builds outside with_job_que")
        t |> equal(0, snap?["threads"] ?? -1, "no queue: the lanes receipt is 0, never a throw")
        t |> success((snap?["q8_token_block"] ?? -1) > 0, "the knob values ride along")
        delete_json(snap)
    }
}

//! every knob a first-start or box race writes - the crown list, the Metal knees, the Vulkan and CUDA short-window knees -
//! as a cell found them
struct private RacedKnobs {
    crowns : string
    q8, kq, rows, q8b, kqb, hmm, devw, vk, cud : int64
}

def private raced_knobs() : RacedKnobs
    => RacedKnobs(crowns = get_metal_tensor_crowns(), q8 = metal_split_k_tgs(), kq = metal_split_k_tgs_kq(),
        rows = metal_q8_n32_rows(), q8b = metal_split_k_tgs_base(), kqb = metal_split_k_tgs_kq_base(),
        hmm = metal_split_k_tgs_hmm(), devw = metal_devw_min_tgs(), vk = vk_split_k_tgs(), cud = cud_split_k_tgs())

def private put_back(knobs : RacedKnobs) {
    set_metal_tensor_crowns(knobs.crowns)
    set_metal_split_k_tgs(knobs.q8)
    set_metal_split_k_tgs_kq(knobs.kq)
    set_metal_q8_n32_rows(knobs.rows)
    set_metal_split_k_tgs_base(knobs.q8b)
    set_metal_split_k_tgs_kq_base(knobs.kqb)
    set_metal_split_k_tgs_hmm(knobs.hmm)
    set_metal_devw_min_tgs(knobs.devw)
    set_vk_split_k_tgs(knobs.vk)
    set_cud_split_k_tgs(knobs.cud)
}

//! the redirected sidecar at `path` after `wrote` reported a write of the runtime section: the file, its section's crown
//! list and knees, the kernels section left empty, and the checked apply reading it back
def private check_runtime_section(t : T?; path : string; wrote : bool) {
    t |> success(wrote, "the redirected sidecar is written")
    t |> success(fexist(path), "the sidecar exists at the redirected path: {path}")
    let text = fread(path)
    t |> success(find(text, "\"runtime\"") >= 0, "the file carries a runtime section:\n{text}")
    t |> success(find(text, "\"metal_tensor\"") >= 0, "the section names the crown list, empty or not")
    t |> success(find(text, "\"metal_split_k_tgs\"") >= 0 && find(text, "\"metal_split_k_tgs_kq\"") >= 0
        && find(text, "\"metal_q8_n32_rows\"") >= 0 && find(text, "\"metal_split_k_tgs_base\"") >= 0
        && find(text, "\"metal_split_k_tgs_kq_base\"") >= 0, "the section carries the split-K knees")
    var err = ""
    var doc = read_json(text, err)
    let ks = doc?["kernels"]
    t |> success(ks == null || !(ks.value is _object) || empty(ks as _object), "the kernels section stays empty - a first start races no kernel:\n{text}")
    delete_json(doc)
    apply_box_profile_runtime()
    t |> success(box_profile_runtime_applied(), "the checked apply reads the section it wrote (declined: '{box_profile_runtime_declined()}')")
}

[test(description = "The runtime section's writer writes a redirected sidecar whose runtime section names the crown list it was handed and the split-K knees with an empty kernels section, and the checked box-profile apply then reads it back")]
def test_the_runtime_section_writer_writes_what_the_apply_reads(t : T?) {
    with_job_que() {
        with_temp_sidecar(t) $(path) {
            let before = raced_knobs()
            check_runtime_section(t, path, dasllama_write_runtime_section("mulmm_q8"))
            t |> success(find(fread(path), "mulmm_q8") >= 0, "the section names the crown it was handed")
            put_back(before)
        }
    }
}

[test(description = "In the full suite the first-start race runs its real ladders and writes a redirected sidecar whose runtime section names the crown list and the split-K knees with an empty kernels section, which the checked box-profile apply then reads back")]
def test_first_start_race_writes_the_section_the_apply_reads(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (full_suite_only(t, "the first start's Metal crown and knee races run their whole ladders"))
    }
    with_job_que() {
        with_temp_sidecar(t) $(path) {
            let before = raced_knobs()
            check_runtime_section(t, path, dasllama_race_runtime_section())
            put_back(before)
        }
    }
}

//! The split-K knee race crowns this box's knees: on a Metal box each lands on its race's ladder and goes live in the knobs the
//! snapshot records; without one it races nothing and the knobs stand (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k)
[test(description = "On a Metal box the split-K knee race lands every knee, the n32 rows and the dev-W grid floor on a rung of its own race or off, and the snapshot records the raced K-quant knees; with no device every knee stands")]
def test_the_knee_race_sets_the_split_knees(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return if (full_suite_only(t, "the Metal knee race runs its whole ladder"))
    }
    let before = raced_knobs()
    let ran = dasllama_metal_knee_race()
    if (!ran) {
        t |> equal(metal_split_k_tgs(), before.q8, "no device: the q8 knee stands")
        t |> skip("no Metal device - the knee race ran nothing")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {   //! the knee race ran: a Metal device answered
        var inscope q8_rungs <- knee_candidates(KNEE_Q8_WIDTHS) $(n) => metal_knee_tiles(n)
        var inscope kq_rungs <- knee_candidates(KNEE_KQ_WIDTHS) $(n) => metal_knee_tiles(n)
        var inscope devw_grids <- metal_devw_race_grids()
        t |> success(metal_split_k_tgs() == 0l || has_value(q8_rungs, metal_split_k_tgs()), "the q8 knee is a rung of its race ({metal_split_k_tgs()})")
        t |> success(metal_split_k_tgs_kq() == 0l || has_value(kq_rungs, metal_split_k_tgs_kq()), "the K-quant knee is a rung of its race ({metal_split_k_tgs_kq()})")
        t |> success(metal_split_k_tgs_base() == 0l || has_value(q8_rungs, metal_split_k_tgs_base()), "the q8 base knee is a rung of its race ({metal_split_k_tgs_base()})")
        t |> success(metal_split_k_tgs_kq_base() == 0l || has_value(kq_rungs, metal_split_k_tgs_kq_base()), "the K-quant base knee is a rung of its race ({metal_split_k_tgs_kq_base()})")
        t |> success(metal_split_k_tgs_hmm() == 0l || has_value(q8_rungs, metal_split_k_tgs_hmm()), "the dev-W split knee is a rung of its race ({metal_split_k_tgs_hmm()})")
        t |> success(metal_devw_min_tgs() == 1048576l || has_value(devw_grids, metal_devw_min_tgs()),
            "the dev-W grid floor is a rung's grid, or off ({metal_devw_min_tgs()})")
        t |> success(metal_q8_n32_rows() == 0l || has_value(METAL_N32_ROWS, metal_q8_n32_rows()), "the one-way rows are a rung of their race ({metal_q8_n32_rows()})")
    }
    var js = dasllama_runtime_snapshot("")
    t |> equal(read_json_field(js, "metal_split_k_tgs_kq", -1l), metal_split_k_tgs_kq(), "the snapshot records the raced K-quant knee")
    t |> equal(read_json_field(js, "metal_split_k_tgs_kq_base", -1l), metal_split_k_tgs_kq_base(), "the snapshot records the raced K-quant base knee")
    delete_json(js)
    put_back(before)
}

//! whether this context's Vulkan device is up and serves the cm2 s column
def private vk_cm2_device() : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return g_gpu != null && g_gpu.coopmat_mode == COOPMAT_CM2
    } else {
        return false
    }
}

//! The Vulkan short window's knee race (`dasllama_vulkan_knee_race`, ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split): on a device
//! with the cm2 s column the knee lands on a rung of its race, or 0, and goes live in the knob the snapshot records; on any other
//! device, or with the GPU tier unasked, the knob stands. Which rung wins is the race's verdict, not this cell's.
[test(description = "On a Vulkan device with the cm2 s column the short-window knee race overwrites a marker knob with a rung of its race or 0, recorded in the snapshot; on any other device the marker stands")]
def test_the_vulkan_knee_race_sets_the_knee(t : T?) {
    let saved = vk_split_k_tgs()
    let marker = 77l   // a knob no rung offers: the race either overwrites it or leaves it standing
    set_vk_split_k_tgs(marker)
    let ran = dasllama_vulkan_knee_race()
    if (!(ran && vk_cm2_device())) {
        t |> equal(vk_split_k_tgs(), marker, "no cm2 s column to race (tier up: {ran}): the knee stands")
        set_vk_split_k_tgs(saved)
        t |> skip("no Vulkan device with the cm2 s column under an asked GPU tier - the knee race ran nothing")
        return
    }
    var inscope rungs <- knee_candidates(KNEE_Q8_WIDTHS) $(d) => 2l * ((d + 127l) / 128l)
    let knee = vk_split_k_tgs()
    t |> success(knee == 0l || has_value(rungs, knee), "the raced knee is a rung of its race, or 0 ({knee})")
    var js = dasllama_runtime_snapshot("")
    t |> equal(read_json_field(js, "vk_split_k_tgs", -1l), knee, "the snapshot records the raced knee")
    delete_json(js)
    set_vk_split_k_tgs(saved)
}

//! The CUDA short window's knee race (`dasllama_cuda_knee_race`, the Vulkan race's twin on the shared rung loop): under the CUDA
//! home's ask on a device with the q8 s column the knee lands on a rung of its race, or 0, goes live in the knob the snapshot
//! records, and a second race allocates nothing the first left - every width's planes came back through the rung loop's close;
//! with the home unasked or no device, the knob stands. Which rung wins is the race's verdict, not this cell's.
[test(description = "Under the CUDA home the knee race lands the knob on a rung of its race or 0, recorded in the snapshot, and a second race leaves device bytes allocated unchanged; with no CUDA device a marker knob stands")]
def test_the_cuda_knee_race_sets_the_knee(t : T?) {
    let saved = cud_split_k_tgs()
    let marker = 77l   // a knob no rung offers: the race either overwrites it or leaves it standing
    set_cud_split_k_tgs(marker)
    let ran = dasllama_cuda_knee_race()
    if (!ran) {
        t |> equal(cud_split_k_tgs(), marker, "the CUDA home unasked or no device: the knee stands")
        set_cud_split_k_tgs(saved)
        t |> skip("no CUDA device under an asked CUDA home (DASLLAMA_GPU=1 DASLLAMA_CUDA=1) - the knee race ran nothing")
        return
    }
    static_if (typeinfo builtin_module_exists(cuda)) {   // the knee race ran: a CUDA device answered
        var inscope rungs <- knee_candidates(KNEE_Q8_WIDTHS) $(d) => (d + 127l) / 128l
        let knee = cud_split_k_tgs()
        t |> success(knee == 0l || has_value(rungs, knee), "the raced knee is a rung of its race, or 0 ({knee})")
        var js = dasllama_runtime_snapshot("")
        t |> equal(read_json_field(js, "cud_split_k_tgs", -1l), knee, "the snapshot records the raced knee")
        delete_json(js)
        let held = cuda_device_bytes_allocated()
        t |> success(dasllama_cuda_knee_race(), "the race runs again on the same device")
        t |> equal(cuda_device_bytes_allocated(), held, "... and holds no device bytes the first race left: every width's planes came back")
    }
    set_cud_split_k_tgs(saved)
}

// the fake race the hook-wiring cell installs: it mints one knob and counts its firings
var private g_fake_race_fired = 0

def private fake_race() : bool {
    g_fake_race_fired++
    let noKernels : table<string; string>
    var runtime = JV((q8_token_block = 224))
    return tune_sidecar_merge(noKernels, runtime)
}

[test(description = "The box-profile apply fires an installed race hook exactly once for an absent sidecar, reads the knob the race minted, and never re-races once the box's own section exists")]
def test_the_apply_fires_the_race_hook_only_for_a_missing_section(t : T?) {
    with_temp_sidecar(t) $(_path) {
        let savedBlock = get_q8_token_block()
        g_fake_race_fired = 0
        let old = set_runtime_race_hook(@@fake_race)
        // absent sidecar: the hook mints the section and the same apply reads it
        apply_box_profile_runtime()
        t |> equal(g_fake_race_fired, 1, "an absent sidecar fires the race once")
        t |> success(box_profile_runtime_applied(), "the apply reads what the race wrote (declined: '{box_profile_runtime_declined()}')")
        t |> equal(get_q8_token_block(), 224l, "the minted knob is live")
        // a sidecar with a section of this box's own: no race
        apply_box_profile_runtime()
        t |> equal(g_fake_race_fired, 1, "a present section never re-races")
        set_runtime_race_hook(old)
        set_q8_token_block(savedBlock)
    }
}

// restoring a null previous hook must disarm the race, or the next apply on a missing section
// would invoke a null function
[test(description = "Restoring a null race hook after a fake one disarms the race: the apply on an absent section declines with absent instead of invoking a null function")]
def test_restoring_a_null_hook_disarms_the_race(t : T?) {
    t |> run("a null hook restored after a fake one leaves the apply declining, never invoking") @(t : T?) {
        let orig = set_runtime_race_hook(@@fake_race)
        let none : function<() : bool>
        set_runtime_race_hook(none)   // a null goes in: disarmed, whatever was armed before
        with_temp_sidecar(t) $(path : string) {
            apply_box_profile_runtime_checked(path)
            t |> equal("absent", box_profile_runtime_declined(), "an absent section with no hook armed declines and races nothing")
        }
        set_runtime_race_hook(orig)
    }
}

[test(description = "`tune_policy_keeps_crowns` keeps the Metal crowns on the default and every named kernel policy but `reference` and `fallback`, the crownless portable arm")]
def test_tune_policy_keeps_crowns(t : T?) {
    t |> success(tune_policy_keeps_crowns(""), "the default policy keeps the crowns")
    t |> success(tune_policy_keeps_crowns("tuned"), "a named policy keeps them")
    t |> success(!tune_policy_keeps_crowns("reference"), "reference keeps none")
    t |> success(!tune_policy_keeps_crowns("fallback"), "fallback keeps none")
}

[test(description = "Outside a fat exe `dasllama_fat_first_start` declines and writes no sidecar")]
def test_the_fat_hook_is_inert_outside_a_fat_exe(t : T?) {
    with_temp_sidecar(t) $(path) {
        t |> success(!dasllama_fat_first_start(), "this process is not a fat exe, so the fat hook declines")
        t |> success(!fexist(path), "and writes nothing")
    }
}

// the fake placement the hook-wiring cell installs: it counts its firings and points the apply at
// the sidecar the cell staged, so the apply reading that sidecar proves the hook ran first
var private g_fake_place_fired = 0
var private g_fake_place_target = ""

def private fake_place() {
    g_fake_place_fired++
    set_tune_manifest_runtime_path(g_fake_place_target)
}

//! a box file's path under a temp home of its own
def private box_file_scratch() : string {
    var terr = ""
    return path_join(path_join(temp_directory(terr), "dasllama_test_box_home_{get_time_usec(0l)}"), "box.tune.json")
}

//! the box file at `path` after `wrote` reported its write: valid for this run's kernels, no kernels section, read by the
//! apply; the file is removed
def private check_box_file(t : T?; path : string; wrote : bool) {
    t |> success(wrote, "the file is written: {path}")
    t |> success(dasllama_box_runtime_valid(path, dasllama_kernel_sources_digest()), "the file is valid for this run")
    let text = fread(path)
    t |> success(find(text, "\"kernels\"") < 0, "no kernels section is written to the box file:\n{text}")
    apply_box_profile_runtime_at(path)
    t |> success(box_profile_runtime_applied(), "the apply reads the section (declined: '{box_profile_runtime_declined()}')")
    remove(path)
}

//! one kernel source set under `root`: an engine folder with two Metal driver files and an unrelated one, an emitter folder
//! with one; `body` is the second Metal file's text
def private write_kernel_set(root, body : string) : tuple<engine : string; emitter : string> {
    let engine = path_join(root, "engine")
    let emitter = path_join(root, "emitter")
    mkdir_rec(engine)
    mkdir_rec(emitter)
    fwrite(path_join(engine, "dasllama_metal_a.das"), "options gen2\nlet A = 1\n")
    fwrite(path_join(engine, "dasllama_metal_b.das"), body)
    fwrite(path_join(engine, "dasllama_other.das"), "options gen2\n")
    fwrite(path_join(emitter, "msl.das"), "options gen2\nlet E = 3\n")
    return (engine = engine, emitter = emitter)
}

[test(description = "The kernel sources' digest reads their content: two copies of one source set written at different times digest the same, one changed byte changes it, and a touched mtime leaves it")]
def test_kernel_digest_reads_content(t : T?) {
    var terr = ""
    let tmp = temp_directory(terr)
    t |> success(terr == "", "temp_directory resolves")
    let root = path_join(tmp, "dasllama_test_kernel_digest_{get_time_usec(0l)}")
    let body = "options gen2\nlet B = 2\n"
    let a = write_kernel_set(path_join(root, "a"), body)
    let b = write_kernel_set(path_join(root, "b"), body)
    let old = stat(path_join(get_das_root(), "daslib/fio.das")).mtime
    for (dir in [a.engine, a.emitter]) {
        fio::dir(dir) $(fname) {
            if (fname |> ends_with(".das")) {
                var err = ""
                let moved = set_mtime(path_join(dir, fname), old, err)
                t |> success(moved, "the copy's mtime moves back ({err})")
            }
        }
    }
    let f_a = path_join(a.engine, "dasllama_metal_b.das")
    let f_b = path_join(b.engine, "dasllama_metal_b.das")
    t |> success(stat(f_a).mtime != stat(f_b).mtime,
        "the two copies carry different mtimes (the compare's precondition)")
    let da = dasllama_kernel_sources_digest_of(a.engine, a.emitter)
    t |> success(!empty(da), "a source set digests to a name")
    t |> equal(dasllama_kernel_sources_digest_of(b.engine, b.emitter), da,
        "two copies of one source set digest the same")
    fwrite(f_b, "options gen2\nlet B = 3\n")
    t |> success(dasllama_kernel_sources_digest_of(b.engine, b.emitter) != da,
        "one changed byte changes the digest (the control)")
    fwrite(f_b, body)
    var err = ""
    let touched = set_mtime(f_b, old, err)
    t |> success(touched, "the touch lands ({err})")
    t |> equal(dasllama_kernel_sources_digest_of(b.engine, b.emitter), da,
        "a touched mtime leaves the digest")
    t |> equal(dasllama_kernel_sources_digest_of(path_join(root, "none"), b.emitter), "",
        "no engine sources: no digest")
    let t0 = ref_time_ticks()
    let live = dasllama_kernel_sources_digest()
    let ms = double(get_time_usec(t0)) / 1000.0lf
    to_log(LOG_INFO, "the engine's kernel sources digest to {live} in {ms} ms\n")
    rmdir_rec(root)
}

[test(description = "A kernel source the digest cannot open - a link to no file - is left out of the digest, the set digesting as it does without it, where a readable file under that name changes it", timeout = 1200)]
def test_kernel_digest_skips_unreadable(t : T?) {
    if (get_platform_name() == "windows") {
        t |> skip("the unreadable source is a dangling symlink, which `ln -s` makes on a POSIX box")
        return
    }
    let root = path_join(test_temp_dir(), "kernel_digest_unreadable")
    let a = write_kernel_set(root, "options gen2\nlet B = 2\n")
    let clean = dasllama_kernel_sources_digest_of(a.engine, a.emitter)
    let gone = path_join(a.engine, "dasllama_metal_gone.das")
    let rc = unsafe(popen("ln -s \"{path_join(a.engine, "nowhere.txt")}\" \"{gone}\" 2>&1") $(f) {
        let _drain = fread(f)
    })
    t |> equal(rc, 0, "the dangling link is made")
    t |> success(!fexist(gone), "the link reaches no file")
    t |> equal(dasllama_kernel_sources_digest_of(a.engine, a.emitter), clean,
        "an unreadable source is left out of the digest")
    remove(gone)
    fwrite(gone, "options gen2\nlet G = 4\n")
    t |> success(dasllama_kernel_sources_digest_of(a.engine, a.emitter) != clean,
        "the control: a readable file under that name changes it")
    rmdir_rec(root)
}

//! the per-box runtime file: its path under the home's tune/, what makes it valid for this run (this box, these kernels, a
//! runtime section), the writer writing one the apply reads, and the hook standing down under an explicit manifest
[test(description = "The per-box runtime file sits at a stable box-keyed path under tune, is valid only for this box, these kernels and a runtime section, is written by its writer in a form the apply reads, and stands down under an explicit manifest")]
def test_box_runtime_file(t : T?) {
    t |> run("the per-box path sits under tune/ with a key that is this box's and stable") @(t : T?) {
        let p = dasllama_box_runtime_path_from("H/.dasllama")
        t |> success(p |> starts_with(path_join(path_join("H/.dasllama", "tune"), "box-")), "under the home's tune/: {p}")
        t |> success(p |> ends_with(".tune.json"), "a .tune.json file")
        t |> equal(p, dasllama_box_runtime_path_from("H/.dasllama"), "the key is stable across calls")
        t |> success(!empty(dasllama_kernel_sources_digest()), "the Metal driver sources beside the engine digest to a name")
    }
    t |> run("valid = this box, these kernels, a runtime section; anything else is not") @(t : T?) {
        var terr = ""
        let dir = temp_directory(terr)
        t |> success(terr == "", "temp_directory resolves")
        let path = path_join(dir, "dasllama_test_box_runtime_{get_time_usec(0l)}.json")
        let digest = dasllama_kernel_sources_digest()
        let box = tune_box_identity()
        fwrite(path, "\{\"kernel_library\":\"{digest}\",\"provenance\":\{\"box\":\"{box}\"},\"runtime\":\{\"q8_token_block\":224}}")
        t |> success(dasllama_box_runtime_valid(path, digest), "this box, these kernels, a section: valid")
        t |> success(!dasllama_box_runtime_valid(path, "{digest}x"), "other kernels: not valid")
        fwrite(path, "\{\"kernel_library\":\"{digest}\",\"provenance\":\{\"box\":\"other|box|id|0|cpu\"},\"runtime\":\{\"q8_token_block\":224}}")
        t |> success(!dasllama_box_runtime_valid(path, digest), "another box's file: not valid")
        fwrite(path, "\{\"kernel_library\":\"{digest}\",\"provenance\":\{\"box\":\"{box}\"},\"kernels\":\{}}")
        t |> success(!dasllama_box_runtime_valid(path, digest), "no runtime section: not valid")
        remove(path)
        t |> success(!dasllama_box_runtime_valid(path, digest), "no file: not valid")
    }
    t |> run("the writer writes a file the apply reads, valid for these kernels, with and without the knees") @(t : T?) {
        with_job_que() {
            let before = raced_knobs()
            for (knees in [false, true]) {
                let path = box_file_scratch()
                let wrote = dasllama_box_runtime_write(path, dasllama_kernel_sources_digest(), "mulmm_q8", knees)
                let text = fread(path)
                t |> equal(find(text, "\"metal_split_k_tgs\"") >= 0, knees, "the section carries the knees only where a knee race ran (knees {knees}):\n{text}")
                t |> success(find(text, "mulmm_q8") >= 0, "... and the crown list it was handed")
                check_box_file(t, path, wrote)
            }
            put_back(before)
        }
    }
    t |> run("under an explicit manifest the hook stands down") @(t : T?) {
        with_temp_sidecar(t) $(_path) {
            t |> equal("", dasllama_box_runtime_file(), "a chosen manifest keeps the process off the box file")
        }
    }
}

[test(description = "The per-box runtime file's race writes a file the apply reads with no kernels section, and a child with no manifest races into its home's box file while a child on the reference policy writes nothing")]
def test_box_runtime_file_is_raced(t : T?) {
    return if (full_suite_only(t, "the box file's race runs the Metal ladders, and each of its two children compiles the engine"))
    t |> run("the race writes a file the apply reads, valid for these kernels") @(t : T?) {
        with_job_que() {
            let before = raced_knobs()
            let path = box_file_scratch()
            check_box_file(t, path, dasllama_box_runtime_race(path, dasllama_kernel_sources_digest()))
            put_back(before)
        }
    }
    t |> run("a run with no manifest races into the home's box file; the reference policy stands down") @(t : T?) {
        var terr = ""
        let home = path_join(temp_directory(terr), "dasllama_box_home_{get_time_usec(0l)}")
        mkdir_rec(home)
        let want = dasllama_box_runtime_path_from(dasllama_home_dir_from(home))
        var no_manifest <- [(name = "HOME", value = home), (name = "USERPROFILE", value = home), (name = "DAS_TUNE_MANIFEST", value = ""),
            (name = "DAS_TUNE_FORCE", value = "")]   // the runner hands its children their manifest as DAS_TUNE_FORCE
        var out : string
        let rc = run_child(child_cmd("_box_runtime_root.das", no_manifest), out)
        t |> equal(0, rc, "the child ran (rc {rc}):\n{out}")
        t |> success(find(out, "BOX_FILE=[{want}] exists=true") >= 0, "with no manifest the run reads the box file under its home, raced and written first:\n{out}")
        t |> success(stat(want).is_valid, "the file is on disk: {want}")
        remove(want)
        no_manifest |> push((name = "DAS_TUNE_POLICY", value = "reference"))
        let rc2 = run_child(child_cmd("_box_runtime_root.das", no_manifest), out)
        t |> equal(0, rc2, "the child ran (rc {rc2}):\n{out}")
        t |> success(find(out, "BOX_FILE=[] exists=false") >= 0, "on the reference policy the hook stands down and writes nothing:\n{out}")
        t |> success(!stat(want).is_valid, "no box file was written under the reference policy")
        rmdir_rec(home)
    }
}

[test(description = "The fat sidecar lives under the per-user tune directory, is adopted from beside the exe once and never overwritten, its placement hook runs before the default apply reads the path, and only a chosen manifest counts as explicit")]
def test_fat_sidecar_lives_per_user(t : T?) {
    t |> run("the per-user path keeps the exe's sidecar name under tune/") @(t : T?) {
        t |> equal(dasllama_fat_sidecar_path("H/.dasllama", "/opt/dasllama/dasllama-server.tune.json"),
            path_join(path_join("H/.dasllama", "tune"), "dasllama-server.tune.json"))
    }
    t |> run("a sidecar beside the exe is copied once; one already home is never overwritten") @(t : T?) {
        var terr = ""
        let dir = path_join(temp_directory(terr), "dasllama_adopt_{get_time_usec(0l)}")
        let beside = path_join(dir, "exe/app.tune.json")
        let home_path = path_join(dir, "home/tune/app.tune.json")
        mkdir_rec(dir_name(beside))
        dasllama_adopt_sidecar(beside, home_path)
        t |> success(stat(dir_name(home_path)).is_dir, "the tune directory is made")
        t |> success(!stat(home_path).is_valid, "nothing to adopt: nothing copied")
        fwrite(beside, "old")
        dasllama_adopt_sidecar(beside, home_path)
        t |> equal(fread(home_path), "old")
        fwrite(beside, "older")
        dasllama_adopt_sidecar(beside, home_path)
        t |> equal(fread(home_path), "old", "the home copy wins")
        rmdir_rec(dir)
    }
    t |> run("the engine registers the placement, and outside a fat exe it leaves the sidecar where it was") @(t : T?) {
        let registered = set_sidecar_place_hook(@@fake_place)
        set_sidecar_place_hook(registered)
        t |> success(registered == @@dasllama_place_fat_sidecar, "dasllama_fat_start's [init] installs the placement hook")
        let none_box : function<() : string>
        let box_hook = set_box_runtime_hook(none_box)   //! disarmed for this apply: an unmanifested test process must not race into the user's box file
        t |> success(box_hook == @@dasllama_box_runtime_file, "dasllama_fat_start's [init] installs the box-runtime hook")
        let before = tune_manifest_path()
        apply_box_profile_runtime()
        t |> equal(tune_manifest_path(), before)
        set_box_runtime_hook(box_hook)
    }
    t |> run("the default apply runs the placement hook before it reads the path") @(t : T?) {
        with_temp_sidecar(t) $(path) {
            let savedBlock = get_q8_token_block()
            let noKernels : table<string; string>
            var runtime = JV((q8_token_block = 232))
            t |> success(tune_sidecar_merge(noKernels, runtime), "staged a section of this box's own")
            set_tune_manifest_runtime_path("")   // the apply's own default would miss the staged file
            g_fake_place_fired = 0
            g_fake_place_target = path
            let old = set_sidecar_place_hook(@@fake_place)
            apply_box_profile_runtime()
            set_sidecar_place_hook(old)
            t |> equal(g_fake_place_fired, 1)
            t |> equal(get_q8_token_block(), 232l, "the apply read the sidecar the hook placed")
            set_q8_token_block(savedBlock)
        }
    }
    t |> run("with no placement hook the default apply reads where the framework points") @(t : T?) {
        with_temp_sidecar(t) $(_path) {
            let none : function<() : void>
            let old = set_sidecar_place_hook(none)
            apply_box_profile_runtime()
            set_sidecar_place_hook(old)
            t |> equal(box_profile_runtime_declined(), "absent", "an absent sidecar, and no hook to call")
        }
    }
    t |> run("a chosen sidecar location is explicit; the default beside the app is not") @(t : T?) {
        let env_manifest = env_is_set("DAS_TUNE_MANIFEST") ? env_value_of("DAS_TUNE_MANIFEST") : ""
        let env_chose_manifest = !empty(env_manifest)   // the suite runner hands a stocked box its manifest this way
        t |> equal(tune_manifest_explicit(), env_chose_manifest, "nothing chosen in this process - explicit only where the environment chose (DAS_TUNE_MANIFEST='{env_manifest}')")
        set_tune_manifest_runtime_path("chosen.tune.json")
        t |> success(tune_manifest_explicit(), "the runtime override chooses it")
        t |> equal(tune_manifest_path(), "chosen.tune.json", "and it is the path read")
        set_tune_manifest_runtime_path("")
        t |> equal(tune_manifest_explicit(), env_chose_manifest, "cleared (DAS_TUNE_MANIFEST='{env_manifest}')")
        if (env_chose_manifest) {
            t |> equal(tune_manifest_path(), env_manifest, "cleared, the environment's manifest is the path read again")
        }
    }
}

//! The rule both GPU homes serve and the pricing their knee races share (`dasllama_gpu_knee`): the split engages at half the
//! knee, doubles to eight while the grid fits, keeps each share whole chunks - even, or the last one short - and the pickers
//! crown the cheapest priced knob or cutoff, each standing down under a 1% gain
[test(description = "The shared GPU split-K rule engages at half the knee, doubles ways to eight while the grid fits, keeps shares in whole chunks, and the knee and cutoff pickers crown the cheapest priced rung, standing down under a 1 percent gain")]
def test_the_shared_knee_rule_and_pricing(t : T?) {
    let metal = SplitKnee(tgs = 256l, min_k = 128l)
    t |> success(split_k_on(128l, 2048l, metal) && !split_k_on(129l, 2048l, metal), "the split engages at half the knee")
    t |> equal(split_k_ways(16l, 2048l, metal), 8l, "a small grid splits eight ways")
    t |> equal(split_k_ways(64l, 2048l, metal), 4l, "the count doubles only while the grid times it fits the knee")
    t |> equal(split_k_ways(16l, 384l, metal), 2l, "an even share stops where the next would leave its 64-wide chunks")
    let vk = SplitKnee(tgs = 128l, min_k = 256l, align = 256l, exact = false)
    t |> equal(split_k_ways(9l, 6912l, vk), 4l, "a short last chunk lets an odd 256-step K split (gemma-3-1b's down)")
    t |> equal(split_k_share(6912l, 4l, vk), 1792l, "... its shares cm2_chunk's")
    t |> equal(split_k_share(6912l, 2l, SplitKnee(align = 256l)), 0l, "the even rule refuses that K")
    var inscope rungs <- [KneeRung(tiles = 8l, kdim = 2048l, base = 30.0lf, split = fixed_array(18.0lf, 11.0lf, 8.0lf)),
        KneeRung(tiles = 128l, kdim = 2048l, base = 47.0lf, split = fixed_array(52.0lf, 49.0lf, 51.0lf))]
    t |> equal(knee_pick(rungs, SplitKnee(min_k = 256l, align = 256l, exact = false), "TEST_KNEE"), 64l,
        "the least knee that splits the small grid eight ways and leaves the wide one whole")
    var inscope cuts <- [CutRung(x = 20l, base = 20.0lf, alt = 28.0lf), CutRung(x = 68l, base = 20.0lf, alt = 19.0lf),
        CutRung(x = 128l, base = 6.0lf, alt = 4.0lf)]
    t |> equal(knee_cut_pick(cuts, true, 1048576l, "TEST_CUT"), 68l, "the cutoff takes the other form from the first grid it wins on")
    var inscope flat <- [CutRung(x = 20l, base = 20.0lf, alt = 20.1lf)]
    t |> equal(knee_cut_pick(flat, true, 1048576l, "TEST_CUT"), 1048576l, "no 1% gain keeps every rung on the current form")
}

[test(description = "The test child's exe key takes the CPU the exe is built for: two CPU brands give two keys, one brand one key, this box's key is its own brand's, and the libraries it reads leave out the folder's dot entries.")]
def test_child_exe_key_takes_the_cpu(t : T?) {
    t |> success(child_exe_key_for("AMD EPYC 7663 56-Core Processor") != child_exe_key_for("AMD EPYC 9654 96-Core Processor"),
        "an exe another CPU built is a stale one")
    t |> equal(child_exe_key_for("Apple M5 Max"), child_exe_key_for("Apple M5 Max"), "the control: one CPU, one key")
    t |> equal(child_exe_key(), child_exe_key_for(probe_cpu_brand()), "this box's key is its CPU brand's")
    var inscope libs <- child_lib_names()
    t |> success(find_index(libs, ".") < 0 && find_index(libs, "..") < 0,
        "the key reads no `.` or `..` of lib/: their times move with the tree's root, a new key a session")
}
