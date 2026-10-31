options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

// The host twin's model-level matrix: Qwen1.5-MoE-A2.7B-Chat's routed experts requantized to one format a file
// (`moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-<TYPE>.gguf`, the rest Q8_0; the down stacks' 1408-wide rows take the type's
// per-32 fallback where a superblock type is asked), four routed layers' experts pinned to the host under the streamed
// split, the rare rail reading them off the tier's pinned device-layout planes through the host twin: the split token
// command and the cut window chain against the CPU chain, one window and two; the pool's two knobs (the DMA fill, the
// decode meet) and the rare rail's three A/B arms off, each pinned in process on a carrier of the matrix; the NVFP4 row
// runs the same two cells on Qwen3.6-35B-A3B's own NVFP4 file (large tier). Skips: a file not
// minted (the 35B NVFP4 not stocked - it is downloaded, not minted), DASLLAMA_PARITY_FULL unset for the large tier, no armed
// Vulkan tier, the knob cells where the plan found no room for the pool, the expert-cache cell under
// DASLLAMA_IMAGE=0 (it reads the lane). Run the format cells under DASLLAMA_IMAGE=0: a dozen carriers would mint a dozen
// images beside them (the image rail's own coverage is the image suites'), and the planes in RAM reach the same kernels.

require dastest/testing_boost public
require _moe_resident
require dasllama/dasllama_kv_dtype
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_env
require daslib/fio
require dasllama/dasllama_blocks

let private HOST_FIRST = 8l   //! the rarest experts the meet hands the host before the device claims: the twin's rows on every carrier
let private QWEN15_MOE_EXPERTS = 60l   //! the carrier's routed experts a layer (Qwen1.5-MoE-A2.7B): a pool at or past it never fills
let private SHARE_CELL_SLOTS = 16l     //! the share cell's pool: a quarter of the experts, so a step's misses outrun the free slots

//! the cells' recipe: the streamed prompt (the split is the environment's default) with the host's share on the arena and a
//! host share the device cannot take whole, pinned here so the file carries its own shape; the levers as the cell found them
//! come back (`stream_pins`), for `restore_stream_pins`
def private pin_recipe() : StreamPins {
    let was = stream_pins()
    set_gpu_want_pf_stream(1)
    set_rare_arena(1)
    set_moe_host_first(HOST_FIRST)
    return was
}

//! `body` with the meet's host counter as the witness: the cell's windows handed the host experts; under `dev_rows` the
//! one-pass rail read them off the device planes (the three-pass rail reads the planar planes); under `one_row_rail` the fed
//! steps' picks went through the one-row rail (a decode-meet cell routes them through the window's meet instead); a cell that
//! served no window (its carrier not minted, the driver declined) skipped and is not asked
def private witnessed(t : T?; one_row_rail, dev_rows : bool; body : block<() : void>) {
    let c0 = g_moe_meet_cpu_experts
    let r0 = g_rare_dev_rows
    let d0 = g_rare_dec_rows
    let p0 = gpu_resident_prefills_()
    invoke(body)
    return if (gpu_resident_prefills_() == p0)
    t |> success(g_moe_meet_cpu_experts > c0, "the meet handed the host {g_moe_meet_cpu_experts - c0} experts over the cell's windows")
    if (dev_rows) {
        t |> success(g_rare_dev_rows > r0, "the rare rail read {g_rare_dev_rows - r0} rows off the device planes over the cell's windows - the kernels' rows are in the bar")
    }
    if (one_row_rail) {
        t |> success(g_rare_dec_rows > d0, "the fed steps' host picks read {g_rare_dec_rows - d0} pairs off the device planes through the rail - the steps' logits are in the bar")
    }
}

//! after a feed: false after the skip where the plan sized no pool beside the mirror; the pool's counters reset at the arm
//! and outlive the drop, so a pool that served the feed counted its picks
def private pool_armed(t : T?) : bool {
    if (gpu_resident_hot_stats().picks == 0l) {
        t |> skip("the plan found no room for the pool beside the mirror on this card")
        return false
    }
    return true
}

//! false after the skip where the environment asks no hot pool (the plan then sizes none, whatever the card's room)
def private pool_asked(t : T?) : bool {
    if (g_env_gpu.gpu_heat ?? 1l == 0l) {
        t |> skip("DASLLAMA_GPU_HEAT=0 asks no hot expert pool - the cell needs that unset")
        return false
    }
    return true
}

[cold_path]
def private format_cells(t : T?; tag : string) {
    format_cells_of(t, tag, MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-{tag}.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l))
}

