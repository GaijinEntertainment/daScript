options gen2
options persistent_heap
options stack = 131072

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE029 — umbrella fires each arch [init] registration
require dasllama/dasllama_image
require dasllama/dasllama_config
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/fio
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
