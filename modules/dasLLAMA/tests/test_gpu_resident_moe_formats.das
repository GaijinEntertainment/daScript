options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

// The host twin's model-level matrix: Qwen1.5-MoE-A2.7B-Chat's routed experts requantized to one format a file
// (`moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-<TYPE>.gguf`, the rest Q8_0; the down stacks' 1408-wide rows take the type's
// per-32 fallback where a superblock type is asked), four routed layers' experts pinned to the host under the streamed
// split, the rare rail reading them off the tier's pinned device-layout planes through the host twin: the split token
// command and the cut window chain against the CPU chain, one window and two. A file not minted skips. Run under
// DASLLAMA_IMAGE=0: a dozen carriers would mint a dozen images beside them (the image rail's own coverage is the image
// suites'), and the planes in RAM reach the same twin.

require dastest/testing_boost public
require _moe_resident
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_env
require daslib/fio
require dasllama/dasllama_blocks

let private HOST_FIRST = 8l   //! the rarest experts the meet hands the host before the device claims: the twin's rows on every carrier

//! the cells' recipe: the streamed prompt (the split is the environment's default) with the host's share on the arena and a
//! host share the device cannot take whole, pinned here so the file carries its own shape
def private pin_recipe() {
    set_gpu_want_pf_stream(1)
    set_rare_arena(1)
    set_moe_meet_host_first(HOST_FIRST)
}

//! `body` with the meet's host counter as the witness: the cell's windows handed the host experts, so the host rail's rows
//! reached the bar off the device planes; a cell that served no window (its carrier not minted, the driver declined)
//! skipped and is not asked
def private witnessed(t : T?; body : block<() : void>) {
    let c0 = g_moe_meet_cpu_experts
    let r0 = g_rare_dev_rows
    let d0 = g_rare_dec_rows
    let p0 = moe_gpu_resident_prefills()
    invoke(body)
    return if (moe_gpu_resident_prefills() == p0)
    t |> success(g_moe_meet_cpu_experts > c0, "the meet handed the host {g_moe_meet_cpu_experts - c0} experts over the cell's windows")
    t |> success(g_rare_dev_rows > r0, "the rare rail read {g_rare_dev_rows - r0} rows off the device planes over the cell's windows - the kernels' rows are in the bar")
    t |> success(g_rare_dec_rows > d0, "the fed steps' host picks read {g_rare_dec_rows - d0} pairs off the device planes through the rail - the steps' logits are in the bar")
}

def private format_cells(t : T?; tag : string) {
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-{tag}.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
    t |> run("{tag} experts on four host layers through the one-copy rail: the split token command and the cut window chain vs the CPU chain, one window") @(ct : T?) {
        pin_recipe()
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            witnessed(ct) {
                moe_forced_feed(ct, 40l, fx)
            }
        }
    }
    t |> run("{tag} experts on four host layers through the one-copy rail: the same across two windows") @(ct : T?) {
        pin_recipe()
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            assert_pf_window(ct)
            witnessed(ct) {
                moe_forced_feed(ct, PF_WINDOW_ROWS + 88l, fx)
            }
        }
    }
}

//! one [test] a format, so a run names one carrier a process (`--test-names test_moe_formats_<TYPE>`): under DASLLAMA_IMAGE=0 a loaded model's planes stay in the heap past its delete, and four carriers in one process exhaust the box
[test]
def test_moe_formats_Q2_K(t : T?) {
    format_cells(t, "Q2_K")
}

[test]
def test_moe_formats_Q3_K(t : T?) {
    format_cells(t, "Q3_K")
}

[test]
def test_moe_formats_Q4_K(t : T?) {
    format_cells(t, "Q4_K")
}

[test]
def test_moe_formats_Q5_K(t : T?) {
    format_cells(t, "Q5_K")
}

[test]
def test_moe_formats_Q6_K(t : T?) {
    format_cells(t, "Q6_K")
}

[test]
def test_moe_formats_Q4_0(t : T?) {
    format_cells(t, "Q4_0")
}

[test]
def test_moe_formats_Q8_0(t : T?) {   //! the source file itself, linked into the matrix folder under the matrix's naming
    format_cells(t, "Q8_0")
}

[test]
def test_moe_formats_IQ4_XS(t : T?) {
    format_cells(t, "IQ4_XS")
}

