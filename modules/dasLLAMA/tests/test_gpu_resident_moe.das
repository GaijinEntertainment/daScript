options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
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
