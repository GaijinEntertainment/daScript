options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_common
require dasllama/dasllama_math   // the knob getters/setters the snapshot records
require dasllama/dasllama_fat_start
require dasllama/dasllama_tune   // box_profile_path
require llvm/daslib/llvm_tune    // set_tune_manifest_runtime_path, tune_sidecar_merge
require daslib/json_boost
require daslib/jobque_boost   // with_job_que - the snapshot's lane count and the race both need a queue
require daslib/fio
require strings

// The runtime section's writer and first-start race (dasllama_fat_start), model-free: the
// snapshot carries the crowns and the knobs; the race writes the app sidecar's runtime section
// and the checked apply reads it back (on a Metal box the twins race, ~15 s; elsewhere the
// section records the defaults); the box-profile apply fires an installed race hook exactly when
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

[test]
def test_the_fat_hook_is_inert_outside_a_fat_exe(t : T?) {
    with_temp_sidecar(t) $(path) {
        t |> success(!dasllama_fat_first_start(), "this process is not a fat exe, so the fat hook declines")
        t |> success(!fexist(path), "and writes nothing")
    }
}
