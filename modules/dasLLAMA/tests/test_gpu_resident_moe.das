options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _moe_resident
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)

// The whole-model resident driver on a MoE (Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local, the Q4_K_M mint of
// the Q8_0 carrier, under DASLLAMA_GPU=1): the expert stacks in the arena beside the attention
// quads and the shared experts, the window chain's routed block (the router GEMM, the per-row
// select, the device bucket schedule, the f16 gather, the expert tiles, the gated combine) and the
// token command's routed block (the router GEMV, the top-k writing the expert GEMVs' slot regions,
// the combine), in _moe_resident's forced-feed form. Two prompt lengths: one window, two windows.
// Three fixtures: the twin, the Qwen3.6-35B UD-IQ2_XXS hybrid, whose recurrent layers take the routed
// block after the deltanet head, and the Qwen3-30B UD-IQ2_XXS, the MoE with no shared expert (the
// residual step's add partner off); the two UD-IQ2_XXS fixtures and the Q4_K_M host-layer fixture each hold the format census
// first - every site the file carries read in its own format. Qwen3-Coder-Next (512 experts) holds the device select past
// 256 experts beside the host-layer split. All the fixtures are large-tier (DASLLAMA_PARITY_FULL=1); the
// file is -jit only, and skips without the file or the armed tier; the driver's admission of every fixture is asserted,
// never skipped - on a device with neither the cm2 tile family nor KHR cooperative matrices at subgroup 32 (or under
// DASLLAMA_COOPMAT=sdot4) the expert pieces ride the Q8-fed chain's batch tiles.

let private TWIN = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)   //! the Q4_K_M mint of the Q8_0 carrier (its recipe row in ../performance/model_specs.das)
let private HYBRID = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)   //! the deltanet hybrid MoE with a gated shared expert, the 16 GB card's whole-model fit
let private NO_SHEXP = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, census = true)   //! the MoE with no shared expert: the routed block alone at the FFN site

[test]
def test_gpu_resident_moe(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the resident MoE prefill window + the routed token command vs the CPU chain, one window") @(t : T?) {
        moe_forced_feed(t, 40l, TWIN)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the same across two windows") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, TWIN)
    }
}

//! the per-32 mints of the Q8_0 carrier (`--pure`, their recipe rows in ../performance/model_specs.das): the Q5_1 file's every
//! plane q51 - the shared expert and the 1408-wide routed rows among them - the Q4_0 file's 1408-wide routed down rows q4032, and
//! the Q5_0 file's every plane q5032 - the routed and shared expert stacks among them
let private PER32_Q51 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q5_1-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)
let private PER32_Q4032 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_0-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)
let private PER32_Q5032 = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q5_0-pure-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, census = true)

[test]
def test_gpu_resident_moe_per32(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q5_1: the q51 shared expert and routed stacks on the device, every site in its own format, one window") @(t : T?) {
        if (!family_on(t, "qwen2moe") || !arm_on(t, "vk-per32-moe")) return
        moe_forced_feed(t, 40l, PER32_Q51)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_0: the q4032 routed down stacks on the device, every site in its own format, one window") @(t : T?) {
        if (!family_on(t, "qwen2moe") || !arm_on(t, "vk-per32-moe")) return
        moe_forced_feed(t, 40l, PER32_Q4032)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q5_0: the q5032 shared expert and routed stacks on the device, every site in its own format, one window") @(t : T?) {
        if (!family_on(t, "qwen2moe") || !arm_on(t, "vk-per32-moe")) return
        moe_forced_feed(t, 40l, PER32_Q5032)
    }
}

let private HEAT_HYBRID = MoeFixture(file = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)   //! the per-op census carrier: its per-op chain holds the routed bar

[test]
def test_gpu_perop_moe_heat(t : T?) {
    t |> run("Qwen3.6-35B-A3B IQ4_XS on the per-op tier: twenty routed layers placed, twenty streamed under the heat pool, every layer's shared expert on the device after the routed chain or the pool's hits, against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen35moe")) return
        moe_perop_heat_cell(t, HEAT_HYBRID, 20l, 16l)
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q5_1 on the per-op tier: twelve routed layers placed, twelve streamed under the heat pool's q51 slots, the q51 shared expert on the device after them, against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen2moe")) return
        moe_perop_heat_cell(t, PER32_Q51, 12l, 8l)
    }
}

//! the twin under tq4: the device's summed distance from the f16 chain reads 1.06 of the CPU tq4 chain's own (14.7 against
//! 13.9, the f16 rows one step off 5.0 times it) and the two tq4 chains 1.11 of it apart (the CPU tq4 rows one step off 5.0):
//! the router's near-ties part two tq4 chains as far as tq4 parts either from f16. In the forced-feed form the same rows read
//! 1.4-1.9 against bars of 2.1-2.3
let private MOE_CODEC_DIST_REL = 1.15
let private MOE_CODEC_PAIR_REL = 1.4

