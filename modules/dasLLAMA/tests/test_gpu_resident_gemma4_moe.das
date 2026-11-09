options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require _gemma_resident

// gemma-4-26B-A4B UD-IQ3_XXS (the routed MoE with its parallel dense shared expert and the routed branch's own norms),
// large tier: arm `g4m`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without
// the file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf", arm = "g4m")
//! the routed model's argmax slack: a near-tie at the router flips whole positions between the arms, and the CPU chain
//! sits farther from the reference exe on this model than the resident does (its own forced-feed logits are no instrument
//! here); the hits read 126 to 139 against the CPU chain's 133 across kernel recompiles (the readings: CLAUDE.md)
let private GEMMA4_26B_HITS_SLACK = 12l

[test(description = "On the whole-model driver the routed gemma-4-26B UD-IQ3_XXS scores teacher-forced perplexity within 2x of the CPU chain's over one and two windows, hits within 12, and serves every site in its own format with every call on the device")]
def test_gpu_resident_gemma4_26b(t : T?) {
    t |> run("gemma-4-26B-A4B IQ3_XXS: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the routed block's gemma-4 form") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_FILE, GEMMA4_26B_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B IQ3_XXS: every site in its own format - the expert stacks and the token table among them - and the driver serving every call") @(t : T?) {
        gemma_format_cell(t, GEMMA4_26B_FILE, false)
    }
}

// gemma-4-26B-A4B Q4_K_M (the routed MoE in the model table's official quant: k4 gate and up expert stacks beside native
// Q5_1 down stacks - the q51 expert rail - with the dense triple and the attention head on the K-quant rails), large
// tier: arm `g4mk`. Skips off the JIT, without DASLLAMA_GPU=1 on a device the whole-model driver serves, without the
// file under models_dir(), or when the residency plan does not fit the card (DASLLAMA_GPU_MIN_CTX shortens the mirror).
let private GEMMA4_26B_K_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-Q4_K_M.gguf", arm = "g4mk")
//! the routed model's argmax slack: a router near-tie flips whole positions between the arms, and a kernel recompile alone
//! moves the hits - 122 to 128 against the CPU chain's 134 over the 150-position cell (the readings: CLAUDE.md)
let private GEMMA4_26B_K_HITS_SLACK = 16l
//! the two-window cell's band: a k4 fold's rounding pattern alone moved it 1.70 -> 2.44 against the CPU chain's 1.03 (a router near-tie flips whole positions), the 150 + 150 cell 5.51 -> 5.91 against 3.36
let private GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS = 2.5

[test(description = "On the whole-model driver gemma-4-26B Q4_K_M with q51 down experts scores teacher-forced perplexity within 2x of the CPU chain over 150 positions and 2.5x over two windows, hits within 16, and serves every site in its own format")]
def test_gpu_resident_gemma4_26b_k(t : T?) {
    t |> run("gemma-4-26B-A4B Q4_K_M: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the q51 down experts on their own rail") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
    t |> run("gemma-4-26B-A4B Q4_K_M: 520 prose ids prefilled (two windows), 80 teacher-forced, scored as a perplexity against the CPU chain") @(t : T?) {
        assert_pf_window(t)
        ppl_compare(t, 520l, 80l, GEMMA4_26B_K_FILE, GEMMA4_26B_K_HITS_SLACK, GEMMA4_26B_K_PPL_RATIO_TWO_WINDOWS)
    }
    t |> run("gemma-4-26B-A4B Q4_K_M: every site in its own format - the expert stacks and the token table among them - and the driver serving every call") @(t : T?) {
        gemma_format_cell(t, GEMMA4_26B_K_FILE, false)
    }
}

// gemma-4-26B-A4B MXFP4_MOE (the Q4_K_M file requantized, `../performance/model_specs.das`: the fused gate_up and the down
// expert stacks MXFP4 on disk, every other plane the source's), large tier: arm `mx26`. The expert stacks load as the
// native-MXFP4 planes under gemma-4's GeGLU and per-expert down scale. Skips off the JIT, without DASLLAMA_GPU=1 on a device
// the whole-model driver serves, without the file under models_dir(), or when the residency plan does not fit the card.
let private GEMMA4_26B_MX4_FILE = GemmaFile(file = "gemma-4-26B-A4B-it-MXFP4_MOE-q4km-local.gguf", arm = "mx26")
//! the routed model's argmax slack: the IQ3_XXS file's (a router near-tie flips whole positions between the arms)
let private GEMMA4_26B_MX4_HITS_SLACK = 12l

[test(description = "On the whole-model driver gemma-4-26B MXFP4_MOE serves every site in its own format, the native-MXFP4 expert stacks dispatched on the mx4 stamps, with the driver serving every call")]
def test_gpu_resident_gemma4_26b_mx4_format(t : T?) {
    t |> run("gemma-4-26B-A4B MXFP4_MOE: every site in its own format - the native-MXFP4 expert stacks among them, on the mx4 stamps - and the driver serving every call") @(t : T?) {
        gemma_format_cell(t, GEMMA4_26B_MX4_FILE, true)
    }
}

[test(description = "On the whole-model driver gemma-4-26B MXFP4_MOE's native-MXFP4 routed rail scores a 150-position teacher-forced perplexity within 2x of the CPU chain's, hits within 12")]
def test_gpu_resident_gemma4_26b_mx4_ppl(t : T?) {
    t |> run("gemma-4-26B-A4B MXFP4_MOE: 150 prose ids prefilled, 150 teacher-forced, scored as a perplexity against the CPU chain - the native-MXFP4 routed rail") @(t : T?) {
        ppl_compare(t, 150l, 150l, GEMMA4_26B_MX4_FILE, GEMMA4_26B_MX4_HITS_SLACK, GEMMA_MOE_PPL_RATIO)
    }
}
