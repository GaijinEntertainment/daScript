options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 — umbrella fires each arch [init] registration; used only inside the vulkan static_if half
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if half
require dasllama/dasllama_config   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if half
require dasllama/dasllama_math
require dasllama/dasllama_gpu_resident   // vulkan_bake_* — the offline flavor bake under test
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if half
require _model_tier

// The OFFLINE vulkan bake (plan-as-data): the dry tier runs the load walk's accept/refuse
// arithmetic against the config's numbers with NO device — the converter's `-f vulkan` rail.
// Drive through run.das --suite image-vulkan (it arms DASLLAMA_GPU + a small VRAM budget);
// unarmed contexts skip cleanly, so the file is safe in any suite.

//! the minted twin against the eager dry bake's - the walk over the prepared RAM planes, the mint's reference: the same
//! entries in the same order, every entry's device bytes equal, a poisoned byte in the reference missing the compare
def private mint_matches_eager_bake(t : T?; minted : Model; path : string) {
    vulkan_bake_begin()
    var eager <- load_model_(path, QuantMode.q8)
    t |> success(vulkan_bake_take(eager), "the eager dry bake collected its plan")
    t |> equal(long_length(eager.vkplan), long_length(minted.vkplan), "the mint's plan has the eager bake's entry count")
    var shape_mism = 0
    var byte_mism = 0
    var compared = 0
    for (e, i in minted.vkplan, count()) {
        if (i >= length(eager.vkplan)) {
            break
        }
        let f = eager.vkplan[i]
        if (e.fmt != f.fmt || e.woff != f.woff || e.n != f.n || e.rows != f.rows || e.role != f.role || e.wq_bytes != f.wq_bytes || e.ws_bytes != f.ws_bytes) {
            shape_mism++
            continue
        }
        unsafe {
            if (e.wq_bytes > 0l && memcmp(addr<void?>(minted.vkblob[e.wq_off]), addr<void?>(eager.vkblob[f.wq_off]), e.wq_bytes) != 0) {
                byte_mism++
            }
            if (e.ws_bytes > 0l && memcmp(addr<void?>(minted.vkblob[e.ws_off]), addr<void?>(eager.vkblob[f.ws_off]), e.ws_bytes) != 0) {
                byte_mism++
            }
        }
        compared++
    }
    t |> equal(shape_mism, 0, "every minted entry is the eager bake's entry (fmt, offset, shape, role)")
    t |> equal(byte_mism, 0, "every minted entry's device bytes are the eager gather's, byte for byte ({compared} entries compared)")
    if (compared > 0 && minted.vkplan[0].wq_bytes > 0l) {
        let e0 = minted.vkplan[0]
        let f0 = eager.vkplan[0]
        unsafe {
            eager.vkblob[f0.wq_off] = uint8(uint(eager.vkblob[f0.wq_off]) ^ 0x5Au)
            t |> success(memcmp(addr<void?>(minted.vkblob[e0.wq_off]), addr<void?>(eager.vkblob[f0.wq_off]), e0.wq_bytes) != 0,
                "the control: one poisoned byte in the reference misses the compare")
        }
    }
    delete eager
}

def private load_cached_q8(path : string) : Model {
    var m = Model()
    with_dasllama_jobque() {
        m <- load_model_cached(path, QuantMode.q8)   // nolint:PERF030,LINT019 -- the move must sit inside with_job_que; the target is fresh-empty, so nothing drops
    }
    return <- m
}

