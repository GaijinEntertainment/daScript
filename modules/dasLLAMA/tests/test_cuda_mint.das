options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // nolint:STYLE029 — the umbrella fires each arch [init] registration (qwen35) and the CUDA home's; no symbol of its own is read
require dasllama/dasllama_image   // image_lane_name, dlim_file_lane: the lane's names
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_env   // g_env_gpu.gpu_vram_mb: the environment's VRAM pin, which shadows the want
require ?cuda dasllama/dasllama_cuda_common   // cud_init, g_cud.cc: the card whose total memory the lane's identity reads
require daslib/fio   // base_name
require strings
require _resident_feed   // cuda_home_asked, with_gpu_kv_dtype
require _vulkan_lane   // the lane cells both homes run, the mint's frame

// The CUDA home's lane (`DASLLAMA_GPU=1 DASLLAMA_CUDA=1`), the Vulkan mint file's cells on this home: a cold load mints the `cuda`
// lane and no other - the walk the Vulkan home mints by, under this home's dry flag - the trimmed lane serves the same greedy
// tokens off the same device twin and the same CPU embed rows off the packed emb region, and maps back warm to the same tokens;
// the K-quant twin's lane mints off its native planes; and a trimmed lane minted under one K/V mirror codec maps under the others
// and serves each codec's own tokens; every lane cell pins the f16 mirror for its loads (`with_gpu_kv_dtype`). The identity cell
// holds the lane's budget word to the card's total memory less the headroom, and to the pinned figure under a VRAM pin. Skips
// when interpreted, without DASLLAMA_CUDA=1, without the model, with the image rail off (DASLLAMA_IMAGE=0), without the armed
// home, when the whole-model driver declines the model on this box, and where DASLLAMA_VK_KV32=1 arms an f32 mirror over the
// f16 the lane cells ask; the identity cell skips without a CUDA device and where DASLLAMA_GPU_VRAM_MB in the environment
// shadows the pin it sets.

//! the lane the home mints is its own, by the engine's names: the tag's lane name and the file's lane segment both read the
//! armed backend's name (`moe_gpu_backend_name`, `cuda` under the CUDA ask)
def private cuda_lane_named(t : T?; img : string) {
    t |> equal(image_lane_name(moe_gpu_bake_tag()), moe_gpu_backend_name(), "the home's bake tag names the armed backend's lane ({moe_gpu_bake_tag()})")
    t |> equal(dlim_file_lane(img), moe_gpu_backend_name(), "the lane file is the armed backend's lane ({base_name(img)})")
}

[test]
def test_cuda_mint_lanes(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: a cold CUDA-armed load mints the cuda lane alone; the trimmed lane serves the same tokens and embed rows, cold and warm") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            with_gpu_kv_dtype(t, KVDtype.f16) {
                lanes_cell(t, path) $(img) {
                    cuda_lane_named(t, img)
                }
            }
        }
    }
}

[test]
def test_cuda_mint_kquant(tst : T?) {
    tst |> run("Qwen3.5-0.8B-Q4_K_M: the K-quant twin's cuda lane mints off its native planes and maps back warm to the same tokens") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_mint_file(t, "qwen35", KQ_FILE) $(path) {
            with_gpu_kv_dtype(t, KVDtype.f16) {
                kquant_cell(t, path) $(img) {
                    cuda_lane_named(t, img)
                }
            }
        }
    }
}

[test]
def test_cuda_mint_kv_codecs(tst : T?) {
    tst |> run("Qwen3.5-0.8B-MTP: the trimmed cuda lane minted under a q8_0 mirror maps under tq4 and f16 with no re-mint, and serves at each codec the tokens the staged load serves there") @(t : T?) {
        return if (!cuda_home_asked(t))
        with_mint_file(t, "qwen35", MINT_FILE) $(path) {
            kv_codecs_cell(t, path) $(img) {
                cuda_lane_named(t, img)
            }
        }
    }
}

let private VRAM_PIN_MB = 4096l   //! the identity cell's VRAM pin: a figure no card's total less the headroom reads

//! the budget word of a bake tag (`dlim_vulkan_tag`'s ` b<mb>`), -1 where the tag has none
def private tag_budget_mb(tag : string) : int64 {
    let at = find(tag, " b")
    return -1l if (at < 0)
    let rest = slice(tag, at + 2, length(tag))
    let sp = find(rest, " ")
    return to_int64(sp < 0 ? rest : slice(rest, 0, sp), false)
}

//! the lane's identity carries the card's weight cap: unpinned, the total memory less the headroom (`cud_identity_cap`); under
//! a VRAM pin (`set_gpu_tier_want_`'s `vram_mb`), the pinned figure
[test]
def test_cuda_mint_identity_cap(tst : T?) {
    tst |> run("the cuda bake tag's budget word reads the card's total memory less the headroom unpinned, and the pinned figure under a VRAM pin of {VRAM_PIN_MB} MB") @(t : T?) {
        return if (!cuda_home_asked(t))
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (!cud_init()) {
                t |> skip("no CUDA device the home arms (the cuda tier's log names why)")
                return
            }
            let env_pin = g_env_gpu.gpu_vram_mb ?? 0l
            if (env_pin != 0l) {
                t |> skip("DASLLAMA_GPU_VRAM_MB={env_pin} in the environment shadows the pin the cell sets")
                return
            }
            let total_mb = int64(cuda_device_total_memory(g_cud.cc)) / 1_000_000l
            let want0 = g_gpu_want
            var want = want0
            want.vram_mb = 0l
            set_gpu_tier_want_(want)
            let unpinned = tag_budget_mb(moe_gpu_bake_tag())
            to_log(LOG_INFO, "cuda identity: the card's total {total_mb} MB, the unpinned tag {moe_gpu_bake_tag()}\n")
            t |> success(unpinned > 0l && unpinned < total_mb && unpinned >= total_mb - 1024l, "unpinned, the budget word reads the card's total memory ({total_mb} MB) less the headroom ({unpinned} MB)")
            want.vram_mb = VRAM_PIN_MB
            set_gpu_tier_want_(want)
            let pinned = tag_budget_mb(moe_gpu_bake_tag())
            set_gpu_tier_want_(want0)
            t |> equal(pinned, VRAM_PIN_MB, "under the VRAM pin the budget word reads the pinned figure ({moe_gpu_bake_tag()})")
            t |> success(pinned != unpinned, "control: the pin moved the identity off the card's figure ({unpinned} MB)")
        } else {
            t |> skip("no dasCUDA module on this build")
        }
    }
}
