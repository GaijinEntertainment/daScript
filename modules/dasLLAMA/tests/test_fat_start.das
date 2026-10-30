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
require _child_run   // the per-box file's policy stand-down reads the tune policy at context init, so it is a child's to show
require dasllama/dasllama_gpu_knee   // the split rule and the pricing both homes' knee races share
require ?das_metal dasllama/dasllama_metal_kernels   // metal_knee_tiles: a Metal rung's grid
require ?das_metal dasllama/dasllama_metal_prefill   // metal_devw_race_grids, METAL_N32_ROWS: the dev-W and one-way races' candidates
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: whether the device the Vulkan knee race ran on serves the cm2 s column
require ?cuda cuda/das_cuda_boost   // cuda_device_bytes_allocated: the CUDA knee race's planes come back
require dasllama/dasllama_env   // dasllama_home_dir_from - the box file's path under a child's redirected home

// The runtime section's writer and first-start race (dasllama_fat_start), model-free: the
// snapshot carries the crowns and the knobs; the race writes the app sidecar's runtime section
// and the checked apply reads it back (on a Metal box the twins race, ~15 s; elsewhere the
// section records the defaults, and the split-K knee race's cell skips); the box-profile apply fires an installed race hook exactly when
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

[test]
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
[test]
def test_the_snapshot_needs_no_job_queue(t : T?) {
    t |> run("the runtime snapshot is written with no job queue, its lanes receipt 0") @(t : T?) {
        var snap = dasllama_runtime_snapshot("")
        t |> success(snap != null, "the snapshot builds outside with_job_que")
        t |> equal(0, snap?["threads"] ?? -1, "no queue: the lanes receipt is 0, never a throw")
        t |> success((snap?["q8_token_block"] ?? -1) > 0, "the knob values ride along")
        delete_json(snap)
    }
}

[test]
def test_first_start_race_writes_the_section_the_apply_reads(t : T?) {
    with_job_que() {
        with_temp_sidecar(t) $(path) {
            let crowns = get_metal_tensor_crowns()
            t |> success(dasllama_race_runtime_section(), "the race writes the redirected sidecar")
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
            t |> success(box_profile_runtime_applied(), "the checked apply reads the section the race wrote (declined: '{box_profile_runtime_declined()}')")
            set_metal_tensor_crowns(crowns)
        }
    }
}

//! The split-K knee race crowns this box's knees: on a Metal box each lands on its race's ladder and goes live in the knobs the
//! snapshot records; without one it races nothing and the knobs stand (ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k)
[test]
def test_the_knee_race_sets_the_split_knees(t : T?) {
    let q8 = metal_split_k_tgs()
    let kq = metal_split_k_tgs_kq()
    let rows = metal_q8_n32_rows()
    let q8b = metal_split_k_tgs_base()
    let kqb = metal_split_k_tgs_kq_base()
    let hmm = metal_split_k_tgs_hmm()
    let devw = metal_devw_min_tgs()
    let ran = dasllama_metal_knee_race()
    if (!ran) {
        t |> equal(metal_split_k_tgs(), q8, "no device: the q8 knee stands")
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
    set_metal_split_k_tgs(q8)
    set_metal_split_k_tgs_kq(kq)
    set_metal_q8_n32_rows(rows)
    set_metal_split_k_tgs_base(q8b)
    set_metal_split_k_tgs_kq_base(kqb)
    set_metal_split_k_tgs_hmm(hmm)
    set_metal_devw_min_tgs(devw)
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
[test]
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
[test]
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

[test]
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
[test]
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

[test]
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

//! the per-box runtime file: its path under the home's tune/, what makes it valid for this run (this box, these kernels, a
//! runtime section), the race writing one the apply reads, and the hook standing down under an explicit manifest
[test]
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
    t |> run("the race writes a file the apply reads, valid for these kernels") @(t : T?) {
        with_job_que() {
            var terr = ""
            let dir = temp_directory(terr)
            let path = path_join(path_join(dir, "dasllama_test_box_home_{get_time_usec(0l)}"), "box.tune.json")
            let digest = dasllama_kernel_sources_digest()
            let crowns = get_metal_tensor_crowns()
            t |> success(dasllama_box_runtime_race(path, digest), "the race writes the file: {path}")
            t |> success(dasllama_box_runtime_valid(path, digest), "the file it wrote is valid for this run")
            let text = fread(path)
            t |> success(find(text, "\"kernels\"") < 0, "no kernels section is written to the box file:\n{text}")
            apply_box_profile_runtime_at(path)
            t |> success(box_profile_runtime_applied(), "the apply reads the section (declined: '{box_profile_runtime_declined()}')")
            set_metal_tensor_crowns(crowns)
            remove(path)
        }
    }
    t |> run("under an explicit manifest the hook stands down") @(t : T?) {
        with_temp_sidecar(t) $(_path) {
            t |> equal("", dasllama_box_runtime_file(), "a chosen manifest keeps the process off the box file")
        }
    }
    t |> run("a run with no manifest races into the home's box file; the reference policy stands down") @(t : T?) {
        var terr = ""
        let home = path_join(temp_directory(terr), "dasllama_box_home_{get_time_usec(0l)}")
        mkdir_rec(home)
        let want = dasllama_box_runtime_path_from(dasllama_home_dir_from(home))
        var no_manifest <- [(name = "HOME", value = home), (name = "USERPROFILE", value = home), (name = "DAS_TUNE_MANIFEST", value = "")]
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

[test]
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
[test]
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