[test]
def test_vulkan_dry_bake(tst : T?) {
    tst |> run("SmolLM2 q8: dry bake collects a role-stamped plan and the image round-trips") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!family_on(t, "llama") || !arm_on(t, "vulkan")) return
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            // the converter's probed-config path: needs the tier armed (run.das --suite image-vulkan)
            apply_box_profile_runtime()
            let probed = dlim_config_current("q8")
            if (!probed.vulkan_on) {
                t |> skip("vulkan tier not armed (DASLLAMA_GPU) - run via run.das --suite image-vulkan")
                return
            }
            if (!moe_gpu_dry_bake_arm(probed.vulkan)) {
                t |> skip("dry bake declined to arm - see the log")
                return
            }
            vulkan_bake_begin()
            var m <- load_model_(path, QuantMode.q8)
            t |> success(vulkan_bake_take(m), "the dry walk collected a bake")
            t |> success(long_length(m.vkplan) > 0l, "plan has entries ({long_length(m.vkplan)})")
            t |> success(long_length(m.vkblob) > 0l, "blob has bytes ({long_length(m.vkblob)})")
            // every entry is role-stamped and points inside the blob
            var roles_ok = true
            var bounds_ok = true
            for (e in m.vkplan) {
                roles_ok = roles_ok && e.role != int(VkBakeRole.none)
                bounds_ok = (bounds_ok && e.wq_off + e.wq_bytes <= long_length(m.vkblob)
                    && e.ws_off + e.ws_bytes <= long_length(m.vkblob))
            }
            t |> success(roles_ok, "every plan entry carries a stage role")
            t |> success(bounds_ok, "every plan entry's planes sit inside the blob")
            // the flavor image round-trips the plan verbatim
            var terr : string
            let tmp = temp_directory(terr)
            let img = path_join(empty(tmp) ? "." : tmp, "dasllama_vkbake_{ref_time_ticks()}.dlim")
            let tag = moe_gpu_bake_tag()
            t |> success(!empty(tag), "the armed tier names its bake tag")
            t |> success(save_model_image(m, img, tag, "q8"), "flavor image saves")
            var b = Model()
            t |> success(load_model_image(img, b, tag), "flavor image maps")
            t |> equal(long_length(m.vkplan), long_length(b.vkplan), "plan survives the round trip")
            t |> equal(long_length(m.vkblob), long_length(b.vkblob), "blob survives the round trip")
            delete b
            delete m
            remove(img)
        }
    }
}

[test]
def test_vulkan_inline_bake(tst : T?) {
    tst |> run("SmolLM2 q8: a cold cached load under the armed tier mints the vulkan lane ALONE - the twin streamed off the gguf inside the save, no planar file - and the trimmed lane drops the planar families") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!family_on(t, "llama") || !arm_on(t, "vulkan")) return
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            return if (!jit_model_available(t, path))
            apply_box_profile_runtime()
            let probed = dlim_config_current("q8")
            if (!probed.vulkan_on) {
                t |> skip("vulkan tier not armed (DASLLAMA_GPU) - run via run.das --suite image-vulkan")
                return
            }
            if (!moe_gpu_dry_bake_arm(probed.vulkan)) {
                t |> skip("dry bake declined to arm - see the log")
                return
            }
            let route0 = gpu_resident_route_lever()
            let trim_set0 = vulkan_trim_lever_set()
            let trim0 = vulkan_trim()
            set_gpu_resident_route(true)   // the trimmed lane's admission reads the driver's route lever
            set_vulkan_trim(false)
            let vk_tag = moe_gpu_bake_tag()
            let img_v = image_path_for(path, vk_tag)
            let img_p = image_path_for(path, "", "q8")
            remove(img_v)
            remove(img_p)
            var a <- load_cached_q8(path)
            t |> success(a.image_map != null, "the cold cached load serves an image")
            t |> success(stat(img_v).is_valid, "the vulkan lane was minted")
            t |> success(!stat(img_p).is_valid, "no planar lane was written: a vulkan-armed load mints its own lane and no other")
            t |> success(long_length(a.vkplan) > 0l, "the served image IS the vulkan lane: it carries the plan ({long_length(a.vkplan)} entries)")
            t |> success(long_length(a.vkblob) > 0l, "... and the device twin")
            t |> success(!a.planes_trimmed && long_length(a.qblob) > 0l, "with the trim off the lane keeps the planar families")
            mint_matches_eager_bake(t, a, path)
            let plain_bytes = stat(img_v).size
            delete a
            set_vulkan_trim(true)
            let trimmed_tag = moe_gpu_bake_tag()
            t |> success(trimmed_tag != vk_tag, "the trim folds into the lane's identity on the dry tier too ({trimmed_tag} against {vk_tag})")
            remove(img_v)
            var b <- load_cached_q8(path)
            set_gpu_resident_route(route0)
            restore_vulkan_trim(trim_set0, trim0)
            t |> success(b.image_map != null && b.planes_trimmed, "the trimmed lane serves, flagged trimmed")
            t |> success(long_length(b.vkplan) > 0l && long_length(b.vkblob) > 0l, "... with the plan and the twin")
            t |> equal(long_length(b.qblob), 0l, "... and no q8 planar plane")
            t |> success(long_length(b.embq) > 0l, "... the emb region packed for the CPU embed ({long_length(b.embq)} bytes)")
            t |> success(stat(img_v).size < plain_bytes, "the trimmed lane is the smaller file ({stat(img_v).size} against {plain_bytes} bytes)")
            t |> success(!stat(img_p).is_valid, "still no planar lane")
            delete b
            remove(img_v)
        }
    }
}
