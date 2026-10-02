options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_kv_dtype   // KVDtype: the block-codec cells' mirror ask
require _moe_resident

// The whole-model resident driver on a MoE (Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local, the Q4_K_M mint of
// the Q8_0 carrier, under DASLLAMA_GPU=1): the expert stacks in the arena beside the attention
// quads and the shared experts, the window chain's routed block (the router GEMM, the per-row
// select, the device bucket schedule, the f16 gather, the expert tiles, the gated combine) and the
// token command's routed block (the router GEMV, the top-k writing the expert GEMVs' slot regions,
// the combine), in _moe_resident's forced-feed form. Two prompt lengths: one window, two windows.
// Three fixtures: the twin, the Qwen3.6-35B UD-IQ2_XXS hybrid, whose recurrent layers take the routed
// block after the deltanet head, and the Qwen3-30B UD-IQ2_XXS, the MoE with no shared expert (the
// residual step's add partner off). All three fixtures are large-tier (DASLLAMA_PARITY_FULL=1); the
// file is -jit only, and skips without the file, the armed tier, or a device with neither the cm2 tile family
// nor KHR cooperative matrices at subgroup 32 (the expert pieces' two tiles); on a device with either the
// driver's admission of every fixture is asserted, never skipped.

let private TWIN = MoeFixture(file = "Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)   //! the Q4_K_M mint of the Q8_0 carrier (its recipe row in ../performance/model_specs.das)
let private HYBRID = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)   //! the deltanet hybrid MoE with a gated shared expert, the 16 GB card's whole-model fit
let private NO_SHEXP = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL)   //! the MoE with no shared expert: the routed block alone at the FFN site

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
let private HOST_LAYERS = MoeFixture(file = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", shexp = false, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 32768l)

//! 131072 positions asked whole: on a 16 GB card the mirror leaves room for the dense planes and most of the expert stacks, so the first routed layers' go to the host
let private HOST_CTX = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL, expect_host_layers = true, strict_ctx = 131072l)

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