//! the matrix's two cells over any host-pinned carrier: `tag` names the expert format in the cell's title
def private format_cells_of(t : T?; tag : string; fx : MoeFixture) {
    t |> run("{tag} experts on at least four host layers through the one-copy rail: the split token command and the cut window chain vs the CPU chain, one window") @(ct : T?) {
        let pins = pin_recipe()
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            witnessed(ct, true, true) {
                moe_forced_feed(ct, 40l, fx)
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("{tag} experts on at least four host layers through the one-copy rail: the same across two windows") @(ct : T?) {
        let pins = pin_recipe()
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            if (assert_pf_window(ct)) {
                witnessed(ct, true, true) {
                    moe_forced_feed(ct, PF_WINDOW_ROWS + 88l, fx)
                }
            }
        }
        restore_stream_pins(pins)
    }
}

//! the pool's two knobs pinned on in process, one carrier a family of the matrix: the DMA fill (a token's misses copied into
//! the pool on the transfer queue, no host gather) and the decode meet (a token's picks claimed through the prefill's meet) -
//! the fed steps' picks against the CPU chain at the matrix's bar, each knob's own counter the witness that it ran
def private knob_cells(t : T?; tag : string) {
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-{tag}.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 20l)
    // the share's cell goes first: the pool outlives a cell on the same carrier, and it is armed at the slot ask this cell pins -
    // fewer slots than the carrier's experts, so the pool fills and the heat gate has misses to leave the share
    t |> run("{tag} experts on twenty host layers, the fill's share of a step's misses past the heat gate: the token command vs the CPU chain") @(ct : T?) {
        return if (!pool_asked(ct))
        let pins = pin_recipe()
        let heat0 = g_gpu_want.heat
        g_gpu_want.heat = SHARE_CELL_SLOTS
        set_gpu_hot_dma(1)
        set_gpu_hot_share(1.0)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let p0 = gpu_resident_prefills_()
            let s0 = gpu_resident_hot_stats()
            witnessed(ct, false, true) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0 && pool_armed(ct)) {
                let s1 = gpu_resident_hot_stats()
                ct |> success(s1.dma > s0.dma, "the fill landed {s1.dma - s0.dma} experts by DMA over the cell")
                if (gpu_resident_hot_slots() >= QWEN15_MOE_EXPERTS) {
                    ct |> skip("the pool holds every expert of the carrier ({gpu_resident_hot_slots()} slots): it never fills, and the heat gate leaves the share nothing - the pool was armed before this cell's slot pin")
                } else {
                    ct |> success(s1.share_seen > s0.share_seen, "the heat gate left the share misses to admit ({s1.share_seen - s0.share_seen}) with {gpu_resident_hot_slots()} slots over {QWEN15_MOE_EXPERTS} experts")
                    ct |> equal(s1.share_fills - s0.share_fills, s1.share_seen - s0.share_seen, "at share 1 every miss the heat gate left ({s1.share_seen - s0.share_seen}) was admitted - every expert of the carrier is a copy the tier can make")
                }
            }
        }
        g_gpu_want.heat = heat0
        restore_stream_pins(pins)
    }
    t |> run("{tag} experts on twenty host layers, the pool's misses filled by DMA: the token command vs the CPU chain") @(ct : T?) {
        return if (!pool_asked(ct))
        let pins = pin_recipe()
        set_gpu_hot_dma(1)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let p0 = gpu_resident_prefills_()
            witnessed(ct, false, true) {   //! the misses land in the pool by DMA, so the one-row rail reads none of the steps' picks
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0 && pool_armed(ct)) {
                ct |> success(gpu_resident_hot_stats().dma > 0l, "the DMA fill landed {gpu_resident_hot_stats().dma} experts in the pool over the cell")
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("{tag} experts on twenty host layers, the DMA fill off and the meet off: no expert lands by DMA, the token steps' misses take the host's upload beside the window's placements") @(ct : T?) {
        return if (!pool_asked(ct))
        let pins = pin_recipe()
        set_gpu_hot_dma(0)   // the DMA cell's no-route leg (the meet off too: its claims ride DMA whatever the knob says)
        set_gpu_dec_meet(0)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let p0 = gpu_resident_prefills_()
            witnessed(ct, false, true) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0 && pool_armed(ct)) {
                // the pool's counters are the armed driver's, zeroed at its arm inside the feed's load: absolute reads
                ct |> equal(gpu_resident_hot_stats().dma, 0l, "control: with the fill off no expert landed by DMA over the cell")
                ct |> success(gpu_resident_hot_stats().uploads > 0l, "the window's placements and the steps' misses took the host's upload ({gpu_resident_hot_stats().uploads})")
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("{tag} experts on twenty host layers, a token's picks through the decode meet: the token command vs the CPU chain") @(ct : T?) {
        let pins = pin_recipe()
        set_gpu_dec_meet(1)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let p0 = gpu_resident_prefills_()
            witnessed(ct, false, true) {   // the meet replaces the one-row rail
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0) {
                ct |> success(gpu_resident_hot_stats().meet_steps > 0l, "the decode meet claimed {gpu_resident_hot_stats().meet_steps} token steps' picks over the cell")
            }
        }
        restore_stream_pins(pins)
    }
}

[test]
def test_moe_formats_knobs_Q8_0(t : T?) {
    knob_cells(t, "Q8_0")
}

[test]
def test_moe_formats_knobs_Q4_K(t : T?) {
    knob_cells(t, "Q4_K")
}

[test]
def test_moe_formats_knobs_IQ4_XS(t : T?) {
    knob_cells(t, "IQ4_XS")
}

//! the rare rail's three A/B arms pinned off on the Q4_K file, one cell each: the three region-list passes in place of the
//! one-pass lane, the rows core a row in place of the batched cores, and the planar planes in place of the arena - the same
//! compare at the matrix's bar, the arm's counter the witness
[test]
def test_moe_formats_rail_arms(t : T?) {
    let fx = MoeFixture(file = "moe-formats/Qwen1.5-MoE-A2.7B-Chat-exps-Q4_K.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l)
    t |> run("Q4_K experts on four host layers through the three-pass rail (DASLLAMA_RARE_ONEPASS=0): the token command vs the CPU chain") @(ct : T?) {
        let pins = pin_recipe()
        set_rare_onepass(0)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            witnessed(ct, true, false) {
                moe_forced_feed(ct, 40l, fx)
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("Q4_K experts on four host layers, the rows core a row (DASLLAMA_RARE_BATCHED=0): the token command vs the CPU chain") @(ct : T?) {
        let pins = pin_recipe()
        set_rare_batched(0)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            witnessed(ct, true, true) {
                moe_forced_feed(ct, 40l, fx)
            }
        }
        restore_stream_pins(pins)
    }
    t |> run("Q4_K experts on four host layers off the planar planes (DASLLAMA_RARE_ARENA=0): the token command vs the CPU chain") @(ct : T?) {
        let pins = pin_recipe()
        set_rare_arena(0)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let r0 = g_rare_dev_rows
            let p0 = gpu_resident_prefills_()
            moe_forced_feed(ct, 40l, fx)
            if (gpu_resident_prefills_() != p0) {
                ct |> equal(g_rare_dev_rows, r0, "the arena off: the rail read no row off the device planes")
            }
        }
        restore_stream_pins(pins)
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

//! the carrier is the 35B NVFP4 file itself (large tier): its routed stacks reach the nv4 row-major kernels under the per-tensor scale word
[test]
def test_moe_formats_NVFP4_35B(t : T?) {
    //! the census binds the file's nv4 sites; its BF16 deltanet planes, table and classifier serve as q8 by the tier's design
    format_cells_of(t, "NVFP4 (Qwen3.6-35B-A3B)", MoeFixture(file = "Qwen3.6-35B-A3B-NVFP4.gguf", shexp = true, census = true,
        census_sites = "attention q/k/v/o;routed experts;shared expert", bar_rel = MOE_LOGIT_BAR_REL, host_layers_ask = 4l))
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
        return if (!pool_asked(ct))
        let pins = pin_recipe()
        set_gpu_hot_dma(0)   //! the host fill is the cell's subject: the DMA fill is `knob_cells`'
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let c0 = g_rare_slot_copies
            let p0 = gpu_resident_prefills_()
            witnessed(ct, true, true) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0 && pool_armed(ct)) {
                ct |> success(gpu_resident_hot_stats().uploads > 0l, "the plan armed the hot pool beside the mirror and it uploaded {gpu_resident_hot_stats().uploads} experts over the cell")
                ct |> equal(g_rare_slot_copies - c0, 3l * gpu_resident_hot_stats().uploads, "every slot filled off the pinned mirrors: three plane copies an upload, no planar plane read")
            }
        }
        restore_stream_pins(pins)
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
        let pins = pin_recipe()
        let cache_was = expert_cache_mb_pin()
        set_expert_cache_mb(3500l)
        remove("{path_join(models_dir(), fx.file)}.vulkan.dlim.heat")   //! an earlier run's profile would seat the first feed's hottest experts already
        let trim_set = vulkan_trim_lever_set()   //! the lane mints untrimmed: the cell's reference pass is the all-CPU chain, which reads the planar planes
        let trim = vulkan_trim()
        set_vulkan_trim(false)
        with_gpu_kv_dtype(ct, KVDtype.f16) {
            let f0 = moe_gpu_stream_cache_stats().fetches
            let p0 = gpu_resident_prefills_()
            witnessed(ct, true, true) {
                moe_forced_feed(ct, 40l, fx)
            }
            if (gpu_resident_prefills_() != p0) {
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
        set_expert_cache_mb(cache_was)
        restore_stream_pins(pins)
    }
}
