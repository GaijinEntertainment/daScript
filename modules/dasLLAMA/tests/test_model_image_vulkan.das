options gen2
options persistent_heap
options stack = 131072

require dastest/testing_boost public
// STYLE030 nolints: the whole test body is static_if'd on builtin_module_exists(vulkan), so a
// no-vulkan build compiles it away and every body-only require reads as unused
require dasllama/dasllama   // nolint:STYLE029,STYLE030 — umbrella fires each arch [init] registration
require dasllama/dasllama_image   // nolint:STYLE030
require dasllama/dasllama_config   // nolint:STYLE030
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/fio   // nolint:STYLE030
require _model_tier

// The OFFLINE vulkan bake (plan-as-data): the dry tier runs the load walk's accept/refuse
// arithmetic against the config's numbers with NO device — the converter's `-f vulkan` rail.
// Drive through run.das --suite image-vulkan (it arms DASLLAMA_GPU + a small VRAM budget);
// unarmed contexts skip cleanly, so the file is safe in any suite.

[test]
def test_vulkan_dry_bake(tst : T?) {
    tst |> run("SmolLM2 q8: dry bake collects a role-stamped plan and the image round-trips") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!family_on(t, "llama")) return
            if (!arm_on(t, "vulkan")) {
                return
            }
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            // the converter's probed-config path: needs the tier armed (run.das --suite image-vulkan)
            apply_box_profile_runtime()
            let probed = dlim_config_current("q8")
            if (!probed.vulkan_on) {
                t |> skip("vulkan tier not armed (DASLLAMA_GPU) — run via run.das --suite image-vulkan")
                return
            }
            if (!moe_gpu_dry_bake_arm(probed.vulkan)) {
                t |> skip("dry bake declined to arm — see the log")
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
    tst |> run("SmolLM2 q8: a cold cached load bakes the vulkan flavor beside a PLANAR image") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!family_on(t, "llama")) return
            if (!arm_on(t, "vulkan")) {
                return
            }
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
            if (!model_available(t, path)) {
                return
            }
            apply_box_profile_runtime()
            let probed = dlim_config_current("q8")
            if (!probed.vulkan_on) {
                t |> skip("vulkan tier not armed (DASLLAMA_GPU) — run via run.das --suite image-vulkan")
                return
            }
            if (!moe_gpu_dry_bake_arm(probed.vulkan)) {
                t |> skip("dry bake declined to arm — see the log")
                return
            }
            // the INLINE bake: load_model_cached mints both flavors off one cold load. The
            // ordering matters — the vulkan flavor carries the collected planes, the planar one
            // must not, and the load itself has to come back on the image rail like any other.
            let vk_tag = moe_gpu_bake_tag()
            let img_v = image_path_for(path, vk_tag)
            let img_p = image_path_for(path, "", "q8")
            remove(img_v)
            remove(img_p)
            var a = Model()
            with_job_que() {
                a <- load_model_cached(path, QuantMode.q8)
            }
            t |> success(a.image_map != null, "the cold cached load serves an image")
            t |> success(stat(img_v).is_valid, "the vulkan flavor was baked")
            t |> success(stat(img_p).is_valid, "the planar image was written")
            t |> equal(long_length(a.vkblob), 0l, "the served planar image carries no bake blob")
            delete a
            // and the flavor file is the real thing, not an empty shell
            var v = Model()
            t |> success(load_model_image(img_v, v, vk_tag), "the baked flavor maps")
            t |> success(long_length(v.vkplan) > 0l, "the baked flavor kept its plan ({long_length(v.vkplan)})")
            t |> success(long_length(v.vkblob) > 0l, "the baked flavor kept its blob")
            delete v
            remove(img_v)
        }
    }
}