[test]
def test_gpu_resident_moe_block_codecs(t : T?) {
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the routed chain over a q8_0 K/V mirror vs the CPU chain on q8_0 sessions, one window") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.q8_0) {
            moe_forced_feed(t, 40l, TWIN)
        }
    }
    t |> run("Qwen1.5-MoE-A2.7B-Q4_K_M: the routed chain over a tq4 mirror - the device, the CPU tq4 chain and the CPU f16 chain over one prompt, the distances held as ratios") @(t : T?) {
        with_gpu_kv_dtype(t, KVDtype.tq4) {
            moe_codec_distance(t, 40l, TWIN, KVDtype.tq4, MOE_CODEC_DIST_REL, MOE_CODEC_PAIR_REL)
        }
    }
}

[test]
def test_gpu_resident_moe_hybrid(t : T?) {
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS: the deltanet hybrid MoE - the recurrent head beside the routed block, one window") @(t : T?) {
        moe_forced_feed(t, 40l, HYBRID)
    }
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS: the same across two windows (the conv tail and the state carry the seam)") @(t : T?) {
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, HYBRID)
    }
}

[test]
def test_gpu_resident_moe_no_shexp(t : T?) {
    t |> run("Qwen3-30B-A3B-UD-IQ2_XXS: the MoE with no shared expert - the routed block alone at the FFN site, one window") @(t : T?) {
        moe_forced_feed(t, 40l, NO_SHEXP)
    }
    t |> run("Qwen3-30B-A3B-UD-IQ2_XXS: the same across two windows") @(t : T?) {
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, NO_SHEXP)
    }
}

//! 32768 positions asked whole: a 16 GB card cannot hold every expert stack of this 17 GB file beside the mirror, so the first routed layers' go to the host
let private HOST_LAYERS = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 32768l, census = true)

//! 131072 positions asked whole: on a 16 GB card the mirror leaves room for the dense planes and most of the expert stacks, so the first routed layers' go to the host
let private HOST_CTX = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 131072l, census = true)

//! the 512-expert qwen3next hybrid (48.5 GB): no card the rig stocks holds its expert stacks whole, so the driver arms with the
//! first routed layers' experts on the host at a context asked whole
let private CODER_NEXT = MoeFixture(file = "Qwen3-Coder-Next-Q4_K_M.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 8192l)

[test]
def test_gpu_resident_moe_512_experts(t : T?) {
    t |> run("Qwen3-Coder-Next-Q4_K_M: the 512-expert hybrid on the whole-model driver - the device select past 256 experts, the host-layer split, one window against the CPU chain") @(t : T?) {
        moe_forced_feed(t, 40l, CODER_NEXT)
    }
}

[test]
def test_gpu_resident_moe_host_layers(t : T?) {
    t |> run("Qwen3-30B-A3B-Q4_K_M: the experts the card does not hold serve on the host - the split token command and the cut window chain vs the CPU chain, one window") @(t : T?) {
        moe_forced_feed(t, 40l, HOST_LAYERS)
    }
    t |> run("Qwen3-30B-A3B-Q4_K_M: the same across two windows") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, HOST_LAYERS)
    }
}

[test]
def test_gpu_resident_moe_ctx_strict(t : T?) {
    t |> run("Qwen3.6-35B-A3B-UD-IQ2_XXS at a context asked whole (131072): the experts yield to the mirror and serve on the host, the hybrid chain against the CPU over two windows") @(t : T?) {
        assert_pf_window(t)
        moe_forced_feed(t, PF_WINDOW_ROWS + 88l, HOST_CTX)
    }
}

//! a two-token prompt on a MoE whose experts serve on the host takes the window chain: the token command's peel declines the host experts
[test]
def test_gpu_resident_moe_host_layers_short_window(t : T?) {
    t |> run("Qwen3-30B-A3B-Q4_K_M with host layers: a two-token prompt takes the window chain, not the token command's peel") @(t : T?) {
        let path = path_join(models_dir(), HOST_LAYERS.file)
        if (!resident_ready(t, path)) return
        var tr <- load_resident(path, 1l, HOST_LAYERS.strict_ctx)
        if (moe_fixture_admitted(t, tr, HOST_LAYERS) > 0l) {
            with_dasllama_jobque_() {
                assert_short_window_takes_chain(t, tr, "routed experts on the host")
            }
        }
        delete tr
    }
}