[test]
def test_moe_formats_IQ4_NL(t : T?) {
    format_cells(t, "IQ4_NL")
}

[test]
def test_moe_formats_IQ3_XXS(t : T?) {
    format_cells(t, "IQ3_XXS")
}

[test]
def test_moe_formats_IQ3_S(t : T?) {
    format_cells(t, "IQ3_S")
}

[test]
def test_moe_formats_IQ2_S(t : T?) {
    format_cells(t, "IQ2_S")
}

[test]
def test_moe_formats_IQ2_XS(t : T?) {
    format_cells(t, "IQ2_XS")
}

[test]
def test_moe_formats_IQ2_XXS(t : T?) {
    format_cells(t, "IQ2_XXS")
}

[test]
def test_moe_formats_Q5_0(t : T?) {
    format_cells(t, "Q5_0")
}

[test]
def test_moe_formats_Q5_1(t : T?) {
    format_cells(t, "Q5_1")
}

[test]
def test_moe_formats_MXFP4(t : T?) {
    format_cells(t, "MXFP4")
}

//! the hot expert pool over streamed layers: twenty of the Q4_K file's routed layers on the host arm the unasked pool, its slots filled off the layers' pinned mirrors - the fed steps' picks against the CPU chain, the slot copies and the uploads the witness
[test]
def test_moe_formats_hot_pool(t : T?) {
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 20l)
    t |> run("Q4_K experts on twenty host layers with the hot pool: the slots filled off the mirrors, the token command vs the CPU chain") @(ct : T?) {
        pin_recipe()
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let c0 = g_rare_slot_copies
            let u0 = gpu_resident_hot_stats().uploads
            let p0 = moe_gpu_resident_prefills()
            witnessed(ct) {
                moe_forced_feed(ct, 40l, fx)
            }
            return if (moe_gpu_resident_prefills() == p0)
            ct |> success(gpu_resident_hot_stats().uploads > u0, "the plan armed the hot pool beside the mirror and it uploaded {gpu_resident_hot_stats().uploads - u0} experts over the cell")
            ct |> equal(g_rare_slot_copies - c0, 3l * (gpu_resident_hot_stats().uploads - u0), "every slot filled off the pinned mirrors: three plane copies an upload, no planar plane read")
        }
    }
}

//! the expert cache at half: twenty of the Q4_K file's routed layers streamed under a budget keeping about half of each layer's experts resident, the misses read off the lane into the ring - the fed steps' picks against the CPU chain, the on-demand reads the witness
[test]
def test_moe_formats_expert_cache(t : T?) {
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 20l, image = true)
    t |> run("Q4_K experts on twenty host layers under a 3500 MB expert cache: the misses read off the lane, the token command vs the CPU chain") @(ct : T?) {
        if (!g_env_engine.image) {
            ct |> skip("the expert cache reads its misses off the lane file, and DASLLAMA_IMAGE=0 mints none")
            return
        }
        pin_recipe()
        set_expert_cache_mb(3500l)
        remove("{path_join(models_dir(), fx.file)}.vulkan.dlim.heat")   //! an earlier run's profile would seat the first feed's hottest experts already
        let trim_set = vulkan_trim_lever_set()   //! the lane mints untrimmed: the cell's reference pass is the all-CPU chain, which reads the planar planes
        let trim = vulkan_trim()
        set_vulkan_trim(false)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let f0 = moe_gpu_stream_cache_stats().fetches
            let p0 = moe_gpu_resident_prefills()
            witnessed(ct) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (moe_gpu_resident_prefills() != p0) {
                let reads1 = moe_gpu_stream_cache_stats().fetches - f0
                ct |> success(reads1 > 0l, "the cache read {reads1} planes off the lane on demand over the cell")
                //! the drop wrote the picks' profile beside the lane: the same feed again keeps each layer's hottest experts resident and misses less
                let f1 = moe_gpu_stream_cache_stats().fetches
                moe_forced_feed(ct, 40l, fx)
                let reads2 = moe_gpu_stream_cache_stats().fetches - f1
                ct |> success(reads2 < reads1, "the profile-ordered cache read {reads2} planes on demand against {reads1} with the first ids resident")
            }
        }
        restore_vulkan_trim(trim_set, trim)
        set_expert_cache_mb(-1l)
    }
}
